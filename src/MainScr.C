/*
    WIZARDS.EXE
    ovr057
    ovr058
    ovr061
    ovr062
    ovr063
    ovr064
    ovr095
    ovr097

¿ MoO2  Module: SHIPSTK ?

*/

#include "MoM.H"



// extern uint8_t DBG_debug_flag;
uint8_t DBG_debug_flag = 0;
void Main_Screen_Draw_Debug_Information(void);



/*
    Forward Declare Private Functions
*/

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
// AKA Do_Move_Stack_DirKey()
void Move_Stack_DirKey(int16_t movement_direction);

/*
    WIZARDS.EXE  ovr058
*/
// WZD o58p01
void Main_Screen_Draw_Unit_Window(int16_t start_x, int16_t start_y);
// WZD o58p02
void Set_Mouse_List_Default(void);



/*
    WIZARDS.EXE  ovr062
*/
// WZD o62p01
int16_t o62p01_empty_function(int16_t player_idx);
// WZD o62p02
// AKA OVL_StackSelect()
void Select_Unit_Stack(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t unit_x, int16_t unit_y);
// WZD o62p03
void Sort_Unit_Stack(void);
// WZD o62p04
void Build_Unit_Stack(int16_t player_idx, int16_t world_plane, int16_t world_x, int16_t world_y);
// WZD o62p05
void OVL_BringIntoView(int16_t *map_x, int16_t *map_y, int16_t unit_x, int16_t unit_y, int16_t map_plane);
// WZD o62p06
int16_t OVL_TileOffScrnEdge(int16_t *map_x, int16_t *map_y, int16_t unit_x, int16_t unit_y, int16_t map_width, int16_t map_height);
// WZD o62p07
// DONT  int16_t o62p07_Empty_pFxn(int16_t troop_count, int16_t * troops);
// WZD o62p08
void Stack_Action(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p, int16_t action, int16_t destination_x, int16_t destination_y);
// WZD o62p09
int16_t EarthGateTeleport__WIP(int16_t wx, int16_t wy, int16_t wp);
// WZD o62p10
void USW_FullDisplay(int16_t unit_idx, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

/*
    WIZARDS.EXE  ovr063
*/
// WZD o063p01
void Main_Screen_Draw_Status_Window(void);
// WZD o063p02
void Main_Screen_Draw_Do_Draw(int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t x_pos, int16_t y_pos, int16_t player_idx);
// WZD o063p03
// PUBLIC  void Draw_Unit_Enchantment_Outline(int16_t unit_idx);
// WZD o063p04
// PUBLIC  void Cycle_Unit_Enchantment_Animation(void);
// WZD o063p05
void Draw_Unit_Picture(int16_t x, int16_t y, int16_t unit_stack_unit_idx, int16_t flag);
// WZD o063p05
// AKA OVL_DrawUnitImage()
void Draw_Unit_StatFig(int16_t x, int16_t y, int16_t unit_idx, int16_t flag);
// WZD o063p06
void Unit_Window_Draw_Unit_Attributes(int16_t x, int16_t y, int16_t unit_idx);

// WZD o63p07
void Main_Screen_Draw_Movement_Bar();

// WZD o63p08
void Draw_Movement_Mode_Icons(int16_t x, int16_t y, int16_t unit_idx);

// WZD o63p10
int16_t Stack_Has_PlanarTravel(void);

// WZD o63p11
int16_t Active_Stack_On_Astral_Gate();

// WZD o63p12
// AKA OVL_GetStackHMoves()
// AKA OVL_GetStackHMoves_1()
int16_t Stack_Moves_Active(void);

// WZD o63p13
// AKA OVL_GetStackHMoves_2()
int16_t Stack_Moves(void);

// WZD o63p14
// AKA RP_TILE_HasCity2()
// functionally identical to TILE_HasCity, would be byte identical if it wasn't for the reversed order of checking the three parameters
int16_t City_At_Square__2(int16_t wx, int16_t wy, int16_t wp);

// WZD o63p15
int16_t Check_Planar_Seal(void);



/*
    WIZARDS.EXE  ovr064
*/
// WZD o64p01
// void Allocate_Reduced_Map__1(void);
// WZD o64p02
// void Allocate_Reduced_Map__2(void);
void Allocate_Reduced_Map(void);
// WZD o064p04
void Main_Screen_Draw_Summary_Window(void);
// WZD o064p05
void Main_Screen_Draw_Next_Turn_Button(void);
// WZD o64p06
// drake178: OVL_DisableIncmBlink()
void OVL_DisableIncmBlink(void);
// WZD o64p07
// drake178: OVL_EnableIncmBlink()
void OVL_EnableIncmBlink(void);
// WZD o64p08
void Main_Screen_Draw_Unit_Action_Locked_Buttons(void);
// WZD o64p09
void Unit_Window_Picture_Coords(int16_t stack_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2);
// WZD o64p10
void Set_Active_Stack_Movement_Path(int16_t unit_idx);
// WZD o64p11
void Draw_Active_Stack_Movement_Path(int16_t path_length, int8_t movepath_x_array[], int8_t movepath_y_array[]);

/*
    WIZARDS.EXE  ovr095
*/

// WZD o95p01
// drake178: STK_Move()
// AKA Move_Stack()
int16_t Move_Units(int16_t player_idx, int16_t destination_x, int16_t destination_y, int16_t path_type, int16_t * map_x, int16_t * map_y, int16_t map_p, int16_t unit_array_count, int16_t unit_array[]);

// WZD o95p02
// AKA OVL_MoveUnitStack()
// AKA Move_Units()
void Move_Units_Draw(int16_t player_idx, int16_t map_p, int16_t movepath_length, int16_t * map_x, int16_t * map_y, int16_t unit_array[], int16_t unit_array_count);

// WZD o95p03
// AI_ContactWizards()

// WZD o95p04
// drake178: G_STK_OvlObstacles()
void Update_MovePathMap(int8_t * ptr_movepath_cost_map_moves2, int16_t boatrider_count, int16_t troop_count, int16_t wp, int16_t player_idx, int16_t dst_wx, int16_t dst_wy, int16_t src_wx, int16_t src_wy);

// WZD o95p05
int16_t Army_Boatriders(int16_t troop_count, int16_t troops[], int16_t boatriders[]);

// WZD o95p06
// Fix_Patrol_On_Boats()


/*
    WIZARDS.EXE  ovr097
*/

// WZD o97p01
// STK_EvaluatePath()

// WZD o97p02
// sub_7E597()

// WZD o97p03
// drake178: TILE_HasCity()
int16_t Square_Has_City(int16_t world_x, int16_t world_y, int16_t map_plane);
// PRIVATE?  int16_t Square_Has_City(int16_t world_x, int16_t world_y, int16_t map_plane);

// WZD o97p04
// AKA STK_GetMovesLeft()
// ¿ vs. WZD o63p12  Stack_Moves_Active(void) ?
// ¿ vs. WZD o63p13  Stack_Moves(void) ?
int16_t Units_Moves(int16_t unit_array[], int16_t unit_array_count);

// WZD o97p05
// drake178: RP_CTY_CheckSpellWard()
int16_t RP_CTY_CheckSpellWard__STUB(int16_t city_idx, int16_t * stack_size, int16_t * stack_array);

// WZD o97p06
// drake178: CTY_CheckSpellWard()
int16_t CTY_CheckSpellWard__STUB(int16_t city_idx, int16_t * stack_size, int16_t * stack_array);

// WZD o97p07
// OVL_SpellWardError()

// WZD o97p08
void Print_Moves_String(int16_t x_start, int16_t y_start, int16_t moves2, int16_t right_align_flag);



// dseg:2E10                                                 BEGIN: Main Screen

// WZD dseg:2E10
// int16_t CRP_DBG_Alt_K_State = ST_FALSE;
// WZD dseg:2E12
int16_t DBG_Alt_A_State = ST_FALSE;
// WZD dseg:2E14
// MoO2  _given_chance_to_rename_home_star
int16_t _given_chance_to_rename_home_city = ST_FALSE;
// WZD dseg:2E16
// hotkey_MainScrn_X db 'X'

// WZD dseg:2E16                                                 the 4 above maybe are DBG ?
// WZD dseg:2E16                                                 below here, maybe compiler added as encountered?

// WZD dseg:2E17
char empty_string__ovr057[] = "";
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
// enum Key_Press  KP_Home  = 1Ah
// WZD dseg:2E2C
// cnst_HOTKEY_Up dw KP_Up
// enum Key_Press  KP_Up  = 3
// WZD dseg:2E2E
// cnst_HOTKEY_PgUp dw KP_PgUp
// enum Key_Press  KP_PgUp  = 19h
// WZD dseg:2E30
// cnst_HOTKEY_Left dw KP_Left
// enum Key_Press  KP_Left  = 1
// WZD dseg:2E32
// cnst_HOTKEY_Right dw KP_Right
// enum Key_Press  KP_Right  = 2
// WZD dseg:2E34
// cnst_HOTKEY_End dw KP_End
// enum Key_Press  KP_End  = 1Ch
// WZD dseg:2E36
// cnst_HOTKEY_Down dw KP_Down
// enum Key_Press  KP_Down  = 4
// WZD dseg:2E38
// cnst_HOTKEY_PgDn dw KP_PgDn
// enum Key_Press  KP_PgDn  = 1Dh

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
char hotkey_ArmiesButton[] = "A";
// WZD dseg:2E52
char aYouMayNotThrowAnySp[] = "You may not throw any spells while you are banished.  There are at least ";
// WZD dseg:2E9C
char aTurnsRemainingUntil[] = " turns remaining until you may return.";
// WZD dseg:2EC3
char _msg_not_enough_food_units_will_die[] = "Some units do not have enough food and will die unless you allocate more farmers in a city.  Do you wish to allow them to die?";
// WZD dseg:2F42
char cstrWarnNoPlaneMove[] = "The selected units cannot move on this plane.";
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






// WZD dseg:2F84                                                 BEGIN:  ovr058 - Initialized Data

// WZD dseg:2F84
int16_t m_background_music_ctr = 0;

// WZD dseg:2F86
char music_lbx_file__ovr058[] = "MUSIC";

// WZD dseg:2F86                                                 END:  ovr058 - Initialized Data












// WZD dseg:305B  align 2

// WZD dseg:305C                                                 ? BEGIN: Draw Unit Window ?

// WZD dseg:305C 
// XREF: Main_Screen_Draw_Unit_Window(); Unit_Window_Draw_Unit_Attributes();
// drake178: WP_Quality_Anim_Stg
int16_t unit_weapon_type_animation_count;
// WZD dseg:305E
// XREF:: UNIT_Draw_UE_Outline(); Cycle_Unit_Enchantment_Animation(); LBX_IMG_UE_Outline();
// AKA UE_Anim_State
int16_t unit_enchantment_animation_count;
// WZD dseg:3060
// AKA UE_Anim_Skip
int16_t unit_enchantment_animation_flag;  


// WZD  dseg:31AE
char cnst_Moves[] = "Moves:";





// WZD dseg:C03E                                                 ¿ BEGIN:  ovr057 - Uninitialized Data  (Main_Screen) ?

// WZD dseg:C03E
// CRP_OverlandVar_2 dw 0
// WZD dseg:C040
// CRP_OverlandVar_4 dw 0
// WZD dseg:C042
// OVL_MapVar3 dw 0

// WZD dseg:C044
int16_t CRP_OVL_Obstacle_Var1;

// WZD dseg:C046
// CRP_OVL_UU_Control_1 dw 0
// WZD dseg:C048
// CRP_OVL_UU_Control_2 dw 0
// WZD dseg:C04A
// CRP_OVL_UU_Control_3 dw 0
// WZD dseg:C04C
// CRP_OVL_UU_Control_4 dw 0

// WZD dseg:C04E
/*
; set to indicate any extra action possible for the
; selected stack
;   -1: no extra action possible
;    0: road building possible
;    1: settling possible
;    2: purifying possible
;    9: melding possible
*/
int16_t special_action_flag;

// WZD dseg:C050
int16_t _next_turn_button;
// WZD dseg:C052
int16_t g_unit_window_fields[MAX_STACK];
// WZD dseg:C064
// CRP_OverlandVar_3 dw 0

// WZD dseg:C066
int16_t _active_stack_path_length;
// WZD dseg:C068 
/*
; set to 0 after display-sorting the active stack
; set to 1 if road-building, but the unit is not on any of the plotted line tiles (before returning)
; set to 1 if road-building, and tiles left to do
; set to 1 if moving with path left to go
*/
int16_t _active_stack_has_path;



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
// WZD dseg:C088 
int16_t UU_MainScreen_flag;

// WZD dseg:C08A
// drake178: SND_Bkgrnd_Track
/*
; the current power-based background music track index
*/
int16_t m_background_music_num;

// WZD dseg:C08C word_42B2C dw 0


// WZD dseg:C08E
/*
    movement_points_to_use
    set by
        WZD o63p12 Stack_Moves_Active()
        WZD o63p13 Stack_Moves()
        WZD o97p04 Units_Moves()
*/
// AKA STK_HMoves_Left
int16_t stack_moves;
    // XREF  Stack_Moves_Active+D3      mov     ax, [stack_moves]                
    // XREF  Stack_Moves_Active:@@Done  mov     [stack_moves], _SI_MovementPoints
    // XREF  Units_Moves+A3        mov     ax, [stack_moves]                
    // XREF  Units_Moves:loc_7E717 mov     [stack_moves], si                
    // XREF  Stack_Moves+132              mov     [stack_moves], ax                
    // XREF  Stack_Moves+135              mov     ax, [stack_moves]                


int16_t main_screen_loaded = ST_FALSE;





char cstr_1st5[] = " !\"$#";
char cstr_ABC[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char cstr_WarGames[] = "WOULD YOU LIKE TO PLAY A GAME?";



// WZD dseg:3062

/*
    ¿ 2-D Array ?
    not just for enchentments
    more like 'magic realm outline'
    enchantment_outline_colors[magic_realm][animation_count]

array used for color outline for enchanted units and powered items

5 magic realms {Chaos, Death, Life, Nature, Sorcery}

8 animation stages

// ? 3 colors for each stages ?
// 5 * 8 * 3 = 120

*/
uint8_t enchantment_outline_colors[5][8][8] =
{
    {
        { 45, 44, 43, 42, 41, 42, 43, 44 },
        { 44, 43, 42, 41, 42, 43, 44, 45 },
        { 43, 42, 41, 42, 43, 44, 45, 44 },
        { 42, 41, 42, 43, 44, 45, 44, 43 },
        { 41, 42, 43, 44, 45, 44, 43, 42 },
        { 42, 43, 44, 45, 44, 43, 42, 41 },
        { 43, 44, 45, 44, 43, 42, 41, 42 },
        { 44, 45, 44, 43, 42, 41, 42, 43 }
    },
    {
        { 36, 35, 34, 33, 32, 33, 34, 35 },
        { 35, 34, 33, 32, 33, 34, 35, 36 },
        { 34, 33, 32, 33, 34, 35, 36, 35 },
        { 33, 32, 33, 34, 35, 36, 35, 34 },
        { 32, 33, 34, 35, 36, 35, 34, 33 },
        { 33, 34, 35, 36, 35, 34, 33, 32 },
        { 34, 35, 36, 35, 34, 33, 32, 33 },
        { 35, 36, 35, 34, 33, 32, 33, 34 }
    },
    {
        { 11, 12, 13, 14, 15, 14, 13, 12 },
        { 12, 13, 14, 15, 14, 13, 12, 11 },
        { 13, 14, 15, 14, 13, 12, 11, 12 },
        { 14, 15, 14, 13, 12, 11, 12, 13 },
        { 15, 14, 13, 12, 11, 12, 13, 14 },
        { 14, 13, 12, 11, 12, 13, 14, 15 },
        { 13, 12, 11, 12, 13, 14, 15, 14 },
        { 12, 11, 12, 13, 14, 15, 14, 13 }
    },
    {
        { 68, 67, 66, 65, 64, 65, 66, 67},
        { 67, 66, 65, 64, 65, 66, 67, 68},
        { 66, 65, 64, 65, 66, 67, 68, 67},
        { 65, 64, 65, 66, 67, 68, 67, 66},
        { 64, 65, 66, 67, 68, 67, 66, 65},
        { 65, 66, 67, 68, 67, 66, 65, 64},
        { 66, 67, 68, 67, 66, 65, 64, 65},
        { 67, 68, 67, 66, 65, 64, 65, 66}
    },
    {
        { 100, 99, 98, 97, 96, 97, 98, 99 },
        { 99, 98, 97, 96, 97, 98, 99, 100 },
        { 98, 97, 96, 97, 98, 99, 100, 99 },
        { 97, 96, 97, 98, 99, 100, 99, 98 },
        { 96, 97, 98, 99, 100, 99, 98, 97 },
        { 97, 98, 99, 100, 99, 98, 97, 96 },
        { 98, 99, 100, 99, 98, 97, 96, 97 },
        { 99, 100, 99, 98, 97, 96, 97, 98 }
    }
};



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



// WZD dseg:9548  IMG_OVL_BuildBtn_BG  ; DATA XREF: GFX_Swap_AppendUView() ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:954A  IMG_WIZ_BookBinders dw 12h dup(0)  ; DATA XREF: sub_6343B()
// WZD dseg:956E
SAMB_ptr mirror_screen_background_seg;
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







// WZD dseg:998C 00 00                   GUI_SmallWork_IMG@ dw 0                 ; 96h LBX_Alloc_Space paragraphs
// WZD dseg:998E 00 00                   gsa_Sandbox dw 0                        ; 203h + 11F8h LBX_Alloc_Space paragraphs
// WZD dseg:9990 00 00                   _current_screen dw 0
// WZD dseg:9992 00 00                   GrowOutFrames dw 0
// WZD dseg:9994 00 00                   GrowOutTop dw 0
// WZD dseg:9996 00 00                   GrowOutLeft dw 0
// WZD dseg:9998 00 00 00 00             TBL_Events dd 0                         ; 7 LBX_Alloc_Space paragraphs (112 bytes)

// WZD dseg:999C
// uint16_t g_ActiveStack_UnitCount = 0;  // AKA OVL_StackSize
// _unit_stack_count = 0;
// WZD dseg:999E
// struct STK_UNIT UNIT_Stack[9];



/*
                                        ¿ BEGIN: ovr052 Terrain_Init() ?
*/
// WZD dseg:CB5E                                                 ovr052

// WZD dseg:CB5E
// MAPBACK.LBX  93  WARPED  warped mask
SAMB_ptr node_warped_seg;                // ; reserved EMM header pointer for a single image

// WZD dseg:CB60
// AKA gsa_OVL_Tile_WorkArea
SAMB_ptr Map_Square_WorkArea;               // alloc in Terrain_Init  // ; 70 * 16 = 1120 bytes

// WZD dseg:CB62
// AKA UU_IMG_OVL_Empty3
SAMB_ptr UU_hunters_lodge_seg;                 // ; single-loaded image, called "hunter's lodge"

// WZD dseg:CB64
SAMB_ptr IMG_OVL_Nightshade;                // ; reserved EMM header pointer for a single image

// WZD dseg:CB66
SAMB_ptr IMG_OVL_WildGame;                  // ; reserved EMM header pointer for a single image

// WZD dseg:CB68 00 00                                           UU_IMG_OVL_Empty1@ dw 0                 ; DATA XREF: Terrain_Init:loc_46318w
// WZD dseg:CB68                                                                                         ; single-loaded image, called "mine" in the file

// WZD dseg:CB6A
SAMB_ptr IMG_OVL_Corruption;                // ; reserved EMM header pointer for a single image

// WZD dseg:CB6C 00 00                                           UU_IMG_OVL_Mud@ dw 0                    ; DATA XREF: Terrain_Init+1F6w
// WZD dseg:CB6C                                                                                         ; reserved EMM header pointer (unused in the game?)

// WZD dseg:CB6E
/*
    load in Terrain_Init()
    MAPBACK.LBX  78-86
    SITES - coal, iron, silver, gold, gems, mithril, adamantium, quork, crysx
*/
SAMB_ptr mineral_site_segs[10];  // ¿ loops from 1 to 10 ?


// WZD dseg:CB80 00                                              db    0
// WZD dseg:CB81 00                                              db    0

// WZD dseg:CB82 
SAMB_ptr node_auras_seg[5];               // ; array of 5 reserved EMM header pointers for  ; 6 frame animations

// WZD dseg:CB8C 00                                              db    0
// WZD dseg:CB8D 00                                              db    0
// WZD dseg:CB8E 00 00                                           UU_IMG_OVL_Empty2@ dw 0                 ; DATA XREF: Terrain_Init
// WZD dseg:CB8E                                                                                         ; single-loaded image, called "lumber camp" in the file

// WZD dseg:CB90
SAMB_ptr Warp_Node_WorkArea;                // ; used to save and manipulate the tile graphics to ; display the warp node effect


// WZD dseg:CB92
SAMB_ptr _unit_colored_backgrounds_seg[6];  // load in Terrain_Init() ovr052

// WZD dseg:CB92                                                                                         ; DATA XREF: Terrain_Init
// WZD dseg:CB92                                                                                         ; array of 6 reserved EMM header pointers
// WZD dseg:CB92                                                                                         ; (1 per banner color)
// WZD dseg:CB9E 00                                              db    0
// WZD dseg:CB9F 00                                              db    0

// WZD dseg:CBA0
SAMB_ptr eroads_seg[8];                     // ; array of 8 reserved EMM header pointers  ; for 6 frame animations  ; clockwise 1-connection e.roads starting up
// WZD dseg:CBB0
SAMB_ptr eroad_lone_seg;                    // ; reserved EMM header pointer for a 6 frame animation  ; no connection enchanted road
// WZD dseg:CBB0
SAMB_ptr roads_seg[8];                      // ; reserved EMM header pointer for a 6 frame animation  ; no connection enchanted road
// WZD dseg:CBB2
SAMB_ptr road_lone_seg;                     // ; array of 8 reserved EMM header pointers  ; clockwise 1-connection roads starting up

// WZD dseg:CBC4
SAMB_ptr IMG_OVL_EZ_Cave;                   // ; reserved EMM header pointer for a single image
// WZD dseg:CBC6
SAMB_ptr IMG_OVL_EZ_Dung;                   // ; reserved EMM header pointer for a single image
// WZD dseg:CBC8
SAMB_ptr IMG_OVL_EZ_ATmpl;                   // ; reserved EMM header pointer for a single image
// WZD dseg:CBCA
SAMB_ptr IMG_OVL_EZ_AKeep;                   // ; reserved EMM header pointer for a single image
// WZD dseg:CBCC
SAMB_ptr IMG_OVL_EZ_MLair;                   // ; reserved EMM header pointer for a single image
// WZD dseg:CBCE
SAMB_ptr IMG_OVL_EZ_Ruins;                   // ; reserved EMM header pointer for a single image
// WZD dseg:CBD0
SAMB_ptr IMG_OVL_EZ_FTmpl;                   // ; reserved EMM header pointer for a single image

// WZD dseg:CBD2 00 00                                           word_43672 dw 0

// WZD dseg:CBD4
SAMB_ptr tower_unowned_seg;                 // ; reserved EMM header pointer for a single image

// WZD dseg:CBD6
SAMB_ptr tower_owned_seg;                   // ; reserved EMM header pointer for a single image

// WZD dseg:CBD8
SAMB_ptr IMG_OVL_Walled_City;               // ; reserved EMM header pointer for a 5 frame image

// WZD dseg:CBDA
SAMB_ptr IMG_OVL_NoWall_City;               // ; reserved EMM header pointer for a 5 frame image

// WZD dseg:CBDC 00                                              db    0
// WZD dseg:CBDD 00                                              db    0
// WZD dseg:CBDE 00                                              db    0
// WZD dseg:CBDF 00                                              db    0
// WZD dseg:CBE0 00                                              db    0
// WZD dseg:CBE1 00                                              db    0
// WZD dseg:CBE2 00                                              db    0
// WZD dseg:CBE3 00                                              db    0
// WZD dseg:CBE4 00                                              db    0
// WZD dseg:CBE5 00                                              db    0
// WZD dseg:CBE6 00                                              db    0
// WZD dseg:CBE7 00                                              db    0
// WZD dseg:CBE8 00                                              unk_43688 db    0
// WZD dseg:CBE9 00                                              db    0
// WZD dseg:CBEA 00                                              db    0
// WZD dseg:CBEB 00                                              db    0
// WZD dseg:CBEC 00                                              unk_4368C db    0
// WZD dseg:CBED 00                                              db    0
// WZD dseg:CBEE 00                                              db    0
// WZD dseg:CBEF 00                                              db    0
// WZD dseg:CBF0 00                                              db    0
// WZD dseg:CBF1 00                                              db    0
// WZD dseg:CBF2 00                                              db    0
// WZD dseg:CBF3 00                                              db    0
// WZD dseg:CBF4 00                                              db    0
// WZD dseg:CBF5 00                                              db    0
// WZD dseg:CBF6 00                                              db    0
// WZD dseg:CBF7 00                                              db    0
// WZD dseg:CBF8 00                                              db    0
// WZD dseg:CBF9 00                                              db    0
// WZD dseg:CBFA 00                                              db    0
// WZD dseg:CBFB 00                                              db    0
// WZD dseg:CBFC 00                                              db    0
// WZD dseg:CBFD 00                                              db    0
// WZD dseg:CBFE 00                                              db    0
// WZD dseg:CBFF 00                                              db    0
// WZD dseg:CC00 00                                              db    0
// WZD dseg:CC01 00                                              db    0

// WZD dseg:CC02
SAMB_ptr unexplored_mask_seg[14];       // ; array of 14 reserved EMM header pointers

// WZD dseg:CC1E 00                                              db    0
// WZD dseg:CC1F 00                                              db    0
// WZD dseg:CC20 00                                              db    0
// WZD dseg:CC21 00                                              db    0
// WZD dseg:CC22 00 00 00 00                                     terrain_lbx_001 dd 0                    ; DATA XREF: Terrain_Init
// WZD dseg:CC26 00 00                                           terrain_001_1 dw 0                      ; DATA XREF: Draw_Map_Terrain
// WZD dseg:CC28 00 00                                           g_EmmHndl_TERRAIN dw 0                  ; DATA XREF: Terrain_Init
// WZD dseg:CC28                                                                                         ; EMM_Load_LBX handle
// WZD dseg:CC2A 00                                              db    0
// WZD dseg:CC2B 00                                              db    0
// WZD dseg:CC2C 00                                              db    0
// WZD dseg:CC2D 00                                              db    0
/*
                                        ¿ END: ovr052 Terrain_Init() ?
*/



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
o57p10  ? Move_Stack_DirKey() ? 
*/


// WZD 057p01
void Main_Screen(void)
{
    int16_t hotkey_idx_Alt_P = 0;
    int16_t hotkey_idx_U = 0;
    char temp_string[10];
    int16_t hotkey_idx_F9 = 0;
    int16_t hotkey_idx_F8 = 0;
    int16_t hotkey_idx_F7 = 0;
    int16_t hotkey_idx_F6 = 0;
    int16_t hotkey_idx_F5 = 0;
    int16_t hotkey_idx_F4 = 0;
    int16_t hotkey_idx_F3 = 0;
    int16_t hotkey_idx_F2 = 0;
    int16_t hotkey_idx_F1 = 0;
    int16_t IDK_EoT_flag = 0;
    int16_t DBG_Alt_A__TurnCount = 0;
    int16_t hotkey_idx_Alt_A = 0;
    int16_t hotkey_idx_RightDown = 0;
    int16_t hotkey_idx_Down = 0;
    int16_t hotkey_idx_LeftDown = 0;
    int16_t hotkey_idx_Right = 0;
    int16_t hotkey_idx_Left = 0;
    int16_t hotkey_idx_RightUp = 0;
    int16_t hotkey_idx_LeftUp = 0;
    int16_t hotkey_idx_Up = 0;
    int16_t eot_gold_nop = 0;  // only used for 'Next Turn'; place-holder, not used; set by Player_Resource_Income_Total()
    int16_t eot_food = 0;  // only used for 'Next Turn'; set by Player_Resource_Income_Total()
    int16_t eot_mana_nop = 0;  // only used for 'Next Turn'; place-holder, not used; set by Player_Resource_Income_Total()
    int16_t hotkey_idx_Alt_N = 0;
    int16_t hotkey_idx_Shift_3 = 0;
    int16_t hotkey_idx_Alt_K = 0;
    int16_t hotkey_idx_F10 = 0;
    int16_t hotkey_idx_Alt_T = 0;
    int16_t unit_stack_hmoves = 0;
    int16_t unit_stack_world_y = 0;
    int16_t unit_stack_world_x = 0;
    int16_t IDK_unit_stack_in_view = 0;
    int16_t entity_idx = 0;  // AKA "input"
    int16_t hotkey_idx_Q = 0;
    int16_t hotkey_idx_C = 0;
    int16_t hotkey_idx_Shift_2 = 0;
    int16_t hotkey_idx_Shift_1 = 0;
    int16_t Unused_Button_Index = 0;
    int16_t hotkey_idx_X = 0;
    int16_t selected_unit_wy = 0;
    int16_t selected_unit_wx = 0;
    int16_t usw_y2 = 0;
    int16_t usw_x2 = 0;
    int16_t target_world_y = 0;  // doubles as usw_y1
    int16_t target_world_x = 0;  // doubles as usw_x1
    int16_t reduced_map_window_wy = 0;  // DNE in Dasm, uses target_world_y
    int16_t reduced_map_window_wx = 0;  // DNE in Dasm, uses target_world_x
    int16_t unit_idx = 0;
    int16_t allow_units_to_die = 0;
    int16_t Stack_Index = 0;  /* unit_idx || player_idx;  itr for _unit_stack;  also used for itr _num_players in Alt-P Debug Randomized Personality */
    int16_t itr_stack = 0;
    // itr__stack_idx__player_idx__turns_til_return
    int16_t turns_til_return = 0;  // only used for 'Spells Button'; turn count to complete 'Spell of Return'
    int16_t leave_screen_flag = 0;
    int16_t screen_changed = 0;
    int16_t input_field_idx = 0;
    int16_t mouse_x = 0;
    int16_t mouse_y = 0;
    int16_t itr_units = 0;

    int16_t hotkey_idx_D = 0;  // debug_hotkey
    int16_t hotkey_idx_T = 0;  // test_hotkey


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen()\n", __FILE__, __LINE__);
#endif



    // WZD main() |-> WZD_Load_Init_MainGame() |-> Main_Screen_Load_Pictures()
    if(main_screen_loaded == ST_FALSE)
    {
        Main_Screen_Load_Pictures();
        main_screen_loaded = ST_TRUE;
    }



    Reset_Window();
    Clear_Fields();

    // TODO  UU_MainScreen_flag = 1;  // only XREF Main_Screen(), sets TRUE, never tests

    Allocate_Reduced_Map();
    Reset_Draw_Active_Stack();
    Set_Outline_Color(ST_TRANSPARENT);
    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();

    Deactivate_Auto_Function();
    Assign_Auto_Function(Main_Screen_Draw, 1);

    _unit_window_start_x = 247;
    _unit_window_start_y = 79;

    m_citylist_first_item = 0;

    // TODO  CRP_OverlandVar_2 = 0;  // ? ST_FALSE ?
    // TODO  CRP_OVL_Obstacle_Var1 = 0;  // ? ST_FALSE ?
    // TODO  OVL_MapVar3 = 1;  // ? ST_TRUE ?

    Reset_Map_Draw();
    MainScr_Create_Reduced_Map_Picture();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

    Set_Mouse_List_Default();

    Set_Unit_Action_Special();

    // TODO  if (CRP_OverlandVar_3 != 1) { CRP_OverlandVar_3 = 0; }  // ? ST_TRUE ST_FALSE ?
    // TODO  if (CRP_OverlandVar_4 != 1) { CRP_OverlandVar_4 = 0; }  // ? ST_TRUE ST_FALSE ?

    screen_changed = ST_FALSE;

    IDK_EoT_flag = ST_FALSE;

    Set_Input_Delay(1);

    Reset_Cycle_Palette_Color();

    Deactivate_Help_List();
    Set_Main_Screen_Help_List();

    DBG_Alt_A__TurnCount = ST_UNDEFINED;
    
    assert(_map_x >= WORLD_X_MIN && _map_x <= WORLD_X_MAX);  /*  0 & 59 */
    assert(_map_y >= WORLD_Y_MIN && _map_y <= WORLD_Y_MAX);  /*  0 & 39 */

    Main_Screen_Draw();

    PageFlip_FX();

    /*
        BEGIN: Screen-Loop
    */

    leave_screen_flag = ST_FALSE;
    while(leave_screen_flag == ST_FALSE)
    {

        Mark_Time();

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

        /*
            {UpLeft, Up, UpRight, Left, Right, DownLeft, Down, DownRight}
            Read_Key()  {}
            the manual says "by hitting the keys on the numeric keypad"

        */
        hotkey_idx_LeftUp     = Add_Hot_Key(ST_KEY_LEFTUP);  // cnst_HOTKEY_Home
        hotkey_idx_Up         = Add_Hot_Key(ST_KEY_UP);
        hotkey_idx_RightUp    = Add_Hot_Key(ST_KEY_RIGHTUP);
        hotkey_idx_Left       = Add_Hot_Key(ST_KEY_LEFT);
        hotkey_idx_Right      = Add_Hot_Key(ST_KEY_RIGHT);
        hotkey_idx_LeftDown   = Add_Hot_Key(ST_KEY_LEFTDOWN);
        hotkey_idx_Down       = Add_Hot_Key(ST_KEY_DOWN);
        hotkey_idx_RightDown  = Add_Hot_Key(ST_KEY_RIGHTDOWN);

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

        hotkey_idx_D = Add_Hot_Key('Z');  // debug_hotkey  ...  Derp. 'D' is already used for the "Done" button.
        hotkey_idx_T = Add_Hot_Key('T');  // test_hotkey

        /*
            END: Add Fields
        */


        input_field_idx = Get_Input();

        if(quit_game_flag == ST_TRUE)
        {
            current_screen = scr_Quit_To_DOS;
            leave_screen_flag = ST_TRUE;
        }
        else
        {




/*
    BEGIN: Check Input against Fields
*/



        /*
            BEGIN:  Advisor - Hot-Key
        */
        {
            // Advisor - Surveyor
            if(input_field_idx == hotkey_idx_F1)
            {
                Advisor_Screen(0);
                Assign_Auto_Function(Main_Screen_Draw, 1);
                Main_Screen_Reset();
            }
            // Advisor - Cartographer
            if(input_field_idx == hotkey_idx_F2)
            {
                Advisor_Screen(1);
                Assign_Auto_Function(Main_Screen_Draw, 1);
                Main_Screen_Reset();
            }
            // Advisor - Apprentice
            if(input_field_idx == hotkey_idx_F3)
            {
                Advisor_Screen(2);
                Assign_Auto_Function(Main_Screen_Draw, 1);
                Main_Screen_Reset();
            }
            // Advisor - Historian
            if(input_field_idx == hotkey_idx_F4)
            {
                Advisor_Screen(3);
                Assign_Auto_Function(Main_Screen_Draw, 1);
                Main_Screen_Reset();
            }
            // Advisor - Astrologer
            if(input_field_idx == hotkey_idx_F5)
            {
                Advisor_Screen(4);
                Assign_Auto_Function(Main_Screen_Draw, 1);
                Main_Screen_Reset();
            }
            // Advisor - Chancellor
            if(input_field_idx == hotkey_idx_F6)
            {
                Advisor_Screen(5);
                Assign_Auto_Function(Main_Screen_Draw, 1);
                Main_Screen_Reset();
            }
            // Advisor - Tax Collector
            if(input_field_idx == hotkey_idx_F7)
            {
                Advisor_Screen(6);
                Assign_Auto_Function(Main_Screen_Draw, 1);
                Main_Screen_Reset();
            }
            // Advisor - Grand Vizier
            if(input_field_idx == hotkey_idx_F8)
            {
                Advisor_Screen(7);
                Assign_Auto_Function(Main_Screen_Draw, 1);
                Main_Screen_Reset();
            }
            // Advisor - Mirror
            if(input_field_idx == hotkey_idx_F9)
            {
                Advisor_Screen(8);
                Assign_Auto_Function(Main_Screen_Draw, 1);
                Main_Screen_Reset();
            }
        }
        /*
            END:  Advisor - Hot-Key
        */

        /* Alt-A   */  /* if(input_field_idx == hotkey_idx_Alt_A) {if(Check_Release_Version()==ST_FALSE){DBG_Alt_A_State=1-DBG_Alt_A_State;}} */
        /* Alt-P   */
        /* Alt-K   */
        /* Shift-3 '#' */
        /* Alt-T   */
        /* Alt-N   */  /* if(input_field_idx == hotkey_idx_Alt_A) {if(Check_Release_Version()==ST_FALSE){DBG_ShowTileInfo=1-DBG_ShowTileInfo;}} */

        // ST_DEBUG Hot-Keys
        if(input_field_idx == hotkey_idx_D)  /* Debug Hot-Key */
        {
            DLOG("STU_DEBUG: debug_hotkey");
            DBG_debug_flag = !DBG_debug_flag;  // ~== `^= 1`
            if(DBG_debug_flag)
            {

            }

        }
        if(input_field_idx == hotkey_idx_T)  /* Test Hot-Key */
        {
            DLOG("STU_DEBUG: test_hotkey");
            // city in last column / on right edge Center_Map(&_map_x, &_map_y, 8+6, 10+5, 0);
            // TST?  Center_Map(&_map_x, &_map_y, 8+6, 10+5, 0);
            // Center_Map(&_map_x, &_map_y, 18+6, 11+5, 0);  // TST: Validate_Entities_On_Movement_Map()
            // // ¿ Req'd ? MainScr_Create_Reduced_Map_Picture();
            // // ¿ Req'd ? Set_Mouse_List(1, mouse_list_default);
            // // ¿ Req'd ? Reset_Map_Draw();
            // _prev_world_x = 18 + (_main_map_grid_x - (MAP_WIDTH  / 2));
            // _prev_world_y = 11 + (_main_map_grid_y - (MAP_HEIGHT / 2));
            // IDK_CheckSet_MapDisplay_XY(); // translates _prev_world_x,y coordinates
            _prev_world_x = 18;
            _prev_world_y = 11;
#ifdef STU_DEBUG
            DBG_TST_Made_Map_Move = 1;
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: DBG_TST_Made_Map_Move: %d\n", __FILE__, __LINE__, DBG_TST_Made_Map_Move);
#endif
            /*
                DBG_TST
                    Right-Click Movement Map Grid Field
                    Entity - Unit - Human Player
            */
            // _unit = entity_idx;
            // unit_idx = _unit;
            // selected_unit_wx = _UNITS[unit_idx].wx;
            // selected_unit_wy = _UNITS[unit_idx].wy;
            // OVL_Map_CenterX = _UNITS[unit_idx].wx;
            // OVL_Map_CenterY = _UNITS[unit_idx].wy;
            // Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, selected_unit_wx, selected_unit_wy);
        }

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
                _map_plane = _UNITS[unit_idx].wp;
                Center_Map(&_map_x, &_map_y, _UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _UNITS[unit_idx].wp);
                MainScr_Create_Reduced_Map_Picture();
                Set_Mouse_List(1, mouse_list_default);
                Reset_Map_Draw();
            }
        }
        
        // ? Quit Action / Unselect Unit ?
        if((input_field_idx == hotkey_idx_Q) || (input_field_idx == hotkey_idx_U) )
        {
            _unit_stack_count = 0;
            Set_Draw_Active_Stack_Always();
            Set_Unit_Draw_Priority();
            Reset_Stack_Draw_Priority();
            Set_Entities_On_Map_Window(_map_x, _map_y,_map_plane);
            Reset_Map_Draw();
            screen_changed = ST_TRUE;
#ifdef STU_DEBUG
            DBG_TST_Selected_Stack = 0;
#endif
        }

        /*
            BEGIN: Mouse Click
        */

        /*
            BEGIN: Game Buttons - Info Button
        */
        if(input_field_idx == _info_button)
        {
            Play_Left_Click__STUB();
            current_screen = scr_Advisor_Screen;
            leave_screen_flag = ST_TRUE;
        }

        /*
            BEGIN: Game Buttons - Game Button
        */
        if(input_field_idx == _game_button)
        {
            Play_Left_Click__STUB();
            current_screen = scr_Load_Screen;
            leave_screen_flag = ST_TRUE;
        }
        /*
            BEGIN: Game Buttons - Cities Button
        */
        if(input_field_idx == _cities_button)
        {
            Play_Left_Click__STUB();
            current_screen = scr_Cities_Screen;
            leave_screen_flag = ST_TRUE;
        }
        /*
            BEGIN: Game Buttons - Magic Button
        */
        if(input_field_idx == _magic_button)
        {
            Play_Left_Click__STUB();
            current_screen = scr_Magic_Screen;
            leave_screen_flag = ST_TRUE;
        }
        /*
            BEGIN: Game Buttons - Armies Button
        */
        if(input_field_idx == _armies_button)
        {
            Play_Left_Click__STUB();
            current_screen = scr_Armies_Screen;
            leave_screen_flag = ST_TRUE;
        }

        /*
            BEGIN: Game Buttons - Spells Button
        */
        if(input_field_idx == _spells_button)
        {
            Play_Left_Click__STUB();
            if(_players[_human_player_idx].casting_spell_idx == 214) /* Spell of Return */
            {
                turns_til_return = _players[HUMAN_PLAYER_IDX].casting_cost_remaining / _players[HUMAN_PLAYER_IDX].Nominal_Skill;
                itoa(turns_til_return, temp_string, 10);
                strcpy(GUI_NearMsgString, aYouMayNotThrowAnySp);  // "You may not throw any spells while you are banished.  There are at least "
                strcat(GUI_NearMsgString, temp_string);
                strcat(GUI_NearMsgString, aTurnsRemainingUntil);  // " turns remaining until you may return."
                Warn0(GUI_NearMsgString);
                Assign_Auto_Function(Main_Screen_Draw, 1);
            }
            else
            {
                Set_Draw_Active_Stack_Always();
                Set_Unit_Draw_Priority();
                Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                leave_screen_flag = ST_TRUE;
                current_screen = scr_Spellbook_Screen;
            }
        }

        if(input_field_idx == _patrol_button)
        {
            Play_Left_Click__STUB();
            Reset_Draw_Active_Stack();
            Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, 1, 0, 0);  /* Action 1: 'Patrol' */
            WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
            Main_Screen_Reset();
            if(all_units_moved == ST_TRUE)
            {
                Set_Unit_Draw_Priority();
                Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                screen_changed = ST_TRUE;
            }
        }

        if(input_field_idx == _special_button)
        {
            Play_Left_Click__STUB();
            /*
                -1: no extra action possible
                 0: road building possible
                 1: settling possible
                 2: purifying possible
                 9: melding
            */
            switch(special_action_flag)
            {
                case 0:  /* Road Building */
                {
                    DLOG("switch(special_action_flag)  case 0:");
                    leave_screen_flag = ST_UNDEFINED;
                    current_screen = scr_Road_Build;
                } break;
                case 1:  /* Settle */
                {
                    DLOG("switch(special_action_flag)  case 1:");
                    // o100p04
                    if(Do_Build_Outpost() == ST_TRUE)
                    {
                        o62p01_empty_function(_human_player_idx);
                        Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, selected_unit_wx, selected_unit_wy);
                        WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
                        Main_Screen_Reset();
                        screen_changed = ST_TRUE;
                    }
                    Assign_Auto_Function(Main_Screen_Draw, 1);
                } break;
                case 2:  /* Purify */
                {
                    DLOG("switch(special_action_flag)  case 2:");
                    Play_Left_Click__STUB();
                    // DNE    Reset_Draw_Active_Stack();
                    Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, 8, 0, 0);  /* Action 8: 'Purify' */
                    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
                    Main_Screen_Reset();
                    if(all_units_moved == ST_TRUE)
                    {
                        // DNE  Set_Unit_Draw_Priority();
                        // DNE  Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                        screen_changed = ST_TRUE;
                    }

                } break;
                case 3:
                {
                    DLOG("switch(special_action_flag)  case 3:");

                } break;
                case 4:
                {
                    DLOG("switch(special_action_flag)  case 4:");

                } break;
                case 5:
                {
                    DLOG("switch(special_action_flag)  case 5:");

                } break;
                case 6:
                {
                    DLOG("switch(special_action_flag)  case 6:");

                } break;
                case 7:
                {
                    DLOG("switch(special_action_flag)  case 7:");

                } break;
                case 8:
                {
                    DLOG("switch(special_action_flag)  case 8:");

                } break;
                case 9:  /* Meld */
                {
                    DLOG("switch(special_action_flag)  case 9:");
                    STK_MeldWithNode();
                    o62p01_empty_function(_human_player_idx);
                    Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, selected_unit_wx, selected_unit_wy);
                    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
                    Main_Screen_Reset();
                } break;
            }
        }

        if(input_field_idx == _done_button)
        {
            Play_Left_Click__STUB();
            Reset_Draw_Active_Stack();
            Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, 4, 0, 0);  /* Action 4: 'Done' */
            WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
            Main_Screen_Reset();
            if(all_units_moved == ST_TRUE)
            {
                Set_Unit_Draw_Priority();
                Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                screen_changed = ST_TRUE;
            }
        }

        if(input_field_idx == _wait_button)
        {
            Play_Left_Click__STUB();
            Reset_Draw_Active_Stack();
            Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, 5, 0, 0);  /* Action 5: 'Wait' */
            WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
            Main_Screen_Reset();
            if(all_units_moved == ST_TRUE)
            {
                Set_Unit_Draw_Priority();
                Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                screen_changed = ST_TRUE;
            }
        }

        /*
            BEGIN: Game Buttons - Plane Button
        */
        if(input_field_idx == _plane_button)
        {
            Play_Left_Click__STUB();
            Do_Plane_Button__WIP(_human_player_idx, &_map_x, &_map_y, &_map_plane);
            Main_Screen_Reset();
            Reset_Map_Draw();
            MainScr_Create_Reduced_Map_Picture();
            Deactivate_Auto_Function();
            Assign_Auto_Function(Main_Screen_Draw, 1);
            Set_Mouse_List_Default();
        }
        /*
            END: Game Button - Plane Button
        */



        /*
            BEGIN: Left-Click Unit Window Grid Field
        */
        // TODO:  rename g_unit_window_fields to something like _main_map_grid_field
        // Custom Color: 255, 192, 128 ~light orange  DESC:"input field select relation - secondary/ignoreable/background noise"
        // Node Group Name: Left-Click on Unit/Stack
        // Left-Click Unit Window Grid Field
        // Left-Click Unit Window
        // ovr057:097B
        // Main_Screen+097B
        for(Stack_Index = 0; Stack_Index < _unit_stack_count; Stack_Index++)
        {
            if(g_unit_window_fields[Stack_Index] == input_field_idx)
            {
                Play_Left_Click__DUPE();
                // HERE:  itr_stack ~== unit window field idx
                Update_Stack_Active(Stack_Index);
                Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                UU_MainScreen_flag = ST_TRUE;
            }
        }
        /*
            END: Left-Click Unit Window Grid Field
        */
        
        /*
            BEGIN: Right-Click Unit Window Grid Field
        */
        {
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                if(g_unit_window_fields[itr_stack] == -(input_field_idx))
                {
                    DLOG("(g_unit_window_fields[Stack_Index] = -input_field_idx)");
                    Set_Draw_Active_Stack_Always();
                    Set_Unit_Draw_Priority();
                    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                    Main_Screen_Draw();
                    PageFlip_FX();

                    Unit_Window_Picture_Coords(itr_stack, &target_world_x, &target_world_y, &usw_x2, &usw_y2);

                    // NOTE(JimBalcomb,20230802): this here looks like what I just saw for clicking the Hero Picture on the Items Screen
                    //                            so, YayNayMay Unit_Window_Picture_Coords() is just getting the coords for the grow-out pop-up effect

                    USW_FullDisplay(_unit_stack[itr_stack].unit_idx, target_world_x, target_world_y, (target_world_x + 18), (target_world_y + 18));

                    Assign_Auto_Function(Main_Screen_Draw, 1);
                    Allocate_Reduced_Map();
                    Set_Mouse_List_Default();
                    Reset_Draw_Active_Stack();
                    Set_Unit_Draw_Priority();
                    Reset_Stack_Draw_Priority();
                    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                    Reset_Map_Draw();
                    MainScr_Create_Reduced_Map_Picture();
                    screen_changed = ST_TRUE;
                    Deactivate_Help_List();
                    Set_Main_Screen_Help_List();
                }
            }
        }
        /*
            END: Right-Click Unit Window Grid Field
        */



        /*
            BEGIN: Next Turn
        */

        // Next Turn Button ... and time-stuff ? end of turn wait ?
        // IDA: Main_Screen+AB0
        // @@Check_Input_NextTurnButton

        if(
            (input_field_idx == _next_turn_button) ||
            ((g_TimeStop_PlayerNum > 0) && ((_human_player_idx + 1) != g_TimeStop_PlayerNum)) ||
            ((all_units_moved == ST_TRUE) && (IDK_EoT_flag == ST_FALSE) && (magic_set.EoT_Wait == ST_FALSE) && (Any_Units_Not_Busy() == ST_TRUE))
        )
        {

            Play_Left_Click__STUB();

            Player_Resource_Income_Total(_human_player_idx, &eot_gold_nop, &eot_food, &eot_mana_nop);

            screen_changed = ST_TRUE;

            if(eot_food < 0)
            {
                if( (input_field_idx == _next_turn_button) || (IDK_EoT_flag == ST_TRUE) )
                {
                    if( (g_TimeStop_PlayerNum > 0) && ((_human_player_idx + 1) != g_TimeStop_PlayerNum) )
                    {
                        allow_units_to_die = ST_TRUE;
                    }
                    else
                    {
                        if(eot_food < 0)
                        {
                            // "Some units do not have enough food and will die unless you allocate more farmers in a city.  Do you wish to allow them to die?"
                            //   [YES]  [NO]
                            // ; loads and displays the provided message in a
                            // ; confirmation dialog box, providing "Yes" and "No"
                            // ; option buttons, and halting all other screen
                            // ; animation until a result is selected
                            // ; returns 1 if "Yes" is selected, or 0 otherwise
                            allow_units_to_die = Confirmation_Box(_msg_not_enough_food_units_will_die);
                        }
                    }

                    if(allow_units_to_die != ST_TRUE)
                    {
                        IDK_EoT_flag = ST_TRUE;
                    }
                    else
                    {
                        leave_screen_flag = ST_UNDEFINED;
                        current_screen = ST_UNDEFINED;
                        Main_Screen_Draw();
                        Clear_Fields();
                        PageFlip_FX();
                        current_screen = scr_NextTurn;
                        IDK_EoT_flag = ST_FALSE;
                    }
                }
                else
                {
                    IDK_EoT_flag = ST_TRUE;
                }
            }
            else  /* (eot_food >= 0) */
            {
                current_screen = ST_UNDEFINED;
                Main_Screen_Draw();
                Clear_Fields();
                PageFlip_FX();
                current_screen = scr_NextTurn;
                IDK_EoT_flag = ST_FALSE;
                leave_screen_flag = ST_UNDEFINED;
            }

            Assign_Auto_Function(Main_Screen_Draw, 1);
        }

        /*
            END: Next Turn
        */



        /*
            BEGIN: Direction Keys
        */

        // @@BEGIN_DirectionKeys
        // Main_Screen+BC2

        // hotkey_idx_Up    NumPad 8    ↑
        // hotkey_idx_Home  NumPad 7    ↖
        // hotkey_idx_PgUp  NumPad 9    ↗
        // hotkey_idx_Left  NumPad 4    ←
        // hotkey_idx_Right NumPad 6    →
        // hotkey_idx_End   NumPad 1    ↙
        // hotkey_idx_Down  NumPad 2    ↓
        // hotkey_idx_PgDn  NumPad 3    ↘
        if(input_field_idx == hotkey_idx_Up)         { Move_Stack_DirKey(8); screen_changed = ST_TRUE; }
        if(input_field_idx == hotkey_idx_LeftUp)     { Move_Stack_DirKey(7); screen_changed = ST_TRUE; }
        if(input_field_idx == hotkey_idx_RightUp)    { Move_Stack_DirKey(9); screen_changed = ST_TRUE; }
        if(input_field_idx == hotkey_idx_Left)       { Move_Stack_DirKey(4); screen_changed = ST_TRUE; }
        if(input_field_idx == hotkey_idx_Right)      { Move_Stack_DirKey(6); screen_changed = ST_TRUE; }
        if(input_field_idx == hotkey_idx_LeftDown)   { Move_Stack_DirKey(1); screen_changed = ST_TRUE; }
        if(input_field_idx == hotkey_idx_Down)       { Move_Stack_DirKey(2); screen_changed = ST_TRUE; }
        if(input_field_idx == hotkey_idx_RightDown)  { Move_Stack_DirKey(3); screen_changed = ST_TRUE; }

        /*
            END: Direction Keys
        */



        /*
            Movement Map Grid Field
                Left-Click
                Right-Click
        */

        /*
            Left-Click Movement Map Grid Field
                In IDA, color #42 (~Gold)
            
            Do Nothing
            Set Path
            Move Stack  (if stack active)
        */
        if((input_field_idx == _main_map_grid_field) && (_unit_stack_count != 0))
        {
            unit_stack_hmoves = Stack_Moves_Active();  // stack movement points to use for move stack
            if(unit_stack_hmoves < 1)
            {
                Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, us_GOTO, ((_map_x + _main_map_grid_x) % WORLD_WIDTH), ((_map_y + _main_map_grid_y) % WORLD_HEIGHT));
                unit_idx = _unit_stack[0].unit_idx;
                Set_Active_Stack_Movement_Path(unit_idx);
                if(_active_stack_path_length < 1)
                {
                    Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, us_Ready, ((_map_x + _main_map_grid_x) % WORLD_WIDTH), ((_map_y + _main_map_grid_y) % WORLD_HEIGHT));
                }
                Set_Mouse_List_Default();
                Reset_Map_Draw();
                // TODO  UU_MainScreen_flag == 1;
            }

            if(_active_stack_has_path == ST_TRUE)
            {
                // TODO  UU_MainScreen_flag = 1

                unit_idx = _unit_stack[0].unit_idx;

                if(_UNITS[unit_idx].wp == _map_plane)
                {
                    unit_stack_world_y = _UNITS[unit_idx].wy;

                    if(unit_stack_world_y >= _map_y && unit_stack_world_y < (_map_y + MAP_HEIGHT))
                    {
                        IDK_unit_stack_in_view = ST_FALSE;

                        unit_stack_world_x = _UNITS[unit_idx].wx;
                        if(
                            ( unit_stack_world_x >= _map_x && unit_stack_world_x < (_map_x + MAP_WIDTH) ) ||
                            ( (unit_stack_world_x + WORLD_WIDTH) >= _map_x && (unit_stack_world_x + WORLD_WIDTH) < (_map_x + MAP_WIDTH) )
                        )
                        {
                            unit_stack_world_x -= _map_x;
                            IDK_unit_stack_in_view = ST_TRUE;
                        }

                        if(IDK_unit_stack_in_view == ST_TRUE)
                        {
                            if(_main_map_grid_x == unit_stack_world_x)
                            {
                                unit_stack_world_y -= _map_y;
                                if(_main_map_grid_y == unit_stack_world_y)
                                {
                                    Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, us_Ready, 0, 0);
                                    // TODO  CRP_OverlandVar_3 == ST_FALSE;
                                    _active_stack_has_path = ST_FALSE;
                                }
                            }


                        }
                    }
                }
            }
            else
            {
/*
    BEGIN:  Left-Click Move Stack
*/
                // TODO  UU_MainScreen_flag = 1;
                if(all_units_moved == ST_FALSE)
                {
                    target_world_x = ((_map_x + _main_map_grid_x) % WORLD_WIDTH);
                    target_world_y =  (_map_y + _main_map_grid_y);
                    if(EarthGateTeleport__WIP(target_world_x, target_world_y, _map_plane) == ST_FALSE)
                    {
                        if(
                            (Stack_Has_PlanarTravel() == ST_TRUE) ||
                            (_UNITS[_unit_stack[0].unit_idx].wp == _map_plane) ||
                            (_UNITS[_unit_stack[0].unit_idx].in_tower == ST_TRUE)
                        )
                        {
                            assert(_map_x >= WORLD_X_MIN && _map_x <= WORLD_X_MAX);  /*  0 & 59 */
                            assert(_map_y >= WORLD_Y_MIN && _map_y <= WORLD_Y_MAX);  /*  0 & 39 */

                            Move_Stack(target_world_x, target_world_y, _human_player_idx, &_map_x, &_map_y, &_map_plane);
                        }
                        else
                        {
                            Warn0(cstrWarnNoPlaneMove);  // "The selected units cannot move on this plane."
                        }
                    }
                    Main_Screen_Reset();
                }
/*
    END:  Left-Click Move Stack
*/
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
            entity_idx = entities_on_movement_map[( (_main_map_grid_y * MAP_WIDTH) + _main_map_grid_x )];

            if(entity_idx == ST_UNDEFINED)
            {
                _prev_world_x += (_main_map_grid_x - (MAP_WIDTH  / 2));
                _prev_world_y += (_main_map_grid_y - (MAP_HEIGHT / 2));
                IDK_CheckSet_MapDisplay_XY();
            }
            else
            {
                Play_Left_Click__DUPE();

                entity_idx = abs(entity_idx);
                // entity_idx = entity_idx > 0 ? entity_idx : entity_idx*-1;

                if(entity_idx < 1000)
                {
                    /* #### Section 9.2.2      Right-Click Movement Map - Stack */
                    if(_UNITS[entity_idx].owner_idx == _human_player_idx)
                    {
                        /* #### Section 9.2.2.1      Right-Click Movement Map - Stack - Own */
                        _unit = entity_idx;
                        unit_idx = _unit;
                        selected_unit_wx = _UNITS[unit_idx].wx;
                        selected_unit_wy = _UNITS[unit_idx].wy;
                        _active_world_x = _UNITS[unit_idx].wx;
                        _active_world_y = _UNITS[unit_idx].wy;
                        Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, selected_unit_wx, selected_unit_wy);
                        if(all_units_moved != ST_TRUE)
                        {
                            all_units_moved = ST_FALSE;
                        }
                        Reset_Draw_Active_Stack();
                        draw_active_stack_flag = 0;
                        Set_Unit_Draw_Priority();
                        Reset_Stack_Draw_Priority();
                        Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                        Set_Mouse_List_Default();
                        Reset_Map_Draw();
                    }
                    else
                    {
                        /* #### Section 9.2.2.2      Right-Click Movement Map - Stack - Other */
                        target_world_x = (_main_map_grid_x * SQUARE_WIDTH);
                        target_world_y = (_main_map_grid_y * SQUARE_HEIGHT);
                        Deactivate_Help_List();
                        Unit_List_Window(entity_idx, 0, target_world_x, target_world_y);
                        Assign_Auto_Function(Main_Screen_Draw, 1);
                        Allocate_Reduced_Map();
                        Set_Mouse_List_Default();
                        Reset_Map_Draw();
                        MainScr_Create_Reduced_Map_Picture();
                        Deactivate_Help_List();
                        Set_Main_Screen_Help_List();
                    }

                    screen_changed = ST_TRUE;
                    UU_MainScreen_flag = ST_TRUE;
                }
                else if(entity_idx < 1100)
                {
                    _city_idx = (entity_idx - 1000);
                    if(_CITIES[_city_idx].owner_idx == _human_player_idx)
                    {
                        if(_CITIES[_city_idx].size == 0)
                        {
                            /* #### Section 9.2.3.1.1      Right-Click Movement Map - City - Own - Outpost */
                            PageFlipEffect = 4;
                            GrowOutLeft = (_main_map_grid_x * SQUARE_WIDTH);
                            GrowOutTop = (MAP_SCREEN_Y + (_main_map_grid_y * SQUARE_HEIGHT));
                            GrowOutFrames = 8;
                            Deactivate_Help_List();
                            target_world_x = (_main_map_grid_x * SQUARE_WIDTH);
                            target_world_y = (MAP_SCREEN_Y + (_main_map_grid_y * SQUARE_HEIGHT));
                            // ; displays and processes the outpost screen - if the view type is 1, the header calls for naming the new outpost and does not display the units on the tile
                            // ; BUG: fails to draw the altered backgrounds of the Gaia's Blessing, Flying Fortress, Famine, and Cursed Lands enchantments
                            Outpost_Screen(ST_FALSE);
                            Assign_Auto_Function(Main_Screen_Draw, 1);
                            Allocate_Reduced_Map();
                            Set_Mouse_List_Default();
                            Reset_Map_Draw();
                            MainScr_Create_Reduced_Map_Picture();
                            screen_changed = ST_TRUE;
                            Deactivate_Help_List();
                            Set_Main_Screen_Help_List();
                            UU_MainScreen_flag = ST_TRUE;





                        }
                        else
                        {
                            PageFlipEffect = 4;
                            // TODO  GrowOutLeft = (_main_map_grid_x * 20);
                            // TODO  GrowOutTop = ((_main_map_grid_y * 18) + 20);
                            // TODO  GrowOutFrames = 8;
                            current_screen = scr_City_Screen;
                            leave_screen_flag = ST_TRUE;

                        }
                    }
                    else
                    {
                        Deactivate_Help_List();
                        Enemy_City_Screen();
                        Assign_Auto_Function(Main_Screen_Draw, 1);
                        Allocate_Reduced_Map();
                        Set_Mouse_List_Default();
                        Reset_Map_Draw();
                        MainScr_Create_Reduced_Map_Picture();
                        screen_changed = ST_TRUE;
                        Deactivate_Help_List();
                        Set_Main_Screen_Help_List();
                        UU_MainScreen_flag = ST_TRUE;
                    }

                }

            }

       }



        /*
            BEGIN: Reduced Map Grid Field
                In IDA, custom color #1 (~MoM's Lightest Purpleish (154,117,190))
        */
        // Left-Click Reduced Map Grid Field
        // Right-Click Reduced Map Grid Field

        if(abs(input_field_idx) == _minimap_grid_field)
        {
            Play_Left_Click__STUB();
            // HERE: target_world_x,y are not *used*, just in-out'd to the x,y for the reduced map window, in world coordinates
            Reduced_Map_Coords(&reduced_map_window_wx, &reduced_map_window_wy, ((_map_x + (MAP_WIDTH / 2)) % WORLD_WIDTH), (_map_y + (MAP_HEIGHT / 2)), REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT);
            _prev_world_x = reduced_map_window_wx + _minimap_grid_x;  // ...is the 'wx' of the clicked square
            _prev_world_y = reduced_map_window_wy + _minimap_grid_y;  // ...is the 'wy' of the clicked square
            _map_x = _prev_world_x;
            _map_y = _prev_world_y;
            Center_Map(&_map_x, &_map_y, _prev_world_x, _prev_world_y, _map_plane);
            // Hot-Key 'C'
                // unit_idx = _unit_stack[0].unit_idx;
                // _map_plane = _UNITS[unit_idx].wp;
                // Center_Map(&_map_x, &_map_y, _UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _UNITS[unit_idx].wp);
                // MainScr_Create_Reduced_Map_Picture();
                // Set_Mouse_List(1, mouse_list_default);
                // Reset_Map_Draw();

        }

        /*
            END: Reduced Map Grid Field
        */



        /*
            END: Mouse Click
        */

/*
    END: Check Input against Fields
*/

        }



        if((leave_screen_flag == ST_FALSE) && (screen_changed == ST_FALSE) )
        {
            Main_Screen_Draw();
            if(
                (_turn == 0)
                &&
                (_given_chance_to_rename_home_city == ST_FALSE)
            )
            {
                NameStartingCity_Dialog_Popup(0);
                Assign_Auto_Function(Main_Screen_Draw, 1);
                _given_chance_to_rename_home_city = ST_TRUE;
            }
            else
            {
                _given_chance_to_rename_home_city = ST_TRUE;
            }
            // o146p04_Empty_pFxn();
            // o108p03_Empty_pFxn();
            PageFlip_FX();
            Release_Time(1);
        }
        
        screen_changed = ST_FALSE;

    }  /* while(leave_screen_flag == ST_FALSE) */

    Deactivate_Auto_Function();
    Deactivate_Help_List();
    Reset_Window();

}


// WZD o57p02
void Main_Screen_Add_Fields(void)
{
    int16_t itr_unit_stack;

    // CRP_OVL_UU_Control_4 = INVALID_FIELD;
    // CRP_OVL_UU_Control_3 = INVALID_FIELD;
    // CRP_OVL_UU_Control_2 = INVALID_FIELD;
    // CRP_OVL_UU_Control_1 = INVALID_FIELD;
    _main_map_grid_field = INVALID_FIELD;
    _minimap_grid_field = INVALID_FIELD;
    _next_turn_button = INVALID_FIELD;
    _special_button = INVALID_FIELD;
    _patrol_button = INVALID_FIELD;
    _wait_button = INVALID_FIELD;
    _done_button = INVALID_FIELD;
    _game_button = INVALID_FIELD;
    _spells_button = INVALID_FIELD;
    _armies_button = INVALID_FIELD;
    _cities_button = INVALID_FIELD;
    _magic_button = INVALID_FIELD;
    _info_button = INVALID_FIELD;
    _plane_button = INVALID_FIELD;

    for(itr_unit_stack = 0; itr_unit_stack < 9; itr_unit_stack++)
    {
        g_unit_window_fields[itr_unit_stack] = INVALID_FIELD;
    }

    if(_unit_stack_count > 0)
    {
        Add_Unit_Window_Fields();
    }
    
    if(all_units_moved == ST_TRUE)
    {
        _next_turn_button = Add_Hidden_Field(248, 175, 313, 199, 'N', ST_UNDEFINED);

    }

    if((_map_x == _prev_world_x) && (_map_y == _prev_world_y) )
    {
        _main_map_grid_field = Add_Grid_Field(MAP_SCREEN_X, MAP_SCREEN_Y, SQUARE_WIDTH, SQUARE_HEIGHT, MAP_WIDTH, MAP_HEIGHT, &_main_map_grid_x, &_main_map_grid_y, ST_UNDEFINED);
    }

    if((_map_x == _prev_world_x) && (_map_y == _prev_world_y) )
    {
        _minimap_grid_field = Add_Grid_Field(REDUCED_MAP_SCREEN_X, REDUCED_MAP_SCREEN_Y, REDUCED_MAP_SQUARE_WIDTH, REDUCED_MAP_SQUARE_HEIGHT, REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT, &_minimap_grid_x, &_minimap_grid_y, ST_UNDEFINED);
    }

    Add_Game_Button_Fields();
    Add_Unit_Action_Fields();

}

// WZD o57p03
void Add_Unit_Action_Fields(void)
{
    if(_unit_stack_count > 0)
    {
        _done_button   = Add_Button_Field(246, 176, "", main_done_button, 'D', ST_UNDEFINED);
        _patrol_button = Add_Button_Field(280, 176, "", main_patrol_button, 0, ST_UNDEFINED);
        _wait_button   = Add_Button_Field(246, 186, "", main_wait_button, 'W', ST_UNDEFINED);
    }

    if(special_action_flag != ST_UNDEFINED)
    {
        if(special_action_flag == 2)
        {
            _special_button = Add_Button_Field(280, 186, "", main_purify_button, 0, ST_UNDEFINED);
        }
        else if(special_action_flag == 9)
        {
            _special_button = Add_Button_Field(280, 186, "", main_meld_button, 0, ST_UNDEFINED);
        }
        else
        {
            _special_button = Add_Button_Field(280, 186, "", main_build_button, 'B', ST_UNDEFINED);
        }
    }

    if(_unit_stack_count < 1)
    {
        _done_button = INVALID_FIELD;
        _patrol_button = INVALID_FIELD;
        _wait_button = INVALID_FIELD;
        _special_button = INVALID_FIELD;

        if(all_units_moved == ST_FALSE)
        {
            _wait_button = Add_Button_Field(246, 186, "", main_wait_button, 'W', ST_UNDEFINED);
        }
    }
}

// WZD o57p04
// AKA Main_Screen_Draw_Unit_Actions()
// AKA j_OVL_DrawUnitActnBtns()
void Main_Screen_Draw_Unit_Action_Buttons(void)
{

    if(_unit_stack_count > 0)
    {
        FLIC_Set_CurrentFrame(main_done_button, 0);
        FLIC_Draw(246, 176, main_done_button);
        // TODO  _help_entries.Entry_Index+46h = HLP_DONE;

        FLIC_Set_CurrentFrame(main_patrol_button, 0);
        FLIC_Draw(280, 176, main_patrol_button);
        // TODO  _help_entries.Entry_Index+50h = HLP_PATROL;

        FLIC_Set_CurrentFrame(main_wait_button, 0);
        FLIC_Draw(246, 186, main_wait_button);
        // TODO  _help_entries.Entry_Index+5Ah = ST_UNDEFINED;

        if(special_action_flag == ST_UNDEFINED)
        {
            FLIC_Set_CurrentFrame(main_lock_build_button, 0);
            FLIC_Draw(280, 186, main_lock_build_button);
            // TODO  _help_entries.Entry_Index+64h = ST_UNDEFINED;
        }
        else
        {
            if(special_action_flag == 2)
            {
                FLIC_Set_CurrentFrame(main_purify_button, 0);
                FLIC_Draw(280, 186, main_purify_button);
                // TODO  _help_entries.Entry_Index+64h = HLP_PURIFY;
            }
            else if(special_action_flag == 9)
            {
                FLIC_Set_CurrentFrame(main_meld_button, 0);
                FLIC_Draw(280, 186, main_meld_button);
                // TODO  _help_entries.Entry_Index+64h = HLP_MELD;
            }
            else
            {
                FLIC_Set_CurrentFrame(main_build_button, 0);
                FLIC_Draw(280, 186, main_build_button);
                // TODO  _help_entries.Entry_Index+64h = HLP_BUILD;
            }
        }
    }
    else
    {
        FLIC_Set_CurrentFrame(main_lock_done_button, 0);
        FLIC_Draw(246, 176, main_lock_done_button);
        // TODO  _help_entries.Entry_Index+46h = ST_UNDEFINED;

        FLIC_Set_CurrentFrame(main_lock_patrol_button, 0);
        FLIC_Draw(280, 176, main_lock_patrol_button);
        // TODO  _help_entries.Entry_Index+50h = ST_UNDEFINED;

        FLIC_Set_CurrentFrame(main_lock_build_button, 0);
        FLIC_Draw(280, 186, main_lock_build_button);
        // TODO  _help_entries.Entry_Index+64h = ST_UNDEFINED;

        FLIC_Set_CurrentFrame(main_wait_button, 0);  // BUGBUG  should be main_lock_wait_button
        FLIC_Draw(246, 186, main_wait_button);
        // TODO  _help_entries.Entry_Index+5Ah = ST_UNDEFINED;
    }

}

// WZD o57p05
// OVL_SetMenuButtons
void Add_Game_Button_Fields(void)
{
    _game_button    = Add_Button_Field(  7, 4, empty_string__ovr057, main_game_button,   hotkey_GameButton[0], ST_UNDEFINED);
    _spells_button  = Add_Button_Field( 47, 4, empty_string__ovr057, main_spells_button, hotkey_SpellsButton[0], ST_UNDEFINED);
    _armies_button  = Add_Button_Field( 89, 4, empty_string__ovr057, main_armies_button, hotkey_ArmiesButton[0], ST_UNDEFINED);
    _cities_button  = Add_Button_Field(140, 4, empty_string__ovr057, main_cities_button, empty_string__ovr057[0], ST_UNDEFINED);
    _magic_button   = Add_Button_Field(184, 4, empty_string__ovr057, main_magic_button,  hotkey_MagicButton[0], ST_UNDEFINED);
    _info_button    = Add_Button_Field(226, 4, empty_string__ovr057, main_info_button,   hotkey_InfoButton[0], ST_UNDEFINED);
    _plane_button   = Add_Button_Field(270, 4, empty_string__ovr057, main_plane_button,  hotkey_PlaneButton[0], ST_UNDEFINED);
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
    int16_t itr_stack;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        Unit_Window_Picture_Coords(itr_stack, &x1, &y1, &x2, &y2);
        g_unit_window_fields[itr_stack] = Add_Hidden_Field((x1 - 1), (y1 - 1), (x2 - 2), (y2 - 2), 0, ST_UNDEFINED);
    }
}


// WZD o57p08
void Main_Screen_Draw(void)
{
    Reset_Window();
    Set_Page_Off();

    assert(_map_x >= WORLD_X_MIN && _map_x <= WORLD_X_MAX);  /*  0 & 59 */
    assert(_map_y >= WORLD_Y_MIN && _map_y <= WORLD_Y_MAX);  /*  0 & 39 */

    Main_Screen_Draw_Do_Draw(&_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);
}


// WZD o57p09
void Main_Screen_Reset(void)
{
    Allocate_Reduced_Map();
    Deactivate_Auto_Function();
    Clear_Fields();
    Reset_Draw_Active_Stack();
    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    Set_Mouse_List_Default();
    MainScr_Create_Reduced_Map_Picture();
    Assign_Auto_Function(Main_Screen_Draw, 1);
    Reset_Map_Draw();
    Deactivate_Help_List();
    Set_Main_Screen_Help_List();
    UU_MainScreen_flag = ST_TRUE;
}

// WZD o57p10
void Move_Stack_DirKey(int16_t movement_direction)
{
    int16_t movement_points;
    int16_t move_x;
    int16_t move_y;

    if(_unit_stack_count == 0)
    {
        return;
    }

    if(movement_points = Stack_Moves_Active() == 0)
    {
        return;
    }

    if(all_units_moved == ST_FALSE)
    {
        switch(movement_direction)
        {
            case 1:  /* LeftDown  -x,+y */
            {
                move_x = ((_UNITS[_unit_stack[0].unit_idx].wx - 1) % WORLD_WIDTH);
                move_y = (_UNITS[_unit_stack[0].unit_idx].wy + 1);
            } break;
            case 2:  /* Down  +y */
            {
                move_x = (_UNITS[_unit_stack[0].unit_idx].wx);
                move_y = (_UNITS[_unit_stack[0].unit_idx].wy + 1);
            } break;
            case 3:  /* RightDown  +x,+y */
            {
                move_x = ((_UNITS[_unit_stack[0].unit_idx].wx + 1) % WORLD_WIDTH);
                move_y = (_UNITS[_unit_stack[0].unit_idx].wy + 1);
            } break;
            case 4:  /* Left  -x */
            {
                move_x = (_UNITS[_unit_stack[0].unit_idx].wx - 1);
                move_y = (_UNITS[_unit_stack[0].unit_idx].wy);
            } break;
            case 5:  /* DNE */
            {
            } break;
            case 6:  /* Right  +x */
            {
                move_x = (_UNITS[_unit_stack[0].unit_idx].wx + 1);
                move_y = (_UNITS[_unit_stack[0].unit_idx].wy);
            } break;
            case 7:  /* LeftUp  -x,-y */
            {
                move_x = ((_UNITS[_unit_stack[0].unit_idx].wx - 1) % WORLD_WIDTH);
                move_y = (_UNITS[_unit_stack[0].unit_idx].wy - 1);
            } break;
            case 8:  /* Up  -y */
            {
                move_x = (_UNITS[_unit_stack[0].unit_idx].wx);
                move_y = (_UNITS[_unit_stack[0].unit_idx].wy - 1);
            } break;
            case 9:  /* RightUp  +x,-y */
            {
                move_x = ((_UNITS[_unit_stack[0].unit_idx].wx + 1) % WORLD_WIDTH);
                move_y = (_UNITS[_unit_stack[0].unit_idx].wy - 1);
            } break;

        }

        // DELETEME  if(move_y < WORLD_Y_MIN)
        // DELETEME  {
        // DELETEME      move_y = WORLD_Y_MIN;
        // DELETEME  }
        SETMIN(move_y, WORLD_Y_MIN);

        // DELETEME  if(move_y >= WORLD_HEIGHT)
        // DELETEME  {
        // DELETEME      move_y = WORLD_Y_MAX;
        // DELETEME  }
        SETMAX(move_y, WORLD_HEIGHT);

        if(Stack_Has_PlanarTravel() == ST_TRUE)
        {
            assert(_map_x >= WORLD_X_MIN && _map_x <= WORLD_X_MAX);  /*  0 & 59 */
            assert(_map_y >= WORLD_Y_MIN && _map_y <= WORLD_Y_MAX);  /*  0 & 39 */

            Move_Stack(move_x, move_y, _human_player_idx, &_map_x, &_map_y, &_map_plane);
        }
        else
        {
            if( (_UNITS[_unit_stack[0].unit_idx].wp == _map_plane) || (_UNITS[_unit_stack[0].unit_idx].in_tower == ST_TRUE) )
            {
                assert(_map_x >= WORLD_X_MIN && _map_x <= WORLD_X_MAX);  /*  0 & 59 */
                assert(_map_y >= WORLD_Y_MIN && _map_y <= WORLD_Y_MAX);  /*  0 & 39 */

                Move_Stack(move_x, move_y, _human_player_idx, &_map_x, &_map_y, &_map_plane);
            }
            else
            {
                Warn0(cstrWarnNoPlaneMove);
            }
        }
    }

}



/*
    WIZARDS.EXE  ovr058
*/

// WZD o58p01
void Main_Screen_Draw_Unit_Window(int16_t start_x, int16_t start_y)
{
    int16_t itr_stack;
    int16_t unit_idx;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;

    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;

    if(_unit == ST_UNDEFINED || _unit_stack_count == 0)
    {
        return;
    }

    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        unit_idx = _unit_stack[itr_stack].unit_idx;

        Unit_Window_Picture_Coords(itr_stack, &x1, &y1, &x2, &y2);
        FLIC_Draw(x1-1, y1-1, unit_backgrounds[itr_stack]);
        Draw_Unit_Picture(x1, y1, unit_idx, _unit_stack[itr_stack].active);
        Unit_Window_Draw_Unit_Attributes(x1, y1, unit_idx);
    }

    Cycle_Unit_Enchantment_Animation();

    unit_weapon_type_animation_count++;
    if(unit_weapon_type_animation_count > 3)
    {
        unit_weapon_type_animation_count = 0;
    }

}


// WZD o58p02
void Set_Mouse_List_Default(void)
{
    Set_Mouse_List(1, mouse_list_default);
}


// WZD o58p03
/*
OON XREF: 
Move_Stack() |-> j_o58p03_Empty_pFxn() |-> o58p03_empty_function()
called when CRP_OVL_Obstacle_Var1 == ST_FALSE
*/
void o58p03_empty_function(void)
{
// push    bp
// mov     bp, sp
// pop     bp
// retf
}

// WZD o58p04
// drake178: sub_518A3()
/*
NOT IN USE

¿ would control whether o58p03_empty_function() gets called from Move_Stack() ?

XREF:
    Falsify_CRP_OVL_Obstacle_Var1()
        NX_j_Falsify_CRP_OVL_Obstacle_Var1()
*/
void Falsify_CRP_OVL_Obstacle_Var1(void)
{
    CRP_OVL_Obstacle_Var1 = ST_FALSE;
}

// WZD o58p05
// drake178: ¿ ?
void Increment_Background_Music(void)
{
    int16_t background_music_num;

    if(m_background_music_ctr > 3)
    {
        m_background_music_ctr = 0;

        background_music_num = Get_Background_Music();

        if(background_music_num != m_background_music_num)
        {
            Play_Background_Music__STUB();
        }

    }
    else
    {
        m_background_music_ctr++;
    }

}


// WZD o58p06
// drake178: SND_GetBkgrndTone()
/*
; returns the entry index of one of the three main
; background music tracks, based on the player's
; performance versus the best and worst computer
; players (negative is below 110% of the worst power,
; good is above 90% of the best AI, using historian)
*/
int16_t Get_Background_Music(void)
{
    int16_t Highest_Power;
    int16_t background_music_num;
    int16_t itr_players;  // _SI_
    int16_t IDK;  // _CX_
    int16_t wizards_power;  // _DI_

    background_music_num = MUSIC_Bkgrnd_Neutral;

    if(_turn < 288)
    {
        IDK = (_turn - 1);
    }
    else
    {
        IDK = 287;
    }

    SETMIN(IDK, 0);

    Highest_Power = 0;

    wizards_power = 255;

    for(itr_players = 1; itr_players < _num_players; itr_players++);
    {

        if(_players[itr_players].Historian[IDK] > Highest_Power)
        {
            Highest_Power = _players[itr_players].Historian[IDK];
        }

        if(_players[itr_players].Historian[IDK] < wizards_power)
        {
            wizards_power = _players[itr_players].Historian[IDK];
        }

    }

    if(wizards_power == 255)
    {
        wizards_power = 0;
    }

    if((_players[_human_player_idx].Historian[IDK] + (Highest_Power / 10)) >= Highest_Power)
    {
        background_music_num = MUSIC_Bkrgnd_Good;
    }

    if(wizards_power > 0)
    {
        if((_players[_human_player_idx].Historian[IDK] - (wizards_power / 10)) <= wizards_power)
        {
            background_music_num = MUSIC_Bkgrnd_Negative;
        }
    }

    return background_music_num;
}


// WZD o58p07
// drake178: SND_PlayBkgrndTrack()
/*
; stops all sound playback and, if background music is
; enabled, selects and starts playing the currently
; appropriate background track based on player power
*/
void Play_Background_Music__STUB(void)
{
    SAMB_ptr sound_seg;
    int16_t background_music_num;  // _SI_

    if(magic_set.background_music == ST_TRUE)
    {
        Stop_All_Sounds__STUB();

        background_music_num = Get_Background_Music();

        m_background_music_num = background_music_num;

        sound_seg = LBX_Reload(music_lbx_file__ovr058, background_music_num, SND_Music_Segment);

        Play_Sound__STUB(sound_seg);

    }
    else
    {
        Stop_All_Sounds__STUB();
    }


}


// WZD o58p08
// drake178: sub_51A1A()
/*

XREF:
    Cast_SummonHero()
    Cast_Incarnation()
    Cast_Resurrection()
    Cast_Spell_Overland__WIP()
        ¿ sdt_Summoning, but not 'Floating Island' ?

*/
void IDK_HumanPlayer_SelectStack_UnitLocation(int16_t unit_idx)
{

    o62p01_empty_function(_human_player_idx);

    Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _UNITS[unit_idx].wp, _UNITS[unit_idx].wx, _UNITS[unit_idx].wy);

    Set_Unit_Draw_Priority();

    Reset_Stack_Draw_Priority();

    all_units_moved = ST_FALSE;

    Reset_Draw_Active_Stack();

    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

}


// WZD o58p09
// drake178: sub_51AA0()
/*

XREF:
    UV_Remove_Unit_Enchantment()

*/
void IDK_HumanPlayer_SelectStack_SummonLocation(void)
{

    o62p01_empty_function(_human_player_idx);

    Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _players[_human_player_idx].summon_wp, _players[_human_player_idx].summon_wx, _players[_human_player_idx].summon_wy);

    Set_Unit_Draw_Priority();

    Reset_Stack_Draw_Priority();

    all_units_moved = ST_FALSE;

    Reset_Draw_Active_Stack();

    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

}



/*
    WIZARDS.EXE  ovr062
*/

// WZD o62p01
/*
drake178: does nothing and returns zero; at some point must have been some wizard data refresh function
*/
int16_t o62p01_empty_function(int16_t player_idx)
{
// proc o62p01_empty_function far
// player_idx= word ptr  6
// push    bp
// mov     bp, sp
// xor     ax, ax
// jmp     short $+2
// pop     bp
// retf
// endp o62p01_empty_function

    return 0;

}


// WZD o62p02
/*
//drake178
builds a stack from the units on the tile, selects
the one with the highest draw priority that can move
as the current unit, brings the stack into view, sets
in_tower flags, calculates GUI_ExtraUnitAction and,
if all the units belong to the human player and have
a destination already set, loads their path into the
global arrays, setting GUI_StackHasPath to 1
*/
// MoO2  Module: SHIPSTK  Find_Ship_Stacks_()
// _NUM_SHIPS, _ship_stack_count, _PLAYER_NUM, _ship, _ship_stack_start, _ship_node
// Remove_Non_Detected_Ships_(), Sort_Ships_In_Stack_()
/*
Build_Unit_Stack() sets _unit_stack_count
then, short-circuits if(_unit_stack_count == 0)
calls Build_Unit_Stack() & Sort_Unit_Stack(); Sort_Unit_Stack() updates `_unit`
*/
void Select_Unit_Stack(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t unit_wx, int16_t unit_wy)
{
    int16_t goto_unit_idx = 0;
    int16_t itr_stack = 0;
    int16_t unit_idx = 0;
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t troop_count = 0;

    reset_active_stack = ST_TRUE;

    Build_Unit_Stack(player_idx, map_plane, unit_wx, unit_wy);  // sets _unit_stack_count

    if(_unit_stack_count == 0)
    {
        return;
    }

    Sort_Unit_Stack();

    _active_stack_has_path = ST_FALSE;

    goto_unit_idx = ST_UNDEFINED;

    if(player_idx == _human_player_idx)
    {
        for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
        {
            unit_idx = _unit_stack[itr_stack].unit_idx;
            if(_UNITS[unit_idx].Status == us_GOTO)
            {
                goto_unit_idx = unit_idx;
            }
            else
            {
                goto_unit_idx = ST_UNDEFINED;
                break;
            }
        }
    }

    if(goto_unit_idx != ST_UNDEFINED)
    {
        Set_Active_Stack_Movement_Path(goto_unit_idx);
    }

    unit_idx = _unit;
    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;

    OVL_BringIntoView(map_x, map_y, unit_wx, unit_wy, map_plane);
        
    Set_Unit_Action_Special();
        
    Active_Unit_Stack(&troop_count, &troops[0]);

    // DONT  o62p07_Empty_pFxn(unit_array_count, &unit_array[0]);

    Units_In_Tower(troop_count, &troops[0], map_plane);

    if(_unit_stack_count < 1)
    {
        _active_stack_has_path = ST_FALSE;
    }

}

// WZD o62p03
/*
OON XREF: Select_Unit_Stack()
*/
void Sort_Unit_Stack(void)
{
    int16_t itr_stack_outer;
    int16_t itr_stack_inner;
    int16_t itr_stack;
    int16_t unit_idx_1;
    int16_t unit_idx_2;
    int16_t unit_1_priority;
    int16_t unit_2_priority;
    int16_t tmp_stack_unit_idx;
    int16_t tmp_stack_active_flag;

    for(itr_stack_outer = 0; itr_stack_outer < _unit_stack_count; itr_stack_outer++)
    {
        for(itr_stack_inner = 0; itr_stack_inner < (_unit_stack_count - 1); itr_stack_inner++)
        {

            unit_idx_1 = _unit_stack[itr_stack_inner].unit_idx;
            unit_idx_2 = _unit_stack[itr_stack_inner + 1].unit_idx;

            // ¿ Melee Attack Strength + Ranged Attack Strength ?
            // ...index into the _unit_type_table[] based on the _UNITS[].type

            unit_1_priority = _unit_type_table[_UNITS[unit_idx_1].type].Melee + _unit_type_table[_UNITS[unit_idx_1].type].Ranged;

            unit_2_priority = _unit_type_table[_UNITS[unit_idx_2].type].Melee + _unit_type_table[_UNITS[unit_idx_2].type].Ranged;

            if(_unit_type_table[_UNITS[unit_idx_1].type].Transport > 0)
            {
                unit_1_priority = 50;  // ¿ Maximum Stack Sort Priority ?
            }

            if(_unit_type_table[_UNITS[unit_idx_2].type].Transport > 0)
            {
                unit_2_priority = 50;  // ¿ Maximum Stack Sort Priority ?
            }

            if(unit_1_priority < unit_2_priority)
            {
                // ; exchanges an amount of bytes between near addresses
                // MEM_SwapBytes(_unit_stack[unit_idx_2], _unit_stack[unit_idx_2 + 1], 4);

                tmp_stack_unit_idx = _unit_stack[itr_stack_inner].unit_idx;
                tmp_stack_active_flag = _unit_stack[itr_stack_inner].active;
                _unit_stack[itr_stack_inner].unit_idx = _unit_stack[itr_stack_inner + 1].unit_idx;
                _unit_stack[itr_stack_inner].active = _unit_stack[itr_stack_inner + 1].active;
                _unit_stack[itr_stack_inner + 1].unit_idx = tmp_stack_unit_idx;
                _unit_stack[itr_stack_inner + 1].active = tmp_stack_active_flag;

            }

        }
    }

    _unit = _unit_stack[0].unit_idx;
    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        if(_unit_stack[itr_stack].active == ST_TRUE)
        {
            _unit = _unit_stack[itr_stack].unit_idx;
            break;
        }
    }

}

// WZD o62p04
/*
sets _unit_stack_count
populates _unit_stack[]
OON XREF:  Select_Unit_Stack()

if us_Wait then us_Ready
if us_Purify then active = ST_FALSE
*/
void Build_Unit_Stack(int16_t player_idx, int16_t wp, int16_t wx, int16_t wy)
{
    int16_t unit_stack_idx;  // _DI_
    int16_t itr_units;  // _SI_
    int16_t unit_idx;  // _DX_
    int16_t itr;  // _SI_

    unit_stack_idx = 0;

    for(itr = 0; itr < MAX_STACK; itr++)
    {
        _unit_stack[itr].active = ST_FALSE;
    }

    for (itr_units = 0; itr_units < _units; itr_units++)
    {
        unit_idx = itr_units;

        // ¿ if NOT then continue ?
        if(
            (_UNITS[unit_idx].owner_idx == player_idx)
            &&
            (_UNITS[unit_idx].wx == wx)
            &&
            (_UNITS[unit_idx].wy == wy)
            &&
            (_UNITS[unit_idx].owner_idx != ST_UNDEFINED)
            &&
            (
                (_UNITS[unit_idx].wp == wp)
                ||
                (_UNITS[unit_idx].in_tower == ST_TRUE)
            )
        )
        {
            _unit_stack[unit_stack_idx].unit_idx = unit_idx;

            if((_UNITS[unit_idx].moves2 > 0) && (_UNITS[unit_idx].Finished == ST_FALSE))
            {
                _unit_stack[unit_stack_idx].active = ST_TRUE;

                if(_UNITS[unit_idx].Status == us_Wait)
                {
                    _UNITS[unit_idx].Status = us_Ready;
                }
            }

            if(_UNITS[unit_idx].Status == us_Purify)
            {
                _unit_stack[unit_stack_idx].active = ST_FALSE;
            }

            unit_stack_idx++;
            assert(unit_stack_idx <= MAX_STACK);
        }

    }

    _unit_stack_count = unit_stack_idx;

}

// WZD o62p05
// Select_Unit_Stack(... *map_x, *map_y, map_plane ...) |-> OVL_BringIntoView(*map_x, *map_y, unit_x, unit_y, map_plane);
// MoO2  ¿ Star_On_Screen_(); Ship_Completely_On_Screen_(); Ship_On_Screen_(); ?
void OVL_BringIntoView(int16_t *map_x, int16_t *map_y, int16_t unit_x, int16_t unit_y, int16_t map_plane)
{
    int16_t in_view;
    int16_t l_unit_y;
    int16_t l_unit_x;

    // int16_t l_map_x;  // _SI_
    // int16_t l_map_y;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: OVL_BringIntoView(*map_x = %d, *map_y = %d, unit_x = %d, unit_y = %d, map_plane = %d)\n", __FILE__, __LINE__, *map_x, *map_y, unit_x, unit_y, map_plane);
#endif

    // l_map_x = *map_x;
    // l_map_y = *map_y;

    l_unit_y = unit_y;

    in_view = ST_FALSE;

    // test in-bounds
    if(
        (l_unit_y >= *map_y)
        &&
        (l_unit_y < (*map_y + MAP_HEIGHT))
    )
    {
        l_unit_x = unit_x;
        if(
            (
                (l_unit_x >= *map_x)
                &&
                (l_unit_x < *map_x + MAP_WIDTH)
            )
            ||
            (
                ((l_unit_x + WORLD_WIDTH) >= *map_x)
                &&
                ((l_unit_x + WORLD_WIDTH) < (*map_x + MAP_WIDTH))
            )
        )
        {
            if(
                ((l_unit_x + WORLD_WIDTH) >= *map_x)
                &&
                ((l_unit_x + WORLD_WIDTH) < (*map_x + MAP_WIDTH))
            )
            {
                l_unit_x += WORLD_WIDTH;
            }

            l_unit_x = l_unit_x - *map_x;  // translate unit's x coordinate from the world view to the map view
            in_view = ST_TRUE;
        }
    }

    if(in_view == ST_FALSE)
    {
        // ¿ same logic as Center_Map() ?
        l_unit_x = unit_x - (MAP_WIDTH / 2);  // get the wx for the map, assuming we're centering on the unit
        if(l_unit_x > 0)
        {
            *map_x = l_unit_x;
        }
        else
        {
            *map_x = ((l_unit_x + WORLD_WIDTH) % WORLD_WIDTH);
        }
        l_unit_y = unit_y - (MAP_HEIGHT / 2);  // get the wy for the map, assuming we're centering on the unit
        if(l_unit_y > 0)
        {
            *map_y = l_unit_y;
        }
        else
        {
            *map_y = 0;
        }
        if(*map_y + MAP_HEIGHT >= WORLD_HEIGHT)
        {
            *map_y = (WORLD_HEIGHT - MAP_HEIGHT);  // (40 - 10) = 30
        }

        _prev_world_x = *map_x;
        _prev_world_y = *map_y;

        // CRP_OVL_MapWindowX = _prev_world_x * SQUARE_WIDTH;
        // CRP_OVL_MapWindowY = _prev_world_y * SQUARE_HEIGHT;

    }
    else
    {
        // HERE: map_x, map_y, unit_x, unit_y are as they were passed in
        OVL_TileOffScrnEdge(map_x, map_y, unit_x, unit_y, MAP_WIDTH, MAP_HEIGHT);
    }

    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    Set_Entities_On_Map_Window(*map_x, *map_y, map_plane);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: OVL_BringIntoView(*map_x = %d, *map_y = %d, unit_x = %d, unit_y = %d, map_plane = %d)\n", __FILE__, __LINE__, *map_x, *map_y, unit_x, unit_y, map_plane);
#endif

}



// WZD o62p06
// drake178: OVL_TileOffScrnEdge()
// MoO2  ¿ Star_On_Screen_(); Ship_Completely_On_Screen_(); Ship_On_Screen_(); ?
/*
    tests if Stack is in First or Last Column and Last or First Row
    prev_world_x,y to scroll Movement Map 1 world-sqaure right, left, down, or up
*/
int16_t OVL_TileOffScrnEdge(int16_t *map_x, int16_t *map_y, int16_t unit_x, int16_t unit_y, int16_t map_width, int16_t map_height)
{
    // int16_t l_map_x;  // _SI_
    // int16_t l_map_y;  // _DI_
    int16_t off_map_flag;  // _CX_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: OVL_TileOffScrnEdge(*map_x = %d, *map_y = %d, unit_x = %d, unit_y = %d, map_width = %d, map_height = %d)\n", __FILE__, __LINE__, *map_x, *map_y, unit_x, unit_y, map_width, map_height);
#endif

    // l_map_x = map_x;
    // l_map_y = map_y;

    off_map_flag = ST_FALSE;

    // ¿ Stack is in Fst Column of Movement Map ?
    if(*map_x == unit_x)
    {
        // ¿ map_x == 0 ?
        *map_x = *map_x - 1;  // move Map left 1 world-square
        if(*map_x < 0)
        {
            *map_x = *map_x + WORLD_WIDTH;  // move Map right 1 world-square
        }
        off_map_flag = ST_TRUE;  // set flag - map x,y changed
    }

    // ¿ Stack is in Lst Column of Movement Map ?
    if(((*map_x + map_width - 1) % WORLD_WIDTH) == unit_x)
    {
        *map_x = ((*map_x + 1) % WORLD_WIDTH);
        off_map_flag = ST_TRUE;  // set flag - map x,y changed
    }

    // ¿ Stack is in Lst Row of Movement Map ?
    if((*map_y + map_height - 1) == unit_y)
    {
        // ¿ Movement Map is not already at the botton edge of the World  i.e., the map is able to be moved down ?
        if((*map_y + map_height + 1) < WORLD_HEIGHT)
        {
            *map_y = *map_y + 1;  // move Map down 1 world-square
            off_map_flag = ST_TRUE;  // set flag - map x,y changed
        }
    }

    // ¿ Stack is in Fst Row of Movement Map ?
    if(*map_y == unit_y)
    {
        // ¿ Movement Map is not already at the top edge of the World  i.e., the map is able to be moved up ?
        if(*map_y != 0)
        {
            *map_y = *map_y - 1;  // move Map up 1 world-square
            off_map_flag = ST_TRUE;  // set flag - map x,y changed
        }
    }

    if(off_map_flag == ST_TRUE)
    {
        _prev_world_x = *map_x;
        _prev_world_y = *map_y;

        // NIU? CRP_OVL_MapWindowX = _prev_world_x * SQUARE_WIDTH;
        // NIU? CRP_OVL_MapWindowY = _prev_world_y * SQUARE_HEIGHT;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: OVL_TileOffScrnEdge(*map_x = %d, *map_y = %d, unit_x = %d, unit_y = %d, map_width = %d, map_height = %d) { off_map_flag = %d }\n", __FILE__, __LINE__, *map_x, *map_y, unit_x, unit_y, map_width, map_height, off_map_flag);
#endif

    return off_map_flag;
}


// WZD o62p07
// DONT  int16_t o62p07_Empty_pFxn(int16_t unit_array_count, int16_t * unit_array);


// WZD o62p08
/*
    Handle 'Unit Action' buttons/hotkeys

    no action 2, 6, 7
    action 1 sets status to action and sets inactive and unavailable
    destination_x, destination_y only used for Go-To, action 3

    Unit Action Patrol  calls with action 1
    Unit Action Done    calls with action 4
    Unit Action Wait    calls with action 5
    ¿ Special Unit Actions ? ¿ Settle, Meld, Build Road(s) ?

    action 0:  set Ready / Doing Nothing and available/unavailable; if unavailable, status is Reached Destination / Completed Action
    action 1:  if available, set status = action and set unavailable
    action 2:  DNE
    action 3:  Go-To
    action 4:  Reached Destination / Completed Action
    action 5:  Wait  if active and available, set status and inactive  (¿ unsets destination x,y ?)
    action 6:  DNE
    action 7:  DNE
    action 8:  Purify 

*/
void Stack_Action(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p, int16_t action, int16_t destination_x, int16_t destination_y)
{
    int16_t unit_type;
    int16_t case_1_count;
    int16_t itr_stack;
    int16_t unit_idx;

    case_1_count = 0;

// mov     [OVL_MapVar3], 1
// mov     [CRP_OVL_Obstacle_Var1], 0

    /*
        ¿ no 2 or 7 ?
        ¿ cmp's 8, says switch 9 cases ?
        ¿ says default is 6 ?
        shl, but no sub 1
    */
    switch(action)
    {
        case 0:  /* Ready */
        {
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                unit_idx = _unit_stack[itr_stack].unit_idx;
                _UNITS[unit_idx].Status = 0;  /* us_Ready */
                _UNITS[unit_idx].Rd_Constr_Left = ST_UNDEFINED;
                if(_UNITS[unit_idx].moves2 > 0)
                {
                    _unit_stack[itr_stack].active = ST_TRUE;
                    _UNITS[unit_idx].Finished = ST_FALSE;
                }
                else
                {
                    _UNITS[unit_idx].Status = 4;  /* "DONE"  us_ReachedDest */
                    _UNITS[unit_idx].Finished = ST_TRUE;
                }
                _UNITS[unit_idx].dst_wx = 0;
                _UNITS[unit_idx].dst_wy = 0;
            }
        } break;
        case 1:
        {
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                unit_idx = _unit_stack[itr_stack].unit_idx;
                if(
                    (_unit_stack[itr_stack].active == ST_TRUE) &&
                    (_UNITS[unit_idx].moves2 > 0) &&
                    (_UNITS[unit_idx].Finished == ST_FALSE)
                )
                {
                    _UNITS[unit_idx].Status = action;
                    _UNITS[unit_idx].Finished = ST_TRUE;
                    _unit_stack[itr_stack].active = ST_FALSE;
                    _UNITS[unit_idx].dst_wx = 0;
                    _UNITS[unit_idx].dst_wy = 0;
                    case_1_count++;
                }

            }
        } break;
        case 2:
        {
        } break;
        case 3:  /* Go-To */
        {
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                unit_idx = _unit_stack[itr_stack].unit_idx;
                if(
                    ((_UNITS[unit_idx].Status & 4) != 0) ||  /* "DONE"  us_ReachedDest */
                    ((_UNITS[unit_idx].Status & 3) != 0) ||  /* "GOTO"  us_GOTO */
                    ((_UNITS[unit_idx].Status & 0) != 0)     /* us_Ready */
                )
                {
                    _UNITS[unit_idx].Status = 3;  /* "GOTO"  us_GOTO */
                    _UNITS[unit_idx].dst_wx = destination_x;
                    _UNITS[unit_idx].dst_wy = destination_y;
                }
            }
        } break;
        case 4:  /* "DONE"  ¿ completed on-going action (move path, pave path, purify) ? */
        {
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                if(
                    (_unit_stack[itr_stack].active == ST_TRUE) &&
                    ((_UNITS[_unit_stack[itr_stack].unit_idx].Status & 3) == 0) &&  /* "GOTO"  us_GOTO */
                    ((_UNITS[_unit_stack[itr_stack].unit_idx].Status & 8) == 0)     /* "PURIFY"  us_Purify  */
                )
                {
                    unit_idx = _unit_stack[itr_stack].unit_idx;
                    _unit_stack[itr_stack].active = ST_FALSE;
                    _UNITS[unit_idx].Finished = ST_TRUE;
                    _UNITS[unit_idx].Status = 4;  /* "DONE"  us_ReachedDest */
                    _UNITS[unit_idx].Rd_Constr_Left = ST_UNDEFINED;
                    _UNITS[unit_idx].moves2 = 0;
                    _UNITS[unit_idx].dst_wx = 0;
                    _UNITS[unit_idx].dst_wy = 0;
                }
            }
        } break;
        case 5:  /* Wait */
        {
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                unit_idx = _unit_stack[itr_stack].unit_idx;
                if(
                    (_unit_stack[itr_stack].active == ST_TRUE)
                    &&
                    (_UNITS[unit_idx].moves2 > 0)
                    &&
                    (_UNITS[unit_idx].Finished == ST_FALSE)
                )
                {
                    _UNITS[unit_idx].Status = 5;  /* "WAIT"  us_Wait */
                    _unit_stack[itr_stack].active = ST_FALSE;
                    _UNITS[unit_idx].dst_wx = 0;
                    _UNITS[unit_idx].dst_wy = 0;
                }
            }
        } break;
        case 6:
        {

        } break;
        case 7:
        {

        } break;
        case 8:  /* Purify */
        {
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                if(_unit_stack[itr_stack].active == ST_TRUE)
                {
                    unit_idx = _unit_stack[itr_stack].unit_idx;
                    unit_type = _UNITS[unit_idx].type;
                    if((_unit_type_table[unit_type].Abilities & UA_PURIFY) != 0)
                    {
                        _unit_stack[itr_stack].active = ST_FALSE;
                        _UNITS[unit_idx].Finished = ST_TRUE;
                        _UNITS[unit_idx].Status = us_Purify;
                        _UNITS[unit_idx].moves2 = 0;
                        _UNITS[unit_idx].dst_wx = 0;
                        _UNITS[unit_idx].dst_wy = 0;
                        _UNITS[unit_idx].Rd_Constr_Left = ST_UNDEFINED;
                    }
                }
            }
        } break;

    }


    Reset_Draw_Active_Stack();
    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    Set_Entities_On_Map_Window(*map_x, *map_y, *map_p);

}


// WZD o62p09
int16_t EarthGateTeleport__WIP(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t unit_array[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t moveable_units_count = 0;
    int16_t unit_count = 0;
    int16_t Radius = 0;
    int16_t player_idx = 0;
    int16_t dst_city_idx = 0;
    int16_t did_earthgate = 0;
    int16_t src_city_idx = 0;
    int16_t unit_idx = 0;  // _SI_
    int16_t itr = 0;  // _DI_

    if(_unit_stack_count < 1)
    {
        goto Failure;
    }

    dst_city_idx = City_At_Square__2(wx, wy, wp);

    if(dst_city_idx == ST_UNDEFINED)
    {
        goto Failure;
    }

    Player_Army_At_Square(wx, wy, wp, _human_player_idx, &unit_count, &unit_array[0]);

    if(unit_count >= 9)
    {
        goto Failure;
    }

    moveable_units_count = 0;

    did_earthgate = ST_FALSE;

    unit_idx = _unit_stack[0].unit_idx;

    src_city_idx = City_At_Square__2(_UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _UNITS[unit_idx].wp);

    if(
        (dst_city_idx != ST_UNDEFINED) &&
        (_CITIES[src_city_idx].wp == _CITIES[dst_city_idx].wp) &&
        (_CITIES[src_city_idx].owner_idx == _CITIES[dst_city_idx].owner_idx) &&
        (_CITIES[src_city_idx].enchantments[EARTH_GATE] == ST_TRUE) &&
        (_CITIES[dst_city_idx].enchantments[EARTH_GATE] == ST_TRUE)
    )
    {
        for(itr = 0; itr < _unit_stack_count; itr++)
        {
            unit_idx = _unit_stack[itr].unit_idx;
            if(
                (_UNITS[unit_idx].Finished != ST_FALSE) &&
                (_unit_stack[itr].active == ST_TRUE) &&
                (_UNITS[unit_idx].moves2 > 0)
            )
            {
                moveable_units_count++;
            }
        }

        if((moveable_units_count + unit_count) <= MAX_STACK)
        {
            for(itr = 0; itr < _unit_stack_count; itr++)
            {
                unit_idx = _unit_stack[itr].unit_idx;
                if(
                    (_UNITS[unit_idx].Finished != ST_FALSE) &&
                    (_unit_stack[itr].active == ST_TRUE) &&
                    (_UNITS[unit_idx].moves2 > 0)
                )
                {
                    _UNITS[unit_idx].wx = _CITIES[dst_city_idx].wx;
                    _UNITS[unit_idx].wy = _CITIES[dst_city_idx].wy;
                    _UNITS[unit_idx].moves2 -= 2;
                    if(_UNITS[unit_idx].moves2 < 1)
                    {
                        _UNITS[unit_idx].Finished = ST_TRUE;
                    }
                }
            }

            player_idx = _CITIES[src_city_idx].owner_idx;
            // TODO  Radius = IDK_City_Radius_s34255(player_idx, wx, wy, wp);
            // AI_ContactWizards((player_idx, wx, wy, wp, Radius);
            // TODO TILE_ExploreRadius__WIP(wx, wy, wp, Radius);
            TILE_ExploreRadius__WIP(wx, wy, wp, 2);
            _active_world_x = _CITIES[dst_city_idx].wx;
            _active_world_y = _CITIES[dst_city_idx].wy;
            WIZ_NextIdleStack(player_idx, &_map_x, &_map_y, &wp);
            did_earthgate = ST_TRUE;
            Update_Scouted_And_Contacted__WIP();
        }
    }


Failure:
    did_earthgate = ST_FALSE;


Done:

    return did_earthgate;
}


// WZD o62p10
/*

XREF:
    ArmyList_Screen()
    City_Screen__WIP()
    Item_Screen()
    Main_Screen()
    Outpost_Screen()

USW_FullDisplay()
    |-> Load_Battle_Unit()
    |-> USW_LoadAndShow()
    
*/
void USW_FullDisplay(int16_t unit_idx, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    Set_Mouse_List(1, mouse_list_default);
    Load_Battle_Unit(unit_idx, global_battle_unit);
    USW_LoadAndShow(31, 6, x1, y1, x2, y2, 1, unit_idx);
}



/*
    WIZARDS.EXE  ovr063
*/

// WZD o063p01
// MoO2: ~Print_Main_Screen_Data_()
void Main_Screen_Draw_Status_Window(void)
{

    Set_Outline_Color(0);

    if(_players[player_idx].gold_reserve > 9999 || _players[player_idx].mana_reserve > 9999)
    {
        Set_Font_Style_Shadow_Down(0, 0, 0, 0);  // ¿ narrow ?
    }
    else
    {
        Set_Font_Style_Shadow_Down(1, 0, 0, 0);  // ¿ default / regular / normal ?
    }

    Set_Alias_Color(ST_GRAY);

    Print_Integer_Right(265, 68, _players[_human_player_idx].gold_reserve);
    Print_Integer_Right(303, 68, _players[_human_player_idx].mana_reserve);

    Set_Font_Style_Shadow_Down(0, 0, 0, 0);

    Print(268, 68, cstr_GP);
    Print(306, 68, cstr_MP);

    Set_Outline_Color(0);

}

// WZD o063p02
void Main_Screen_Draw_Do_Draw(int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t x_pos, int16_t y_pos, int16_t player_idx)
{
    assert(*map_x >= WORLD_X_MIN && *map_x <= WORLD_X_MAX);  /*  0 & 59 */
    assert(*map_y >= WORLD_Y_MIN && *map_y <= WORLD_Y_MAX);  /*  0 & 39 */

    Reset_Map_Draw();

    Reduced_Map_Set_Dims(REDUCED_MAP_W, REDUCED_MAP_H);

    assert(*map_x >= WORLD_X_MIN && *map_x <= WORLD_X_MAX);  /*  0 & 59 */
    assert(*map_y >= WORLD_Y_MIN && *map_y <= WORLD_Y_MAX);  /*  0 & 39 */

    Draw_Maps(MAP_SCREEN_X, MAP_SCREEN_Y, MAP_WIDTH, MAP_HEIGHT, map_x, map_y, map_plane, x_pos, y_pos, player_idx);

    FLIC_Draw(0, 0, main_background_seg);

    Draw_World_Window(REDUCED_MAP_X, REDUCED_MAP_Y, REDUCED_MAP_W, REDUCED_MAP_H);

    // TODO  _help_entries.Entry_Index+6Eh = ST_UNDEFINED;
    // TODO  _help_entries.Entry_Index+96h = ST_UNDEFINED;
    // TODO  _help_entries.Entry_Index+0A0h = ST_UNDEFINED;
    // TODO  _help_entries.Entry_Index+0F0h = ST_UNDEFINED;

    if(_unit_stack_count > 0)
    {
        Main_Screen_Draw_Unit_Window(_unit_window_start_x, _unit_window_start_y);
        // TODO(JimBalcomb,20230616): make note of the drawing of locked buttons in here  ¿ program-flow oddities ?  ¿ screen component drawing flowchart ?
        Main_Screen_Draw_Unit_Action_Buttons();
    }
    else
    {
        Main_Screen_Draw_Summary_Window();
        Main_Screen_Draw_Unit_Action_Locked_Buttons();
        Main_Screen_Draw_Next_Turn_Button();
    }

    Main_Screen_Draw_Status_Window();

    if(_active_stack_has_path == ST_TRUE)
    {
        Draw_Active_Stack_Movement_Path(_active_stack_path_length, &movepath_x_array[2], &movepath_y_array[2]);
    }

    Main_Screen_Draw_Game_Buttons();
   
    Main_Screen_Draw_Movement_Bar();

    if(DBG_debug_flag)
    {
        Main_Screen_Draw_Debug_Information();
    }

}


// WZD o063p03
/*

¿¿¿
MoO2
_draw_ship_bitmap
_ship_bitmap
_combat_bitmap
???

SEE ALSO:  CMB_Draw_Unit_Enchantment_Outline()
*/
void Draw_Unit_Enchantment_Outline(int16_t unit_idx)
{
    int8_t enchantment_magic_realm = 0;
    uint32_t unit_enchantments = 0;
    uint8_t * color_list = 0;

    unit_enchantments = _UNITS[unit_idx].enchantments;

    enchantment_magic_realm = ST_UNDEFINED;

    if(Unit_Has_Invisibility(unit_idx) == ST_TRUE)
    {
        unit_enchantments = 0;
    }

    if(unit_enchantments != 0)
    {

        if((unit_enchantments & UE_REGENERATION) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_RESISTMAGIC) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_RIGHTEOUSNESS) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_PLANARTRAVEL) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_BLACKCHANNELS) != 0)
        {
            enchantment_magic_realm = mr_Death;
        }
        else if((unit_enchantments & UE_LIONHEART) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_IRONSKIN) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_MAGICIMMUNITY) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_WINDWALKING) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_ELEMENTALARMOR) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_TRUESIGHT) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_INVULNERABILITY) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_PATHFINDING) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_FLIGHT) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_ELDRITCHWEAPON) != 0)
        {
            enchantment_magic_realm = mr_Chaos;
        }
        else if((unit_enchantments & UE_WRAITHFORM) != 0)
        {
            enchantment_magic_realm = mr_Death;
        }
        else if((unit_enchantments & UE_GIANTSTRENGTH) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_IMMOLATION) != 0)
        {
            enchantment_magic_realm = mr_Chaos;
        }
        else if((unit_enchantments & UE_GUARDIANWIND) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_SPELLLOCK) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_HEROISM) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_BERSERK) != 0)
        {
            enchantment_magic_realm = mr_Death;
        }
        else if((unit_enchantments & UE_HOLYARMOR) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_WATERWALKING) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_ENDURANCE) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_CLOAKOFFEAR) != 0)
        {
            enchantment_magic_realm = mr_Death;
        }
        else if((unit_enchantments & UE_STONESKIN) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_FLAMEBLADE) != 0)
        {
            enchantment_magic_realm = mr_Chaos;
        }
        else if((unit_enchantments & UE_BLESS) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_HOLYWEAPON) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_RESISTELEMENTS) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
    }

    if(enchantment_magic_realm != ST_UNDEFINED)
    {
        Outline_Bitmap_Pixels_No_Glass(UnitDraw_WorkArea, 255);
        Bitmap_Aura_Pixels(UnitDraw_WorkArea, 255, &enchantment_outline_colors[enchantment_magic_realm][unit_enchantment_animation_count][0]);
    }

}




// WZD o063p04
/*

XREF:
    j_Cycle_Unit_Enchantment_Animation()
        City_Screen_Draw_Garrison_Window()
        Enemy_City_Screen_Draw()
        Main_Screen_Draw_Unit_Window()
        Main_Screen_Draw_Next_Turn_Button()
        ArmyList_Screen_Draw()
        Draw_Unit_List_Window_Pup()
        Unit_Figure_Draw()
        CMB_CreateEntities()

*/
void Cycle_Unit_Enchantment_Animation(void)
{
    unit_enchantment_animation_flag += 1;

    if(unit_enchantment_animation_flag > 1)
    {
        unit_enchantment_animation_flag = 0;
    }

    if(unit_enchantment_animation_flag < 1)
    {
        unit_enchantment_animation_count += 1;

        if(unit_enchantment_animation_count > 7)
        {
            unit_enchantment_animation_count = 0;
        }
    }
}

// WZD o063p05
void Draw_Unit_Picture(int16_t x, int16_t y, int16_t unit_idx, int16_t flag)
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
        unit_owner_idx = _UNITS[unit_idx].owner_idx;
        if(unit_owner_idx != ST_UNDEFINED)
        {
            if(unit_owner_idx != NEUTRAL_PLAYER_IDX)
            {
                unit_colored_backgrounds_idx = _players[unit_owner_idx].banner_id;
            }
            else
            {
                unit_colored_backgrounds_idx = 5;
                _players[unit_owner_idx].banner_id = 5;
            }
        }

        FLIC_Draw(x, y, _unit_colored_backgrounds_seg[unit_colored_backgrounds_idx]);

    }

    Draw_Unit_StatFig(x, y, unit_idx, flag);

}


// WZD o063p05
// AKA OVL_DrawUnitImage()
// pict_seg is 
// MoO2  Draw_Ship_Icon()
/*

Outpost_Screen_Draw()
    Draw_Unit_StatFig(figure_x1, figure_y1, CTY_Garrison_Units[itr], 1);
*/
void Draw_Unit_StatFig(int16_t x, int16_t y, int16_t unit_idx, int16_t flag)
{
    uint8_t unit_type_idx = 0;
    int16_t unit_owner_idx = 0;
    int16_t banner_idx = 0;
    uint16_t itr_color_remap = 0;
    // uint8_t Color_1;
    // uint8_t Color_2;
    uint8_t color_array[2] = { 0, 0 };
    uint8_t * vbb_ptr = 0;
    int16_t itr_width = 0;
    int16_t itr_height = 0;

    unit_type_idx = _UNITS[unit_idx].type;

    FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 0);

    // TODO  if(flag == 0)
    // TODO  {

    FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 1);
    Draw_Picture_To_Bitmap(_unit_type_table[unit_type_idx].pict_seg, UnitDraw_WorkArea);

    unit_owner_idx = (int16_t)_UNITS[unit_idx].owner_idx;

    if(unit_owner_idx == NEUTRAL_PLAYER_IDX)
    {
        banner_idx = NEUTRAL_PLAYER_BANNER_COLOR_IDX;
    }
    else
    {
        banner_idx = (int16_t)_players[unit_owner_idx].banner_id;
    }

    // TODO: add contstants for the color index and color count
    for(itr_color_remap = 0; itr_color_remap < 5; itr_color_remap++)
    {
        Replace_Color(UnitDraw_WorkArea, 214 + itr_color_remap, COL_Banners[ ((banner_idx * 5) + itr_color_remap) ]);
    }

    // TODO  } /* if(flag == 0) */

    /*
        flag: {0,1,2,3}
            draw Unit Enchantment Outline
        Invisible:
            ?
        ? Inactive / Finished / Patrol ?
            ?
    */
    if(flag == 0 || flag == 1 || flag == 2 || flag == 3)
    {

        if(Unit_Has_Invisibility(unit_idx) == ST_TRUE)
        {
            Replace_Color_All(UnitDraw_WorkArea, 233); // sets every non-transparent pixel; 233 means use remap colors - (233-232=1) remap color block 1
            Outline_Bitmap_Pixels(UnitDraw_WorkArea, ST_BLACK);
        }

        if((flag != 3) && 
            (_UNITS[unit_idx].Status == us_Patrol) && 
            (Unit_Has_Invisibility(unit_idx) == ST_FALSE)  && 
            (unit_owner_idx == _human_player_idx)
        )
        {
            Gray_Scale_Bitmap(UnitDraw_WorkArea, 1);  // TODO  ¿ 1 means ... ?
        }
    
        Draw_Unit_Enchantment_Outline(unit_idx);
    }

    FLIC_Set_LoopFrame_1(UnitDraw_WorkArea);  // sets for/enables remap colors
    Draw_Picture(x+1, y+1, UnitDraw_WorkArea);

    /*
        BEGIN: Unit Status - Icon/Text
    */
    if((unit_owner_idx == _human_player_idx) && ( (flag == 0) || (flag == 1) ) )
    {
        // Color_1 = 10;
        // Color_2 = 15;
        // Set_Font_Colors_15(0, &Color_1);
        color_array[0] = 10;  // ¿ palette index 10 0x0A  [0A] 2B 29 28 ?
        color_array[1] = 15;  // ¿ palette index 15 0x0F  [0F] 3F 3F 3F ?
        Set_Font_Colors_15(0, &color_array[0]); // set font style num 0; set font colors block 15
        Set_Outline_Color(4);
        Set_Font_Style_Shadow_Down(0, 15, 0, 0); // set font style num 0; set normal,highlight,special colors - blocks 15,0,0; set shadow bottom right

        // switch( (_UNITS[unit_idx].Status - 2) )
        switch(_UNITS[unit_idx].Status)
        {
        case us_BuildRoad:  /* ¿ us_BuildRoad == 2 ? */
            {
                Print(x+2, y+2, "B");
            } break;
            case us_GOTO:  /* ¿ us_GOTO == 3 ? */
            {
                if(_UNITS[unit_idx].Rd_Constr_Left == ST_UNDEFINED)
                {
                    Print(x+2, y+2, "G");
                }
                else
                {
                    Print(x+2, y+2, "B");
                }
            } break;
            case us_Casting:
            {
                Print(x+4, y+5, "C");  // 'MoM Demo' "Casting"
            } break;
            case us_Purify:
            {
                Print(x+2, y+2, "P");
            } break;
        }
    }
    /*
        END: Unit Status - Icon/Text
    */

}


// WZD o063p06
void Unit_Window_Draw_Unit_Attributes(int16_t x, int16_t y, int16_t unit_idx)
{
    int8_t Weapon_Quality;
    int16_t Relative_Zero_Level;
    int16_t Icons_Drawn;
    int16_t bar_length;
    int16_t current_hits;
    int16_t max_hits;
    int16_t Wpn_Top;
    int16_t Wpn_Left;
    int16_t Exp_Top;
    int16_t Exp_Left;
    int16_t Level_Index;
    uint8_t Unit_Type;
    uint8_t Color;
    SAMB_ptr experience_level_icon_seg;
    // SAMB_ptr weapon_type_icon_seg;
    SAMB_ptr weapon_type_icon_seg = NULL;  // Error	C4703	potentially uninitialized local pointer variable 'weapon_type_icon_seg' used

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Window_Draw_Unit_Attributes(x = %d, y = %d, unit_idx = %d)\n", __FILE__, __LINE__, x, y, unit_idx);
#endif

    Unit_Type = _UNITS[unit_idx].type;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Unit_Type: %d\n", __FILE__, __LINE__, Unit_Type);
#endif

    /*
        BEGIN: Draw Unit Damaged Bar
    */
    // MoO2  Module: AIPOWER  Current_Ship_Hits_  Address: 01:0005F2C3
    // MoO2  Module: AIPOWER  Max_Ship_Hits_      Address: 01:0005F2F6
    // MoO2  Module: CMBTAI   Get_Total_Hits_     Address: 01:0002BB06
    max_hits = Unit_Hit_Points(unit_idx) * _unit_type_table[Unit_Type].Figures;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: max_hits: %d\n", __FILE__, __LINE__, max_hits);
#endif
    current_hits = max_hits - _UNITS[unit_idx].Damage;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _UNITS[unit_idx].Damage: %d\n", __FILE__, __LINE__, _UNITS[unit_idx].Damage);
    dbg_prn("DEBUG: [%s, %d]: current_hits: %d\n", __FILE__, __LINE__, current_hits);
#endif
    // cmp  current_hits, max_hits; jz ... So, != OR just <
    if((current_hits != max_hits) && (current_hits > 0) )
    {
        // NOTE: `CWD; IDIV;` is 'divide', 'not 'modulus'
        bar_length = (((current_hits * 10) / max_hits) - 1);  // `- 1`, because Line() will draw one pixel, then this length
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: bar_length: %d\n", __FILE__, __LINE__, bar_length);
#endif
        if(bar_length >= 6)
        {
            DLOG("(bar_length >= 6)");
            Color = DAMAGE_BAR_GREEN;  /* Damage Bar Green */
        }
        else if(bar_length >= 3)
        {
            DLOG("(bar_length >= 3)");
            Color = DAMAGE_BAR_YELLOW;  /* Damage Bar Yellow */
        }
        else
        {
            DLOG("(bar_length < 3)");
            Color = DAMAGE_BAR_RED;  /* Damage Bar Red */
        }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Color: %d\n", __FILE__, __LINE__, Color);
#endif
        Line(x + 5, y + 19, x + bar_length + 5, y + 19, Color);
    }
    /*
        END: Draw Unit Damaged Bar
    */

    /*
        BEGIN: Experience Level - "Circle"/"Ring"/"Medal"/"Icon"
    */
    if(_UNITS[unit_idx].Level > 0)
    {
        if(_UNITS[unit_idx].Level > 6)  /* ¿ Only Hero - Super Hero - Ring Symbol - {1 Red, 2 Red} */
        {
            experience_level_icon_seg = main_red_medal_icon;
            Relative_Zero_Level = 6;
        }
        else if(_UNITS[unit_idx].Level > 6)
        {
            experience_level_icon_seg = main_gold_medal_icon;
            Relative_Zero_Level = 3;
        }
        else
        {
            experience_level_icon_seg = main_white_medal_icon;
            Relative_Zero_Level = 0;
        }
        Icons_Drawn = 0;
        Level_Index = Relative_Zero_Level;
        while(_UNITS[unit_idx].Level > Level_Index)
        {
            Exp_Left = 2 + x + (Icons_Drawn * 4);
            Exp_Top = y + 21;
            FLIC_Draw(Exp_Left, Exp_Top, experience_level_icon_seg);
            Icons_Drawn++;
            Level_Index++;
        }
    }
    /*
        END: Experience Level - "Circle"/"Ring"/"Medal"/"Icon"
    */

    /*
        BEGIN: Weapon - "magic", "mithril", "adamantium"
    */
        Weapon_Quality = (_UNITS[unit_idx].mutations & 0x03);  /* mask-off bits besides weapon quality;  bit set - 1st and/or 2nd  OR  2^2 values {0,1,2,3} */
        if(Weapon_Quality > 0)
        {
            Wpn_Left = x + 13;
            Wpn_Top = y + 19;

            switch(Weapon_Quality)
            {
                case 1:
                {
                    /* WQ_Magic */
                    weapon_type_icon_seg = main_magic_weapon_icon;
                } break;
                case 2:
                {
                    /* WQ_Mithril */
                    weapon_type_icon_seg = main_mithril_weapon_icon;
                } break;
                case 3:
                {
                    /* WQ_Adamantium */
                    weapon_type_icon_seg = main_adamantium_weapon_icon;
                } break;
            }
            FLIC_Set_CurrentFrame(weapon_type_icon_seg, unit_weapon_type_animation_count);
            FLIC_Draw(Wpn_Left, Wpn_Top, weapon_type_icon_seg);
        }
    /*
        END: Weapon - "Circle"/"Ring"/"Medal"/"Icon"
    */

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Unit_Window_Draw_Unit_Attributes(x = %d, y = %d, unit_idx = %d)\n", __FILE__, __LINE__, x, y, unit_idx);
#endif

}

// WZD o63p07
// drake178: OVL_DrawStackMoves
void Main_Screen_Draw_Movement_Bar(void)
{
    uint16_t movement_points;

    // _help_entries+0AAh = ST_UNDEFINED

    if(_unit_stack_count > 0)
    {
        movement_points = Stack_Moves_Active();
        // movement_points = 17;
        if(movement_points != 0)
        {
            // _help_entries+0AAh = HLP_MOVES
            Set_Font_Style_Shadow_Down(1,1,0,0);
            Set_Outline_Color(0);
            Set_Alias_Color(0);
            Print(246, 167, cnst_Moves);
            Print_Moves_String(275, 167, movement_points, ST_FALSE);

            Set_Font_Style(1,1,0,0);
            Set_Outline_Color(0);
            Set_Alias_Color(0);
            Print(246, 168, cnst_Moves);
            Print_Moves_String(275, 168, movement_points, ST_FALSE);

            Set_Font_Style(1,0,0,0);
            Set_Alias_Color(0);
            Print(246, 167, cnst_Moves);
            Print_Moves_String(275, 167, movement_points, ST_FALSE);

            Draw_Movement_Mode_Icons(286, 167, ST_UNDEFINED);
        }
    }

}


// WZD o63p08
// drake178: OVL_DrawMoveTypes()
/*
    if called with unit_idx = -1, _unit_stack_count MUST BE >= 1
    Main_Screen_Draw_Movement_Bar() calls with unit_idx = ST_UNDEFINED
*/
void Draw_Movement_Mode_Icons(int16_t x, int16_t y, int16_t unit_idx)
{
    int16_t movement_icons[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t movement_mode_flags[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t stack = 0;
    int16_t x_start = 0;
    int itr = 0;
    int movement_mode_icon_count = 0;

    if(_unit_stack_count >= 1 || unit_idx != ST_UNDEFINED)
    {

        for(itr = 0; itr < 9; itr++)  /* TODO: add define for 9 - (Special) Movemenet Mode Count/Max */
        {
            movement_mode_flags[itr] = ST_FALSE;
        }

        if(unit_idx == ST_UNDEFINED)
        {
            Active_Stack_Movement_Modes(&movement_mode_flags[0]);
        }
        else
        {
            stack = unit_idx;
            Army_Movement_Modes(&movement_mode_flags[0], &stack, 1);
        }

    }

    // ¿ drake178: BUG: this won't work for the single unit version ?
    if(Stack_Has_PlanarTravel() == ST_TRUE)
    {
        // movement_mode_flags.mm_PlanarTravel = ST_TRUE;
        movement_mode_flags[OFS_PLANARTRAVEL] = ST_TRUE;
    }


    movement_mode_icon_count = 0;
    // if(movement_mode_flags.mm_PlanarTravel == ST_TRUE)
    if(movement_mode_flags[OFS_PLANARTRAVEL] == ST_TRUE)
    {
        movement_icons[movement_mode_icon_count] = 6;  /* M_PShift_Icon */
        movement_mode_icon_count++;
    }

    // if(movement_mode_flags.mm_Flying == ST_TRUE)
    if(movement_mode_flags[OFS_FLYING] == ST_TRUE)
    {
        movement_icons[movement_mode_icon_count] = 4;  /* M_Fly_Icon */
        movement_mode_icon_count++;
    }
    else if(movement_mode_flags[OFS_SAILING] == ST_TRUE)  // if(movement_mode_flags.mm_Sailing == ST_TRUE)
    {
        movement_icons[movement_mode_icon_count] = 0;  /* M_Sail_Icon */
        movement_mode_icon_count++;
    }
    else
    {
        // if( (movement_mode_flags.mm_Forester == ST_TRUE) && (movement_mode_flags.mm_Mountaineer == ST_TRUE) )
        if( (movement_mode_flags[OFS_FORESTER] == ST_TRUE) && (movement_mode_flags[OFS_MOUNTAINEER] == ST_TRUE) )
        {
            movement_icons[movement_mode_icon_count] = 5;  /* M_Pathf_Icon */
            movement_mode_icon_count++;
        }
        else
        {
            // if(movement_mode_flags.mm_Forester == ST_TRUE)
            if(movement_mode_flags[OFS_FORESTER] == ST_TRUE)
            {
                movement_icons[movement_mode_icon_count] = 3;  /* M_Forest_Icon */
                movement_mode_icon_count++;

            }
            else
            {
                // if(movement_mode_flags.mm_Mountaineer == ST_TRUE)
                if(movement_mode_flags[OFS_MOUNTAINEER] == ST_TRUE)
                {
                    movement_icons[movement_mode_icon_count] = 2;  /* M_Mntn_Icon */
                    movement_mode_icon_count++;

                }
            }
        }

        // if(movement_mode_flags.mm_Swimming == ST_TRUE)
        if(movement_mode_flags[OFS_SWIMMING] == ST_TRUE)
        {
            movement_icons[movement_mode_icon_count] = 1;  /* M_Swim_Icon */
            movement_mode_icon_count++;
        }
    }

    if(movement_mode_icon_count == 0)
    {
        movement_icons[movement_mode_icon_count] = 8;  /* M_Grnd_Icon */
        movement_mode_icon_count++;
    }


    x += ((3 - movement_mode_icon_count) * 10);

    for(itr = 0; itr < movement_mode_icon_count; itr++)
    {
        x_start = x + (itr * 10);
        FLIC_Draw(x_start, y, movement_mode_icons[movement_icons[itr]]);
    }

}


// WZD o63p09
/*
    checks if Active Stack is on City with Astral Gate
    or if all Units have Planar Travel, by Enchantment, Ability, or Item
    returns ST_TRUE or ST_FALSE
*/
int16_t Stack_Has_PlanarTravel(void)
{
    int16_t itr;
    int16_t stack_has_no_active_units;
    int16_t has_planartravel;
    int16_t stack_on_astral_gate;
    int16_t unit_idx;

    stack_has_no_active_units = ST_TRUE;

    for(itr = 0; (itr < _unit_stack_count) && (stack_has_no_active_units == ST_TRUE); itr++)
    {
        if(_unit_stack[itr].active != ST_FALSE)
        {
            stack_has_no_active_units = ST_FALSE;
        }
    }

    if(stack_has_no_active_units == ST_TRUE)
    {
        for(itr = 0; itr < _unit_stack_count; itr++)
        {
            _unit_stack[itr].active = ST_TRUE;
        }
    }

    has_planartravel = ST_FALSE;

    stack_on_astral_gate = Active_Stack_On_Astral_Gate();

    // Unused_Local = 0

    for(itr = 0; itr < _unit_stack_count; itr++)
    {
        unit_idx = _unit_stack[itr].unit_idx;
        if(_unit_stack[itr].active == ST_TRUE)
        {
            if(
                ((_UNITS[unit_idx].enchantments & UE_PLANARTRAVEL) != 0)
                ||
                ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_PLANARTRAVEL) != 0)
                ||
                (Unit_Has_PlanarTravel_Item(unit_idx) == ST_TRUE)
            )
            {
                has_planartravel = ST_TRUE;
            }
            else
            {
                has_planartravel = ST_FALSE;
            }
        }
    }

    if(stack_has_no_active_units == ST_TRUE)
    {
        for(itr = 0; itr < _unit_stack_count; itr++)
        {
            _unit_stack[itr].active = ST_FALSE;
        }
    }

    return has_planartravel;
}


// WZD o63p11
/*
OON XREF: Stack_Has_PlanarTravel()
*/
int16_t Active_Stack_On_Astral_Gate(void)
{
    int16_t astral_gate;
    int16_t city_idx;  // _DI_
    int16_t unit_idx;  // _SI_

    astral_gate = ST_FALSE;

    unit_idx = _unit_stack[0].unit_idx;

    city_idx = City_At_Square__2(UNITX(), UNITY(), 0);

    if(city_idx == ST_UNDEFINED)
    {
        city_idx = City_At_Square__2(UNITX(), UNITY(), 1);
    }

    if(
        (city_idx != ST_UNDEFINED)
        &&
        (_CITIES[city_idx].enchantments[ASTRAL_GATE] == ST_TRUE)
    )
    {
        astral_gate = ST_TRUE;
    }

    return astral_gate;
}


// WZD o63p12
// OVL_GetStackHMoves
// OVL_GetStackHMoves_1
/*
    movement points to be used for 'move stack'
    ¿ just here to avoid loading the overlay for 'Move Stack' ?
        ...which would mean being in ovr063 is meaingful?
        ...and ovr097 is differently meaningful?
            ...and, cause called by ovr095?
    
*/
int16_t Stack_Moves_Active(void)
{
/*
XREFs
    j_Stack_Moves_Active()
    Stack_Moves_Active()

j_Stack_Moves_Active()
    Main_Screen()
    Move_Stack_DirKey
    Move_Stack


    loops through current stack
        checks for active
        checks if the unit type is/has/does transport/carries units
            breaks and assigns units current moves2
        checks for wind walking - ability or enchantment
            does not break, but assigns units current moves2
    Scenarios/Use-Cases:
        stack does not have a 'carries units' and does not have a 'has wind walking'
        stack does have a 'carries units' but does not have a 'has wind walking'
        stack does not have a 'carries units' but does have a 'has wind walking'
        
        if stack does have a 'carries units', that is that and its are the movement points
        ...
        ? nay 'carries units', nay 'has wind walking' ?
            returns lowest moves2 of 'active' units

NOTE: you can have multiple transport units in a stack

almost the same code as in WZD ovr097 Units_Moves()
Units_Moves() breaks after Unit_Has_WindWalking(); does not have stack_has_windwalker or windwalker_unit_idx
OON XREF STK_Move() WZD o95p01

*/
    int16_t stack_has_windwalker;
    int16_t windwalker_unit_idx;
    int16_t movement_points; // _SI_
    int16_t tmp_unit_idx;  // _DI_
    int16_t itr_stack;
    int8_t tmp_unit_type;  // DNE in Dasm

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Stack_Moves_Active()\n", __FILE__, __LINE__);
#endif

    movement_points = 1000;

    stack_has_windwalker = ST_FALSE;

    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        tmp_unit_idx = _unit_stack[itr_stack].unit_idx;
        if(_unit_stack[itr_stack].active == ST_TRUE)
        {
            if(_UNITS[tmp_unit_idx].moves2 < movement_points)
            {
                movement_points = _UNITS[tmp_unit_idx].moves2;
            }

            tmp_unit_type = _UNITS[tmp_unit_idx].type;
            if(_unit_type_table[tmp_unit_type].Transport > 0)
            {
                movement_points = _UNITS[tmp_unit_idx].moves2;
                break;
            }

            if(Unit_Has_WindWalking(tmp_unit_idx) == ST_TRUE)
            {
                stack_has_windwalker = ST_TRUE;
                windwalker_unit_idx = tmp_unit_idx;
                movement_points = _UNITS[tmp_unit_idx].moves2;
            }
        }
    }

    if(movement_points == 1000)
    {
        movement_points = 0;
    }

    stack_moves = movement_points;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Stack_Moves_Active() { stack_moves = %d }\n", __FILE__, __LINE__, stack_moves);
#endif

    return stack_moves;
}


// WZD o63p13
// AKA OVL_GetStackHMoves_2()
/*
    does not test for 'active'
        ... vs. WZD o63p12 Stack_Moves_Active()
*/
int16_t Stack_Moves(void)
{
    int16_t windwalker_unit_idx;
    int16_t stack_has_windwalker;
    int16_t movement_points;

    int16_t itr_stack;
    int16_t unit_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Stack_Moves()\n", __FILE__, __LINE__);
#endif

    movement_points = 1000;

    stack_has_windwalker = ST_FALSE;


    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        unit_idx = _unit_stack[itr_stack].unit_idx;
        if(_UNITS[unit_idx].moves2 < movement_points)
        {
            movement_points = _UNITS[unit_idx].moves2;
        }
        if(_unit_type_table[_UNITS[unit_idx].type].Transport > 0)
        {
            movement_points = _UNITS[unit_idx].moves2;
            break;
        }
        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
        {
            stack_has_windwalker = ST_TRUE;
            movement_points = _UNITS[unit_idx].moves2;
            windwalker_unit_idx = unit_idx;
        }
    }

    if(stack_has_windwalker == ST_TRUE)
    {
        if(_UNITS[windwalker_unit_idx].moves2 > 0)
        {
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                unit_idx = _unit_stack[itr_stack].unit_idx;

                if( (_UNITS[windwalker_unit_idx].moves2 < 0) && (Unit_Has_WindWalking(unit_idx) != ST_TRUE) )
                {
                    _UNITS[unit_idx].moves2 = 1;
                    _UNITS[unit_idx].Finished = ST_FALSE;
                }
            }
        }
    }

    if(movement_points == 1000)
    {
        movement_points = 0;
    }

    stack_moves = movement_points;

    return stack_moves;
}


// WZD o63p14
// drake178: RP_TILE_HasCity2
// functionally identical to TILE_HasCity, would be byte identical if it wasn't for the reversed order of checking the three parameters
int16_t City_At_Square__2(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t has_city;  // _SI_
    int16_t itr;  // _CX_

    has_city = ST_UNDEFINED;

    for(itr = 0; ((itr < _cities) && (has_city == ST_UNDEFINED)); itr++)
    {
        if(
            (_CITIES[itr].wx == wx) &&
            (_CITIES[itr].wy == wy) &&
            (_CITIES[itr].wp == wp)
        )
        {
            has_city = itr;
        }
    }

    return has_city;
}


// WZD o63p15
/*
    returns {F,T} - any player has 'Planar Seal'
*/
int16_t Check_Planar_Seal(void)
{
    int16_t itr_players;
    int16_t planar_seal;

    for(itr_players = 0, planar_seal = ST_FALSE; ((itr_players < _num_players) && (planar_seal == ST_FALSE)); itr_players++)
    {
        if(_players[itr_players].Globals[PLANAR_SEAL] == ST_TRUE)
        {
            planar_seal = ST_TRUE;
        }
    }

    return planar_seal;
}



/* 
    WIZARDS.EXE  ovr064
*/

// WZD o64p01
// void Allocate_Reduced_Map__1(void)
// WZD o64p02
// PRIVATE  void Allocate_Reduced_Map__2(void);
void Allocate_Reduced_Map(void)
{
    _reduced_map_seg = Allocate_First_Block(_screen_seg, 303);  // 303 * 16 = 4848 bytes
}


// WZD o64p03
// ¿ subset of Draw_Maps() ?
// AKA OVL_PrepMinimap()
// AKA MainScr_Prepare_Reduced_Map()
/*

*/
void MainScr_Create_Reduced_Map_Picture(void)
{
    int16_t reduced_map_window_wx = 0;
    int16_t reduced_map_window_wy = 0;
    int16_t reduced_map_width = 0;  // _SI_
    int16_t reduced_map_height = 0;  // _DI_

    reduced_map_width = REDUCED_MAP_W;
    reduced_map_height = REDUCED_MAP_H;

    Reduced_Map_Set_Dims(reduced_map_width, reduced_map_height);

    Reduced_Map_Coords(&reduced_map_window_wx, &reduced_map_window_wy, ((_map_x + (MAP_WIDTH / 2)) % WORLD_WIDTH), (_map_y + (MAP_HEIGHT / 2)), reduced_map_width, reduced_map_height);

    Create_Reduced_Map_Picture(reduced_map_window_wx, reduced_map_window_wy, _map_plane, _reduced_map_seg, reduced_map_width, reduced_map_height, 0, 0, 0);
}

// WZD o064p04
void Main_Screen_Draw_Summary_Window(void)
{
    int16_t gold = 0;
    int16_t food = 0;
    int16_t mana = 0;
    uint8_t colors[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t active_event_id = 0;
    uint8_t color_index = 0;

    gold = 0;  // DNE in Dasm
    food = 0;  // DNE in Dasm
    mana = 0;  // DNE in Dasm



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



    Player_Resource_Income_Total(_human_player_idx, &gold, &food, &mana);


    colors[0] = 198;
    colors[1] = 198;
    colors[2] = 198;
    Set_Font_Colors_15(0, colors);
    // ¿ this only impacts gold < 0 and cycle_incomes != ST_UNDEFINED ?

    Set_Outline_Color(ST_BLACK);



    if(gold >= 0)
    {
        Set_Font_Style_Outline(0, 4, 0, 0);
        Set_Alias_Color(184);  // 184d  B8h  ~"dark gold"
    }
    else
    {
        if(cycle_incomes == ST_UNDEFINED)
        {
            Set_Font_Style_Outline(0, 2, 0, 0);
        }
        else
        {
            Set_Font_Style_Outline(0, 15, 0, 0);
        }
    }

    Print_Integer_Right(273, 101, gold);

    Print(278, 101, cstr_Gold);


    if(food >= 0)
    {
        Set_Font_Style_Outline(0, 4, 0, 0);
        Set_Alias_Color(184);
    }
    else
    {
        if(cycle_incomes == ST_UNDEFINED)
        {
            Set_Font_Style_Outline(0, 2, 0, 0);
        }
        else
        {
            Set_Font_Style_Outline(0, 15, 0, 0);
        }
    }
    Print_Integer_Right(273, 133, food);
    Print(278, 133, cstr_Food);


    if(mana >= 0)
    {
        Set_Font_Style_Outline(0, 4, 0, 0);
        Set_Alias_Color(184);
    }
    else
    {
        if(cycle_incomes == ST_UNDEFINED)
        {
            Set_Font_Style_Outline(0, 2, 0, 0);
        }
        else
        {
            Set_Font_Style_Outline(0, 15, 0, 0);
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
    if(events_table->Conjunction_Sorcery_Status == 2)
    {
        colors[0] = 0;
        colors[1] = 172;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style_Outline(0, 15, 0, 0);
        Print_Centered(280, 157, cstr_Conjunction);
        active_event_id = 0;
    }

    if(events_table->Conjunction_Chaos_Status == 2)
    {
        colors[0] = 0;
        colors[1] = 201;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style_Outline(0, 15, 0, 0);
        Print_Centered(280, 158, cstr_Conjunction);
        active_event_id = 1;
    }

    if(events_table->Conjunction_Nature_Status == 2)
    {
        colors[0] = 0;
        colors[1] = 217;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style_Outline(0, 15, 0, 0);
        Print_Centered(280, 158, cstr_Conjunction);
        active_event_id = 2;
    }
    /*
        END: Conjunction - Sorcery, Chaos, Nature
    */

    if(events_table->Bad_Moon_Status == 2)
    {
        colors[0] = 0;
        colors[1] = 9;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style_Outline(0, 15, 0, 0);
        Print_Centered(280, 158, cstr_Bad_Moon);
        active_event_id = 3;
    }

    if(events_table->Good_Moon_Status == 2)
    {
        colors[0] = 0;
        colors[1] = 15;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style_Outline(0, 15, 0, 0);
        Print_Centered(280, 158, cstr_Good_Moon);
        active_event_id = 4;
    }

    if(events_table->Mana_Short_Status == 2)
    {
        colors[0] = 0;
        colors[1] = 123;
        Set_Font_Colors_15(0, colors);
        Set_Outline_Color(0);
        Set_Font_Style_Outline(0, 15, 0, 0);
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


    // cycles reds for negative values
    if(cycle_incomes != ST_UNDEFINED)
    {
        Cycle_Palette_Color__STUB(198, 40, 0, 0, 63, 0, 0, 3);
    }


// ? Error if alone or unsued/unreferenced ? Done:

}


// WZD o064p05
void Main_Screen_Draw_Next_Turn_Button(void)
{

    // _help_entries.Entry_Index+6Eh = ST_UNDEFINED;

    if(all_units_moved == ST_TRUE)
    {
        // _help_entries.Entry_Index+6Eh = HLP_NEXT_TURN

        FLIC_Draw(240, 173, next_turn_background_seg);

        Set_Draw_Active_Stack_Never();

        if(current_screen == ST_UNDEFINED)
        {
            FLIC_Draw(246, 178, next_turn_button_seg);
        }
        else
        {
            Cycle_Unit_Enchantment_Animation();
        }
    }

}


// WZD o64p06
void OVL_DisableIncmBlink(void)
{
    cycle_incomes = -1;
}


// WZD o64p07
void OVL_EnableIncmBlink(void)
{
    cycle_incomes = 0;
}


// WZD o64p08
void Main_Screen_Draw_Unit_Action_Locked_Buttons(void)
{

    FLIC_Set_CurrentFrame(main_lock_done_button, 0);
    FLIC_Draw(246, 176, main_lock_done_button);

    FLIC_Set_CurrentFrame(main_lock_patrol_button, 0);
    FLIC_Draw(280, 176, main_lock_patrol_button);

    FLIC_Set_CurrentFrame(main_lock_wait_button, 0);
    FLIC_Draw(246, 186, main_lock_wait_button);

    if(special_action_flag == ST_UNDEFINED)
    {
        FLIC_Set_CurrentFrame(main_lock_build_button, 0);
        FLIC_Draw(280, 186, main_lock_build_button);
    }
    else
    {
        FLIC_Set_CurrentFrame(main_lock_purify_button, 0);
        FLIC_Draw(280, 186, main_lock_purify_button);
    }

}


// WZD o64p09
void Unit_Window_Picture_Coords(int16_t stack_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2)
{

    // TODO(JimBalcomb,20230616): add module-scoped (manifest) contstants for the magic-numbers used here-in
    // 3 for figures width, 3 for figures height
    // ¿ 23 for figures background width, 29 for figures background height ?
    // ¿ 22 for figrues width, 28 for figures height ?
    *x1 = _unit_window_start_x + ((stack_idx % 3) * 23);
    *y1 = _unit_window_start_y + ((stack_idx / 3) * 29);
    *x2 = *x1 + 22;
    *y2 = *y1 + 28;

}

// WZD o64p10
// drake178: UNIT_SetGlobalPath()
void Set_Active_Stack_Movement_Path(int16_t unit_idx)
{
    int16_t boatriders[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t movement_mode_flags[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t movepath_idx = 0;
    int16_t road_src_wy = 0;
    int16_t road_src_wx = 0;
    int16_t owner_idx = 0;
    int16_t troop_count = 0;
    int16_t boatrider_count = 0;
    int16_t unit_goto_wy = 0;
    int16_t unit_goto_wx = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t goto_units_count = 0;
    int16_t l_unit_idx = 0;
    int16_t unit_stack_idx = 0;  // _SI_
    int16_t itr_troops = 0;  // _DI_
    int16_t itr = 0;  // _DI_

    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_goto_wx = _UNITS[unit_idx].dst_wx;
    unit_goto_wy = _UNITS[unit_idx].dst_wy;
    owner_idx = _UNITS[unit_idx].owner_idx;

    troop_count = _unit_stack_count;

    goto_units_count = 0;

    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        troops[itr_troops] = _unit_stack[itr_troops].unit_idx;
        if(_UNITS[troops[itr_troops]].Status == us_GOTO)
        {
            goto_units_count++;
        }
    }

    if(goto_units_count < 1)
    {
        return;
    }

    Army_Movement_Modes(&movement_mode_flags[0], &troops[0], troop_count);

    boatrider_count = Army_Boatriders(troop_count, &troops[0], &boatriders[0]);

    if(_UNITS[unit_idx].Rd_Constr_Left != ST_UNDEFINED)
    {
        road_src_wx = _UNITS[unit_idx].Rd_From_X;
        road_src_wy = _UNITS[unit_idx].Rd_From_Y;

        _active_stack_path_length = Path_Wrap(road_src_wx, road_src_wy, unit_goto_wx, unit_goto_wy, &movepath_x_array[2], &movepath_y_array[2], WORLD_WIDTH);

        movepath_idx = ST_UNDEFINED;

        for(itr = 0; itr < _active_stack_path_length; itr++)
        {
            if(
                (movepath_x_array[(2 + itr)] == unit_wx)
                &&
                (movepath_y_array[(2 + itr)] == unit_wy)
            )
            {
                movepath_idx = itr;
            }
        }

        if(movepath_idx != ST_UNDEFINED)
        {
            _active_stack_path_length -= (movepath_idx - 1);
        }

        for(itr = 0; itr < _active_stack_path_length; itr++)
        {
            movepath_x_array[(2 + itr)] = movepath_x_array[(3 + itr)];
            movepath_y_array[(2 + itr)] = movepath_y_array[(3 + itr)];
        }

    }
    else
    {
        _active_stack_path_length = Make_Move_Path(
            movement_mode_flags[0],
            movement_mode_flags[1],
            movement_mode_flags[2],
            movement_mode_flags[3],
            movement_mode_flags[4],
            movement_mode_flags[5],
            unit_wx,
            unit_wy,
            unit_goto_wx,
            unit_goto_wy,
            _map_plane,
            &movepath_x_array[2],
            &movepath_y_array[2],
            &movepath_cost_array[0],
            1,
            30,
            boatrider_count,
            troop_count,
            player_idx
        );
    }

    if(_active_stack_path_length > 0)
    {
        _active_stack_has_path = ST_TRUE;
    }

}


// WZD o64p11
// drake178: OVL_DrawPath()
void Draw_Active_Stack_Movement_Path(int16_t path_length, int8_t movepath_x_array[], int8_t movepath_y_array[])
{
    int16_t movepath_y;
    int16_t movepath_x;
    int16_t itr_path_length;  // _SI_

    for(itr_path_length = 0; itr_path_length < path_length; itr_path_length++)
    {
        movepath_x = movepath_x_array[itr_path_length];
        movepath_y = movepath_y_array[itr_path_length];

        if(World_To_Screen(_map_x, _map_y, &movepath_x, &movepath_y) == ST_TRUE)
        {
            FLIC_Draw((8 + movepath_x), (6 + movepath_y), goto_booty_icon);
        }
    }
}



/*
    WIZARDS.EXE  ovr095
*/

// WZD o95p01
// AKA Move_Stack()
/*
RdBd_UNIT_MoveStack_WIP()
    path_type = Special_Move = 2, if entire stack is roadbuilders

path_type
    switch() {}
        2: 

*/
int16_t Move_Units(int16_t player_idx, int16_t destination_x, int16_t destination_y, int16_t path_type, int16_t * map_x, int16_t * map_y, int16_t map_p, int16_t troop_count, int16_t troops[])
{
    int16_t boat_rider_array[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t movement_mode_flags[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // BUG should be 6
    int16_t construction_points = 0;
    int16_t uu_road_builder_unit_idx = 0;
    int16_t boatrider_count = 0;
    int16_t UU_flag_FALSE = 0;
    int16_t UU_unit_wp = 0;
    int16_t Total_Move_Cost = 0;
    int16_t Out_Of_Moves = 0;
    int16_t movement_points = 0;
    int16_t defender_idx = 0;
    int16_t attack_flag = 0;
    int16_t Current_Step = 0;
    int16_t YPos = 0;
    int16_t XPos = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t path_length = 0;
    int16_t First_Unit_Index = 0;

    int16_t unit_idx = 0;
    int16_t return_value = 0;
    int16_t itr_eight = 0;
    int16_t itr_Path_Length = 0;
    int16_t itr_units = 0;

    int16_t lair_combat_result = 0;
    int16_t combat_result = 0;
    int16_t itr_troops = 0;  // _SI_
    int16_t itr_path = 0;  // _SI_

    assert(*map_x >= WORLD_X_MIN && *map_x <= WORLD_X_MAX);  /*  0 & 59 */
    assert(*map_y >= WORLD_Y_MIN && *map_y <= WORLD_Y_MAX);  /*  0 & 39 */

    unit_idx = troops[0];
    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    UU_unit_wp = _UNITS[unit_idx].wp;

    UU_flag_FALSE = ST_FALSE;

    Set_Draw_Active_Stack_Always();
    Reset_Map_Draw();

    movement_points = Units_Moves(troops, troop_count);

    if(movement_points < 1)
    {
        goto Done_Return_FALSE;
    }

    for(itr_eight = 0; itr_eight < 8; itr_eight++)  // BUG should be 6 - the flags don't include teleporting or merging
    {
        movement_mode_flags[itr_eight] = 0;
    }
    Army_Movement_Modes(&movement_mode_flags[0], troops, troop_count);
    boatrider_count = Army_Boatriders(troop_count, troops, &boat_rider_array[0]);

    switch(path_type)
    {
        case 0:  { goto Prep_Move_Path; } break;
        case 1:  { goto Start_Path;     } break;
        case 2:  { goto Prep_Road_Path; } break;
        case 3:  { goto Prep_Move_Path; } break;
        default: { goto Start_Path;     } break;
    }

Prep_Road_Path:
{

    // @@IDK_AllUnitsRoadBuilding:
    XPos = _UNITS[unit_idx].Rd_From_X;
    YPos = _UNITS[unit_idx].Rd_From_Y;


    // ; BUG: should either use the same procedure for human roadbuilding, or otherwise utilise a function that can check for path validity
    if(player_idx == _human_player_idx)
    {
        path_length = Path_Wrap(XPos, YPos, destination_x, destination_y, &movepath_x_array[2], &movepath_y_array[2], WORLD_WIDTH);
    }
    else
    {
        path_length = OVL_GetRoadPath(XPos, YPos, destination_x, destination_y, map_p, &movepath_x_array[2], &movepath_y_array[2]);
    }

    Current_Step = -1;

    for(itr_path = 0; itr_path < path_length; itr_path++)
    {
        if((movepath_x_array[(2 + itr_path)] == unit_wx) && (movepath_y_array[(2 + itr_path)] == unit_wy))
        {
                Current_Step = itr_path;
                break;
        }
    }

    if(Current_Step != -1)
    {
        path_length -= Current_Step;
        for(itr_path = 0; itr_path < path_length; itr_path++)
        {
            movepath_x_array[(2 + itr_path)] = movepath_x_array[(3 + (Current_Step + itr_path))];
            movepath_y_array[(2 + itr_path)] = movepath_y_array[(3 + (Current_Step + itr_path))];
        }
    }

    // ¿ ~ same as code-block from Road_Build_Screen() ?
    construction_points = 0;
    // @@Loop_Troops__AccumulateConstruction
    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        unit_idx = troops[itr_troops];
        if(_unit_type_table[_UNITS[unit_idx].type].Construction > 0)
        {
            construction_points += _unit_type_table[_UNITS[unit_idx].type].Construction;

            uu_road_builder_unit_idx = unit_idx;
            if(Unit_Has_Endurance(unit_idx) == ST_TRUE)
            {
                uu_road_builder_unit_idx = unit_idx;
                construction_points += 1;
            }
        }
    }
    // @@Next_Troop__AccumulateConstruction

    for(itr_path = 0; itr_path < path_length; itr_path++)
    {
        movepath_cost_array[itr_path] = Turns_To_Build_Road(movepath_x_array[(2 + itr_path)], movepath_y_array[(2 + itr_path)], map_p);
        if(construction_points > 0)
        {
            movepath_cost_array[itr_path] /= construction_points;
        }
        SETMIN(movepath_cost_array[itr_path], 1);
    }


    Current_Step = 1;

    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {

        if(_UNITS[troops[itr_troops]].Rd_Constr_Left > 0)
        {
            Current_Step = 0;
            _UNITS[troops[itr_troops]].Finished = ST_TRUE;
            _UNITS[troops[itr_troops]].Rd_Constr_Left -= 1;
        }
        else
        {
            if(path_length == 0)
            {
                Current_Step = 0;
                _UNITS[troops[itr_troops]].Rd_Constr_Left = ST_UNDEFINED;
                _UNITS[troops[itr_troops]].Status = us_Ready;
                TILE_CreateRoad(_UNITS[troops[itr_troops]].wx, _UNITS[troops[itr_troops]].wy, map_p);
                TILE_ResetRoadConns(_UNITS[troops[itr_troops]].wx, _UNITS[troops[itr_troops]].wy, map_p);
            }
            else
            {
                _UNITS[troops[itr_troops]].Rd_Constr_Left = movepath_cost_array[0];
                Current_Step = 1;
            }
        }

        // Where is this?
        // ovr095:03FC loc_7BC7C:
        // 000714CC  0007BC7C
        if(Current_Step == 1)
        {
            path_length = 1;
        }
        else
        {
            path_length = 0;
        }

    }

    // @@END_ROAD_BUILD:

    goto Start_Path;
}

Prep_Move_Path:
{

    path_length = Make_Move_Path(
        movement_mode_flags[0],
        movement_mode_flags[1],
        movement_mode_flags[2],
        movement_mode_flags[3],
        movement_mode_flags[4],
        movement_mode_flags[5],
        unit_wx,
        unit_wy,
        destination_x,
        destination_y,
        map_p,
        &movepath_x_array[2],
        &movepath_y_array[2],
        &movepath_cost_array[0],
        1,
        movement_points,
        boatrider_count,
        troop_count,
        player_idx
    );

    goto Start_Path;

}

Start_Path:
{

    if(path_length < 1)
    {
        goto Done_Return_FALSE;
    }

    Out_Of_Moves = ST_FALSE;
    attack_flag = ST_FALSE;
    OVL_SWardTriggered = ST_FALSE;


    Eval_Move_Path__WIP(
        player_idx,
        &movepath_x_array[2],
        &movepath_y_array[2],
        map_p,
        &movepath_cost_array[0],
        movement_points,
        &defender_idx,
        &attack_flag,
        &path_length,
        &Out_Of_Moves,
        troops,
        troop_count
    );


    // ¿ accumulate path cost ?
    Total_Move_Cost = 0;
    for(itr_Path_Length = 0; itr_Path_Length < path_length; itr_Path_Length++)
    {
        Total_Move_Cost += movepath_cost_array[itr_Path_Length];
    }

    if(path_length <= 1)
    {
        OVL_Action_OriginX = unit_wx;
        OVL_Action_OriginY = unit_wy;
    }
    else
    {
        OVL_Action_OriginX = movepath_x_array[(path_length - 1)];
        OVL_Action_OriginY = movepath_y_array[(path_length - 1)];
    }

    if(path_length <= 0)
    {
        Total_Move_Cost = 0;
        Out_Of_Moves = ST_FALSE;
    }
    else
    {
        assert(*map_x >= WORLD_X_MIN && *map_x <= WORLD_X_MAX);  /*  0 & 59 */
        assert(*map_y >= WORLD_Y_MIN && *map_y <= WORLD_Y_MAX);  /*  0 & 39 */

        Move_Units_Draw(player_idx, map_p, path_length, map_x, map_y, troops, troop_count);
    }

    // BUGBUG  can't be get spell warded and also go into combat
    if( (OVL_SWardTriggered == ST_TRUE) && (player_idx == _human_player_idx) )
    {
        // TODO  OVL_SpellWardError();
    }

    if((attack_flag == ST_TRUE) || (UU_flag_FALSE == ST_TRUE))
    {
        goto Combat_Handlers;
    }

    goto End_Of_Moving;

}


Combat_Handlers:
{
    // Combat_Move == ST_TRUE
    // loc_7BD7B:
    // TODO  First_Unit_Index = unit_idx;                // unit_idx of passed in troops[0]
    // TODO  CRP_OVL_Obstacle_Var1 = ST_FALSE;           // cmp'd in Move_Stack(), if FALSE, calls empty function pointer; set to FALSE if OVL_MapVar3 is TRUE, right before call to Move_Units()
    // TODO  CRP_OVL_Obstacle_Var2 = First_Unit_Index;   // OON XREF;  CRP_OVL_Obstacle_Var2 = First_Unit_Index = _DI_unit_idx = troops[0]
    // TODO  CRP_OVL_Obstacle_Var3 = attackee_idx;       // OON XREF;  CRP_OVL_Obstacle_Var3 = attackee_idx = Obstacle_Value = dst_troops[0] || s_CITY.owner_idx || NEUTRAL_PLAYER;  set in Eval_Move_Path__WIP(); 

    // set in Eval_Move_Path__WIP() (AKA STK_EvaluatePath)
    // {0: Enemy Stack, 1: Enemy City, 5: Lair}
    if(OVL_Action_Type == 5)
    {
        lair_combat_result = Lair_Combat__WIP(OVL_Action_Structure, player_idx);

        if(lair_combat_result == 0)  // ~ Winner == False / Combat - Lose
        {
            for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {
                if(
                    (_UNITS[troops[itr_troops]].wx == _combat_wx) &&
                    (_UNITS[troops[itr_troops]].wy == _combat_wy)
                )
                {
                    _UNITS[troops[itr_troops]].wx = OVL_Action_OriginX;
                    _UNITS[troops[itr_troops]].wy = OVL_Action_OriginY;
                }
            }
        }
        else if(lair_combat_result == 1)  // ~ Winner == True / Combat - Win
        {
            Player_Army_At_Square(_combat_wx, _combat_wy, _combat_wp, player_idx, &troop_count, troops);

        }
        else if(lair_combat_result == 99)  // ~ Combat - Cancel
        {
            for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {
                _UNITS[troops[itr_troops]].wx = OVL_Action_OriginX;
                _UNITS[troops[itr_troops]].wy = OVL_Action_OriginY;
            }
        }
    }
    else
    {
        combat_result = Combat__WIP(player_idx, defender_idx, troop_count, &troops[0]);

        if(combat_result == ST_TRUE)  /* Winner == Attacker */
        {
            Player_Army_At_Square(_combat_wx, _combat_wy, _combat_wp, player_idx, &troop_count, troops);
        }
    }

}


End_Of_Moving:
{

    _combat_wx = ST_UNDEFINED;
    _combat_wy = ST_UNDEFINED;

    Units_In_Tower(troop_count, troops, map_p);

    for(itr_units = 0; itr_units < troop_count; itr_units++)
    {
        unit_idx = troops[itr_units];
        if(_UNITS[unit_idx].Rd_Constr_Left == 99)
        {
            _UNITS[unit_idx].Rd_Constr_Left = ST_UNDEFINED;
        }

        if((_UNITS[unit_idx].Finished == ST_FALSE) && (_UNITS[unit_idx].Status != PATROL))
        {
            _UNITS[unit_idx].moves2 -= Total_Move_Cost;

            if(_UNITS[unit_idx].moves2 < 1)
            {
                _UNITS[unit_idx].Finished = ST_TRUE; 
                _UNITS[unit_idx].moves2 = 0;
            }

            if(
                (_UNITS[unit_idx].Status != us_GOTO) &&
                (attack_flag == ST_TRUE) &&
                (_UNITS[unit_idx].owner_idx == _human_player_idx)
            )
            {
                _UNITS[unit_idx].Status = us_Ready;
                _UNITS[unit_idx].dst_wx = 0;
                _UNITS[unit_idx].dst_wy = 0;
                Out_Of_Moves = ST_FALSE;
            }
            
            if(
                (Out_Of_Moves == ST_TRUE) &&
                (_UNITS[unit_idx].Status != us_Move)
            )
            {
                _UNITS[unit_idx].Status = us_GOTO;
                _UNITS[unit_idx].dst_wx = destination_x;
                _UNITS[unit_idx].dst_wy = destination_y;
                _UNITS[unit_idx].Finished = ST_TRUE;
            }

            if(
                (_UNITS[unit_idx].Status == us_GOTO) &&
                (_UNITS[unit_idx].wx == _UNITS[unit_idx].dst_wx) &&
                (_UNITS[unit_idx].wy == _UNITS[unit_idx].dst_wy)
            )
            {
                if(_UNITS[unit_idx].Rd_Constr_Left == -1)
                {
                    _UNITS[unit_idx].Status = us_Ready;
                    _UNITS[unit_idx].dst_wx = 0;
                    _UNITS[unit_idx].dst_wy = 0;
                    if(_UNITS[unit_idx].moves2 > 0)
                    {
                        _UNITS[unit_idx].Finished = ST_FALSE;
                    }
                }
                Out_Of_Moves = ST_FALSE;
            }

            if(_UNITS[unit_idx].Status == us_GOTO)
            {
                _UNITS[unit_idx].Finished = ST_TRUE;
            }
        }
    }

    // DEMO  Fix_Patrol_On_Boats(troop_count, troops);
    
    Reset_Draw_Active_Stack();

    if(player_idx == _human_player_idx)
    {
        Update_Scouted_And_Contacted__WIP();
    }

    goto Done_Return_TRUE;

}


    goto Done;


Done_Return_FALSE:
    Reset_Draw_Active_Stack();
    return_value = ST_FALSE;
    goto Done;


Done_Return_TRUE:
    return_value = ST_TRUE;
    goto Done;


Done:

    return return_value;
}

// WZD o95p02
/*

OON XREF
Move_Units()
    Move_Units_Draw(player_idx, map_p, path_length, map_x, map_y, troops, troop_count);

*/
void Move_Units_Draw(int16_t player_idx, int16_t map_p, int16_t movepath_length, int16_t * map_x, int16_t * map_y, int16_t unit_array[], int16_t unit_array_count)
{

    int16_t display_moves;
    int16_t Highest_Priority;
    int16_t curr_src_wy;
    int16_t curr_src_wx;
    int16_t Move_Stages;
    int16_t curr_dst_sy;
    int16_t curr_dst_sx;
    int16_t curr_src_sy;
    int16_t curr_src_sx;
    int16_t scout_range;
    int16_t Building_Road;
    int16_t curr_dst_wy;
    int16_t curr_dst_wx;
    int16_t unit_pict_sy;
    int16_t unit_pict_sx;
    int16_t unit_y;
    int16_t unit_x;
    int16_t Highest_Priority_Unit__Loop_Var;
    int16_t destination_y;
    int16_t destination_x;  // In the Dasm, doubles as `invisibility_unit_count`
    int16_t unit_pict_shift_sy;
    int16_t unit_pict_shift_sx;
    int16_t unit_idx;

    int16_t itr_path_length;
    int16_t itr_unit_array_count;
    int16_t invisibility_unit_count;
    int16_t itr_move_stages;
    int16_t itr_units;

    assert(*map_x >= WORLD_X_MIN && *map_x <= WORLD_X_MAX);  /*  0 & 59 */
    assert(*map_y >= WORLD_Y_MIN && *map_y <= WORLD_Y_MAX);  /*  0 & 39 */

    unit_idx = unit_array[0];

    /*
        BEGIN: display_moves
    */
    display_moves = ST_FALSE;
    if(player_idx == _human_player_idx)
    {
        display_moves = ST_TRUE;
    }
    if( (player_idx != _human_player_idx) && (magic_set.Enemy_Moves == ST_TRUE) )
    {
        for(itr_path_length = 0; (itr_path_length < movepath_length) && (display_moves == ST_FALSE); itr_path_length++)
        {
            if(Check_Square_Scouted(movepath_x_array[itr_path_length], movepath_y_array[itr_path_length], map_p) == ST_TRUE)
            {
                display_moves = ST_TRUE;
            }
        }

        if(display_moves == ST_TRUE)
        {
            invisibility_unit_count = 0;
            for(itr_unit_array_count = 0; itr_unit_array_count < unit_array_count; itr_unit_array_count++)
            {
                if(Unit_Has_Invisibility(unit_array[itr_unit_array_count]) == ST_TRUE)
                {
                    invisibility_unit_count++;
                }
            }
            if(invisibility_unit_count == unit_array_count)
            {
                display_moves = ST_FALSE;
            }
        }
    }
    /*
        END: display_moves
    */

    if(display_moves == ST_TRUE)
    {
        OVL_BringIntoView(map_x, map_y, _UNITS[unit_idx].wx, _UNITS[unit_idx].wy, map_p);
        _map_x = *map_x;
        _map_y = *map_y;
        Set_Unit_Draw_Priority();
        Set_Entities_On_Map_Window(*map_x, *map_y, map_p);
        Reset_Map_Draw();
        MainScr_Create_Reduced_Map_Picture();

        assert(*map_x >= WORLD_X_MIN && *map_x <= WORLD_X_MAX);  /*  0 & 59 */
        assert(*map_y >= WORLD_Y_MIN && *map_y <= WORLD_Y_MAX);  /*  0 & 39 */

        Main_Screen_Draw_Do_Draw(map_x, map_y, map_p, *map_x, *map_y, player_idx);

        PageFlip_FX();
        Copy_On_To_Off_Page();
        Reset_Map_Draw();
    }

    /*
        BEGIN: scout_range
    */
    scout_range = 1;
    for(itr_unit_array_count = 0; itr_unit_array_count < unit_array_count; itr_unit_array_count++)
    {
        if(_UNITS[unit_array[itr_unit_array_count]].Sight_Range > scout_range)
        {
            scout_range = _UNITS[unit_array[itr_unit_array_count]].Sight_Range;
        }
    }
    if(scout_range < 2)
    {
        for(itr_unit_array_count = 0; itr_unit_array_count < unit_array_count; itr_unit_array_count++)
        {
            if( 
                (Unit_Has_AirTravel(itr_unit_array_count) == ST_TRUE) ||
                (Unit_Has_WindWalking(itr_unit_array_count) == ST_TRUE) ||
                /* DONT  (o71p08_Empty_pFxn(itr_unit_array_count) == ST_TRUE) || */
                (Unit_Has_AirTravel_Item(itr_unit_array_count) == ST_TRUE)
            )
            {
                scout_range = 2;
            }
        }
    }
    /*
        END: scout_range
    */

    Highest_Priority = _UNITS[unit_array[0]].Draw_Priority;
    Highest_Priority_Unit__Loop_Var = 0;
    _UNITS[unit_array[0]].Draw_Priority = 0;
    for(itr_unit_array_count = 1; itr_unit_array_count < unit_array_count; itr_unit_array_count++)
    {
        if(_UNITS[unit_array[itr_unit_array_count]].Draw_Priority > Highest_Priority)
        {
            Highest_Priority = _UNITS[unit_array[itr_unit_array_count]].Draw_Priority;
            Highest_Priority_Unit__Loop_Var = itr_unit_array_count;
        }
        _UNITS[unit_array[itr_unit_array_count]].Draw_Priority = 0;
    }
    unit_idx = unit_array[Highest_Priority_Unit__Loop_Var];
    unit_x = _UNITS[unit_idx].wx;
    unit_y = _UNITS[unit_idx].wy;

    destination_x = movepath_x_array[(1 + movepath_length)];
    destination_y = movepath_y_array[(1 + movepath_length)];

    for(itr_units = 0; itr_units < unit_array_count; itr_units++)
    {
        _UNITS[unit_array[itr_units]].wx = destination_x;
        _UNITS[unit_array[itr_units]].wy = destination_y;
    }


    Building_Road = ST_FALSE;
    if(_UNITS[unit_idx].Rd_Constr_Left != -1)
    {
        if(_UNITS[unit_idx].Rd_Constr_Left == 99)
        {
            _UNITS[unit_idx].Rd_Constr_Left = -1;
        }
        Building_Road = ST_TRUE;
    }


    Reset_Window();
    Set_Page_Off();

    if(display_moves == ST_TRUE)
    {
        Set_Draw_Active_Stack_Never();
        Set_Entities_On_Map_Window(*map_x, *map_y, map_p);
    }

    curr_src_wx = unit_x;
    curr_src_wy = unit_y;


    if(Building_Road == ST_TRUE)
    {
        TILE_CreateRoad(unit_x, unit_y, map_p);
        TILE_ResetRoadConns(unit_x, unit_y, map_p);
    }


    for(itr_path_length = 0; itr_path_length < movepath_length; itr_path_length++)
    {
        curr_dst_wx = movepath_x_array[(2 + itr_path_length)];
        curr_dst_wy = movepath_y_array[(2 + itr_path_length)];

        unit_pict_shift_sx = 0;
        if(curr_dst_wx > unit_x)  /* ¿ moving to the right ? */
        {

            if(unit_x == 0 && curr_dst_wx == WORLD_X_MAX)
            {
                unit_pict_shift_sx = -4;  // MAP_WIDTH / 3
                unit_x -= 1;
                if(unit_x == 0)
                {
                    unit_x = WORLD_X_MAX;
                }
            }
            else
            {
                unit_pict_shift_sx = 4;  // MAP_WIDTH / 3
                unit_x += 1;
                if(unit_x == WORLD_WIDTH)
                {
                    unit_x = 0;
                }
            }
        }

        if(curr_dst_wx < unit_x)  /* ¿ moving to the left ? */
        {
            if(unit_x == WORLD_X_MAX && curr_dst_wx == 0)
            {
                unit_pict_shift_sx = 4;  // MAP_WIDTH / 3
                unit_x += 1;
                if(unit_x == WORLD_WIDTH)
                {
                    unit_x = 0;
                }
            }
            else
            {
                unit_pict_shift_sx = -4;  // MAP_WIDTH / 3
                unit_x -= 1;
                if(unit_x == 0)
                {
                    unit_x = WORLD_X_MAX;
                }
            }
        }

        unit_pict_shift_sy = 0;
        if(curr_dst_wy > unit_y)  /* ¿ moving down ? */
        {
            unit_pict_shift_sy = 3;  // MAP_HEIGHT / 3
            unit_y += 1;
        }
        if(curr_dst_wy < unit_y)  /* ¿ moving up ? */
        {
            unit_pict_shift_sy = -3;  // MAP_HEIGHT / 3
            unit_y -= 1;
        }


        Move_Stages = 0;
        if(unit_pict_shift_sx == 0)
        {
            Move_Stages = 6;  // MAP_WIDTH / 2
        }
        if(unit_pict_shift_sy == 0)
        {
            Move_Stages = 5;  // MAP_HEIGHT / 2
        }

        if(Move_Stages == 0)
        {
            if(unit_pict_shift_sy > 0)
            {
                unit_pict_shift_sy = 9;  // MAP_HEIGHT - 1
            }
            else
            {
                unit_pict_shift_sy = -9;  // MAP_HEIGHT - 1
            }
            if(unit_pict_shift_sx > 0)
            {
                unit_pict_shift_sx = 10;  // MAP_WIDTH - 2
            }
            else
            {
                unit_pict_shift_sx = -10;  // MAP_WIDTH - 2
            }
            Move_Stages = 2;
        }


        if(display_moves == ST_TRUE)
        {
            if(OVL_TileOffScrnEdge(map_x, map_y, unit_x, unit_y, MAP_WIDTH, MAP_HEIGHT) == ST_TRUE)
            {
                Reset_Map_Draw();

                if(player_idx == _human_player_idx)
                {
                    _map_x = *map_x;
                    _map_y = *map_y;
                }

                Set_Entities_On_Map_Window(*map_x, *map_y, map_p);

                unit_pict_sx = curr_src_wx;  // HERE: curr_src_wx = unit_x || MovePath_X[itr_path_length]
                unit_pict_sy = curr_src_wy;
                World_To_Screen(*map_x, *map_y, &unit_pict_sx, &unit_pict_sy);

                if(player_idx == _human_player_idx)
                {
                    // ; the same call with the same parameters is repeated below, except it more appropriately also updates the minimap after the call
                    // ; explores map tiles in the specified radius, usually referred to as the scouting range
                    TILE_ExploreRadius__WIP(unit_x, unit_y, map_p, scout_range);
                }

            }
            else
            {
                unit_pict_sx = curr_src_wx;  // HERE: curr_src_wx = unit_x || MovePath_X[itr_path_length]
                unit_pict_sy = curr_src_wy;
                World_To_Screen(*map_x, *map_y, &unit_pict_sx, &unit_pict_sy);
            }

            curr_src_sx = curr_src_wx;  // HERE: curr_src_wx = unit_x || MovePath_X[itr_path_length]
            curr_src_sy = curr_src_wy;
            World_To_Screen(*map_x, *map_y, &curr_src_sx, &curr_src_sy);

            curr_dst_sx = curr_dst_wx;  // HERE: curr_dst_wx = MovePath_X[itr_path_length]
            curr_dst_sy = curr_dst_wy;
            World_To_Screen(*map_x, *map_y, &curr_dst_sx, &curr_dst_sy);

            Set_Window(0, 20, 239, 199);


            // @@Loop_MoveStages
            for(itr_move_stages = 0; itr_move_stages < Move_Stages; itr_move_stages++)
            {
                Set_Page_Off();

                unit_pict_sx = unit_pict_sx + unit_pict_shift_sx;
                unit_pict_sy = unit_pict_sy + unit_pict_shift_sy;

                if( (itr_move_stages == 1) && (player_idx == _human_player_idx) )
                {
                    Reset_Map_Draw();
                    TILE_ExploreRadius__WIP(unit_x, unit_y, map_p, scout_range);
                    MainScr_Create_Reduced_Map_Picture();
                }

                Set_Window(0, 20, 239, 199);
                Draw_Maps(0, 20, MAP_WIDTH, MAP_HEIGHT, map_x, map_y, map_p, *map_x, *map_y, player_idx);

                Set_Window(0, 20, 239, 199);
                // TODO  OVL_DrawMapSection(curr_src_sx, curr_src_sy, map_p, *map_x, *map_y);
                // TODO  OVL_DrawMapSection(curr_dst_sx, curr_dst_sy, map_p, *map_x, *map_y);

                // ; BUG: this call is unnecessary, and will glitch the
                // ; city's image over any surrounding units for the
                // ; duration of the movement "animation"
                // TODO  RP_OVL_DrawCities2(0, 20, 12, 10, *map_x, *map_y, map_p);
                // ; draws cities into the current draw segment based on
                // ; the specified map parameters, unless the tile they
                // ; are on is unscouted
                // ;
                // ; byte-identical to OVL_DrawCities without the city
                // ;   screen draw window setup
                // ; entirely unnecessary and can be freely removed
                // ;
                // ; contains 2 BUGs: non-walled cities use the wrong
                // ; graphics due to an outdated value check, and cities
                // ; at a relative X coordinate of -1 don't get their
                // ; image flowing into the screen as they should

                Set_Window(0, 20, 239, 199);
                Draw_Unit_Picture(unit_pict_sx, unit_pict_sy, unit_idx, 2);

                Set_Window(0, 20, 239, 199);
                // TODO  Redraw_Map_Unexplored_Area(0, 20, MAP_WIDTH, MAP_HEIGHT, *map_x, *map_y, map_p);

                PageFlip_FX();

            }

        }

        // TOOD  AI_ContactWizards(player_idx, unit_x, unit_y, map_p, scout_range);


        curr_src_wx = movepath_x_array[(2 + itr_path_length)];
        curr_src_wy = movepath_y_array[(2 + itr_path_length)];

    }  /* for(itr_path_length = 0; itr_path_length < path_length; itr_path_length++) */


    Reset_Window();
    Reset_Map_Draw();

}

// WZD o95p03
// AI_ContactWizards()

// WZD o95p04
// drake178: G_STK_OvlObstacles()
/*
    Update Move-Map for Pathing Finding Algorithm
    allows pathed to, but disallows pathed through

; processes overland obstacles on the passed movement
; map array, marking tiles as impassable if they can't
; be pathed through
;
; enemy armies are passable if visible relations are
; below -60 and their stack size is equal or smaller
;
; contains some weird transport-related tile enablers,
*/
void Update_MovePathMap(int8_t * ptr_movepath_cost_map_moves2, int16_t boatrider_count, int16_t troop_count, int16_t wp, int16_t player_idx, int16_t dst_wx, int16_t dst_wy, int16_t src_wx, int16_t src_wy)
{
    int16_t dst_troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t checked_troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t checked_troops_boatriders_count = 0;
    int16_t dst_troop_count = 0;
    int16_t checked_troop_count = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t dst_troops_carry_capacity = 0;
    int16_t itr_troops = 0;
    int16_t itr_units = 0;   // _DI_
    int16_t itr_lairs = 0;   // _DI_
    int16_t itr_cities = 0;  // _DI_

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if((_UNITS[itr_units].wp != wp) || (_UNITS[itr_units].owner_idx == ST_UNDEFINED))
        {
            continue;
        }

        unit_wx = _UNITS[itr_units].wx;
        unit_wy = _UNITS[itr_units].wy;

        if(_UNITS[itr_units].owner_idx == NEUTRAL_PLAYER_IDX)
        {
            if(player_idx != NEUTRAL_PLAYER_IDX)
            {
                if((unit_wx != dst_wx) || (unit_wy != dst_wy))
                {
                    ptr_movepath_cost_map_moves2[((unit_wy * WORLD_WIDTH) + unit_wx)] = -1;  // ~== Lairs, Cities - only allow dst in path finding
                }
            }
            else
            {
                ptr_movepath_cost_map_moves2[((unit_wy * WORLD_WIDTH) + unit_wx)] = -1;  // neutral player can't merge stacks
            }

            continue;
        }

        if(_UNITS[itr_units].owner_idx != player_idx)
        {
            if((unit_wx != dst_wx) || (unit_wy != dst_wy))
            {
                if(ptr_movepath_cost_map_moves2[((unit_wy * WORLD_WIDTH) + unit_wx)] != -1)
                {
                    if(((_players[_UNITS[itr_units].owner_idx].Dipl.Visible_Rel[player_idx] + 100) / 20) <= 2)
                    {
                        Player_Army_At_Square(unit_wx, unit_wy, wp, _UNITS[itr_units].owner_idx, &checked_troop_count, &checked_troops[0]);
                        if(checked_troop_count > troop_count)
                        {
                            ptr_movepath_cost_map_moves2[((unit_wy * WORLD_WIDTH) + unit_wx)] = -1;
                        }
                    }
                    else
                    {
                        ptr_movepath_cost_map_moves2[((unit_wy * WORLD_WIDTH) + unit_wx)] = -1;
                    }
                }
            }

            continue;
        }

        /*
            Nay Neutral Player, Yay Owner is Player
        */

        // Unit is Trooper & Destination is Ocean
        if(
            (_UNITS[itr_units].wx == src_wx) &&
            (_UNITS[itr_units].wy == src_wy) &&
            (TBL_Landmasses[((dst_wy * WORLD_WIDTH) + dst_wx)] == TT_Ocean1)
        )
        {
            dst_troops_carry_capacity = 0;
            Player_Army_At_Square(dst_wx, dst_wy, wp, player_idx, &dst_troop_count, &dst_troops[0]);
            for(itr_troops = 0; itr_troops < dst_troop_count; itr_troops)
            {
                dst_troops_carry_capacity += _unit_type_table[_UNITS[dst_troops[itr_troops]].type].Transport;
            }
            if(dst_troops_carry_capacity > 0)
            {
                Player_Army_At_Square(dst_wx, dst_wy, wp, player_idx, &checked_troop_count, &checked_troops[0]);
                checked_troops_boatriders_count = checked_troop_count;
                for(itr_troops = 0; itr_troops < checked_troop_count; itr_troops++)
                {
                    if(
                        (_UNITS[checked_troops[itr_troops]].type > ut_Chosen) ||
                        (Unit_Has_AirTravel_Item(checked_troops[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_WaterTravel_Item(checked_troops[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_Swimming(checked_troops[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_WindWalking(checked_troops[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_AirTravel(checked_troops[itr_troops]) == ST_TRUE)
                    )
                    {
                        checked_troops_boatriders_count--;
                    }
                    if(
                        ((checked_troops_boatriders_count - 1) <= dst_troops_carry_capacity) &&
                        (checked_troops_boatriders_count <= 9)
                    )
                    {
                        ptr_movepath_cost_map_moves2[((unit_wy * WORLD_WIDTH) + unit_wx)] = 2;
                    }
                }
            }

            continue;
        }

        if((unit_wx == src_wx) && (unit_wy == src_wy))
        {
            checked_troop_count = 0;
        }
        else
        {
            Player_Army_At_Square(unit_wx, unit_wy, wp, player_idx, &checked_troop_count, &checked_troops[0]);
        }

        if((checked_troop_count + troop_count) > 9)
        {
            ptr_movepath_cost_map_moves2[((unit_wy * WORLD_WIDTH) + unit_wx)] = -1;
        }
        else
        {
            if(TBL_Landmasses[((dst_wy * WORLD_WIDTH) + dst_wx)] == TT_Ocean1)
            {
                dst_troops_carry_capacity = 0;
                Player_Army_At_Square(dst_wx, dst_wy, wp, player_idx, &dst_troop_count, &dst_troops[0]);
                for(itr_troops = 0; itr_troops < dst_troop_count; itr_troops)
                {
                    dst_troops_carry_capacity += _unit_type_table[_UNITS[dst_troops[itr_troops]].type].Transport;
                }
                if(dst_troops_carry_capacity > 0)
                {
                    checked_troops_boatriders_count = checked_troop_count;
                    for(itr_troops = 0; itr_troops < checked_troop_count; itr_troops++)
                    {
                        if(
                            (_UNITS[checked_troops[itr_troops]].type > ut_Chosen) ||
                            (Unit_Has_AirTravel_Item(checked_troops[itr_troops]) == ST_TRUE) ||
                            (Unit_Has_WaterTravel_Item(checked_troops[itr_troops]) == ST_TRUE) ||
                            (Unit_Has_Swimming(checked_troops[itr_troops]) == ST_TRUE) ||
                            (Unit_Has_WindWalking(checked_troops[itr_troops]) == ST_TRUE) ||
                            (Unit_Has_AirTravel(checked_troops[itr_troops]) == ST_TRUE)
                        )
                        {
                            checked_troops_boatriders_count--;
                        }
                        if(
                            ((checked_troops_boatriders_count - 1) <= dst_troops_carry_capacity) &&
                            (checked_troops_boatriders_count <= 9)
                        )
                        {
                            ptr_movepath_cost_map_moves2[((unit_wy * WORLD_WIDTH) + unit_wx)] = 2;
                        }
                    }
                }
            }
        }

    }


    for(itr_lairs = 0; itr_lairs < NUM_LAIRS; itr_lairs++)
    {
        if(
            (_LAIRS[itr_lairs].wp == wp) &&
            (_LAIRS[itr_lairs].Intact == ST_TRUE) &&
            ((_LAIRS[itr_lairs].wx != dst_wx) || (_LAIRS[itr_lairs].wy != dst_wy))
        )
        {
            ptr_movepath_cost_map_moves2[((_LAIRS[itr_lairs].wy * WORLD_WIDTH) + _LAIRS[itr_lairs].wx)] = -1;
        }
    }


    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(
            (_CITIES[itr_cities].wp == wp) &&
            (_CITIES[itr_cities].owner_idx != player_idx) &&
            ((_CITIES[itr_cities].wx != dst_wx) || (_CITIES[itr_cities].wy != dst_wy))
        )
        {
            ptr_movepath_cost_map_moves2[((_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)] = -1;
        }
    }
 
}


// WZD o95p05
/*
array and count of units that require a seat on a boat

XREF:
    Move_Units()
    j_Army_Boatriders()
        Build_Moveable_Stack()
        Build_RoadBuilder_Stack()
        Set_Active_Stack_Movement_Path()

*/
int16_t Army_Boatriders(int16_t troop_count, int16_t troops[], int16_t boatriders[])
{
    int16_t boatrider_count = 0;
    int16_t itr_troops = 0;

    boatrider_count = 0;

    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        if(
            (Unit_Has_WindWalking(troops[itr_troops]) == ST_TRUE)
            ||
            (ST_FALSE)  /* DONT  o71p08_Empty_pFxn()  ~ Unit_Has_WindWalking_Item() */
        )
        {
            return 0;
        }

        if(
            (Unit_Has_AirTravel(troops[itr_troops]) != ST_TRUE)
            &&
            (Unit_Has_WaterTravel(troops[itr_troops]) != ST_TRUE)
            &&
            (Unit_Has_AirTravel_Item(troops[itr_troops]) != ST_TRUE)
            &&
            (Unit_Has_WaterTravel_Item(troops[itr_troops]) != ST_TRUE)
        )
        {
            boatriders[boatrider_count] = troops[itr_troops];
            boatrider_count++;
        }
    }

    return boatrider_count;
}

// WZD o95p06
/*
OON XREF:  Move_Units()
(re-)sets units on boats to patrol status
if the stack has a boat that still has movement points
¿ because the boat will get selected again before the 'Next Turn' process does its own thing ?
*/
void Fix_Patrol_On_Boats(int16_t troop_count, int16_t troops[])
{
    int16_t Stack_Units[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t unit_owner_idx = 0;
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t Stack_Count = 0;
    int16_t Carry_Stack = 0;
    int16_t itr_troops = 0;  // _SI_
    int16_t * troops_ptr = 0;  // _DI_

    troops_ptr = &troops[0];

    unit_wx = _UNITS[troops_ptr[0]].wx;
    unit_wy = _UNITS[troops_ptr[0]].wy;
    unit_wp = _UNITS[troops_ptr[0]].wp;

    // get owner from first non-dead troop
    unit_owner_idx = ST_UNDEFINED;
    itr_troops = 0;
    while((unit_owner_idx == ST_UNDEFINED) && (itr_troops++ < troop_count))
    {
        unit_owner_idx = _UNITS[troops_ptr[itr_troops]].owner_idx;
    }

    if(unit_owner_idx == ST_UNDEFINED)
    {
        return;
    }


    Player_Army_At_Square(unit_wx, unit_wy, unit_wp, unit_owner_idx, &Stack_Count, &Stack_Units[0]);

    Carry_Stack = ST_FALSE;

    for(itr_troops = 0; itr_troops < Stack_Count; itr_troops++)
    {
        if(
            (_unit_type_table[_UNITS[Stack_Units[itr_troops]].type].Transport > 0)
            &&
            (_UNITS[Stack_Units[itr_troops]].moves2 > 0)
        )
        {
            Carry_Stack = ST_TRUE;
        }
    }

    if(Carry_Stack == ST_TRUE)
    {
        for(itr_troops = 0; itr_troops < Stack_Count; itr_troops++)
        {
            if(_unit_type_table[_UNITS[Stack_Units[itr_troops]].type].Transport < 1)
            {
                _UNITS[Stack_Units[itr_troops]].Status = us_Patrol;
                _UNITS[Stack_Units[itr_troops]].Finished = ST_TRUE;
            }
        }
    }
    else
    {
        for(itr_troops = 0; itr_troops < Stack_Count; itr_troops++)
        {
            if(_UNITS[Stack_Units[itr_troops]].Status != us_Patrol)
            {
                // jnz     short $+2
            }
        }
    }

}



/*
    WIZARDS.EXE  ovr097
*/

// WZD o97p01
// STK_EvaluatePath()
void Eval_Move_Path__WIP(int16_t player_idx, int8_t mvpth_x[], int8_t mvpth_y[], int16_t wp, int8_t mvpth_c[], int16_t moves2, int16_t * atackee_idx, int16_t * attack_flag, int16_t * path_length, int16_t * Cmplt, int16_t troops[], int16_t troop_count)
{
    int16_t Units[MAX_STACK] = { 0xBBBB };
    int16_t src_boatriders_count;
    int16_t dst_boatriders_count;
    int16_t Transport_Capacity;
    int16_t Cant_Enter;
    int16_t Path_Length_Copy;
    int16_t Stop_Short;
    int16_t Out_of_Moves_Value;
    int16_t HMoves_Used;
    int16_t lair_idx;
    int16_t UU_Encounter;   // boolean flag for movement blocked by Lair
    int16_t UU_City;        // boolean flag for movement blocked by City - spellwarded or enemy
    int16_t UU_EnemyUnit;   // boolean flag for movement blocked by Enemy Army
    // ¿ no flag for movement blocked by own army, in city or not
    int16_t city_idx;
    int16_t Unit_Count;
    int16_t unit_idx;
    int16_t Move_Interrupted;
    int16_t Path_Length;
    int16_t Combat_Path_Value;
    int16_t Obstacle_Value;
    int16_t itr_Path_Length;  // _DI_
    int16_t itr_troops;  // _SI_

    Obstacle_Value = -1;
    Combat_Path_Value = *attack_flag;
    Path_Length = *path_length;
    Out_of_Moves_Value = *Cmplt;
    Path_Length_Copy = *path_length;

    Obstacle_Value = -1;
    Combat_Path_Value = *attack_flag;
    Out_of_Moves_Value = *Cmplt;
    Path_Length = Path_Length_Copy;

    HMoves_Used = 0;
    Move_Interrupted = ST_FALSE;
    for(itr_Path_Length = 0; ((itr_Path_Length < Path_Length) && (Move_Interrupted == ST_FALSE)); itr_Path_Length++)
    {
        HMoves_Used += mvpth_c[itr_Path_Length];
        
        if(HMoves_Used >= moves2)
        {
            Path_Length = (itr_Path_Length + 1);
            Out_of_Moves_Value = ST_TRUE;
            Combat_Path_Value = ST_FALSE;
            Move_Interrupted = ST_TRUE;
        }


        lair_idx = Square_Has_Lair(mvpth_x[itr_Path_Length], mvpth_y[itr_Path_Length], wp);

        if(lair_idx != ST_UNDEFINED)
        {
            Move_Interrupted = ST_TRUE;
            UU_Encounter = 1;
            Out_of_Moves_Value = 0;
            Path_Length = itr_Path_Length;
            Path_Length = itr_Path_Length + 1;
            Combat_Path_Value = 1;
            OVL_Action_Type = 5;
            OVL_Action_Structure = lair_idx;
            Obstacle_Value = 5;
            Out_of_Moves_Value = 0;

            continue;
        }


        city_idx = Square_Has_City(mvpth_x[itr_Path_Length], mvpth_y[itr_Path_Length], wp);

        if(city_idx != ST_UNDEFINED)
        {
            // TODO  RP_CTY_CheckSpellWard__STUB()
            Cant_Enter = RP_CTY_CheckSpellWard__STUB(city_idx, &troop_count, &troops[0]);
            if(Cant_Enter == ST_TRUE)
            {
                Move_Interrupted = 1;
                Path_Length = itr_Path_Length;
                UU_City = 1;
                Out_of_Moves_Value = 0;
            }
            else
            {
                if(_CITIES[city_idx].owner_idx == player_idx)
                {
                    Player_Army_At_Square(mvpth_x[itr_Path_Length], mvpth_y[itr_Path_Length], wp, player_idx, &Unit_Count, &Units[0]);

                    if((troop_count + Unit_Count) > 9)
                    {
                        Move_Interrupted = 1;
                        Path_Length = itr_Path_Length;
                        Out_of_Moves_Value = 0;
                    }

                }
                else
                {
                    Move_Interrupted = 1;
                    Path_Length = itr_Path_Length;
                    UU_City = 1;
                    Out_of_Moves_Value = 0;
                    Path_Length = (itr_Path_Length + 1);
                    Combat_Path_Value = 1;
                    Obstacle_Value = _CITIES[city_idx].owner_idx;
                    OVL_Action_Structure = city_idx;
                    OVL_Action_Type = 1;
                    Out_of_Moves_Value = 0;
                }

            }

            continue;
        }


        Enemy_Army_At_Square(mvpth_x[itr_Path_Length], mvpth_y[itr_Path_Length], wp, player_idx, &Unit_Count, &Units[0]);

        if(Unit_Count > 0)
        {
            Move_Interrupted = 1;
            UU_EnemyUnit = 1;
            Out_of_Moves_Value = 0;
            Path_Length = itr_Path_Length;
            Path_Length = (itr_Path_Length + 1);
            Out_of_Moves_Value = 0;
            Combat_Path_Value = 1;
            Obstacle_Value = Units[0];
            OVL_Action_Type = 0;

            continue;
        }


        Player_Army_At_Square(mvpth_x[itr_Path_Length], mvpth_y[itr_Path_Length], wp, player_idx, &Unit_Count, &Units[0]);

        if((troop_count + Unit_Count) > 9)
        {
            Move_Interrupted = 1;
            Path_Length = itr_Path_Length;
            Out_of_Moves_Value = 0;

            continue;
        }

        if(Unit_Count > 0)
        {
            Transport_Capacity = 0;

            for(itr_troops = 0; itr_troops < Unit_Count; itr_troops++)
            {
                if(_unit_type_table[_UNITS[Units[itr_troops]].type].Transport > 0)
                {
                    Transport_Capacity += _unit_type_table[_UNITS[Units[itr_troops]].type].Transport;
                    Move_Interrupted = 1;
                    Path_Length = (itr_Path_Length + 1);
                    Out_of_Moves_Value = 0;
                }
            }

            for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {
                if(_unit_type_table[_UNITS[troops[itr_troops]].type].Transport > 0)
                {
                    Transport_Capacity += _unit_type_table[_UNITS[troops[itr_troops]].type].Transport;
                    Move_Interrupted = 1;
                    Path_Length = (itr_Path_Length + 1);
                    Out_of_Moves_Value = 0;
                }
            }

            if(
                (Move_Interrupted == ST_TRUE) &&
                (Transport_Capacity > 0)
            )
            {
                dst_boatriders_count = Unit_Count;
                for(itr_troops = 0; itr_troops < Unit_Count; itr_troops++)
                {
                    if(
                        (_UNITS[Units[itr_troops]].type > ut_Chosen) ||
                        (Unit_Has_AirTravel_Item(Units[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_WaterTravel_Item(Units[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_Swimming(Units[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_WindWalking(Units[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_AirTravel(Units[itr_troops]) == ST_TRUE)
                    )
                    {
                        dst_boatriders_count--;
                    }
                }
                src_boatriders_count = troop_count;
                for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                {
                    if(
                        (_UNITS[Units[itr_troops]].type > ut_Chosen) ||
                        (Unit_Has_AirTravel_Item(Units[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_WaterTravel_Item(Units[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_Swimming(Units[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_WindWalking(Units[itr_troops]) == ST_TRUE) ||
                        (Unit_Has_AirTravel(Units[itr_troops]) == ST_TRUE)
                    )
                    {
                        src_boatriders_count--;
                    }
                }

                if((dst_boatriders_count + src_boatriders_count - 1) > Transport_Capacity)
                {
                    Path_Length = itr_Path_Length;                    
                }
                else
                {
                    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                    {
                        if(_unit_type_table[_UNITS[troops[itr_troops]].type].Transport < 0)
                        {
                            // DEPR:  ¿ empty macro ?
                        }
                    }
                }
            }


        }

    }

    /*
        Cancel 'Go-To'
    */
    if(
        (Out_of_Moves_Value == ST_FALSE)
        &&
        (Move_Interrupted == ST_TRUE)
    )
    {
        Stop_Short = ST_FALSE;
        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
        {
            unit_idx = troops[itr_troops];
            if(
                (_UNITS[unit_idx].Status == us_GOTO)
                &&
                (player_idx == _human_player_idx)
            )
            {
                if(Combat_Path_Value == ST_TRUE)
                {
                    Combat_Path_Value = ST_FALSE;
                    Stop_Short = ST_TRUE;
                }
                _UNITS[unit_idx].Status = us_Ready;
                _UNITS[unit_idx].dst_wx = 0;
                _UNITS[unit_idx].dst_wy = 0;
            }
        }
        if(Stop_Short == ST_TRUE)
        {
            Path_Length--;
        }
    }


    *atackee_idx = Obstacle_Value;
    *attack_flag = Combat_Path_Value;
    *path_length = Path_Length;
    *Cmplt = Out_of_Moves_Value;
}


// WZD o97p02
// sub_7E597()

// WZD o97p03
// TODO  ¿ vs. City_At_Square__2() ?
/*
    returns city_idx or -1
*/
int16_t Square_Has_City(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t square_has_city;
    int16_t itr_cities;

    itr_cities = 0;
    square_has_city = ST_UNDEFINED;
    while(itr_cities++ < _cities && square_has_city == ST_UNDEFINED)
    {
        if((_CITIES[itr_cities].wp == wp) && (_CITIES[itr_cities].wx == wx) && (_CITIES[itr_cities].wy == wy))
        {
            square_has_city = itr_cities;
        }
    }

    return square_has_city;
}


// WZD o97p04
// AKA STK_GetMovesLeft()
// ¿ vs. WZD o63p12  Stack_Moves_Active(void) ?
// ¿ vs. WZD o63p13  Stack_Moves(void) ?
/*
    Units_Moves()
        DOES NOT have windwalker_unit_idx or stack_has_windwalker
        DOES NOT test 'active'
    
*/
int16_t Units_Moves(int16_t unit_array[], int16_t unit_array_count)
{
    int16_t itr_unit_array_count;
    int16_t movement_points; // _SI_
    int16_t unit_idx;  // _DI_

    movement_points = 1000;

    for(itr_unit_array_count = 0; itr_unit_array_count < _unit_stack_count; itr_unit_array_count++)
    {
        unit_idx = _unit_stack[itr_unit_array_count].unit_idx;
        
        if(_UNITS[unit_idx].moves2 < movement_points)
        {
            movement_points = _UNITS[unit_idx].moves2;
        }

        if(
            (_unit_type_table[_UNITS[unit_idx].type].Transport > 0)
            ||
            (Unit_Has_WindWalking(unit_idx) == ST_TRUE)
        )
        {
            movement_points = _UNITS[unit_idx].moves2;
            break;
        }

    }

    if(movement_points == 1000)
    {
        movement_points = 0;
    }

    stack_moves = movement_points;

    return stack_moves;
}


// WZD o97p05
// drake178: RP_CTY_CheckSpellWard()
/*
    determines if Stack is prohibited from entering the City due to Spell Ward
    returns {ST_FALSE,ST_TRUE}

Spell Ward:
    "...magically prohibits creatures (whether friendly or enemy) summoned from the warded realm from entering the town."
*/
int16_t RP_CTY_CheckSpellWard__STUB(int16_t city_idx, int16_t * troop_count, int16_t * troops)
{
    int16_t UU_troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Cant_Enter = 0;
    int16_t Retn_Value = 0;
    int16_t Unused_Local = 0;
    int16_t UU_l_troop_count = 0;
    int16_t UU_troops_owner_idx = 0;
    int16_t city_has_spellward = 0;
    int16_t * l_troop_count = 0;  // _DI_
    int16_t * l_troops = 0;  // _SI_

    l_troop_count = troop_count;
    l_troops = troops;

    city_has_spellward = ST_FALSE;

    UU_troops_owner_idx = _UNITS[l_troops[0]].owner_idx;

    UU_l_troop_count = *l_troop_count;

    // IDGI  Cant_Enter = CTY_CheckSpellWard(city_idx, &UU_l_troop_count, l_troops, &UU_troop_count, &UU_troops[0]);
    Cant_Enter = CTY_CheckSpellWard__STUB(city_idx, &UU_l_troop_count, l_troops);

    if(Cant_Enter == ST_TRUE)
    {
        OVL_SWardTriggered = ST_TRUE;
        city_has_spellward = ST_TRUE;
    }
 
    return city_has_spellward;
}


// WZD o97p06
// drake178: CTY_CheckSpellWard()
int16_t CTY_CheckSpellWard__STUB(int16_t city_idx, int16_t * troop_count, int16_t * troops)
{
    int16_t city_has_spellward;


    city_has_spellward = ST_FALSE;


    return city_has_spellward;
}


// WZD o97p07
// OVL_SpellWardError()


// WZD o97p08
/*

XREF:
    Main_Screen_Draw_Movement_Bar()
    CMB_DrawAUWStats()
    CMB_DrawUnitDisplay()

*/
void Print_Moves_String(int16_t x_start, int16_t y_start, int16_t moves2, int16_t right_align_flag)
{
    char buffer[8];
    char string[8];
    int16_t moves;

    if(moves2 < 0)
    {
        moves = 0;
    }
    else
    {
        moves = moves2 / 2;
    }
    
    strcpy(string, "");  // WZD dseg:569A cnst_Nullstring[] = {'\0'}

    if(moves > 0)
    {
        itoa(moves, buffer, 10);
        strcat(string, buffer);
    }

    if((moves2 % 2) != 0)
    {
        strcat(string, ".5");  // WZD dseg:56C9 cnst_Half_2[] = ".5"
    }

    if(right_align_flag == ST_FALSE)
    {
        Print(x_start, y_start, string);
    }
    else  /* DEDU  ¿ WTF:  else if(right_align_flag == ST_TRUE) ? */
    {
        Print_Right(x_start, y_start, string);
    }

}


/*
_map_x,y
_main_map_grid_x,y
mouse_x,y
screen_x,y
field_x,y
grid_x,y
world_x,y
*/
void Main_Screen_Draw_Debug_Information(void)
{
    int16_t mouse_x;
    int16_t mouse_y;
    int16_t screen_x;
    int16_t screen_y;
    int16_t field_x;
    int16_t field_y;
    int16_t grid_x;
    int16_t grid_y;
    int16_t world_x;
    int16_t world_y;
    int pos;

    Set_Outline_Color(0);
    // Set_Font_Style_Shadow_Down(0, 0, 0, 0);
    Set_Font_Style_Shadow_Down(1, 0, 0, 0);
    Set_Alias_Color(8);

    pos = 0;

    //   M   a   p       X   ,   Y
    // ~ 6 + 5 + 5 + 4 + 6 + 3 + 6 = 35 + ? 10
    Print(2, (22+(8*pos)), "Map X,Y");
    Print_Integer(45, (22+(8*pos)), _map_x);
    Print_Integer(57, (22+(8*pos)), _map_y);
    pos++;

    //   G   r   i   d       X   ,   Y
    // ~ 5 + 5 + 2 + 5 + 4 + 6 + 3 + 6 = 36 + ? 9
    Print(2, (22+(8*pos)), "Grid X,Y");
    Print_Integer(45, (22+(8*pos)), (int16_t)_main_map_grid_x);
    Print_Integer(57, (22+(8*pos)), (int16_t)_main_map_grid_y);
    pos++;

    mouse_x = Pointer_X();
    mouse_y = Pointer_Y();
    screen_x = mouse_x;
    screen_y = mouse_y;
    // ~ translate screen coordinates to field coordinates
    field_x = screen_x -  0;  // always  0
    field_y = screen_y - 20;  // always 20
    // ~ translate field coordinates to grid coordinates  (from pixels to squares)
    grid_x = field_x / MOVEMENT_MAP_WIDTH;
    grid_y = field_y / MOVEMENT_MAP_HEIGHT;
    // ~ translate field coordinates to world coordinates  (from pixels to squares)
    world_x = field_x / WORLD_WIDTH;
    world_y = field_y / WORLD_HEIGHT;

    //   M   D       X   ,   Y
    // ~ 6 + 6 + 4 + 6 + 3 + 6 = 31 + ? 
    Print(2, (22+(2*8)), "MD X,Y");
    Print_Integer(40, (22+(8*pos)), mouse_x);
    Print_Integer(56, (22+(8*pos)), mouse_y);
    pos++;

    //   M   D       M   X   ,   M   Y
    // ~ 6 + 6 + 4 + 6 + 6 + 3 + 6 + 6 = 43 + ? 
    Print(2, (22+(3*8)), "MD MX,MY");
    Print_Integer(52, (22+(8*pos)), grid_x);
    Print_Integer(64, (22+(8*pos)), grid_y);
    pos++;

    //   M   D       W   X   ,   W   Y
    // ~ 6 + 6 + 4 + 6 + 6 + 3 + 6 + 6 = 43 + ? 
    Print(2, (22+(4*8)), "MD WX,WY");
    Print_Integer(52, (22+(8*pos)), world_x);
    Print_Integer(64, (22+(8*pos)), world_y);
    pos++;

    if(_unit_stack_count > 0)
    {
        //   U   N   I   T       I   D   X
        // ~ ? + ? + ? + ? + 4 + ? + 6 + 6 = 
        Print(2, (22+(8*pos)), "UNIT IDX");
        Print_Integer(46, (22+(8*pos)), _unit_stack[0].unit_idx);
        pos++;
        // UNIT WX
        Print(2, (22+(8*pos)), "UNIT WX");
        Print_Integer(46, (22+(8*pos)), _UNITS[_unit_stack[0].unit_idx].wx);
        pos++;
        // UNIT WY
        Print(2, (22+(8*pos)), "UNIT WY");
        Print_Integer(46, (22+(8*pos)), _UNITS[_unit_stack[0].unit_idx].wy);
        pos++;
    }

    Set_Font_Style_Shadow_Down(0, 0, 0, 0);
    Set_Outline_Color(0);
    // TODO  ¿ Set_Alias_Color(8); ?

}
