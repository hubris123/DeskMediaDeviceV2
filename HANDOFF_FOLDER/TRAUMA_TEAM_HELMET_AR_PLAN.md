# Trauma Team Helmet — AR System Design Plan
## Reference: https://cyberpunk.fandom.com/wiki/Trauma_Team

---

## Display Hardware — Video Glasses

### VISIONhmd Bigeyes H3 — Recommended (~$139 each)
- **Amazon:** https://www.amazon.com/dp/B0828K335J
- HDMI input — plugs directly into Jetson HDMI out, no adapter needed
- Independent LCD panel per eye — pairs perfectly with dual Jetson setup
- 1280x720 per eye (2.5K equivalent perceived resolution)
- Built-in battery — does not draw from the helmet power system
- Optics built in for close-eye viewing — correct focal distance for
  screens mounted inches from the eye
- Compact enough to mount inside a helmet shell
- DJI FPV drone community confirmed working with live video passthrough
- **Two units total: ~$278** (one per Jetson, one per eye)

### VISIONhmd Bigeyes H3 Pro — Alternative (~$159 each)
- **Amazon:** https://www.amazon.com/dp/B0CGTVXV66
- USB-C input instead of HDMI — useful if HDMI connector clearance is
  tight inside the helmet shell
- Otherwise same specs as H3

---

## Core Concept

A wearable tactical AR helmet inspired by the Trauma Team design from
Cyberpunk. The outside mounts multiple cameras. The inside houses a 1080p
video display (glasses-style). A Jetson Orin Nano processes all video feeds
and composites the HUD overlay in real time before sending to the display
via HDMI.

---

## Dual Jetson Architecture — One Per Eye

The key architectural decision is running **two Nvidia Jetson Orin Nano
modules**, one dedicated to each eye display.

**Why dual Jetson:**
- Each Jetson handles its own 1080p HDMI output independently
- Double the CPU cores, double the GPU compute, double the video bandwidth
- Each unit runs its own camera inputs — no bottleneck sharing feeds
- If one unit has a processing spike the other eye is unaffected
- Allows eye-specific rendering — left eye can run a different overlay
  layout than right eye if needed (e.g. vitals left, targeting right)
- Combined: ~14-20W total draw, still very manageable on a battery pack

**Jetson Orin Nano specs (per unit):**
- 6-core ARM CPU
- 1024-core Ampere GPU
- 8GB LPDDR5 RAM
- Up to 40 TOPS AI performance
- Native HDMI out
- Multiple USB 3.0 ports for camera inputs
- Size: credit card (~70x45mm) — fits easily inside a helmet

**Communication between the two Jetsons:**
- A small Ethernet cable or USB-C link connects them
- One acts as primary (Left), one as secondary (Right)
- Primary handles weapon cam signal distribution and mode state
- When user switches mode, primary broadcasts the new mode to secondary
  so both eyes update simultaneously and stay in sync

---

## Camera Inputs

### 1. Main Passthrough Cameras (x2 — Stereo)
- Mounted flush on the outer visor at eye-level spacing
- USB or CSI connection into their respective Jetson (left cam → left
  Jetson, right cam → right Jetson)
- Provides the base full-screen video the user sees as the world
- Stereo pair gives natural depth perception

### 2. Thermal Camera (FLIR Lepton module)
- Native resolution 160x120 or 320x240, upscaled to fill PIP box
- USB connection into primary (Left) Jetson
- Signal shared to Right Jetson over the inter-unit link if needed
- Shows heat signatures — people, vehicles, hot surfaces
- Eerie green/white palette fits the Trauma Team aesthetic perfectly

### 3. Night Vision Camera
- Low-light IR camera with IR-cut filter removed
- Paired with small IR LED arrays mounted on the helmet rim
- Works in complete darkness, high detail
- USB connection into secondary (Right) Jetson
- Complements thermal — thermal for detection, night vision for detail

### 4. Weapon Camera (Wired)
- Small micro camera mounted on weapon rail or inside a scope housing
- Cable runs along the weapon to the grip
- **Magnetic pogo-pin connector** (4-5 pin) at the shooting hand wrist
  — Apple MagSafe style, drops cleanly when weapon is holstered,
  snaps back instantly when picked up
- Cable routes up the inside of the suit sleeve — the natural bend at
  the inside of the elbow provides a free service loop, zero strain
- Enters the helmet at the collar via a small connector port
- USB video capture dongle into primary (Left) Jetson
- Zero wireless latency — wired all the way through

---

## HUD Layout — Normal Mode

```
┌──────────────────────────────────────────┐
│ [THERMAL]              [VITALS / DATA]   │
│                                          │
│                                          │
│           MAIN PASSTHROUGH               │
│            (full color stereo)           │
│                                          │
│                                          │
│ [WEAPON CAM]           [NIGHT VISION]   │
└──────────────────────────────────────────┘
```

Four PIP boxes in the corners. Main passthrough center.
Each PIP box is independently selectable and promotable to full screen.

---

## Thermal Camera Module — Waveshare Thermal-90 Camera ESP32 Module

**Product:** https://www.waveshare.com/thermal-45-camera-esp32-module.htm
**Price:** $86.99 | **Choose:** 90° FOV version for helmet situational awareness

### Why This Module

- ESP32-S3 onboard — handles all thermal processing itself, zero load
  on either Jetson
- WiFi built in — streams thermal feed over TCP to the Jetson wirelessly,
  no USB cable needed from the thermal camera
- Own battery header — runs off a small LiPo completely isolated from the
  helmet power budget
- 80x62 native resolution — low but standard for this price tier, soft
  upscaling in the compositor turns it into a cinematic heat glow
- Compact size — mounts cleanly behind the visor panel
- ESP-IDF compatible — familiar development environment (same as the
  DeskMediaDevice project)

### Integration

The module boots, joins the helmet's internal WiFi network (or runs as
its own AP), and streams its thermal feed as a TCP video stream. The
primary Jetson receives it over WiFi and pulls it into the compositor
as a video source — fully self-contained, no direct wiring to the Jetson.

---

## Blended Thermal Overlay Mode — "Heat Aura"

Rather than switching between thermal and regular video, the thermal feed
can be composited directly onto the color passthrough as a semi-transparent
additive layer. Anything warm glows subtly on top of the normal color image.
People have a heat aura around their outline. Hot objects glow. Cold
environments stay exactly as the regular camera sees them.

### Compositor Logic (per frame, on Jetson)

```
1. Capture regular camera frame (full color, full resolution)
2. Receive thermal frame (80x62 native)
3. Upscale thermal to match camera resolution with Gaussian blur pass
   — blur turns low-res blocks into soft organic heat glow
4. Colorize thermal (orange/red/white heat palette)
5. Blend: cv2.addWeighted(color_frame, 1.0, thermal_colored, blend, 0)
6. Output blended frame to HDMI
```

### Blend Level Effects

| Blend % | Visual Result                                         |
|---------|-------------------------------------------------------|
| 0%      | Pure regular color video, thermal off                 |
| 15-25%  | Subtle heat aura — people glow slightly, feels natural|
| 40-50%  | Predator vision — heat signatures clearly visible     |
| 75%     | Thermal dominant, color detail fading                 |
| 100%    | Pure thermal replacement mode                         |

### Blend Control

- A variable blend level adjustable via helmet button (cycle through
  presets: 0%, 20%, 50%, 100%)
- Or voice command: "thermal low", "thermal high", "thermal off"
- Default on boot: 20% — subtle enhancement without being distracting
- In darkness the thermal contribution becomes naturally more dominant
  as the regular camera dims, giving automatic low-light compensation

### Why the Low Resolution Helps

The 80x62 thermal resolution, when upscaled and blurred before compositing,
produces a soft organic glow rather than blocky pixels. The blur is not
a workaround — it is the effect. Heat in the real world diffuses and
radiates softly. The blurred upscale mimics that physics and looks
intentional. Higher resolution thermal would actually look less cinematic
at this blend level.

---

## Display Modes

| Mode | Full Screen      | PIP Corners                        |
|------|------------------|------------------------------------|
| 1    | Main passthrough | Thermal, Night vision, Weapon cam  |
| 2    | Weapon cam       | Main, Thermal, Night vision        |
| 3    | Thermal          | Main, Night vision, Weapon cam     |
| 4    | Night vision     | Main, Thermal, Weapon cam          |
| 5    | Split — color L / thermal R | Weapon cam bottom center |
| 6    | Scope only       | All PIP hidden                     |

---

## Mode Switching

**Physical controls:**
- Left helmet button: cycle active PIP selection
- Right helmet button: promote selected PIP to full screen / demote back
- Both held 1 second: reset to Mode 1 default layout

**Voice commands (optional — Whisper tiny or Vosk, runs locally):**
- "Scope full" → weapon cam full screen
- "Thermal" → thermal full screen
- "Reset" → back to Mode 1
- No internet required, processed entirely on the Jetson

---

## Software Stack (per Jetson)

- **OS:** Ubuntu 22.04 (Jetson Linux)
- **Video compositing:** OpenCV + Python or lightweight C++ renderer
- **HUD graphics:** Rendered as transparent RGBA overlays, blitted over
  the camera frame each tick
- **Target framerate:** 1080p @ 30fps minimum, 60fps achievable
- **End-to-end latency target:** under 20ms (threshold where head movement
  feels natural with passthrough video)
- **Inter-Jetson sync:** lightweight UDP socket, mode state broadcast

---

## Power System

| Component              | Draw         |
|------------------------|--------------|
| Jetson Orin Nano (x2)  | ~7-10W each  |
| 1080p glasses display  | ~2-3W        |
| Cameras (x4)           | ~1W total    |
| IR LED arrays          | ~1-2W        |
| **Total**              | ~20-26W      |

A **20,000mAh USB-C PD power bank** at 20V output provides roughly
3-4 hours of runtime. Fits in a belt pack or rear helmet compartment.
A small DC-DC regulator steps voltage down appropriately per component.

---

## Shoulder Pack — Compute and Power Housing

All heavy compute and power hardware lives in a small hard shell pack
mounted high on the upper back between the shoulder blades. This is the
strongest, most balanced carry point on the body. The helmet contains
only the optics, cameras, and wiring — keeping it as light as possible.
Even a few hundred grams of helmet weight feels like several pounds after
an hour of wear. Military helmet designers treat weight as the primary
comfort variable for exactly this reason.

### Pack Contents
- Both Jetson Orin Nano modules
- Main battery / power bank
- DC-DC voltage regulators (per component)
- DJI O3 receiver module
- USB hubs and video capture dongles
- Passive or active cooling for Jetsons (small heatsinks, optional fan)

### Pack Design
- Hard shell — 3D printed or modified small hard case
- Sits above where a backpack would hang — upper back, between shoulder blades
- Secured via chest and waist strap or integrated into the suit back panel
- Dimensions roughly 20x15x8cm — similar to a large hardback book
- Trauma Team red/white accent panels to match the costume

### Helmet Contents (kept minimal)
- Two VISIONhmd Bigeyes H3 goggle units (unavoidable — eyes are in the head)
- Stereo passthrough cameras x2 (tiny, negligible weight)
- Thermal camera module (FLIR Lepton — postage stamp sized)
- Night vision camera (small USB camera, no IR cut filter)
- IR LED arrays around visor rim (negligible)
- Wiring only — zero compute hardware in the helmet

### Target Helmet Weight
Under 500g total including all cameras and goggle units. Comparable to
a standard motorcycle helmet insert. Wearable for extended periods without
neck or shoulder strain.

### Cable Bundle — Pack to Helmet
A single braided multi-conductor cable bundle runs from the pack up the
back of the neck into the helmet base:

- HDMI x2 — Left Jetson → Left goggle, Right Jetson → Right goggle
- USB x4 — All cameras back down to their respective Jetsons
- Power x1 — 5V/12V up to the helmet for cameras and IR LEDs
- Weapon cam x1 — from wrist connector up the sleeve, into the pack

The bundle routes inside the suit collar — invisible externally.
A single multi-pin connector plate at the helmet base allows the helmet
to detach completely in seconds for transport or maintenance.

---

## Helmet Integration

### Reference Design
**Trauma Team Medic Helmet:**
https://cyberpunk.fandom.com/wiki/Trauma_Team?file=CP2077_Trauma_Team_medic_helmet_design.jpg

This design was chosen over the original concept helmet because the wider,
deeper visor area accommodates both Bigeyes H3 goggle units side by side
at the correct interpupillary distance. The broader front face panel also
provides natural mounting positions for cameras and IR LEDs without looking
like afterthought modifications. The deeper helmet cavity keeps all internal
wiring clean and routed without bulging the shell.

The visor front panel is designed as a separate detachable piece — ideal
for housing the cameras as a self-contained unit that snaps on and off
for maintenance and upgrades without disassembling the whole helmet.

### Construction
- Shell: 3D printed (PETG or ASA for durability) or fiberglass layup
- Outer visor: separate snap-on panel, smoked/tinted, hides cameras and
  IR LED arrays behind the tint — looks stock from the outside
- Inner mount: dual Bigeyes H3 units side by side, padded and positioned
  at correct eye level and interpupillary distance
- Cable exit: single multi-pin connector plate at helmet base — full
  detach in seconds
- Wrist connector port: small flush panel at collar/neck entry for weapon cam
- Magnetic wrist connector ring: glows red when connected (Trauma Team
  colors), goes dark on disconnect — LED tied to connection detect pin
- No compute hardware inside the helmet — all processing in shoulder pack

---

## Build Order (when ready)

1. Source both Jetson Orin Nano developer kits
2. Source 1080p video glasses and verify HDMI input works with Jetson output
3. Set up one Jetson with a single USB camera — get passthrough working first
4. Add thermal and night vision feeds, build the compositor
5. Add weapon cam via capture dongle, test wired path
6. Add mode switching logic and inter-Jetson sync
7. Add voice command layer (optional)
8. Design and print helmet shell around the confirmed working electronics
9. Integrate magnetic wrist connector and cable routing through suit
10. Final calibration — stereo alignment, latency testing, HUD positioning

---

## Notes

- The dual Jetson approach is the correct call for this build. A single unit
  could work but would be running at its limit. Two units gives headroom for
  future features (AI object detection, face recognition, scan overlays)
  without a hardware swap.
- The FLIR Lepton is the most cost-effective thermal module for embedded use.
  Lepton 3.5 gives 160x120 at ~$200. Higher res thermal (320x240) modules
  exist but cost $500+.
- Magnetic pogo-pin connectors (4-5 pin) are available on AliExpress for
  under $10 for a pair. Specify coaxial or shielded if carrying video signal.
- This system is modular — each camera feed is independent and can be
  upgraded, swapped, or removed without affecting the others.

---

## Drone Integration — Field Situational Awareness

### Concept

A fifth video source fed wirelessly from a drone into the helmet HUD.
The operator gets a live overhead or orbiting view without looking away
from the scene. Combined with the other four feeds this creates a complete
field system — ground level, thermal, night vision, scope, and aerial all
visible simultaneously or on demand.

### Video Link Options

| System          | Latency  | Quality     | Range    | Cost  |
|-----------------|----------|-------------|----------|-------|
| 5.8GHz FPV analog | ~30ms  | 720p        | ~1km     | $30   |
| DJI O3 digital  | ~30ms    | 1080p/4K    | ~10km    | $200  |
| DJI Goggles link| ~20ms    | 1080p       | ~10km    | $300  |

**Recommended:** DJI O3 receiver module. Low latency, excellent range,
1080p quality. The receiver module is small enough to mount inside the
helmet. Outputs via USB into a capture dongle on the secondary (Right)
Jetson — same path as any other camera input.

### Drone Hardware

Any DJI-compatible drone works out of the box with O3. For a field build:
- **DJI Mini 4 Pro** — lightweight, foldable, 4K, long battery, ~$760
- **DJI Avata 2** — FPV style, faster and more agile, ~$600
- **Custom FPV build** — cheapest, most configurable, can carry a FLIR
  thermal payload for dual aerial thermal + video feed

The drone does not need modification — the O3 receiver in the helmet
simply picks up the existing video downlink signal.

### Updated HUD Layout — Full System Mode

```
┌──────────────────────────────────────────────┐
│ [THERMAL]    [DRONE OVERHEAD]   [VITALS/HUD] │
│                                              │
│                                              │
│            MAIN PASSTHROUGH                  │
│             (full color stereo)              │
│                                              │
│                                              │
│ [WEAPON CAM]              [NIGHT VISION]     │
└──────────────────────────────────────────────┘
```

Drone feed occupies the top center PIP — natural position for an overhead
view. All five feeds independently promotable to full screen.

### Drone Control

Two options for operating the drone while wearing the helmet:

**Option A — Autonomous orbit:**
Pre-program the drone to orbit a GPS coordinate or follow the operator
using DJI's ActiveTrack. The operator never touches a controller — the
feed streams passively into the HUD while the drone flies itself.
Most practical for field use where hands are occupied.

**Option B — Helmet-controlled:**
A small thumb controller on the suit forearm or belt. Minimal input
device — joystick for direction, one button for altitude. The operator
glances at the drone PIP and makes coarse adjustments without breaking
focus on the main scene.

### Updated Mode Table

| Mode | Full Screen       | Notable PIPs                          |
|------|-------------------|---------------------------------------|
| 1    | Main passthrough  | Thermal, Drone, Weapon cam, NV        |
| 2    | Weapon cam        | Main, Thermal, Drone, NV              |
| 3    | Thermal           | Main, Drone, Weapon cam, NV           |
| 4    | Night vision      | Main, Thermal, Drone, Weapon cam      |
| 5    | Drone overhead    | Main, Thermal, Weapon cam             |
| 6    | Scope only        | All PIP hidden                        |
| 7    | Split — color L / drone R | Thermal + weapon cam small   |

### Updated Power Budget

| Component              | Draw         |
|------------------------|--------------|
| Jetson Orin Nano (x2)  | ~7-10W each  |
| 1080p glasses display  | ~2-3W        |
| Cameras (x4 + capture) | ~2W total    |
| DJI O3 receiver        | ~3W          |
| IR LED arrays          | ~1-2W        |
| **Total**              | ~23-30W      |

A 20,000mAh USB-C PD bank at 20V still delivers 2.5-3 hours runtime.
Step up to a 30,000mAh pack for full shift endurance.

### What This System Is

With drone integration the helmet becomes a complete one-person field
operations platform:

- **Ground level:** stereo color passthrough — you see the world
- **Thermal:** heat detection through darkness, smoke, cover
- **Night vision:** high detail low-light navigation
- **Scope:** precision aim without raising the weapon to eye level
- **Drone:** overhead positioning, area surveillance, threat detection
  before entering a space

All five feeds on one dual-Jetson system inside a single helmet.
No external screens, no separate operator, no second person needed
to run the drone. A genuine force multiplier for a single operator.
