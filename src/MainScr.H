#ifndef MAINSCR_H
#define MAINSCR_H


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
*/

#include "MOX/MOX_TYPE.H"

#include "MOM_DEF.H"



// WZD dseg:2E12
extern int16_t DBG_Alt_A_State;



// WZD dseg:305E
extern int16_t unit_enchantment_animation_count;



// WZD dseg:3062
extern uint8_t enchantment_outline_colors[5][8][8];

// WZD  dseg:31AE
// char cnst_Moves[]  = "Moves:";



// WZD dseg:973E
extern int16_t _minimap_grid_field;
// WZD dseg:9740
extern int16_t _main_map_grid_field;

// WZD dseg:9742
// int16_t _minimap_grid_x;
extern int64_t _minimap_grid_x;
// WZD dseg:9744
// int16_t _minimap_grid_y;
extern int64_t _minimap_grid_y;

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
extern int16_t special_action_flag;

// WZD dseg:C050
// int16_t _next_turn_button;

// WZD dseg:C052
extern int16_t g_unit_window_fields[MAX_STACK];

// WZD dseg:C064
// CRP_OverlandVar_3 dw 0

// WZD dseg:C066
// int16_t _active_stack_path_length;



// WZD dseg:C072
extern int16_t _plane_button;



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
extern SAMB_ptr IMG_OVL_Corruption;                        // ; reserved EMM header pointer for a single image

// WZD dseg:CB6C 00 00                                           UU_IMG_OVL_Mud@ dw 0                    ; DATA XREF: Terrain_Init+1F6w
// WZD dseg:CB6C                                                                                         ; reserved EMM header pointer (unused in the game?)

// WZD dseg:CB6E
/*
    load in Terrain_Init()
    MAPBACK.LBX  78-86
    SITES - coal, iron, silver, gold, gems, mithril, adamantium, quork, crysx
*/
extern SAMB_ptr mineral_site_segs[10];

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
extern SAMB_ptr _unit_colored_backgrounds_seg[6];    // load in Terrain_Init() ovr052

// WZD dseg:CB92                                                                                         ; DATA XREF: Terrain_Init+F3w ...
// WZD dseg:CB92                                                                                         ; array of 6 reserved EMM header pointers
// WZD dseg:CB92                                                                                         ; (1 per banner color)
// WZD dseg:CB9E 00                                              db    0
// WZD dseg:CB9F 00                                              db    0

// WZD dseg:CBA0
extern SAMB_ptr eroads_seg[8];               // ; array of 8 reserved EMM header pointers  ; for 6 frame animations  ; clockwise 1-connection e.roads starting up
// WZD dseg:CBB0
extern SAMB_ptr eroad_lone_seg;             // ; reserved EMM header pointer for a 6 frame animation  ; no connection enchanted road
// WZD dseg:CBB0
extern SAMB_ptr roads_seg[8];                // ; array of 8 reserved EMM header pointers  ; clockwise 1-connection roads starting up
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
// NEWCODE  extern SAMB_ptr unexplored_mask_seg[14];       // ; array of 14 reserved EMM header pointers
extern SAMB_ptr unexplored_mask_seg[15];

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
    WIZARDS.EXE  ovr067
*/
// WZD o67p01
void Draw_Maps(int16_t x, int16_t y, int16_t HTiles, int16_t VTiles, int16_t *MapX, int16_t *MapY, int16_t Plane, int16_t XPos, int16_t YPos, int16_t player_idx);
// WZD o67p02
void IDK_CheckSet_MapDisplay_XY(void);
// WZD o67p05
void Set_Draw_Active_Stack_Never(void);
// WZD o67p08
void Reduced_Map_Set_Dims(int16_t width, int16_t height);
// WZD o67p09
void Draw_World_Window(int16_t start_x, int16_t start_y, int16_t width, int16_t height);
// WZD o67p15
int16_t IsPassableTower(int16_t wx, int16_t wy);
/*
    WIZARDS.EXE  ovr068
*/
// WZD o68p03
void Reduced_Map_Coords(int16_t * minimap_x, int16_t * minimap_y, int16_t mid_x, int16_t mid_y, int16_t minimap_width, int16_t minimap_height);
// WZD o68p09
void List_Screen_Draw_Reduced_Map(int16_t x, int16_t y, int16_t w, int16_t h, int16_t wp, int16_t wx, int16_t wy);
/*
    WIZARDS.EXE  ovr150
*/
// WZD o150p16
void Create_Reduced_Map_Picture(int16_t minimap_start_x, int16_t minimap_start_y, int16_t world_plane, byte_ptr minimap_pict_seg, int16_t minimap_width, int16_t minimap_height, int16_t mark_x, int16_t mark_y, int16_t mark_flag);



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
void Main_Screen(void);

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
void Move_Stack_DirKey(int16_t movement_direction);



/*
    WIZARDS.EXE  ovr058
*/

// WZD o58p01
void Main_Screen_Draw_Unit_Window(int16_t start_x, int16_t start_y);

// WZD o58p02
void Set_Mouse_List_Default(void);

// WZD o58p03
void o58p03_empty_function(void);

// WZD o58p04
void Falsify_CRP_OVL_Obstacle_Var1(void);

// WZD o58p05
void Increment_Background_Music(void);

// WZD o58p06
int16_t Get_Background_Music(void);

// WZD o58p07
void Play_Background_Music__STUB(void);

// WZD o58p08
void IDK_HumanPlayer_SelectStack_UnitLocation(int16_t unit_idx);

// WZD o58p09
void IDK_HumanPlayer_SelectStack_SummonLocation(void);


/*
    WIZARDS.EXE  ovr062
*/

// WZD o62p01
int16_t o62p01_empty_function(int16_t player_idx);

// WZD o62p02
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
// WZD o63p01
void Main_Screen_Draw_Status_Window(void);

// WZD o063p02
void Main_Screen_Draw_Do_Draw(int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t x_pos, int16_t y_pos, int16_t player_idx);

// WZD o063p03
void Draw_Unit_Enchantment_Outline(int16_t unit_idx);

// WZD o63p04
void Cycle_Unit_Enchantment_Animation(void);

// WZD o63p05
void Draw_Unit_Picture(int16_t x, int16_t y, int16_t unit_idx, int16_t flag);
// called from WZD o67p07 Draw_Active_Unit_Stack()

// WZD o63p06
void Draw_Unit_StatFig(int16_t x, int16_t y, int16_t unit_idx, int16_t flag);

// WZD o063p07
void Unit_Window_Draw_Unit_Attributes(int16_t x, int16_t y, int16_t unit_idx);

// WZD o63p08
void Main_Screen_Draw_Movement_Bar(void);

// WZD o63p09
void Draw_Movement_Mode_Icons(int16_t x, int16_t y, int16_t unit_idx);

// WZD o63p10
int16_t Stack_Has_PlanarTravel(void);

// WZD o63p11
int16_t Active_Stack_On_Astral_Gate(void);

// WZD o63p12
int16_t Stack_Moves_Active(void);

// WZD o63p13
int16_t Stack_Moves(void);

// WZD o63p14
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

// WZD o64p03
// AKA OVL_PrepMinimap()
void MainScr_Create_Reduced_Map_Picture(void);

// WZD o064p04
void Main_Screen_Draw_Summary_Window(void);

// WZD o064p05
void Main_Screen_Draw_Next_Turn_Button(void);

// WZD o64p06
void OVL_DisableIncmBlink(void);

// WZD o64p07
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
// AKA Move_Stack()
int16_t Move_Units(int16_t player_idx, int16_t destination_x, int16_t destination_y, int16_t path_type, int16_t * map_x, int16_t * map_y, int16_t map_p, int16_t troop_count, int16_t troops[]);

// WZD o95p02
void Move_Units_Draw(int16_t player_idx, int16_t map_p, int16_t movepath_length, int16_t * map_x, int16_t * map_y, int16_t unit_array[], int16_t unit_array_count);

// WZD o95p03
// AI_ContactWizards()

// WZD o95p04
void Update_MovePathMap(int8_t * ptr_movepath_cost_map_moves2, int16_t boatrider_count, int16_t troop_count, int16_t wp, int16_t player_idx, int16_t dst_wx, int16_t dst_wy, int16_t src_wx, int16_t src_wy);

// WZD o95p05
int16_t Army_Boatriders(int16_t troop_count, int16_t troops[], int16_t boatriders[]);

// WZD o95p06
void Fix_Patrol_On_Boats(int16_t troop_count, int16_t troops[]);



/*
    WIZARDS.EXE  ovr097
*/

// WZD o97p01
void Eval_Move_Path__WIP(int16_t player_idx, int8_t mvpth_x[], int8_t mvpth_y[], int16_t wp, int8_t mvpth_c[], int16_t moves2, int16_t * atackee_idx, int16_t * attack_flag, int16_t * path_length, int16_t * Cmplt, int16_t troops[], int16_t troop_count);

// WZD o97p02
// sub_7E597()

// WZD o97p03
int16_t Square_Has_City(int16_t wx, int16_t wy, int16_t wp);

// WZD o97p04
int16_t Units_Moves(int16_t unit_array[], int16_t unit_array_count);

// WZD o97p05
int16_t RP_CTY_CheckSpellWard__STUB(int16_t city_idx, int16_t * stack_size, int16_t * stack_array);

// WZD o97p06
int16_t CTY_CheckSpellWard__STUB(int16_t city_idx, int16_t * stack_size, int16_t * stack_array);

// WZD o97p07
// OVL_SpellWardError()

// WZD o97p08
void Print_Moves_String(int16_t x_start, int16_t y_start, int16_t moves2, int16_t right_align_flag);



void Main_Screen_Draw_Debug_Information(void);



#ifdef __cplusplus
}
#endif



#endif  /* MAINSCR_H */