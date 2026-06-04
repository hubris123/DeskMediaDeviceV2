#include "../GUI.h"

void GUI_initScreen__keyboardpassword() {
  GUI_Screen__keyboardpassword = lv_obj_create(NULL);
  lv_obj_remove_flag(GUI_Screen__keyboardpassword, LV_OBJ_FLAG_SCROLLABLE);
  GUI_Container__keyboardpassword__container =
      lv_obj_create(GUI_Screen__keyboardpassword);
  lv_obj_remove_style_all(GUI_Container__keyboardpassword__container);
  lv_obj_remove_flag(GUI_Container__keyboardpassword__container,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__keyboardpassword__container,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__keyboardpassword__container, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__keyboardpassword__container, 770, 480);

  GUI_Textarea__keyboardpassword__textareapassword =
      lv_textarea_create(GUI_Container__keyboardpassword__container);
  lv_textarea_set_text(GUI_Textarea__keyboardpassword__textareapassword, "");
  lv_textarea_set_placeholder_text(
      GUI_Textarea__keyboardpassword__textareapassword, "password");
  lv_textarea_set_one_line(GUI_Textarea__keyboardpassword__textareapassword,
                           false);
  lv_textarea_set_password_mode(
      GUI_Textarea__keyboardpassword__textareapassword, true);
  lv_obj_set_align(GUI_Textarea__keyboardpassword__textareapassword,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Textarea__keyboardpassword__textareapassword, 600, 50);

  GUI_Keyboard__keyboardpassword__keyboardpass =
      lv_keyboard_create(GUI_Container__keyboardpassword__container);
  lv_obj_remove_flag(GUI_Keyboard__keyboardpassword__keyboardpass,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(GUI_Keyboard__keyboardpassword__keyboardpass,
                  LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_keyboard_set_mode(GUI_Keyboard__keyboardpassword__keyboardpass,
                       LV_KEYBOARD_MODE_TEXT_LOWER);
  lv_keyboard_set_textarea(GUI_Keyboard__keyboardpassword__keyboardpass,
                           GUI_Textarea__keyboardpassword__textareapassword);
  lv_obj_set_align(GUI_Keyboard__keyboardpassword__keyboardpass,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Keyboard__keyboardpassword__keyboardpass, 740, 350);

  GUI_Container__keyboardpassword__container_1 =
      lv_obj_create(GUI_Container__keyboardpassword__container);
  lv_obj_remove_style_all(GUI_Container__keyboardpassword__container_1);
  lv_obj_remove_flag(GUI_Container__keyboardpassword__container_1,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__keyboardpassword__container_1,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__keyboardpassword__container_1,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__keyboardpassword__container_1, 302, 49);

  GUI_Button__keyboardpassword__button_3 =
      lv_button_create(GUI_Container__keyboardpassword__container_1);
  lv_obj_set_align(GUI_Button__keyboardpassword__button_3, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Button__keyboardpassword__button_3, 120, 50);

  GUI_Label__keyboardpassword__label_10 =
      lv_label_create(GUI_Button__keyboardpassword__button_3);
  lv_label_set_long_mode(GUI_Label__keyboardpassword__label_10,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__keyboardpassword__label_10, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__keyboardpassword__label_10, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Button__keyboardpassword__button_2 =
      lv_button_create(GUI_Container__keyboardpassword__container_1);
  lv_obj_set_align(GUI_Button__keyboardpassword__button_2, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Button__keyboardpassword__button_2, 20, 20);
  lv_obj_set_size(GUI_Button__keyboardpassword__button_2, 120, 50);

  GUI_Label__keyboardpassword__label_9 =
      lv_label_create(GUI_Button__keyboardpassword__button_2);
  lv_obj_add_flag(GUI_Label__keyboardpassword__label_9, LV_OBJ_FLAG_CLICKABLE);
  lv_label_set_long_mode(GUI_Label__keyboardpassword__label_9,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__keyboardpassword__label_9, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__keyboardpassword__label_9, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_initScreenStyles__keyboardpassword();
  GUI_initScreenTexts__keyboardpassword();
}

void GUI_initScreenTexts__keyboardpassword() {
  lv_label_set_text(GUI_Label__keyboardpassword__label_10, "SAVE");
  lv_label_set_text(GUI_Label__keyboardpassword__label_9, "EXIT");
}

void GUI_initScreenStyles__keyboardpassword() {
  lv_obj_add_style(GUI_Screen__keyboardpassword,
                   &GUI_Style__class_AvvcYB4wlpzNqr__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__keyboardpassword__container,
                   &GUI_Style__class_iwIHCP82XdjJdR__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Textarea__keyboardpassword__textareapassword,
                   &GUI_Style__class_3D1LJzVb3VpFXw__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardpassword__textareapassword,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
      _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardpassword__textareapassword,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
      _ui_theme_bg_opa_white);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardpassword__textareapassword,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
      _ui_theme_bg_color_black);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardpassword__textareapassword,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_bg_opa_black);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardpassword__textareapassword,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_COLOR,
      _ui_theme_grad_color_black);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardpassword__textareapassword,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_OPA,
      _ui_theme_grad_opa_black);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardpassword__textareapassword,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_DIR,
      _ui_theme_grad_dir_black);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardpassword__textareapassword,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_MAIN_STOP,
      _ui_theme_main_stop_black);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardpassword__textareapassword,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_GRAD_STOP,
      _ui_theme_grad_stop_black);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardpassword__textareapassword,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_OUTLINE_COLOR,
      _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Textarea__keyboardpassword__textareapassword,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_OUTLINE_OPA,
      _ui_theme_bg_opa_light_gray);

  lv_obj_add_style(GUI_Keyboard__keyboardpassword__keyboardpass,
                   &GUI_Style__class_T55yx89GcErjMB__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Keyboard__keyboardpassword__keyboardpass,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_OUTLINE_COLOR,
      _ui_theme_bg_color_light_gray);
  ui_object_set_themeable_style_property(
      GUI_Keyboard__keyboardpassword__keyboardpass,
      LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_OUTLINE_OPA,
      _ui_theme_bg_opa_light_gray);
  lv_obj_add_style(GUI_Keyboard__keyboardpassword__keyboardpass,
                   &GUI_Style__class_N2ZGjd4KyT2jge__,
                   LV_PART_ITEMS | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__keyboardpassword__container_1,
                   &GUI_Style__class_SeaE8oLUEcFIjo__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Button__keyboardpassword__button_3,
                   &GUI_Style__class_S00QKhlb0gBiuM__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__keyboardpassword__label_10,
                   &GUI_Style__class_jAGg6Vr5bnL97F__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Button__keyboardpassword__button_2,
                   &GUI_Style__class_Qk2ZaylebJZndu__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__keyboardpassword__label_9,
                   &GUI_Style__class_c1pd7LDWptdXA5__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
}
