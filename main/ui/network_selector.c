#include "network_selector.h"
#include "GUI.h"
#include "audio.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "bsp/esp32_p4_wifi6_touch_lcd_4_3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "NetworkSelector";

// ── State ────────────────────────────────────────────────────────────────────

#define MAX_NETWORKS    20
static char s_ssid_pool[MAX_NETWORKS][33];  // static pool — no malloc/leak
static int  s_ssid_pool_idx = 0;
#define ROW_PAD_X       10
#define ROW_PAD_Y       10
#define ROW_PAD_LEFT    5
#define ROW_PAD_RIGHT   10
#define BAR_W           10
#define BAR_GAP         3
#define BAR_MAX_H       24

static char s_selected_ssid[33] = "";
static lv_obj_t *s_selected_row = NULL;
static network_selected_cb_t s_on_selected = NULL;
static lv_obj_t *s_list_cont = NULL;
static lv_obj_t *s_searching_overlay = NULL;
static volatile bool s_scan_running = false;

// ── Searching overlay ─────────────────────────────────────────────────────────

static void show_searching_overlay(bool show)
{
    if (!s_searching_overlay) {
        lv_obj_t *screen = GUI_Screen__networkselector;
        s_searching_overlay = lv_obj_create(screen);
        lv_obj_set_size(s_searching_overlay, 504, 200);
        lv_obj_align(s_searching_overlay, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_bg_color(s_searching_overlay, lv_color_hex(0x222222), 0);
        lv_obj_set_style_bg_opa(s_searching_overlay, LV_OPA_COVER, 0);
        lv_obj_set_style_radius(s_searching_overlay, 5, 0);
        lv_obj_set_style_border_width(s_searching_overlay, 0, 0);
        lv_obj_clear_flag(s_searching_overlay, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_t *lbl = lv_label_create(s_searching_overlay);
        lv_label_set_text(lbl, "STARTING WIFI RECEIVER");
        lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, 0);
        lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);
    }
    if (show) {
        lv_obj_clear_flag(s_searching_overlay, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(s_searching_overlay);
    } else {
        lv_obj_add_flag(s_searching_overlay, LV_OBJ_FLAG_HIDDEN);
    }
}

// ── Signal bar widget ─────────────────────────────────────────────────────────

static lv_obj_t *create_signal_bars(lv_obj_t *parent, int bars_lit)
{
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, (BAR_W * 4) + (BAR_GAP * 3) + 2, BAR_MAX_H);
    lv_obj_set_layout(cont, LV_LAYOUT_NONE);

    lv_color_t color_on;
    if (bars_lit <= 1)      color_on = lv_color_hex(0xFF2020);
    else if (bars_lit <= 2) color_on = lv_color_hex(0xFFCC00);
    else                    color_on = lv_color_hex(0x22CC44);

    lv_color_t color_off = lv_color_hex(0xCCCCCC);

    for (int i = 0; i < 4; i++) {
        int bar_h = 6 + (i * 6);
        lv_obj_t *bar = lv_obj_create(cont);
        lv_obj_remove_style_all(bar);
        lv_obj_set_size(bar, BAR_W, bar_h);
        lv_obj_set_pos(bar, i * (BAR_W + BAR_GAP), BAR_MAX_H - bar_h);
        lv_obj_set_style_radius(bar, 1, 0);
        lv_color_t c = (i < bars_lit) ? color_on : color_off;
        lv_obj_set_style_bg_color(bar, c, 0);
        lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
    }
    return cont;
}

// ── RSSI → bar count ─────────────────────────────────────────────────────────

static int rssi_to_bars(int rssi)
{
    if (rssi >= -55) return 4;
    if (rssi >= -67) return 3;
    if (rssi >= -78) return 2;
    return 1;
}

// ── Row highlight ─────────────────────────────────────────────────────────────

static void highlight_row(lv_obj_t *row)
{
    if (s_selected_row && s_selected_row != row) {
        lv_obj_set_style_bg_color(s_selected_row, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_bg_opa(s_selected_row, LV_OPA_COVER, 0);
    }
    s_selected_row = row;
    lv_obj_set_style_bg_color(row, lv_color_hex(0x2979FF), 0);
    lv_obj_set_style_bg_opa(row, LV_OPA_COVER, 0);
}

// ── Row tap callback ──────────────────────────────────────────────────────────

static void row_click_cb(lv_event_t *e)
{
    lv_obj_t *row = lv_event_get_target(e);
    const char *ssid = (const char *)lv_event_get_user_data(e);
    if (ssid) {
        strncpy(s_selected_ssid, ssid, sizeof(s_selected_ssid) - 1);
        s_selected_ssid[sizeof(s_selected_ssid) - 1] = '\0';
        ESP_LOGI(TAG, "Row tapped: %s", s_selected_ssid);
    }
    highlight_row(row);
    audio_play_tick();
}

// ── List container ────────────────────────────────────────────────────────────

static lv_obj_t *get_or_create_list_cont(void)
{
    if (!s_list_cont) {
        // Build the scrollable list inside contscroll
        s_list_cont = lv_obj_create(GUI_Container__networkselector__contscroll);
        lv_obj_set_size(s_list_cont, LV_PCT(80), LV_PCT(100));
        lv_obj_set_style_pad_all(s_list_cont, 0, 0);
        lv_obj_set_style_pad_row(s_list_cont, 0, 0);
        lv_obj_set_scroll_dir(s_list_cont, LV_DIR_VER);
        lv_obj_set_scrollbar_mode(s_list_cont, LV_SCROLLBAR_MODE_AUTO);
        lv_obj_set_layout(s_list_cont, LV_LAYOUT_FLEX);
        lv_obj_set_style_flex_flow(s_list_cont, LV_FLEX_FLOW_COLUMN, 0);
        lv_obj_set_style_bg_color(s_list_cont, lv_color_hex(0xF5F5F5), 0);
        lv_obj_set_style_bg_opa(s_list_cont, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(s_list_cont, 1, 0);
        lv_obj_set_style_border_color(s_list_cont, lv_color_hex(0xAAAAAA), 0);
        lv_obj_set_style_radius(s_list_cont, 4, 0);
        // Wide scrollbar
        lv_obj_set_style_width(s_list_cont, 10, LV_PART_SCROLLBAR);
        lv_obj_set_style_bg_color(s_list_cont, lv_color_hex(0x666666), LV_PART_SCROLLBAR);
        lv_obj_set_style_bg_opa(s_list_cont, LV_OPA_COVER, LV_PART_SCROLLBAR);
        lv_obj_set_style_radius(s_list_cont, 5, LV_PART_SCROLLBAR);
    } else {
        lv_obj_clean(s_list_cont);
    }
    return s_list_cont;
}

// ── Populate list ─────────────────────────────────────────────────────────────

static void populate_network_list(wifi_ap_record_t *aps, uint16_t count)
{
    lv_obj_t *cont = get_or_create_list_cont();

    // Deduplicate — keep strongest signal per SSID
    // Use a simple seen-list since MAX_NETWORKS is small
    bool seen[MAX_NETWORKS] = {false};
    for (uint16_t i = 0; i < count && i < MAX_NETWORKS; i++) {
        if (seen[i] || aps[i].ssid[0] == '\0') continue;
        for (uint16_t j = i + 1; j < count && j < MAX_NETWORKS; j++) {
            if (!seen[j] && strcmp((char*)aps[i].ssid, (char*)aps[j].ssid) == 0) {
                // Keep whichever has stronger RSSI, mark the weaker as seen
                if (aps[j].rssi > aps[i].rssi) {
                    seen[i] = true;
                } else {
                    seen[j] = true;
                }
            }
        }
    }

    // Count valid unique
    uint16_t valid = 0;
    for (uint16_t i = 0; i < count && i < MAX_NETWORKS; i++) {
        if (!seen[i] && aps[i].ssid[0] != '\0') valid++;
    }

    if (valid == 0) {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_label_set_text(lbl, "No networks found.");
        lv_obj_set_style_text_color(lbl, lv_color_hex(0x444444), 0);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, 0);
        lv_obj_set_style_pad_all(lbl, 12, 0);
        return;
    }

    for (uint16_t i = 0; i < count && i < MAX_NETWORKS; i++) {
        const char *ssid = (const char *)aps[i].ssid;

        // Skip blank or duplicate SSIDs
        if (seen[i] || ssid[0] == '\0') continue;

        bool secured = (aps[i].authmode != WIFI_AUTH_OPEN);
        int bars = rssi_to_bars(aps[i].rssi);

        // Row — height sized to content
        lv_obj_t *row = lv_obj_create(cont);
        lv_obj_remove_style_all(row);
        lv_obj_set_width(row, LV_PCT(100));
        lv_obj_set_height(row, LV_SIZE_CONTENT);
        lv_obj_set_style_min_height(row, 52, 0);
        lv_obj_set_style_pad_left(row, ROW_PAD_LEFT, 0);
        lv_obj_set_style_pad_right(row, ROW_PAD_RIGHT, 0);
        lv_obj_set_style_pad_top(row, ROW_PAD_Y, 0);
        lv_obj_set_style_pad_bottom(row, ROW_PAD_Y, 0);
        lv_obj_set_style_bg_color(row, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_bg_opa(row, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(row, 1, 0);
        lv_obj_set_style_border_color(row, lv_color_hex(0xDDDDDD), 0);
        lv_obj_set_style_border_side(row, LV_BORDER_SIDE_BOTTOM, 0);
        lv_obj_set_style_radius(row, 0, 0);
        lv_obj_add_flag(row, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_layout(row, LV_LAYOUT_FLEX);
        lv_obj_set_style_flex_flow(row, LV_FLEX_FLOW_ROW, 0);
        lv_obj_set_style_flex_main_place(row, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_flex_cross_place(row, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_pad_column(row, ROW_PAD_X, 0);

        char *ssid_copy = NULL;
        if (s_ssid_pool_idx < MAX_NETWORKS) {
            ssid_copy = s_ssid_pool[s_ssid_pool_idx++];
            strncpy(ssid_copy, ssid, 32);
            ssid_copy[32] = '\0';
        }
        lv_obj_add_event_cb(row, row_click_cb, LV_EVENT_CLICKED, ssid_copy);

        // SSID label
        lv_obj_t *lbl = lv_label_create(row);
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_DOT);
        lv_obj_set_width(lbl, 260);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0x111111), 0);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, 0);
        lv_label_set_text(lbl, ssid);

        // Lock icon
        lv_obj_t *lock = lv_label_create(row);
        lv_obj_set_style_text_color(lock, secured ? lv_color_hex(0xFF8800) : lv_color_hex(0xDDDDDD), 0);
        lv_obj_set_style_text_font(lock, &lv_font_montserrat_20, 0);
        lv_label_set_text(lock, secured ? LV_SYMBOL_WARNING : " ");

        // Signal bars
        create_signal_bars(row, bars);
    }
}

// ── WiFi scan task (runs off LVGL task to avoid stack overflow) ───────────────

static void wifi_scan_task(void *arg)
{
    s_scan_running = true;
    // Show "starting" message, wait for WiFi to be ready
    bsp_display_lock(-1);
    if (s_searching_overlay) {
        lv_obj_t *lbl = lv_obj_get_child(s_searching_overlay, 0);
        if (lbl) lv_label_set_text(lbl, "STARTING WIFI RECEIVER");
    }
    bsp_display_unlock();

    vTaskDelay(pdMS_TO_TICKS(3000));

    // Switch to searching message
    bsp_display_lock(-1);
    if (s_searching_overlay) {
        lv_obj_t *lbl = lv_obj_get_child(s_searching_overlay, 0);
        if (lbl) lv_label_set_text(lbl, "SEARCHING FOR NETWORKS");
    }
    bsp_display_unlock();

    vTaskDelay(pdMS_TO_TICKS(500));

    wifi_scan_config_t scan_cfg = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = false,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
    };

    esp_err_t err = esp_wifi_scan_start(&scan_cfg, true); // blocking

    bsp_display_lock(-1);

    show_searching_overlay(false);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Scan failed: %s", esp_err_to_name(err));
        lv_obj_t *cont = get_or_create_list_cont();
        lv_obj_t *err_lbl = lv_label_create(cont);
        lv_label_set_text(err_lbl, "Scan failed.\nCheck WiFi init.");
        lv_obj_set_style_text_color(err_lbl, lv_color_hex(0xFF0000), 0);
        lv_obj_set_style_text_font(err_lbl, &font_15, 0);
    } else {
        uint16_t count = MAX_NETWORKS;
        wifi_ap_record_t *ap_records = malloc(sizeof(wifi_ap_record_t) * MAX_NETWORKS);
        if (ap_records) {
            esp_wifi_scan_get_ap_records(&count, ap_records);
            ESP_LOGI(TAG, "Scan found %d networks", count);
            populate_network_list(ap_records, count);
            free(ap_records);
        }
    }

    bsp_display_unlock();
    s_scan_running = false;
    vTaskDelete(NULL);
}

static void do_wifi_scan(void)
{
    if (s_scan_running) {
        ESP_LOGW(TAG, "Scan already running, skipping");
        return;
    }
    show_searching_overlay(true);
    xTaskCreate(wifi_scan_task, "wifi_scan", 8192, NULL, 5, NULL);
}

// ── Button callbacks ──────────────────────────────────────────────────────────

static void networksave_cb(lv_event_t *e)
{
    (void)e;
    if (strlen(s_selected_ssid) == 0) {
        ESP_LOGW(TAG, "Save pressed but no network selected");
        return;
    }
    ESP_LOGI(TAG, "Network selected: %s", s_selected_ssid);
    if (s_on_selected) s_on_selected(s_selected_ssid);
    audio_play_tick();
    lv_screen_load(GUI_Screen__settingswindow);
}

static void networkexit_cb(lv_event_t *e)
{
    (void)e;
    s_selected_ssid[0] = '\0';
    s_selected_row = NULL;
    audio_play_tick();
    lv_screen_load(GUI_Screen__settingswindow);
}

// ── Screen load ───────────────────────────────────────────────────────────────

static void networkselector_load_cb(lv_event_t *e)
{
    (void)e;
    s_selected_row = NULL;
    s_selected_ssid[0] = '\0';
    s_ssid_pool_idx = 0;

    // Wait for any previous scan to finish before destroying UI objects
    int wait = 0;
    while (s_scan_running && wait < 50) {
        vTaskDelay(pdMS_TO_TICKS(100));
        wait++;
    }
    if (s_scan_running) {
        esp_wifi_scan_stop(); // force stop if still running
        vTaskDelay(pdMS_TO_TICKS(200));
    }

    // Destroy old list container so it gets recreated fresh
    if (s_list_cont) {
        lv_obj_delete(s_list_cont);
        s_list_cont = NULL;
    }
    // Destroy old overlay so it gets recreated fresh
    if (s_searching_overlay) {
        lv_obj_delete(s_searching_overlay);
        s_searching_overlay = NULL;
    }

    do_wifi_scan();
}

// ── Public API ────────────────────────────────────────────────────────────────

void network_selector_register_cb(network_selected_cb_t cb)
{
    s_on_selected = cb;
}

void network_selector_ui_init(void)
{
    lv_obj_add_event_cb(GUI_Screen__networkselector,
                        networkselector_load_cb,
                        LV_EVENT_SCREEN_LOADED, NULL);

    lv_obj_add_event_cb(GUI_Button__networkselector__networksave,
                        networksave_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_add_event_cb(GUI_Button__networkselector__networkexit,
                        networkexit_cb, LV_EVENT_CLICKED, NULL);

    // Fix SquareLine (20,20) position offset on exit button + expand touch area
    lv_obj_set_pos(GUI_Button__networkselector__networkexit, 0, 0);
    lv_obj_set_ext_click_area(GUI_Button__networkselector__networkexit, 10);
    lv_obj_set_ext_click_area(GUI_Button__networkselector__networksave, 10);

    // Force exit button style (red) — style class may have been lost in truncation
    lv_obj_set_style_bg_color(GUI_Button__networkselector__networkexit,
                              lv_color_hex(0xCC2222), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(GUI_Button__networkselector__networkexit,
                            LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(GUI_Label__networkselector__label_13,
                                lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(GUI_Label__networkselector__label_13,
                               &title_1, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Force save button style (green)
    lv_obj_set_style_bg_color(GUI_Button__networkselector__networksave,
                              lv_color_hex(0x228822), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(GUI_Button__networkselector__networksave,
                            LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(GUI_Label__networkselector__label_14,
                                lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(GUI_Label__networkselector__label_14,
                               &title_1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ESP_LOGI(TAG, "Network selector UI initialized");
}
