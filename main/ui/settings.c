#include "settings.h"
#include "GUI.h"
#include "bsp/display.h"
#include "audio.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "Settings";

// Stored zip code — persists between settings visits, default on first boot
static char s_zipcode[16] = "88002";

// ── Event callbacks ──────────────────────────────────────────────────────────

static void settings_btn_cb(lv_event_t *e)
{
    (void)e;
    ESP_LOGI(TAG, "Opening settings screen");
    // Set brightness slider to current value before showing
    int cur = bsp_display_brightness_get();
    if (cur < 0) cur = 80;
    lv_slider_set_value(GUI_Slider__settingswindow__slider_1, cur, LV_ANIM_OFF);
    lv_screen_load(GUI_Screen__settingswindow);
}

static void save_btn_cb(lv_event_t *e)
{
    (void)e;
    ESP_LOGI(TAG, "Settings saved");
    audio_play_success();  // plays SUCCESS.WAV
    lv_screen_load(GUI_Screen__home);
}

static void exit_btn_cb(lv_event_t *e)
{
    (void)e;
    ESP_LOGI(TAG, "Settings exit");
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
    ESP_LOGI(TAG, "Brightness set to %d%%", val);
}

static void volume_slider_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int val = lv_slider_get_value(slider);
    audio_set_volume(val);
    audio_play_tick();
    ESP_LOGI(TAG, "Volume set to %d%%", val);
}

static void touch_feedback_cb(lv_event_t *e)
{
    (void)e;
    audio_play_tick();
}

static void mute_checkbox_cb(lv_event_t *e)
{
    lv_obj_t *cb = lv_event_get_target(e);
    bool checked = lv_obj_has_state(cb, LV_STATE_CHECKED);
    // Change checkbox indicator color: red when muted, default when unmuted
    if (checked) {
        lv_obj_set_style_bg_color(cb, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_set_style_border_color(cb, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_CHECKED);
    } else {
        lv_obj_set_style_bg_color(cb, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_set_style_border_color(cb, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_CHECKED);
    }
    ESP_LOGI(TAG, "Mute: %s", checked ? "ON" : "OFF");
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

    // Brightness slider (slider_1) → live update
    lv_obj_add_event_cb(GUI_Slider__settingswindow__slider_1,
                        brightness_slider_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);

    // Mute checkbox → red indicator when checked, expanded touch area
    lv_obj_set_ext_click_area(GUI_Checkbox__settingswindow__checkbox, 20);
    lv_obj_add_event_cb(GUI_Checkbox__settingswindow__checkbox,
                        mute_checkbox_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);

    // Volume slider → set volume + play preview
    lv_obj_add_event_cb(GUI_Slider__settingswindow__slider,
                        volume_slider_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);

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

    // Set initial brightness slider position
    int cur = bsp_display_brightness_get();
    if (cur < 0) cur = 80;
    lv_slider_set_value(GUI_Slider__settingswindow__slider_1, cur, LV_ANIM_OFF);

    ESP_LOGI(TAG, "Settings UI initialized");
}
