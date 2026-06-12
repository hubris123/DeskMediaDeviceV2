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

/** Mark the running image good (cancels pending rollback). Idempotent. */
void ota_update_mark_boot_valid(void);
