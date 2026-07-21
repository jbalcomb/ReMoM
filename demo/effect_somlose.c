/*
    effect_somlose.c — direct test of the real Spell_Of_Mastery_Lose() from a save.

    Spell_Of_Mastery_Lose() is the full "you lost because someone else cast the Spell of Mastery"
    sequence: the 43-frame lose animation -> Combat_Cache_Read -> Lose_Animation -> End_Of_Game_Score.
    It reads a whole loaded game (players, world) and its tail needs the overland, so -- like `score`
    and `mastery` -- this uses the full_boot + save-load path and calls the REAL function.

    Two setup fixups so the real function runs from a mid-game save:
      - m_magic_winner_idx: the save has no Spell-of-Mastery caster (ST_UNDEFINED), and _Load indexes
        _players[m_magic_winner_idx], so point it at a valid wizard.
      - Combat_Cache_Write(): Spell_Of_Mastery_Lose calls Combat_Cache_Read (reads COMBAT.TMP); write
        it first (from the loaded World_Data) so the read has a file.

    Interactive (ESC advances the lose animation, then the Lose_Animation + Hall-of-Fame score) and it
    writes the SET file -- run from an isolated asset copy (see effect_score.c).  full_boot = 1.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/LOADSAVE.h"   /* Load_SAVE_GAM() */
#include "../MoX/src/MOX_DEF.h"    /* ST_UNDEFINED, HUMAN_PLAYER_IDX */

#include "../MoM/src/LOADER.h"     /* Load_WZD_Resources() */
#include "../MoM/src/LoadScr.h"    /* Loaded_Game_Update() */
#include "../MoM/src/Combat.h"     /* Combat_Cache_Write() */
#include "../MoM/src/SPLMASTR.h"   /* Spell_Of_Mastery_Lose(), m_magic_winner_idx */

#include "../src/ReMoM_Init.h"     /* ReMoM_Init_Engine() */

#include <stdio.h>

/* A valid _players[] index for the SoM caster caption (the save has none set). */
#define SOMLOSE_CASTER_IDX  1



static void Effect_SomLose_Run(void)
{
    printf("[somlose] full engine boot; loading SAVETEST.GAM ...\n");

    ReMoM_Init_Engine();
    Load_WZD_Resources();
    Load_SAVE_GAM(ST_UNDEFINED);   /* -1 -> SAVETEST.GAM */
    Loaded_Game_Update();

    /* Point the "X has cast the Spell of Mastery" caption at a valid wizard, and create the
       combat cache Spell_Of_Mastery_Lose's Combat_Cache_Read expects. */
    m_magic_winner_idx = SOMLOSE_CASTER_IDX;
    Combat_Cache_Write();

    printf("[somlose] save loaded; running Spell_Of_Mastery_Lose (ESC advances)\n");
    Spell_Of_Mastery_Lose();       /* real: lose animation -> Lose_Animation -> End_Of_Game_Score */
}


const Demo_Effect effect_somlose =
{
    "somlose",
    "Spell of Mastery LOSE sequence from SAVETEST.GAM (full boot; ESC advances). Isolated asset copy.",
    Effect_SomLose_Run,
    1   /* full_boot */
};
