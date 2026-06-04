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

  GUI_Container__home__container_14 =
      lv_obj_create(GUI_Container__home__container_13);
  lv_obj_remove_style_all(GUI_Container__home__container_14);
  lv_obj_remove_flag(GUI_Container__home__container_14, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__container_14, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__container_14, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__home__container_14, 800, 480);

  GUI_Container__home__weatherwidget =
      lv_obj_create(GUI_Container__home__container_14);
  lv_obj_remove_style_all(GUI_Container__home__weatherwidget);
  lv_obj_remove_flag(GUI_Container__home__weatherwidget, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__weatherwidget,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__weatherwidget, LV_ALIGN_CENTER);
  lv_obj_set_x(GUI_Container__home__weatherwidget, -281);
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

  GUI_Container__home__container_15 =
      lv_obj_create(GUI_Container__home__container_13);
  lv_obj_remove_style_all(GUI_Container__home__container_15);
  lv_obj_remove_flag(GUI_Container__home__container_15, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__container_15, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__container_15, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__home__container_15, 800, 480);

  GUI_Container__home__NETWORKSTAUSANDMENUCONT =
      lv_obj_create(GUI_Container__home__container_15);
  lv_obj_remove_style_all(GUI_Container__home__NETWORKSTAUSANDMENUCONT);
  lv_obj_remove_flag(GUI_Container__home__NETWORKSTAUSANDMENUCONT,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__home__NETWORKSTAUSANDMENUCONT,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__home__NETWORKSTAUSANDMENUCONT,
                   LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__home__NETWORKSTAUSANDMENUCONT, 374, -219);
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
}

void GUI_initScreenStyles__home() {
  lv_obj_add_style(GUI_Screen__home, &GUI_Style__class_8rQJj2WR7KS5BW__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__home__container_13,
                   &GUI_Style__class_votutC5OWpgnlu__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__home__weatherwidget,
                   &GUI_Style__class_qx6LXcI5pACvOu__,
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

  lv_obj_add_style(GUI_Container__home__NETWORKSTAUSANDMENUCONT,
                   &GUI_Style__class_292pAkrGlFbI2Z__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

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
}
