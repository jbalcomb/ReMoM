/*
    demo_harness.c — shared boot + helpers for the VGA / video-effect demos.  See demo_harness.h.
*/

#include "demo_harness.h"

#include "../MoX/src/MOX_BASE.h"
#include "../MoX/src/MOX_TYPE.h"
#include "../MoX/src/Video.h"     /* current_video_page */
#include "../MoX/src/Video2.h"    /* screen_pixel_width / screen_pixel_height / screen_pixel_size */
#include "../MoX/src/Mouse.h"     /* Set_Mouse_List(), mouse_list_none */
#include "../MoX/src/Graphics.h"  /* Reset_Window() */
#include "../MoX/src/Fonts.h"     /* Load_Palette() */
#include "../MoX/src/FLIC_Draw.h" /* Draw_Picture() */
#include "../MoX/src/LBX_Load.h"  /* LBX_Load() */

#include "../MoM/src/Init.h"      /* Init_Drivers() */

#include "../platform/include/Platform.h"

#include "../ext/stu_compat.h"    /* stu_strcpy() */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defined in MoX (Fonts.c); the engine reads it during Platform_Palette_Update(). */
extern uint8_t * current_palette;

#define DEMO_POLL_MILLIES  16   /* event-pump cadence during waits */

static uint8_t * demo_video_memory = NULL;   /* only used by the no-asset manual boot */
static uint8_t * demo_palette = NULL;        /* only used by the no-asset manual boot */
static uint32_t demo_rng_state = 0x12345678u;
static int demo_assets_ready = 0;
static int demo_arg_count = 0;
static char ** demo_args = NULL;

/* Writable copy of the font file name (LBX loaders mutate the name buffer). */
static char demo_font_file[] = "FONTS.LBX";



static int Demo_File_Exists(const char * name)
{
    FILE * f;

    f = fopen(name, "rb");
    if(f != NULL)
    {
        fclose(f);
        return 1;
    }
    return 0;
}


/*
    Build a vivid 256-entry palette (768 bytes, 6-bit VGA DAC values) for the no-asset fallback.
    Index 0 is black, so the edge "smear cleanup" in the shift-copy effects reads as black gaps.
*/
static void Demo_Build_Palette(void)
{
    int itr;

    for(itr = 0; itr < 256; itr++)
    {
        demo_palette[(itr * 3) + 0] = (uint8_t)(((itr >> 0) & 0x07) << 3);   /* red   0..56 */
        demo_palette[(itr * 3) + 1] = (uint8_t)(((itr >> 3) & 0x07) << 3);   /* green 0..56 */
        demo_palette[(itr * 3) + 2] = (uint8_t)(((itr >> 6) & 0x03) << 4);   /* blue  0..48 */
    }
}


/* Minimal manual boot used when no game assets are present: pages + synthetic palette only. */
static void Demo_Boot_Synthetic(void)
{
    screen_pixel_width = PLATFORM_SCREEN_WIDTH;
    screen_pixel_height = PLATFORM_SCREEN_HEIGHT;
    screen_pixel_size = screen_pixel_width * screen_pixel_height;

    demo_video_memory = (uint8_t *)malloc(4 * 64 * 1024);
    assert(demo_video_memory != NULL);
    memset(demo_video_memory, 0, (4 * 64 * 1024));
    video_page_buffer[0] = demo_video_memory + (0 * 64 * 1024);
    video_page_buffer[1] = demo_video_memory + (1 * 64 * 1024);
    video_page_buffer[2] = demo_video_memory + (2 * 64 * 1024);
    video_page_buffer[3] = demo_video_memory + (3 * 64 * 1024);
    draw_page_num = 0;
    current_video_page = video_page_buffer[draw_page_num];

    demo_palette = (uint8_t *)malloc(768);
    assert(demo_palette != NULL);
    current_palette = demo_palette;
    Demo_Build_Palette();

    /* "None" cursor: keeps Check_Mouse_Shape / Draw_Mouse_Off_Page safe with no sprite data. */
    Set_Mouse_List(1, mouse_list_none);
}


void Demo_Boot(void)
{
    Startup_Platform();

    if(Demo_File_Exists(demo_font_file))
    {
        /* Real assets present: let the engine's own driver init set up video pages, fonts, palette,
           and mouse from FONTS.LBX.  Sound params are inert (audio init is not wired up here). */
        Init_Drivers(1, 2, demo_font_file, 0, -1, -1, -1, 0, -1, -1, -1);
        Reset_Window();
        demo_assets_ready = 1;
    }
    else
    {
        Demo_Boot_Synthetic();
        demo_assets_ready = 0;
    }
}


void Demo_Shutdown(void)
{
    /* In the asset path the engine owns the page/palette buffers; only free what we malloc'd. */
    free(demo_palette);
    demo_palette = NULL;

    free(demo_video_memory);
    demo_video_memory = NULL;

    Shutdown_Platform();
}


int Demo_Assets_Ready(void)
{
    return demo_assets_ready;
}


int Demo_Load_Screen(const char * lbx, int entry, int palette_entry, int page)
{
    char lbx_name[16];
    char lbx_file[20];
    SAMB_ptr picture;

    if(demo_assets_ready == 0)
    {
        return 0;
    }

    /* Existence-check first: LBX_Load() aborts the process via Exit_With_Message on a missing file. */
    stu_strcpy(lbx_file, lbx);
    strcat(lbx_file, ".LBX");
    if(Demo_File_Exists(lbx_file) == 0)
    {
        return 0;
    }

    /* LBX loaders uppercase / strip the name in place, so pass a writable copy. */
    stu_strcpy(lbx_name, lbx);
    picture = LBX_Load(lbx_name, (int16_t)entry);
    if(picture == NULL)
    {
        return 0;
    }

    current_video_page = video_page_buffer[page];

    /* FLIC_Draw is the decompressing full-picture blitter the game uses for full-screen
       backgrounds (e.g. FLIC_Draw(0,0,magic_background_seg) in MagicScr.c).  Draw_Picture is only
       for raw sprites and would mis-decode / overrun on a compressed background. */
    if(palette_entry >= 0)
    {
        Load_Palette(palette_entry, -1, 0);
        /* Build the shading/remap tables for the current palette; without this, picture pixels that
           use remap indices (>= 233) decode to garbage "splotches" (the game calls this after
           Load_Palette during screen setup, e.g. Combat.c / LoadScr.c). */
        Calculate_Remap_Colors();
    }
    FLIC_Draw(0, 0, picture);

    return 1;
}


void * Demo_Lbx_Load(const char * lbx, int entry)
{
    char lbx_name[16];
    char lbx_file[20];

    if(demo_assets_ready == 0)
    {
        return NULL;
    }

    /* Existence-check first: LBX_Load() aborts the process via Exit_With_Message on a missing file. */
    stu_strcpy(lbx_file, lbx);
    strcat(lbx_file, ".LBX");
    if(Demo_File_Exists(lbx_file) == 0)
    {
        return NULL;
    }

    stu_strcpy(lbx_name, lbx);   /* writable copy; LBX loaders uppercase / strip the name in place */
    return (void *)LBX_Load(lbx_name, (int16_t)entry);
}


void Demo_Fill_Pattern(uint8_t * page, int phase)
{
    int x;
    int y;
    uint8_t * row;

    for(y = 0; y < screen_pixel_height; y++)
    {
        row = page + (y * screen_pixel_width);
        for(x = 0; x < screen_pixel_width; x++)
        {
            if((phase & 1) == 0)
            {
                row[x] = (uint8_t)(((x >> 2) + phase) & 0xFF);
            }
            else
            {
                row[x] = (uint8_t)(((y >> 1) + phase) & 0xFF);
            }
        }
    }
}


void Demo_Present(void)
{
    /* Apply the palette every frame so effects that never page-flip still show correct colors. */
    Platform_Palette_Update();
    Platform_Video_Update();
}


int Demo_Quit(void)
{
    Platform_Event_Handler();
    return (quit_game_flag != 0);
}


void Demo_Dwell(uint64_t millies)
{
    uint64_t deadline;

    deadline = Platform_Get_Millies() + millies;
    while((quit_game_flag == 0) && (Platform_Get_Millies() < deadline))
    {
        Platform_Event_Handler();
        Demo_Present();
        Platform_Sleep_Millies(DEMO_POLL_MILLIES);
    }
}


int Demo_Rand(int n)
{
    if(n <= 0)
    {
        return 0;
    }
    demo_rng_state = (demo_rng_state * 1103515245u) + 12345u;
    return (int)((demo_rng_state >> 16) % (uint32_t)n);
}


void Demo_Set_Args(int count, char ** args)
{
    demo_arg_count = (count > 0) ? count : 0;
    demo_args = args;
}


const char * Demo_Arg(int index)
{
    if((index >= 0) && (index < demo_arg_count))
    {
        return demo_args[index];
    }
    return NULL;
}


int Demo_Arg_Int(int index, int fallback)
{
    const char * s = Demo_Arg(index);
    return (s != NULL) ? atoi(s) : fallback;
}
