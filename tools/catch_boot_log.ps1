# catch_boot_log.ps1 — capture boot output after flash without resetting the chip.
# Start this BEFORE running flash_and_reset.ps1 (in a second terminal).
# It polls COM4 until esptool releases it, then opens it instantly with
# DTR/RTS disabled (no reset) and logs everything to boot_log.txt.
# Ctrl+C to stop.

param(
    [string]$Port = "COM4",
    [int]$Baud = 115200,
    [string]$OutFile = "$PSScriptRoot\..\boot_log.txt"
)

$sp = New-Object System.IO.Ports.SerialPort $Port, $Baud
$sp.DtrEnable = $false
$sp.RtsEnable = $false
$sp.ReadTimeout = 200

Write-Host "Waiting for the flash to START on $Port (run flash_and_reset.ps1 now)..."
# Phase 1: wait until esptool has the port (our open attempts fail)
while ($true) {
    try {
        $sp.Open()
        $sp.Close()          # port is still free — flash hasn't started yet
        Start-Sleep -Milliseconds 200
    } catch { break }        # open failed -> esptool owns the port now
}
Write-Host "Flash in progress. Waiting for $Port to be released..."
# Phase 2: grab the port the moment esptool releases it
while ($true) {
    try { $sp.Open(); break } catch { Start-Sleep -Milliseconds 50 }
}
Write-Host "Port opened WITHOUT reset. Logging to $OutFile  (Ctrl+C to stop)"
"" | Out-File $OutFile -Encoding utf8

try {
    while ($true) {
        $chunk = $sp.ReadExisting()
        if ($chunk) {
            Write-Host -NoNewline $chunk
            Add-Content -Path $OutFile -Value $chunk -NoNewline
        }
        Start-Sleep -Milliseconds 50
    }
}
finally {
    $sp.Close()
    Write-Host "`nPort closed. Log saved to $OutFile"
}
