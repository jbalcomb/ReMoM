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
for ReMoM that is `ReMoM` + `libSDL2` + `libSDL2_mixer` + their transitive
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
link straight to the bundled executable (`usr/bin/ReMoM`) and relies on the
runtime to set `LD_LIBRARY_PATH` so the bundled libraries resolve. There is no
wrapper logic: the AppImage simply runs `ReMoM` with the environment and CWD it
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

1. **The executable dir is *inside* the mount.** Because `AppRun` → `usr/bin/ReMoM`,
   the running executable is `/tmp/.mount_XXXX/usr/bin/ReMoM`. So search-path
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
# ReMoM AppImage launcher.  (Abridged -- packaging/AppRun is the real thing.)
#   Portable: if the game data (FONTS.LBX) sits next to the .AppImage, keep
#   everything -- data, MAGIC.SET, saves, ReMoM.ini, logs -- in a "remom_app_dir/"
#   folder right there. Uses $APPIMAGE, so it does NOT depend on the current
#   directory.  Otherwise: normal install (per-user XDG dirs + GOG/Steam detect).
BIN="${APPDIR}/usr/bin/ReMoM"
HERE="$(dirname "$(readlink -f "${APPIMAGE:-$0}")")"

if [ -w "$HERE" ] && { [ -e "$HERE/FONTS.LBX" ] || [ -e "$HERE/fonts.lbx" ] || [ -d "$HERE/remom_app_dir" ]; }; then
    export XDG_DATA_HOME="$HERE"      # -> $HERE/remom_app_dir/  (MAGIC.SET, saves)
    export XDG_CONFIG_HOME="$HERE"    # -> $HERE/remom_app_dir/  (ReMoM.ini)
    export XDG_STATE_HOME="$HERE"     # -> $HERE/remom_app_dir/  (logs)
    export REMOM_APP_DIR="remom_app_dir"   # must not be "ReMoM" -- see below
    if [ -e "$HERE/FONTS.LBX" ] || [ -e "$HERE/fonts.lbx" ]; then
        export REMOM_DATA_DIR="${REMOM_DATA_DIR:-$HERE}"   # read LBX from here
    fi
fi
exec "$BIN" "$@"
```

Key properties:

- **CWD-independent.** It reads `$APPIMAGE` (always the path to the `.AppImage`), so
  it behaves the same whether you double-click or launch from a terminal.
- **Gated, not forced.** Portable engages only when the game data (`FONTS.LBX`) — or a
  previously-created `remom_app_dir/` folder — is present next to the `.AppImage` **and**
  that folder is writable. A system-installed or read-only AppImage falls straight back to
  normal XDG + auto-detect.
- **One tidy folder.** ReMoM appends `/$REMOM_APP_DIR` to each XDG dir, so setting all
  three to `$HERE` collapses them into a single **`<folder>/remom_app_dir/`** beside the
  `.AppImage`, instead of a hidden `~/.local/share/ReMoM`.
- **The XDG dirs are FORCED (`=`, not `:=`).** A desktop session or a sandbox
  (snap/flatpak) may already have `XDG_DATA_HOME` set; portable mode has to win, or it
  silently does nothing. `REMOM_DATA_DIR` keeps `:-` — an explicit data dir from the
  caller still wins, because that one is a genuine user choice rather than ambient state.
- **The app-dir name is not `ReMoM`.** It shares the folder with the executable, which is
  now also called `ReMoM`, and a filesystem cannot hold both. See the collision section
  below.

Result: drop your Master of Magic files in the folder with the `.AppImage`, launch it
however you like, and the LBX data + `MAGIC.SET` + saves + settings + logs all live
together in `<folder>/remom_app_dir/`.

---

## How it fits ReMoM's file model — the two modes

| | Portable (data beside the `.AppImage`) | Installed (no data beside it, or read-only) |
|---|---|---|
| Game data (LBX) | `REMOM_DATA_DIR = <folder>` | search path: auto-detect GOG/Steam, `REMOM_DATA_DIR`, `ReMoM.ini` |
| `MAGIC.SET` / saves / logs | `<folder>/remom_app_dir/` | `~/.local/share/ReMoM/` (per-user XDG) |
| Behaves like | a self-contained game folder | a native (non-AppImage) install |

Both modes go through the *same* seeding/`STU_GRAF_Open_User` code — the custom
`AppRun` only changes *where the dirs point*, not how ReMoM uses them. Nothing about
the game logic or the file contracts changes.

---

## Caveats and improvements

**Caveats**

- **Two copies of `MAGIC.SET` in portable mode.** ReMoM *seeds* rather than reading
  in place, so the `MAGIC.SET` you place in `<folder>/` is copied once into
  `<folder>/remom_app_dir/`, and the game then uses the copy. Both sit beside the `.AppImage`
  (not hidden), but there are two. Never-clobber means editing the source after the
  first run does not propagate — use `--orig-files` to re-seed.
- **Gate heuristic.** Portable triggers on `FONTS.LBX` (or an existing `remom_app_dir/`)
  beside the `.AppImage`. Launch *before* placing the data and that run is in installed
  mode.
- **Writability.** A read-only folder deliberately falls back to XDG — you cannot
  write saves next to a read-only `.AppImage`.
- **The portable folder is `remom_app_dir/`, not `ReMoM/`.** See below — the
  `ReMoMber` → `ReMoM` rename made the old name collide with the executable.

### Name collision: the app directory vs the `ReMoM` executable — resolved

> **Resolved.** Portable mode now writes to **`<folder>/remom_app_dir/`**, and
> `STU_GRAF` reports a blocked directory instead of silently mis-succeeding. What
> follows is why, kept because the reasoning governs anyone changing these names.

Portable mode creates a **directory** called `ReMoM/` beside the `.AppImage`. Since the
player executable was renamed from `ReMoMber` to `ReMoM`, a *file* named `ReMoM` and the
*directory* named `ReMoM` now compete for the same name in the same folder — and a POSIX
filesystem allows only one of them. On Linux there is no extension to keep them apart
(Windows is immune: `ReMoM.exe`).

**How a user gets into this.** The portable archives extract to a top-level folder
(`ReMoM-<ver>-Linux-x86_64/ReMoM`), so a plain `tar xf` next to the `.AppImage` is safe —
the binary lands in its own subdirectory. The collision needs a deliberate extra step:
copying or extracting the bare `ReMoM` binary *flat* into the portable folder. The obvious
motivation is "I want to try the plain build against the saves I already have here."

**Both orders fail, and the first one fails quietly.**

*Binary first, then run the AppImage* — the nastier case. `STU_GRAF`'s
[`graf_mkdir_one`](../../STU/src/STU_GRAF.c#L469) treats `EEXIST` as success:

```c
return (errno == EEXIST) ? 1 : 0;
```

That is correct for "the directory is already there", but a *regular file* named `ReMoM`
also makes `mkdir()` fail with `EEXIST`. So directory creation reports **success**, and
every later open of `<folder>/ReMoM/MAGIC.SET` fails with `ENOTDIR` instead. The user sees
missing saves and settings with no message pointing at the real cause.

*Directory first, then extract the binary over it* — depends on whether you have saves yet:

| State of `<folder>/ReMoM/` | Result |
|---|---|
| empty (portable mode ran, nothing saved) | `tar` **silently replaces the directory with the file**, exit 0 |
| populated (`MAGIC.SET`, `SAVE1.GAM`, …) | `tar: ReMoM: Cannot open: File exists`, exit 2 — extraction aborts partway |

Verified with GNU tar; `unzip` behaves the same way.

### What was done

**1. The portable app directory was renamed — in portable mode only.**

`STU_GRAF` composes every per-user path from one name (the `ReMoM` in
`~/.local/share/ReMoM`). That name now comes from a single accessor,
`graf_app_dir_name()`, which honours the **`REMOM_APP_DIR`** environment variable and
otherwise returns `STU_GRAF_APP_DIR_DEFAULT` (`"ReMoM"`). `packaging/AppRun` sets
`REMOM_APP_DIR=remom_app_dir` when — and only when — it engages portable mode:

```
<folder>/
  ReMoM-<ver>-x86_64.AppImage
  FONTS.LBX  ...              <- your Master of Magic data
  remom_app_dir/              <- MAGIC.SET, saves, ReMoM.ini, logs
  ReMoM                       <- a plain binary can now sit here safely
```

Nothing else sets the variable, so **installed and XDG users are untouched and need no
migration** — `~/.local/share/ReMoM/` keeps working exactly as before. This was free to
do because no released AppImage ever had portable mode: `v0.0.1` is `afe3171b` (2026-07-21)
and `packaging/AppRun` landed in `946b2a85` (2026-07-22). It would not have been free later.

The override is validated as a bare **name**: separators, `.` and `..` are rejected and fall
back to the default, so it can only ever pick a leaf directory under the resolved base and
can never redirect writes somewhere else.

**2. `graf_mkdir_one` no longer treats `EEXIST` as success.** It now `stat()`s the path and
succeeds only if a *directory* is really there, logging `LOG_ERROR` otherwise. This is
correct independently of the collision — any file blocking any app directory was previously
silent.

Worth knowing if you touch these tests: `STU_GRAF_Open_User` returns `NULL` **either way**,
because the doomed `fopen()` just fails with `ENOTDIR` instead. The log line is the only
externally visible difference between fixed and broken, so
`STU_GRAF.AppDirBlockedByFileIsDiagnosed` asserts on the log — and was checked to fail
against the pre-fix code. A test that only asserts the `NULL` passes either way and proves
nothing.

**Still true, and still worth avoiding:** don't mix a plain `ReMoM` binary into the
AppImage's portable folder out of habit. To run a plain build against the same data, run it
from its own extracted directory with `REMOM_DATA_DIR=<folder>`.

**Not done, deliberately:** renaming the app directory *globally*. On Windows and macOS the
per-user directories live nowhere near the executable, so a global rename buys nothing and
costs every existing user a migration. A case-only change (`ReMoM` → `remom`) was rejected
for a sharper reason: it is a real rename on case-sensitive Linux and a no-op on
case-insensitive macOS/APFS and Windows/NTFS, so migration would behave differently per
platform — skip it and Linux users lose their saves while everyone else is fine.

**Possible improvements**

- **In-place writes (one copy).** Teach `STU_GRAF_Open_User` an "in-place" mode that
  writes `MAGIC.SET` / saves directly into `REMOM_DATA_DIR` instead of a separate
  per-user dir. Removes the second copy; small, self-contained C change.
- **First-run message.** Print one line on first launch — *where* the data was found
  and *where* saves/settings are kept — so the location is never a surprise, in either
  mode.
- **Explicit toggle.** A `ReMoM.portable` marker file (or `--portable` flag) to force
  portable independently of the data-present heuristic.
