# Playing ReMoM

ReMoM is a source-code reconstruction of **Master of Magic** (Simtex, 1994-95).
You need the original game data files to play.

## Quick Start

1. Have your original **Master of Magic v1.31** files.
   A GOG, Steam, or original floppy/CD install all work.

2. Run the game:
   - **Windows:** `ReMoM.exe`
   - **Linux:** see **Which Linux download?** below — the `.deb` to install it
     properly, or the `.AppImage` to just run it.
   - **macOS:** not available yet.

   **ReMoM checks the usual GOG / Steam / CD install locations automatically** —
   if your Master of Magic is installed in one of those, it just works, with no
   setup at all.

3. If ReMoM can't find your data, it shows a dialog saying so. Point it at your
   Master of Magic folder — see **Telling ReMoM where your data is** below.

## Which Linux download?

Three artifacts, in the order most people want them:

| Download | Use it when | How |
|----------|-------------|-----|
| `remom_<ver>_amd64.deb` | You're on Debian, Ubuntu, Mint, Pop!_OS… and want it **installed** — in your application menu, uninstallable, SDL2 handled by the package manager. | `sudo apt install ./remom_<ver>_amd64.deb` |
| `ReMoM-<ver>-x86_64.AppImage` | Any other distro, or you'd rather not install anything. Bundles SDL2, so it runs anywhere. | `chmod +x` it, then run it |
| `ReMoM-<ver>-Linux-*.zip` / `.tar.gz` | You want the bare binary and already have SDL2. | extract, `./ReMoM` |

The `.deb` puts `ReMoM` on your `PATH`, so you can launch it from the menu or
just type `ReMoM`. `apt` pulls in SDL2 and SDL2_mixer for you. To remove it:

```sh
sudo apt remove remom
```

That deletes the program but leaves your saves and settings alone (they live in
your home directory — see below). There is no repository to add yet, so `apt`
won't auto-update it; watch the [Releases](https://github.com/jbalcomb/ReMoM/releases)
page and install the newer `.deb` over the top when one appears.

**The AppImage can sit right in your Master of Magic folder.** Put the `.AppImage` in the
folder with your game files and it uses them. Your settings, saves and logs still go in
the per-user folders below, the same as with the `.deb`.

Older AppImages (0.99.1 and before) kept settings and saves in a `remom_app_dir/` folder
next to the `.AppImage` instead. The first time you run a newer one, it copies them to the
per-user folders, without overwriting anything already there, and leaves a `MOVED.txt` in
`remom_app_dir/` listing what went where. After that you can delete `remom_app_dir/`.

## Telling ReMoM where your data is

Only needed if auto-detect didn't find your install (e.g. it's in a
non-standard folder). ReMoM searches these places in order and uses the first
one that has the data:

1. the `REMOM_DATA_DIR` environment variable,
2. the `game_data` path in your `ReMoM.ini`,
3. the folder next to `ReMoM`,
4. the usual GOG / Steam / CD install locations (auto-detected),
5. the current working directory.

Pick whichever is easiest:

**`REMOM_DATA_DIR` — quickest.** Point it at the folder that holds your `.LBX`
files:

- **Linux / macOS:** `REMOM_DATA_DIR="/games/Master of Magic" ./ReMoM`
  (add `export REMOM_DATA_DIR="/games/Master of Magic"` to `~/.bashrc` /
  `~/.zshrc` to make it permanent).
- **Windows (PowerShell):** `$env:REMOM_DATA_DIR = "D:\Master of Magic"; .\ReMoM.exe`
- **Windows (cmd):** `set REMOM_DATA_DIR=D:\Master of Magic` then `ReMoM.exe`

**`ReMoM.ini` — permanent, no environment variable.** Open your `ReMoM.ini` (see
*Your settings file* below), find the `game_data` line, remove the `;` in front of it and
put in your folder:

```ini
[Paths]
game_data = /games/Master of Magic
```

The path may contain spaces (no quotes needed).

**Copy the files next to `ReMoM`.** Or just drop every `.LBX` file into the
same folder as the executable — no config needed. (`CONFIG.MOM` is optional —
it's the old DOS sound config; without it ReMoM just uses defaults.)

## Fullscreen, window and picture

ReMoM starts **fullscreen**: a borderless window covering your whole monitor, taskbar included. Your screen never changes
resolution, and Alt+Tab is instant. (ReMoM has no *exclusive* fullscreen, the old kind that switches the monitor's resolution.)
While you're in fullscreen, the mouse stays on the game picture until you Alt+Tab away.

Press **Alt+Enter** at any time to switch to a normal window, which you can move and resize, and again to go back. The game
itself never sees Alt+Enter, so it can't trigger anything in-game.

To choose how ReMoM **starts**, edit the `[Video]` section of your `ReMoM.ini` (see *Your settings file* below). Remove the `;`
in front of a setting to switch it on:

```ini
[Video]
startup_window = windowed
scale_mode = integer
aspect_ratio = 4:3
```

| Setting | Choices | Default |
|---------|---------|---------|
| `startup_window` | `fullscreen`: the whole monitor, as above.<br>`windowed`: a normal window you can move and resize. | `fullscreen` |
| `scale_mode` | `integer`: the biggest whole-number scale that fits, black bars around it. Every pixel the same size, perfectly crisp.<br>`fit_nearest`: as big as the screen allows. Fills more of it, but some pixel rows and columns come out one screen pixel wider than others.<br>`fit_sharp_bilinear`: the same size as `fit_nearest`, every pixel the same size, with the edges between pixels softened very slightly. | `integer` |
| `aspect_ratio` | `16:10`: square pixels.<br>`4:3`: the shape the game had on a 1994 monitor, each pixel a little taller than wide, so circles come out round. In a window, it opens at 1280x960. | `16:10` |

A value ReMoM doesn't understand is ignored, and the default is used, so a typo never stops the game from starting.

## Your settings file

ReMoM makes `ReMoM.ini` the first time you start it, with every setting in it switched off (a `;` at the start of the line)
and explained, so the defaults are used until you change one:

| OS | Location of `ReMoM.ini` |
|----|-------------------------|
| Windows | `%APPDATA%\ReMoM\ReMoM.ini` (paste `%APPDATA%\ReMoM` into the Explorer address bar) |
| Linux | `~/.config/ReMoM/ReMoM.ini` |

- **`ReMoM_example.ini`**, in the same folder, shows every setting at its default. ReMoM never reads it, and each new
  version replaces it.
- **When a new version adds settings**, they're added to your `ReMoM.ini`, switched off, with their explanation. Your file
  as it was is kept beside it as `ReMoM_backup_<date and time>.ini`. Nothing you wrote is changed.
- **A `ReMoM.ini` next to the program** (next to `ReMoM.exe`, or next to the `.AppImage` file) is used instead, if you make
  one: that whole file, and nothing from the one above. It gets new settings, a backup and an example file the same way.
  That's handy for a copy kept on a USB stick.

To see which file ReMoM used, look for the `settings: using` line near the top of its log.

## Where ReMoM keeps your saves and settings

ReMoM never writes into your original Master of Magic folder — it keeps its own
working copies of `CONFIG.MOM`, `MAGIC.SET`, and your save games in a per-user
data folder:

| OS | ReMoM data folder |
|----|-------------------|
| Linux | `~/.local/share/ReMoM/` |
| Windows | `%APPDATA%\ReMoM\` |
| macOS | `~/Library/Application Support/ReMoM/` |

On first run it copies those files (including any existing `SAVE1.GAM`–`SAVE9.GAM`
that sit next to your game data) into that folder; from then on it reads and
writes only its own copies, so your originals stay put.

To **reset ReMoM's working files from your original data** — for example after
replacing your game files — run:

```sh
ReMoM --orig-files
```

This re-copies `CONFIG.MOM`, `MAGIC.SET`, and `SAVE1-9.GAM` from your original
Master of Magic folder, first moving the current copies into a timestamped
`backup-YYYYMMDD-HHMMSS/` folder inside the ReMoM data folder (so nothing is
lost).

## Troubleshooting

- **"ReMoM could not find your Master of Magic game data"** — the `.LBX` files
  aren't where ReMoM looked. Either copy them next to `ReMoM`, or point ReMoM
  at them with `REMOM_DATA_DIR` or `ReMoM.ini` (see *Telling ReMoM where your
  data is* above). The dialog lists which files are missing.

- **Black screen on launch** — try resizing or alt-tabbing. Some systems
  need a moment for the SDL window to initialize.

- **Mouse cursor looks wrong or disappears** — ReMoM draws the cursor as a
  hardware/OS cursor for smoothness. If it misbehaves on your GPU or driver, fall
  back to the old software cursor by setting `REMOM_HW_CURSOR=0` (same way you'd
  set `REMOM_DATA_DIR` above).

## Links

- Project page: https://github.com/jbalcomb/ReMoM
- Discord: [#mom-reverse-engineering](https://discord.gg/kSNA8bE6Wt)
  on the "Master of Magic - Fans" server
