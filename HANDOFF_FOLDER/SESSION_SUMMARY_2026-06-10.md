# Session Summary — June 10, 2026
**For:** Next session (first Claude Code session) | **Project:** ESP32-P4 Desk Media Device

## Build Commands
```powershell
idf.py fullclean 2>&1 | Out-Null
idf.py build 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\build_log.txt" -Encoding UTF8
.\parse_logs.ps1 claudetransferv2\build_log.txt build
idf.py -p COM4 flash
idf.py -p COM4 monitor --no-reset
```
- Power cycle after flash. Never use monitor during flash (under investigation —
  see MONITOR_CRASH_INVESTIGATION.md, run those experiments FIRST next session).

## What Was Accomplished This Session

### 1. Code review + bug fixes (commits e7b1843, 177e22d)
- Removed a literal NUL byte embedded in main.c (made git/grep treat it as binary)
- `weather_geocode_zipcode` now checks HTTP err/status and null-terminates
  before cJSON parse (was parsing uninitialized heap on failed requests)
- Removed stale `Host: nominatim.openstreetmap.org` header sent to Open-Meteo
  and the TLS common-name-check bypass
- Hourly forecast times and daily day-names now use `utc_offset_seconds`
  from the API instead of hardcoded UTC-7 / device-UTC localtime

### 2. Switched to NCEP HRRR model (commit d1d34a3)
- `&models=gfs_hrrr` — user-tested as more accurate than default/seamless
  for US locations (seamless was 3–8°F off)

### 3. Hourly fetch + 15-minute step-through (commits a04adce, + fix)
- Fetches once per HOUR (was 15 min) → ~24 API calls/day instead of ~96
- Request includes `minutely_15` (7 vars) × `forecast_minutely_15=8` (2h cushion)
- `weather_get_data()` overlays the 15-min step covering "now" onto current
  conditions; step index folded into `last_update` so main.c's change-detect
  refreshes UI each 15-min boundary (zero main.c changes)
- New struct `minutely_step_t` + `minutely[8]`/`minutely_count` in weather_data_t

### 4. Post-flash bug fixes from device testing
- **"HTTP_CLIENT: Out of buffer"**: the long minutely_15 URL (~620 bytes)
  exceeded esp_http_client's default 512-byte TX buffer → `.buffer_size_tx = 1024`
- **Temp ~10°F off**: overlay was applying STALE steps (latest ≤ now, however
  old). Now a step must cover now within 16 min AND be newer than the API's
  current-conditions timestamp, else fall back to the plain current block
- **Daily forecast showed today first**: API daily[0] is always today; parser
  now skips it and shows the NEXT 3 days (Thu/Fri/Sat when today is Wed).
  Requires `forecast_days=4` (restored from 3 — the display needs day indices
  1–3; cost difference is a fraction of one API credit)

## NOT yet verified on device
- The stale-step fix and daily-skip fix were committed but NOT yet flashed.
  Build + flash + verify: current temp sane, daily rows show Thu/Fri/Sat,
  monitor log shows "Parsed 8 15-minutely steps".

## Known Issues / Next Steps
1. **Run MONITOR_CRASH_INVESTIGATION.md experiments** (top priority per user)
2. Verify HRRR current temp accuracy after the stale-step fix — if still off,
   compare the `Weather parsed: X°F` log line against the displayed value to
   distinguish API data vs overlay
3. Deferred improvements from the June 10 review (user-approved list pending):
   precomputed tick sound (640 sinf per press), larger SD read buffers
   (MP3 2KB / WAV 512B), NVS weather cache as single blob, network selector
   blocks LVGL task up to 5s on screen load, dead code (es8311_minimal.*,
   most of data_binding.c, unused callbacks in main.c)
4. Mute checkbox in settings doesn't actually mute (colors/logs only) — ask user
5. Boot does TWO geocode+fetch cycles (app_main + got-IP handler both send zip)
   — dedupe would remove the race the ready-gate waits on

## Git Checkpoints (this session, oldest first)
- `e7b1843` NUL byte fix in main.c
- `177e22d` weather_api: geocode validation, Host header, timezone fixes
- `d1d34a3` HRRR model switch
- `a04adce` hourly fetch + 15-min step-through
- (next) TX buffer 1024 fix
- (next) stale-step overlay fix + daily skip-today fix

## Current Device State (as of last flash mid-session)
- HRRR weather fetching working after TX buffer fix
- Music, video player, settings, WiFi — all working as before
- Daily forecast + stale-step fixes committed but awaiting flash
