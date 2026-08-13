Combat-Combat_Screen_Map_Draw.md

SEEALSO:  Combat-Combat.md
SEEALSO:  Combat-Combat_Screen_Map_Draw.md
SEEALSO:  Combat-Combat_Figure_Compose.md
SEEALSO:  Combat-Generate_Combat_Map.md
SEEALSO:  Combat-End_Of_Combat.md
SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoM-CombatScreen-Grid.md

CMB_SpawnProjectiles()                  ==>  Spawn_Missile_Entities()
BU_SetBaseAnims__WIP()                  ==>  Battle_Unit_Set_Animation_Flags()
Combat_Grid_Entities__WIP()             ==>  Combat_Grid_Entities()
Combat_Screen_Map_Draw__WIP()           ==>  Combat_Screen_Map_Draw()
Combat_Screen_Map_Draw_Entities__WIP()  ==>  Combat_Screen_Map_Draw_Entities()
CMB_SpawnFigure__WIP()                  ==>  Spawn_Figure_Entity()
CMB_SpawnUnitCurse()                    ==>  Spawn_Curse_Entity()
Combat_Grid_Entity_Create__WIP()        ==>  Combat_Grid_Entity_Create()
CMB_SpawnStructures__WIP()              ==>  Spawn_Structure_Entities()
Combat_Grid_Entity_Create_Vortexes()    ==>  Spawn_Vortex_Entities()
BU_GetCombatEffect__WIP()               ==>  Battle_Unit_Curse_Effects()
NX_IDK_CMB_BloodFrames()                ==>  NIU_Gibs_Frames()
CMB_SpawnTrees()                        ==>  Spawn_Tree_Entities()
CMB_SpawnRocks()                        ==>  Spawn_Rock_Entities()
CMB_SpawnStoneWall__WIP()               ==>  Spawn_Stone_Wall_Entities()
CMB_SpawnDarkWall__WIP()                ==>  Spawn_Dark_Wall_Entities()
CMB_SpawnFireWall__WIP()                ==>  Spawn_Fire_Wall_Entities()
BU_CreateImage__SEGRAX()                ==>  Battle_Unit_Compose_Bitmap()

CMB_CurseAnimStage                      ==>  _combat_curse_anim_frame
cnst_CMBEntity_Error                    ==>  str_E1
IMG_CMB_Curses                          ==>  combat_curse_entity_seg
CMB_RangedAtx_GFX
CMB_Enchanted_Roads

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr099\Combat_Grid_Entities__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\Combat_Screen_Map_Draw__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\Combat_Screen_Map_Draw_Entities__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\NX_IDK_CMB_BloodFrames.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\CMB_SpawnTrees.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\CMB_SpawnRocks.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\CMB_SpawnStructures.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\CMB_SpawnFireWall.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\CMB_SpawnDarkWall.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\CMB_SpawnStoneWall.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\CMB_SpawnFigure__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\CMB_SpawnUnitCurse.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\Clear_Combat_Grid_Entities.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\CMB_SpawnProjectiles.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr105\BU_SetBaseAnims__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr105\BU_GetCombatEffect__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\BU_CreateImage__SEGRAX.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\Combat_Figure_Compose_USEFULL.asm

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\Combat_Grid_Entity_Create__WIP.asm
    |-> Screen_To_Combat_Grid_Cell_X_And_Offset()
    |-> Screen_To_Combat_Grid_Cell_Y_And_Offset()
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\Set_Entity_Draw_Order.asm

Combat_Screen()
    |-> Combat_Screen_Draw()
        |-> Combat_Grid_Entities()
        |-> Combat_Screen_Map_Draw()
            |-> Combat_Screen_Map_Draw_Entities()

Combat_Grid_Entity_Create()
    |-> Screen_To_Combat_Grid_Cell_X_And_Offset()
    |-> Screen_To_Combat_Grid_Cell_Y_And_Offset()
    |-> Exit_With_Message()
    |-> Set_Entity_Draw_Order()

Combat_Screen()
    |-> Combat_Screen_Draw()
        |-> Combat_Grid_Entities()
            |-> Clear_Combat_Grid_Entities()
            |-> Spawn_Tree_Entities()
            |-> Spawn_Rock_Entities()
            |-> Update_Sees_Illusions()
            |-> Combat_Unit_Enchantment_Outline_Set()
            |-> Battle_Unit_Set_Animation_Flags()
            |-> Battle_Unit_Curse_Effects()
            |-> Battle_Unit_Set_Invisibility_Effect()
            |-> Combat_Screen_Map_Draw_Entities()
            |-> Spawn_Curse_Entity()
            |-> Spawn_Missile_Entities()
            |-> Spawn_Vortex_Entities()
            |-> Spawn_Structure_Entities()
                |-> Spawn_Stone_Wall_Entities()
                |-> Spawn_Dark_Wall_Entities()
                |-> Spawn_Fire_Wall_Entities()
            |-> Cycle_Unit_Enchantment_Animation()
        |-> Combat_Screen_Map_Draw()
            |-> Combat_Screen_Map_Compose_Figures()
            |-> Combat_Screen_Map_Draw_Entities()



Spawn_Stone_Wall_Entities()
Spawn_Dark_Wall_Entities()
Spawn_Fire_Wall_Entities()
    // north-west corner
    // west
    // south
    // north
    // east
3 different calls to Combat_Grid_Entity_Create()
        if((_wall_rise_type == 0) && (_wall_rise_on == ST_TRUE))
        else
            if(battlefield->walls[0][(itr - 9)] == 1)  /* flat: itr-9 */
            if(battlefield->walls[0][(itr - 9)] == 2)  /* flat: itr-9 */

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-08).** No findings open; one deviation. Thirty-three functions compared against 7,009 asm lines.

Thirteen reconstruction errors were raised and all thirteen are fixed. `cmake --build --preset MSVC-debug` is clean — every target links, `MOM_tests` included.

`Combat_Grid_Entities` builds the per-frame entity list — scenery, one entity per unit figure, curses, missiles, vortexes, structures, walls. `Combat_Screen_Map_Draw` paints the terrain grid, then `Combat_Screen_Map_Draw_Entities` draws that list in sorted order. Together they are the combat screen's redraw path.

Six of the thirteen changed what reaches the screen. Trees, rocks and curse overlays never entered the entity list. River, ocean and cloud terrain never drew. Roads, node graphics and the chasm effect painted *on top of* mud squares. `s_BATTLEFIELD.terrain_type` byte-indexed a word array, which also put every later field 462 bytes off its own offset comment. Every combat figure was recoloured with the human player's banner regardless of owner. And the second wall segment of both magic walls drew at the wrong anchor.

## Scope

The Production column carries the current name and line; the ASM column keeps the IDA names, which never change.

| Function (production) | Production | ASM (ground truth) | Lines |
| --- | --- | --- | --- |
| `Spawn_Stone_Wall_Entities` | [Combat.c:20062](../../MoM/src/Combat.c#L20062) | `ovr153/CMB_SpawnStoneWall.asm` | 775 |
| `Combat_Screen_Map_Draw` | [Combat.c:19568](../../MoM/src/Combat.c#L19568) | `ovr153/Combat_Screen_Map_Draw__WIP.asm` | 695 |
| `Spawn_Dark_Wall_Entities` | [Combat.c:20181](../../MoM/src/Combat.c#L20181) | `ovr153/CMB_SpawnDarkWall.asm` | 617 |
| `Spawn_Fire_Wall_Entities` | [Combat.c:20322](../../MoM/src/Combat.c#L20322) | `ovr153/CMB_SpawnFireWall.asm` | 617 |
| `Combat_Screen_Map_Draw_Entities` | [Combat.c:19874](../../MoM/src/Combat.c#L19874) | `ovr153/Combat_Screen_Map_Draw_Entities__WIP.asm` | 467 |
| `Spawn_Figure_Entity` | [Combat.c:20503](../../MoM/src/Combat.c#L20503) | `ovr153/CMB_SpawnFigure__WIP.asm` | 423 |
| `Combat_Unit_Enchantment_Outline_Set` | [Combat.c:7602](../../MoM/src/Combat.c#L7602) | `ovr105/Combat_Unit_Enchantment_Outline_Set.asm` | 366 |
| `Combat_Screen_Map_Compose_Figures` | [Combat.c:21007](../../MoM/src/Combat.c#L21007) | `ovr153/Combat_Figure_Compose_USEFULL.asm` | 284 |
| `Spawn_Structure_Entities` | [Combat.c:20004](../../MoM/src/Combat.c#L20004) | `ovr153/CMB_SpawnStructures.asm` | 282 |
| `Unit_Figure_Position` | [UnitView.c:2717](../../MoM/src/UnitView.c#L2717) | `ovr089/Unit_Figure_Position.asm` | 242 |
| `Battle_Unit_Figure_Position` | [Combat.c:20711](../../MoM/src/Combat.c#L20711) | `ovr153/Battle_Unit_Figure_Position.asm` | 236 |
| `Combat_Grid_Entities` | [Combat.c:5902](../../MoM/src/Combat.c#L5902) | `ovr099/Combat_Grid_Entities__WIP.asm` | 216 |
| `Spawn_Missile_Entities` | [Combat.c:20654](../../MoM/src/Combat.c#L20654) | `ovr153/CMB_SpawnProjectiles.asm` | 204 |
| `Combat_Grid_Entity_Create` | [Combat.c:20907](../../MoM/src/Combat.c#L20907) | `ovr153/Combat_Grid_Entity_Create__WIP.asm` | 195 |
| `Draw_Unit_Figure` | [UnitView.c:2640](../../MoM/src/UnitView.c#L2640) | `ovr089/Unit_Figure_Draw.asm` | 181 |
| `Battle_Unit_Set_Invisibility_Effect` | — | `ovr105/BU_SetVisibility__WIP.asm` | 156 |
| `Battle_Unit_Curse_Effects` | [Combat.c:7908](../../MoM/src/Combat.c#L7908) | `ovr105/BU_GetCombatEffect__WIP.asm` | 144 |
| `Spawn_Vortex_Entities` | [Combat.c:20462](../../MoM/src/Combat.c#L20462) | `ovr153/Combat_Grid_Entity_Create_Vortexes.asm` | 137 |
| `Spawn_Curse_Entity` | [Combat.c:20593](../../MoM/src/Combat.c#L20593) | `ovr153/CMB_SpawnUnitCurse.asm` | 92 |
| `Combat_Figure_Effect` | [Combat.c:7564](../../MoM/src/Combat.c#L7564) | `ovr105/Combat_Figure_Effect__WIP.asm` | 90 |
| `Battle_Unit_Set_Animation_Flags` | [Combat.c:7940](../../MoM/src/Combat.c#L7940) | `ovr105/BU_SetBaseAnims__WIP.asm` | 82 |
| `Combat_Figure_Compose` | [Combat.c:20639](../../MoM/src/Combat.c#L20639) | `ovr153/USELESS_Combat_Figure_Load_Compose.asm` | 77 |
| `Set_Entity_Draw_Order` | [Combat.c:20939](../../MoM/src/Combat.c#L20939) | `ovr153/Set_Entity_Draw_Order.asm` | 71 |
| `Update_Sees_Illusions` | [Combat.c:7800](../../MoM/src/Combat.c#L7800) | `ovr105/Update_Sees_Illusions.asm` | 67 |
| `Spawn_Tree_Entities` | [Combat.c:19955](../../MoM/src/Combat.c#L19955) | `ovr153/CMB_SpawnTrees.asm` | 58 |
| `Spawn_Rock_Entities` | [Combat.c:19979](../../MoM/src/Combat.c#L19979) | `ovr153/CMB_SpawnRocks.asm` | 58 |
| `Combat_Figure_Banner_Color` | [Combat.c:7537](../../MoM/src/Combat.c#L7537) | `ovr105/Combat_Figure_Banner_Color.asm` | 45 |
| `Battle_Unit_Is_Airborne` | [Combat.c:5199](../../MoM/src/Combat.c#L5199) | `ovr098/BU_CheckFlight__WIP.asm` | 43 |
| `Combat_Unit_Enchantment_Outline_Draw` | [Combat.c:7762](../../MoM/src/Combat.c#L7762) | `ovr105/Combat_Unit_Enchantment_Outline_Draw.asm` | 36 |
| `NIU_Gibs_Frames` | [Combat.c:19945](../../MoM/src/Combat.c#L19945) | `ovr153/NX_IDK_CMB_BloodFrames.asm` | 25 |
| `Combat_Figure_Active_Red_Outline` | [Combat.c:7554](../../MoM/src/Combat.c#L7554) | `ovr105/Combat_Figure_Active_Red_Outline.asm` | 23 |
| `Cycle_Unit_Enchantment_Animation` | [MainScr.c:3744](../../MoM/src/MainScr.c#L3744) | `ovr063/Cycle_Unit_Enchantment_Animation.asm` | 23 |
| `Clear_Combat_Grid_Entities` | [Combat.c:20624](../../MoM/src/Combat.c#L20624) | `ovr153/Clear_Combat_Grid_Entities.asm` | 19 |

`Battle_Unit_Set_Invisibility_Effect` was already DONE-DONE via [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md) and was not re-reviewed.

**Corrections to the seeded header.** `Spawn_Missile_Entities` is `ovr153/CMB_SpawnProjectiles.asm`. `Battle_Unit_Set_Animation_Flags` is `ovr105/BU_SetBaseAnims__WIP.asm`, not `ovr153`. `Draw_Unit_Figure` is `ovr089/Unit_Figure_Draw.asm` — the two halves of the name are inverted between the sides.

**Rename ledger** — IDA name on the left, production name on the right.

```
CMB_SpawnStoneWall / DarkWall / FireWall ==> Spawn_Stone_Wall_Entities() / Spawn_Dark_Wall_Entities() / Spawn_Fire_Wall_Entities()
CMB_SpawnStructures            ==>  Spawn_Structure_Entities()
CMB_SpawnFigure__WIP           ==>  Spawn_Figure_Entity()
CMB_SpawnUnitCurse             ==>  Spawn_Curse_Entity()
CMB_SpawnProjectiles           ==>  Spawn_Missile_Entities()
CMB_SpawnTrees / CMB_SpawnRocks ==> Spawn_Tree_Entities() / Spawn_Rock_Entities()
Combat_Grid_Entity_Create_Vortexes ==> Spawn_Vortex_Entities()
BU_GetCombatEffect__WIP        ==>  Battle_Unit_Curse_Effects()
BU_SetBaseAnims__WIP           ==>  Battle_Unit_Set_Animation_Flags()
BU_CheckFlight__WIP            ==>  Battle_Unit_Is_Airborne()
Unit_Figure_Draw               ==>  Draw_Unit_Figure()
NX_IDK_CMB_BloodFrames         ==>  NIU_Gibs_Frames()
EMM_TILEX_Init__HACK           ==>  Map_Tile_EMS_Page_As_Sandbox()
EMM_FIGUREX_Init__HACK         ==>  Claim_EMS_Page_For_Figure_Set()
CMB_BloodFrames + IDK_CMB_BloodFrames ==> m_gibs_frames[20]
combat_entity_draw_order_array ==>  combat_grid_entities_draw_order[]
Blood_Amt / Blood_Frame        ==>  gibs / gibs_frame
SrcBld / LostFigs              ==>  gibs_bank / figures_lost
Always_Animate / Move_Bob      ==>  animate_idle / animate_move_as_idle
Moving                         ==>  mid_move
Combat_Effects                 ==>  combat_effects
UU_Facing                      ==>  niu_figure_set_idx
CMB_MudAnimStage               ==>  _combat_mud_anim_phase
CMB_WaterAnimStage             ==>  _combat_water_anim_frame
CMB_CNodeAnimStage             ==>  _combat_chaos_node_anim_frame
CMB_RoadAnimStage              ==>  _combat_road_anim_frame
G_CMB_MWallAnimStage           ==>  _combat_magic_wall_anim_frame
CMB_BaseAnimFrame              ==>  combat_idle_anim_frame
CMB_MoveAnimFrame              ==>  combat_walk_anim_frame
CMB_ProjectileFrame            ==>  projectile_anim_frame
CMB_ChasmAnimStage             ==>  cmbt_cell_effect_frame
CMB_StoneWallType              ==>  _combat_wall_sprite_bank
GfxBuf_2400B                   ==>  scratch_bitmap_seg
IMG_CMB_OceanTile / ChaosOcn / Cloud / RiverTile / RivrNULLs  ==>  cmbtcity_ocean_segs / chriver_chaoswat_segs / cmbtcity_cmbcloud_segs / cmbtcity_river_segs / chriver_river_segs
IMG_CMB_RoadTiles              ==>  cmbtcity_roadgrid_segs
IMG_CMB_Blood                  ==>  cmbtcity_blood_segs
```

**Evidence standard.** A divergence is recorded only where the bytes show a different computation, a different order, or a different operand width.

---

# Findings

None open.

# Deviations

## D1 — the cell-effect frame divisor

[Combat.c:19806](../../MoM/src/Combat.c#L19806) divides by `CELL_EFFECT_ANIM_HOLD`; `Combat_Screen_Map_Draw__WIP.asm:502` passes `CMB_ChasmAnimStage` raw. Paired with the rescaled counter in `Combat_Screen_Draw`, which wraps at `7 * CELL_EFFECT_ANIM_HOLD` where `Combat_Screen_Draw.asm:621` wraps at 7.

Deliberate, marked `HACK`, and now self-consistent: with `HOLD` = 7 the counter runs 0..49 and the reader yields 0..7, exactly the original's frame range with each frame held seven redraws. [Spells133.c:681](../../MoM/src/Spells133.c#L681) writes on the same scale.

One thing to watch: the `7` in `(7 * CELL_EFFECT_ANIM_HOLD)` is the **last frame index**, not a hold count — equal to `HOLD` by coincidence. If `CELL_EFFECT_ANIM_HOLD` changes, that bare 7 stays put.

---

# Notes worth keeping

## Three different figure-position tables

The comment at [CMBTDEF.h:301-306](../../MoM/src/CMBTDEF.h#L301-L306) — "`Spawn_Figure_Entity()` / `Battle_Unit_Figure_Position()` / `Unit_Figure_Position()` !!!!! ALL DIFFERENT !!!!!" — is confirmed for all three. Separate layouts in the original; none can be refactored into another:

| | case 4, fig 1 | case 6, fig 1 |
| --- | --- | --- |
| `BATTLE_UNIT_FIGURE_POSITION` (macro, used by `Spawn_Figure_Entity`) | −7, 8 | 4, 7 |
| `Battle_Unit_Figure_Position` | 8, 8 | 10, 8 |
| `Unit_Figure_Position` | −9, 8 | 4, 7 |

`Battle_Unit_Figure_Position` has two callers: `Make_Missiles` for the volley spread, and `Battle_Unit_Compose_Bitmap` ([Combat.c:6836](../../MoM/src/Combat.c#L6836)), reached from [Spells133.c:979](../../MoM/src/Spells133.c#L979) for combat teleport, tunnel and summon.

## The three FIGUREX macros really do differ in order

The `¿ three different macros for a (real) reason ?` note at [Combat.h:575-582](../../MoM/src/Combat.h#L575-L582) is right, and ordering is the reason:

| function | order |
| --- | --- |
| `Combat_Figure_Load` | MAP, then OFFSET |
| `Combat_Screen_Map_Compose_Figures` | MAP, then OFFSET |
| `Combat_Figure_Compose` | **OFFSET, then MAP** |

Production preserves each function's own order. Do not normalise them.

## The wall spawners

`_wall_rise_type` is **0** for stone, **1** for fire, **2** for dark (asm:34 in each listing).

`Spawn_Stone_Wall_Entities` has five blocks of three arms — wall-rise, then `walls[itr][0] == 1` with frame **0**, then `== 2` with frame **1**. The magic walls have seven blocks of two arms that converge on one shared `Combat_Grid_Entity_Create` call.

Anchor pairs, verified across all 40 call sites:

| block | stone | dark / fire |
| --- | --- | --- |
| 1 | `15,18` rise; `width/2,18` ×2 | `15,19` rise; `width/2,19` |
| 2 | `17,17` | `17,18` |
| 3 | `17,17` | `17,17` |
| 4 | `16,30` | `16,29` |
| 5 | `16,30` | `16,29` |
| 6 | — | `16,30` |
| 7 | — | `16,30` |

Only block 1 uses `FLIC_Get_Width`; every other block uses literals.

**The `flag` parameter is a draw-layer selector.** Blocks with `flag == 0` cover cells whose `cgx + cgy` sums are 15-18 (the far west and north edges); `flag == 1` covers 18-21 (the near south and east). `Spawn_Structure_Entities` calls the three spawners six times — stone(0), dark(0), fire(0), fire(1), dark(1), stone(1) — which is back-to-front painting order. Collapsing that into three paired calls would break it.

## OGBUGs — faithful, do not fix

- **`Spawn_Rock_Entities` uses the tree arrays.** `CMB_SpawnRocks.asm:33` reads `Tree_Indices` and asm:53 bounds the loop on `Tree_Count`, while asm:40/45 correctly read `Rock_DrawYs`/`Rock_DrawXs`. `Rock_Count` and `Rock_IMG_Segs` are written by the generator and never read.
- **`Set_Entity_Draw_Order` copies one element too many.** asm:60-61 is `jge`, so the shift loop reads `combat_grid_entities_draw_order[entity_idx - 1]`, which underflows when `entity_idx` is 0.
- **`Combat_Grid_Entity_Create`'s overflow guard is `>` not `>=`** (asm:28-29 `jle`), so index `MAX_ENTITIES` is reachable on a 256-element array.
- **`Battle_Unit_Set_Animation_Flags` calls `Battle_Unit_Is_Airborne` with two arguments** — asm:23-26 pushes a literal `0` and the index with two `pop cx`, while the callee's frame declares only `battle_unit_idx`.
- **`Spawn_Figure_Entity`'s gib bank is never selected.** Every caller passes `0` for `gibs_bank`, so the upper ten `m_gibs_frames` slots are seeded by `Set_Gibs` and never read. IDA's name for that array is `IDK_CMB_BloodFrames`.
- **`NIU_Gibs_Frames` is dead in the original too.** `WIZARDS.asm:79353-79355` is a thunk that jumps to it and nothing calls the thunk; `Set_Gibs` carries its own inlined copy.
- **Two write-only fields in `s_COMBAT_ENTITY`.** `niu_figure_set_idx` receives the computed 0-7 facing and is never read; `Unused_14h` receives `mid_move` a second time and is never read.

## Added defensive code

`Unit_Figure_Position` carries four `assert()` calls at [UnitView.c:2722-2725](../../MoM/src/UnitView.c#L2722-L2725) with no asm counterpart. They matter because production spells each table's last arm as an explicit `case N:` where the original uses a fall-through default — so an out-of-range `current_figure` leaves both outputs at their initializers in production, while the original would assign the default pair.

## Build state

`cmake --build --preset MSVC-debug` — **clean, exit code 0**. All 27 targets link, including `momlib`, `ReMoM.exe`, `HeMoM.exe` and `MOM_tests`.

---

# What is not covered

`Battle_Unit_Compose_Bitmap` ([Combat.c:6836](../../MoM/src/Combat.c#L6836)) shares `Battle_Unit_Figure_Position` with `Make_Missiles` and was not reviewed. `Draw_Unit_Enchantment_Outline` and `Unit_Has_Invisibility` are called from `Draw_Unit_Figure`, which sits outside both entry points' call trees and was reviewed only because it shares the figure-position tables.
