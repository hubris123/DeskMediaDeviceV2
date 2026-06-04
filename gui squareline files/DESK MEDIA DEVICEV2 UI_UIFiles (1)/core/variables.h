#ifndef _GUI_VARIABLES_H
#define _GUI_VARIABLES_H

#include "gui_core.h"

#ifdef __cplusplus
extern "C" {
#endif

extern lv_obj_t *GUI_Screen__home;
extern lv_obj_t *GUI_Container__home__container_13;
extern lv_obj_t *GUI_Container__home__container_14;
extern lv_obj_t *GUI_Container__home__weatherwidget;
extern lv_obj_t *GUI_Panel__home__weather_panel;
extern lv_obj_t *GUI_Container__home__main_display_cont;
extern lv_obj_t *GUI_Label__home__CURRENTTIMEQ;
extern lv_obj_t *GUI_Label__home__LOCATIONQ;
extern lv_obj_t *GUI_Label__home__CURRENTTEMPQ;
extern lv_obj_t *GUI_Container__home__current_status_cont;
extern lv_obj_t *GUI_Image__home__60X60ICONQ;
extern lv_obj_t *GUI_Label__home__CURRENTSTATUSQ;
extern lv_obj_t *GUI_Container__home__details_cont;
extern lv_obj_t *GUI_Container__home__details_feels_like_cont;
extern lv_obj_t *GUI_Label__home__details_feels_like_title_lbl;
extern lv_obj_t *GUI_Label__home__FEELSLIKETEMPQ;
extern lv_obj_t *GUI_Panel__home__details_separator_1_panel;
extern lv_obj_t *GUI_Container__home__details_humidity_cont;
extern lv_obj_t *GUI_Label__home__details_humidity_title_lbl;
extern lv_obj_t *GUI_Label__home__HUMIDITYPERCENTQ;
extern lv_obj_t *GUI_Panel__home__details_separator_2_panel;
extern lv_obj_t *GUI_Container__home__details_wind_cont;
extern lv_obj_t *GUI_Label__home__details_wind_title_lbl;
extern lv_obj_t *GUI_Container__home__CURRENTWINDQ;
extern lv_obj_t *GUI_Label__home__details_wind_speed_lbl;
extern lv_obj_t *GUI_Container__home__container_15;
extern lv_obj_t *GUI_Container__home__NETWORKSTAUSANDMENUCONT;
extern lv_obj_t *GUI_Panel__home__panel;
extern lv_obj_t *GUI_Image__home__NETWORKCONNECTEDICONQ;
extern lv_obj_t *GUI_Image__home__SETTINGSBUTTON;

extern lv_obj_t *GUI_Screen__keyboardpassword;
extern lv_obj_t *GUI_Container__keyboardpassword__container;
extern lv_obj_t *GUI_Textarea__keyboardpassword__textareapassword;
extern lv_obj_t *GUI_Keyboard__keyboardpassword__keyboardpass;
extern lv_obj_t *GUI_Container__keyboardpassword__container_1;
extern lv_obj_t *GUI_Button__keyboardpassword__button_3;
extern lv_obj_t *GUI_Label__keyboardpassword__label_10;
extern lv_obj_t *GUI_Button__keyboardpassword__button_2;
extern lv_obj_t *GUI_Label__keyboardpassword__label_9;

extern lv_obj_t *GUI_Screen__keyboardzipcode;
extern lv_obj_t *GUI_Container__keyboardzipcode__container_2;
extern lv_obj_t *GUI_Textarea__keyboardzipcode__textarea_zipcode;
extern lv_obj_t *GUI_Keyboard__keyboardzipcode__keyboardzip;
extern lv_obj_t *GUI_Container__keyboardzipcode__container_12;
extern lv_obj_t *GUI_Button__keyboardzipcode__button_5;
extern lv_obj_t *GUI_Label__keyboardzipcode__label_12;
extern lv_obj_t *GUI_Button__keyboardzipcode__button_4;
extern lv_obj_t *GUI_Label__keyboardzipcode__label_11;

extern lv_obj_t *GUI_Screen__settingswindow;
extern lv_obj_t *GUI_Container__settingswindow__container_3;
extern lv_obj_t *GUI_Panel__settingswindow__panel_1;
extern lv_obj_t *GUI_Container__settingswindow__container_4;
extern lv_obj_t *GUI_Label__settingswindow__labelsettings;
extern lv_obj_t *GUI_Checkbox__settingswindow__checkbox;
extern lv_obj_t *GUI_Label__settingswindow__label;
extern lv_obj_t *GUI_Slider__settingswindow__slider;
extern lv_obj_t *GUI_Label__settingswindow__label_1;
extern lv_obj_t *GUI_Slider__settingswindow__slider_1;
extern lv_obj_t *GUI_Container__settingswindow__container_5;
extern lv_obj_t *GUI_Label__settingswindow__label_3;
extern lv_obj_t *GUI_Container__settingswindow__container_6;
extern lv_obj_t *GUI_Label__settingswindow__label_4;
extern lv_obj_t *GUI_Textarea__settingswindow__textarea_1;
extern lv_obj_t *GUI_Container__settingswindow__container_8;
extern lv_obj_t *GUI_Label__settingswindow__label_6;
extern lv_obj_t *GUI_Textarea__settingswindow__textarea_2;
extern lv_obj_t *GUI_Container__settingswindow__container_9;
extern lv_obj_t *GUI_Label__settingswindow__label_7;
extern lv_obj_t *GUI_Textarea__settingswindow__textarea_3;
extern lv_obj_t *GUI_Container__settingswindow__container_7;
extern lv_obj_t *GUI_Button__settingswindow__button_8;
extern lv_obj_t *GUI_Label__settingswindow__label_5;
extern lv_obj_t *GUI_Button__settingswindow__button_1;
extern lv_obj_t *GUI_Label__settingswindow__label_2;

extern lv_obj_t *GUI_Screen__networkselector;
extern lv_obj_t *GUI_Container__networkselector__container_10;
extern lv_obj_t *GUI_Panel__networkselector__panel_2;
extern lv_obj_t *GUI_Label__networkselector__label_8;
extern lv_obj_t *GUI_Textarea__networkselector__textarea;
extern lv_obj_t *GUI_Container__networkselector__container_11;
extern lv_obj_t *GUI_Button__networkselector__button_7;
extern lv_obj_t *GUI_Label__networkselector__label_14;
extern lv_obj_t *GUI_Button__networkselector__button_6;
extern lv_obj_t *GUI_Label__networkselector__label_13;

extern lv_obj_t *GUI_Screen__screen;
extern lv_obj_t *GUI_Image__screen__image;
extern lv_obj_t *GUI_Image__screen__image_1;
extern lv_obj_t *GUI_Image__screen__image_2;
extern lv_obj_t *GUI_Image__screen__image_3;
extern lv_obj_t *GUI_Image__screen__image_4;
extern lv_obj_t *GUI_Image__screen__image_5;
extern lv_obj_t *GUI_Image__screen__image_6;
extern lv_obj_t *GUI_Image__screen__image_7;
extern lv_obj_t *GUI_Image__screen__image_8;
extern lv_obj_t *GUI_Image__screen__image_9;
extern lv_obj_t *GUI_Image__screen__image_10;
extern lv_obj_t *GUI_Image__screen__image_11;
extern lv_obj_t *GUI_Image__screen__image_12;
extern lv_obj_t *GUI_Image__screen__image_13;
extern lv_obj_t *GUI_Image__screen__image_14;
extern lv_obj_t *GUI_Image__screen__image_15;
extern lv_obj_t *GUI_Image__screen__image_16;
extern lv_obj_t *GUI_Image__screen__image_17;
extern lv_obj_t *GUI_Image__screen__image_18;
extern lv_obj_t *GUI_Image__screen__image_19;
extern lv_obj_t *GUI_Image__screen__image_20;
extern lv_obj_t *GUI_Image__screen__image_21;
extern lv_obj_t *GUI_Image__screen__image_22;
extern lv_obj_t *GUI_Image__screen__image_23;
extern lv_obj_t *GUI_Image__screen__image_24;
extern lv_obj_t *GUI_Image__screen__image_25;
extern lv_obj_t *GUI_Image__screen__image_26;
extern lv_obj_t *GUI_Image__screen__image_27;
extern lv_obj_t *GUI_Image__screen__image_28;
extern lv_obj_t *GUI_Image__screen__image_29;
extern lv_obj_t *GUI_Image__screen__image_30;
extern lv_obj_t *GUI_Image__screen__image_31;
extern lv_obj_t *GUI_Image__screen__image_32;
extern lv_obj_t *GUI_Image__screen__image_33;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
