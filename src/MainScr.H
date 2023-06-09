#ifndef MAINSCR_H
#define MAINSCR_H



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


void Main_Screen(void);

void Main_Screen_Draw(void);

void Main_Screen_Draw_Do_Draw(int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t x_pos, int16_t y_pos, int16_t player_idx);


/*
    WIZARDS.EXE  ovr057
*/

// WZD o57p06
void Main_Screen_Draw_Game_Buttons(void);

// WZD o57p09
void Main_Screen_Reset(void);


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

// WZD o063p05
void Unit_Window_Draw_Unit_Picture(int16_t x, int16_t y, int16_t unit_stack_unit_idx, int16_t flag);

// WZD o063p05
// AKA OVL_DrawUnitImage()
void Unit_Window_Draw_Unit_StatFig(int16_t x, int16_t y, int16_t unit_idx, int16_t flag);



/*
    WIZARDS.EXE  ovr064
*/

// WZD o64p01
void Allocate_Reduced_Map__1(void);

// WZD o064p04
void Main_Screen_Draw_Summary_Window(void);

// WZD o064p05
void Main_Screen_Draw_Next_Turn_Button(void);

// WZD o64p09
void Unit_Window_Picture_Coords(int16_t unit_stack_unit_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2);



#ifdef __cplusplus
}
#endif


#endif  /* MAINSCR_H */
