#include "../GUI.h"

const ui_theme_variable_t _ui_theme_bg_color_white[1] = {0xFFFFFF};
const ui_style_variable_t _ui_theme_bg_opa_white[1] = {255};
const ui_style_variable_t _ui_theme_grad_color_white[1] = {0x000000};
const ui_style_variable_t _ui_theme_grad_opa_white[1] = {0};
const ui_style_variable_t _ui_theme_grad_dir_white[1] = {LV_GRAD_DIR_NONE};
const ui_style_variable_t _ui_theme_main_stop_white[1] = {0};
const ui_style_variable_t _ui_theme_grad_stop_white[1] = {0};

const ui_theme_variable_t _ui_theme_bg_color_green[1] = {0x8AEF2C};
const ui_style_variable_t _ui_theme_bg_opa_green[1] = {255};
const ui_style_variable_t _ui_theme_grad_color_green[1] = {0x000000};
const ui_style_variable_t _ui_theme_grad_opa_green[1] = {0};
const ui_style_variable_t _ui_theme_grad_dir_green[1] = {LV_GRAD_DIR_NONE};
const ui_style_variable_t _ui_theme_main_stop_green[1] = {0};
const ui_style_variable_t _ui_theme_grad_stop_green[1] = {0};

const ui_theme_variable_t _ui_theme_bg_color_orange[1] = {0xFFAE00};
const ui_style_variable_t _ui_theme_bg_opa_orange[1] = {255};
const ui_style_variable_t _ui_theme_grad_color_orange[1] = {0x000000};
const ui_style_variable_t _ui_theme_grad_opa_orange[1] = {0};
const ui_style_variable_t _ui_theme_grad_dir_orange[1] = {LV_GRAD_DIR_NONE};
const ui_style_variable_t _ui_theme_main_stop_orange[1] = {0};
const ui_style_variable_t _ui_theme_grad_stop_orange[1] = {0};

const ui_theme_variable_t _ui_theme_bg_color_black[1] = {0x131116};
const ui_style_variable_t _ui_theme_bg_opa_black[1] = {255};
const ui_style_variable_t _ui_theme_grad_color_black[1] = {0x000000};
const ui_style_variable_t _ui_theme_grad_opa_black[1] = {0};
const ui_style_variable_t _ui_theme_grad_dir_black[1] = {LV_GRAD_DIR_NONE};
const ui_style_variable_t _ui_theme_main_stop_black[1] = {0};
const ui_style_variable_t _ui_theme_grad_stop_black[1] = {0};

const ui_theme_variable_t _ui_theme_bg_color_red[1] = {0xCC372C};
const ui_style_variable_t _ui_theme_bg_opa_red[1] = {255};
const ui_style_variable_t _ui_theme_grad_color_red[1] = {0x000000};
const ui_style_variable_t _ui_theme_grad_opa_red[1] = {0};
const ui_style_variable_t _ui_theme_grad_dir_red[1] = {LV_GRAD_DIR_NONE};
const ui_style_variable_t _ui_theme_main_stop_red[1] = {0};
const ui_style_variable_t _ui_theme_grad_stop_red[1] = {0};

const ui_theme_variable_t _ui_theme_bg_color_blue[1] = {0x2C76DD};
const ui_style_variable_t _ui_theme_bg_opa_blue[1] = {255};
const ui_style_variable_t _ui_theme_grad_color_blue[1] = {0x000000};
const ui_style_variable_t _ui_theme_grad_opa_blue[1] = {0};
const ui_style_variable_t _ui_theme_grad_dir_blue[1] = {LV_GRAD_DIR_NONE};
const ui_style_variable_t _ui_theme_main_stop_blue[1] = {0};
const ui_style_variable_t _ui_theme_grad_stop_blue[1] = {0};

const ui_theme_variable_t _ui_theme_bg_color_light_gray[1] = {0x9B9B9B};
const ui_style_variable_t _ui_theme_bg_opa_light_gray[1] = {255};
const ui_style_variable_t _ui_theme_grad_color_light_gray[1] = {0x000000};
const ui_style_variable_t _ui_theme_grad_opa_light_gray[1] = {0};
const ui_style_variable_t _ui_theme_grad_dir_light_gray[1] = {LV_GRAD_DIR_NONE};
const ui_style_variable_t _ui_theme_main_stop_light_gray[1] = {0};
const ui_style_variable_t _ui_theme_grad_stop_light_gray[1] = {0};

const ui_theme_variable_t _ui_theme_bg_color_dark_gray[1] = {0x434240};
const ui_style_variable_t _ui_theme_bg_opa_dark_gray[1] = {255};
const ui_style_variable_t _ui_theme_grad_color_dark_gray[1] = {0x000000};
const ui_style_variable_t _ui_theme_grad_opa_dark_gray[1] = {0};
const ui_style_variable_t _ui_theme_grad_dir_dark_gray[1] = {LV_GRAD_DIR_NONE};
const ui_style_variable_t _ui_theme_main_stop_dark_gray[1] = {0};
const ui_style_variable_t _ui_theme_grad_stop_dark_gray[1] = {0};

const ui_theme_variable_t _ui_theme_bg_color_temphigh[1] = {0xFFE500};
const ui_style_variable_t _ui_theme_bg_opa_temphigh[1] = {255};
const ui_style_variable_t _ui_theme_grad_color_temphigh[1] = {0x000000};
const ui_style_variable_t _ui_theme_grad_opa_temphigh[1] = {0};
const ui_style_variable_t _ui_theme_grad_dir_temphigh[1] = {LV_GRAD_DIR_NONE};
const ui_style_variable_t _ui_theme_main_stop_temphigh[1] = {0};
const ui_style_variable_t _ui_theme_grad_stop_temphigh[1] = {0};

const ui_theme_variable_t _ui_theme_bg_color_templow[1] = {0x6DA9FC};
const ui_style_variable_t _ui_theme_bg_opa_templow[1] = {255};
const ui_style_variable_t _ui_theme_grad_color_templow[1] = {0x000000};
const ui_style_variable_t _ui_theme_grad_opa_templow[1] = {0};
const ui_style_variable_t _ui_theme_grad_dir_templow[1] = {LV_GRAD_DIR_NONE};
const ui_style_variable_t _ui_theme_main_stop_templow[1] = {0};
const ui_style_variable_t _ui_theme_grad_stop_templow[1] = {0};

uint8_t ui_theme_idx = UI_THEME_DEFAULT;

void ui_theme_set(uint8_t theme_idx) {
  ui_theme_idx = theme_idx;
  _ui_theme_set_variable_styles(UI_VARIABLE_STYLES_MODE_FOLLOW);
  _ui_theme_update_images();
}
