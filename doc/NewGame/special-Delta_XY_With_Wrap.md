special-Delta_XY_With_Wrap.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg023\Delta_XY_With_Wrap.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg023\Delta_XY_With_Wrap.c

MoO2 module: `special`
1oom: (no direct counterpart — closest sibling is `util_math_line_plot`)

---

# `Delta_XY_With_Wrap` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Delta_XY_With_Wrap` | [special.c:134-184](../../MoX/src/special.c#L134-L184) | Chebyshev (king-move) distance between two map squares with **X-axis wrap**. Returns `max( min(direct_dx, wrap_dx), abs_dy )`. Y is not wrapped. |

Verified faithful to the disassembly `Delta_XY_With_Wrap.asm` throughout (structure 1:1, no RNG calls, no side effects).

## Purpose

The overland map is a **cylinder, not a torus**: it wraps east-west (X) but not north-south (Y). `Delta_XY_With_Wrap` is the distance helper that respects that — it computes the shorter of the two possible X-distances (direct vs through the wrap seam) and takes the max with the (unwrapped) Y-distance.

This is a pure Chebyshev metric — the longer of the two axes, with no diagonal weighting. Its sibling in the same file, `Range()` ([special.c:78-119](../../MoX/src/special.c#L78-L119)), adds a `+ smaller/2` term for a 1.5×-diagonal-cost approximation **and does not wrap**. The two are not interchangeable: switching one for the other changes both wrap behavior **and** the diagonal weighting. See [BUGBUG sites](#bugbug-sites-existing-code-that-should-call-this-but-doesnt) below.

## Signature

```c
int16_t Delta_XY_With_Wrap(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t wrap_x);
```

Every production caller passes `WORLD_WIDTH` (= `60`, [MOX_DEF.h:734](../../MoX/src/MOX_DEF.h#L734)) for `wrap_x`. The parameter exists in the OG signature so it stays parameterized.

## Algorithm

Given two points `(x1, y1)`, `(x2, y2)` and a wrap width `wrap_x`:

1. `delta_x1 = abs(x2 - x1)` — direct X distance.
2. `delta_y = abs(y2 - y1)` — Y distance (no wrap).
3. Compute the **wrap-around** X distance:
   - If `x2 < x1`: `delta_x2 = abs((x2 + wrap_x) - x1)` (forward wrap: continue east past the seam).
   - Else: `delta_x2 = abs((x2 - wrap_x) - x1)` (backward wrap: walk west through the seam).
4. `delta_x_final = min(delta_x1, delta_x2)` — pick the shorter X path.
5. `range = max(delta_x_final, delta_y)` — Chebyshev distance.

### Worked examples on a 60-wide map

| `x1` | `x2` | direct `delta_x1` | wrap `delta_x2` | min |
|---:|---:|---:|---:|---:|
| 1 | 59 | 58 | `\|59 − 60 − 1\| = 2` | **2** |
| 59 | 1 | 58 | `\|1 + 60 − 59\| = 2` | **2** |
| 0 | 30 | 30 | `\|30 − 60 − 0\| = 30` | **30** |
| 30 | 30 | 0 | `\|30 − 60 − 30\| = 60` | **0** |

For `x2 == x1`, the `else` branch computes `delta_x2 = wrap_x` — never the smaller value, so the `min` correctly returns `0`.

Full two-point distance (`range = max(min(delta_x_direct, delta_x_wrap), delta_y)`):

| `(x1, y1)` | `(x2, y2)` | `min(delta_x_direct, delta_x_wrap)` | `delta_y` | `range` |
|---|---|---:|---:|---:|
| `(19, 5)` | `(16, 2)` | `min(3, 57) = 3` | `3` | **`3`** |

`x2 = 16 < x1 = 19`, so the wrap branch picks `\|16 + 60 − 19\| = 57`. Direct X-delta is 3, so `min = 3`. Y is unwrapped: `\|2 − 5\| = 3`. Chebyshev: `max(3, 3) = 3`. In `Generate_Towers`' node-distance test (`< 4`, [MAPGEN.c:789](../../MoM/src/MAPGEN.c#L789)) this candidate is **rejected** — a node at `(16, 2)` would block placement at `(19, 5)`.

## How it's reached

Used everywhere distance-on-the-overland matters. Selected call sites (full list 30+ across MAPGEN / AIMOVE / AIDATA / UNITSTK / Outpost / Combat):

| Caller | Site | Use |
|---|---|---|
| `Init_New_Game` map-gen helpers | [MAPGEN.c:778](../../MoM/src/MAPGEN.c#L778), [789](../../MoM/src/MAPGEN.c#L789), [952](../../MoM/src/MAPGEN.c#L952), [975](../../MoM/src/MAPGEN.c#L975), [1021](../../MoM/src/MAPGEN.c#L1021), [2141](../../MoM/src/MAPGEN.c#L2141), [2199](../../MoM/src/MAPGEN.c#L2199), [2664](../../MoM/src/MAPGEN.c#L2664), [2703](../../MoM/src/MAPGEN.c#L2703), [5119](../../MoM/src/MAPGEN.c#L5119) | Site-spacing tests (towers, nodes, fortresses, lairs, cities). |
| AI strategic eval | [AIMOVE.c:721](../../MoM/src/AIMOVE.c#L721), [747](../../MoM/src/AIMOVE.c#L747), [894](../../MoM/src/AIMOVE.c#L894), [1196](../../MoM/src/AIMOVE.c#L1196), [1477](../../MoM/src/AIMOVE.c#L1477), [1695](../../MoM/src/AIMOVE.c#L1695), [2616](../../MoM/src/AIMOVE.c#L2616), [3001](../../MoM/src/AIMOVE.c#L3001), [4029](../../MoM/src/AIMOVE.c#L4029), [4519](../../MoM/src/AIMOVE.c#L4519), [4568](../../MoM/src/AIMOVE.c#L4568), [4988](../../MoM/src/AIMOVE.c#L4988), [5522](../../MoM/src/AIMOVE.c#L5522), [5588](../../MoM/src/AIMOVE.c#L5588), [5859](../../MoM/src/AIMOVE.c#L5859), [6560](../../MoM/src/AIMOVE.c#L6560), [6618](../../MoM/src/AIMOVE.c#L6618), [6683](../../MoM/src/AIMOVE.c#L6683), [6745](../../MoM/src/AIMOVE.c#L6745), [7273](../../MoM/src/AIMOVE.c#L7273) | Distance scoring (fortress/dock/ferry/centroid). |
| AI target-value | [AIDATA.c:649](../../MoM/src/AIDATA.c#L649), [656](../../MoM/src/AIDATA.c#L656), [693](../../MoM/src/AIDATA.c#L693) | `10 - troop_count - distance` formula. |
| Unit selection | [UNITSTK.c:704](../../MoM/src/UNITSTK.c#L704), [851](../../MoM/src/UNITSTK.c#L851) | Distance from active square. |
| Combat | [Combat.c:8623](../../MoM/src/Combat.c#L8623) | Distance from `Selected_Unit_*` to a battle unit. |
| Outpost | [Outpost.c:772](../../MoM/src/Outpost.c#L772) | Distance to nearest city. |

## Structure

```mermaid
flowchart TD
    Entry(["Delta_XY_With_Wrap(x1,y1,x2,y2,wrap_x)"])
    DX1["delta_x1 = abs(x2 - x1)"]
    DY["delta_y = abs(y2 - y1)"]
    WrapPick{"x2 &lt; x1?"}
    DX2a["delta_x2 = abs((x2 + wrap_x) - x1)"]
    DX2b["delta_x2 = abs((x2 - wrap_x) - x1)"]
    Min["delta_x1 = min(delta_x1, delta_x2)"]
    Max{"delta_x1 &gt; delta_y?"}
    RetDX["range = delta_x1"]
    RetDY["range = delta_y"]
    Done(["return range"])

    Entry --> DX1 --> DY --> WrapPick
    WrapPick -->|yes| DX2a --> Min
    WrapPick -->|no|  DX2b --> Min
    Min --> Max
    Max -->|yes| RetDX --> Done
    Max -->|no|  RetDY --> Done
```

## Code walk

Line refs are production [special.c](../../MoX/src/special.c); cross-checked against `Delta_XY_With_Wrap.asm` (the authority). No RNG calls.

### Phase 1 — Direct deltas + abs ([141-153](../../MoX/src/special.c#L141-L153))

```c
delta_x1 = x2 - x1;
delta_y  = y2 - y1;
if(delta_x1 < 0) { delta_x1 *= -1; }
if(delta_y  < 0) { delta_y  *= -1; }
```

Maps 1:1 onto asm `loc_1E055` (subtract, store DX/CX) and the two `or reg,reg; jge skip; neg ax; mov reg, ax` abs idioms (asm lines 27-38). The `*= -1` shape vs `= -(…)` is cosmetic — both compile to the same `neg` instruction.

### Phase 2 — Wrap-around delta_x ([155-167](../../MoX/src/special.c#L155-L167))

```c
if(x2 < x1) { delta_x2 = x2 + wrap_x - x1; }
else        { delta_x2 = x2 - wrap_x - x1; }
if(delta_x2 < 0) { delta_x2 *= -1; }
```

Maps onto asm `loc_1E075` (cmp DI, SI; jge), branch arms at `loc_1E080`/the fallthrough, joining at `loc_1E085` for the final `sub ax, SI`. The third abs follows the same `or reg,reg; jge; neg` pattern (asm lines 52-56).

The branch direction matters for *which* expression the compiler emits, but **after the abs both branches produce the same |x2 ± wrap_x − x1| magnitude**. The split exists in the asm; production preserves it.

### Phase 3 — min of the two X-distances ([169-172](../../MoX/src/special.c#L169-L172))

```c
if(delta_x1 > delta_x2) { delta_x1 = delta_x2; }
```

Maps onto asm `loc_1E093` (`cmp DX, BX; jle skip; mov DX, BX`).

### Phase 4 — max with delta_y ([174-181](../../MoX/src/special.c#L174-L181))

```c
if(delta_x1 > delta_y) { range = delta_x1; }
else                   { range = delta_y;  }
return range;
```

Maps onto asm `loc_1E099` (`cmp DX, CX; jle loc_1E0A1`), the two branches loading either DX or CX into AX before the epilogue.

## Sub-functions / external calls

None. Pure arithmetic. No RNG, no globals read or written, no I/O.

## OG quirks preserved (faithful — do not "fix")

- **`delta_x_final = min(delta_x1, delta_x2)` rather than `min` of the *signed* values.** Both candidate X-distances are taken `abs` before the min, so the function is symmetric in `(x1, x2)` and never returns a negative. Faithful to the asm `or reg,reg; jge; neg` idioms.
- **`int16_t` arithmetic.** All math is `int16_t`. With `wrap_x = 60` and map-tile coordinates in `[0, 60)`, the intermediate `x2 + wrap_x - x1` worst case is ~120 and `x2 - wrap_x - x1` worst case is ~-120 — both well inside `int16_t` range. If a caller ever passed a wrap_x near `INT16_MAX/2`, the intermediate could overflow. **No production caller does**; every site passes `WORLD_WIDTH = 60`. Faithful — the asm uses 16-bit registers throughout.
- **No bounds check on `x1`/`x2` vs `[0, wrap_x)`.** Out-of-range coordinates produce defined math (still `abs`-ed) but a semantically meaningless distance. Callers are trusted to pass valid square coordinates. Faithful.

## BUGBUG sites (existing code that should call this but doesn't)

Two sites in MAPGEN.c are flagged in-source as misuses of the unwrapped `Range()` where `Delta_XY_With_Wrap()` is the correct helper:

- [MAPGEN.c:999](../../MoM/src/MAPGEN.c#L999) — `Range(wx, wy, _TOWERS[bldg_idx].wx, _TOWERS[bldg_idx].wy)` in a min-distance test against towers.
  ```c
  if(Range(...) < minimum_site_distance)  // BUGBUG  should be Delta_XY_With_Wrap()
  ```
- [MAPGEN.c:5609](../../MoM/src/MAPGEN.c#L5609) — `Range(src_wx, src_wy, dst_wx, dst_wy)` in a `>= 11` test.
  ```c
  (Range(...) >= 11)  // BUGBUG  should used Delta_XY_With_Wrap()
  ```

These are real bugs — at map-X edges, `Range()` reports a far-away site as far when it's actually adjacent through the wrap. **Don't fix them in the reconstruction**: the OG behavior is the misuse, and per repo policy OG bugs are preserved with the `BUGBUG` flag rather than corrected.

Note also that swapping `Range()` → `Delta_XY_With_Wrap()` is not a behavior-equivalent edit even ignoring the wrap: `Range()` returns `max + min/2`, `Delta_XY_With_Wrap()` returns plain `max`. The "should-be" call would tighten *both* the threshold semantics and the wrap behavior at the same time.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg023\Delta_XY_With_Wrap.asm` — IDA Pro 5.5 disassembly (the authority).
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg023\Delta_XY_With_Wrap.c` — Piethawn IDA→C translation (the in-file `#if 0 __GEMINI` copy at [special.c:185-256](../../MoX/src/special.c#L185-L256) is the same body, also faithful to the asm).
- [`Range()`](../../MoX/src/special.c#L78-L119) — sibling unwrapped distance with `+ smaller/2` diagonal weighting (the other common distance helper in `special.c`).
- `MOX_DEF.h` — `WORLD_WIDTH = 60` (the value every caller passes for `wrap_x`).
