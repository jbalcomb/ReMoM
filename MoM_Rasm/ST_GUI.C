
#include "ST_GUI.H"

/*
    Initialized Data
*/

int g_GUI_CurrentCursor = 0;                // dseg:438A
int g_GUI_PrevCursor = 0;                   // dseg:438C
int g_MOUSE_CursorDraw = 0;                 // dseg:438E
int g_MouseDriverInstalled = 0;             // dseg:4390
/* dseg:4392 00 00                         MOUSE_Usable dw 0 */
int g_MOUSE_CurrentX = 158;                 // dseg:4394
int g_MOUSE_CurrentY = 100;                 // dseg:4396
int g_GUI_WindowCount = 1;                  // dseg:4398
int g_GUI_CursorOffset = 0;                 // dseg:439A
int g_MOUSE_ClickRec1 = 0;                  // dseg:439C
int g_MOUSE_ClickX = 0;                     // dseg:439E
int g_MOUSE_ClickY = 0;                     // dseg:43A0
int g_MOUSE_ClickBtns = 0;                  // dseg:43A2
int g_MOUSE_ClickRec2 = 0;                  // dseg:43A4
int g_MOUSE_INT_Process = 0;                // dseg:43A6
int g_GUI_HaveAreaSave = 0;                 // dseg:43A8
int g_MOUSE_InitX = 158;                    // dseg:43AA
int g_MOUSE_InitY = 100;                    // dseg:43AC
int g_MOUSE_CDraw_Save = 0;                 // dseg:43AE
struct s_GUI_WINDOW *g_GUI_Windows;         // dseg:43B0
// extern struct s_Cursor_Area_Copy Cursor_Save_Main; //dseg:43B2 
// // dseg:4538  REUSABLE_810bytes   db 32Ah dup(0)
unsigned int g_Cursor_Save_Main[600];
// extern struct s_Cursor_Area_Copy Cursor_Save_Bkup; //dseg:4862 
// // dseg:49E8  REUSABLE_810bytes_2 db 32Ah dup(0)
unsigned int g_Cursor_Save_Bkup[600];

// dseg:4D12 00 00                         UU_GUI_ClickBufferX dw 0
// dseg:4D14 00 00                         UU_GUI_ClickBuffNext dw 0
// dseg:4D16 00 00                         UU_GUI_ClickBuffVar1 dw 0
// dseg:4D18 00 00                         UU_GUI_ClickBuffer@ dw 0
// dseg:4D1A 00 00                         UU_GUI_ClickBufCount dw 0
int g_GUI_RedrawFn_Present = 0;             // dseg:4D1C
int g_GUI_Last_Key_Control = 0;             // dseg:4D1E
int g_MOUSE_Hardware;                       // dseg:4D20 
int g_GUI_FocusedControl = -1;              // dseg:4D22
/*
dseg:4D24 01 00                         GUI_BtnClk_TextLeft dw 1                ; added to the Left position of text drawn on buttons when clicked
dseg:4D26 FF FF                         GUI_BtnClk_TextTop dw -1                ; added to the Top position of text drawn on buttons when clicked
dseg:4D28 FF FF                         GUI_EmptyTitleHelp dw 0FFFFh
*/
int g_GUI_DialogDirections = 0;             // dseg:4D2A
int g_GUI_Redraw_Timer = 2;                 // dseg:4D2C
struct s_GUI_WINDOW g_GUI_VirtualScreen[1] = {0, 0, 0, 319, 199, 0};    // dseg:4D2E
int g_GUI_MouseFocusCtrl = 0;               // dseg:4D3A
// dseg:4D3C GUI_MouseHighlight dw 1
// dseg:4D3E GUI_NoDialogWrap dw 0
// dseg:4D40 ; int GUI_NoTxtSelectColor
// dseg:4D40 GUI_NoTxtSelectColor dw 0
// dseg:4D42 GUI_OnLastDialogLine dw 0
int g_GUI_EscapeOverride = 0;               // dseg:4D44
int g_GUI_ClickActivate = 0;                // dseg:4D46
// dseg:4D48 aNotInUse db 'NOT IN USE',0
// dseg:4D53 align 2
// dseg:4D54 GUI_Edit_Position dw 0
int g_GUI_EditAnimStage = 0;                // dseg:4D56
int g_GUI_EditCursorOn = 0;                 // dseg:4D58
int g_GUI_InEditSelect = 0;                 // dseg:4D5A
int g_DBG_ControlHighlight = ST_FALSE;      // dseg:4D5C 
int g_DBG_HelpHighlight = ST_FALSE;         // dseg:4D5E
int g_GUI_Active_EditSlct = -1;             // dseg:4D60
// dseg:4D62 GUI_EditTimeOutType dw 0
// dseg:4D64 UU_GUI_UUClickBufVar dw 0
// dseg:4D66 UU_GUI_ClickBuffer dw 0
int g_GUI_Help_Available = 0;               // dseg:4D68
int g_HLP_Entry_Count = 0;                  // dseg:4D6A
int g_GUI_ActiveStringTrig = 0;             // dseg:4D6C
// dseg:4D6E UU_cnst_RB7 db 'rb'
// dseg:4D70 ; char cnst_ZeroString_1
// dseg:4D70 cnst_ZeroString_1 db 0
// dseg:4D71 UU_cnst_WB7 db 'wb',0
// dseg:4D74 ; int UU_GUI_Up_Hotkey
// dseg:4D74 UU_GUI_Up_Hotkey db    5,   0                            ; 0
// dseg:4D76 ; int UU_GUI_Down_Hotkey
// dseg:4D76 UU_GUI_Down_Hotkey db    6,   0                            ; 0
// dseg:4D78 GUI_EditCursor db '_',0


/*
    Uninitialized Data
*/

int g_HLP_Entry_Table;                      // dseg:A868
int g_GUI_Processed_Btns;                   // dseg:A86A
int g_GUI_Processed_LastY;                  // dseg:A86C
int g_GUI_Processed_LastX;                  // dseg:A86E
char g_GUI_EditString[30];                  // dseg:A870
int g_GUI_Delay;                            // dseg:A88E
int g_MOUSE_Emu_Y;                          // dseg:A890
int g_MOUSE_Emu_X;                          // dseg:A892
int g_GUI_Cursor_Offset;                    // dseg:A894
// dseg:A896 GUI_PrevControlCount dw 0
int g_GUI_Control_Count;                    // dseg:A898
// dseg:A89A gfp_GUI_Prev_Redraw_Fn dd 0
//void _FAR *gfp_GUI_Redraw_Function;         // dseg:A89E 
//void      (*uiobj_callback)(void *) = NULL;
void (*gfp_GUI_Redraw_Function)(void);
//void _FAR *gfp_GUI_Control_Table;           // dseg:A8A2
struct s_GUI_CTRL _FAR *gfp_GUI_Control_Table;  // dseg:A8A2
