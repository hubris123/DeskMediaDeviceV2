# Vehicle-Mounted Drone Defense System — Concept Note
## Status: Long-term idea, not started. Capture only.
## Intent: Defensive — protecting medical, humanitarian, and emergency
## vehicles from drone harassment or attack. Getting help to people
## who need it. No offensive application intended.

---

## Core Concept

A vehicle-mounted drone defense system using a fixed 9-barrel compressed
air array on a heavy geared stepper mount. Primary use case: protecting
medical vehicles, humanitarian convoys, and emergency responders from
drone harassment or attack. A drone disabling a medical vehicle en route
to a patient is a life-safety emergency — this system clears the path.

Proof of concept uses compressed air with small pellets. Fully defensive
application — the goal is to get the vehicle and its occupants through
safely, not to cause harm.

## Why 9 Barrels Instead of 1 Tracking Barrel

A single tracking barrel must mechanically lead a fast moving drone with
perfect precision — any stepper lag or prediction error is a miss. A 9-barrel
array at overlapping angles trades mechanical precision for geometric coverage.
The stepper only needs gross pointing into the threat quadrant. The barrel
spread handles the precision. At close range adjacent barrels overlap — the
drone is covered by 2-3 barrels simultaneously. Fire the zone, not the point.

This makes target acquisition dramatically faster — it's a lookup into which
barrel zone the drone occupies, not a pursuit problem.

## Barrel Array Geometry

9 barrels arranged in a pattern of slightly different angles — center barrel
plus 8 surrounding at increasing angular offsets. At close range the spread
is tight and overlapping. At longer range it fans out to cover more area.
The targeting algorithm determines which barrel or combination of barrels
has the drone in its coverage cone and fires those only.

## Vision System — 3 Cameras

- Stereo pair (wide baseline) — triangulation for X/Y/Z depth of target
- Third camera offset — better vertical tracking for diving or climbing drones
- All three feeds into a Jetson for real-time target detection and position
- Target position in 3D space maps to barrel selection lookup table
- Stepper handles gross pointing — cameras handle fine targeting solution

## Targeting Algorithm Concept

```
1. Cameras detect and track drone in frame (YOLO or similar)
2. Stereo triangulation gives X, Y, Z position in space
3. Convert 3D position to angular offset from array center
4. Look up which barrel(s) cover that angular zone
5. Command stepper to center array on threat quadrant
6. Fire selected barrel(s) when confidence threshold met
7. Re-evaluate every frame — update barrel selection as drone moves
```

## Proof of Concept Build

- Compressed air solenoids replace live rounds — fully legal anywhere
- 3D printed barrel array at correct angles
- Heavy geared stepper motor — sized to handle real recoil loads in
  production, so the proof of concept mount is already correctly spec'd
- Jetson Orin Nano for vision processing (same as helmet project)
- 3 cameras — USB or CSI

## Why This Architecture Is Sound

Fast racing drones do 60-80mph. At 30 meters that's still a manageable
angular rate. The stepper doesn't need to perfectly track — it needs to
get within the array's coverage cone. The barrel geometry does the rest.
Coverage zones overlap at close range where the threat is greatest.
This is a fundamentally more robust solution than single-barrel tracking
for close-range fast-moving targets.

## Market Context

Medical and humanitarian vehicles operating in conflict zones or high-risk
areas have no current affordable drone defense options. A drone can disable
a vehicle, block a road, or target occupants — all of which cost lives when
the vehicle is carrying medical personnel or supplies. NGOs, field hospitals,
emergency services, and humanitarian organizations all face this problem with
no good solution available at a price point they can afford.

A compact, vehicle-mountable, automated system that requires minimal operator
attention — the driver focuses on driving, the system handles the threat —
fills a real gap. The same architecture scales from a single vehicle unit
to convoy protection with networked units sharing target data.

## Priority

**LOW — long term concept only.**
Current priority order:
1. ESP32-P4 weather display (active)
2. Weapon round counter (next)
3. AR helmet system
4. Nerf turret sentry
5. This system

Finish the desk device first. The ideas will keep.
