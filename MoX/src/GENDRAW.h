/*
    WIZARDS.EXE
        ovr149

    MoO2
        GENDRAW
*/

#ifndef GENDRAW_H
#define GENDRAW_H

#include "EMS/EMS.h"
#include "MOX_T4.h"
#include "MOX_TYPE.h"
#include "TEXTBOX.h"

#include <string.h>     /* memcpy() memset(), stu_strcat(), stu_strcpy(), stricmp() */



/*
¿ ~== MoO2's Save_Block_To_Drive() in Text_Box_Startup_() ?
~== Save_ScreenSeg()
¿ ~== MoO2's Restore_Block_From_Drive() in Text_Box_Cleanup_() ?
~== Restore_ScreenSeg()
*/

#define SAVE_SCREEN_SEG     { EMS_PFBA = EMM_Map4Pages_VGAFILEH1234(); memcpy(&EMS_PFBA[0], &_screen_seg[0], 32768); memcpy(&EMS_PFBA[(2048 * SZ_SEGMENT)], &_screen_seg[(2048 * SZ_SEGMENT)], 32768); }
#define RESTORE_SCREEN_SEG  { EMS_PFBA = EMM_Map4Pages_VGAFILEH1234(); memcpy(&_screen_seg[0], &EMS_PFBA[0], 32768); memcpy(&_screen_seg[(2048 * SZ_SEGMENT)], &EMS_PFBA[(2048 * SZ_SEGMENT)], 32768); }



#ifdef __cplusplus
extern "C" {
#endif



enum e_TEXTBOX_BACKGROUND_COLOR
{
    tb_Blue,
    tb_Red,
    tb_Green,
    tb_Brown
};





// WZD dseg:6F78
extern uint8_t COL_ConfirmShadows[5];
// WZD dseg:6F7D
extern uint8_t COL_Dialog_Text[5];


// WZD dseg:6F96
extern int16_t confirmation_box_x_offset;
// WZD dseg:6F98
extern int16_t confirmation_box_y_offset;
// WZD dseg:6F9A
extern int16_t notify_draw_offscr;



// WZD dseg:CA8E                                                 BEGIN:  ovr149  GENDRAW - Uninitialized Data

extern SAMB_ptr confirmation_button_no_seg;
extern SAMB_ptr confirmation_button_yes_seg;
extern SAMB_ptr confirmation_background_bottom_seg;
extern SAMB_ptr confirmation_background_top_seg;
extern int16_t notify_pict1_y;
extern int16_t notify_pict1_x;
extern SAMB_ptr notify_pict1;
extern int16_t notify_color_slide;
extern int16_t notify_color_slide_cycle;
extern char * message_box_text;
extern int16_t message_box_y;
extern int16_t message_box_x;

// WZD dseg:CB54                                                 END:  ovr149  GENDRAW - Uninitialized Data ; 





// WZD o149p01
int16_t Confirmation_Box(char * text_string);

// WZD o149p02
void Confirmation_Box_Draw(void);

// WZD o149p03
void Warn1(char * msg);

// WZD o149p04
void Warn0(char * msg);

// WZD o149p05
void Warn(char * msg);

// WZD o149p06
void Warn_Draw(void);

// WZD o149p07
int16_t Selection_Box(int16_t item_count, char ** list_text, int16_t multi_page, char * title_string);

// WZD o149p08
void Selection_Box_Draw(void);

// WZD o149p09
void Selection_Box_Coords(int16_t item_count, char ** list_text, char * title_string);

// WZD o149p10
void Notify_Load(int16_t notify_color, SAMB_ptr notiy_pict_seg);

// WZD o149p11
void Notify1(int16_t Center, int16_t Top, int16_t notify_color, char * Msg, int16_t Gem, SAMB_ptr notiy_pict_seg, int16_t IMGLeft, int16_t IMGTop, SAMB_ptr IMG2, int16_t IMG2Left, int16_t IMG2Top, int16_t CSlide, int16_t BGShade);

// WZD o149p12
void Notify1_Draw(void);

// WZD o149p13
void Notify2(int16_t Center, int16_t Top, int16_t notify_color, char * Msg, int16_t Gem, SAMB_ptr notiy_pict_seg, int16_t IMGLeft, int16_t IMGTop, SAMB_ptr IMG2, int16_t IMG2Left, int16_t IMG2Top, int16_t CSlide, int16_t BGShade);

// WZD o149p14
void Notify2_Draw(void);

#define EXPANDING_BOX_STAGES  5

extern int16_t _draw_expanding_box;
extern int16_t expanding_origin_y2;
extern int16_t expanding_origin_x2;
extern int16_t expanding_origin_y1;
extern int16_t expanding_origin_x1;
extern int16_t expanding_box_y2;
extern int16_t expanding_box_x2;
extern int16_t expanding_box_y1;
extern int16_t expanding_box_x1;

// WZD o149p15
void Draw_Expanding_Box(int16_t box_x1, int16_t box_y1, int16_t box_x2, int16_t box_y2, int16_t origin_x1, int16_t origin_y1, int16_t origin_x2, int16_t origin_y2, int16_t draw_expanding_box);

// WZD o149p16
int16_t Draw_Expanding_Box_Picture(int16_t box_x1, int16_t box_y1, int16_t box_x2, int16_t box_y2, int16_t picture_x1, int16_t picture_y1, int16_t picture_x2, int16_t picture_y2, int16_t origin_x1, int16_t origin_y1, int16_t origin_x2, int16_t origin_y2, int16_t draw_expanding_box);

// WZD o149p17
int16_t Redraw_Expanding_Box_Lines(void);

// WZD o149p18
void Draw_Expanding_Box_Lines(int16_t origin_x1, int16_t origin_y1, int16_t origin_x2, int16_t origin_y2, int16_t box_x1, int16_t box_y1, int16_t box_x2, int16_t box_y2, int16_t color_1a, int16_t color_1b, int16_t color_2a, int16_t color_2b, int16_t color_3a, int16_t color_3b);

// WZD o149p19
void Expanding_Picture_Position(int16_t start_x, int16_t start_y, SAMB_ptr picture_seg, int16_t stages, int16_t * draw_x, int16_t * draw_y);

// WZD o149p20
void ITEM_ResetViewGrow(void);

// WZD o149p21
void Save_ScreenSeg(void);

// WZD o149p22
void Restore_ScreenSeg(void);



#ifdef __cplusplus
}
#endif

#endif  /* GENDRAW_H */
