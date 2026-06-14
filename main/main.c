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
#include "driver/i2c_master.h"
#include "esp_attr.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "lvgl.h"
#include "bsp/esp-bsp.h"
#include "bsp/display.h"
#include "bsp_board_extra.h"
#include "esp_codec_dev.h"
#include "esp_heap_caps.h"

// Weather system integration
#include "GUI.h"
#include "audio.h"
#include "ota_update.h"
#include "content_sync.h"
#include "power_gate.h"
#include "settings.h"
#include "weather/weather_task.h"
#include "weather/weather_data.h"
#include "weather/wmo_icon_map.h"
#include "storage/nvs_storage.h"
#include "ui/data_binding.h"
#include "wifi_manager.h"
#include "weather_display.h"
#include "weather_icons.h"

// ── WiFi disconnect reason codes ─────────────────────────────────────────────
#define WIFI_REASON_INTENTIONAL     8
#define WIFI_REASON_ASSOC_LEAVE     15
#define WIFI_REASON_AUTH_FAIL       202
#define WIFI_REASON_NO_AP_FOUND     205

static const char *TAG = "DeskMediaDevice";

#define AUDIO_SAMPLE_RATE       32000  // matches re-encoded MP3s; mp3_task reconfigures per-file

// ── Tuning knobs — easy to flip for testing ──────────────────────────────────
#define TICK_TASK_PRIORITY      4   // button click sound
#define MP3_TASK_PRIORITY       5   // below LVGL (6) so LVGL always wins touch scheduling
#define SCROLL_TIMER_MS         67  // title scroll interval — 67=15fps, 50=20fps, 40=25fps
#define SCROLL_STEP_PX          3   // pixels per scroll tick
// NOTE: CONFIG_LV_DEF_REFR_PERIOD in sdkconfig left at 15 — raising it hurts touch responsiveness
#define STATUS_BAR_SLIDE_IN_MS  500  // slide down into view — slower feels more intentional
#define STATUS_BAR_SLIDE_OUT_MS 300  // slide up out of view — quicker, just getting out of the way
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
// Codec is fixed at AUDIO_SAMPLE_RATE (32000Hz) — all MP3s must be encoded at this rate

// ── MP3 playlist ──────────────────────────────────────────────────────────────
#define MP3_MAX_TRACKS 32

static char mp3_playlist[MP3_MAX_TRACKS][256];
static int  mp3_track_count   = 0;
static int  mp3_current_track = 0;
static volatile bool mp3_stop_requested = false;
static volatile bool mp3_is_playing     = false;
static volatile bool mp3_pause_requested = false;
static volatile bool fx_playing          = false; // success/preview WAV writing to codec
static TaskHandle_t  mp3_task_handle    = NULL;

// ── UTC offset from last weather fetch (for time display) ────────────────────
static int32_t g_utc_offset_seconds = 0;

// ── Weather status overlay ────────────────────────────────────────────────────
static lv_obj_t *s_weather_overlay     = NULL;
static lv_obj_t *s_weather_overlay_lbl = NULL;

// ── Memory display label ──────────────────────────────────────────────────────
static lv_obj_t *mem_label = NULL;

// ── Home screen timer handles — reset after video playback to prevent burst ──
static lv_timer_t *s_clock_timer         = NULL;
static lv_timer_t *s_music_watchdog_timer = NULL;
static lv_timer_t *s_mem_label_timer     = NULL;
static lv_timer_t *s_weather_overlay_timer = NULL;

static void mem_label_cb(lv_timer_t *t)
{
    (void)t;
    if (!mem_label) return;
    if (lv_scr_act() != GUI_Screen__home) return;
    size_t free_internal = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    size_t free_psram    = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    lv_label_set_text_fmt(mem_label, "INT:%uK PSRAM:%uK",
                          (unsigned)(free_internal / 1024),
                          (unsigned)(free_psram    / 1024));
}

// ── 1-second LVGL timer to update the clock ──────────────────────────────────
static void clock_timer_cb(lv_timer_t *t)
{
    (void)t;
    if (lv_scr_act() != GUI_Screen__home) return;
    time_t now = time(NULL);
    if (now < 1700000000) return;         // SNTP not synced yet
    if (g_utc_offset_seconds == 0) return; // UTC offset not yet set by weather API
    time_t local = now + g_utc_offset_seconds;
    struct tm *tm_info = gmtime(&local);
    if (!tm_info) return;
    char tmp[16] = {0};
    strftime(tmp, sizeof(tmp), "%I:%M%p", tm_info);
    const char *ts = (tmp[0] == '0') ? tmp + 1 : tmp;
    lv_label_set_text(GUI_Label__home__CURRENTTIMEQ, ts);
}

// Forward declaration — anim_y_cb defined later in file
static void anim_y_cb(void *obj, int32_t y);

// Set when the user tries to start playback while a network burst is running;
// drives the "WEATHER UPDATING - PLEASE WAIT" banner. Cleared once the burst ends.
static volatile bool g_net_wait_warning = false;

// ── Status bar animation complete — hide after slide-up ──────────────────────
static void status_bar_hide_cb(lv_anim_t *a)
{
    lv_obj_add_flag((lv_obj_t *)a->var, LV_OBJ_FLAG_HIDDEN);
}

// ── Weather status overlay timer (500ms) ─────────────────────────────────────
// Logic:
//   - No overlay if WiFi connected AND weather data is valid (happy path)
//   - "LOADING WEATHER DATA"        — WiFi up, fetch in progress
//   - "WIFI REQUIRED FOR WEATHER DATA" — not connected, never had data, no error
//   - "RECONNECTING TO WIFI — RETRY IN NNs" — persistent reconnect, live countdown
//   - "WRONG PASSWORD"              — disconnected with reason 15 or 202
//   - "NETWORK NOT FOUND"           — disconnected with reason 205
//   - "WIFI CONNECTION FAILED"      — max retries, other reason
static void weather_overlay_cb(lv_timer_t *t)
{
    (void)t;
    if (!s_weather_overlay) return;
    if (lv_scr_act() != GUI_Screen__home) return;

    bool connected    = wifi_manager_is_connected();
    bool connecting   = wifi_manager_is_connecting();
    bool reconnecting = wifi_manager_is_reconnecting();
    bool fetching     = weather_task_is_fetching();
    bool data_valid   = (weather_get_last_update() > 0);
    uint8_t reason    = wifi_manager_get_last_disconnect_reason();

    // Dynamic countdown text needs a stable buffer (built fresh each tick below).
    static char s_reconnect_buf[48];

    // Clear the playback-blocked warning once the network burst has finished.
    if (!power_gate_net_busy()) g_net_wait_warning = false;

    const char *msg  = NULL;

    if (g_net_wait_warning && power_gate_net_busy()) {
        // User tried to play a clip/track while a network update (weather, OTA,
        // or content sync) was running — ask them to wait so the radio burst and
        // playback don't peak together.
        msg = "UPDATING - PLEASE WAIT";
    } else if (connected && data_valid && !fetching) {
        // All good — hide overlay
    } else if (connected && (fetching || !data_valid)) {
        msg = "LOADING WEATHER DATA";
    } else if (reconnecting) {
        // Lost a working connection (or rebooted while the AP is down) — keep
        // trying every 30 s and show the live countdown to the next attempt.
        int secs = wifi_manager_get_reconnect_countdown();
        snprintf(s_reconnect_buf, sizeof(s_reconnect_buf),
                 "RECONNECTING TO WIFI - RETRY IN %ds", secs);
        msg = s_reconnect_buf;
    } else if (connecting) {
        // Active attempt in progress — always show this, overrides any stale error
        msg = "CONNECTING TO WIFI...";
    } else if (!connected) {
        if (reason == WIFI_REASON_ASSOC_LEAVE || reason == WIFI_REASON_AUTH_FAIL) {
            msg = "WRONG PASSWORD";
        } else if (reason == WIFI_REASON_NO_AP_FOUND) {
            msg = "NETWORK NOT FOUND";
        } else if (reason != 0 && reason != WIFI_REASON_INTENTIONAL) {
            msg = "WIFI CONNECTION FAILED";
        } else {
            msg = "WIFI REQUIRED FOR WEATHER DATA";
        }
    }

    // Only touch LVGL if the message actually changed
    static const char *s_last_msg = NULL;
    // The reconnect countdown reuses one buffer but its text ticks every second —
    // refresh the label in place without re-running the slide-in animation.
    if (msg == s_reconnect_buf && s_last_msg == s_reconnect_buf) {
        lv_label_set_text(s_weather_overlay_lbl, msg);
        return;
    }
    if (msg == s_last_msg) return;
    s_last_msg = msg;

    if (msg == NULL) {
        // Slide up off screen then hide
        lv_obj_clear_flag(s_weather_overlay, LV_OBJ_FLAG_HIDDEN);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, s_weather_overlay);
        lv_anim_set_exec_cb(&a, anim_y_cb);
        lv_anim_set_values(&a, -221, -266);
        lv_anim_set_duration(&a, STATUS_BAR_SLIDE_OUT_MS);
        lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
        lv_anim_set_completed_cb(&a, status_bar_hide_cb);
        lv_anim_start(&a);
    } else {
        // Background and text color by message type
        lv_color_t bg_color;
        lv_color_t txt_color;
        if (!reconnecting &&
            (reason == WIFI_REASON_ASSOC_LEAVE || reason == WIFI_REASON_AUTH_FAIL ||
             reason == WIFI_REASON_NO_AP_FOUND ||
             (reason != 0 && reason != WIFI_REASON_INTENTIONAL && !connected && !connecting))) {
            // Error states — red background, black text
            bg_color  = lv_color_hex(0xCC2222);
            txt_color = lv_color_hex(0x000000);
        } else if (!connected && !connecting && !reconnecting) {
            // No WiFi, no error yet — orange warning, black text
            bg_color  = lv_color_hex(0xCC6600);
            txt_color = lv_color_hex(0x000000);
        } else {
            // Connecting / reconnecting / loading — dark gray, white text
            bg_color  = lv_color_hex(0x222222);
            txt_color = lv_color_hex(0xFFFFFF);
        }
        lv_obj_set_style_bg_color(s_weather_overlay, bg_color, 0);
        lv_obj_set_style_text_color(s_weather_overlay_lbl, txt_color, 0);
        lv_label_set_text(s_weather_overlay_lbl, msg);
        lv_obj_clear_flag(s_weather_overlay, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(s_weather_overlay);
        // Slide down into view from parked position
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, s_weather_overlay);
        lv_anim_set_exec_cb(&a, anim_y_cb);
        lv_anim_set_values(&a, -266, -221);
        lv_anim_set_duration(&a, STATUS_BAR_SLIDE_IN_MS);
        lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
        lv_anim_start(&a);
    }
}

// ── Music player slide state ──────────────────────────────────────────────────
static bool music_player_open  = false;
static bool music_scroll_active = false;
static lv_anim_t musiclabel_anim;
static lv_timer_t *scroll_timer = NULL;
static int32_t scroll_x = 0;
static int32_t scroll_unit_w = 0;

// SquareLine canvas 800w x 480h, center-origin positions
// music_player: rest pos (-132, 115), hidden off bottom at y=400
// musicbuttoncont: starts at same x as music_player (-132), shifts right when open
#define MUSIC_PLAYER_Y_REST    115
#define MUSIC_PLAYER_Y_HIDDEN  400
#define MUSICBTN_X_REST        -132   // same x as music_player, sits next to weather widget
#define MUSICBTN_X_OPEN         -68   // just to the right of music_player when open

// Scroll panel width from SquareLine (424px) used for animation range
#define ML_W  424

static void scan_sd_card(void);
static void create_ui(void);
static esp_err_t i2s_init(void);

// ── Video player ──────────────────────────────────────────────────────────────
#include "video_player.h"
lv_display_t *g_lv_disp = NULL;
lv_obj_t     *g_video_fade_overlay = NULL;

// ── System ready flag — gates music player and video touch until fully booted ─
static volatile bool g_system_ready = false;

static void skull_touch_cb(lv_event_t *e)
{
    (void)e;
    if (!g_system_ready) return;
    if (power_gate_net_busy()) {
        // A weather/OTA/content burst is on the air — don't peak on top of it.
        g_net_wait_warning = true;
        ESP_LOGW(TAG, "Video held off — network update in progress");
        return;
    }
    ESP_LOGI(TAG, "Skull logo touched — starting video player");
    video_player_start();
}
static esp_err_t codec_init(void);
static void audio_task(void *param);
static void mp3_stop(void);
static void music_scroll_stop(void);

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
    int saved_vol = nvs_load_volume(80);
    audio_set_volume(saved_vol);   // routes through the 95% output cap
    bool saved_mute = nvs_load_mute(false);
    if (saved_mute) {
        esp_codec_dev_set_out_mute(spk_codec_dev, true);
    }
    ESP_LOGI(TAG, "Codec initialized, volume %d%%, mute %s", saved_vol, saved_mute ? "ON" : "OFF");
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

    // Scan root for WAV/PCM
    DIR *dir = opendir("/sdcard");
    if (!dir) { ESP_LOGE(TAG, "Failed to open /sdcard directory"); return; }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char full_path[256];
            snprintf(full_path, sizeof(full_path), "/sdcard/%s", entry->d_name);
            if (strstr(entry->d_name, ".pcm") || strstr(entry->d_name, ".PCM")) {
                snprintf(pcm_file_path, sizeof(pcm_file_path), "%s", full_path);
                ESP_LOGI(TAG, "Found PCM: %s", pcm_file_path);
            } else if (strstr(entry->d_name, ".wav") || strstr(entry->d_name, ".WAV")) {
                snprintf(wav_file_path, sizeof(wav_file_path), "%s", full_path);
                ESP_LOGI(TAG, "Found WAV: %s", wav_file_path);
            }
        }
    }
    closedir(dir);

    // Scan /sdcard/music/ for MP3s
    mp3_track_count = 0;
    DIR *mdir = opendir("/sdcard/music");
    if (!mdir) {
        ESP_LOGW(TAG, "No /sdcard/music folder found — no MP3s loaded");
        return;
    }
    // Collect names first so we can sort them
    char names[MP3_MAX_TRACKS][128];
    int name_count = 0;
    while ((entry = readdir(mdir)) != NULL && name_count < MP3_MAX_TRACKS) {
        if (entry->d_type == DT_REG &&
            (strstr(entry->d_name, ".mp3") || strstr(entry->d_name, ".MP3"))) {
            snprintf(names[name_count], sizeof(names[0]), "%s", entry->d_name);
            name_count++;
        }
    }
    closedir(mdir);

    // Sort alphabetically so track order matches filename order (01, 02, 03...)
    for (int i = 0; i < name_count - 1; i++) {
        for (int j = i + 1; j < name_count; j++) {
            if (strcmp(names[i], names[j]) > 0) {
                char tmp[128];
                memcpy(tmp,       names[i], sizeof(tmp));
                memcpy(names[i],  names[j], sizeof(names[0]));
                memcpy(names[j],  tmp,      sizeof(names[0]));
            }
        }
    }

    for (int i = 0; i < name_count; i++) {
        snprintf(mp3_playlist[i], sizeof(mp3_playlist[0]), "/sdcard/music/%s", names[i]);
        ESP_LOGI(TAG, "MP3 [%d]: %s", i, mp3_playlist[i]);
    }
    mp3_track_count = name_count;
    ESP_LOGI(TAG, "Loaded %d MP3 track(s) from /sdcard/music", mp3_track_count);
}

static void stop_callback(lv_event_t *e)      { is_playing = false; ESP_LOGI(TAG, "Audio stopped"); }


// ── Public audio API ─────────────────────────────────────────────────────────

static void tick_task(void *arg)
{
    (void)arg;
    if (spk_codec_dev == NULL) { vTaskDelete(NULL); return; }
    // Skip tick during MP3 playback — codec is owned by mp3_task, write would block/corrupt.
    // A PAUSED mp3 task isn't writing, so ticks are fine (settings screen relies on this).
    if (mp3_is_playing && !mp3_pause_requested) { vTaskDelete(NULL); return; }

    // Generate a short 880Hz sine burst (20ms @ 32kHz = 640 samples)
    const int SAMPLES = 640;
    int16_t buf[SAMPLES * 2]; // stereo
    for (int i = 0; i < SAMPLES; i++) {
        float t = (float)i / (float)AUDIO_SAMPLE_RATE;
        float env = (i < 160) ? (float)i / 160.0f : (float)(SAMPLES - i) / (float)(SAMPLES - 160);
        int16_t s = (int16_t)(env * 8000.0f * sinf(2.0f * 3.14159f * 880.0f * t));
        buf[i * 2]     = s;
        buf[i * 2 + 1] = s;
    }
    esp_codec_dev_write(spk_codec_dev, buf, sizeof(buf));
    vTaskDelete(NULL);
}

void audio_play_tick(void)
{
    xTaskCreate(tick_task, "tick", 4096, NULL, TICK_TASK_PRIORITY, NULL);
}

void audio_set_volume(int percent)
{
    if (spk_codec_dev == NULL) return;
    if (percent < 0) percent = 0;
    // Cap real output at 90%: high amp current browns out the board on the
    // current supply (mp3 crashes). The slider still shows up to 100 — the top
    // maps to 90.
    if (percent > 90) percent = 90;
    esp_codec_dev_set_out_vol(spk_codec_dev, percent);
}

void audio_set_mute(bool mute)
{
    if (spk_codec_dev == NULL) return;
    esp_codec_dev_set_out_mute(spk_codec_dev, mute);
    ESP_LOGI(TAG, "Speaker %s", mute ? "muted" : "unmuted");
}

bool music_panel_is_open(void)
{
    return music_player_open;
}

void audio_music_set_paused(bool paused)
{
    mp3_pause_requested = paused;
    if (mp3_is_playing) {
        ESP_LOGI(TAG, "Music %s", paused ? "paused" : "resumed");
    }
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
    // fx_playing was set in audio_play_success(); every exit must clear it or
    // the mp3 task stays parked forever.
    if (spk_codec_dev == NULL) { fx_playing = false; vTaskDelete(NULL); return; }
    FILE *f = fopen("/sdcard/SUCCESS.WAV", "rb");
    if (!f) { ESP_LOGW("Audio", "SUCCESS.WAV not found"); fx_playing = false; vTaskDelete(NULL); return; }
    fseek(f, 44, SEEK_SET);  // skip WAV header
    uint8_t buf[512];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0)
        esp_codec_dev_write(spk_codec_dev, buf, (int)n);
    fclose(f);
    fx_playing = false;
    vTaskDelete(NULL);
}

void audio_play_success(void)
{
    if (spk_codec_dev == NULL) return;
    // Set BEFORE the task exists: a paused mp3 task polls every 50ms and must
    // never see a window where resume was requested but the effect hasn't
    // claimed the codec yet.
    fx_playing = true;
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
    if (spk_codec_dev == NULL) { free(args); mp3_is_playing = false; vTaskDelete(NULL); return; }

    FILE *f = fopen(args->path, "rb");
    free(args);
    if (!f) { ESP_LOGW("MP3", "File not found"); mp3_is_playing = false; vTaskDelete(NULL); return; }

    HMP3Decoder decoder = MP3InitDecoder();
    if (!decoder) { fclose(f); mp3_is_playing = false; vTaskDelete(NULL); return; }

    uint8_t *read_buf = malloc(MP3_READ_BUF_SIZE);
    short   *pcm_buf  = malloc(MP3_PCM_BUF_FRAMES * 2 * sizeof(short));
    if (!read_buf || !pcm_buf) {
        free(read_buf); free(pcm_buf);
        MP3FreeDecoder(decoder); fclose(f); mp3_is_playing = false; vTaskDelete(NULL); return;
    }

    // ── Pop/click suppression — PA gate ──────────────────────────────────────
    // Mute/volume/silence-flush approaches all failed in previous attempts:
    // the ES8311 digital controls don't reach the analog output fast enough.
    // Direct PA GPIO control is the only path that reliably suppresses the pop.
    // MP3 files must have ~0.5s silence at the start (added via FFmpeg) so the
    // PA comes up into guaranteed silence before any real audio hits the speaker.
    gpio_set_level(BSP_POWER_AMP_IO, 0);
    {
        int16_t silence[MP3_PCM_BUF_FRAMES * 2];
        memset(silence, 0, sizeof(silence));
        esp_codec_dev_write(spk_codec_dev, silence, sizeof(silence));
        esp_codec_dev_write(spk_codec_dev, silence, sizeof(silence));
    }

    int bytes_in_buf = 0;
    uint8_t *read_ptr = read_buf;
    bool pa_enabled = false;

    while (!mp3_stop_requested) {
        // Pause: idle without touching the PA (toggling it pops). Decoder, file
        // position and buffers all stay put, so resume is gapless. Also hold here
        // while a sound effect (success/preview WAV) is writing to the codec —
        // resuming mid-effect interleaves two writers and garbles both.
        while ((mp3_pause_requested || fx_playing || is_playing) && !mp3_stop_requested) {
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        if (mp3_stop_requested) break;

        // Refill buffer from SD
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

        if (!pa_enabled) {
            pa_enabled = true;
            gpio_set_level(BSP_POWER_AMP_IO, 1); // PA on — file has leading silence, no transient
        }

        int samples = info.outputSamps; // total shorts (channels * frames)
        esp_codec_dev_write(spk_codec_dev, pcm_buf, samples * sizeof(short));
        taskYIELD(); // priority 5 — explicitly yields to LVGL at 6
    }

    // PA restore on natural EOF only — mp3_stop() already lowered it for stop/skip.
    if (!mp3_stop_requested) {
        gpio_set_level(BSP_POWER_AMP_IO, 1);
    }

    free(read_buf);
    free(pcm_buf);
    MP3FreeDecoder(decoder);
    fclose(f);
    mp3_is_playing = false;
    mp3_task_handle = NULL;
    vTaskDelete(NULL);
}

// ── Parse filename: get 2-digit track number and scroll title ────────────────
static void mp3_parse_filename(const char *path, char *tracknum, size_t tn_len,
                                char *title, size_t title_len)
{
    const char *base = strrchr(path, '/');
    base = base ? base + 1 : path;

    snprintf(tracknum, tn_len, "%c%c",
             (base[0] >= '0' && base[0] <= '9') ? base[0] : '-',
             (base[1] >= '0' && base[1] <= '9') ? base[1] : '-');

    snprintf(title, title_len, "%s", base);
    char *dot = strrchr(title, '.');
    if (dot) *dot = '\0';
}

// ── Build looping scroll string ───────────────────────────────────────────────
// Build scroll string with enough repeats for seamless looping.
// We animate x from 0 to -unit_w (one title+sep width), repeat infinite.
// Need at least 2 visible copies so there's no gap during the loop reset.
static char *mp3_build_scroll_text(const char *title)
{
    const char *sep = "     "; // 5 spaces
    size_t tlen = strlen(title);
    size_t slen = strlen(sep);
    // 4 repeats — first unit scrolls off, next is already on screen
    size_t unit = tlen + slen;
    size_t total = unit * 4 + 1;
    char *buf = malloc(total);
    if (!buf) return NULL;
    snprintf(buf, total, "%s%s%s%s%s%s%s%s",
             title, sep, title, sep, title, sep, title, sep);
    return buf;
}

// ── Animation callbacks ───────────────────────────────────────────────────────
static void musiclabel_anim_cb(void *obj, int32_t x)
{
    lv_obj_set_x((lv_obj_t *)obj, x);
}

static void anim_y_cb(void *obj, int32_t y)
{
    lv_obj_set_y((lv_obj_t *)obj, y);
}

static void anim_x_cb(void *obj, int32_t x)
{
    lv_obj_set_x((lv_obj_t *)obj, x);
}

// ── Open/close music player panel ────────────────────────────────────────────
// ── Music panel auto-close ───────────────────────────────────────────────────
// The power gate holds network off while the music panel is open, so the panel
// must not stay open forever. It self-closes 30 s after the last interaction
// (closing also stops playback), which bounds how long updates can be deferred.
#define MUSIC_AUTOCLOSE_MS  30000
static lv_timer_t *s_music_autoclose = NULL;
static void music_player_toggle(void);   // forward decl

static void music_autoclose_cb(lv_timer_t *t)
{
    (void)t;
    if (!music_player_open) {
        if (s_music_autoclose) lv_timer_pause(s_music_autoclose);
        return;
    }
    if (mp3_is_playing) {
        // A track is still playing — never cut it off. Restart the countdown
        // and re-check; the panel only closes once playback has stopped.
        lv_timer_reset(s_music_autoclose);
        return;
    }
    ESP_LOGI(TAG, "Music panel auto-closing (30s idle, nothing playing)");
    music_player_toggle();   // slides closed
    if (s_music_autoclose) lv_timer_pause(s_music_autoclose);
}

// Restart the 30 s countdown — called on open and on every panel interaction.
static void music_autoclose_kick(void)
{
    if (!s_music_autoclose) {
        s_music_autoclose = lv_timer_create(music_autoclose_cb, MUSIC_AUTOCLOSE_MS, NULL);
    }
    lv_timer_reset(s_music_autoclose);
    lv_timer_resume(s_music_autoclose);
}

static void music_autoclose_stop(void)
{
    if (s_music_autoclose) lv_timer_pause(s_music_autoclose);
}

static void music_player_toggle(void)
{
    music_player_open = !music_player_open;


    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, GUI_Container__home__music_player);
    lv_anim_set_exec_cb(&a, anim_y_cb);
    lv_anim_set_values(&a,
        music_player_open ? MUSIC_PLAYER_Y_HIDDEN : MUSIC_PLAYER_Y_REST,
        music_player_open ? MUSIC_PLAYER_Y_REST   : MUSIC_PLAYER_Y_HIDDEN);
    lv_anim_set_duration(&a, 300);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);

    lv_anim_t b;
    lv_anim_init(&b);
    lv_anim_set_var(&b, GUI_Container__home__musicbuttoncont);
    lv_anim_set_exec_cb(&b, anim_x_cb);
    lv_anim_set_values(&b,
        music_player_open ? MUSICBTN_X_REST : MUSICBTN_X_OPEN,
        music_player_open ? MUSICBTN_X_OPEN : MUSICBTN_X_REST);
    lv_anim_set_duration(&b, 300);
    lv_anim_set_path_cb(&b, lv_anim_path_ease_out);
    lv_anim_start(&b);

    if (music_player_open) {
        lv_image_set_src(GUI_Image__home__musicbtn,
                         &upload_music_btn_red_410df9ce0fe84d7ea36d6e6554682ce8_png);
    } else {
        lv_image_set_src(GUI_Image__home__musicbtn,
                         &upload_music_btn_ab57fd2bae02484db3c1484de8de889c_png);
        if (mp3_is_playing) {
            mp3_stop();
            lv_image_set_src(GUI_Image__home__image_35,
                             &upload_play_6c36b149bbde4d87af41769e62ca887f_png);
            music_scroll_stop();
        }
    }

    // Arm/disarm the 30 s self-close so the panel (and the network hold) is bounded.
    if (music_player_open) {
        music_autoclose_kick();
    } else {
        music_autoclose_stop();
    }
}

static void music_player_btn_cb(lv_event_t *e)
{
    (void)e;
    if (!g_system_ready) return;
    music_player_toggle();
}

// ── Scroll timer — rate controlled by SCROLL_TIMER_MS / SCROLL_STEP_PX ───────
static void scroll_tick_cb(lv_timer_t *t)
{
    (void)t;
    if (!music_scroll_active || scroll_unit_w == 0) return;
    scroll_x -= SCROLL_STEP_PX;
    if (scroll_x <= -scroll_unit_w) scroll_x += scroll_unit_w;
    lv_obj_set_x(GUI_Label__home__musicscrolllabel, scroll_x);
}

// ── Scroll title start/stop ───────────────────────────────────────────────────
static void music_scroll_start(const char *title)
{
    music_scroll_active = true;

    // Build looping text
    char *scroll_text = mp3_build_scroll_text(title);
    if (!scroll_text) return;
    lv_label_set_text(GUI_Label__home__musicscrolllabel, scroll_text);
    free(scroll_text);

    // Label must be LV_SIZE_CONTENT wide and CLIP long mode
    lv_label_set_long_mode(GUI_Label__home__musicscrolllabel, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(GUI_Label__home__musicscrolllabel, LV_SIZE_CONTENT);

    // Measure one unit width after layout
    lv_obj_update_layout(GUI_Label__home__musicscrolllabel);
    int32_t total_w = lv_obj_get_width(GUI_Label__home__musicscrolllabel);
    scroll_unit_w = total_w / 4; // 4 repeats in scroll text
    scroll_x = 0;

    // Start timer if not already running
    if (!scroll_timer) {
        scroll_timer = lv_timer_create(scroll_tick_cb, SCROLL_TIMER_MS, NULL);
    } else {
        lv_timer_resume(scroll_timer);
    }

    // Slide panel up into view
    lv_anim_t slide;
    lv_anim_init(&slide);
    lv_anim_set_var(&slide, GUI_Panel__home__musicscrollbg);
    lv_anim_set_exec_cb(&slide, anim_y_cb);
    lv_anim_set_values(&slide, 255, 224);
    lv_anim_set_duration(&slide, 300);
    lv_anim_set_path_cb(&slide, lv_anim_path_ease_out);
    lv_anim_start(&slide);
}

static void music_scroll_stop(void)
{
    if (!music_scroll_active) return;
    music_scroll_active = false;

    if (scroll_timer) lv_timer_pause(scroll_timer);
    scroll_x = 0;
    lv_obj_set_x(GUI_Label__home__musicscrolllabel, 0);

    // Slide panel back down off screen
    lv_anim_t slide;
    lv_anim_init(&slide);
    lv_anim_set_var(&slide, GUI_Panel__home__musicscrollbg);
    lv_anim_set_exec_cb(&slide, anim_y_cb);
    lv_anim_set_values(&slide, 224, 255);
    lv_anim_set_duration(&slide, 250);
    lv_anim_set_path_cb(&slide, lv_anim_path_ease_in);
    lv_anim_start(&slide);
}

// ── Update textarea_4 (track number) ─────────────────────────────────────────
static void music_update_display(void)
{
    if (mp3_track_count == 0) {
        lv_textarea_set_text(GUI_Textarea__home__textarea_4, "--");
        return;
    }
    char tracknum[4], title[128];
    mp3_parse_filename(mp3_playlist[mp3_current_track], tracknum, sizeof(tracknum),
                       title, sizeof(title));
    lv_textarea_set_text(GUI_Textarea__home__textarea_4, tracknum);
}

// ── Stop any playing MP3 ──────────────────────────────────────────────────────
static void mp3_stop(void)
{
    if (mp3_is_playing) {
        // Kill PA first so there's no pop when the task drains and exits.
        // The next mp3_task will bring it back up after its silence flush.
        gpio_set_level(BSP_POWER_AMP_IO, 0);
        mp3_stop_requested = true;
        for (int i = 0; i < 50 && mp3_is_playing; i++) {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        mp3_stop_requested = false;
    }
}

// ── Video player MP3 bridge ───────────────────────────────────────────────────
void video_mp3_play(const char *path)
{
    mp3_stop();
    if (spk_codec_dev == NULL) return;
    mp3_stop_requested = false;
    mp3_is_playing = true;
    mp3_play_args_t *args = malloc(sizeof(mp3_play_args_t));
    if (!args) { mp3_is_playing = false; return; }
    strncpy(args->path, path, sizeof(args->path) - 1);
    args->path[sizeof(args->path) - 1] = '\0';
    xTaskCreate(mp3_task, "vid_mp3", 8192, args, MP3_TASK_PRIORITY, &mp3_task_handle);
}

void video_mp3_stop(void)
{
    mp3_stop();
}

// ── Start playing current track ───────────────────────────────────────────────
static void mp3_play_current(void)
{
    if (mp3_track_count == 0 || spk_codec_dev == NULL) return;
    mp3_stop_requested = false;
    mp3_is_playing = true;
    mp3_play_args_t *args = malloc(sizeof(mp3_play_args_t));
    if (!args) { mp3_is_playing = false; return; }
    strncpy(args->path, mp3_playlist[mp3_current_track], sizeof(args->path) - 1);
    args->path[sizeof(args->path) - 1] = '\0';
    xTaskCreate(mp3_task, "mp3_play", 8192, args, MP3_TASK_PRIORITY, &mp3_task_handle);
    ESP_LOGI(TAG, "Playing track %d: %s", mp3_current_track, args->path);
}

// ── Music player button callbacks ─────────────────────────────────────────────
static void music_buttonplay_cb(lv_event_t *e)
{
    (void)e;
    if (!g_system_ready || mp3_track_count == 0) return;
    music_autoclose_kick();   // interaction — restart the 30 s self-close
    if (mp3_is_playing) {
        mp3_stop();
        lv_image_set_src(GUI_Image__home__image_35, &upload_play_6c36b149bbde4d87af41769e62ca887f_png);
        music_scroll_stop();
    } else {
        if (power_gate_net_busy()) {
            // Network update in flight — warn and don't start on top of it.
            g_net_wait_warning = true;
            ESP_LOGW(TAG, "MP3 held off — network update in progress");
            return;
        }
        char tracknum[4], title[128];
        mp3_parse_filename(mp3_playlist[mp3_current_track], tracknum, sizeof(tracknum),
                           title, sizeof(title));
        mp3_play_current();
        lv_image_set_src(GUI_Image__home__image_35, &upload_stopbutton_a37c756148194bc181010db779e72ea4_png);
        music_scroll_start(title);
    }
}

static void music_buttonup_cb(lv_event_t *e)
{
    (void)e;
    if (!g_system_ready || mp3_track_count == 0) return;
    music_autoclose_kick();   // interaction — restart the 30 s self-close
    bool was_playing = mp3_is_playing;
    mp3_stop();
    mp3_current_track = (mp3_current_track - 1 + mp3_track_count) % mp3_track_count;
    music_update_display();
    if (was_playing) {
        char tracknum[4], title[128];
        mp3_parse_filename(mp3_playlist[mp3_current_track], tracknum, sizeof(tracknum),
                           title, sizeof(title));
        mp3_play_current();
        music_scroll_stop();
        music_scroll_start(title);
    }
}

static void music_buttondown_cb(lv_event_t *e)
{
    (void)e;
    if (!g_system_ready || mp3_track_count == 0) return;
    music_autoclose_kick();   // interaction — restart the 30 s self-close
    bool was_playing = mp3_is_playing;
    mp3_stop();
    mp3_current_track = (mp3_current_track + 1) % mp3_track_count;
    music_update_display();
    if (was_playing) {
        char tracknum[4], title[128];
        mp3_parse_filename(mp3_playlist[mp3_current_track], tracknum, sizeof(tracknum),
                           title, sizeof(title));
        mp3_play_current();
        music_scroll_stop();
        music_scroll_start(title);
    }
}

void audio_play_mp3(const char *path)
{
    if (spk_codec_dev == NULL) return;
    mp3_play_args_t *args = malloc(sizeof(mp3_play_args_t));
    if (!args) return;
    strncpy(args->path, path, sizeof(args->path) - 1);
    args->path[sizeof(args->path)-1] = '\0';
    xTaskCreate(mp3_task, "mp3_play", 8192, args, MP3_TASK_PRIORITY, NULL);
}

// ── Periodic timer: detect when MP3 finishes naturally ───────────────────────
static bool mp3_was_playing = false;

static void music_watchdog_cb(lv_timer_t *t)
{
    (void)t;
    if (lv_scr_act() != GUI_Screen__home) return;
    if (mp3_was_playing && !mp3_is_playing) {
        lv_image_set_src(GUI_Image__home__image_35, &upload_play_6c36b149bbde4d87af41769e62ca887f_png);
        if (music_scroll_active) {
            music_scroll_stop();
        }
        // Playback just ended — start the 30 s close countdown from here so the
        // panel only closes after a track finishes, never mid-song.
        if (music_player_open) music_autoclose_kick();
    }
    mp3_was_playing = mp3_is_playing;
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
    ESP_LOGI(TAG, "Initializing SquareLine UI");
    GUI_init();

    lv_obj_set_style_bg_image_src(GUI_Screen__home, &upload_hclbg1_52bba57ce173452fadd7595a14167a99_png, 0);
    ui_set_default_weather();
    settings_ui_init();

    lv_obj_add_event_cb(GUI_Button__home__buttonplay,      music_buttonplay_cb,  LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(GUI_Button__home__buttonup,        music_buttonup_cb,    LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(GUI_Button__home__buttondown,      music_buttondown_cb,  LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(GUI_Button__home__musicbuttongrey, music_player_btn_cb,  LV_EVENT_CLICKED, NULL);

    // ── Button press visual feedback — red overlay on LV_STATE_PRESSED ────────
    // LVGL applies this automatically on finger-down, removes on finger-up.
    // Replaces the audio tick which can't play during MP3 playback.
    lv_obj_t *music_btns[] = {
        GUI_Button__home__buttonplay,
        GUI_Button__home__buttonup,
        GUI_Button__home__buttondown,
        GUI_Button__home__musicbuttongrey,
    };
    for (int i = 0; i < 4; i++) {
        lv_obj_set_style_bg_color(music_btns[i], lv_color_hex(0xFF2200), LV_STATE_PRESSED | LV_PART_MAIN);
        lv_obj_set_style_bg_opa(music_btns[i],   LV_OPA_40,              LV_STATE_PRESSED | LV_PART_MAIN);
        lv_obj_set_style_radius(music_btns[i],   8,                      LV_STATE_PRESSED | LV_PART_MAIN);
    }

    lv_obj_set_y(GUI_Container__home__music_player, MUSIC_PLAYER_Y_HIDDEN);
    lv_obj_set_x(GUI_Container__home__musicbuttoncont, MUSICBTN_X_REST);
    lv_obj_set_y(GUI_Panel__home__musicscrollbg, 255); // parked off bottom

    lv_obj_set_style_bg_color(GUI_Panel__home__musicscrollbg, lv_color_hex(0x222222), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(GUI_Panel__home__musicscrollbg, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_text_color(GUI_Label__home__musicscrolllabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_label_set_long_mode(GUI_Label__home__musicscrolllabel, LV_LABEL_LONG_CLIP);
    // Panel: 2px taller, original width (424px)
    lv_obj_set_size(GUI_Panel__home__musicscrollbg, 424, 26);
    // Label: 4px left inset, 1px down
    lv_obj_set_pos(GUI_Label__home__musicscrolllabel, 4, 1);
    lv_obj_set_width(GUI_Label__home__musicscrolllabel, 416);

    // Rename "Precipitation" label to "Chance Rain"
    lv_label_set_text(GUI_Label__home__Precipitation, "Chance Rain");

    lv_obj_set_size(GUI_Panel__home__panel_3, 60, 220);

    // Memory label — hidden for now, re-enable by removing LV_OBJ_FLAG_HIDDEN
    mem_label = lv_label_create(GUI_Screen__home);
    lv_obj_set_style_text_font(mem_label, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(mem_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mem_label, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_align(mem_label, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_pos(mem_label, 2, -18);
    lv_label_set_text(mem_label, "INT:--- PSRAM:---");
    lv_obj_add_flag(mem_label, LV_OBJ_FLAG_HIDDEN); // hidden — remove to show
    lv_timer_create(mem_label_cb, 1000, NULL);

    lv_timer_create(music_watchdog_cb, 500, NULL);
    lv_timer_create(clock_timer_cb, 1000, NULL);
    music_update_display();

    // ── Skull logo touch target (transparent, covers logo area) ──────────────
    lv_obj_t *skull_touch = lv_obj_create(GUI_Screen__home);
    lv_obj_set_pos(skull_touch, 310, 40);
    lv_obj_set_size(skull_touch, 340, 310);
    lv_obj_set_style_bg_opa(skull_touch, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(skull_touch, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(skull_touch, 0, LV_PART_MAIN);
    lv_obj_clear_flag(skull_touch, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(skull_touch, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_move_foreground(skull_touch);  // ensure nothing is on top of it
    lv_obj_add_event_cb(skull_touch, skull_touch_cb, LV_EVENT_CLICKED, NULL);

    // ── Weather status overlay — slim bar, top of screen right of weather panel
    s_weather_overlay = lv_obj_create(GUI_Screen__home);
    lv_obj_set_size(s_weather_overlay, 430, 40);
    lv_obj_set_align(s_weather_overlay, LV_ALIGN_CENTER);
    lv_obj_set_pos(s_weather_overlay, 81, -266); // parked above top edge
    lv_obj_set_style_bg_color(s_weather_overlay, lv_color_hex(0x222222), 0);
    lv_obj_set_style_bg_opa(s_weather_overlay, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(s_weather_overlay, 5, 0);
    lv_obj_set_style_border_width(s_weather_overlay, 0, 0);
    lv_obj_set_style_pad_all(s_weather_overlay, 0, 0);
    lv_obj_clear_flag(s_weather_overlay, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(s_weather_overlay, LV_OBJ_FLAG_HIDDEN); // starts hidden

    s_weather_overlay_lbl = lv_label_create(s_weather_overlay);
    lv_label_set_text(s_weather_overlay_lbl, "");
    lv_obj_set_style_text_color(s_weather_overlay_lbl, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(s_weather_overlay_lbl, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_align(s_weather_overlay_lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(s_weather_overlay_lbl, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(s_weather_overlay_lbl, 430);
    lv_obj_align(s_weather_overlay_lbl, LV_ALIGN_CENTER, 0, 0);

    lv_timer_create(weather_overlay_cb, 500, NULL);

    // ── Video fade-in overlay — full screen black, hidden normally ────────────
    // After video ends, shown at full opacity then faded out to reveal home screen
    g_video_fade_overlay = lv_obj_create(GUI_Screen__home);
    lv_obj_set_size(g_video_fade_overlay, LV_HOR_RES + 80, LV_VER_RES + 80);
    lv_obj_set_align(g_video_fade_overlay, LV_ALIGN_CENTER);
    lv_obj_set_pos(g_video_fade_overlay, 0, 0);
    lv_obj_set_style_bg_color(g_video_fade_overlay, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(g_video_fade_overlay, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_video_fade_overlay, 0, 0);
    lv_obj_set_style_radius(g_video_fade_overlay, 0, 0);
    lv_obj_set_style_pad_all(g_video_fade_overlay, 0, 0);
    lv_obj_clear_flag(g_video_fade_overlay, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_move_foreground(g_video_fade_overlay);
    lv_obj_add_flag(g_video_fade_overlay, LV_OBJ_FLAG_HIDDEN); // hidden until video ends
}

// ── OTA rollback test hook ────────────────────────────────────────────────────
// Set to 1 ONLY for a deliberate poison build: the app aborts instantly on
// boot, before the wedge detector can mark the image valid. The bootloader
// must then revert to the previous OTA slot on the following boot.
// NEVER commit/release with this set to 1 except for a planned rollback test.
#define OTA_ROLLBACK_TEST_CRASH 0

void app_main(void)
{
#if OTA_ROLLBACK_TEST_CRASH
    ESP_LOGE(TAG, "OTA ROLLBACK TEST BUILD — crashing on purpose NOW");
    abort();
#endif
    // Hardware-reset the panel on BOTH candidate reset lines. BSP_LCD_RST (27) is
    // what the BSP toggles, but the vendor code marks BSP_LCD_TOUCH_RST (23) as
    // "shared with LCD reset" — if 23 is the real panel reset, toggling only 27
    // leaves the panel un-reset after a soft reboot (blank screen until power cycle).
    gpio_set_direction(BSP_LCD_RST, GPIO_MODE_OUTPUT);
    gpio_set_direction(BSP_LCD_TOUCH_RST, GPIO_MODE_OUTPUT);
    gpio_set_level(BSP_LCD_RST, 0);
    gpio_set_level(BSP_LCD_TOUCH_RST, 0);
    // 20ms proved marginal: boots alternated between clean module reset (GT911 at
    // 0x5d, display OK) and dirty (GT911 at 0x14, display black). Hold reset long
    // enough for the module's reset RC to fully settle regardless of prior state.
    vTaskDelay(pdMS_TO_TICKS(250));
    gpio_set_level(BSP_LCD_RST, 1);
    gpio_set_level(BSP_LCD_TOUCH_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(120));

    // Wedge detector + self-heal. After a soft reset that interrupted active DSI
    // video, the display module comes up wedged (panel stays black no matter what;
    // GPIO reset, DSI SWRESET and PHY LDO power-cycle all proven insufficient).
    // Empirically 7/7 boots: module wedged <=> GT911 latches backup address 0x14
    // instead of 0x5d, and a wedged boot ALWAYS clears the wedge for the next boot.
    // So: probe the GT911; if it answers at 0x14, restart once to land on the
    // clean-boot side of the cycle. An NVS flag guards against restart loops.
    // NVS must come up before the wedge detector — its restart-loop guard lives
    // in NVS (the only storage that survives every reset type on this board:
    // RTC_NOINIT is NOT retained across esptool watchdog resets, and
    // esp_reset_reason() can't distinguish our restart from any other
    // esp_restart(), e.g. the post-OTA reboot — that ambiguity caused a blank
    // screen after OTA installs).
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(nvs_storage_init());

    if (bsp_i2c_init() == ESP_OK) {
        int wedge_restarts = nvs_load_wedge_restart();
        if (i2c_master_probe(bsp_i2c_get_handle(), 0x14, 100) == ESP_OK) {
            if (wedge_restarts < 3) {
                ESP_LOGW(TAG, "Display module wedged (GT911 at 0x14) — restart %d/3 to clear",
                         wedge_restarts + 1);
                // A freshly OTA'd image is still PENDING_VERIFY; our restart would
                // look like a boot failure and trigger rollback. The app clearly
                // runs (we're executing), so validate it before restarting.
                ota_update_mark_boot_valid();
                nvs_store_wedge_restart(wedge_restarts + 1);
                vTaskDelay(pdMS_TO_TICKS(100));
                esp_restart();
            }
            ESP_LOGE(TAG, "Display wedge persists after %d restarts — continuing anyway",
                     wedge_restarts);
            nvs_store_wedge_restart(0);
        } else if (wedge_restarts != 0) {
            nvs_store_wedge_restart(0); // clean boot — reset the counter
        }
    }

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    // Set the network hostname (shown on the router/DHCP, default is "espressif")
    if (sta_netif) {
        esp_err_t hn = esp_netif_set_hostname(sta_netif, "HVMediaDevice");
        if (hn != ESP_OK) {
            ESP_LOGW(TAG, "Failed to set hostname: %s", esp_err_to_name(hn));
        }
    }
    wifi_init_config_t wifi_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "WiFi initialized in STA mode");

    wifi_manager_init();
    wifi_manager_connect_saved();

    ESP_LOGI(TAG, "Starting Desk Media Device");
    scan_sd_card();

    bsp_display_cfg_t cfg = {
        .lv_adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG(),
        .rotation = ESP_LV_ADAPTER_ROTATE_90,
        // ROTATE_90 requires a tear-avoid mode (NONE rejects rotation and hangs
        // display start). DOUBLE_FULL works on power-on boot; see reset_fix_changes.md
        // for the soft-reset blank-screen investigation.
        .tear_avoid_mode = ESP_LV_ADAPTER_TEAR_AVOID_MODE_DOUBLE_FULL,
        .touch_flags = { .swap_xy = 1, .mirror_x = 1, .mirror_y = 0 }
    };

    g_lv_disp = bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();
    int saved_brightness = nvs_load_brightness(80);
    if (saved_brightness < 10) saved_brightness = 10;
    bsp_display_brightness_set(saved_brightness);
    ESP_LOGI(TAG, "Brightness restored to %d%%", saved_brightness);

    bsp_display_lock(-1);
    create_ui();
    lv_obj_invalidate(lv_scr_act());
    bsp_display_unlock();

    vTaskDelay(pdMS_TO_TICKS(500));

    ret = i2s_init();
    if (ret != ESP_OK) { ESP_LOGE(TAG, "I2S init failed"); }

    ret = codec_init();
    if (ret != ESP_OK) { ESP_LOGE(TAG, "Codec init failed"); }

    ESP_LOGI(TAG, "Initializing weather system");
    ret = weather_task_start();
    if (ret != ESP_OK) { ESP_LOGW(TAG, "Weather task start failed: %s", esp_err_to_name(ret)); }

    char saved_zip[16] = "";
    if (nvs_load_zipcode(saved_zip, sizeof(saved_zip)) != ESP_OK || strlen(saved_zip) == 0) {
        strncpy(saved_zip, "88002", sizeof(saved_zip) - 1);
        nvs_store_zipcode(saved_zip);
        ESP_LOGI(TAG, "No zip code saved — using default: %s", saved_zip);
    } else {
        ESP_LOGI(TAG, "Loaded zip code from NVS: %s", saved_zip);
    }
    // NOTE: weather_set_location() is intentionally NOT called here. The WiFi
    // got-IP handler (wifi_manager.c) fires it once connectivity exists; calling
    // it here too caused a full duplicate geocode + NWS grid + fetch cycle on
    // every boot where WiFi connected quickly. Without network the call could
    // never succeed anyway (weather task loads cached location/grid from NVS).

    ESP_LOGI(TAG, "Initialization complete");
    video_player_init();

    // Wait for first weather fetch OR timeout before enabling touch.
    // Prevents crashes from touching music/video before codec/wifi/weather are stable.
    ESP_LOGI(TAG, "Waiting for system ready...");
    const int READY_TIMEOUT_MS = 30000;
    const int READY_POLL_MS    = 500;
    for (int elapsed = 0; elapsed < READY_TIMEOUT_MS; elapsed += READY_POLL_MS) {
        // Wait for first weather fetch AND for it to fully complete (not still fetching)
        if (weather_get_last_update() > 0 && !weather_task_is_fetching()) {
            // Extra settle time — second geocode+fetch fires immediately after first
            // Give it a moment to start so we don't race with it
            vTaskDelay(pdMS_TO_TICKS(2000));
            if (!weather_task_is_fetching()) {
                ESP_LOGI(TAG, "Weather ready — enabling touch");
                break;
            }
            // Still fetching after settle — keep waiting
        }
        // Allow through if WiFi failed with an error (no point waiting for weather)
        uint8_t reason = wifi_manager_get_last_disconnect_reason();
        if (!wifi_manager_is_connecting() && !wifi_manager_is_connected() && reason != 0) {
            ESP_LOGW(TAG, "WiFi unavailable — enabling touch anyway");
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(READY_POLL_MS));
    }
    g_system_ready = true;
    ESP_LOGI(TAG, "System ready — touch enabled");

    // System proven healthy: cancel any pending OTA rollback, start update checks
    ota_update_mark_boot_valid();
    ota_update_start();
    content_sync_start();   // background: pull new media from the "content" release onto SD

    weather_data_t api_data = {0};
    uint32_t last_displayed_update = 0;

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));

        if (weather_get_data(&api_data) != ESP_OK) continue;
        if (api_data.last_update == last_displayed_update) continue;
        last_displayed_update = api_data.last_update;

        weather_display_t disp = {0};
        disp.current_temp_f    = api_data.current_temp;
        disp.feels_like_f      = api_data.current_apparent_temp;
        disp.current_humidity  = (int)api_data.current_humidity;
        disp.current_precip_prob = api_data.current_precip_prob;
        disp.current_wmo       = api_data.current_weather_code;
        disp.is_night          = (api_data.current_is_day == 0);
        disp.wifi_connected    = wifi_manager_is_connected();

        // Wind string e.g. "9SE"
        const char *compass[] = {"N","NE","E","SE","S","SW","W","NW"};
        const char *dir = compass[((api_data.current_wind_direction + 22) % 360) / 45];
        snprintf(disp.current_wind_str, sizeof(disp.current_wind_str),
                 "%.0f%s", api_data.current_wind_speed, dir);

        strncpy(disp.city,  api_data.city,  sizeof(disp.city)  - 1);
        strncpy(disp.state, api_data.state, sizeof(disp.state) - 1);

        // Store UTC offset for the 1-second clock timer
        g_utc_offset_seconds = api_data.utc_offset_seconds;

        // Hourly
        for (int i = 0; i < 4; i++) {
            disp.hourly_temp_f[i] = api_data.hourly.hours[i].temperature;
            disp.hourly_wmo[i]    = api_data.hourly.hours[i].weather_code;
            strncpy(disp.hourly_time[i], api_data.hourly.times_12h[i],
                    sizeof(disp.hourly_time[i]) - 1);
        }

        // Daily
        for (int i = 0; i < 3; i++) {
            disp.daily_high_f[i] = api_data.daily[i].temp_high;
            disp.daily_low_f[i]  = api_data.daily[i].temp_low;
            disp.daily_wmo[i]    = api_data.daily[i].weather_code;
            strncpy(disp.daily_day[i], api_data.daily[i].day_name,
                    sizeof(disp.daily_day[i]) - 1);
        }

        bsp_display_lock(-1);
        weather_display_update(&disp);
        bsp_display_unlock();
    }
}
