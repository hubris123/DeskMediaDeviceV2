# ==============================================================================
# parse_logs.ps1 - Extract errors and warnings from build/flash/monitor logs
# ==============================================================================
# Usage:
#   .\parse_logs.ps1 <log_file> <command_type>
#   .\parse_logs.ps1 claudetransferv2\build_log.txt build
#   .\parse_logs.ps1 claudetransferv2\monitor_log.txt monitor
#   .\parse_logs.ps1 claudetransferv2\git_log.txt git
#
# Output files in claudetransferv2/:
#   ERRORS_critical.txt    - stop and fix these
#   WARNINGS_safe.txt      - usually ignorable
# ==============================================================================

param(
    [Parameter(Mandatory=$true)][string]$LogFile,
    [Parameter(Mandatory=$true)][string]$CommandType
)

$BASE_DIR = "C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2"
$TIMESTAMP = Get-Date -Format "yyyy-MM-dd HH:mm:ss"

$ErrorFile   = "$BASE_DIR\ERRORS_critical.txt"
$WarningFile = "$BASE_DIR\WARNINGS_safe.txt"

if (-not (Test-Path $BASE_DIR)) { New-Item -ItemType Directory -Path $BASE_DIR | Out-Null }
if (-not (Test-Path $LogFile))  { Write-Host "[ERROR] Log file not found: $LogFile"; exit 1 }

# Read as text, stripping any binary/null chars
$lines = Get-Content $LogFile -ErrorAction SilentlyContinue | Where-Object { $_ -ne $null }

Write-Host ""
Write-Host "Parsing: $LogFile ($($lines.Count) lines)" -ForegroundColor Cyan

# ── Error patterns (stop and fix) ────────────────────────────────────────────

$errorPatterns = @(
    'error:',
    'undefined reference',
    'ld returned 1',
    'multiple definition',
    'undeclared identifier',
    'ESP_FAIL',
    'ESP_ERR_NO_MEM',
    'ESP_ERR_INVALID',
    'bsp_audio_init failed',
    'esp_codec_dev_open failed',
    'bsp_sdcard_mount failed',
    'allocation failed',
    'out of memory',
    'i2s_channel_enable failed'
)

# I2C errors only if NOT GT911 (expected hardware issue)
$i2cLines = $lines | Where-Object { $_ -match 'i2c transaction failed' -and
                                      $_ -notmatch 'GT911|0x14|0x5d|lcd_panel.io.i2c' }

$errorLines = @()
foreach ($pat in $errorPatterns) {
    $errorLines += $lines | Where-Object { $_ -match [regex]::Escape($pat) } |
                   Select-Object @{N='Line';E={$_}}
}
$errorLines += $i2cLines | Select-Object @{N='Line';E={$_}}

# ── Warning patterns (safe to skip) ──────────────────────────────────────────

$warnPatterns = @(
    'unused variable',
    'unused function',
    'implicit function declaration',
    'deprecated',
    'format string',
    'comparison always'
)

# GT911 is always a warning (known hardware issue)
$gt911Lines = $lines | Where-Object { $_ -match 'GT911|touch_gt911' }

$warnLines = @()
foreach ($pat in $warnPatterns) {
    $warnLines += $lines | Where-Object { $_ -match [regex]::Escape($pat) } |
                  Select-Object @{N='Line';E={$_}}
}
$warnLines += $gt911Lines | Select-Object @{N='Line';E={$_}}

# ── Write output ──────────────────────────────────────────────────────────────

$errorHeader = "`n[$TIMESTAMP] === $CommandType - ERRORS (fix these) ===`n"
$errorHeader | Out-File -FilePath $ErrorFile -Append -Encoding UTF8
if ($errorLines.Count -gt 0) {
    $errorLines.Line | Out-File -FilePath $ErrorFile -Append -Encoding UTF8
} else {
    "  (no errors found)" | Out-File -FilePath $ErrorFile -Append -Encoding UTF8
}

$warnHeader = "`n[$TIMESTAMP] === $CommandType - WARNINGS (usually safe) ===`n"
$warnHeader | Out-File -FilePath $WarningFile -Append -Encoding UTF8
if ($warnLines.Count -gt 0) {
    $warnLines.Line | Select-Object -Unique | Out-File -FilePath $WarningFile -Append -Encoding UTF8
} else {
    "  (no warnings found)" | Out-File -FilePath $WarningFile -Append -Encoding UTF8
}

# ── Summary ───────────────────────────────────────────────────────────────────

Write-Host ""
if ($errorLines.Count -gt 0) {
    Write-Host "  ERRORS found: $($errorLines.Count) lines" -ForegroundColor Red
    Write-Host "  -> See: claudetransferv2\ERRORS_critical.txt" -ForegroundColor Red
} else {
    Write-Host "  No errors found" -ForegroundColor Green
}
Write-Host "  Warnings: $($warnLines.Count) lines -> claudetransferv2\WARNINGS_safe.txt" -ForegroundColor Yellow
Write-Host ""
