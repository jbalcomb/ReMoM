# Releasing ReMoM

This is the runbook for cutting a public release. Releases are **engine-only** —
we ship the `ReMoMber` player executable and the bits needed to run it; the
player supplies their own original Master of Magic data files.

> Scope note: this document covers the **technical** release process only. Any
> licensing / copyright questions about what may be distributed are handled
> separately and are deliberately out of scope here.

> See also: [doc/Release-User-Journeys.md](doc/Release-User-Journeys.md) — the
> new-user's-eye view of a release (discovery → download → data files → play),
> including the "Sam" (doesn't own MoM) and "Carl" (already owns MoM) use-cases.

---

## Versioning

- **The git tag is the single source of truth.** Tag format is
  `vMAJOR.MINOR.PATCH` (e.g. `v0.3.0`). The pipeline strips the leading `v` and
  passes the rest to the build as `-DREMOM_VERSION=…`, which drives:
  - every artifact filename (ZIP / NSIS installer / AppImage / macOS ZIP / source),
  - the in-app window-title version banner (`REMOM_VERSION_STRING`, generated
    into `remom_version.h` from `cmake/remom_version.h.in`).
- **No pre-release suffixes for now.** `CMake`'s `project(VERSION …)` needs a
  clean numeric `MAJOR.MINOR.PATCH`, so keep tags to three numbers. (Adding
  `-rc1`/`-beta` support later means teaching `CMakeLists.txt` to split the
  suffix off the numeric triple — not done yet.)
- **We are in `0.x` (alpha).** Anything can change between releases. Rough
  convention while pre-1.0:
  - **PATCH** (`0.3.0 → 0.3.1`): bug fixes, no new player-visible behavior.
  - **MINOR** (`0.3.1 → 0.4.0`): new playable features / notable changes.
  - **MAJOR** stays `0` until the game is broadly playable end-to-end.
- Local / developer builds that aren't tagged report version `0.0.0-dev`.

There is no fixed cadence — cut a release whenever `main` is in a good state and
you want to publish one.

---

## What ships (per platform)

All artifacts are **engine-only**: just `ReMoMber` and `PLAYING.md`. **No** game
data, save games, test fixtures, or config templates are included. (Power-user
config such as the `ReMoM.ini` `--newgame` template is intentionally deferred —
see [Future phase — Dev / Modder tooling](#future-phase--dev--modder-tooling).)

| Platform | Artifact(s) | Backend | Notes |
|----------|-------------|---------|-------|
| Windows x64 | `ReMoM-<ver>-Windows-AMD64.zip`, NSIS `.exe` installer | **SDL2** + SDL2_mixer | Has audio. Self-contained — `SDL2.dll`, `SDL2_mixer.dll`, and the MSVC runtime DLLs are bundled next to the exe. |
| Linux x86_64 | `ReMoM-<ver>-x86_64.AppImage` (recommended), `ReMoM-<ver>-Linux-*.zip` / `.tar.gz` | **SDL2** + SDL2_mixer | The **AppImage bundles SDL** (via `linuxdeploy`) and is self-contained. The plain ZIP/TGZ does **not** bundle SDL — use it only if SDL2 is already installed. |
| ~~macOS arm64~~ | — | — | **Deferred (TBD)** — does not ship in the current release; arm64 link fails on the `pack(2)` DOS structs. See *Known limitations* below. |

(No source package is shipped — the tree carries ~100 MB of docs, IDE packages, and
reverse-engineering reference material that a CPack source tarball would sweep in.
Source lives on GitHub; a curated source distribution is out of scope for v0.x.)

### Windows installer behaviour

The NSIS installer is a **per-user install and never prompts for UAC**:

- Installs to **`%LOCALAPPDATA%\ReMoM`** (not `Program Files`). The install directory
  must be user-writable — the player drops their `.LBX` game data in there and the
  engine writes saves/logs alongside the executable, which a read-only `Program Files`
  install would break.
- Registers its uninstall entry in the **current-user** hive (`HKCU`), so it appears in
  *Add or Remove Programs* for that user only.
- Creates three per-user Start Menu shortcuts under **ReMoM**: `ReMoM` (launches the
  game), **`ReMoM Game Data Folder`** (opens the install directory, so "where do I put
  my LBX files?" is one click, since `%LOCALAPPDATA%` is a hidden path), and `Uninstall`.
- **Asks for the player's Master of Magic v1.31 folder and copies the game data in.**
  A second directory page (pre-filled by probing the usual GOG / Steam / `C:\MPS\MAGIC`
  locations) takes the path to the player's own MoM installation and copies every `.LBX`
  plus `CONFIG.MOM` into the ReMoM install directory. The player's original files are
  only ever read — never modified or moved — and **we still ship no game data**; the
  download stays engine-only. This makes the install self-contained, so it keeps working
  if the player later moves or uninstalls their GOG/Steam copy. If the folder isn't
  found, the player can continue and set it up later; `ReMoMber` fails soft.
- Uninstalling removes the copied game data along with the binaries, and leaves
  `%APPDATA%\ReMoM` — which holds the player's **save games** — strictly alone.

This behaviour comes from a repo-local **`cmake/NSIS.template.in`**, which overrides
CPack's stock NSIS template. CPack's stock template hardcodes `RequestExecutionLevel
admin` and a `$DOCUMENTS` per-user fallback with no variable to change either, so the
template override is the only way to get a predictable per-user install. CPack picks the
override up via `CPACK_MODULE_PATH` (which defaults to `CMAKE_MODULE_PATH`, already
containing `cmake/`). The `release-check.yml` packaging smoke test asserts the override
is still in effect on every push/PR.

What the **player must supply** (documented in `PLAYING.md`): every `.LBX` file
plus `CONFIG.MOM` from an original Master of Magic v1.31 (DOS) installation,
copied next to the executable — or pointed at via the `REMOM_DATA_DIR` environment
variable. If the data is missing, `ReMoMber` does **not** crash: it shows a dialog
naming the missing files and where to put them.

Known limitations carried intentionally (follow-ups, not blockers):
- **Windows uses the SDL2 backend** (default), so the release has audio and bundles
  `SDL2.dll` / `SDL2_mixer.dll`. A silent, self-contained native-Win32 build (no SDL)
  is still available on demand via the `MSVC-win32-*` presets (`USE_WIN32=TRUE`).
- **macOS is deferred — TBD.** The macOS jobs in `release.yml` and
  `release-check.yml` are gated off (`if: false`) and do **not** block releases.
  Reason: the reconstruction's `pack(2)` DOS structs (e.g. `s_UNIT_MUTATION` /
  `_unit_mutation_data` in `MoM/src/UnitView.c`) put a native 8-byte pointer at a
  non-8-byte-aligned offset. The arm64 macOS linker rejects that
  (`ld: pointer not aligned … _unit_mutation_data+0xC`); x86-64 Linux/Windows link
  it fine. `pack(2)` is correct and intentional — it mirrors the 1995 DOS layout —
  so this is macOS-toolchain strictness, not a code bug. **Unresolved; options when
  revisited:** accommodate in the macOS link only (`-Wl,-no_fixup_chains` /
  `-ld_classic`), build x86-64 (`runs-on: macos-13`), or leave macOS out. When
  re-enabled, note the binary is unsigned/un-notarized — first launch hits
  Gatekeeper; clear it with `xattr -dr com.apple.quarantine ReMoMber`.

---

## Future phase — Dev / Modder tooling

The player release above is intentionally minimal. Several features exist in the
engine but are **not** surfaced to players; exposing and documenting them is
deferred to a later, separate effort aimed at **developers and game modders**:

- `--newgame [file.ini]` — launch straight into a configured game (difficulty,
  wizard, race, spell books, retorts) without the setup screens.
- Deterministic worldgen via a fixed RNG `seed` (reproducible maps).
- Runtime logging control (per-category enables, severity threshold).
- Headless / replay flags (`--headless`, `--replay game.RMR`) used by the HeMoM
  test harness.

Planned later: a Dev/Modder bundle (or docs) that ships the `ReMoM.ini` template
and documents these flags for that audience — kept out of the player release so
the default download stays dead-simple. The template already lives at
`assets/ReMoM.ini`.

---

## Pre-release checklist

1. You're on `main`, working tree clean, and it's pushed.
2. CI is green — the **Release Check** workflow (`release-check.yml`) builds and
   tests Windows, Linux, and macOS on every push/PR. Don't tag a red `main`.
3. `README.md` / `PLAYING.md` are current (esp. the required-data-files list).
4. Smoke-build locally at least on this machine:
   ```sh
   cmake --preset clang-release -DREMOM_VERSION=0.0.0-test
   cmake --build out/build/clang-release
   ctest --test-dir out/build/clang-release --output-on-failure \
         --label-exclude integration --exclude-regex "Reload_Melee_Sound_test"
   ```

---

## Cutting a release

1. **Tag and push** (this is the only trigger needed):
   ```sh
   git tag -a v0.3.0 -m "ReMoM v0.3.0"
   git push origin v0.3.0
   ```
2. The **Release** workflow (`release.yml`) runs automatically on the `v*` tag:
   builds → **runs the test suite (release gate)** → packages, on all three
   platforms. If tests fail on any platform, nothing is published.
3. It then creates a **draft** GitHub Release with auto-generated notes and all
   artifacts attached.
4. **Review the draft** on the [Releases page](https://github.com/jbalcomb/ReMoM/releases):
   - Confirm all platforms are present and every filename carries the **same**
     version.
   - Sanity-check at least one artifact (e.g. run the Linux AppImage on a clean
     box to confirm SDL is bundled).
   - Edit the release notes.
5. **Publish** the draft.

### Dry run (build without publishing)
From the **Actions** tab, run the **Release** workflow manually
(`workflow_dispatch`) with the `tag` input set to e.g. `v0.3.0`. It builds and
tests all platforms and uploads the artifacts as **workflow artifacts**, but does
**not** create a GitHub Release (publishing is gated on a real tag push). Use this
to validate packaging before committing to a tag.

```sh
gh workflow run release.yml -f tag=v0.3.0
```

---

## Rolling back a bad release

```sh
# Delete the tag locally and on the remote
git tag -d v0.3.0
git push --delete origin v0.3.0
```
Then delete the draft/published release from the GitHub Releases page. Fix the
problem on `main`, and re-tag (reuse the same version only if the bad release was
never published; otherwise bump PATCH).

---

## Where the automation lives

| File | Role |
|------|------|
| `.github/workflows/release.yml` | Tag-driven build + test + package + draft publish (Win/Linux/macOS). |
| `.github/workflows/release-check.yml` | Per-push/PR release-build gate (Win/Linux/macOS); pass/fail only, no artifacts. |
| `CMakeLists.txt` | `REMOM_VERSION` handling, generated `remom_version.h`, engine-only install rules, CPack config. |
| `cmake/remom_version.h.in` | Template for the generated version header. |
| `CMakePresets.json` | `MSVC-release`, `clang-release`, `macos-release` (+ debug) presets used by CI. |
