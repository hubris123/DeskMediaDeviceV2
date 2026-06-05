#pragma once

// Callback type — called with the chosen SSID when user presses Save
typedef void (*network_selected_cb_t)(const char *ssid);

// Register a callback to receive the selected SSID on Save
void network_selector_register_cb(network_selected_cb_t cb);

// Call once from main after GUI_init() to wire up all callbacks
void network_selector_ui_init(void);
