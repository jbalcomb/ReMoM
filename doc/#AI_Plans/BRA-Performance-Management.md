# BRA — Software Performance Management System (Architecture Decision)

**Status:** Proposed — awaiting owner sign-off.
**Date:** 2026-07-25
**Author role:** Business Requirements Analyst (decision record)
**Related:** [`PRD-Performance-Management.md`](PRD-Performance-Management.md), [`PLAN-Performance-Management.md`](PLAN-Performance-Management.md), [`GUIDE-Frame-Rate-Performance.md`](GUIDE-Frame-Rate-Performance.md), [`BRA-Platform-Input.md`](BRA-Platform-Input.md) (the sibling effort whose discipline this generalizes).

This records *how* we decided to approach performance — the goal, the criteria, the tool families considered (in-house instrumentation vs. external profilers vs. sampling vs. CI gating), and the decision to build a small **in-house, always-available, deterministic** performance layer while *interoperating with*, not replacing, best-in-class external profilers. It is a decision record, not an implementation plan.

---

## 1. The ultimate goal

Players report intermittent **frame-rate slowdowns**. "It got slow" is, like "it feels laggy" was for input, an **unreproducible, un-regressable, un-attributable** complaint. The goal is a performance-management *system*: the ability to **confirm** a slowdown is real and quantified, **localize** where the time goes (which layer, which call), **guard** against regression automatically, and **prove** any optimization helped — on developer machines and in CI, with numbers, not feel.

Two things must be decided on purpose, because they shape everything:

- **The unit is frame time in milliseconds, not FPS, and the statistic is a distribution, not a mean.** MoM's loop is paced to the DOS BIOS tick — **55 ms/frame (~18.2 fps) by design** (`PLATFORM_MILLISECONDS_PER_FRAME`). "Slow" = frames **exceeding** that budget. A mean FPS hides exactly the fat-tail hitches players feel; **p50/p95/p99/max** expose them. (Same conclusion the input BRA reached for latency/jitter.)
- **Logic time and render time are separate quantities with separate fixes.** The classic MoM sink is **AI/turn logic** (late game), not drawing. A system that reports one blended number can't guide a fix and will send effort to the wrong layer.

---

## 2. Current-state finding

We are **not** starting from zero; we are starting from scattered, half-connected pieces.

| Asset | State | Evidence |
|---|---|---|
| A monotonic ms clock | exists | `Platform_Get_Millies()` |
| A per-**present** frame-time recorder → `.fwv` | exists, runtime-gated, off by default | `PFL_Input_Metrics` (`REMOM_INPUT_METRICS=1`), built for the input work |
| A per-**frame** counter/summary (frametime, max input gap) | exists, Debug-only, TRACE | `DBG_Frame_Reset` |
| A per-**call** timing macro | exists, ad-hoc, one function | `PHASE(CALL)` in `NEXTTURN.c` |
| A deterministic headless runner | exists | HeMoM (`--seed1`, `--load`, `--continue`) |
| Deterministic input replay | exists | `.RMR` record/replay |
| A frame budget / SLA | exists implicitly | 55 ms tick |
| A debug on-screen summary window | exists | `DBG_Main_Screen_Draw_Summary_Window` |

Findings that shape the effort:

- **Finding A — the pieces don't compose into a system.** The clock, the `.fwv` recorder, the frame counter, and the `PHASE` macro were each built for a local purpose and don't share a sink, a naming scheme, a gate, or a report. There is no single "turn on performance" switch, no percentile report, no CI assertion. The scarce thing is **integration and a regression gate**, not raw capability.
- **Finding B — `PHASE` is the right idea in the wrong packaging.** It proves the value of scoped call timing, but it clutters call sites, is toggled by **hand-editing two `#define`s per file** (not tied to build type), only measures what you manually wrap (blind spots), and dumps to `LOG_INFO` rather than a structured sink. It needs to become a **first-class, build-gated, auto-stripped zone primitive**, not stay an ad-hoc macro. (Detailed in the PRD and PLAN.)
- **Finding C — the fidelity line runs through the pacing code.** Some timing lives in **reconstructed** MoX (`Release_Time` in `Timer.c`, `Get_Input`). Instrumenting *around* them is free; *changing* them (to fix a slowdown) is a deliberate divergence from the disassembly and must be `/* CLAUDE */`-marked with justification. Measurement is additive and fidelity-neutral; the eventual *fixes* are the governed part.

---

## 3. Criteria considered

| Criterion | Why it matters |
|---|---|
| **Separates logic-time from render-time** | Different causes/fixes; a blended number misdirects effort. |
| **Always available, zero-setup** | The reporting player's machine and CI must produce data without installing a profiler. |
| **Deterministic & CI-able** | A regression gate needs a repeatable number, not an interactive session. |
| **Localizes the cost** | "Slow" must resolve to a layer and a call, not just a frame total. |
| **Distributions, not means** | p95/p99/max expose the tail that is the actual complaint. |
| **Low observer effect** | The instrument must not add the time it measures (per-event `fprintf` does). |
| **Reuses existing infrastructure** | clock, `.fwv`, `DBG_Frame_Reset`, `PHASE`, HeMoM, `.RMR`, CTest. |
| **Interops with external profilers** | For deep dives, don't reinvent VTune/Tracy/perf — feed or coexist with them. |
| **Code-clutter cost** | Instrumentation that litters every call site has a real readability/maintenance price. |
| **Fidelity cost** | Does measuring (or fixing) touch reconstructed original code? |

### Explicitly excluded
- **A single absolute FPS SLA.** The budget is 55 ms/frame; targets are expressed as frame-time percentiles vs. that budget and vs. a reference baseline.
- **Chasing 60 fps.** Not the design target; matching modern refresh is not a goal (BRA-Platform-Input §1).
- **Field telemetry / opt-in performance uploads.** A real option long-term; out of scope now (privacy + infra).

---

## 4. The crux

**Crux 1 — you cannot fix what you cannot measure, and we have no repeatable number.** So *measure-and-guard first*: a deterministic baseline + a regression gate come **before** any optimization, and every perf change is gated on a measured delta. (Mirrors the input and combat-testing decisions: ship *observation* first.)

**Crux 2 — no single tool spans "always-on in CI" and "deep interactive dive."** In-house instrumentation is always available, deterministic, and CI-gateable, but shallow and adds code. External profilers (Tracy, VTune, Superluminal, perf, Callgrind) are deep and need no code, but need installation, aren't deterministic across runs (sampling), and can't gate a build. They are **complementary**: build the thin always-on layer for confirm/localize/guard, and *reach for* an external profiler for the deep dive once the layer says where to look.

---

## 5. Options considered

### Option A — Keep relying on playtest reports (status quo)
"It got slow" with no repro, no number, no attribution. **Rejected** — it is the problem statement.

### Option B — In-house instrumentation only (zones + frame-time sink + CI gate)
Formalize `PHASE` into a build-gated zone API, unify it and `PFL_Input_Metrics`/`DBG_Frame_Reset` on one `.fwv` sink with percentile reporting, add headless (HeMoM) and windowed (replay) perf tests with reference baselines in CTest.
- **Pros:** always available, deterministic, CI-gateable, reuses everything, no external dependency, works on the reporter's machine.
- **Cons:** shallow (only what you instrument); adds code; manual zone placement has blind spots.
- **Verdict:** the necessary backbone.

### Option C — External profilers only (Tracy / VTune / Superluminal / perf / Callgrind)
Adopt a profiler; no in-house code.
- **Pros:** deep, sample-based, low-friction for a session, rich visualizations (flame graphs, hardware counters).
- **Cons:** not always available (install/setup), not deterministic for a gate, can't fail a build, doesn't run on a player's machine or unattended CI, per-tool platform limits.
- **Verdict:** right for deep dives, wrong as the system of record.

### Option D — Hybrid: thin always-on in-house layer for confirm/localize/guard, external profilers for deep dives  ✅ **chosen**
Build Option B as the always-on, deterministic, CI-gated backbone; make it **profiler-friendly** (a deterministic headless workload external tools can attach to, optional Tracy zone emission behind a build flag) so a deep dive is one step away when the backbone localizes a hotspot.
- **Pros:** covers the whole span — always-on numbers + regression gate + a clean on-ramp to deep tooling; maximal reuse; no mandatory dependency.
- **Cons:** two things to keep coherent (in-house sink + optional profiler hooks); zone placement is still manual.
- **Verdict:** **Accepted.**

---

## 6. Decision

**Implement Option D.** Build a small in-house performance layer — *unified clock + build-gated zone primitive + one frame-time sink with percentile reporting + live readouts + deterministic headless & windowed perf tests + a CTest regression gate against reference baselines* — and keep it **interoperable with** external profilers for deep dives. **Measure and guard first; optimizations are separate, measurement-gated changes.**

Supporting decisions:
1. **Frame time in ms, percentiles not means, budget = 55 ms.** Every report is a p50/p95/p99/max distribution against the tick budget and a named-hardware baseline.
2. **Logic and render are measured and reported separately.** Headless HeMoM isolates logic; windowed replay measures render/present.
3. **`PHASE` graduates to a build-gated zone API, auto-stripped in Release.** No hand-toggled per-file `#define`s; one build option; zones feed the shared sink. The code-clutter cost is acknowledged and managed (see PRD §"Instrumentation ergonomics").
4. **The instrument must not perturb what it measures.** Timestamp arithmetic into a preallocated buffer; no I/O on the hot path; no string formatting in a zone body.
5. **External profilers are documented, not mandated.** A recommended tool per platform and a deterministic workload for them to chew on; none is a build dependency.
6. **Fixes remain fidelity-governed.** Measurement is additive; any change to reconstructed pacing code is a marked, justified divergence.

---

## 7. What this explicitly does not decide

- **The specific optimizations.** Chosen later, by what the baseline shows.
- **Absolute per-scenario time targets.** Set once baselines exist, relative to named hardware.
- **Adopting a specific external profiler as *the* tool.** Recommended, not standardized; the in-house layer is the system of record.
- **Field telemetry.** Deferred (privacy + infrastructure).
- **GPU-side profiling.** MoM is a 320×200 software-rendered blit; GPU time is negligible. Revisit only if an upscale/shader path lands (see [`PRD-Graphics-Upscale.md`] if present).
- **Multithreading the engine.** Out of scope; the game loop is single-threaded and stays so.
