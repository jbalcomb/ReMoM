/*
    MAGIC.EXE
    ovr056

MoO2
Module: INITGAME
*/

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/random.h"

#include "../../STU/src/STU_LOG.h"  /* Init_Heroes Spells write trace */

#include "NewGame.h"
#include "Spellbook.h"
#include "UNITTYPE.h"

#include "INITGAME.h"

#include "../../ext/stu_compat.h"

#include <string.h>



// MGC  dseg:2AD0
extern struct s_WIZARD_PRESET _wizard_presets_table[15];

// MGC  dseg:2C6A
extern struct s_DEFAULT_SPELLS _default_spells[5];

// MGC  dseg:354C
extern int16_t TBL_AI_Realm_PRS[6][6];
// MGC  dseg:3594
extern int16_t TBL_AI_Realm_OBJ[6][5];
// MGC  dseg:35D0
extern struct S_HERO_TEMPLATE _hero_premade_table[NUM_HERO_TYPES];

// ...oops... MGC  dseg:8A42 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+_start_spells s_Init_Spells_Table <0>   ; DATA XREF: GAME_New_Screen_5+171w ...
// MGC  dseg:8A42 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+_player_start_spells s_Init_Base_Realms 6 dup(<0>)
// ...needed by INITGAME.c, so defn in NewGame.h
extern struct s_Init_Base_Realms _player_start_spells[PLAYER_COUNT_MAX];

// o51p36
void Draw_Building_The_Worlds(int16_t percent);



// MGC o56p1
/**
 * @brief Initializes AI player profiles and their starting spell libraries.
 *
 * @details
 * Builds non-human wizard setup data for a new game by first generating AI
 * wizard profiles, then copying default realm spell selections into
 * `_player_start_spells` for each computer player, and finally creating each
 * AI wizard's known/knowable spell library.
 *
 * The neutral player record is also initialized with baseline values used by
 * neutral units/cities in world simulation.
 *
 * Side effects include mutation of global game-state tables such as
 * `_players[]` and `_player_start_spells[]`.
 *
 * @note Player index 0 (human) is skipped in the AI spell-copy loop; AI
 *       initialization begins at player index 1 and runs to `_num_players - 1`.
 *
 * @return void
 *
 * @see Init_Computer_Players_Wizard_Profile
 * @see Init_Computer_Players_Spell_Library
 */
void Init_Computer_Players(void)
{
    int16_t itr_num_players = 0;
    int16_t itr2 = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    Init_Computer_Players_Wizard_Profile();

    for(itr_num_players = 1; itr_num_players < _num_players; itr_num_players++)
    {

        for(itr2 = 0; itr2 < 13; itr2++)
        {

            _player_start_spells[itr_num_players].realms[sbr_Nature].spells[itr2]   = _default_spells[sbr_Nature].spells[itr2];
            _player_start_spells[itr_num_players].realms[sbr_Sorcery].spells[itr2]  = _default_spells[sbr_Sorcery].spells[itr2];
            _player_start_spells[itr_num_players].realms[sbr_Chaos].spells[itr2]    = _default_spells[sbr_Chaos].spells[itr2];
            _player_start_spells[itr_num_players].realms[sbr_Life].spells[itr2]     = _default_spells[sbr_Life].spells[itr2];
            _player_start_spells[itr_num_players].realms[sbr_Death].spells[itr2]    = _default_spells[sbr_Death].spells[itr2];

        }

    }

    Init_Computer_Players_Spell_Library();

    _players[NEUTRAL_PLAYER_IDX].Defeated_Wizards = 0;

    _players[NEUTRAL_PLAYER_IDX].average_unit_cost = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Init_Computer_Players rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p2
/* COPILOT */
/**
 * @brief Finalizes per-player and global runtime state for a new game.
 *
 * @details
 * Runs the late initialization pass after the world, cities, and fortress
 * records already exist. The routine advances the world-building progress
 * display, clears item state, initializes hero data, resets player runtime
 * fields, seeds AI strategy timers, derives AI personalities/objectives,
 * copies fortress coordinates into each player's summoning-circle state,
 * clears message counters, and computes initial wizard-to-wizard diplomatic
 * relations.
 *
 * It then derives each active player's `capital_race` from the matching entry
 * in `_CITIES[]`, assigns the human player's starting gold from `_difficulty`,
 * and gives each AI player a fixed opening treasury plus an initial
 * `AI_SCircle_Reevals[]` delay randomized in the range `[80, 119]`.
 *
 * @return void
 *
 * @note Expects `_num_players`, `_players[]`, `_CITIES[]`, and fortress data
 *       to have already been populated by earlier new-game setup.
 * @note Mutates several global runtime tables as part of the final startup
 *       pass.
 *
 * @see Draw_Building_The_Worlds
 * @see Initialize_Items
 * @see Init_Heroes
 * @see Init_Players
 * @see Init_CP_Strategy
 * @see Init_Magic_Personalities_Objectives
 * @see Init_Summoning_Circle_And_Spell_Of_Mastery
 * @see Initialize_Messages
 * @see Init_Diplomatic_Relations
 */
void Init_Runtime(void)
{
    int16_t itr_players = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    Draw_Building_The_Worlds(100);
    Initialize_Items();
    Init_Heroes();
    Init_Players();
    Init_CP_Strategy();
    Init_Magic_Personalities_Objectives();
    Init_Summoning_Circle_And_Spell_Of_Mastery();
    Initialize_Messages();
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].capital_race = _CITIES[itr_players].race;
    }
    Init_Diplomatic_Relations();
    _players[HUMAN_PLAYER_IDX].gold_reserve = ((5 - _difficulty) * 25);
    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].gold_reserve = 150;
        AI_SCircle_Reevals[itr_players] = (80 + Random(40));
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o56p3
// drake178: AI_WIZ_SetRelations()
/*
sets the starting relations between all wizards based
on their spellbooks
RE-CHECK: sets fields $182 to the visible relations
 and $4B0 to 0 - what are these?
 */
/*
    sets Grand Vizier to 'off'

*/
void Init_Diplomatic_Relations(void)
{
    int16_t Wiz_1_Books = 0;
    int16_t Wiz_2_Books = 0;
    int16_t Most_Chaos_Books = 0;
    int16_t Life_Book_Total = 0;
    int16_t Most_Death_Books = 0;
    int16_t itr_players1 = 0;  // _DI_
    int16_t itr_players2 = 0;  // _SI_
    int16_t IDK = 0;  // _CX_

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr_players1 = 0; itr_players1 < _num_players; itr_players1++)
    {

        for(itr_players2 = (itr_players1 + 1); itr_players2 < _num_players; itr_players2++)
        {
            Most_Death_Books = _players[itr_players1].spellranks[sbr_Death];
            if(_players[itr_players2].spellranks[sbr_Death] > Most_Death_Books)
            {
                Most_Death_Books = _players[itr_players2].spellranks[sbr_Death];
            }

            Life_Book_Total = _players[itr_players1].spellranks[sbr_Life];

            Life_Book_Total += _players[itr_players2].spellranks[sbr_Life];

            Most_Chaos_Books = _players[itr_players1].spellranks[sbr_Chaos];
            if(_players[itr_players2].spellranks[sbr_Chaos] > Most_Chaos_Books)
            {
                Most_Chaos_Books = _players[itr_players2].spellranks[sbr_Chaos];
            }

            IDK = 0;
            if(
                (Life_Book_Total > 0)
                &&
                (Most_Death_Books > 0)
            )
            {
                IDK -= ((Life_Book_Total + Most_Death_Books) * 5);
            }
            else
            {
                IDK += (Life_Book_Total * 2);
                IDK -= (Most_Death_Books * 3);
            }

            IDK -= (Most_Chaos_Books * 2);

            Wiz_2_Books = _players[itr_players2].spellranks[sbr_Sorcery];
            Wiz_1_Books = _players[itr_players1].spellranks[sbr_Sorcery];
            if(Wiz_1_Books < Wiz_2_Books)
            {
                Wiz_2_Books = Wiz_1_Books;
            }
            IDK += Wiz_2_Books;

            Wiz_2_Books = _players[itr_players2].spellranks[sbr_Chaos];
            Wiz_1_Books = _players[itr_players1].spellranks[sbr_Chaos];
            if(Wiz_1_Books < Wiz_2_Books)
            {
                Wiz_2_Books = Wiz_1_Books;
            }
            IDK += Wiz_2_Books;

            Wiz_2_Books = _players[itr_players2].spellranks[sbr_Life];
            Wiz_1_Books = _players[itr_players1].spellranks[sbr_Life];
            if(Wiz_1_Books < Wiz_2_Books)
            {
                Wiz_2_Books = Wiz_1_Books;
            }
            IDK += Wiz_2_Books;

            if(IDK < -90)
            {
                IDK = -90;
            }

            _players[itr_players1].Dipl.Visible_Rel[itr_players2] = (int8_t)IDK;
            _players[itr_players2].Dipl.Visible_Rel[itr_players1] = (int8_t)IDK;

            _players[itr_players1].Dipl.Default_Rel[itr_players2] = (int8_t)IDK;
            _players[itr_players2].Dipl.Default_Rel[itr_players1] = (int8_t)IDK;

            _players[itr_players1].peace_duration[itr_players2] = 0;
            _players[itr_players2].peace_duration[itr_players1] = 0;

        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Init_Diplomatic_Relations rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p4
void Initialize_Items(void)
{
    int16_t itr = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr = 0; itr < 138; itr++)
    {
        _ITEMS[itr].cost = 0;
    }
    for(itr = 0; itr < 250; itr++)
    {
        TBL_Premade_Items[itr] = 0;
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Initialize_Items rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p5
// drake178: WIZ_Set_Sum_n_SoM()
/*
sets the summon location and spell of mastery
research cost into the record of every wizard
*/
/*
    sets summon wx,wy,wp from fortress wx,wy,wp, for all players
    sets research cost for Spell of Mastery, for all players    
*/
void Init_Summoning_Circle_And_Spell_Of_Mastery(void)
{
    int16_t itr_players = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].summon_wp = _FORTRESSES[itr_players].wp;
        _players[itr_players].summon_wx = _FORTRESSES[itr_players].wx;
        _players[itr_players].summon_wy = _FORTRESSES[itr_players].wy;
        
        _players[itr_players].som_research_cost = spell_data_table[spl_Spell_Of_Mastery].research_cost;

        if(
            (_players[itr_players].spellranks[0] == 11)
            &&
            (_players[itr_players].spellranks[1] == 11)
            &&
            (_players[itr_players].spellranks[2] == 11)
            &&
            (_players[itr_players].spellranks[3] == 11)
            &&
            (_players[itr_players].spellranks[4] == 11)
        )
        {
            _players[itr_players].som_research_cost -= 3000;
        }

    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Init_Summoning_Circle_And_Spell_Of_Mastery rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p6
// drake178: WIZ_SetPersonalities()
// MoO2  Module: KEN  Init_NPC_Personalities_Objectives_Themes_()
/*
sets the primary and secondary realms into each
player's wizard record, then rolls out the AI
personalities and objectives based on that and the
retorts already stored
BUG: the guaranteed AI researchable spells for Hard
 and Impossible will also remove those spells if
 the wizard already has them
BUG: 2+ realm wizards with Nature as the primary will
 also always set that as the secondary
*/
/*
    sets Primary_Realm and Secondary_Realm for all players
    sets starting spells for AI players
    sets Personality and Object for AI players
*/
void Init_Magic_Personalities_Objectives(void)
{
    int16_t AI_OBJ_Weights[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t AI_PRS_Weights[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t Max_Value = 0;
    int16_t Random_Result = 0;
    int16_t Secondary_Realm = 0;
    int16_t Primary_Realm = 0;
    int16_t Picked_Objective = 0;
    int16_t Picked_Personality = 0;
    int16_t spellranks = 0;
    int16_t itr = 0;  // _SI_
    int16_t itr_players = 0;  // _DI_

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    // ; loop through the human player's bookshelf to find
    // ; the realm they have the most books of
    Primary_Realm = 0;
    spellranks = _players[HUMAN_PLAYER_IDX].spellranks[0];
    for(itr = 1; itr < NUM_MAGIC_TYPES; itr++)
    {
        if(_players[HUMAN_PLAYER_IDX].spellranks[itr] > spellranks)
        {
            Primary_Realm = itr;
            spellranks = _players[HUMAN_PLAYER_IDX].spellranks[itr];
        }
    }

    // ; loop through the human player's bookshelf to find
    // ; the realm they have the second most books of
    Secondary_Realm = 0;
    spellranks = _players[HUMAN_PLAYER_IDX].spellranks[0];
    for(itr = 1; itr < NUM_MAGIC_TYPES; itr++)
    {
        if(_players[HUMAN_PLAYER_IDX].spellranks[itr] > spellranks)
        {
            Secondary_Realm = itr;
            spellranks = _players[HUMAN_PLAYER_IDX].spellranks[itr];
        }
    }

    if(_players[HUMAN_PLAYER_IDX].spellranks[Secondary_Realm] == 0)
    {
        Secondary_Realm = Primary_Realm;
    }

    _players[HUMAN_PLAYER_IDX].Prim_Realm = Primary_Realm;

    _players[HUMAN_PLAYER_IDX].Sec_Realm = Secondary_Realm;

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {
        Primary_Realm = 0;
        spellranks = _players[itr_players].spellranks[0];
        
        for(itr = 1; itr < NUM_MAGIC_TYPES; itr++)
        {
            if(_players[itr_players].spellranks[itr] > spellranks)
            {
                Primary_Realm = itr;
                spellranks = _players[itr_players].spellranks[itr];
            }
        }

        switch(Primary_Realm)
        {
            case sbr_Nature:
            {
                _players[itr_players].spells_list[spl_Sprites] = 2;
            } break;
            case sbr_Sorcery:
            {
                _players[itr_players].spells_list[spl_Nagas] = 2;
            } break;
            case sbr_Chaos:
            {
                _players[itr_players].spells_list[spl_Hell_Hounds] = 2;
            } break;
            case sbr_Life:
            {
                _players[itr_players].spells_list[spl_Guardian_Spirit] = 2;
            } break;
            case sbr_Death:
            {
                _players[itr_players].spells_list[spl_Ghouls] = 2;
            } break;
        }

        if(_difficulty >= god_Hard)
        {
            switch(Primary_Realm)
            {
                case sbr_Nature:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == 0)
                    {
                        _players[itr_players].spells_list[spl_Basilisk] = 1;
                    }
                } break;
                case sbr_Sorcery:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == 0)  /* ; BUG: this is not the same spell! */
                    {
                        _players[itr_players].spells_list[spl_Phantom_Beast] = 1;
                    }
                } break;
                case sbr_Chaos:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == 0)  /* ; BUG: this is not the same spell! */
                    {
                        _players[itr_players].spells_list[spl_Chimeras] = 1;
                    }
                } break;
                case sbr_Life:
                {
                    if(_players[itr_players].spells_list[spl_Path_Finding] == 0)  /* ; BUG: this is not the same spell! */
                    {
                        _players[itr_players].spells_list[spl_Unicorns] = 1;
                    }
                } break;
                case sbr_Death:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == 0)  /* ; BUG: this is not the same spell! */
                    {
                        _players[itr_players].spells_list[spl_Shadow_Demons] = 1;
                    }
                } break;
            }
        }

        Secondary_Realm = 0;
        spellranks = _players[itr_players].spellranks[0];
        for(itr = 1; itr < NUM_MAGIC_TYPES; itr++)
        {
            if(_players[itr_players].spellranks[itr] > spellranks)
            {
                Secondary_Realm = itr;
                spellranks = _players[itr_players].spellranks[itr];
            }
        }
    
        if(_players[itr_players].spellranks[Secondary_Realm] == 0)
        {
            Secondary_Realm = Primary_Realm;
        }
    
        _players[itr_players].Prim_Realm = Primary_Realm;
    
        _players[itr_players].Sec_Realm = Secondary_Realm;
    
        for(itr = 1; itr < 6; itr++)
        {
            AI_PRS_Weights[itr] = TBL_AI_Realm_PRS[Primary_Realm][itr];
        }

        for(itr = 1; itr < 5; itr++)
        {
            AI_OBJ_Weights[itr] = TBL_AI_Realm_OBJ[Primary_Realm][itr];
        }

        if(_players[itr_players].warlord == 1)
        {
            AI_PRS_Weights[AI_PRS_RUTHLESS] += 2;
            AI_PRS_Weights[AI_PRS_AGGRESSIVE] += 3;
            AI_OBJ_Weights[AI_OBJ_MILITARIST] += 2;
            AI_OBJ_Weights[AI_OBJ_EXPANSIONIST] += 3;
        }

        if(_players[itr_players].chaos_mastery == 1)
        {
            AI_PRS_Weights[AI_PRS_CHAOTIC] += 3;
            AI_OBJ_Weights[AI_OBJ_THEURGIST] += 1;
        }

        if(_players[itr_players].nature_mastery == 1)
        {
            AI_PRS_Weights[AI_PRS_LAWFUL] += 3;
            AI_OBJ_Weights[AI_OBJ_THEURGIST] += 1;
        }

        if(_players[itr_players].infernal_power == 1)
        {
            AI_PRS_Weights[AI_PRS_MANIACAL] += 4;
        }

        if(_players[itr_players].divine_power == 1)
        {
            AI_PRS_Weights[AI_PRS_PEACEFUL] += 4;
        }

        if(_players[itr_players].channeler == 1)
        {
            AI_PRS_Weights[AI_PRS_AGGRESSIVE] += 2;
        }

        if(_players[itr_players].alchemy == 1)
        {
            AI_PRS_Weights[AI_OBJ_PERFECTIONIST] += 2;
        }

        if(_players[itr_players].archmage == 1)
        {
            AI_PRS_Weights[AI_OBJ_THEURGIST] += 4;
        }

        if(_players[itr_players].myrran == 1)
        {
            AI_PRS_Weights[AI_OBJ_EXPANSIONIST] += 4;
        }

        if(_players[itr_players].conjurer == 1)
        {
            AI_PRS_Weights[AI_OBJ_MILITARIST] += 3;
        }

        if(Primary_Realm == sbr_Life)
        {
            AI_PRS_Weights[AI_PRS_MANIACAL] = 0;
        }

        if(Primary_Realm == sbr_Death)
        {
            AI_PRS_Weights[AI_PRS_PEACEFUL] = 0;
        }

        Max_Value = 0;
        for(itr = 0; itr < 6; itr++)
        {
            Max_Value += AI_PRS_Weights[itr];
        }
        Random_Result = Random(Max_Value);
        Picked_Personality = 0;
        Random_Result -= AI_PRS_Weights[AI_PRS_MANIACAL];
        while((Random_Result > 0) && (Picked_Personality < 5))
        {
            Picked_Personality++;
            Random_Result -= AI_PRS_Weights[Picked_Personality];
        }

        Max_Value = 0;
        for(itr = 0; itr < 6; itr++)
        {
            Max_Value += AI_OBJ_Weights[itr];
        }
        Random_Result = Random(Max_Value);
        Picked_Objective = 0;
        Random_Result -= AI_OBJ_Weights[AI_PRS_MANIACAL];
        while((Random_Result > 0) && (Picked_Objective < 5))
        {
            Picked_Objective++;
            Random_Result -= AI_OBJ_Weights[Picked_Personality];
        }

        _players[itr_players].Personality = Picked_Personality;

        _players[itr_players].Objective = Picked_Objective;

    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Init_Magic_Personalities_Objectives rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p7
/*
MoO2  Module: DIPLOMAC  Init_Diplomatic_Relations_()
*/
/**
 * @brief Resets per-player AI strategy timers and relationship sentinel state.
 *
 * @details
 * Iterates across all active players in `_num_players` and clears the
 * reevaluation countdowns used by AI hostility, magic-strategy, and
 * magic-power logic. It also initializes every entry in each player's
 * `peace_duration[]` array to `ST_UNDEFINED`, then resets the four
 * undocumented `IDK_AI_Strategy_*` fields to the same sentinel value.
 *
 * Despite the function name, the reset is applied to every active player slot
 * rather than only computer-controlled wizards.
 *
 * @return void
 *
 * @note This routine mutates the global `_players[]` table in place.
 * @note `ST_UNDEFINED` is used here as an initialization sentinel, not as a
 *       countdown value.
 */
void Init_CP_Strategy(void)
{
    int16_t player_idx = 0;
    int16_t other_player_idx = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(player_idx = 0; player_idx < _num_players; player_idx++)
    {
        _players[player_idx].reevaluate_hostility_countdown = 0;
        _players[player_idx].reevaluate_magic_strategy_countdown = 0;
        _players[player_idx].reevaluate_magic_power_countdown = 0;
        for(other_player_idx = 0; other_player_idx < _num_players; other_player_idx++)
        {
            _players[player_idx].peace_duration[other_player_idx] = ST_UNDEFINED;
        }
        _players[player_idx].niu_cp_target_1 = ST_UNDEFINED;
        _players[player_idx].niu_cp_target_2 = ST_UNDEFINED;
        _players[player_idx].cp_target_3 = ST_UNDEFINED;
        _players[player_idx].niu_cp_target_4 = ST_UNDEFINED;
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Init_CP_Strategy rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p8
/**
 * @brief Initializes baseline runtime state for every active player.
 *
 * @details
 * Walks the active player range `[0, _num_players)` and derives each
 * wizard's opening fame, nominal casting skill, and invested casting skill
 * from stored spellbook counts and retorts. It then clears or seeds the core
 * strategic resource fields used at game start, including mana, research,
 * casting progress, power-distribution ratios, tax rate, global enchantment
 * flags, vault contents, and hero slot assignments.
 *
 * The routine computes a target `spell_skill` from the player's five realm
 * book counts, adds the Archmage bonus when present, doubles the result, and
 * increments `spell_casting_skill` until `Player_Nominal_Skill()` reaches that
 * derived target.
 *
 * @return void
 *
 * @note Mutates the global `_players[]` table in place.
 * @note Every player starts with `spl_Spell_Of_Mastery` marked with state `3`
 *       and all hero slots initialized to `ST_UNDEFINED`.
 *
 * @see Player_Nominal_Skill
 */
void Init_Players(void)
{
    int16_t itr_heroes = 0;
    int16_t itr_players = 0;
    int16_t spell_skill = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[itr_players].famous == ST_TRUE)
        {
            _players[itr_players].fame = 10;
        }
        else
        {
            _players[itr_players].fame = 0;
        }
        spell_skill = (_players[itr_players].spellranks[0] + _players[itr_players].spellranks[1] + _players[itr_players].spellranks[2] + _players[itr_players].spellranks[3] + _players[itr_players].spellranks[4]);
        if(_players[itr_players].archmage != ST_FALSE)
        {
            spell_skill += 5;
        }
        /* Base skill is doubled for starting calculation */
        spell_skill += spell_skill;
        _players[itr_players].Nominal_Skill = 0;
        _players[itr_players].spell_casting_skill = 0;
        // The grow-until-reached loop lands spell_casting_skill at the smallest value where Player_Base_Casting_Skill__MGC returns spell_skill.
        // The inverse formula is N*(N-1)+1.
        while(_players[itr_players].Nominal_Skill < spell_skill)
        {
            _players[itr_players].spell_casting_skill += 1;
            _players[itr_players].Nominal_Skill = Player_Base_Casting_Skill__MGC(itr_players);
        }
        _players[itr_players].researching_spell_idx = spl_NONE;
        _players[itr_players].mana_reserve = 0;
        _players[itr_players].research_cost_remaining = 0;
        _players[itr_players].skill_ratio = 33;
        _players[itr_players].mana_ratio = 33;
        _players[itr_players].research_ratio = 34;
        _players[itr_players].spells_list[spl_Spell_Of_Mastery] = 3;
        _players[itr_players].casting_spell_idx = spl_NONE;
        _players[itr_players].casting_cost_remaining = 0;
        _players[itr_players].Globals[ETERNAL_NIGHT] = 0;
        _players[itr_players].Globals[EVIL_OMENS] = 0;
        _players[itr_players].Globals[ZOMBIE_MASTERY] = 0;
        _players[itr_players].Globals[AURA_OF_MAJESTY] = 0;
        _players[itr_players].Globals[WIND_MASTERY] = 0;
        _players[itr_players].Globals[SUPPRESS_MAGIC] = 0;
        _players[itr_players].Globals[TIME_STOP] = 0;
        _players[itr_players].Globals[NATURES_AWARENESS] = 0;
        _players[itr_players].Globals[NATURES_WRATH] = 0;
        _players[itr_players].Globals[HERB_MASTERY] = 0;
        _players[itr_players].Globals[CHAOS_SURGE] = 0;
        _players[itr_players].Globals[DOOM_MASTERY] = 0;
        _players[itr_players].Globals[GREAT_WASTING] = 0;
        _players[itr_players].Globals[METEOR_STORM] = 0;
        _players[itr_players].Globals[ARMAGEDDON] = 0;
        _players[itr_players].Globals[TRANQUILITY] = 0;
        _players[itr_players].Globals[LIFE_FORCE] = 0;
        _players[itr_players].Globals[CRUSADE] = 0;
        _players[itr_players].Globals[JUST_CAUSE] = 0;
        _players[itr_players].Globals[HOLY_ARMS] = 0;
        _players[itr_players].volcanoes = 0;
        _players[itr_players].Globals[DETECT_MAGIC] = 0;
        _players[itr_players].tax_rate = 2;
        _players[itr_players].Vault_Items[0] = ST_UNDEFINED;
        _players[itr_players].Vault_Items[1] = ST_UNDEFINED;
        _players[itr_players].Vault_Items[2] = ST_UNDEFINED;
        _players[itr_players].Vault_Items[3] = ST_UNDEFINED;
        for(itr_heroes = 0; itr_heroes < NUM_HERO_SLOTS; itr_heroes++)
        {
            _players[itr_players].Heroes[itr_heroes].unit_idx = ST_UNDEFINED;
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Init_Players rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p9
void Init_Computer_Players_Wizard_Profile(void)
{
    int16_t Realm_Retort_Available = 0;
    int16_t Realm_Index = 0;
    int16_t Bookshelf[5] = { 0, 0, 0, 0, 0 };
    int16_t myrran_count = 0;
    int16_t Picks_Left = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    int16_t banners[5] = { 0, 0, 0, 0, 0 };
    int8_t * wsa_ptr = 0;  // Pointer to 'Wizard Special Abilities'
    int16_t Book_Count = 0;
    int16_t random_banner = 0;
    int16_t Trait_Value = 0;  // NOTE: used to index wsa_ptr[]
    int16_t Picks_Used = 0;
    int16_t Trait_Type = 0;
    int16_t Erasure_Dice = 0;
    int16_t Random_Result = 0;
    int16_t Portrait_Taken = 0;
    int16_t itr3 = 0;
    int16_t itr1 = 0;
    int16_t itr2 = 0;


    /* Phase 1: Initialize local banner tracking */
    for(itr1 = 0; itr1 < NUM_BANNER_SELECTIONS; itr1++)
    {
        banners[itr1] = ST_FALSE;
    }
    banners[_players[HUMAN_PLAYER_IDX].banner_id] = ST_TRUE;


BeginTopLevelPlayerLoop:


    /* Phase 2 */
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        do {
            Portrait_Taken = ST_FALSE;
            Random_Result = Random(14) - 1; /* {0,...,13} */
            for(itr3 = 0; itr3 < itr2; itr3++)
            {
                if(_players[itr3].wizard_id == Random_Result)
                {
                    Portrait_Taken = ST_TRUE;
                }
            }
        } while(Portrait_Taken != ST_FALSE);
        _players[itr2].wizard_id = (uint8_t)Random_Result;
    }
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        stu_strcpy(_players[itr2].name, _wizard_presets_table[_players[itr2].wizard_id].name);
    }


    /* Phase 3 */
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        _players[itr2].spellranks[sbr_Life] = _wizard_presets_table[_players[itr2].wizard_id].life;
        _players[itr2].spellranks[sbr_Death] = _wizard_presets_table[_players[itr2].wizard_id].death;
        _players[itr2].spellranks[sbr_Chaos] = _wizard_presets_table[_players[itr2].wizard_id].chaos;
        _players[itr2].spellranks[sbr_Nature] = _wizard_presets_table[_players[itr2].wizard_id].nature;
        _players[itr2].spellranks[sbr_Sorcery] = _wizard_presets_table[_players[itr2].wizard_id].sorcery;
        if(
            (Random(5) == 1)  // 1:5, 20%
            &&
            (_difficulty > god_Normal)
        )
        {
            Consolidate_Spell_Book_Realms(itr2);
        }
        wsa_ptr = &_players[itr2].alchemy;
        for(itr1 = 0; itr1 < NUM_WIZARD_SPECIAL_ABILITIES; itr1++)
        {
            wsa_ptr[itr1] = 0;
        }
        if(_wizard_presets_table[_players[itr2].wizard_id].special != ST_UNDEFINED)
        {
            wsa_ptr[_wizard_presets_table[_players[itr2].wizard_id].special] = ST_TRUE;
        }
    }


    /* Phase 4 */
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        /* Calculate "Erasure Dice" to randomize/reduce AI starting power */
        switch(_difficulty)
        {
            case 0: { Erasure_Dice = 0; } break;
            case 1: { Erasure_Dice = 0; } break;
            case 2: { Erasure_Dice = Random(3); } break;
            case 3: { Erasure_Dice = (Random(3) + Random(3)); } break;
            case 4: { Erasure_Dice = (Random(5) + Random(5)); } break;
            case 5: { Erasure_Dice = (Random(8) + Random(8)); } break;
        }
        wsa_ptr = &_players[itr2].alchemy;
        for(itr1 = 0; itr1 < Erasure_Dice; itr1++)
        {
            Trait_Type = Random(6);
            switch(Trait_Type)  // - 1; <= 5;
            {
                case 1: { if(_players[itr2].spellranks[sbr_Life]    > 1) { _players[itr2].spellranks[sbr_Life]    -= 1; } } break;
                case 2: { if(_players[itr2].spellranks[sbr_Death]   > 1) { _players[itr2].spellranks[sbr_Death]   -= 1; } } break;
                case 3: { if(_players[itr2].spellranks[sbr_Chaos]   > 1) { _players[itr2].spellranks[sbr_Chaos]   -= 1; } } break;
                case 4: { if(_players[itr2].spellranks[sbr_Nature]  > 1) { _players[itr2].spellranks[sbr_Nature]  -= 1; } } break;
                case 5: { if(_players[itr2].spellranks[sbr_Sorcery] > 1) { _players[itr2].spellranks[sbr_Sorcery] -= 1; } } break;
                case 6: {
                    wsa_ptr = &_players[itr2].alchemy;
                    for(itr3 = 0; itr3 < NUM_WIZARD_SPECIAL_ABILITIES; itr3++)
                    {
                        /* OGBUG  should use itr3, not itr1 */
                        wsa_ptr[itr1] = ST_FALSE;
                    }
                } break;
            }
        }
    }


    /* Phase 5 */
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        wsa_ptr = &_players[itr2].alchemy;
        Picks_Used = (
            _players[itr2].spellranks[sbr_Life]
            +
            _players[itr2].spellranks[sbr_Death]
            +
            _players[itr2].spellranks[sbr_Nature]
            +
            _players[itr2].spellranks[sbr_Chaos]
            +
            _players[itr2].spellranks[sbr_Sorcery]
        );
        Book_Count = Picks_Used;
        for(itr1 = 0; itr1 < NUM_WIZARD_SPECIAL_ABILITIES; itr1++)
        {
            if(wsa_ptr[itr1] == 0)
            {
                continue;
            }
            if(itr1 == wsa_Myrran)
            {
                Picks_Used += 3;
            }
            else if(
                (itr1 == wsa_Warlord)
                ||
                (itr1 == wsa_Infernal_Power)
                ||
                (itr1 == wsa_Divine_Power)
                ||
                (itr1 == wsa_Famous)
                ||
                (itr1 == wsa_Channeller)
            )
            {
                Picks_Used += 2;
            }
            else
            {
                Picks_Used += 1;
            }
        }
        Picks_Left = 11;
        if(_difficulty == god_Hard)
        {
            Picks_Left = 13;
        }
        if(_difficulty == god_Impossible)
        {
            Picks_Left = 15;
        }
        while(Picks_Used < Picks_Left)
        {
            Trait_Type = Random(8);
            Trait_Value = (1 + Random(4));  // NOTE: used to index wsa_ptr[]
            /* clamps: Trait_Value vs (Picks_Left-Picks_Used) and vs (12-Book_Count) */
            /* if Trait_Value <= 0 -> Trait_Type = 6 ; if Trait_Type == 6 -> Trait_Value = 1 */
            if((Trait_Value + Picks_Used) > Picks_Left)
            {
                Trait_Value = (Picks_Left - Picks_Used);
            }
            if((Trait_Value + Book_Count) > 12)
            {
                Trait_Value = (12 - Book_Count);
            }
            if(Trait_Value <= 0)
            {
                Trait_Type = 6;
            }
            if(Trait_Type == 6)
            {
                Trait_Value = 1;
            }
            switch(Trait_Type)
            {
                case 1:  // add books to Life
                {
                    if(
                        (_players[itr2].spellranks[sbr_Life] < 11)
                        &&
                        (_players[itr2].spellranks[sbr_Death] == 0)
                    )
                    {
                        _players[itr2].spellranks[sbr_Life] += Trait_Value;
                    }
                    if(_players[itr2].spellranks[sbr_Life] > 11)
                    {
                        _players[itr2].spellranks[sbr_Life] = 11;
                    }
                } break;

                case 2:  // add books to Death
                {
                    if(
                        (_players[itr2].spellranks[sbr_Death] < 11)
                        &&
                        (_players[itr2].spellranks[sbr_Life] == 0)
                    )
                    {
                        _players[itr2].spellranks[sbr_Death] += Trait_Value;
                    }
                    if(_players[itr2].spellranks[sbr_Death] > 11)
                    {
                        _players[itr2].spellranks[sbr_Death] = 11;
                    }
                } break;

                case 3:  // add books to Nature
                {
                    if(
                        (_players[itr2].spellranks[sbr_Nature] < 11)
                    )
                    {
                        _players[itr2].spellranks[sbr_Nature] += Trait_Value;
                    }
                    if(_players[itr2].spellranks[sbr_Nature] > 11)
                    {
                        _players[itr2].spellranks[sbr_Nature] = 11;
                    }
                } break;

                case 4:  // add books to Sorcery
                {
                    if(
                        (_players[itr2].spellranks[sbr_Sorcery] < 11)
                    )
                    {
                        _players[itr2].spellranks[sbr_Sorcery] += Trait_Value;
                    }
                    if(_players[itr2].spellranks[sbr_Sorcery] > 11)
                    {
                        _players[itr2].spellranks[sbr_Sorcery] = 11;
                    }
                } break;

                case 5:  // add books to Chaos
                {
                    if(
                        (_players[itr2].spellranks[sbr_Chaos] < 11)
                    )
                    {
                        _players[itr2].spellranks[sbr_Chaos] += Trait_Value;
                    }
                    if(_players[itr2].spellranks[sbr_Chaos] > 11)
                    {
                        _players[itr2].spellranks[sbr_Chaos] = 11;
                    }
                } break;

                case 6:
                case 7:
                case 8:
                {
                    Trait_Value = (Random(NUM_WIZARD_SPECIAL_ABILITIES) - 1);
                    if( (Trait_Value >= wsa_Chaos_Mastery) && (Trait_Value <= wsa_Divine_Power))
                    {
                        for(Realm_Index = 0; Realm_Index <= 4; Realm_Index++)
                        {
                            Bookshelf[Realm_Index] = _players[itr2].spellranks[Realm_Index];
                        }
                        if(_players[itr2].chaos_mastery > 0)
                        {
                            Bookshelf[sbr_Chaos] = 0;
                        }
                        if(_players[itr2].nature_mastery > 0)
                        {
                            Bookshelf[sbr_Nature] = 0;
                        }
                        if(_players[itr2].sorcery_mastery > 0)
                        {
                            Bookshelf[sbr_Sorcery] = 0;
                        }
                        /* Semantically reads as "drop this realm from the weighted retort pool if the wizard already has the ability, or if picks are too tight to afford it" */
                        if((_players[itr2].divine_power > 0) || ((Picks_Left - 2) < Picks_Used))
                        {
                            Bookshelf[sbr_Life] = 0;
                        }
                        if((_players[itr2].infernal_power > 0) || ((Picks_Left - 2) < Picks_Used))
                        {
                            Bookshelf[sbr_Death] = 0;
                        }
                        Realm_Retort_Available = 0;
                        for(Realm_Index = 0; Realm_Index <= 4; Realm_Index++)
                        {
                            Realm_Retort_Available += Bookshelf[Realm_Index];
                        }
                        if(Realm_Retort_Available != 0)
                        {
                            Trait_Value = (2 + Get_Weighted_Choice(&Bookshelf[0], 5));
                            switch(Trait_Value)
                            {
                                case 2: { Trait_Value = wsa_Nature_Mastery;  } break;
                                case 3: { Trait_Value = wsa_Sorcery_Mastery; } break;
                                case 4: { Trait_Value = wsa_Chaos_Mastery;   } break;
                                case 5: { Trait_Value = wsa_Divine_Power;    } break;
                                case 6: { Trait_Value = wsa_Infernal_Power;  } break;
                            }
                            wsa_ptr[Trait_Value] = ST_TRUE;
                        }
                    }
                    else
                    {
                        if(
                            (Trait_Value == wsa_Warlord)
                            ||
                            (Trait_Value == wsa_Channeller)
                            ||
                            (Trait_Value == wsa_Famous)
                        )
                        {
                            if((Picks_Left - 1) > Picks_Used)
                            {
                                wsa_ptr[Trait_Value] = ST_TRUE;
                            }
                        }
                        else if(Trait_Value == wsa_Myrran)
                        {
                            if((Picks_Left - 2) > Picks_Used)
                            {
                                wsa_ptr[Trait_Value] = ST_TRUE;
                            }
                        }
                        else
                        {
                            wsa_ptr[Trait_Value] = ST_TRUE;
                        }
                    }
                } break;  /* END  case {6,7,8}: */

            }  /* END:  switch(Trait_Type) */

            /* after ... switch(Trait_Type) ... ```dasm @@AfterSwitchTrait_Type:  ; default ``` */

            Picks_Used = (
                _players[itr2].spellranks[sbr_Life]
                +
                _players[itr2].spellranks[sbr_Death]
                +
                _players[itr2].spellranks[sbr_Nature]
                +
                _players[itr2].spellranks[sbr_Chaos]
                +
                _players[itr2].spellranks[sbr_Sorcery]
            );
            Book_Count = Picks_Used;
            for(itr1 = 0; itr1 < NUM_WIZARD_SPECIAL_ABILITIES; itr1++)
            {
                if(wsa_ptr[itr1] == ST_FALSE)
                {
                    continue;
                }
                if(itr1 == wsa_Myrran)
                {
                    Picks_Used += 3;
                }
                else if(
                        (itr1 == wsa_Warlord)
                        ||
                        (itr1 == wsa_Infernal_Power)
                        ||
                        (itr1 == wsa_Divine_Power)
                        ||
                        (itr1 == wsa_Famous)
                        ||
                        (itr1 == wsa_Channeller)
                )
                {
                    Picks_Used += 2;
                }
                else
                {
                    Picks_Used += 1;
                }
            }

        }

    }


    /* Phase 6: Myrran count sanity check */
    myrran_count = 0;
    for(itr1 = 1; itr1 < _num_players; itr1++)
    {
        if(_players[itr1].myrran != ST_FALSE)
        {
            myrran_count++;
        }
    }
    /* Requirement: At least 1 (but no more than 2) Myrrans on high difficulty / player counts */
    if((myrran_count < 1 || myrran_count > 2) && (_difficulty > god_Normal && _num_players > 3))
    {
        goto BeginTopLevelPlayerLoop;
    }
    /* dead-code for changing Sss'ra */
    /* Patch for specific preset table index if logic fails? */
    /* 9Ah 154d / sizeof(s_WIZARD_PRESET) = 7 */
    /* {"Sss'ra",  4,  0,  0, 0,  4, wsa_Myrran}, */
    /* _wizard_presets_table[7].special = ST_UNDEFINED; */
    /* _wizard_presets_table[7].chaos = 7; */


/*
END:  ¿ jmp     @@BeginTopLevelPlayerLoop ?
*/


    /* Phase ? */
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        if(
            (_players[itr2].spellranks[sbr_Nature] > 3)
            &&
            (banners[BNR_Green] == ST_FALSE)
        )
        {
            _players[itr2].banner_id = BNR_Green;
            banners[BNR_Green] = ST_TRUE;
        }
        else if(
            (_players[itr2].spellranks[sbr_Chaos] > 3)
            &&
            (banners[BNR_Red] == ST_FALSE)
        )
        {
            _players[itr2].banner_id = BNR_Red;
            banners[BNR_Red] = ST_TRUE;
        }
        else if(
            (_players[itr2].spellranks[sbr_Sorcery] > 3)
            &&
            (banners[BNR_Blue] == ST_FALSE)
        )
        {
            _players[itr2].banner_id = BNR_Blue;
            banners[BNR_Blue] = ST_TRUE;
        }
        else if(
            (_players[itr2].spellranks[sbr_Death] > 3)
            &&
            (banners[BNR_Purple] == ST_FALSE)
        )
        {
            _players[itr2].banner_id = BNR_Purple;
            banners[BNR_Purple] = ST_TRUE;
        }
        else
        {
            /* Pick random available banner */
            do {
                random_banner = (Random(5) - 1);
            } while(banners[random_banner] == ST_TRUE);
            banners[random_banner] = ST_TRUE;
            _players[itr2].banner_id = (uint8_t)random_banner;
        }
    }

    // @@Done

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Init_Computer_Players_Wizard_Profile rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p10
/**
 * @brief Builds the initial spell-library state for every active player.
 *
 * @details
 * Clears each player's spell availability table, then repopulates it from the
 * player's starting realm books and preselected starting spell lists. The
 * routine marks the Arcane realm's fixed researchable spells, grants starting
 * spells for each non-zero realm, and expands the library with knowable
 * common, uncommon, rare, and very-rare spells according to the wizard's book
 * count in that realm.
 *
 * For fully mastered realms, all spells in the realm are made knowable and
 * the appropriate starting spells are promoted to known status. The function
 * also ensures that Spell of Mastery is always known, and that Artificer
 * wizards begin with Detect Magic and Disjunction.
 *
 * Side effects are limited to mutation of the global `_players[]` spell
 * library tables.
 *
 * @return void
 *
 * @note Expects `_num_players`, `_players[]`, and `_player_start_spells[]` to
 *       have already been initialized by earlier new-game setup.
 * @note Uses `Random()` while selecting knowable spells, so the resulting
 *       library distribution is intentionally nondeterministic.
 */
void Init_Computer_Players_Spell_Library(void)
{
    int16_t Availability_Array[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // 2-byte, signed, sizeof 10
    int16_t Available_Spells = 0;
    int16_t Common_Index = 0;
    int16_t InRarity_Index = 0;
    int16_t Availability_Limit = 0;
    int16_t sri = 0;
    int16_t sbr = 0;
    int16_t itr = 0;
    int16_t itr2 = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr = 0; itr < _num_players; itr++)
    {

        for(sbr = 0; sbr < NUM_MAGIC_REALMS; sbr++)
        {
            for(sri = 0; sri < NUM_SPELLS_PER_MAGIC_REALM; sri++)
            {
                _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + sri)] = 0;
            }
        }

        for(sbr = 0; sbr < NUM_MAGIC_REALMS; sbr++)
        {

            if(sbr == sbr_Arcane)
            {
                _players[itr].spells_list[((sbr * 40) +  0)] = sls_Knowable;  // spl_Demon_Lord
                _players[itr].spells_list[((sbr * 40) +  1)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  2)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  3)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  4)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  5)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  6)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  7)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  8)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  9)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) + 10)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) + 11)] = sls_Knowable;
                // WTF. Why not 0x0C?  ¿ spl_Summon_Champion ?  _players[itr].spells_list[((sbr * 40) + 12)] = 1;
                _players[itr].spells_list[((sbr * 40) + 13)] = sls_Known;  // spl_Spell_Of_Mastery
                continue;
            }

            if(_players[itr].spellranks[sbr] == 0)
            {
                continue;
            }
        

            if(_players[itr].spellranks[sbr] != 11)
            {

                for(itr2 = 0; itr2 < 10; itr2++)
                {
                    Availability_Array[itr2] = 0;
                }

                switch(_players[itr].spellranks[sbr])
                {
                    case  1: { Availability_Limit =  3; } break;
                    case  2: { Availability_Limit =  5; } break;
                    case  3: { Availability_Limit =  6; } break;
                    case  4: { Availability_Limit =  7; } break;
                    case  5: { Availability_Limit =  8; } break;
                    case  6: { Availability_Limit =  9; } break;
                    case  7: { Availability_Limit = 10; } break;
                    case  8: { Availability_Limit = 10; } break;
                    case  9: { Availability_Limit = 10; } break;
                    case 10: { Availability_Limit = 10; } break;
                }

                switch(sbr)
                {
                    case sbr_Nature:
                    {
                        for(itr2 = 0; (_players[itr].spellranks[sbr] - 1) > itr2; itr2++)
                        {
                            _players[itr].spells_list[
                                (
                                    (
                                        (
                                            (sbr * NUM_SPELLS_PER_MAGIC_REALM)
                                            +
                                            (_player_start_spells[itr].realms[pssr_Nature].spells[itr2])
                                        )
                                    - 1
                                    ) % NUM_SPELLS_PER_MAGIC_REALM
                                )
                            ] = sls_Known;
                        }
                    } break;
                    case sbr_Sorcery:
                    {
                        for(itr2 = 0; (_players[itr].spellranks[sbr] - 1) > itr2; itr2++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + (_player_start_spells[itr].realms[pssr_Sorcery].spells[itr2] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        }
                    } break;
                    case sbr_Chaos:
                    {
                        for(itr2 = 0; (_players[itr].spellranks[sbr] - 1) > itr2; itr2++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + (_player_start_spells[itr].realms[pssr_Chaos].spells[itr2] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        }
                    } break;
                    case sbr_Life:
                    {
                        for(itr2 = 0; (_players[itr].spellranks[sbr] - 1) > itr2; itr2++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + (_player_start_spells[itr].realms[pssr_Life].spells[itr2] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        }
                    } break;
                    case sbr_Death:
                    {
                        for(itr2 = 0; (_players[itr].spellranks[sbr] - 1) > itr2; itr2++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + (_player_start_spells[itr].realms[pssr_Death].spells[itr2] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        }
                    } break;
                }

                // IDA purple #32
                Available_Spells = (_players[itr].spellranks[sbr] - 1);
                LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @before-common    p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                while(Available_Spells < Availability_Limit)
                {
                    itr2 = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);
                    if(_players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] == sls_Unknown)
                    {
                        _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] = sls_Knowable;
                    }
                    Available_Spells = 0;
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        if(_players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] != sls_Unknown)
                        {
                            Available_Spells++;
                        }
                    }
                }
                LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @after-common     p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);

/*
BEGIN: Knowable - Uncommon
*/

                if(_players[itr].spellranks[sbr] > 7)
                {
                    for(itr2 = 0; itr2 < 10; itr2++)
                    {
                        _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 10 + itr2)] = sls_Knowable;
                    }
                }
                else
                {
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        Availability_Array[itr2] = 0;
                    }
                    switch(_players[itr].spellranks[sbr])
                    {
                        case  1: { Availability_Limit =  1; } break;
                        case  2: { Availability_Limit =  2; } break;
                        case  3: { Availability_Limit =  3; } break;
                        case  4: { Availability_Limit =  4; } break;
                        case  5: { Availability_Limit =  5; } break;
                        case  6: { Availability_Limit =  6; } break;
                        case  7: { Availability_Limit =  8; } break;
                        case  8: { Availability_Limit = 10; } break;
                        case  9: { Availability_Limit = 10; } break;
                        case 10: { Availability_Limit = 10; } break;
                    }
                    itr2 = 0;
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @before-uncommon  p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    while(itr2 < Availability_Limit)
                    {
                        InRarity_Index = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);
                        if(Availability_Array[InRarity_Index] != 1)
                        {
                            Availability_Array[InRarity_Index] = 1;
                            itr2++;
                        }
                    }
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @after-uncommon   p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        if(Availability_Array[itr2] == 1)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 10 + itr2)] = sls_Knowable;
                        }
                    }
                }

/*
END: Knowable - Uncommon
*/

/*
BEGIN: Knowable - Rare
*/
                
                if(_players[itr].spellranks[sbr] > 9)
                {
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_REALM; itr2++)
                    {
                        _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 20 + itr2)] = sls_Knowable;
                    }
                }
                else
                {
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        Availability_Array[itr2] = 0;
                    }
                    switch(_players[itr].spellranks[sbr])
                    {
                        case  1: { Availability_Limit = 0; } break;
                        case  2: { Availability_Limit = 1; } break;
                        case  3: { Availability_Limit = 2; } break;
                        case  4: { Availability_Limit = 3; } break;
                        case  5: { Availability_Limit = 4; } break;
                        case  6: { Availability_Limit = 5; } break;
                        case  7: { Availability_Limit = 6; } break;
                        case  8: { Availability_Limit = 7; } break;
                        case  9: { Availability_Limit = 9; } break;
                        // WTF  DNE  case 10: { Availability_Limit = ; } break;
                    }
                    itr2 = 0;
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @before-rare      p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    while(itr2 < Availability_Limit)
                    {
                        InRarity_Index = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);
                        if(Availability_Array[InRarity_Index] != 1)
                        {
                            Availability_Array[InRarity_Index] = 1;
                            itr2++;
                        }
                    }
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @after-rare       p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        if(Availability_Array[itr2] == 1)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 20 + itr2)] = sls_Knowable;
                        }
                    }
                }

/*
END: Knowable - Rare
*/

/*
BEGIN: Knowable - Very Rare
*/
                
                if(_players[itr].spellranks[sbr] > 9)
                {
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_REALM; itr2++)
                    {
                        _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 30 + itr2)] = sls_Knowable;
                    }
                }
                else
                {
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        Availability_Array[itr2] = 0;
                    }
                    switch(_players[itr].spellranks[sbr])
                    {
                        case  1: { Availability_Limit = 0; } break;
                        case  2: { Availability_Limit = 0; } break;
                        case  3: { Availability_Limit = 1; } break;
                        case  4: { Availability_Limit = 2; } break;
                        case  5: { Availability_Limit = 3; } break;
                        case  6: { Availability_Limit = 4; } break;
                        case  7: { Availability_Limit = 5; } break;
                        case  8: { Availability_Limit = 6; } break;
                        case  9: { Availability_Limit = 7; } break;
                        // WTF  DNE  case 10: { Availability_Limit = ; } break;
                    }
                    itr2 = 0;
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @before-very_rare p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    while(itr2 < Availability_Limit)
                    {
                        InRarity_Index = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);
                        if(Availability_Array[InRarity_Index] != 1)
                        {
                            Availability_Array[InRarity_Index] = 1;
                            itr2++;
                        }
                    }
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @after-very_rare  p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        if(Availability_Array[itr2] == 1)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 30 + itr2)] = sls_Knowable;
                        }
                    }
                }

/*
END: Knowable - Very Rare
*/

            }
            else  /* (_players[itr].spellranks[sbr] == 11) */
            {
                for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_REALM; itr2++)
                {
                    _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] = sls_Knowable;
                }

                switch(sbr)
                {
                    case sbr_Nature:
                    {
                        for(Common_Index = 0; Common_Index < 10; Common_Index++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                        }
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Nature].spells[10] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Nature].spells[11] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Nature].spells[12] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                    } break;
                    case sbr_Sorcery:
                    {
                        for(Common_Index = 0; Common_Index < 10; Common_Index++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                        }
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Sorcery].spells[10] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Sorcery].spells[11] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Sorcery].spells[12] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                    } break;
                    case sbr_Chaos:
                    {
                        for(Common_Index = 0; Common_Index < 10; Common_Index++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                        }
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Chaos].spells[10] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Chaos].spells[11] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Chaos].spells[12] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                    } break;
                    case sbr_Life:
                    {
                        for(Common_Index = 0; Common_Index < 10; Common_Index++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                        }
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Life].spells[10] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Life].spells[11] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Life].spells[12] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                    } break;
                    case sbr_Death:
                    {
                        for(Common_Index = 0; Common_Index < 10; Common_Index++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                        }
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Death].spells[10] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Death].spells[11] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[pssr_Death].spells[12] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                    } break;
                }

            }  /* END  if(_players[itr].spellranks[sbr] != 11) else */

        }

        _players[itr].spells_list[spl_Demon_Lord] = sls_Known;
        if(_players[itr].artificer != 0)
        {
            _players[itr].spells_list[spl_Detect_Magic] = sls_Known;
            _players[itr].spells_list[spl_Disjunction] = sls_Known;
        }

    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Init_Computer_Players_Spell_Library rng_call=%llu", (unsigned long long)g_random_call_count);
    
}
/* GEMINI */
#if 0
void Init_Computer_Players_Spell_Library__GEMINI(void)
{
    int availability_array[10]; /* [bp-22h] Availability_Array */
    int available_spells;       /* [bp-0Eh] Available_Spells */
    int common_index;           /* [bp-0Ch] Common_Index */
    int in_rarity_index;        /* [bp-0Ah] InRarity_Index */
    int availability_limit;     /* [bp-08h] Availability_Limit */
    int sri;                    /* [bp-06h] sri - spell realm index */
    int sbr;                    /* [bp-04h] sbr - spell book realm index */
    int itr;                    /* [bp-02h] itr - player iterator */
    int idk_itr_10;             /* di      itr2 */

    for (itr = 0; itr < _num_players; itr++)
    {
        /* Clear all spell library entries for this player */
        for (sbr = 0; sbr < 6; sbr++)
        {
            for (sri = 0; sri < 40; sri++)
            {
                /* _players[itr].spells_list[(sbr * 40) + sri] = 0; */
                _players[itr].spells_list[sbr * 40 + sri] = 0;
            }
        }

        for (sbr = 0; sbr < NUM_MAGIC_REALMS; sbr++)
        {
            if (sbr == SBR_ARCANE)
            {
                /* Arcane spells 0-11 are researchable (1), 13 is starting (2) */
                _players[itr].spells_list[sbr * 40 + 0] = 1;
                _players[itr].spells_list[sbr * 40 + 1] = 1;
                _players[itr].spells_list[sbr * 40 + 2] = 1;
                _players[itr].spells_list[sbr * 40 + 3] = 1;
                _players[itr].spells_list[sbr * 40 + 4] = 1;
                _players[itr].spells_list[sbr * 40 + 5] = 1;
                _players[itr].spells_list[sbr * 40 + 6] = 1;
                _players[itr].spells_list[sbr * 40 + 7] = 1;
                _players[itr].spells_list[sbr * 40 + 8] = 1;
                _players[itr].spells_list[sbr * 40 + 9] = 1;
                _players[itr].spells_list[sbr * 40 + 10] = 1;
                _players[itr].spells_list[sbr * 40 + 11] = 1;
                _players[itr].spells_list[sbr * 40 + 13] = 2;
                continue;
            }

            if (_players[itr].spellranks[sbr] == 0)
            {
                continue;
            }

            if (_players[itr].spellranks[sbr] == 11) /* Max books/special case */
            {
                for (idk_itr_10 = 0; idk_itr_10 < 40; idk_itr_10++)
                {
                    _players[itr].spells_list[sbr * 40 + idk_itr_10] = 1;
                }

                /* Grant all common spells as starting spells */
                for (common_index = 0; common_index < 10; common_index++)
                {
                    _players[itr].spells_list[sbr * 40 + common_index] = 2;
                }

                /* Grant additional starting spells based on realm */
                switch (sbr)
                {
                    case 0: /* Nature */
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[10] - 1) % 40)] = 2;
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[11] - 1) % 40)] = 2;
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[12] - 1) % 40)] = 2;
                        break;
                    case 1: /* Sorcery */
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[62] - 1) % 40)] = 2;
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[63] - 1) % 40)] = 2;
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[64] - 1) % 40)] = 2;
                        break;
                    case 2: /* Chaos */
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[36] - 1) % 40)] = 2;
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[37] - 1) % 40)] = 2;
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[38] - 1) % 40)] = 2;
                        break;
                    case 3: /* Life */
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[49] - 1) % 40)] = 2;
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[50] - 1) % 40)] = 2;
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[51] - 1) % 40)] = 2;
                        break;
                    case 4: /* Death */
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[23] - 1) % 40)] = 2;
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[24] - 1) % 40)] = 2;
                        _players[itr].spells_list[sbr * 40 + ((_player_start_spells.realms[itr].spells[25] - 1) % 40)] = 2;
                        break;
                }
            }
            else
            {
                /* Standard initialization for Rank 1-10 */
                for (idk_itr_10 = 0; idk_itr_10 < 10; idk_itr_10++) availability_array[idk_itr_10] = 0;

                /* Determine starting common spells limit */
                switch (_players[itr].spellranks[sbr])
                {
                    case 1: availability_limit = 3; break;
                    case 2: availability_limit = 5; break;
                    case 3: availability_limit = 6; break;
                    case 4: availability_limit = 7; break;
                    case 5: availability_limit = 8; break;
                    case 6: availability_limit = 9; break;
                    default: availability_limit = 10; break;
                }

                /* Select starting common spells from predefined list */
                for (idk_itr_10 = 0; idk_itr_10 < (_players[itr].spellranks[sbr] - 1); idk_itr_10++)
                {
                    int spell_idx;
                    switch (sbr)
                    {
                        case 0: spell_idx = (_player_start_spells.realms[itr].spells[0 + idk_itr_10] - 1) % 40; break;
                        case 1: spell_idx = (_player_start_spells.realms[itr].spells[39 + idk_itr_10] - 1) % 40; break;
                        case 2: spell_idx = (_player_start_spells.realms[itr].spells[52 + idk_itr_10] - 1) % 40; break;
                        case 3: spell_idx = (_player_start_spells.realms[itr].spells[26 + idk_itr_10] - 1) % 40; break;
                        case 4: spell_idx = (_player_start_spells.realms[itr].spells[13 + idk_itr_10] - 1) % 40; break;
                        default: spell_idx = 0; break;
                    }
                    _players[itr].spells_list[sbr * 40 + spell_idx] = 2;
                }

                /* Fill out the library until availability_limit commons are present */
                available_spells = _players[itr].spellranks[sbr] - 1;
                while (available_spells < availability_limit)
                {
                    idk_itr_10 = (Random(10) - 1);
                    if (_players[itr].spells_list[sbr * 40 + idk_itr_10] == 0)
                    {
                        _players[itr].spells_list[sbr * 40 + idk_itr_10] = 1;
                    }
                    
                    /* Re-count library size */
                    available_spells = 0;
                    for (idk_itr_10 = 0; idk_itr_10 < 10; idk_itr_10++)
                    {
                        if (_players[itr].spells_list[sbr * 40 + idk_itr_10] != 0)
                            available_spells++;
                    }
                }

                /* Process Uncommon Spells (10-19) */
                if (_players[itr].spellranks[sbr] > 7)
                {
                    for (idk_itr_10 = 0; idk_itr_10 < 10; idk_itr_10++)
                        _players[itr].spells_list[sbr * 40 + 10 + idk_itr_10] = 1;
                }
                else
                {
                    for (idk_itr_10 = 0; idk_itr_10 < 10; idk_itr_10++) availability_array[idk_itr_10] = 0;
                    switch (_players[itr].spellranks[sbr])
                    {
                        case 1: availability_limit = 1; break;
                        case 2: availability_limit = 2; break;
                        case 3: availability_limit = 3; break;
                        case 4: availability_limit = 4; break;
                        case 5: availability_limit = 5; break;
                        case 6: availability_limit = 6; break;
                        case 7: availability_limit = 8; break;
                        default: availability_limit = 10; break;
                    }
                    for (idk_itr_10 = 0; idk_itr_10 < availability_limit; )
                    {
                        in_rarity_index = (Random(10) - 1);
                        if (availability_array[in_rarity_index] != 1)
                        {
                            availability_array[in_rarity_index] = 1;
                            idk_itr_10++;
                        }
                    }
                    for (idk_itr_10 = 0; idk_itr_10 < 10; idk_itr_10++)
                    {
                        if (availability_array[idk_itr_10] == 1)
                            _players[itr].spells_list[sbr * 40 + 10 + idk_itr_10] = 1;
                    }
                }

                /* Process Rare Spells (20-29) */
                if (_players[itr].spellranks[sbr] > 9)
                {
                    for (idk_itr_10 = 0; idk_itr_10 < 10; idk_itr_10++)
                        _players[itr].spells_list[sbr * 40 + 20 + idk_itr_10] = 1;
                }
                else
                {
                    for (idk_itr_10 = 0; idk_itr_10 < 10; idk_itr_10++) availability_array[idk_itr_10] = 0;
                    switch (_players[itr].spellranks[sbr])
                    {
                        case 1: availability_limit = 0; break;
                        case 2: availability_limit = 1; break;
                        case 3: availability_limit = 2; break;
                        case 4: availability_limit = 3; break;
                        case 5: availability_limit = 4; break;
                        case 6: availability_limit = 5; break;
                        case 7: availability_limit = 6; break;
                        case 8: availability_limit = 7; break;
                        case 9: availability_limit = 9; break;
                    }
                    for (idk_itr_10 = 0; idk_itr_10 < availability_limit; )
                    {
                        in_rarity_index = (Random(10) - 1);
                        if (availability_array[in_rarity_index] != 1)
                        {
                            availability_array[in_rarity_index] = 1;
                            idk_itr_10++;
                        }
                    }
                    for (idk_itr_10 = 0; idk_itr_10 < 10; idk_itr_10++)
                    {
                        if (availability_array[idk_itr_10] == 1)
                            _players[itr].spells_list[sbr * 40 + 20 + idk_itr_10] = 1;
                    }
                }

                /* Process Very Rare Spells (30-39) */
                if (_players[itr].spellranks[sbr] > 9)
                {
                    for (idk_itr_10 = 0; idk_itr_10 < 10; idk_itr_10++)
                        _players[itr].spells_list[sbr * 40 + 30 + idk_itr_10] = 1;
                }
                else
                {
                    for (idk_itr_10 = 0; idk_itr_10 < 10; idk_itr_10++) availability_array[idk_itr_10] = 0;
                    switch (_players[itr].spellranks[sbr])
                    {
                        case 1:
                        case 2: availability_limit = 0; break;
                        case 3: availability_limit = 1; break;
                        case 4: availability_limit = 2; break;
                        case 5: availability_limit = 3; break;
                        case 6: availability_limit = 4; break;
                        case 7: availability_limit = 5; break;
                        case 8: availability_limit = 6; break;
                        case 9: availability_limit = 7; break;
                    }
                    for (idk_itr_10 = 0; idk_itr_10 < availability_limit; )
                    {
                        in_rarity_index = (Random(10) - 1);
                        if (availability_array[in_rarity_index] != 1)
                        {
                            availability_array[in_rarity_index] = 1;
                            idk_itr_10++;
                        }
                    }
                    for (idk_itr_10 = 0; idk_itr_10 < 10; idk_itr_10++)
                    {
                        if (availability_array[idk_itr_10] == 1)
                            _players[itr].spells_list[sbr * 40 + 30 + idk_itr_10] = 1;
                    }
                }
            }
        }

        /* Fixed spell indices (e.g., Spell of Mastery, special traits) */
        _players[itr].spells_list[200] = 2; /* 0xC8 */
        if (_players[itr].artificer != 0)
        {
            _players[itr].spells_list[206] = 2; /* 0xCE */
            _players[itr].spells_list[210] = 2; /* 0xD2 */
        }
    }
}
#endif


// MGC o56p11
// drake178: NEWG_ClearEvents()
// ¿ ~== MoO2  Module: REPORT  Initialize_Reports_() ?
/*
; clears the event data structure by setting the status
; of all events to 0
*/
/*

*/
void Initialize_Events(void)
{
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    events_table->last_event_turn = 50;
    events_table->Meteor_Status = 0;
    events_table->Gift_Status = 0;
    events_table->Disjunction_Status = 0;
    events_table->marriage_status = 0;
    events_table->Earthquake_Status = 0;
    events_table->Pirates_Status = 0;
    events_table->Plague_Status = 0;
    events_table->Rebellion_Status = 0;
    events_table->Donation_Status = 0;
    events_table->Depletion_Status = 0;
    events_table->minerals_status = 0;
    events_table->Population_Boom_Status = 0;
    events_table->Good_Moon_Status = 0;
    events_table->Bad_Moon_Status = 0;
    events_table->Conjunction_Chaos_Status = 0;
    events_table->Conjunction_Nature_Status = 0;
    events_table->Conjunction_Sorcery_Status = 0;
    events_table->Mana_Short_Status = 0;
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Initialize_Events rng_call=%llu", (unsigned long long)g_random_call_count);
}

// MGC o56p12
// drake178: NEWG_CreateHeroes()
// MoO2  Module: INITGAME  Init_Leaders_()
void Init_Heroes(void)
{
    int32_t abilities = 0;
    int16_t all_picks = 0;
    int16_t mage_picks = 0;
    int16_t casting_skill = 0;
    int16_t itr_hero_types = 0;
    int16_t itr_players = 0;
    int16_t warrior_picks = 0;  // _DI_

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr_players = 0; itr_players < 5; itr_players++)
    {

        for(itr_hero_types = 0; itr_hero_types < NUM_HERO_TYPES; itr_hero_types++)
        {

            _HEROES2[itr_players]->heroes[itr_hero_types].Level = 0;

            warrior_picks = 0;

            mage_picks = 0;

            all_picks = 0;

            switch(_hero_premade_table[itr_hero_types].pick_type)
            {
                case 0:  /* Warrior */
                {
                    warrior_picks = _hero_premade_table[itr_hero_types].pick_count;
                } break;
                case 1:  /* Mage */
                {
                    mage_picks = _hero_premade_table[itr_hero_types].pick_count;
                } break;
                case 2:  /* All/Any/Both/Either/Neither */
                {
                    all_picks = _hero_premade_table[itr_hero_types].pick_count;
                } break;
            }

            abilities = _hero_premade_table[itr_hero_types].abilities;

            casting_skill = _hero_premade_table[itr_hero_types].casting_skill;

            if(all_picks > 0)
            {
                warrior_picks = all_picks;
                mage_picks = all_picks;
            }

            while((warrior_picks > 0) || (mage_picks > 0))
            {

                switch(Random(14) - 1)
                {
                    case 0:  /* Leadership */
                    {
                        if(((abilities & HSA_LEADERSHIP2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_LEADERSHIP) != 0) { abilities ^= HSA_LEADERSHIP; abilities |= HSA_LEADERSHIP2; } else { abilities |= HSA_LEADERSHIP; } warrior_picks--; mage_picks--; }
                    } break;
                    case 1:  /* Legendary */
                    {
                        if(((abilities & HSA_LEGENDARY2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_LEGENDARY) != 0) { abilities ^= HSA_LEGENDARY; abilities |= HSA_LEGENDARY2; } else { abilities |= HSA_LEGENDARY; } warrior_picks--; mage_picks--; }
                    } break;
                    case 2:  /* Armsmaster */
                    {
                        if(((abilities & HSA_ARMSMASTER2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_ARMSMASTER) != 0) { abilities ^= HSA_ARMSMASTER; abilities |= HSA_ARMSMASTER2; } else { abilities |= HSA_ARMSMASTER; } warrior_picks--; mage_picks--; }
                    } break;
                    case 3:  /* Blademaster */
                    {
                        if(((abilities & HSA_BLADEMASTER2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_BLADEMASTER) != 0) { abilities ^= HSA_BLADEMASTER; abilities |= HSA_BLADEMASTER2; } else { abilities |= HSA_BLADEMASTER; } warrior_picks--; mage_picks--; }
                    } break;
                    case 4:  /* Might */
                    {
                        if(((abilities & HSA_MIGHT2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_MIGHT) != 0) { abilities ^= HSA_MIGHT; abilities |= HSA_MIGHT2; } else { abilities |= HSA_MIGHT; } warrior_picks--; mage_picks--; }
                    } break;
                    case 5:  /* Constitution */
                    {
                        if(((abilities & HSA_CONSTITUTION2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_CONSTITUTION) != 0) { abilities ^= HSA_CONSTITUTION; abilities |= HSA_CONSTITUTION2; } else { abilities |= HSA_CONSTITUTION; } warrior_picks--; mage_picks--; }
                    } break;
                    case 6:  /* Arcane Power */
                    {
                        if(((abilities & HSA_ARCANE_POWER2) == 0) && (mage_picks > 0))
                        {
                            // ; BUG: the Knight has no caster picks, this should be $1E (the Elven Archer)
                            if(itr_hero_types != ut_Knight)  /* ¿ should ut_ElvenArcher  = 30 ? */
                            {
                                // ; BUG: excludes the Warlock and the Chaos Warrior, this should be $1E (Ranged_Lightning)
                                if(
                                    ((abilities & HSA_ARCANE_POWER) != 0)
                                    &&
                                    (_unit_type_table[itr_hero_types].Ranged_Type >= rat_Fireball)  /* ¿ should be rat_Lightning    =  30 ? */
                                    &&
                                    (_unit_type_table[itr_hero_types].Ranged_Type <= rat_Nat_Bolt)
                                )
                                {
                                    abilities ^= HSA_ARCANE_POWER;
                                    abilities |= HSA_ARCANE_POWER2;
                                }
                                else
                                {
                                    abilities |= HSA_ARCANE_POWER;
                                }
                            }
                        }
                    } break;
                    case 7:  /* Prayermaster */
                    {
                        if(((abilities & HSA_PRAYERMASTER2) == 0) && (mage_picks > 0)) { if((abilities & HSA_PRAYERMASTER) != 0) { abilities ^= HSA_PRAYERMASTER; abilities |= HSA_PRAYERMASTER2; } else { abilities |= HSA_PRAYERMASTER; } warrior_picks--; mage_picks--; }
                    } break;
                    case 8:  /* Casting Skill */
                    {
                        if(mage_picks > 0) { casting_skill++; warrior_picks--; mage_picks--; }
                    } break;
                    case 9:  /* Noble */
                    {
                        if(((abilities & HSA_NOBLE) == 0) && (itr_hero_types == ut_Chosen)) { abilities |= HSA_NOBLE; warrior_picks--; mage_picks--; }
                    } break;
                    case 10:  /* Charm */
                    {
                        if(((abilities & HSA_CHARMED) == 0) && ((_unit_type_table[itr_hero_types].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)) { abilities |= HSA_CHARMED; warrior_picks--; mage_picks--; }
                    } break;
                    case 11:  /* Lucky */
                    {
                        if((abilities & HSA_LUCKY) == 0) { abilities |= HSA_LUCKY; warrior_picks--; mage_picks--; }
                    } /* BUGBUG: case 11/Luck missing `break;`, falls through to case 12/Agility */
                    case 12:  /* Agility */
                    {
                        if(((abilities & HSA_AGILITY2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_AGILITY) != 0) { abilities |= HSA_AGILITY2; } else { abilities |= HSA_AGILITY; } warrior_picks--; mage_picks--; }
                    } break;
                    case 13:  /* Sage */
                    {
                        if(((abilities & HSA_SAGE2) == 0) && (mage_picks > 0)) { if((abilities & HSA_SAGE) != 0) { abilities |= HSA_SAGE2; } else { abilities |= HSA_SAGE; } warrior_picks--; mage_picks--; }
                    } break;

                }

            }

            _HEROES2[itr_players]->heroes[itr_hero_types].abilities = abilities;
            _HEROES2[itr_players]->heroes[itr_hero_types].Casting_Skill = (int8_t)casting_skill;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[0] = (uint8_t)_hero_premade_table[itr_hero_types].spell_1;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[1] = (uint8_t)_hero_premade_table[itr_hero_types].spell_2;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[2] = (uint8_t)_hero_premade_table[itr_hero_types].spell_3;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[3] = (uint8_t)_hero_premade_table[itr_hero_types].spell_4;

            LOG_TRACE(LOG_CAT_GENERAL,
                "[Init_Heroes] p=%d ht=%d table_spells=(%d,%d,%d,%d) struct_after=(0x%02X,0x%02X,0x%02X,0x%02X) rng_call=%llu",
                itr_players, itr_hero_types,
                _hero_premade_table[itr_hero_types].spell_1,
                _hero_premade_table[itr_hero_types].spell_2,
                _hero_premade_table[itr_hero_types].spell_3,
                _hero_premade_table[itr_hero_types].spell_4,
                _HEROES2[itr_players]->heroes[itr_hero_types].Spells[0],
                _HEROES2[itr_players]->heroes[itr_hero_types].Spells[1],
                _HEROES2[itr_players]->heroes[itr_hero_types].Spells[2],
                _HEROES2[itr_players]->heroes[itr_hero_types].Spells[3],
                (unsigned long long)g_random_call_count);

        }

    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o56p13
// drake178: CRP_MSG_Clear()
/*
zeroes the counters of all chancellor (scroll)
message types, clearing all event messages
*/
/*

*/
void Initialize_Messages(void)
{
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    MSG_UnitLost_Count = 0;
    MSG_UnitKilled_Count = 0;
    MSG_UEsLost_Count = 0;
    MSG_CEsLost_Count = 0;
    MSG_GEs_Lost = 0;
    MSG_CityGrowth_Count = 0;
    MSG_CityDeath_Count = 0;
    MSG_BldLost_Count = 0;
    MSG_CityLost_Count = 0;
    MSG_CityGained_Count = 0;
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Initialize_Messages rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p14
// drake178: UU_ITEM_SetHeroSlots()
// UU_ITEM_SetHeroSlots()

// MGC o56p15
// drake178: WIZ_ConsolidateBooks()
/*
; PATCHED / rewritten
;
; if the wizard has two different realms of spellbooks,
; consolidates them into a single realm, chosen
; randomly from the two they have
;
; INCONSISTENT: ignores the Chaos + Nature combination
; WARNING: will not work properly if defaults are
;  altered to have 3 realms or Life + Death
*/
/*
  Death & Chaos
  Death & Nature
  Death & Sorcery
   Life & Chaos
   Life & Nature
   Life & Sorcery
Sorcery & Chaos
Sorcery & Nature
*/
void Consolidate_Spell_Book_Realms(int16_t player_idx)
{

    /*
        BEGIN:  Death & Chaos
    */
    if((_players[player_idx].spellranks[sbr_Death] != 0) && (_players[player_idx].spellranks[sbr_Chaos] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Death] += _players[player_idx].spellranks[sbr_Chaos];
            _players[player_idx].spellranks[sbr_Chaos] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Chaos] += _players[player_idx].spellranks[sbr_Death];
            _players[player_idx].spellranks[sbr_Death] = 0;
        }
    }
    /*
        END:  Death & Chaos
    */

    /*
        BEGIN:  Death & Nature
    */
    if((_players[player_idx].spellranks[sbr_Death] != 0) && (_players[player_idx].spellranks[sbr_Nature] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Death] += _players[player_idx].spellranks[sbr_Nature];
            _players[player_idx].spellranks[sbr_Nature] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Nature] += _players[player_idx].spellranks[sbr_Death];
            _players[player_idx].spellranks[sbr_Death] = 0;
        }
    }
    /*
        END:  Death & Nature
    */
   
    /*
        BEGIN:  Death & Sorcery
    */
    if((_players[player_idx].spellranks[sbr_Death] != 0) && (_players[player_idx].spellranks[sbr_Sorcery] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Death] += _players[player_idx].spellranks[sbr_Sorcery];
            _players[player_idx].spellranks[sbr_Sorcery] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Sorcery] += _players[player_idx].spellranks[sbr_Death];
            _players[player_idx].spellranks[sbr_Death] = 0;
        }
    }
    /*
        END:  Death & Sorcery
    */
   
    /*
        BEGIN:  Life & Chaos
    */
    if((_players[player_idx].spellranks[sbr_Life] != 0) && (_players[player_idx].spellranks[sbr_Chaos] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Life] += _players[player_idx].spellranks[sbr_Chaos];
            _players[player_idx].spellranks[sbr_Chaos] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Chaos] += _players[player_idx].spellranks[sbr_Life];
            _players[player_idx].spellranks[sbr_Life] = 0;
        }
    }
    /*
        END:  Life & Chaos
    */
   
    /*
        BEGIN:  Life & Nature
    */
    if((_players[player_idx].spellranks[sbr_Life] != 0) && (_players[player_idx].spellranks[sbr_Nature] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Life] += _players[player_idx].spellranks[sbr_Nature];
            _players[player_idx].spellranks[sbr_Nature] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Nature] += _players[player_idx].spellranks[sbr_Life];
            _players[player_idx].spellranks[sbr_Life] = 0;
        }
    }
    /*
        END:  Life & Nature
    */
   
    /*
        BEGIN:  Life & Sorcery
    */
    if((_players[player_idx].spellranks[sbr_Life] != 0) && (_players[player_idx].spellranks[sbr_Sorcery] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Life] += _players[player_idx].spellranks[sbr_Sorcery];
            _players[player_idx].spellranks[sbr_Sorcery] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Sorcery] += _players[player_idx].spellranks[sbr_Life];
            _players[player_idx].spellranks[sbr_Life] = 0;
        }
    }
    /*
        END:  Life & Sorcery
    */
   
    /*
        BEGIN:  Sorcery & Chaos
    */
    if((_players[player_idx].spellranks[sbr_Sorcery] != 0) && (_players[player_idx].spellranks[sbr_Chaos] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Sorcery] += _players[player_idx].spellranks[sbr_Chaos];
            _players[player_idx].spellranks[sbr_Chaos] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Chaos] += _players[player_idx].spellranks[sbr_Sorcery];
            _players[player_idx].spellranks[sbr_Sorcery] = 0;
        }
    }
    /*
        END:  Sorcery & Chaos
    */
   
    /*
        BEGIN:  Sorcery & Nature
    */
    if((_players[player_idx].spellranks[sbr_Sorcery] != 0) && (_players[player_idx].spellranks[sbr_Nature] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Sorcery] += _players[player_idx].spellranks[sbr_Nature];
            _players[player_idx].spellranks[sbr_Nature] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Nature] += _players[player_idx].spellranks[sbr_Sorcery];
            _players[player_idx].spellranks[sbr_Sorcery] = 0;
        }
    }
    /*
        END:  Sorcery & Nature
    */

}


// MGC o56p16
/*
¿ ~ Player_Base_Casting_Skill() ?
*/
/**
 * @brief Converts stored casting-skill investment into nominal casting skill.
 *
 * @details
 * Interprets `_players[player_idx].spell_casting_skill` as an accumulated pool
 * of invested skill points and walks the game's triangular progression one
 * step at a time until the running threshold meets or exceeds that stored
 * investment. The resulting step count becomes the player's nominal casting
 * skill, with an additional +10 applied when the wizard has the Archmage
 * retort.
 *
 * This routine is used during player initialization to translate invested
 * casting points back into the displayed base skill value.
 *
 * @param player_idx Index of the player record in `_players[]`.
 *
 * @return The player's nominal casting skill, including the Archmage bonus
 *         when applicable.
 *
 * @note Uses the same progression as `Player_Base_Casting_Skill()` in
 *       [MoM/src/NEXTTURN.c](MoM/src/NEXTTURN.c).
 */
int16_t Player_Base_Casting_Skill__MGC(int16_t player_idx)
{
    int32_t num = 0;
    int16_t twos = 0;
    int16_t casting_skill = 0;
    twos = 0;
    casting_skill = 0;
    num = 0;
    while(_players[player_idx].spell_casting_skill > num)
    {
        casting_skill += 1;
        twos += 2;
        num += twos;
    }
    if(_players[player_idx].archmage > 0)
    {
        casting_skill += 10;
    }
    return casting_skill;
}


// MGC o56p17
// drake178: RNG_WeightedPick16()
// RNG_WeightedPick16()
