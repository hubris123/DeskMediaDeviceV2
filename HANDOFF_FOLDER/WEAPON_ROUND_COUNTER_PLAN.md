# Weapon Round Counter — Design Plan
## Hardware: Waveshare ESP32-S3-Touch-LCD-1.69 + INMP441 I2S Mic

---

## Concept

A small weapon-mounted device that counts rounds remaining by detecting
the dual signature of each shot — the recoil impulse on the IMU and the
muzzle blast on the microphone. Both sensors must trigger within a tight
time window for a shot to be counted. Neither alone is sufficient.

The user sets the starting round count, fires, and the display counts
down. At zero it enters reload-wait mode. First shot after reload resets
to full and resumes counting. A manual reload button on screen handles
tactical reloads before empty.

---

## Hardware

### Primary Board — Waveshare ESP32-S3-Touch-LCD-1.69 ($21.99)
- ESP32-S3R8 dual core 240MHz, 8MB PSRAM, 16MB Flash
- 1.69" 240×280 touch LCD
- QMI8658 6-axis IMU (3-axis accel + 3-axis gyro) — onboard
- MX1.25 LiPo battery header — onboard
- LiPo charge management IC — onboard
- WiFi + Bluetooth (not needed for core function, can be disabled)

### Added Component — INMP441 I2S MEMS Microphone (~$3-5)
- I2S digital output — clean signal, no ADC noise
- 3 GPIO pins: SCK, WS, SD (available on the 1.69 expansion pads)
- Size: ~13x13mm — mounts on the back or side of the board
- SNR 61dB — more than adequate for detecting muzzle blast
- ESP-IDF has native I2S driver support

### Mounting
- Weapon rail mount, stock mount, or grip mount depending on platform
- 3D printed bracket sized to 38x17mm board footprint
- Orientation matters for IMU — mount axis aligned with recoil direction

---

## Shot Detection — Dual Sensor Signature

### Why Dual Sensor
- Sound alone: loud noise nearby (door slam, another weapon) = false positive
- IMU alone: bumping the weapon, dropping it = false positive
- Both together within 50ms: extremely high confidence it's a shot

### Calibration Mode
User fires 5-10 rounds during calibration. The device records:
- Peak accelerometer magnitude on the recoil axis (X or Y depending on mount)
- Peak gyroscope rate (weapon rotates slightly on recoil)
- Sound amplitude peak from INMP441
- Time delta between sound and IMU peak (typically sound hits mic slightly
  before recoil peak is felt — consistent per weapon)

These values are stored as the weapon's signature profile in NVS.
Each weapon gets its own profile — multiple profiles storable and selectable.

### Detection Algorithm (runtime)
```
Continuously sample IMU at 200Hz and mic at 16kHz

On any IMU spike above threshold:
  Start 50ms window
  If mic amplitude exceeds threshold within window:
    AND combined signature matches stored profile within tolerance:
      Count shot — decrement round counter
      Start 500ms lockout (prevents double-count on same shot)
  End window

If no mic trigger within 50ms:
  Ignore IMU spike (not a shot)
```

### Two-Phase Calibration Protocol

Calibration uses 20 rounds split across two distinct firing phases.
The screen guides the user through each phase with clear prompts —
no technical knowledge required to calibrate correctly.

**Phase 1 — Slow Fire (10 rounds, paced)**
- Screen displays: "CALIBRATING — SLOW FIRE"
- Countdown between each shot: "Fire... 3... 2... 1... Fire..."
- Full settle time between rounds — weapon returns to rest between shots
- Captures the clean baseline signature per shot:
  - Peak accelerometer magnitude on recoil axis
  - Peak gyroscope rotation rate and direction
  - Muzzle blast amplitude and frequency profile from mic
  - Time delta between sound peak and IMU peak (ms)
  - Recoil impulse duration (ms)
- Builds the slow-fire envelope: min/max bounds across all 10 shots
- This is the gold standard reference — the tightest, cleanest signature

**Phase 2 — Rapid Fire (10 rounds, burst)**
- Screen displays: "CALIBRATING — RAPID FIRE — Fire when ready"
- User fires as fast as comfortable — no pacing enforced
- Captures overlapping recoil events where weapon hasn't fully settled:
  - Lower peak magnitudes (shooter absorbing recoil actively)
  - Compressed time deltas between consecutive shots
  - IMU still moving from previous shot when next fires
  - Separation algorithm must isolate individual events within the burst
- Builds the rapid-fire envelope: wider bounds, faster event cadence
- Without this phase, rapid fire shots would be missed — they look
  different from slow fire shots on the IMU

**Combined Signature**
Two envelopes stored per profile. Detection checks:
- Does this event fit inside the slow-fire envelope? → count it
- Does this event fit inside the rapid-fire envelope? → count it
- Fits neither? → ignore (not this weapon, or not a shot at all)

The high-pass filter on IMU data eliminates running/vehicle vibration
(continuous low-frequency) before envelope comparison. Only sharp
impulse events in the correct frequency range reach the detector.

**Calibration Screen Flow**
```
[Welcome] → [Select weapon slot] → [Set round count] →
[Phase 1 intro] → [Slow fire 10 rounds with countdown] →
[Phase 1 complete] → [Phase 2 intro] → [Rapid fire 10 rounds] →
[Phase 2 complete] → [Saving profile...] → [Ready]
```

Total calibration time: approximately 3-5 minutes including setup.

### Sensitivity Tuning
- Threshold stored per profile in NVS
- Adjustable via settings screen — slider for IMU sensitivity and mic
  sensitivity independently
- Re-calibrate any time from the settings menu

---

## Round Counter Logic

### Starting a Session
- User sets round count on screen before first shot
- Preset values: 5, 10, 15, 17, 20, 30 (common magazine sizes)
- Custom entry via touch keyboard for any count

### Counting Down
- Each detected shot decrements counter by 1
- Display updates immediately on detection
- Counter never goes below 0

### At Zero — Reload Wait Mode
- Display shows "RELOAD" prominently
- Device enters low-power wait state
- Watches only for first shot signature
- First shot detected: resets to full count minus 1, resumes countdown
- This handles running a magazine dry cleanly

### Manual Reload Button
- Tap the reload button on screen at any time
- Counter resets to the last set full count immediately
- Handles tactical reloads before empty
- Whichever happens first (button tap or auto-reset at zero) takes effect

### Multiple Magazines
- Round count is set per session — change it any time on screen
- Swapping mag sizes: tap reload, set new count, ready to go
- Detection signature unchanged — only the starting number matters

---

## Power — Primary Design Constraint

**Power is the hardest problem on this device.** A small LiPo on a
weapon-mounted board has severe capacity limits. Every design decision
must consider power first.

### Display Power Strategy
- **Dark UI by default** — black background, only essential elements lit
- On the 1.69 LCD, the backlight is the main power draw — dim it
  aggressively during idle (10-20% brightness)
- On a future AMOLED version: black pixels draw zero power, dark theme
  is essentially free — design for AMOLED from the start even on LCD
- Round counter number: white or bright color on black — maximum contrast,
  minimum lit pixels
- No animations, no gradients, no decorative elements — every lit pixel
  costs power
- Screen dims further after 10 seconds of no touch — tap to restore
- Screen off after 60 seconds (configurable) — IMU and mic still active,
  tap or shot wakes it

### CPU Power Strategy
- WiFi disabled completely — not needed for core function, saves ~80mA
- Bluetooth disabled completely — same reason
- ESP32-S3 in light sleep between IMU polling cycles
- IMU configured for hardware interrupt mode — QMI8658 raises an interrupt
  pin when acceleration exceeds threshold, ESP32 wakes only then
- Not polling continuously — event-driven wake on IMU interrupt
- Mic sampled in bursts only when IMU interrupt fires, not continuously
- CPU clock scaled down to minimum needed when in idle watch mode
  (80MHz is sufficient, no need for 240MHz except during calibration)

### Sleep and Shake-Wake System

Sleep and wake behavior is critical — the device must be invisible when
not needed and instant when it is.

**Sleep triggers (progressive):**
- 10 seconds no touch, no shot → backlight dims to 10%
- 60 seconds no touch, no shot → screen off, CPU light sleep,
  IMU in interrupt-watch mode (~4-6mA)
- 5 minutes no activity → deep sleep, IMU in wake-on-motion mode (<1mA)

**Wake triggers:**
- **Shake-to-wake** — QMI8658 has a hardware any-motion interrupt that
  fires when acceleration exceeds a low threshold on any axis. ESP32
  wakes from deep sleep in ~5ms. Picking up the weapon, chambering a
  round, or any deliberate movement wakes it instantly. No button needed.
- **Touch wake** — tap the screen from light sleep to restore full brightness
- **Shot detected** — a shot during screen-off wakes the display immediately
  to show the updated count, then sleeps again after 10 seconds

**Wake-to-ready time:**
- From light sleep: ~50ms (display on, counter visible)
- From deep sleep: ~300ms (full boot from retained RTC memory — counter
  state saved to RTC RAM so it wakes with the same count it had)

**RTC RAM state preservation:**
Round count, current profile, and session state stored in RTC RAM so
deep sleep does not lose the count. Wakes exactly where it left off.
NVS is only written on deliberate save (profile change, new session)
to protect flash write cycles.

### Power Budget Estimate (target)
| State              | Estimated Draw  |
|--------------------|-----------------|
| Active (shot just fired, display on) | ~40-60mA |
| Idle watch (display dim, IMU interrupt mode) | ~8-12mA |
| Screen off, IMU watching | ~4-6mA |
| Deep sleep (if user powers down) | <1mA |

### Battery Life Target
A 500mAh LiPo (fits the board footprint easily):
- Idle watch mode: ~40-60 hours
- Mixed active/idle (typical use): 8-15 hours per charge
- Charging via USB-C on the board — standard cable, no special charger

### Future AMOLED Upgrade
When this moves to a custom PCB or the 1.8 AMOLED board, the display
power situation improves dramatically. AMOLED black pixels are literally
off — a round counter on a black background draws almost nothing from
the display. Target UI design keeps 80%+ of pixels black at all times.

---

## UI Design — Power Optimized

```
┌─────────────────────┐
│                     │  ← black, no pixels lit
│         17          │  ← large white number, centered
│                     │
│    ████████████░░░  │  ← depletion bar, dark gray empties to black
│                     │
│  [RELOAD]  [SET]    │  ← small dim buttons, bottom
└─────────────────────┘
```

- Background: pure black (#000000)
- Round number: white, large font, centered — maximum readability
- Depletion bar: transitions from green → yellow → red as rounds deplete
  (only the filled portion is lit, emptied portion goes black)
- Buttons: minimal, dim, bottom of screen
- Warning state (last 3 rounds): number turns red, subtle pulse
- Reload state: "RELOAD" in red centered, everything else black

---

## Profiles and NVS Storage

Each weapon profile stored in NVS:
- Profile name (e.g. "Pistol A", "Rifle B")
- IMU threshold and axis weights
- Mic amplitude threshold
- Sound-to-recoil time delta (ms)
- Default round count
- Sensitivity tuning values

Up to 8 profiles storable. Selected from a simple list on screen.

---

## Build Order

1. Wire INMP441 to 1.69 board — confirm I2S audio capture working
2. Read QMI8658 IMU — confirm accel/gyro data streaming
3. Build shot detection test app — log IMU + mic peaks to serial, fire
   weapon, tune thresholds manually
4. Implement calibration mode — auto-capture signature from 5 shots
5. Implement runtime detection with 50ms window and lockout
6. Build the counter UI — dark theme, round number, depletion bar
7. Implement reload button and auto-reset at zero
8. Implement NVS profile storage and profile selector
9. Implement power optimizations — WiFi/BT off, IMU interrupt mode,
   display dim/sleep
10. Mount on weapon, field test, tune sensitivity

---

## Field Testing — Mobile Development Station

Live fire testing at a range is mandatory. No simulation or bench test
replaces real recoil data, real muzzle blast, and real environmental
noise. The calibration algorithm must be validated against actual rounds
fired before the detection logic is considered reliable.

### Mobile Dev Station Setup
- Laptop with ESP-IDF environment (already configured)
- USB-C cable — flash and serial monitor at the range
- The existing build/parse/flash workflow works anywhere:
  ```powershell
  idf.py build
  idf.py -p COM4 flash
  idf.py -p COM4 monitor --no-reset
  ```
- Small powered USB hub if needed
- 12V battery bank or car inverter if no power available at range
- Notebook for logging shot counts, conditions, and observations

### Field Test Protocol
1. Mount device on weapon — confirm orientation and secure attachment
2. Run calibration — both phases, full 20 rounds
3. Fire 20 additional rounds at various rates — verify count accuracy
4. Test with adjacent shooter firing different weapon — verify no
   false positives from nearby shots
5. Test while moving between shooting positions — verify motion rejection
6. Adjust thresholds in firmware if needed, reflash at the range
7. Repeat until detection is reliable across all conditions

### What to Capture at the Range
- Serial monitor output for every shot — log IMU peaks, mic peaks,
  time deltas, detection confidence — raw data to refine the algorithm
- Note any false positives (counted when shouldn't) and false negatives
  (missed shots) — these guide threshold tuning
- Test in different positions: standing, kneeling, prone
- Test slow fire and rapid fire after calibration — both phases must work

The ability to reflash firmware at the range means the algorithm can
be tuned against real data in real time. One range session of iterative
testing is worth weeks of desk development.

## Notes

- The dual-sensor approach is the key reliability feature. Do not ship
  without both sensors confirmed working together.
- Power is not an afterthought — design every feature around it from
  day one. A dead counter at the range is useless.
- The 1.69 LCD board is the development platform. The target production
  form factor is a custom PCB at 35x16mm with AMOLED display, INMP441,
  QMI8658, and LiPo header — all components are available in packages
  small enough to fit that footprint.
- Keep the UI violent and simple — a soldier does not have time to read.
  One big number. One color change. Done.
