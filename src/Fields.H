#ifndef FIELDS_H
#define FIELDS_H

#include "MoX_TYPE.H"


extern int16_t fields_count;
extern struct s_Field * p_fields;



enum e_Field_Type
{
    ft_ClickButton,     /*  0  0x00 */
    ft_ToggleButton,    /*  1  0x01 */
    ft_LockableButton,  /*  2  0x02 */
    ft_MStateButton,    /*  3  0x03 */
    ft_EditBox,         /*  4  0x04 */
    ft_ImageLabel,      /*  5  0x05 */
    ft_Slidebar,        /*  6  0x06 */
    ft_HotKey,          /*  7  0x07 */
    ft_AltString,       /*  8  0x08 */
    ft_ClickLink,       /*  9  0x09 */
    ft_DialogLine,      /* 10  0x0A */
    ft_EditSelect,      /* 11  0x0B */
    ft_ClickGrid        /* 12  0x0C */
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
    int16_t Param0;     // Text@        (6:Min_Val, 9:Parent)
    int16_t Param1;     // Trig_State   (6:Max_Val, 9/A:Index, B:IMG, C:CellW)
    int16_t Param2;     // State@       (4/B:BG_Color, 6:StSeg, 8:Len, C:CellH)
    int16_t Param3;     // FullFrames   (4:Align, 6:MinSlide, A:CBlock, C:Col@)
    int16_t Param4;     // MaxSlide     (4:CStyle, C:Row@)
    int16_t Param5;     // IMG_Seg      (4/B:MaxLen, 6:Orient)
    int16_t Param6;     // CursorColors (6:StOff, B:Outline)
    int16_t hotkey;     // ASCII Character *Code*
    int16_t Unused_25h; // ?IDK/IDGI?
};



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

// MGC s34p47
int16_t Add_Hidden_Field(int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax, int16_t hotkey, int16_t help);

// MGC s34p49
int16_t Add_Hot_Key(int16_t select_char);

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

// TODO  // MGC s34p85
// TODO  void Toggle_Pages(void);




#ifdef __cplusplus
}
#endif



#endif /* FIELDS_H */
