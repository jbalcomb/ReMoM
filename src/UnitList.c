/*
    Unit List Window

    WIZARDS.EXE
        ovr079
*/

#include "MOX/Allocate.h"
#include "MOX/FLIC_Draw.h"
#include "MOX/Fields.h"
#include "MOX/Fonts.h"
#include "MOX/Graphics.h"
#include "MOX/MOX_DAT.h"  /* _screen_seg */
#include "MOX/MOX_DEF.h"
#include "MOX/MOX_T4.h"
#include "MOX/MOX_TYPE.h"
#include "MOX/Video.h"
#include "MOX/Timer.h"

#include "Combat.h"
#include "Help.h"
#include "LOADER.h"
#include "UNITTYPE.h"
#include "UnitMove.h"
#include "UnitStat.h"
#include "MainScr.h"

#include <string.h>

#include "UnitList.h"



// dseg:BFF2                                                 ¿ BEGIN:  ovr056  Production Screen ?
// ...
// WZD dseg:C00E 00 00                                           m_ulw_window_field dw 0                         ; DATA XREF: IDK_ULW_Add_Fields+20w ...
// WZD dseg:C010 00 00                                           m_ulw_full_screen_ESC_field dw 0               ; DATA XREF: Unit_List_Window+272r ...

int16_t _ulw_window_field;
int16_t _ulw_full_screen_ESC_field;



// WZD dseg:49E6                                                 BEGIN:  ovr079 - Initialized Data  (Unit List Window)

// WZD dseg:49E6 58                                              
char str_hotkey_X__ovr079[] = "X";

// WZD dseg:49E7 00                                              
char str_emptystring__ovr079[] = "";

// WZD dseg:49E8
char str_RampagingMonsters[] = "Rampaging Monsters";

// WZD dseg:49FB
char str_Raiders[] = "Raiders";

// WZD dseg:4A03
char str_Units[] = " Units";

// WZD dseg:4A0A
char str_hotkey_ESC__ovr079[] = "\x1B";

// WZD dseg:4A0A                                                 END:  ovr079 - Initialized Data  (Unit List Window)



// WZD dseg:C2C0                                                 BEGIN:  ovr079 - Uninitialized Data  (Unit List Window)

// WZD dseg:C2C0
int16_t m_ulw_height;

// WZD dseg:C2C2
int16_t m_ulw_unit_owner_idx;

// WZD dseg:C2C4 00 00                                           dw 0

// WZD dseg:C2C6
int16_t m_ulw_troop_count;

// WZD dseg:C2C8
int16_t * ULW_stack_fields;

// WZD dseg:C2CA
int16_t * m_ulw_troops;

// WZD dseg:C2CC
int16_t m_ulw_unit_idx;

// WZD dseg:C2CE
int16_t ULW_IDK_dim2;

// WZD dseg:C2D0
int16_t ULW_IDK_dim1;

// WZD dseg:C2D2
int16_t m_ulw_start_y;

// WZD dseg:C2D4
int16_t m_ulw_start_x;

// WZD dseg:C2D6 00 00                                           dw 0

// WZD dseg:C2D6                                                 END:  ovr079 - Uninitialized Data  (Unit List Window)



/*
    WIZARDS.EXE  ovr079
*/

// WZD o79p01
/*
x,y only used for the expanding box/grow-out effect

Main_Screen()
    #### Section 9.2.2.2      Right-Click Movement Map - Stack - Other
    target_world_x = (_main_map_grid_x * SQUARE_WIDTH);
    target_world_y = (_main_map_grid_y * SQUARE_HEIGHT);
    |-> Unit_List_Window(unit_idx, 0, target_world_x, target_world_y)

IDK_Spell_Casting_Screen()
    |-> Unit_List_Window(_SI_, 1, LeftX@, TopY@)

*/
int16_t Unit_List_Window(int16_t unit_idx, int16_t flag, int16_t x, int16_t y)
{
    int16_t hotkey_X;
    SAMB_ptr temp_screen_seg;
    int16_t selected_unit_idx;
    int16_t screen_changed;
    int16_t scanned_field;
    int16_t input_field_idx;
    int16_t itr_units;  // _SI_
    int16_t itr_troops;  // _SI_
    int16_t leave_screen;  // _DI_

    Cache_Graphics_Unit_List_Window();

    Load_Palette_From_Animation(_unitlist_background_seg);
    Apply_Palette();

    Set_Page_Off();
    Copy_On_To_Off_Page();
    Copy_Off_To_Back();

    selected_unit_idx = ST_UNDEFINED;

    m_ulw_unit_idx = unit_idx;

    Set_Mouse_List(1, mouse_list_default);

    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);

    m_ulw_troops = (int16_t *)Near_Allocate_Next(18);

    ULW_stack_fields = (int16_t *)Near_Allocate_Next(18);

    m_ulw_unit_owner_idx = _UNITS[m_ulw_unit_idx].owner_idx;

    m_ulw_troop_count = 0;


    for(itr_units = 0; ((itr_units < _units) && (m_ulw_troop_count < MAX_STACK)); itr_units++)
    {
        if(
            (_UNITS[itr_units].owner_idx == _UNITS[m_ulw_unit_idx].owner_idx)
            &&
            (
                (_UNITS[itr_units].wp == _UNITS[m_ulw_unit_idx].wp)
                ||
                (_UNITS[itr_units].in_tower == ST_TRUE)
            )
            &&
            (_UNITS[itr_units].wx == _UNITS[m_ulw_unit_idx].wx)
            &&
            (_UNITS[itr_units].wy == _UNITS[m_ulw_unit_idx].wy)
            &&
            (
                (m_ulw_unit_owner_idx == _human_player_idx)
                ||
                (Unit_Has_Invisibility(itr_units) != ST_TRUE)
            )
        )
        {
            m_ulw_troops[m_ulw_troop_count] = itr_units;
            m_ulw_troop_count++;
        }
    }


    /*
        x1 is always 45
        y1 is 76 for 1 unit     ... 24 + (1 * 19) = 24 + 19 = 43 ... 97 - (43 / 2) = 97 - 21 = 76
              57 for 3 units
              47 for 4 units
    */
    m_ulw_start_x = 45;
    m_ulw_start_y = 0;
    ULW_IDK_dim1 = 8;
    ULW_IDK_dim2 = 24;

    m_ulw_height = (ULW_IDK_dim2 + (m_ulw_troop_count * 19));

    m_ulw_start_y = (97 - (m_ulw_height / 2));

    m_ulw_height = (m_ulw_start_y + m_ulw_height);  // ~ y2 = y1 + h


    temp_screen_seg = Allocate_First_Block(_screen_seg, 1);

    Set_Page_Off();

    Unit_List_Window_Draw();

    // TODO  HLP_NoResizeExpand__TODO(m_ulw_start_x, m_ulw_start_y, (m_ulw_start_x + 238), (m_ulw_height + 5), x, y, (x + 19), (y + 16), 1);

    Deactivate_Auto_Function();

    Assign_Auto_Function(Unit_List_Window_Draw, 1);

    Set_Input_Delay(1);

    Deactivate_Help_List();

    GUI_SetFullHelp();  // DEDU  what does this accomplish?

    leave_screen = ST_FALSE;

    screen_changed = ST_FALSE;  // DNE in Dasm

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        Clear_Fields();

        Unit_List_Window_Add_Fields(flag);

        hotkey_X = Add_Hot_Key(str_hotkey_X__ovr079[0]);

        input_field_idx = Get_Input();

        scanned_field = Scan_Input();

        if(input_field_idx == _ulw_full_screen_ESC_field)
        {
            leave_screen = ST_TRUE;
        }

        for(itr_troops = 0; itr_troops < m_ulw_troop_count; itr_troops++)
        {
            if(ULW_stack_fields[itr_troops] == input_field_idx)
            {
                selected_unit_idx = m_ulw_troops[itr_troops];
                leave_screen = ST_UNDEFINED;
            }
        }

        if((leave_screen == ST_FALSE) && (screen_changed == ST_FALSE) )
        {
            Copy_Back_To_Off();
            Unit_List_Window_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
        screen_changed = ST_FALSE;
    }


    _page_flip_effect = 3;

    Deactivate_Auto_Function();
    Reset_Window();
    Clear_Fields();
    Deactivate_Help_List();
    Cache_Graphics_Overland();

    return selected_unit_idx;
}


// WZD o79p02
/*


UNITVIEW.LBX, 028  UNITLIST    unitlist backgrnd
UNITVIEW.LBX, 029  UNITLIST    unitlist bottom
_unitlist_background_seg = LBX_Reload_Next(unitview_lbx_file, 28, GFX_Swap_Seg);
_unitlist_bottom_seg = LBX_Reload_Next(unitview_lbx_file, 29, GFX_Swap_Seg);
*/
void Unit_List_Window_Draw(void)
{
    int16_t temp_unit_type;
    int16_t stats[5];
    int16_t stat_y;
    int16_t stat_x;
    SAMB_ptr stat_icon_pict;
    uint8_t colors[6];
    int16_t unit_type;
    int16_t y_start;
    int16_t x_start;
    int16_t hero_unit_idx;
    int16_t attack_type_icon;  // in Dasm, hero_unit_idx
    int16_t itr;  // _SI_
    int16_t itr_stats;  // _DI_

    if(m_ulw_unit_owner_idx == NEUTRAL_PLAYER_IDX)
    {
        _players[m_ulw_unit_owner_idx].banner_id = NEUTRAL_PLAYER_BANNER_COLOR_IDX;
    }

    colors[0] = COL_Banners2[_players[m_ulw_unit_owner_idx].banner_id];
    colors[1] = COL_Banners2[_players[m_ulw_unit_owner_idx].banner_id];
    colors[2] = COL_Banners2[_players[m_ulw_unit_owner_idx].banner_id];
    colors[3] = COL_Banners2[_players[m_ulw_unit_owner_idx].banner_id];
    colors[4] = COL_Banners2[_players[m_ulw_unit_owner_idx].banner_id];
    colors[5] = COL_Banners2[_players[m_ulw_unit_owner_idx].banner_id];

    Set_Font_Colors_15(4, &colors[0]);

    Set_Font_Style_Shadow_Down(4, 15, 0, 0);

    Set_Outline_Color(0);

    for(itr = 0; itr < 50; itr++)
    {
        _help_entries[itr].help_idx = ST_UNDEFINED;
    }

    Set_Page_Off();

    // TODO  G_USW_DrawEHLines();

    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, m_ulw_height);

    Clipped_Draw(m_ulw_start_x, m_ulw_start_y, _unitlist_background_seg);

    Reset_Window();

    FLIC_Draw(m_ulw_start_x, m_ulw_height, _unitlist_bottom_seg);

    if(m_ulw_unit_owner_idx == NEUTRAL_PLAYER_IDX)
    {
        if((_unit_type_table[_UNITS[m_ulw_troops[0]].type].Abilities & UA_FANTASTIC) == 0)
        {
            strcpy(GUI_String_1, str_Raiders);  // "Raiders"
        }
        else
        {
            strcpy(GUI_String_1, str_RampagingMonsters);  // "Rampaging Monsters"
        }
    }
    else
    {
        strcpy(GUI_String_1, _players[m_ulw_unit_owner_idx].name);
        strcat(GUI_String_1, str_Units);  // " Units"
    }

    Set_Font_Style_Outline(4, 15, 0, 0);

    Set_Outline_Color(252);

    Print_Centered((m_ulw_start_x + 118), (m_ulw_start_y + 7), GUI_String_1);


    x_start = (m_ulw_start_x + ULW_IDK_dim1);

    y_start = (m_ulw_start_y + ULW_IDK_dim2);


    for(itr = 0; itr < m_ulw_troop_count; itr++)
    {
        unit_type = _UNITS[m_ulw_troops[itr]].type;

        if(_UNITS[m_ulw_troops[itr]].owner_idx == _human_player_idx)
        {
            Draw_Unit_Picture(x_start, y_start, m_ulw_troops[itr], 1);
        }
        else
        {
            Draw_Unit_Picture(x_start, y_start, m_ulw_troops[itr], 3);
        }

        hero_unit_idx = _UNITS[m_ulw_troops[itr]].Hero_Slot;

        if(hero_unit_idx == ST_UNDEFINED)
        {
            strcpy(GUI_String_1, *_unit_type_table[unit_type].name);
        }
        else
        {
            strcpy(GUI_String_1, _players[_UNITS[m_ulw_troops[itr]].owner_idx].Heroes[hero_unit_idx].name);
        }

        colors[0] = 197;
        colors[1] = 255;
        colors[2] = 80;
        colors[3] = 255;
        Set_Font_Colors_15(2, &colors[0]);
        Set_Font_Style_Outline(2, 15, 0, 0);
        Set_Outline_Color(251);

        Print((x_start + 24), (y_start + 5), GUI_String_1);


        temp_unit_type = _UNITS[m_ulw_troops[itr]].type;

        Load_Battle_Unit(m_ulw_troops[itr], global_battle_unit);

        _UNITS[m_ulw_troops[itr]].type = temp_unit_type;

        colors[0] = 250;
        colors[1] = 252;
        Set_Font_Colors_15(1, &colors[0]);
        Set_Font_Style(1, 15, 0, 0);
        Set_Outline_Color(252);


        stat_x = (m_ulw_start_x + 140);

        stat_y = (y_start + 7);

        stats[0] = global_battle_unit->melee;


        if(
            (global_battle_unit->ranged_type != ST_UNDEFINED)
            &&
            (
                ((global_battle_unit->ranged_type / 10) == 1)
                ||
                ((global_battle_unit->ranged_type / 10) == 2)
                ||
                ((global_battle_unit->ranged_type / 10) == 3)
            )
        )
        {
            stats[1] = global_battle_unit->ranged;
        }
        else
        {
            stats[1] = 0;
        }

        stats[2] = global_battle_unit->defense;

        stats[3] = global_battle_unit->hits;

        stats[4] = (global_battle_unit->movement_points / 2);


        for(itr_stats = 0; itr_stats < 5; itr_stats++)
        {

            if(stats[itr_stats]  > 0)
            {
                Print_Integer_Right(stat_x, stat_y, stats[itr_stats]);

// TODO  _help_entries
// mov     ax, _SI_itr
// mov     dx, 5
// imul    dx
// add     ax, _DI_itr_stats
// mov     dx, 10
// imul    dx
// mov     dx, [bp+y]
// add     dx, -9
// mov     bx, ax
// mov     [_help_entries.help_00.x1+bx], dx
// mov     ax, _SI_itr
// mov     dx, 5
// imul    dx
// add     ax, _DI_itr_stats
// mov     dx, 10
// imul    dx
// mov     dx, [bp+x]
// add     dx, 0FFFEh
// mov     bx, ax
// mov     [_help_entries.help_00.y1+bx], dx
// mov     ax, _SI_itr
// mov     dx, 5
// imul    dx
// add     ax, _DI_itr_stats
// mov     dx, 10
// imul    dx
// mov     dx, [bp+y]
// add     dx, 9
// mov     bx, ax
// mov     [_help_entries.help_00.x2+bx], dx
// mov     ax, _SI_itr
// mov     dx, 5
// imul    dx
// add     ax, _DI_itr_stats
// mov     dx, 10
// imul    dx
// mov     dx, [bp+x]
// add     dx, 5
// mov     bx, ax
// mov     [_help_entries.help_00.y2+bx], dx

                switch(itr_stats)
                {
                    case 0:
                    {
                        // TODO  mov     [word ptr _help_entries.help_00.entry_idx+bx], HLP_MELEE
                        Melee_Type_Icon(global_battle_unit->Weapon_Plus1, &attack_type_icon);
                        stat_icon_pict = unitview_stat_icons_seg[attack_type_icon];
                    } break;
                    case 1:
                    {
                        // TODO  mov     [word ptr _help_entries.help_00.entry_idx+bx], HLP_RANGE
                        Ranged_Type_Icon(global_battle_unit->ranged_type, &attack_type_icon);
                        stat_icon_pict = unitview_stat_icons_seg[attack_type_icon];
                    } break;
                    case 2:
                    {
                        // TODO  mov     [word ptr _help_entries.help_00.entry_idx+bx], HLP_ARMOR
                        stat_icon_pict = unitview_stat_icons_seg[9];
                    } break;
                    case 3:
                    {
                        // TODO  mov     [word ptr _help_entries.help_00.entry_idx+bx], HLP_HITS
                        stat_icon_pict = unitview_stat_icons_seg[10];
                    } break;
                    case 4:
                    {
                        // TODO  mov     [word ptr _help_entries.help_00.entry_idx+bx], HLP_UNIT_MOVES
                        Moves_Type_Icon(global_battle_unit->ranged_type, &attack_type_icon);
                        stat_icon_pict = unitview_stat_icons_seg[attack_type_icon];
                    } break;
                }

                FLIC_Draw((stat_x + 1), (stat_y - 2), stat_icon_pict);

                Line((stat_x + 1), (stat_y - 1), (stat_x + 1), (stat_y + 5), 197);

                Line((stat_x + 1), (stat_y + 5), (stat_x + 9), (stat_y + 5), 246);


            }

            stat_x += 20;
        }

        y_start += 19;
    }


// DEDU What's with the sizeof(50)?
// TODO  _help_entries
// mov     ax, [m_ulw_troop_count]
// mov     dx, 50
// imul    dx
// mov     dx, [m_ulw_start_x]
// mov     bx, ax
// mov     [_help_entries.help_00.x1+bx], dx
// mov     ax, [m_ulw_troop_count]
// mov     dx, 50
// imul    dx
// mov     dx, [m_ulw_start_y]
// mov     bx, ax
// mov     [_help_entries.help_00.y1+bx], dx
// mov     ax, [m_ulw_troop_count]
// mov     dx, 50
// imul    dx
// mov     dx, [m_ulw_start_x]
// add     dx, 237
// mov     bx, ax
// mov     [_help_entries.help_00.x2+bx], dx
// mov     ax, [m_ulw_troop_count]
// mov     dx, 50
// imul    dx
// mov     dx, [m_ulw_height]
// add     dx, 4
// mov     bx, ax
// mov     [_help_entries.help_00.y2+bx], dx
// mov     ax, [m_ulw_troop_count]
// mov     dx, 50
// imul    dx
// mov     bx, ax
// mov     [word ptr _help_entries.help_00.entry_idx+bx], HLP_UNIT_LIST


    Cycle_Unit_Enchantment_Animation();


}


// WZD o79p03
void Unit_List_Window_Add_Fields(int16_t flag)
{
    int16_t x1;
    int16_t ULW_fields_flag;
    int16_t itr_max_stack;  // _SI_
    int16_t itr_troops;  // _SI_
    int16_t y1;  // _DI_

    for(itr_max_stack = 0; itr_max_stack < MAX_STACK; itr_max_stack++)
    {
        ULW_stack_fields[itr_max_stack] = INVALID_FIELD;
    }

    _ulw_window_field = INVALID_FIELD;

    if(flag == ST_TRUE)
    {
        x1 = (m_ulw_start_x + ULW_IDK_dim1);
        y1 = (m_ulw_start_y + ULW_IDK_dim2);

        for(itr_troops = 0; itr_troops < m_ulw_troop_count; itr_troops++)
        {
            ULW_stack_fields[itr_troops] = Add_Hidden_Field(x1, y1, (x1 + 222), (y1 + 18), str_emptystring__ovr079[0], ST_UNDEFINED);
            y1 += 19;
        }

        _ulw_window_field = Add_Hidden_Field(m_ulw_start_x, m_ulw_start_y, (m_ulw_start_x + 237), (m_ulw_height + 4), str_emptystring__ovr079[0], ST_UNDEFINED);
    }

    _ulw_full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr079[0], ST_UNDEFINED);

}