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

  lv_obj_add_style(GUI_Label__home__Precipitation,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__Precipitation, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__Precipitation, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Label__home__PRECIPITATIONQ,
                   &GUI_Style__class_rh9OIkK6YioF9J__test1style,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__home__PRECIPITATIONQ, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__home__PRECIPITATIONQ, LV_PART_MAIN | LV_STATE_DEFAULT,
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
