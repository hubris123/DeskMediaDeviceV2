#pragma once

#include "esp_err.h"

/**
 * Play a short tick sound (generated in memory, no SD card needed).
 * Safe to call from LVGL event callbacks — runs on a background task.
 */
void audio_play_tick(void);

/**
 * Set speaker volume 0-100.
 */
void audio_set_volume(int percent);

/**
 * Play the WAV file from SD card (non-blocking).
 * Used by the volume slider preview.
 */
void audio_play_wav_preview(void);

/**
 * Play SUCCESS.WAV from SD card (non-blocking).
 * Used by the save button.
 */
void audio_play_success(void);

/**
 * Play an MP3 file from SD card (non-blocking).
 * @param path  Full path e.g. "/sdcard/SUCCESS.MP3"
 */
void audio_play_mp3(const char *path);
