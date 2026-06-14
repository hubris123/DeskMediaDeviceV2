/*
 * video_player.c - MJPEG video player for ESP32-P4 DeskMediaDevice
 *
 * Key findings from testing:
 *   - dummy_draw_blit uses physical portrait coords (480x800)
 *   - HW JPEG decoder writes directly into panel DMA framebuffer (internal RAM)
 *   - DMA2D cannot read from PSRAM — always decode into internal RAM
 *   - Color order: JPEG_DEC_RGB_ELEMENT_ORDER_BGR with JPEG_DECODE_OUT_FORMAT_RGB565
 *
 * File encoding:
 *   Video: ffmpeg -i input.mp4 -c:v mjpeg -q:v 5 -vf "transpose=1,scale=480:800" -r 20 "01 - Title.mjpeg"
 *   Audio: ffmpeg -i input.mp4 -ac 2 -ar 32000 -ab 128k -codec:a libmp3lame "01 - Title.mp3"
 *   Place both in /sdcard/video/ with matching base names
 *
 * Bumper (brand intro):
 *   /sdcard/video/bumper.mjpeg + bumper.mp3
 *   Plays on every skull tap while the real video loads in the background.
 *   Same encoding as regular videos — no adelay needed on audio.
 *   Keep under 5MB so it fits in its own PSRAM slot.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "esp_random.h"
#include "esp_timer.h"
#include "esp_cache.h"
#include "esp_private/esp_cache_private.h"
#include "esp_lcd_panel_ops.h"
#include "driver/jpeg_decode.h"
#include "bsp/esp-bsp.h"
#include "bsp/display.h"
#include "esp_lv_adapter.h"
#include "lvgl.h"

#include "video_player.h"
#include "power_gate.h"

static const char *TAG = "VideoPlayer";

#define VIDEO_DIR           "/sdcard/video"
#define BUMPER_MJPEG        "/sdcard/video/bumper.mjpeg"
#define BUMPER_MP3          "/sdcard/video/bumper.mp3"
#define VIDEO_MAX_TRACKS    32
#define FRAME_INTERVAL_MS   50

// ── Streaming pipeline ───────────────────────────────────────────────────────
// A producer task reads the clip from SD into RING_BUF; the consumer (the video
// task) pulls complete MJPEG frames and decodes them paced to 20 fps. Read and
// decode overlap, so playback starts from a prefilled buffer and never stalls,
// for any clip length. Both tasks are pinned to PIPE_CORE so the shared PSRAM
// ring needs no cross-core cache-coherency handling.
#define RING_SIZE      (8 * 1024 * 1024)   // streaming ring buffer (PSRAM)
#define FRAME_ASM_SIZE (1 * 1024 * 1024)   // scratch for a frame that wraps the ring
#define PIPE_CHUNK     (256 * 1024)        // producer SD read size
#define PIPE_PREFILL   (RING_SIZE / 2)     // start consuming once this much is buffered
#define PIPE_CORE      1

#define DISP_W  BSP_LCD_H_RES   // 480
#define DISP_H  BSP_LCD_V_RES   // 800

static char  video_playlist[VIDEO_MAX_TRACKS][256];
static int   video_track_count   = 0;
static int   video_current_idx   = 0;

static uint8_t *ring_buf  = NULL;   // streaming ring (RING_SIZE)
static uint8_t *frame_asm = NULL;   // wrapped-frame reassembly (FRAME_ASM_SIZE)

// Lock-free single-producer/single-consumer ring: free-running byte counts
// (won't wrap in practice). Producer owns s_wr, consumer owns s_rd; both tasks
// pinned to one core, so [s_rd, s_wr) is a stable, coherent readable region.
static volatile uint64_t s_wr = 0;
static volatile uint64_t s_rd = 0;
static volatile bool s_prod_eof     = false;
static volatile bool s_prod_stop    = false;
static volatile bool s_prod_running = false;
static FILE *s_prod_file = NULL;

// Bumper — kept resident in a right-sized PSRAM slot, loaded once at init
static uint8_t *bumper_buf      = NULL;
static size_t   bumper_buf_len  = 0;
static bool     bumper_available = false;

static void  *decode_buf      = NULL;
static size_t decode_buf_size = 0;

static jpeg_decoder_handle_t jpeg_handle = NULL;

static void *lcd_fb[CONFIG_BSP_LCD_DPI_BUFFER_NUMS];
static int   fb_idx = 0;

static bool video_player_running = false;

static void video_player_task(void *arg);
static void video_cleanup_task(void *arg);
static void video_producer_task(void *arg);
static void fade_overlay_anim_cb(void *obj, int32_t v);
static void trigger_fade_in(void *arg);
static size_t render_first_frame(const uint8_t *mjpeg_data, size_t mjpeg_len, jpeg_decode_cfg_t *dec_cfg);
static void play_mjpeg_from(const uint8_t *mjpeg_data, size_t mjpeg_len, jpeg_decode_cfg_t *dec_cfg, size_t start_offset);
static uint8_t *load_mjpeg_alloc(const char *path, size_t *out_len);
static void shuffle_playlist(void);
static int  find_frame(const uint8_t *buf, size_t buf_len, size_t offset,
                       size_t *frame_start, size_t *frame_len);
static bool video_pipeline_start(const char *path);
static void video_pipeline_run(const char *mp3_path, jpeg_decode_cfg_t *dec_cfg);

extern void video_mp3_play(const char *path);
extern void video_mp3_stop(void);

// ── Load an MJPEG file into a freshly-allocated, right-sized PSRAM buffer ─────
// Used for the resident bumper (was a fixed 5 MB slot for an ~1.8 MB file).
static uint8_t *load_mjpeg_alloc(const char *path, size_t *out_len)
{
    FILE *f = fopen(path, "rb");
    if (!f) { ESP_LOGW(TAG, "Cannot open %s", path); return NULL; }
    fseek(f, 0, SEEK_END);
    size_t sz = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz == 0) { fclose(f); return NULL; }
    uint8_t *buf = heap_caps_malloc(sz, MALLOC_CAP_SPIRAM);
    if (!buf) {
        fclose(f);
        ESP_LOGE(TAG, "alloc %u KB for %s failed", (unsigned)(sz / 1024), path);
        return NULL;
    }
    size_t rd = fread(buf, 1, sz, f);
    fclose(f);
    if (rd == 0) { heap_caps_free(buf); return NULL; }
    *out_len = rd;
    ESP_LOGI(TAG, "Loaded %s (%u KB)", path, (unsigned)(rd / 1024));
    return buf;
}

void video_player_init(void)
{
    // Streaming ring + wrapped-frame scratch (replaces the old 16 MB whole-file buffer)
    ring_buf = heap_caps_malloc(RING_SIZE, MALLOC_CAP_SPIRAM);
    if (!ring_buf) { ESP_LOGE(TAG, "Failed to alloc ring_buf"); return; }
    frame_asm = heap_caps_malloc(FRAME_ASM_SIZE, MALLOC_CAP_SPIRAM);
    if (!frame_asm) { ESP_LOGE(TAG, "Failed to alloc frame_asm"); return; }

    // Get panel framebuffers
    esp_lcd_panel_handle_t panel = bsp_display_get_panel_handle();
    esp_err_t fb_err = esp_lcd_dpi_panel_get_frame_buffer(
        panel, CONFIG_BSP_LCD_DPI_BUFFER_NUMS,
        &lcd_fb[0], &lcd_fb[1], &lcd_fb[2]);
    if (fb_err != ESP_OK) {
        fb_err = esp_lcd_dpi_panel_get_frame_buffer(panel, 2, &lcd_fb[0], &lcd_fb[1], NULL);
    }
    if (fb_err != ESP_OK) { ESP_LOGE(TAG, "Failed to get panel framebuffers"); return; }

    // Init HW JPEG decoder
    jpeg_decode_engine_cfg_t eng_cfg = { .intr_priority = 0, .timeout_ms = 100 };
    if (jpeg_new_decoder_engine(&eng_cfg, &jpeg_handle) != ESP_OK) {
        ESP_LOGE(TAG, "HW JPEG decoder init failed"); return;
    }

    // Allocate decode buffer
    size_t req = (size_t)BSP_LCD_V_RES * BSP_LCD_H_RES * 2;
    jpeg_decode_memory_alloc_cfg_t mem_cfg = { .buffer_direction = JPEG_DEC_ALLOC_OUTPUT_BUFFER };
    decode_buf = jpeg_alloc_decoder_mem(req, &mem_cfg, &decode_buf_size);
    if (!decode_buf) { ESP_LOGE(TAG, "Failed to alloc decode buf"); return; }

    // Load bumper into a right-sized resident PSRAM slot
    bumper_buf = load_mjpeg_alloc(BUMPER_MJPEG, &bumper_buf_len);
    bumper_available = (bumper_buf != NULL);
    if (bumper_available) {
        ESP_LOGI(TAG, "Bumper loaded (%u KB)", (unsigned)(bumper_buf_len/1024));
    } else {
        ESP_LOGW(TAG, "No bumper found at %s — videos play without intro", BUMPER_MJPEG);
    }

    // Scan for video files
    DIR *dir = opendir(VIDEO_DIR);
    if (!dir) { ESP_LOGW(TAG, "No video dir at %s", VIDEO_DIR); return; }
    struct dirent *entry;
    video_track_count = 0;
    while ((entry = readdir(dir)) != NULL && video_track_count < VIDEO_MAX_TRACKS) {
        if (entry->d_type != DT_REG) continue;
        const char *name = entry->d_name;
        // Skip bumper files
        if (strcasecmp(name, "bumper.mjpeg") == 0) continue;
        size_t len = strlen(name);
        if (len < 7) continue;
        if (strcasecmp(name + len - 6, ".mjpeg") != 0) continue;
        snprintf(video_playlist[video_track_count], sizeof(video_playlist[0]),
                 "%s/%s", VIDEO_DIR, name);
        video_track_count++;
    }
    closedir(dir);

    if (video_track_count == 0) { ESP_LOGW(TAG, "No .mjpeg files in %s", VIDEO_DIR); return; }
    ESP_LOGI(TAG, "Found %d video tracks", video_track_count);

    srand(esp_random());
    shuffle_playlist();

    ESP_LOGI(TAG, "Video player ready");
}

void video_player_start(void)
{
    if (video_player_running) { ESP_LOGW(TAG, "Already running"); return; }
    if (video_track_count == 0 || jpeg_handle == NULL || decode_buf == NULL) {
        ESP_LOGW(TAG, "Not initialized or no tracks"); return;
    }
    // Atomically claim the power gate so a network burst can't start alongside.
    // Marks the gate AND video_player_running synchronously (before the task is
    // even scheduled) so there's no window where video looks "not active".
    if (!power_gate_video_try_begin()) {
        ESP_LOGW(TAG, "Video held off — network update in progress");
        return;
    }
    video_player_running = true;
    xTaskCreatePinnedToCore(video_player_task, "video_play", 8192, NULL, 5, NULL, PIPE_CORE);
}

bool video_player_is_active(void)
{
    return video_player_running;
}

static void shuffle_playlist(void)
{
    for (int i = video_track_count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char tmp[256];
        memcpy(tmp,                  video_playlist[i], sizeof(tmp));
        memcpy(video_playlist[i],    video_playlist[j], sizeof(tmp));
        memcpy(video_playlist[j],    tmp,               sizeof(tmp));
    }
    video_current_idx = 0;
    ESP_LOGI(TAG, "Playlist shuffled");
}

static int find_frame(const uint8_t *buf, size_t buf_len, size_t offset,
                      size_t *frame_start, size_t *frame_len)
{
    size_t soi = offset;
    while (soi + 1 < buf_len) {
        if (buf[soi] == 0xFF && buf[soi + 1] == 0xD8) break;
        soi++;
    }
    if (soi + 1 >= buf_len) return 0;
    size_t eoi = soi + 2;
    while (eoi + 1 < buf_len) {
        if (buf[eoi] == 0xFF && buf[eoi + 1] == 0xD9) {
            *frame_start = soi;
            *frame_len   = eoi + 2 - soi;
            return 1;
        }
        eoi++;
    }
    return 0;
}

// ── Decode and display one MJPEG stream ──────────────────────────────────────
// start_offset: byte offset to start from (pass 0 for beginning, or result of
// a prior find_frame+frame_len to skip the first frame)
static void play_mjpeg_from(const uint8_t *mjpeg_data, size_t mjpeg_len,
                             jpeg_decode_cfg_t *dec_cfg, size_t start_offset)
{
    int frame_count = 0;
    size_t offset = start_offset, frame_start, frame_len;
    int64_t t0 = esp_timer_get_time();
    while (find_frame(mjpeg_data, mjpeg_len, offset, &frame_start, &frame_len)) {
        frame_count++;
        int64_t t_start = esp_timer_get_time();
        void *target_fb = lcd_fb[fb_idx % CONFIG_BSP_LCD_DPI_BUFFER_NUMS];
        uint32_t out_size = 0;
        esp_err_t err = jpeg_decoder_process(
            jpeg_handle, dec_cfg,
            mjpeg_data + frame_start, (uint32_t)frame_len,
            target_fb, (size_t)DISP_W * DISP_H * 2,
            &out_size);
        if (err == ESP_OK) {
            esp_lv_adapter_dummy_draw_blit(g_lv_disp, 0, 0, DISP_W, DISP_H, target_fb, true);
            fb_idx++;
        }
        offset = frame_start + frame_len;
        int64_t elapsed_ms = (esp_timer_get_time() - t_start) / 1000;
        int32_t delay_ms   = FRAME_INTERVAL_MS - (int32_t)elapsed_ms;
        if (delay_ms > 1) vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
    ESP_LOGI(TAG, "play_mjpeg: %d frames in %lld ms", frame_count, (esp_timer_get_time()-t0)/1000);
}

// ── Decode and display first frame, return byte offset after it ──────────────
static size_t render_first_frame(const uint8_t *mjpeg_data, size_t mjpeg_len,
                                  jpeg_decode_cfg_t *dec_cfg)
{
    size_t frame_start, frame_len;
    if (!find_frame(mjpeg_data, mjpeg_len, 0, &frame_start, &frame_len)) return 0;
    void *target_fb = lcd_fb[fb_idx % CONFIG_BSP_LCD_DPI_BUFFER_NUMS];
    uint32_t out_size = 0;
    esp_err_t err = jpeg_decoder_process(
        jpeg_handle, dec_cfg,
        mjpeg_data + frame_start, (uint32_t)frame_len,
        target_fb, (size_t)DISP_W * DISP_H * 2, &out_size);
    if (err == ESP_OK) {
        esp_lv_adapter_dummy_draw_blit(g_lv_disp, 0, 0, DISP_W, DISP_H, target_fb, true);
        fb_idx++;
    }
    return frame_start + frame_len;
}

// ── Streaming pipeline ───────────────────────────────────────────────────────

// Producer: read the clip from SD into the ring, backpressuring when it's full.
// Pinned to PIPE_CORE alongside the consumer (same core => coherent PSRAM ring).
static void video_producer_task(void *arg)
{
    (void)arg;
    s_prod_running = true;
    while (!s_prod_stop) {
        size_t avail = (size_t)(s_wr - s_rd);
        size_t freeb = RING_SIZE - avail;
        if (freeb <= PIPE_CHUNK) { vTaskDelay(pdMS_TO_TICKS(3)); continue; }  // ring full -> wait
        size_t widx   = (size_t)(s_wr % RING_SIZE);
        size_t contig = RING_SIZE - widx;        // space before the physical wrap
        size_t want   = freeb - 1;               // keep 1 byte free (full/empty disambiguation)
        if (want > contig)     want = contig;
        if (want > PIPE_CHUNK) want = PIPE_CHUNK;
        size_t n = fread(ring_buf + widx, 1, want, s_prod_file);
        __sync_synchronize();                    // data committed before advancing the write index
        s_wr += n;
        if (n < want) { s_prod_eof = true; break; }   // short read = EOF
    }
    s_prod_running = false;
    vTaskDelete(NULL);
}

// Find the next complete JPEG frame (SOI..EOI) in the readable region [s_rd, s_wr).
// Returns its absolute byte offset + length; false if not fully buffered yet.
static bool ring_find_frame(uint64_t *fs, size_t *flen)
{
    uint64_t end = s_wr;             // snapshot (only ever grows)
    uint64_t i = s_rd;
    while (i + 1 < end) {            // scan for SOI 0xFFD8
        if (ring_buf[i % RING_SIZE] == 0xFF && ring_buf[(i + 1) % RING_SIZE] == 0xD8) break;
        i++;
    }
    if (i + 1 >= end) return false;
    uint64_t soi = i;
    for (uint64_t j = soi + 2; j + 1 < end; j++) {   // scan for EOI 0xFFD9
        if (ring_buf[j % RING_SIZE] == 0xFF && ring_buf[(j + 1) % RING_SIZE] == 0xD9) {
            *fs   = soi;
            *flen = (size_t)(j + 2 - soi);
            return true;
        }
    }
    return false;                    // EOI not in the buffer yet
}

// Open the clip and start the producer filling the ring. The bumper plays while
// it prefills, so by the time we consume, the ring is full.
static bool video_pipeline_start(const char *path)
{
    s_prod_file = fopen(path, "rb");
    if (!s_prod_file) { ESP_LOGE(TAG, "Pipeline: cannot open %s", path); return false; }
    s_wr = s_rd = 0;
    s_prod_eof = s_prod_stop = false;
    xTaskCreatePinnedToCore(video_producer_task, "vid_prod", 4096, NULL, 5, NULL, PIPE_CORE);
    return true;
}

// Consume the ring: decode + display frames paced to 20 fps, logging buffer fill
// once a second. Steady state rides near full (read >> drain); if the buffer
// trends down the clip's bitrate (quality) is outrunning the SD read.
static void video_pipeline_run(const char *mp3_path, jpeg_decode_cfg_t *dec_cfg)
{
    while ((s_wr - s_rd) < PIPE_PREFILL && !s_prod_eof) vTaskDelay(pdMS_TO_TICKS(5)); // prefill cushion

    bool audio_started   = false;
    bool underrun_logged = false;
    int  frame_count = 0, fps_frames = 0;
    int64_t t0 = esp_timer_get_time();
    int64_t last_telem = t0;

    while (!s_prod_stop) {
        uint64_t fs; size_t flen;
        if (ring_find_frame(&fs, &flen)) {
            underrun_logged = false;
            int64_t t_start = esp_timer_get_time();
            size_t pidx = (size_t)(fs % RING_SIZE);
            const uint8_t *fp;
            if (flen > FRAME_ASM_SIZE) {                 // pathological frame — skip
                ESP_LOGW(TAG, "frame %u KB exceeds scratch — skipping", (unsigned)(flen / 1024));
                s_rd = fs + flen;
                continue;
            }
            if (pidx + flen <= RING_SIZE) {
                fp = ring_buf + pidx;                    // contiguous in the ring
            } else {                                     // wraps -> reassemble into scratch
                size_t first = RING_SIZE - pidx;
                memcpy(frame_asm, ring_buf + pidx, first);
                memcpy(frame_asm + first, ring_buf, flen - first);
                fp = frame_asm;
            }
            void *target_fb = lcd_fb[fb_idx % CONFIG_BSP_LCD_DPI_BUFFER_NUMS];
            uint32_t out_size = 0;
            esp_err_t err = jpeg_decoder_process(jpeg_handle, dec_cfg, fp, (uint32_t)flen,
                                                 target_fb, (size_t)DISP_W * DISP_H * 2, &out_size);
            if (err == ESP_OK) {
                esp_lv_adapter_dummy_draw_blit(g_lv_disp, 0, 0, DISP_W, DISP_H, target_fb, true);
                fb_idx++;
                if (!audio_started) { video_mp3_play(mp3_path); audio_started = true; }
            }
            s_rd = fs + flen;                            // consume through end of frame
            frame_count++; fps_frames++;

            int64_t now = esp_timer_get_time();
            if (now - last_telem >= 1000000) {           // once-a-second buffer telemetry
                uint64_t a = s_wr - s_rd;
                ESP_LOGI(TAG, "buf %u%% (%u KB)  frame=%d  ~%dfps",
                         (unsigned)(a * 100 / RING_SIZE), (unsigned)(a / 1024),
                         frame_count, fps_frames);
                last_telem = now; fps_frames = 0;
            }

            int64_t elapsed_ms = (now - t_start) / 1000;
            int32_t delay_ms   = FRAME_INTERVAL_MS - (int32_t)elapsed_ms;
            if (delay_ms > 1) vTaskDelay(pdMS_TO_TICKS(delay_ms));
            continue;
        }
        if (s_prod_eof) break;                           // producer done + no frame left
        if (!underrun_logged) {                          // producer fell behind
            ESP_LOGW(TAG, "buf UNDERRUN (%u KB) — quality may be too high",
                     (unsigned)((s_wr - s_rd) / 1024));
            underrun_logged = true;
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }

    s_prod_stop = true;                                  // stop + reap the producer, then close
    while (s_prod_running) vTaskDelay(pdMS_TO_TICKS(2));
    if (s_prod_file) { fclose(s_prod_file); s_prod_file = NULL; }
    ESP_LOGI(TAG, "Pipeline: %d frames in %lld ms", frame_count, (esp_timer_get_time() - t0) / 1000);
}

static void fade_overlay_anim_cb(void *obj, int32_t v)
{
    lv_obj_set_style_bg_opa((lv_obj_t *)obj, (lv_opa_t)v, 0);
    if (v == LV_OPA_TRANSP) lv_obj_add_flag((lv_obj_t *)obj, LV_OBJ_FLAG_HIDDEN);
}

// Defined in main.c — resets the home-screen timers so they don't all fire at
// once when LVGL resumes after a video (runs here, inside the LVGL task).
extern void home_timers_reset_after_video(void);

static void trigger_fade_in(void *arg)
{
    home_timers_reset_after_video();
    lv_obj_t *overlay = (lv_obj_t *)arg;
    lv_obj_clear_flag(overlay, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_bg_opa(overlay, LV_OPA_COVER, 0);
    lv_obj_move_foreground(overlay);
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, overlay);
    lv_anim_set_exec_cb(&a, fade_overlay_anim_cb);
    lv_anim_set_values(&a, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_duration(&a, 400);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_start(&a);
}

static void video_cleanup_task(void *arg)
{
    (void)arg;
    video_mp3_stop();
    vTaskDelete(NULL);
}

static void video_player_task(void *arg)
{
    (void)arg;
    // video_player_running was set synchronously in video_player_start().
    ESP_LOGI(TAG, "Video player task started");

    esp_lv_adapter_pause(-1);
    esp_lv_adapter_set_dummy_draw(g_lv_disp, true);

    jpeg_decode_cfg_t dec_cfg = {
        .output_format = JPEG_DECODE_OUT_FORMAT_RGB565,
        .rgb_order     = JPEG_DEC_RGB_ELEMENT_ORDER_BGR,
    };

    int track_idx = video_current_idx;

    // Derive the sidecar mp3 path.
    char mp3_path[256];
    strncpy(mp3_path, video_playlist[track_idx], sizeof(mp3_path) - 1);
    mp3_path[sizeof(mp3_path) - 1] = '\0';
    { char *ext = strrchr(mp3_path, '.'); if (ext) strcpy(ext, ".mp3"); }

    // Start streaming the clip into the ring NOW; the bumper masks the prefill,
    // so by the time it finishes the ring is full and the clip starts instantly.
    bool started = video_pipeline_start(video_playlist[track_idx]);

    if (bumper_available) {
        // Render first bumper frame, then start audio — keeps video/audio in sync
        size_t bumper_after_first = render_first_frame(bumper_buf, bumper_buf_len, &dec_cfg);
        video_mp3_play(BUMPER_MP3);
        play_mjpeg_from(bumper_buf, bumper_buf_len, &dec_cfg, bumper_after_first);
        video_mp3_stop();
    }

    if (started) {
        ESP_LOGI(TAG, "Playing: %s", video_playlist[track_idx]);
        video_pipeline_run(mp3_path, &dec_cfg);
        ESP_LOGI(TAG, "Track done");
    }

    // Advance playlist
    video_current_idx = (track_idx + 1) % video_track_count;
    if (video_current_idx == 0) shuffle_playlist();

    // Restore display
    esp_lv_adapter_set_dummy_draw(g_lv_disp, false);
    esp_lv_adapter_resume();
    video_player_running = false;
    power_gate_video_end();   // release the gate so deferred network can proceed

    // Fade home screen in — queue via lv_async_call so it runs inside the LVGL task
    if (g_video_fade_overlay) {
        lv_async_call(trigger_fade_in, g_video_fade_overlay);
    }

    // Stop any remaining audio in background
    xTaskCreate(video_cleanup_task, "vid_cleanup", 4096, NULL, 5, NULL);

    ESP_LOGI(TAG, "Video player done");
    vTaskDelete(NULL);
}
