# Linux Graphics — startup crashes and choppy framerate, and how to diagnose them

Two families of Linux display report live here: a **hard crash in `SDL_CreateRenderer`** at
startup (GLX-over-XWayland and friends), and a **choppy / stuttering framerate** (usually a
gaming monitor's variable-refresh path). Both are diagnosed with the same tool,
`tools/remom_video_probe`, and the same startup log block.

ReMoM is a 320×200 palettized blit. It does **not** ask for OpenGL — the window is
created with `SDL_WINDOW_RESIZABLE` only ([sdl2_Init.c](../../platform/sdl2/sdl2_Init.c),
`Startup_Platform`). But the very next line,

```c
sdl2_renderer = SDL_CreateRenderer(sdl2_window, -1, SDL_RENDERER_PRESENTVSYNC);
```

lets SDL pick the *first* render driver, and on X11 that is **`opengl`** — which creates
a GLX context. That single line is where a whole family of Linux display bugs land.

---

## The symptom

A hard crash on startup, before any window appears, e.g. the report that opened this:

```
X Error of failed request:  BadValue (integer parameter out of range for operation)
  Major opcode of failed request:  149 (GLX)
  Minor opcode of failed request:  3 (X_GLXCreateContext)
  Value in failed request:  0x0
```

Reported on **Kubuntu 26.04, Wayland**. The app is running through **XWayland**, and
GLX-over-XWayland with that Mesa stack hands back a null FBConfig (`Value 0x0`).

> **Resolved (2026-07-28) — driver-side, not a ReMoM bug.** The reporter took a system
> update that included a new video driver and, after the required reboot, ReMoM launched
> normally with no workaround. That confirms the diagnosis: a broken/mismatched GLX driver
> on the host, not a defect in ReMoM. Everything below is kept anyway — it is what turned
> this from a blind "crashes on startup" into a one-shot diagnosis, and it is exactly what
> the *next* such report will need. **First thing to ask a reporter: are your graphics
> drivers and kernel fully updated, and have you rebooted since?** A stale driver after a
> distro upgrade is the likeliest cause.

Two properties make this nasty to handle in code:

- **It is a *fatal* Xlib error.** Xlib's default handler prints the message and calls
  `exit()`, so the process dies *inside* `SDL_CreateRenderer`, before it returns. The
  `assert(sdl2_renderer != NULL)` never runs, and **no `if (renderer == NULL)` fallback
  can catch it** — the process is already gone.
- **The failing layer is not the one the user is on.** They are on Wayland, but the crash
  is in X11/GLX, because SDL defaulted to the XWayland video driver and then the GL
  renderer.

---

## Diagnosis is built in

`Startup_Platform` logs a graphics block before it creates the window/renderer, and it
**flushes the log** right before the risky `SDL_CreateRenderer` call — so even a fatal
Xlib exit leaves the breadcrumb behind. In the log (`LOG_CAT_SDL2_INIT`, category
`SDL2_INIT`) you get:

- SDL compiled vs linked version,
- the env vars that steer driver choice (`SDL_VIDEODRIVER`, `SDL_RENDER_DRIVER`,
  `XDG_SESSION_TYPE`, `WAYLAND_DISPLAY`, `DISPLAY`, `LIBGL_ALWAYS_SOFTWARE`, …),
- every **video** driver SDL built in, and which one is current,
- every **render** driver available, in priority order (index −1 picks the first),
- one line naming the renderer actually created — or, if the log ends at
  *"if this is the LAST line…"*, the create crashed hard.

SDL's own log stream is bridged into the same file. `REMOM_SDL_LOG=1` raises SDL to
verbose; otherwise SDL's error-level messages still flow through at no extra noise.

ReMoM also **falls back to the software renderer** if `SDL_CreateRenderer` returns NULL.
That rescues drivers that fail *politely*; it cannot rescue the *fatal-exit* case above
(nothing can, in-process) — for that, avoid GL up front (below).

---

## The fix for a user, right now

Set the **software renderer**. It skips GL entirely, so it never reaches the GLX path,
and it is more than fast enough for a 320×200 game:

```sh
SDL_RENDER_DRIVER=software ./ReMoM
```

This is the recommended workaround. Note what is **not** recommended:

- **`SDL_VIDEODRIVER=wayland` is *not* a reliable fix.** It swaps GLX for EGL and helps on
  *some* boxes, but SDL2's Wayland backend needs `libdecor` for client-side window
  decorations, and where that fails to load the Wayland path **segfaults** in window
  creation (observed on a KDE/Wayland dev box: `libdecor-gtk` fails → SIGSEGV, for every
  render driver including software). Forcing Wayland trades one crash for another,
  depending on the machine. Prefer the software-renderer knob, which is orthogonal to the
  video driver.

To make it permanent, add `export SDL_RENDER_DRIVER=software` to `~/.bashrc` /
`~/.zshrc`, or set it in a `.desktop` launcher's `Exec=`.

---

## The probe tool

`tools/remom_video_probe.c` (target **`remom_video_probe`**, Linux + SDL2 only) tests
**every video × render driver combination**, each in its **own forked child**, so a fatal
Xlib exit or segfault only takes that child down — the probe survives and prints a full
table. This is the thing to hand a reporter:

```
remom_video_probe            # enumerate, then probe the whole matrix
remom_video_probe --list     # just list the drivers, create nothing
remom_video_probe --once VD RD   # one attempt in-process (VD/RD "" = default),
                                 #   so SDL's real error text reaches the terminal
remom_video_probe --timing       # display refresh rates + present-cadence/jitter
                                 #   (the "choppy framerate" side -- see below)
```

Sample from a KDE/Wayland box (note it is the *inverse* of the Kubuntu report — here
XWayland works and native Wayland is the broken one):

```
  video x11:
    render opengl     : OK
    render software   : OK
  video wayland:
    render opengl     : CRASH (killed by signal)     <- libdecor segfault
    render software   : CRASH (killed by signal)
```

The verdicts:

- **OK** — window + renderer created and a frame presented.
- **CRASH** — the combination faults fatally (fatal Xlib exit, or a signal). Do not use it.
- **fail:\*** — SDL declined cleanly (`SDL_Init` / window / renderer returned an error);
  the combination is simply unavailable, not dangerous.

Build it with the normal Linux preset (`cmake --build --preset clang-debug --target
remom_video_probe`). It is a dev/QA diagnostic and ships in no release.

---

## Choppy / stuttering framerate

A different class of report: the game runs, but the picture judders — often "only on my
gaming monitor" while it's fine on ordinary panels. Two facts about ReMoM's timing frame
what's going on:

- **Game logic is wall-clock paced, not frame paced.** `Release_Time(ticks)` in
  [MoX/src/Timer.c](../../MoX/src/Timer.c) waits until `mark_time + ticks × 55 ms` using
  `SDL_GetTicks` — the 18.2 Hz DOS BIOS-tick rate (`PLATFORM_MILLISECONDS_PER_FRAME = 55`).
  So a 144 Hz monitor does **not** make the game run fast; speed is time-locked. A
  "choppy" report is therefore about how frames *land*, not game speed.
- **Presents are vsync-blocked and sparse.** The renderer is created with
  `SDL_RENDERER_PRESENTVSYNC`, and `Platform_Video_Update` → `SDL_RenderPresent`
  ([sdl2_Video.c](../../platform/sdl2/sdl2_Video.c)) blocks until the monitor's next
  refresh. Frames are produced irregularly, at roughly the 18 fps tick rate for
  animations.

An irregular ~18 fps stream into a vsync'd present is exactly the input that gaming-monitor
features handle worst:

- **VRR (G-Sync / FreeSync) below its floor.** Variable-refresh panels have a minimum
  refresh (commonly ~48 Hz). An 18 fps app is far under it, so the monitor engages
  **Low Framerate Compensation** — duplicating frames to stay in range. If that
  duplication doesn't line up with when ReMoM presents, you get visible judder that a
  fixed-60 Hz panel never shows. **This is the leading suspect for "choppy only on my
  gaming monitor."**
- **Non-integer refresh ÷ tick.** 55 ms isn't a whole number of refreshes (≈3.3 @ 60 Hz,
  ≈7.9 @ 144 Hz), so vsync'd frames land on inconsistent refresh boundaries — mild beat
  judder, usually worse at 60 Hz than high refresh.
- **"Gaming" toggles.** Backlight strobing (ULMB) forces a fixed refresh and disables VRR;
  driver low-latency modes (NVIDIA Ultra Low Latency, AMD Anti-Lag) cap the flip queue and
  change present timing. Any of these can shift the picture.

Vsync being *on* is not itself the bug — it just adds up to one refresh of latency per
present. The bug is jitter in *when* those presents complete.

### Measuring it: `remom_video_probe --timing`

`--timing` reports each display's refresh rate and then measures the **present-interval
jitter** under vsync at several frame rates, each run in its own child process:

```
remom_video_probe --timing
```

```
displays (2), current video driver = x11:
  [0] DP-2 34"
      desktop mode : 3440x1440 @ 60 Hz  (SDL_PIXELFORMAT_RGB888)
...
  vsync OFF, uncapped    : mean   0.19 ms  ... ->  5383.9 fps  [vsync off]
  vsync ON,  uncapped    : mean  16.77 ms  min  3.76  max 55.62  jitter(sd) 3.26 ms  -> 59.6 fps  [vsync granted]
  vsync ON,  ~18 fps     : mean  55.42 ms  min 54.81  max 55.69  jitter(sd) 0.15 ms  -> 18.0 fps  [vsync granted]
  vsync ON,  ~30 fps     : mean  33.25 ms  ...                   jitter(sd) 0.39 ms  -> 30.1 fps  [vsync granted]
  vsync ON,  ~60 fps     : mean  16.68 ms  ...                   jitter(sd) 0.57 ms  -> 60.0 fps  [vsync granted]
```

Reading it:

- **The refresh rate** is the headline. SDL2 cannot query VRR/G-Sync state, so a
  variable-refresh panel reports its *max* Hz — infer VRR from the jitter, not from a flag.
- **`jitter(sd)` is the choppiness number.** Low = smooth. High jitter, or a `max` far
  above the `mean`, on the `~18 fps` / `~30 fps` rows — the rates ReMoM actually produces —
  is the VRR/LFC signature. The example above is a plain 60 Hz box: the ~18 fps row is rock
  steady (0.15 ms), which is why the game looks fine there. On the reporter's gaming
  monitor those rows are where the numbers should blow up.
- **`vsync ON, uncapped` mean ≈ 1000 ÷ refresh** (16.7 ms @ 60, 6.9 ms @ 144) confirms
  vsync is honored. A near-zero mean means a driver/compositor is overriding it.

The startup log also records every display's mode now
(`graphics: display [0] … = 3440x1440 @ 60 Hz`, category `SDL2_INIT`), so a plain game log
already shows the refresh rate without running the probe.

### What to tell a reporter

1. Run `remom_video_probe --timing` and paste the output — the refresh rate and the
   ~18/~30 fps jitter rows localize it immediately.
2. As a test, **turn off VRR / G-Sync / FreeSync** for ReMoM (or globally) and any
   "low-latency"/strobing monitor mode, and re-check. If the judder clears, it was the
   monitor's variable-refresh path reacting to ReMoM's sub-floor frame rate.
3. If it persists with VRR off, capture the `--timing` numbers on the bad monitor vs a
   normal one — the row where they diverge names the misbehaving layer.

> **Not yet a code fix.** ReMoM presents when the game redraws; it does not currently pace
> presents to a steady cadence or expose a vsync toggle. If sub-floor-VRR judder turns out
> to be common, the fixes to weigh are: a runtime **vsync off** option (via
> `SDL_RenderSetVSync`, so presents don't wait on refresh), or presenting at a steady
> rate. Deferred until the `--timing` data from a real bad monitor says which. For now the
> reliable user-side answer is disabling the monitor's VRR/gaming modes for ReMoM.

---

## Why not just default to software on Linux?

Because GL works for the large majority, and the software renderer gives up
GPU-accelerated window scaling for everyone to fix a minority of broken stacks. The
current posture is: **keep the GL default, log enough to diagnose in one shot, fall back
on soft failures, and document the `SDL_RENDER_DRIVER=software` escape hatch** for the
fatal-exit case.

### Possible improvement — self-healing startup probe

The fatal-exit case is the only one ReMoM cannot currently recover from automatically,
because the process dies before any fallback runs. A robust fix is to **fork a short-lived
child at startup that attempts the GL renderer** (exactly what `remom_video_probe` does
for one combo); if the child dies, the parent sets `SDL_RENDER_DRIVER=software` before
creating the *real* renderer. That would make ReMoM self-heal on broken-GLX boxes with no
user action and no env var — at the cost of one ~millisecond fork on Linux startup.

**Priority: low, deferred.** The one field report so far resolved itself with a host
driver update (see *The symptom*), so this is not chasing a live bug. Build it only if
these reports *recur on up-to-date drivers* — i.e. if the fault turns out to be something
a host update does not fix. Until then the startup diagnostics, `remom_video_probe`, and
the documented `SDL_RENDER_DRIVER=software` knob are the right amount of machinery.
