# Session Summary — June 5, 2026
**For:** Next Sonnet session | **Project:** ESP32-P4 Weather Display

## Build Commands (ALWAYS use these exactly)

**Clean:**
```powershell
idf.py fullclean 2>&1 | Out-Null
```
**Build:**
```powershell
idf.py build 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\build_log.txt" -Encoding UTF8
```
**Parse:**
```powershell
.\parse_logs.ps1 claudetransferv2\build_log.txt build
```
**Flash:**
```powershell
idf.py -p COM4 flash
```
- Stop between parse and flash to check for errors
- Power cycle after flash — monitor crashes the device, don't use combined flash+monitor
- Use `parse_logs.ps1` NOT `.bat`

## What Was Accomplished This Session

1. **Network selector screen** — WiFi scan, SSID list with signal bars, lock icon, row highlight, save/exit flow
2. **Password keyboard** — tapping password field navigates to keyboardpassword screen, returns to settings on save
3. **Factory reset** — 10-second hold on mute checkbox → plays SUCCESS.WAV → erases NVS partition → restart
4. **NVS persistence** — WiFi SSID, password, zip code saved across reboots. Default zip 88002 written on first boot
5. **WiFi auto-connect** — saved credentials connect on boot via `wifi_manager.c`
6. **SNTP** — starts after WiFi connects, feeds real time to display
7. **Weather API working** — Zippopotam.us geocoding (HTTP, no SSL issues), Open-Meteo for weather data
8. **Full weather display** — temp, feels like, humidity, wind+direction (e.g. `8SE`), precipitation in inches, hourly forecast, 3-day forecast, day/night icons
9. **Timezone from API** — `timezone=auto` + `utc_offset_seconds` from Open-Meteo used for correct local time
10. **UI cleanup** — removed DAYLY STATUS labels, added PRECIPITATIONQ, numb50 font, music player panel height fixed

## Current File State

### New files added this session
- `main/wifi_manager.c` + `wifi_manager.h` — WiFi connect, events, SNTP, `wifi_manager_is_connected()`
- `main/ui/network_selector.c` + `.h` — network scan UI, row selection, save/exit

### Key settings.c additions
- Password keyboard callbacks (`password_field_cb`, `password_save_cb`, `password_exit_cb`)
- Factory reset via FreeRTOS timer on mute long-press (`mute_long_press_cb`, `factory_reset_timer_cb`)
- NVS load on boot for SSID, password, zip code
- NVS save on settings Save button

### weather_data.h additions
- `current_apparent_temp`, `current_wind_speed`, `current_wind_direction`, `current_is_day`
- `current_precip` (sum of rain+showers+snowfall in inches)
- `utc_offset_seconds` (from Open-Meteo timezone=auto)

### weather_display.h
- Struct renamed from `weather_data_t` → `weather_display_t` to avoid conflict
- Added `current_precip_in`, `current_wind_str` (e.g. "8SE")

## Known Bugs / Pinned Issues

### Time display issue (FIXED this session but watch for regression)
- SNTP `time(NULL)` returns epoch 0 until sync completes (~10s after WiFi connect)
- Fix: only update time display if `time(NULL) > 1700000000`
- Timezone: `utc_offset_seconds` from Open-Meteo API response, applied as `now + utc_offset_seconds`
- If time shows wrong: check `SNTP time:` log line for raw epoch and utc_offset value
- MDT (Las Cruces NM 88002) = UTC-6 = `-21600` seconds

### Factory reset manual power cycle needed
- `nvs_flash_erase_partition("nvs")` works but `esp_restart()` after it doesn't auto-reboot cleanly
- User must manually power cycle after factory reset — not a showstopper

### Monitor crashes device
- Never run `idf.py monitor` — it crashes the device
- Flash only: `idf.py -p COM4 flash` then power cycle

### HTTP header buffer warning
- `E (xxxxx) HTTP_HEADER: Buffer length is small to fit all the headers` appears on every weather fetch
- Harmless — data parses correctly. Buffer is 32KB which is enough for response body

## WiFi Architecture (IMPORTANT)
- Board uses ESP32-C6 coprocessor via SDIO for WiFi
- Requires `espressif/esp_wifi_remote` + `espressif/esp_hosted` in `idf_component.yml`
- `esp_hosted` runs a constructor before `app_main` — causes boot crash if main task stack too small
- Main task stack set to 8192 in `sdkconfig` — do NOT increase, do NOT go back to 32768
- Weather task stack set to 8192 for HTTPS SSL

## API Details
- **Geocoding:** `http://api.zippopotam.us/us/{zip}` — HTTP (no SSL), returns city/state/lat/lon as strings
- **Weather:** `https://api.open-meteo.com/v1/forecast` with `timezone=auto` — SSL via `esp_crt_bundle_attach`
- Weather buffer: 32KB (`WEATHER_HTTP_BUFFER_SIZE` in weather_api.h)
- Weather task stack: 8192 bytes

## UI / SquareLine Notes
- Run `fix_ui_export.py` after EVERY SquareLine export — patches GUI.h + home.c truncation
- `home_styles_backup.c` in HANDOFF_FOLDER — updated this session (763 lines → 35709 chars)
- Background image: `upload_hclbg1_52bba57ce173452fadd7595a14167a99_png.c` — must re-copy after UI swap
- `numb50.c` is a new font added this session — in `components/squareline_ui/CMakeLists.txt`
- `GUI_Panel__home__panel_3` (music player dark panel) — height set to 220 in code after merge resets to 210
- global_styles.c is ALWAYS truncated from SquareLine — fix with regex strip + append `}\n`

## Zip Code Update Bug (FIXED end of session)
- Changing zip in settings did NOT trigger weather update
- Root cause: `zipcode_save_cb` updates `s_zipcode` when keyboard save is pressed, so by the time `save_btn_cb` runs, `strcmp` sees them as equal and skips the update
- Fix: removed strcmp check — `save_btn_cb` always calls `weather_set_location(zip)` on save
- Power cycle after changing zip still works too (loads from NVS on boot)

## Next Tasks
1. **Music player** — wire up buttonup/buttonplay/buttondown callbacks (widgets exist, unwired)
2. **WiFi reconnect on settings save** — currently saves credentials but doesn't reconnect automatically; requires `esp_wifi_set_config()` + `esp_wifi_connect()` after save
3. **Lock icon** — PNG to be added by user next UI export, replace LV_SYMBOL_WARNING
4. **FPS counter** — still showing bottom-left, hide for production
5. **Weather refresh interval** — currently 15 minutes, consider making configurable in settings
6. **Searching overlay** — shows on every network selector open even if WiFi already warmed up; could check `wifi_manager_is_connected()` and skip the 3-second wait

## Last git commit
Check `git log --oneline -3`
