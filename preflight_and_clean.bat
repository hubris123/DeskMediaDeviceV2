@echo off
REM ============================================================================
REM Preflight Check + Fullclean - Combined build preparation
REM ============================================================================
REM Runs preflight_check.bat to verify build readiness, then waits for user
REM confirmation before proceeding with idf.py fullclean
REM ============================================================================

REM Run preflight and capture output to both console and log file
.\preflight_check.bat 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\preflight_log.txt" -Encoding UTF8
echo.
echo Preflight complete. Press Enter to continue with fullclean...
pause
idf.py fullclean 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\clean_log.txt" -Encoding UTF8
