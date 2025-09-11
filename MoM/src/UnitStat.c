/*
    Unit Statistics Window
    
    WIZARDS.EXE
        ovr072
*/

#include "../../STU/src/STU_CHK.h"
#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Video.h"
#include "../../MoX/src/paragrph.h"

#include "CITYCALC.h"
#include "Combat.h"
#include "LOADER.h"
#include "Terrain.h"
#include "UNITSTK.h"
#include "UNITTYPE.h"
#include "CityScr.h"
#include "Lair.h"
#include "MainScr.h"
#include "MainScr_Maps.h"
#include "UnitView.h"
#include "City_ovr55.h"
#include "ItemView.h"
#include "RACETYPE.h"
#include "WZD_o059.h"

#include <stdlib.h>
#include <string.h>

#include <SDL_stdinc.h>

#include "UnitStat.h"



/*
    Product Screen
    used for Thing_View(); CalledFromFlag == 0, thing_idx < 100, view_type == 0
*/
extern char * building_description;
void Building_Allows_List__WIP(int16_t bldg_idx, int16_t * allows_list_count, int16_t allows_list[]);



// WZD dseg:33B8 01 00 20 00 40 00 04 00 02 00 08 00             MoveFlag_Array dw M_Cavalry, M_Forester, M_Mntnr, M_Swimming, M_Sailing, M_Flying
// WZD dseg:33B8                                                                                         ; DATA XREF: Stack_Movement_Modes+10o

// WZD dseg:33C4
char portrait_lbx_file[] = "PORTRAIT";

// WZD dseg:33CC
char cnst_ZeroString_10[] = "0";

// WZD dseg:33CD
char cnst_Ok_BtnMsg_2[] = "Ok";

// WZD dseg:33D0
char cnst_HOTKEY_O_6[] = "O";

// WZD dseg:33D2
char cnst_Dismiss_BtnMsg[] = "Dismiss";

// WZD dseg:33DA
char cnst_HOTKEY_D_4[] = "D";

// WZD dseg:33DC
char cnst_HOTKEY_Esc6[] = "\1B";

// WZD dseg:33DE
char cnst_HOTKEY_X_4[] = "X";

// WZD dseg:33E0
char cnst_Disband_Msg1[] = "Do you wish to disband the unit of \x02";
// WZD dseg:3405
char cnst_Disband_Msg2[] = "\x01?";
// WZD dseg:3408
char cnst_Dismiss_Msg[] = "Do you wish to dismiss \x02";

// WZD dseg:3421 43 6F 73 74 00                                  cnst_Cost db 'Cost',0                   ; DATA XREF: Unit_Statistics_Popup_Draw+190o
// WZD dseg:3426 28 00                                           str_PAREN_OPEN db '(',0              ; DATA XREF: Unit_Statistics_Popup_Draw+1C6o
// WZD dseg:3428 29 00                                           str_PAREN_CLOSE db ')',0              ; DATA XREF: Unit_Statistics_Popup_Draw+202o
// WZD dseg:342A 54 68 65 00                                     cnst_The db 'The',0                     ; DATA XREF: Unit_Statistics_Popup_Draw+32Co
// WZD dseg:342E 4D 6F 76 65 73 00                               cnst_Moves_2 db 'Moves',0               ; DATA XREF: Unit_Statistics_Popup_Draw:loc_5F1CCo
// WZD dseg:342E                                                                                         ; could use dseg:6204
// WZD dseg:3434 48 65 61 64 73 00                               cnst_Heads db 'Heads',0                 ; DATA XREF: Unit_Statistics_Popup_Draw+5FEo
// WZD dseg:343A 44 61 6D 61 67 65 00                            cnst_Damage db 'Damage',0               ; DATA XREF: Unit_Statistics_Popup_Draw:loc_5F2BAo
// WZD dseg:343A                                                                                         ; could use dseg:4259
// WZD dseg:3441 55 70 6B 65 65 70 00                            cnst_Upkeep_2 db 'Upkeep',0             ; DATA XREF: Unit_Statistics_Popup_Draw:loc_5F304o
// WZD dseg:3448 4D 65 6C 65 65 00                               cnst_Melee db 'Melee',0                 ; DATA XREF: Unit_Statistics_Popup_Draw+72Bo
// WZD dseg:344E 52 61 6E 67 65 00                               cnst_Range db 'Range',0                 ; DATA XREF: Unit_Statistics_Popup_Draw+7A7o
// WZD dseg:344E                                                                                         ; could use dseg:40c0
// WZD dseg:3454 41 72 6D 6F 72 00                               cnst_Armor db 'Armor',0                 ; DATA XREF: Unit_Statistics_Popup_Draw:loc_5F4C9o
// WZD dseg:3454                                                                                         ; could use dseg:4363 (or dseg:4425)
// WZD dseg:345A 52 65 73 69 73 74 00                            cnst_Resist db 'Resist',0               ; DATA XREF: Unit_Statistics_Popup_Draw+8BBo
// WZD dseg:3461 48 69 74 73 00                                  cnst_Hits db 'Hits',0                   ; DATA XREF: Unit_Statistics_Popup_Draw+916o
// WZD dseg:3466 4D 61 69 6E 74 65 6E 61 6E 63 65 00             cnst_Maintenance db 'Maintenance',0     ; DATA XREF: Unit_Statistics_Popup_Draw:loc_5F764o
// WZD dseg:3466                                                                                         ; could use dseg:2d3c
// WZD dseg:3472 41 6C 6C 6F 77 73 00                            cnst_Allows db 'Allows',0               ; DATA XREF: Unit_Statistics_Popup_Draw+C3Bo
// WZD dseg:3479 2E 00                                           cnst_Dot4 db '.',0                      ; DATA XREF: Unit_Statistics_Popup_Draw+D4Fo
// WZD dseg:3479                                                                                         ; should use dseg:2b31
// WZD dseg:347B 55 00                                           cnst_HOTKEY_U_4 db 'U',0                ; DATA XREF: USW_CreateScrollBtns+22o
// WZD dseg:347B                                                                                         ; should use dseg:2e28
// WZD dseg:347D 00                                              db    0

// WZD dseg:347E                                                 ?
// WZD dseg:347E                                                     BEGIN: DIPLOMAC || Magic Screen - Initialized Data
// WZD dseg:347E                                                 ?
// WZD dseg:347E
// WZD dseg:347E 01 00                                           GAME_AlchemyDir dw 1                    ; DATA XREF: GAME_AlchemyWindow+19Dr ...



// WZD dseg:C176                                                 ¿ BEGIN:  ovr072  Unit View ?

// dseg:C176
/*
{F,T} 'Unit View' was called from Combat
*/
int16_t uv_combat;

// dseg:C178
/*
{F,T} Viewed Unit is Hero Unit
*/
int16_t uv_hero;

// dseg:C17A
/*
{F,T} ¿ fill background for Unit Figure / Hero Portrait ?
*/
int16_t uv_fill_base;

// dseg:C17C
// XREFs: Load_Unit_Figure()  Draw_Unit_Figure()
SAMB_ptr unit_figure_seg;  // loaded into screen_seg

// dseg:C17E
int16_t unitview_dn_button;

// dseg:C180
int16_t unitview_up_button;

// dseg:C182
int16_t unitview_dismiss_button;

// dseg:C184
int16_t unitview_ok_button;

// dseg:C186
int16_t unitview_full_screen;

// dseg:C188
int16_t unitview_button_box;

// dseg:C18A
int16_t unitview_window;

// dseg:C18C
int16_t uv_specials_list_index;

// dseg:C18E
/*
{F,T} more than one page of specials - add up/dn buttons, etc.  (>8 or hero & >4)
*/
int16_t uv_multipage;

// dseg:C190
int16_t uv_specials_list_count;

// dseg:C192
struct s_UV_List * uv_specials_list_array;

// dseg:C194
// drake178: USW_ShownFX_Flags
int32_t uv_specials_fields_ability_flags[8];

// dseg:C1B4
// drake178: USW_ItemSlot_ClkLbls
int16_t uv_item_desc_fields[3];

// dseg:C1BA
// drake178: USW_ItemSlot_IMGLbls
int16_t uv_item_icon_fields[3];

// dseg:C1C0
// drake178: USW_FX_ClickLabels
int16_t uv_special_desc_fields[8];

// dseg:C1D0
// drake178: USW_FX_ImageLabels
int16_t uv_special_icon_fields[8];

// dseg:C1E0
// ~== m_item_icon_workarea@ for Item Screen
SAMB_ptr UV_item_icon_pict_seg;

// dseg:C1E2
int16_t uv_unit_idx;

// dseg:C1E4
int16_t uv_view_type;  // enum e_VIEW_TYPE

// dseg:C1E6
int16_t UV_y_start;

// dseg:C1E8
int16_t UV_x_start;

// WZD dseg:C1E8                                                 ¿ END:  ovr072  Unit View ?



/*
    WIZARDS.EXE  ovr072
*/

// WZD o72p01
// drake178: USW_CombatDisplay()
/*
; a wrapper for USW_Display that performs the necessary
; memory management to show the unit statistics window
; in combat before doing it, and swaps back the combat
; graphics afterward
; inherits multiple BUGs
*/
/*

no reason the view type isn't hard-coded here?
 ...the graphics cache is hard-coded...
 ...the combat flag is hard-coded...
*/
void Combat_Unit_Statistics_Window(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t view_type, int16_t unit_idx)
{

    Cache_Graphics_Things();

    Mark_Block(_screen_seg);

    Unit_View_Allocate();

    if(_UNITS[unit_idx].type > ut_Chosen)
    {
        Load_Unit_Figure(unit_idx, 0);
    }
    else
    {
        m_hero_portrait_seg = LBX_Reload_Next(portrait_lbx_file, _unit_type_table[_UNITS[unit_idx].type].hero_portrait, _screen_seg);
    }

    uv_combat = ST_TRUE;

    Unit_Statistics_Popup(x_start, y_start, x1, y1, x2, y2, view_type, unit_idx);

    Release_Block(_screen_seg);

    Cache_Graphics_Combat();

}


// WZD o72p02
// drake178: Overland_Unit_Statistics_Window()
/*
; a wrapper for USW_Display that swaps in the overland
; graphics, loads the figure or portrait image of the
; unit, and allocates the USW structure memory,
; swapping back the city graphics afterwards
; inherits all the BUGs
*/
/*
OON XREF: Main_Unit_Statistics_Window()

Main_Screen()
    BEGIN: Right-Click Unit Window Grid Field
        Main_Unit_Statistics_Window(_unit_stack[Stack_Index].unit_idx, target_world_x, target_world_y, (target_world_x + 18), (target_world_y + 18));
            Overland_Unit_Statistics_Window(31, 6, x1, y1, x2, y2, uvt_Stat, unit_idx);

combat calls directly, without a local/warpper function
everything, except production secreen, uses the main/local/wrapper function
what does production secreen call?
...must call with uvt_Prod
*/
void Overland_Unit_Statistics_Window(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t view_type, int16_t unit_idx)
{
    SAMB_ptr temp_seg;

    Cache_Graphics_Things();

    temp_seg = Allocate_First_Block(_screen_seg, 1);

    Mark_Block(_screen_seg);

    Unit_View_Allocate();

    uv_combat = ST_FALSE;
    
    if(_UNITS[unit_idx].type > ut_Chosen)
    {
        Load_Unit_Figure(unit_idx, 0);
    }
    else
    {
        m_hero_portrait_seg = LBX_Reload_Next(portrait_lbx_file, _unit_type_table[_UNITS[unit_idx].type].hero_portrait, _screen_seg);
    }

    Unit_Statistics_Popup(x_start, y_start, x1, y1, x2, y2, view_type, unit_idx);

    Release_Block(_screen_seg);

    Cache_Graphics_Overland();

}

// WZD o72p03
/*
; displays the unit statistics window for the specified
; unit at the selected coordinates, "growing out" of
; the passed rectangle if expanding help is enabled and
; the function is called out of combat
; inherits multiple BUGs
*/
void Unit_Statistics_Popup(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t view_type, int16_t unit_idx)
{
    int16_t hotkey_idx_X;
    int16_t hotkey_idx_ESC;
    int16_t item_idx;
    int16_t screen_changed;
    int16_t Shown_Effect_Count;
    int16_t enable_disenchant;  /* {F,T} allow left-click specials - remove enchantment */
    int16_t scanned_field;
    int16_t input_field_idx;
    int16_t dismissed_unit;  // {F,T} unit was dismissed as a consequence of the enchantment removal
    int16_t leave_screen;
    int16_t itr;  // _SI_


    UV_x_start = x_start;
    UV_y_start = y_start;

    uv_view_type = view_type;

    uv_unit_idx = unit_idx;


    Clear_Fields();

    Deactivate_Auto_Function();
    Assign_Auto_Function(Unit_Statistics_Popup_Do_Draw, 1);

    Deactivate_Help_List();

    Set_Unit_View_Help(view_type);

    uv_specials_list_count = 0;

    if((view_type == uvt_Stat) || (view_type == uvt_Cmbt))
    {
        USW_Build_Effect_List(uv_unit_idx, &uv_specials_list_array[0], &uv_specials_list_count);
    }


    uv_specials_list_index = 0;
    uv_multipage = ST_FALSE;
    uv_hero = ST_FALSE;


    if(_UNITS[uv_unit_idx].Hero_Slot != -1)
    {
        uv_hero = ST_TRUE;
    }
    
    if( (uv_hero == ST_TRUE) && (uv_specials_list_count > 4) )
    {
        uv_multipage = ST_TRUE;
    }

    if(uv_specials_list_count > 8)
    {
        uv_multipage = ST_TRUE;
    }

    enable_disenchant = ST_TRUE;

    if(view_type == uvt_Cmbt)
    {
        enable_disenchant = ST_FALSE;
    }

    Load_Palette_From_Animation(unitview_large_background_seg);
    Apply_Palette();
    Set_Palette_Changes(244, 255);
    Calculate_Remap_Colors();
    Set_Page_Off();
    Copy_On_To_Off_Page();
    Copy_Off_To_Back();
    screen_changed = ST_FALSE;  // DNE in Dasm
    Set_Input_Delay(4);

    
    uv_fill_base = ST_TRUE;
    Set_Page_Off();
    Unit_Statistics_Popup_Do_Draw();

    if(uv_combat == ST_FALSE)
    {
        // TODO  HLP_DrawExpanding__TODO(UV_x_start, UV_y_start, (UV_x_start + 217), (UV_y_start + 184), UV_x_start, UV_y_start, (UV_x_start + 282), (UV_y_start + 184),x1, y1, x2, y2, 1);
    }

    uv_fill_base = 0;

    UV_Set_Specials_Help_Fields(uv_specials_list_array, uv_specials_list_count, 0);

    leave_screen = ST_FALSE;
    while(leave_screen == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();

        UV_Add_Specials_Fields((UV_x_start + 8), (UV_y_start + 108), uv_specials_list_array, uv_specials_list_count, uv_specials_list_index);

        Set_Font_Style(4, 4, 0, 0);
        Set_Alias_Color(203);

        unitview_ok_button = INVALID_FIELD;
        if(view_type != uvt_Cmbt)
        {
            // unitview_ok_button = Add_Button_Field((UV_x_start + 222), (UV_y_start + 162), cnst_Ok_BtnMsg_2, red_button_seg, cnst_HOTKEY_O_6, ST_UNDEFINED);
            unitview_ok_button = Add_Button_Field((UV_x_start + 222), (UV_y_start + 162), cnst_Ok_BtnMsg_2, red_button_seg, 'O', -1);
        }

        unitview_dismiss_button = INVALID_FIELD;
        if(view_type == uvt_Stat)
        {
            // unitview_dismiss_button = Add_Button_Field((UV_x_start + 222), (UV_y_start + 143), cnst_Dismiss_BtnMsg, red_button_seg, cnst_HOTKEY_D_4, ST_UNDEFINED);
            unitview_dismiss_button = Add_Button_Field((UV_x_start + 222), (UV_y_start + 143), cnst_Dismiss_BtnMsg, red_button_seg, 'D', -1);
        }

        // unitview_window = Add_Hidden_Field(UV_x_start, UV_y_start, (UV_x_start + 217), (UV_y_start + 184), cnst_ZeroString_10, -1);
        unitview_window = Add_Hidden_Field(UV_x_start, UV_y_start, (UV_x_start + 217), (UV_y_start + 184), 0, -1);

        unitview_button_box = INVALID_FIELD;
        if(view_type != uvt_Cmbt)
        {
            // unitview_button_box = Add_Hidden_Field((UV_x_start + 213), (UV_y_start + 133), (UV_x_start + 280), (UV_y_start + 183), cnst_ZeroString_10, ST_UNDEFINED);
            unitview_button_box = Add_Hidden_Field((UV_x_start + 213), (UV_y_start + 133), (UV_x_start + 280), (UV_y_start + 183), 0, -1);
        }

        // unitview_full_screen = Add_Hidden_Field(0, 0, 319, 199, cnst_ZeroString_10, ST_UNDEFINED);
        unitview_full_screen = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 0, ST_UNDEFINED);

        // hotkey_idx_ESC = Add_Hot_Key(cnst_HOTKEY_Esc6);
        hotkey_idx_ESC = Add_Hot_Key('\x1B');
        // hotkey_idx_X = Add_Hot_Key(cnst_HOTKEY_X_4);
        hotkey_idx_X = Add_Hot_Key('X');

        UV_Set_Specials_Help_Fields(uv_specials_list_array, uv_specials_list_count, 0);


        input_field_idx = Get_Input();

        scanned_field = Scan_Input();


        // BUG:  bjorked hotkey 'X' ... ¿ macro of if() with empty body ?
        // mov     ax, [bp+input_field_idx];  cmp     ax, [bp+hotkey_idx_X];  jnz     short $+2;
        if(input_field_idx == hotkey_idx_X)
        {

        }

        UV_Handle_Arrow_Buttons(input_field_idx, &Shown_Effect_Count);

        /*
            BEGIN:  Right-Click Items Fields - Icon or Description
        */
        {
            for(itr = 0; itr < NUM_HERO_ITEMS; itr++)
            {
                if(
                    (-(uv_item_icon_fields[itr]) == input_field_idx)
                    ||
                    (-(uv_item_desc_fields[itr]) == input_field_idx)
                )
                {
                    Play_Left_Click();
                    
                    item_idx = _players[_UNITS[uv_unit_idx].owner_idx].Heroes[_UNITS[uv_unit_idx].Hero_Slot].Items[itr];
                    if(item_idx != -1)
                    {
                        Deactivate_Help_List();

                        Item_View_Popup(item_idx, (UV_x_start + 16), (UV_y_start + 166 + (17 * itr)));
                        
                        Deactivate_Auto_Function();
                        Assign_Auto_Function(Unit_Statistics_Popup_Do_Draw, 1);
                        Set_Input_Delay(2);
                        screen_changed = ST_TRUE;
                        Release_Block(_screen_seg);
                        Mark_Block(_screen_seg);
                        Unit_View_Allocate();
                        if(_UNITS[uv_unit_idx].type > ut_Chosen)
                        {
                            Load_Unit_Figure(uv_unit_idx, 0);
                        }
                        else
                        {
                            m_hero_portrait_seg = LBX_Reload_Next(portrait_lbx_file, _unit_type_table[_UNITS[uv_unit_idx].type].hero_portrait, _screen_seg);
                        }
                        USW_Build_Effect_List(uv_unit_idx, uv_specials_list_array, &uv_specials_list_count);
                        if(uv_specials_list_index >= uv_specials_list_count)
                        {
                            uv_specials_list_index -= 8;
                        }
                        uv_multipage = ST_FALSE;
                        if(
                            (uv_hero == ST_TRUE)
                            &&
                            (uv_specials_list_count > 4)
                        )
                        {
                            uv_multipage = ST_TRUE;
                        }
                        if(uv_specials_list_count > 8)
                        {
                            uv_multipage = ST_TRUE;
                        }
                        Set_Unit_View_Help(view_type);
                        UV_Set_Specials_Help_Fields(uv_specials_list_array, uv_specials_list_count, 0);
                    }
                }
            }
        }
        /*
            END:    Right-Click Items Fields - Icon or Description
        */

        /*
            BEGIN:  Left-Click Specials Fields - Icon or Description - Remove Unit Enchantment
        */
        {
            for(itr = 0; itr < Shown_Effect_Count; itr++)
            {
                if(enable_disenchant == ST_TRUE)
                {
                    if(
                        (uv_special_icon_fields[itr] == input_field_idx)
                        ||
                        (uv_special_desc_fields[itr] == input_field_idx)
                    )
                    {
                        if(
                            (uv_specials_fields_ability_flags[itr] != -1)
                            &&
                            (uv_specials_fields_ability_flags[itr] != -99)
                        )
                        {
                            Play_Left_Click();
                            Deactivate_Help_List();

                            dismissed_unit = UV_Remove_Unit_Enchantment(uv_unit_idx, uv_specials_fields_ability_flags[itr], GUI_String_1);

                            if(dismissed_unit == ST_TRUE)
                            {
                                leave_screen = ST_UNDEFINED;
                                WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
                            }
                            else
                            {
                                Deactivate_Auto_Function();
                                Assign_Auto_Function(Unit_Statistics_Popup_Do_Draw, 1);
                                screen_changed = ST_TRUE;
                                Release_Block(_screen_seg);
                                Mark_Block(_screen_seg);
                                Unit_View_Allocate();
                                if(_UNITS[uv_unit_idx].type > ut_Chosen)
                                {
                                    Load_Unit_Figure(uv_unit_idx, 0);
                                }
                                else
                                {
                                    m_hero_portrait_seg = LBX_Reload_Next(portrait_lbx_file, _unit_type_table[_UNITS[uv_unit_idx].type].hero_portrait, _screen_seg);
                                }
                                Load_Battle_Unit(uv_unit_idx, global_battle_unit);
                                USW_Build_Effect_List(uv_unit_idx, uv_specials_list_array, &uv_specials_list_count);
                                if(uv_specials_list_index >= uv_specials_list_count)
                                {
                                    uv_specials_list_index -= 8;
                                }
                                uv_multipage = ST_FALSE;
                                if(
                                    (uv_hero == ST_TRUE)
                                    &&
                                    (uv_specials_list_count > 4)
                                )
                                {
                                    uv_multipage = ST_TRUE;
                                }
                                if(uv_specials_list_count > 8)
                                {
                                    uv_multipage = ST_TRUE;
                                }
                                Set_Unit_View_Help(view_type);
                                UV_Set_Specials_Help_Fields(uv_specials_list_array, uv_specials_list_count, 0);
                            }
                        }
                    }
                }
            }
        }
        /*
            END:    Left-Click Unit Ability/Effect Field - Icon or Description - Remove Unit Enchantment
        */

        /*
            BEGIN:  Left-Click Dismiss Button
        */
        {
            if(input_field_idx == unitview_dismiss_button)
            {
                Play_Left_Click();
                if(_UNITS[uv_unit_idx].Hero_Slot != -1)
                {
                    strcpy(GUI_String_1, cnst_Dismiss_Msg);  // "Do you wish to dismiss "
                    strcat(GUI_String_1, _players[_human_player_idx].Heroes[_UNITS[uv_unit_idx].Hero_Slot].name);
                }
                else
                {
                    strcpy(GUI_String_1, cnst_Disband_Msg1);  // "Do you wish to disband the unit of \x02"
                    strcat(GUI_String_1, *_unit_type_table[_UNITS[uv_unit_idx].type].name);
                }
                strcat(GUI_String_1, cnst_Disband_Msg2);
                Deactivate_Help_List();
                if(Confirmation_Box(GUI_String_1) == ST_TRUE)
                {
                    leave_screen = ST_TRUE;
                    Dismiss_Unit(uv_unit_idx);
                    // DONT  j_o62p01_Empty_pFxn(_human_player_idx);  // ; does nothing and returns zero; at some point must have been some wizard data refresh function
                    Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, _UNITS[uv_unit_idx].wx, _UNITS[uv_unit_idx].wy);
                    Set_Unit_Draw_Priority();
                    Reset_Stack_Draw_Priority();
                }
            }
        }
        /*
            END:    Left-Click Dismiss Button
        */

        /*
            BEGIN: Left-Click Off-Window, Left-Click OK Button, hotkey ESCAPE
        */
        {
            if(
                (input_field_idx == unitview_full_screen)
                ||
                (input_field_idx == unitview_ok_button)
                ||
                (input_field_idx == hotkey_idx_ESC)
            )
            {
                Play_Left_Click();
                leave_screen = ST_UNDEFINED;
            }
        }
        /*
            END: Left-Click Off-Window, Left-Click OK Button, hotkey ESCAPE
        */


        if(
            (leave_screen == ST_FALSE)
            &&
            (screen_changed == ST_FALSE)
        )
        {
            Copy_Back_To_Off();
            Unit_Statistics_Popup_Do_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
        screen_changed = ST_FALSE;
    }


// @@Done:
    _page_flip_effect = 3;
    Reset_Window();
    Clear_Fields();
    Deactivate_Auto_Function();
    Deactivate_Help_List();

}

// WZD o72p04
/*
    calls Thing_View_Draw() for the *body* of the 'Unit View'
    handles expanding box, Ok and Dismiss buttons, and Up/Down arrow buttons
*/
void Unit_Statistics_Popup_Do_Draw(void)
{

    if(uv_combat != ST_FALSE)
    {
        // TODO  G_USW_DrawEHLines__TODO();
    }

    Thing_View_Draw__WIP(UV_x_start, UV_y_start, uv_view_type, uv_unit_idx, uv_specials_list_array, uv_specials_list_count, UV_item_icon_pict_seg);

    if(uv_view_type != uvt_Cmbt)
    {
        Set_Animation_Frame(red_button_seg, 0);
        FLIC_Draw((UV_x_start + 222), (UV_y_start + 162), red_button_seg);  // Ok
        Set_Animation_Frame(red_button_seg, 0);
        FLIC_Draw((UV_x_start + 222), (UV_y_start + 143), red_button_seg);  // Dismiss
    }

    if(uv_multipage == ST_TRUE)
    {
        Set_Animation_Frame(unitview_up_arrow_seg, 0);
        FLIC_Draw((UV_x_start + 204), (UV_y_start + 105), unitview_up_arrow_seg);
        Set_Animation_Frame(unitview_down_arrow_seg, 0);
        FLIC_Draw((UV_x_start + 204), (UV_y_start + 168), unitview_down_arrow_seg);
    }

}

// WZD o72p05
/*
    ~ Draw 'View' of *Thing*

Production_Screen_Draw()
Unit_Statistics_Popup_Draw__WIP((production_screen_xstart + build_table_item_xadd), production_screen_ystart, 0, product_idx, prod_specials_list, uv_specials_list_count, PS_product_pict_seg);
product_idx is building or unit
'View Type' is 0 - ¿ ?

; draws the unit statistics window, recruitment, or production detail window into the current draw frame
; BUG: draws the wrong movement type icons in certain situations (only uses ground, water, and air too)
; BUG: the "Allows" list for buildings can contain incorrect information
; BUG: fails to animate item slot 0
; WARNING: not enough icons for top tier units

TODO  rename View_Type; rename CalledFromFlag to view_type
*/
void Thing_View_Draw__WIP(int16_t x_start, int16_t y_start, int16_t CalledFromFlag, int16_t thing_idx, struct s_UV_List * specials_list, int16_t specials_count, SAMB_ptr item_icon_pict_seg)
{
    char temp_string[20];
    int16_t Allows_List[9];
    SAMB_ptr bldg_pict_seg;
    int16_t Object_IMG_Height;
    int16_t Object_IMG_Width;
    int16_t Object_IMG_Top;
    int16_t Object_IMG_Left;  // In Dasm, reused as tmp unit_type_idx
    int16_t unit_type_idx;
    int16_t stat_icon_row_limit;
    uint8_t colors[6];
    int16_t bldg_pict_draw_y;
    int16_t bldg_pict_draw_x;
    int16_t Gold_Icons;
    int16_t unit_race;
    int16_t View_Type;
    int16_t Allows_Count;
    int16_t bldg_idx;
    int16_t unit_idx;
    int16_t hero_owner_idx;
    int16_t product_cost;
    int16_t Text_Top;
    int16_t Text_Left;
    int16_t product_idx;
    int16_t attack_icon_index;
    int16_t Allows_List_Size;
    int16_t itr;
    int16_t UV_x_start_offset;  // _SI_
    int16_t bldg_pict_h;   // DNE in Dasm
    int16_t bldg_pict_w;   // DNE in Dasm
    int16_t bldg_pict_y1;  // DNE in Dasm
    int16_t bldg_pict_x1;  // DNE in Dasm
    int16_t moves_icon_idx;  // DNE in Dasm, uses product_idx

    UV_x_start = x_start;
    UV_y_start = y_start;

    unit_idx = thing_idx;
    bldg_idx = thing_idx;

    Set_Page_Off();

    if(uv_fill_base == ST_TRUE)
    {
        Fill(UV_x_start, UV_y_start, (UV_x_start + 281), (UV_y_start + 183), ST_TRANSPARENT);
    }

    /*
        Prod:  small background
        Stat:  large background && buttons
        Cmbt:  large background
    */
    if(CalledFromFlag == uvt_Prod)
    {
        FLIC_Draw(UV_x_start, UV_y_start, unitview_small_background_seg);
    }
    else
    {
        FLIC_Draw(UV_x_start, UV_y_start, unitview_large_background_seg);
        if(CalledFromFlag != uvt_Cmbt)
        {
            FLIC_Draw((UV_x_start + 213), (UV_y_start + 133), unitview_button_background_seg);
        }
    }


    /*
        CalledFromFlag && thing_idk
        
        State 1:  Prod && Bldg
        State 2:  Prod && Norm
        State 3:  Stat
        State 4:  Cmbt
    */
    View_Type = CalledFromFlag;
    if(CalledFromFlag == uvt_Prod)
    {
        if(unit_idx < 100)
        {
            View_Type = 0;
        }
        else
        {
            unit_idx -= 100;
            unit_type_idx = unit_idx;
            View_Type = 1;
        }
    }
    else
    {
        unit_type_idx = _UNITS[thing_idx].type;
    }
    uv_hero = ST_FALSE;
    if(CalledFromFlag != uvt_Prod)
    {
        if(_UNITS[thing_idx].Hero_Slot != -1)
        {
            uv_hero = ST_TRUE;
        }
    }
    /*
        CalledFromFlag && View_Type && uv_hero
        {0,0,0}  Prod-Bldg
        {0,1,0}  Prod-Norm
        {1,1,0}  Stat-Norm
        {2,2,0}  Cmbt-Norm
        {1,1,1}  Stat-Hero
        {2,2,1}  Cmbt-Hero
    
    */

    colors[0] = 197;
    colors[1] = 255;

    Set_Font_Colors_15(1, &colors[0]);

    Set_Font_Style_Outline(1, 15, 0, 0);

    Set_Outline_Color(251);

    UV_x_start_offset = 0;

    if(CalledFromFlag == uvt_Prod)  /* on 'Production Screen' */
    {

        product_idx = _CITIES[_city_idx].construction;

        _CITIES[_city_idx].construction = thing_idx;

        product_cost = City_Current_Product_Cost(_city_idx);  // Discount Cost

        _CITIES[_city_idx].construction = product_idx;
if(_CITIES[_city_idx].construction < 0)
{
    STU_DEBUG_BREAK();
}
if(_CITIES[_city_idx].construction > 298)
{
    STU_DEBUG_BREAK();
}

        UV_x_start_offset = 4;

        Print((UV_x_start + UV_x_start_offset + 51), (UV_y_start + 34), "Cost");

        SDL_itoa(product_cost, GUI_NearMsgString, 10);

        if(View_Type == 1)  /* Product is Unit */
        {
            
            strcat(GUI_NearMsgString, "(");

            SDL_itoa(_unit_type_table[unit_type_idx].cost, temp_string, 10);  // Full Cost

            strcat(GUI_NearMsgString, temp_string);

            strcat(GUI_NearMsgString, ")");

        }

        Print((UV_x_start + UV_x_start_offset + 73), (UV_y_start + 34), GUI_NearMsgString);

    }



    // Dasm show `if != 0,1,2 then fall-through`; probably was a switch statement
    // HERE: (View_Type == 0) means the CalledFromFlag was 0 and the thing_idx was less than 100  i.e., On 'Production Screen', producing 'Building'
    if(View_Type == 0)
    {
/*
    BEGIN:  Buidling View
*/

        bldg_pict_draw_x = (UV_x_start + 6);
        bldg_pict_draw_y = (UV_y_start + 4);

        Mark_Block(_screen_seg);
        bldg_pict_seg = Allocate_Next_Block(_screen_seg, 500);  // 500 PR  8000 B
        Release_Block(_screen_seg);

        Draw_Building_Picture_To_Bitmap(_city_idx, bldg_idx, &bldg_pict_x1, &bldg_pict_y1, &bldg_pict_w, &bldg_pict_h, bldg_pict_seg);

        bldg_pict_draw_x += (((44 - bldg_pict_w) / 2) - bldg_pict_x1);
        bldg_pict_draw_y += (((36 - bldg_pict_h) / 2) - bldg_pict_y1);

        FLIC_Set_LoopFrame_1(bldg_pict_seg);

        Draw_Picture(bldg_pict_draw_x, bldg_pict_draw_y, bldg_pict_seg);

        colors[0] = 197;
        colors[1] = 14;
        colors[2] = 255;
        colors[3] = 80;
        colors[4] = 255;

        Set_Font_Colors_15(4, colors);
        Set_Font_Style_Outline(4, 15, 0, 0);
        Set_Outline_Color(251);
        Set_Font_Spacing_Width(1);

        strcpy(GUI_NearMsgString, bldg_data_table[bldg_idx].name);

        if(Get_String_Width(GUI_NearMsgString) > 100)
        {
            Print_Paragraph((UV_x_start_offset + UV_x_start + 51), (UV_y_start + 7), 100, GUI_NearMsgString, 0);
        }
        else
        {
            Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + 11), GUI_NearMsgString);
        }

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 47), "Maintenance");

        colors[0] = 197;
        colors[1] = 255;

        Set_Font_Colors_15(2, colors);
        Set_Font_Style_Outline(2, 15, 0, 0);
        Set_Outline_Color(251);

        if(bldg_data_table[bldg_idx].maintenance_cost == 0)
        {
            Print_Integer_Right((UV_x_start_offset + UV_x_start + 92), (UV_y_start + 48), bldg_data_table[bldg_idx].maintenance_cost);
        }
        else
        {
            Gold_Icons = abs(bldg_data_table[bldg_idx].maintenance_cost);

            for(itr = 0; itr < Gold_Icons; itr++)
            {
                FLIC_Draw((UV_x_start_offset + UV_x_start + (itr * 9) + 89), (UV_y_start + 49), gold1_icon_seg);
            }
        }

        Building_Allows_List__WIP(bldg_idx, &Allows_Count, &Allows_List[0]);

        colors[0] = 197;
        colors[1] = 14;
        colors[2] = 255;
        colors[3] = 80;
        colors[4] = 255;

        Set_Font_Colors_15(4, colors);
        Set_Font_Style_Outline(4, 15, 0, 0);
        Set_Outline_Color(251);

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 58), "Allows");

        Text_Left = (UV_x_start_offset + UV_x_start + 60);
        Text_Top = (UV_y_start + 59);

        colors[0] = 197;
        colors[1] = 255;

        Set_Font_Colors_15(2, colors);
        Set_Font_Style_Outline(2, 15, 0, 0);
        Set_Font_Spacing_Width(1);
        Set_Font_LF(1);

        strcpy(GUI_NearMsgString, "");

        Allows_List_Size = 0;

        for(itr = 0; itr < Allows_Count; itr++)
        {
            Add_Comma_Or_And(&Allows_List_Size, Allows_Count, GUI_NearMsgString);

            if(Allows_List[itr] < 100)
            {
                strcpy(temp_string, bldg_data_table[Allows_List[itr]].name);
                strcat(GUI_NearMsgString, temp_string);
            }
            else
            {
                strcat(GUI_NearMsgString, *_unit_type_table[(Allows_List[itr] - 100)].name);
            }
        }

        if(Allows_Count > 0)
        {
            strcat(GUI_NearMsgString, ".");
        }

        Print_Paragraph(Text_Left, Text_Top, 100, GUI_NearMsgString, 0);

        colors[0] = 197;
        colors[1] = 14;
        colors[2] = 255;
        colors[3] = 80;
        colors[4] = 255;

        Set_Font_Colors_15(4, colors);
        Set_Font_Style_Outline(4, 15, 0, 0);
        Set_Outline_Color(251);
        Set_Font_Spacing_Width(2);

        Print_Paragraph((UV_x_start_offset + UV_x_start + 9), (UV_y_start + 107), 147, building_description, 0);

/*
    END:  Buidling View
*/
    }
    else if( (View_Type == 1) || (View_Type == 2) )
    {
/*
    BEGIN:  Unit View
*/
        colors[0] = 197;
        colors[1] =  14;
        colors[2] = 255;
        colors[3] =  80;
        colors[4] = 255;
        Set_Font_Colors_15(4, &colors[0]);
        Set_Font_Style_Outline(4, 15, 0, 0);
        Set_Outline_Color(251);

        if((CalledFromFlag != uvt_Prod) && (_UNITS[unit_idx].Hero_Slot != -1))
        {
            hero_owner_idx = _UNITS[unit_idx].owner_idx;
            FLIC_Draw((UV_x_start + 9), (UV_y_start + 7), m_hero_portrait_seg);
            FLIC_Draw((UV_x_start + 8), (UV_y_start + 6), IMG_USW_Portrt_Brdr);
            Print((UV_x_start + 51), (UV_y_start + 6), _players[hero_owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].name);
            Print((UV_x_start + 51), (UV_y_start + 17), "The");
            Print((UV_x_start + 51 + 24), (UV_y_start + 17), *_unit_type_table[unit_type_idx].name);
        }
        else
        {
            
            if(
                (global_battle_unit->carry_capacity > 0) ||
                ((CalledFromFlag == uvt_Stat) && (Square_Is_Sailable(_UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _UNITS[unit_idx].wp) == ST_TRUE))
            )
            {
                FLIC_Draw((UV_x_start_offset + UV_x_start + 6), (UV_y_start + 18), unit_water_diamond_seg);
                Draw_Unit_Figure((UV_x_start_offset + UV_x_start + 11), (UV_y_start - 3), unit_idx, CalledFromFlag);
            }
            else
            {
                Set_Animation_Frame(unit_grass_diamond_seg, 0);
                FLIC_Draw(( UV_x_start_offset + (UV_x_start + 5)), (UV_y_start + 5), unit_grass_diamond_seg);
                Draw_Unit_Figure((UV_x_start_offset + UV_x_start + 11), (UV_y_start - 3), unit_idx, CalledFromFlag);  // HERE: unit_idx = (thing_idx - 100)
            }
            
            if(
                (CalledFromFlag == uvt_Prod) ||
                ((global_battle_unit->Attribs_1 & USA_RACE_SPECIFIC_UNIT) == 0)
            )
            {
                product_idx = unit_type_idx;
                // WTF? or      _DI_CalledFromFlag, _DI_CalledFromFlag
                // WTF? jz      short $+2
                bldg_pict_y1 = 0;
                if (CalledFromFlag == uvt_Prod)
                {
                    bldg_pict_y1 = -4;
                }
                Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + bldg_pict_y1 + 11), *_unit_type_table[product_idx].name);
            }
            else
            {
                unit_race = global_battle_unit->race;
                bldg_pict_x1 = unit_type_idx;
                // if(unit_race < rt_Arcane) // TODO  figure out how the Dasm makes sense for race-specific, race 14 Standard - Trireme, Galley, Catapult, Warship
                if(unit_race < rt_Standard)
                {
                    Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + 11), *_race_type_table[unit_race].name);
                    product_idx = Get_String_Width(*_race_type_table[unit_race].name);
                    Print((UV_x_start_offset + UV_x_start + product_idx + 56), (UV_y_start + 11), *_unit_type_table[bldg_pict_x1].name);
                }
                else
                {
                    Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + 11), _race_type_names[unit_race]);  // TODO  figure out the structure that would make the indexing in the Dasm make sense
                    product_idx = Get_String_Width(_race_type_names[unit_race]);
                    Print((UV_x_start_offset + UV_x_start + product_idx + 56), (UV_y_start + 11), *_unit_type_table[bldg_pict_x1].name);
                }
            }
        
        }


        // @@After_PortraitFigure_Name

        colors[0] = 197;
        colors[1] = 255;
        Set_Font_Colors_15(1, &colors[0]);
        Set_Font_Style_Outline(1, 15, 0, 0);
        Set_Outline_Color(251);
        bldg_pict_y1 = 0;
        if(CalledFromFlag == uvt_Prod)
        {
            bldg_pict_y1 = -7;
        }
        Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + bldg_pict_y1 + 27), "Moves");
        Moves_Type_Icon(global_battle_unit->Move_Flags, &moves_icon_idx);  // {11,12,13} {"walking", "fly", "swim"}
        bldg_pict_x1 = 0;
        for(itr = 0; (global_battle_unit->movement_points / 2) > itr; itr++)
        {
            FLIC_Draw((UV_x_start_offset + UV_x_start + bldg_pict_x1 + 84), (UV_y_start + bldg_pict_y1 + 25), unitview_stat_icons_seg[moves_icon_idx]);
            bldg_pict_x1 += 8;
        }

        if(CalledFromFlag != uvt_Prod)
        {
            product_idx = unit_type_idx;
            // 'Hydra' manual override of figure count, because 'Cur_Figures' is actually 'Heads'
            if(spell_data_table[SPL_HYDRA].Param0 == product_idx)
            {
                Print((UV_x_start_offset + UV_x_start + 121), (UV_y_start + 27), "Heads");
                Print_Integer_Right((UV_x_start_offset + UV_x_start + 153), (UV_y_start + 27), global_battle_unit->Cur_Figures);
            }
        }

        if(
            (CalledFromFlag == uvt_Prod)
            ||
            (CalledFromFlag == uvt_Stat)
        )
        {
            bldg_pict_y1 = 0;
            if(CalledFromFlag == uvt_Prod)
            {
                bldg_pict_y1 = -7;
            }
            Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + bldg_pict_y1 + 34), "Upkeep");
            if(CalledFromFlag == uvt_Prod)
            {
                // Production Screen
                UV_Draw_Upkeep_Icons((UV_x_start_offset + UV_x_start + 85), (UV_y_start + bldg_pict_y1 + 33), -(unit_type_idx));
            }
            else
            {
                // Main Screen, City Screen, Armies Screen, Item Screen
                UV_Draw_Upkeep_Icons((UV_x_start_offset + UV_x_start + 85), (UV_y_start + bldg_pict_y1 + 33), unit_idx);
            }
        }
        else
        {
            // Combat Screen
            Print((UV_x_start + 51), (UV_y_start + 34), "Damage");
            Print_Integer_Right((UV_x_start + 90), (UV_y_start + 34), global_battle_unit->front_figure_damage);
        }

        colors[0] = 197;
        colors[1] = 14;
        colors[2] = 255;
        colors[3] = 80;
        colors[4] = 255;

        Set_Font_Colors_15(4, &colors[0]);
        Set_Font_Style_Outline(4, 15, 0, 0);
        Set_Outline_Color(251);

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 47), "Melee");
        Melee_Type_Icon(global_battle_unit->Weapon_Plus1, &attack_icon_index);
        stat_icon_row_limit = 15;
        if(CalledFromFlag == uvt_Prod)
        {
            stat_icon_row_limit = 10;
        }
        UV_Draw_Stat_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 47), attack_icon_index, global_battle_unit->melee, stat_icon_row_limit, global_battle_unit->Gold_Melee, global_battle_unit->Grey_Melee);

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 58), "Range");
        if(
            (global_battle_unit->ranged_type != -1) &&
            (
                ((global_battle_unit->ranged_type / 10) == 1) || /* RGrp_Boulder */
                ((global_battle_unit->ranged_type / 10) == 2) || /* RGrp_Missile */
                ((global_battle_unit->ranged_type / 10) == 3)    /* RGrp_Magic   */
            )
        )
        {
            Ranged_Type_Icon(global_battle_unit->ranged_type, &attack_icon_index);
            UV_Draw_Stat_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 58), attack_icon_index, global_battle_unit->ranged, stat_icon_row_limit, global_battle_unit->Gold_Ranged, global_battle_unit->Grey_Ranged);
        }

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 68), "Armor");
        UV_Draw_Stat_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 68), 9, global_battle_unit->defense, stat_icon_row_limit, global_battle_unit->Gold_Defense, global_battle_unit->Grey_Defense);

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 79), "Resist");
        UV_Draw_Stat_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 79), 14, global_battle_unit->resist, stat_icon_row_limit, global_battle_unit->Gold_Resist, global_battle_unit->Grey_Resist);

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 90), "Hits");
        UV_Draw_Hits_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 90), 10, global_battle_unit->hits, global_battle_unit->front_figure_damage, stat_icon_row_limit, global_battle_unit->Gold_Hits);

        USW_DrawListPage((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 108), specials_list, specials_count, uv_specials_list_index, item_icon_pict_seg);

/*
    END:  Unit View
*/
    }

}


// WZD o72p06
/*
    calls Draw_Resource_Icons() for gold, mana, food
    unit_or_neg_type
        if negative, then Unit Type
        if non-negative, then Unit Index

XREF:
Up J NX_j_UV_Draw_Upkeep_Icons jmp     UV_Draw_Upkeep_Icons            ; draws the icon representation of the unit's total
Up p Thing_View_Draw__WIP+6E0  call    near ptr UV_Draw_Upkeep_Icons   ; draws the icon representation of the unit's total

*/
void UV_Draw_Upkeep_Icons(int16_t xstart, int16_t ystart, int16_t unit_or_neg_type)
{
    int16_t mana_upkeep;
    int16_t gold_upkeep;
    int16_t total_upkeep;
    int16_t icon_x;
    int16_t icon_y;
    int16_t unit_idx;  // _SI_
    int16_t food_upkeep;  // _DI_

    unit_idx = unit_or_neg_type;

    if(unit_or_neg_type > -1)
    {
        // Unit Index
        gold_upkeep = Unit_Gold_Upkeep(unit_idx);
        mana_upkeep = Unit_Mana_Upkeep(unit_idx);
        food_upkeep = 0;
        if(
            ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_FANTASTIC) != 0)
            &&
            (_UNITS[unit_idx].type > ut_Trireme)  /* ¿ BUG: should > ut_Chosen ? */
        )
        {
            food_upkeep = 1;
        }
        if((_UNITS[unit_idx].mutations & UM_UNDEAD) != 0)
        {
            food_upkeep = 0;
        }
    }
    else
    {
        // Unit Type
        unit_idx = abs(unit_or_neg_type);
        gold_upkeep = _unit_type_table[unit_idx].Upkeep;
        mana_upkeep = 0;
        food_upkeep = 1;
    }

    total_upkeep = gold_upkeep + mana_upkeep + food_upkeep;

    icon_y = ystart;
    icon_x = xstart;

    Draw_Resource_Icons(&icon_x, icon_y, total_upkeep, gold_upkeep, city_big_resource_icon_seg[2], city_lil_resource_icon_seg[2]);

    Draw_Resource_Icons(&icon_x, icon_y, total_upkeep, mana_upkeep, city_big_resource_icon_seg[3], city_lil_resource_icon_seg[3]);

    Draw_Resource_Icons(&icon_x, icon_y, total_upkeep, food_upkeep, city_big_resource_icon_seg[0], city_lil_resource_icon_seg[0]);

}

// WZD o72p07
// TODO  figure what the Gold and Grey really and rename everything and properly document the process
// UV_Draw_Stat_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 47), attack_icon_index, global_battle_unit->melee, stat_icon_row_limit, global_battle_unit->Gold_Melee, global_battle_unit->Grey_Melee)
void UV_Draw_Stat_Icons(int16_t xstart, int16_t ystart, int16_t icon_idx, int16_t Attrib, int16_t row_length, int16_t Gold, int16_t Grey)
{
    int16_t total_icons;
    int16_t row2_length;
    int16_t icon_y;
    int16_t icon_x;  // _SI_
    int16_t itr;  // _DI_

    total_icons = Attrib + Grey;

    if(total_icons >= 1)
    {
        if((row_length * 2) < total_icons)
        {
            if((row_length * 2) < Attrib)
            {
                total_icons = (row_length * 2);
                Grey = 0;
            }
            else
            {
                Grey = ((row_length * 2) - Attrib);
                total_icons = (Attrib + Grey);
            }
        }

        Gold = (total_icons - Gold);
        Grey = (total_icons - Grey);

        icon_x = xstart;
        icon_y = ystart;

        if(total_icons <= row_length)
        {
            for(itr = 0; itr < total_icons; itr++)
            {
                if(itr < Gold)
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                }
                else
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                }

                if(itr >= Grey)
                {
                    Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                }

                icon_x += 10;

                if(((itr + 1) % 5) == 0)
                {
                    icon_x += 2;
                }
            }
        }
        else
        {
            row2_length = (total_icons - row_length);

            for(itr = 0; itr < row_length; itr++)
            {
                if(itr < Gold)
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                }
                else
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                }

                if(itr >= Grey)
                {
                    Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                }

                icon_x += 10;

                if(((itr + 1) % 5) == 0)
                {
                    icon_x += 2;
                }
            }

            icon_x = (xstart + 3);
            icon_y = (ystart + 2);

            for(itr = 0; itr < row2_length; itr++)
            {
                if((itr + row_length) < Gold)
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                }
                else
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                }

                if((itr + row_length) >= Grey)
                {
                    Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                }

                icon_x += 10;

                if(((itr + 1) % 5) == 0)
                {
                    icon_x += 2;
                }
            }
        }
    }

}


// WZD o72p08
/*
UV_Draw_Hits_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 90), 10, global_battle_unit->hits, stat_icon_row_limit, global_battle_unit->TopFig_Dmg, global_battle_unit->Gold_Hits);
global_battle_unit->hits is copied from the unit_type_table
base hits?
includes level bonuses?
    improvements are added to the base attributes (before level raises) of the unit
haven't seen that yet, because it would apply to the 'Production Screen'?
the (bonus = total - bonus) must mean that the total already includes the bonus? so, it's definitely total, not base?

    ¿ vs. UV_Draw_Stat_Icons() ?
        ~ 3 icons  regular, bonus, damage
        total is base + bonus
        or base is total - bonus
        draw regular and/or bonus
        until undamaged count is reach
        then draws damaged regular and/or bonus

"damage hits against its total hits"
"darkened hearts"
"more of its hearts are darkened"

¿ where do bonus hits come from ?
*/
void UV_Draw_Hits_Icons(int16_t xstart, int16_t ystart, int16_t icon_idx, int16_t hits_total, int16_t hits_damaged, int16_t row_length, int16_t hits_bonus)
{
    int16_t row2_length;
    int16_t row1_length;
    int16_t hits_undamaged;
    int16_t icon_y;
    int16_t icon_x;  // _SI_
    int16_t itr;  // _DI_
    int16_t hits_base;  // DNE in Dasm

    hits_base = (hits_total - hits_bonus);

    if(hits_total >= 1)
    {
        if((row_length * 2) < hits_total)
        {
            hits_total = (row_length * 2);
        }

        icon_x = xstart;
        icon_y = ystart;

        if(hits_total <= row_length)
        {
            hits_undamaged = (hits_total - hits_damaged);

            for(itr = 0; itr < hits_total; itr++)
            {
                if(itr >= hits_base)
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                }
                else
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                }

                if(itr >= hits_undamaged)
                {
                    Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                }

                icon_x += 10;

                if(((itr + 1) % 5) == 0)
                {
                    icon_x += 2;
                }
            }
        }
        else
        {
            row1_length = row_length;
            row2_length = (hits_total - row_length);

            if(hits_damaged < row2_length)
            {
                for(itr = 0; itr < row_length; itr++)
                {
                    if(itr >= hits_base)
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                    }
                    else
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                    }

                    icon_x += 10;

                    if(((itr + 1) % 5) == 0)
                    {
                        icon_x += 2;
                    }
                }

                icon_x = (xstart + 3);
                icon_y = (ystart + 2);

                hits_undamaged = (row2_length - hits_damaged);

                for(itr = 0; itr < row2_length; itr++)
                {
                    if(itr >= (hits_base - row_length))
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                    }
                    else
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                    }

                    if(itr >= hits_undamaged)
                    {
                        Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                    }

                    icon_x += 10;

                    if(((itr + 1) % 5) == 0)
                    {
                        icon_x += 2;
                    }
                }
            }
            else  /* (hits_damaged >= row2_length) */
            {
                for(itr = 0; itr < row2_length; itr++)
                {
                    if(itr >= (hits_base - row_length))
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                    }
                    else
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                    }

                    Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed

                    icon_x += 10;

                    if(((itr + 1) % 5) == 0)
                    {
                        icon_x += 2;
                    }
                }

                icon_x = (xstart + 3);
                icon_y = (ystart + 2);

                hits_undamaged = (row1_length - (hits_damaged - row2_length));

                for(itr = 0; itr < row1_length; itr++)
                {
                    if(itr >= hits_base)
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                    }
                    else
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                    }

                    if(itr >= hits_undamaged)
                    {
                        Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                    }

                    icon_x += 10;

                    if(((itr + 1) % 5) == 0)
                    {
                        icon_x += 2;
                    }
                }
            }
        }
    }

}

// WZD o72p09
/*
    UnitView: Thing_View_Draw__WIP()
    UnitList: Draw_Unit_List_Window_Pup()

    Dasm looks like generated code
        tests 0x00, which is a no-op
        only tests one bit, in-order of types
*/
void Moves_Type_Icon(int16_t move_flags, int16_t * moves_icon_idx)
{
    int16_t icon_idx;  // _DI_

    icon_idx = 11;

    if((move_flags & MV_WALKING) != 0)
    {
        icon_idx = 11;
    }
    else if((move_flags & MV_CAVALRY) != 0)
    {
        icon_idx = 11;
    }
    else if((move_flags & MV_SAILING) != 0)
    {
        icon_idx = 13;
    }
    else if((move_flags & MV_FLYING) != 0)
    {
        icon_idx = 12;
    }
    else if((move_flags & MV_SWIMMING) != 0)
    {
        icon_idx = 13;
    }
    else if((move_flags & MV_TELEPORT) != 0)
    {
        icon_idx = 11;
    }
    else if((move_flags & MV_FORESTER) != 0)
    {
        icon_idx = 11;
    }
    else if((move_flags & MV_MOUNTAINEER) != 0)
    {
        icon_idx = 11;
    }
    else if((move_flags & MV_MERGING) != 0)
    {
        icon_idx = 11;
    }

    *moves_icon_idx = icon_idx;
}


// WZD o72p10
/*
    OON Thing_View_Draw__WIP()



*/
void USW_DrawListPage(int16_t xstart, int16_t ystart, struct s_UV_List * specials_list, int16_t specials_count, int16_t specials_item, SAMB_ptr item_icon_pict_seg)
{
    uint8_t colors[6];
    int16_t item_icon_y_start;
    int16_t item_icon_x_start;
    int16_t display_count;
    int16_t itr; // _SI_

    display_count = 8;

    if((specials_item + display_count) > specials_count)
    {
        display_count = (specials_count - specials_item);
    }

    if(
        (specials_item == 0) &&
        (uv_hero == ST_TRUE)
    )
    {
        display_count = 4;
    }

    colors[0] = 197;
    colors[1] = 255;
    colors[2] =  80;
    colors[3] = 255;

    Set_Font_Colors_15(2, &colors[0]);
    Set_Font_Style_Outline(2, 15, 0, 0);
    Set_Outline_Color(251);
    Set_Font_Spacing_Width(1);

    item_icon_y_start = ystart;

    for(itr = 0; itr < display_count; itr++)
    {
        item_icon_x_start = xstart;
        if(display_count > 4)
        {
            if(itr > 3)
            {
                item_icon_x_start += 107;
                if(itr == 4)
                {
                    item_icon_y_start = ystart;
                }
            }
        }

        if(specials_list->picts[(specials_item + itr)] != -1)
        {
            FLIC_Draw((item_icon_x_start - 1), (item_icon_y_start - 1), unitview_specials_borders_seg[itr]);

            if(specials_list->flags[(specials_item + itr)] == -99)
            {
                // SPECIAL.LBX, 003  UNICON1   item
                FLIC_Draw(item_icon_x_start, item_icon_y_start, special_seg[3]);
                // helps was set to the negative if the hero item slot was -1/invalid/empty so do 'slot' instead of 'item'
                if(specials_list->helps[(specials_item + itr)] > 0)
                {
                    Draw_Item_Icon_With_Enchantment_Outline(specials_list->helps[(specials_item + itr)], item_icon_pict_seg);
                    Draw_Picture_Windowed((item_icon_x_start - 2), (item_icon_y_start - 2), item_icon_pict_seg);
                }
                else
                {
                    FLIC_Draw(item_icon_x_start, item_icon_y_start, (SAMB_ptr)specials_list->picts[(specials_item + itr)]);
                }
            }
            else
            {
                FLIC_Draw(item_icon_x_start, item_icon_y_start, (SAMB_ptr)specials_list->picts[(specials_item + itr)]);
            }
        }

        Print((item_icon_x_start + 19), (item_icon_y_start + 5), specials_list->names[(specials_item + itr)]);

        item_icon_y_start += 17;
    }

    Cycle_Item_Enchantment_Animation();

}


// WZD o72p11
/*
    adds the fields for the icons and descriptions for the specials/effects

*/
void UV_Add_Specials_Fields(int16_t xstart, int16_t ystart, struct s_UV_List * specials_list, int16_t specials_count, int16_t specials_index)
{
    int16_t display_index;
    int16_t xpos;
    int16_t display_count;
    int16_t itr;  // _SI_
    int16_t ypos;  // _DI_

    UV_Clear_Specials_Fields();

    display_count = 8;

    if((specials_index + display_count) > specials_count)
    {
        display_count = (specials_count - specials_index);
    }

    if((uv_specials_list_index == 0) && (uv_hero == ST_TRUE))
    {
        display_count = 4;
    }

    UV_Add_UpDn_Buttons((xstart + 196), (ystart - 3));

    display_index = 0;

    ypos = ystart;

    for(itr = 0; itr < display_count; itr++)
    {
        xpos = xstart;
        
        if((display_count > 4) && (itr > 3))
        {
            xpos += 107;
            if(itr == 4)
            {
                ypos = ystart;
            }
        }

        if(specials_list->flags[(specials_index + itr)] == -99)  /* 'Special' is 'Item' */
        {
            uv_item_icon_fields[display_index] = Add_Picture_Field(xpos, ypos, (SAMB_ptr)specials_list->picts[(specials_index + itr)], 0, ST_UNDEFINED);
            uv_item_desc_fields[display_index] = Add_Hidden_Field((xpos + 19), (ypos + 5), (xpos + 93), (ypos + 13), 0, ST_UNDEFINED);
            display_index++;
        }
        else
        {
            uv_special_icon_fields[itr] = Add_Picture_Field(xpos, ypos, (SAMB_ptr)specials_list->picts[(specials_index + itr)], 0,  specials_list->helps[(specials_index + itr)]);
            uv_special_desc_fields[itr] = Add_Hidden_Field((xpos + 19), (ypos + 5), (xpos + 93), (ypos + 13), 0, specials_list->helps[(specials_index + itr)]);
            uv_specials_fields_ability_flags[itr] = specials_list->flags[(specials_index + itr)];
        }
        ypos += 17;
    }

}


// WZD o72p12
void UV_Set_Specials_Help_Fields(struct s_UV_List * specials_list, int16_t specials_count, int16_t specials_index)
{
    int16_t display_count;
    int16_t itr;  // _SI_
    int16_t display_index;  // _DI_

    display_index = specials_index;

    if(uv_multipage != ST_TRUE)
    {
        // TODO  _help_entries.help_idx + 0x46 + (display_index * sizeof(struct s_HELP_FIELD)] = ST_UNDEFINED;
        // TODO  _help_entries.help_idx + 0x50 + (display_index * sizeof(struct s_HELP_FIELD)] = ST_UNDEFINED;
        // *((int16_t *)&_help_entries.help_07 + (display_index * sizeof(struct s_HELP_FIELD))) = ST_UNDEFINED;
        // *((int16_t *)&_help_entries.help_08 + (display_index * sizeof(struct s_HELP_FIELD))) = ST_UNDEFINED;
        _help_entries[(7 + display_index)].help_idx = ST_UNDEFINED;
        _help_entries[(8 + display_index)].help_idx = ST_UNDEFINED;
    }

    display_count = 8;

    if((uv_specials_list_index + display_count) > specials_count)
    {
        display_count = (specials_count - uv_specials_list_index);
    }

    if(
        (uv_specials_list_index == 0)
        &&
        (uv_hero == ST_TRUE)
    )
    {
        display_count = 4;
    }

    for(itr = 0; itr < 12; itr++)
    {
        _help_entries[(11 + (display_index + itr))].help_idx = ST_UNDEFINED;
    }

    for(itr = 0; itr < display_count; itr++)
    {
        if(specials_list->flags[(uv_specials_list_index + itr)] == -99)
        {
            if(specials_list->helps[(uv_specials_list_index + itr)] < 0)  /* ¿ negative for item slot type help ? */
            {
                _help_entries[(11 + (display_index + itr))].help_idx = abs(specials_list->helps[(uv_specials_list_index + itr)]);
            }
        }
        else
        {
            if(
                (uv_specials_list_index == 0)
                &&
                (uv_hero == ST_TRUE)
            )
            {
                _help_entries[(11 + (display_index + itr))].help_idx = specials_list->helps[(uv_specials_list_index + itr)];
            }
            else
            {
                _help_entries[(15 + (display_index + itr))].help_idx = specials_list->helps[(uv_specials_list_index + itr)];
            }
        }
    }

}


// WZD o72p13
void UV_Add_UpDn_Buttons(int16_t xstart, int16_t ystart)
{
    unitview_up_button = INVALID_FIELD;
    unitview_dn_button = INVALID_FIELD;

    if(uv_multipage == ST_TRUE)
    {
        unitview_up_button = Add_Button_Field(xstart, ystart,        "", unitview_up_arrow_seg, 'U', ST_UNDEFINED);
        unitview_dn_button = Add_Button_Field(xstart, (ystart + 63), "", unitview_down_arrow_seg, 'D', ST_UNDEFINED);
    }

}

// WZD o72p14
void UV_Clear_Specials_Fields(void)
{
    int16_t itr;  // _AX_

    for(itr = 0; itr < 8; itr++)
    {
        uv_special_icon_fields[itr] = INVALID_FIELD;
        uv_special_desc_fields[itr] = INVALID_FIELD;
        uv_specials_fields_ability_flags[itr]  = INVALID_FIELD;
    }

    for(itr = 0; itr < 3; itr++)
    {
        uv_item_icon_fields[itr] = INVALID_FIELD;
        uv_item_desc_fields[itr] = INVALID_FIELD;
    }
    
}


// WZD o72p15
void UV_Handle_Arrow_Buttons(int16_t input_field_idx, int16_t * display_count)
{

    if(input_field_idx == unitview_dn_button)
    {
        Play_Left_Click();

        if(
            (uv_specials_list_index == 0)
            &&
            (uv_hero == ST_TRUE)
        )
        {
            uv_specials_list_index += 4;
        }
        else
        {
            uv_specials_list_index += 8;
        }

        if(uv_specials_list_index >= uv_specials_list_count)
        {
            uv_specials_list_index = 0;
        }
    }

    if(input_field_idx == unitview_up_button)
    {
        Play_Left_Click();

        if(
            (uv_specials_list_index == 0)
            &&
            (uv_hero == ST_TRUE)
        )
        {
            uv_specials_list_index = ((((uv_specials_list_count - 4) / 8) * 8) + 4);
            if(
                (uv_specials_list_index != 4)
                &&
                (uv_specials_list_index == uv_specials_list_count)
            )
            {
                uv_specials_list_index -= 8;
            }
        }
        else
        {
            uv_specials_list_index -= 8;
        }

        if(uv_specials_list_index < 0)
        {
            if(
                (uv_specials_list_index == -4)
                &&
                (uv_hero == ST_TRUE)
            )
            {
                uv_specials_list_index = 0;
            }
            else
            {
                uv_specials_list_index = ((uv_specials_list_count / 8) * 8);
            }
        }
    }

    *display_count = 8;

    if((uv_specials_list_index + *display_count) > uv_specials_list_count)
    {
        *display_count = (uv_specials_list_count - uv_specials_list_index);
    }

    if(
        (uv_specials_list_index == 0)
        &&
        (uv_hero == ST_TRUE)
    )
    {
        *display_count = 4;
    }

}


// WZD o72p16
void Melee_Type_Icon(int16_t melee_type, int16_t * icon_idx)
{
    if(melee_type > 0)
    {
        if(melee_type == 2)
        {
            *icon_idx = 3;
        }
        else if(melee_type == 3)
        {
            *icon_idx = 2;
        }
        else if(melee_type == 4)
        {
            *icon_idx = 4;
        }
        else
        {
            *icon_idx = 0;
        }
    }
    else
    {
        *icon_idx = 0;
    }
}


// WZD o72p17
void Ranged_Type_Icon(int16_t ranged_type, int16_t * icon_idx)
{
    switch(ranged_type)
    {
        case rat_Rock:
        case rat_Cannon:
        case rat_Sling:
        case rat_Unknown:
        {
            *icon_idx = 6;
        } break;
        case rat_Lightning:
        case rat_Fireball:
        case rat_Sorcery:
        case rat_Deathbolt:
        case rat_Icebolt:
        case rat_Pr_Shaman:
        case rat_Drow:
        case rat_Sprite:
        case rat_Nat_Bolt:
        {
            *icon_idx = 1;
        } break;
        case srat_Thrown:
        {
            *icon_idx = 8;
        } break;
        case srat_FireBreath:
        case srat_Lightning:
        case srat_StoneGaze:
        {
            *icon_idx = 7;
        } break;
        default:
        {
            *icon_idx = 5;
        } break;
    }
}
// WZD o72p18
void UV_Setup_HireScr(int16_t unit_idx, int16_t specials_list_count)
{

    uv_specials_list_index = 0;
    uv_multipage = ST_FALSE;
    uv_specials_list_count = specials_list_count;
    uv_hero = ST_FALSE;

    if(_UNITS[unit_idx].Hero_Slot != -1)
    {
        uv_hero = ST_TRUE;
        uv_specials_list_index = 4;
        m_hero_portrait_seg = LBX_Reload_Next(portrait_lbx_file, _unit_type_table[_UNITS[unit_idx].type].hero_portrait, _screen_seg);
    }

    if(
        (uv_hero == ST_TRUE)
        &&
        (uv_specials_list_count > 4)
    )
    {
        uv_multipage = ST_TRUE;
    }

    if(uv_specials_list_count > 8)
    {
        uv_multipage = ST_TRUE;
    }

    uv_fill_base = ST_FALSE;

}

// WZD o72p19
void UV_Setup_ProdScr(int16_t specials_count)
{
    uv_specials_list_index = 0;

    uv_multipage = ST_FALSE;

    uv_specials_list_count = specials_count;

    uv_hero = ST_FALSE;

    uv_fill_base = ST_FALSE;

}
