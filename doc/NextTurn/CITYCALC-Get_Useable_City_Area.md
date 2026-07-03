CITYCALC-Get_Useable_City_Area.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\Get_Useable_City_Area.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\Get_Useable_City_Area.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> All_Outpost_Population_Growth()
            |-> City_Food_Terrain()
                |-> Get_Useable_City_Area()

---

# `Get_Useable_City_Area` — Walkthrough

**Location:** [CITYCALC.c:1740-1797](../../MoM/src/CITYCALC.c#L1740-L1797) (~58 lines, ends [line 1797](../../MoM/src/CITYCALC.c#L1797)).
**WZD overlay:** ovr142, p08.
**Called from:** the city-catchment scanners — [`City_Food_Terrain`](../../MoM/src/CITYCALC.c#L1655), `City_Food_WildGame`, [`All_Outpost_Population_Growth`](../../MoM/src/CITYCALC.c#L3255), and other city-area food/production calcs.

> Verified against the **`.asm`** (117 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. Production is now 1:1 with the asm — the one divergence found during review (the east-edge wrap `>` vs `>=`) is fixed, and the Doxygen comment that mischaracterized it as "preserved historical behavior" has been corrected.

## Purpose

Builds the coordinate list of a city's **useable work area**: the tiles the city may draw food/production from. It walks the 5×5 footprint centered on `(city_wx, city_wy)` with the four corners trimmed (top and bottom rows narrowed from 5 to 3 wide), skips rows outside the world's vertical bounds, wraps X around the world's east/west edges, and **excludes corrupted tiles** (`MSF_CORRUPTION`). Accepted `(x, y)` pairs are written into `wx_array` / `wy_array` and the count is returned (max 21 for the trimmed-diamond shape; the arrays are `CITY_AREA_SIZE = 25`).

## Signature

```c
int16_t Get_Useable_City_Area(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t *wx_array, int16_t *wy_array)
```

## Code walk

Line refs are production [CITYCALC.c](../../MoM/src/CITYCALC.c); cross-checked against `Get_Useable_City_Area.asm`.

1. **Row loop** ([1752-1759](../../MoM/src/CITYCALC.c#L1752-L1759), asm:23-37) — `itr_city_area_squares` from `-2` to `+2`; `square_y = city_wy + itr`; skip the row unless `0 <= square_y < WORLD_HEIGHT` (no vertical wrap — off-map rows are dropped).
2. **Corner trim** ([1761-1771](../../MoM/src/CITYCALC.c#L1761-L1771), asm:40-48) — X offsets `-2..+2`, but the top/bottom rows (`itr == ±2`) narrow to `-1..+1`, giving the rounded-diamond city shape.
3. **Row base pointer** ([1772](../../MoM/src/CITYCALC.c#L1772), asm:50-63) — `terrain_flags_table_row = &_map_square_flags[city_wp * WORLD_SIZE + square_y * WORLD_WIDTH]`.
4. **Column loop + X wrap** ([1774-1792](../../MoM/src/CITYCALC.c#L1774-L1792), asm:64-101) — `square_x = city_wx + itr_world_x`; `< 0` → `+= WORLD_WIDTH` ([1777](../../MoM/src/CITYCALC.c#L1777), asm:72-74); `>= WORLD_WIDTH` → `-= WORLD_WIDTH` ([1781](../../MoM/src/CITYCALC.c#L1781), asm:76-78).
5. **Corruption filter + emit** ([1785-1790](../../MoM/src/CITYCALC.c#L1785-L1790), asm:80-95) — if `MSF_CORRUPTION` is clear, append `square_x`/`square_y` and `map_square_count++`.
6. **Return count** ([1796](../../MoM/src/CITYCALC.c#L1796), asm:110).

## Faithful (do not "fix")

- **East-edge wrap is `>= WORLD_WIDTH`** ([1781](../../MoM/src/CITYCALC.c#L1781)) — matches asm:76-78 (`cmp square_x, WORLD_WIDTH; jl skip; sub`), so `square_x == 60` (reachable for an east-edge city) correctly wraps to 0. (This was the reconstruction bug `>`, corrected this session.)
- **West-edge wrap is `< 0`** ([1777](../../MoM/src/CITYCALC.c#L1777), asm:72 `or si,si; jge`). Faithful.
- **Vertical bounds drop off-map rows (no wrap); X wraps both edges** ([1755-1759](../../MoM/src/CITYCALC.c#L1755-L1759) vs [1776-1784](../../MoM/src/CITYCALC.c#L1776-L1784)) — Y clamped by skipping (asm:30-37), X wrapped (asm:72-78). Faithful.
- **Corner-trimmed 5×5 shape** ([1761-1771](../../MoM/src/CITYCALC.c#L1761-L1771), asm:40-48) — the standard MoM city catchment. Faithful.

## ReMoM additions (not OG)

- `LOG_TRACE(...)` FN-ENTER / FN-EXIT ([1750](../../MoM/src/CITYCALC.c#L1750), [1795](../../MoM/src/CITYCALC.c#L1795)) — STU_LOG instrumentation.
- The Doxygen `/** … */` header block — ReMoM/Copilot-authored. Its former `@warning` (which framed the pre-fix `>` wrap as "preserved historical behavior") was **corrected this session** to an accurate `@note` describing the `< 0` / `>= WORLD_WIDTH` X-wrap.

## Related references

- `…\ovr142\Get_Useable_City_Area.asm` — IDA Pro 5.5 disassembly (ground truth, 117 lines).
- `…\ovr142\Get_Useable_City_Area.c` — Gemini translation of the asm (agrees with production, wrap `>=`).
- A separate **`Get_Useable_City_Area_NewGame`** exists in `MAGIC/ovr051` (new-game variant) — not this function.
- Callers: [`CITYCALC-City_Food_Terrain.md`](CITYCALC-City_Food_Terrain.md), [`CITYCALC-All_Outpost_Population_Growth.md`](CITYCALC-All_Outpost_Population_Growth.md); also `City_Food_WildGame` and city gold/production/mana area scans.
- Constants: `WORLD_WIDTH = 60`, `WORLD_HEIGHT = 40`, `WORLD_SIZE = 2400`, `CITY_AREA_SIZE = 25`, `MSF_CORRUPTION`.
