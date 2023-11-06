#ifndef MAINSCR_H
#define MAINSCR_H


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
*/



// WZD  dseg:31AE
// char cnst_Moves[]  = "Moves:";





// WZD dseg:974E 
// int16_t _main_map_grid_x;
extern int64_t _main_map_grid_x;
// WZD dseg:9750 
// int16_t _main_map_grid_y;
extern int64_t _main_map_grid_y;





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
// int16_t special_action_flag;  // AKA OVL_ExtraUnitAction
// WZD dseg:C050
// int16_t _next_turn_button;
// WZD dseg:C052
// int16_t Unit_Window_Fields[9];
// WZD dseg:C064
// CRP_OverlandVar_3 dw 0
// WZD dseg:C066
// int16_t OVL_Path_Length;
// WZD dseg:C068
extern int16_t OVL_StackHasPath;





/*
                                        ¿ BEGIN: ovr052 Terrain_Init() ?
*/
// WZD dseg:CB5E                                                 ovr052

// WZD dseg:CB5E
extern SAMB_ptr node_warped_seg;                // ; reserved EMM header pointer for a single image

// WZD dseg:CB60
// AKA gsa_OVL_Tile_WorkArea
extern SAMB_ptr Map_Square_WorkArea;              // alloc in Terrain_Init  // ; 70 * 16 = 1120 bytes


// WZD dseg:CB62
// AKA UU_IMG_OVL_Empty3
extern SAMB_ptr UU_hunters_lodge_seg;                 // ; single-loaded image, called "hunter's lodge"

// WZD dseg:CB64
extern SAMB_ptr IMG_OVL_Nightshade;                // ; reserved EMM header pointer for a single image

// WZD dseg:CB66
extern SAMB_ptr IMG_OVL_WildGame;                  // ; reserved EMM header pointer for a single image

// WZD dseg:CB68 00 00                                           UU_IMG_OVL_Empty1@ dw 0                 ; DATA XREF: Terrain_Init:loc_46318w
// WZD dseg:CB68                                                                                         ; single-loaded image, called "mine" in the file

// WZD dseg:CB6A
SAMB_ptr IMG_OVL_Corruption;                        // ; reserved EMM header pointer for a single image

// WZD dseg:CB6C 00 00                                           UU_IMG_OVL_Mud@ dw 0                    ; DATA XREF: Terrain_Init+1F6w
// WZD dseg:CB6C                                                                                         ; reserved EMM header pointer (unused in the game?)

// WZD dseg:CB6E
/*
    load in Terrain_Init()
    MAPBACK.LBX  78-86
    SITES - coal, iron, silver, gold, gems, mithril, adamantium, quork, crysx
*/
extern SAMB_ptr mineral_site_segs[];

// WZD dseg:CB80 00                                              db    0
// WZD dseg:CB81 00                                              db    0

// WZD dseg:CB82 
extern SAMB_ptr node_auras_seg[5];               // ; array of 5 reserved EMM header pointers for  ; 6 frame animations

// WZD dseg:CB8C 00                                              db    0
// WZD dseg:CB8D 00                                              db    0
// WZD dseg:CB8E 00 00                                           UU_IMG_OVL_Empty2@ dw 0                 ; DATA XREF: Terrain_Init+22Cw
// WZD dseg:CB8E                                                                                         ; single-loaded image, called "lumber camp" in the file

// WZD dseg:CB90
extern SAMB_ptr Warp_Node_WorkArea;                // ; used to save and manipulate the tile graphics to ; display the warp node effect

// WZD dseg:CB92
extern SAMB_ptr _unit_colored_backgrounds_seg[];    // load in Terrain_Init() ovr052

// WZD dseg:CB92                                                                                         ; DATA XREF: Terrain_Init+F3w ...
// WZD dseg:CB92                                                                                         ; array of 6 reserved EMM header pointers
// WZD dseg:CB92                                                                                         ; (1 per banner color)
// WZD dseg:CB9E 00                                              db    0
// WZD dseg:CB9F 00                                              db    0

// WZD dseg:CBA0
extern SAMB_ptr eroads_seg[];               // ; array of 8 reserved EMM header pointers  ; for 6 frame animations  ; clockwise 1-connection e.roads starting up
// WZD dseg:CBB0
extern SAMB_ptr eroad_lone_seg;             // ; reserved EMM header pointer for a 6 frame animation  ; no connection enchanted road
// WZD dseg:CBB0
extern SAMB_ptr roads_seg[];                // ; array of 8 reserved EMM header pointers  ; clockwise 1-connection roads starting up
// WZD dseg:CBB2
extern SAMB_ptr road_lone_seg;              // ; reserved EMM header pointer for a single image  ; no connection normal road

// WZD dseg:CBC4
extern SAMB_ptr IMG_OVL_EZ_Cave;            // ; reserved EMM header pointer for a single image
// WZD dseg:CBC6
extern SAMB_ptr IMG_OVL_EZ_Dung;                   // ; reserved EMM header pointer for a single image
// WZD dseg:CBC8
extern SAMB_ptr IMG_OVL_EZ_ATmpl;                   // ; reserved EMM header pointer for a single image
// WZD dseg:CBCA
extern SAMB_ptr IMG_OVL_EZ_AKeep;                   // ; reserved EMM header pointer for a single image
// WZD dseg:CBCC
extern SAMB_ptr IMG_OVL_EZ_MLair;                   // ; reserved EMM header pointer for a single image
// WZD dseg:CBCE
extern SAMB_ptr IMG_OVL_EZ_Ruins;                   // ; reserved EMM header pointer for a single image
// WZD dseg:CBD0
extern SAMB_ptr IMG_OVL_EZ_FTmpl;                   // ; reserved EMM header pointer for a single image

// WZD dseg:CBD2 00 00                                           word_43672 dw 0

// WZD dseg:CBD4
extern SAMB_ptr tower_unowned_seg;                 // ; reserved EMM header pointer for a single image

// WZD dseg:CBD6
extern SAMB_ptr tower_owned_seg;                   // ; reserved EMM header pointer for a single image

// WZD dseg:CBD8
extern SAMB_ptr IMG_OVL_Walled_City;               // ; reserved EMM header pointer for a 5 frame image

// WZD dseg:CBDA
extern SAMB_ptr IMG_OVL_NoWall_City;               // ; reserved EMM header pointer for a 5 frame image

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
extern SAMB_ptr unexplored_mask_seg[];       // ; array of 14 reserved EMM header pointers

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




#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


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

/*
    WIZARDS.EXE  ovr057
*/
// WZD o57p01
/* PUBLIC */  void Main_Screen(void);
// WZD o57p02
// PRIVATE  void Main_Screen_Add_Fields(void);
// WZD o57p03
// PRIVATE  void Add_Unit_Action_Fields(void);
// WZD o57p04
// AKA j_OVL_DrawUnitActnBtns()
// PRIVATE  void Main_Screen_Draw_Unit_Action_Buttons(void);
// WZD o57p05
// PRIVATE  void Add_Game_Button_Fields(void);
// WZD o57p06
// PRIVATE  void Main_Screen_Draw_Game_Buttons(void);
// WZD o57p07
// PRIVATE  void Add_Unit_Window_Fields(void);
// WZD o57p08
// called by `Spellbook_Screen()`, to create its background
void Main_Screen_Draw(void);
// WZD o57p09
// PRIVATE  void Main_Screen_Reset(void);
// WZD o57p10
// AKA Do_Move_Stack_DirKey()
// PRIVATE  void Move_Stack_DirKey(int16_t movement_direction);

/*
    WIZARDS.EXE  ovr058
*/
// WZD o58p01
// PRIVATE  void Main_Screen_Draw_Unit_Window(int16_t start_x, int16_t start_y);

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
// ¿PRIVATE?  void Units_In_Tower(int16_t unit_array_count, int16_t unit_array[], int16_t map_p);

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
// AKA IDK_ActiveUnitStack_MovesOrPath_s53150()
// UNITSTK  void Update_Stack_Active(int16_t stack_idx);

// WZD o61p02
// AKA Do_Move_Stack()
// ¿PRIVATE?  int16_t Move_Stack(int16_t move_x, int16_t move_y, int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p);

// WZD o61p03
// UNIT_MoveStack()
// ¿PRIVATE?  int16_t RdBd_UNIT_MoveStack_STUB(int16_t player_idx, int16_t unit_idx, int16_t destination_world_x, int16_t destination_world_y, int16_t * map_x, int16_t * map_y, int16_t map_p)

// WZD o61p04
void WIZ_NextIdleStack(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p);

// WZD o61p05
// ¿PRIVATE?  int16_t WIZ_NextUnit(int16_t player_idx, int16_t * map_plane);

// WZD o61p06
// ¿PRIVATE?  
void STK_GetExtraActions(void);

// WZD o61p07
// drake178: STK_GetMovableUnits()
// ¿PRIVATE?  
void Active_UnitGroup(int16_t * unit_array_count, int16_t unit_array[]);

// WZD o61p08
void Build_Moveable_Stack(int16_t * unit_count, int16_t unit_array[]);

// WZD o61p09
// ¿PRIVATE?  void RdBd_UNIT_SelectStack_STUB(int16_t & unit_array_count, int16_t unit_array[], int16_t wx, int16_t wy, int16_t player_idx, int16_t unit_idx);

// WZD o61p10
// WZD o61p11
// WZD o61p12
// WZD o61p13
// WZD o61p14
// WZD o61p15

// WZD o61p16
// ¿PRIVATE?  int16_t Any_Units_Not_Busy();



/*
    WIZARDS.EXE  ovr062
*/

// WZD o62p01
// DONT  int16_t o62p01_Empty_pFxn(int16_t player_idx)

// WZD o62p02
// AKA OVL_StackSelect()
void Select_Unit_Stack(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t unit_x, int16_t unit_y);


// WZD o62p03
// drake178: STK_DisplaySort
// OON XREF: Select_Unit_Stack()
// PRIVATE  void Sort_Unit_Stack(void);

// WZD o62p04
// PRIVATE  void Build_Unit_Stack(int16_t player_idx, int16_t world_plane, int16_t world_x, int16_t world_y);

// WZD o62p05
// PRIVATE void OVL_BringIntoView(int16_t *map_x, int16_t *map_y, int16_t unit_x, int16_t unit_y, int16_t map_plane)

// WZD o62p06
// drake178: OVL_TileOffScrnEdge()
// PRIVATE int16_t OVL_TileOffScrnEdge(int16_t map_x, int16_t map_y, int16_t unit_x, int16_t unit_y, int16_t map_width, int16_t map_height)

// WZD o62p07
// DONT  int16_t o62p07_Empty_pFxn(int16_t unit_array_count, int16_t * unit_array);

// WZD o62p08
// ¿PRIVATE?  void Stack_Action(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p, int16_t action, int16_t destination_x, int16_t destination_y);

// WZD o62p09
// EarthGateTeleport()

// WZD o62p10
// USW_FullDisplay()


/*
    WIZARDS.EXE  ovr063
*/
// WZD o63p01
// PRIVATE  void Main_Screen_Draw_Status_Window(void);
// WZD o063p02
// PRIVATE  void Main_Screen_Draw_Do_Draw(int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t x_pos, int16_t y_pos, int16_t player_idx);

// WZD o063p03
void Draw_Unit_Enchantment_Outline(int16_t unit_idx);

// WZD o63p04
// drake178: j_GAME_Animate_UEs()
void Cycle_Unit_Enchantment_Animation(void);

// WZD o63p05
void Draw_Unit_Picture(int16_t x, int16_t y, int16_t unit_idx, int16_t flag);
// called from WZD o67p07 Draw_Active_Unit_Stack()

// WZD o63p05
// AKA OVL_DrawUnitImage()
// AKA Unit_Window_Draw_Unit_StatFig()
void Draw_Unit_StatFig(int16_t x, int16_t y, int16_t unit_idx, int16_t flag);

// WZD o063p06
// PRIVATE  void Unit_Window_Draw_Unit_Attributes(int16_t x, int16_t y, int16_t unit_idx)

// WZD o63p07
// drake178: OVL_DrawStackMoves
void Main_Screen_Draw_Movement_Bar();

// WZD o63p08
// drake178: OVL_DrawMoveTypes()
// void Draw_Movement_Mode_Icons(int16_t x, int16_t y, int16_t unit_idx);

// WZD o63p12
// AKA OVL_GetStackHMoves()
// AKA OVL_GetStackHMoves_1()
// PRIVATE int16_t Stack_Moves_Active(void);

// WZD o63p13
// AKA OVL_GetStackHMoves_2()
// PRIVATE int16_t Stack_Moves(void);

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
void Allocate_Reduced_Map(void);  // called from GAME_Overland_Init()  <-|  WZD_Startup_MainGame()  <-|  _main()
// WZD o64p03
// AKA OVL_PrepMinimap()
void MainScr_Prepare_Reduced_Map(void);
// WZD o64p04
// PRIVATE  void Main_Screen_Draw_Summary_Window(void);
// WZD o64p05
// PRIVATE  void Main_Screen_Draw_Next_Turn_Button(void);
// WZD o64p08
// PRIVATE  void Main_Screen_Draw_Unit_Action_Locked_Buttons(void);
// WZD o64p09
// PRIVATE  void Unit_Window_Picture_Coords(int16_t unit_stack_unit_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2);



/*
    WIZARDS.EXE  ovr095
*/

// WZD o95p01
// AKA Move_Stack()
// ¿PRIVATE? int16_t Move_Units(int16_t player_idx, int16_t destination_x, int16_t destination_y, int16_t Spec, int16_t * map_x, int16_t * map_y, int16_t map_p, int16_t unit_array_count, int16_t unit_array[]);

// WZD o95p02
// AKA OVL_MoveUnitStack()
// AKA Move_Units()
// ¿PRIVATE? void Move_Units_Draw(int16_t player_idx, int16_t map_p, int16_t Path_Length, int16_t * map_x, int16_t * map_y, int16_t unit_array[], int16_t unit_array_count);

// WZD o95p03
// AI_ContactWizards()

// WZD o95p04
// G_STK_OvlObstacles()

// WZD o95p05
// ¿PRIVATE? int16_t STK_GetLandlubbers(int16_t Stack_Size, int16_t Stack_Array[], int16_t LL_Array[]);

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
// PRIVATE?  int16_t Square_Has_City(int16_t world_x, int16_t world_y, int16_t map_plane);

// WZD o97p04
// AKA STK_GetMovesLeft()
// ¿ vs. WZD o63p12  Stack_Moves_Active() ?
// ¿ vs. WZD o63p13  Stack_Moves() ?
// ¿PRIVATE? int16_t Units_Moves(int16_t unit_array[], int16_t unit_array_count);

// WZD o97p05
// RP_CTY_CheckSpellWard()

// WZD o97p06
// CTY_CheckSpellWard()

// WZD o97p07
// OVL_SpellWardError()

// WZD o97p08
// PRIVATE?  void Print_Moves_String(int16_t x_start, int16_t y_start, int16_t half_value, int16_t right_align_flag);



#ifdef __cplusplus
}
#endif



#endif  /* MAINSCR_H */