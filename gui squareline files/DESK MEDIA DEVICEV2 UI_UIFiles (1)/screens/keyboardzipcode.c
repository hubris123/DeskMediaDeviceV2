#include "../GUI.h"

void GUI_initScreen__keyboardzipcode() {
  GUI_Screen__keyboardzipcode = lv_obj_create(NULL);
  lv_obj_remove_flag(GUI_Screen__keyboardzipcode, LV_OBJ_FLAG_SCROLLABLE);
  GUI_Container__keyboardzipcode__container_2 =
      lv_obj_create(GUI_Screen__keyboardzipcode);
  lv_obj_remove_style_all(GUI_Container__keyboardzipcode__container_2);
  lv_obj_remove_flag(GUI_Container__keyboardzipcode__container_2,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__keyboardzipcode__container_2,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__keyboardzipcode__container_2,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__keyboardzipcode__container_2, 770, 480);

  GUI_Textarea__keyboardzipcode__textarea_zipcode =
      lv_textarea_create(GUI_Container__keyboardzipcode__container_2);
  lv_textarea_set_one_line(GUI_Textarea__keyboardzipcode__textarea_zipcode,
                           false);
  lv_textarea_set_password_mode(GUI_Textarea__keyboardzipcode__textarea_zipcode,
                                false);
  lv_textarea_set_placeholder_text(
      GUI_Textarea__keyboardzipcode__textarea_zipcode, "zip code");
  lv_textarea_set_text(GUI_Textarea__keyboardzipcode__textarea_zipcode, "");
  lv_textarea_set_max_length(GUI_Textarea__keyboardzipcode__textarea_zipcode,
                             10);
  lv_obj_set_align(GUI_Textarea__keyboardzipcode__textarea_zipcode,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Textarea__keyboardzipcode__textarea_zipcode, 600, 50);

  GUI_Keyboard__keyboardzipcode__keyboardzip =
      lv_keyboard_create(GUI_Container__keyboardzipcode__container_2);
  lv_obj_remove_flag(GUI_Keyboard__keyboardzipcode__keyboardzip,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(GUI_Keyboard__keyboardzipcode__keyboardzip,
                  LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_keyboard_set_mode(GUI_Keyboard__keyboardzipcode__keyboardzip,
                       LV_KEYBOARD_MODE_NUMBER);
  lv_keyboard_set_textarea(GUI_Keyboard__keyboardzipcode__keyboardzip,
                           GUI_Textarea__keyboardzipcode__textarea_zipcode);
  lv_obj_set_align(GUI_Keyboard__keyboardzipcode__keyboardzip, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Keyboard__keyboardzipcode__keyboardzip, 3, 49);
  lv_obj_set_size(GUI_Keyboard__keyboardzipcode__keyboardzip, 740, 350);

  GUI_Container__keyboardzipcode__container_12 =
      lv_obj_create(GUI_Container__keyboardzipcode__container_2);
  lv_obj_remove_style_all(GUI_Container__keyboardzipcode__container_12);
  lv_obj_remove_flag(GUI_Container__keyboardzipcode__container_12,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__keyboardzipcode__container_12,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__keyboardzipcode__container_12,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__keyboardzipcode__container_12, 302, 49);

  GUI_Button__keyboardzipcode__button_5 =
      lv_button_create(GUI_Container__keyboardzipcode__container_12);
  lv_obj_set_align(GUI_Button__keyboardzipcode__button_5, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Button__keyboardzipcode__button_5, 120, 50);

  GUI_Label__keyboardzipcode__label_12 =
      lv_label_create(GUI_Button__keyboardzipcode__button_5);
  lv_label_set_long_mode(GUI_Label__keyboardzipcode__label_12,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__keyboardzipcode__label_12, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__keyboardzipcode__label_12, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Button__keyboardzipcode__button_4 =
      lv_button_create(GUI_Container__keyboardzipcode__container_12);
  lv_obj_set_align(GUI_Button__keyboardzipcode__button_4, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Button__keyboardzipcode__button_4, 20, 20);
  lv_obj_set_size(GUI_Button__keyboardzipcode__button_4, 120, 50);

  GUI_Label__keyboardzipcode__label_11 =
      lv_label_create(GUI_Button__keyboardzipcode__button_4);
  lv_obj_add_flag(GUI_Label__keyboardzipcode__label_11, LV_OBJ_FLAG_CLICKABLE);
  lv_label_set_long_mode(GUI_Label__keyboardzipcode__label_11,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__keyboardzipcode__label_11, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__keyboardzipcode__label_11, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_initScreenStyles__keyboardzipcode();
  GUI_initScreenTexts__keyboardzipcode();
}

void GUI_initScreenTexts__keyboardzipcode() {
  lv_label_set_text(GUI_Label__keyboardzipcode__label_12, "SAVE");
  lv_label_set_text(GUI_Label__keyboardzipcode__label_11, "EXIT");
}

void GUI_initScreenStyles__keyboardzipcode() {
  lv_obj_add_style(GUI_Screen__keyboardzipcode,
                   &GUI_Style__class_dVaeR4MlNk6JVP__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__keyboardzipcode__container_2,
                   &GUI_Style__class_vksvC08E9lfusD__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Textarea__keyboardzipcode__textarea_zipcode,
                   &GUI_Style__class_fqJ9BAuupzIIWh__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardzipcode__textarea_zipcode,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_OUTLINE_COLOR,
      _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardzipcode__textarea_zipcode,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_OUTLINE_OPA,
      _ui_theme_bg_opa_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardzipcode__textarea_zipcode,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
      _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardzipcode__textarea_zipcode,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
      _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Keyboard__keyboardzipcode__keyboardzip,
                   &GUI_Style__class_3VuxfhxbEdo6EO__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Keyboard__keyboardzipcode__keyboardzip,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_OUTLINE_COLOR,
      _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Keyboard__keyboardzipcode__keyboardzip,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_OUTLINE_OPA,
      _ui_theme_bg_opa_light_gray);
  lv_obj_add_style(GUI_Keyboard__keyboardzipcode__keyboardzip,
                   &GUI_Style__class_EQHHuxrbsUsFwQ__,
                   LV_PART_ITEMS | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__keyboardzipcode__container_12,
                   &GUI_Style__class_YQdvrXqq6m6UwT__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Button__keyboardzipcode__button_5,
                   &GUI_Style__class_5dd3lX6oAKILzH__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__keyboardzipcode__label_12,
                   &GUI_Style__class_WOdzLKOUWTKn2D__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Button__keyboardzipcode__button_4,
                   &GUI_Style__class_PWig0wvDygIn92__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__keyboardzipcode__label_11,
                   &GUI_Style__class_zIG6cz2FsG6fjL__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
}
