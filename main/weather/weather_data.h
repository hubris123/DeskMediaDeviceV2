#ifndef WEATHER_DATA_H
#define WEATHER_DATA_H

#include <time.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char city[50];
    char state[3];
    float latitude;
    float longitude;
    uint32_t timestamp;
} location_t;

typedef struct {
    float temperature;
    int weather_code;
    int humidity;
    int precipitation_prob;
    uint32_t timestamp;
} weather_point_t;

typedef struct {
    weather_point_t hours[5];
    char times_12h[5][10];
} hourly_forecast_t;

typedef struct {
    float temp_high;
    float temp_low;
    int weather_code;
    int precipitation_prob;
    char date_str[12];
    char day_name[10];
} daily_forecast_t;

/**
 * Complete weather dataset.
 * Current conditions populated by NWS observations every 15 min.
 * Hourly/daily populated by NWS forecast every 60 min.
 */
typedef struct {
    // Current conditions
    float current_temp;
    float current_apparent_temp;
    float current_humidity;
    float current_wind_speed;
    int   current_wind_direction;
    int   current_weather_code;
    int   current_precip_prob;
    int   current_is_day;
    float current_precip;
    uint32_t current_time;
    char  status_text[64];      // NWS textDescription, e.g. "Partly Cloudy"

    // Location info
    char city[50];
    char state[3];
    float latitude;
    float longitude;
    char timezone[50];

    // Hourly forecast (next 4 hours, periods[1..4] from NWS hourly)
    hourly_forecast_t hourly;

    // Daily forecast (next 3 days)
    daily_forecast_t daily[3];

    // UTC offset derived from NWS hourly startTime (feeds the clock)
    int32_t utc_offset_seconds;

    // Metadata
    uint32_t last_update;
    bool is_valid;
} weather_data_t;

void weather_data_init(weather_data_t *data);
bool weather_data_is_current(const weather_data_t *data, uint32_t max_age_seconds);

#endif // WEATHER_DATA_H
