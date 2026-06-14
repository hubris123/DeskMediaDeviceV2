# System Efficiency & UX Review — 2026-06-13 (analysis only, nothing implemented)

Scope: every task, LVGL timer, allocation, API call, and core-affinity decision in `main/`.
Two ranked lists below: **A) Hardware (CPU / memory / power / cores)** and **B) User experience**, each most-impactful first. Each item has the finding (with file refs), why it matters, a proposed approach, and risk/effort. We decide in the morning which to take.

---

## Baseline map (how the system is wired today)

**Always-on LVGL timers** (run on the LVGL task, prio 6):
- `weather_overlay_cb` — 500 ms ([main.c:1165](main/main.c:1165)) — queries 6 wifi/weather state fns every tick; only touches LVGL when the message changes.
- `music_watchdog_cb` — 500 ms ([main.c:1127](main/main.c:1127)) — detects natural mp3 end; runs forever even with no music.
- `clock_timer_cb` — 1000 ms ([main.c:1128](main/main.c:1128)) — re-renders the time label every second though it changes once a minute.
- `mem_label_cb` — 1000 ms ([main.c:1125](main/main.c:1125)) — DEBUG heap readout (`INT:xK PSRAM:xK`).
- `scroll_tick_cb` — 67 ms, only while music panel open+playing.
- `music_autoclose` — 30 s, only while panel open.

**Tasks** (all `xTaskCreate` = NO core affinity unless noted):
- LVGL render task: prio 6, **core affinity -1 (floats)** — `ESP_LV_ADAPTER_DEFAULT_CONFIG`.
- `app_main`/weather-display loop: **pinned CPU0**, 1 s poll, change-detect.
- `esp_timer` task: **CPU0** (drives the wifi reconnect timer; LVGL tick path).
- mp3 (prio 5), video_player (prio 5), tick/preview/success (4–5), weather (prio 2), ota_check (3), ota_manual (4), content (3), wifi_scan (5) — all float.
- esp_hosted WiFi: prio 23; LWIP tcpip: no affinity.

**API cadence:** NWS obs every 15 min + forecast (hourly+daily = 2 calls) every 60 min ≈ 6 calls/hr (~144/day); grid/station cached in NVS. OTA check 60 s after boot then 24 h. content_sync 75 s then 24 h. Geocode+grid only on zip change / reconnect.

**Big allocations (PSRAM):** `video_buf` 16 MB + `bumper_buf` 5 MB allocated at boot for the whole runtime ([video_player.c:51-52](main/video_player.c:51)); 3 DPI framebuffers (~2.25 MB); JPEG decode buffer (~1.5 MB). ~21 MB tied up before any video plays.

---

## A. HARDWARE (CPU / memory / power / cores) — most → least impactful

### A1. Dual-core: almost nothing is pinned (biggest untapped lever)
**Finding:** LVGL render task floats (core -1); every app task floats; only `app_main` and `esp_timer` are on CPU0. FreeRTOS load-balances dynamically, which causes cross-core cache thrash and unpredictable scheduling — the opposite of the "give the heavy job a clean core" idea we used for video.
**Why:** This board is dual RISC-V. A deliberate split could parallelize render vs. decode vs. radio and cut contention.
**Plan:** Define a core strategy and pin explicitly via `xTaskCreatePinnedToCore` / the adapter's `task_core_id`:
- Pin the **LVGL/display render task to CPU1** (a dedicated UI core).
- Keep **WiFi(hosted)/LWIP + weather/OTA/content + app_main on CPU0**.
- Decide where **mp3/video decode** lands (CPU-heavy; ideally the core NOT doing the radio burst at that moment).
**Measure first/after** with `CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS` + `vTaskGetRunTimeStats` (per-core %). 
**Risk/effort:** Medium-high; wrong pinning can hurt — needs A/B measurement. **Impact: High (if measured).**

### A2. Idle timer load — 4 always-on timers, two at 500 ms
**Finding:** Even fully idle, the LVGL task wakes ~4×/sec for `weather_overlay` (500 ms) + `music_watchdog` (500 ms), plus 2 at 1 s. `weather_overlay_cb` calls 6 state fns every 500 ms; `clock` re-renders every second for a per-minute value; `music_watchdog` polls with no music.
**Why:** Constant low-level CPU, blocks deeper idle/power-down, adds steady wakeups.
**Plan:**
- Remove `mem_label` debug timer entirely (also a UX request).
- Slow `weather_overlay` to 1 s (banner latency is fine) or event-drive it.
- `clock`: update only when the minute changes (dedup `lv_label_set_text`), align to the minute.
- `music_watchdog`: start it on play, stop it on stop — not always-on.
**Risk/effort:** Low. **Impact: Medium** (cuts idle wakeups ~half+).

### A3. MP3 read buffer is only 2 KB
**Finding:** `MP3_READ_BUF_SIZE = 2048` ([main.c:569](main/main.c:569)) → many small `fread`s per second during playback (FATFS + SD overhead). Already flagged in prior handoff.
**Why:** Extra CPU/IO and SD activity (power) throughout every song.
**Plan:** Raise to 16–32 KB (internal or PSRAM). Fewer SD transactions, smoother decode, less CPU. Same idea applies to WAV/PCM paths.
**Risk/effort:** Low. **Impact: Medium.**

### A4. 21 MB PSRAM held for video even when video never plays
**Finding:** `video_buf` 16 MB + `bumper_buf` 5 MB allocated at boot for the whole runtime; bumper file is only ~1.8 MB so the 5 MB slot is oversized.
**Why:** Ties up ~21 MB PSRAM permanently (leaves ~7–10 MB), and PSRAM refresh is a small constant power draw.
**Plan:** Lazy-allocate `video_buf` on first skull tap; shrink `bumper_buf` to actual size + margin (~3 MB). Optionally free `video_buf` when idle if PSRAM pressure ever matters (with a fragmentation fallback).
**Risk/effort:** Medium (alloc-on-demand needs a clean failure path). **Impact: Medium** (headroom, minor power).

### A5. Tick sound recomputes 640 `sinf` per tap + spawns a task each time
**Finding:** `tick_task` builds the 880 Hz burst with 640 `sinf` calls on every touch and `xTaskCreate(4096)` per tap ([main.c:472](main/main.c:472)); same spawn pattern for preview/success.
**Why:** A CPU spike + task churn on every single tap.
**Plan:** Precompute the tick PCM once at boot into a static buffer and just write it; consider one long-lived audio-fx worker instead of spawning a task per sound.
**Risk/effort:** Low. **Impact: Low-Medium** (per-tap responsiveness).

### A6. LVGL timer "burst" after video resume is unhandled
**Finding:** `s_clock_timer`/`s_music_watchdog_timer`/`s_mem_label_timer`/`s_weather_overlay_timer` are declared ([main.c:99-102](main/main.c:99)) with the comment "reset after video playback to prevent burst" — but they're never assigned (dead vars; the build warns). On `esp_lv_adapter_resume()` after a clip, paused timers can all fire at once → CPU spike as the home screen returns.
**Why:** Jank/spike right at the video→home transition.
**Plan:** Capture the handles and `lv_timer_reset()` them on resume, OR just fix via A2 (fewer/slower timers). 
**Risk/effort:** Low. **Impact: Low-Medium.**

### A7. Dead code / files
**Finding:** `es8311_minimal.c` (140 lines) is **not even in CMakeLists** — dead file on disk. `data_binding.c` (399 lines) is largely unused (`ui_update_*` superseded by `weather_display.c`; maybe a couple `format_*` helpers still used). Unused callbacks: `play_wav_callback`, `play_pcm_callback`, `stop_callback`, `musiclabel_anim_cb` (build warns).
**Why:** Binary size + maintenance noise; negligible CPU.
**Plan:** Delete `es8311_minimal.c`; audit `data_binding.c` and keep only what's referenced (or drop the file); remove dead callbacks/vars.
**Risk/effort:** Low (verify references first). **Impact: Low** (cleanliness, slight flash).

### A8. NWS API cadence (minor)
**Finding:** obs every 15 min is reasonable and grid is cached; ~144 calls/day, well-behaved.
**Plan (optional):** Relax obs to 20–30 min if that freshness is acceptable — fewer radio wakeups/power. **Impact: Low.**

---

## B. USER EXPERIENCE — most → least impactful

### B1. OTA offers the OLDER v0.2.1 and would downgrade if installed
**Finding:** Version compare is plain string inequality, so any dev build treats the published `v0.2.1` as "new" and (pre-snooze) nagged every boot. Installing it reverts today's work.
**Plan:** Proper "strictly newer" comparison (don't offer older/equal), and/or suppress prompts for `-dirty`/dev builds, and/or cut a real release after this work so `running` == a real tag. (5-day snooze already softens it.)
**Risk/effort:** Low-Medium. **Impact: High** (avoids confusion + accidental downgrade).

### B2. Music panel behavior vs. weather freshness
**Finding:** No playlist auto-advance (one track then stop), and the panel-open gate defers ALL network for the whole song (weather can be song-length + 30 s stale).
**Plan:** Decide desired behavior: auto-advance? Allow lightweight weather during music but still block OTA/content? Tune the 30 s. 
**Risk/effort:** Low-Medium. **Impact: Medium.**

### B3. No "checking…" feedback on manual Check for Updates
**Finding:** ~1–2 s silent gap between tapping the button and the result dialog.
**Plan:** Brief "Checking for updates…" toast/spinner until the result. 
**Risk/effort:** Low. **Impact: Low-Medium.**

### B4. "UPDATING – PLEASE WAIT" needs a re-tap to play
**Finding:** When playback is blocked during a burst, the user must tap again after it clears (no auto-resume).
**Plan:** Optionally auto-start the requested clip/track once the burst ends (1–3 s). 
**Risk/effort:** Low-Medium. **Impact: Low-Medium.**

### B5. Clock blanks until SNTP/UTC ready; updates every second
**Finding:** Time label is empty until SNTP + UTC offset arrive; then re-renders every second for a per-minute value.
**Plan:** Show `--:--` placeholder meanwhile; only re-render on minute change (ties to A2).
**Risk/effort:** Low. **Impact: Low.**

### B6. Settings "Check for Updates" button layout is a guess
**Finding:** Two-line "Check for / Updates" on a 120 px button at offsets −150/0/+150; orange/black — needs your eyeball.
**Plan:** Adjust width/spacing/text/shade once you see it on-device. **Impact: Low (visual).**

---

## Suggested order to tackle (proposed)
1. **B1** (OTA downgrade/nag — correctness + safety).
2. **A2 + A6 + remove mem_label** (idle timers — easy, real, low risk).
3. **A3** (MP3 read buffer — easy win).
4. **A5** (precomputed tick — easy).
5. **A1** (core pinning — measure first; biggest but needs care).
6. **A4** (PSRAM lazy alloc — medium).
7. **A7** (dead-code cleanup).
8. UX polish: **B3, B2, B4, B5, B6**.

*Measurement to add before A1/A3/A4:* enable `CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS`, log `vTaskGetRunTimeStats` + `heap_caps_print_heap_info` periodically, so we have before/after numbers instead of guessing.
