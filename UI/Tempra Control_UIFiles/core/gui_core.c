#include "../GUI.h"

void GUI_load() {
  GUI_initFramework();
  GUI_initHAL();
  GUI_loadContent();
}

void GUI_init() {
  GUI_loadContent();
}

void GUI_refresh() {
  lv_timer_handler();
  //...
}

void GUI_initHAL() {
  HAL_init();
}

void GUI_initFramework() {
  lv_init();
  LV_EVENT_GET_COMP_CHILD = lv_event_register_id();
}

void GUI_loadContent() {
  GUI_initContent();
  GUI_loadFirstScreen();
}

void GUI_initContent() {
  GUI_initTheme();
  GUI_initGlobalStyles();
  GUI_initScreens();
  GUI_initAnimations();
}

void GUI_initTheme() {
  lv_display_t *Display = lv_display_get_default();
  lv_theme_t *Theme = lv_theme_simple_init(Display);
  lv_display_set_theme(Display, Theme);
}

void GUI_initScreens() {
  GUI_initScreenContents();
}

void GUI_loadFirstScreen() {
  lv_screen_load(GUI_Screen__home);
}

void GUI_initScreenContents() {
  GUI_initScreen__home();
  GUI_initScreen__keyboardpassword();
  GUI_initScreen__keyboardzipcode();
  GUI_initScreen__settingswindow();
  GUI_initScreen__networkselector();
}

void GUI_initScreenTexts() {
  GUI_initScreenTexts__home();
  GUI_initScreenTexts__keyboardpassword();
  GUI_initScreenTexts__keyboardzipcode();
  GUI_initScreenTexts__settingswindow();
  GUI_initScreenTexts__networkselector();
}

void GUI_initScreenStyles() {
  GUI_initScreenStyles__home();
  GUI_initScreenStyles__keyboardpassword();
  GUI_initScreenStyles__keyboardzipcode();
  GUI_initScreenStyles__settingswindow();
  GUI_initScreenStyles__networkselector();
}
