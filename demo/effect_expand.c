/*
    effect_expand.c — demo of the 'Expanding Box' / 'Expanding Box Lines' popup-grow effect (MoX GENDRAW.c).

    Draw_Expanding_Box_Picture() captures a picture area of the off page and grows it out of a small origin rectangle (the
    thing that was clicked: a map square, a wizard gem, a city) into its box over EXPANDING_BOX_STAGES page flips, optionally
    with Draw_Expanding_Box_Lines() sweeping dashed outlines from the origin to each stage.  Redraw_Expanding_Box_Lines()
    re-draws the final growth lines so the screen's own draw function can keep them on screen.

    Scenes (each modeled on a real call site; the call is printed on the caption strip):
        mirror     Mirror_Screen()           Draw_Expanding_Box(... ST_TRUE)            BACKGRND.LBX 4, grows from a wizard gem
        unitlist   Unit_List_Window()        Draw_Expanding_Box(... ST_TRUE)            UNITVIEW.LBX 28/29, grows from a map square
        unitstat   Unit_Statistics_Popup()   Draw_Expanding_Box_Picture(... ST_TRUE)    UNITVIEW.LBX 1, picture 282 wide, box/lines 217 wide
        city       Cast_Spell_City_Enchantment_Animation_1  Draw_Expanding_Box(... ST_FALSE)   no growth lines
        helpoff    magic_set.expanding_help == ST_FALSE  -> Draw_Expanding_Box_Picture returns at once, the popup just appears
        lines      Draw_Expanding_Box_Lines() alone, origin swept around a fixed box (above/level/below x left/inside/right)
                   with the game's colors and with alternate color pairs

    Usage:  demo_vga expand [scene]      (no scene = cycle through all of them; ESC skips the current scene)

    Setup beyond the harness boot: Allocate_Data_Space() for _screen_seg (the box picture is allocated in it); _VGAFILEH_seg
    (the picture's restore copy) comes from the platform EMS startup inside Init_Drivers.  Needs the game LBX files.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/MOX_TYPE.h"   /* SAMB_ptr */
#include "../MoX/src/MOX_DEF.h"    /* ST_TRUE, ST_FALSE */
#include "../MoX/src/MOX_SET.h"    /* magic_set.expanding_help */
#include "../MoX/src/MOM_DAT.h"    /* World_Data */
#include "../MoX/src/Video.h"      /* Set_Page_Off */
#include "../MoX/src/Graphics.h"   /* Fill, Set_Window, Reset_Window, UU_VGA_DrawRect */
#include "../MoX/src/FLIC_Draw.h"  /* FLIC_Draw, Clipped_Draw, Load_Palette_From_Animation */
#include "../MoX/src/Fonts.h"      /* Load_Palette, Calculate_Remap_Colors, Set_Font_Style_Shadow_Down, Print, Print_Centered */
#include "../MoX/src/Input.h"      /* Toggle_Pages */
#include "../MoX/src/Keyboard.h"   /* Keyboard_Status, Read_Key */
#include "../MoX/src/GENDRAW.h"    /* Draw_Expanding_Box, Draw_Expanding_Box_Picture, Redraw_Expanding_Box_Lines, Draw_Expanding_Box_Lines */

#include "../MoM/src/ALLOC.h"      /* Allocate_Data_Space() */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define EXPAND_DATA_SPACE_PARAS  6100   /* same as ReMoM_Init_Engine's Allocate_Data_Space(6100) */
#define EXPAND_HOLD_MILLIES      1600   /* hold the opened popup */
#define EXPAND_PAUSE_MILLIES      700   /* hold the bare screen + origin marker before the grow */
#define EXPAND_LINES_MILLIES      450   /* per origin position in the 'lines' scene */
#define EXPAND_CAPTION_Y          190   /* caption strip: y 190..199 */
#define EXPAND_MARKER_COLOR       198   /* origin marker outline */
#define EXPAND_CAPTION_COLOR       24   /* caption strip / panel fill */
#define EXPAND_KEY_ESCAPE          27

static SAMB_ptr expand_main_background_seg = ST_NULL;      /* MAIN.LBX 0 - the screen the popups open over */
static SAMB_ptr expand_mirror_background_seg = ST_NULL;    /* BACKGRND.LBX 4 - Mirror_Screen */
static SAMB_ptr expand_unitlist_background_seg = ST_NULL;  /* UNITVIEW.LBX 28 - Unit_List_Window */
static SAMB_ptr expand_unitlist_bottom_seg = ST_NULL;      /* UNITVIEW.LBX 29 - Unit_List_Window */
static SAMB_ptr expand_unitview_background_seg = ST_NULL;  /* UNITVIEW.LBX 1 - Unit_Statistics_Popup */

static int expand_skip = 0;   /* ESC pressed: finish the current scene early */



/* ESC skips the rest of the current scene. */
static int Expand_Skip_Requested(void)
{
    while(Keyboard_Status())
    {
        if(Read_Key() == EXPAND_KEY_ESCAPE)
        {
            expand_skip = 1;
        }
    }
    return (expand_skip != 0) || (Demo_Quit() != 0);
}


/* Wait 'millies' unless the user skips; presents the on page meanwhile. */
static void Expand_Hold(uint64_t millies)
{
    if(Expand_Skip_Requested() == 0)
    {
        Demo_Dwell(millies);
    }
}


static void Expand_Main_Palette(void)
{
    Load_Palette(0, -1, 0);
    Calculate_Remap_Colors();
}


/* Off page: the screen the popup opens over, the origin marker, and the caption strip. */
static void Expand_Draw_Background(int16_t origin_x1, int16_t origin_y1, int16_t origin_x2, int16_t origin_y2, const char * caption_1, const char * caption_2)
{
    Set_Page_Off();
    Reset_Window();
    /* MAIN.LBX 0 leaves the map area transparent, so clear first or the previous scene's popup and lines show through */
    Fill(0, 0, 319, 199, 0);
    if(expand_main_background_seg != ST_NULL)
    {
        FLIC_Draw(0, 0, expand_main_background_seg);
    }
    if(origin_x2 >= origin_x1)
    {
        UU_VGA_DrawRect(origin_x1, origin_y1, ((origin_x2 - origin_x1) + 1), ((origin_y2 - origin_y1) + 1), EXPAND_MARKER_COLOR);
    }
    Fill(0, EXPAND_CAPTION_Y, 319, 199, EXPAND_CAPTION_COLOR);   /* color set 15 prints dark - needs a light strip */
    Set_Font_Style_Shadow_Down(1, 15, 0, 0);
    Print(2, (EXPAND_CAPTION_Y + 1), (char *)caption_1);
    if(caption_2 != NULL)
    {
        Print(170, (EXPAND_CAPTION_Y + 1), (char *)caption_2);
    }
}


/* Show the bare screen with its origin marker for a moment, then leave the same screen on the off page for the popup to be drawn over. */
static void Expand_Show_Background(int16_t origin_x1, int16_t origin_y1, int16_t origin_x2, int16_t origin_y2, const char * caption_1, const char * caption_2)
{
    Expand_Draw_Background(origin_x1, origin_y1, origin_x2, origin_y2, caption_1, caption_2);
    Toggle_Pages();
    Expand_Hold(EXPAND_PAUSE_MILLIES);
    Expand_Draw_Background(origin_x1, origin_y1, origin_x2, origin_y2, caption_1, caption_2);
}


/* A plain framed panel for the scenes whose real popup needs game state (city enchantment). */
static void Expand_Draw_Panel(int16_t x1, int16_t y1, int16_t x2, int16_t y2, const char * title, const char * body)
{
    Fill(x1, y1, x2, y2, EXPAND_CAPTION_COLOR);
    UU_VGA_DrawRect(x1, y1, ((x2 - x1) + 1), ((y2 - y1) + 1), 186);
    UU_VGA_DrawRect((x1 + 2), (y1 + 2), ((x2 - x1) - 3), ((y2 - y1) - 3), 180);
    Set_Font_Style_Shadow_Down(4, 15, 0, 0);
    Print_Centered(((x1 + x2) / 2), (y1 + 10), (char *)title);
    Set_Font_Style_Shadow_Down(1, 15, 0, 0);
    Print_Centered(((x1 + x2) / 2), (y1 + 36), (char *)body);
}


/*
    Mirror_Screen(): Draw_Expanding_Box(mirror_start_x, mirror_start_y, (mirror_start_x + 179), (mirror_start_y + 195), OL, OT, OR, OB, ST_TRUE)
    The origin is the clicked wizard gem.  Mirror_Screen_Draw() then calls Redraw_Expanding_Box_Lines() every frame.
*/
static void Expand_Scene_Mirror(void)
{
    int16_t mirror_start_x = 75;
    int16_t mirror_start_y = 2;
    int16_t origin_x1 = 268;
    int16_t origin_y1 = 4;
    int16_t origin_x2 = (origin_x1 + 20);
    int16_t origin_y2 = (origin_y1 + 18);

    Expand_Main_Palette();
    Expand_Show_Background(origin_x1, origin_y1, origin_x2, origin_y2, "mirror: Draw_Expanding_Box", "lines ON, origin above-right");
    FLIC_Draw(mirror_start_x, mirror_start_y, expand_mirror_background_seg);

    Draw_Expanding_Box(mirror_start_x, mirror_start_y, (mirror_start_x + 179), (mirror_start_y + 195), origin_x1, origin_y1, origin_x2, origin_y2, ST_TRUE);

    /* the screen's own draw: background, Redraw_Expanding_Box_Lines(), then the popup */
    Expand_Draw_Background(origin_x1, origin_y1, origin_x2, origin_y2, "mirror: + Redraw_Expanding_Box_Lines", NULL);
    Redraw_Expanding_Box_Lines();
    FLIC_Draw(mirror_start_x, mirror_start_y, expand_mirror_background_seg);
    Toggle_Pages();
    Expand_Hold(EXPAND_HOLD_MILLIES);
}


/*
    Unit_List_Window(): Draw_Expanding_Box(m_ulw_start_x, m_ulw_start_y, (m_ulw_start_x + 238), (m_ulw_height + 5), x, y, (x + 19), (y + 16), ST_TRUE)
    The origin is the clicked map square (20 x 17).  The window height follows the troop count; Unit_List_Window_Draw() clips the
    background to it and draws the bottom strip under it.
*/
static void Expand_Scene_Unit_List(int16_t troop_count, int16_t x, int16_t y, const char * where)
{
    int16_t m_ulw_start_x = 45;
    int16_t m_ulw_start_y = 0;
    int16_t m_ulw_height = 0;
    char caption[64];

    m_ulw_height = (24 + (troop_count * 19));
    m_ulw_start_y = (97 - (m_ulw_height / 2));
    m_ulw_height = (m_ulw_start_y + m_ulw_height);

    snprintf(caption, sizeof(caption), "%d troops, origin %s", troop_count, where);

    Expand_Main_Palette();
    Expand_Show_Background(x, y, (x + 19), (y + 16), "unitlist: Draw_Expanding_Box", caption);
    Set_Window(0, 0, 319, m_ulw_height);
    Clipped_Draw(m_ulw_start_x, m_ulw_start_y, expand_unitlist_background_seg);
    Reset_Window();
    FLIC_Draw(m_ulw_start_x, m_ulw_height, expand_unitlist_bottom_seg);

    Draw_Expanding_Box(m_ulw_start_x, m_ulw_start_y, (m_ulw_start_x + 238), (m_ulw_height + 5), x, y, (x + 19), (y + 16), ST_TRUE);

    Expand_Draw_Background(x, y, (x + 19), (y + 16), "unitlist: + Redraw_Expanding_Box_Lines", caption);
    Redraw_Expanding_Box_Lines();
    Set_Window(0, 0, 319, m_ulw_height);
    Clipped_Draw(m_ulw_start_x, m_ulw_start_y, expand_unitlist_background_seg);
    Reset_Window();
    FLIC_Draw(m_ulw_start_x, m_ulw_height, expand_unitlist_bottom_seg);
    Toggle_Pages();
    Expand_Hold(EXPAND_HOLD_MILLIES);
}


/*
    Unit_Statistics_Popup(): Draw_Expanding_Box_Picture(UV_x_start, UV_y_start, (UV_x_start + 217), (UV_y_start + 184), UV_x_start, UV_y_start, (UV_x_start + 282), (UV_y_start + 184), x1, y1, x2, y2, ST_TRUE)
    The only call with a picture area different from the box: the whole 282-wide unit view grows, but the box (and so the growth
    lines) is only the 217-wide statistics panel.
*/
static void Expand_Scene_Unit_Statistics(void)
{
    int16_t UV_x_start = 19;
    int16_t UV_y_start = 4;
    int16_t x1 = 140;
    int16_t y1 = 150;
    int16_t x2 = (x1 + 19);
    int16_t y2 = (y1 + 16);

    Expand_Main_Palette();
    Load_Palette_From_Animation(expand_unitview_background_seg);
    Calculate_Remap_Colors();
    Expand_Show_Background(x1, y1, x2, y2, "unitstat: Draw_Expanding_Box_Picture", "picture 282w, box 217w");
    FLIC_Draw(UV_x_start, UV_y_start, expand_unitview_background_seg);

    Draw_Expanding_Box_Picture(UV_x_start, UV_y_start, (UV_x_start + 217), (UV_y_start + 184), UV_x_start, UV_y_start, (UV_x_start + 282), (UV_y_start + 184), x1, y1, x2, y2, ST_TRUE);

    Expand_Draw_Background(x1, y1, x2, y2, "unitstat: + Redraw_Expanding_Box_Lines", NULL);
    Redraw_Expanding_Box_Lines();
    FLIC_Draw(UV_x_start, UV_y_start, expand_unitview_background_seg);
    Toggle_Pages();
    Expand_Hold(EXPAND_HOLD_MILLIES);
}


/*
    Cast_Spell_City_Enchantment_Animation_1(): Draw_Expanding_Box(53, 38, 268, 162, OL, OT, (OL + 20), (OT + 18), ST_FALSE)
    The origin is the city's map square; no growth lines, and Redraw_Expanding_Box_Lines() then draws nothing.
*/
static void Expand_Scene_City(void)
{
    int16_t OL = 60;
    int16_t OT = 140;

    Expand_Main_Palette();
    Expand_Show_Background(OL, OT, (OL + 20), (OT + 18), "city: Draw_Expanding_Box", "lines OFF (ST_FALSE)");
    Expand_Draw_Panel(53, 38, 268, 162, "City Enchantment", "draw_expanding_box = ST_FALSE");

    Draw_Expanding_Box(53, 38, 268, 162, OL, OT, (OL + 20), (OT + 18), ST_FALSE);

    Expand_Draw_Background(OL, OT, (OL + 20), (OT + 18), "city: Redraw_Expanding_Box_Lines -> 0", NULL);
    Redraw_Expanding_Box_Lines();
    Expand_Draw_Panel(53, 38, 268, 162, "City Enchantment", "draw_expanding_box = ST_FALSE");
    Toggle_Pages();
    Expand_Hold(EXPAND_HOLD_MILLIES);
}


/* 'Expanding Help' off in the settings: Draw_Expanding_Box_Picture() only records the rectangles and returns 0 - the popup appears in one flip. */
static void Expand_Scene_Help_Off(void)
{
    uint16_t expanding_help_save = magic_set.expanding_help;
    int16_t mirror_start_x = 75;
    int16_t mirror_start_y = 2;
    int16_t origin_x1 = 268;
    int16_t origin_y1 = 4;
    int16_t origin_x2 = (origin_x1 + 20);
    int16_t origin_y2 = (origin_y1 + 18);
    int16_t result = 0;
    char caption[64];

    magic_set.expanding_help = ST_FALSE;

    Expand_Main_Palette();
    Expand_Show_Background(origin_x1, origin_y1, origin_x2, origin_y2, "helpoff: expanding_help = ST_FALSE", "no grow, no lines");
    FLIC_Draw(mirror_start_x, mirror_start_y, expand_mirror_background_seg);

    result = Draw_Expanding_Box_Picture(mirror_start_x, mirror_start_y, (mirror_start_x + 179), (mirror_start_y + 195), mirror_start_x, mirror_start_y, (mirror_start_x + 179), (mirror_start_y + 195), origin_x1, origin_y1, origin_x2, origin_y2, ST_TRUE);

    /* the box was never animated; the caller's normal flip shows the popup - the lines flag was still recorded */
    snprintf(caption, sizeof(caption), "returned %d", result);
    Expand_Draw_Background(origin_x1, origin_y1, origin_x2, origin_y2, "helpoff: popup just appears", caption);
    FLIC_Draw(mirror_start_x, mirror_start_y, expand_mirror_background_seg);
    Toggle_Pages();
    Expand_Hold(EXPAND_HOLD_MILLIES);

    magic_set.expanding_help = expanding_help_save;
}


/*
    Draw_Expanding_Box_Lines() on its own: which edges it draws depends on where the origin is - above, level with or below the
    box, then left of, inside or right of it - so the origin is swept around a fixed box.  Second pass: alternate color pairs.
*/
static void Expand_Scene_Lines(void)
{
    static const int16_t origin_xs[3] = { 8, 150, 290 };
    static const int16_t origin_ys[3] = { 4, 88, 166 };
    static const char * const row_names[3] = { "above", "level", "below" };
    static const char * const column_names[3] = { "left", "inside", "right" };
    int16_t box_x1 = 100;
    int16_t box_y1 = 60;
    int16_t box_x2 = 220;
    int16_t box_y2 = 130;
    int16_t itr_passes = 0;
    int16_t itr_rows = 0;
    int16_t itr_columns = 0;
    char caption[64];

    Expand_Main_Palette();

    for(itr_passes = 0; (itr_passes < 2) && (Expand_Skip_Requested() == 0); itr_passes++)
    {
        for(itr_rows = 0; (itr_rows < 3) && (Expand_Skip_Requested() == 0); itr_rows++)
        {
            for(itr_columns = 0; (itr_columns < 3) && (Expand_Skip_Requested() == 0); itr_columns++)
            {
                int16_t ox = origin_xs[itr_columns];
                int16_t oy = origin_ys[itr_rows];

                snprintf(caption, sizeof(caption), "origin %s / %s%s", row_names[itr_rows], column_names[itr_columns], (itr_passes == 0) ? "" : " (alt colors)");
                Expand_Draw_Background(ox, oy, (ox + 19), (oy + 16), "lines: Draw_Expanding_Box_Lines", caption);

                if(itr_passes == 0)
                {
                    /* the colors Draw_Expanding_Box_Picture() and Redraw_Expanding_Box_Lines() pass */
                    Draw_Expanding_Box_Lines(ox, oy, (ox + 19), (oy + 16), box_x1, box_y1, box_x2, box_y2, 180, 181, 185, 186, 187, 186);
                }
                else
                {
                    Draw_Expanding_Box_Lines(ox, oy, (ox + 19), (oy + 16), box_x1, box_y1, box_x2, box_y2, 198, 15, 201, 202, 214, 15);
                }

                UU_VGA_DrawRect(box_x1, box_y1, ((box_x2 - box_x1) + 1), ((box_y2 - box_y1) + 1), 15);
                Toggle_Pages();
                Expand_Hold(EXPAND_LINES_MILLIES);
            }
        }
    }
}


typedef struct Expand_Scene
{
    const char * name;
    void (*run)(void);
} Expand_Scene;

static void Expand_Scene_Unit_List_All(void)
{
    Expand_Scene_Unit_List(1, 230, 150, "below-right");
    if(Expand_Skip_Requested() == 0)
    {
        Expand_Scene_Unit_List(3, 20, 20, "above-left");
    }
    if(Expand_Skip_Requested() == 0)
    {
        Expand_Scene_Unit_List(6, 150, 90, "inside");
    }
}

static const Expand_Scene expand_scenes[] =
{
    { "mirror",   Expand_Scene_Mirror },
    { "unitlist", Expand_Scene_Unit_List_All },
    { "unitstat", Expand_Scene_Unit_Statistics },
    { "city",     Expand_Scene_City },
    { "helpoff",  Expand_Scene_Help_Off },
    { "lines",    Expand_Scene_Lines }
};
#define EXPAND_SCENE_COUNT  ((int)(sizeof(expand_scenes) / sizeof(expand_scenes[0])))



static SAMB_ptr Expand_Load(const char * lbx, int entry)
{
    SAMB_ptr picture = (SAMB_ptr)Demo_Lbx_Load(lbx, entry);
    if(picture == ST_NULL)
    {
        printf("[expand] could not load %s.LBX entry %d\n", lbx, entry);
    }
    return picture;
}


static void Effect_Expand_Run(void)
{
    const char * only = Demo_Arg(0);
    int itr_scenes = 0;
    int found = 0;

    if(Demo_Assets_Ready() == 0)
    {
        printf("[expand] assets unavailable -- needs the game LBX files (MAIN, BACKGRND, UNITVIEW)\n");
        return;
    }

    if(only != NULL)
    {
        for(itr_scenes = 0; itr_scenes < EXPAND_SCENE_COUNT; itr_scenes++)
        {
            if(strcmp(only, expand_scenes[itr_scenes].name) == 0)
            {
                found = 1;
            }
        }
        if(found == 0)
        {
            printf("[expand] unknown scene '%s'; scenes:", only);
            for(itr_scenes = 0; itr_scenes < EXPAND_SCENE_COUNT; itr_scenes++)
            {
                printf(" %s", expand_scenes[itr_scenes].name);
            }
            printf("\n");
            return;
        }
    }

    /* Draw_Expanding_Box_Picture allocates the box picture in _screen_seg; the trimmed boot skips the data-space init. */
    if(World_Data == NULL)
    {
        Allocate_Data_Space(EXPAND_DATA_SPACE_PARAS);
    }

    expand_main_background_seg = Expand_Load("MAIN", 0);
    expand_mirror_background_seg = Expand_Load("BACKGRND", 4);
    expand_unitlist_background_seg = Expand_Load("UNITVIEW", 28);
    expand_unitlist_bottom_seg = Expand_Load("UNITVIEW", 29);
    expand_unitview_background_seg = Expand_Load("UNITVIEW", 1);
    if((expand_mirror_background_seg == ST_NULL) || (expand_unitlist_background_seg == ST_NULL) || (expand_unitlist_bottom_seg == ST_NULL) || (expand_unitview_background_seg == ST_NULL))
    {
        return;
    }

    magic_set.expanding_help = ST_TRUE;

    printf("[expand] %s -- ESC skips a scene; close the window to exit\n", (only != NULL) ? only : "all scenes");

    while(Demo_Quit() == 0)
    {
        for(itr_scenes = 0; (itr_scenes < EXPAND_SCENE_COUNT) && (Demo_Quit() == 0); itr_scenes++)
        {
            if((only != NULL) && (strcmp(only, expand_scenes[itr_scenes].name) != 0))
            {
                continue;
            }
            expand_skip = 0;
            printf("[expand] scene: %s\n", expand_scenes[itr_scenes].name);
            expand_scenes[itr_scenes].run();
        }
    }
}


const Demo_Effect effect_expand =
{
    "expand",
    "Expanding Box / Lines popup-grow (Mirror, Unit List, Unit Stats, city, help-off, bare lines); arg [scene]",
    Effect_Expand_Run,
    0   /* trimmed boot */
};
