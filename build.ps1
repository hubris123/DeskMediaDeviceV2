# ==============================================================================
# Build + Parse Logs - Compile and analyze build output
# ==============================================================================
# Runs idf.py build with dual output (terminal + log file), then parses
# the build log for errors and warnings.
#
# Usage (from project root in PowerShell):
#   .\build.ps1
# ==============================================================================

Write-Host ""
Write-Host "========== BUILD ==========" -ForegroundColor Cyan
Write-Host ""

idf.py build 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\build_log.txt" -Encoding UTF8

Write-Host ""
Write-Host "========== PARSING BUILD LOG ==========" -ForegroundColor Cyan
Write-Host ""

.\scripts\parse_logs.bat claudetransferv2\build_log.txt build

Write-Host ""
Write-Host "========== DONE ==========" -ForegroundColor Green
Write-Host ""
