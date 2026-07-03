Terrain-Square_Food_x2.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr161\Map_Square_Food2.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr161\Map_Square_Food2.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> All_Outpost_Population_Growth()
            |-> City_Food_Terrain()
                |-> Square_Food_x2()

---

# `Square_Food_x2` — Walkthrough

**Location:** [Terrain.c:70-194](../../MoM/src/Terrain.c#L70-L194) (~124 lines after flattening, ends [line 194](../../MoM/src/Terrain.c#L194)).
**WZD overlay:** ovr161, p03 (drake178 IDA label: `TILE_GetFood`).
**On-disk OG name:** the disassembly/decompilation files are `Map_Square_Food2.asm` / `.c` — IDA naming decoupled from the renamed production symbol. (Also called `j_Map_Square_Food2` from the asm side.)
**Called from:** [`City_Food_Terrain`](../../MoM/src/CITYCALC.c#L1661) (and other terrain-food sums).

> Verified against the **`.asm`** (183 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.**

## Purpose

Returns one map square's food contribution to a city, in **quarter-food units** (i.e. real food × 4), halved when the tile is shared between two cities' work areas. The per-terrain base is a half-food table (grassland = 3 = 1.5 × 2), which is then doubled (`× 2`) and, for shared tiles, halved again.

Net return per tile:

| | value |
|---|---|
| non-shared | `real_food × 4` |
| shared (two cities) | `real_food × 2` |

Its caller `City_Food_Terrain` sums these and divides by 4, recovering **real food** from tiles the city fully owns and **half food** from shared tiles.

## Signature

```c
int16_t Square_Food_x2(int16_t wx, int16_t wy, int16_t wp)
```

## Per-terrain food table (half-food base → ×2 → return)

From the range/switch dispatch ([83-184](../../MoM/src/Terrain.c#L83-L184), asm:48-150). "base" = the value assigned before the `× 2`:

| Terrain | base | non-shared return | real food |
|---|---|---|---|
| Grasslands (1-4), BugGrass | 3 | 6 | 1.5 |
| Forest (1-3), Hills, Shore/Desert-shore variants | 1 | 2 | 0.5 |
| Sorcery Node, 4-way River, river/lake/shore-2 ranges | 4 | 8 | 2.0 |
| Nature Node | 5 | 10 | 2.5 |
| Mountain, Desert, Swamp, Tundra, Chaos Node, Volcano, Ocean, OceanAnim | 0 | 0 | 0 |

Verified value-for-value against the asm jump table `sw_terrtype_based_grass` (asm:107-150) and the surrounding range checks (asm:48-105).

> Production was flattened from the OG's deep if/else nest into a flat `if … else if(> threshold) … else switch` ladder — this matches the asm's linear jump structure more directly than the nest did.

## Code walk

Line refs are production [Terrain.c](../../MoM/src/Terrain.c); cross-checked against `Map_Square_Food2.asm`.

1. **Terrain type** ([74](../../MoM/src/Terrain.c#L74), asm:14-31) — `terrain_type = p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES`.
2. **Ocean guards** ([75-82](../../MoM/src/Terrain.c#L75-L82), asm:32-42) — `tt_Ocean (0)` and `>= tt_OceanAnim` `return 0` directly.
3. **Range ladder** ([83-118](../../MoM/src/Terrain.c#L83-L118), asm:48-105) — descending `> threshold` tests (`_River1111_5`, `_Shore111R1110`, … `tt_Forest3`) set the half-food `base`; the `> _1Hills2` and `> tt_Rivers_end` ranges `return 0` directly.
4. **Switch** ([119-184](../../MoM/src/Terrain.c#L119-L184), asm:107-150) — grass/forest/node table (+ a `default: 0` matching the asm's `SI=0` fallback).
5. **`× 2`** ([186](../../MoM/src/Terrain.c#L186), asm:151-153 `add ax, ax`) — `food_units *= 2` (half-food → quarter-food).
6. **Shared-tile halving** ([188-190](../../MoM/src/Terrain.c#L188-L190), asm:156-169) — if `City_Area_Square_Is_Shared`, `food_units /= 2` (signed round-toward-zero).
7. **Return** ([193](../../MoM/src/Terrain.c#L193), asm:171).

## OG quirks preserved (faithful — do not "fix")

- **Swamp yields 0, not the manual's ½** ([147-152](../../MoM/src/Terrain.c#L147-L152)) — base 0. Preserved `OGBUG  manual says 1/2 food for Swamp` note.
- **Nature Node = 5** ([165](../../MoM/src/Terrain.c#L165)) — the in-game (WIZARDS/ovr161) value; the new-game version (`Square_Food2_NewGame`, MAGIC/ovr051) uses 4. Preserved with the `OGBUG  MGC NewGame version has 4` note.
- **Signed `/2` for shared tiles** ([190](../../MoM/src/Terrain.c#L190), asm:164-168 `cwd; sub; sar 1`) — round-toward-zero. Faithful.

## Two paths to zero (both match the asm)

`Square_Food_x2` returns 0 by two different routes, and production reproduces each faithfully after the flatten:

- **Range-0** (ocean, `>= tt_OceanAnim`, `> _1Hills2`, `> tt_Rivers_end`) — `return 0` **directly** ([77](../../MoM/src/Terrain.c#L77)/[81](../../MoM/src/Terrain.c#L81)/[105](../../MoM/src/Terrain.c#L105)/[113](../../MoM/src/Terrain.c#L113)), skipping `× 2` and the shared check. Matches the asm's `jmp @@JmpJmpDone_Return_Zero` (asm:42/82/96). *(Before the flatten these fell through to the shared call — the flatten corrected that.)*
- **Switch-0** (Mountain, Desert, Swamp, Tundra, Chaos Node, Volcano, `default`) — set `food_units = 0` and **fall through** to `× 2` (0) and the `City_Area_Square_Is_Shared` call (0 either way). Matches the asm's `sw_17 → IDK_switch_deault` path (asm:144-169), which likewise runs the shared check.

## Related references

- `…\ovr161\Map_Square_Food2.asm` — IDA Pro 5.5 disassembly (ground truth, 183 lines). On-disk name `Map_Square_Food2`.
- `…\ovr161\Map_Square_Food2.c` — Gemini translation of the asm (matches production).
- `MAGIC/ovr051/Square_Food2_NewGame.asm` — the new-game-setup variant (Nature Node = 4, not 5).
- [`CITYCALC-City_Food_Terrain.md`](CITYCALC-City_Food_Terrain.md) — the caller; sums `Square_Food_x2` and `/ 4` to recover real catchment food. `City_Area_Square_Is_Shared` — separate review.
- Constants: `NUM_TERRAIN_TYPES`, terrain-type ids (`tt_Ocean`, `tt_OceanAnim`, `tt_Grasslands1..4`, `tt_Forest1..3`, `tt_SorceryNode`, `tt_NatureNode`, `tt_Hills1`, etc.), `ST_TRUE`.
