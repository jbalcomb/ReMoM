#ifndef MOUSE_H
#define MOUSE_H

#include "MoX_TYPE.H"


//enum Cursor_Types
//crsr_None  = 0
//crsr_Finger  = 1
//crsr_CastBase  = 2
//crsr_RedCross  = 3
//crsr_Ranged  = 4
//crsr_Melee  = 5
//crsr_Hourglass  = 6
//crsr_WingedBoot  = 7
//crsr_Empty  = 8
//crsr_CastAnim1  = 9
//crsr_CastAnim2  = 0Ah
//crsr_CastAnim3  = 0Bh
//crsr_CastAnim4  = 0Ch
//crsr_CastAnim5  = 0Dh


struct s_mouse_list {
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
// WZD dseg:78BC MOUSE_CursorDraw dw 0
// WZD dseg:78BE
// drake178 MOUSE_DriverPresent
// MoO2 mouse_driver_installed
int16_t mouse_driver_installed;

// WZD dseg:78C0 MOUSE_Usable dw 0
// WZD dseg:78C2
// MOUSE_CurrentX dw 9Eh  158
extern int16_t mouse_x;
// WZD dseg:78C4 MOUSE_CurrentY dw 64h  100
extern int16_t mouse_y;


// ...
/* END: Initialized Data [Data Segment - DSEG]*/




#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    MGC  seg033
*/


// MGC s33p01
// WZD s35p01
void Set_Mouse_List(int16_t count, struct s_mouse_list * list);

// MGC s33p02
void Check_Mouse_Shape(int16_t x, int16_t y);

// MGC s33p03
int16_t Get_Pointer_Image_Number(void);

// MGC s33p04
int16_t Get_Pointer_Offset(void);

// WZD s35p05
int16_t MD_GetButtonStatus(void);

// MGC s33p07
int16_t Init_Mouse_Driver(void);


// MGC s33p13
void MD_Draw_Disable(void);

// MGC s33p14
void MD_Draw_Restore(void);

// MGC s33p18
int16_t Pointer_X(void);

// MGC s33p19
int16_t Pointer_Y(void);

// WZD s35p20
// MD_MoveCursor
void Set_Pointer_Position(int16_t x, int16_t y);

// WZD s35p21
// AKA MD_MoveCursorINT() AKA MOUSE_MoveCursorINT()
void Set_Mouse_Position(int16_t x, int16_t y);

// MGC s33p30
// MGC s33p31
// MGC s33p32
void Save_Mouse(int16_t x, int16_t y);

// MGC s33p34
// MGC s33p35
// MGC s33p36
void Restore_Mouse(void);

// MGC s33p37
// MGC s33p38
// MGC s33p39
// MGC s33p40
void Draw_Mouse(int16_t x, int16_t y);


#ifdef __cplusplus
}
#endif


#endif  /* MOUSE_H */
