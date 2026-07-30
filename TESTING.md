# Testing ReMoM — the Diagnostic build

Thanks for helping test ReMoM. This guide is for the **diagnostic build** — a special
version that produces detailed logs and readable crash reports so we can track down bugs
on your machine.

> **Linux tester?** The diagnostic build isn't bundled into your download. Instead, grab
> the separate **`ReMoM-<ver>-linux-diagnostics.zip`** from the release's Assets — it has a
> verbose `ReMoM_diagnostic` build plus `remom_video_probe` (the graphics tester) and its
> own README. The rest of this page is Windows-specific; the Linux zip's README covers the
> Linux equivalents (logs live in `~/.local/state/ReMoM/`, run `./remom_video_probe` for
> graphics problems).

## What it is (and where it already is)

The normal Windows download — the installer **and** the portable ZIP — ships **three**
programs side by side:

- **`ReMoM.exe`** — the normal game.
- **`ReMoM_diagnostic.exe`** — the **diagnostic build** for bug hunting.
- **`remom_video_probe.exe`** — the **graphics tester**, for "won't start" and "choppy".

You don't download anything extra. If you already installed ReMoM, you already have all
three. The diagnostic build plays exactly the same game (same graphics, same SDL2
backend), but it writes **verbose logs**, trips **assertions** that pinpoint problems, and
ships a symbol file so crashes produce a **readable stack trace**. It's unoptimized, so
it's a little slower and bigger — that's expected; don't judge performance by it.

### If ReMoM won't start at all, or the picture is choppy

Run **`remom_video_probe.exe`** *first* — a crash before any window appears usually can't
write a useful log, and this is the tool that finds it anyway. Open a Command Prompt in
your ReMoM folder and run:

```
remom_video_probe.exe                  ..tests every graphics driver, one at a time
remom_video_probe.exe --timing         ..for "choppy": refresh rate + frame pacing
```

It opens a few small windows briefly, then prints a table. **Copy the whole output into
your report.** It is safe to run even if ReMoM itself crashes — each test runs in its own
separate process, so a driver that crashes takes down only that one test and the table
still finishes.

If a row says `CRASH`, that graphics driver is broken on your machine. You can force
ReMoM onto a working one by setting an environment variable before launching — e.g. from
a Command Prompt in the ReMoM folder:

```
set SDL_RENDER_DRIVER=software
ReMoM.exe
```

Tell us if that fixes it; that's the single most useful fact in the report.

## Running it

1. Make sure your Master of Magic v1.31 data files are set up — same as the normal game.
   If not, see [PLAYING.md](PLAYING.md) ("Telling ReMoM where your data is").
2. Open your ReMoM folder:
   - If you used the installer: Start Menu → **ReMoM → "ReMoM Game Data Folder"**
     (that opens the install folder), or go to `%LOCALAPPDATA%\ReMoM`.
   - If you used the ZIP: wherever you unzipped it.
3. Double-click **`ReMoM_diagnostic.exe`**.

That's it — no command line, no settings to change.

## The logs — and what to send us

ReMoM writes rolling log files. On Windows they're in:

```
%LOCALAPPDATA%\ReMoM\logs\
```

(Paste that into the File Explorer address bar.) You'll find:

| File | What it is |
|------|-----------|
| `remom_log_current.txt` | this run's log — **the one we usually need** |
| `remom_log_previous.txt` | the run before this one |
| `remom_log_new.txt` | scratch file for a run in progress |

Reproduce the bug, quit, then send us **`remom_log_current.txt`** (add
`remom_log_previous.txt` if the problem was on the run before). The **Collect a report**
tool below can bundle these for you automatically.

## If it crashes

The diagnostic build catches crashes and writes a **stack trace** — lines starting with
`[stu-brak]` — into the log. Because the build ships with its symbol file, those lines
name the actual functions and source locations, which is exactly what we need. **Just send
the log** — the crash trace is already in it.

## Reporting a bug — please include

- **What you did** — steps to reproduce, and which screen you were on.
- **A save file and/or seed** if you can — lets us reproduce it exactly. Saves are in
  `%APPDATA%\ReMoM` (`SAVE1.GAM`–`SAVE9.GAM`). *(Note: saves and logs are in **different**
  folders — saves in `%APPDATA%`, logs in `%LOCALAPPDATA%`.)*
- **`remom_log_current.txt`** (see above).
- **Your Windows version.**

The version you're running is in the window title bar (e.g. `... (v0.0.5)`), and it's also
recorded at the top of the log.

## Collect a report automatically (optional)

If you have **Python 3** installed, this bundles the logs plus your system info into one
zip you can attach to a report — cross-platform, no arguments needed:

```
python3 tools/collect_report.py
```

It finds the log folder for your OS on its own and writes `remom_report_<timestamp>.zip`
in the current folder. Python is **not required** to run or test the game — this is just a
convenience for filing reports. Without it, just attach `remom_log_current.txt` by hand.

## If the mouse cursor misbehaves

ReMoM draws the cursor as a hardware/OS cursor. If it looks wrong or disappears on your
GPU, that itself is worth reporting — and you can fall back to the old software cursor;
see the note in [PLAYING.md](PLAYING.md) ("Troubleshooting").

## Where to reach us

- Issues: https://github.com/jbalcomb/ReMoM/issues
- Discord: [#mom-reverse-engineering](https://discord.gg/kSNA8bE6Wt) on the "Master of Magic - Fans" server

*(Developer detail on crash symbolization lives in `doc/#Devel/Devel-Crash-Reports.md`.)*
