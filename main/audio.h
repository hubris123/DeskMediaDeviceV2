#pragma once

#include "esp_err.h"
#include <stdbool.h>

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
 * Mute / unmute the speaker output at the codec.
 * Independent of volume — volume changes while muted stay silent.
 */
void audio_set_mute(bool mute);

/**
 * Pause / resume MP3 playback (gapless; no-op when nothing is playing).
 * Used when the settings screen is open so UI sounds stay audible.
 */
void audio_music_set_paused(bool paused);

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
