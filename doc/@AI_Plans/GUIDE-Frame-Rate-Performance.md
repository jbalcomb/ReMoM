# GUIDE — Frame-Rate Slowdowns: Confirm, Localize, Test

**Status:** Reference / playbook
**Date:** 2026-07-25
**Related:** [`BRA-Platform-Input.md`](BRA-Platform-Input.md), [`PRD-Platform-Input.md`](PRD-Platform-Input.md), [`PLAN-Platform-Input.md`](PLAN-Platform-Input.md) — the same measure-before-fix discipline and much of the same harness apply here.

This is the playbook for when players report "it got slow." It covers how to **confirm** a slowdown is real, how to **localize** where the frame time goes, and a recommendation to stand up **real performance testing** so "feels slow" becomes a pass/fail number. Most of the tools below already exist — the work is mostly connecting and surfacing them, not building new subsystems.

---

## 0. The mental model (read this first)

Two things about MoM's timing that change how you reason about "frame rate":

- **The logical frame rate is ~18 fps by design, not 60.** The game loop is paced to the DOS BIOS timer tick — `PLATFORM_MILLISECONDS_PER_FRAME = 55` ms ([`Platform.h:84`](../../platform/include/Platform.h#L84)), i.e. **~18.2 fps**. That is the *budget*: one logical frame should complete in ≤55 ms. A "slowdown" means frames are **exceeding** 55 ms (the loop can't keep the tick) or a specific screen/operation blows far past it. Do **not** chase 60 fps — that's not the target, and matching it isn't a fidelity goal (see BRA-Platform-Input §1).
- **Two different times hide inside "slow," measure them apart:**
  - **Logic/update time** — AI, city calculations, turn processing, pathfinding. CPU-bound, no rendering. This is the classic MoM late-game sink.
  - **Render/present time** — drawing the screen and getting it to the glass (`Platform_Video_Update` → blit/present).
  A change can improve one and worsen the other; a single "fps" number conflates them. (Same split as latency-vs-jitter in the input work.)
- **Cursor smoothness is no longer a frame-rate proxy.** The hardware cursor (`REMOM_HW_CURSOR`, now default-on) tracks natively and is decoupled from the framebuffer present. So a smooth cursor over a slow game is now possible — judge frame rate by the *content* updating, not the pointer.

**Two principles borrowed from the input work, that govern everything below:**
1. **Measure before you fix.** "Feels laggy" is unreproducible, un-regressable, un-attributable. Get a number and a repro first.
2. **Report distributions, not means.** A 55 ms average with a fat tail of 300 ms hitches *is* the stutter players feel; a mean hides it. Always look at **p50 / p95 / p99 / max**.

---

## 1. Confirm it's real — with what already exists (zero new code)

Before writing anything, reproduce and quantify using tools already in the tree.

### 1a. The input-metrics `.fwv` already records per-frame time

The Platform-Input Layer 1 module ([`platform/metrics/PFL_Input_Metrics.c`](../../platform/metrics/PFL_Input_Metrics.c)) writes one row **per present**, including `dt_ms` — the inter-present interval, i.e. **frame time**. It's runtime-gated and off by default:

```sh
# Windows (cmd):  set REMOM_INPUT_METRICS=1 && ReMoM.exe
# bash/Linux:     REMOM_INPUT_METRICS=1 ./ReMoM
```

This drops `remom_input_metrics.fwv` next to the exe. The `dt_ms` column is your frame-time timeline; compute p50/p95/p99/max over the slow section. (This is exactly how the cursor baseline was taken — see PLAN-Platform-Input "Layer 1 result".) `block_ms` also tells you how much of a frame was a vsync stall vs. real work.

### 1b. `DBG_Frame_Reset` already summarizes frames

In Debug builds, [`DBG_Frame_Reset`](../../platform/sdl2/sdl2_PFL.c) already logs every 60 frames: `frametime`, `handler_calls`, `max_delta` (worst input-tick gap), `avg_delta`, event counts. It's behind `STU_DEBUG` at TRACE level today. It's a ready-made per-frame counter to extend (see §3c).

### 1c. Pin the repro

The scarce thing is a **reproducible** slow case. From the complaint, capture: *which screen* (main map, city, combat, spellbook, Next-Turn), *when* (early vs. late game — map size, city count, unit count), and ideally a **save file + seed** so it re-runs identically (`--load <save>` / `--seed1 N`, `--continue`). A repro is what turns the rest of this doc from guesswork into measurement.

---

## 2. Surface it live — at-a-glance readouts (your three ideas)

Once confirmed, make the number visible so you and reporters get instant feedback.

### 2a. FPS in the window title bar  ✅ good idea, cheap

The window title is set **once** today and there is **no runtime setter** ([`win_PFL.cpp:67`](../../platform/win32/win_PFL.cpp#L67), [`sdl2_PFL.c:54`](../../platform/sdl2/sdl2_PFL.c#L54)). Add a small platform entry point:

```c
void Platform_Set_Window_Title(const char * title);   /* SetWindowTextA / SDL_SetWindowTitle */
```

Then, ~once per second from the main loop, format `"(ReMoM) … — 17.8 fps (worst 41 ms)"` and set it. Rolling average **plus worst frame in the window** — the worst-frame number is what exposes hitches the average smooths over. Lowest-effort, highest-immediacy signal; fine to ship (it's just a title string).

### 2b. FPS on the Debug Main Screen  ✅ good, Debug-only

There's already a debug summary window — `DBG_Main_Screen_Draw_Summary_Window` ([`STU_DBG.h:114`](../../STU/src/STU_DBG.h#L114)). Draw the frame-time stats there (p50/p95/p99/max, and later the per-stage breakdown from §4). Debug build only, so it can be richer than the title bar and cost nothing in release.

### 2c. Periodic summary log  ✅ good — extend `DBG_Frame_Reset`

Instead of the current every-60-frames TRACE line, emit a **time-based** summary (every ~2 s) at `LOG_INFO`: frame-time p50/p95/p99/max over the interval, plus present-block time. Keep a small ring of recent `dt_ms` to compute percentiles. Always-on in Debug; gate behind the metrics flag in Release so it never costs an un-instrumented player anything. This gives a scrollback timeline you can diff across runs.

---

## 3. Localize it — sub-frame breakdown (the PHASE idea)  ✅ yes, generalize it

Your instinct to reuse the `PHASE` pattern is exactly right. The macro already exists in [`NEXTTURN.c:681`](../../MoM/src/NEXTTURN.c#L681):

```c
#define PHASE(CALL) do { uint64_t _ps = Platform_Get_Millies(); CALL; \
    uint64_t _pe = Platform_Get_Millies(); \
    LOG_INFO(LOG_CAT_NEXTTURN, "[NEXTTURN] phase %-48s = %llu ms", #CALL, (unsigned long long)(_pe - _ps)); } while(0)
// ...toggled to a pass-through when off:
#define PHASE(CALL) CALL
```

It's a **compile-time toggle** — instrumented or a bare pass-through — so it costs **nothing** in a normal build. `Next_Turn_Proc` is already carved into `PHASE(All_City_Calculations())`, `PHASE(AI_Next_Turn())`, etc., so a slow turn already attributes itself to a stage.

**Generalize the same pattern to the per-frame path.** Wrap the main loop's stages so a slow *frame* attributes itself:
- input pump (`Platform_Event_Handler` / `Platform_Pump_Events`),
- the active screen's update/handler (main-map, city, combat, spellbook…),
- render + present (`Page_Flip` → `Platform_Video_Update` → blit).

When one stage dominates, PHASE *its* sub-calls to drill in (a slow city screen → which draw; a slow combat frame → which animation). This is the localization layer — the frame-time analogue of what Layer 1 does for input.

**Known suspects to instrument first** (where MoM historically spends frame time):
- **`Next_Turn_Proc` / AI turn** — the classic late-game sink; already `PHASE`-wrapped, so start here.
- **Combat** — move/attack animations, per-figure redraws.
- **City screen** — full redraws on interaction.
- **Spellbook / research animations** ([`Spellbook.c`](../../MoM/src/Spellbook.c), the file you're in).
- **Worldgen** — one-time but a visible stall at new-game.

---

## 4. The recommendation — yes, this is the moment for real performance testing

You're right that this is the point to get serious. "Feels slow" is exactly the unmeasurable, unregressable complaint the input work already had to solve — so **reuse that discipline and most of that harness** rather than inventing a new one. The BRA/PRD/PLAN-Platform-Input trio is the template; this is the same shape of problem one layer up.

**What you already have to build on:** deterministic RNG seed (`--seed1`), `.RMR` record/replay, the **HeMoM headless deterministic runner** ([`src/HeMoM.c`](../../src/HeMoM.c)), the `PFL_Input_Metrics` frame-time `.fwv`, the `PHASE` macro, and CTest fixtures. Very little is missing.

**Stand up two complementary test modes:**

### Mode A — Headless logic perf (the backbone)
Run a **deterministic scenario** (fixed seed or a committed late-game save) headless in HeMoM to a fixed point — e.g. *N* turns of a large, populated map — and time the logic stages with `PHASE` / `Platform_Get_Millies`. **No rendering**, so it isolates pure CPU/logic cost, is fully deterministic, and runs in CI without a display. Because the dominant MoM slowdown is AI/turn logic, this is where most of the signal is, and HeMoM can already drive it. Output: per-stage ms for `Next_Turn`, `AI_Next_Turn`, `All_City_Calculations`, etc.

### Mode B — Windowed frame-time perf (the render half)
Drive a deterministic `.RMR` replay through the real windowed backend with the metrics `.fwv` on, and assert `dt_ms` p95/max against a committed baseline. This is **literally Layer 2 of the Platform-Input plan, generalized** from cursor position to frame time — same replay+capture+analyze+threshold chain, same "windowed, runs where a display exists" constraint.

### The regression gate
Commit a baseline on **named reference hardware**; a CTest test asserts "**not measurably worse than baseline**" using relative thresholds on **percentiles** (p95/p99/max), and fails the build on regression — the same model as the combat-testing and input-metrics gates. Natural targets:
- **Absolute budget** for the windowed loop: **55 ms/frame** is the built-in SLA — flag frames over budget.
- **Relative** for headless logic: "turn *N* on the reference save takes ≤ *X* ms," X set from the baseline, ratcheted down as it's optimized.

This converts every future perf complaint (and every optimization) into a number and a pass/fail, instead of another round of "does this feel better to you?"

---

## 5. Build-out order (tracer bullets)

1. **Confirm (zero code).** Get a repro (save+seed). Capture `REMOM_INPUT_METRICS=1` `.fwv`; read `dt_ms` p50/p95/p99/max. Decide it's real and roughly where.
2. **FPS in the title bar.** `Platform_Set_Window_Title`, rolling avg + worst-frame, ~1 Hz. Immediate feedback for you and reporters.
3. **Periodic summary log + Debug-screen FPS.** Extend `DBG_Frame_Reset` to a time-based percentile summary; draw it on `DBG_Main_Screen_Draw_Summary_Window`.
4. **PHASE-ize the per-frame path.** Add frame-stage `PHASE` wrappers; localize the guilty stage on the repro; drill into its sub-calls.
5. **Real perf tests.** Mode A (headless logic) first — biggest signal, CI-able, deterministic — then Mode B (windowed frame-time). Commit reference baselines; wire the CTest regression gate.

Steps 1–4 are hours-to-days and mostly wiring existing pieces; step 5 is the durable investment.

---

## 6. Reference (clickable)

| Thing | Where |
|---|---|
| Frame budget (55 ms / ~18 fps) | [`Platform.h:84`](../../platform/include/Platform.h#L84) |
| `PHASE` timing macro (the template) | [`NEXTTURN.c:681`](../../MoM/src/NEXTTURN.c#L681) |
| Per-frame frame-time recorder (`.fwv`) | [`PFL_Input_Metrics.c`](../../platform/metrics/PFL_Input_Metrics.c), gate `REMOM_INPUT_METRICS=1` |
| Existing per-frame counter/summary | `DBG_Frame_Reset`, [`sdl2_PFL.c`](../../platform/sdl2/sdl2_PFL.c) |
| Debug summary window | `DBG_Main_Screen_Draw_Summary_Window` [`STU_DBG.h:114`](../../STU/src/STU_DBG.h#L114) |
| Window titles (need a runtime setter) | [`win_PFL.cpp:67`](../../platform/win32/win_PFL.cpp#L67), [`sdl2_PFL.c:54`](../../platform/sdl2/sdl2_PFL.c#L54) |
| Headless deterministic runner | [`src/HeMoM.c`](../../src/HeMoM.c), `--seed1`, `--load`, `--continue` |
| Measurement discipline (reuse it) | [`BRA-Platform-Input.md`](BRA-Platform-Input.md) §1, §6 |

**The one-liner to remember:** MoM's frame budget is **55 ms**; confirm slowdowns with the existing `REMOM_INPUT_METRICS` `.fwv` (`dt_ms`, percentiles not means); localize with the `PHASE` macro; and make it a pass/fail with a deterministic HeMoM-headless (logic) + replay-capture (render) perf harness gated in CTest — the same play the input work already ran.
