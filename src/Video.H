#ifndef VIDEO_H
#define VIDEO_H

#include "MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif



extern int16_t draw_page_num;
extern uint8_t * current_video_page;
extern uint8_t * video_page_buffer[];
extern uint8_t * draw_page;




// WZD s28p01
void Set_Page_On(void);

// WZD s28p02
void Set_Page_Off(void);

// WZD s28p03
void Check_Default_Video_Page(void);



// WZD s28p04
// PLATFORM
void Page_Flip(void);



// WZD s28p05
// MoO2  Copy_On_To_Off_Page()
void Copy_On_To_Off_Page(void);

// WZD s28p06
void Copy_On_To_Off_YM(void);

// WZD s28p07
void Copy_On_To_Off_NM(void);

// WZD s28p08
void Copy_Off_To_Back(void);

// WZD s28p09
void Copy_Back_To_Off(void);

// WZD s28p10
// drake178: VGA_SaveDraw_Frame4()
// copy VGA RAM 0xA000 to VGA RAM 0xAC00 - frame-buffer 0 to frame-buffer 3
void VGA_SaveDraw_Frame4(void);

// WZD s28p11
// drake178: VGA_LoadDraw_Frame4()
// copy VGA RAM 0xAC00 to VGA RAM 0xA000 - frame-buffer 3 to frame-buffer 0
void VGA_LoadDraw_Frame4(void);



#ifdef __cplusplus
}
#endif

#endif  /* VIDEO_H */
