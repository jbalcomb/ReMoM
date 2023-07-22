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
// MGC s26p01
void Set_Page_On(void);

// WZD s28p02
// MGC s26p02
void Set_Page_Off(void);

// WZD s28p03
// MGC s26p03
void Check_Default_Video_Page(void);



// WZD s28p04
// MGC s26p04
// PLATFORM
void Page_Flip(void);



// WZD s28p05
// MGC s26p05
void Copy_On_to_Off_Page(void);

// WZD s28p06
// MGC s26p06
void Copy_On_to_Off_YM(void);

// WZD s28p07
// MGC s26p07
void Copy_On_to_Off_NM(void);



#ifdef __cplusplus
}
#endif

#endif  /* VIDEO_H */
