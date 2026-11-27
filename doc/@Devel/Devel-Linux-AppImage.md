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

3. **The settings file is out of reach too.** A `ReMoM.ini` "next to the program" means next to
   the running executable, which is inside the mount. A player who keeps the `.AppImage` in
   their game folder and puts a `ReMoM.ini` beside it expects that file to be used.

Net effect: without help, the "put the AppImage in your game folder" workflow depends on the
launch method, and a `ReMoM.ini` beside the `.AppImage` is never read.

---

## The custom `AppRun`

ReMoM ships a custom `AppRun` (via `linuxdeploy --custom-apprun`), `packaging/AppRun`. It works
out the folder holding the `.AppImage` from **`$APPIMAGE`**, not the CWD, so it behaves the same
whether you double-click or launch from a terminal, and passes that folder on:

| `AppRun` sets | When | ReMoM uses it for |
|---|---|---|
| `REMOM_PROGRAM_DIR = <folder>` | always (when run as an `.AppImage`) | "next to the program": a `ReMoM.ini` here is used instead of `~/.config/ReMoM/ReMoM.ini` (`src/Settings_Ini.c`) |
| `REMOM_DATA_DIR = <folder>` | `FONTS.LBX` is in the folder, and the caller didn't set it | the game data (an explicit `REMOM_DATA_DIR` still wins) |

Everything else is the normal per-user layout, the same as the `.deb`:

| | Where |
|---|---|
| Settings (`ReMoM.ini`, `ReMoM_example.ini`, backups) | `~/.config/ReMoM/`, or `<folder>/ReMoM.ini` if the player made one (that whole file) |
| `MAGIC.SET`, `CONFIG.MOM`, saves | `~/.local/share/ReMoM/` |
| Logs | `~/.local/state/ReMoM/` |

The rule for `ReMoM.ini` is the same on every download; see `doc/@AI_Plans/PLAN-ReMoM-Ini.md`.

### Moving players off the old portable folder

AppImages up to and including 0.99.1 had a "portable mode": with the game files next to the
`.AppImage`, `AppRun` pointed `XDG_DATA_HOME`, `XDG_CONFIG_HOME` and `XDG_STATE_HOME` at that folder
and set `REMOM_APP_DIR=remom_app_dir`, so settings, saves and logs went into `<folder>/remom_app_dir/`.
Players never asked for it, and it silently ignored `~/.config/ReMoM/ReMoM.ini`, so it was removed.

`AppRun` now moves an existing `remom_app_dir/` over once:

- `ReMoM.ini` is copied to `~/.config/ReMoM/`, and `MAGIC.SET`, `CONFIG.MOM` and `SAVE*.GAM` to
  `~/.local/share/ReMoM/` (names matched case-insensitively);
- a file already at the destination is kept, never overwritten; if the two are identical (`cmp -s`) the report says so, and if
  they differ the folder's copy is marked "still only here", with both modification dates;
- `remom_app_dir/MOVED.txt` lists what went where, and its presence stops the move running again;
- the folder and its old logs are left for the player to delete. `MOVED.txt` says the folder can be deleted only when nothing is
  marked "still only here" (a differing file, or one that could not be copied); otherwise it says to keep it. (0.99.2's first build
  said "You can delete this folder" even when a newer save in it had not been copied.)

`STU_GRAF` still honours `REMOM_APP_DIR` (a bare directory name; separators, `.` and `..` are
rejected), but nothing sets it any more.

---

## Caveats

- **Two copies of `MAGIC.SET`.** ReMoM *seeds* rather than reading in place, so a `MAGIC.SET`
  in the game folder is copied once into `~/.local/share/ReMoM/` and the game uses the copy.
  Never-clobber means editing the original after the first run does not propagate; use
  `--orig-files` to re-seed.
- **A read-only folder.** `ReMoM.ini` next to a read-only `.AppImage` is still read, but ReMoM
  cannot add new settings to it or write `ReMoM_example.ini` beside it; it logs a warning and
  carries on.
- **A plain `ReMoM` binary next to the `.AppImage`** no longer collides with anything: nothing
  creates a directory there any more.
- **SDL3 on Wayland: no OS pointer warp while the mouse is captured (mutter 49.0 crash).** On GNOME 49 / mutter 49.0,
  SDL3's warp of a captured pointer at startup crashed GNOME Shell and the whole desktop session (SIGFPE in
  `meta_window_update_visibility` after "Invalid window geometry for xdg_surface"). Proven by bisecting in a throwaway login
  session. The backend skips that warp and moves only the game cursor. Do not remove the check; details in the
  `MUTTER-49.0-CRASH` note in `platform/sdl3/sdl3_MD.c`. Test SDL3 Linux changes in a throwaway login session, never on a
  desktop with work open.
