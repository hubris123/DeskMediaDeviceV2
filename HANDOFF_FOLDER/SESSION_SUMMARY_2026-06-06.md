# Session Summary — June 6, 2026
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

### 1. Weather Status Overlay — BUILT & STYLED
- Dark panel on home screen shows WiFi/weather status messages
- Position: `LV_ALIGN_CENTER` + offset `(81, -221)` — matches SquareLine exactly
- Size: 430×40px, radius 5, font Montserrat 16, centered text
- Slides down from parked position (-266) to visible (-221) on show, reverses on hide
- Slide-in: `STATUS_BAR_SLIDE_IN_MS` (500ms), Slide-out: `STATUS_BAR_SLIDE_OUT_MS` (300ms)
- Colors by state:
  - Dark gray `0x222222` + white text → CONNECTING TO WIFI... / LOADING WEATHER DATA
  - Orange `0xCC6600` + black text → WIFI REQUIRED FOR WEATHER DATA
  - Red `0xCC2222` + black text → WRONG PASSWORD / NETWORK NOT FOUND / WIFI CONNECTION FAILED
- Only calls LVGL when message state actually changes (static `s_last_msg` guard)
- All home-screen timers gated with `lv_scr_act() == GUI_Screen__home`

### 2. WiFi Manager — Connecting State Added
- `s_connecting` flag — set true in `wifi_manager_connect()`, cleared on got-IP or max retries
- `s_last_disconnect_reason` reset to 0 on every new connect attempt (clears stale errors)
- New public APIs: `wifi_manager_is_connecting()`, `wifi_manager_get_last_disconnect_reason()`
- Overlay message sequence now: WIFI REQUIRED → CONNECTING TO WIFI... → LOADING WEATHER DATA → hidden

### 3. Weather Task — Fetch Status API
- `s_fetching` volatile bool — set true before `weather_fetch_current()`, false after
- New public API: `weather_task_is_fetching()`
- `weather_task` priority lowered from 3 → 2 (background, yields to everything)

### 4. Clock Fix — No More UTC Flash
- `clock_timer_cb` now gates on `g_utc_offset_seconds != 0` in addition to SNTP sync check
- Clock stays at `--:--` until weather API sets the UTC offset — no brief UTC time flash
- `weather_display_update()` no longer writes to the clock label (removed line 65 in weather_display.c)
- Clock has single owner: `clock_timer_cb` in main.c

### 5. MP3 CPU Optimizations
- Scroll timer: 25ms → 67ms (15fps), step 2px → 3px — 62% fewer LVGL invalidations
- All home-screen timers gated: `clock_timer_cb`, `music_watchdog_cb`, `mem_label_cb`, `weather_overlay_cb`
- `mp3_task` priority: currently at 5 (below LVGL at 6) with `taskYIELD()` after each frame
- `weather_task` priority: 3 → 2

### 6. MP3 Format — Fixed at 32kHz
- `AUDIO_SAMPLE_RATE` changed from 44100 → 32000
- Codec fixed at 32kHz — `codec_set_sample_rate()` and `sample_rate_set` logic removed entirely
- All MP3s must be re-encoded at 32000Hz 128kbps stereo (see MP3_CONVERSION_GUIDE.md)
- FFmpeg: `ffmpeg -i "input.mp3" -ac 2 -ar 32000 -ab 128k -codec:a libmp3lame "output.mp3"`

### 7. Tuning Knobs — All in One Place (top of main.c)
```c
#define TICK_TASK_PRIORITY      4    // button click sound
#define MP3_TASK_PRIORITY       5    // MP3 decoder — below LVGL (6)
#define SCROLL_TIMER_MS         67   // 67=15fps, 50=20fps, 40=25fps
#define SCROLL_STEP_PX          3    // pixels per scroll tick
#define STATUS_BAR_SLIDE_IN_MS  500  // overlay slide down
#define STATUS_BAR_SLIDE_OUT_MS 300  // overlay slide up
// sdkconfig: CONFIG_LV_DEF_REFR_PERIOD=15 — DO NOT raise, hurts touch responsiveness
```

## Key Files Changed This Session
- `main/main.c` — overlay, clock fix, MP3 priority/rate, tuning knobs, core pinning reverted
- `main/wifi_manager.h` — added `wifi_manager_is_connecting()`, `wifi_manager_get_last_disconnect_reason()`
- `main/wifi_manager.c` — `s_connecting`, `s_last_disconnect_reason` reset on connect
- `main/weather/weather_task.h` — added `weather_task_is_fetching()`
- `main/weather/weather_task.c` — `s_fetching` flag, priority 2
- `main/weather_display.c` — removed clock label write (clock owned by clock_timer_cb)
- `HANDOFF_FOLDER/MP3_CONVERSION_GUIDE.md` — updated to 32000Hz 128kbps

## Architecture Reminders
- ESP32-P4 + ESP32-C6 coprocessor via SDIO for WiFi (2.4GHz ONLY)
- LVGL task runs at priority 6 with no core affinity — DO NOT pin to a core, breaks touch
- Main task stack: 8192 — do NOT change
- SquareLine canvas: 800×480, rotated 90° in code → logical 480×800
- SquareLine uses CENTER-ORIGIN coordinates — convert with `lv_obj_set_align(LV_ALIGN_CENTER)` + `lv_obj_set_pos(x, y)`
- Weather: Zippopotam.us geocoding (HTTP) + Open-Meteo (HTTPS)
- `fix_ui_export.py` must run after every SquareLine export
- See `HANDOFF_FOLDER/CODE_SIDE_FIXES.md` for post-export checklist

## Next Tasks

### 1. MP3 Start Pop/Click (PRIORITY)
- Harsh transient at beginning of every track
- Codec opened once at boot — PA stays on permanently — pop is NOT from PA enable on open
- Pop occurs on every track start: DMA transitioning from previous track's tail to new track's first frame
- Tried: silence flush (4 iterations) — didn't help
- Tried: silence flush (16 iterations) — didn't help, added touch delay
- Tried: `esp_codec_dev_set_out_mute()` → silence → unmute — didn't help (digital, doesn't reach analog in time)
- Tried: set volume to 0 → silence → restore — didn't help (same reason)
- Trying: direct `gpio_set_level(BSP_POWER_AMP_IO, 0)` at task start → 2× silence frames → decode first frame → `gpio_set_level(1)` before first write
  - Bypasses codec stack entirely — only untried path remaining
  - Also applies to video player MP3s (both go through mp3_task via video_mp3_play)
- If PA gate still pops: move gpio_set_level(1) to AFTER first esp_codec_dev_write (frame in FIFO first, then PA on)

### 2. libhelix MP3 Pre-buffering in PSRAM (Task #7)
- Software decoder causes ~5fps drop and touch delay during playback
- libhelix crashes when decoder STATE lands in PSRAM
- Plan: pin decoder state to internal RAM with `heap_caps_malloc(MALLOC_CAP_INTERNAL)`
  while buffering decoded PCM output in PSRAM ahead of playback
- This is the real fix for smooth playback and touch responsiveness

### 3. Touch Delay During MP3 Playback
- Slight but noticeable delay on music player buttons during playback
- Currently at hardware/software decoder ceiling
- Will improve significantly once pre-buffering is solved (task 2 above)
- Core pinning was tried and reverted — broke touch intermittently (WiFi driver conflict on core 1)

## Reverting 32kHz If It Causes Problems
If 32kHz causes issues (wrong speed on certain files, audio quality complaints):
1. Change `#define AUDIO_SAMPLE_RATE 32000` back to `44100` in main.c
2. Restore `codec_set_sample_rate()` function and `sample_rate_set` logic in `mp3_task`
3. Re-encode files back to 48kHz or leave at 44.1kHz originals
4. The per-file rate detection was working before this session — it just caused
   the first track to play slow (44.1kHz codec clock, 32kHz data) until reconfigured
   on the first decoded frame. That is fixable by pre-opening the codec at the file's
   sample rate before starting the decode loop, but was not attempted this session.

## What Was Tried and Reverted
- `CONFIG_LV_DEF_REFR_PERIOD=33` — made touch WORSE (LVGL polls input less frequently)
- `mp3_task` priority 7 — good audio but killed touch responsiveness
- `vTaskDelay(1ms)` in decode loop — made delay worse at 32kHz (more frames/sec = more total delay)
- Core pinning (LVGL→core1, mp3→core0) — broke touch intermittently, reverted
