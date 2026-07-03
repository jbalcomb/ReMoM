CITYCALC-City_Food_Terrain.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\City_Food_Terrain.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\City_Food_Terrain.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> All_Outpost_Population_Growth()
            |-> City_Food_Terrain()

---

# `City_Food_Terrain` — Walkthrough

**Location:** [CITYCALC.c:1643-1670](../../MoM/src/CITYCALC.c#L1643-L1670) (~28 lines, ends [line 1670](../../MoM/src/CITYCALC.c#L1670)).
**WZD overlay:** ovr142, p06 (drake178 IDA label: `CTY_GetTileFood`).
**Called from:** [`All_Outpost_Population_Growth`](../../MoM/src/CITYCALC.c#L3244) (outpost growth), and city-size/farming calcs at [CITYCALC.c:1860](../../MoM/src/CITYCALC.c#L1860), [1975](../../MoM/src/CITYCALC.c#L1975), [3146](../../MoM/src/CITYCALC.c#L3146).

> Verified against the **`.asm`** (98 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.**

## Purpose

Returns the terrain-derived food yield of a city's work area. It walks the useable city-area squares (from `Get_Useable_City_Area`, which already drops corrupted tiles), sums each square's `Square_Food_x2` value, applies a `+50%` bonus if the city has Gaia's Blessing, and returns the total divided by 4.

Note the function correctly indexes the coordinate arrays by the **inner loop var** `itr` — the same pattern its caller `All_Outpost_Population_Growth` gets *wrong* (that one indexes by the outer city counter — a preserved OGBUG). Here the asm and production agree on `itr`.

The preserved header comment carries two DEDU-style questions (`why the / 4 on food2_units?`, `food, food2, pop?`) — open notes, not bugs.

## Signature

```c
int16_t City_Food_Terrain(int16_t city_idx)
```

## Code walk

Line refs are production [CITYCALC.c](../../MoM/src/CITYCALC.c); cross-checked against `City_Food_Terrain.asm`.

1. **Useable city area** ([1652-1655](../../MoM/src/CITYCALC.c#L1652-L1655), asm:16-49) — `city_wp = _CITIES[city_idx].wp`; `useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, wx_array, wy_array)` (corruption-filtered).
2. **Sum `Square_Food_x2`** ([1657-1662](../../MoM/src/CITYCALC.c#L1657-L1662), asm:50-72) — `food2_units += Square_Food_x2(wx_array[itr], wy_array[itr], city_wp)` over `itr` in `[0, useable_map_squares)`. (asm calls the same routine under its OG name `j_Map_Square_Food2`.)
3. **Gaia's Blessing +50%** ([1664-1667](../../MoM/src/CITYCALC.c#L1664-L1667), asm:73-86) — if present, `food2_units = (food2_units * 3) / 2` (signed round-toward-zero, matching asm `imul 3; cwd; sub; sar 1`).
4. **Return `food2_units / 4`** ([1669](../../MoM/src/CITYCALC.c#L1669), asm:88-91) — signed divide. See "The `/ 4`" below.

## The `/ 4` (not a bug — quarter-food `Square_Food_x2` → real food)

The header's `why the / 4 on food2_units?` DEDU has an answer. `Square_Food_x2` returns per-tile food in **quarter-food units** (`real_food × 4`) for a non-shared tile, and `× 2` for a tile shared between two cities: its switch assigns a half-food base (grassland `3` = 1.5 × 2), then doubles it (`× 2`, [Terrain.c:186](../../MoM/src/Terrain.c#L186)) and, for shared tiles, halves it again ([Terrain.c:188-190](../../MoM/src/Terrain.c#L188-L190)). So per tile the sum receives `food × 4` (owned) or `food × 2` (shared). The `/ 4` here converts quarter-food back to whole food:

```
City_Food_Terrain = Σ Square_Food_x2(tile) / 4
   owned tile   →  (food × 4) / 4 = food        (full food)
   shared tile  →  (food × 2) / 4 = food / 2    (half food)
```

So the net is **real catchment food** — full food from tiles this city fully owns, half food from tiles shared with a neighbor. That result seeds `maximum_size` (base **max city size**, before Granary/Farmers'-Market bonuses) at [CITYCALC.c:1975](../../MoM/src/CITYCALC.c#L1975), and is the terrain component of outpost growth. Magnitude checks out (a ~21-tile grassland site ≈ 21 × 1.5 ≈ 31 food ≈ max size ~25 capped). Only this path uses `Square_Food_x2`'s scaled representation, which is why the sibling food functions (`City_Food_WildGame`, terrain gold/mana) carry no `/ 4`. See [`Terrain-Square_Food_x2.md`](Terrain-Square_Food_x2.md) for the full scaling.

## Faithful notes (do not "fix")

- **Array indexing by `itr`** ([1661](../../MoM/src/CITYCALC.c#L1661)) — the asm indexes `wx_array`/`wy_array` with `_SI_itr_useable_map_squares` = the loop counter (asm:56-65). Correct here (contrast the `[itr_cities]` OGBUG in `All_Outpost_Population_Growth`).
- **Gaia's Blessing test** ([1664](../../MoM/src/CITYCALC.c#L1664)) — production writes `!= ST_FALSE`; the asm tests `> 0` (`cmp …, e_ST_FALSE; jle @@Done`, asm:78-79). Equivalent for the non-negative enchantment field. Faithful.
- **`/ 4` and `* 3 / 2` in 16-bit** ([1666](../../MoM/src/CITYCALC.c#L1666), [1669](../../MoM/src/CITYCALC.c#L1669)) — the asm's `imul 3` product is used as 16-bit before the `sar`, and the final `idiv 4` is 16-bit; production's `int` arithmetic matches. The `/ 4` is explained above (quarter-food `Square_Food_x2` → real catchment food); the header `food/food2/pop?` DEDU is a naming note, not a bug.

## Related references

- `…\ovr142\City_Food_Terrain.asm` — IDA Pro 5.5 disassembly (ground truth, 98 lines).
- `…\ovr142\City_Food_Terrain.c` — Gemini translation of the asm (matches production).
- [`CITYCALC-All_Outpost_Population_Growth.md`](CITYCALC-All_Outpost_Population_Growth.md) — the Wave-9A caller. Also called by city max-size / farming-threshold calcs (siblings: `City_Food_WildGame` at [1674](../../MoM/src/CITYCALC.c#L1674)).
- `Get_Useable_City_Area` ([1752](../../MoM/src/CITYCALC.c#L1752)), `Square_Food_x2` — separate reviews.
- Constants: `CITY_AREA_SIZE = 25`, `GAIAS_BLESSING`, `ST_FALSE = 0`.
