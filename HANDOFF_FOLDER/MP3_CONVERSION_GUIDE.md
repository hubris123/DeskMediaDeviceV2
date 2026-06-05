# MP3 Conversion Guide — Desk Media Device

## Required Format
| Setting | Value |
|---------|-------|
| Sample Rate | 22050 Hz |
| Bitrate | 96 kbps |
| Channels | Stereo |
| Format | MP3 (CBR preferred) |

## FFmpeg Command

Single file:
```
ffmpeg -i "input.mp3" -ac 2 -ar 22050 -ab 96k -codec:a libmp3lame "output.mp3"
```

Batch convert entire folder (PowerShell):
```powershell
Get-ChildItem ".\source\*.mp3" | ForEach-Object {
    ffmpeg -i $_.FullName -ac 2 -ar 22050 -ab 96k -codec:a libmp3lame ".\converted\$($_.Name)"
}
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
At 22050Hz mono 96kbps:
- 1 minute ≈ 720 KB
- 3 minutes ≈ 2.2 MB
- 5 minutes ≈ 3.6 MB

## Notes
- The device codec is initialized at 22050 Hz — files at other sample rates will play at wrong speed
- Keep files under 4MB each
- SD card folder must be named exactly: music (lowercase)
- FFmpeg is available on the dev machine at C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2\
