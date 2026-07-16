# Playing ReMoM

ReMoM is a source-code reconstruction of **Master of Magic** (Simtex, 1994-95).
You need the original game data files to play.

## Quick Start

1. Have your original **Master of Magic v1.31** files — every `.LBX` file and
   `CONFIG.MOM`. A GOG, Steam, or original-CD install all work.

2. Run the game:
   - **Windows:** `ReMoMber.exe`. *(The Windows build currently runs silent — no audio yet.)*
   - **Linux:** the `.AppImage` (`chmod +x` it first), or `./ReMoMber` from the ZIP if you have SDL2 installed.
   - **macOS:** `./ReMoMber`. It's unsigned, so the first time clear the quarantine flag:
     `xattr -dr com.apple.quarantine ReMoMber`.

   **ReMoM checks the usual GOG / Steam / CD install locations automatically** —
   if your Master of Magic is installed in one of those, it just works, with no
   setup at all.

3. If ReMoM can't find your data, it shows a dialog saying so. Point it at your
   Master of Magic folder — see **Telling ReMoM where your data is** below.

## Telling ReMoM where your data is

Only needed if auto-detect didn't find your install (e.g. it's in a
non-standard folder). ReMoM searches these places in order and uses the first
one that has the data:

1. the `REMOM_DATA_DIR` environment variable,
2. the `game_data` path in your `ReMoM.ini`,
3. the folder next to `ReMoMber`,
4. the usual GOG / Steam / CD install locations (auto-detected),
5. the current working directory.

Pick whichever is easiest:

**`REMOM_DATA_DIR` — quickest.** Point it at the folder that holds your `.LBX`
files and `CONFIG.MOM`:

- **Linux / macOS:** `REMOM_DATA_DIR="/games/Master of Magic" ./ReMoMber`
  (add `export REMOM_DATA_DIR="/games/Master of Magic"` to `~/.bashrc` /
  `~/.zshrc` to make it permanent).
- **Windows (PowerShell):** `$env:REMOM_DATA_DIR = "D:\Master of Magic"; .\ReMoMber.exe`
- **Windows (cmd):** `set REMOM_DATA_DIR=D:\Master of Magic` then `ReMoMber.exe`

**`ReMoM.ini` — permanent, no environment variable.** Create a plain text file
named `ReMoM.ini` containing:

```ini
[Paths]
game_data=/games/Master of Magic
```

The path may contain spaces (no quotes needed). Put the file here:

| OS | Location of `ReMoM.ini` |
|----|-------------------------|
| Linux | `~/.config/ReMoM/ReMoM.ini` |
| Windows | `%APPDATA%\ReMoM\ReMoM.ini` |
| macOS | `~/Library/Application Support/ReMoM/ReMoM.ini` |

(Create the `ReMoM` folder if it doesn't exist.)

**Copy the files next to `ReMoMber`.** Or just drop every `.LBX` and
`CONFIG.MOM` into the same folder as the executable — no config needed.

## Troubleshooting

- **"ReMoM could not find your Master of Magic game data"** — the `.LBX` files
  and `CONFIG.MOM` aren't where ReMoM looked. Either copy them next to
  `ReMoMber`, or point ReMoM at them with `REMOM_DATA_DIR` or `ReMoM.ini` (see
  *If your game data is somewhere else* above). The dialog lists which files are
  missing.

- **Black screen on launch** — try resizing or alt-tabbing. Some systems
  need a moment for the SDL window to initialize.

## Links

- Project page: https://github.com/jbalcomb/ReMoM
- Discord: [#mom-reverse-engineering](https://discord.gg/kSNA8bE6Wt)
  on the "Master of Magic - Fans" server
