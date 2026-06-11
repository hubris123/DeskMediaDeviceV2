#!/usr/bin/env python3
"""
Trigger a software reset on ESP32-P4 via LP_WDT register write.
Requires the device to be in ROM bootloader mode (e.g. after idf.py flash stalls).

Usage: python reset_esp32p4.py [COM_PORT]
Runs with the IDF Python env:
  C:\Users\Administrator\.espressif\python_env\idf5.5_py3.12_env\Scripts\python.exe reset_esp32p4.py COM4
"""

import sys
import time
import esptool

PORT = sys.argv[1] if len(sys.argv) > 1 else "COM4"
BAUD = 115200

# ESP32-P4 LP_WDT registers
# DR_REG_LPAON_BASE = 0x50110000, LP_WDT_BASE = LPAON + 0x6000
LP_WDT_BASE    = 0x50116000
WPROTECT_REG   = LP_WDT_BASE + 0x18   # 0x50116018
CONFIG0_REG    = LP_WDT_BASE + 0x0    # 0x50116000  — EN, STG0 action
CONFIG1_REG    = LP_WDT_BASE + 0x4    # 0x50116004  — STG0 hold (timeout ticks)

UNLOCK_KEY = 0x50D83AA1

# CONFIG0: EN=1(bit31), STG0=RESET_SYSTEM=3(bits 30:28), SYS_RESET_LENGTH=1(bits 15:13), PAUSE_IN_SLP=1(bit9)
CONFIG0_VAL = (1 << 31) | (3 << 28) | (1 << 13) | (1 << 9)  # 0xB0002200

# STG0_HOLD: LP oscillator ~150kHz → 1024 ticks ≈ 7ms
STG0_HOLD = 1024

print(f"Connecting to {PORT} (no_reset — device must already be in bootloader mode) ...")
try:
    esp = esptool.detect_chip(PORT, BAUD, connect_mode="no_reset")
    print(f"Chip detected: {esp.CHIP_NAME}")
    print("Uploading stub ...")
    esp = esp.run_stub()
    print("Stub running.")
except Exception as e:
    print(f"Connection failed: {e}")
    print("  Make sure the device is in ROM bootloader (just flashed, not power-cycled).")
    sys.exit(1)

print("Writing LP_WDT registers to trigger RESET_SYSTEM ...")
print(f"  WPROTECT  0x{WPROTECT_REG:08X} <- 0x{UNLOCK_KEY:08X}  (unlock)")
esp.write_reg(WPROTECT_REG, UNLOCK_KEY)

print(f"  CONFIG1   0x{CONFIG1_REG:08X} <- 0x{STG0_HOLD:08X}  (stage0 hold = {STG0_HOLD} ticks)")
esp.write_reg(CONFIG1_REG, STG0_HOLD)

print(f"  CONFIG0   0x{CONFIG0_REG:08X} <- 0x{CONFIG0_VAL:08X}  (EN=1, STG0=RESET_SYSTEM)")
esp.write_reg(CONFIG0_REG, CONFIG0_VAL)

print("Reset triggered — device should boot the application in ~10ms.")
