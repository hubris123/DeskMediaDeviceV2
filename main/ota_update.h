#pragma once

/**
 * OTA firmware updates from GitHub releases.
 *
 * Flow: a background task waits for WiFi, then queries
 * https://api.github.com/repos/hubris123/DeskMediaDeviceV2/releases/latest
 * once shortly after boot and every 24h after. If the release tag differs
 * from the running firmware version and the release has a .bin asset, an
 * on-screen prompt offers Install / Later. Install streams the image into
 * the inactive OTA slot via esp_https_ota and reboots.
 *
 * Rollback: CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE is on. Call
 * ota_update_mark_boot_valid() once the system is demonstrably healthy;
 * until then a freshly updated image that crashes reverts on next boot.
 */

/** Start the background check task. Call once after the UI is up. */
void ota_update_start(void);

/**
 * Run an immediate "check for updates" (from the settings button). Ignores the
 * 5-day "Later" snooze and the power gate, and always gives feedback: the
 * Install/Later prompt if an update exists, or an "up to date" dialog if not.
 * Spawns its own task; safe to call from an LVGL event callback.
 */
void ota_update_check_now(void);

/** Mark the running image good (cancels pending rollback). Idempotent. */
void ota_update_mark_boot_valid(void);

#include <stdbool.h>
#include <stddef.h>
/**
 * Release tag of the RUNNING firmware, hash-validated: returns true and fills
 * buf only if the stored tag was confirmed on this exact binary. False for
 * dev/USB builds, after rollbacks, or any tag/binary mismatch.
 */
bool ota_get_installed_tag(char *buf, size_t len);
