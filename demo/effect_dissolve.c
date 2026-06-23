/*
    effect_dissolve.c — mosaic dissolve page-flip demo (MoX PageFlip_Dissolve).

    Dissolves back and forth between two real MoM screens (the magic screen and the overland main
    screen).  The two decoded images are kept in the spare video pages (F3 / F4 = pages 2 / 3),
    which the dissolve itself never touches; each cycle copies the next image into the off page and
    PageFlip_Dissolve() reveals it onto the visible page, then page-flips.

    If the game assets are unavailable, two synthetic patterns are used instead.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/Video.h"   /* PageFlip_Dissolve(), video_page_buffer, draw_page_num */
#include "../MoX/src/Video2.h"  /* screen_pixel_size */

#include <string.h>

#define DEMO_DWELL_MILLIES  1200   /* pause between transitions so each dissolve is watchable */

/* Spare pages used as decoded-image storage (the dissolve only uses pages 0 and 1). */
#define DEMO_IMG_A_PAGE  2
#define DEMO_IMG_B_PAGE  3



static void Effect_Dissolve_Run(void)
{
    uint8_t * img_a;
    uint8_t * img_b;
    int have_assets;
    int cycle;

    /* Pace the 4 grain passes so the dissolve is perceptible (production default is 0 = instant). */
    g_pageflip_dissolve_pass_delay = 90;

    img_a = video_page_buffer[DEMO_IMG_A_PAGE];
    img_b = video_page_buffer[DEMO_IMG_B_PAGE];

    /* Decode the two screens once into the spare pages (both use main-game palette 0).
       Defaults are opaque full-screen backgrounds that render standalone; override with:
       demo_vga dissolve <lbxA> <entryA> <lbxB> <entryB> */
    {
        const char * lbx_a = (Demo_Arg(0) != NULL) ? Demo_Arg(0) : "MAIN";
        int          ent_a = Demo_Arg_Int(1, 0);
        const char * lbx_b = (Demo_Arg(2) != NULL) ? Demo_Arg(2) : "NEWGAME";
        int          ent_b = Demo_Arg_Int(3, 0);
        /* One shared palette: a cross-dissolve shows both images at once under a single active
           palette, so per-screen palettes would be incoherent.  Pick screens that share one. */
        int          palette = Demo_Arg_Int(4, 0);

        have_assets = Demo_Load_Screen(lbx_a, ent_a, palette, DEMO_IMG_A_PAGE);
        if(have_assets)
        {
            have_assets = Demo_Load_Screen(lbx_b, ent_b, palette, DEMO_IMG_B_PAGE);
        }
    }
    if(have_assets == 0)
    {
        Demo_Fill_Pattern(img_a, 0);
        Demo_Fill_Pattern(img_b, 1);
    }

    /* Show image A on the visible page to start. */
    memcpy(video_page_buffer[draw_page_num], img_a, (size_t)screen_pixel_size);
    Demo_Present();
    Demo_Dwell(DEMO_DWELL_MILLIES);

    cycle = 0;
    while(Demo_Quit() == 0)
    {
        /* Alternate target B, A, B, A, ...  Copy it into the off page, then dissolve to it. */
        uint8_t * target = ((cycle & 1) == 0) ? img_b : img_a;

        memcpy(video_page_buffer[1 - draw_page_num], target, (size_t)screen_pixel_size);
        PageFlip_Dissolve();

        Demo_Dwell(DEMO_DWELL_MILLIES);
        cycle++;
    }
}


const Demo_Effect effect_dissolve =
{
    "dissolve",
    "Mosaic dissolve page-flip between two real screens (PageFlip_Dissolve)",
    Effect_Dissolve_Run
};
