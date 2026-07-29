# SPEC — On-Screen Performance Readout (PRD FR10)

**Status:** Spec / research write-up
**Date:** 2026-07-29
**Parent:** [`PRD-Performance-Management.md`](PRD-Performance-Management.md) FR10 · [`PLAN-Performance-Management.md`](PLAN-Performance-Management.md) step 3
**Existing reference (read these first, this doc does not duplicate them):** [`MoX-Fonts.md`](../MoX-Fonts.md), [`MoX-Print.md`](../MoX-Print.md), [`MoM-ScreenLayouts.md`](../MoM-ScreenLayouts.md), [`MoX-Debug-Fonts.md`](../MoX-Debug-Fonts.md), [`MoM-MainScr-Draw.md`](../MoM-MainScr-Draw.md)

---

## 0. Correction to the GUIDE's pointer

[`GUIDE-Frame-Rate-Performance.md`](GUIDE-Frame-Rate-Performance.md) §2b says *"There's already a debug summary window — `DBG_Main_Screen_Draw_Summary_Window` ([`STU_DBG.h:114`](../../STU/src/STU_DBG.h#L114))."* That symbol is a **dead flag** — a tree-wide grep finds only its definition and its `extern`, no reader and no writer:

```
STU/src/STU_DBG.c:106:  int DBG_Main_Screen_Draw_Summary_Window = 0;
STU/src/STU_DBG.h:114:  extern int DBG_Main_Screen_Draw_Summary_Window;
```

**The code that actually draws debug text on the Main Screen is [`Main_Screen_Draw_Debug_Information()`](../../MoM/src/MainScr.c#L6900) (`MainScr.c:6900`).** That is the adopter for FR10.

Also, "window" here means a **drawn panel region of the 320×200 framebuffer**, per [`MoM-ScreenLayouts.md`](../MoM-ScreenLayouts.md) ("Summary Window AND Unit Window", background at `240,76`). It does not mean an OS window or a windowing system. Any statement that FR10 requires "building the surface" is wrong — the surface is the framebuffer and the text API already exists.

---

## 1. How the existing debug overlay works

| aspect | detail |
|---|---|
| Draw function | `Main_Screen_Draw_Debug_Information()` — [`MainScr.c:6900`](../../MoM/src/MainScr.c#L6900) |
| Call site | [`MainScr.c:3570`](../../MoM/src/MainScr.c#L3570), last in the Main Screen draw, after `Main_Screen_Draw_Movement_Bar()` |
| Gate | `if(DBG_debug_flag)` — so it draws **every frame** while enabled |
| Toggle | **`Z`** hotkey on the Main Screen — [`MainScr.c:1110`](../../MoM/src/MainScr.c#L1110) (`DBG_debug_flag = !DBG_debug_flag`). Also toggled in Combat ([`Combat.c:2543`](../../MoM/src/Combat.c#L2543)) and the Main Menu ([`MainMenu.c:585`](../../MoM/src/MainMenu.c#L585)) |
| Definition | `uint8_t DBG_debug_flag = ST_FALSE;` — [`MOM_DBG.c:76`](../../MoM/src/MOM_DBG.c#L76) |

Its preamble sets the text style once, then prints rows:

```c
Set_Outline_Color(0);
Set_Font_Style_Shadow_Down(1, 0, 0, 0);   /* style slot 1, shadow-down */
Set_Alias_Color(8);

pos = 0;
Print(2, (22 + (8 * pos)), "Map X,Y");
Print_Integer(45, (22 + (8 * pos)), _map_x);
Print_Integer(57, (22 + (8 * pos)), _map_y);
pos++;
```

**Conventions to match:** labels at `x = 2`; values at fixed `x` (45/57, 40/56, 52/64, 46 depending on label width); rows at `y = 22 + 8*pos`, i.e. **8 px line pitch starting at y = 22**; `pos++` per row. Current content is ~8 rows (`Map X,Y`, `Grid X,Y`, `MD X,Y`, `MD MX,MY`, `MD WX,WY`, `UNIT IDX/WX/WY`), so it occupies roughly **y = 22 … 86**.

---

## 2. The print/font API, as needed here

Full surface is in [`Fonts.h`](../../MoX/src/Fonts.h); the useful subset:

| call | use |
|---|---|
| `int16_t Print(x, y, char *s)` | draw a string; **returns the next x**, so calls can be chained without measuring |
| `int16_t Print_Integer(x, y, int16_t v)` | draw an integer |
| `int16_t Print_Long(x, y, int32_t v)` | 32-bit |
| `Print_Right(x, y, s)` / `Print_Integer_Right(x, y, v)` | right-align at `x` — the natural choice for a numeric column |
| `Print_Centered(x, y, s)` | centre on `x` |
| `Clipped_Print*(...)` | same, clipped to the current work area. Per [`MoX-Print.md`](../MoX-Print.md) the clipped family does **not** support aliasing, full/justified text, or the text control codes — do not assume parity |
| `Set_Font_Style_Shadow_Down(slot, c1, c2, c3)` | style slot + shadow; what the debug overlay already uses |
| `Set_Outline_Color(n)` / `Set_Alias_Color(n)` | outline and anti-alias colour |
| `Set_Font_LF(n)` / `Set_Font_Spacing(n)` | line-feed and inter-character spacing |

The font is **variable width** — the source comments estimate per-glyph widths (`"M a p   X , Y" ~ 6+5+5+4+6+3+6 = 35`), so ≈ **5 px average**, ≈ 4 px for digits. Do not compute layout from a fixed character cell; either use `Print`'s return value or right-align.

There is no `snprintf`-free float printer. `%.1f` formatting has to happen in C before the call.

---

## 3. Screen-space budget

320 × 200. Relevant regions:

| region | extent | note |
|---|---|---|
| Top bar | `y 0 … 19` | `field_y = screen_y - 20` in the debug code confirms the map starts at y = 20 |
| Map viewport | `x 0 … 239`, `y 20 … ~180` | 20×18 px squares (`SQUARE_WIDTH`/`SQUARE_HEIGHT`) |
| Right panel | `x 240 … 319` | Summary/Unit Window background at `240,76` |
| Unit action buttons | `246,176` Done · `280,176` Patrol · `246,186` Wait | bottom-right, do not overlap |
| Existing debug overlay | `x 2 … ~70`, `y 22 … 86` | drawn over the map |

**The concern about fit is justified but the readout is small.** Six figures (fps, p50, p95, p99, max, over-budget) at one per row would be 6 × 8 = 48 px of column — pushing the overlay to y ≈ 134 and covering a third of the map. Packing them into **three rows of ≈ 20 characters (≈ 100 px wide)** costs 24 px and fits the existing left column with room to spare.

---

## 4. Proposed layout

Continue the existing `pos` sequence so the block sits under the current rows and cannot collide with them:

```
FPS 17.8            <- rows continue at x=2, y = 22 + 8*pos
MS 55/111/166
MAX 1280  OVR 121
```

| row | content | source |
|---|---|---|
| 1 | `FPS` + rolling rate, 1 decimal | `Perf_Live_Get_Stats(&fps, …)` |
| 2 | `MS` + `p50/p95/p99` frame interval | same call |
| 3 | `MAX` + worst, `OVR` + count over the 55 ms budget | same call |

Data comes from `Perf_Live_Get_Stats(fps, p50, p95, p99, max, over_budget)` in [`Platform_Perf.h`](../../platform/include/Platform_Perf.h), already implemented and returning non-zero once samples exist. Draw nothing when it returns 0.

**Alternative placement** if the left column proves too cluttered: right-align into the panel at `x = 316` using `Print_Right`/`Print_Integer_Right`, rows `y = 22 … 46`. Keeps the map clear, but overlaps the Summary Window background when that is drawn — needs checking against `MoM-MainScr-UnitWindow.md` before choosing.

---

## 5. The one real hazard: this draws every frame

`Main_Screen_Draw_Debug_Information()` runs **once per frame** while `DBG_debug_flag` is set. A performance readout that formats its own strings every frame would add `snprintf` to the hot path — the exact observer effect the rest of this system is built to avoid, and it would inflate the very frame time it displays.

**Requirement:** the readout must not format per frame.

`Perf_Live_Note_Present()` already recomputes only about once per second. FR10 should follow the same cadence — cache the three formatted strings when the 1 Hz window rolls over, and have the draw function `Print` the cached buffers. Per frame that is three `Print` calls and no formatting, no percentile sort, no allocation.

Note `Perf_Live_Get_Stats()` **sorts a 256-entry ring on every call** — cheap at 1 Hz, wasteful at 18 fps. Calling it directly from the draw path would be the mistake. Either call it only on the 1 Hz tick, or add a cached-string accessor alongside it.

---

## 6. Work required

1. Add a cached-string accessor to the perf layer (e.g. `Perf_Live_Get_Display_Lines(const char **l1, **l2, **l3)`), populated on the existing 1 Hz rollover in `PFL_Perf.c`.
2. In `Main_Screen_Draw_Debug_Information()`, after the existing rows, `Print` the three lines at `x = 2`, `y = 22 + 8*pos`, continuing `pos`.
3. Verify legibility over dark terrain — the overlay already relies on `Set_Font_Style_Shadow_Down` + `Set_Outline_Color(0)`; reuse, do not restyle.
4. Manual verification only. Headless never presents, so no automated test can exercise this; it needs a windowed run with `Z` pressed on the Main Screen.

Estimated: small — one accessor plus one block of `Print` calls. The research above was the actual cost, not the code.
