# BRA — Combat Testing Architecture Decision

**Status:** Decided
**Date:** 2026-07-16
**Author role:** Business Requirements Analyst (decision record)
**Related:** `PRD-Combat-Testing.md`, `PLAN-Combat-Testing.md`

This document records *how* the combat-testing approach was decided — the goal it serves, the criteria weighed, the options considered and rejected, and specifically the misstep of forcing tactical combat to run headless and how the goal exposed that as a poor fit. It is a decision record, not an implementation plan.

---

## 1. The ultimate goal

"Combat testing" is not one deliverable. Combat has **two natures**, and the goal is different for each:

- **Combat logic** — who wins, damage dealt, figures/units killed, experience gained, city damage, loot, fame, diplomacy changes. This is deterministic given (game state, RNG seed) and is *assertable*.
- **Combat presentation** — the tactical battle screen: terrain drawing, unit figures, movement and attack animations, palette cycling, sound, and the turn-by-turn input loop. This is *visual* and is verified by watching, not by asserting field values.

The goal of combat testing is therefore threefold:

1. **Detect regressions in combat outcomes** automatically, deterministically, on every build.
2. **Provide a harness to set up and observe a specific tactical battle** — to develop, debug, and visually verify combat behavior and its presentation.
3. **Localize which calculation changed** when an outcome regression fires.

The mistake this document exists to record: an early, unstated decision collapsed all three into a single mold — *headless, automated, assertion-based* — and then pushed tactical combat (which is fundamentally presentation) through it.

---

## 2. Criteria considered

| Criterion | Why it matters |
|---|---|
| **Determinism** | A test that varies run-to-run under a fixed seed is not a regression test. |
| **Automatability** | Can it run pass/fail in CI / pre-push without a human? |
| **Fidelity of substrate** | Does the code under test run on the substrate it was *designed* for, or is it being forced onto a foreign one? |
| **Reuse of existing infrastructure** | HeMoM fixtures, save-dump + `check_save_fields`, `.hms` scenarios, `.RMR` record-replay, `demo_vga` harness. |
| **Observability** | Can a developer *watch* what happened, not just read a diff? |
| **Coverage granularity** | Whole-battle vs. single-calculation isolation. |
| **Maintenance cost** | Seed-stream sensitivity; golden churn; brittleness to unrelated changes. |

### Criteria explicitly excluded

- **Pixel-exact rendering comparison.** Out of scope; presentation is verified by observation and (optionally) replay-log comparison, not framebuffer diffs.
- **Performance/timing of combat resolution.** Not a goal.
- **Exhaustive per-spell combat coverage** as an initial bar. Structural coverage first.

---

## 3. The substrate question (the crux)

The decisive criterion turned out to be **fidelity of substrate**: does the code run where it was designed to run?

- `Strategic_Combat__WIP` (auto-resolve) is **pure computation** — no battle screen. It was *designed* to run without rendering. Headless is its native substrate.
- `Combat_Screen__WIP` (tactical) is a **rendering, interactive screen** — animations, palette cycling, LBX graphic reloads, a per-frame draw loop, and an input loop. It was *designed* to run windowed against a real display. `demo_vga` is the existing precedent: a windowed harness that links the real `Platform` (not `Platform_Headless`) precisely because video effects are visual.

Forcing the tactical screen through `Platform_Headless` violated substrate fidelity. Every failure encountered while doing so was a symptom of running a renderer with no renderer, **not** a combat bug:

- empty-tileset LBX loader hang on shore/water terrain,
- `_getch()` blocking on fatal-exit under a non-interactive console,
- a palette-cycle / LBX-reload / `Apply_Palette` animation loop that never terminated,
- access-fault / breakpoint crashes from draw paths reading graphics state the headless stubs never initialized,
- **nondeterministic** hang-vs-crash under a *fixed* seed — the tell that uninitialized-under-headless memory, not logic, was driving behavior.

The one genuine combat finding surfaced during the detour was substrate-independent: `bua_Healing` in the AI turn is an unreconstructed stub (`STU_DEBUG_BREAK(); /* DNE in Dasm */`) — it would fire windowed too.

---

## 4. Options considered

### Option A — Everything headless (HeMoM + CTest), tactical included
Drive `Combat_Screen__WIP` under `Platform_Headless` with forced auto-combat so the AI plays both sides with no input.

- **Pros:** maximal reuse of HeMoM fixtures and assertion tooling; one harness; CI-friendly.
- **Cons:** wrong substrate for the tactical screen; fought animation/draw/palette machinery; nondeterministic; converts presentation bugs into test-harness bugs; grinding each symptom is open-ended reconstruction work.
- **Verdict:** **Rejected for tactical.** This was the initial (unstated) default and the error being recorded.

### Option B — Everything windowed (a `demo_vga`-style combat harness)
One rendering harness that loads a combat fixture and opens the real battle screen.

- **Pros:** correct substrate for tactical; animations/draw work; observable; `.RMR` replay gives deterministic input + RNG-log comparison.
- **Cons:** a rendering harness is a poor fit for *strategic* auto-resolve (no screen to show) and for fast pass/fail CI regression of pure-logic outcomes.
- **Verdict:** **Right for tactical, wrong as the sole model.**

### Option C — Split by nature (chosen)
Match each of combat's two natures to its native substrate, plus a math-isolation layer.

- **Pros:** each layer runs where the code was designed to run; strategic stays deterministic/CI-automated; tactical stays observable/renderable; kernels localize regressions.
- **Cons:** three harnesses instead of one; the tactical layer is observed/replay-based rather than simple pass/fail.
- **Verdict:** **Accepted.**

---

## 5. Decision — three layers

| Layer | Nature tested | Substrate | Harness | Determinism / mode | When run |
|---|---|---|---|---|---|
| **Strategic regression** | Combat *logic* (auto-resolve) | Headless | HeMoM `--combat` + CTest, dump + `check_save_fields` | Seeded, pass/fail | Every build / pre-push / CI |
| **Tactical harness** | Combat *presentation* + full battle | Windowed (real `Platform`) | `demo_vga`-style target opening `Combat_Screen__WIP` | Observed; `.RMR` replay + RNG-log compare for a regression mode | Development, debugging, visual verification, reproducing a reported battle |
| **Combat math kernels** | Individual calculations | None (unit test) | GTest with synthesized battle units | Seeded, pass/fail | Every build |

Guiding principle: **test code on the substrate it was designed for.** Strategic logic is screen-free → headless. The tactical screen renders → windowed. The attack/defense/damage math is pure → isolated unit tests.

---

## 6. What did and did not fit the goal

**Fit the goal:**

- Strategic auto-resolve headless (delivered as phase 1): deterministic, automated, screen-free — a clean fit for regression detection.
- A windowed tactical harness modeled on `demo_vga`: a clean fit for observing/debugging tactical battles and their presentation.
- GTest kernels: a clean fit for localizing which calculation changed.

**Did not fit the goal:**

- Tactical combat under `Platform_Headless`: failed the substrate-fidelity criterion. It produced nondeterministic hangs/crashes that are presentation-layer artifacts, not combat regressions — noise that would erode trust in the suite. Rejected.
- Treating "combat testing" as a single automated-assertion deliverable: conflated three distinct goals with three distinct substrates. Rejected.

---

## 7. Byproducts worth keeping (substrate-independent)

Surfaced during the headless-tactical detour; valuable regardless of the final architecture:

- **`_getch()` isatty guard** in the fatal-exit path — any fatal error now exits cleanly under a non-interactive console instead of hanging. General robustness fix.
- **SEH crash-address logging** — turns `EXCEPTION_UNKNOWN` into an actionable exception code + module RVA.
- **`bua_Healing` unreconstructed AI stub** — a real reconstruction gap (`STU_DEBUG_BREAK`, "DNE in Dasm") that any AI-vs-AI tactical battle will hit; belongs in the reconstruction backlog, not the test backlog.

---

## 8. Lesson recorded

When a premise is flagged as unproven ("tactical runs headless") and then proves false, the correct response is to **re-examine the premise**, not to grind through fixing each downstream symptom. The symptoms here were all substrate-mismatch artifacts; the goal — matching each nature of combat to its designed substrate — made the mismatch obvious in hindsight. The user's first-message cue to consider the graphics-demo harness alongside HeMoM was the signal that should have prompted the substrate split from the start.
