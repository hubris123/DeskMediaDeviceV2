#include "wifi_manager.h"
#include "storage/nvs_storage.h"
#include "weather/weather_task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "lwip/apps/sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include <string.h>

static const char *TAG = "WiFiManager";

#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1
#define MAX_RETRIES         5
#define RECONNECT_INTERVAL_US  (30 * 1000000LL)  // 30 s between persistent reconnect attempts

static EventGroupHandle_t s_wifi_event_group = NULL;
static bool s_connected = false;
static bool s_connecting = false;
static int s_retry_count = 0;
static char s_ip[16] = "0.0.0.0";
static uint8_t s_last_disconnect_reason = 0;

// ── Persistent reconnect ──────────────────────────────────────────────────────
// After the fast retry burst fails, if we have saved credentials we keep trying
// forever on a 30 s timer. Covers an overnight AP drop and the power-loss case
// where the device reboots before the router is back. Cleared on a successful IP.
static bool s_persistent_reconnect = false;
static esp_timer_handle_t s_reconnect_timer = NULL;
static int64_t s_next_attempt_us = 0;

static bool have_saved_creds(void)
{
    char ssid[33] = "";
    char pass[65] = "";
    if (nvs_load_wifi(ssid, sizeof(ssid), pass, sizeof(pass)) != ESP_OK) return false;
    return strlen(ssid) > 0;
}

static void reconnect_timer_cb(void *arg)
{
    (void)arg;
    if (!s_persistent_reconnect) return;
    s_next_attempt_us = esp_timer_get_time() + RECONNECT_INTERVAL_US;
    ESP_LOGI(TAG, "Persistent reconnect: attempting...");
    esp_wifi_connect();
}

static void start_persistent_reconnect(void)
{
    if (!s_reconnect_timer) {
        const esp_timer_create_args_t args = {
            .callback = reconnect_timer_cb,
            .name = "wifi_reconnect",
        };
        if (esp_timer_create(&args, &s_reconnect_timer) != ESP_OK) {
            ESP_LOGE(TAG, "Failed to create reconnect timer");
            return;
        }
    }
    s_persistent_reconnect = true;
    s_next_attempt_us = esp_timer_get_time() + RECONNECT_INTERVAL_US;
    if (!esp_timer_is_active(s_reconnect_timer)) {
        esp_timer_start_periodic(s_reconnect_timer, RECONNECT_INTERVAL_US);
    }
}

static void stop_persistent_reconnect(void)
{
    s_persistent_reconnect = false;
    if (s_reconnect_timer && esp_timer_is_active(s_reconnect_timer)) {
        esp_timer_stop(s_reconnect_timer);
    }
}

// ── SNTP ─────────────────────────────────────────────────────────────────────

static bool s_sntp_started = false;

static void start_sntp(void)
{
    if (s_sntp_started) {
        ESP_LOGI(TAG, "SNTP already running, skipping");
        return;
    }
    ESP_LOGI(TAG, "Starting SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
    s_sntp_started = true;
}

// ── Event handler ─────────────────────────────────────────────────────────────

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_STA_START) {
            ESP_LOGI(TAG, "WiFi STA started");
        } else if (event_id == WIFI_EVENT_STA_CONNECTED) {
            ESP_LOGI(TAG, "WiFi connected to AP");
            s_retry_count = 0;
        } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
            s_connected = false;
            strcpy(s_ip, "0.0.0.0");
            wifi_event_sta_disconnected_t *disc = (wifi_event_sta_disconnected_t *)event_data;
            s_last_disconnect_reason = disc->reason;
            ESP_LOGW(TAG, "WiFi disconnected, reason: %d", disc->reason);
            if (s_persistent_reconnect) {
                // Already in slow-retry mode — the 30 s timer owns the next attempt.
                s_connecting = false;
            } else if (s_retry_count < MAX_RETRIES) {
                s_retry_count++;
                ESP_LOGI(TAG, "Retrying connection (%d/%d)...", s_retry_count, MAX_RETRIES);
                esp_wifi_connect();
            } else {
                s_connecting = false;
                if (have_saved_creds()) {
                    ESP_LOGW(TAG, "Max retries reached — entering persistent reconnect (every 30 s)");
                    start_persistent_reconnect();
                } else {
                    ESP_LOGW(TAG, "Max retries reached, no saved credentials, giving up");
                    if (s_wifi_event_group)
                        xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
                }
            }
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        snprintf(s_ip, sizeof(s_ip), IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI(TAG, "Got IP: %s", s_ip);
        s_connected = true;
        s_connecting = false;
        s_retry_count = 0;
        stop_persistent_reconnect();  // back online — cancel the 30 s reconnect loop
        if (s_wifi_event_group)
            xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        start_sntp();
        // Re-trigger weather fetch after reconnect (handles network switch case)
        char zip[16] = "";
        if (nvs_load_zipcode(zip, sizeof(zip)) == ESP_OK && strlen(zip) > 0) {
            weather_set_location(zip);
        }
    }
}

// ── Public API ────────────────────────────────────────────────────────────────

esp_err_t wifi_manager_init(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                    ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                    IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));

    ESP_LOGI(TAG, "WiFi manager initialized");
    return ESP_OK;
}

esp_err_t wifi_manager_connect(const char *ssid, const char *password)
{
    if (!ssid || strlen(ssid) == 0) {
        ESP_LOGW(TAG, "No SSID provided");
        return ESP_ERR_INVALID_ARG;
    }

    wifi_config_t wifi_config = {0};
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
    if (password && strlen(password) > 0) {
        strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);
    }
    wifi_config.sta.threshold.authmode = WIFI_AUTH_OPEN;

    s_retry_count = 0;
    s_connected = false;
    s_connecting = true;
    s_last_disconnect_reason = 0;  // clear stale reason — new attempt starting
    stop_persistent_reconnect();   // fresh managed attempt owns the retry burst

    // Disconnect first if already connected — required before switching networks
    esp_wifi_disconnect();
    vTaskDelay(pdMS_TO_TICKS(500));

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    esp_err_t err = esp_wifi_connect();
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Connecting to SSID: %s", ssid);
    } else {
        ESP_LOGE(TAG, "esp_wifi_connect failed: %s", esp_err_to_name(err));
    }
    return err;
}

esp_err_t wifi_manager_connect_saved(void)
{
    char ssid[33] = "";
    char pass[65] = "";

    if (nvs_load_wifi(ssid, sizeof(ssid), pass, sizeof(pass)) != ESP_OK) {
        ESP_LOGW(TAG, "No saved WiFi credentials");
        return ESP_FAIL;
    }

    if (strlen(ssid) == 0) {
        ESP_LOGW(TAG, "Saved SSID is empty");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Connecting with saved credentials: %s", ssid);
    return wifi_manager_connect(ssid, pass);
}

bool wifi_manager_is_connected(void)
{
    return s_connected;
}

void wifi_manager_get_ip(char *buf, size_t len)
{
    if (buf) strncpy(buf, s_ip, len - 1);
}

uint8_t wifi_manager_get_last_disconnect_reason(void)
{
    return s_last_disconnect_reason;
}

bool wifi_manager_is_connecting(void)
{
    return s_connecting;
}

bool wifi_manager_is_reconnecting(void)
{
    return s_persistent_reconnect;
}

int wifi_manager_get_reconnect_countdown(void)
{
    if (!s_persistent_reconnect) return 0;
    int64_t rem = s_next_attempt_us - esp_timer_get_time();
    if (rem < 0) rem = 0;
    return (int)(rem / 1000000LL);
}
