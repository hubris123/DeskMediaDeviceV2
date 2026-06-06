# Video Player Debug Attempts Log
**Project:** ESP32-P4 DeskMediaDevice — MJPEG video player
**Session:** June 5, 2026
**IDF Version:** v5.5.4
**Board:** Waveshare ESP32-P4-WIFI6-Touch-LCD-4.3 (ST7701S, MIPI DSI, 480×800 physical portrait)

---

## Attempt 1 — draw_bitmap with external PSRAM buffer (no alignment)
**What:** `esp_lcd_panel_draw_bitmap()` with our own `heap_caps_malloc` PSRAM decode buffer
**Result:** Vertical striping. Panel ignored external buffer, reading internal DMA fb instead.

## Attempt 2 — dummy_draw_blit with physical coords
**What:** `esp_lv_adapter_set_dummy_draw()` + `esp_lv_adapter_dummy_draw_blit()` using physical panel coords (480×800)
**Result:** Striping. Wrong coordinates — dummy_draw_blit expects logical LVGL coords (800×480 landscape).

## Attempt 3 — dummy_draw_blit with logical coords
**What:** `dummy_draw_blit(0, 0, 800, 480, buf)` with video re-encoded as landscape 800×480
**Result:** Striping. Still corrupted.

## Attempt 4 — RGB888 output format
**What:** Changed JPEG decoder output to `JPEG_DECODE_OUT_FORMAT_RGB888` to avoid byte-order ambiguity
**Result:** Noise/static. Panel configured for RGB565 — 3-byte pixels interpreted as 1.5 pixels. out_size=1152000 vs expected 768000.

## Attempt 5 — RGB565 with RGB element order RGB
**What:** `JPEG_DECODE_OUT_FORMAT_RGB565` + `JPEG_DEC_RGB_ELEMENT_ORDER_RGB`
**Result:** Striping.

## Attempt 6 — RGB565 with BGR element order
**What:** `JPEG_DECODE_OUT_FORMAT_RGB565` + `JPEG_DEC_RGB_ELEMENT_ORDER_BGR`
**Result:** Striping. Same visual result as RGB — color order is not the issue.

## Attempt 7 — Decode directly into panel DMA framebuffers
**What:** `esp_lcd_dpi_panel_get_frame_buffer()` to get panel fbs, decode JPEG into those, then `draw_bitmap`
**Result:** CRASH — `Load access fault` in `dma2d_connect`. Panel framebuffers are NOT CPU-writable. DMA2D faults when CPU tries to memcpy into them.

## Attempt 8 — jpeg_alloc_decoder_mem + draw_bitmap
**What:** Used `jpeg_alloc_decoder_mem()` (correct alignment API) to allocate PSRAM decode buffer, then `draw_bitmap`
**Result:** Striping. `out_size=768000` matches expected (480×800×2). Stride confirmed correct. No crash. 602 frames decoded and displayed — just corrupted.

## Attempt 9 — esp_cache_msync flush before draw_bitmap
**What:** `esp_cache_msync(decode_buf, size, ESP_CACHE_MSYNC_FLAG_DIR_C2M)` to flush CPU cache to PSRAM before DMA reads it, then `draw_bitmap`
**Result:** Building/testing at time of writing.

## Attempt 10 — esp_lcd_dpi_panel_enable_dma2d()
**What:** IDF v6.0 API to enable DMA2D hook on panel for proper PSRAM→display transfers
**Result:** COMPILE ERROR — API does not exist in IDF v5.5.4. Doc was for v6.0.

---

## Known Facts (confirmed working)
- Touch: ✅ Working
- Audio (MP3 streaming): ✅ Working
- JPEG decode: ✅ Working — correct size, correct frame count, no errors
- LVGL pause/resume: ✅ Working — weather screen returns cleanly after video
- Playlist/shuffle: ✅ Working
- PSRAM buffers: ✅ Allocated correctly
- Stride: ✅ Confirmed 480×800×2 = 768000 bytes, matches decoder output
- Panel framebuffers: NOT CPU-writable (DMA only)
- `draw_bitmap` on DPI panel uses DMA2D internally to copy src→internal fb

## Core Unsolved Problem
How to get our `jpeg_alloc_decoder_mem` PSRAM RGB565 buffer displayed without corruption
on this MIPI DSI DPI panel under IDF v5.5.4.

The data is correct (correct size, correct frame count) but the DMA2D transfer from
our PSRAM buffer to the panel's internal framebuffer produces vertical striping,
suggesting a stride or alignment mismatch in the DMA2D copy path.

---

## SOLUTION — What Finally Worked

**Display path:**
- HW JPEG decoder writes directly into panel DMA framebuffer (`lcd_fb[0]` at `0x48007f80`, internal RAM)
- `esp_lv_adapter_dummy_draw_blit(disp, 0, 0, 480, 800, lcd_fb[0], true)` — physical portrait coords
- No PPA, no memcpy, no cache sync needed

**Key facts discovered:**
- `dummy_draw_blit` is a raw passthrough — NO rotation happens in this path
- DMA2D cannot read from PSRAM reliably — always decode into internal RAM (panel framebuffers)
- PPA scrambles data when source is in PSRAM
- Panel framebuffers are at `0x48007f80` (internal RAM), writable by HW JPEG decoder
- Buffer size: `480 * 800 * 2 = 768000` bytes, no stride padding

**Video file format:**
- Resolution: 480×800 portrait MJPEG
- FFmpeg command: `ffmpeg -i input.mp4 -c:v mjpeg -q:v 5 -vf "transpose=1,scale=480:800" -r 20 "01 - Title.mjpeg"`
- Audio: `ffmpeg -i input.mp4 -ac 2 -ar 44100 -q:a 2 "01 - Title.mp3"`
- Place both in `/sdcard/video/`

**Color order:** `JPEG_DEC_RGB_ELEMENT_ORDER_BGR` with `JPEG_DECODE_OUT_FORMAT_RGB565`

---

## Leads Not Yet Tried
- `esp_cache_msync` flush (Attempt 9 — in progress)
- Using PPA (Pixel Processing Accelerator) instead of DMA2D for the copy
- Registering a custom `draw_bitmap_hook` on the panel (available in v5.5.4 as `esp_lcd_dpi_panel_register_hooks`)
- Checking if `draw_bitmap` on this BSP ignores the source buffer entirely and we need a different approach
- Looking at how LVGL adapter's PPA rotation path works and replicating it for video
- Allocating the decode buffer with `esp_cache_get_alignment` size padding to match DMA2D requirements

## Video File Requirements
- Format: MJPEG (raw concatenated JPEG frames, FF D8 ... FF D9)
- Resolution: 480×800 portrait (physical panel orientation)
- FFmpeg: `ffmpeg -i input.mp4 -c:v mjpeg -q:v 5 -vf "scale=480:800,transpose=1" -r 20 "01 - Title.mjpeg"`
- Audio: `ffmpeg -i input.mp4 -ac 2 -ar 44100 -q:a 2 "01 - Title.mp3"`
- Place both in /sdcard/video/ with matching base names
