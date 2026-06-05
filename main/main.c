#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mp3dec.h"
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
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "lvgl.h"
#include "bsp/esp-bsp.h"
#include "bsp/display.h"
#include "bsp_board_extra.h"
#include "esp_codec_dev.h"

// Weather system integration
#include "GUI.h"
#include "audio.h"
#include "settings.h"
#include "weather/weather_task.h"
#include "weather/weather_data.h"
#include "weather/wmo_icon_map.h"
#include "storage/nvs_storage.h"
#include "ui/data_binding.h"
#include "wifi_manager.h"
#include "weather_display.h"
#include "weather_icons.h"

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


// ── Public audio API ─────────────────────────────────────────────────────────

static void tick_task(void *arg)
{
    (void)arg;
    if (spk_codec_dev == NULL) { vTaskDelete(NULL); return; }

    // Generate a short 880Hz sine burst (20ms @ 16kHz = 320 samples)
    const int SAMPLES = 320;
    int16_t buf[SAMPLES * 2]; // stereo
    for (int i = 0; i < SAMPLES; i++) {
        float t = (float)i / 16000.0f;
        float env = (i < 80) ? (float)i / 80.0f : (float)(SAMPLES - i) / (float)(SAMPLES - 80);
        int16_t s = (int16_t)(env * 8000.0f * sinf(2.0f * 3.14159f * 880.0f * t));
        buf[i * 2]     = s;
        buf[i * 2 + 1] = s;
    }
    esp_codec_dev_write(spk_codec_dev, buf, sizeof(buf));
    vTaskDelete(NULL);
}

void audio_play_tick(void)
{
    xTaskCreate(tick_task, "tick", 4096, NULL, 6, NULL);
}

void audio_set_volume(int percent)
{
    if (spk_codec_dev == NULL) return;
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    esp_codec_dev_set_out_vol(spk_codec_dev, percent);
}

void audio_play_wav_preview(void)
{
    if (wav_file_path[0] == '\0') return;
    if (is_playing) return;
    xTaskCreate(audio_task, "audio_preview", 4096,
                (void *)(uintptr_t)AUDIO_TYPE_WAV, 5, &audio_task_handle);
}

static void success_task(void *arg)
{
    (void)arg;
    if (spk_codec_dev == NULL) { vTaskDelete(NULL); return; }
    FILE *f = fopen("/sdcard/SUCCESS.WAV", "rb");
    if (!f) { ESP_LOGW("Audio", "SUCCESS.WAV not found"); vTaskDelete(NULL); return; }
    fseek(f, 44, SEEK_SET);  // skip WAV header
    uint8_t buf[512];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0)
        esp_codec_dev_write(spk_codec_dev, buf, (int)n);
    fclose(f);
    vTaskDelete(NULL);
}

void audio_play_success(void)
{
    if (spk_codec_dev == NULL) return;
    xTaskCreate(success_task, "success_snd", 4096, NULL, 5, NULL);
}


// ── MP3 playback ─────────────────────────────────────────────────────────────

#define MP3_READ_BUF_SIZE   2048
#define MP3_PCM_BUF_FRAMES  1152  // max frames per MP3 frame

typedef struct {
    char path[256];
} mp3_play_args_t;

static void mp3_task(void *arg)
{
    mp3_play_args_t *args = (mp3_play_args_t *)arg;
    if (spk_codec_dev == NULL) { free(args); vTaskDelete(NULL); return; }

    FILE *f = fopen(args->path, "rb");
    free(args);
    if (!f) { ESP_LOGW("MP3", "File not found"); vTaskDelete(NULL); return; }

    HMP3Decoder decoder = MP3InitDecoder();
    if (!decoder) { fclose(f); vTaskDelete(NULL); return; }

    uint8_t *read_buf = malloc(MP3_READ_BUF_SIZE);
    short   *pcm_buf  = malloc(MP3_PCM_BUF_FRAMES * 2 * sizeof(short)); // stereo
    if (!read_buf || !pcm_buf) {
        free(read_buf); free(pcm_buf);
        MP3FreeDecoder(decoder); fclose(f); vTaskDelete(NULL); return;
    }

    int bytes_in_buf = 0;
    uint8_t *read_ptr = read_buf;

    while (1) {
        // Refill buffer
        int space = MP3_READ_BUF_SIZE - bytes_in_buf;
        if (space > 0 && read_ptr != read_buf) {
            memmove(read_buf, read_ptr, bytes_in_buf);
            read_ptr = read_buf;
        }
        if (space > 0) {
            size_t n = fread(read_buf + bytes_in_buf, 1, space, f);
            bytes_in_buf += (int)n;
            if (n == 0 && bytes_in_buf == 0) break; // EOF
        }

        // Find sync word
        int offset = MP3FindSyncWord(read_ptr, bytes_in_buf);
        if (offset < 0) { bytes_in_buf = 0; read_ptr = read_buf; continue; }
        read_ptr    += offset;
        bytes_in_buf -= offset;

        // Decode one frame
        unsigned char *rp = read_ptr;
        int bl = bytes_in_buf;
        int err = MP3Decode(decoder, &rp, &bl, pcm_buf, 0);
        if (err != 0) {
            read_ptr++; bytes_in_buf--;
            continue;
        }
        int consumed = bytes_in_buf - bl;
        read_ptr    += consumed;
        bytes_in_buf = bl;

        MP3FrameInfo info;
        MP3GetLastFrameInfo(decoder, &info);
        int samples = info.outputSamps; // total shorts (channels * frames)
        esp_codec_dev_write(spk_codec_dev, pcm_buf, samples * sizeof(short));
    }

    free(read_buf);
    free(pcm_buf);
    MP3FreeDecoder(decoder);
    fclose(f);
    vTaskDelete(NULL);
}

void audio_play_mp3(const char *path)
{
    if (spk_codec_dev == NULL) return;
    mp3_play_args_t *args = malloc(sizeof(mp3_play_args_t));
    if (!args) return;
    strncpy(args->path, path, sizeof(args->path) - 1);
    args->path[sizeof(args->path)-1] = '\0';
    xTaskCreate(mp3_task, "mp3_play", 8192, args, 5, NULL);
}

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

    // Set clean default values on all weather widgets — prevents blank/junk display
    ui_set_default_weather();


    // Wire up settings button and settings screen
    settings_ui_init();
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
    ESP_ERROR_CHECK(nvs_storage_init());

    // Required for esp_hosted / esp_wifi_remote
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "WiFi initialized in STA mode");

    // Init WiFi manager event handlers and connect with saved credentials
    wifi_manager_init();
    wifi_manager_connect_saved();

    ESP_LOGI(TAG, "Starting Desk Media Device");
    scan_sd_card();

    bsp_display_cfg_t cfg = {
        .lv_adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG(),
        .rotation = ESP_LV_ADAPTER_ROTATE_90,
        .tear_avoid_mode = ESP_LV_ADAPTER_TEAR_AVOID_MODE_DOUBLE_FULL,
        .touch_flags = { .swap_xy = 1, .mirror_x = 1, .mirror_y = 0 }
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

    ret = weather_task_start();
    if (ret != ESP_OK) { ESP_LOGW(TAG, "Weather task start failed: %s", esp_err_to_name(ret)); }

    // Load saved zip code and kick off weather fetch
    char saved_zip[16] = "";
    if (nvs_load_zipcode(saved_zip, sizeof(saved_zip)) != ESP_OK || strlen(saved_zip) == 0) {
        // No zip saved — write the default so it persists
        strncpy(saved_zip, "88002", sizeof(saved_zip) - 1);
        nvs_store_zipcode(saved_zip);
        ESP_LOGI(TAG, "No zip code saved — using default: %s", saved_zip);
    } else {
        ESP_LOGI(TAG, "Loaded zip code from NVS: %s", saved_zip);
    }
    weather_set_location(saved_zip);

    ESP_LOGI(TAG, "Initialization complete");

    // Main loop — poll weather data and update display
    weather_data_t api_data = {0};
    uint32_t last_displayed_update = 0;

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000)); // check every 5 seconds

        if (weather_get_data(&api_data) == ESP_OK &&
            api_data.last_update != last_displayed_update) {

            last_displayed_update = api_data.last_update;

            // Convert weather_data_t → weather_display_t and update UI
            weather_display_t disp = {0};

            disp.current_temp_f   = api_data.current_temp;
            disp.feels_like_f     = api_data.current_apparent_temp;
            disp.current_humidity = (int)api_data.current_humidity;
            disp.current_wind_mph  = api_data.current_wind_speed;
            disp.current_precip_in = api_data.current_precip;
            // Wind string: speed + compass direction, no space e.g. "9NW"
            const char *compass[] = {"N","NE","E","SE","S","SW","W","NW"};
            const char *dir = compass[((api_data.current_wind_direction + 22) % 360) / 45];
            snprintf(disp.current_wind_str, sizeof(disp.current_wind_str),
                     "%.0f%s", api_data.current_wind_speed, dir);
            disp.current_wmo      = api_data.current_weather_code;
            disp.is_night         = (api_data.current_is_day == 0);
            disp.wifi_connected   = wifi_manager_is_connected();

            strncpy(disp.city,  api_data.city,  sizeof(disp.city) - 1);
            strncpy(disp.state, api_data.state, sizeof(disp.state) - 1);

            // Time from SNTP — apply UTC offset from API
            time_t now = time(NULL);
            ESP_LOGI(TAG, "SNTP time: %lu, utc_offset: %ld", (unsigned long)now, (long)api_data.utc_offset_seconds);
            if (now > 1700000000) { // valid if past Nov 2023
                time_t local = now + api_data.utc_offset_seconds;
                struct tm *tm_info = gmtime(&local);
                if (tm_info) {
                    char tmp[12] = {0};
                    strftime(tmp, sizeof(tmp), "%I:%M%p", tm_info);
                    // Strip leading zero: "04:10PM" -> "4:10PM"
                    const char *ts = (tmp[0] == '0') ? tmp + 1 : tmp;
                    size_t ts_len = strlen(ts);
                    size_t tc_len = ts_len < sizeof(disp.time_str) - 1 ? ts_len : sizeof(disp.time_str) - 1;
                    memcpy(disp.time_str, ts, tc_len);
                    disp.time_str[tc_len] = '\0';
                }
            }

            // Hourly
            for (int i = 0; i < 4; i++) {
                disp.hourly_temp_f[i] = api_data.hourly.hours[i].temperature;
                disp.hourly_wmo[i]    = api_data.hourly.hours[i].weather_code;
                strncpy(disp.hourly_time[i], api_data.hourly.times_12h[i],
                        sizeof(disp.hourly_time[i]) - 1);
            }

            // Daily — use weather description, not date string
            for (int i = 0; i < 3; i++) {
                disp.daily_high_f[i] = api_data.daily[i].temp_high;
                disp.daily_low_f[i]  = api_data.daily[i].temp_low;
                disp.daily_wmo[i]    = api_data.daily[i].weather_code;
                strncpy(disp.daily_day[i], api_data.daily[i].day_name,
                        sizeof(disp.daily_day[i]) - 1);
                strncpy(disp.daily_status[i], weather_description(api_data.daily[i].weather_code),
                        sizeof(disp.daily_status[i]) - 1);
            }

            bsp_display_lock(100);
            weather_display_update(&disp);
            bsp_display_unlock();

            ESP_LOGI(TAG, "UI updated: %.1f°F %s", disp.current_temp_f, disp.city);
        }

        // Update network status icon every loop
        bsp_display_lock(100);
        weather_display_update_network(wifi_manager_is_connected());
        bsp_display_unlock();
    }
}