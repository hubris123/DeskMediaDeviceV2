# ==============================================================================
# Flash + Parse Logs - Flash device and analyze output
# ==============================================================================
# Runs idf.py flash with dual output (terminal + log file), then parses
# the flash log for errors and warnings.
#
# After flashing completes, manually power-cycle the device (unplug/replug
# or press RST button), then run monitor.ps1 to capture boot output.
#
# Usage (from project root in PowerShell):
#   .\flash.ps1
# ==============================================================================

Write-Host ""
Write-Host "========== FLASH ==========" -ForegroundColor Cyan
Write-Host ""

idf.py -p COM4 flash 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\flash_log.txt" -Encoding UTF8

Write-Host ""
Write-Host "========== PARSING FLASH LOG ==========" -ForegroundColor Cyan
Write-Host ""

.\scripts\parse_logs.bat claudetransferv2\flash_log.txt flash

Write-Host ""
Write-Host "========== DONE - Power cycle device, then run monitor.ps1 ==========" -ForegroundColor Green
Write-Host ""
