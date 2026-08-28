# ADDENDUM — Well-Known Performance Mistakes & Our Status

**Parent:** [`BRA-Performance-Management.md`](BRA-Performance-Management.md) · [`PRD-Performance-Management.md`](PRD-Performance-Management.md) · [`GUIDE-Frame-Rate-Performance.md`](GUIDE-Frame-Rate-Performance.md)
**Date:** 2026-07-25

The classic ways performance work goes wrong, and where ReMoM stands on each. Status legend:

- ✅ **Addressed** — the design/discipline already prevents it.
- 🟡 **Partial** — guarded in principle, a concrete gap remains.
- ⚠️ **At risk** — a live trap in the current tree.
- 🔵 **Planned** — the system will address it; not yet built.

| # | Mistake | Why it bites | Our status | Notes |
|---|---------|--------------|------------|-------|
| 1 | **Reporting a mean, not a distribution** | A 55 ms average with a fat 300 ms tail reads "fine" while players feel every hitch | ✅ Addressed | Percentiles (p50/p95/p99/max) + count-over-budget are mandated in BRA/PRD; a bare mean is banned |
| 2 | **Profiling a Debug build** | Unoptimized code has a different hot-path shape; numbers don't transfer | ✅ Addressed | Guidance: profile Release-like builds; Debug only to validate the instrument itself |
| 3 | **Optimizing without measuring first** | Effort goes to a guessed cause; often the wrong layer | ✅ Addressed | "Measure-before-fix" is the BRA crux; every perf change gated on a measured delta |
| 4 | **Fixing render when logic is the sink (or vice-versa)** | The two are separate; a blended number misdirects | ✅ Addressed | Logic/render split is a first-class requirement; HeMoM isolates logic. *This exact mistake was avoided in the input work* — vsync was suspected, cursor cadence was the cause |
| 5 | **Observer effect: `fprintf`/format on the hot path** | The instrument adds the time it claims to measure | 🟡 Partial | `PFL_Input_Metrics` buffers in memory (good); but `PHASE` logs inside the timed region (DZ-4) and `CALL_TRACE` per-call logging exists (DZ-3). The zone migration + "timestamp-only bodies" rule closes it |
| 6 | **No deterministic repro** | Can't compare before/after or gate a build | ✅ Addressed | Fixed seed (`--seed1`), committed saves (`--load`), `.RMR` replay, HeMoM headless |
| 7 | **Chasing the wrong budget (60 fps / 16.7 ms)** | MoM's loop is a 55 ms DOS tick by design; 60 fps isn't the target | ✅ Addressed | Budget framed as 55 ms/frame everywhere; "don't chase 60" stated in BRA §1 and the GUIDE |
| 8 | **Measuring with tracing/logging on** | Turns a clean run into a self-inflicted slowdown that looks real | ⚠️ At risk | `TRACE`/`CALL_TRACE` on during a capture will dominate (DZ-3). Mitigation planned: perf header records the log level; gate asserts it off |
| 9 | **Instrumentation not stripped from Release** | Ships overhead to players; or worse, wrong-state commits | ⚠️ At risk | `PHASE` is toggled by hand-editing two `#define`s per file, unrelated to build type (DZ-4). The build-gated `PERF_INSTRUMENT` option + Release-symbol check (like the `dbg_prn` leak gate) fixes it |
| 10 | **Code clutter from instrumentation** | Per-call wrappers smeared across files wreck readability | 🟡 Partial | `PHASE` wraps every call (`/* CLAUDE */ PHASE(...)` columns). Fix: scope-based zones (bracket a stage once), coarse density, drill-in-then-remove (PRD §ergonomics) |
| 11 | **Micro-optimizing a cold path** | Time spent where the profile isn't | ✅ Addressed | "Localize first" — the reporter + zones point at the actual hot stage before anyone edits |
| 12 | **Single run / no warmup / benchmark noise** | One sample lies; first run pays cold-cache costs | ✅ Addressed (measured 2026-07-29) | **Confirmed empirically, and the obvious remedy was not enough.** Across five identical captures, run 1 was high on 6 of 9 qualifying zones (to 1.97×) — textbook cold-cache. But discarding it left the worst case unchanged at 2.46×, because that came from a *different* cause: `Diplomacy_Growth()` at n=11, where a median over 11 samples is movable by one slow call. Fix is both — `--merge` median-of-N **plus** a minimum-sample rule. See PRD FR14a |
| 13 | **Comparing numbers across machines** | GPU/CPU/OS variance swamps the signal | ✅ Addressed | Baselines are tagged to *named* reference hardware; cross-machine comparison declared invalid (BRA §6, PRD FR15) |
| 14 | **Heavy profiler perturbing the result (Heisenberg)** | Instrumentation-heavy tools change timing; sampling can miss short spikes | 🔵 Planned | In-house zones stay light; sampling profilers (VS/perf/Superluminal) cover blind spots; Callgrind used only for deterministic instruction-count profiling, not wall-clock |
| 15 | **No regression guard — a fix silently rots** | The next change quietly undoes the win | 🟡 Partial | `tests/check_perf_baseline.c` + CTest `Perf_Baseline_Compare` land the comparator: structure exact, 2.0× `p50` ceiling, median-of-N merge; verified to catch a 4× `p50` regression and an instance-count change. **Gap:** it compares two *committed* captures — producing a *fresh* capture still needs a display-gated Mode B run, which is unwired. Note the gate is NOT on percentiles as originally planned (PRD FR14a) |
| 16 | **Premature abstraction / threading for speed** | Complexity and bugs before a measured need | ✅ Addressed | Engine stays single-threaded (BRA §7); optimizations chosen from the baseline, not speculation |
| 17 | **Optimizing allocation/`fopen` blindly** | Guessing at malloc/IO without evidence | 🟡 Partial | Asset `fopen` stalls (DZ-6) are plausible but *unmeasured*; attribute with a zone before touching |

---

## The three that need action (not just discipline)

Everything above is either handled by design or handled by the planned system. Three are **live traps in the current tree** worth fixing early:

1. **#8 / DZ-3 — capturing with `CALL_TRACE`/`TRACE` on.** Any perf number taken this way is fiction. Cheap fix: the perf capture records and asserts the log level; document "captures run with tracing off."
2. **#9 / DZ-4 — `PHASE` isn't build-gated.** It's a hand-toggled per-file `#define`; a wrong-state commit ships timing code or a formatting cost into Release. The `PERF_INSTRUMENT` build option + Release-symbol check is the fix, and it's step 4 of the PLAN.
3. **#5 / DZ-4 — `PHASE` logs inside the timed region.** It inflates the stage times it reports. The zone migration (timestamp-only bodies, deferred reporting) fixes it.

The rest are "keep doing what the BRA/PRD already commit to."
