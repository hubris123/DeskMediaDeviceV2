#include "../GUI.h"

void GUI_initScreen__home() {
  GUI_Screen__home = lv_obj_create(NULL);
  lv_obj_remove_flag(GUI_Screen__home, LV_OBJ_FLAG_SCROLLABLE);
  GUI_Container__home__container_13 = lv_obj_create(GUI_Screen__home);
  lv_obj_remove_style_all(GUI_Container__home__container_13);
  lv_obj_remove_flag(GUI_Container__home__container_13, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__container_13, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__container_13, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__home__container_13, 800, 480);

  GUI_Container__home__weatherwidget = lv_obj_create(GUI_Screen__home);
  lv_obj_remove_style_all(GUI_Container__home__weatherwidget);
  lv_obj_remove_flag(GUI_Container__home__weatherwidget, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__weatherwidget,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__weatherwidget, LV_ALIGN_CENTER);
  lv_obj_set_x(GUI_Container__home__weatherwidget, -282);
  lv_obj_set_size(GUI_Container__home__weatherwidget, 248, 497);

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
  lv_obj_set_pos(GUI_Container__home__main_display_cont, -2, -161);
  lv_obj_set_size(GUI_Container__home__main_display_cont, 220, 143);

  GUI_Label__home__CURRENTTIMEQ =
      lv_label_create(GUI_Container__home__main_display_cont);
  lv_label_set_long_mode(GUI_Label__home__CURRENTTIMEQ, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__CURRENTTIMEQ, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__home__CURRENTTIMEQ, 20, 20);
  lv_obj_set_size(GUI_Label__home__CURRENTTIMEQ, LV_SIZE_CONTENT,
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
  lv_obj_set_pos(GUI_Container__home__current_status_cont, 32, 136);
  lv_obj_set_size(GUI_Container__home__current_status_cont, 173, 69);

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
  lv_obj_set_pos(GUI_Container__home__details_cont, -11, 188);
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

  GUI_Container__home__container_14 =
      lv_obj_create(GUI_Panel__home__weather_panel);
  lv_obj_remove_style_all(GUI_Container__home__container_14);
  lv_obj_remove_flag(GUI_Container__home__container_14, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__container_14, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__container_14, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__container_14, -1, 33);
  lv_obj_set_size(GUI_Container__home__container_14, 204, 24);

  GUI_Label__home__label_15 =
      lv_label_create(GUI_Container__home__container_14);
  lv_label_set_long_mode(GUI_Label__home__label_15, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__label_15, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__label_15, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Label__home__label_16 =
      lv_label_create(GUI_Container__home__container_14);
  lv_label_set_long_mode(GUI_Label__home__label_16, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__label_16, LV_ALIGN_CENTER);
  lv_obj_set_x(GUI_Label__home__label_16, 41);
  lv_obj_set_size(GUI_Label__home__label_16, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Container__home__hourly_forecast_cont =
      lv_obj_create(GUI_Panel__home__weather_panel);
  lv_obj_remove_style_all(GUI_Container__home__hourly_forecast_cont);
  lv_obj_remove_flag(GUI_Container__home__hourly_forecast_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__hourly_forecast_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__hourly_forecast_cont, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__hourly_forecast_cont, -2, 90);
  lv_obj_set_size(GUI_Container__home__hourly_forecast_cont, 214, 89);

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
                   lv_pct(113));

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

  GUI_Container__home__daily_forecast_cont =
      lv_obj_create(GUI_Panel__home__weather_panel);
  lv_obj_remove_style_all(GUI_Container__home__daily_forecast_cont);
  lv_obj_remove_flag(GUI_Container__home__daily_forecast_cont,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__daily_forecast_cont,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__daily_forecast_cont,
                   LV_ALIGN_BOTTOM_LEFT);
  lv_obj_set_pos(GUI_Container__home__daily_forecast_cont, -2, -3);
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

  GUI_Label__home__DAYLY1FORCASTDAYQ =
      lv_label_create(GUI_Container__home__DAYLY1FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY1FORCASTDAYQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY1FORCASTDAYQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__DAYLY1FORCASTDAYQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ =
      lv_label_create(GUI_Container__home__DAYLY1FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__DAYLY1FORCASTTEMPHIGHQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY1FORCASTTEMPLOWQ =
      lv_label_create(GUI_Container__home__DAYLY1FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY1FORCASTTEMPLOWQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY1FORCASTTEMPLOWQ, LV_ALIGN_CENTER);
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

  GUI_Label__home__DAYLY2FORCASTDAYQ =
      lv_label_create(GUI_Container__home__DAYLY2FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY2FORCASTDAYQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY2FORCASTDAYQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__DAYLY2FORCASTDAYQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ =
      lv_label_create(GUI_Container__home__DAYLY2FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__DAYLY2FORCASTTEMPHIGHQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY2FORCASTTEMPLOWQ =
      lv_label_create(GUI_Container__home__DAYLY2FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY2FORCASTTEMPLOWQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY2FORCASTTEMPLOWQ, LV_ALIGN_CENTER);
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

  GUI_Label__home__DAYLY3FORCASTDAYQ =
      lv_label_create(GUI_Container__home__DAYLY3FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY3FORCASTDAYQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY3FORCASTDAYQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__DAYLY3FORCASTDAYQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ =
      lv_label_create(GUI_Container__home__DAYLY3FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__home__DAYLY3FORCASTTEMPHIGHQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__home__DAYLY3FORCASTTEMPLOWQ =
      lv_label_create(GUI_Container__home__DAYLY3FORCASTCONT);
  lv_label_set_long_mode(GUI_Label__home__DAYLY3FORCASTTEMPLOWQ,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__home__DAYLY3FORCASTTEMPLOWQ, LV_ALIGN_CENTER);
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
  lv_obj_set_pos(GUI_Container__home__NETWORKSTAUSANDMENUCONT, 375, -220);
  lv_obj_set_size(GUI_Container__home__NETWORKSTAUSANDMENUCONT, 140, 88);

  GUI_Panel__home__panel =
      lv_obj_create(GUI_Container__home__NETWORKSTAUSANDMENUCONT);
  lv_obj_remove_flag(GUI_Panel__home__panel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__home__panel, LV_ALIGN_TOP_RIGHT);
  lv_obj_set_pos(GUI_Panel__home__panel, -38, 19);
  lv_obj_set_size(GUI_Panel__home__panel, 83, 49);

  GUI_Image__home__NETWORKCONNECTEDICONQ =
      lv_image_create(GUI_Panel__home__panel);
  lv_obj_add_flag(GUI_Image__home__NETWORKCONNECTEDICONQ,
                  LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__NETWORKCONNECTEDICONQ,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__NETWORKCONNECTEDICONQ, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__home__NETWORKCONNECTEDICONQ, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Image__home__SETTINGSBUTTON = lv_image_create(GUI_Panel__home__panel);
  lv_obj_add_flag(GUI_Image__home__SETTINGSBUTTON, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__SETTINGSBUTTON, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__SETTINGSBUTTON, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Image__home__SETTINGSBUTTON, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__home__music_player = lv_obj_create(GUI_Screen__home);
  lv_obj_remove_style_all(GUI_Container__home__music_player);
  lv_obj_remove_flag(GUI_Container__home__music_player, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__music_player, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__music_player, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__music_player, -132, 115);
  lv_obj_set_size(GUI_Container__home__music_player, 77, 242);

  GUI_Panel__home__panel_3 = lv_obj_create(GUI_Container__home__music_player);
  lv_obj_remove_flag(GUI_Panel__home__panel_3, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__home__panel_3, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Panel__home__panel_3, 60, 210);

  GUI_Button__home__buttonup = lv_button_create(GUI_Panel__home__panel_3);
  lv_obj_set_align(GUI_Button__home__buttonup, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Button__home__buttonup, 44, 44);

  GUI_Image__home__image_34 = lv_image_create(GUI_Button__home__buttonup);
  lv_obj_add_flag(GUI_Image__home__image_34, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__image_34, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__image_34, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Image__home__image_34, -4, -6);
  lv_obj_set_size(GUI_Image__home__image_34, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Button__home__buttonplay = lv_button_create(GUI_Panel__home__panel_3);
  lv_obj_set_align(GUI_Button__home__buttonplay, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Button__home__buttonplay, 20, 20);
  lv_obj_set_size(GUI_Button__home__buttonplay, 44, 44);

  GUI_Image__home__image_35 = lv_image_create(GUI_Button__home__buttonplay);
  lv_obj_add_flag(GUI_Image__home__image_35, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__image_35, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__image_35, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Image__home__image_35, -5, -4);
  lv_obj_set_size(GUI_Image__home__image_35, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_Textarea__home__textarea_4 = lv_textarea_create(GUI_Panel__home__panel_3);
  lv_textarea_set_text(GUI_Textarea__home__textarea_4, "1");
  lv_textarea_set_max_length(GUI_Textarea__home__textarea_4, 2);
  lv_textarea_set_accepted_chars(GUI_Textarea__home__textarea_4, "1234567890");
  lv_textarea_set_placeholder_text(GUI_Textarea__home__textarea_4, "");
  lv_textarea_set_one_line(GUI_Textarea__home__textarea_4, true);
  lv_obj_set_align(GUI_Textarea__home__textarea_4, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Textarea__home__textarea_4, 44, 44);

  GUI_Button__home__buttondown = lv_button_create(GUI_Panel__home__panel_3);
  lv_obj_set_align(GUI_Button__home__buttondown, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Button__home__buttondown, 20, 20);
  lv_obj_set_size(GUI_Button__home__buttondown, 44, 44);

  GUI_Image__home__image_36 = lv_image_create(GUI_Button__home__buttondown);
  lv_obj_add_flag(GUI_Image__home__image_36, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_remove_flag(GUI_Image__home__image_36, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Image__home__image_36, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Image__home__image_36, -8, -6);
  lv_obj_set_size(GUI_Image__home__image_36, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  GUI_initScreenStyles__home();
  GUI_initScreenTexts__home();
}

void GUI_initScreenTexts__home() {
  lv_label_set_text(GUI_Label__home__CURRENTTIMEQ, "10:34PM");
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
  lv_label_set_text(GUI_Label__home__label_15, "Chance for rain");
  lv_label_set_text(GUI_Label__home__label_16, "50%");
  lv_label_set_text(GUI_Label__home__HR1FORCASTTIMEQ, "7PM");
  lv_label_set_text(GUI_Label__home__HR1FORCASTTEMPQ, "115°");
  lv_label_set_text(GUI_Label__home__HR2FORCASTTIMEQ, "8PM");
  lv_label_set_text(GUI_Label__home__HR2FORCASTTEMPQ, "116°");
  lv_label_set_text(GUI_Label__home__HR3FORCASTTIMEQ, "9PM");
  lv_label_set_text(GUI_Label__home__HR3FORCASTTEMPQ, "117°");
  lv_label_set_text(GUI_Label__home__HR4FORCASTTIMEQ, "10PM");
  lv_label_set_text(GUI_Label__home__HR4FORCASTTEMPQ, "118°");
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

  lv_obj_add_style(GUI_Container__home__weatherwidget,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Container__home__weatherwidget, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Container__home__weatherwidget, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);
  lv_obj_add_style(GUI_Container__home__weatherwidget,
                   &GUI_Style__class_qx6LXcI5pACvOu__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Panel__home__weather_panel,
                   &GUI_Style__class_jyulY1Pqi8nomS__homepanelsgreydrop,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__weather_panel, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_COLOR, _ui_theme_bg_color_dark_gray);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__weather_panel, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_OPA, _ui_theme_bg_opa_dark_gray);

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
                   &GUI_Style__class_hTcCCcEmxbV9uE__lightgreytextlabelsize,
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
                   &GUI_Style__class_hTcCCcEmxbV9uE__lightgreytextlabelsize,
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
                   &GUI_Style__class_hTcCCcEmxbV9uE__lightgreytextlabelsize,
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

  lv_obj_add_style(GUI_Container__home__container_14,
                   &GUI_Style__class_Nw5ZzS54u0kNST__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__label_15,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__label_15, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__label_15, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Label__home__label_16,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__label_16, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__label_16, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Container__home__hourly_forecast_cont,
                   &GUI_Style__class_JfsW2zlbgqJxeX__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Container__home__hourly_forecast_cont,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
      _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Container__home__hourly_forecast_cont,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
      _ui_theme_bg_opa_white);
  lv_obj_add_style(GUI_Container__home__hourly_forecast_cont,
                   &GUI_Style__class_E4wegkc5VJFKsz__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__home__hourly_forecast_items_cont,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Container__home__hourly_forecast_items_cont,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
      _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Container__home__hourly_forecast_items_cont,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
      _ui_theme_bg_opa_white);
  lv_obj_add_style(GUI_Container__home__hourly_forecast_items_cont,
                   &GUI_Style__class_NHMYIepongiLxJ__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__home__HR1FORCASTCONT,
                   &GUI_Style__class_bnXaTUcRtszjic__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__HR1FORCASTTIMEQ,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR1FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR1FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_image_set_src(GUI_Image__home__HR1FORCASICON22X22Q,
                   &upload_1s_a942bd7a65904cab870f8cf337226275_png);

  lv_obj_add_style(GUI_Image__home__HR1FORCASICON22X22Q,
                   &GUI_Style__class_UwhUwo9ljeO8lt__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__HR1FORCASTTEMPQ,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
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
                   &GUI_Style__class_hTcCCcEmxbV9uE__lightgreytextlabelsize,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR2FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR2FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);
  lv_obj_add_style(GUI_Label__home__HR2FORCASTTIMEQ,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR2FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR2FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_image_set_src(GUI_Image__home__HR2FORCASTICON22X22Q,
                   &upload_1s_a942bd7a65904cab870f8cf337226275_png);

  lv_obj_add_style(GUI_Label__home__HR2FORCASTTEMPQ,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
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
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR3FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR3FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_image_set_src(GUI_Image__home__HR3FORCASTICON22X22Q,
                   &upload_1s_a942bd7a65904cab870f8cf337226275_png);

  lv_obj_add_style(GUI_Label__home__HR3FORCASTTEMPQ,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
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
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR4FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR4FORCASTTIMEQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_image_set_src(GUI_Image__home__HR4FORCASTICON22X22Q,
                   &upload_1s_a942bd7a65904cab870f8cf337226275_png);

  lv_obj_add_style(GUI_Label__home__HR4FORCASTTEMPQ,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR4FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__HR4FORCASTTEMPQ, LV_PART_MAIN | LV_STATE_DEFAULT,
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
                   &GUI_Style__class_7OQhhQvZCJEXNK__orangetitle,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY1FORCASTTEMPLOWQ,
                   &GUI_Style__class_9MpPQcvJSEks3y__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY1FORCASTSTATUSQ,
                   &GUI_Style__class_hTcCCcEmxbV9uE__lightgreytextlabelsize,
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
                   &GUI_Style__class_7OQhhQvZCJEXNK__orangetitle,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY2FORCASTTEMPLOWQ,
                   &GUI_Style__class_wKOu7L7fJpWUc8__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY2FORCASTSTATUSQ,
                   &GUI_Style__class_hTcCCcEmxbV9uE__lightgreytextlabelsize,
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
                   &GUI_Style__class_7OQhhQvZCJEXNK__orangetitle,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY3FORCASTTEMPLOWQ,
                   &GUI_Style__class_5aC5gkRCTJRBpX__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__home__DAYLY3FORCASTSTATUSQ,
                   &GUI_Style__class_hTcCCcEmxbV9uE__lightgreytextlabelsize,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY3FORCASTSTATUSQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__home__DAYLY3FORCASTSTATUSQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Container__home__NETWORKSTAUSANDMENUCONT,
                   &GUI_Style__class_292pAkrGlFbI2Z__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Panel__home__panel,
                   &GUI_Style__class_jyulY1Pqi8nomS__homepanelsgreydrop,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__panel, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_COLOR, _ui_theme_bg_color_dark_gray);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__panel, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_OPA, _ui_theme_bg_opa_dark_gray);
  lv_obj_add_style(GUI_Panel__home__panel, &GUI_Style__class_5TGhYvIVJqeKbz__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_image_set_src(GUI_Image__home__NETWORKCONNECTEDICONQ,
                   &upload_network_e6b6f98e6cbd4491bfe5310d621d416f_png);

  lv_obj_add_style(GUI_Image__home__NETWORKCONNECTEDICONQ,
                   &GUI_Style__class_q2R0whE4QSYoQW__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_image_set_src(GUI_Image__home__SETTINGSBUTTON,
                   &upload_menusettings_ac03cce753704d6fa240fb7acbd315c1_png);

  lv_obj_add_style(GUI_Image__home__SETTINGSBUTTON,
                   &GUI_Style__class_9GC5Lu563pp2v8__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__home__music_player,
                   &GUI_Style__class_xexLdCVLH1nhdG__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Panel__home__panel_3,
                   &GUI_Style__class_jyulY1Pqi8nomS__homepanelsgreydrop,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__panel_3, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_COLOR, _ui_theme_bg_color_dark_gray);
  ui_object_set_themeable_style_property(
      GUI_Panel__home__panel_3, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_OPA, _ui_theme_bg_opa_dark_gray);
  lv_obj_add_style(GUI_Panel__home__panel_3, &GUI_Style__class_WbVp4cvhuaek12__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Button__home__buttonup,
                   &GUI_Style__class_jyulY1Pqi8nomS__homepanelsgreydrop,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonup, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_COLOR, _ui_theme_bg_color_dark_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonup, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_OPA, _ui_theme_bg_opa_dark_gray);
  lv_obj_add_style(GUI_Button__home__buttonup,
                   &GUI_Style__class_qiMEmWPuCKrDEz__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonup, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonup, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_OPA, _ui_theme_bg_opa_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonup, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_COLOR, _ui_theme_grad_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonup, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_OPA, _ui_theme_grad_opa_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonup, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_DIR, _ui_theme_grad_dir_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonup, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_MAIN_STOP, _ui_theme_main_stop_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonup, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_STOP, _ui_theme_grad_stop_light_gray);

  lv_image_set_src(GUI_Image__home__image_34,
                   &upload_up1_09eda56b61fb420bb4679dd39a4c9782_png);

  lv_obj_add_style(GUI_Image__home__image_34,
                   &GUI_Style__class_HIiXHuL38TNuIt__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Button__home__buttonplay,
                   &GUI_Style__class_jyulY1Pqi8nomS__homepanelsgreydrop,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonplay, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_COLOR, _ui_theme_bg_color_dark_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonplay, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_OPA, _ui_theme_bg_opa_dark_gray);
  lv_obj_add_style(GUI_Button__home__buttonplay,
                   &GUI_Style__class_HldN8l5VcFDMxv__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonplay, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonplay, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_OPA, _ui_theme_bg_opa_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonplay, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_COLOR, _ui_theme_grad_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonplay, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_OPA, _ui_theme_grad_opa_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonplay, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_DIR, _ui_theme_grad_dir_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonplay, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_MAIN_STOP, _ui_theme_main_stop_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttonplay, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_STOP, _ui_theme_grad_stop_light_gray);

  lv_image_set_src(GUI_Image__home__image_35,
                   &upload_play_6c36b149bbde4d87af41769e62ca887f_png);

  lv_obj_add_style(GUI_Image__home__image_35,
                   &GUI_Style__class_IYhAyYrDrwaPbi__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Textarea__home__textarea_4,
                   &GUI_Style__class_Na1ZxCw4bLah1N__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Textarea__home__textarea_4, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Textarea__home__textarea_4, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_OPA, _ui_theme_bg_opa_white);
  ui_object_set_themeable_style_property(
      GUI_Textarea__home__textarea_4, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_COLOR, _ui_theme_grad_color_white);
  ui_object_set_themeable_style_property(
      GUI_Textarea__home__textarea_4, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_OPA, _ui_theme_grad_opa_white);
  ui_object_set_themeable_style_property(
      GUI_Textarea__home__textarea_4, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_DIR, _ui_theme_grad_dir_white);
  ui_object_set_themeable_style_property(
      GUI_Textarea__home__textarea_4, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_MAIN_STOP, _ui_theme_main_stop_white);
  ui_object_set_themeable_style_property(
      GUI_Textarea__home__textarea_4, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_STOP, _ui_theme_grad_stop_white);

  lv_obj_add_style(GUI_Button__home__buttondown,
                   &GUI_Style__class_jyulY1Pqi8nomS__homepanelsgreydrop,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttondown, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_COLOR, _ui_theme_bg_color_dark_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttondown, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_SHADOW_OPA, _ui_theme_bg_opa_dark_gray);
  lv_obj_add_style(GUI_Button__home__buttondown,
                   &GUI_Style__class_XiuhNUm3yt1F7y__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttondown, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttondown, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_OPA, _ui_theme_bg_opa_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttondown, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_COLOR, _ui_theme_grad_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttondown, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_OPA, _ui_theme_grad_opa_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttondown, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_DIR, _ui_theme_grad_dir_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttondown, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_MAIN_STOP, _ui_theme_main_stop_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Button__home__buttondown, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_STOP, _ui_theme_grad_stop_light_gray);

  lv_image_set_src(GUI_Image__home__image_36,
                   &upload_down1_991ef596d2b7435c9663fdfd256c222a_png);

  lv_obj_add_style(GUI_Image__home__image_36,
                   &GUI_Style__class_TYxKoIRA89pYki__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
}
