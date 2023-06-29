

#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

#include "MoX_Data.H"
#include "MoX_GAM.H"
#include "UNITTYPE.H"

#include "MoM_main.H"

#include "MainScr.H"
#include "MainScr_Maps.H"

#include "Allocate.H"
#include "FLIC_Draw.H"
#include "Fields.H"
#include "Fonts.H"
#include "Graphics.H"
#include "Input.H"
#include "LBX_Load.H"
#include "Mouse.H"
#include "UnitMove.H"
#include "Video.H"  /* Set_Page_Off() */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


extern void Pump_Events(void);


void Main_Screen_Load_Pictures(void);

/*
    WIZARDS.EXE  ovr057
*/
// WZD o57p01
// PUBLIC void Main_Screen(void);
// WZD o57p02
void Main_Screen_Add_Fields(void);
// WZD o57p03
void Add_Unit_Action_Fields(void);
// WZD o57p04
void Main_Screen_Draw_Unit_Action_Buttons(void);
// WZD o57p05
void Add_Game_Button_Fields(void);
// WZD o57p06
void Main_Screen_Draw_Game_Buttons(void);
// WZD o57p07
void Add_Unit_Window_Fields(void);
// WZD o57p08
void Main_Screen_Draw(void);
// WZD o57p09
void Main_Screen_Reset(void);
// WZD o57p10
void IDK_UnitMoves_and_PlanarTravel(void);
/*
    WIZARDS.EXE  ovr058
*/
// WZD o58p01
void Main_Screen_Draw_Unit_Window(int16_t start_x, int16_t start_y);
/*
    WIZARDAS.EXE  ovr062
*/
// WZD o62p04
void Build_Unit_Stack(int16_t player_idx, int16_t world_plane, int16_t world_x, int16_t world_y);
/*
    WIZARDAS.EXE  ovr063
*/
// WZD o063p01
void Main_Screen_Draw_Status_Window(void);
// WZD o063p02
void Main_Screen_Draw_Do_Draw(int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t x_pos, int16_t y_pos, int16_t player_idx);
// WZD o063p04
void Cycle_Unit_Enchantment_Animation(void);
// WZD o063p05
void Unit_Window_Draw_Unit_Picture(int16_t x, int16_t y, int16_t unit_stack_unit_idx, int16_t flag);
// WZD o063p05
// AKA OVL_DrawUnitImage()
void Unit_Window_Draw_Unit_StatFig(int16_t x, int16_t y, int16_t unit_idx, int16_t flag);
// WZD o63p12
int16_t OVL_GetStackHMoves(void);
/*
    WIZARDS.EXE  ovr064
*/
// WZD o64p01
void Allocate_Reduced_Map__1(void);
// WZD o064p04
void Main_Screen_Draw_Summary_Window(void);
// WZD o064p05
void Main_Screen_Draw_Next_Turn_Button(void);
// WZD o64p08
void Main_Screen_Draw_Unit_Action_Locked_Buttons(void);
// WZD o64p09
void Unit_Window_Picture_Coords(int16_t unit_stack_unit_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2);




// dseg:2E10                                                 BEGIN: Main Screen

// WZD dseg:2E10
// CRP_DBG_Alt_K_State dw 0
// WZD dseg:2E12
// DBG_Alt_A_State dw 0
// WZD dseg:2E14
int16_t UU_first_turn_done_flag = ST_FALSE;
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

// WZD dseg:305B  align 2

// WZD dseg:305C                                                 ? BEGIN: Draw Unit Window ?

// WZD dseg:305C 
int16_t WP_Quality_Anim_Stg;
// WZD dseg:305E
// AKA UE_Anim_State
int16_t unit_enchantment_animation_count;
// WZD dseg:3060
// AKA UE_Anim_Skip
int16_t unit_enchantment_animation_flag;  


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
int16_t OVL_Path_Length;
// WZD dseg:C068
int16_t OVL_StackHasPath;

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


// WZD dseg:C080 dw 0
// WZD dseg:C082 _unit_window_start_y dw 0
// WZD dseg:C084 _unit_window_start_x dw 0
// WZD dseg:C086 _reduced_map_seg dw 0
// WZD dseg:C088 NIU_MainScreen_local_flag dw 0
// WZD dseg:C08A SND_Bkgrnd_Track dw 0
// WZD dseg:C08C word_42B2C dw 0


// WZD dseg:C08E
int16_t STK_HMoves_Left;
    // XREF  OVL_GetStackHMoves+D3      mov     ax, [STK_HMoves_Left]                
    // XREF  OVL_GetStackHMoves:@@Done  mov     [STK_HMoves_Left], _SI_MovementPoints
    // XREF  STK_GetMovesLeft+A3        mov     ax, [STK_HMoves_Left]                
    // XREF  STK_GetMovesLeft:loc_7E717 mov     [STK_HMoves_Left], si                
    // XREF  sub_56B56+132              mov     [STK_HMoves_Left], ax                
    // XREF  sub_56B56+135              mov     ax, [STK_HMoves_Left]                


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
// WZD dseg:31A8 42 00                   UNIT_Building_Mark db 'B',0
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
// int16_t _minimap_grid_x;
int64_t _minimap_grid_x;
// WZD dseg:9744
// int16_t _minimap_grid_y;
int64_t _minimap_grid_y;

// WZD dseg:9746 CRP_OVL_MapWindowY dw 0
// WZD dseg:9748 CRP_OVL_MapWindowX dw 0


// WZD dseg:974A
int16_t _prev_world_y;
// WZD dseg:974C
int16_t _prev_world_x;

// WZD dseg:974E 
// int16_t _main_map_grid_x;
int64_t _main_map_grid_x;
// WZD dseg:9750 
int64_t _main_map_grid_y;
// int16_t _main_map_grid_y;


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





/*
    WIZARDS.EXE  ovr057

  Name, Start,  End, R, W, X, D, L, Align, Base,   Type,   Class, AD,   es,   ss,   ds,  fs,  gs
ovr057,  0000, 1C77, ?, ?, ?, ., L,  para, 4FB6, public, OVERLAY, 16, FFFF, FFFF, 36AA, N/A, N/A

0x0000
0x1C77

0x4FB6

0x36AA


; Segment type: Pure code
segment ovr057 para public 'OVERLAY' use16
assume cs:ovr057
assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing
; Attributes: bp-based frame
proc Main_Screen far
? 00042F20                 "Current offset in the input file"
? 0004FB60: Main_Screen    "Current location"


o57p01  Main_Screen()
o57p02  Main_Screen_Add_Fields()
o57p03  Add_Unit_Action_Fields()
o57p04  Main_Screen_Draw_Unit_Action_Buttons()
o57p05  Add_Game_Button_Fields()
o57p06  Main_Screen_Draw_Game_Buttons()
o57p07  Add_Unit_Window_Fields()
o57p08  Main_Screen_Draw()
o57p09  Main_Screen_Reset()
o57p10  ? IDK_UnitMoves_and_PlanarTravel() ? 
*/

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

    main_lock_done_button = LBX_Load(main_lbx_file, 12);
    main_lock_patrol_button = LBX_Load(main_lbx_file, 13);
    main_lock_wait_button = LBX_Load(main_lbx_file, 14);
    main_lock_build_button = LBX_Load(main_lbx_file, 15);
    main_lock_purify_button = LBX_Load(main_lbx_file, 43);

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



// WZD 057p01
void Main_Screen(void)
{
    int16_t hotkey_idx_Alt_P;
    int16_t hotkey_idx_U;
// strP= byte ptr -6Ch
    int16_t hotkey_idx_F9;
    int16_t hotkey_idx_F8;
    int16_t hotkey_idx_F7;
    int16_t hotkey_idx_F6;
    int16_t hotkey_idx_F5;
    int16_t hotkey_idx_F4;
    int16_t hotkey_idx_F3;
    int16_t hotkey_idx_F2;
    int16_t hotkey_idx_F1;
// Local_0= word ptr -50h
// DBG_Alt_A__TurnCount= word ptr -4Eh
    int16_t hotkey_idx_Alt_A;
    int16_t hotkey_idx_PgDn;
    int16_t hotkey_idx_Down;
    int16_t hotkey_idx_End;
    int16_t hotkey_idx_Right;
    int16_t hotkey_idx_Left;
    int16_t hotkey_idx_PgUp;
    int16_t hotkey_idx_Home;
    int16_t hotkey_idx_Up;
    int16_t gold;
    int16_t food;
    int16_t mana;
    int16_t hotkey_idx_Alt_N;
    int16_t hotkey_idx_Shift_3;
    int16_t hotkey_idx_Alt_K;
    int16_t hotkey_idx_F10;
    int16_t hotkey_idx_Alt_T;
    int16_t unit_stack_hmoves;
    int16_t unit_stack_world_y;
    int16_t unit_stack_world_x;
    int16_t IDK_unit_stack_in_view;
    int16_t entity_idx;  // AKA "input"
    int16_t hotkey_idx_Q;
    int16_t hotkey_idx_C;
    int16_t hotkey_idx_Shift_2;
    int16_t hotkey_idx_Shift_1;
    int16_t Unused_Button_Index;
    int16_t hotkey_idx_X;
// Unit_Y= word ptr -14h
// Unit_X= word ptr -12h
// Bottom@= word ptr -10h
// Right@= word ptr -0Eh
    int16_t target_world_y;
    int16_t target_world_x;
    int16_t unit_idx;
// var_ConfirmDialogResponse= word ptr -6
    int16_t Stack_Index;
    int16_t leave_screen_flag;
    
    int16_t screen_changed;
    int16_t input_field_idx;
    int16_t mouse_x;
    int16_t mouse_y;

    int16_t itr_units;


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: MainScreen - InitialMapValues\n", __FILE__, __LINE__);
        dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
        dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
        dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
        dbg_prn("DEBUG: [%s, %d]: _prev_world_y: %d\n", __FILE__, __LINE__, _prev_world_y);
        dbg_prn("DEBUG: [%s, %d]: _main_map_grid_x: %d\n", __FILE__, __LINE__, _main_map_grid_x);
        dbg_prn("DEBUG: [%s, %d]: _main_map_grid_y: %d\n", __FILE__, __LINE__, _main_map_grid_y);
#endif


//     // HACK:
//     _unit = 46;  // ¿ Where'd I get this _unit from or the x,y's ? 
//     // Build_Unit_Stack(0, 0, 18, 17);
//     Build_Unit_Stack(0, 0, _UNITS[_unit].world_x, _UNITS[_unit].world_y);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: _unit: %d\n", __FILE__, __LINE__, _unit);
//     dbg_prn("DEBUG: [%s, %d]: _unit_stack_count: %d\n", __FILE__, __LINE__, _unit_stack_count);
//     dbg_prn("DEBUG: [%s, %d]: _unit_stack[0].unit_idx: %d\n", __FILE__, __LINE__, _unit_stack[0].unit_idx);
//     dbg_prn("DEBUG: [%s, %d]: _unit_stack[0].active: %d\n", __FILE__, __LINE__, _unit_stack[0].active);
// #endif


    // WZD main() |-> WZD_Load_Init_MainGame() |-> Main_Screen_Load_Pictures()
    if(main_screen_loaded == ST_FALSE)
    {
        Main_Screen_Load_Pictures();
        main_screen_loaded = ST_TRUE;
    }



    Reset_Window();
    Clear_Fields();
    // NIU_MainScreen_local_flag = 1;  // only XREF Main_Screen(), sets TRUE, never tests
    // j_Allocate_Reduced_Map__2();  byte-identical to LBX_Minimap_Alloc, should not exist
    Reset_Draw_Active_Stack();  // AKA j_OVL_ResetStackDraw()
    Set_Outline_Color(0);
    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    // Disable_Redraw_Function();
    // Set_Redraw_Function(j_Main_Screen_Draw, 1);
    // _unit_window_start_x = 247;  // AKA OVL_STKUnitCards_Lft
    // _unit_window_start_y = 79;  // AKA OVL_STKUnitCards_Top
    // G_Some_OVL_Var_1 = 0;  // ? ST_FALSE ?
    // CRP_OverlandVar_2 = 0;  // ? ST_FALSE ?
    // CRP_OVL_Obstacle_Var1 = 0;  // ? ST_FALSE ?
    // OVL_MapVar3 = 1;  // ? ST_TRUE ?
    Reset_Map_Draw();
    // TODO  j_MainScr_Prepare_Reduced_Map();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    Set_Mouse_List(1, mouse_list_default);  // ~== Set_Mouse_List_MainScr() |-> Set_Mouse_List(1, mouse_list_main/default/normal/arrow);
    // TODO  j_STK_GetExtraActions();
    // if (CRP_OverlandVar_3 != 1) { CRP_OverlandVar_3 = 0; }  // ? ST_TRUE ST_FALSE ?
    // if (CRP_OverlandVar_4 != 1) { CRP_OverlandVar_4 = 0; }  // ? ST_TRUE ST_FALSE ?
    screen_changed = ST_FALSE;
    // Local_0 = 0;  // ? ST_FALSE ?
    Set_Input_Delay(1);
    // TODO  Reset_Cycle_Palette_Color()  AKA VGA_BlinkReset()
    // TODO  Deactivate_Help_List();
    // TODO  Main_Screen_Help();  // ? |-> WZD s104 HLP_Load_OVL_View() |-> WZD s10 LBXR_DirectLoader() ?
    // DBG_Alt_A__TurnCount = -1
    Main_Screen_Draw();
    PageFlip_FX();



    /*
        BEGIN: Screen-Loop
    */

    leave_screen_flag = ST_FALSE;
    while(leave_screen_flag == ST_FALSE)
    {
        // TODO  CLK_Save();

        /*
            BEGIN: Add Fields
        */
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
        // hotkey_idx_Home = Add_Hot_Key(KP_Home);
        // hotkey_idx_Up = Add_Hot_Key(KP_Up);
        // hotkey_idx_PgUp = Add_Hot_Key(KP_PgUp);
        // hotkey_idx_Left = Add_Hot_Key(KP_Left);
        // hotkey_idx_Right = Add_Hot_Key(KP_Right);
        // hotkey_idx_End = Add_Hot_Key(KP_End);
        // hotkey_idx_Down = Add_Hot_Key(KP_Down);
        // hotkey_idx_PgDn = Add_Hot_Key(KP_PgDn);
        hotkey_idx_F10 = Add_Hot_Key(ST_KEY_F10);
        hotkey_idx_F1 = Add_Hot_Key(ST_KEY_F1);
        hotkey_idx_F2 = Add_Hot_Key(ST_KEY_F2);
        hotkey_idx_F3 = Add_Hot_Key(ST_KEY_F3);
        hotkey_idx_F4 = Add_Hot_Key(ST_KEY_F4);
        hotkey_idx_F5 = Add_Hot_Key(ST_KEY_F5);
        hotkey_idx_F6 = Add_Hot_Key(ST_KEY_F6);
        hotkey_idx_F7 = Add_Hot_Key(ST_KEY_F7);
        hotkey_idx_F8 = Add_Hot_Key(ST_KEY_F8);
        hotkey_idx_F9 = Add_Hot_Key(ST_KEY_F9);
        hotkey_idx_Alt_K = Add_Multi_Hot_Key_Field("K");
        hotkey_idx_Alt_A = Add_Multi_Hot_Key_Field("A");
        /*
            END: Add Fields
        */


        input_field_idx = Get_Input();





/*
    BEGIN: Check Input against Fields
*/


        // Advisor - Surveyor
        if(input_field_idx == hotkey_idx_F1)
        {
            // Advisor_Screen(0);
            // Set_Redraw_Function(Main_Screen_Draw, 1);
            // Main_Screen_Reset();
        }
        // Advisor - Cartographer
        if(input_field_idx == hotkey_idx_F2)
        {
            // Advisor_Screen(1);
            // Set_Redraw_Function(Main_Screen_Draw, 1);
            // Main_Screen_Reset();
        }
        // Advisor - Apprentice
        if(input_field_idx == hotkey_idx_F3)
        {
            // Advisor_Screen(2);
            // Set_Redraw_Function(Main_Screen_Draw, 1);
            // Main_Screen_Reset();
        }
        // Advisor - Historian
        if(input_field_idx == hotkey_idx_F4)
        {
            // Advisor_Screen(3);
            // Set_Redraw_Function(Main_Screen_Draw, 1);
            // Main_Screen_Reset();
        }
        // Advisor - Astrologer
        if(input_field_idx == hotkey_idx_F5)
        {
            // Advisor_Screen(4);
            // Set_Redraw_Function(Main_Screen_Draw, 1);
            // Main_Screen_Reset();
        }
        // Advisor - Chancellor
        if(input_field_idx == hotkey_idx_F6)
        {
            // Advisor_Screen(5);
            // Set_Redraw_Function(Main_Screen_Draw, 1);
            // Main_Screen_Reset();
        }
        // Advisor - Tax Collector
        if(input_field_idx == hotkey_idx_F7)
        {
            // Advisor_Screen(6);
            // Set_Redraw_Function(Main_Screen_Draw, 1);
            // Main_Screen_Reset();
        }
        // Advisor - Grand Vizier
        if(input_field_idx == hotkey_idx_F8)
        {
            // Advisor_Screen(7);
            // Set_Redraw_Function(Main_Screen_Draw, 1);
            // Main_Screen_Reset();
        }
        // Advisor - Mirror
        if(input_field_idx == hotkey_idx_F9)
        {
            // Advisor_Screen(8);
            // Set_Redraw_Function(Main_Screen_Draw, 1);
            // Main_Screen_Reset();
        }

        /* Alt-A   */  /* if(input_field_idx == hotkey_idx_Alt_A) {if(Check_Release_Version()==ST_FALSE){DBG_Alt_A_State=1-DBG_Alt_A_State;}} */
        /* Alt-P   */
        /* Alt-K   */
        /* Shift-3 */
        /* Alt-T   */
        /* Alt-N   */  /* if(input_field_idx == hotkey_idx_Alt_A) {if(Check_Release_Version()==ST_FALSE){DBG_ShowTileInfo=1-DBG_ShowTileInfo;}} */

        // Quick-Save
        if(input_field_idx == hotkey_idx_F10)
        {
            Save_SAVE_GAM(8);
        }

        // empty MACRO / null-function-pointer
        if(input_field_idx == hotkey_idx_X)
        {
            
        }

        // Center Map
        if(input_field_idx == hotkey_idx_C)
        {
            if(_unit_stack_count > 0)
            {
                unit_idx = _unit_stack[0].unit_idx;
                _map_plane = _UNITS[unit_idx].world_plane;
                Center_Map(&_map_x, &_map_y, _UNITS[unit_idx].world_x, _UNITS[unit_idx].world_y, _UNITS[unit_idx].world_plane);
                MainScr_Prepare_Reduced_Map();
                Set_Mouse_List(1, mouse_list_default);
                Reset_Map_Draw();
            }
        }
        
        // ? Quit Action / Unselect Unit ?
        if( (input_field_idx == hotkey_idx_Q) || (input_field_idx == hotkey_idx_U) )
        {
            // _unit_stack_count = 0;
            // OVL_ShowActiveStack();
            // UNIT_DrawPriorities();
            // STK_NoUnitDraw();
            // Set_Entities_On_Map_Window(_curr_world_x, _curr_world_y, _world_plane);
            // Reset_Map_Draw();
            // screen_changed = ST_TRUE;
        }

        /*
            BEGIN: Mouse Click
        */

        // Info Button
        /* if(input_field_idx == _info_button) { SND_LeftClickSound(); _current_screen = scr_Info_Screen; leave_screen_flag = ST_TRUE; } */
        /* if(input_field_idx == _game_button) { SND_LeftClickSound(); _current_screen = scr_Load_Screen; leave_screen_flag = ST_TRUE; } */
        /* if(input_field_idx == _cities_button) { SND_LeftClickSound(); _current_screen = scr_Cities_Screen; leave_screen_flag = ST_TRUE; } */
        /* if(input_field_idx == _magic_button) { SND_LeftClickSound(); _current_screen = scr_Magic_Screen; leave_screen_flag = ST_TRUE; } */
        /* if(input_field_idx == _armies_button) { SND_LeftClickSound(); _current_screen = scr_Armies_Screen; leave_screen_flag = ST_TRUE; } */
        if(input_field_idx == _spells_button)
        {
            // SND_LeftClickSound();
            if(_players[_human_player_idx].Spell_Cast == 0xD6) /* Spell of Return */
            {
                // You may not throw any spells while you are banished.
                // GUI_NearMsgString
                // strcpy()
                // strcat()
                // strcat()
                // GUI_WarningType0()
                // Set_Redraw_Function(Main_Screen_Draw, 1);
            }
            else
            {
                // OVL_ShowActiveStack();
                // UNIT_DrawPriorities();
                // Set_Entities_On_Map_Window(_curr_world_x, _curr_world_y, _world_plane);
                // leave_screen_flag = ST_TRUE;
                // _current_screen = scr_Spellbook_Screen;
            }
        }

        if(input_field_idx == _patrol_button)
        {
            
        }

        if(input_field_idx == _special_button)
        {
            // SND_LeftClickSound();
            // switch(special_action_flag)
            //     -1: no extra action possible
            //      0: road building possible
            //      1: settling possible
            //      2: purifying possible
            //      9: melding
        }

        if(input_field_idx == _done_button)
        {
            
        }

        if(input_field_idx == _wait_button)
        {
            
        }

        if(input_field_idx == _plane_button)
        {
            
        }

        // Left-Click Unit Window Grid Field
        for(Stack_Index = 0; Stack_Index < _unit_stack_count; Stack_Index++)
        {
            if(Unit_Window_Fields[Stack_Index] = input_field_idx)
            {
                // RP_SND_LeftClickSound2();  // byte-identical to SND_LeftClickSound() 
                // IDK_ActiveUnitStack_MovesOrPath_s53150(Stack_Index);
                // Set_Entities_On_Map_Window(_curr_world_x, _curr_world_y, _world_plane);
                // NIU_MainScreen_local_flag = 1;
            }
        }
        
        // Right-Click Unit Window Grid Field
        for(Stack_Index = 0; Stack_Index < _unit_stack_count; Stack_Index++)
        {
            if(Unit_Window_Fields[Stack_Index] = -input_field_idx)
            {
                // OVL_ShowActiveStack();
                // UNIT_DrawPriorities();
                // Set_Entities_On_Map_Window(_curr_world_x, _curr_world_y, _world_plane);
                // Main_Screen_Draw();
                // PageFlip_FX();
                // Unit_Window_Picture_Coords(Stack_Index, &OLft, &OTop, Right@, Bottom@);
                // USW_FullDisplay(_unit_stack[unit_idx].unit_idx, OLft, OTop, OLft+18, OTop+18);
                // Set_Redraw_Function(Main_Screen_Draw, 1);
                // Allocate_Reduced_Map__2();
                // Set_Mouse_List_Normal();
                // Reset_Active_Stack_Draw();
                // UNIT_DrawPriorities();
                // STK_NoUnitDraw();
                // Set_Entities_On_Map_Window(_curr_world_x, _curr_world_y, _world_plane);
                // Reset_Map_Draw();
                // MainScr_Prepare_Reduced_Map();
                // screen_changed = ST_TRUE;
                // Clear_Help_Fields();
                // Main_Screen_Help();
            }
        }

        // Next Turn Button ... and time-stuff ? end of turn wait ?
        // if(
        //     (input_field_idx == _next_turn_button) ||
        //     ((g_TimeStop_PlayerNum > 0) && (g_TimeStop_PlayerNum == _human_player_idx+1)) ||
        //     ((all_units_moved = ST_TRUE) && (Local_0 == 0) && (magic_set.EoT_Wait == ST_FALSE) && (j_IDK_Check_Unit_Status_BUSY() == ST_TRUE))
        // )
        if(input_field_idx == _next_turn_button)
        {
            DLOG("if(input_field_idx == _next_turn_button)");
            /* BIG EFFORT */
        }

        // hotkey_idx_Up
        // hotkey_idx_Home
        // hotkey_idx_PgUp
        // hotkey_idx_Left
        // hotkey_idx_Right
        // hotkey_idx_End
        // hotkey_idx_Down
        // hotkey_idx_PgDn

        /*
            Movement Map Grid Field
                Left-Click
                Right-Click
        */

        /*
            Left-Click Movement Map Grid Field
                In IDA, color #42 (~Gold)
            - only if unit/stack active/selected
        */
        if( (input_field_idx == _main_map_grid_field) && (_unit_stack_count != 0) )
        {
            unit_stack_hmoves = OVL_GetStackHMoves();
            if(unit_stack_hmoves < 1)
            {
                // j_IDK_MainScr_SUA_s553C3()
                // j_UNIT_SetGlobalPath()  // calcs OVL_Path_Length; sets OVL_StackHasPath
                if(OVL_Path_Length < 1)
                {
                    // j_IDK_MainScr_SUA_s553C3()
                }
                // j_Set_Mouse_List_Normal();
                // j_Reset_Map_Draw();
                // NIU_MainScreen_local_flag == 1; // ? ST_TRUE ?
            }
            if(OVL_StackHasPath == ST_TRUE)
            {
                // NIU_MainScreen_local_flag = 1
                unit_idx = _unit_stack[0].unit_idx;
                if(_UNITS[unit_idx].world_plane == _map_plane)
                {
                    unit_stack_world_y = _UNITS[unit_idx].world_y;
                    if(unit_stack_world_y >= _map_y && unit_stack_world_y <= _map_y + MAP_WIDTH)
                    {
                        IDK_unit_stack_in_view = ST_FALSE;
                        unit_stack_world_x = _UNITS[unit_idx].world_x;
                        if(
                            ( unit_stack_world_x >= _map_x && unit_stack_world_x <= (_map_x + MAP_HEIGHT) ) ||
                            ( (unit_stack_world_x + WORLD_WIDTH) >= _map_x && (unit_stack_world_x + WORLD_WIDTH) <= (_map_x + MAP_HEIGHT) )
                        )
                        {
                            unit_stack_world_x = unit_stack_world_x - _map_x;
                            IDK_unit_stack_in_view = ST_TRUE;
                        }
                        if(IDK_unit_stack_in_view == ST_TRUE)
                        {
                            if(_main_map_grid_x == unit_stack_world_x)
                            {
                                unit_stack_world_y = unit_stack_world_y - _map_y;
                                if(_main_map_grid_y == unit_stack_world_y)
                                {
                                    // j_IDK_MainScr_SUA_s553C3(_human_player_idx, &_map_x, &_map_y, &_map_plane, 0, 0, 0)
                                    // CRP_OverlandVar_3 == ST_FALSE;
                                    // OVL_StackHasPath == ST_FALSE;
                                }
                            }


                        }
                    }
                }
            }
            else
            {
                // NIU_MainScreen_local_flag = 1; // ? ST_TRUE ?
                if(all_units_moved == ST_FALSE)
                {
                    target_world_x = (_map_x + _main_map_grid_x) % 60;  // world_x of click
                    target_world_y = _map_y + _main_map_grid_y;         // world_y of click
                    // ovr062
                    // x,y,p for dst; gets src from _UNITS[_unit_stack[0].unit_idx]
                    // if(EarthGateTeleport(target_world_x, target_world_y, _map_plane) == ST_FALSE)
                    // {
                    //     // if ...
                    //     /// j_OVL_CanPlanarTravel() == ST_FALSE
                    //     // _UNITS[_unit_stack[0].unit_idx].world_plane == _map_plane;
                    //     // _UNITS[_unit_stack[0].unit_idx].In_Tower == ST_TRUE;
                    //     // WZD o61p02
                    //     // j_IDK_DoMoveStack_s5336C(target_world_x, target_world_y, _human_player_idx, &_map_x, &_map_y, &_map_plane)
                    //     // else
                    //     // "The selected units cannot move on this plane."
                    //     // ; displays the passed message in a red warning dialog
                    //     // j_GUI_WarningType0(cstrWarnNoPlaneMove)
                    // }
                }
            }
        }



        /*
            Right-Click Movement Map Grid Field
                In IDA, color #45 (~SeaFoamGreen)
            
            Scroll Map
            Select Entity
                Unit
                    Own - Active Stack
                    Other - Unit List Window
                City
                    Own
                        Size
                            0 - Outpost Screen
                            1+ - City Screen
                    Other - Enemy City Window

        */
       if(-input_field_idx == _main_map_grid_field)
       {
            DLOG("if(-input_field_idx == _main_map_grid_field)");
            DLOG("@@RightClickMovementMap");

            // TODO  rename this variable to something more sensible - maybe add more if it gets reused - here ~entity_idx, was "input"
            entity_idx = entities_on_movement_map[( (_main_map_grid_y * MAP_WIDTH) + _main_map_grid_x )];
#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: entity_idx: %d\n", __FILE__, __LINE__, entity_idx);
#endif

            if(entity_idx == ST_UNDEFINED)
            {
                DLOG("if(entity_idx == ST_UNDEFINED)");
                DLOG("ScrollTheMap");
                // _main_map_grid_x,y is now what got updated in Param3,4 way over in Interpret_Mouse_Input() |-> Push_Field_Down()
                // ¿ here, _prev_world_x, y is final destination for scrolling the map ?
                _prev_world_x = _prev_world_x + (_main_map_grid_x - (MAP_WIDTH  / 2));  // ¿ grid x - (map width / 2) = map x ?
                _prev_world_y = _prev_world_y + (_main_map_grid_y - (MAP_HEIGHT / 2));  // ¿ grid y - (map height / 2) = map y ?
#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: _main_map_grid_x: %d\n", __FILE__, __LINE__, _main_map_grid_x);
        dbg_prn("DEBUG: [%s, %d]: _main_map_grid_y: %d\n", __FILE__, __LINE__, _main_map_grid_y);
        dbg_prn("DEBUG: [%s, %d]: _main_map_grid_x - (MAP_WIDTH / 2): %d\n", __FILE__, __LINE__, _main_map_grid_x - (MAP_WIDTH / 2));
        dbg_prn("DEBUG: [%s, %d]: _main_map_grid_y - (MAP_HEIGHT / 2): %d\n", __FILE__, __LINE__, _main_map_grid_y - (MAP_HEIGHT / 2));
        dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
        dbg_prn("DEBUG: [%s, %d]: _prev_world_y: %d\n", __FILE__, __LINE__, _prev_world_y);
        dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
#endif
                IDK_CheckSet_MapDisplay_XY();
#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
        dbg_prn("DEBUG: [%s, %d]: _prev_world_y: %d\n", __FILE__, __LINE__, _prev_world_y);
        dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
#endif
            }
            else
            {
                DLOG("if(entity_idx != ST_UNDEFINED)");

                // RP_SND_LeftClickSound2()

                // entity_idx = abs(entity_idx);
                entity_idx = entity_idx > 0 ? entity_idx : entity_idx*-1;
#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: entity_idx: %d\n", __FILE__, __LINE__, entity_idx);
#endif

                if(entity_idx >= 1000)
                {
                    DLOG("if(entity_idx >= 1000)");

                }
                else
                {
                    DLOG("if(entity_idx < 1000)");
                    if(_UNITS[entity_idx].owner_idx == _human_player_idx)
                    {
                        DLOG("if(_UNITS[entity_idx].owner_idx == _human_player_idx)");
                        // Select Stack
                    }
                    else
                    {
                        DLOG("if(_UNITS[entity_idx].owner_idx != _human_player_idx)");
                        // Enemy Unit - Unit List Screen (Pop-Up)
                    }
                }

            }

       }





        /*
            END: Mouse Click
        */

/*
    END: Check Input against Fields
*/





        if( (leave_screen_flag == ST_FALSE) && (screen_changed == ST_FALSE) )
        {
            Main_Screen_Draw();
            if(_turn == 0)
            {
                // TODO  j_NameStartingCity_Dialog_Popup(0);
                // TODO  Set_Redraw_Function(j_Main_Screen_Draw, 1);
                UU_first_turn_done_flag = ST_TRUE;
            }
            else
            {
                UU_first_turn_done_flag = ST_TRUE;
            }
            // j_nulsub_D50FF();
            // j_nulsub_87A9A();
            PageFlip_FX();
            // TODO  CLK_Wait(1);
        }
        screen_changed = ST_FALSE;

        /*
        PageFlip_FX();
        Main_Screen_Draw();
        Toggle_Pages();
        mouse_x = Pointer_X();
        mouse_y = Pointer_Y();
        Check_Mouse_Shape(mouse_x, mouse_y);
        Draw_Mouse(mouse_x, mouse_y);
        */

        // HACK: hard-coded to get back around to the Windows Message Loop
        leave_screen_flag = ST_TRUE;
        Pump_Events();
    }

    // TODO  Disable_Redraw_Function()
    // TODO  Deactivate_Help_List();
    Reset_Window();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen()\n", __FILE__, __LINE__);
#endif
}


// WZD o57p02
void Main_Screen_Add_Fields(void)
{
    int16_t itr_unit_stack;

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

    if( (_map_x == _prev_world_x) && (_map_y == _prev_world_y) )
    {
        DLOG("Yay - Movement Map Grid Field");
        // (xmin, ymin, box_width, box_height, horizontal_count, vertical_count, *xpos, *ypos, help)
        // TODO  add defines for the map dims/coords
        // TODO  confirm what these values/calculations should end up being
        _main_map_grid_field = Add_Grid_Field(0, 20, 20, 18, 12, 10, &_main_map_grid_x, &_main_map_grid_y, ST_UNDEFINED);
#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: _main_map_grid_field: %d\n", __FILE__, __LINE__, _main_map_grid_field);
        DBG_movement_map_grid_field_idx = _main_map_grid_field;
        strcpy(field_names[_main_map_grid_field], "Movement Map"); // 12+1 char
#endif
    }
    else
    {
        DLOG("Nay - Movement Map Grid Field");
    }

    if( (_map_x == _prev_world_x) && (_map_y == _prev_world_y) )
    {
        DLOG("Yay - Reduced Map Grid Field");
        // screen x, screen y, cell w, cell h, grid w, grid h, grid x, grid y, help
        _minimap_grid_field = Add_Grid_Field(251, 21, 1, 1, 58, 30, &_minimap_grid_x, &_minimap_grid_y, ST_UNDEFINED);
#ifdef STU_DEBUG
        DBG_reduced_map_grid_field_idx = _minimap_grid_field;
        strcpy(field_names[_minimap_grid_field], "Reduced Map"); // 11+1 char
#endif
    }
    else
    {
        DLOG("Nay - Reduced Map Grid Field");
    }

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
// AKA Main_Screen_Draw_Unit_Actions()
// AKA j_OVL_DrawUnitActnBtns()
void Main_Screen_Draw_Unit_Action_Buttons(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw_Unit_Action_Buttons()\n", __FILE__, __LINE__);
#endif

    if(_unit_stack_count > 0)
    {
        FLIC_Set_CurrentFrame(main_done_button, 0);
        FLIC_Draw(246, 176, main_done_button);
        // _help_entries.Entry_Index+46h = HLP_DONE;
        FLIC_Set_CurrentFrame(main_patrol_button, 0);
        FLIC_Draw(280, 176, main_patrol_button);
        // _help_entries.Entry_Index+50h = HLP_PATROL;
        FLIC_Set_CurrentFrame(main_wait_button, 0);
        FLIC_Draw(246, 186, main_wait_button);
        /// _help_entries.Entry_Index+5Ah = ST_UNDEFINED;

        if(special_action_flag != ST_UNDEFINED)
        {
            if(special_action_flag == 2)
            {
                FLIC_Set_CurrentFrame(main_purify_button, 0);
                FLIC_Draw(280, 186, main_purify_button);
                // _help_entries.Entry_Index+64h = HLP_PURIFY;
            }
            else if(special_action_flag == 9)
            {
                FLIC_Set_CurrentFrame(main_meld_button, 0);
                FLIC_Draw(280, 186, main_meld_button);
                // _help_entries.Entry_Index+64h = HLP_MELD;
            }
            else
            {
                FLIC_Set_CurrentFrame(main_build_button, 0);
                FLIC_Draw(280, 186, main_build_button);
                // _help_entries.Entry_Index+64h = HLP_BUILD;
            }

        }
        else
        {
            FLIC_Set_CurrentFrame(main_lock_build_button, 0);
            FLIC_Draw(280, 186, main_lock_build_button);
            // _help_entries.Entry_Index+64h = ST_UNDEFINED;
        }
    }
    else
    {
        FLIC_Set_CurrentFrame(main_lock_done_button, 0);
        FLIC_Draw(246, 176, main_lock_done_button);
        // _help_entries.Entry_Index+46h = ST_UNDEFINED;
        FLIC_Set_CurrentFrame(main_lock_patrol_button, 0);
        FLIC_Draw(280, 176, main_lock_patrol_button);
        // _help_entries.Entry_Index+50h = ST_UNDEFINED;
        FLIC_Set_CurrentFrame(main_lock_build_button, 0);
        FLIC_Draw(280, 186, main_lock_build_button);
        // _help_entries.Entry_Index+64h = ST_UNDEFINED;
        FLIC_Set_CurrentFrame(main_wait_button, 0);
        FLIC_Draw(246, 186, main_wait_button);
        /// _help_entries.Entry_Index+5Ah = ST_UNDEFINED;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw_Unit_Action_Buttons()\n", __FILE__, __LINE__);
#endif
}

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
// MoO2: ~Add_Map_Fields_()
void Main_Screen_Draw_Game_Buttons(void)
{
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
}


// WZD o57p07
void Add_Unit_Window_Fields(void)
{
    int16_t itr_unit_stack_count;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;

    for(itr_unit_stack_count = 0; itr_unit_stack_count < _unit_stack_count; itr_unit_stack_count++)
    {
        Unit_Window_Picture_Coords(itr_unit_stack_count, &x1, &y1, &x2, &y2);
        Unit_Window_Fields[itr_unit_stack_count] = Add_Hidden_Field(x1-1, y1-1, x2-2, y2-2, 0, ST_UNDEFINED);
    }

}


// WZD o57p08
void Main_Screen_Draw(void)
{
    int16_t before_map_x;
    int16_t before_map_y;
    int16_t after_map_x;
    int16_t after_map_y;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw()\n", __FILE__, __LINE__);
#endif

    Reset_Window();
    Set_Page_Off();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
    dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
    dbg_prn("DEBUG: [%s, %d]: _map_plane: %d\n", __FILE__, __LINE__, _map_plane);
    dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
    dbg_prn("DEBUG: [%s, %d]: _prev_world_y: %d\n", __FILE__, __LINE__, _prev_world_y);
    dbg_prn("DEBUG: [%s, %d]: _human_player_idx: %d\n", __FILE__, __LINE__, _human_player_idx);
#endif

    before_map_x = _map_x;
    before_map_y = _map_y;

    Main_Screen_Draw_Do_Draw(&_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);

    after_map_x = _map_x;
    after_map_y = _map_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: before_map_x: %d\n", __FILE__, __LINE__, before_map_x);
    dbg_prn("DEBUG: [%s, %d]: before_map_y: %d\n", __FILE__, __LINE__, before_map_y);
    dbg_prn("DEBUG: [%s, %d]: after_map_x: %d\n", __FILE__, __LINE__, after_map_x);
    dbg_prn("DEBUG: [%s, %d]: after_map_y: %d\n", __FILE__, __LINE__, after_map_y);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
    dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
    dbg_prn("DEBUG: [%s, %d]: _map_plane: %d\n", __FILE__, __LINE__, _map_plane);
    dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
    dbg_prn("DEBUG: [%s, %d]: _prev_world_y: %d\n", __FILE__, __LINE__, _prev_world_y);
    dbg_prn("DEBUG: [%s, %d]: _human_player_idx: %d\n", __FILE__, __LINE__, _human_player_idx);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Draw()\n", __FILE__, __LINE__);
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

// WZD o57p10
void IDK_UnitMoves_and_PlanarTravel(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: IDK_UnitMoves_and_PlanarTravel()\n", __FILE__, __LINE__);
#endif



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: IDK_UnitMoves_and_PlanarTravel()\n", __FILE__, __LINE__);
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

    /*
        Reset_Map_Draw()
            _prev_map_draw_x = e_ST_UNDEFINED;
            _prev_map_draw_y = e_ST_UNDEFINED;
        
        _prev_map_draw_x = ST_UNDEFINED;  // WZD dseg:6FFE  AKA Map_LastDraw_X
        _prev_map_draw_y = ST_UNDEFINED;  // WZD dseg:7000  AKA Map_LastDraw_Y
    */
    // j_Reset_Map_Draw()  // AKA j_Unset_LastMapDraw_XY();

    Minimap_Set_Dims(58, 30);


    Draw_Maps(MAP_START_X, MAP_START_Y, MAP_WIDTH, MAP_HEIGHT, map_x, map_y, map_plane, x_pos, y_pos, player_idx);
    // x and y origin for screen
    // width and height of map in squares
    // ? x, y, plane for world ?
    // player_idx of current/human_player_idx

    FLIC_Draw(0, 0, main_background);

    // WZD o67p09
    Draw_Minimap_Window(251, 21, 58, 30);

    // _help_entries.Entry_Index+6Eh = ST_UNDEFINED;
    // _help_entries.Entry_Index+96h = ST_UNDEFINED;
    // _help_entries.Entry_Index+0A0h = ST_UNDEFINED;
    // _help_entries.Entry_Index+0F0h = ST_UNDEFINED;



    // DEBUG
    // void Build_Unit_Stack(int16_t player_idx, int16_t world_plane, int16_t world_x, int16_t world_y)
    // Build_Unit_Stack(0, 0, 0, 0);
    // Build_Unit_Stack(0, 0, 0, 0);
    // Build_Unit_Stack(0, _UNITS[46].world_plane, _UNITS[46].world_x, _UNITS[46].world_y);

    if(_unit_stack_count > 0)
    {
        Main_Screen_Draw_Unit_Window(_unit_window_start_x, _unit_window_start_y);  // AKA j_OVL_DrawStackSelect(OVL_STKUnitCards_Lft, OVL_STKUnitCards_Top)
        /*
        ; draws the unit action buttons into the current draw
; frame, locking everything but "WAIT" if no stack is
; selected, otherwise the extra action is based on the
; value stored in OVL_ExtraUnitAction
; Attributes: thunk
WZD o57p04
*/
        // TODO(JimBalcomb,20230616): make note of the drawing of locked buttons in here  ? program-flow oddities ? screen component drawing graph ?
        Main_Screen_Draw_Unit_Action_Buttons();  // AKA j_OVL_DrawUnitActnBtns
    }
    else
    {
        Main_Screen_Draw_Summary_Window();  // AKA  j_OVL_DrawDeselectIMG()
        Main_Screen_Draw_Unit_Action_Locked_Buttons();  // AKA  j_OVL_DrawLockedActns()
        Main_Screen_Draw_Next_Turn_Button();  // AKA  j_OVL_DrawNextTurnBtn()
    }

    Main_Screen_Draw_Status_Window();  // AKA  OVL_DrawGoldnMana()

    // if(OVL_StackHasPath == ST_TRUE)
    // TODO  j_OVL_DrawPath(OVL_Path_Length, OVL_Path_Xs, OVL_Path_Ys);

    Main_Screen_Draw_Game_Buttons();

    // TODO  OVL_DrawStackMoves();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Draw_Do_Draw()\n", __FILE__, __LINE__);
#endif
}

// WZD o063p04
void Cycle_Unit_Enchantment_Animation(void)
{
    unit_enchantment_animation_flag++;
    if(unit_enchantment_animation_flag > 1)
    {
        unit_enchantment_animation_flag = 0;
    }
    if(unit_enchantment_animation_flag < 1)
    {
        unit_enchantment_animation_count++;
    }
    if(unit_enchantment_animation_count > 7)
    {
        unit_enchantment_animation_count = 0;
    }

}

// WZD o063p05
void Unit_Window_Draw_Unit_Picture(int16_t x, int16_t y, int16_t unit_stack_unit_idx, int16_t flag)
{
    int16_t unit_owner_idx;
    int16_t unit_colored_backgrounds_idx;


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

    unit_type_idx = _UNITS[unit_idx].type;

    FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 0);
    FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 1);
    Draw_Picture_To_Bitmap(_unit_type_table[unit_type_idx].pict_seg, UnitDraw_WorkArea);

    unit_owner_idx = (int16_t)_UNITS[unit_idx].owner_idx;

    if(unit_owner_idx != NEUTRAL_PLAYER_IDX)
    {
        banner = (int16_t)_players[unit_owner_idx].Banner;
    }
    else
    {
        banner = 5;
    }

    // TODO  for(itr_banner_colors = 0; itr_banner_colors < 5; itr_banner_colors++)
    // TODO  {
    // TODO      LBX_IMG_ColorReplace(UnitDraw_WorkArea, itr_banner_colors + 214, COL_Banners_P0[itr_banner_colors * 5]);
    // TODO  }

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

    Draw_Picture(x+1, y+1, UnitDraw_WorkArea);

    /*
        BEGIN: Unit Status - Icon/Text
    */
    // TODO  if(unit_owner_idx == _human_player_idx)
    // TODO  {
    // TODO  
    // TODO  }
    /*
        END: Unit Status - Icon/Text
    */

}

// WZD o63p12
// OVL_GetStackHMoves
int16_t OVL_GetStackHMoves(void)
{
/*
XREFs
    j_OVL_GetStackHMoves()
    OVL_DrawStackMoves()
j_OVL_GetStackHMoves
    Main_Screen()
    IDK_UnitMoves_and_PlanarTravel
    IDK_ActiveUnitStack_MovesOrPath_s53150


    loops through current unit stack
        checks for active
        checks if the unit type is/has/does transport/carries units
            breaks and assigns units current HMoves
        checks for wind walking - ability or enchantment
            does not break, but assigns units current HMoves
    Scenarios/Use-Cases:
        stack does not have a 'carries units' and does not have a 'has wind walking'
        stack does have a 'carries units' but does not have a 'has wind walking'
        stack does not have a 'carries units' but does have a 'has wind walking'
        
        if stack does have a 'carries units', that is that and its are the movement points
        ...
        ? nay 'carries units', nay 'has wind walking' ?
            returns lowest HMoves of 'active' units

NOTE: you can have multiple transport units in a stack

almost the same code as in WZD ovr097 STK_GetMovesLeft()
STK_GetMovesLeft() breaks after UNIT_HasWindWalking(); does have StackHasWindWalkingUnit or WindWalkingUnitIdx
OON XREF STK_move() WZD o95p01

*/
    int16_t StackHasWindWalkingUnit;
    int16_t WindWalkingUnitIdx;
    int16_t tmp_half_move_points; // In Dasm, SI
    int16_t tmp_unit_idx;  // In Dasm, DI
    int16_t itr_unit_stack;
    int8_t tmp_unit_type;  // In Dasm, DNE

    tmp_half_move_points = 1000;

    StackHasWindWalkingUnit = ST_FALSE;

    for(itr_unit_stack = 0; itr_unit_stack < _unit_stack_count; itr_unit_stack++)
    {
        tmp_unit_idx = _unit_stack[itr_unit_stack].unit_idx;
        if(_unit_stack[itr_unit_stack].active == ST_TRUE)
        {
            if(_UNITS[tmp_unit_idx].HMoves < tmp_half_move_points)
            {
                tmp_half_move_points = _UNITS[tmp_unit_idx].HMoves;
            }

            tmp_unit_type = _UNITS[tmp_unit_idx].type;
            if(_unit_type_table[tmp_unit_type].Transport == ST_TRUE)
            {
                tmp_half_move_points = _UNITS[tmp_unit_idx].HMoves;
                break;
            }

            if(UNIT_HasWindWalking(tmp_unit_idx) == ST_TRUE)
            {
                StackHasWindWalkingUnit = ST_TRUE;
                WindWalkingUnitIdx = tmp_unit_idx;
                tmp_half_move_points = _UNITS[tmp_unit_idx].HMoves;
            }
        }
    }

    if(tmp_half_move_points == 1000)
    {
        tmp_half_move_points = 0;
    }

    STK_HMoves_Left = tmp_half_move_points;

    return STK_HMoves_Left;
}


/* 
    WIZARDS.EXE  ovr064
*/

// WZD o64p01
void Allocate_Reduced_Map__1(void)
{
    _reduced_map_seg = Allocate_First_Block(_screen_seg, 303);  // 303 * 16 = 4848 bytes
}

// WZD o64p02
// PRIVATE  void Allocate_Reduced_Map__2(void);

// WZD o64p03
// ? subset of Draw_Maps() ?
// AKA OVL_PrepMinimap()
void MainScr_Prepare_Reduced_Map(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: MainScr_Prepare_Reduced_Map()\n", __FILE__, __LINE__);
#endif
    int16_t minimap_x;
    int16_t minimap_y;
    int16_t minimap_width;
    int16_t minimap_height;
    minimap_width = 58;
    minimap_height = 30;
    Minimap_Set_Dims(minimap_width, minimap_height);
    Minimap_Coords(&minimap_x, &minimap_y, ((_map_x+6)%60), (_map_y+5), minimap_width, minimap_height);
    Draw_Minimap(minimap_x, minimap_y, _map_plane, _reduced_map_seg, minimap_width, minimap_height, 0, 0, 0);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: MainScr_Prepare_Reduced_Map()\n", __FILE__, __LINE__);
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

    // TODO  Access Violation  if(_events_table[92] == 2)
    // TODO  Access Violation  if(_events_table[84] == 2)
    // TODO  Access Violation  if(_events_table[88] == 2)
    // TODO  Access Violation  if(_events_table[80] == 2)
    // TODO  Access Violation  if(_events_table[76] == 2)
    // TODO  Access Violation  if(_events_table[96] == 2)




    /*
        BEGIN: Conjunction - Sorcery, Chaos, Nature
    */
    // TBL_Events
    // s_EVENT_DATA.Conjunction_Sorcery.Status], 2
    // 0x0403af5c  access violation: 0x403AF5C
    // _events_table[]  0x0403af5c
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

    // _help_entries.Entry_Index+6Eh = ST_UNDEFINED;

    if(all_units_moved == ST_TRUE)
    {
        // _help_entries.Entry_Index+6Eh = HLP_NEXT_TURN

        FLIC_Draw(240, 173, next_turn_background_seg);

        Set_Draw_Active_Stack_Never();

        if(g_Current_Screen == ST_UNDEFINED)
        {
            FLIC_Draw(246, 178, next_turn_button_seg);  // ? offset by 6,5 to support the 3-D illusion of being depressed
        }
        else
        {
            Cycle_Unit_Enchantment_Animation();
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Draw_Next_Turn_Button()\n", __FILE__, __LINE__);
#endif
}

// WZD o64p08
void Main_Screen_Draw_Unit_Action_Locked_Buttons(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw_Unit_Action_Locked_Buttons()\n", __FILE__, __LINE__);
#endif

    FLIC_Set_CurrentFrame(main_lock_done_button, 0);
    FLIC_Draw(246, 176, main_lock_done_button);

    FLIC_Set_CurrentFrame(main_lock_patrol_button, 0);
    FLIC_Draw(280, 176, main_lock_patrol_button);

    FLIC_Set_CurrentFrame(main_wait_button, 0);
    FLIC_Draw(246, 186, main_wait_button);

    if(special_action_flag == ST_UNDEFINED)
    {
        FLIC_Set_CurrentFrame(main_lock_purify_button, 0);
        FLIC_Draw(280, 186, main_lock_purify_button);
    }
    else
    {
        FLIC_Set_CurrentFrame(main_lock_build_button, 0);
        FLIC_Draw(280, 186, main_lock_build_button);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Draw_Unit_Action_Locked_Buttons()\n", __FILE__, __LINE__);
#endif
}


// WZD o64p09
void Unit_Window_Picture_Coords(int16_t unit_stack_unit_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2)
{

    // TODO(JimBalcomb,20230616): add module-scoped (manifest) contstants for the magic-numbers used here-in
    // 3 for figures width, 3 for figures height
    // ? 23 for figures background width, 29 for figures background height ?
    // ? 22 for figrues width, 28 for figures height ?
    *x1 = _unit_window_start_x + ((unit_stack_unit_idx % 3) * 23);
    *y1 = _unit_window_start_y + ((unit_stack_unit_idx / 3) * 29);
    *x2 = *x1 + 22;
    *y2 = *y1 + 28;

}
