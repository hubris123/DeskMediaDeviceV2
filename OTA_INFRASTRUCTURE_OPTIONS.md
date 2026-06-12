# OTA / Firmware Distribution — Infrastructure Options
*Captured June 11, 2026. Decision deferred until product launch planning.*

## What is ALREADY implemented (working tree, June 11)
- `main/ota_update.c/.h` — background task: checks GitHub releases API 60s after
  boot + every 24h, compares release tag vs running firmware version, on-screen
  Install/Later prompt, installs via `esp_https_ota`, reboots
- `partitions.csv` — OTA layout: otadata @0x10000, ota_0 @0x20000 (8MB), ota_1 (8MB),
  nvs/phy_init offsets unchanged (settings survive the migration flash)
- `tools/flash_and_reset.ps1` — updated for app @0x20000 + otadata erase
- sdkconfig — `CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE=y` (boot-then-crash auto-reverts;
  `ota_update_mark_boot_valid()` called at System Ready)
- Wired to the CURRENT public repo `hubris123/DeskMediaDeviceV2` releases — works
  today with zero auth for a hobby/single-device setup
- **DEPLOYED & VERIFIED June 11 2026:** v0.1.0→v0.1.5 shipped via OTA, self-healing
  post-OTA boot (wedge detector, up to 3 retries), installed-tag dedup, quiet
  install screen, version footer in settings. Device fully updates itself.

## Decision 1 — Where do release binaries live?

| Option | Cost | Security | Effort | Notes |
|---|---|---|---|---|
| Public GitHub repo releases (current code) | free | none — anyone can download fw | done | fine for hobby, wrong for product |
| **Separate private GitHub repo, releases only** | free | token-gated download | small | the leading candidate pre-launch; keeps source repo public/private independently |
| S3 / Cloudflare R2 + signed URLs | ~free at small scale | presigned URLs, no long-lived device secret | medium | needs a tiny backend to mint URLs |
| Own update server (static HTTPS + manifest JSON) | VPS cost | full control, per-device auth possible | medium | manifest = version + url + sha256 + sig |
| Managed OTA platform (Golioth, Memfault, ESP RainMaker, hawkBit) | $ per device | fleet-grade: staged rollout, cohorts, metrics | low code / high lock-in | the "real product at scale" answer |

## Decision 2 — Device authentication (if source is private/gated)

| Option | Blast radius if extracted | Rotation | Notes |
|---|---|---|---|
| Fine-grained PAT baked in firmware (gitignored ota_secrets.h) | read-only on ONE repo | manual, 1yr max expiry | simplest; token ships inside every binary |
| PAT in NVS (provisioned per device) | same, but binary itself is clean | per-device | needs a provisioning step (serial/UI) |
| Per-device client certs (mTLS) | one device | per-device revocation | gold standard for fleets; needs PKI + server that speaks mTLS (not GitHub) |
| Short-lived URLs from a backend | ~minutes | n/a | device holds no long-lived secret; backend holds the GitHub/S3 secret |

Key insight: **GitHub PATs cap out at "okay for one device / a few devices."**
A real fleet wants per-device identity → that forces a backend or managed platform.

## Decision 3 — Firmware authenticity (independent of where it's hosted)
- `CONFIG_SECURE_SIGNED_APPS_NO_SECURE_BOOT` + ECDSA app signing:
  device refuses unsigned/wrongly-signed OTA images. Signing key stays offline.
  Reversible, no efuses. **Do this regardless of hosting choice once shipping.**
- Full Secure Boot v2 + Flash Encryption (ESP32-P4): efuse-burning, irreversible,
  protects against physical flash dump + bootloader swap. Product-launch decision;
  test thoroughly on a sacrificial board first.
- Minimum bar even today: HTTPS w/ cert bundle (already done) + esp_https_ota's
  built-in image header validation.

## Decision 4 — Versioning & release pipeline
- Firmware version = git describe (currently e.g. `9ae9d2f-dirty`). Building from a
  tag makes version == tag → device can tell "already on this release".
- Manual today: `gh release create vX.Y.Z build\DeskMediaDevice.bin`
- Product: GitHub Actions on tag push → idf build → sign → attach to release
  (or push to S3/platform). Add sha256 to release notes / manifest.
- Consider A/B staged rollout (release to 10% first) — only managed platforms or a
  custom manifest server do this well.

## Decision 5 — Fleet/product extras (only matter at scale)
- Per-device update channels (stable/beta) — manifest field or platform feature
- Update metrics: who's on what version, failure rates → Memfault-style platform
- Forced/security updates vs user-prompted (current UX: prompt Install/Later)
- Rate limits: GitHub API anonymous = 60 req/h per IP; authenticated = 5000 req/h.
  One device checking daily is nothing; 10k devices need caching/CDN or a platform.

## Recommended sequencing (when launch planning starts)
1. Decide hosting (Decision 1) FIRST — it constrains auth (Decision 2)
2. Add image signing (Decision 3, NO_SECURE_BOOT variant) — cheap, huge win, host-agnostic
3. CI release pipeline (Decision 4)
4. Secure Boot + Flash Encryption call (Decision 3 full) before mass production —
   cannot be retrofitted after units ship
5. Fleet tooling (Decision 5) only when device count justifies it

## What's safe to do NOW without betting on infrastructure
- The implemented OTA code is host-agnostic at its core (esp_https_ota + a URL).
  Swapping GitHub-public → GitHub-private → S3 → platform later only changes the
  "where do I find the latest version + URL" function (`fetch_latest_release()`).
- The partition table change is required for ANY OTA path — no waste there.
- Rollback protection is required for ANY OTA path — no waste there.
