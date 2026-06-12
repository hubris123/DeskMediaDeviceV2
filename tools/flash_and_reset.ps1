param([string]$Port = "COM4")

$Esptool = "C:\Users\Administrator\.espressif\python_env\idf5.5_py3.12_env\Scripts\esptool.exe"
$Build   = "C:\Users\Administrator\Documents\DeskMediaDeviceV2\build"

Write-Host "Flashing ESP32-P4 on $Port with watchdog reset..."

$bootloader = $Build + "\bootloader\bootloader.bin"
$partition  = $Build + "\partition_table\partition-table.bin"
$app        = $Build + "\DeskMediaDevice.bin"

# App offset 0x20000 since the OTA partition layout (ota_0); otadata at 0x10000.
# Erase otadata first (all-0xFF -> bootloader falls back to ota_0) so a USB flash
# always boots the freshly flashed image even if OTA had switched to ota_1.
& $Esptool --chip esp32p4 --port $Port --baud 921600 --before default_reset --after no_reset erase_region 0x10000 0x2000
& $Esptool --chip esp32p4 --port $Port --baud 921600 --before no_reset --after watchdog_reset write_flash --flash_mode dio --flash_size 32MB --flash_freq 80m 0x2000 $bootloader 0x8000 $partition 0x20000 $app

if ($LASTEXITCODE -eq 0) {
    Write-Host "Done - watchdog reset triggered, device should boot shortly."
} else {
    Write-Host "Failed with exit code $LASTEXITCODE"
}
