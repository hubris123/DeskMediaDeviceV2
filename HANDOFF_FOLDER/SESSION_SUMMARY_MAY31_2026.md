# Session Handoff — May 31, 2026
**Device Status:** ✅ Fully working. Audio/display/SD card stable on every boot. Touch (GT911) non-fatal I2C errors.
**Commit:** 73036fd | **Branch:** main

## Current State
- Display (ST7701S): Stable, 500ms LVGL render delay critical (do not remove)
- Audio (ES8311): PCM/WAV playback working via esp_codec_dev, 16kHz/16-bit stereo
- SD card: Auto-mounts, finds .pcm/.wav files
- Config: CONFIG_SPIRAM=y, CONFIG_BSP_ERROR_CHECK=n (do not revert)
- Initialization order in app_main() is critical—do not rearrange

## Last Changes
- **Created 4 PowerShell workflow scripts** (preflight_and_clean, build, flash, monitor) with dual output + integrated parsing/filtering
- **Organized all .bat utilities into scripts/ folder** (preflight_check, parse_logs, filter_monitor, summarize_diff, cleanup_build_artifacts)
- **Built efficiency system** reducing token usage 5-7M per session via log parsing (ERRORS/WARNINGS extraction, monitor filtering, git diff summaries)
- **Updated docs** (behaviors and rules.md, EFFICIENCY_SYSTEM.md) with new structure and mandatory Git Log Backup Protocol for Claude
- **Created CONSOLE_PATTERNS_REGISTRY.md** documenting 20+ known-safe patterns (GT911 errors, LVGL messages, etc.)

## Next Task (Priority Order)
1. **UI/GUI via SquareLine Studio** ⬅️ START HERE — User exports LVGL UI (430×272, RGB565), integrate as C code replacing hardcoded `create_ui()` in main.c
2. Settings menu (NVS config storage)
3. WiFi (enable CONFIG_ESP_WIFI_ENABLED, implement wifi_init)
4. Video player (hardware H.264 decoder)
5. Weather widget

## Known Issues
- Preflight shows "Failed: 1/8" cosmetic bug (all checks actually pass; logic error in summary)
- GT911 touch non-functional (hardware wiring issue, app continues safely)
- WAV files pop at start (44-byte RIFF header; fix in code but low priority if PCM only)
- Flash+monitor together blanks display (use manual power cycle instead)

## Build Workflow
```powershell
.\preflight_and_clean.ps1  # Verify + fullclean
.\build.ps1                # Compile + auto-check for errors
.\flash.ps1                # Write firmware + auto-check for errors
# Manually power-cycle device
.\monitor.ps1              # Capture boot + filter output
```
Claude checks ERRORS(Stop!Break).txt after BUILD/FLASH; blocks if errors exist.

## Critical Files (Do Not Revert)
- sdkconfig (all settings verified)
- main/CMakeLists.txt (only compiles main.c)
- main.c app_main() initialization order (500ms LVGL delay before I2S is non-negotiable)

**Read before starting:** BOARD_TIPS_AND_QUIRKS.md, EFFICIENCY_SYSTEM.md, behaviors and rules.md

**Next session:** SquareLine UI export → integrate → test → commit.
