#include "weather_api.h"
#include "wifi_manager.h"
#include "esp_crt_bundle.h"
#include "wmo_icon_map.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "esp_log.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>

static const char *TAG = "WeatherAPI";

static const char *degrees_to_compass(int deg)
{
    const char *dirs[] = {"N","NE","E","SE","S","SW","W","NW"};
    return dirs[((deg + 22) % 360) / 45];
}

/**
 * HTTP response buffer for API calls
 */
typedef struct {
    char *buffer;
    size_t size;
    size_t capacity;
} http_response_t;

/**
 * Callback for HTTP response body
 */
static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    http_response_t *resp = (http_response_t *)evt->user_data;

    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            if (resp->size + evt->data_len >= resp->capacity) {
                ESP_LOGW(TAG, "HTTP response buffer overflow");
                return ESP_FAIL;
            }
            memcpy(resp->buffer + resp->size, evt->data, evt->data_len);
            resp->size += evt->data_len;
            break;

        case HTTP_EVENT_ERROR:
            ESP_LOGE(TAG, "HTTP error");
            return ESP_FAIL;

        default:
            break;
    }
    return ESP_OK;
}

/**
 * Make HTTP GET request and return response body
 * @param url: Full URL to fetch
 * @param response: Pointer to http_response_t to fill
 * @return: ESP_OK if successful
 */
static esp_err_t http_get(const char *url, http_response_t *response)
{
    response->buffer = malloc(WEATHER_HTTP_BUFFER_SIZE);
    if (!response->buffer) {
        ESP_LOGE(TAG, "Failed to allocate HTTP buffer");
        return ESP_ERR_NO_MEM;
    }
    response->size = 0;
    response->capacity = WEATHER_HTTP_BUFFER_SIZE;

    esp_http_client_config_t config = {
        .url = url,
        .timeout_ms = WEATHER_API_TIMEOUT_MS,
        .user_data = response,
        .event_handler = http_event_handler,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) {
        ESP_LOGE(TAG, "Failed to init HTTP client");
        free(response->buffer);
        return ESP_FAIL;
    }

    esp_http_client_set_header(client, "User-Agent", "DeskMediaDevice/1.0 (weather display)");
    esp_http_client_set_header(client, "Accept", "application/json");

    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        free(response->buffer);
        return err;
    }

    int status_code = esp_http_client_get_status_code(client);
    if (status_code != 200) {
        ESP_LOGE(TAG, "HTTP %d", status_code);
        esp_http_client_cleanup(client);
        free(response->buffer);
        return ESP_FAIL;
    }

    // Null-terminate response
    if (response->size < response->capacity) {
        response->buffer[response->size] = '\0';
    }

    esp_http_client_cleanup(client);
    ESP_LOGD(TAG, "HTTP GET success: %zu bytes", response->size);
    return ESP_OK;
}

/**
 * Parse Nominatim geocoding response
 */
static esp_err_t parse_geocoding_response(const char *json_str, location_t *loc)
{
    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        ESP_LOGE(TAG, "JSON parse error (geocoding)");
        return ESP_FAIL;
    }

    // Zippopotam.us format: {"places": [{"place name":..., "state abbreviation":..., "latitude":..., "longitude":...}]}
    cJSON *array = cJSON_GetObjectItem(root, "places");
    if (!cJSON_IsArray(array) || cJSON_GetArraySize(array) == 0) {
        ESP_LOGE(TAG, "No results from geocoding API");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *place_name = cJSON_GetObjectItem(first, "place name");
    cJSON *state      = cJSON_GetObjectItem(first, "state abbreviation");
    cJSON *lat        = cJSON_GetObjectItem(first, "latitude");
    cJSON *lon        = cJSON_GetObjectItem(first, "longitude");

    if (!place_name || !state || !lat || !lon) {
        ESP_LOGE(TAG, "Missing fields in geocoding response");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    strncpy(loc->city, place_name->valuestring, sizeof(loc->city) - 1);
    loc->city[sizeof(loc->city) - 1] = '\0';

    strncpy(loc->state, state->valuestring, sizeof(loc->state) - 1);
    loc->state[sizeof(loc->state) - 1] = '\0';

    // Zippopotam returns lat/lon as strings
    loc->latitude  = (float)atof(lat->valuestring);
    loc->longitude = (float)atof(lon->valuestring);
    loc->timestamp = (uint32_t)time(NULL);

    cJSON_Delete(root);
    ESP_LOGI(TAG, "Geocoded: %s, %s (%.4f, %.4f)", loc->city, loc->state,
             loc->latitude, loc->longitude);
    return ESP_OK;
}

/**
 * Parse Open-Meteo weather response
 */
static esp_err_t parse_weather_response(const char *json_str, const location_t *loc,
                                       weather_data_t *weather)
{
    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        ESP_LOGE(TAG, "JSON parse error (weather)");
        return ESP_FAIL;
    }

    // Current weather
    cJSON *current = cJSON_GetObjectItem(root, "current");
    if (!current) {
        ESP_LOGE(TAG, "Missing 'current' in weather response");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    cJSON *temp          = cJSON_GetObjectItem(current, "temperature_2m");
    cJSON *apparent_temp = cJSON_GetObjectItem(current, "apparent_temperature");
    cJSON *humidity      = cJSON_GetObjectItem(current, "relative_humidity_2m");
    cJSON *wmo_code      = cJSON_GetObjectItem(current, "weather_code");
    cJSON *wind_speed    = cJSON_GetObjectItem(current, "wind_speed_10m");
    cJSON *wind_dir      = cJSON_GetObjectItem(current, "wind_direction_10m");
    cJSON *is_day        = cJSON_GetObjectItem(current, "is_day");
    cJSON *precip        = cJSON_GetObjectItem(current, "precipitation");
    cJSON *rain          = cJSON_GetObjectItem(current, "rain");
    cJSON *showers       = cJSON_GetObjectItem(current, "showers");
    cJSON *snowfall      = cJSON_GetObjectItem(current, "snowfall");
    cJSON *current_time  = cJSON_GetObjectItem(current, "time");

    if (!temp || !wmo_code) {
        ESP_LOGE(TAG, "Missing temperature or weather_code");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    weather->current_temp          = temp->valuedouble;
    weather->current_apparent_temp = apparent_temp ? apparent_temp->valuedouble : temp->valuedouble;
    weather->current_humidity      = humidity ? (float)humidity->valueint : 0;
    weather->current_wind_speed     = wind_speed ? wind_speed->valuedouble : 0;
    weather->current_wind_direction = wind_dir   ? wind_dir->valueint     : 0;
    weather->current_weather_code  = wmo_code->valueint;
    weather->current_precip_prob   = 0;
    // Sum all precipitation types for current total
    weather->current_precip = (precip ? precip->valuedouble : 0.0f)
                            + (rain    ? rain->valuedouble    : 0.0f)
                            + (showers ? showers->valuedouble : 0.0f)
                            + (snowfall? snowfall->valuedouble: 0.0f);
    weather->current_is_day        = is_day ? is_day->valueint : 1;
    weather->current_time          = current_time ? (uint32_t)current_time->valueint : (uint32_t)time(NULL);

    // Location data
    strncpy(weather->city, loc->city, sizeof(weather->city) - 1);
    weather->city[sizeof(weather->city) - 1] = '\0';
    strncpy(weather->state, loc->state, sizeof(weather->state) - 1);
    weather->state[sizeof(weather->state) - 1] = '\0';
    weather->latitude = loc->latitude;
    weather->longitude = loc->longitude;

    // Get timezone from response
    cJSON *tz = cJSON_GetObjectItem(root, "timezone");
    if (tz) {
        strncpy(weather->timezone, tz->valuestring, sizeof(weather->timezone) - 1);
        weather->timezone[sizeof(weather->timezone) - 1] = '\0';
    }

    // Parse UTC offset from timezone=auto — needed below for local time formatting
    cJSON *utc_offset = cJSON_GetObjectItem(root, "utc_offset_seconds");
    weather->utc_offset_seconds = utc_offset ? utc_offset->valueint : -21600; // default MDT

    // Hourly forecast (next 5 hours)
    cJSON *hourly = cJSON_GetObjectItem(root, "hourly");
    if (hourly) {
        cJSON *hourly_times  = cJSON_GetObjectItem(hourly, "time");
        cJSON *hourly_temps  = cJSON_GetObjectItem(hourly, "temperature_2m");
        cJSON *hourly_codes  = cJSON_GetObjectItem(hourly, "weather_code");
        cJSON *hourly_precip_prob = cJSON_GetObjectItem(hourly, "precipitation_probability");

        // Find the next hour's precipitation probability
        if (hourly_times && hourly_precip_prob) {
            time_t now = time(NULL);
            int total = cJSON_GetArraySize(hourly_times);
            for (int i = 0; i < total; i++) {
                cJSON *t = cJSON_GetArrayItem(hourly_times, i);
                cJSON *p = cJSON_GetArrayItem(hourly_precip_prob, i);
                if (t && p && (time_t)t->valueint >= now) {
                    weather->current_precip_prob = p->valueint;
                    ESP_LOGI(TAG, "Precip prob for next hour: %d%%", weather->current_precip_prob);
                    break;
                }
            }
        }

        if (hourly_times && hourly_temps && hourly_codes) {
            int num_hours = cJSON_GetArraySize(hourly_times);
            if (num_hours > 5) num_hours = 5;

            for (int i = 0; i < num_hours; i++) {
                cJSON *time_item = cJSON_GetArrayItem(hourly_times, i);
                cJSON *temp_item = cJSON_GetArrayItem(hourly_temps, i);
                cJSON *code_item = cJSON_GetArrayItem(hourly_codes, i);

                if (time_item && temp_item && code_item) {
                    // timeformat=unixtime so times are integers
                    time_t t = (time_t)time_item->valueint;

                    weather->hourly.hours[i].timestamp = (uint32_t)t;
                    weather->hourly.hours[i].temperature = temp_item->valuedouble;
                    weather->hourly.hours[i].weather_code = code_item->valueint;

                    // Format time as "3 PM", "4 PM", etc.
                    // Apply UTC offset from the API (location's local time)
                    time_t t_local = t + weather->utc_offset_seconds;
                    struct tm *local_tm = gmtime(&t_local);
                    if (local_tm) {
                        char tmp[10] = {0};
                        strftime(tmp, sizeof(tmp), "%I%p", local_tm);
                        // Strip leading zero: "04PM" -> "4PM"
                        const char *tp = (tmp[0] == '0') ? tmp + 1 : tmp;
                        size_t tp_len = strlen(tp);
                        size_t copy_len = tp_len < sizeof(weather->hourly.times_12h[i]) - 1
                                        ? tp_len : sizeof(weather->hourly.times_12h[i]) - 1;
                        memcpy(weather->hourly.times_12h[i], tp, copy_len);
                        weather->hourly.times_12h[i][copy_len] = '\0';
                    } else {
                        snprintf(weather->hourly.times_12h[i],
                                sizeof(weather->hourly.times_12h[i]), "--");
                    }
                }
            }
        }
    }

    // Daily forecast (next 3 days)
    cJSON *daily = cJSON_GetObjectItem(root, "daily");
    if (daily) {
        cJSON *daily_times  = cJSON_GetObjectItem(daily, "time");
        cJSON *daily_highs  = cJSON_GetObjectItem(daily, "temperature_2m_max");
        cJSON *daily_lows   = cJSON_GetObjectItem(daily, "temperature_2m_min");
        cJSON *daily_codes  = cJSON_GetObjectItem(daily, "weather_code");
        cJSON *daily_precip = cJSON_GetObjectItem(daily, "precipitation_sum");

        if (daily_times && daily_highs && daily_lows && daily_codes) {
            int num_days = cJSON_GetArraySize(daily_times);
            if (num_days > 3) num_days = 3;

            // Use today's daily precipitation sum for the display (index 0)
            if (daily_precip) {
                cJSON *today_precip = cJSON_GetArrayItem(daily_precip, 0);
                if (today_precip) {
                    weather->current_precip = today_precip->valuedouble;
                }
            }

            for (int i = 0; i < num_days; i++) {
                cJSON *time_item = cJSON_GetArrayItem(daily_times, i);
                cJSON *high_item = cJSON_GetArrayItem(daily_highs, i);
                cJSON *low_item  = cJSON_GetArrayItem(daily_lows, i);
                cJSON *code_item = cJSON_GetArrayItem(daily_codes, i);

                if (time_item && high_item && low_item && code_item) {
                    // timeformat=unixtime — times are integers
                    time_t t = (time_t)time_item->valueint;
                    snprintf(weather->daily[i].date_str,
                            sizeof(weather->daily[i].date_str), "%lu", (unsigned long)t);

                    weather->daily[i].temp_high = high_item->valuedouble;
                    weather->daily[i].temp_low = low_item->valuedouble;
                    weather->daily[i].weather_code = code_item->valueint;

                    // Format day name from unix timestamp at the location's local time
                    // (device clock is UTC — localtime() would be wrong near midnight)
                    time_t t_local = t + weather->utc_offset_seconds;
                    struct tm *local_tm = gmtime(&t_local);
                    if (local_tm) {
                        strftime(weather->daily[i].day_name,
                                sizeof(weather->daily[i].day_name),
                                "%a", local_tm);  // "Mon", "Tue", etc.
                    } else {
                        snprintf(weather->daily[i].day_name,
                                sizeof(weather->daily[i].day_name), "---");
                    }
                }
            }
        }
    }

    // 15-minutely steps (HRRR) — walked through locally between hourly fetches
    weather->minutely_count = 0;
    cJSON *m15 = cJSON_GetObjectItem(root, "minutely_15");
    if (m15) {
        cJSON *m_times = cJSON_GetObjectItem(m15, "time");
        cJSON *m_temp  = cJSON_GetObjectItem(m15, "temperature_2m");
        cJSON *m_app   = cJSON_GetObjectItem(m15, "apparent_temperature");
        cJSON *m_hum   = cJSON_GetObjectItem(m15, "relative_humidity_2m");
        cJSON *m_code  = cJSON_GetObjectItem(m15, "weather_code");
        cJSON *m_isday = cJSON_GetObjectItem(m15, "is_day");
        cJSON *m_wspd  = cJSON_GetObjectItem(m15, "wind_speed_10m");
        cJSON *m_wdir  = cJSON_GetObjectItem(m15, "wind_direction_10m");

        if (m_times && m_temp && m_code) {
            int n = cJSON_GetArraySize(m_times);
            if (n > MINUTELY_15_STEPS) n = MINUTELY_15_STEPS;
            for (int i = 0; i < n; i++) {
                cJSON *t = cJSON_GetArrayItem(m_times, i);
                cJSON *v = cJSON_GetArrayItem(m_temp,  i);
                cJSON *c = cJSON_GetArrayItem(m_code,  i);
                // null values = outside HRRR coverage — skip, fall back to current block
                if (!t || !v || !c || !cJSON_IsNumber(v) || !cJSON_IsNumber(c)) continue;

                minutely_step_t *st = &weather->minutely[weather->minutely_count];
                st->timestamp    = (uint32_t)t->valueint;
                st->temperature  = v->valuedouble;
                st->weather_code = c->valueint;

                cJSON *x;
                x = m_app   ? cJSON_GetArrayItem(m_app, i)   : NULL;
                st->apparent_temp  = (x && cJSON_IsNumber(x)) ? x->valuedouble : v->valuedouble;
                x = m_hum   ? cJSON_GetArrayItem(m_hum, i)   : NULL;
                st->humidity       = (x && cJSON_IsNumber(x)) ? (float)x->valuedouble : weather->current_humidity;
                x = m_isday ? cJSON_GetArrayItem(m_isday, i) : NULL;
                st->is_day         = (x && cJSON_IsNumber(x)) ? x->valueint : weather->current_is_day;
                x = m_wspd  ? cJSON_GetArrayItem(m_wspd, i)  : NULL;
                st->wind_speed     = (x && cJSON_IsNumber(x)) ? x->valuedouble : weather->current_wind_speed;
                x = m_wdir  ? cJSON_GetArrayItem(m_wdir, i)  : NULL;
                st->wind_direction = (x && cJSON_IsNumber(x)) ? x->valueint : weather->current_wind_direction;

                weather->minutely_count++;
            }
            ESP_LOGI(TAG, "Parsed %d 15-minutely steps", weather->minutely_count);
        }
    }

    weather->is_valid = true;
    weather->last_update = (uint32_t)time(NULL);

    cJSON_Delete(root);
    ESP_LOGI(TAG, "Weather parsed: %.1f°F, %d%%", weather->current_temp,
            weather->current_humidity);
    return ESP_OK;
}

// ============ Public API ============

esp_err_t weather_geocode_zipcode(const char *zip, location_t *location)
{
    if (!zip || !location) {
        return ESP_ERR_INVALID_ARG;
    }

    // Zippopotam.us — free, plain HTTP, returns city/state/lat/lon
    char url[512];
    snprintf(url, sizeof(url), "http://api.zippopotam.us/us/%s", zip);

    ESP_LOGI(TAG, "Geocoding ZIP: %s", url);

    // Use a separate HTTP client with no SSL for Zippopotam
    http_response_t response = {0};
    response.buffer = malloc(WEATHER_HTTP_BUFFER_SIZE);
    if (!response.buffer) return ESP_ERR_NO_MEM;
    response.size = 0;
    response.capacity = WEATHER_HTTP_BUFFER_SIZE;

    esp_http_client_config_t geo_config = {
        .url = url,
        .timeout_ms = WEATHER_API_TIMEOUT_MS,
        .user_data = &response,
        .event_handler = http_event_handler,
        .transport_type = HTTP_TRANSPORT_OVER_TCP,
        .skip_cert_common_name_check = false,
    };
    esp_http_client_handle_t geo_client = esp_http_client_init(&geo_config);
    if (!geo_client) { free(response.buffer); return ESP_FAIL; }

    esp_http_client_set_header(geo_client, "User-Agent", "DeskMediaDevice/1.0");
    esp_http_client_set_header(geo_client, "Accept", "application/json");

    esp_err_t err = esp_http_client_perform(geo_client);
    int status = esp_http_client_get_status_code(geo_client);
    esp_http_client_cleanup(geo_client);

    if (err != ESP_OK || status != 200 || response.size == 0) {
        ESP_LOGE(TAG, "Geocode HTTP failed: err=%s, status=%d, size=%u",
                 esp_err_to_name(err), status, (unsigned)response.size);
        free(response.buffer);
        return (err != ESP_OK) ? err : ESP_FAIL;
    }
    // Null-terminate before JSON parse (size < capacity guaranteed by event handler)
    response.buffer[response.size] = '\0';

    err = parse_geocoding_response(response.buffer, location);
    free(response.buffer);
    return err;
}

esp_err_t weather_fetch_current(const location_t *location, weather_data_t *weather)
{
    if (!location || !weather) {
        return ESP_ERR_INVALID_ARG;
    }

    // Build URL with Open-Meteo parameters
    char url[1024];
    snprintf(url, sizeof(url),
             "https://api.open-meteo.com/v1/forecast?"
             "latitude=%.4f&longitude=%.4f"
             "&current=temperature_2m,relative_humidity_2m,apparent_temperature,weather_code,wind_speed_10m,wind_direction_10m,is_day"
             "&hourly=temperature_2m,weather_code,precipitation_probability"
             "&daily=weather_code,temperature_2m_max,temperature_2m_min"
             "&models=gfs_hrrr"  // NCEP HRRR — US CONUS model
             "&minutely_15=temperature_2m,relative_humidity_2m,apparent_temperature,weather_code,is_day,wind_speed_10m,wind_direction_10m"
             "&forecast_minutely_15=8"  // next 2h of 15-min steps — display walks these between hourly fetches
             "&timeformat=unixtime"
             "&temperature_unit=fahrenheit"
             "&wind_speed_unit=mph"
             "&precipitation_unit=inch"
             "&forecast_days=3"
             "&forecast_hours=6"
             "&timezone=auto",
             location->latitude, location->longitude);

    ESP_LOGI(TAG, "Fetching weather for (%.4f, %.4f)", location->latitude, location->longitude);

    http_response_t response = {0};
    esp_err_t err = http_get(url, &response);
    if (err != ESP_OK) {
        return err;
    }

    err = parse_weather_response(response.buffer, location, weather);
    free(response.buffer);
    return err;
}

esp_err_t weather_update_from_zipcode(const char *zip, weather_data_t *weather)
{
    location_t location = {0};

    esp_err_t err = weather_geocode_zipcode(zip, &location);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Geocoding failed for %s", zip);
        return err;
    }

    err = weather_fetch_current(&location, weather);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Weather fetch failed");
        return err;
    }

    return ESP_OK;
}

bool weather_is_wifi_ready(void)
{
    return wifi_manager_is_connected();
}
