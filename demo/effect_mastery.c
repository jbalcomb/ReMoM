/*
    effect_mastery.c — direct test of the real Cast_Spell_Of_Mastery() from a save.

    Like `score`, this needs a whole loaded game: Cast_Spell_Of_Mastery reads _players[].name and,
    when the casting animation ends, calls Allocate_Reduced_Map + Main_Screen_Draw (the overland,
    which needs a loaded world).  So it uses the same full_boot + save-load path and calls the real
    function on real state from SAVETEST.GAM.

    Boot -> Load_WZD_Resources -> Load_SAVE_GAM(-1) [SAVETEST.GAM] -> Loaded_Game_Update ->
    Cast_Spell_Of_Mastery(HUMAN_PLAYER_IDX).  The casting animation self-terminates (_combat_wx < 200)
    or on ESC, then returns to the overland; the effect then holds until the window is closed.

    Run from an isolated copy of the assets (the boot / game code may write files) -- see effect_score.c.
    MOM_FONT_FILE is defined once in effect_score.c (same target).
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/LOADSAVE.h"    /* Load_SAVE_GAM() */
#include "../MoX/src/MOX_DEF.h"     /* ST_UNDEFINED, HUMAN_PLAYER_IDX */

#include "../MoM/src/LOADER.h"      /* Load_WZD_Resources() */
#include "../MoM/src/LoadScr.h"     /* Loaded_Game_Update() */
#include "../MoM/src/SPLMASTR.h"    /* Cast_Spell_Of_Mastery() */

#include "../src/ReMoM_Init.h"      /* ReMoM_Init_Engine() */

#include <stdio.h>



static void Effect_Mastery_Run(void)
{
    printf("[mastery] full engine boot; loading SAVETEST.GAM ...\n");

    ReMoM_Init_Engine();
    Load_WZD_Resources();
    Load_SAVE_GAM(ST_UNDEFINED);   /* -1 -> SAVETEST.GAM */
    Loaded_Game_Update();

    printf("[mastery] save loaded; casting Spell of Mastery (ESC to skip)\n");
    Cast_Spell_Of_Mastery(HUMAN_PLAYER_IDX);   /* self-terminating; returns to the overland */

    /* Hold the final frame until the user closes the window. */
    while(Demo_Quit() == 0)
    {
        Demo_Dwell(200);
    }
}


const Demo_Effect effect_mastery =
{
    "mastery",
    "Spell of Mastery casting animation from SAVETEST.GAM (full boot; ESC skips). Isolated asset copy.",
    Effect_Mastery_Run,
    1   /* full_boot */
};
