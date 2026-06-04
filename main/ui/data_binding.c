#include "data_binding.h"
#include "weather_data.h"
#include "wmo_icon_map.h"
#include "GUI.h"
#include "esp_log.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

static const char *TAG = "DataBinding";

// ============ Helper Functions ============

int format_temperature(float temp, char *buf, size_t len)
{
    return snprintf(buf, len, "%.0f°F", temp);
}

int format_time_12h(uint32_t timestamp, char *buf, size_t len)
{
    time_t t = (time_t)timestamp;
    struct tm *tm_info = localtime(&t);
    return strftime(buf, len, "%I %p", tm_info);  // "3 PM", "10 AM", etc.
}

int format_day_name(uint32_t timestamp, char *buf, size_t len)
{
    time_t t = (time_t)timestamp;
    struct tm *tm_info = localtime(&t);
    return strftime(buf, len, "%a", tm_info);  // "Mon", "Tue", etc.
}

/**
 * Get embedded weather icon from AccuWeather icon number
 * Maps icon numbers to SquareLine embedded image resources
 */
static const lv_image_dsc_t *get_weather_icon_60(int icon_num)
{
    // Embedded icons from SquareLine GUI.h
    // These are available as upload_X_HASH_png, upload_Xs_HASH_png
    switch (icon_num) {
        case 1:  return &upload_1_7fa59a4c14414e4ab42f8f8cf8ee5df8_png;
        case 2:  return &upload_2_e2278bcef5404f3da02d7aa2a510f534_png;
        case 3:  return &upload_3_20748ad6e84045c396cda8b366aa5d07_png;
        case 7:  return &upload_7_5e6957ba671848fcae58518a4764438c_png;
        case 11: return &upload_11_d928da2afce943088fc003fded319081_png;
        case 12: return &upload_12_71141d8aba63422c8a7377026b8cecc6_png;
        case 14: return &upload_14_14ff6d55212c4e7b945e173b90b81139_png;
        case 15: return &upload_15_3a414dd3d7e14d559033812d417eaa90_png;
        case 18: return &upload_18_b42c444b7f1445fc9003e4caa65d5a60_png;
        case 19: return &upload_19_2425f3503fc04098b87dd50fc6604b61_png;
        case 22: return &upload_22_6dccc7824e764e4a9b70ff8d5b078fad_png;
        case 26: return &upload_26_28f0c2296c7742bbad7e6ed91f80b81a_png;
        case 33: return &upload_33_b02a9c2486c94766bd9856f56db21f1f_png;
        case 34: return &upload_34_4c27b41ae3a1496089c4d14141615cbd_png;
        case 35: return &upload_35_cc7cb52862b9422aba85d07b935bc63c_png;
        default: return &upload_1_7fa59a4c14414e4ab42f8f8cf8ee5df8_png;  // Fallback
    }
}

/**
 * Get embedded weather icon (22x22 small version)
 */
static const lv_image_dsc_t *get_weather_icon_22(int icon_num)
{
    switch (icon_num) {
        case 1:  return &upload_1s_a942bd7a65904cab870f8cf337226275_png;
        case 2:  return &upload_2s_37e4aa2fa7024542b513edb68eebbfa3_png;
        case 3:  return &upload_3s_e6f8aad40a204657b22d932c4b20fb18_png;
        case 7:  return &upload_7s_c385fe6874fa4da5853f367e29db7db4_png;
        case 11: return &upload_11s_7c1fa968af52485bbeb874d89e56f96b_png;
        case 12: return &upload_12s_7ed83dc3cb5f41f1a541dfd52ba15f5c_png;
        case 14: return &upload_14s_b570241f67234745a56f9ddc06bd7800_png;
        case 15: return &upload_15s_71a05d920e1a428e86707846a7f46127_png;
        case 18: return &upload_18s_e55d60ec77034c32846327663b493f72_png;
        case 19: return &upload_19s_c04d370ce95c403b82416b454729a8a8_png;
        case 22: return &upload_22s_f739a4fff8ce46c5a09433c09b5b94c6_png;
        case 26: return &upload_26s_731f337d30e945da969f8e1271adfa24_png;
        case 33: return &upload_33s_b5e096b136ce4b6fbf7748e265b3d5eb_png;
        case 34: return &upload_34s_d824cbffe8e8400cabc43e41608fd72d_png;
        case 35: return &upload_35s_7a2b8dbb6e8a4c1d8a796fed8458d198_png;
        default: return &upload_1s_a942bd7a65904cab870f8cf337226275_png;  // Fallback
    }
}

// ============ Public UI Update Functions ============

esp_err_t ui_update_current_weather(const weather_data_t *weather)
{
    if (!weather) {
        return ESP_ERR_INVALID_ARG;
    }

    char buffer[64];

    // Current time
    format_time_12h(weather->current_time, buffer, sizeof(buffer));
    lv_label_set_text(GUI_Label__home__CURRENTTIMEQ, buffer);

    // Location: City, State
    snprintf(buffer, sizeof(buffer), "%s, %s", weather->city, weather->state);
    lv_label_set_text(GUI_Label__home__LOCATIONQ, buffer);

    // Current temperature
    format_temperature(weather->current_temp, buffer, sizeof(buffer));
    lv_label_set_text(GUI_Label__home__CURRENTTEMPQ, buffer);

    // Feels like (use same as current temp for now)
    format_temperature(weather->current_temp, buffer, sizeof(buffer));
    lv_label_set_text(GUI_Label__home__FEELSLIKETEMPQ, buffer);

    // Humidity percentage
    snprintf(buffer, sizeof(buffer), "%d%%", weather->current_humidity);
    lv_label_set_text(GUI_Label__home__HUMIDITYPERCENTQ, buffer);

    // Chance of rain
    snprintf(buffer, sizeof(buffer), "%d%%", weather->current_precip_prob);
    // lv_label_set_text(GUI_Label__home__CHANCEOFRAINQ, buffer); // removed from UI

    // Weather description
    const char *desc = wmo_get_description(weather->current_weather_code);
    lv_label_set_text(GUI_Label__home__CURRENTSTATUSQ, desc);

    // Weather icon (60x60)
    int icon_id = wmo_get_icon_id(weather->current_weather_code,
                                  wmo_is_nighttime(weather->current_time, -7));  // MST offset
    const lv_image_dsc_t *icon = get_weather_icon_60(icon_id);
    lv_image_set_src(GUI_Image__home__60X60ICONQ, icon);

    ESP_LOGI(TAG, "Updated current weather: %.1f°F, %d%%, icon=%d",
            weather->current_temp, weather->current_humidity, icon_id);

    return ESP_OK;
}

esp_err_t ui_update_hourly_forecast(const weather_data_t *weather)
{
    if (!weather) {
        return ESP_ERR_INVALID_ARG;
    }

    char buffer[64];

    // HR1-HR5
    lv_obj_t *time_labels[] = {
        GUI_Label__home__HR1FORCASTTIMEQ,
        GUI_Label__home__HR2FORCASTTIMEQ,
        GUI_Label__home__HR3FORCASTTIMEQ,
        GUI_Label__home__HR4FORCASTTIMEQ,
        // GUI_Label__home__HR5FORCASTTIMEQ, // removed from UI
    };

    lv_obj_t *icon_imgs[] = {
        GUI_Image__home__HR1FORCASICON22X22Q,
        GUI_Image__home__HR2FORCASTICON22X22Q,
        GUI_Image__home__HR3FORCASTICON22X22Q,
        GUI_Image__home__HR4FORCASTICON22X22Q,
        // GUI_Image__home__HR5FORCASTICON22X22Q, // removed from UI
    };

    lv_obj_t *temp_labels[] = {
        GUI_Label__home__HR1FORCASTTEMPQ,
        GUI_Label__home__HR2FORCASTTEMPQ,
        GUI_Label__home__HR3FORCASTTEMPQ,
        GUI_Label__home__HR4FORCASTTEMPQ,
        // GUI_Label__home__HR5FORCASTTEMPQ, // removed from UI
    };

    for (int i = 0; i < 4; i++) {
        // Time
        lv_label_set_text(time_labels[i], weather->hourly.times_12h[i]);

        // Temperature
        format_temperature(weather->hourly.hours[i].temperature, buffer, sizeof(buffer));
        lv_label_set_text(temp_labels[i], buffer);

        // Icon
        int icon_id = wmo_get_icon_id(weather->hourly.hours[i].weather_code,
                                     wmo_is_nighttime(weather->hourly.hours[i].timestamp, -7));
        const lv_image_dsc_t *icon = get_weather_icon_22(icon_id);
        lv_image_set_src(icon_imgs[i], icon);
    }

    ESP_LOGI(TAG, "Updated hourly forecast (5 hours)");
    return ESP_OK;
}

esp_err_t ui_update_daily_forecast(const weather_data_t *weather)
{
    if (!weather) {
        return ESP_ERR_INVALID_ARG;
    }

    char buffer[64];

    // DAYLY1-DAYLY3
    lv_obj_t *day_labels[] = {
        GUI_Label__home__DAYLY1FORCASTDAYQ,
        GUI_Label__home__DAYLY2FORCASTDAYQ,
        GUI_Label__home__DAYLY3FORCASTDAYQ,
    };

    lv_obj_t *high_labels[] = {
        GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ,
        GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ,
        GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ,
    };

    lv_obj_t *low_labels[] = {
        GUI_Label__home__DAYLY1FORCASTTEMPLOWQ,
        GUI_Label__home__DAYLY2FORCASTTEMPLOWQ,
        GUI_Label__home__DAYLY3FORCASTTEMPLOWQ,
    };

    lv_obj_t *status_labels[] = {
        GUI_Label__home__DAYLY1FORCASTSTATUSQ,
        GUI_Label__home__DAYLY2FORCASTSTATUSQ,
        GUI_Label__home__DAYLY3FORCASTSTATUSQ,
    };

    for (int i = 0; i < 3; i++) {
        // Day name
        lv_label_set_text(day_labels[i], weather->daily[i].day_name);

        // High/Low temps
        format_temperature(weather->daily[i].temp_high, buffer, sizeof(buffer));
        lv_label_set_text(high_labels[i], buffer);

        format_temperature(weather->daily[i].temp_low, buffer, sizeof(buffer));
        lv_label_set_text(low_labels[i], buffer);

        // Status
        const char *desc = wmo_get_description(weather->daily[i].weather_code);
        lv_label_set_text(status_labels[i], desc);
    }

    ESP_LOGI(TAG, "Updated daily forecast (3 days)");
    return ESP_OK;
}

esp_err_t ui_update_network_status(bool connected)
{
    if (connected) {
        lv_image_set_src(GUI_Image__home__NETWORKCONNECTEDICONQ,
                        &upload_network_e6b6f98e6cbd4491bfe5310d621d416f_png);
        ESP_LOGI(TAG, "Network status: CONNECTED (green)");
    } else {
        lv_image_set_src(GUI_Image__home__NETWORKCONNECTEDICONQ,
                        &upload_networkred_45539a8ee52f46688e0fbeb4c45b9ddf_png);
        ESP_LOGI(TAG, "Network status: DISCONNECTED (red)");
    }

    return ESP_OK;
}

esp_err_t ui_refresh_weather_display(const weather_data_t *weather, bool wifi_connected)
{
    if (!weather) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = ESP_OK;

    ret |= ui_update_current_weather(weather);
    ret |= ui_update_hourly_forecast(weather);
    ret |= ui_update_daily_forecast(weather);
    ret |= ui_update_network_status(wifi_connected);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Weather display fully updated");
    } else {
        ESP_LOGW(TAG, "Some weather display updates failed");
    }

    return ret;
}
