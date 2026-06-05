#ifndef WEATHER_API_H
#define WEATHER_API_H

#include "weather_data.h"
#include "esp_err.h"
#include <stdbool.h>

/**
 * Maximum HTTP response buffer size (8KB)
 * Open-Meteo responses are typically 2-3KB
 */
#define WEATHER_HTTP_BUFFER_SIZE 32768

/**
 * API timeout in milliseconds
 */
#define WEATHER_API_TIMEOUT_MS 10000

/**
 * Geocoding API: Convert ZIP code to coordinates
 *
 * Uses Nominatim OpenStreetMap API (free, no key required)
 * Endpoint: https://nominatim.openstreetmap.org/search
 *
 * @param zip: ZIP code (e.g., "80202")
 * @param location: Pointer to location_t to fill with results
 * @return: ESP_OK if successful, ESP_FAIL on error
 */
esp_err_t weather_geocode_zipcode(const char *zip, location_t *location);

/**
 * Weather API: Fetch current and forecast weather
 *
 * Uses Open-Meteo API (free, no key required)
 * Endpoint: https://api.open-meteo.com/v1/forecast
 *
 * @param location: Pointer to location_t with latitude/longitude
 * @param weather: Pointer to weather_data_t to fill with results
 * @return: ESP_OK if successful, ESP_FAIL on error
 */
esp_err_t weather_fetch_current(const location_t *location, weather_data_t *weather);

/**
 * Combined geocoding + weather fetch
 * Geocodes the ZIP, then fetches weather for that location
 *
 * @param zip: ZIP code
 * @param weather: Pointer to weather_data_t to fill
 * @return: ESP_OK if both operations succeed, ESP_FAIL otherwise
 */
esp_err_t weather_update_from_zipcode(const char *zip, weather_data_t *weather);

/**
 * Check if WiFi is available for API calls
 * @return: true if WiFi connected, false otherwise
 */
bool weather_is_wifi_ready(void);

#endif // WEATHER_API_H
