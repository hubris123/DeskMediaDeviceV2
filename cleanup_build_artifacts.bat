@echo off
REM ============================================================================
REM Build Artifact Cleanup — Remove noisy object file listings from build logs
REM ============================================================================
REM Purpose:
REM   Build logs include massive object file (.o, .a) listings that are noise.
REM   This script removes them while keeping actual errors and warnings.
REM
REM Removes:
REM   - Object/archive file listings (xxx.o, xxx.a paths)
REM   - Verbose dependency trees
REM   - Linker map sections
REM   - Unused symbol warnings in linked objects
REM
REM Keeps:
REM   - Actual compilation errors
REM   - Final linking results
REM   - Binary size summaries
REM   - Real warnings we care about
REM
REM Usage:
REM   .\cleanup_build_artifacts.bat build_log.txt
REM
REM Output:
REM   Creates: <project>/claudetransferv2/BUILD_CLEANED.txt (appended)
REM
REM ============================================================================

setlocal enabledelayedexpansion

REM ============================================================================
REM CONFIGURATION
REM ============================================================================
set BASE_DIR=C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2

REM Input log file (passed as parameter)
set INPUT_LOG=%1

REM Generate timestamp
for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c%%a%%b)
for /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set mytime=%%a%%b)
set TIMESTAMP=%mydate%_%mytime%

REM Output file
set OUTPUT_FILE=%BASE_DIR%\BUILD_CLEANED.txt
set TEMP_CLEANED=%BASE_DIR%\.temp_build_cleaned.txt

REM Create output directory if needed
if not exist "%BASE_DIR%" mkdir "%BASE_DIR%"

REM Validate input
if "%INPUT_LOG%"=="" (
    echo.
    echo ERROR: No input log file specified
    echo Usage: cleanup_build_artifacts.bat ^<log_file^>
    echo Example: cleanup_build_artifacts.bat build_log.txt
    echo.
    exit /b 1
)

if not exist "%INPUT_LOG%" (
    echo [%TIMESTAMP%] ERROR: Input log file not found: %INPUT_LOG% >> "%OUTPUT_FILE%"
    exit /b 1
)

REM ============================================================================
REM STEP 1: Copy entire log, then remove noise
REM ============================================================================
(
    echo.
    echo [%TIMESTAMP%] ========== BUILD CLEANED ==========
    echo [INPUT] %INPUT_LOG%
    echo.
) > "%TEMP_CLEANED%"

REM Copy all lines EXCEPT noisy ones
setlocal enabledelayedexpansion
for /f "usebackq delims=" %%A in ("%INPUT_LOG%") do (
    set "line=%%A"

    REM Skip lines that are pure object/archive file listings
    if not "!line:~0,1!"="." (
        REM Filter out specific noise patterns

        REM Skip pure object file paths (ending in .o: or .a:)
        if not "!line:~-3!"=".o:" (
            if not "!line:~-3!"=".a:" (
                REM Skip verbose dependency trees (lines of just spaces + filenames)
                if not "!line!" == "!line:                                    =!" (
                    REM Skip linker map sections
                    if not "!line:*Memory Configuration*=!" == "!line!" (
                        if not "!line:*Linker script and memory map*=!" == "!line!" (
                            REM Skip unused symbol warnings in linked objects
                            if not "!line:*undefined reference*=!" == "!line!" (
                                if not "!line:*(text+0x*=!" == "!line!" (
                                    echo !line! >> "%TEMP_CLEANED%"
                                )
                            )
                        )
                    )
                )
            )
        )
    )
)

REM ============================================================================
REM STEP 2: Append cleaned output
REM ============================================================================
type "%TEMP_CLEANED%" >> "%OUTPUT_FILE%"

REM Add summary
(
    echo.
    echo [SUMMARY]
    echo  - Cleaned and appended to main build log
    echo  - Timestamp: %TIMESTAMP%
    echo.
) >> "%OUTPUT_FILE%"

REM ============================================================================
REM CLEANUP
REM ============================================================================
if exist "%TEMP_CLEANED%" del "%TEMP_CLEANED%"

REM ============================================================================
REM REPORT
REM ============================================================================
echo.
echo [SUCCESS] Build artifacts cleaned and appended to:
echo   %OUTPUT_FILE%
echo.

exit /b 0
