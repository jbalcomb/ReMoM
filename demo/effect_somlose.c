/*
    effect_somlose.c — the "lost because someone else cast the Spell of Mastery" animation
    (MoM Spell_Of_Mastery_Lose).

    The full Spell_Of_Mastery_Lose() is a chain: the 43-frame lose animation -> Combat_Cache_Read ->
    Lose_Animation -> End_Of_Game_Score.  The tail (Lose_Animation, End_Of_Game_Score) is already
    covered by the `lose` and `score` effects, and Combat_Cache_Read needs a COMBAT.TMP that won't
    exist here.  So this effect shows just the distinctive part: the SoM-lose animation itself
    (Spell_Of_Mastery_Lose_Load + the Spell_Of_Mastery_Lose_Draw loop) -- the wizard's sphere
    wobble-collapses via Shear_Bitmap_Y + Scale_Bitmap + Vanish_Bitmap.

    No world state -- trimmed boot.  Only setup: _screen_seg (chained scratch buffer the load uses),
    the wizard id, and GAME_SoM_Cast_By (whose name shows in the caption).

    Choose the losing wizard with:  demo_vga somlose <wizard_id>   (0..13)
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/Video.h"      /* Set_Page_Off, Page_Flip, video pages */
#include "../MoX/src/Allocate.h"   /* Allocate_Space() */
#include "../MoX/src/Keyboard.h"   /* Keyboard_Status, Read_Key (ESC) */
#include "../MoX/src/MOX_TYPE.h"   /* SAMB_ptr */
#include "../MoX/src/MOX_DEF.h"    /* HUMAN_PLAYER_IDX */
#include "../MoX/src/MOM_DAT.h"    /* struct s_WIZARD, _osc_anim_ctr */
#include "../MoX/src/MOX_DAT.h"    /* _screen_seg, _players[] */

#include "../MoM/src/SPLMASTR.h"   /* Spell_Of_Mastery_Lose_Load/_Draw, GAME_SoM_Cast_By */

#include "../platform/include/Platform.h"

#include <stdint.h>
#include <stdio.h>

#define SOMLOSE_WIZARD_MAX      13
#define SOMLOSE_FRAMES          43
#define SOMLOSE_FRAME_MS        90
#define SOMLOSE_SCREEN_SEG_PARAS  6615   /* 6100 + 515, matching Allocate_Data_Space */
#define SOMLOSE_KEY_ESC         27



static void Effect_SomLose_Run(void)
{
    int wizard_id;
    int done;

    if(Demo_Assets_Ready() == 0)
    {
        printf("[somlose] assets unavailable -- needs SPELLOSE.LBX\n");
        return;
    }

    wizard_id = Demo_Arg_Int(0, 0);
    if(wizard_id < 0) { wizard_id = 0; }
    if(wizard_id > SOMLOSE_WIZARD_MAX) { wizard_id = SOMLOSE_WIZARD_MAX; }

    /* _screen_seg is the chained scratch buffer Spell_Of_Mastery_Lose_Load loads/allocates into. */
    if(_screen_seg == NULL)
    {
        _screen_seg = Allocate_Space(SOMLOSE_SCREEN_SEG_PARAS);
    }

    /* _players is zeroed by the harness; pick the losing wizard and whose name captions the screen. */
    _players[HUMAN_PLAYER_IDX].wizard_id = (uint8_t)wizard_id;
    GAME_SoM_Cast_By = HUMAN_PLAYER_IDX;

    Spell_Of_Mastery_Lose_Load((int16_t)wizard_id);

    printf("[somlose] wizard %d -- Spell of Mastery lose animation; ESC / close to exit\n", wizard_id);

    /* Drive the real draw at the game's cadence; self-terminates at 43 frames. */
    done = 0;
    for(_osc_anim_ctr = 0; (_osc_anim_ctr < SOMLOSE_FRAMES) && (done == 0) && (Demo_Quit() == 0); _osc_anim_ctr++)
    {
        while(Keyboard_Status())
        {
            if(Read_Key() == SOMLOSE_KEY_ESC) { done = 1; }
        }

        Set_Page_Off();
        Spell_Of_Mastery_Lose_Draw();
        Page_Flip();
        Platform_Sleep_Millies(SOMLOSE_FRAME_MS);
    }

    /* Hold the final frame until ESC / the window is closed. */
    while((done == 0) && (Demo_Quit() == 0))
    {
        while(Keyboard_Status())
        {
            if(Read_Key() == SOMLOSE_KEY_ESC) { done = 1; }
        }
        Demo_Dwell(200);
    }
}


const Demo_Effect effect_somlose =
{
    "somlose",
    "Spell of Mastery LOSE animation (Spell_Of_Mastery_Lose_Draw); wizard arg 0-13, ESC exits",
    Effect_SomLose_Run,
    0   /* trimmed boot */
};
