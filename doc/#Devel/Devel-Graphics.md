# Graphics — startup crashes and choppy framerate, and how to diagnose them

Two families of display report live here: a **hard crash in `SDL_CreateRenderer`** at
startup, and a **choppy / stuttering framerate** (usually a gaming monitor's
variable-refresh path). Both are diagnosed with the same tool, `tools/remom_video_probe`,
and the same startup log block.

Both symptoms occur on Linux and on Windows, from the *same* line of ReMoM code, but the
failing layer differs — GLX-over-XWayland on Linux, a faulting D3D/OpenGL driver DLL on
Windows. Most of this document works through the Linux case in detail because that is
where the reports came from; [Windows](#windows) covers what differs there. The probe
handles both.

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

`tools/remom_video_probe.c` (target **`remom_video_probe`**, SDL2, Linux + Windows) tests
**every video × render driver combination**, each in its **own child process**, so a fatal
Xlib exit, a segfault, or a Windows access violation only takes that child down — the probe
survives and prints a full table. This is the thing to hand a reporter:

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
- **CRASH** — the combination faults fatally (fatal Xlib exit, a signal, or on Windows an
  unhandled exception, whose code is printed). Do not use it.
- **fail:\*** — SDL declined cleanly (`SDL_Init` / window / renderer returned an error);
  the combination is simply unavailable, not dangerous.
- **`[actual: X]`** — the driver SDL *really* used. `SDL_HINT_RENDER_DRIVER` is a
  preference, not a demand: with index `-1` SDL tries the hinted driver and then falls
  through the rest of its list. When `actual` differs from the requested column, that
  driver was **unavailable** and the row says nothing about it. (This is why every
  `video dummy` row reads `[actual: software]`.)

Build it with the normal preset for your platform — `cmake --build --preset clang-debug
--target remom_video_probe` on Linux, `cmake --build --preset MSVC-debug` on Windows.

**End-users don't build it.** How it reaches them differs per platform, because the two
packages are shaped differently:

- **Linux** — published in the optional **`ReMoM-<ver>-linux-diagnostics.zip`** asset,
  alongside a verbose `ReMoM_diagnostic` build and a README. The Linux diagnostic build is
  *not* in the main package, so it needs its own download.
- **Windows** — shipped **inside the normal ZIP / NSIS package**, next to `ReMoM.exe`,
  `ReMoM_diagnostic.exe` and `SDL2.dll`. The Windows package already carries the diagnostic
  exe and the SDL2 runtime, so folding in a ~100 KB probe costs nothing and spares testers
  a second download. There is no `windows-diagnostics.zip`.

See [RELEASES.md → Diagnostic tools (Linux, testers)](../../RELEASES.md#diagnostic-tools-linux-testers)
and `packaging/diagnostics-README.md` (the guide shipped inside the Linux zip).

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

## Windows

Same ReMoM line, same two symptoms, different failing layer. What changes:

**The matrix has a different shape.** SDL2 on Windows enumerates only `windows` and
`dummy` for video, so the video axis is degenerate and the **render** axis *is* the table.
The candidates are `direct3d11, direct3d12, direct3d, opengl, opengles2, software` — index
`-1` picks `direct3d11` on a normal box. This is why `[actual: X]` matters more here than
on Linux: with only one real video driver, an unlabelled `OK` on a fallback row would be
actively misleading.

**Isolation is spawn, not fork.** Windows has no `fork()`, so the probe re-runs *itself*
with an internal `--child VD RD` and reads the child's exit code. A faulting child exits
with its **exception code**, which the probe prints (`CRASH (exit/exception 0xC0000005)`).
The code is diagnostic in itself: `0xC0000005` is an access violation (a driver bug),
`0xC0000135` is a missing DLL (a broken/partial driver install), `0xC0000409` is a
stack-cookie fast-fail.

**The child must suppress every modal dialog**, which the Linux path never has to think
about. A faulting child would otherwise pop Windows Error Reporting and **block the probe
forever** waiting for a click nobody is watching for — a hang, not a failure. See
`probe_Harden_Child()`: `SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | …)`,
`_set_abort_behavior`, and the debug-CRT `_CrtSetReportMode` calls.

**Windows-only facts the probe reports**, because they decide the outcome and none of them
is an environment variable:

- **Graphics adapters via DXGI** — name, vendor/device ID, VRAM, and a `[SOFTWARE/WARP]`
  flag. SDL2 exposes none of this, and it is the single most useful line in a Windows
  "won't start" report: it names the GPU whose driver is faulting. A machine whose *only*
  adapter is `Microsoft Basic Render Driver` has no usable 3D at all.
- **Remote Desktop session** (`GetSystemMetrics(SM_REMOTESESSION)`) — an RDP session has no
  real GPU, so D3D paths fail or drop to software and the frame rate is at the mercy of the
  network. Both read as "ReMoM is broken" in a report that never mentions RDP.
- **The real OS build**, via `RtlGetVersion`. `GetVersionEx` lies to unmanifested processes
  (it caps at 6.2 / Windows 8).

**`--timing` raises the timer resolution.** Windows' default sleep granularity is ~15.6 ms,
so the paced rows would otherwise report jitter that is an artifact of `SDL_Delay`, not of
the display — precisely the false positive `--timing` exists to rule out. The paced cases
wrap the measurement in `timeBeginPeriod(1)` / `timeEndPeriod(1)` and the output says so.

A healthy box looks like this (note every requested driver was granted, and the `dummy`
rows fall back as expected):

```
  video windows:
    render direct3d11 : [actual: direct3d11] OK
    render direct3d12 : [actual: direct3d12] OK
    render direct3d   : [actual: direct3d] OK
    render opengl     : [actual: opengl] OK
    render opengles2  : [actual: opengles2] OK
    render software   : [actual: software] OK

  video dummy:
    render direct3d11 : [actual: software] OK
    ...
```

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
