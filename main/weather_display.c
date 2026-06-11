#include "weather_display.h"
#include "weather_icons.h"
#include "data_binding.h"
#include "GUI.h"
#include "lvgl.h"
#include "esp_log.h"

static const char *TAG = "WeatherDisplay";

/* Network icon declarations */
LV_IMG_DECLARE(upload_network_e6b6f98e6cbd4491bfe5310d621d416f_png);    // green - connected
LV_IMG_DECLARE(upload_networkred_45539a8ee52f46688e0fbeb4c45b9ddf_png); // red - disconnected

/* ── Internal helpers ───────────────────────────────────────────────── */

/* Wind label is a child of CURRENTWINDQ container — find it at index 0 */
static lv_obj_t *get_wind_label(void)
{
    return lv_obj_get_child(GUI_Container__home__CURRENTWINDQ, 0);
}

/* ── Public API ─────────────────────────────────────────────────────── */

void weather_display_update(const weather_display_t *w)
{
    if (!w) return;
    char buf[64];

    /* ── Current conditions ─────────────────────────────────────────── */

    /* Location */
    snprintf(buf, sizeof(buf), "%s, %s", w->city, w->state);
    lv_label_set_text(GUI_Label__home__LOCATIONQ, buf);

    /* Current temperature */
    snprintf(buf, sizeof(buf), "%.0f°", w->current_temp_f);
    lv_label_set_text(GUI_Label__home__CURRENTTEMPQ, buf);

    /* Feels like */
    snprintf(buf, sizeof(buf), "%.0f°", w->feels_like_f);
    lv_label_set_text(GUI_Label__home__FEELSLIKETEMPQ, buf);

    /* Main 60x60 weather icon */
    lv_image_set_src(GUI_Image__home__60X60ICONQ,
                     weather_icon_60(w->current_wmo, w->is_night));

    /* Status text (e.g. "Partly Cloudy") — auto-fits font, never splits words */
    ui_set_status_text(weather_description(w->current_wmo));

    /* Humidity */
    snprintf(buf, sizeof(buf), "%d%%", w->current_humidity);
    lv_label_set_text(GUI_Label__home__HUMIDITYPERCENTQ, buf);

    /* Chance of rain (next hour precipitation probability) */
    snprintf(buf, sizeof(buf), "%d%%", w->current_precip_prob);
    lv_label_set_text(GUI_Label__home__PRECIPITATIONQ, buf);

    /* Wind speed + direction (child label inside the container) */
    lv_obj_t *wind_label = get_wind_label();
    if (wind_label) {
        lv_label_set_text(wind_label, w->current_wind_str);
    }

    /* Time is owned by clock_timer_cb in main.c — do not set here */

    /* Network icon */
    weather_display_update_network(w->wifi_connected);

    /* ── 5-hour hourly forecast ─────────────────────────────────────── */

    /* Hour 1 */
    lv_label_set_text(GUI_Label__home__HR1FORCASTTIMEQ, w->hourly_time[0]);
    lv_image_set_src(GUI_Image__home__HR1FORCASICON22X22Q,
                     weather_icon_22(w->hourly_wmo[0], w->is_night));
    snprintf(buf, sizeof(buf), "%.0f°", w->hourly_temp_f[0]);
    lv_label_set_text(GUI_Label__home__HR1FORCASTTEMPQ, buf);

    /* Hour 2 */
    lv_label_set_text(GUI_Label__home__HR2FORCASTTIMEQ, w->hourly_time[1]);
    lv_image_set_src(GUI_Image__home__HR2FORCASTICON22X22Q,
                     weather_icon_22(w->hourly_wmo[1], w->is_night));
    snprintf(buf, sizeof(buf), "%.0f°", w->hourly_temp_f[1]);
    lv_label_set_text(GUI_Label__home__HR2FORCASTTEMPQ, buf);

    /* Hour 3 */
    lv_label_set_text(GUI_Label__home__HR3FORCASTTIMEQ, w->hourly_time[2]);
    lv_image_set_src(GUI_Image__home__HR3FORCASTICON22X22Q,
                     weather_icon_22(w->hourly_wmo[2], w->is_night));
    snprintf(buf, sizeof(buf), "%.0f°", w->hourly_temp_f[2]);
    lv_label_set_text(GUI_Label__home__HR3FORCASTTEMPQ, buf);

    /* Hour 4 */
    lv_label_set_text(GUI_Label__home__HR4FORCASTTIMEQ, w->hourly_time[3]);
    lv_image_set_src(GUI_Image__home__HR4FORCASTICON22X22Q,
                     weather_icon_22(w->hourly_wmo[3], w->is_night));
    snprintf(buf, sizeof(buf), "%.0f°", w->hourly_temp_f[3]);
    lv_label_set_text(GUI_Label__home__HR4FORCASTTEMPQ, buf);

    /* Hour 5 removed from UI */

    /* ── 3-day daily forecast ───────────────────────────────────────── */

    /* Day 1 */
    lv_label_set_text(GUI_Label__home__DAYLY1FORCASTDAYQ, w->daily_day[0]);
    snprintf(buf, sizeof(buf), "%.0f°", w->daily_high_f[0]);
    lv_label_set_text(GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ, buf);
    snprintf(buf, sizeof(buf), "%.0f°", w->daily_low_f[0]);
    lv_label_set_text(GUI_Label__home__DAYLY1FORCASTTEMPLOWQ, buf);
    /* Day 2 */
    lv_label_set_text(GUI_Label__home__DAYLY2FORCASTDAYQ, w->daily_day[1]);
    snprintf(buf, sizeof(buf), "%.0f°", w->daily_high_f[1]);
    lv_label_set_text(GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ, buf);
    snprintf(buf, sizeof(buf), "%.0f°", w->daily_low_f[1]);
    lv_label_set_text(GUI_Label__home__DAYLY2FORCASTTEMPLOWQ, buf);

    /* Day 3 */
    lv_label_set_text(GUI_Label__home__DAYLY3FORCASTDAYQ, w->daily_day[2]);
    snprintf(buf, sizeof(buf), "%.0f°", w->daily_high_f[2]);
    lv_label_set_text(GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ, buf);
    snprintf(buf, sizeof(buf), "%.0f°", w->daily_low_f[2]);
    lv_label_set_text(GUI_Label__home__DAYLY3FORCASTTEMPLOWQ, buf);

    ESP_LOGI(TAG, "Display updated: %s %.0fF %s",
             w->city, w->current_temp_f, weather_description(w->current_wmo));
}

void weather_display_update_time(const char *time_str)
{
    lv_label_set_text(GUI_Label__home__CURRENTTIMEQ, time_str);
}

void weather_display_update_network(bool connected)
{
    if (connected) {
        lv_image_set_src(GUI_Image__home__NETWORKCONNECTEDICONQ,
                         &upload_network_e6b6f98e6cbd4491bfe5310d621d416f_png);
    } else {
        lv_image_set_src(GUI_Image__home__NETWORKCONNECTEDICONQ,
                         &upload_networkred_45539a8ee52f46688e0fbeb4c45b9ddf_png);
    }
}
