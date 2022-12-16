
#include "MoM.hpp"


void Render_VBB(game_offscreen_buffer * Buffer)
{
    uint8_t * bbuf;
    bbuf = (uint8_t *)g_Video_Back_Buffer;
    // ?  struct tagRGBQUAD XBGR;

    uint32_t * Pixel = (uint32_t*)Buffer->Memory;
    int itr;
    unsigned char vbb_byte;
    unsigned int * p_XBGR;
    unsigned int long_XBGR;
    p_XBGR = (uint32_t *)g_Palette_XBGR;
    for(itr = 0; itr < 64000; itr++)
    {
        vbb_byte = *(g_Video_Back_Buffer + itr);
        long_XBGR = *(p_XBGR + vbb_byte);
        *Pixel++ = long_XBGR;
    }

}


void GameUpdateAndRender(game_offscreen_buffer * Buffer)
{
    Render_VBB(Buffer);
}