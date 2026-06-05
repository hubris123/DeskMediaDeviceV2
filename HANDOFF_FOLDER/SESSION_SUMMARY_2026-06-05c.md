# Session Summary — June 5, 2026 (Evening)
**For:** Next Sonnet session | **Project:** ESP32-P4 Weather Display

## Build Commands
```powershell
idf.py fullclean 2>&1 | Out-Null
idf.py build 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\build_log.txt" -Encoding UTF8
.\parse_logs.ps1 claudetransferv2\build_log.txt build
idf.py -p COM4 flash
```
- Power cycle after flash. Never use monitor. PuTTY on COM4 115200 for logs.

## What's Working
- Full weather display: temp, feels like, humidity, wind+direction, chance of rain %, hourly/daily forecast, icons, location, time
- Time updates every second via LVGL timer (independent of weather fetch, uses utc_offset from API)
- Settings screen: WiFi SSID/password, zip code — all persist via NVS
- Factory reset: 10s hold on mute checkbox → SUCCESS.WAV → NVS erase → restart
- WiFi auto-connect on boot, SNTP after connect
- Music player: slides up/down from bottom, music button slides right/left
- MP3 playback from `/sdcard/music/` — track number in textarea, title scrolls at bottom
- Play/Stop icon swaps, tick sound on all buttons, up/down changes track (seamless while playing)
- Chance of Rain label shows next-hour precipitation probability from Open-Meteo hourly data

## MP3 Notes — IMPORTANT
- Use original 48kHz stereo MP3 files — they work perfectly
- 22kHz re-encoded files play too fast (codec mismatch issue, not worth fixing)
- `AUDIO_SAMPLE_RATE` in main.c is set to 22050 — change back to 44100 if using 48kHz files
- FPS drops to ~5 during playback — hardware limitation of software MP3 decoder, acceptable
- DO NOT attempt PSRAM preloading — libhelix crashes when its state lands in PSRAM
- DO NOT change `CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL` from 16384
- MP3 files go in `/sdcard/music/`, named `01 - Title.mp3`, `02 - Title.mp3` etc.

## Key Files Changed This Session
- `main/main.c` — music player, scroll bar, clock timer, weather display loop
- `main/ui/data_binding.c` — wind default "--", precip default "--%"
- `main/weather_display.c` — shows precip prob % instead of inches
- `main/weather/weather_api.c` — added precipitation_probability to hourly request
- `main/weather_display.h` — `current_precip_in` replaced with `current_precip_prob`
- `sdkconfig` — `CONFIG_FATFS_LFN_HEAP=y` for long MP3 filenames
- `HANDOFF_FOLDER/CODE_SIDE_FIXES.md` — all post-SL code fixes documented
- `HANDOFF_FOLDER/MP3_CONVERSION_GUIDE.md` — FFmpeg conversion reference

## Code-Side Fixes (applied every export)
See `HANDOFF_FOLDER/CODE_SIDE_FIXES.md` for full list. Key ones:
- `panel_3` height forced to 220 (SL resets to 210)
- Music player/button containers parked off-screen on boot
- Scroll panel styled dark gray, white text, parked off bottom
- "Precipitation" label renamed to "Chance Rain" in code
- Wind default overridden from SL's "12" to "--"

## Next Tasks
1. **Hide FPS counter** for production
2. **WiFi reconnect on settings save** — credentials save but don't reconnect automatically
3. **Lock icon** — replace LV_SYMBOL_WARNING with PNG on next UI export
4. **Network selector searching overlay** — shows even when WiFi already connected
5. **Weather refresh interval** — consider making configurable in settings

## Architecture Reminders
- ESP32-P4 + ESP32-C6 coprocessor via SDIO for WiFi
- Main task stack: 8192 — do NOT change
- `esp_hosted` runs constructor before app_main — stack size critical
- SquareLine canvas: 800×480, rotated 90° in code → logical 480×800
- Weather: Zippopotam.us geocoding (HTTP) + Open-Meteo (HTTPS)
- `fix_ui_export.py` must run after every SquareLine export
