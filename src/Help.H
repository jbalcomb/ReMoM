#ifndef HELP_H
#define HELP_H

#include "MoX_TYPE.H"
#include "MoX_DEF.H"


//  ; (sizeof=0xA)
struct s_HLP_ENTRY
{
    int16_t help_idx;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
};

//  ; (sizeof=0x418)
struct s_HLP_DATA
{
    char title[30];
    char lbx_name[14];
    int16_t lbx_entry;
    int16_t Include_Entry;
    char help_text[1000];
};




// dseg:E872                                                 ? BEGIN: fields, intput, ... ?

// WZD dseg:E872
// AKA _help_list
extern struct s_HLP_ENTRY * help_struct_pointer;




// WZD dseg:8296
// AKA have_help
extern int16_t help_list_active;
// WZD dseg:8298
// AKA _help_count
extern int16_t help_list_count;
// dseg:829A 00 00                                           KD_ActiveStringTrig dw 0                ; DATA XREF: Interpret_Keyboard_Input:loc_27F9Br ...
// dseg:829C 72 62                                           UU_cnst_RB7 db 'rb'                     ; DATA XREF: UU_GUI_LoadClickFile+Eo
// dseg:829E 00                                              cnst_ZeroString_12 db 0                 ; DATA XREF: GUI_TextEditDialog+3Bo ...
// dseg:829F 77 62 00                                        UU_cnst_WB8 db 'wb',0                   ; DATA XREF: UU_GUI_SaveClickFile+Bo
// dseg:82A2 05 00                                           UU_GUI_Up_Hotkey db 5, 0                ; DATA XREF: UU_GUI_ScrollableTxtDlg+21Do ...
// dseg:82A4 06 00                                           UU_GUI_Down_Hotkey db 6, 0              ; DATA XREF: UU_GUI_ScrollableTxtDlg+240o ...
// dseg:82A6 5F 00                                           GUI_EditCursor db '_',0                 ; DATA XREF: Draw_Fields+117Do

// dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard
// dseg:82A6



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    WZD  seg036
    MGC  seg034
*/

// WZD s36p12
void Set_Help_List(struct s_Help_Entry * help_pointer, int16_t count);

// WZD s36p13
void Deactivate_Help_List(void);

// WZD s36p14
int16_t Check_Help_List(void);

// WZD o159p01
void Draw_Help_Entry_(int16_t help_entry_idx);


#ifdef __cplusplus
}
#endif



#endif /* HELP_H */
