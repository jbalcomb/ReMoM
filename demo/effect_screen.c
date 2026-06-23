/*
    effect_screen.c — Phase 1 asset-pipeline tracer.

    Loads one real MoM screen and holds it, with no effect applied.  This isolates and validates
    the load/blit/palette path (LBX_Load -> Draw_Picture -> Load_Palette) before any effect logic
    is layered on.  If assets are unavailable it falls back to a synthetic pattern so the demo
    still runs.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/Video.h"   /* video_page_buffer, draw_page_num */

#include <stdio.h>

/* Default source screen: MAIN.LBX entry 0 is the opaque overland HUD frame, which renders
   correctly standalone.  Override with: demo_vga screen <LBX> <entry>
   (Note: overlay-style screens like MAGIC have translucent/shaded regions that resolve to
    "splotches" when drawn over a blank page, so they don't render well standalone.) */
#define DEMO_SCREEN_LBX_DEFAULT      "MAIN"
#define DEMO_SCREEN_ENTRY_DEFAULT     0
#define DEMO_SCREEN_PALETTE           0   /* EMPERATO main-game palette */



static void Effect_Screen_Run(void)
{
    const char * lbx = (Demo_Arg(0) != NULL) ? Demo_Arg(0) : DEMO_SCREEN_LBX_DEFAULT;
    int entry = Demo_Arg_Int(1, DEMO_SCREEN_ENTRY_DEFAULT);
    int palette = Demo_Arg_Int(2, DEMO_SCREEN_PALETTE);

    if(Demo_Load_Screen(lbx, entry, palette, draw_page_num))
    {
        printf("[screen] loaded %s entry %d (palette %d)\n", lbx, entry, palette);
    }
    else
    {
        printf("[screen] assets unavailable -- falling back to synthetic pattern\n");
        Demo_Fill_Pattern(video_page_buffer[draw_page_num], 0);
    }

    Demo_Present();

    /* Hold the image until the user closes the window. */
    while(Demo_Quit() == 0)
    {
        Demo_Dwell(200);
    }
}


const Demo_Effect effect_screen =
{
    "screen",
    "Display one real MoM screen (asset-pipeline tracer)",
    Effect_Screen_Run
};
