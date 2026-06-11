#include "weather_api.h"
#include "nws_api.h"
#include "wifi_manager.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "cJSON.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

static const char *TAG = "WeatherAPI";

/* ── Zippopotam geocoding ───────────────────────────────────────────────── */

typedef struct {
    char *buffer;
    size_t size;
    size_t capacity;
} geo_resp_t;

static esp_err_t geo_evt(esp_http_client_event_t *evt)
{
    geo_resp_t *r = evt->user_data;
    if (evt->event_id == HTTP_EVENT_ON_DATA) {
        if (r->size + evt->data_len >= r->capacity) {
            ESP_LOGW(TAG, "geo buffer overflow");
            return ESP_FAIL;
        }
        memcpy(r->buffer + r->size, evt->data, evt->data_len);
        r->size += evt->data_len;
    }
    return ESP_OK;
}

esp_err_t weather_geocode_zipcode(const char *zip, location_t *location)
{
    if (!zip || !location) return ESP_ERR_INVALID_ARG;

    char url[128];
    snprintf(url, sizeof(url), "http://api.zippopotam.us/us/%s", zip);
    ESP_LOGI(TAG, "Geocoding ZIP: %s", zip);

    geo_resp_t r = {0};
    r.buffer = malloc(WEATHER_HTTP_BUFFER_SIZE);
    if (!r.buffer) return ESP_ERR_NO_MEM;
    r.capacity = WEATHER_HTTP_BUFFER_SIZE;

    esp_http_client_config_t cfg = {
        .url            = url,
        .timeout_ms     = WEATHER_API_TIMEOUT_MS,
        .user_data      = &r,
        .event_handler  = geo_evt,
        .transport_type = HTTP_TRANSPORT_OVER_TCP,
    };
    esp_http_client_handle_t client = esp_http_client_init(&cfg);
    if (!client) { free(r.buffer); return ESP_FAIL; }

    esp_http_client_set_header(client, "User-Agent", "DeskMediaDevice/1.0");
    esp_http_client_set_header(client, "Accept",     "application/json");

    esp_err_t err = esp_http_client_perform(client);
    int status    = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);

    if (err != ESP_OK || status != 200 || r.size == 0) {
        ESP_LOGE(TAG, "Geocode HTTP failed: err=%s status=%d size=%u",
                 esp_err_to_name(err), status, (unsigned)r.size);
        free(r.buffer);
        return (err != ESP_OK) ? err : ESP_FAIL;
    }
    r.buffer[r.size] = '\0';

    cJSON *root = cJSON_Parse(r.buffer);
    free(r.buffer);
    if (!root) { ESP_LOGE(TAG, "geocode JSON parse error"); return ESP_FAIL; }

    cJSON *places = cJSON_GetObjectItem(root, "places");
    if (!cJSON_IsArray(places) || cJSON_GetArraySize(places) == 0) {
        ESP_LOGE(TAG, "geocode: no results for ZIP %s", zip);
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    cJSON *first      = cJSON_GetArrayItem(places, 0);
    cJSON *place_name = cJSON_GetObjectItem(first, "place name");
    cJSON *state      = cJSON_GetObjectItem(first, "state abbreviation");
    cJSON *lat        = cJSON_GetObjectItem(first, "latitude");
    cJSON *lon        = cJSON_GetObjectItem(first, "longitude");

    if (!place_name || !state || !lat || !lon) {
        ESP_LOGE(TAG, "geocode: missing fields");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    strncpy(location->city,  place_name->valuestring, sizeof(location->city)  - 1);
    strncpy(location->state, state->valuestring,       sizeof(location->state) - 1);
    location->latitude  = (float)atof(lat->valuestring);
    location->longitude = (float)atof(lon->valuestring);
    location->timestamp = (uint32_t)time(NULL);

    cJSON_Delete(root);
    ESP_LOGI(TAG, "Geocoded: %s, %s (%.4f, %.4f)",
             location->city, location->state, location->latitude, location->longitude);
    return ESP_OK;
}

bool weather_is_wifi_ready(void)
{
    return wifi_manager_is_connected();
}
