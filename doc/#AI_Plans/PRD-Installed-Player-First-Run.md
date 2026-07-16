# PRD — Installed-Player First Run: Data Discovery, Fail-Soft & File Locations

**Status:** All six phases implemented — Phases 1–3 committed (read search path, fail-soft preflight, config+cache); Phases 4–6 uncommitted (writable per-user layout + first-run seeding; checksum compatibility pass on a dependency-free SHA-256 against a **compiled-in** manifest, authored by the new `lbx_hashes` generator and populated from the v1.31 assets; **log relocation to `XDG_STATE_HOME` for the player build**). **Design notes:** (a) the writable dir is resolved by `STU_GRAF` itself (XDG / `%APPDATA%` / `~/Library`), *not* `SDL_GetPrefPath()` as sketched below — keeps the seam SDL-free; `Platform_Get_User_Data_Dir` isn't needed. (b) The log dir is passed to `STU_LOG` by the caller (ReMoMber) via `STU_Log_Set_Base_Dir`, not resolved inside `STU_LOG`, avoiding an `STU_LOG`↔`STU_GRAF` cycle.
**Owner:** TBD
**Date:** 2026-07-03 (updated 2026-07-16)
**Tracks:** Alpha-quality first-run experience (portable ZIP / installed player build)

> **References:**
> - Provides the **binary-side consumer** of the installer's `[Paths] game_data` config and satisfies the **"Binary-side fail-soft on missing LBX"** dependency in [PRD-Installer-Game-Data-Setup.md](PRD-Installer-Game-Data-Setup.md). Shares that PRD's checksum-manifest concept.
> - **Amends** the "logs live in the CWD" decision in [PRD-Logging-System-And-Curator-Agent.md](PRD-Logging-System-And-Curator-Agent.md) for the *player* build only.

---

## Summary

Make the installed / portable player build "just run" without the user managing directories, without cluttering their filesystem, and without touching the reconstructed game code:

1. **Data-directory discovery (read-only originals).** ReMoMber finds the user's original Master of Magic `.LBX` files via an ordered search path (env → config → next-to-exe → CWD), implemented at the existing `stu_fopen_ci` asset-open seam so the disassembled MoM/MoX code is untouched.
2. **A proper per-user file layout (writable state).** Mutable files — `CONFIG.MOM`, `MAGIC.SET`, save games — live in `XDG_DATA_HOME` (via `SDL_GetPrefPath()`), logs in `XDG_STATE_HOME`, settings in `XDG_CONFIG_HOME`, and any ReMoM-modified data copies in `XDG_CACHE_HOME`. The user's original MoM install is treated as **read-only** and never written to.
3. **Fail-soft + compatibility check.** A startup preflight verifies the required data files are present and (optionally) match a known-good checksum manifest; on failure it shows a clear cross-platform error dialog and exits cleanly instead of crashing.

If in-engine path/writability handling proves too messy across platforms, the documented fallback is a small external **config/launcher app** that resolves everything and launches ReMoMber (see Implementation Notes).

## Problem

Everything resolves relative to the current working directory, the data must sit in that directory, writes land next to read-only originals, and missing data crashes hard:

- Run `ReMoMber` from anywhere without the `.LBX` files present → blank window opens and closes (missing `FONTS.LBX`/`MAINSCRN.LBX` has no fallback, [MoM/src/LOADER.c](../../MoM/src/LOADER.c)); the only trace is a log the player has no reason to know exists.
- After install / portable-ZIP extract, the binary and the data live in different places, so "just run it" fails.
- `MAGIC.SET` and saves are written **next to the data** ([MoM/src/Settings.c](../../MoM/src/Settings.c)); if that's a read-only GOG/Steam install, writes fail.
- The only user-facing dialog is a Win32-only allocation-error `MessageBoxA` ([platform/win32/win_Exit.c:15](../../platform/win32/win_Exit.c#L15)); no cross-platform way to tell the user anything.
- Logs clutter the launch directory ([STU/src/STU_LOG.c:53](../../STU/src/STU_LOG.c#L53)).

## File & Directory Map

The target layout (Linux shown; macOS/Windows via the same XDG roles, resolved by `SDL_GetPrefPath` for the data dir):

| Purpose | XDG role | Linux default | R/W | Mechanism |
|---------|----------|---------------|-----|-----------|
| **Original game data** — `.LBX`, source `CONFIG.MOM` | (user's own) | the user's MoM install dir | **read-only** | discovery search path; never written |
| **Modified / derived data** — if ReMoM must patch LBX etc. | `XDG_CACHE_HOME` | `~/.cache/ReMoM/` | read+write | written by ReMoM; searched **ahead of** originals |
| **Mutable game files** — `CONFIG.MOM` working copy, `MAGIC.SET`, saves | `XDG_DATA_HOME` | `~/.local/share/ReMoM/` | read+write | `SDL_GetPrefPath()` (Win32 fallback) |
| **Settings** — `game_data` path, prefs | `XDG_CONFIG_HOME` | `~/.config/ReMoM/` | read (installer/launcher writes) | `ReMoM.ini` |
| **Logs** — `remom_log_*.txt` | `XDG_STATE_HOME` | `~/.local/state/ReMoM/` | write (player build) | `STU_State_Log_Dir` |

Two distinct resolution rules follow from this:
- **Read-only shipped assets (`.LBX`):** resolved by the discovery **search path**.
- **Mutable per-user files (`CONFIG.MOM`/`MAGIC.SET`/saves):** resolved to the **`XDG_DATA_HOME`** dir (read+write), seeded on first run.

## Goals

- **Discovery** locates read-only game data via an ordered, documented search path so the installed/portable binary boots without a manual `cd`.
- **Minimal original-code impact:** discovery lives in the shared `stu_fopen_ci` wrapper ([ext/stu_compat.h](../../ext/stu_compat.h)); reconstructed MoM/MoX code keeps passing **bare filenames**.
- **Writable files go to `XDG_DATA_HOME`** via `SDL_GetPrefPath()`; the user's original MoM install is never modified.
- **Fail-soft:** never crash solely because data is missing — show a visible, actionable dialog and exit non-zero.
- **Compatibility:** verify the data files against a known-good **checksum manifest** (the project owner holds every MoM version and can author it authoritatively), sharing the installer PRD's manifest format.
- **Logs → `XDG_STATE_HOME`** (player build only); HeMoM/tests/matchup keep CWD for both data and logs.
- **Env overrides:** `REMOM_DATA_DIR` (search path), `REMOM_LOG_DIR` (logs).

## Non-Goals

- **A full settings/config system or launcher UI.** Discovery *reads* `[Paths] game_data`; the installer/launcher writes it.
- **The NSIS installer flow** — [PRD-Installer-Game-Data-Setup.md](PRD-Installer-Game-Data-Setup.md).
- **Solving every cross-platform R/W corner up front.** We take `SDL_GetPrefPath()` as the primary mechanism and address platform writability quirks **as they arise**; the launcher app is the escape hatch if the engine path gets messy.
- **A patch pipeline for non-canonical data.** This PRD reserves `XDG_CACHE_HOME` as the destination for any ReMoM-modified data, but the transform mechanism itself is out of scope (see installer PRD's patch open-question).
- **Mid-session fail-soft** after a successful preflight.

## Functional Requirements

### Data-directory discovery (read-only assets)

1. **Ordered search path** consulted by `stu_fopen_ci` for a bare read-only asset name; first hit wins:
   1. `REMOM_DATA_DIR` env var (override).
   2. `XDG_CACHE_HOME/ReMoM/` (ReMoM-modified copies, if any).
   3. `[Paths] game_data` from the config file (`$XDG_CONFIG_HOME/ReMoM/ReMoM.ini` on Linux; installer-written path on Windows).
   4. The executable's own directory (portable ZIP: data beside the binary).
   5. CWD (legacy/default).
2. **Population API** in the support layer (`stu_data_path_reset()` / `stu_data_path_add(dir)`), called **once** by ReMoMber at startup.
3. **Default = CWD only** when unpopulated (HeMoM, tests, matchup) — `stu_fopen_ci` behaves exactly as today.
4. **Bare filenames unchanged;** resolution happens entirely inside `stu_fopen_ci`. No `MoM/src` or `MoX/src` game-logic edits.

### Writable per-user files

5. **`CONFIG.MOM`, `MAGIC.SET`, and save games resolve to `XDG_DATA_HOME`** (via `SDL_GetPrefPath()`), read+write. The original MoM install is never written to.
6. **First-run seeding — copy originals, then always read the copies.** On first run, seed `XDG_DATA_HOME` by copying `CONFIG.MOM` (and `MAGIC.SET`, if the install ships one) from the discovered game-data dir. Thereafter the engine **always** reads and writes the `XDG_DATA_HOME` copies — never read-through to the originals. Save games are created directly in `XDG_DATA_HOME`. (`MAGIC.SET` is still auto-created with engine defaults there if the install had none — [MoM/src/Settings.c](../../MoM/src/Settings.c).)
7. **`XDG_CACHE_HOME/ReMoM/`** is the write destination if ReMoM ever needs to modify/patch original data; such copies are searched ahead of the originals (FR-1.2).

### Compatibility / checksums

8. **Checksum manifest** of known-good SHA256s covering **every `.LBX` file** (~121 in a full v1.31 install; exact set defined by the manifest) per MoM version, authored authoritatively by the project owner (who holds every version), in the installer PRD's `lbx-hashes.txt` format (shared, single source).
9. **Preflight compatibility pass** (non-blocking, after the presence pass): hash **every** discovered `.LBX` file and compare against the manifest; if any are unrecognized or match a wrong/unsupported version, warn via the dialog/log (do not hard-block — mirrors the installer's "continue anyway" posture). Presence failure remains blocking.

> **Implemented deviation (2026-07-16):** the *runtime* manifest is **compiled into the binary** (`STU/src/lbx_manifest.c`, `extern g_lbx_manifest`), not read from an external `lbx-hashes.txt` — a runtime file can go missing/drift/tamper, and the v1.31 hashes belong with the release. The shared piece is now the `(name, sha256, version-tag)` **schema** and the **`lbx_hashes` generator** (`tools/`), not a single file. The installer keeps its own text manifest for install-time validation. An empty compiled-in table = silent no-op, so the owner authors it (one `lbx_hashes` run per MoM distribution) without any stock build false-warning.

### Fail-soft preflight + dialog

10. **Preflight** `STU_Preflight_Check_Game_Data(...)` verifies the required set (`FONTS.LBX`, `MAINSCRN.LBX`, … — aligned with the installer PRD) resolves via the search path. Home: the TODO at [STU/src/STU_INIT.c:87](../../STU/src/STU_INIT.c#L87).
11. **Call site:** ReMoMber `main()`, after `STU_Log_Startup()` ([src/ReMoM.c:225](../../src/ReMoM.c#L225)) and search-path population, before the first asset load.
12. **On missing data:** log `ERROR`, call `Platform_Show_Error(...)`, return non-zero — no crash.
13. **New platform API** `void Platform_Show_Error(const char * title, const char * message);` — `SDL_ShowSimpleMessageBox` (SDL2/SDL3, no window needed), `MessageBoxA` (Win32), `stderr` (headless). Safe to call before a window exists.

### Log relocation (player build only)

14. **Base-dir precedence** in `STU_LOG`: `REMOM_LOG_DIR` → caller-supplied dir (ReMoMber passes the state dir) → CWD (default).
15. **`STU_GRAF_User_State_Dir()`** (implemented name) resolves `$XDG_STATE_HOME/ReMoM/` (Linux) / `%LOCALAPPDATA%\ReMoM\logs\` / `~/Library/Logs/ReMoM/`, creating it; ReMoMber passes it to `STU_Log_Set_Base_Dir()` and the 3-file rotation runs within it. Falls back to CWD on failure.

## Acceptance Criteria

<!-- Phase mapping: 1=STU_GRAF+read; 2=fail-soft; 3=config+cache; 4=writable+seeding; 5=checksums; 6=log relocation. Phases 1-3 committed (bcc2a9b7 / d827d9be / 56e2680a). -->

- [ ] Game data **only** next to the exe (not CWD) → `ReMoMber` from an unrelated CWD boots to the title screen. *(P1 — mechanism in place; data-complete GUI boot not run locally)*
- [ ] `REMOM_DATA_DIR=<dir>` and `[Paths] game_data=<dir>` each direct discovery to `<dir>`. *(P1/P3 — `game_data` half unit-verified `InitUsesConfigGameData`; a REMOM_DATA_DIR-positive test is the gap)*
- [x] `MAGIC.SET` and a new save are written under `~/.local/share/ReMoM/` (or the OS `SDL_GetPrefPath` location), **not** into the user's read-only MoM install. *(P4 — via `STU_GRAF_Open_User` → `XDG_DATA_HOME/ReMoM`, resolved in STU (not SDL); mechanism unit-proven, save sites swapped; GUI save-cycle not run locally)*
- [x] With a read-only game-data dir, saving still succeeds (writes go to the data dir, not the source). *(P4 — writes target the user-data dir, disjoint from the read-only source; unit-proven source-untouched)*
- [x] On first run, `CONFIG.MOM` is copied into `XDG_DATA_HOME` and read from there on later runs; the original in the game-data dir is never modified. *(P4 — `STU_GRAF_Seed_User_File` unit-proven + wired at PLAYER startup)*
- [x] No data discoverable anywhere → GUI dialog names the missing files + fix, exits non-zero, no crash; HeMoM prints the same to stderr. *(P2 — verified end-to-end on both builds; ctest `HeMoM_Preflight_Missing_Data`)*
- [x] Data files whose checksums don't match any manifest entry → a non-blocking "unrecognized/……" warning (presence still passing). *(P5 — `STU_GRAF_Check_Data_Compat` over the **compiled-in** `g_lbx_manifest` on a verified SHA-256; unit-proven; silent until the manifest is authored via the `lbx_hashes` generator)*
- [x] `HeMoM`/`ctest`/matchup still resolve data from CWD and write `remom_log_*.txt` to CWD ([tools/log-tools/log_triage.py](../../tools/log-tools/log_triage.py), [tools/parity_check.py](../../tools/parity_check.py) unaffected). *(P1/P6 — end-to-end: HeMoM with `XDG_STATE_HOME` set still logged to CWD, 0 in the state dir; `STU_LOG` default (no `Set_Base_Dir`) = CWD, byte-identical)*
- [x] Player-build logs land under `~/.local/state/ReMoM/`, not CWD. *(P6 — end-to-end: ReMoMber `--headless` wrote its log under `XDG_STATE_HOME/ReMoM/`, 0 in its CWD; unit-tested)*
- [x] No `MoM/src` / `MoX/src` game-logic file is modified for discovery. *(git-verified: only `MoX/src/LBX_Load.c` open-call plumbing changed)*

## Implementation Notes

### The seam — why original-code impact is minimal

`stu_fopen_ci` ([ext/stu_compat.h](../../ext/stu_compat.h)) is the case-insensitive asset-open wrapper — **support-layer, not disassembled game code** — and already scans directories to match names case-insensitively. Discovery extends it to walk the search path, applying the existing case-insensitive match per dir. Verified funnels: LBX loaders open every `.LBX` via `stu_fopen_ci(..., "rb")` ([MoX/src/LBX_Load.c](../../MoX/src/LBX_Load.c) lines 234/446/710, with handle caching); `CONFIG.MOM`/`MAGIC.SET`/saves also go through `stu_fopen_ci`.

### `SDL_GetPrefPath()` and the Win32 fallback

`SDL_GetPrefPath("ReMoM", "ReMoM")` returns the writable per-user data dir — `XDG_DATA_HOME` on Linux, `~/Library/Application Support/…` on macOS, `%APPDATA%\…` on Windows — and creates it. Use it on the SDL2/SDL3 backends. The **Win32 backend links no SDL**, so it needs a native fallback (`SHGetKnownFolderPath(FOLDERID_RoamingAppData)` or `%APPDATA%`). Expose one `Platform_Get_User_Data_Dir()` that each backend implements. (Note the deliberate split: logs → `%LOCALAPPDATA%`/state, mutable data → `%APPDATA%`/roaming.)

### Read family vs. write family

Two path families must not be conflated: the **read search path** (originals, read-only) and the **write/mutable target** (`XDG_DATA_HOME`; derived copies in `XDG_CACHE_HOME`). `MAGIC.SET` is read *and* written — it reads from and writes to the data dir, seeded from the original on first run. This is the crux that keeps a read-only GOG/Steam install working.

### Launcher-app escape hatch

If in-engine discovery + seeding + writability across Win32/SDL/macOS becomes too messy, pivot to a small **config/launcher app**: it picks the MoM data dir, runs checksum validation, seeds `XDG_DATA_HOME`, writes `ReMoM.ini`, and launches `ReMoMber` with the resolved paths (env/config). Keeps the engine dumb. Recorded as a deliberate alternative, chosen if the coupled changes prove invasive.

### Log-dir + mkdir portability

Recursive-mkdir helper; `getenv("XDG_STATE_HOME")` + `HOME + "/.local/state"` fallback (Linux), `%LOCALAPPDATA%` (Win), `HOME + "/Library/Logs"` (macOS). In `STU/src/` so `STU_LOG` needs no platform backend.

### Player vs. dev/test

Per-executable at runtime, not a build macro (shared `libstu`). ReMoMber populates the data search path + data/log dirs; HeMoM does neither (defaults → CWD). Env overrides win.

## Risks / Open Questions

- **Cross-platform writability** — handled **as it arises** per the user's call; `SDL_GetPrefPath` is the primary bet, Win32 native fallback the known gap. Launcher app is the fallback if it gets messy.
- **[Decided] Seeding = copy-then-always-read-copy.** First run copies `CONFIG.MOM`/`MAGIC.SET` into `XDG_DATA_HOME`; the engine always reads the copies, never read-through. Remaining sub-point: how "first run" is detected — absence of the copy, or an explicit version stamp so re-seeding triggers if the source install changes.
- **[Decided] Hash every `.LBX` (~121), not a subset.** This differs from the installer PRD's suggested 3–5 high-signal subset (chosen there for install-time speed). Reconcile: the **shared manifest covers all LBX**; the installer may still spot-check a subset at install time, but runtime compat hashes them all. Align filename/columns so both consume one file.
- **`stu_fopen_ci` multi-dir scan cost** — amortized by LBX handle caching; confirm no re-open storms.
- **Early `SDL_ShowSimpleMessageBox`** before video init — verify per backend; stderr fallback.
- **AppImage exe-dir** points into the mounted squashfs (has the binary, not user data) — fine, lower-priority CWD/config still apply.
- **`STU_Log_Startup` signature change** ripples to HeMoM + `STU_LOG` tests; a `Set_Base_Dir()` side-channel avoids it.

## Dependencies

- New platform APIs: `Platform_Show_Error()`, `Platform_Get_User_Data_Dir()`, exe-dir resolver — across sdl2/sdl3/win32/headless.
- `stu_fopen_ci` search-path extension + population API in `ext/stu_compat.h`.
- Shared **checksum manifest** with [PRD-Installer-Game-Data-Setup.md](PRD-Installer-Game-Data-Setup.md).
- Builds on `STU_LOG` ([PRD-Logging-System-And-Curator-Agent.md](PRD-Logging-System-And-Curator-Agent.md)).

## Testing Decisions

- **Discovery:** temp-tree unit tests — file only in exe-dir → resolves; `REMOM_DATA_DIR` wins; cache-dir copy shadows original; empty → open fails; unpopulated → CWD only.
- **Write family:** with a read-only game-data dir, `MAGIC.SET`/save writes land in the `XDG_DATA_HOME`-equivalent temp dir, not the source.
- **Preflight/compat:** missing required file → blocking failure + list; mismatched checksum → non-blocking warning; good tree → pass.
- **Log base-dir:** `REMOM_LOG_DIR` wins; explicit base honored; default CWD; rotation in resolved dir.
- **`Platform_Show_Error` headless** asserts on stderr; GUI variants manual per backend.
- **Regression guard:** a HeMoM CI run still resolves data from CWD and emits logs to CWD.

## Out of Scope

- Installer UI; launcher app *implementation* (recorded as a fallback, not committed).
- Data-patch/transform pipeline (only the `XDG_CACHE_HOME` destination is reserved).
- Full settings system; version/hash *enforcement* (compat is advisory here).
- Mid-session fail-soft; network/remote anything.
