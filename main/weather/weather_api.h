#ifndef WEATHER_API_H
#define WEATHER_API_H

#include "weather_data.h"
#include "esp_err.h"
#include <stdbool.h>

#define WEATHER_HTTP_BUFFER_SIZE 32768
#define WEATHER_API_TIMEOUT_MS   10000

/**
 * Geocoding: convert ZIP code to lat/lon + city/state via zippopotam.us.
 */
esp_err_t weather_geocode_zipcode(const char *zip, location_t *location);

/**
 * Check if WiFi is available for API calls.
 */
bool weather_is_wifi_ready(void);

#endif // WEATHER_API_H
