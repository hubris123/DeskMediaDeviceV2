# Behaviors and Rules

## Session Handoff Summary (MANDATORY)

Every session must end by writing a handoff summary before closing. This is the
single highest-impact quota saver — it prevents the next session from spending
300-500K tokens re-reading code to get oriented.

### Where to save it
Save as `HANDOFF_FOLDER/SESSION_SUMMARY_[date].md` OR update the current session's
handoff doc in `HANDOFF_FOLDER/`.

### Required sections (keep it to ~100 lines total)

```markdown
## Current State
[What is working, what is broken, what is in-progress — one sentence each]

## Last Changes Made
[Files modified, what was changed, why — bullet list]

## Next Task
[The exact next step, specific enough that a new session starts immediately without questions]

## Known Issues / Gotchas
[Board quirks, config requirements, anything that will waste time if forgotten]
```

### Rules
- This is NOT optional. Every session end = one summary written.
- Keep it tight — 100 lines max. Dense is better than verbose.
- Include the git commit hash of the last checkpoint.
- If blocked mid-task, describe EXACTLY where it's blocked and what was tried.

---

## Research Protocol

When trying to figure something difficult out or when there are too many questions to easily move forward, research in the folder:

```
C:\Users\Administrator\Documents\DeskMediaDeviceV2\docs
```

> If this folder does not exist because it was deleted, please ask and I will replace it.

## Restore Points and Git Backups

After each milestone, or at points where we need to try multiple coding options for getting something working, we must:
1. Set a restore point
2. Push up to git to have a backup and spot to return to
3. Document details explaining why the point was created

### Git Checkpoint Format

Commit messages should include:
- What feature/fix was completed or stabilized
- Key changes made (config, components, code)
- Why the checkpoint matters (memory constraints, breaking changes, major milestone)

Example:
```
git commit -m "Audio codec working - ES8311 direct I2S setup

- Disabled CONFIG_BSP_ERROR_CHECK to prevent GT911 abort
- Implemented es8311_minimal codec driver
- I2S streaming at 16kHz verified
- Reason: Need stable baseline before adding button playback logic"
```

---

## Build Discipline

### Fullclean Requirements

**ALWAYS run `idf.py fullclean` BEFORE build if:**
- Code files were modified (.c, .h, CMakeLists.txt)
- sdkconfig was changed
- Components were added/removed
- Includes or dependencies changed

Only skip fullclean for retry builds with identical code after a transient error.

### Feature Freeze Gates

After completing a major feature or reaching a stable milestone:
1. Create a git checkpoint with detailed commit message
2. Do NOT add new major components or features until the next checkpoint
3. If memory or linker constraints are tight, this becomes a hard gate—document it
4. Lock in the stable state before attempting experimental changes

### Build and Flash Commands

```powershell
idf.py fullclean 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\clean_log.txt" -Encoding UTF8
```

```powershell
idf.py build 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\build_log.txt" -Encoding UTF8
```

**IMPORTANT: Always use the combined flash+monitor command. Never run flash and monitor as separate commands.**
The ST7701S display chip stays powered during a soft reset (EN pin toggle). When monitor runs separately it resets the ESP32 but not the display, causing a blank screen. The combined command resets once during flash and monitor captures the boot immediately — no second reset.

```powershell
idf.py -p COM4 flash monitor 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\monitor_log.txt" -Encoding UTF8
```

If you need to attach monitor to an already-running device without resetting it:

```powershell
idf.py -p COM4 monitor --no-reset 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\monitor_log.txt" -Encoding UTF8
```

### Workflow Scripts

All build workflow scripts are located in the `scripts/` folder at project root:
- `preflight_and_clean.ps1` — Preflight check + fullclean (in project root, calls scripts)
- `build.ps1` — Build + parse logs (in project root, calls scripts)
- `flash.ps1` — Flash + parse logs (in project root, calls scripts)
- `monitor.ps1` — Monitor + filter logs (in project root, calls scripts)

All `.bat` utility scripts are located in `scripts/`:
- `preflight_check.bat` — Verify build readiness
- `parse_logs.bat` — Extract errors/warnings from logs
- `filter_monitor.bat` — Extract critical messages from monitor output
- `summarize_diff.bat` — Summarize git diffs
- `cleanup_build_artifacts.bat` — Clean object file noise from logs

### Git Commands with Logging

Git commands should follow the same logging format:

```powershell
git status 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_status_log.txt" -Encoding UTF8
```

```powershell
git add -A 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_add_log.txt" -Encoding UTF8
```

```powershell
git commit -m "Your message here" 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_commit_log.txt" -Encoding UTF8
```

```powershell
git push origin main 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_push_log.txt" -Encoding UTF8
```

```powershell
git log --oneline -10 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_log.txt" -Encoding UTF8
```

### Git Log Backup Protocol

After each git command session, back up all git log files to the archive:

```powershell
# Backup all git_*.txt files with Unix timestamp
TIMESTAMP=$(date +%s)
foreach ($f in Get-ChildItem "claudetransferv2\git_*.txt") {
    Move-Item $f.FullName "claudetransferv2\backup\$($f.BaseName)_attempt_${TIMESTAMP}.txt"
}
```

This preserves a complete history of all git operations with timestamps, parallel to build/flash/monitor log backups.

### Build Error Triage Process

When a build fails:
1. Search the log for "error:" to find the first error
2. Read the full error message with context (lines before/after)
3. Identify error category: linking, include path, compilation, config conflict, or memory
4. Do NOT proceed to next build attempt until root cause is understood
5. Document the error and fix in a comment or commit message for future reference

---

## Log Files

### Location

Log files from the above commands are written to:

```
C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2
```

When the commands are run, the log file will be there so Claude can read them.

### Backup Protocol

Before each build, clean, flash, monitor, or git command is requested by Claude, the files need to be backed up to the backup folder:

```
C:\Users\Administrator\Documents\DeskMediaDeviceV2\claudetransferv2\backup
```

Claude can decide how to organize the files and rename them to keep them as an archive after build, clean, flash, monitor, or git commands.

### Archive Organization

**My Archive Strategy:**

Log files are backed up with Unix timestamps to maintain a chronological, complete history of all build and git operations. Before any new build/flash/monitor/git commands are executed, I will:

1. Check for existing log files in `claudetransferv2/` (clean_log.txt, build_log.txt, flash_log.txt, monitor_log.txt, git_*.txt)
2. Back them up to the `backup/` folder with naming format: `{logtype}_attempt_{timestamp}.txt`
3. Delete the original log files from the main claudetransferv2 folder
4. Run the new commands, which will create fresh log files in the main folder
5. After reviewing the results, back those up the same way and clean up again

Example structure:

```
backup/
├── clean_log_attempt_1780180416.txt
├── build_log_attempt_1780180416.txt
├── flash_log_attempt_1780180416.txt
├── monitor_log_attempt_1780180416.txt
├── git_status_log_attempt_1780180416.txt
├── git_commit_log_attempt_1780180416.txt
├── clean_log_attempt_1780180500.txt
├── build_log_attempt_1780180500.txt
└── ...
```

This approach keeps the working directory clean while maintaining a timestamped archive of all attempts (build and git operations) for debugging and reference.

---

## Rollback Procedure

If a build breaks and needs to be reverted to a known good state:

1. Identify the last working git commit (from checkpoint messages)
2. Note the commit hash from git log
3. Run: `git checkout {commit_hash}`
4. Run: `idf.py fullclean && idf.py build`
5. Flash and test to verify stability before attempting new changes

Keep commit hashes of stable checkpoints documented in milestone notes for quick reference.

---

## Configuration Change Log

Track sdkconfig changes separately from code changes. When modifying sdkconfig:

1. Document WHAT was changed (e.g., `CONFIG_BSP_ERROR_CHECK=y` → `CONFIG_BSP_ERROR_CHECK=n`)
2. Document WHY it was changed (e.g., "Prevents GT911 I2C error from aborting app")
3. Include in commit message if it's a critical change
4. If a config change is reverted, explain why in both directions

This prevents accidentally re-introducing config problems in future work.
