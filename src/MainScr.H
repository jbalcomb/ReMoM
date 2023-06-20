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


// WZD dseg:CB92
extern SAMB_ptr _unit_colored_backgrounds_seg[];




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
