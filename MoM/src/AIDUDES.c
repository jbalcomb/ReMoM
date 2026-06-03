/*
    AI - Data

    WIZARDS.EXE
        ¿ ovr145 ?
*/

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"
#include "../../STU/src/AI_METRICS.h"
#include "../../platform/include/Platform.h"  /* Platform_Get_Millies() */

#include "../../MoX/src/EMS/EMS.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"

#include "AIBUILD.h"
#include "AIDATA.h"
#include "AIMOVE.h"
#include "AISPELL.h"
#include "CITYCALC.h"
#include "NEXTTURN.h"
#include "RACETYPE.h"
#include "SETTLE.h"
#include "Spellbook.h"
#include "Terrain.h"
#include "UNITTYPE.h"
#include "UnitMove.h"

#include "AIDUDES.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>     /* memcpy() memset(), stu_strcat(), stu_strcpy(), stricmp() */



// WZD dseg:6F60                                                 BEGIN:  ovr145 - Initialized Data

// WZD dseg:6F60
int16_t m_niu_ai_turn_eval_var = 0;

// WZD dseg:6F62 02 03 04 05 06 07 08 00                         UU_IDK_OVL_AI_7x1 db  2, 3, 4, 5, 6, 7, 8, 0
// WZD dseg:6F6A 00 00                                           dw 0
// WZD dseg:6F6C 00 00                                           dw 0
// WZD dseg:6F6E 0A 00                                           dw 10
// WZD dseg:6F70 0A 00                                           dw 10
// WZD dseg:6F72 32 00                                           dw 50
// WZD dseg:6F74 32 00                                           dw 50

// WZD dseg:6F74                                                 END:  ovr145 - Initialized Data



/*
    WIZARDS.EXE  ovr145
*/

// WZD o145p01
// drake178: RP_WIZ_AllowMercs()
/*
; returns 1 if it is at least turn 50, and the player
; can afford to spend the specified amount of gold
;
; This function should not exist - both of these
; factors are already checked in the main mercenary
; generator function, turn also has lower limit
*/
/*
    decides if the CP/NP should accept the offer of mercenaries
*/
int16_t AI_Accept_Mercenaries(int16_t player_idx, int16_t cost)
{

    if(
        (_turn < 50)
        ||
        ((_players[player_idx].gold_reserve / 2) < cost)
    )
    {
        return ST_FALSE;
    }
    else
    {
        return ST_TRUE;
    }

}


static void AI_Log_Metrics(void)
{
    int16_t player_idx = 0;
    int16_t itr = 0;
    int16_t unit_counts[NUM_PLAYERS];
    int16_t city_counts[NUM_PLAYERS];

    for (player_idx = 0; player_idx < NUM_PLAYERS; player_idx++)
    {
        unit_counts[player_idx] = 0;
        city_counts[player_idx] = 0;
    }

    for (itr = 0; itr < _units; itr++)
    {
        if (_UNITS[itr].owner_idx >= 0 && _UNITS[itr].owner_idx < NUM_PLAYERS)
        {
            unit_counts[_UNITS[itr].owner_idx]++;
        }
    }

    for (itr = 0; itr < _cities; itr++)
    {
        if (_CITIES[itr].owner_idx >= 0 && _CITIES[itr].owner_idx < NUM_PLAYERS)
        {
            city_counts[_CITIES[itr].owner_idx]++;
        }
    }

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_METRICS: Turn %d  Difficulty %d  Players %d", _turn, _difficulty, _num_players);

    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_METRICS: P0 %-20s units=%-4d cities=%-3d gold=%-5d mana=%-5d (Human)",
        _players[HUMAN_PLAYER_IDX].name,
        unit_counts[HUMAN_PLAYER_IDX],
        city_counts[HUMAN_PLAYER_IDX],
        _players[HUMAN_PLAYER_IDX].gold_reserve,
        _players[HUMAN_PLAYER_IDX].mana_reserve);

    for (player_idx = 1; player_idx < _num_players; player_idx++)
    {
        LOG_DEBUG(LOG_CAT_AIMOVE, "AI_METRICS: P%d %-20s units=%-4d cities=%-3d gold=%-5d mana=%-5d active=%d",
            player_idx,
            _players[player_idx].name,
            unit_counts[player_idx],
            city_counts[player_idx],
            _players[player_idx].gold_reserve,
            _players[player_idx].mana_reserve,
            _FORTRESSES[player_idx].active);
    }

    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_METRICS: NPC %-19s units=%-4d cities=%-3d raider_acc=%-3d monster_acc=%-3d",
        "(Neutral)",
        unit_counts[NEUTRAL_PLAYER_IDX],
        city_counts[NEUTRAL_PLAYER_IDX],
        _players[NEUTRAL_PLAYER_IDX].casting_cost_original,
        _players[NEUTRAL_PLAYER_IDX].average_unit_cost);
#endif
}


// WZD o145p02
// ¿ MoO2  Module: AIDATA  Compute_AI_Data_() ?
void AI_Next_Turn(void)
{
    int i;                  /* _DI_itr_units__peace */
    int player_idx;         /* _SI_itr_players */
    int other_player_idx;   /* _DI_itr_units__peace (re-used) */
    // int16_t itr_players = 0;  // _SI_
    // int16_t itr_units = 0;  // _DI_
    // int16_t itr_players2 = 0;  // _DI_
#ifdef STU_DEBUG
    uint64_t ntc_start_ms = Platform_Get_Millies();
#endif

#ifdef STU_DEBUG
    LOG_INFO(LOG_CAT_AIDUDES, "DEBUG: [%s, %d]: BEGIN: AI_Next_Turn()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: BEGIN: AI_Next_Turn()", __FILE__, __LINE__);
    LOG_TRACE(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: BEGIN: AI_Next_Turn()", __FILE__, __LINE__);
#endif

/* CLAUDE */ #ifdef STU_DEBUG
/* CLAUDE */ #define PHASE(CALL) do { uint64_t _ps = Platform_Get_Millies(); CALL; { uint64_t _pe = Platform_Get_Millies(); LOG_INFO(LOG_CAT_AIDUDES, "[NEXTTURN] phase %-48s = %llu ms", #CALL, (unsigned long long)(_pe - _ps)); LOG_TRACE(LOG_CAT_AIMOVE, "[NEXTTURN] phase %-48s = %llu ms", #CALL, (unsigned long long)(_pe - _ps)); } } while(0)
/* CLAUDE */ #else
/* CLAUDE */ #define PHASE(CALL) CALL
/* CLAUDE */ #endif


    /* Unit sanity/bounds check loop */
    for (i = 0; i < _units; i++)
    {
        /* Sanity check Plane (wp) */
        if (_UNITS[i].wp < 0 || _UNITS[i].wp > 1)
        {
            _UNITS[i].wp = 0;
            _UNITS[i].owner_idx = ST_UNDEFINED;
        }

        /* Sanity check World X (wx) */
        if (_UNITS[i].wx < 0 || _UNITS[i].wx >= WORLD_WIDTH)
        {
            _UNITS[i].wx = 0;
            _UNITS[i].owner_idx = ST_UNDEFINED;
        }

        /* Sanity check World Y (wy) */
        if (_UNITS[i].wy < 0 || _UNITS[i].wy >= WORLD_HEIGHT)
        {
            _UNITS[i].wy = 0;
            _UNITS[i].owner_idx = ST_UNDEFINED;
        }
    }

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: === BEGIN Turn %d ===", _turn);
#endif

    PHASE(EMMDATAH_Map());
    PHASE(Allocate_AI_Data());

    /* Main AI Player processing loop (Skip Human Player 0) */
    for (player_idx = 1; player_idx < _num_players; player_idx++)
    {
        /* Check for Time Stop effect */
        if (g_timestop_player_num != 0)
        {
            if ((g_timestop_player_num - 1) != player_idx)
            {
#ifdef STU_DEBUG
                LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: Player %d (%s) SKIPPED (Time Stop)", player_idx, _players[player_idx].name);
#endif
                continue;
            }
        }

        /* Check if Wizard is active or returning */
        /* MoO2 ~ `s_PLAYER.eliminated == ST_FALSE` */
        if (_FORTRESSES[player_idx].active != ST_TRUE && _players[player_idx].casting_spell_idx != spl_Spell_Of_Return)
        {
#ifdef STU_DEBUG
            LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: Player %d (%s) SKIPPED (inactive)", player_idx, _players[player_idx].name);
#endif
            continue;
        }

        /* Update Summoning Circle re-evaluation counter */
        AI_SCircle_Reevals[player_idx]--;

#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: --- Player %d (%s) BEGIN ---", player_idx, _players[player_idx].name);
#endif

        PHASE(AI_Evaluate_Hostility(player_idx));

        PHASE(AI_Evaluate_Magic_Power_Strategy(player_idx));

        m_niu_ai_turn_eval_var = 0;  /* OON XREF */

        PHASE(Player_Hostile_Opponents(player_idx));

        PHASE(AI_Player_Calculate_Target_Values(player_idx));

        PHASE(AI_Landmass_Values_And_Strengths(player_idx));  // uses the arrays just populated in AI_Player_Calculate_Target_Values()

        PHASE(AI_Choose_War_Landmass(player_idx));  /* populates _ai_landmass_war_targets[]; uses the arrays just populated in AI_Landmass_Values_And_Strengths() */

        /* Handle Overland Casting Completion */
        if (_players[player_idx].casting_cost_remaining <= 0 && _players[player_idx].casting_spell_idx != spl_NONE)
        {
            // Cast_Spell_Overland__WIP(player_idx);
            PHASE(EMMDATAH_Map());
            _players[player_idx].casting_spell_idx = spl_NONE;
            if (AI_Eval_After_Spell == ST_TRUE)
            {
                PHASE(Allocate_AI_Data());
                PHASE(Player_Hostile_Opponents(player_idx));
                PHASE(AI_Player_Calculate_Target_Values(player_idx));
            }
        }

        /* Handle New Spell Selection if not casting */
        if (_players[player_idx].casting_spell_idx == spl_NONE)
        {
            PHASE(AI_Spell_Select__STUB(player_idx));
        }

        PHASE(EMMDATAH_Map());
        PHASE(AI_Update_Magic_Power(player_idx));
        PHASE(AI_Sanity_Check_Overland_Enchantments(player_idx));
        PHASE(AI_Update_Gold_And_Mana_Reserves(player_idx));
        PHASE(AI_Update_Gold_Income_And_Food_Income(player_idx));

        Some_AI_Turn_Var_2 = 0;
        Some_AI_Turn_Var_3 = 0;

        /* Decrement peace treaties durations */
        for (other_player_idx = 0; other_player_idx < _num_players; other_player_idx++)
        {
            if (_players[player_idx].peace_duration[other_player_idx] != 0)
            {
                _players[player_idx].peace_duration[other_player_idx]--;
            }
        }

        PHASE(Player_All_Colony_Autobuild(player_idx));
        PHASE(AI_Evaluation_Map(player_idx));
        PHASE(AI_Evaluate_Continents(player_idx));
        PHASE(AI_Set_Unit_Orders(player_idx));
        
        PHASE(EMMDATAH_Map());
        PHASE(AI_Kill_Excess_Settlers_And_Engineers(player_idx));

#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: --- Player %d (%s) END ---", player_idx, _players[player_idx].name);
#endif
    }

    PHASE(EMMDATAH_Map());

    /* Reset Move_Failed flag for all non-human units */
    for (i = 0; i < _units; i++)
    {
        if (_UNITS[i].owner_idx != HUMAN_PLAYER_IDX)
        {
            _UNITS[i].Move_Failed = ST_FALSE;
        }
    }

    /* AI Unit Movement Phase */
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: === Movement Phase ===");
#endif
    for (player_idx = 1; player_idx < _num_players; player_idx++)
    {
        /* Check for Time Stop effect */
        if (g_timestop_player_num != 0)
        {
            if ((g_timestop_player_num - 1) != player_idx)
            {
                continue;
            }
        }

        /* Check if Wizard is active or returning */
        if (_FORTRESSES[player_idx].active == ST_TRUE || _players[player_idx].casting_spell_idx == spl_Spell_Of_Return)
        {
#ifdef STU_DEBUG
            LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: Moving units for Player %d (%s)", player_idx, _players[player_idx].name);
#endif
            PHASE(AI_MoveUnits(player_idx));
        }
    }

    PHASE(EMMDATAH_Map());

    /* Neutral Player Turn Processing */
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: === Neutral Player Phase ===");
#endif
    PHASE(Player_All_Colony_Autobuild(NEUTRAL_PLAYER_IDX));
    PHASE(NPC_Farmers());
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: NPC_Farmers done");
#endif
    PHASE(NPC_Destinations());
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: NPC_Destinations done");
#endif
    PHASE(AI_MoveUnits(NEUTRAL_PLAYER_IDX));
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: NPC movement done");
#endif

    /* Event Generation */
    PHASE(Make_Raiders());
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: Make_Raiders done");
#endif
    PHASE(Make_Monsters());
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: Make_Monsters done");
#endif

    /* Cleanup and Stasis */
    PHASE(NPC_Excess_Garrison());
    PHASE(AI_Hopeless_Stasis());

    AI_Log_Metrics();
    AI_Metrics_Emit_Turn_Summary(_turn, _difficulty, _num_players);
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "AI_TURN: === END Turn %d ===", _turn);
#endif

#ifdef STU_DEBUG
    LOG_INFO(LOG_CAT_AIDUDES, "DEBUG: [%s, %d]: END: AI_Next_Turn()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: END: AI_Next_Turn()", __FILE__, __LINE__);
    LOG_TRACE(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: END: AI_Next_Turn()", __FILE__, __LINE__);
#endif

}


// WZD o145p03
/*
uses values just calculated in AI_Player_Calculate_Target_Values()
odd that it calculates landmass_idx instead of using the stack struct value
*/
/**
 * @brief Aggregates per-landmass military strength and strategic value totals
 *        for one AI player.
 *
 * Using the city, garrison, and stack valuation data produced by
 * @c AI_Player_Calculate_Target_Values(), this routine clears the shared
 * landmass summary tables for both planes and then repopulates them from the
 * player's own stacks, hostile stacks, intact lairs, city values, and magic
 * nodes. Own and enemy military pressure are tracked separately from own and
 * enemy strategic value so later AI passes can decide where the player is weak
 * or overcommitted.
 *
 * After accumulation, the function computes one final balance value per plane
 * and landmass by subtracting enemy strength and a defense-overhead fraction of
 * the player's own value from the player's own strength. Positive balances are
 * then reduced by a fixed 25-point hysteresis threshold before being written to
 * the final strength-ratio table.
 *
 * @param player_idx Index of the wizard whose ownership perspective should be
 *                   used when classifying node value as friendly or enemy.
 *
 * @return This function does not return a value. It refreshes the shared
 *         arrays @c _ai_landmass_own_strengths,
 *         @c _ai_landmass_enemy_strengths,
 *         @c _ai_landmass_own_values,
 *         @c _ai_landmass_enemy_values, and
 *         @c _ai_landmass_strength_ratios.
 *
 * @note The routine depends on the stack and city valuation buffers having
 *       already been rebuilt for the same player earlier in the AI turn.
 * @note Intact lairs always contribute to enemy strength, and magic node value
 *       is scaled by @c (_magic + 1) * 2 before being added to the owning or
 *       opposing landmass totals.
 */
void AI_Landmass_Values_And_Strengths(int16_t player_idx)
{
    int16_t strength_ratio = 0;
    int16_t wp = 0;
    int16_t landmass_idx = 0;
    int16_t itr = 0;
    int16_t node_value = 0;

    /* 1. Initialize arrays for all continents across both planes */
    for(itr = 0; itr < NUM_LANDMASSES; itr++)
    {
        _ai_landmass_own_strengths[ARCANUS_PLANE][itr] = 0;
        _ai_landmass_own_strengths[MYRROR_PLANE][itr] = 0;
        _ai_landmass_enemy_strengths[ARCANUS_PLANE][itr] = 0;
        _ai_landmass_enemy_strengths[MYRROR_PLANE][itr] = 0;
        _ai_landmass_own_values[ARCANUS_PLANE][itr] = 0;
        _ai_landmass_own_values[MYRROR_PLANE][itr] = 0;
        _ai_landmass_enemy_values[ARCANUS_PLANE][itr] = 0;
        _ai_landmass_enemy_values[MYRROR_PLANE][itr] = 0;
    }

    /* 2. Calculate military strength of AI's own stacks on each continent */
    /* IDA: light-purple */
    for(itr = 0; itr < _ai_all_own_stack_count; itr++)
    {
        wp = _ai_all_own_stacks[itr].wp;
        landmass_idx = _landmasses[((wp * WORLD_SIZE) + (_ai_all_own_stacks[itr].wy * WORLD_WIDTH) + _ai_all_own_stacks[itr].wx)];
        if(landmass_idx < NUM_LANDMASSES)
        {
            /* Add 1/10th of stack value to the continent's own strength */
            _ai_landmass_own_strengths[wp][landmass_idx] += (_ai_all_own_stacks[itr].value / 10);
        }
    }

    /* 3. Calculate military strength of enemy stacks on each continent */
    /* IDA: grey-purple */
    for(itr = 0; itr < _ai_all_enemy_stack_count; itr++)
    {
        wp = _ai_all_enemy_stacks[itr].wp;
        landmass_idx = _landmasses[((wp * WORLD_SIZE) + (_ai_all_enemy_stacks[itr].wy * WORLD_WIDTH) + _ai_all_enemy_stacks[itr].wx)];
        if(landmass_idx < NUM_LANDMASSES)
        {
            _ai_landmass_enemy_strengths[wp][landmass_idx] += (_ai_all_enemy_stacks[itr].value / 10);
        }
    }

    /* 4. Add strength of monsters inside intact Lairs to enemy strength */
    for(itr = 0; itr < NUM_LAIRS; itr++)
    {
        if(_LAIRS[itr].intact != ST_TRUE) { continue; }
        wp = _LAIRS[itr].wp;
        landmass_idx = _landmasses[((wp * WORLD_SIZE) + (_LAIRS[itr].wy * WORLD_WIDTH) + _LAIRS[itr].wx)];
        if(landmass_idx < NUM_LANDMASSES)
        {
            /* Strength = Guard Count * (Unit Cost / 10) */
            /* guard_count is masked with 0Fh (low nibble) */
            _ai_landmass_enemy_strengths[wp][landmass_idx] += ((_LAIRS[itr].guard1_count & 0x0F) * (_unit_type_table[_LAIRS[itr].guard1_unit_type].cost / 10));
            _ai_landmass_enemy_strengths[wp][landmass_idx] += ((_LAIRS[itr].guard2_count & 0x0F) * (_unit_type_table[_LAIRS[itr].guard2_unit_type].cost / 10));
        }
    }

    /* 5. Accumulate strategic value of cities on each continent */
    for(itr = 0; itr < _cities; itr++)
    {
        wp = _CITIES[itr].wp;
        landmass_idx = _landmasses[((wp * WORLD_SIZE) + (_CITIES[itr].wy * WORLD_WIDTH) + _CITIES[itr].wx)];
        if(landmass_idx < NUM_LANDMASSES)
        {
            _ai_landmass_own_values[wp][landmass_idx] = _ai_all_own_city_values[itr];
            _ai_landmass_enemy_values[wp][landmass_idx] = _ai_all_enemy_city_values[itr];
        }
    }

    /* 6. Accumulate strategic value of Magic Nodes on each continent */
    for(itr = 0; itr < NUM_NODES; itr++)
    {
        wp = _NODES[itr].wp;
        landmass_idx = _landmasses[((wp * WORLD_SIZE) + (_NODES[itr].wy * WORLD_WIDTH) + _NODES[itr].wx)];
        if(landmass_idx < NUM_LANDMASSES)
        {
            /* convert game option magic level from index to number */
            /* Value = Power * (Global Magic + 1) * 2 */
            node_value = _NODES[itr].power * (_magic + 1) * 2;
            if(_NODES[itr].owner_idx == player_idx)
            {
                _ai_landmass_own_values[wp][landmass_idx] += node_value;
            }
            else
            {
                _ai_landmass_enemy_values[wp][landmass_idx] += node_value;
            }
        }
    }

    /* 7. Final Balance Calculation: (Own Strength - 10% of Own Value) - Enemy Strength */
    for(wp = 0; wp < 2; wp++)
    {
        for(landmass_idx = 0; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {
            /* the own value subtraction is a "defense overhead" - the more value on the landmass, the more you want to protect it */
            strength_ratio = (
                _ai_landmass_own_strengths[wp][landmass_idx]
                -
                (_ai_landmass_own_values[wp][landmass_idx] / 10)
                -
                _ai_landmass_enemy_strengths[wp][landmass_idx]
            );

            /* Hysteresis/Thresholding: If positive, subtract 25 to favor defensive posture unless significantly ahead. */
            if(strength_ratio > 0)
            {
                strength_ratio -= 25;  /* DEDU  how much and for how long could 25 really impact the decision? */
                if(strength_ratio < 0)
                {
                    strength_ratio = 0;
                }
            }
            _ai_landmass_strength_ratios[wp][landmass_idx] = strength_ratio;
        }
    }

}


// WZD o145p04
// drake178: AI_Power_Distrib()
/*
sets the AI's power distribution percentages based
on objective, magic strategy, current skill & mana,
spells left to research, and spell being cast

contains multiple BUGs that prohibit this function
from working properly once the AI has ran out of
spells to research
*/
/*

~ Players_Update_Magic_Power()

"...available magic power is divided between researching, reserves and spell casting skill..."
"...spell casting skill..." ... "...total current spell casting skill..."
"...adjust how much of your total magic power you allocate to your mana reserves, researching efforts and spell casting skill..."

OSG  Page 406  (PDF Page 403)
I. Computer Players Prepare
...
    F. Adjust mana ratios for wand, research, and skill level.

*/
void AI_Update_Magic_Power(int16_t player_idx)
{
    int16_t Mana_Total = 0;
    int16_t Nominal_Skill = 0;
    int16_t itr = 0;
    int16_t Unresearched_Spells = 0;
    int16_t itr_spells = 0;
    int16_t itr_realms = 0;
    int16_t desired_magic_investment_type = 0;

    Nominal_Skill = Player_Base_Casting_Skill(player_idx);

    Mana_Total = _players[player_idx].mana_reserve;

    _players[player_idx].reevaluate_magic_power_countdown -= 1;

    if(_players[player_idx].reevaluate_magic_power_countdown > 0)
    {
        return;
    }

    _players[player_idx].reevaluate_magic_power_countdown = (15 + Random(10));

    // ; check the wizard's spell library to see if there are
    // ; any spells still marked as "knowable"
    // ; 
    // ; BUG: spells that are already research
    // ; candidates are ignored by this loop

    Unresearched_Spells = 0;

    for(itr_realms = 0; ((itr_realms < NUM_MAGIC_REALMS) & (Unresearched_Spells == 0)); itr_realms++)
    {

        for(itr_spells = 0; ((itr_spells < NUM_SPELLS_PER_MAGIC_REALM) & (Unresearched_Spells == 0)); itr_spells++)
        {

            if(_players[player_idx].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == 2 /* S_Knowable */)
            {

                Unresearched_Spells++;

            }

        }

    }

    // ; BUG: branching this here removes the AI's ability
    // ; to adjust power distribution entirely when all
    // ; spells have been researched, including the intended
    // ; modifiers for researching and casting the SoM

    if(Unresearched_Spells == 0)
    {

        // ; BUG: should be added, not replacing

        _players[player_idx].skill_ratio = (_players[player_idx].research_ratio / 2);

        _players[player_idx].research_ratio = 0;

    }
    else
    {

        _players[player_idx].research_ratio = 10;
        _players[player_idx].skill_ratio = 10;
        _players[player_idx].mana_ratio = 30;

        for(itr = 0; itr < 5; itr++)
        {

            desired_magic_investment_type = (Random(3) - 1);

            if(
                (_players[player_idx].Objective == OBJ_Theurgist)
                &&
                (Random(3) == 1)
            )
            {
                desired_magic_investment_type = 2;  /* Research */
            }

            if(
                (_players[player_idx].Objective == OBJ_Militarist)
                &&
                (Random(3) == 1)
            )
            {
                desired_magic_investment_type = 1;  /* Skill */
            }

            if(
                (_players[player_idx].Objective == OBJ_Perfectionist)
                &&
                (Random(3) == 1)
            )
            {
                desired_magic_investment_type = 0;
            }

            if(
                (Player_Base_Casting_Skill(player_idx) > 100)
                &&
                (Random(3) == 1)
            )
            {
                desired_magic_investment_type = 2;  /* Research */  // same as OBJ_Theurgist
            }

            if(
                (_players[player_idx].alchemy != 0)
                &&
                (Random(3) == 1)
            )
            {
                desired_magic_investment_type = 0;  /* Mana */  // same as OBJ_Perfectionist
            }

            if(
                (_players[player_idx].magic_power_strategy == 6)
                &&
                (Random(3) == 1)
            )
            {
                desired_magic_investment_type = 0;  /* Mana */  // same as OBJ_Perfectionist
            }

            if(
                (_players[player_idx].magic_power_strategy == 1)
                &&
                (Random(3) == 1)
            )
            {
                desired_magic_investment_type = 2;  /* Research */  // same as OBJ_Theurgist
            }

            if(
                (_players[player_idx].magic_power_strategy == 2)
                &&
                (Random(3) == 1)
            )
            {
                desired_magic_investment_type = 1;  /* Skill */  // same as OBJ_Militarist
            }

            if(
                (_players[player_idx].mana_reserve < (_turn * 3))
                &&
                (Random(2) == 1)
            )
            {
                desired_magic_investment_type = 0;  /* Mana */  // same as OBJ_Perfectionist
            }

            if(_players[player_idx].mana_reserve > 1000)
            {
                if(Random(2) == 1)
                {
                    desired_magic_investment_type = 2;  /* Research */  // same as OBJ_Theurgist
                }
                else
                {
                    desired_magic_investment_type = 1;  /* Skill */  // same as OBJ_Militarist
                }
            }

            if(
                (desired_magic_investment_type == 1)  /* Skill */
                &&
                ((Mana_Total * 4) < Nominal_Skill)
            )
            {
                desired_magic_investment_type = 0;  /* Mana */ // same as OBJ_Perfectionist
            }

            switch(desired_magic_investment_type)
            {
                case 0:  /* Mana */
                {
                    _players[player_idx].mana_ratio += 10;
                } break;
                case 1:  /* Skill */
                {
                    _players[player_idx].skill_ratio += 10;
                } break;
                case 2:  /* Research */
                {
                    _players[player_idx].research_ratio += 10;
                } break;
            }

        }

        /* OGBUG  should be spl_Spell_Of_Mastery */
        if(_players[player_idx].researching_spell_idx == spl_Fire_Elemental)
        {
            _players[player_idx].research_ratio = 70;
            _players[player_idx].skill_ratio = 10;
            _players[player_idx].mana_ratio = 20;
        }

        if(_turn < 30)
        {
            _players[player_idx].research_ratio = 35;
            _players[player_idx].skill_ratio = 25;
            _players[player_idx].mana_ratio = 40;
        }

    }

    _players[player_idx].mana_ratio = ((100 - _players[player_idx].skill_ratio) - _players[player_idx].research_ratio);

}


// WZD o145p05
// drake178: WIZ_IsSpellKnown()
/*
; returns 1 if the spell is known, or 0 otherwise
*/
/*



*/
int16_t Player_Knows_Spell(int16_t player_idx, int16_t spell_idx)
{

    if(
        (spell_idx == 0)
        ||
        (_players[player_idx].spells_list[ ( (((spell_idx - 1) / NUM_SPELLS_PER_MAGIC_REALM) * NUM_SPELLS_PER_MAGIC_REALM) + ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM) ) ] == 2 /* S_Known */)
    )
    {

        return ST_FALSE;

    }
    else
    {

        return ST_TRUE;

    }

}


// WZD o145p06
// drake178: WIZ_KnownSpellCount()
/*
; returns the amount of spells known by the wizard
*/
/*



*/
int16_t Player_Known_Spell_Count(int16_t player_idx)
{
    int16_t itr_realms = 0;  // _CX_
    int16_t known_spell_count = 0;  // _DI_
    int16_t itr_spells = 0;  // _SI_

    known_spell_count = 0;

    for(itr_realms = 0; itr_realms < NUM_MAGIC_REALMS; itr_realms++)
    {

        for(itr_spells = 0; itr_spells < NUM_SPELLS_PER_MAGIC_REALM; itr_spells++)
        {

            if(_players[player_idx].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == 2 /* S_Known */)
            {

                known_spell_count++;

            }

        }

    }

    return known_spell_count;

}


// WZD o145p07
// drake178: AI_OVL_Eval_Assets()
/*
evaluates all overland cities and units, creating
tables for own stack capabilities, own and enemy
stack and garrison values, and city values

contains a host of BUGs, needs re-evaluating later!
*/
/*

sets values for own and enemy cities, stacks, garrisons
¿ used to calculate landmass values ?
¿ used to choose targets for positive and negative city (area) spells ?


*/
/**
 * @brief Rebuilds the AI's cached city, garrison, and stack target values for
 *        one player.
 *
 * This evaluator clears the shared overland-value tables and then walks every
 * city and unit on the map to classify them as owned or hostile relative to
 * @p player_idx. City entries receive heuristic values from population,
 * building count, race bonuses, and fortress presence. Unit entries are folded
 * into aggregate own-stack or enemy-stack records while also contributing to
 * nearby garrison-strength tables used by later continent and targeting logic.
 *
 * While aggregating units, the routine records stack capabilities such as
 * transport capacity, road building, purification, melding, wind walking,
 * settler status, and hero presence, and it suppresses strength contribution
 * for transports, construction units, and outpost-capable units in the same
 * places as the original game logic. At the end, it also refreshes the global
 * transport-unit count for the player.
 *
 * @param player_idx Index of the wizard whose perspective should be used when
 *                   classifying owned versus enemy cities and stacks.
 *
 * @return This function does not return a value. It mutates shared AI working
 *         buffers including @c _ai_all_enemy_garrison_strengths,
 *         @c _ai_all_own_garrison_strengths,
 *         @c _ai_all_enemy_city_values, @c _ai_all_own_city_values,
 *         @c _ai_all_enemy_stacks, @c _ai_all_own_stacks,
 *         @c _ai_all_enemy_stack_count, @c _ai_all_own_stack_count, and
 *         @c ai_transport_count.
 *
 * @note The function preserves several documented original-game bugs noted in
 *       the local comments, including the human-city iterator mix-up, the
 *       Troll-race check using @c rt_Standard, and the late-pass stack and
 *       garrison adjustments that use inconsistent indexes.
 * @note Neutral cities are always considered valid enemy-city targets, even
 *       when no hostility relationship exists.
 */
void AI_Player_Calculate_Target_Values(int16_t player_idx)
{
    int16_t City_Loop_Var = 0;
    int16_t unit_has_watertravel = 0;
    int16_t unit_has_melding = 0;
    int16_t unit_is_hero = 0;
    int16_t unit_has_windwalking = 0;
    int16_t unit_has_airtravel = 0;
    int16_t effective_unit_strength = 0;
    int16_t seen_unit = 0;
    int16_t itr = 0;
    int16_t city_owner_idx = 0;
    int16_t itr_cities = 0;
    int16_t itr_units = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        _ai_all_enemy_garrison_strengths[itr_cities] = 0;
        _ai_all_own_garrison_strengths[itr_cities] = 0;
        _ai_all_enemy_city_values[itr_cities] = 0;
        _ai_all_own_city_values[itr_cities] = 0;

        city_owner_idx = _CITIES[itr_cities].owner_idx;

        if(city_owner_idx != player_idx)
        {

            if(
                (
                    (_players[player_idx].Hostility[city_owner_idx] >= 2)  /* high-hostility */
                    &&
                    (_players[player_idx].peace_duration[city_owner_idx] == 0)
                )
                ||
                (city_owner_idx == NEUTRAL_PLAYER_IDX)
            )
            {

                _ai_all_enemy_city_values[itr_cities] = _CITIES[itr_cities].population;

                if(
                    (_CITIES[itr_cities].race == rt_Beastmen)
                    ||
                    (_CITIES[itr_cities].race == rt_Dark_Elf)
                    ||
                    (_CITIES[itr_cities].race == rt_Draconian)
                    ||
                    (_CITIES[itr_cities].race == rt_Dwarf)
                    ||
                    (_CITIES[itr_cities].race == rt_Standard)  /* OGBUG  should be rt_Trolls */
                )
                {

                    _ai_all_enemy_city_values[itr_cities] *= 2;

                }

                _ai_all_enemy_city_values[itr_cities] += (_CITIES[itr_cities].bldg_cnt * 2);

                /* Fortress City Bonus */
                if(
                    (_FORTRESSES[city_owner_idx].wx == _CITIES[itr_cities].wx)
                    &&
                    (_FORTRESSES[city_owner_idx].wy == _CITIES[itr_cities].wy)
                    &&
                    (_FORTRESSES[city_owner_idx].wp == _CITIES[itr_cities].wp)
                    &&
                    (city_owner_idx != NEUTRAL_PLAYER_IDX)
                )
                {

                    _ai_all_enemy_city_values[itr_cities] += 200;

                }
                
                /* The Human Player is our favorite Enemy */
                /* OGBUG  should use itr_cities, not itr_units */
                if(_CITIES[itr_units].owner_idx == HUMAN_PLAYER_IDX)
                {

                    _ai_all_enemy_city_values[itr_cities] *= 2;

                }

            }

        }
        else
        {

            _ai_all_own_city_values[itr_cities] = _CITIES[itr_cities].population;

            if(
                (_CITIES[itr_cities].race == rt_Beastmen)
                ||
                (_CITIES[itr_cities].race == rt_Dark_Elf)
                ||
                (_CITIES[itr_cities].race == rt_Draconian)
                ||
                (_CITIES[itr_cities].race == rt_Dwarf)
                ||
                (_CITIES[itr_cities].race == rt_Standard)  /* OGBUG  should be rt_Trolls */
            )
            {

                _ai_all_own_city_values[itr_cities] *= 2;

            }

            _ai_all_own_city_values[itr_cities] += (_CITIES[itr_cities].bldg_cnt * 2);

            /* Fortress City Bonus */
            if(
                (_FORTRESSES[player_idx].wx == _CITIES[itr_cities].wx)
                &&
                (_FORTRESSES[player_idx].wy == _CITIES[itr_cities].wy)
                &&
                (_FORTRESSES[player_idx].wp == _CITIES[itr_cities].wp)
            )
            {

                _ai_all_own_city_values[itr_cities] += (_turn * 2);

            }

        }

    }

    _ai_all_own_stack_count = 0;
    _ai_all_enemy_stack_count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        /* ¿ OGBUG  gets here with -1 and illegally indexes arrays ? */
        /* HACK */  if(
        /* HACK */      (_UNITS[itr_units].owner_idx == ST_UNDEFINED)
        /* HACK */      ||
        /* HACK */      (_UNITS[itr_units].wp == ST_UNDEFINED)
        /* HACK */  )
        /* HACK */  {
        /* HACK */      continue;
        /* HACK */  }

        effective_unit_strength = Effective_Unit_Strength(itr_units);

        unit_has_airtravel = Unit_Has_AirTravel(itr_units);

        unit_has_windwalking = Unit_Has_WindWalking(itr_units);

        unit_has_watertravel = Unit_Has_WaterTravel(itr_units);

        /* OGBUG  ignores Barbarian Spearmen */
        if(_UNITS[itr_units].type < ut_BarbSwordsmen)  /* ¿ hero or boat ? */ 
        {

            unit_is_hero = ST_TRUE;

        }
        else
        {

            unit_is_hero = ST_FALSE;

        }

        if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_MELD) != 0)
        {

            unit_has_melding = ST_TRUE;

        }
        else
        {

            unit_has_melding = ST_FALSE;

        }

        // `seen_unit`: the intent is the standard linear-search-then-insert pattern

        if(_UNITS[itr_units].owner_idx != player_idx)
        {

            /* Enemy Unit Processing */

            if(_ai_all_enemy_stack_count >= 240) { continue; }

            seen_unit = ST_FALSE;

            for(itr = 0; ((itr < _cities) & (seen_unit == ST_FALSE)); itr++)
            {

                if(
                    (_UNITS[itr_units].wx == _CITIES[itr].wx)
                    &&
                    (_UNITS[itr_units].wy == _CITIES[itr].wy)
                    &&
                    (_UNITS[itr_units].wp == _CITIES[itr].wp)
                )
                {

                    seen_unit = ST_TRUE;

                }

            }


            for(itr = 0; ((itr < _ai_all_enemy_stack_count) & (seen_unit == ST_FALSE)); itr++)
            {

                if(
                    (_UNITS[itr_units].wx == _ai_all_enemy_stacks[itr].wx)
                    &&
                    (_UNITS[itr_units].wy == _ai_all_enemy_stacks[itr].wy)
                    &&
                    (_UNITS[itr_units].wp == _ai_all_enemy_stacks[itr].wp)
                )
                {

                    if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
                    {

                        effective_unit_strength = 0;

                    }

                    if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_CREATEOUTPOST) != 0)
                    {

                        effective_unit_strength = 0;

                    }

                    if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
                    {

                        effective_unit_strength = 0;

                    }

                    _ai_all_enemy_stacks[itr].value = effective_unit_strength;

                    seen_unit = ST_TRUE;

                }

            }

            /* not in a City Garrison and not in a known enemy stack, so must be previously unseen enemy stack */
            /* Units Loop; Enemy Units; New Stack; loc_D3D24; grey-purple (not light-purple) */
            if(seen_unit == ST_FALSE)
            {

                _ai_all_enemy_stacks[_ai_all_enemy_stack_count].wx = _UNITS[itr_units].wx;

                _ai_all_enemy_stacks[_ai_all_enemy_stack_count].wy = _UNITS[itr_units].wy;

                _ai_all_enemy_stacks[_ai_all_enemy_stack_count].wp = _UNITS[itr_units].wp;

                if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
                {

                    effective_unit_strength = 0;

                }

                if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_CREATEOUTPOST) != 0)
                {

                    effective_unit_strength = 0;

                }

                if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
                {

                    effective_unit_strength = 0;

                }

                _ai_all_enemy_stacks[_ai_all_enemy_stack_count].value = effective_unit_strength;

                _ai_all_enemy_stack_count++;

            }

            for(itr = 0; itr < _cities; itr++)
            {
                if(_UNITS[itr_units].wp == _CITIES[itr].wp)
                {
                    if(
                        (_UNITS[itr_units].wx == _CITIES[itr].wx)
                        &&
                        (_UNITS[itr_units].wy == _CITIES[itr].wy)
                    )
                    {
                        _ai_all_enemy_garrison_strengths[itr] += effective_unit_strength;
                    }
                    else
                    {
                        if(
                            (abs(_UNITS[itr_units].wx - _CITIES[itr].wx) < 3)
                            &&
                            (abs(_UNITS[itr_units].wy - _CITIES[itr].wy) < 3)
                        )
                        {
                            _ai_all_enemy_garrison_strengths[itr] += (effective_unit_strength / 2);
                        }
                    }
                }
            }

        }
        else
        {

            if(_ai_all_own_stack_count >= MAX_AI_STACKS) { continue; }
            
            seen_unit = ST_FALSE;

            for(itr = 0; ((itr < _ai_all_own_stack_count) & (seen_unit == ST_FALSE)); itr++)
            {

                if(
                    (_UNITS[itr_units].wx == _ai_all_own_stacks[itr].wx)
                    &&
                    (_UNITS[itr_units].wy == _ai_all_own_stacks[itr].wy)
                    &&
                    (_UNITS[itr_units].wp == _ai_all_own_stacks[itr].wp)
                )
                {

                    _ai_all_own_stacks[itr].unit_count += 1;

                    if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
                    {

                        _ai_all_own_stacks[itr].abilities |= AICAP_Transport;

                        _ai_all_own_stacks[itr].transport_capacity += _unit_type_table[_UNITS[itr_units].type].Transport;

                        effective_unit_strength = 0;

                    }
                    else
                    {

                        // subtract boatrider from capacity
                        if(
                            (unit_has_watertravel == ST_FALSE)
                            &&
                            (unit_has_airtravel == ST_FALSE)
                        )
                        {

                            _ai_all_own_stacks[itr].transport_capacity -= 1;

                        }

                    }

                    if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
                    {

                        _ai_all_own_stacks[itr].abilities |= AICAP_RoadBuild;

                        effective_unit_strength = 0;

                    }

                    if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_PURIFY) != 0)
                    {

                        _ai_all_own_stacks[itr].abilities |= AICAP_Purify;

                    }

                    if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_CREATEOUTPOST) != 0)
                    {

                        _ai_all_own_stacks[itr].abilities |= AICAP_Settler;

                        effective_unit_strength = 0;

                    }

                    if(
                        (player_idx == NEUTRAL_PLAYER_IDX)
                        &&
                        ((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_FANTASTIC) != 0)
                    )
                    {

                        _ai_all_own_stacks[itr].abilities |= AICAP_Settler;

                        effective_unit_strength = 0;

                    }

                    /* OGBUG  duplicate transport check */
                    if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
                    {

                        _ai_all_own_stacks[itr].abilities |= AICAP_Transport;

                        _ai_all_own_stacks[itr].transport_capacity += _unit_type_table[_UNITS[itr_units].type].Transport;

                        effective_unit_strength = 0;

                    }

                    if(
                        (unit_has_airtravel == ST_FALSE)
                        &&
                        (unit_has_watertravel == ST_FALSE)
                    )
                    {

                        _ai_all_own_stacks[itr].abilities ^= AICAP_LandOnly;

                    }

                    if(unit_has_windwalking == ST_TRUE)
                    {

                        _ai_all_own_stacks[itr].abilities |= AICAP_WindWalk;

                        _ai_all_own_stacks[itr].transport_capacity = 8;

                    }

                    if(unit_is_hero == ST_TRUE)
                    {

                        _ai_all_own_stacks[itr].abilities |= AICAP_Hero;

                        effective_unit_strength = ((effective_unit_strength * 2) / 3);

                    }

                    if(unit_has_melding == ST_TRUE)
                    {

                        _ai_all_own_stacks[itr].abilities |= AICAP_Melding;

                    }

                    /* OGBUG  duplicate construction check */
                    if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
                    {

                        _ai_all_own_stacks[itr].abilities |= AICAP_RoadBuild;

                        effective_unit_strength = 0;

                    }

                    _ai_all_own_stacks[itr].value += effective_unit_strength;

                    if(_UNITS[itr_units].Status == us_BuildRoad)
                    {

                        _ai_all_own_stacks[itr].unit_status = us_BuildRoad;

                    }
                    else
                    {

                        if(
                            (_ai_all_own_stacks[itr].unit_status != us_BuildRoad)
                            &&
                            (_UNITS[itr_units].Status == us_GOTO)
                        )
                        {

                            _ai_all_own_stacks[itr].unit_status = us_GOTO;

                        }

                    }

                    seen_unit = ST_TRUE;

                }

            }

            /* Units Loop; Own Units; New Stack; loc_D35EA; light-purple (not grey-purple) */
            if(seen_unit == ST_FALSE)
            {

                _ai_all_own_stacks[_ai_all_own_stack_count].wx = _UNITS[itr_units].wx;
                _ai_all_own_stacks[_ai_all_own_stack_count].wy = _UNITS[itr_units].wy;
                _ai_all_own_stacks[_ai_all_own_stack_count].wp = _UNITS[itr_units].wp;
                _ai_all_own_stacks[_ai_all_own_stack_count].abilities = AICAP_LandOnly;
                _ai_all_own_stacks[_ai_all_own_stack_count].value = 0;
                _ai_all_own_stacks[_ai_all_own_stack_count].transport_capacity = 0;
                _ai_all_own_stacks[_ai_all_own_stack_count].unit_count = 1;
                _ai_all_own_stacks[_ai_all_own_stack_count].landmass_idx = 0;
                _ai_all_own_stacks[_ai_all_own_stack_count].landmass_idx = _landmasses[((_UNITS[itr_units].wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)];

                if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
                {

                    _ai_all_own_stacks[_ai_all_own_stack_count].abilities |= AICAP_Transport;

                    _ai_all_own_stacks[_ai_all_own_stack_count].transport_capacity += _unit_type_table[_UNITS[itr_units].type].Transport;

                    effective_unit_strength = 0;

                }
                else
                {

                    if(
                        (unit_has_watertravel == ST_FALSE)
                        &&
                        (unit_has_airtravel == ST_FALSE)
                    )
                    {

                        _ai_all_own_stacks[_ai_all_own_stack_count].transport_capacity -= 1;

                    }

                }

                if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
                {

                    _ai_all_own_stacks[_ai_all_own_stack_count].abilities |= AICAP_RoadBuild;

                    effective_unit_strength = 0;

                }

                if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_PURIFY) != 0)
                {

                    _ai_all_own_stacks[_ai_all_own_stack_count].abilities |= AICAP_Purify;

                }

                if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_CREATEOUTPOST) != 0)
                {

                    _ai_all_own_stacks[_ai_all_own_stack_count].abilities |= AICAP_Settler;

                    effective_unit_strength = 0;

                }

                if(
                    (unit_has_airtravel == ST_FALSE)
                    &&
                    (unit_has_watertravel == ST_FALSE)
                )
                {

                    _ai_all_own_stacks[_ai_all_own_stack_count].abilities ^= AICAP_LandOnly;

                }

                if(unit_has_windwalking == ST_TRUE)
                {

                    _ai_all_own_stacks[_ai_all_own_stack_count].abilities |= AICAP_WindWalk;

                    _ai_all_own_stacks[_ai_all_own_stack_count].transport_capacity = 8;

                }

                if(unit_is_hero == ST_TRUE)
                {

                    _ai_all_own_stacks[_ai_all_own_stack_count].abilities |= AICAP_Hero;

                    /* ¿ OGBUG  does not do effective_unit_strength += 50% ? */

                }

                if(unit_has_melding == ST_TRUE)
                {

                    _ai_all_own_stacks[_ai_all_own_stack_count].abilities |= AICAP_Melding;

                }

                _ai_all_own_stacks[_ai_all_own_stack_count].value += effective_unit_strength;

                _ai_all_own_stacks[_ai_all_own_stack_count].unit_status = _UNITS[itr_units].Status;

                if(_UNITS[itr_units].Status == us_BuildRoad)
                {

                    _ai_all_own_stacks[_ai_all_own_stack_count].unit_status = us_BuildRoad;

                }
                else
                {

                    if(
                        (_ai_all_own_stacks[_ai_all_own_stack_count].unit_status != us_BuildRoad)
                        &&
                        (_UNITS[itr_units].Status == us_GOTO)
                    )
                    {

                        _ai_all_own_stacks[_ai_all_own_stack_count].unit_status = us_GOTO;

                    }

                }

                if(player_idx != NEUTRAL_PLAYER_IDX)
                {

                    _ai_all_own_stack_count++;

                }
                else
                {

                    seen_unit = ST_FALSE;

                    if((_ai_all_own_stacks[_ai_all_own_stack_count].abilities & AICAP_Settler) == 0)
                    {
                        seen_unit = ST_TRUE;
                    }

                    for(City_Loop_Var = 0; ((City_Loop_Var < _cities) && (seen_unit == ST_FALSE)); City_Loop_Var++)
                    {

                        if(
                        (_CITIES[City_Loop_Var].wx == _ai_all_own_stacks[_ai_all_own_stack_count].wx)
                        &&
                        (_CITIES[City_Loop_Var].wy == _ai_all_own_stacks[_ai_all_own_stack_count].wy)
                        &&
                        (_CITIES[City_Loop_Var].wp == _ai_all_own_stacks[_ai_all_own_stack_count].wp)
                        )
                        {
                            seen_unit = ST_TRUE;
                        }

                    }

                    if(seen_unit == ST_FALSE)
                    {
                        _ai_all_own_stack_count++;
                    }

                }

            }  /* if(seen_unit == ST_FALSE) */

            for(itr = 0; itr < _cities; itr++)
            {
                if(
                    (_UNITS[itr_units].wx == _CITIES[itr].wx)
                    &&
                    (_UNITS[itr_units].wy == _CITIES[itr].wy)
                    &&
                    (_UNITS[itr_units].wp == _CITIES[itr].wp)
                )
                {
                    _ai_all_own_garrison_strengths[itr] += effective_unit_strength;
                }

            }

        }

    }


    /* OGBUG  doesn't set city_owner_idx */
    /* OGBUG  _ai_all_enemy_garrison_strengths[] is per city not per stack */
    /* OGBUG  should check high-hostility `>= 2` */
    for(itr_units = 0; itr_units < _ai_all_enemy_stack_count; itr_units++)
    {

        if(_players[player_idx].Hostility[city_owner_idx] == 2)
        {

            _ai_all_enemy_garrison_strengths[itr_units] *= 2;

        }

    }


    /* OGBUG  mixed up iterators; c&p error? */
    /* OGBUG  should be NOT instead of OR; c&p error? */
    for(itr_units = 0; itr_units < _ai_all_own_stack_count; itr_units++)
    {
        if((_ai_all_own_stacks[itr_units].abilities & AICAP_WindWalk) != 0)
        {
            _ai_all_own_stacks[itr].abilities |= AICAP_LandOnly;
        }
    }


    /* Eh? Might as well count up the transport units while we're here. */
    ai_transport_count = 0;
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].owner_idx == player_idx)
            &&
            (_unit_type_table[_UNITS[itr_units].type].Transport > 0)
        )
        {
            ai_transport_count++;
        }
    }

}


// WZD o145p08
/*
uses _players[].Hostility[], which was just reevaluated in AI_Evaluate_Hostility()
uses _players[].peace_duration[], to avoid timiing mishap
...later, used to iterate through and weigh target selection
*/
/**
 * @brief Rebuilds the current AI player's list of hostile opponent indexes.
 *
 * This helper clears the shared hostile-opponent count and scans every player
 * slot, selecting only other wizards whose hostility rating toward the caller
 * is at least annoyed/warlike and whose peace timer has already expired. Each
 * qualifying player index is appended to the global
 * @c _cp_hostile_opponents array for later target-selection logic.
 *
 * @param player_idx Index of the AI wizard whose hostile-opponent list should
 *                   be refreshed.
 *
 * @return This function does not return a value. It updates the global
 *         @c _cp_hostile_opponent_count and populates
 *         @c _cp_hostile_opponents with matching player indexes.
 *
 * @note The caller itself is always excluded, even if its hostility table
 *       would otherwise satisfy the filter.
 * @note Players still covered by a non-zero peace duration are intentionally
 *       omitted to avoid transient post-treaty targeting.
 */
void Player_Hostile_Opponents(int16_t player_idx)
{
    int16_t itr_players = 0;

    _cp_hostile_opponent_count = 0;

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(
            (_players[player_idx].Hostility[itr_players] >= 2)  /* high-hostility */
            &&
            (_players[player_idx].peace_duration[itr_players] == 0)
            &&
            (player_idx != itr_players)
        )
        {

            _cp_hostile_opponents[_cp_hostile_opponent_count] = itr_players;

            _cp_hostile_opponent_count++;

        }

    }

}


// WZD o145p09
// drake178: sub_D405F()

// WZD o145p10
// drake178: TILE_CountUnits()
int16_t Map_Square_Unit_Count(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t unit_count;  // _SI_
    int16_t itr_units;  // _DI_

    unit_count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].wx == wx)
            &&
            (_UNITS[itr_units].wy == wy)
            &&
            (_UNITS[itr_units].wp == wp)
        )
        {
            unit_count += 1;
        }
    }

    return unit_count;
}

// WZD o145p11
// drake178: AI_Alchemy()
/*
converts the player's gold into mana or vice versa
based on a set of criteria

BUG: when banished, converted gold replaces existing
mana instead of being added to it
*/
/*

rearranges gold and mana

*/
void AI_Update_Gold_And_Mana_Reserves(int16_t player_idx)
{
    int16_t Desired_Mana = 0;
    int16_t Desired_Gold = 0;
    int16_t Resource_Total = 0;

    if(_players[player_idx].casting_spell_idx == spl_Spell_Of_Return)
    {

        // ; convert all gold into mana
        // ; 
        // ; BUG: assumes no mana to begin with, overwriting it
        // ; instead of adding to it

        if(_players[player_idx].alchemy == 1)
        {

            _players[player_idx].mana_reserve = _players[player_idx].gold_reserve;  // ; BUG: assumes that the wizard had no mana originally

        }
        else
        {
            
            _players[player_idx].mana_reserve = (_players[player_idx].gold_reserve / 2);  // ; BUG: assumes that the wizard had no mana originally

        }

        _players[player_idx].gold_reserve = 0;

    }
    else
    {

        if(_players[player_idx].alchemy != 1)
        {

            if(_players[player_idx].mana_reserve == 0)
            {

                _players[player_idx].mana_reserve = (_players[player_idx].gold_reserve / 4);

                _players[player_idx].gold_reserve = (_players[player_idx].gold_reserve / 2);

            }

            if(Random(10) == 1)
            {
    
                if(_players[player_idx].mana_reserve >= (_players[player_idx].gold_reserve / 5))
                {

                    if(_players[player_idx].gold_reserve >= (_players[player_idx].mana_reserve / 4))
                    {

                        if(
                            (_turn < 100)
                            &&
                            _players[player_idx].mana_reserve > 50
                        )
                        {

                            Desired_Gold = _players[player_idx].gold_reserve;

                            Desired_Mana = _players[player_idx].mana_reserve;

                            Desired_Gold = (Desired_Mana / 4);

                            Desired_Mana = (Desired_Mana - (Desired_Mana / 2));

                            SETMAX(Desired_Mana, MAX_MANA_RESERVE);

                            SETMAX(Desired_Gold, MAX_GOLD_RESERVE);

                            _players[player_idx].gold_reserve = Desired_Gold;

                            _players[player_idx].mana_reserve = Desired_Mana;

                        }

                    }

                }
                else
                {

                    if(_players[player_idx].gold_reserve > 100)
                    {

                        Desired_Gold = _players[player_idx].gold_reserve;

                        Desired_Mana = _players[player_idx].mana_reserve;

                        Desired_Mana = (Desired_Gold / 4);

                        Desired_Gold = (Desired_Gold - (Desired_Gold / 2));

                        SETMAX(Desired_Mana, MAX_MANA_RESERVE);

                        SETMAX(Desired_Gold, MAX_GOLD_RESERVE);

                        _players[player_idx].gold_reserve = Desired_Gold;

                        _players[player_idx].mana_reserve = Desired_Mana;

                    }

                }

            }

        }
        else
        {

            // want more gold or more mana
            if(_turn >= 150)
            {

                Resource_Total = (_players[player_idx].gold_reserve + _players[player_idx].mana_reserve);

                Desired_Mana = (Resource_Total * 7 / 10);  // mana = total * .7

                SETMAX(Desired_Mana, MAX_MANA_RESERVE);

                Desired_Gold = (Resource_Total - Desired_Mana);

                SETMAX(Desired_Gold, MAX_GOLD_RESERVE);

            }
            else
            {

                Resource_Total = (_players[player_idx].gold_reserve + _players[player_idx].mana_reserve);

                Desired_Gold = (Resource_Total * 9 / 10);  // gold = total * .9

                SETMAX(Desired_Gold, MAX_GOLD_RESERVE);

                Desired_Mana = (Resource_Total - Desired_Gold);


                SETMAX(Desired_Mana, MAX_MANA_RESERVE);

            }

            _players[player_idx].gold_reserve = Desired_Gold;

            _players[player_idx].mana_reserve = Desired_Mana;

        }

    }

}


// WZD o145p12
// drake178: AI_Tax_And_Farmers()
/*
sets the tax rate for the player and, if food
production is below 0, makes 50 attempts at
converting a random worker into a farmer

BUG: treats all farmers as generating 2 food
*/
/*

updates tax_rate (∴ gold_income)
updates food_income

*/
void AI_Update_Gold_Income_And_Food_Income(int16_t player_idx)
{
    int16_t food_income = 0;
    int16_t mana_income = 0;
    int16_t gold_income = 0;
    int16_t Tries = 0;
    int16_t UU_Local_0_3 = 0;
    int16_t UU_Local_0_2 = 0;
    int16_t UU_Local_0_1 = 0;
    int16_t city_idx = 0;  // _DI_

    UU_Local_0_1 = 0;
    UU_Local_0_3 = 0;
    UU_Local_0_2 = 0;

    Player_Resource_Income_Total(player_idx, &gold_income, &food_income, &mana_income);

    if(
        (((_turn / 50) * 75) > gold_income)
        &&
        (_players[player_idx].tax_rate < 6)
    )
    {

        _players[player_idx].tax_rate += 1;

    }

    Player_Resource_Income_Total(player_idx, &gold_income, &food_income, &mana_income);

    if(
        (gold_income > 200)
        &&
        (_players[player_idx].tax_rate > 1)
    )
    {

        _players[player_idx].tax_rate -= 1;

    }


    if(_players[player_idx].tax_rate > 4)
    {

        _players[player_idx].tax_rate = 4;

    }


    Tries = 0;

    food_income = -(food_income);

    while(((Tries < 50) && (food_income > 0)))
    {

        city_idx = (Random(_cities) - 1);

        if(_CITIES[city_idx].owner_idx == player_idx)
        {

            if(_CITIES[city_idx].farmer_count < _CITIES[city_idx].population)
            {

                CITIES_FARMER_COUNT(city_idx, (_CITIES[city_idx].farmer_count + 1));

                // ; BUG: not all farmers generate 2 food
                food_income -= 2;

            }

        }

        Tries++;

    }

}


// WZD o145p13
/**
 * @brief Recomputes farmer allocation for all neutral-owned cities.
 *
 * Iterates over every city owned by the neutral player and sets its
 * farmer_count high enough to satisfy the city's minimum food requirement plus
 * the upkeep burden from units currently standing on that city's square.
 *
 * The routine applies a special divisor of 3 food per farmer for Halfling
 * cities and cities with a built granary, and otherwise uses the standard
 * divisor of 2. Each result is rounded up so partial farmer demand becomes a
 * full assigned farmer, then clamped so the final farmer count never exceeds
 * the city's total population.
 *
 * @note This function preserves the original game behavior noted in the local
 *       comments, including the Halfling/granary overland food calculation.
 * @note Reads global city and unit state from _CITIES, _cities, and the unit
 *       list queried via Map_Square_Unit_Count(), and writes updated
 *       farmer_count values back into neutral cities.
 */
/* GEMINI */
void NPC_Farmers(void)
{
    int16_t city_idx = 0;
    int16_t min_farmers = 0;
    int16_t unit_count = 0;
    int16_t temp_val = 0;
    for (city_idx = 0; city_idx < _cities; city_idx++)
    {
        if (_CITIES[city_idx].owner_idx == NEUTRAL_PLAYER_IDX)
        {
            /* Calculate base farmers needed for population */
            min_farmers = City_Minimum_Farmers(city_idx);
            /* Count units currently in the city square to calculate food maintenance */
            unit_count = Map_Square_Unit_Count(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp);
            /* Check for production bonus (Halflings or Granary). */
            /* OGBUG:  wrong calculation */
            if (_CITIES[city_idx].race == rt_Halfling || _CITIES[city_idx].bldg_status[GRANARY] == bs_Built)
            {
                /* Halfling/Granary logic (3 food per farmer) */
                temp_val = min_farmers + unit_count;
                _CITIES[city_idx].farmer_count = (temp_val / 3);
                /* Round up if there is a remainder */
                if ((temp_val % 3) != 0)
                {
                    _CITIES[city_idx].farmer_count++;
                }
            }
            else
            {
                /* Standard logic (2 food per farmer) */
                temp_val = min_farmers + unit_count;
                _CITIES[city_idx].farmer_count = (temp_val / 2);
                /* Round up if there is a remainder */
                if ((temp_val % 2) != 0)
                {
                    _CITIES[city_idx].farmer_count++;
                }
            }
            /* Ensure farmer count does not exceed total population */
            if (_CITIES[city_idx].farmer_count > _CITIES[city_idx].population)
            {
                _CITIES[city_idx].farmer_count = _CITIES[city_idx].population;
            }
        }
    }
}


// WZD o145p14
// drake178: AI_ExcessBuilders()
/*
ensure that there is at most one settler and, after
turn 200, at most one engineer on any continent
*/
/*

inherently keeps the oldest
kills off 2+ settlers, per continent
after turn 200, kills off 2+ engineers, per continent

*/
void AI_Kill_Excess_Settlers_And_Engineers(int16_t player_idx)
{
    int16_t engineer_count[NUM_LANDMASSES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t settler_count[NUM_LANDMASSES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t landmass_idx = 0;
    int16_t itr = 0;  // _DI_

    for(itr = 0; itr <NUM_LANDMASSES; itr++)
    {

        settler_count[itr] = 0;

        engineer_count[itr] = 0;

    }

    for(itr = 0; itr < _units; itr++)
    {

        if(_UNITS[itr].owner_idx == player_idx)
        {

            if((_unit_type_table[_UNITS[itr].type].Abilities & UA_CREATEUNDEAD) != 0)
            {

                landmass_idx = _landmasses[((_UNITS[itr].wp * WORLD_SIZE) + (_UNITS[itr].wy * WORLD_WIDTH) + _UNITS[itr].wx)];

                if(
                    (landmass_idx != 0)
                    &&
                    (landmass_idx < NUM_LANDMASSES)
                )
                {

                    settler_count[landmass_idx] += 1;

                    if(settler_count[landmass_idx] > 1)
                    {

                        Kill_Unit(itr, 0);

                    }

                }

            }

            if(
                (_unit_type_table[_UNITS[itr].type].Construction > 0)
                &&
                (_turn > 200)
            )
            {

                landmass_idx = _landmasses[((_UNITS[itr].wp * WORLD_SIZE) + (_UNITS[itr].wy * WORLD_WIDTH) + _UNITS[itr].wx)];

                if(
                    (landmass_idx != 0)
                    &&
                    (landmass_idx < NUM_LANDMASSES)
                )
                {

                    engineer_count[landmass_idx] += 1;

                    if(engineer_count[landmass_idx] > 1)
                    {

                        Kill_Unit(itr, 0);

                    }

                }

            }

        }

    }

}


// WZD o145p15
void AI_Hopeless_Stasis(void)
{
    int16_t unit_idx = 0;
    for (unit_idx = 0; unit_idx < _units; unit_idx++)
    {
        if (_UNITS[unit_idx].owner_idx == HUMAN_PLAYER_IDX)
        {
            continue;
        }
        if (!(_UNITS[unit_idx].mutations & (C_STASISINIT | C_STASISLINGER)))
        {
            continue;
        }
        if (_unit_type_table[_UNITS[unit_idx].type].Resist < 7)
        {
            Kill_Unit(unit_idx, kt_Normal);
        }
    }
}


// WZD o145p16
// drake178: EMM_Map_CONTXXX__WIP()
/*
maps in the EMM_ContXXX_H handle (all 4 pages), and
resets its corresponding global pointers
*/
/*
EmmHndl_CONTXXX is allocated in ALLOC.C Allocate_Data_Space()

¿ kinda like ?
m_terrain_lbx_000
LOADER.C
Terrain_Init()
// ehn_terrain_lbx = EMM_Load_LBX_File("TERRAIN.LBX", 1);
// HACK:  no EMM, so just load entry and monkey with offset adjustments
// TODO  make this an allocate and reload and set another pointer to the offset, so it looks like it did and dont need the math elsewhere - e.g., Draw_Map_Terrain()
m_terrain_lbx_000 = LBX_Load(terrain_lbx_file, 0);  // 676416 B

*/
void EMM_Map_CONTXXX__WIP(void)
{
    int16_t itr = 0;  // _SI_

    // TODO  EMM_Map4Pages(0, EmmHndl_CONTXXX);
    // ...
    // TODO  CONTX_Arc_TileXs = SA_MK_FP0(EMS_PFBA);
    // ...
    // ...
    // ...

    // EMS_PFBA = Allocate_Space(((4 * 16384) / 16));
    EMS_PFBA = EmmHndl_CONTXXX;

    _ai_landmass_land_squares_wx_array[0]           =  (int8_t *)(&EMS_PFBA[0] + (   0 * SZ_PARAGRAPH_B));  //  100 -    0 = 100 * 1 PR = 1600 B
    _ai_landmass_land_squares_wx_array[1]           =  (int8_t *)(&EMS_PFBA[0] + ( 100 * SZ_PARAGRAPH_B));  //  200 -  100 = 100 * 1 PR = 1600 B
    _ai_landmass_land_squares_wy_array[0]           =  (int8_t *)(&EMS_PFBA[0] + ( 200 * SZ_PARAGRAPH_B));  //  300 -  200 = 100 * 1 PR = 1600 B
    _ai_landmass_land_squares_wy_array[1]           =  (int8_t *)(&EMS_PFBA[0] + ( 300 * SZ_PARAGRAPH_B));  //  400 -  300 = 100 * 1 PR = 1600 B
    _ai_landmass_land_squares_lists[0]        = (int16_t *)(&EMS_PFBA[0] + ( 400 * SZ_PARAGRAPH_B));  //  600 -  400 = 200 * 1 PR = 3200 B
    _ai_landmass_land_squares_lists[1]        = (int16_t *)(&EMS_PFBA[0] + ( 600 * SZ_PARAGRAPH_B));  //  800 -  600 = 200 * 1 PR = 3200 B
    _ai_landmass_land_squares_heads[0]       = (int16_t *)(&EMS_PFBA[0] + ( 800 * SZ_PARAGRAPH_B));  //  825 -  800 =  25 * 1 PR =  400 B
    _ai_landmass_land_squares_heads[1]       = (int16_t *)(&EMS_PFBA[0] + ( 825 * SZ_PARAGRAPH_B));  //  850 -  825 =  25 * 1 PR =  400 B
    _ai_own_stack_wx          = (&EMS_PFBA[0] + ( 850 * SZ_PARAGRAPH_B));  //  860 -  850 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_wy          = (&EMS_PFBA[0] + ( 860 * SZ_PARAGRAPH_B));  //  870 -  860 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_wp          = (&EMS_PFBA[0] + ( 870 * SZ_PARAGRAPH_B));  //  880 -  870 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_type        = (&EMS_PFBA[0] + ( 880 * SZ_PARAGRAPH_B));  //  890 -  880 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_unit_count  = (&EMS_PFBA[0] + ( 890 * SZ_PARAGRAPH_B));  //  900 -  890 =  10 * 1 PR =  160 B  80 2-byte values
    g_ai_evaluation_map[0]        = (uint16_t *)(&EMS_PFBA[0] + ( 900 * SZ_PARAGRAPH_B));  // 1500 -  900 = 600 * 1 PR = 9600 B
    g_ai_evaluation_map[1]        = (uint16_t *)(&EMS_PFBA[0] + (1500 * SZ_PARAGRAPH_B));  // 2100 - 1500 = 600 * 1 PR = 9600 B
    _ai_landmass_dock_squares_wx_array[0]       =  (int8_t *)(&EMS_PFBA[0] + (2100 * SZ_PARAGRAPH_B));  // 2200 - 2100 = 100 * 1 PR = 1600 B
    _ai_landmass_dock_squares_wx_array[1]       =  (int8_t *)(&EMS_PFBA[0] + (2200 * SZ_PARAGRAPH_B));  // 2300 - 2200 = 100 * 1 PR = 1600 B
    _ai_landmass_dock_squares_wy_array[0]       =  (int8_t *)(&EMS_PFBA[0] + (2300 * SZ_PARAGRAPH_B));  // 2400 - 2300 = 100 * 1 PR = 1600 B
    _ai_landmass_dock_squares_wy_array[1]       =  (int8_t *)(&EMS_PFBA[0] + (2400 * SZ_PARAGRAPH_B));  // 2500 - 2400 = 100 * 1 PR = 1600 B
    _ai_landmass_dock_squares_lists[0]       = (int16_t *)(&EMS_PFBA[0] + (2500 * SZ_PARAGRAPH_B));  // 2700 - 2500 = 200 * 1 PR = 3200 B
    _ai_landmass_dock_squares_lists[1]       = (int16_t *)(&EMS_PFBA[0] + (2700 * SZ_PARAGRAPH_B));  // 2900 - 2700 = 200 * 1 PR = 3200 B
    _ai_landmass_dock_squares_heads[0]        = (int16_t *)(&EMS_PFBA[0] + (2900 * SZ_PARAGRAPH_B));  // 3000 - 2900 = 100 * 1 PR = 1600 B
    _ai_landmass_dock_squares_heads[1]        = (int16_t *)(&EMS_PFBA[0] + (3000 * SZ_PARAGRAPH_B));  // 3200 - 3000 = 200 * 1 PR = 3200 B

    /* HACK */  memset(g_ai_evaluation_map[0], 0, 9600);
    /* HACK */  memset(g_ai_evaluation_map[1], 0, 9600);

// mov     ax, _SI_itr
// shl     ax, 1
// mov     dx, [EMS_PFBA]             ; contains the segment address of the EMS page frame
// add     dx, ax
// add     dx, 3200
// push    dx                              ; addr
// call    SA_MK_FP0
// pop     cx
// mov     bx, _SI_itr
// mov     cl, 2
// shl     bx, cl
// mov     [word ptr (_ai_own_stack_unit_list+2)+bx], dx ; 2 segment increments each (32 bytes data space)
//                                         ; each holds a list of unit indices or -1s for units
//                                         ; that have already been assigned orders
// mov     [word ptr _ai_own_stack_unit_list+bx], ax ; 2 segment increments each (32 bytes data space)
//                                         ; each holds a list of unit indices or -1s for units
//                                         ; that have already been assigned orders
// inc     _SI_itr

    for(itr = 0; itr < 80; itr++)
    {

        // ¿ get a pointer at every 32 bytes, starting at 51200 bytes from the EMM base ?
        _ai_own_stack_unit_list[itr] = (int16_t *)(&EMS_PFBA[0] + (3200 * SZ_PARAGRAPH_B) + ((itr * 2) * SZ_PARAGRAPH_B));
        
    }

}

// WZD o145p17
/**
 * @brief Rebuilds the per-plane linked lists of land tiles for every landmass.
 *
 * This routine maps the temporary CONTXXX working area, clears the head and
 * next-index arrays for both planes, and then scans every world square. Each
 * non-ocean square is appended to the array-backed linked list for its landmass,
 * with the square's world coordinates recorded in the parallel x/y arrays and
 * the corresponding head entry initialized on first insertion.
 *
 * The resulting structure gives each nonzero landmass ID a forward chain of
 * all of its land squares on Arcanus and Myrror, which later AI routines can
 * traverse without rescanning the full map.
 *
 * @return This function does not return a value. It repopulates
 *         @c _ai_landmass_land_squares_heads,
 *         @c _ai_landmass_land_squares_lists,
 *         @c _ai_landmass_land_squares_wx_array, and
 *         @c _ai_landmass_land_squares_wy_array.
 *
 * @note Landmass index 0 is treated as ocean and is intentionally excluded
 *       from the generated lists.
 * @note The routine temporarily remaps CONTXXX via
 *       @c EMM_Map_CONTXXX__WIP() and restores the normal data mapping with
 *       @c EMMDATAH_Map() before returning.
 */
void Build_Land_Linked_List(void)
{
    int16_t previous_node_idx = 0;
    int16_t landmass_idx = 0;
    int16_t count[NUM_PLANES] = { 0, 0 };
    int16_t itr = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t wp = 0;
    int16_t next_node_idx = 0;  // DNE in Dasm, reuses itr

    EMM_Map_CONTXXX__WIP();

    for(itr = 0; itr < NUM_LANDMASSES; itr++)
    {
        _ai_landmass_land_squares_heads[ARCANUS_PLANE][itr] = ST_UNDEFINED;
        _ai_landmass_land_squares_heads[MYRROR_PLANE][itr] = ST_UNDEFINED;
    }

    /* DEDU  WTF w/ 1600? */
    for(itr = 0; itr < 1600; itr++)
    {
        _ai_landmass_land_squares_lists[ARCANUS_PLANE][itr] = ST_UNDEFINED;
        _ai_landmass_land_squares_lists[MYRROR_PLANE][itr] = ST_UNDEFINED;
    }

    count[ARCANUS_PLANE] = 0;
    count[MYRROR_PLANE] = 0;

    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                landmass_idx = _landmasses[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];
                assert(landmass_idx < NUM_LANDMASSES);
                if(landmass_idx == 0)
                {
                    continue;
                }
                if(_ai_landmass_land_squares_heads[wp][landmass_idx] == ST_UNDEFINED)
                {
                    _ai_landmass_land_squares_heads[wp][landmass_idx] = count[wp];
                }
                else
                {
                    next_node_idx = _ai_landmass_land_squares_heads[wp][landmass_idx];
                    previous_node_idx = next_node_idx;
                    while(next_node_idx != ST_UNDEFINED)
                    {
                        previous_node_idx = next_node_idx;
                        next_node_idx = _ai_landmass_land_squares_lists[wp][next_node_idx];
                    }
                    _ai_landmass_land_squares_lists[wp][previous_node_idx] = count[wp];
                }
                _ai_landmass_land_squares_wx_array[wp][count[wp]] = (int8_t)wx;
                _ai_landmass_land_squares_wy_array[wp][count[wp]] = (int8_t)wy;
                _ai_landmass_land_squares_lists[wp][count[wp]] = ST_UNDEFINED;
                count[wp] += 1;
            }
        }
    }

    EMMDATAH_Map();

}


// WZD o145p18
/**
 * @brief Precomputes each landmass's candidate embark/disembark edge.
 *
 * The AI repeatedly needs a small set of coastal squares for a continent when
 * it is deciding where transports should leave land, where they should land on
 * a target continent, and where the "edge" of a landmass is when comparing
 * continents over water. Rather than rescanning the full map every time, this
 * routine builds one linked list per (plane, landmass) containing the land
 * squares that sit on that landmass's shoreline band.
 *
 * A square qualifies if it belongs to a real landmass and some square in its 3x3
 * neighborhood is shoreline terrain. That deliberately produces a slightly
 * thicker coastal fringe than a strict "adjacent cardinal edge only" test,
 * which matches how later AI movement code treats usable embark/disembark
 * approach areas: consumers want a pool of plausible coastal land squares, not
 * one exact water boundary pixel.
 *
 * The resulting arrays are therefore not just a cache of "tiles that passed a
 * predicate." They are the AI's reusable per-continent entry/exit candidates:
 * later code walks the list, computes coarse coastal centroids, and chooses a
 * reachable square near that edge instead of rescanning the entire world map.
 *
 * @return This function does not return a value. It repopulates
 *         @c _ai_landmass_dock_squares_heads,
 *         @c _ai_landmass_dock_squares_lists,
 *         @c _ai_landmass_dock_squares_wx_array, and
 *         @c _ai_landmass_dock_squares_wy_array.
 *
 * @note Landmass index 0 is the ocean sentinel, so it intentionally gets no
 *       chain; these tables are keyed by actual continents only.
 * @note The routine temporarily remaps CONTXXX via
 *       @c EMM_Map_CONTXXX__WIP() and restores the normal data mapping with
 *       @c EMMDATAH_Map() before returning.
 */
void Build_Dock_Linked_List(void)
{
    int16_t square_is_shoreline = 0;
    int16_t wy_ofst = 0;
    int16_t wx_ofst = 0;
    int16_t previous_node_idx = 0;
    int16_t landmass_idx = 0;
    int16_t count[NUM_PLANES] = { 0, 0 };  /* an array tracking the coastal square count per plane */
    int16_t itr = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t wp = 0;
    int16_t next_node_idx = 0;  // DNE in Dasm, reuses itr

    EMM_Map_CONTXXX__WIP();

    /* 2. Initialize the "Head" pointers of the Linked Lists to -1 (Empty) */
    /* default all of boths planes to invalid */
    for(itr = 0; itr < NUM_LANDMASSES; itr++)
    {
        _ai_landmass_dock_squares_heads[ARCANUS_PLANE][itr] = ST_UNDEFINED;
        _ai_landmass_dock_squares_heads[MYRROR_PLANE][itr] = ST_UNDEFINED;
    }

    /* CLAUDE: mirror Build_Land_Linked_List() — initialize the "next" pointers of
       every list slot to -1.  Without this, slots beyond the populated chains
       hold allocator zero-fill, which looks like "go to slot 0" to any reader
       that walks past a chain tail due to a bug elsewhere → infinite loop. */
    for(itr = 0; itr < 1600; itr++)
    {
        _ai_landmass_dock_squares_lists[ARCANUS_PLANE][itr] = ST_UNDEFINED;
        _ai_landmass_dock_squares_lists[MYRROR_PLANE][itr] = ST_UNDEFINED;
    }

    count[ARCANUS_PLANE] = 0;
    count[MYRROR_PLANE] = 0;

    /* 3. The Global Sweep */
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                /* Fetch the Continent ID for the current square */
                landmass_idx = _landmasses[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];
                assert(landmass_idx < NUM_LANDMASSES);
                /* If it's ocean (0), skip it */
                if(landmass_idx == 0)
                {
                    continue;
                }
/* 4. The 3x3 Shoreline Scan  (3x3 Moore neighborhood) */
                square_is_shoreline = ST_FALSE;
                /* OGBUG  no bounds checks */
                for(wx_ofst = -1; ((wx_ofst < 2) && (square_is_shoreline == ST_FALSE)); wx_ofst++)
                {
                    for(wy_ofst = -1; ((wy_ofst < 2) && (square_is_shoreline == ST_FALSE)); wy_ofst++)
                    {
/* Does this land square touch the ocean? */
                        if(Square_Is_Shoreline((wx + wx_ofst), (wy + wy_ofst), wp) == ST_TRUE)
                        {
                            square_is_shoreline = ST_TRUE;
                        }
                    }
                }
/* 5. The Linked List Insertion */
                if(square_is_shoreline == ST_TRUE)
                {
/* If this continent doesn't have a head node yet, this square becomes the head */
/* First square in the chain for this landmass */
                    if(_ai_landmass_dock_squares_heads[wp][landmass_idx] == ST_UNDEFINED)
                    {
                        _ai_landmass_dock_squares_heads[wp][landmass_idx] = count[wp];
                    }
                    else
                    {
/* Otherwise, traverse the linked list to find the tail */
/*
What you are looking at is the classic "Trailing Pointer"
(or "Previous Node") pattern for traversing an array-based linked list.
1. The Problem with Array-Based Linked Lists
In standard C, a linked list uses pointers (Node *next). But in this engine, the "pointers" are just integer indices pointing to a slot in the CONTX_LoadTChain array.
To add a new coastal square to the end of the chain, the engine has to walk from the Head node all the way down to the Tail node. The Tail node is the one whose next pointer is e_ST_UNDEFINED (-1).
If you just write a loop that says next_node_idx = next[next_node_idx] until next_node_idx == -1, you fall off the edge of the list. By the time the loop finishes, next_node_idx is -1, and you have completely forgotten the index of the actual Tail node, meaning you have nowhere to attach the new square!
2. The "Trailing Pointer" Solution
To prevent falling off the edge, the assembly uses previous_node_idx to follow exactly one step behind next_node_idx.

*/
/* Start at the head of the chain */
                        next_node_idx = _ai_landmass_dock_squares_heads[wp][landmass_idx];
/* Defensive initialization */
                        previous_node_idx = next_node_idx;
/* Walk down the chain until 'next_node_idx' falls off the edge */
                        while(next_node_idx != ST_UNDEFINED)
                        {
/* Park the trailing pointer on the current valid node */
                            previous_node_idx = next_node_idx;
/* Step 'next_node_idx' forward to the next node */
                            next_node_idx = _ai_landmass_dock_squares_lists[wp][next_node_idx];
                        }
/* Append the new coastal square to the tail */
/* Link the previous square to this new square */
                        _ai_landmass_dock_squares_lists[wp][previous_node_idx] = count[wp];
                    }
/* 6. Save the actual World Coordinates in the data arrays */
                    _ai_landmass_dock_squares_wx_array[wp][count[wp]] = (int8_t)wx;
                    _ai_landmass_dock_squares_wy_array[wp][count[wp]] = (int8_t)wy;
/* Terminate the new tail of the list */
                    _ai_landmass_dock_squares_lists[wp][count[wp]] = ST_UNDEFINED;
/* Increment the coastal tile counter for this plane */
                    count[wp] += 1;
                }
            }
        }
    }
/* Safely remap the standard Data Handle before exiting */
    EMMDATAH_Map();  /* ¿ set EMM back to the default handle - EMMDATAH ? */

}
