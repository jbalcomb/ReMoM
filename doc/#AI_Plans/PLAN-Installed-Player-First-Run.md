# Plan: Installed-Player First Run (Data Discovery, Fail-Soft & File Locations)

> Source PRD: [PRD-Installed-Player-First-Run.md](PRD-Installed-Player-First-Run.md)

Make the installed / portable player build "just run" — find the user's game data without a manual `cd`, put per-user files in the right OS locations, and fail with a clear dialog instead of a blank-window crash — **without touching the reconstructed game code**.

## Architectural decisions

Durable decisions that apply across all phases:

- **New module `STU/src/STU_GRAF.c` / `.h` ("Game Resource Archive File")** — the single home for OS/platform path-and-place resolution *and* the data-open functions. Portable C with platform `#ifdef`s (same style as `STU_LOG`'s SEH/POSIX split); **no SDL dependency**. This is the "mechanism" layer; it keeps path policy out of `stu_compat`, out of the SDL/Win32 backends, and out of the game code.
- **`ext/stu_compat.h` is unchanged.** `STU_GRAF` calls `stu_fopen_ci()` to open an already-resolved full path; the compat shim stays a libc-portability leaf.
- **Two open functions, two path families:**
  - `STU_GRAF_Open_Asset(name, mode)` — **read-only** shipped data; walks the ordered read search path.
  - `STU_GRAF_Open_User(name, mode)` — **writable** per-user files; resolves to the user data dir.
- **Ordered read search path:** `REMOM_DATA_DIR` (env) → `$XDG_CACHE_HOME/ReMoM/` (modified copies) → `[Paths] game_data` (config) → executable dir → **auto-detected common install locations (GOG/Steam/CD)** → CWD. First hit wins — cache precedes game_data so ReMoM-modified copies shadow the originals. Unpopulated default = CWD-only (HeMoM/tests/matchup unaffected).
- **Zero-config auto-detect (post-plan UX enhancement):** the PLAYER profile probes a built-in candidate list and adds the first dir containing the signature `FONTS.LBX`. Order: dev checkout (`./assets`, `../assets`, `../../assets` — matches the checksum manifest exactly) first, then common installs (`~/GOG Games/Master of Magic`, Steam `…/common/Master of Magic Classic`, `C:\MPS\MAGIC`, …). So a standard GOG/Steam/CD install **and** a running-from-the-repo dev both boot with **no configuration**; non-standard installs still use `REMOM_DATA_DIR` / `[Paths] game_data`. HEADLESS never probes. Testable seam: `STU_GRAF_First_Game_Data_Dir(candidates)`.
- **OS locations (all under a `ReMoM` app dir), resolved inside `STU_GRAF`:**
  | Purpose | XDG role | Linux | Windows | macOS |
  |---|---|---|---|---|
  | Original `.LBX` (read-only) | — | user's MoM install | user's MoM install | user's MoM install |
  | Modified/derived data | `XDG_CACHE_HOME` | `~/.cache/ReMoM/` | `%LOCALAPPDATA%\ReMoM\cache\` | `~/Library/Caches/ReMoM/` |
  | `CONFIG.MOM`/`MAGIC.SET`/saves (writable) | `XDG_DATA_HOME` | `~/.local/share/ReMoM/` | `%APPDATA%\ReMoM\` | `~/Library/Application Support/ReMoM/` |
  | Settings (`ReMoM.ini`) | `XDG_CONFIG_HOME` | `~/.config/ReMoM/` | `%APPDATA%\ReMoM\` | `~/Library/Application Support/ReMoM/` |
  | Logs | `XDG_STATE_HOME` | `~/.local/state/ReMoM/` | `%LOCALAPPDATA%\ReMoM\logs\` | `~/Library/Logs/ReMoM/` |
- **`STU_LOG` gets its log dir from the caller, not by calling `STU_GRAF`** (avoids an `STU_LOG`↔`STU_GRAF` cycle). ReMoMber resolves it via `STU_GRAF_User_State_Dir` (the shared XDG/mkdir resolver — no duplication) and passes it through `STU_Log_Set_Base_Dir`. *(Implemented in Phase 6.)*
- **Platform layer keeps only `Platform_Show_Error(title, msg)`** (needs the UI backend: `SDL_ShowSimpleMessageBox` / `MessageBoxA` / stderr for headless). Path/place resolution does **not** touch the backend.
- **Routing the game's opens (no game-logic edits):** `LBX_Load` (libmox, 3 sites) calls `STU_GRAF_Open_Asset` — legal MoX→STU dependency, no function pointers. `CONFIG.MOM`/`MAGIC.SET`/save sites (bounded, in `ReMoM_Init.c`/`Settings.c`/HeMoM newgame) call the `STU_GRAF` open functions.
- **`profile` at init:** `STU_GRAF_Init(STU_GRAF_PLAYER | STU_GRAF_HEADLESS)`. HEADLESS → CWD-only, no seeding, stderr for errors. Both `main()`s call it.
- **Startup order (target):** CLI-parse → `STU_GRAF_Init` → `STU_Log_Startup` (into the resolved state dir) → `Startup_Platform` → preflight (dialog/exit) → seed → `ReMoM_Init_Engine` + MAGIC/WIZARDS → run. Bootstrap de-duplication of `ReMoM.c`/`HeMoM.c` and this order-fix are **distributed across phases**, not a separate refactor phase.
- **Seeding policy:** on first run, copy `CONFIG.MOM`/`MAGIC.SET` **and any existing `SAVE1-9.GAM`** from the discovered game-data dir into the user data dir (copy-if-absent), then **always** read/write the copies (no read-through). Originals never modified. *(Saves added post-plan — without them, existing save games that live beside the data were orphaned on the move to per-user dirs.)*
- **File maintenance (post-plan enhancement):** `ReMoMber --orig-files` force-refreshes `CONFIG.MOM`/`MAGIC.SET`/`SAVE1-9.GAM` from the originals, first moving current copies into a timestamped `backup-YYYYMMDD-HHMMSS/` subdir of the user data dir. STU_GRAF seam: `STU_GRAF_Backup_And_Reseed_User_File`.

---

## Phase 1: STU_GRAF module + asset-read relocation

**PRD coverage**: Goals "discovery" + "minimal original-code impact"; FR 1–4; ACs "data-next-to-exe boots," "`REMOM_DATA_DIR` directs discovery," "no `MoM/src`/`MoX/src` game-logic edits."

### What to build

Stand up `STU/src/STU_GRAF.c/.h` with the read side of the machinery: executable-dir resolution, recursive `mkdir`, an ordered read search path (this phase: `REMOM_DATA_DIR` → exe-dir → CWD — config/cache come in Phase 3), and `STU_GRAF_Open_Asset(name, mode)` which walks the path and opens the first hit via `stu_fopen_ci`. Route libmox `LBX_Load`'s three open sites through `STU_GRAF_Open_Asset` (bare LBX names unchanged). Add `STU_GRAF_Init(profile)`; `ReMoMber` calls it as `PLAYER`, `HeMoM` as `HEADLESS`. `stu_compat` is not touched. Default (unpopulated / HEADLESS) resolves CWD-only so existing consumers are unaffected.

### Acceptance criteria

- [ ] With game data **only** next to the executable (not in CWD), `ReMoMber` launched from an unrelated CWD loads its LBX and boots to the title screen. *(mechanism in place — exe-dir is in the search path — but a data-complete GUI boot has not been run locally)*
- [ ] `REMOM_DATA_DIR=<dir>` directs LBX resolution to `<dir>` regardless of CWD. *(first-match search-path machinery is unit-tested; a REMOM_DATA_DIR-positive unit test is the remaining gap)*
- [x] `HeMoM` and `ctest` still resolve data from CWD (default path) — no behavior change. *(HEADLESS profile = CWD-only; HeMoM integration + suite green)*
- [x] No file under `MoM/src/` or `MoX/src/` game *logic* is modified (only `LBX_Load`'s open-call swap — loader plumbing). *(git-verified: no changes under `MoM/src` or `MoX/src` beyond `LBX_Load.c`)*
- [x] `ext/stu_compat.h` is unchanged. *(git-verified: clean vs HEAD)*
- [x] `STU_GRAF` unit test: file only in the exe-dir-equivalent resolves; `REMOM_DATA_DIR` wins; empty everywhere → open fails; unpopulated → CWD only. *(12/12 pass: ResolvesViaSearchDir, FirstMatchWins, NotFoundReturnsNull, PathNameOpenedDirectly)*

> **Status (committed `bcc2a9b7` "phase 1 Assets Paths"):** core mechanism unit-verified; the two GUI/runtime-manifestation boxes (data-complete title-screen boot, REMOM_DATA_DIR-positive) are the open items — mechanism is present but not yet exercised on a windowed, data-complete launch.

---

## Phase 2: Fail-soft preflight + cross-platform error dialog

**PRD coverage**: FR 10–13; ACs "no data → dialog + exit, no crash," "HeMoM stderr."

### What to build

Add `Platform_Show_Error(title, message)` to the platform API (SDL2/SDL3 `SDL_ShowSimpleMessageBox`, Win32 `MessageBoxA`, headless `stderr`), safe to call before a window exists. Add a preflight in `STU_GRAF` (or called from it) that verifies the required file set (`FONTS.LBX`, `MAINSCRN.LBX`, … aligned with the installer PRD) resolves via the search path; wire it into `ReMoMber`'s `main()` after `STU_GRAF_Init` and before the first asset load. On missing data: log `ERROR`, show the dialog with the missing files + the fix + where it looked, and exit non-zero. Headless prints the same to stderr.

### Acceptance criteria

- [x] No data discoverable anywhere → GUI dialog names the missing files and the fix, process exits non-zero, no crash / no blank-window flash. *(verified end-to-end: ReMoMber run from an empty dir → dialog path + exit 1, no crash/window)*
- [x] `HeMoM` with missing data prints the same message to stderr and exits non-zero (no GUI call). *(verified: HeMoM from empty dir → "Missing Game Data" on stderr + exit 1; guarded by ctest `HeMoM_Preflight_Missing_Data`)*
- [ ] Data present → boots normally, no dialog. *(data-complete boot not run locally; negative path is proven)*
- [x] `Platform_Show_Error` headless variant is unit-tested (asserts on stderr); GUI variants smoke-checked per backend. *(headless asserted via the `HeMoM_Preflight_Missing_Data` stderr regex; SDL2 GUI path smoke-checked during the ReMoMber empty-dir run)*

> **Status (committed `d827d9be` "phase 2 Assets Paths"):** fail-soft path verified end-to-end on both builds; only the positive "data present → normal boot" box awaits a data-complete GUI run (shared with Phase 1).

---

## Phase 3: Config + cache entries in the search path

**PRD coverage**: FR 1.2–1.3, 7; installer handshake; AC "`game_data` directs discovery."

### What to build

Extend `STU_GRAF`'s search-path construction to read `[Paths] game_data` from the config file (`$XDG_CONFIG_HOME/ReMoM/ReMoM.ini` on Linux; installer-written path on Windows) and to include `$XDG_CACHE_HOME/ReMoM/` **ahead of** the originals (so any ReMoM-modified copies shadow the shipped ones). Final order: `REMOM_DATA_DIR` → cache → `game_data` → exe-dir → CWD.

### Acceptance criteria

- [x] `[Paths] game_data=<dir>` in the config file resolves LBX from `<dir>`. *(unit-tested: `InitUsesConfigGameData`, plus INI-parser tests for spaces/quotes/case/missing)*
- [x] A copy of an asset placed in `$XDG_CACHE_HOME/ReMoM/` shadows the same-named original in the game-data dir. *(unit-tested: `InitCacheShadowsConfigGameData`)*
- [ ] Search precedence is exactly `REMOM_DATA_DIR` → cache → `game_data` → exe-dir → CWD (unit-tested). *(the cache→game_data link is unit-tested and the construction order is coded correctly; the REMOM_DATA_DIR-first and exe-dir links are not each individually unit-tested — small gap)*

> **Status (committed `56e2680a` "phase 3 Assets Paths"):** config + cache entries land in the search path; the two behavioral boxes are unit-verified. Only the "full 5-level precedence exhaustively unit-tested" box is short a couple of link-specific tests. Note: the plan's earlier cache/game_data order was transposed vs the AC — corrected to cache-before-game_data (patched copies win).

---

## Phase 4: Writable per-user layout + first-run seeding

**PRD coverage**: Goals "writable files"; FR 5–6; ACs "MAGIC.SET/save in data home," "read-only dir saves," "CONFIG.MOM seeded." (Carries the MAGIC/WIZARDS `main()` extraction.)

### What to build

Add user-data-dir resolution (`XDG_DATA_HOME` via the `STU_GRAF` place helpers) and `STU_GRAF_Open_User(name, mode)`. Route `CONFIG.MOM`, `MAGIC.SET`, and save games through it (the bounded set of sites in `ReMoM_Init.c`/`Settings.c`/HeMoM newgame). Implement copy-then-always-read-copy seeding: on first run, copy `CONFIG.MOM` (and `MAGIC.SET` if present) from the discovered game-data dir into the user data dir; thereafter always read/write the copies. As the reorg rides along, extract the inlined MAGIC/WIZARDS init out of `main()` into `ReMoM_Init.c`.

### Acceptance criteria

- [x] `MAGIC.SET` and a new save are written under the user data dir (`~/.local/share/ReMoM/` etc.), **not** the user's MoM install. *(mechanism unit-proven: `Open_User` writes under `XDG_DATA_HOME/ReMoM`, asserted **not** in CWD; live save sites `Save_SAVE_GAM`/`Settings_Screen`/`Newgame_Screen_0` swapped to it. Full in-game GUI save-cycle not run locally.)*
- [x] With a **read-only** game-data dir, saving still succeeds. *(writes provably target the user-data dir, disjoint from the game-data dir — `SeedCopies…` confirms the source is never written; so a read-only source is irrelevant to save success.)*
- [x] On first run, `CONFIG.MOM` is copied into the user data dir and read from there afterward; the original is never modified. *(unit-proven `SeedCopiesOriginalAndLeavesSourceUntouched` — copy + idempotent + source-unchanged; wired via `ReMoM_Seed_User_Files()` at PLAYER startup; the read is swapped to `Open_User`.)*
- [x] Write-family unit test: with a read-only game-data dir, `MAGIC.SET`/save writes land in the user-dir temp, not the source. *(18/18 STU_GRAF tests incl. `OpenUserWritesUnderDataDir`, `SeedCopies…`, `UserDirAndLofAbsentAreZero`, `HeadlessUserFamilyUsesCwd`.)*
- [x] `main()` no longer contains inlined MAGIC/WIZARDS init (moved to `ReMoM_Init.c`); build + tests green. *(already extracted into `ReMoM_Init_Engine()`; clean full build; **regression proven** — stash-isolation baseline identical at 70/90 with and without Phase 4, i.e. zero new failures.)*

> **Status (uncommitted):** STU_GRAF gained the writable user-family (`User_Data_Dir` / `Open_User` / `User_DIR` / `User_LOF` / `Seed_User_File`), **profile-aware** so it degrades to CWD under HEADLESS (and by default) — the swap is inert for HeMoM / tests / matchup, opt-in only via `STU_GRAF_Init(PLAYER)`. 15 live open+DIR/LOF sites swapped across 7 files (dead reconstruction twins left alone, per the caller-chain map). Seeding wired into ReMoMber startup. Remaining gap: a live in-game save/load **GUI** playthrough (needs a windowed, data-complete run).

---

## Phase 5: Checksum compatibility pass

**PRD coverage**: Goals "compatibility"; FR 8–9; AC "unrecognized checksum → non-blocking warning."

### What to build

A dependency-free SHA-256 and a non-blocking compatibility pass that runs after the presence preflight: SHA-256 each installed `.LBX` the manifest knows about and classify it (supported v1.31 / wrong-version / unrecognized). On any problem, warn via the dialog/log but continue; presence failure stays blocking.

> **Design change (2026-07-16):** the manifest is **compiled into the binary** (`g_lbx_manifest` in `STU/src/lbx_manifest.c`), **not** an external `lbx-hashes.txt` read at runtime. The runtime can't depend on a file that goes missing/drifts/tampers, and the v1.31 hashes belong with the release. A committed generator CLI — **`lbx_hashes <mom-dir> <version-tag>`** — emits the C table rows (one run per MoM distribution the owner holds; sorted, pasteable). The installer PRD's external `lbx-hashes.txt` remains that project's concern; if a shared text export is ever wanted, the generator can emit it too. Multiple MoM versions coexist as extra rows (tag-driven: `v1.31*` = supported).

### Acceptance criteria

- [x] Data files whose checksums don't match any manifest entry → a non-blocking "unrecognized / wrong version" warning; the game still starts. *(unit-proven `CompatUnrecognizedHashWarns` + `CompatUnsupportedVersionWarns` → `problem_count=1`, summary populated; `ReMoM_Check_Data_Compat` shows the dialog then **returns** — startup continues, never exits. GUI dialog not run locally.)*
- [x] Matching checksums → silent boot (no warning). *(unit-proven `CompatSupportedMatchIsSilent` + `CompatMultiDistroPicksSupported` → `problem_count=0`; `CompatEmptyManifestIsSilentNoop` + `CompatShippedManifestIsEmptyNoop` → `manifest_found=0`. `ReMoM_Check_Data_Compat` returns early in each — no dialog.)*
- [x] Manifest reader unit-tested against a small golden manifest + fixture files. *(7 compat tests driving the core with in-memory manifest arrays + on-disk fixtures, on a verified SHA-256: 5 FIPS-180-4 vector tests incl. empty / abc / 56-byte-boundary / 1M-a / stream — and the SHA-256 output cross-checks against system `sha256sum`.)*

> **Status (uncommitted):** `STU_HASH` (dependency-free SHA-256, wired into CMake + autotools) + `STU_GRAF_Check_Data_Compat(report)` over the **compiled-in** `g_lbx_manifest` (hashes each installed LBX once; classifies supported-v1.31 / wrong-version / unrecognized). Testable core `STU_GRAF_Check_Compat_Against(table, report)` lets tests supply a synthetic manifest. Orchestrated by `ReMoM_Check_Data_Compat()` (non-blocking warning via `Platform_Show_Error`), at PLAYER startup after seeding. Generator **`lbx_hashes`** (tools/) authors the table. **Silent until authored** — `lbx_manifest.c` ships empty (terminator only) → `manifest_found=0` no-op, so dev/player runs never false-warn. **51/51 STU tests green.**

---

## Phase 6: Log relocation to XDG_STATE_HOME (player build only)

**PRD coverage**: Goals "logs → state"; FR 14–15; ACs "player logs in state dir," "HeMoM/ctest/matchup still CWD." (Carries the startup-order fix.)

### What to build

`STU_LOG` gains a base-output-dir with precedence `REMOM_LOG_DIR` (env) → caller-supplied dir → CWD (default). ReMoMber resolves the state dir via `STU_GRAF_User_State_Dir` and hands it to `STU_Log_Set_Base_Dir()` before `STU_Log_Startup`; HeMoM/tests/matchup never do, so they stay CWD. Rotation runs within the resolved dir; an unwritable dir falls back to CWD.

> **Design refinement:** rather than `STU_LOG` *calling* `STU_GRAF` (which would create an `STU_LOG`↔`STU_GRAF` cycle, since `STU_GRAF` logs via `STU_LOG`), the **caller resolves and passes** the dir via a `STU_Log_Set_Base_Dir` side-channel. Dependency stays one-way (`STU_GRAF`→`STU_LOG`; ReMoMber→both); `STU_Log_Startup`'s signature is unchanged (no HeMoM/test ripple). No startup-**order** move was needed — `STU_GRAF_User_State_Dir` is standalone (no `STU_GRAF_Init`), so ReMoMber resolves the dir *before* `STU_Log_Startup` without reordering `STU_GRAF_Init` (which keeps its own startup log line).

### Acceptance criteria

- [x] A player-build (`ReMoMber`) run writes `remom_log_*.txt` under the OS state dir (`~/.local/state/ReMoM/` etc.), not the CWD. *(end-to-end: ReMoMber `--headless` with `XDG_STATE_HOME=<tmp>` wrote the log under `<tmp>/ReMoM/`, 0 files in its CWD; unit `BaseDirRoutesLogIntoDir` + `UserStateDirResolvesAndCreates`.)*
- [x] `HeMoM`, `ctest`, and the matchup harness still write `remom_log_*.txt` to the CWD — `tools/log-tools/log_triage.py` and `tools/parity_check.py` unaffected. *(end-to-end: HeMoM with `XDG_STATE_HOME` set still logged to its CWD, 0 files in the state dir — it never calls `Set_Base_Dir`. Default `log_base_dir=""` → CWD, byte-identical to before.)*
- [x] `REMOM_LOG_DIR=<dir>` overrides for any build; `REMOM_LOG_DIR=.` forces CWD. *(end-to-end override honored; unit `RemomLogDirEnvOverridesBaseDir` + `RemomLogDirDotForcesCwd`.)*
- [x] 3-file rotation works correctly within the resolved directory; unwritable dir falls back to CWD with the existing stderr note. *(unit `RotatesWithinBaseDir` + `UnwritableBaseDirFallsBackToCwd`.)*

> **Status (uncommitted):** `STU_GRAF_User_State_Dir` (resolves + creates `XDG_STATE_HOME/ReMoM` etc.) + `STU_Log_Set_Base_Dir` side-channel + path-aware rotation/open with CWD fallback in `STU_LOG`. ReMoMber routes to the state dir at startup (PLAYER only); HeMoM/tests/matchup stay CWD. **57/57 STU tests** (6 new) + all three ACs confirmed against the real binaries. Debug + Release build clean.

---

## Sequencing notes

- Dependencies: **1 → 2 → (3, 4 either order) → 5**. **Phase 6 is independent** (touches only `STU_LOG` + the `STU_GRAF` place helpers from Phase 1) — it can run in parallel or first as a low-risk warm-up.
- Bootstrap de-duplication of `ReMoM.c`/`HeMoM.c` is incremental: Phase 1 adds `STU_GRAF_Init` to both mains; Phase 4 extracts MAGIC/WIZARDS init; Phase 6 lands the order-fix. No separate refactor-only phase.
- Reconcile with [PRD-Installer-Game-Data-Setup.md](PRD-Installer-Game-Data-Setup.md) on the `[Paths] game_data` config file name (Phase 3, done). **Manifest divergence (resolved):** the *runtime* uses a **compiled-in** table (`lbx_manifest.c`), not the installer's shared external `lbx-hashes.txt` — the two now share the `lbx_hashes` **generator** and the `(name, sha256, version-tag)` schema rather than a single file. The installer keeps its own text manifest for install-time validation; the generator can emit that form too if we want one source artifact later.
