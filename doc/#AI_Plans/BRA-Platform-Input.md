# BRA — Platform Input Responsiveness & Smoothness Architecture Decision

**Status:** Proposed — awaiting owner sign-off. Layer 1 recommended for immediate funding; Layers 2/3 sequenced behind it.
**Date:** 2026-07-23
**Author role:** Business Requirements Analyst (decision record)
**Related:** `PRD-Platform-Input.md`, `PLAN-Platform-Input.md`, [`PRD-SDL2-Platform-Backend.md`](PRD-SDL2-Platform-Backend.md), [`PlatformLayer-Input.md`](../PlatformLayer/PlatformLayer-Input.md), [`RecordReplay-Overview.md`](../RecordReplay/RecordReplay-Overview.md)

This records *how* we decided to approach input responsiveness — the goal, the criteria weighed, the measurement options considered and rejected, and specifically the decision to **measure before we fix** and to layer three complementary instruments rather than pick one. It is a decision record, not an implementation plan.

---

## 1. The ultimate goal

The first public release drew consistent, independent feedback that mouse movement feels **laggy, glitchy, stuttery, rough**. The goal is input that feels responsive and smooth on modern hardware, and — because "feels laggy" is not a work item — the ability to **measure it, guard it against regression, and prove that any change helped**.

Two decisions hide inside "smooth input" and must be made on purpose:

- **The reference is a modern native application, not the DOS original.** OG-MoM's input feel came from 70 Hz VGA, BIOS timer ticks, and an INT 33h mouse driver — a substrate that no longer exists. This is *not* a fidelity target: matching the disassembly's *pacing* would reproduce the lag, not cure it. Input responsiveness is a **platform-substrate concern**, explicitly outside the 100%-fidelity rule. (Contrast: combat *outcomes* are fidelity-governed; cursor *smoothness* is not.)
- **"Smoothness" is two distinct measurable quantities, not one.** *Latency* (responsiveness — how long from a physical move to the cursor moving on the glass) and *jitter* (smoothness — how evenly spaced the updates are). They have different causes and different fixes, and a change can improve one while worsening the other. They must be measured separately.

---

## 2. Current-state finding

We have been changing input pacing already — **blind, without a baseline or a regression guard**.

| Fact | Evidence |
|---|---|
| The cursor is **software-drawn into the framebuffer**, so a cursor move costs a full `Platform_Video_Update()` → `SDL_RenderPresent()` | [`sdl2_PFL.c:170-186`](../../platform/sdl2/sdl2_PFL.c#L170-L186) |
| **VSync is on** — `SDL_RenderPresent` blocks until vblank | [`sdl2_Init.c:99`](../../platform/sdl2/sdl2_Init.c#L99) |
| There are **many present sites**; cursor motion presents on *every* move, so 2–3 vsync-blocked presents can stack per displayed frame | [`sdl2_Video.c:132`](../../platform/sdl2/sdl2_Video.c#L132), [`sdl2_PFL.c:184`](../../platform/sdl2/sdl2_PFL.c#L184), [`Video.c:389`](../../MoX/src/Video.c#L389) |
| The mouse is **polled** (`SDL_GetMouseState`), not consumed from `SDL_MOUSEMOTION` events — cursor cadence == poll cadence | [`sdl2_PFL.c:572-575`](../../platform/sdl2/sdl2_PFL.c#L572-L575), [`sdl2_PFL.c:605`](../../platform/sdl2/sdl2_PFL.c#L605) |
| Pacing has **already been changed ad hoc**: `SDL_Delay(10)` removed from the handler; `Release_Time`'s double busy-wait rewritten | [`sdl2_PFL.c:627`](../../platform/sdl2/sdl2_PFL.c#L627), [`Timer.c:76-89`](../../MoX/src/Timer.c#L76-L89) |
| Internal instrumentation **already exists** but only prints a 60-frame summary behind `STU_DEBUG` | `DBG_Frame_Reset` [`sdl2_PFL.c:416`](../../platform/sdl2/sdl2_PFL.c#L416); `MOUSE_LOG` behind `MOUSE_DEBUG` |
| A reproducible-input substrate **already exists**: `.RMR` replay is a **text CSV carrying `timestamp_ms,delta_ms,mouse_x,mouse_y`** per frame | [`Replay.c:172`](../../platform/replay/Replay.c#L172), [`Platform_Replay.h`](../../platform/include/Platform_Replay.h) |
| A frame-capture path **already exists**: `--capture DIR [--capture-fps N]` writes fixed-fps RGB video, backfilling gaps by duplicating the last frame | [`PFL_Capture.c`](../../platform/capture/PFL_Capture.c), [`Platform_Capture.h`](../../platform/include/Platform_Capture.h) |

Three findings reshaped the effort:

**Finding A — we have been fixing blind.** Every pacing change above is a plausible improvement with *no measured evidence* it helped, and no guard that the next change won't quietly undo it. The scarce thing is not fix ideas; it is a way to tell a fix from a regression.

**Finding B — most of the measurement scaffolding already exists.** The DBG counters already compute the worst-case cursor freeze; the `.RMR` format already records a timestamped input trajectory; `--capture` already records what reached the screen over time. The work is to *connect and quantify*, not to build a new subsystem.

**Finding C — the fidelity line runs through the pacing code.** The `platform/` layer is the modern substrate and is *not* fidelity-governed — responsiveness work belongs there by default. But some pacing lives in **reconstructed** MoX code (`Release_Time` in `Timer.c`, `Get_Input` in `Input.c`). Touching those is a deliberate divergence from the disassembly and must be `/* CLAUDE */`-marked with a recorded justification, exactly as the existing pacing changes already are.

---

## 3. Criteria considered

| Criterion | Why it matters |
|---|---|
| **Separates latency from jitter** | They have different causes/fixes; an instrument that conflates them can't guide a fix. |
| **Substrate fidelity** | Input can only be measured where it runs — **windowed, against a real display**. Headless has no cursor and no present. |
| **Coverage of the chain** | The lag lives somewhere in OS→poll→engine→render→present→compositor→glass. An instrument that sees only part of the chain can mislocate the cause. |
| **Automatability / regression-guard** | Can it run pass/fail so a regression fails a build, without a human watching? |
| **Ground truth** | Only an external measurement sees the *whole* chain including compositor and display buffering. |
| **Reuse of existing infrastructure** | `DBG_Frame_Reset`, `.RMR` replay, `PFL_Capture`, HeMoM, CTest. |
| **Observer effect** | The instrument must not add the latency it is measuring (per-event `fprintf` does). |
| **Fidelity cost** | Does the measurement (or the eventual fix) touch reconstructed original code? |

### Criteria explicitly excluded

- **A single absolute latency SLA in milliseconds.** Deferred until a baseline exists; targets are set *after* we can measure, and expressed relative to that baseline.
- **Pixel-exact cursor-sprite comparison.** We measure *where* and *when* the cursor is, not whether its sprite bytes match a golden.
- **Matching the DOS original's input feel.** Rejected as a target per §1.

---

## 4. The crux

Two crux points decided the shape of this effort.

**Crux 1 — you cannot fix what you cannot measure, and we have been fixing blind (Finding A).** The decisive criterion is therefore *measure-before-fix*: establish a baseline and a regression guard **first**, and gate every subsequent responsiveness change on a measured delta. This mirrors the Combat-Testing decision — deliver *observation*, not *correction*, as the first product.

**Crux 2 — no single instrument sees the whole chain.** Internal instrumentation sees the game loop but goes blind at the `SDL_RenderPresent` call boundary. Replay-driven measurement sees *engine → present* but **bypasses the OS poll** (it injects game-coordinate positions straight into the engine globals — [`Replay.c:572`](../../platform/replay/Replay.c#L572)), so it cannot measure OS→engine latency. Only an external high-speed camera sees compositor and display, but it can't run in CI and can't localize a cause. No layer is sufficient; they are complementary. The decision is to **layer them**, each answering the question the others can't.

---

## 5. Options considered

### Option A — Keep relying on subjective playtest reports (status quo)
- **Verdict: Rejected.** "Feels laggy" is unreproducible, un-regressable, and un-attributable. It is exactly what produced Finding A.

### Option B — Internal instrumentation only (structured CSV timeline)
Turn the existing DBG counters into a per-event CSV: present-block time, poll intervals, cursor step size, presents-per-frame.
- **Pros:** cheapest; most *diagnostic* — it localizes the cause (freeze vs. vsync stall vs. teleport); reuses `DBG_Frame_Reset`.
- **Cons:** goes blind at the present boundary — cannot measure true motion-to-photon; sees nothing the compositor adds.
- **Verdict: Necessary but not sufficient.** Adopted as Layer 1, not as the whole answer.

### Option C — External motion-to-photon only (high-speed camera)
Film the screen at 240 fps, count frames from physical move to cursor move.
- **Pros:** authoritative end-to-end number; sees the whole chain.
- **Cons:** manual; not automatable; gives a number but no localization; hardware/setup dependent.
- **Verdict: Right for ground truth, wrong as the sole model.** Adopted as Layer 3.

### Option D — Layered: internal CSV + synthetic replay/capture harness + periodic external motion-to-photon  ✅ **chosen**
Three instruments, each covering a different span of the chain, with SDL2 as the reference backend and a measure-first scope.
- **Pros:** Layer 1 localizes; Layer 2 (replay-driven `--capture`, offline-analyzed) is automatable and regression-guards *engine→present*; Layer 3 calibrates the absolute number and catches what the others structurally can't. Maximal reuse of existing infrastructure.
- **Cons:** three instruments to build/maintain; Layer 2 measures only the engine→present half; absolute numbers are reference-hardware dependent.
- **Verdict: Accepted.**

### The scope question — does this effort also *fix* the lag?
- **Decision: characterize and guard now; fixes are a gated follow-on.** The measurement phase is purely additive (behind a debug flag, in-memory buffered, no original code touched). Each candidate fix — reduce presents-per-frame, decouple the cursor from vsync, a hardware/OS cursor, coarser-sleep replacement in `Release_Time` — is then justified by a measured improvement against the baseline, and any change to reconstructed pacing code is `/* CLAUDE */`-marked per Finding C. This keeps us from trading one blind change for another.

---

## 6. Decision

**Implement Option D. Fund Layer 1 now; sequence Layers 2 and 3 behind it. Measure and guard first; fixes are a separate, measurement-gated effort.**

Supporting decisions taken with the same stroke:

1. **SDL2 is the reference backend.** It is what shipped and what users are reporting on ([`PRD-SDL2-Platform-Backend.md`](PRD-SDL2-Platform-Backend.md)). Instrument at the seams so SDL3 inherits the same hooks, but validate against SDL2 first.
2. **Latency and jitter are reported separately, as distributions, never as a mean.** A 60 fps *average* with a fat tail of 40 ms gaps is exactly the "stutter" being reported; percentiles (p50/p95/p99) expose it, the mean hides it.
3. **Windowed is the only valid substrate.** Input measurement links the real `Platform`, not `Platform_Headless` — same principle as the tactical-combat harness: test on the substrate the code was designed for.
4. **The instrument must not perturb what it measures.** Measurement is behind a debug flag and buffers in memory (ring buffer flushed at exit), not per-event `fprintf` — unlike the existing `MOUSE_LOG`, whose per-line disk writes make it trustworthy for *ordering* but not for *absolute timing*.
5. **The fidelity rule holds for the fix phase.** Responsiveness work lives in `platform/` by default; any change to reconstructed pacing code (`Release_Time`, `Get_Input`) is a deliberate, marked, justified divergence.

---

## 7. What this explicitly does not decide

- **The fixes themselves.** Present-coalescing, vsync strategy, hardware/OS cursor, render-thread decoupling — all deferred to the gated follow-on, chosen by what the baseline shows.
- **The absolute latency target.** Set after Layer 1 + Layer 3 establish a baseline; expressed relative to it.
- **SDL3 and Win32 parity.** Hooks are placed to make it mechanical later; not measured now.
- **Wayland vs X11 as a supported-matrix question.** Noted as a variance source that can *itself* be the bug; the harness records which is in use, but picking a supported set is out of scope here.
- **Touch, trackpad-gesture, and gamepad input.** Mouse only.

---

## 8. Update — Layer 1 baseline vindicates measure-before-fix (2026-07-24)

The first Layer 1 capture on the reporting hardware (SDL2 / X11 / 4× / 60 Hz; raw data at [`baseline_2026-07-24_x11_scale4.fwv`](../PlatformLayer/input-metrics/baseline_2026-07-24_x11_scale4.fwv), analysis in `PLAN-Platform-Input.md` §*Layer 1 result*) confirms this decision was the right one: **the two causes the plan leaned toward were both wrong.**

- *Stacked vsync presents* (Finding §2 fact #3, the strongest prior suspicion) — **rejected by data**: `block_ms` is 0 for 84% of presents.
- *Cursor freeze from sparse polling* — **rejected**: input is pumped ~829/s; worst tick gap is 2 ms.
- **Actual cause:** the software cursor is drawn into the framebuffer and only reaches the screen at the main loop's ~55 ms BIOS-tick present cadence (~18 fps), while the dedicated cursor-present path fires **0** times. Had we "fixed blind" (Finding A), the obvious first move — attacking vsync/present-coalescing — would have spent effort on a non-cause.

**Consequence for the effort:** the fix is promoted ahead of Layers 2–3, and it is now specific — restore the cursor's independent fast path via a real SDL hardware/OS cursor (the modern equivalent of the DOS hardware sprite). This does not change the layered decision; Layer 2/3 still follow, now as the regression guard around a known fix. It does resolve one "does not decide" item (§7): the fix direction is chosen, by measurement.
