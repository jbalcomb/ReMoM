#ifndef MAINSCR_H
#define MAINSCR_H


/*
    WIZARDS.EXE
    ovr057
    ovr058
    ovr062
    ovr063
    ovr064
*/


// WZD dseg:C082
// AKA OVL_STKUnitCards_Top
extern int16_t _unit_window_start_y;
// WZD dseg:C084
// AKA OVL_STKUnitCards_Lft
extern int16_t _unit_window_start_x;


// WZD dseg:C086 
extern SAMB_ptr _reduced_map_seg;
// ; 12Fh LBX_Alloc_First paragraphs in the LBX_Sandbox_Segment


/*
                                        ¿ BEGIN: ovr052 Terrain_Init() ?
*/
// WZD dseg:CB5E                                                 ovr052
// WZD dseg:CB5E 00 00                                           IMG_OVL_WarpedMask@ dw 0                ; DATA XREF: Terrain_Init+320w ...
// WZD dseg:CB5E                                                                                         ; reserved EMM header pointer for a single image

// WZD dseg:CB60
extern SAMB_ptr gsa_OVL_Tile_WorkArea;              // alloc in Terrain_Init  // ; 70 * 16 = 1120 bytes


// WZD dseg:CB62
extern SAMB_ptr UU_IMG_OVL_Empty3;                 // ; single-loaded image, called "hunter's lodge"

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
extern SAMB_ptr _mineral_sites_seg[];

// WZD dseg:CB80 00                                              db    0
// WZD dseg:CB81 00                                              db    0
// WZD dseg:CB82 00 00 00 00 00 00 00 00 00 00                   IMG_OVL_Sparkles@ dw 5 dup(0)           ; DATA XREF: Terrain_Init+2BFw ...
// WZD dseg:CB82                                                                                         ; array of 5 reserved EMM header pointers for
// WZD dseg:CB82                                                                                         ; 6 frame animations
// WZD dseg:CB8C 00                                              db    0
// WZD dseg:CB8D 00                                              db    0
// WZD dseg:CB8E 00 00                                           UU_IMG_OVL_Empty2@ dw 0                 ; DATA XREF: Terrain_Init+22Cw
// WZD dseg:CB8E                                                                                         ; single-loaded image, called "lumber camp" in the file
// WZD dseg:CB90 00 00                                           OVL_WarpNodeFX_Prep@ dw 0               ; DATA XREF: Terrain_Init+2A3w ...
// WZD dseg:CB90                                                                                         ; 18h LBX_Alloc_Space paragraphs
// WZD dseg:CB90                                                                                         ; used to save and manipulate the tile graphics to
// WZD dseg:CB90                                                                                         ; display the warp node effect

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
extern SAMB_ptr roads_seg[];                // ; reserved EMM header pointer for a 6 frame animation  ; no connection enchanted road
// WZD dseg:CBB2
extern SAMB_ptr road_lone_seg;              // ; array of 8 reserved EMM header pointers  ; clockwise 1-connection roads starting up

// WZD dseg:CBC2                                                                                         ; reserved EMM header pointer for a single image
// WZD dseg:CBC2                                                                                         ; no connection normal road
// WZD dseg:CBC4 00 00                                           IMG_OVL_EZ_Cave@ dw 0                   ; DATA XREF: Terrain_Init+154w ...
// WZD dseg:CBC4                                                                                         ; reserved EMM header pointer for a single image
// WZD dseg:CBC6 00 00                                           IMG_OVL_EZ_Dung@ dw 0                   ; DATA XREF: Terrain_Init+166w
// WZD dseg:CBC6                                                                                         ; reserved EMM header pointer for a single image
// WZD dseg:CBC8 00 00                                           IMG_OVL_EZ_ATmpl@ dw 0                  ; DATA XREF: Terrain_Init+178w
// WZD dseg:CBC8                                                                                         ; reserved EMM header pointer for a single image
// WZD dseg:CBCA 00 00                                           IMG_OVL_EZ_AKeep@ dw 0                  ; DATA XREF: Terrain_Init+18Aw
// WZD dseg:CBCA                                                                                         ; reserved EMM header pointer for a single image
// WZD dseg:CBCC 00 00                                           IMG_OVL_EZ_MLair@ dw 0                  ; DATA XREF: Terrain_Init+19Cw
// WZD dseg:CBCC                                                                                         ; reserved EMM header pointer for a single image
// WZD dseg:CBCE 00 00                                           IMG_OVL_EZ_Ruins@ dw 0                  ; DATA XREF: Terrain_Init+1AEw
// WZD dseg:CBCE                                                                                         ; reserved EMM header pointer for a single image
// WZD dseg:CBD0 00 00                                           IMG_OVL_EZ_FTmpl@ dw 0                  ; DATA XREF: Terrain_Init+1C0w
// WZD dseg:CBD0                                                                                         ; reserved EMM header pointer for a single image
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
// WZD dseg:CC02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+gsa_IMG_OVL_Exploration@ dw 0Eh dup(0)  ; DATA XREF: Terrain_Init+D0w ...
// WZD dseg:CC02 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 reserved EMM header pointers
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
o57p10  ? IDK_UnitMoves_and_PlanarTravel() ? 

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
// PRIVATE  void Main_Screen_Draw(void);
// WZD o57p09
// PRIVATE  void Main_Screen_Reset(void);
// WZD o57p10
// PRIVATE  void IDK_UnitMoves_and_PlanarTravel(void);
/*
    WIZARDS.EXE  ovr058
*/
// WZD o58p01
// PRIVATE  void Main_Screen_Draw_Unit_Window(int16_t start_x, int16_t start_y);
/*
    WIZARDAS.EXE  ovr062
*/
// WZD o62p04
// PRIVATE  void Build_Unit_Stack(int16_t player_idx, int16_t world_plane, int16_t world_x, int16_t world_y);
/*
    WIZARDAS.EXE  ovr063
*/
// WZD o63p01
// PRIVATE  void Main_Screen_Draw_Status_Window(void);
// WZD o063p02
// PRIVATE  void Main_Screen_Draw_Do_Draw(int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t x_pos, int16_t y_pos, int16_t player_idx);

// WZD o63p04
// ???

// WZD o63p04
// PRIVATE  void Cycle_Unit_Enchantment_Animation(void);
// WZD o63p05
void Unit_Window_Draw_Unit_Picture(int16_t x, int16_t y, int16_t unit_stack_unit_idx, int16_t flag);
// called from WZD o67p07 Draw_Active_Unit_Stack()
// WZD o63p05
// AKA OVL_DrawUnitImage()
// PRIVATE  void Unit_Window_Draw_Unit_StatFig(int16_t x, int16_t y, int16_t unit_idx, int16_t flag);
// WZD o63p12
// PRIVATE int16_t OVL_GetStackHMoves(void);
/*
    WIZARDS.EXE  ovr064
*/
// WZD o64p01
// PRIVATE  void Allocate_Reduced_Map__1(void);
// WZD o64p02
// PRIVATE  void Allocate_Reduced_Map__2(void);
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


#ifdef __cplusplus
}
#endif


#endif  /* MAINSCR_H */
