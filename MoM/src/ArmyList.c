/*
    Armies Screen

    WIZARDS.EXE
        ovr066
    
    MoO2:
        Module: ¿ Fleet Screen ?
*/

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Util.h"

#include "CITYCALC.h"
#include "../../MoX/src/Help.h"
#include "ItemScrn.h"
#include "MainScr.h"
#include "MainScr_Maps.h"
#include "MOM_SCR.h"
#include "RACETYPE.h"
#include "UNITTYPE.h"   // WTFMATE
#include "UnitView.h"

#include <string.h>

#include "ArmyList.h"



/*
Army List Screen Background

ARMYLIST.LBX
    Entries: 9
    Type:    0  ("Pictures")

Name        Description
ARMYLIST    armylist backgrnd
ARMYLBUT    armylist up but
ARMYLBUT    armylist down but
ARMLBUT2    armylist items bu
ARMLBUT2    armylist ok but
HEROBACK    hero background
HEROBAC2    hero outline
HEROBUT4    hero alchemy butt
HEROBUT4    hero ok butt

*/




// ¿ BEGIN: ArmyList Screen - Initialized Data ?

// WZD dseg:3244
char armylist_hotkey_NUL = '\0';
// char armylist_hotkey_NUL[] = "";
// WZD dseg:3245
char cnst_HOTKEY_I_2 = 'I';
// WZD dseg:3247
char cnst_HOTKEY_O_4 = 'O';
// WZD dseg:3249
char cnst_HOTKEY_Esc5 = '\x1B';
// WZD dseg:324B
char cnst_HOTKEY_U_3 = 'U';
// WZD dseg:324D
char cnst_HOTKEY_D_3 = 'D';
// WZD dseg:324F
char aTheArmiesOf[] = "The Armies Of ";
// WZD dseg:325E
char cnst_Upkeep[] = "UPKEEP";
// WZD dseg:3265
char cnst_ARMYLIST_File2[] = "ARMYLIST";
// WZD dseg:326E
char cnst_PORTRAIT_File[] = "PORTRAIT";
// WZD dseg:3277 00                                              db    0
// WZD dseg:3278 05 07 0A 0B 0D 0E                               NoSide_Explores db SCT_BottomLeft or SCT_TopRight
// WZD dseg:3278                                                                                         ; DATA XREF: TILE_ExploreCorners+Do

// ¿ END: ArmyList Screen - Initialized Data ?





// WZD dseg:956E 00 00                                           mirror_screen_background_seg dw 0           ; DATA XREF: Main_Screen_Load_Pictures+35Fw ...

// WZD dseg:9570
// ; 2 frame image (normal - clicked)
SAMB_ptr armylist_ok_button_seg;
// WZD dseg:9572
// ; 2 frame image (normal - clicked)
SAMB_ptr armylist_items_button_seg;
// WZD dseg:9574
// ; 2 frame image (normal - clicked)  ; should use dseg:9510
SAMB_ptr armylist_down_button_seg;
// WZD dseg:9576
// ; 2 frame image (normal - clicked)  ; should use dseg:9512
SAMB_ptr armylist_up_button_seg;
// WZD dseg:9578
SAMB_ptr armylist_background_seg;

// WZD dseg:957A 00 00 00 00 00 00 00 00 00 00 00 00             item_slot_icons_seg@ ISLT_ICONS <0>      ; DATA XREF: GFX_Swap_AppendItems+A3w ...
// WZD dseg:957A                                                                                         ; array of 6 appended reserved EMM headers in
// WZD dseg:957A                                                                                         ; GFX_Swap_Seg, each with one item slot image
// WZD dseg:9586 00 00                                           IMG_ItemScrn_Ok@ dw 0                   ; DATA XREF: ITEM_ScreenRedraw+3D6r ...
// WZD dseg:9586                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9588 00 00                                           IMG_ItemScrn_Alch@ dw 0                 ; DATA XREF: ITEM_ScreenRedraw+3AEr ...
// WZD dseg:9588                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:958A 00 00                                           IMG_ItemScrn_HeroBr@ dw 0               ; DATA XREF: ITEM_ScreenRedraw+9Cr ...
// WZD dseg:958C 00 00                                           IMG_ItemScrn_BG@ dw 0                   ; DATA XREF: Items_Screen+2Fr ...
// WZD dseg:958E 00 00                                           profile_gem dw 0                        ; DATA XREF: Magic_Screen_Load_Pictures+BFw ...



// WZD dseg:95F2 00 00                                           goto_booty_icon dw 0                    ; DATA XREF: Main_Screen_Load_Pictures+371w ...

// WZD dseg:95F4
// drake178: IMG_Hero_Portraits
// shared with 'Item Screen'
SAMB_ptr hero_portraits_seg[6];

// WZD dseg:9600 00 00 00 00                                     TBL_TERRSTAT_EMS@ dd 0                  ; allocated in the overland djikstra patch





// WZD o66p01
/*
TODO(JimBalcomb,20230922): finish ArmyList_Screen()
    turn back on drawing units, hero portraits, reduced map, unit(+race) name
    buttons moving on any/every click  ...gets to field push down?
    default state is down? ...gets to field push down?
    getting units that aren't owned
    not showing muliple units on a row
    not highlighting the scanned unit
    not drawing hero portraits
    reduced map location?
    reduced map mark?
    *.* unit enchantments
    *.* Unit Statistics Window
    *.* Hero Statistics Window
*/
void ArmyList_Screen(void)
{
    
    int16_t hotkey_ESC;
    int16_t IDK_have_active_stack;
    int16_t y2;
    int16_t x2;
    int16_t y1;
    int16_t x1;
    int16_t screen_changed;
    int16_t button_armylist_items;
    int16_t button_armylist_ok;
    int16_t hotkey_D;
    int16_t hotkey_U;
    int16_t button_armylist_down_left;
    int16_t button_armylist_up_left;
    int16_t button_armylist_down_right;
    int16_t button_armylist_up_right;
    int16_t scanned_field;
    int16_t leave_screen_flag;
    
    int16_t input_field_idx;
    int16_t hotkey_idx_ESC;

    int16_t itr_hero_portraits;

    int16_t itr;  // _SI_


    ArmyList_Screen_Load();

    armylist_start_x = 77;
    armylist_start_y = 24;
    armylist_hero_portrait_start_x = 12;
    armylist_hero_portrait_start_y = 5;
    // TODO  UU_IDK_ArmyList_w42B70 = 5;  // ¿ was to be start_y of second column of hero portraits ?
    // TODO  UU_IDK_ArmyList_w42B6E = 168;  // ¿ was to be start_x of second column of hero portraits ?

    Build_Army_List();

    list_first_item = 0;

    Build_Army_List_Counts();

    Deactivate_Auto_Function();

    Assign_Auto_Function(ArmyList_Screen_Draw, 1);

    Set_Input_Delay(1);

    IDK_have_active_stack = ST_FALSE;

    if(_unit_stack_count != 0)
    {
        IDK_have_active_stack = ST_TRUE;
    }

    ArmyList_Draw_Reduced_Map();

    armylist_upkeep_gold = Player_Armies_Gold_Upkeep(_human_player_idx);
    armylist_upkeep_mana = Player_Armies_And_Enchantments_Mana_Upkeep(_human_player_idx);
    armylist_upkeep_food = Player_Armies_Food_Upkeep(_human_player_idx);

    Load_Palette_From_Animation(armylist_up_button_seg);


    Apply_Palette();


    Clear_Palette_Changes(0, 243);

    Set_Palette_Changes(0, 255);

    Update_Remap_Color_Range(0, 1);

    Deactivate_Help_List();

    Set_ArmyList_Screen_Help();

    armylist_item_scanned_field = 0;  /* ¿ Field NONE ?*/

    screen_changed = ST_FALSE;  // DNE in Dasm

    leave_screen_flag = ST_FALSE;

    while(leave_screen_flag == ST_FALSE)
    {

        Mark_Time();
        
        Clear_Fields();

        ArmyList_Add_List_Fields();

        button_armylist_items = Add_Button_Field(273, 163, "", armylist_items_button_seg, cnst_HOTKEY_I_2, ST_UNDEFINED);

        button_armylist_ok = Add_Button_Field(273, 182, "", armylist_ok_button_seg, cnst_HOTKEY_O_4, ST_UNDEFINED);
        
        hotkey_ESC = Add_Hot_Key(cnst_HOTKEY_Esc5);

        // YayNay Army List Scroll Up
        if(list_first_item != 0)
        {
            hotkey_U = Add_Hot_Key(cnst_HOTKEY_U_3);
            button_armylist_up_right = Add_Button_Field(250, 26, "", armylist_up_button_seg, armylist_hotkey_NUL, ST_UNDEFINED);
            button_armylist_up_left = Add_Button_Field(  60, 26, "", armylist_up_button_seg, armylist_hotkey_NUL, ST_UNDEFINED);
        }
        else
        {
            hotkey_U = INVALID_FIELD;
            button_armylist_up_right = INVALID_FIELD;
            button_armylist_up_left = INVALID_FIELD;
        }
        
        // YayNay Army List Scroll Down
        if( (((list_first_item + 6) - m_army_list_count) != 0) && ((list_item_count - m_army_list_count) != 0) )
        {
            hotkey_D = Add_Hot_Key(cnst_HOTKEY_D_3);
            button_armylist_down_right = Add_Button_Field(250, 139, "", armylist_down_button_seg, armylist_hotkey_NUL, ST_UNDEFINED);
            button_armylist_down_left = Add_Button_Field(  60, 139, "", armylist_down_button_seg, armylist_hotkey_NUL, ST_UNDEFINED);
        }
        else
        {
            hotkey_D = INVALID_FIELD;
            button_armylist_down_right = INVALID_FIELD;
            button_armylist_down_left = INVALID_FIELD;
        }

        input_field_idx = Get_Input();

        scanned_field = Scan_Input();  /* "highlighted unit" */


        /*
            Leave Screen
        */
        if(input_field_idx == button_armylist_ok || input_field_idx == hotkey_ESC)
        {
            Play_Left_Click();
            leave_screen_flag = ST_TRUE;
            current_screen = scr_Main_Screen;
            // TODO  j_o62p01_Empty_pFxn(_human_player_idx);

            if(IDK_have_active_stack != ST_TRUE)
            {
                _unit_stack_count = 0;
            }
            else
            {
                Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, _UNITS[_unit].wx, _UNITS[_unit].wy);
            }
        }


        /*
            Items Button
        */
        if(input_field_idx == button_armylist_items)
        {
                Play_Left_Click();
                leave_screen_flag = ST_TRUE;
                item_pool_item_idx = ST_UNDEFINED;
                current_screen = scr_Item_Screen;
        }


        /*
            Scroll Up
        */
        if( (input_field_idx == button_armylist_up_right) || (input_field_idx == button_armylist_up_left) || (input_field_idx == hotkey_U) )
        {
            Play_Left_Click();
            if(list_first_item != 0)
            {
                list_first_item--;
                Build_Army_List_Counts();
                screen_changed = ST_TRUE;
            }
        }

        /*
            Scroll Down
        */
        if( (input_field_idx == button_armylist_down_right) || (input_field_idx == button_armylist_down_left) || (input_field_idx == hotkey_D) )
        {
            Play_Left_Click();
            if(list_first_item + 6 < m_army_list_count)
            {
                list_first_item++;
                Build_Army_List_Counts();
                screen_changed = ST_TRUE;
            }
        }

        /*
            ¿ left-click army list unit ?
        */
        for(itr = 0; itr < m_armies_list_field_count; itr++)
        {
            if(m_armies_list_fields[itr] == input_field_idx)
            {
                Play_Left_Click();
                _unit = list_armies[itr];
                _map_plane  = _UNITS[_unit].wp;
                Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, _UNITS[_unit].wx, _UNITS[_unit].wy);
                if(all_units_moved == ST_TRUE)
                {
                    all_units_moved = ST_FALSE;
                }
                _active_world_x = _UNITS[_unit].wx;
                _active_world_y = _UNITS[_unit].wy;
                leave_screen_flag = ST_TRUE;
                current_screen = scr_Main_Screen;
            }
        }

        /*
            ¿ right-click army list unit ?
        */
        for(itr = 0; itr < m_armies_list_field_count; itr++)
        {
            if(-(m_armies_list_fields[itr]) == input_field_idx)
            {
                Deactivate_Help_List();
                ArmyList_UnitFigure_Coordinates(itr, &x1, &y1, &x2, &y2);
                Main_Unit_Statistics_Window(list_armies[itr], x1, y1, x2, y2);
                Assign_Auto_Function(ArmyList_Screen_Draw, 1);
                ArmyList_Screen_Load();
                Build_Army_List();
                Build_Army_List_Counts();
                ArmyList_Draw_Reduced_Map();
                Set_Input_Delay(1);
                armylist_upkeep_gold = Player_Armies_Gold_Upkeep(_human_player_idx);
                armylist_upkeep_mana = Player_Armies_And_Enchantments_Mana_Upkeep(_human_player_idx);
                armylist_upkeep_food = Player_Armies_Food_Upkeep(_human_player_idx);
                Deactivate_Help_List();
                Set_ArmyList_Screen_Help();
                screen_changed = ST_TRUE;
                ArmyList_Set_List_Item_Count();
            }
        }

        /*
            scanned army list unit
        */
        for(itr = 0; itr < m_armies_list_field_count; itr++)
        {
            if(m_armies_list_fields[itr] == scanned_field)
            {
                armylist_item_scanned_field = itr;
            }
        }

        /*
            scanned hero portrait
        */
        for(itr_hero_portraits = 0; itr_hero_portraits < armylist_hero_portrait_count; itr_hero_portraits++)
        {
            if(armylist_hero_portrait_fields[itr_hero_portraits] == scanned_field)
            {
                armylist_item_scanned_field = Get_List_Armies_Index(armylist_hero_portrait_unit_indices[itr_hero_portraits]);
            }
        }

        /*
            ¿ left-click hero portrait ?
        */
        for(itr = 0; itr < armylist_hero_portrait_count; itr++)
        {
            if(armylist_hero_portrait_fields[itr] == input_field_idx)
            {
                Play_Left_Click();
                _unit = _players[_human_player_idx].Heroes[itr].unit_idx;
                _map_plane  = _UNITS[_unit].wp;
                Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, _UNITS[_unit].wx, _UNITS[_unit].wy);
                if(all_units_moved == ST_TRUE)
                {
                    all_units_moved = ST_FALSE;
                }
                leave_screen_flag = ST_TRUE;
                current_screen = scr_Main_Screen;
            }
        }

        /*
            ¿ right-click hero portrait ?
        */
        for(itr = 0; itr < armylist_hero_portrait_count; itr++)
        {
            if(-(armylist_hero_portrait_fields[itr]) == input_field_idx)
            {
                Deactivate_Help_List();
                ArmyList_Hero_Portrait_Coords(itr, &x1, &y1, &x2, &y2);
                Main_Unit_Statistics_Window(armylist_hero_portrait_unit_indices[itr], x1, y1, x2, y2);
                Assign_Auto_Function(ArmyList_Screen_Draw, 1);
                ArmyList_Screen_Load();
                Build_Army_List();
                Build_Army_List_Counts();
                ArmyList_Draw_Reduced_Map();
                armylist_upkeep_gold = Player_Armies_Gold_Upkeep(_human_player_idx);
                armylist_upkeep_mana = Player_Armies_And_Enchantments_Mana_Upkeep(_human_player_idx);
                armylist_upkeep_food = Player_Armies_Food_Upkeep(_human_player_idx);
                Deactivate_Help_List();
                Set_ArmyList_Screen_Help();
                ArmyList_Set_List_Item_Count();
                screen_changed = ST_TRUE;
            }
        }


        if(leave_screen_flag == ST_FALSE && screen_changed == ST_FALSE)
        {
            ArmyList_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
        screen_changed = ST_FALSE;
    }

    Clear_Palette_Changes(0, 255);
    Set_Palette_Changes(0, 223);
    Update_Remap_Gray_Palette();
    Deactivate_Auto_Function();
    Deactivate_Help_List();
    Reset_Window();
    Clear_Fields();

}


// WZD o66p02
void ArmyList_Screen_Draw(void)
{
    uint8_t colors1[6];
    uint8_t colors2[6];
    int16_t unit_race;
    int16_t hero_unit_idx;
    int16_t scanned_unit_idx;
    int16_t unit_enchantment_count;
    int16_t unit_type;
    int16_t list_armies_idx;
    int16_t y2;
    int16_t x2;
    int16_t y1;
    int16_t x1;
    int16_t itr_units;

    int16_t itr_colors;
    int16_t itr_hero_portraits;
    // int16_t itr_list_item_count;
    int16_t itr_stacks;
    int16_t itr_unit_enchantment_count;

    int16_t IDK_Hero_Slot;  // _DI_
    int16_t print_y_offset;  // _DX_

    int16_t unit_idx;


    ArmyList_Set_List_Item_Count();

    Set_Page_Off();

    Copy_Back_To_Off();


    /*
        BEGIN: Print Title
    */
    strcpy(GUI_String_1, aTheArmiesOf);
    strcat(GUI_String_1, _players[_human_player_idx].name);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 237;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(162, 9, GUI_String_1);
    Print_Centered(161, 9, GUI_String_1);
    Print_Centered(162, 8, GUI_String_1);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 238;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(161, 7, GUI_String_1);
    Print_Centered(160, 8, GUI_String_1);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 239;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(161, 8, GUI_String_1);
    colors2[0] = 236;
    colors2[1] = 177;
    colors2[2] = 178;
    colors2[3] = 177;
    colors2[4] = 176;
    Set_Font_Colors_15(4, &colors2[0]);
    Print_Centered(160, 7, GUI_String_1);
    /*
        END: Print Title
    */


    Set_Outline_Color(231);
    colors2[0] = 236;
    colors2[1] = 129;
    colors2[2] = 129;
    Set_Font_Colors_15(1, &colors2[0]);
    Set_Font_Style_Shadow_Down(1, 15, 0, 0);

    Print_Integer_Centered(45, 173, armylist_upkeep_gold);
    Print_Integer_Centered(45, 183, armylist_upkeep_mana);
    Print_Integer_Centered(45, 192, armylist_upkeep_food);
    Print_Centered(30, 162, cnst_Upkeep);

    /*
        Up Arrows
    */
    if(list_first_item == 0 && m_army_list_count > 6)
    {
        Set_Animation_Frame(armylist_up_button_seg, 1);
        FLIC_Draw(250, 26, armylist_up_button_seg);
        Set_Animation_Frame(armylist_up_button_seg, 1);
        FLIC_Draw(60, 26, armylist_up_button_seg);
    }
    else
    {
        Set_Animation_Frame(armylist_up_button_seg, 0);
        FLIC_Draw(250, 26, armylist_up_button_seg);
        Set_Animation_Frame(armylist_up_button_seg, 0);
        FLIC_Draw(60, 26, armylist_up_button_seg);
    }

    /*
        Down Arrows
    */
    if(m_army_list_count > NUM_ARMY_LIST && ( (((NUM_ARMY_LIST + list_first_item) - m_army_list_count) == 0)  || ((list_item_count - m_army_list_count) == 0)))
    {
        Set_Animation_Frame(armylist_down_button_seg, 1);
        FLIC_Draw(250, 139, armylist_down_button_seg);
        Set_Animation_Frame(armylist_down_button_seg, 1);
        FLIC_Draw(60, 139, armylist_down_button_seg);
    }
    else
    {
        Set_Animation_Frame(armylist_down_button_seg, 0);
        FLIC_Draw(250, 139, armylist_down_button_seg);
        Set_Animation_Frame(armylist_down_button_seg, 0);
        FLIC_Draw(60, 139, armylist_down_button_seg);
    }

    FLIC_Draw(273, 163, armylist_items_button_seg);

    FLIC_Draw(273, 182, armylist_ok_button_seg);

    if(m_army_list_count <= NUM_ARMY_LIST)
    {
        // TODO  mov     [word ptr _help_entries+78h], 0FFFFh
        // TODO  mov     [word ptr _help_entries+82h], 0FFFFh
        // TODO  mov     [word ptr _help_entries+8Ch], 0FFFFh
        // TODO  mov     [word ptr _help_entries+96h], 0FFFFh
    }

    scanned_unit_idx = list_armies[armylist_item_scanned_field];

    Set_Outline_Color(240);

    /*
        Hero Portraits
    */
    for(itr_hero_portraits = 0; itr_hero_portraits < NUM_HEROES; itr_hero_portraits++)
    {
        hero_unit_idx = _players[_human_player_idx].Heroes[itr_hero_portraits].unit_idx;
        if((hero_unit_idx <= ST_UNDEFINED) || (_UNITS[hero_unit_idx].owner_idx == ST_UNDEFINED) )
        {
            // ¿ help entry offset ? (itr * 10)
            // TODO  *(_help_entries + 60 + (10 * itr)) = 243;  // "EMPTY HERO SLOT"
        }
        else
        {
            ArmyList_Hero_Portrait_Coords(itr_hero_portraits, &x1, &y1, &x2, &y2);

            FLIC_Draw(x1, y1, hero_portraits_seg[itr_hero_portraits]);

            colors2[0] = 236;
            colors2[1] = 129;
            colors2[2] = 129;
            Set_Font_Colors_15(0, &colors2[0]);
            Set_Font_Style_Shadow_Down(0, 15, 0, 0);
            Set_Font_LF(0);
            Set_Font_Spacing_Width(1);
            Print_Centered(x1 + 15, y1 + 37, _players[_human_player_idx].Heroes[itr_hero_portraits].name);

            // ¿ help entry offset ? (itr * 10)
            // TODO  *(_help_entries + 60 + (10 * itr)) = ST_UNDEFINED;
        }
    }

    Set_Outline_Color(231);

    /*
        BEGIN:  Armies, Units
    */
    list_armies_idx = 0;

    for(itr_stacks = 0; itr_stacks < list_item_count; itr_stacks++)
    {
        for(itr_units = 0; itr_units < armylist_list_item_count[itr_stacks]; itr_units++)
        {
            x1 = armylist_start_x + (18 * itr_units);
            y1 = armylist_start_y + (22 * itr_stacks);

            if(list_armies_idx == armylist_item_scanned_field)
            {
                Gradient_Fill((x1 + 1), (y1 + 2), (x1 + 19), (y1 + 16), 15, 1, ST_NULL, ST_NULL, ST_NULL);
            }

            unit_idx = list_armies[list_armies_idx];

            Draw_Unit_StatFig(x1, y1, list_armies[list_armies_idx], 1);

            list_armies_idx++;
        }
    }
    /*
        END:  Armies, Units
    */


    Cycle_Unit_Enchantment_Animation();


    /*
        BEGIN:  Scanned Unit Info
    */
    {
        if(m_army_list_count > 0)
        {
            unit_type = _UNITS[scanned_unit_idx].type;

            IDK_Hero_Slot = _UNITS[scanned_unit_idx].Hero_Slot;

            if(IDK_Hero_Slot != ST_UNDEFINED)
            {
                strcpy(GUI_String_1, _players[_human_player_idx].Heroes[_UNITS[scanned_unit_idx].Hero_Slot].name);
            }
            else
            {
                if((_unit_type_table[unit_type].Attribs_1 & 0x8000) == 0)  /* enum ATTRIB_1 Std_Unit */
                {
                    /* "Standard Units" */
                    strcpy(GUI_String_1, *(_unit_type_table[unit_type].name));
                }
                else
                {
                    /* Race-Specific Units */
                    unit_race = _unit_type_table[unit_type].Race;
                    // TODO  if(unit_race < 15)
                    if(unit_race < 14)
                    {
                        strcpy(GUI_String_1, *_race_type_table[unit_race].name);
                    }
                    else
                    {
                        /* Summoned Units */
                        // mov     bx, [bp+unit_race]
                        // shl     bx, 1
                        // push    [(TBL_Races.Growth_Mod+120h)+bx]
                        // DSEG:1EBA
                        // ¿ indexing into an array of 21 DW's (42 bytes) of offsets to (char *) strings of Summoned Unit Race Names ?
                        // starting at 15 * 2 ... 6 offsets ... Arcane, Nature, Sorcery, Chaos, Life, Death
                        // [(_race_type_table.Growth_Mod+120h)+bx]
                        // TODO  strcpy(GUI_String_1, "SUMMONED");
                        strcpy(GUI_String_1, "");
                    }
                        strcat(GUI_String_1, " ");  // offset aTheArmiesOf+0Dh AKA aTheArmiesOf_SPACE
                        strcat(GUI_String_1, *(_unit_type_table[unit_type].name));
                }
            }

            Set_Outline_Color(231);
            colors2[0] = 236;
            colors2[1] = 129;
            colors2[2] = 129;
            Set_Font_Colors_15(1, &colors2[0]);
            Set_Font_Style_Shadow_Down(1, 15, 0, 0);
            String_To_Upper(GUI_String_1);
            Print_Centered(188, 161, GUI_String_1);


            Get_Unit_Enchantment_Names(scanned_unit_idx, &armylist_unit_enchantment_names[0], &unit_enchantment_count);
            
            if(unit_enchantment_count > 0)
            {
                Set_Font_Style_Shadow_Down(1, 15, 0, 0);
                SETMAX(unit_enchantment_count, 4);
                for(itr_unit_enchantment_count = 0; itr_unit_enchantment_count < unit_enchantment_count; itr_unit_enchantment_count++)
                {
                    if(unit_enchantment_count < 4)
                    {
                        print_y_offset = 7;
                    }
                    else
                    {
                        print_y_offset = 6;
                    }
                    Print(144, 172 + (print_y_offset * itr_unit_enchantment_count), armylist_unit_enchantment_names[itr_unit_enchantment_count]);
                }
            }
        }
    }
    /*
        END:  Scanned Unit Info
    */


    Set_Outline_Color(0);

    ArmyList_Draw_Reduced_Map();

}


// WZD o66p03
/*
    armylist_fields[] field_idx for each Unit
    armylist_hero_portrait_fields[] field_idx for each Hero Portrait
    ¿ IDK_armylist_row_fields[] field_idx for each Row/Item List ?
*/
void ArmyList_Add_List_Fields(void)
{
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
    int16_t hero_unit_idx;
    int16_t itr_list_item_count;
    int16_t itr_armylist_list_item_count;
    int16_t itr_num_hero_portrait;
    int16_t itr_stacks;
    int16_t itr_units;

    m_armies_list_field_count = 0;

    ArmyList_Set_List_Item_Count();

    /*
        iter over the count of armies
            iter over the count of units in each army
    */
    for(itr_stacks = 0; itr_stacks < list_item_count; itr_stacks++)
    {
        for(itr_units = 0; itr_units < armylist_list_item_count[itr_stacks]; itr_units++)
        {
            x1 = armylist_start_x + (18 * itr_units) + 1;
            y1 = armylist_start_y + (22 * itr_stacks) + 1;
            x2 = x1 + 18;  /* unit figure icon width */
            y2 = y1 + 15;  /* unit figure icon height */
            m_armies_list_fields[m_armies_list_field_count] = Add_Hidden_Field(x1, y1, x2, y2, armylist_hotkey_NUL, ST_UNDEFINED);
            m_armies_list_field_count++;
        }
    }

    /*
        IDK_armylist_row_fields
        ¿ may be here just to *block* clicks ?
    */
    IDK_armylist_row_count = 0;
    for(itr_list_item_count = 0; itr_list_item_count < list_item_count; itr_list_item_count++)
    {
        x1 = armylist_start_x + 1;
        y1 = armylist_start_y + (22 * itr_list_item_count) + 1;
        x2 = x1 + 162;  /* armies list row width  (9 * 18) (MAX_STACK * unit figure icon width) */
        y2 = y1 + 15;   /* armies list row height (unit figure icon height) */
        IDK_armylist_row_fields[IDK_armylist_row_count] = Add_Hidden_Field(x1, y1, x2, y2, armylist_hotkey_NUL, ST_UNDEFINED);
        IDK_armylist_row_count++;
    }

    /*
        armylist_hero_portrait_fields
    */
    armylist_hero_portrait_count = NUM_HERO_PORTRAIT;
    for(itr_num_hero_portrait = 0; itr_num_hero_portrait < NUM_HERO_PORTRAIT; itr_num_hero_portrait++)
    {
        hero_unit_idx = _players[_human_player_idx].Heroes[itr_num_hero_portrait].unit_idx;
        if( (hero_unit_idx <= ST_UNDEFINED) || (_UNITS[hero_unit_idx].owner_idx == ST_UNDEFINED) )
        {
            armylist_hero_portrait_fields[itr_num_hero_portrait] = INVALID_FIELD;
            armylist_hero_portrait_unit_indices[itr_num_hero_portrait] = ST_UNDEFINED;
        }
        else
        {
            ArmyList_Hero_Portrait_Coords(itr_num_hero_portrait, &x1, &y1, &x2, &y2);
            armylist_hero_portrait_fields[itr_num_hero_portrait] = Add_Hidden_Field(x1, y1, x2, y2, armylist_hotkey_NUL, ST_UNDEFINED);
            armylist_hero_portrait_unit_indices[itr_num_hero_portrait] = _players[_human_player_idx].Heroes[itr_num_hero_portrait].unit_idx;
        }
    }

}


// WZD o66p04
void ArmyList_UnitFigure_Coordinates(int16_t armylist_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2)
{
    int16_t var_4;
    int16_t var_2;
    int16_t _SI_;
    int16_t _CX_;

    _SI_ = 0;
    
    _CX_ = armylist_list_item_count[_SI_];

    var_2 = 0;

    while(var_2 == 0)
    {
        if(_CX_ > armylist_idx)
        {
            _CX_ = (_CX_ - armylist_list_item_count[_SI_]);
            var_4 = (armylist_idx - _CX_);
            _CX_ = var_4;
            *x1 = (armylist_start_x + (_CX_ * 18) - 1);
            *y1 = (armylist_start_y + (_SI_ * 22) - 1);
            *x2 = (*x1 + 18);
            *y2 = (*y1 + 16);
            var_2 = 1;
        }
        else
        {
            _SI_++;
            _CX_ += armylist_list_item_count[_SI_];
        }
    }

}


// WZD o66p05
void ArmyList_Draw_Reduced_Map(void)
{
    int16_t unit_world_x;
    int16_t unit_world_y;
    int16_t unit_world_p;
    unit_world_x = _UNITS[list_armies[armylist_item_scanned_field]].wx;
    unit_world_y = _UNITS[list_armies[armylist_item_scanned_field]].wy;
    unit_world_p = _UNITS[list_armies[armylist_item_scanned_field]].wp;
    List_Screen_Draw_Reduced_Map(ARMYLIST_REDUCED_MAP_X, ARMYLIST_REDUCED_MAP_Y, ARMYLIST_REDUCED_MAP_W, ARMYLIST_REDUCED_MAP_H, unit_world_p, unit_world_x, unit_world_y);
}


// WZD o66p06
// ¿ MoO2  Get_Sint_Index_From_List_() ?
/*
    loops through m_armies_list_field_count[] to match unit_idx
    three branches back to top of loop, but just fall-throughs
    sets list_first_item to 0 on first run through
    ¿ (only) calls Build_Army_List_Counts(), because it changes list_first_item ?
    ¿ branch around list_first_item is to avoid changing the screen if the hero unit is already displayed ?
    so, runs thorugh current list
    then, restarts from the beginning

    calls Build_Army_List_Counts()
        calls ArmyList_Set_List_Item_Count(), which uses list_first_item to set list_item_count
        uses list_item_count to iterate through armylist_list_item_count
        uses list_first_item to while indexing into armylist_world_?_1000
        updates list_armies[]

*/
/*
    moves army list view to show scanned hero portrait unit
    returns index of list_armies[]
*/
int16_t Get_List_Armies_Index(int16_t unit_idx)
{
    int16_t list_armies_itr;
    int16_t found_unit_flag;
    int16_t itr;
    int16_t first_time_done;

    first_time_done = ST_FALSE;

    found_unit_flag = ST_FALSE;

    while(found_unit_flag == ST_FALSE)
    {

        for(itr = 0; itr < m_armies_list_field_count; itr++)
        {
            if(list_armies[itr] == unit_idx)
            {
                list_armies_itr = itr;
                found_unit_flag = ST_TRUE;
            }
        }

        if(found_unit_flag == ST_FALSE)
        {
            if(first_time_done == ST_FALSE)
            {
                list_first_item = 0;
                first_time_done = ST_TRUE;
            }
            else
            {
                list_first_item++;
            }
            Build_Army_List_Counts();
        }

    }

    return list_armies_itr;
}


// WZD o66p07
/*
calculates m_army_list_count
populates m_armies_wx, m_armies_wy, and m_armies_wp
armylist_unit_count is bogus
*/
void Build_Army_List(void)
{
    int16_t itr_units;
    int16_t itr_armylist_army_count;
    int16_t same_stack;

    armylist_unit_count = 0;  // BUGBUG
    m_army_list_count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == _human_player_idx)
        {
            armylist_unit_count++;  // BUGBUG
            same_stack = ST_UNDEFINED;
            for(itr_armylist_army_count = 0; itr_armylist_army_count < m_army_list_count; itr_armylist_army_count++)
            {
                if(
                    (_UNITS[itr_units].wx == m_armies_wx[itr_armylist_army_count])
                    &&
                    (_UNITS[itr_units].wy == m_armies_wy[itr_armylist_army_count])
                    &&
                    (
                        (_UNITS[itr_units].wp == m_armies_wp[itr_armylist_army_count])
                        ||
                        (_UNITS[itr_units].in_tower == ST_TRUE)
                    )
                )
                {
                    same_stack = ST_TRUE;
                }
            }
            if(same_stack == ST_UNDEFINED)
            {
                m_armies_wx[m_army_list_count] = _UNITS[itr_units].wx;
                m_armies_wy[m_army_list_count] = _UNITS[itr_units].wy;
                m_armies_wp[m_army_list_count] = _UNITS[itr_units].wp;
                m_army_list_count++;
            }
        }
    }

}


// WZD o66p08
// ¿ ~== Update_Cities_List() ?
/*

called from Get_List_Armies_Index(), after changing list_first_item
which could change list_item_count, in the call to ArmyList_Set_List_Item_Count()

*/
void Build_Army_List_Counts(void)
{
    int16_t list_armies_idx;
    int16_t itr_list_item_count;
    int16_t itr_units;

    ArmyList_Set_List_Item_Count();

    list_armies_idx = 0;  /* count of units in the army */

    for(itr_list_item_count = 0; itr_list_item_count < list_item_count; itr_list_item_count++)
    {
        armylist_list_item_count[itr_list_item_count] = 0;

        for(itr_units = 0; itr_units < _units; itr_units++)
        {
            if(
                (_UNITS[itr_units].owner_idx == _human_player_idx)
                &&
                (_UNITS[itr_units].wx == m_armies_wx[(list_first_item + itr_list_item_count)])
                &&
                (_UNITS[itr_units].wy == m_armies_wy[(list_first_item + itr_list_item_count)])
                &&
                (_UNITS[itr_units].wp == m_armies_wp[(list_first_item + itr_list_item_count)])
            )
            {
                list_armies[list_armies_idx] = itr_units;  // ¿ ~== unit_idx ? Yup. SEE: 'left-click army list unit'
                list_armies_idx++;
                armylist_list_item_count[itr_list_item_count]++;
            }
        }
    }

}


// WZD o66p09
// ~== CityList_Set_List_Item_Count()
/*
    sets list item count to max of 6 or army count
    (expects list_first_item and m_army_list_count to already be set)
*/
void ArmyList_Set_List_Item_Count(void)
{
    list_item_count = NUM_ARMY_LIST + list_first_item;
    SETMAX(list_item_count, NUM_ARMY_LIST);
    SETMAX(list_item_count, m_army_list_count);
}



// WZD o66p10
void ArmyList_Screen_Load(void)
{
    int16_t itr;
    int16_t hero_unit_idx;

    int16_t hero_portrait_lbx_entry_num;

    m_armies_wx = Near_Allocate_First(1000);  // 1000 is one byte per unit of 1000 unit count max
    m_armies_wy = Near_Allocate_Next(1000);   // 1000 is one byte per unit of 1000 unit count max
    m_armies_wp = Near_Allocate_Next(1000);   // 1000 is one byte per unit of 1000 unit count max

    armylist_list_item_count = Near_Allocate_Next(54);  // ¿ only uses first six bytes ? NOT 54 for bytes for 6 x 9 Units

    list_armies = (int16_t *)Near_Allocate_Next(108);

    GUI_String_1 = (char *)Near_Allocate_Next(100);

    for(itr = 0; itr < 20; itr++)
    {
        armylist_unit_enchantment_names[itr] = (char *)Near_Allocate_Next(20);
    }

    m_armies_list_fields = (int16_t *)Near_Allocate_Next(108);

    armylist_hero_portrait_fields = (int16_t *)Near_Allocate_Next(12);        // 6 2-byte values
    armylist_hero_portrait_unit_indices = (int16_t *)Near_Allocate_Next(12);  // 6 2-byte values
    IDK_armylist_row_fields = Near_Allocate_Next(12);  // ¿ 12 is 6 2-byte values, 1 per ArmyList List Item Field ?

    Set_Page_Off();

    armylist_background_seg = LBX_Reload("ARMYLIST", 0, _screen_seg);

    FLIC_Draw(0, 0, armylist_background_seg);

    Copy_Off_To_Back();

    armylist_up_button_seg    = LBX_Reload("ARMYLIST", 1, _screen_seg);
    armylist_down_button_seg  = LBX_Reload_Next("ARMYLIST", 2, _screen_seg);
    armylist_items_button_seg = LBX_Reload_Next("ARMYLIST", 3, _screen_seg);
    armylist_ok_button_seg    = LBX_Reload_Next("ARMYLIST", 4, _screen_seg);

    for(itr = 0; itr < NUM_HERO_PORTRAIT; itr++)
    {
        hero_unit_idx = _players[_human_player_idx].Heroes[itr].unit_idx;

        if((hero_unit_idx > -1) && (_UNITS[hero_unit_idx].owner_idx != ST_UNDEFINED))
        {
            hero_portraits_seg[itr] = LBX_Reload_Next("PORTRAIT", _unit_type_table[_UNITS[hero_unit_idx].type].hero_portrait, _screen_seg);
        }
    }

    _reduced_map_seg = Allocate_Next_Block(_screen_seg, 153);  // 153 PR, 2448 B;  ¿ only needs 49 * 33 = 1617 ? ¿ allocating for 60 * 40 = 2400 + 3 16-byte SAMB headers ?

}


// WZD o66p11
// NX_j_IDK_ArmlyIst_PrintParagraph()
// NX_IDK_ArmlyIst_PrintParagraph()


// WZD o66p12
void ArmyList_Hero_Portrait_Coords(int16_t hero_portrait_num, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2)
{
    *x1 = armylist_hero_portrait_start_x + ((hero_portrait_num % 2) * 265);
    *y1 = armylist_hero_portrait_start_y + ((hero_portrait_num / 2) *  51);
    *x2 = *x1 + 32;
    *y2 = *y1 + 32;
}
