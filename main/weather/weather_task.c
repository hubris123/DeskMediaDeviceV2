#include "weather_task.h"
#include "weather_api.h"
#include "nvs_storage.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_err.h"
#include <string.h>
#include <time.h>

static const char *TAG = "WeatherTask";

/**
 * Task internal state
 */
typedef struct {
    TaskHandle_t task_handle;
    SemaphoreHandle_t data_mutex;
    QueueHandle_t cmd_queue;
    weather_data_t data;
    location_t location;
    uint32_t update_interval_sec;
    uint32_t last_update;
    bool initialized;
} weather_task_state_t;

static weather_task_state_t g_state = {0};

/**
 * Commands for the task
 */
typedef enum {
    CMD_UPDATE_NOW,
    CMD_SET_LOCATION,
    CMD_STOP,
} task_cmd_t;

typedef struct {
    task_cmd_t cmd;
    union {
        char zip[10];
    } data;
} task_msg_t;

/**
 * Weather background task main loop
 */
static void weather_task_main(void *param)
{
    ESP_LOGI(TAG, "Weather task started");
    task_msg_t msg = {0};

    // Try to load location from NVS on startup
    if (nvs_load_location(&g_state.location) == ESP_OK) {
        ESP_LOGI(TAG, "Loaded location from NVS: %s, %s",
                g_state.location.city, g_state.location.state);
    } else {
        ESP_LOGW(TAG, "No location in NVS, waiting for SET_LOCATION");
    }

    while (1) {
        // Wait for either command or timeout
        if (xQueueReceive(g_state.cmd_queue, &msg, pdMS_TO_TICKS(g_state.update_interval_sec * 1000)) == pdTRUE) {
            // Got a command
            switch (msg.cmd) {
                case CMD_UPDATE_NOW:
                    ESP_LOGI(TAG, "Force update requested");
                    break;

                case CMD_SET_LOCATION:
                    ESP_LOGI(TAG, "Setting location to ZIP: %s", msg.data.zip);
                    // Wait for WiFi before geocoding
                    {
                        int wait_count = 0;
                        while (!weather_is_wifi_ready() && wait_count < 30) {
                            ESP_LOGI(TAG, "Waiting for WiFi before geocoding... (%d)", wait_count);
                            vTaskDelay(pdMS_TO_TICKS(1000));
                            wait_count++;
                        }
                    }
                    if (!weather_is_wifi_ready()) {
                        ESP_LOGW(TAG, "WiFi not ready after wait, skipping geocode");
                        continue;
                    }
                    if (weather_geocode_zipcode(msg.data.zip, &g_state.location) == ESP_OK) {
                        nvs_store_location(&g_state.location);
                        ESP_LOGI(TAG, "Location set: %s, %s", g_state.location.city, g_state.location.state);
                    } else {
                        ESP_LOGE(TAG, "Failed to geocode ZIP: %s", msg.data.zip);
                        continue;  // Skip weather fetch
                    }
                    break;

                case CMD_STOP:
                    ESP_LOGI(TAG, "Weather task stopping");
                    vTaskDelete(NULL);
                    return;

                default:
                    ESP_LOGW(TAG, "Unknown command: %d", msg.cmd);
                    continue;
            }
        }

        // Perform weather update if location is set
        if (strlen(g_state.location.city) == 0) {
            ESP_LOGW(TAG, "Location not set, skipping update");
            continue;
        }

        if (!weather_is_wifi_ready()) {
            ESP_LOGW(TAG, "WiFi not connected, skipping update");
            continue;
        }

        // Fetch weather
        weather_data_t temp_data = {0};
        esp_err_t err = weather_fetch_current(&g_state.location, &temp_data);

        if (err == ESP_OK) {
            // Update shared data with mutex
            xSemaphoreTake(g_state.data_mutex, portMAX_DELAY);
            memcpy(&g_state.data, &temp_data, sizeof(weather_data_t));
            g_state.last_update = (uint32_t)time(NULL);
            xSemaphoreGive(g_state.data_mutex);

            // Store to NVS for offline fallback
            nvs_store_weather(&g_state.data);

            ESP_LOGI(TAG, "Weather updated: %.1f°F, %d%%",
                    g_state.data.current_temp, g_state.data.current_humidity);
        } else {
            ESP_LOGW(TAG, "Weather fetch failed, trying to load cached data");

            // Try to load from NVS as fallback
            xSemaphoreTake(g_state.data_mutex, portMAX_DELAY);
            if (nvs_load_weather(&g_state.data) != ESP_OK) {
                ESP_LOGW(TAG, "No cached weather data available");
                // Mark data as stale but valid
                g_state.data.is_valid = false;
            }
            xSemaphoreGive(g_state.data_mutex);
        }
    }
}

// ============ Public API ============

esp_err_t weather_task_start(void)
{
    if (g_state.initialized) {
        ESP_LOGW(TAG, "Weather task already running");
        return ESP_OK;
    }

    // Create mutex for data access
    g_state.data_mutex = xSemaphoreCreateMutex();
    if (!g_state.data_mutex) {
        ESP_LOGE(TAG, "Failed to create mutex");
        return ESP_FAIL;
    }

    // Create queue for commands
    g_state.cmd_queue = xQueueCreate(5, sizeof(task_msg_t));
    if (!g_state.cmd_queue) {
        ESP_LOGE(TAG, "Failed to create command queue");
        vSemaphoreDelete(g_state.data_mutex);
        return ESP_FAIL;
    }

    // Initialize state
    g_state.update_interval_sec = 15 * 60;  // 15 minutes
    g_state.last_update = 0;
    memset(&g_state.data, 0, sizeof(weather_data_t));
    memset(&g_state.location, 0, sizeof(location_t));

    // Create task
    BaseType_t ret = xTaskCreate(weather_task_main,
                                "weather_task",
                                8192,  // 8KB stack — needed for HTTPS SSL
                                NULL,
                                3,     // Low priority
                                &g_state.task_handle);

    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create task");
        vSemaphoreDelete(g_state.data_mutex);
        vQueueDelete(g_state.cmd_queue);
        return ESP_FAIL;
    }

    g_state.initialized = true;
    ESP_LOGI(TAG, "Weather task initialized");
    return ESP_OK;
}

esp_err_t weather_task_stop(void)
{
    if (!g_state.initialized) {
        return ESP_OK;
    }

    task_msg_t msg = {.cmd = CMD_STOP};
    if (xQueueSend(g_state.cmd_queue, &msg, pdMS_TO_TICKS(1000)) != pdTRUE) {
        ESP_LOGW(TAG, "Failed to send STOP command");
        return ESP_FAIL;
    }

    vTaskDelay(pdMS_TO_TICKS(100));

    if (g_state.data_mutex) vSemaphoreDelete(g_state.data_mutex);
    if (g_state.cmd_queue) vQueueDelete(g_state.cmd_queue);

    g_state.initialized = false;
    return ESP_OK;
}

esp_err_t weather_task_update_now(void)
{
    if (!g_state.initialized) {
        return ESP_FAIL;
    }

    task_msg_t msg = {.cmd = CMD_UPDATE_NOW};
    if (xQueueSend(g_state.cmd_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE) {
        ESP_LOGW(TAG, "Failed to queue update command");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t weather_get_data(weather_data_t *out)
{
    if (!out || !g_state.initialized) {
        return ESP_ERR_INVALID_ARG;
    }

    if (xSemaphoreTake(g_state.data_mutex, pdMS_TO_TICKS(500)) != pdTRUE) {
        ESP_LOGW(TAG, "Failed to acquire data mutex");
        return ESP_FAIL;
    }

    if (!g_state.data.is_valid) {
        xSemaphoreGive(g_state.data_mutex);
        return ESP_FAIL;
    }

    memcpy(out, &g_state.data, sizeof(weather_data_t));
    xSemaphoreGive(g_state.data_mutex);

    return ESP_OK;
}

esp_err_t weather_set_location(const char *zip)
{
    if (!zip || !g_state.initialized) {
        return ESP_ERR_INVALID_ARG;
    }

    task_msg_t msg = {.cmd = CMD_SET_LOCATION};
    strncpy(msg.data.zip, zip, sizeof(msg.data.zip) - 1);
    msg.data.zip[sizeof(msg.data.zip) - 1] = '\0';

    if (xQueueSend(g_state.cmd_queue, &msg, pdMS_TO_TICKS(100)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to queue location command");
        return ESP_FAIL;
    }

    return ESP_OK;
}

void weather_set_update_interval(uint32_t minutes)
{
    if (minutes < 1) minutes = 1;
    if (minutes > 1440) minutes = 1440;  // Max 24 hours

    g_state.update_interval_sec = minutes * 60;
    ESP_LOGI(TAG, "Update interval set to %lu minutes", minutes);
}

uint32_t weather_get_last_update(void)
{
    return g_state.last_update;
}
