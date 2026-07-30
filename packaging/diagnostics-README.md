# ReMoM — Linux diagnostics

This is **not** the game. It's a small toolkit for tracking down a graphics or crash
problem on your machine, so we can fix it. Two programs:

| Program | What it's for |
|---------|---------------|
| `remom_video_probe` | Tests your display/graphics drivers — the fix for "won't start" and "choppy" reports. |
| `ReMoM_diagnostic`  | The game, built to write **verbose logs** and readable crash traces (unoptimized, so a little slower — that's expected). |

You only need these if something's wrong with the normal download. When you're done, send
us the output (see the bottom).

## First: you need SDL2

Both programs use SDL2. Most desktops already have it; if not:

```sh
sudo apt install libsdl2-2.0-0 libsdl2-mixer-2.0-0      # Debian / Ubuntu / Mint / Pop!_OS
```

(`ReMoM_diagnostic` also needs your Master of Magic v1.31 data files, same as the normal
game — see the main `PLAYING.md`.)

Make them executable if your download didn't keep the bit:

```sh
chmod +x remom_video_probe ReMoM_diagnostic
```

## `remom_video_probe` — graphics driver tester

No data files needed. Run it and paste us the output.

```sh
./remom_video_probe            # tests every video + render driver combination
./remom_video_probe --timing   # display refresh rates + frame-pacing / vsync jitter
./remom_video_probe --list     # just list the drivers, create no windows
```

- **`won't start` / crashes on launch:** run `./remom_video_probe`. It tries each driver
  combination in a separate process, so even the one that crashes can't stop the test — you
  get a full table of what works. If it shows `software` as OK, you can run the game with
  `SDL_RENDER_DRIVER=software ./ReMoM_diagnostic`.
  Each row also prints `[actual: X]` — the driver that really got used. If that differs
  from the one on the left, the requested one wasn't available and SDL quietly picked
  another, so that row tells you nothing about the driver you asked for. Paste the whole
  table either way; don't try to interpret it yourself.
- **`choppy` / stuttering:** run `./remom_video_probe --timing`. It reports your monitor's
  refresh rate and how steadily frames land. If you have a gaming monitor, try turning
  **off** its VRR / G-Sync / FreeSync and any low-latency/strobing mode, then re-run.

## `ReMoM_diagnostic` — the verbose game build

Runs the game exactly like normal, but logs everything. Useful env knobs (set them before
the command, same line):

```sh
./ReMoM_diagnostic                                  # normal, verbose logging
SDL_RENDER_DRIVER=software ./ReMoM_diagnostic       # skip OpenGL (fixes many "won't start")
REMOM_SDL_LOG=1            ./ReMoM_diagnostic        # also capture SDL's own internal log
REMOM_DATA_DIR="/path/to/MoM" ./ReMoM_diagnostic    # point at your game data explicitly
```

Logs are written to (honoring `$XDG_STATE_HOME` if you've set it):

```
~/.local/state/ReMoM/
```

- `remom_log_current.txt` — this run's log (**the one we usually need**)
- `remom_log_previous.txt` — the run before it

Reproduce the problem, quit, then send `remom_log_current.txt`. If it crashed, the stack
trace (lines starting with `[stu-brak]`) is already in that log — just send the log.

## What to send us

1. The **`remom_video_probe`** output (copy-paste the terminal, or `./remom_video_probe --timing > probe.txt` and attach `probe.txt`).
2. **`~/.local/state/ReMoM/remom_log_current.txt`**.
3. What you did, and your distro + desktop (e.g. "Kubuntu 26.04, Wayland").

If you have Python 3, this bundles the logs + system info into one zip for you:

```sh
python3 collect_report.py      # writes remom_report_<timestamp>.zip
```

## Where to reach us

- Issues: https://github.com/jbalcomb/ReMoM/issues
- Discord: [#mom-reverse-engineering](https://discord.gg/kSNA8bE6Wt) on the "Master of Magic - Fans" server
