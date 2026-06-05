#pragma once
#include <stdbool.h>
#include "esp_err.h"

// Initialize WiFi manager — registers event handlers
esp_err_t wifi_manager_init(void);

// Connect using saved NVS credentials. Returns ESP_OK if credentials found and connect initiated.
esp_err_t wifi_manager_connect_saved(void);

// Connect with explicit SSID/password
esp_err_t wifi_manager_connect(const char *ssid, const char *password);

// Returns true if currently connected with an IP
bool wifi_manager_is_connected(void);

// Get current IP as string (returns "0.0.0.0" if not connected)
void wifi_manager_get_ip(char *buf, size_t len);
