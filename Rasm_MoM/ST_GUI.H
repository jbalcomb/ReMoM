#ifndef ST_GUI_H
#define ST_GUI_H

enum Cursor_Types {
    Crsr_None,
    Crsr_Normal,
    Crsr_CastBase,
    Crsr_RedCross,
    Crsr_Ranged,
    Crsr_Melee,
    Crsr_Hourglas,
    Crsr_MoveHat,
    Crsr_Empty,
    Crsr_CastAnim,
    Crsr_CastAnim2,
    Crsr_CastAnim3,
    Crsr_CastAnim4,
    Crsr_CastAnim5
};

struct GUI_WINDOW {
    int Cursor_Index;                               // ; enum Cursor_Types
    int Cursor_Offset;
    int Left;
    int Top;
    int Right;
    int Bottom;
};


extern int g_GUI_Windows;                   // dseg:43B0

extern int g_GUI_WindowCount;               // dseg:4398


extern int g_MOUSE_Hardware;                // dseg:4D20 
extern int g_GUI_FocusedControl;            // dseg:4D22

extern struct GUI_WINDOW g_GUI_VirtualScreen;   // dseg:4D2E ; a record of size 0Ch, this is off by 2 bytes based on the coordinates it contains

extern int g_GUI_EscapeOverride;            // dseg:4D44


extern void far * gfp_GUI_Control_Table;    // dseg:A8A2

extern int g_GUI_Delay;                     // dseg:A88E
extern int g_MOUSE_Emu_Y;                   // dseg:A890
extern int g_MOUSE_Emu_X;                   // dseg:A892


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg033
*/
void GUI_SetWindows(int argWindowCount, struct GUI_WINDOW *argWindowArray);

/*
    seg034
*/
void GUI_Init(int Input_Type);


#ifdef __cplusplus
}
#endif

#endif /* ST_GUI_H */
