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
| Windows x64 | `ReMoM-<ver>-Windows-AMD64.zip`, NSIS `.exe` installer | native **Win32** | **Silent** — the Win32 backend links no audio library yet. Self-contained (no DLLs). |
| Linux x86_64 | `ReMoM-<ver>-x86_64.AppImage` (recommended), `ReMoM-<ver>-Linux-*.zip` / `.tar.gz` | **SDL2** + SDL2_mixer | The **AppImage bundles SDL** (via `linuxdeploy`) and is self-contained. The plain ZIP/TGZ does **not** bundle SDL — use it only if SDL2 is already installed. |
| macOS arm64 | `ReMoM-<ver>-Darwin-arm64.zip` | **SDL2** + SDL2_mixer | SDL dylibs are bundled (via `dylibbundler`). Binary is **unsigned** → see Gatekeeper note below. |
| Source | `ReMoM-<ver>-Source.zip` / `.tar.gz` | — | Excludes `assets/`, build dirs, and `.git`. |

What the **player must supply** (documented in `PLAYING.md`): every `.LBX` file
plus `CONFIG.MOM` from an original Master of Magic v1.31 (DOS) installation,
copied next to the executable.

Known limitations carried intentionally (follow-ups, not blockers):
- **Windows is silent** (Win32 backend). Switching Windows to SDL2 for sound is a
  future change; the `if(WIN32 AND NOT USE_WIN32)` block in `CMakeLists.txt`
  already handles bundling SDL DLLs when that happens.
- **macOS is arm64 only.** Adding an Intel build is a one-line matrix addition
  (`runs-on: macos-13`) in `release.yml` when wanted.
- **macOS binary is unsigned / un-notarized.** First launch hits Gatekeeper;
  users clear it with `xattr -dr com.apple.quarantine ReMoMber` (covered in
  `PLAYING.md`).

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
