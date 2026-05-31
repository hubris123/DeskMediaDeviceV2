@echo off
REM ============================================================================
REM Preflight Checklist — Verify build readiness before compilation
REM ============================================================================
REM Purpose:
REM   Verify all critical settings and files before building.
REM   Catches configuration errors early, prevents wasted compile time.
REM
REM Checks:
REM   - sdkconfig: CONFIG_SPIRAM=y, CONFIG_BSP_ERROR_CHECK=n
REM   - CMakeLists.txt: main.c is in SRCS
REM   - Required tools exist: parse_logs.bat, filter_monitor.bat, etc.
REM   - Backup directories exist
REM   - Log files cleanable
REM
REM Usage:
REM   Before every build:
REM     .\preflight_check.bat
REM
REM   Will either:
REM   - Report all checks passed, ready to build
REM   - Report specific failures and how to fix them
REM
REM Output:
REM   Console output + appends to: <project>/claudetransferv2/PREFLIGHT_CHECK.txt
REM
REM ============================================================================

setlocal enabledelayedexpansion

REM ============================================================================
REM CONFIGURATION
REM ============================================================================
set BASE_DIR=C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2
set PROJECT_ROOT=C:\Users\Administrator\Documents\DeskMediaDeviceV2

REM Generate timestamp
for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c%%a%%b)
for /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set mytime=%%a%%b)
set TIMESTAMP=%mydate%_%mytime%

REM Output file
set CHECK_FILE=%BASE_DIR%\PREFLIGHT_CHECK.txt
set CHECK_COUNT=0
set PASS_COUNT=0
set FAIL_COUNT=0

REM Create output directory if needed
if not exist "%BASE_DIR%" mkdir "%BASE_DIR%"

REM ============================================================================
REM PREFLIGHT CHECKS
REM ============================================================================

echo.
echo ========== PREFLIGHT CHECKLIST ==========
echo.

(
    echo.
    echo [%TIMESTAMP%] ========== PREFLIGHT CHECK ==========
) >> "%CHECK_FILE%"

REM NOTE: sdkconfig is a binary file on ESP-IDF v5.x — findstr fails silently.
REM       We use Python (bundled with ESP-IDF) to read it as text instead.
REM       Python is at: C:\Espressif\tools\python\v5.5.4\venv\Scripts\python.exe

set PYTHON=C:\Espressif\tools\python\v5.5.4\venv\Scripts\python.exe

REM --- CHECK 1: CONFIG_SPIRAM in sdkconfig ---
set /a CHECK_COUNT+=1
echo [CHECK %CHECK_COUNT%] CONFIG_SPIRAM in sdkconfig...

if not exist "%PROJECT_ROOT%\sdkconfig" (
    echo   [FAIL] sdkconfig not found at: %PROJECT_ROOT%\sdkconfig
    echo [FAIL] Check %CHECK_COUNT%: sdkconfig not found >> "%CHECK_FILE%"
    set /a FAIL_COUNT+=1
) else (
    "%PYTHON%" -c "import sys; data=open(r'%PROJECT_ROOT%\sdkconfig','rb').read().decode('utf-8','ignore'); sys.exit(0 if 'CONFIG_SPIRAM=y' in data else 1)" >nul 2>&1
    if errorlevel 1 (
        echo   [FAIL] CONFIG_SPIRAM not set to y
        echo [FAIL] Check %CHECK_COUNT%: CONFIG_SPIRAM not y >> "%CHECK_FILE%"
        echo   FIX: Run idf.py menuconfig, enable SPIRAM
        set /a FAIL_COUNT+=1
    ) else (
        echo   [OK] CONFIG_SPIRAM=y
        echo [PASS] Check %CHECK_COUNT%: CONFIG_SPIRAM=y >> "%CHECK_FILE%"
        set /a PASS_COUNT+=1
    )
)

REM --- CHECK 2: CONFIG_BSP_ERROR_CHECK=n in sdkconfig ---
set /a CHECK_COUNT+=1
echo [CHECK %CHECK_COUNT%] CONFIG_BSP_ERROR_CHECK in sdkconfig...

"%PYTHON%" -c "import sys; data=open(r'%PROJECT_ROOT%\sdkconfig','rb').read().decode('utf-8','ignore'); sys.exit(0 if 'CONFIG_BSP_ERROR_CHECK=n' in data else 1)" >nul 2>&1
if errorlevel 1 (
    echo   [FAIL] CONFIG_BSP_ERROR_CHECK not set to n - GT911 will crash app
    echo [FAIL] Check %CHECK_COUNT%: CONFIG_BSP_ERROR_CHECK not n >> "%CHECK_FILE%"
    echo   FIX: Run idf.py menuconfig, disable BSP Error Check
    set /a FAIL_COUNT+=1
) else (
    echo   [OK] CONFIG_BSP_ERROR_CHECK=n
    echo [PASS] Check %CHECK_COUNT%: CONFIG_BSP_ERROR_CHECK=n >> "%CHECK_FILE%"
    set /a PASS_COUNT+=1
)

REM --- CHECK 3: main.c in CMakeLists.txt ---
set /a CHECK_COUNT+=1
echo [CHECK %CHECK_COUNT%] main.c in CMakeLists.txt SRCS...

findstr /I "main.c" "%PROJECT_ROOT%\main\CMakeLists.txt" >nul 2>&1
if errorlevel 1 (
    echo   [FAIL] main.c not in main/CMakeLists.txt
    echo [FAIL] Check %CHECK_COUNT%: main.c not in CMakeLists >> "%CHECK_FILE%"
    echo   FIX: Edit main/CMakeLists.txt, add main.c to SRCS
    set /a FAIL_COUNT+=1
) else (
    echo   [OK] main.c in CMakeLists.txt
    echo [PASS] Check %CHECK_COUNT%: main.c in CMakeLists >> "%CHECK_FILE%"
    set /a PASS_COUNT+=1
)

REM --- CHECK 4: parse_logs.bat exists ---
set /a CHECK_COUNT+=1
echo [CHECK %CHECK_COUNT%] parse_logs.bat exists...

if not exist "%PROJECT_ROOT%\parse_logs.bat" (
    echo   [WARN] parse_logs.bat not found - optional but recommended
    echo [WARN] Check %CHECK_COUNT%: parse_logs.bat missing >> "%CHECK_FILE%"
) else (
    echo   [OK] parse_logs.bat exists
    echo [PASS] Check %CHECK_COUNT%: parse_logs.bat found >> "%CHECK_FILE%"
    set /a PASS_COUNT+=1
)

REM --- CHECK 5: filter_monitor.bat exists ---
set /a CHECK_COUNT+=1
echo [CHECK %CHECK_COUNT%] filter_monitor.bat exists...

if not exist "%PROJECT_ROOT%\filter_monitor.bat" (
    echo   [WARN] filter_monitor.bat not found - optional but recommended
    echo [WARN] Check %CHECK_COUNT%: filter_monitor.bat missing >> "%CHECK_FILE%"
) else (
    echo   [OK] filter_monitor.bat exists
    echo [PASS] Check %CHECK_COUNT%: filter_monitor.bat found >> "%CHECK_FILE%"
    set /a PASS_COUNT+=1
)

REM --- CHECK 6: summarize_diff.bat exists ---
set /a CHECK_COUNT+=1
echo [CHECK %CHECK_COUNT%] summarize_diff.bat exists...

if not exist "%PROJECT_ROOT%\summarize_diff.bat" (
    echo   [WARN] summarize_diff.bat not found - optional but recommended
    echo [WARN] Check %CHECK_COUNT%: summarize_diff.bat missing >> "%CHECK_FILE%"
) else (
    echo   [OK] summarize_diff.bat exists
    echo [PASS] Check %CHECK_COUNT%: summarize_diff.bat found >> "%CHECK_FILE%"
    set /a PASS_COUNT+=1
)

REM --- CHECK 7: Backup directory exists ---
set /a CHECK_COUNT+=1
echo [CHECK %CHECK_COUNT%] Backup directory exists...

if not exist "%BASE_DIR%\backup" (
    echo   [INFO] Creating backup directory...
    mkdir "%BASE_DIR%\backup"
    echo   [OK] Backup directory created
    echo [PASS] Check %CHECK_COUNT%: backup dir created >> "%CHECK_FILE%"
    set /a PASS_COUNT+=1
) else (
    echo   [OK] Backup directory exists
    echo [PASS] Check %CHECK_COUNT%: backup dir exists >> "%CHECK_FILE%"
    set /a PASS_COUNT+=1
)

REM --- CHECK 8: CONSOLE_PATTERNS_REGISTRY exists ---
set /a CHECK_COUNT+=1
echo [CHECK %CHECK_COUNT%] CONSOLE_PATTERNS_REGISTRY.md exists...

if not exist "%PROJECT_ROOT%\CONSOLE_PATTERNS_REGISTRY.md" (
    echo   [WARN] CONSOLE_PATTERNS_REGISTRY.md not found - optional but useful
    echo [WARN] Check %CHECK_COUNT%: registry missing >> "%CHECK_FILE%"
) else (
    echo   [OK] CONSOLE_PATTERNS_REGISTRY.md exists
    echo [PASS] Check %CHECK_COUNT%: registry found >> "%CHECK_FILE%"
    set /a PASS_COUNT+=1
)

REM ============================================================================
REM SUMMARY
REM ============================================================================

echo.
echo [SU