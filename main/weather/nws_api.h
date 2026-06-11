#ifndef NWS_API_H
#define NWS_API_H

#include "weather_data.h"
#include "esp_err.h"

/**
 * NWS grid / station resolved once per ZIP via /points + observationStations.
 * Cached in NVS; only re-fetched on ZIP change.
 */
typedef struct {
    char grid_id[8];   // WFO office, e.g. "VEF"
    int  grid_x;
    int  grid_y;
    char station[12];  // nearest obs station, e.g. "KVGT"
} nws_grid_t;

/**
 * One-time per ZIP: resolve lat/lon → WFO grid + nearest obs station.
 * Requires WiFi. Results should be cached to NVS.
 */
esp_err_t nws_resolve_grid(float lat, float lon, nws_grid_t *grid);

/**
 * Every 15 min: fetch current observations for the station.
 * Updates current_temp, humidity, wind, status_text, weather_code, is_day.
 * Preserves hourly/daily fields already in *weather.
 */
esp_err_t nws_fetch_observations(const nws_grid_t *grid, weather_data_t *weather);

/**
 * Every 60 min: fetch hourly forecast.
 * Parses only the first 6 periods (truncate-and-repair) to keep heap usage low.
 * Updates hourly.*, utc_offset_seconds, current_precip_prob.
 */
esp_err_t nws_fetch_hourly(const nws_grid_t *grid, weather_data_t *weather);

/**
 * Every 60 min: fetch daily forecast.
 * Skips today, fills daily[0..2] with the next 3 days.
 */
esp_err_t nws_fetch_daily(const nws_grid_t *grid, weather_data_t *weather);

#endif // NWS_API_H
