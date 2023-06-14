
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoX_Data.H"
#include "UNITTYPE.H"

#include "MoM_main.H"

#include "MainScr.H"
#include "MainScr_Maps.H"

#include "Allocate.H"
#include "FLIC_Draw.H"
#include "Fields.H"
#include "Fonts.H"
#include "Input.H"
#include "LBX_Load.H"
#include "Mouse.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif



void Main_Screen_Load_Pictures(void);
// WZD o57p02
void Main_Screen_Add_Fields(void);
// WZD o57p03
void Add_Unit_Action_Fields(void);
// WZD o57p04
// Main_Screen_Draw_Unit_Actions()
// WZD o57p05
void Add_Game_Button_Fields(void);
// WZD o57p06
// Main_Screen_Draw_Game_Buttons()
// WZD o57p07
void Add_Unit_Window_Fields(void);



// int16_t field_idx_hotkey_C;
// int16_t field_idx_hotkey_L;
// int16_t field_idx_hotkey_N;
// int16_t field_idx_hotkey_H;
int16_t field_idx_hotkey_Q;
// int16_t field_idx_hotkey_ESC;
// int16_t field_idx_click_C;
// int16_t field_idx_click_L;
// int16_t field_idx_click_N;
// int16_t field_idx_click_H;
// int16_t field_idx_click_Q;
int16_t field_idx_hotkey_G;
// OVL_GAME_Button
// _game_button
// _main_game_button_seg


// dseg:2E10                                                 BEGIN: Main Screen

// WZD dseg:2E10
// CRP_DBG_Alt_K_State dw 0
// WZD dseg:2E12
// DBG_Alt_A_State dw 0
// WZD dseg:2E14
// IDK_FirstTurnFlag_w398B4 dw 0
// WZD dseg:2E16
// hotkey_MainScrn_X db 'X'
// WZD dseg:2E17
// cnst_ZeroString_31 db 0
// WZD dseg:2E18
// cnst_HOTKEY_S1 db '!',0
// WZD dseg:2E1A
// cnst_HOTKEY_S2 db '@',0
// WZD dseg:2E1C
// cnst_HOTKEY_S3 db '#',0
// WZD dseg:2E1E
// cnst_HOTKEY_C_2 db 'C',0
// WZD dseg:2E20
// cnst_HOTKEY_T db 'T',0
// WZD dseg:2E22
char hotkey_NextTurn[] = "N";
// WZD dseg:2E24
char hotkey_PlaneButton[] = "P";
// WZD dseg:2E26
// cnst_HOTKEY_Q db 'Q',0
// WZD dseg:2E28
// cnst_HOTKEY_U db 'U',0
// WZD dseg:2E2A
// cnst_HOTKEY_Home dw KP_Home
// WZD dseg:2E2C
// cnst_HOTKEY_Up dw KP_Up
// WZD dseg:2E2E
// cnst_HOTKEY_PgUp dw KP_PgUp
// WZD dseg:2E30
// cnst_HOTKEY_Left dw KP_Left
// WZD dseg:2E32
// cnst_HOTKEY_Right dw KP_Right
// WZD dseg:2E34
// cnst_HOTKEY_End dw KP_End
// WZD dseg:2E36
// cnst_HOTKEY_Down dw KP_Down
// WZD dseg:2E38
// cnst_HOTKEY_PgDn dw KP_PgDn
// WZD dseg:2E3A
// cnst_HOTKEY_F10 dw KP_F10
// WZD dseg:2E3C
// cnst_HOTKEY_F1 dw KP_F1
// WZD dseg:2E3E
// cnst_HOTKEY_F2 dw KP_F2
// WZD dseg:2E40
// cnst_HOTKEY_F3 dw KP_F3
// WZD dseg:2E42
// cnst_HOTKEY_F4 dw KP_F4
// WZD dseg:2E44
// cnst_HOTKEY_F5 dw KP_F5
// WZD dseg:2E46
// cnst_HOTKEY_F6 dw KP_F6
// WZD dseg:2E48
// cnst_HOTKEY_F7 dw KP_F7
// WZD dseg:2E4A
// cnst_HOTKEY_F8 dw KP_F8
// WZD dseg:2E4C
// cnst_HOTKEY_F9 dw KP_F9
// WZD dseg:2E4E
// cnst_HOTKEY_K db 'K',0
// WZD dseg:2E50
// cnst_HOTKEY_A db 'A',0
// WZD dseg:2E52
// aYouMayNotThrowAnySp db 'You may not throw any spells while you are banished.  There are at least ',0
// WZD dseg:2E9C
// aTurnsRemainingUntil db ' turns remaining until you may return.',0
// WZD dseg:2EC3
// aSomeUnitsDoNotHaveE db 'Some units do not have enough food and will die unless you allocate more farmers in a city.  Do you wish to allow them to die?',0
// WZD dseg:2F42
// aTheSelectedUnitsCan db 'The selected units cannot move on this plane.',0
// WZD dseg:2F70
// aTurn_1 db 'turn',0

// WZD dseg:2F75 cnst_HOTKEY_D db 'D',0
// WZD dseg:2F77 cnst_HOTKEY_W db 'W',0
// WZD dseg:2F79 cnst_HOTKEY_B_2 db 'B',0
// WZD dseg:2F7B 
char hotkey_GameButton[] = "G";
// WZD dseg:2F7D 
char hotkey_SpellsButton[] = "S";
// WZD dseg:2F7F 
char hotkey_MagicButton[] = "M";
// WZD dseg:2F81 
char hotkey_InfoButton[] = "I";


// WZD dseg:C03E                                                 BEGIN: Main_Screen
// WZD dseg:C03E
// CRP_OverlandVar_2 dw 0
// WZD dseg:C040
// CRP_OverlandVar_4 dw 0
// WZD dseg:C042
// OVL_MapVar3 dw 0
// WZD dseg:C044
// CRP_OVL_Obstacle_Var1 dw 0
// WZD dseg:C046
// CRP_OVL_UU_Control_1 dw 0
// WZD dseg:C048
// CRP_OVL_UU_Control_2 dw 0
// WZD dseg:C04A
// CRP_OVL_UU_Control_3 dw 0
// WZD dseg:C04C
// CRP_OVL_UU_Control_4 dw 0
// WZD dseg:C04E
int16_t special_action_flag;  // AKA OVL_ExtraUnitAction
// WZD dseg:C050
int16_t _next_turn_button;
// WZD dseg:C052
int16_t Unit_Window_Fields[9];
// WZD dseg:C064
// CRP_OverlandVar_3 dw 0
// WZD dseg:C066
// OVL_Path_Length dw 0
// WZD dseg:C068
// OVL_StackHasPath dw 0

// WZD dseg:C06A
int16_t _done_button;
// WZD dseg:C06C
int16_t _wait_button;
// WZD dseg:C06E
int16_t _patrol_button;
// WZD dseg:C070
int16_t _special_button;
// WZD dseg:C072
int16_t _plane_button;
// WZD dseg:C074
int16_t _info_button;
// WZD dseg:C076
int16_t _magic_button;
// WZD dseg:C078
int16_t _cities_button;
// WZD dseg:C07A
int16_t _armies_button;
// WZD dseg:C07C
int16_t _spells_button;
// WZD dseg:C07E
int16_t _game_button;


int16_t main_screen_loaded = ST_FALSE;



char main_lbx_file[] = "MAIN";                  // WZD dseg:29EF
char backgrnd_lbx_file[] = "BACKGRND";          // WZD dseg:29F4



char cstr_1st5[] = " !\"$#";
char cstr_ABC[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char cstr_WarGames[] = "WOULD YOU LIKE TO PLAY A GAME?";
// WZD dseg:31A2
char cstr_GP[] = "GP";
// WZD dseg:31A5
char cstr_MP[] = "MP";

// WZD dseg:31A3 50 00                   UNIT_Purifying_Mark db 'P',0
// WZD dseg:31A8 42 00                   UNIT_Building_Mark db 'B',0             ; could use dseg:2c50
// WZD dseg:31AA 47 00                   UNIT_Going_Mark db 'G',0
// WZD dseg:31AC 43 00                   UNIT_Unk_C_Mark db 'C',0
// WZD dseg:31AE 4D 6F 76 65 73 3A 00    cnst_Moves db 'Moves:',0
// WZD dseg:31B5 00                      db    0
/*
    ? BEGIN: Main_Screen_Draw_Summary_Window ?
*/
// WZD dseg:31B6
char cstr_Gold[] = "Gold";
// WZD dseg:31BB
char cstr_Food[] = "Food";
// WZD dseg:31C0
char cstr_Mana[] = "Mana";
// WZD dseg:31C5
char cstr_Conjunction[] = "Conjunction";
// WZD dseg:31D1
char cstr_Bad_Moon[] = "Bad Moon";
// WZD dseg:31DA
char cstr_Good_Moon[] = "Good Moon";
// WZD dseg:31E4
char cstr_Mana_Short[] = "Mana Short";
/*
    ? END: Main_Screen_Draw_Summary_Window ?
*/
// WZD dseg:31EF 00                      db    0
// WZD dseg:31F0 00                      db    0


// MoX_Data  // WZD dseg:974A
// MoX_Data  int16_t G_OVL_MapDisplay_Y;
// MoX_Data  // WZD dseg:974C
// MoX_Data  int16_t G_OVL_MapDisplay_X;


// How did I get to duplicating these here/Mox_Data?
// MoX_Data  // WZD dseg:BD86
// MoX_Data  int16_t map_plane;
// WZD dseg:BD88
int16_t map_y;
// WZD dseg:BD8A
int16_t map_x;
// WZD dseg:BD8C
int16_t player_idx;






// WZD dseg:942E
SAMB_ptr main_lilevent_icons[6];
// XREF: Main_Screen_Load_Pictures()
// MAIN.LBX,{59,60,61,62,63,64}  LILEVENT  {blue, red, grn, bad, good, short}
// NOTE(drake178): array of 6 single-loaded images
// NOTE(drake178): (blue - red - green - bad - good - short)
// NOTE(drake178): not actually used as far as  i can tell...


// WZD dseg:947E
int16_t mana_staff_lock_flag;
// WZD dseg:9480
int16_t research_staff_lock_flag;
// WZD dseg:9482
int16_t skill_staff_lock_flag;



// WZD dseg:9516
SAMB_ptr next_turn_button_seg;
// XREF: Main_Screen_Load_Pictures(); OVL_DrawNextTurnBtn()
// MAIN.LBX,58  DESELECT    next turn button..

// WZD dseg:9518
SAMB_ptr survey_background;
// XREF: Main_Screen_Load_Pictures(); sub_7A6A9()
// MAIN.LBX,57  MAINSRVY    survey backgrnd

// WZD dseg:951A
SAMB_ptr road_button_border;
// XREF: Main_Screen_Load_Pictures(); sub_5BDCE()
// MAIN.LBX,48  C&RBORDR    road button border

// WZD dseg:951C
SAMB_ptr road_background;
// XREF: Main_Screen_Load_Pictures(); sub_5BDCE()
// MAIN.LBX,45  MAINROAD    road background

// WZD dseg:951E
SAMB_ptr road_ok_button;
// XREF: Main_Screen_Load_Pictures(); sub_5BC57()
// MAIN.LBX,46  CASTCNCL    road ok button

// WZD dseg:9522
SAMB_ptr DESELECT_button_blockout;
// XREF: Main_Screen_Load_Pictures(); sub_5BDCE(); sub_5D5D5(); sub_7A6A9()
// MAIN.LBX,44    DESELECT    button blockout

// WZD dseg:9524
SAMB_ptr cast_button_border;
// XREF: Main_Screen_Load_Pictures(); sub_5D5D5(); sub_7A6A9()
// MAIN.LBX,47  C&RBORDR    cast button border

// WZD dseg:9526
SAMB_ptr cast_cancel_button;
// XREF: Main_Screen_Load_Pictures(); sub_5BC57(); sub_5CDB2(); sub_5CE84(); sub_7A5EF()
// MAIN.LBX,41  CASTCNCL    cast cancel button

// WZD dseg:9528
SAMB_ptr cast_background;
// XREF: Main_Screen_Load_Pictures(); sub_5D5D5()
// MAIN.LBX,40  MAINCAST    cast background

// WZD dseg:952C
SAMB_ptr next_turn_background_seg;
// XREF: Main_Screen_Load_Pictures(); OVL_DrawNextTurnBtn()
// MAIN.LBX,35  DESELECT    next turn backgrnd

// WZD dseg:952E
SAMB_ptr deselect_background;
// XREF: Main_Screen_Load_Pictures(); OVL_DrawDeselectIMG()
// MAIN.LBX,34  DESELECT    deselect backgrnd

// WZD dseg:9548  IMG_OVL_BuildBtn_BG  ; DATA XREF: GFX_Swap_AppendUView() ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:954A  IMG_WIZ_BookBinders dw 12h dup(0)  ; DATA XREF: sub_6343B()
// WZD dseg:956E
SAMB_ptr mirror_screen_background;
// XREF: Main_Screen_Load_Pictures(); sub_62BBC()
// ; single-loaded image
// WZD dseg:9570 IMG_ArmyScrn_Ok  ; DATA XREF: Armies_Screen(); sub_58CD2(); sub_598F1(); 2 frame image (normal - clicked)


// WZD dseg:95F2
SAMB_ptr goto_booty_icon;
// MAIN.LBX,50  GOTO        goto booty



// WZD dseg:973E
int16_t _minimap_grid_field;
// WZD dseg:9740
int16_t _main_map_grid_field;
// WZD dseg:9742
int16_t _minimap_grid_x;
// WZD dseg:9744
int16_t _minimap_grid_y;
// WZD dseg:9746 CRP_OVL_MapWindowY dw 0
// WZD dseg:9748 CRP_OVL_MapWindowX dw 0


// WZD dseg:974A
int16_t _prev_world_y;
// WZD dseg:974C
int16_t _prev_world_x;

// WZD dseg:974E 
int16_t _main_map_grid_x;
// WZD dseg:9750 
int16_t _main_map_grid_y;


// WZD dseg:9922
SAMB_ptr main_background;

// WZD dseg:9924
// XREF: Main_Screen_Load_Pictures(); OVL_DrawStackSelect()
// ; array of 9 single-loaded images
SAMB_ptr unit_backgrounds[9];




// XREF: Main_Screen_Load_Pictures(); OVL_DrawUnitAttribs()
// single-loaded image
SAMB_ptr main_red_medal_icon;
SAMB_ptr main_gold_medal_icon;
SAMB_ptr main_white_medal_icon;
SAMB_ptr main_adamantium_weapon_icon;
SAMB_ptr main_mithril_weapon_icon;
SAMB_ptr main_magic_weapon_icon;

// Screen Layout: L-R, T-B
//     Done, Patrol, Wait, Build
// XREF: Main_Screen_Load_Pictures(); OVL_DrawUnitActnBtns(); OVL_DrawLockedActns()
// single-loaded image
SAMB_ptr main_lock_purify_button;
SAMB_ptr main_lock_build_button;
SAMB_ptr main_lock_wait_button;
SAMB_ptr main_lock_patrol_button;
SAMB_ptr main_lock_done_button;

// XREF: Main_Screen_Load_Pictures(); OVL_SetUnitButtons(); OVL_DrawUnitActnBtns()
// ; single-loaded 2 frame image (normal - clicked)
SAMB_ptr main_meld_button;
SAMB_ptr main_purify_button;
SAMB_ptr main_build_button;
SAMB_ptr main_done_button;
SAMB_ptr main_wait_button;
SAMB_ptr main_patrol_button;

// ? block of Main Screen - Top Buttons Bar ?
// XREF: Main_Screen_Load_Pictures(); OVL_SetMenuButtons(); OVL_DrawMainMenu()
// ; single-loaded 2 frame image (normal - clicked)
// Code Order:   ?
// Data Order:   S,I,M,C,G,A,P
// Load Order:   G,S,A,C,M,I,P
// LBX Order:    G,S,A,C,M,I,P
// Screen Order: G,S,A,C,M,I,P
SAMB_ptr main_spells_button;
SAMB_ptr main_info_button;
SAMB_ptr main_magic_button;
SAMB_ptr main_cities_button;
SAMB_ptr main_game_button;
SAMB_ptr main_armies_button;
SAMB_ptr main_plane_button;

// XREF: Main_Screen_Load_Pictures(); OVL_DrawMoveTypes()
// single-loaded image
SAMB_ptr move_sail_icon;
SAMB_ptr move_swim_icon;
SAMB_ptr move_mt_icon;
SAMB_ptr move_forest_icon;
SAMB_ptr move_fly_icon;
SAMB_ptr move_path_icon;
SAMB_ptr move_astral_icon;
SAMB_ptr move_wind_icon;
SAMB_ptr move_boot_icon;

// WZD dseg:998C 00 00                   GUI_SmallWork_IMG@ dw 0                 ; 96h LBX_Alloc_Space paragraphs
// WZD dseg:998E 00 00                   gsa_Sandbox dw 0                        ; 203h + 11F8h LBX_Alloc_Space paragraphs
// WZD dseg:9990 00 00                   _current_screen dw 0
// WZD dseg:9992 00 00                   RP_GUI_GrowOutFrames dw 0
// WZD dseg:9994 00 00                   RP_GUI_GrowOutTop dw 0
// WZD dseg:9996 00 00                   RP_GUI_GrowOutLeft dw 0
// WZD dseg:9998 00 00 00 00             TBL_Events dd 0                         ; 7 LBX_Alloc_Space paragraphs (112 bytes)

// WZD dseg:999C
// uint16_t g_ActiveStack_UnitCount = 0;  // AKA OVL_StackSize
// _unit_stack_count = 0;
// WZD dseg:999E
// struct STK_UNIT UNIT_Stack[9];



// WZD dseg:C082
// AKA OVL_STKUnitCards_Top
int16_t _unit_window_start_y;
// WZD dseg:C084
// AKA OVL_STKUnitCards_Lft
int16_t _unit_window_start_x;

// WZD dseg:C086
SAMB_ptr _reduced_map_seg;
// ; 12Fh LBX_Alloc_First paragraphs in the LBX_Sandbox_Segment


// WZD dseg:C092
int16_t cycle_incomes; //  dw 0                      ; -1 draws negative incomes with a static color instead


// WZD dseg:CB92
SAMB_ptr _unit_colored_backgrounds_seg[6];




void Main_Screen_Load_Pictures(void)
{
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Load_Pictures()\n", __FILE__, __LINE__);
#endif

    main_background = LBX_Load(main_lbx_file,  0);

    main_game_button = LBX_Load(main_lbx_file, 1);
    main_spells_button = LBX_Load(main_lbx_file, 2);
    main_armies_button = LBX_Load(main_lbx_file, 3);
    main_cities_button = LBX_Load(main_lbx_file, 4);
    main_magic_button = LBX_Load(main_lbx_file, 5);
    main_info_button = LBX_Load(main_lbx_file, 6);
    main_plane_button = LBX_Load(main_lbx_file, 7);

    main_done_button = LBX_Load(main_lbx_file, 8);
    main_patrol_button = LBX_Load(main_lbx_file, 9);
    main_wait_button = LBX_Load(main_lbx_file, 10);
    main_build_button = LBX_Load(main_lbx_file, 11);
    main_purify_button = LBX_Load(main_lbx_file, 42);
    main_meld_button = LBX_Load(main_lbx_file, 49);
//     main_lock_done_button = LBX_Load(main_lbx_file, 12);
//     main_lock_patrol_button = LBX_Load(main_lbx_file, 13);
//     main_lock_wait_button = LBX_Load(main_lbx_file, 14);
//     main_lock_build_button = LBX_Load(main_lbx_file, 15);
//     main_lock_purify_button = LBX_Load(main_lbx_file, 43);
// 
//     main_white_medal_icon = LBX_Load(main_lbx_file, 51);
//     main_gold_medal_icon = LBX_Load(main_lbx_file, 52);
//     main_red_medal_icon = LBX_Load(main_lbx_file, 53);
//     main_magic_weapon_icon = LBX_Load(main_lbx_file, 54);
//     main_mithril_weapon_icon = LBX_Load(main_lbx_file, 55);
//     main_adamantium_weapon_icon = LBX_Load(main_lbx_file, 56);
// 
//     move_sail_icon = LBX_Load(main_lbx_file, 18);
//     move_swim_icon = LBX_Load(main_lbx_file, 19);
//     move_mt_icon = LBX_Load(main_lbx_file, 20);
//     move_forest_icon = LBX_Load(main_lbx_file, 21);
//     move_fly_icon = LBX_Load(main_lbx_file, 22);
//     move_path_icon = LBX_Load(main_lbx_file, 23);
//     move_astral_icon = LBX_Load(main_lbx_file, 36);
//     move_wind_icon = LBX_Load(main_lbx_file, 37);
//     move_boot_icon = LBX_Load(main_lbx_file, 38);

    for(itr = 0; itr < 9; itr++)
    {
        unit_backgrounds[itr] = LBX_Load(main_lbx_file, 24 + itr);
    }

    next_turn_button_seg = LBX_Load(main_lbx_file, 58);

    deselect_background = LBX_Load(main_lbx_file, 34);
    next_turn_background_seg = LBX_Load(main_lbx_file, 35);

//     cast_background = LBX_Load(main_lbx_file, 40);
//     cast_cancel_button = LBX_Load(main_lbx_file, 41);
//     cast_button_border = LBX_Load(main_lbx_file, 47);
// 
//     DESELECT_button_blockout = LBX_Load(main_lbx_file, 44);
// 
//     road_background = LBX_Load(main_lbx_file, 45);
//     road_ok_button = LBX_Load(main_lbx_file, 46);
//     road_button_border = LBX_Load(main_lbx_file, 48);
// 
//     survey_background = LBX_Load(main_lbx_file, 57);
// 
//     mirror_screen_background = LBX_Load(backgrnd_lbx_file, 4);
// 
//     goto_booty_icon = LBX_Load(main_lbx_file, 50);
// 
// 
//     // LILEVENT blue
//     // LILEVENT red
//     // LILEVENT grn
//     // LILEVENT bad
//     // LILEVENT good
//     // LILEVENT short
// 
// 
//     for(itr = 0; itr < 6; itr++)
//     {
//         main_lilevent_icons[itr] = LBX_Load(main_lbx_file, 59 + itr);
//     }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Load_Pictures()\n", __FILE__, __LINE__);
#endif

}


// WZD o57p02
void Main_Screen_Add_Fields(void)
{
    int16_t itr_unit_stack;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Add_Fields()\n", __FILE__, __LINE__);
#endif

    // CRP_OVL_UU_Control_4 = -1000;
    // CRP_OVL_UU_Control_3 = -1000;
    // CRP_OVL_UU_Control_2 = -1000;
    // CRP_OVL_UU_Control_1 = -1000;
    _main_map_grid_field = -1000;
    _minimap_grid_field = -1000;
    _next_turn_button = -1000;
    _special_button = -1000;
    _patrol_button = -1000;
    _wait_button = -1000;
    _done_button = -1000;
    _game_button = -1000;
    _spells_button = -1000;
    _armies_button = -1000;
    _cities_button = -1000;
    _magic_button = -1000;
    _info_button = -1000;
    _plane_button = -1000;

    for(itr_unit_stack = 0; itr_unit_stack < 9; itr_unit_stack++)
    {
        Unit_Window_Fields[itr_unit_stack] = -1000;
    }

    if(_unit_stack_count > 0)
    {
        Add_Unit_Window_Fields();
    }
    
    if(all_units_moved == ST_TRUE)
    {
        // _next_turn_button = Add_Hidden_Field(248, 175, 313, 199, hotkey_NextTurn, ST_UNDEFINED);
        _next_turn_button = Add_Hidden_Field(248, 175, 313, 199, 'N', ST_UNDEFINED);
    }

    // if( (_curr_world_x == _prev_world_x) && (_curr_world_x == _prev_world_x) )
    if( (_map_x == _prev_world_x) && (_map_y == _prev_world_y) )
    {
        // screen x, screen y, cell w, cell h, grid w, grid h, grid x, grid y, help
        _main_map_grid_field = Add_Grid_Field(0, 20, 20, 18, 12, 10, &_main_map_grid_y, &_main_map_grid_x, ST_UNDEFINED);
    }

    // if( (_curr_world_x == _prev_world_x) && (_curr_world_x == _prev_world_x) )
    if( (_map_x == _prev_world_x) && (_map_y == _prev_world_y) )
    {
        // screen x, screen y, cell w, cell h, grid w, grid h, grid x, grid y, help
        _minimap_grid_field = Add_Grid_Field(251, 21, 1, 1, 58, 30, &_minimap_grid_x, &_minimap_grid_y, ST_UNDEFINED);
    }

    // OVL_SetMenuButtons()
    // OVL_SetUnitButtons()
    Add_Game_Button_Fields();
    Add_Unit_Action_Fields();


//     // field_idx_click_C = Add_Hidden_Field(108, (138 + (12 * 0)), 211, (149 + (12 * 0)), 0, -1);
//     // field_idx_click_L = Add_Hidden_Field(108, 150, 211, 161, 0, -1);
//     // field_idx_click_N = Add_Hidden_Field(108, 162, 211, 173, 0, -1);
//     // field_idx_click_H = Add_Hidden_Field(108, 174, 211, 185, 0, -1);
//     // field_idx_click_Q = Add_Hidden_Field(108, 186, 211, 199, 0, -1);
// 
//     // int16_t Add_Hot_Key(int16_t select_char)
//     field_idx_hotkey_G = Add_Hot_Key('G');
//     field_idx_hotkey_Q = Add_Hot_Key('Q');
// 
//     // int16_t Add_Hidden_Field(int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax, int16_t hotkey, int16_t help)
//     _game_button = Add_Hidden_Field(6, 6, 46, 26, ST_NULL, ST_UNDEFINED);



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Add_Fields()\n", __FILE__, __LINE__);
#endif
}

// WZD o57p03
void Add_Unit_Action_Fields(void)
{
    if(_unit_stack_count > 0)
    {
        _done_button = Add_Button_Field(246, 176, '0', main_done_button, 'D', ST_UNDEFINED);
        _patrol_button = Add_Button_Field(280, 176, '0', main_patrol_button, '0', ST_UNDEFINED);
        _wait_button = Add_Button_Field(246, 186, '0', main_wait_button, 'W', ST_UNDEFINED);
    }

    _special_button  = -1000;
    if(special_action_flag != ST_UNDEFINED)
    {
        if(special_action_flag == 2)
        {
            _special_button = Add_Button_Field(280, 186, '0', main_purify_button, '0', ST_UNDEFINED);
        }
        else if(special_action_flag == 9)
        {
            _special_button = Add_Button_Field(280, 186, '0', main_meld_button, '0', ST_UNDEFINED);
        }
        else
        {
            _special_button = Add_Button_Field(280, 186, '0', main_build_button, 'B', ST_UNDEFINED);
        }
    }

    if(_unit_stack_count < 1)
    {
        _done_button = -1000;
        _patrol_button = -1000;
        _wait_button = -1000;
        _special_button = -1000;

        if(all_units_moved == ST_FALSE)
        {
            _wait_button = Add_Button_Field(246, 186, '0', main_wait_button, 'W', ST_UNDEFINED);
        }
    }
}

// WZD o57p04
// Main_Screen_Draw_Unit_Actions()

// WZD o57p05
// OVL_SetMenuButtons
void Add_Game_Button_Fields(void)
{
    // cnst_ZeroString_31 ... hotkey_GameButton, etc.
    _game_button    = Add_Button_Field(  7, 4, '0', main_game_button,   'G', ST_UNDEFINED);
    _spells_button  = Add_Button_Field( 47, 4, '0', main_spells_button, 'S', ST_UNDEFINED);
    _armies_button  = Add_Button_Field( 89, 4, '0', main_armies_button, 'A', ST_UNDEFINED);
    _cities_button  = Add_Button_Field(140, 4, '0', main_cities_button, '0', ST_UNDEFINED);
    _magic_button   = Add_Button_Field(184, 4, '0', main_magic_button,  'M', ST_UNDEFINED);
    _info_button    = Add_Button_Field(226, 4, '0', main_info_button,   'I', ST_UNDEFINED);
    _plane_button   = Add_Button_Field(270, 4, '0', main_plane_button,  'P', ST_UNDEFINED);
}

// WZD o57p06
// Main_Screen_Draw_Game_Buttons()

// WZD o57p07
void Add_Unit_Window_Fields(void)
{

}

// WZD 
void Main_Screen(void)
{
    int16_t finished;
    int16_t input_field_idx;
    int16_t mouse_x;
    int16_t mouse_y;

    int16_t gold;
    int16_t food;
    int16_t mana;

    int16_t itr_units;

    int16_t Unused_Button_Index;
    int16_t hotkey_idx_C;
    int16_t hotkey_idx_Q;
    int16_t hotkey_idx_U;
    int16_t hotkey_idx_X;
    int16_t hotkey_idx_Shift_1;
    int16_t hotkey_idx_Shift_2;
    int16_t hotkey_idx_Shift_3;
    int16_t hotkey_idx_Alt_A;
    int16_t hotkey_idx_Alt_K;
    int16_t hotkey_idx_Alt_N;
    int16_t hotkey_idx_Alt_P;
    int16_t hotkey_idx_Alt_T;
    int16_t hotkey_idx_Up;
    int16_t hotkey_idx_Down;
    int16_t hotkey_idx_Left;
    int16_t hotkey_idx_Right;
    int16_t hotkey_idx_PgUp;
    int16_t hotkey_idx_PgDn;
    int16_t hotkey_idx_Home;
    int16_t hotkey_idx_End;
    int16_t hotkey_idx_F1;
    int16_t hotkey_idx_F2;
    int16_t hotkey_idx_F3;
    int16_t hotkey_idx_F4;
    int16_t hotkey_idx_F5;
    int16_t hotkey_idx_F6;
    int16_t hotkey_idx_F7;
    int16_t hotkey_idx_F8;
    int16_t hotkey_idx_F9;
    int16_t hotkey_idx_F10;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
    dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
    dbg_prn("DEBUG: [%s, %d]: _map_plane: %d\n", __FILE__, __LINE__, _map_plane);
    dbg_prn("DEBUG: [%s, %d]: G_OVL_MapDisplay_X: %d\n", __FILE__, __LINE__, G_OVL_MapDisplay_X);
    dbg_prn("DEBUG: [%s, %d]: G_OVL_MapDisplay_Y: %d\n", __FILE__, __LINE__, G_OVL_MapDisplay_Y);
    dbg_prn("DEBUG: [%s, %d]: _human_player_idx: %d\n", __FILE__, __LINE__, _human_player_idx);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _units: %d\n", __FILE__, __LINE__, _units);
    dbg_prn("DEBUG: [%s, %d]: _unit: %d\n", __FILE__, __LINE__, _unit);
    dbg_prn("DEBUG: [%s, %d]: _unit_stack_count: %d\n", __FILE__, __LINE__, _unit_stack_count);

    dbg_prn("DEBUG: [%s, %d]: _UNITS[_unit].owner_idx: %d\n", __FILE__, __LINE__, _UNITS[_unit].owner_idx);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[_unit].world_x: %d\n", __FILE__, __LINE__, _UNITS[_unit].world_x);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[_unit].world_y: %d\n", __FILE__, __LINE__, _UNITS[_unit].world_y);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[_unit].world_plane: %d\n", __FILE__, __LINE__, _UNITS[_unit].world_plane);
#endif
// DEBUG: [J:\STU\devel\STU-MoM_Rasm\src\MainScr.C, 476]: _units: 778
// DEBUG: [J:\STU\devel\STU-MoM_Rasm\src\MainScr.C, 477]: _unit: 176
// DEBUG: [J:\STU\devel\STU-MoM_Rasm\src\MainScr.C, 478]: _unit_stack_count: 0
// DEBUG: [J:\STU\devel\STU-MoM_Rasm\src\MainScr.C, 480]: _UNITS[_unit].owner_idx: 1
// DEBUG: [J:\STU\devel\STU-MoM_Rasm\src\MainScr.C, 481]: _UNITS[_unit].world_x: 36
// DEBUG: [J:\STU\devel\STU-MoM_Rasm\src\MainScr.C, 482]: _UNITS[_unit].world_y: 14
// DEBUG: [J:\STU\devel\STU-MoM_Rasm\src\MainScr.C, 483]: _UNITS[_unit].world_plane: 0
#ifdef STU_DEBUG
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == 0)
        {
            // dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].world_x: %02X\n", __FILE__, __LINE__, itr_units, _UNITS[itr_units].world_x);
            // dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].world_y: %02X\n", __FILE__, __LINE__, itr_units, _UNITS[itr_units].world_y);
            // dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].world_plane: %02X\n", __FILE__, __LINE__, itr_units, _UNITS[itr_units].world_plane);
            dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].world_x: %d\n", __FILE__, __LINE__, itr_units, _UNITS[itr_units].world_x);
            dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].world_y: %d\n", __FILE__, __LINE__, itr_units, _UNITS[itr_units].world_y);
            dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].world_plane: %d\n", __FILE__, __LINE__, itr_units, _UNITS[itr_units].world_plane);
        }
    }
#endif

    _unit = 46;
    Build_Unit_Stack(0, 0, 18, 17);
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _unit: %d\n", __FILE__, __LINE__, _unit);
    dbg_prn("DEBUG: [%s, %d]: _unit_stack_count: %d\n", __FILE__, __LINE__, _unit_stack_count);

    dbg_prn("DEBUG: [%s, %d]: _unit_stack[0].unit_idx: %d\n", __FILE__, __LINE__, _unit_stack[0].unit_idx);
    dbg_prn("DEBUG: [%s, %d]: _unit_stack[0].active: %d\n", __FILE__, __LINE__, _unit_stack[0].active);
#endif


    // WZD main() |-> WZD_Load_Init_MainGame() |-> Main_Screen_Load_Pictures()
    if(main_screen_loaded == ST_FALSE)
    {
        Main_Screen_Load_Pictures();
        main_screen_loaded = ST_TRUE;
    }



    Reset_Window();
    Clear_Fields();
    // WZD ovr058 j_Set_Mouse_List_Normal() |-> Set_Mouse_List_Normal() |-> Set_Mouse_List(1, normal_mouse_list)
    // WZD dseg:00EE  normal_mouse_list GUI_WINDOW <Crsr_Normal, 0, 0, 0, 319, 199>
    // ~== Set_Mouse_List_MainScr() |-> Set_Mouse_List(1, mouse_list_main/default/normal/arrow);
    Set_Mouse_List(1, mouse_list_default);
    // Set_Outline_Color(0)

    // Disable_Redraw_Function()
    // Set_Redraw_Function(Main_Screen_Draw(), 1)

    // Allocate_Reduced_Map__2()
    // j_OVL_ResetStackDraw
    // j_UNIT_DrawPriorities
    // j_STK_NoUnitDraw
    // j_OVL_MapDrawRenew
    // j_OVL_PrepMinimap
    // j_OVL_SetUnitsOnMap(map_x, map_y, map_plane)
    // j_STK_GetExtraActions()

    // OVL_STKUnitCards_Lft = 247;  // F7h
    // OVL_STKUnitCards_Top = 79;   // 4Fh
    // G_Some_OVL_Var_1 = 0;  // ? ST_FALSE ?
    // CRP_OverlandVar_2 = 0;  // ? ST_FALSE ?
    // CRP_OVL_Obstacle_Var1 = 0;  // ? ST_FALSE ?
    // OVL_MapVar3 = 1;  // ? ST_TRUE ?

    // if (CRP_OverlandVar_3 != ST_TRUE) { CRP_OverlandVar_3 = ST_FALSE; }
    // if (CRP_OverlandVar_4 != ST_TRUE) { CRP_OverlandVar_4 = ST_FALSE; }

    // screen_changed = ST_FALSE;

    // Local_0 = 0;  // ? ST_FALSE ?

    // TODO  Set_Input_Delay(1);
    // TODO  VGA_BlinkReset()
    // TODO  Clear_Help_Fields();
    // TODO  Main_Screen_Help();
    // DBG_Alt_A__TurnCount = ST_UNDEFINED

    // about to call Main_Screen_Draw_Do_Draw(), with ...
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
    dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
    dbg_prn("DEBUG: [%s, %d]: _map_plane: %d\n", __FILE__, __LINE__, _map_plane);
    dbg_prn("DEBUG: [%s, %d]: G_OVL_MapDisplay_X: %d\n", __FILE__, __LINE__, G_OVL_MapDisplay_X);
    dbg_prn("DEBUG: [%s, %d]: G_OVL_MapDisplay_Y: %d\n", __FILE__, __LINE__, G_OVL_MapDisplay_Y);
    dbg_prn("DEBUG: [%s, %d]: _human_player_idx: %d\n", __FILE__, __LINE__, _human_player_idx);
#endif
    Main_Screen_Draw();

    // PageFlip_FX();  // WZD s01p03
    // WZD dseg:9C8E PageFlipEffect dw 0
    // WZD_StartUp_MainGame() { ... PageFlipEffect = 0; ... } 
    // Loaded_Game_Update() { ... PageFlipEffect = 0; ... }
    // So, ... PageFlip_FX(PageFlipEffect = 0) |-> Apply_Palette(); Toggle_Pages()
    Apply_Palette();
    Toggle_Pages();

    // DNE input_field_idx = ST_FALSE;
//     finished = ST_FALSE;
//     while(!finished)
//     {
//         // CLK_Save

        Clear_Fields();
        Main_Screen_Add_Fields();

        hotkey_idx_X = Add_Hot_Key('X');
        Unused_Button_Index = -1000;
        hotkey_idx_Shift_1 = Add_Hot_Key('!');
        hotkey_idx_Shift_2 = Add_Hot_Key('@');
        hotkey_idx_Shift_3 = Add_Hot_Key('#');
        hotkey_idx_C = Add_Hot_Key('C');
        hotkey_idx_Alt_T = Add_Multi_Hot_Key_Field("T");
        hotkey_idx_Alt_N = Add_Multi_Hot_Key_Field("N");
        hotkey_idx_Alt_P = Add_Multi_Hot_Key_Field("P");
        hotkey_idx_Q = Add_Hot_Key('Q');
        hotkey_idx_U = Add_Hot_Key('U');
        hotkey_idx_Home = Add_Hot_Key(KP_Home);
        hotkey_idx_Up = Add_Hot_Key(KP_Up);
        hotkey_idx_PgUp = Add_Hot_Key(KP_PgUp);
        hotkey_idx_Left = Add_Hot_Key(KP_Left);
        hotkey_idx_Right = Add_Hot_Key(KP_Right);
        hotkey_idx_End = Add_Hot_Key(KP_End);
        hotkey_idx_Down = Add_Hot_Key(KP_Down);
        hotkey_idx_PgDn = Add_Hot_Key(KP_PgDn);
        hotkey_idx_F10 = Add_Hot_Key(KP_F10);
        hotkey_idx_F1 = Add_Hot_Key(KP_F1);
        hotkey_idx_F2 = Add_Hot_Key(KP_F2);
        hotkey_idx_F3 = Add_Hot_Key(KP_F3);
        hotkey_idx_F4 = Add_Hot_Key(KP_F4);
        hotkey_idx_F5 = Add_Hot_Key(KP_F5);
        hotkey_idx_F6 = Add_Hot_Key(KP_F6);
        hotkey_idx_F7 = Add_Hot_Key(KP_F7);
        hotkey_idx_F8 = Add_Hot_Key(KP_F8);
        hotkey_idx_F9 = Add_Hot_Key(KP_F9);
        hotkey_idx_Alt_K = Add_Multi_Hot_Key_Field("K");
        hotkey_idx_Alt_A = Add_Multi_Hot_Key_Field("A");


        input_field_idx = Get_Input();

        if(input_field_idx == field_idx_hotkey_G)
        {
            DLOG("(input_field_idx == field_idx_hotkey_G)");
            finished = ST_TRUE;
            g_Current_Screen = scr_Main_Menu_Screen;
        }
        if(input_field_idx == field_idx_hotkey_Q)
        {
            DLOG("(input_field_idx == field_idx_hotkey_G)");
            finished = ST_TRUE;
            g_Current_Screen = scr_Quit_To_DOS;
        }

    // about to call Main_Screen_Draw_Do_Draw(), with ...
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
    dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
    dbg_prn("DEBUG: [%s, %d]: _map_plane: %d\n", __FILE__, __LINE__, _map_plane);
    dbg_prn("DEBUG: [%s, %d]: G_OVL_MapDisplay_X: %d\n", __FILE__, __LINE__, G_OVL_MapDisplay_X);
    dbg_prn("DEBUG: [%s, %d]: G_OVL_MapDisplay_Y: %d\n", __FILE__, __LINE__, G_OVL_MapDisplay_Y);
    dbg_prn("DEBUG: [%s, %d]: _human_player_idx: %d\n", __FILE__, __LINE__, _human_player_idx);
#endif
            Main_Screen_Draw();

            Toggle_Pages();
            mouse_x = Pointer_X();
            mouse_y = Pointer_Y();
            Check_Mouse_Shape(mouse_x, mouse_y);
            Draw_Mouse(mouse_x, mouse_y);
//         
//     }

    // TODO  Disable_Redraw_Function()
    // TODO  Clear_Help_Fields()
    // TODO  Reset_Window()  AKA VGA_ResetDrawWindow()


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen()\n", __FILE__, __LINE__);
#endif
}


// WZD o57p08
void Main_Screen_Draw(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw()\n", __FILE__, __LINE__);
#endif

    Reset_Window();
    Set_Page_Off();

    Main_Screen_Draw_Do_Draw(&_map_x, &_map_y, _map_plane, G_OVL_MapDisplay_X, G_OVL_MapDisplay_Y, _human_player_idx);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Draw()\n", __FILE__, __LINE__);
#endif
}


// WZD o57p06
// MoO2: ~Add_Map_Fields_()
void Main_Screen_Draw_Game_Buttons(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Main_Screen_Draw_Game_Buttons()\n", __FILE__, __LINE__);
#endif

FLIC_Set_CurrentFrame(main_game_button, 0);
FLIC_Draw((7 + (40 * 0)), 4, main_game_button);

FLIC_Set_CurrentFrame(main_spells_button, 0);
FLIC_Draw(47, 4, main_spells_button);

FLIC_Set_CurrentFrame(main_armies_button, 0);
FLIC_Draw(89, 4, main_armies_button);

FLIC_Set_CurrentFrame(main_cities_button, 0);
FLIC_Draw(140, 4, main_cities_button);

FLIC_Set_CurrentFrame(main_magic_button, 0);
FLIC_Draw(184, 4, main_magic_button);

FLIC_Set_CurrentFrame(main_info_button, 0);
FLIC_Draw(226, 4, main_info_button);

FLIC_Set_CurrentFrame(main_plane_button, 0);
FLIC_Draw(270, 4, main_plane_button);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Main_Screen_Draw_Game_Buttons()\n", __FILE__, __LINE__);
#endif

}


// WZD o57p09
void Main_Screen_Reset(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Reset()\n", __FILE__, __LINE__);
#endif

// call    j_RP_LBX_Minimap_Alloc2         ; byte-identical to LBX_Minimap_Alloc, should not exist
// call    Disable_Redraw_Function         ; disables any active redraw function
// call    Clear_Fields                    ; resets the main GUI control variables, removing all
// call    j_OVL_ResetStackDraw            ; sets the overland map drawing to renew everything on
// call    j_UNIT_DrawPriorities           ; sets the draw priority field of each unit record
// call    j_STK_NoUnitDraw                ; set the draw priority of all units in the active
// push    [OVL_Map_Plane]                 ; Plane
// push    [OVL_Map_TopY]                  ; TopY
// push    [OVL_Map_LeftX]                 ; LeftX
// call    j_OVL_SetUnitsOnMap             ; fills out the OVL_UnitsOnMap array with the unit or
// call    j_CRP_GUI_NormalFullscreen      ; sets the Normal_Fullscreen window (GUI_SetWindows)
// call    j_OVL_PrepMinimap               ; draws a minimap into the Minimap_IMG_Seg allocation
// mov     ax, 1
// push    ax                              ; Refresh_Timer
// mov     ax, seg stub057
// push    ax
// mov     ax, offset j_Main_Screen_Draw   ; a wrapper for OVL_DrawMainScreen that passes the
// push    ax                              ; Refresh_Fn
// call    Set_Redraw_Function             ; sets the passed function as the current screen redraw
// call    j_OVL_MapDrawRenew              ; ensures that any subsequent map drawing is treated as
// call    Clear_Help_Fields               ; disables the context-based help by zeroing out its
// call    j_Main_Screen_Help              ; loads and sets the GUI help entry area array for the
// mov     [CRP_OverlandVar], 1

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Reset()\n", __FILE__, __LINE__);
#endif
}




/*
    WIZARDS.EXE  ovr058
*/

// WZD o58p01
void Main_Screen_Draw_Unit_Window(int16_t start_x, int16_t start_y)
{
    int16_t itr_unit_stack_count;
    int16_t unit_stack_unit_idx;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw_Unit_Window(start_x = %d, start_y = %d)\n", __FILE__, __LINE__, start_x, start_y);
#endif

    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _unit_stack_count: %d\n", __FILE__, __LINE__, _unit_stack_count);
#endif

    if(_unit != ST_UNDEFINED && _unit_stack_count != 0)
    {

        for(itr_unit_stack_count = 0; itr_unit_stack_count < _unit_stack_count; itr_unit_stack_count++)
        {
            unit_stack_unit_idx = _unit_stack[itr_unit_stack_count].unit_idx;

            Unit_Window_Picture_Coords(itr_unit_stack_count, &x1, &y1, &x2, &y2);
            FLIC_Draw(x1-1, y1-1, unit_backgrounds[itr_unit_stack_count]);
            Unit_Window_Draw_Unit_Picture(x1, y1, unit_stack_unit_idx, _unit_stack[itr_unit_stack_count].active);
            // DEBUG 'active' Unit_Window_Draw_Unit_Picture(x1, y1, unit_stack_unit_idx, 1);
            // TODO  Unit_Window_Draw_Unit_Attributes(x1, y1, unit_stack_unit_idx);
        }

        // TODO  j_GAME_Animate_UEs
        // TODO  // ; on every second call, advances the unit enchantment
        // TODO  // ; animation stage by one, wrapping from 7 to 0
        // TODO  WP_Quality_Anim_Stg++;
        // TODO  if(WP_Quality_Anim_Stg > 3) { WP_Quality_Anim_Stg = 0; }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Draw_Unit_Window(start_x = %d, start_y = %d)\n", __FILE__, __LINE__, start_x, start_y);
#endif

}



/*
    WIZARDAS.EXE  ovr062
*/

// WZD o62p04
void Build_Unit_Stack(int16_t player_idx, int16_t world_plane, int16_t world_x, int16_t world_y)
{
    int16_t itr_unit_stack;
    int16_t itr_units;
    int16_t current_unit_idx;

    int16_t unit_stack_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Build_Unit_Stack()\n", __FILE__, __LINE__);
#endif

    unit_stack_idx = 0;


    for(itr_unit_stack = 0; itr_unit_stack < 9; itr_unit_stack++)
    {
        _unit_stack[itr_unit_stack].active = ST_FALSE;
    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        current_unit_idx = itr_units;

        if(_UNITS[current_unit_idx].owner_idx != player_idx)
        {
            continue;
        }
        if(_UNITS[current_unit_idx].world_x != world_x)
        {
            continue;
        }
        if(_UNITS[current_unit_idx].world_y != world_y)
        {
            continue;
        }
        if(_UNITS[current_unit_idx].owner_idx == ST_UNDEFINED)
        {
            continue;
        }
        if( (_UNITS[current_unit_idx].world_plane != world_plane) && (_UNITS[current_unit_idx].In_Tower != ST_TRUE) )
        {
            continue;
        }

        _unit_stack[unit_stack_idx].unit_idx = current_unit_idx;

        // ? inactive: no moves, finished, or purify ?
        // ? what about patrol ?
        if(_UNITS[current_unit_idx].HMoves > 0)
        {
            if(_UNITS[current_unit_idx].Finished == ST_FALSE)
            {
                _unit_stack[unit_stack_idx].active = ST_TRUE;

                if(_UNITS[current_unit_idx].Status == 5)  // enum Unit Status - Wait
                {
                    _UNITS[current_unit_idx].Status = 0;  // enum Unit Status - Ready
                }
            }
        }

        if(_UNITS[current_unit_idx].Status == 8)  // enum Unit Status - Purify
        {

        }

        _unit_stack->active = ST_FALSE;


        unit_stack_idx++;
    }



    _unit_stack_count = unit_stack_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Build_Unit_Stack()\n", __FILE__, __LINE__);
#endif
}



/*
    WIZARDAS.EXE  ovr063
*/

// WZD o063p01
// MoO2: ~Print_Main_Screen_Data_()
void Main_Screen_Draw_Status_Window(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Main_Screen_Draw_Status_Window()\n", __FILE__, __LINE__);
#endif

    Set_Outline_Color(0);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] player_idx: %d\n", __FILE__, __LINE__, player_idx);
    dbg_prn("DEBUG: [%s, %d] _players[player_idx].gold_reserve: %d\n", __FILE__, __LINE__, _players[player_idx].gold_reserve);
    dbg_prn("DEBUG: [%s, %d] _players[player_idx].mana_reserve: %d\n", __FILE__, __LINE__, _players[player_idx].mana_reserve);
#endif

    if(_players[player_idx].gold_reserve > 9999 || _players[player_idx].mana_reserve > 9999)
    {
        Set_Font_Style1(0, 0, 0, 0);
    }
    else
    {
        Set_Font_Style1(1, 0, 0, 0);
    }

    Set_Alias_Color(8);

    Print_Integer_Right(265, 68, _players[_human_player_idx].gold_reserve);
    Print_Integer_Right(303, 68, _players[_human_player_idx].mana_reserve);
    // Print_Integer_Right(265, 68, 111);
    // Print_Integer_Right(303, 68, 111);

    Set_Font_Style1(0, 0, 0, 0);

    // // Print(40, 100, cstr_1st5);
    // // Print(40, 180, cstr_ABC);
    // Print(10, 189, cstr_WarGames);
    // // Print(268, 68, "GP");
    // // Print(306, 68, "MP");
    Print(268, 68, cstr_GP);
    Print(306, 68, cstr_MP);

    Set_Outline_Color(0);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Main_Screen_Draw_Status_Window()\n", __FILE__, __LINE__);
#endif

}

// WZD o063p02
// ; int __cdecl __far OVL_DrawMainScreen(int *MapX@, int *MapY@, int Plane, int XPos, int YPos, int Player_Index)
void Main_Screen_Draw_Do_Draw(int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t x_pos, int16_t y_pos, int16_t player_idx)
{
// MapX@= word ptr  6
// MapY@= word ptr  8
// Plane= word ptr  0Ah
// XPos= word ptr  0Ch
// YPos= word ptr  0Eh
// Player_Index= word ptr  10h

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw_Do_Draw(*map_x = %d, *map_y = %d, map_plane = %d, x_pos = %d , y_pos = %d, player_idx = %d)\n", __FILE__, __LINE__, *map_x, *map_y, map_plane, x_pos, y_pos, player_idx);
#endif

    // AKA j_Unset_LastMapDraw_XY();
    
    Minimap_Set_Dims(58, 30);

    // WZD dseg:6FFE Map_LastDraw_X dw 0FFFFh
    // WZD dseg:7000 Map_LastDraw_Y dw 0FFFFh
    // Map_LastDraw_X = e_ST_UNDEFINED
    // Map_LastDraw_Y = e_ST_UNDEFINED


    Draw_Maps(0, 20, 12, 10, map_x, map_y, map_plane, x_pos, y_pos, player_idx);
    // x and y origin for screen
    // width and height of map in squares
    // ? x, y, plane for world ?
    // player_idx of current/human_player_idx

    FLIC_Draw(0, 0, main_background);

    // WZD o67p09
    Draw_Minimap_Window(251, 21, 58, 30);

    // unk_42688 = ST_UNDEFINED;
    // word_426B0 = ST_UNDEFINED;
    // unk_426BA = ST_UNDEFINED;
    // unk_4270A = ST_UNDEFINED;



    // DEBUG
    // void Build_Unit_Stack(int16_t player_idx, int16_t world_plane, int16_t world_x, int16_t world_y)
    // Build_Unit_Stack(0, 0, 0, 0);
    // Build_Unit_Stack(0, 0, 0, 0);
    // Build_Unit_Stack(0, _UNITS[46].world_plane, _UNITS[46].world_x, _UNITS[46].world_y);

    if(_unit_stack_count > 0)
    {
        DLOG("(_unit_stack_count > 0)");
        // j_OVL_DrawStackSelect(OVL_STKUnitCards_Lft, OVL_STKUnitCards_Top)
        Main_Screen_Draw_Unit_Window(_unit_window_start_x, _unit_window_start_y);
        // j_OVL_DrawUnitActnBtns
        // TODO  Main_Screen_Draw_Unit_Action_Buttons();
    }
    else
    {
        DLOG("(_unit_stack_count <= 0)");
        // AKA  j_OVL_DrawDeselectIMG();
        Main_Screen_Draw_Summary_Window();

        // AKA  j_OVL_DrawLockedActns();
        // Main_Screen_Draw_Locked_Actions();

        // AKA  j_OVL_DrawNextTurnBtn();
        Main_Screen_Draw_Next_Turn_Button();
    }

    // AKA  OVL_DrawGoldnMana();
    Main_Screen_Draw_Status_Window();

    // if(OVL_StackHasPath == ST_TRUE)
    // j_OVL_DrawPath(OVL_Path_Length, OVL_Path_Xs, OVL_Path_Ys);

    Main_Screen_Draw_Game_Buttons();

    // OVL_DrawStackMoves

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Draw_Do_Draw()\n", __FILE__, __LINE__);
#endif
}

// WZD o063p05
void Unit_Window_Draw_Unit_Picture(int16_t x, int16_t y, int16_t unit_stack_unit_idx, int16_t flag)
{
    int16_t unit_owner_idx;
    int16_t unit_colored_backgrounds_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Window_Draw_Unit_Picture(x = %d, y = %d, unit_stack_unit_idx = %d, flag = %d)\n", __FILE__, __LINE__, x, y, unit_stack_unit_idx, flag);
#endif

    /*
        if flag is 0 / FALSE / None, skip to draw
        if unit owner is undefined, skip to draw
        ? invalid test and jump for TRUE ?
        ? maybe not &&, because two different jump locations ?
        ...
        WTF?
        if owner is player 5
            set banner to 5
            set owner banner to 5
        ? generalization of neutral player and neutral planner banner ?
    */
    if(flag != 0)
    {
        unit_owner_idx = _UNITS[unit_stack_unit_idx].owner_idx;
        if(unit_owner_idx != ST_UNDEFINED)
        {
            if(unit_owner_idx != NEUTRAL_PLAYER_IDX)
            {
                unit_colored_backgrounds_idx = _players[unit_owner_idx].Banner;
            }
            else
            {
                unit_colored_backgrounds_idx = 5;
                _players[unit_owner_idx].Banner = 5;
            }
        }

        FLIC_Draw(x, y, _unit_colored_backgrounds_seg[unit_colored_backgrounds_idx]);
    }

    Unit_Window_Draw_Unit_StatFig(x, y, unit_stack_unit_idx, flag);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Unit_Window_Draw_Unit_Picture(x = %d, y = %d, unit_stack_unit_idx = %d, flag = %d)\n", __FILE__, __LINE__, x, y, unit_stack_unit_idx, flag);
#endif
}


// WZD o063p05
// AKA OVL_DrawUnitImage()
// pict_seg is 
void Unit_Window_Draw_Unit_StatFig(int16_t x, int16_t y, int16_t unit_idx, int16_t flag)
{
    int16_t unit_type_idx;
    int16_t unit_owner_idx;
    int16_t banner;
    uint16_t itr_banner_colors;
#ifdef STU_DEBUG
    SAMB_ptr pict_seg;
    int16_t pict_seg_width;
    int16_t pict_seg_height;
    uint16_t itr_pict_seg;
    uint16_t itr_UnitDraw_WorkArea;
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Window_Draw_Unit_StatFig()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: unit_idx: %d\n", __FILE__, __LINE__, unit_idx);
#endif
    // unit_type_idx = _UNITS[unit_idx].type;
    // unit_type_idx = 0;  // ? UNITS1.LBX, Entry 0 "Dwarf" BRAX ?
    // unit_type_idx = 35;  // ? UNITS1.LBX, Entry 34  "Trireme" ?
    unit_type_idx = _UNITS[unit_idx].type;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: unit_type_idx: %d\n", __FILE__, __LINE__, unit_type_idx);
#endif
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: _unit_type_table[unit_type_idx].Name: %s\n", __FILE__, __LINE__, _unit_type_table[unit_type_idx].Name);
// #endif


#ifdef STU_DEBUG
    pict_seg = _unit_type_table[unit_type_idx].pict_seg;
    pict_seg_width = GET_2B_OFS(pict_seg, 0);
    pict_seg_height = GET_2B_OFS(pict_seg, 2);
    dbg_prn("DEBUG: [%s, %d]: pict_seg: %p\n", __FILE__, __LINE__, pict_seg);
    dbg_prn("DEBUG: [%s, %d]: pict_seg_width: %d\n", __FILE__, __LINE__, pict_seg_width);
    dbg_prn("DEBUG: [%s, %d]: pict_seg_height: %d\n", __FILE__, __LINE__, pict_seg_height);
#endif


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: FLIC_GET_CURRENT_FRAME(_unit_type_table[unit_type_idx].pict_seg): %d\n", __FILE__, __LINE__, FLIC_GET_CURRENT_FRAME(_unit_type_table[unit_type_idx].pict_seg));
#endif
    FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 0);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: FLIC_GET_CURRENT_FRAME(_unit_type_table[unit_type_idx].pict_seg): %d\n", __FILE__, __LINE__, FLIC_GET_CURRENT_FRAME(_unit_type_table[unit_type_idx].pict_seg));
#endif
    /*
        weird bit of business with branch on flag
        but paths are code equivalent
        maybe related to setting frame 0 or 1
    */
    FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 1);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: FLIC_GET_CURRENT_FRAME(_unit_type_table[unit_type_idx].pict_seg): %d\n", __FILE__, __LINE__, FLIC_GET_CURRENT_FRAME(_unit_type_table[unit_type_idx].pict_seg));
#endif

    // WZD s30p13
    // src  _unit_type_table[unit_type_idx].pict_seg
    // dst  UnitDraw_WorkArea
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: GFX_Swap_Seg: %p\n", __FILE__, __LINE__, GFX_Swap_Seg);
    dbg_prn("DEBUG: [%s, %d]: _unit_type_table[unit_type_idx].pict_seg: %p\n", __FILE__, __LINE__, _unit_type_table[unit_type_idx].pict_seg);
    dbg_prn("DEBUG: [%s, %d]: UnitDraw_WorkArea: %p\n", __FILE__, __LINE__, UnitDraw_WorkArea);
#endif
    Draw_Picture_To_Bitmap(_unit_type_table[unit_type_idx].pict_seg, UnitDraw_WorkArea);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: UnitDraw_WorkArea: %p\n", __FILE__, __LINE__, UnitDraw_WorkArea);
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(UnitDraw_WorkArea, 0): %d\n", __FILE__, __LINE__, GET_2B_OFS(UnitDraw_WorkArea, 0));
    dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(UnitDraw_WorkArea, 2): %d\n", __FILE__, __LINE__, GET_2B_OFS(UnitDraw_WorkArea, 2));
#endif

#ifdef STU_DEBUG

    for(itr_UnitDraw_WorkArea = 0; itr_UnitDraw_WorkArea < 18*16; itr_UnitDraw_WorkArea++)
    {
        dbg_prn("%02x\n", *(UnitDraw_WorkArea + 16 + itr_UnitDraw_WorkArea));
    }

#endif


    unit_owner_idx = (int16_t)_UNITS[unit_idx].owner_idx;

    if(unit_owner_idx != NEUTRAL_PLAYER_IDX)
    {
        banner = (int16_t)_players[unit_owner_idx].Banner;
    }
    else
    {
        banner = 5;
    }

    // for(itr_banner_colors = 0; itr_banner_colors < 5; itr_banner_colors++)
    // {
    //     LBX_IMG_ColorReplace(UnitDraw_WorkArea, itr_banner_colors + 214, COL_Banners_P0[itr_banner_colors * 5]);
    // }




    /*
        flag: {0,1,2,3}
            draw Unit Enchantment Outline
        Invisible:
            ?
        ? Inactive / Finished / Patrol ?
            ?
    */
    // TODO  if(flag == 0 || flag == 1 || flag == 2 || flag == 3)
    // TODO  {
    // TODO      if(Unit_Is_Invisible() == ST_TRUE)
    // TODO      {
    // TODO          LBX_IMG_ClearGraphic(UnitDraw_WorkArea, 233);
    // TODO          LBX_IMG_OutlineOvr(UnitDraw_WorkArea, 1);
    // TODO      }
    // TODO  
    // TODO      if(flag != 3 && 
    // TODO      _UNITS[unit_idx].Status == 1 /* US_Patrol */ && 
    // TODO      Unit_Is_Invisible() != ST_FALSE  && 
    // TODO      unit_owner_idx == _human_player_idx)
    // TODO      {
    // TODO          LBX_IMG_Grayscale(UnitDraw_WorkArea, 1);
    // TODO      }
    // TODO  
    // TODO      UNIT_Draw_UE_Outline(unit_idx);
    // TODO  }


    DLOG("Draw_Picture(x+1, y+1, UnitDraw_WorkArea);");
    // ? UnitDraw_WorkArea = 00000192402C352C ?
    Draw_Picture(x+1, y+1, UnitDraw_WorkArea);


    /*
        BEGIN: Unit Status - Icon/Text
    */
    if(unit_owner_idx == _human_player_idx)
    {

    }
    /*
        END: Unit Status - Icon/Text
    */

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Unit_Window_Draw_Unit_StatFig()\n", __FILE__, __LINE__);
#endif

}



/* 
    WIZARDS.EXE  ovr064
*/

// WZD o64p01
void Allocate_Reduced_Map__1(void)
{
    _reduced_map_seg = Allocate_First_Block(_screen_seg, 303);  // 303 * 16 = 4848 bytes

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _reduced_map_seg: %p\n", __FILE__, __LINE__, _reduced_map_seg);
#endif

}

// WZD o064p04
void Main_Screen_Draw_Summary_Window(void)
{
    int16_t gold;
    int16_t food;
    int16_t mana;
    uint8_t colors[16] = {0};
    int16_t active_event_id;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw_Summary_Window()\n", __FILE__, __LINE__);
#endif

    gold = 0;
    food = 0;
    mana = 0;



// mov     [_help_entries.Entry_Index+96h], -1
// mov     [_help_entries.Entry_Index+0A0h], -1
// mov     [_help_entries.Entry_Index+0F0h], -1

// TODO      // if(g_ActiveStack_UnitCount > 0)
// TODO      if(_unit_stack_count > 0)
// TODO      {
// TODO          goto Done;
// TODO      }

// mov     [_help_entries.Entry_Index+46h], -1
// mov     [_help_entries.Entry_Index+50h], -1
// mov     [_help_entries.Entry_Index+5Ah], -1
// mov     [_help_entries.Entry_Index+64h], -1
// mov     [_help_entries.Entry_Index+96h], HLP_GOLD_INCOME
// mov     [_help_entries.Entry_Index+0A0h], HLP_FOOD_RESERVE
// mov     [_help_entries.Entry_Index+0F0h], HLP_MANA_INCOME

    FLIC_Draw(240, 76, deselect_background);



//     // Get_Incomes(_human_player_idx, &gold, &food, &mana);
    gold = -12;
    food = 34;
    mana = 56;


// // // mov     [bp+Color_0], 198
// // // mov     [bp+Color_1], 198
// // // mov     [bp+Color_2], 198
// // // lea     ax, [bp+Color_0]
// //     Color_0 = 198;
// //     Color_1 = 198;
// //     Color_2 = 198;
// // VGA_SetFontnColor(0, Color_0, Color_1, Color_2);
    colors[0] = 198;
    colors[1] = 198;
    colors[2] = 198;
    Set_Font_Colors_15(0, colors);


    Set_Outline_Color(0);



    if(gold >= 0)
    {
        Set_Font_Style4(0, 4, 0, 0);
        Set_Alias_Color(184);
    }
    else
    {
        if(cycle_incomes == ST_UNDEFINED)
        {
            Set_Font_Style4(0, 2, 0, 0);
        }
        else
        {
            Set_Font_Style4(0, 15, 0, 0);
        }
    }
    Print_Integer_Right(273, 101, gold);
    Print(278, 101, cstr_Gold);


    if(food >= 0)
    {
        Set_Font_Style4(0, 4, 0, 0);
        Set_Alias_Color(184);
    }
    else
    {
        if(cycle_incomes == ST_UNDEFINED)
        {
            Set_Font_Style4(0, 2, 0, 0);
        }
        else
        {
            Set_Font_Style4(0, 15, 0, 0);
        }
    }
    Print_Integer_Right(273, 133, food);
    Print(278, 133, cstr_Food);


    if(mana >= 0)
    {
        Set_Font_Style4(0, 4, 0, 0);
        Set_Alias_Color(184);
    }
    else
    {
        if(cycle_incomes == ST_UNDEFINED)
        {
            Set_Font_Style4(0, 2, 0, 0);
        }
        else
        {
            Set_Font_Style4(0, 15, 0, 0);
        }
    }
    Print_Integer_Right(273, 165, mana);
    Print(278, 165, cstr_Mana);



    Set_Font_Spacing_Width(1);

    // ?
    active_event_id = 0;

    // ?
    // for(itr = 0; itr < 6; itr++) { /* int16_t array[6] */ array[itr] = ST}_UNDEFINED; }

    active_event_id = ST_UNDEFINED;

// mov     [_help_entries.Entry_Index+0B4h], 0FFFFh
// mov     [_help_entries.Entry_Index+0BEh], 0FFFFh
// mov     [_help_entries.Entry_Index+0C8h], 0FFFFh
// mov     [_help_entries.Entry_Index+0D2h], 0FFFFh
// mov     [_help_entries.Entry_Index+0DCh], 0FFFFh
// mov     [_help_entries.Entry_Index+0E6h], 0FFFFh



    // TBL_Events
    // s_EVENT_DATA.Conjunction_Sorcery.Status], 2
    // s_EVENT_DATA.Conjunction_Chaos.Status], 2
    // s_EVENT_DATA.Conjunction_Nature.Status], 2
    // s_EVENT_DATA.Bad_Moon.Status], 2
    // s_EVENT_DATA.Good_Moon.Status], 2
    // s_EVENT_DATA.Mana_Short.Status], 2

    if(_events_table[92] == 2)
    if(_events_table[84] == 2)
    if(_events_table[88] == 2)
    if(_events_table[80] == 2)
    if(_events_table[76] == 2)
    if(_events_table[96] == 2)




    /*
        BEGIN: Conjunction - Sorcery, Chaos, Nature
    */
    // TBL_Events
    // s_EVENT_DATA.Conjunction_Sorcery.Status], 2
    if(_events_table[92] == 2)
    {
        colors[0] = 0;
        colors[1] = 172;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style4(0, 15, 0, 0);
        Print_Centered(280, 157, cstr_Conjunction);
        active_event_id = 0;
    }
    // TBL_Events
    // s_EVENT_DATA.Conjunction_Chaos.Status], 2
    if(_events_table[84] == 2)
    {
        colors[0] = 0;
        colors[1] = 201;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style4(0, 15, 0, 0);
        Print_Centered(280, 158, cstr_Conjunction);
        active_event_id = 1;
    }
    // TBL_Events
    // s_EVENT_DATA.Conjunction_Nature.Status], 2
    if(_events_table[88] == 2)
    {
        colors[0] = 0;
        colors[1] = 217;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style4(0, 15, 0, 0);
        Print_Centered(280, 158, cstr_Conjunction);
        active_event_id = 2;
    }
    /*
        END: Conjunction - Sorcery, Chaos, Nature
    */

    // TBL_Events
    // s_EVENT_DATA.Bad_Moon.Status], 2
    if(_events_table[80] == 2)
    {
        colors[0] = 0;
        colors[1] = 9;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style4(0, 15, 0, 0);
        Print_Centered(280, 158, cstr_Bad_Moon);
        active_event_id = 3;
    }

    // TBL_Events
    // s_EVENT_DATA.Good_Moon.Status], 2
    if(_events_table[76] == 2)
    {
        colors[0] = 0;
        colors[1] = 15;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style4(0, 15, 0, 0);
        Print_Centered(280, 158, cstr_Good_Moon);
        active_event_id = 4;
    }

    // TBL_Events
    // s_EVENT_DATA.Mana_Short.Status], 2
    if(_events_table[96] == 2)
    {
        colors[0] = 0;
        colors[1] = 123;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style4(0, 15, 0, 0);
        Print_Centered(280, 158, cstr_Mana_Short);
        active_event_id = 5;
    }

    // ? if(_SI > -1 ) ?
    if(active_event_id == ST_UNDEFINED)
    {
// mov     ax, si
// mov     dx, 10
// imul    dx
// mov     dx, si
// add     dx, HLP_BLUE_CONJUNCTION
// mov     bx, ax
// mov     [(_help_entries.Entry_Index+0B4h)+bx], dx
// mov     ax, si
// mov     dx, 10
// imul    dx
// mov     bx, ax
// mov     [(_help_entries.Left+0B4h)+bx], 260
// mov     ax, si
// mov     dx, 10
// imul    dx
// mov     bx, ax
// mov     [(_help_entries.Top+0B4h)+bx], 157
// mov     ax, si
// mov     dx, 10
// imul    dx
// mov     bx, ax
// mov     [(_help_entries.Right+0B4h)+bx], 300
// mov     ax, si
// mov     dx, 10
// imul    dx
// mov     bx, ax
// mov     [(_help_entries.Bottom+0B4h)+bx], 163
    }


    if(cycle_incomes != ST_UNDEFINED)
    {
        // TODO  Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 3);
    }


// ? Error if alone or unsued/unreferenced ? Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Draw_Summary_Window()\n", __FILE__, __LINE__);
#endif

}


// WZD o064p05
void Main_Screen_Draw_Next_Turn_Button(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw_Next_Turn_Button()\n", __FILE__, __LINE__);
#endif

//     // [_help_entries.Entry_Index+6Eh], -1
// 
//     if(all_units_moved == ST_TRUE)
//     {
//         // [_help_entries.Entry_Index+6Eh], HLP_NEXT_TURN
// 
//         FLIC_Draw(240, 173, next_turn_background_seg);
// 
//         // TODO j_OVL_HideActiveStack
// 
//         if(g_Current_Screen == ST_UNDEFINED)
//         {
//             FLIC_Draw(246, 178, next_turn_button_seg);
//         }
//         else
//         {
//             // TODO j_GAME_Animate_UEs
//         }
//     }

    FLIC_Draw(240, 173, next_turn_background_seg);
    FLIC_Draw(246, 178, next_turn_button_seg);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Draw_Next_Turn_Button()\n", __FILE__, __LINE__);
#endif

}

// WZD o64p09
void Unit_Window_Picture_Coords(int16_t unit_stack_unit_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Window_Picture_Coords(unit_stack_unit_idx = %d, *x1 =%d, *y1 = %d, *x2 = %d, *y2 = %d)\n", __FILE__, __LINE__, unit_stack_unit_idx, *x1, *y1, *x2, *y2);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _unit_window_start_x: %d\n", __FILE__, __LINE__, _unit_window_start_x);
    dbg_prn("DEBUG: [%s, %d]: _unit_window_start_y: %d\n", __FILE__, __LINE__, _unit_window_start_y);
    dbg_prn("DEBUG: [%s, %d]: (unit_stack_unit_idx \\% 3): %d\n", __FILE__, __LINE__, (unit_stack_unit_idx % 3));
    dbg_prn("DEBUG: [%s, %d]: ((unit_stack_unit_idx \\% 3) * 23): %d\n", __FILE__, __LINE__, ((unit_stack_unit_idx % 3) * 23));
    dbg_prn("DEBUG: [%s, %d]: ((unit_stack_unit_idx / 3) * 29): %d\n", __FILE__, __LINE__, ((unit_stack_unit_idx / 3) * 29));
#endif

    *x1 = _unit_window_start_x + ((unit_stack_unit_idx % 3) * 23);
    *y1 = _unit_window_start_y + ((unit_stack_unit_idx / 3) * 29);
    *x2 = *x1 + 22;
    *y2 = *y1 + 28;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Unit_Window_Picture_Coords(unit_stack_unit_idx = %d, *x1 =%d, *y1 = %d, *x2 = %d, *y2 = %d)\n", __FILE__, __LINE__, unit_stack_unit_idx, *x1, *y1, *x2, *y2);
#endif
}
