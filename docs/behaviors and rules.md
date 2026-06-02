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

### Build and Flash Commands (Use PowerShell Workflows)

**ALWAYS use the .ps1 workflow scripts. They handle logging, parsing, and error checking automatically.**

```powershell
# Complete test cycle (from project root):
.\preflight_and_clean.ps1    # Preflight check + fullclean, logs to claudetransferv2\
.\build.ps1                  # Build + auto-parse for errors, logs to claudetransferv2\
.\flash.ps1                  # Flash + auto-parse for errors, logs to claudetransferv2\
# Manually power-cycle device (CRITICAL: unplug/replug or press RST button)
.\monitor.ps1                # Monitor + filter critical messages, logs to claudetransferv2\
```

**CRITICAL: FLASH AND MONITOR MUST NOT RUN TOGETHER**
The ST7701S display chip stays powered during soft reset (EN pin toggle). If flash and monitor run in one command (`idf.py -p COM4 flash monitor`), the device resets twice:
1. Once during flash (resets ESP32 AND display correctly)
2. Again during monitor (resets ESP32 only, display stays in confused state → blank screen)

**Solution:** Flash separately, then manually power-cycle device, then run monitor with `--no-reset` flag (which the .ps1 script does automatically).

**Raw command reference (if not using .ps1):**
```powershell
idf.py -p COM4 flash 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\flash_log.txt" -Encoding UTF8
# THEN: Manually power-cycle device (unplug/replug or RST button)
idf.py -p COM4 monitor --no-reset 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\monitor_log.txt" -Encoding UTF8
```

**Do NOT do this:**
```powershell
idf.py -p COM4 flash monitor   # ❌ WRONG — causes blank screen
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

**ALWAYS use combined add+commit+push in one command to reduce back-and-forth.**

Chain them with semicolons and use `-Append` for commit and push:

```powershell
git add "files/you/modified" 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_combined_log.txt" -Encoding UTF8; git commit -m "Your commit message" 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_combined_log.txt" -Encoding UTF8 -Append; git push origin main 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_combined_log.txt" -Encoding UTF8 -Append
```

This single line:
- Adds files to staging
- Commits with message
- Pushes to GitHub
- Logs all output to ONE combined log file for easy review

**Individual commands (if needed):**

```powershell
git status 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_status_log.txt" -Encoding UTF8
git log --oneline -10 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath "claudetransferv2\git_log.txt" -Encoding UTF8
```

### Git Log Backup Protocol (MANDATORY FOR CLAUDE)

**After EVERY git commit/push sequence:**

1. **User says "done"** → Claude READS git_combined_log.txt
2. **Claude VERIFIES success:**
   - Look for commit hash line: `[main XXXXX] Commit message`
   - Look for push confirmation: `XXXXX..YYYYY main -> main`
   - Check for errors: NO lines starting with `fatal:` or `error:`
3. **Claude REPORTS to user:** "Commit successful (hash XXXXX). Backing up logs."
4. **Only THEN:** Back up log file to `backup/` with timestamp
5. **Then:** Delete original from `claudetransferv2/`
6. **Finally:** Proceed to next step

**Example verification:**
```
✅ Good: [main 73036fd] Update rules - add combined git...
✅ Good:    73036fd..61dbba3  main -> main
❌ Bad:  fatal: pathspec 'docs/behaviors\' did not match
❌ Bad:  Everything up-to-date (means nothing was committed)
```

**Backup script (after verification passes):**
```bash
cd claudetransferv2
TIMESTAMP=$(date +%s)
mv git_combined_log.txt "backup/git_combined_log_attempt_${TIMESTAMP}.txt"
```

**CRITICAL:** Never skip the verification step. Silent git failures waste time.

This preserves a complete history of all git operations with timestamps, parallel to build/flash/monitor log backups.

**Why this matters:** Git logs get stale quickly. Archiving them with timestamps keeps the working directory clean and prevents confusion between old and new operations.

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
