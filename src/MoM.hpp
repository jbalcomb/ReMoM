#ifndef MOM_HPP



struct game_offscreen_buffer
{
    void *Memory;
    int Width;
    int Height;
    int Pitch;
};



// TODO(JimBalcomb,20221216): Services that the *platform layer* provides to the *game*



// TODO(JimBalcomb,20221216): Services that the *game* provides to the *platform layer*
void GameUpdateAndRender(game_offscreen_buffer * Buffer);



#define MOM_HPP



#endif /* MOM_HPP */
