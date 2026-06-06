#pragma once

#include "esp_err.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

// Exposed by main.c — the LVGL display handle needed for dummy_draw_blit
extern lv_display_t *g_lv_disp;

/**
 * Call once at boot after SD card is mounted.
 * Scans /sdcard/video/ for *.mjpeg files, builds a shuffled playlist,
 * and pre-loads the first two clips into PSRAM.
 */
void video_player_init(void);

/**
 * Launch video playback in a new FreeRTOS task.
 * Pauses LVGL, plays clips from the shuffled playlist (with matching .mp3),
 * then resumes LVGL when done.
 * Safe to call from an LVGL event callback.
 */
void video_player_start(void);

#ifdef __cplusplus
}
#endif
