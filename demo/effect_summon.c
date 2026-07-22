/*
    effect_summon.c — watch the summoning animation (MoM Summon_Animation_Draw).

    The full Summon_Animation() needs a whole game world (music, reduced map, Full_Draw_Main_Screen,
    an input loop), and Summon_Animation_Load() calls Main_Screen_Draw().  Too heavy for a harness.
    But the animation itself lives in Summon_Animation_Draw(), and the only thing that actually moves
    is the summoned figure rising (frames 0..29), gated on magic_set.spell_animations and
    _osc_player_idx == HUMAN_PLAYER_IDX.

    So this effect mirrors the sprite-loading half of Summon_Animation_Load() (via Demo_Lbx_Load),
    substitutes a real overland screen (MAIN) for the Main_Screen_Draw backdrop, sets the globals
    the real Summon_Animation_Draw reads, builds the back-buffer scene, then loops the REAL
    Summon_Animation_Draw() at the game's Set_Page_Off / Page_Flip cadence.

    Reliable only for the NEGATIVE special summon types (they avoid the unit/spell data tables):
        -1 = item enchanted,  -2 = male hero,  -3 = female hero.
    Positive creature types need those tables loaded and rely on a MONSTER.LBX figure-index formula
    the reconstruction flags as uncertain, so they are not supported here.

    Override the summon type with:  demo_vga summon <type>   (e.g. demo_vga summon -3)
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/Video.h"      /* video_page_buffer, draw_page_num, current_video_page, Set_Page_Off, Page_Flip, Copy_Off_To_Back */
#include "../MoX/src/Video2.h"     /* screen_pixel_size */
#include "../MoX/src/FLIC_Draw.h"  /* FLIC_Draw, Load_Palette_From_Animation */
#include "../MoX/src/Fonts.h"      /* Load_Palette, Apply_Palette, Calculate_Remap_Colors */
#include "../MoX/src/MOX_TYPE.h"   /* SAMB_ptr */
#include "../MoX/src/MOX_SET.h"    /* magic_set */
#include "../MoX/src/MOX_DEF.h"    /* HUMAN_PLAYER_IDX, ST_TRUE */

#include "../MoM/src/Spells137.h"  /* Summon_Animation_Draw */

#include "../platform/include/Platform.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Summon-overlay sprite segments — defined (external linkage) in MoM/src/Spells137.c, not
   header-declared.  The real Summon_Animation_Draw reads these. */
extern SAMB_ptr spell_subject_figure_seg;
extern SAMB_ptr summon_fx_back_seg;
extern SAMB_ptr summon_fx_front_seg;
extern SAMB_ptr summon_flame_frame_seg;
extern SAMB_ptr spell_caster_figure_seg;

/* Animation state (Spells137.c aliases: _osc_summon_unit_type -> _temp_sint_4,
   _osc_player_idx -> _temp_sint_1). */
extern int16_t _osc_anim_ctr;
extern int16_t _temp_sint_4;
extern int16_t _temp_sint_1;

/* SPELLSCR.LBX entries used by Summon_Animation_Load. */
#define SPELLSCR_ENTRY_FLAMEFR1   9
#define SPELLSCR_ENTRY_GLOBALMK  10
#define SPELLSCR_ENTRY_ENCHANT   11
#define RT_ARCANE                15   /* RACETYPE.h rt_Arcane */
#define SPELLSCR_PALETTE_BASE    46   /* palette flic = base + race_type */
#define SPELLSCR_WIZ_BASE        46   /* per-wizard summon flic = base + wizard_id (wizard 0 here) */

#define DEMO_SUMMON_FRAMES      130
#define DEMO_SUMMON_FRAME_MS     80   /* slowed so the rising-figure animation is watchable */



/* Negative special summon type -> MONSTER.LBX figure entry (per Summon_Animation_Load). */
static int Demo_Monster_Entry(int summon_type)
{
    if(summon_type == -1)                          { return 46; }  /* item / sword */
    if((summon_type == -2) || (summon_type == -20)) { return 45; } /* male hero    */
    if((summon_type == -3) || (summon_type == -30)) { return 44; } /* female hero  */
    return 45;
}


static void Effect_Summon_Run(void)
{
    int summon_type;
    int off_page;
    SAMB_ptr palette_flic;

    if(Demo_Assets_Ready() == 0)
    {
        printf("[summon] assets unavailable -- the summon animation needs the game LBX files\n");
        return;
    }

    summon_type = Demo_Arg_Int(0, -2);   /* default: male hero */
    if(summon_type >= 0)
    {
        printf("[summon] positive creature types are not supported (need unit/spell tables + an\n");
        printf("         uncertain figure index); using male hero (-2) instead\n");
        summon_type = -2;
    }

    /* Globals the real Summon_Animation_Draw reads. */
    magic_set.spell_animations = ST_TRUE;        /* enable the rising-figure animation */
    _temp_sint_1 = HUMAN_PLAYER_IDX;             /* _osc_player_idx: human -> figure rises */
    _temp_sint_4 = (int16_t)summon_type;         /* _osc_summon_unit_type */

    /* Load the summon-overlay sprites (mirrors Summon_Animation_Load). */
    summon_flame_frame_seg  = (SAMB_ptr)Demo_Lbx_Load("SPELLSCR", SPELLSCR_ENTRY_FLAMEFR1);
    summon_fx_back_seg  = (SAMB_ptr)Demo_Lbx_Load("SPELLSCR", SPELLSCR_ENTRY_GLOBALMK);
    summon_fx_front_seg   = (SAMB_ptr)Demo_Lbx_Load("SPELLSCR", SPELLSCR_ENTRY_ENCHANT);
    spell_caster_figure_seg = (SAMB_ptr)Demo_Lbx_Load("SPELLSCR", SPELLSCR_WIZ_BASE + 0);
    spell_subject_figure_seg        = (SAMB_ptr)Demo_Lbx_Load("MONSTER", Demo_Monster_Entry(summon_type));
    palette_flic           = (SAMB_ptr)Demo_Lbx_Load("SPELLSCR", SPELLSCR_PALETTE_BASE + RT_ARCANE);

    if((summon_flame_frame_seg == NULL) || (summon_fx_back_seg == NULL) ||
       (summon_fx_front_seg == NULL) || (spell_subject_figure_seg == NULL) || (palette_flic == NULL))
    {
        printf("[summon] could not load SPELLSCR/MONSTER assets\n");
        return;
    }

    /* Build the frozen back-buffer scene on the off page: overland backdrop + the flame frame,
       then switch to the summon palette (mirrors Summon_Animation_Load's order), then Copy_Off_To_Back. */
    off_page = 1 - draw_page_num;
    current_video_page = video_page_buffer[off_page];
    /* Neutral (black) backdrop: the real summon draws over the live overland (Main_Screen_Draw),
       which the harness can't reproduce; a static MAIN.LBX substitute showed the HUD's corner panel
       as a "wall" artifact.  Black keeps the focus on the summon overlay. */
    memset(current_video_page, 0, (size_t)screen_pixel_size);
    Load_Palette(0, -1, 0);                        /* base palette + remap tables for overlay remap pixels */
    Calculate_Remap_Colors();
    FLIC_Draw(30, 42, summon_flame_frame_seg);
    Load_Palette_From_Animation(palette_flic);     /* switch to the summon palette */
    Apply_Palette();
    Copy_Off_To_Back();                            /* off page -> frame F3 (back buffer) */

    printf("[summon] type %d -- close the window to exit\n", summon_type);

    /* Drive the real animation at the game's cadence (Set_Page_Off; Summon_Animation_Draw; flip). */
    for(_osc_anim_ctr = 0; (_osc_anim_ctr < DEMO_SUMMON_FRAMES) && (Demo_Quit() == 0); _osc_anim_ctr++)
    {
        Set_Page_Off();
        Summon_Animation_Draw();
        Page_Flip();                               /* presents (Platform_Palette_Update + _Video_Update) */
        Platform_Sleep_Millies(DEMO_SUMMON_FRAME_MS);
    }

    /* Hold the final frame until the user closes the window. */
    while(Demo_Quit() == 0)
    {
        Demo_Dwell(200);
    }
}


const Demo_Effect effect_summon =
{
    "summon",
    "Summoning animation via Summon_Animation_Draw (hero/item; type arg, e.g. -2 -3 -1)",
    Effect_Summon_Run
};
