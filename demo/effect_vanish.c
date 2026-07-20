/*
    effect_vanish.c — demo of Vanish_Bitmap (the dither materialize/dematerialize effect).

    Modeled on how Cityscape_Draw_Buildings uses it (CITYSCAP.c): each frame it renders a sprite into
    a raw scratch bitmap (Draw_Picture_To_Bitmap), dithers that copy at a visibility percent
    (Vanish_Bitmap -- pixels whose dither threshold exceeds 'percent' become transparent), flags it
    (FLIC_Set_LoopFrame_1), and draws it (Draw_Picture).  As percent rises 0->100 the sprite
    materializes; 100->0 it dematerializes.  Vanish_Bitmap is destructive, so we re-render a fresh
    copy every frame, exactly as the cityscape does.

    Default sprite is a CITYSCAP.LBX building (entries 42+).  Override with:
        demo_vga vanish <LBX> <entry>

    No world state needed -- trimmed boot.  Transparent (vanished) pixels show the black background.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/Video.h"      /* video_page_buffer, draw_page_num, current_video_page */
#include "../MoX/src/Graphics.h"   /* Fill */
#include "../MoX/src/FLIC_Draw.h"  /* Draw_Picture_To_Bitmap, Vanish_Bitmap, FLIC_Set_LoopFrame_1, Draw_Picture, Set_Animation_Frame */
#include "../MoX/src/Fonts.h"      /* Load_Palette, Apply_Palette, Calculate_Remap_Colors */
#include "../MoX/src/MOX_TYPE.h"   /* SAMB_ptr */

#include "../platform/include/Platform.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define VANISH_STEP       4    /* percent per frame */
#define VANISH_FRAME_MS  40    /* ~25 fps           */
#define VANISH_HOLD_MS  600    /* pause at full / empty */
#define VANISH_DRAW_X   136    /* where to draw the sprite */
#define VANISH_DRAW_Y    72
#define VANISH_BUF_SIZE (64000 + 64)   /* raw bitmap scratch; big enough for any sprite */



static void Effect_Vanish_Run(void)
{
    const char * lbx = (Demo_Arg(0) != NULL) ? Demo_Arg(0) : "CITYSCAP";
    int entry = Demo_Arg_Int(1, 42);   /* CITYSCAP building sprites start at 42 */
    SAMB_ptr src;
    uint8_t * work;
    int percent;
    int dir;

    if(Demo_Assets_Ready() == 0)
    {
        printf("[vanish] assets unavailable -- needs the game LBX files\n");
        return;
    }

    src = (SAMB_ptr)Demo_Lbx_Load(lbx, entry);
    if(src == NULL)
    {
        printf("[vanish] could not load %s entry %d\n", lbx, entry);
        return;
    }

    work = (uint8_t *)malloc(VANISH_BUF_SIZE);
    if(work == NULL)
    {
        return;
    }

    Load_Palette(0, -1, 0);
    Calculate_Remap_Colors();
    Apply_Palette();

    printf("[vanish] %s entry %d -- materialize/dematerialize; close to exit\n", lbx, entry);

    percent = 0;
    dir = VANISH_STEP;
    while(Demo_Quit() == 0)
    {
        current_video_page = video_page_buffer[draw_page_num];
        Fill(0, 0, 319, 199, 0);

        /* Fresh raw copy of the sprite, then dither it at the current visibility percent. */
        Set_Animation_Frame(src, 0);
        Draw_Picture_To_Bitmap(src, (SAMB_ptr)work);
        Vanish_Bitmap((SAMB_ptr)work, (int16_t)percent);
        FLIC_Set_LoopFrame_1((SAMB_ptr)work);
        Draw_Picture(VANISH_DRAW_X, VANISH_DRAW_Y, (SAMB_ptr)work);

        Demo_Present();

        percent += dir;
        if(percent >= 100)
        {
            percent = 100;
            dir = -VANISH_STEP;
            Demo_Dwell(VANISH_HOLD_MS);   /* hold fully materialized */
        }
        else if(percent <= 0)
        {
            percent = 0;
            dir = VANISH_STEP;
            Demo_Dwell(VANISH_HOLD_MS);   /* hold fully vanished */
        }

        Platform_Sleep_Millies(VANISH_FRAME_MS);
    }

    free(work);
}


const Demo_Effect effect_vanish =
{
    "vanish",
    "Vanish_Bitmap dither materialize/dematerialize of a sprite (default CITYSCAP building 42)",
    Effect_Vanish_Run,
    0   /* trimmed boot */
};
