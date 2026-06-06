# Session Summary — June 5, 2026 (Evening/Night)
**For:** Next Sonnet session | **Project:** ESP32-P4 Weather Display

## Build Commands
```powershell
idf.py fullclean 2>&1 | Out-Null
idf.py build 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\build_log.txt" -Encoding UTF8
.\parse_logs.ps1 claudetransferv2\build_log.txt build
idf.py -p COM4 flash
```
- Power cycle after flash. Never use monitor during flash. PuTTY or `idf.py -p COM4 monitor --no-reset` for logs.

## What Was Accomplished This Session

### Video Player — WORKING
- Touch skull logo on home screen → plays MJPEG video full screen with MP3 audio
- LVGL paused during playback, resumes cleanly after
- Playlist scans /sdcard/video/, shuffles, pre-loads 2 clips into PSRAM

**Critical video player facts (hard won):**
- HW JPEG decoder writes directly into panel DMA framebuffer (`lcd_fb[0]` at `0x48007f80`, internal RAM)
- `esp_lv_adapter_dummy_draw_blit(disp, 0, 0, 480, 800, lcd_fb[idx], true)` — physical portrait coords
- DMA2D cannot read from PSRAM — always decode into internal RAM (panel framebuffers)
- PPA scrambles data when source is PSRAM — don't use PPA for this
- Color order: `JPEG_DEC_RGB_ELEMENT_ORDER_BGR` with `JPEG_DECODE_OUT_FORMAT_RGB565`

**Video file format:**
- Resolution: 480×800 portrait MJPEG
- FFmpeg: `ffmpeg -i input.mp4 -c:v mjpeg -q:v 5 -vf "transpose=1,scale=480:800" -r 20 "01 - Title.mjpeg"`
- Audio: `ffmpeg -i input.mp4 -ac 2 -ar 32000 -ab 128k -codec:a libmp3lame "01 - Title.mp3"` (must match codec fixed at 32kHz)
- Place both in `/sdcard/video/` with matching base names

### WiFi Reconnect — FIXED
- Settings save now calls `wifi_manager_connect_saved()` immediately
- `wifi_manager_connect()` now calls `esp_wifi_disconnect()` + 500ms delay before reconnecting
- SNTP double-init crash fixed with `s_sntp_started` guard flag
- After reconnect (got IP), weather auto-refreshes via `weather_set_location(zip)`

### Network Selector Crash — FIXED
- Was: `malloc`'d SSID strings leaked on every screen open → heap exhaustion crash
- Fix: static pool `s_ssid_pool[MAX_NETWORKS][33]` — no malloc, no leak
- `bsp_display_lock(0)` → `bsp_display_lock(-1)` — was dropping LVGL lock under load
- Scan task stack 4096 → 8192 bytes
- Guard flag `s_scan_running` prevents double scan on rapid re-entry

### Location Default — FIXED
- Was showing "Las Cruces, NM" on boot
- Now shows "--- --, --" until first API response

## Current File State — Key Changes This Session
- `main/video_player.c` — NEW: full video player implementation
- `main/video_player.h` — NEW: public API
- `main/main.c` — skull touch callback, g_lv_disp exposed, video_player_init call
- `main/CMakeLists.txt` — added video_player.c, esp_driver_jpeg, esp_lvgl_adapter, esp_lcd, esp_mm
- `main/ui/settings.c` — added wifi_manager_connect_saved() on save, wifi_manager.h include
- `main/ui/network_selector.c` — static SSID pool, lock fix, scan guard, stack increase
- `main/wifi_manager.c` — disconnect before connect, SNTP guard, weather refresh on IP
- `main/ui/data_binding.c` — location default changed to dashes

## Git State
- Restore point from start of session: `2455246`
- End of session commit: pushed, WiFi/video all working

## Next Tasks

### 1. Weather Loading Overlay (home screen)
Reuse exact same dark panel + message style as network selector overlay.
Show on home screen in these states:
- **"LOADING WEATHER DATA"** — while API fetch is in progress
- **"WIFI REQUIRED FOR WEATHER DATA"** — no WiFi connected
- **"WRONG PASSWORD"** — WiFi disconnect reason 15 or 202
- **"NETWORK NOT FOUND"** — WiFi disconnect reason 205
- **"WIFI CONNECTION FAILED"** — max retries reached, other reason

WiFi disconnect reason codes (from wifi_manager.c):
- Reason 8 = intentional disconnect (we triggered it)
- Reason 15 = ASSOC_LEAVE (AP rejected — wrong password)
- Reason 202 = auth fail
- Reason 205 = NO_AP_FOUND
- Reason 2 = auth expire

Overlay should hide automatically when weather data arrives and WiFi is connected.
The weather task already knows its state — hook into it.

### 2. Clock Time Display Fix
Current behavior on boot:
1. Shows "--:--" (correct — no data yet)
2. Shows wrong time (UTC) for a few seconds after SNTP syncs
3. Goes back to "--:--" briefly
4. Shows correct local time

Root cause: `clock_timer_cb` checks `if (now < 1700000000) return` to detect unsynced SNTP,
but UTC offset (`g_utc_offset_seconds`) may not be set yet when SNTP first syncs.
Fix: only show time when BOTH SNTP is synced AND `g_utc_offset_seconds` is non-zero (has been set by weather API).

## Architecture Reminders
- ESP32-P4 + ESP32-C6 coprocessor via SDIO for WiFi (2.4GHz ONLY — no 5GHz)
- Main task stack: 8192 — do NOT change
- SquareLine canvas: 800×480, rotated 90° in code → logical 480×800
- Weather: Zippopotam.us geocoding (HTTP) + Open-Meteo (HTTPS)
- `fix_ui_export.py` must run after every SquareLine export
- See `HANDOFF_FOLDER/CODE_SIDE_FIXES.md` for post-export checklist
- See `HANDOFF_FOLDER/VIDEO_PLAYER_ATTEMPTS.md` for full video player debug history
