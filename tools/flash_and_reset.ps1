param([string]$Port = "COM4")

$Esptool = "C:\Users\Administrator\.espressif\python_env\idf5.5_py3.12_env\Scripts\esptool.exe"
$Build   = "C:\Users\Administrator\Documents\DeskMediaDeviceV2\build"

Write-Host "Flashing ESP32-P4 on $Port with watchdog reset..."

$bootloader = $Build + "\bootloader\bootloader.bin"
$partition  = $Build + "\partition_table\partition-table.bin"
$app        = $Build + "\lvgl_demo_v9.bin"

& $Esptool --chip esp32p4 --port $Port --baud 921600 --before default_reset --after watchdog_reset write_flash --flash_mode dio --flash_size 32MB --flash_freq 80m 0x2000 $bootloader 0x8000 $partition 0x10000 $app

if ($LASTEXITCODE -eq 0) {
    Write-Host "Done - watchdog reset triggered, device should boot shortly."
} else {
    Write-Host "Failed with exit code $LASTEXITCODE"
}
