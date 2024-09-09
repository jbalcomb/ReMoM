/*
    WIZARDS.EXE
        ovr149

    MoO2
        GENDRAW
        TEXTBOX
        Message_Box_() |-> Do_Text_Box()

Confirm
Select
Warn
Notify
...Box

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

// WZD dseg:6F8C
// drake178: COL_Warning1
uint8_t COL_Warning1[5] = {228, 228, 228, 228, 228};

// WZD dseg:6F91
// drake178: COL_Warning2
uint8_t COL_Warning2[5] = {45, 45, 45, 45, 45};

// WZD dseg:6F96
// drake178: RP_GUI_Confirm_XShift
int16_t confirmation_box_x_offset = 0;

// WZD dseg:6F98
// drake178: RP_GUI_Confirm_YShift
int16_t confirmation_box_y_offset = 0;

// WZD dseg:6F9A
// drake178: GUI_Notify_Compose
int16_t notify_draw_offscr = ST_FALSE;

// WZD dseg:6F9C
// drake178: COL_Notify_Shadows
uint8_t notify_shadow_colors[4][5] =
{
    {240, 240, 240, 240, 240},
    {240, 240, 240, 240, 240},
    {240, 240, 240, 240, 240}, 
    { 20,  20,  20,  20,  20}
};

// WZD dseg:6FB0
// drake178: COL_Notify_Text
uint8_t notify_text_colors[4][5] = 
{
    { 25, 178, 179, 180, 180},
    { 25, 178, 179, 180, 180},
    { 25, 178, 179, 180, 180},
    { 25, 178, 179, 180, 180}
};

// WZD dseg:6FC4
// drake178: GUI_Familiar_IMGLeft
// WZD dseg:6FCE
// drake178: GUI_Familiar_IMGTop
// WZD dseg:6FD8
// drake178: GUI_Familiar_IMGText
int16_t notify2_mascot_x1[5] = { -2,   0, -22,   8,  2};
int16_t notify2_mascot_y1[5]  = {  2, -20,  -4, -12, -3};
int16_t notify2_text_x1[5] = { 70,  73,  62,  68, 70};

// WZD dseg:6FE2 01 00                                           
int16_t m_item_view_grow_stage = 1;


/*
    BEGIN:
        Confirmation Box
        Selection Box
*/
// WZD dseg:6FE4
char resource_lbx_file[] = "RESOURCE";
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





// WZD dseg:9408
// drake178: IMG_GUI_Familiars@
SAMB_ptr notify_magic_picts_seg[6];






// WZD dseg:CA8E                                                 BEGIN:  ovr149  GENDRAW - Uninitialized Data

// WZD dseg:CA8E
int16_t _draw_expanding_box;
// WZD dseg:CA90
int16_t HLP_ExpandOrigBottom;
// WZD dseg:CA92
int16_t HLP_ExpandOrigRight;
// WZD dseg:CA94
int16_t HLP_ExpandOrigTop;
// WZD dseg:CA96
int16_t HLP_ExpandOrigLeft;
// WZD dseg:CA98
int16_t HLP_ExpandTgtBottom;
// WZD dseg:CA9A
int16_t HLP_ExpandTgtRight;
// WZD dseg:CA9C
int16_t HLP_ExpandTgtTop;
// WZD dseg:CA9E
int16_t HLP_ExpandTgtLeft;

// WZD dseg:CAA0                                                 BEGIN:  Selection Box
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
// WZD dseg:CAA0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+selectbox_fields dw 0Ah dup(0)          ; DATA XREF: Selection_Box+217w ...
// WZD dseg:CAB4 00 00                                           selectbox_title dw 0                    ; DATA XREF: Selection_Box+1CBw ...
// WZD dseg:CAB6 00 00                                           selectbox_list dw 0                     ; DATA XREF: Selection_Box+1C5w ...
// WZD dseg:CAB8 00 00                                           selectbox_items dw 0                    ; DATA XREF: Selection_Box+1BFw ...
// WZD dseg:CABA 00 00                                           selectbox_first_item dw 0               ; DATA XREF: Selection_Box+1CEw ...
// WZD dseg:CABC 00 00                                           selectbox_highlight_item dw 0           ; DATA XREF: Selection_Box+1D4w ...
// WZD dseg:CABE 00 00                                           UU_selectbox dw 0
// WZD dseg:CAC0 00 00                                           selectbox_y2 dw 0                       ; DATA XREF: Selection_Box+441r ...
// WZD dseg:CAC2 00 00                                           selectbox_x2 dw 0                       ; DATA XREF: Selection_Box+2EFr ...
// WZD dseg:CAC4 00 00                                           selectbox_y1 dw 0                       ; DATA XREF: Selection_Box+2DAr ...
// WZD dseg:CAC6 00 00                                           selectbox_x1 dw 0                       ; DATA XREF: Selection_Box+321r ...
// WZD dseg:CAC8 00 00                                           selectbox_multi dw 0                    ; DATA XREF: Selection_Box+1B9w ...
// WZD dseg:CACA 00 00                                           GUI_ListSel_Count dw 0                  ; DATA XREF: Selection_Box+264r ...
// WZD dseg:CACA                                                 END:  Selection Box

// WZD dseg:CACA                                                 END:  Selection Box

// WZD dseg:CACC                                                 BEGIN:  Confirmation Box
// WZD dseg:CACC
SAMB_ptr confirmation_button_no_seg;            // ; 2 frame image appended in the LBX_Sandbox_Segment
// WZD dseg:CACE
SAMB_ptr confirmation_button_yes_seg;           // ; 2 frame image appended in the LBX_Sandbox_Segment
// WZD dseg:CAD0
SAMB_ptr confirmation_background_bottom_seg;    // ; single image appended in the LBX_Sandbox_Segment
// WZD dseg:CAD2
SAMB_ptr confirmation_background_top_seg;       // ; LBXE_LoadReplace into the LBX_Sandbox_Segment
// WZD dseg:CAD2                                                 END:  Confirmation Box

// WZD dseg:CAD4                                                 BEGIN: Notify
// WZD dseg:CAD4
int16_t notify_pict2_x;
// WZD dseg:CAD6
int16_t notify_pict2_y;
// WZD dseg:CAD8
int16_t notify_pict1_y;
// WZD dseg:CADA
int16_t notify_pict1_x;
// WZD dseg:CADC
int16_t notify_box_border;
// WZD dseg:CADE
SAMB_ptr notify_pict2;
// WZD dseg:CAE0
SAMB_ptr notify_pict1;
// WZD dseg:CAE2
int16_t notify_type;
// WZD dseg:CAE4
// UU_notify
// WZD dseg:CAE6
SAMB_ptr notify_gem_box_seg[4];
// WZD dseg:CAEE
SAMB_ptr notify_pict_box_seg[4];
// WZD dseg:CAF6
SAMB_ptr notify_seg[4];
// WZD dseg:CAFE
int16_t notify_color_slide;
// WZD dseg:CB00
int16_t notify_color_slide_cycle;  // {0, ..., 7}

// WZD dseg:CB02
SAMB_ptr IMG_GUI_RedMsg2Btm;
// WZD dseg:CB04
SAMB_ptr IMG_GUI_RedMessage2;
// WZD dseg:CB06
SAMB_ptr IMG_GUI_RedMsg1Btm;
// WZD dseg:CB08
SAMB_ptr IMG_GUI_RedMessage1;

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
SAMB_ptr selectbk_button_22to26_seg[10];
// WZD dseg:CB2A
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

// WZD dseg:CB4C
int16_t notify_magic_idx;

// WZD dseg:CB4E
// AKA confirmation_box_text
char * message_box_text;
// WZD dseg:CB50
// AKA confirmation_box_y
int16_t message_box_y;
// WZD dseg:CB52
// AKA confirmation_box_x
int16_t message_box_x;

// WZD dseg:CB54
// ; determines whether to use the first or the second of the two different warning message backgrounds (both of which are red, but slightly different and use a different font color)
int16_t GUI_RedMsg_Type;

// WZD dseg:CB54                                                 END:  ovr149  GENDRAW - Uninitialized Data ; 





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
    confirmation_background_top_seg = LBX_Reload(resource_lbx_file, 0, _screen_seg);
    // RESOURCE.LBX, 1  "CONFMBAK", ""
    confirmation_background_bottom_seg = LBX_Reload_Next(resource_lbx_file, 1, _screen_seg);
    // RESOURCE.LBX, 3  CONFMBUT    yes
    confirmation_button_yes_seg = LBX_Reload_Next(resource_lbx_file, 3, _screen_seg);
    // RESOURCE.LBX, 4  CONFMBUT    no
    confirmation_button_no_seg = LBX_Reload_Next(resource_lbx_file, 4, _screen_seg);

    message_box_text = text_string;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: message_box_text: %s)\n", __FILE__, __LINE__, message_box_text);
#endif


    Set_Font_Style(4, 4, 15, 0);


    paragraph_height = Get_Paragraph_Max_Height(166, text_string);

    Box_Height = paragraph_height + 34;
    message_box_x = 68 + confirmation_box_x_offset;
    message_box_y = ((SCREEN_HEIGHT - Box_Height) / 2) + confirmation_box_y_offset;

    Clear_Fields();

    // confirmation_button_yes = Add_Button_Field((message_box_x + 101), (message_box_y + paragraph_height + 15), cnst_ZeroString_5, confirmation_button_yes_seg, cnst_HOTKEY_Y_3, -1);
    confirmation_button_yes = Add_Button_Field((message_box_x + 101), (message_box_y + paragraph_height + 15), cnst_ZeroString_5, confirmation_button_yes_seg, 'Y', -1);

    // confirmation_button_no = Add_Button_Field((message_box_x + 18), (message_box_y + paragraph_height + 15), cnst_ZeroString_5, confirmation_button_no_seg, cnst_HOTKEY_N_5, -1);
    confirmation_button_no = Add_Button_Field((message_box_x + 18), (message_box_y + paragraph_height + 15), cnst_ZeroString_5, confirmation_button_no_seg, 'N', -1);

    // Text_Label_Index = Add_Hidden_Field(message_box_x, message_box_y, (message_box_x + 185), (message_box_y + 63), cnst_ZeroString_5, -1);
    Text_Label_Index = Add_Hidden_Field(message_box_x, message_box_y, (message_box_x + 185), (message_box_y + 63), 0, -1);

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

    Set_Font_Style(4, 4, 15, ST_NULL);

    paragraph_height = Get_Paragraph_Max_Height(166, message_box_text);

    Set_Page_Off();

    Set_Window(0, 0, SCREEN_XMAX, (message_box_y + paragraph_height + 12));

    Clipped_Draw(message_box_x, message_box_y, confirmation_background_top_seg);

    Reset_Window();

    FLIC_Draw(message_box_x, (message_box_y + paragraph_height + 10), confirmation_background_bottom_seg);

    Set_Font_Colors_15(4, &COL_ConfirmShadows[0]);
    Set_Font_Style(4, 15, 15, ST_NULL);
    Print_Paragraph((message_box_x + 10), (message_box_y + 10), 166, message_box_text, 0);  // print_type 0: Print Left Aligned
    Print_Paragraph((message_box_x +  9), (message_box_y + 10), 166, message_box_text, 0);  // print_type 0: Print Left Aligned

    Set_Alias_Color(18);
    Set_Font_Colors_15(4, &COL_Dialog_Text[0]);
    Set_Font_Style(4, 4, 15, ST_NULL);
    Print_Paragraph((message_box_x +  9), (message_box_y +  9), 166, message_box_text, 0);  // print_type 0: Print Left Aligned
}


// WZD o149p03
// drake178: GUI_WarningType1()
void Warn1(char * msg)
{
    GUI_RedMsg_Type = 1;
    Warn(msg);
}

// WZD o149p04
// drake178: GUI_WarningType0()
void Warn0(char * msg)
{
    GUI_RedMsg_Type = 0;
    Warn(msg);
}

// WZD o149p05
// drake178: GUI_ShowRedMessage
void Warn(char * msg)
{
    int16_t UU_var3;
    int16_t textbox_height;
    int16_t max_para_height;

        Save_ScreenSeg();

        // RESOURCE.LBX,  38  WARNBACK    warning top
        // RESOURCE.LBX,  39  WARNBACK    warning bottom
        // RESOURCE.LBX,  49  WARNBCK2    warning top
        // RESOURCE.LBX,  50  WARNBCK2    warning bottom

        IMG_GUI_RedMessage1  = LBX_Reload(resource_lbx_file, 38, _screen_seg);
        IMG_GUI_RedMsg1Btm   = LBX_Reload_Next(resource_lbx_file, 39, _screen_seg);
        IMG_GUI_RedMessage2  = LBX_Reload_Next(resource_lbx_file, 49, _screen_seg);
        IMG_GUI_RedMsg2Btm   = LBX_Reload_Next(resource_lbx_file, 50, _screen_seg);

        Save_Auto_Function();

        Save_Alias_Colors();

        Set_Font_Colors_15(0, COL_Dialog_Text);

        Copy_On_To_Off_Page();

        message_box_text = msg;

        Set_Font_Style(4, 4, 4, ST_NULL);

        max_para_height = Get_Paragraph_Max_Height(166, msg);

        textbox_height = max_para_height + 33;

        message_box_x = 68;

        message_box_y = ((200 - textbox_height) / 2);

        Assign_Auto_Function(&Warn_Draw, 1);

        UU_var3 = 0;

        // TODO  SND_PlayClickSound();

        Wait_For_Input();

        Restore_Alias_Colors();
        Reset_Window();
        Restore_Auto_Function();
        Restore_ScreenSeg();
}


// WZD o149p06
// drake178: GUI_DrawRedMessage()
void Warn_Draw(void)
{
    int16_t max_para_height;  // _SI_

    Set_Font_Style(4, 4, 4, ST_NULL);

    max_para_height = Get_Paragraph_Max_Height(166, message_box_text);

    Set_Page_Off();

    Set_Window(0, 0, SCREEN_XMAX, (message_box_y + max_para_height + 12));

    if(GUI_RedMsg_Type == 0)
    {
        Clipped_Draw(message_box_x, message_box_y, IMG_GUI_RedMessage1);
    }
    else
    {
        Clipped_Draw(message_box_x, message_box_y, IMG_GUI_RedMessage2);
    }

    Reset_Window();

    if(GUI_RedMsg_Type == 0)
    {
        Set_Font_Colors_15(4, COL_Warning1);
    }
    else
    {
        Set_Font_Colors_15(4, COL_Warning2);
    }

    Set_Font_Style(4, 15, 15, ST_NULL);

    Print_Paragraph((message_box_x + 11), (message_box_y + 11), 166, message_box_text, 2);
    Print_Paragraph((message_box_x + 10), (message_box_y + 11), 166, message_box_text, 2);

    Set_Alias_Color(184);
    Set_Font_Style(4, 4, 4, ST_NULL);

    Print_Paragraph((message_box_x + 10), (message_box_y + 10), 166, message_box_text, 2);

    if(GUI_RedMsg_Type == 0)
    {
        FLIC_Draw(message_box_x, (message_box_y + max_para_height + 10), IMG_GUI_RedMsg1Btm);
    }
    else
    {
        FLIC_Draw(message_box_x, (message_box_y + max_para_height + 10), IMG_GUI_RedMsg2Btm);
    }

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
/*
        // ; displays a list selection dialog with the specified
        // ; title and items, with a separate button corresponding
        // ; to each one, that can be highlighted by mousing over
        // ; it or selected by clicking
        // ; returns the index of the selected item
        // ; BUG: uses two graphical button controls for
        // ; multi-page lists that are no longer in memory when
        // ; returning - doing a GUI page flip before clearing
        // ; them will cause a crash
        // ; WARNING: the drawing function expects the list of
        // ; items to be terminated by a zero string
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


    selectbd_left_seg = LBX_Reload(resource_lbx_file, 5, _screen_seg);

    selectbd_bottom_left_seg         = LBX_Reload_Next(resource_lbx_file,  6, _screen_seg);
    selectbd_top_seg                 = LBX_Reload_Next(resource_lbx_file,  7, _screen_seg);
    selectbd_bottom_seg              = LBX_Reload_Next(resource_lbx_file,  9, _screen_seg);
    selectbd_right_seg               = LBX_Reload_Next(resource_lbx_file,  8, _screen_seg);
    selectbd_bottom_right_seg        = LBX_Reload_Next(resource_lbx_file, 10, _screen_seg);
    selectbd_scroll_seg              = LBX_Reload_Next(resource_lbx_file, 11, _screen_seg);

    selectbk_top_arrow_seg           = LBX_Reload_Next(resource_lbx_file, 32, _screen_seg);
    selectbk_bottom_arrow_seg        = LBX_Reload_Next(resource_lbx_file, 33, _screen_seg);
    selectbk_scroll_top_seg          = LBX_Reload_Next(resource_lbx_file, 34, _screen_seg);
    selectbk_scroll_bottom_seg       = LBX_Reload_Next(resource_lbx_file, 35, _screen_seg);
    selectbk_scroll_locked_up_seg    = LBX_Reload_Next(resource_lbx_file, 36, _screen_seg);
    selectbk_scroll_locked_down_seg  = LBX_Reload_Next(resource_lbx_file, 37, _screen_seg);

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
        selectbk_button_12to16_seg[itr] = LBX_Reload_Next(resource_lbx_file, (itr + 12), _screen_seg);

        selectbk_button_22to26_seg[itr] = LBX_Reload_Next(resource_lbx_file, (itr + 22), _screen_seg);
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

    Set_Font_Style_Shadow_Down(4, 1, 0, 0);

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


    Set_Font_Style(4, 0, 0, 0);


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
// drake178: GUI_LoadNotifyGfx()
void Notify_Load(int16_t type, SAMB_ptr pict1)
{
    int16_t itr;  // _SI_

    Save_ScreenSeg();

// IDGI    if( !((_screen_seg + 1500) < pict1) )
// IDGI    {
// IDGI        farpokew(_screen_seg, SAMB.used, 1500);
// IDGI    }
// IDGI    else
// IDGI    {
// IDGI        farpokew(_screen_seg, SAMB.used, 1);
// IDGI    }

// RESOURCE.LBX,  40  G_NOTIFY    green notify
// RESOURCE.LBX,  41  G_NOTIFY    
// RESOURCE.LBX,  42  G_NOTIFY    
// RESOURCE.LBX,  43  F_NOTIFY    red notify
// RESOURCE.LBX,  44  F_NOTIFY    pict box
// RESOURCE.LBX,  45  F_NOTIFY    gem box
// RESOURCE.LBX,  46  B_NOTIFY    green notify
// RESOURCE.LBX,  47  B_NOTIFY    pict box
// RESOURCE.LBX,  48  B_NOTIFY    gem box
// RESOURCE.LBX,  56  BRNOTIFY    
// RESOURCE.LBX,  57  BRNOTIFY    
// RESOURCE.LBX,  58  BRNOTIFY    

    switch(type)
    {
        case 0:
        {
            notify_seg[0]          = LBX_Reload_Next(resource_lbx_file, 46, _screen_seg);
            notify_pict_box_seg[0] = LBX_Reload_Next(resource_lbx_file, 47, _screen_seg);
            notify_gem_box_seg[0]  = LBX_Reload_Next(resource_lbx_file, 48, _screen_seg);
        } break;
        case 1:
        {
            notify_seg[1]          = LBX_Reload_Next(resource_lbx_file, 43, _screen_seg);
            notify_pict_box_seg[1] = LBX_Reload_Next(resource_lbx_file, 44, _screen_seg);
            notify_gem_box_seg[1]  = LBX_Reload_Next(resource_lbx_file, 45, _screen_seg);
        } break;
        case 2:
        {
            notify_seg[2]          = LBX_Reload_Next(resource_lbx_file, 40, _screen_seg);
            notify_pict_box_seg[2] = LBX_Reload_Next(resource_lbx_file, 41, _screen_seg);
            notify_gem_box_seg[2]  = LBX_Reload_Next(resource_lbx_file, 42, _screen_seg);
        } break;
        case 3:
        {
            notify_seg[3]          = LBX_Reload_Next(resource_lbx_file, 56, _screen_seg);
            notify_pict_box_seg[3] = LBX_Reload_Next(resource_lbx_file, 57, _screen_seg);
            notify_gem_box_seg[3]  = LBX_Reload_Next(resource_lbx_file, 58, _screen_seg);
        } break;
    }

    // RESOURCE.LBX,  51  CHAOS       
    // RESOURCE.LBX,  52  DEATH       
    // RESOURCE.LBX,  53  LIFE        
    // RESOURCE.LBX,  54  NATURE      
    // RESOURCE.LBX,  55  SORCERY     

    for(itr = 0; itr < 5; itr++)
    {
        notify_magic_picts_seg[itr] = LBX_Reload_Next(resource_lbx_file, (51 + itr), _screen_seg);
    }

}


// WZD o149p11
// GUI_Notify_Dialog   
// WZD o149p12
// GUI_DrawNotifyDialog

// WZD o149p11
// drake178: GUI_Notify_Dialog
/*
    ¿ Notify1() vs. Notify2() ?


; the original version of GUI_Familiar_Notify that uses
; the original redraw function that only shows the
; familiar with a specific notification background and
; would use both the Center and BGShade parameters; it
; is now only used for treasure, where two out of those
; don't make a difference because they are not used
; Attributes: bp-based frame

; int __cdecl __far Notify1(int box_center_x, int box_top_y, int notify_color, char *message_text, int box_border, unsigned int notify_pict_seg, int pict1_x, int pict1_y, unsigned int pict2, int pict2_x, int pict2_y, int do_cslide, int do_dim_back)

e.g.,
Lair_Treasure_Popup()
    |-> Notify1(190, window_y, 3, lair_message_box_text, 0, Treasure_IMG, 2, 8, 0, 0, 0, 2, 0); 

¿ Type 3 ?

*/
void Notify1(int16_t box_center_x, int16_t box_top_y, int16_t type, char * message_text, int16_t box_border, SAMB_ptr pict1, int16_t pict1_x, int16_t pict1_y, SAMB_ptr pict2, int16_t pict2_x, int16_t pict2_y, int16_t do_cslide, int16_t do_dim_back)
{
    int16_t UU_var2;
    int16_t max_para_width;

    Notify_Load(type, pict1);

    notify_magic_idx = 3;  // Life

    if(_players[HUMAN_PLAYER_IDX].spellranks[notify_magic_idx] < _players[HUMAN_PLAYER_IDX].spellranks[2])  /* Chaos */
    {
        notify_magic_idx = 2;  // Chaos
    }

    if(_players[HUMAN_PLAYER_IDX].spellranks[notify_magic_idx] < _players[HUMAN_PLAYER_IDX].spellranks[4])  /* Death */
    {
        notify_magic_idx = 4;  // Death
    }

    if(_players[HUMAN_PLAYER_IDX].spellranks[notify_magic_idx] < _players[HUMAN_PLAYER_IDX].spellranks[0])  /* Nature */
    {
        notify_magic_idx = 0;  // Nature
    }

    if(_players[HUMAN_PLAYER_IDX].spellranks[notify_magic_idx] < _players[HUMAN_PLAYER_IDX].spellranks[1])  /* Sorcery */
    {
        notify_magic_idx = 1;  // Sorcery
    }


    switch(notify_magic_idx)
    {
        case 0:  /* Nature */
        {
            notify_magic_idx = mr_Nature;
        } break;
        case 1:  /* Sorcery */
        {
            notify_magic_idx = mr_Sorcery;
        } break;
        case 2:  /* Chaos */
        {
            notify_magic_idx = mr_Chaos;
        } break;
        case 3:  /* Life */
        {
            notify_magic_idx = mr_Life;
        } break;
        case 4:  /* Death */
        {
            notify_magic_idx = mr_Death;
        } break;
    }


    notify_color_slide = do_cslide;
    notify_color_slide_cycle = 0;

    Save_Alias_Colors();

    if(notify_draw_offscr == 2)
    {
        Copy_On_To_Off_Page();
    }

    if(do_dim_back == ST_TRUE)
    {
        Set_Page_Off();
        // DELETEME  Gradient_Fill(0, 0, SCREEN_XMAX, SCREEN_YMAX, 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ..., remap_block, Slope, Scale, Seed
        DARKEN_SCREEN();
    }

    Copy_Off_To_Back();

    message_box_text = message_text;

    notify_pict1 = pict1;

    notify_pict2 = pict2;

    notify_type = type;

    notify_box_border = box_border;

    notify_pict1_x = pict1_x;
    notify_pict1_y = pict1_y;
    notify_pict2_x = pict2_x;
    notify_pict2_y = pict2_y;

    notify_color_slide_cycle = 0;

    Set_Font_Style(4, 0, ST_NULL, ST_NULL);  // ...color2, color3

    max_para_width = Get_Paragraph_Max_Width(175, message_text, 0);

    // DIFF: Notify1 vs. Notify2
    message_box_x = (box_center_x - ((96 + max_para_width) / 2));
    // BEh  190d  ADh  173d  (190 - ((96 + 173) / 2)) = (190 - (269 / 2)) = (190 - 134) = 56a
    message_box_y = box_top_y;
    // 3Ch  60d
    Assign_Auto_Function(Notify1_Draw, 1);

    Load_Palette_From_Animation(notify_seg[notify_type]);

    if(pict1 != ST_NULL)
    {
        Load_Palette_From_Animation(pict1);
    }

    // Notify2() has Apply_Palette() first instead of last here
    // Notify2() has Set_Palette_Changes(0, 243);
    Set_Palette_Changes(224, 255);
    Clear_Palette_Changes(198, 198);
    Update_Remap_Gray_Palette();
    Apply_Palette();

    UU_var2 = 0;

    if(notify_draw_offscr == 1)
    {
        Set_Page_Off();
        Notify1_Draw();  // Notify2() also uses Notify1_Draw() here
    }
    else
    {
        Wait_For_Input();
    }

    Set_Palette_Changes(0, 223);
    Clear_Palette_Changes(198, 198);
    Update_Remap_Gray_Palette();
    Deactivate_Auto_Function();
    Restore_Alias_Colors();
    Reset_Window();
    Restore_ScreenSeg();
}


// WZD o149p12
// drake178: GUI_DrawNotifyDialog
void Notify1_Draw(void)
{
    int16_t xadd;  // _SI_
    int16_t max_para_width;  // _DI_

    if(notify_type == 2)
    {
        xadd = 50;
    }
    else
    {
        xadd = 0;
    }

    if(notify_color_slide == 1)
    {
        // TODO  VGA_SlideColors(244, 11, notify_color_slide_cycle);
        notify_color_slide_cycle = ((notify_color_slide_cycle + 1) % 11);
    }
    if(notify_color_slide == 2)
    {
        // TODO  VGA_SlideColors(247, 8, notify_color_slide_cycle);
        notify_color_slide_cycle = ((notify_color_slide_cycle + 1) % 8);
    }

    Copy_Back_To_Off();

    Set_Font_Style(4, 0, ST_NULL, ST_NULL);  // ...color2, color3

    max_para_width = Get_Paragraph_Max_Width(175, message_box_text, 0);

    Set_Page_Off();

    Set_Window(0, 0, (max_para_width + message_box_x + xadd + 12), SCREEN_YMAX);

    Clipped_Draw(message_box_x, message_box_y, notify_seg[notify_type]);

    if(notify_type == 2)
    {
        FLIC_Draw((message_box_x - 23), (message_box_y - 19), notify_magic_picts_seg[notify_magic_idx]);
    }

    Reset_Window();

    Fill((message_box_x + max_para_width + xadd + 12), message_box_y, (message_box_x + max_para_width + xadd + 62), (message_box_y + 55), 0);

    if(notify_box_border != 0)
    {
        FLIC_Draw((message_box_x + max_para_width + xadd + 12), message_box_y, notify_gem_box_seg[notify_type]);
    }
    else
    {
        FLIC_Draw((message_box_x + max_para_width + xadd + 12), message_box_y, notify_pict_box_seg[notify_type]);
    }

    if(notify_pict1 != ST_NULL)
    {
        Draw_Picture_To_Bitmap(notify_pict1, GfxBuf_2400B);
        Draw_Picture((message_box_x + max_para_width + notify_pict1_x + xadd + 16), (message_box_y + notify_pict1_y), GfxBuf_2400B);
    }

    if(notify_pict2 != ST_NULL)
    {
        Draw_Picture_To_Bitmap(notify_pict2, GfxBuf_2400B);
        Draw_Picture((message_box_x + max_para_width + notify_pict2_x + xadd + 16), (message_box_y + notify_pict2_y), GfxBuf_2400B);
    }

    if(notify_type == 3)
    {
        Set_Font_Colors_15(4, COL_ConfirmShadows);
        Set_Font_Style(4, 15, 15, ST_NULL);
        Print_Paragraph((message_box_x + xadd + 11), (message_box_y + 9), (max_para_width + 1), message_box_text, 0);
        Print_Paragraph((message_box_x + xadd + 10), (message_box_y + 9), (max_para_width + 1), message_box_text, 0);
        Set_Font_Style(4, 4, 15, ST_NULL);
        Set_Alias_Color(21);
        Print_Paragraph((message_box_x + xadd + 10), (message_box_y + 8), (max_para_width + 1), message_box_text, 0);
    }
    else
    {
        Set_Font_Colors_15(4, notify_shadow_colors[notify_type]);
        Set_Font_Style(4, 15, 15, ST_NULL);
        Print_Paragraph((message_box_x + xadd + 11), (message_box_y + 9), (max_para_width + 1), message_box_text, 0);
        Print_Paragraph((message_box_x + xadd + 10), (message_box_y + 9), (max_para_width + 1), message_box_text, 0);
        Set_Font_Style(4, 4, 15, ST_NULL);
        Set_Font_Colors_15(4, notify_text_colors[notify_type]);
        Print_Paragraph((message_box_x + xadd + 10), (message_box_y + 8), (max_para_width + 1), message_box_text, 0);
    }

    if(notify_color_slide == 1)
    {
        // VGA_SlideColors(244, 11, notify_color_slide_cycle);
        notify_color_slide_cycle = ((notify_color_slide_cycle + 1) % 11);
    }
    if(notify_color_slide == 2)
    {
        // VGA_SlideColors(247, 8, notify_color_slide_cycle);
        notify_color_slide_cycle = ((notify_color_slide_cycle + 1) % 8);
    }

}


// WZD o149p13
// drake178: GUI_Familiar_Notify()
/*

¿ mascot picture ?

Show_Event_Message()
    Notify2(160, 70, 2, m_event_message, 1, m_event_display_seg, 0, 8, 0, 0, 0, 1, 1);
        type == 2
        box_border == 1
        pict1 == m_event_display_seg
        pict1_x == 0
        pict1_y == 8
        pict2    == ST_NULL
        pict2_x  == 0
        pict2_y  == 0
        do_cslide   == ST_TRUE
        do_dim_back == ST_TRUE
*/
void Notify2(int16_t UU_box_center_x, int16_t box_top_y, int16_t type, char * message_text, int16_t box_border, SAMB_ptr pict1, int16_t pict1_x, int16_t pict1_y, SAMB_ptr pict2, int16_t pict2_x, int16_t pict2_y, int16_t do_cslide, int16_t do_dim_back)
{
    int16_t UU_var2;
    int16_t UU_max_para_width;

    Notify_Load(type, pict1);

    do_dim_back = ST_FALSE;

    notify_magic_idx = 3;  // Life

    if(_players[HUMAN_PLAYER_IDX].spellranks[notify_magic_idx] < _players[HUMAN_PLAYER_IDX].spellranks[2])  /* Chaos */
    {
        notify_magic_idx = 2;  // Chaos
    }

    if(_players[HUMAN_PLAYER_IDX].spellranks[notify_magic_idx] < _players[HUMAN_PLAYER_IDX].spellranks[4])  /* Death */
    {
        notify_magic_idx = 4;  // Death
    }

    if(_players[HUMAN_PLAYER_IDX].spellranks[notify_magic_idx] < _players[HUMAN_PLAYER_IDX].spellranks[0])  /* Nature */
    {
        notify_magic_idx = 0;  // Nature
    }

    if(_players[HUMAN_PLAYER_IDX].spellranks[notify_magic_idx] < _players[HUMAN_PLAYER_IDX].spellranks[1])  /* Sorcery */
    {
        notify_magic_idx = 1;  // Sorcery
    }

    switch(notify_magic_idx)
    {
        case 0:  /* Nature */
        {
            notify_magic_idx = mr_Nature;
        } break;
        case 1:  /* Sorcery */
        {
            notify_magic_idx = mr_Sorcery;
        } break;
        case 2:  /* Chaos */
        {
            notify_magic_idx = mr_Chaos;
        } break;
        case 3:  /* Life */
        {
            notify_magic_idx = mr_Life;
        } break;
        case 4:  /* Death */
        {
            notify_magic_idx = mr_Death;
        } break;
    }

    notify_color_slide = do_cslide;
    notify_color_slide_cycle = 0;

    Save_Alias_Colors();

    if(notify_draw_offscr == 2)
    {
        Copy_On_To_Off_Page();
    }

    if(do_dim_back == ST_TRUE)
    {
        Set_Page_Off();
        Gradient_Fill(0, 0, SCREEN_XMAX, SCREEN_YMAX, 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ..., remap_block, Slope, Scale, Seed
    }

    Copy_Off_To_Back();

    message_box_text = message_text;

    notify_pict1 = pict1;

    notify_pict2 = pict2;

    notify_type = type;

    notify_box_border = box_border;

    notify_pict1_x = pict1_x;
    notify_pict1_y = pict1_y;
    notify_pict2_x = pict2_x;
    notify_pict2_y = pict2_y;

    notify_color_slide_cycle = 0;

    Set_Font_Style(4, 0, ST_NULL, ST_NULL);  // ...color2, color3

    UU_max_para_width = Get_Paragraph_Max_Width(175, message_text, 0);

    if(notify2_mascot_x1[notify_magic_idx] < 0)
    {
        message_box_x = (6 - notify2_mascot_x1[notify_magic_idx]);  // { -2,   0, -22,   8,  2}
    }
    else
    {
        message_box_x = 6;
    }

    message_box_y = box_top_y;

    Assign_Auto_Function(Notify2_Draw, 1);

    Load_Palette_From_Animation(notify_seg[notify_type]);

    if(pict1 != ST_NULL)
    {
        Load_Palette_From_Animation(pict1);
    }

    Apply_Palette();
    Set_Palette_Changes(0, 243);
    Clear_Palette_Changes(198, 198);
    Update_Remap_Gray_Palette();

    UU_var2 = 0;

    if(notify_draw_offscr == 1)
    {
        Set_Page_Off();
        Notify1_Draw();  // BUGBUG  should be Notify2_Draw() - hot sloppy copy pasta
    }
    else
    {
        Wait_For_Input();
    }

    Set_Palette_Changes(0, 223);
    Clear_Palette_Changes(198, 198);
    Update_Remap_Gray_Palette();
    Deactivate_Auto_Function();
    Restore_Alias_Colors();
    Reset_Window();
    Restore_ScreenSeg();
}



// WZD o149p14
// drake178: GUI_DrawFamiliarNtfy()
/*

*/
void Notify2_Draw(void)
{
    int16_t text_x2;  // _SI_
    int16_t max_para_width;  // _DI_

    if(notify_color_slide == 1)
    {
        // TODO  VGA_SlideColors(244, 11, notify_color_slide_cycle);
        notify_color_slide_cycle = ((notify_color_slide_cycle + 1) % 11);
    }

    Copy_Back_To_Off();

    Set_Font_Style(4, 0, ST_NULL, ST_NULL);

    max_para_width = Get_Paragraph_Max_Width(175, message_box_text, 0);

    text_x2 = notify2_text_x1[notify_magic_idx] + message_box_x + max_para_width;  // pict box x1 = msg box start x + text x1 + text w

    Set_Page_Off();

    Set_Window(0, 0, (message_box_x + text_x2 + 12), SCREEN_YMAX);

    Clipped_Draw(message_box_x, message_box_y, notify_seg[notify_type]);

    FLIC_Draw((message_box_x + notify2_mascot_x1[notify_magic_idx] - 1), (message_box_y + notify2_mascot_y1[notify_magic_idx]), notify_magic_picts_seg[notify_magic_idx]);

    Reset_Window();


    /*
        BEGIN:  'Pict Box'
    */
    Fill((text_x2 + 2), message_box_y, (text_x2 + 47), (message_box_y + 55), 0);

    // pict border - {pict box, gem box}  F/T ~== 'type'
    if(notify_box_border != ST_FALSE)
    {
        FLIC_Draw((text_x2 + 1), message_box_y, notify_gem_box_seg[notify_type]);
    }
    else
    {
        FLIC_Draw((text_x2 + 1), message_box_y, notify_pict_box_seg[notify_type]);
    }

    if(notify_pict1 != ST_NULL)
    {
        Draw_Picture_To_Bitmap(notify_pict1, GfxBuf_2400B);
        Draw_Picture((text_x2 + notify_pict1_x + 7), (message_box_y + notify_pict1_y), GfxBuf_2400B);
    }

    if(notify_pict2 != ST_NULL)
    {
        Draw_Picture_To_Bitmap(notify_pict2, GfxBuf_2400B);
        Draw_Picture((notify_pict2_x + text_x2 + 7), (message_box_y + notify_pict2_y), GfxBuf_2400B);
    }
    /*
        END:  'Pict Box'
    */


    // draw outline/shadow
    Set_Font_Colors_15(4, notify_shadow_colors[notify_type]);
    Set_Font_Style(4, 15, 15, ST_NULL);
    Print_Paragraph((message_box_x + notify2_text_x1[notify_magic_idx] + 1), (message_box_y + 9), (max_para_width + 1), message_box_text, 0);
    Print_Paragraph((message_box_x + notify2_text_x1[notify_magic_idx]    ), (message_box_y + 9), (max_para_width + 1), message_box_text, 0);

    // draw text
    Set_Font_Style(4, 4, 15, ST_NULL);
    Set_Font_Colors_15(4, notify_text_colors[notify_type]);
    Print_Paragraph((message_box_x + notify2_text_x1[notify_magic_idx]    ), (message_box_y + 8), (max_para_width + 1), message_box_text, 0);


    if(notify_color_slide == 1)
    {
        // TODO  VGA_SlideColors(244, 11, notify_color_slide_cycle);
        notify_color_slide_cycle = ((notify_color_slide_cycle + 1) % 11);
    }

}


// WZD o149p15
/*
XREF:
    Mirror_Screen()
    Unit_List_Window_Pup()
    IDK_Spell_Cityscape_1()

Mirror_Screen()         expand_flag = ST_TRUE
Unit_List_Window_Pup()  expand_flag = ST_TRUE
IDK_Spell_Cityscape_1()  expand_flag = ST_FALSE


Mirror_Screen()
    j_HLP_NoResizeExpand(mirror_start_x, mirror_start_y, (mirror_start_x + 179), (mirror_start_y + 195), OL, OT, OR, OB, ST_TRUE)

Unit_List_Window_Pup()
    j_HLP_NoResizeExpand(ULW_start_x, ULW_start_y, (ULW_start_x + 238), (ULW_IDK_upper_height + 5), OL, OT, (OL + 19), (OT + 16), ST_TRUE)

IDK_Spell_Cityscape_1()
    j_HLP_NoResizeExpand(53, 38, 268, 162, OL, OT, (OL + 20), (OT + 18), ST_FALSE)

*/
void HLP_NoResizeExpand__TODO(int16_t TL, int16_t TT, int16_t TR, int16_t TB, int16_t OL, int16_t OT, int16_t OR, int16_t OB, int16_t expand_flag)
{
    HLP_DrawExpanding__TODO(TL, TT, TR, TB, TL, TT, TR, TB, OL, OT, OR, OB, expand_flag);
}

// WZD o149p16
/*
; if expanding help is enabled, "grows" an area of the
; current draw frame out of the current display frame
; using both EMM_VGAFILE_H and a sandbox buffer, and
; sets up the global variables used for redrawing the
; growth lines afterwards, if necessary

*/
void HLP_DrawExpanding__TODO(int16_t TL, int16_t TT, int16_t TR, int16_t TB, int16_t FL, int16_t FT, int16_t FR, int16_t FB, int16_t OL, int16_t OT, int16_t OR, int16_t OB, int16_t expand_flag)
{
    _draw_expanding_box = expand_flag;

    HLP_ExpandTgtLeft = TL;
    HLP_ExpandTgtTop = TT;
    HLP_ExpandTgtRight = TR;
    HLP_ExpandTgtBottom = TB;

    HLP_ExpandOrigLeft = OL;
    HLP_ExpandOrigTop = OT;
    HLP_ExpandOrigRight = OR;
    HLP_ExpandOrigBottom = OB;

    if(magic_set.Expanding_Help != ST_FALSE)
    {

        if(FL > FR)
        {
            Swap_Short(&FL, &FR);
        }

        if(FT > FB)
        {
            Swap_Short(&FT, &FB);
        }

        if(OL > OR)
        {
            Swap_Short(&OL, &OR);
        }

        if(OT > OB)
        {
            Swap_Short(&OT, &OB);
        }




    }


}


// WZD o149p17
/*
XREF:
    Unit_Statistics_Popup_Do_Draw()
    Mirror_Screen_Draw()
    Draw_Unit_List_Window_Pup()


C32 = 186
C31 = 187
C22 = 186
C21 = 185
C12 = 181
C11 = 180

*/
void G_USW_DrawEHLines__TODO(void)
{
    if(_draw_expanding_box)
    {
        Set_Page_Off();
        HLP_DrawExpandLines__TODO(HLP_ExpandOrigLeft, HLP_ExpandOrigTop, HLP_ExpandOrigRight, HLP_ExpandOrigBottom, HLP_ExpandTgtLeft, HLP_ExpandTgtTop, HLP_ExpandTgtRight, HLP_ExpandTgtBottom, 180, 181, 185, 186, 187, 186);
    }
}


// WZD o149p18
/*
XREF:
    NX_j_HLP_DrawExpandLines()
    HLP_DrawExpanding()
    G_USW_DrawEHLines()

G_USW_DrawEHLines()
    HLP_DrawExpandLines(HLP_ExpandOrigLeft, HLP_ExpandOrigTop, HLP_ExpandOrigRight, HLP_ExpandOrigBottom, HLP_ExpandTgtLeft, HLP_ExpandTgtTop, HLP_ExpandTgtRight, HLP_ExpandTgtBottom, 180, 181, 185, 186, 187, 186);

HLP_DrawExpanding()
    HLP_DrawExpandLines(OL, OT, OR, OB, StageSize_Left, StageSize_Top, StageSize_Right, StageSize_Bottom, 180, 181, 185, 186, 187, 186);


*/
void HLP_DrawExpandLines__TODO(int16_t OL, int16_t OT, int16_t OR, int16_t OB, int16_t TL, int16_t TT, int16_t TR, int16_t TB, int16_t C11, int16_t C12, int16_t C21, int16_t C22, int16_t C31, int16_t C32)
{

}


// WZD o149p19
/*
; calculates and sets the return pointer values to the
; Left and Top drawing coordinates of the item view
; window based on the item's origin location, and
; resizes the image during the initial "grow out"
; animation

ITEM_GetViewDrawPos(ITEM_ViewLeft, ITEM_ViewTop, item_view_popup_bitm, 5, &IView_Draw_Left, &IView_Draw_Top);
*/
void ITEM_GetViewDrawPos(int16_t start_x, int16_t start_y, SAMB_ptr picture_seg, int16_t stages, int16_t * draw_x, int16_t * draw_y)
{
    int16_t scale_percentage;
    int16_t y2;
    int16_t x2;
    int16_t y1;
    int16_t width;
    int16_t height;
    int16_t x1;  // _DI_

    // TODO  width = farpeekw(picture_seg, 0);
    // TODO  height = farpeekw(picture_seg, 2);
    width = FLIC_GET_WIDTH(picture_seg);
    height = FLIC_GET_HEIGHT(picture_seg);

    x1 = (start_x - (width / 2));
    y1 = (start_y - height);
    x2 = (x1 + width);
    y2 = (y1 + height);

    if(x1 < 5)
    {
        x1 = 5;
        x2 = (4 + (width / 2));
    }

    if(y1 < 5)
    {
        y1 = 5;
        y2 = (4 + (height / 2));
    }

    if(x2 > 315)
    {
        x2 = 315;
        x1 = ((315 - width) + 1);
    }

    // BUG: should be 195  (200 - 5)(SCREENYMAX - item view window margin)
    if(y2 > 315)
    {
        y1 = ((315 - height) + 1);
    }

    if(m_item_view_grow_stage > stages)
    {
        m_item_view_grow_stage = stages;
    }

    if(m_item_view_grow_stage == stages)
    {
        *draw_x = x1;
        *draw_y = y1;
    }
    else
    {
        *draw_x = (start_x + (((x1 - start_x) * m_item_view_grow_stage) / stages));
        *draw_y = (start_y + (((y1 - start_y) * m_item_view_grow_stage) / stages));
        scale_percentage = ((m_item_view_grow_stage * 100) / stages);
        Scale_Bitmap(picture_seg, scale_percentage, scale_percentage);
    }

    m_item_view_grow_stage++;

}


// WZD o149p20
void ITEM_ResetViewGrow(void)
{
    m_item_view_grow_stage = 1;
}

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
