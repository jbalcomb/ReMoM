
#include <string.h>

#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoX_Data.H"

#include "Fields.H"
#include "FLIC_Draw.H"
#include "Fonts.H"
#include "Graphics.H"
#include "Mouse.H"
#include "Video.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"    /* DLOG() */
#endif


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
dseg:E89E 00 00                                           cursor_offset dw 0                      ; DATA XREF: Interpret_Mouse_Input+33w ...
dseg:E8A0 00 00                                           GUI_PrevControlCount dw 0               ; DATA XREF: G_GUI_PressAnyKey+13w ...
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
dseg:78BE 00 00                                           mouse_driver_installed dw 0             ; DATA XREF: MD_GetButtonStatus+7r ...
dseg:78C0 00 00                                           MOUSE_Usable dw 0                       ; DATA XREF: RP_MOUSE_SetUsable+6w ...
dseg:78C2 9E 00                                           mouse_x dw 158                          ; DATA XREF: Init_Mouse_Driver+40w ...
dseg:78C4 64 00                                           mouse_y dw 100                          ; DATA XREF: Init_Mouse_Driver+44w ...
dseg:78C6 01 00                                           current_mouse_list_count dw 1           ; DATA XREF: Set_Mouse_List+Cw ...
dseg:78C8 00 00                                           current_pointer_offset dw 0             ; DATA XREF: Set_Mouse_List+39w ...
dseg:78CA 00 00                                           MOUSE_ClickRec1 dw 0                    ; DATA XREF: Init_Mouse_Driver+7Ew ...
dseg:78CC 00 00                                           MOUSE_ClickX dw 0                       ; DATA XREF: MOUSE_GetClickX+6r ...
dseg:78CE 00 00                                           MOUSE_ClickY dw 0                       ; DATA XREF: MOUSE_GetClickY+6r ...
dseg:78D0 00 00                                           MOUSE_ClickBtns dw 0                    ; DATA XREF: MD_GetClickedBtns+6r ...
dseg:78D2 00 00                                           MOUSE_ClickRec2 dw 0                    ; DATA XREF: Init_Mouse_Driver+84w ...
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
dseg:825A 02 00                                           GUI_Redraw_Timer dw 2                   ; DATA XREF: GUI_EditBoxRedraw+3D7r ...
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
dseg:8282 00 00                                           GUI_Edit_Position dw 0                  ; DATA XREF: GUI_EditBoxControl:loc_29A23w ...
dseg:8284 00 00                                           GUI_EditAnimStage dw 0                  ; DATA XREF: Interpret_Mouse_Input+35Aw ...
dseg:8286 00 00                                           GUI_EditCursorOn dw 0                   ; DATA XREF: Interpret_Mouse_Input+360w ...
dseg:8288 00 00                                           input_field_active dw 0                 ; DATA XREF: Interpret_Mouse_Input:loc_26941r ...
dseg:828A 00 00                                           DBG_ControlHighlight dw 0               ; DATA XREF: Interpret_Mouse_Input+DCr ...
dseg:828C 00 00                                           DBG_HelpHighlight dw 0                  ; DATA XREF: Interpret_Mouse_Input+100r ...
dseg:828E FF FF                                           active_input_field_number dw 0FFFFh     ; DATA XREF: Interpret_Mouse_Input+2DCr ...
dseg:8290 00 00                                           GUI_EditTimeOutType dw 0                ; DATA XREF: UU_GUI_SetTimeOutP+14w ...
dseg:8292 00 00                                           UU_GUI_UUClickBufVar dw 0               ; DATA XREF: UU_GUI_SetClickBuffer+27w
dseg:8294 00 00                                           UU_GUI_ClickBuffer dw 0                 ; DATA XREF: UU_GUI_SetTimeOutP+3r ...
dseg:8296                                                 ¿ BEGIN: Help - Initialized Data ?
dseg:8296 00 00                                           help_list_active dw 0                   ; DATA XREF: Interpret_Mouse_Input+752r ...
dseg:8298 00 00                                           help_list_count dw 0                    ; DATA XREF: Set_Help_List+12w ...
dseg:8298                                                 ¿ END: Help - Initialized Data ?
dseg:829A                                                 ? BEGIN: Mouse Buffer ?
dseg:829A 00 00                                           KD_ActiveStringTrig dw 0                ; DATA XREF: Interpret_Keyboard_Input:loc_27F9Br ...
dseg:829C 72 62                                           UU_cnst_RB7 db 'rb'                     ; DATA XREF: UU_GUI_LoadClickFile+Eo
dseg:829E 00                                              cnst_ZeroString_12 db 0                 ; DATA XREF: GUI_TextEditDialog+3Bo ...
dseg:829F 77 62 00                                        UU_cnst_WB8 db 'wb',0                   ; DATA XREF: UU_GUI_SaveClickFile+Bo
dseg:829F                                                 ? END: Mouse Buffer ?
dseg:82A2 05 00                                           UU_GUI_Up_Hotkey db 5, 0                ; DATA XREF: UU_GUI_ScrollableTxtDlg+21Do ...
dseg:82A4 06 00                                           UU_GUI_Down_Hotkey db 6, 0              ; DATA XREF: UU_GUI_ScrollableTxtDlg+240o ...
dseg:82A6 5F 00                                           GUI_EditCursor db '_',0                 ; DATA XREF: Draw_Fields+117Do
dseg:82A6
dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard
*/



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
// WZD dseg:824A GUI_RedrawFn_Present dw 0               
// WZD dseg:824C KD_prev_field_idx dw 0                  

// WZD dseg:824E
// DONT  mouse_installed = ST_FALSE;                     

// WZD dseg:8250
int16_t down_mouse_button = ST_UNDEFINED;

// WZD dseg:8252
int16_t down_x = 1 ;

// WZD dseg:8254
int16_t down_y = -1;

// WZD dseg:8256 GUI_EmptyTitleHelp dw 0FFFFh            
// WZD dseg:8258 GUI_DialogDirections dw 0               
// WZD dseg:825A GUI_Redraw_Timer dw 2                   

// WZD dseg:825C
// AKA mouse_list_none_init s_MOUSE_LIST <0, 0, 0, 319, 199, 0>
// ¿ int16_t mouse_list_init_count = 1; ?
// WZD dseg:825C
// DEDUCE: only XREF in Init_Mouse_Keyboard() ?
struct s_mouse_list mouse_list_init[1] = {
    {0, 0, 0, 0, 319, 199}
};

// WZD dseg:8268
// AKA GUI_MouseFocusCtrl
int16_t GUI_MouseFocusCtrl = 0;
// ¿ int16_t focused_field = ST_UNDEFINED; ?
// ¿ int16_t mouse_field = ST_FALSE; ?

// WZD dseg:826A GUI_MouseHighlight dw 1                 
// WZD dseg:826C GUI_NoDialogWrap dw 0                   
// WZD dseg:826E GUI_NoTxtSelectColor dw 0               
// WZD dseg:8270 GUI_OnLastDialogLine dw 0               

// WZD dseg:8272
int16_t _global_esc = ST_FALSE;

// WZD dseg:8274
int16_t GUI_ClickActivate;

// WZD dseg:8276 aNotInUse db 'NOT IN USE',0
// WZD dseg:8281 db    0
// WZD dseg:8282 GUI_Edit_Position dw 0                  
// WZD dseg:8284 GUI_EditAnimStage dw 0                  
// WZD dseg:8286 GUI_EditCursorOn dw 0                   

// WZD dseg:8288
int16_t input_field_active = ST_FALSE;

// WZD dseg:828A
int16_t DBG_ControlHighlight = ST_FALSE;
// WZD dseg:828C
int16_t DBG_HelpHighlight = ST_FALSE;

// WZD dseg:828E
// AKA GUI_Active_EditSlct
int16_t active_input_field_number = ST_UNDEFINED;

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
// dseg:E87A GUI_EditString db 30 dup(0)

// WZD dseg:E898
int16_t input_delay;

// dseg:E89A MOUSE_Emu_Y dw 0           
// dseg:E89C MOUSE_Emu_X dw 0           

// WZD dseg:E89E
int16_t cursor_offset;

// dseg:E8A0 GUI_PrevControlCount dw 0  

// WZD dseg:E8A2
int16_t fields_count;

// dseg:E8A4 GUI_Prev_Redraw_Fn dd 0    
// dseg:E8A8 GUI_Redraw_Function dd 0   

// WZD dseg:E8AC
struct s_Field * p_fields;  // "p_fields dd 0" ? Far Pointer?

/*
    WZD dseg:E8AC                           END: Fields - Uninitialized Data
*/



/*
    WZD  seg036
    MGC  seg034
*/

// WZD s36p38
int16_t Add_Multi_Hot_Key_Field(char * argString)
{
    char varString[30];
    int16_t itr_string_length = 0;

    strcpy(varString, argString);

    while(varString[itr_string_length] != '\0' && itr_string_length < 30) itr_string_length++;

    p_fields[fields_count].x1 = 500;
    p_fields[fields_count].y1 = 500;
    p_fields[fields_count].x2 = 500;
    p_fields[fields_count].y2 = 500;
    p_fields[fields_count].type = ft_MultiHotKey;
    p_fields[fields_count].help = ST_UNDEFINED;
    p_fields[fields_count].Param0 = argString[0];  // TODO(JimBalcomb,20230612): fix Param0 - ? need char * ?
    p_fields[fields_count].Param1 = 0;
    p_fields[fields_count].Param2 = itr_string_length;
    p_fields[fields_count].hotkey = varString[0];  // TODO(JimBalcomb,20230612): fix hotkey - ? need char * ?

    if ((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123) )
    {
        p_fields[fields_count].hotkey -= 32;
    }

    fields_count +=1;

    return (fields_count - 1);
}


// WZD s36p42
void Add_Button_Info(int16_t xmin, int16_t ymin, int16_t string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help)
{

    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;

    p_fields[fields_count].Param0 = string;

    p_fields[fields_count].x2 = FLIC_Get_Width(pict_seg) + p_fields[fields_count].x1 - 1;
    p_fields[fields_count].y2 = FLIC_Get_Height(pict_seg) + p_fields[fields_count].y1 - 1;

    p_fields[fields_count].Font_Index = Get_Current_Font_Index();
    p_fields[fields_count].ColorSet1 = Get_Current_Font_Color();

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
int16_t Add_Button_Field(int16_t xmin, int16_t ymin, int16_t string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help)
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

    if ((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123) )
    {
        p_fields[fields_count].hotkey -= 32;
    }

    fields_count += 1;

    return (fields_count - 1);
}

// WZD s34p49
int16_t Add_Hot_Key(int16_t select_char)
{

    p_fields[fields_count].x1 = 500;
    p_fields[fields_count].y1 = 500;
    p_fields[fields_count].x2 = 500;
    p_fields[fields_count].y2 = 500;
    p_fields[fields_count].type = ft_HotKey;
    p_fields[fields_count].help = ST_UNDEFINED;
    p_fields[fields_count].hotkey = select_char;

    if((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123))
    {
        p_fields[fields_count].hotkey -= 32;
    }

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
    p_fields[fields_count].Param3 = (int64_t)xpos;
    p_fields[fields_count].Param4 = (int64_t)ypos;

    if((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123))
    {
        p_fields[fields_count].hotkey -= 32;
    }

    fields_count += 1;

    return (fields_count - 1);
}

// WZD s36p57
void Clear_Fields(void)
{

    fields_count = 1;
    down_mouse_button = ST_UNDEFINED;
    // focused_field = ST_UNDEFINED;
    // mouse_field = ST_FALSE;
    input_field_active = ST_FALSE;
    active_input_field_number = ST_UNDEFINED;

}

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
    int16_t old_windows_x1;
    int16_t old_windows_x2;
    int16_t old_windows_y1;
    int16_t old_windows_y2;
    int16_t itr_fields_count;
    int16_t itr_help_list_count;

    old_windows_x1 = screen_window_x1;
    old_windows_x2 = screen_window_x2;
    old_windows_y1 = screen_window_y1;
    old_windows_y2 = screen_window_y2;

    Reset_Window();

    Set_Page_Off();


    for(itr_fields_count = 1; itr_fields_count < fields_count; itr_fields_count++)
    {
        if( (down_mouse_button == itr_fields_count) && (p_fields[itr_fields_count].type != ft_Input) && (p_fields[itr_fields_count].type != ft_Grid) )
        {
            Draw_Field(itr_fields_count, 1);
        }
        else
        {

            // DEDUCE: ¿ does this not include field types 05, 07, 08, 09, and 12 ?
            switch(p_fields[itr_fields_count].type)
            {
                case ft_Button:                 /*  0  0x00 */  //drake178: TODO
                {

                } break;
                case ft_RadioButton:            /*  1  0x01 */  //drake178: ToggleButton
                {

                } break;
                case ft_LockedButton:           /*  2  0x02 */  //drake178: LockableButton
                {

                } break;
                case ft_MultiButton:            /*  3  0x03 */  //drake178: MStateButton
                {

                } break;
                case ft_Input:                  /*  4  0x04 */  //drake178: EditBox
                {

                } break;
                case ft_ImageLabel:             /*  5  0x05 */  //drake178: ImageLabel      DNE/NIU in MoO2
                {

                } break;
                case ft_Scroll:                 /*  6  0x06 */  //drake178: SlideBar
                {

                } break;
                case ft_HotKey:                 /*  7  0x07 */  //drake178: Label
                {

                } break;
                case ft_MultiHotKey:            /*  8  0x08 */  //drake178: Ctrl_AltString
                {

                } break;
                case ft_ClickLink:              /*  9  0x09 */  //drake178: ClickLink       DNE/NIU in MoO2
                {

                } break;
                case ft_StringList:             /* 10  0x0A */  //drake178: DialogLine
                {

                } break;
                case ft_ContinuousStringInput:  /* 11  0x0B */  //drake178: EditSelect
                {

                } break;
                case ft_Grid:                   /* 12  0x0C */  //drake178: TODO
                {
                    DLOG("switch(p_fields[itr_fields_count].type)  case ft_Grid");
                    
                } break;

            }

        }

        if( (DBG_ControlHighlight != ST_FALSE) && (p_fields[itr_fields_count].type != ft_MultiHotKey) )
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


// TODO      if( (DBG_HelpHighlight != ST_FALSE) && (help_list_count != 0) )
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
void Draw_Field(int16_t field_num, int16_t up_down_flag)
{
    int16_t screen_x;  // ~ Pointer_X()
    int16_t screen_y;  // ~ Pointer_Y()
    // ~ translate screen coordinates to field coordinates
    int16_t field_x;  // field_x = screen_x - p_fields[].x1
    int16_t field_y;  // field_y = screen_y - p_fields[].y1
    // ~ translate field coordinates to grid coordinates  (from pixels to squares)
    int16_t grid_x;  // grid_x = field_x % box_width
    int16_t grid_y;  // grid_y = field_y % box_height

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Field()\n", __FILE__, __LINE__);
#endif

    switch(p_fields[field_num].type)
    {
        case ft_Button:                 /*  0  0x00 */  //drake178: TODO
        {

// Old-Code: .\MoM_Rasm\_s34p71c.c
// #ifdef STU_DEBUG
//                 dbg_prn("DEBUG: [%s, %d] case Ctrl_ClickButton\n", __FILE__, __LINE__);
// #endif
//                 FLIC_Reset_CurrentFrame(p_fields[field_num].Param5);
//                 // FLIC_Draw_XY(p_fields[field_num].Left, p_fields[field_num].Top, p_fields[field_num].Param5);
//                 FLIC_Draw(p_fields[field_num].Left, p_fields[field_num].Top, (SAMB_ptr)MK_FP(p_fields[field_num].Param5,0));
//                 VGA_SetFont(p_fields[field_num].Font_Index, p_fields[field_num].ColorSet1, 0, 0);
//                 Half_Font_Height = ( (VGA_GetFontHeight() - 1) / 2);
//                 VGA_DrawCentered(
//                     ((p_fields[field_num].Right - p_fields[field_num].Left) + p_fields[field_num].Left),
//                     (((p_fields[field_num].Bottom - p_fields[field_num].Top) / 2) + p_fields[field_num].Top - Half_Font_Height),
//                     p_fields[field_num].Param0
//                 );

        } break;
        case ft_RadioButton:            /*  1  0x01 */  //drake178: ToggleButton
        {

        } break;
        case ft_LockedButton:           /*  2  0x02 */  //drake178: LockableButton
        {

        } break;
        case ft_MultiButton:            /*  3  0x03 */  //drake178: MStateButton
        {

        } break;
        case ft_Input:                  /*  4  0x04 */  //drake178: EditBox
        {

        } break;
        case ft_ImageLabel:             /*  5  0x05 */  //drake178: ImageLabel      DNE/NIU in MoO2
        {

        } break;
        case ft_Scroll:                 /*  6  0x06 */  //drake178: SlideBar
        {

        } break;
        case ft_HotKey:                 /*  7  0x07 */  //drake178: Label
        {

        } break;
        case ft_MultiHotKey:            /*  8  0x08 */  //drake178: Ctrl_AltString
        {

        } break;
        case ft_ClickLink:              /*  9  0x09 */  //drake178: ClickLink       DNE/NIU in MoO2
        {

        } break;
        case ft_StringList:             /* 10  0x0A */  //drake178: DialogLine
        {

        } break;
        case ft_ContinuousStringInput:  /* 11  0x0B */  //drake178: EditSelect
        {

        } break;
        case ft_Grid:                   /* 12  0x0C */  //drake178: TODO
        {
            DLOG("switch(p_fields[field_num].type) case ft_Grid");
            if(up_down_flag == 1)  /* ¿ field up/down state: down ? */
            {
                DLOG("if(up_down_flag == 1)");

                /*
                    What it is, what is what, what is up?

                        Get the x,y coordinates relative to the field
                        ~ translate screen coordinates to field coordinates
                            field_x = Pointer_X() - p_fields[].x1
                            field_y = Pointer_Y() - p_fields[].y1

                        ~ translate field coordinates to grid coordinates
                            grid_x = field_x % box_width
                            grid_y = field_y % box_height
                        

                    e.g.,
                        Movement Map Grid Field
                                                                 (xmin, ymin, box_width, box_height, horizontal_count, vertical_count, *xpos, *ypos, help)
                            _main_map_grid_field = Add_Grid_Field(0, 20, 20, 18, 12, 10, &_main_map_grid_y, &_main_map_grid_x, ST_UNDEFINED);
                                p_fields[].x1 =  0
                                p_fields[].y1 = 20
                                p_fields[].x2 = 239 = (240 - 1) = (240 +  0 - 1) = ((20 * 12) +  0 - 1) = (box_width  * horizontal_count) + xmin - 1;
                                p_fields[].y2 = 199 = (200 - 1) = (180 + 20 - 1) = ((18 * 10) + 20 - 1) = (box_height * vertical_count  ) + ymin - 1;
                                p_fields[].Param1 = 20  box_width
                                p_fields[].Param2 = 18  box_height
                                p_fields[].Param3 = memory address of _main_map_grid_x
                                p_fields[].Param4 = memory address of _main_map_grid_y
                */

                screen_x = Pointer_X();
                screen_y = Pointer_Y();
                // ~ translate screen coordinates to field coordinates
                field_x = screen_x - p_fields[field_num].x1;  // always  0
                field_y = screen_y - p_fields[field_num].y1;  // always 20
                // ~ translate field coordinates to grid coordinates  (from pixels to squares)
                grid_x = field_x / p_fields[field_num].Param1;  // always 12  box_width
                grid_y = field_y / p_fields[field_num].Param2;  // always 10  box_height

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: field_num: %d\n", __FILE__, __LINE__, field_num);
//     dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].type: %d\n", __FILE__, __LINE__, p_fields[field_num].type);
// 
//     dbg_prn("DEBUG: [%s, %d]: screen_x: %d\n", __FILE__, __LINE__, screen_x);
//     dbg_prn("DEBUG: [%s, %d]: screen_y: %d\n", __FILE__, __LINE__, screen_y);
//     
//     dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].x1: %d\n", __FILE__, __LINE__, p_fields[field_num].x1);
//     dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].y1: %d\n", __FILE__, __LINE__, p_fields[field_num].y1);
//     
//     dbg_prn("DEBUG: [%s, %d]: field_x: %d\n", __FILE__, __LINE__, field_x);
//     dbg_prn("DEBUG: [%s, %d]: field_y: %d\n", __FILE__, __LINE__, field_y);
// 
//     dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param1: %d\n", __FILE__, __LINE__, p_fields[field_num].Param1);
//     dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param2: %d\n", __FILE__, __LINE__, p_fields[field_num].Param2);
// 
//     dbg_prn("DEBUG: [%s, %d]: grid_x: %d\n", __FILE__, __LINE__, grid_x);
//     dbg_prn("DEBUG: [%s, %d]: grid_y: %d\n", __FILE__, __LINE__, grid_y);
// 
//     // dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param3: %d\n", __FILE__, __LINE__, p_fields[field_num].Param3);
//     // dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param4: %d\n", __FILE__, __LINE__, p_fields[field_num].Param4);
//     // dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param3: 0x%02X\n", __FILE__, __LINE__, p_fields[field_num].Param3);
//     // dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param4: 0x%02X\n", __FILE__, __LINE__, p_fields[field_num].Param4);
//     dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param3: 0x%08X\n", __FILE__, __LINE__, p_fields[field_num].Param3);
//     dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param4: 0x%08X\n", __FILE__, __LINE__, p_fields[field_num].Param4);
// 
//     // dbg_prn("DEBUG: [%s, %d]: (int16_t *)p_fields[field_num].Param3: %p\n", __FILE__, __LINE__, (int16_t *)p_fields[field_num].Param3);
//     // dbg_prn("DEBUG: [%s, %d]: (int16_t *)p_fields[field_num].Param4: %p\n", __FILE__, __LINE__, (int16_t *)p_fields[field_num].Param4);
//     dbg_prn("DEBUG: [%s, %d]: (int64_t *)p_fields[field_num].Param3: %p\n", __FILE__, __LINE__, (int64_t *)p_fields[field_num].Param3);
//     dbg_prn("DEBUG: [%s, %d]: (int64_t *)p_fields[field_num].Param4: %p\n", __FILE__, __LINE__, (int64_t *)p_fields[field_num].Param4);
// 
//     // ~ get the value in Param3, as an unsigned 2-byte value, treat it as a pointer to a signed 2-byte value
//     // dbg_prn("DEBUG: [%s, %d]: *( (int16_t *) ((uint16_t)p_fields[field_num].Param3) ): %d\n", __FILE__, __LINE__, *( (int16_t *) ((uint16_t)p_fields[field_num].Param3) ) );
//     // dbg_prn("DEBUG: [%s, %d]: *( (int16_t *) ((uint16_t)p_fields[field_num].Param4) ): %d\n", __FILE__, __LINE__, *( (int16_t *) ((uint16_t)p_fields[field_num].Param4) ) );
//     dbg_prn("DEBUG: [%s, %d]: *((int64_t *)p_fields[field_num].Param3): %d\n", __FILE__, __LINE__, *((int64_t *)p_fields[field_num].Param3));
//     dbg_prn("DEBUG: [%s, %d]: *((int64_t *)p_fields[field_num].Param4): %d\n", __FILE__, __LINE__, *((int64_t *)p_fields[field_num].Param4));
//     // ¿ should show addresses of _main_map_grid_x and _main_map_grid_y, correct?
// 
// #endif

            // ¿ same code as in Add_Grid_Field() ?
            // NO!! Here, assigning value, there assigning memory address

            // ~ access Param3/4 as an unsigned 2-byte value, treat that value as a pointer to a signed 2-byte value, dereference that pointer to assign a signed 2-byte value
            // *( (int16_t *) ( (uint16_t) p_fields[field_num].Param3 ) ) = grid_x;
            // *( (int16_t *) ( (uint16_t) p_fields[field_num].Param4 ) ) = grid_y;
            *((int64_t *)p_fields[field_num].Param3) = grid_x;
            *((int64_t *)p_fields[field_num].Param4) = grid_y;

// #ifdef STU_DEBUG
//     // dbg_prn("DEBUG: [%s, %d]: *( (int16_t *) ((uint16_t)p_fields[field_num].Param3) ): %d\n", __FILE__, __LINE__, *( (int16_t *) ((uint16_t)p_fields[field_num].Param3) ) );
//     // dbg_prn("DEBUG: [%s, %d]: *( (int16_t *) ((uint16_t)p_fields[field_num].Param4) ): %d\n", __FILE__, __LINE__, *( (int16_t *) ((uint16_t)p_fields[field_num].Param4) ) );
//     dbg_prn("DEBUG: [%s, %d]: *((int64_t *)p_fields[field_num].Param3): %d\n", __FILE__, __LINE__, *((int64_t *)p_fields[field_num].Param3));
//     dbg_prn("DEBUG: [%s, %d]: *((int64_t *)p_fields[field_num].Param4): %d\n", __FILE__, __LINE__, *((int64_t *)p_fields[field_num].Param4));
// #endif

            }
        } break;

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Field()\n", __FILE__, __LINE__);
#endif
}

// WZD s36p73
void Push_Field_Down(int16_t field_num, int16_t mouse_x, int16_t mouse_y)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Push_Field_Down()\n", __FILE__, __LINE__);
#endif


    if( (mouse_x >= 0) && (mouse_x < 320) && (mouse_y >= 0) && (mouse_y < 200) )
    {
        // DONT  if(mouse_installed != ST_FALSE)
        // DONT  {
        // DONT      Restore_Mouse_On_Page();
        // DONT      VGA_SetDirectDraw();
        // DONT      down_mouse_button = field_num;
        // DONT      Draw_Field(field_num, 1);
        // DONT      down_mouse_button = -1;
        // DONT      VGA_Set_DSP_Addr();
        // DONT      Save_Mouse_On_Page(mouse_x, mouse_y);
        // DONT      Draw_Mouse_On_Page(mouse_x, mouse_y);
        // DONT      Set_Pointer_Position(mouse_x, mouse_y);
        // DONT  } else {

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

                    /*
                        Yay/Nay Redraw Previous
                        Yay:
                            Fall-Through:
                                previous control is not Ctrl_MStateButton
                                previous control is Ctrl_MStateButton but current control is not Ctrl_MStateButton
                                previous control is Ctrl_DialogLine and current control is Ctrl_DialogLine
                                current control is Selectable

                        Nay:
                            previous control is Ctrl_MStateButton but current control is not Ctrl_MStateButton
                            previous control is Ctrl_DialogLine but current control is not Ctrl_MStateButton
                            current control is not Selectable

                    */

                    if( !((p_fields[down_mouse_button].type == ft_MultiButton) && (p_fields[field_num].type != ft_MultiButton)) )
                    {
                        if(p_fields[down_mouse_button].type != ft_StringList)
                        {
                            Draw_Field(down_mouse_button, 0);
                        }
                        else
                        {
                            if( (p_fields[down_mouse_button].type == ft_StringList) && (p_fields[field_num].Selectable != ST_FALSE) )
                            {
                                Draw_Field(down_mouse_button, 0);
                            }
                        }
                    }

                    // @@DrawControl:
                    down_mouse_button = field_num;
                    Draw_Field(field_num, 1);

                    if(p_fields[field_num].type == ft_Input)
                    {
                        mouse_x = Pointer_X();
                        mouse_y = Pointer_Y();
                    }

                    Save_Mouse_On_Page(mouse_x, mouse_y);
                    Draw_Mouse_On_Page(mouse_x, mouse_y);
                    Set_Pointer_Position(mouse_x, mouse_y);

                }

            }
            else
            {
                Restore_Mouse_On_Page();
                down_mouse_button = field_num;
                Draw_Field(field_num, 1);

                if(p_fields[field_num].type != ft_Input)
                {
                    mouse_x = Pointer_X();
                    mouse_y = Pointer_Y();
                }
                
                Save_Mouse_On_Page(mouse_x, mouse_y);
                Draw_Mouse_On_Page(mouse_x, mouse_y);
                Set_Pointer_Position(mouse_x, mouse_y);

            }

        }

        Restore_Mouse_State();
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Push_Field_Down()\n", __FILE__, __LINE__);
#endif
}
