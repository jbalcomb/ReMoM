#ifndef ST_GUI_H
#define ST_GUI_H

#include "ST_HEAD.H"


#define ADDR_RSP() { \
    asm mov ax, [g_RenderScreenPage]; \
    asm mov ah, al; \
    asm xor al, al; \
    asm shl ax, 1; \
    asm shl ax, 1; \
    asm add ax, 0A000h; \
    asm add ax, dx; \
    asm mov es, ax; \
}
#define ADDR_DSP() { \
    asm mov ax, 1; \
    asm sub ax, [g_RenderScreenPage]; \
    asm mov ah, al; \
    asm xor al, al; \
    asm shl ax, 1; \
    asm shl ax, 1; \
    asm add ax, 0A000h; \
    asm add ax, dx; \
    asm mov es, ax; \
}
//asm   mov ax, [g_RenderScreenPage]
//asm   mov ah, al                              //; (_AX << 8)
//asm   sub al, al                              //; (_AX & 0xFF00)
//asm   shl ax, 1
//asm   shl ax, 1                               //; (_AX << 2) == (_AX * 2^2) == (_AX * 4)
//asm   add ax, 0A000h                          //; VGA - Graphics Mode - Video Memory Segment Address / Base
//asm   mov ds, ax



enum GUI_Controls
{
    Ctrl_ClickButton  = 0,
    Ctrl_ToggleButton  = 1,
    Ctrl_LockableButton  = 2,
    Ctrl_MStateButton  = 3,
    Ctrl_EditBox  = 4,
    Ctrl_ImageLabel  = 5,
    Ctrl_Slidebar  = 6,
    Ctrl_Label  = 7,
    Ctrl_AltString  = 8,
    Ctrl_ClickLink  = 9,
    Ctrl_DialogLine  = 10,
    Ctrl_EditSelect  = 11,
    Ctrl_ClickGrid  = 12,
};

// sizeof 0Ch 12d (6 values * 2 bytes)
struct s_GUI_WINDOW {
    int Cursor_Index;                               // ; enum Cursor_Types
    int Cursor_Offset;  // ? drake178:"Offset", but maybe "HotSpot" ?
    int Left;
    int Top;
    int Right;
    int Bottom;
};

struct s_GUI_CTRL //; (sizeof=0x26)
{
    int Left;
    int Top;
    int Right;
    int Bottom;
    int Ctrl_Type;
    int UU_Help;
    int Font_Index;
    int ColorSet1;
    int Rect_FX;
    int Selectable;
    int ColorSet2;
    int Param0;                             //; Text@                           (6:Min_Val, 9:Parent)
    int Param1;                             //; Trig_State     (6:Max_Val, 9/A:Index, B:IMG, C:CellW)
    int Param2;                             //; State@        (4/B:BG_Color, 6:StSeg, 8:Len, C:CellH)
    int Param3;                             //; FullFrames    (4:Align, 6:MinSlide, A:CBlock, C:Col@)
    int Param4;                             //; MaxSlide                           (4:CStyle, C:Row@)
    int Param5;                             //; IMG_Seg                        (4/B:MaxLen, 6:Orient)
    int Param6;                             //; CorsorColors@                    (6:StOff, B:Outline)
    char Hotkey;
    char Unused_25h;
};
/*
Params:                 Param0      Param1      Param2      Param3      Param4      Param5      Param6
 0: Ctrl_ClickButton    Text                                                        IMG_Seg
 1: Ctrl_ToggleButton   
 2: Ctrl_LockableButton 
 3: Ctrl_MStateButton   
 4: Ctrl_EditBox        
 5: Ctrl_ImageLabel     
 6: Ctrl_Slidebar       
 7: Ctrl_Label          
 8: Ctrl_AltString      
 9: Ctrl_ClickLink      
10: Ctrl_DialogLine     
11: Ctrl_EditSelect     
12: Ctrl_ClickGrid      
*/

/*
    Initialized Data
*/

// \/\/\/ ? VGA, GUI, etc ? \/\/\/
// dseg:436C VGA_WriteMapMasks2 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:4370 GUI_DS_Save dw 0
// dseg:4372 g_VGA_WriteMapMasks3 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:4376 VGA_WriteMapMasks4 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:437A VGA_WriteMapMasks5 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:437E dw 0
// dseg:4380 VGA_WriteMapMasks6 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:4384 dw 0
// dseg:4386 VGA_WriteMapMasks7 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// /\/\/\ ? VGA, GUI, etc ? /\/\/\

extern int g_CRSR_Curr;             // dseg:438A
extern int g_CRSR_Prev;                // dseg:438C
extern int g_MD_CursorDraw;              // dseg:438E
extern int g_MouseDriverInstalled;          // dseg:4390
/* dseg:4392 00 00                         MOUSE_Usable dw 0 */
extern int g_MD_CurrentX;                // dseg:4394
extern int g_MD_CurrentY;                // dseg:4396
extern int g_GUI_WindowCount;               // dseg:4398
extern int g_CRSR_Offset;              // dseg:439A
extern int g_MD_ClickRec1;               // dseg:439C
extern int g_MD_ClickX;                  // dseg:439E
extern int g_MD_ClickY;                  // dseg:43A0
extern int g_MD_ClickBtns;               // dseg:43A2
extern int g_MD_ClickRec2;               // dseg:43A4
extern int g_MD_INT_InProcess;             // dseg:43A6
extern int g_CRSR_HaveSave;                // dseg:43A8
extern int g_MD_InitY;                   // dseg:43AC
extern int g_MD_InitX;                   // dseg:43AA
extern int g_MD_CDraw_Save;              // dseg:43AE
extern struct s_GUI_WINDOW * g_GUI_Windows;  // dseg:43B0
// extern struct s_Cursor_Area_Copy Cursor_Save_Main; //dseg:43B2 
// // dseg:4538  REUSABLE_810bytes   db 32Ah dup(0)
extern unsigned int g_CRSR_Save_RSP[];
// extern struct s_Cursor_Area_Copy Cursor_Save_Bkup; //dseg:4862 
// // dseg:49E8  REUSABLE_810bytes_2 db 32Ah dup(0)
extern unsigned int g_CRSR_Save_DSP[];
// dseg:4D12 00 00                         UU_GUI_ClickBufferX dw 0
// dseg:4D14 00 00                         UU_GUI_ClickBuffNext dw 0
// dseg:4D16 00 00                         UU_GUI_ClickBuffVar1 dw 0
// dseg:4D18 00 00                         UU_GUI_ClickBuffer@ dw 0
// dseg:4D1A 00 00                         UU_GUI_ClickBufCount dw 0
extern int g_SCRN_RedrawFn_Present;          // dseg:4D1C
extern int g_GUI_Last_Key_Control;          // dseg:4D1E
extern int g_MD_Hardware;                // dseg:4D20 
extern int g_CTRL_Focused;            // dseg:4D22
/*
dseg:4D24 01 00                         GUI_BtnClk_TextLeft dw 1                ; added to the Left position of text drawn on buttons when clicked
dseg:4D26 FF FF                         GUI_BtnClk_TextTop dw -1                ; added to the Top position of text drawn on buttons when clicked
dseg:4D28 FF FF                         GUI_EmptyTitleHelp dw 0FFFFh
*/
extern int g_GUI_DialogDirections;          // dseg:4D2A

extern int g_SCRN_Redraw_Timer; // dseg:4D2C
extern struct s_GUI_WINDOW g_GUI_VirtualScreen[];   // dseg:4D2E
extern int g_CTRL_MouseFocusCtrl;            // dseg:4D3A
// dseg:4D3C GUI_MouseHighlight dw 1
// dseg:4D3E GUI_NoDialogWrap dw 0
// dseg:4D40 GUI_NoTxtSelectColor dw 0
// dseg:4D42 GUI_OnLastDialogLine dw 0
extern int g_IN_EscapeOverride;            // dseg:4D44
extern int g_GUI_ClickActivate; // dseg:4D46
// dseg:4D48 aNotInUse db 'NOT IN USE',0
// dseg:4D53 align 2
// dseg:4D54 GUI_Edit_Position dw 0
extern int g_GUI_EditAnimStage;             // dseg:4D56
extern int g_GUI_EditCursorOn;              // dseg:4D58
extern int g_CTRL_InEditSelect;              // dseg:4D5A
extern int g_DBG_ControlHighlight;          // dseg:4D5C 
extern int g_DBG_HelpHighlight;             // dseg:4D5E
extern int g_CTRL_Active_EditSlct;           // dseg:4D60
// dseg:4D62 GUI_EditTimeOutType dw 0
// dseg:4D64 UU_GUI_UUClickBufVar dw 0
// dseg:4D66 UU_GUI_ClickBuffer dw 0
extern int g_GUI_Help_Available;            // dseg:4D68
extern int g_HLP_Entry_Count;               // dseg:4D6A
extern int g_GUI_ActiveStringTrig;          // dseg:4D6C
// dseg:4D6E UU_cnst_RB7 db 'rb'
// dseg:4D70 cnst_ZeroString_1 db 0
// dseg:4D71 UU_cnst_WB7 db 'wb',0
// dseg:4D74 UU_GUI_Up_Hotkey db    5,   0                            ; 0
// dseg:4D76 UU_GUI_Down_Hotkey db    6,   0                            ; 0
// dseg:4D78 GUI_EditCursor db '_',0


/*
    Uninitialized Data
*/

extern int g_HLP_Entry_Table;               // dseg:A868
extern int g_GUI_Processed_Btns;            // dseg:A86A
extern int g_GUI_Processed_LastY;           // dseg:A86C
extern int g_GUI_Processed_LastX;           // dseg:A86E
extern char g_GUI_EditString[];             // dseg:A870
extern int g_IN_Skip;                     // dseg:A88E
extern int g_MD_Emu_Y;                   // dseg:A890
extern int g_MD_Emu_X;                   // dseg:A892
extern int g_GUI_Cursor_Offset;             // dseg:A894
// dseg:A896 GUI_PrevControlCount dw 0
extern int g_CTRL_Control_Count;             // dseg:A898
// dseg:A89A gfp_CTRL_Prev_Redraw_Fn dd 0
//extern void _FAR *gfp_GUI_Redraw_Function;  // dseg:A89E
//void      (*uiobj_callback)(void *) = NULL;
extern void (*gfp_SCRN_Redraw_Function)(void);

//extern void _FAR *gfp_GUI_Control_Table;    // dseg:A8A2
extern struct s_GUI_CTRL _FAR * gfp_CTRL_Control_Table;  // dseg:A8A2



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg033
*/
// _s33p01
void GUI_SetWindows(int argWindowCount, struct s_GUI_WINDOW *argWindowArray);
// _s33p02
void GUI_FindWindow(int X_Pos, int Y_Pos);
// _s33p03
int CR_GetIndex(void);
// _s33p04
int CR_GetOffset(void);
// _s33p05
int MD_GetButtonStatus(void);
// _s33p06
int MD_WaitRelease(void);
// _s33p07
int MD_Init(void);
// _s33p09
void MD_Reset(void);
// _s33p10
void MD_INT_SetMvOnly(void);
// _s33p11
void MD_INT_SetMvBtns(void);
// _s33p12
void MD_INT_Handler(void);
// _s33p13
void MD_CDraw_Disable(void);
// _s33p14
void MD_CDraw_Restore(void);
// _s33p16.asm
int KD_CheckBuffer(void);
// _s33p17.asm
int KD_GetKeyCode(void);
// _s33p18c.c
int MD_GetX(void);
// _s33p19c.c
int MD_GetY(void);
// _s33p20
void MD_MoveCursor(int X_Pos, int Y_Pos);
// _s33p21
void MD_MoveCursorINT(int X_Pos, int Y_Pos);
// _s33p22c.c
int MD_GetClickX(void);
// _s33p23c.c
int MD_GetClickY(void);
// _s33p24c.c
int MD_GetClickedBtns(void);

// ...
// _s33p25c.c
int MD_GetClickRec1(void);
// _s33p26c.c
int MD_GetClickRec2(void);
// ...
// _s33p29
int MD_SaveClick(int X_Pos, int Y_Pos, int Buttons);

/*
    seg034
*/
// _s34p01.asm
int IN_ProcessInput(void);
// _s34p08
void IN_SetEscOverride(void);

// _s34p12
void HLP_SetHelp(int Entry_Tbl, int Entry_Count);
// _s34p13
void HLP_ClearHelp(void);

// _s34p22.asm
int GUI_NoTriggerKeys(int *Retn_Pointer);

// _s34p25
int GUI_MousedControl(void);
// _s34p26
int GUI_MouseOverControl(void);


// _s34p57c.c
void CTRL_Clear(void);

// _s34p65
void IN_Init(int Input_Type);

// _s34p66
int IN_GetInput(void);
// _s34p67
void IN_Set_Skip(int Input_Skip);


#ifdef __cplusplus
}
#endif

#endif /* ST_GUI_H */
