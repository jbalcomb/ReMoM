Combat-Combat_Screen.md

SEEALSO:  Combat-Combat.md
SEEALSO:  Combat-Combat_Screen.md
SEEALSO:  Combat-Combat_Figure_Compose.md
SEEALSO:  Combat-End_Of_Combat.md
SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoM-CombatScreen-Grid.md

CMB_PrepareTurn__WIP()        ==>  Begin_Combat_Turn()
Combat_Screen_Draw_Buttons()  ==>  Tactical_Combat_Draw_Buttons()
BU_SetVisibility__WIP()       ==>  Battle_Unit_Set_Invisibility_Effect()
Combat_Compose_Background()    ==>  Combat_Compose_Background()
Combat_Screen__WIP()          ==>  Combat_Screen()
BU_HasSpellAbility()          ==>  Battle_Unit_Has_Spell_Ability()
BU_HasSpellAbility__WIP()     ==>  Battle_Unit_Has_Spell_Ability()
sub_7EC81()                   ==>  NIU_Who_Has_More_Leadership()
CMB_BaseAllocs__WIP()         ==>  Allocate_Combat_Base_Blocks()
CMB_SetNearAllocs__WIP()      ==>  Allocate_Combat_Near_Buffers()
CMB_LoadResources__WIP()      ==>  Combat_Screen_Load_Resources()
CMB_Terrain_Init__WIP()       ==>  Build_Battlefield()
EMM_TILEX_Init__HACK()        ==>  Claim_EMS_Page_For_Figure_Set()
CMB_Units_Init__WIP()         ==>  Prepare_All_Battle_Units()
BU_Apply_Specials()           ==>  Apply_Enchantment_And_Mutation_Effects()
BU_GetRangedCursor__WIP()     ==>  Ranged_Mouse_Image()

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
COL_CMBUI_Banner              ==>  combat_name_font_colors_by_banner
IMG_CMB_ScanIcons             ==>  combat_scan_icon_segs

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr090\Combat_Screen__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\CMB_BaseAllocs__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\CMB_LoadResources__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\CMB_SetNearAllocs__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr052\Cache_Graphics_Combat.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\CMB_Terrain_Init__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\Load_Combat_Terrain_Pictures.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\Combat_Compose_Background.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\EMM_TILEX_Init__HACK.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Combat_Grid_Screen_Coordinates.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\CMB_Units_Init__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\Load_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr120\Unit_Gold_Upkeep.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\Battle_Unit_Regular_Stats.asm
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
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Calc_Battlefield_Bonuses.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Combat_Cache_Write.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Combat_Cache_Read.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\Switch_Active_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\CMB_PrepareTurn__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr099\Combat_Screen_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr105\Battle_Unit_Melee_Attack_Icon.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr105\Battle_Unit_Ranged_Attack_Icon.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Assign_Mouse_Images.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\BU_GetRangedCursor__WIP.asm
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

OON XEF:  Combat_Screen() |-> Assign_Mouse_Images() |-> Ranged_Mouse_Image()

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
            |-> Combat_Figure_Compose()                             Combat-Combat_Figure_Compose.md
        |-> Deploy_Battle_Units()
            |-> Undeployable_Battle_Units_On_Water()
    |-> Combat_Structure()
    |-> Update_Combat_Enchantments_Icon_And_Help()
    |-> Combat_Node_Type()
    |-> Combat_Info_Effects_Count()                                 Combat-Combat_Information_Window.md
    |-> Calc_Battlefield_Bonuses()
    |-> Combat_Cache_Write()
    |-> Switch_Active_Battle_Unit()
    |-> Begin_Combat_Turn()
    |-> Combat_Screen_Draw()
        |-> Combat_Grid_Entities__WIP()                             Combat-Combat_Screen_Map_Draw.md
        |-> Combat_Screen_Map_Draw()                                Combat-Combat_Screen_Map_Draw.md
        |-> Combat_Info_Effects_Count()                             Combat-Combat_Information_Window.md
        |-> Tactical_Combat_Draw_Buttons()
        |-> Draw_Spell_Information_Window()
        |-> Draw_Combat_Unit_Display()
        |-> Draw_Active_Unit_Window()                               Combat-Combat_Figure_Compose.md
    |-> Assign_Mouse_Images()
        |-> Ranged_Mouse_Image()
    |-> Auto_Cast_Spell_And_Do_Combat_Turn()
        |-> Combat_Cast_Spell()
        |-> Check_For_Winner()
        |-> Update_Combat_Enchantments_Icon_And_Help()
        |-> Auto_Do_Combat_Turn()
    |-> Check_For_Winner()
    |-> Next_Battle_Unit()
    |-> Assign_Combat_Grids()
    |-> Turn_Off_Auto_Combat()
    |-> Battle_Unit_Has_Spell_Ability()
    |-> Add_Combat_Enchantment_Fields()
    |-> Move_Confused()
    |-> Battle_Unit_Action()
    |-> Battle_Unit_Set_Invisibility_Effect()
    |-> Get_Combat_Grid_Cell_X()
    |-> Get_Combat_Grid_Cell_Y()
    |-> Combat_Compose_Background()
    |-> End_Of_Combat()                                         Combat-End_Of_Combat.md

Draw_Active_Unit_Stats_And_Icons()
    |-> Battle_Unit_Ranged_Attack_Icon()
Draw_Combat_Unit_Display()
    |-> Battle_Unit_Ranged_Attack_Icon()

---


# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-25).** No findings open. Three passes: the setup chain (2026-08-08, thirty functions against 8,297 asm lines), the input loop (2026-08-15, seventeen functions against 2,486 asm lines), and the mouse-image pair (2026-08-25, `Assign_Mouse_Images` and `Ranged_Mouse_Image`, 486 asm lines) - R23 raised and fixed in that last pass. `Combat_Info_Effects_Count` moved out to [Combat-Combat_Information_Window.md](Combat-Combat_Information_Window.md) and D2 was resolved in the code.

The first pass scoped itself to `Combat_Screen` "plus everything it reaches during setup" and left the input-loop handlers alone; seventeen functions this doc's header block names were never given a verdict. The second pass walked every one of them. Nine reconstruction errors were raised and all nine are fixed. `cmake --build --preset MSVC-debug` is clean — `STU.lib`, `MOX.lib`, `momlib.lib`, `Platform_Headless.lib`, `ReMoM.exe` and `HeMoM.exe` all build and link.

Two functions the header block names are owned by nobody and were moved to [Combat-Miscellaneous.md](Combat-Miscellaneous.md) by the `MISC_DOC_SPREAD` rule in [tools/review_coverage.py](../../tools/review_coverage.py): `Combat_Grid_Screen_Coordinates` (walked, faithful) and `Calc_Battlefield_Bonuses` (not yet walked).

Regenerate the outstanding set at any time with `python3 tools/review_coverage.py --attribution` — it is derived from this doc's header block and coverage table, so it cannot drift out of step with them the way a hand-typed list does.

## Scope


**`Combat_Info_Effects_Count` moved out (2026-08-25).** It held a row here and a walked section, because this screen's layout path calls it. It counts what the info-window builders assemble, and only reads correctly beside them, so it now lives in [Combat-Combat_Information_Window.md](Combat-Combat_Information_Window.md). It was re-walked there rather than inheriting this doc's verdict; the result is unchanged - faithful. The `cnt_` label-swap note that used to sit here moved with it.
The Production column carries the current name and line; the ASM column keeps the IDA names, which never change.

| Function (production) | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| `Combat_Screen` | [Combat.c:1425](../../MoM/src/Combat.c#L1425) | [ovr090/Combat_Screen__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr090/Combat_Screen__WIP.asm) (1724) | faithful |
| `Allocate_Combat_Base_Blocks` | [Combat.c:22272](../../MoM/src/Combat.c#L22272) | [ovr163/CMB_BaseAllocs__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr163/CMB_BaseAllocs__WIP.asm) (76) | faithful |
| `Combat_Screen_Load_Resources` | [Combat.c:6660](../../MoM/src/Combat.c#L6660) | [ovr103/CMB_LoadResources__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr103/CMB_LoadResources__WIP.asm) (386) | faithful |
| `Allocate_Combat_Near_Buffers` | [Combat.c:6787](../../MoM/src/Combat.c#L6787) | [ovr103/CMB_SetNearAllocs__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr103/CMB_SetNearAllocs__WIP.asm) (59) | faithful |
| `Cache_Graphics_Combat` | [LOADER.c:1832](../../MoM/src/LOADER.c#L1832) | [ovr052/Cache_Graphics_Combat.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr052/Cache_Graphics_Combat.asm) (13) | faithful |
| `Build_Battlefield` | [Combat.c:19741](../../MoM/src/Combat.c#L19741) | [ovr154/CMB_Terrain_Init__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr154/CMB_Terrain_Init__WIP.asm) (627) | faithful |
| `Prepare_All_Battle_Units` | [Combat.c:4809](../../MoM/src/Combat.c#L4809) | [ovr098/CMB_Units_Init__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr098/CMB_Units_Init__WIP.asm) (527) | faithful |
| `Battle_Unit_Has_Spell_Ability` | [Combat.c:3658](../../MoM/src/Combat.c#L3658) | [ovr098/BU_HasSpellAbility__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr098/BU_HasSpellAbility__WIP.asm) (73) | faithful |
| `NIU_Who_Has_More_Leadership` | [Combat.c:3689](../../MoM/src/Combat.c#L3689) | [ovr098/NIU_Who_Has_More_Leadership.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr098/NIU_Who_Has_More_Leadership.asm) (27) | faithful |
| `Load_Battle_Unit` | [COMBINIT.c:445](../../MoM/src/COMBINIT.c#L445) | [ovr116/Load_Battle_Unit.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/Load_Battle_Unit.asm) (286) | faithful |
| `Unit_Gold_Upkeep` | [CITYCALC.c:242](../../MoM/src/CITYCALC.c#L242) | [ovr120/Unit_Gold_Upkeep.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr120/Unit_Gold_Upkeep.asm) (140) | faithful |
| `Battle_Unit_Regular_Stats` | [COMBINIT.c:535](../../MoM/src/COMBINIT.c#L535) | [ovr116/Battle_Unit_Regular_Stats.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/Battle_Unit_Regular_Stats.asm) (478) | faithful |
| `Battle_Unit_Level_Stats` | [COMBINIT.c:893](../../MoM/src/COMBINIT.c#L893) | [ovr116/BU_Apply_Level.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Level.asm) (275) | faithful |
| `Battle_Unit_Hero_Skill_Stats` | [COMBINIT.c:1067](../../MoM/src/COMBINIT.c#L1067) | [ovr116/BU_Init_Hero_Unit.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Init_Hero_Unit.asm) (394) | faithful |
| `Battle_Unit_Item_Stats` | [COMBINIT.c:39](../../MoM/src/COMBINIT.c#L39) | [ovr116/BU_Apply_Item_Powers.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Item_Powers.asm) (467) | faithful |
| `Item_Powers_To_Unit_Enchantments` | [COMBINIT.c:135](../../MoM/src/COMBINIT.c#L135) | [ovr116/BU_Apply_Item_Enchantments.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Item_Enchantments.asm) (469) | faithful |
| `Item_Powers_To_Attack_Attributes` | [COMBINIT.c:224](../../MoM/src/COMBINIT.c#L224) | [ovr116/BU_Apply_Item_Attack_Specials.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Item_Attack_Specials.asm) (194) | faithful |
| `Apply_Enchantment_And_Mutation_Effects` | [COMBINIT.c:705](../../MoM/src/COMBINIT.c#L705) | [ovr116/BU_Apply_Specials.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Specials.asm) (509) | faithful |
| `Unit_Moves2` | [NEXTTURN.c:1530](../../MoM/src/NEXTTURN.c#L1530) | [ovr121/Unit_Moves2.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr121/Unit_Moves2.asm) (244) | faithful |
| `Unit_Hit_Points` | [COMBINIT.c:266](../../MoM/src/COMBINIT.c#L266) | [ovr116/Unit_Hit_Points.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/Unit_Hit_Points.asm) (216) | faithful |
| `Get_Combat_Grid_Cell_X` | [Combat.c:21903](../../MoM/src/Combat.c#L21903) | [ovr154/Get_Combat_Grid_Cell_X.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr154/Get_Combat_Grid_Cell_X.asm) (25) | faithful |
| `Get_Combat_Grid_Cell_Y` | [Combat.c:21912](../../MoM/src/Combat.c#L21912) | [ovr154/Get_Combat_Grid_Cell_Y.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr154/Get_Combat_Grid_Cell_Y.asm) (28) | faithful |
| `Deploy_Battle_Units` | [Combat.c:11488](../../MoM/src/Combat.c#L11488) | [ovr113/Deploy_Battle_Units.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr113/Deploy_Battle_Units.asm) (375) | faithful |

**Two header lines need correcting.**

- `BU_HasSpellAbility() ==> Battle_Unit_Has_Spell_Ability()` is backwards, and no rename is needed either way — the IDA proc is *already* `Battle_Unit_Has_Spell_Ability` (asm:2) and so is production. The `__WIP` suffix is inherited from the IDA database, not a marker somebody added here.
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
combat_scan_icon_segs@.Stat.Melee@ ==>  combat_scan_icon_segs[]
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
Battle_Unit_Has_Spell_Ability        ==>  Battle_Unit_Has_Spell_Ability()
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
- **`Highest_Melee_Unit` never held a unit.** It holds a slot within the ranged list, which is exactly what let the OGBUG at [Combat.c:11676-11679](../../MoM/src/Combat.c#L11676-L11679) hide.
- **`RightClick_X` / `RightClick_Y` are grid cells, not pixels.** They come out of `Get_Combat_Grid_Cell_X`/`_Y`, and the very next line subscripts `g_combat_grid_action_map` with them.

`CMB_DEFR_First_CE` was also the only `DEFR` spelling in the file; everything else uses `DFNDR` or `ATTKR`.

`CMB_ProgressTurnFlow__WIP` is the thunk label; the proc it lands on is `Combat_Next_Turn` in `ovr098`. The extractor emitted `CMB_ProgressTurnFlow__WIP.c` and `Combat_Next_Turn.asm` for the same function — the IDA database was renamed between the two passes, which is why neither name has both files.

**Evidence standard.** A divergence is recorded only where the bytes show a different computation, a different order, or a different operand width. Branch layout alone is not a finding — but a `jz`/`jnz` whose target changes *which* arm executes is, because that is the computation.

**Anchors.** Production links point at **function start lines**. `Combat.c` moved repeatedly during this review while it was open in the editor; every line number was re-verified against the file as it stands after the last change. For statement-level precision use the `asm:N` references, which are stable ground truth.

---

---

# Reopened scope

17 functions, 2,486 asm lines. Every one is named in this doc's header block; none has a coverage-table row.

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `Combat_Screen_Draw` | [Combat.c:5159](../../MoM/src/Combat.c#L5159) | `ovr099/Combat_Screen_Draw.asm` | 644 | **full body walk — faithful** |
| `Tactical_Combat_Draw_Buttons` | [Combat.c:5489](../../MoM/src/Combat.c#L5489) | `ovr099/Combat_Screen_Draw_Buttons.asm` | 172 | **full body walk — faithful** |
| `Battle_Unit_Ranged_Attack_Icon` | [Combat.c:7456](../../MoM/src/Combat.c#L7456) | `ovr105/Battle_Unit_Ranged_Attack_Icon.asm` | 77 | **full body walk — faithful** |
| `Battle_Unit_Melee_Attack_Icon` | [Combat.c:7403](../../MoM/src/Combat.c#L7403) | `ovr105/Battle_Unit_Melee_Attack_Icon.asm` | 61 | **full body walk — faithful** |
| `Assign_Mouse_Images` | [Combat.c:4460](../../MoM/src/Combat.c#L4460) | `ovr098/Assign_Mouse_Images.asm` | 336 | **full body walk — faithful** (2026-08-25) |
| `Ranged_Mouse_Image` | [Combat.c:4637](../../MoM/src/Combat.c#L4637) | `ovr098/BU_GetRangedCursor__WIP.asm` | 150 | **full body walk — faithful** (2026-08-25) |
| `Draw_Combat_Unit_Display` | [Combat.c:7526](../../MoM/src/Combat.c#L7526) | `ovr105/Draw_Combat_Unit_Display.asm` | 566 | **full body walk — faithful; D8** |
| `Load_Combat_Terrain_Pictures` | [Combat.c:22042](../../MoM/src/Combat.c#L22042) | `ovr163/Load_Combat_Terrain_Pictures.asm` | 161 | **faithful** — walk at [line 632](Combat-Combat_Screen.md#L632), spot-checked |
| `Undeployable_Battle_Units_On_Water` | [Combat.c:15654](../../MoM/src/Combat.c#L15654) | `ovr124/Undeployable_Battle_Units_On_Water.asm` | 135 | **full body walk — faithful** |
| `Draw_Spell_Information_Window` | [Combat.c:5549](../../MoM/src/Combat.c#L5549) | `ovr099/Draw_Spell_Information_Window.asm` | 125 | **full body walk — faithful** |
| `Update_Combat_Enchantments_Icon_And_Help` | [Combat.c:3604](../../MoM/src/Combat.c#L3604) | `ovr098/Update_Combat_Enchantments_Icon_And_Help.asm` | 108 | **full body walk — faithful** |
| `Add_Combat_Enchantment_Fields` | [Combat.c:4682](../../MoM/src/Combat.c#L4682) | `ovr098/Add_Combat_Enchantment_Fields.asm` | 103 | **full body walk — faithful** |
| `Next_Battle_Unit` | [Combat.c:6373](../../MoM/src/Combat.c#L6373) | `ovr103/Next_Battle_Unit.asm` | 89 | **full body walk — faithful** |
| `Combat_Structure` | [Combat.c:15603](../../MoM/src/Combat.c#L15603) | `ovr124/Combat_Structure.asm` | 84 | **full body walk — faithful** |
| `Combat_Node_Type` | [Combat.c:6249](../../MoM/src/Combat.c#L6249) | `ovr103/Combat_Node_Type.asm` | 82 | **full body walk — faithful** |
| `Move_Confused` | [Combat.c:6627](../../MoM/src/Combat.c#L6627) | `ovr103/Move_Confused.asm` | 77 | **full body walk — faithful** |
| `Combat_Cache_Write` | [Combat.c:22014](../../MoM/src/Combat.c#L22014) | `ovr154/Combat_Cache_Write.asm` | 64 | **full body walk — faithful** |
| `Combat_Cache_Read` | [Combat.c:21989](../../MoM/src/Combat.c#L21989) | `ovr154/Combat_Cache_Read.asm` | 64 | **full body walk — faithful** |
| `Map_Tile_EMS_Page_As_Sandbox` | [Combat.c:19595](../../MoM/src/Combat.c#L19595) | `ovr153/EMM_TILEX_Init__HACK.asm` | 41 | **faithful** — walk at [line 643](Combat-Combat_Screen.md#L643), spot-checked; EMS mapper call is D4 |
| `Auto_Cast_Spell_And_Do_Combat_Turn` | [Combat.c:3707](../../MoM/src/Combat.c#L3707) | `ovr098/Auto_Cast_Spell_And_Do_Combat_Turn.asm` | 40 | **full body walk — faithful** |
| `Switch_Active_Battle_Unit` | [Combat.c:2710](../../MoM/src/Combat.c#L2710) | `ovr091/Switch_Active_Battle_Unit.asm` | 16 | **full body walk — faithful** |
| `Turn_Off_Auto_Combat` | [Combat.c:5072](../../MoM/src/Combat.c#L5072) | `ovr098/Turn_Off_Auto_Combat.asm` | 12 | **full body walk — faithful** |

**The two prose walks are now transcribed.** `Load_Combat_Terrain_Pictures` and `Map_Tile_EMS_Page_As_Sandbox` were walked in full earlier in this doc and declared faithful; both verdicts are spot-checked against their listings and carried into the table above.

- `Load_Combat_Terrain_Pictures` — the `cts_Water` early return is the bare zero test `or ctt, ctt` / `jnz` at asm:15-16, returning 0 in `ax`; the first load loop bound is `cmp itr, 48` at asm:117; and the filename buffer runs `bp-20h` through `bp-3` with `temp_seg` at `bp-2`, i.e. 30 bytes, matching `LEN_STRING`.
- `Map_Tile_EMS_Page_As_Sandbox` — four `farpokew` writes at bare `EMS_PFBA` for MemSig1, MemSig2, size **3071** and used **1**, matching the four `SET_2B_OFS` calls at [Combat.c:19844-19847](../../MoM/src/Combat.c#L19844-L19847). The `EMM_MapMulti4(0, EmmHndl_TILEXXX)` the asm opens with is replaced by flat arithmetic in production — that is deviation D4, not a finding.

**`Combat_Grid_Screen_Coordinates` is misc, not this doc's.** Its twelve call sites span four reviews — [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md) (8), [Combat-Generate_Combat_Map.md](Combat-Generate_Combat_Map.md) (2), [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md) (1) and this one (1) — so no single review can own it. It was walked here and found faithful; the verdict is recorded in [Combat-Miscellaneous.md](Combat-Miscellaneous.md) and its row has been removed from the table above.

**`Calc_Battlefield_Bonuses` is misc, not this doc's.** Its callers span three reviews — `Combat_Screen` here, `Strategic_Combat` in [Combat-Strategic_Combat.md](Combat-Strategic_Combat.md) and `Begin_Combat_Turn` in [Combat-Init_Prep_Etc.md](Combat-Init_Prep_Etc.md) — with no dominant owner, so it falls to [Combat-Miscellaneous.md](Combat-Miscellaneous.md) by the `MISC_DOC_SPREAD` rule. It has not been walked by anyone yet.

**Not in this scope, though the header block names them.** `Battle_Unit_Action`, `Assign_Combat_Grids`, `Combat_Next_Turn`, `Begin_Combat_Turn`, `Deploy_Battle_Units`, `Claim_EMS_Page_For_Figure_Set`, `Combat_Figure_Load`, `Draw_Active_Unit_Window`, `Combat_Compose_Background` and `Battle_Unit_Set_Invisibility_Effect` all carry verdicts in other reviews — the header block is the call tree `Combat_Screen` reaches, not the set this doc adjudicates.

## Reopened 2026-08-25 — the mouse-image pair

Two functions the call graph puts squarely here and no doc had ever claimed. `Assign_Mouse_Images` has exactly one caller, `Combat_Screen` at [Combat.c:2182](../../MoM/src/Combat.c#L2182), and `Ranged_Mouse_Image` has exactly two, both inside `Assign_Mouse_Images`. They are adjacent overlay slots, `o98p10` and `o98p11`.

The second one's listing is named for drake178's alias, `ovr098/BU_GetRangedCursor__WIP.asm` - which is why its slot showed no listing until the mapping went into the header block.

### R23 (fixed) — both enchantment chains in `Ranged_Mouse_Image` dropped the unit-type source

Raised and fixed the same day. The two chains at [Combat.c:4674-4675](../../MoM/src/Combat.c#L4674-L4675) OR'd only the battle-unit and item enchantments where the listing ORs a third source - the unit-type enchantments, at asm:62-70 for the defender and asm:98-106 for the attacker, each reached through `mov ax, [es:bx+s_BATTLE_UNIT.unit_idx]` / `shl ax, 5` / `les bx, [_UNITS]`.

It mattered: each value is tested for exactly one bit, `UE_TRUE_SIGHT` on the attacker and `UE_INVISIBILITY` on the defender, so a unit carrying either from its unit type was invisible to both tests and drew the wrong ranged cursor. Both lines now carry all three terms.

### `Assign_Mouse_Images` ([Combat.c:4460](../../MoM/src/Combat.c#L4460), asm 336)

Faithful, all 336 lines.

**Frame - order matches.** `ranged_attack_type_group` `-0Ch`, `cgy` `-0Ah`, `screen_y` `-8`, `screen_x` `-6`, `range_y` `-4`, `range_x` `-2`, with `sub sp, 0Ch` reconciling at twelve bytes; [Combat.c:4407-4412](../../MoM/src/Combat.c#L4407-L4412) declares those six in that order, and `scanned_battle_unit_idx` / `cgx` are register locals.

**The pointer read is offset by four on both axes** - asm:54-59 is `call Pointer_X` / `add ax, 4` / store, then the same for Y ([Combat.c:4434-4435](../../MoM/src/Combat.c#L4434-L4435)) - and the whole body is skipped when `screen_y >= 168`.

**The grid cell dispatches three ways** on `g_combat_grid_action_map[cgy][cgx]`: `-2`, `-1`, `99` (asm:83-88), matching [Combat.c:4441-4452](../../MoM/src/Combat.c#L4441-L4452) and the cell-value constants in [CMBTDEF.h:100-103](../../MoM/src/CMBTDEF.h#L100-L103).

**The melee/ranged split is `range_x <= 1 && range_y <= 1`** (asm:271-274, `jg` to the ranged branch). Inside melee, `Check_Attack_Melee_City_Wall` decides between the melee cursor and a ranged one; outside, it is always ranged. Both call sites push `scanned_battle_unit_idx` then `_active_battle_unit`, so the argument order is `(_active_battle_unit, scanned_battle_unit_idx)` - [Combat.c:4516-4535](../../MoM/src/Combat.c#L4516-L4535). Two IDA aliases resolve here: `j_BU_MeleeWallCheck` is `Check_Attack_Melee_City_Wall` and `BU_GetRangedCursor__WIP` is `Ranged_Mouse_Image`.

### `Ranged_Mouse_Image` ([Combat.c:4637](../../MoM/src/Combat.c#L4637), asm 150)

Faithful.

**Frame - order matches.** `defender_enchantments` `-0Ah` and `attacker_enchantments` `-6`, both `dword`, then `ranged_attack_group` `-2`, with `sub sp, 0Ah`; [Combat.c:4584-4586](../../MoM/src/Combat.c#L4584-L4586) declares them in that order and `image_num` is SI.

**The early-out is a three-way `!=` chain.** asm:28-34 sends `rag_Boulder`, `rag_Missile` and `rag_Magic` to the body and everything else to `crsr_RedCross`; production writes it as the negated `&&` chain at [Combat.c:4663-4671](../../MoM/src/Combat.c#L4663-L4671).

**The two mask tests sit in different words.** `UE_TRUESIGHT` is high-word - asm:111-112 is `and dx, 0` / `and ax, UE_TRUESIGHT` - while `UE_INVISIBILITY` is low-word at asm:120-121. Both reconstructed as plain 32-bit masks.

**The invisibility branch is an `||` and then a two-way.** Either the defender's enchantment bit or its `UA_INVISIBILITY` ability sends control to the `_attacker_sees_illusions` test (asm:118-130), which picks the ranged cursor when true and the red cross when false - [Combat.c:4608-4623](../../MoM/src/Combat.c#L4608-L4623).

## Reopened — verified faithful

### `Combat_Screen_Draw` ([Combat.c:5159](../../MoM/src/Combat.c#L5159), asm 644)

**Frame.** Five slotted locals and two register locals, and [Combat.c:5209-5215](../../MoM/src/Combat.c#L5209-L5215) declares them in frame order — `enchantment_help_entry_base` at `bp-0Eh`, `Opponent_Type` at `-0Ch`, `Can_Split_Name` at `-0Ah`, `enchantment_strip_left_x` at `-8`, `colors[6]` at `-6`, with `itr` in SI and `string_index` in DI.

**Both enchantment strips.** The compound condition is carried intact: `((first_shown + itr) < count) && (itr < 4)`, emitted as `jge` to the exit then `cmp si, 4` / `jl` back to the body (asm:87-93 and asm:138-144). `FLIC_Draw(left + itr*17, 179, icon_seg)` matches the push order, and the help-entry write scales by `size s_HELP_ENTRY` off `_help_entries.help_00`. The 247/9 and 8/13 pairs, and the eight `ST_UNDEFINED` help resets, all match.

**The environment chain.** `_combat_environ == 0`, `== 1`, `== 5` as a compare chain, with `jz` / `jmp` long-branch fixups carrying the source conditions un-inverted. The city-siege neutral test — `(Opponent_Type == 0) || (_combat_attacker_player != NEUTRAL_PLAYER_IDX)` to the city name, else Monsters/Raiders — matches asm:337-356 clause for clause.

**The lair name split.** `stu_strcpy(GUI_String_1, TBL_EZ_Names[_LAIRS[_combat_environ_idx].type])` with the word-scaled table read; the space scan with `string_index++` and `itr++` both inside the not-a-space path and the `break` skipping both (asm:387-402); the post-loop `string_index++` then `if(Can_Split_Name == ST_FALSE) string_index = 0` where the `jnz` skips the set; and `GUI_String_1[string_index] -= 32` reloading the base on both sides of the read-modify-write, exactly as asm:409-415 does.

**The text calls.** `Set_Font_Colors_15(4, &colors[0])`, `Set_Font_Style_Shadow_Down(4, 15, 0, 0)`, `Set_Outline_Color(241)` in that order at all three sites; `Print_Centered(40, 168, ...)` for the open-field and siege banners, `(40, 167, &GUI_String_1[string_index])` for the lair, `(278, 167, _players[_current_player_idx].name)` for the human strip. `Cycle_Palette_Color(198, 0, 0, 0, 55, 0, 0, 11)` — eight words, `add sp, 10h`.

**The immobile block.** The three-clause guard is `jz` / `jz` / `jnz` to continue, so the source reads `== ST_TRUE`, `== ST_FALSE`, `!= ST_UNDEFINED` (asm:537-547). `Print_Paragraph(84, 167, 58, cnst_Cmbt_Immobile, 0)` — five words, `add sp, 0Ah`. `colors[0] = 227`, `colors[1] = 243`, and `Set_Font_LF(1)` all match.

**The counters.** Vortex wraps at 5 (asm:614) against `VORTEX_ANIM_FRAME_COUNT 5`, `frame_anim_cycle` at 2 (asm:628). The cell-effect counter is the known rescaling, not a finding — see the deviation note below.

### `Update_Combat_Enchantments_Icon_And_Help` ([Combat.c:3604](../../MoM/src/Combat.c#L3604), asm 108)

Faithful apart from R4. `s_COMBAT_ENCHANTMENTS` is `sizeof=0x1E`, fifteen 2-byte entries, so the `cmp itr, 30` bound at asm:98 is `NUM_COMBAT_ENCHANTMENTS * 2`. The array is bytes (`db`) read through `byte ptr`, and the Counter Magic test uses `jle` — a signed compare — matching `combat_enchantments[itr] > 0` with an `if`/`else` writing `is_active` twice rather than a ternary. `idx = itr / 2` is the `cwd` / `sub ax,dx` / `sar ax,1` idiom (asm:44-47) and the even/odd split is a real `idiv` with `or dx, dx` (asm:49-53). Both branches scale by 4 into `s_COMBAT_ENCHANTMENT_ICON_DATA` and `s_COMBAT_ENCHANTMENT_ICON` — `icon_idx`/`icon_seg` at +0, `help_idx` at +2 — with `combat_enchantment_icon_segs` word-indexed via `shl bx, 1`. `e_ST_TRUE = 1` (`WIZARDS.inc:9163`), so the literal `1` at [Combat.c:3556](../../MoM/src/Combat.c#L3556) is the same constant.

### `Turn_Off_Auto_Combat` ([Combat.c:5072](../../MoM/src/Combat.c#L5072), asm 12)

Faithful, nothing to change. Word store of `e_ST_FALSE` to `_auto_combat_flag` ([Combat.h:1399](../../MoM/src/Combat.h#L1399), `int16_t`); one `push` and a `pop cx` for `Next_Battle_Unit(_current_player_idx)`; no push and no cleanup for `Assign_Combat_Grids()`, matching its `(void)` signature. No `sub sp` — no locals — and nothing in `ax` before `retf`.

### `Switch_Active_Battle_Unit` ([Combat.c:2710](../../MoM/src/Combat.c#L2710), asm 16)

Faithful. Two statements in the asm's order: clear `Moving` on the *outgoing* unit — indexed by `_active_battle_unit`, before it is overwritten — then store the parameter into `_active_battle_unit`. The store carries no `byte ptr` override because the struct member settles the width: `Moving dw` in `s_BATTLE_UNIT` (`WIZARDS.inc`), matching `int16_t mid_move` at [Combat.h:1290](../../MoM/src/Combat.h#L1290). No `sub sp` — no locals — and one word parameter at `bp+6`.

### `Auto_Cast_Spell_And_Do_Combat_Turn` ([Combat.c:3707](../../MoM/src/Combat.c#L3707), asm 40)

Faithful. `winner` at `bp-2` is the only slotted local and production declares only that one; the parameter lives in SI, which is register allocation of the parameter itself, not an extra local.

- **The guard is un-inverted.** asm:13-15 is `cmp player_idx, [_combat_human_player]` / `jnz short loc_7ECC1` / `jmp short loc_7ECDB` — the Borland long-branch shape, so the surviving short `jnz` carries the source condition as written: `player_idx != _combat_local_player`. (`combat_human_player` to `_combat_local_player` is the rename already in this doc's ledger.)
- **Four arguments, right-to-left.** asm:17-24 pushes `_combat_wp`, `_combat_wy`, `_combat_wx__som_started_anim_ctr`, then `player_idx + 20`, and cleans with `add sp, 8`. That is `Combat_Cast_Spell((20 + player_idx), _combat_wx, _combat_wy, _combat_wp)`. `_combat_wx__som_started_anim_ctr` is the IDA label for `_combat_wx` — one storage reused for two purposes, already recorded in [Combat-Init_Prep_Etc.md:390](Combat-Init_Prep_Etc.md#L390) and [Combat-End_Of_Combat.md:332](Combat-End_Of_Combat.md#L332).
- **The winner test runs the block when equal.** asm:28-29 is `cmp [bp+winner], e_ST_UNDEFINED` / `jnz short @@Done`, the jump-skips-the-body idiom, so the source reads `winner == ST_UNDEFINED`. `Update_Combat_Enchantments_Icon_And_Help()` then `Auto_Do_Combat_Turn(player_idx)` in that order.

### `Combat_Grid_Screen_Coordinates` ([Combat.c:21862](../../MoM/src/Combat.c#L21862), asm 44)

Faithful. `sx = ((cgx - cgy) * 16) + 158` is `sub` / `shl ax, 4` / `add 158` (asm:17-22); `sy = ((cgx + cgy) * 8) - 80` is `add` / `shl ax, 3` / `add -80` (asm:23-28). The sub-cell offsets follow the same shape — `(ox - oy) * 2` added to `sx` via `shl ax, 1` (asm:29-32) and a bare `(ox + oy)` added to `sy` (asm:33-35) — then both are stored through the out-pointers. The two accumulators live in DX and SI with no stack slots, so their declaration order carries no constraint.

The asm parameter names are misleading and worth not copying: `cgc2` at `bp+6` is the **x** cell and `cgc1` at `bp+8` is the **y** cell, which is why production's `cgx, cgy` order is right.

### The cache pair — `Combat_Cache_Read` ([Combat.c:22009](../../MoM/src/Combat.c#L22009)) and `Combat_Cache_Write` ([Combat.c:22034](../../MoM/src/Combat.c#L22034)), asm 64 each

Mirror images: spill `World_Data` to `COMBAT.TMP` before combat and read it back after. Both call the reconstructed `seg011` wrapper in [GFILE.c](../../MoX/src/GFILE.c) — `gfopen`, `gfread`/`gfwrite` twice, `gfclose` — so both are 1:1 with the listing and neither carries a substitution.

**The frame is one stack local.** `Pass_2_Paras` at `bp-2` (IDA's name in the Read listing; `more_world_data_size_PR` in the Write one) is the only slot; `world_data_size` is SI and `file_handle` is DI.

`world_data_size_PR > 2048` is `cmp 2048` / `jle` to the else (asm:12-13); the clamp writes **2047**, not 2048, and `more_world_data_size_PR = tmp_World_Data_Paras - 2047` is emitted as `add ax, -2047` (asm:14-17) — note it re-reads the **global**, not the value just clamped, and production does the same. Both transfer calls scale by `shl ax, 4`, i.e. `SZ_PARAGRAPH_B` = 16 ([MOX_BASE.h:82](../../MoX/src/MOX_BASE.h#L82)).

**Why the clamp is at 2048.** 2048 paragraphs × 16 = 32768, which is exactly where a 16-bit offset dies. The pair reads and writes in two passes for that reason and no other.

**The leading argument is a far pointer, not a file offset.** `WIZARDS/seg011/gfread.asm` declares `ptr= dword ptr 6`, `size= word ptr 0Ah`, `nitems= word ptr 0Ch`, `uu_file_handle= word ptr 0Eh`, and `gfwrite` has the identical frame. The caller's `push [World_Data]` is a single word — the **segment** half — and the `xor ax, ax` or `shl ax, 4` beside it is the **offset** half, so the first call passes offset `0` within that segment and the second passes `world_data_size_PR * 16`. `World_Data` is [`SAMB_ptr` = `unsigned char *`](../../MoX/src/MOX_TYPE.h#L26) in the port, so `World_Data + (world_data_size_pr * SZ_PARAGRAPH_B)` is the exact translation. The callee also carries the zero-segment sentinel `_fstrcpy` uses — it tests the segment half against 0 and substitutes `DS`.

**The fourth argument is passed and ignored, in both the original and the port.** The caller pushes five words (`add sp, 0Ah`), but `gfread`/`gfwrite` take the handle from the `_gf_fhandle` global that `gfopen` set. [GFILE.c:52-63](../../MoX/src/GFILE.c#L52-L63) reproduces that faithfully, so passing `file_pointer` at the call site is correct rather than redundant.

**No overflow in the port.** `world_data_size_pr` is clamped to 2047, so `world_data_size_pr * SZ_PARAGRAPH_B` is at most 32752 and fits `gfread`'s `int size` parameter — the two `size_t` widening locals an earlier reconstruction carried were unnecessary and are gone.

**The second transfer runs unconditionally**, with a length of 0 when there is no second pass. That is what the listing does; there is no guard to add.

### `Combat_Node_Type` ([Combat.c:6249](../../MoM/src/Combat.c#L6249), asm 82)

Faithful. Both loops carry compound conditions and both are intact:

- Outer, asm:70-75 — `cmp _CX_itr_nodes, e_NUM_NODES` / `jge` then `cmp _DI_node_type, e_ST_UNDEFINED` / `jnz`, i.e. `(itr_nodes < NUM_NODES) && (node_type == ST_UNDEFINED)`.
- Inner, asm:56-67 — `power` loaded and compared with `jle` to exit, then the same `node_type` test, i.e. `(itr_auras < power) && (node_type == ST_UNDEFINED)`.

The `wp` mismatch at asm:21-22 jumps to `loc_84868`, which is the outer `inc`, so it really is a `continue` and not a skipped body. The `Aura_Xs` / `Aura_Ys` pair short-circuits — both mismatches jump to `@@Next_Node` (asm:35, asm:45). All three locals are register-allocated, so no frame ordering applies. The proc is spelled `Combat_Node_type` in IDA; the rename is already in the ledger.

### `Move_Confused` ([Combat.c:6627](../../MoM/src/Combat.c#L6627), asm 77)

Faithful. The compound loop condition is carried intact — `(i < 600) && (target_found == ST_FALSE)` — with nothing jumping out of the body, matching asm:46-50. Everything else matches: `Random(21)` and `Random(22)` against `COMBAT_GRID_WIDTH 21` / `COMBAT_GRID_HEIGHT 22` ([Combat.h:143-144](../../MoM/src/Combat.h#L143-L144)), each followed by `dec ax` for the 1-based-to-0-based idiom; the cell test reads a pointer out of the row table (`shl bx, 1` then `mov bx, [g_combat_grid_action_map+bx]`, matching `int8_t * g_combat_grid_action_map[COMBAT_GRID_HEIGHT]` at [Combat.h:1421](../../MoM/src/Combat.h#L1421)) and compares the byte against `e_ST_UNDEFINED_DB`; `Move_Battle_Unit(battle_unit_idx, target_x, target_y)` matches the push order; and the two tail stores use `imul 6Eh` = 110 = `sizeof(s_BATTLE_UNIT)`.

The frame confirms the declaration order exactly — `Target_Found` at `bp-0Ah`, then `cgx`, `cgy`, `Target_Y`, `Target_X` — against production's `target_found, random_x, random_y, target_y, target_x`.

## More verified faithful

### `Combat_Structure` ([Combat.c:15603](../../MoM/src/Combat.c#L15603), asm 84)

Faithful. `cs_NONE` is the `xor si, si` initialiser; `Square_Is_Sailable` is tested with `or ax, ax` / `jz`, i.e. `!= ST_FALSE`; both city-enchantment tests use `jle` for `> 0` and write **2** into the `Dfndr` byte of `True_Light` and `Darkness`. The map read is byte-addressed — `wp * 4800`, `wy * 120`, then `wx * 2` for a word cell — matching `int16_t (*p_world_map)[WORLD_HEIGHT][WORLD_WIDTH]` ([MOM_DAT.h:3931](../../MoX/src/MOM_DAT.h#L3931)) with `e_WORLD_SIZE_DB` 4800 and `e_WORLD_WIDTH_DB` 120. The three node tests are separate `if`s, not an `else if` chain, exactly as production has them.

Two things that look like findings and are not: the modulo is emitted as unsigned `div` while production writes `%` on a signed `int16_t`, and `_combat_environ == 1` is a literal where `cnv_Enemy_City` exists ([Combat.h:579](../../MoM/src/Combat.h#L579)). Both are house-wide idioms — the modulo form appears at six sites including [MAPGEN.c:7163](../../MoM/src/MAPGEN.c#L7163), and the literal appears in already-done-done code at [Combat.c:15787](../../MoM/src/Combat.c#L15787). Changing them here alone would make this function the odd one out.

### `Undeployable_Battle_Units_On_Water` ([Combat.c:15654](../../MoM/src/Combat.c#L15654), asm 135)

Faithful. The three-clause disqualifier short-circuits to a shared `xor ax, ax` return, and the second pass ANDs four conditions before writing `bus_Uninvolved` and bumping the count. The enchantment test is a split 32-bit compare — `and dx, 0` on the low word and `and ax, UE_WINDWALKING` on the high (asm:50-54) — which is `UE_WIND_WALKING` = `0x00010000` ([MOM_DEF.h:914](../../MoX/src/MOM_DEF.h#L914)) with its high word as the asm's mask value of 1. The unsailable path falls into the same `return unit_count` exit, and `unit_count` is still 0 there.

### `Draw_Spell_Information_Window` ([Combat.c:5549](../../MoM/src/Combat.c#L5549), asm 125)

Faithful, and the frame settles the array sizes exactly: `colors` at `bp-10h` (4 bytes), `Range_Display_String` at `bp-0Ch` (6), `Range_Number_String` at `bp-6` (6), `sub sp, 10h` — matching production's `uint8_t colors[4]`, `char [6]`, `char [6]` in that declaration order. The `value < 10` arm is the physically-first block, so it is the source `if`; the nested `Range_Number_String[1] != '0'` test is the jump-skips-the-body idiom. All eight text calls match in order and argument order.

Worth noting alongside `Combat_Screen_Draw`: `Set_Font_Colors_15` copies 16 bytes ([Fonts.c:361](../../MoX/src/Fonts.c#L361)) from a **4**-byte `colors` here, so this site over-reads further than that one does. Faithful in both.

### `Draw_Combat_Unit_Display` ([Combat.c:7526](../../MoM/src/Combat.c#L7526), asm 566)

## Deviation

**D8 — the `Gradient_Fill` call is under-supplied in the original.** `seg014/Gradient_Fill.asm` declares nine parameters (`bp+6` through `bp+16h`), and this function calls it twice, under-supplying both times. The box fill at asm:35-44 pushes **six** (`add sp, 0Ch`); the hits-bar fill at asm:331-346 pushes **five** (`add sp, 0Ah`). Either way the original leaves the trailing parameters reading whatever sits above the pushed words — its own return address and saved registers. [Combat.c:7644](../../MoM/src/Combat.c#L7644) and [Combat.c:7706](../../MoM/src/Combat.c#L7706) pass `ST_NULL` for the missing trailing parameters.

This one cannot be reproduced 1:1 in C99 — a short-argument call was legal in K&R C and is a constraint violation now. Production's explicit `ST_NULL` padding is the established house form, used at every other `Gradient_Fill` site ([AdvsrScr.c:827](../../MoM/src/AdvsrScr.c#L827), [ArmyList.c:664](../../MoM/src/ArmyList.c#L664)). Recorded, not to be changed.

## Verified faithful

**Frame.** Twelve slotted locals from `bp-1Ah` to `bp-2` in declaration order — `resist_value`, `defense_value`, `colors[2]`, `attack_strength`/`level_cursor`, `y2`, `x2`, `level_icon_count`, `battle_unit_movement_mode`, `level_icon_seg`, `unit_idx`, `unit_type`, `unit_owner_idx` — with `x1` in SI and `y1` in DI, matching [Combat.c:7979-7994](../../MoM/src/Combat.c#L7979-L7994). IDA labels the last slot `unit_hero_slot_idx__Attr_Display_Var` because the compiler shared one slot between two locals with disjoint lifetimes; production declaring `unit_hero_slot_idx`, `scan_icon_idx` and `level_tier_base` separately is correct and is not a divergence.

**Entry guard.** asm:23-29 is two Borland long-branch fixups, so both surviving short jumps carry the source conditions un-inverted: `!= ST_UNDEFINED` and `< 36`.

**The box.** Four `Line` calls in order with the right corners — `(x1,y1,x1,y2,230)`, `(x1,y1,x2-1,y1,230)`, `(x2,y1,x2,y2,237)`, `(x1+1,y2,x2,y2,237)`.

**The movement row.** `movement_points != 0` takes the physically-first arm calling `Print_Moves_String(x1+9, y1+24, movement_points, ST_TRUE)` (four words, `add sp, 8`); the zero arm calls `Print_Integer_Right` (three words, `add sp, 6`). Both read the field as a signed byte via `cbw`. The icon selection that follows is three separate tests — `== 2` sets 12, then `== 4 || == 5` sets 13 — over a default of 11, not an if/else chain.

**Mana and ammo carry different signedness, and both match.** asm:429-430 is `cmp mana, 0` / **`jbe`** — unsigned — while asm:464-465 is `cmp ammo, 0` / **`jle`** — signed. Two branch families on two adjacent fields, exactly as [Combat-Combat_Screen.md](Combat-Combat_Screen.md) already recorded for this struct.

**The level tiers.** `Level > 0` gates the block (`jg`, else a far jump to the exit); inside, `cmp 6` / `jle` puts the `> 6` case in the fall-through and `cmp 3` / `jle` does the same for `> 3`, giving the `if` / `else if` / `else` production has, with icons 17/16/15 and start values 6/3/0.

**The name row.** `unit_idx`, `unit_owner_idx`, `unit_hero_slot_idx` and `unit_type` are read in that order off `_UNITS[unit_idx]`, which is indexed `shl ax, 5` — 32 bytes per `s_UNIT`. The first three sign-extend with `cbw`; `unit_type` uses `mov ah, 0` instead, a zero-extend, matching `uint8_t type` at [MOM_DAT.h:1848](../../MoX/src/MOM_DAT.h#L1848) where the other three are signed. `unit_hero_slot_idx != ST_UNDEFINED` picks the hero name — `owner * sizeof(s_WIZARD) + slot * sizeof(s_OWNED_HERO) + offset _players.Heroes.name` — against the unit-type name otherwise, which is a word read at offset 0 of the 0x24-byte `_unit_type_table` element, i.e. the pointer production dereferences. `Print_Centered` then centres on `x1 + (x2 - x1) / 2` using the signed-halve idiom `cwd` / `sub ax, dx` / `sar ax, 1`.

**The melee and ranged rows** (asm:178-247). Both have the same shape: call the icon function, store into the shared `bp-2` slot, `cmp e_ST_UNDEFINED` / `jz` to skip the row. Every coordinate matches — melee number at `(x1+9, y1+10)` and icon at `(x1+11, y1+8)`; ranged number at `(x1+9, y1+17)` and icon at `(x1+11, y1+15)`.

**The hits bar and the defense/resist rows** (asm:310-422). Movement icon at `(x1+11, y1+22)`; `cnst_Hits_2` at `(x1+3, y1+32)`; the bar's `Gradient_Fill` at `(x1+19, y1+34)`-`(x1+38, y1+34)` with fill type 3, its `Line` underneath at `y1+35` in colour 1, and `Draw_Active_Unit_Damage_Bar(_scanned_battle_unit, x1+19, y1+34)`. Defense prints at `(x1+48, y1+10)` with its icon at `(x1+50, y1+8)`; resist prints at `(x1+48, y1+17)` with its icon at `(x1+50, y1+15)`.

The two icon reads there are the independent confirmation of the hardcoded indices: IDA labels them `IMG_CMB_ScanIcons@.Stat.Defense@` and `IMG_CMB_ScanIcons@.Stat.Resist@`, which are entries **9** and **14** — exactly what production uses, and exactly what the `COMPIX.LBX` 61-78 comment block at [Combat.c:7166-7183](../../MoM/src/Combat.c#L7166-L7183) names "defense" and "resistance".

## Reopened deviations

**The cell-effect counter is rescaled on purpose.** asm:621 wraps `CMB_ChasmAnimStage` at 7; [Combat.c:5430](../../MoM/src/Combat.c#L5430) wraps `cmbt_cell_effect_frame` at `CELL_EFFECT_ANIM_FRAME_COUNT * CELL_EFFECT_ANIM_HOLD` = 49. This is the host-side slowdown already recorded at [Combat-Combat_Screen_Map_Draw.md:220](Combat-Combat_Screen_Map_Draw.md#L220), where the paired divide lives. Not a finding — but the two halves must move together if either is ever changed.

**Renames not yet in the ledger.** `CMB_HumanTurn` to `m_turn_is_local`, `CMB_VortexAnimStage` to `_combat_vortex_anim_frame`, `CMB_ChasmAnimStage` to `cmbt_cell_effect_frame`, `CMB_MudAnimStage` to `_combat_mud_anim_phase`, `CMB_CityName` to `_combat_city_name`, `magic_set.Auto_Unit_Info` to `magic_set.auto_unit_information`, `Combat_Screen_Draw_Buttons` to `Tactical_Combat_Draw_Buttons`, `Combat_Grid_Entities__WIP` to `Combat_Grid_Entities`, `Cycle_Palette_Color__STUB` to `Cycle_Palette_Color`.

**A debug block with no asm counterpart.** [Combat.c:5442-5447](../../MoM/src/Combat.c#L5442-L5447) calls `Combat_Screen_Draw_Debug_Information()` under `#ifdef STU_DEBUG`. Guarded, so it is a project addition rather than a divergence.


# Findings

None open.

# Deviations

Five, all deliberate and all still present. Rechecked against both the listings and the current source on 2026-08-08.

## D1 — `grid_sx` / `grid_sy` are 64-bit

[Combat.c:1435-1436](../../MoM/src/Combat.c#L1435-L1436). asm slots `Grid_X` / `Grid_Y` are `word ptr -0Ah` / `-0Ch`; production declares them `int64_t`, with the original `int16_t` declarations preserved directly above at [10442-10443](../../MoM/src/Combat.c#L10442-L10443), commented and marked `DOMSDOS`.

Platform change, not a reconstruction difference: [`Add_Grid_Field`](../../MoM/src/Combat.c#L1671) takes their addresses and the host writes pointer-width values through them. Every read casts back through `(int16_t)` — see [Combat.c:1849](../../MoM/src/Combat.c#L1849). Leave it.

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

[Combat.c:19828](../../MoM/src/Combat.c#L19828) in `Claim_EMS_Page_For_Figure_Set`, and [Combat.c:19980](../../MoM/src/Combat.c#L19980) in `Map_Tile_EMS_Page_As_Sandbox`.

`EMM_FIGUREX_Init__HACK.asm:31-33` and `EMM_TILEX_Init__HACK.asm:5-8` both call `EMM_MapMulti4`. Production computes `EmmHndl_* + (logical_page * SZ_EMM_LOGICAL_PAGE)` instead and leaves the original call as a `TODO` comment on the same line. Consistent across both sites.

## D5 — the figure block is over-allocated

[Combat.h:588-590](../../MoM/src/Combat.h#L588-L590), the `FIGUREX_POINTER` macro.

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
- **The attacker-side seat call passes `_current_player_idx`, not a literal** (asm:290), and the `else` arm passes `_combat_total_unit_count - defender_unit_count` (asm:294-297).
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

Faithful. Production [Combat.c:22628](../../MoM/src/Combat.c#L22628).

Four far allocations then five near ones: 348, 482, 10, 9, then `Near_Allocate_First(504)` and `Near_Allocate_Next` of 504, 1008, **60**, **60** (asm:5-73). It opens with `Allocate_First_Block`, which **resets** the screen segment's allocation chain — everything afterwards, here and through `Combat_Screen_Load_Resources`, carves with `Allocate_Next_Block`. The two are strictly ordered and calling the second without the first carves from wherever the previous screen left the pointer.

The near block is redundant *on the `Combat_Screen` path* — `Combat_Screen_Load_Resources` runs next and its tail call re-issues `Near_Allocate_First`. It is **not** redundant on the other path: `NX_IDK_CombatInit_Tactical` calls this function and then `Build_Battlefield` with no resource load between them, so those five pointers are the ones that survive.

IDA's `CMB_Projectiles@` is production's `_missiles`; the sizes match and only the name differs.

# Combat_Screen_Load_Resources

Faithful. Production [Combat.c:6718](../../MoM/src/Combat.c#L6718).

- **The allocation sequence is 249, 6, 6, 6, 3, then 55 × 18** — nothing else (asm:7-50, asm:99-109, asm:363-376).
- **Fifty-nine COMPIX.LBX entries across 32 call sites**, three of them loops. Every load targets `compix_lbx_file__ovr103`; nothing else is opened.
- **The lock-button LBX indices are not sequential** and production reproduces the jumble exactly: cancel 22, spell 23, wait 24, **done 28**, auto 26, **info 25**, flee 27 (asm:240-295). Easy to "tidy" into 23-28 order; do not.
- **Fifteen combat-enchantment icons** from LBX entries 5-15, then 41, 60, 79, 80 (asm:120-239) — the four out-of-run indices are correct.
- **`combat_enchantments` is allocated 3 paragraphs and then zeroed for 30 bytes** (asm:99-119). 3 paragraphs is 48 bytes; only the first 30 are cleared, which is the fifteen `{Attkr, Dfndr}` pairs.
- **`Mark_Block` / `Release_Block` bracket only the `battle_unit_picts_seg` loop** (asm:359, asm:378), not the LBX loads above it. Those 18 blocks are *reserved*, not loaded — the figure compositor fills them later, and the mark is what lets combat rewind to that point.
- **The picts loop bound is `e_MAX_BATTLE_UNIT_COUNT`** = `12h` = 18 in `WIZARDS.inc`, matching production's `MAX_BATTLE_UNIT_COUNT` ([MOM_DEF.h:65](../../MoX/src/MOM_DEF.h#L65)).

# Allocate_Combat_Near_Buffers

Faithful. Production [Combat.c:6772](../../MoM/src/Combat.c#L6772).

Seven allocations then a 22-iteration loop then one more: 504, 504, 1008, 60, 60, 12, `22 ×` 21, 20. The loop literals settle the grid constants outright — asm:49 is `cmp _SI_itr, 22` and asm:40 is `mov ax, 21`, matching `COMBAT_GRID_HEIGHT` 22 and `COMBAT_GRID_WIDTH` 21 ([Combat.h:143-144](../../MoM/src/Combat.h#L143-L144)). Only the first call is `Near_Allocate_First`; all others are `Near_Allocate_Next`.

It is called five times, not once: the tail of `Combat_Screen_Load_Resources`, and four times inside the input loop after each modal sub-window returns. `Near_Allocate_First` resets a single shared 4400-byte DGROUP arena that every screen in the game re-carves for itself, so combat has to re-establish its layout whenever it gets control back.

# Build_Battlefield

Faithful. Production [Combat.c:20016](../../MoM/src/Combat.c#L20016).

- **`Location_Type` is resolved by four independent passes, each overwriting the last** — cities, then towers, then nodes, then lairs (asm:54-374). A square that is both a tower and a lair ends up a lair. No `else`, no early exit.
- **The node arm subtracts, the lair arm adds** — asm:316-318 is `mov dx, 7` / `sub dx, ax`, asm:368 is `add ax, 4`. Production writes these as `clt_SorceryNode - _NODES[itr].type` and `clt_Tower + _LAIRS[itr].type`, which is the same arithmetic with the constants named. Against `nt_ {Sorcery=0, Nature=1, Chaos=2}` the subtraction yields 7, 6, 5 — the `clt_` enum is ordered to make that work ([Combat.h:618-621](../../MoM/src/Combat.h#L618-L621)).
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

Faithful. Production [Combat.c:4754](../../MoM/src/Combat.c#L4754).

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
- **`BU_CASTER_40()` / `BU_CASTER_20()` / `BU_LUCKY()` take no arguments** and bind the enclosing `battle_unit` textually ([Combat.h:505-507](../../MoM/src/Combat.h#L505-L507)). They resolve to the parameter here, matching asm:117/123/136.

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

Both faithful. Production [Combat.c:22151](../../MoM/src/Combat.c#L22151) and [Combat.c:22151](../../MoM/src/Combat.c#L22151).

These are the algebraic inverse of the grid-to-screen transform at [Combat.c:22114-22115](../../MoM/src/Combat.c#L22114-L22115). With `u = (screen_x - 158) / 2` and `v = screen_y + 80`:

```
u = 8 * (cgx - cgy)      cgx = (u + v) / 16
v = 8 * (cgx + cgy)      cgy = (v - u) / 16
```

The `-158` and `+80` translate the screen origin to the grid origin; the `/2` normalizes the 2:1 isometric aspect so the rest is a plain 45-degree rotation; the `/16` is 16 normalized units per cell.

- **The two listings together prove the grouping.** Y saves the halved term and restores it (asm:14-18, `push ax` then `pop dx` then `sub ax, dx`), which is what Borland emits for a genuine grouped operand. X has no `push`/`pop` — asm:13-15 accumulates — which is why the source is `+ screen_y + 80` and not `+ (screen_y + 80)`.
- **The `Nope.` in the header comment is wrong, not the formula.** The worked example (78, 73) resolves to cell (7, 12), and the forward transform confirms it: `16 * (7 - 12) + 158 = 78` and `8 * 19 - 80 = 72`, one pixel above the sampled row. `cgy` comes out 12 from the same inputs.
- **Both divisions truncate toward zero.** For the `/16` that never matters on a 320x200 screen — `u+v` and `v-u` both stay non-negative, since `v` is at least 80 and `u` bottoms out at -79. The `/2` is different: `screen_x - 158` is negative across the whole left half of the map, so odd pixel columns there round the opposite way from their mirror on the right. A one-pixel seam, symmetric about the centre line.

# Combat_Structure

Production [Combat.c:15802](../../MoM/src/Combat.c#L15802). **One finding — R18.** Everything else faithful.

- `Square_Is_Sailable(wx, wy, wp)` argument order confirmed by push order (asm:14-17); `cs_NONE` is 0, matching `xor si,si`.
- The two city-enchantment writes are `> 0` tests (`jle`) storing the literal 2 into the attacker/defender pair (asm:30-43).
- The three node tests are consecutive `if`s, not `else if` (asm:66-76), so a later match overwrites an earlier one.

**Deviation — `_combat_environ == 1`.** [Combat.c:15921](../../MoM/src/Combat.c#L15921) uses the literal where asm:23 names `cnv_Enemy_City`, which [Combat.h:579](../../MoM/src/Combat.h#L579) defines as 1. Same value; the enum name belongs there.

# Undeployable_Battle_Units_On_Water

Production [Combat.c:15838](../../MoM/src/Combat.c#L15838). **Three findings — R19, R20, R21.**

The rest is faithful: two separate passes over `_combat_total_unit_count`, the first bailing with `return 0` and the second setting `bus_Uninvolved` and counting. The second pass's four-term guard — `controller_idx == player_idx`, `carry_capacity == 0`, `!MV_FLYING`, `!MV_SWIMMING` — matches asm:90-114 term for term and in order.

# Combat_Grid_Screen_Coordinates

Production [Combat.c:22108](../../MoM/src/Combat.c#L22108). **One finding — R22.**

The arithmetic itself is exact: `(cgx - cgy) * 16 + 158` and `(cgx + cgy) * 8 - 80` via `shl 4` / `shl 3` (asm:17-28), then `+= (something_x - something_y) * 2` and `+= (something_x + something_y)` (asm:29-35). This is the forward transform that [Get_Combat_Grid_Cell_X](../../MoM/src/Combat.c#L21903) inverts.

# Load_Combat_Terrain_Pictures

Faithful. Production [Combat.c:22235](../../MoM/src/Combat.c#L22235).

- **The `cts_Water` early return is a bare zero test** — asm:19 is `or di,di / jnz`, and `cts_Water` is 0.
- **Five terrain arms, four filename pairs.** Plains, Hills and Forest share one arm (asm:24-29); Desert, Mountains and Tundra get their own. Every arm picks on `wp == 0`, and `ARCANUS_PLANE` is 0.
- **Three sequential load loops with the right bases** — 48 entries into `_combat_terrain_pict_segs` from index 0, then 5 into `_combat_tree_pict_segs` from 48, then 5 into `IMG_CMB_Rocks` from 53 (asm:103-154).
- **The filename buffer is 30 bytes** in both: asm puts it at `bp-20h` with `temp_seg` at `bp-2`, and `LEN_STRING` is 30 ([MOX_BASE.h:27](../../MoX/src/MOX_BASE.h#L27)). The 24 zero initializers in production are a partial initializer; C zero-fills the rest.

The asm returns 0 in `ax` on the water path (asm:21) while the normal exit leaves `ax` undefined, so the `void` return in production is compatible with the bytes. Nothing to change.

# Claim_EMS_Page_For_Figure_Set and Map_Tile_EMS_Page_As_Sandbox

Both faithful. Production [Combat.c:19828](../../MoM/src/Combat.c#L19828) and [Combat.c:19980](../../MoM/src/Combat.c#L19980), against `ovr153/EMM_FIGUREX_Init__HACK.asm` (77) and `ovr153/EMM_TILEX_Init__HACK.asm` (41).

These are two different functions and the earlier draft of this doc paired the wrong listing with the wrong name. `FIGUREX_MAP` ([Combat.h:488](../../MoM/src/Combat.h#L488)) expands to `Claim_EMS_Page_For_Figure_Set`, which is `EMM_FIGUREX_Init__HACK`; `EMM_TILEX_Init__HACK` is the no-argument `Map_Tile_EMS_Page_As_Sandbox`, called from `Load_Combat_Terrain_Pictures` and `Combat_Compose_Background`.

- `logical_page = (bufpi * 3) / 2` — `imul 3` then the signed-halve idiom (asm:13-18).
- The four `farpokew` calls match `SET_2B_OFS` in order and in argument order: MemSig1, MemSig2, size, used. Sizes differ between the two functions and both are right — **1591** paragraphs for the figure page (asm:54), **3071** for the tile page (`EMM_TILEX_Init__HACK.asm:25`).
- The figure-page pokes are all at `EMS_PFBA + offset`; the tile-page pokes are at bare `EMS_PFBA`.

**Deviation D5 — the EMS mapper call is replaced by flat arithmetic.** asm:31-33 and `EMM_TILEX_Init__HACK.asm:5-8` both call `EMM_MapMulti4`; production computes `EmmHndl_* + (logical_page * SZ_EMM_LOGICAL_PAGE)` and leaves the original call as a `TODO` comment. Deliberate host substitution, consistent across both functions.

# Combat_Figure_Load

Faithful. Production [Combat.c:22401](../../MoM/src/Combat.c#L22401).

- **The filename is built in four steps and the plural test is `< 10`** — asm:71-72 is `cmp ax, 10 / jge`, so `str_figure_plural_s__ovr163` is appended only below ten. `itoa`, `strcpy`, the conditional `strcat`, then `strcat` of the buffer, then `strcat` of the empty string (asm:47-94).
- **`(unit_type / 15) + 1` is computed three separate times** in the asm (asm:51-55, 66-70, 95-98) rather than cached, and production writes it out three times too.
- **`entry_num = (unit_type % 15) * 8`** — the same `idiv` taking the remainder from `dx`, then `shl dx, 3` (asm:95-101).
- **The load loop is 8 wide** and stores word-sized segments (asm:102-123).
- **`Combat_Figure_Compose(bufpi, 0, 0, 0, 0)`** is present and the push order confirms `bufpi` is the first argument (asm:124-133).
- Frame order matches: `file_name` (20 bytes at `bp-24h`), `buffer` (6 at `bp-10h`), the figure pointer, `offset`, `entry_num`.

**Deviation D5 (setup pass) — the figure block is over-allocated.** `FIGUREX_POINTER` ([Combat.h:588-590](../../MoM/src/Combat.h#L588-L590)) asks `Allocate_First_Block` for **33** paragraphs; asm:33-38 asks for **2**. The host needs more than 2 because `SAMB_ptr` is wider than a real-mode segment word, but 33 does not follow from anything written down, and the comment beside it does not explain the number. Settle the figure before this bundle closes.

# Deploy_Battle_Units

Faithful. Production [Combat.c:11588](../../MoM/src/Combat.c#L11588).

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

- **All twelve deployment constants match the bytes.** `DEPLOY_DFNDR_ROW1..3` are 8, 7, 6 (asm:27-39); `DEPLOY_ATTKR_ROW1..3` are 14, 15, 16 (asm:42-53); `DEPLOY_COL1..4` are 12, 11, 13, 10 repeated three times (asm:55-66). All ten `#define`s are at [CMBTDEF.h:153-162](../../MoM/src/CMBTDEF.h#L153-L162).
- **The return value of `Undeployable_Battle_Units_On_Water` is discarded** — asm:21-23 is `push` / `call` / `pop cx` with no use of `ax`.
- **The two player-identity comparisons use opposite operand order, and production preserves both.** asm:24-25 loads `player_idx` first (`player_idx == _combat_defender_player`, [11603](../../MoM/src/Combat.c#L11603)); asm:180-181 and asm:246-247 load `_combat_defender_player` first (`_combat_defender_player == player_idx`, [11695](../../MoM/src/Combat.c#L11695) and [11725](../../MoM/src/Combat.c#L11725)). Same for `_combat_attacker_player` at asm:321-322.
- **The wall-corner skip is a loop, the central-structure skip is not.** asm:190-193 `jz`s back to `inc di` and re-tests, so squares 2 and 3 are skipped repeatedly; asm:198-200 tests square 9 once. Production writes the first as `while` and the second as `if` ([11697-11716](../../MoM/src/Combat.c#L11697-L11716)).
- **`ctr` is not reset between the melee and ranged placement loops.** asm:243 resets only `si`. The ranged group continues on from wherever the melee group stopped, which is what makes the whole twelve-square layout fill in order.
- **`Clear_Structure` argument order confirmed by push order** — asm:158-163 pushes `Ranged_Count`, `2`, `&Ranged_Units`, `Highest_Melee_Unit`, so the call is `(slot, array, element_size, count)` ([11686](../../MoM/src/Combat.c#L11686)).
- **The facing pass runs over every battle unit, not just the deployed ones**, and rewrites `target_cgx` / `target_cgy` for any unit this player controls (asm:308-368).

## OGBUG — faithful, do not fix

**The highest-melee scan indexes the wrong array.** [Combat.c:11676-11679](../../MoM/src/Combat.c#L11676-L11679) iterates slots `0..ranged_battle_unit_count-1` and reads `battle_units[itr_ranged_slots].melee` — a *slot* used as a battle-unit index — then uses the winning slot as `ranged_battle_unit_idxs[...]`. asm:120-125 does exactly the same thing, with no `Ranged_Units` lookup anywhere in the block:

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

Three calls in order — `Graphics_Cache_Reset`, `Reload_Combat_Graphics_Cache`, `Spellbook_Load_Small_Pictures` (asm:5-10) — and nothing else. Production [Cache_Graphics_Combat](../../MoM/src/LOADER.c#L1832) adds a `LOG_DEBUG` on entry and exit; that is the project's logging instrumentation, not a reconstruction difference.

# Battle_Unit_Has_Spell_Ability

Faithful. Production [Combat.c:3603](../../MoM/src/Combat.c#L3603).

Six tests, each returning `ST_TRUE` on the first hit: `Attribs_2` against `USA_HEALING`, `USA_FIREBALL`, `USA_DOOMBOLT`, `USA_WEB`, then `Attribs_1` against `USA_SUMMON_DEMON_1 or USA_SUMMON_DEMON_2` and `USA_CASTER_40` (asm:14-66). Production reproduces the order, the field switch, and the paired demon mask as a single `&`.

**`USA_CASTER_20` is not tested.** asm:64 checks only `USA_CASTER_40` ([MOM_DEF.h:653-654](../../MoX/src/MOM_DEF.h#L653-L654) give 0x2000 and 0x4000), so a 20-mana caster does not count as having a spell ability here. That feeds the Spell button's condition.

# NIU_Who_Has_More_Leadership

Faithful. Production [Combat.c:3634](../../MoM/src/Combat.c#L3634).

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

---

# The two attack-icon pickers

`ovr105` p11 and p12, walked 2026-08-26, 138 asm lines. Both faithful. They pick the weapon glyph
drawn beside a unit's melee and ranged attack strengths — `combat_weapon_icon_segs[icon_idx]` in the
active-unit panel ([Combat.c:5634](../../MoM/src/Combat.c#L5634), [Combat.c:5641](../../MoM/src/Combat.c#L5641))
and `combat_scan_icon_segs[icon_idx]` in the scan window ([Combat.c:7606](../../MoM/src/Combat.c#L7606),
[Combat.c:7613](../../MoM/src/Combat.c#L7613)). Both callers gate on `!= ST_UNDEFINED`, so returning
`-1` means *draw no icon at all* rather than *draw a default one* — which matters for the OGBUG below.

## `Battle_Unit_Melee_Attack_Icon` ([Combat.c:7403](../../MoM/src/Combat.c#L7403), asm 61)

Faithful, all 61 lines. No `sub sp` and no stack locals: `icon_idx` is CX and `weapon_type` is DI,
both annotated `// _CX_` / `// _DI_` in production, and SI caches the parameter.

`icon_idx = ST_UNDEFINED` (asm:12), then `cmp melee, 0 / jle @@Done` for `melee > 0` (asm:18-19),
then `weapon_type = Weapon_Plus1` (asm:25-27) and `or di, di / jle` for `weapon_type > 0` (asm:28-29)
— the `or reg, reg` idiom standing in for `cmp reg, 0`.

**The selector is a compare chain, not a jump table.** asm:30-37 tests 2, 3 and 4 in turn with
`jz`, then falls through to a default. Three cases is below Borland's table threshold, so production's
`if` / `else if` / `else` is the faithful shape. The mapping crosses over and production keeps it:

| `weapon_type` | icon |
| --- | --- |
| 2 | 3 |
| 3 | 2 |
| 4 | 4 |
| anything else > 0 | 0 |
| <= 0 | 0 |

The two `icon_idx = 0` arms are separate stores in the listing (asm:48 and asm:53), reached by
different paths, and production writes both out rather than merging them.

**The header comment's question has an answer.** [Combat.c:7422](../../MoM/src/Combat.c#L7422) asks
`¿ no weapon type 1 ?`. Correct — there is no case 1, and `Weapon_Plus1` value 1 falls to the same
default as 5, 6 or any other unlisted positive value, giving icon 0. Nothing is missing from the
reconstruction; the OG simply special-cases only 2, 3 and 4.

## `Battle_Unit_Ranged_Attack_Icon` ([Combat.c:7456](../../MoM/src/Combat.c#L7456), asm 77)

Faithful, all 77 lines, and it carries a real OGBUG that production preserves correctly.

**The guard** (asm:21-40) is `ranged_type != ST_UNDEFINED` followed by three `ranged_type / 10`
tests against `rag_Boulder`, `rag_Missile` and `rag_Magic`. The listing recomputes the division for
each test — three separate `mov bx, 10 / cwd / idiv bx` sequences — and production writes the
division three times to match.

**The switch is a 17-entry value-table scan, not a computed jump.** asm:43-53 is
`mov cx, 17 / mov bx, offset vt_ranged_attack_weapon_type`, a `loop` comparing each entry, then
`jmp [word ptr cs:bx+22h]` into a parallel jump table. Both tables are in `ovr105/_misc.asm`, and
they line up positionally:

| value-table entries | jump target | icon | production cases |
| --- | --- | --- | --- |
| 0-3 — `rat_Rock`, `rat_Cannon`, `rat_Sling`, `rat_Unknown` | `rat_00_01_02_03` | 6 | 4 cases |
| 4-12 — `rat_Lightning` … `rat_Nat_Bolt` | `rat_04_05_06_07_08_09_10_11_12` | 1 | 9 cases |
| 13 — `srat_Thrown` | `rat_13` | 8 | 1 case |
| 14-16 — `srat_FireBreath`, `srat_Lightning`, `srat_StoneGaze` | `rat_14_15_17` | 7 | 3 cases |
| no match | `loc_864B2` | 5 | `default` |

All 17 accounted for, in order.

### The OGBUG — four cases the guard makes unreachable

`rag_Short` is 10, covering `{100 … 109}` ([UNITTYPE.h:267](../../MoM/src/UNITTYPE.h#L267)), and the
guard never tests for it. So the four `srat_` entries — `srat_Thrown` (100), `srat_FireBreath` (101),
`srat_Lightning` (102), `srat_StoneGaze` (103) — pass the value-table scan only if control gets that
far, and it never does. **`icon_idx = 8` (thrown) and `icon_idx = 7` (breath/gaze) are dead code in
the original.** SimTex wrote the cases, filled both tables, wired the jump — and then gated them out.

The consequence is not the default icon: a thrown- or breath-attack unit never enters the switch, so
`icon_idx` stays at the `ST_UNDEFINED` set at asm:12 and both callers skip the `FLIC_Draw` entirely.
Those units get **no ranged icon at all**.

Production records it in two lines above the guard — the mechanism at
[Combat.c:7485](../../MoM/src/Combat.c#L7485) (`the guard admits only ranged_type / 10 in {1, 2, 3}`)
and the consequence at [Combat.c:7486](../../MoM/src/Combat.c#L7486), which names all four dead
`srat_` cases. Between them they state the whole finding; nothing further is needed here.

### Two further observations, neither filed as a defect

- **`rat_Bow` (20) is the only `rat_` value in the 10-39 range missing from the value table.** It
  passes the guard — `20 / 10` is `rag_Missile` — reaches the scan, matches nothing, and takes the
  `default` to icon 5. Whether that is intended depends on what icon 5 depicts, which this walk did
  not establish; recorded so the next reader does not have to re-derive it.
- **`srat_MultiGaze` (104) and `srat_DeathGaze` (105) are absent from the table entirely.** Widening
  the guard to admit `rag_Short` would reach `srat_Thrown` through `srat_StoneGaze` but still leave
  these two on the `default`. Any future fix needs both halves.

# Listing notes — `ovr105`

- The jump-table label `rat_14_15_17` names entries 14, 15 and **17**, but the three entries it
  serves are 14, 15 and 16. IDA's label, not a table error — the `dw` list has exactly 17 entries and
  the last is index 16. Do not read the label as evidence of a missing entry.

---

# `Tactical_Combat_Draw_Buttons`

`o99p03`, walked 2026-08-27, 172 asm lines. IDA's `Combat_Screen_Draw_Buttons`. Faithful, no findings.

## `Tactical_Combat_Draw_Buttons` ([Combat.c:5489](../../MoM/src/Combat.c#L5489), asm 172)

Draws the six combat-screen buttons, each either live or in its locked variant.

No frame at all — no `sub sp`, no locals, no saved registers beyond `bp`.

**Every coordinate checks out.** The listing pushes literal 144/168/170/178/188 and production writes
them against `SCREEN_YBOT_CMBT`, which is 168 ([MOX_DEF.h:766](../../MoX/src/MOX_DEF.h#L766)): 168
itself, `+ 10` for 178 and `+ 20` for 188. All six `FLIC_Draw` calls and their `Set_Animation_Frame`
partners are present in listing order.

**The auto-combat arm is a clean early branch** — asm:5-49 draws the auto button live and the other
five locked, then `jmp @@Done`.

**`mana >= 3` is an unsigned compare and that is correct.** asm:63-64 is `cmp [...mana], 3 / jnb`.
`s_BATTLE_UNIT.mana` is `uint8_t` ([Combat.h:1279](../../MoM/src/Combat.h#L1279)), so the unsigned
`jnb` and production's `>=` agree on every value.

**The spell-button guard is two-tier, and the tiers matter.** asm:55-79: `cmp movement_points, 0 /
jle short loc_81724` skips the three unit-based tests but lands **on** the `CMB_WizCastAvailable`
test at `loc_81724`, which can still enable the button. So the condition is
`(movement_points > 0 && (mana >= 3 || has_ability || item_charges > 0)) || wizard_cast_available` —
the wizard-cast term is a fallback for the whole `&&`, not a fourth alternative inside it. Production
at [Combat.c:5511-5523](../../MoM/src/Combat.c#L5511-L5523) writes exactly that, relying on `&&`
binding tighter than `||`. Folding the fourth term into the inner group would lock the spell button
for a unit that is out of movement while the wizard still holds a combat cast.
