#include "settings.h"
#include "network_selector.h"
#include "wifi_manager.h"
#include "weather/weather_task.h"
#include "GUI.h"
#include "bsp/display.h"
#include "audio.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs_storage.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include <string.h>

static const char *TAG = "Settings";

// Stored zip code — persists between settings visits, default on first boot
static char s_zipcode[16] = "88002";
static char s_password[65] = "";

// ── Event callbacks ──────────────────────────────────────────────────────────

static void settings_btn_cb(lv_event_t *e)
{
    (void)e;
    ESP_LOGI(TAG, "Opening settings screen");
    audio_music_set_paused(true);  // pause MP3 (not mute) while in settings
    lv_screen_load(GUI_Screen__settingswindow);
}

static void save_btn_cb(lv_event_t *e)
{
    (void)e;
    ESP_LOGI(TAG, "Settings saved");

    // Load current saved values to compare against
    char saved_ssid[33] = "";
    char saved_pass[65] = "";
    char saved_zip[16]  = "";
    nvs_load_wifi(saved_ssid, sizeof(saved_ssid), saved_pass, sizeof(saved_pass));
    nvs_load_zipcode(saved_zip, sizeof(saved_zip));

    const char *new_ssid = lv_textarea_get_text(GUI_Textarea__settingswindow__textarea_2);
    const char *new_pass = lv_textarea_get_text(GUI_Textarea__settingswindow__textarea_3);
    const char *new_zip  = lv_textarea_get_text(GUI_Textarea__settingswindow__textarea_1);
    if (!new_ssid) new_ssid = "";
    if (!new_pass) new_pass = "";
    if (!new_zip)  new_zip  = "";

    bool wifi_changed = (strcmp(new_ssid, saved_ssid) != 0) ||
                        (strcmp(new_pass, saved_pass) != 0);
    bool zip_changed  = (strcmp(new_zip, saved_zip) != 0);

    // Save WiFi credentials only if changed
    if (wifi_changed && strlen(new_ssid) > 0) {
        ESP_LOGI(TAG, "WiFi credentials changed — saving and reconnecting");
        nvs_store_wifi(new_ssid, new_pass);
        wifi_manager_connect_saved();
    } else {
        ESP_LOGI(TAG, "WiFi unchanged — skipping reconnect");
    }

    // Save zip and trigger weather update only if changed
    if (zip_changed && strlen(new_zip) > 0) {
        ESP_LOGI(TAG, "Zip changed to %s — triggering weather update", new_zip);
        nvs_store_zipcode(new_zip);
        strncpy(s_zipcode, new_zip, sizeof(s_zipcode) - 1);
        s_zipcode[sizeof(s_zipcode) - 1] = '\0';
        weather_set_location(new_zip);
    } else {
        ESP_LOGI(TAG, "Zip unchanged — skipping weather update");
    }

    audio_play_success();
    audio_music_set_paused(false); // resume MP3 when leaving settings
    lv_screen_load(GUI_Screen__home);
}

static void exit_btn_cb(lv_event_t *e)
{
    (void)e;
    ESP_LOGI(TAG, "Settings exit");
    audio_music_set_paused(false); // resume MP3 when leaving settings
    lv_screen_load(GUI_Screen__home);
}

// ── Zip code keyboard screen ──────────────────────────────────────────────────

static void zipcode_field_cb(lv_event_t *e)
{
    (void)e;
    // Clear textarea and navigate to zip code keyboard
    lv_textarea_set_text(GUI_Textarea__keyboardzipcode__textarea_zipcode, "");
    lv_screen_load(GUI_Screen__keyboardzipcode);
}

static void zipcode_save_cb(lv_event_t *e)
{
    (void)e;
    // Get entered text and save it
    const char *zip = lv_textarea_get_text(GUI_Textarea__keyboardzipcode__textarea_zipcode);
    if (zip && strlen(zip) > 0) {
        strncpy(s_zipcode, zip, sizeof(s_zipcode) - 1);
        s_zipcode[sizeof(s_zipcode) - 1] = '\0';
        // Update the zip code field on settings screen
        lv_textarea_set_text(GUI_Textarea__settingswindow__textarea_1, s_zipcode);
        ESP_LOGI(TAG, "Zip code saved: %s", s_zipcode);
    }
    audio_play_success();
    lv_screen_load(GUI_Screen__settingswindow);
}

static void zipcode_exit_cb(lv_event_t *e)
{
    (void)e;
    ESP_LOGI(TAG, "Zip code entry cancelled");
    lv_screen_load(GUI_Screen__settingswindow);
}

static void brightness_slider_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int val = lv_slider_get_value(slider);
    // Clamp minimum to 10% so screen never goes fully black
    if (val < 10) {
        val = 10;
        lv_slider_set_value(slider, 10, LV_ANIM_OFF);
    }
    bsp_display_brightness_set(val);
    audio_play_tick();
}

static void brightness_slider_save_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int val = lv_slider_get_value(slider);
    if (val < 10) val = 10;
    nvs_store_brightness(val);
    ESP_LOGI(TAG, "Brightness saved to NVS: %d%%", val);
}

static void volume_slider_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int val = lv_slider_get_value(slider);
    audio_set_volume(val);
    audio_play_tick();
}

static void volume_slider_save_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int val = lv_slider_get_value(slider);
    nvs_store_volume(val);
    ESP_LOGI(TAG, "Volume saved to NVS: %d%%", val);
}

static void touch_feedback_cb(lv_event_t *e)
{
    (void)e;
    audio_play_tick();
}

static void mute_apply_indicator(lv_obj_t *cb, bool muted)
{
    // Red indicator when muted, default white when unmuted
    lv_color_t c = muted ? lv_color_hex(0xFF0000) : lv_color_hex(0xFFFFFF);
    lv_obj_set_style_bg_color(cb, c, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(cb, c, LV_PART_INDICATOR | LV_STATE_CHECKED);
}

static void mute_checkbox_cb(lv_event_t *e)
{
    lv_obj_t *cb = lv_event_get_target(e);
    bool checked = lv_obj_has_state(cb, LV_STATE_CHECKED);
    mute_apply_indicator(cb, checked);
    audio_set_mute(checked);
    nvs_store_mute(checked);
    ESP_LOGI(TAG, "Mute: %s", checked ? "ON" : "OFF");
}

// ── Factory reset ─────────────────────────────────────────────────────────────

#define FACTORY_RESET_HOLD_MS   10000

static TimerHandle_t s_reset_timer = NULL;

static void factory_reset_task(void *arg)
{
    (void)arg;
    ESP_LOGW(TAG, "Factory reset triggered!");
    audio_play_success();
    vTaskDelay(pdMS_TO_TICKS(2500)); // let sound finish
    // Erase NVS partition by name then restart
    nvs_flash_erase_partition("nvs");
    esp_restart();
}

static void factory_reset_timer_cb(TimerHandle_t xTimer)
{
    (void)xTimer;
    xTaskCreate(factory_reset_task, "factory_reset", 4096, NULL, 5, NULL);
}

static void mute_long_press_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_PRESSED) {
        if (!s_reset_timer) {
            s_reset_timer = xTimerCreate("factory_reset", pdMS_TO_TICKS(FACTORY_RESET_HOLD_MS),
                                         pdFALSE, NULL, factory_reset_timer_cb);
        }
        if (s_reset_timer) xTimerStart(s_reset_timer, 0);
    } else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST) {
        if (s_reset_timer) xTimerStop(s_reset_timer, 0);
    }
}

// ── Password keyboard ─────────────────────────────────────────────────────────

static void password_field_cb(lv_event_t *e)
{
    (void)e;
    lv_textarea_set_text(GUI_Textarea__keyboardpassword__textareapassword, "");
    lv_screen_load(GUI_Screen__keyboardpassword);
}

static void password_save_cb(lv_event_t *e)
{
    (void)e;
    const char *pwd = lv_textarea_get_text(GUI_Textarea__keyboardpassword__textareapassword);
    if (pwd) {
        strncpy(s_password, pwd, sizeof(s_password) - 1);
        s_password[sizeof(s_password) - 1] = '\0';
        lv_textarea_set_text(GUI_Textarea__settingswindow__textarea_3, s_password);
        ESP_LOGI(TAG, "Password saved");
    }
    audio_play_success();
    lv_screen_load(GUI_Screen__settingswindow);
}

static void password_exit_cb(lv_event_t *e)
{
    (void)e;
    lv_screen_load(GUI_Screen__settingswindow);
}

// ── Network selector ─────────────────────────────────────────────────────────

static void wifi_field_cb(lv_event_t *e)
{
    (void)e;
    lv_screen_load(GUI_Screen__networkselector);
}

static void on_network_selected(const char *ssid)
{
    // Write chosen SSID into the WiFi name field in settings
    lv_textarea_set_text(GUI_Textarea__settingswindow__textarea_2, ssid);
    ESP_LOGI(TAG, "WiFi network set to: %s", ssid);
}

// ── Public init ──────────────────────────────────────────────────────────────

void settings_ui_init(void)
{
    // Fix touch on settings button — SquareLine sets ADV_HITTEST and removes
    // CLICKABLE from the parent container, which blocks touch events
    lv_obj_add_flag(GUI_Container__home__NETWORKSTAUSANDMENUCONT, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(GUI_Image__home__SETTINGSBUTTON, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_add_flag(GUI_Image__home__SETTINGSBUTTON, LV_OBJ_FLAG_CLICKABLE);
    // Expand touch area without changing visual size
    lv_obj_set_ext_click_area(GUI_Image__home__SETTINGSBUTTON, 20);

    // Settings button on home screen → open settings
    lv_obj_add_event_cb(GUI_Image__home__SETTINGSBUTTON,
                        settings_btn_cb,
                        LV_EVENT_CLICKED, NULL);

    // Save button → save & return home
    lv_obj_add_event_cb(GUI_Button__settingswindow__button_8,
                        save_btn_cb,
                        LV_EVENT_CLICKED, NULL);

    // Exit button → return home without saving
    lv_obj_add_event_cb(GUI_Button__settingswindow__button_1,
                        exit_btn_cb,
                        LV_EVENT_CLICKED, NULL);

    // Brightness slider (slider_1) → live update + save on release
    lv_obj_add_event_cb(GUI_Slider__settingswindow__slider_1,
                        brightness_slider_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(GUI_Slider__settingswindow__slider_1,
                        brightness_slider_save_cb,
                        LV_EVENT_RELEASED, NULL);

    // Mute checkbox → red indicator when checked, expanded touch area
    lv_obj_set_ext_click_area(GUI_Checkbox__settingswindow__checkbox, 20);
    // Restore persisted mute state (codec itself is muted in codec_init)
    {
        bool muted = nvs_load_mute(false);
        if (muted) {
            lv_obj_add_state(GUI_Checkbox__settingswindow__checkbox, LV_STATE_CHECKED);
        } else {
            lv_obj_remove_state(GUI_Checkbox__settingswindow__checkbox, LV_STATE_CHECKED);
        }
        mute_apply_indicator(GUI_Checkbox__settingswindow__checkbox, muted);
    }
    lv_obj_add_event_cb(GUI_Checkbox__settingswindow__checkbox,
                        mute_checkbox_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);
    // 10-second hold → factory reset
    lv_obj_add_event_cb(GUI_Checkbox__settingswindow__checkbox,
                        mute_long_press_cb,
                        LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(GUI_Checkbox__settingswindow__checkbox,
                        mute_long_press_cb,
                        LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(GUI_Checkbox__settingswindow__checkbox,
                        mute_long_press_cb,
                        LV_EVENT_PRESS_LOST, NULL);

    // Volume slider → set volume + play preview + save on release
    lv_obj_add_event_cb(GUI_Slider__settingswindow__slider,
                        volume_slider_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(GUI_Slider__settingswindow__slider,
                        volume_slider_save_cb,
                        LV_EVENT_RELEASED, NULL);

    // Touch feedback on settings widgets (not home screen background)
    // Note: save button (button_8) skipped here — it plays success sound instead
    lv_obj_add_event_cb(GUI_Button__settingswindow__button_1, touch_feedback_cb,
                        LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(GUI_Checkbox__settingswindow__checkbox, touch_feedback_cb,
                        LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(GUI_Textarea__settingswindow__textarea_1, touch_feedback_cb,
                        LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(GUI_Textarea__settingswindow__textarea_2, touch_feedback_cb,
                        LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(GUI_Textarea__settingswindow__textarea_3, touch_feedback_cb,
                        LV_EVENT_PRESSED, NULL);

    // Zip code field → navigate to keyboard screen
    lv_obj_add_event_cb(GUI_Textarea__settingswindow__textarea_1,
                        zipcode_field_cb, LV_EVENT_CLICKED, NULL);
    // Default zip code on first boot
    lv_textarea_set_placeholder_text(GUI_Textarea__settingswindow__textarea_1, "88002");
    // Widen field to fit up to 10 digits
    lv_obj_set_width(GUI_Textarea__settingswindow__textarea_1, 200);
    // Style: larger font, black text, centered vertically
    lv_obj_set_style_text_font(GUI_Textarea__settingswindow__textarea_1,
                               &fixed23, LV_PART_MAIN);
    lv_obj_set_style_text_color(GUI_Textarea__settingswindow__textarea_1,
                                lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_align(GUI_Textarea__settingswindow__textarea_1,
                                LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_pad_top(GUI_Textarea__settingswindow__textarea_1, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(GUI_Textarea__settingswindow__textarea_1, 0, LV_PART_MAIN);

    // WiFi name field styling — fixed23 matches zip code size but non-bold
    lv_obj_set_style_text_font(GUI_Textarea__settingswindow__textarea_2,
                               &fixed23, LV_PART_MAIN);
    lv_obj_set_style_text_color(GUI_Textarea__settingswindow__textarea_2,
                                lv_color_hex(0x111111), LV_PART_MAIN);
    lv_obj_set_style_text_align(GUI_Textarea__settingswindow__textarea_2,
                                LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_pad_top(GUI_Textarea__settingswindow__textarea_2, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(GUI_Textarea__settingswindow__textarea_2, 0, LV_PART_MAIN);

    // Password field styling
    lv_obj_set_style_text_font(GUI_Textarea__settingswindow__textarea_3,
                               &title_1, LV_PART_MAIN);
    lv_obj_set_style_text_color(GUI_Textarea__settingswindow__textarea_3,
                                lv_color_hex(0x111111), LV_PART_MAIN);
    lv_obj_set_style_text_align(GUI_Textarea__settingswindow__textarea_3,
                                LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_pad_top(GUI_Textarea__settingswindow__textarea_3, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(GUI_Textarea__settingswindow__textarea_3, 0, LV_PART_MAIN);

    // Fix SquareLine (20,20) offset on all exit buttons + expand touch area
    lv_obj_set_pos(GUI_Button__keyboardzipcode__button_4, 0, 0);
    lv_obj_set_ext_click_area(GUI_Button__keyboardzipcode__button_4, 10);
    lv_obj_set_pos(GUI_Button__keyboardpassword__button_2, 0, 0);
    lv_obj_set_ext_click_area(GUI_Button__keyboardpassword__button_2, 10);

    // Filter out +/- and . from the zip textarea — digits only
    lv_textarea_set_accepted_chars(GUI_Textarea__keyboardzipcode__textarea_zipcode,
                                   "0123456789");

    // Zip code keyboard save/exit
    lv_obj_add_event_cb(GUI_Button__keyboardzipcode__button_5,
                        zipcode_save_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(GUI_Button__keyboardzipcode__button_4,
                        zipcode_exit_cb, LV_EVENT_CLICKED, NULL);

    // Touch feedback on zip code keyboard buttons
    lv_obj_add_event_cb(GUI_Button__keyboardzipcode__button_5, touch_feedback_cb,
                        LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(GUI_Button__keyboardzipcode__button_4, touch_feedback_cb,
                        LV_EVENT_PRESSED, NULL);

    // Touch feedback on keyboard key presses
    lv_obj_add_event_cb(GUI_Keyboard__keyboardzipcode__keyboardzip,
                        touch_feedback_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(GUI_Keyboard__keyboardpassword__keyboardpass,
                        touch_feedback_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // Keyboard font — bigger and black for both keyboards
    // LV_PART_ITEMS targets the individual key buttons
    lv_obj_set_style_text_font(GUI_Keyboard__keyboardzipcode__keyboardzip,
                               &lv_font_montserrat_26, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(GUI_Keyboard__keyboardzipcode__keyboardzip,
                                lv_color_hex(0x000000), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(GUI_Keyboard__keyboardpassword__keyboardpass,
                               &lv_font_montserrat_26, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(GUI_Keyboard__keyboardpassword__keyboardpass,
                                lv_color_hex(0x000000), LV_PART_ITEMS | LV_STATE_DEFAULT);

    // Restore brightness from NVS and apply
    int saved_brightness = nvs_load_brightness(80);
    if (saved_brightness < 10) saved_brightness = 10;
    bsp_display_brightness_set(saved_brightness);
    lv_slider_set_value(GUI_Slider__settingswindow__slider_1, saved_brightness, LV_ANIM_OFF);

    // Restore volume from NVS and apply
    int saved_volume = nvs_load_volume(80);
    audio_set_volume(saved_volume);
    lv_slider_set_value(GUI_Slider__settingswindow__slider, saved_volume, LV_ANIM_OFF);

    // WiFi name field → open network selector screen
    lv_obj_add_event_cb(GUI_Textarea__settingswindow__textarea_2,
                        wifi_field_cb, LV_EVENT_CLICKED, NULL);

    // Password field → open password keyboard
    lv_obj_add_event_cb(GUI_Textarea__settingswindow__textarea_3,
                        password_field_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(GUI_Button__keyboardpassword__button_3,
                        password_save_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(GUI_Button__keyboardpassword__button_2,
                        password_exit_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(GUI_Button__keyboardpassword__button_3,
                        touch_feedback_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(GUI_Button__keyboardpassword__button_2,
                        touch_feedback_cb, LV_EVENT_PRESSED, NULL);

    // Network selector — init and register SSID-return callback
    network_selector_register_cb(on_network_selected);
    network_selector_ui_init();

    // Load saved settings from NVS and populate fields
    char saved_ssid[33] = "";
    char saved_pass[65] = "";
    if (nvs_load_wifi(saved_ssid, sizeof(saved_ssid), saved_pass, sizeof(saved_pass)) == ESP_OK) {
        if (strlen(saved_ssid) > 0)
            lv_textarea_set_text(GUI_Textarea__settingswindow__textarea_2, saved_ssid);
        if (strlen(saved_pass) > 0) {
            memcpy(s_password, saved_pass, sizeof(s_password) - 1);
            s_password[sizeof(s_password) - 1] = '\0';
            lv_textarea_set_text(GUI_Textarea__settingswindow__textarea_3, saved_pass);
        }
    }

    char saved_zip[16] = "";
    if (nvs_load_zipcode(saved_zip, sizeof(saved_zip)) == ESP_OK && strlen(saved_zip) > 0) {
        memcpy(s_zipcode, saved_zip, sizeof(s_zipcode) - 1);
        s_zipcode[sizeof(s_zipcode) - 1] = '\0';
    } else {
        // No zip in NVS — write the default
        nvs_store_zipcode(s_zipcode);
        memcpy(saved_zip, s_zipcode, sizeof(saved_zip));
        ESP_LOGI(TAG, "Writing default zip to NVS: %s", s_zipcode);
    }
    lv_textarea_set_text(GUI_Textarea__settingswindow__textarea_1, s_zipcode);

    ESP_LOGI(TAG, "Settings UI initialized");
}
