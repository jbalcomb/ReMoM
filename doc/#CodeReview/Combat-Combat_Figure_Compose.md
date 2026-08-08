Combat-Combat_Figure_Compose.md

SEEALSO:  Combat-Combat_Screen_Map_Draw.md
SEEALSO:  Combat-Combat_Figure_Compose.md
SEEALSO:  Combat-End_Of_Combat.md
SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoM-CombatScreen-Grid.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\BU_CreateImage__SEGRAX.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\Combat_Figure_Compose_USEFULL.asm


UU_BU_LoadFigureGFX()         ==>  



// WZD ovr153p23
void Combat_Figure_Compose_USEFULL(void)
    Combat_Figure_Banner_Color(player_idx);
    Combat_Unit_Enchantment_Outline_Draw(outline_magic_realm);
    Combat_Figure_Effect__WIP(figure_effect);
    Combat_Figure_Active_Red_Outline(battle_unit_idx);

Combat_Screen()
    |-> Combat_Screen_Draw()
        |-> Draw_Active_Unit_Window()
            |-> Combat_Figure_Compose_USEFULL()

Battle_Unit_Compose_Bitmap()
    Combat_Unit_Enchantment_Outline_Set(battle_unit_idx);
    Battle_Unit_Set_Animation_Flags(battle_unit_idx);
    Battle_Unit_Set_Invisibility_Effect(battle_unit_idx);
    // ~ Combat_Figure_Compose_USEFULL()
    Combat_Figure_Banner_Color(player_idx);
    Combat_Unit_Enchantment_Outline_Draw(enchantment_magic_realm);
    Combat_Figure_Effect__WIP(figure_effect);
    // ~ CMB_DrawEntities__WIP()
    for(cur_fig = 0; cur_fig < fig_cnt; cur_fig++)
    {
        Battle_Unit_Figure_Position(fig_max, cur_fig, &fig_x, &fig_y);
        Clipped_Copy_Bitmap(fig_x, fig_y - 4, battle_unit_scratch_seg, scratch_bitmap_seg);

Draw_Active_Unit_Window()
    if(_active_battle_unit == ST_UNDEFINED)
    {
        Set_Animation_Frame(magic_vortex_seg, 1);
        Draw_Picture_To_Bitmap(magic_vortex_seg, scratch_bitmap_seg);
        Set_Animation_Frame(magic_vortex_seg, CMB_VortexAnimStage);
        start_x = 82;
        start_y = 170;
        Get_Bitmap_Actual_Size(scratch_bitmap_seg, &bitm_x, &bitm_y, &bitm_w, &bitm_h);
        start_x += (((32 - bitm_w) / 2) - bitm_x);
        start_y += (((25 - bitm_h) / 2) - bitm_y);
        Draw_Picture_Windowed(start_x, start_y, scratch_bitmap_seg);
        colors[0] = 227;
        colors[1] = 243;
        Set_Font_Colors_15(0, &colors[0]);
        Set_Outline_Color(227);
        Set_Font_Style_Shadow_Down(0, 15, 0, 0);
        Set_Font_Spacing_Width(1);
        Print(115, 172, cnst_VortexMove_1);  // "Move"
        Print(115, 178, cnst_VortexMove_2);  // "Magic"
        Print(115, 184, cnst_VortexMove_3);  // "Vortex"
        Print(115, 190, cnst_VortexMove_4);  // "1 Space"
    }
    else
        // composes figure picture into scratch_bitmap_seg
        USELESS_Combat_Figure_Load_Compose(battle_units[_active_battle_unit].bufpi, 2, battle_units[_active_battle_unit].controller_idx, battle_units[_active_battle_unit].outline_magic_realm, 0);
        Combat_Figure_Effect__WIP(battle_units[_active_battle_unit].figure_effect);
        if((_unit_type_table[_UNITS[battle_units[_active_battle_unit].unit_idx].type].Abilities & UA_INVISIBILITY) != 0)
        {
            Outline_Bitmap_Pixels(scratch_bitmap_seg, 1);
        }
        // draws the composed figure picture, USELESS_Combat_Figure_Load_Compose() put in scratch_bitmap_seg
        Draw_Picture(start_x, start_y, scratch_bitmap_seg);
        Draw_Active_Unit_Stats_And_Icons();
        Draw_Active_Unit_Damage_Bar(_active_battle_unit, 118, 195);
 
Combat_Screen()
    |-> Allocate_Combat_Base_Blocks()
    |-> Combat_Screen_Load_Resources()
    |-> Cache_Graphics_Combat()
    |-> Build_Battlefield()
    |-> Prepare_All_Battle_Units()
        |-> Load_Battle_Unit()
        |-> Combat_Figure_Load()
            |-> USELESS_Combat_Figure_Load_Compose()                Combat-Combat_Figure_Compose.md

        |-> Combat_Screen_Map_Draw()
            |-> Combat_Figure_Compose_USEFULL()
            |-> Combat_Screen_Map_Draw_Entities()



## Unit Figure Composition
Combat.c    WZD ovr153p23   Combat_Figure_Compose_USEFULL()
vs.
Combat.c    WZD ovr153p15   USELESS_Combat_Figure_Load_Compose()
vs.
Combat.c    WZD o99p06      Combat_Grid_Entities()
vs.
Combat.c    WZD s103p07     Battle_Unit_Compose_Bitmap()
vs.
UnitView.C  WZD o89p05      Draw_Unit_Figure()

The four
USELESS_Combat_Figure_Load_Compose — faithful. It's the cut-down twin of Combat_Figure_Compose_USEFULL: same figure-page setup and bitmap compose, but stops after the banner colour and outline — no Combat_Figure_Effect__WIP, no Combat_Figure_Active_Red_Outline.
It also settles the ¿ three different macros for a (real) reason ? question in Combat.h:575-582. The reason is ordering: Combat_Figure_Load and Combat_Figure_Compose_USEFULL both do MAP-then-OFFSET, but this one does OFFSET-then-MAP (asm:18-33). Production preserves each function's own order. Don't normalise them.



## Unit Figure Position
!!!!! ALL DIFFERENT !!!!!
Combat.c    Combat_Grid_Entities()  (in-line)
vs.
UnitView.C  Unit_Figure_Position()
vs.
Combat.c    Battle_Unit_Figure_Position()

Unit_Figure_Position — faithful, all 36 pairs. This is the third figure-position table, and the "ALL DIFFERENT" comment is now confirmed for all three:
                                    case 4, fig 1   case 6, fig 1
BATTLE_UNIT_FIGURE_POSITION macro   −7, 8           4, 7
Battle_Unit_Figure_Position          8, 8           10, 8
Unit_Figure_Position                −9, 8           4, 7

---
