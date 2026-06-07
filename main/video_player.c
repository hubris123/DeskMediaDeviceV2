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

static const char *TAG = "VideoPlayer";

#define VIDEO_DIR           "/sdcard/video"
#define BUMPER_MJPEG        "/sdcard/video/bumper.mjpeg"
#define BUMPER_MP3          "/sdcard/video/bumper.mp3"
#define VIDEO_MAX_TRACKS    32
#define VIDEO_BUF_SIZE      (10 * 1024 * 1024)  // main video PSRAM slot
#define BUMPER_BUF_SIZE     (5  * 1024 * 1024)  // bumper PSRAM slot
#define FRAME_INTERVAL_MS   50

#define DISP_W  BSP_LCD_H_RES   // 480
#define DISP_H  BSP_LCD_V_RES   // 800

static char  video_playlist[VIDEO_MAX_TRACKS][256];
static int   video_track_count   = 0;
static int   video_current_idx   = 0;

// Slot 0: main video buffer, Slot 1: unused (kept for future multi-slot)
static uint8_t *video_buf       = NULL;
static size_t   video_buf_len   = 0;
static int      video_buf_track = -1;

// Bumper — always kept in PSRAM, loaded once at init
static uint8_t *bumper_buf      = NULL;
static size_t   bumper_buf_len  = 0;
static bool     bumper_available = false;

static void  *decode_buf      = NULL;
static size_t decode_buf_size = 0;

static jpeg_decoder_handle_t jpeg_handle = NULL;

static void *lcd_fb[CONFIG_BSP_LCD_DPI_BUFFER_NUMS];
static int   fb_idx = 0;

static bool video_player_running = false;

// Background load state — used to load real video while bumper plays
static volatile bool s_bg_load_done  = false;
static volatile bool s_bg_load_ok    = false;
static volatile int  s_bg_load_track = -1;

static void video_player_task(void *arg);
static void video_cleanup_task(void *arg);
static void video_bg_load_task(void *arg);
static void fade_overlay_anim_cb(void *obj, int32_t v);
static void trigger_fade_in(void *arg);
static size_t render_first_frame(const uint8_t *mjpeg_data, size_t mjpeg_len, jpeg_decode_cfg_t *dec_cfg);
static void play_mjpeg_from(const uint8_t *mjpeg_data, size_t mjpeg_len, jpeg_decode_cfg_t *dec_cfg, size_t start_offset);
static bool load_mjpeg_to_buf(const char *path, uint8_t *buf, size_t buf_size, size_t *out_len);
static void shuffle_playlist(void);
static int  find_frame(const uint8_t *buf, size_t buf_len, size_t offset,
                       size_t *frame_start, size_t *frame_len);
static void play_mjpeg(const uint8_t *mjpeg_data, size_t mjpeg_len,
                       jpeg_decode_cfg_t *dec_cfg);

extern void video_mp3_play(const char *path);
extern void video_mp3_stop(void);

// ── Load MJPEG file into a PSRAM buffer ──────────────────────────────────────
static bool load_mjpeg_to_buf(const char *path, uint8_t *buf, size_t buf_size, size_t *out_len)
{
    FILE *f = fopen(path, "rb");
    if (!f) { ESP_LOGE(TAG, "Cannot open %s", path); return false; }
    fseek(f, 0, SEEK_END);
    size_t sz = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz > buf_size) {
        ESP_LOGW(TAG, "%s too large (%u KB), truncating", path, (unsigned)(sz/1024));
        sz = buf_size;
    }
    size_t rd = fread(buf, 1, sz, f);
    fclose(f);
    *out_len = rd;
    ESP_LOGI(TAG, "Loaded %s (%u KB)", path, (unsigned)(rd/1024));
    return (rd > 0);
}

void video_player_init(void)
{
    // Allocate main video PSRAM buffer
    video_buf = heap_caps_malloc(VIDEO_BUF_SIZE, MALLOC_CAP_SPIRAM);
    if (!video_buf) { ESP_LOGE(TAG, "Failed to alloc video_buf"); return; }

    // Allocate bumper PSRAM buffer
    bumper_buf = heap_caps_malloc(BUMPER_BUF_SIZE, MALLOC_CAP_SPIRAM);
    if (!bumper_buf) { ESP_LOGE(TAG, "Failed to alloc bumper_buf"); return; }

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

    // Load bumper into PSRAM permanently
    bumper_available = load_mjpeg_to_buf(BUMPER_MJPEG, bumper_buf, BUMPER_BUF_SIZE, &bumper_buf_len);
    if (bumper_available) {
        ESP_LOGI(TAG, "Bumper loaded (%u KB)", (unsigned)(bumper_buf_len/1024));
    } else {
        ESP_LOGW(TAG, "No bumper found at %s — videos will load without intro", BUMPER_MJPEG);
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
    xTaskCreate(video_player_task, "video_play", 8192, NULL, 5, NULL);
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

static void play_mjpeg(const uint8_t *mjpeg_data, size_t mjpeg_len,
                       jpeg_decode_cfg_t *dec_cfg)
{
    play_mjpeg_from(mjpeg_data, mjpeg_len, dec_cfg, 0);
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

static void fade_overlay_anim_cb(void *obj, int32_t v)
{
    lv_obj_set_style_bg_opa((lv_obj_t *)obj, (lv_opa_t)v, 0);
    if (v == LV_OPA_TRANSP) lv_obj_add_flag((lv_obj_t *)obj, LV_OBJ_FLAG_HIDDEN);
}

static void trigger_fade_in(void *arg)
{
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

static void video_bg_load_task(void *arg)
{
    (void)arg;
    ESP_LOGI(TAG, "BG load: starting track %d", s_bg_load_track);
    int64_t t0 = esp_timer_get_time();
    s_bg_load_ok = load_mjpeg_to_buf(
        video_playlist[s_bg_load_track], video_buf, VIDEO_BUF_SIZE, &video_buf_len);
    if (s_bg_load_ok) video_buf_track = s_bg_load_track;
    ESP_LOGI(TAG, "BG load: done in %lld ms, ok=%d", (esp_timer_get_time()-t0)/1000, s_bg_load_ok);
    s_bg_load_done = true;
    vTaskDelete(NULL);
}

static void video_player_task(void *arg)
{
    (void)arg;
    video_player_running = true;
    ESP_LOGI(TAG, "Video player task started");

    esp_lv_adapter_pause(-1);
    esp_lv_adapter_set_dummy_draw(g_lv_disp, true);

    jpeg_decode_cfg_t dec_cfg = {
        .output_format = JPEG_DECODE_OUT_FORMAT_RGB565,
        .rgb_order     = JPEG_DEC_RGB_ELEMENT_ORDER_BGR,
    };

    int track_idx = video_current_idx;

    // ── Play bumper while loading the real video concurrently ────────────────
    if (bumper_available) {
        bool loaded = false;

        if (video_buf_track != track_idx) {
            // Kick off load in background before bumper starts
            s_bg_load_track = track_idx;
            s_bg_load_done  = false;
            s_bg_load_ok    = false;
            xTaskCreate(video_bg_load_task, "vid_load", 4096, NULL, 2, NULL);
        } else {
            // Already cached — no load needed
            s_bg_load_done = true;
            s_bg_load_ok   = true;
        }

        // Render first bumper frame, then start audio — keeps video/audio in sync
        size_t bumper_after_first = render_first_frame(bumper_buf, bumper_buf_len, &dec_cfg);
        video_mp3_play(BUMPER_MP3);
        play_mjpeg_from(bumper_buf, bumper_buf_len, &dec_cfg, bumper_after_first);
        video_mp3_stop();

        // Wait for load if bumper finished before it completed
        if (!s_bg_load_done) {
            ESP_LOGW(TAG, "Bumper done but load still running — waiting...");
            while (!s_bg_load_done) { vTaskDelay(pdMS_TO_TICKS(50)); }
            ESP_LOGI(TAG, "Load complete after bumper");
        } else {
            ESP_LOGI(TAG, "Load completed during bumper — no gap");
        }
        loaded = s_bg_load_ok;

        // Now play the real video
        if (loaded && video_buf_len > 0) {
            char mp3_path[256];
            strncpy(mp3_path, video_playlist[track_idx], sizeof(mp3_path) - 1);
            mp3_path[sizeof(mp3_path) - 1] = '\0';
            char *ext = strrchr(mp3_path, '.');
            if (ext) strcpy(ext, ".mp3");

            ESP_LOGI(TAG, "Playing: %s", video_playlist[track_idx]);
            size_t after_first = render_first_frame(video_buf, video_buf_len, &dec_cfg);
            video_mp3_play(mp3_path);
            play_mjpeg_from(video_buf, video_buf_len, &dec_cfg, after_first);
            ESP_LOGI(TAG, "Track done");
        }

    } else {
        // No bumper — load and play directly
        if (video_buf_track != track_idx) {
            load_mjpeg_to_buf(video_playlist[track_idx], video_buf, VIDEO_BUF_SIZE, &video_buf_len);
            video_buf_track = track_idx;
        }
        if (video_buf_len > 0) {
            char mp3_path[256];
            strncpy(mp3_path, video_playlist[track_idx], sizeof(mp3_path) - 1);
            mp3_path[sizeof(mp3_path) - 1] = '\0';
            char *ext = strrchr(mp3_path, '.');
            if (ext) strcpy(ext, ".mp3");
            size_t after_first = render_first_frame(video_buf, video_buf_len, &dec_cfg);
            video_mp3_play(mp3_path);
            play_mjpeg_from(video_buf, video_buf_len, &dec_cfg, after_first);
        }
    }

    // Advance playlist
    video_current_idx = (track_idx + 1) % video_track_count;
    if (video_current_idx == 0) shuffle_playlist();

    // Restore display
    esp_lv_adapter_set_dummy_draw(g_lv_disp, false);
    esp_lv_adapter_resume();
    video_player_running = false;

    // Fade home screen in — queue via lv_async_call so it runs inside the LVGL task
    if (g_video_fade_overlay) {
        lv_async_call(trigger_fade_in, g_video_fade_overlay);
    }

    // Stop any remaining audio in background
    xTaskCreate(video_cleanup_task, "vid_cleanup", 4096, NULL, 5, NULL);

    ESP_LOGI(TAG, "Video player done");
    vTaskDelete(NULL);
}
