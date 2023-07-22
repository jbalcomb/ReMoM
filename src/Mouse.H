#ifndef MOUSE_H
#define MOUSE_H

#include "MoX_TYPE.H"

#ifdef __cplusplus
extern "C" {
#endif



// "cursor image"
enum e_Cursor_Image
{
    crsr_None,
    crsr_CastBase,
    crsr_RedCross,
    crsr_Ranged,
    crsr_Melee,
    crsr_Hourglass,
    crsr_WingedBoot,
    crsr_Empty,
    crsr_CastAnim1,
    crsr_CastAnim2,
    crsr_CastAnim3,
    crsr_CastAnim4,
    crsr_CastAnim5
};


struct s_mouse_list
{
    int16_t image_num;
    int16_t center_offset;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
};


// WZD dseg:00EE    mouse_list_default s_MOUSE_LIST <crsr_Finger, 0, 0, 0, 319, 199>
extern struct s_mouse_list mouse_list_default[];
// WZD dseg:00FA    mouse_list_none s_MOUSE_LIST <0, 0, 0, 0, 319, 199>
// WZD dseg:0106    mouse_list_hourglass s_MOUSE_LIST <crsr_Hourglass, 0, 0, 0, 319, 199>
// WZD dseg:0112    NIU_mouse_list_normal s_MOUSE_LIST <crsr_Finger, 0, 0, 0, 319, 199>
// WZD dseg:011E    NIU_mouse_list_boot s_MOUSE_LIST <crsr_WingedBoot, 4, 0, 0, 319, 158> ; ? 158 is main map width ?


/* BEGIN: Initialized Data [Data Segment - DSEG]*/
// ...
// dseg:78B8                                                 BEGIN: Fields, Input, Mouse, Keyboard

// WZD dseg:78B8 current_pointer_image_number dw 0
// WZD dseg:78BA previous_pointer_image_number dw 0

// WZD dseg:78BC
// drake178: MOUSE_CursorDraw
extern int16_t mouse_enabled;

// WZD dseg:78BE
// drake178 MOUSE_DriverPresent
// MoO2 mouse_driver_installed
extern int16_t mouse_driver_installed;

// WZD dseg:78C0 MOUSE_Usable dw 0

// WZD dseg:78C2
// MOUSE_CurrentX dw 9Eh  158
// extern int16_t mouse_x;
// WZD dseg:78C4 MOUSE_CurrentY dw 64h  100
// extern int16_t mouse_y;
extern int16_t mouse_x;
extern int16_t mouse_y;

// WZD dseg:78C6
extern int16_t current_mouse_list_count;

// WZD dseg:78C8
extern int16_t current_pointer_offset;

// WZD dseg:78CA 00 00                   MOUSE_ClickRec1 dw 0
// WZD dseg:78CC 00 00                   MOUSE_ClickX dw 0
// WZD dseg:78CE 00 00                   MOUSE_ClickY dw 0
// WZD dseg:78D0 00 00                   MOUSE_ClickBtns dw 0
// WZD dseg:78D2 00 00                   MOUSE_ClickRec2 dw 0
// WZD dseg:78D4 00 00                   MOUSE_INT_Process dw 0

// WZD dseg:78D4
extern int16_t MOUSE_INT_Process;


// ...
/* END: Initialized Data [Data Segment - DSEG]*/



/*
    WZD  seg035
    MGC  seg033
*/


// WZD s35p01
void Set_Mouse_List(int16_t count, struct s_mouse_list * list);

// WZD s35p02
void Check_Mouse_Shape(int16_t x, int16_t y);

// WZD s35p03
int16_t Get_Pointer_Image_Number(void);

// WZD s35p04
int16_t Get_Pointer_Offset(void);

// WZD s35p05
int16_t MD_GetButtonStatus(void);

// WZD s35p07
int16_t Init_Mouse_Driver(void);

// WZD s35p08
// RP_Mouse_SetUsable

// WZD s35p09
// MOUSE_Reset

// WZD s35p10
void Mouse_Movement_Handler(void);

// WZD s35p11
void Mouse_Button_Handler(void);

// WZD s35p12
// MOUSE_INT_Handler

// WZD s35p13
void Save_Mouse_State(void);

// WZD s35p14
void Restore_Mouse_State(void);


// WZD s35p18
int16_t Pointer_X(void);

// WZD s35p19
int16_t Pointer_Y(void);

// WZD s35p20
// MD_MoveCursor
void Set_Pointer_Position(int16_t x, int16_t y);

// WZD s35p21
// AKA MD_MoveCursorINT() AKA MOUSE_MoveCursorINT()
void Set_Mouse_Position(int16_t x, int16_t y);



// WZD s35p30  Save_Mouse_On_Page   CRL_Save_RSP  GUI_SaveCursorArea
// WZD s35p31  Save_Mouse_Off_Page  CRL_Save_DSP  GUI_SaveDrawCrsrArea
// WZD s35p32  IN_CRL_Save_RSP  VGA_SaveCursorArea

/*
    [draw_page_num -> mouse_background_buffer
*/
void Save_Mouse_On_Page(int16_t x, int16_t y);

/*
    [1 - draw_page_num] -> mouse_off_page_buffer
*/
void Save_Mouse_Off_Page(int16_t x, int16_t y);

/*
    [draw_page_num -> mouse_background_buffer
    nearly same as Save_Mouse_On_Page(), without none/save check
*/
void Save_Mouse_On_Page_(int16_t x, int16_t y);


// WZD s35p33  Copy_Mouse_Off_To_Mouse_Back  CRL_Copy_DSP2RSP  GUI_CursorCopy_Bkup2Main
/*
    mouse_off_page_buffer -> mouse_background_buffer
*/
void Copy_Mouse_Off_To_Mouse_Back(void);


// WZD s35p34  Restore_Mouse_On_Page   CRL_Restore_RSP     VGA_RestoreCursrArea
// WZD s35p35  Restore_Mouse_Off_Page  CRL_Restore_DSP     VGA_RestoreDrawCArea
// WZD s35p36  ???                     UU_CRL_Restore_SP3  UU_VGA_RestoreCursorArea_SP3

/*
    mouse_background_buffer -> [draw_page_num]
*/
void Restore_Mouse_On_Page(void);

/*
    mouse_background_buffer -> [1 - draw_page_num]
*/
void Restore_Mouse_Off_Page(void);


// WZD s35p37  Draw_Mouse_On_Page    CRH_Draw_RSP  GUI_DisplayCursor
// WZD s35p38  Draw_Mouse_Off_Page   CRH_Draw_DSP  GUI_DrawCursor
// WZD s35p39  Draw_Mouse_On_Page_   CRL_Draw_RSP  VGA_DisplayCursor
// WZD s35p40  Draw_Mouse_Off_Page_  CRL_Draw_DSP  VGA_DrawCursor

/*
    mouse_palette -> [draw_page_num]
*/
void Draw_Mouse_On_Page(int16_t x, int16_t y);

/*
    mouse_palette -> [1 - draw_page_num]
*/
void Draw_Mouse_Off_Page(int16_t x, int16_t y);

/*
    mouse_palette -> [draw_page_num]
*/
void Draw_Mouse_On_Page_(int16_t x, int16_t y);

/*
    mouse_palette -> [1 - draw_page_num]
*/
void Draw_Mouse_Off_Page_(int16_t x, int16_t y);



#ifdef __cplusplus
}
#endif

#endif  /* MOUSE_H */
