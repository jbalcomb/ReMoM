# ReMoM — Master of Magic, Reassembled

**The 1994 classic, rebuilt from the original program and running natively on today's computers.**

Master of Magic was Simtex's game of wizards, spellbooks and conquest across two worlds, Arcanus and Myrror. ReMoM brings it back as a native program for modern Windows and Linux, with no DOSBox and no emulator. It's the same game you remember: the same rules, the same AI and the same spells, running at the speed it always did, in a window or full screen.

Bring your own copy of the game, and ReMoM does the rest.

▶ **[Watch the alpha demo](https://youtu.be/d-PlvxV_Ek0)**

---

## What you need

- **Your own copy of Master of Magic, version 1.31.** The GOG and Steam releases and the original disks all work. ReMoM ships the engine only: the art, music and game data come from your copy.
- **A 64-bit Windows or Linux computer.** macOS isn't supported yet.

## Get it

Download the latest version from the **[Releases page](https://github.com/jbalcomb/ReMoM/releases/latest)**:

| You're on | Download | Then |
|-----------|----------|------|
| Windows | `ReMoM-<version>-Windows-AMD64.exe` | Run the installer. |
| Windows, no install | `ReMoM-<version>-Windows-AMD64.zip` | Unzip it anywhere and run `ReMoM.exe`. |
| Debian, Ubuntu, Mint, Pop!_OS | `remom_<version>_amd64.deb` | `sudo apt install ./remom_<version>_amd64.deb`, then find ReMoM in your applications menu. |
| Any other Linux | `ReMoM-<version>-x86_64.AppImage` | `chmod +x` it, then run it. |

**Start it up.** If Master of Magic is installed from GOG, Steam or the original CD in the usual place, ReMoM finds it by itself. If it can't, it tells you which files it's missing, and [PLAYING.md](PLAYING.md) shows how to point it at your game folder.

**Your originals stay untouched.** ReMoM never writes to your Master of Magic folder. Saves and settings live in a folder of their own, so the original game keeps working exactly as before.

## Made for modern screens

- **Fullscreen from the start.** ReMoM opens full screen as a borderless window, so Alt+Tab is instant and your monitor never changes resolution. **Alt+Enter** switches to a normal window and back.
- **Sharp pixels.** By default every game pixel is scaled up by the same whole number, so the art stays crisp, with no blur and no wobble.
- **Or fill the screen.** Two other scale modes make the picture as big as your monitor allows. One keeps the pixels hard-edged; the other evens out their size with a slight soft edge.
- **The 1994 picture shape.** On a CRT monitor, Master of Magic's pixels were a little taller than they were wide. Switch to 4:3 and the screen looks the way it did back then: rounder circles and the original proportions.
- **Sound and music,** played through your computer's normal audio.

## Settings: `ReMoM.ini`

ReMoM keeps its settings in a plain text file called `ReMoM.ini`. It makes the file the first time you start the game, with every setting listed and explained:

| You're on | Your `ReMoM.ini` is in |
|-----------|------------------------|
| Windows | `%APPDATA%\ReMoM\` (paste that into the Explorer address bar) |
| Linux | `~/.config/ReMoM/` |

Open it in any text editor. A `;` at the start of a line switches that setting off, so the default is used. To change a setting, remove the `;` and edit the value:

```ini
[Video]
scale_mode = fit_sharp_bilinear
```

| Setting | What it does | Choices (default first) |
|---------|--------------|-------------------------|
| `game_data` | Where your Master of Magic files are, if ReMoM didn't find them by itself | a folder |
| `startup_window` | Whether the game starts fullscreen or in a window | `fullscreen`, `windowed` |
| `scale_mode` | How the picture is enlarged | `integer` (sharpest), `fit_nearest` (biggest), `fit_sharp_bilinear` (biggest, evenly sized pixels) |
| `aspect_ratio` | The picture's shape | `16:10` (square pixels), `4:3` (as on a 1994 monitor) |

A setting ReMoM doesn't understand is ignored, so a typo never stops the game from starting.

**New versions add their new settings for you.** They go into your `ReMoM.ini` switched off, with their explanation, and your file as it was is kept as `ReMoM_backup_<date and time>.ini`. Beside it, `ReMoM_example.ini` always shows every setting at its default. ReMoM never reads that one, and each new version replaces it.

**Keeping everything in one folder?** A `ReMoM.ini` next to `ReMoM.exe`, or next to the `.AppImage` file, is used instead of the one above: that whole file, and nothing from the other. [PLAYING.md](PLAYING.md) explains each setting in full.

## Is it finished?

Not yet, and we'd rather say so up front. ReMoM is rebuilt by hand from the original game program, one routine at a time, and the aim is to match the original exactly, bugs included. It's playable today. You'll still find rough edges, and every report helps.

**Found a bug?** Windows downloads include a diagnostic build, `ReMoM_diagnostic.exe`, that records what happened. Linux has a separate diagnostics download on the Releases page. [TESTING.md](TESTING.md) explains what to send us.

## Join us

Players, testers and tinkerers hang out in the **[#mom-reverse-engineering](https://discord.gg/kSNA8bE6Wt)** channel on the *Master of Magic – Fans* Discord server. Come say hello.

## For developers and modders

ReMoM is C source code that follows the original program closely, so you can read it, change it and build on it. Start with [CONTRIBUTING.md](CONTRIBUTING.md); [BUILDING.md](BUILDING.md) covers compiling it yourself.

---

*Master of Magic was created by Simtex and published by MicroProse in 1994. ReMoM is a fan project and includes none of the original game's art, music or data.*
