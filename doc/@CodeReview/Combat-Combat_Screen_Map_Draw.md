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

**Status: DONE-DONE (2026-08-25).** No findings open; one deviation. Thirty-five functions compared against 7,086 asm lines. Reopened 2026-08-25 to adjudicate `Screen_To_Combat_Grid_Cell_X_And_Offset` and `Screen_To_Combat_Grid_Cell_Y_And_Offset`, both faithful; see *The screen-to-grid inverses* below.

Thirteen reconstruction errors were raised and all thirteen are fixed. `cmake --build --preset MSVC-debug` is clean — every target links, `MOM_tests` included.

`Combat_Grid_Entities` builds the per-frame entity list — scenery, one entity per unit figure, curses, missiles, vortexes, structures, walls. `Combat_Screen_Map_Draw` paints the terrain grid, then `Combat_Screen_Map_Draw_Entities` draws that list in sorted order. Together they are the combat screen's redraw path.

Six of the thirteen changed what reaches the screen. Trees, rocks and curse overlays never entered the entity list. River, ocean and cloud terrain never drew. Roads, node graphics and the chasm effect painted *on top of* mud squares. `s_BATTLEFIELD.terrain_type` byte-indexed a word array, which also put every later field 462 bytes off its own offset comment. Every combat figure was recoloured with the human player's banner regardless of owner. And the second wall segment of both magic walls drew at the wrong anchor.

## Scope

The Production column carries the current name and line; the ASM column keeps the IDA names, which never change.

| Function (production) | Production | ASM (ground truth) | Lines |
| --- | --- | --- | --- |
| `Screen_To_Combat_Grid_Cell_X_And_Offset` | [Combat.c:21940](../../MoM/src/Combat.c#L21940) | `ovr154/Screen_To_Combat_Grid_Cell_X_And_Offset.asm` | 38 |
| `Screen_To_Combat_Grid_Cell_Y_And_Offset` | [Combat.c:21974](../../MoM/src/Combat.c#L21974) | `ovr154/Screen_To_Combat_Grid_Cell_Y_And_Offset.asm` | 39 |
| `Spawn_Stone_Wall_Entities` | [Combat.c:18691](../../MoM/src/Combat.c#L18691) | `ovr153/CMB_SpawnStoneWall.asm` | 775 |
| `Combat_Screen_Map_Draw` | [Combat.c:18197](../../MoM/src/Combat.c#L18197) | `ovr153/Combat_Screen_Map_Draw__WIP.asm` | 695 |
| `Spawn_Dark_Wall_Entities` | [Combat.c:18810](../../MoM/src/Combat.c#L18810) | `ovr153/CMB_SpawnDarkWall.asm` | 617 |
| `Spawn_Fire_Wall_Entities` | [Combat.c:18951](../../MoM/src/Combat.c#L18951) | `ovr153/CMB_SpawnFireWall.asm` | 617 |
| `Combat_Screen_Map_Draw_Entities` | [Combat.c:18503](../../MoM/src/Combat.c#L18503) | `ovr153/Combat_Screen_Map_Draw_Entities__WIP.asm` | 467 |
| `Spawn_Figure_Entity` | [Combat.c:19132](../../MoM/src/Combat.c#L19132) | `ovr153/CMB_SpawnFigure__WIP.asm` | 423 |
| `Combat_Unit_Enchantment_Outline_Set` | [Combat.c:7027](../../MoM/src/Combat.c#L7027) | `ovr105/Combat_Unit_Enchantment_Outline_Set.asm` | 366 |
| `Combat_Screen_Map_Compose_Figures` | [Combat.c:19616](../../MoM/src/Combat.c#L19616) | `ovr153/Combat_Figure_Compose_USEFULL.asm` | 284 |
| `Spawn_Structure_Entities` | [Combat.c:18633](../../MoM/src/Combat.c#L18633) | `ovr153/CMB_SpawnStructures.asm` | 282 |
| `Unit_Figure_Position` | [UnitView.c:2717](../../MoM/src/UnitView.c#L2717) | `ovr089/Unit_Figure_Position.asm` | 242 |
| `Battle_Unit_Figure_Position` | [Combat.c:19340](../../MoM/src/Combat.c#L19340) | `ovr153/Battle_Unit_Figure_Position.asm` | 236 |
| `Combat_Grid_Entities` | [Combat.c:5658](../../MoM/src/Combat.c#L5658) | `ovr099/Combat_Grid_Entities__WIP.asm` | 216 |
| `Spawn_Missile_Entities` | [Combat.c:19283](../../MoM/src/Combat.c#L19283) | `ovr153/CMB_SpawnProjectiles.asm` | 204 |
| `Combat_Grid_Entity_Create` | [Combat.c:19516](../../MoM/src/Combat.c#L19516) | `ovr153/Combat_Grid_Entity_Create__WIP.asm` | 195 |
| `Draw_Unit_Figure` | [UnitView.c:2640](../../MoM/src/UnitView.c#L2640) | `ovr089/Unit_Figure_Draw.asm` | 181 |
| `Battle_Unit_Set_Invisibility_Effect` | — | `ovr105/BU_SetVisibility__WIP.asm` | 156 |
| `Battle_Unit_Curse_Effects` | [Combat.c:7333](../../MoM/src/Combat.c#L7333) | `ovr105/BU_GetCombatEffect__WIP.asm` | 144 |
| `Spawn_Vortex_Entities` | [Combat.c:19091](../../MoM/src/Combat.c#L19091) | `ovr153/Combat_Grid_Entity_Create_Vortexes.asm` | 137 |
| `Spawn_Curse_Entity` | [Combat.c:19222](../../MoM/src/Combat.c#L19222) | `ovr153/CMB_SpawnUnitCurse.asm` | 92 |
| `Combat_Figure_Effect` | [Combat.c:6989](../../MoM/src/Combat.c#L6989) | `ovr105/Combat_Figure_Effect__WIP.asm` | 90 |
| `Battle_Unit_Set_Animation_Flags` | [Combat.c:7365](../../MoM/src/Combat.c#L7365) | `ovr105/BU_SetBaseAnims__WIP.asm` | 82 |
| `Combat_Figure_Compose` | [Combat.c:19268](../../MoM/src/Combat.c#L19268) | `ovr153/USELESS_Combat_Figure_Load_Compose.asm` | 77 |
| `Set_Entity_Draw_Order` | [Combat.c:19548](../../MoM/src/Combat.c#L19548) | `ovr153/Set_Entity_Draw_Order.asm` | 71 |
| `Update_Sees_Illusions` | [Combat.c:7225](../../MoM/src/Combat.c#L7225) | `ovr105/Update_Sees_Illusions.asm` | 67 |
| `Spawn_Tree_Entities` | [Combat.c:18584](../../MoM/src/Combat.c#L18584) | `ovr153/CMB_SpawnTrees.asm` | 58 |
| `Spawn_Rock_Entities` | [Combat.c:18608](../../MoM/src/Combat.c#L18608) | `ovr153/CMB_SpawnRocks.asm` | 58 |
| `Combat_Figure_Banner_Color` | [Combat.c:6962](../../MoM/src/Combat.c#L6962) | `ovr105/Combat_Figure_Banner_Color.asm` | 45 |
| `Battle_Unit_Is_Airborne` | [Combat.c:4964](../../MoM/src/Combat.c#L4964) | `ovr098/BU_CheckFlight__WIP.asm` | 43 |
| `Combat_Unit_Enchantment_Outline_Draw` | [Combat.c:7187](../../MoM/src/Combat.c#L7187) | `ovr105/Combat_Unit_Enchantment_Outline_Draw.asm` | 36 |
| `NIU_Gibs_Frames` | [Combat.c:18574](../../MoM/src/Combat.c#L18574) | `ovr153/NX_IDK_CMB_BloodFrames.asm` | 25 |
| `Combat_Figure_Active_Red_Outline` | [Combat.c:6979](../../MoM/src/Combat.c#L6979) | `ovr105/Combat_Figure_Active_Red_Outline.asm` | 23 |
| `Cycle_Unit_Enchantment_Animation` | [MainScr.c:3744](../../MoM/src/MainScr.c#L3744) | `ovr063/Cycle_Unit_Enchantment_Animation.asm` | 23 |
| `Clear_Combat_Grid_Entities` | [Combat.c:19253](../../MoM/src/Combat.c#L19253) | `ovr153/Clear_Combat_Grid_Entities.asm` | 19 |

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
- **`Combat_Grid_Entity_Create`'s parameter map.** The listing's names are IDA's and several are misleading, so the correspondence is worth having in one place. `ovr153/Combat_Grid_Entity_Create__WIP.asm` declares, in push order: `draw_x`(6), `draw_y`(8), `pict_seg_or_idx`(0Ah), `draw_x_shift`(0Ch), `draw_y_shift`(0Eh), `Frame`(10h), `entity_type`(12h), `Owner`(14h), `UU_10h`(16h), `outline_magic_realm`(18h), `BldAmt`(1Ah), `UU_14h`(1Ch), `BldFrm`(1Eh). Production carries these as `frame_num`, `controller_idx`, `niu_figure_set_idx`, `gibs`, `niu_14h` and `blood_frame` respectively. Note the argument order and the **struct** field order differ — the struct is outline(0x12), Unused(0x14), Blood_Frame(0x16), Blood_Amt(0x18), so `BldAmt`/`BldFrm` swap places between the two.
- **`blood_frame` is live; `niu_14h` is not.** `Blood_Frame` is read at [Combat.c:18526-18531](../../MoM/src/Combat.c#L18526-L18531), where `!= 3` gates it, `>= 0` guards it, and it indexes `Set_Animation_Frame(cmbtcity_blood_segs[gibs - 1], ...)` — so it is the blood animation's frame, paired with `gibs` selecting the sprite bank. `Unused_14h` is written at [Combat.c:19536](../../MoM/src/Combat.c#L19536) and **read nowhere in the tree**. Twelve of the thirteen call sites pass `0` for it; the one that does not, `Spawn_Figure_Entity` at [Combat.c:19215](../../MoM/src/Combat.c#L19215), passes `frame_num` — the same value it already passed as argument 6. A dead duplicate rather than a lost feature, but one caller is not enough to name it, so it stays `niu_14h`.
- **`outline_magic_realm` and `Unused_14h` are not halves of a 32-bit enchantment field**, despite sitting adjacent at 0x12/0x14 and despite this codebase genuinely splitting `uint32_t` enchantment masks into word pairs elsewhere. Four things rule it out: the two parameters are not adjacent (`BldAmt` sits between them at `bp+1Ah`); each store recomputes the element address from scratch (asm:102-103 then asm:104-110) where a 32-bit member would use one computation and `[x]`/`[x+2]`; the struct and parameter orders disagree, which a split variable could not do; and `outline_magic_realm` holds a small magic-realm enum used as an array subscript into `enchantment_outline_colors[][]` ([Combat.c:7192](../../MoM/src/Combat.c#L7192)), assigned `mr_Life`/`mr_Death`/`mr_Nature`/`mr_Chaos` by the priority chain ending at [Combat.c:7179](../../MoM/src/Combat.c#L7179), with `ST_UNDEFINED` as its none-value. It is the *result* of testing the enchantment mask, not part of one.
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

---

# The screen-to-grid inverses

Two `ovr154` functions this document has named in its call tree since the first pass without ever giving them a row. Walked 2026-08-25, both faithful, 77 asm lines. They are the inverse of `Combat_Grid_Screen_Coordinates`, which carries its verdict in [Combat-Miscellaneous.md](Combat-Miscellaneous.md); both are called from `Combat_Screen_Map_Draw_Entities` and `Combat_Grid_Entity_Create`, which is why they land here.

## `Screen_To_Combat_Grid_Cell_X_And_Offset` ([Combat.c:21940](../../MoM/src/Combat.c#L21940), asm 38)

No `sub sp` and no stack locals; the two out-pointers are cached into SI and DI, and `IDK` is CX. Production declares the single register local with a `// _CX_` note.

`add ax, -158` then the signed-halve idiom `cwd / sub ax, dx / sar ax, 1` (asm:17-20), then `+ screen_y`, then `+ 80` — [Combat.c:22190](../../MoM/src/Combat.c#L22190) writes it in that order.

**The divide runs twice.** asm:25-28 does `mov bx, 16 / cwd / idiv bx` and stores AX; asm:30-33 repeats the *whole* divide and stores DX. The remainder from the first `idiv` is discarded rather than reused. That is what a compiler with no div/mod CSE emits for two separate expressions, and production writes two separate statements — faithful, not a missed optimisation to tidy.

## `Screen_To_Combat_Grid_Cell_Y_And_Offset` ([Combat.c:21974](../../MoM/src/Combat.c#L21974), asm 39)

Same shape. The one difference is the order of the subtraction: asm:21-23 loads `screen_y + 80` into DX and subtracts the halved X term from it, so the source reads `(screen_y + 80) - ((screen_x - 158) / 2)` — which is exactly how [Combat.c:22225](../../MoM/src/Combat.c#L22225) writes it. Operand order preserved. The double `idiv` is present here too.

**Both check out algebraically against the forward transform.** `Combat_Grid_Screen_Coordinates` computes `sx = ((cgx - cgy) * 16) + 158` and `sy = ((cgx + cgy) * 8) - 80`, so `(sx - 158) = 16(cgx - cgy)` and `(sy + 80) = 8(cgx + cgy)`. Substituting:

- X: `(((sx - 158) / 2) + (sy + 80)) / 16` = `(8(cgx - cgy) + 8(cgx + cgy)) / 16` = `cgx`
- Y: `((sy + 80) - ((sx - 158) / 2)) / 16` = `(8(cgx + cgy) - 8(cgx - cgy)) / 16` = `cgy`

The two round-trip exactly, which is an independent check on both directions that does not depend on reading either listing correctly.
