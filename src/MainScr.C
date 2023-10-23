/*
    WIZARDS.EXE
    ovr057
    ovr058
    ovr059
    ovr061
    ovr062
    ovr063
    ovr064
    ovr095
    ovr097

¿ MoO2  Module: SHIPSTK ?

*/

#include "MoX.H"

#include "MoM_DEF.H"

#include "MainScr.H"
#include "MainScr_Maps.H"
#include "SCastScr.H"  /* World_To_Screen() */



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
    WIZARDS.EXE  ovr059
*/

// WZD o59p01
// WZD o59p02
// WZD o59p03
// WZD o59p04
// WZD o59p05
// WZD o59p06
// WZD o59p07
// WZD o59p08
// WZD o59p09
// WZD o59p10
// WZD o59p11
// WZD o59p12

// WZD o59p13
// drake178: STK_CheckTower()
int16_t Units_In_Tower(int16_t unit_array_count, int16_t unit_array[], int16_t map_p);

// WZD o59p14
// WZD o59p15
// WZD o59p16
// WZD o59p17
// WZD o59p18

// WZD o59p19
// AKA IDK_MaybeSwitchStackPlane_s52514()
void Do_Plane_Button(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_plane);

// WZD o59p20
// AKA IDK_PlanarTravel_TestWaterCity_s52774()
int16_t Check_Stack_Plane_Shift(int16_t unit_idx, int16_t map_plane);



/*
    WIZARDS.EXE  ovr061
*/
// WZD o61p01
// IDK_ActiveUnitStack_MovesOrPath_s53150()

// WZD o61p02
// AKA Do_Move_Stack()
int16_t Move_Stack(int16_t move_x, int16_t move_y, int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p);

// WZD o61p03
// UNIT_MoveStack()

// WZD o61p04
void WIZ_NextIdleStack(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_plane);

// WZD o61p05
int16_t WIZ_NextUnit(int16_t player_idx, int16_t * map_plane);

// WZD o61p06
// STK_GetExtraActions()

// WZD o61p07
// STK_GetMovableUnits()

// WZD o61p08
void Build_Moveable_Stack(int16_t * unit_count, int16_t unit_array[]);

/*
    WIZARDS.EXE  ovr062
*/
// WZD o62p01
// DONT  int16_t o62p01_Empty_pFxn(int16_t player_idx)
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
int16_t OVL_TileOffScrnEdge(int16_t map_x, int16_t map_y, int16_t unit_x, int16_t unit_y, int16_t map_width, int16_t map_height);
// WZD o62p07
// DONT  int16_t o62p07_Empty_pFxn(int16_t unit_array_count, int16_t * unit_array);
// WZD o62p08
void Stack_Action(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p, int16_t action, int16_t destination_x, int16_t destination_y);
// WZD o62p09
// EarthGateTeleport()
// WZD o62p10
// USW_FullDisplay()

/*
    WIZARDS.EXE  ovr063
*/
// WZD o063p01
void Main_Screen_Draw_Status_Window(void);
// WZD o063p02
void Main_Screen_Draw_Do_Draw(int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t x_pos, int16_t y_pos, int16_t player_idx);
// WZD o063p03
void Draw_Unit_Enchantment_Outline(int16_t unit_idx);
// WZD o063p04
void Cycle_Unit_Enchantment_Animation(void);
// WZD o063p05
void Draw_Unit_Picture(int16_t x, int16_t y, int16_t unit_stack_unit_idx, int16_t flag);
// WZD o063p05
// AKA OVL_DrawUnitImage()
void Draw_Unit_StatFig(int16_t x, int16_t y, int16_t unit_idx, int16_t flag);
// WZD o063p06
void Unit_Window_Draw_Unit_Attributes(int16_t x, int16_t y, int16_t unit_idx);

// WZD o63p07
// drake178: OVL_DrawStackMoves
void Main_Screen_Draw_Movement_Bar();

// WZD o63p08
// drake178: OVL_DrawMoveTypes()
void Draw_Movement_Mode_Icons(int16_t x, int16_t y, int16_t unit_idx);

// WZD o63p09
// drake178: OVL_CanPlanarTravel()
int16_t Stack_Has_PlanarTravel(void);

// WZD o63p12
// AKA OVL_GetStackHMoves()
// AKA OVL_GetStackHMoves_1()
int16_t Stack_Moves_Active(void);

// WZD o63p13
// AKA OVL_GetStackHMoves_2()
int16_t Stack_Moves(void);

// WZD o63p14
// RP_TILE_HasCity2()

// WZD o63p14
// Check_Planar_Seal()

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
// WZD o64p08
void Main_Screen_Draw_Unit_Action_Locked_Buttons(void);
// WZD o64p09
void Unit_Window_Picture_Coords(int16_t unit_stack_unit_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2);


/*
    WIZARDS.EXE  ovr095
*/

// WZD o95p01
// drake178: STK_Move()
// AKA Move_Stack()
int16_t Move_Units(int16_t player_idx, int16_t destination_x, int16_t destination_y, int16_t Spec, int16_t * map_x, int16_t * map_y, int16_t map_p, int16_t unit_array_count, int16_t unit_array[]);

// WZD o95p02
// AKA OVL_MoveUnitStack()
// AKA Move_Units()
void Move_Units_Draw(int16_t player_idx, int16_t map_p, int16_t Path_Length, int16_t * map_x, int16_t * map_y, int16_t unit_array[], int16_t unit_array_count);

// WZD o95p03
// AI_ContactWizards()

// WZD o95p04
// G_STK_OvlObstacles()

// WZD o95p05
int16_t STK_GetLandlubbers(int16_t Stack_Size, int16_t Stack_Array[], int16_t LL_Array[]);

// WZD o95p06
// G_STK_SetPatrol()


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
void Print_Moves_String(int16_t x_start, int16_t y_start, int16_t half_value, int16_t right_align_flag);



// dseg:2E10                                                 BEGIN: Main Screen

// WZD dseg:2E10
// CRP_DBG_Alt_K_State dw 0
// WZD dseg:2E12
// DBG_Alt_A_State dw 0
// WZD dseg:2E14
int16_t UU_first_turn_done_flag = ST_FALSE;
// WZD dseg:2E16
// hotkey_MainScrn_X db 'X'

// WZD dseg:2E16                                                 the 4 above maybe are DBG ?             ; should use dseg:2a8a
// WZD dseg:2E16                                                 below here, maybe compiler added as encountered?

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



char main_lbx_file[] = "MAIN";                  // WZD dseg:29EF
char backgrnd_lbx_file[] = "BACKGRND";          // WZD dseg:29F4



char cstr_1st5[] = " !\"$#";
char cstr_ABC[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char cstr_WarGames[] = "WOULD YOU LIKE TO PLAY A GAME?";



// array used for color outline for enchanted units
// 5 magic realms {Chaos, Death, Life, Nature, Sorcery}
// ? 8 animation stages ?
// ? 3 colors for each stages ?
// 5 * 8 * 3 = 120
// WZD dseg:3062
// drake178: COL_UE_Chaos
// WZD dseg:30A2
// drake178: COL_UE_Death
// WZD dseg:30E2
// drake178: COL_UE_Life
// WZD dseg:3122
// drake178: COL_UE_Nature
// WZD dseg:3162
// drake178: COL_UE_Sorcery
uint8_t enchantment_outline_colors[320] = 
{
    0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x2A, 0x2B, 0x2C, 0x2C, 0x2B, 0x2A, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
    0x2B, 0x2A, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2C, 0x2A, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2C, 0x2B,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2C, 0x2B, 0x2A, 0x2A, 0x2B, 0x2C, 0x2D, 0x2C, 0x2B, 0x2A, 0x29,
    0x2B, 0x2C, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x2A, 0x2C, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x2A, 0x2B,
    0x24, 0x23, 0x22, 0x21, 0x20, 0x21, 0x22, 0x23, 0x23, 0x22, 0x21, 0x20, 0x21, 0x22, 0x23, 0x24,
    0x22, 0x21, 0x20, 0x21, 0x22, 0x23, 0x24, 0x23, 0x21, 0x20, 0x21, 0x22, 0x23, 0x24, 0x23, 0x22,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x23, 0x22, 0x21, 0x21, 0x22, 0x23, 0x24, 0x23, 0x22, 0x21, 0x20,
    0x22, 0x23, 0x24, 0x23, 0x22, 0x21, 0x20, 0x21, 0x23, 0x24, 0x23, 0x22, 0x21, 0x20, 0x21, 0x22,
    0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x0E, 0x0D, 0x0C, 0x0C, 0x0D, 0x0E, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B,
    0x0D, 0x0E, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0C, 0x0E, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0C, 0x0D,
    0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0C, 0x0D, 0x0E, 0x0E, 0x0D, 0x0C, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x0D, 0x0C, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x0E, 0x0C, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x0E, 0x0D,
    0x44, 0x43, 0x42, 0x41, 0x40, 0x41, 0x42, 0x43, 0x43, 0x42, 0x41, 0x40, 0x41, 0x42, 0x43, 0x44,
    0x42, 0x41, 0x40, 0x41, 0x42, 0x43, 0x44, 0x43, 0x41, 0x40, 0x41, 0x42, 0x43, 0x44, 0x43, 0x42,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x43, 0x42, 0x41, 0x41, 0x42, 0x43, 0x44, 0x43, 0x42, 0x41, 0x40,
    0x42, 0x43, 0x44, 0x43, 0x42, 0x41, 0x40, 0x41, 0x43, 0x44, 0x43, 0x42, 0x41, 0x40, 0x41, 0x42,
    0x64, 0x63, 0x62, 0x61, 0x60, 0x61, 0x62, 0x63, 0x63, 0x62, 0x61, 0x60, 0x61, 0x62, 0x63, 0x64,
    0x62, 0x61, 0x60, 0x61, 0x62, 0x63, 0x64, 0x63, 0x61, 0x60, 0x61, 0x62, 0x63, 0x64, 0x63, 0x62,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x63, 0x62, 0x61, 0x61, 0x62, 0x63, 0x64, 0x63, 0x62, 0x61, 0x60,
    0x62, 0x63, 0x64, 0x63, 0x62, 0x61, 0x60, 0x61, 0x63, 0x64, 0x63, 0x62, 0x61, 0x60, 0x61, 0x62
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

// XREF: Main_Screen_Load_Pictures(); Draw_Movement_Mode_Icons()
// single-loaded image
// gets indexed like an array of pictures in Draw_Movement_Mode_Icons()
SAMB_ptr move_sail_icon;
SAMB_ptr move_swim_icon;
SAMB_ptr move_mt_icon;
SAMB_ptr move_forest_icon;
SAMB_ptr move_fly_icon;
SAMB_ptr move_path_icon;
SAMB_ptr move_astral_icon;
SAMB_ptr move_wind_icon;
SAMB_ptr move_boot_icon;
SAMB_ptr movement_mode_icons[10];  // {0,...,9} 10 icons


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
// WZD dseg:CB8E 00 00                                           UU_IMG_OVL_Empty2@ dw 0                 ; DATA XREF: Terrain_Init+22Cw
// WZD dseg:CB8E                                                                                         ; single-loaded image, called "lumber camp" in the file

// WZD dseg:CB90
SAMB_ptr Warp_Node_WorkArea;                // ; used to save and manipulate the tile graphics to ; display the warp node effect


// WZD dseg:CB92
SAMB_ptr _unit_colored_backgrounds_seg[6];  // load in Terrain_Init() ovr052

// WZD dseg:CB92                                                                                         ; DATA XREF: Terrain_Init+F3w ...
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
// WZD dseg:CC22 00 00 00 00                                     terrain_lbx_001 dd 0                    ; DATA XREF: Terrain_Init+21w ...
// WZD dseg:CC26 00 00                                           terrain_001_1 dw 0                      ; DATA XREF: Draw_Map_Terrain+C0w ...
// WZD dseg:CC28 00 00                                           g_EmmHndl_TERRAIN dw 0                  ; DATA XREF: Terrain_Init:loc_4615Bw ...
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

    main_white_medal_icon = LBX_Load(main_lbx_file, 51);
    main_gold_medal_icon = LBX_Load(main_lbx_file, 52);
    main_red_medal_icon = LBX_Load(main_lbx_file, 53);
    main_magic_weapon_icon = LBX_Load(main_lbx_file, 54);
    main_mithril_weapon_icon = LBX_Load(main_lbx_file, 55);
    main_adamantium_weapon_icon = LBX_Load(main_lbx_file, 56);

    move_sail_icon = LBX_Load(main_lbx_file, 18);
    move_swim_icon = LBX_Load(main_lbx_file, 19);
    move_mt_icon = LBX_Load(main_lbx_file, 20);
    move_forest_icon = LBX_Load(main_lbx_file, 21);
    move_fly_icon = LBX_Load(main_lbx_file, 22);
    move_path_icon = LBX_Load(main_lbx_file, 23);
    move_astral_icon = LBX_Load(main_lbx_file, 36);
    move_wind_icon = LBX_Load(main_lbx_file, 37);
    move_boot_icon = LBX_Load(main_lbx_file, 38);

    movement_mode_icons[0] = LBX_Load(main_lbx_file, 18);
    movement_mode_icons[1] = LBX_Load(main_lbx_file, 19);
    movement_mode_icons[2] = LBX_Load(main_lbx_file, 20);
    movement_mode_icons[3] = LBX_Load(main_lbx_file, 21);
    movement_mode_icons[4] = LBX_Load(main_lbx_file, 22);
    movement_mode_icons[5] = LBX_Load(main_lbx_file, 23);
    movement_mode_icons[6] = LBX_Load(main_lbx_file, 36);
    movement_mode_icons[7] = LBX_Load(main_lbx_file, 37);
    movement_mode_icons[8] = LBX_Load(main_lbx_file, 38);
    // movement_mode_icons[9] ¿ drake178: "Cavalry"


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
    int16_t hotkey_idx_RightDown;
    int16_t hotkey_idx_Down;
    int16_t hotkey_idx_LeftDown;
    int16_t hotkey_idx_Right;
    int16_t hotkey_idx_Left;
    int16_t hotkey_idx_RightUp;
    int16_t hotkey_idx_LeftUp;
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
    int16_t Unit_Y;
    int16_t Unit_X;
// Bottom@= word ptr -10h
// Right@= word ptr -0Eh
    int16_t target_world_y;
    int16_t target_world_x;
    int16_t unit_idx;
// var_ConfirmDialogResponse= word ptr -6
    int16_t Stack_Index;  /* unit_idx || player_idx;  itr for _unit_stack;  also used for itr _num_players in Alt-P Debug Randomized Personality */
    int16_t leave_screen_flag;
    
    int16_t screen_changed;
    int16_t input_field_idx;
    int16_t mouse_x;
    int16_t mouse_y;

    int16_t itr_units;

    int16_t hotkey_idx_D ;  // debug_hotkey
    int16_t hotkey_idx_T ;  // test_hotkey


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen()\n", __FILE__, __LINE__);
#endif

// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d]: MainScreen - InitialMapValues\n", __FILE__, __LINE__);
//         dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
//         dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
//         dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
//         dbg_prn("DEBUG: [%s, %d]: _prev_world_y: %d\n", __FILE__, __LINE__, _prev_world_y);
//         dbg_prn("DEBUG: [%s, %d]: _main_map_grid_x: %d\n", __FILE__, __LINE__, _main_map_grid_x);
//         dbg_prn("DEBUG: [%s, %d]: _main_map_grid_y: %d\n", __FILE__, __LINE__, _main_map_grid_y);
// #endif


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
    Allocate_Reduced_Map();
    Reset_Draw_Active_Stack();
    Set_Outline_Color(0);  // ¿ BLACK / NONE / TRANSPARENT ?
    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    // Deactivate_Auto_Function();
    // Assign_Auto_Function(Main_Screen_Draw, 1);
    // IDK   _unit_window_start_x = 247;  // AKA OVL_STKUnitCards_Lft
    // IDK   _unit_window_start_y = 79;  // AKA OVL_STKUnitCards_Top
    // DONT  G_Some_OVL_Var_1 = 0;  // ? ST_FALSE ?
    // DONT  CRP_OverlandVar_2 = 0;  // ? ST_FALSE ?
    // DONT  CRP_OVL_Obstacle_Var1 = 0;  // ? ST_FALSE ?
    // DONT  OVL_MapVar3 = 1;  // ? ST_TRUE ?
    Reset_Map_Draw();
    MainScr_Prepare_Reduced_Map();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    // // TEST
    // // Export_Entities_On_Movement_Map(l_map_x, l_map_y, map_plane);
    // Validate_Entities_On_Movement_Map(_map_x, _map_y, _map_plane);

    Set_Mouse_List(1, mouse_list_default);  // ~== Set_Mouse_List_MainScr() |-> Set_Mouse_List(1, mouse_list_main/default/normal/arrow);
    // TODO  j_STK_GetExtraActions();
    // DONT  if (CRP_OverlandVar_3 != 1) { CRP_OverlandVar_3 = 0; }  // ? ST_TRUE ST_FALSE ?
    // DONT  if (CRP_OverlandVar_4 != 1) { CRP_OverlandVar_4 = 0; }  // ? ST_TRUE ST_FALSE ?
    screen_changed = ST_FALSE;
    // DONT  Local_0 = 0;  // ? ST_FALSE ?
    Set_Input_Delay(1);
    // TODO  Reset_Cycle_Palette_Color()  AKA VGA_BlinkReset()
    // TODO  Deactivate_Help_List();
    // TODO  Main_Screen_Help();  // ? |-> WZD s104 HLP_Load_OVL_View() |-> WZD s10 LBXR_DirectLoader() ?
    // TODO  DBG_Alt_A__TurnCount = -1
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

        hotkey_idx_D = Add_Hot_Key('D');  // debug_hotkey
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
            // // ¿ Req'd ? MainScr_Prepare_Reduced_Map();
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
            // Unit_X = _UNITS[unit_idx].world_x;
            // Unit_Y = _UNITS[unit_idx].world_y;
            // OVL_Map_CenterX = _UNITS[unit_idx].world_x;
            // OVL_Map_CenterY = _UNITS[unit_idx].world_y;
            // Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, Unit_X, Unit_Y);
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
                _map_plane = _UNITS[unit_idx].world_plane;
                Center_Map(&_map_x, &_map_y, _UNITS[unit_idx].world_x, _UNITS[unit_idx].world_y, _UNITS[unit_idx].world_plane);
                MainScr_Prepare_Reduced_Map();
                Set_Mouse_List(1, mouse_list_default);
                Reset_Map_Draw();
            }
        }
        
        // ? Quit Action / Unselect Unit ?
        if((input_field_idx == hotkey_idx_Q) || (input_field_idx == hotkey_idx_U) )
        {
            _unit_stack_count = 0;
            Set_Draw_Active_Stack_Always();     // OVL_ShowActiveStack();
            Set_Unit_Draw_Priority();           // UNIT_DrawPriorities();
            Reset_Stack_Draw_Priority();        // STK_NoUnitDraw();
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
// DEMO          if(input_field_idx == _info_button)
// DEMO          {
// DEMO              // TODO  SND_LeftClickSound();
// DEMO              current_screen = scr_Advisor_Screen;
// DEMO              leave_screen_flag = ST_TRUE;
// DEMO          }

        /*
            BEGIN: Game Buttons - Game Button
        */
        if(input_field_idx == _game_button)
        {
            // TODO  SND_LeftClickSound();
            current_screen = scr_Load_Screen;
            leave_screen_flag = ST_TRUE;
        }
        /*
            BEGIN: Game Buttons - Cities Button
        */
        if(input_field_idx == _cities_button)
        {
            // TODO  SND_LeftClickSound();
            current_screen = scr_Cities_Screen;
            leave_screen_flag = ST_TRUE;
        }
        /*
            BEGIN: Game Buttons - Magic Button
        */
        if(input_field_idx == _magic_button)
        {
            // TODO  SND_LeftClickSound();
            current_screen = scr_Magic_Screen;
            leave_screen_flag = ST_TRUE;
        }
        /*
            BEGIN: Game Buttons - Armies Button
        */
        if(input_field_idx == _armies_button)
        {
            // TODO  SND_LeftClickSound();
            current_screen = scr_Armies_Screen;
            leave_screen_flag = ST_TRUE;
        }
        /*
            BEGIN: Game Buttons - Spells Button
        */
// DEMO          if(input_field_idx == _spells_button)
// DEMO          {
// DEMO              // SND_LeftClickSound();
// DEMO              if(_players[_human_player_idx].Spell_Cast == 0xD6) /* Spell of Return */
// DEMO              {
// DEMO                  // You may not throw any spells while you are banished.
// DEMO                  // GUI_NearMsgString
// DEMO                  // strcpy()
// DEMO                  // strcat()
// DEMO                  // strcat()
// DEMO                  // GUI_WarningType0()
// DEMO                  // Set_Redraw_Function(Main_Screen_Draw, 1);
// DEMO              }
// DEMO              else
// DEMO              {
// DEMO                  Set_Draw_Active_Stack_Always();
// DEMO                  Set_Unit_Draw_Priority();
// DEMO                  Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
// DEMO                  leave_screen_flag = ST_TRUE;
// DEMO                  current_screen = scr_Spellbook_Screen;
// DEMO              }
// DEMO          }

        if(input_field_idx == _patrol_button)
        {
            // TODO  SND_LeftClickSound();
            Reset_Draw_Active_Stack();
            Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, 1, 0, 0);  /* Action 1: 'Patrol' */
            WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
            Main_Screen_Reset();
            if(all_units_moved == ST_TRUE)
            {
                Set_Unit_Draw_Priority();
                Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                // TODO  IDK_screen_changed = ST_TRUE;
            }
        }

        if(input_field_idx == _special_button)
        {
            // TODO  SND_LeftClickSound();

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
                    // TODO  leave_screen_flag = ST_UNDEFINED;
                    // TODO  current_screen = scr_Road_Building;
                } break;
                case 1:  /* Settle */
                {
                    DLOG("switch(special_action_flag)  case 1:");
                    // o100p04
                    // TODO  if(Do_Build_Outpost() == ST_TRUE)
                    // TODO  {
                    // TODO      // DONT  o62p01_Empty_pFxn(_human_player_idx)
                    // TODO      Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, selected_unit_x, selected_unit_y);
                    // TODO      WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
                    // TODO      Main_Screen_Reset()
                    // TODO      // TODO  IDK_screen_changed = ST_TRUE;
                    // TODO  }
                    // TODO  Assign_Auto_Function(Main_Screen_Draw(), 1);
                } break;
                case 2:  /* Purify */
                {
                    DLOG("switch(special_action_flag)  case 2:");
                    // ABOVE  SND_LeftClickSound();
                    // DNE    Reset_Draw_Active_Stack();
                    Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, 8, 0, 0);  /* Action 8: 'Purify' */
                    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
                    Main_Screen_Reset();
                    if(all_units_moved == ST_TRUE)
                    {
                        // DNE  Set_Unit_Draw_Priority();
                        // DNE  Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                        // TODO  IDK_screen_changed = ST_TRUE;
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
                    // TODO  STK_MeldWithNode()
                    // DONT  o62p01_Empty_pFxn(_human_player_idx)
                    // TODO  Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, selected_unit_x, selected_unit_y);
                    // TODO  WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
                    // TODO  Main_Screen_Reset()
                } break;
            }
        }

        if(input_field_idx == _done_button)
        {
            // TODO  SND_LeftClickSound();
            Reset_Draw_Active_Stack();
            Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, 4, 0, 0);  /* Action 4: 'Done' */
            WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
            Main_Screen_Reset();
            if(all_units_moved == ST_TRUE)
            {
                Set_Unit_Draw_Priority();
                Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                // TODO  IDK_screen_changed = ST_TRUE;
            }
        }

        if(input_field_idx == _wait_button)
        {
            // TODO  SND_LeftClickSound();
            Reset_Draw_Active_Stack();
            Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, 5, 0, 0);  /* Action 5: 'Wait' */
            WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
            Main_Screen_Reset();
            if(all_units_moved == ST_TRUE)
            {
                Set_Unit_Draw_Priority();
                Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
                // TODO  IDK_screen_changed = ST_TRUE;
            }
        }

        /*
            BEGIN: Game Buttons - Plane Button
        */
        if(input_field_idx == _plane_button)
        {
            DLOG("(input_field_idx == _plane_button)");
            // TODO  SND_LeftClickSound();
            Do_Plane_Button(_human_player_idx, &_map_x, &_map_y, &_map_plane);
            Main_Screen_Reset();
            Reset_Map_Draw();
            MainScr_Prepare_Reduced_Map();
            // TODO  Deactivate_Auto_Function
            // TODO  Assign_Auto_Function(Main_Screen_Draw(), 1);
            Set_Mouse_List_Default();
        }
        /*
            END: Game Button - Plane Button
        */

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
                // NOTE(JimBalcomb,20230802): this here looks like what I just saw for clicking the Hero Picture on the Items Screen
                //                            so, YayNayMay Unit_Window_Picture_Coords() is just getting the coords for the grow-out pop-up effect
                // TODO  USW_FullDisplay(_unit_stack[unit_idx].unit_idx, OLft, OTop, OLft+18, OTop+18);
                // Assign_Auto_Function(Main_Screen_Draw, 1);
                // Allocate_Reduced_Map();
                // Set_Mouse_List_Normal();
                // Reset_Active_Stack_Draw();
                // UNIT_DrawPriorities();
                // STK_NoUnitDraw();
                // Set_Entities_On_Map_Window(_curr_world_x, _curr_world_y, _world_plane);
                // Reset_Map_Draw();
                // MainScr_Prepare_Reduced_Map();
                // screen_changed = ST_TRUE;
                // Clear_Help_Fields();
                // TODO  Main_Screen_Help();
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




        /*
            BEGIN: Direction Keys
        */

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
            - only if unit/stack active/selected
        */
        if((input_field_idx == _main_map_grid_field) && (_unit_stack_count != 0) )
        {
            unit_stack_hmoves = Stack_Moves_Active();  // stack movement points to use for move stack
            if(unit_stack_hmoves < 1)
            {
                DLOG("(unit_stack_hmoves < 1)");
                // j_IDK_MainScr_SUA_s553C3()
                // j_UNIT_SetGlobalPath()  // calcs OVL_Path_Length; sets OVL_StackHasPath
                if(OVL_Path_Length < 1)
                {
                    // j_IDK_MainScr_SUA_s553C3()
                }
                // IDK   Set_Mouse_List_Normal();
                Reset_Map_Draw();
                // DONT  NIU_MainScreen_local_flag == 1; // ? ST_TRUE ?
            }
            else
            {
                DLOG("(unit_stack_hmoves >= 1)");
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
                    DLOG("(all_units_moved == ST_FALSE)");
                    target_world_x = (_map_x + _main_map_grid_x) % WORLD_WIDTH;  // world_x of click
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
                    //     // Move_Stack(target_world_x, target_world_y, _human_player_idx, &_map_x, &_map_y, &_map_plane)
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
            DLOG("(-input_field_idx == _main_map_grid_field)");
            DLOG("@@RightClickMovementMap");

            // TODO  rename this variable to something more sensible - maybe add more if it gets reused - here ~entity_idx, was "input"
            entity_idx = entities_on_movement_map[( (_main_map_grid_y * MAP_WIDTH) + _main_map_grid_x )];
#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: entity_idx: %d\n", __FILE__, __LINE__, entity_idx);
#endif

            if(entity_idx == ST_UNDEFINED)
            {
                DLOG("(entity_idx == ST_UNDEFINED)");
                DLOG("ScrollTheMap");
                // _main_map_grid_x,y is now what got updated in Param3,4 way over in Interpret_Mouse_Input() |-> Push_Field_Down()
                // ¿ here, _prev_world_x, y is final destination for scrolling the map ?
                _prev_world_x = _prev_world_x + (_main_map_grid_x - (MAP_WIDTH  / 2));  // ¿ grid x - (map width / 2) = map x ?
                _prev_world_y = _prev_world_y + (_main_map_grid_y - (MAP_HEIGHT / 2));  // ¿ grid y - (map height / 2) = map y ?
// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d]: _main_map_grid_x: %d\n", __FILE__, __LINE__, _main_map_grid_x);
//         dbg_prn("DEBUG: [%s, %d]: _main_map_grid_y: %d\n", __FILE__, __LINE__, _main_map_grid_y);
//         dbg_prn("DEBUG: [%s, %d]: _main_map_grid_x - (MAP_WIDTH / 2): %d\n", __FILE__, __LINE__, _main_map_grid_x - (MAP_WIDTH / 2));
//         dbg_prn("DEBUG: [%s, %d]: _main_map_grid_y - (MAP_HEIGHT / 2): %d\n", __FILE__, __LINE__, _main_map_grid_y - (MAP_HEIGHT / 2));
//         dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
//         dbg_prn("DEBUG: [%s, %d]: _prev_world_y: %d\n", __FILE__, __LINE__, _prev_world_y);
//         dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
// #endif
                IDK_CheckSet_MapDisplay_XY();
// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
//         dbg_prn("DEBUG: [%s, %d]: _prev_world_y: %d\n", __FILE__, __LINE__, _prev_world_y);
//         dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
// #endif
            }
            else
            {
                DLOG("(entity_idx != ST_UNDEFINED)");

                // RP_SND_LeftClickSound2()

                // entity_idx = abs(entity_idx);
                entity_idx = entity_idx > 0 ? entity_idx : entity_idx*-1;
#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: entity_idx: %d\n", __FILE__, __LINE__, entity_idx);
#endif

                if(entity_idx >= 1000)
                {
                    DLOG("(entity_idx >= 1000)");

                }
                else
                {
                    DLOG("(entity_idx < 1000)");
                    if(_UNITS[entity_idx].owner_idx == _human_player_idx)
                    {
                        DLOG("(_UNITS[entity_idx].owner_idx == _human_player_idx)");
                        DLOG("@@SelectedUnitStack");
#ifdef STU_DEBUG
                        DBG_TST_Selected_Stack = 0;
                        DBG_TST_Select_Unit_Stack = 0;
                        DBG_TST_Build_Unit_Stack = 0;
                        DBG_TST_Draw_Map_Units = 0;
#endif
                        _unit = entity_idx;
                        unit_idx = _unit;
                        Unit_X = _UNITS[unit_idx].world_x;
                        Unit_Y = _UNITS[unit_idx].world_y;
                        // OVL_Map_CenterX = _UNITS[unit_idx].world_x;
                        // OVL_Map_CenterY = _UNITS[unit_idx].world_y;
                        _active_world_x = _UNITS[unit_idx].world_x;
                        _active_world_y = _UNITS[unit_idx].world_y;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: unit_idx: %d\n", __FILE__, __LINE__, unit_idx);
    dbg_prn("DEBUG: [%s, %d]: Unit_X: %d\n", __FILE__, __LINE__, Unit_X);
    dbg_prn("DEBUG: [%s, %d]: Unit_Y: %d\n", __FILE__, __LINE__, Unit_Y);
    dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
    dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
    dbg_prn("DEBUG: [%s, %d]: _map_plane: %d\n", __FILE__, __LINE__, _map_plane);

    if(_UNITS[unit_idx].world_x == 19 && _UNITS[unit_idx].world_y == 10)
    {
        DBG_TST_Selected_Stack = 1;
        DBG_TST_Select_Unit_Stack = 1;
        DBG_TST_Build_Unit_Stack = 1;
        DBG_TST_Draw_Map_Units = 1;
        dbg_prn("DEBUG: [%s, %d]: DBG_TST_Selected_Stack: %d\n", __FILE__, __LINE__, DBG_TST_Selected_Stack);
        dbg_prn("DEBUG: [%s, %d]: DBG_TST_Select_Unit_Stack: %d\n", __FILE__, __LINE__, DBG_TST_Select_Unit_Stack);
        dbg_prn("DEBUG: [%s, %d]: DBG_TST_Build_Unit_Stack: %d\n", __FILE__, __LINE__, DBG_TST_Build_Unit_Stack);
    }
#endif
                        Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, Unit_X, Unit_Y);
#ifdef STU_DEBUG
        DBG_TST_Selected_Stack = 0;
        DBG_TST_Select_Unit_Stack = 0;
        DBG_TST_Build_Unit_Stack = 0;
#endif
                        if(all_units_moved != ST_TRUE)
                        {
                            DLOG("(all_units_moved != ST_TRUE)");
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
                        DLOG("if(_UNITS[entity_idx].owner_idx != _human_player_idx)");
                        // Enemy Unit - Unit List Screen (Pop-Up)
                        // target_world_x = _main_map_grid_x * SQUARE_WIDTH;
                        // target_world_y = (_main_map_grid_y * SQUARE_HEIGHT) + SQUARE_WIDTH;
                        // TODO  Deactivate_Help_List();
                        // TODO  Unit_List_Window_Pup(input__entity_idx, 0, target_world_x, target_world_y);
                        // TODO  Assign_Auto_Function(Main_Screen_Draw, 1);
                        // Allocate_Reduced_Map();
                        // Set_Mouse_List_Default();
                        // Reset_Map_Draw();
                        // MainScr_Prepare_Reduced_Map();
                        // TODO  Deactivate_Help_List();
                        // TODO  Main_Screen_Help();
                    }

                    DLOG("@@DidRightClickUnit");
                    screen_changed = ST_TRUE;
                    // DONT  NIU_MainScreen_local_flag = ST_TRUE;
                }

            }

       }



        /*
            BEGIN: Reduced Map Grid Field
                In IDA, custom color #1 (~MoM's Lighest Purpleish (154,117,190))
        */
        // Left-Click Reduced Map Grid Field
        // Right-Click Reduced Map Grid Field

        if(abs(input_field_idx) == _minimap_grid_field)
        {
            // TODO  SND_LeftClickSound();

            Reduced_Map_Coords(&target_world_x, &target_world_y, ((_map_x + (MAP_WIDTH / 2)) % WORLD_WIDTH), (_map_y + (MAP_HEIGHT / 2)), REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT);
#ifdef STU_DEBUG
            dbg_prn("DEBUG: [%s, %d]: _minimap_grid_x: %d\n", __FILE__, __LINE__, _minimap_grid_x);
            dbg_prn("DEBUG: [%s, %d]: _minimap_grid_y: %d\n", __FILE__, __LINE__, _minimap_grid_y);
            dbg_prn("DEBUG: [%s, %d]: target_world_x: %d\n", __FILE__, __LINE__, target_world_x);
            dbg_prn("DEBUG: [%s, %d]: target_world_y: %d\n", __FILE__, __LINE__, target_world_y);
#endif
#ifdef STU_DEBUG
            dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
            dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
#endif
            _prev_world_x = _minimap_grid_x + target_world_x;
            _prev_world_y = _minimap_grid_y + target_world_y;
#ifdef STU_DEBUG
            dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
            dbg_prn("DEBUG: [%s, %d]: _prev_world_x: %d\n", __FILE__, __LINE__, _prev_world_x);
#endif
#ifdef STU_DEBUG
            dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
            dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
#endif
            _map_x = _prev_world_x;
            _map_y = _prev_world_y;
#ifdef STU_DEBUG
            dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
            dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
#endif
            Center_Map(&_map_x, &_map_y, _prev_world_x, _prev_world_y, _map_plane);
#ifdef STU_DEBUG
            dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
            dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
#endif
            // Hot-Key 'C'
                // unit_idx = _unit_stack[0].unit_idx;
                // _map_plane = _UNITS[unit_idx].world_plane;
                // Center_Map(&_map_x, &_map_y, _UNITS[unit_idx].world_x, _UNITS[unit_idx].world_y, _UNITS[unit_idx].world_plane);
                // MainScr_Prepare_Reduced_Map();
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
            // o146p04_Empty_pFxn();
            // o108p03_Empty_pFxn();
            PageFlip_FX();
            Release_Time(1);
        }
        
        screen_changed = ST_FALSE;

    }  /* while(leave_screen_flag == ST_FALSE) */

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
        _next_turn_button = Add_Hidden_Field(248, 175, 313, 199, 'N', ST_UNDEFINED);
    }

    if((_map_x == _prev_world_x) && (_map_y == _prev_world_y) )
    {
        // TODO  add defines for the map dims/coords
        // TODO  confirm what these values/calculations should end up being
        _main_map_grid_field = Add_Grid_Field(MAP_SCREEN_X, MAP_SCREEN_Y, SQUARE_WIDTH, SQUARE_HEIGHT, MAP_WIDTH, MAP_HEIGHT, &_main_map_grid_x, &_main_map_grid_y, ST_UNDEFINED);
    }

    if((_map_x == _prev_world_x) && (_map_y == _prev_world_y) )
    {
        _minimap_grid_field = Add_Grid_Field(REDUCED_MAP_SCREEN_X, REDUCED_MAP_SCREEN_Y, REDUCED_MAP_SQUARE_WIDTH, REDUCED_MAP_SQUARE_HEIGHT, REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT, &_minimap_grid_x, &_minimap_grid_y, ST_UNDEFINED);
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
        _done_button = Add_Button_Field(246, 176, "", main_done_button, 'D', ST_UNDEFINED);
        _patrol_button = Add_Button_Field(280, 176, "", main_patrol_button, '0', ST_UNDEFINED);
        _wait_button = Add_Button_Field(246, 186, "", main_wait_button, 'W', ST_UNDEFINED);
    }

    if(special_action_flag != ST_UNDEFINED)
    {
        if(special_action_flag == 2)
        {
            _special_button = Add_Button_Field(280, 186, "", main_purify_button, '0', ST_UNDEFINED);
        }
        else if(special_action_flag == 9)
        {
            _special_button = Add_Button_Field(280, 186, "", main_meld_button, '0', ST_UNDEFINED);
        }
        else
        {
            _special_button = Add_Button_Field(280, 186, "", main_build_button, 'B', ST_UNDEFINED);
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
            _wait_button = Add_Button_Field(246, 186, "", main_wait_button, 'W', ST_UNDEFINED);
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
    _game_button    = Add_Button_Field(  7, 4, "", main_game_button,   'G', ST_UNDEFINED);
    _spells_button  = Add_Button_Field( 47, 4, "", main_spells_button, 'S', ST_UNDEFINED);
    _armies_button  = Add_Button_Field( 89, 4, "", main_armies_button, 'A', ST_UNDEFINED);
    _cities_button  = Add_Button_Field(140, 4, "", main_cities_button, '0', ST_UNDEFINED);
    _magic_button   = Add_Button_Field(184, 4, "", main_magic_button,  'M', ST_UNDEFINED);
    _info_button    = Add_Button_Field(226, 4, "", main_info_button,   'I', ST_UNDEFINED);
    _plane_button   = Add_Button_Field(270, 4, "", main_plane_button,  'P', ST_UNDEFINED);
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
        Unit_Window_Fields[itr_stack] = Add_Hidden_Field(x1-1, y1-1, x2-2, y2-2, 0, ST_UNDEFINED);
    }

}


// WZD o57p08
void Main_Screen_Draw(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw()\n", __FILE__, __LINE__);
#endif

    Reset_Window();
    Set_Page_Off();

    Main_Screen_Draw_Do_Draw(&_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);

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

    Allocate_Reduced_Map();
    // TODO  Deactivate_Auto_Function();
    Clear_Fields();
    Reset_Draw_Active_Stack();
    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    Set_Mouse_List_Default();
    MainScr_Prepare_Reduced_Map();
    // TODO  Assign_Auto_Function(Main_Screen_Draw, 1);
    Reset_Map_Draw();
    // TODO  Deactivate_Help_List(0;)
    // TODO  Main_Screen_Help();
    // DONT  NIU_MainScreen_local_flag = ST_TRUE;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Reset()\n", __FILE__, __LINE__);
#endif
}

// WZD o57p10
// AKA Do_Move_Stack_DirKey()
void Move_Stack_DirKey(int16_t movement_direction)
{
    int16_t movement_points;
    int16_t move_x;
    int16_t move_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Move_Stack_DirKey(movement_direction = %d)\n", __FILE__, __LINE__, movement_direction);
#endif

    if(_unit_stack_count != 0)
    {
        DLOG("(_unit_stack_count != 0)");
        // movement_points = Stack_Moves_Active();
        // if(movement_points != 0)
        if(movement_points = Stack_Moves_Active() != 0)
        {
            DLOG("(Stack_Moves_Active() != 0))");
            if(all_units_moved == ST_FALSE)
            {
                DLOG("(all_units_moved == ST_FALSE)");

                switch(movement_direction)
                {
                    case 1:  /* LeftDown  -x,+y */
                    {
                        DLOG("switch(movement_direction)  case 1:  LeftDown");
                        move_x = ((_UNITS[_unit_stack[0].unit_idx].world_x - 1) % WORLD_WIDTH);
                        move_y = (_UNITS[_unit_stack[0].unit_idx].world_y + 1);
                    } break;
                    case 2:  /* Down  +y */
                    {
                        DLOG("switch(movement_direction)  case 2:  Down");
                        move_x = (_UNITS[_unit_stack[0].unit_idx].world_x);
                        move_y = (_UNITS[_unit_stack[0].unit_idx].world_y + 1);
                    } break;
                    case 3:  /* RightDown  +x,+y */
                    {
                        DLOG("switch(movement_direction)  case 3:  RightDown");
                        move_x = ((_UNITS[_unit_stack[0].unit_idx].world_x + 1) % WORLD_WIDTH);
                        move_y = (_UNITS[_unit_stack[0].unit_idx].world_y + 1);
                    } break;
                    case 4:  /* Left  -x */
                    {
                        DLOG("switch(movement_direction)  case 4:  Left");
                        move_x = (_UNITS[_unit_stack[0].unit_idx].world_x - 1);
                        move_y = (_UNITS[_unit_stack[0].unit_idx].world_y);
                    } break;
                    case 5:  /* DNE */
                    {
                        DLOG("switch(movement_direction)  case 5:  DNE");
                    } break;
                    case 6:  /* Right  +x */
                    {
                        DLOG("switch(movement_direction)  case 6:  Right");
                        move_x = (_UNITS[_unit_stack[0].unit_idx].world_x + 1);
                        move_y = (_UNITS[_unit_stack[0].unit_idx].world_y);
                    } break;
                    case 7:  /* LeftUp  -x,-y */
                    {
                        DLOG("switch(movement_direction)  case 7:  LeftUp");
                        move_x = ((_UNITS[_unit_stack[0].unit_idx].world_x - 1) % WORLD_WIDTH);
                        move_y = (_UNITS[_unit_stack[0].unit_idx].world_y - 1);
                    } break;
                    case 8:  /* Up  -y */
                    {
                        DLOG("switch(movement_direction)  case 8:  Up");
                        move_x = (_UNITS[_unit_stack[0].unit_idx].world_x);
                        move_y = (_UNITS[_unit_stack[0].unit_idx].world_y - 1);
                    } break;
                    case 9:  /* RightUp  +x,-y */
                    {
                        DLOG("switch(movement_direction)  case 9:  RightUp");
                        move_x = ((_UNITS[_unit_stack[0].unit_idx].world_x + 1) % WORLD_WIDTH);
                        move_y = (_UNITS[_unit_stack[0].unit_idx].world_y - 1);
                    } break;

                }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _UNITS[_unit_stack[0].unit_idx].world_x: %d\n", __FILE__, __LINE__, _UNITS[_unit_stack[0].unit_idx].world_x);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[_unit_stack[0].unit_idx].world_y: %d\n", __FILE__, __LINE__, _UNITS[_unit_stack[0].unit_idx].world_y);
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: move_x: %d\n", __FILE__, __LINE__, move_x);
    dbg_prn("DEBUG: [%s, %d]: move_y: %d\n", __FILE__, __LINE__, move_y);
#endif

                if(move_y < WORLD_Y_MIN)
                {
                    move_y = WORLD_Y_MIN;
                }
                if(move_y >= WORLD_HEIGHT)
                {
                    move_y = WORLD_Y_MAX;
                }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Stack_Has_PlanarTravel(): %d\n", __FILE__, __LINE__, Stack_Has_PlanarTravel());

    dbg_prn("DEBUG: [%s, %d]: (_UNITS[_unit_stack[0].unit_idx].world_plane == _map_plane): %d\n", __FILE__, __LINE__, (_UNITS[_unit_stack[0].unit_idx].world_plane == _map_plane));

    dbg_prn("DEBUG: [%s, %d]: (_UNITS[_unit_stack[0].unit_idx].In_Tower == ST_TRUE): %d\n", __FILE__, __LINE__, (_UNITS[_unit_stack[0].unit_idx].In_Tower == ST_TRUE));

#endif

                if(Stack_Has_PlanarTravel() == ST_TRUE)
                {
                    DLOG("(Stack_Has_PlanarTravel() == ST_TRUE)");
                    Move_Stack(move_x, move_y, _human_player_idx, &_map_x, &_map_y, &_map_plane);
                }
                else
                {
                    DLOG("(Stack_Has_PlanarTravel() != ST_TRUE)");
                    if( (_UNITS[_unit_stack[0].unit_idx].world_plane == _map_plane) || (_UNITS[_unit_stack[0].unit_idx].In_Tower == ST_TRUE) )
                    {
                        DLOG("( (_UNITS[_unit_stack[0].unit_idx].world_plane == _map_plane) || (_UNITS[_unit_stack[0].unit_idx].In_Tower == ST_TRUE) )");
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: &_map_x: %p\n", __FILE__, __LINE__, &_map_x);
    dbg_prn("DEBUG: [%s, %d]: &_map_y: %p\n", __FILE__, __LINE__, &_map_y);
    dbg_prn("DEBUG: [%s, %d]: &_map_plane: %p\n", __FILE__, __LINE__, &_map_plane);

    dbg_prn("DEBUG: [%s, %d]: _map_x: %d\n", __FILE__, __LINE__, _map_x);
    dbg_prn("DEBUG: [%s, %d]: _map_y: %d\n", __FILE__, __LINE__, _map_y);
    dbg_prn("DEBUG: [%s, %d]: _map_plane: %d\n", __FILE__, __LINE__, _map_plane);
#endif

                        Move_Stack(move_x, move_y, _human_player_idx, &_map_x, &_map_y, &_map_plane);
                    }
                    else
                    {
                        DLOG("( (_UNITS[_unit_stack[0].unit_idx].world_plane != _map_plane) && (_UNITS[_unit_stack[0].unit_idx].In_Tower != ST_TRUE) )");
                        // TODO  GUI_WarningType0(cstrWarnNoPlaneMove);
                    }
                }
            }
            else
            {
                DLOG("(all_units_moved == ST_TRUE)");
            }
        }
        else
        {
            DLOG("(Stack_Moves_Active() == 0))");
        }
    }
    else
    {
        DLOG("(_unit_stack_count == 0)");
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Move_Stack_DirKey(movement_direction = %d)\n", __FILE__, __LINE__, movement_direction);
#endif
}



/*
    WIZARDS.EXE  ovr058
*/

// WZD o58p01
void Main_Screen_Draw_Unit_Window(int16_t start_x, int16_t start_y)
{
    int16_t itr_stack;
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

    assert(_unit_stack_count >= 0);
    assert(_unit_stack_count <= 9);
    if(_unit != ST_UNDEFINED && _unit_stack_count != 0)
    {

        for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
        {
            unit_stack_unit_idx = _unit_stack[itr_stack].unit_idx;

            Unit_Window_Picture_Coords(itr_stack, &x1, &y1, &x2, &y2);
            FLIC_Draw(x1-1, y1-1, unit_backgrounds[itr_stack]);
            Draw_Unit_Picture(x1, y1, unit_stack_unit_idx, _unit_stack[itr_stack].active);
            Unit_Window_Draw_Unit_Attributes(x1, y1, unit_stack_unit_idx);
        }

        Cycle_Unit_Enchantment_Animation();
        unit_weapon_type_animation_count++;
        if(unit_weapon_type_animation_count > 3)
        {
            unit_weapon_type_animation_count = 0;
        }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Draw_Unit_Window(start_x = %d, start_y = %d)\n", __FILE__, __LINE__, start_x, start_y);
#endif

}


// WZD o58p02
void Set_Mouse_List_Default(void)
{
    Set_Mouse_List(1, mouse_list_default);
}



/*
    WIZARDS.EXE  ovr059
*/

// WZD o59p01
// WZD o59p02
// WZD o59p03
// WZD o59p04
// WZD o59p05
// WZD o59p06
// WZD o59p07
// WZD o59p08
// WZD o59p09
// WZD o59p10
// WZD o59p11
// WZD o59p12

// WZD o59p13
// drake178: STK_CheckTower()
int16_t Units_In_Tower(int16_t unit_array_count, int16_t unit_array[], int16_t map_p)
{
    int16_t units_y;
    int16_t units_x;
    int16_t tower_idx;
    int16_t units_in_tower;
    int16_t unit_idx;

    int16_t itr_towers;
    int16_t itr_units;
    int16_t return_value;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Units_In_Tower(unit_array_count = %d, &unit_array[0] = %p, map_p = %d)\n", __FILE__, __LINE__, unit_array_count, &unit_array[0], map_p);
#endif


    if(unit_array_count < 1)
    {
        return_value = ST_FALSE;
    }
    else
    {
        units_x = _UNITS[unit_array[0]].world_x;
        units_y = _UNITS[unit_array[0]].world_y;
        units_in_tower = ST_FALSE;

        for(itr_towers = 0; (itr_towers < NUM_TOWERS) && (units_in_tower == ST_FALSE); itr_towers++)
        {
            if( (_TOWERS[itr_towers].world_x == units_x) && (_TOWERS[itr_towers].world_y == units_y) )
            {
                units_in_tower = ST_TRUE;
                tower_idx = itr_towers;
            }
        }

        if( (units_in_tower == ST_TRUE) && (Check_Planar_Seal() == ST_TRUE) )
        {
            units_in_tower = ST_FALSE;
        }

        if(units_in_tower == ST_TRUE)
        {
            for(itr_units = 0; itr_units < unit_array_count; itr_units++)
            {
                unit_idx = unit_array[itr_units];
                _UNITS[unit_idx].In_Tower = ST_TRUE;
            }

            unit_idx = unit_array[0];

            _TOWERS[tower_idx].owner_idx = _UNITS[unit_idx].owner_idx;

            if(_UNITS[unit_idx].owner_idx == _human_player_idx)
            {
                TILE_Explore(_TOWERS[tower_idx].world_x, _TOWERS[tower_idx].world_y, 0);
                TILE_Explore(_TOWERS[tower_idx].world_x, _TOWERS[tower_idx].world_y, 1);
            }

        }
        else
        {
            for(itr_units = 0; itr_units < unit_array_count; itr_units++)
            {
                unit_idx = unit_array[itr_units];
                if(_UNITS[unit_idx].In_Tower == ST_TRUE)
                {
                    _UNITS[unit_idx].In_Tower = ST_FALSE;
                }
            }
        }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Units_In_Tower(unit_array_count = %d, &unit_array[0] = %p, map_p = %d)\n", __FILE__, __LINE__, unit_array_count, &unit_array[0], map_p);
#endif

    return return_value;

}

// WZD o59p14
// WZD o59p15
// WZD o59p16
// WZD o59p17
// WZD o59p18

// WZD o59p19
// AKA IDK_MaybeSwitchStackPlane_s52514()
void Do_Plane_Button(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_plane)
{
    int16_t stack_is_on_tower;
    int16_t stack_plane_shift;
// Radius= word ptr -0Eh
    int16_t unit_world_x;
    int16_t unit_world_y;
    int16_t stack_planar_travel;
    int16_t unit_stack_unit_idx;
    int16_t itr_towers;
    int16_t orig_map_plane;
    // int16_t curr_map_plane;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Do_Plane_Button(player_idx = %d, *map_x = %d, *map_y = %d, *map_plane = %d)\n", __FILE__, __LINE__, player_idx, *map_x, *map_y, *map_plane);
#endif

    // curr_map_plane = *map_plane;
    OVL_StackHasPath = 0;
    // orig_map_plane = curr_map_plane;
    orig_map_plane = *map_plane;
    // curr_map_plane = ((curr_map_plane + 1) % PLANE_COUNT_MAX);
    *map_plane = ((*map_plane + 1) % PLANE_COUNT_MAX);
    stack_planar_travel = ST_FALSE;

#ifdef STU_DEBUG
    // dbg_prn("DEBUG: [%s, %d]: curr_map_plane: %d\n", __FILE__, __LINE__, curr_map_plane);
    dbg_prn("DEBUG: [%s, %d]: *map_plane: %d\n", __FILE__, __LINE__, *map_plane);
#endif

    /*
        Active Stack && Can Travel && Sealed
    */
    if(all_units_moved == ST_FALSE)
    {
        DLOG("(all_units_moved == ST_FALSE)");
        if(_unit_stack_count > 0)
        {
            DLOG("(_unit_stack_count > 0)");
            stack_planar_travel = Stack_Has_PlanarTravel();
            if(stack_planar_travel == ST_TRUE)
            {
                DLOG("(stack_planar_travel == ST_TRUE)");
                if(Check_Planar_Seal() == ST_TRUE)
                {
                    DLOG("(Check_Planar_Seal() == ST_TRUE)");
                    // TODO GUI_WarningType0(aPlanarSealPrev);  // "Planar Seal prevents your units from changing planes."
                    // curr_map_plane = orig_map_plane;
                    *map_plane = orig_map_plane;
                    stack_planar_travel = 99;  // ¿ `99` means 'Yay Planar Trav, Yay Planar Seal' ?
                }
                else
                {
                    DLOG("(Check_Planar_Seal() != ST_TRUE)");
                }
            }
            else
            {
                DLOG("(stack_planar_travel != ST_TRUE)");
            }
        }
        else
        {
            DLOG("(_unit_stack_count <= 0)");
        }
    }
    else
    {
        DLOG("(all_units_moved != ST_FALSE)");
    }
    /*
        HERE: stack_planar_travel = {-1, 0, 1, 99}
        -1: default/undefined
         0: Stack_Has_PlanarTravel() == ST_FALSE && _unit_stack_count > 0 && all_units_moved == ST_FALSE
         1: Stack_Has_PlanarTravel() == ST_TRUE && Check_Planar_Seal() == ST_FALSE && _unit_stack_count > 0 && all_units_moved == ST_FALSE
        99: Stack_Has_PlanarTravel() == ST_TRUE && Check_Planar_Seal() == ST_TRUE && _unit_stack_count > 0 && all_units_moved == ST_FALSE
    */
#ifdef STU_DEBUG
    // dbg_prn("DEBUG: [%s, %d]: curr_map_plane: %d\n", __FILE__, __LINE__, curr_map_plane);
    dbg_prn("DEBUG: [%s, %d]: *map_plane: %d\n", __FILE__, __LINE__, *map_plane);
#endif

    /*
        BEGIN: Yay PlanarTravel, Nay PlanarSeal
    */
    if(stack_planar_travel == ST_TRUE)
    {
        DLOG("(stack_planar_travel == ST_TRUE)");
        // stack_plane_shift = Check_Stack_Plane_Shift(_unit_stack[0].unit_idx, curr_map_plane);
        stack_plane_shift = Check_Stack_Plane_Shift(_unit_stack[0].unit_idx, *map_plane);

        if(stack_plane_shift == -2)
        {
            DLOG("(stack_plane_shift == -2)");
            // curr_map_plane = orig_map_plane;
            *map_plane = orig_map_plane;
            // TODO  GUI_WarningType0(aTheSelectedUni); // "The selected units cannot Planar Travel at this location."
        }
        else
        {
            DLOG("(stack_plane_shift != -2)");
        }
        if(stack_plane_shift == -1)
        {
            DLOG("(stack_plane_shift == -1)");

        }
        else
        {
            DLOG("(stack_plane_shift != -1)");
        }

        Reset_Map_Draw();
    }
    /*
        END: Yay PlanarTravel, Nay PlanarSeal
    */
    /*
        BEGIN: Nay Active Stack OR Nay PlanarTravel OR Yay PlanarTravel, Yay PlanarSeal
    */
    else
    {
        DLOG("(stack_planar_travel != ST_TRUE)");
        // HERE: Yay PlanarTravel, Yay PlanarSeal falls through to '@@Done'
        if(stack_planar_travel != 99)
        {
            // HERE: Nay Active Stack OR Nay PlanarTravel NOT Yay PlanarTravel, Yay PlanarSeal
            // if(curr_map_plane == orig_map_plane)  /* ¿ ~== stack_planar_travel = 99 ? ¿ couln't possibly, but would only happens if Check_Stack_Plane_Shift() returned -2 ? */
            if(*map_plane == orig_map_plane)
            {
                // curr_map_plane = ((curr_map_plane + 1) % PLANE_COUNT_MAX);
                *map_plane = ((*map_plane + 1) % PLANE_COUNT_MAX);
                _unit_stack_count = 0;
            }

            if(_unit_stack_count > 0)
            {
                // HERE: Yay Active Stack, Nay Astral Gate, Nay All Unit Have Planar Travel
                // THEN: Reset Plane, Red Notify
                if(Check_Planar_Seal() == ST_TRUE)
                {
                    stack_is_on_tower = ST_FALSE;
                    unit_stack_unit_idx = _unit_stack[0].unit_idx;
                    unit_world_x = _UNITS[unit_stack_unit_idx].world_x;
                    unit_world_y = _UNITS[unit_stack_unit_idx].world_y;
                    for(itr_towers = 0; itr_towers < TOWER_COUNT_MAX && stack_is_on_tower == ST_FALSE; itr_towers++)
                    {
                        if(_TOWERS[itr_towers].world_x == unit_world_x && _TOWERS[itr_towers].world_y == unit_world_y)
                        {
                            stack_is_on_tower = ST_TRUE;
                        }
                    }
                    // HERE: Yay/Nay Active Stack is on/inside a Tower of Wizardy
                    if(stack_is_on_tower == ST_TRUE)
                    {
                        // curr_map_plane = orig_map_plane;
                        *map_plane = orig_map_plane;
                        // TODO  GUI_WarningType0(aPlanarSealPrev);  // "Planar Seal prevents your units from changing planes."
                    }
                }
            }
        }
    }
    /*
        END: Nay PlanarTravel OR Yay PlanarTravel, Yay PlanarSeal
    */

#ifdef STU_DEBUG
    // dbg_prn("DEBUG: [%s, %d]: curr_map_plane: %d\n", __FILE__, __LINE__, curr_map_plane);
    dbg_prn("DEBUG: [%s, %d]: *map_plane: %d\n", __FILE__, __LINE__, *map_plane);
#endif

// @@Done:
    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    // Set_Entities_On_Map_Window(*map_x, *map_y, curr_map_plane);
    Set_Entities_On_Map_Window(*map_x, *map_y, *map_plane);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Do_Plane_Button(player_idx = %d, *map_x = %d, *map_y = %d, *map_plane = %d)\n", __FILE__, __LINE__, player_idx, *map_x, *map_y, *map_plane);
#endif

}

// WZD o59p20
// AKA IDK_PlanarTravel_TestWaterCity_s52774()
/*
    returns {-2, -1, ¿ ... ?}
    -2: terrain is ocean||shore||river && 
*/
int16_t Check_Stack_Plane_Shift(int16_t unit_stack_unit_idx, int16_t map_plane)
{
    int16_t movement_mode_flags[6];
    int16_t stack_array[9];
    int16_t city_has_spellward;
    // int16_t laird_idx__city_idx;
    int16_t lair_idx;
    int16_t city_idx;
    int16_t unit_world_x;
    int16_t unit_world_y;
    int16_t stack_size;

    int16_t unit_idx;
    int16_t _DI_return_value;


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Check_Stack_Plane_Shift(unit_stack_unit_idx = %d, map_plane = %d)\n", __FILE__, __LINE__, unit_stack_unit_idx, map_plane);
#endif

    unit_idx = unit_stack_unit_idx;

    _DI_return_value = -1;  // ¿ ST_SUCCESS ?

    Active_Stack_Movement_Modes(&movement_mode_flags[0]);

    if(Terrain_Is_Sailable(_UNITS[unit_idx].world_x, _UNITS[unit_idx].world_y, map_plane) == ST_TRUE)
    {
        _DI_return_value = -2;  // ¿ -2 ~== Non-Walking Terrain ?
        if(movement_mode_flags[MM_SWIMMING] == ST_TRUE || movement_mode_flags[MM_SAILING] == ST_TRUE || movement_mode_flags[MM_FLYING] == ST_TRUE)
        {
            _DI_return_value = -1;  // ¿ ST_SUCCESS ?
        }
    }

    if(_DI_return_value == -1)
    {
        unit_world_x = _UNITS[unit_stack_unit_idx].world_x;
        unit_world_y = _UNITS[unit_stack_unit_idx].world_y;
        lair_idx = Square_Has_Lair(unit_world_x, unit_world_y, map_plane);
        if(lair_idx == -1)
        {
            city_idx = Square_Has_City(unit_world_x, unit_world_y, map_plane);
            if(city_idx == -1)
            {
                stack_size = 0;
                // drake178: fills the return variables with the count and indices of all units on the selected tile that do not belong to the specified player
                // TODO  TILE_GetEnemyStack(unit_world_x, unit_world_y, map_plane, _UNITS[unit_idx].owner_idx, &stack_size, &stack_array[0]);
                if(stack_size > 0)
                {
                    _DI_return_value = -2;
                }
                else
                {
                    stack_size = 0;
                    // drake178: fills the return variables with the count and indices of all units on the selected tile that belong to the specified player
                    // TODO  TILE_GetUnitStack(_UNITS[unit_idx].world_x, _UNITS[unit_idx].world_y, map_plane, _UNITS[unit_idx].owner_idx, &stack_size, &stack_array[0]);
                    stack_size = stack_size + _unit_stack_count;
                    if(stack_size > 9)
                    {
                        _DI_return_value = -2;
                    }
                }
            }
            else
            {
                city_has_spellward = RP_CTY_CheckSpellWard__STUB(city_idx, &stack_size, &stack_array[0]);
                if(city_has_spellward == ST_TRUE)
                {
                    _DI_return_value = -2;
                }
                else
                {
                    if(_CITIES[city_idx].owner_idx != _UNITS[unit_idx].owner_idx)
                    {
                        _DI_return_value = -2;
                    }
                    else
                    {
                        // drake178: fills the return variables with the count and indices of all units on the selected tile that belong to the specified player
                        // TODO  TILE_GetUnitStack(unit_world_x, unit_world_y, map_plane, _UNITS[unit_idx].owner_idx, &stack_size, &stack_array[0]);
                        stack_size = stack_size + _unit_stack_count;

                        if(stack_size > 9)
                        {
                            _DI_return_value = -2;
                        }
                    }
                }
            }
        }
    }



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Check_Stack_Plane_Shift(unit_stack_unit_idx = %d, map_plane = %d)\n", __FILE__, __LINE__, unit_stack_unit_idx, map_plane);
#endif
    return _DI_return_value;
}



/*
    WIZARDS.EXE  ovr061
*/

// WZD o61p01
// IDK_ActiveUnitStack_MovesOrPath_s53150()

// WZD o61p02
// AKA Do_Move_Stack()
int16_t Move_Stack(int16_t move_x, int16_t move_y, int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p)
{
    int16_t unit_array[9];
    int16_t unit_array_count;
    int16_t unit_p;
    int16_t unit_y;
    int16_t unit_x;
    int16_t movement_points_available;
    int16_t Spec;
    int16_t unit_idx;

    int16_t itr_units;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Move_Stack(move_x = %d, move_y = %d, player_idx = %d, *map_x = %d, *map_y = %d, *map_p = %d)\n", __FILE__, __LINE__, move_x, move_y, player_idx, *map_x, *map_y, *map_p);
#endif

    Spec = 0;

    if(move_y != WORLD_Y_MIN && move_y != WORLD_Y_MAX)
    {
        Build_Moveable_Stack(&unit_array_count, &unit_array[0]);

        _unit = unit_array[0];

// DONT          if(OVL_MapVar3 == 1)
// DONT          {
// DONT              OVL_MapVar3 = 0;
// DONT              CRP_OVL_Obstacle_Var1 = 0;
// DONT          }
// DONT          if(CRP_OVL_Obstacle_Var1 == 0)
// DONT          {
// DONT              o58p03_Empty_pFxn();
// DONT          }


        Move_Units(player_idx, move_x, move_y, Spec, map_x, map_y, *map_p, unit_array_count, &unit_array[0]);
// ; moves, or starts to move, an overland stack towards a target destination
// ; returns 1 if the stack has moved, 0 if out of moves or no valid path
// ; complicated, BUG-ridden, progress in here, RE-EXPLORE


        unit_idx = _unit;
        unit_x = _UNITS[unit_idx].world_x;
        unit_y = _UNITS[unit_idx].world_y;
        unit_p = _UNITS[unit_idx].world_plane;


        // DONT  o62p01_Empty_pFxn(player_idx);
        movement_points_available = Stack_Moves();


        // DONT  o62p01_Empty_pFxn(player_idx);
        Select_Unit_Stack(player_idx, map_x, map_y, *map_p, unit_x, unit_y);
        movement_points_available = Stack_Moves();


        if(movement_points_available < 1)
        {
            DLOG("(movement_points_available < 1)");
            for(itr_units = 0; itr_units < unit_array_count; itr_units++)
            {
                if( (_UNITS[unit_array[itr_units]].Status & 0x00) == 0)  /* US_Ready  "NO ORDERS" */
                {
                    _UNITS[unit_array[itr_units]].Finished = ST_TRUE;
                    _UNITS[unit_array[itr_units]].Status = 0x04;  /* US_ReachDest  "DONE" */
                }
            }

            WIZ_NextIdleStack(player_idx, map_x, map_y, map_p);
        }
        else
        {
            DLOG("(movement_points_available >= 1)");
        }

        Set_Unit_Draw_Priority();
        Reset_Stack_Draw_Priority();
        Set_Entities_On_Map_Window(*map_x, *map_y, *map_p);
        // DONT  fxnptr_o59p();
        Reset_Draw_Active_Stack();
    }

// IDGI    else
// IDGI    {
// IDGI        return ST_FALSE;
// IDGI    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Move_Stack(move_x = %d, move_y = %d, player_idx = %d, *map_x = %d, *map_y = %d, *map_p = %d)\n", __FILE__, __LINE__, move_x, move_y, player_idx, *map_x, *map_y, *map_p);
#endif

    return ST_TRUE;
}

// WZD o61p03
// UNIT_MoveStack()

// WZD o61p04
void WIZ_NextIdleStack(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_plane)
{
    // Unused_Local= word ptr -10h
    int16_t Dest_Y;
    int16_t Dest_X;
    int16_t Current_Y;
    int16_t Current_X;
    int16_t AllUnitsMoved;
    int16_t Finished;
    int16_t Current_Unit;


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WIZ_NextIdleStack(player_idx = %d, *map_x = %d, *map_y = %d, *map_plane = %d)\n", __FILE__, __LINE__, player_idx, *map_x, *map_y, *map_plane);
#endif

    // Unused_Local = 0;
    AllUnitsMoved = ST_FALSE;
    Finished = ST_FALSE;

    Reset_Map_Draw();

    // TODO  o62p01_Empty_pFxn(player_idx);

    while(Finished == ST_FALSE)
    {
        // CRP_OverlandVar_3 = 0;

        AllUnitsMoved = WIZ_NextUnit(player_idx, map_plane);

        if(AllUnitsMoved == ST_TRUE)
        {
            DLOG("(AllUnitsMoved == ST_TRUE)");
            all_units_moved = ST_TRUE;
            OVL_StackHasPath = ST_FALSE;
            Finished = ST_TRUE;
        }
        else
        {
            DLOG("(AllUnitsMoved != ST_TRUE)");
            Current_Unit = _unit;
            Current_X = _UNITS[Current_Unit].world_x;
            Current_Y = _UNITS[Current_Unit].world_y;
            Select_Unit_Stack(player_idx, map_x, map_y, *map_plane, Current_X, Current_Y);
        }


        if(Finished == ST_FALSE)
        {
            Finished = ST_TRUE;

            if( (_UNITS[_unit].Status & 0x03) != 0)  /* "GOTO"  US_GoingTo */
            {
                Dest_X = _UNITS[_unit].dst_wx;
                Dest_Y = _UNITS[_unit].dst_wy;

                // CRP_OverlandVar_3 = ST_TRUE;

                if(
                    (_UNITS[_unit].world_x != Dest_X) ||
                    (_UNITS[_unit].world_y != Dest_Y) ||
                    (_UNITS[_unit].Rd_Constr_Left != ST_UNDEFINED)
                )
                {
                    Allocate_Reduced_Map();
                    // TODO  UNIT_MoveStack(player_idx, _unit, Dest_X, Dest_Y, map_x, map_y, *map_plane);
                    Allocate_Reduced_Map();
                }
                else
                {
                    _UNITS[_unit].Status = 0x00;  /* US_Ready */
                }

                Finished = ST_FALSE;

            }
        }

    }



    if(all_units_moved == ST_FALSE)
    {
        DLOG("(all_units_moved == ST_FALSE)");
        Current_Unit = _unit;
        Current_X = _UNITS[Current_Unit].world_x;
        Current_Y = _UNITS[Current_Unit].world_y;
        Select_Unit_Stack(player_idx, map_x, map_y, *map_plane, Current_X, Current_Y);
    }
    else
    {
        DLOG("(all_units_moved == ST_TRUE)");
        _unit_stack_count = 0;
        Set_Draw_Active_Stack_Always();
        Set_Entities_On_Map_Window(*map_x, *map_y, *map_plane);
    }


    // OVL_MapVar3 = ST_TRUE;


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: WIZ_NextIdleStack(player_idx = %d, *map_x = %d, *map_y = %d, *map_plane = %d)\n", __FILE__, __LINE__, player_idx, *map_x, *map_y, *map_plane);
#endif

}



// WZD o61p05
int16_t WIZ_NextUnit(int16_t player_idx, int16_t * map_plane)
{
    int16_t Unused_Local;
    int16_t tried_other_plane;
    int16_t itr_wait_units;
    int16_t Closest_Active_Unit;
    int16_t Closest_Active_Dist;
    int16_t Closest_Waiting_Unit;
    int16_t Closest_Waiting_Dist;
    int16_t itr_units;
    int16_t Distance_From_Center;
    int16_t Return_Value;
    int16_t Finished;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WIZ_NextUnit(player_idx = %d, *map_plane = %d)\n", __FILE__, __LINE__, player_idx, *map_plane);
#endif

    Closest_Waiting_Dist = 1000;
    Closest_Waiting_Unit = ST_UNDEFINED;
    Closest_Active_Dist = 1000;
    Closest_Active_Unit = ST_UNDEFINED;

    tried_other_plane = ST_FALSE;

    Unused_Local = ST_UNDEFINED;
    Return_Value = ST_FALSE;

    Finished = ST_FALSE;
    itr_units = 0;

    while(Finished == ST_FALSE)
    {

        if(
            (_UNITS[itr_units].owner_idx == player_idx) &&
            ( (_UNITS[itr_units].world_plane == *map_plane) || (_UNITS[itr_units].In_Tower == ST_TRUE) ) &&
            (_UNITS[itr_units].owner_idx != ST_UNDEFINED) &&
            (_UNITS[itr_units].Finished == ST_FALSE)
        )
        {
            Distance_From_Center = Delta_XY_With_Wrap(_active_world_x, _active_world_y, _UNITS[itr_units].world_x, _UNITS[itr_units].world_y, WORLD_WIDTH);

            if( (_UNITS[itr_units].Status & 0x05) != 0)  /* US_Wait */
            {
                if(Closest_Waiting_Dist > Distance_From_Center)
                {
                    Closest_Waiting_Dist = Distance_From_Center;
                    Closest_Waiting_Unit = itr_units;
                }
            }
            else
            {
                if( ((_UNITS[itr_units].Status & 0x08) == 0) /* US_Purify */ && ((_UNITS[itr_units].Status & 0x64) == 0) /* US_Unkown_100 */ )
                {
                    if(Closest_Waiting_Dist > Distance_From_Center)
                    {
                        Closest_Active_Dist = Distance_From_Center;
                        Closest_Active_Unit = itr_units;
                    }
                }
            }
        }

        itr_units++;
        if(itr_units == _units)
        {
            if(Closest_Active_Unit != ST_UNDEFINED)
            {
                Finished = ST_TRUE;
                if(*map_plane == 2)
                {
                    *map_plane = 0;
                }
                _unit = Closest_Active_Unit;
                _active_world_x = _UNITS[_unit].world_x;
                _active_world_y = _UNITS[_unit].world_y;
            }
            else if(Closest_Waiting_Unit != ST_UNDEFINED)
            {
                if(*map_plane == 2)
                {
                    *map_plane = 0;
                }
                _unit = Closest_Active_Unit;
                Finished = ST_TRUE;
                _active_world_x = _UNITS[_unit].world_x;
                _active_world_y = _UNITS[_unit].world_y;
                for(itr_wait_units = 0; itr_wait_units < _units; itr_wait_units++)
                {
                    if(_UNITS[itr_wait_units].owner_idx == player_idx)
                    {
                        if( (_UNITS[itr_wait_units].Status & 0x05) != 0)
                        {
                            _UNITS[itr_wait_units].Status = 0x00;  /* US_Ready */
                        }
                    }
                }
            }
            else if(tried_other_plane != ST_TRUE)
            {
                tried_other_plane = ST_TRUE;
                *map_plane = ((*map_plane + 1) % 2);
                Closest_Waiting_Dist = 1000;
                Closest_Waiting_Unit = ST_UNDEFINED;
                Closest_Active_Dist = 1000;
                Closest_Active_Unit = ST_UNDEFINED;
                Unused_Local = ST_UNDEFINED;
                itr_units = 0;
            }
            else
            {
                Finished = ST_TRUE;
                Return_Value = ST_TRUE;
            }
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: WIZ_NextUnit(player_idx = %d, map_plane = %d)\n", __FILE__, __LINE__, player_idx, map_plane);
#endif

    return Return_Value;
}

// WZD o61p06
// STK_GetExtraActions()

// WZD o61p07
// STK_GetMovableUnits()

// WZD o61p08
/*

*/
void Build_Moveable_Stack(int16_t * unit_count, int16_t unit_array[])
{
    int16_t Stack_Array[9];
    int16_t LandLubber_Array[9];
    int16_t transport_load;
    int16_t LandLubber_Array_Index;
    int16_t transport_capacity;
    int16_t LandLubber_Count;
    int16_t stack_has_transport;
    int16_t IDK_unit_array_cnt;
    int16_t IDK_Stack_Array_cnt;

    int16_t itr_stack;
    int16_t itr_LL_Array;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Build_Moveable_Stack()\n", __FILE__, __LINE__);
#endif

    stack_has_transport = ST_FALSE;
    transport_capacity = 0;

    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        if(_unit_stack[itr_stack].active == ST_TRUE)
        {
            if(_UNITS[_unit_stack[itr_stack].unit_idx].Status != 0x03) /* US_GoingTo */
            {
                if(_unit_type_table[_UNITS[_unit_stack[itr_stack].unit_idx].type].Transport > 0)
                {
                    stack_has_transport = ST_TRUE;
                    transport_capacity = _unit_type_table[_UNITS[_unit_stack[itr_stack].unit_idx].type].Transport;
                }
            }
        }
    }

    if(stack_has_transport == ST_TRUE)
    {
        IDK_Stack_Array_cnt = 0;
        IDK_unit_array_cnt = 0;

        for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
        {
            if(_unit_stack[itr_stack].active == ST_TRUE)
            {
                if(_UNITS[_unit_stack[itr_stack].unit_idx].Status != 0x03) /* US_GoingTo */
                {
                    if(_unit_type_table[_UNITS[_unit_stack[itr_stack].unit_idx].type].Transport > 0)
                    {
                        unit_array[IDK_unit_array_cnt] = _unit_stack[itr_stack].unit_idx;
                        IDK_unit_array_cnt++;
                    }
                }
            }
            Stack_Array[IDK_Stack_Array_cnt] = _unit_stack[itr_stack].unit_idx;
            IDK_Stack_Array_cnt++;
        }

        IDK_Stack_Array_cnt = IDK_unit_array_cnt;
        
        if(Square_Has_City(_UNITS[_unit_stack[0].unit_idx].world_x, _UNITS[_unit_stack[0].unit_idx].world_y, _UNITS[_unit_stack[0].unit_idx].world_plane) == ST_UNDEFINED)
        {
            for(itr_LL_Array = 0; itr_LL_Array < 9; itr_LL_Array++)
            {
                LandLubber_Array[itr_LL_Array] = ST_UNDEFINED;
            }

            LandLubber_Count = STK_GetLandlubbers(_unit_stack_count, &Stack_Array[0], &LandLubber_Array[0]);

            if(LandLubber_Count > 0)
            {
                IDK_Stack_Array_cnt = IDK_unit_array_cnt;
                LandLubber_Array_Index = 0;
                transport_load = 0;

                for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
                {
                    if(LandLubber_Array[LandLubber_Array_Index] == _unit_stack[itr_stack].unit_idx)
                    {
                        if(
                            (transport_load < transport_capacity) ||
                            (_UNITS[_unit_stack[itr_stack].unit_idx].type <= 34)  /* Hero Unit */
                        )
                        {
                            unit_array[IDK_Stack_Array_cnt] = _unit_stack[itr_stack].unit_idx;
                            IDK_Stack_Array_cnt++;
                            LandLubber_Array_Index++;
                            if( _UNITS[_unit_stack[itr_stack].unit_idx].type > 34)  /* Hero Unit */
                            {
                                transport_load++;
                            }
                        }
                    }
                }

                for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
                {
                    if(
                        (Unit_Has_AirTravel_Item(_unit_stack[itr_stack].unit_idx) == ST_FALSE) &&
                        (Unit_Has_WaterTravel_Item(_unit_stack[itr_stack].unit_idx) == ST_FALSE) &&
                        (Unit_Has_Swimming(_unit_stack[itr_stack].unit_idx) == ST_FALSE) &&
                        (Unit_Has_WindWalking(_unit_stack[itr_stack].unit_idx) == ST_FALSE) &&
                        (Unit_Has_AirTravel(_unit_stack[itr_stack].unit_idx) == ST_FALSE)
                    )
                    {
                        if( (_UNITS[_unit_stack[itr_stack].unit_idx].Status & 0x03 /* US_GoingTo */) == 0)
                        {
                            unit_array[IDK_Stack_Array_cnt] = _unit_stack[itr_stack].unit_idx;
                            IDK_Stack_Array_cnt++;
                        }
                    }
                }

                goto Done;

            }
            else  /* !(LandLubber_Count > 0) */
            {
                // HERE: Yay TransPort, Nay City, Nay LandLubbers
                if(IDK_Stack_Array_cnt < _unit_stack_count)
                {
                    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
                    {
                        if( ((_UNITS[itr_stack].Status & 0x03 /* US_GoingTo */) != 0) &&
                            (_unit_type_table[_UNITS[itr_stack].type].Transport < 1)
                        )
                        {
                            unit_array[IDK_Stack_Array_cnt] = _unit_stack[itr_stack].unit_idx;
                            IDK_Stack_Array_cnt++;
                        }
                    }
                }
                else
                {
                    // HERE: ¿ we're on the ocean and all stack units are transports ?
                    goto Done;
                }


            }

        }
        else  /* Square_Has_City() != ST_UNDEFINED */
        {
            goto Done;
        }

    }
    else  /* (stack_has_transport != ST_TRUE) */
    {
        IDK_Stack_Array_cnt = 0;

        for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
        {
            if( 
                (_unit_stack[itr_stack].active == ST_TRUE) &&
                ( ((_UNITS[_unit_stack[itr_stack].unit_idx].Status & 0x03) == 0) /* US_GoingTo */)
            )
            {
                unit_array[IDK_Stack_Array_cnt] = _unit_stack[itr_stack].unit_idx;
                IDK_Stack_Array_cnt++;
            }
        }

    }

Done:
    *unit_count = IDK_Stack_Array_cnt;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Build_Moveable_Stack()\n", __FILE__, __LINE__);
#endif

}



/*
    WIZARDS.EXE  ovr062
*/

// WZD o62p01
// DONT  int16_t o62p01_Empty_pFxn(int16_t player_idx)

// WZD o62p02
/*
//drake178
builds a stack from the units on the tile, selects
the one with the highest draw priority that can move
as the current unit, brings the stack into view, sets
In_Tower flags, calculates GUI_ExtraUnitAction and,
if all the units belong to the human player and have
a destination already set, loads their path into the
global arrays, setting GUI_StackHasPath to 1
*/
// MoO2  Module: SHIPSTK  Find_Ship_Stacks_()
// _NUM_SHIPS, _ship_stack_count, _PLAYER_NUM, _ship, _ship_stack_start, _ship_node
// Remove_Non_Detected_Ships_(), Sort_Ships_In_Stack_()
// AKA OVL_StackSelect()
void Select_Unit_Stack(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t unit_x, int16_t unit_y)
{
    int16_t GoingTo_Unit_idx;
    int16_t itr_stack;
    int16_t unit_idx;
    int16_t unit_list[9];
    int16_t unit_count;
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Select_Unit_Stack(player_idx = %d, *map_x = %d, *map_y = %d, map_plane = %d, unit_x = %d, unit_y = %d)\n", __FILE__, __LINE__, player_idx, *map_x, *map_y, map_plane, unit_x, unit_y);
#endif

    // G_OVL_MapVar4 = 1;

#ifdef STU_DEBUG
    if(DBG_TST_Select_Unit_Stack == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: (DBG_TST_Select_Unit_Stack == 1)\n", __FILE__, __LINE__);
    }
#endif

    Build_Unit_Stack(player_idx, map_plane, unit_x, unit_y);

    if(_unit_stack_count != 0)
    {
        Sort_Unit_Stack();

        OVL_StackHasPath = ST_FALSE;

        GoingTo_Unit_idx = ST_UNDEFINED;

        if(player_idx == _human_player_idx)
        {

            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                unit_idx = _unit_stack[itr_stack].unit_idx;
                if(_UNITS[unit_idx].Status == 3) /* US_GoingTo */
                {
                    GoingTo_Unit_idx = unit_idx;
                }
                else
                {
                    GoingTo_Unit_idx = ST_UNDEFINED;
                    break;
                }
            }
        }

        if(GoingTo_Unit_idx != ST_UNDEFINED)
        {
            // TODO  UNIT_SetGlobalPath(GoingTo_Unit_idx);
        }

        unit_idx = _unit;
        unit_x = _UNITS[unit_idx].world_x;
        unit_y = _UNITS[unit_idx].world_y;
        OVL_BringIntoView(map_x, map_y, unit_x, unit_y, map_plane);
        
        // TODO  STK_GetExtraActions();
        // TODO  STK_GetMovableUnits(*unit_count, *unit_list);
        // DONT  RP_STK_ReturnZero(unit_count, *unit_list);
        // TODO  STK_CheckTower(unit_count, *unit_list);

        if(_unit_stack_count < 1)
        {
            OVL_StackHasPath = ST_FALSE;
        }

    }
    // ¿ else { return 0; } ?

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Select_Unit_Stack(player_idx = %d, *map_x = %d, *map_y = %d, map_plane = %d, unit_x = %d, unit_y = %d)\n", __FILE__, __LINE__, player_idx, *map_x, *map_y, map_plane, unit_x, unit_y);
#endif
}

// WZD o62p03
// drake178: STK_DisplaySort
// OON XREF: Select_Unit_Stack()
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
void Build_Unit_Stack(int16_t player_idx, int16_t world_plane, int16_t world_x, int16_t world_y)
{
    int16_t itr_unit_stack;
    int16_t itr_units;
    int16_t current_unit_idx;

    int16_t unit_stack_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Build_Unit_Stack()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    // if(DBG_TST_Selected_Stack == 1)
    if(DBG_TST_Build_Unit_Stack == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: (DBG_TST_Build_Unit_Stack == 1)\n", __FILE__, __LINE__);
    }
#endif

    unit_stack_idx = 0;

    for(itr_unit_stack = 0; itr_unit_stack < 9; itr_unit_stack++)
    {
        _unit_stack[itr_unit_stack].active = ST_FALSE;
    }

#ifdef STU_DEBUG
    // if(DBG_TST_Selected_Stack == 1)
    if(DBG_TST_Build_Unit_Stack == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: _units: %d\n", __FILE__, __LINE__, _units);
    }
#endif

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        current_unit_idx = itr_units;

#ifdef STU_DEBUG
    // if(DBG_TST_Selected_Stack == 1)
    if(DBG_TST_Build_Unit_Stack == 1)
    {
        //dbg_prn("DEBUG: [%s, %d]: %d, %d, %d, %d, %d\n", __FILE__, __LINE__,
        //    _UNITS[current_unit_idx].owner_idx,
        //    _UNITS[current_unit_idx].world_x,
        //    _UNITS[current_unit_idx].world_y,
        //    _UNITS[current_unit_idx].world_plane,
        //    _UNITS[current_unit_idx].In_Tower
        // );

        //dbg_prn("DEBUG: [%s, %d]: %d, %d, %d, %d, %d\n", __FILE__, __LINE__,
        //    (_UNITS[current_unit_idx].owner_idx != player_idx),
        //    (_UNITS[current_unit_idx].world_x != world_x),
        //    (_UNITS[current_unit_idx].world_y != world_y),
        //    (_UNITS[current_unit_idx].owner_idx == ST_UNDEFINED),
        //    ( (_UNITS[current_unit_idx].world_plane != world_plane) && (_UNITS[current_unit_idx].In_Tower != ST_TRUE) )
        //);
    }
#endif

        if((_UNITS[current_unit_idx].owner_idx == player_idx) && 
            (_UNITS[current_unit_idx].world_x == world_x) &&
            (_UNITS[current_unit_idx].world_y == world_y) &&
            (_UNITS[current_unit_idx].owner_idx != ST_UNDEFINED) && 
            ( (_UNITS[current_unit_idx].world_plane == world_plane) || (_UNITS[current_unit_idx].In_Tower != ST_TRUE) )
        )
        {
            /*
                Add Unit to Stack
            */
            DLOG("@@AddUnitToStack");
            _unit_stack[unit_stack_idx].unit_idx = current_unit_idx;

            // ? inactive: no moves, finished, or purify ?
            // ? what about patrol ?
#ifdef STU_DEBUG
    // if(DBG_TST_Selected_Stack == 1)
    if(DBG_TST_Build_Unit_Stack == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].HMoves:  %d\n", __FILE__, __LINE__, current_unit_idx, _UNITS[current_unit_idx].HMoves);
        dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].Finished:    %d\n", __FILE__, __LINE__, current_unit_idx, _UNITS[current_unit_idx].Finished);
        dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].Status:    %d\n", __FILE__, __LINE__, current_unit_idx, _UNITS[current_unit_idx].Status);
    }
#endif
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
                _unit_stack[unit_stack_idx].active = ST_FALSE;
            }

            unit_stack_idx++;
        }

    }

    _unit_stack_count = unit_stack_idx;

#ifdef STU_DEBUG
    // if(DBG_TST_Selected_Stack == 1)
    if(DBG_TST_Build_Unit_Stack == 1)
    {
        for(itr_unit_stack = 0; itr_unit_stack < 9; itr_unit_stack++)
        {
            dbg_prn("DEBUG: [%s, %d]: _unit_stack[%d].unit_idx:  %d\n", __FILE__, __LINE__, itr_unit_stack, _unit_stack[itr_unit_stack].unit_idx);
            dbg_prn("DEBUG: [%s, %d]: _unit_stack[%d].active:    %d\n", __FILE__, __LINE__, itr_unit_stack, _unit_stack[itr_unit_stack].active);
        }
    }
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Build_Unit_Stack()\n", __FILE__, __LINE__);
#endif
}

// WZD o62p05
// Select_Unit_Stack(... *map_x, *map_y, map_plane ...) |-> OVL_BringIntoView(*map_x, *map_y, unit_x, unit_y, map_plane);
// MoO2  ¿ Star_On_Screen_(); Ship_Completely_On_Screen_(); Ship_On_Screen_(); ?
void OVL_BringIntoView(int16_t *map_x, int16_t *map_y, int16_t unit_x, int16_t unit_y, int16_t map_plane)
{
    int16_t l_map_x;
    int16_t l_map_y;
    int16_t l_unit_x;
    int16_t l_unit_y;
    int16_t in_view;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: OVL_BringIntoView(*map_x = %d, *map_y = %d, unit_x = %d, unit_y = %d, map_plane = %d)\n", __FILE__, __LINE__, *map_x, *map_y, unit_x, unit_y, map_plane);
#endif

    l_map_x = *map_x;
    l_map_y = *map_y;

    l_unit_y = unit_y;

    in_view = ST_FALSE;

    // test in-bounds
    if(l_unit_y >= l_map_y && l_unit_y < l_map_y + MAP_HEIGHT)
    {
        l_unit_x = unit_x;
        if((l_unit_x >= l_map_x && l_unit_x < l_map_x + MAP_WIDTH) ||
            (l_unit_x + WORLD_WIDTH >= l_map_x && l_unit_x + WORLD_WIDTH < l_map_x + MAP_WIDTH)
        )
        {
            if((l_unit_x + WORLD_WIDTH > l_map_x && l_unit_x + WORLD_WIDTH < l_map_x + MAP_WIDTH) )
            {
                l_unit_x += WORLD_WIDTH;
            }
            l_unit_x = l_unit_x - l_map_x;
            in_view = ST_TRUE;
        }
    }

    if(in_view == ST_FALSE)
    {
        // ¿ same logic as Center_Map() ?
        l_unit_x = unit_x - (MAP_WIDTH / 2);
        if(l_unit_x > 0)
        {
            l_map_x = l_unit_x;
        }
        else
        {
            l_map_x = (l_unit_x + WORLD_WIDTH) % WORLD_WIDTH;
        }
        l_unit_y = unit_y - (MAP_HEIGHT / 2);
        if(l_unit_y > 0)
        {
            l_map_y = l_unit_y;
        }
        else
        {
            l_map_y = 0;
        }
        if(l_map_y + MAP_HEIGHT >= WORLD_HEIGHT)
        {
            l_map_y = WORLD_HEIGHT - MAP_HEIGHT;
        }

        _prev_world_x = l_map_x;
        _prev_world_y = l_map_y;

        // CRP_OVL_MapWindowX = _prev_world_x * SQUARE_WIDTH;
        // CRP_OVL_MapWindowY = _prev_world_y * SQUARE_HEIGHT;

    }
    else
    {
        // HERE: map_x, map_y, unit_x, unit_y are as they were passed in
        OVL_TileOffScrnEdge(l_map_x, l_map_y, unit_x, unit_y, MAP_WIDTH, MAP_HEIGHT);
    }

    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    Set_Entities_On_Map_Window(l_map_x, l_map_y, map_plane);

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
int16_t OVL_TileOffScrnEdge(int16_t map_x, int16_t map_y, int16_t unit_x, int16_t unit_y, int16_t map_width, int16_t map_height)
{
    int16_t l_map_x;
    int16_t l_map_y;
    int16_t move_map_flag;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: OVL_TileOffScrnEdge(map_x = %d, map_y = %d, unit_x = %d, unit_y = %d, map_width = %d, map_height = %d)\n", __FILE__, __LINE__, map_x, map_y, unit_x, unit_y, map_width, map_height);
#endif

    l_map_x = map_x;
    l_map_y = map_y;

    move_map_flag = ST_FALSE;

    // ¿ Stack is in Fst Column of Movement Map ?
    if(l_map_x == unit_x)
    {
        // ¿ map_x == 0 ?
        l_map_x = l_map_x - 1;  // move Map left 1 world-square
        if(l_map_x < 0)
        {
            l_map_x = l_map_x + WORLD_WIDTH;  // move Map right 1 world-square
        }
        move_map_flag = ST_TRUE;  // set flag - map x,y changed
    }

    // ¿ Stack is in Lst Column of Movement Map ?
    if((l_map_x + map_width - 1) % WORLD_WIDTH == unit_x)
    {
        l_map_x = ((l_map_x + 1) % WORLD_WIDTH);
        move_map_flag = ST_TRUE;  // set flag - map x,y changed
    }

    // ¿ Stack is in Lst Row of Movement Map ?
    if((l_map_y + map_height - 1) == unit_y)
    {
        // ¿ Movement Map is not already at the botton edge of the World  i.e., the map is able to be moved down ?
        if((l_map_y + map_height + 1) < WORLD_HEIGHT)
        {
            l_map_y = l_map_y + 1;  // move Map down 1 world-square
            move_map_flag = ST_TRUE;  // set flag - map x,y changed
        }
    }

    // ¿ Stack is in Fst Row of Movement Map ?
    if(l_map_y == unit_y)
    {
        // ¿ Movement Map is not already at the top edge of the World  i.e., the map is able to be moved up ?
        if(l_map_y != 0)
        {
            l_map_y = l_map_y - 1;  // move Map up 1 world-square
            move_map_flag = ST_TRUE;  // set flag - map x,y changed
        }
    }


    if(move_map_flag = ST_TRUE)
    {
        _prev_world_x = l_map_x;
        _prev_world_y = l_map_y;
        // NIU? CRP_OVL_MapWindowX = _prev_world_x * SQUARE_WIDTH;
        // NIU? CRP_OVL_MapWindowY = _prev_world_y * SQUARE_HEIGHT;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: OVL_TileOffScrnEdge(map_x = %d, map_y = %d, unit_x = %d, unit_y = %d, map_width = %d, map_height = %d) { move_map_flag = %d }\n", __FILE__, __LINE__, map_x, map_y, unit_x, unit_y, map_width, map_height, move_map_flag);
#endif

    return move_map_flag;
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
    int16_t unit_type_idx;
    int16_t case_1_count;
    int16_t itr_stack;
    int16_t unit_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Stack_Action(player_idx = %d, *map_x = %d, *map_y =%d, *map_p = %d, action = %d, destination_x = %d, destination_y = %d)\n", __FILE__, __LINE__, player_idx, *map_x, *map_y, *map_p, action, destination_x, destination_y);
#endif

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
            DLOG("switch(action)  case 0:");
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                unit_idx = _unit_stack[itr_stack].unit_idx;
                _UNITS[unit_idx].Status = 0x00;  /* US_Ready */
                _UNITS[unit_idx].Rd_Constr_Left = ST_UNDEFINED;
                if(_UNITS[unit_idx].HMoves > 0)
                {
                    _unit_stack[itr_stack].active = ST_TRUE;
                    _UNITS[unit_idx].Finished = ST_FALSE;
                }
                else
                {
                    _UNITS[unit_idx].Status = 0x04;  /* US_ReachedDest */
                    _UNITS[unit_idx].Finished = ST_TRUE;
                }
                _UNITS[unit_idx].dst_wx = 0;
                _UNITS[unit_idx].dst_wy = 0;
            }
        } break;
        case 1:
        {
            DLOG("switch(action)  case 1:");
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                unit_idx = _unit_stack[itr_stack].unit_idx;
                if (
                    (_unit_stack[itr_stack].active == ST_TRUE) &&
                    (_UNITS[unit_idx].HMoves > 0) &&
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
            DLOG("switch(action)  case 2:");

        } break;
        case 3:  /* Go-To */
        {
            DLOG("switch(action)  case 3:");
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                unit_idx = _unit_stack[itr_stack].unit_idx;
                if(
                    ((_UNITS[unit_idx].Status & 0x04) != 0) ||  /* US_ReachedDest */
                    ((_UNITS[unit_idx].Status & 0x03) != 0) ||  /* US_GoingTo */
                    ((_UNITS[unit_idx].Status & 0x00) != 0)     /* US_Ready */
                )
                {
                    _UNITS[unit_idx].Status = 0x03;  /* US_GoingTo */
                    _UNITS[unit_idx].dst_wx = destination_x;
                    _UNITS[unit_idx].dst_wy = destination_y;
                }
            }
        } break;
        case 4:  /* ¿ completed on-going action (move path, pave path, purify) ? */
        {
            DLOG("switch(action)  case 4:");
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                if(
                    (_unit_stack[itr_stack].active == ST_TRUE) &&
                    ((_UNITS[_unit_stack[itr_stack].unit_idx].Status & 0x03) == 0) &&  /* US_GoingTo */
                    ((_UNITS[_unit_stack[itr_stack].unit_idx].Status & 0x08) == 0)     /* US_Purify  */
                )
                {
                    unit_idx = _unit_stack[itr_stack].unit_idx;
                    _unit_stack[itr_stack].active = ST_FALSE;
                    _UNITS[unit_idx].Finished = ST_TRUE;
                    _UNITS[unit_idx].Status = 0x04;  /* US_ReachedDest */
                    _UNITS[unit_idx].Rd_Constr_Left = ST_UNDEFINED;
                    _UNITS[unit_idx].HMoves = 0;
                    _UNITS[unit_idx].dst_wx = 0;
                    _UNITS[unit_idx].dst_wy = 0;
                }
            }
        } break;
        case 5:  /* Wait */
        {
            DLOG("switch(action)  case 5:");
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                unit_idx = _unit_stack[itr_stack].unit_idx;
                if(
                    (_unit_stack[itr_stack].active != ST_FALSE) &&
                    (_UNITS[unit_idx].HMoves > 0) &&
                    (_UNITS[unit_idx].Finished == ST_FALSE)
                )
                {
                    _UNITS[unit_idx].Status = 0x05;  /* US_Wait */
                    _unit_stack[itr_stack].active = ST_FALSE;
                    _UNITS[unit_idx].dst_wx = 0;
                    _UNITS[unit_idx].dst_wy = 0;
                }
            }
        } break;
        case 6:
        {
            DLOG("switch(action)  case 6:");

        } break;
        case 7:
        {
            DLOG("switch(action)  case 7:");

        } break;
        case 8:  /* Purify */
        {
            DLOG("switch(action)  case 8:");
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                if(_unit_stack[itr_stack].active == ST_TRUE)
                {
                    unit_idx = _unit_stack[itr_stack].unit_idx;
                    unit_type_idx = _UNITS[unit_idx].type;
                    if( (_unit_type_table[unit_type_idx].Abilities & 0x4000) != 0)  /* Unit Type Ability: Purify / Purification */
                    {
                        _unit_stack[itr_stack].active = ST_FALSE;
                        _UNITS[unit_idx].Finished = ST_TRUE;
                        _UNITS[unit_idx].Status = 0x08;  /* US_Purify */
                        _UNITS[unit_idx].HMoves = 0;
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


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Stack_Action(player_idx = %d, *map_x = %d, *map_y =%d, *map_p = %d, action = %d, destination_x = %d, destination_y = %d)\n", __FILE__, __LINE__, player_idx, *map_x, *map_y, *map_p, action, destination_x, destination_y);
#endif

}


// WZD o62p09
// EarthGateTeleport()


// WZD o62p10
// USW_FullDisplay()



/*
    WIZARDS.EXE  ovr063
*/

// WZD o063p01
// MoO2: ~Print_Main_Screen_Data_()
void Main_Screen_Draw_Status_Window(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Main_Screen_Draw_Status_Window()\n", __FILE__, __LINE__);
#endif

    Set_Outline_Color(0);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] player_idx: %d\n", __FILE__, __LINE__, player_idx);
//     dbg_prn("DEBUG: [%s, %d] _players[player_idx].gold_reserve: %d\n", __FILE__, __LINE__, _players[player_idx].gold_reserve);
//     dbg_prn("DEBUG: [%s, %d] _players[player_idx].mana_reserve: %d\n", __FILE__, __LINE__, _players[player_idx].mana_reserve);
// #endif

    if(_players[player_idx].gold_reserve > 9999 || _players[player_idx].mana_reserve > 9999)
    {
        Set_Font_Style1(0, 0, 0, 0);
    }
    else
    {
        Set_Font_Style1(1, 0, 0, 0);
    }

    Set_Alias_Color(ST_GRAY);

    Print_Integer_Right(265, 68, _players[_human_player_idx].gold_reserve);
    Print_Integer_Right(303, 68, _players[_human_player_idx].mana_reserve);

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

    Reset_Map_Draw();

    Reduced_Map_Set_Dims(REDUCED_MAP_W, REDUCED_MAP_H);

    Draw_Maps(MAP_SCREEN_X, MAP_SCREEN_Y, MAP_WIDTH, MAP_HEIGHT, map_x, map_y, map_plane, x_pos, y_pos, player_idx);

    FLIC_Draw(0, 0, main_background);

    // Draw_Minimap_Window(251, 21, 58, 30);
    // Draw_Reduced_Map(251, 21, 58, 30);
    // Draw_Reduced_Map(REDUCED_MAP_X, REDUCED_MAP_Y, REDUCED_MAP_W, REDUCED_MAP_H);
    Draw_World_Window(REDUCED_MAP_X, REDUCED_MAP_Y, REDUCED_MAP_W, REDUCED_MAP_H);

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
        Main_Screen_Draw_Unit_Window(_unit_window_start_x, _unit_window_start_y);
        // TODO(JimBalcomb,20230616): make note of the drawing of locked buttons in here  ? program-flow oddities ? screen component drawing graph ?
        Main_Screen_Draw_Unit_Action_Buttons();
    }
    else
    {
        Main_Screen_Draw_Summary_Window();
        Main_Screen_Draw_Unit_Action_Locked_Buttons();
        Main_Screen_Draw_Next_Turn_Button();
    }

    Main_Screen_Draw_Status_Window();

    // TODO  if(OVL_StackHasPath == ST_TRUE)
    // TODO  {
    // TODO      j_OVL_DrawPath(OVL_Path_Length, OVL_Path_Xs, OVL_Path_Ys);
    // TODO  }

    Main_Screen_Draw_Game_Buttons();

    // TODO  OVL_DrawStackMoves();
    Main_Screen_Draw_Movement_Bar();

    if(DBG_debug_flag)
    {
        Main_Screen_Draw_Debug_Information();
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Screen_Draw_Do_Draw()\n", __FILE__, __LINE__);
#endif
}


// WZD o063p03
void Draw_Unit_Enchantment_Outline(int16_t unit_idx)
{
    int16_t Unit_Enchantments_LO;
    int16_t Unit_Enchantments_HI;
    int8_t enchantment_magic_realm;
    uint32_t unit_enchantments;
    uint8_t * color_list;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Unit_Enchantment_Outline(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
#endif

    Unit_Enchantments_HI = _UNITS[unit_idx].Enchants_HI;
    Unit_Enchantments_LO = _UNITS[unit_idx].Enchants_LO;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Unit_Enchantments_HI: %04X\n", __FILE__, __LINE__, Unit_Enchantments_HI);
    dbg_prn("DEBUG: [%s, %d]: Unit_Enchantments_LO: %04X\n", __FILE__, __LINE__, Unit_Enchantments_LO);
#endif
    // unit_enchantments = (Unit_Enchantments_HI || Unit_Enchantments_LO << 16);
    unit_enchantments = (Unit_Enchantments_LO || Unit_Enchantments_HI << 16);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: unit_enchantments: %08X\n", __FILE__, __LINE__, unit_enchantments);
#endif

    enchantment_magic_realm = ST_UNDEFINED;

    if(Unit_Has_Invisibility(unit_idx) == ST_TRUE)
    {
        DLOG("(UNIT_HasInvisibility(unit_idx) == ST_TRUE)");
        Unit_Enchantments_HI = 0x0000;
        Unit_Enchantments_LO = 0x0000;
    }

/*
enum Magic Realm
mr_Chaos = 0,
mr_Death = 1,
mr_Life = 2,
mr_Nature = 3,
mr_Sorcery = 4,
*/

    if( (Unit_Enchantments_HI || Unit_Enchantments_LO) != 0 )
    {
        DLOG("( (Unit_Enchantments_HI || Unit_Enchantments_LO) != 0 )");

        if((Unit_Enchantments_LO & 0x40) != 0)  /* UE_Regeneration */
        {
            enchantment_magic_realm = 3;  /* Nature */
        }
        else if((Unit_Enchantments_HI & 0x4) != 0)  /* UE_Resist_Magic */
        {
            DLOG("((Unit_Enchantments_HI & 0x4) != 0)  /* UE_Resist_Magic */");
            enchantment_magic_realm = 4;  /* Sorcery */
        }
        else if((Unit_Enchantments_HI & 0x4000) != 0)  /* UE_Righteousness */
        {
            enchantment_magic_realm = 2;  /* Life */
        }
        else if((Unit_Enchantments_HI & 0x1000) != 0)  /* UE_Planar_Travel */
        {
            enchantment_magic_realm = 2;  /* Life */
        }
        else if((Unit_Enchantments_LO & 0x10) != 0)  /* UE_Black_Channels */
        {
            enchantment_magic_realm = 1;  /* Death */
        }
        else if((Unit_Enchantments_HI & 0x400) != 0)  /* UE_Lionheart*/
        {
            enchantment_magic_realm = 2;  /* Life */
        }
        else if((Unit_Enchantments_LO & 0x1000) != 0) /* UE_Iron_Skin*/
        {
            enchantment_magic_realm = 3;  /* Nature */
        }
        else if((Unit_Enchantments_HI & 0x8) != 0)  /* UE_Magic_Immunity */
        {
            DLOG("((Unit_Enchantments_HI & 0x8) != 0)  /* UE_Magic_Immunity */");
            enchantment_magic_realm = 4;  /* Sorcery */
        }
        else if((Unit_Enchantments_HI & 0x1) != 0)  /* UE_Wind_Walking */
        {
            DLOG("((Unit_Enchantments_HI & 0x1) != 0)  /* UE_Wind_Walking */");
            enchantment_magic_realm = 4;  /* Sorcery */
        }
        else if((Unit_Enchantments_LO & 0x400) != 0)  /* UE_Elemental_Armor */
        {
            enchantment_magic_realm = 3;  /* Nature */
        }
        else if((Unit_Enchantments_HI & 0x40) != 0)  /* UE_True_Sight */
        {
            enchantment_magic_realm = 2;  /* Life */
        }
        else if((Unit_Enchantments_HI & 0x8000) != 0)  /* UE_Invulnerability */
        {
            enchantment_magic_realm = 2;  /* Life */
        }
        else if((Unit_Enchantments_LO & 0x80) != 0)  /* UE_Path_Finding */
        {
            enchantment_magic_realm = 3;  /* Nature */
        }
        else if((Unit_Enchantments_HI & 0x2) != 0)  /* UE_Flight */
        {
            DLOG("((Unit_Enchantments_HI & 0x2) != 0)  /* UE_Flight */");
            enchantment_magic_realm = 4;  /* Sorcery */
        }
        else if((Unit_Enchantments_HI & 0x20) != 0)  /* UE_Eldritch_Weapon */
        {
            enchantment_magic_realm = 0;  /* Chaos */
        }
        else if((Unit_Enchantments_LO & 0x20) != 0) /* UE_Wraith_Form */
        {
            enchantment_magic_realm = 1;  /* Death */
        }
        else if((Unit_Enchantments_HI & 0x800) != 0)  /* UE_Giant_Strength */
        {
            enchantment_magic_realm = 3;  /* Nature */
        }
        else if((Unit_Enchantments_LO & 0x1) != 0)  /* UE_Immolation */
        {
            enchantment_magic_realm = 0;  /* Chaos */
        }
        else if((Unit_Enchantments_LO & 0x2) != 0)  /* UE_Guardian_Wind */
        {
            DLOG("((Unit_Enchantments_LO & 0x2) != 0)  /* UE_Guardian_Wind */");
            enchantment_magic_realm = 4;  /* Sorcery */
        }
        else if((Unit_Enchantments_LO & 0x4000) != 0)  /* UE_Spell_Lock */
        {
            DLOG("((Unit_Enchantments_LO & 0x4000) != 0)  /* UE_Spell_Lock */");
            enchantment_magic_realm = 4;  /* Sorcery */
        }
        else if((Unit_Enchantments_HI & 0x100) != 0)  /* UE_Heroism */
        {
            enchantment_magic_realm = 2;  /* Life */
        }
        else if((Unit_Enchantments_LO & 0x4) != 0)  /* UE_Berserk */
        {
            enchantment_magic_realm = 1;  /* Death */
        }
        else if((Unit_Enchantments_HI & 0x2000) != 0)  /* UE_Holy_Armor */
        {
            enchantment_magic_realm = 2;  /* Life */
        }
        else if((Unit_Enchantments_LO & 0x100) != 0)  /* UE_Water_Walking */
        {
            enchantment_magic_realm = 3;  /* Nature */
        }
        else if((Unit_Enchantments_LO & 0x2000) != 0)  /* UE_Endurance */
        {
            enchantment_magic_realm = 2;  /* Life */
        }
        else if((Unit_Enchantments_LO & 0x8) != 0)  /* UE_Cloak_of_Fear */
        {
            enchantment_magic_realm = 1;  /* Death */
        }
        else if((Unit_Enchantments_LO & 0x800) != 0)  /* UE_Stone_Skin */
        {
            enchantment_magic_realm = 3;  /* Nature */
        }
        else if((Unit_Enchantments_HI & 0x10) != 0)  /* UE_Flame_Blade */
        {
            enchantment_magic_realm = 0;  /* Chaos */
        }
        else if((Unit_Enchantments_HI & 0x200) != 0)  /* UE_Bless */
        {
            enchantment_magic_realm = 2;  /* Life */
        }
        else if((Unit_Enchantments_HI & 0x80) != 0)  /* UE_Holy_Weapon */
        {
            enchantment_magic_realm = 2;  /* Life */
        }
        else if((Unit_Enchantments_LO & 0x200) != 0)  /* UE_Resist_Elements */
        {
            enchantment_magic_realm = 3;  /* Nature */
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: enchantment_magic_realm: %d\n", __FILE__, __LINE__, enchantment_magic_realm);
#endif

    if(enchantment_magic_realm != ST_UNDEFINED)
    {
        DLOG("(enchantment_magic_realm != ST_UNDEFINED)");
        Outline_Bitmap_Pixels_No_Glass(UnitDraw_WorkArea, 255);
        // Bitmap_Aura_Pixels(UnitDraw_WorkArea, 255, (enchantment_outline_colors + ((enchantment_magic_realm * 64) + (unit_enchantment_animation_count * 8))));
        color_list = (uint8_t *)(enchantment_outline_colors + ((enchantment_magic_realm * 64) + (unit_enchantment_animation_count * 8)));
        Bitmap_Aura_Pixels(UnitDraw_WorkArea, 255, color_list);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Unit_Enchantment_Outline(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
#endif
}




// WZD o063p04
// drake178: j_GAME_Animate_UEs()
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
void Draw_Unit_Picture(int16_t x, int16_t y, int16_t unit_idx, int16_t flag)
{
    int16_t unit_owner_idx;
    int16_t unit_colored_backgrounds_idx;
#ifdef STU_DEBUG
    int16_t screen_x;
    int16_t screen_y;
    uint16_t screen_page;
    uint16_t screen_ofst;
    int16_t itr_width;
    int16_t itr_height;
    uint8_t * vbb_ptr;
#endif

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Unit_Picture(x = %d, y = %d, unit_idx = %d, flag = %d)\n", __FILE__, __LINE__, x, y, unit_idx, flag);
// #endif

// #ifdef STU_DEBUG
//     // if(unit_idx == 79)  /* Unit Enchantment Outline */
//     // {
//     //     DBG_Outline_Bitmap_Pixels_No_Glass = 1;
//     // }
//     // dbg_prn("DEBUG: [%s, %d]: DBG_Outline_Bitmap_Pixels_No_Glass = 1\n", __FILE__, __LINE__);
//     // if(unit_idx == 156)  /* Unit Has Invisibility */
//     if(x == 120 && y == 110 && unit_idx == 156 && flag == 2)
//     {
//         DLOG("(x == 120 && y == 110 && unit_idx == 156 && flag == 2)");
//         DBG_Draw_Invisibility = 1;
//         DBG_Remap_Draw_Picture_ASM = 1;
//     }
// #endif



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
// #ifdef STU_DEBUG
//     if(DBG_Draw_Invisibility == 1)  /* Unit Has Invisibility */
//     {
//         DLOG("Unit Colored Background");
//         // screen_page = (video_page_buffer[1 - draw_page_num]);
//         // screen_ofst = (80 * SCREEN_WIDTH) + 248;
//         screen_x = 121;
//         screen_y = 111;
//         vbb_ptr = (video_page_buffer[1 - draw_page_num] + ((screen_y * SCREEN_WIDTH) + screen_x));
//         for(itr_height = 0; itr_height < STATFIG_HEIGHT; itr_height++)
//         {
//             for(itr_width = 0; itr_width < STATFIG_WIDTH; itr_width++)
//             {
//                 dbg_prn("%02X\n", *(vbb_ptr + ((itr_height * SCREEN_WIDTH) + itr_width)));
//             }
//         }
//     }
// #endif

    }

    Draw_Unit_StatFig(x, y, unit_idx, flag);

// #ifdef STU_DEBUG
//     if(DBG_Draw_Invisibility == 1)  /* Unit Has Invisibility */
//     {
//         DLOG("(DBG_Draw_Invisibility == 1)");
//         // screen_page = (video_page_buffer[1 - draw_page_num]);
//         // screen_ofst = (80 * SCREEN_WIDTH) + 248;
//         // vbb_ptr = (video_page_buffer[1 - draw_page_num] + ((80 * SCREEN_WIDTH) + 248));
//         screen_x = 121;
//         screen_y = 111;
//         vbb_ptr = (video_page_buffer[1 - draw_page_num] + ((screen_y * SCREEN_WIDTH) + screen_x));
//         
//         for(itr_height = 0; itr_height < 16; itr_height++)
//         {
//             for(itr_width = 0; itr_width < 18; itr_width++)
//             {
//                 dbg_prn("%02X\n", *(vbb_ptr + ((itr_height * SCREEN_WIDTH) + itr_width)));
//             }
//         }
//     }
// #endif

#ifdef STU_DEBUG
    // if(unit_idx == 79)  /* Unit Enchantment Outline */
    // {
    //     DBG_Outline_Bitmap_Pixels_No_Glass = 1;
    // }
    // dbg_prn("DEBUG: [%s, %d]: DBG_Outline_Bitmap_Pixels_No_Glass = 1\n", __FILE__, __LINE__);
    // if(unit_idx == 156)  /* Unit Has Invisibility */
    if(x == 120 && y == 110 && unit_idx == 156 && flag == 2)
    {
        DBG_Draw_Invisibility = 0;
        DBG_Remap_Draw_Picture_ASM = 0;
    }
    DBG_Draw_Invisibility = 0;
    DBG_Remap_Draw_Picture_ASM = 0;
#endif

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Draw_Unit_Picture(x = %d, y = %d, unit_idx = %d, flag = %d)\n", __FILE__, __LINE__, x, y, unit_idx, flag);
// #endif
}


// WZD o063p05
// AKA OVL_DrawUnitImage()
// pict_seg is 
// MoO2  Draw_Ship_Icon()
void Draw_Unit_StatFig(int16_t x, int16_t y, int16_t unit_idx, int16_t flag)
{
    uint8_t unit_type_idx;
    int16_t unit_owner_idx;
    int16_t banner_idx;
    uint16_t itr_color_remap;
    // uint8_t Color_1;
    // uint8_t Color_2;
    uint8_t color_array[2];
    uint8_t * vbb_ptr;
    int16_t itr_width;
    int16_t itr_height;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Unit_StatFig(x = %d, y = %d, unit_idx = %d, flag = %d)\n", __FILE__, __LINE__, x, y, unit_idx, flag);
// #endif

// DELETE  #ifdef STU_DEBUG
// DELETE      if(x == 120 && y == 110 && unit_idx == 156 && flag == 2)
// DELETE      {
// DELETE          DLOG("(x == 120 && y == 110 && unit_idx == 156 && flag == 2)");
// DELETE          // DBG_Draw_Unit_StatFig = 1;
// DELETE          DBG_Remap_Draw_Picture_ASM = 1;
// DELETE      }
// DELETE  #endif

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
            // DLOG("(UNIT_HasInvisibility(unit_idx) == ST_TRUE)");
            Replace_Color_All(UnitDraw_WorkArea, 233); // sets every non-transparent pixel; 233 means use remap colors - (233-232=1) remap color block 1
            Outline_Bitmap_Pixels(UnitDraw_WorkArea, ST_BLACK);
        }

        if((flag != 3) && 
            (_UNITS[unit_idx].Status == 1) /* US_Patrol */ && 
            (Unit_Has_Invisibility(unit_idx) == ST_FALSE)  && 
            (unit_owner_idx == _human_player_idx)
        )
        {
            // DLOG("Unit is Inactive/Finished");
            Gray_Scale_Bitmap(UnitDraw_WorkArea, 1);  // ¿ 1 means ... ?
        }
    
        Draw_Unit_Enchantment_Outline(unit_idx);
    }

    FLIC_Set_LoopFrame_1(UnitDraw_WorkArea);  // sets for/enables remap colors
    Draw_Picture(x+1, y+1, UnitDraw_WorkArea);

// DELETE  #ifdef STU_DEBUG
// DELETE      if(x == 120 && y == 110 && unit_idx == 156 && flag == 2)
// DELETE      {
// DELETE          DLOG("(x == 120 && y == 110 && unit_idx == 156 && flag == 2)");
// DELETE          // vbb_ptr = (video_page_buffer[1 - draw_page_num] + (((y+1) * SCREEN_WIDTH) + (x+1)));
// DELETE          vbb_ptr = (video_page_buffer[draw_page_num] + (((y+1) * SCREEN_WIDTH) + (x+1)));
// DELETE          
// DELETE          for(itr_height = 0; itr_height < 16; itr_height++)
// DELETE          {
// DELETE              for(itr_width = 0; itr_width < 18; itr_width++)
// DELETE              {
// DELETE                  dbg_prn("%02X\n", *(vbb_ptr + ((itr_height * SCREEN_WIDTH) + itr_width)));
// DELETE              }
// DELETE          }
// DELETE      }
// DELETE  #endif

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
        Set_Font_Style1(0, 15, 0, 0); // set font style num 0; set normal,highlight,special colors - blocks 15,0,0; set shadow bottom right

        switch( (_UNITS[unit_idx].Status - 2) )
        {
            case 0:  /* ¿ US_BuildRoad == 2 ? */
            {
                Print(x+2, y+2, "B");  // drak178: UNIT_Building_Mark
            } break;
            case 1:  /* ¿ US_GoingTo == 3 ? */
            {
                if(_UNITS[unit_idx].Rd_Constr_Left == ST_UNDEFINED)
                {
                    Print(x+2, y+2, "G");  // drak178: UNIT_Going_Mark
                }
                else
                {
                    Print(x+2, y+2, "B");  // drak178: UNIT_Building_Mark
                }
            } break;
            case 4:
            {
                Print(x+4, y+5, "C");  // drak178: UNIT_Unk_C_Mark
            } break;
            case 6:  /* ¿ US_Purify == 8 ? */
            {
                Print(x+2, y+2, "P");  // drak178: UNIT_Purifying_Mark
            } break;
        }
    }
    /*
        END: Unit Status - Icon/Text
    */


// DELETE #ifdef STU_DEBUG
// DELETE     if(x == 120 && y == 110 && unit_idx == 156 && flag == 2)
// DELETE     {
// DELETE         DBG_Remap_Draw_Picture_ASM = 0;
// DELETE     }
// DELETE #endif

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Draw_Unit_StatFig(x = %d, y = %d, unit_idx = %d, flag = %d)\n", __FILE__, __LINE__, x, y, unit_idx, flag);
// #endif
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
    max_hits = UNIT_GetHitsPerFig(unit_idx) * _unit_type_table[Unit_Type].Figures;
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
        Weapon_Quality = (_UNITS[unit_idx].Mutations & 0x03);  /* bit set - 1st and/or 2nd  OR  2^2 values {0,1,2,3} */
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
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw_Movement_Bar()\n", __FILE__, __LINE__);
#endif


    // _help_entries+0AAh = ST_UNDEFINED

    if(_unit_stack_count > 0)
    {
        movement_points = Stack_Moves_Active();
        // movement_points = 17;
        if(movement_points != 0)
        {
            // _help_entries+0AAh = HLP_MOVES
            Set_Font_Style1(1,1,0,0);
            Set_Outline_Color(0);
            Set_Alias_Color(0);
            Print(246, 167, cnst_Moves);
            Print_Moves_String(275, 167, movement_points, ST_FALSE);

            Set_Font(1,1,0,0);
            Set_Outline_Color(0);
            Set_Alias_Color(0);
            Print(246, 168, cnst_Moves);
            Print_Moves_String(275, 168, movement_points, ST_FALSE);

            Set_Font(1,0,0,0);
            Set_Alias_Color(0);
            Print(246, 167, cnst_Moves);
            Print_Moves_String(275, 167, movement_points, ST_FALSE);

            Draw_Movement_Mode_Icons(286, 167, ST_UNDEFINED);
        }
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Screen_Draw_Movement_Bar()\n", __FILE__, __LINE__);
#endif
}


// WZD o63p08
// drake178: OVL_DrawMoveTypes()
/*
    if called with unit_idx = -1, _unit_stack_count MUST BE >= 1
    Main_Screen_Draw_Movement_Bar() calls with unit_idx = ST_UNDEFINED
*/
void Draw_Movement_Mode_Icons(int16_t x, int16_t y, int16_t unit_idx)
{
    int16_t movement_icons[9];
    // struct s_Movement_Modes movement_mode_flags;
    int16_t movement_mode_flags[9];
    int16_t stack;
    int16_t x_start;
    int itr;
    int movement_mode_icon_count;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Movement_Mode_Icons(x = %d, y = %d, unit_idx = %d)\n", __FILE__, __LINE__, x, y, unit_idx);
#endif

    /*
        set array of movement mode flags to false
    */
    if(_unit_stack_count >= 1 || unit_idx != ST_UNDEFINED)
    {
#ifdef STU_DEBUG
        if(_unit_stack_count >= 1)
            DLOG("(_unit_stack_count >= 1)");
        if(unit_idx != ST_UNDEFINED)
            DLOG("(unit_idx != ST_UNDEFINED)");
#endif

        for(itr = 0; itr < 9; itr++)  /* TODO: add define for 9 - (Special) Movemenet Mode Count/Max */
        {
            movement_mode_flags[itr] = ST_FALSE;
        }


        if(unit_idx == ST_UNDEFINED)
        {
            // Active_Stack_Movement_Modes(&movement_mode_flags);
            Active_Stack_Movement_Modes(&movement_mode_flags[0]);
        }
        else
        {
            stack = unit_idx;
            // Stack_Movement_Modes(&movement_mode_flags, &stack, 1);
            Stack_Movement_Modes(&movement_mode_flags[0], &stack, 1);
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
    else
    {
        // if(movement_mode_flags.mm_Sailing == ST_TRUE)
        if(movement_mode_flags[OFS_SAILING] == ST_TRUE)
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


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Movement_Mode_Icons(x = %d, y = %d, unit_idx = %d)\n", __FILE__, __LINE__, x, y, unit_idx);
#endif
}


// WZD o63p09
// drake178: OVL_CanPlanarTravel()
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Stack_Has_PlanarTravel()\n", __FILE__, __LINE__);
#endif


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
        DLOG("(stack_has_no_active_units == ST_TRUE)");
        for(itr = 0; itr < _unit_stack_count; itr++)
        {
            _unit_stack[itr].active = ST_TRUE;
        }
    }
    else
    {
        DLOG("(stack_has_no_active_units == ST_FALSE)");
    }

    has_planartravel = ST_FALSE;

    // drake178: returns 1 if the active stack is on a tile that has a city equipped with an Astral Gate (cast by the human player) on either plane; or 0 otherwise
    // OVL_CheckAstralGate()
    // TODO  stack_on_astral_gate = Active_Stack_On_Astral_Gate()
    stack_on_astral_gate = ST_FALSE;

    // Unused_Local = 0

    for(itr = 0; itr < _unit_stack_count; itr++)
    {
        unit_idx = _unit_stack[itr].unit_idx;
        if(_unit_stack[itr].active == ST_TRUE)
        {
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: ( (_UNITS[unit_idx].Enchants_HI & UE_PLANARTRAVEL) != 0 ): %d\n", __FILE__, __LINE__, ( (_UNITS[unit_idx].Enchants_HI & UE_PLANARTRAVEL) != 0 ));
    dbg_prn("DEBUG: [%s, %d]: ( (_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_PLANARTRAVEL) != 0 ): %d\n", __FILE__, __LINE__, ( (_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_PLANARTRAVEL) != 0 ));
    dbg_prn("DEBUG: [%s, %d]: Unit_Has_PlanarTravel_Item(unit_idx): %d\n", __FILE__, __LINE__, Unit_Has_PlanarTravel_Item(unit_idx));
#endif
            if(
                ( (_UNITS[unit_idx].Enchants_HI & UE_PLANARTRAVEL) != 0 ) ||
                ( (_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_PLANARTRAVEL) != 0 ) ||
                ( Unit_Has_PlanarTravel_Item(unit_idx) == ST_TRUE )
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Stack_Has_PlanarTravel() { has_planartravel = %d }\n", __FILE__, __LINE__, has_planartravel);
#endif
    return has_planartravel;
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

almost the same code as in WZD ovr097 Units_Moves()
Units_Moves() breaks after Unit_Has_WindWalking(); does not have stack_has_windwalker or windwalker_unit_idx
OON XREF STK_Move() WZD o95p01

*/
    int16_t stack_has_windwalker;
    int16_t windwalker_unit_idx;
    int16_t movement_points; // In Dasm, SI
    int16_t tmp_unit_idx;  // In Dasm, DI
    int16_t itr_unit_stack;
    int8_t tmp_unit_type;  // In Dasm, DNE

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Stack_Moves_Active()\n", __FILE__, __LINE__);
#endif

    movement_points = 1000;

    stack_has_windwalker = ST_FALSE;

    for(itr_unit_stack = 0; itr_unit_stack < _unit_stack_count; itr_unit_stack++)
    {
        tmp_unit_idx = _unit_stack[itr_unit_stack].unit_idx;
        if(_unit_stack[itr_unit_stack].active == ST_TRUE)
        {
            if(_UNITS[tmp_unit_idx].HMoves < movement_points)
            {
                movement_points = _UNITS[tmp_unit_idx].HMoves;
            }

            tmp_unit_type = _UNITS[tmp_unit_idx].type;
            if(_unit_type_table[tmp_unit_type].Transport == ST_TRUE)
            {
                movement_points = _UNITS[tmp_unit_idx].HMoves;
                break;
            }

            if(Unit_Has_WindWalking(tmp_unit_idx) == ST_TRUE)
            {
                stack_has_windwalker = ST_TRUE;
                windwalker_unit_idx = tmp_unit_idx;
                movement_points = _UNITS[tmp_unit_idx].HMoves;
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
        if(_UNITS[unit_idx].HMoves < movement_points)
        {
            movement_points = _UNITS[unit_idx].HMoves;
        }
        if(_unit_type_table[_UNITS[unit_idx].type].Transport > 0)
        {
            movement_points = _UNITS[unit_idx].HMoves;
            break;
        }
        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
        {
            stack_has_windwalker = ST_TRUE;
            movement_points = _UNITS[unit_idx].HMoves;
            windwalker_unit_idx = unit_idx;
        }
    }

    if(stack_has_windwalker == ST_TRUE)
    {
        if(_UNITS[windwalker_unit_idx].HMoves > 0)
        {
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                unit_idx = _unit_stack[itr_stack].unit_idx;

                if( (_UNITS[windwalker_unit_idx].HMoves < 0) && (Unit_Has_WindWalking(unit_idx) != ST_TRUE) )
                {
                    _UNITS[unit_idx].HMoves = 1;
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Stack_Moves() { stack_moves = %d }\n", __FILE__, __LINE__, stack_moves);
#endif

    return stack_moves;
}


// WZD o63p14
// RP_TILE_HasCity2()


// WZD o63p14
// Check_Planar_Seal()



/* 
    WIZARDS.EXE  ovr064
*/

// WZD o64p01
// void Allocate_Reduced_Map__1(void)
// WZD o64p02
// PRIVATE  void Allocate_Reduced_Map__2(void);
// drake178: j_RP_LBX_Minimap_Alloc2
void Allocate_Reduced_Map(void)
{
    _reduced_map_seg = Allocate_First_Block(_screen_seg, 303);  // 303 * 16 = 4848 bytes
}


// WZD o64p03
// ? subset of Draw_Maps() ?
// AKA OVL_PrepMinimap()
void MainScr_Prepare_Reduced_Map(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: MainScr_Prepare_Reduced_Map()\n", __FILE__, __LINE__);
#endif
    int16_t minimap_x = 0;
    int16_t minimap_y = 0;
    int16_t minimap_width;
    int16_t minimap_height;

    // minimap_width = 58;
    // minimap_height = 30;
    minimap_width = REDUCED_MAP_W;
    minimap_height = REDUCED_MAP_H;

    Reduced_Map_Set_Dims(minimap_width, minimap_height);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _map_x: %d  _map_y: %d\n", __FILE__, __LINE__, _map_x, _map_y);
#endif

    Reduced_Map_Coords(&minimap_x, &minimap_y, ((_map_x + (MAP_WIDTH / 2)) % WORLD_WIDTH), (_map_y + (MAP_HEIGHT / 2)), minimap_width, minimap_height);
    
    Draw_Reduced_Map(minimap_x, minimap_y, _map_plane, _reduced_map_seg, minimap_width, minimap_height, 0, 0, 0);

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
    uint8_t color_index;

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
    gold = 142;
    food = 5;
    mana = 35;


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
    // ¿ this only impacts gold < 0 and cycle_incomes != ST_UNDEFINED ?

    Set_Outline_Color(ST_BLACK);



    if(gold >= 0)
    {
        DLOG("(gold >= 0)");
        Set_Font_Style_Outline(0, 4, 0, 0);

// DELETE  #ifdef STU_DEBUG
// DELETE      for(color_index = 0; color_index < 16; color_index++)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: current_colors[%d]: 0x%02X\n", __FILE__, __LINE__, color_index, GET_1B_OFS(font_style_data, (FONT_HDR_POS_CURRENT_COLORS + color_index)));
// DELETE      }
// DELETE  #endif

        Set_Alias_Color(184);  // 184d  B8h  ~"dark gold"

// DELETE  #ifdef STU_DEBUG
// DELETE      for(color_index = 0; color_index < 16; color_index++)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: current_colors[%d]: 0x%02X\n", __FILE__, __LINE__, color_index, GET_1B_OFS(font_style_data, (FONT_HDR_POS_CURRENT_COLORS + color_index)));
// DELETE      }
// DELETE  #endif

    }
    else
    {
        DLOG("(gold < 0)");
        if(cycle_incomes == ST_UNDEFINED)
        {
            Set_Font_Style_Outline(0, 2, 0, 0);
        }
        else
        {
            Set_Font_Style_Outline(0, 15, 0, 0);
        }
    }
// #ifdef STU_DEBUG
//     DBG_Main_Screen_Draw_Summary_Window = 1;
//     DBG_Print_Integer_Right = 1;
//     DBG_Print_Right = 1;
//     DBG_Get_String_Width = 1;
//     DBG_Print = 1;
//     DBG_Print_Display = 1;
//     DBG_Print_String = 1;
//     DBG_Print_Character = 1;
//     DBG_Print_Character_ASM = 1;
//     DBG_Print_Character_No_Alias = 1;
//     DBG_Print_Character_No_Alias_ASM = 1;
// #endif
    Print_Integer_Right(273, 101, gold);
#ifdef STU_DEBUG
    DBG_Main_Screen_Draw_Summary_Window = 0;
    DBG_Print_Integer_Right = 0;
    DBG_Print_Right = 0;
    DBG_Get_String_Width = 0;
    DBG_Print = 0;
    DBG_Print_Display = 0;
    DBG_Print_String = 0;
    DBG_Print_Character = 0;
    DBG_Print_Character_ASM = 0;
    DBG_Print_Character_No_Alias = 0;
    DBG_Print_Character_No_Alias_ASM = 0;
#endif

    Print(278, 101, cstr_Gold);


    if(food >= 0)
    {
        DLOG("(food >= 0)");
        Set_Font_Style_Outline(0, 4, 0, 0);
        Set_Alias_Color(184);
    }
    else
    {
        DLOG("(food < 0)");
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
        DLOG("(mana >= 0)");
        Set_Font_Style_Outline(0, 4, 0, 0);
        Set_Alias_Color(184);
    }
    else
    {
        DLOG("(mana < 0)");
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
        Set_Font_Style_Outline(0, 15, 0, 0);
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
        Set_Font_Style_Outline(0, 15, 0, 0);
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
        Set_Font_Style_Outline(0, 15, 0, 0);
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
        Set_Font_Style_Outline(0, 15, 0, 0);
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
        Set_Font_Style_Outline(0, 15, 0, 0);
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

        if(current_screen == ST_UNDEFINED)
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

    FLIC_Set_CurrentFrame(main_lock_wait_button, 0);
    FLIC_Draw(246, 186, main_lock_wait_button);

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




/*
    WIZARDS.EXE  ovr095
*/

// WZD o95p01
// AKA Move_Stack()
int16_t Move_Units(int16_t player_idx, int16_t destination_x, int16_t destination_y, int16_t Spec, int16_t * map_x, int16_t * map_y, int16_t map_p, int16_t unit_array_count, int16_t unit_array[])
{

    int16_t boat_rider_array[9];
    int16_t movement_modes[9];
// Construction_Total= word ptr -22h
// UU_Endurance_Value= word ptr -20h
    int16_t boat_rider_count;
    int16_t UU_flag_FALSE;
    int16_t UU_unit_p;
    int16_t Total_Move_Cost;
    int16_t Out_Of_Moves;
    int16_t movement_points;
// Obstacle_Index= word ptr -12h
    int16_t Combat_Move;
// Current_Step= word ptr -0Eh
// YPos= word ptr -0Ch
// XPos= word ptr -0Ah
    int16_t unit_y;
    int16_t unit_x;
    int16_t Path_Length;
// First_Unit_Index= word ptr -2

    int16_t unit_idx;
    int16_t return_value;
    int16_t itr_eight;
    int16_t itr_Path_Length;
    int16_t itr_units;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Move_Units(player_idx = %d, destination_x = %d, destination_y = %d, Spec = %d, *map_x = %d, *map_y = %d, map_p = %d, unit_array_count = %d, &unit_array[0] = %p)\n", __FILE__, __LINE__, player_idx, destination_x, destination_y, Spec, *map_x, *map_y, map_p, unit_array_count, &unit_array[0]);
#endif

    unit_idx = unit_array[0];
    unit_x = _UNITS[unit_idx].world_x;
    unit_y = _UNITS[unit_idx].world_y;
    UU_unit_p = _UNITS[unit_idx].world_plane;

    UU_flag_FALSE = ST_FALSE;

    Set_Draw_Active_Stack_Always();
    Reset_Map_Draw();

    movement_points = Units_Moves(unit_array, unit_array_count);

    if(movement_points >= 1)
    {
        DLOG("(movement_points >= 1)");
        // movement_modes[{0,...,8}] = 0
        for(itr_eight = 0; itr_eight < 8; itr_eight++)
        {
            movement_modes[itr_eight] = 0;
        }

        Stack_Movement_Modes(&movement_modes[0], unit_array, unit_array_count);

        boat_rider_count = STK_GetLandlubbers(unit_array_count, unit_array, &boat_rider_array[0]);

        if(Spec == 0)
        {
            DLOG("(Spec == 0)");
// push    [bp+player_idx]
// push    [bp+unit_array_count]           ; UCnt
// push    [bp+boat_rider_count]           ; LLCnt
// push    [bp+movement_points]            ; UU2
// mov     ax, 1
// push    ax                              ; UU1
// mov     ax, offset OVL_Path_Costs
// push    ax                              ; RCs@
// mov     ax, offset OVL_Path_Ys
// push    ax                              ; RYs@
// mov     ax, offset OVL_Path_Xs
// push    ax                              ; RXs@
// push    [bp+map_plane]                  ; Plane
// push    [bp+destination_y]              ; TgtY
// push    [bp+destination_x]              ; TgtX
// push    [bp+unit_y]                     ; SrcY
// push    [bp+unit_x]                     ; SrcX
// push    [bp+movement_modes+0Ah]
// push    [bp+movement_modes+8]
// push    [bp+movement_modes+6]
// push    [bp+movement_modes+4]
// push    [bp+movement_modes+2]
// push    [bp+movement_modes]                     ; MTypes
// Path_Length = STK_GetPath();
// fills the return arrays with a path from the source to the destination tile,
// either by reading it out from the stored paths or by generating a new one from scratch
// ; returns the path length
// ; rewritten in the overland djikstra patch
            Path_Length = 1;
            OVL_Path_Costs[0] = 1;
            // MovePath_X[0] = unit_x;
            // MovePath_Y[0] = unit_y;
            // MovePath_X[1] = destination_x;
            // MovePath_Y[1] = destination_y;
            MovePath_X[0] = destination_x;
            MovePath_Y[0] = destination_y;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Path_Length: %d\n", __FILE__, __LINE__, Path_Length);
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: OVL_Path_Costs[0]: %d\n", __FILE__, __LINE__, OVL_Path_Costs[0]);
    // dbg_prn("DEBUG: [%s, %d]: MovePath_X[0]: %d\n", __FILE__, __LINE__, MovePath_X[0]);
    // dbg_prn("DEBUG: [%s, %d]: MovePath_Y[0]: %d\n", __FILE__, __LINE__, MovePath_Y[0]);
    // dbg_prn("DEBUG: [%s, %d]: MovePath_X[1]: %d\n", __FILE__, __LINE__, MovePath_X[1]);
    // dbg_prn("DEBUG: [%s, %d]: MovePath_Y[1]: %d\n", __FILE__, __LINE__, MovePath_Y[1]);
    dbg_prn("DEBUG: [%s, %d]: MovePath_X[0]: %d\n", __FILE__, __LINE__, MovePath_X[0]);
    dbg_prn("DEBUG: [%s, %d]: MovePath_Y[0]: %d\n", __FILE__, __LINE__, MovePath_Y[0]);
#endif



            if(Path_Length >= 1)
            {
                DLOG("(Path_Length >= 1)");
                Out_Of_Moves = ST_FALSE;
                Combat_Move = ST_FALSE;
                //TODO  OVL_SWardTriggered = ST_FALSE;

// push    [bp+unit_array_count]           ; UCount
// push    [bp+unit_array@]                ; UList@
// lea     ax, [bp+Out_Of_Moves]
// push    ax                              ; Cmplt@
// lea     ax, [bp+Path_Length]
// push    ax                              ; Length@
// lea     ax, [bp+Combat_Move]
// push    ax                              ; Cmbt@
// lea     ax, [bp+Obstacle_Index]
// push    ax                              ; Obst@
// push    [bp+movement_points]            ; HMoves
// mov     ax, offset OVL_Path_Costs
// push    ax                              ; Costs@
// push    [bp+map_plane]                  ; Plane
// mov     ax, offset OVL_Path_Ys
// push    ax                              ; Ys@
// mov     ax, offset OVL_Path_Xs
// push    ax                              ; Xs@
// push    [bp+player_idx]                 ; Player_Index
// call    j_STK_EvaluatePath              
// ; evaluates the stack's set path, and sets the return
// ; values accordingly:
// ;   Cmplt@ - 1 if running out of moves (no obstacles)
// ;   Path_Length@ - amount of moves that can be executed
// ;   Combat@ - the move enters a hostile tile (the human
// ;     player's move is stopped short before this)
// ;   Obst@ - a player or unit index identifying the
// ;     hostile entity (BUG? should be player index?)
// ; returns 1 if the stack can use up all of its
// ; movement, or 0 otherwise (moving gets interrupted)
// ;
// ; contains multiple transport-related BUGs

                // ¿ accumulate path cost ?
                Total_Move_Cost = 0;
                for(itr_Path_Length = 0; itr_Path_Length < Path_Length; itr_Path_Length++)
                {
                    Total_Move_Cost += OVL_Path_Costs[itr_Path_Length];
                }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Total_Move_Cost: %d\n", __FILE__, __LINE__, Total_Move_Cost);
#endif


                if(Path_Length <= 1)
                {
                    DLOG("(Path_Length <= 1)");
                    // NOTE: (Path_Length >= 1) && (Path_Length <= 1) ∴ (Path_Length == 1)

                    // drake178: set to the path tile before the last before moving units overland
                    OVL_Action_OriginX = unit_x;
                    OVL_Action_OriginY = unit_y;
                }
                else  /* !(Path_Length <= 1) */
                {
                    DLOG("(Path_Length > 1)");
                    // TODO(JimBalcomb,20231016):  ¿ why the `[bx-2]` ? indexing lo-bye of word, but array is DB? so, sizeof() pointer data-type?
                    // mov     bx, [bp+Path_Length];  mov     al, OVL_Path_Xs[bx-2];  cbw;  mov     [OVL_Action_OriginX], ax
                    // ... mov     al, [bx-3922h] ...
                    
                    // mov     bx, [bp+Path_Length];  mov     al, OVL_Path_Ys[bx-2];  cbw;  mov     [OVL_Action_OriginY], ax
                    // ... mov     al, [bx-399Ah] ...
                    
                    // mov     bx, [bp+Path_Length];  mov     al, [Scd_Dst_Y+bx];     cbw;  mov     [bp+destination_y], ax
                    // ... mov     al, [bx-3999h] ...
                    // mov     al, [(IDK_MovePath_Y+1)+bx]

                    // // // OVL_Action_OriginX = Fst_Dst_X[Path_Length];
                    // // // OVL_Action_OriginY = Fst_Dst_Y[Path_Length];
                    // // OVL_Action_OriginX = *(Fst_Dst_X + Path_Length);
                    // // OVL_Action_OriginY = *(Fst_Dst_Y + Path_Length);
                    // OVL_Action_OriginX = *(((uint8_t *)(&Fst_Dst_X)) + 1 + Path_Length);
                    // OVL_Action_OriginY = *(((uint8_t *)(&Fst_Dst_Y)) + 1 + Path_Length);
                    OVL_Action_OriginX = MovePath_X[(Path_Length - 1)];
                    OVL_Action_OriginY = MovePath_Y[(Path_Length - 1)];
                }

                if(Path_Length <= 0)
                {
                    DLOG("(Path_Length <= 1)");
                    Total_Move_Cost = 0;
                    Out_Of_Moves = ST_FALSE;
                }
                else  /* !(Path_Length <= 0) */
                {
                    DLOG("(Path_Length > 0)");
                    Move_Units_Draw(player_idx, map_p, Path_Length, map_x, map_y, unit_array, unit_array_count);
                }

                // TODO  if( (OVL_SWardTriggered == ST_TRUE) && (player_idx == _human_player_idx) )
                // TODO  {
                // TODO      OVL_SpellWardError();
                // TODO  }

                /*
                    BEGIN:  ¿ if STK_EvaluatePath() determined that the move(s) is going to result in an encounter / combat ?
                */
                if( (Combat_Move == ST_TRUE) || (UU_flag_FALSE == ST_TRUE) )
                {

                }
                /*
                    END:  ¿ if STK_EvaluatePath() determined that the move(s) is going to result in an encounter / combat ?
                */



                /*
                    HERE:
                    after YayNayMay Move_Units_Draw()
                    after YayNay Combat_Move
                    ¿¿¿
                    still inside movement_points > 0 && movement_path_length > 0
                    ???
                */

                OVL_Action_XPos = -1;
                OVL_Action_YPos = -1;

                Units_In_Tower(unit_array_count, unit_array, map_p);
                // ; checks if the stack is in a Tower of Wizardry; sets
                // ; the In_Tower flags and explores both maps at the
                // ; location if they are, or clears the flag and pulls
                // ; all of the units to the specified plane if not
                // ; considers Planar Seal
                // ;
                // ; BUG: ignores scouting ranges

                for(itr_units = 0; itr_units < unit_array_count; itr_units++)
                {
                    unit_idx = unit_array[itr_units];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _unit_idx: %d\n", __FILE__, __LINE__, unit_idx);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].Finished: %d\n", __FILE__, __LINE__, unit_idx, _UNITS[unit_idx].Finished);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].Status: %d\n", __FILE__, __LINE__, unit_idx, _UNITS[unit_idx].Status);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].HMoves: %d\n", __FILE__, __LINE__, unit_idx, _UNITS[unit_idx].HMoves);
#endif

                    if(_UNITS[unit_idx].Rd_Constr_Left == 99)  /* ¿ Cancelled ? */
                    {
                        _UNITS[unit_idx].Rd_Constr_Left = -1;  /* ¿ NONE ? */
                    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: (_UNITS[%d].Finished == ST_FALSE): %d\n", __FILE__, __LINE__, unit_idx, (_UNITS[unit_idx].Finished == ST_FALSE));
    dbg_prn("DEBUG: [%s, %d]: ((_UNITS[%d].Status & PATROL) == 0): %d\n", __FILE__, __LINE__, unit_idx, ((_UNITS[unit_idx].Status & PATROL) == 0));
    dbg_prn("DEBUG: [%s, %d]: _UNITS[%d].HMoves: %d\n", __FILE__, __LINE__, unit_idx, _UNITS[unit_idx].HMoves);
#endif
                    // MoX_Data.H  enum Unit_Status  US_Patrol  2
                    // if( (_UNITS[unit_idx].Finished == ST_FALSE) && ((_UNITS[unit_idx].Status & PATROL) != 0) )
                    // ¿ Nay Finished && Nay Patrol ?
                    if( (_UNITS[unit_idx].Finished == ST_FALSE) && ((_UNITS[unit_idx].Status & PATROL) == 0) )
                    {
                        DLOG("( (_UNITS[unit_idx].Finished == ST_FALSE) && ((_UNITS[unit_idx].Status & PATROL) == 0) )");
                        _UNITS[unit_idx].HMoves = _UNITS[unit_idx].HMoves - Total_Move_Cost;

                        if(_UNITS[unit_idx].HMoves < 1)
                        {
                            _UNITS[unit_idx].Finished = ST_TRUE; 
                            _UNITS[unit_idx].HMoves = 0;
                        }

                        if(
                            (_UNITS[unit_idx].Status != US_GoingTo) &&
                            (Combat_Move == ST_TRUE) &&
                            (_UNITS[unit_idx].owner_idx == _human_player_idx)
                        )
                        {
                            _UNITS[unit_idx].Status = 0x00 /* "NO ORDERS"  US_Ready */;
                            _UNITS[unit_idx].dst_wx = 0;
                            _UNITS[unit_idx].dst_wy = 0;
                            Out_Of_Moves = ST_FALSE;
                        }
                        
                        if( (Out_Of_Moves == ST_TRUE) && ((_UNITS[unit_idx].Status & 0x10 /* US_Move */) != 0) )
                        {
                            _UNITS[unit_idx].Status = 0x03;  /* "GOTO"  US_GoingTo */
                            _UNITS[unit_idx].dst_wx = destination_x;
                            _UNITS[unit_idx].dst_wy = destination_y;
                            _UNITS[unit_idx].Finished = ST_TRUE;
                        }

                        if(
                            ( (_UNITS[unit_idx].Status & 0x03 /* US_GoingTo */) == 0) &&
                            ( _UNITS[unit_idx].world_x == _UNITS[unit_idx].dst_wx ) &&
                            ( _UNITS[unit_idx].world_y == _UNITS[unit_idx].dst_wy )
                        )
                        {
                            // ¿ logic to land here ?

                            if(_UNITS[unit_idx].Rd_Constr_Left == -1)
                            {
                                _UNITS[unit_idx].Status = 0x00;  /* "NO ORDERS"  US_Ready */
                                _UNITS[unit_idx].dst_wx = 0;
                                _UNITS[unit_idx].dst_wy = 0;
                                if(_UNITS[unit_idx].HMoves > 0)
                                {
                                    _UNITS[unit_idx].Finished = ST_FALSE;
                                }
                            }

                            Out_Of_Moves = ST_FALSE;

                        }

                        if( (_UNITS[unit_idx].Status & 0x03 /* US_GoingTo */) != 0)
                        {
                            _UNITS[unit_idx].Finished = ST_TRUE;
                        }

                    }
                    else
                    {
                        DLOG("( (_UNITS[unit_idx].Finished != ST_FALSE) || ((_UNITS[unit_idx].Status & PATROL) != 0) )");
                    }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: (_UNITS[unit_idx].Finished == ST_FALSE): %d\n", __FILE__, __LINE__, (_UNITS[unit_idx].Finished == ST_FALSE));
    dbg_prn("DEBUG: [%s, %d]: ((_UNITS[unit_idx].Status & PATROL) != 0): %d\n", __FILE__, __LINE__, ((_UNITS[unit_idx].Status & PATROL) != 0));
    dbg_prn("DEBUG: [%s, %d]: _UNITS[unit_idx].HMoves: %d\n", __FILE__, __LINE__, _UNITS[unit_idx].HMoves);
#endif


                }  /* for(itr_units = 0; itr_units < unit_array_count; itr_units++) */


// push    [bp+unit_array@]
// push    [bp+unit_array_count]           ; unit_array_count
// nop
// push    cs
// call    near ptr G_STK_SetPatrol        ; sets all non-transport, non-dead units in the stack
//                                         ; to patrol
//                                         ;
//                                         ; why?
// 
// pop     cx
// pop     cx
// 
// 
// call    j_Reset_Draw_Active_Stack       ; sets the overland map drawing to renew everything on
//                                         ; its next refresh, and if there is a stack selected,
//                                         ; resets its display to the normal blinking behaviour,
//                                         ; starting with the first four "off" states
// 
// 
// 
// mov     ax, [bp+player_idx]
// cmp     ax, [_human_player_idx]         ; index of the human player (special rules apply)
// jnz     short @@JmpDone_Return_TRUE
// 
// call    j_Update_Scouted_And_Contacted  ; recalculates the visibility arrays for both planes
//                                         ; after clearing them entirely, and marks contacted
//                                         ; players accordingly if they haven't been already






            }
            else  /* !(Path_Length >= 1) */
            {
                DLOG("(Path_Length < 1)");
                goto Done_Return_FALSE;
            }



        }


    }
    else  /* !(movement_points >= 1) */
    {
        DLOG("(movement_points < 1)");
        goto Done_Return_FALSE;
    }

Done_Return_FALSE:
    Reset_Draw_Active_Stack();
    return_value = ST_FALSE;
    goto Done;

Done_Return_TRUE:
    return_value = ST_TRUE;
    goto Done;

Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Move_Units(player_idx = %d, destination_x = %d, destination_y = %d, Spec = %d, *map_x = %d, *map_y = %d, map_p = %d, unit_array_count = %d, &unit_array[0] = %p) { return_value = %d }\n", __FILE__, __LINE__, player_idx, destination_x, destination_y, Spec, *map_x, *map_y, map_p, unit_array_count, &unit_array[0], return_value);
#endif

    return return_value;
}

// WZD o95p02
// AKA OVL_MoveUnitStack()
// AKA Move_Units()
void Move_Units_Draw(int16_t player_idx, int16_t map_p, int16_t Path_Length, int16_t * map_x, int16_t * map_y, int16_t unit_array[], int16_t unit_array_count)
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Move_Units_Draw(player_idx = %d, map_p = %d, Path_Length = %d, *map_x = %d, *map_y = %d, &unit_array[0] = %p, unit_array_count = %d)\n", __FILE__, __LINE__, player_idx, map_p, Path_Length, *map_x, *map_y, &unit_array[0], unit_array_count);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: unit_array_count: %d\n", __FILE__, __LINE__, unit_array_count);
    for(itr_units = 0; itr_units < unit_array_count; itr_units++)
    {
        dbg_prn("DEBUG: [%s, %d]: unit_array[%d]: %d\n", __FILE__, __LINE__, itr_units, unit_array[itr_units]);
    }
#endif

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
        for(itr_path_length = 0; (itr_path_length < Path_Length) && (display_moves == ST_FALSE); itr_path_length++)
        {
            // TODO  figure out the paths arrays  if(Check_Square_Scouted(OVL_Path_Xs[itr_path_length], OVL_Path_Ys[itr_path_length]) == ST_TRUE)
            if(Check_Square_Scouted(MovePath_X[itr_path_length], MovePath_Y[itr_path_length], map_p) == ST_TRUE)
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
        DLOG("(display_moves == ST_TRUE)");
        OVL_BringIntoView(map_x, map_y, _UNITS[unit_idx].world_x, _UNITS[unit_idx].world_y, map_p);
        _map_x = *map_x;
        _map_y = *map_y;
        Set_Unit_Draw_Priority();
        Set_Entities_On_Map_Window(*map_x, *map_y, map_p);
        Reset_Map_Draw();
        MainScr_Prepare_Reduced_Map();
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
    unit_idx = Highest_Priority_Unit__Loop_Var;
    unit_x = _UNITS[unit_idx].world_x;
    unit_y = _UNITS[unit_idx].world_y;

// IDGI  // TODO: ¿ why the OVL_Path_Xs-1[bx] ? getting the hi-byte of a word? casting? because the sizeof() OVL_Path_Xs[] is DW?
// IDGI  // mov     bx, [bp+Path_Length]
// IDGI  // mov     al, OVL_Path_Xs-1[bx]
// IDGI  // cbw
// IDGI  // mov     [bp+destination_x], ax
// IDGI  // mov     bx, [bp+Path_Length]
// IDGI  // mov     al, OVL_Path_Ys-1[bx]
// IDGI  // cbw
// IDGI  // mov     [bp+destination_y], ax
// IDGI      // destination_x = Scd_Dst_X[Path_Length];  // TODO  IDK_MovePath_DestinationX[] vs. OVL_Path_Xs[]
// IDGI      // destination_y = Scd_Dst_Y[Path_Length];  // TODO  IDK_MovePath_DestinationY[] vs. OVL_Path_Ys[]
// IDGI      destination_x = *(((uint8_t *)(&Scd_Dst_X)) + 1 + Path_Length);
// IDGI      destination_y = *(((uint8_t *)(&Scd_Dst_Y)) + 1 + Path_Length);
    destination_x = MovePath_X[(Path_Length - 1)];
    destination_y = MovePath_Y[(Path_Length - 1)];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: destination_x: %d\n", __FILE__, __LINE__, destination_x);
    dbg_prn("DEBUG: [%s, %d]: destination_y: %d\n", __FILE__, __LINE__, destination_y);
#endif

    for(itr_units = 0; itr_units < unit_array_count; itr_units++)
    {
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _UNITS[unit_array[itr_units]].world_x: %d\n", __FILE__, __LINE__, _UNITS[unit_array[itr_units]].world_x);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[unit_array[itr_units]].world_y: %d\n", __FILE__, __LINE__, _UNITS[unit_array[itr_units]].world_y);
#endif
        _UNITS[unit_array[itr_units]].world_x = destination_x;
        _UNITS[unit_array[itr_units]].world_y = destination_y;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _UNITS[unit_array[itr_units]].world_x: %d\n", __FILE__, __LINE__, _UNITS[unit_array[itr_units]].world_x);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[unit_array[itr_units]].world_y: %d\n", __FILE__, __LINE__, _UNITS[unit_array[itr_units]].world_y);
#endif
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
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: curr_src_wx: %d\n", __FILE__, __LINE__, curr_src_wx);
    dbg_prn("DEBUG: [%s, %d]: curr_src_wy: %d\n", __FILE__, __LINE__, curr_src_wy);
#endif


    if(Building_Road == ST_TRUE)
    {
        // TODO  TILE_CreateRoad(unit_x, unit_y, map_p);
        // TODO  TILE_ResetRoadConns(unit_x, unit_y, map_p);
    }



    for(itr_path_length = 0; itr_path_length < Path_Length; itr_path_length++)
    {
        curr_dst_wx = MovePath_X[itr_path_length];
        curr_dst_wy = MovePath_Y[itr_path_length];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: curr_dst_wx: %d\n", __FILE__, __LINE__, curr_dst_wx);
    dbg_prn("DEBUG: [%s, %d]: curr_dst_wy: %d\n", __FILE__, __LINE__, curr_dst_wy);
#endif


        unit_pict_shift_sx = 0;
        if(curr_dst_wx > unit_x)  /* ¿ moving to the right ? */
        {

            if(unit_x == 0 && curr_dst_wx == WORLD_X_MAX)
            {
                unit_pict_shift_sx = -4;  // MAP_WIDTH / 3
                unit_x--;
                if(unit_x == 0)
                {
                    unit_x = WORLD_X_MAX;
                }
            }
            else
            {
                unit_pict_shift_sx = 4;  // MAP_WIDTH / 3
                unit_x++;
                if(unit_x == WORLD_WIDTH)
                {
                    unit_x = 0;
                }
            }
        }
        else  /* (curr_dst_wx <= unit_x)  ¿ moving to the left ? */
        {
            if(unit_x == WORLD_X_MAX && curr_dst_wx == 0)
            {
                unit_pict_shift_sx = 4;  // MAP_WIDTH / 3
                unit_x++;
                if(unit_x == WORLD_WIDTH)
                {
                    unit_x = 0;
                }
            }
            else
            {
                unit_pict_shift_sx = -4;  // MAP_WIDTH / 3
                unit_x--;
                if(unit_x == 0)
                {
                    unit_x = WORLD_X_MAX;
                }
            }
        }

        unit_pict_shift_sy = 0;
        if(curr_dst_wy > unit_y)
        {
            unit_pict_shift_sy = 3;  // MAP_HEIGHT / 3
            unit_y++;
        }
        else
        {
            unit_pict_shift_sy = -3;  // MAP_HEIGHT / 3
            unit_y--;
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

        if(Move_Stages != 0)
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
            if(OVL_TileOffScrnEdge(*map_x, *map_y, unit_x, unit_y, MAP_WIDTH, MAP_HEIGHT) == ST_TRUE)
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
                    // TODO  TILE_ExploreRadius(unit_x, unit_y, map_p, scout_range);
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
                    // TODO  TILE_ExploreRadius(unit_x, unit_y, map_p, scout_range);
                    MainScr_Prepare_Reduced_Map();
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
                // Redraw_Map_Unexplored_Area(0, 20, MAP_WIDTH, MAP_HEIGHT, *map_x, *map_y, map_p);
                // ; redraws the unscouted area coverage into the current
                // ; draw segment based on the specified map parameters
                // ; this is always a full draw, unlike OVL_DrawScouting,
                // ; and will refresh the black rectangles covering
                // ; unscouted tiles in addition to drawing the edges

                PageFlip_FX();

            }

        }

        // ; if the units on the tile scouted another wizard's
        // ; unit or city, establishes contact between the two
        // ; wizards, provided that they are both AI
        // ; returns the index of the unit that was spotted if any
        // TOOD  AI_ContactWizards(player_idx, unit_x, unit_y, map_p, scout_range);

        curr_src_wx = MovePath_X[itr_path_length];
        curr_src_wy = MovePath_Y[itr_path_length];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: curr_src_wx: %d\n", __FILE__, __LINE__, curr_src_wx);
    dbg_prn("DEBUG: [%s, %d]: curr_src_wy: %d\n", __FILE__, __LINE__, curr_src_wy);
#endif

    }  /* for(itr_path_length = 0; itr_path_length < Path_Length; itr_path_length++) */


    Reset_Window();
    Reset_Map_Draw();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Move_Units_Draw(player_idx = %d, map_p = %d, Path_Length = %d, *map_x = %d, *map_y = %d, &unit_array[0] = %p, unit_array_count = %d)\n", __FILE__, __LINE__, player_idx, map_p, Path_Length, *map_x, *map_y, &unit_array[0], unit_array_count);
#endif

}

// WZD o95p03
// AI_ContactWizards()

// WZD o95p04
// G_STK_OvlObstacles()

// WZD o95p05
int16_t STK_GetLandlubbers(int16_t Stack_Size, int16_t Stack_Array[], int16_t LL_Array[])
{
    int16_t Landlubber_Count;

    int16_t itr_Stack_Size;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: STK_GetLandlubbers()\n", __FILE__, __LINE__);
#endif

    Landlubber_Count = 0;

    for(itr_Stack_Size = 0; itr_Stack_Size < Stack_Size; itr_Stack_Size++)
    {
        // drake178: returns 1 if the unit has wind walking, whether natural or through an enchantment; or 0 otherwise
        // WZD o71p04
        if(Unit_Has_WindWalking(Stack_Array[itr_Stack_Size]) == ST_TRUE)
        {
            goto Return_Zero;
        }
// DONT          // drake178: does nothing and returns zero, at some point must have been a unit ability or enchantment check
// DONT          // WZD o71p08
// DONT          if(UNIT_ReturnZero(*Stack_Array[itr_Stack_Size]) == ST_TRUE)
// DONT          {
// DONT              return 0;
// DONT          }
        // WZD o71p03
        if(Unit_Has_AirTravel(Stack_Array[itr_Stack_Size]) == ST_TRUE)
        {
            continue;
        }
        // WZD o71p05
        if(Unit_Has_WaterTravel(Stack_Array[itr_Stack_Size]) == ST_TRUE)
        {
            continue;
        }
        // WZD o71p010
        if(Unit_Has_AirTravel_Item(Stack_Array[itr_Stack_Size]) == ST_TRUE)
        {
            continue;
        }
        // WZD o71p09
        if(Unit_Has_WaterTravel_Item(Stack_Array[itr_Stack_Size]) == ST_TRUE)
        {
            continue;
        }

        LL_Array[Landlubber_Count] = Stack_Array[itr_Stack_Size];
        Landlubber_Count++;
    }
    goto Return_Count;

Return_Zero:
    Landlubber_Count = 0;
    goto Done;

Return_Count:
    goto Done;

Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: STK_GetLandlubbers()\n", __FILE__, __LINE__);
#endif

    return Landlubber_Count;
}

// WZD o95p06
// G_STK_SetPatrol()



/*
    WIZARDS.EXE  ovr097
*/

// WZD o97p01
// STK_EvaluatePath()

// WZD o97p02
// sub_7E597()

// WZD o97p03
// drake178: TILE_HasCity()
int16_t Square_Has_City(int16_t world_x, int16_t world_y, int16_t map_plane)
{
    int16_t square_has_city;
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Square_Has_City(world_x = %d, world_y = %d, map_plane = %d)\n", __FILE__, __LINE__, world_x, world_y, map_plane);
#endif

    square_has_city = ST_UNDEFINED;

    for(itr = 0; itr < _cities && square_has_city == ST_UNDEFINED; itr++)
    {
        if( (_CITIES[itr].world_plane == map_plane) && (_CITIES[itr].world_x = world_x) && (_CITIES[itr].world_y == world_y) )
        {
            square_has_city = itr;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Square_Has_City(world_x = %d, world_y = %d, map_plane = %d)\n", __FILE__, __LINE__, world_x, world_y, map_plane);
#endif
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

    int16_t movement_points; // In Dasm, SI
    int16_t unit_idx;  // In Dasm, DI

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Units_Moves(&unit_array[0] = %p, unit_array_count = %d)\n", __FILE__, __LINE__, &unit_array[0], unit_array_count);
#endif

    movement_points = 1000;

    for(itr_unit_array_count = 0; itr_unit_array_count < _unit_stack_count; itr_unit_array_count++)
    {
        unit_idx = _unit_stack[itr_unit_array_count].unit_idx;
        
        if(_UNITS[unit_idx].HMoves < movement_points)
        {
            movement_points = _UNITS[unit_idx].HMoves;
        }

        if(
            (_unit_type_table[_UNITS[unit_idx].type].Transport > 0) ||
            (Unit_Has_WindWalking(unit_idx) == ST_TRUE)
        )
        {
            movement_points = _UNITS[unit_idx].HMoves;
            break;
        }

    }

    if(movement_points == 1000)
    {
        movement_points = 0;
    }

    stack_moves = movement_points;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Units_Moves(&unit_array[0] = %p, unit_array_count = %d) { stack_moves = %d }\n", __FILE__, __LINE__, &unit_array[0], unit_array_count, stack_moves);
#endif

    return stack_moves;

}


// WZD o97p05
// drake178: RP_CTY_CheckSpellWard()
/*
    determines if Stack is prohibited from entering the City due to Spell Ward
    returns {ST_FALSE,ST_TRUE}
*/
int16_t RP_CTY_CheckSpellWard__STUB(int16_t city_idx, int16_t * stack_size, int16_t * stack_array)
{
// Unused_Array= word ptr -1Ch
// Cant_Enter= word ptr -0Ah
// Retn_Value= word ptr -8
// Unused_Local= word ptr -6
// Stack_Size= word ptr -4
// Unit_Owner= word ptr -2
    int16_t city_has_spellward;

#ifdef STU_DEBUG
    // dbg_prn("DEBUG: [%s, %d]: BEGIN: RP_CTY_CheckSpellWard(city_idx = %d, stack_size = %d, stack_array = %d)\n", __FILE__, __LINE__, city_idx, stack_size, stack_array);
    dbg_prn("DEBUG: [%s, %d]: BEGIN: RP_CTY_CheckSpellWard__STUB(city_idx = %d)\n", __FILE__, __LINE__, city_idx);
#endif

    city_has_spellward = ST_FALSE;



    // TODO  CTY_CheckSpellWard(int16_t city_idx, int16_t * stack_size, int16_t * stack_array)



#ifdef STU_DEBUG
    // dbg_prn("DEBUG: [%s, %d]: END: RP_CTY_CheckSpellWard(city_idx = %d, stack_size = %d, stack_array = %d)\n", __FILE__, __LINE__, city_idx, stack_size, stack_array);
    dbg_prn("DEBUG: [%s, %d]: END: RP_CTY_CheckSpellWard__STUB(city_idx = %d) { city_has_spellward = %d }\n", __FILE__, __LINE__, city_idx, city_has_spellward);
#endif

    return city_has_spellward;
}


// WZD o97p06
// drake178: CTY_CheckSpellWard()
int16_t CTY_CheckSpellWard__STUB(int16_t city_idx, int16_t * stack_size, int16_t * stack_array)
{
    int16_t city_has_spellward;
#ifdef STU_DEBUG
    // dbg_prn("DEBUG: [%s, %d]: BEGIN: CTY_CheckSpellWard(city_idx = %d, stack_size = %d, stack_array = %d)\n", __FILE__, __LINE__, city_idx, stack_size, stack_array);
    dbg_prn("DEBUG: [%s, %d]: BEGIN: CTY_CheckSpellWard__STUB(city_idx = %d)\n", __FILE__, __LINE__, city_idx);
#endif

    city_has_spellward = ST_FALSE;

#ifdef STU_DEBUG
    // dbg_prn("DEBUG: [%s, %d]: END: CTY_CheckSpellWard(city_idx = %d, stack_size = %d, stack_array = %d)\n", __FILE__, __LINE__, city_idx, stack_size, stack_array);
    dbg_prn("DEBUG: [%s, %d]: END: CTY_CheckSpellWard__STUB(city_idx = %d) { city_has_spellward = %d }\n", __FILE__, __LINE__, city_idx, city_has_spellward);
#endif
    return city_has_spellward;
}


// WZD o97p07
// OVL_SpellWardError()


// WZD o97p08
// drake178: VGA_DrawHalfValue()
// OON XREF: j_VGA_DrawHalfValue()
// Main_Screen_Draw_Movement_Bar
// Main_Screen_Draw_Movement_Bar
// Main_Screen_Draw_Movement_Bar
// CMB_DrawAUWStats+10F         
// CMB_DrawUnitDisplay+278      
void Print_Moves_String(int16_t x_start, int16_t y_start, int16_t half_value, int16_t right_align_flag)
{
    char buffer[8];
    char string[8];
    int16_t whole_value;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Moves_String(x_start = %d, y_start = %d, half_value = %d, right_align_flag = %d)\n", __FILE__, __LINE__, x_start, y_start, half_value, right_align_flag);
#endif

    if(half_value < 0)
    {
        DLOG("(half_value < 0)");
        whole_value = 0;
    }
    else
    {
        DLOG("(half_value >= 0)");
        whole_value = half_value / 2;
    }
    
    strcpy(string, "");  // WZD dseg:569A cnst_Nullstring[] = {'\0'}
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: string: %s\n", __FILE__, __LINE__, string);
#endif

    if(whole_value > 0)
    {
        DLOG("(whole_value > 0)");
        itoa(whole_value, buffer, 10);
        strcat(string, buffer);
    }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: string: %s\n", __FILE__, __LINE__, string);
#endif

    if(half_value % 2 != 0)
    {
        DLOG("(half_value % 2 != 0)");
        strcat(string, ".5");  // WZD dseg:56C9 cnst_Half_2[] = ".5"
    }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: string: %s\n", __FILE__, __LINE__, string);
#endif

    if(right_align_flag == ST_FALSE)
    {
        Print(x_start, y_start, string);
    }
    else  /* ¿ WTF:  else if(right_align_flag == ST_TRUE) ? */
    {
        Print_Right(x_start, y_start, string);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Print_Moves_String(x_start = %d, y_start = %d, half_value = %d, right_align_flag = %d)\n", __FILE__, __LINE__, x_start, y_start, half_value, right_align_flag);
#endif
}
