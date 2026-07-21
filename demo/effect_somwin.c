/*
    effect_somwin.c — direct test of the real Spell_Of_Mastery() from a save.

    Spell_Of_Mastery(player_idx) is the "someone casts the Spell of Mastery" sequence.  Called with
    the human, it's the WIN path: the wizlab Spell-of-Mastery screen animation (one pass per rival
    wizard) -> Combat_Cache_Read -> Win_Animation -> End_Of_Game_Score.  (Called with a non-human it
    instead chains into Spell_Of_Mastery_Lose -- that's the `somlose` effect.)

    It reads a whole loaded game (_num_players, _players[].casting_spell_idx / wizard_id, World_Data),
    so -- like `score` / `mastery` / `somlose` -- this uses the full_boot + save-load path and calls
    the REAL function.

    One setup fixup: Spell_Of_Mastery early-returns if magic_master_idx is already set, so clear it
    to ST_UNDEFINED before calling.  (It does its own Combat_Cache_Write, so we don't need to.)

    Interactive (ESC advances the SoM screen, then the victory animation and Hall-of-Fame score).
    It writes MAGIC.SET / COMBAT.TMP -- demo_main snapshots and restores those, so running straight
    from bin/Debug is safe.  full_boot = 1.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/LOADSAVE.h"   /* Load_SAVE_GAM() */
#include "../MoX/src/MOX_DEF.h"    /* ST_UNDEFINED, HUMAN_PLAYER_IDX */

#include "../MoM/src/LOADER.h"     /* Load_WZD_Resources() */
#include "../MoM/src/LoadScr.h"    /* Loaded_Game_Update() */
#include "../MoM/src/NEXTTURN.h"   /* magic_master_idx (early-exit guard) */
#include "../MoM/src/SPLMASTR.h"   /* Spell_Of_Mastery() */

#include "../MoX/src/Allocate.h"   /* Near_Allocate_First() */
#include "../MoX/src/MOM_DAT.h"    /* GUI_String_1 */

#include "../src/ReMoM_Init.h"     /* ReMoM_Init_Engine() */

#include <stdio.h>



static void Effect_SomWin_Run(void)
{
    printf("[somwin] full engine boot; loading SAVETEST.GAM ...\n");

    ReMoM_Init_Engine();
    Load_WZD_Resources();
    Load_SAVE_GAM(ST_UNDEFINED);   /* -1 -> SAVETEST.GAM */
    Loaded_Game_Update();

    /* Spell_Of_Mastery bails immediately if a magic master is already recorded. */
    magic_master_idx = ST_UNDEFINED;

    /* Spell_Of_Mastery's tail runs Win_Animation, whose Win_Animation_Draw stu_strcpy's the victory
       text into GUI_String_1 and hands it to Print_Centered.  GUI_String_1 is the engine's shared
       scratch string: every screen allocates its own (Near_Allocate_First(100) in AdvsrScr/CityScr/
       Combat/...), and in-game Win_Animation inherits whatever the previous screen left.  This demo
       never enters one of those screens, so allocate it the same way they do -- otherwise
       Get_String_Width dereferences NULL. */
    if(GUI_String_1 == NULL)
    {
        GUI_String_1 = (char *)Near_Allocate_First(100);
    }

    printf("[somwin] save loaded; running Spell_Of_Mastery (ESC advances)\n");
    Spell_Of_Mastery(HUMAN_PLAYER_IDX);   /* real: SoM screen -> Win_Animation -> End_Of_Game_Score */
}


const Demo_Effect effect_somwin =
{
    "somwin",
    "Spell of Mastery WIN sequence from SAVETEST.GAM (counterpart to somlose; full boot; ESC advances)",
    Effect_SomWin_Run,
    1   /* full_boot */
};
