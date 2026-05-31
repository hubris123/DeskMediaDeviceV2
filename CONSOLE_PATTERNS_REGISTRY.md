# Console Patterns Registry — Known Safe Patterns

## Overview

This registry documents expected console output that appears during normal operation on the ESP32-P4 board. When these patterns appear, **no action is required** — they can be safely ignored in debugging.

Reference this file instead of re-investigating the same patterns repeatedly. Saves significant quota by avoiding redundant analysis.

---

## Touch (GT911) — Hardware Issue

### Pattern 1: I2C Transaction Failed @ 0x14
```
E (xxxx) lcd_panel.io.i2c: i2c transaction failed
E (xxxx) GT911: touch_gt911_read_cfg: GT911 read error!
```

**Why it happens:** GT911 capacitive touch controller has a hardware wiring issue on this board revision. Unable to communicate via I2C.

**Is it a problem?** NO. Non-fatal. The app continues normally.

**How it's handled:** `CONFIG_BSP_ERROR_CHECK=n` in sdkconfig prevents the error from aborting the entire application.

**What to do:** Ignore it. Touch won't work, but audio and display will function perfectly.

**First seen:** Session 2 (May 30, 2026)

**Reference:** `docs/BOARD_TIPS_AND_QUIRKS.md` — Touch section

---

### Pattern 2: Touch Initialization Delay During Boot
```
I (xxxxx) GT911: touch_gt911_init: GT911 driver init ok
```

Followed immediately by ~130ms of silence or blank LVGL frames.

**Why it happens:** GT911 init attempts to read configuration from the touch controller (~130ms). During this window, LVGL renders blank frames because GT911 is blocking.

**Is it a problem?** NO. Expected behavior. Final display frame is not blank (UI loads correctly after).

**What to do:** Ignore the blank frames during startup. Device boots normally.

**First seen:** Session 2 (May 30, 2026)

---

## Display (ST7701S via MIPI DSI)

### Pattern 1: Display Lock/Unlock Messages
```
I (xxxxx) bsp_display: bsp_display_lock
I (xxxxx) bsp_display: bsp_display_unlock
```

**Why it happens:** LVGL rendering requires the display to be locked (prevents concurrent DMA writes). Normal operation.

**Is it a problem?** NO. This is expected synchronization.

**What to do:** Ignore. These are benign debug messages from the BSP.

**First seen:** Session 1 (May 29, 2026)

---

### Pattern 2: LVGL Adapter Initialization
```
I (xxxxx) ESP_LV_ADAPTER: LVGL adapter initialized
I (xxxxx) ESP_LV_ADAPTER: Registered flush callback
```

**Why it happens:** LVGL display adapter initialization during BSP setup.

**Is it a problem?** NO. Normal boot sequence.

**What to do:** Ignore.

**First seen:** Session 1

---

## Audio (ES8311 Codec)

### Pattern 1: I2C Bus Access During Codec Init
```
I (xxxxx) i2c: I2S codec initialization via I2C bus 1
I (xxxxx) ES8311: Codec initialized, volume 80%
```

May be followed by brief I2S clock messages:

```
I (xxxxx) i2s: I2S DMA channel enabled
```

**Why it happens:** ES8311 codec initialization requires multiple I2C writes (for register config). I2S DMA startup happens after display is stable (500ms delay prevents blank screen).

**Is it a problem?** NO. These are necessary initialization steps.

**What to do:** Ignore. Audio output is working if you see "Codec initialized".

**First seen:** Session 3 (May 30, 2026)

---

### Pattern 2: Audio Playback Messages
```
I (xxxxx) DeskMediaDevice: Playing PCM: /sdcard/CANON.PCM
I (xxxxx) audio_task: End of file reached
I (xxxxx) DeskMediaDevice: Playback finished
```

**Why it happens:** Normal audio playback lifecycle.

**Is it a problem?** NO. Expected during audio operations.

**What to do:** Ignore (or use to verify audio is working).

**First seen:** Session 3

---

## SD Card

### Pattern 1: SD Card Mount Messages
```
I (xxxxx) bsp_sdcard: SD card mounted successfully at /sdcard
I (xxxxx) DeskMediaDevice: Found PCM file: /sdcard/CANON.PCM
I (xxxxx) DeskMediaDevice: Found WAV file: /sdcard/CANON.WAV
```

**Why it happens:** Normal SD card initialization and file discovery.

**Is it a problem?** NO. Expected if SD card is present and files exist.

**What to do:** Ignore. Verify files are found if audio playback is the goal.

**First seen:** Session 1

---

### Pattern 2: No Audio Files Found
```
W (xxxxx) DeskMediaDevice: No audio files found on SD card
```

**Why it happens:** SD card is mounted but no .pcm or .wav files in root directory.

**Is it a problem?** Only if you expect audio files to be there. If intentional, ignore.

**What to do:** Check `/sdcard` root directory for .pcm or .wav files. Add them if missing.

**First seen:** Session 1

---

## Memory & Linker

### Pattern 1: PSRAM Allocation
```
I (xxxxx) esp_psram: Found 32MB PSRAM chip
I (xxxxx) psram: PSRAM initialized, cache is in low/high mode
```

**Why it happens:** Board has 32MB PSRAM + 512KB internal RAM. Both are initialized on boot.

**Is it a problem?** NO. Expected for this board.

**What to do:** Ignore. Confirms memory is available for display framebuffer.

**First seen:** Session 1

---

## Build-Time Warnings (Safe to Ignore)

### Pattern 1: Unused Variable Warnings
```
warning: unused variable 'x' [-Wunused-variable]
```

**Why it happens:** Code has variables declared but not used. Common in development.

**Is it a problem?** NO. Warning only, doesn't affect functionality.

**What to do:** Ignore during development. Can clean up later with `-Wno-unused-variable` if desired.

**First seen:** Session 1

---

### Pattern 2: Comparison Signed/Unsigned
```
warning: comparison between signed and unsigned integer expressions [-Wsign-compare]
```

**Why it happens:** Type mismatch in comparisons (e.g., `int` vs `size_t`).

**Is it a problem?** NO. Minor type safety issue, doesn't affect behavior.

**What to do:** Ignore. Code still compiles and runs.

**First seen:** Session 2

---

### Pattern 3: Implicit Function Declaration
```
warning: implicit declaration of function 'xxx' [-Wimplicit-function-declaration]
```

**Why it happens:** Function used before being declared. Usually harmless if from a standard library.

**Is it a problem?** Only if the function doesn't exist (would fail at link time). If it compiles, it's fine.

**What to do:** Ignore if build succeeds. Add forward declaration if desired for cleanliness.

**First seen:** Session 1

---

## NVS (Non-Volatile Storage)

### Pattern 1: NVS Initialization
```
I (xxxxx) nvs: NVS partition "nvs" has not been initialized yet
I (xxxxx) nvs: NVS partition initialized
```

**Why it happens:** First boot, NVS needs to initialize the flash partition.

**Is it a problem?** NO. Normal first-boot behavior.

**What to do:** Ignore. Only happens once (on first boot).

**First seen:** Session 1

---

## GPIO & Hardware Initialization

### Pattern 1: GPIO Mode Setup
```
I (xxxxx) gpio: GPIO[XX]: mode:GPIO_MODE_OUTPUT
I (xxxxx) gpio: GPIO[YY]: mode:GPIO_MODE_INPUT
```

**Why it happens:** Application code sets GPIO pin modes (e.g., LCD reset, power amplifier enable).

**Is it a problem?** NO. Normal hardware setup.

**What to do:** Ignore.

**First seen:** Session 1

---

## How to Use This Registry

### When debugging:
1. If you see an unexpected console message, search this file
2. If it's here → **no action needed**, it's expected
3. If it's NOT here → investigate, and add it to the registry once solved

### When reporting issues to Claude:
- If a message appears in this registry, don't mention it in your bug report
- Focus Claude on novel errors/warnings not in this list
- Reference this file: "I checked the registry, this is new"

### Keeping it fresh:
- Add new safe patterns as you discover them
- Include: pattern text, why it happens, first date seen, session #
- Remove patterns if they later indicate real problems

---

## Quick Reference — All Safe Patterns at a Glance

| Category | Pattern | Safe? | Action |
|----------|---------|-------|--------|
| Touch | `i2c transaction failed` @ 0x14 | ✅ YES | Ignore |
| Touch | Blank frames during GT911 init | ✅ YES | Ignore |
| Display | `bsp_display_lock/unlock` | ✅ YES | Ignore |
| Display | LVGL adapter init messages | ✅ YES | Ignore |
| Audio | ES8311 codec init | ✅ YES | Ignore |
| Audio | Playback started/ended | ✅ YES | Ignore |
| SD Card | Files found on mount | ✅ YES | Ignore |
| SD Card | No files found | ⚠️ CHECK | Add files if needed |
| Memory | PSRAM init | ✅ YES | Ignore |
| Build | Unused variable warnings | ✅ YES | Ignore |
| Build | Signed/unsigned comparison | ✅ YES | Ignore |
| Build | Implicit declarations | ✅ YES | Ignore (if builds) |
| NVS | Partition init | ✅ YES | Ignore |
| GPIO | Mode setup | ✅ YES | Ignore |

---

**Last Updated:** May 31, 2026  
**Applicable Board:** Waveshare ESP32-P4-WIFI6-Touch-LCD-4.3  
**IDF Version:** v5.5.4  
**LVGL Version:** v9.4
