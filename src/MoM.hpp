#ifndef MOM_HPP



#include "MoX_TYPE.H"



struct game_offscreen_buffer
{
    void * Memory;
    int Width;
    int Height;
    int Pitch;
};



#ifdef __cplusplus
extern "C" {
#endif

extern int16_t draw_page_num;
extern uint8_t * video_page_buffer[2];
extern uint8_t * draw_page;

#ifdef __cplusplus
}
#endif



// TODO(JimBalcomb,20221216): Services that the *platform layer* provides to the *game*



// TODO(JimBalcomb,20221216): Services that the *game* provides to the *platform layer*
void GameUpdateAndRender(game_offscreen_buffer * Buffer);



#define MOM_HPP



#endif /* MOM_HPP */
