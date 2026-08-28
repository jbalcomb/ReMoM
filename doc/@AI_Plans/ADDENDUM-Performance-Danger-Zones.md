# ADDENDUM — Performance Danger Zones & Known Issues (code)

**Parent:** [`PRD-Performance-Management.md`](PRD-Performance-Management.md) · [`PLAN-Performance-Management.md`](PLAN-Performance-Management.md) · [`GUIDE-Frame-Rate-Performance.md`](GUIDE-Frame-Rate-Performance.md)
**Date:** 2026-07-25

Specific places in the current tree that either **cost frame time**, **can perturb a measurement**, or are **traps for a perf change**. Each entry: what it is, the risk, current status, and the fidelity note where the code is reconstructed (and therefore not freely editable).

> Reminder: the frame budget is **55 ms** (`PLATFORM_MILLISECONDS_PER_FRAME`). "Over budget" below means > 55 ms.

---

## DZ-1 — `Release_Time` busy-ish wait ([`MoX/src/Timer.c:76`](../../MoX/src/Timer.c#L76))

The frame-pacing wait spins `while (Platform_Get_Millies() < tick_end) { Platform_Pump_Events(); Platform_Sleep_Millies(1); }`. ~110 call sites.

- **Risk:** it is the loop's *slack absorber* — if logic finished under budget, time is burned here (fine); if logic **overran**, this contributes nothing but the overrun already blew the frame. It also pumps events + refreshes the cursor every ~1 ms, so a heavy `Pump_Events` multiplies across the wait. A `Platform_Sleep_Millies(1)` that oversleeps (scheduler granularity) can quantize waits upward.
- **Status:** already reworked once (`/* CLAUDE */`: the original double-loop busy-wait was replaced to stay input-responsive). Cursor cost is now negligible (hardware cursor). It is **the** place to measure "logic overran the tick."
- **Fidelity:** reconstructed OG code (`ui_delay_ticks_or_click`). Any further change is a marked, justified divergence — measure around it freely, edit it only with `/* CLAUDE */` + rationale.
- **Metric trap (important):** because `Release_Time` **holds without presenting**, a `Release_Time(N)` hold collapses into **one** long present-to-present `dt_ms` interval instead of **N** on-budget 55 ms ticks — so `dt_ms` conflates *intentional multi-tick waits* with *real overruns* and over-counts "over budget." The fix is to make the perf metric **tick-aware**: `Release_Time` reports its `N` so a hold reads as N idle on-budget ticks, and over-budget means a single tick's *work* > 55 ms (PRD FR5a, PLAN decision 7). Actually re-presenting each tick (Option B) is a separate, deferred, measured decision.

## DZ-2 — Multiple present sites per frame ([`sdl2_Video.c:138`/`143`](../../platform/sdl2/sdl2_Video.c#L138), `Platform_Video_Update` from [`sdl2_PFL.c:200`](../../platform/sdl2/sdl2_PFL.c#L200) & [`:405`](../../platform/sdl2/sdl2_PFL.c#L405))

`Platform_Video_Update()` (→ `SDL_RenderPresent` / `StretchDIBits`) is reachable from several call paths in one logical frame (main render, mouse-move redraw, font/screen paths).

- **Risk:** with vsync on, each `SDL_RenderPresent` blocks to vblank; 2–3 stacked presents/frame = 2–3× vblank stalls (the input BRA measured this — it was *not* the cursor's dominant cause, but it is real overhead). More presents also means more full-frame blits.
- **Status:** the software-cursor extra present is gone under the hardware cursor. Remaining multi-present paths (fonts, screen transitions) are unmeasured; the frame-time `.fwv` `reason` tag + present count is the tool to quantify.
- **Fidelity:** the `platform/` present code is substrate (not fidelity-governed) — free to coalesce if measured to help.

## DZ-3 — Call-trace logging on the hot path (`LOG_TRACE` `[FN-ENTER]/[FN-EXIT]`, `LOG_CAT_CALL_TRACE`)

Functions like `Release_Time` emit `LOG_TRACE` enter/exit lines; the `CALL_TRACE` category is per-call.

- **Risk:** if `CALL_TRACE` (or TRACE globally) is enabled, per-call logging in a hot loop is a **massive** perturbation and a self-inflicted slowdown — and it will *look like* a real slowdown in any measurement taken with it on. This is the classic observer-effect trap.
- **Status:** gated by severity/category, normally off. **Danger is measuring with it on.** Perf captures must run with CALL_TRACE/TRACE **off**; the perf gate should assert the log level.
- **Action:** the perf harness records the active log level in its `.fwv` header so a noisy capture is identifiable.

## DZ-4 — `PHASE` logs inside the timed region ([`NEXTTURN.c:681`](../../MoM/src/NEXTTURN.c#L681))

The current `PHASE` macro calls `LOG_INFO` *inside* the timing wrapper (after the call, but on the measured code's thread/timeline).

- **Risk:** `LOG_INFO` formatting + write per phase is exactly the "don't format/allocate in a zone body" anti-pattern; it inflates the very stage times it reports, and at high phase counts adds up.
- **Status:** tolerable today (coarse, per-turn, not per-frame), but it's the wart the zone migration fixes: zones must be **timestamp-into-buffer only**, reporting deferred to shutdown (PRD §ergonomics, PLAN §migration).

## DZ-5 — Per-frame palette LUT rebuild, Win32 ([`win_PFL.cpp` `Win_Convert_Engine_Pixels_To_Back_Buffer`](../../platform/win32/win_PFL.cpp))

The honest-palette fix builds a 256-entry `palette_lut` from `platform_palette_buffer` **every frame** before the 64 000-pixel blit loop.

- **Risk:** 256 composes/frame is trivial (~microseconds) next to the 64 000-pixel copy; **not** a real hotspot. Listed so nobody "optimizes" it prematurely or mistakes it for one — the pixel loop itself (64 000 × per-frame) is the larger cost if the blit ever shows up.
- **Status:** intentional; keep. If the blit is ever measured hot, cache the LUT and rebuild only on palette change (but that reintroduces the shared-`Fonts.c`-direct-write staleness problem the per-frame rebuild solves — see the palette-honesty history).

## DZ-6 — Asset loading (`fopen` per `.LBX` open, `STU_GRAF_Open_Asset`)

Asset opens walk the search path and `fopen` per request.

- **Risk:** not a steady-frame cost, but a **stall** source at screen transitions / worldgen / combat entry if assets are (re)opened mid-flow. Shows up as an isolated over-budget frame (a "hitch"), not sustained low fps.
- **Status:** unmeasured. The frame-time `.fwv` will surface it as a spike; attribute with a zone around the load.

## DZ-7 — The 55 ms coupling itself (`Release_Time` + logic)

The loop's responsiveness is *defined by* logic completing within the tick. There is no frame-drop/decouple mechanism: overrunning logic directly stretches wall-clock frames.

- **Risk:** late-game AI turns that exceed 55 ms cannot be "hidden" — they *are* the slowdown. The fix space (spread AI over frames, background it, optimize the algorithm) all touches reconstructed logic and pacing.
- **Status:** this is the core perf question the system exists to quantify. Measure first (Mode A headless logic timing), then decide.
- **Fidelity:** any decoupling/spreading is a deliberate divergence from the DOS single-tick model — marked, justified, and likely opt-in.

## DZ-8 — Measurement observer effect (the meta danger)

Every instrument added here can itself cost time: a `.fwv` flush, a zone body that formats, a live title-bar update that calls the OS.

- **Risk:** measuring *worse* than an un-instrumented build → the instrument is too heavy and the numbers are fiction.
- **Status:** mitigated by design (in-memory buffer, periodic flush, timestamp-only zones, ~1 Hz title update). The rule: **if the instrumented build is measurably slower than the un-instrumented one, the instrument is the bug.** Keep hot-path taps to timestamp arithmetic into a preallocated buffer.

---

### Quick triage map

| Symptom in the `.fwv` | Likely zone | Danger zone |
|---|---|---|
| Sustained `dt_ms` >> 55, `block_ms` ~0 | logic overran the tick | DZ-1, DZ-7 |
| `dt_ms` high, `block_ms` ~16×N | stacked vsync presents | DZ-2 |
| Isolated spikes at transitions | asset (re)load | DZ-6 |
| Everything slow only when TRACE/CALL_TRACE on | the log itself | DZ-3 |
| Instrumented slower than clean build | the instrument | DZ-8, DZ-4 |
