#ifndef MOM_HPP
#define MOM_HPP




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


#include "MoX_TYPE.H"


extern int16_t draw_page_num;
extern uint8_t * video_page_buffer[2];
extern uint8_t * draw_page;


// TODO(JimBalcomb,20221216): Services that the *platform layer* provides to the *game*
void Pump_Events(void);

// TODO(JimBalcomb,20221216): Services that the *game* provides to the *platform layer*
void GameUpdateAndRender(game_offscreen_buffer * Buffer);

void Update_Mouse_Position(int16_t mouse_x, int16_t mouse_y);


#ifdef __cplusplus
}
#endif


#endif /* MOM_HPP */
