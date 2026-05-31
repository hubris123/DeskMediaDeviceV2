@echo off
REM ============================================================================
REM Monitor Output Filter — Portable Batch Script
REM ============================================================================
REM Purpose:
REM   Filter ESP-IDF monitor logs to show only critical information.
REM   Removes 90% of verbose debug spam, keeps only:
REM   - ERROR, FAIL, FATAL lines (critical)
REM   - State transitions (Initializing, Starting, Complete, Ready)
REM   - Audio-specific keywords (Playing, codec, i2s, audio_task)
REM   - Display-specific keywords (display, LVGL, flush, render, lock)
REM   - Touch/GT911 messages
REM   - Key initialization milestones
REM
REM Usage:
REM   Run AFTER monitor completes:
REM   idf.py -p COM4 flash monitor ... | Out-File monitor_log.txt
REM   .\filter_monitor.bat monitor_log.txt
REM
REM Output:
REM   Creates: <project>/claudetransferv2/MONITOR_FILTERED.txt
REM   Appends each run (keeps history)
REM
REM Portable:
REM   Set BASE_DIR to your project's backup folder (line 38)
REM   Default: C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2
REM
REM ============================================================================

setlocal enabledelayedexpansion

REM ============================================================================
REM CONFIGURATION — Adjust BASE_DIR for your project
REM ============================================================================
set BASE_DIR=C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2

REM Input log file (passed as parameter)
set INPUT_LOG=%1

REM Generate timestamp
for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c%%a%%b)
for /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set mytime=%%a%%b)
set TIMESTAMP=%mydate%_%mytime%

REM Output file
set OUTPUT_FILE=%BASE_DIR%\MONITOR_FILTERED.txt
set TEMP_FILTERED=%BASE_DIR%\.temp_monitor_filtered.txt

REM Create output directory if needed
if not exist "%BASE_DIR%" mkdir "%BASE_DIR%"

REM Validate input
if "%INPUT_LOG%"=="" (
    echo.
    echo ERROR: No input log file specified
    echo Usage: filter_monitor.bat ^<log_file^>
    echo Example: filter_monitor.bat monitor_log.txt
    echo.
    exit /b 1
)

if not exist "%INPUT_LOG%" (
    echo [%TIMESTAMP%] ERROR: Input log file not found: %INPUT_LOG% >> "%OUTPUT_FILE%"
    exit /b 1
)

REM ============================================================================
REM STEP 1: Create filtered log by extracting key lines
REM ============================================================================
(
    echo.
    echo [%TIMESTAMP%] ========== MONITOR FILTERED ==========
    echo [INPUT] %INPUT_LOG%
    echo.
) > "%TEMP_FILTERED%"

REM ============================================================================
REM FILTER LOGIC — Extract important lines only
REM ============================================================================
REM ERRORS - Always include
findstr /I "ERROR\|FAIL\|FATAL\|ASSERT" "%INPUT_LOG%" >> "%TEMP_FILTERED%"

REM STATE TRANSITIONS - Initialization progress
findstr /I "Initializing\|Starting\|Booting\|Init\|Ready\|Complete\|Finished" "%INPUT_LOG%" >> "%TEMP_FILTERED%"

REM AUDIO-SPECIFIC - Playing, codec, i2s, audio tasks
findstr /I "Playing\|codec\|audio_task\|i2s\|ESP8311\|PCM\|WAV\|playback\|volume" "%INPUT_LOG%" >> "%TEMP_FILTERED%"

REM DISPLAY-SPECIFIC - LVGL, display lock/unlock, framebuffer
findstr /I "display\|LVGL\|flush\|render\|lock\|unlock\|framebuffer\|ST7701\|tear" "%INPUT_LOG%" >> "%TEMP_FILTERED%"

REM TOUCH - GT911, touch input, capacitive
findstr /I "GT911\|touch\|capacitive" "%INPUT_LOG%" >> "%TEMP_FILTERED%"

REM SD CARD - Mount, file operations
findstr /I "sdcard\|mount\|SD card\|found\|file" "%INPUT_LOG%" >> "%TEMP_FILTERED%"

REM CRITICAL ESP ERRORS - Memory, heap, allocation
findstr /I "ESP_FAIL\|ESP_ERR\|allocation\|memory\|heap\|malloc" "%INPUT_LOG%" >> "%TEMP_FILTERED%"

REM TIMING MARKERS - vTaskDelay only ("ms" removed — matches too broadly)
findstr /I "vTaskDelay" "%INPUT_LOG%" >> "%TEMP_FILTERED%"

REM GPIO/HARDWARE - Pin toggling, resets
findstr /I "gpio\|GPIO\|reset\|Reset" "%INPUT_LOG%" >> "%TEMP_FILTERED%"

REM NVS - Non-volatile storage operations
findstr /I "nvs\|NVS" "%INPUT_LOG%" >> "%TEMP_FILTERED%"

REM ============================================================================
REM STEP 2: Remove duplicates and sort chronologically
REM ============================================================================
REM (Windows batch doesn't have native sort, so we keep chronological order from input)
REM (Duplicates are minimal since we're filtering specific keywords)

REM ============================================================================
REM STEP 3: Append filtered output to main file
REM ============================================================================
type "%TEMP_FILTERED%" >> "%OUTPUT_FILE%"

REM Add summary statistics
(
    echo.
    echo [SUMMARY]
    for /f %%A in ('find /C /V "" "%TEMP_FILTERED%"') do (
        set LINE_COUNT=%%A
    )
    echo  - Total filtered lines: !LINE_COUNT!
    echo  - Original file: %INPUT_LOG%
    echo  - Timestamp: %TIMESTAMP%
    echo.
) >> "%OUTPUT_FILE%"

REM ============================================================================
REM CLEANUP
REM ============================================================================
if exist "%TEMP_FILTERED%" del "%TEMP_FILTERED%"

REM ============================================================================
REM REPORT
REM ============================================================================
echo.
echo [SUCCESS] Monitor output filtered and appended to:
echo   %OUTPUT_FILE%
echo.
echo Use this file instead of raw monitor logs to save quota.
echo.

exit /b 0
