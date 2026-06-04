#include "weather_icons.h"

/* ── Internal helper: WMO code → icon index ─────────────────────────── */

typedef enum {
    ICON_CLEAR_DAY = 0,
    ICON_MAINLY_CLEAR_DAY,
    ICON_PARTLY_CLOUDY_DAY,
    ICON_CLOUDY,
    ICON_FOG,
    ICON_DRIZZLE,
    ICON_RAIN_SHOWERS,
    ICON_THUNDERSTORM,
    ICON_RAIN,
    ICON_SNOW_SHOWERS,
    ICON_SNOW,
    ICON_FREEZING_RAIN,
    ICON_CLEAR_NIGHT,
    ICON_MAINLY_CLEAR_NIGHT,
    ICON_PARTLY_CLOUDY_NIGHT,
    ICON_COUNT
} icon_index_t;

/* 60x60 icon table */
static const lv_image_dsc_t *icons_60[ICON_COUNT] = {
    [ICON_CLEAR_DAY]           = &upload_1_7fa59a4c14414e4ab42f8f8cf8ee5df8_png,
    [ICON_MAINLY_CLEAR_DAY]    = &upload_2_e2278bcef5404f3da02d7aa2a510f534_png,
    [ICON_PARTLY_CLOUDY_DAY]   = &upload_3_20748ad6e84045c396cda8b366aa5d07_png,
    [ICON_CLOUDY]              = &upload_7_5e6957ba671848fcae58518a4764438c_png,
    [ICON_FOG]                 = &upload_11_d928da2afce943088fc003fded319081_png,
    [ICON_DRIZZLE]             = &upload_12_71141d8aba63422c8a7377026b8cecc6_png,
    [ICON_RAIN_SHOWERS]        = &upload_14_14ff6d55212c4e7b945e173b90b81139_png,
    [ICON_THUNDERSTORM]        = &upload_15_3a414dd3d7e14d559033812d417eaa90_png,
    [ICON_RAIN]                = &upload_18_b42c444b7f1445fc9003e4caa65d5a60_png,
    [ICON_SNOW_SHOWERS]        = &upload_19_2425f3503fc04098b87dd50fc6604b61_png,
    [ICON_SNOW]                = &upload_22_6dccc7824e764e4a9b70ff8d5b078fad_png,
    [ICON_FREEZING_RAIN]       = &upload_26_28f0c2296c7742bbad7e6ed91f80b81a_png,
    [ICON_CLEAR_NIGHT]         = &upload_33_b02a9c2486c94766bd9856f56db21f1f_png,
    [ICON_MAINLY_CLEAR_NIGHT]  = &upload_34_4c27b41ae3a1496089c4d14141615cbd_png,
    [ICON_PARTLY_CLOUDY_NIGHT] = &upload_35_cc7cb52862b9422aba85d07b935bc63c_png,
};

/* 22x22 icon table */
static const lv_image_dsc_t *icons_22[ICON_COUNT] = {
    [ICON_CLEAR_DAY]           = &upload_1s_a942bd7a65904cab870f8cf337226275_png,
    [ICON_MAINLY_CLEAR_DAY]    = &upload_2s_37e4aa2fa7024542b513edb68eebbfa3_png,
    [ICON_PARTLY_CLOUDY_DAY]   = &upload_3s_e6f8aad40a204657b22d932c4b20fb18_png,
    [ICON_CLOUDY]              = &upload_7s_c385fe6874fa4da5853f367e29db7db4_png,
    [ICON_FOG]                 = &upload_11s_7c1fa968af52485bbeb874d89e56f96b_png,
    [ICON_DRIZZLE]             = &upload_12s_7ed83dc3cb5f41f1a541dfd52ba15f5c_png,
    [ICON_RAIN_SHOWERS]        = &upload_14s_b570241f67234745a56f9ddc06bd7800_png,
    [ICON_THUNDERSTORM]        = &upload_15s_71a05d920e1a428e86707846a7f46127_png,
    [ICON_RAIN]                = &upload_18s_e55d60ec77034c32846327663b493f72_png,
    [ICON_SNOW_SHOWERS]        = &upload_19s_c04d370ce95c403b82416b454729a8a8_png,
    [ICON_SNOW]                = &upload_22s_f739a4fff8ce46c5a09433c09b5b94c6_png,
    [ICON_FREEZING_RAIN]       = &upload_26s_731f337d30e945da969f8e1271adfa24_png,
    [ICON_CLEAR_NIGHT]         = &upload_33s_b5e096b136ce4b6fbf7748e265b3d5eb_png,
    [ICON_MAINLY_CLEAR_NIGHT]  = &upload_34s_d824cbffe8e8400cabc43e41608fd72d_png,
    [ICON_PARTLY_CLOUDY_NIGHT] = &upload_35s_7a2b8dbb6e8a4c1d8a796fed8458d198_png,
};

/* Map WMO code to day icon index */
static icon_index_t wmo_to_day_icon(int wmo_code)
{
    switch (wmo_code) {
        case 0:                         return ICON_CLEAR_DAY;
        case 1:                         return ICON_MAINLY_CLEAR_DAY;
        case 2:                         return ICON_PARTLY_CLOUDY_DAY;
        case 3:                         return ICON_CLOUDY;
        case 45: case 48:               return ICON_FOG;
        case 51: case 53: case 55:
        case 56: case 57:               return ICON_DRIZZLE;
        case 61: case 63: case 65:      return ICON_RAIN;
        case 66: case 67:               return ICON_FREEZING_RAIN;
        case 71: case 73: case 75:
        case 77:                        return ICON_SNOW;
        case 80: case 81: case 82:      return ICON_RAIN_SHOWERS;
        case 85: case 86:               return ICON_SNOW_SHOWERS;
        case 95: case 96: case 99:      return ICON_THUNDERSTORM;
        default:                        return ICON_CLEAR_DAY;
    }
}

/* Map WMO code to night icon index */
static icon_index_t wmo_to_night_icon(int wmo_code)
{
    switch (wmo_code) {
        case 0:                         return ICON_CLEAR_NIGHT;
        case 1:                         return ICON_MAINLY_CLEAR_NIGHT;
        case 2:                         return ICON_PARTLY_CLOUDY_NIGHT;
        default:
            /* All other conditions look the same day or night */
            return wmo_to_day_icon(wmo_code);
    }
}

/* ── Public API ──────────────────────────────────────────────────────── */

const lv_image_dsc_t *weather_icon_60(int wmo_code, bool is_night)
{
    icon_index_t idx = is_night ? wmo_to_night_icon(wmo_code)
                                : wmo_to_day_icon(wmo_code);
    return icons_60[idx];
}

const lv_image_dsc_t *weather_icon_22(int wmo_code, bool is_night)
{
    icon_index_t idx = is_night ? wmo_to_night_icon(wmo_code)
                                : wmo_to_day_icon(wmo_code);
    return icons_22[idx];
}

const char *weather_description(int wmo_code)
{
    switch (wmo_code) {
        case 0:                    return "Clear";
        case 1:                    return "Mainly Clear";
        case 2:                    return "Partly Cloudy";
        case 3:                    return "Cloudy";
        case 45: case 48:          return "Foggy";
        case 51: case 53: case 55: return "Drizzle";
        case 56: case 57:          return "Freezing Drizzle";
        case 61: case 63: case 65: return "Rain";
        case 66: case 67:          return "Freezing Rain";
        case 71: case 73: case 75:
        case 77:                   return "Snow";
        case 80: case 81: case 82: return "Rain Showers";
        case 85: case 86:          return "Snow Showers";
        case 95:                   return "Thunderstorm";
        case 96: case 99:          return "Severe Storm";
        default:                   return "Unknown";
    }
}
