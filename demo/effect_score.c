/*
    effect_score.c — direct test of the real End_Of_Game_Score() from a save.

    Unlike the other effects, this does the FULL engine boot (ReMoM_Init_Engine) rather than the
    trimmed harness boot, because End_Of_Game_Score reads a whole loaded game (players, cities,
    fortresses, spells, difficulty, turn) -- so we load real state from SAVETEST.GAM instead of
    manufacturing it.

    Boot -> Load_WZD_Resources -> Load_SAVE_GAM(-1) [SAVETEST.GAM] -> Loaded_Game_Update ->
    End_Of_Game_Score().  The real function is interactive: press ESC to advance through the score
    screen and the Hall of Fame.  It also writes the SET file (high-score upsert) into the current
    directory, so run this from an isolated copy of the assets (see the tools note) to avoid
    clobbering the real one.

    full_boot = 1: main() calls only Startup_Platform(); ReMoM_Init_Engine() does the rest.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/LOADSAVE.h"   /* Load_SAVE_GAM() */
#include "../MoX/src/MOX_DEF.h"    /* ST_UNDEFINED */

#include "../MoM/src/LOADER.h"     /* Load_WZD_Resources() */
#include "../MoM/src/LoadScr.h"    /* Loaded_Game_Update() */
#include "../MoM/src/SCORE.h"      /* End_Of_Game_Score() */

#include "../src/ReMoM_Init.h"     /* ReMoM_Init_Engine() */

#include <stdio.h>

/* ReMoM_Init.c (compiled into this target) externs MOM_FONT_FILE, normally defined in ReMoM.c. */
char MOM_FONT_FILE[] = "FONTS.LBX";



static void Effect_Score_Run(void)
{
    printf("[score] full engine boot; loading SAVETEST.GAM ...\n");

    ReMoM_Init_Engine();          /* config, drivers, data-space, MGC/TERRSTAT/SPELLDAT resources */
    Load_WZD_Resources();         /* WIZARDS.EXE resources (as scr_Continue does before a load)    */
    Load_SAVE_GAM(ST_UNDEFINED);  /* -1 -> SAVETEST.GAM                                             */
    Loaded_Game_Update();         /* post-load fixups                                               */

    printf("[score] save loaded; showing End_Of_Game_Score (ESC to advance)\n");
    End_Of_Game_Score();          /* interactive: ESC through the score + Hall of Fame             */
}


const Demo_Effect effect_score =
{
    "score",
    "End-of-game score from SAVETEST.GAM (full boot; ESC advances). Run in an isolated asset copy.",
    Effect_Score_Run,
    1   /* full_boot */
};
