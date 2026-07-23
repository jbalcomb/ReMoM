# BRA — Linux Native Package Install ("`apt install`") Architecture Decision

**Status:** Decided — Tier 1 approved for implementation; Tiers 2/3 deferred
**Date:** 2026-07-22
**Author role:** Business Requirements Analyst (decision record)
**Related:** `PRD-Linux-Package-Install.md`, `PLAN-Linux-Package-Install.md`, [`RELEASES.md`](../../RELEASES.md), [`Devel-Linux-AppImage.md`](../#Devel/Devel-Linux-AppImage.md)

This records *how* the Linux native-package approach was decided — the goal, the criteria weighed, the distribution options considered, and why the effort is split into three tiers with only the first one funded now. It is a decision record, not an implementation plan.

---

## 1. The ultimate goal

A Linux user should be able to install ReMoM the way they install anything else on their machine — through the system package manager — and have the dependencies, the menu entry, the upgrade path, and the uninstall all handled by that package manager. Concretely, the target sentence is:

> "`apt install` it and it's there."

That sentence hides an important distinction the project has to make on purpose:

- **A package the user installs** — `sudo apt install ./remom_X.Y.Z_amd64.deb`. Real `dpkg` metadata, real dependency resolution, real `apt remove`. Requires no hosting.
- **A package the user *subscribes* to** — `apt install remom` bare, after adding a repository. Same artifact, plus an APT repository, a signing key, and a hosting commitment that never ends.

Both are "apt install." Only the second is free of a leading `./`, and the second costs an order of magnitude more to operate. Deciding which one we mean *is* this decision.

---

## 2. Current-state finding

ReMoM already ships two Linux artifacts, and neither is a system package.

| Fact | Evidence |
|---|---|
| Linux CPack generators are archive-only | [`CMakeLists.txt:326`](../../CMakeLists.txt#L326) — `set(CPACK_GENERATOR "ZIP;TGZ")` |
| The AppImage bundles SDL2 rather than depending on it | [`release.yml:158`](../../.github/workflows/release.yml#L158) — `linuxdeploy … --output appimage` |
| The install set is already engine-only and correct | [`CMakeLists.txt:286`](../../CMakeLists.txt#L286) — `install(TARGETS ReMoM)` only |
| `GNUInstallDirs` is already included, so `bin/` is already the default | [`CMakeLists.txt:285`](../../CMakeLists.txt#L285) |
| The release build deliberately flattens that | `release.yml` passes `-DCMAKE_INSTALL_BINDIR=.` for the portable ZIP layout |
| The `.desktop` entry and icon exist only as heredoc/base64 inside CI | [`release.yml:135`](../../.github/workflows/release.yml#L135), and a base64 placeholder PNG at `release.yml:151` |
| Per-user files already go to XDG dirs, never next to the binary | `STU_GRAF_Open_User` — see [`PRD-Installed-Player-First-Run.md`](PRD-Installed-Player-First-Run.md) |

Two findings reshaped the effort:

**Finding A — the hard part is already done.** A Debian package needs an FHS layout, no writes under `/usr` at runtime, and declared shared-library dependencies. ReMoM satisfies all three *today*: `GNUInstallDirs` is wired, the install set is engine-only, and the XDG work from the Installed-Player First Run PRD means the game never writes beside its own binary. The remaining work is metadata and a desktop entry, not architecture.

**Finding B — the `.desktop` file and icon are trapped in CI.** They are generated inline in `release.yml`, so a `.deb` cannot install them without duplicating a heredoc. Any package format beyond a raw tarball needs them as real repo files. This makes "extract the desktop entry into `packaging/`" a shared prerequisite rather than deb-specific work — the AppImage benefits from the same change.

**Finding C — the AppImage and the `.deb` are complements, not competitors.** The AppImage bundles SDL2 and runs on any distro; the `.deb` depends on the distro's SDL2 and integrates with the system. Shipping both is the norm, not a redundancy. Neither replaces the other.

---

## 3. Criteria considered

| Criterion | Why it matters |
|---|---|
| **Does the user get real package-manager integration?** | Dependency resolution, menu entry, clean uninstall, upgrade path. This is the entire ask. |
| **Ongoing operational burden** | A repository and a signing key are a *forever* commitment. An artifact on a release page is not. |
| **Secret/key custody** | A GPG signing key held in CI is a durable liability the project must be prepared to rotate and protect. |
| **Distro reach** | How many distributions does one artifact serve? |
| **glibc floor** | A package built on a newer distro silently refuses to install on older ones. Affects which runner we build on. |
| **Time to first working artifact** | We want the next release to carry it. |
| **Third-party review exposure** | Some channels put a human gatekeeper (and a licensing question about a Master of Magic reimplementation) between us and the user. |
| **Reversibility** | If the choice is wrong, how expensive is backing out? |

---

## 4. Options considered

### Option A — local `.deb` as a release asset  ✅ **chosen (Tier 1)**

Ship `remom_X.Y.Z_amd64.deb` on the GitHub release. The user runs `sudo apt install ./remom_X.Y.Z_amd64.deb`.

- **Package-manager integration:** full. `apt` resolves and installs `libsdl2-2.0-0` / `libsdl2-mixer-2.0-0` itself; `apt remove remom` works; the app appears in the desktop menu.
- **Operational burden:** none beyond building it. No hosting, no key.
- **Reach:** Debian, Ubuntu, Mint, Pop!_OS, and derivatives — the large majority of desktop Linux.
- **Cost:** CPack already knows how to do this; the work is metadata plus the desktop-entry extraction that Finding B requires anyway.
- **Weakness:** no `apt upgrade` path — the user re-downloads. Accepted for a `0.x` alpha where users are checking the releases page regardly anyway.

### Option B — APT repository on GitHub Pages  ⏸ **deferred (Tier 2)**

Publish a signed flat repository so `apt install remom` works bare after adding a source.

- **Package-manager integration:** full, *plus* automatic updates via `apt upgrade`.
- **Cost:** a GPG signing key in Actions secrets (custody, rotation, and revocation are now ours), an `apt-ftparchive` publish step, and a `gh-pages` hosting commitment that outlives any individual maintainer's interest.
- **Why deferred, not rejected:** it consumes the Tier 1 artifact unchanged. Building the `.deb` first is a strict prerequisite, and doing Tier 1 alone tells us whether anyone actually installs it before we take on a permanent key.

### Option C — Launchpad PPA  ❌ rejected for now

- Source-based: upload a signed source package, Launchpad builds it. Gives real `apt` subscription with no hosting of our own.
- **Rejected because:** Ubuntu-only, requires a Debian source-package layout (`debian/` directory, changelog discipline) that duplicates the CPack path, and requires a Launchpad-registered OpenPGP key. All of Tier 2's key burden, with a second packaging pipeline bolted on.

### Option D — official Debian / Ubuntu archive  ❌ rejected

- The genuine "`apt install remom` with nothing added" endpoint.
- **Rejected because:** requires a Debian Developer sponsor, full DFSG licensing review, and a maintenance commitment measured in years. ReMoM is a reimplementation of a commercial 1994 game with no root licence file; the licensing conversation alone is out of scope and is explicitly the project owner's to have with a lawyer, not something to trigger accidentally by filing an ITP bug.

### Option E — Flatpak / Flathub  ⏸ noted, different track

- Arguably the modern default for Linux games; distro-agnostic, sandboxed, one manifest, and Flathub does the hosting and signing for us.
- **Not chosen now because:** the sandbox intersects awkwardly with ReMoM's central UX fact — the user supplies their own game data from an arbitrary path (a GOG install, a CD rip). Flatpak's filesystem portals would need explicit thought, and the AppImage already covers "runs on any distro." Revisit as a peer of Tier 2, not a replacement for Tier 1.

### Option F — Snap  ❌ rejected

Same sandbox concern as Flatpak, narrower reach, and a single-vendor store. No advantage over E.

### Option G — do nothing; the AppImage is enough  ❌ rejected

The AppImage runs anywhere but is not *installed*: no menu entry until the user wires one up, no `apt remove`, no shared SDL2, and it is invisible to the package manager. The user's request was specifically for the normal path, and Finding A says the normal path is cheap.

---

## 5. Decision

**Implement Option A now. Defer Option B until Tier 1 has shipped and been used. Reject C and D. Keep E on the list as a separate track.**

Supporting decisions taken with the same stroke:

1. **The `.deb` is a second, separate configure of the same tree** — not a variant of the release configure. The portable ZIP wants `CMAKE_INSTALL_BINDIR=.`; the `.deb` wants FHS `bin/`. Trying to serve both from one build directory is what previously produced a stale-cache bug when `BINDIR=.` persisted into a local install. A distinct build dir is the fix, and it is cheap.
2. **The `.deb` depends on the distro's SDL2; it does not bundle it.** Bundling is the AppImage's job. A system package that carries its own copy of a library the distro already ships is the thing distro packaging exists to prevent.
3. **The desktop entry and icon move out of `release.yml` into `packaging/`.** Required by Finding B; the AppImage build switches to the same files, which also retires the base64 placeholder PNG as the *only* icon.
4. **Build the `.deb` on the oldest supported runner, not the newest.** A package built against a newer glibc refuses to install on older distros, and that failure is invisible until a user reports it. This costs a runner pin and nothing else.
5. **Package name is `remom`, lowercase.** Debian policy. The CPack `CPACK_PACKAGE_NAME` stays `ReMoM` for the archive artifacts; the DEB generator gets its own name.

---

## 6. What this explicitly does not decide

- **Licensing.** Out of scope by standing instruction. Note only that Options C and D would *force* the conversation; A and B do not.
- **RPM (Fedora/openSUSE).** CPack's RPM generator is the same shape of work as DEB and can be added later at low cost. Not funded now — it doubles the CI verification surface for a smaller slice of the desktop-Linux audience.
- **arm64 Linux.** amd64 only, matching every other Linux artifact today.
- **Whether the AppImage stays.** It does. See Finding C.
