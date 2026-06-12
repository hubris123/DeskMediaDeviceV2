#ifndef NVS_STORAGE_H
#define NVS_STORAGE_H

#include "weather_data.h"
#include "esp_err.h"
#include <stdbool.h>

/**
 * NVS Storage for Weather System
 *
 * Namespace: "weather"
 * Keys:
 *   - "city" (string, max 50)
 *   - "state" (string, max 3)
 *   - "lat" (float32)
 *   - "lon" (float32)
 *   - "timezone" (string, max 50)
 */

/**
 * Initialize NVS for weather storage
 * Must be called after nvs_flash_init()
 * @return: ESP_OK if successful
 */
esp_err_t nvs_storage_init(void);

/**
 * Store location to NVS
 * @param loc: Pointer to location_t with city, state, lat, lon
 * @return: ESP_OK if successful
 */
esp_err_t nvs_store_location(const location_t *loc);

/**
 * Load location from NVS
 * @param loc: Pointer to location_t to fill
 * @return: ESP_OK if location found and loaded
 */
esp_err_t nvs_load_location(location_t *loc);

/**
 * Check if valid location exists in NVS
 * @return: true if location found, false otherwise
 */
bool nvs_has_valid_location(void);

/**
 * Store weather data to NVS for offline fallback
 * @param weather: Pointer to weather_data_t
 * @return: ESP_OK if successful
 */
esp_err_t nvs_store_weather(const weather_data_t *weather);

/**
 * Load weather data from NVS (offline fallback)
 * @param weather: Pointer to weather_data_t to fill
 * @return: ESP_OK if weather data found and loaded
 */
esp_err_t nvs_load_weather(weather_data_t *weather);

/**
 * Clear all weather-related NVS data
 * @return: ESP_OK if successful
 */
esp_err_t nvs_clear_weather(void);

// ── WiFi credentials ──────────────────────────────────────────────────────────
esp_err_t nvs_store_wifi(const char *ssid, const char *password);
esp_err_t nvs_load_wifi(char *ssid, size_t ssid_len, char *password, size_t pass_len);

// ── Zip code ──────────────────────────────────────────────────────────────────
esp_err_t nvs_store_zipcode(const char *zip);
esp_err_t nvs_load_zipcode(char *zip, size_t len);

// ── NWS grid / station cache (stored once per ZIP, re-used across boots) ──────
#include "nws_api.h"
esp_err_t nvs_store_nws_grid(const nws_grid_t *grid);
esp_err_t nvs_load_nws_grid(nws_grid_t *grid);

// ── Display brightness (0-100) ────────────────────────────────────────────────
esp_err_t nvs_store_brightness(int value);
int       nvs_load_brightness(int default_val);

// ── Speaker volume (0-100) ────────────────────────────────────────────────────
esp_err_t nvs_store_volume(int value);
int       nvs_load_volume(int default_val);

// ── Speaker mute ──────────────────────────────────────────────────────────────
esp_err_t nvs_store_mute(bool mute);
bool      nvs_load_mute(bool default_val);

// ── Display wedge self-restart flag (see app_main wedge detector) ─────────────
esp_err_t nvs_store_wedge_restart(bool flag);
bool      nvs_load_wedge_restart(void);

// ── Installed firmware release tag (set by OTA before reboot) ────────────────
esp_err_t nvs_store_fw_tag(const char *tag);
esp_err_t nvs_load_fw_tag(char *buf, size_t len);

#endif // NVS_STORAGE_H
