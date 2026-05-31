# Efficiency System — How to Run Sessions Efficiently
**Created:** May 31, 2026 | **For:** All future sessions (Haiku + Sonnet)

This document explains the quota/token efficiency system built for this project.
Read this before starting work so you know how to use the tools.

---

## Why This Exists

Long debugging sessions on this project were burning enormous amounts of tokens because:
- Full 4000-line build logs were being fed to Claude raw
- Monitor logs were being read completely instead of filtered
- Each session re-learned the same board quirks from scratch
- Git diffs were being analyzed in full instead of summarized

These tools cut that waste significantly.

---

## The 6 Tools (build_status.bat was DROPPED — adds overhead, no benefit)

### 1. `parse_logs.bat` — Extract errors/warnings from build logs
**When to use:** After every build. Run it on `build_log.txt` before sharing with Claude.

```powershell
cd C:\Users\Administrator\Documents\DeskMediaDeviceV2
.\parse_logs.bat claudetransferv2\build_log.txt build
```

**Output files in `claudetransferv2/`:**
- `ERRORS(Stop!Break).txt` — actual build errors (stop and fix these)
- `WARNINGS(Safe to Skip).txt` — non-critical warnings (usually ignorable)

**Share these instead of the raw build log.** GT911 I2C errors are automatically
excluded from the errors file (they're in warnings where they belong).

---

### 2. `filter_monitor.bat` — Extract critical lines from monitor logs
**When to use:** After every flash+monitor cycle.

```powershell
.\filter_monitor.bat claudetransferv2\monitor_log.txt
```

**Output:** `claudetransferv2\MONITOR_FILTERED.txt`

Share `MONITOR_FILTERED.txt` instead of the raw monitor log. Cuts out boot spam,
keeps errors, init milestones, audio/display/SD status.

---

### 3. `summarize_diff.bat` — Summarize git diffs after commits
**When to use:** After `git commit` when you want Claude to review what changed.

```powershell
.\summarize_diff.bat
```

**Output:** `claudetransferv2\GIT_DIFF_SUMMARY.txt`

Share this instead of running `git diff` — shows files changed and key line modifications.

---

### 4. `cleanup_build_artifacts.bat` — Remove object file noise from build logs
**When to use:** Optional. Run on build_log.txt if it's particularly noisy.

```powershell
.\cleanup_build_artifacts.bat claudetransferv2\build_log.txt
```

---

### 5. `preflight_check.bat` — Verify build readiness before compiling
**When to use:** Before every build cycle, especially after menuconfig changes.

```powershell
.\preflight_check.bat
```

Checks: CONFIG_SPIRAM=y, CONFIG_BSP_ERROR_CHECK=n, main.c in CMakeLists,
tool scripts present, backup directory exists.

**If preflight fails:** Fix the issue before building. Saves a wasted compile cycle.

---

### 6. `CONSOLE_PATTERNS_REGISTRY.md` — Reference of known-safe patterns
**When to use:** When you see a warning/error you don't recognize, check here first.

```
C:\Users\Administrator\Documents\DeskMediaDeviceV2\CONSOLE_PATTERNS_REGISTRY.md
```

Documents 20+ expected patterns that look scary but are harmless.
Update this file every time you confirm a new pattern is safe.

---

## Recommended Workflow Per Build Cycle

```powershell
# 1. Preflight check
.\preflight_check.bat

# 2. Build (fullclean mandatory if .c/.h/CMakeLists changed)
idf.py fullclean 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\clean_log.txt" -Encoding UTF8
idf.py build 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\build_log.txt" -Encoding UTF8

# 3. Parse build output
.\parse_logs.bat claudetransferv2\build_log.txt build
# → Share ERRORS(Stop!Break).txt with Claude instead of raw build_log.txt

# 4. Flash + Monitor (combined — NEVER separate)
idf.py -p COM4 flash monitor 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\monitor_log.txt" -Encoding UTF8

# 5. Filter monitor output
.\filter_monitor.bat claudetransferv2\monitor_log.txt
# → Share MONITOR_FILTERED.txt with Claude instead of raw monitor_log.txt
```

---

## Fixes Made to the Tools (May 31, 2026)

These bugs were found by Sonnet during review and fixed:

**parse_logs.bat:**
- Removed `findstr /I "expected"` — was matching documentation and CMake output (false positives)
- Added GT911 exclusion to I2C error filter: `findstr /V "GT911\|0x14\|0x5d\|lcd_panel.io.i2c"`

**filter_monitor.bat:**
- Replaced `"delay\|Delay\|vTaskDelay\|ms"` with just `"vTaskDelay"` — "ms" matched everything

**preflight_check.bat:**
- Replaced `findstr` sdkconfig checks with Python one-liners — sdkconfig is binary on ESP-IDF v5.x,
  findstr silently returns wrong results on binary files
- Uses ESP-IDF bundled Python: `C:\Espressif\tools\python\v5.5.4\venv\Scripts\python.exe`

**build_status.bat:**
- Renamed to `build_status.bat.DROPPED` — adds workflow overhead without saving tokens

---

## New Rule: Session Handoff Summary (Mandatory)

Added to `docs/behaviors and rules.md`.

**Every session must end with a ~100 line summary covering:**
1. Current state (what works, what's broken)
2. Last changes made (files + why)
3. Next task (specific enough to start immediately)
4. Known issues / gotchas

**Why:** The biggest token waste is a new session re-reading all the code to get oriented.
A tight summary prevents 300-500K tokens of context-building at session start.

See `docs/behaviors and rules.md` for the full rule and template.

---

## Git Verification Protocol (Mandatory)

**After every git command (add, commit, push), always:**
1. Ask the user for the command output
2. Check for errors or warnings
3. Verify the commit/push succeeded before proceeding
4. If errors appear, diagnose and fix before continuing

**Why:** Silent git failures waste time. Corrupted commits or failed pushes can derail the session.
Early detection prevents needing to rollback later.

**Example:**
```powershell
git commit -m "Fix audio initialization"
# → Claude asks: "What did the output show?"
# → User pastes: [main abc1234] Fix audio initialization
# → Claude confirms: "Good, committed. Ready to push?" 
# → User: git push
# → User shows output
# → Claude checks for errors and confirms GitHub status
```

---

## File Locations Reference

| File | Location |
|------|----------|
| All 6 tool scripts | `DeskMediaDeviceV2/` root |
| Filtered outputs | `claudetransferv2/` |
| Log backups | `claudetransferv2/backup/` |
| Patterns registry | `DeskMediaDeviceV2/CONSOLE_PATTERNS_REGISTRY.md` |
| Tool review notes | `claudetransferv2/EFFICIENCY_TOOLS_REVIEW.md` |
| Behaviors & rules | `docs/behaviors and rules.md` |
| Board quirks | `HANDOFF_FOLDER/BOARD_TIPS_AND_QUIRKS.md` |
| This document | `HANDOFF_FOLDER/EFFICIENCY_SYSTEM.md` |
