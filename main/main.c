#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "driver/gpio.h"
#include "lvgl.h"
#include "bsp/esp-bsp.h"
#include "bsp/display.h"
#include "bsp_board_extra.h"

static const char *TAG = "DeskMediaDevice";

/* Audio configuration (matching canon.pcm format) */
#define AUDIO_SAMPLE_RATE       16000
#define AUDIO_MCLK_MULTIPLE     384
#define AUDIO_MCLK_FREQ_HZ      (AUDIO_SAMPLE_RATE * AUDIO_MCLK_MULTIPLE)
#define AUDIO_BUFFER_SIZE       4096

/* File system paths */
static char pcm_file_path[256] = "";
static char wav_file_path[256] = "";

/* Audio playback state */
static bool is_playing = false;
static TaskHandle_t audio_task_handle = NULL;

/* Forward declarations */
static void scan_sd_card(void);
static void create_ui(void);

/**
 * Scan SD card and find audio files
 */
static void scan_sd_card(void)
{
    esp_err_t ret = bsp_sdcard_mount();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount SD card: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "SD card mounted successfully at /sdcard");

    DIR *dir = opendir("/sdcard");
    if (!dir) {
        ESP_LOGE(TAG, "Failed to open /sdcard directory");
        return;
    }

    struct dirent *entry;
    int file_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  // Regular file
            char full_path[256];
            snprintf(full_path, sizeof(full_path), "/sdcard/%s", entry->d_name);

            // Check file extension
            if (strstr(entry->d_name, ".pcm") || strstr(entry->d_name, ".PCM")) {
                snprintf(pcm_file_path, sizeof(pcm_file_path), "%s", full_path);
                ESP_LOGI(TAG, "Found PCM file: %s", pcm_file_path);
                file_count++;
            } else if (strstr(entry->d_name, ".wav") || strstr(entry->d_name, ".WAV")) {
                snprintf(wav_file_path, sizeof(wav_file_path), "%s", full_path);
                ESP_LOGI(TAG, "Found WAV file: %s", wav_file_path);
                file_count++;
            }
        }
    }
    closedir(dir);

    if (file_count > 0) {
        ESP_LOGI(TAG, "Found %d audio file(s) on SD card", file_count);
    } else {
        ESP_LOGW(TAG, "No audio files found on SD card");
    }
}

/**
 * Stop audio button callback
 */
static void stop_callback(lv_event_t *e)
{
    is_playing = false;
    ESP_LOGI(TAG, "Audio stopped");
}

/**
 * Play PCM file button callback
 */
static void play_pcm_callback(lv_event_t *e)
{
    if (pcm_file_path[0] == '\0') {
        ESP_LOGW(TAG, "PCM file not found");
        return;
    }
    ESP_LOGI(TAG, "Play PCM button pressed - audio integration coming soon");
}

/**
 * Play WAV file button callback
 */
static void play_wav_callback(lv_event_t *e)
{
    if (wav_file_path[0] == '\0') {
        ESP_LOGW(TAG, "WAV file not found");
        return;
    }
    ESP_LOGI(TAG, "Play WAV button pressed - audio integration coming soon");
}

/**
 * Create simple UI with two play buttons
 */
static void create_ui(void)
{
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1a1a1a), 0);

    // Title label
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Desk Media Device");
    lv_obj_set_style_text_color(title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // Status label
    lv_obj_t *status = lv_label_create(scr);
    lv_label_set_text(status, "Ready");
    lv_obj_set_style_text_color(status, lv_color_hex(0x00ff00), 0);
    lv_obj_align(status, LV_ALIGN_TOP_MID, 0, 60);

    // PCM button
    lv_obj_t *btn_pcm = lv_btn_create(scr);
    lv_obj_set_size(btn_pcm, 200, 60);
    lv_obj_align(btn_pcm, LV_ALIGN_CENTER, -120, -80);
    lv_obj_set_style_bg_color(btn_pcm, lv_color_hex(0x0066cc), 0);
    lv_obj_add_event_cb(btn_pcm, play_pcm_callback, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label_pcm = lv_label_create(btn_pcm);
    lv_label_set_text(label_pcm, "Play PCM");
    lv_obj_center(label_pcm);

    // WAV button
    lv_obj_t *btn_wav = lv_btn_create(scr);
    lv_obj_set_size(btn_wav, 200, 60);
    lv_obj_align(btn_wav, LV_ALIGN_CENTER, 120, -80);
    lv_obj_set_style_bg_color(btn_wav, lv_color_hex(0x00cc66), 0);
    lv_obj_add_event_cb(btn_wav, play_wav_callback, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label_wav = lv_label_create(btn_wav);
    lv_label_set_text(label_wav, "Play WAV");
    lv_obj_center(label_wav);

    // Stop button
    lv_obj_t *btn_stop = lv_btn_create(scr);
    lv_obj_set_size(btn_stop, 200, 60);
    lv_obj_align(btn_stop, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(btn_stop, lv_color_hex(0xcc0000), 0);
    lv_obj_add_event_cb(btn_stop, stop_callback, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label_stop = lv_label_create(btn_stop);
    lv_label_set_text(label_stop, "Stop");
    lv_obj_center(label_stop);

    // File info label
    lv_obj_t *info = lv_label_create(scr);
    char info_text[256];
    snprintf(info_text, sizeof(info_text), "PCM: %s\nWAV: %s",
             pcm_file_path[0] != '\0' ? "Found" : "Not found",
             wav_file_path[0] != '\0' ? "Found" : "Not found");
    lv_label_set_text(info, info_text);
    lv_obj_set_style_text_color(info, lv_color_hex(0xcccccc), 0);
    lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -20);
}

/**
 * Main application entry point
 */
void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Starting Desk Media Device");

    // Scan SD card for audio files
    scan_sd_card();

    // Initialize display via BSP
    bsp_display_cfg_t cfg = {
        .lv_adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG(),
        .rotation = ESP_LV_ADAPTER_ROTATE_0,
        .tear_avoid_mode = ESP_LV_ADAPTER_TEAR_AVOID_MODE_TRIPLE_PARTIAL,
        .touch_flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0
        }
    };

    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();

    // Create UI
    bsp_display_lock(-1);
    create_ui();
    bsp_display_unlock();

    ESP_LOGI(TAG, "Initialization complete");

    // Keep app running
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
