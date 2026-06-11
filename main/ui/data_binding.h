#ifndef DATA_BINDING_H
#define DATA_BINDING_H

#include "weather_data.h"
#include "esp_err.h"
#include <time.h>

/**
 * UI Data Binding
 *
 * Maps weather_data_t to SquareLine UI widgets
 * All functions assume display lock is held by caller
 *
 * Widget mapping (from home.c):
 * - Current: temp, humidity, rain%, wind, status, icon
 * - 5-hour forecast: time, temp, icon (5x)
 * - 3-day forecast: day, high, low, status (3x)
 * - Network: WiFi icon
 */

/**
 * Update current weather section
 * Updates: CURRENTTEMPQ, CURRENTTIMEQ, LOCATIONQ, HUMIDITYPERCENTQ,
 *          CHANCEOFRAINQ, CURRENTSTATUSQ, 60X60ICONQ
 *
 * @param weather: Valid weather_data_t from weather API
 * @return: ESP_OK if successful
 */
esp_err_t ui_update_current_weather(const weather_data_t *weather);

/**
 * Update 5-hour hourly forecast
 * Updates: HRxFORCASTTIMEQ, HRxFORCASTTEMPQ, HRxFORCASICON22X22Q (for x=1..5)
 *
 * @param weather: Valid weather_data_t with hourly data filled
 * @return: ESP_OK if successful
 */
esp_err_t ui_update_hourly_forecast(const weather_data_t *weather);

/**
 * Update 3-day forecast
 * Updates: DAYLYxFORCASTDAYQ, DAYLYxFORCASTTEMPHIGHQ, DAYLYxFORCASTTEMPLOWQ,
 *          DAYLYxFORCASTSTATUSQ (for x=1..3)
 *
 * @param weather: Valid weather_data_t with daily data filled
 * @return: ESP_OK if successful
 */
esp_err_t ui_update_daily_forecast(const weather_data_t *weather);

/**
 * Update network/WiFi status icon
 * Updates: NETWORKCONNECTEDICONQ (green icon if connected, red if not)
 *
 * @param connected: true if WiFi connected, false otherwise
 * @return: ESP_OK if successful
 */
esp_err_t ui_update_network_status(bool connected);

/**
 * Refresh entire weather display
 * Calls all update functions in sequence
 *
 * @param weather: Valid weather_data_t
 * @param wifi_connected: WiFi connection status
 * @return: ESP_OK if all updates successful
 */
esp_err_t ui_refresh_weather_display(const weather_data_t *weather, bool wifi_connected);

/**
 * Set clean default placeholder values on all weather widgets
 * Call once after GUI_init() so display never shows blank/junk
 */
void ui_set_default_weather(void);

/**
 * Set CURRENTSTATUSQ with auto-fit font. Never splits a word across lines;
 * wraps at spaces; shrinks the font until the longest word fits one line and
 * the wrapped block fits the label area.
 */
void ui_set_status_text(const char *desc);

/**
 * Helper: Format temperature with degree symbol
 * @param temp: Temperature in Fahrenheit
 * @param buf: Output buffer
 * @param len: Buffer size
 * @return: Number of bytes written
 */
int format_temperature(float temp, char *buf, size_t len);

/**
 * Helper: Format time in 12-hour format with AM/PM
 * @param timestamp: Unix timestamp
 * @param buf: Output buffer
 * @param len: Buffer size
 * @return: Number of bytes written
 */
int format_time_12h(uint32_t timestamp, char *buf, size_t len);

/**
 * Helper: Format date as "Mon", "Tue", etc.
 * @param timestamp: Unix timestamp
 * @param buf: Output buffer
 * @param len: Buffer size
 * @return: Number of bytes written
 */
int format_day_name(uint32_t timestamp, char *buf, size_t len);

#endif // DATA_BINDING_H
