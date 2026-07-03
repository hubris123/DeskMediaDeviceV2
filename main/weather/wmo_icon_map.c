#include "wmo_icon_map.h"
#include <time.h>
#include <string.h>
#include <math.h>

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

    int local_hour = tm_info->tm_hour + timezone_offset;
    if (local_hour < 0)        local_hour += 24;
    else if (local_hour >= 24) local_hour -= 24;

    return (local_hour >= 18) || (local_hour < 6);
}

bool solar_is_night(uint32_t timestamp, float lat_deg, float lon_deg)
{
    // NOAA solar elevation algorithm.
    // Returns true when sun is below the horizon (elevation < -0.833 deg,
    // which accounts for atmospheric refraction and solar disc radius).
    const double DEG = M_PI / 180.0;

    // Julian date from Unix timestamp
    double jd = (double)timestamp / 86400.0 + 2440587.5;

    // Julian century
    double jc = (jd - 2451545.0) / 36525.0;

    // Mean longitude and anomaly (degrees)
    double ml = fmod(280.46646 + jc * (36000.76983 + jc * 0.0003032), 360.0);
    double ma = 357.52911 + jc * (35999.05029 - 0.0001537 * jc);

    // Sun's equation of center
    double eoc = sin(ma * DEG) * (1.914602 - jc * (0.004817 + 0.000014 * jc))
               + sin(2.0 * ma * DEG) * (0.019993 - 0.000101 * jc)
               + sin(3.0 * ma * DEG) * 0.000289;

    // Sun's true longitude → apparent longitude
    double stl = ml + eoc;
    double sal  = stl - 0.00569 - 0.00478 * sin((125.04 - 1934.136 * jc) * DEG);

    // Mean obliquity of ecliptic + correction
    double moe = 23.0 + (26.0 + (21.448 - jc * (46.8150 + jc * (0.00059 - jc * 0.001813))) / 60.0) / 60.0;
    double oe  = moe + 0.00256 * cos((125.04 - 1934.136 * jc) * DEG);

    // Sun's declination (degrees)
    double decl = asin(sin(oe * DEG) * sin(sal * DEG)) / DEG;

    // Equation of time (minutes)
    double y    = tan(oe * DEG / 2.0);
    y *= y;
    double eqt  = 4.0 / DEG * (y * sin(2.0 * ml * DEG)
                  - 2.0 * 0.016708634 * sin(ma * DEG)
                  + 4.0 * 0.016708634 * y * sin(ma * DEG) * cos(2.0 * ml * DEG)
                  - 0.5 * y * y * sin(4.0 * ml * DEG)
                  - 1.25 * 0.016708634 * 0.016708634 * sin(2.0 * ma * DEG));

    // True solar time (minutes into current day, UTC)
    double utc_min = fmod((double)timestamp / 60.0, 1440.0);
    double tst = fmod(utc_min + eqt + 4.0 * (double)lon_deg, 1440.0);
    if (tst < 0.0) tst += 1440.0;

    // Hour angle (degrees)
    double ha = (tst / 4.0) - 180.0;

    // Solar zenith angle
    double lat = (double)lat_deg;
    double cos_z = sin(lat * DEG) * sin(decl * DEG)
                 + cos(lat * DEG) * cos(decl * DEG) * cos(ha * DEG);

    // Elevation = 90 - zenith; night when elevation < -0.833 deg
    double elevation = 90.0 - acos(cos_z) / DEG;
    return elevation < -0.833;
}
