/*
    Hire - Merchant

    WIZARDS.EXE
        ovr127
*/

#include "MoX.H"



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
extern SAMB_ptr ITEM_Draw_Seg;



// WZD dseg:6504                                                 BEGIN:  ovr127 - Strings

// WZD dseg:6504
char cnst_HIRE_File[] = "HIRE";
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
char cnst_NAMES_File[] = "NAMES";
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
char cnst_HOTKEY_H[] = "H";
// WZD dseg:65E3
char cnst_HOTKEY_A_5[] = "A";
// WZD dseg:65E5
char cnst_HLPENTRY_File5[] = "hlpentry";
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
// EVNT_LoadMerchantWnd()

// WZD s127p02
// EVNT_DrawMerchantWnd()

// WZD s127p03
// EVNT_MerchantDialog()

// WZD s127p04
void Hire_Hero_Load(int16_t unit_type)
{

    Set_Mouse_List(1, mouse_list_default);

    Reset_First_Block(_screen_seg);

    Near_Allocate_Reset();  // NOTE  ; discards the contents of the LBX near memory buffer

    Unit_View_Allocate();

    _reduced_map_seg = Allocate_Next_Block(_screen_seg, 303);

    // HIRE.LBX, 4  000  BANNER
    hire_banner_seg = LBX_Reload_Next(cnst_HIRE_File, 0, _screen_seg);

    // BUG  ¿ ; should use the already allocated USW_ItemDraw_Seg ?
    ITEM_Draw_Seg = Allocate_Next_Block(_screen_seg, 30);  // 30 PR, 480 B

    // BUG  ¿ ; asset never used ?
    // HIRE.LBX, 4  004  NAMEBOX
    hire_namebox_seg = LBX_Reload_Next(cnst_HIRE_File, 4, _screen_seg);

    hirehero_unit_type_name = (char *)Near_Allocate_Next(13);

    // BUG  ~macro for unit figure or hero portrait, but only loading hero name records, and this whole function/popup is only for heros
    if(unit_type > ut_Chosen)
    {
        LBX_Load_Data_Static(cnst_NAMES_File, 0, (SAMB_ptr)hirehero_unit_type_name, unit_type, 1, 13);
    }
    else
    {
        Load_Unit_Figure(unit_type, 1);
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

    Thing_View_Draw__WIP(window_x, window_y, 1, (_units - 1), uv_specials_list_array, uv_specials_list_count, ITEM_Draw_Seg);

    FLIC_Draw((window_x - 25), 0, hire_banner_seg);

    Set_Alias_Color(163);

    Set_Font_Style(4, 4, 4, 4);

    if(GAME_HeroHireType == 0)  // ; 0: random, 1: summon, 2: prisoner, 3: champion
    {
        strcpy(GUI_NearMsgString, cnst_Hire_Msg_1);
        itoa(GAME_AssetCost, temp_string, 10);
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
        FLIC_Draw((window_x + 29), (window_y + 55), outpost_name_background_seg);
        Print_Centered((window_x + 104), (window_y + 61), cnst_Hire_Msg_6);
        FLIC_Reset_CurrentFrame(red_button_seg);
        FLIC_Draw((window_x + 221), (window_y + 143), red_button_seg);
        FLIC_Reset_CurrentFrame(red_button_seg);
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
    uint8_t Name_Edit_Colors[20];
    int16_t Hero_Hired;
    int16_t Hire_Success;
    int16_t window_y;
    int16_t Hire_Button_Index;
    int16_t Reject_Button_Index;
    int16_t itr;
    int16_t leave_screen;
    int16_t input_field_idx;
    int16_t window_x;  // _DI_

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

    GFX_Swap_Overland();

    Hire_Hero_Load(unit_type_idx);

    Assign_Auto_Function(Hire_Hero_Popup_Draw, 2);

    Set_Button_Down_Offsets(1, 1);

    PageFlipEffect = 0;

    window_x = 25;
    window_y = 17;

    strcpy(_players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].name, hirehero_unit_type_name);

    Hire_Success = WIZ_HireHero(HUMAN_PLAYER_IDX, unit_type_idx, hero_slot_idx, 0);

    // ; conflicting condition - will always jump
    if(Hire_Success == ST_FALSE)
    {
        GFX_Swap_Cities();
        return;
    }

    GAME_AssetCost = (_unit_type_table[unit_type_idx].Cost + ((_UNITS[(_units - 1)].Level * _unit_type_table[unit_type_idx].Cost) / 4));

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
        UNIT_MarkRemoved((_units - 1), 1);
        GFX_Swap_Cities();
        return ST_FALSE;
    }

    if(_players[HUMAN_PLAYER_IDX].charismatic > 0)
    {
        GAME_AssetCost /= 2;
    }

    Load_Battle_Unit((_units - 1), global_battle_unit);

    USW_Build_Effect_List((_units - 1), &uv_specials_list_array[0], &uv_specials_list_count);

    UV_Setup_HireScr((_units - 1), uv_specials_list_count);

    Clear_Fields();

    Set_Font_Style(4, 4, 0, 0);

    if(GAME_HeroHireType == 0)  // ; 0: random, 1: summon, 2: prisoner, 3: champion
    {
        Hire_Button_Index = Add_Button_Field((window_x + 221), (window_y + 143), cnst_Hire_Msg_7, red_button_seg, 'H' /* cnst_HOTKEY_H */, -1);
    }
    else
    {
        Hire_Button_Index = Add_Button_Field((window_x + 221), (window_y + 143), cnst_Hire_Msg_8, red_button_seg, 'A' /* cnst_HOTKEY_A_5 */, -1);
    }

    Reject_Button_Index = Add_Button_Field((window_x + 221), (window_y + 162), cnst_Merchant_Msg_5, red_button_seg, -1, -1);

    UV_Add_Specials_Fields((window_x + 8), (window_y + 108), uv_specials_list_array, uv_specials_list_count, uv_specials_list_index);

    Set_Font_Style(0, 0, 2, 0);

    Set_Page_Off();

    Hire_Hero_Popup_Draw();

    Draw_Fields();

    Apply_Palette();

    // TODO  VGA_MosaicFlip();


    LBX_Load_Data_Static(cnst_HLPENTRY_File5, 23, (SAMB_ptr)_help_entries, 0, 23, 10);
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
            BEGIN:  Reject
        */
        if(input_field_idx = Reject_Button_Index)
        {
            leave_screen = ST_TRUE;
            // TODO  RP_SND_LeftClickSound2();
            if(_UNITS[(_units - 1)].Level < 4)
            {
                _UNITS[(_units - 1)].Level += 1;
                _UNITS[(_units - 1)].XP = TBL_Experience[_UNITS[(_units - 1)].Level];
            }
            UNIT_MarkRemoved((_units - 1), 1);
        }
        /*
            END:  Reject
        */

        /*
            BEGIN:  Hire / Accept
        */
        if(input_field_idx == Hire_Button_Index)
        {
            // TODO  SND_LeftClickSound();
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
                // ; BUG: can push a unit out of the capital when finding a prisoner
                UNIT_RemoveExcess((_units - 1));
                Hero_Hired = ST_TRUE;
                leave_screen = ST_TRUE;
            }
        }
        /*
            END:  Hire / Accept
        */

        Clear_Fields_Above(Reject_Button_Index);
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

        // ; clears the GUI, creates an edit box with the passed
        // ; parameters, and transfers control to it until it
        // ; loses focus, at which point the resulting string is
        // ; saved to the passed pointer and the function returns
        // ; -1 if the Esc key was pressed, or 0 otherwise
        // TODO  Input_Box_Popup((window_x + 45), (window_y + 75), 120, _players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].name, 13, 0, 0, 0, &Name_Edit_Colors[0], empty_string__ovr127, -1);
        strcpy(hero_names_table[unit_type_idx].name, _players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].name);
    }

    Clear_Fields();
    Deactivate_Auto_Function();
    GFX_Swap_Cities();

    Full_Draw_Main_Screen();
    OVL_EnableIncmBlink();
    Deactivate_Help_List();

    return Hero_Hired;
}


// WZD s127p07
// EVNT_DrawMercHire()

// WZD s127p08
// EVNT_MercHireDialog()

// WZD s127p09
// AI_OfferHero()
