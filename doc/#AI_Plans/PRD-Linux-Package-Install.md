# PRD — Linux Native Package Install (`.deb`, and the road to `apt install remom`)

**Status:** Tier 1 approved, in implementation. Tier 2 deferred. Tier 3 rejected.
**Owner:** TBD
**Date:** 2026-07-22
**Tracks:** Public Release Pipeline — Linux distribution channels

> **References:**
> - Decision record: [BRA-Linux-Package-Install.md](BRA-Linux-Package-Install.md) — why `.deb`-as-release-asset first, and why not PPA/Debian/Snap.
> - Implementation plan: [PLAN-Linux-Package-Install.md](PLAN-Linux-Package-Install.md).
> - **Depends on** the per-user file layout from [PRD-Installed-Player-First-Run.md](PRD-Installed-Player-First-Run.md). A system package installs to a read-only `/usr`; without the XDG work already done there, the game could not run when installed.
> - **Complements** [Devel-Linux-AppImage.md](../#Devel/Devel-Linux-AppImage.md). The AppImage bundles SDL2 for any distro; the `.deb` depends on the distro's SDL2 and integrates with the system. Both ship.
> - Maintainer runbook: [RELEASES.md](../../RELEASES.md). Player quick start: [PLAYING.md](../../PLAYING.md). UX view: [Release-User-Journeys.md](../Release-User-Journeys.md).

---

## Summary

Linux users get a real system package. Three tiers, only the first funded:

| Tier | User does | Needs | Status |
|---|---|---|---|
| **1** | `sudo apt install ./remom_X.Y.Z_amd64.deb` | nothing but the release page | **this PRD** |
| **2** | add a source, then `apt install remom` | a signed APT repo on GitHub Pages + a GPG key we hold forever | deferred |
| **3** | `apt install remom` with nothing added | a Debian sponsor, DFSG licensing review | rejected |

Tier 1 gives full package-manager behaviour — `apt` resolves and installs SDL2 itself, ReMoM appears in the desktop menu, `apt remove remom` cleans up — with zero ongoing operational burden. Tier 2 adds only automatic updates, and costs a permanent signing key; it consumes the Tier 1 artifact unchanged, so nothing here is wasted if we take it later.

## Problem

Linux users have two artifacts today and neither is *installed*:

- The **AppImage** runs anywhere but is invisible to the system. No menu entry unless the user wires one up, no `apt remove`, no shared SDL2, no upgrade path. It is a file in `~/Downloads` that happens to be executable.
- The **ZIP/TGZ** is worse: it requires SDL2 to already be present and says so nowhere the package manager can act on.

Neither answers "install it the normal way." Meanwhile the prerequisites for a proper package are already satisfied — `GNUInstallDirs` is wired ([CMakeLists.txt:285](../../CMakeLists.txt#L285)), the install set is engine-only ([CMakeLists.txt:286](../../CMakeLists.txt#L286)), and per-user files already resolve to XDG dirs rather than beside the binary — so the gap is metadata, not architecture.

Two concrete blockers stand in the way:

1. **The release configure flattens the layout.** `release.yml` passes `-DCMAKE_INSTALL_BINDIR=.` so the portable ZIP has `ReMoMber` at its root. A `.deb` built from that same configure would install `/usr/ReMoMber` and `/usr/PLAYING.md`.
2. **The `.desktop` entry and icon exist only inside CI** — a heredoc and a base64 blob in [release.yml:135](../../.github/workflows/release.yml#L135). A package cannot install files that only exist as shell strings.

## File & Directory Map (installed `.deb`, FHS)

| Path | Content | Notes |
|---|---|---|
| `/usr/bin/ReMoMber` | the player executable, **stripped** | the only binary; dev/QA tools stay unpackaged |
| `/usr/share/applications/remom.desktop` | desktop entry | makes it appear in the app menu |
| `/usr/share/icons/hicolor/256x256/apps/remom.png` | icon | referenced as `Icon=remom` |
| `/usr/share/doc/remom/PLAYING.md` | player quick start | lowercase dir, matching the package name |
| `/usr/share/doc/remom/changelog.gz` | Debian changelog | required by policy; native package, so `changelog.gz` not `changelog.Debian.gz` |
| `/usr/share/doc/remom/copyright` | copyright notice | required by policy; not gzipped. Wording is the owner's — `packaging/copyright` |
| `/usr/share/man/man6/ReMoMber.6.gz` | man page | section 6 (games) |
| `/usr/share/lintian/overrides/remom` | lintian overrides | one entry, with its reason |
| *(nothing else)* | | no game data, no saves, no fixtures, no bundled SDL2 |

Runtime writes go where they already go — **not** under `/usr`:

| Purpose | Location |
|---|---|
| Game data (`.LBX`, `CONFIG.MOM`) | the user's own MoM install, via the `STU_GRAF_Open_Asset` search path |
| `MAGIC.SET`, saves | `~/.local/share/ReMoM/` |
| `ReMoM.ini` | `~/.config/ReMoM/` |
| Logs | `~/.local/state/ReMoM/` |

The AppImage's portable mode does **not** apply to an installed package: `/usr` is not writable, so the `AppRun` heuristic is never in play and the game runs in installed mode. That is the correct behaviour and requires no code change.

## Goals

- **A `.deb` on every Linux release** whose install is a one-liner and whose dependencies `apt` satisfies automatically.
- **Dependencies declared, not bundled.** `libsdl2-2.0-0` and `libsdl2-mixer-2.0-0` come from the distro, derived automatically from the built ELF rather than hand-maintained.
- **FHS-correct layout** so the package passes `lintian` without errors and behaves like any other game package.
- **Desktop integration** — the app appears in the menu with an icon, without the user editing anything.
- **A single source for the desktop entry and icon**, shared by the `.deb` and the AppImage, replacing the CI heredoc and the base64 placeholder.
- **A wide glibc floor** — the package installs on distros older than the build runner.
- **No regression to existing artifacts.** The AppImage, ZIP, TGZ, Windows ZIP and NSIS installer are byte-for-byte unaffected in content.
- **Verified locally, not just in CI.** The build box is Linux/amd64 with `dpkg-shlibdeps` present, so the whole artifact is testable here before it reaches a runner.

## Non-Goals

- **Tier 2 (APT repository) and Tier 3 (official archives).** Deferred and rejected respectively — see the BRA.
- **RPM.** Same shape of work, addable later; not funded now.
- **arm64 Linux.** amd64 only, matching the other Linux artifacts.
- **Flatpak/Snap.** Separate track; the sandbox's interaction with user-supplied game data needs its own thought.
- **Replacing the AppImage.** The two are complements.
- **Licensing / copyright.** Out of scope by standing instruction. Noted only because Tier 3 would force the question and Tier 1 does not.
- **Shipping game data.** Unchanged: engine-only, the player supplies their own.

## Functional Requirements

**FR1 — Desktop entry and icon become repo files.**
`packaging/remom.desktop` and an icon at a standard size are real, reviewable files. `release.yml` stops generating them inline and consumes these instead, so the AppImage and the `.deb` are guaranteed to agree.

**FR2 — FHS install rules.**
`ReMoMber` installs to `${CMAKE_INSTALL_BINDIR}`, `PLAYING.md` to a docdir named for the *package* (`share/doc/remom`, not `share/doc/ReMoM`), the desktop entry to `share/applications`, and the icon to `share/icons/hicolor/<size>/apps`. `PLAYING.md`'s current `DESTINATION .` — which lands it at `/usr/PLAYING.md` under an FHS prefix — is corrected. The desktop/icon rules are Linux-only and must not alter the Windows or macOS install sets.

**FR3 — CPack DEB configuration.**
Package name `remom` (lowercase, per Debian policy) while the archive artifacts keep `ReMoM`. Section `games`, priority `optional`, architecture auto-detected via `dpkg --print-architecture`, a maintainer, a homepage, and a short+long description. Filename per Debian convention: `remom_X.Y.Z_amd64.deb`.

**FR4 — Automatic dependency derivation.**
Dependencies are derived from the built binary by `dpkg-shlibdeps` rather than hand-listed, so an added or dropped library cannot silently desynchronise the metadata.

**FR5 — A dedicated `.deb` build.**
The `.deb` is produced from its own configure/build directory with the FHS layout, separate from the release configure that sets `CMAKE_INSTALL_BINDIR=.`. A CMake preset makes this reproducible locally rather than a CI-only incantation.

**FR6 — CI produces and publishes the `.deb`.**
The release workflow builds it on a runner chosen for glibc floor rather than recency, runs the same test gate as every other release job, and uploads the `.deb` as a release asset alongside the AppImage and archives.

**FR7 — Documentation.**
`RELEASES.md` gains the `.deb` in its artifact table and the runbook. `PLAYING.md` and `Release-User-Journeys.md` gain the install/uninstall commands. The tier model and the deferred Tier 2 are recorded so the next person does not re-derive them.

## Acceptance Criteria

- [x] `dpkg -c` on the built package lists exactly the eight files in the map above — and nothing else. No `.LBX`, no saves, no fixtures, no bundled `libSDL2`. *(asserted in CI; dry-run green locally)*
- [x] The shipped binary is stripped. *(asserted in CI; 4.17 MB → 2.14 MB, package 1.65 MB → 801 KB)*
- [x] Installed directory permissions are `0755` regardless of the builder's umask. *(`CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS`; cleared 10 lintian warnings)*
- [ ] `dpkg -I` shows `Package: remom`, a correct `Version`, `Architecture: amd64`, `Section: games`, and a `Depends:` line that includes SDL2 and SDL2_mixer without either having been typed by hand.
- [ ] `sudo apt install ./remom_X.Y.Z_amd64.deb` succeeds on a machine without SDL2 and pulls the SDL2 packages in as part of that transaction.
- [ ] After install, `ReMoMber` is on `PATH` and launches; with game data present it reaches the title screen.
- [ ] After install, the game appears in the desktop application menu with its icon.
- [ ] Running the installed binary writes nothing under `/usr`; `MAGIC.SET`, saves, `ReMoM.ini` and logs land in the XDG dirs.
- [ ] `sudo apt remove remom` removes every packaged file and leaves the user's XDG data intact.
- [x] `lintian` reports **no errors** — verified, exit 0. Cleared: `no-copyright-file`, `no-changelog`, `unstripped-binary-or-object`, `no-manual-page`, `package-section-games-but-contains-no-game`, 10× `non-standard-dir-perm`. The single remaining `W: groff-message` is a local man-db artifact (a canonical five-line man page reproduces it identically) — CI's lintian is the authority.
- [ ] The package installs on a distro older than the one it was built on — the glibc floor is verified, not assumed.
- [ ] The AppImage, ZIP/TGZ, Windows ZIP and NSIS artifacts are unchanged in content; the AppImage now sources its `.desktop`/icon from `packaging/` and still launches.
- [ ] A tagged release publishes the `.deb` as an asset next to the existing artifacts.
- [ ] The full test gate passes in the `.deb` job, same exclusions as every other release job.

## Open Questions

- **~~`/usr/share/doc/remom/copyright`~~ — RESOLVED 2026-07-23.** The owner supplied the wording;
  it lives verbatim in `packaging/copyright` and installs to `<docdir>/copyright`. `lintian` now
  reports **zero errors**. The text is the owner's legal statement, not engineering content — change
  it only at their direction. Two notes for whoever touches it next: the year range `2021-2026` is
  derived from the first and latest commit dates in git, and the transformative-purpose clause reads
  "educational and tribute" (the template's "parody" option was plainly inapplicable).
- **`W: groff-message` is a local artifact, not a defect.** On the dev box lintian reports this for
  the man page, but a five-line *canonical* man page reproduces it identically, and `groff -wmac`,
  `man --warnings`, and lintian's exact four-stage pipeline all exit 0 on the real page. Treat CI's
  lintian output (ubuntu-22.04) as the authority. Non-blocking either way — the CI lint step is
  `continue-on-error`.
- **Icon.** `packaging/remom.png` is the CI placeholder promoted to a tracked file — a flat 256x256
  PNG. Replacing it with real artwork at the same path and size needs no other change.
- **Maintainer field.** Currently `Jim Balcomb <jim.balcomb@gmail.com>`, the address already in every
  public commit. Which address ships in package metadata is the owner's decision.
- **`/usr/bin` vs `/usr/games`.** Debian Policy 11.11 says game binaries belong in `/usr/games`, and
  the mismatch with `Section: games` is what `package-section-games-but-contains-no-game` reports.
  We install to `/usr/bin` and override the tag: `/usr/bin` is on `PATH` everywhere, `/usr/games`
  only on desktop installs, and we do not target the Debian archive. Revisit only if Tier 3 is ever
  reconsidered.
- **Which runner for the glibc floor?** Settled at `ubuntu-22.04` (glibc 2.35). Locally derived floor
  is `libc6 (>= 2.34)`; CI logs the value each run so a bump is visible in the run log rather than in
  a user's bug report.
- **`libsdl2-mixer` and the `DISABLE_AUDIO` build.** Resolved: the packaged configure finds
  SDL2_mixer 2.8.1 and the derived `Depends:` carries `libsdl2-mixer-2.0-0 (>= 2.6.0)`. CI fails the
  job if that dependency ever disappears.
