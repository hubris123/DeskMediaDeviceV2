#pragma once

/**
 * Content sync: downloads media (music / video) from a dedicated GitHub release
 * onto the SD card so the boot-time playlist scan can pick it up.
 *
 * Source: the "content" release of the project repo, marked as a pre-release so
 * the OTA checker (which queries /releases/latest) never sees it. A
 * content_manifest.json asset in that release maps each filename to a
 * destination folder; an embedded default manifest is the offline fallback.
 *
 * Phase 1a: additive only - never deletes existing files; only downloads ones
 * that are missing or whose size differs. New content is picked up on the next
 * boot. A manual "Sync now" trigger + live re-scan come later.
 */

// Start the background sync task: waits for WiFi, syncs once, then re-checks daily.
void content_sync_start(void);

// Trigger an immediate one-off sync (used by a future "Sync now" button).
// Safe to call from an LVGL event callback - it spawns its own task.
void content_sync_now(void);
