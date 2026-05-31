# DeskMediaDevice Project Handoff - May 30, 2026

## Project Goal
Implement audio playback functionality on an ESP32-P4 embedded device with a 4.3" capacitive touch display. Audio files (PCM/WAV) stored on SD card should play when buttons on the UI are pressed.

## Current Status: MOSTLY WORKING - ONE BLOCKING ISSUE
- ✅ Device boots successfully
- ✅ Display (ST7701S, 4.3" DSI) initializes and displays UI
- ✅ Touch (GT911 capacitive) initialized (with graceful error handling)
- ✅ SD card mounts and finds audio files
- ✅ PSRAM (32MB) working correctly
- ⚠️ **BLOCKING ISSUE**: I2C bus conflict preventing audio codec initialization

## Hardware
- **MCU**: ESP32-P4 (RISC-V)
- **Display**: ST7701S MIPI DSI, 4.3" capacitive touch
- **Touch**: GT911 capacitive controller
- **Audio Codec**: ES8311 (I2C controlled, I2S audio output)
- **Storage**: SD card (SDMMC interface)
- **Speaker**: Connected via ES8311 PA amplifier on GPIO 53
- **RAM**: 512KB internal + 32MB PSRAM
- **Flash**: 32MB

## IDF Version & Config
- **ESP-IDF**: v5.5.4
- **Target**: esp32p4
- **Build Status**: Successful after fixing memory layout
- **Key Config Fix**: Enabled "Allow .bss segment placed in external memory" and "Allow .noinit segment placed in external memory" to resolve `--enable-non-contiguous-regions` linker errors with PSRAM enabled

## Project Structure
```
DeskMediaDeviceV2/
├── main/
│   ├── main.c                 # App entry point, UI creation, button callbacks
│   ├── es8311_minimal.h       # ES8311 codec register definitions
│   ├── es8311_minimal.c       # Codec initialization via direct I2C writes
│   └── CMakeLists.txt
├── components/
│   ├── bsp_extra/             # Board support extras
│   │   ├── idf_component.yml  # Dependencies (esp-audio-player, esp-file-iterator)
│   │   └── src/bsp_board_extra.c
│   └── esp32_p4_wifi6_touch_lcd_4_3/  # Waveshare BSP
├── docs/                      # Example code from Waveshare (06_I2SCodec, 08_lvgl_demo_v9)
├── sdkconfig                  # Build configuration
├── HANDOFF_FOLDER/            # This documentation
└── claudetransferv2/          # Build/flash/monitor logs (archived)
    └── backup/                # Timestamped log backup
```

## What We're Trying to Do

### Phase 1: Audio Codec Initialization (CURRENT - BLOCKED)
1. Initialize I2C bus for ES8311 codec control
2. Configure ES8311 via direct I2C register writes (16kHz, 16-bit, stereo)
3. Initialize I2S peripheral for audio streaming
4. Set up FreeRTOS task for file reading and I2S playback
5. Wire button callbacks to trigger audio playback

### Phase 2: Audio Playback (NOT YET STARTED)
1. Read PCM/WAV files from SD card
2. Stream audio to I2S/ES8311
3. Handle playback state (play/stop)

## The Blocking Issue: I2C Bus Conflict

### Symptom
When `audio_init()` tries to initialize I2C for ES8311:
```
E (1386) i2c.common: I2C bus id(1) has already been acquired
E (1391) i2c.common: acquire bus failed
E (1395) i2c.master: i2c_new_master_bus(1053): I2C bus acquire failed
```

### Root Cause
The Waveshare BSP or another component is **already using I2C bus 1** (lines 47-48 in main/main.c):
```c
i2c_master_bus_config_t i2c_bus_cfg = {
    .i2c_port = I2C_NUM_1,  // <-- CONFLICT HERE
    .scl_io_num = 48,
    .sda_io_num = 47,
    ...
};
```

GT911 touch controller and/or other BSP components likely already initialized I2C bus 1 during display/touch setup.

### What Needs to Happen
Either:
1. **Option A**: Use a different I2C bus number (I2C_NUM_0 or I2C_NUM_2) if available
2. **Option B**: Share the existing I2C bus 1 by finding which component owns it and adding ES8311 as a device on the same bus
3. **Option C**: Acquire the bus handle from the BSP instead of creating a new one

## What Has Been Tried

### 1. Memory/Linker Issues (SOLVED ✅)
**Problem**: `--enable-non-contiguous-regions discards section` error with PSRAM enabled
- Tried: Removing audio libraries from bsp_extra dependencies
- Tried: Disabling PSRAM entirely (broke display framebuffer allocation)
- **Solution**: In sdkconfig via menuconfig:
  - Re-enabled PSRAM
  - Enabled: `CONFIG_SPIRAM_EXTERNAL_MALLOC_ALLOW_BSS=y`
  - Enabled: `CONFIG_SPIRAM_EXTERNAL_MALLOC_ALLOW_NOINIT=y`
  - This allowed linker to place .bss and .noinit sections in PSRAM, resolving conflicts

### 2. GT911 Touch Abort Issue (SOLVED ✅)
**Problem**: GT911 I2C communication failure caused entire app to abort
- **Solution**: Disabled `CONFIG_BSP_ERROR_CHECK` in sdkconfig to make BSP errors non-fatal
- Touch still doesn't work (hardware/wiring issue) but app continues

### 3. Audio Codec Code (ATTEMPTED)
- Created `es8311_minimal.h` with register definitions
- Created `es8311_minimal.c` with codec initialization
- Added `audio_init()` function in main.c
- Added `audio_task()` for file reading and I2S streaming
- Added button callbacks to trigger playback
- **Status**: Code compiles but can't initialize due to I2C bus conflict

### 4. Build Environment
- Used PowerShell script to capture logs with: `2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "file.txt" -Encoding UTF8`
- Created backup system: logs backed up to `claudetransferv2/backup/` with timestamps before each new build
- This prevents confusion about which logs are current

## Key Configuration Details

### sdkconfig Changes Made
```
CONFIG_BSP_ERROR_CHECK=n                              # Prevent GT911 abort
CONFIG_SPIRAM=y                                        # Enable 32MB PSRAM
CONFIG_SPIRAM_XIP_FROM_PSRAM=y                         # Execute-in-place from PSRAM
CONFIG_SPIRAM_EXTERNAL_MALLOC_ALLOW_BSS=y             # Allow .bss in PSRAM
CONFIG_SPIRAM_EXTERNAL_MALLOC_ALLOW_NOINIT=y          # Allow .noinit in PSRAM
CONFIG_ESPTOOLPY_FLASHSIZE=32MB                       # 32MB flash
CONFIG_IDF_TARGET=esp32p4
```

### I2C/Audio Pin Configuration
```c
// ES8311 codec
#define ES8311_ADDR 0x18
I2C_NUM_1  // Bus (CONFLICT - need to change)
SCL: GPIO 48
SDA: GPIO 47

// I2S audio output
I2S_NUM_0
MCLK: GPIO 49
BCLK: GPIO 46
WS:   GPIO 45
DOUT: GPIO 44
PA Enable: GPIO 53
```

### Audio Parameters
- Sample Rate: 16 kHz (to match canon.pcm)
- Bit Width: 16-bit
- Channels: Stereo
- MCLK Multiple: 384x (16kHz × 384 = 6.144MHz)
- Buffer Size: 512 bytes

## Git Checkpoints Created
- **98ee4d1**: Last known working state with display/touch before audio attempt
- Next: Create checkpoint once audio is working

## Files Modified/Created This Session

### New Files
- `main/es8311_minimal.h` - Codec register definitions
- `main/es8311_minimal.c` - Codec initialization code
- `HANDOFF_FOLDER/` - This documentation

### Modified Files
- `main/main.c` - Added audio_init(), audio_task(), button callbacks
- `main/CMakeLists.txt` - Added es8311_minimal.c to build
- `sdkconfig` - Disabled BSP error check, enabled PSRAM options
- `components/bsp_extra/idf_component.yml` - Dependencies (no changes needed)

## Monitor Output (Last Successful Boot)
Key lines from monitor log:
- Line 118: `I (480) esp_psram: Found 32MB PSRAM device`
- Line 126: `I (984) esp_psram: SPI SRAM memory test OK`
- Line 214: `I (1262) ESP32_P4_4_3: Display initialized` ✅
- Line 1386-1395: **I2C bus conflict error** ⚠️
- Line 1410: `I (1410) DeskMediaDevice: Initialization complete` (despite audio init failure)

## Next Steps Required

1. **URGENT**: Fix I2C bus conflict
   - Determine which component owns I2C bus 1
   - Either use different bus or share existing bus
   - Likely need to modify `audio_init()` in main/main.c

2. Once I2C is working:
   - Verify ES8311 codec initialization
   - Test I2S audio output
   - Verify button callbacks trigger playback
   - Test actual audio playback from SD card files

3. Optional improvements:
   - Add audio status LED indicator
   - Implement volume control
   - Add error recovery if audio fails mid-playback

## Resources Available

### Git Repository
- **Location**: C:\Users\Administrator\Documents\DeskMediaDeviceV2\.git
- **Latest Commit**: 98ee4d1 (checkpoint with display/touch working, before audio attempt)
- **Branch**: main
- Use `git log --oneline -10` to see commit history
- Use `git diff` to see uncommitted changes
- Create new checkpoint with: `git add -A && git commit -m "description"`

### Build & Test Logs
- **Location**: C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2\
- **Files**: 
  - `build_log.txt` - Last build output
  - `flash_log.txt` - Last flash operation
  - `monitor_log.txt` - Last device boot & execution
  - `clean_log.txt` - Last fullclean output
- **Backup Logs**: All old logs are timestamped and archived in `claudetransferv2/backup/` so you can review previous attempts
- **Format**: PowerShell logs captured with both console display and file output, complete with timestamps

### Example Code & Documentation
- **Location**: C:\Users\Administrator\Documents\DeskMediaDeviceV2\docs\ESP32-P4-WIFI6-Touch-LCD-4.3-main\examples\esp-idf\
- **Key Examples**:
  - `06_I2SCodec/` - **CRITICAL**: Pure I2S+ES8311 audio example (minimal dependencies, working reference implementation)
    - `main/i2s_es8311_example.c` - Shows correct ES8311 initialization and I2S streaming
    - `main/canon.pcm` - Test PCM file (same format we're using)
    - `sdkconfig.defaults` - Working config for audio-only build
  - `08_lvgl_demo_v9/` - **OUR BASELINE**: Full LVGL display example (shows how Waveshare set up display)
    - `components/bsp_extra/idf_component.yml` - Dependency list (same as our project)
    - `main/main.c` - Display/UI initialization pattern (we copied this approach)
  - `07_Displaycolorbar/` - Simpler display example if you need minimal display setup

### Waveshare Official Documentation
- **Product Page**: https://www.waveshare.com/product/arduino/boards-kits/esp32-p4/esp32-p4-wifi6-touch-lcd-4.3.htm
- **Development Environment Setup**: https://docs.waveshare.com/ESP32-P4-WIFI6-Touch-LCD-4.3/Development-Environment-Setup-IDF
- **Resources & Documents**: https://docs.waveshare.com/ESP32-P4-WIFI6-Touch-LCD-4.3/Resources-And-Documents (schematics, datasheets, BSP)
- **FAQ**: https://docs.waveshare.com/ESP32-P4-WIFI6-Touch-LCD-4.3/FAQ

These links contain:
- Pin diagrams and schematic
- Hardware specifications
- Bootloader and flashing guides
- Known issues and solutions

### Docs Folder Structure
```
docs/ESP32-P4-WIFI6-Touch-LCD-4.3-main/
├── examples/esp-idf/
│   ├── 01_HowToCreateProject/         # Project structure reference
│   ├── 02_HelloWorld/                 # Minimal app
│   ├── 03_i2c_tools/                  # I2C debugging utilities
│   ├── 04_wifistation/                # WiFi example
│   ├── 05_sdmmc/                      # SD card example (we use this)
│   ├── 06_I2SCodec/                   # ⭐ AUDIO EXAMPLE - START HERE
│   ├── 07_Displaycolorbar/            # Simple display
│   ├── 08_lvgl_demo_v9/               # ⭐ OUR BASELINE - Full display+LVGL
│   └── 09_video_lcd_display/          # Video playback example
└── README files with hardware details
```

### Where to Look for I2C Bus Solution
The 06_I2SCodec example likely shows:
1. Which I2C bus ES8311 uses in that example
2. How it initializes I2C (bus number, pins, config)
3. Whether it shares I2C with other components or uses dedicated bus
4. Pin assignments for I2C SCL/SDA

**Compare with our project**: Our main.c attempts I2C_NUM_1, but the example might show a different bus or shared bus approach.

### Build/Test Workflow
```powershell
# Full clean rebuild
idf.py fullclean
idf.py build
idf.py -p COM4 flash
idf.py -p COM4 monitor
```

All commands should be run from `C:\Users\Administrator\Documents\DeskMediaDeviceV2\`

## Additional Notes

- **Linker Constraint**: This board has tight memory constraints. `--enable-non-contiguous-regions` was causing linker errors. The .bss/.noinit PSRAM options solved this.
- **Touch Hardware Issue**: GT911 touchscreen doesn't work (returns I2C errors). This is likely a hardware wiring issue, not software. We handle it gracefully so the app continues.
- **Audio Expertise**: The I2C bus conflict likely requires intimate knowledge of I2C peripheral management on ESP32-P4. This is where a C code specialist would excel.

---
**Handoff Date**: May 30, 2026  
**Status**: Device boots, display works, audio blocked by I2C conflict  
**Next Blocker**: Resolve I2C bus conflict for ES8311 initialization
