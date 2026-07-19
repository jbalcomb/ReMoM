/*
    effect_lose.c — the defeat animation (MoM Lose_Animation).

    Unlike the summon animation, Lose_Animation() is self-contained: no Main_Screen_Draw / game
    world, and its loop self-terminates (m_conquest_anim_stage > 45).  So this effect just calls the
    REAL Lose_Animation() and lets it run.

    It shows the defeated wizard (hands up) falling into the limbo vortex, scaling down and rotating
    over 27 steps, over the VORTEX file-animation background, under the LOSE palette (Load_Palette 6).

    The only setup beyond the harness boot is _screen_seg (the chained-allocation scratch buffer that
    Lose_Animation loads the wizard image into) and picking which wizard's defeat image plays.

    Choose the wizard with:  demo_vga lose <wizard_id>   (0..13; LOSE.LBX entry 1 + id)
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/Allocate.h"   /* Allocate_Space() */
#include "../MoX/src/MOX_TYPE.h"   /* SAMB_ptr */
#include "../MoX/src/MOM_DAT.h"    /* struct s_WIZARD (wizard_id field) */
#include "../MoX/src/MOX_DAT.h"    /* extern _screen_seg, _players[] */

#include "../MoM/src/CONQUEST.h"   /* Lose_Animation() */

#include <stdint.h>
#include <stdio.h>

#define DEMO_LOSE_WIZARD_MAX   13    /* LOSE.LBX has 14 wizard defeat images (entries 1..14) */
#define DEMO_SCREEN_SEG_PARAS  6615  /* 6100 + 515, matching Allocate_Data_Space */



static void Effect_Lose_Run(void)
{
    int wizard_id;

    if(Demo_Assets_Ready() == 0)
    {
        printf("[lose] assets unavailable -- the lose animation needs LOSE.LBX\n");
        return;
    }

    wizard_id = Demo_Arg_Int(0, 0);
    if(wizard_id < 0) { wizard_id = 0; }
    if(wizard_id > DEMO_LOSE_WIZARD_MAX) { wizard_id = DEMO_LOSE_WIZARD_MAX; }

    /* _screen_seg is the chained scratch buffer Lose_Animation loads into; the trimmed boot doesn't
       set it up (Allocate_Data_Space does in the full game). */
    if(_screen_seg == NULL)
    {
        _screen_seg = Allocate_Space(DEMO_SCREEN_SEG_PARAS);
    }

    /* _players is zeroed by the harness; pick which wizard's defeat image plays (LOSE.LBX 1 + id). */
    _players[0].wizard_id = (uint8_t)wizard_id;

    printf("[lose] wizard %d -- playing Lose_Animation\n", wizard_id);

    /* The real animation runs its own self-terminating loop (ends at stage 46) and presents itself. */
    Lose_Animation(0);

    /* Hold until the user closes the window. */
    while(Demo_Quit() == 0)
    {
        Demo_Dwell(200);
    }
}


const Demo_Effect effect_lose =
{
    "lose",
    "Defeat animation (Lose_Animation) -- wizard falls into limbo; wizard arg 0-13",
    Effect_Lose_Run
};
