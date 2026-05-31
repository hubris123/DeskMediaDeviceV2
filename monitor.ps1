# ==============================================================================
# Monitor + Filter Logs - Capture and analyze boot output
# ==============================================================================
# Runs idf.py monitor with --no-reset (device already booted from power cycle)
# with dual output (terminal + log file), then filters the monitor log for
# critical boot messages.
#
# Prerequisites: Device must be power-cycled BEFORE running this script
# (unplug/replug or press RST button after flashing).
#
# Usage (from project root in PowerShell):
#   .\monitor.ps1
# ==============================================================================

Write-Host ""
Write-Host "========== MONITOR ==========" -ForegroundColor Cyan
Write-Host ""

idf.py -p COM4 monitor --no-reset 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\monitor_log.txt" -Encoding UTF8

Write-Host ""
Write-Host "========== FILTERING MONITOR LOG ==========" -ForegroundColor Cyan
Write-Host ""

.\scripts\filter_monitor.bat claudetransferv2\monitor_log.txt

Write-Host ""
Write-Host "========== DONE ==========" -ForegroundColor Green
Write-Host ""
