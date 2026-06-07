# Session Summary — June 7, 2026
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

### 1. Video Player — Major Overhaul
Complete rewrite of `video_player.c` to fix multiple issues:

**Bumper intro system:**
- `/sdcard/video/bumper.mjpeg` + `bumper.mp3` — brand intro plays on every skull tap
- Bumper loaded permanently into its own 5MB PSRAM slot at boot
- Real video loads in background (`video_bg_load_task` at priority 2) while bumper plays
- If load finishes before bumper ends → seamless transition, no gap
- If bumper ends before load → brief wait logged as warning
- Bumper encoding: same as regular video, NO `adelay` on audio

**Audio/video sync fix:**
- Was: `video_mp3_play()` called before first frame rendered → audio ahead of video
- Fix: `render_first_frame()` decodes and displays frame 1, THEN audio starts
- `play_mjpeg_from()` continues from frame 2 onward

**Last-frame hang fix:**
- Display now restored to LVGL immediately when decode loop ends
- `video_player_running = false` cleared before cleanup
- Audio stop runs on `video_cleanup_task` (priority 5) in background
- `lv_async_call(trigger_fade_in)` queues fade animation safely in LVGL context

**Fade-out on return:**
- Full-screen black overlay (`g_video_fade_overlay`) on home screen, hidden normally
- After video ends: overlay shown at full opacity, fades to transparent over 400ms
- Overlay is 560×560 centered to cover coordinate system offset quirks
- Fade-in at start was attempted but caused issues — removed, fade-out only

**Random seed fix:**
- Was: `srand(time(NULL))` — same sequence every boot (SNTP not synced)
- Fix: `srand(esp_random())` — hardware RNG, true entropy every boot

**Touch responsiveness:**
- Removed background preload (was blocking SD bus, causing 7-9s touch dead zone)
- Load-on-demand: each video loads when needed, bumper covers the wait

**Video load optimized:**
- clip1.mjpeg was 11.7MB → truncated to 10MB buffer → partial video
- Re-encode at `-q:v 8` to get under 8MB so it loads within bumper duration

### 2. Button Press Visual Feedback
- Removed all `audio_play_tick()` calls from music player buttons
- Added `LV_STATE_PRESSED` red overlay (0xFF2200, 40% opacity, radius 8) to all 4 music buttons
- LVGL handles press/release automatically — zero callback overhead

### 3. Tick Sound Fixed
- Was generating 320 samples at 16kHz math but codec runs at 32kHz → wrong pitch
- Fixed: 640 samples at `AUDIO_SAMPLE_RATE` (32000Hz) = correct 20ms 880Hz burst
- Tick skips silently if `mp3_is_playing` (codec busy, would block/corrupt)

### 4. libhelix Decoder Pinned to Internal RAM
- All 8 `AllocateBuffers()` mallocs → `heap_caps_malloc(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT)`
- Prevents crashes + reduces cache-miss thrashing
- File: `managed_components/chmorgan__esp-libhelix-mp3/libhelix-mp3/real/buffers.c`

### 5. System Ready Gate
- `g_system_ready` flag blocks skull touch and all music buttons until system is stable
- Waits for: first weather fetch complete + not fetching + 2s settle (second fetch runs)
- Fallback: WiFi error → enable anyway; 30s absolute timeout
- Prevents crashes from touching codec/video before initialization complete
- Log: "System ready — touch enabled" fires ~16s after boot on good WiFi

### 6. Video Audio Format
- All video MP3s must be 32kHz to match fixed codec rate
- Music player MP3s: use `adelay=500|500` for leading silence (PA pop suppression)
- Video audio: NO adelay — codec already warm

## Key Files Changed This Session
- `main/video_player.c` — complete rewrite (bumper, concurrent load, fade, sync fix)
- `main/video_player.h` — added `g_video_fade_overlay` extern
- `main/main.c` — system ready gate, fade overlay creation, button feedback, tick fix, helix RAM fix
- `managed_components/chmorgan__esp-libhelix-mp3/libhelix-mp3/real/buffers.c` — MALLOC_CAP_INTERNAL
- `HANDOFF_FOLDER/MP3_CONVERSION_GUIDE.md` — split music/video sections, adelay documented

## Git Checkpoints
- `2a4cef9` — Pop fix, button feedback, helix RAM, tick fix, video 32kHz
- `9cc4cb9` — Video player overhaul, bumper, fade, sync, system ready gate

## Video/Bumper File Format

### Bumper (/sdcard/video/bumper.mjpeg + bumper.mp3)
```
ffmpeg -i "intro.mp4" -c:v mjpeg -q:v 5 -vf "transpose=1,scale=480:800" -r 20 "bumper.mjpeg"
ffmpeg -i "intro.mp4" -ac 2 -ar 32000 -ab 128k -codec:a libmp3lame "bumper.mp3"
```
- Keep under 5MB
- Must be longer than longest video load time (~4-5s for 8MB clips)
- No adelay on audio

### Regular Videos (/sdcard/video/)
```
ffmpeg -i input.mp4 -c:v mjpeg -q:v 5 -vf "transpose=1,scale=480:800" -r 20 "01 - Title.mjpeg"
ffmpeg -i input.mp4 -ac 2 -ar 32000 -ab 128k -codec:a libmp3lame "01 - Title.mp3"
```
- Keep under 8MB per clip (loads within bumper duration)
- Use `-q:v 8` or higher for larger/longer clips to reduce file size
- bumper.mjpeg is automatically excluded from the playlist

## Architecture Reminders
- ESP32-P4 + ESP32-C6 coprocessor via SDIO for WiFi (2.4GHz ONLY)
- LVGL task runs at priority 6 — DO NOT pin to a core, breaks touch
- Main task stack: 8192 — do NOT change
- SquareLine canvas: 800×480, rotated 90° → logical 480×800
- Codec fixed at 32kHz — ALL MP3s must be encoded at 32000Hz
- PA gate (GPIO 53) controlled directly in `mp3_task` for pop suppression
- `fix_ui_export.py` must run after every SquareLine export
- See `HANDOFF_FOLDER/CODE_SIDE_FIXES.md` for post-export checklist
- `g_system_ready` must be true before any audio/video touch is processed

## Next Session Tasks

### 1. Windows SD Card Content Converter App
- GUI app: drag and drop source files → correctly formatted SD card files
- Handles: music MP3 (32kHz + adelay), video MJPEG + audio (32kHz, no adelay), naming convention
- Bundles FFmpeg internally
- Optional file obfuscation (XOR/AES) — requires matching firmware decrypt
- Built as standalone .exe (Python + PyInstaller or C# WinForms)

### 2. GitHub Hosting Setup (prerequisite for OTA)
- Create public repo with GitHub Pages for manifest JSON
- Stable URL: `https://username.github.io/repo/manifest.json`
- GitHub Releases for firmware .bin and content files
- Verify HTTPS reachable from device before writing OTA code

### 3. OTA Firmware Update System
- Update `partitions.csv`: add `otadata` + `ota_0(6MB)` + `ota_1(6MB)`, remove unused SPIFFS
- Add `APP_VERSION` string to firmware
- Manifest JSON check on boot → compare versions → `esp_https_ota()` if newer
- **WARNING:** first flash after partition change wipes NVS (re-enter WiFi + zip code)

### 4. SD Card Content Sync
- Single "Check for Updates" button triggers both firmware + content check
- Manifest lists files with SHA256 checksums + URLs
- Device downloads only changed files (~50MB total, viable over WiFi)
- Hosting: GitHub Pages for manifest, GitHub Releases for content files
- Future: can switch hosting provider by updating manifest URL only
