# Core-Pinning / Dual-Core Plan — ESP32-P4 Desk Media Device

**Purpose:** This is a self-contained brief for a FRESH session to improve dual-core
utilization safely. A previous attempt at core pinning **crashed the device hard**
(details not remembered), so the entire point of this doc is to do it *measured and
incremental* instead of big-bang. Read it fully before changing anything.

**Project:** `C:\Users\Administrator\Documents\DeskMediaDeviceV2`
**Toolchain:** ESP-IDF **v5.5.4 ONLY** — `& 'C:\esp\v5.5.4\esp-idf\export.ps1'`. DO NOT change IDF version.
**Board:** Waveshare ESP32-P4-WIFI6-Touch-LCD-4.3 (dual RISC-V, cores 0 & 1). WiFi is a
separate C6 over SDIO via `esp_hosted`.
**Flash:** free COM4 first (kill stale `idf.py monitor` python procs holding the port), then
`idf.py -p COM4 flash`.

---

## ⛑️ Safety rules (non-negotiable — this is the part the last attempt skipped)

1. **Known-good baseline = commit `1636431` on `main`** (today's verified-stable firmware).
   Do NOT commit any pinning experiment until it's soak-verified. If anything crash-loops:
   `git stash` (or `git checkout -- .`), rebuild, reflash → you're back to good.
2. **Measure before and after.** Don't guess which core is the bottleneck — instrument it
   (Step 0). The crash last time was almost certainly an unmeasured over-aggressive pin.
3. **ONE change at a time** → build → flash → **capture boot serial** → **soak** → keep or
   revert. Never stack two pinning changes before testing.
4. **After every flash, capture the boot log and check:** reset reason (3=SW from your RTS
   pulse and 1=POWERON-after-pulse are normal for a tool reset; a *spontaneous* POWERON =
   brownout, `Guru Meditation` = panic, `wdt`/`watchdog` = stall), display comes up, touch
   works, WiFi connects, weather fetches. Any of those failing → revert.
5. **Soak each change:** play 2–3 videos, play music, leave idle through a weather tick
   (~15 min or force one), run a manual "Check for updates". Watch for resets.

---

## Traps — why core-pinning crashes on THIS stack

- **LVGL render task vs the display peripheral.** The display panel + touch are initialized
  in `app_main` (runs on **CPU0**), so their DPI/DMA2D/touch ISRs live on CPU0. The LVGL task
  currently **floats** (`task_core_id = -1`). Forcing the LVGL task onto CPU1 while its flush
  waits on CPU0-registered interrupts is the historically risky move. If you pin LVGL, try
  **CPU0 first** (same core as its peripheral) — that's the conservative version.
- **`esp_hosted` WiFi (SDIO, prio 23)** is sensitive — do NOT repin its tasks or the SDIO
  ISRs. Leave the WiFi stack alone.
- **ISR-driven semaphores:** pinning a task that blocks on a sem given from an ISR to a core
  the ISR doesn't fire on can stall. (Sem-give-from-ISR is core-agnostic in IDF, but be wary.)
- **LVGL is not thread-safe.** Any LVGL call from a non-LVGL task MUST hold `bsp_display_lock`
  — the code already does this; don't introduce new unlocked LVGL access when moving work.
- **Do NOT** touch `CONFIG_FREERTOS_UNICORE`, `app_main` affinity, or the hosted/SDIO affinity.

---

## Baseline (current state, from the code)

- **LVGL render task:** prio 6, **core -1 (floats)** — set via `ESP_LV_ADAPTER_DEFAULT_CONFIG`
  in [main.c:1278](main/main.c:1278) (`cfg.lv_adapter_cfg`). Field to change: `cfg.lv_adapter_cfg.task_core_id`.
- **app_main / weather-display loop:** pinned **CPU0** (`CONFIG_ESP_MAIN_TASK_AFFINITY_CPU0`).
- **esp_timer task:** CPU0.
- **All app tasks float** (`xTaskCreate`, no affinity):
  - mp3 decode `mp3_task`: [main.c:937](main/main.c:937) (video audio), [main.c:955](main/main.c:955) (music), [main.c:1030](main/main.c:1030) (fx helper) — prio 5.
  - `video_player_task`: [video_player.c:212](main/video_player.c:212) — prio 5.
  - `weather_task_main`: [weather_task.c:247](main/weather/weather_task.c:247) — prio 2.
  - `ota_check_task`: [ota_update.c:433](main/ota_update.c:433); `ota_manual_check_task`: [ota_update.c:439](main/ota_update.c:439) — prio 3/4.
  - `content_check_task` / `sync_once_task`: [content_sync.c:347](main/content_sync.c:347),357,366 — prio 3.
- **esp_hosted WiFi:** prio 23. **LWIP tcpip:** no affinity.
- **Mechanism to pin a task:** change `xTaskCreate(fn, name, stack, arg, prio, &h)` →
  `xTaskCreatePinnedToCore(fn, name, stack, arg, prio, &h, CORE)` (CORE = 0 or 1).

---

## Step 0 — Measurement FIRST (do this before any pin)

Goal: a baseline of **per-core utilization** in three states (idle, music, video), so we know
where the load is and can prove each change helped.

**sdkconfig changes** (set in `sdkconfig` AND `sdkconfig.defaults` so they survive fullclean):
- `CONFIG_FREERTOS_USE_TRACE_FACILITY=y`
- `CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS=y`
- `CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER=y`
- `CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID=y`  (adds the core column to `vTaskList`)

**Temporary debug print** (remove before final commit): a one-shot task or `esp_timer` that
every ~30 s logs both:
- `vTaskGetRunTimeStats(buf)` → per-task accumulated runtime + % (read **IDLE0 vs IDLE1** % =
  inverse of each core's utilization).
- `vTaskList(buf)` → per-task state, **core**, and stack high-water-mark.

Capture the log in each state: (a) home screen idle, (b) music playing, (c) video playing.
Save the three snapshots — they are the before/after yardstick.

> Reading it: if one core's IDLE% is near 0 while the other is high, that core is the
> bottleneck and pinning heavy work to the idle core is the win. If both idle high, pinning
> won't help much and we stop (don't pin for its own sake).

---

## Options, ranked LOW→HIGH risk (apply one at a time, measure each)

### Option A — Pin the two heavy DECODE tasks to CPU1  *(lowest risk, do first)*
Change `mp3_task` creates ([main.c:937,955,1030](main/main.c:955)) and `video_player_task`
([video_player.c:212](main/video_player.c:212)) to `xTaskCreatePinnedToCore(..., 1)`.
- **Why:** decode (libhelix MP3 on CPU; HW-JPEG completion handling) is the heaviest app CPU;
  WiFi/system effectively sit on CPU0. Give decode the other core. LVGL is *paused* during
  video, so no UI contention there; during music, pinning mp3→CPU1 lets LVGL prefer CPU0.
- **Risk:** low — these don't own display ISRs; I2S/JPEG completions are core-agnostic.
- **Test:** music soak + video soak; confirm audio isn't choppy and frames are smooth.

### Option C — Pin background NETWORK tasks to CPU0  *(low-medium)*
`weather_task_main`, `ota_check_task`/`ota_manual_check_task`, `content_*` →
`xTaskCreatePinnedToCore(..., 0)`.
- **Why:** TLS/mbedtls + LWIP data locality on the WiFi core; keeps them off the decode core.
  (Note: `power_gate` already blocks network during video/music, so this is mostly locality.)
- **Risk:** low-medium.
- **Test:** weather tick + manual update check while idle; confirm no WiFi/TLS regressions.

### Option B — Pin the LVGL render task  *(medium-HIGH risk; the suspected prior crash)*
Set `cfg.lv_adapter_cfg.task_core_id` in [main.c:1278](main/main.c:1278).
- **B-conservative (try first):** pin to **CPU0** (same core as the display peripheral init).
  Safer; guarantees the task runs where its DPI/touch ISRs live.
- **B-aggressive (only if data demands):** pin to **CPU1** (dedicated UI core) — ONLY if Step 0
  shows CPU0 saturated and CPU1 has headroom after A+C. Heavy soak: UI responsiveness, touch,
  the video→home transition, the music scroll animation.
- **Risk:** this is the revert-prone one. If the display wedges/blanks or touch dies, revert
  immediately and reflash `1636431`.

### Option D — (skip unless measured) LWIP tcpip affinity
Only consider pinning tcpip to CPU0 if Step 0 shows it bouncing cores and costing cache. Low value.

### DO NOT
- Change `CONFIG_FREERTOS_UNICORE`, `app_main` affinity, or repin `esp_hosted`/SDIO.

---

## Recommended sequence
1. **Step 0 measurement** → baseline (idle / music / video).
2. **Option A** (decode → CPU1) → measure → soak.
3. **Option C** (network → CPU0) → measure → soak.
4. **Option B-conservative** (LVGL → CPU0) → measure → soak.
5. **Option B-aggressive** (LVGL → CPU1) ONLY if data says CPU0 is the bottleneck → heavy soak.
6. Keep what measurably improves AND stays stable; revert the rest. Remove the debug
   stats print. Commit once verified.

## Per-change test protocol
```
& 'C:\esp\v5.5.4\esp-idf\export.ps1'
idf.py build
# free COM4: kill any python 'idf.py monitor ... COM4' procs, then:
idf.py -p COM4 flash
# capture boot serial (reset reason, no panic, display up, wifi+weather ok)
# soak: 2-3 videos, music, idle weather tick, manual update check
# capture runtime-stats snapshot in idle/music/video; compare to baseline
# stable + improved -> keep ; crash/regression -> git stash + rebuild + reflash 1636431
```

## Expected outcome
Most likely win is **Option A + Option C** (decode on CPU1, network on CPU0) with LVGL left
floating or pinned conservatively to CPU0 — a clean "UI+system on 0, heavy decode on 1" split
without the risky LVGL-to-CPU1 move that probably caused the prior crash. Let the **measurement
decide** whether B-aggressive is worth the risk.
