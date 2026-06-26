MAPGEN-Movement_Mode_Cost_Maps.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Movement_Mode_Cost_Maps.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Movement_Mode_Cost_Maps.c

Init_New_Game()
    |-> Movement_Mode_Cost_Maps(ARCANUS_PLANE);
    |-> Movement_Mode_Cost_Maps(MYRROR_PLANE);

---

# `Movement_Mode_Cost_Maps` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Movement_Mode_Cost_Maps` | [MAPGEN.c:6716-6761](../../MoM/src/MAPGEN.c#L6716-L6761) | Precomputes the per-square movement cost (in half-move "moves2" units) for each of the six movement modes on a plane, from the terrain stats table, with a road override. |

## Purpose

Builds `movement_mode_cost_maps[wp]` — six parallel 60×40 cost maps (one per
movement mode) used by the pathfinders. Each square's cost per mode is looked up
from `terrain_stats_table` by terrain type; road squares are then overridden
(cheap/free land movement, sailing disabled).

**RNG-neutral: zero `Random()` calls** ([6716-6761](../../MoM/src/MAPGEN.c#L6716-L6761)). Does not affect new-game `Random()` alignment.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:609-610](../../MoM/src/MAPGEN.c#L609-L610) | Called once per plane; followed by `gd_dump_movement_mode_cost_maps("26_…")` ([611](../../MoM/src/MAPGEN.c#L611)). |

## How it works

1. **`EMMDATAH_Map()`** ([6722](../../MoM/src/MAPGEN.c#L6722)) — maps the EMM bank holding the cost-map data (asm `EMM_Map4_EMMDATAH`).
2. **Per-square loop** ([6723-6759](../../MoM/src/MAPGEN.c#L6723-L6759)) — over `itr_wy < WORLD_HEIGHT` × `itr_wx < WORLD_WIDTH`:
   - `terrain_type_idx = p_world_map[wp][itr_wy][itr_wx]` ([6727](../../MoM/src/MAPGEN.c#L6727)).
   - Write each mode's `moves2[itr_wy*WORLD_WIDTH + itr_wx]` from `terrain_stats_table[terrain_type_idx*6 + col]` ([6728-6733](../../MoM/src/MAPGEN.c#L6728-L6733)). The table is `[terrain_type][6]`; the column→mode map is:

     | col | mode | submap |
     |---|---|---|
     | 0 | Unused move type | `UU_MvMd` |
     | 1 | Walking | `walking` |
     | 2 | Forester | `forester` |
     | 3 | Mountaineer | `mountaineer` |
     | 4 | Swimming | `swimming` |
     | 5 | Sailing | `sailing` |

   - **Road override** ([6735-6757](../../MoM/src/MAPGEN.c#L6735-L6757)) — if the square has `MSF_ROAD`:
     - **Myrror** (`wp != ARCANUS_PLANE`): the five land modes (walking, UU_MvMd, forester, mountaineer, swimming) = **0** (Enchanted Roads → free movement).
     - **Arcanus** (else): those five = **1**.
     - **Sailing = `ST_UNDEFINED`** on the road square, both planes ([6756](../../MoM/src/MAPGEN.c#L6756)).

## Verification against the asm

Production is 1:1 with the disassembly; the differences are all name aliases.

| Item | OG asm | Production |
| --- | --- | --- |
| EMM map call | `call EMM_Map4_EMMDATAH` | `EMMDATAH_Map()` ✓ |
| Loops | `itr_wy < WORLD_HEIGHT`, `itr_wx < WORLD_WIDTH` (`jge` exit) | same ✓ |
| Terrain read | `p_world_map[wp][itr_wy][itr_wx]` (word-strided) | same ✓ |
| Cost writes | `terrain_stats_table[idx*6 + {1,0,2,3,4,5}]` → `{Walking, Unused_MoveType, Forester, Mountaineer, Swimming, Sailing}.HalfMPCost` (index folded into `bx`) | same six writes, same offsets, into `.{walking,UU_MvMd,forester,mountaineer,swimming,sailing}.moves2[…]` ✓ |
| Submap order | `Unused_MoveType`(0), `Walking`(0x960), `Forester`, `Mountaineer`, `Swimming`, `Sailing` | `s_MOVE_MODE_COST_MAPS` same order ([MovePath.h](../../MoM/src/MovePath.h)) ✓ |
| Road test | `test [_map_square_flags…], TF_Road / jnz` | `(MAP_SQUARE_FLAG(...) & MSF_ROAD) != 0` (`TF_Road` = `MSF_ROAD` = 0x08) ✓ |
| Road branch | `wp==0 → 1` (Arcanus, `loc_4C935`), else `0` (Myrror, `loc_4C9D6`); 5 land modes | `wp != ARCANUS → 0`, else `1` ✓ |
| Sailing on road | `Sailing.HalfMPCost = e_ST_UNDEFINED_DB` | `sailing.moves2[…] = ST_UNDEFINED` (both = −1) ✓ |

`moves2[]` is the asm's `.HalfMPCost` (IDA's name for the submap's byte array, `s_MOVE_COST_MAP.moves2[WORLD_SIZE]`); the square index `itr_wy*WORLD_WIDTH + itr_wx` is folded into `bx` in the asm. No reconstruction errors.

## Sub-functions / external calls

- **`EMMDATAH_Map`** — maps the EMM page for the cost-map data (asm `EMM_Map4_EMMDATAH`); used across the AI/mapgen code.
- **`terrain_stats_table`** ([MGC_DATA.h:9](../../MoM/src/MGC_DATA.h#L9)) — `SAMB_ptr` byte table, `[terrain_type][6]` half-MP costs.
- **`MAP_SQUARE_FLAG`** / `MSF_ROAD` — road detection.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Movement_Mode_Cost_Maps.asm` — IDA Pro 5.5 disassembly (authority); six `terrain_stats_table` writes (`es:si+0..+5`), road branch (`loc_4C935` Arcanus=1 / `loc_4C9D6` Myrror=0), sailing `e_ST_UNDEFINED_DB`.
- [MovePath.h](../../MoM/src/MovePath.h) — `s_MOVE_MODE_COST_MAPS` (6 submaps) / `s_MOVE_COST_MAP.moves2[WORLD_SIZE]`.
- [MAPGEN.c:609-611](../../MoM/src/MAPGEN.c#L609-L611) — call sites + dump.
- [MAPGEN-Generate_Roads.md](MAPGEN-Generate_Roads.md) — sets the `MSF_ROAD` flags this function reads.
