Combat-Combat_Screen.md

SEEALSO:  Combat-Combat_Screen_Map_Draw.md
SEEALSO:  Combat-Combat_Figure_Compose.md
SEEALSO:  Combat-End_Of_Combat.md
SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoM-CombatScreen-Grid.md

CMB_PrepareTurn__WIP()        ==>  Begin_Combat_Turn()
Combat_Screen_Draw_Buttons()  ==>  Tactical_Combat_Draw_Buttons()
BU_SetVisibility__WIP()       ==>  Battle_Unit_Set_Invisibility_Effect()
CMB_ComposeBackgrnd__WIP()    ==>  Combat_Compose_Background()
Combat_Screen__WIP()          ==>  Combat_Screen()
BU_HasSpellAbility()          ==>  BU_HasSpellAbility__WIP()
sub_7EC81()                   ==>  NIU_Who_Has_More_Leadership()
CMB_BaseAllocs__WIP()         ==>  Allocate_Combat_Base_Blocks()
CMB_SetNearAllocs__WIP()      ==>  Allocate_Combat_Near_Buffers()
CMB_LoadResources__WIP()      ==>  Combat_Screen_Load_Resources()
CMB_Terrain_Init__WIP()       ==>  Build_Battlefield()
EMM_TILEX_Init__HACK()        ==>  Claim_EMS_Page_For_Figure_Set()
CMB_Units_Init__WIP()         ==>  Prepare_All_Battle_Units()
BU_Apply_Specials()           ==>  Apply_Enchantment_And_Mutation_Effects()
BU_HasSpellAbility__WIP()     ==>  Battle_Unit_Has_Spell_Ability()

combat_human_player           ==>  _combat_local_player
combat_computer_player        ==>  _combat_remote_player
CMB_AI_Player                 ==>  _combat_ai_player
CMB_ATKR_First_CE             ==>  attacker_enchantment_first_shown
CMB_DEFR_First_CE             ==>  defender_enchantment_first_shown
m_unknown_variable__ovr090    ==>  niu_combat_screen_dirty
First_CE_Help_Entry           ==>  enchantment_help_entry_base
CE_Window_Left                ==>  enchantment_strip_left_x
RightClick_X                  ==>  right_click_cgx 
RightClick_Y                  ==>  right_click_cgy
Battle_Result                 ==>  end_of_combat_message_type

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr090\Combat_Screen__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\CMB_BaseAllocs__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\CMB_LoadResources__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\CMB_SetNearAllocs__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr052\Cache_Graphics_Combat.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\CMB_Terrain_Init__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\Load_Combat_Terrain_Pictures.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\CMB_ComposeBackgrnd__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\EMM_TILEX_Init__HACK.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Combat_Grid_Screen_Coordinates.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\CMB_Units_Init__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\Load_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr120\Unit_Gold_Upkeep.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Init_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Apply_Level.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Init_Hero_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Apply_Item_Powers.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Apply_Specials.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr121\Unit_Moves2.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\Unit_Hit_Points.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\Combat_Figure_Load.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\Deploy_Battle_Units.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Undeployable_Battle_Units_On_Water.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Combat_Structure.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Update_Combat_Enchantments_Icon_And_Help.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\Combat_Node_Type.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\Combat_Info_Effects_Count.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Calc_Battlefield_Bonuses.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Combat_Cache_Write.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\Switch_Active_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\CMB_PrepareTurn__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr099\Combat_Screen_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr099\Combat_Screen_Draw_Buttons.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr099\Draw_Spell_Information_Window.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr105\Draw_Combat_Unit_Display.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr099\Draw_Active_Unit_Window.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Auto_Cast_Spell_And_Do_Combat_Turn.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr105\Check_For_Winner.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\Next_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\Assign_Combat_Grids.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Turn_Off_Auto_Combat.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\BU_HasSpellAbility__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Add_Combat_Enchantment_Fields.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\Move_Confused.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\Battle_Unit_Action.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr105\BU_SetVisibility__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Get_Combat_Grid_Cell_X.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Get_Combat_Grid_Cell_Y.asm

WIZARDS.EXE  ovr090

int16_t Combat_Screen(
    int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx,
    int16_t troops[], int16_t troop_count,
    int16_t wx, int16_t wy, int16_t wp,
    int16_t * item_count, int16_t item_list[]
)

Combat_Screen()
    |-> Allocate_Combat_Base_Blocks()
    |-> Combat_Screen_Load_Resources()
        |-> Allocate_Combat_Near_Buffers()
    |-> Cache_Graphics_Combat()
    |-> Build_Battlefield()
        |-> Load_Combat_Terrain_Pictures()
        |-> Generate_Combat_Map()
        |-> Combat_Compose_Background()
            |-> Claim_EMS_Page_For_Figure_Set()
            |-> Combat_Grid_Screen_Coordinates()
    |-> Prepare_All_Battle_Units()
        |-> Load_Battle_Unit()
            |-> Unit_Gold_Upkeep()
            |-> Battle_Unit_Regular_Stats()
                |-> Battle_Unit_Level_Stats()
                |-> Battle_Unit_Hero_Skill_Stats()
                |-> Battle_Unit_Item_Stats()
                |-> Apply_Enchantment_And_Mutation_Effects()
                |-> Unit_Moves2()
                |-> Unit_Hit_Points()
        |-> Combat_Figure_Load()
            |-> Combat_Figure_Compose()                Combat-Combat_Figure_Compose.md
        |-> Deploy_Battle_Units()
            |-> Undeployable_Battle_Units_On_Water()
    |-> Combat_Structure()
    |-> Update_Combat_Enchantments_Icon_And_Help()
    |-> Combat_Node_Type()
    |-> Combat_Info_Effects_Count()
    |-> Calc_Battlefield_Bonuses()
    |-> Combat_Cache_Write()
    |-> Switch_Active_Battle_Unit()
    |-> Begin_Combat_Turn()
    |-> Combat_Screen_Draw()
        |-> Combat_Grid_Entities__WIP()                             Combat-Combat_Screen_Map_Draw.md
        |-> Combat_Screen_Map_Draw()                           Combat-Combat_Screen_Map_Draw.md
        |-> Combat_Info_Effects_Count()
        |-> Tactical_Combat_Draw_Buttons()
        |-> Draw_Spell_Information_Window()
        |-> Draw_Combat_Unit_Display()
        |-> Draw_Active_Unit_Window()                               Combat-Combat_Figure_Compose.md
    |-> Auto_Cast_Spell_And_Do_Combat_Turn()
        |-> Combat_Cast_Spell()
        |-> Check_For_Winner()
        |-> Update_Combat_Enchantments_Icon_And_Help()
        |-> Auto_Do_Combat_Turn()
    |-> Check_For_Winner()
    |-> Next_Battle_Unit()
    |-> Assign_Combat_Grids()
    |-> Turn_Off_Auto_Combat()
    |-> BU_HasSpellAbility__WIP()
    |-> Add_Combat_Enchantment_Fields()
    |-> Move_Confused()
    |-> Battle_Unit_Action()
    |-> Battle_Unit_Set_Invisibility_Effect()
    |-> Get_Combat_Grid_Cell_X()
    |-> Get_Combat_Grid_Cell_Y()
    |-> Combat_Compose_Background()
    |-> End_Of_Combat__WIP()                                        Combat-End_Of_Combat.md

---


# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-08).** No findings open; five deviations. Thirty functions compared against 8,297 asm lines.

Twenty-two reconstruction errors were raised and all twenty-two are fixed. Every game target links — `momlib`, `ReMoM.exe`, `HeMoM.exe`, `MOX_tests.exe` — and every rename in the ledger below compiles. `MOM_tests` does not build, on a pre-existing missing-`Combat.h` break in four test files that predates this review and touches none of its functions; see Build state.

`Combat_Compose_Background` and `Generate_Combat_Map` were moved out of scope by decision and are not covered by this verdict.

`Combat_Screen` is the tactical-combat screen: it allocates the combat heaps, loads the graphics, builds the battlefield, seats the units, then runs the input loop until somebody wins or flees. The bundle below is that function plus everything it reaches during setup.

## Scope

The Production column carries the current name and line; the ASM column keeps the IDA names, which never change.

| Function (production) | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| `Combat_Screen` | [Combat.c:1408](../../MoM/src/Combat.c#L1408) | [ovr090/Combat_Screen__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr090/Combat_Screen__WIP.asm) (1724) | faithful |
| `Allocate_Combat_Base_Blocks` | [Combat.c:23807](../../MoM/src/Combat.c#L23807) | [ovr163/CMB_BaseAllocs__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr163/CMB_BaseAllocs__WIP.asm) (76) | faithful |
| `Combat_Screen_Load_Resources` | [Combat.c:7228](../../MoM/src/Combat.c#L7228) | [ovr103/CMB_LoadResources__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr103/CMB_LoadResources__WIP.asm) (386) | faithful |
| `Allocate_Combat_Near_Buffers` | [Combat.c:7356](../../MoM/src/Combat.c#L7356) | [ovr103/CMB_SetNearAllocs__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr103/CMB_SetNearAllocs__WIP.asm) (59) | faithful |
| `Cache_Graphics_Combat` | [LOADER.c:1832](../../MoM/src/LOADER.c#L1832) | [ovr052/Cache_Graphics_Combat.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr052/Cache_Graphics_Combat.asm) (13) | faithful |
| `Build_Battlefield` | [Combat.c:21132](../../MoM/src/Combat.c#L21132) | [ovr154/CMB_Terrain_Init__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr154/CMB_Terrain_Init__WIP.asm) (627) | faithful |
| `Prepare_All_Battle_Units` | [Combat.c:5044](../../MoM/src/Combat.c#L5044) | [ovr098/CMB_Units_Init__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr098/CMB_Units_Init__WIP.asm) (527) | faithful |
| `Battle_Unit_Has_Spell_Ability` | [Combat.c:3664](../../MoM/src/Combat.c#L3664) | [ovr098/BU_HasSpellAbility__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr098/BU_HasSpellAbility__WIP.asm) (73) | faithful |
| `NIU_Who_Has_More_Leadership` | [Combat.c:3695](../../MoM/src/Combat.c#L3695) | [ovr098/NIU_Who_Has_More_Leadership.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr098/NIU_Who_Has_More_Leadership.asm) (27) | faithful |
| `Load_Battle_Unit` | [COMBINIT.c:445](../../MoM/src/COMBINIT.c#L445) | [ovr116/Load_Battle_Unit.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/Load_Battle_Unit.asm) (286) | faithful |
| `Unit_Gold_Upkeep` | [CITYCALC.c:242](../../MoM/src/CITYCALC.c#L242) | [ovr120/Unit_Gold_Upkeep.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr120/Unit_Gold_Upkeep.asm) (140) | faithful |
| `Battle_Unit_Regular_Stats` | [COMBINIT.c:535](../../MoM/src/COMBINIT.c#L535) | [ovr116/BU_Init_Battle_Unit.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Init_Battle_Unit.asm) (478) | faithful |
| `Battle_Unit_Level_Stats` | [COMBINIT.c:893](../../MoM/src/COMBINIT.c#L893) | [ovr116/BU_Apply_Level.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Level.asm) (275) | faithful |
| `Battle_Unit_Hero_Skill_Stats` | [COMBINIT.c:1067](../../MoM/src/COMBINIT.c#L1067) | [ovr116/BU_Init_Hero_Unit.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Init_Hero_Unit.asm) (394) | faithful |
| `Battle_Unit_Item_Stats` | [COMBINIT.c:39](../../MoM/src/COMBINIT.c#L39) | [ovr116/BU_Apply_Item_Powers.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Item_Powers.asm) (467) | faithful |
| `Item_Powers_To_Unit_Enchantments` | [COMBINIT.c:135](../../MoM/src/COMBINIT.c#L135) | [ovr116/BU_Apply_Item_Enchantments.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Item_Enchantments.asm) (469) | faithful |
| `Item_Powers_To_Attack_Attributes` | [COMBINIT.c:224](../../MoM/src/COMBINIT.c#L224) | [ovr116/BU_Apply_Item_Attack_Specials.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Item_Attack_Specials.asm) (194) | faithful |
| `Apply_Enchantment_And_Mutation_Effects` | [COMBINIT.c:705](../../MoM/src/COMBINIT.c#L705) | [ovr116/BU_Apply_Specials.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Specials.asm) (509) | faithful |
| `Unit_Moves2` | [NEXTTURN.c:1530](../../MoM/src/NEXTTURN.c#L1530) | [ovr121/Unit_Moves2.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr121/Unit_Moves2.asm) (244) | faithful |
| `Unit_Hit_Points` | [COMBINIT.c:266](../../MoM/src/COMBINIT.c#L266) | [ovr116/Unit_Hit_Points.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/Unit_Hit_Points.asm) (216) | faithful |
| `Get_Combat_Grid_Cell_X` | [Combat.c:23326](../../MoM/src/Combat.c#L23326) | [ovr154/Get_Combat_Grid_Cell_X.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr154/Get_Combat_Grid_Cell_X.asm) (25) | faithful |
| `Get_Combat_Grid_Cell_Y` | [Combat.c:23335](../../MoM/src/Combat.c#L23335) | [ovr154/Get_Combat_Grid_Cell_Y.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr154/Get_Combat_Grid_Cell_Y.asm) (28) | faithful |
| `Deploy_Battle_Units` | [Combat.c:12280](../../MoM/src/Combat.c#L12280) | [ovr113/Deploy_Battle_Units.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr113/Deploy_Battle_Units.asm) (375) | faithful |

**Two header lines need correcting.**

- `BU_HasSpellAbility() ==> BU_HasSpellAbility__WIP()` is backwards, and no rename is needed either way — the IDA proc is *already* `BU_HasSpellAbility__WIP` (asm:2) and so is production. The `__WIP` suffix is inherited from the IDA database, not a marker somebody added here.
- `sub_7EC81() ==> NIU_Who_Has_More_Leadership()` is already done on both sides: the listing is `ovr098/NIU_Who_Has_More_Leadership.asm` and its proc is named that.

**Rename ledger** — IDA name on the left, production name on the right. The first two answer the `???` in the header block:

```
CMB_AIGoesFirst                ==>  m_cp_took_turn
CMB_ProgressTurnFlow__WIP      ==>  Combat_Next_Turn()
CMB_WizCastAvailable           ==>  m_wizard_cast_available
CMB_Projectiles@               ==>  _missiles
BU_SetVisibility__WIP          ==>  Battle_Unit_Set_Invisibility_Effect()
Combat_Node_type               ==>  Combat_Node_Type()
Bottom_GUI_Escape_Hotkey       ==>  escape_field
Grid_X / Grid_Y                ==>  grid_sx / grid_sy
IMG_CMB_ScanIcons@.Stat.Melee@ ==>  IMG_CMB_ScanIcons[]
GUI_String_1@                  ==>  GUI_String_1
IDK_base_cgc2 / IDK_base_cgc1  ==>  row_start_cgx / row_start_cgy
var_8                          ==>  niu_variable
Charm_of_Life_Hits             ==>  charm_of_life_bonus
hero_owner_idx                 ==>  unit_owner_idx
CMB_ATKR_First_CE              ==>  attacker_enchantment_first_shown
CMB_DEFR_First_CE              ==>  defender_enchantment_first_shown
m_unknown_variable__ovr090     ==>  niu_combat_screen_dirty
First_CE_Help_Entry            ==>  enchantment_help_entry_base
CE_Window_Left                 ==>  enchantment_strip_left_x
RightClick_X / RightClick_Y    ==>  right_click_cgx / right_click_cgy
BU_HasSpellAbility__WIP        ==>  Battle_Unit_Has_Spell_Ability()
Battle_Result                  ==>  end_of_combat_message_type
Ranged_Units / Melee_Units     ==>  ranged_battle_unit_idxs / melee_battle_unit_idxs
Ranged_Count / Melee_Count     ==>  ranged_battle_unit_count / melee_battle_unit_count
MoveFront_Count                ==>  move_to_front_count
Highest_Melee_Unit             ==>  highest_melee_ranged_slot
Highest_Melee                  ==>  highest_melee_strength
Loop_Var                       ==>  itr_ranged_slots
starting_cgc1 / starting_cgc2  ==>  starting_cgy / starting_cgx
something_x / something_y      ==>  cgx_subcell_offset / cgy_subcell_offset
```

Three of those correct a name rather than just expanding it, and the correction is the point:

- **`m_unknown_variable__ovr090` is write-only in the whole executable.** 19 references in `WIZARDS.asm` — the `dw 0` at 85366 and 18 stores of `1`. Zero reads. The `niu_` prefix is the only thing that tells the next reader those 18 assignments are dead.
- **`Highest_Melee_Unit` never held a unit.** It holds a slot within the ranged list, which is exactly what let the OGBUG at [Combat.c:12540-12543](../../MoM/src/Combat.c#L12540-L12543) hide.
- **`RightClick_X` / `RightClick_Y` are grid cells, not pixels.** They come out of `Get_Combat_Grid_Cell_X`/`_Y`, and the very next line subscripts `g_combat_grid_action_map` with them.

`CMB_DEFR_First_CE` was also the only `DEFR` spelling in the file; everything else uses `DFNDR` or `ATTKR`.

`CMB_ProgressTurnFlow__WIP` is the thunk label; the proc it lands on is `Combat_Next_Turn` in `ovr098`. The extractor emitted `CMB_ProgressTurnFlow__WIP.c` and `Combat_Next_Turn.asm` for the same function — the IDA database was renamed between the two passes, which is why neither name has both files.

**Evidence standard.** A divergence is recorded only where the bytes show a different computation, a different order, or a different operand width. Branch layout alone is not a finding — but a `jz`/`jnz` whose target changes *which* arm executes is, because that is the computation.

**Anchors.** Production links point at **function start lines**. `Combat.c` moved repeatedly during this review while it was open in the editor; every line number was re-verified against the file as it stands after the last change. For statement-level precision use the `asm:N` references, which are stable ground truth.

---

# Findings

None open.

# Deviations

Five, all deliberate and all still present. Rechecked against both the listings and the current source on 2026-08-08.

## D1 — `grid_sx` / `grid_sy` are 64-bit

[Combat.c:1427-1428](../../MoM/src/Combat.c#L1427-L1428). asm slots `Grid_X` / `Grid_Y` are `word ptr -0Ah` / `-0Ch`; production declares them `int64_t`, with the original `int16_t` declarations preserved directly above at [1439-1440](../../MoM/src/Combat.c#L11445-L11446), commented and marked `DOMSDOS`.

Platform change, not a reconstruction difference: [`Add_Grid_Field`](../../MoM/src/Combat.c#L1663) takes their addresses and the host writes pointer-width values through them. Every read casts back through `(int16_t)` — see [Combat.c:1863](../../MoM/src/Combat.c#L1841). Leave it.

## D2 — Swamp and Grasslands are split into two `else if`

[Combat.c:21993](../../MoM/src/Combat.c#L21993) and [22018](../../MoM/src/Combat.c#L21995), two consecutive arms with the same body.

`CMB_Terrain_Init__WIP.asm:519-527` is a single `||`: the swamp test `jz`s **into** the shared block at `loc_DFD03`, the grasslands test `jnz`s **past** it. One body, two entry points. The River arm at asm:532 is genuinely separate, so only these two are folded. Identical behaviour, different structure.

## D3 — the enchantment-paging loop condition is expanded into guard-breaks

[Combat.c:1778-1785](../../MoM/src/Combat.c#L1778-L1785) and again at [1800-1807](../../MoM/src/Combat.c#L1800-L1807) — the attacker and defender copies.

`Combat_Screen__WIP.asm:726-731` is a three-term `for` condition emitted at the bottom of the loop, in this order:

```
cmp     _SI_itr, [attacker_enchantment_count]
jge     short loc_7669B
cmp     _SI_itr, 4
jge     short loc_7669B
cmp     [attacker_enchantment_count], 4
jg      short loc_7665C
```

Production writes `for(itr = 0; itr < count; itr++)` with `if(itr >= 4) break;` and `if(count <= 4) break;` as the first two statements. Same three tests, same order, same result — and both copies are written the same way.

## D4 — the EMS mapper call is replaced by flat arithmetic

[Combat.c:21605](../../MoM/src/Combat.c#L21605) in `Claim_EMS_Page_For_Figure_Set`, and [Combat.c:21616](../../MoM/src/Combat.c#L21616) in `Map_Tile_EMS_Page_As_Sandbox`.

`EMM_FIGUREX_Init__HACK.asm:31-33` and `EMM_TILEX_Init__HACK.asm:5-8` both call `EMM_MapMulti4`. Production computes `EmmHndl_* + (logical_page * SZ_EMM_LOGICAL_PAGE)` instead and leaves the original call as a `TODO` comment on the same line. Consistent across both sites.

## D5 — the figure block is over-allocated

[Combat.h:588-590](../../MoM/src/Combat.h#L588-L590), the `FIGUREX_POINTER` macro, consumed at [Combat.c:24120](../../MoM/src/Combat.c#L24120).

`Combat_Figure_Load.asm:33-38` asks `Allocate_First_Block` for **2** paragraphs; the macro asks for **33**. The host needs more than 2 because `SAMB_ptr` is wider than a real-mode segment word, but 33 does not follow from anything written down and the comment beside it does not explain the number. **This is the one deviation with an open question against it.**

---

# Combat_Screen

## Frame map

`sub sp, 34h` (asm:41) = 52 bytes, plus two register locals. Nine parameters from `bp+6`.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-34h` | `Battle_Result` | `Battle_Result` |
| `bp-32h` | `defender_unit_count` | `defender_unit_count` |
| `bp-30h` | `human_player_did_flee` | `human_player_did_flee` |
| `bp-2Eh` | `winner` | `winner` |
| `bp-2Ch` | `auto_combat_cancel_ESC_field` | `auto_combat_cancel_ESC_field` |
| `bp-2Ah` | `cast_status` | `cast_status` |
| `bp-28h` | `temp_movement_points` | `temp_movement_points` |
| `bp-26h` | `temp_unit_enchantments` (dword) | `temp_unit_enchantments` |
| `bp-22h` | `active_unit_window_field` | `active_unit_window_field` |
| `bp-20h` | `space_hotkey_field` | `space_hotkey_field` |
| `bp-1Eh` | `info_button_field` | `info_button_field` |
| `bp-1Ch` | `flee_button_field` | `flee_button_field` |
| `bp-1Ah` | `auto_button_field` | `auto_button_field` |
| `bp-18h` | `done_button_field` | `done_button_field` |
| `bp-16h` | `wait_button_field` | `wait_button_field` |
| `bp-14h` | `spell_button_field` | `spell_button_field` |
| `bp-12h` | `RightClick_Y` | `RightClick_Y` |
| `bp-10h` | `RightClick_X` | `RightClick_X` |
| `bp-0Eh` | `combat_grid_field` | `combat_grid_field` |
| `bp-0Ch` | `Grid_Y` | `grid_sy` — see D1 |
| `bp-0Ah` | `Grid_X` | `grid_sx` — see D1 |
| `bp-8` | `Bottom_GUI_Escape_Hotkey` | `escape_field` |
| `bp-6` | `screen_changed` | `screen_changed` |
| `bp-4` | `leave_screen` | `leave_screen` |
| `bp-2` | `input_field_idx` | `input_field_idx` |
| `si` | `_SI_itr` | `itr` |
| `di` | `_DI_battle_unit_idx` | `battle_unit_idx` |
| — | — | `did_win` — no slot; see below |

`hotkey_idx_Z` and `hotkey_idx_T` are inside `#ifdef STU_DEBUG` and correctly absent from the release frame.

**`did_win` has no frame slot, and that proves nothing.** `sub sp, 34h` is 52 bytes and the 25 named slots consume all 52, so `did_win` is not on the stack. But the asm's return path (asm:1708-1717) never stores the value anywhere — it materialises `ST_TRUE` or `0` straight into `AX` and falls to `retf`. A local assigned once and immediately returned is exactly what a compiler keeps in `AX`, so the absent slot is equally consistent with the source having declared it. Both `return ST_TRUE; ... return ST_FALSE;` and `did_win = ...; return did_win;` compile to these bytes; the disassembly cannot tell them apart.

## Faithful — verified, leave alone

- **The city-siege test is `_combat_environ == 1` AND defender not neutral** (asm:48-52), and `_computer_player_city_seige` is cleared first so the guard only ever sets it.
- **`Switch_Active_Battle_Unit` is called twice with the same operands, before and after the first `Begin_Combat_Turn`** (asm:240 and asm:263). The original really does re-seat the active unit; asm:262's comment that nothing above changes the focus unit is why.
- **The defending-AI double turn is preserved.** asm:271-278 calls `Auto_Cast_Spell_And_Do_Combat_Turn` then `CMB_PrepareTurn__WIP` again, then sets `CMB_AIGoesFirst`.
- **The attacker-side seat call passes `_human_player_idx`, not a literal** (asm:290), and the `else` arm passes `_combat_total_unit_count - defender_unit_count` (asm:294-297).
- **The end-of-turn block's condition is a genuine `||` over an `&&`** — asm:585-602 is `(movement_points < 1 && action != bua_Finished) || status > bus_Active`, with `jge`/`jg` both signed.
- **`mana` is unsigned and `item_charges` is signed** — asm:381 is `jnb` and asm:393 is `jg`, two branch families on two adjacent byte fields of the same struct.
- **The Spell button's condition is `(mp > 0 && (mana >= 3 || hasSpell || charges > 0)) || wizCastAvailable`** — asm:373-396 reaches `loc_762F6` from two paths, and `CMB_WizCastAvailable` is on both. The click handler mirrors it.
- **Confusion state 2 swaps the controller between attacker and defender** (asm:627-640), and state 1 with movement left routes to `Move_Confused` — two separate blocks, not an `if`/`else`.
- **The enchantment paging wraps at `> 8`, not `>= 8`** — asm:717-719 is `cmp 8` / `jle` / `mov 0`, so 0, 4 and 8 are all valid first-icon offsets. Both attacker and defender copies.
- **Both right-click stats windows copy `battle_units[battle_unit_idx]`** (asm:1027-1031, asm:1188-1192) while the third site copies `_active_battle_unit` (asm:1551). Three sites, two different indices, all correct.
- **Both right-click branches restore `movement_points` with a plain store**, not an add — asm:1065-1066 and asm:1226-1227 are `mov al, [byte ptr bp+temp_movement_points]` / `mov [...], al`.
- **The tail redraw requires both `leave_screen` and `screen_changed` clear** (asm:1622-1625), so the handlers that set `screen_changed` suppress the extra `Combat_Screen_Draw` + `PageFlip_FX` + `Release_Time(1)`.
- **`Battle_Result` is a five-way ladder in a fixed order** (asm:1668-1689): computer fled → 5, human won → 1, human fled → 3, `_combat_turn > 50` → 4, else 2. The turn test is `cmp 50` / `jle`, so strictly greater.
- **The return is `winner == _combat_attacker_player`** (asm:1708-1717), not "human won" — a defender-side human battle returns `ST_FALSE` on a win.
- **`Clear_Palette_Changes(0, 255)` then `Set_Palette_Changes(0, 223)`** (asm:1641-1652) — two different upper bounds, and the entry path used 243 (asm:226). Three distinct constants, all reproduced.
- **`Combat_Unit_Statistics_Window` takes `2` as its seventh argument at all three sites** (asm:1044, asm:1535, and the middle block); `uvt_Cmbt` is 2 ([UnitView.h:49-53](../../MoM/src/UnitView.h#L49-L53)), so all three are faithful.

# Allocate_Combat_Base_Blocks

Faithful. Production [Combat.c:24110](../../MoM/src/Combat.c#L24110).

Four far allocations then five near ones: 348, 482, 10, 9, then `Near_Allocate_First(504)` and `Near_Allocate_Next` of 504, 1008, **60**, **60** (asm:5-73). It opens with `Allocate_First_Block`, which **resets** the screen segment's allocation chain — everything afterwards, here and through `Combat_Screen_Load_Resources`, carves with `Allocate_Next_Block`. The two are strictly ordered and calling the second without the first carves from wherever the previous screen left the pointer.

drake178's note preserved at [Combat.c:24117](../../MoM/src/Combat.c#L24117) is right that the near block is redundant *on the `Combat_Screen` path* — `Combat_Screen_Load_Resources` runs next and its tail call re-issues `Near_Allocate_First`. It is **not** redundant on the other path: `NX_IDK_CombatInit_Tactical` calls this function and then `Build_Battlefield` with no resource load between them, so those five pointers are the ones that survive.

IDA's `CMB_Projectiles@` is production's `_missiles`; the sizes match and only the name differs.

# Combat_Screen_Load_Resources

Faithful. Production [Combat.c:7297](../../MoM/src/Combat.c#L7297).

- **The allocation sequence is 249, 6, 6, 6, 3, then 55 × 18** — nothing else (asm:7-50, asm:99-109, asm:363-376).
- **Fifty-nine COMPIX.LBX entries across 32 call sites**, three of them loops. Every load targets `compix_lbx_file__ovr103`; nothing else is opened.
- **The lock-button LBX indices are not sequential** and production reproduces the jumble exactly: cancel 22, spell 23, wait 24, **done 28**, auto 26, **info 25**, flee 27 (asm:240-295). Easy to "tidy" into 23-28 order; do not.
- **Fifteen combat-enchantment icons** from LBX entries 5-15, then 41, 60, 79, 80 (asm:120-239) — the four out-of-run indices are correct.
- **`combat_enchantments` is allocated 3 paragraphs and then zeroed for 30 bytes** (asm:99-119). 3 paragraphs is 48 bytes; only the first 30 are cleared, which is the fifteen `{Attkr, Dfndr}` pairs.
- **`Mark_Block` / `Release_Block` bracket only the `battle_unit_picts_seg` loop** (asm:359, asm:378), not the LBX loads above it. Those 18 blocks are *reserved*, not loaded — the figure compositor fills them later, and the mark is what lets combat rewind to that point.
- **The picts loop bound is `e_MAX_BATTLE_UNIT_COUNT`** = `12h` = 18 in `WIZARDS.inc`, matching production's `MAX_BATTLE_UNIT_COUNT` ([MOM_DEF.h:65](../../MoX/src/MOM_DEF.h#L65)).

# Allocate_Combat_Near_Buffers

Faithful. Production [Combat.c:7356](../../MoM/src/Combat.c#L7356).

Seven allocations then a 22-iteration loop then one more: 504, 504, 1008, 60, 60, 12, `22 ×` 21, 20. The loop literals settle the grid constants outright — asm:49 is `cmp _SI_itr, 22` and asm:40 is `mov ax, 21`, matching `COMBAT_GRID_HEIGHT` 22 and `COMBAT_GRID_WIDTH` 21 ([Combat.h:238-239](../../MoM/src/Combat.h#L238-L239)). Only the first call is `Near_Allocate_First`; all others are `Near_Allocate_Next`.

It is called five times, not once: the tail of `Combat_Screen_Load_Resources`, and four times inside the input loop after each modal sub-window returns. `Near_Allocate_First` resets a single shared 4400-byte DGROUP arena that every screen in the game re-carves for itself, so combat has to re-establish its layout whenever it gets control back.

# Build_Battlefield

Faithful. Production [Combat.c:21787](../../MoM/src/Combat.c#L21787).

- **`Location_Type` is resolved by four independent passes, each overwriting the last** — cities, then towers, then nodes, then lairs (asm:54-374). A square that is both a tower and a lair ends up a lair. No `else`, no early exit.
- **The node arm subtracts, the lair arm adds** — asm:316-318 is `mov dx, 7` / `sub dx, ax`, asm:368 is `add ax, 4`. Production writes these as `clt_SorceryNode - _NODES[itr].type` and `clt_Tower + _LAIRS[itr].type`, which is the same arithmetic with the constants named. Against `nt_ {Sorcery=0, Nature=1, Chaos=2}` the subtraction yields 7, 6, 5 — the `clt_` enum is ordered to make that work ([Combat.h:686-688](../../MoM/src/Combat.h#L686-L688)).
- **The house-style swap is two-way** — asm:159-166 tests `1` in the first arm and `2` in the second, with `jmp short loc_DF985` at asm:162 forming the `else`. The `rht_` constants ([RACETYPE.h:43-45](../../MoM/src/RACETYPE.h#L43-L45)) name what the literals mean; the swap reconciles the race table's ordering with the combat sprite bank's.
- **The city-enchantment copy bound is inclusive.** asm:233-241 recomputes `&_CITIES[itr].NIGHTSHADE` each pass and loops while `src <=` it (`jnb`, unsigned). In `WIZARDS.inc`, `CTY_ENCH` is `sizeof=0x19` (25 named bytes) and `NIGHTSHADE db ?` is a *separate* 26th byte declared after it in `s_CITY`; production folds both into one `enchantments[NUM_CITY_ENCHANTMENTS]` with `NIGHTSHADE` as index 25 ([MOM_DEF.h:1390](../../MoX/src/MOM_DEF.h#L1390)), and `s_BATTLEFIELD` folds `city_enchantments CTY_ENCH` + `Nightshades db` the same way ([Combat.h:1044](../../MoM/src/Combat.h#L1044)). So the loop copies 26 bytes.
- **The copy's destination is zeroed for 30 bytes at entry** (asm:36-50), four more than the 26 copied, and reuses the same pointer local the asm reuses at slot `-20h`.
- **The enchantment bytes are signed.** Not provable from the copy loop — a byte-to-byte move through `AL` carries no sign information — but `ovr093/Resolve_Wizard_Conquest.asm:102-107` reads the same array with `cmp byte, 0` / `jle` and then `mov al` / `cbw`, both signed forms. Production types it `int8_t` ([MOM_DAT.h:1765](../../MoX/src/MOM_DAT.h#L1765)).
- **The fortress scan runs only for populated cities** and sets `Location_Type = 3` plus a flag at `bp-18h` that is written and never read (asm:139-140). It has a real frame slot, so it is faithful-and-unused rather than phantom.
- **The tower test checks `wx` and `wy` but not `wp`** (asm:254-273) — unlike the city, node and lair tests, which all check all three. Faithful; towers exist on both planes at the same coordinates.
- **The road scan writes a 3×3 neighbourhood with a bounds check per cell** (asm:403-453), indexing the flat 9-element array by `(dy+1)*3 + (dx+1)`. The original bases at `&Road_Matrix[1]` with `(dy+1)*3 + dx` — same nine slots, same order.
- **The river array is never written.** Zeroed at asm:387-398 and passed to `Generate_Combat_Map`, with no scan anywhere that fills it. Its consumer, `Carve_River_Terrain`, is also unwritten.
- **`CMB_StoneWallType` is set to 0 by Mountains and Desert only** (asm:500, asm:511), then unconditionally to 1 when `wp == 1` (asm:558-560) — so the plane check overrides both, and an Arcanus battle on any other terrain inherits whatever the previous battle left. It is `dw 0` in `dseg` and written nowhere else.
- **The Nature-node terrain override is vestigial but not dead** (asm:554-556). `Square_Is_Forest` already accepts `tt_NatureNode` ([Terrain.c:944](../../MoM/src/Terrain.c#L944)), so `cts` is `cts_Forest` before the override runs. It executes on every Nature node battle and changes nothing. Faithful — keep it.
- **The RNG is saved and restored around map generation** (asm:562-564, asm:616-620), so terrain generation is deterministic from position and consumes nothing from the live sequence. The seed depends on the *product* `wx * wy`, so all coordinate pairs with the same product generate identical trees, rocks and houses, and the two planes differ by only `wx * wy` — making row 0 and column 0 identical across planes.
- **`_vortex_count = 0` happens after `Generate_Combat_Map` and before the seed is restored** (asm:615).
- **`Generate_Combat_Map` is gated on `RP_CMB_NoCombatMap == ST_FALSE`** (asm:594) and takes twelve arguments; `add sp, 18h` confirms the count. That flag is `dw 0` in `dseg` and written nowhere in the binary, so map generation is unconditional in practice.

# Prepare_All_Battle_Units

Faithful. Production [Combat.c:5041](../../MoM/src/Combat.c#L5041).

Clears all 36 battle-unit slots, loads the attacker's troop list, scans `_UNITS` for defenders standing on the combat square, resolves the four player-identity globals, resets the per-battle fields on every seated unit, deploys both sides, and sets each wizard's combat skill pool. Returns the defender count, which `Combat_Screen` uses to seat the first defender.

**Frame: no stack locals at all.** There is no `sub sp` — only the two register locals `_SI_itr` and `_DI_count` (asm:9-10). Production declares exactly `itr` and `count`, so there is nothing phantom.

- **The slot-clear loop bound is 36** (asm:28), matching `MAX_BATTLE_UNIT_SLOT_COUNT` ([MOM_DEF.h:67](../../MoX/src/MOM_DEF.h#L67)) — not the 18 that `Combat_Screen_Load_Resources` uses for the picture buffers.
- **The two `Combat_Figure_Load` calls take different second arguments**, and the push order proves it: the attacker side pushes `itr` first (asm:71-84) → `(type, itr)`; the defender side pushes `_combat_total_unit_count` first (asm:247-256) → `(type, _combat_total_unit_count)`. They are only equal while the attacker loop runs.
- **The attacker's `us_Ready` guard covers two writes**, `Status` and `Rd_Constr_Left` (asm:111-133), and fires when the controller is *not* the human. `e_ST_UNDEFINED_DB` is `0FFh`, which is `−1` in the `int8_t` `Rd_Constr_Left` ([MOM_DAT.h:1866](../../MoX/src/MOM_DAT.h#L1866)).
- **The defender scan matches four fields** — `wx`, `wy`, `wp`, `owner_idx` — and sets `us_Ready` *before* `Load_Battle_Unit`, not after (asm:195-222).
- **`controller_idx` is overwritten on both sides** even though `Load_Battle_Unit` already set it from `_UNITS[].owner_idx` (asm:100-101, asm:272-273). Faithful; the production comment noting the redundancy is correct.
- **Four `MOO_MONSTER_PLAYER_IDX` fallbacks in a fixed order** (asm:283-299): defender, attacker, then the AI-player assignment, then the AI-player fallback. The third is a plain assignment, not a fallback, and sits between two that are.
- **The human/computer assignment swaps both operands in the `else` arm** (asm:301-313) — `_combat_human_player` takes the attacker and `_combat_computer_player` the defender, or the reverse.
- **Fifteen fields are reset per unit, in asm order** (asm:316-406), and `Confusion_State` and `Unknown_57h` are deliberately stepped over — they sit between `action` and `gibs` in the struct. Three of the fifteen are renamed in production; `WIZARDS.inc` gives the tail as `Unknown_5A`, `Always_Animate`, `Melee_Anim`, `Image_Effect`, `Move_Bob`, and [Combat.h:1272-1276](../../MoM/src/Combat.h#L1272-L1276) has them at 0x5A, 0x5C, 0x5E, 0x60, 0x62 as `Unknown_5A`, `animate_idle`, `Melee_Anim`, `figure_effect`, `animate_move_as_idle` — same sequence, so the renames map to the right offsets.
- **The city-siege exclusion needs all three of `Transport > 0`, `MV_SAILING` set, and `MV_FLYING` clear** (asm:424-439, `jle` / `jz` / `jnz`).
- **The defender's skill branch tests `MOO_MONSTER_PLAYER_IDX` first** (asm:485), so the out-of-bounds `_players[]` write is reached only on that path. The attacker's has no such guard.

## OGBUGs — faithful, do not fix

- **Transport units with Wraith Form are excluded from defending a city.** The exclusion tests `MV_SAILING` and `!MV_FLYING`, so a ship that gained flight is correctly spared — but one that gained *swimming* is not, because the comparison value never included it.
- **The neutral defender's skill write is out of bounds.** asm:514-519 indexes `_players[]` by `MOO_MONSTER_PLAYER_IDX` to store zero. Production preserves it with the `OGBUG OOB` note.

# Load_Battle_Unit

Faithful. Production [COMBINIT.c:445](../../MoM/src/COMBINIT.c#L445).

Seats one `_UNITS` entry into a `s_BATTLE_UNIT`: copies the unit-type record over the top of it, zeroes about thirty per-battle fields, computes upkeep and poison strength, calls `Battle_Unit_Regular_Stats`, then derives figure count and front-figure damage from accumulated damage.

- **The `memcpy` source is `&_unit_type_table[type].Melee`, not the record start**, and the size is the full `sizeof(s_UNIT_TYPE)` (asm:14-31). It begins at offset 2 and copies 36 bytes, so it reads two bytes past the end of the record. Faithful — the push order and the `mov ah, 0` on the `uint8_t` type both match.
- **The figure math is two separate `idiv`s, not one** (asm:147-183). `front_figure_damage` takes the remainder from `dl`; `figure_cnt` takes `Max_Figures` minus the quotient. Both operands are `cbw`-widened `int8_t`.
- **`Unknown_5A = 0` really is emitted twice** (asm:191 and asm:193). Production preserves both, with the note.
- **Field renames verified by offset** against `WIZARDS.inc`'s `s_BATTLE_UNIT`: `Always_Animate` to `animate_idle` (0x5C), `Image_Effect` to `figure_effect` (0x60), `Move_Bob` to `animate_move_as_idle` (0x62), `Max_Figures` to `figure_max` (0x13). `bus_Active` and `bua_Ready` are both 0, matching asm:78 and asm:203.
- **`controller_idx` is a same-width byte move** (asm:84-86, no `cbw`), so it carries no signedness evidence either way.

## OGBUG — faithful, do not fix

**Only the first hero item's embedded spell is ever usable.** asm:240 indexes `_players.Heroes.Items` with no `shl` on any counter — there is no loop. The other two item slots are never consulted for charges.

# Unit_Gold_Upkeep

Faithful. Production [CITYCALC.c:242](../../MoM/src/CITYCALC.c#L242).

- **`type > ut_Chosen` is unsigned** — asm:41-42 is `jbe`, matching the `uint8_t` `s_UNIT.type`.
- **The two difficulty adjustments are sequential `if`s, not `else if`** — asm:116 `jnz loc_9F6EE` falls into the Hard test. Impossible halves via `cwd / sub ax,dx / sar ax,1`; Hard is `imul 3` then `idiv 4`.
- **`HERO_NOBLE` resolves correctly**: `HSA_NOBLE` is 0x20000000, whose high word 0x2000 is asm:75's `HSA_HI_NOBLE`, and the test is `or dx,ax / jnz` — a plain not-equal-zero.

# Battle_Unit_Regular_Stats

Faithful. Production [COMBINIT.c:535](../../MoM/src/COMBINIT.c#L535).

## Frame map

`sub sp, 0Ah` (asm:13) = 10 bytes, plus two register locals.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-0Ah` | `weapon_quality` | `weapon_quality` |
| `bp-8` | `var_8` | `niu_variable` |
| `bp-7` | `unit_mutations` | `unit_mutations` |
| `bp-6` | `battle_unit_enchantments` (dword) | `battle_unit_enchantments` |
| `bp-2` | `chaos_surge` | `chaos_surge` |
| `si` | `_SI_unit_idx` | `unit_idx` |
| `di` | `_DI_itr_players` | `itr_players` |

**`niu_variable` is a real slot with no uses.** `var_8` appears exactly once in all 478 lines — the frame declaration. No read, no write. The slot is accounted for by `sub sp, 0Ah`, so the declaration is faithful; only the `= 0` initializer has no counterpart.

- **The weapon-quality tail is outside the ranged block and inside the greater-than-zero guard.** asm:252 skips to `loc_98991`, which is where `defense`, `Gold_Defense` and `Weapon_Plus1` are written (asm:271-287). The indentation in production is misleading; the nesting is right.
- **`race > rt_Troll` is signed** — asm:410 is `jle`.
- **`BU_CASTER_40()` / `BU_CASTER_20()` / `BU_LUCKY()` take no arguments** and bind the enclosing `battle_unit` textually ([Combat.h:600-602](../../MoM/src/Combat.h#L600-L602)). They resolve to the parameter here, matching asm:117/123/136.

## OGBUG — faithful, do not fix

**Chaos Surge is read from the wrong player.** asm:293 is `cmp [_players.Globals.CHAOS_SURGE], 0` with no `+bx` — index 0, the human player — while the loop it sits inside iterates `itr_players`. Production preserves it with the note.

# Battle_Unit_Level_Stats

Faithful. Production [COMBINIT.c:893](../../MoM/src/COMBINIT.c#L893).

Applies per-level stat bumps to a hero. Reads the **stored** `_UNITS[].Level` field, floors it at 3 for Heroism, then bumps `resist`, `tohit`, `melee`, `ranged` and `defense` on a fixed schedule.

**The function comment is accurate and the omissions are real.** There is no reference to Crusade, Warlord, `XP` or `TBL_Experience` anywhere in the 275 lines. Three things the overland path does that this one does not:

| | overland `Calc_Unit_Level` | here |
| --- | --- | --- |
| derive level from XP | walks `TBL_Experience` | reads the stored `.Level` |
| Warlord +1 | yes | no |
| Crusade +1 | yes | no |

The Heroism *ordering* is the sharpest part: overland the floor is applied and Warlord/Crusade then stack on top, so a Heroism unit under both reaches 5; here the floor is 3 and nothing stacks. `Battle_Unit_Hit_Points` applies the Crusade bonus but not Warlord, so all three paths disagree with each other. OGBUG, faithful, leave it.

# Battle_Unit_Hero_Skill_Stats

Faithful. Production [COMBINIT.c:1067](../../MoM/src/COMBINIT.c#L1067).

- **All nine `HSA_` masks verified** against `WIZARDS.inc:5647-5671`, including the low/high split: `BLADEMASTER`, `BLADEMASTER2`, `MIGHT` and `CONSTITUTION` are low-word; `MIGHT2`, `ARCANE_POWER`, `ARCANE_POWER2`, `AGILITY`, `AGILITY2`, `LUCKY` and `NOBLE` are high-word.
- **The mana formula matches operand for operand** — asm:320-349 computes `(Casting_Skill + 1) * (Level + 1)`, then `* 5`, then the `cwd / sub / sar` halve. The multiply by 5 really does precede the divide by 2.
- **No `sub sp`.** Both locals are register-allocated, so the frame gives no declaration-order signal.

# Battle_Unit_Item_Stats, Item_Powers_To_Unit_Enchantments, Item_Powers_To_Attack_Attributes

All three faithful. Production [COMBINIT.c:39](../../MoM/src/COMBINIT.c#L39), [135](../../MoM/src/COMBINIT.c#L135) and [224](../../MoM/src/COMBINIT.c#L224).

- **`mana_max > 0` is unsigned** — `cmp byte,0` / `jbe` (asm:117-119), matching the `uint8_t` field.
- **The five-way disjunction reproduces the Bow arm's double exit.** asm:286 and asm:294 both fall through to the Staff test; only asm:295 jumps into the body. That arm is the one a transcription would silently flatten.
- **`s_ITEM.enchantments` sits at offset 46 = 0x2E**, so `ITEM_OFS_POWERS` and `GET_4B_OFS` read the right four bytes.
- **Test order differs between the two attack blocks and both are right**: melee tests `type <= it_Axe` or `type == it_Misc` (asm:176-186, `jle` signed); ranged tests Bow/Missile, Staff/Magic, Wand/Magic, Axe/Thrown, Misc in that sequence.
- p02 runs 20 power tests and p03 runs 9, each in exact asm order.

**Note — the original treated `Powers` as a signed `long`.** Every one of the 33 `ITEM_POWER` sites compiles to a signed 32-bit greater-than-zero test (`or ax,ax / jl / jg / or dx,dx / jbe`), with one exception: `IP_Invulnerability` at `BU_Apply_Item_Enchantments.asm:423-427` uses `jb`/`ja`. It has to — `ip_Invulnerability` is bit 31, so a signed test would read negative and the enchantment would never apply. Production's single `ITEM_POWER()` macro tests not-equal-zero on a `uint32_t`, which is exactly right for Invulnerability and behaviourally identical for every other mask. No change; recorded because it explains why the listing looks the way it does.

# Apply_Enchantment_And_Mutation_Effects

Faithful. Production [COMBINIT.c:705](../../MoM/src/COMBINIT.c#L705).

- **Three ranged-type test orders, all different, all reproduced.** Flame Blade tests `srat_Thrown` then `rag_Missile` (asm:241-249); Eldritch Weapon tests `rag_Missile` then `srat_Thrown` (asm:329-337); Lionheart is a three-way `rag_Missile`, `rag_Boulder`, `srat_Thrown` (asm:411-427).
- **Iron Skin / Stone Skin is the only true `else if`** — asm:177's `jmp short loc_98DDC` skips the second test.
- **Berserk's melee guard covers only two statements** (asm:485-497). `Grey_Defense = defense` and `defense = -20` sit outside it at asm:499-504.
- **The `race == rt_Death` immunity is positioned after everything that can set `rt_Death`** (asm:470), including the Undead mutation and Black Channels. The position is load-bearing.
- **Each of the three Chaos Channels mutations also sets `race = rt_Chaos`** (asm:354-386).
- `rat_None` at `WIZARDS.inc:6911` is **255** — a different symbol from `rat_NONE = 0` at 6890 — so as an `int8_t` it is minus one, and the `rat_UNDEF` test at asm:114 is correct.

# Unit_Moves2

Faithful. Production [NEXTTURN.c:1530](../../MoM/src/NEXTTURN.c#L1530).

- Constants verified: `moves2_base` is offset 0x09 = `Move_Halves`; `UE_ENDURANCE` 0x2000 low-word, `UE_FLIGHT` 0x00020000 high-word, `CC_FLIGHT` 8, `ip_Endurance` index 13 = `IP_Endurance` 0x2000; `Globals[WIND_MASTERY]` 0x04 matches `s_OVERLAND_ENCHANTMENTS` offset 4.
- **Wind Mastery is indexed by `itr_players`** (asm:196-199), unlike the Chaos Surge loop in `Battle_Unit_Regular_Stats`. That loop is not buggy.
- The frame's five stack locals at `-0Ch, -8, -6, -4, -2` appear in production in exactly that sequence.

# Unit_Hit_Points

Faithful. Production [COMBINIT.c:266](../../MoM/src/COMBINIT.c#L266).

**The level-bump split is the part that would fail silently, and it is exact.** Levels 0, 1 and 3 plus the two CONSTITUTION tests sit inside the hero block; levels 2, 4, 5, 6 and 7 sit outside it. asm:165 `loc_9809A` — where the non-hero `jmp` at asm:81 lands — is between the level-3 and level-2 tests.

Charm of Life is `hit_points / 4`, floored at 1, then added (asm:198-207).

# Get_Combat_Grid_Cell_X / Get_Combat_Grid_Cell_Y

Both faithful. Production [Combat.c:23498](../../MoM/src/Combat.c#L23498) and [Combat.c:23507](../../MoM/src/Combat.c#L23507).

These are the algebraic inverse of the grid-to-screen transform at [Combat.c:23521-23522](../../MoM/src/Combat.c#L23521-L23522). With `u = (screen_x - 158) / 2` and `v = screen_y + 80`:

```
u = 8 * (cgx - cgy)      cgx = (u + v) / 16
v = 8 * (cgx + cgy)      cgy = (v - u) / 16
```

The `-158` and `+80` translate the screen origin to the grid origin; the `/2` normalizes the 2:1 isometric aspect so the rest is a plain 45-degree rotation; the `/16` is 16 normalized units per cell.

- **The two listings together prove the grouping.** Y saves the halved term and restores it (asm:14-18, `push ax` then `pop dx` then `sub ax, dx`), which is what Borland emits for a genuine grouped operand. X has no `push`/`pop` — asm:13-15 accumulates — which is why the source is `+ screen_y + 80` and not `+ (screen_y + 80)`.
- **The `Nope.` in the header comment is wrong, not the formula.** The worked example (78, 73) resolves to cell (7, 12), and the forward transform confirms it: `16 * (7 - 12) + 158 = 78` and `8 * 19 - 80 = 72`, one pixel above the sampled row. `cgy` comes out 12 from the same inputs.
- **Both divisions truncate toward zero.** For the `/16` that never matters on a 320x200 screen — `u+v` and `v-u` both stay non-negative, since `v` is at least 80 and `u` bottoms out at -79. The `/2` is different: `screen_x - 158` is negative across the whole left half of the map, so odd pixel columns there round the opposite way from their mirror on the right. A one-pixel seam, symmetric about the centre line.

# Combat_Structure

Production [Combat.c:16985](../../MoM/src/Combat.c#L16985). **One finding — R18.** Everything else faithful.

- `Square_Is_Sailable(wx, wy, wp)` argument order confirmed by push order (asm:14-17); `cs_NONE` is 0, matching `xor si,si`.
- The two city-enchantment writes are `> 0` tests (`jle`) storing the literal 2 into the attacker/defender pair (asm:30-43).
- The three node tests are consecutive `if`s, not `else if` (asm:66-76), so a later match overwrites an earlier one.

**Deviation — `_combat_environ == 1`.** [Combat.c:17017](../../MoM/src/Combat.c#L17017) uses the literal where asm:23 names `cnv_Enemy_City`, which [Combat.h:652](../../MoM/src/Combat.h#L652) defines as 1. Same value; the enum name belongs there.

# Undeployable_Battle_Units_On_Water

Production [Combat.c:17036](../../MoM/src/Combat.c#L17036). **Three findings — R19, R20, R21.**

The rest is faithful: two separate passes over `_combat_total_unit_count`, the first bailing with `return 0` and the second setting `bus_Uninvolved` and counting. The second pass's four-term guard — `controller_idx == player_idx`, `carry_capacity == 0`, `!MV_FLYING`, `!MV_SWIMMING` — matches asm:90-114 term for term and in order.

# Combat_Grid_Screen_Coordinates

Production [Combat.c:23445](../../MoM/src/Combat.c#L23445). **One finding — R22.**

The arithmetic itself is exact: `(cgx - cgy) * 16 + 158` and `(cgx + cgy) * 8 - 80` via `shl 4` / `shl 3` (asm:17-28), then `+= (something_x - something_y) * 2` and `+= (something_x + something_y)` (asm:29-35). This is the forward transform that [`Get_Combat_Grid_Cell_X`](../../MoM/src/Combat.c#L23498) inverts.

# Load_Combat_Terrain_Pictures

Faithful. Production [Combat.c:23688](../../MoM/src/Combat.c#L23688).

- **The `cts_Water` early return is a bare zero test** — asm:19 is `or di,di / jnz`, and `cts_Water` is 0.
- **Five terrain arms, four filename pairs.** Plains, Hills and Forest share one arm (asm:24-29); Desert, Mountains and Tundra get their own. Every arm picks on `wp == 0`, and `ARCANUS_PLANE` is 0.
- **Three sequential load loops with the right bases** — 48 entries into `_combat_terrain_pict_segs` from index 0, then 5 into `IMG_CMB_Trees` from 48, then 5 into `IMG_CMB_Rocks` from 53 (asm:103-154).
- **The filename buffer is 30 bytes** in both: asm puts it at `bp-20h` with `temp_seg` at `bp-2`, and `LEN_STRING` is 30 ([MOX_BASE.h:27](../../MoX/src/MOX_BASE.h#L27)). The 24 zero initializers in production are a partial initializer; C zero-fills the rest.

The asm returns 0 in `ax` on the water path (asm:21) while the normal exit leaves `ax` undefined, so the `void` return in production is compatible with the bytes. Nothing to change.

# Claim_EMS_Page_For_Figure_Set and Map_Tile_EMS_Page_As_Sandbox

Both faithful. Production [Combat.c:21599](../../MoM/src/Combat.c#L21599) and [Combat.c:21614](../../MoM/src/Combat.c#L21614), against `ovr153/EMM_FIGUREX_Init__HACK.asm` (77) and `ovr153/EMM_TILEX_Init__HACK.asm` (41).

These are two different functions and the earlier draft of this doc paired the wrong listing with the wrong name. `FIGUREX_MAP` ([Combat.h:583](../../MoM/src/Combat.h#L583)) expands to `Claim_EMS_Page_For_Figure_Set`, which is `EMM_FIGUREX_Init__HACK`; `EMM_TILEX_Init__HACK` is the no-argument `Map_Tile_EMS_Page_As_Sandbox`, called from `Load_Combat_Terrain_Pictures` and `Combat_Compose_Background`.

- `logical_page = (bufpi * 3) / 2` — `imul 3` then the signed-halve idiom (asm:13-18).
- The four `farpokew` calls match `SET_2B_OFS` in order and in argument order: MemSig1, MemSig2, size, used. Sizes differ between the two functions and both are right — **1591** paragraphs for the figure page (asm:54), **3071** for the tile page (`EMM_TILEX_Init__HACK.asm:25`).
- The figure-page pokes are all at `EMS_PFBA + offset`; the tile-page pokes are at bare `EMS_PFBA`.

**Deviation D5 — the EMS mapper call is replaced by flat arithmetic.** asm:31-33 and `EMM_TILEX_Init__HACK.asm:5-8` both call `EMM_MapMulti4`; production computes `EmmHndl_* + (logical_page * SZ_EMM_LOGICAL_PAGE)` and leaves the original call as a `TODO` comment. Deliberate host substitution, consistent across both functions.

# Combat_Figure_Load

Faithful. Production [Combat.c:24110](../../MoM/src/Combat.c#L24110).

- **The filename is built in four steps and the plural test is `< 10`** — asm:71-72 is `cmp ax, 10 / jge`, so `str_figure_plural_s__ovr163` is appended only below ten. `itoa`, `strcpy`, the conditional `strcat`, then `strcat` of the buffer, then `strcat` of the empty string (asm:47-94).
- **`(unit_type / 15) + 1` is computed three separate times** in the asm (asm:51-55, 66-70, 95-98) rather than cached, and production writes it out three times too.
- **`entry_num = (unit_type % 15) * 8`** — the same `idiv` taking the remainder from `dx`, then `shl dx, 3` (asm:95-101).
- **The load loop is 8 wide** and stores word-sized segments (asm:102-123).
- **`Combat_Figure_Compose(bufpi, 0, 0, 0, 0)`** is present and the push order confirms `bufpi` is the first argument (asm:124-133).
- Frame order matches: `file_name` (20 bytes at `bp-24h`), `buffer` (6 at `bp-10h`), the figure pointer, `offset`, `entry_num`.

**Deviation D6 — the figure block is over-allocated.** `FIGUREX_POINTER` ([Combat.h:588-590](../../MoM/src/Combat.h#L588-L590)) asks `Allocate_First_Block` for **33** paragraphs; asm:33-38 asks for **2**. The host needs more than 2 because `SAMB_ptr` is wider than a real-mode segment word, but 33 does not follow from anything written down, and the comment beside it does not explain the number. Settle the figure before this bundle closes.

# Deploy_Battle_Units

Faithful. Production [Combat.c:12455](../../MoM/src/Combat.c#L12455).

Splits one player's battle units into a ranged group and a melee group, moves surplus ranged units to the front line, seats both groups on the twelve deployment squares, then points every unit at the enemy side.

## Frame map

`sub sp, 60h` (asm:18) = 96 bytes, plus two register locals.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-60h` | `starting_cgc1` (12 words) | `starting_cgy` |
| `bp-48h` | `starting_cgc2` (12 words) | `starting_cgx` |
| `bp-30h` | `Ranged_Units` (9 words) | `ranged_battle_unit_idxs` |
| `bp-1Eh` | `Melee_Units` (9 words) | `melee_battle_unit_idxs` |
| `bp-0Ch` | `MoveFront_Count` | `move_to_front_count` |
| `bp-0Ah` | `Highest_Melee_Unit` | `highest_melee_ranged_slot` |
| `bp-8` | `Highest_Melee` | `highest_melee_strength` |
| `bp-6` | `Loop_Var` | `itr_ranged_slots` |
| `bp-4` | `Ranged_Count` | `ranged_battle_unit_count` |
| `bp-2` | `Melee_Count` | `melee_battle_unit_count` |
| `si` | `_SI_itr` | `itr` |
| `di` | `_DI_ctr` | `ctr` |

`starting_cgc1` sits at the lower address, so it is the first-declared local — which makes it `starting_cgy`, and `starting_cgc2` `starting_cgx`. The value assignments confirm it independently: the arrays production fills with `DEPLOY_COL*` are the ones the asm fills with the 12/11/13/10 pattern.

## Verified

- **All twelve deployment constants match the bytes.** `DEPLOY_DFNDR_ROW1..3` are 8, 7, 6 (asm:27-39); `DEPLOY_ATTKR_ROW1..3` are 14, 15, 16 (asm:42-53); `DEPLOY_COL1..4` are 12, 11, 13, 10 repeated three times (asm:55-66). All ten `#define`s are at [CMBTDEF.h:138-147](../../MoM/src/CMBTDEF.h#L138-L147).
- **The return value of `Undeployable_Battle_Units_On_Water` is discarded** — asm:21-23 is `push` / `call` / `pop cx` with no use of `ax`.
- **The two player-identity comparisons use opposite operand order, and production preserves both.** asm:24-25 loads `player_idx` first (`player_idx == _combat_defender_player`, [12492](../../MoM/src/Combat.c#L12470)); asm:180-181 and asm:246-247 load `_combat_defender_player` first (`_combat_defender_player == player_idx`, [12584](../../MoM/src/Combat.c#L12562) and [12614](../../MoM/src/Combat.c#L12592)). Same for `_combat_attacker_player` at asm:321-322.
- **The wall-corner skip is a loop, the central-structure skip is not.** asm:190-193 `jz`s back to `inc di` and re-tests, so squares 2 and 3 are skipped repeatedly; asm:198-200 tests square 9 once. Production writes the first as `while` and the second as `if` ([12586-12605](../../MoM/src/Combat.c#L12564-L12583)).
- **`ctr` is not reset between the melee and ranged placement loops.** asm:243 resets only `si`. The ranged group continues on from wherever the melee group stopped, which is what makes the whole twelve-square layout fill in order.
- **`Clear_Structure` argument order confirmed by push order** — asm:158-163 pushes `Ranged_Count`, `2`, `&Ranged_Units`, `Highest_Melee_Unit`, so the call is `(slot, array, element_size, count)` ([12575](../../MoM/src/Combat.c#L12553)).
- **The facing pass runs over every battle unit, not just the deployed ones**, and rewrites `target_cgx` / `target_cgy` for any unit this player controls (asm:308-368).

## OGBUG — faithful, do not fix

**The highest-melee scan indexes the wrong array.** [Combat.c:12540-12543](../../MoM/src/Combat.c#L12540-L12543) iterates slots `0..ranged_battle_unit_count-1` and reads `battle_units[itr_ranged_slots].melee` — a *slot* used as a battle-unit index — then uses the winning slot as `ranged_battle_unit_idxs[...]`. asm:120-125 does exactly the same thing, with no `Ranged_Units` lookup anywhere in the block:

```
mov     ax, [bp+Loop_Var]
mov     dx, size s_BATTLE_UNIT
imul    dx
les     bx, [battle_units]
add     bx, ax
mov     al, [es:bx+s_BATTLE_UNIT.melee]
```

So "the ranged unit with the highest melee strength" is actually chosen by scanning the first N units on the battlefield, whoever they belong to. Preserved with the `OGBUG` note in place.

# Cache_Graphics_Combat

Faithful.

Three calls in order — `Graphics_Cache_Reset`, `Reload_Combat_Graphics_Cache`, `Spellbook_Load_Small_Pictures` (asm:5-10) — and nothing else. Production [Cache_Graphics_Combat](../../MoM/src/LOADER.c#L1765) adds a `LOG_DEBUG` on entry and exit; that is the project's logging instrumentation, not a reconstruction difference.

# BU_HasSpellAbility__WIP

Faithful. Production [Combat.c:3661](../../MoM/src/Combat.c#L3661).

Six tests, each returning `ST_TRUE` on the first hit: `Attribs_2` against `USA_HEALING`, `USA_FIREBALL`, `USA_DOOMBOLT`, `USA_WEB`, then `Attribs_1` against `USA_SUMMON_DEMON_1 or USA_SUMMON_DEMON_2` and `USA_CASTER_40` (asm:14-66). Production reproduces the order, the field switch, and the paired demon mask as a single `&`.

**`USA_CASTER_20` is not tested.** asm:64 checks only `USA_CASTER_40` ([MOM_DEF.h:653-654](../../MoX/src/MOM_DEF.h#L653-L654) give 0x2000 and 0x4000), so a 20-mana caster does not count as having a spell ability here. That feeds the Spell button's condition.

# NIU_Who_Has_More_Leadership

Faithful. Production [Combat.c:3692](../../MoM/src/Combat.c#L3692).

`_battlefield_leadership[_combat_attacker_player]` against `_battlefield_leadership[_combat_defender_player]`, attacker on the left. asm:17 is `jle` — signed, matching `int16_t *` — and jumps to the defender return, so **ties go to the defender** and only a strict attacker majority returns the attacker.

Currently unreferenced, hence the `NIU_` prefix; the only mention is the note at [CMBMAGIC.c:100](../../MoM/src/CMBMAGIC.c#L100).

---

# What else belongs in this review

**Moved out of scope for this session.** Two setup-path functions are deferred by decision, not by oversight:

| Function | ASM | Lines |
| --- | --- | --- |
| `Combat_Compose_Background` | `ovr163/CMB_ComposeBackgrnd__WIP.asm` | 206 |
| `Generate_Combat_Map` | `ovr154/Generate_Combat_Map.asm` | 907 |

`Combat_Compose_Background` carries a `faithful` row in [Combat-Combat_Spellbook_Screen.md](Combat-Combat_Spellbook_Screen.md) from a spellbook-callee pass. That verdict covers fidelity only — the function still has a live `STU_DEBUG_BREAK()` on its river path, IDA-prefixed animation globals and bare magic numbers, so it is not finished to this project's standard.

`Generate_Combat_Map` is the largest single listing in the bundle and is its own review. It consumes `Location_Type`, the house style, both matrices and the wall flags — every value the `Build_Battlefield` findings touched — so it is where those fixes actually land.

**Not covered, and the `faithful` verdicts do not extend to them.** `Combat_Screen`'s input loop dispatches to a large set of handlers that are their own review: `Combat_Screen_Draw`, `Battle_Unit_Action`, `Combat_Next_Turn`, `Auto_Cast_Spell_And_Do_Combat_Turn`, `Check_For_Winner`, `Next_Battle_Unit`, `Switch_Active_Battle_Unit`, `Assign_Combat_Grids`, `Move_Confused`, `Combat_Cast_Spell*`, `Combat_Unit_Statistics_Window`, `Combat_Information_Window`, `End_Of_Combat__WIP`.

**`Combat_Compose_Background` is the tail call of `Build_Battlefield`** (asm:621) and carries a `faithful` row in [Combat-Combat_Spellbook_Screen.md](Combat-Combat_Spellbook_Screen.md), where it was reviewed as a spellbook callee. That verdict covers fidelity only — the function still has a live `STU_DEBUG_BREAK()` on its river path, six IDA-prefixed animation globals, and bare magic numbers, so it is not finished to this project's standard. It needs its own pass, and this bundle is the more natural home for it.

**Highest-value follow-on: `Generate_Combat_Map`** (`ovr154`). It consumes `Location_Type`, the house style, both matrices and the wall flags — every value the `Build_Battlefield` findings touched — so it is where those fixes actually land. Reviewing it would also settle whether the two wall-related OGBUGs noted in its header comment are reproduced, and whether the `/` where a `%` belongs in the vortex de-phasing at the entity-create call is OG or reconstruction.

**Second: `Calc_Battlefield_Bonuses`** (`ovr122`, 450 lines) — still the outstanding Tier 2 from the init review, and it produces the `_battlefield_leadership` that `NIU_Who_Has_More_Leadership` reads.

**Third, and outside this bundle: the `s_CITY.enchantments` signedness sweep.** [MOM_DAT.h:1765](../../MoX/src/MOM_DAT.h#L1765) is now `int8_t`, which the `ovr093` evidence supports, but six consumers still reach it through a `uint8_t *`.

## Build state

`cmake --build --preset MSVC-debug` (2026-08-08). **Every game target links**: `momlib.lib`, `MOX.lib`, `STU.lib`, `Platform.lib`, `Platform_Headless.lib`, `ReMoM.exe`, `HeMoM.exe`, `MOX_tests.exe` and all tools. Zero `C4057`s — the `s_CITY.enchantments` signedness sweep is finished. Every rename in the ledger above compiles.

`MOM_tests` is the only failing project, with 38 errors across four files. Pre-existing, unrelated to this review, and previously masked: `MOM_tests` never compiled while `momlib` was failing, because MSBuild skips dependents.

Root cause is a missing include. These four declare `struct s_BATTLE_UNIT` **by value or as an array**, which needs the complete type, but none of them includes `Combat.h` where it is defined ([Combat.h:1192](../../MoM/src/Combat.h#L1192)):

- [test_AI_Stacks_Survey_Expedition_Forces.cpp](../../MoM/tests/test_AI_Stacks_Survey_Expedition_Forces.cpp)
- [test_Ranged_Mouse_Image.cpp](../../MoM/tests/test_Ranged_Mouse_Image.cpp)
- [test_Reload_Melee_Sound.cpp](../../MoM/tests/test_Reload_Melee_Sound.cpp)
- [test_Set_Gibs.cpp](../../MoM/tests/test_Set_Gibs.cpp)

`MOM_DAT.h` only declares `extern struct s_BATTLE_UNIT *` pointers, which compile fine against an incomplete type — which is why the omission went unnoticed. The 25 `C2109` "subscript requires array or pointer type" errors are all downstream of that one incomplete type. [test_Clear_Gibs.cpp](../../MoM/tests/test_Clear_Gibs.cpp) already carries the include and now compiles, which confirms the fix; several other MoM tests do the same.

Outside this review, and the only thing standing between this doc and DONE-DONE.

## Related references

- [Combat-Init_Prep_Etc.md](Combat-Init_Prep_Etc.md) — `Begin_Combat_Turn` (the `CMB_PrepareTurn__WIP` this function calls twice) and the `ovr116` unit-stat chain; DONE-DONE.
- [Combat-Battle_Unit_Action.md](Combat-Battle_Unit_Action.md) — the grid-click handler; DONE-DONE.
- [Combat-Combat_Spellbook_Screen.md](Combat-Combat_Spellbook_Screen.md) — carries the `Combat_Compose_Background` row discussed above.
- [Combat.h:681-695](../../MoM/src/Combat.h#L681-L695) — the `clt_` enum whose ordering makes the node arm's subtraction correct.
