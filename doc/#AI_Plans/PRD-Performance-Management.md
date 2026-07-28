# PRD — Software Performance Management System

**Status:** Draft
**Owner:** TBD
**Date:** 2026-07-25
**Companion:** [`BRA-Performance-Management.md`](BRA-Performance-Management.md) (decision record), [`PLAN-Performance-Management.md`](PLAN-Performance-Management.md) (build-out + diagram), [`GUIDE-Frame-Rate-Performance.md`](GUIDE-Frame-Rate-Performance.md) (practical playbook). Addenda: [`ADDENDUM-Performance-Danger-Zones.md`](ADDENDUM-Performance-Danger-Zones.md), [`ADDENDUM-Performance-Common-Mistakes.md`](ADDENDUM-Performance-Common-Mistakes.md).

---

## Problem Statement

Players report intermittent frame-rate slowdowns and we cannot act: no way to **reproduce** deterministically, **quantify** (how slow, how often, how bad the tail), **localize** (logic vs. render; which call), or **prove** a change helped or hurt. The raw ingredients exist but are unconnected: a ms clock, a per-present `.fwv` recorder (`PFL_Input_Metrics`), a Debug per-frame counter (`DBG_Frame_Reset`), a hand-toggled `PHASE` timing macro in one file, a deterministic headless runner (HeMoM), and `.RMR` replay. There is no unified sink, no percentile report, no live readout, and no regression gate.

## Solution

A small **in-house performance layer** — unified clock + a build-gated **zone** primitive (the graduated `PHASE`) + one **frame-time sink** with percentile reporting + **live readouts** + **deterministic perf tests** (headless-logic and windowed-render) + a **CTest regression gate** against reference-hardware baselines — that **interoperates with** external profilers for deep dives without depending on any. Measurement and guarding ship first; optimizations are separate, measurement-gated work (per BRA).

All reports are **frame time in milliseconds, as p50/p95/p99/max distributions**, against the **55 ms tick budget** and a named baseline — never a mean, never a bare FPS.

## User Stories

1. As a dev, I want **one switch** (`REMOM_PERF=1` / a config key) that turns on performance capture, so I don't wire up four subsystems by hand.
2. As a dev triaging a report, I want the capture to say **logic-time vs render-time** per frame, so I know which half to chase.
3. As a dev, I want **frame-time percentiles (p50/p95/p99/max)** over a session, so the fat-tail hitches are visible where a mean would hide them.
4. As a dev, I want a **live FPS/frame-time readout** (window title + Debug screen), so I get immediate feedback while reproducing a slow case.
5. As a dev, I want to wrap a suspect call in a **zone** and see its time in the same report, **without hand-editing per-file `#define`s** and **with zero cost in Release**.
6. As a dev, I want to run a **deterministic headless scenario** (fixed seed / save, N turns) and get per-stage logic ms, so I can profile the AI/turn sink in CI with no display.
7. As a dev, I want a **deterministic windowed replay** that records frame-time and asserts p95/max against a **committed baseline**, so a render regression fails the build.
8. As a dev, I want the perf test to **self-report its environment** (backend, CPU, build type, scale) into the report header, so numbers are only compared across like environments.
9. As a dev doing a deep dive, I want a **recommended external profiler per platform** and a deterministic workload to point it at, so I'm not reinventing flame graphs.
10. As a maintainer, I want the instrumentation **off and stripped in Release by default**, so shipped players pay nothing and the hot path isn't perturbed.
11. As a reviewer, I want the zone instrumentation to be **visually contained** (not one wrapper smeared across every line) so it doesn't wreck readability.
12. As a dev proposing an optimization, I want to **re-run the gate before/after** and see the delta, so no perf change lands on vibes.

## Functional Requirements

### The measurement core
1. **Unified clock.** All timing uses the existing monotonic `Platform_Get_Millies()` (ms). A higher-resolution `Platform_Get_Micros()` may be added if ms proves too coarse for sub-call zones (55 ms frames make ms adequate for frame-level; zones inside a frame may want µs).
2. **One frame-time sink.** Reuse/extend `PFL_Input_Metrics`' `.fwv` as *the* structured sink for frame rows **and** zone rows. Single format, `tools/field_catalog/fwv.py`-readable, in-memory buffered, periodic `fflush` (crash-safe), no per-row syscall. Header self-reports backend / build / CPU / scale (US8).
3. **Single gate.** One runtime switch, `REMOM_PERF` (env) and/or `[Perf] enable` in `ReMoM.ini`, turns capture on. Unset/`0` costs one branch on the hot path and no I/O. Supersedes the ad-hoc `REMOM_INPUT_METRICS` gate (kept as an alias).
4. **Percentile reporting.** A small reporter computes p50/p95/p99/max (and count over-budget, i.e. > 55 ms) from a `.fwv`, emitted at shutdown to log and optionally stdout. Never report a mean alone.
5. **Logic vs render split.** Frame rows carry present/render time separately from a per-frame logic total, so the split in US2 is directly readable.
5a. **Tick-aware frame accounting — intentional wait vs. overrun (the `dt_ms` trap).** A "frame" in MoM is a **55 ms logic tick**, and `Release_Time(N)` advances the clock by N ticks while holding the same image. The present-to-present interval (`dt_ms`, from the input metrics) **conflates** a legitimate `Release_Time(3)` hold (three idle on-budget ticks) with a single frame that overran — because `Release_Time` holds without presenting ([`Timer.c:76`](../../MoX/src/Timer.c#L76)), a multi-tick hold collapses into one long interval. *(Confirmed by the first real capture — 2026-07-28, SDL2 release: median frame exactly on budget (55 ms) but a fat tail, p95 = 111 ms / p99 = 166 ms / 47% of intervals > 55 ms, with **94% of frame time non-vsync work** — which this conflation inflates.)* The perf metric therefore accounts in **logical ticks, not present intervals**: `Release_Time` **reports its `N`** to the perf layer, a hold is recorded as N idle **on-budget** ticks, and **over-budget** means *a single tick's **work** exceeded 55 ms*, never *an interval was long*. This is the surgical fix (one marked hook call in `Release_Time`); it is **distinct from** actually re-presenting each tick (Option B — deferred, see Out of Scope). The clean logic number needs neither: Mode A headless has no `Release_Time` waits at all.

### The zone primitive (graduated `PHASE`)
6. **Build-gated zone macros**, e.g. `PERF_ZONE("name") { ... }` or `PERF_BEGIN(id)/PERF_END(id)`, that:
   - compile to **timestamp-into-preallocated-buffer** when `PERF_INSTRUMENT` is defined, and to **nothing** (a bare pass-through / empty) otherwise;
   - are gated by **one build option** wired in CMake + Autotools (default **OFF in Release**, optional in Debug) — *not* hand-edited `#define`s per file;
   - feed the shared sink (FR2), not `LOG_INFO`;
   - never allocate or format strings in the zone body (observer effect).
7. **Migrate `NEXTTURN.c`'s `PHASE`** to the new primitive as the reference adopter, preserving its stage breakdown, and delete the hand-toggled two-`#define` scheme.
8. **Nested zones** produce a stage tree (frame → logic → AI_Next_Turn → …) so localization can drill in.

### Live readouts (the GUIDE's three ideas)
9. **FPS/frame-time in the window title** via a new `Platform_Set_Window_Title()` (titles are static today), updated ~1 Hz with rolling avg **and worst-frame-in-window**. Cheap enough to ship.
10. **Debug-screen readout** on `DBG_Main_Screen_Draw_Summary_Window` — richer (p50/p95/p99/max, top zones), Debug build only.
11. **Periodic summary log** — a time-based (~2 s) percentile line, extending `DBG_Frame_Reset`; always-on in Debug, gated in Release.

### Deterministic perf tests + CI gate
12. **Headless logic perf (Mode A).** A deterministic HeMoM scenario (fixed seed / committed late-game save, N turns) times logic zones with **no rendering**; emits per-stage ms. This is the backbone (logic is the sink).
13. **Windowed render perf (Mode B).** A deterministic `.RMR` replay through a real backend with capture on; emits frame-time `.fwv`. Generalizes Layer 2 of the input plan.
14. **Regression gate.** A CTest test asserts each mode's percentiles are **not measurably worse than a committed baseline** on named reference hardware (relative thresholds on p95/p99/max), non-zero exit on regression. Mode A runs anywhere (headless); Mode B runs where a display exists.
15. **Baseline artifacts** are committed `.fwv`/summary files tagged with the reference machine and build; cross-machine comparison is explicitly invalid.

### External-tool interop
16. **Documented external-profiler on-ramp** (see §"External tools" below): a recommended profiler per platform and the deterministic HeMoM workload to attach it to. No external tool is a build dependency.
17. **Optional Tracy emission** behind a separate build flag (`PERF_TRACY`): the same zones emit Tracy zones for interactive flame-graph sessions. Off by default; never in Release.

## Instrumentation ergonomics — the `PHASE`/zone clutter problem (called out explicitly)

The current `PHASE` macro (`MoM/src/NEXTTURN.c:681`) is instructive about the cost, and the design must manage it:

- **Clutter.** Every measured call becomes `/* CLAUDE */ PHASE(Some_Call());` — one wrapper per line, plus a comment prefix. A function's call sequence turns into a column of `PHASE(...)`. Wrapping the whole per-frame path across many files would smear this everywhere and hurt readability (US11). **Mitigations required:** (a) prefer **scope-based** zones (`PERF_ZONE("stage") { ...block... }`) that bracket a region rather than wrap each call; (b) keep zone density **coarse** — one per meaningful stage, drill in only when a stage is guilty, then remove; (c) a C `__attribute__((cleanup))`-based scoped timer (or a `for`-loop-scope trick) so a single line opens a zone for the rest of the block, no closing call to forget.
- **Debug vs. Release — the current wart.** `PHASE` is toggled by **hand-commenting two `#define`s** in the file:
  ```c
  #define PHASE(CALL) do { ...time... } while(0)   // instrumented
  // #define PHASE(CALL) CALL                        // pass-through
  ```
  This is manual, per-file, and unrelated to the build type — easy to commit in the wrong state, invisible to Release. **Requirement:** replace with a **single build option** (`PERF_INSTRUMENT`, off in Release) so the compiler strips all zones from Release automatically and no source edit is needed to switch. Verified by: a Release build contains **no** zone timing code (checked by symbol/string absence, like the `dbg_prn`/`trc_prn` Release check already in the pre-push gate).
- **Blind spots.** Manual zones only measure what you wrap; unwrapped time hides in the parent. Accept this (it's the instrumentation tradeoff) and cover it with a **sampling external profiler** for the "what did I forget to wrap" question (§External tools).
- **Observer effect.** A zone must be timestamp-arithmetic only. No `LOG_` inside a zone body, no `snprintf`, no allocation — those perturb the very time being measured (the reason the input `.fwv` buffers in memory rather than `fprintf`-per-event).

## External tools & industry standards (required coverage)

The in-house layer is the system of record; these are for **deep dives** once it localizes a hotspot. None is a build dependency.

### Profiler families
- **Instrumentation profilers** (precise, need code/markers, some overhead, no blind spots for what's marked): our zones; **Tracy** (frame-oriented, real-time, MIT — the closest external match to our zones and the recommended interactive tool); **Callgrind/KCachegrind** (Valgrind — instruction-exact, deterministic, ~20–50× slow, ideal for reproducible logic profiling; we already ship valgrind suppressions).
- **Sampling profilers** (near-zero overhead, no code changes, statistical, find the blind spots): **Superluminal** (Windows, commercial, excellent UX); **Intel VTune** (Windows/Linux, free, hardware counters); **Very Sleepy** (Windows, free, simple); **Visual Studio Performance Profiler** (built into our VS2022 — CPU Usage/Instrumentation, zero setup); **Linux `perf`** (ubiquitous, hardware counters); **Windows Performance Toolkit / ETW (WPR+WPA)** (system-wide, free).

### Recommended per platform (deep dive)
- **Windows, interactive:** VS Performance Profiler (zero setup) → Superluminal or Tracy for frame-level flame graphs.
- **Windows, system-wide stalls (I/O, scheduler):** WPR/WPA (ETW).
- **Linux:** `perf record`/`perf report` + flame graph; **Callgrind** for deterministic call-graph on the HeMoM headless workload.
- **Deterministic/CI-ish deep profile:** Callgrind on the Mode A HeMoM scenario — repeatable instruction counts, no timing noise.

### Industry standards adopted
- **Think in frame-time budgets (ms), not FPS.** Our budget is 55 ms. (16.67 ms at 60 Hz is the general-industry figure; ours is the DOS tick.)
- **Percentiles / "1% low" framing.** p95/p99/max and count-over-budget — the game-benchmarking industry's smoothness metric; a mean is banned.
- **Flame graphs** (Brendan Gregg) as the localization visualization for sampling data.
- **USE method** (Utilization/Saturation/Errors) for reasoning about a saturated stage.
- **Profile optimized/Release-like builds** for representative numbers; use Debug only for correctness of the instrumentation itself.
- **Deterministic repro** as the precondition for any logic profile (seed/save).
- **Continuous performance testing** — perf budgets enforced in CI, the same way correctness is (our CTest gate).
- **Observer-effect discipline** — buffer, don't `fprintf`; measure what you ship, not what the instrument added.

## Acceptance Criteria

- [ ] With perf off, the app makes no perf I/O and the hot path adds at most one branch; a Release build contains no zone-timing code (symbol/string check).
- [ ] `REMOM_PERF=1` produces one `.fwv` with frame rows carrying separate render and logic time, and a shutdown percentile summary (p50/p95/p99/max, count > 55 ms).
- [ ] A zone added with the new macro appears in the report and requires **no** per-file `#define` edit to enable/disable across Debug/Release.
- [ ] The window title shows a live frame-time/FPS with a worst-frame figure; the Debug screen shows the percentile set.
- [ ] Mode A (HeMoM headless, fixed seed/save, N turns) emits per-stage logic ms deterministically (same numbers ±noise on re-run).
- [ ] Mode B (windowed `.RMR` replay) emits a frame-time `.fwv`; a CTest test fails when p95/max exceed the committed baseline by the threshold.
- [ ] The docs name a recommended profiler per platform and the exact HeMoM workload to attach it to.
- [ ] `NEXTTURN.c`'s stage breakdown is preserved after migrating off the hand-toggled `PHASE`.

## Out of Scope

- **The optimizations themselves** (gated follow-on, chosen by the baseline).
- **Present-per-tick `Release_Time` refactor (Option B).** Making `Release_Time` blit once per tick so present-cadence == tick-rate (with a display-liveness upside) is **deferred, a measured decision** — it re-presents static content (N vsync-blocked blits per hold) and is a larger divergence from reconstructed pacing than the FR5a hook. Revisit only if a measured benefit justifies it; the deepest form (a true fixed-timestep loop) is a bigger reconstructed-code rewrite still. The tick-aware metric (FR5a) fixes measurement without any of it.
- **Field telemetry / performance uploads** (privacy + infra; later).
- **GPU profiling** (negligible for 320×200 software blit; revisit with an upscale path).
- **Engine multithreading.**
- **An absolute FPS SLA** (budget is 55 ms/frame; targets are percentile-vs-baseline).
- **macOS** deep-profiling parity (macOS is deferred project-wide).
