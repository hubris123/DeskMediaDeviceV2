# Session Summary — June 5, 2026 (Afternoon)
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
- Use PuTTY on COM4 at 115200 baud for serial logs (read-only, safe)
- Use `parse_logs.ps1` NOT `.bat`

## What Was Accomplished This Session

### Music Player (fully wired)
1. **MP3 scanner** — reads from `/sdcard/music/`, sorts alphabetically, supports LFN (long filenames via `CONFIG_FATFS_LFN_HEAP=y` in sdkconfig)
2. **Track textarea** — shows 2-digit track number from filename (e.g. `01`)
3. **Play/Stop button** — swaps icon between play and stop (`upload_stopbutton_...` / `upload_play_...`)
4. **Up/Down buttons** — previous/next track. While playing: seamlessly switches track
5. **Tick sound** on all button presses
6. **Music player panel** — slides up from bottom when music button pressed, slides back down on close
7. **Music button container** — slides right when player opens (makes room), slides back left on close
8. **Music button icon** — grey note when closed, red note when open
9. **Scrolling title bar** — slides up from bottom of screen when playing, shows `"01 - Title     01 - Title"` continuously scrolling using timer-based approach (25ms tick, 2px/step)
10. **Watchdog timer** — detects track end, resets play icon and hides scroll bar
11. **Sample rate detection** — codec reconfigured to file's actual sample rate on first decoded frame

### Weather Display Fixes
12. **Chance of Rain** — replaced "Precipitation 0.00 IN" with next-hour precipitation probability from Open-Meteo hourly data. Label renamed to "Chance Rain" in code
13. **Time clock** — moved to independent 1-second LVGL timer (no longer tied to weather fetch). Uses `g_utc_offset_seconds` from last API response
14. **Wind default** — fixed SquareLine default "12" showing on boot by overriding `details_wind_speed_lbl` in `ui_set_default_weather()`

### UI Changes
15. **Scroll panel** — dark gray (#222222), white text, 424×26px, label inset 4px left, 1px down
16. **FPS counter** — still visible bottom-left (hide later for production)
17. **Memory label** — created but hidden (`LV_OBJ_FLAG_HIDDEN`). Remove flag to show INT/PSRAM free KB

## Current File State

### New files this session
- `HANDOFF_FOLDER/MP3_CONVERSION_GUIDE.md` — FFmpeg command reference for MP3 encoding

### Key main.c changes
- `scan_sd_card()` — reads MP3s from `/sdcard/music/`, alphabetical sort
- `mp3_task()` — streams from SD card, auto-detects sample rate on first frame
- `codec_set_sample_rate()` — reconfigures codec when file sample rate differs from init
- `music_player_toggle()` — animates player panel (y-slide) and button container (x-slide)
- `music_scroll_start/stop()` — timer-based ticker, slides scroll panel in/out
- `scroll_tick_cb()` — moves label x by -2px every 25ms, wraps at -unit_w
- `clock_timer_cb()` — 1-second LVGL timer, updates CURRENTTIMEQ independently
- `mem_label_cb()` — hidden memory display (INT + PSRAM free)
- `g_utc_offset_seconds` — global updated on each weather fetch, used by clock timer

### New SquareLine widgets (merged from UI/mergeUI)
- `GUI_Panel__home__musicscrollbg` — scroll title background panel
- `GUI_Label__home__musicscrolllabel` — scrolling track title label
- `GUI_Container__home__musicbuttoncont` — music button container
- `GUI_Panel__home__musicbuttonpanel` — music button panel
- `GUI_Button__home__musicbuttongrey` — music open/close button
- `GUI_Image__home__musicbtn` — music note icon

### New assets added
- `upload_stopbutton_a37c756148194bc181010db779e72ea4_png` — stop button icon
- `upload_music_btn_ab57fd2bae02484db3c1484de8de889c_png` — grey music note
- `upload_music_btn_red_410df9ce0fe84d7ea36d6e6554682ce8_png` — red music note

## Known Issues / Notes

### MP3 FPS Drop
- FPS drops to ~5-7 during MP3 playback (from 53 FPS idle)
- Root cause: libhelix software decoder is CPU-intensive, competes with LVGL display task
- **DO NOT** attempt preloading to PSRAM — libhelix crashes when decoder state lands in PSRAM (`xmp3_UnpackSideInfo` Store access fault at PSRAM address)
- **DO NOT** change `CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL` — it was 16384 when working, changing it breaks things
- Display still updates during playback, just slower — acceptable behavior

### MP3 File Requirements
- Format: 48kHz stereo MP3 (original files work fine)
- Place in `/sdcard/music/` folder
- Name format: `01 - Title.mp3`, `02 - Title.mp3` etc.
- See `HANDOFF_FOLDER/MP3_CONVERSION_GUIDE.md` for FFmpeg command
- 22kHz re-encoded files play too fast — stick with 48kHz originals

### sdkconfig critical settings
- `CONFIG_FATFS_LFN_HEAP=y` — required for long MP3 filenames
- `CONFIG_FATFS_LFN_NONE` must NOT be set
- `CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL=16384` — do not change
- `CONFIG_SPIRAM_USE_MALLOC=y` — already set, do not change
- Main task stack: 8192 — do NOT increase or go back to 32768

### Scroll behavior
- Short song titles (shorter than scroll panel width) behave oddly — acceptable, just use normal-length names
- Scroll speed: ~80px/s (2px every 25ms)
- Scroll text: `"01 - Title     01 - Title     01 - Title     01 - Title     "` (4 repeats)

### fix_ui_export.py
- Run after EVERY SquareLine export
- `home_styles_backup.c` in HANDOFF_FOLDER is from previous session
- `global_styles.c` is ALWAYS truncated from SquareLine — the script fixes it
- New styles added this session: `QumCJvXClgKqXt`, `9NSNgaDRtTcYTS`, `Eh5lp5suTww3t9`, `341wNmAsuFcHIa`, `wCVCeNc0Hy2Pc9`, `5bE6Rj3PdUsKzR`, `EQHHuxrbsUsFwQ`

## API Details
- **Geocoding:** `http://api.zippopotam.us/us/{zip}` — HTTP, returns city/state/lat/lon
- **Weather:** `https://api.open-meteo.com/v1/forecast` with `timezone=auto`
  - Current: temp, humidity, apparent_temp, weather_code, wind_speed, wind_direction, is_day
  - Hourly: temp, weather_code, `precipitation_probability` (used for Chance Rain display)
  - Daily: weather_code, temp_max, temp_min
- Weather buffer: 32KB
- Weather task stack: 8192 bytes
- Update interval: 15 minutes

## Next Tasks
1. **Hide FPS counter** for production — find where it's enabled in BSP/LVGL config
2. **WiFi reconnect on settings save** — saves credentials but doesn't reconnect automatically
3. **Lock icon** — PNG to be added by user next UI export, replace LV_SYMBOL_WARNING
4. **Searching overlay** — shows on every network selector open even if WiFi already warmed up
5. **Weather refresh interval** — currently 15 minutes, consider making configurable

## Last known good state
- Music player: fully working with 48kHz original MP3 files
- Weather: updating correctly with Chance Rain %
- Time: updating every second via LVGL timer
- Settings: WiFi, zip code, volume all persisting via NVS
- Factory reset: 10s hold on mute checkbox works
