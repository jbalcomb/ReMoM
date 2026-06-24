/*
    effect_earthquake.c — screen-shake demo (MoX VGA_PartCopyFromF3).

    Mirrors the city-enchantment Earthquake animation (MoM Spells137.c): a clean screen is held in
    frame F3 (video_page_buffer[2]); the shake copies it back into the visible page at decaying
    random offsets via VGA_PartCopyFromF3, which also blacks out the top/left gap exposed by the
    shift.  As the offset magnitude decays the image settles back to rest.

    The shaken screen is the overland main screen (MAIN.LBX entry 0) -- the earthquake's real
    in-game context.  If assets are unavailable a synthetic pattern is shaken instead.

    The overland variant (MoM Spells129.c) instead uses a constant jitter, VGA_PartCopyFromF3(Random(4)+1,
    Random(3)+1); the decaying form is used here because it reads as one self-contained quake.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/MOX_BASE.h"
#include "../MoX/src/Video.h"   /* VGA_PartCopyFromF3(), current_video_page, video_page_buffer, draw_page_num */

#define DEMO_REST_MILLIES   700   /* steady image before/after each quake */
#define DEMO_SHAKE_MILLIES   33   /* per shake-frame (~30 fps)            */

/* VGA_ShiftCopyFromF3 reads frame F3 == video_page_buffer[2]. */
#define DEMO_F3_PAGE  2



static void Effect_Earthquake_Run(void)
{
    int itr;

    /* The clean base image lives in the F3 save page; loaded once.
       Default is the overland screen; override with: demo_vga earthquake <LBX> <entry> */
    {
        const char * lbx = (Demo_Arg(0) != ST_NULL) ? Demo_Arg(0) : "MAIN";
        int entry = Demo_Arg_Int(1, 0);
        int palette = Demo_Arg_Int(2, 0);

        if(Demo_Load_Screen(lbx, entry, palette, DEMO_F3_PAGE) == 0)
        {
            Demo_Fill_Pattern(video_page_buffer[DEMO_F3_PAGE], 0);
        }
    }

    /* The visible page is what we present and what VGA_PartCopyFromF3 writes into. */
    current_video_page = video_page_buffer[draw_page_num];

    while(Demo_Quit() == 0)
    {
        /* Steady image at rest. */
        VGA_PartCopyFromF3(0, 0);
        Demo_Present();
        Demo_Dwell(DEMO_REST_MILLIES);

        /* Decaying shake: offset magnitude shrinks as itr falls (mirrors Random(itr) / 5). */
        for(itr = 35; (itr > 5) && (Demo_Quit() == 0); itr--)
        {
            VGA_PartCopyFromF3(Demo_Rand(itr) / 5, Demo_Rand(itr) / 5);
            Demo_Present();
            Demo_Dwell(DEMO_SHAKE_MILLIES);
        }

        /* Settle back to the clean image. */
        VGA_PartCopyFromF3(0, 0);
        Demo_Present();
        Demo_Dwell(DEMO_REST_MILLIES);
    }
}


const Demo_Effect effect_earthquake =
{
    "earthquake",
    "Screen-shake of the overland screen via VGA_PartCopyFromF3 (decaying offsets)",
    Effect_Earthquake_Run
};
