# Next Session Prep — June 2, 2026
Quick reference to accelerate SquareLine UI integration and device development.

---

## SquareLine Integration Checklist

**Step 1: Export from SquareLine Studio**
- Resolution: 480×800 (NOT 430×272)
- Color format: RGB565 16-bit
- LVGL version: 9.4 compatible
- Export as: C code (ui/ folder with ui.c, ui.h, etc.)

**Step 2: Integrate into main.c**
- Copy exported ui/ folder to main/
- In main.c: `#include "ui/ui.h"` (add to includes)
- In `create_ui()` function: replace entire function body with:
  ```c
  void create_ui(void) {
      ui_init();  // Call SquareLine-generated init
  }
  ```
- Ensure button callbacks from SquareLine map to existing functions:
  - `play_pcm_callback()` for Play PCM button
  - `play_wav_callback()` for Play WAV button
  - `stop_callback()` for Stop button
- If SquareLine generates different callback names, create wrapper functions or modify callbacks in ui.c

**Step 3: Build & Test**
```powershell
.\preflight_and_clean.ps1    # Verify + fullclean
.\build.ps1                  # Should pass with no errors
.\flash.ps1                  # Flash to device
# Power-cycle device
.\monitor.ps1                # Verify UI displays correctly
```

**Step 4: Verify Display**
- UI should display at full 480×800 resolution
- No scaling artifacts
- Buttons should be clickable
- Audio playback should work (test with Play PCM, Play WAV, Stop)

**Step 5: Commit**
```powershell
git add main/ui/ main/main.c
git commit -m "Integrate SquareLine UI - 480x800 display with audio playback callbacks"
git push
```

---

## Display Config Reference

**Location:** `C:\Users\Administrator\Documents\DeskMediaDeviceV2\components\esp32_p4_wifi6_touch_lcd_4_3\include\bsp\display.h`

**Critical defines:**
```c
#define BSP_LCD_H_RES              (480)   // Horizontal resolution
#define BSP_LCD_V_RES              (800)   // Vertical resolution
#define BSP_LCD_DRAW_BUFF_SIZE     (BSP_LCD_H_RES * 50)  // 24KB framebuffer
```

**Do NOT change these.** If display resolution needs adjustment in future, it's here.

---

## Verified Test Cycle (June 2, 2026)

Last successful build + flash + boot:
- ✅ Preflight check: All 8 checks passed (cosmetic counter bug aside)
- ✅ Fullclean: Build directory cleaned, managed components removed
- ✅ Build: lvgl_demo_v9.bin generated (899KB), 89% partition space free
- ✅ Flash: Bootloader + partition table + firmware written successfully
- ✅ Boot: Device boots on every power cycle, display stable, audio works

No blocking errors. Device is stable baseline for UI integration.

---

## Audio Pipeline (Reference Diagram)

```
┌─────────────────────────────────────────────────────────────┐
│ SD Card Audio Playback Pipeline                              │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  1. SD Card (CANON.PCM or CANON.WAV)                        │
│          ↓                                                   │
│  2. audio_task() → fread(512-byte chunks)                   │
│          ↓                                                   │
│  3. esp_codec_dev_write() ← BSP audio codec API             │
│          ↓                                                   │
│  4. I2S TX (GPIO 9/10/12/13) via esp_codec_dev             │
│          ↓                                                   │
│  5. ES8311 Codec (I2C 0x18, GPIO 7/8)                       │
│          ↓                                                   │
│  6. NS4150B Power Amp (GPIO 53 enable)                      │
│          ↓                                                   │
│  7. Speaker Output (3.5mm jack)                             │
│                                                              │
└─────────────────────────────────────────────────────────────┘

Key settings:
- Sample rate: 16,000 Hz
- Bit depth: 16-bit
- Channels: Stereo
- Volume: 80% (esp_codec_dev_set_out_vol)
- Buffer: 512 bytes per write
```

---

## I2S & Display Timing (CRITICAL)

**app_main() initialization order (DO NOT REARRANGE):**
1. Hardware reset ST7701S via GPIO 27 (20ms LOW, 120ms HIGH)
2. NVS flash init
3. SD card scan
4. `bsp_display_start_with_config()` with TEAR_AVOID_MODE_NONE
5. `bsp_display_backlight_on()`
6. `bsp_display_lock(-1)` — lock display
7. `create_ui()` — build LVGL tree
8. `lv_obj_invalidate(lv_scr_act())` — mark for redraw
9. `bsp_display_unlock()` — **LVGL renders first frame here**
10. **`vTaskDelay(500ms)` — WAIT for first frame to fully flush to display**
11. `i2s_init()` — I2S DMA starts AFTER display stable
12. `codec_init()` — ES8311 init (outside lock, I2C OK now)

**Why 500ms delay?** I2S DMA startup was interfering with LVGL's first display flush, causing blank screen. Delay ensures display is stable before I2S fires up.

**Never remove this delay.** Many reboots were spent debugging this one timing issue.

---

## Key Gotchas for Next Tasks

**WiFi phase:**
- Enable CONFIG_ESP_WIFI_ENABLED in sdkconfig
- WiFi init must happen AFTER audio codec (to avoid I2C conflicts)
- Test with existing button callbacks first before adding network logic

**Video player phase:**
- ST7701S supports hardware H.264 decode via ESP32-P4 media engine
- Resolution locked at 480×800 — video must scale to fit
- Audio sync critical if playing video with audio

**Settings menu phase:**
- NVS (Non-Volatile Storage) already initialized in app_main()
- Use existing NVS handle for config persistence
- Touch input (GT911) is non-functional — use buttons for menu navigation until fixed

**Weather widget phase:**
- Requires WiFi (task 2 above)
- Display update cycle must not block audio playback (use FreeRTOS task)

---

## Quick Reference: File Locations

| What | Where |
|------|-------|
| Main app code | main/main.c |
| Display config | components/esp32_p4_wifi6_touch_lcd_4_3/include/bsp/display.h |
| Audio codec setup | main/main.c (codec_init function) |
| Build scripts | project root (preflight_and_clean.ps1, build.ps1, flash.ps1, monitor.ps1) |
| Utility tools | scripts/ (preflight_check.bat, parse_logs.bat, filter_monitor.bat) |
| Efficiency docs | HANDOFF_FOLDER/ (EFFICIENCY_SYSTEM.md, behaviors and rules.md) |
| Board quirks | HANDOFF_FOLDER/BOARD_TIPS_AND_QUIRKS.md |
| Known patterns | CONSOLE_PATTERNS_REGISTRY.md |

---

**Session date created:** June 2, 2026  
**Purpose:** Accelerate SquareLine UI integration and prep for subsequent features (WiFi, video, weather)  
**Status:** Ready for UI export and integration
