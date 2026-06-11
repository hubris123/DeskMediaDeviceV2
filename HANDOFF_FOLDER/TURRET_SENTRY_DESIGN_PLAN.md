# Mobile Sentry Turret — Design Plan
## Concept: ESP32-Controlled Nerf Dart Turret, Multi-Role Deployment

---

## Vision

A compact, self-contained rotating turret unit that can be deployed by
drone, placed by hand, or dropped into position to guard a location while
operators push forward. The turret is operated by a dedicated gunner via
a separate wireless interface — completely independent of whoever is flying
or moving the platform it sits on.

The system is intentionally human-in-the-loop only. It never acts without
a conscious human decision. This is a hard design requirement, not a
feature to be added later.

If this concept proves viable and the surrounding product ecosystem
succeeds, this system could one day protect lives in real field situations —
that possibility is worth building it right from the start.

---

## Core Design Philosophy

**Task separation:** Pilots fly. Gunners shoot. Doing both simultaneously
under high stress leads to doing neither well. This is a fundamental
principle in real tactical systems. The turret operator has no flight
controls. The pilot has no weapon controls. Two people, two interfaces,
one coordinated platform.

**Human in the loop — always:** The turret only moves on active gunner
input. It only fires on a deliberate conscious button press. No automation,
no motion-triggered response, no autonomous behavior of any kind. A human
makes every decision.

**Modular platform independence:** The turret is a standalone unit. It
does not know or care what it is mounted on — drone, ground vehicle, fixed
post, or carried by hand. Swap the base, the turret is unchanged.

---

## Deployment Modes

### Mode 1 — Aerial / Drone Mounted
- Drone carries the turret to position, hovers or lands
- Turret is stowed flush with the drone body during flight
- On landing or at operator command, launcher deploys upward to firing position
- Pilot focuses entirely on positioning and flight
- Gunner takes control of turret independently via their own interface
- Drone can reposition on command without interrupting turret operation

### Mode 2 — Ground Sentry / Hand Placed
- Operator carries the unit to a chokepoint — doorway, hallway,
  stairwell, corner, entry point
- Sets it down on any reasonably flat surface
- Weighted low base and deployable legs self-stabilize on uneven ground
- Turret deploys upward automatically on power-up
- Operator walks away — unit holds position
- Remote gunner takes over via WiFi, guards the rear while team advances
- Shift rotation: one gunner watches, hands off to the next when needed,
  coverage never drops

### Mode 3 — Drop Deployment
- Drone carries the unit to position and releases it from low altitude
- Small spring-loaded landing legs absorb the drop impact
- Unit powers up on impact, turret deploys, connects to gunner interface
- Gunner receives "ready" confirmation on their screen
- No human needs to physically enter the area to place it

---

## Mechanical Design

### Overall Form
- Cylindrical body — round cross section allows smooth 360° rotation
  of the turret ring
- Size target: roughly the size of a large thermos or wide water bottle
  when fully stowed — one person carries it in one hand or a small bag
- Weight target: under 1kg fully loaded
- Two sections: fixed weighted base + rotating turret ring on top

### Turret Ring
- Rotates 360° continuously (pan axis)
- Driven by a small stepper motor for precise positioning or a continuous
  rotation servo for simplicity
- Rotation centered on the vertical axis of the cylinder — weight stays
  balanced as it pans, does not destabilize a drone or tip on the ground

### Launcher Mount
- Pivot point at the center/middle of the turret ring (tilt axis)
- Allows the launcher to raise from fully stowed (flush with body,
  pointing down) to fully deployed (pointing forward/up)
- Tilt range: approximately -10° (slightly down) to +45° (up)
- Single servo controls the tilt
- Camera mounted on the launcher tilts with it — always points where
  the launcher points

### Stow / Deploy Mechanism
- At rest / in flight: launcher folds down flush with the body,
  self-contained, no protrusions
- On deploy command: tilt servo raises launcher to operational position
- A worm gear or lead screw drive keeps it locked in position without
  the servo holding continuous torque — important for battery life

### Landing Legs
- 3 or 4 small spring-loaded legs folded flat during flight
- Release on ground contact or manual command
- Spread wide enough to prevent tipping during turret rotation
- Absorb low-altitude drop impact (1-2 meter drop target)

### Fire Mechanism
- Small servo arm added to depress the nerf pistol trigger
- Or flywheel motors bypassed and controlled directly via ESP32 GPIO
- Single fire per button press — no burst, no hold-to-fire
- 10-dart capacity (stock nerf pistol magazine)
- Reload requires physical access to the unit

---

## Electronics — ESP32 Control System

### Microcontroller
**ESP32-S3** — dual core, WiFi, Bluetooth, enough GPIO for all control
functions, familiar development environment (ESP-IDF, same as DeskMediaDevice).

### What the ESP32 Controls
- Pan stepper/servo — turret rotation
- Tilt servo — launcher raise/lower
- Fire servo/motor — trigger mechanism
- Deploy servo — stow/deploy the launcher
- Status LEDs — connection indicator, ready state, fire confirmation
- Camera — FPV camera mounted on launcher, streams to gunner

### Communication
- ESP32 runs as a WiFi Access Point
- Gunner's device connects directly to the turret AP
- Commands sent via UDP for low latency (pan/tilt position, fire, deploy)
- Camera feed streamed back via MJPEG or UDP video stream
- No internet required — fully self-contained local link

### Safety Rules (hard coded, not configurable)
1. If WiFi link drops → turret freezes immediately, does nothing
2. Gunner must actively reconnect AND send a "resume" confirmation
   before any control resumes after a dropped link
3. Fire command requires a separate arm signal before the fire button
   is accepted — two deliberate actions to fire, not one
4. Deploy/stow only on explicit command, never automatic
5. No motion detection, no autonomous tracking, no scheduled actions

### Authentication
- Simple pre-shared key on the WiFi AP
- Only authorized devices with the correct credentials can connect
- Prevents accidental or unauthorized connection

---

## Gunner Interface

### Hardware Option A — Dedicated Handheld Controller
- Small handheld unit, fits in one hand
- Thumb joystick: pan left/right, tilt up/down
- Button 1: arm (safety)
- Button 2: fire (only works when armed)
- Button 3: deploy / stow launcher
- Small screen: live turret camera feed + connection status + dart count
- Battery powered, connects to turret WiFi AP

### Hardware Option B — Helmet HUD Integration
- Turret camera feed pipes into the AR helmet as a seventh video source
- Appears as a PIP on the HUD or promotes to full screen
- Wrist-mounted joystick controls pan/tilt
- Glove button or wrist button for arm and fire
- Gunner sees the turret view without looking away from their environment
- Fully integrated into the Trauma Team helmet system

### Shift Rotation
- Active gunner closes their interface, next gunner opens theirs
- Connects to the turret AP, turret accepts the new connection
- No interruption to the turret's physical position or state
- Handoff takes seconds — coverage is continuous

---

## Operator Roles Summary

| Role    | Controls          | Sees                        | Does not control     |
|---------|-------------------|-----------------------------|----------------------|
| Pilot   | Drone flight      | Drone FPV camera            | Turret, weapon       |
| Gunner  | Pan, tilt, fire   | Turret camera feed          | Drone flight         |

Additional gunners can monitor the feed in read-only mode during their
off-shift so they are always situationally aware and ready to take over.

---

## Power

- Small LiPo battery inside the turret body — independent of drone battery
- Target runtime: 2-4 hours on standby, 1-2 hours active panning/firing
- USB-C charge port accessible when stowed
- Low battery alert sent to gunner interface before critical shutdown

---

## Build Order (when ready)

1. Source nerf pistol and test trigger mechanism — confirm servo arm works
2. Build basic ESP32 pan/tilt rig on a bench, test servo control and WiFi
3. Build simple gunner interface (phone app or handheld controller)
4. Get camera streaming working end to end
5. Design and print the cylinder body with rotation ring
6. Integrate stow/deploy mechanism
7. Add landing legs
8. Test drone mount — confirm rotation doesn't destabilize hover
9. Test drop deployment from 1-2 meters
10. Final integration with helmet HUD if desired

---

## Notes

- This project is intentionally parked until the surrounding product
  ecosystem (DeskMediaDevice, AR helmet) proves viable commercially.
- The human-in-the-loop requirement is non-negotiable and should never
  be relaxed regardless of how capable the hardware becomes.
- The ESP32-S3 is sufficient for this entire control system — the same
  chip used in the DeskMediaDevice and the Waveshare thermal camera module.
  No need for a Jetson or more powerful compute at the turret level.
- If this system ever moves toward real-world protective use, proper legal,
  safety, and operational review would be required before deployment.
  The goal of protecting lives is what makes getting it right matter.
