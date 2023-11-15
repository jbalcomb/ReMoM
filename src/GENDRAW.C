/*
    WIZARDS.EXE
        ovr149

    MoO2
        GENDRAW
        TEXTBOX
        Message_Box_() |-> Do_Text_Box()

*/
/*
    ¿ Data Segment Layout ?
        multiple groupings, but grouped and continguous
            so, ...
        any/all in order    
*/
#include "MoX.H"



// WZD dseg:6F78
uint8_t COL_ConfirmShadows[5] = {0x11, 0x11, 0x11, 0x11, 0x11};
// WZD dseg:6F7D
uint8_t COL_Dialog_Text[5] = {0x06, 0x09, 0x0B, 0x0D, 0x0F};



/*
    BEGIN:  Selection Box
*/
// WZD dseg:6F82 14 B2 B3 B4 B4                                  
uint8_t COL_ListSel_Shadows[5] = {0x14, 0xB2, 0xB3, 0xB4, 0xB4};
// WZD dseg:6F82                                                                                         ; DATA XREF: Selection_Box_Draw+5E6o
// WZD dseg:6F87 66 FB FB FB FB                                  
uint8_t COL_ListSel_Titles[5] = {0x66, 0xFB, 0xFB, 0xFB, 0xFB};
/*
    END:  Selection Box
*/

// WZD dseg:6F87                                                 END:  Selection Box
// WZD dseg:6F8C E4 E4 E4 E4 E4                                  COL_Warning1 db 0E4h, 0E4h, 0E4h, 0E4h, 0E4h
// WZD dseg:6F8C                                                                                         ; DATA XREF: GUI_DrawRedMessage+76o
// WZD dseg:6F91 2D 2D 2D 2D 2D                                  COL_Warning2 db 2Dh, 2Dh, 2Dh, 2Dh, 2Dh ; DATA XREF: GUI_DrawRedMessage:loc_D6C3Do

// WZD dseg:6F96
// drake178: RP_GUI_Confirm_XShift
int16_t confirmation_box_x_offset = 0;

// WZD dseg:6F98
// drake178: RP_GUI_Confirm_YShift
int16_t confirmation_box_y_offset = 0;

// WZD dseg:6F9A 00 00                                           GUI_Notify_Compose dw 0                 ; DATA XREF: EZ_Resolve+13w ...
// WZD dseg:6F9A                                                                                         ; 1 - notify dialogs are drawn but not displayed
// WZD dseg:6F9A                                                                                         ; 2 - notify dialogs save the current display frame as the image they will draw on
// WZD dseg:6F9C F0 F0 F0 F0 F0 F0 F0 F0 F0 F0 F0 F0 F0 F0 F0 14+COL_Notify_Shadows db 0Fh dup(0F0h), 5 dup(14h)
// WZD dseg:6F9C 14 14 14 14                                                                             ; DATA XREF: GUI_DrawNotifyDialog+264o ...
// WZD dseg:6FB0 19 B2 B3 B4 B4 19 B2 B3 B4 B4 19 B2 B3 B4 B4 19+COL_Notify_Text db 19h, 0B2h, 0B3h, 0B4h, 0B4h, 19h, 0B2h, 0B3h, 0B4h, 0B4h, 19h, 0B2h, 0B3h, 0B4h, 0B4h, 19h, 0B2h, 0B3h, 0B4h, 0B4h
// WZD dseg:6FB0 B2 B3 B4 B4                                                                             ; DATA XREF: GUI_DrawNotifyDialog+2D5o ...
// WZD dseg:6FC4 FE FF 00 00 EA FF 08 00 02 00                   GUI_Familiar_IMGLeft dw 0FFFEh, 0, 0FFEAh, 8, 2
// WZD dseg:6FC4                                                                                         ; DATA XREF: GUI_Familiar_Notify+159r ...
// WZD dseg:6FCE 02 00 EC FF FC FF F4 FF FD FF                   GUI_Familiar_IMGTop dw 2, 0FFECh, 0FFFCh, 0FFF4h, 0FFFDh
// WZD dseg:6FCE                                                                                         ; DATA XREF: GUI_DrawFamiliarNtfy+ADr
// WZD dseg:6FD8 46 00 49 00 3E 00 44 00 46 00                   GUI_Familiar_IMGText dw 46h, 49h, 3Eh, 44h, 46h
// WZD dseg:6FD8                                                                                         ; DATA XREF: GUI_DrawFamiliarNtfy+62r ...
// WZD dseg:6FE2 01 00                                           ITEM_ViewGrowStage dw 1                 ; DATA XREF: ITEM_GetViewDrawPos:loc_D9474r ...

/*
    BEGIN:
        Confirmation Box
        Selection Box
*/
// WZD dseg:6FE4
char cnst_RESOURCE_File4[] = "RESOURCE";
// WZD dseg:6FEC 00                                              
char cnst_ZeroString_5[] = "";
/*
    END:
        Confirmation Box
        Selection Box
*/

/*
    BEGIN:
        Confirmation Box
*/
// WZD dseg:6FED
char cnst_HOTKEY_Y_3[] = "Y";

// WZD dseg:6FEF
char cnst_HOTKEY_N_5[] = "N";

/*
    BEGIN:
        Confirmation Box
*/

/*
    BEGIN:
        Confirmation Box
        Selection Box
*/
// WZD dseg:6FF1
char cnst_HOTKEY_Esc21[] = "\x1B";
/*
    END:
        Confirmation Box
        Selection Box
*/

// align 2?
// WZD dseg:6FF3 00                                              db    0
// WZD dseg:6FF3                                                 ? END: ?










/*
    BEGIN:  Selection Box
*/
// WZD dseg:CAA0
int16_t selectbox_fields[10];
// WZD dseg:CAB4 00 00                                           
char * selectbox_title;
// WZD dseg:CAB6 00 00                                           
// char * selectbox_list[];
char ** selectbox_list;
// WZD dseg:CAB8 00 00                                           
int16_t selectbox_items;
// WZD dseg:CABA 00 00                                           
int16_t selectbox_first_item;
// WZD dseg:CABC 00 00                                           
int16_t selectbox_highlight_item;
// WZD dseg:CABE 00 00                                           
int16_t UU_selectbox;
// WZD dseg:CAC0 00 00                                           
int16_t selectbox_y2;
// WZD dseg:CAC2 00 00                                           
int16_t selectbox_x2;
// WZD dseg:CAC4 00 00                                           
int16_t selectbox_y1;
// WZD dseg:CAC6 00 00                                           
int16_t selectbox_x1;
// WZD dseg:CAC8 00 00                                           
int16_t selectbox_multi;
// WZD dseg:CACA 00 00                                           
int16_t GUI_ListSel_Count;
/*
    END:  Selection Box
*/



// WZD dseg:CACC 00 00                                           
SAMB_ptr confirmation_button_no_seg;
// WZD dseg:CACC                                                                                         ; 2 frame image appended in the LBX_Sandbox_Segment
// WZD dseg:CACE 00 00                                           
SAMB_ptr confirmation_button_yes_seg;
// WZD dseg:CACE                                                                                         ; 2 frame image appended in the LBX_Sandbox_Segment
// WZD dseg:CAD0 00 00                                           
SAMB_ptr confirmation_background_bottom_seg;
// WZD dseg:CAD0                                                                                         ; single image appended in the LBX_Sandbox_Segment
// WZD dseg:CAD2 00 00                                           
SAMB_ptr confirmation_background_top_seg;
// WZD dseg:CAD2                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment




/*
    BEGIN:  Selection Box
*/
// WZD dseg:CB0A
SAMB_ptr selectbk_scroll_locked_down_seg;
// WZD dseg:CB0C
SAMB_ptr selectbk_scroll_locked_up_seg;
// WZD dseg:CB0E
SAMB_ptr selectbk_scroll_bottom_seg;
// WZD dseg:CB10
SAMB_ptr selectbk_scroll_top_seg;
// WZD dseg:CB12
SAMB_ptr selectbk_bottom_arrow_seg;
// WZD dseg:CB14
SAMB_ptr selectbk_top_arrow_seg;
// WZD dseg:CB16
// drake178: IMG_GUI_ListS_IBGRs@
// ; array of 5 2-frame images appended into the sandbox
SAMB_ptr selectbk_button_22to26_seg[10];
// WZD dseg:CB2A
// drake178: IMG_GUI_ListS_IBGs@
// ; array of 5 2-frame images appended into the sandbox
SAMB_ptr selectbk_button_12to16_seg[10];
// WZD dseg:CB3E
SAMB_ptr selectbd_scroll_seg;
// WZD dseg:CB40
SAMB_ptr selectbd_bottom_right_seg;
// WZD dseg:CB42
SAMB_ptr selectbd_right_seg;
// WZD dseg:CB44
SAMB_ptr selectbd_bottom_seg;
// WZD dseg:CB46
SAMB_ptr selectbd_top_seg;
// WZD dseg:CB48
SAMB_ptr selectbd_bottom_left_seg;
// WZD dseg:CB4A
SAMB_ptr selectbd_left_seg;
/*
    END:  Selection Box
*/



// WZD dseg:CB4C 00 00                                           GUI_FamiliarIMGIndex dw 0               ; DATA XREF: GUI_Notify_Dialog+16w ...

// WZD dseg:CB4E
char * confirmation_box_text;

// WZD dseg:CB50 00 00                                           
// drake178:  GUI_Dialog_Top;
int16_t confirmation_box_y;
// WZD dseg:CB52 00 00                                           
// drake178: GUI_Dialog_Left;
int16_t confirmation_box_x;

// WZD dseg:CB54 00 00                                           GUI_RedMsg_Type dw 0                    ; DATA XREF: GUI_WarningType1+3w ...
// WZD dseg:CB54                                                                                         ; determines whether to use the first or the second
// WZD dseg:CB54                                                                                         ; of the two different warning message backgrounds
// WZD dseg:CB54                                                                                         ; (both of which are red, but slightly different and
// WZD dseg:CB54                                                                                         ; use a different font color)
// WZD dseg:CB56                                                 ovr150





// WZD o149p01
// drake178: GUI_Confirm_Dialog()
int16_t Confirmation_Box(char * text_string)
{

    int16_t choice;
    int16_t Text_Label_Index;
    int16_t hotkey_ESC;
    int16_t confirmation_button_no;
    int16_t confirmation_button_yes;
    int16_t input_field_idx;
    int16_t Box_Height;

    int16_t paragraph_height;
    int16_t leave_screen;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Confirmation_Box(text_string = %s)\n", __FILE__, __LINE__, text_string);
#endif

    Save_Alias_Colors();

    Set_Font_Colors_15(0, &COL_Dialog_Text[0]);

    Save_ScreenSeg();

 
    // RESOURCE.LBX, 0  "CONFMBAK", ""
    confirmation_background_top_seg = LBX_Reload(cnst_RESOURCE_File4, 0, _screen_seg);
    // RESOURCE.LBX, 1  "CONFMBAK", ""
    confirmation_background_bottom_seg = LBX_Reload_Next(cnst_RESOURCE_File4, 1, _screen_seg);
    // RESOURCE.LBX, 3  CONFMBUT    yes
    confirmation_button_yes_seg = LBX_Reload_Next(cnst_RESOURCE_File4, 3, _screen_seg);
    // RESOURCE.LBX, 4  CONFMBUT    no
    confirmation_button_no_seg = LBX_Reload_Next(cnst_RESOURCE_File4, 4, _screen_seg);

    confirmation_box_text = text_string;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: confirmation_box_text: %s)\n", __FILE__, __LINE__, confirmation_box_text);
#endif


    Set_Font(4, 4, 15, 0);


    paragraph_height = Get_Paragraph_Max_Height(166, text_string);

    Box_Height = paragraph_height + 34;
    confirmation_box_x = 68 + confirmation_box_x_offset;
    confirmation_box_y = ((SCREEN_HEIGHT - Box_Height) / 2) + confirmation_box_y_offset;

    Clear_Fields();

    // confirmation_button_yes = Add_Button_Field((confirmation_box_x + 101), (confirmation_box_y + paragraph_height + 15), cnst_ZeroString_5, confirmation_button_yes_seg, cnst_HOTKEY_Y_3, -1);
    confirmation_button_yes = Add_Button_Field((confirmation_box_x + 101), (confirmation_box_y + paragraph_height + 15), cnst_ZeroString_5, confirmation_button_yes_seg, 'Y', -1);

    // confirmation_button_no = Add_Button_Field((confirmation_box_x + 18), (confirmation_box_y + paragraph_height + 15), cnst_ZeroString_5, confirmation_button_no_seg, cnst_HOTKEY_N_5, -1);
    confirmation_button_no = Add_Button_Field((confirmation_box_x + 18), (confirmation_box_y + paragraph_height + 15), cnst_ZeroString_5, confirmation_button_no_seg, 'N', -1);

    // Text_Label_Index = Add_Hidden_Field(confirmation_box_x, confirmation_box_y, (confirmation_box_x + 185), (confirmation_box_y + 63), cnst_ZeroString_5, -1);
    Text_Label_Index = Add_Hidden_Field(confirmation_box_x, confirmation_box_y, (confirmation_box_x + 185), (confirmation_box_y + 63), 0, -1);

    // hotkey_ESC = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_Esc21, -1);
    hotkey_ESC = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, '\x1B', ST_UNDEFINED);

    // TODO  Assign_Auto_Function(Confirmation_Box_Draw, 1);


    leave_screen = ST_FALSE;
    while(leave_screen == ST_FALSE)
    {

        // NOTE: This `abs()` means right-clicking on a Confirmation Box is equivalent to left-clicking
        input_field_idx = abs(Get_Input());

        if( (input_field_idx == ST_UNDEFINED) || (input_field_idx == confirmation_button_no) )
        {
            leave_screen = ST_TRUE;
            choice = ST_FALSE;
        }

        if(input_field_idx == confirmation_button_yes)
        {
            leave_screen = ST_TRUE;
            choice = ST_TRUE;
        }

        Confirmation_Box_Draw();
        PageFlip_FX();
    }

    // TODO  Deactivate_Auto_Function();
    Clear_Fields();
    Restore_Alias_Colors();
    Reset_Window();
    Restore_ScreenSeg();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Confirmation_Box(text_string = %s)\n", __FILE__, __LINE__, text_string);
#endif

    return choice;
}

// WZD o149p02
// drake178: GUI_DrawConfrmDialog()
void Confirmation_Box_Draw(void)
{
    int16_t paragraph_height;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Confirmation_Box_Draw()\n", __FILE__, __LINE__);
#endif

    Set_Font(4, 4, 15, 0);

    paragraph_height = Get_Paragraph_Max_Height(166, confirmation_box_text);

    Set_Page_Off();

    Set_Window(0, 0, SCREEN_XMAX, (confirmation_box_y + paragraph_height + 12));

    Clipped_Draw(confirmation_box_x, confirmation_box_y, confirmation_background_top_seg);

    Reset_Window();

    FLIC_Draw(confirmation_box_x, (confirmation_box_y + paragraph_height + 10), confirmation_background_bottom_seg);

    Set_Font_Colors_15(4, &COL_ConfirmShadows[0]);
    Set_Font(4, 15, 15, 0);
    Print_Paragraph((confirmation_box_x + 10), (confirmation_box_y + 10), 166, confirmation_box_text, 0);  // print_type 0: Print Left Aligned
    Print_Paragraph((confirmation_box_x + 9), (confirmation_box_y + 10), 166, confirmation_box_text, 0);  // print_type 0: Print Left Aligned

    Set_Alias_Color(18);
    Set_Font_Colors_15(4, &COL_Dialog_Text[0]);
    Set_Font(4, 4, 15, 0);
    Print_Paragraph((confirmation_box_x + 9), (confirmation_box_y + 9), 166, confirmation_box_text, 0);  // print_type 0: Print Left Aligned

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Confirmation_Box_Draw()\n", __FILE__, __LINE__);
#endif

}


// WZD o149p03
// GUI_WarningType1    

// WZD o149p04
// drake178: GUI_WarningType0()
void GUI_WarningType0(char * msg)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GUI_WarningType0(msg = %s)\n", __FILE__, __LINE__, msg);
#endif



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GUI_WarningType0(msg = %s)\n", __FILE__, __LINE__, msg);
#endif

}

// WZD o149p05
// GUI_ShowRedMessage  

// WZD o149p06
void GUI_DrawRedMessage(void)
{

    // Set_Font(4, 4, 4);

    // _SI_ = VGA_GetTextHeight(166, GUI_Dialog_Text@)

    // Set_Page_Off();

    // Set_Window(0, 0, 319, (GUI_Dialog_Top + _SI_ + 12);

//     if(GUI_RedMsg_Type == 0)
//     {
// VGA_WndDrawLBXImage(GUI_Dialog_Left, GUI_Dialog_Top, IMG_GUI_RedMessage1@);
//     }
//         
//     else
//     {
// VGA_WndDrawLBXImage(GUI_Dialog_Left, GUI_Dialog_Top, IMG_GUI_RedMessage2@);
//     }

    // Reset_Window();

//     if(GUI_RedMsg_Type == 0)
//     {
// Set_Font_Colors_15(4, COL_Warning1);
//     }
//         
//     else
//     {
// Set_Font_Colors_15(4, COL_Warning2);
//     }

    // Set_Font(4, 15, 15);

// mov     ax, 2
// push    ax                              ; Align
// push    [GUI_Dialog_Text@]              ; Text@
// mov     ax, 166
// push    ax                              ; Width
// mov     ax, [GUI_Dialog_Top]
// add     ax, 11
// push    ax                              ; Top
// mov     ax, [GUI_Dialog_Left]
// add     ax, 11
// push    ax                              ; Left
// call    Print_Paragraph   

// mov     ax, 2
// push    ax                              ; Align
// push    [GUI_Dialog_Text@]              ; Text@
// mov     ax, 166
// push    ax                              ; Width
// mov     ax, [GUI_Dialog_Top]
// add     ax, 11
// push    ax                              ; Top
// mov     ax, [GUI_Dialog_Left]
// add     ax, 10
// push    ax                              ; Left
// call    Print_Paragraph                 ; drake178: VGA_DrawText

    // Set_Alias_Color(184);

    // Set_Font(4, 4, 4);

// mov     ax, 2
// push    ax                              ; Align
// push    [GUI_Dialog_Text@]              ; Text@
// mov     ax, 166
// push    ax                              ; Width
// mov     ax, [GUI_Dialog_Top]
// add     ax, 10
// push    ax                              ; Top
// mov     ax, [GUI_Dialog_Left]
// add     ax, 10
// push    ax                              ; Left
// call    Print_Paragraph                 ; drake178: VGA_DrawText

//     if(GUI_RedMsg_Type == 0)
//     {
//     FLIC_Draw(GUI_Dialog_Left, (GUI_Dialog_Top + _SI_ + 10), IMG_GUI_RedMsg1Btm@);
//     }
//         
//     else
//     {
//     FLIC_Draw(GUI_Dialog_Left, (GUI_Dialog_Top + _SI_ + 10), IMG_GUI_RedMsg2Btm@);
//     }

}

// WZD o149p07
/*
    returns the index of the selection

CMB_RaiseDead()
    calls with 'mutli page' TRUE and size of 6
CMB_AnimateDead()
    calls with 'mutli page' TRUE and size of 6
IDK_ResurectHero_sBCC70
    calls with 'mutli page' TRUE and size of 6

*/
// int16_t Selection_Box(int16_t item_count, char * list_text[], int16_t multi_page, char * title_string)
int16_t Selection_Box(int16_t item_count, char ** list_text, int16_t multi_page, char * title_string)
{
    int16_t selectbox_window;
    int16_t list_select_idx;
    int16_t hotkey_ESC;
    int16_t Down_Arrow_Ctrl_Index;
    int16_t Up_Arrow_Ctrl_Index;
    int16_t leave_screen;

    int16_t itr;
    int16_t input_field_idx;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Selection_Box()\n", __FILE__, __LINE__);
#endif

    // ; saves the anti-aliasing colors of each font color
    // ; set to static variables starting at dseg:e800 (16
    // ; color blocks + current colors + 3 default sets)
    Save_Alias_Colors();

    Copy_On_To_Off_Page();
    Copy_Off_To_Back();

    // ; saves the first 64k of the LBX_Sandbox_Segment into
    // ; pages 1-5 of the VGAFILEH EMS handle
    // EMM_Sandbox2VGAFILEH()
    Save_ScreenSeg();


    selectbd_left_seg = LBX_Reload(cnst_RESOURCE_File4, 5, _screen_seg);

    selectbd_bottom_left_seg         = LBX_Reload_Next(cnst_RESOURCE_File4,  6, _screen_seg);
    selectbd_top_seg                 = LBX_Reload_Next(cnst_RESOURCE_File4,  7, _screen_seg);
    selectbd_bottom_seg              = LBX_Reload_Next(cnst_RESOURCE_File4,  9, _screen_seg);
    selectbd_right_seg               = LBX_Reload_Next(cnst_RESOURCE_File4,  8, _screen_seg);
    selectbd_bottom_right_seg        = LBX_Reload_Next(cnst_RESOURCE_File4, 10, _screen_seg);
    selectbd_scroll_seg              = LBX_Reload_Next(cnst_RESOURCE_File4, 11, _screen_seg);

    selectbk_top_arrow_seg           = LBX_Reload_Next(cnst_RESOURCE_File4, 32, _screen_seg);
    selectbk_bottom_arrow_seg        = LBX_Reload_Next(cnst_RESOURCE_File4, 33, _screen_seg);
    selectbk_scroll_top_seg          = LBX_Reload_Next(cnst_RESOURCE_File4, 34, _screen_seg);
    selectbk_scroll_bottom_seg       = LBX_Reload_Next(cnst_RESOURCE_File4, 35, _screen_seg);
    selectbk_scroll_locked_up_seg    = LBX_Reload_Next(cnst_RESOURCE_File4, 36, _screen_seg);
    selectbk_scroll_locked_down_seg  = LBX_Reload_Next(cnst_RESOURCE_File4, 37, _screen_seg);

    /*
        12  SELECTBK    button 1
        13  SELECTBK    button 2
        14  SELECTBK    button 3
        15  SELECTBK    button 4
        16  SELECTBK    button 5

        22  SELECTBK    button 1
        23  SELECTBK    button 2
        24  SELECTBK    button 3
        25  SELECTBK    button 4
        26  SELECTBK    button 5
    */
    for(itr = 0; itr < 5; itr++)
    {
        selectbk_button_12to16_seg[itr] = LBX_Reload_Next(cnst_RESOURCE_File4, (itr + 12), _screen_seg);

        selectbk_button_22to26_seg[itr] = LBX_Reload_Next(cnst_RESOURCE_File4, (itr + 22), _screen_seg);
    }

    for(itr = 5; itr < 10; itr++)
    {
        selectbk_button_12to16_seg[itr] = selectbk_button_12to16_seg[itr - 5];
        selectbk_button_22to26_seg[itr] = selectbk_button_22to26_seg[itr - 5];
    }



    selectbox_multi = multi_page;
    selectbox_items = item_count;
    selectbox_list = list_text;
    selectbox_title = title_string;
    selectbox_first_item = 0;
    selectbox_highlight_item = 0;


    Selection_Box_Coords(item_count, selectbox_list, title_string);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: selectbox_x1: %d\n", __FILE__, __LINE__, selectbox_x1);
//     dbg_prn("DEBUG: [%s, %d]: selectbox_y1: %d\n", __FILE__, __LINE__, selectbox_y1);
//     dbg_prn("DEBUG: [%s, %d]: selectbox_x2: %d\n", __FILE__, __LINE__, selectbox_x2);
//     dbg_prn("DEBUG: [%s, %d]: selectbox_y2: %d\n", __FILE__, __LINE__, selectbox_y2);
// #endif


    // TODO  Assign_Auto_Function(Selection_Box_Draw, 1);


    leave_screen = ST_FALSE;


    Up_Arrow_Ctrl_Index = INVALID_FIELD;
    Down_Arrow_Ctrl_Index = INVALID_FIELD;

    for(itr = 0; itr < 10; itr++)
    {
        selectbox_fields[itr] = INVALID_FIELD;
    }

    hotkey_ESC = INVALID_FIELD;



    while(leave_screen == ST_FALSE)
    {
        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_ESC)
        {
            leave_screen = ST_TRUE;
            list_select_idx = ST_UNDEFINED;
        }

        for(itr = 0; (itr < selectbox_items) && (itr < GUI_ListSel_Count); itr++)
        {
            if(selectbox_fields[itr] == input_field_idx)
            {
                leave_screen = ST_TRUE;
                list_select_idx = selectbox_first_item + itr;
            }
        }

        if(input_field_idx == Up_Arrow_Ctrl_Index)
        {
            selectbox_first_item--;
        }
        if(input_field_idx == Down_Arrow_Ctrl_Index)
        {
            selectbox_first_item++;
        }

        Set_Page_Off();

        Selection_Box_Draw();

        Clear_Fields();

        Up_Arrow_Ctrl_Index = INVALID_FIELD;
        Down_Arrow_Ctrl_Index = INVALID_FIELD;

        for(itr = 0; itr < 10; itr++)
        {
            selectbox_fields[itr] = INVALID_FIELD;
        }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: selectbox_items: %d\n", __FILE__, __LINE__, selectbox_items);
    dbg_prn("DEBUG: [%s, %d]: GUI_ListSel_Count: %d\n", __FILE__, __LINE__, GUI_ListSel_Count);
#endif

        for(itr = 0; (itr < selectbox_items) && (itr < GUI_ListSel_Count); itr++)
        {
            if(selectbox_multi == ST_TRUE)
            {
                // DLOG("(selectbox_multi == ST_TRUE)");
                y2 = selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * (itr + 1));
                x2 = selectbox_x2 - FLIC_Get_Width(selectbd_right_seg);
                y1 = selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * itr);
                x1 = selectbox_x1 + FLIC_Get_Width(selectbd_left_seg) + FLIC_Get_Width(selectbd_scroll_seg);

                // selectbox_fields[itr] = Add_Hidden_Field(x1, y1, x2, y2,  cnst_ZeroString_5, -1);
                selectbox_fields[itr] = Add_Hidden_Field(x1, y1, x2, y2, 0, -1);
            }
            else
            {
                // DLOG("(selectbox_multi != ST_TRUE)");
                y2 = selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * (itr + 1));
                x2 = selectbox_x2 - FLIC_Get_Width(selectbd_right_seg);
                y1 = selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * itr);
                x1 = selectbox_x1 + FLIC_Get_Width(selectbd_left_seg);

                // selectbox_fields[itr] = Add_Hidden_Field(x1, y1, x2, y2,  cnst_ZeroString_5, -1);
                selectbox_fields[itr] = Add_Hidden_Field(x1, y1, x2, y2, 0, -1);
            }
        }

#ifdef STU_DEBUG
        for(itr = 0; (itr < selectbox_items) && (itr < GUI_ListSel_Count); itr++)
        {
            dbg_prn("DEBUG: [%s, %d]: selectbox_fields[%d]: %d\n", __FILE__, __LINE__, itr, selectbox_fields[itr]);
        }
#endif

        if(selectbox_multi == ST_TRUE)
        {

            if(selectbox_first_item > 0)
            {
// mov     ax, -1
// push    ax                              ; help
// mov     ax, offset cnst_ZeroString_5
// push    ax                              ; hotkey
// push    [selectbk_top_arrow_seg]        ; pict_seg
// mov     ax, offset cnst_ZeroString_5
// push    ax                              ; string
// push    [selectbd_top_seg]              ; pict_seg
// call    FLIC_Get_Height                 ; returns the draw height of an LBX image
// mov     dx, [selectbox_y1]
// add     dx, ax
// add     dx, 3
// push    dx                              ; ymin
// push    [selectbd_left_seg]             ; pict_seg
// call    FLIC_Get_Width
// mov     dx, [selectbox_x1]
// add     dx, ax
// add     dx, 4
// push    dx                              ; xmin
// call    Add_Button_Field
// mov     [bp+Up_Arrow_Ctrl_Index], ax
            }

            if((selectbox_first_item + item_count) < GUI_ListSel_Count)
            {
// mov     ax, -1
// push    ax                              ; help
// mov     ax, offset cnst_ZeroString_5
// push    ax                              ; hotkey
// push    [selectbk_bottom_arrow_seg]     ; pict_seg
// mov     ax, offset cnst_ZeroString_5
// push    ax                              ; string
// push    [selectbd_bottom_seg]           ; pict_seg
// call    FLIC_Get_Height                 ; returns the draw height of an LBX image
// pop     cx
// mov     dx, [selectbox_y2]
// sub     dx, ax
// add     dx, -2
// push    dx
// push    [selectbk_bottom_arrow_seg]     ; pict_seg
// call    FLIC_Get_Height                 ; returns the draw height of an LBX image
// pop     cx
// pop     dx
// sub     dx, ax
// push    dx                              ; ymin
// push    [selectbd_left_seg]             ; pict_seg
// call    FLIC_Get_Width
// pop     cx
// mov     dx, [selectbox_x1]
// add     dx, ax
// add     dx, 4
// push    dx                              ; xmin
// call    Add_Button_Field
// add     sp, 0Ch
// mov     [bp+Down_Arrow_Ctrl_Index], ax
            }

        }


        // selectbox_window = Add_Hidden_Field(selectbox_x1, selectbox_y1, selectbox_x2, selectbox_y2, cnst_ZeroString_5, -1);
        selectbox_window = Add_Hidden_Field(selectbox_x1, selectbox_y1, selectbox_x2, selectbox_y2, 0, -1);

        // hotkey_ESC = Add_Hidden_Field(0, 0, 319, 199, cnst_HOTKEY_Esc21, -1);
        hotkey_ESC = Add_Hidden_Field(0, 0, 319, 199, '\x1B', -1);

        PageFlip_FX();

    }  /* END: while(leave_screen == ST_FALSE)*/



    // TODO  Deactivate_Auto_Function();

    Restore_Alias_Colors();

    // ; loads pages 1-5 of the VGAFILEH EMS handle into the
    // ; first 64k of the LBX_Sandbox_Segment
    // ; BUG: restoring the original sandbox removes the
    // ; images of the up and down arrows from memory - these
    // ; are needed for the standard GUI redraw functions,
    // ; and will cause a crash if the page is flipped before
    // ; the GUI is cleared
    // EMM_VGAFILEH2Sandbox();
    Restore_ScreenSeg();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Selection_Box()\n", __FILE__, __LINE__);
#endif

    return list_select_idx;
}


// WZD o149p08
/*
    The palette for the Selection Box is in selectbd_top_seg - RESOURCE.LBX, 7  "SELECTBD","top".

    OoO:
        Draw Selection Box Borders - Left, Top  (Top-Left is part of Left, Top-Right is part of Right)
        Draw Button Backgrounds
        Draw Selection Box Borders - Bottom, Bottom-Left, Right, Bottom-Right
        Draw Button Text

*/
void Selection_Box_Draw(void)
{
    int16_t scanned_field;

    int16_t itr_selectbox_items;

    int16_t x_start;  // DNE in Dasm
    int16_t y_start;  // DNE in Dasm
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Selection_Box_Draw()\n", __FILE__, __LINE__);
#endif

    scanned_field = Scan_Input();
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: scanned_field: %d\n", __FILE__, __LINE__, scanned_field);
#endif

    if( (scanned_field > GUI_ListSel_Count) || (scanned_field > selectbox_items) )
    {
        scanned_field = 0;
    }

    if(scanned_field > 0)
    {
        selectbox_highlight_item = scanned_field - 1;
    }


    Copy_Back_To_Off();

    /*
        BEGIN: Draw Selection Box
    */
    Set_Window(selectbox_x1, selectbox_y1, selectbox_x2, selectbox_y2);
    Clipped_Draw(selectbox_x1, selectbox_y1, selectbd_left_seg);
    Clipped_Draw((selectbox_x1 + FLIC_Get_Width(selectbd_left_seg)), selectbox_y1, selectbd_top_seg);


    /*
        BEGIN: Draw Button Background
    */
    Set_Window(selectbox_x1, selectbox_y1, (selectbox_x2 - 5), selectbox_y2);
    if(selectbox_multi == ST_FALSE)
    {
        DLOG("(selectbox_multi == ST_FALSE)");
        itr_selectbox_items = 0;
        while((selectbox_items - 1) >= itr_selectbox_items)
        {
            if(itr_selectbox_items != selectbox_highlight_item)
            {
                FLIC_Set_CurrentFrame(selectbk_button_12to16_seg[itr_selectbox_items], 0);
                FLIC_Set_CurrentFrame(selectbk_button_22to26_seg[itr_selectbox_items], 0);
            }
            else
            {
                FLIC_Set_CurrentFrame(selectbk_button_12to16_seg[itr_selectbox_items], 1);
                FLIC_Set_CurrentFrame(selectbk_button_22to26_seg[itr_selectbox_items], 1);
            }

            y_start = (selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * itr_selectbox_items));
            x_start = (selectbox_x1 + FLIC_Get_Width(selectbd_left_seg));
            Clipped_Draw(x_start, y_start, selectbk_button_12to16_seg[itr_selectbox_items]);

            y_start = (selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * itr_selectbox_items));
            x_start = (selectbox_x2 - FLIC_Get_Width(selectbd_right_seg) + 1);
            Clipped_Draw(x_start, y_start, selectbk_button_22to26_seg[itr_selectbox_items]);

            itr_selectbox_items++;
        }

        if((selectbox_items - 1) != selectbox_highlight_item)
        {
            FLIC_Set_CurrentFrame(selectbk_button_12to16_seg[9], 0);
            FLIC_Set_CurrentFrame(selectbk_button_22to26_seg[9], 0);
        }
        else
        {
            FLIC_Set_CurrentFrame(selectbk_button_12to16_seg[9], 1);
            FLIC_Set_CurrentFrame(selectbk_button_22to26_seg[9], 1);
        }

        y_start = (selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * (selectbox_items - 1)));
        x_start = (selectbox_x1 + FLIC_Get_Width(selectbd_left_seg));
        Clipped_Draw(x_start, y_start, selectbk_button_12to16_seg[9]);

        y_start = (selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * (selectbox_items - 1)));
        x_start = (selectbox_x2 - FLIC_Get_Width(selectbd_right_seg) + 1);
        Clipped_Draw(x_start, y_start, selectbk_button_22to26_seg[9]);
    }
    else
    {
        DLOG("(selectbox_multi != ST_FALSE)");
        itr_selectbox_items = 0;
        while((selectbox_items - 1) >= itr_selectbox_items)
        {
            if(itr_selectbox_items != selectbox_highlight_item)
            {
                FLIC_Set_CurrentFrame(selectbk_button_12to16_seg[itr_selectbox_items], 0);
                FLIC_Set_CurrentFrame(selectbk_button_22to26_seg[itr_selectbox_items], 0);
            }
            else
            {
                FLIC_Set_CurrentFrame(selectbk_button_12to16_seg[itr_selectbox_items], 1);
                FLIC_Set_CurrentFrame(selectbk_button_22to26_seg[itr_selectbox_items], 1);
            }

            y_start = (selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * itr_selectbox_items));
            x_start = (selectbox_x1 + FLIC_Get_Width(selectbd_left_seg) + FLIC_Get_Width(selectbd_scroll_seg));
            Clipped_Draw(x_start, y_start, selectbk_button_12to16_seg[itr_selectbox_items]);

            y_start = (selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * itr_selectbox_items));
            x_start = (selectbox_x2 - FLIC_Get_Width(selectbd_right_seg) + FLIC_Get_Width(selectbd_scroll_seg) + 1);
            Clipped_Draw(x_start, y_start, selectbk_button_22to26_seg[itr_selectbox_items]);

            itr_selectbox_items++;
        }

        
        if((selectbox_items - 1) != selectbox_highlight_item)
        {
            FLIC_Set_CurrentFrame(selectbk_button_12to16_seg[9], 0);
            FLIC_Set_CurrentFrame(selectbk_button_22to26_seg[9], 0);
        }
        else
        {
            FLIC_Set_CurrentFrame(selectbk_button_12to16_seg[9], 1);
            FLIC_Set_CurrentFrame(selectbk_button_22to26_seg[9], 1);
        }

        y_start = (selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * (selectbox_items - 1)));
        x_start = (selectbox_x1 + FLIC_Get_Width(selectbd_left_seg) + FLIC_Get_Width(selectbd_scroll_seg));
        Clipped_Draw(x_start, y_start, selectbk_button_12to16_seg[9]);

        y_start = (selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * (selectbox_items - 1)));
        x_start = (selectbox_x2 - FLIC_Get_Width(selectbd_right_seg) + FLIC_Get_Width(selectbd_scroll_seg) + 1);
        Clipped_Draw(x_start, y_start, selectbk_button_22to26_seg[9]);
    }
    /*
        END: Draw Button Background
    */


    /*
        Finish Selection Box Borders
            bottom
            bottom left
            right
            bottom right
    */
    Set_Window(selectbox_x1, selectbox_y1, selectbox_x2, selectbox_y2);
    Clipped_Draw((selectbox_x1 + FLIC_Get_Width(selectbd_left_seg)), (selectbox_y2 - FLIC_Get_Height(selectbd_bottom_seg) + 1), selectbd_bottom_seg);
    Clipped_Draw(selectbox_x1, (selectbox_y2 - FLIC_Get_Height(selectbd_bottom_left_seg) + 1), selectbd_bottom_left_seg);
    Clipped_Draw((selectbox_x2 - FLIC_Get_Width(selectbd_right_seg) + 1), selectbox_y1, selectbd_right_seg);
    Clipped_Draw((selectbox_x2 - FLIC_Get_Width(selectbd_bottom_right_seg) + 1), (selectbox_y2 - FLIC_Get_Height(selectbd_bottom_right_seg) + 1), selectbd_bottom_right_seg);
    /*
        END: Draw Selecttion Box
    */



    /*
        BEGIN: Draw Button Text
    */
    Set_Font_Colors_15(4, &COL_ListSel_Titles[0]);
    Set_Alias_Color(251);
    Print_Centered((((selectbox_x1 + selectbox_x2) / 2) + 1), (selectbox_y1 + 5), selectbox_title);

    Set_Font_Colors_15(4, &COL_ListSel_Shadows[0]);
    Set_Alias_Color(20);
    Print_Centered(((selectbox_x1 + selectbox_x2) / 2), (selectbox_y1 + 4), selectbox_title);

    Reset_Window();

    Set_Font_Style1(4, 1, 0, 0);

    for(itr_selectbox_items = 0; (itr_selectbox_items < selectbox_items) && (itr_selectbox_items < GUI_ListSel_Count); itr_selectbox_items++)
    {
        if(itr_selectbox_items != selectbox_highlight_item)
        {
            Set_Outline_Color(182);
            Set_Alias_Color(53);
        }
        else
        {
            Set_Outline_Color(178);
            Set_Alias_Color(182);
        }

        if(selectbox_multi != ST_TRUE)
        {
            y_start = selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * itr_selectbox_items) + 2;
            x_start = selectbox_x1 + FLIC_Get_Width(selectbd_left_seg) + 4;
            Print(x_start, y_start, selectbox_list[selectbox_first_item + itr_selectbox_items]);
        }
        else
        {
            y_start = selectbox_y1 + FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * itr_selectbox_items) + 2;
            x_start = selectbox_x1 + FLIC_Get_Width(selectbd_left_seg) + FLIC_Get_Width(selectbd_scroll_seg) + 4;
            Print(x_start, y_start, selectbox_list[selectbox_first_item + itr_selectbox_items]);
        }
    }
    /*
        END: Draw Button Text
    */



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Selection_Box_Draw()\n", __FILE__, __LINE__);
#endif

}

// WZD o149p09
// void Selection_Box_Coords(int16_t item_count, char * list_text[], char * title_string)
void Selection_Box_Coords(int16_t item_count, char ** list_text, char * title_string)
{
    int16_t title_string_width;
    int16_t selectbox_height;
    int16_t selectbox_width;

    int16_t horizontal_space;
    int16_t itr_GUI_ListSel_Count;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Selection_Box_Coords(item_count = %d, list_text[0] = %s, title_string = %s)\n", __FILE__, __LINE__, item_count, list_text[0], title_string);
#endif

    GUI_ListSel_Count = 0;

    while(*list_text[GUI_ListSel_Count] != '\0')
    {
        GUI_ListSel_Count++;
    }


    Set_Font(4, 0, 0, 0);


    horizontal_space = 0;

    for(itr_GUI_ListSel_Count = 0; itr_GUI_ListSel_Count < GUI_ListSel_Count; itr_GUI_ListSel_Count++)
    {
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: list_text[itr_GUI_ListSel_Count]: %s\n", __FILE__, __LINE__, list_text[itr_GUI_ListSel_Count]);
    dbg_prn("DEBUG: [%s, %d]: Get_String_Width(list_text[itr_GUI_ListSel_Count]): %d\n", __FILE__, __LINE__, Get_String_Width(list_text[itr_GUI_ListSel_Count]));
#endif
        if(Get_String_Width(list_text[itr_GUI_ListSel_Count]) > horizontal_space)
        {
            horizontal_space = Get_String_Width(list_text[itr_GUI_ListSel_Count]);
        }
    }
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 689]: list_text[itr_GUI_ListSel_Count]: Surveyor	 (F1)
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 690]: Get_String_Width(list_text[itr_GUI_ListSel_Count]): 56
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 689]: list_text[itr_GUI_ListSel_Count]: Cartographer	 (F2)
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 690]: Get_String_Width(list_text[itr_GUI_ListSel_Count]): 78
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 689]: list_text[itr_GUI_ListSel_Count]: Apprentice	 (F3)
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 690]: Get_String_Width(list_text[itr_GUI_ListSel_Count]): 65
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 689]: list_text[itr_GUI_ListSel_Count]: Historian	 (F4)
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 690]: Get_String_Width(list_text[itr_GUI_ListSel_Count]): 57
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 689]: list_text[itr_GUI_ListSel_Count]: Astrologer	 (F5)
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 690]: Get_String_Width(list_text[itr_GUI_ListSel_Count]): 65
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 689]: list_text[itr_GUI_ListSel_Count]: Chancellor	 (F6)
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 690]: Get_String_Width(list_text[itr_GUI_ListSel_Count]): 63
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 689]: list_text[itr_GUI_ListSel_Count]: Tax Collector	 (F7)
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 690]: Get_String_Width(list_text[itr_GUI_ListSel_Count]): 76
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 689]: list_text[itr_GUI_ListSel_Count]: Grand Vizier	 (F8)
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 690]: Get_String_Width(list_text[itr_GUI_ListSel_Count]): 73
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 689]: list_text[itr_GUI_ListSel_Count]: Mirror	 (F9)
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 690]: Get_String_Width(list_text[itr_GUI_ListSel_Count]): 47

// DEBUG: [C:\STU\devel\ReMoM\src\Fonts.C, 852]: BEGIN: Set_Font(font_idx = 1, color1 = 0, color2 = 0, color3 = 0)
// DEBUG: [C:\STU\devel\ReMoM\src\Fonts.C, 907]: END: Set_Font(font_idx = 1, color1 = 0, color2 = 0, color3 = 0)


    if(selectbox_multi != ST_FALSE)
    {
        horizontal_space += FLIC_Get_Width(selectbd_scroll_seg);
    }

    title_string_width = Get_String_Width(title_string);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: title_string_width: %d\n", __FILE__, __LINE__, title_string_width);
#endif

    if(title_string_width > horizontal_space)
    {
        horizontal_space = title_string_width;
    }

    horizontal_space += 8;

    selectbox_width = FLIC_Get_Width(selectbd_left_seg) + horizontal_space + FLIC_Get_Width(selectbd_right_seg);
    selectbox_height = FLIC_Get_Height(selectbd_top_seg) + (FLIC_Get_Height(selectbk_button_12to16_seg[0]) * item_count) + FLIC_Get_Height(selectbd_bottom_seg);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: selectbox_width: %d\n", __FILE__, __LINE__, selectbox_width);
    dbg_prn("DEBUG: [%s, %d]: selectbox_height: %d\n", __FILE__, __LINE__, selectbox_height);
#endif

// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 709]: title_string_width: 75

// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 722]: selectbox_width: 120
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 723]: selectbox_height: 158


    selectbox_x1 = ((SCREEN_WIDTH - selectbox_width) / 2);
    selectbox_y1 = ((SCREEN_HEIGHT - selectbox_height) / 2);
    selectbox_x2 = selectbox_x1 + selectbox_width - 1;
    selectbox_y2 = selectbox_y1 + selectbox_height - 1;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: selectbox_x1: %d\n", __FILE__, __LINE__, selectbox_x1);
    dbg_prn("DEBUG: [%s, %d]: selectbox_y1: %d\n", __FILE__, __LINE__, selectbox_y1);
    dbg_prn("DEBUG: [%s, %d]: selectbox_x2: %d\n", __FILE__, __LINE__, selectbox_x2);
    dbg_prn("DEBUG: [%s, %d]: selectbox_y2: %d\n", __FILE__, __LINE__, selectbox_y2);
#endif
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 320]: selectbox_x1: 101
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 321]: selectbox_y1: 21
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 322]: selectbox_x2: 217
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 323]: selectbox_y2: 178
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 732]: selectbox_x1: 100
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 733]: selectbox_y1: 21
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 734]: selectbox_x2: 219
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 735]: selectbox_y2: 178
// ~ {86, 22, 233, 179}
// w: 147, h: 157
// DEBUG: [C:\STU\devel\ReMoM\src\GENDRAW.C, 709]: title_string_width: 75
// title x1 106 x2 213 w 107
// 17 characters  avg 6.29412 pixels per


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Selection_Box_Coords()\n", __FILE__, __LINE__);
#endif

}

// WZD o149p10
// GUI_LoadNotifyGfx   
// WZD o149p11
// GUI_Notify_Dialog   
// WZD o149p12
// GUI_DrawNotifyDialog
// WZD o149p13
// GUI_Familiar_Notify 
// WZD o149p14
// GUI_DrawFamiliarNtfy
// WZD o149p15
// HLP_NoResizeExpand  
// WZD o149p16
// HLP_DrawExpanding   
// WZD o149p17
// G_USW_DrawEHLines   
// WZD o149p18
// HLP_DrawExpandLines 
// WZD o149p19
// ITEM_GetViewDrawPos 
// WZD o149p20
// ITEM_ResetViewGrow  

// WZD o149p21
// drake178: EMM_Sandbox2VGAFILEH
void Save_ScreenSeg(void)
{
// ovr159
// call    j_EMM_Map_VGAFILEH1_5           ; maps in pages 1 - 5 of the VGAFILEH EMM handle ; returns the page frame segment
// mov     _SI_EMM_PageFrame, ax           ; EMM_PageFrame
// xor     ax, ax
// mov     dx, 32768
// push    ax
// push    dx                              ; count
// push    [_screen_seg]                   ; 203h + 11F8h LBX_Alloc_Space paragraphs
// xor     ax, ax
// push    ax                              ; src
// push    _SI_EMM_PageFrame
// xor     ax, ax
// push    ax                              ; dst
// call    _fmemcpy
// xor     ax, ax
// mov     dx, 32768
// push    ax
// push    dx                              ; count
// mov     ax, [_screen_seg]               ; 203h + 11F8h LBX_Alloc_Space paragraphs
// add     ax, 2048
// push    ax
// xor     ax, ax
// push    ax                              ; src
// mov     ax, _SI_EMM_PageFrame
// add     ax, 2048
// push    ax
// xor     ax, ax
// push    ax                              ; dst
// call    _fmemcpy

    memcpy(_VGAFILEH_seg, _screen_seg, 32768);
    memcpy(_VGAFILEH_seg+32768, _screen_seg+32768, 32768);

}

// WZD o149p22
// drake178: EMM_VGAFILEH2Sandbox
void Restore_ScreenSeg(void)
{
// ovr159
// call    j_EMM_Map_VGAFILEH1_5           ; maps in pages 1 - 5 of the VGAFILEH EMM handle ; returns the page frame segment
// mov     _SI_EMM_PageFrame, ax           ; EMM_PageFrame
// xor     ax, ax
// mov     dx, 32768
// push    ax
// push    dx                              ; count
// push    _SI_EMM_PageFrame
// xor     ax, ax
// push    ax                              ; src
// push    [_screen_seg]                   ; 203h + 11F8h LBX_Alloc_Space paragraphs
// xor     ax, ax
// push    ax                              ; dst
// call    _fmemcpy                        ; _fmemcpy(0, _screen_seg, 0, EMM_PageFrame, 32768, 0)
// xor     ax, ax
// mov     dx, 32768
// push    ax
// push    dx                              ; count
// mov     ax, _SI_EMM_PageFrame
// add     ax, 2048
// push    ax
// xor     ax, ax
// push    ax                              ; src
// mov     ax, [_screen_seg]               ; 203h + 11F8h LBX_Alloc_Space paragraphs
// add     ax, 2048
// push    ax
// xor     ax, ax
// push    ax                              ; dst
// call    _fmemcpy                        ; _fmemcpy(0, _screen_seg+32768, 0, EMM_PageFrame+32768, 32768, 0)

    memcpy(_screen_seg, _VGAFILEH_seg, 32768);
    memcpy(_screen_seg+32768, _VGAFILEH_seg+32768, 32768);

}
