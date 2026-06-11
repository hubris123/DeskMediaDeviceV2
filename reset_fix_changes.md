# Display Blank After Reset — Code Changes

## Status
**FIXED 2026-06-11 — verified 4/4 consecutive flash+watchdog-reset cycles.**

**Root cause:** a soft reset that interrupts active DSI video leaves the display module
wedged: panel black, backlight on, app fully healthy. Nothing un-wedges it within that
boot — GPIO 27/23 reset pulses (up to 250ms), DSI SWRESET, DSI PHY LDO power-cycle were
all tried and proven insufficient. But a wedged boot ALWAYS clears the wedge for the
next boot (perfect alternation, 7/7 observed).

**The fix (main/main.c, top of app_main):** self-healing wedge detector.
The GT911 touch controller latches I2C address 0x5d on a clean module reset and 0x14 on
a wedged one — 100% correlated with display state across every observed boot. So:
1. Pulse GPIO 27 + GPIO 23 low 250ms / high 120ms (module hardware reset attempt)
2. `bsp_i2c_init()` + `i2c_master_probe(handle, 0x14, 100)`
3. If 0x14 answers AND `esp_reset_reason() != ESP_RST_SW` → `esp_restart()`
   (stateless loop guard: only our own restart reports ESP_RST_SW; esptool watchdog
   reset and power-on both report ESP_RST_POWERON)
4. Wedged boots cost one extra ~2s restart; clean boots cost one 100ms probe

Findings along the way (all verified):
- GT911 is NOT broken hardware — works perfectly on clean boots; failures are the wedge canary
- GPIO 27 is the real panel reset (schematic: display pin 23 via R60 0R, R102 10K pulldown)
- `esp_reset_reason()` returns ESP_RST_POWERON even after esptool watchdog reset
- RTC_NOINIT_ATTR is NOT retained across esptool watchdog reset on this P4 (fresh garbage every flash)
- `LCD ID: FF FF FF` is normal for this panel (identical on working boots)
- `ledc: GPIO 26 not usable` warning appears on every boot including working ones — harmless
- DSI PHY LDO power-cycle kept in BSP (harmless, defense in depth)
- st7701 `reset_gpio_num = -1` kept (driver sends DSI SWRESET instead — defense in depth)
- TEAR_AVOID_MODE_NONE breaks ROTATE_90 (display start fails, main hangs) — must stay DOUBLE_FULL
- LVGL was never deadlocked: weather task acquired the LVGL lock fine during blank boots

---

## File 1: `components/esp32_p4_wifi6_touch_lcd_4_3/esp32_p4_wifi6_touch_lcd_4_3.c`

### Add include (near top of file, with other includes)
```c
#include "esp_system.h"   // for esp_reset_reason()
```

### Change 1 — 120ms delay between panel reset and init
Find this block:
```c
ESP_GOTO_ON_ERROR(esp_lcd_panel_reset(panel_handle), err, TAG, "LCD panel reset failed");
ESP_GOTO_ON_ERROR(esp_lcd_panel_init(panel_handle), err, TAG, "LCD panel init failed");
```
Change to:
```c
ESP_GOTO_ON_ERROR(esp_lcd_panel_reset(panel_handle), err, TAG, "LCD panel reset failed");
vTaskDelay(pdMS_TO_TICKS(120)); // ST7701 requires 120ms after hardware reset before commands
ESP_GOTO_ON_ERROR(esp_lcd_panel_init(panel_handle), err, TAG, "LCD panel init failed");
```

### Change 2 — LDO power-cycle block in `bsp_enable_dsi_phy_power()`
Find the final `esp_ldo_acquire_channel` call that stores into `phy_pwr_chan`. Insert this block **immediately before** it:
```c
// After a software/watchdog reset the LDO stays energized, so the DSI PHY
// never sees a clean power-on edge and stays wedged (backlight on, no video).
// Force a real power cycle of the PHY rail before bringing it up.
if (esp_reset_reason() != ESP_RST_POWERON)
{
    esp_ldo_channel_handle_t tmp_chan = NULL;
    if (esp_ldo_acquire_channel(&ldo_cfg, &tmp_chan) == ESP_OK)
    {
        esp_ldo_release_channel(tmp_chan); // last reference dropped -> LDO output off
        vTaskDelay(pdMS_TO_TICKS(20));     // let the PHY rail fully discharge
        ESP_LOGI(TAG, "DSI PHY LDO power-cycled after non-poweron reset");
    }
}
ESP_RETURN_ON_ERROR(esp_ldo_acquire_channel(&ldo_cfg, &phy_pwr_chan), TAG, "Acquire LDO channel for DPHY failed");
vTaskDelay(pdMS_TO_TICKS(10)); // PHY rail stabilization before DSI bus init
```

---

## File 2: `sdkconfig`

These lines must be set/unset relative to git HEAD (commit `8862372`):

```
# Fix IRAM overflow / linker error
CONFIG_FREERTOS_PLACE_FUNCTIONS_INTO_FLASH=y

# Reduce IRAM pressure (was =y)
# CONFIG_LV_ATTRIBUTE_FAST_MEM_USE_IRAM is not set

# Keep these OFF — enabling broke esp_hosted SPI timing
# CONFIG_SPIRAM_XIP_FROM_PSRAM is not set
# CONFIG_SPIRAM_FETCH_INSTRUCTIONS is not set
# CONFIG_SPIRAM_RODATA is not set
# CONFIG_SPIRAM_FLASH_LOAD_TO_PSRAM is not set

# ESP-Hosted transport — SDIO (matches C6 factory firmware and git HEAD)
# Reverted from SPI back to SDIO after transport loop failure
# SDIO pins: CMD=19, CLK=18, D0=14, D1=15, D2=16, D3=17, Reset=54
# CONFIG_ESP_HOSTED_SPI_HOST_INTERFACE is not set
CONFIG_ESP_HOSTED_SDIO_HOST_INTERFACE=y
CONFIG_ESP_HOSTED_SDIO_RESET_ACTIVE_HIGH=y
CONFIG_ESP_HOSTED_SDIO_OPTIMIZATION_RX_STREAMING_MODE=y
CONFIG_ESP_HOSTED_SDIO_GPIO_RESET_SLAVE=54
CONFIG_ESP_HOSTED_SDIO_4_BIT_BUS=y
CONFIG_ESP_HOSTED_SDIO_BUS_WIDTH=4
CONFIG_ESP_HOSTED_SDIO_CLOCK_FREQ_KHZ=40000
CONFIG_ESP_HOSTED_SDIO_PIN_CMD=19
CONFIG_ESP_HOSTED_SDIO_PIN_CLK=18
CONFIG_ESP_HOSTED_SDIO_PIN_D0=14
CONFIG_ESP_HOSTED_SDIO_PIN_D1=15
CONFIG_ESP_HOSTED_SDIO_PIN_D2=16
CONFIG_ESP_HOSTED_SDIO_PIN_D3=17
CONFIG_ESP_HOSTED_GPIO_SLAVE_RESET_SLAVE=54
```

### Why SPI failed
A previous session switched esp_hosted from SDIO to SPI. The C6 on this board has factory SDIO firmware — so the SPI handshake never completed, causing the "Not able to connect with ESP-Hosted slave device" loop. The sdkconfig has been reverted to SDIO (matching git HEAD commit `8862372`).

---

## Theory Behind the Fix

The ESP32-P4 has an internal LDO (VO3) that powers the MIPI DSI PHY. On a true power-on reset this starts de-energized, the PHY gets a clean power-on edge, and the display works. On a software or watchdog reset the LDO stays on — the PHY never resets cleanly and remains wedged: backlight lights up but no video signal is output.

The fix acquires then immediately releases the LDO channel (dropping the reference count to zero turns off the output), waits 20ms for the rail to discharge, then re-acquires it normally. This gives the PHY the clean power cycle it needs even after a soft reset.

---

## Next Steps if Still Not Working

1. Get a monitor log (`idf.py monitor`) and run through `parse_logs.ps1`
2. Search log for `"DSI PHY LDO power-cycled"` — confirms the new code path executed
3. If device not booting at all: flash git HEAD `8862372` to confirm hardware is OK, then re-apply changes one at a time
4. If power cycle works but watchdog reset still blank: the LDO discharge time (20ms) may need increasing, or the PHY needs a full DSI bus reset sequence after power-cycle
