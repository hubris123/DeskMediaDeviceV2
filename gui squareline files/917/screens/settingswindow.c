#include "../GUI.h"

void GUI_initScreen__settingswindow() {
  GUI_Screen__settingswindow = lv_obj_create(NULL);
  lv_obj_remove_flag(GUI_Screen__settingswindow, LV_OBJ_FLAG_SCROLLABLE);
  GUI_Container__settingswindow__container_3 =
      lv_obj_create(GUI_Screen__settingswindow);
  lv_obj_remove_style_all(GUI_Container__settingswindow__container_3);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_3,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_3,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__settingswindow__container_3, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__settingswindow__container_3, 215, -40);
  lv_obj_set_size(GUI_Container__settingswindow__container_3, 412, 464);

  GUI_Panel__settingswindow__panel_1 =
      lv_obj_create(GUI_Container__settingswindow__container_3);
  lv_obj_remove_flag(GUI_Panel__settingswindow__panel_1,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__settingswindow__panel_1, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Panel__settingswindow__panel_1, 360, 390);

  GUI_Container__settingswindow__container_4 =
      lv_obj_create(GUI_Panel__settingswindow__panel_1);
  lv_obj_remove_style_all(GUI_Container__settingswindow__container_4);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_4,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_4,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__settingswindow__container_4, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__settingswindow__container_4, 347, 361);

  GUI_Label__settingswindow__labelsettings =
      lv_label_create(GUI_Container__settingswindow__container_4);
  lv_label_set_long_mode(GUI_Label__settingswindow__labelsettings,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__settingswindow__labelsettings, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__settingswindow__labelsettings, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Checkbox__settingswindow__checkbox =
      lv_checkbox_create(GUI_Container__settingswindow__container_4);
  lv_obj_add_flag(GUI_Checkbox__settingswindow__checkbox,
                  LV_OBJ_FLAG_CHECKABLE);
  lv_checkbox_set_text(GUI_Checkbox__settingswindow__checkbox, "   MUTE");
  lv_obj_set_align(GUI_Checkbox__settingswindow__checkbox, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Checkbox__settingswindow__checkbox, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Label__settingswindow__label =
      lv_label_create(GUI_Container__settingswindow__container_4);
  lv_label_set_long_mode(GUI_Label__settingswindow__label, LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__settingswindow__label, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__settingswindow__label, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Slider__settingswindow__slider =
      lv_slider_create(GUI_Container__settingswindow__container_4);
  lv_slider_set_value(GUI_Slider__settingswindow__slider, 50, LV_ANIM_OFF);
  lv_slider_set_mode(GUI_Slider__settingswindow__slider, LV_SLIDER_MODE_NORMAL);
  lv_obj_set_align(GUI_Slider__settingswindow__slider, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Slider__settingswindow__slider, 300, 30);

  GUI_Label__settingswindow__label_1 =
      lv_label_create(GUI_Container__settingswindow__container_4);
  lv_label_set_long_mode(GUI_Label__settingswindow__label_1,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__settingswindow__label_1, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Label__settingswindow__label_1, 20, 20);
  lv_obj_set_size(GUI_Label__settingswindow__label_1, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Slider__settingswindow__slider_1 =
      lv_slider_create(GUI_Container__settingswindow__container_4);
  lv_slider_set_mode(GUI_Slider__settingswindow__slider_1,
                     LV_SLIDER_MODE_NORMAL);
  lv_slider_set_value(GUI_Slider__settingswindow__slider_1, 50, LV_ANIM_OFF);
  lv_obj_set_align(GUI_Slider__settingswindow__slider_1, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Slider__settingswindow__slider_1, 20, 20);
  lv_obj_set_size(GUI_Slider__settingswindow__slider_1, 300, 30);

  GUI_Container__settingswindow__container_5 =
      lv_obj_create(GUI_Container__settingswindow__container_4);
  lv_obj_remove_style_all(GUI_Container__settingswindow__container_5);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_5,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_5,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__settingswindow__container_5, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__settingswindow__container_5, 301, 1);

  GUI_Label__settingswindow__label_3 =
      lv_label_create(GUI_Container__settingswindow__container_4);
  lv_label_set_long_mode(GUI_Label__settingswindow__label_3,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__settingswindow__label_3, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__settingswindow__label_3, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__settingswindow__container_6 =
      lv_obj_create(GUI_Container__settingswindow__container_4);
  lv_obj_remove_style_all(GUI_Container__settingswindow__container_6);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_6,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_6,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__settingswindow__container_6, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__settingswindow__container_6, 20, 20);
  lv_obj_set_size(GUI_Container__settingswindow__container_6, 296, 23);

  GUI_Label__settingswindow__label_4 =
      lv_label_create(GUI_Container__settingswindow__container_6);
  lv_label_set_long_mode(GUI_Label__settingswindow__label_4,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__settingswindow__label_4, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__settingswindow__label_4, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Textarea__settingswindow__textarea_1 =
      lv_textarea_create(GUI_Container__settingswindow__container_6);
  lv_textarea_set_accepted_chars(GUI_Textarea__settingswindow__textarea_1, "");
  lv_textarea_set_one_line(GUI_Textarea__settingswindow__textarea_1, true);
  lv_textarea_set_placeholder_text(GUI_Textarea__settingswindow__textarea_1,
                                   "");
  lv_textarea_set_text(GUI_Textarea__settingswindow__textarea_1, "");
  lv_obj_set_align(GUI_Textarea__settingswindow__textarea_1, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Textarea__settingswindow__textarea_1, 130, 30);

  GUI_Container__settingswindow__container_8 =
      lv_obj_create(GUI_Container__settingswindow__container_4);
  lv_obj_remove_style_all(GUI_Container__settingswindow__container_8);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_8,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_8,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__settingswindow__container_8, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__settingswindow__container_8, 296, 23);

  GUI_Label__settingswindow__label_6 =
      lv_label_create(GUI_Container__settingswindow__container_8);
  lv_label_set_long_mode(GUI_Label__settingswindow__label_6,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__settingswindow__label_6, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__settingswindow__label_6, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Textarea__settingswindow__textarea_2 =
      lv_textarea_create(GUI_Container__settingswindow__container_8);
  lv_textarea_set_accepted_chars(GUI_Textarea__settingswindow__textarea_2, "");
  lv_textarea_set_one_line(GUI_Textarea__settingswindow__textarea_2, true);
  lv_textarea_set_placeholder_text(GUI_Textarea__settingswindow__textarea_2,
                                   "");
  lv_textarea_set_text(GUI_Textarea__settingswindow__textarea_2, "");
  lv_obj_set_align(GUI_Textarea__settingswindow__textarea_2, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Textarea__settingswindow__textarea_2, 180, 30);

  GUI_Container__settingswindow__container_9 =
      lv_obj_create(GUI_Container__settingswindow__container_4);
  lv_obj_remove_style_all(GUI_Container__settingswindow__container_9);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_9,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_9,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__settingswindow__container_9, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__settingswindow__container_9, 20, 20);
  lv_obj_set_size(GUI_Container__settingswindow__container_9, 296, 23);

  GUI_Label__settingswindow__label_7 =
      lv_label_create(GUI_Container__settingswindow__container_9);
  lv_label_set_long_mode(GUI_Label__settingswindow__label_7,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__settingswindow__label_7, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__settingswindow__label_7, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Textarea__settingswindow__textarea_3 =
      lv_textarea_create(GUI_Container__settingswindow__container_9);
  lv_textarea_set_accepted_chars(GUI_Textarea__settingswindow__textarea_3, "");
  lv_textarea_set_one_line(GUI_Textarea__settingswindow__textarea_3, true);
  lv_textarea_set_placeholder_text(GUI_Textarea__settingswindow__textarea_3,
                                   "");
  lv_textarea_set_text(GUI_Textarea__settingswindow__textarea_3, "");
  lv_textarea_set_password_mode(GUI_Textarea__settingswindow__textarea_3, true);
  lv_obj_set_align(GUI_Textarea__settingswindow__textarea_3, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Textarea__settingswindow__textarea_3, 172, 30);

  GUI_Container__settingswindow__container_7 =
      lv_obj_create(GUI_Container__settingswindow__container_4);
  lv_obj_remove_style_all(GUI_Container__settingswindow__container_7);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_7,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__settingswindow__container_7,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__settingswindow__container_7, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__settingswindow__container_7, 302, 37);

  GUI_Button__settingswindow__button =
      lv_button_create(GUI_Container__settingswindow__container_7);
  lv_obj_set_align(GUI_Button__settingswindow__button, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Button__settingswindow__button, 102, 28);

  GUI_Label__settingswindow__label_2 =
      lv_label_create(GUI_Button__settingswindow__button);
  lv_label_set_long_mode(GUI_Label__settingswindow__label_2,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__settingswindow__label_2, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__settingswindow__label_2, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Button__settingswindow__button_1 =
      lv_button_create(GUI_Container__settingswindow__container_7);
  lv_obj_set_align(GUI_Button__settingswindow__button_1, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Button__settingswindow__button_1, 20, 20);
  lv_obj_set_size(GUI_Button__settingswindow__button_1, 102, 28);

  GUI_Label__settingswindow__label_5 =
      lv_label_create(GUI_Button__settingswindow__button_1);
  lv_obj_add_flag(GUI_Label__settingswindow__label_5, LV_OBJ_FLAG_CLICKABLE);
  lv_label_set_long_mode(GUI_Label__settingswindow__label_5,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__settingswindow__label_5, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__settingswindow__label_5, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_initScreenStyles__settingswindow();
  GUI_initScreenTexts__settingswindow();
}

void GUI_initScreenTexts__settingswindow() {
  lv_label_set_text(GUI_Label__settingswindow__labelsettings, "SETTINGS");
  lv_label_set_text(GUI_Label__settingswindow__label, "VOLUME");
  lv_label_set_text(GUI_Label__settingswindow__label_1, "BRIGHTNESS");
  lv_label_set_text(GUI_Label__settingswindow__label_3,
                    "ENTER ZIP CODE FOR WEATHER DATA");
  lv_label_set_text(GUI_Label__settingswindow__label_4, "ZIP CODE");
  lv_label_set_text(GUI_Label__settingswindow__label_6, "WIFI NAME");
  lv_label_set_text(GUI_Label__settingswindow__label_7, "PASSWORD");
  lv_label_set_text(GUI_Label__settingswindow__label_2, "SAVE");
  lv_label_set_text(GUI_Label__settingswindow__label_5, "EXIT");
}

void GUI_initScreenStyles__settingswindow() {
  lv_obj_add_style(GUI_Screen__settingswindow,
                   &GUI_Style__class_GGJhSwdaT2bCHt__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__settingswindow__container_3,
                   &GUI_Style__class_bvAupbhGzO9lgK__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Panel__settingswindow__panel_1,
                   &GUI_Style__class_BMQiIJTpQecj2H__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__settingswindow__container_4,
                   &GUI_Style__class_qbslh11Av1hMuq__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__settingswindow__labelsettings,
                   &GUI_Style__class_aZS4mzVqpqteVv__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__labelsettings, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__labelsettings, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Checkbox__settingswindow__checkbox,
                   &GUI_Style__class_Sew7zZBd2dEAC5__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Checkbox__settingswindow__checkbox, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Checkbox__settingswindow__checkbox, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);
  lv_obj_add_style(GUI_Checkbox__settingswindow__checkbox,
                   &GUI_Style__class_4hGM1DOyWzKXGG__,
                   LV_PART_INDICATOR | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(GUI_Checkbox__settingswindow__checkbox,
                                         LV_PART_INDICATOR | LV_STATE_DEFAULT,
                                         LV_STYLE_BG_COLOR,
                                         _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(GUI_Checkbox__settingswindow__checkbox,
                                         LV_PART_INDICATOR | LV_STATE_DEFAULT,
                                         LV_STYLE_BG_OPA,
                                         _ui_theme_bg_opa_white);
  ui_object_set_themeable_style_property(GUI_Checkbox__settingswindow__checkbox,
                                         LV_PART_INDICATOR | LV_STATE_DEFAULT,
                                         LV_STYLE_BG_GRAD_COLOR,
                                         _ui_theme_grad_color_white);
  ui_object_set_themeable_style_property(GUI_Checkbox__settingswindow__checkbox,
                                         LV_PART_INDICATOR | LV_STATE_DEFAULT,
                                         LV_STYLE_BG_GRAD_OPA,
                                         _ui_theme_grad_opa_white);
  ui_object_set_themeable_style_property(GUI_Checkbox__settingswindow__checkbox,
                                         LV_PART_INDICATOR | LV_STATE_DEFAULT,
                                         LV_STYLE_BG_GRAD_DIR,
                                         _ui_theme_grad_dir_white);
  ui_object_set_themeable_style_property(GUI_Checkbox__settingswindow__checkbox,
                                         LV_PART_INDICATOR | LV_STATE_DEFAULT,
                                         LV_STYLE_BG_MAIN_STOP,
                                         _ui_theme_main_stop_white);
  ui_object_set_themeable_style_property(GUI_Checkbox__settingswindow__checkbox,
                                         LV_PART_INDICATOR | LV_STATE_DEFAULT,
                                         LV_STYLE_BG_GRAD_STOP,
                                         _ui_theme_grad_stop_white);

  lv_obj_add_style(GUI_Label__settingswindow__label,
                   &GUI_Style__class_HK0JrRlgfa8ZRB__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Slider__settingswindow__slider,
                   &GUI_Style__class_JAL8G2SBlbgsMK__,
                   LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_add_style(GUI_Slider__settingswindow__slider,
                   &GUI_Style__class_kkoJh1hF2B6tCJ__,
                   LV_PART_KNOB | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__settingswindow__label_1,
                   &GUI_Style__class_GBcuAJEHFeiSNf__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label_1, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label_1, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Slider__settingswindow__slider_1,
                   &GUI_Style__class_QdPpiSKXWeZY8l__,
                   LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_add_style(GUI_Slider__settingswindow__slider_1,
                   &GUI_Style__class_jKApx7l6HgsZwU__,
                   LV_PART_KNOB | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__settingswindow__container_5,
                   &GUI_Style__class_5Eafbd6yvjyvsT__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__settingswindow__label_3,
                   &GUI_Style__class_Vtp3KvKLKtwz05__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label_3, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label_3, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Container__settingswindow__container_6,
                   &GUI_Style__class_IehN4pBvRaLoqF__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__settingswindow__label_4,
                   &GUI_Style__class_UsTxBsVMWpTM8Y__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label_4, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label_4, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Textarea__settingswindow__textarea_1,
                   &GUI_Style__class_HRt1tUvdsVkBGo__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(GUI_Textarea__settingswindow__textarea_1,
                   &GUI_Style__class_lot1J19xVfrTSM__,
                   LV_PART_TEXTAREA_PLACEHOLDER | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__settingswindow__container_8,
                   &GUI_Style__class_ponc57mppwwLBh__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__settingswindow__label_6,
                   &GUI_Style__class_ewB9ltfpeonQfa__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label_6, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label_6, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Textarea__settingswindow__textarea_2,
                   &GUI_Style__class_q7eZlp9MO0EzKy__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(GUI_Textarea__settingswindow__textarea_2,
                   &GUI_Style__class_AIIBPtShI6bHVL__,
                   LV_PART_TEXTAREA_PLACEHOLDER | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__settingswindow__container_9,
                   &GUI_Style__class_UWMNsTuzutTPfW__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__settingswindow__label_7,
                   &GUI_Style__class_Pxj4bSx4XPxr9u__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label_7, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Label__settingswindow__label_7, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Textarea__settingswindow__textarea_3,
                   &GUI_Style__class_F0bYoBMAkxnmi5__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(GUI_Textarea__settingswindow__textarea_3,
                   &GUI_Style__class_h8YvkrJbLCkkxu__,
                   LV_PART_TEXTAREA_PLACEHOLDER | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__settingswindow__container_7,
                   &GUI_Style__class_P0ckvf5Cd2Mzgn__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Button__settingswindow__button,
                   &GUI_Style__class_8giE6vbQ8nbm6U__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__settingswindow__label_2,
                   &GUI_Style__class_TkoVT5AI0Y9ewe__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Button__settingswindow__button_1,
                   &GUI_Style__class_jiEaq40fppn8N1__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__settingswindow__label_5,
                   &GUI_Style__class_yL8O2Ek0kq8lRL__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
}
