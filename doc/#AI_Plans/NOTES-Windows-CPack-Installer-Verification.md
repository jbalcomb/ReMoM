# NOTES — Windows CPack Installer Build & Test Verification

**Status:** In progress
**Owner:** Jim (Windows box) + Claude
**Date started:** 2026-07-22
**Goal:** Make sure the Windows CPack installer (NSIS `.exe` + portable ZIP) actually
builds and installs/runs correctly. The usual release agent runs on **Linux** and
cannot exercise the NSIS installer or a Windows install; this machine can, so this
doc captures that verification pass.

---

## Reference materials (where the packaging lives)

| File | Role |
|------|------|
| `CMakeLists.txt` L273–339 | Engine-only `install()` rules + all CPack config (generators, NSIS names, source-ignore). |
| `.github/workflows/release.yml` | Tag-driven Windows job: configure `MSVC-release` w/ `-DCMAKE_INSTALL_BINDIR=.`, build, test, `choco install nsis`, `--target package`, upload `ReMoM-*.zip` / `ReMoM-*.exe`. |
| `.github/workflows/release-check.yml` | Per-push gate: configure/build/test `MSVC-release` only. **No packaging.** |
| `CMakePresets.json` | `MSVC-release` preset (VS2022 x64, SDL2 backend). No package preset sets `CMAKE_INSTALL_BINDIR`. |
| `RELEASES.md` | Human runbook. Says Windows ships **SDL2** backend, self-contained (bundles SDL2.dll / SDL2_mixer.dll / MSVC runtime). |
| `doc/#AI_Plans/PRD-Installer-Game-Data-Setup.md` | Future NSIS enhancement (LBX path prompt + hash validation). **Not yet implemented.** |
| `tools/test_cpack_ignore.cmake` | Test for the source-package ignore regex (source pkg is not shipped). |

## What the CPack config produces (from CMakeLists.txt)

- `CPACK_PACKAGE_NAME = ReMoM`
- `CPACK_PACKAGE_FILE_NAME = ReMoM-${REMOM_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}`
- Windows generators: `ZIP;NSIS`
- NSIS: `CPACK_NSIS_PACKAGE_NAME=ReMoM`, display `ReMoM <ver>`, install dir `ReMoM`,
  finish-page run target = `ReMoMber.exe`.
- Installed payload (engine-only): `ReMoMber.exe`, `SDL2.dll`, `SDL2_mixer.dll`,
  MSVC runtime DLLs (`InstallRequiredSystemLibraries`), `PLAYING.md`.
- **No** LBX/game data, saves, test fixtures, or config templates.

## Local environment (this Windows box, 2026-07-22)

- CMake / CPack: **4.2.3**
- NSIS: **v3.12** at `C:\Program Files (x86)\NSIS\makensis.exe` (not on PATH)
- SDL2 dev libs present: `C:/devellib/SDL2-2.32.2`, `C:/devellib/SDL2_mixer-2.8.1`
- Existing build: `out/build/MSVC-release/bin/Release/` has `ReMoMber.exe`,
  `SDL2.dll`, `SDL2_mixer.dll` (staged 2026-07-22).

---

## Findings

### F1 — Local build was NOT configured the way CI packages it
CI configures with `-DCMAKE_INSTALL_BINDIR=.` (flat layout: `ReMoMber.exe` at package
root). The local `MSVC-release` preset leaves `CMAKE_INSTALL_BINDIR` at the
GNUInstallDirs default (`bin`), so a local `--target package` would put the exe under
`bin/`. To reproduce the CI package faithfully, reconfigure with the same flags before
packaging. *(To verify below.)*

### F2 — PRD is stale on which binary ships (documentation only)
`PRD-Installer-Game-Data-Setup.md` L140 says the installer ships the `USE_WIN32=TRUE`
(no-SDL) binary. That predates the switch to SDL2-as-default; `release.yml` now ships
the **SDL2** backend with DLLs bundled. Not a build blocker — a doc-drift note.

### F3 — **Stale `USE_WIN32=TRUE` cache silently builds the WRONG backend locally** ⚠️ [FIXED]

**Fix applied (2026-07-22):** pinned `USE_WIN32=FALSE` and `USE_SDL3=FALSE` in the
`cacheVariables` of the three SDL2 MSVC presets (`MSVC-debug`, `MSVC-nosound-debug`,
`MSVC-release`) in `CMakePresets.json`. Preset cache variables override an existing cache,
so the intended backend can no longer be silently overridden.

**Verified:** poisoned the `MSVC-release` cache with `-DUSE_WIN32=TRUE`, then ran a plain
`cmake --preset MSVC-release` (no `-D` override) → cache flipped to `USE_WIN32:BOOL=FALSE`.
`release.yml` relies on the preset default and therefore inherits the pin with no workflow
change.

_Original finding:_

The existing `out/build/MSVC-release` dir carried `USE_WIN32:BOOL=TRUE` in
`CMakeCache.txt` (from a prior `MSVC-win32-*` configure). `cmake --preset MSVC-release`
does **not** pin `USE_WIN32=OFF`, so the stale cached `TRUE` won. Result of the first
package attempt:
- `ReMoMber.exe` was built as the **native Win32 backend** (imports `WINMM.dll`, **no
  SDL2** — verified via `dumpbin /DEPENDENTS`), silent, no audio.
- The generated `cmake_install.cmake` had **zero** SDL references, so no SDL DLLs were
  installed. The `SDL2.dll` / `SDL2_mixer.dll` sitting in `bin/Release` were **stale
  leftovers** from an earlier SDL2 configure, not part of the package.
- The resulting ZIP/installer was internally consistent (a valid self-contained Win32
  build needs no SDL) but **did not match what CI ships** (SDL2 + audio + bundled DLLs).

This is exactly the hazard the `release-check.yml` comment warns about: *"a restored
stale cache would keep USE_WIN32=TRUE (cache vars persist) and silently build the wrong
backend."* CI dodges it by never caching the Windows build dir and doing a fresh
configure. **A local operator reproducing the release in an existing build dir gets the
wrong backend with no error.**

**Fix confirmed:** re-running configure with explicit `-DUSE_WIN32=OFF` flips the
backend (SDL2/SDL2_mixer found, `USE_WIN32:BOOL=OFF`). Recommended hardening: pin
`USE_WIN32=OFF` (and `USE_SDL3=OFF`) in the `MSVC-release` preset's `cacheVariables` so
the intended backend can't be silently overridden by a stale cache.

---

## Decision process / plan

1. Reconfigure `MSVC-release` with CI-equivalent flags (`-DREMOM_VERSION`,
   `-DCMAKE_INSTALL_BINDIR=.`). ← reproduce the real release package layout.
2. Build `ReMoMber` (Release).
3. Run the `package` target → produce `ReMoM-*.zip` + `ReMoM-*.exe`.
4. Inspect the ZIP contents (flat layout, all DLLs present).
5. Verify the NSIS `.exe` was produced and, if possible, do a silent install (`/S`)
   into a temp dir and confirm the payload + uninstaller land correctly.
6. Record results below.

---

## Results (2026-07-22, `REMOM_VERSION=0.0.0-test`)

All steps run on this Windows box against `out/build/MSVC-release`.

**Backend-correct build (after `-DUSE_WIN32=OFF`) — this is what CI ships:**

| Check | Result |
|-------|--------|
| Configure (SDL2 found) | ✅ `SDL2 2.32.2`, `SDL2_mixer 2.8.1` found |
| `ReMoMber.exe` links SDL2 | ✅ `dumpbin /DEPENDENTS` → `SDL2.dll`, `SDL2_mixer.dll` |
| `package` target → ZIP + NSIS | ✅ `ReMoM-0.0.0-test-Windows-AMD64.zip` (2.06 MB) + `.exe` (1.92 MB) |
| ZIP layout | ✅ **flat** (`ReMoMber.exe` at root), `PLAYING.md`, `SDL2.dll`, `SDL2_mixer.dll`, 8× MSVC runtime DLLs — 12 files, engine-only |
| NSIS silent install (`/S /D=<tmp>`) | ✅ exit 0; full payload landed flat + `Uninstall.exe` generated |
| NSIS silent uninstall (`/S _?=<dir>`) | ✅ exit 0; all payload removed (leftover `Uninstall.exe` is expected in `_?=` sync mode; a real uninstall self-deletes + RMDirs) |

**Conclusion:** With the backend pinned correctly, the Windows CPack ZIP **and** NSIS
installer build, install, and uninstall correctly, and the package is self-contained
(SDL2 + MSVC runtime bundled). The CI `release.yml` Windows job is trustworthy **because
it does a fresh configure**.

### Recommendation (hardening — not yet applied; diagnose-only pass)
Pin the backend in the `MSVC-release` configure preset so a stale local cache can't
silently flip it:
```jsonc
// CMakePresets.json → configurePresets → "MSVC-release" → cacheVariables
"USE_WIN32": { "type": "BOOL", "value": "OFF" },
"USE_SDL3":  { "type": "BOOL", "value": "OFF" }
```
This makes local `--target package` reproduce the CI package faithfully in an existing
build dir, closing the F3 trap. Also worth a one-line doc fix: `PRD-Installer-Game-Data-
Setup.md` L140 still names the `USE_WIN32=TRUE` binary as what ships (F2).

---

## F5 — Install location: `Documents` vs `Program Files`, and the per-user fix

**Why the installer came up with `Documents`:** CPack's stock NSIS template
(`Modules/Internal/CPack/NSIS.template.in`) decides the default install dir at runtime
via a "JustMe vs AllUsers" heuristic in `.onInit`:
- It first sets `$INSTDIR = $DOCUMENTS\ReMoM` (the per-user "JustMe" default), then
- calls `UserInfo::GetAccountType` and **only if that returns `Admin`/`Power`** switches
  it to `$PROGRAMFILES64\ReMoM`.

So a **non-elevated** run defaults to `Documents\ReMoM`; an elevated/admin run defaults
to `Program Files\ReMoM`. Line 41 is `RequestExecutionLevel admin` (hardcoded in the
template — no CPack variable changes it), which normally forces UAC → elevated → Program
Files; the `Documents` outcome means that run wasn't treated as admin. NSIS writes **no
install log** by default — the generated `_CPack_Packages/win64/NSIS/project.nsi` is the
source of truth.

**What's controllable vs hardcoded in the stock template (verified against CMake 4.2.3):**
- `RequestExecutionLevel admin` — **hardcoded** (line 41). No `CPACK_*` override.
- JustMe fallback `$DOCUMENTS\...` — **hardcoded** (line 973). No override.
- `InstallDir` / default root — templated via `@CPACK_NSIS_INSTALL_ROOT@` (settable).
- There is **no `CPACK_NSIS_TEMPLATE`** variable in this CMake version. The supported
  override is a repo-local `NSIS.template.in` on `CPACK_MODULE_PATH` (which defaults to
  `CMAKE_MODULE_PATH`; our `CMakeLists.txt` already appends `cmake/`).

### Decision (2026-07-22): per-user install into `%LOCALAPPDATA%\ReMoM`
Chosen because the engine needs a **user-writable** install dir — the player copies their
`.LBX` game data next to `ReMoMber.exe` and the engine writes saves/logs there, so a
read-only `Program Files` install is wrong for this app. Per-user also means **no UAC**.

### Implementation (applied)
1. **New `cmake/NSIS.template.in`** — repo-local override of CPack's stock template. Three
   surgical edits (all marked `ReMoM:`):
   - `RequestExecutionLevel admin` → `user` (no elevation).
   - `.onInit` — replaced the JustMe/AllUsers/`$DOCUMENTS` dance with `StrCpy
     $SV_ALLUSERS "JustMe"` + `SetShellVarContext current` (consistent per-user; `/D`
     override still honored; the gated-off `CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL`
     HKLM previous-version check is untouched — it jumps straight to `inst:`).
   - `un.onInit` — replaced the same admin dance with `SetShellVarContext current` so the
     uninstaller reads/deletes the current-user hive it wrote.
   - `$SV_ALLUSERS` is read nowhere else; `CPACK_NSIS_MODIFY_PATH` is off, so the PATH
     logic is inert — the simplification is safe.
2. **`CMakeLists.txt`** — added `set(CPACK_NSIS_INSTALL_ROOT "$LOCALAPPDATA")` in the
   `if(WIN32)` CPack block (feeds the template's `InstallDir` root).

> Note: `NSIS.template.in` has two dots but is a **CPack-mandated filename** (CPack
> searches for exactly that name), so the repo's one-dot filename convention doesn't apply.
> Autotools/`Makefile.am` needs no change — CPack/NSIS packaging is CMake-only.

### Results — per-user override (2026-07-22, verified)

| Check | Result |
|-------|--------|
| Generated `project.nsi` reflects override | ✅ `InstallDir "$LOCALAPPDATA\ReMoM"`, `RequestExecutionLevel user`, `SetShellVarContext current` in both init fns; no `$DOCUMENTS`/`all` reassignments |
| Silent install, **no `/D`**, no elevation | ✅ landed in `C:\Users\jbalcomb\AppData\Local\ReMoM` with full engine-only payload |
| Uninstall registry hive | ✅ `HKCU\…\Uninstall\ReMoM` (DisplayName `ReMoM 0.0.0`); **absent** from HKLM |
| Silent uninstall, no elevation | ✅ exit 0; all payload removed; HKCU key deleted (leftover `Uninstall.exe` is the `_?=` sync-mode artifact) |

**Conclusion:** the Windows installer now installs **per-user into `%LOCALAPPDATA%\ReMoM`
with no UAC prompt**, consistently, and uninstalls cleanly from the current-user hive.
Files changed: `cmake/NSIS.template.in` (new), `CMakeLists.txt` (one line). *Not committed
— user handles commits.*

---

## F6 — UX goal, and what the first-run journey actually looks like

Stating the goal explicitly, because the shortcut question can't be answered without it:

> **A player who already owns MoM v1.31 gets from *downloaded installer* to *playing* in the
> fewest obvious steps — and at every point where they must act, the app tells them exactly
> what to do and where.**

**Correction to an earlier assumption:** ReMoMber does **not** crash on missing game data.
Verified by running it in an empty directory — it prints/shows:

```
ReMoM could not find your Master of Magic game data.
Missing: FONTS.LBX, MAINSCRN.LBX, WIZARDS.LBX, SPELLDAT.LBX
Copy every .LBX file from your original Master of Magic v1.31 installation into the
same folder as the ReMoMber executable (or set the REMOM_DATA_DIR environment
variable to point at them).  See PLAYING.md for details.
```

(Process sat on a modal dialog until the 15 s timeout killed it — exit 124.) So the
`PRD-Installer-Game-Data-Setup.md` premise "install, launch, immediate crash" is **stale**,
and `REMOM_DATA_DIR` already provides an out-of-tree data path.

Journey against the goal:

| Step | State |
|------|-------|
| Install | ✅ per-user `%LOCALAPPDATA%\ReMoM`, no UAC |
| Launch | ✅ already fails soft with a precise, actionable message |
| **Get LBX files into the right folder** | ⚠️ the real friction — the dialog says "same folder as the executable" but not *where*, and `%LOCALAPPDATA%` is hidden |

This reframed the shortcut work: a launch shortcut is *safe* (the app degrades gracefully),
but the higher-value item is **folder discoverability** — hence the `ReMoM Game Data Folder`
Start Menu link, which is the direct mitigation for choosing a hidden install root.

## F7 — Installer game-data page: copy, don't record a path

Added a second `MUI_PAGE_DIRECTORY` (bound to `$OGMOM_DIR`, distinct from `$INSTDIR`) that
asks where the player's Master of Magic v1.31 data lives, then **copies** `*.LBX` +
`CONFIG.MOM` into `$INSTDIR`.

**Decision — copy, not path-recording (reverses a PRD non-goal).**
`PRD-Installer-Game-Data-Setup.md` listed copying as a Non-Goal, reasoning it would be
"file redistribution". That conflates two things: bundling LBX *in the installer* would be
redistribution; copying *from the user's own machine to their own machine* is not. We still
ship zero game data and the download stays ~4.7 MB. Copying makes the install
self-contained — it survives the player moving, updating, or uninstalling their
GOG/Steam/floppy copy — and needs no configuration, since `$INSTDIR` is the exe-dir and
already on the engine's search path.

**Copy XOR record — not both.** The search order is
`REMOM_DATA_DIR -> cache -> [Paths] game_data -> exe-dir -> CWD`, so `game_data` precedes
exe-dir. Writing a recorded path to the *original* alongside a copy would make the original
**shadow** the copy: the copy becomes dead weight and any divergence silently favours the
original. So the `[Paths] game_data` write was removed when copying was adopted.

**Do not touch `%APPDATA%\ReMoM`.** On Windows both `STU_GRAF_User_Config_Dir` and
`STU_GRAF_User_Data_Dir` resolve there (`STU_GRAF.c:240`, `:437`), so it holds the player's
`SAVE*.GAM`. The PRD's FR#10 ("uninstaller removes the generated `ReMoM.ini`") would be
actively dangerous if applied to that directory. The uninstaller leaves it strictly alone.

**Uninstall.** Copied files are **not** in CPack's generated uninstall manifest (that only
covers INSTALL-target output), so the uninstaller explicitly deletes `$INSTDIR\*.LBX` and
`$INSTDIR\CONFIG.MOM` — otherwise they'd be orphaned and `$INSTDIR` would never be
removable. `$OGMOM_DIR` is only ever read.

**Autodetect** (`ReMoM_DetectGameDataDir`) probes the same locations the engine's
`graf_add_autodetected_game_data()` does — GOG, Steam via `$PROGRAMFILES32`, `C:\MPS\MAGIC`
— so installer and engine agree. It also sets `$OGMOM_VALID`, because a silent (`/S`)
install skips all pages and the LEAVE handler never runs; without that an unattended install
would never copy anything.

**Validation** is presence-only (`FONTS.LBX`), mirroring `graf_dir_has_signature()`. Not
finding it is non-fatal — Yes/No to continue and set up later — because ReMoMber fails soft
(F6). The PRD's SHA256 hash-manifest half remains unimplemented and separable.

### Verified / not verified

| Check | Result |
|-------|--------|
| NSIS script compiles, installer generates | ✅ |
| Copy mechanics (standalone NSIS harness) | ✅ all `*.LBX` incl. lower-case `terrain.lbx`, `CONFIG.MOM` via the guard; `README.TXT` / `SAVE1.GAM` correctly not copied |
| Earlier path-recording build (before the switch to copying) | ✅ had written `[Paths] game_data`, uninstall removed only that key, unrelated `[Video]` setting preserved, saves intact |
| Engine consumes `[Paths] game_data` | ✅ with only the INI supplying the path, ReMoMber launched instead of showing the missing-data dialog |
| **Interactive directory page (wording, layout, prepopulation, Yes/No prompt)** | ❌ **NOT verified — needs a manual GUI run** |
| **End-to-end copy via the real installer UI** | ❌ **NOT verified — autodetect probes fixed paths (`C:\GOG Games`, Steam, `C:\MPS\MAGIC`) that can't be created without writing to protected/system locations** |

> **Process note:** during testing I created `C:\MPS\MAGIC` to force an autodetect hit. That
> was writing to the root of the user's system drive to test our own code — not acceptable.
> It was removed. Exercise autodetect against a path you control, or test the page
> interactively instead.

## Remaining gaps ("what else") — 2026-07-22

Prioritized, all grounded in files/tools this session:

1. **Start Menu shortcuts** — [DONE]. `CPACK_PACKAGE_EXECUTABLES` was unset, so the installer
   created only `Uninstall.lnk`. Added a launch shortcut plus a `ReMoM Game Data Folder` link
   (see F6). Also set `CPACK_NSIS_EXECUTABLES_DIRECTORY` to track `CMAKE_INSTALL_BINDIR` —
   the default (`bin`) would have generated a shortcut to a non-existent
   `$INSTDIR\bin\ReMoMber.exe` in our flat release layout.
2. **CI packaging smoke test** — [DONE]. Added to `release-check.yml`'s Windows job: installs
   NSIS, runs `--target package`, asserts the `.zip`/`.exe` exist, and greps the generated
   `project.nsi` for `RequestExecutionLevel user` + the `$LOCALAPPDATA` `InstallDir` so a
   regression in our template override fails the build. Also covers gap #5.
3. **Doc updates** — [DONE]. `RELEASES.md` now documents the per-user install location,
   shortcuts, the template override, and fail-soft + `REMOM_DATA_DIR`.
   `PRD-Installer-Game-Data-Setup.md` had three stale spots corrected (crash premise,
   template-override-needed note, `USE_WIN32=TRUE`).
4. **Still open — installer-side game-data setup.** The remaining friction from F6: the
   installer doesn't ask for / validate the player's MoM directory. That's exactly what
   `PRD-Installer-Game-Data-Setup.md` specifies, and the template override it needed now
   exists. Its urgency is lower than the PRD implies (fail-soft already works), but it is the
   last real step in the first-run journey.
5. **Template version coupling** (carried, mitigated by #2). `cmake/NSIS.template.in` is a copy
   of CMake 4.2.3's; a runner with a materially different CMake could mismatch. The smoke test
   now catches this on the runner.

## Commands used (reproducible)
```sh
# From repo root. NSIS on PATH: C:\Program Files (x86)\NSIS
cmake --preset MSVC-release -DREMOM_VERSION=0.0.0-test -DCMAKE_INSTALL_BINDIR=. -DUSE_WIN32=OFF
cmake --build out/build/MSVC-release --config Release --target ReMoMber
cmake --build out/build/MSVC-release --config Release --target package
# ZIP:  out/build/MSVC-release/ReMoM-0.0.0-test-Windows-AMD64.zip
# NSIS: out/build/MSVC-release/ReMoM-0.0.0-test-Windows-AMD64.exe
# Silent install/uninstall test (MSYS: set MSYS2_ARG_CONV_EXCL="/S;/D=" so switches pass literally):
#   ./ReMoM-...exe /S /D=<abs\win\path>
#   <dir>/Uninstall.exe /S _?=<abs\win\path>
```
