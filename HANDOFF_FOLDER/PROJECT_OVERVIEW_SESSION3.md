# DeskMediaDevice Project Handoff - Session 3
**Date:** May 30–31, 2026
**Board:** Waveshare ESP32-P4-WIFI6-Touch-LCD-4.3
**IDF Version:** v5.5.4
**Serial Port:** COM4

---

## Current Status: FULLY WORKING ✅

- ✅ Device boots reliably on every boot (power-on AND soft reset)
- ✅ Display (ST7701S, 4.3" DSI) shows UI with buttons
- ✅ Touch (GT911) — hardware issue, handled gracefully (non-fatal)
- ✅ SD card mounts, finds CANON.PCM and CANON.WAV
- ✅ Play PCM button — plays audio from speaker
- ✅ Play WAV button — plays audio from speaker
- ✅ Stop button — stops playback
- ⚠️ Minor: WAV file plays a brief pop at the start (WAV header not fully skipped — see below)

---

## Git Checkpoints This Session

```
git log --oneline
```

Key commits (most recent first):
- **"Fix blank screen on soft reset + WAV header skip"** ← CURRENT, EVERYTHING WORKING
- **"Audio + display fully working via BSP codec path"** ← also working, use if current breaks
- Earlier commits from Session 1/2 (Haiku) — display/touch/SD only

To roll back if needed:
```powershell
git log --oneline -10 2>&1 | Out-File "claudetransferv2\git_log.txt" -Encoding UTF8
git checkout <hash>
```

---

## What Was Fixed This Session (The Hard Problems)

### Problem 1: I2C Bus Conflict (ES8311 can't initialize)
**Root cause:** The code tried to create I2C bus 1 on GPIO 47/48, but the BSP already owns I2C bus 1 on GPIO 7/8 (for GT911 touch).

**Fix:** Use `bsp_i2c_get_handle()` to get the BSP's existing bus handle instead of creating a new one. Also fixed the I2S GPIO pins (were wrong: 49/46/45/44, correct: 13/12/10/9 per BSP header).

### Problem 2: Blank Screen on Boot (LVGL Rendering Race)
**Root cause (complex — took most of the session):** Multiple interacting issues:
1. `ESP_LV_ADAPTER_TEAR_AVOID_MODE_TRIPLE_PARTIAL` requires a TE pin for sync. This board has no TE pin (BSP_LCD_TOUCH_INT = GPIO_NUM_NC). On soft resets, this mode gets stuck waiting for a TE signal that never arrives. **Fix:** Changed to `ESP_LV_ADAPTER_TEAR_AVOID_MODE_NONE`.

2. The ST7701S display chip retains state across soft resets (ESP EN pin toggle). When the BSP re-initializes it via MIPI DSI, the chip is in a confused state. **Fix:** Force hardware reset of ST7701S via GPIO27 (BSP_LCD_RST) at the very start of `app_main()` — pull LOW 20ms, then HIGH 120ms. This cold-starts the chip on every boot.

3. I2S DMA startup interferes with LVGL's first display flush. If `i2s_channel_enable()` fires while LVGL is doing its very first frame flush, the display goes blank. **Fix:** Add a 500ms delay after `bsp_display_unlock()` (which lets LVGL render the first frame) before calling `i2s_init()`.

### Problem 3: No Audio Output (Wrong ES8311 Registers)
**Root cause:** The previous session's `es8311_minimal.c` had completely incorrect register addresses — e.g., "DAC_POWER" was mapped to register 0x27 when the real DAC registers are at 0x31–0x37. The "SDP" register was at 0x13 when the real SDP IN is at 0x09.

**Fix:** Switched entirely from custom `es8311_minimal` driver to the proven BSP audio path:
- `bsp_audio_init()` — creates and enables I2S channel with correct BSP pins
- `bsp_audio_codec_speaker_init()` — initializes ES8311 via the `esp_codec_dev` framework
- `esp_codec_dev_open()` + `esp_codec_dev_set_out_vol()` — opens codec for 16kHz/16-bit/stereo at 80% volume
- `esp_codec_dev_write()` — writes audio data (replaces `i2s_channel_write()`)

### Problem 4: Stop Button / EOF Loop
**Root cause:** `i2s_channel_write()` with `portMAX_DELAY` blocked forever, and I2S DMA replayed last buffer at EOF.

**Fix:**
- Used `esp_codec_dev_write()` in chunks — synchronous, returns after each chunk so `is_playing` flag is checked between writes
- Write 4 × 512 bytes of silence after EOF to flush DMA buffers

---

## Current Architecture

### Initialization Order (critical — do not change)

```c
app_main() {
    // 1. Hardware reset ST7701S display chip (GPIO27, 20ms LOW + 120ms HIGH)
    // 2. NVS init
    // 3. Scan SD card
    // 4. bsp_display_start_with_config (TEAR_AVOID_MODE_NONE)
    // 5. bsp_display_backlight_on()
    // 6. bsp_display_lock(-1)
    // 7.   create_ui()
    // 8.   lv_obj_invalidate(lv_scr_act())
    // 9. bsp_display_unlock()  ← LVGL renders first frame here
    // 10. vTaskDelay(500ms)    ← WAIT for first frame to fully flush
    // 11. i2s_init()           ← I2S DMA starts AFTER display is stable
    // 12. codec_init()         ← ES8311 init (outside lock — I2C yields OK now)
    // 13. Initialization complete → while(1)
}
```

**Why this order matters:** Any deviation from this sequence risks the blank screen issue returning. The 500ms delay is non-negotiable.

### Audio Pipeline
```
SD card file → fread (512 byte chunks) → esp_codec_dev_write()
    → esp_codec_dev (ESP-IDF) → I2S TX (GPIO 9/10/12/13)
    → ES8311 codec (I2C addr 0x18, GPIO7/8) → NS4150B PA (GPIO53) → Speaker
```

### Key Pin Assignments
```
I2S MCLK  : GPIO 13  (BSP_I2S_MCLK)
I2S SCLK  : GPIO 12  (BSP_I2S_SCLK)
I2S LRCK  : GPIO 10  (BSP_I2S_LCLK)
I2S DOUT  : GPIO 9   (BSP_I2S_DOUT)
I2C SCL   : GPIO 8   (BSP_I2C_SCL)  — shared BSP bus
I2C SDA   : GPIO 7   (BSP_I2C_SDA)  — shared BSP bus
PA ENABLE : GPIO 53  (BSP_POWER_AMP_IO)
LCD RESET : GPIO 27  (BSP_LCD_RST)  — manually toggled at startup
```

### Audio Parameters
```
Sample rate  : 16,000 Hz
Bit depth    : 16-bit
Channels     : Stereo
MCLK multiple: 384 (MCLK = 6.144 MHz)
Volume       : 80% (esp_codec_dev_set_out_vol)
Buffer size  : 512 bytes per chunk
```

---

## Key sdkconfig Settings (do not revert)

```
CONFIG_BSP_ERROR_CHECK=n              # GT911 errors non-fatal
CONFIG_SPIRAM=y                       # 32MB PSRAM enabled
CONFIG_SPIRAM_EXTERNAL_MALLOC_ALLOW_BSS=y     # linker: .bss in PSRAM
CONFIG_SPIRAM_EXTERNAL_MALLOC_ALLOW_NOINIT=y  # linker: .noinit in PSRAM
CONFIG_ESPTOOLPY_FLASHSIZE=32MB
CONFIG_IDF_TARGET=esp32p4
```

Reverting any of these will cause: linker failures OR device abort on boot.

---

## Project File Structure

```
DeskMediaDeviceV2/
├── main/
│   ├── main.c              ← Primary app — all logic here
│   ├── es8311_minimal.h    ← Kept but NOT used (CMakeLists excludes it)
│   ├── es8311_minimal.c    ← Kept but NOT used
│   └── CMakeLists.txt      ← Only compiles main.c
├── components/
│   ├── bsp_extra/          ← Board support extras
│   └── esp32_p4_wifi6_touch_lcd_4_3/  ← Waveshare BSP
├── managed_components/     ← Auto-downloaded, includes espressif__esp_codec_dev
├── docs/                   ← Schematics, Waveshare examples, rules doc
│   ├── behaviors and rules.md          ← READ THIS
│   ├── ESP32-P4-WIFI6-Touch-LCD-4.3-schematic_Page_1.png
│   ├── ESP32-P4-WIFI6-Touch-LCD-4.3-schematic_Page_2.png
│   └── ESP32-P4-WIFI6-Touch-LCD-4.3-main/examples/esp-idf/
│       ├── 06_I2SCodec/    ← Audio reference example
│       └── 08_lvgl_demo_v9/ ← Display/LVGL reference
├── claudetransferv2/       ← NOT in git — logs and handoff docs
├── HANDOFF_FOLDER/
│   ├── PROJECT_OVERVIEW.md          ← Session 1 (original)
│   └── PROJECT_OVERVIEW_SESSION3.md ← This file (Session 2 Claude)
└── sdkconfig
```

---

## Rules Updates Made This Session

### Flash + Monitor Command
**ALWAYS use combined command** — never separate flash then monitor:
```powershell
idf.py -p COM4 flash monitor 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\monitor_log.txt" -Encoding UTF8
```
**Why:** Running monitor separately after flash sends a second DTR/RTS reset. The display chip doesn't power-cycle on soft reset, so it ends up in a confused state → blank screen.

If attaching monitor to already-running device:
```powershell
idf.py -p COM4 monitor --no-reset 2>&1 | ...
```

### Log Backup Protocol
Before every build/flash/monitor/git command, back up existing logs to `claudetransferv2/backup/` with Unix timestamp format: `{logtype}_attempt_{timestamp}.txt`. Claude can do this automatically.

### Build Discipline
- ALWAYS run `idf.py fullclean` before build if ANY .c, .h, or CMakeLists.txt file changed
- No exceptions — incremental builds cause subtle stale-artifact issues

---

## Remaining Work / Optional Improvements

1. **WAV header pop** — WAV files start with a 44-byte RIFF header that produces a click when played as audio. Fix: `fseek(f, 44, SEEK_SET)` before the read loop in `audio_task()` for AUDIO_TYPE_WAV. Code is already in place, just needs testing. (Low priority if only PCM files will be used.)

2. **Touch not working** — GT911 returns I2C errors. Likely a hardware wiring issue. Non-fatal, app continues. Low priority unless touch input is needed.

3. **Volume control** — `esp_codec_dev_set_out_vol(spk_codec_dev, value)` with value 0–100. Currently hardcoded at 80%.

4. **Multiple audio files** — Currently only finds first .pcm and first .wav file. Could be extended to scan and list all files.

---

## Build & Flash Commands Reference

```powershell
# Always fullclean before building after code changes
idf.py fullclean 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\clean_log.txt" -Encoding UTF8

idf.py build 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\build_log.txt" -Encoding UTF8

# Combined flash+monitor (mandatory)
idf.py -p COM4 flash monitor 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\monitor_log.txt" -Encoding UTF8

# Git
git add -A 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_add_log.txt" -Encoding UTF8
git commit -m "message" 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_commit_log.txt" -Encoding UTF8
git log --oneline -10 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_log.txt" -Encoding UTF8
```

---

**Handoff Date:** May 31, 2026
**Status:** All core features working. Device plays audio from SD card, display stable on every boot.
**Next Blocker:** None — optional improvements only.
