/*
    effect_scast.c — demo-test of the real Spell_Casting_Screen_Draw() (MoM SCastScr.c) from a save, with screenshots.

    Question under test - the BUGBUG note on Spell_Casting_Screen_Draw():
        Main_Screen_Draw_Unit_Action_Locked_Buttons();  // BUGBUG  ¿ isn't this completly hidden by the deselect bk and cancel butt ?
    The locked Done / Patrol / Wait / Build buttons are drawn at (246,176)..(280,186); afterwards Spell_Casting_Screen_Draw_Panel()
    draws deselect_button_blockout at (242,173), cast_background at (240,76) and cast_button_border at (240,173), and the draw ends
    with cast_cancel_button at (263,181).

    Method (deterministic, no window grabbing):
        A  the REAL Spell_Casting_Screen_Draw()
        B  the same call sequence with the Main_Screen_Draw_Unit_Action_Locked_Buttons() call left out
        C  A, then the locked buttons drawn again on top (shows where they sit)
    A and B are rendered from the same state, back to back.  Every pixel that differs between A and B is a locked-button pixel that
    survives into the final frame; zero differences means the call is completely hidden.  The differing pixels are counted for the
    whole screen and for the button area (x 240..319, y 170..199) - the map/minimap can animate between renders, the button area cannot.

    Writes into the working directory:
        scast_real.bmp           A
        scast_no_locked.bmp      B
        scast_locked_on_top.bmp  C
        scast_diff.bmp           A with every pixel that differs from B painted bright
    then shows A in the window until it is closed.

    Setup mirrors Spell_Casting_Screen__WIP() up to its input loop: full boot + SAVETEST.GAM (like `score` / `mastery`),
    Spell_Casting_Screen_Allocate, Build_Select_Target_String, Set_Unit_Draw_Priority, Set_Entities_On_Map_Window,
    Reset_Map_Draw, MainScr_Create_Reduced_Map_Picture.

    Usage:  demo_vga scast [save_slot]     (default -1 = SAVETEST.GAM)
    full_boot = 1.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/MOX_DEF.h"    /* ST_UNDEFINED, ST_TRANSPARENT */
#include "../MoX/src/MOX_TYPE.h"   /* SAMB_ptr */
#include "../MoX/src/MOM_DAT.h"    /* _map_x, _map_y, _map_plane, _prev_world_x, _prev_world_y, _current_player_idx, main_background_seg */
#include "../MoX/src/LOADSAVE.h"   /* Load_SAVE_GAM() */
#include "../MoX/src/Video.h"      /* Set_Page_Off, video_page_buffer, draw_page_num, current_video_page */
#include "../MoX/src/Video2.h"     /* screen_pixel_width, screen_pixel_height */
#include "../MoX/src/Graphics.h"   /* Reset_Window */
#include "../MoX/src/FLIC_Draw.h"  /* FLIC_Draw, Set_Animation_Frame */
#include "../MoX/src/Fonts.h"      /* Load_Palette, Calculate_Remap_Colors, Set_Outline_Color, current_palette */
#include "../MoX/src/Input.h"      /* Toggle_Pages */

#include "../MoM/src/LOADER.h"     /* Load_WZD_Resources(), cast_cancel_button */
#include "../MoM/src/LoadScr.h"    /* Loaded_Game_Update() */
#include "../MoM/src/MainScr.h"    /* Main_Screen_Draw_*, MainScr_Create_Reduced_Map_Picture */
#include "../MoM/src/MainScr_Maps.h"  /* Reset_Map_Draw, Draw_Maps, Draw_World_Window, Set_Unit_Draw_Priority, Set_Entities_On_Map_Window, Set_Draw_Active_Stack_Always */
#include "../MoM/src/SCastScr.h"   /* Spell_Casting_Screen_Draw, Spell_Casting_Screen_Allocate, Build_Select_Target_String, ... */
#include "../MoM/src/SPELLDEF.h"   /* stt_Map_Square */

#include "../src/ReMoM_Init.h"     /* ReMoM_Init_Engine() */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* SCastScr.c module globals that SCastScr.h does not export */
extern int16_t _osc_spell_target_type;
extern int16_t _osc_color_cycle_down;
extern int16_t _osc_color_cycle_up;
extern int16_t _osc_map_plane;
extern int16_t _osc_wx;
extern int16_t _osc_wy;
extern int16_t _osc_wp;

#define SCAST_PAGE_SIZE      (320 * 200)
#define SCAST_AREA_X1        240   /* button area: the locked buttons, the blockout, the border and the cancel button */
#define SCAST_AREA_Y1        170
#define SCAST_DIFF_COLOR     ((uint8_t)198)



/* 8-bit palettized BMP of a 320x200 page; palette is the engine's 6-bit DAC palette. */
static int Scast_Write_BMP(const char * file_name, const uint8_t * pixels)
{
    FILE * f;
    uint8_t header[54];
    uint8_t quad[4];
    uint32_t palette_size = (256 * 4);
    uint32_t data_offset = (54 + palette_size);
    uint32_t file_size = (data_offset + SCAST_PAGE_SIZE);
    int itr;
    int row;

    f = fopen(file_name, "wb");
    if(f == NULL)
    {
        printf("[scast] ERROR: cannot write %s\n", file_name);
        return 0;
    }

    memset(header, 0, sizeof(header));
    header[0] = 'B';
    header[1] = 'M';
    header[2] = (uint8_t)(file_size);  header[3] = (uint8_t)(file_size >> 8);  header[4] = (uint8_t)(file_size >> 16);  header[5] = (uint8_t)(file_size >> 24);
    header[10] = (uint8_t)(data_offset);  header[11] = (uint8_t)(data_offset >> 8);
    header[14] = 40;                     /* BITMAPINFOHEADER */
    header[18] = (uint8_t)(320 & 0xFF);  header[19] = (uint8_t)(320 >> 8);
    header[22] = 200;                    /* positive height = bottom-up rows */
    header[26] = 1;                      /* planes */
    header[28] = 8;                      /* bits per pixel */
    header[46] = 0;  header[47] = 1;     /* 256 palette entries */
    fwrite(header, 1, sizeof(header), f);

    for(itr = 0; itr < 256; itr++)
    {
        uint8_t r6 = current_palette[(itr * 3) + 0];
        uint8_t g6 = current_palette[(itr * 3) + 1];
        uint8_t b6 = current_palette[(itr * 3) + 2];
        quad[0] = (uint8_t)((b6 << 2) | (b6 >> 4));
        quad[1] = (uint8_t)((g6 << 2) | (g6 >> 4));
        quad[2] = (uint8_t)((r6 << 2) | (r6 >> 4));
        quad[3] = 0;
        fwrite(quad, 1, 4, f);
    }

    for(row = 199; row >= 0; row--)
    {
        fwrite(&pixels[row * 320], 1, 320, f);
    }

    fclose(f);
    printf("[scast] wrote %s\n", file_name);
    return 1;
}


/* B: Spell_Casting_Screen_Draw() with the Main_Screen_Draw_Unit_Action_Locked_Buttons() call left out - every other call as in SCastScr.c */
static void Scast_Draw_Without_Locked_Buttons(void)
{
    Reset_Window();
    Set_Page_Off();
    Reset_Map_Draw();
    Draw_Maps(MAP_SCREEN_X, MAP_SCREEN_Y, MAP_WIDTH, MAP_HEIGHT, &_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _current_player_idx);
    FLIC_Draw(0, 0, main_background_seg);
    Draw_World_Window(REDUCED_MAP_SCREEN_X, REDUCED_MAP_SCREEN_Y, REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT);
    /* Main_Screen_Draw_Unit_Action_Locked_Buttons();  -- left out */
    Main_Screen_Draw_Game_Buttons();
    Main_Screen_Draw_Status_Window();
    Spell_Casting_Screen_Draw_Panel();
    Spell_Casting_Screen_Assign_Mouse_Images();
    Set_Animation_Frame(cast_cancel_button, 0);
    FLIC_Draw(263, 181, cast_cancel_button);
}


/* Same panel color-cycle state before each render, so A and B run Spell_Casting_Screen_Draw_Panel() identically. */
static void Scast_Reset_Cycle(void)
{
    _osc_color_cycle_down = 0;
    _osc_color_cycle_up = 0;
}


static void Effect_Scast_Run(void)
{
    static uint8_t page_real[SCAST_PAGE_SIZE];
    static uint8_t page_no_locked[SCAST_PAGE_SIZE];
    static uint8_t page_on_top[SCAST_PAGE_SIZE];
    static uint8_t page_diff[SCAST_PAGE_SIZE];
    char spell_name[] = "Floating Island";
    int save_slot = Demo_Arg_Int(0, ST_UNDEFINED);
    long diff_total = 0;
    long diff_area = 0;
    int x;
    int y;

    printf("[scast] full engine boot; loading save slot %d (-1 = SAVETEST.GAM) ...\n", save_slot);

    ReMoM_Init_Engine();
    Load_WZD_Resources();
    Load_SAVE_GAM((int16_t)save_slot);
    Loaded_Game_Update();

    Load_Palette(0, ST_UNDEFINED, ST_NULL);  /* EMPERATO - main game palette, as LoadScr does before the main screen */
    Calculate_Remap_Colors();

    /* Spell_Casting_Screen__WIP() up to its input loop */
    _osc_color_cycle_down = 0;
    _osc_color_cycle_up = 0;
    _osc_spell_target_type = stt_Map_Square;
    _osc_wx = ST_UNDEFINED;
    _osc_wy = ST_UNDEFINED;
    _osc_wp = ST_UNDEFINED;
    _osc_map_plane = _map_plane;
    Set_Draw_Active_Stack_Always();
    Set_Outline_Color(ST_TRANSPARENT);
    Spell_Casting_Screen_Allocate();
    Build_Select_Target_String(_osc_spell_target_type, spell_name);
    Set_Unit_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    Spell_Casting_Screen_Assign_Mouse_Images();
    Reset_Map_Draw();
    MainScr_Create_Reduced_Map_Picture();

    /* A: the real draw */
    Scast_Reset_Cycle();
    Spell_Casting_Screen_Draw();
    memcpy(page_real, current_video_page, SCAST_PAGE_SIZE);

    /* B: without the locked buttons */
    Scast_Reset_Cycle();
    Scast_Draw_Without_Locked_Buttons();
    memcpy(page_no_locked, current_video_page, SCAST_PAGE_SIZE);

    /* C: the real draw, then the locked buttons on top */
    Scast_Reset_Cycle();
    Spell_Casting_Screen_Draw();
    Main_Screen_Draw_Unit_Action_Locked_Buttons();
    memcpy(page_on_top, current_video_page, SCAST_PAGE_SIZE);

    memcpy(page_diff, page_real, SCAST_PAGE_SIZE);
    for(y = 0; y < 200; y++)
    {
        for(x = 0; x < 320; x++)
        {
            if(page_real[(y * 320) + x] != page_no_locked[(y * 320) + x])
            {
                diff_total++;
                if((x >= SCAST_AREA_X1) && (y >= SCAST_AREA_Y1))
                {
                    diff_area++;
                    printf("[scast]   differs at (%3d,%3d): real %3u  without-locked %3u\n", x, y, page_real[(y * 320) + x], page_no_locked[(y * 320) + x]);
                }
                page_diff[(y * 320) + x] = SCAST_DIFF_COLOR;
            }
        }
    }

    Scast_Write_BMP("scast_real.bmp", page_real);
    Scast_Write_BMP("scast_no_locked.bmp", page_no_locked);
    Scast_Write_BMP("scast_locked_on_top.bmp", page_on_top);
    Scast_Write_BMP("scast_diff.bmp", page_diff);

    printf("[scast] RESULT: %ld pixel(s) differ in the button area (x>=%d, y>=%d); %ld on the whole screen\n", diff_area, SCAST_AREA_X1, SCAST_AREA_Y1, diff_total);
    if(diff_area == 0)
    {
        printf("[scast] RESULT: Main_Screen_Draw_Unit_Action_Locked_Buttons() is COMPLETELY HIDDEN in Spell_Casting_Screen_Draw()\n");
    }
    else
    {
        printf("[scast] RESULT: Main_Screen_Draw_Unit_Action_Locked_Buttons() is VISIBLE in Spell_Casting_Screen_Draw() (%ld pixels)\n", diff_area);
    }

    /* show A until the window is closed */
    memcpy(current_video_page, page_real, SCAST_PAGE_SIZE);
    Toggle_Pages();
    while(Demo_Quit() == 0)
    {
        Demo_Dwell(200);
    }
}


const Demo_Effect effect_scast =
{
    "scast",
    "Spell-casting screen draw from SAVETEST.GAM; screenshots + pixel test of the locked-buttons BUGBUG; arg [save_slot]",
    Effect_Scast_Run,
    1   /* full_boot */
};
