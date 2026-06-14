#include "power_gate.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lvgl.h"
#include "audio.h"

static const char *TAG = "PowerGate";

// Short debounce: just long enough to confirm a tap/swipe sequence has ended so
// we don't fire network mid-interaction. Deliberately small (NOT a long idle
// wall) so weather updates promptly once the user stops touching and never goes
// stale. Video playback is gated separately and fully — that's the real peak.
#define IDLE_REQUIRED_MS  5000

// Both flags live behind one spinlock so a network burst and a video can never
// both win the gate (the brownout case). Reads are plain (single word).
static portMUX_TYPE  s_mux         = portMUX_INITIALIZER_UNLOCKED;
static volatile bool s_net_busy    = false;
static volatile bool s_video_active = false;

// True once the user has stopped interacting for at least IDLE_REQUIRED_MS.
static bool idle_long_enough(void)
{
    lv_display_t *d = lv_display_get_default();
    if (!d) return true;   // display not up yet — don't block early boot work
    return lv_display_get_inactive_time(d) >= IDLE_REQUIRED_MS;
}

void power_gate_net_wait_and_begin(void)
{
    int waited = 0;
    for (;;) {
        bool got = false;
        // Cheap pre-checks (idle/video/music-panel) outside the lock, then claim atomically.
        if (!s_video_active && !music_panel_is_open() && idle_long_enough()) {
            taskENTER_CRITICAL(&s_mux);
            if (!s_net_busy && !s_video_active && !music_panel_is_open()) {
                s_net_busy = true;
                got = true;
            }
            taskEXIT_CRITICAL(&s_mux);
        }
        if (got) return;
        vTaskDelay(pdMS_TO_TICKS(1000));
        if (++waited % 30 == 0) {
            ESP_LOGI(TAG, "Holding network (video=%d, music_panel=%d, busy=%d)",
                     (int)s_video_active, (int)music_panel_is_open(), (int)s_net_busy);
        }
    }
}

void power_gate_net_end(void)
{
    s_net_busy = false;
}

bool power_gate_net_busy(void)
{
    return s_net_busy;
}

bool power_gate_video_try_begin(void)
{
    bool ok = false;
    taskENTER_CRITICAL(&s_mux);
    if (!s_net_busy) {
        s_video_active = true;
        ok = true;
    }
    taskEXIT_CRITICAL(&s_mux);
    return ok;
}

void power_gate_video_end(void)
{
    s_video_active = false;
}
