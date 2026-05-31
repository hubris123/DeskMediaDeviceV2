# ==============================================================================
# Preflight Check + Fullclean - Combined build preparation
# ==============================================================================
# Runs preflight_check.bat, shows output in terminal AND saves to log file.
# If preflight passes, runs idf.py fullclean with the same dual output.
#
# Usage (from project root in PowerShell):
#   .\preflight_and_clean.ps1
# ==============================================================================

Write-Host ""
Write-Host "========== STEP 1: PREFLIGHT CHECK ==========" -ForegroundColor Cyan
Write-Host ""

# Run preflight - show in terminal AND save to log
.\scripts\preflight_check.bat 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\preflight_log.txt" -Encoding UTF8

Write-Host ""
Write-Host "--- Preflight log saved to claudetransferv2\preflight_log.txt ---" -ForegroundColor Gray
Write-Host ""

# Check preflight result
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "*** PREFLIGHT FAILED - fullclean aborted. Fix the issues above first. ***" -ForegroundColor Red
    Write-Host ""
    exit 1
}

Write-Host ""
Write-Host "========== STEP 2: FULLCLEAN ==========" -ForegroundColor Cyan
Write-Host ""

# Run fullclean - show in terminal AND save to log
idf.py fullclean 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\clean_log.txt" -Encoding UTF8

Write-Host ""
Write-Host "========== DONE - Ready to build ==========" -ForegroundColor Green
Write-Host ""
