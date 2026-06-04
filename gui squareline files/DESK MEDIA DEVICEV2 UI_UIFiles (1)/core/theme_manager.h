#ifndef _UI_THEMES_H
#define _UI_THEMES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ui_theme_manager.h"

#define UI_THEME_COLOR_WHITE 0
#define UI_THEME_COLOR_GREEN 1
#define UI_THEME_COLOR_ORANGE 2
#define UI_THEME_COLOR_BLACK 3
#define UI_THEME_COLOR_RED 4
#define UI_THEME_COLOR_BLUE 5
#define UI_THEME_COLOR_LIGHT_GRAY 6
#define UI_THEME_COLOR_DARK_GRAY 7
#define UI_THEME_COLOR_TEMPHIGH 8
#define UI_THEME_COLOR_TEMPLOW 9

#define UI_THEME_DEFAULT 0

// Declarations for gradient style 'White'
extern const ui_theme_variable_t _ui_theme_bg_color_white[1];
extern const ui_style_variable_t _ui_theme_bg_opa_white[1];
extern const ui_style_variable_t _ui_theme_grad_color_white[1];
extern const ui_style_variable_t _ui_theme_grad_opa_white[1];
extern const ui_style_variable_t _ui_theme_grad_dir_white[1];
extern const ui_style_variable_t _ui_theme_main_stop_white[1];
extern const ui_style_variable_t _ui_theme_grad_stop_white[1];

// Declarations for gradient style 'Green'
extern const ui_theme_variable_t _ui_theme_bg_color_green[1];
extern const ui_style_variable_t _ui_theme_bg_opa_green[1];
extern const ui_style_variable_t _ui_theme_grad_color_green[1];
extern const ui_style_variable_t _ui_theme_grad_opa_green[1];
extern const ui_style_variable_t _ui_theme_grad_dir_green[1];
extern const ui_style_variable_t _ui_theme_main_stop_green[1];
extern const ui_style_variable_t _ui_theme_grad_stop_green[1];

// Declarations for gradient style 'Orange'
extern const ui_theme_variable_t _ui_theme_bg_color_orange[1];
extern const ui_style_variable_t _ui_theme_bg_opa_orange[1];
extern const ui_style_variable_t _ui_theme_grad_color_orange[1];
extern const ui_style_variable_t _ui_theme_grad_opa_orange[1];
extern const ui_style_variable_t _ui_theme_grad_dir_orange[1];
extern const ui_style_variable_t _ui_theme_main_stop_orange[1];
extern const ui_style_variable_t _ui_theme_grad_stop_orange[1];

// Declarations for gradient style 'Black'
extern const ui_theme_variable_t _ui_theme_bg_color_black[1];
extern const ui_style_variable_t _ui_theme_bg_opa_black[1];
extern const ui_style_variable_t _ui_theme_grad_color_black[1];
extern const ui_style_variable_t _ui_theme_grad_opa_black[1];
extern const ui_style_variable_t _ui_theme_grad_dir_black[1];
extern const ui_style_variable_t _ui_theme_main_stop_black[1];
extern const ui_style_variable_t _ui_theme_grad_stop_black[1];

// Declarations for gradient style 'Red'
extern const ui_theme_variable_t _ui_theme_bg_color_red[1];
extern const ui_style_variable_t _ui_theme_bg_opa_red[1];
extern const ui_style_variable_t _ui_theme_grad_color_red[1];
extern const ui_style_variable_t _ui_theme_grad_opa_red[1];
extern const ui_style_variable_t _ui_theme_grad_dir_red[1];
extern const ui_style_variable_t _ui_theme_main_stop_red[1];
extern const ui_style_variable_t _ui_theme_grad_stop_red[1];

// Declarations for gradient style 'Blue'
extern const ui_theme_variable_t _ui_theme_bg_color_blue[1];
extern const ui_style_variable_t _ui_theme_bg_opa_blue[1];
extern const ui_style_variable_t _ui_theme_grad_color_blue[1];
extern const ui_style_variable_t _ui_theme_grad_opa_blue[1];
extern const ui_style_variable_t _ui_theme_grad_dir_blue[1];
extern const ui_style_variable_t _ui_theme_main_stop_blue[1];
extern const ui_style_variable_t _ui_theme_grad_stop_blue[1];

// Declarations for gradient style 'Light Gray'
extern const ui_theme_variable_t _ui_theme_bg_color_light_gray[1];
extern const ui_style_variable_t _ui_theme_bg_opa_light_gray[1];
extern const ui_style_variable_t _ui_theme_grad_color_light_gray[1];
extern const ui_style_variable_t _ui_theme_grad_opa_light_gray[1];
extern const ui_style_variable_t _ui_theme_grad_dir_light_gray[1];
extern const ui_style_variable_t _ui_theme_main_stop_light_gray[1];
extern const ui_style_variable_t _ui_theme_grad_stop_light_gray[1];

// Declarations for gradient style 'Dark Gray'
extern const ui_theme_variable_t _ui_theme_bg_color_dark_gray[1];
extern const ui_style_variable_t _ui_theme_bg_opa_dark_gray[1];
extern const ui_style_variable_t _ui_theme_grad_color_dark_gray[1];
extern const ui_style_variable_t _ui_theme_grad_opa_dark_gray[1];
extern const ui_style_variable_t _ui_theme_grad_dir_dark_gray[1];
extern const ui_style_variable_t _ui_theme_main_stop_dark_gray[1];
extern const ui_style_variable_t _ui_theme_grad_stop_dark_gray[1];

// Declarations for gradient style 'temphigh'
extern const ui_theme_variable_t _ui_theme_bg_color_temphigh[1];
extern const ui_style_variable_t _ui_theme_bg_opa_temphigh[1];
extern const ui_style_variable_t _ui_theme_grad_color_temphigh[1];
extern const ui_style_variable_t _ui_theme_grad_opa_temphigh[1];
extern const ui_style_variable_t _ui_theme_grad_dir_temphigh[1];
extern const ui_style_variable_t _ui_theme_main_stop_temphigh[1];
extern const ui_style_variable_t _ui_theme_grad_stop_temphigh[1];

// Declarations for gradient style 'templow'
extern const ui_theme_variable_t _ui_theme_bg_color_templow[1];
extern const ui_style_variable_t _ui_theme_bg_opa_templow[1];
extern const ui_style_variable_t _ui_theme_grad_color_templow[1];
extern const ui_style_variable_t _ui_theme_grad_opa_templow[1];
extern const ui_style_variable_t _ui_theme_grad_dir_templow[1];
extern const ui_style_variable_t _ui_theme_main_stop_templow[1];
extern const ui_style_variable_t _ui_theme_grad_stop_templow[1];

extern uint8_t ui_theme_idx;

void ui_theme_set(uint8_t theme_idx);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //_UI_THEMES_H
