#ifndef FIELDS_H
#define FIELDS_H

#include "MoX_TYPE.H"


extern int16_t fields_count;
extern struct s_Field * p_fields;



enum e_Field_Type
{
    ft_Button,          /*  0  0x00 */
    ft_ToggleButton,    /*  1  0x01 */
    ft_LockableButton,  /*  2  0x02 */
    ft_MStateButton,    /*  3  0x03 */
    ft_EditBox,         /*  4  0x04 */
    ft_ImageLabel,      /*  5  0x05 */
    ft_Slidebar,        /*  6  0x06 */
    ft_HotKey,          /*  7  0x07 */
    ft_MultiHotKey,     /*  8  0x08 */
    ft_ClickLink,       /*  9  0x09 */
    ft_DialogLine,      /* 10  0x0A */
    ft_EditSelect,      /* 11  0x0B */
    ft_Grid             /* 12  0x0C */
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



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    MGC  seg034
*/

// TODO  // MGC s34p01
// TODO  int16_t Interpret_Mouse_Input(void);

// TODO  // MGC s34p08
// TODO  void Set_Global_Esc(void);

// TODO  // MGC s34p12
// TODO  void Set_Help_List(uint16_t help_seg, int16_t count);

// TODO  // MGC s34p13
// TODO  void Deactivate_Help_List(void);

// TODO  // MGC s34p22
// TODO  int16_t Interpret_Keyboard_Input(char  * character);

// TODO  // MGC s34p25
// TODO  int16_t Scan_Input(void);
// TODO  // MGC s34p26
// TODO  int16_t Scan_Field(void);


// WZD s36p38
int16_t Add_Multi_Hot_Key_Field(char * string);


// WZD s36p42
void Add_Button_Info(int16_t xmin, int16_t ymin, int16_t string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help);

// WZD s36p43
int16_t Add_Button_Field(int16_t xmin, int16_t ymin, int16_t string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help);

// WZD s36p47
// MGC s34p47
int16_t Add_Hidden_Field(int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax, int16_t hotkey, int16_t help);

// WZD s36p49
// MGC s34p49
int16_t Add_Hot_Key(int16_t select_char);

// WZD s36p49
int16_t Add_Grid_Field(int16_t xmin, int16_t ymin, int16_t box_width, int16_t box_height, int16_t horizontal_count, int16_t vertical_count, int16_t *xpos, int16_t *ypos, int16_t help);

// MGC s34p57
void Clear_Fields(void);


// TODO // MGC s34p66
// TODO int16_t Get_Input(void);

// TODO  // MGC s34p67
// TODO  void Set_Input_Delay(int16_t input_delay);

// TODO  // MGC s34p71
// TODO  Draw_Fields();
// TODO  // MGC s34p72
// TODO  Draw_Field();
// TODO  // MGC s34p72
// TODO  Push_Field_Down();

// WZD s36p85
// MGC s34p85
// in Input  void Toggle_Pages(void);




#ifdef __cplusplus
}
#endif



#endif /* FIELDS_H */
