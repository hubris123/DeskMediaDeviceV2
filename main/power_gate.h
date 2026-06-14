#pragma once
#include <stdbool.h>

/**
 * Power/CPU interlock between high-power WiFi bursts and video playback.
 *
 * A WiFi radio burst stacked on a video's peak current draw browned the board
 * out (reset reason POWERON, no panic). Network work and video playback are
 * mutually exclusive: whichever claims the gate first runs; the other waits.
 * The claim is done under a critical section so the two can never both start.
 */

// ── Network side ─────────────────────────────────────────────────────────────
// Block the calling network task until it is safe to make HTTP calls:
//   - no video is playing,
//   - the user has stopped interacting (~5 s debounce, NOT a long idle wall, so
//     weather stays fresh and never goes stale waiting for a long quiet gap),
//   - no other network burst is already in progress.
// Atomically marks a network burst in-progress until power_gate_net_end().
void power_gate_net_wait_and_begin(void);
void power_gate_net_end(void);

// True while a deferred network burst (weather / OTA / content sync) is running.
bool power_gate_net_busy(void);

// ── Video side ───────────────────────────────────────────────────────────────
// Atomically claim the gate for video playback. Returns false if a network
// burst already holds it (caller should warn + not start). On true the caller
// MUST pair it with power_gate_video_end() when playback finishes.
bool power_gate_video_try_begin(void);
void power_gate_video_end(void);
