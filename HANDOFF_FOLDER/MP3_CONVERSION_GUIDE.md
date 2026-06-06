# MP3 Conversion Guide — Desk Media Device

## Required Format
| Setting | Value |
|---------|-------|
| Sample Rate | 32000 Hz |
| Bitrate | 128 kbps |
| Channels | Stereo |
| Format | MP3 (CBR preferred) |

**NOTE:** The codec is fixed at 32000Hz in firmware — all MP3s MUST be at this rate or they will play at wrong speed.

## FFmpeg Command — Music Player (/sdcard/music/)

Requires 0.5s leading silence (`adelay=500|500`) to suppress PA amplifier pop on track start.

Single file:
```
ffmpeg -i "input.mp3" -af "adelay=500|500" -ac 2 -ar 32000 -ab 128k -codec:a libmp3lame "output.mp3"
```

Batch convert entire folder (PowerShell):
```powershell
Get-ChildItem ".\source\*.mp3" | ForEach-Object {
    ffmpeg -i $_.FullName -af "adelay=500|500" -ac 2 -ar 32000 -ab 128k -codec:a libmp3lame ".\converted\$($_.Name)"
}
```

## FFmpeg Command — Video Audio (/sdcard/video/)

Do NOT add leading silence — video audio starts alongside the video, codec is already warm.

```
ffmpeg -i "input.mp4" -ac 2 -ar 32000 -ab 128k -codec:a libmp3lame "01 - Title.mp3"
```

## File Naming Convention
Files must be named with a 2-digit track number prefix:
```
01 - Song Title.mp3
02 - Song Title.mp3
03 - Song Title.mp3
```
- First 2 characters = track number shown in the player textarea
- Everything after = scroll title (minus .mp3 extension)
- Place converted files in /sdcard/music/ on the SD card

## Expected File Sizes
At 32000Hz stereo 128kbps:
- 1 minute ≈ 960 KB
- 3 minutes ≈ 2.9 MB
- 5 minutes ≈ 4.8 MB

## Notes
- **0.5s leading silence is required** — the PA amplifier needs time to settle after enabling. Files without leading silence will pop on playback start. Use `adelay=500|500` in FFmpeg.
- The codec is fixed at 32000Hz — files at other sample rates will play at wrong speed
- SD card folder must be named exactly: music (lowercase)
- FFmpeg is available on the dev machine at C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2\
