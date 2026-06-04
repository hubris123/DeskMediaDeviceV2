#include "wmo_icon_map.h"
#include <time.h>
#include <string.h>

int wmo_get_icon_id(int wmo_code, bool is_night)
{
    for (int i = 0; i < WMO_MAP_SIZE; i++) {
        if (WMO_ICON_MAP[i].wmo_code == wmo_code) {
            return is_night ? WMO_ICON_MAP[i].accuweather_icon_night
                           : WMO_ICON_MAP[i].accuweather_icon_day;
        }
    }
    // Fallback: clear day
    return 1;
}

const char *wmo_get_description(int wmo_code)
{
    for (int i = 0; i < WMO_MAP_SIZE; i++) {
        if (WMO_ICON_MAP[i].wmo_code == wmo_code) {
            return WMO_ICON_MAP[i].description;
        }
    }
    return "Unknown condition";
}

bool wmo_is_nighttime(uint32_t timestamp, int timezone_offset)
{
    time_t t = (time_t)timestamp;
    struct tm *tm_info = gmtime(&t);

    // Adjust to local time using timezone offset
    int local_hour = tm_info->tm_hour + timezone_offset;

    // Handle day wraparound
    if (local_hour < 0) {
        local_hour += 24;
    } else if (local_hour >= 24) {
        local_hour -= 24;
    }

    // Night: 18:00 (6 PM) to 06:00 (6 AM)
    return (local_hour >= 18) || (local_hour < 6);
}
