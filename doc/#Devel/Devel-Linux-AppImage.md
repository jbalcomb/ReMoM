# Linux AppImage — how ReMoM ships, and why it deviates from the default

This explains the AppImage format, how one normally behaves, why ReMoM ships a
**custom `AppRun`** instead of the stock one, and how that choice fits ReMoM's
game-data / config / save-file model.

- Player-facing "how do I run it": see [`PLAYING.md`](../../PLAYING.md).
- The release/packaging pipeline (the `linuxdeploy` step): see [`RELEASES.md`](../../RELEASES.md).

> **Status:** implemented. `packaging/AppRun` is the custom launcher, wired into the
> AppImage build via `linuxdeploy --custom-apprun` in `release.yml`. The
> file-resolution model below is what ReMoM does today.

---

## What an AppImage is

A single-file, distribution-agnostic Linux application bundle. One `.AppImage`
file is the whole app — no install, no package manager, no root.

Structurally it is a small ELF **runtime** prepended to a compressed **squashfs**
image (the "AppDir"). Running the file mounts that squashfs (via FUSE, or via
`--appimage-extract-and-run` when FUSE is unavailable) and executes **`AppRun`**
from inside it. The AppDir carries the executable plus every bundled library —
for ReMoM that is `ReMoMber` + `libSDL2` + `libSDL2_mixer` + their transitive
dependencies, gathered by [`linuxdeploy`](https://github.com/linuxdeploy/linuxdeploy).

Environment variables the runtime sets for the app:

| Var | Meaning |
|-----|---------|
| `$APPDIR` | the squashfs mount point (read-only), e.g. `/tmp/.mount_XXXX` |
| `$APPIMAGE` | absolute path to the `.AppImage` **file** itself |
| `$OWD` | the working directory at launch ("original working directory") |
| `$ARGV0` | how the app was invoked |

**The runtime does not change the working directory.** CWD is inherited from
whatever launched the AppImage (a shell, a file manager, a `.desktop` entry).

---

## How an AppImage normally works

`AppRun` is the entry point. `linuxdeploy`'s **default** makes `AppRun` a symbolic
link straight to the bundled executable (`usr/bin/ReMoMber`) and relies on the
runtime to set `LD_LIBRARY_PATH` so the bundled libraries resolve. There is no
wrapper logic: the AppImage simply runs `ReMoMber` with the environment and CWD it
inherited. **Where the app looks for data/config is entirely the app's problem.**

There is also a native **portable mode**: if a directory named `<appimage>.home`
exists next to the `.AppImage`, the runtime redirects `$HOME` to it (and
`<appimage>.config` → `$XDG_CONFIG_HOME`). It is opt-in and hijacks *all* of
`$HOME`, so anything the app reads from home moves with it.

---

## How ReMoM resolves files

ReMoM keeps two deliberately separate families of files:

**1. Read-only game assets** — the `*.LBX` data. Resolved through an ordered search
path (`STU_GRAF_Open_Asset`), first hit wins:

1. `REMOM_DATA_DIR` environment variable
2. a cache dir (modified copies)
3. `ReMoM.ini` → `[Paths] game_data`
4. the executable's own directory
5. GOG / Steam / CD auto-detect (common install locations)
6. the current working directory (CWD)

**2. Writable per-user files** — `MAGIC.SET`, `CONFIG.MOM`, `SAVE1-9.GAM`, and logs.
Resolved via `STU_GRAF_Open_User` into the platform's per-user dirs. On Linux these
are the XDG dirs:

- data (`MAGIC.SET`, `CONFIG.MOM`, saves) → `$XDG_DATA_HOME/ReMoM` (default `~/.local/share/ReMoM`)
- config (`ReMoM.ini`) → `$XDG_CONFIG_HOME/ReMoM`
- logs → `$XDG_STATE_HOME/ReMoM`

**First-run seeding:** on first PLAYER launch, ReMoM copies any `MAGIC.SET` /
`CONFIG.MOM` / saves it finds on the read-only search path into the per-user dir —
**copy-if-absent, never clobber.** After that it reads and writes only its own
per-user copies, so the player's original data is never modified. `--orig-files`
forces a re-seed (backing up the current copies first).

---

## Why the default `AppRun` is wrong for this model

Combine the two families above with the stock symlink `AppRun`, and the documented
"copy your Master of Magic files next to the AppImage" workflow becomes silently
fragile:

1. **The executable dir is *inside* the mount.** Because `AppRun` → `usr/bin/ReMoMber`,
   the running executable is `/tmp/.mount_XXXX/usr/bin/ReMoMber`. So search-path
   entry #4 ("executable dir") points into the read-only squashfs — **never** next to
   the `.AppImage` file. Files dropped beside the `.AppImage` are not on that entry.

2. **Only CWD can see those files — and CWD depends on launch method.**
   `./ReMoM-….AppImage` from a terminal *in that folder* → CWD = the folder → found.
   Double-click in a file manager → CWD = your home dir → **not** found. The
   instruction quietly depends on how you started the program.

3. **Saves live somewhere else anyway.** Even when the data is found, `MAGIC.SET` and
   saves are read from `~/.local/share/ReMoM/` (seeded once). Never-clobber means a
   stale or empty per-user copy shadows the files the user just dropped next to the
   `.AppImage`, and new saves land in the hidden dir — "the saves don't show" beside
   the game.

Net effect: the workflow depends on the launch method *and* a clean per-user dir,
and the player's saves are tucked away from the game folder.

---

## The deviation — a custom `AppRun` for portable mode

ReMoM ships a custom `AppRun` (via `linuxdeploy --custom-apprun`) that derives the
real folder from **`$APPIMAGE`** — not CWD — and, when it looks like the user wants
portable, points ReMoM's dirs right there:

```sh
#!/bin/sh
# ReMoM AppImage launcher.
#   Portable: if the game data (FONTS.LBX) sits next to the .AppImage, keep
#   everything -- data, MAGIC.SET, saves, ReMoM.ini, logs -- in a "ReMoM/" folder
#   right there. Uses $APPIMAGE, so it does NOT depend on the current directory.
#   Otherwise: normal install (per-user XDG dirs + GOG/Steam auto-detect).
BIN="${APPDIR}/usr/bin/ReMoMber"
HERE="$(dirname "$(readlink -f "${APPIMAGE:-$0}")")"

if { [ -e "$HERE/FONTS.LBX" ] || [ -e "$HERE/fonts.lbx" ] || [ -d "$HERE/ReMoM" ]; } && [ -w "$HERE" ]; then
    export REMOM_DATA_DIR="${REMOM_DATA_DIR:-$HERE}"          # read LBX + user's MoM files here
    : "${XDG_DATA_HOME:=$HERE}"  ; export XDG_DATA_HOME       # -> $HERE/ReMoM/  (MAGIC.SET, saves)
    : "${XDG_CONFIG_HOME:=$HERE}"; export XDG_CONFIG_HOME     # -> $HERE/ReMoM/  (ReMoM.ini)
    : "${XDG_STATE_HOME:=$HERE}" ; export XDG_STATE_HOME      # -> $HERE/ReMoM/  (logs)
fi
exec "$BIN" "$@"
```

Key properties:

- **CWD-independent.** It reads `$APPIMAGE` (always the path to the `.AppImage`), so
  it behaves the same whether you double-click or launch from a terminal.
- **Gated, not forced.** Portable engages only when the game data (`FONTS.LBX`) — or a
  previously-created `ReMoM/` folder — is present next to the `.AppImage` **and** that
  folder is writable. A system-installed or read-only AppImage falls straight back to
  normal XDG + auto-detect.
- **One tidy folder.** Because ReMoM appends `/ReMoM` to each XDG dir, setting
  `XDG_DATA/CONFIG/STATE_HOME = $HERE` collapses them into a single **`<folder>/ReMoM/`**
  beside the `.AppImage`, instead of a hidden `~/.local/share/ReMoM`.
- **Respects the caller.** The `:-` / `:=` defaults mean an explicit `REMOM_DATA_DIR`
  or `XDG_*` set by the user still wins.

Result: drop your Master of Magic files in the folder with the `.AppImage`, launch it
however you like, and the LBX data + `MAGIC.SET` + saves + settings + logs all live
together in `<folder>/ReMoM/`.

---

## How it fits ReMoM's file model — the two modes

| | Portable (data beside the `.AppImage`) | Installed (no data beside it, or read-only) |
|---|---|---|
| Game data (LBX) | `REMOM_DATA_DIR = <folder>` | search path: auto-detect GOG/Steam, `REMOM_DATA_DIR`, `ReMoM.ini` |
| `MAGIC.SET` / saves / logs | `<folder>/ReMoM/` | `~/.local/share/ReMoM/` (per-user XDG) |
| Behaves like | a self-contained game folder | a native (non-AppImage) install |

Both modes go through the *same* seeding/`STU_GRAF_Open_User` code — the custom
`AppRun` only changes *where the dirs point*, not how ReMoM uses them. Nothing about
the game logic or the file contracts changes.

---

## Caveats and improvements

**Caveats**

- **Two copies of `MAGIC.SET` in portable mode.** ReMoM *seeds* rather than reading
  in place, so the `MAGIC.SET` you place in `<folder>/` is copied once into
  `<folder>/ReMoM/`, and the game then uses the copy. Both sit beside the `.AppImage`
  (not hidden), but there are two. Never-clobber means editing the source after the
  first run does not propagate — use `--orig-files` to re-seed.
- **Gate heuristic.** Portable triggers on `FONTS.LBX` (or an existing `ReMoM/`)
  beside the `.AppImage`. Launch *before* placing the data and that run is in installed
  mode.
- **Writability.** A read-only folder deliberately falls back to XDG — you cannot
  write saves next to a read-only `.AppImage`.

**Possible improvements**

- **In-place writes (one copy).** Teach `STU_GRAF_Open_User` an "in-place" mode that
  writes `MAGIC.SET` / saves directly into `REMOM_DATA_DIR` instead of a separate
  per-user dir. Removes the second copy; small, self-contained C change.
- **First-run message.** Print one line on first launch — *where* the data was found
  and *where* saves/settings are kept — so the location is never a surprise, in either
  mode.
- **Explicit toggle.** A `ReMoM.portable` marker file (or `--portable` flag) to force
  portable independently of the data-present heuristic.
