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

// Returns true while a connection attempt is in progress (cleared on success or max retries)
bool wifi_manager_is_connecting(void);

// Returns true while in persistent reconnect mode: the fast retry burst failed but
// saved credentials exist, so the manager keeps retrying every 30 s indefinitely.
// Cleared once an IP is obtained. Survives reboots (re-entered on boot if the AP is down).
bool wifi_manager_is_reconnecting(void);

// Seconds remaining until the next persistent reconnect attempt (0 when not reconnecting).
int wifi_manager_get_reconnect_countdown(void);

// Get the reason code from the last disconnect event (0 if never disconnected)
// Reason 15  = wrong password / auth failure
// Reason 202 = auth fail
// Reason 205 = AP not found
// Reason 8   = intentional disconnect (we triggered it)
uint8_t wifi_manager_get_last_disconnect_reason(void);
