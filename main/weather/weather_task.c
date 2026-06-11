#include "weather_task.h"
#include "weather_api.h"
#include "nws_api.h"
#include "nvs_storage.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_err.h"
#include <string.h>
#include <time.h>

static const char *TAG = "WeatherTask";

#define OBS_INTERVAL_SEC      (15 * 60)   // observations every 15 min
#define FORECAST_TICKS        4            // hourly+daily every 4th obs tick (60 min)

typedef struct {
    TaskHandle_t    task_handle;
    SemaphoreHandle_t data_mutex;
    QueueHandle_t   cmd_queue;
    weather_data_t  data;
    location_t      location;
    nws_grid_t      grid;
    bool            grid_valid;
    uint32_t        obs_tick;         // counts 15-min ticks; forecast on tick % FORECAST_TICKS == 0
    bool            force_full_fetch; // set after location change to fetch everything immediately
    bool            initialized;
} weather_task_state_t;

static weather_task_state_t g_state = {0};
static volatile bool s_fetching = false;

typedef enum { CMD_UPDATE_NOW, CMD_SET_LOCATION, CMD_STOP } task_cmd_t;

typedef struct {
    task_cmd_t cmd;
    union { char zip[10]; } data;
} task_msg_t;

/* ── Internal helpers ───────────────────────────────────────────────────── */

static void resolve_and_cache_grid(void)
{
    nws_grid_t grid = {0};
    if (nws_resolve_grid(g_state.location.latitude,
                         g_state.location.longitude, &grid) == ESP_OK) {
        g_state.grid       = grid;
        g_state.grid_valid = true;
        nvs_store_nws_grid(&grid);
        ESP_LOGI(TAG, "Grid resolved and cached: %s/%d,%d station=%s",
                 grid.grid_id, grid.grid_x, grid.grid_y, grid.station);
    } else {
        ESP_LOGW(TAG, "Grid resolution failed — will retry next tick");
    }
}

static void do_obs_fetch(void)
{
    weather_data_t tmp = {0};

    // Seed tmp with current data so hourly/daily fields are preserved
    if (xSemaphoreTake(g_state.data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
        memcpy(&tmp, &g_state.data, sizeof(weather_data_t));
        xSemaphoreGive(g_state.data_mutex);
    }

    snprintf(tmp.city,  sizeof(tmp.city),  "%s", g_state.location.city);
    snprintf(tmp.state, sizeof(tmp.state), "%s", g_state.location.state);

    if (nws_fetch_observations(&g_state.grid, &tmp) != ESP_OK) {
        ESP_LOGW(TAG, "Obs fetch failed");
        return;
    }

    xSemaphoreTake(g_state.data_mutex, portMAX_DELAY);
    memcpy(&g_state.data, &tmp, sizeof(weather_data_t));
    xSemaphoreGive(g_state.data_mutex);

    nvs_store_weather(&g_state.data);
}

static void do_forecast_fetch(void)
{
    weather_data_t tmp = {0};

    if (xSemaphoreTake(g_state.data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
        memcpy(&tmp, &g_state.data, sizeof(weather_data_t));
        xSemaphoreGive(g_state.data_mutex);
    }

    bool ok = true;
    if (nws_fetch_hourly(&g_state.grid, &tmp) != ESP_OK) {
        ESP_LOGW(TAG, "Hourly forecast fetch failed");
        ok = false;
    }
    if (nws_fetch_daily(&g_state.grid, &tmp) != ESP_OK) {
        ESP_LOGW(TAG, "Daily forecast fetch failed");
        ok = false;
    }

    if (ok) {
        xSemaphoreTake(g_state.data_mutex, portMAX_DELAY);
        memcpy(&g_state.data, &tmp, sizeof(weather_data_t));
        g_state.data.last_update = tmp.last_update + 1; // force main loop re-render
        xSemaphoreGive(g_state.data_mutex);
    }
}

/* ── Task main loop ─────────────────────────────────────────────────────── */

static void weather_task_main(void *param)
{
    ESP_LOGI(TAG, "Weather task started");
    task_msg_t msg = {0};

    // Load cached location + grid from NVS
    if (nvs_load_location(&g_state.location) == ESP_OK) {
        ESP_LOGI(TAG, "Location loaded: %s, %s", g_state.location.city, g_state.location.state);
        nws_grid_t cached = {0};
        if (nvs_load_nws_grid(&cached) == ESP_OK) {
            g_state.grid       = cached;
            g_state.grid_valid = true;
            ESP_LOGI(TAG, "Grid loaded from NVS: %s/%d,%d station=%s",
                     cached.grid_id, cached.grid_x, cached.grid_y, cached.station);
        } else {
            ESP_LOGW(TAG, "No cached grid — will resolve after WiFi connects");
        }
    } else {
        ESP_LOGW(TAG, "No location in NVS, waiting for SET_LOCATION");
    }

    // Load last-known weather for offline display while connecting
    xSemaphoreTake(g_state.data_mutex, portMAX_DELAY);
    nvs_load_weather(&g_state.data);
    xSemaphoreGive(g_state.data_mutex);

    g_state.force_full_fetch = true;  // fetch everything on first opportunity

    while (1) {
        TickType_t wait = pdMS_TO_TICKS((uint32_t)OBS_INTERVAL_SEC * 1000);
        bool timed_out  = (xQueueReceive(g_state.cmd_queue, &msg, wait) != pdTRUE);

        if (!timed_out) {
            switch (msg.cmd) {
                case CMD_UPDATE_NOW:
                    ESP_LOGI(TAG, "Force update requested");
                    g_state.force_full_fetch = true;
                    break;

                case CMD_SET_LOCATION: {
                    ESP_LOGI(TAG, "SET_LOCATION: ZIP %s", msg.data.zip);
                    int wait_count = 0;
                    while (!weather_is_wifi_ready() && wait_count < 30) {
                        vTaskDelay(pdMS_TO_TICKS(1000));
                        wait_count++;
                    }
                    if (!weather_is_wifi_ready()) {
                        ESP_LOGW(TAG, "WiFi not ready — skipping geocode");
                        continue;
                    }
                    if (weather_geocode_zipcode(msg.data.zip, &g_state.location) != ESP_OK) {
                        ESP_LOGE(TAG, "Geocode failed for %s", msg.data.zip);
                        continue;
                    }
                    nvs_store_location(&g_state.location);
                    g_state.grid_valid = false;
                    resolve_and_cache_grid();
                    g_state.force_full_fetch = true;
                    break;
                }

                case CMD_STOP:
                    ESP_LOGI(TAG, "Weather task stopping");
                    vTaskDelete(NULL);
                    return;

                default:
                    break;
            }
        }

        // Skip if no location configured
        if (!g_state.location.city[0]) {
            ESP_LOGW(TAG, "No location set");
            continue;
        }

        if (!weather_is_wifi_ready()) {
            ESP_LOGW(TAG, "WiFi not connected, skipping fetch");
            continue;
        }

        // Resolve grid if we have a location but no grid (e.g. first boot after WiFi)
        if (!g_state.grid_valid) {
            resolve_and_cache_grid();
            if (!g_state.grid_valid) continue;
        }

        s_fetching = true;

        bool do_forecast = g_state.force_full_fetch ||
                           (g_state.obs_tick % FORECAST_TICKS == 0);

        do_obs_fetch();
        g_state.obs_tick++;

        if (do_forecast) {
            do_forecast_fetch();
            g_state.force_full_fetch = false;
        }

        s_fetching = false;

        ESP_LOGI(TAG, "Tick %lu: obs%s", (unsigned long)g_state.obs_tick,
                 do_forecast ? " + forecast" : "");
    }
}

/* ── Public API ─────────────────────────────────────────────────────────── */

esp_err_t weather_task_start(void)
{
    if (g_state.initialized) return ESP_OK;

    g_state.data_mutex = xSemaphoreCreateMutex();
    if (!g_state.data_mutex) return ESP_FAIL;

    g_state.cmd_queue = xQueueCreate(5, sizeof(task_msg_t));
    if (!g_state.cmd_queue) {
        vSemaphoreDelete(g_state.data_mutex);
        return ESP_FAIL;
    }

    BaseType_t ret = xTaskCreate(weather_task_main, "weather_task",
                                 8192, NULL, 2, &g_state.task_handle);
    if (ret != pdPASS) {
        vSemaphoreDelete(g_state.data_mutex);
        vQueueDelete(g_state.cmd_queue);
        return ESP_FAIL;
    }

    g_state.initialized = true;
    ESP_LOGI(TAG, "Weather task initialized (obs=15min, forecast=60min)");
    return ESP_OK;
}

esp_err_t weather_task_stop(void)
{
    if (!g_state.initialized) return ESP_OK;

    task_msg_t msg = {.cmd = CMD_STOP};
    xQueueSend(g_state.cmd_queue, &msg, pdMS_TO_TICKS(1000));
    vTaskDelay(pdMS_TO_TICKS(100));

    if (g_state.data_mutex) vSemaphoreDelete(g_state.data_mutex);
    if (g_state.cmd_queue)  vQueueDelete(g_state.cmd_queue);

    g_state.initialized = false;
    return ESP_OK;
}

esp_err_t weather_task_update_now(void)
{
    if (!g_state.initialized) return ESP_FAIL;
    task_msg_t msg = {.cmd = CMD_UPDATE_NOW};
    return (xQueueSend(g_state.cmd_queue, &msg, pdMS_TO_TICKS(100)) == pdTRUE)
               ? ESP_OK : ESP_FAIL;
}

esp_err_t weather_get_data(weather_data_t *out)
{
    if (!out || !g_state.initialized) return ESP_ERR_INVALID_ARG;

    if (xSemaphoreTake(g_state.data_mutex, pdMS_TO_TICKS(500)) != pdTRUE)
        return ESP_FAIL;

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
    if (!zip || !g_state.initialized) return ESP_ERR_INVALID_ARG;

    task_msg_t msg = {.cmd = CMD_SET_LOCATION};
    strncpy(msg.data.zip, zip, sizeof(msg.data.zip) - 1);

    return (xQueueSend(g_state.cmd_queue, &msg, pdMS_TO_TICKS(100)) == pdTRUE)
               ? ESP_OK : ESP_FAIL;
}

uint32_t weather_get_last_update(void)
{
    return g_state.data.last_update;
}

bool weather_task_is_fetching(void)
{
    return s_fetching;
}
