#include "ota_update.h"
#include "wifi_manager.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_crt_bundle.h"
#include "esp_ota_ops.h"
#include "esp_app_desc.h"
#include "esp_system.h"
#include "cJSON.h"
#include "lvgl.h"
#include "bsp/esp-bsp.h"

static const char *TAG = "OTA";

#define OTA_RELEASES_URL  "https://api.github.com/repos/hubris123/DeskMediaDeviceV2/releases/latest"
#define OTA_USER_AGENT    "DeskMediaDevice/1.0"
#define OTA_FIRST_CHECK_DELAY_MS  (60 * 1000)
#define OTA_RECHECK_PERIOD_MS     (24 * 60 * 60 * 1000)
#define OTA_API_BUF_SIZE          (16 * 1024)

static char s_new_tag[64];
static char s_bin_url[512];
static volatile bool s_install_requested = false;
static volatile bool s_prompt_open = false;

// ── GitHub API: fetch latest release tag + .bin asset URL ────────────────────

static esp_err_t fetch_latest_release(char *tag, size_t tag_len,
                                      char *url, size_t url_len)
{
    char *buf = malloc(OTA_API_BUF_SIZE);
    if (!buf) return ESP_ERR_NO_MEM;

    esp_http_client_config_t cfg = {
        .url = OTA_RELEASES_URL,
        .user_agent = OTA_USER_AGENT,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .timeout_ms = 15000,
    };
    esp_http_client_handle_t client = esp_http_client_init(&cfg);
    if (!client) { free(buf); return ESP_FAIL; }
    esp_http_client_set_header(client, "Accept", "application/vnd.github+json");

    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) goto out;
    esp_http_client_fetch_headers(client);

    int total = 0, n;
    while ((n = esp_http_client_read(client, buf + total, OTA_API_BUF_SIZE - 1 - total)) > 0) {
        total += n;
        if (total >= OTA_API_BUF_SIZE - 1) break;
    }
    buf[total] = '\0';

    int status = esp_http_client_get_status_code(client);
    if (status != 200) {
        ESP_LOGW(TAG, "Release API HTTP %d", status);
        err = ESP_FAIL;
        goto out;
    }

    cJSON *root = cJSON_Parse(buf);
    if (!root) { err = ESP_FAIL; goto out; }

    err = ESP_ERR_NOT_FOUND;
    cJSON *tag_j = cJSON_GetObjectItem(root, "tag_name");
    cJSON *assets = cJSON_GetObjectItem(root, "assets");
    if (cJSON_IsString(tag_j) && cJSON_IsArray(assets)) {
        cJSON *a;
        cJSON_ArrayForEach(a, assets) {
            cJSON *name = cJSON_GetObjectItem(a, "name");
            cJSON *dl   = cJSON_GetObjectItem(a, "browser_download_url");
            if (cJSON_IsString(name) && cJSON_IsString(dl) &&
                strstr(name->valuestring, ".bin")) {
                strlcpy(tag, tag_j->valuestring, tag_len);
                strlcpy(url, dl->valuestring, url_len);
                err = ESP_OK;
                break;
            }
        }
    }
    cJSON_Delete(root);

out:
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    free(buf);
    return err;
}

// ── Install ──────────────────────────────────────────────────────────────────

static void ota_install_task(void *arg)
{
    (void)arg;
    ESP_LOGI(TAG, "Starting OTA from %s", s_bin_url);

    esp_http_client_config_t http_cfg = {
        .url = s_bin_url,
        .user_agent = OTA_USER_AGENT,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .timeout_ms = 30000,
        .buffer_size = 8 * 1024,
        .buffer_size_tx = 2048,   // GitHub redirect URLs are long
    };
    esp_https_ota_config_t ota_cfg = {
        .http_config = &http_cfg,
    };

    esp_err_t err = esp_https_ota(&ota_cfg);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "OTA OK — rebooting into new firmware");
        vTaskDelay(pdMS_TO_TICKS(1000));
        esp_restart();
    }

    ESP_LOGE(TAG, "OTA failed: %s", esp_err_to_name(err));
    s_install_requested = false; // allow a retry on the next check cycle
    vTaskDelete(NULL);
}

// ── Update prompt (runs in LVGL context) ─────────────────────────────────────

static void prompt_btn_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    bool install = (bool)(uintptr_t)lv_event_get_user_data(e);
    lv_obj_t *mbox = lv_obj_get_parent(lv_obj_get_parent(btn));
    if (install) {
        s_install_requested = true;
        xTaskCreate(ota_install_task, "ota_install", 8192, NULL, 5, NULL);
    }
    s_prompt_open = false;
    // Async: deleting the msgbox synchronously from inside its own button's
    // event handler frees the object tree the event core is still walking.
    lv_msgbox_close_async(mbox);
}

LV_FONT_DECLARE(header_1);          // 27px UI font (SquareLine family)
LV_FONT_DECLARE(title_1);           // 17px UI font

static void style_prompt_button(lv_obj_t *btn, lv_color_t bg)
{
    lv_obj_set_style_bg_color(btn, bg, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(btn, 6, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_text_color(btn, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(btn, &title_1, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(btn, 18, LV_PART_MAIN);
    lv_obj_set_style_pad_ver(btn, 8, LV_PART_MAIN);
}

static void show_update_prompt(void)
{
    if (s_prompt_open) return;
    s_prompt_open = true;

    // -1 = block until the LVGL mutex is ours. 0 (try-once) returned failure
    // under load and the code then touched LVGL unlocked — a race with the
    // render task.
    bsp_display_lock(-1);
    lv_obj_t *mbox = lv_msgbox_create(NULL);

    // Match the app theme: black panel, white text, thin light border
    lv_obj_set_style_bg_color(mbox, lv_color_hex(0x0A0A0A), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mbox, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(mbox, lv_color_hex(0x3A3A3A), LV_PART_MAIN);
    lv_obj_set_style_border_width(mbox, 2, LV_PART_MAIN);
    lv_obj_set_style_radius(mbox, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_all(mbox, 14, LV_PART_MAIN);
    lv_obj_set_width(mbox, 330);

    lv_obj_t *title = lv_msgbox_add_title(mbox, "Firmware Update");
    lv_obj_set_style_text_color(title, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &header_1, LV_PART_MAIN);
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_width(title, lv_pct(100));

    char msg[160];
    snprintf(msg, sizeof(msg), "New version available: %s\nCurrent: %s",
             s_new_tag, esp_app_get_description()->version);
    lv_obj_t *text = lv_msgbox_add_text(mbox, msg);
    lv_obj_set_style_text_color(text, lv_color_hex(0xCCCCCC), LV_PART_MAIN);
    lv_obj_set_style_text_font(text, &title_1, LV_PART_MAIN);
    lv_obj_set_style_text_align(text, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_width(text, lv_pct(100));
    // Breathing room between the text block and the buttons
    lv_obj_set_style_pad_bottom(text, 14, LV_PART_MAIN);

    // Header/content/footer wrappers inherit theme colors — flatten them
    lv_obj_set_style_bg_opa(lv_msgbox_get_header(mbox), LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(lv_msgbox_get_content(mbox), LV_OPA_TRANSP, LV_PART_MAIN);

    lv_obj_t *b1 = lv_msgbox_add_footer_button(mbox, "Install");
    lv_obj_t *b2 = lv_msgbox_add_footer_button(mbox, "Later");
    lv_obj_t *footer = lv_msgbox_get_footer(mbox);
    if (footer) lv_obj_set_style_bg_opa(footer, LV_OPA_TRANSP, LV_PART_MAIN);
    style_prompt_button(b1, lv_color_hex(0xE07A00));   // amber accent like the temps
    style_prompt_button(b2, lv_color_hex(0x2E2E2E));   // muted gray

    lv_obj_add_event_cb(b1, prompt_btn_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)1);
    lv_obj_add_event_cb(b2, prompt_btn_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)0);
    lv_obj_center(mbox);
    bsp_display_unlock();
}

// ── Check task ───────────────────────────────────────────────────────────────

static void ota_check_task(void *arg)
{
    (void)arg;
    vTaskDelay(pdMS_TO_TICKS(OTA_FIRST_CHECK_DELAY_MS));

    for (;;) {
        if (wifi_manager_is_connected() && !s_install_requested) {
            char tag[sizeof(s_new_tag)] = "";
            char url[sizeof(s_bin_url)] = "";
            if (fetch_latest_release(tag, sizeof(tag), url, sizeof(url)) == ESP_OK) {
                const char *running = esp_app_get_description()->version;
                ESP_LOGI(TAG, "Latest release: %s (running: %s)", tag, running);
                if (strcmp(tag, running) != 0) {
                    strlcpy(s_new_tag, tag, sizeof(s_new_tag));
                    strlcpy(s_bin_url, url, sizeof(s_bin_url));
                    show_update_prompt();
                }
            } else {
                ESP_LOGW(TAG, "Release check failed (no release yet, or network)");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(OTA_RECHECK_PERIOD_MS));
    }
}

// ── Public API ───────────────────────────────────────────────────────────────

void ota_update_start(void)
{
    xTaskCreate(ota_check_task, "ota_check", 8192, NULL, 3, NULL);
}

void ota_update_mark_boot_valid(void)
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_ota_img_states_t state;
    if (esp_ota_get_state_partition(running, &state) == ESP_OK &&
        state == ESP_OTA_IMG_PENDING_VERIFY) {
        esp_ota_mark_app_valid_cancel_rollback();
        ESP_LOGI(TAG, "Boot marked valid — rollback cancelled");
    }
}
