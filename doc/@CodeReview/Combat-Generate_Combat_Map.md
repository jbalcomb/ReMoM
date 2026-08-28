Combat-Generate_Combat_Map.md

SEEALSO:  Combat-Combat.md
SEEALSO:  Combat-Combat_Screen_Map_Draw.md
SEEALSO:  Combat-Combat_Figure_Compose.md
SEEALSO:  Combat-Generate_Combat_Map.md
SEEALSO:  Combat-End_Of_Combat.md
SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoM-CombatScreen-Grid.md

CMB_TreeGen()       ==>  Scatter_Tree_Scenery()
CMB_RockGen()       ==>  Scatter_Rock_Scenery()
CMB_RoadGen()       ==>  Build_Road_Network()
CMB_RiverGen()      ==>  Carve_River_Terrain()
CMB_MergeDirt()     ==>  Merge_Dirt_Patches()
CMB_RemoveRough()   ==>  Remove_Rough_Terrain()
CMB_TileGen__WIP()  ==>  Set_Terrain_Tile_Types()
Patch_Terrain_Group()  ==>  Scatter_Terrain_Patches()

MoveDir_NextX_0     ==>  step_delta_cgx
MoveDir_NextY_0     ==>  step_delta_cgy
Rock_IMG_Segs       ==>  rock_pict_segs
Rock_DrawXs         ==>  rock_sx_array
Rock_DrawYs         ==>  rock_sy_array

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Generate_Combat_Map.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Patch_Terrain_Group.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\CMB_RiverGen.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\CMB_RoadGen.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\CMB_RemoveRough.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\CMB_MergeDirt.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\CMB_TileGen__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Set_Movement_Cost_Maps.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\CMB_TreeGen.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\CMB_RockGen.asm

Combat_Screen()
    |-> Allocate_Combat_Base_Blocks()
    |-> Combat_Screen_Load_Resources()
    |-> Cache_Graphics_Combat()
    |-> Build_Battlefield()
        |-> Load_Combat_Terrain_Pictures()
        |-> Generate_Combat_Map()
            |-> Scatter_Terrain_Patches()
            |-> Carve_River_Terrain()
            |-> Build_Road_Network()
            |-> Remove_Rough_Terrain()
            |-> Merge_Dirt_Patches()
            |-> Set_Terrain_Tile_Types()
            |-> Set_Movement_Cost_Maps()
            |-> Scatter_Tree_Scenery()
            |-> Scatter_Rock_Scenery()
        |-> Combat_Compose_Background()

---


# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-08).** No findings open. Ten functions, 3,887 asm lines, every one walked against its listing.

`Generate_Combat_Map` is the battlefield generator: it clears the wall grid, picks the central structure and scatters houses from the location type, restores the RNG seed, patches rough and dirt terrain, then hands off to eight subfunctions for rivers, roads, tiling, movement costs, trees and rocks.

At the first pass six of those eight were `// TODO` stubs. All are now written and renamed. Three rounds of findings — five from the subfunction recheck, four from the full walk of `Generate_Combat_Map`, plus the `clt_City` duplication from the first pass — have all been applied and re-verified against the listings.

Ten functions, 3,887 asm lines. Every one has now had a body walk.

## Renames

| IDA / previous | production |
| --- | --- |
| `CMB_TreeGen` | `Scatter_Tree_Scenery` |
| `CMB_RockGen` | `Scatter_Rock_Scenery` |
| `CMB_RoadGen` | `Build_Road_Network` |
| `CMB_RiverGen` | `Carve_River_Terrain` |
| `CMB_MergeDirt` | `Merge_Dirt_Patches` |
| `CMB_RemoveRough` | `Remove_Rough_Terrain` |
| `CMB_TileGen__WIP` | `Set_Terrain_Tile_Types` |
| `Patch_Terrain_Group` | `Scatter_Terrain_Patches` |
| `MoveDir_NextX_0` / `_Y_0` | `step_delta_cgx` / `step_delta_cgy` (proposed) |
| `CTG_DeepWater` / `CTG_IDK` | `ctg_Ocean` / `ctg_NIU_5` |

## Scope

The Production column carries the current name and line; the ASM column keeps the IDA names, which never change.

| Function (production) | Production | ASM (ground truth) | Lines | Coverage |
| --- | --- | --- | --- | --- |
| `Generate_Combat_Map` | [Combat.c:20015](../../MoM/src/Combat.c#L20015) | `ovr154/Generate_Combat_Map.asm` | 907 | **full body walk — R1-R4** |
| `Set_Terrain_Tile_Types` | [Combat.c:21255](../../MoM/src/Combat.c#L21255) | `ovr154/CMB_TileGen__WIP.asm` | 815 | **full body walk — faithful** |
| `Build_Road_Network` | [Combat.c:20743](../../MoM/src/Combat.c#L20743) | `ovr154/CMB_RoadGen.asm` | 759 | frame + walk logic + phase split |
| `Scatter_Tree_Scenery` | [Combat.c:20422](../../MoM/src/Combat.c#L20422) | `ovr154/CMB_TreeGen.asm` | 452 | frame + seed/scatter + guards |
| `Set_Movement_Cost_Maps` | [Combat.c:21696](../../MoM/src/Combat.c#L21696) | `ovr154/Set_Movement_Cost_Maps.asm` | 408 | **full body walk — faithful** |
| `Scatter_Rock_Scenery` | [Combat.c:20622](../../MoM/src/Combat.c#L20622) | `ovr154/CMB_RockGen.asm` | 341 | **full body walk — faithful** |
| `Carve_River_Terrain` | [Combat.c:21053](../../MoM/src/Combat.c#L21053) | `ovr154/CMB_RiverGen.asm` | 210 | frame + walk 1 + both tails |
| `Remove_Rough_Terrain` | [Combat.c:21190](../../MoM/src/Combat.c#L21190) | `ovr154/CMB_RemoveRough.asm` | 181 | both nests, one gap at asm:75-95 |
| `Scatter_Terrain_Patches` | [Combat.c:21823](../../MoM/src/Combat.c#L21823) | `ovr154/Patch_Terrain_Group.asm` | 124 | **full body walk — faithful** |
| `Merge_Dirt_Patches` | [Combat.c:21153](../../MoM/src/Combat.c#L21153) | `ovr154/CMB_MergeDirt.asm` | 98 | **full body walk — faithful** |

`Generate_Combat_Map` is the sole caller of all eight subfunctions — verified by grepping `call` across every listing in the overlay. Each is called once, except `Scatter_Terrain_Patches`, which is called twice.

---

# Findings

None open.

---

# Verified faithful

## `Set_Terrain_Tile_Types` — full body walk, no defects

Converts the abstract `terrain_group` map into concrete `terrain_type` tile indices: 10 group reads in, 26 tile writes out. Every block checked against the listing.

| block | asm |
| --- | --- |
| prologue — five tile-base indices zeroed | 25-29 |
| 3x3 neighbourhood sample, numpad layout | 35-122 |
| perimeter clamp to Grass (4 clamps) | 123-146 |
| Grass arm — 15 dirt-edge tiles + grass fallback | 147-438 |
| River block — 4 clamps to River, then 6 shape tiles | 443-638 |
| Dirt / Ocean / `ctg_NIU_5` arms | 639-695 |
| Rough arm — 16-way autotile | 697-796 |

The details that would have caught a transcription slip all hold. The **two separate perimeter-clamp blocks** are both present — one forcing off-grid neighbours to Grass before the Grass arm, a second forcing them to River inside the River block — each in the same cgy=0, cgy=21, cgx=0, cgx=20 order with the same neighbour triples. The **15 dirt-edge tiles** come out in identical sequence (27, 11, 29, 21, 23, 21, 15, 17, 11, 15, 9, 7, 25, 19, 13), including 21, 11 and 15 each appearing at two different sites. The **six river shapes** map correctly onto their neighbour pairs, and the **16 autotile variants** match in exact order (4, 10, 13, 12, 11, 1, 14, 5, 8, 7, 3, 2, 9, 0, 6, 15) with the same N-E-S-W nesting.

Two things not to "tidy": the River block re-reads `terrain_group` from memory (asm:443) instead of using the already-sampled centre, and production reproduces that; and the parameter the caller pushes (asm:863-867) is never read by the callee, which the `(void)` signature plus the `OGBUG` note at [Combat.c:21842](../../MoM/src/Combat.c#L21842) records.

## `Scatter_Terrain_Patches` — full body walk, no defects

Frame order matches slot for slot. Both loops signed, the clamps match `SETMAX`/`SETMIN` against asm:81-95, and `COMBAT_GRID_WIDTH` 21 / `COMBAT_GRID_HEIGHT` 22 line up with the `Random` bounds.

Two details worth leaving alone: the two direction lookups index differently on purpose — seeds use `step_delta_cgx[Random(4)]` with the raw 1-based return (asm:38-44), the walk uses `[1 + direction]` with `direction = Random(4) - 1` (asm:65-74). And `niu_variable` is written once with `ST_UNDEFINED` (asm:56) and never read — dead in the OG too.

The parameters `length_span` / `length_base` are named for what they do rather than what the asm frame calls them (`max` / `min`), because `size = base + Random(span)` makes the range `base+1 .. base+span` — neither parameter is ever the minimum or the maximum.

## `Merge_Dirt_Patches` — full body walk, no defects

Three nested loops with **no stack frame at all** — `di`, `si` and `cx` (asm:88, 82, 76) — bounds 10 / 22 / 21, and both bridge tests (W+E and N+S) present. Confirms this compiler allocates CX as a register local, not just SI and DI.

## `Set_Movement_Cost_Maps` — full body walk, no defects

Fills the four movement-cost arrays. Six phases, all checked.

**Terrain-group switch** (asm:26-141). The jump table at `ovr154/_misc.asm:21-25` was read from the raw bytes — five entries for groups 0-4, guarded by `cmp bx, 4` / `jbe`, so anything above 4 skips the cell entirely. Case 2 (`ctg_Dirt`) is a bare `jmp loc_E2230` into case 0's body, which production reproduces as fallthrough labels sharing one block:

| terrain group | Ground | Teleport | Ground2 | Sailing |
| --- | --- | --- | --- | --- |
| `ctg_Grass` (0) **and** `ctg_Dirt` (2) | 2 | 2 | 2 | `INF` |
| `CTG_Rough` (1) | 4 | 2 | 4 | `INF` |
| `ctg_River` (3) | 4 | 2 | 4 | 2 |
| `ctg_Ocean` (4) | `INF` | 2 | `INF` | 2 |

Rough and River share their Sailing writes with Grass and Ocean respectively via `jmp loc_E226F` and `jmp loc_E234B` (asm:85, 110) — compiler tail-merging, no source implication.

**Roads overlay** (asm:160-201). `terain_group = roads[cell]`; if nonzero *and* not `0x81`, the cell becomes Ground 1 / Teleport 2 / Ground2 1 / Sailing `INF`. This is the one place `roads[]`'s double life matters — road bitmasks on land maps, floating-island decks on water maps — and the `0x81` carve-out exists solely to keep an island's edge cell unwalkable. See the OG-behaviours section.

**Four corner triangles** (asm:212-388), which carve the 21x22 rectangle down to the playable diamond:

| corner | cgy | cgx |
| --- | --- | --- |
| top-left | 0-10 | `0 .. (11 - cgy)` |
| bottom-left | 10-21 | `0 .. (cgy - 9)` |
| top-right | 0-11 | `(cgy + 10) .. 20` |
| bottom-right | 10-21 | `(32 - cgy) .. 20` |

All four set every cost to `INF`. Production matches each bound exactly, including the asymmetric row limits (11, HEIGHT, 12, HEIGHT).

**Central structure** (asm:390-402). When the centre is neither `CS_None` nor `CS_City`, all four costs at offset `0EDh` become `INF`. That offset is 237 = `11 * 21 + 6`, i.e. cell (6, 11) — exactly `COMBAT_STRUCTURE_IDX` ([Combat.h:250-252](../../MoM/src/Combat.h#L250-L252)), so a lair, node or fortress blocks its own square.

One cosmetic note: this function writes `-1  // INF` where the rest of the file uses `INF`. Both are `0xFF` in an `int8_t`, so it is only an inconsistency.

## The location-type switch

The jump table was read from the raw bytes at `ovr154/_misc.asm:5-19` — fifteen entries, `clt__00` through `clt__14`, table order equal to index order. The guard is `cmp bx, 14` / `jbe` (asm:58), unsigned, so anything outside 0-14 skips the switch entirely; production has no `default`, which matches.

All fifteen cases agree with [Combat.h:687-701](../../MoM/src/Combat.h#L687-L701). Two share bodies in the asm via `jmp` (asm:577-580) and production writes them out separately — same result:

| location type | central structure |
| --- | --- |
| `clt_Cave` (8) **and** `clt_MonsterLair` (12) | `CS_Cave` |
| `clt_Dungeon` (9) **and** `clt_Ruins` (13) | `CS_Dungeon` |
| `clt_AncientTemple` (10) | `CS_Temple` |
| `clt_Keep` (11) | `CS_Fort` |
| `clt_FallenTemple` (14) | `CS_Ruins` |

The last row is the one that looks wrong and isn't — `clt_FallenTemple` really does map to `CS_Ruins`, not `CS_Temple` (asm:597-599).

## House placement thresholds

`clt_City` fills the 4x4 grid when `city_population >= 12` (asm:75) and scatters otherwise; `clt_Fortress` uses `>= 11` (asm:314) because the fortress occupies `(1,1)`. Both match, and both scatter paths draw `Random(4)-1` twice for the position and `Random(5)` for the picture.

## RNG accounting

Eight `Random` call sites in the listing (asm:125, 165, 171, 247, 369, 409, 415, 497), bracketed by `Get_Random_Seed` (asm:54) and `Set_Random_Seed` (asm:604) so the generator's draws do not disturb the combat sequence. Production has nine textual sites, but two of them are R1's duplicate of one asm site, so the logical count is eight.

This matters because `HeMoM_Combat_Strategic_Assertions` fails on `combat.rng_calls`. **Nothing in this review is that cause**; the accounting here balances.

---

# OG behaviours — faithful, do not fix

- **`Scatter_Tree_Scenery` bounds-checks the wrong variable.** The four range tests examine the patch seed, which is 0-20 / 0-21 by construction and can never fail, while the scattered tree cell ranges -3..+23 and is never range-checked.
- **`Scatter_Tree_Scenery`'s 500-try budget is shared across all patches and returns from the whole function**, not just the current patch (asm:108-110, `loc_E0BAD` is the epilogue). A crowded map can silently produce far fewer patches than asked for.
- **`ctg_NIU_5` is unreachable.** Every `terrain_group` write in the entire disassembly uses Grass, Rough, Dirt, River or Ocean. The tile arm at `CMB_TileGen__WIP.asm:678-695` is dead code.
- **`Build_Road_Network` repurposes `roads[]` in place** — 0/1 flags in phase 1, a connection bitmask after phase 2. Anything testing `roads[] != 0` afterwards is reading the bitmask.

---

# What is not covered

Nothing in this overlay's generation chain. `Build_Battlefield`, the caller, is covered in [Combat-Combat_Screen.md](Combat-Combat_Screen.md); `Combat_Grid_Screen_Coordinates` and the `Get_/Screen_To_Combat_Grid_Cell_*` helpers are covered in [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md).

# Build state

`cmake --build --preset MSVC-debug --target ReMoM` is clean — `momlib.lib` and `ReMoM.exe` both link.

The full build and the test suite are **not** run for this review, by agreement: several fixes here change generated maps (Tundra now draws 30 rough patches where it drew 0; floating islands write `roads[]` instead of `terrain_type[]`), so the combat goldens need updating before `ctest` means anything. `HeMoM_Combat_Strategic_Assertions` was already failing on a pre-existing `combat.rng_calls` divergence unrelated to this overlay.

## Separately — an allocation bug found while debugging, not a review finding

`battlefield` was allocated at a hardcoded 348 paragraphs (5568 bytes), the size of the **OG** `s_BATTLEFIELD`. This port is x64 and `SAMB_ptr` is 8 bytes rather than 2, which inflates `rock_pict_segs[100]` by 600 bytes and `house_pict_segs[16]` by 96 — so the real struct is about 6234 bytes and `muds` starts at 5504, 64 bytes before the end of the block. Roughly 398 of its 462 entries lay outside the allocation, which is why battlefields filled with mud that nothing ever set, and why everything after `muds` — `house_pict_segs` included — was live-fire memory.

This predates the review and is a port-model bug rather than a fidelity one. `s_BATTLE_UNIT` and `s_COMBAT_ENTITY` carry the same hazard and are still sized by literal paragraph counts; `s_CITY`, `s_MISSILE`, `s_MAGIC_VORTEX`, `s_BLDG` and `s_CITYSCAPE_RC` have no pointer-width members and are unaffected.
