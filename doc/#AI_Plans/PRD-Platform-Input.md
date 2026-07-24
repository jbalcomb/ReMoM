# PRD — Platform Input Responsiveness & Smoothness Measurement (SDL2 first)

**Status:** Draft
**Owner:** TBD
**Date:** 2026-07-23
**Companion:** `BRA-Platform-Input.md` (decision record), `PLAN-Platform-Input.md` (survey + build-out order)

---

## Problem Statement

The first public release drew consistent, independent feedback that mouse movement feels laggy, glitchy, stuttery, and rough. We cannot act on it: there is no way to **reproduce** the symptom deterministically, no way to **quantify** it (latency? jitter? both? how much?), no way to **localize** where in the input chain it originates, and no way to tell whether a change **helped or hurt**. Worse, input pacing has already been altered several times (the `SDL_Delay(10)` removal in the event handler, the `Release_Time` busy-wait rewrite) with no baseline and no regression guard — every such change is a plausible improvement backed by no evidence, and nothing prevents the next change from silently undoing it.

## Solution

Build a three-layer input-measurement toolkit for the **SDL2 backend** (the shipped one), plus a characterization baseline and relative regression thresholds. Each layer covers a span of the chain the others cannot (see `BRA-Platform-Input.md` §4):

1. **Layer 1 — Internal FWV timeline.** Promote the existing `DBG_Frame_Reset` counters to a per-event, in-memory-buffered **fixed-width values** table (`.fwv`, project convention — see `tools/field_catalog/fwv.py`, *not* CSV): time of each `Platform_Event_Handler` / `Platform_Pump_Events` tick, inter-tick gap (worst-case cursor freeze), `SDL_RenderPresent` block time, presents-per-displayed-frame, OS-poll interval, and cursor step size per move. Flushed at exit. This *localizes* the cause.
2. **Layer 2 — Synthetic replay + capture harness.** Drive a known mouse trajectory through the existing `.RMR` replay (which already carries `timestamp_ms,mouse_x,mouse_y` per frame), capture the presented frames via `--capture`, and analyze offline: extract the drawn cursor position per captured frame and compare against the injected trajectory to compute *engine→present* latency, dropped/duplicated samples, and interval jitter. This is the **automatable, regression-guarding** layer.
3. **Layer 3 — External motion-to-photon procedure.** A documented high-speed-camera method (240 fps phone slow-mo, ~4 ms/frame) that measures true end-to-end latency including compositor and display, with the "leave the OS cursor visible" differential trick. Manual; run periodically to calibrate the absolute number the internal layers can't see.

All three report **latency and jitter as separate distributions** (p50/p95/p99), never as a mean. The toolkit is measurement only; fixes are a separate, measurement-gated effort per the BRA.

## User Stories

1. As a ReMoM developer, I want to reproduce the reported lag **deterministically** from a recorded trajectory, so that I can study it without a human wiggling the mouse.
2. As a ReMoM developer, I want the **worst-case cursor freeze** (longest gap between input ticks) reported per session, so that I know whether the symptom is freezes or slow-but-even pacing.
3. As a ReMoM developer, I want a **fixed-width (FWV) timeline** of present-block time and presents-per-frame, so that I can see whether vsync-blocked presents are stacking (2–3 per displayed frame).
4. As a ReMoM developer, I want the **cursor step size** per update logged, so that I can distinguish smooth tracking from teleport/quantization at non-integer window scales.
5. As a ReMoM developer, I want an **automated harness** that drives a known trajectory and outputs latency/jitter numbers, so that a responsiveness regression can fail a build without a human watching.
6. As a ReMoM developer, I want a **ground-truth motion-to-photon** number from a documented camera procedure, so that I can calibrate what the internal instruments can't see past the present call.
7. As a ReMoM developer, I want to **film the OS cursor and the game cursor together** (temporarily un-hiding the OS cursor), so that the gap between them directly reads out my engine's *added* latency.
8. As a ReMoM developer, I want the metrics reported as **percentiles, not means**, so that a fat tail of stutter is visible even when average FPS looks fine.
9. As a ReMoM developer, I want a **captured baseline** on reference hardware with committed thresholds, so that "regression" means "measurably worse than baseline," not a guess.
10. As a ReMoM developer, I want the harness to record whether the run was on **X11 or Wayland**, so that a platform-specific input difference is attributable rather than mysterious.
11. As a ReMoM developer, I want the instrumentation **behind a debug flag and buffered in memory**, so that measuring the input does not itself add the latency I'm measuring.
12. As a ReMoM developer proposing a fix, I want to **re-run the harness before/after** and see the delta, so that no responsiveness change lands on vibes.
13. As a ReMoM developer, I want the hooks placed at the **Platform seam**, so that the SDL3 backend can later inherit the same measurement mechanically.

## Implementation Decisions

- **Reuse `DBG_Frame_Reset`, don't replace it.** The counters at [`sdl2_PFL.c:401-439`](../../platform/sdl2/sdl2_PFL.c#L401-L439) already compute inter-tick delta, max delta, and event/mouse-update counts. Layer 1 adds a per-event ring buffer feeding a fixed-width values file; the 60-frame summary log stays.
- **On-disk tabular output is FWV, not CSV.** New metrics/dump files use fixed-width, column-aligned values (`.fwv`) via `tools/field_catalog/fwv.py` — project convention (aligns for eyeball-diffing, predictable for `awk`/`cut`). The pre-existing `.RMR` replay format stays as-is; the preference governs *new* files this effort creates.
- **Wrap `SDL_RenderPresent` with tick timing** at [`sdl2_Video.c:132`](../../platform/sdl2/sdl2_Video.c#L132) to capture present-block time and count presents between displayed frames — the direct test of the "stacked vsync presents" hypothesis.
- **Layer 2 reuses `.RMR` replay + `--capture` verbatim.** No new injection path. The replay CSV header [`Replay.c:172`](../../platform/replay/Replay.c#L172) already carries `timestamp_ms,delta_ms,mouse_x,mouse_y`; `--capture` [`ReMoM.c:415-440`](../../src/ReMoM.c#L415-L440) already emits fixed-fps RGB. The new artifact is an **offline analysis script** (Python) that reads the capture's `video_rgb24.raw` + `capture_info.txt`, finds the cursor centroid per frame, and joins it against the replay timeline.
- **Metrics are latency percentiles + interval jitter.** Latency = frames between an injected position change and the captured frame that shows it, scaled by capture fps. Jitter = p95−p50 and stddev of inter-present intervals and of cursor step size.
- **Windowed substrate only.** The harness links the real `Platform` (SDL2), not `Platform_Headless` — input has no meaning headless.
- **No original pacing code is touched in the measurement phase.** All Layer 1 hooks are additive, `/* CLAUDE */`-marked, and compiled only under the measurement flag. The observer-effect rule (BRA §6.4) means the metrics file buffers in memory and flushes at exit — never per-event `fprintf`.
- **Both build systems** (CMake and Autotools) list any new sources; the measurement flag is wired in both.

## Testing Decisions

- **Characterization, relative thresholds.** Capture a baseline (Layer 1 FWV + Layer 2 numbers + a Layer 3 camera reading) on a named reference machine, commit it, and express regression thresholds as a percentage worse than baseline — not as an absolute millisecond SLA (deferred per BRA §7).
- **Layer 2 is the CI-able artifact.** A fixed seed, a checked-in trajectory `.RMR`, a `--capture` run, the analysis script, and a threshold check, wired as a CTest test under the existing HeMoM/CTest fixture pattern (Setup → replay+capture Run → analyze+assert). Because it drives real presents, it is **windowed**, so it runs where a display exists (developer machine / a display-capable CI runner), not the headless suite.
- **Layer 3 is manual calibration, not CI.** Documented procedure with a results log; run per milestone, not per build.
- **Distributions, not means** — every reported metric is a percentile set. A regression on p95/p99 fires even if p50 is unchanged.
- **The harness self-reports its environment** — backend (SDL2/SDL3), window scale, refresh rate, and X11-vs-Wayland — into the FWV file header so numbers are comparable only across like environments.

## Windows (native Win32) backend

Added 2026-07-24. The measure-then-fix discipline and the hardware-cursor fix apply to the native Win32 backend (`USE_WIN32`) as much as to SDL2/SDL3. Two things make Windows distinct from the deferred SDL3 case: the fix commit left it **failing to link**, and — unlike SDL3 — it is **locally buildable and runnable** on the Windows dev box, so it can be fully verified rather than ported-and-hoped.

### Requirements (all delivered 2026-07-24)

- **W1 — Restore the `USE_WIN32` build (blocker). ✅ DONE.** `Platform_HW_Cursor_Active`/`Refresh` are now defined in `win_MD.c`, resolving the `LNK2019`. `ReMoM.exe`/`demo_vga.exe` link again. (Implemented as the full fix, not a stub.)
- **W2 — Hardware cursor on Win32. ✅ DONE.** `Platform_HW_Cursor_Active`/`Refresh` implemented in `win_MD.c` — `HCURSOR` built from the game sprite via `CreateIconIndirect` (32-bit BGRA `BITMAPV5HEADER` DIB + all-zero mask), same `REMOM_HW_CURSOR` gate, off by default. Same sprite conversion as SDL2 (column-major, index-0 transparent, scale-upscaled, top-left hotspot). `WM_SETCURSOR` handled (re-asserts via `Win_HW_Cursor_Apply`); init shows the OS cursor when active; `Platform_Maybe_Move_Mouse` skips the software path when active.
- **W3 — Layer 1 metrics on Win32. ✅ DONE.** Tick hooks in `Platform_Event_Handler`/`Platform_Pump_Events`, poll in `Platform_Maybe_Move_Mouse`, present bracket around the `Win_Blit_Back_Buffer` blit, and `Input_Metrics_Init/Shutdown` in `Startup_/Shutdown_Platform`. Verified: `REMOM_INPUT_METRICS=1` produces a real `.fwv` (`backend=Win32`, 27 present rows).
- **W4 — Verify locally. ✅ Build + run + metrics verified** on this Windows box (`MSVC-win32-debug`): links with zero `/W4 /WX` warnings, launches to the title screen, writes a well-formed Win32 `.fwv`. **Remaining:** the cursor-smoothness **visual A/B** and a mouse-moving Layer 1 baseline — the same visual step the SDL2 fix also needed, and doable locally.

Implementation specifics live in `PLAN-Platform-Input.md` §*Fix — hardware cursor: Win32 port*.

## Out of Scope

- **Fixing the lag.** This PRD delivers measurement and a regression guard, not corrections. Present-coalescing, vsync strategy, hardware/OS cursor, and `Release_Time` sleep replacement are the gated follow-on (BRA §5, §7).
- **SDL3 parity.** Hooks are placed to make it mechanical later; not measured or validated here.
- ~~**Win32 parity.**~~ **Now in scope — see *Windows (native Win32) backend* below.** Promoted because the SDL2/SDL3 fix commit (`53eb71b`) left the `USE_WIN32` backend **not linking** (`Mouse.c`→`Platform_HW_Cursor_Active` unresolved), so Win32 is no longer merely a "later, mechanical" parity item — it is a build regression plus the same smoothness fix.
- **A byte/pixel diff of the cursor sprite.** Layer 2 locates the cursor centroid; it does not compare sprite pixels to a golden.
- **An absolute latency SLA.** Set after the baseline exists.
- **Touch, trackpad-gesture, and gamepad input.** Mouse only.
- **Matching the DOS original's input feel.** Explicitly rejected as a target (BRA §1).

## Further Notes

- **Layer 2 measures only the engine→present half of the chain.** Replay injects game-coordinate positions straight into the engine globals ([`Replay.c:572`](../../platform/replay/Replay.c#L572)), bypassing `SDL_GetMouseState`. It therefore cannot see OS→engine latency — that gap is Layer 3's job. This is a deliberate scoping boundary, not a defect.
- **Capture resolution is bounded by `--capture-fps`.** `PFL_Capture` backfills gaps by duplicating the last presented frame, so a cursor freeze shows up as a run of identical frames — a usable smoothness signal — but the *time* resolution is one capture interval (~16 ms at 60 fps). Raising `--capture-fps` sharpens Layer 2's latency resolution at the cost of larger raw video.
- **The existing `MOUSE_LOG` (behind `MOUSE_DEBUG`) is trustworthy for ordering, not timing.** Its per-line `fprintf` inside the event path perturbs the very timing it records — the reason Layer 1's FWV file must buffer in memory.
- **X11 vs Wayland is a first-class variance source.** Under Wayland, SDL2 typically runs through XWayland and absolute-pointer/warp semantics differ; the difference can *be* the reported bug. The harness records which is active so a divergence is attributable.
- **This is the same discipline as Combat-Testing:** deliver observation first, pin current behavior, and make every subsequent change prove itself against a baseline.
