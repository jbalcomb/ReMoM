#ifndef FIELDS_H
#define FIELDS_H

#include "MoX_TYPE.H"

#include <assert.h>


#ifdef __cplusplus
extern "C" {
#endif



// NOTE(JimBalcomb,20230628): Don't recall how/why I got to putting this in here. Should be with notes about C arrays, strings, char *'s, **'s?
/*
C:\STU\developc\moo2x-master\src\moo2x\Platform.cpp

Line 5194
#define ESTRINGS__strings ((i32**)(0x0056B788))
i32 __cdecl ESTRINGS_E_Strings_(__int16 index) // 0x004A0050
{
	char* addr = (char*)(ESTRINGS__strings[index]);
	//pLog.Printf("%d = %s\n",index,addr);
  return (i32)ESTRINGS__strings[index];
}

Line 5227
p=&ESTRINGS__strings[368];
*p = (i32*)maintextlbxs[k];

Line 5265
sprintf(v13, (char*)ESTRINGS_E_Strings_(42));

*/



enum e_Field_Type
{
    ft_Button,                  /*  0  0x00 */  //drake178: TODO
    ft_RadioButton,             /*  1  0x01 */  //drake178: ToggleButton
    ft_LockedButton,            /*  2  0x02 */  //drake178: LockableButton
    ft_MultiButton,             /*  3  0x03 */  //drake178: MStateButton
    ft_Input,                   /*  4  0x04 */  //drake178: EditBox
    ft_ImageLabel,              /*  5  0x05 */  //drake178: ImageLabel      DNE/NIU in MoO2
    ft_Scroll,                  /*  6  0x06 */  //drake178: SlideBar
    ft_HotKey,                  /*  7  0x07 */  //drake178: Label
    ft_MultiHotKey,             /*  8  0x08 */  //drake178: Ctrl_AltString
    ft_ClickLink,               /*  9  0x09 */  //drake178: ClickLink       DNE/NIU in MoO2
    ft_StringList,              /* 10  0x0A */  //drake178: DialogLine
    ft_ContinuousStringInput,   /* 11  0x0B */  //drake178: EditSelect
    ft_Grid                     /* 12  0x0C */  //drake178: TODO
    // MoO2 ft_IrregularButton  /* 13  0x0D */
    // MoO2 ft_Circle           /* 14  0x0E */
    // MoO2 ?!? Add_String_Field() ?!?
};

struct s_Field
{
    int16_t x1;         // 
    int16_t y1;         // 
    int16_t x2;         // 
    int16_t y2;         // 
    int16_t type;       // 
    int16_t help;       // 
    int16_t Font_Index; // 
    int16_t ColorSet1;  // 
    int16_t Rect_FX;    // 
    int16_t Selectable; // 
    int16_t ColorSet2;  // 
                        // ? ft 0 ?      ft 4      ft 6     ft 8    ft 9    ft 10   ft 11     ft 12
    int16_t Param0;     // Text@                   Min_Val  Str*    Parent
    int16_t Param1;     // Trig_State              Max_Val  StrIdx  Index   Index   IMG       box_width
    int16_t Param2;     // State@        BG_Color  StSeg    StrLen                  BG_Color  box_height
    // int16_t Param3;  // FullFrames    Align     MinSlide                 CBlock            xpos
    // int16_t Param4;  // MaxSlide      CStyle                                               ypos
    int64_t Param3;     // FullFrames    Align     MinSlide                 CBlock            xpos
    int64_t Param4;     // MaxSlide      CStyle                                               ypos
    int16_t Param5;     // IMG_Seg       MaxLen    Orient                           MaxLen
    union {
        SAMB_ptr pict_seg;  // ft_Button
        int16_t MaxLen;     // ft_EditBox, ft_EditSelect
        int16_t Orient;     // ft_SlideBar
    };
    int16_t Param6;     // CursorColors            StOff                            Outline
    int16_t hotkey;     // ASCII Character *Code*
    int16_t Unused_25h; // ?IDK/IDGI?
    // union { int16_t Param1, Max_Val, Index, IMG, box_width; };

};
// TODO(JimBalcomb,20230614): ASSERT sizeof(struct s_Field)



/*
    WZD dseg:78B8                           BEGIN: Fields - Initialized Data
*/

// WZD dseg:78B8 current_pointer_image_number dw 0       
// WZD dseg:78BA previous_pointer_image_number dw 0      
// WZD dseg:78BC mouse_enabled dw 0                      
// WZD dseg:78BE mouse_driver_installed dw 0             
// WZD dseg:78C0 MOUSE_Usable dw 0                       
// WZD dseg:78C2 mouse_x dw 158                          
// WZD dseg:78C4 mouse_y dw 100                          
// WZD dseg:78C6 current_mouse_list_count dw 1           
// WZD dseg:78C8 current_pointer_offset dw 0             
// WZD dseg:78CA MOUSE_ClickRec1 dw 0                    
// WZD dseg:78CC MOUSE_ClickX dw 0                       
// WZD dseg:78CE MOUSE_ClickY dw 0                       
// WZD dseg:78D0 MOUSE_ClickBtns dw 0                    
// WZD dseg:78D2 MOUSE_ClickRec2 dw 0                    
// WZD dseg:78D4 MOUSE_INT_Process dw 0                  
// WZD dseg:78D6 mouse_save_flag dw 0                    
// WZD dseg:78D8 MOUSE_InitX dw 158                      
// WZD dseg:78DA MOUSE_InitY dw 100                      
// WZD dseg:78DC mouse_state dw 0                        
// WZD dseg:78DE current_mouse_list_0 dw 0               
// WZD dseg:78E0 mouse_background_buffer dw 600 dup(0)
// WZD dseg:7D90 mouse_off_page_buffer dw 600 dup(0) 
// WZD dseg:8240 UU_GUI_ClickBufferX dw 0                
// WZD dseg:8242 UU_GUI_ClickBuffNext dw 0               
// WZD dseg:8244 UU_GUI_ClickBuffVar1 dw 0               
// WZD dseg:8246 UU_GUI_ClickBuffer@ dw 0                
// WZD dseg:8248 UU_GUI_ClickBufCount dw 0               


// WZD dseg:824A
// drake178: GUI_RedrawFn_Present
// AKA g_SCRN_RedrawFn_Present
// AKA redraw_function_flag
// MoO2  Module: fields  auto_active_flag
// PRIVATE  extern uint16_t auto_active_flag;


// WZD dseg:824C KD_prev_field_idx dw 0                  

// WZD dseg:824E
// DONT  mouse_installed = ST_FALSE;                     

// WZD dseg:8250
extern int16_t down_mouse_button;

// WZD dseg:8252
extern int16_t down_x;

// WZD dseg:8254
extern int16_t down_y;

// WZD dseg:8256 GUI_EmptyTitleHelp dw 0FFFFh            
// WZD dseg:8258 GUI_DialogDirections dw 0               


// WZD dseg:825A
// drake178: GUI_Redraw_Timer
// AKA g_SCRN_Redraw_Timer
// AKA redraw_timer
// MoO2  Module: fields  auto_function_delay
// PIVATE  extern uint16_t auto_function_delay;


// WZD dseg:825C
// AKA mouse_list_none_init s_MOUSE_LIST <0, 0, 0, 319, 199, 0>
// ¿ int16_t mouse_list_init_count = 1; ?
// WZD dseg:825C
// DEDUCE: only XREF in Init_Mouse_Keyboard() ?
extern struct s_mouse_list mouse_list_init[];

// WZD dseg:8268
// AKA GUI_MouseFocusCtrl
extern int16_t GUI_MouseFocusCtrl;
// ¿ int16_t focused_field = ST_UNDEFINED; ?
// ¿ int16_t mouse_field = ST_FALSE; ?

// WZD dseg:826A GUI_MouseHighlight dw 1                 
// WZD dseg:826C GUI_NoDialogWrap dw 0                   
// WZD dseg:826E GUI_NoTxtSelectColor dw 0               
// WZD dseg:8270 GUI_OnLastDialogLine dw 0               

// WZD dseg:8272
extern int16_t _global_esc;  // ERROR: mistook for mouse_cancel_disabled, for many moons
extern int16_t mouse_cancel_disabled;

// WZD dseg:8274
extern int16_t GUI_ClickActivate;
// WZD dseg:8276 aNotInUse db 'NOT IN USE',0
// WZD dseg:8281 db    0
// WZD dseg:8282 GUI_Edit_Position dw 0                  
// WZD dseg:8284 GUI_EditAnimStage dw 0                  
// WZD dseg:8286 GUI_EditCursorOn dw 0                   

// WZD dseg:8288
extern int16_t input_field_active;

// WZD dseg:828A
extern int16_t DBG_ControlHighlight;
// WZD dseg:828C
extern int16_t DBG_HelpHighlight;

// WZD dseg:828E
// AKA GUI_Active_EditSlct
extern int16_t active_input_field_number;

// WZD dseg:8290 GUI_EditTimeOutType dw 0                
// WZD dseg:8292 UU_GUI_UUClickBufVar dw 0               
// WZD dseg:8294 UU_GUI_ClickBuffer dw 0                 

// WZD dseg:8296
// HELP  int16_t help_list_active = ST_FALSE;

// WZD dseg:8298
// HELP  int16_t help_list_count = 0;

// WZD dseg:829A KD_ActiveStringTrig dw 0                
// WZD dseg:829C UU_cnst_RB7 db 'rb'                     
// WZD dseg:829E cnst_ZeroString_12 db 0                 
// WZD dseg:829F UU_cnst_WB8 db 'wb',0                   
// WZD dseg:82A2 UU_GUI_Up_Hotkey db 5, 0                
// WZD dseg:82A4 UU_GUI_Down_Hotkey db 6, 0              
// WZD dseg:82A6 GUI_EditCursor db '_',0                 

/*
    WZD dseg:82A6                           END: Fields - Initialized Data
*/


/*
    WZD dseg:E872                           BEGIN: Fields - Uninitialized Data
*/

// WZD dseg:E872
// AKA _help_list
// HELP  help_struct_pointer

// dseg:E874 GUI_Processed_Btns dw 0    
// dseg:E876 GUI_Processed_LastY dw 0   
// dseg:E878 GUI_Processed_LastX dw 0   

// WZD dseg:E87A
// drake178: GUI_EditString
extern char continuous_string[];

// WZD dseg:E898
extern int16_t input_delay;

// dseg:E89A MOUSE_Emu_Y dw 0           
// dseg:E89C MOUSE_Emu_X dw 0           

// WZD dseg:E89E
extern int16_t cursor_offset;

// dseg:E8A0 GUI_PrevControlCount dw 0  

// WZD dseg:E8A2
extern int16_t fields_count;



// WZD dseg:E8A4 
// drake178: GUI_Prev_Redraw_Fn
// AKA gfp_GUI_Prev_Redraw_Fn
// AKA gfp_CTRL_Prev_Redraw_Fn
// MoO2  Module: fields  function2
// PRIVATE  void (*function2)(void);

// WZD dseg:E8A8
// drake178: GUI_Redraw_Fn
// AKA GUI_Redraw_Function
// AKA gfp_GUI_Redraw_Function
// AKA gfp_SCRN_Redraw_Function
// AKA Redraw_Function
// 1oom  void (*uiobj_callback)(void *) = NULL;
// MoO2  Module: fields  function
// PRIVATE  void (*function)(void);



// WZD dseg:E8AC
extern struct s_Field * p_fields;  // "p_fields dd 0" ? Far Pointer?
// MoO2  Module: fields
// array (13750 bytes) sfields
// Address: 02:001A8848
// Num elements:  250, Type:        unsigned integer (4 bytes) 
// 13750 / 250 = 55  sizeof(struct field)
// Why the pointer `p_fields` if `sfields` is statically-allocated?


/*
    WZD dseg:E8AC                           END: Fields - Uninitialized Data
*/




/*
    WZD  seg036
    MGC  seg034
*/

// WZD s36p01
// INPUT  int16_t Interpret_Mouse_Input(void);

// WZD s36p08
// TODO  void Set_Global_Esc(void);
void Enable_Cancel(void);
// WZD s36p09
//drake178: UU_GUI_ClearEscOverride
void Disable_Cancel(void);


/*
    BEGIN: Help
*/
// WZD s36p12
// HELP  void Set_Help_List(struct s_Help_Entry * help_pointer, int16_t count);
// WZD s36p13
// HELP  void Deactivate_Help_List(void);
// WZD s36p14
// HELP  void Check_Help_List(void);
// WZD o159p01
// HELP Draw_Help_Entry_()
/*
    END: Help
*/

// WZD s36p22
// KD int16_t Interpret_Keyboard_Input(char  * character);

// WZD s36p25
// INPUT  int16_t Scan_Input(void);
// WZD s36p26
// INPUT  int16_t Scan_Field(void);


// WZD s36p38
int16_t Add_Multi_Hot_Key_Field(char * string);


// WZD s36p42
void Add_Button_Info(int16_t xmin, int16_t ymin, int16_t string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help);

// WZD s36p43
int16_t Add_Button_Field(int16_t xmin, int16_t ymin, int16_t string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help);

// WZD s36p47
int16_t Add_Hidden_Field(int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax, int16_t hotkey, int16_t help);

// WZD s36p49
int16_t Add_Hot_Key(int16_t select_char);

// WZD s36p49
// int16_t Add_Grid_Field(int16_t xmin, int16_t ymin, int16_t box_width, int16_t box_height, int16_t horizontal_count, int16_t vertical_count, int16_t *xpos, int16_t *ypos, int16_t help);
int16_t Add_Grid_Field(int16_t xmin, int16_t ymin, int16_t box_width, int16_t box_height, int16_t horizontal_count, int16_t vertical_count, int64_t *xpos, int64_t *ypos, int16_t help);

// WZD s36p57
void Clear_Fields(void);


// WZD s36p66
// INPUT  int16_t Get_Input(void);

// WZD s36p67
// INPUT  void Set_Input_Delay(int16_t input_delay);


// MoO2: Draw_Visible_Fields()
// WZD s36p71
void Draw_Fields(void);

// WZD s36p72
void Draw_Field(int16_t field_num, int16_t up_down_flag);

// WZD s36p73
void Push_Field_Down(int16_t field_num, int16_t mouse_x, int16_t mouse_y);




// WZD s36p74
// UU_GUI_SetDlgTitleHelp()

// WZD s36p75
// UU_GUI_ClearTitleHelp()


// WZD s36p76
// drake178: GUI_SetRedrawFn()
// AKA Set_Redraw_Function()
// AKA SCRN_Set_Redraw_Function()
// AKA GUI_Set_Redraw_Function()
// MoO2  Module: fields  Assign_Auto_Function()
void Assign_Auto_Function(void (*auto_function)(void), uint16_t delay);


// WZD s36p77
// drake178: GUI_SaveRedrawFn()
// AKA SCRN_SaveRedrawFn()
// MoO2  Module: fields  Save_Auto_Function()
void Save_Auto_Function(void);

// WZD s36p78
// drake178: GUI_LoadRedrawFn
// AKA SCRN_LoadRedrawFn()
// MoO2  Module: fields  Restore_Auto_Function()
void Restore_Auto_Function(void);


// WZD s36p79
// drake178: GUI_DisableRedraw()
// AKA Disable_Redraw_Function()
// AKA SCRN_DisableRedraw()
// MoO2  Module: fields  Deactivate_Auto_Function()
void Deactivate_Auto_Function(void);

// WZD s36p80
// drake178: UU_GUI_EnableRedraw(void)
// MoO2  Module: fields  Activate_Auto_Function()
void UU_Activate_Auto_Function(void);


// WZD s36p81
// drake178: GUI_CallRedrawFn
// AKA Call_Redraw_Function()
// AKA SCRN_CallRedrawFn()
// MoO2  Module: fields  Invoke_Auto_Function()
void Invoke_Auto_Function(void);


// WZD s36p82
// drake178: 
// AKA SCRN_Redraw_WaitOne
// AKA GUI_Redraw_WaitOne()
// AKA GUI_1TickRedraw()
// MoO2  Module: fields  Quick_Call_Auto_Function()
void Quick_Call_Auto_Function(void);


// WZD s36p83
// drake178: GUI_NormalRedraw()
// AKA GUI_Redraw_WaitTimer
// AKA SCRN_Redraw_WaitTimer
// MoO2  Module: fields  Call_Auto_Function()
void Call_Auto_Function(void);



// WZD s36p84
// drake178: UU_GUI_ExtClear()
void UU_GUI_ExtClear(void);



// WZD s36p85
// INPUT  void Toggle_Pages(void);



#ifdef __cplusplus
}
#endif



#endif /* FIELDS_H */
