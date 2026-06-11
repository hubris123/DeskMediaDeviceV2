# Session 2026-06-11 — Display Reset Bug SOLVED + WiFi SDIO Revert

Committed as `9ae9d2f` on top of `8862372`. Working tree clean. All fixes verified on hardware.

## What was broken at session start
1. Device stuck in endless `Not able to connect with ESP-Hosted slave device` loop (no WiFi, boot never finished)
2. Display blank (backlight on, no image) after any soft/watchdog reset — only a power cycle recovered it

## Fix 1 — WiFi: esp_hosted SPI → SDIO revert
A previous session had switched the esp_hosted transport from SDIO to SPI in sdkconfig.
The C6 slave firmware on this board speaks **SDIO** — SPI can never connect.
Reverted to SDIO matching git HEAD: pins CMD=19 CLK=18 D0-D3=14/15/16/17, slave reset GPIO 54.
Result: slave connects in ~1.3s, WiFi + weather fully working.

## Fix 2 — Display blank after soft reset (the big one)

### Root cause
A soft reset that interrupts **active DSI video streaming** leaves the display module
wedged: panel black, backlight on, app completely healthy (WiFi, weather, LVGL all run).
Nothing in software un-wedges it *within that boot* — all of these were tried and proven
insufficient: GPIO27/GPIO23 reset pulses (20–250ms), DSI SWRESET command, MIPI DSI PHY
LDO (VO3) power-cycle, ST7701 full re-init.

**But a wedged boot always clears the wedge for the next boot** — perfect alternation,
observed 7/7 (blank, work, blank, work...).

### The detector
GT911 touch controller latches its I2C address from INT pin state when its reset releases:
- **0x5d** = module reset cleanly = display will work
- **0x14** = module wedged = display will be black
100% correlated in every observed boot.

### The fix (main/main.c, top of app_main)
1. Pulse GPIO27 + GPIO23 low 250ms, high 120ms
2. `bsp_i2c_init()` then `i2c_master_probe(bsp_i2c_get_handle(), 0x14, 100)`
3. If 0x14 answers AND `esp_reset_reason() != ESP_RST_SW` → `esp_restart()`
4. Stateless loop guard: only our own restart reports ESP_RST_SW (3); esptool watchdog
   reset and true power-on both report ESP_RST_POWERON (1)

Cost: wedged boots take one extra ~2s restart; clean boots cost one 100ms I2C probe.
**Verified 4/4 consecutive flash+watchdog-reset cycles — display up every time.**

## Hard-won facts (do not re-learn these)
- "GT911 is broken hardware" was WRONG — it works on every clean boot; failures = wedge canary
- GPIO27 is the real panel reset (schematic: ribbon pin 23 ← R60 0R, R102 10K pulldown); GPIO23 is touch-only
- `esp_reset_reason()` returns ESP_RST_POWERON even after esptool `--after watchdog_reset`
- RTC_NOINIT_ATTR is NOT retained across esptool watchdog reset on this P4 (fresh garbage every flash)
- `LCD ID: FF FF FF` is normal for this panel — identical on working boots
- `W ledc: GPIO 26 not usable` appears every boot including working ones — harmless
- TEAR_AVOID_MODE_NONE rejects ROTATE_90 → display start fails → main hangs w/ task_wdt. Must stay DOUBLE_FULL
- LVGL was never deadlocked during blank boots (weather task got the LVGL lock fine)
- Defense-in-depth kept in place: DSI PHY LDO power-cycle in BSP, st7701 `reset_gpio_num=-1`
  (driver sends DSI SWRESET), 120ms post-reset delay before panel init

## Build config state (vs 8862372)
- `CONFIG_FREERTOS_PLACE_FUNCTIONS_INTO_FLASH=y` (fixes IRAM overflow linker error)
- `CONFIG_LV_ATTRIBUTE_FAST_MEM_USE_IRAM` off (IRAM pressure)
- All SPIRAM XIP options off (XIP breaks esp_hosted timing)
- esp_hosted = SDIO (see Fix 1)

## New tooling
- `tools/flash_and_reset.ps1` — flash with `--after watchdog_reset` (no manual power cycle)
- `tools/catch_boot_log.ps1` — start BEFORE flashing; waits for esptool to take then release
  COM4, grabs the port instantly with DTR/RTS disabled, logs boot to boot_log.txt.
  Also works for power-cycle capture (unplug → it waits → replug → captures from boot)
- Repo hygiene: build/ untracked, imagecapture/ + boot_log*.txt ignored

## Open items for next session
- Switch zip code back to Las Vegas 89031 (currently Davenport 52801) — user does manually on device
- NWS weather migration files committed as WIP (main/weather/nws_api.c/.h) — continue per NWS migration plan
- GT911 touch now WORKS on clean boots — any "expect touch errors" handling can likely be tightened
