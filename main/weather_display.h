#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

/**
 * weather_display.h
 *
 * Weather data structure and GUI update functions.
 * Connects Open-Meteo API data to SquareLine UI widgets.
 *
 * Widget name corrections vs original plan doc:
 *   Plan "HUMIDITYQ"     → actual HUMIDITYPERCENTQ
 *   Plan "RAINPERCENT_Q" → actual CHANCEOFRAINQ
 *   Plan "WINDQ" (Label) → actual CURRENTWINDQ (Container — wind label is a child)
 *   Plan "HOURnTIMEQ"    → actual HRnFORCASTTIMEQ
 *   Plan "HOURnICONQ"    → actual HRnFORCASICON22X22Q  (HR2+ uses FORCASTICON)
 *   Plan "HOURnTEMPQ"    → actual HRnFORCASTTEMPQ
 *   Plan "TODAYHIGHQ/LOWQ" → NOT IN UI
 *   Hourly rain % labels  → NOT IN UI
 *   Extra: NETWORKCONNECTEDICONQ (green/red WiFi status icon)
 */

/* ── Weather data structure ─────────────────────────────────────────── */

typedef struct {
    /* Current conditions */
    float   current_temp_f;        // degrees Fahrenheit
    float   feels_like_f;
    int     current_humidity;      // 0-100 %
    int     current_precip_prob;   // chance of rain 0-100%
    float   current_wind_mph;
    char    current_wind_str[10];  // e.g. "9NW"
    int     current_wmo;           // WMO weather code

    /* Location */
    char    city[64];              // e.g. "Denver"
    char    state[32];             // e.g. "CO"

    /* 5-hour forecast (hours[0] = next hour) */
    float   hourly_temp_f[5];
    int     hourly_wmo[5];
    char    hourly_time[5][8];     // e.g. "3 PM"

    /* 3-day forecast (days[0] = today+1) */
    float   daily_high_f[3];
    float   daily_low_f[3];
    int     daily_wmo[3];
    char    daily_day[3][4];       // e.g. "Mon"
    char    daily_status[3][32];   // e.g. "Cloudy"

    /* Meta */
    bool    is_night;              // true after sunset
    bool    wifi_connected;
    char    time_str[10];          // e.g. "2:34 PM"
} weather_display_t;

/* ── Display update functions ───────────────────────────────────────── */

/**
 * Update all home screen widgets from weather data.
 * Must be called while holding bsp_display_lock.
 */
void weather_display_update(const weather_display_t *w);

/**
 * Update only the time label (call every minute).
 * Must be called while holding bsp_display_lock.
 */
void weather_display_update_time(const char *time_str);

/**
 * Update only the network status icon.
 * Must be called while holding bsp_display_lock.
 */
void weather_display_update_network(bool connected);
