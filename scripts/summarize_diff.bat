@echo off
REM ============================================================================
REM Git Diff Summarizer — Portable Batch Script
REM ============================================================================
REM Purpose:
REM   Extract high-level summary of git changes instead of full diff output.
REM   Reduces quota waste by showing only:
REM   - Files changed (with +/- line counts)
REM   - Function/method signatures modified
REM   - Brief context (1-2 lines before/after each change)
REM
REM Usage:
REM   Run at the END of a git commit or push:
REM   git commit -m "message" && summarize_diff.bat
REM
REM Output:
REM   Appends to: <project_root>\claudetransferv2\GIT_DIFF_SUMMARY.txt
REM   (Creates if doesn't exist)
REM
REM Portable:
REM   Set BASE_DIR to your project's backup folder (see line 33)
REM   Default: C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2
REM
REM ============================================================================

setlocal enabledelayedexpansion

REM ============================================================================
REM CONFIGURATION — Adjust BASE_DIR for your project
REM ============================================================================
set BASE_DIR=C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2

REM Generate timestamp for logging
for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c%%a%%b)
for /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set mytime=%%a%%b)
set TIMESTAMP=%mydate%_%mytime%

REM Output file
set OUTPUT_FILE=%BASE_DIR%\GIT_DIFF_SUMMARY.txt

REM Temp files for processing
set TEMP_DIFF=%BASE_DIR%\.temp_diff.txt
set TEMP_SUMMARY=%BASE_DIR%\.temp_summary.txt

REM Create output directory if needed
if not exist "%BASE_DIR%" mkdir "%BASE_DIR%"

REM ============================================================================
REM STEP 1: Capture git diff of last commit
REM ============================================================================
git show --format="" > "%TEMP_DIFF%" 2>&1

REM Check if git command succeeded
if errorlevel 1 (
    echo [%TIMESTAMP%] ERROR: git show failed. Is this a git repository? >> "%OUTPUT_FILE%"
    exit /b 1
)

REM ============================================================================
REM STEP 2: Extract commit message
REM ============================================================================
for /f "tokens=*" %%A in ('git log -1 --pretty=format:%%B') do (
    set COMMIT_MSG=%%A
    goto :commit_msg_done
)
:commit_msg_done

REM ============================================================================
REM STEP 3: Extract changed files and line counts
REM ============================================================================
echo. >> "%OUTPUT_FILE%"
echo [%TIMESTAMP%] ========================================== >> "%OUTPUT_FILE%"
echo [COMMIT] %COMMIT_MSG% >> "%OUTPUT_FILE%"
echo. >> "%OUTPUT_FILE%"

REM Use git diff-tree to get file stats
for /f "tokens=1,2,3,4,5" %%A in ('git diff-tree --no-commit-id --name-status -r HEAD') do (
    set CHANGE_TYPE=%%A
    set FILE_NAME=%%B

    REM Get line stats for this file
    for /f "tokens=*" %%X in ('git diff HEAD~1 HEAD --numstat -- "!FILE_NAME!" 2^>nul ^| findstr "!FILE_NAME!"') do (
        echo  [FILE] !FILE_NAME! (type: !CHANGE_TYPE!) >> "%OUTPUT_FILE%"
    )
)

REM Fallback: Use simpler approach with --shortstat
echo. >> "%OUTPUT_FILE%"
echo [FILE SUMMARY]:  >> "%OUTPUT_FILE%"
git diff --shortstat HEAD~1 HEAD >> "%OUTPUT_FILE%" 2>&1

REM ============================================================================
REM STEP 4: Extract function signatures from diff (C/C++/Python focus)
REM ============================================================================
echo. >> "%OUTPUT_FILE%"
echo [FUNCTIONS MODIFIED]: >> "%OUTPUT_FILE%"

REM Search for common function/method signatures in the diff
REM Patterns: ^static, ^void, ^int, ^bool, ^auto, ^def, etc.
findstr /R "^static.*(\|^void.*(\|^int.*(\|^bool.*(\|^auto.*(\|^def " "%TEMP_DIFF%" >> "%OUTPUT_FILE%" 2>nul

REM If no matches, note it
if errorlevel 1 (
    echo  [No function signatures found in diff] >> "%OUTPUT_FILE%"
)

REM ============================================================================
REM STEP 5: Extract context (2 lines before/after changes)
REM ============================================================================
echo. >> "%OUTPUT_FILE%"
echo [KEY CHANGES - Context]: >> "%OUTPUT_FILE%"

REM Extract lines starting with + or - (actual changes)
REM but skip the +++ and --- file markers
findstr /V "^+++\|^---" "%TEMP_DIFF%" | findstr /R "^[\+\-]" | findstr /V "^[\+\-][\+\-][\+\-]" > "%TEMP_SUMMARY%"

REM Count changes and show summary
set CHANGE_COUNT=0
for /f "tokens=*" %%Z in ('type "%TEMP_SUMMARY%"') do (
    set /a CHANGE_COUNT+=1
    if !CHANGE_COUNT! LEQ 20 (
        echo  %%Z >> "%OUTPUT_FILE%"
    )
)

if %CHANGE_COUNT% GTR 20 (
    echo. >> "%OUTPUT_FILE%"
    echo  ... and %CHANGE_COUNT% more lines changed (not shown) >> "%OUTPUT_FILE%"
)

REM ============================================================================
REM STEP 6: Add divider and metadata
REM ============================================================================
echo. >> "%OUTPUT_FILE%"
echo [SUMMARY] >> "%OUTPUT_FILE%"
echo  - Lines changed: %CHANGE_COUNT% >> "%OUTPUT_FILE%"
echo  - Timestamp: %TIMESTAMP% >> "%OUTPUT_FILE%"
echo. >> "%OUTPUT_FILE%"

REM ============================================================================
REM CLEANUP
REM ============================================================================
if exist "%TEMP_DIFF%" del "%TEMP_DIFF%"
if exist "%TEMP_SUMMARY%" del "%TEMP_SUMMARY%"

REM ============================================================================
REM REPORT
REM ============================================================================
echo.
echo [SUCCESS] Diff summary appended to:
echo   %OUTPUT_FILE%
echo.

exit /b 0
