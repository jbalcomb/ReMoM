#ifndef VIDEO_H
#define VIDEO_H

#include "MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



extern int16_t draw_page_num;
extern uint8_t * current_video_page;
extern uint8_t * video_page_buffer[];
extern uint8_t * draw_page;



// MGC s26p04
void Page_Flip(void);



#ifdef __cplusplus
}
#endif

#endif  /* VIDEO_H */
