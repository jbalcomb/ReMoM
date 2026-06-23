/*
    effect_summon.c — watch the summoning animation (MoM IDK_SummonAnim_Draw).

    The full IDK_SummonAnim() needs a whole game world (music, reduced map, Full_Draw_Main_Screen,
    an input loop), and IDK_SummonAnim_Load() calls Main_Screen_Draw().  Too heavy for a harness.
    But the animation itself lives in IDK_SummonAnim_Draw(), and the only thing that actually moves
    is the summoned figure rising (frames 0..29), gated on magic_set.spell_animations and
    _osc_player_idx == HUMAN_PLAYER_IDX.

    So this effect mirrors the sprite-loading half of IDK_SummonAnim_Load() (via Demo_Lbx_Load),
    substitutes a real overland screen (MAIN) for the Main_Screen_Draw backdrop, sets the globals
    the real IDK_SummonAnim_Draw reads, builds the back-buffer scene, then loops the REAL
    IDK_SummonAnim_Draw() at the game's Set_Page_Off / Page_Flip cadence.

    Reliable only for the NEGATIVE special summon types (they avoid the unit/spell data tables):
        -1 = item enchanted,  -2 = male hero,  -3 = female hero.
    Positive creature types need those tables loaded and rely on a MONSTER.LBX figure-index formula
    the reconstruction flags as uncertain, so they are not supported here.

    Override the summon type with:  demo_vga summon <type>   (e.g. demo_vga summon -3)
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/Video.h"      /* video_page_buffer, draw_page_num, current_video_page, Set_Page_Off, Page_Flip, Copy_Off_To_Back */
#include "../MoX/src/FLIC_Draw.h"  /* FLIC_Draw, Load_Palette_From_Animation */
#include "../MoX/src/Fonts.h"      /* Apply_Palette */
#include "../MoX/src/MOX_TYPE.h"   /* SAMB_ptr */
#include "../MoX/src/MOX_SET.h"    /* magic_set */
#include "../MoX/src/MOX_DEF.h"    /* HUMAN_PLAYER_IDX, ST_TRUE */

#include "../MoM/src/Spells137.h"  /* IDK_SummonAnim_Draw */

#include "../platform/include/Platform.h"

#include <stdint.h>
#include <stdio.h>

/* Summon-overlay sprite segments — defined (external linkage) in MoM/src/Spells137.c, not
   header-declared.  The real IDK_SummonAnim_Draw reads these. */
extern SAMB_ptr IDK_MONSTER_seg;
extern SAMB_ptr SPELLSCR_GLOBALMK_seg;
extern SAMB_ptr SPELLSCR_ENCHANT_seg;
extern SAMB_ptr SPELLSCR_FLAMEFR1_seg;
extern SAMB_ptr IDK_wizard_id_thing_seg;

/* Animation state (Spells137.c aliases: _osc_summon_unit_type -> _temp_sint_4,
   _osc_player_idx -> _temp_sint_1). */
extern int16_t _osc_anim_ctr;
extern int16_t _temp_sint_4;
extern int16_t _temp_sint_1;

/* SPELLSCR.LBX entries used by IDK_SummonAnim_Load. */
#define SPELLSCR_ENTRY_FLAMEFR1   9
#define SPELLSCR_ENTRY_GLOBALMK  10
#define SPELLSCR_ENTRY_ENCHANT   11
#define RT_ARCANE                15   /* RACETYPE.h rt_Arcane */
#define SPELLSCR_PALETTE_BASE    46   /* palette flic = base + race_type */
#define SPELLSCR_WIZ_BASE        46   /* per-wizard summon flic = base + wizard_id (wizard 0 here) */

#define DEMO_SUMMON_FRAMES      130
#define DEMO_SUMMON_FRAME_MS     40   /* ~25 fps */



/* Negative special summon type -> MONSTER.LBX figure entry (per IDK_SummonAnim_Load). */
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

    /* Globals the real IDK_SummonAnim_Draw reads. */
    magic_set.spell_animations = ST_TRUE;        /* enable the rising-figure animation */
    _temp_sint_1 = HUMAN_PLAYER_IDX;             /* _osc_player_idx: human -> figure rises */
    _temp_sint_4 = (int16_t)summon_type;         /* _osc_summon_unit_type */

    /* Load the summon-overlay sprites (mirrors IDK_SummonAnim_Load). */
    SPELLSCR_FLAMEFR1_seg  = (SAMB_ptr)Demo_Lbx_Load("SPELLSCR", SPELLSCR_ENTRY_FLAMEFR1);
    SPELLSCR_GLOBALMK_seg  = (SAMB_ptr)Demo_Lbx_Load("SPELLSCR", SPELLSCR_ENTRY_GLOBALMK);
    SPELLSCR_ENCHANT_seg   = (SAMB_ptr)Demo_Lbx_Load("SPELLSCR", SPELLSCR_ENTRY_ENCHANT);
    IDK_wizard_id_thing_seg = (SAMB_ptr)Demo_Lbx_Load("SPELLSCR", SPELLSCR_WIZ_BASE + 0);
    IDK_MONSTER_seg        = (SAMB_ptr)Demo_Lbx_Load("MONSTER", Demo_Monster_Entry(summon_type));
    palette_flic           = (SAMB_ptr)Demo_Lbx_Load("SPELLSCR", SPELLSCR_PALETTE_BASE + RT_ARCANE);

    if((SPELLSCR_FLAMEFR1_seg == NULL) || (SPELLSCR_GLOBALMK_seg == NULL) ||
       (SPELLSCR_ENCHANT_seg == NULL) || (IDK_MONSTER_seg == NULL) || (palette_flic == NULL))
    {
        printf("[summon] could not load SPELLSCR/MONSTER assets\n");
        return;
    }

    /* Build the frozen back-buffer scene on the off page: overland backdrop + the flame frame,
       then switch to the summon palette (mirrors IDK_SummonAnim_Load's order), then Copy_Off_To_Back. */
    off_page = 1 - draw_page_num;
    Demo_Load_Screen("MAIN", 0, 0, off_page);     /* MAIN backdrop + game palette 0 + remap */
    current_video_page = video_page_buffer[off_page];
    FLIC_Draw(30, 42, SPELLSCR_FLAMEFR1_seg);
    Load_Palette_From_Animation(palette_flic);     /* switch to the summon palette */
    Apply_Palette();
    Copy_Off_To_Back();                            /* off page -> frame F3 (back buffer) */

    printf("[summon] type %d -- close the window to exit\n", summon_type);

    /* Drive the real animation at the game's cadence (Set_Page_Off; IDK_SummonAnim_Draw; flip). */
    for(_osc_anim_ctr = 0; (_osc_anim_ctr < DEMO_SUMMON_FRAMES) && (Demo_Quit() == 0); _osc_anim_ctr++)
    {
        Set_Page_Off();
        IDK_SummonAnim_Draw();
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
    "Summoning animation via IDK_SummonAnim_Draw (hero/item; type arg, e.g. -2 -3 -1)",
    Effect_Summon_Run
};
