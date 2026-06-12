#include "nvs_storage.h"
#include "nws_api.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include <string.h>
#include <time.h>

static const char *TAG = "NVSStorage";
static const char *NAMESPACE = "weather";

static nvs_handle_t nvs_h = 0;
static bool nvs_initialized = false;

// Helper: store float as u32 bits
static esp_err_t nvs_set_float(nvs_handle_t h, const char *key, float val) {
    uint32_t u; memcpy(&u, &val, 4); return nvs_set_u32(h, key, u);
}
// Helper: load float from u32 bits
static esp_err_t nvs_get_float(nvs_handle_t h, const char *key, float *val) {
    uint32_t u = 0; esp_err_t r = nvs_get_u32(h, key, &u); memcpy(val, &u, 4); return r;
}

// ============ Public API ============

esp_err_t nvs_storage_init(void)
{
    if (nvs_initialized) {
        return ESP_OK;
    }

    esp_err_t ret = nvs_open(NAMESPACE, NVS_READWRITE, &nvs_h);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS: %s", esp_err_to_name(ret));
        return ret;
    }

    nvs_initialized = true;
    ESP_LOGI(TAG, "NVS storage initialized");
    return ESP_OK;
}

esp_err_t nvs_store_location(const location_t *loc)
{
    if (!loc) return ESP_ERR_INVALID_ARG;
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }

    esp_err_t ret = ESP_OK;
    ret |= nvs_set_str(nvs_h, "city", loc->city);
    ret |= nvs_set_str(nvs_h, "state", loc->state);
    ret |= nvs_set_float(nvs_h, "lat", loc->latitude);
    ret |= nvs_set_float(nvs_h, "lon", loc->longitude);
    ret |= nvs_commit(nvs_h);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Location stored: %s, %s (%.4f, %.4f)",
                loc->city, loc->state, loc->latitude, loc->longitude);
    } else {
        ESP_LOGE(TAG, "Failed to store location");
    }
    return ret;
}

esp_err_t nvs_load_location(location_t *loc)
{
    if (!loc) return ESP_ERR_INVALID_ARG;
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }

    esp_err_t ret = ESP_OK;
    size_t len = 0;

    ret = nvs_get_str(nvs_h, "city", NULL, &len);
    if (ret == ESP_OK && len > 0) {
        if (len > sizeof(loc->city)) len = sizeof(loc->city);
        nvs_get_str(nvs_h, "city", loc->city, &len);
    } else {
        memset(loc->city, 0, sizeof(loc->city));
        return ESP_FAIL;
    }

    len = 0;
    ret = nvs_get_str(nvs_h, "state", NULL, &len);
    if (ret == ESP_OK && len > 0) {
        if (len > sizeof(loc->state)) len = sizeof(loc->state);
        nvs_get_str(nvs_h, "state", loc->state, &len);
    } else {
        memset(loc->state, 0, sizeof(loc->state));
        return ESP_FAIL;
    }

    ret = nvs_get_float(nvs_h, "lat", &loc->latitude);
    if (ret != ESP_OK) { ESP_LOGW(TAG, "Failed to load latitude"); return ESP_FAIL; }

    ret = nvs_get_float(nvs_h, "lon", &loc->longitude);
    if (ret != ESP_OK) { ESP_LOGW(TAG, "Failed to load longitude"); return ESP_FAIL; }

    loc->timestamp = (uint32_t)time(NULL);
    ESP_LOGI(TAG, "Location loaded: %s, %s (%.4f, %.4f)",
            loc->city, loc->state, loc->latitude, loc->longitude);
    return ESP_OK;
}

bool nvs_has_valid_location(void)
{
    if (!nvs_initialized) return false;
    char city[50] = {0};
    size_t len = sizeof(city);
    esp_err_t ret = nvs_get_str(nvs_h, "city", city, &len);
    return (ret == ESP_OK && len > 1);
}

esp_err_t nvs_store_weather(const weather_data_t *weather)
{
    if (!weather) return ESP_ERR_INVALID_ARG;
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }

    esp_err_t ret = ESP_OK;

    ret |= nvs_set_float(nvs_h, "temp", weather->current_temp);
    ret |= nvs_set_i32(nvs_h, "humidity", (int32_t)weather->current_humidity);
    ret |= nvs_set_i32(nvs_h, "precip", (int32_t)weather->current_precip_prob);
    ret |= nvs_set_i32(nvs_h, "wmo", (int32_t)weather->current_weather_code);
    ret |= nvs_set_u32(nvs_h, "w_time", weather->last_update);
    ret |= nvs_set_str(nvs_h, "w_city", weather->city);
    ret |= nvs_set_str(nvs_h, "w_state", weather->state);
    ret |= nvs_set_str(nvs_h, "w_tz", weather->timezone);

    for (int i = 0; i < 5; i++) {
        char key[16];
        snprintf(key, sizeof(key), "h_temp_%d", i);
        nvs_set_float(nvs_h, key, weather->hourly.hours[i].temperature);
    }

    for (int i = 0; i < 3; i++) {
        char key[16];
        snprintf(key, sizeof(key), "d_high_%d", i);
        nvs_set_float(nvs_h, key, weather->daily[i].temp_high);
        snprintf(key, sizeof(key), "d_low_%d", i);
        nvs_set_float(nvs_h, key, weather->daily[i].temp_low);
    }

    ret |= nvs_commit(nvs_h);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Weather cached: %.1f, %.0f%%", weather->current_temp,
                weather->current_humidity);
    } else {
        ESP_LOGW(TAG, "Failed to cache weather");
    }
    return ret;
}

esp_err_t nvs_load_weather(weather_data_t *weather)
{
    if (!weather) return ESP_ERR_INVALID_ARG;
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }

    memset(weather, 0, sizeof(weather_data_t));

    size_t len = 0;

    nvs_get_float(nvs_h, "temp", &weather->current_temp);

    int32_t humidity = 0;
    nvs_get_i32(nvs_h, "humidity", &humidity);
    weather->current_humidity = (int)humidity;

    int32_t precip = 0;
    nvs_get_i32(nvs_h, "precip", &precip);
    weather->current_precip_prob = (int)precip;

    int32_t wmo = 0;
    nvs_get_i32(nvs_h, "wmo", &wmo);
    weather->current_weather_code = (int)wmo;

    uint32_t w_time = 0;
    nvs_get_u32(nvs_h, "w_time", &w_time);
    weather->last_update = w_time;

    len = sizeof(weather->city);
    nvs_get_str(nvs_h, "w_city", weather->city, &len);

    len = sizeof(weather->state);
    nvs_get_str(nvs_h, "w_state", weather->state, &len);

    len = sizeof(weather->timezone);
    nvs_get_str(nvs_h, "w_tz", weather->timezone, &len);

    for (int i = 0; i < 5; i++) {
        char key[16];
        snprintf(key, sizeof(key), "h_temp_%d", i);
        nvs_get_float(nvs_h, key, &weather->hourly.hours[i].temperature);
    }

    for (int i = 0; i < 3; i++) {
        char key[16];
        snprintf(key, sizeof(key), "d_high_%d", i);
        nvs_get_float(nvs_h, key, &weather->daily[i].temp_high);
        snprintf(key, sizeof(key), "d_low_%d", i);
        nvs_get_float(nvs_h, key, &weather->daily[i].temp_low);
    }

    if (weather->current_temp != 0.0f && weather->last_update != 0) {
        weather->is_valid = true;
    }

    if (weather->is_valid) {
        ESP_LOGI(TAG, "Weather loaded from cache: %.1f", weather->current_temp);
    } else {
        ESP_LOGW(TAG, "No valid cached weather");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t nvs_clear_weather(void)
{
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }
    nvs_erase_all(nvs_h);
    nvs_commit(nvs_h);
    ESP_LOGI(TAG, "Weather data cleared");
    return ESP_OK;
}

// ── WiFi credentials ──────────────────────────────────────────────────────────

esp_err_t nvs_store_wifi(const char *ssid, const char *password)
{
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }
    esp_err_t ret = ESP_OK;
    ret |= nvs_set_str(nvs_h, "wifi_ssid", ssid ? ssid : "");
    ret |= nvs_set_str(nvs_h, "wifi_pass", password ? password : "");
    ret |= nvs_commit(nvs_h);
    if (ret == ESP_OK) ESP_LOGI(TAG, "WiFi credentials saved: %s", ssid);
    else ESP_LOGE(TAG, "Failed to save WiFi credentials");
    return ret;
}

esp_err_t nvs_load_wifi(char *ssid, size_t ssid_len, char *password, size_t pass_len)
{
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }
    esp_err_t ret = ESP_OK;
    ret |= nvs_get_str(nvs_h, "wifi_ssid", ssid, &ssid_len);
    ret |= nvs_get_str(nvs_h, "wifi_pass", password, &pass_len);
    if (ret == ESP_OK) ESP_LOGI(TAG, "WiFi credentials loaded: %s", ssid);
    else ESP_LOGW(TAG, "No saved WiFi credentials");
    return ret;
}

// ── Zip code ──────────────────────────────────────────────────────────────────

esp_err_t nvs_store_zipcode(const char *zip)
{
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }
    esp_err_t ret = nvs_set_str(nvs_h, "zipcode", zip ? zip : "");
    ret |= nvs_commit(nvs_h);
    if (ret == ESP_OK) ESP_LOGI(TAG, "Zip code saved: %s", zip);
    return ret;
}

esp_err_t nvs_load_zipcode(char *zip, size_t len)
{
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }
    esp_err_t ret = nvs_get_str(nvs_h, "zipcode", zip, &len);
    if (ret == ESP_OK) ESP_LOGI(TAG, "Zip code loaded: %s", zip);
    else ESP_LOGW(TAG, "No saved zip code");
    return ret;
}

// ── NWS grid / station cache ──────────────────────────────────────────────────

esp_err_t nvs_store_nws_grid(const nws_grid_t *grid)
{
    if (!grid) return ESP_ERR_INVALID_ARG;
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }
    esp_err_t ret = ESP_OK;
    ret |= nvs_set_str(nvs_h, "nws_gid",  grid->grid_id);
    ret |= nvs_set_i32(nvs_h, "nws_gx",   (int32_t)grid->grid_x);
    ret |= nvs_set_i32(nvs_h, "nws_gy",   (int32_t)grid->grid_y);
    ret |= nvs_set_str(nvs_h, "nws_sta",  grid->station);
    ret |= nvs_commit(nvs_h);
    if (ret == ESP_OK)
        ESP_LOGI(TAG, "NWS grid cached: %s/%d,%d station=%s",
                 grid->grid_id, grid->grid_x, grid->grid_y, grid->station);
    else
        ESP_LOGW(TAG, "Failed to cache NWS grid");
    return ret;
}

esp_err_t nvs_load_nws_grid(nws_grid_t *grid)
{
    if (!grid) return ESP_ERR_INVALID_ARG;
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }
    memset(grid, 0, sizeof(*grid));

    size_t len = sizeof(grid->grid_id);
    if (nvs_get_str(nvs_h, "nws_gid", grid->grid_id, &len) != ESP_OK ||
        !grid->grid_id[0]) {
        ESP_LOGW(TAG, "No cached NWS grid");
        return ESP_FAIL;
    }

    int32_t gx = 0, gy = 0;
    nvs_get_i32(nvs_h, "nws_gx", &gx);
    nvs_get_i32(nvs_h, "nws_gy", &gy);
    grid->grid_x = (int)gx;
    grid->grid_y = (int)gy;

    len = sizeof(grid->station);
    nvs_get_str(nvs_h, "nws_sta", grid->station, &len);

    ESP_LOGI(TAG, "NWS grid loaded: %s/%d,%d station=%s",
             grid->grid_id, grid->grid_x, grid->grid_y, grid->station);
    return ESP_OK;
}

// ── Display brightness ────────────────────────────────────────────────────────

esp_err_t nvs_store_brightness(int value)
{
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }
    esp_err_t ret = nvs_set_i32(nvs_h, "brightness", (int32_t)value);
    ret |= nvs_commit(nvs_h);
    return ret;
}

int nvs_load_brightness(int default_val)
{
    if (!nvs_initialized) return default_val;
    int32_t val = (int32_t)default_val;
    nvs_get_i32(nvs_h, "brightness", &val);
    return (int)val;
}

// ── Speaker volume ────────────────────────────────────────────────────────────

esp_err_t nvs_store_volume(int value)
{
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }
    esp_err_t ret = nvs_set_i32(nvs_h, "volume", (int32_t)value);
    ret |= nvs_commit(nvs_h);
    return ret;
}

int nvs_load_volume(int default_val)
{
    if (!nvs_initialized) return default_val;
    int32_t val = (int32_t)default_val;
    nvs_get_i32(nvs_h, "volume", &val);
    return (int)val;
}

// ── Speaker mute ──────────────────────────────────────────────────────────────

esp_err_t nvs_store_mute(bool mute)
{
    if (!nvs_initialized) { ESP_LOGW(TAG, "NVS not initialized"); return ESP_FAIL; }
    esp_err_t ret = nvs_set_i32(nvs_h, "mute", mute ? 1 : 0);
    ret |= nvs_commit(nvs_h);
    return ret;
}

bool nvs_load_mute(bool default_val)
{
    if (!nvs_initialized) return default_val;
    int32_t val = default_val ? 1 : 0;
    nvs_get_i32(nvs_h, "mute", &val);
    return val != 0;
}

// ── Display wedge self-restart flag ───────────────────────────────────────────
// Set just before the wedge detector's esp_restart(); consumed on next boot.
// Lives in NVS because nothing else survives every reset type on this board.

esp_err_t nvs_store_wedge_restart(bool flag)
{
    if (!nvs_initialized) return ESP_FAIL;
    esp_err_t ret = nvs_set_i32(nvs_h, "wedge_rst", flag ? 1 : 0);
    ret |= nvs_commit(nvs_h);
    return ret;
}

bool nvs_load_wedge_restart(void)
{
    if (!nvs_initialized) return false;
    int32_t val = 0;
    nvs_get_i32(nvs_h, "wedge_rst", &val);
    return val != 0;
}
