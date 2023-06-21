
#include "MoX_TYPE.H"

#include "Help.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif



// dseg:E872                                                 ? BEGIN: fields, intput, ... ?

// WZD dseg:E872
// AKA _help_list
struct s_HLP_ENTRY * help_struct_pointer;




// WZD dseg:8296
// AKA have_help
int16_t help_list_active = ST_FALSE;
// WZD dseg:8298
// AKA _help_count
int16_t help_list_count = 0;
// dseg:829A 00 00                                           KD_ActiveStringTrig dw 0                ; DATA XREF: Interpret_Keyboard_Input:loc_27F9Br ...
// dseg:829C                                                 ; char UU_cnst_RB7[]
// dseg:829C 72 62                                           UU_cnst_RB7 db 'rb'                     ; DATA XREF: UU_GUI_LoadClickFile+Eo
// dseg:829C                                                                                         ; should use dseg:28b8
// dseg:829E                                                 ; char cnst_ZeroString_12[]
// dseg:829E 00                                              cnst_ZeroString_12 db 0                 ; DATA XREF: GUI_TextEditDialog+3Bo ...
// dseg:829F                                                 ; char UU_cnst_WB8[]
// dseg:829F 77 62 00                                        UU_cnst_WB8 db 'wb',0                   ; DATA XREF: UU_GUI_SaveClickFile+Bo
// dseg:829F                                                                                         ; should use dseg:28ef
// dseg:82A2 05 00                                           UU_GUI_Up_Hotkey db 5, 0                ; DATA XREF: UU_GUI_ScrollableTxtDlg+21Do ...
// dseg:82A4 06 00                                           UU_GUI_Down_Hotkey db 6, 0              ; DATA XREF: UU_GUI_ScrollableTxtDlg+240o ...
// dseg:82A6 5F 00                                           GUI_EditCursor db '_',0                 ; DATA XREF: Draw_Fields+117Do
// dseg:82A6
// dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard
// dseg:82A6


/*
    WZD  seg036
    MGC  seg034
*/

// WZD s36p12
void Set_Help_List(struct s_Help_Entry * help_pointer, int16_t count)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Set_Help_List()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Set_Help_List()\n", __FILE__, __LINE__);
#endif
}

// WZD s36p13
void Deactivate_Help_List(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Deactivate_Help_List()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Deactivate_Help_List()\n", __FILE__, __LINE__);
#endif
}

// WZD s36p14
// CAUTION: returns ZERO on SUCCESS
int16_t Check_Help_List(void)
{
    int16_t help_entry_found;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Check_Help_List()\n", __FILE__, __LINE__);
#endif

    help_entry_found = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Check_Help_List()\n", __FILE__, __LINE__);
#endif
    return help_entry_found;
}

// WZD o159p01
void Draw_Help_Entry_(int16_t help_entry_idx)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Draw_Help_Entry_()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Draw_Help_Entry_()\n", __FILE__, __LINE__);
#endif
}
