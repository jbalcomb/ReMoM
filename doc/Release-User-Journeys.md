# ReMoM Release — New-User Journeys

How a new user discovers ReMoM, gets their hands on it, supplies the game data,
and starts playing. This is the **UX view** of a public release — a companion to
[RELEASES.md](../RELEASES.md) (maintainer-facing: how a release is cut) and
[PLAYING.md](../PLAYING.md) (player-facing quick start).

It walks two representative use-cases and calls out where the journey is rough.

## The shipped release, in one line

Public releases are **engine-only**: each artifact is just `ReMoMber` +
`PLAYING.md`. The player supplies their own original **Master of Magic v1.31
(DOS)** data — every `.LBX` file plus `CONFIG.MOM` — dropped next to the
executable. Per platform:

| Platform | Download | Notes |
|----------|----------|-------|
| Windows x64 | `ReMoM-<ver>-Windows-AMD64.zip` or the `.exe` installer | Native Win32 backend. Self-contained. **Silent** (no audio yet). |
| Linux x86_64 | `remom_<ver>_amd64.deb`, `ReMoM-<ver>-x86_64.AppImage`, or `.zip`/`.tar.gz` | See **Which Linux download?** below. |
| macOS arm64 | `ReMoM-<ver>-Darwin-arm64.zip` | SDL dylibs bundled. **Unsigned** → clear quarantine on first run. |

> **Status:** live. `v0.0.1` is published, the release test-gate is green on
> Windows and Linux, and `tools/do_github_release.py` cuts subsequent releases in
> one command. macOS remains deferred (TBD) — see
> [RELEASES.md](../RELEASES.md) *Known limitations*.

---

## Which Linux download?

Linux users see three artifacts and need to know which is theirs. In the order
most people want them:

| Download | Use it when | How |
|----------|-------------|-----|
| `remom_<ver>_amd64.deb` | Debian / Ubuntu / Mint / Pop!_OS, and you want it **installed** — application menu, `apt remove`, SDL2 handled for you | `sudo apt install ./remom_<ver>_amd64.deb` |
| `ReMoM-<ver>-x86_64.AppImage` | Any other distro, or you'd rather not install anything. Bundles SDL2. | `chmod +x`, then run |
| `ReMoM-<ver>-Linux-*.zip` / `.tar.gz` | You want the bare binary and already have SDL2 | extract, `./ReMoMber` |

Two rough edges to be aware of:

- **No `apt upgrade` path.** There is no APT repository yet (Tier 2, deferred), so
  an installed `.deb` never updates itself. The user must notice a new release and
  install the newer `.deb` over the top. Documented in `PLAYING.md`, but it is
  still a thing they have to remember.
- **Portable mode is AppImage-only.** Dropping the MoM files next to the
  `.AppImage` keeps saves and settings right there; an installed `.deb` always
  uses the per-user XDG folders. That is correct for a system install, but it
  means the two Linux artifacts behave differently in a way the user has to read
  about rather than discover.

---

## Use-case A — "Sam": new to MoM, does **not** own the game

Sam is a strategy-game fan who has never played Master of Magic.

1. **Discovery.** Sam hears about ReMoM through the **"Master of Magic – Fans"
   Discord** ([#mom-reverse-engineering](https://discord.gg/kSNA8bE6Wt)) or the
   alpha-demo YouTube video linked from the [README](../README.md), and clicks
   through to the GitHub repo.
2. **Releases page.** The README's "Download and Play" sends Sam to
   [Releases](https://github.com/jbalcomb/ReMoM/releases).
3. **Download.** Sam grabs the artifact for their platform (see table above) and
   extracts it.
4. **The wall: game data.** Sam reads that ReMoM ships **no game content** and
   needs every `.LBX` + `CONFIG.MOM` from an original **MoM v1.31 (DOS)** install
   — which Sam doesn't have. Sam has to *acquire the original game first*, then
   locate its DOS data files. **This is where Sam most likely stalls.**
5. If Sam clears that hurdle, the rest is the shared **Launch** flow below.

**Verdict:** Sam is the hardest case. The engine-only model asks Sam to bring a
copy of a 1994 game before anything runs. Nothing in the current docs helps Sam
*obtain* the game or find its files — see the friction list at the end.

---

## Use-case B — "Carl": just found ReMoM, **already owns** OG-MoM

Carl is a long-time Master of Magic fan who already has a copy of the game. He is
exactly the player the engine-only release is designed for.

1. **Discovery.** Same as Sam — Carl finds ReMoM via the Discord or the repo.
   As an existing fan, he's probably *already in* the MoM community where ReMoM
   is shared, so discovery is a non-issue for him.
2. **Releases page → download.** Carl downloads the artifact for his platform and
   extracts it (see table above).
3. **Game data — the step Carl breezes through.** Carl already owns the game, so
   instead of "acquire MoM," his task is just "**find the data files in the copy
   I already have and copy them next to `ReMoMber`.**" Where they live depends on
   how he owns it:
   - **Original DOS (CD/floppy install):** the `.LBX` files and `CONFIG.MOM` are
     in the game directory itself — copy them straight across.
   - **GOG / Steam "Master of Magic Classic":** the modern re-release runs the
     original DOS game under an emulator, so the same DOS assets are **bundled
     inside the install folder** (typically a game-data subfolder). Carl browses
     his install directory, finds the `*.LBX` files and `CONFIG.MOM`, and copies
     them over.
   - Either way: **select all `.LBX` files + `CONFIG.MOM` → copy → paste into the
     folder with `ReMoMber`.**
   > **Version caveat:** ReMoM targets **v1.31** specifically. If Carl's copy is a
   > different revision, most assets still load, but some data may not line up
   > perfectly. If something looks off, the version mismatch is the first suspect.
4. **Launch** (shared flow below). Carl is playing within minutes of downloading.

**Verdict:** Carl is the target user, and his journey is smooth. His only real
friction is *locating* the files inside a modern re-release install — a "where are
they?" problem, not an "acquire the game" problem.

---

## Shared: first launch (per-platform quirks)

Once the `.LBX` files + `CONFIG.MOM` sit beside the executable:

- **Windows:** double-click `ReMoMber.exe`. Just works (silent).
- **Linux (AppImage):** `chmod +x ReMoM-*.AppImage`, then run it. Self-contained.
- **Linux (ZIP):** `./ReMoMber` — only if SDL2 is installed; otherwise use the AppImage.
- **macOS:** clear the quarantine flag once — `xattr -dr com.apple.quarantine ReMoMber` — then `./ReMoMber`.

The window opens titled `(...) Master of Magic v1.31 - Reassembly (v<ver>)`; the
version banner confirms exactly which build is running.

If the first launch fails, [PLAYING.md](../PLAYING.md)'s Troubleshooting covers the
two common cases: *"Cannot open MAGIC.LBX"* (data files missing / wrong folder —
the #1 issue) and a black screen on launch.

---

## Where the journey breaks down (ranked friction)

1. **No release exists yet** + the test-gate blocks the first tag → nothing to
   download today.
2. **Getting / locating the data files** → the biggest real drop-off. Painful for
   Sam (must acquire the game), mild for Carl (must find files in his install).
   Neither is helped by the current docs beyond a bare filename list.
3. **Discovery** → GitHub-repo-first is unfriendly to non-technical players; fine
   for community insiders like Carl, a barrier for newcomers like Sam.
4. **Platform quirks** (silent Windows, macOS quarantine, Linux ZIP needs SDL2) →
   documented, but rough edges.

**Highest-leverage improvement:** a "Where to get / find your game data files"
section in [PLAYING.md](../PLAYING.md) — covering DOS vs GOG/Steam install layouts
and the v1.31 caveat — would directly smooth step 2 for both Sam and Carl.
