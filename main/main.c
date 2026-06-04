#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdint.h>
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
#include "esp_codec_dev.h"

// Weather system integration
#include "GUI.h"
#include "weather/weather_task.h"
#include "weather/weather_data.h"
#include "weather/wmo_icon_map.h"
#include "storage/nvs_storage.h"
#include "ui/data_binding.h"

static const char *TAG = "DeskMediaDevice";

#define AUDIO_SAMPLE_RATE       16000
#define AUDIO_MCLK_MULTIPLE     384
#define AUDIO_MCLK_FREQ_HZ      (AUDIO_SAMPLE_RATE * AUDIO_MCLK_MULTIPLE)
#define AUDIO_BUFFER_SIZE       512
#define AUDIO_TYPE_PCM          0
#define AUDIO_TYPE_WAV          1

static char pcm_file_path[256] = "";
static char wav_file_path[256] = "";

static bool is_playing = false;
static TaskHandle_t audio_task_handle = NULL;
static esp_codec_dev_handle_t spk_codec_dev = NULL;

static void scan_sd_card(void);
static void create_ui(void);
static esp_err_t i2s_init(void);
static esp_err_t codec_init(void);
static void audio_task(void *param);

static esp_err_t i2s_init(void)
{
    i2s_std_config_t i2s_cfg = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(AUDIO_SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = BSP_I2S_MCLK,
            .bclk = BSP_I2S_SCLK,
            .ws   = BSP_I2S_LCLK,
            .dout = BSP_I2S_DOUT,
            .din  = BSP_I2S_DSIN,
            .invert_flags.mclk_inv = false,
        },
    };
    i2s_cfg.clk_cfg.mclk_multiple = AUDIO_MCLK_MULTIPLE;
    esp_err_t ret = bsp_audio_init(&i2s_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "bsp_audio_init failed: %s", esp_err_to_name(ret));
    }
    return ret;
}

static esp_err_t codec_init(void)
{
    ESP_LOGI(TAG, "Initializing ES8311 codec...");
    spk_codec_dev = bsp_audio_codec_speaker_init();
    if (spk_codec_dev == NULL) {
        ESP_LOGE(TAG, "bsp_audio_codec_speaker_init failed");
        return ESP_FAIL;
    }
    esp_codec_dev_sample_info_t fs = {
        .sample_rate     = AUDIO_SAMPLE_RATE,
        .channel         = 2,
        .bits_per_sample = 16,
    };
    int ret = esp_codec_dev_open(spk_codec_dev, &fs);
    if (ret != ESP_CODEC_DEV_OK) {
        ESP_LOGE(TAG, "esp_codec_dev_open failed: %d", ret);
        return ESP_FAIL;
    }
    esp_codec_dev_set_out_vol(spk_codec_dev, 80);
    ESP_LOGI(TAG, "Codec initialized, volume set to 80%%");
    return ESP_OK;
}

static void audio_task(void *param)
{
    int audio_type = (int)(uintptr_t)param;
    const char *file_path = (audio_type == AUDIO_TYPE_PCM) ? pcm_file_path : wav_file_path;
    ESP_LOGI(TAG, "Playing %s: %s", audio_type == AUDIO_TYPE_PCM ? "PCM" : "WAV", file_path);

    FILE *f = fopen(file_path, "rb");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open file: %s", file_path);
        is_playing = false;
        vTaskDelete(NULL);
        return;
    }

    if (audio_type == AUDIO_TYPE_WAV) {
        fseek(f, 44, SEEK_SET);
    }

    uint8_t buffer[AUDIO_BUFFER_SIZE];
    while (is_playing) {
        size_t bytes_read = fread(buffer, 1, AUDIO_BUFFER_SIZE, f);
        if (bytes_read == 0) { ESP_LOGI(TAG, "End of file reached"); break; }
        int written = esp_codec_dev_write(spk_codec_dev, buffer, (int)bytes_read);
        if (written < 0) { ESP_LOGE(TAG, "codec write failed: %d", written); break; }
    }

    memset(buffer, 0, AUDIO_BUFFER_SIZE);
    for (int i = 0; i < 4; i++) {
        esp_codec_dev_write(spk_codec_dev, buffer, AUDIO_BUFFER_SIZE);
    }

    fclose(f);
    is_playing = false;
    ESP_LOGI(TAG, "Playback finished");
    vTaskDelete(NULL);
}

static void scan_sd_card(void)
{
    esp_err_t ret = bsp_sdcard_mount();
    if (ret != ESP_OK) { ESP_LOGE(TAG, "Failed to mount SD card: %s", esp_err_to_name(ret)); return; }
    ESP_LOGI(TAG, "SD card mounted successfully at /sdcard");
    DIR *dir = opendir("/sdcard");
    if (!dir) { ESP_LOGE(TAG, "Failed to open /sdcard directory"); return; }
    struct dirent *entry;
    int file_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char full_path[256];
            snprintf(full_path, sizeof(full_path), "/sdcard/%s", entry->d_name);
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
    if (file_count > 0) { ESP_LOGI(TAG, "Found %d audio file(s) on SD card", file_count); }
    else { ESP_LOGW(TAG, "No audio files found on SD card"); }
}

static void stop_callback(lv_event_t *e)      { is_playing = false; ESP_LOGI(TAG, "Audio stopped"); }

static void play_pcm_callback(lv_event_t *e)
{
    if (pcm_file_path[0] == '\0') { ESP_LOGW(TAG, "PCM file not found"); return; }
    if (is_playing) { ESP_LOGW(TAG, "Already playing"); return; }
    is_playing = true;
    xTaskCreate(audio_task, "audio_task", 4096, (void *)(uintptr_t)AUDIO_TYPE_PCM, 5, &audio_task_handle);
}

static void play_wav_callback(lv_event_t *e)
{
    if (wav_file_path[0] == '\0') { ESP_LOGW(TAG, "WAV file not found"); return; }
    if (is_playing) { ESP_LOGW(TAG, "Already playing"); return; }
    is_playing = true;
    xTaskCreate(audio_task, "audio_task", 4096, (void *)(uintptr_t)AUDIO_TYPE_WAV, 5, &audio_task_handle);
}

static void create_ui(void)
{
    // Initialize SquareLine UI
    // Note: GUI_init() must be called AFTER bsp_display_start_with_config()
    // and INSIDE bsp_display_lock() to avoid timing issues
    ESP_LOGI(TAG, "Initializing SquareLine UI");
    GUI_init();

    // Set background image on home screen
    lv_obj_set_style_bg_image_src(GUI_Screen__home, &upload_hclbg1_52bba57ce173452fadd7595a14167a99_png, 0);
}

void app_main(void)
{
    gpio_set_direction(BSP_LCD_RST, GPIO_MODE_OUTPUT);
    gpio_set_level(BSP_LCD_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(BSP_LCD_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(120));

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Starting Desk Media Device");
    scan_sd_card();

    bsp_display_cfg_t cfg = {
        .lv_adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG(),
        .rotation = ESP_LV_ADAPTER_ROTATE_90,
        .tear_avoid_mode = ESP_LV_ADAPTER_TEAR_AVOID_MODE_DOUBLE_FULL,
        .touch_flags = { .swap_xy = 1, .mirror_x = 0, .mirror_y = 0 }
    };

    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();

    bsp_display_lock(-1);
    create_ui();
    lv_obj_invalidate(lv_scr_act());
    bsp_display_unlock();

    vTaskDelay(pdMS_TO_TICKS(500));

    ret = i2s_init();
    if (ret != ESP_OK) { ESP_LOGE(TAG, "I2S init failed"); }

    ret = codec_init();
    if (ret != ESP_OK) { ESP_LOGE(TAG, "Codec init failed"); }

    // Initialize weather system
    ESP_LOGI(TAG, "Initializing weather system");
    ret = nvs_storage_init();
    if (ret != ESP_OK) { ESP_LOGW(TAG, "NVS storage init failed: %s", esp_err_to_name(ret)); }

    ret = weather_task_start();
    if (ret != ESP_OK) { ESP_LOGW(TAG, "Weather task start failed: %s", esp_err_to_name(ret)); }

    ESP_LOGI(TAG, "Initialization complete");
    while (1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
}