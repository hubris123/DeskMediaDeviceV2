#ifndef WEATHER_TASK_H
#define WEATHER_TASK_H

#include "weather_data.h"
#include "esp_err.h"
#include <stdbool.h>

/**
 * FreeRTOS background task for weather updates.
 * Fetches NWS observations every 15 min; hourly + daily forecast every 60 min.
 */

esp_err_t weather_task_start(void);
esp_err_t weather_task_stop(void);
esp_err_t weather_task_update_now(void);

/**
 * Get current weather data (thread-safe copy).
 * Returns ESP_FAIL if no valid data is available yet.
 */
esp_err_t weather_get_data(weather_data_t *out);

/**
 * Queue a ZIP code change. Geocodes and resolves NWS grid, then fetches.
 */
esp_err_t weather_set_location(const char *zip);

uint32_t  weather_get_last_update(void);
bool      weather_task_is_fetching(void);

#endif // WEATHER_TASK_H
