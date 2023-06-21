#ifndef FIELDS_H
#define FIELDS_H

#include "MoX_TYPE.H"

#include "assert.h"


// WZD dseg:E872                                                 ? BEGIN: fields, intput, ... ?

// WZD dseg:E872
// help_list dw 0
// WZD dseg:E874
// GUI_Processed_Btns dw 0
// WZD dseg:E876
// GUI_Processed_LastY dw 0
// WZD dseg:E878
// GUI_Processed_LastX dw 0
// WZD dseg:E87A
// GUI_EditString db 30 dup(0)
// WZD dseg:E898
extern int16_t input_delay;
// WZD dseg:E89A
// MOUSE_Emu_Y dw 0
// WZD dseg:E89C
// MOUSE_Emu_X dw 0
// WZD dseg:E89E
extern int16_t cursor_offset;
// WZD dseg:E8A0
// GUI_PrevControlCount dw 0
// WZD dseg:E8A2
extern int16_t fields_count;
// WZD dseg:E8A4
// GUI_Prev_Redraw_Fn dd 0
// WZD dseg:E8A8
// GUI_Redraw_Function dd 0
// WZD dseg:E8AC
extern struct s_Field * p_fields;  // "p_fields dd 0" ? Far Pointer?

// WZD dseg:E8AC                                                 ? END: fields, input, ... ?


// dseg:E872                                                 ? BEGIN: fields, intput, ... ?

// WZD dseg:E872
// AKA _help_list
// HELP int16_t help_struct_pointer;




// WZD dseg:8296
// AKA have_help
// HELP int16_t help_list_active = ST_FALSE;
// WZD dseg:8298
// AKA _help_count
// HELP int16_t help_list_count = 0;
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
    int16_t Param3;     // FullFrames    Align     MinSlide                 CBlock            xpos
    int16_t Param4;     // MaxSlide      CStyle                                               ypos
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



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    WZD  seg036
    MGC  seg034
*/

// WZD s36p01
// INPUT  int16_t Interpret_Mouse_Input(void);

// WZD s36p08
// TODO  void Set_Global_Esc(void);

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
int16_t Add_Grid_Field(int16_t xmin, int16_t ymin, int16_t box_width, int16_t box_height, int16_t horizontal_count, int16_t vertical_count, int16_t *xpos, int16_t *ypos, int16_t help);

// WZD s36p57
void Clear_Fields(void);


// WZD s36p66
// INPUT  int16_t Get_Input(void);

// WZD s36p67
// INPUT  void Set_Input_Delay(int16_t input_delay);

// WZD s36p71
// TODO  Draw_Fields();
// WZD s36p72
// TODO  Draw_Field();
// WZD s36p72
// TODO  Push_Field_Down();

// WZD s36p85
// INPUT  void Toggle_Pages(void);




#ifdef __cplusplus
}
#endif



#endif /* FIELDS_H */
