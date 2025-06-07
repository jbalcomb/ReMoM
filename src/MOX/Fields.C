
#include "MOX_Lib.H"



/*
    WIZARDS.EXE
    Data Segment (DSEG)
    Initialized Data
    Uninitialized Data
    seg035
    seg036
    Begin: dseg:E872
    End:   dseg:E8B0
    Last:  dseg:E8AC

    WZD dseg:E872                           BEGIN: Fields - Uninitialized Data
    WZD dseg:E8AC                           END: Fields - Uninitialized Data
    WZD dseg:78B8                           BEGIN: Fields - Initialized Data
    WZD dseg:82A6                           END: Fields - Initialized Data
*/

/*
dseg:E872                                                 ? BEGIN: fields, intput, ... ?
dseg:E872
dseg:E872                                                 ¿ BEGIN: Help - Uninitialized Data ?
dseg:E872 00 00                                           help_struct_pointer dw 0                ; DATA XREF: Set_Help_List+6w ...
dseg:E872                                                 ¿ END: Help - Uninitialized Data ?
dseg:E874 00 00                                           GUI_Processed_Btns dw 0                 ; DATA XREF: Interpret_Mouse_Input+9E9w ...
dseg:E874                                                                                         ; can possibly be repurposed
dseg:E876 00 00                                           GUI_Processed_LastY dw 0                ; DATA XREF: Interpret_Mouse_Input+9E3w ...
dseg:E876                                                                                         ; can possibly be repurposed
dseg:E878 00 00                                           GUI_Processed_LastX dw 0                ; DATA XREF: Interpret_Mouse_Input+9DDw ...
dseg:E878                                                                                         ; can possibly be repurposed
dseg:E87A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+GUI_EditString db 30 dup(0)             ; DATA XREF: Interpret_Mouse_Input+2E0o ...
dseg:E898 00 00                                           input_delay dw 0                        ; DATA XREF: Init_Mouse_Keyboard+B1w ...
dseg:E89A 00 00                                           MOUSE_Emu_Y dw 0                        ; DATA XREF: RP_GUI_KeyInputOnly+Er ...
dseg:E89C 00 00                                           MOUSE_Emu_X dw 0                        ; DATA XREF: RP_GUI_KeyInputOnly+8r ...
dseg:E89E 00 00                                           pointer_offset dw 0                      ; DATA XREF: Interpret_Mouse_Input+33w ...
dseg:E8A0 00 00                                           temp_field_count dw 0               ; DATA XREF: G_GUI_PressAnyKey+13w ...
dseg:E8A2 00 00                                           fields_count dw 0                       ; DATA XREF: Interpret_Mouse_Input:loc_26C37r ...
dseg:E8A4 00 00 00 00                                     GUI_Prev_Redraw_Fn dd 0                 ; DATA XREF: GUI_SaveRedrawFn+Dw ...
dseg:E8A8 00 00 00 00                                     GUI_Redraw_Function dd 0                ; DATA XREF: Set_Redraw_Function+Fw ...
dseg:E8AC 00 00 00 00                                     p_fields dd 0                           ; DATA XREF: Interpret_Mouse_Input+11Dr ...
dseg:E8AC                                                                                         ; 165h LBX_Alloc_Space paragraphs (can hold 150)
dseg:E8AC                                                 ? END: fields, input, ... ?
*/
/*
dseg:78B8                                                 BEGIN: Fields, Input, Mouse, Keyboard - Initialized Data
dseg:78B8
dseg:78B8
dseg:78B8                                                 BEGIN: Mouse - Initialized Data
dseg:78B8
dseg:78B8 00 00                                           current_pointer_image_number dw 0       ; DATA XREF: Set_Mouse_List:loc_2583Aw ...
dseg:78BA 00 00                                           previous_pointer_image_number dw 0      ; DATA XREF: Check_Mouse_Shape+Fw ...
dseg:78BC 00 00                                           mouse_enabled dw 0                      ; DATA XREF: Init_Mouse_Driver+78w ...
dseg:78BE 00 00                                           mouse_driver_installed dw 0             ; DATA XREF: Mouse_Button+7r ...
dseg:78C0 00 00                                           MOUSE_Usable dw 0                       ; DATA XREF: RP_MOUSE_SetUsable+6w ...
dseg:78C2 9E 00                                           mouse_x dw 158                          ; DATA XREF: Init_Mouse_Driver+40w ...
dseg:78C4 64 00                                           mouse_y dw 100                          ; DATA XREF: Init_Mouse_Driver+44w ...
dseg:78C6 01 00                                           current_mouse_list_count dw 1           ; DATA XREF: Set_Mouse_List+Cw ...
dseg:78C8 00 00                                           current_pointer_offset dw 0             ; DATA XREF: Set_Mouse_List+39w ...
dseg:78CA 00 00                                           mouse_buffer_flag dw 0                    ; DATA XREF: Init_Mouse_Driver+7Ew ...
dseg:78CC 00 00                                           mouse_buffer_x dw 0                       ; DATA XREF: Mouse_Buffer_X+6r ...
dseg:78CE 00 00                                           mouse_buffer_y dw 0                       ; DATA XREF: Mouse_Buffer_Y+6r ...
dseg:78D0 00 00                                           mouse_buffer_button dw 0                    ; DATA XREF: Mouse_Buffer_Button+6r ...
dseg:78D2 00 00                                           mouse_buffer_flag2 dw 0                    ; DATA XREF: Init_Mouse_Driver+84w ...
dseg:78D4 00 00                                           MOUSE_INT_Process dw 0                  ; DATA XREF: Init_Mouse_Driver+32w ...
dseg:78D6 00 00                                           mouse_save_flag dw 0                    ; DATA XREF: Save_Mouse_On_Page+16r ...
dseg:78D8 9E 00                                           MOUSE_InitX dw 158                      ; DATA XREF: Init_Mouse_Driver+3Cr ...
dseg:78DA 64 00                                           MOUSE_InitY dw 100                      ; DATA XREF: Init_Mouse_Driver+38r ...
dseg:78DC 00 00                                           mouse_state dw 0                        ; DATA XREF: Save_Mouse_State+Bw ...
dseg:78DE 00 00                                           current_mouse_list_0 dw 0               ; DATA XREF: Set_Mouse_List+15w ...
dseg:78E0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+mouse_background_buffer dw 600 dup(    0)
dseg:78E0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; DATA XREF: Save_Mouse_On_Page+60w ...
dseg:7D90 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+mouse_off_page_buffer dw 600 dup(    0) ; DATA XREF: Save_Mouse_Off_Page+6Bw ...
dseg:8240                                                 ? BEGIN: Mouse Buffer - Initialized Data ?
dseg:8240 00 00                                           UU_GUI_ClickBufferX dw 0                ; DATA XREF: UU_GUI_ClearClickBuffer+17w ...
dseg:8240                                                                                         ; can be reused
dseg:8242 00 00                                           UU_GUI_ClickBuffNext dw 0               ; DATA XREF: UU_GUI_ClearClickBuffer+1Aw ...
dseg:8242                                                                                         ; can be reused
dseg:8244 00 00                                           UU_GUI_ClickBuffVar1 dw 0               ; DATA XREF: UU_GUI_SetClickBuffVar1w ...
dseg:8244                                                                                         ; can be reused
dseg:8246 00 00                                           UU_GUI_ClickBuffer@ dw 0                ; DATA XREF: UU_GUI_ClearClickBufferr ...
dseg:8246                                                                                         ; can be reused
dseg:8248 00 00                                           UU_GUI_ClickBufCount dw 0               ; DATA XREF: UU_GUI_ClearClickBuffer+Br ...
dseg:8248                                                                                         ; can be reused
dseg:824A 00 00                                           GUI_RedrawFn_Present dw 0               ; DATA XREF: Set_Redraw_Function+13w ...
dseg:824C 00 00                                           KD_prev_field_idx dw 0                  ; DATA XREF: Interpret_Keyboard_Input+13r ...
dseg:824E 00 00                                           mouse_installed dw 0                    ; DATA XREF: UU_GUI_MouseEMUMoveTo+Br ...
dseg:8250 FF FF                                           down_mouse_button dw 0FFFFh             ; DATA XREF: Interpret_Mouse_Input+Dw ...
dseg:8252 01 00                                           down_x dw 1                             ; DATA XREF: Set_Button_Down_Offsets+6w ...
dseg:8252                                                                                         ; offset added to 'down' button field text
dseg:8254 FF FF                                           down_y dw -1                            ; DATA XREF: Set_Button_Down_Offsets+Cw ...
dseg:8254                                                 ? END: Mouse - Initialized Data ?       ; offset added to 'down' button field text
dseg:8256 FF FF                                           GUI_EmptyTitleHelp dw 0FFFFh            ; DATA XREF: UU_GUI_ScrollableTxtDlg+1BEr ...
dseg:8258 00 00                                           GUI_DialogDirections dw 0               ; DATA XREF: Interpret_Mouse_Input:loc_26BD8r ...
dseg:825A 02 00                                           GUI_Redraw_Timer dw 2                   ; DATA XREF: Draw_Input_Box_Popup+3D7r ...
dseg:825C                                                 ; #314 mouse_list_none_init
dseg:825C 00 00 00 00 00 00 3F 01 C7 00 00 00             mouse_list_none_init s_MOUSE_LIST <0, 0, 0, 319, 199, 0>
dseg:825C                                                                                         ; DATA XREF: Init_Mouse_Keyboard:loc_2D1C5o
dseg:8268 00 00                                           GUI_MouseFocusCtrl dw 0                 ; DATA XREF: Interpret_Mouse_Input+18w ...
dseg:826A 01 00                                           GUI_MouseHighlight dw 1                 ; DATA XREF: UU_GUI_ScrollableTxtDlg:loc_2C67Fr ...
dseg:826C 00 00                                           GUI_NoDialogWrap dw 0                   ; DATA XREF: UU_GUI_ScrollableTxtDlg+11w ...
dseg:826E 00 00                                           GUI_NoTxtSelectColor dw 0               ; DATA XREF: UU_GUI_NoSelectColor+Fw ...
dseg:8270 00 00                                           GUI_OnLastDialogLine dw 0               ; DATA XREF: UU_GUI_ScrollableTxtDlg+17w ...
dseg:8272 00 00                                           _global_esc dw 0                        ; DATA XREF: Interpret_Mouse_Input+C3r ...
dseg:8274 00 00                                           GUI_ClickActivate dw 0                  ; DATA XREF: Interpret_Mouse_Input+2FCr ...
dseg:8276 4E 4F 54 20 49 4E 20 55 53 45 00                aNotInUse db 'NOT IN USE',0
dseg:8281 00                                              db    0
dseg:8282 00 00                                           GUI_Edit_Position dw 0                  ; DATA XREF: Input_Box_Popup:loc_29A23w ...
dseg:8284 00 00                                           GUI_EditAnimStage dw 0                  ; DATA XREF: Interpret_Mouse_Input+35Aw ...
dseg:8286 00 00                                           GUI_EditCursorOn dw 0                   ; DATA XREF: Interpret_Mouse_Input+360w ...
dseg:8288 00 00                                           input_field_active dw 0                 ; DATA XREF: Interpret_Mouse_Input:loc_26941r ...
dseg:828A 00 00                                           field_box_mode dw 0               ; DATA XREF: Interpret_Mouse_Input+DCr ...
dseg:828C 00 00                                           help_box_mode dw 0                  ; DATA XREF: Interpret_Mouse_Input+100r ...
dseg:828E FF FF                                           active_input_field_number dw 0FFFFh     ; DATA XREF: Interpret_Mouse_Input+2DCr ...
dseg:8290 00 00                                           GUI_EditTimeOutType dw 0                ; DATA XREF: UU_GUI_SetTimeOutP+14w ...
dseg:8292 00 00                                           UU_GUI_UUClickBufVar dw 0               ; DATA XREF: UU_GUI_SetClickBuffer+27w
dseg:8294 00 00                                           UU_GUI_ClickBuffer dw 0                 ; DATA XREF: UU_GUI_SetTimeOutP+3r ...
dseg:8296                                                 ¿ BEGIN: Help - Initialized Data ?
dseg:8296 00 00                                           help_list_active dw 0                   ; DATA XREF: Interpret_Mouse_Input+752r ...
dseg:8298 00 00                                           help_list_count dw 0                    ; DATA XREF: Set_Help_List+12w ...
dseg:8298                                                 ¿ END: Help - Initialized Data ?
dseg:829A                                                 ? BEGIN: Mouse Buffer ?
dseg:829A 00 00                                           multi_hotkey_active_field dw 0                ; DATA XREF: Interpret_Keyboard_Input:loc_27F9Br ...
dseg:829C 72 62                                           UU_cnst_RB7 db 'rb'                     ; DATA XREF: UU_GUI_LoadClickFile+Eo
dseg:829E 00                                              cnst_ZeroString_12 db 0                 ; DATA XREF: Setup_Input_Box_Popup+3Bo ...
dseg:829F 77 62 00                                        UU_cnst_WB8 db 'wb',0                   ; DATA XREF: UU_GUI_SaveClickFile+Bo
dseg:829F                                                 ? END: Mouse Buffer ?
dseg:82A2 05 00                                           UU_GUI_Up_Hotkey db 5, 0                ; DATA XREF: UU_GUI_ScrollableTxtDlg+21Do ...
dseg:82A4 06 00                                           UU_GUI_Down_Hotkey db 6, 0              ; DATA XREF: UU_GUI_ScrollableTxtDlg+240o ...
dseg:82A6 5F 00                                           GUI_EditCursor db '_',0                 ; DATA XREF: Draw_Fields+117Do
dseg:82A6
dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard
*/

// WZD dseg:8282
int16_t GUI_Edit_Position = 0;
// WZD dseg:8284 00 00                                           GUI_EditAnimStage dw 0                  ; DATA XREF: Interpret_Mouse_Input+35Aw ...
// WZD dseg:8286 00 00                                           GUI_EditCursorOn dw 0                   ; DATA XREF: Interpret_Mouse_Input+360w ...

// WZD dseg:8290
int16_t GUI_EditTimeOutType = 0;

// WZD dseg:829E
char cnst_ZeroString_12[] = "";



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
// WZD dseg:78CA mouse_buffer_flag dw 0                    
// WZD dseg:78CC mouse_buffer_x dw 0                       
// WZD dseg:78CE mouse_buffer_y dw 0                       
// WZD dseg:78D0 mouse_buffer_button dw 0                    
// WZD dseg:78D2 mouse_buffer_flag2 dw 0                    
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
uint16_t auto_active_flag = ST_FALSE;


// WZD dseg:824C KD_prev_field_idx dw 0                  

// WZD dseg:824E
int16_t mouse_installed = ST_FALSE;                     

// WZD dseg:8250
int16_t down_mouse_button = ST_UNDEFINED;

// WZD dseg:8252
int16_t down_x = 1;

// WZD dseg:8254
int16_t down_y = -1;

// WZD dseg:8256 GUI_EmptyTitleHelp dw 0FFFFh            

// WZD dseg:8258
int16_t GUI_DialogDirections = 0;

// WZD dseg:825A
// MoO2  Module: fields  auto_function_delay
uint16_t auto_function_delay = 2;


// WZD dseg:825C
// AKA mouse_list_none_init s_MOUSE_LIST <0, 0, 0, 319, 199, 0>
// ¿ int16_t mouse_list_init_count = 1; ?
// WZD dseg:825C
// DEDUCE: only XREF in Init_Mouse_Keyboard() ?
// MoO2  default_mouse
struct s_mouse_list mouse_list_init[1] = {
    {0, 0, 0, 0, 319, 199}
};

// WZD dseg:8268
// drake178: GUI_MouseFocusCtrl
// AKA mouse_field
// MoO2  Module: fields  auto_input_variable
int16_t auto_input_variable = 0;

// WZD dseg:826A GUI_MouseHighlight dw 1                 
// WZD dseg:826C GUI_NoDialogWrap dw 0                   
// WZD dseg:826E GUI_NoTxtSelectColor dw 0               
// WZD dseg:8270 GUI_OnLastDialogLine dw 0               

// WZD dseg:8272
int16_t _global_esc = ST_FALSE;  // ERROR: mistook for mouse_cancel_disabled, for many moons
int16_t mouse_cancel_disabled = ST_FALSE;

// WZD dseg:8274
int16_t mouse_auto_exit = ST_FALSE;

// WZD dseg:8276
// drake178: aNotInUse
/*
    MoO2
    Module: fields
    array (12 bytes) default_string
    Address: 02:0017C4B8
    Num elements:   12, Type:        unsigned integer (4 bytes) 
*/
/*
    NIU in MGC, WZD, and MoO2
*/
char default_string[12] = "NOT IN USE";

// WZD dseg:8281 db    0  // align 2

// WZD dseg:8282 GUI_Edit_Position dw 0                  

// WZD dseg:8284
int16_t GUI_EditAnimStage;
// WZD dseg:8286
int16_t GUI_EditCursorOn;

// WZD dseg:8288
int16_t input_field_active = ST_FALSE;

// WZD dseg:828A
// DBG_ControlHighlight
// MoO2  Module: fields  field_box_mode
int16_t field_box_mode = ST_FALSE;
// WZD dseg:828C
// DBG_HelpHighlight
// MoO2  Module: fields  help_box_mode
int16_t help_box_mode = ST_FALSE;

// WZD dseg:828E
/*
    field_idx of ft_ContinuousStringInput
*/
int16_t active_input_field_number = ST_UNDEFINED;

// WZD dseg:8290 GUI_EditTimeOutType dw 0                
// WZD dseg:8292 UU_GUI_UUClickBufVar dw 0               
// WZD dseg:8294 UU_GUI_ClickBuffer dw 0                 

// WZD dseg:8296
// HELP  int16_t help_list_active = ST_FALSE;

// WZD dseg:8298
// HELP  int16_t help_list_count = 0;

// WZD dseg:829A multi_hotkey_active_field dw 0                
// WZD dseg:829C UU_cnst_RB7 db 'rb'                     
// WZD dseg:829E cnst_ZeroString_12 db 0                 
// WZD dseg:829F UU_cnst_WB8 db 'wb',0                   
// WZD dseg:82A2 UU_GUI_Up_Hotkey db 5, 0                
// WZD dseg:82A4 UU_GUI_Down_Hotkey db 6, 0              

// WZD dseg:82A6
char GUI_EditCursor[] = "_";

/*
    WZD dseg:82A6                           END: Fields - Initialized Data
*/


/*
    WZD dseg:E872                           BEGIN: seg036 (Fields) - Uninitialized Data
*/

// WZD dseg:E872
// AKA _help_list
// HELP  help_struct_pointer

// WZD dseg:E874
// drake178: GUI_Processed_Btns
// MoO2  Module: fields  last_button_number
int16_t last_button_number;
// WZD dseg:E876
// drake178: GUI_Processed_LastY
// MoO2  Module: fields  last_button_y
int16_t last_button_y;
// WZD dseg:E878
// drake178: GUI_Processed_LastX
// MoO2  Module: fields  last_button_x
int16_t last_button_x;

// WZD dseg:E87A
// drake178: GUI_EditString
char continuous_string[30];

// WZD dseg:E898
int16_t input_delay;

// WZD dseg:E89A
int16_t MOUSE_Emu_Y;
// WZD dseg:E89C
int16_t MOUSE_Emu_X;

// WZD dseg:E89E
int16_t pointer_offset;

// WZD dseg:E8A0
int16_t temp_field_count;

// WZD dseg:E8A2
int16_t fields_count;

// WZD dseg:E8A4 
// drake178: GUI_Prev_Redraw_Fn
// AKA gfp_GUI_Prev_Redraw_Fn
// AKA gfp_CTRL_Prev_Redraw_Fn
// MoO2  Module: fields  function2
void (*function2)(void);

// WZD dseg:E8A8
// drake178: GUI_Redraw_Fn
// AKA GUI_Redraw_Function
// AKA gfp_GUI_Redraw_Function
// AKA gfp_SCRN_Redraw_Function
// AKA Redraw_Function
// 1oom  void (*uiobj_callback)(void *) = NULL;
// MoO2  Module: fields  function
void (*function)(void);

// WZD dseg:E8AC
/*
Init_Mouse_Keyboard()
    p_fields = (struct s_Field *)Allocate_Space(357);  // 357 PR, 5712 B  (¿ 150 * 38 = 5700 ¿ + 12 ?)
*/
struct s_Field * p_fields;  // "p_fields dd 0" ? Far Pointer?

/*
    WZD dseg:E8AC                           END: seg036 (Fields) - Uninitialized Data
*/



/*
    WIZARDS.EXE     seg036
    MAGIC.EXE       seg034
*/

// WZD s36p08
// TODO  void Set_Global_Esc(void);
void Enable_Cancel(void)
{
    mouse_cancel_disabled = ST_FALSE;
}
// WZD s36p09
// drake178: UU_GUI_ClearEscOverride
void Disable_Cancel(void)
{
    mouse_cancel_disabled = ST_TRUE;
}

// WZD s36p10
void Disable_Mouse_Wait(void)
{
    mouse_auto_exit = ST_TRUE;
}
// WZD s36p11
void Enable_Mouse_Wait(void)
{
    mouse_auto_exit = ST_FALSE;
}


// WZD s36p20
// drake178: GUI_GetMouseFocus()
// AKA Get_Mouse_Field()
// MoO2  Module: fields  Auto_Input()
int16_t Auto_Input(void)
{
    return auto_input_variable;
}



// WZD s36p30
// WZD s36p31
// WZD s36p32
// WZD s36p33

// WZD s36p34
// MoO2  Module: fields  Add_Input_Field()
/*
    function (0 bytes) Add_Input_Field
    Address: 01:00115E3B
        Num params: 4
        Return type: signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        pointer (4 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) xmin
            signed integer (2 bytes) ymin
            signed integer (2 bytes) width
            pointer (4 bytes) string
            signed integer (2 bytes) max_characters
            signed integer (2 bytes) fill_color
            signed integer (2 bytes) justification
            signed integer (2 bytes) cursor_type
            pointer (4 bytes) color_array
            pointer (4 bytes) select_char
            signed integer (2 bytes) button_sound
*/
/*

*/
int16_t Add_Input_Field(int16_t xmin, int16_t ymin, int16_t width, char * string, int16_t max_characters, int16_t fill_color, int16_t justification, int16_t cursor_type, uint8_t color_array[], char * select_char, int16_t help)
{
    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;
    p_fields[fields_count].font_style_num = Get_Current_Font_Style();
    p_fields[fields_count].font_normal_color = Get_Current_Normal_Color();
    p_fields[fields_count].font_highlight_color = Get_Current_Highlight_Color();
    p_fields[fields_count].x2 = (xmin + width);
    p_fields[fields_count].y2 = (ymin + Get_Font_Height());
    p_fields[fields_count].max_characters = max_characters;
    p_fields[fields_count].hotkey = select_char[0];
    UPPERCASE(p_fields[fields_count].hotkey);
    p_fields[fields_count].help = help;
    p_fields[fields_count].string = string;
    p_fields[fields_count].fill_color = fill_color;
    p_fields[fields_count].justification = justification;
    p_fields[fields_count].cursor_type = cursor_type;
    p_fields[fields_count].color_array = color_array;
    p_fields[fields_count].type = ft_Input;
    fields_count += 1;
    return (fields_count - 1);
}


// WZD s36p35
/*
; drake178: GUI_CreateEditSelect()
; creates a custom edit box with the passed attributes,
; returns the index of the created control:
;   Text@  - pointer to the edit string
;   MaxL   - maximum acceptable string length
;   BGCol  - background color for the control box
;   S_Img@ - segment pointer to the selection indicator
;   Style  - text outline style
; different than the basic edit box in that it does not
; have its own focus event handler, but is handled with
; the rest of the controls, albeit that results in
; reduced functionality (e.g. no positioning)
*/
/*

save_slot_fields[itr_save_slot_fields] = Add_Continuous_String_Input_Field(x_start, (47 + (itr_save_slot_fields * 15)), 260, magic_set.Save_Names[itr_save_slot_fields], (LEN_SAVE_DESCRIPTION - 1), ST_TRANSPARENT, selection_marker, ST_UNDEFINED);

*/
int16_t Add_Continuous_String_Input_Field(int16_t xmin, int16_t ymin, int16_t width, char * string, int16_t max_characters, int16_t fill_color, SAMB_ptr marker_picture, int16_t help, int16_t shadow)
{
    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;
    p_fields[fields_count].font_style_num = Get_Current_Font_Style();
    p_fields[fields_count].font_normal_color = Get_Current_Normal_Color();
    p_fields[fields_count].font_highlight_color = Get_Current_Highlight_Color();
    p_fields[fields_count].x2 = (xmin + width);
    p_fields[fields_count].y2 = (ymin + Get_Font_Height());
    p_fields[fields_count].max_characters = max_characters;
    p_fields[fields_count].hotkey = ST_NULL;
    p_fields[fields_count].help = help;
    p_fields[fields_count].string = string;
    p_fields[fields_count].marker_picture = marker_picture;
    p_fields[fields_count].fill_color = fill_color;
    p_fields[fields_count].shadow = shadow;
    p_fields[fields_count].type = ft_ContinuousStringInput;
    fields_count += 1;
    return (fields_count - 1);
}

// WZD s36p36
// UU_GUI_Ctrl_DecHeight();

// WZD s36p37
// GUI_CreateDialogLine();

// WZD s36p38
int16_t Add_Multi_Hot_Key_Field(char * string)
{
    char hotkey_string[LEN_STRING];
    int16_t itr_string_length = 0;
    strcpy(hotkey_string, string);
    while(hotkey_string[itr_string_length] != '\0' && itr_string_length < LEN_STRING) itr_string_length++;
    p_fields[fields_count].x1 = OFF_SCREEN;
    p_fields[fields_count].y1 = OFF_SCREEN;
    p_fields[fields_count].x2 = OFF_SCREEN;
    p_fields[fields_count].y2 = OFF_SCREEN;
    p_fields[fields_count].type = ft_MultiHotKey;
    p_fields[fields_count].help = ST_UNDEFINED;
    p_fields[fields_count].string = string;
    p_fields[fields_count].string_pos = 0;
    p_fields[fields_count].string_len = itr_string_length;
    p_fields[fields_count].hotkey = hotkey_string[0];  // TODO(JimBalcomb,20230612): fix hotkey - ? need char * ?
    UPPERCASE(p_fields[fields_count].hotkey);
    fields_count += 1;
    return (fields_count - 1);
}


// WZD s36p39
// drake178: GUI_CreateImageLabel()
int16_t Add_Picture_Field(int16_t xmin, int16_t ymin, SAMB_ptr pict_seg, int16_t hotkey, int16_t help)
{
    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;
    p_fields[fields_count].x2 = p_fields[fields_count].x1 + FLIC_Get_Width(pict_seg);
    p_fields[fields_count].y2 = p_fields[fields_count].y1 + FLIC_Get_Height(pict_seg);
    p_fields[fields_count].help = help;
    p_fields[fields_count].pict_seg = pict_seg;
    p_fields[fields_count].type = ft_Picture;
    UPPERCASE(p_fields[fields_count].type);
    fields_count += 1;
    return (fields_count - 1);
}


// WZD s36p40
// UU_GUI_CreateWndImgLbl()

// WZD s36p41
// MoO2  Module: fields  Set_Button_Down_Offsets()
void Set_Button_Down_Offsets(int16_t x, int16_t y)
{
    down_x = x;
    down_y = y;
}

// WZD s36p42
void Add_Button_Info(int16_t xmin, int16_t ymin, char * string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help)
{

    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;

    // p_fields[fields_count].Param0 = string;
    p_fields[fields_count].string = string;

    p_fields[fields_count].x2 = p_fields[fields_count].x1 + FLIC_Get_Width(pict_seg) - 1;
    p_fields[fields_count].y2 = p_fields[fields_count].y1 + FLIC_Get_Height(pict_seg) - 1;

    p_fields[fields_count].font_style_num = Get_Current_Font_Style();
    p_fields[fields_count].font_normal_color = Get_Current_Normal_Color();

    p_fields[fields_count].help = help;

    // p_fields[fields_count].Param5 = pict_seg;
    // p_fields[fields_count].Param5 = (uint16_t *)pict_seg;
    p_fields[fields_count].pict_seg = pict_seg;

    p_fields[fields_count].Param3 = Get_Full_Store_Flag(pict_seg);

    // TODO(JimBalcomb,20230612): make hotkey make sense - looks like (char *)
    // mov     si, [bp+select_char]
    // mov     al, [si]
    // mov     [byte ptr es:bx+s_Field.hotkey], al

    p_fields[fields_count].hotkey = hotkey;

    if ((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123) )
    {
        p_fields[fields_count].hotkey -= 32;
    }

}


// WZD s36p43
int16_t Add_Button_Field(int16_t xmin, int16_t ymin, char * string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help)
{
    Add_Button_Info(xmin, ymin, string, pict_seg, hotkey, help);
    p_fields[fields_count].type = ft_Button;
    fields_count += 1;
    return (fields_count - 1);
}


// WZD s36p47
int16_t Add_Hidden_Field(int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax, int16_t hotkey, int16_t help)
{
    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;
    p_fields[fields_count].x2 = xmax;
    p_fields[fields_count].y2 = ymax;
    p_fields[fields_count].type = ft_HotKey;
    p_fields[fields_count].help = help;
    p_fields[fields_count].hotkey = hotkey;
    UPPERCASE(p_fields[fields_count].hotkey);
    fields_count += 1;
    return (fields_count - 1);
}

// WZD s34p49
int16_t Add_Hot_Key(int16_t select_char)
{
    p_fields[fields_count].x1 = OFF_SCREEN;
    p_fields[fields_count].y1 = OFF_SCREEN;
    p_fields[fields_count].x2 = OFF_SCREEN;
    p_fields[fields_count].y2 = OFF_SCREEN;
    p_fields[fields_count].type = ft_HotKey;
    p_fields[fields_count].help = ST_UNDEFINED;
    p_fields[fields_count].hotkey = select_char;
    UPPERCASE(p_fields[fields_count].hotkey);
    fields_count += 1;
    return (fields_count - 1);
}

// WZD s34p51
// DEDUCE: What happens with the xpos,ypos here? How does it get used? How does that work?
// int16_t Add_Grid_Field(int16_t xmin, int16_t ymin, int16_t box_width, int16_t box_height, int16_t horizontal_count, int16_t vertical_count, int16_t *xpos, int16_t *ypos, int16_t help)
int16_t Add_Grid_Field(int16_t xmin, int16_t ymin, int16_t box_width, int16_t box_height, int16_t horizontal_count, int16_t vertical_count, int64_t *xpos, int64_t *ypos, int16_t help)
{
    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;
    p_fields[fields_count].x2 = (box_width * horizontal_count) + xmin - 1;
    p_fields[fields_count].y2 = (box_height * vertical_count) + ymin - 1;
    p_fields[fields_count].help = help;
    p_fields[fields_count].type = ft_Grid;
    p_fields[fields_count].Param1 = box_width;
    p_fields[fields_count].Param2 = box_height;
    p_fields[fields_count].hotkey = ST_NULL;
    // p_fields[fields_count].Param3 = *xpos;
    // p_fields[fields_count].Param4 = *ypos;
    // NOTE: We are storing the addresses of the pointers, to later be recast and dereferenced.
    // p_fields[fields_count].Param3 = (uint16_t)xpos;
    // p_fields[fields_count].Param4 = (uint16_t)ypos;
    p_fields[fields_count].Param3 = (int64_t)xpos;  // NOTE: assigning address, not value
    p_fields[fields_count].Param4 = (int64_t)ypos;  // NOTE: assigning address, not value
    UPPERCASE(p_fields[fields_count].hotkey);
    fields_count += 1;
    return (fields_count - 1);
}


// WZD s34p52
// WZD s34p53
// 1oom  uiobj.c  uiobj_add_slider_int() / uiobj_add_slider_func()
// 1oom: int16_t uiobj_add_slider_func(uint16_t x0, uint16_t y0, uint16_t vmin, uint16_t vmax, uint16_t w, uint16_t h, int16_t *vptr, void (*cb)(void *ctx, uint8_t slideri, int16_t value), void *ctx, uint8_t slideri)
/*
MoO2  Module: fields  Add_Scroll_Field()

    Add_Scroll_Field()
        Address: 01:00115478
            Return type: signed integer (2 bytes) 
        signed integer (2 bytes) xmin
        signed integer (2 bytes) ymin
        signed integer (2 bytes) min_value
        signed integer (2 bytes) max_value
        signed integer (2 bytes) min_valid
        signed integer (2 bytes) max_valid
        signed integer (2 bytes) width
        signed integer (2 bytes) height
        pointer (4 bytes) variable
        pointer (4 bytes) hotkey
        signed integer (2 bytes) button_sound
*/
/*
drake178: GUI_CreateNearSlider(); GUI_CreateSlideBar();
STARMAP.EXE: uiobj_add_slider_noseg(); uiobj_add_slider();
Add_Scroll_Field(); Add_Scroll_Field_Far()

; a wrapper for GUI_CreateSlideBar to create a slide bar control with a near state pointer returns the index of the created control
...
; creates a slider bar with the passed attributes, returns the index of the created control:
; the orientation (horizontal/vertical) is defined by the larger of Width/Height, slider bars need to be drawn using a graphic redraw function, the GUI only controls their input state

;   Min_S  - minimum slider position
;   Max_S  - maximum slider position
;   Min_V  - minimum represented value
;   Max_V  - maximum represented value
;   Pos@   - near pointer to the position state
*/
// int16_t Add_Scroll_Field_Far(int16_t xmin, int16_t ymin, int16_t min_value, int16_t max_value, int16_t min_valid, int16_t max_valid, int16_t width, int16_t height, int16_t * variable, char * hotkey, int16_t help)
// int16_t Add_Scroll_Field_Far(int16_t xmin, int16_t ymin, int16_t min_value, int16_t max_value, int16_t min_valid, int16_t max_valid, int16_t width, int16_t height, int16_t * ofst_variable, int16_t * sgmt_variable, char * hotkey, int16_t help)
/*
    uses all 6 parameters

Alchemy_Popup_Add_Fields()
    |-> Add_Scroll_Field((m_alchemy_popup_start_x + 50), (m_alchemy_popup_start_y + 25), 0, 55, 3, 53, 55, 7, m_alchemy_arrowbar_pos, cnst_HOTKEY_A_2, ST_UNDEFINED);

*/
int16_t Add_Scroll_Field(int16_t xmin, int16_t ymin, int16_t min_value, int16_t max_value, int16_t min_valid, int16_t max_valid, int16_t width, int16_t height, int16_t * variable, int16_t hotkey, int16_t help)
{
    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;
    p_fields[fields_count].x2 = (p_fields[fields_count].x1 + width);
    p_fields[fields_count].y2 = (p_fields[fields_count].y1 + height);
    p_fields[fields_count].min_valid = min_valid;
    p_fields[fields_count].max_valid = max_valid;
    p_fields[fields_count].min_value = min_value;
    p_fields[fields_count].max_value = max_value;
    p_fields[fields_count].help = help;
    p_fields[fields_count].orientation = (width > height) ? 0 : 1;  // Orientation: {Horizontal, Vertical}
    p_fields[fields_count].type = ft_Scroll;
    p_fields[fields_count].variable = variable;
    p_fields[fields_count].hotkey = hotkey;
    UPPERCASE(p_fields[fields_count].hotkey);
    // TODO  SET_HOTKEY(hotkey);
    fields_count += 1;
    return (fields_count - 1);
}


// WZD s34p54
// GUI_SliderUpdate()
// 1oom  uiobj_handle_t6_slider_input()
/*
    function (0 bytes) Find_Bar_Position
    Address: 01:001156E2
        Num params: 1
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) field_num
            signed integer (2 bytes) pos
            signed integer (2 bytes) width
            signed integer (2 bytes) bar_value
            signed integer (4 bytes) value
*/
int16_t Find_Bar_Position(int16_t field_num)
{
    int16_t pos;
    int16_t width;
    int16_t bar_value;
    int32_t value;

    if(p_fields[field_num].orientation == fo_Horizontal)  /* horizontal L->R */
    {
        width = (p_fields[field_num].x2 - p_fields[field_num].x1);
        pos = Pointer_X() + pointer_offset;
        value = (((pos - p_fields[field_num].x1) * (p_fields[field_num].max_value - p_fields[field_num].min_value)) / width);
        if(p_fields[field_num].x2 <= pos)
        {
            bar_value = p_fields[field_num].max_value;
        }
        else if(p_fields[field_num].x1 >= pos)
        {
            bar_value = p_fields[field_num].min_value;
        }
        else
        {
            bar_value = p_fields[field_num].min_value + value;
        }
    }
    else  /* vertical B->T */
    {
        width = (p_fields[field_num].y2 - p_fields[field_num].y1);
        pos = Pointer_Y() + pointer_offset;
        value = (((p_fields[field_num].y2 - pos) * (p_fields[field_num].max_value - p_fields[field_num].min_value)) / width);
        if(p_fields[field_num].y2 <= pos)
        {
            bar_value = p_fields[field_num].min_value;
        }
        else if(p_fields[field_num].y1 >= pos)
        {
            bar_value = p_fields[field_num].max_value;
        }
        else
        {
            bar_value = value;
        }
    }

    SETMIN(bar_value, p_fields[field_num].Param0);
    SETMAX(bar_value, p_fields[field_num].Param1);

    *p_fields[field_num].variable = bar_value;

    return bar_value;
}

// WZD s34p55
// GUI_SlideUp();
// uiobj_slider_plus()
/*
    function (0 bytes) Increment_Scroll_Bar
    Address: 01:00115994
        Num params: 1
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) field_num
            signed integer (2 bytes) percent
            signed integer (2 bytes) width
            signed integer (2 bytes) bar_value
            signed integer (4 bytes) value
*/
void Increment_Scroll_Bar(int16_t field_num)
{
    int32_t bar_value;
    // uint16_t ofst_variable;
    // uint16_t sgmt_variable;
    int16_t value;
    int16_t width;
    int16_t percent;
    
    // sgmt_variable = p_fields[field_num].Param2
    // ofst_variable = p_fields[field_num].Param6
    value = *p_fields[field_num].variable;

    width = (p_fields[field_num].max_value - p_fields[field_num].min_value);

    percent = ((((value - p_fields[field_num].min_value) * 100) / width) + 5);

    if(percent > 100)
    {
        value = p_fields[field_num].Param1;
    }
    else
    {
        bar_value = ((percent * width) / 100);
        value = (p_fields[field_num].min_value + bar_value);
    }

    if(p_fields[field_num].Param1 < value)
    {
        value = p_fields[field_num].Param1;
    }

    *p_fields[field_num].variable = value;

}

// WZD s34p56
// GUI_SlideDown();
// 1oom  uiobj_slider_minus()
/*
    function (0 bytes) Decrement_Scroll_Bar
    Address: 01:00115AC0
        Num params: 1
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) field_num
            signed integer (2 bytes) percent
            signed integer (2 bytes) width
            signed integer (2 bytes) bar_value
            signed integer (4 bytes) value
*/
void Decrement_Scroll_Bar(int16_t field_num)
{
    int32_t bar_value;
    // uint16_t ofst_variable;
    // uint16_t sgmt_variable;
    int16_t value;
    int16_t width;
    int16_t percent;
    
    // sgmt_variable = p_fields[field_num].Param2
    // ofst_variable = p_fields[field_num].Param6
    value = *p_fields[field_num].variable;

    width = (p_fields[field_num].Param4 - p_fields[field_num].Param3);

    percent = ((((value - p_fields[field_num].Param3) * 100) / width) - 5);

    if(percent < 0)
    {
        value = p_fields[field_num].Param0;
    }
    else
    {
        bar_value = ((percent * width) / 100);
        value = (p_fields[field_num].Param3 + bar_value);
    }

    if(p_fields[field_num].Param0 > value)
    {
        value = p_fields[field_num].Param0;
    }

    *p_fields[field_num].variable = value;

}


// WZD s36p57
/*
MoO1
    fields_count = 1;
    down_mouse_button = ST_UNDEFINED;
    uiobj_clicked_oi = 0;

MoO2
    fields_count = 1;
    input_delay = 0;
    down_mouse_button = ST_UNDEFINED;
    auto_input_variable = 0;
    input_field_active = ST_FALSE;
    active_input_field_number = ST_UNDEFINED;
    multi_hotkey_active_field = ST_FALSE;
    Mouse_Buffer();
    Mouse_Buffer2();
*/
/*

*/
void Clear_Fields(void)
{
    fields_count = 1;
    down_mouse_button = ST_UNDEFINED;
    auto_input_variable = 0;
    input_field_active = ST_FALSE;
    active_input_field_number = ST_UNDEFINED;
}

// WZD s36p58
// UU_GUI_NoSelectColor();

// WZD s36p59
// UU_GUI_ScrollableTxtDlg();

// WZD s36p60
// GUI_CreateTextDialog();

// WZD s36p61
// UU_GUI_NoMouseHighlight();

// WZD s36p62
/*
; moves the currently selected dialog option up or down
; if possible, along with the mouse pointer, and sets
; the control state variable accordingly
*/
int16_t GUI_MoveDlgHighlight__STUB(int16_t Y_Direction)
{
    int16_t itr_fields_count = 0;
    int16_t field_idx = 0;  // _SI_
    int16_t scanned_field = 0;  // _DI_

    scanned_field = Scan_Input();

    field_idx = scanned_field;





    return field_idx;
}


// WZD s36p63
// MoO2  Module: fields  Clear_Fields_Above()
void Clear_Fields_Above(int16_t field_num)
{
    fields_count = field_num + 1;
    down_mouse_button = ST_UNDEFINED;
}


// WZD s36p64
int16_t Get_Field_Counter(void)
{
    return fields_count;
}


// WZD s36p65  AKA GUI_Init()  AKA IN_Init()
// INPUT  void Init_Mouse_Keyboard(int16_t input_type);

// WZD s36p68
// UU_Get_Input_Delay();

// WZD s36p69
// UU_GUI_ClearSelectable();

// WZD s36p70
// UU_GUI_SetSelectable();


/*
Moo2
function (0 bytes) Draw_Visible_Fields
Address: 01:0011E718
Num params: 0
Return type: void (1 bytes) 
Locals:
signed integer (2 bytes) i
signed integer (2 bytes) j
signed integer (2 bytes) height
signed integer (2 bytes) space
signed integer (2 bytes) temp_value
signed integer (2 bytes) var_segment
signed integer (2 bytes) var_offset
signed integer (2 bytes) cursor_change
signed integer (2 bytes) old_window_x1
signed integer (2 bytes) old_window_x2
signed integer (2 bytes) old_window_y1
signed integer (2 bytes) old_window_y2
*/
// MoO2: Draw_Visible_Fields()

void Draw_Fields()
{
    int16_t old_windows_x1 = 0;
    int16_t old_windows_x2 = 0;
    int16_t old_windows_y1 = 0;
    int16_t old_windows_y2 = 0;
    int16_t Text_Change = 0;
    int16_t State_Pointer1 = 0;
    int16_t State_Pointer2 = 0;
    int16_t bar_position = 0;
    int16_t Half_V_Spacing = 0;
    int16_t Half_Font_Height = 0;
    int16_t itr_fields_count = 0;  // _SI_
    int16_t itr_help_list_count = 0;  // _SI_
    int16_t itr_continuous_string = 0;  // _DI_
    int16_t IDK_InputField_ShadowType = 0;  // _AX_ ... DNE in MoM Dasm, but in MoO2 Dasm

    old_windows_x1 = screen_window_x1;
    old_windows_x2 = screen_window_x2;
    old_windows_y1 = screen_window_y1;
    old_windows_y2 = screen_window_y2;

    Reset_Window();

    Set_Page_Off();

    for(itr_fields_count = 1; itr_fields_count < fields_count; itr_fields_count++)
    {
        if((down_mouse_button == itr_fields_count) && (p_fields[itr_fields_count].type != ft_Input) && (p_fields[itr_fields_count].type != ft_Grid))
        {
            Draw_Field(itr_fields_count, 1);
        }
        else
        {

            // DEDUCE: ¿ does this not include field types 05, 07, 08, 09, and 12 ?
            switch(p_fields[itr_fields_count].type)
            {
                case ft_Button:                 /*  0  0x00 */  // drake178: TODO
                {
                    Draw_Field(itr_fields_count, 0);

                } break;
                case ft_RadioButton:            /*  1  0x01 */  // drake178: ToggleButton
                {

                } break;
                case ft_LockedButton:           /*  2  0x02 */  // drake178: LockableButton
                {

                } break;
                case ft_MultiButton:            /*  3  0x03 */  // drake178: MStateButton
                {

                } break;
                case ft_Input:                  /*  4  0x04 */  // drake178: EditBox
                {
                    if(down_mouse_button != itr_fields_count)
                    {
                        Set_Font_Style(p_fields[itr_fields_count].font_style_num, p_fields[itr_fields_count].font_normal_color, p_fields[itr_fields_count].font_highlight_color, ST_NULL);

                        if (p_fields[itr_fields_count].fill_color != 0)
                        {
                            Fill(p_fields[itr_fields_count].x1, p_fields[itr_fields_count].y1, p_fields[itr_fields_count].x2, p_fields[itr_fields_count].y2, p_fields[itr_fields_count].fill_color);
                        }

                        if(p_fields[itr_fields_count].justification != 0)
                        {
                            // Print_Right(p_fields[itr_fields_count].x2, p_fields[itr_fields_count].y1, p_fields[itr_fields_count].string);
                        }
                        else
                        {
                            // Print(p_fields[itr_fields_count].x1, p_fields[itr_fields_count].y1, p_fields[itr_fields_count].string);
                        }
                    }
                } break;
                case ft_Picture:                /*  5  0x05 */  // drake178: ImageLabel      DNE/NIU in MoO2
                {

                } break;
                case ft_Scroll:                 /*  6  0x06 */  // drake178: SlideBar
                {
                    // ~== MoO2  Module: fields  Find_Bar_Position()
                    bar_position = *p_fields[itr_fields_count].variable;
                    SETRANGE(bar_position, p_fields[itr_fields_count].min_valid, p_fields[itr_fields_count].max_valid);
                    *p_fields[itr_fields_count].variable = bar_position;
                } break;
                case ft_HotKey:                 /*  7  0x07 */  // drake178: Label
                {

                } break;
                case ft_MultiHotKey:            /*  8  0x08 */  // drake178: Ctrl_AltString
                {

                } break;
                case ft_ClickLink:              /*  9  0x09 */  // drake178: ClickLink       DNE/NIU in MoO2
                {

                } break;
                case ft_StringList:             /* 10  0x0A */  // drake178: DialogLine
                {

                } break;
                case ft_ContinuousStringInput:  /* 11  0x0B */  // drake178: EditSelect
                {
                    if(active_input_field_number == itr_fields_count)
                    {
                        IDK_InputField_ShadowType = p_fields[itr_fields_count].shadow;
                        if(IDK_InputField_ShadowType == -1)
                        {
                            Set_Font_Style_Shadow_Down(p_fields[itr_fields_count].font_style_num, p_fields[itr_fields_count].font_normal_color, p_fields[itr_fields_count].font_highlight_color, ST_NULL);
                        }
                        else if(IDK_InputField_ShadowType == 1)
                        {
                            Set_Font_Style_Shadow_Up(p_fields[itr_fields_count].font_style_num, p_fields[itr_fields_count].font_normal_color, p_fields[itr_fields_count].font_highlight_color, ST_NULL);
                        }
                        else
                        {
                            Set_Font_Style(p_fields[itr_fields_count].font_style_num, p_fields[itr_fields_count].font_normal_color, p_fields[itr_fields_count].font_highlight_color, ST_NULL);
                        }

                        Set_Highlight_Colors_On();

                        if(p_fields[itr_fields_count].fill_color != 0)
                        {
                            Fill(p_fields[itr_fields_count].x1, p_fields[itr_fields_count].y1, p_fields[itr_fields_count].x2, p_fields[itr_fields_count].y2, p_fields[itr_fields_count].fill_color);
                        }
                        
                        Text_Change = ST_FALSE;
                        if((GUI_EditAnimStage / 2) != GUI_EditCursorOn)
                        {
                            Text_Change = ST_TRUE;
                        }
                        GUI_EditCursorOn = (GUI_EditAnimStage / 2);
                        GUI_EditAnimStage++;
                        if(GUI_EditAnimStage > 3)
                        {
                            GUI_EditAnimStage = 0;
                        }

                        if(Text_Change != ST_FALSE)
                        {
                            if(GUI_EditCursorOn != 0)
                            {
                                itr_continuous_string = 0;
                                while(
                                    (continuous_string[itr_continuous_string] != '\0')
                                    &&
                                    (itr_continuous_string < p_fields[itr_fields_count].max_characters)
                                )
                                {
                                    itr_continuous_string++;
                                }
                                if((p_fields[itr_fields_count].max_characters - 1) > itr_continuous_string)
                                {
                                    strcat(continuous_string, GUI_EditCursor);  // "_"
                                }
                            }
                            else
                            {
                                itr_continuous_string = 0;
                                while(
                                    (continuous_string[itr_continuous_string] != '_')
                                    &&
                                    (itr_continuous_string < p_fields[itr_fields_count].max_characters)
                                )
                                {
                                    itr_continuous_string++;
                                }
                                continuous_string[itr_continuous_string] = '\0';
                            }
                        }

                        if(p_fields[itr_fields_count].marker_picture != ST_NULL)
                        {
                            FLIC_Draw(p_fields[itr_fields_count].x1, p_fields[itr_fields_count].y1, p_fields[itr_fields_count].marker_picture);
                        }

                        Print((p_fields[itr_fields_count].x1 + 10), p_fields[itr_fields_count].y1, continuous_string);

                        Set_Normal_Colors_On();

                    }
                    else
                    {
                        IDK_InputField_ShadowType = p_fields[itr_fields_count].shadow;
                        if(IDK_InputField_ShadowType == -1)
                        {
                            Set_Font_Style_Shadow_Down(p_fields[itr_fields_count].font_style_num, p_fields[itr_fields_count].font_normal_color, p_fields[itr_fields_count].font_highlight_color, ST_NULL);
                        }
                        else if(IDK_InputField_ShadowType == 1)
                        {
                            Set_Font_Style_Shadow_Up(p_fields[itr_fields_count].font_style_num, p_fields[itr_fields_count].font_normal_color, p_fields[itr_fields_count].font_highlight_color, ST_NULL);
                        }
                        else
                        {
                            Set_Font_Style(p_fields[itr_fields_count].font_style_num, p_fields[itr_fields_count].font_normal_color, p_fields[itr_fields_count].font_highlight_color, ST_NULL);
                        }

                        if(p_fields[itr_fields_count].fill_color != 0)
                        {
                            Fill(p_fields[itr_fields_count].x1, p_fields[itr_fields_count].y1, p_fields[itr_fields_count].x2, p_fields[itr_fields_count].y2, p_fields[itr_fields_count].fill_color);
                        }

                        Print((p_fields[itr_fields_count].x1 + 10), p_fields[itr_fields_count].y1, p_fields[itr_fields_count].string);

                        
                    }
                } break;
                case ft_Grid:                   /* 12  0x0C */  // drake178: TODO
                {
                    // N/A; Manually Excluded
                } break;

            }

        }

        if( (field_box_mode != ST_FALSE) && (p_fields[itr_fields_count].type != ft_MultiHotKey) )
        {
            /*
                Draw Rectangle
            */

            // color = 0x0E  14

            // p_fields[itr_fields_count].x1
            // p_fields[itr_fields_count].y1
            // p_fields[itr_fields_count].x2
            // p_fields[itr_fields_count].y2

            // TODO  VGA_WndDrawLine()
            // TODO  VGA_WndDrawLine()
            // TODO  VGA_WndDrawLine()
            // TODO  VGA_WndDrawLine()
        }

    }


// TODO      if( (help_box_mode != ST_FALSE) && (help_list_count != 0) )
// TODO      {
// TODO          for(itr_help_list_count = 1; itr_help_list_count < help_list_count; itr_help_list_count++)
// TODO          {
// TODO              if(help_struct_pointer[itr_help_list_count]->help_idx != ST_UNDEFINED)
// TODO              {
// TODO              /*
// TODO                  Draw Rectangle
// TODO              */
// TODO              // color = 0xB4 180
// TODO              // p_fields[itr_fields_count].x1
// TODO              // p_fields[itr_fields_count].y1
// TODO              // p_fields[itr_fields_count].x2
// TODO              // p_fields[itr_fields_count].y2
// TODO              // VGA_WndDrawLine()
// TODO              // VGA_WndDrawLine()
// TODO              // VGA_WndDrawLine()
// TODO              // VGA_WndDrawLine()
// TODO              }
// TODO          }
// TODO      }


    Set_Window(old_windows_x1, old_windows_y1, old_windows_x2, old_windows_y2);

}

// WZD s36p71
/*
MoO2
function (0 bytes) Draw_Field
Address: 01:0011B05A
Num params: 2
Return type: void (1 bytes) 
signed integer (2 bytes) 
signed integer (2 bytes) 
Locals:
signed integer (2 bytes) field_num
signed integer (2 bytes) up_down_flag
signed integer (2 bytes) i
signed integer (2 bytes) height
signed integer (2 bytes) space
signed integer (2 bytes) var_segment
signed integer (2 bytes) var_offset
signed integer (2 bytes) temp_value
*/

// ¿ Is the code here for `case ft_Grid` the same as in Scan_Input() ?

// WZD s36p72
/*

up_down_flag    {0: Up, 1: Down}  ~== Down: {F,T}

*/
void Draw_Field(int16_t field_num, int16_t up_down_flag)
{
    int16_t Half_Font_Height;
    int16_t screen_x;
    int16_t screen_y;
    int16_t field_x;
    int16_t field_y;
    int16_t grid_x;
    int16_t grid_y;

    switch(p_fields[field_num].type)
    {
        case ft_Button:                 /*  0  0x00 */  // drake178: TODO
        {
            if(up_down_flag == 0)  /* up */
            {
                Reset_Animation_Frame((SAMB_ptr)p_fields[field_num].pict_seg);
                FLIC_Draw(p_fields[field_num].x1, p_fields[field_num].y1, (SAMB_ptr)p_fields[field_num].pict_seg);
                Set_Font_Style(p_fields[field_num].font_style_num, p_fields[field_num].font_normal_color, ST_NULL, ST_NULL);
                Half_Font_Height = ((Get_Font_Height() - 1) / 2);
                Print_Centered(
                    (((p_fields[field_num].x2 - p_fields[field_num].x1) / 2) + p_fields[field_num].x1),
                    (((p_fields[field_num].y2 - p_fields[field_num].y1) / 2) + p_fields[field_num].y1 - Half_Font_Height),
                    (char *)p_fields[field_num].string
                );
            }
            else
            {
                if(p_fields[field_num].Param3 == 0)  /* FullFrames */
                {
                    Reset_Animation_Frame((SAMB_ptr)p_fields[field_num].pict_seg);
                    FLIC_Draw(p_fields[field_num].x1, p_fields[field_num].y1, (SAMB_ptr)p_fields[field_num].pict_seg);
                }
                else
                {
                    Set_Animation_Frame((SAMB_ptr)p_fields[field_num].pict_seg, 1);
                }
                FLIC_Draw(p_fields[field_num].x1, p_fields[field_num].y1, (SAMB_ptr)p_fields[field_num].pict_seg);
                Set_Font_Style(p_fields[field_num].font_style_num, p_fields[field_num].font_normal_color, ST_NULL, ST_NULL);
                Half_Font_Height = ((Get_Font_Height() - 1) / 2);
                Print_Centered(
                    ((((p_fields[field_num].x2 - p_fields[field_num].x1) / 2) + p_fields[field_num].x1) + down_x),
                    ((((p_fields[field_num].y2 - p_fields[field_num].y1) / 2) + p_fields[field_num].y1 - Half_Font_Height) + down_y),
                    (char *)p_fields[field_num].string
                );
            }
        } break;
        case ft_RadioButton:            /*  1  0x01 */  // drake178: ToggleButton
        {

        } break;
        case ft_LockedButton:           /*  2  0x02 */  // drake178: LockableButton
        {

        } break;
        case ft_MultiButton:            /*  3  0x03 */  // drake178: MStateButton
        {

        } break;
        case ft_Input:                  /*  4  0x04 */  // drake178: EditBox
        {
            // TODO  Input_Box_Popup(field_num);
        } break;
        case ft_Picture:                /*  5  0x05 */  // drake178: ImageLabel      DNE/NIU in MoO2
        {
            // default
        } break;
        case ft_Scroll:                 /*  6  0x06 */  // drake178: SlideBar
        {
            if(up_down_flag == 1)
            {
                Find_Bar_Position(field_num);
            }
        } break;
        case ft_HotKey:                 /*  7  0x07 */  // drake178: Label
        {
            // default
        } break;
        case ft_MultiHotKey:            /*  8  0x08 */  // drake178: Ctrl_AltString
        {
            // default
        } break;
        case ft_ClickLink:              /*  9  0x09 */  // drake178: ClickLink       DNE/NIU in MoO2
        {

        } break;
        case ft_StringList:             /* 10  0x0A */  // drake178: DialogLine
        {

        } break;
        case ft_ContinuousStringInput:  /* 11  0x0B */  // drake178: EditSelect
        {
            // default
        } break;
        case ft_Grid:                   /* 12  0x0C */  // drake178: TODO
        {
            if(up_down_flag == 1)  /* ¿ field up/down state: down ? */
            {
                screen_x = Pointer_X();
                screen_y = Pointer_Y();
                // ~ translate screen coordinates to field coordinates
                field_x = screen_x - p_fields[field_num].x1;  // always  0
                field_y = screen_y - p_fields[field_num].y1;  // always 20
                // ~ translate field coordinates to grid coordinates  (from pixels to squares)
                grid_x = field_x / p_fields[field_num].Param1;  // always 12  box_width
                grid_y = field_y / p_fields[field_num].Param2;  // always 10  box_height
                // ¿ same code as in Add_Grid_Field() ?
                // NO!! Here, assigning value, there assigning memory address
                // ~ access Param3/4 as an unsigned 2-byte value, treat that value as a pointer to a signed 2-byte value, dereference that pointer to assign a signed 2-byte value
                // *( (int16_t *) ( (uint16_t) p_fields[field_num].Param3 ) ) = grid_x;
                // *( (int16_t *) ( (uint16_t) p_fields[field_num].Param4 ) ) = grid_y;
                *((int64_t *)p_fields[field_num].Param3) = grid_x;
                *((int64_t *)p_fields[field_num].Param4) = grid_y;
            }
        } break;

    }

}

// WZD s36p73
void Push_Field_Down(int16_t field_num, int16_t l_mx, int16_t l_my)
{

    if( (l_mx < 0) || (l_mx >= SCREEN_WIDTH) || (l_my < 0) || (l_my >= SCREEN_HEIGHT) )
    {
        return;
    }

    if(mouse_installed)
    {
        Save_Mouse_State();
        if(field_num != down_mouse_button)
        {
            if(down_mouse_button != ST_UNDEFINED)
            {
                /*
                    Redraw Previous & Draw Current
                */
                if(down_mouse_button != field_num)
                {
                    Restore_Mouse_On_Page();
                    if((p_fields[down_mouse_button].type != ft_MultiButton) || (p_fields[field_num].type == ft_MultiButton))
                    {
                        if(
                            (p_fields[down_mouse_button].type != ft_StringList)
                            ||
                            ( (p_fields[down_mouse_button].type == ft_StringList) && (p_fields[field_num].Selectable != ST_FALSE) )
                        )
                        {
                            Draw_Field(down_mouse_button, 0);
                        }
                    }
                    // @@DrawControl:
                    down_mouse_button = field_num;
                    Draw_Field(field_num, 1);
                    if(p_fields[field_num].type == ft_Input)
                    {
                        l_mx = Pointer_X();
                        l_my = Pointer_Y();
                    }
                    Save_Mouse_On_Page(l_mx, l_my);
                    Draw_Mouse_On_Page(l_mx, l_my);
                    Set_Pointer_Position(l_mx, l_my);
                }
            }
            else  /* if(down_mouse_button != ST_UNDEFINED) */
            {
                Restore_Mouse_On_Page();
                down_mouse_button = field_num;
                Draw_Field(field_num, 1);
                if(p_fields[field_num].type == ft_Input)
                {
                    l_mx = Pointer_X();
                    l_my = Pointer_Y();
                }
                Save_Mouse_On_Page(l_mx, l_my);
                Draw_Mouse_On_Page(l_mx, l_my);
                Set_Pointer_Position(l_mx, l_my);
            }
        }
        Restore_Mouse_State();
    }
    else  /* if(mouse_installed) */
    {
        Restore_Mouse_On_Page();
        Set_Page_On();
        down_mouse_button = field_num;
        Draw_Field(field_num, 1);
        down_mouse_button = ST_UNDEFINED;
        Set_Page_Off();
        Save_Mouse_On_Page(l_mx, l_my);
        Draw_Mouse_On_Page(l_mx, l_my);
        Set_Pointer_Position(l_mx, l_my);
    }

}



// WZD s36p74
// UU_GUI_SetDlgTitleHelp()

// WZD s36p75
// UU_GUI_ClearTitleHelp()



// WZD s36p76
// drake178: 
// AKA Set_Redraw_Function()
// AKA SCRN_Set_Redraw_Function()
// AKA GUI_Set_Redraw_Function()
// MoO2  Module: fields  Assign_Auto_Function()
void Assign_Auto_Function(void (*auto_function)(void), uint16_t delay)
{

    function = auto_function;  // in MoG, this works, as `void (*fxnptr)(void);` ... `fxnptr = &fxn;`

    auto_active_flag = ST_TRUE;

    if ( (delay <= 0) || (delay >= 10) )
    {
        auto_function_delay = 2;
    }
    else
    {
        auto_function_delay = delay;
    }

}


// WZD s36p77
// drake178: GUI_SaveRedrawFn()
// AKA SCRN_SaveRedrawFn()
// MoO2  Module: fields  Save_Auto_Function()
void Save_Auto_Function(void)
{
    function2 = function;
}

// WZD s36p78
// drake178: GUI_LoadRedrawFn
// AKA SCRN_LoadRedrawFn()
// MoO2  Module: fields  Restore_Auto_Function()
void Restore_Auto_Function(void)
{
    function = function2;
    auto_active_flag = ST_TRUE;
}


// WZD s36p79
// drake178: GUI_DisableRedraw()
// AKA Disable_Redraw_Function()
// AKA SCRN_DisableRedraw()
// MoO2  Module: fields  Deactivate_Auto_Function()
void Deactivate_Auto_Function(void)
{
    auto_active_flag = ST_FALSE;
}

// WZD s36p80
// drake178: UU_GUI_EnableRedraw(void)
// MoO2  Module: fields  Activate_Auto_Function()
void UU_Activate_Auto_Function(void)
{
    auto_active_flag = ST_TRUE;
}


// WZD s36p81
// drake178: GUI_CallRedrawFn
// AKA Call_Redraw_Function()
// AKA SCRN_CallRedrawFn()
// MoO2  Module: fields  Invoke_auto_function()
void Invoke_Auto_Function(void)
{
    if(auto_active_flag == ST_TRUE)
    {
        function();
    }
}


// WZD s36p82
// MoO2  Module: fields  Quick_Call_Auto_Function()
void Quick_Call_Auto_Function(void)
{
    if(auto_active_flag == ST_TRUE)
    {
        Mark_Time();
        Set_Page_Off();
        Invoke_Auto_Function();
        Apply_Palette();
        Toggle_Pages();
        Release_Time(1);
    }
    else
    {
        Apply_Palette();
        Toggle_Pages();
    }
}


// WZD s36p83
// drake178: GUI_NormalRedraw()
// AKA GUI_Redraw_WaitTimer
// AKA SCRN_Redraw_WaitTimer
// MoO2  Module: fields  Call_Auto_Function()
void Call_Auto_Function(void)
{

    if(auto_active_flag == ST_TRUE)
    {
        Mark_Time();
        Set_Page_Off();
        Invoke_Auto_Function();
        Apply_Palette();
        Toggle_Pages();
        Release_Time(auto_function_delay);
    }
    else
    {
        Apply_Palette();
        Toggle_Pages();
    }

}



// WZD s36p84
// drake178: UU_GUI_ExtClear()
void UU_GUI_ExtClear(void)
{
    Clear_Fields();
    Set_Input_Delay(1);
}



// WZD s36p85
// INPUT  void Toggle_Pages(void);
