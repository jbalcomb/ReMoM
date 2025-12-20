/*
    Hire - Merchant

    WIZARDS.EXE
        ovr127
*/

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */
#include "../../MoX/src/LBX_Load.h"

#include "CITYCALC.h"
#include "Combat.h"
#include "HIRE.h"
#include "Items.h"
#include "ItemScrn.h"
#include "ItemView.h"
#include "LOADER.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/paragrph.h"
#include "MainScr.h"
#include "MainScr_Maps.h"
#include "MOM_SCR.h"
#include "NEXTTURN.h"
#include "SBookScr.h"
#include "Spellbook.h"
#include "Spells132.h"
#include "UnitStat.h"
#include "UNITTYPE.h"   // WTFMATE
#include "UnitView.h"

#include <stdlib.h>
#include <string.h>

#include <SDL_stdinc.h>



// WZD dseg:953C

/*
in MoX_DATA, may be should be in ~City

LOADER.C  City_Screen_Load_Pictures()
// BACKGRND.LBX, 33  OUTNAME  outpost name backg
outpost_name_background_seg = LBX_Reload_Next(backgrnd_lbx_file, 33, GFX_Swap_Seg);
*/
extern SAMB_ptr outpost_name_background_seg;

// ovr115 'Item Make'
// WZD dseg:C8DC
extern SAMB_ptr m_itemmake_item_icon_bitmap_seg;



// WZD dseg:6504                                                 BEGIN:  ovr127 - Initialized Data

// WZD dseg:6504
char hire_lbx_file__ovr127[] = "HIRE";
// WZD dseg:6508
char empty_string__ovr127[] = "";
// WZD dseg:6509
char cnst_Merchant_Msg_1[] = "A merchant arrives and offers a magic ";
// WZD dseg:6530
char cnst_Merchant_Msg_2[] = " for sale.  The price is only ";
// WZD dseg:654F
char cnst_Merchant_Msg_3[] = " gold pieces.";
// WZD dseg:655D
char cnst_Merchant_Msg_4[] = "Buy";
// WZD dseg:6561
char cnst_Merchant_Msg_5[] = "Reject";
// WZD dseg:6568
char names_lbx_file__ovr127[] = "NAMES";
// WZD dseg:656E
char cnst_Hire_Msg_1[] = "Hero for Hire: ";
// WZD dseg:657E
char cnst_Hire_Msg_2[] = " gold";
// WZD dseg:6584
char cnst_Hire_Msg_3[] = "Hero Summoned";
// WZD dseg:6592
char cnst_Hire_Msg_4[] = "Champion Summoned";
// WZD dseg:65A4
char cnst_Hire_Msg_5[] = "A prisoner asks to join your army";
// WZD dseg:65C6
char cnst_Hire_Msg_6[] = "Name Your Hero";
// WZD dseg:65D5
char cnst_Hire_Msg_7[] = "Hire";
// WZD dseg:65DA
char cnst_Hire_Msg_8[] = "Accept";
// WZD dseg:65E1
char str_hotkey_H__ovr127[] = "H";
// WZD dseg:65E3
char str_hotkey_A__ovr127[] = "A";
// WZD dseg:65E5
char hlpentry_lbx_file__ovr127[] = "hlpentry";
// WZD dseg:65EE
char cnst_Hire_Msg_9[] = "Mercenary for Hire: ";
// WZD dseg:6603
char cnst_Hire_Msg_A[] = " Mercenaries for Hire: ";

// WZD dseg:661B 00                                              align 2

// WZD dseg:661B                                                 END:  ovr127 - Strings



// WZD dseg:C9DE                                                 ¿ BEGIN:  ovr127 - Uninitialized Data ?

// WZD dseg:C9DE
char * hirehero_unit_type_name;

// WZD dseg:C9E0
// drake178: multi-use variable!
/*
Usage               Location
    Item Index          Determine_Offer()

*/
int16_t GUI_InHeroNaming;

// WZD dseg:C9E2
int16_t GAME_HeroHireType;  // ; 0: random, 1: summon, 2: prisoner, 3: champion

// WZD dseg:C9E4
SAMB_ptr hire_namebox_seg;

// WZD dseg:C9E6
SAMB_ptr IMG_MerchantBtns;

// WZD dseg:C9E8
SAMB_ptr hire_banner_seg;

// WZD dseg:C9EA
int16_t EVNT_MercUnitCount;

// WZD dseg:C9EC
int16_t CRP_EVNT_MercUnitType;

// WZD dseg:C9EE
int16_t GAME_AssetCost;

// WZD dseg:C9EE                                                 ¿ END:  ovr127 - Uninitialized Data ?



/*
    WIZARDS.EXE  ovr127
*/

// WZD s127p01
// drake178: EVNT_LoadMerchantWnd()
/*
; redraws the overland map, saves it to the third VGA
; frame, and loads the images for showing the merchant
; window
;
; WARNING: reuses variables defined elsewhere
*/
/*

*/
void Merchant_Popup_Load(void)
{

    Allocate_Reduced_Map();
    Set_Page_Off();
    Main_Screen_Draw();
    Copy_Off_To_Back();
    Set_Page_On();

    // HIRE.LBX, 002  MERCHBAK
    hire_banner_seg = LBX_Reload_Next(hire_lbx_file__ovr127, 2, _screen_seg);

    // HIRE.LBX, 001  REDBUTT
    IMG_MerchantBtns = LBX_Reload_Next(hire_lbx_file__ovr127, 1, _screen_seg);

    m_itemmake_item_icon_bitmap_seg = Allocate_Next_Block(_screen_seg, 1200);

    IMG_SBK_PageText = Allocate_Next_Block(_screen_seg, 30);

}


// WZD s127p02
// drake178: EVNT_DrawMerchantWnd()
void Merchant_Popup_Draw(void)
{
    char item_name[LEN_ITEM_NAME];
    int16_t screen_x;  // _SI_
    int16_t screen_y;  // _DI_

    screen_x = 5;
    screen_y = 15;

    Cycle_Item_Enchantment_Animation();

    Item_View_Prepare(GUI_InHeroNaming, m_itemmake_item_icon_bitmap_seg, IMG_SBK_PageText);

    Copy_Back_To_Off();

    FLIC_Draw(screen_x, screen_y, hire_banner_seg);

    strcpy(GUI_NearMsgString, cnst_Merchant_Msg_1);

    strcpy(item_name, _ITEMS[GUI_InHeroNaming].name);

    strcat(GUI_NearMsgString, item_name);

    strcat(GUI_NearMsgString, cnst_Merchant_Msg_2);

    SDL_itoa(GAME_AssetCost, item_name, 10);

    strcat(GUI_NearMsgString, item_name);

    strcat(GUI_NearMsgString, cnst_Merchant_Msg_3);

    Set_Alias_Color(163);

    Set_Font_Style_Shadow_Down(4, 4, 0, 0);

    Set_Outline_Color(253);

    Print_Paragraph((screen_x + 57), (screen_y + 9), 182, GUI_NearMsgString, 0);

    Draw_Picture((screen_x + 14), (screen_y + 65), m_itemmake_item_icon_bitmap_seg);

}


// WZD s127p03
// drake178: EVNT_MerchantDialog()
void Merchant_Popup(void)
{
    int16_t merchant_popup_y_start;
    int16_t merchant_popup_x_start;
    int16_t merchant_buy_button;
    int16_t merchant_reject_button;
    int16_t input_field_idx;  // _DI_
    int16_t leave_screen;  // _SI_

    OVL_DisableIncmBlink();

    Set_Button_Down_Offsets(1, 1);

    Merchant_Popup_Load();

    merchant_popup_x_start = 5;
    merchant_popup_y_start = 15;

    Clear_Fields();

    GAME_AssetCost = _ITEMS[GUI_InHeroNaming].cost;

    if(_players[HUMAN_PLAYER_IDX].charismatic > 0)
    {
        GAME_AssetCost /= 2;
    }

    Set_Font_Style(4, 4, 0, 0);

    merchant_buy_button = Add_Button_Field((merchant_popup_x_start + 252), (merchant_popup_y_start + 121), cnst_Merchant_Msg_4, IMG_MerchantBtns, empty_string__ovr127[0], ST_UNDEFINED);

    merchant_reject_button = Add_Button_Field((merchant_popup_x_start + 252), (merchant_popup_y_start + 140), cnst_Merchant_Msg_5, IMG_MerchantBtns, ST_UNDEFINED, ST_UNDEFINED);

    Assign_Auto_Function(Merchant_Popup_Draw, 2);

    leave_screen = ST_FALSE;

    Set_Input_Delay(2);

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();

        if(input_field_idx == merchant_reject_button)
        {
            Remove_Item(GUI_InHeroNaming);
            leave_screen = ST_TRUE;
        }

        if(input_field_idx == merchant_buy_button)
        {

            _players[HUMAN_PLAYER_IDX].gold_reserve -= GAME_AssetCost;

            OVL_MosaicFlip__STUB();

            Process_Item_Pool(1, &GUI_InHeroNaming);

            current_screen = scr_Main_Screen;

            leave_screen = ST_TRUE;
        }

        if(leave_screen == ST_FALSE)
        {
            Set_Page_Off();
            Merchant_Popup_Draw();
            PageFlip_FX();
        }
    }

    Release_Block(_screen_seg);

    Clear_Fields();

    Deactivate_Auto_Function();

}


// WZD s127p04
void Hire_Hero_Load(int16_t unit_type)
{

    Set_Mouse_List(1, mouse_list_default);

    Reset_First_Block(_screen_seg);

    Near_Allocate_Reset();  // NOTE  ; discards the contents of the LBX near memory buffer

    Unit_View_Allocate();

    _reduced_map_seg = Allocate_Next_Block(_screen_seg, 303);

    // HIRE.LBX, 4  000  BANNER
    hire_banner_seg = LBX_Reload_Next(hire_lbx_file__ovr127, 0, _screen_seg);

    // BUG  ¿ ; should use the already allocated USW_ItemDraw_Seg ?
    m_itemmake_item_icon_bitmap_seg = Allocate_Next_Block(_screen_seg, 30);  // 30 PR, 480 B

    // BUG  ¿ ; asset never used ?
    // HIRE.LBX, 4  004  NAMEBOX
    hire_namebox_seg = LBX_Reload_Next(hire_lbx_file__ovr127, 4, _screen_seg);

    hirehero_unit_type_name = (char *)Near_Allocate_Next(13);

    // BUG  ~macro for unit figure or hero portrait, but only loading hero name records, and this whole function/popup is only for heros
    if(unit_type > ut_Chosen)
    {
        Load_Unit_Figure(unit_type, 1);
    }
    else
    {
        LBX_Load_Data_Static(names_lbx_file__ovr127, 0, (SAMB_ptr)hirehero_unit_type_name, unit_type, 1, 13);
    }

}


// WZD s127p05
/*

; redraws the hero hiring screen onto a background
; loaded from the third VGA frame, drawing the naming
; prompt and side buttons only if necessary
;
; BUG: uses the wrong item draw segment (double alloc)

*/
void Hire_Hero_Popup_Draw(void)
{
    char temp_string[10];
    int16_t window_x;  // _SI_
    int16_t window_y;  // _DI_

    window_x = 25;
    window_y = 17;

    Copy_Back_To_Off();

    Thing_View_Draw__WIP(window_x, window_y, 1, (_units - 1), uv_specials_list_array, uv_specials_list_count, m_itemmake_item_icon_bitmap_seg);

    FLIC_Draw((window_x - 25), 0, hire_banner_seg);

    Set_Alias_Color(163);

    Set_Font_Style(4, 4, 4, 4);

    if(GAME_HeroHireType == 0)  // ; 0: random, 1: summon, 2: prisoner, 3: champion
    {
        strcpy(GUI_NearMsgString, cnst_Hire_Msg_1);
        SDL_itoa(GAME_AssetCost, temp_string, 10);
        strcat(GUI_NearMsgString, temp_string);
        strcat(GUI_NearMsgString, cnst_Hire_Msg_2);
    }
    else if(GAME_HeroHireType == 1)
    {
        strcpy(GUI_NearMsgString, cnst_Hire_Msg_3);
    }
    else if(GAME_HeroHireType == 3)
    {
        strcpy(GUI_NearMsgString, cnst_Hire_Msg_4);
        
    }
    else
    {
        strcpy(GUI_NearMsgString, cnst_Hire_Msg_5);
    }

    Print_Centered((window_x + 103), (window_y - 11), GUI_NearMsgString);

    if(GUI_InHeroNaming == ST_TRUE)
    {
        Set_Font_Style(4, 4, 4, 4);
        // BACKGRND.LBX, 33  OUTNAME  outpost name backg
        outpost_name_background_seg = LBX_Reload_Next("BACKGRND", 33, g_graphics_cache_seg);
        FLIC_Draw((window_x + 29), (window_y + 55), outpost_name_background_seg);
        Print_Centered((window_x + 104), (window_y + 61), cnst_Hire_Msg_6);
        Reset_Animation_Frame(red_button_seg);
        FLIC_Draw((window_x + 221), (window_y + 143), red_button_seg);
        Reset_Animation_Frame(red_button_seg);
        FLIC_Draw((window_x + 221), (window_y + 162), red_button_seg);
        if(GAME_HeroHireType == 0)  // ; 0: random, 1: summon, 2: prisoner, 3: champion
        {
            Print_Centered((window_x + 246), (window_y + 144), cnst_Hire_Msg_7);  // "Hire"
        }
        else
        {
            Print_Centered((window_x + 246), (window_y + 144), cnst_Hire_Msg_8);  // "Accept"
        }
    }

}


// WZD s127p06
/*

; displays and processes the hero hiring screen,
; including the naming dialog
;
; BUG: can push a unit out of the capital when a
;  prisoner is found
; also has some inherited BUGs, and handles the USW not
;  completely through its own functions - RE-EXPLORE!

*/
int16_t Hire_Hero_Popup(int16_t hero_slot_idx, int16_t unit_type_idx, int16_t hire_type)
{
    uint8_t Name_Edit_Colors[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Hero_Hired = 0;
    int16_t Hire_Success = 0;
    int16_t window_y = 0;
    int16_t hire_button_field = 0;
    int16_t reject_button_field = 0;
    int16_t itr = 0;
    int16_t leave_screen = 0;
    int16_t input_field_idx = 0;
    int16_t window_x = 0;  // _DI_

    for(itr = 0; itr < 20; itr++)
    {

        Name_Edit_Colors[itr] = (176 + (itr % 10));

    }

    Hero_Hired = ST_FALSE;

    GUI_InHeroNaming = ST_FALSE;

    GAME_HeroHireType = hire_type;  // ; 0: random, 1: summon, 2: prisoner, 3: champion

    OVL_DisableIncmBlink();

    Allocate_Reduced_Map();

    Set_Page_Off();

    Reset_Map_Draw();

    Main_Screen_Draw();

    Copy_Off_To_Back();

    Set_Page_On();

    Cache_Graphics_Things();

    Hire_Hero_Load(unit_type_idx);

    Assign_Auto_Function(Hire_Hero_Popup_Draw, 2);

    Set_Button_Down_Offsets(1, 1);

    _page_flip_effect = pfe_None;

    window_x = 25;
    window_y = 17;

    strcpy(_players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].name, hirehero_unit_type_name);

    Hire_Success = WIZ_HireHero(HUMAN_PLAYER_IDX, unit_type_idx, hero_slot_idx, ST_FALSE);

    // ; conflicting condition - will always jump
    if(Hire_Success == ST_FALSE)
    {

        Cache_Graphics_Overland();

        return ST_FALSE;

    }

    GAME_AssetCost = (_unit_type_table[unit_type_idx].cost + ((_UNITS[(_units - 1)].Level * _unit_type_table[unit_type_idx].cost) / 4));

    if(
        (_players[HUMAN_PLAYER_IDX].gold_reserve < GAME_AssetCost)
        &&
        (hire_type == 0)
    )
    {

        if(_UNITS[(_units - 1)].Level < 4)
        {

            _UNITS[(_units - 1)].Level += 1;

            _UNITS[(_units - 1)].XP = TBL_Experience[_UNITS[(_units - 1)].Level];

        }

        Kill_Unit((_units - 1), kt_Dismissed);

        Cache_Graphics_Overland();

        return ST_FALSE;

    }

    if(_players[HUMAN_PLAYER_IDX].charismatic > 0)
    {

        GAME_AssetCost /= 2;  // 50%

    }

    Load_Battle_Unit((_units - 1), global_battle_unit);

    USW_Build_Effect_List((_units - 1), &uv_specials_list_array[0], &uv_specials_list_count);

    UV_Setup_HireScr((_units - 1), uv_specials_list_count);

    Clear_Fields();

    Set_Font_Style(4, 4, 0, 0);

    if(GAME_HeroHireType == 0)  // ; 0: random, 1: summon, 2: prisoner, 3: champion
    {

        // ¿ "[H]ire" ?
        hire_button_field = Add_Button_Field((window_x + 221), (window_y + 143), cnst_Hire_Msg_7, red_button_seg, str_hotkey_H__ovr127[0], ST_UNDEFINED);

    }
    else
    {

        // ¿ "[A]ccept" ?
        hire_button_field = Add_Button_Field((window_x + 221), (window_y + 143), cnst_Hire_Msg_8, red_button_seg, str_hotkey_A__ovr127[0], ST_UNDEFINED);

    }

    reject_button_field = Add_Button_Field((window_x + 221), (window_y + 162), cnst_Merchant_Msg_5, red_button_seg, ST_UNDEFINED, ST_UNDEFINED);

    UV_Add_Specials_Fields((window_x + 8), (window_y + 108), uv_specials_list_array, uv_specials_list_count, uv_specials_list_index);

    Set_Font_Style(0, 0, 2, 0);

    Set_Page_Off();

    Hire_Hero_Popup_Draw();

    Draw_Fields();

    Apply_Palette();

    VGA_MosaicFlip__STUB();

    // ~== Set_Hire_Hero_Popup_Help_List()
    LBX_Load_Data_Static(hlpentry_lbx_file__ovr127, 23, (SAMB_ptr)_help_entries, 0, 23, 10);

    Set_Help_List((char *)_help_entries, 23);

    leave_screen = ST_FALSE;

    Set_Input_Delay(2);

    while(leave_screen == ST_FALSE)
    {

        UV_Set_Specials_Help_Fields(uv_specials_list_array, uv_specials_list_count, 0);

        Mark_Time();

        input_field_idx = Get_Input();

        UV_Handle_Arrow_Buttons(input_field_idx, &itr);

        /*
            BEGIN:  Left-Click Reject Button
        */
        if(input_field_idx == reject_button_field)
        {

            leave_screen = ST_TRUE;

            Play_Left_Click__DUPE();

            if(_UNITS[(_units - 1)].Level < 4)
            {

                _UNITS[(_units - 1)].Level += 1;

                _UNITS[(_units - 1)].XP = TBL_Experience[_UNITS[(_units - 1)].Level];

            }
            
            Kill_Unit((_units - 1), 1);

        }
        /*
            END:  Left-Click Reject Button
        */

        /*
            BEGIN:  Left-Click Hire / Accept Button
        */
        if(input_field_idx == hire_button_field)
        {

            Play_Left_Click();

            GUI_InHeroNaming = ST_TRUE;

            _HEROES2[HUMAN_PLAYER_IDX]->heroes[unit_type_idx].Level = -20;

            if(GAME_HeroHireType == 0)
            {

                _players[HUMAN_PLAYER_IDX].gold_reserve -= GAME_AssetCost;

            }
            
            // DEDU  Is this for real? Who would ever and/or why?
            if((GAME_HeroHireType % 2) != 0)  // ~== 1 or 3  Summon Hero or Champion
            {

                _UNITS[(_units - 1)].wx = _players[HUMAN_PLAYER_IDX].summon_wx;

                _UNITS[(_units - 1)].wy = _players[HUMAN_PLAYER_IDX].summon_wy;

                _UNITS[(_units - 1)].wp = _players[HUMAN_PLAYER_IDX].summon_wp;

            }

            // ; BUG: can push a unit out of the capital when finding a prisoner
            UNIT_RemoveExcess((_units - 1));

            Hero_Hired = ST_TRUE;

            leave_screen = ST_TRUE;

        }
        /*
            END:  Left-Click Hire / Accept Button
        */

        Clear_Fields_Above(reject_button_field);

        UV_Add_Specials_Fields((window_x + 8), (window_y + 108), uv_specials_list_array, uv_specials_list_count, uv_specials_list_index);

        if(leave_screen == ST_FALSE)
        {

            Set_Page_Off();

            Hire_Hero_Popup_Draw();

            PageFlip_FX();

            Release_Time(1);

        }

    }

    if(Hero_Hired == ST_TRUE)
    {

        Set_Font_Style(4, 4, 4, 4);

        // // int16_t Setup_Input_Box_Popup(int16_t x_start, int16_t y_start, int16_t width, char * string, int16_t max_characters, int16_t fill_color, int16_t justification, int16_t cursor_type, uint8_t colors[], int16_t help)
        // Setup_Input_Box_Popup((window_x + 45), (window_y + 75), 120, _players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].name, (LEN_HERO_NAME - 1), 0, 0, 0, &Name_Edit_Colors[0], empty_string__ovr127[0], ST_UNDEFINED);
        Setup_Input_Box_Popup((window_x + 45), (window_y + 75), 120, _players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].name, (LEN_HERO_NAME - 1), 0, 0, 0, &Name_Edit_Colors[0], empty_string__ovr127[0]);

        strcpy(hero_names_table[unit_type_idx].name, _players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].name);

    }

    Clear_Fields();

    Deactivate_Auto_Function();

    Cache_Graphics_Overland();

    Full_Draw_Main_Screen();

    OVL_EnableIncmBlink();

    Deactivate_Help_List();

    return Hero_Hired;

}


// WZD s127p07
// drake178: EVNT_DrawMercHire()
void Hire_Merc_Popup_Draw(void)
{

    char temp_string[10];
    int16_t window_x;  // _SI_
    int16_t window_y;  // _DI_

    window_x = 25;
    window_y = 17;

    Copy_Back_To_Off();

    Thing_View_Draw__WIP(window_x, window_y, 1, (_units - 1), uv_specials_list_array, uv_specials_list_count, m_itemmake_item_icon_bitmap_seg);

    FLIC_Draw((window_x - 25), 0, hire_banner_seg);

    Set_Alias_Color(163);

    Set_Font_Style(4, 4, 4, 4);

    if(EVNT_MercUnitCount == 1)
    {
        strcpy(GUI_NearMsgString, cnst_Hire_Msg_9);
    }
    else
    {
        SDL_itoa(EVNT_MercUnitCount, temp_string, 10);
        strcpy(GUI_NearMsgString, temp_string);
        strcat(GUI_NearMsgString, cnst_Hire_Msg_A);
    }

    SDL_itoa(GAME_AssetCost, temp_string, 10);

    strcat(GUI_NearMsgString, temp_string);

    strcat(GUI_NearMsgString, cnst_Hire_Msg_2);

    Print_Centered((window_x + 103), (window_y - 11), GUI_NearMsgString);

}


// WZD s127p08
// drake178: EVNT_MercHireDialog()
/*
; displays and processes the mercenary hire dialog
; returns 1 if the mercenaries are hired, or 0 if not
;
; BUG: undoes a sandbox allocation it never marks
*/
/*

*/
int16_t Hire_Merc_Popup(int16_t type, int16_t count, int16_t level, int16_t cost)
{
    int16_t return_value;
    int16_t window_y;
    int16_t window_x;
    int16_t hire_button_field;
    int16_t reject_button_field;
    int16_t input_field_idx;
    int16_t leave_screen;  // _SI_

    window_x = 25;
    window_y = 17;

    OVL_DisableIncmBlink();

    Allocate_Reduced_Map();

    Set_Page_Off();

    Reset_Map_Draw();

    Main_Screen_Draw();

    Copy_Off_To_Back();

    Set_Page_On();

    Cache_Graphics_Things();

    CRP_EVNT_MercUnitType = type;

    EVNT_MercUnitCount = count;

    GAME_AssetCost = cost;

    Set_Button_Down_Offsets(1, 1);

    Create_Unit__WIP(CRP_EVNT_MercUnitType, HUMAN_PLAYER_IDX, _FORTRESSES[HUMAN_PLAYER_IDX].wx, _FORTRESSES[HUMAN_PLAYER_IDX].wy, _FORTRESSES[HUMAN_PLAYER_IDX].wp, ST_UNDEFINED);

    _UNITS[(_units - 1)].Level = level;

    _UNITS[(_units - 1)].XP = TBL_Experience[level];

    Hire_Hero_Load(type);

    Load_Battle_Unit((_units - 1), global_battle_unit);

    USW_Build_Effect_List((_units - 1), uv_specials_list_array, &uv_specials_list_count);

    UV_Setup_HireScr((_units - 1), uv_specials_list_count);

    Assign_Auto_Function(Hire_Merc_Popup_Draw, 2);

    Clear_Fields();

    Set_Font_Style(4, 4, 0, 0);

    hire_button_field = Add_Button_Field((window_x + 221), (window_y + 143), cnst_Hire_Msg_7, red_button_seg, empty_string__ovr127[0], ST_UNDEFINED);

    reject_button_field = Add_Button_Field((window_x + 221), (window_y + 162), cnst_Merchant_Msg_5, red_button_seg, ST_UNDEFINED, ST_UNDEFINED);

    Set_Font_Style(0, 0, 2, 0);

    _page_flip_effect = 3;

    Set_Page_Off();

    Hire_Merc_Popup_Draw();

    Draw_Fields();

    PageFlip_FX();

    _page_flip_effect = 0;

    leave_screen = ST_FALSE;

    Set_Input_Delay(2);

    // ~== Set_Hire_Merc_Popup_Help_List()
    LBX_Load_Data_Static(hlpentry_lbx_file__ovr127, 23, (SAMB_ptr)_help_entries, 0, 23, 10);
    Set_Help_List((char *)&_help_entries[0], 23);

    while(leave_screen == ST_FALSE)
    {

        UV_Set_Specials_Help_Fields(uv_specials_list_array, uv_specials_list_count, 0);

        input_field_idx = Get_Input();

        if(input_field_idx == reject_button_field)
        {
            leave_screen = ST_TRUE;
            Play_Left_Click__DUPE();
            _units -= 1;
            return_value = ST_FALSE;
        }

        if(input_field_idx == hire_button_field)
        {
            Play_Left_Click();
            leave_screen = ST_TRUE;
            _units -= 1;
            return_value = ST_TRUE;
        }

        if(leave_screen == ST_FALSE)
        {
            Set_Page_Off();
            Hire_Merc_Popup_Draw();
            PageFlip_FX();
        }

    }

    Clear_Fields();
    Cache_Graphics_Overland();
    Full_Draw_Main_Screen();
    Deactivate_Auto_Function();
    Release_Block(_screen_seg);
    // TODO  Near_Allocate_Undo(_screen_seg);
    OVL_EnableIncmBlink();
    Deactivate_Help_List();

    return return_value;
}


// WZD s127p09
// drake178: AI_OfferHero()
/*
; checks whether the player can afford the hero (cost
; less than 90% gold reserves), and if so, hires the
; hero to the specified slot
*/
/*

*/
void AI_Accept_Hero(int16_t player_idx, int16_t hero_slot_idx, int16_t unit_type_idx)
{
    int16_t Hero_Hired;
    int16_t allowance;
    int16_t unit_cost;  // _DI_

    unit_cost = _unit_type_table[unit_type_idx].cost;

    if(_players[player_idx].charismatic > 0)
    {
        unit_cost /= 2;  // 50%  half price
    }

    allowance = ((_players[player_idx].gold_reserve * 100) / 90);

    if(allowance >= unit_cost)
    {
        Hero_Hired = WIZ_HireHero(player_idx, unit_type_idx, hero_slot_idx, ST_FALSE);

        if(Hero_Hired == ST_FALSE)
        {
            return;
        }

        UNIT_RemoveExcess((_units - 1));

        _players[player_idx].gold_reserve -= unit_cost;

        _HEROES2[player_idx]->heroes[unit_type_idx].Level = -20;  // DEDU  What's with the `-20`? Didn't I see that somewhere else - Kill_Units(), Remove_Dead_Units()?

    }

}
