/*
    SimTex Universe (STU)
    Test

*/

#include "STU_DBG.h"
#include "STU_LOG.h"
#include "STU_TST.h"

#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOM_DEF.h"

#include "../../MoM/src/CITYCALC.h" /* Kill_Unit() */
#include "../../MoM/src/MainScr.h"  /* Build_Unit_Stack() */
#include "../../MoM/src/Spellbook.h"
#include "../../MoM/src/NEXTTURN.h" /* Delete_Dead_Units() */
#include "../../MoM/src/UNITSTK.h"  /* WIZ_NextIdleStack() */

#include "../../ext/stu_compat.h"  /* stu_strcmp -- scenario selection */



void TST_Cheat_Power(int16_t player_idx)
{

    // Alt-PWR Cheat_Power()
    // _players[itr1].mana_reserve = 10000;
    // _players[itr1].gold_reserve = 10000;
    // _players[itr1].spell_casting_skill = 10000;
    // _players[itr1].Nominal_Skill = Player_Base_Casting_Skill(itr1);
    // _players[itr1].Skill_Left = _players[itr1].Nominal_Skill;

    _players[player_idx].gold_reserve = 32767;  // MAX_SINT32

    // NOWORKIE  "-1", because of Print_Integer_Right()  _players[player_idx].mana_reserve = 65535;  // MAX_UINT16
    _players[player_idx].mana_reserve = 32767;  // MAX_SINT32

    _players[player_idx].spell_casting_skill = 32767;  // MAX_SINT32
    _players[player_idx].Nominal_Skill = Player_Base_Casting_Skill(player_idx);  // 181
    _players[player_idx].Skill_Left = _players[player_idx].Nominal_Skill;

}

void TST_Learn_Spell(int16_t player_idx, int16_t spell_idx)
{
    int16_t spell_realm_idx = 0;
    int16_t spell_realm = 0;

    spell_realm = ((spell_idx - 1) / NUM_SPELLS_PER_MAGIC_REALM);
    spell_realm_idx = ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM);
    _players[player_idx].spells_list[((spell_realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_realm_idx)] = sls_Known;

}

/*
takes either Id or Location
kills all units in the stack

*/
void TST_Delete_Stack(int16_t target_unit_idx, int16_t target_wx, int16_t target_wy, int16_t target_wp)
{
    int16_t itr_units = 0;

    if(target_unit_idx != ST_UNDEFINED)
    {
        target_wx = _UNITS[target_unit_idx].wx;
        target_wy = _UNITS[target_unit_idx].wy;
        target_wp = _UNITS[target_unit_idx].wp;
    }
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].wx == target_wx)
            &&
            (_UNITS[itr_units].wy == target_wy)
            &&
            (_UNITS[itr_units].wp == target_wp)
        )
        {
            Kill_Unit(itr_units, kt_Normal);
        }
    }
    Delete_Dead_Units();

    /* Delete_Dead_Units -> Delete_Structure renumbers _UNITS[], so the currently-selected _unit and
       _unit_stack[] now hold pre-compaction indices (that is the Earth-Elemental-in-the-unit-window
       bug -- a stale index landing on a different unit).  Re-select via the game's own post-turn path
       (NEXTTURN.c:469 does exactly this): WIZ_NextIdleStack picks a valid idle stack, updates _unit,
       rebuilds _unit_stack[] fresh through Select_Unit_Stack -> Build_Unit_Stack, and sets the map
       focus.  Rebuilding for the deleted tile instead would leave an empty (unselected) stack. */
    WIZ_NextIdleStack(HUMAN_PLAYER_IDX, &_map_x, &_map_y, &_map_plane);
}

/* Selected by ReMoM's `--patch <name>`.  NULL = inert; see STU_TST.h for why this is not gated
   behind STU_DEBUG. */
const char * g_tst_patch_scenario = NULL;


/*
    som_ready -- the human wizard knows the Spell of Mastery and can afford to cast it.

    This sets PRECONDITIONS ONLY.  Nothing here casts the spell, decides the game is over, or touches
    a presentation function.  After this runs, the player (or a .hms script) opens the spellbook and
    casts it, SBookScr.c hands off to Cast_Spell_Of_Mastery(), casting accumulates over turns, and
    when it completes OverSpel.c calls Spell_Of_Mastery() and the endgame plays out -- all the game's
    own logic.  That distinction is the whole point; see doc/#AI_Plans/BRA-Demo-Reel.md section 9.
*/
static void TST_Scenario_Som_Ready(int16_t player_idx)
{
    TST_Learn_Spell(player_idx, spl_Spell_Of_Mastery);
    TST_Cheat_Power(player_idx);

    LOG_INFO(LOG_CAT_GENERAL, "[TST] scenario 'som_ready': player %d knows Spell of Mastery; mana=%d skill=%d skill_left=%d", player_idx, _players[player_idx].mana_reserve, _players[player_idx].spell_casting_skill, _players[player_idx].Skill_Left);
}


/*
    som_casting -- the human wizard is mid-cast on the Spell of Mastery, one turn from completing.

    This is the second half of the two-part Spell-of-Mastery filming (BRA-Demo-Reel.md section 9.2).
    Part 1 (som_ready) films the human BEGINNING the cast; this films it COMPLETING and the endgame.
    The many turns of accumulation between the two are simply not filmed -- that is editing, not
    fabrication, and both halves are real game logic from states a real game reaches.

    Why this state is legitimately reachable: a wizard who selected the Spell of Mastery some turns
    ago and has been pouring skill into it every turn since holds exactly these fields.  We are not
    inventing a state, we are starting at turn 27 of 28 instead of playing the first 27.

    Nothing here casts anything.  On the next turn NEXTTURN.c's per-turn loop applies Skill_Left,
    drives casting_cost_remaining to 0, and the existing check at NEXTTURN.c:453 fires
    Cast_Spell_Overland() -> OverSpel.c -> Spell_Of_Mastery() -> the endgame.  All the game's own code.
*/
static void TST_Scenario_Som_Casting(int16_t player_idx)
{
    int16_t one_turn_of_casting = 0;

    /*
        starting from SAVETEST.GAM
    */
    // When it hits next turn, the AI initiates combat with the human, so we need to patch the data so that doesn't happen. We need to delete the stack contain unit_idx 685 at 20,16
    TST_Delete_Stack(685, 20, 16, 0);

    TST_Learn_Spell(player_idx, spl_Spell_Of_Mastery);
    TST_Cheat_Power(player_idx);

    /* Leave less than a single turn's casting to do, so the very next Next Turn completes it.
       Nominal_Skill is what the per-turn loop subtracts (NEXTTURN.c:3350). */
    one_turn_of_casting = (_players[player_idx].Nominal_Skill / 2);
    if(one_turn_of_casting < 1)
    {
        one_turn_of_casting = 1;
    }

    _players[player_idx].casting_spell_idx = spl_Spell_Of_Mastery;
    _players[player_idx].casting_cost_original = (uint16_t)spell_data_table[spl_Spell_Of_Mastery].casting_cost;
    _players[player_idx].casting_cost_remaining = (uint16_t)one_turn_of_casting;

    LOG_INFO(LOG_CAT_GENERAL, "[TST] scenario 'som_casting': player %d mid-cast on Spell of Mastery; cost_original=%d cost_remaining=%d nominal_skill=%d", player_idx, _players[player_idx].casting_cost_original, _players[player_idx].casting_cost_remaining, _players[player_idx].Nominal_Skill);
}


void TST_Patch_Game_Data(void)
{
    int16_t player_idx = 0;
    int16_t spell_idx = 0;

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: BEGIN: TST_Patch_Game_Data()", __FILE__, __LINE__);
#endif

    player_idx = HUMAN_PLAYER_IDX;

    /* Inert unless a scenario was explicitly requested.  A normal player's run returns here having
       changed nothing, which is exactly what this function does today. */
    if(g_tst_patch_scenario == NULL)
    {
        return;
    }

    LOG_INFO(LOG_CAT_GENERAL, "[TST] applying scenario patch '%s'", g_tst_patch_scenario);

    if(stu_strcmp(g_tst_patch_scenario, "som_ready") == 0)
    {
        TST_Scenario_Som_Ready(player_idx);
    }
    else if(stu_strcmp(g_tst_patch_scenario, "som_casting") == 0)
    {
        TST_Scenario_Som_Casting(player_idx);
    }
    else
    {
        LOG_INFO(LOG_CAT_GENERAL, "[TST] unknown scenario '%s' -- nothing patched", g_tst_patch_scenario);
    }

    // TST_Cheat_Power(player_idx);

    // TST_Learn_Spell(player_idx, spl_Dispel_Magic);

    // _CITIES[0].enchantments[FLYING_FORTRESS] = ST_TRUE;
    // _CITIES[0].enchantments[CHAOS_WARD] = ST_TRUE;
    // _players[0].Globals[DETECT_MAGIC     ] = (0 + 1);  // 206  spl_Detect_Magic
    // _players[1].Globals[AWARENESS        ] = (1 + 1);  // 209  spl_Awareness

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: END: TST_Patch_Game_Data()", __FILE__, __LINE__);
#endif
}
