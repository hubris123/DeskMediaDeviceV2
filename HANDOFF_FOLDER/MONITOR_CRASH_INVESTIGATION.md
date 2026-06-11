# Monitor/Warm-Reset Crash Investigation Plan
**Created:** June 10, 2026 | **For:** Next session (Claude Code)
**Goal:** Find out why the device "crashes" when monitor runs with flash, and why warm resets in general need a power cycle.

---

## The Key Realization

**Nobody has ever captured what the failure actually looks like.** The
"never run monitor" rule meant the one tool that could show us the crash
was the thing being avoided. The first job of the next session is simply
to LOOK at the output when it happens.

Second realization: per SESSION_SUMMARY_2026-06-05, **factory reset
(`esp_restart()`) also requires a manual power cycle** — and that involves
zero USB/DTR/RTS activity. So warm resets are broken in general. Whatever
breaks on `flash monitor` may be the same thing that breaks on `esp_restart`.

---

## What Was Already Ruled Out (June 10 session, code inspection)

- **ESP32-C6 reset polarity/ordering:** esp-hosted's `reset_slave()`
  (managed_components/espressif__esp_hosted/host/drivers/transport/transport_drv.c)
  pulses GPIO54 → C6 CHIP_PU (verified against schematic: GPIO54 → R34 0R →
  C6_CHIP_PU, with 1uF cap C10). Sequence: high 50ms → low 50ms → high.
  Correct for a CHIP_PU pin, and it runs from `transport_drv_reconfigure()`
  BEFORE the SDIO probe (`sdio_read_task` waits for `is_transport_rx_ready()`
  which is set right after `reset_slave()`). So the C6 *should* get a clean
  reset on every boot, warm or cold.
- **LCD soft-reset blank screen:** already fixed in Session 3 (GPIO27 hard
  reset pulse at top of app_main) — present in current main.c.

## Known Weakness Found (not yet proven to be THE cause)

In `sdio_read_task` (sdio_drv.c ~line 775): if `sdio card init failed`,
the task **returns and never retries**. WiFi stays dead for that entire
boot with only a single error line. If warm-reset boots intermittently
fail SDIO probe, this makes it unrecoverable without a power cycle.

---

## Ranked Suspects

### Suspect A — idf_monitor's connect-reset (DTR/RTS) glitches BOOT pin
`idf.py monitor` WITHOUT `--no-reset` deliberately toggles DTR/RTS to reset
the chip when it connects — even in combined `flash monitor` this is a
SECOND reset right after esptool's post-flash hard reset. On Windows the
port close/reopen between esptool and monitor can glitch the lines so the
P4 resets into **ROM download mode** — which looks exactly like a crash:
black screen, unresponsive, only a power cycle exits it.
Fits the evidence: `monitor --no-reset` is documented safe, PuTTY is safe
(opens both lines together, the two-transistor circuit treats that as no-op).

### Suspect B — SD card wedges on warm reset
An SD card interrupted mid-transaction by a reset can hold the bus and has
NO reset line — only power loss recovers it. Would explain factory-reset
needing power cycle. Current code only logs "Failed to mount SD card" and
continues, but a wedged card can also hang the SDMMC probe inside
`bsp_sdcard_mount()` → boot appears dead.

### Suspect C — C6/SDIO stale state despite reset
Possible race: C6 needs time after CHIP_PU pulse to boot its slave firmware
before host probes. Or interrupt line stuck. Less likely after code review,
but the no-retry weakness above amplifies any intermittent failure.

### Suspect D — genuine firmware panic on the second reset
Would show a backtrace the moment output is finally captured.

---

## Experiments (run in order, capture EVERYTHING)

### Experiment 1 — capture the actual failure
```powershell
idf.py -p COM4 flash monitor 2>&1 | Out-File claudetransferv2\crash_capture.txt -Encoding UTF8
```
Let it "crash", save the full output. Read the ROM boot line:
- `ESP-ROM:...` then `boot:0x...` — the boot-mode hex digits tell you if it
  entered DOWNLOAD mode (Suspect A confirmed) vs normal SPI boot.
- "waiting for download" → Suspect A confirmed.
- Panic + backtrace → Suspect D; decode with
  `xtensa/riscv32-esp-elf-addr2line -e build/lvgl_demo_v9.elf <addrs>`.
- Boot log that stalls at a specific init line → note WHICH line:
  - stalls around `sdmmc`/SD mount → Suspect B
  - stalls around esp_hosted/SDIO/`Attempt connection with slave` → Suspect C
- Boots fine but screen blank → display path, re-examine LCD reset fix.

### Experiment 2 — isolate USB from warm reset
Attach PuTTY (115200, COM4) — PuTTY does NOT reset the chip. Trigger factory
reset (10s hold on mute checkbox) or any `esp_restart()` path. Watch what the
reboot prints. This is a pure software warm reset with zero DTR/RTS activity.
- Same failure as Exp 1 → warm-reset problem (Suspects B/C), monitor is
  just one trigger among many.
- Boots fine → warm reset is OK; the problem is specific to DTR/RTS
  (Suspect A) and Exp 1's ROM line should already have shown it.

### Experiment 3 — pin down B vs C (if Exp 2 failed)
Remove the SD card, repeat Exp 2.
- Now boots fine → Suspect B (SD card wedge). Fix: tolerate mount failure
  with retry/timeout; document that SD recovery needs power cycle (or add
  card power-cycle circuit in future hardware rev).
- Still fails → Suspect C. Fix candidates: pulse GPIO54 low→high at top of
  app_main (mirror the LCD fix; esp-hosted's constructor runs pre-app_main,
  so may need ESP_SYSTEM_INIT_FN with low priority instead), add delay
  after CHIP_PU release, and patch the sdio_read_task no-retry weakness.

### Fixes to apply once cause is known
- Suspect A: make `monitor --no-reset` the only sanctioned attach (update
  flash.ps1/monitor.ps1 to enforce), keep power-cycle-after-flash, optionally
  test `idf.py -p COM4 flash && idf.py -p COM4 monitor --no-reset` as a
  working one-liner.
- Suspect B: guard `bsp_sdcard_mount()` with timeout/retry, boot UI shows
  "SD unavailable" instead of hanging.
- Suspect C: early C6 reset + sdio retry patch (see above).
- Suspect D: fix the actual bug.

## Hardware reference
- C6 CHIP_PU: P4 GPIO54 → R34(0R) → C6_CHIP_PU, 1uF (C10) to GND.
- C6 IO2 strap: P4 GPIO6 → R33(0R) → C6_IO2.
- sdkconfig: `CONFIG_ESP_HOSTED_SDIO_GPIO_RESET_SLAVE=54`,
  `CONFIG_ESP_HOSTED_SDIO_RESET_ACTIVE_HIGH=y` (correct for CHIP_PU wiring).
- SDIO pins: CMD=19 CLK=18 D0=14 D1=15 D2=16 D3=17, 40MHz, 4-bit.
