@echo off
REM ============================================================================
REM Log Parser for DeskMediaDevice Build/Flash/Monitor/Git Commands
REM ============================================================================
REM Usage: parse_logs.bat <log_file> <command_type>
REM Example: parse_logs.bat monitor_log.txt build
REM          parse_logs.bat git_commit_log.txt git
REM
REM Output Files:
REM   - claudetransferv2\ERRORS(Stop/Break).txt (appended, non-git)
REM   - claudetransferv2\WARNINGS_safe.txt (appended, non-git)
REM   - claudetransferv2\ERRORS(Stop/Break)_git.txt (appended, git only)
REM   - claudetransferv2\WARNINGS_safe_git.txt (appended, git only)
REM ============================================================================

setlocal enabledelayedexpansion

REM Parameters
set LOG_FILE=%1
set COMMAND_TYPE=%2
set BASE_DIR=C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2
set TIMESTAMP=%date:~-4%%date:~-10,2%%date:~-7,2%_%time:~0,2%%time:~3,2%%time:~6,2%

REM Create output directory if it doesn't exist
if not exist "%BASE_DIR%" mkdir "%BASE_DIR%"

REM Determine output file names based on command type
if /i "%COMMAND_TYPE%"=="git" (
    set ERROR_FILE=%BASE_DIR%\ERRORS_critical_git.txt
    set WARNING_FILE=%BASE_DIR%\WARNINGS_safe_git.txt
) else (
    set ERROR_FILE=%BASE_DIR%\ERRORS_critical.txt
    set WARNING_FILE=%BASE_DIR%\WARNINGS_safe.txt
)

REM Check if log file exists
if not exist "%LOG_FILE%" (
    echo [%TIMESTAMP%] Log file not found: %LOG_FILE% >> "%ERROR_FILE%"
    exit /b 1
)

REM ============================================================================
REM ERRORS (Stop/Break) - Patterns to search for:
REM ============================================================================
REM - error: (compiler errors)
REM - undefined reference (linker)
REM - ld returned 1 (linker failure)
REM - multiple definition (duplicate symbols)
REM - expected (syntax error)
REM - undeclared identifier (missing variable/function)
REM - ESP_FAIL (critical runtime)
REM - ESP_ERR_NO_MEM (out of memory)
REM - ESP_ERR_INVALID (invalid parameter)
REM - bsp_audio_init failed (audio won't work)
REM - esp_codec_dev_open failed (codec init failed)
REM - bsp_sdcard_mount failed (SD card dead)
REM - bsp_display failed (display won't show)
REM - i2s_channel_enable failed (audio won't work)
REM - allocation failed (heap exhausted)
REM - out of memory (heap exhausted)
REM - i2c transaction failed (only if NOT GT911 @ 0x14 or 0x5d)
REM ============================================================================

echo. >> "%ERROR_FILE%"
echo [%TIMESTAMP%] === %COMMAND_TYPE% Command === >> "%ERROR_FILE%"

REM Search for error patterns
findstr /N /I "error:" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N /I "undefined reference" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N /I "ld returned 1" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N /I "multiple definition" "%LOG_FILE%" >> "%ERROR_FILE%"
REM NOTE: "expected" removed — too broad, matches docs/comments/CMake output
findstr /N /I "undeclared identifier" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N "ESP_FAIL" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N "ESP_ERR_NO_MEM" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N "ESP_ERR_INVALID" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N /I "bsp_audio_init failed" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N /I "esp_codec_dev_open failed" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N /I "bsp_sdcard_mount failed" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N /I "bsp_display" "%LOG_FILE%" | findstr /I "failed" >> "%ERROR_FILE%"
findstr /N /I "i2s_channel_enable failed" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N /I "allocation failed" "%LOG_FILE%" >> "%ERROR_FILE%"
findstr /N /I "out of memory" "%LOG_FILE%" >> "%ERROR_FILE%"
REM I2C errors — exclude GT911 (known hardware issue, always fails safely)
findstr /N /I "i2c transaction failed" "%LOG_FILE%" | findstr /V /I "GT911\|0x14\|0x5d\|lcd_panel.io.i2c" >> "%ERROR_FILE%"

REM ============================================================================
REM WARNINGS (Safe to Skip) - Patterns to search for:
REM ============================================================================
REM - unused variable (cleanup only)
REM - unused function (cleanup only)
REM - implicit function declaration (still compiles/works)
REM - signed/unsigned comparison (type mismatch, minor)
REM - GT911: touch_gt911_read_cfg: GT911 read error (expected—disabled)
REM - i2c transaction failed @ 0x14 (GT911, expected)
REM - deprecated function (still works)
REM - -Wall / -Wextra warnings (style, not functionality)
REM - format string (non-critical)
REM - comparison always true/false (constant expressions)
REM - unused result of function call (non-critical)
REM ============================================================================

echo. >> "%WARNING_FILE%"
echo [%TIMESTAMP%] === %COMMAND_TYPE% Command === >> "%WARNING_FILE%"

REM Search for warning patterns
findstr /N /I "unused variable" "%LOG_FILE%" >> "%WARNING_FILE%"
findstr /N /I "unused function" "%LOG_FILE%" >> "%WARNING_FILE%"
findstr /N /I "implicit function declaration" "%LOG_FILE%" >> "%WARNING_FILE%"
findstr /N /I "signed/unsigned comparison" "%LOG_FILE%" >> "%WARNING_FILE%"
findstr /N /I "GT911" "%LOG_FILE%" >> "%WARNING_FILE%"
findstr /N /I "touch_gt911" "%LOG_FILE%" >> "%WARNING_FILE%"
findstr /N /I "i2c transaction failed" "%LOG_FILE%" >> "%WARNING_FILE%"
findstr /N /I "deprecated" "%LOG_FILE%" >> "%WARNING_FILE%"
findstr /N /I "format s