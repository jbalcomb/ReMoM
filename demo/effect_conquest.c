/*
    effect_conquest.c — the wizard-conquest animation (MoM Conquest_Animation).

    Conquest_Animation() is self-contained and self-terminating (its loop ends at m_conquest_anim_stage
    > 30), so this effect calls the REAL Conquest_Animation() directly, the same way effect_win and
    effect_lose do.

    It shows the conquered wizard in the WIZLAB background, the conquerors (wizard, axeboy, conehead)
    approaching, the conqueror's grow-zap, the ZAP file-animation (CONQUEST.LBX entry 17, streamed a
    frame at a time through Open_File_Animation / Draw_File_Animation), and the conquered wizard's
    end effect, under the WIZLAB palette (Load_Palette 3).

    Setup beyond the harness boot:
      - Allocate_Data_Space(): Conquest_Animation needs _screen_seg (it LBX_Reload's every image into
        it) and _CITIES (it counts the conquered wizard's cities).  This is the game's own data-space
        init (the trimmed harness boot skips it).  _cities is left 0, so the text reads "defeats".
      - magic_set.sound_effects = ST_TRUE: the zap-strike stages Play_Sound(SND_GAME_Scream) even with
        sound effects off (Conquest_Animation_Draw, stage == 11), and SND_GAME_Scream is only loaded
        when they are on.  With it NULL the SDL audio layer dereferences it.
      - _players[0] is the conqueror, _players[1] the conquered; names are set so the title text prints.

    Choose the wizards with:  demo_vga conquest <winner_wizard_id> <loser_wizard_id> [raiders]
        wizard ids 0..13; `raiders` makes the conqueror the neutral player (no zap, no file animation)
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../MoX/src/MOX_TYPE.h"   /* SAMB_ptr */
#include "../MoX/src/MOM_DAT.h"    /* struct s_WIZARD, World_Data, _cities, _current_player_idx */
#include "../MoX/src/MOX_DAT.h"    /* extern _screen_seg, _players[] */
#include "../MoX/src/MOX_DEF.h"    /* NEUTRAL_PLAYER_IDX, ST_TRUE */
#include "../MoX/src/MOX_SET.h"    /* magic_set */

#include "../MoM/src/ALLOC.h"      /* Allocate_Data_Space() */
#include "../MoM/src/CONQUEST.h"   /* Conquest_Animation() */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CONQUEST_WIZARD_MAX 13
#define CONQUEST_DATA_SPACE_PARAS 6100  /* same as ReMoM_Init_Engine's Allocate_Data_Space(6100) */

extern int16_t m_conquest_anim_stage;  /* CONQUEST.c; not in CONQUEST.h */

#define CONQUEST_WINNER_IDX 0
#define CONQUEST_LOSER_IDX 1



static void Effect_Conquest_Run(void)
{
    int winner_wizard_id;
    int loser_wizard_id;
    int winner_idx;
    const char * mode;

    if(Demo_Assets_Ready() == 0)
    {
        printf("[conquest] assets unavailable -- the conquest animation needs WIZLAB.LBX + CONQUEST.LBX\n");
        return;
    }

    winner_wizard_id = Demo_Arg_Int(0, 0);
    if(winner_wizard_id < 0) { winner_wizard_id = 0; }
    if(winner_wizard_id > CONQUEST_WIZARD_MAX) { winner_wizard_id = CONQUEST_WIZARD_MAX; }

    loser_wizard_id = Demo_Arg_Int(1, 1);
    if(loser_wizard_id < 0) { loser_wizard_id = 0; }
    if(loser_wizard_id > CONQUEST_WIZARD_MAX) { loser_wizard_id = CONQUEST_WIZARD_MAX; }

    mode = Demo_Arg(2);
    winner_idx = CONQUEST_WINNER_IDX;
    if((mode != NULL) && (strcmp(mode, "raiders") == 0))
    {
        winner_idx = NEUTRAL_PLAYER_IDX;
    }

    /* Conquest_Animation needs _screen_seg + _CITIES; Allocate_Data_Space sets both up (the trimmed boot skips it). */
    if(World_Data == NULL)
    {
        Allocate_Data_Space(CONQUEST_DATA_SPACE_PARAS);
    }
    _cities = 0;

    /* SND_GAME_Scream is played at the zap strike regardless of the setting; only loaded when it is on. */
    magic_set.sound_effects = ST_TRUE;

    /* _players is zeroed by the harness; pick the conqueror and the conquered. */
    _players[CONQUEST_WINNER_IDX].wizard_id = (uint8_t)winner_wizard_id;
    strcpy(_players[CONQUEST_WINNER_IDX].name, "Conqueror");
    _players[CONQUEST_LOSER_IDX].wizard_id = (uint8_t)loser_wizard_id;
    strcpy(_players[CONQUEST_LOSER_IDX].name, "Conquered");
    _current_player_idx = CONQUEST_WINNER_IDX;

    printf("[conquest] winner wizard %d%s, loser wizard %d -- playing Conquest_Animation (ESC skips)\n", winner_wizard_id, (winner_idx == NEUTRAL_PLAYER_IDX) ? " (raiders)" : "", loser_wizard_id);

    /* The real animation runs its own self-terminating loop and presents itself. */
    Conquest_Animation(CONQUEST_LOSER_IDX, (int16_t)winner_idx);

    printf("[conquest] Conquest_Animation returned at stage %d\n", m_conquest_anim_stage);

    /* Hold the final frame until the user closes the window. */
    while(Demo_Quit() == 0)
    {
        Demo_Dwell(200);
    }
}


const Demo_Effect effect_conquest =
    {
        "conquest",
        "Wizard-conquest animation (Conquest_Animation) incl. the ZAP file-animation; args <winner 0-13> <loser 0-13> [raiders]",
        Effect_Conquest_Run};
