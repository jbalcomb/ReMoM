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

**Verification.** Visual A/B on the reporting hardware (SDL2/X11): with `REMOM_HW_CURSOR=1` the user reported it "felt a whole lot better" (2026-07-24). **Layer 1 metrics are unchanged by design** — the HW cursor bypasses the framebuffer present, so `dt_ms` still shows ~18 fps; smoothness here is verified visually (and, later, by Layer 3 camera), *not* by the `.fwv`. A metrics run with the flag on is still useful to confirm **no present-side regression**.

**Follow-ups.**
- **Win32 backend** — same shape of fix, different cursor API (build an `HCURSOR` from the sprite via `CreateIconIndirect`; `SetCursor` + `ShowCursor(TRUE)`; handle `WM_SETCURSOR`/set the window-class cursor so Windows doesn't reset it each move). The shared `Mouse.c` suppression already accommodates it — Win32 only needs its own `Platform_HW_Cursor_Active/Refresh` plus the show/grab hooks. Not started.
- Per-cursor **hotspot** refinement (via `current_pointer_offset`/`center_offset`) if any cursor's active point isn't top-left.
- Rebuild the cursor on **palette-cycle** changes (currently rebuilt only on shape change), for cursors that use cycled palette entries.
- After a soak, consider making `REMOM_HW_CURSOR` the **default** (or an in-game setting) rather than opt-in.

## CTest & build wiring

- Layer 2 becomes a windowed CTest test under the existing fixture chain: `Input_Metrics_Setup` (ensure `sweep.RMR` + a cursor-free reference frame) → `Input_Metrics_Run` (`--replay … --capture …`) → `Input_Metrics_Assert` (`analyze_capture.py` + threshold). Gated to run only where a display is available.
- Layer 1's module is already listed in **both** [`platform/CMakeLists.txt`](../../platform/CMakeLists.txt) and [`Makefile.am`](../../Makefile.am) (windowed backend only; headless excluded). No compile flag — the gate is the `REMOM_INPUT_METRICS` env var at runtime, so no CMake/Autotools option is needed. Layer 2's `analyze_capture.py` is data-side.

## Build-out order

1. ~~**Tracer bullet — Layer 1 FWV.**~~ **DONE (2026-07-24).** Module built, both build systems wired, captured on the reporting hardware; the baseline named the cause (see *Layer 1 result* above). The dominant cause turned out to be the render/cursor cadence, so the fix effort is promoted ahead of Layers 2–3.
2. **Layer 2 skeleton.** Hand-author `sweep.RMR`; confirm `--replay sweep.RMR --capture out/` produces a clean fixed-fps video of the presented cursor.
3. **Layer 2 analysis + baseline.** Write `analyze_capture.py`; capture and commit a baseline on the named reference machine; wire the CTest test with relative thresholds.
4. **Layer 3 procedure + first reading.** Document and take one ground-truth motion-to-photon number; reconcile it against Layer 1's internal input→present number to size the OS/compositor contribution the internal layers can't see.
5. **Fix effort.** First fix **delivered** — the hardware cursor (see *Fix — hardware cursor* above), chosen directly from the baseline's root cause. SDL2 confirmed; SDL3 ported (unverified); Win32 outstanding.

## Open risks

- **Layer 2 is half-chain by construction.** Replay bypasses the OS poll ([`Replay.c:572`](../../platform/replay/Replay.c#L572)); it cannot catch an OS→engine regression. Layer 3 is the only cover for that half — accept and document, don't paper over.
- **Capture time-resolution.** At 60 fps capture, one frame ≈ 16 ms — coarse for latency. `--capture-fps 120+` helps but grows `video_rgb24.raw`; confirm the backend actually presents fast enough to fill higher-fps buckets rather than backfilling duplicates (which would floor the measured latency at the capture interval).
- **Cursor centroid extraction.** Template-matching the software cursor against a palettized RGB frame is the fiddly part of the analysis script; a cursor-free reference frame + diff is the fallback if template-match is noisy.
- **Reference-hardware dependence.** Absolute numbers vary by GPU/compositor/refresh; thresholds are relative to a *named* baseline machine, and cross-machine comparisons are invalid.
- **Observer effect.** If Layer 1 ever measures *worse* pacing than an un-instrumented build, the hooks are too heavy — keep them to timestamp arithmetic into a preallocated buffer, no I/O on the hot path.
