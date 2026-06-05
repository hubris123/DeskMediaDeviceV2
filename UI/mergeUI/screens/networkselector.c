#include "../GUI.h"

void GUI_initScreen__networkselector() {
  GUI_Screen__networkselector = lv_obj_create(NULL);
  lv_obj_remove_flag(GUI_Screen__networkselector, LV_OBJ_FLAG_SCROLLABLE);
  GUI_Container__networkselector__container_10 =
      lv_obj_create(GUI_Screen__networkselector);
  lv_obj_remove_style_all(GUI_Container__networkselector__container_10);
  lv_obj_remove_flag(GUI_Container__networkselector__container_10,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__networkselector__container_10,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__networkselector__container_10,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__networkselector__container_10, 800, 480);

  GUI_Panel__networkselector__panel_2 =
      lv_obj_create(GUI_Container__networkselector__container_10);
  lv_obj_remove_flag(GUI_Panel__networkselector__panel_2,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Panel__networkselector__panel_2, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Panel__networkselector__panel_2, 518, 470);

  GUI_Container__networkselector__container_5 =
      lv_obj_create(GUI_Panel__networkselector__panel_2);
  lv_obj_remove_style_all(GUI_Container__networkselector__container_5);
  lv_obj_remove_flag(GUI_Container__networkselector__container_5,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__networkselector__container_5,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__networkselector__container_5,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__networkselector__container_5, 518, 40);

  GUI_Label__networkselector__label_8 =
      lv_label_create(GUI_Container__networkselector__container_5);
  lv_label_set_long_mode(GUI_Label__networkselector__label_8,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__networkselector__label_8, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__networkselector__label_8, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Container__networkselector__contscroll =
      lv_obj_create(GUI_Panel__networkselector__panel_2);
  lv_obj_remove_style_all(GUI_Container__networkselector__contscroll);
  lv_obj_remove_flag(GUI_Container__networkselector__contscroll,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__networkselector__contscroll,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__networkselector__contscroll, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__networkselector__contscroll, 20, 20);
  lv_obj_set_size(GUI_Container__networkselector__contscroll, 518, 370);

  GUI_Container__networkselector__contbottom =
      lv_obj_create(GUI_Panel__networkselector__panel_2);
  lv_obj_remove_style_all(GUI_Container__networkselector__contbottom);
  lv_obj_remove_flag(GUI_Container__networkselector__contbottom,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__networkselector__contbottom,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__networkselector__contbottom, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Container__networkselector__contbottom, 40, 40);
  lv_obj_set_size(GUI_Container__networkselector__contbottom, 518, 60);

  GUI_Container__networkselector__buttonsbottom =
      lv_obj_create(GUI_Container__networkselector__contbottom);
  lv_obj_remove_style_all(GUI_Container__networkselector__buttonsbottom);
  lv_obj_remove_flag(GUI_Container__networkselector__buttonsbottom,
                     LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(GUI_Container__networkselector__buttonsbottom,
                     LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_align(GUI_Container__networkselector__buttonsbottom,
                   LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Container__networkselector__buttonsbottom, 302, 49);

  GUI_Button__networkselector__networksave =
      lv_button_create(GUI_Container__networkselector__buttonsbottom);
  lv_obj_set_align(GUI_Button__networkselector__networksave, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Button__networkselector__networksave, 120, 50);

  GUI_Label__networkselector__label_14 =
      lv_label_create(GUI_Button__networkselector__networksave);
  lv_label_set_long_mode(GUI_Label__networkselector__label_14,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__networkselector__label_14, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__networkselector__label_14, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_Button__networkselector__networkexit =
      lv_button_create(GUI_Container__networkselector__buttonsbottom);
  lv_obj_set_align(GUI_Button__networkselector__networkexit, LV_ALIGN_CENTER);
  lv_obj_set_pos(GUI_Button__networkselector__networkexit, 20, 20);
  lv_obj_set_size(GUI_Button__networkselector__networkexit, 120, 50);

  GUI_Label__networkselector__label_13 =
      lv_label_create(GUI_Button__networkselector__networkexit);
  lv_obj_add_flag(GUI_Label__networkselector__label_13, LV_OBJ_FLAG_CLICKABLE);
  lv_label_set_long_mode(GUI_Label__networkselector__label_13,
                         LV_LABEL_LONG_WRAP);
  lv_obj_set_align(GUI_Label__networkselector__label_13, LV_ALIGN_CENTER);
  lv_obj_set_size(GUI_Label__networkselector__label_13, LV_SIZE_CONTENT,
                  LV_SIZE_CONTENT);

  GUI_initScreenStyles__networkselector();
  GUI_initScreenTexts__networkselector();
}

void GUI_initScreenTexts__networkselector() {
  lv_label_set_text(GUI_Label__networkselector__label_8, "CHOOSE NETWORK");
  lv_label_set_text(GUI_Label__networkselector__label_14, "SAVE");
  lv_label_set_text(GUI_Label__networkselector__label_13, "EXIT");
}

void GUI_initScreenStyles__networkselector() {
  lv_obj_add_style(GUI_Screen__networkselector,
                   &GUI_Style__class_O3vmtiU810aw4c__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__networkselector__container_10,
                   &GUI_Style__class_DQ4kt4qPpym6rl__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Panel__networkselector__panel_2,
                   &GUI_Style__class_kQXih6nLC1SrH3__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Panel__networkselector__panel_2, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_COLOR, _ui_theme_bg_color_black);
  ui_object_set_themeable_style_property(
      GUI_Panel__networkselector__panel_2, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_OPA, _ui_theme_bg_opa_black);
  ui_object_set_themeable_style_property(
      GUI_Panel__networkselector__panel_2, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_COLOR, _ui_theme_grad_color_black);
  ui_object_set_themeable_style_property(
      GUI_Panel__networkselector__panel_2, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_OPA, _ui_theme_grad_opa_black);
  ui_object_set_themeable_style_property(
      GUI_Panel__networkselector__panel_2, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_DIR, _ui_theme_grad_dir_black);
  ui_object_set_themeable_style_property(
      GUI_Panel__networkselector__panel_2, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_MAIN_STOP, _ui_theme_main_stop_black);
  ui_object_set_themeable_style_property(
      GUI_Panel__networkselector__panel_2, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_GRAD_STOP, _ui_theme_grad_stop_black);

  lv_obj_add_style(GUI_Container__networkselector__container_5,
                   &GUI_Style__class_QF2GdgY5xfVzQ0__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__networkselector__label_8,
                   &GUI_Style__class_0LBWzZLzLfqETo__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(
      GUI_Label__networkselector__label_8, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR, _ui_theme_bg_color_white);
  ui_object_set_themeable_style_property(
      GUI_Label__networkselector__label_8, LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA, _ui_theme_bg_opa_white);

  lv_obj_add_style(GUI_Container__networkselector__contscroll,
                   &GUI_Style__class_kpy4gFNlDBjSEj__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__networkselector__contbottom,
                   &GUI_Style__class_BaRRq8Hq0lhEou__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Container__networkselector__buttonsbottom,
                   &GUI_Style__class_4JlJ2ejqL1Psax__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Button__networkselector__networksave,
                   &GUI_Style__class_3YoiCm5zB32T9v__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__networkselector__label_14,
                   &GUI_Style__class_M62PYEgOaUQxR4__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Button__networkselector__networkexit,
                   &GUI_Style__class_XequMyVxnqRNmJ__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_style(GUI_Label__networkselector__label_13,
                   &GUI_Style__class_XzyMwqbFt9YVZL__,
                   LV_PART_MAIN | LV_STATE_DEFAULT);
}
