#include "../GUI.h"

void GUI_initScreen__home() {
  GUI_Screen__home = lv_obj_create(NULL);
  lv_obj_remove_flag(GUI_Screen__home, LV_OBJ_FLAG_SCROLLABLE);
  GUI_Container__home__weatherwidget = lv_obj_create(GUI_Screen__home);
  lv_obj_remove_style_all(GUI_Container__home__weatherwidget);
  lv_obj_remove_flag(GUI_Container__home__weatherwidget, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__weatherwidget,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__weatherwidget, LV_ALIGN_CENTER);
  lv_obj_set_x(GUI_Container__home__weatherwidget, -275);
  lv_obj_set_size(GUI_Container__home__weatherwidget, 232, 475);

  GUI_Panel__home__weather_panel =
      lv_obj_create(GUI_Container__home__weatherwidget);
  lv_obj_remove_flag(GUI_Panel__home__weather_panel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__home__weather_panel, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Panel__home__weather_panel, 230, 470);

  GUI_Container__home__main_display_cont =
      lv_obj_create(GUI_Panel__home__weather_panel);
  lv_obj_remove_style_all(GUI_Container__home__main_display_cont);
  lv_obj_remove_flag(GUI_Container__home__main_display_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__main_display_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__main_display_cont, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__main_display_cont, -7, -155);
  lv_obj_set_size(GUI_Container__home__main_display_cont, 214, 179);

  GUI_Label__home__CURRENTTIMEQ =
      lv_label_create(GUI_Container__home__main_display_cont);
  lv_label_set_long_mode(GUI_Label__home__CURRENTTIMEQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__CURRENTTIMEQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__CURRENTTIMEQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__CURRENTTIMEQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__main_display_title_lbl =
      lv_label_create(GUI_Container__home__main_display_cont);
  lv_label_set_long_mode(GUI_Label__home__main_display_title_lbl,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__main_display_title_lbl, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__main_display_title_lbl, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__LOCATIONQ =
      lv_label_create(GUI_Container__home__main_display_cont);
  lv_label_set_long_mode(GUI_Label__home__LOCATIONQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__LOCATIONQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__LOCATIONQ, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Label__home__CURRENTTEMPQ =
      lv_label_create(GUI_Container__home__main_display_cont);
  lv_label_set_long_mode(GUI_Label__home__CURRENTTEMPQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__CURRENTTEMPQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__CURRENTTEMPQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__current_status_cont =
      lv_obj_create(GUI_Panel__home__weather_panel);
  lv_obj_remove_style_all(GUI_Container__home__current_status_cont);
  lv_obj_remove_flag(GUI_Container__home__current_status_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__current_status_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_pos(GUI_Container__home__current_status_cont, 23, 153);
  lv_obj_set_size(GUI_Container__home__current_status_cont, 188, 95);

  GUI_Image__home__60X60ICONQ =
      lv_image_create(GUI_Container__home__current_status_cont);
  lv_obj_add_flag(GUI_Image__home__60X60ICONQ, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__60X60ICONQ, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__60X60ICONQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__home__60X60ICONQ, 60, 60);

  GUI_Label__home__CURRENTSTATUSQ =
      lv_label_create(GUI_Container__home__current_status_cont);
  lv_label_set_long_mode(GUI_Label__home__CURRENTSTATUSQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__CURRENTSTATUSQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__CURRENTSTATUSQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__details_cont =
      lv_obj_create(GUI_Panel__home__weather_panel);
  lv_obj_remove_style_all(GUI_Container__home__details_cont);
  lv_obj_remove_flag(GUI_Container__home__details_cont, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__details_cont, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__details_cont, LV_ALIGN_TOP_RIGHT);
  lv_obj_set_pos(GUI_Container__home__details_cont, -11, 217);
  lv_obj_set_size(GUI_Container__home__details_cont, 211, 83);

  GUI_Container__home__details_feels_like_cont =
      lv_obj_create(GUI_Container__home__details_cont);
  lv_obj_remove_style_all(GUI_Container__home__details_feels_like_cont);
  lv_obj_remove_flag(GUI_Container__home__details_feels_like_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__details_feels_like_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__details_feels_like_cont, 1);
  lv_obj_set_align(GUI_Container__home__details_feels_like_cont,
                   LV_ALIGN_CENTER);
  lv_obj_set_height(GUI_Container__home__details_feels_like_cont, 50);

  GUI_Label__home__details_feels_like_title_lbl =
      lv_label_create(GUI_Container__home__details_feels_like_cont);
  lv_label_set_long_mode(GUI_Label__home__details_feels_like_title_lbl,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__details_feels_like_title_lbl,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__details_feels_like_title_lbl,
                  LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Label__home__FEELSLIKETEMPQ =
      lv_label_create(GUI_Container__home__details_feels_like_cont);
  lv_label_set_long_mode(GUI_Label__home__FEELSLIKETEMPQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__FEELSLIKETEMPQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__FEELSLIKETEMPQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__FEELSLIKETEMPQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Panel__home__details_separator_1_panel =
      lv_obj_create(GUI_Container__home__details_cont);
  lv_obj_remove_flag(GUI_Panel__home__details_separator_1_panel,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__home__details_separator_1_panel, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Panel__home__details_separator_1_panel, 1, 40);

  GUI_Container__home__details_humidity_cont =
      lv_obj_create(GUI_Container__home__details_cont);
  lv_obj_remove_style_all(GUI_Container__home__details_humidity_cont);
  lv_obj_remove_flag(GUI_Container__home__details_humidity_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__details_humidity_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__details_humidity_cont, 1);
  lv_obj_set_align(GUI_Container__home__details_humidity_cont, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__details_humidity_cont, 20, 20);
  lv_obj_set_height(GUI_Container__home__details_humidity_cont, 50);

  GUI_Label__home__details_humidity_title_lbl =
      lv_label_create(GUI_Container__home__details_humidity_cont);
  lv_label_set_long_mode(GUI_Label__home__details_humidity_title_lbl,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__details_humidity_title_lbl,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__details_humidity_title_lbl, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__HUMIDITYPERCENTQ =
      lv_label_create(GUI_Container__home__details_humidity_cont);
  lv_label_set_long_mode(GUI_Label__home__HUMIDITYPERCENTQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__HUMIDITYPERCENTQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__HUMIDITYPERCENTQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__HUMIDITYPERCENTQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Panel__home__details_separator_2_panel =
      lv_obj_create(GUI_Container__home__details_cont);
  lv_obj_remove_flag(GUI_Panel__home__details_separator_2_panel,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__home__details_separator_2_panel, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Panel__home__details_separator_2_panel, 20, 20);
  lv_obj_set_size(GUI_Panel__home__details_separator_2_panel, 1, 40);

  GUI_Container__home__details_wind_cont =
      lv_obj_create(GUI_Container__home__details_cont);
  lv_obj_remove_style_all(GUI_Container__home__details_wind_cont);
  lv_obj_remove_flag(GUI_Container__home__details_wind_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__details_wind_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__details_wind_cont, 1);
  lv_obj_set_align(GUI_Container__home__details_wind_cont, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__details_wind_cont, 40, 40);
  lv_obj_set_height(GUI_Container__home__details_wind_cont, 50);

  GUI_Label__home__details_wind_title_lbl =
      lv_label_create(GUI_Container__home__details_wind_cont);
  lv_label_set_long_mode(GUI_Label__home__details_wind_title_lbl,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__details_wind_title_lbl, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__details_wind_title_lbl, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__CURRENTWINDQ =
      lv_obj_create(GUI_Container__home__details_wind_cont);
  lv_obj_remove_style_all(GUI_Container__home__CURRENTWINDQ);
  lv_obj_remove_flag(GUI_Container__home__CURRENTWINDQ, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__CURRENTWINDQ, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__CURRENTWINDQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__home__CURRENTWINDQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__details_wind_speed_lbl =
      lv_label_create(GUI_Container__home__CURRENTWINDQ);
  lv_label_set_long_mode(GUI_Label__home__details_wind_speed_lbl,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__details_wind_speed_lbl, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__details_wind_speed_lbl, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__hourly_forecast_cont =
      lv_obj_create(GUI_Panel__home__weather_panel);
  lv_obj_remove_style_all(GUI_Container__home__hourly_forecast_cont);
  lv_obj_remove_flag(GUI_Container__home__hourly_forecast_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__hourly_forecast_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__hourly_forecast_cont,
                   LV_ALIGN_LEFT_MID);
  lv_obj_set_pos(GUI_Container__home__hourly_forecast_cont, 7, 101);
  lv_obj_set_size(GUI_Container__home__hourly_forecast_cont, 215, 113);

  GUI_Container__home__hourly_forecast_title_cont =
      lv_obj_create(GUI_Container__home__hourly_forecast_cont);
  lv_obj_remove_style_all(GUI_Container__home__hourly_forecast_title_cont);
  lv_obj_remove_flag(GUI_Container__home__hourly_forecast_title_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__hourly_forecast_title_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__hourly_forecast_title_cont,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__home__hourly_forecast_title_cont, lv_pct(105),
                  13);

  GUI_Label__home__pollen_count_title =
      lv_label_create(GUI_Container__home__hourly_forecast_title_cont);
  lv_label_set_long_mode(GUI_Label__home__pollen_count_title,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__pollen_count_title, LV_ALIGN_LEFT_MID);
  lv_obj_set_pos(GUI_Label__home__pollen_count_title, 21, 25);
  lv_obj_set_size(GUI_Label__home__pollen_count_title, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__CHANCEOFRAINQ =
      lv_label_create(GUI_Container__home__hourly_forecast_title_cont);
  lv_label_set_long_mode(GUI_Label__home__CHANCEOFRAINQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__CHANCEOFRAINQ, LV_ALIGN_LEFT_MID);
  lv_obj_set_pos(GUI_Label__home__CHANCEOFRAINQ, 41, 45);
  lv_obj_set_size(GUI_Label__home__CHANCEOFRAINQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__hourly_forecast_items_cont =
      lv_obj_create(GUI_Container__home__hourly_forecast_cont);
  lv_obj_remove_style_all(GUI_Container__home__hourly_forecast_items_cont);
  lv_obj_remove_flag(GUI_Container__home__hourly_forecast_items_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__hourly_forecast_items_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__hourly_forecast_items_cont, 1);
  lv_obj_set_align(GUI_Container__home__hourly_forecast_items_cont,
                   LV_ALIGN_CENTER);
  lv_obj_set_width(GUI_Container__home__hourly_forecast_items_cont,
                   lv_pct(100));

  GUI_Container__home__HR1FORCASTCONT =
      lv_obj_create(GUI_Container__home__hourly_forecast_items_cont);
  lv_obj_remove_style_all(GUI_Container__home__HR1FORCASTCONT);
  lv_obj_remove_flag(GUI_Container__home__HR1FORCASTCONT,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__HR1FORCASTCONT,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__HR1FORCASTCONT, 1);
  lv_obj_set_align(GUI_Container__home__HR1FORCASTCONT, LV_ALIGN_CENTER);
  lv_obj_set_height(GUI_Container__home__HR1FORCASTCONT, LV_SIZE_CONTENT);

  GUI_Label__home__HR1FORCASTTIMEQ =
      lv_label_create(GUI_Container__home__HR1FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__HR1FORCASTTIMEQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__HR1FORCASTTIMEQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__HR1FORCASTTIMEQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__home__HR1FORCASICON22X22Q =
      lv_image_create(GUI_Container__home__HR1FORCASTCONT);
  lv_obj_add_flag(GUI_Image__home__HR1FORCASICON22X22Q,
                  LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__HR1FORCASICON22X22Q,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__HR1FORCASICON22X22Q, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__home__HR1FORCASICON22X22Q, lv_pct(58), 22);

  GUI_Label__home__HR1FORCASTTEMPQ =
      lv_label_create(GUI_Container__home__HR1FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__HR1FORCASTTEMPQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__HR1FORCASTTEMPQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__HR1FORCASTTEMPQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__HR1FORCASTTEMPQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__HR2FORCASTCONT =
      lv_obj_create(GUI_Container__home__hourly_forecast_items_cont);
  lv_obj_remove_style_all(GUI_Container__home__HR2FORCASTCONT);
  lv_obj_remove_flag(GUI_Container__home__HR2FORCASTCONT,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__HR2FORCASTCONT,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__HR2FORCASTCONT, 1);
  lv_obj_set_align(GUI_Container__home__HR2FORCASTCONT, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__HR2FORCASTCONT, 20, 20);
  lv_obj_set_height(GUI_Container__home__HR2FORCASTCONT, LV_SIZE_CONTENT);

  GUI_Label__home__HR2FORCASTTIMEQ =
      lv_label_create(GUI_Container__home__HR2FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__HR2FORCASTTIMEQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__HR2FORCASTTIMEQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__HR2FORCASTTIMEQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__home__HR2FORCASTICON22X22Q =
      lv_image_create(GUI_Container__home__HR2FORCASTCONT);
  lv_obj_add_flag(GUI_Image__home__HR2FORCASTICON22X22Q,
                  LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__HR2FORCASTICON22X22Q,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__HR2FORCASTICON22X22Q, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__home__HR2FORCASTICON22X22Q, 22, 22);

  GUI_Label__home__HR2FORCASTTEMPQ =
      lv_label_create(GUI_Container__home__HR2FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__HR2FORCASTTEMPQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__HR2FORCASTTEMPQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__HR2FORCASTTEMPQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__HR2FORCASTTEMPQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__HR3FORCASTCONT =
      lv_obj_create(GUI_Container__home__hourly_forecast_items_cont);
  lv_obj_remove_style_all(GUI_Container__home__HR3FORCASTCONT);
  lv_obj_remove_flag(GUI_Container__home__HR3FORCASTCONT,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__HR3FORCASTCONT,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__HR3FORCASTCONT, 1);
  lv_obj_set_align(GUI_Container__home__HR3FORCASTCONT, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__HR3FORCASTCONT, 40, 40);
  lv_obj_set_height(GUI_Container__home__HR3FORCASTCONT, LV_SIZE_CONTENT);

  GUI_Label__home__HR3FORCASTTIMEQ =
      lv_label_create(GUI_Container__home__HR3FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__HR3FORCASTTIMEQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__HR3FORCASTTIMEQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__HR3FORCASTTIMEQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__home__HR3FORCASTICON22X22Q =
      lv_image_create(GUI_Container__home__HR3FORCASTCONT);
  lv_obj_add_flag(GUI_Image__home__HR3FORCASTICON22X22Q,
                  LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__HR3FORCASTICON22X22Q,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__HR3FORCASTICON22X22Q, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__home__HR3FORCASTICON22X22Q, 22, 22);

  GUI_Label__home__HR3FORCASTTEMPQ =
      lv_label_create(GUI_Container__home__HR3FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__HR3FORCASTTEMPQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__HR3FORCASTTEMPQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__HR3FORCASTTEMPQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__HR3FORCASTTEMPQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__HR4FORCASTCONT =
      lv_obj_create(GUI_Container__home__hourly_forecast_items_cont);
  lv_obj_remove_style_all(GUI_Container__home__HR4FORCASTCONT);
  lv_obj_remove_flag(GUI_Container__home__HR4FORCASTCONT,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__HR4FORCASTCONT,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__HR4FORCASTCONT, 1);
  lv_obj_set_align(GUI_Container__home__HR4FORCASTCONT, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__HR4FORCASTCONT, 60, 60);
  lv_obj_set_height(GUI_Container__home__HR4FORCASTCONT, LV_SIZE_CONTENT);

  GUI_Label__home__HR4FORCASTTIMEQ =
      lv_label_create(GUI_Container__home__HR4FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__HR4FORCASTTIMEQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__HR4FORCASTTIMEQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__HR4FORCASTTIMEQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__home__HR4FORCASTICON22X22Q =
      lv_image_create(GUI_Container__home__HR4FORCASTCONT);
  lv_obj_add_flag(GUI_Image__home__HR4FORCASTICON22X22Q,
                  LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__HR4FORCASTICON22X22Q,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__HR4FORCASTICON22X22Q, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__home__HR4FORCASTICON22X22Q, 22, 22);

  GUI_Label__home__HR4FORCASTTEMPQ =
      lv_label_create(GUI_Container__home__HR4FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__HR4FORCASTTEMPQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__HR4FORCASTTEMPQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__HR4FORCASTTEMPQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__HR4FORCASTTEMPQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__HR5FORCASTCONT =
      lv_obj_create(GUI_Container__home__hourly_forecast_items_cont);
  lv_obj_remove_style_all(GUI_Container__home__HR5FORCASTCONT);
  lv_obj_remove_flag(GUI_Container__home__HR5FORCASTCONT,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__HR5FORCASTCONT,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__HR5FORCASTCONT, 1);
  lv_obj_set_align(GUI_Container__home__HR5FORCASTCONT, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__HR5FORCASTCONT, 80, 80);
  lv_obj_set_height(GUI_Container__home__HR5FORCASTCONT, LV_SIZE_CONTENT);

  GUI_Label__home__HR5FORCASTTIMEQ =
      lv_label_create(GUI_Container__home__HR5FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__HR5FORCASTTIMEQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__HR5FORCASTTIMEQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__HR5FORCASTTIMEQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__home__HR5FORCASTICON22X22Q =
      lv_image_create(GUI_Container__home__HR5FORCASTCONT);
  lv_obj_add_flag(GUI_Image__home__HR5FORCASTICON22X22Q,
                  LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__HR5FORCASTICON22X22Q,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__HR5FORCASTICON22X22Q, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__home__HR5FORCASTICON22X22Q, 22, 22);

  GUI_Label__home__HR5FORCASTTEMPQ =
      lv_label_create(GUI_Container__home__HR5FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__HR5FORCASTTEMPQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__HR5FORCASTTEMPQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__HR5FORCASTTEMPQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__HR5FORCASTTEMPQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__daily_forecast_cont =
      lv_obj_create(GUI_Container__home__weatherwidget);
  lv_obj_remove_style_all(GUI_Container__home__daily_forecast_cont);
  lv_obj_remove_flag(GUI_Container__home__daily_forecast_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__daily_forecast_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__daily_forecast_cont,
                   LV_ALIGN_BOTTOM_LEFT);
  lv_obj_set_pos(GUI_Container__home__daily_forecast_cont, 2, 3);
  lv_obj_set_size(GUI_Container__home__daily_forecast_cont, 229, 109);

  GUI_Container__home__daily_forecast_items_cont =
      lv_obj_create(GUI_Container__home__daily_forecast_cont);
  lv_obj_remove_style_all(GUI_Container__home__daily_forecast_items_cont);
  lv_obj_remove_flag(GUI_Container__home__daily_forecast_items_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__daily_forecast_items_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__daily_forecast_items_cont, 1);
  lv_obj_set_align(GUI_Container__home__daily_forecast_items_cont,
                   LV_ALIGN_CENTER);
  lv_obj_set_width(GUI_Container__home__daily_forecast_items_cont, lv_pct(98));

  GUI_Container__home__DAYLY1FORCASTCONT =
      lv_obj_create(GUI_Container__home__daily_forecast_items_cont);
  lv_obj_remove_style_all(GUI_Container__home__DAYLY1FORCASTCONT);
  lv_obj_remove_flag(GUI_Container__home__DAYLY1FORCASTCONT,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__DAYLY1FORCASTCONT,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__DAYLY1FORCASTCONT, 1);
  lv_obj_set_align(GUI_Container__home__DAYLY1FORCASTCONT, LV_ALIGN_CENTER);
  lv_obj_set_height(GUI_Container__home__DAYLY1FORCASTCONT, lv_pct(100));

  GUI_Panel__home__daily_forecast_1_item_data_panel =
      lv_obj_create(GUI_Container__home__DAYLY1FORCASTCONT);
  lv_obj_remove_flag(GUI_Panel__home__daily_forecast_1_item_data_panel,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__home__daily_forecast_1_item_data_panel,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Panel__home__daily_forecast_1_item_data_panel,
                  lv_pct(100), 58);

  GUI_Label__home__DAYLY1FORCASTDAYQ =
      lv_label_create(GUI_Panel__home__daily_forecast_1_item_data_panel);
  lv_label_set_long_mode(GUI_Label__home__DAYLY1FORCASTDAYQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY1FORCASTDAYQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY1FORCASTDAYQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__DAYLY1FORCASTDAYQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ =
      lv_label_create(GUI_Panel__home__daily_forecast_1_item_data_panel);
  lv_label_set_long_mode(GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ, 40, 40);
  lv_obj_set_size(GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY1FORCASTTEMPLOWQ =
      lv_label_create(GUI_Panel__home__daily_forecast_1_item_data_panel);
  lv_label_set_long_mode(GUI_Label__home__DAYLY1FORCASTTEMPLOWQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY1FORCASTTEMPLOWQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY1FORCASTTEMPLOWQ, 60, 60);
  lv_obj_set_size(GUI_Label__home__DAYLY1FORCASTTEMPLOWQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY1FORCASTSTATUSQ =
      lv_label_create(GUI_Container__home__DAYLY1FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY1FORCASTSTATUSQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY1FORCASTSTATUSQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY1FORCASTSTATUSQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__DAYLY1FORCASTSTATUSQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Panel__home__details_separator_1_panel_2 =
      lv_obj_create(GUI_Container__home__daily_forecast_items_cont);
  lv_obj_remove_flag(GUI_Panel__home__details_separator_1_panel_2,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__home__details_separator_1_panel_2,
                   LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Panel__home__details_separator_1_panel_2, 20, 20);
  lv_obj_set_size(GUI_Panel__home__details_separator_1_panel_2, 1, 70);

  GUI_Container__home__DAYLY2FORCASTCONT =
      lv_obj_create(GUI_Container__home__daily_forecast_items_cont);
  lv_obj_remove_style_all(GUI_Container__home__DAYLY2FORCASTCONT);
  lv_obj_remove_flag(GUI_Container__home__DAYLY2FORCASTCONT,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__DAYLY2FORCASTCONT,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__DAYLY2FORCASTCONT, 1);
  lv_obj_set_align(GUI_Container__home__DAYLY2FORCASTCONT, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__DAYLY2FORCASTCONT, 20, 20);
  lv_obj_set_height(GUI_Container__home__DAYLY2FORCASTCONT, lv_pct(100));

  GUI_Panel__home__daily_forecast_tuesday_item_data_panel =
      lv_obj_create(GUI_Container__home__DAYLY2FORCASTCONT);
  lv_obj_remove_flag(GUI_Panel__home__daily_forecast_tuesday_item_data_panel,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__home__daily_forecast_tuesday_item_data_panel,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Panel__home__daily_forecast_tuesday_item_data_panel,
                  lv_pct(100), 58);

  GUI_Label__home__DAYLY2FORCASTDAYQ =
      lv_label_create(GUI_Panel__home__daily_forecast_tuesday_item_data_panel);
  lv_label_set_long_mode(GUI_Label__home__DAYLY2FORCASTDAYQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY2FORCASTDAYQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY2FORCASTDAYQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__DAYLY2FORCASTDAYQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ =
      lv_label_create(GUI_Panel__home__daily_forecast_tuesday_item_data_panel);
  lv_label_set_long_mode(GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ, 40, 40);
  lv_obj_set_size(GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY2FORCASTTEMPLOWQ =
      lv_label_create(GUI_Panel__home__daily_forecast_tuesday_item_data_panel);
  lv_label_set_long_mode(GUI_Label__home__DAYLY2FORCASTTEMPLOWQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY2FORCASTTEMPLOWQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY2FORCASTTEMPLOWQ, 60, 60);
  lv_obj_set_size(GUI_Label__home__DAYLY2FORCASTTEMPLOWQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY2FORCASTSTATUSQ =
      lv_label_create(GUI_Container__home__DAYLY2FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY2FORCASTSTATUSQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY2FORCASTSTATUSQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY2FORCASTSTATUSQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__DAYLY2FORCASTSTATUSQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Panel__home__details_separator_1_panel_1 =
      lv_obj_create(GUI_Container__home__daily_forecast_items_cont);
  lv_obj_remove_flag(GUI_Panel__home__details_separator_1_panel_1,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__home__details_separator_1_panel_1,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Panel__home__details_separator_1_panel_1, 1, 70);

  GUI_Container__home__DAYLY3FORCASTCONT =
      lv_obj_create(GUI_Container__home__daily_forecast_items_cont);
  lv_obj_remove_style_all(GUI_Container__home__DAYLY3FORCASTCONT);
  lv_obj_remove_flag(GUI_Container__home__DAYLY3FORCASTCONT,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__DAYLY3FORCASTCONT,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_grow(GUI_Container__home__DAYLY3FORCASTCONT, 1);
  lv_obj_set_align(GUI_Container__home__DAYLY3FORCASTCONT, LV_ALIGN_CENTER);
  lv_obj_set_height(GUI_Container__home__DAYLY3FORCASTCONT, lv_pct(100));

  GUI_Panel__home__daily_forecast_wednesday_item_data_panel =
      lv_obj_create(GUI_Container__home__DAYLY3FORCASTCONT);
  lv_obj_remove_flag(GUI_Panel__home__daily_forecast_wednesday_item_data_panel,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__home__daily_forecast_wednesday_item_data_panel,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Panel__home__daily_forecast_wednesday_item_data_panel,
                  lv_pct(100), 58);

  GUI_Label__home__DAYLY3FORCASTDAYQ = lv_label_create(
      GUI_Panel__home__daily_forecast_wednesday_item_data_panel);
  lv_label_set_long_mode(GUI_Label__home__DAYLY3FORCASTDAYQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY3FORCASTDAYQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY3FORCASTDAYQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__DAYLY3FORCASTDAYQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ = lv_label_create(
      GUI_Panel__home__daily_forecast_wednesday_item_data_panel);
  lv_label_set_long_mode(GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ, 40, 40);
  lv_obj_set_size(GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY3FORCASTTEMPLOWQ = lv_label_create(
      GUI_Panel__home__daily_forecast_wednesday_item_data_panel);
  lv_label_set_long_mode(GUI_Label__home__DAYLY3FORCASTTEMPLOWQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY3FORCASTTEMPLOWQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY3FORCASTTEMPLOWQ, 60, 60);
  lv_obj_set_size(GUI_Label__home__DAYLY3FORCASTTEMPLOWQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY3FORCASTSTATUSQ =
      lv_label_create(GUI_Container__home__DAYLY3FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY3FORCASTSTATUSQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY3FORCASTSTATUSQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__DAYLY3FORCASTSTATUSQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__DAYLY3FORCASTSTATUSQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__NETWORKSTAUSANDMENUCONT =
      lv_obj_create(GUI_Screen__home);
  lv_obj_remove_style_all(GUI_Container__home__NETWORKSTAUSANDMENUCONT);
  lv_obj_remove_flag(GUI_Container__home__NETWORKSTAUSANDMENUCONT,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__NETWORKSTAUSANDMENUCONT,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__NETWORKSTAUSANDMENUCONT,
                   LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__NETWORKSTAUSANDMENUCONT, 385, -237);
  lv_obj_set_size(GUI_Container__home__NETWORKSTAUSANDMENUCONT, 122, 90);

  GUI_Panel__home__panel =
      lv_obj_create(GUI_Container__home__NETWORKSTAUSANDMENUCONT);
  lv_obj_remove_flag(GUI_Panel__home__panel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__home__panel, LV_ALIGN_TOP_RIGHT);
  lv_obj_set_pos(GUI_Panel__home__panel, -12, 8);
  lv_obj_set_size(GUI_Panel__home__panel, 94, 63);

  GUI_Image__home__NETWORKCONNECTEDICONQ =
      lv_image_create(GUI_Container__home__NETWORKSTAUSANDMENUCONT);
  lv_obj_add_flag(GUI_Image__home__NETWORKCONNECTEDICONQ,
                  LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__NETWORKCONNECTEDICONQ,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__NETWORKCONNECTEDICONQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Image__home__NETWORKCONNECTEDICONQ, -30, 11);
  lv_obj_set_size(GUI_Image__home__NETWORKCONNECTEDICONQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__home__SETTINGSBUTTON =
      lv_image_create(GUI_Container__home__NETWORKSTAUSANDMENUCONT);
  lv_obj_add_flag(GUI_Image__home__SETTINGSBUTTON, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__SETTINGSBUTTON, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__SETTINGSBUTTON, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Image__home__SETTINGSBUTTON, -7, 12);
  lv_obj_set_size(GUI_Image__home__SETTINGSBUTTON, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);
  lv_obj_add_event_cb(GUI_Image__home__SETTINGSBUTTON,
                      GUI_event__Image__home__SETTINGSBUTTON__Clicked,
                      LV_EVENT_CLICKED, NULL);

  GUI_initScreenStyles__home();
  GUI_initScreenTexts__home();
}

void GUI_initScreenTexts__home() {
  lv_label_set_text(GUI_Label__home__CURRENTTIMEQ, "10:34PM");
  lv_label_set_text(GUI_Label__home__main_display_title_lbl, "Weather Now");
  lv_label_set_text(GUI_Label__home__LOCATIONQ, "City, state");
  lv_label_set_text(GUI_Label__home__CURRENTTEMPQ, "115°");
  lv_label_set_text(GUI_Label__home__CURRENTSTATUSQ, "Partly\nCloudy");
  lv_label_set_text(GUI_Label__home__details_feels_like_title_lbl,
                    "Feels Like");
  lv_label_set_text(GUI_Label__home__FEELSLIKETEMPQ, "113°");
  lv_label_set_text(GUI_Label__home__details_humidity_title_lbl, "Humidity");
  lv_label_set_text(GUI_Label__home__HUMIDITYPERCENTQ, "63%");
  lv_label_set_text(GUI_Label__home__details_wind_title_lbl, "  Wind-mph");
  lv_label_set_text(GUI_Label__home__details_wind_speed_lbl, "12");
  lv_label_set_text(GUI_Label__home__pollen_count_title, "CHANCE OF RAIN");
  lv_label_set_text(GUI_Label__home__CHANCEOFRAINQ, "50%");
  lv_label_set_text(GUI_Label__home__HR1FORCASTTIMEQ, "7PM");
  lv_label_set_text(GUI_Label__home__HR1FORCASTTEMPQ, "115°");
  lv_label_set_text(GUI_Label__home__HR2FORCASTTIMEQ, "8PM");
  lv_label_set_text(GUI_Label__home__HR2FORCASTTEMPQ, "116°");
  lv_label_set_text(GUI_Label__home__HR3FORCASTTIMEQ, "9PM");
  lv_label_set_text(GUI_Label__home__HR3FORCASTTEMPQ, "117°");
  lv_label_set_text(GUI_Label__home__HR4FORCASTTIMEQ, "10PM");
  lv_label_set_text(GUI_Label__home__HR4FORCASTTEMPQ, "118°");
  lv_label_set_text(GUI_Label__home__HR5FORCASTTIMEQ, "11PM");
  lv_label_set_text(GUI_Label__home__HR5FORCASTTEMPQ, "119°");
  lv_label_set_text(GUI_Label__home__DAYLY1FORCASTDAYQ, "Mon");
  lv_label_set_text(GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ, "115°");
  lv_label_set_text(GUI_Label__home__DAYLY1FORCASTTEMPLOWQ, "115°");
  lv_label_set_text(GUI_Label__home__DAYLY1FORCASTSTATUSQ, "CLOUDY\nCLOUDY");
  lv_label_set_text(GUI_Label__home__DAYLY2FORCASTDAYQ, "Tue");
  lv_label_set_text(GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ, "115°");
  lv_label_set_text(GUI_Label__home__DAYLY2FORCASTTEMPLOWQ, "115°");
  lv_label_set_text(GUI_Label__home__DAYLY2FORCASTSTATUSQ, "SUNNY\nSUNNY");
  lv_label_set_text(GUI_Label__home__DAYLY3FORCASTDAYQ, "Wed");
  lv_label_set_text(GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ, "115°");
  lv_label_set_text(GUI_Label__home__DAYLY3FORCASTTEMPLOWQ, "115°");
  lv_label_set_text(GUI_Label__home__DAYLY3FORCASTSTATUSQ, "RAIN\nRAIN");
}

void GUI_initScreenStyles__home() {
  lv_obj_add_style(GUI_Screen__home, &GUI_Style__class_8rQJj2WR7KS5BW__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Panel__home__weather_panel,
                   &GUI_Style__class_HxADxF6juDKWfB__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__home__main_display_cont,
                   &GUI_Style__class_l2KF3gLqvrvgNA__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__CURRENTTIMEQ,
                   &GUI_Style__class_QPHwtUHlUxUOEn__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__CURRENTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__CURRENTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Label__home__main_display_title_lbl,
                   &GUI_Style__class_l05DXbHrWKpOYP__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__main_display_title_lbl, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__main_display_title_lbl, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Label__home__LOCATIONQ,
                   &GUI_Style__class_lp2SW4VTZ90VZp__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__LOCATIONQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__LOCATIONQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Label__home__CURRENTTEMPQ,
                   &GUI_Style__class_S38iEdXYHp1zFH__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__CURRENTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__CURRENTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Container__home__current_status_cont,
                   &GUI_Style__class_kL3MCphSm1zdhB__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_image_set_src(GUI_Image__home__60X60ICONQ,
                   &upload_1_7fa59a4c14414e4ab42f8f8cf8ee5df8_png);

  lv_obj_add_style(GUI_Image__home__60X60ICONQ,
                   &GUI_Style__class_ouIAe8vEK7tLAC__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__CURRENTSTATUSQ,
                   &GUI_Style__class_OWp7o3fr8u7nZG__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__CURRENTSTATUSQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__CURRENTSTATUSQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Container__home__details_cont,
                   &GUI_Style__class_dlCXVJg3k6UwKj__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__home__details_feels_like_cont,
                   &GUI_Style__class_UthKrP17uaRpGr__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__details_feels_like_title_lbl,
                   &GUI_Style__class_7TxYpcbJKqrqzb__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__details_feels_like_title_lbl,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
      _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__details_feels_like_title_lbl,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
      _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Label__home__FEELSLIKETEMPQ,
                   &GUI_Style__class_fMZLS649q9lJQi__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__FEELSLIKETEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__FEELSLIKETEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Panel__home__details_separator_1_panel,
                   &GUI_Style__class_Jw5auITeCyGC1B__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
      _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_bg_opa_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_COLOR,
      _ui_theme_grad_color_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_OPA,
      _ui_theme_grad_opa_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_DIR,
      _ui_theme_grad_dir_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_MAIN_STOP,
      _ui_theme_main_stop_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_STOP,
      _ui_theme_grad_stop_white);

  lv_obj_add_style(GUI_Container__home__details_humidity_cont,
                   &GUI_Style__class_3kssyjEdbcCLSh__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__details_humidity_title_lbl,
                   &GUI_Style__class_JC5FT5PPZmgeAd__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__details_humidity_title_lbl,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
      _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__details_humidity_title_lbl,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
      _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Label__home__HUMIDITYPERCENTQ,
                   &GUI_Style__class_z6I6jhPHmAziTD__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HUMIDITYPERCENTQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HUMIDITYPERCENTQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Panel__home__details_separator_2_panel,
                   &GUI_Style__class_u3dHPTQfScNGqf__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_2_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
      _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_2_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_bg_opa_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_2_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_COLOR,
      _ui_theme_grad_color_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_2_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_OPA,
      _ui_theme_grad_opa_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_2_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_DIR,
      _ui_theme_grad_dir_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_2_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_MAIN_STOP,
      _ui_theme_main_stop_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_2_panel,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_STOP,
      _ui_theme_grad_stop_white);

  lv_obj_add_style(GUI_Container__home__details_wind_cont,
                   &GUI_Style__class_Kv9aIdJXPzd4E0__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__details_wind_title_lbl,
                   &GUI_Style__class_d6oNFVfxUQNLTp__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__details_wind_title_lbl, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__details_wind_title_lbl, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Container__home__CURRENTWINDQ,
                   &GUI_Style__class_330bxbZF7kZsG8__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__details_wind_speed_lbl,
                   &GUI_Style__class_aB5O6v8av05RG3__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__details_wind_speed_lbl, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__details_wind_speed_lbl, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Container__home__hourly_forecast_cont,
                   &GUI_Style__class_E4wegkc5VJFKsz__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__home__hourly_forecast_title_cont,
                   &GUI_Style__class_txXxHpeZgsni8H__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__pollen_count_title,
                   &GUI_Style__class_ePErXDcnIvioYq__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__pollen_count_title, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__pollen_count_title, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Label__home__CHANCEOFRAINQ,
                   &GUI_Style__class_orAle4UexZ0qwV__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__CHANCEOFRAINQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__CHANCEOFRAINQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Container__home__hourly_forecast_items_cont,
                   &GUI_Style__class_NHMYIepongiLxJ__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__home__HR1FORCASTCONT,
                   &GUI_Style__class_bnXaTUcRtszjic__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__HR1FORCASTTIMEQ,
                   &GUI_Style__class_e1RJqKV5YEtyLL__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_image_set_src(GUI_Image__home__HR1FORCASICON22X22Q,
                   &upload_1s_a942bd7a65904cab870f8cf337226275_png);

  lv_obj_add_style(GUI_Image__home__HR1FORCASICON22X22Q,
                   &GUI_Style__class_UwhUwo9ljeO8lt__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__HR1FORCASTTEMPQ,
                   &GUI_Style__class_z1zvszPSytj7GK__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR1FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR1FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Container__home__HR2FORCASTCONT,
                   &GUI_Style__class_SqnJ2yTfN70m5F__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__HR2FORCASTTIMEQ,
                   &GUI_Style__class_Sl2id12bl9NQMv__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR2FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR2FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_image_set_src(GUI_Image__home__HR2FORCASTICON22X22Q,
                   &upload_1s_a942bd7a65904cab870f8cf337226275_png);

  lv_obj_add_style(GUI_Label__home__HR2FORCASTTEMPQ,
                   &GUI_Style__class_Yoie1JkY7ut7dR__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR2FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR2FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Container__home__HR3FORCASTCONT,
                   &GUI_Style__class_9V6WvrW17HbJD1__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__HR3FORCASTTIMEQ,
                   &GUI_Style__class_uctqe13XSbRVrf__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR3FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR3FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_image_set_src(GUI_Image__home__HR3FORCASTICON22X22Q,
                   &upload_1s_a942bd7a65904cab870f8cf337226275_png);

  lv_obj_add_style(GUI_Label__home__HR3FORCASTTEMPQ,
                   &GUI_Style__class_CrXq0J1lzKaWzo__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR3FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR3FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Container__home__HR4FORCASTCONT,
                   &GUI_Style__class_yL6megXhm9GYb7__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__HR4FORCASTTIMEQ,
                   &GUI_Style__class_4FP9fwNC3Jfkoc__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR4FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR4FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_image_set_src(GUI_Image__home__HR4FORCASTICON22X22Q,
                   &upload_1s_a942bd7a65904cab870f8cf337226275_png);

  lv_obj_add_style(GUI_Label__home__HR4FORCASTTEMPQ,
                   &GUI_Style__class_wWU5ITomt2XhrC__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR4FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR4FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Container__home__HR5FORCASTCONT,
                   &GUI_Style__class_u9F6wP0qFMftID__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__HR5FORCASTTIMEQ,
                   &GUI_Style__class_zkJojhqYN6amU5__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR5FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR5FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_image_set_src(GUI_Image__home__HR5FORCASTICON22X22Q,
                   &upload_1s_a942bd7a65904cab870f8cf337226275_png);

  lv_obj_add_style(GUI_Label__home__HR5FORCASTTEMPQ,
                   &GUI_Style__class_zMg69aamtHeGHF__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR5FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR5FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Container__home__daily_forecast_cont,
                   &GUI_Style__class_N6QjKxi1tUPVcv__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__home__daily_forecast_items_cont,
                   &GUI_Style__class_WhQHp8mtjRM1Z6__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__home__DAYLY1FORCASTCONT,
                   &GUI_Style__class_6Zqyl1PAzGby72__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Panel__home__daily_forecast_1_item_data_panel,
                   &GUI_Style__class_P20EIfA3luXdY3__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY1FORCASTDAYQ,
                   &GUI_Style__class_mGTxkagT6y6pNF__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY1FORCASTDAYQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY1FORCASTDAYQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ,
                   &GUI_Style__class_QtBH9YSEMkGaW1__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY1FORCASTTEMPLOWQ,
                   &GUI_Style__class_9MpPQcvJSEks3y__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY1FORCASTSTATUSQ,
                   &GUI_Style__class_wlkSJFhc3canUg__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY1FORCASTSTATUSQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY1FORCASTSTATUSQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Panel__home__details_separator_1_panel_2,
                   &GUI_Style__class_OvMbXZAsPbZjnb__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_2,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
      _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_2,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_bg_opa_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_2,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_COLOR,
      _ui_theme_grad_color_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_2,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_OPA,
      _ui_theme_grad_opa_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_2,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_DIR,
      _ui_theme_grad_dir_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_2,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_MAIN_STOP,
      _ui_theme_main_stop_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_2,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_STOP,
      _ui_theme_grad_stop_white);

  lv_obj_add_style(GUI_Container__home__DAYLY2FORCASTCONT,
                   &GUI_Style__class_E4t2gqcHR8MbAP__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Panel__home__daily_forecast_tuesday_item_data_panel,
                   &GUI_Style__class_XwvnqhCeV2aoM1__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY2FORCASTDAYQ,
                   &GUI_Style__class_1w85YzMTP9d5q4__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY2FORCASTDAYQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY2FORCASTDAYQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ,
                   &GUI_Style__class_j3PIJOujL57xFq__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY2FORCASTTEMPLOWQ,
                   &GUI_Style__class_wKOu7L7fJpWUc8__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY2FORCASTSTATUSQ,
                   &GUI_Style__class_DaDvAetiVI0Sy4__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY2FORCASTSTATUSQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY2FORCASTSTATUSQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Panel__home__details_separator_1_panel_1,
                   &GUI_Style__class_HZ1hQzhR4glwPE__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_1,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
      _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_1,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_bg_opa_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_1,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_COLOR,
      _ui_theme_grad_color_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_1,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_OPA,
      _ui_theme_grad_opa_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_1,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_DIR,
      _ui_theme_grad_dir_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_1,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_MAIN_STOP,
      _ui_theme_main_stop_white);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__details_separator_1_panel_1,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_STOP,
      _ui_theme_grad_stop_white);

  lv_obj_add_style(GUI_Container__home__DAYLY3FORCASTCONT,
                   &GUI_Style__class_sI3TdUhM0WS1Zb__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Panel__home__daily_forecast_wednesday_item_data_panel,
                   &GUI_Style__class_f9uFiY7jL0YFTZ__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY3FORCASTDAYQ,
                   &GUI_Style__class_tpmJOiEkbfWCQ5__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY3FORCASTDAYQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY3FORCASTDAYQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ,
                   &GUI_Style__class_NqL7LHebj5jnE1__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY3FORCASTTEMPLOWQ,
                   &GUI_Style__class_5aC5gkRCTJRBpX__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY3FORCASTSTATUSQ,
                   &GUI_Style__class_MNThGQl2quXcmK__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY3FORCASTSTATUSQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY3FORCASTSTATUSQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Panel__home__panel, &GUI_Style__class_5TGhYvIVJqeKbz__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_image_set_src(GUI_Image__home__NETWORKCONNECTEDICONQ,
                   &upload_network_e6b6f98e6cbd4491bfe5310d621d416f_png);

  lv_obj_add_style(GUI_Image__home__NETWORKCONNECTEDICONQ,
                   &GUI_Style__class_q2R0whE4QSYoQW__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_image_set_src(GUI_Image__home__SETTINGSBUTTON,
                   &upload_menusettings_ac03cce753704d6fa240fb7acbd315c1_png);
}
