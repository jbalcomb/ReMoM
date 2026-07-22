/*
    effect_collage.c — debug inspector for the summon animation elements.

    Left: a COMPOSITE panel showing all elements assembled the way Summon_Animation_Draw lays them out
    (flame frame, wizard, summoning circle, monster, enchant sparkle).
    Right: each element on its own, in a labelled thumbnail panel.

    Everything is drawn through the same Clipped_Draw + Set_Window path the real summon uses, so if the
    composite skews but a given element's thumbnail is clean, the artifact is in the compositing /
    positioning; if an element's own thumbnail skews too, it's that sprite's data (step frames to find
    which one).

    SPACE steps the animation frame (clamped per sprite by its FLIC frame count).
    ESC or closing the window exits.  Manual visual-test tool, not gameplay.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/Video.h"      /* current_video_page, video_page_buffer, draw_page_num */
#include "../MoX/src/Graphics.h"   /* Fill, UU_VGA_DrawRect, Set_Window, Reset_Window */
#include "../MoX/src/FLIC_Draw.h"  /* Clipped_Draw, Set_Animation_Frame, FLIC_Get_FrameCount */
#include "../MoX/src/Fonts.h"      /* Load_Palette, Apply_Palette, Calculate_Remap_Colors, Print_Centered */
#include "../MoX/src/Keyboard.h"   /* Keyboard_Status, Read_Key */
#include "../MoX/src/MOX_TYPE.h"   /* SAMB_ptr */
#include "../MoX/src/MOM_DAT.h"    /* GUI_String_1 (defensively allocated for the font system) */

#include "../platform/include/Platform.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define COLLAGE_KEY_SPACE  ' '
#define COLLAGE_KEY_ESC    27
#define COLLAGE_BORDER     0xFF

/* Element indices (also the thumbnail order). */
#define IDX_FLAME    0
#define IDX_CIRCLE   1
#define IDX_ENCHANT  2
#define IDX_WIZARD   3
#define IDX_MONSTER  4

/* Composite panel (left) and thumbnail grid (right). */
#define COMP_X    0
#define COMP_Y    0
#define COMP_W  160
#define COMP_H  200
#define THUMB_X0    162
#define THUMB_COLS    2
#define THUMB_W      79
#define THUMB_H      66



struct Collage_Elem
{
    const char * name;
    const char * lbx;
    int          entry;
    int          off_x;   /* position within the composite, relative to the summon start (30,42) */
    int          off_y;
};

/* The summon animation's drawable elements, with their Summon_Animation_Draw layout offsets. */
static const struct Collage_Elem g_collage[] =
{
    { "FLAME",   "SPELLSCR",  9,  0,  0 },   /* FLIC_Draw(30,42) -> rel (0,0)   */
    { "CIRCLE",  "SPELLSCR", 10, 55, 54 },   /* GLOBALMK, rel (55,54)           */
    { "ENCHANT", "SPELLSCR", 11, 64, 80 },   /* rel (64,80)                     */
    { "WIZARD",  "SPELLSCR", 46,  8,  3 },   /* rel (8,3), Clipped_Draw         */
    { "MONSTER", "MONSTER",  45, 76, 18 },   /* rel (76,18), Clipped_Draw       */
};
#define COLLAGE_COUNT  ((int)(sizeof(g_collage) / sizeof(g_collage[0])))

/* Composite draw order: back-to-front, matching Summon_Animation_Draw. */
static const int g_composite_order[COLLAGE_COUNT] =
{
    IDX_FLAME, IDX_WIZARD, IDX_CIRCLE, IDX_MONSTER, IDX_ENCHANT
};

static SAMB_ptr g_sprite[COLLAGE_COUNT];



static void Collage_Draw(int frame)
{
    int i;

    Fill(0, 0, 319, 199, 0);

    /* Set each sprite's current frame once (clamped to its own frame count). */
    for(i = 0; i < COLLAGE_COUNT; i++)
    {
        if(g_sprite[i] != NULL)
        {
            int fc = FLIC_Get_FrameCount(g_sprite[i]);
            Set_Animation_Frame(g_sprite[i], (int16_t)((fc > 0) ? (frame % fc) : 0));
        }
    }

    /* --- Composite (left): all elements assembled at their summon offsets. --- */
    UU_VGA_DrawRect(COMP_X, COMP_Y, COMP_W, COMP_H, COLLAGE_BORDER);
    Print_Centered(COMP_X + (COMP_W / 2), COMP_Y + 2, "COMPOSITE");
    {
        int bx = COMP_X + 6;
        int by = COMP_Y + 16;
        Set_Window(COMP_X + 2, COMP_Y + 12, COMP_X + COMP_W - 2, COMP_Y + COMP_H - 2);
        for(i = 0; i < COLLAGE_COUNT; i++)
        {
            int e = g_composite_order[i];
            if(g_sprite[e] != NULL)
            {
                Clipped_Draw(bx + g_collage[e].off_x, by + g_collage[e].off_y, g_sprite[e]);
            }
        }
        Reset_Window();
    }

    /* --- Thumbnails (right): each element alone. --- */
    for(i = 0; i < COLLAGE_COUNT; i++)
    {
        int col = i % THUMB_COLS;
        int row = i / THUMB_COLS;
        int x0 = THUMB_X0 + (col * THUMB_W);
        int y0 = row * THUMB_H;
        char label[40];
        int fc;
        int f;

        UU_VGA_DrawRect(x0, y0, THUMB_W, THUMB_H, COLLAGE_BORDER);

        if(g_sprite[i] == NULL)
        {
            Print_Centered(x0 + (THUMB_W / 2), y0 + 28, "(missing)");
            continue;
        }

        fc = FLIC_Get_FrameCount(g_sprite[i]);
        f = (fc > 0) ? (frame % fc) : 0;
        snprintf(label, sizeof(label), "%s %d/%d", g_collage[i].name, f, fc);
        Print_Centered(x0 + (THUMB_W / 2), y0 + 2, label);

        Set_Window(x0 + 2, y0 + 11, x0 + THUMB_W - 2, y0 + THUMB_H - 2);
        Clipped_Draw(x0 + 4, y0 + 13, g_sprite[i]);
        Reset_Window();
    }
}


static void Effect_Collage_Run(void)
{
    int i;
    int frame;
    int redraw;
    int done;

    if(Demo_Assets_Ready() == 0)
    {
        printf("[collage] assets unavailable -- needs the game LBX files\n");
        return;
    }

    /* Palette 0 keeps the font legible; the skew is geometric, so colour doesn't matter here. */
    Load_Palette(0, -1, 0);
    Calculate_Remap_Colors();
    Apply_Palette();

    /* Some font paths scribble through GUI_String_1 (a char* the game normally allocates). */
    if(GUI_String_1 == NULL)
    {
        GUI_String_1 = (char *)malloc(100);
    }

    for(i = 0; i < COLLAGE_COUNT; i++)
    {
        g_sprite[i] = (SAMB_ptr)Demo_Lbx_Load(g_collage[i].lbx, g_collage[i].entry);
    }

    printf("[collage] SPACE = next frame, ESC / close = exit\n");

    frame = 0;
    redraw = 1;
    done = 0;
    while((done == 0) && (Demo_Quit() == 0))
    {
        while(Keyboard_Status())
        {
            uint8_t k = Read_Key();
            if(k == COLLAGE_KEY_SPACE)
            {
                frame++;
                redraw = 1;
            }
            else if(k == COLLAGE_KEY_ESC)
            {
                done = 1;
            }
        }

        if(redraw)
        {
            current_video_page = video_page_buffer[draw_page_num];
            Collage_Draw(frame);
            Demo_Present();
            redraw = 0;
        }

        Platform_Sleep_Millies(16);
    }
}


const Demo_Effect effect_collage =
{
    "collage",
    "Debug: composite + per-element panels for the summon animation; SPACE steps frames, ESC exits",
    Effect_Collage_Run
};
