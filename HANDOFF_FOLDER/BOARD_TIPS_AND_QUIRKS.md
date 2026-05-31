# ESP32-P4-WIFI6-Touch-LCD-4.3 — Board Tips & Quirks
## Hard-won lessons from debugging. Read before touching anything.

---

## Display (ST7701S via MIPI DSI)

### Always force hardware reset at startup
The ST7701S retains state across soft resets (ESP EN pin toggle). Without an explicit
hardware reset, the display initializes into a confused state and LVGL renders nothing.

Add this at the very top of `app_main()`, before NVS init or anything else:
```c
gpio_set_direction(BSP_LCD_RST, GPIO_MODE_OUTPUT);
gpio_set_level(BSP_LCD_RST, 0);
vTaskDelay(pdMS_TO_TICKS(20));
gpio_set_level(BSP_LCD_RST, 1);
vTaskDelay(pdMS_TO_TICKS(120));
```
`BSP_LCD_RST = GPIO_NUM_27` — defined in the BSP header.

### Use TEAR_AVOID_MODE_NONE
`ESP_LV_ADAPTER_TEAR_AVOID_MODE_TRIPLE_PARTIAL` requires a TE (tearing effect) pin for
frame sync. This board has no TE pin (`BSP_LCD_TOUCH_INT = GPIO_NUM_NC`). On soft resets,
TRIPLE_PARTIAL gets stuck waiting for a TE signal that never comes → blank screen.

In your display config:
```c
bsp_display_cfg_t cfg = {
    .tear_avoid_mode = ESP_LV_ADAPTER_TEAR_AVOID_MODE_NONE,
    ...
};
```

### Never use separate flash + monitor commands
Running `idf.py monitor` separately after `idf.py flash` sends a second DTR/RTS reset
via the USB-UART chip. The display chip doesn't power-cycle on that reset → blank screen.

**Always use the combined command:**
```powershell
idf.py -p COM4 flash monitor 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\monitor_log.txt" -Encoding UTF8
```
If you need to attach monitor to an already-running device: `idf.py -p COM4 monitor --no-reset`

### I2S DMA and display DMA conflict on first render
If `i2s_channel_enable()` fires while LVGL is doing its very first frame flush to the display,
the I2S DMA and display DMA compete and the screen goes blank permanently (for that boot).

**Rule:** Always wait for LVGL to complete its first render before enabling I2S. A 500ms
delay after `bsp_display_unlock()` is reliable:
```c
bsp_display_lock(-1);
create_ui();
lv_obj_invalidate(lv_scr_act());
bsp_display_unlock();
vTaskDelay(pdMS_TO_TICKS(500));  // Let LVGL flush first frame before I2S starts
i2s_init();  // Safe to enable I2S DMA now
```

### LVGL blank-frame race condition
LVGL starts rendering blank frames as soon as `bsp_display_start_with_config()` starts
internally (at LVGL adapter init). GT911 touch initialization inside that same call takes
~130ms. During those 130ms, LVGL renders blank frames. If your `create_ui()` runs after
this window, use `bsp_display_lock(-1)` to block LVGL while building the UI.

Any OS yield (vTaskDelay, I2C semaphore wait, etc.) that happens before `create_ui()` and
after `bsp_display_start_with_config()` gives LVGL a chance to render a blank frame that
can leave the flush pipeline stuck.

---

## Audio (ES8311 Codec + NS4150B Amplifier)

### Use the BSP audio path — do NOT write ES8311 registers manually
The ES8311 register map is complex and the addresses are non-obvious. A custom minimal
driver will almost certainly have wrong addresses and produce silence. Use the proven path:

```c
// I2S setup
bsp_audio_init(&i2s_cfg);                    // Creates + enables I2S channel

// Codec setup
esp_codec_dev_handle_t spk = bsp_audio_codec_speaker_init();
esp_codec_dev_sample_info_t fs = { .sample_rate=16000, .channel=2, .bits_per_sample=16 };
esp_codec_dev_open(spk, &fs);
esp_codec_dev_set_out_vol(spk, 80);           // 0–100

// Playback
esp_codec_dev_write(spk, buffer, bytes);
```

### I2C bus ownership — use bsp_i2c_get_handle()
The BSP owns I2C bus 1 on GPIO7 (SDA) / GPIO8 (SCL). Do NOT call `i2c_new_master_bus()`
for the ES8311 — it will conflict. Use `bsp_i2c_get_handle()` after
`bsp_display_start_with_config()` has run (which initializes the I2C bus).

### Correct I2S pin assignments
These are the ONLY correct pins for I2S audio on this board:
```
MCLK = GPIO 13  (BSP_I2S_MCLK)
BCLK = GPIO 12  (BSP_I2S_SCLK)
WS   = GPIO 10  (BSP_I2S_LCLK)
DOUT = GPIO 9   (BSP_I2S_DOUT)  → to ES8311 DSDIN
DIN  = GPIO 11  (BSP_I2S_DSIN)  → from ES8311 ASDOUT (microphone)
PA   = GPIO 53  (BSP_POWER_AMP_IO)
```
GPIO 44 is the SD card CMD pin — do NOT use it for I2S.

### WAV files have a 44-byte header — skip it
Playing a WAV file without skipping the header produces a loud pop at the start.
The raw PCM audio data starts at byte 44 for standard WAV files:
```c
if (audio_type == AUDIO_TYPE_WAV) {
    fseek(f, 44, SEEK_SET);
}
```

### Codec init sequence matters relative to display
`bsp_audio_codec_speaker_init()` does many I2C writes that yield to the OS. These yields
are harmless AFTER LVGL has rendered its first frame. Call codec init AFTER the 500ms delay:
```
display start → create_ui → unlock → 500ms delay → i2s_init → codec_init
```

---

## Touch (GT911)

### GT911 has a hardware wiring issue on this board
The GT911 capacitive touch controller cannot communicate via I2C. It returns errors:
```
E: lcd_panel.io.i2c: i2c transaction failed
E: GT911: touch_gt911_read_cfg: GT911 read error!
```
This is a hardware issue (wiring), not software. The app handles it gracefully.
**Required sdkconfig setting:** `CONFIG_BSP_ERROR_CHECK=n` — prevents GT911 I2C failure
from aborting the entire app.

GT911 initialization takes ~130ms to fail. This is the window where LVGL renders blank
frames — see the display notes above.

---

## Memory / Linker

### PSRAM settings — do not revert
This board has 512KB internal RAM + 32MB PSRAM. Without these settings, the linker fails:
```
CONFIG_SPIRAM=y
CONFIG_SPIRAM_EXTERNAL_MALLOC_ALLOW_BSS=y
CONFIG_SPIRAM_EXTERNAL_MALLOC_ALLOW_NOINIT=y
```
These allow `.bss` and `.noinit` sections to live in PSRAM, which is required for the
display framebuffer allocations to fit.

---

## Build Discipline

### Always fullclean after code changes
Incremental builds cause subtle stale-artifact issues on this project. Always:
```powershell
idf.py fullclean  # then build
```
Rule: any change to `.c`, `.h`, or `CMakeLists.txt` = mandatory fullclean.

### Log backup before every operation
Before running build/flash/monitor/git, back up existing logs in `claudetransferv2/` to
`claudetransferv2/backup/` with Unix timestamp naming: `{logtype}_attempt_{timestamp}.txt`
This ensures you always know which log corresponds to which attempt.

---

## Useful References

- **Schematic PNGs:** `docs/ESP32-P4-WIFI6-Touch-LCD-4.3-schematic_Page_1.png` and `_Page_2.png`
- **Audio example:** `docs/ESP32-P4-WIFI6-Touch-LCD-4.3-main/examples/esp-idf/06_I2SCodec/`
- **BSP header (all pin defines):** `components/esp32_p4_wifi6_touch_lcd_4_3/include/bsp/esp32_p4_wifi6_touch_lcd_4_3.h`
- **ES8311 register reference:** `managed_components/espressif__esp_codec_dev/device/es8311/es8311.c`
- **Waveshare docs:** https://docs.waveshare.com/ESP32-P4-WIFI6-Touch-LCD-4.3/Development-Environment-Setup-IDF
