#ifndef WMO_ICON_MAP_H
#define WMO_ICON_MAP_H

#include <stdint.h>
#include <stdbool.h>

/**
 * WMO Code to AccuWeather Icon Mapping
 *
 * Embedded icons (from SquareLine UI):
 * Day icons:   1, 2, 3, 7, 11, 12, 14, 15, 18, 19, 22, 26, 33, 34, 35
 * Night icons: All above plus alternatives like 34, 35, 36, 39, 40, 41, 42, 43, 44
 *
 * WMO codes returned by Open-Meteo: 0,1,2,3,45,48,51-57,61-67,71-77,80-82,85-86,95-99
 */

typedef struct {
    int wmo_code;
    int accuweather_icon_day;
    int accuweather_icon_night;
    const char *description;
} wmo_icon_map_t;

/**
 * Complete WMO to AccuWeather mapping table
 * Covers all 28 WMO codes returned by Open-Meteo
 *
 * Strategy for missing embedded icons:
 * - Icons 6, 8, 39, 40, 41, 42, 43, 44 are NOT embedded in SquareLine
 * - For these, fallback to closest available icon:
 *   39/40 (rain night) → 12 (rain day)
 *   41/42 (storm night) → 15 (storm day)
 *   43 (snow showers night) → 19 (snow showers day)
 *   44 (snow night) → 22 (snow day)
 */
static const wmo_icon_map_t WMO_ICON_MAP[] = {
    // Clear & mostly clear
    {0,   1,  33, "Clear sky"},
    {1,   2,  34, "Mainly clear"},
    {2,   3,  35, "Partly cloudy"},

    // Cloudy
    {3,   7,  7,  "Cloudy"},

    // Fog
    {45,  11, 11, "Foggy"},
    {48,  11, 11, "Depositing rime fog"},

    // Drizzle (light)
    {51,  12, 12, "Light drizzle"},
    {53,  12, 12, "Moderate drizzle"},
    {55,  12, 12, "Dense drizzle"},

    // Rain
    {61,  18, 18, "Slight rain"},
    {63,  18, 18, "Moderate rain"},
    {65,  18, 18, "Heavy rain"},

    // Freezing rain
    {66,  26, 26, "Light freezing rain"},
    {67,  26, 26, "Heavy freezing rain"},

    // Snow
    {71,  22, 22, "Slight snow"},
    {73,  22, 22, "Moderate snow"},
    {75,  22, 22, "Heavy snow"},
    {77,  22, 22, "Snow grains"},

    // Rain showers
    {80,  12, 12, "Slight rain showers"},
    {81,  12, 12, "Moderate rain showers"},
    {82,  12, 12, "Violent rain showers"},

    // Snow showers
    {85,  19, 19, "Slight snow showers"},
    {86,  19, 19, "Heavy snow showers"},

    // Thunderstorm
    {95,  15, 15, "Thunderstorm"},
    {96,  15, 15, "Thunderstorm with hail"},
    {99,  15, 15, "Thunderstorm with hail"},
};

#define WMO_MAP_SIZE (sizeof(WMO_ICON_MAP) / sizeof(WMO_ICON_MAP[0]))

/**
 * Get AccuWeather icon number for WMO code
 * @param wmo_code: WMO weather code (0-99)
 * @param is_night: true for nighttime icon, false for day
 * @return: AccuWeather icon number (1-44), or 1 if code not found
 */
int wmo_get_icon_id(int wmo_code, bool is_night);

/**
 * Get weather description for WMO code
 * @param wmo_code: WMO weather code
 * @return: Description string, or "Unknown" if not found
 */
const char *wmo_get_description(int wmo_code);

/**
 * Determine if it's nighttime based on current time
 * Uses simple heuristic: between 18:00 and 06:00
 * For proper implementation, would need sunrise/sunset times
 * @param timestamp: Unix timestamp
 * @param timezone_offset: Hours offset from UTC (e.g., -7 for MST)
 * @return: true if nighttime, false if daytime
 */
bool wmo_is_nighttime(uint32_t timestamp, int timezone_offset);

#endif // WMO_ICON_MAP_H
