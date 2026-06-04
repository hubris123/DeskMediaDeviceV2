# Session Summary — June 4, 2026
**For:** Next Sonnet session | **Project:** ESP32-P4 Weather Display

## Current State
- Build passing, UI rendering correctly with background image
- SquareLine Vision UI exported and working on device
- Background image (HCLbg1.png) set via C code as RGB565
- HR5 and CHANCEOFRAINQ removed from current UI (only 4 hourly slots)
- WiFi NOT yet wired up — all weather data is placeholder

## What Was Accomplished This Session
1. Fixed all build errors (GUI.h truncation, home.c truncation, nvs_storage, linker issues)
2. Got hardware rotation working (ROTATE_90 + DOUBLE_FULL tear avoid mode)
3. Resolved watchdog crash (increased WDT to 30s, main stack to 32KB)
4. Fixed SquareLine Vision file truncation workflow
5. Background image (CrosserCorps logo) rendering correctly behind weather widget

## Key Files & State

### SquareLine UI Workflow (CRITICAL)
Every time UI files are swapped in, run manually:
```python
python fix_ui_export.py
```
OR do it manually:
- `GUI.h` — always truncated, missing `#endif` and some font/image declares
- `home.c` — always truncated mid-`GUI_initScreenStyles__home()`
- Backup styles: `HANDOFF_FOLDER/home_styles_backup.c` (580 lines, complete)
- The script auto-patches both files and restores styles from backup

### Known Widget Changes vs Old Code
- HR5 removed from UI → `weather_display.c` and `data_binding.c` updated to loop i<4
- CHANCEOFRAINQ removed from UI → commented out in both files
- If UI adds them back, restore the commented lines and change loop to i<5

### Background Image
- Source: `components/squareline_ui/HCLbg1.png` (800x480)
- Generated C file: `UIFiles/assets/images/upload_hclbg1_52bba57ce173452fadd7595a14167a99_png.c` (RGB565, 750KB)
- In CMakeLists: yes
- In GUI.h: yes
- Set in main.c after `GUI_init()`: `lv_obj_set_style_bg_image_src(GUI_Screen__home, &upload_hclbg1_..._png, 0);`
- When new UI files are swapped in, this C file gets deleted — must re-copy from outputs or regenerate

### main.c Config
- Rotation: `ESP_LV_ADAPTER_ROTATE_90`
- Tear avoid: `ESP_LV_ADAPTER_TEAR_AVOID_MODE_DOUBLE_FULL`
- Touch: `swap_xy = 1`
- WDT timeout: 30s
- Main stack: 32KB

### CMakeLists fonts currently
- display_1, fixed23, font, font_15, header_1, label_1, number_1, subtitle_1, tiny, title_1

## Next Tasks
1. **WiFi** — ESP32-P4 uses ESP32-C6 coprocessor via SDIO for WiFi
   - Needs `espressif/esp_wifi_remote` and `espressif/esp_hosted`
   - `weather_is_wifi_ready()` currently stubbed to return true
2. **Weather API** — wire up real Open-Meteo API calls once WiFi works
3. **NVS** — save/load location (zip code) and cached weather data
4. **Settings screen** — zip code entry keyboard is already in SquareLine UI
5. **Time** — get real time via SNTP once WiFi connected

## Tool Notes
- Use `parse_logs.ps1` NOT `parse_logs.bat`
- Use `idf.py -p COM4 flash monitor` combined — never separate (display goes blank)
- Background image C file must be manually copied when UI files are swapped (sandbox can't write to that folder directly)
- `fix_ui_export.py` in project root handles GUI.h + home.c fixes automatically

## Last git commit
Check `git log --oneline -3`
