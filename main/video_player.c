/*
 * video_player.c - MJPEG video player for ESP32-P4 DeskMediaDevice
 *
 * Key findings from testing:
 *   - dummy_draw_blit uses LOGICAL landscape coords (800x480), adapter rotates to portrait
 *   - Color bar test confirmed: write vertical bars, displayed as horizontal = 90 rotation
 *   - Video must be encoded as 800x480 landscape
 *   - CPU memcpy from JPEG decode buf (PSRAM) works but cache must be invalidated first
 *
 * Video encoding:
 *   ffmpeg -i input.mp4 -c:v mjpeg -q:v 5 -vf "scale=800:480" -r 20 "01 - Title.mjpeg"
 *   ffmpeg -i input.mp4 -ac 2 -ar 32000 -ab 128k -codec:a libmp3lame "01 - Title.mp3"
 * Place both in /sdcard/video/
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
#define VIDEO_MAX_TRACKS    32
#define VIDEO_BUF_SIZE      (10 * 1024 * 1024)
#define FRAME_INTERVAL_MS   50

// Video must be encoded as 480x800 portrait to match physical panel.
// dummy_draw_blit is a raw passthrough - no rotation happens.
#define DISP_W  BSP_LCD_H_RES   // 480
#define DISP_H  BSP_LCD_V_RES   // 800

static char  video_playlist[VIDEO_MAX_TRACKS][256];
static int   video_track_count   = 0;
static int   video_current_idx   = 0;

static uint8_t *video_buf[2]       = {NULL, NULL};
static size_t   video_buf_len[2]   = {0, 0};
static int      video_buf_track[2] = {-1, -1};

static void  *decode_buf      = NULL;
static size_t decode_buf_size = 0;

static jpeg_decoder_handle_t jpeg_handle = NULL;

static void *lcd_fb[CONFIG_BSP_LCD_DPI_BUFFER_NUMS];
static int   fb_idx = 0;

static bool video_player_running = false;

static void video_player_task(void *arg);
static bool load_video_to_slot(int slot, int track_idx);
static void shuffle_playlist(void);
static int  find_frame(const uint8_t *buf, size_t buf_len, size_t offset,
                       size_t *frame_start, size_t *frame_len);

extern void video_mp3_play(const char *path);
extern void video_mp3_stop(void);

void video_player_init(void)
{
    for (int i = 0; i < 2; i++) {
        video_buf[i] = heap_caps_malloc(VIDEO_BUF_SIZE, MALLOC_CAP_SPIRAM);
        if (!video_buf[i]) {
            ESP_LOGE(TAG, "Failed to alloc video_buf[%d]", i);
            return;
        }
        ESP_LOGI(TAG, "video_buf[%d]: %d MB", i, VIDEO_BUF_SIZE / (1024 * 1024));
    }

    esp_lcd_panel_handle_t panel = bsp_display_get_panel_handle();
    esp_err_t fb_err = esp_lcd_dpi_panel_get_frame_buffer(
        panel, CONFIG_BSP_LCD_DPI_BUFFER_NUMS,
        &lcd_fb[0], &lcd_fb[1], &lcd_fb[2]);
    if (fb_err != ESP_OK) {
        fb_err = esp_lcd_dpi_panel_get_frame_buffer(panel, 2, &lcd_fb[0], &lcd_fb[1], NULL);
    }
    if (fb_err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get panel framebuffers");
        return;
    }
    ESP_LOGI(TAG, "Panel fbs: %p %p", lcd_fb[0], lcd_fb[1]);

    jpeg_decode_engine_cfg_t eng_cfg = {
        .intr_priority = 0,
        .timeout_ms    = 100,
    };
    if (jpeg_new_decoder_engine(&eng_cfg, &jpeg_handle) != ESP_OK) {
        ESP_LOGE(TAG, "HW JPEG decoder init failed");
        return;
    }

    // Allocate decode buffer with proper alignment for HW JPEG decoder
    // Use PSRAM - internal RAM doesn't have enough space for 768KB
    size_t req = (size_t)BSP_LCD_V_RES * BSP_LCD_H_RES * 2;  // 800*480*2 = 768000
    jpeg_decode_memory_alloc_cfg_t mem_cfg = {
        .buffer_direction = JPEG_DEC_ALLOC_OUTPUT_BUFFER
    };
    decode_buf = jpeg_alloc_decoder_mem(req, &mem_cfg, &decode_buf_size);
    if (!decode_buf) {
        ESP_LOGE(TAG, "Failed to alloc decode buf");
        return;
    }
    ESP_LOGI(TAG, "Decode buf: %u bytes at %p", (unsigned)decode_buf_size, decode_buf);

    DIR *dir = opendir(VIDEO_DIR);
    if (!dir) {
        ESP_LOGW(TAG, "No video dir at %s", VIDEO_DIR);
        return;
    }
    struct dirent *entry;
    video_track_count = 0;
    while ((entry = readdir(dir)) != NULL && video_track_count < VIDEO_MAX_TRACKS) {
        if (entry->d_type != DT_REG) continue;
        const char *name = entry->d_name;
        size_t len = strlen(name);
        if (len < 7) continue;
        if (strcasecmp(name + len - 6, ".mjpeg") != 0) continue;
        snprintf(video_playlist[video_track_count], sizeof(video_playlist[0]),
                 "%s/%s", VIDEO_DIR, name);
        video_track_count++;
    }
    closedir(dir);

    if (video_track_count == 0) {
        ESP_LOGW(TAG, "No .mjpeg files in %s", VIDEO_DIR);
        return;
    }
    ESP_LOGI(TAG, "Found %d video tracks", video_track_count);

    srand((unsigned)time(NULL));
    shuffle_playlist();
    load_video_to_slot(0, 0);
    if (video_track_count > 1) load_video_to_slot(1, 1);

    ESP_LOGI(TAG, "Video player ready");
}

void video_player_start(void)
{
    if (video_player_running) {
        ESP_LOGW(TAG, "Already running");
        return;
    }
    if (video_track_count == 0 || jpeg_handle == NULL || decode_buf == NULL) {
        ESP_LOGW(TAG, "Not initialized or no tracks");
        return;
    }
    xTaskCreate(video_player_task, "video_play", 8192, NULL, 5, NULL);
}

static void shuffle_playlist(void)
{
    for (int i = video_track_count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char tmp[256];
        memcpy(tmp,               video_playlist[i], sizeof(tmp));
        memcpy(video_playlist[i], video_playlist[j], sizeof(tmp));
        memcpy(video_playlist[j], tmp,               sizeof(tmp));
    }
    video_current_idx = 0;
    ESP_LOGI(TAG, "Playlist shuffled");
}

static bool load_video_to_slot(int slot, int track_idx)
{
    if (track_idx >= video_track_count || !video_buf[slot]) return false;
    FILE *f = fopen(video_playlist[track_idx], "rb");
    if (!f) { ESP_LOGE(TAG, "Cannot open %s", video_playlist[track_idx]); return false; }
    fseek(f, 0, SEEK_END);
    size_t sz = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz > VIDEO_BUF_SIZE) sz = VIDEO_BUF_SIZE;
    size_t rd = fread(video_buf[slot], 1, sz, f);
    fclose(f);
    video_buf_len[slot]   = rd;
    video_buf_track[slot] = track_idx;
    ESP_LOGI(TAG, "Slot %d: track %d (%u KB)", slot, track_idx, (unsigned)(rd / 1024));
    return true;
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

static void video_player_task(void *arg)
{
    video_player_running = true;
    ESP_LOGI(TAG, "Video player task started");

    esp_lv_adapter_pause(-1);
    esp_lv_adapter_set_dummy_draw(g_lv_disp, true);
    ESP_LOGI(TAG, "Display taken over");

    jpeg_decode_cfg_t dec_cfg = {
        .output_format = JPEG_DECODE_OUT_FORMAT_RGB565,
        .rgb_order     = JPEG_DEC_RGB_ELEMENT_ORDER_BGR,
    };

    int  play_slot = 0;
    int  next_slot = 1;
    bool reshuffle_pending = false;

    while (true) {
        int track_idx = video_current_idx;
        if (video_buf_track[play_slot] != track_idx) {
            if (!load_video_to_slot(play_slot, track_idx)) break;
        }

        uint8_t *mjpeg_data = video_buf[play_slot];
        size_t   mjpeg_len  = video_buf_len[play_slot];
        if (mjpeg_len == 0) break;

        char mp3_path[256];
        strncpy(mp3_path, video_playlist[track_idx], sizeof(mp3_path) - 1);
        mp3_path[sizeof(mp3_path) - 1] = '\0';
        char *ext = strrchr(mp3_path, '.');
        if (ext) strcpy(ext, ".mp3");

        ESP_LOGI(TAG, "Playing: %s", video_playlist[track_idx]);
        video_mp3_play(mp3_path);

        int next_track_idx = track_idx + 1;
        if (next_track_idx >= video_track_count) {
            reshuffle_pending = true;
            next_track_idx = 0;
        }
        // NOTE: next track pre-load moved to AFTER the decode loop.
        // Previously it ran here and caused audio/video sync delay —
        // a 10MB SD read was blocking the first JPEG frame while audio played.

        size_t offset      = 0;
        size_t frame_start, frame_len;
        int    frame_count = 0;
        size_t frame_bytes = (size_t)DISP_W * DISP_H * 2;

        while (find_frame(mjpeg_data, mjpeg_len, offset, &frame_start, &frame_len)) {
            int64_t t_start = esp_timer_get_time();

            // Decode directly into panel framebuffer (internal RAM, DMA2D-safe)
            // DMA2D cannot reliably read from PSRAM — decode to internal RAM only.
            void *target_fb = lcd_fb[fb_idx % CONFIG_BSP_LCD_DPI_BUFFER_NUMS];
            uint32_t out_size = 0;
            esp_err_t err = jpeg_decoder_process(
                jpeg_handle, &dec_cfg,
                mjpeg_data + frame_start, (uint32_t)frame_len,
                target_fb, (size_t)DISP_W * DISP_H * 2,
                &out_size);

            if (err == ESP_OK) {
                esp_lv_adapter_dummy_draw_blit(g_lv_disp,
                                               0, 0, DISP_W, DISP_H,
                                               target_fb, true);
                fb_idx++;
                frame_count++;
            } else {
                ESP_LOGW(TAG, "JPEG failed frame %d: %s",
                         frame_count, esp_err_to_name(err));
            }

            offset = frame_start + frame_len;

            int64_t elapsed_ms = (esp_timer_get_time() - t_start) / 1000;
            int32_t delay_ms   = FRAME_INTERVAL_MS - (int32_t)elapsed_ms;
            if (delay_ms > 1) vTaskDelay(pdMS_TO_TICKS(delay_ms));
        }

        ESP_LOGI(TAG, "Track %d done: %d frames", track_idx, frame_count);
        video_mp3_stop();

        // Pre-load next track now — current track is done, no sync issue
        if (video_track_count > 1) load_video_to_slot(next_slot, next_track_idx);

        video_current_idx = next_track_idx;
        if (reshuffle_pending) { shuffle_playlist(); reshuffle_pending = false; }

        int tmp   = play_slot;
        play_slot = next_slot;
        next_slot = tmp;

        break;
    }

    ESP_LOGI(TAG, "Returning display to LVGL");
    esp_lv_adapter_set_dummy_draw(g_lv_disp, false);
    esp_lv_adapter_resume();
    lv_obj_invalidate(lv_scr_act());

    video_player_running = false;
    ESP_LOGI(TAG, "Video player done");
    vTaskDelete(NULL);
}
