#pragma once

#include "esp_err.h"

/**
 * Initialize settings UI — registers event callbacks on the settings button,
 * save button, exit button, and brightness slider.
 * Call once after GUI_init().
 */
void settings_ui_init(void);
