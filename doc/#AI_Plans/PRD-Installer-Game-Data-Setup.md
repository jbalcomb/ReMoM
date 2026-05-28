# PRD — Installer: Game Data Setup

**Status:** Draft
**Owner:** TBD
**Date:** 2026-05-05
**Tracks:** Alpha-quality first-run experience

---

## Summary

Extend the NSIS installer so it prompts the user for their existing Master of Magic v1.31 installation directory, validates that the expected `.LBX` game-data files are present, writes the chosen path into a ReMoM configuration file, and offers to launch ReMoMber on completion. ReMoMber reads the config at startup and loads LBX files from the recorded path. Eliminates the current "install, launch, immediate crash" experience without the game data files.

## Problem

Today, the NSIS installer drops `ReMoMber.exe` into `C:\Program Files\ReMoM\` and the optional finish-page checkbox launches it. Because no LBX files are present, ReMoMber crashes on first asset load. The user's first interaction with ReMoM is a crash dialog. There is no in-installer guidance about where to obtain or place LBX files, no validation that the user has them, and no mechanism for the binary to find LBX files anywhere except next to the executable.

The portable ZIP distribution has the same problem but no installer flow to remedy it; that's covered by the related "fail-soft binary" work.

## Goals

- Installer asks the user for the path to an existing Master of Magic v1.31 installation, pre-populating from a candidate list seeded with the floppy installer's default (`C:\MPS\MAGIC\`) and grown via a community-maintained GitHub Issue where users share their actual install paths and 👍 the entries that match their own.
- Installer validates the chosen directory in two passes: (a) presence of expected LBX filenames, and (b) SHA256-hash comparison against a manifest of known-good v1.31 distributions — seeded with the original 1995 floppy distribution as the canonical reference, expanded over time via community-submitted hashes — to catch wrong-version, corrupted, or repackaged game data before the user sees a crash.
- Ship a small companion hashing tool with the binary package so the community can compute SHA256s on their own MoM installation and submit them when a new distribution variant is discovered. Variants accepted into the manifest either as additional known-good rows or, when bytes have drifted in ways that affect ReMoM's loader, addressed via a separately-shipped patch that converts the variant into a canonical-equivalent form.
- Installer writes the validated path into a ReMoM configuration file readable by ReMoMber.
- Installer's finish page offers to launch ReMoMber, which then boots successfully using LBX files from the recorded path.
- ReMoMber falls back gracefully (no crash) if the config is absent or invalid — see Dependencies.

## Non-Goals

- **Copying LBX files into the install directory.** This is a path-recording feature, not file redistribution.
- **Handling users who don't own MoM v1.31.** ReMoM requires the original game; that's a hard prerequisite, not an installer concern.
- **Multi-language installer.** Alpha quality is acceptable; English-only is fine.
- **Code-signed installer.** Unsigned acceptable for alpha; SmartScreen warning is a known wart.
- **The portable ZIP distribution's first-run experience.** Covered by the parallel "fail-soft on missing LBX" binary work.
- **Per-user installs (no-admin).** Stay with the existing Program Files install behavior.

## Functional Requirements

1. **New installer page: "Choose Master of Magic v1.31 directory."** A `MUI_PAGE_DIRECTORY` placed before `MUI_PAGE_INSTFILES`. Bound to a custom NSIS variable (e.g., `$OGMOM_DIR`), distinct from `$INSTDIR`.
2. **Default path autodetect.** Pre-populate with the first match from a curated list of common install locations. The canonical seed entry is `C:\MPS\MAGIC\` (original MicroProse floppy installer default). Additional entries — GOG, Steam, manual-install paths, etc. — are sourced from a community-maintained GitHub Issue where users post their actual MoM install paths and upvote (👍) the entries that match their own. The most-upvoted entries are folded into the installer's autodetect list before each release. Fall back to empty if no candidate matches the user's filesystem.
3. **Validation on Next — presence pass.** Click-Next handler verifies `$OGMOM_DIR\FONTS.LBX` and a small set of additional required LBX filenames exist. If any are missing, show "Master of Magic v1.31 game data not found at this location." and stay on the page.
4. **Validation on Next — hash pass.** For each required file present, compute SHA256 and compare against a hash manifest bundled with the installer. The manifest covers known-good v1.31 distributions (GOG at minimum, ideally also Steam and original CD-ROM). Three outcomes:
   - **All hashes match a known v1.31 distribution.** Advance silently to file extraction.
   - **Files exist but no hash entry matches.** Show "Game data found but doesn't match a recognized Master of Magic v1.31 distribution. ReMoM is built for v1.31 and may behave unexpectedly with these files." Offer two buttons: "Choose another folder" and "Continue anyway."
   - **Hashes match a recognized-but-unsupported version.** If the manifest also contains hashes for older versions (v1.0, v1.2, etc.) tagged as unsupported, give a more specific message: "This looks like Master of Magic v1.0; ReMoM requires v1.31."
5. **Hash manifest format.** Plain-text file shipped inside the installer (e.g., `share\lbx-hashes.txt`). Initial canonical entries are taken from the 1995 floppy distribution; additional rows are appended as community submissions are accepted:
   ```
   FONTS.LBX     <sha256>  v1.31-floppy-1995    # canonical
   FONTS.LBX     <sha256>  v1.31-cdrom          # community-submitted
   FONTS.LBX     <sha256>  v1.0-floppy          # tagged unsupported
   UNITS1.LBX    <sha256>  v1.31-floppy-1995
   ...
   ```
   Multiple lines per filename allowed (one per known distribution). The trailing tag is informational and drives the version-specific error message. The first row in the file is treated as the canonical reference.
6. **Skip option.** A button or checkbox labeled "Configure later" allows advancing without either validation pass. If chosen, no config file is written. ReMoMber will fail-soft on first launch and the user can edit the config manually.
7. **Config file write at end of install.** Write `<install-dir>\ReMoM.ini` (or a dedicated `<install-dir>\ReMoM.cfg` — see Open Questions) containing the validated path. Format: standard INI, human-editable.
   ```ini
   [Paths]
   game_data=C:\GOG Games\Master of Magic
   ```
8. **Finish page launch.** Existing `CPACK_NSIS_MUI_FINISHPAGE_RUN` checkbox runs ReMoMber. No change needed.
9. **Binary-side config consumption.** ReMoMber at startup reads `[Paths] game_data` from `ReMoM.ini` (or `ReMoM.cfg`). If present and the directory contains LBX files, prepend that path to the asset search list. If absent, fall back to existing behavior (search next to the .exe).
10. **Uninstaller behavior.** The uninstaller removes everything in `<install-dir>` including the generated `ReMoM.ini` and the bundled `lbx-hashes.txt`. It must NOT touch the user's MoM directory at `$OGMOM_DIR`.

## Acceptance Criteria

- [ ] Running the NSIS installer presents a "Choose Master of Magic v1.31 directory" page before file extraction.
- [ ] The page pre-populates with a discovered MoM path if one is found in known locations; empty otherwise.
- [ ] Selecting a directory whose required LBX files match a v1.31 entry in the hash manifest advances to file extraction silently.
- [ ] Selecting a directory missing any required LBX file shows the "not found" error and stays on the page.
- [ ] Selecting a directory with the right filenames but unrecognized hashes shows the "doesn't match a recognized distribution" warning with "Choose another folder" / "Continue anyway" options.
- [ ] Selecting a directory whose hashes match an older-version entry (e.g., v1.0) shows the version-specific "this looks like v1.0" message instead of the generic warning.
- [ ] On successful install, `<install-dir>\ReMoM.ini` contains a `[Paths] game_data=<chosen-path>` entry.
- [ ] Selecting "Configure later" advances without writing the config; install completes; ReMoMber on launch shows the fail-soft "place LBX files…" dialog from the related binary work.
- [ ] Clicking "Run ReMoMber" on the finish page successfully boots into the title screen with no crash, using LBX files at the recorded path.
- [ ] Uninstalling removes the ReMoM install directory and the `ReMoM.ini` it wrote, leaving `$OGMOM_DIR` untouched.

## Implementation Notes

### Installer (NSIS)

- CPack's stock NSIS template (`Modules/Internal/CPack/NSIS.template.in` in the CMake install) does not expose a hook for adding a custom `MUI_PAGE_DIRECTORY`. Will require a template override, either via `CPACK_NSIS_TEMPLATE` (verify support in the CMake version we ship with) or by placing a customized `NSIS.template.in` earlier on `CMAKE_MODULE_PATH` than CMake's internal copy.
- Path autodetect: NSIS macros `${If} ${FileExists}` against a hard-coded candidate list. ~10–20 lines.
- Presence validation: `IfFileExists "$OGMOM_DIR\FONTS.LBX" +2 0` followed by `MessageBox` and `Abort`.
- Hash validation: NSIS has no built-in SHA256. Three options ranked by simplicity:
  1. **Shell out to PowerShell** (`Get-FileHash -Algorithm SHA256`). Available on every modern Windows runner without extra dependencies. Use NSIS `nsExec::ExecToStack` to capture output, parse the hash, and compare. ~15–25 lines per file checked.
  2. **NSIS plugin** like `nsCrypto` or `MD5Plugin` (need to bundle the `.dll` with the installer; smaller install footprint than PowerShell shell-out, more setup).
  3. **Pre-computed signatures.** Skip runtime hashing entirely and just exact-byte-compare a small marker section of FONTS.LBX. Brittle but fast.

  PowerShell shell-out (#1) is recommended for alpha simplicity.
- Hash manifest is bundled as a regular installer resource (`File "share\lbx-hashes.txt"`) and parsed at install time by the same NSIS section that does hash validation. Could also be hard-coded as `!define` constants in the `.nsh` if external file is overkill.
- Config write: `WriteIniStr "$INSTDIR\ReMoM.ini" "Paths" "game_data" "$OGMOM_DIR"`.

### ReMoMber

- Read `ReMoM.ini` at startup using existing INI-handling code (per the project layout, there's already a `ReMoM.ini` shipping in `assets/`, suggesting INI-handling exists).
- If `[Paths] game_data` is set and points to a real directory, treat that as the primary LBX search location. Otherwise, retain current behavior (LBX next to .exe).
- The actual asset-loading code change scope depends on whether ReMoMber currently has a single point of LBX path resolution or hard-codes `fopen("FONTS.LBX")` throughout. Investigation needed — see Risks.

### CPack/CMake

- Install rule for `assets/` already adds `ReMoM.ini` to the package; the installer's `WriteIniStr` will overwrite (or merge into) that file at install time. Confirm overwrite vs. merge behavior of `WriteIniStr` and decide which is desired.
- Alternatively, ship without `ReMoM.ini` in `assets/` and have the installer be the sole writer.

### Community hash tool

A small companion utility that lets users compute the SHA256 of LBX files in their own Master of Magic installation, so they can verify against the shipped manifest or submit hashes for a previously-unfingerprinted distribution.

- **Form factor.** Build as another small CLI utility alongside `savedump`, `rmr2hms`, etc., so it's a tracked C target in the existing utility-tools group. Suggested name: `lbx_hashes` (Windows: `lbx_hashes.exe`).
- **Behavior.**
  - Single positional argument: path to a directory containing LBX files.
  - Walks the directory, computes SHA256 of each `.LBX` file (or just the canonical hash-set chosen for fingerprinting — see Open Questions).
  - Emits manifest-format lines on stdout: `<FILENAME>  <sha256>  <user-supplied-tag-or-blank>`.
  - Optional `--check <manifest>` mode: compute hashes and compare against an existing manifest, reporting matches / unrecognized / mismatched per file. Useful for users who suspect their copy has been tampered with or modded.
  - Reads any LBX file format quirks via the same loader as ReMoMber so users running the tool against an oddly-cased filename or a packed format aren't misled.
- **Distribution.** Ships in the binary package (NSIS install dir + portable ZIP) so any user who installs ReMoM already has it locally. Also runnable from the source build by anyone compiling from source.
- **Submission flow.** When a user finds an unrecognized distribution, the tool's stdout is in manifest format — they can copy-paste it into a GitHub issue (or eventual PR against the manifest file). Project owners then either add the rows to the manifest as another known-good distribution, or — if loader-relevant byte differences exist — ship a binary patch (see Open Questions) that converts the variant LBX files into the canonical floppy form.

## Risks / Open Questions

- **Engine asset-loading structure.** If ReMoMber loads LBX files via a centralized `Asset_Open()` function, the binary-side change is a one-line redirect. If it has hard-coded `fopen()` calls scattered through the engine, the refactor is bigger. Needs an investigation pass.
- **Config file naming.** `ReMoM.ini` is already shipped in `assets/`. Two options: (a) reuse it and have the installer add the `[Paths]` section, or (b) use a separate `ReMoM.cfg` to keep installer-written and shipped configs separate. (b) is cleaner; (a) is one-fewer file.
- **Config file location after install.** `<install-dir>\ReMoM.ini` lives in `Program Files`, which a non-admin user can't edit without UAC. If the user later wants to change the path, they need elevation. Acceptable for alpha; consider `%APPDATA%\ReMoM\config.ini` for GA.
- **Re-install behavior.** Running the installer again with a different OG-MoM path overwrites the config. Likely the desired behavior; flag for testing.
- **`CPACK_NSIS_TEMPLATE` support.** The variable name is referenced in the CMake CPackNSIS module's source but isn't part of the public CPack documentation in all versions. May need to verify behavior on the runner's CMake version (currently 3.x bundled with the Windows runner) and adjust technique if needed.
- **What does "Configure later" mean for the uninstaller?** If no config was written, there's nothing to remove. The uninstaller should tolerate the missing file.
- **Hash manifest seeding.** Alpha ships with the original 1995 floppy distribution as the canonical reference (the project owner has access to the floppies and can fingerprint authoritatively). All other distributions — CD-ROM repackages, GOG, Steam, abandonware archives — are unknowns until community members run the hash tool against their installs and submit results. Acceptable to ship with floppy-only and accept that non-floppy users hit "Continue anyway" until their distribution is fingerprinted and added.
- **Which files to hash.** Hashing all ~50 LBX files would catch every form of corruption but takes seconds during install and bloats the manifest. Hashing 3–5 high-signal files (FONTS.LBX, UNITS1.LBX, SPELLDAT.LBX, TERRAIN.LBX) likely fingerprints the version uniquely with much less overhead. The `lbx_hashes` tool should default to whatever set the manifest uses, so user submissions and installer validation cover the same files. Decide which set is canonical before implementation.
- **Repackaged or modded LBX files.** Some users have modded LBX (color tweaks, balance mods, custom data). Hash check flags these as "unrecognized." The "Continue anyway" escape hatch handles it but adds UX surface. Modders may be a meaningful fraction of the actual ReMoM audience — worth thinking about whether modded-but-recognizable files should match, and whether the `--check` mode of the tool should call out specifically which files differ from canonical so a modder knows what they've changed.
- **Patch mechanism for non-canonical distributions.** When a community-submitted distribution differs from canonical in ways that affect ReMoM's loader (not just innocuous byte shuffling), the project commits to providing a patch that converts the variant LBX files into canonical-equivalent form. Open questions: what patch format (xdelta, bsdiff, custom), how patches are distributed (separate download, baked into the installer, fetched on demand), and how the installer detects "this distribution needs patch X." Out of scope to implement for alpha — but the manifest format and tool design should leave room for it (e.g., a `patch=<patch-id>` column in the manifest).
- **Manifest update cadence.** As community submissions arrive, the manifest needs to be updated. For alpha the manifest is just shipped inside the installer — adding rows requires re-publishing a new installer. Acceptable for alpha; the GA-quality answer is a network-fetched manifest, out of scope for now.
- **Path-list curation cadence.** The community-sourced GitHub Issue collecting install paths ([#13](https://github.com/jbalcomb/ReMoM/issues/13)) needs a curation pass before each release: read the issue, take the most-upvoted entries, append them to the installer's hard-coded autodetect list, ship. Open questions: who owns the curation pass, how often (per release? on demand?), and whether to script-scrape the issue's reactions API for automated inclusion later. Manual curation is fine for alpha; automation is GA-grade.
- **`FONTS.LBX` as the presence-validation file.** Confirm it's universally present in every legitimate v1.31 install (floppy, CD-ROM, GOG, Steam). If not, pick a different file or require multiple.

## Dependencies

- **Binary-side fail-soft on missing LBX** — companion change. ReMoMber should not crash if LBX is missing; it should show a clear MessageBox and exit gracefully. This PRD assumes that change exists or is being done in parallel; the "Configure later" path requires it. Worth a separate small PRD or just handled as a coupled change.
- Existing `release.yml` Windows job already runs NSIS via Chocolatey on `windows-latest` to build the installer. No CI infrastructure changes required.
- Existing `MSVC-release` preset with `USE_WIN32=TRUE` (no SDL2 dependency) is the binary that ships in the installer.
- `assets/ReMoM.ini` (already tracked) — the existing file may need to be treated as a template or excluded from the install rule depending on the resolution of the config-file-naming open question.
