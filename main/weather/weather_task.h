#ifndef WEATHER_TASK_H
#define WEATHER_TASK_H

#include "weather_data.h"
#include "esp_err.h"
#include <stdbool.h>

/**
 * FreeRTOS background task for weather updates
 * - Fetches hourly (configurable); display steps through 15-min HRRR data in between
 * - Fetches from Open-Meteo API
 * - Updates display without blocking
 * - Handles offline scenarios gracefully
 */

/**
 * Start the weather background task
 * Must be called after WiFi is initialized
 * @return: ESP_OK if task created, ESP_FAIL otherwise
 */
esp_err_t weather_task_start(void);

/**
 * Stop the weather background task
 * @return: ESP_OK if task deleted, ESP_FAIL otherwise
 */
esp_err_t weather_task_stop(void);

/**
 * Force an immediate weather update
 * Returns without waiting for the update to complete
 * @return: ESP_OK if signal sent, ESP_FAIL otherwise
 */
esp_err_t weather_task_update_now(void);

/**
 * Get current weather data (thread-safe)
 * Copies data from task's internal buffer
 * @param out: Pointer to weather_data_t to fill
 * @return: ESP_OK if data valid, ESP_FAIL if no data available
 */
esp_err_t weather_get_data(weather_data_t *out);

/**
 * Set location for weather updates
 * Triggers immediate first update
 * @param zip: ZIP code (will be geocoded)
 * @return: ESP_OK if location set, ESP_FAIL otherwise
 */
esp_err_t weather_set_location(const char *zip);

/**
 * Set update interval in minutes (default: 15)
 * @param minutes: Update interval (minimum: 1, maximum: 1440)
 */
void weather_set_update_interval(uint32_t minutes);

/**
 * Get last update timestamp
 * @return: Unix timestamp of last successful update, 0 if never updated
 */
uint32_t weather_get_last_update(void);

/**
 * Returns true while the weather task is actively fetching from the API.
 * Use this to drive "LOADING WEATHER DATA" overlay on the home screen.
 */
bool weather_task_is_fetching(void);

#endif // WEATHER_TASK_H
