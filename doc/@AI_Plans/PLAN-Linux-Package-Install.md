# Plan: Linux Native Package Install (Tier 1 — `.deb` as a release asset)

> Source PRD: [PRD-Linux-Package-Install.md](PRD-Linux-Package-Install.md)
> Decision record: [BRA-Linux-Package-Install.md](BRA-Linux-Package-Install.md)

Ship a real Debian package on every Linux release so `sudo apt install ./remom_X.Y.Z_amd64.deb` gives full package-manager behaviour — dependency resolution, desktop menu entry, clean uninstall — **without** adding a repository, a signing key, or any ongoing hosting commitment.

## Architectural decisions

Durable decisions that apply across all phases:

- **The `.deb` is a separate configure of the same tree.** New preset `linux-package-release` with the FHS layout (`CMAKE_INSTALL_BINDIR=bin`), its own build directory, distinct from `clang-release` which the release pipeline drives with `-DCMAKE_INSTALL_BINDIR=.` for the flat portable ZIP. One build dir cannot serve both layouts; attempting it is what previously left a stale `BINDIR=.` in a cache and made a local `cmake --install` land in the wrong place.
- **Package name is `remom`; project/archive name stays `ReMoM`.** Debian policy requires lowercase. `CPACK_DEBIAN_PACKAGE_NAME` diverges from `CPACK_PACKAGE_NAME` deliberately.
- **Dependencies are derived, never typed.** `CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON` runs `dpkg-shlibdeps` against the built ELF. Hand-maintained `Depends:` lines desynchronise silently; derived ones cannot.
- **The distro's SDL2, not ours.** The `.deb` declares a dependency; only the AppImage bundles. A system package carrying its own copy of a distro-provided library is the exact thing distro packaging exists to prevent.
- **`packaging/` is the single source for the desktop entry and icon.** `packaging/remom.desktop` and `packaging/remom.png` become real repo files, installed by CMake *and* consumed by `linuxdeploy`, replacing the heredoc and base64 blob in `release.yml`. The AppImage and the `.deb` cannot drift.
- **Docdir is named for the package**: `share/doc/remom`, not `GNUInstallDirs`' default `share/doc/ReMoM`. Set explicitly so it is correct for every consumer, not patched per-generator.
- **Linux-only install rules are guarded.** The desktop-entry and icon rules must not appear in the Windows or macOS install sets; those artifacts stay byte-for-byte unchanged in content.
- **Glibc floor beats runner recency.** The `.deb` is built on the oldest supported GitHub runner. A package built against a newer glibc refuses to install on older distros, and the failure is invisible until a user reports it.
- **The `.deb` job carries the same test gate** as every other release job — same `CTEST_ARGS`, same exclusions. No artifact ships untested.
- **Tier 2 (signed APT repo on GitHub Pages) consumes this artifact unchanged.** Nothing in Tier 1 is throwaway if we take Tier 2 later; the only additions there are `apt-ftparchive`, a GPG key, and a publish step.

---

## Phase 1: Desktop entry and icon become repo files

**User stories**: FR1. Prerequisite for every later phase and an immediate improvement to the AppImage.

### What to build

Create `packaging/remom.desktop` with the content currently generated inline in `release.yml`, and `packaging/remom.png` as a real file at a standard icon size. Point the `linuxdeploy` invocation in `release.yml` at both, deleting the heredoc, the `assets/icon.png` conditional, and the base64 placeholder blob. Nothing else changes — this phase is verified by the AppImage still building and launching with the same desktop metadata it had before.

### Acceptance criteria

- [ ] `packaging/remom.desktop` and `packaging/remom.png` exist as tracked files; the icon is a valid PNG at a size `linuxdeploy` accepts.
- [ ] `release.yml` no longer contains a `.desktop` heredoc, a base64 PNG, or an `assets/icon.png` fallback; it passes the `packaging/` files to `linuxdeploy`.
- [ ] The AppImage still builds in CI and launches, with the same `Name`/`Exec`/`Icon`/`Categories` as before.

---

## Phase 2: FHS install rules

**User stories**: FR2.

### What to build

Correct the install layout so a `--prefix /usr` install is FHS-correct. `PLAYING.md` moves off `DESTINATION .` — which lands it at `/usr/PLAYING.md` — into an explicitly-set docdir of `share/doc/remom`. Add Linux-only rules installing `packaging/remom.desktop` to `share/applications` and `packaging/remom.png` to `share/icons/hicolor/<size>/apps`. Guard them so Windows and macOS install sets are untouched. Verified by staging an install to a scratch prefix and reading the resulting tree.

### Acceptance criteria

- [ ] `cmake --install <fhs-build> --prefix <scratch>` produces exactly `bin/ReMoM`, `share/applications/remom.desktop`, `share/icons/hicolor/*/apps/remom.png`, `share/doc/remom/PLAYING.md` — and nothing else.
- [ ] No file lands at the prefix root.
- [ ] The Windows flat layout (`CMAKE_INSTALL_BINDIR=.` → `ReMoM.exe`, `PLAYING.md`, SDL DLLs, MSVC runtime at the package root) is unchanged; no `.desktop` or icon appears in it.
- [ ] The Linux ZIP/TGZ produced by the existing release configure is unchanged in content.

---

## Phase 3: CPack DEB generator

**User stories**: FR3, FR4, FR5. This is the slice that first produces an installable artifact.

### What to build

Add the DEB generator and its metadata — lowercase package name, section, priority, auto-detected architecture, maintainer, homepage, short and long description, Debian-convention filename — plus automatic dependency derivation from the built ELF. Add a `linux-package-release` preset that configures the FHS layout into its own build directory so the whole thing is reproducible with one command locally, not just in CI. End of this phase: a `.deb` exists on the build box and installs.

### Acceptance criteria

- [ ] One documented preset + `cpack` invocation produces `remom_X.Y.Z_amd64.deb` locally.
- [ ] `dpkg -c` lists exactly the Phase 2 file set under `/usr` — no `.LBX`, saves, fixtures, or bundled `libSDL2`.
- [ ] `dpkg -I` shows `Package: remom`, correct `Version`, `Architecture: amd64`, `Section: games`, and a `Depends:` including SDL2 and SDL2_mixer that was derived, not typed.
- [ ] `sudo apt install ./remom_X.Y.Z_amd64.deb` succeeds and pulls SDL2 in as part of the transaction on a machine lacking it.
- [ ] The installed `ReMoM` is on `PATH`, launches, and reaches the title screen with game data present.
- [ ] The game appears in the desktop application menu with its icon.
- [ ] Running the installed binary writes nothing under `/usr`; `MAGIC.SET`, saves, `ReMoM.ini` and logs land in the XDG dirs.
- [ ] `sudo apt remove remom` removes every packaged file and leaves the XDG data intact.
- [x] `lintian` reports **no errors** (verified, exit 0). Warnings fixed or recorded with a reason — copyright, changelog, man page, strip and 0755 dir perms all added; the `Section: games` heuristic carries a documented override.

---

## Phase 4: CI builds and publishes the `.deb`

**User stories**: FR6.

### What to build

A release-workflow job that configures the FHS build, runs the standard test gate, builds the `.deb`, and uploads it as a release asset beside the AppImage and archives. Pin the runner for the glibc floor rather than taking the newest, and verify that floor rather than assuming it. Add the same build to the push/PR gate so a packaging break is caught before a tag is cut.

### Acceptance criteria

- [ ] A tagged release publishes `remom_X.Y.Z_amd64.deb` alongside the existing Linux artifacts, with a version matching every other artifact in the release.
- [ ] The job runs the full test gate with the same exclusions as the other release jobs; a test failure blocks packaging.
- [ ] The published `.deb` installs on a distro older than the build runner — verified, not assumed.
- [ ] The pre-tag gate builds the package too, so packaging breakage surfaces on push/PR.
- [ ] The AppImage, ZIP/TGZ, Windows ZIP and NSIS artifacts are unchanged in content.

---

## Phase 5: Documentation

**User stories**: FR7.

### What to build

Fold the `.deb` into the maintainer runbook and the player-facing docs, and record the tier model so the deferred Tier 2 does not have to be re-derived. `RELEASES.md` gains the artifact in its table and the cut-a-release steps; `PLAYING.md` and `Release-User-Journeys.md` gain install and uninstall commands and the "which download do I want?" guidance now that Linux has three choices.

### Acceptance criteria

- [ ] `RELEASES.md` lists the `.deb` in the per-platform artifact table and covers it in the runbook.
- [ ] `PLAYING.md` gives the install and uninstall one-liners and says where saves/settings live for an installed package.
- [ ] `Release-User-Journeys.md` tells a Linux user which of AppImage / `.deb` / archive they want and why.
- [ ] The three-tier model and the reason Tier 2 is deferred are recorded where the next person will find them.

---

## Deferred — Tier 2 (signed APT repository)

Not part of this plan; recorded so the shape is known. Publish the Tier 1 `.deb` into a flat repository on GitHub Pages: `apt-ftparchive` generates `Packages`/`Release`, the `Release` file is GPG-signed with a key held as an Actions secret, and users add a keyring plus a `sources.list.d` entry to get `apt install remom` bare with `apt upgrade` thereafter. The artifact is consumed unchanged. The cost is a signing key the project must protect and rotate, and a hosting commitment that outlives any individual maintainer's interest — which is why it waits until Tier 1 has shipped and been used.

## Deferred — RPM

CPack's RPM generator is the same shape of work as DEB and reuses Phase 2's FHS layout wholesale. Adding it is cheap once Phase 3 lands; it is unfunded now only because it doubles the CI verification surface for a smaller slice of the desktop-Linux audience.
