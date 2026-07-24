# PLAN — Platform Input Responsiveness & Smoothness Measurement (SDL2 first)

**Status:** Draft
**Owner:** TBD
**Date:** 2026-07-23
**Companion:** `PRD-Platform-Input.md` (requirements), `BRA-Platform-Input.md` (decision record), this file (survey + build-out order)

---

## Goal

Measure input responsiveness (latency) and smoothness (jitter) for the SDL2 backend deterministically, localize where the reported lag comes from, and guard against regression — reusing the existing `DBG_Frame_Reset` counters, `.RMR` record/replay, `PFL_Capture`, and CTest infrastructure rather than building a parallel system. Fixes are out of scope (a gated follow-on); this delivers the ruler.

## What already exists

### Input path (SDL2)

- **`Platform_Event_Handler()`** ([`sdl2_PFL.c:452`](../../platform/sdl2/sdl2_PFL.c#L452)) is the once-per-logical-frame input tick: `SDL_PumpEvents` → per-event switch → a single `SDL_GetMouseState` poll ([`:605`](../../platform/sdl2/sdl2_PFL.c#L605)) → `platform_frame_callback` → replay capture. The `SDL_MOUSEMOTION` case is empty ([`:572`](../../platform/sdl2/sdl2_PFL.c#L572)) — **position is polled, not evented**.
- **`Platform_Pump_Events()`** ([`sdl2_PFL.c:446`](../../platform/sdl2/sdl2_PFL.c#L446)) is the lighter spin-wait tick: `SDL_PumpEvents` + `Platform_Maybe_Move_Mouse()`. Called from `Release_Time`'s wait loop ([`Timer.c:87`](../../MoX/src/Timer.c#L87)) every ~1 ms via `Platform_Sleep_Millies(1)` (= `SDL_Delay(1)`).
- **The cursor is software-drawn.** `Platform_Maybe_Move_Mouse()` ([`sdl2_PFL.c:128`](../../platform/sdl2/sdl2_PFL.c#L128)) polls the OS position, and on a move does `Restore/Save/Draw_Mouse_On_Page` + `Platform_Video_Update()` ([`:184`](../../platform/sdl2/sdl2_PFL.c#L184)). Cursor latency == render+present latency.
- **VSync-blocked present.** `SDL_RENDERER_PRESENTVSYNC` ([`sdl2_Init.c:99`](../../platform/sdl2/sdl2_Init.c#L99)); the single `SDL_RenderPresent` is at [`sdl2_Video.c:132`](../../platform/sdl2/sdl2_Video.c#L132). Multiple call paths reach it per frame (main render, `Maybe_Move_Mouse`, `Fonts.c`, `Video.c`).

### Instrumentation already present

- **`DBG_Frame_Reset()`** ([`sdl2_PFL.c:416`](../../platform/sdl2/sdl2_PFL.c#L416)), called from `Platform_Video_Update` ([`sdl2_Video.c:134`](../../platform/sdl2/sdl2_Video.c#L134)), already tracks per-frame: `frametime`, `handler_calls`, `max_delta` (**worst-case cursor freeze**), `avg_delta`, per-type event counts, `mouse_updates`. It logs a summary every 60 frames behind `STU_DEBUG` — the data is computed but only summarized, never streamed.
- **`MOUSE_LOG`** behind `MOUSE_DEBUG` streams per-event lines with `SDL_GetTicks` stamps ([`sdl2_PFL.c:167,535,618`](../../platform/sdl2/sdl2_PFL.c#L167)) — good for ordering, unreliable for absolute timing (per-line `fprintf` perturbs the path).

### Reproducible-input + capture substrate

- **`.RMR` record/replay** ([`Replay.c`](../../platform/replay/Replay.c), [`Platform_Replay.h`](../../platform/include/Platform_Replay.h)): a **text CSV**, header `# idx,timestamp_ms,delta_ms,mouse_x,mouse_y,mouse_buttons,key_pressed,key_count,keys...` ([`Replay.c:172`](../../platform/replay/Replay.c#L172)). `--record FILE` / `--replay FILE` wired in [`ReMoM.c:441-459`](../../src/ReMoM.c#L441-L459). `Replay_Inject_Frame()` ([`Replay.c:572`](../../platform/replay/Replay.c#L572)) injects game-coordinate positions into engine globals — **bypassing the OS poll**, so a replay covers *engine→present* only.
- **`PFL_Capture`** ([`PFL_Capture.c`](../../platform/capture/PFL_Capture.c), [`Platform_Capture.h`](../../platform/include/Platform_Capture.h)): `--capture DIR [--capture-fps N]` ([`ReMoM.c:415-440`](../../src/ReMoM.c#L415-L440)) writes `video_rgb24.raw` + `audio.wav` + `capture_info.txt` at fixed fps, backfilling gaps by duplicating the last presented frame. Fed from `Platform_Video_Update` ([`sdl2_Video.c:82-85`](../../platform/sdl2/sdl2_Video.c#L82-L85)). A freeze appears as duplicated frames — a usable smoothness signal.
- **CTest fixture pattern** (`tests/CMakeLists.txt`): Setup → Run → Assertions chains, run via `ctest -R "HeMoM_"`. The precedent for wiring a new deterministic test.

## Decisions

1. **No new subsystem — connect and quantify what exists.** Layer 1 extends `DBG_Frame_Reset`; Layer 2 composes `--replay` + `--capture` + a new offline script; Layer 3 is a documented procedure. (BRA §5 Option D.)
2. **Measurement is additive and flag-gated.** A new compile flag (e.g. `INPUT_METRICS`) guards all Layer 1 hooks; the metrics file is an in-memory ring buffer flushed at exit as **fixed-width values** (`.fwv`, not CSV — `tools/field_catalog/fwv.py`) (observer-effect rule, BRA §6.4). No reconstructed pacing code is edited in this effort.
3. **Windowed, SDL2, reference-hardware baseline.** Numbers are comparable only within a like environment; the `.fwv` file header self-reports backend, scale, refresh, and X11/Wayland.
4. **Latency and jitter are separate percentile distributions.** Never a mean.
5. **Fixes are gated follow-on.** This PLAN stops at "we can measure and we have a baseline + a failing-on-regression test."

## Three layers

### Layer 1 — Internal FWV timeline (the tracer bullet) — **IMPLEMENTED**

Delivered as the backend-agnostic module [`platform/metrics/PFL_Input_Metrics.c`](../../platform/metrics/PFL_Input_Metrics.c) + [`Platform_Input_Metrics.h`](../../platform/include/Platform_Input_Metrics.h). One fixed-width row per present capturing: present timestamp, inter-present interval (`dt_ms`), `SDL_RenderPresent` block time (`block_ms`), input-handler ticks since last present, worst tick-to-tick gap (`max_gap`), and cursor step (`dx,dy`). Hooks: tick notes in `Platform_Event_Handler`/`Platform_Pump_Events`, cursor-poll notes in `Platform_Maybe_Move_Mouse`/`Platform_Update_Mouse_Position`, a `cursor` reason tag, and a present-timing bracket around `SDL_RenderPresent` in [`sdl2_Video.c`](../../platform/sdl2/sdl2_Video.c).

Decisions taken during implementation:
- **Runtime-gated, shipped in release, OFF by default** (not a compile flag) — `REMOM_INPUT_METRICS=1` (or a path) writes the `.fwv`; unset/`0` costs one branch. This lets the actual reporting hardware produce a capture.
- **Crash-safe persistence** — incremental buffered append with a periodic `fflush` every ~0.5 s (not ring-buffer-flushed-only-at-exit), so a SIGSEGV / force-quit still yields a `.fwv`. Per-row write is a userspace buffer copy (no syscall), so it does not perturb the present timing it measures. No custom signal handler (STU_BRAK's crash reporter is left intact).
- **FWV, not CSV** (project convention; the read/analysis side can reuse `tools/field_catalog/fwv.py`).

### Layer 2 — Synthetic replay + capture harness (the regression guard)

1. Author a **known-trajectory `.RMR`** — a constant-velocity diagonal sweep across the main-menu screen, fixed seed. Either hand-write the CSV or record one and freeze it.
2. Run `ReMoM --replay sweep.RMR --capture out/ --capture-fps 120` to a fixed frame count, then exit.
3. New **offline analysis script** (`tools/input_metrics/analyze_capture.py`): read `capture_info.txt` + `video_rgb24.raw`, locate the cursor centroid per frame (template-match the cursor sprite, or diff against a cursor-free reference frame), join against `sweep.RMR`'s `timestamp_ms,mouse_x,mouse_y`, and emit engine→present latency percentiles, dropped/duplicated-frame counts, and cursor-step jitter.
4. Threshold-check against a committed baseline; non-zero exit on regression.

### Layer 3 — External motion-to-photon (ground-truth calibration)

Document a procedure in `doc/PlatformLayer/`: 240 fps phone capture of the screen, move a real mouse, count frames from physical motion to on-screen cursor motion (×1000/240 ms per frame). Include the **differential trick** — temporarily skip `SDL_ShowCursor(SDL_DISABLE)` in `pfl_mouse_grab` ([`sdl2_PFL.c:256-264`](../../platform/sdl2/sdl2_PFL.c#L256-L264)) so the OS cursor and game cursor are filmed together and their gap reads out engine-added latency directly. Log readings per milestone.

## Layer 1 result — baseline (2026-07-24)

**Build-out step 1 is DONE, and the first real capture named the dominant cause on the first run — and it is neither hypothesis this plan was built around.**

Environment: SDL2 / X11 / 4× scale / 60 Hz. 1455 presents over 83.3 s, clean exit. Raw evidence committed at [`baseline_2026-07-24_x11_scale4.fwv`](../PlatformLayer/input-metrics/baseline_2026-07-24_x11_scale4.fwv).

| metric | p50 | p90 | p95 | p99 | max | reading |
|---|---|---|---|---|---|---|
| `dt_ms` (present interval) | **55** | 108 | 111 | 187 | 2210 | cursor redraws only ~18×/s |
| `block_ms` (vsync stall) | 0 | 16 | 16 | 18 | 51 | **not vsync** — 0 for 84% of presents |
| `max_gap` (input freeze) | 2 | 2 | 2 | 2 | 24 | **not a freeze** — input pumped every ~2 ms |
| `step` (cursor px/redraw) | 0 | 9 | 22 | 80 | 150 | moving cursor lurches, not glides |

**Root cause:** the cursor is software-drawn into the framebuffer and only reaches the screen when the main loop presents — which is paced to the ~55 ms DOS BIOS-tick (`PLATFORM_MILLISECONDS_PER_FRAME`), i.e. ~18 fps. Input is polled ~829/s (avg 47 ticks/present), so the cursor *position* is sampled fine but only *rendered* ~18×/s; a moving cursor jumps 40–55 px per redraw.

**Hypotheses rejected by the data:**
- *Stacked vsync presents* — **rejected**: `block_ms` = 0 for 84% of presents; only 16% blocked ~16 ms.
- *Cursor freeze / input starvation* — **rejected**: `max_gap` = 2 ms; input pumped ~829/s.

**Corroborating:** `dt_ms` is quantized to the 55 ms BIOS tick (879/1455 presents at 55–82 ms = one tick, 98 at ~110 ms = two ticks), and **0 of 1455 presents were cursor-driven** (`reason=cursor`) — the dedicated `Platform_Maybe_Move_Mouse` present path contributes nothing, so every cursor update piggybacks on the ~18 fps main-loop present.

**Why this regressed from DOS:** OG-MoM's cursor was a hardware sprite the mouse driver moved independently of the ~18 Hz game loop, so it stayed smooth. ReMoM draws the cursor into the framebuffer, which is only presented at the game's frame rate — the cursor lost its independent fast path.

**Fix promoted to the funded follow-on:** restore the cursor's independent fast path with a real **SDL hardware/OS cursor** (`SDL_CreateColorCursor` from the game's cursor bitmap, swapped on shape change), so the compositor tracks it at native rate regardless of the framebuffer present cadence — the modern equivalent of the original hardware sprite. Re-run this capture after the change; success = cursor motion decoupled from `dt_ms`.

## Fix — hardware cursor (delivered 2026-07-24)

**Approach.** Build a real OS cursor from the game's own cursor sprite and let the compositor render it, so cursor motion is fully decoupled from the ~18 fps framebuffer present. This is the modern equivalent of the DOS hardware sprite that kept the original smooth over an 18 Hz game loop. Runtime-gated by `REMOM_HW_CURSOR=1`, **off by default** — when off, every hook is a single branch and behaviour is unchanged.

**Mechanism (backend-agnostic where it can be).**
- `Platform_HW_Cursor_Active()` / `Platform_HW_Cursor_Refresh()` — new entry points on the platform interface ([`Platform.h`](../../platform/include/Platform.h)). `Refresh` reads the current sprite from `mouse_palette` (image `N-1`, `CURSOR_WIDTH*CURSOR_HEIGHT`, **column-major**, index `0` = transparent), converts through `platform_palette_buffer`, nearest-neighbour-upscales by the window `scale`, and builds an `SDL_Cursor` with hotspot `(0,0)` — correct because the engine hit-tests off the sprite's top-left. It rebuilds only when `Get_Pointer_Image_Number()` changes.
- **Software cursor suppressed at the choke point:** guarded early-returns in `Draw_Mouse_On_Page_` / `Draw_Mouse_Off_Page_` ([`MoX/src/Mouse.c`](../../MoX/src/Mouse.c)) — the single point all ~7 callers funnel through, so no second (laggy) cursor is drawn regardless of caller.
- **Backend hooks:** show the OS cursor at init instead of hiding it; refresh shape on the mouse-poll paths; skip the software redraw/present in `Platform_Maybe_Move_Mouse`; and neuter `pfl_mouse_grab`/`pfl_mouse_ungrab` (their cursor-hide + relative-mouse-mode on click would break native tracking).

**Backends.**
- **SDL2** ([`sdl2_MD.c`](../../platform/sdl2/sdl2_MD.c), [`sdl2_Init.c`](../../platform/sdl2/sdl2_Init.c), [`sdl2_PFL.c`](../../platform/sdl2/sdl2_PFL.c)) — **implemented, built, and visually confirmed.**
- **SDL3** ([`sdl3_MD.c`](../../platform/sdl3/sdl3_MD.c), `sdl3_Init.c`, `sdl3_PFL.c`) — **ported** (SDL3 APIs: `SDL_CreateSurface`, `SDL_DestroySurface`/`SDL_DestroyCursor`, no-arg `SDL_ShowCursor()`/`SDL_HideCursor()`). **Compile-unverified** — SDL3 is not installed in the dev environment; needs a build on an SDL3 host before it can be trusted.
- **Headless** ([`headless_PFL.c`](../../platform/headless/headless_PFL.c)) — stub (`Active()` returns 0); satisfies the interface `Mouse.c` now calls so HeMoM and the unit tests link.
- **Win32** ([`win_MD.c`](../../platform/win32/win_MD.c), [`win_PFL.cpp`](../../platform/win32/win_PFL.cpp)) — **implemented, builds clean, runs.** `Platform_HW_Cursor_Active`/`Refresh` build an `HCURSOR` from the sprite via `CreateIconIndirect`; init shows the OS cursor, `WM_SETCURSOR` re-asserts it, `Platform_Maybe_Move_Mouse` skips the software path. Layer 1 metrics hooks also added, so a Windows `.fwv` baseline can be captured. Compile/run-verified on this box (`MSVC-win32-debug`); **cursor smoothness A/B is a pending visual check** (same as the SDL2 confirmation was). See *Win32 port* below.

**Verification.** Visual A/B on the reporting hardware (SDL2/X11): with `REMOM_HW_CURSOR=1` the user reported it "felt a whole lot better" (2026-07-24). **Layer 1 metrics are unchanged by design** — the HW cursor bypasses the framebuffer present, so `dt_ms` still shows ~18 fps; smoothness here is verified visually (and, later, by Layer 3 camera), *not* by the `.fwv`. A metrics run with the flag on is still useful to confirm **no present-side regression**.

**Follow-ups.**
- **Win32 backend** — **not started, and currently a build break.** Same shape of fix, different cursor API. Detailed in *Fix — hardware cursor: Win32 port* below.
- Per-cursor **hotspot** refinement (via `current_pointer_offset`/`center_offset`) if any cursor's active point isn't top-left.
- Rebuild the cursor on **palette-cycle** changes (currently rebuilt only on shape change), for cursors that use cycled palette entries.
- After a soak, consider making `REMOM_HW_CURSOR` the **default** (or an in-game setting) rather than opt-in.

## Fix — hardware cursor: Win32 port (delivered 2026-07-24)

**Status: implemented, builds clean under `/W4 /WX`, runs.** The port both restored the build the SDL2/SDL3 fix commit (`53eb71b`) had broken *and* landed the full hardware cursor + Layer 1 metrics for the native Win32 backend.

### The breakage it resolved (verified before/after)

The shared suppression in `Mouse.c` (`Draw_Mouse_On_Page_`/`Draw_Mouse_Off_Page_`, [`MoX/src/Mouse.c:750,788`](../../MoX/src/Mouse.c#L750)) calls `Platform_HW_Cursor_Active()`. `Mouse.c` links into every backend's player exe, but before this port no `win32/` file defined that symbol (headless got a stub, Win32 did not). A clean `USE_WIN32` build failed at link:

```
MOX.lib(Mouse.obj) : error LNK2019: unresolved external symbol Platform_HW_Cursor_Active
    referenced in function Draw_Mouse_On_Page_ [ ... \src\ReMoM.vcxproj ]
ReMoM.exe : fatal error LNK1120: 1 unresolved externals
```

(`demo_vga.exe` failed identically; `ReMoM.exe` was not produced.) After the port, `cmake --build --preset MSVC-win32-debug` links `ReMoM.exe` with zero errors/warnings. The symbol is now defined in `win_MD.c`, so the same shared `Mouse.c` suppression that SDL2/SDL3 use applies to Win32.

### Current Win32 cursor / present model (what the port changes)

- OS cursor is **hidden**: `ShowCursor(FALSE)` on `WM_CREATE` ([`win_PFL.cpp`](../../platform/win32/win_PFL.cpp)), and the window class sets `wcex.hCursor = NULL`.
- Cursor is **software-drawn** into the framebuffer via the shared `Mouse.c` path, same as SDL2.
- Present is a **`StretchDIBits`** blit (`win_PFL.cpp:228`) inside `Platform_Video_Update` (`:231`).
- `Platform_Maybe_Move_Mouse()` (`win_PFL.cpp:439`) is the cursor fast path: `Restore_/Save_/Draw_Mouse_On_Page`.
- Input ticks: `Platform_Event_Handler()` (`win_PFL.cpp:270`) and the `Platform_Pump_Events()` `PeekMessage` loop (`:335,345`).

### What the port did (mirrors `sdl2_MD.c`) — all delivered

1. **Implemented `Platform_HW_Cursor_Active`/`Refresh` in `win_MD.c`** (was empty; already in `PLATFORM_SOURCES` for `USE_WIN32`). Same `REMOM_HW_CURSOR` env gate. `Refresh` builds an `HCURSOR` from the current sprite (`mouse_palette`, image `N-1`, 16×16, **column-major**, index 0 transparent) through `platform_palette_buffer`, nearest-neighbour-upscaled by the window `scale`, rebuilt only when `Get_Pointer_Image_Number()` changes — the SDL2 logic verbatim, only the OS handoff differs:
   - Build a 32-bit top-down `BITMAPV5HEADER` DIB (BGRA, per-pixel alpha: opaque for palette pixels, 0 for the transparent index), plus a monochrome AND-mask bitmap (all-zero when the color DIB carries alpha).
   - `ii.fIcon = FALSE` (a cursor, not an icon), `ii.xHotspot = ii.yHotspot = 0` (top-left, matching the engine's hit-test), `hbmColor`/`hbmMask` set → `CreateIconIndirect` → `(HCURSOR)`.
   - `SetCursor(hcur)`; `DestroyIcon` the previous; `DeleteObject` both bitmaps.
2. **Show the OS cursor at init (gated):** when `Platform_HW_Cursor_Active()`, do not `ShowCursor(FALSE)` on `WM_CREATE`; leave the OS cursor visible.
3. **Handle `WM_SETCURSOR` (the Win32-specific wrinkle):** Windows resets the cursor to the class cursor (here `NULL`) on every mouse move over the client area, which would erase our `SetCursor` each frame. Add a `WM_SETCURSOR` case: when the hit-test is `HTCLIENT` and the HW cursor is active, `SetCursor(current_hcursor)` and return `TRUE` to stop default processing. (Alternatively set the class cursor, but per-move `SetCursor` keeps the shape swap trivial.)
4. **Skip the software fast path (gated):** early-return the `Restore_/Save_/Draw_Mouse_On_Page` block in `Platform_Maybe_Move_Mouse` (`win_PFL.cpp:439`) when the HW cursor is active, and call `Platform_HW_Cursor_Refresh()` on the poll paths so a shape change is picked up. (The shared `Mouse.c` draw suppression already prevents the sprite being drawn; this also avoids the needless framebuffer churn.)
5. **No grab/relative-mode neuter needed** the way SDL2 needed it — Win32 has no `pfl_mouse_grab` relative-mode analogue — but confirm nothing calls `ShowCursor(FALSE)` after init while the HW cursor is active (`Shutdown_Platform` restoring `ShowCursor(TRUE)` is fine).

Also: `Win_HW_Cursor_Apply()` (defined in `win_MD.c`, declared in `win_PFL.h`) exposes the current `HCURSOR` to the `WM_SETCURSOR` handler in `win_PFL.cpp` without leaking Win32 types into the shared `Platform.h`.

### Layer 1 metrics on Win32 — added

`PFL_Input_Metrics.c` is backend-agnostic (0 SDL refs) and was already compiled into the Win32 backend (`platform/CMakeLists.txt` appends it unconditionally), but its feeding hooks previously existed only in SDL2/SDL3 — on Win32 the module linked yet was never fed. The same hooks were added, mirroring `sdl2_Video.c`/`sdl2_PFL.c`/`sdl2_Init.c`:
- `Input_Metrics_Init(...,"Win32","gdi",scale,0)` / `Input_Metrics_Shutdown()` in `Startup_/Shutdown_Platform`.
- `Input_Metrics_Note_Tick(Platform_Get_Millies())` in `Platform_Event_Handler` and `Platform_Pump_Events`.
- `Input_Metrics_Note_Poll(x,y)` in `Platform_Maybe_Move_Mouse`.
- `Input_Metrics_Record_Present(t0,t1)` bracketing the `Win_Blit_Back_Buffer` present in `Platform_Video_Update`.

### Verification (2026-07-24, this Windows box)

- **Build:** `cmake --build --preset MSVC-win32-debug` links `ReMoM.exe` with **zero errors/warnings under `/W4 /WX`** (was `LNK2019`/not-produced before the port).
- **Run:** launched the Win32 `ReMoM.exe` with `REMOM_HW_CURSOR=1 REMOM_INPUT_METRICS=1` — starts into the title screen, no crash, no missing-data dialog.
- **Metrics fire on Win32:** the run produced a well-formed `.fwv` — `#meta backend=Win32 video_driver=gdi scale=4`, 27 present rows with sensible values (`dt_ms`≈172, ~90 input ticks/present, `max_gap`=16 ms). This confirms the W3 hooks actually execute on Win32, not just compile.
- **Still pending (same gap SDL2 had at first):** the cursor-smoothness **visual A/B** with the flag on/off, and a proper Windows Layer 1 baseline while actively moving the mouse. Win32 is locally testable (`MSVC-win32-*`), so — unlike the deferred SDL3 port — this can be done here rather than deferred.

## CTest & build wiring

- Layer 2 becomes a windowed CTest test under the existing fixture chain: `Input_Metrics_Setup` (ensure `sweep.RMR` + a cursor-free reference frame) → `Input_Metrics_Run` (`--replay … --capture …`) → `Input_Metrics_Assert` (`analyze_capture.py` + threshold). Gated to run only where a display is available.
- Layer 1's module is already listed in **both** [`platform/CMakeLists.txt`](../../platform/CMakeLists.txt) and [`Makefile.am`](../../Makefile.am) (windowed backend only; headless excluded). No compile flag — the gate is the `REMOM_INPUT_METRICS` env var at runtime, so no CMake/Autotools option is needed. Layer 2's `analyze_capture.py` is data-side.

## Build-out order

1. ~~**Tracer bullet — Layer 1 FWV.**~~ **DONE (2026-07-24).** Module built, both build systems wired, captured on the reporting hardware; the baseline named the cause (see *Layer 1 result* above). The dominant cause turned out to be the render/cursor cadence, so the fix effort is promoted ahead of Layers 2–3.
2. **Layer 2 skeleton.** Hand-author `sweep.RMR`; confirm `--replay sweep.RMR --capture out/` produces a clean fixed-fps video of the presented cursor.
3. **Layer 2 analysis + baseline.** Write `analyze_capture.py`; capture and commit a baseline on the named reference machine; wire the CTest test with relative thresholds.
4. **Layer 3 procedure + first reading.** Document and take one ground-truth motion-to-photon number; reconcile it against Layer 1's internal input→present number to size the OS/compositor contribution the internal layers can't see.
5. **Fix effort.** First fix **delivered** — the hardware cursor (see *Fix — hardware cursor* above), chosen directly from the baseline's root cause. SDL2 confirmed; SDL3 ported (unverified); **Win32 implemented, builds clean, runs — build-verified and metrics-verified locally** (see *Fix — hardware cursor: Win32 port*); its cursor-smoothness visual A/B is the one remaining check, and is doable on the dev box.

## Open risks

- **Layer 2 is half-chain by construction.** Replay bypasses the OS poll ([`Replay.c:572`](../../platform/replay/Replay.c#L572)); it cannot catch an OS→engine regression. Layer 3 is the only cover for that half — accept and document, don't paper over.
- **Capture time-resolution.** At 60 fps capture, one frame ≈ 16 ms — coarse for latency. `--capture-fps 120+` helps but grows `video_rgb24.raw`; confirm the backend actually presents fast enough to fill higher-fps buckets rather than backfilling duplicates (which would floor the measured latency at the capture interval).
- **Cursor centroid extraction.** Template-matching the software cursor against a palettized RGB frame is the fiddly part of the analysis script; a cursor-free reference frame + diff is the fallback if template-match is noisy.
- **Reference-hardware dependence.** Absolute numbers vary by GPU/compositor/refresh; thresholds are relative to a *named* baseline machine, and cross-machine comparisons are invalid.
- **Observer effect.** If Layer 1 ever measures *worse* pacing than an un-instrumented build, the hooks are too heavy — keep them to timestamp arithmetic into a preallocated buffer, no I/O on the hot path.
