/*
    City List Screen - Cities Button

    WIZARDS.EXE
        ovr
    
    MoO2:
        Module: ¿ Planets/Colonies Screen ?
*/

#include "MOX/Allocate.h"
#include "MOX/FLIC_Draw.h"
#include "MOX/Fields.h"
#include "MOX/Fonts.h"
#include "MOX/Graphics.h"
#include "MOX/LBX_Load.h"
#include "MOX/MOX_DAT.h"  /* _screen_seg */
#include "MOX/MOX_DEF.h"
#include "MOX/MOX_T4.h"
#include "MOX/Timer.h"
#include "MOX/Util.h"

#include "City_ovr55.h"
#include "Help.h"
#include "MainScr.h"
#include "MainScr_Maps.h"
#include "MOM_SCR.h"
#include "ProdScr.h"
#include "RACETYPE.h"
#include "UNITTYPE.h"   // WTFMATE
#include "WZD_o059.h"

#include <stdlib.h>
#include <string.h>

#include <SDL_stdinc.h>

#include "CityList.h"



// WZD dseg:31F0                                                 BEGIN: ovr065 - Strings  (CityList Screen)

// WZD dseg:31F0
char emptystring__ovr065 = '\0';
// WZD dseg:31F1
char str_citylist_O = 'O';
// WZD dseg:31F3
char str_citylist_ESC = '\x1B';
// WZD dseg:31F5
char str_citylist_U = 'U';
// WZD dseg:31F7
char str_citylist_D = 'D';
// WZD dseg:31F9
char reload_lbx_file__ovr065[] = "RELOAD";
// WZD dseg:3200
char armylist_lbx_file__ovr065[] = "ARMYLIST";
// WZD dseg:3209
char aTheCitiesOf[] = "The Cities Of ";
// WZD dseg:3218
char aName[] = "Name";
// WZD dseg:321D
char aRace[] = "Race";
// WZD dseg:3222
char aPop[] = "Pop";
// WZD dseg:3226
char cnst_Gold_3[] = "Gold";
// WZD dseg:322B
char aPrd[] = "Prd";
// WZD dseg:322F
char aProducing[] = "Producing";
// WZD dseg:3239
char aTime[] = "Time";
// WZD dseg:323E
char cnst_GP_2[] = "GP";
// WZD dseg:3241
char cnst_MP_2[] = "MP";

// WZD dseg:3241                                                 END: ovr065 - Strings  (CityList Screen)





// WZD dseg:94F2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Workers@ dw 0Eh dup(0)          ; DATA XREF: City_Screen_Load_Pictures+136w ...
// WZD dseg:94F2 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:94F2                                                                                         ; GFX_Swap_Seg, 1 worker image per header

// WZD dseg:950E                                                 BEGIN: CityList_Screen_Load()
// WZD dseg:950E
SAMB_ptr citylist_ok_button_seg;            // ; 2 frame image (normal - clicked)
// WZD dseg:9510
SAMB_ptr citylist_down_button_seg;          // ; 2 frame image (normal - clicked)
// WZD dseg:9512
SAMB_ptr citylist_up_button_seg;            // ; 2 frame image (normal - clicked)
// WZD dseg:9514
SAMB_ptr citylist_background_seg;
// WZD dseg:9514                                                 END: CityList_Screen_Load()

// WZD dseg:9516                                                 BEGIN:  Main Screen Pictures
// WZD dseg:9516 00 00                                           next_turn_button_seg dw 0               ; DATA XREF: Main_Screen_Load_Pictures+299w ...




/*
TODO(JimBalcomb,20230922): finish ArmyList_Screen()
    turn back on drawing city info, city name, enchantments, highlight
    add left-click for 'City Screen'
    add right-click for 'Production Screen'
    gold is broken for "Bremen" - should be 90, shouws -166 ... (Gold - Upkeed)
    *.* city garrison
    *.* city enchantments
    *.* production cost
    *.* production time
*/
// WZD o65p01
void CityList_Screen(void)
{
    int16_t troops[9];
    int16_t troop_count;
    int16_t hotkey_ESC;
    int16_t screen_changed;
    int16_t button_citylist_ok;
    int16_t hotkey_D;
    int16_t hotkey_U;
    int16_t citylist_down_button_right;
    int16_t citylist_up_button_right;
    int16_t citylist_down_button_left;
    int16_t citylist_up_button_left;
    int16_t scanned_field;
    int16_t leave_screen_flag;
    int16_t input_field_idx;  // _DI_
    int16_t itr;  // _SI_

    CityList_Screen_Load();

    Load_Palette_From_Animation(citylist_background_seg);
    Apply_Palette();
    Clear_Palette_Changes(0, 243);
    Set_Palette_Changes(0, 255);
    Update_Remap_Color_Range(0, 1);

    UU_citylist_some_x = 16;
    UU_citylist_some_y = 16;

    Build_City_List();

    list_first_item = m_citylist_first_item;

    Update_Cities_List();

    Army_At_City(list_first_item, &troop_count, &troops[0]);

    Deactivate_Auto_Function();
    Assign_Auto_Function(CityList_Screen_Draw, 1);
    Set_Input_Delay(1);

    CityList_Draw_Reduced_Map();

    Set_Mouse_List(1, mouse_list_default);

    CityList_Set_List_Item_Count();

    citylist_item_scanned_field = 0;

    Set_CityList_Screen_Help();

    Set_Input_Delay(2);

    screen_changed = ST_FALSE;
    leave_screen_flag = ST_FALSE;

    while(leave_screen_flag == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        CityList_Add_List_Fields();

        Set_Font_Style(0, 0, 6, 15);
        button_citylist_ok = Add_Button_Field(239, 182, "", citylist_ok_button_seg, str_citylist_O, ST_UNDEFINED);
        hotkey_ESC = Add_Hot_Key(str_citylist_ESC);

        // YayNay City List Scroll Up
        if(list_first_item != 0)
        {
            // TODO  _help_entries = 244
            // TODO  _help_entries+14h = 244

            hotkey_U = Add_Hot_Key(str_citylist_U);
            citylist_up_button_left  = Add_Button_Field( 11, 26, "", citylist_up_button_seg, emptystring__ovr065, ST_UNDEFINED);
            citylist_up_button_right = Add_Button_Field(299, 26, "", citylist_up_button_seg, emptystring__ovr065, ST_UNDEFINED);
        }
        else
        {
            // TODO  _help_entries = ST_UNDEFINED
            // TODO  _help_entries+14h = ST_UNDEFINED

            hotkey_U = INVALID_FIELD;
            citylist_up_button_left = INVALID_FIELD;
            citylist_up_button_right = INVALID_FIELD;
        }

        // YayNay City List Scroll Down
        if((((list_first_item + 9) - m_city_list_count) != 0) && ((list_item_count - m_city_list_count) != 0))
        {
            hotkey_D = Add_Hot_Key(str_citylist_D);
            citylist_down_button_left  = Add_Button_Field( 11, 139, "", citylist_down_button_seg, emptystring__ovr065, ST_UNDEFINED);
            citylist_down_button_right = Add_Button_Field(299, 139, "", citylist_down_button_seg, emptystring__ovr065, ST_UNDEFINED);
        }
        else
        {
            // TODO  _help_entries+0Ah = ST_UNDEFINED
            // TODO  _help_entries+1Eh = ST_UNDEFINED

            hotkey_D = INVALID_FIELD;
            citylist_down_button_left = INVALID_FIELD;
            citylist_down_button_right = INVALID_FIELD;
        }


        input_field_idx = Get_Input();

        scanned_field = Scan_Input();

        /*
            Leave Screen
        */
        if((input_field_idx == ST_UNDEFINED) || (input_field_idx == button_citylist_ok) || (input_field_idx == hotkey_ESC))
        {
            // Play_Left_Click();
            leave_screen_flag = ST_TRUE;
            current_screen = scr_Main_Screen;
        }

        if((input_field_idx == citylist_up_button_left) || (input_field_idx == citylist_up_button_right) || (input_field_idx == hotkey_U))
        {
            Play_Left_Click();
            if(list_first_item != 0)
            {
                if((list_first_item - NUM_CITY_LIST_SCROLL) < 0)
                {
                    list_first_item = 0;
                }
                else
                {
                    list_first_item -= NUM_CITY_LIST_SCROLL;
                }
            }
            Update_Cities_List();
            screen_changed = ST_TRUE;
        }

        if((input_field_idx == citylist_down_button_left) || (input_field_idx == citylist_down_button_right) || (input_field_idx == hotkey_D))
        {
            Play_Left_Click();
            if((list_first_item + NUM_CITY_LIST) < m_city_list_count)
            {
                if((list_first_item + NUM_CITY_LIST + NUM_CITY_LIST_SCROLL) < m_city_list_count)
                {
                    list_first_item += NUM_CITY_LIST_SCROLL;
                }
                else
                {
                    list_first_item += 1;
                }
            }
            Update_Cities_List();
            screen_changed = ST_TRUE;
        }

        for(itr = 0; itr < m_cities_list_field_count; itr++)
        {
            if(m_cities_list_fields[itr] == input_field_idx)
            {
                // Play_Left_Click();
                _city_idx = list_cities[itr];
                if(_CITIES[_city_idx].size == 0)
                {
                    _map_plane = _CITIES[_city_idx].wp;
                    Center_Map(&_map_x, &_map_y, _CITIES[_city_idx].wx, _CITIES[_city_idx].wy, _CITIES[_city_idx].wp);
                    current_screen = scr_Main_Screen;
                }
                else
                {
                    _map_plane = _CITIES[_city_idx].wp;
                    Center_Map(&_map_x, &_map_y, _CITIES[_city_idx].wx, _CITIES[_city_idx].wy, _CITIES[_city_idx].wp);
                    current_screen = scr_City_Screen;
                }
                leave_screen_flag = ST_TRUE;
            }
        }

        for(itr = 0; itr < m_cities_list_field_count; itr++)
        {
            if(-(m_cities_list_fields[itr]) == input_field_idx)
            {
                // Play_Left_Click();
                m_citylist_first_item = list_first_item;
                _city_idx = list_cities[itr];
                if(_CITIES[_city_idx].size == 0)
                {
                    // ; displays and processes the outpost screen - if the view type is 1, the header calls for naming the new outpost and does not display the units on the tile
                    // ; BUG: fails to draw the altered backgrounds of the Gaia's Blessing, Flying Fortress, Famine, and Cursed Lands enchantments
                    // TODO  Outpost_Screen(0, -1, -1);
                    Assign_Auto_Function(CityList_Screen_Draw, 1);
                    CityList_Screen_Load();
                    Build_City_List();
                    Army_At_City(list_first_item, &troop_count, &troops[0]);
                    CityList_Draw_Reduced_Map();
                    screen_changed = ST_TRUE;
                }
                else
                {
                    current_screen = scr_Production_Screen;
                    production_screen_return_screen = 1;  // {1: CityList Screen, 2: City Screen}
                    leave_screen_flag = ST_TRUE;
                }
                
            }
        }

        for(itr = 0; itr < m_cities_list_field_count; itr++)
        {
            if(m_cities_list_fields[itr] == scanned_field)
            {
                citylist_item_scanned_field = itr;
            }
        }

        if((leave_screen_flag == ST_FALSE) && (screen_changed == ST_FALSE))
        {
            CityList_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
        screen_changed = ST_FALSE;
    }

    Deactivate_Help_List();
    Deactivate_Auto_Function();
    Reset_Window();
    Clear_Fields();
    Clear_Palette_Changes(0, 255);
    Set_Palette_Changes(0, 223);
    Update_Remap_Gray_Palette();

}


// WZD o65p02
void CityList_Draw_Reduced_Map(void)
{
    int16_t city_world_x;
    int16_t city_world_y;
    int16_t city_world_p;

    city_world_x = _CITIES[list_cities[citylist_item_scanned_field]].wx;
    city_world_y = _CITIES[list_cities[citylist_item_scanned_field]].wy;
    city_world_p = _CITIES[list_cities[citylist_item_scanned_field]].wp;

    List_Screen_Draw_Reduced_Map(CITYLIST_REDUCED_MAP_X, CITYLIST_REDUCED_MAP_Y, CITYLIST_REDUCED_MAP_W, CITYLIST_REDUCED_MAP_H, city_world_p, city_world_x, city_world_y);

}




// WZD o65p03
void CityList_Screen_Load(void)
{

    m_city_list_array = Near_Allocate_First(200);  // 100 2-byte ints  ~== sizeof(int) * CITY_COUNT_MAX

    list_cities = Near_Allocate_Next(18);  // ¿ 9 2-byte ints, for City List of 9 ?

    GUI_String_1 = (char *)Near_Allocate_Next(80);

    city_enchantment_list        = (int16_t *)Near_Allocate_Next(52);  // ¿ 26 2-byte ints ?
    city_enchantment_owner_list  = (int16_t *)Near_Allocate_Next(52);  // ¿ 26 2-byte ints ?

    _reduced_map_seg = Allocate_First_Block(_screen_seg, 153);

    // RELOAD.LBX, 021  CITYLIST
    // RELOAD.LBX, 022  CITYLBUT

    citylist_background_seg  = LBX_Reload_Next("RELOAD",  21, _screen_seg);
    citylist_ok_button_seg   = LBX_Reload_Next("RELOAD",  22, _screen_seg);

    // ARMYLIST.LBX, 001  ARMYLBUT   armylist up but
    // ARMYLIST.LBX, 002  ARMYLBUT   armylist down but

    citylist_up_button_seg   = LBX_Reload_Next("ARMYLIST", 1, _screen_seg);
    citylist_down_button_seg = LBX_Reload_Next("ARMYLIST", 2, _screen_seg);

}


// WZD o65p04
void CityList_Screen_Draw(void)
{
    uint8_t colors1[6];
    uint8_t colors2[6];
    int16_t production_time;
    int16_t production_cost;
    int16_t city_construction;
    int16_t unit_type;
    int16_t city_idx;

    int16_t itr_colors;  // _SI_
    int16_t itr_list_item_count;
    int16_t y_offset;  // _DI_

    CityList_Set_List_Item_Count();

    Set_Page_Off();
    
    FLIC_Draw(0, 0, citylist_background_seg);

    
    /*
        Up Arrows
            do not draw
            do draw - enabled
            do draw - disabled
    */
    if(list_first_item == 0 && m_city_list_count > NUM_CITY_LIST)
    {
        Set_Animation_Frame(citylist_up_button_seg, 1);
        FLIC_Draw(11, 26, citylist_up_button_seg);
        Set_Animation_Frame(citylist_up_button_seg, 1);
        FLIC_Draw(299, 26, citylist_up_button_seg);
    }
    else if(m_city_list_count > NUM_CITY_LIST)
    {
        Set_Animation_Frame(citylist_up_button_seg, 0);
        FLIC_Draw(11, 26, citylist_up_button_seg);
        Set_Animation_Frame(citylist_up_button_seg, 0);
        FLIC_Draw(299, 26, citylist_up_button_seg);
    }

    /*
        Down Arrows
    */
    if(
        (m_city_list_count > NUM_CITY_LIST)
        &&
        (
            ((list_first_item + NUM_CITY_LIST) == m_city_list_count)
            ||
            (list_item_count == m_city_list_count)
        )
    )
    {
        Set_Animation_Frame(citylist_down_button_seg, 1);
        FLIC_Draw(11, 139, citylist_down_button_seg);
        Set_Animation_Frame(citylist_down_button_seg, 1);
        FLIC_Draw(299, 139, citylist_down_button_seg);
    }
    else if(m_city_list_count > NUM_CITY_LIST)
    {
        Set_Animation_Frame(citylist_down_button_seg, 0);
        FLIC_Draw(11, 139, citylist_down_button_seg);
        Set_Animation_Frame(citylist_down_button_seg, 0);
        FLIC_Draw(299, 139, citylist_down_button_seg);
    }


    // BEGIN: Print Title

    strcpy(GUI_String_1, aTheCitiesOf);
    strcat(GUI_String_1, _players[_human_player_idx].name);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 237;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(162, 5, GUI_String_1);
    Print_Centered(161, 5, GUI_String_1);
    Print_Centered(162, 4, GUI_String_1);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 238;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(161, 3, GUI_String_1);
    Print_Centered(160, 4, GUI_String_1);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 239;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(161, 4, GUI_String_1);
    colors2[0] = 236;
    colors2[1] = 177;
    colors2[2] = 178;
    colors2[3] = 177;
    colors2[4] = 176;
    Set_Font_Colors_15(4, &colors2[0]);
    Print_Centered(160, 3, GUI_String_1);

    // END: Print Title


    Set_Outline_Color(231);
    colors1[0] = 236;
    colors1[1] = 129;
    colors1[2] = 129;
    Set_Font_Colors_15(1, &colors1[0]);  // ¿ sets font style 1 and font remap colors block ?
    Set_Font_Style_Shadow_Down(1, 15, 0, 0);  // shadow - bottom-right, 1 pixel; use special/custom colors font color block

    Print( 32, 17, aName);        /* "Name"      */
    Print( 88, 17, aRace);        /* "Race"      */
    Print(136, 17, aPop);         /* "Pop"       */
    Print(154, 17, cnst_Gold_3);  /* "Gold"      */
    Print(176, 17, aPrd);         /* "Prd"       */
    Print(197, 17, aProducing);   /* "Producing" */
    Print(272, 17, aTime);        /* "Time"      */

    SDL_itoa(_players[_human_player_idx].gold_reserve, GUI_String_1, 10);
    strcat(GUI_String_1, cnst_GP_2);
    Print_Centered(242, 172, GUI_String_1);

    SDL_itoa(_players[_human_player_idx].mana_reserve, GUI_String_1, 10);
    strcat(GUI_String_1, cnst_MP_2);
    Print_Centered(276, 172, GUI_String_1);


    Set_Font_Spacing_Width(1);

    /*
        BEGIN: City Summary
    */
    for(itr_list_item_count = 0; itr_list_item_count < list_item_count; itr_list_item_count++)
    {
        city_idx = list_cities[itr_list_item_count];

        // BUGBUG
        if(itr_list_item_count == citylist_item_scanned_field)
        {
            // jmp     short $+2
        }

        Set_Outline_Color(231);

        if(itr_list_item_count == citylist_item_scanned_field)
        {
            UU_CityListDraw_ScannedHighlight = 0;  // BUGBUG

            y_offset = 25 + (14 * itr_list_item_count);
            Gradient_Fill( 30, (y_offset + 1),  81, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);  // Name
            Gradient_Fill( 86, (y_offset + 1), 130, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);  // Race
            Gradient_Fill(135, (y_offset + 1), 150, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);  // Pop
            Gradient_Fill(155, (y_offset + 1), 170, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);  // Gold
            Gradient_Fill(175, (y_offset + 1), 190, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);  // Prd
            Gradient_Fill(195, (y_offset + 1), 270, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);  // Producing
            Gradient_Fill(275, (y_offset + 1), 289, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);  // Time
        }

        // TODO  Print_Far()
        Print(31, (27 + (14 * itr_list_item_count)), _CITIES[city_idx].name);
        Print(87, (27 + (14 * itr_list_item_count)), *_race_type_table[_CITIES[city_idx].race].name);
        Print_Integer_Right(148, (27 + (14 * itr_list_item_count)), _CITIES[city_idx].population);
        Print_Integer_Right(168, (27 + (14 * itr_list_item_count)), (_CITIES[city_idx].gold_units - _CITIES[city_idx].building_maintenance));
        Print_Integer_Right(188, (27 + (14 * itr_list_item_count)), _CITIES[city_idx].production_units);

        city_construction = _CITIES[city_idx].construction;
        if(city_construction < 100)  /* building */
        {
            // TODO  _fstrcpy()
            strcpy(GUI_String_1, bldg_data_table[city_construction].name);
        }
        else  /* unit */
        {
            unit_type = (city_construction - 100);
            strcpy(GUI_String_1, *_unit_type_table[unit_type].name);
        }
        Print(196, (27 + 14 * itr_list_item_count), GUI_String_1);

        production_cost = City_Production_Cost(city_construction, city_idx);
        production_time = City_N_Turns_To_Produce(production_cost, city_idx);
        Print_Integer_Right(287, (27 + (14 * itr_list_item_count)), production_time);
    }
    /*
        END: City Summary
    */


    if(m_city_list_count > 0)
    {
        city_idx = list_cities[citylist_item_scanned_field];
        Set_Outline_Color(231);
        colors1[0] = 236;
        colors1[1] = 129;
        colors1[2] = 129;
        Set_Font_Colors_15(1, &colors1[0]);
        Set_Font_Style_Shadow_Down(1, 15, 0, 0);
        // TODO  _fstrcpy()
        strcpy(GUI_String_1, _CITIES[city_idx].name);
        String_To_Upper(GUI_String_1);
        Print(99, 158, GUI_String_1);

        Build_City_Enchantment_List(city_idx, city_enchantment_list, city_enchantment_owner_list, &city_enchantment_list_count);

        SETMAX(city_enchantment_list_count, 8);

        Print_City_Enchantment_List(100, 168, city_enchantment_list, city_enchantment_owner_list, city_enchantment_list_count);

        CityList_Draw_Reduced_Map();
    }

}


// WZD o65p05
// ~ MoO2 Build_Global_Colony_List_()
// MoO2  N_Colonies_()
/*
    populates m_city_list_array[]

*/
void Build_City_List(void)
{
    int16_t itr_cities;

    m_city_list_count = 0;  /* MoO2  DNE, always does 250 - max colonies */

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)  /* MoO2  _NUM_COLONIES ~== _cities */
    {
        if(_CITIES[itr_cities].owner_idx == _human_player_idx)  /* MoO2  _PLAYER_NUM ~== _human_player_idx */
        {
            m_city_list_array[m_city_list_count] = itr_cities;  /* MoO2  _g_colony_list_ptr -> l_colony_list[250] */

            m_city_list_count++;

            if(_CITIES[itr_cities].construction == bt_NONE)
            {
                _CITIES[itr_cities].construction = bt_Housing;
            }
        }
    }
}


// WZD o65p06
// ~ MoO2  Update_Col_List_() ... sets _list_col from _g_colony_list_ptr, based on _first + itr
/*
    populates list_cities[]
    sets the list of Cities to display, from list_first_item to list_item_count
*/
void Update_Cities_List(void)
{
    int16_t itr_list_items;

    CityList_Set_List_Item_Count();

    for(itr_list_items = 0; itr_list_items < list_item_count; itr_list_items++)
    {
        list_cities[itr_list_items] = m_city_list_array[(list_first_item + itr_list_items)];
    }

}


// WZD o65p07
// MoO2  Module: COLSUM  Add_Fields_()
void CityList_Add_List_Fields(void)
{
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
    int16_t itr;

    m_cities_list_field_count = 0;

    CityList_Set_List_Item_Count();

    for(itr = 0; itr < list_item_count; itr++)
    {
        x1 = 30;
        y1 = (26 + (14 * itr));  /* cities list row height */
        x2 = x1 + 259;  /* cities list line  width */
        y2 = y1 + 8;  /* cities list line height */
        m_cities_list_fields[m_cities_list_field_count] = Add_Hidden_Field(x1, y1, x2, y2, emptystring__ovr065, ST_UNDEFINED);

        m_cities_list_field_count++;
    }

}


// WZD o65p08
// ~== ArmyList_Set_List_Item_Count()
// ~ MoO2  Update_Col_List_()
/*
    sets list item count to min of 9 or city count
*/
void CityList_Set_List_Item_Count(void)
{
    list_item_count = NUM_CITY_LIST + list_first_item;
    SETMAX(list_item_count, NUM_CITY_LIST);
    SETMAX(list_item_count, m_city_list_count);
}
