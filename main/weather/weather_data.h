#ifndef WEATHER_DATA_H
#define WEATHER_DATA_H

#include <time.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Location information
 */
typedef struct {
    char city[50];
    char state[3];
    float latitude;
    float longitude;
    uint32_t timestamp;  // When location was set
} location_t;

/**
 * Single point-in-time weather observation
 */
typedef struct {
    float temperature;
    int weather_code;        // WMO code
    int humidity;
    int precipitation_prob;
    uint32_t timestamp;
} weather_point_t;

/**
 * Hourly forecast data (5 hours)
 */
typedef struct {
    weather_point_t hours[5];
    char times_12h[5][10];   // "3 PM", "4 PM", etc.
} hourly_forecast_t;

/**
 * Daily forecast data (single day)
 */
typedef struct {
    float temp_high;
    float temp_low;
    int weather_code;
    int precipitation_prob;
    char date_str[12];       // "YYYY-MM-DD"
    char day_name[10];       // "Monday", etc.
} daily_forecast_t;

/**
 * Complete weather dataset
 * Populated by weather API, consumed by UI display functions
 */
typedef struct {
    // Current conditions
    float current_temp;
    float current_humidity;
    int current_weather_code;
    int current_precip_prob;
    uint32_t current_time;

    // Location info
    char city[50];
    char state[3];
    float latitude;
    float longitude;
    char timezone[50];

    // Hourly forecast (next 5 hours)
    hourly_forecast_t hourly;

    // Daily forecast (next 3 days)
    daily_forecast_t daily[3];

    // Metadata
    uint32_t last_update;    // Unix timestamp when data was fetched
    bool is_valid;           // Data validity flag
} weather_data_t;

/**
 * Initialize empty weather data structure
 */
void weather_data_init(weather_data_t *data);

/**
 * Validate weather data before display
 */
bool weather_data_is_current(const weather_data_t *data, uint32_t max_age_seconds);

#endif // WEATHER_DATA_H
