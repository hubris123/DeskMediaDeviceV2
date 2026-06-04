# Session Handoff — June 2, 2026
**Device Status:** ✅ Fully working. Audio/display/SD card stable. Display is 480×800 (not 430×272).
**Commit:** 73036fd | **Branch:** main | **Display:** 480×800 (corrected)

## Current State
- Display (ST7701S): 480×800 LVGL, stable, 500ms render delay before I2S critical (non-negotiable)
- Audio (ES8311): PCM/WAV 16kHz/16-bit stereo working via esp_codec_dev
- SD card: Auto-mounts, finds .pcm/.wav files
- Touch (GT911): I2C errors non-fatal, app continues gracefully
- Config: CONFIG_SPIRAM=y, CONFIG_BSP_ERROR_CHECK=n (locked in)
- app_main() initialization order locked—500ms LVGL wait before I2S must stay

## Efficiency System (Reduces 5-7M tokens/session)
4 PowerShell workflows (in project root):
- `preflight_and_clean.ps1` → preflight check + fullclean
- `build.ps1` → compile + auto-parse errors/warnings  
- `flash.ps1` → write firmware + auto-parse errors/warnings
- `monitor.ps1` → boot capture + filter critical messages

5 .bat utilities (in scripts/ folder):
- preflight_check, parse_logs, filter_monitor, summarize_diff, cleanup_build_artifacts

Claude checks ERRORS(Stop!Break).txt after BUILD/FLASH; blocks proceed if errors exist.

## Next Task: SquareLine UI Integration ⬅️ START HERE
1. Export LVGL UI from SquareLine at **480×800** resolution (NOT 430×272)
2. Export as C code, LVGL 9.4 compatible
3. Integrate into main.c, replace hardcoded `create_ui()` function
4. Test button callbacks work with SquareLine event handlers
5. After: Settings menu → WiFi → Video player → Weather widget

## Known Issues (Non-blocking)
- Preflight counter bug: shows "Failed: 1/8" (all actually pass; cosmetic)
- GT911 touch dead (hardware wiring, app safe)
- WAV files pop at start (RIFF header; code fix exists, low priority)
- Flash+monitor blanks display (use manual power cycle workaround)

## Build & Flash Commands
```powershell
.\preflight_and_clean.ps1    # Verify + fullclean
# Check claudetransferv2\ERRORS(Stop!Break).txt — should be empty
.\build.ps1                  # Compile + auto-parse
# Check errors — should be empty
.\flash.ps1                  # Write to device
# Manually power-cycle device (unplug/replug or RST button)
.\monitor.ps1                # Capture boot output
```

## Critical (Do Not Change)
- sdkconfig: CONFIG_SPIRAM=y, CONFIG_BSP_ERROR_CHECK=n
- main/CMakeLists.txt: only compiles main.c
- main.c: app_main() order (display reset → NVS → SD scan → display init → 500ms wait → I2S → codec)

**Before starting:** Read BOARD_TIPS_AND_QUIRKS.md, EFFICIENCY_SYSTEM.md, behaviors and rules.md

**Next step:** Export 480×800 UI from SquareLine → integrate → build → test → commit (commit 73036fd + new changes).
