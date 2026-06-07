# Media Requirements — Video Player

## Video File (MJPEG)

### Required Format
| Setting | Value |
|---------|-------|
| Codec | MJPEG |
| Resolution | 480 x 800 (portrait) |
| Frame Rate | 20 fps |
| Quality | -q:v 5 (lower = better, range 1-31) |
| Container | Raw MJPEG (.mjpeg) |

### FFmpeg Conversion Command
```powershell
ffmpeg -i "input.mp4" -c:v mjpeg -q:v 5 -vf "transpose=1,scale=480:800" -r 20 "01 - Title.mjpeg"
```

**Filter explanation:**
- `transpose=1` — rotate 90 degrees clockwise first
- `scale=480:800` — then scale to portrait resolution
- Order matters — transpose before scale

### Verify the File
```powershell
ffmpeg -i "01 - Title.mjpeg"
```
Look for: `480x800` in the stream info. Example output:
```
Stream #0:0: Video: mjpeg, yuvj420p, 480x800, 25 fps
```

---

## Audio File (MP3)

### Required Format
| Setting | Value |
|---------|-------|
| Codec | MP3 |
| Sample Rate | 32000 Hz |
| Bitrate | 128 kbps |
| Channels | Stereo |

**NOTE:** Codec is fixed at 32000Hz in firmware — do NOT use 44100Hz or audio will play at wrong speed. No `adelay` needed (codec is already warm when video starts).

### FFmpeg Conversion Command
```powershell
ffmpeg -i "input.mp4" -ac 2 -ar 32000 -ab 128k -codec:a libmp3lame "01 - Title.mp3"
```

---

## File Naming and Placement

- Video and audio files must have **matching base names**
- Both go in `/sdcard/video/` on the SD card
- Example:
  ```
  /sdcard/video/01 - Title.mjpeg
  /sdcard/video/01 - Title.mp3
  ```
- The player picks the matching `.mp3` automatically when playing a `.mjpeg`

---

## Notes
- The ESP32-C6 WiFi coprocessor is **2.4GHz only** — unrelated to video but worth noting
- Video plays at ~20fps — hardware JPEG decoder limit with current setup
- Files are pre-loaded into PSRAM on boot (up to 2 clips, 10MB each)
- Files larger than 10MB will be truncated — keep clips under 10MB
- At q:v 5 and 20fps, a 30-second clip is approximately 9-10MB
