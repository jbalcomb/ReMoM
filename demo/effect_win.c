/*
    effect_win.c — the victory animation (MoM Win_Animation).

    Win_Animation() is self-contained and self-terminating (its loop ends at m_conquest_anim_stage
    > 90), so this effect calls the REAL Win_Animation() directly, the same way effect_lose does.

    It shows the winning wizard raising their hands over the WIN background, the red sparkle, the
    PLANETS file-animation, and the "...Master of Magic" victory text, under the WIN palette.

    Setup beyond the harness boot:
      - Allocate_Data_Space(): Win_Animation needs World_Data (Combat_Cache_Write, and it loads
        IMG_GAME_RedSparkle / _CITIES into it) and _screen_seg.  This is the game's own data-space
        init (the trimmed harness boot skips it).
      - GUI_String_1: a char* the game allocates during conquest-screen setup and Win_Animation_Draw
        stu_strcpy's the victory text into; allocate it here or it's a NULL deref.

    Choose the wizard with:  demo_vga win <wizard_id>   (0..13)
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/MOX_TYPE.h"   /* SAMB_ptr */
#include "../MoX/src/MOM_DAT.h"    /* struct s_WIZARD, World_Data, GUI_String_1 */
#include "../MoX/src/MOX_DAT.h"    /* extern _screen_seg, _players[] */

#include "../MoM/src/ALLOC.h"      /* Allocate_Data_Space() */
#include "../MoM/src/CONQUEST.h"   /* Win_Animation() */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WIN_WIZARD_MAX  13
#define WIN_DATA_SPACE_PARAS  6100  /* same as ReMoM_Init_Engine's Allocate_Data_Space(6100) */



static void Effect_Win_Run(void)
{
    int wizard_id;

    if(Demo_Assets_Ready() == 0)
    {
        printf("[win] assets unavailable -- the victory animation needs WIN.LBX\n");
        return;
    }

    wizard_id = Demo_Arg_Int(0, 0);
    if(wizard_id < 0) { wizard_id = 0; }
    if(wizard_id > WIN_WIZARD_MAX) { wizard_id = WIN_WIZARD_MAX; }

    /* Win_Animation needs World_Data + _screen_seg (Combat_Cache, _CITIES, RedSparkle, image loads);
       Allocate_Data_Space sets all of those up (the trimmed boot skips it). */
    if(World_Data == NULL)
    {
        Allocate_Data_Space(WIN_DATA_SPACE_PARAS);
    }

    /* Win_Animation_Draw stu_strcpy's the victory text into GUI_String_1 (a char* the game allocates). */
    if(GUI_String_1 == NULL)
    {
        GUI_String_1 = (char *)malloc(100);
    }

    /* _players is zeroed by the harness; pick which wizard wins (hands sprite + WIN blue image). */
    _players[0].wizard_id = (uint8_t)wizard_id;

    printf("[win] wizard %d -- playing Win_Animation\n", wizard_id);

    /* The real animation runs its own self-terminating loop and presents itself. */
    Win_Animation(0);

    /* Hold the final frame until the user closes the window. */
    while(Demo_Quit() == 0)
    {
        Demo_Dwell(200);
    }
}


const Demo_Effect effect_win =
{
    "win",
    "Victory animation (Win_Animation) -- wizard raises hands; wizard arg 0-13",
    Effect_Win_Run
};
