#include "nws_api.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "esp_heap_caps.h"
#include "cJSON.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

static const char *TAG = "NWS";

#define NWS_TIMEOUT_MS   15000
#define NWS_SMALL_BUF    32768
#define NWS_LARGE_BUF    (256 * 1024)
#define NWS_USER_AGENT   "DeskMediaDevice/1.0 (mvavrick@gmail.com)"

/* ── HTTP layer ─────────────────────────────────────────────────────────── */

typedef struct {
    char  *buffer;
    size_t size;
    size_t capacity;
} nws_resp_t;

static esp_err_t nws_evt(esp_http_client_event_t *evt)
{
    nws_resp_t *r = evt->user_data;
    if (evt->event_id == HTTP_EVENT_ON_DATA) {
        if (r->size + evt->data_len >= r->capacity) {
            ESP_LOGW(TAG, "HTTP buffer overflow (%zu/%zu)", r->size, r->capacity);
            return ESP_FAIL;
        }
        memcpy(r->buffer + r->size, evt->data, evt->data_len);
        r->size += evt->data_len;
    }
    return ESP_OK;
}

// use_psram=true: allocate buffer from 32MB PSRAM (for the 60-70KB hourly response)
static esp_err_t nws_get(const char *url, nws_resp_t *r, bool use_psram)
{
    size_t cap = use_psram ? NWS_LARGE_BUF : NWS_SMALL_BUF;
    r->buffer = use_psram
        ? heap_caps_malloc(cap, MALLOC_CAP_SPIRAM)
        : malloc(cap);
    if (!r->buffer) {
        ESP_LOGE(TAG, "Failed to allocate %s buffer (%zu B)",
                 use_psram ? "PSRAM" : "heap", cap);
        return ESP_ERR_NO_MEM;
    }
    r->size     = 0;
    r->capacity = cap;

    esp_http_client_config_t cfg = {
        .url              = url,
        .timeout_ms       = NWS_TIMEOUT_MS,
        .user_data        = r,
        .event_handler    = nws_evt,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .buffer_size_tx   = 1024,
    };
    esp_http_client_handle_t client = esp_http_client_init(&cfg);
    if (!client) {
        free(r->buffer);
        r->buffer = NULL;
        return ESP_FAIL;
    }

    esp_http_client_set_header(client, "User-Agent", NWS_USER_AGENT);
    esp_http_client_set_header(client, "Accept", "application/geo+json");

    esp_err_t err = esp_http_client_perform(client);
    int status    = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);

    if (err != ESP_OK || status != 200) {
        ESP_LOGE(TAG, "HTTP failed: err=%s status=%d url=%s",
                 esp_err_to_name(err), status, url);
        free(r->buffer);
        r->buffer = NULL;
        return (err != ESP_OK) ? err : ESP_FAIL;
    }

    if (r->size < r->capacity) r->buffer[r->size] = '\0';
    ESP_LOGD(TAG, "GET %zu B from %s", r->size, url);
    return ESP_OK;
}

/* ── Icon URL → WMO code ────────────────────────────────────────────────── */

// Extracts the keyword from an NWS icon URL and maps it to a WMO code.
// e.g. "https://api.weather.gov/icons/land/day/tsra,80?size=medium" → 95, is_day=1
static int nws_icon_to_wmo(const char *icon_url, int *is_day_out)
{
    if (!icon_url) {
        if (is_day_out) *is_day_out = 1;
        return 0;
    }

    if (is_day_out)
        *is_day_out = (strstr(icon_url, "/day/") != NULL) ? 1 : 0;

    // Find the last '/' before '?' or end of string
    const char *q = strchr(icon_url, '?');
    size_t url_len = q ? (size_t)(q - icon_url) : strlen(icon_url);
    const char *last_slash = NULL;
    for (size_t i = 0; i < url_len; i++) {
        if (icon_url[i] == '/') last_slash = icon_url + i;
    }
    if (!last_slash) return 0;

    // Keyword: after last '/', before ',' or end
    const char *kw_start = last_slash + 1;
    size_t kw_len = 0;
    while (kw_start[kw_len] && kw_start[kw_len] != ',' && kw_start[kw_len] != '?')
        kw_len++;

    char kw[32] = {0};
    if (kw_len >= sizeof(kw)) kw_len = sizeof(kw) - 1;
    memcpy(kw, kw_start, kw_len);

    // Strip wind_ prefix
    const char *lk = (strncmp(kw, "wind_", 5) == 0) ? kw + 5 : kw;

    if (!strcmp(lk, "skc"))                              return 0;
    if (!strcmp(lk, "few"))                              return 1;
    if (!strcmp(lk, "sct"))                              return 2;
    if (!strcmp(lk, "bkn") || !strcmp(lk, "ovc"))       return 3;
    if (!strcmp(lk, "fog") || !strcmp(lk, "haze") ||
        !strcmp(lk, "smoke") || !strcmp(lk, "dust"))     return 45;
    if (!strcmp(lk, "rain_showers") ||
        !strcmp(lk, "rain_showers_hi"))                  return 80;
    if (!strcmp(lk, "rain"))                             return 61;
    if (!strcmp(lk, "tsra") || !strcmp(lk, "tsra_sct") ||
        !strcmp(lk, "tsra_hi"))                          return 95;
    if (!strcmp(lk, "snow") || !strcmp(lk, "rain_snow") ||
        !strcmp(lk, "snow_sleet"))                       return 71;
    if (!strcmp(lk, "sleet") || !strcmp(lk, "fzra") ||
        !strcmp(lk, "rain_fzra"))                        return 66;
    if (!strcmp(lk, "hot") || !strcmp(lk, "cold"))      return 0;
    if (!strcmp(lk, "blizzard"))                         return 73;
    if (!strcmp(lk, "tornado") || !strcmp(lk, "hurricane") ||
        !strcmp(lk, "tropical_storm"))                   return 95;

    ESP_LOGW(TAG, "Unknown NWS icon keyword: '%s'", kw);
    return 0;
}

/* ── Helpers ────────────────────────────────────────────────────────────── */

// Get numeric .value from an NWS observation property object.
// Returns false (sets *out=0) when the value is JSON null or missing.
static bool obs_value(cJSON *props, const char *key, double *out)
{
    *out = 0.0;
    cJSON *obj = cJSON_GetObjectItem(props, key);
    if (!obj) return false;
    cJSON *val = cJSON_GetObjectItem(obj, "value");
    if (!val || !cJSON_IsNumber(val)) return false;
    *out = val->valuedouble;
    return true;
}

// Parse the UTC offset from an NWS ISO8601 startTime string.
// e.g. "2026-06-11T06:00:00-07:00" → -25200
static int32_t parse_utc_offset(const char *st)
{
    int slen = (int)strlen(st);
    if (slen < 6) return 0;
    const char *tz = st + slen - 6;  // ±HH:MM
    int sign = (tz[0] == '-') ? -1 : 1;
    int h = (tz[1] - '0') * 10 + (tz[2] - '0');
    int m = (tz[4] - '0') * 10 + (tz[5] - '0');
    return sign * (h * 3600 + m * 60);
}

// Format hourly time label from NWS ISO8601 startTime.
// e.g. "2026-06-11T06:00:00-07:00" → "6AM"  (local time from the string itself)
static void format_nws_time(const char *st, char *buf, size_t len)
{
    const char *t = strchr(st, 'T');
    if (!t || strlen(t) < 3) { snprintf(buf, len, "--"); return; }
    t++;
    int hour = (t[0] - '0') * 10 + (t[1] - '0');
    const char *ap = (hour < 12) ? "AM" : "PM";
    int h12 = hour % 12;
    if (h12 == 0) h12 = 12;
    snprintf(buf, len, "%d%s", h12, ap);
}

// Truncate hourly JSON in-place: keep only the first `n` period objects.
// Cuts after the nth closing brace and appends "]}" to close the structure.
static esp_err_t truncate_periods(char *buf, size_t buf_cap, int n)
{
    char *arr = strstr(buf, "\"periods\"");
    if (!arr) { ESP_LOGE(TAG, "No periods array"); return ESP_FAIL; }
    char *bracket = strchr(arr, '[');
    if (!bracket) { ESP_LOGE(TAG, "No periods bracket"); return ESP_FAIL; }
    char *p = bracket + 1;

    int depth = 0, in_str = 0, escaped = 0, count = 0;
    char *last_close = NULL;

    while (*p) {
        if (escaped) {
            escaped = 0;
        } else if (in_str) {
            if (*p == '\\') escaped = 1;
            else if (*p == '"') in_str = 0;
        } else {
            if      (*p == '"') in_str = 1;
            else if (*p == '{') depth++;
            else if (*p == '}') {
                depth--;
                if (depth == 0) {
                    last_close = p;
                    if (++count >= n) break;
                }
            }
        }
        p++;
    }

    if (!last_close) { ESP_LOGE(TAG, "No period objects found"); return ESP_FAIL; }

    char *end = last_close + 1;
    // Suffix closes: periods[], properties{}, root{}
    const char suffix[] = "]}}";
    if ((size_t)(end - buf) + sizeof(suffix) >= buf_cap) {
        ESP_LOGE(TAG, "No room for truncation suffix");
        return ESP_FAIL;
    }
    memcpy(end, suffix, sizeof(suffix));  // includes the NUL terminator
    ESP_LOGI(TAG, "Hourly: truncated to %d periods (%zu B)", count, (size_t)(end + sizeof(suffix) - 1 - buf));
    return ESP_OK;
}

/* ── Public API ─────────────────────────────────────────────────────────── */

esp_err_t nws_resolve_grid(float lat, float lon, nws_grid_t *grid)
{
    memset(grid, 0, sizeof(*grid));

    // Step 1: /points → grid id, x, y, observationStations URL
    char url[128];
    snprintf(url, sizeof(url), "https://api.weather.gov/points/%.4f,%.4f", lat, lon);
    ESP_LOGI(TAG, "Resolving grid: %s", url);

    nws_resp_t r = {0};
    esp_err_t err = nws_get(url, &r, false);
    if (err != ESP_OK) return err;

    cJSON *root = cJSON_Parse(r.buffer);
    free(r.buffer);
    if (!root) { ESP_LOGE(TAG, "/points parse failed"); return ESP_FAIL; }

    cJSON *props = cJSON_GetObjectItem(root, "properties");
    if (!props) { cJSON_Delete(root); ESP_LOGE(TAG, "/points: no properties"); return ESP_FAIL; }

    cJSON *gid = cJSON_GetObjectItem(props, "gridId");
    cJSON *gx  = cJSON_GetObjectItem(props, "gridX");
    cJSON *gy  = cJSON_GetObjectItem(props, "gridY");
    cJSON *sta = cJSON_GetObjectItem(props, "observationStations");

    if (!gid || !gx || !gy || !sta || !cJSON_IsString(gid) || !cJSON_IsNumber(gx) ||
        !cJSON_IsNumber(gy) || !cJSON_IsString(sta)) {
        cJSON_Delete(root);
        ESP_LOGE(TAG, "/points: missing fields");
        return ESP_FAIL;
    }

    strncpy(grid->grid_id, gid->valuestring, sizeof(grid->grid_id) - 1);
    grid->grid_x = gx->valueint;
    grid->grid_y = gy->valueint;

    char stations_url[256];
    strncpy(stations_url, sta->valuestring, sizeof(stations_url) - 1);
    cJSON_Delete(root);

    ESP_LOGI(TAG, "Grid: %s/%d,%d  stations: %s",
             grid->grid_id, grid->grid_x, grid->grid_y, stations_url);

    // Step 2: observationStations URL → nearest station identifier (first in list)
    // Station list can exceed 32KB — use PSRAM buffer
    r = (nws_resp_t){0};
    err = nws_get(stations_url, &r, true);
    if (err != ESP_OK) return err;

    // Use strstr — first match is nearest station.
    // NWS formats as "stationIdentifier": "KVGT" (space after colon).
    const char *needle = "\"stationIdentifier\":";
    char *pos = strstr(r.buffer, needle);
    if (pos) {
        pos += strlen(needle);
        while (*pos == ' ' || *pos == '\t') pos++;  // skip whitespace
        if (*pos == '"') {
            pos++;  // skip opening quote
            char *end_q = strchr(pos, '"');
            if (end_q) {
                size_t slen = (size_t)(end_q - pos);
                if (slen < sizeof(grid->station)) {
                    memcpy(grid->station, pos, slen);
                    grid->station[slen] = '\0';
                }
            }
        }
    }
    free(r.buffer);

    if (!grid->station[0]) {
        ESP_LOGE(TAG, "Could not find station identifier");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Nearest station: %s", grid->station);
    return ESP_OK;
}

esp_err_t nws_fetch_observations(const nws_grid_t *grid, weather_data_t *weather)
{
    char url[128];
    snprintf(url, sizeof(url),
             "https://api.weather.gov/stations/%s/observations/latest",
             grid->station);

    nws_resp_t r = {0};
    esp_err_t err = nws_get(url, &r, false);
    if (err != ESP_OK) return err;

    cJSON *root = cJSON_Parse(r.buffer);
    free(r.buffer);
    if (!root) { ESP_LOGE(TAG, "obs: parse failed"); return ESP_FAIL; }

    cJSON *props = cJSON_GetObjectItem(root, "properties");
    if (!props) { cJSON_Delete(root); ESP_LOGE(TAG, "obs: no properties"); return ESP_FAIL; }

    double v;

    // Temperature (°C → °F); mandatory — fall back to previous value on null
    if (obs_value(props, "temperature", &v)) {
        weather->current_temp = (float)(v * 9.0 / 5.0 + 32.0);
    } else {
        ESP_LOGW(TAG, "obs: temp null — keeping previous %.1fF", weather->current_temp);
    }

    // Humidity
    if (obs_value(props, "relativeHumidity", &v))
        weather->current_humidity = (float)v;

    // Wind speed (km/h → mph)
    if (obs_value(props, "windSpeed", &v))
        weather->current_wind_speed = (float)(v / 1.609);

    // Wind direction (degrees)
    if (obs_value(props, "windDirection", &v))
        weather->current_wind_direction = (int)v;

    // Feels-like: heat index or wind chill (°C → °F); fall back to temp
    if (obs_value(props, "heatIndex", &v)) {
        weather->current_apparent_temp = (float)(v * 9.0 / 5.0 + 32.0);
    } else if (obs_value(props, "windChill", &v)) {
        weather->current_apparent_temp = (float)(v * 9.0 / 5.0 + 32.0);
    } else {
        weather->current_apparent_temp = weather->current_temp;
    }

    // Precipitation last hour (mm → inches)
    if (obs_value(props, "precipitationLastHour", &v))
        weather->current_precip = (float)(v / 25.4);
    else
        weather->current_precip = 0.0f;

    // Icon URL → WMO code + is_day
    cJSON *icon_j = cJSON_GetObjectItem(props, "icon");
    if (icon_j && cJSON_IsString(icon_j)) {
        weather->current_weather_code = nws_icon_to_wmo(icon_j->valuestring,
                                                         &weather->current_is_day);
    }

    // Status text
    cJSON *desc = cJSON_GetObjectItem(props, "textDescription");
    if (desc && cJSON_IsString(desc) && desc->valuestring[0]) {
        strncpy(weather->status_text, desc->valuestring, sizeof(weather->status_text) - 1);
        weather->status_text[sizeof(weather->status_text) - 1] = '\0';
    } else {
        weather->status_text[0] = '\0';
    }

    weather->current_time = (uint32_t)time(NULL);
    weather->last_update  = weather->current_time;
    weather->is_valid     = true;

    cJSON_Delete(root);
    ESP_LOGI(TAG, "Obs: %.1fF %d%% '%s'",
             weather->current_temp, (int)weather->current_humidity, weather->status_text);
    return ESP_OK;
}

esp_err_t nws_fetch_hourly(const nws_grid_t *grid, weather_data_t *weather)
{
    char url[128];
    snprintf(url, sizeof(url),
             "https://api.weather.gov/gridpoints/%s/%d,%d/forecast/hourly?units=us",
             grid->grid_id, grid->grid_x, grid->grid_y);

    // PSRAM buffer — response is 60-70KB
    nws_resp_t r = {0};
    esp_err_t err = nws_get(url, &r, true);
    if (err != ESP_OK) return err;

    // Truncate to first 6 periods before parsing to keep heap usage low
    if (truncate_periods(r.buffer, r.capacity, 6) != ESP_OK) {
        free(r.buffer);
        return ESP_FAIL;
    }

    cJSON *root = cJSON_Parse(r.buffer);
    free(r.buffer);
    if (!root) { ESP_LOGE(TAG, "hourly: parse failed"); return ESP_FAIL; }

    cJSON *props   = cJSON_GetObjectItem(root, "properties");
    cJSON *periods = props ? cJSON_GetObjectItem(props, "periods") : NULL;
    if (!periods || !cJSON_IsArray(periods)) {
        cJSON_Delete(root);
        ESP_LOGE(TAG, "hourly: no periods");
        return ESP_FAIL;
    }

    int total = cJSON_GetArraySize(periods);
    ESP_LOGI(TAG, "Hourly: %d periods after truncation", total);

    // period[0] = current hour: extract UTC offset + precip probability
    cJSON *p0 = cJSON_GetArrayItem(periods, 0);
    if (p0) {
        cJSON *st = cJSON_GetObjectItem(p0, "startTime");
        if (st && cJSON_IsString(st))
            weather->utc_offset_seconds = parse_utc_offset(st->valuestring);

        cJSON *pop = cJSON_GetObjectItem(p0, "probabilityOfPrecipitation");
        cJSON *pop_val = pop ? cJSON_GetObjectItem(pop, "value") : NULL;
        if (pop_val && cJSON_IsNumber(pop_val))
            weather->current_precip_prob = pop_val->valueint;
        else
            weather->current_precip_prob = 0;
    }

    // periods[1..4] → hourly display slots [0..3]
    int filled = 0;
    for (int i = 1; i <= 4 && i < total; i++) {
        cJSON *p = cJSON_GetArrayItem(periods, i);
        if (!p) continue;

        cJSON *temp_j = cJSON_GetObjectItem(p, "temperature");
        cJSON *icon_j = cJSON_GetObjectItem(p, "icon");
        cJSON *st_j   = cJSON_GetObjectItem(p, "startTime");

        int slot = i - 1;
        weather->hourly.hours[slot].temperature   =
            (temp_j && cJSON_IsNumber(temp_j)) ? (float)temp_j->valuedouble : 0.0f;
        weather->hourly.hours[slot].weather_code  =
            (icon_j && cJSON_IsString(icon_j)) ? nws_icon_to_wmo(icon_j->valuestring, NULL) : 0;
        weather->hourly.hours[slot].timestamp     = (uint32_t)time(NULL) + (uint32_t)(i * 3600);

        if (st_j && cJSON_IsString(st_j))
            format_nws_time(st_j->valuestring,
                            weather->hourly.times_12h[slot],
                            sizeof(weather->hourly.times_12h[slot]));
        else
            snprintf(weather->hourly.times_12h[slot],
                     sizeof(weather->hourly.times_12h[slot]), "--");
        filled++;
    }

    cJSON_Delete(root);
    ESP_LOGI(TAG, "Hourly: filled %d slots, UTC offset %lds",
             filled, (long)weather->utc_offset_seconds);
    return ESP_OK;
}

esp_err_t nws_fetch_daily(const nws_grid_t *grid, weather_data_t *weather)
{
    char url[128];
    snprintf(url, sizeof(url),
             "https://api.weather.gov/gridpoints/%s/%d,%d/forecast?units=us",
             grid->grid_id, grid->grid_x, grid->grid_y);

    // Daily response can exceed 32KB — use PSRAM buffer
    nws_resp_t r = {0};
    esp_err_t err = nws_get(url, &r, true);
    if (err != ESP_OK) return err;

    cJSON *root = cJSON_Parse(r.buffer);
    free(r.buffer);
    if (!root) { ESP_LOGE(TAG, "daily: parse failed"); return ESP_FAIL; }

    cJSON *props   = cJSON_GetObjectItem(root, "properties");
    cJSON *periods = props ? cJSON_GetObjectItem(props, "periods") : NULL;
    if (!periods || !cJSON_IsArray(periods)) {
        cJSON_Delete(root);
        ESP_LOGE(TAG, "daily: no periods");
        return ESP_FAIL;
    }

    int total     = cJSON_GetArraySize(periods);
    int day_count = 0;
    bool skipped  = false;  // have we passed today's period(s)?

    for (int i = 0; i < total && day_count < 3; i++) {
        cJSON *p    = cJSON_GetArrayItem(periods, i);
        cJSON *name = cJSON_GetObjectItem(p, "name");
        cJSON *isdt = cJSON_GetObjectItem(p, "isDaytime");
        if (!name || !isdt) continue;

        const char *nm  = name->valuestring;
        int is_daytime  = cJSON_IsTrue(isdt) ? 1 : 0;

        // Skip "Today", "This Morning/Afternoon", "Tonight" at start
        bool is_today = (strncmp(nm, "Today",   5) == 0 ||
                         strncmp(nm, "This ",   5) == 0 ||
                         strncmp(nm, "Tonight", 7) == 0);
        if (!skipped && is_today) continue;
        skipped = true;

        if (!is_daytime) continue;  // only use day periods for high/low pairing

        cJSON *temp_j = cJSON_GetObjectItem(p, "temperature");
        cJSON *icon_j = cJSON_GetObjectItem(p, "icon");

        weather->daily[day_count].temp_high    =
            (temp_j && cJSON_IsNumber(temp_j)) ? (float)temp_j->valuedouble : 0.0f;
        weather->daily[day_count].weather_code =
            (icon_j && cJSON_IsString(icon_j)) ? nws_icon_to_wmo(icon_j->valuestring, NULL) : 0;

        // Day name: first 3 chars of period name (e.g. "Wed" from "Wednesday")
        strncpy(weather->daily[day_count].day_name, nm, 3);
        weather->daily[day_count].day_name[3] = '\0';

        // Low from the immediately following night period
        if (i + 1 < total) {
            cJSON *night     = cJSON_GetArrayItem(periods, i + 1);
            cJSON *night_tmp = night ? cJSON_GetObjectItem(night, "temperature") : NULL;
            weather->daily[day_count].temp_low =
                (night_tmp && cJSON_IsNumber(night_tmp)) ? (float)night_tmp->valuedouble : weather->daily[day_count].temp_high;
        } else {
            weather->daily[day_count].temp_low = weather->daily[day_count].temp_high;
        }

        ESP_LOGI(TAG, "Daily[%d]: %s H=%.0fF L=%.0fF",
                 day_count, weather->daily[day_count].day_name,
                 weather->daily[day_count].temp_high, weather->daily[day_count].temp_low);
        day_count++;
    }

    cJSON_Delete(root);
    if (day_count < 3)
        ESP_LOGW(TAG, "Only filled %d of 3 daily slots", day_count);
    return ESP_OK;
}
