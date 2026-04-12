/*
    AI - Data

    WIZARDS.EXE
        ¿ ovr145 ?
        ¿ ovr164 ?
*/


#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/special.h"
#include "../../MoX/src/EMS/EMS.h"

#include "AIDUDES.h"
#include "AIMOVE.h"
#include "CITYCALC.h"
#include "DIPLODEF.h"
#include "NEXTTURN.h"
#include "Spellbook.h"
#include "UnitMove.h"

#include "AIDATA.h"

#include <stdlib.h>



/*
    WIZARDS.EXE  ovr145
*/

// WZD o145p0





/*
    WIZARDS.EXE  ovr164
*/

// WZD o164p01
// drake178: AI_ExcessNeutrals()
// AI_ExcessNeutrals()

// WZD o164p02
// drake178: EVNT_RampageMonsters()
// EVNT_RampageMonsters()

// WZD o164p03
// drake178: EVNT_GenerateRaiders()
// EVNT_GenerateRaiders()

// WZD o164p04
// MoO2  Module: AIMOVE  Move_NPCs_(); NPC_Dest_();
int16_t NPC_Destinations(void)
{
    int16_t troops[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t dst_wy = 0;
    int16_t dst_wx = 0;
    int16_t troop_count = 0;
    int16_t cur_target_val = 0;
    int16_t target_city_idx = 0;
    int16_t stack_idx = 0;
    int16_t uu_city_idx = 0;
    int16_t adj_city_val = 0;
    int16_t adj_city_idx = 0;
    int16_t cities_examined = 0;
    int16_t stack_wp = 0;
    int16_t stack_landmass_idx = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t itr = 0;
    struct s_AI_STACK_DATA * stack = NULL;

    EMM_Map_DataH();

    Build_NPC_Stacks();  // OGBUG  definitely passes NEUTRAL_PLAYER_IDX here, but Build_NPC_Stacks doesn't take a parameter and is hard-coded for NEUTRAL_PLAYER_IDX

    for (stack_idx = 0; stack_idx < AI_Own_Stack_Count; stack_idx++) {
        stack = &AI_Own_Stacks[stack_idx];
        stack_wx = stack->wx;
        stack_wy = stack->wy;
        stack_wp = stack->wp;

        stack_landmass_idx = _landmasses[(stack_wp * WORLD_SIZE) + (stack_wy * WORLD_WIDTH) + stack_wx];

        /* Check for any adjacent non-neutral city */
        adj_city_idx = -1;
        for (itr = 0; itr < _cities; itr++) {
            if (_CITIES[itr].owner_idx != NEUTRAL_PLAYER_IDX && _CITIES[itr].wp == stack_wp) {
                if (abs(stack_wx - _CITIES[itr].wx) < 2 && abs(stack_wy - _CITIES[itr].wy) < 2) {
                    adj_city_idx = itr;
                    break;
                }
            }
        }

        /* If adjacent city found and stack is already moving (GOTO), evaluate redirection */
        if (adj_city_idx > -1 && stack->unit_status == us_GOTO) {
            Army_At_Square_1(stack_wx, stack_wy, stack_wp, &troop_count, troops);
            
            /* Current destination coordinates from the first unit in the stack */
            dst_wx = _UNITS[troops[0]].dst_wx;
            dst_wy = _UNITS[troops[0]].dst_wy;

            /* Identify if current destination is a city */
            target_city_idx = -1;
            for (itr = 0; itr < _cities; itr++) {
                if (_CITIES[itr].wx == dst_wx && _CITIES[itr].wy == dst_wy && _CITIES[itr].wp == stack_wp) {
                    target_city_idx = itr;
                    break;
                }
            }

            if (target_city_idx != adj_city_idx) {
                /* Evaluate the worth of attacking the adjacent city */
                Army_At_Square_1(_CITIES[adj_city_idx].wx, _CITIES[adj_city_idx].wy, _CITIES[adj_city_idx].wp, &troop_count, troops);
                // OGBUG  ¿ Delta_XY_With_Wrap() is always 1 here ?  ... adj_city_idx
                adj_city_val = 10 - troop_count - Delta_XY_With_Wrap(_CITIES[adj_city_idx].wx, _CITIES[adj_city_idx].wy, stack_wx, stack_wy, WORLD_WIDTH);
                
                if (_difficulty > god_Normal && _CITIES[adj_city_idx].owner_idx == HUMAN_PLAYER_IDX) adj_city_val += 5;
                if (_difficulty > god_Hard   && _CITIES[adj_city_idx].owner_idx == HUMAN_PLAYER_IDX) adj_city_val += 5;

                /* Evaluate the worth of continuing to current target */
                Army_At_Square_1(dst_wx, dst_wy, stack_wp, &troop_count, troops);
                cur_target_val = 10 - troop_count - Delta_XY_With_Wrap(dst_wx, dst_wy, stack_wx, stack_wy, WORLD_WIDTH);
                
                if (target_city_idx != -1) {
                    if (_difficulty > god_Normal && _CITIES[target_city_idx].owner_idx == HUMAN_PLAYER_IDX) cur_target_val += 5;
                    if (_difficulty > god_Hard   && _CITIES[target_city_idx].owner_idx == HUMAN_PLAYER_IDX) cur_target_val += 5;
                }

                if (adj_city_val > cur_target_val) {
                    AI_Stack_Set_Destination(stack_idx, _CITIES[adj_city_idx].wx, _CITIES[adj_city_idx].wy, NEUTRAL_PLAYER_IDX);
                    stack->unit_status = us_GOTO;
                }
            }
        }

        /* If stack is idle, find the best non-neutral city to target on the same landmass/plane */
        if (stack->unit_status != us_GOTO) {
            adj_city_val = -1000;
            target_city_idx = -1;
            cities_examined = 0;

            for (itr = 0; itr < _cities; itr++) {
                if (_CITIES[itr].owner_idx == NEUTRAL_PLAYER_IDX) continue;
                if (cities_examined >= 30) break;
                if (_CITIES[itr].wp != stack_wp) continue;

                /* If unit is LandOnly, check if city is on the same landmass */
                if (!(stack->abilities & AICAP_LandOnly)) {
                    if (_landmasses[(_CITIES[itr].wp * WORLD_SIZE) + (_CITIES[itr].wy * WORLD_WIDTH) + _CITIES[itr].wx] != stack_landmass_idx) {
                        continue;
                    }
                }

                Army_At_Square_1(_CITIES[itr].wx, _CITIES[itr].wy, _CITIES[itr].wp, &troop_count, troops);
                cur_target_val = 10 - troop_count - Delta_XY_With_Wrap(_CITIES[itr].wx, _CITIES[itr].wy, stack_wx, stack_wy, WORLD_WIDTH);
                
                if (_difficulty > god_Normal && _CITIES[itr].owner_idx == HUMAN_PLAYER_IDX) cur_target_val += 5;
                if (_difficulty > god_Hard   && _CITIES[itr].owner_idx == HUMAN_PLAYER_IDX) cur_target_val += 5;

                if (cur_target_val > adj_city_val) {
                    target_city_idx = itr;
                    adj_city_val = cur_target_val;
                }
                cities_examined++;
            }

            if (cities_examined > 0) {
                uu_city_idx = target_city_idx;
                AI_Stack_Set_Destination(stack_idx, _CITIES[target_city_idx].wx, _CITIES[target_city_idx].wy, NEUTRAL_PLAYER_IDX);
                stack->unit_status = us_GOTO;
            } else {
                /* No reachable targets: disband the stack to clear conventional memory */
                /* same as seen in MoO2 ... `if(NPC_Dest_() == ST_UNDEFINED)` */
                for (itr = 0; itr < _units; itr++) {
                    if (_UNITS[itr].wx == stack_wx && _UNITS[itr].wy == stack_wy && _UNITS[itr].wp == stack_wp) {
                        Kill_Unit(itr, 0);
                    }
                }
            }
        }
    }

    return stack_idx;
}


// WZD o164p05
// ¿ MoO2  Module: AIDATA  Compute_AI_Data_() ? ¿ ... Deallocate_AI_Data_() ?
void Allocate_AI_Data(void)
{
    AI_Own_City_Values = (int16_t *)Near_Allocate_First(200);
    AI_Enemy_City_Values = (int16_t *)Near_Allocate_Next(200);
    AI_Own_Garr_Strs = (int16_t *)Near_Allocate_Next(200);
    AI_NME_Garrison_Strs = (int16_t *)Near_Allocate_Next(200);
    AI_Own_Stacks = (struct s_AI_STACK_DATA *)Near_Allocate_Next(800);
    AI_Enemy_Stacks = (struct s_AI_TARGET *)Near_Allocate_Next(1440);
    AI_OVL_Spell_Cats = Near_Allocate_Next(92);
    CRP_AI_OVL_SpellList = Near_Allocate_Next(50);
    AI_Cont_Own_Str[0] = (int16_t *)Near_Allocate_Next(120);
    AI_Cont_Own_Str[1] = (int16_t *)Near_Allocate_Next(120);
    CRP_AI_Cont_Nme_Str[0] = (int16_t *)Near_Allocate_Next(120);
    CRP_AI_Cont_Nme_Str[1] = (int16_t *)Near_Allocate_Next(120);
    AI_Cont_Own_Val[0] = (int16_t *)Near_Allocate_Next(120);
    AI_Cont_Own_Val[1] = (int16_t *)Near_Allocate_Next(120);
    AI_Cont_Nme_Val[0] = (int16_t *)Near_Allocate_Next(120);
    AI_Cont_Nme_Val[1] = (int16_t *)Near_Allocate_Next(120);
    AI_ContBalances[0] = CRP_AI_Cont_Nme_Str[0];
    AI_ContBalances[1] = CRP_AI_Cont_Nme_Str[1];
}


// WZD o164p06
// drake178: AI_Magic_Strategy()
/*
evaluates conditions and sets the magic strategy
that may influence the AI's power distribution

has one certain, and multiple possible bugs, as it
sets unused strategies, possibly overwriting others
*/
/*


*/
void AI_Magic_Strategy__WIP(int16_t player_idx)
{
    int16_t Need_Research = 0;
    int16_t city_count = 0;
    int16_t Human_Stronger = 0;
    int16_t Unused_Strategy = 0;
    int16_t Power_Strategy = 0;
    int16_t At_War = 0;
    int16_t itr_players = 0;  // _DI_
    int16_t itr_cities = 0;  // _DI_

    _players[player_idx].reevaluate_magic_strategy_countdown -= 1;

    if(_players[player_idx].reevaluate_magic_strategy_countdown > 0)
    {
        return;
    }

    _players[player_idx].reevaluate_magic_strategy_countdown = (15 + Random(10));

    At_War = ST_FALSE;

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(_players[player_idx].Hostility[itr_players] >= 3)
        {
            At_War = ST_TRUE;
        }

    }

    if(
        (Player_Armies_Gold_Upkeep(player_idx) + (Player_Armies_And_Enchantments_Mana_Upkeep(player_idx) * 2))
        <
        (Player_Armies_Gold_Upkeep(HUMAN_PLAYER_IDX) + (Player_Armies_And_Enchantments_Mana_Upkeep(HUMAN_PLAYER_IDX) / 2))
    )
    {

        Human_Stronger = ST_TRUE;

    }
    else
    {

        Human_Stronger = ST_FALSE;

    }

    city_count = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        if(_CITIES[itr_cities].owner_idx == player_idx)
        {

            city_count++;

        }

    }


    // ; BUG: will always branch as a result of wrong player
    // ; indexing; one of these were likely meant to call the
    // ; function with player index 0 (human player)
    if(
        (Player_Known_Spell_Count(player_idx) >= 20)
        ||
        (Player_Known_Spell_Count(player_idx) * 3 / 4) > Player_Known_Spell_Count(player_idx)
    )
    {

        Need_Research = ST_TRUE;

    }
    else
    {

        Need_Research = ST_TRUE;

    }


    Unused_Strategy = 0;

    if(Need_Research == ST_TRUE)
    {

        Unused_Strategy = 5;

    }

    if(city_count < 4)
    {

        Unused_Strategy = 4;

    }

    if(city_count < 10)
    {

        Unused_Strategy = 3;

    }

    if(Human_Stronger == ST_TRUE)
    {

        Unused_Strategy = 1;

    }

    if((Player_Armies_Gold_Upkeep(player_idx) * 2) > _players[player_idx].gold_reserve)
    {

        Unused_Strategy = 2;

    }


    Power_Strategy = 0;

    if(Human_Stronger == ST_TRUE)
    {

        Power_Strategy = 3;

    }

    if(
        (Player_Base_Casting_Skill(player_idx) < Player_Base_Casting_Skill(HUMAN_PLAYER_IDX) * 3 / 2)
        ||
        (Player_Base_Casting_Skill(player_idx) < 20)
    )
    {

        Power_Strategy = 2;

    }

    if(Need_Research == ST_TRUE)
    {

        Power_Strategy = 1;

    }

    if(At_War == ST_TRUE)
    {

        Power_Strategy = 4;

    }

    if(
        (Player_Knows_Spell(player_idx, spl_Spell_Blast) == ST_TRUE)
        ||
        (Player_Knows_Spell(player_idx, spl_Drain_Power) == ST_TRUE)
        ||
        (Player_Knows_Spell(player_idx, spl_Cruel_Unminding) == ST_TRUE)
    )
    {

        if(_players[HUMAN_PLAYER_IDX].casting_spell_idx == spl_Spell_Of_Mastery)
        {
            Power_Strategy = 5;
        }

        if(_players[player_idx].Hostility[HUMAN_PLAYER_IDX] == 2)
        {

            if(spell_data_table[_players[player_idx].casting_spell_idx].casting_cost >= spell_data_table[spl_Cruel_Unminding].casting_cost)
            {

                Power_Strategy = 5;

            }

        }

    }

    _players[player_idx].Magic_Strategy = Power_Strategy;

}


// WZD o164p07
// drake178: AI_Evaluate_War()
// ¿ MoO2  Module: DIPLOMAC  Diplomacy_Growth_() ?
// ¿ MoO2  Module: AIMOEV  Player_Is_Hostile_To_Player_() ?
/*
decrease the war reevaluation counter, and if 0 or
less reset to Rnd(10)+15 and perform a war
evaluation, resetting Hostility with all other
players, then deciding whether to set it to 2, 3, or
4 instead

BUG: considers the casting of Fire Elemental as an
immediate call for war instead of the Spell of
Mastery in the first instance
*/
/*

sets _players[].Hostility[]

evaluate diplomatic relations
every 15 to 25 turns
always Warlike with Neutral Player


*/
void AI_Evaluate_Hostility(int16_t player_idx)
{
    int16_t Hostility_Chance = 0;
    int16_t itr_players = 0;  // _DI_

    if(_turn < 100)
    {

        return;

    }

    _players[player_idx].reevaluate_hostility_countdown -= 1;

    if(_players[player_idx].reevaluate_hostility_countdown > 0)
    {

        return;

    }

    _players[player_idx].reevaluate_hostility_countdown = (15 + Random(10));

    _players[player_idx].Hostility[5] = 3;  // Change_Relations(): sets 3 if Dipl_Status[] >= 3  ~ DIPL_War

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        _players[player_idx].Hostility[itr_players] = 0;  // Change_Relations():  sets 0 if Dipl_Status[] == 2  ~ DIPL_Alliance

        if(
            (itr_players == player_idx)
            ||
            (_players[player_idx].Dipl.Contacted[itr_players] != ST_TRUE)
        )
        {

            continue;

        }

        if(
            (_players[player_idx].Dipl.Dipl_Status[itr_players] == DIPL_War)
            ||
            (_players[player_idx].casting_spell_idx == spl_Fire_Elemental)
        )
        {

            // ; set Hostility to 3, or randomly to 4 if the source
            // ; wizard is Maniacal (40%), Ruthless (20%), or Chaotic
            // ; (10%)

            _players[player_idx].Hostility[itr_players] = 3;

            if(_players[player_idx].Personality == PRS_Maniacal)
            {

                if(Random(10) <= 4)  // 40% chance
                {

                    _players[player_idx].Hostility[itr_players] = 4;

                }

            }
            else if(_players[player_idx].Personality == PRS_Ruthless)
            {

                if(Random(10) <= 2)  // 20% chance
                {

                    _players[player_idx].Hostility[itr_players] = 4;

                }

            }
            else if(_players[player_idx].Personality == PRS_Chaotic)
            {

                if(Random(10) <= 1)  // 10% chance
                {

                    _players[player_idx].Hostility[itr_players] = 4;

                }

            }

        }
        else
        {

            if(
                (
                    (
                        (_players[player_idx].Dipl.Dipl_Status[itr_players] != DIPL_Alliance)
                        &&
                        (_players[player_idx].Dipl.Dipl_Status[itr_players] != DIPL_WizardPact)
                    )
                    ||
                    (_players[player_idx].peace_duration[itr_players] == 0)
                )
                ||
                (_players[itr_players].casting_spell_idx == spl_Spell_Of_Mastery)
            )
            {

                if(_players[player_idx].Dipl.Visible_Rel[itr_players] < 0)
                {

                    Hostility_Chance = ((50 + -(_players[player_idx].Dipl.Visible_Rel[itr_players])) / 2);

                }
                else
                {

                    Hostility_Chance = ((100 - _players[player_idx].Dipl.Visible_Rel[itr_players]) / 4);

                }

                if(
                    (_players[player_idx].Personality == PRS_Lawful)
                    &&
                    (_players[player_idx].Dipl.Hidden_Rel[itr_players] < 0)
                )
                {

                    Hostility_Chance -= TBL_AI_PRS_War_Mod[PRS_Lawful];

                }
                else
                {

                    Hostility_Chance += TBL_AI_PRS_War_Mod[_players[player_idx].Personality];

                }

                Hostility_Chance += _players[player_idx].Dipl.Hidden_Rel[itr_players];

                if(_players[itr_players].casting_spell_idx == spl_Spell_Of_Mastery)
                {

                    Hostility_Chance += 50;

                }

                if(Random(100) < Hostility_Chance)
                {

                    _players[player_idx].Hostility[itr_players] = 2;  // OSG: "Annoyed"

                    if(Random(100) < Hostility_Chance)
                    {

                        if(Random(100) < Hostility_Chance)
                        {

                            _players[player_idx].Hostility[itr_players] = 2;  // OSG: "Warlike"

                        }

                    }

                }

            }

        }

    }

}


// WZD o164p08
/**
 * @brief Enumerates roaming neutral stacks into the shared AI stack buffer.
 *
 * Scans the global unit list for units owned by the neutral player and builds
 * one AI stack entry per occupied map square, excluding neutral units that are
 * stationed inside cities. When multiple neutral units share the same square,
 * they are coalesced into a single entry in AI_Own_Stacks.
 *
 * Each recorded stack stores its map location, current unit status, and a
 * coarse movement-capability flag derived from the units encountered at that
 * square. If any unit in an already-recorded stack lacks air and water travel,
 * the stack's abilities field is reduced to AICAP_None.
 *
 * @note This routine resets AI_Own_Stack_Count before rebuilding the list and
 *       stops after filling 80 entries, matching the allocated stack buffer.
 * @note The function reads global unit and city state from _UNITS, _CITIES,
 *       _units, and _cities, and writes the results to AI_Own_Stacks and
 *       AI_Own_Stack_Count.
 */
void Build_NPC_Stacks(void)
{
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t city_defender = 0;
    int16_t stack_exists = 0;
    int16_t unit_idx = 0;
    int16_t itr = 0;
    struct s_AI_STACK_DATA * stack_ptr;
    struct s_UNIT * curr_unit;
    struct s_CITY * curr_city;

    AI_Own_Stack_Count = 0;
    
    for (unit_idx = 0; unit_idx < _units; unit_idx++)
    {
        if (AI_Own_Stack_Count >= 80)
        {
            break;
        }

        curr_unit = &_UNITS[unit_idx];

        /* Check if unit is owned by Neutral (Owner Index 5) */
        if (curr_unit->owner_idx != NEUTRAL_PLAYER_IDX)
        {
            continue;
        }

        unit_wx = curr_unit->wx;
        unit_wy = curr_unit->wy;
        unit_wp = curr_unit->wp;
        stack_exists = 0;

        /* Check if a stack already exists at this location in the AI's collection */
        for (itr = 0; itr < AI_Own_Stack_Count; itr++)
        {
            stack_ptr = &AI_Own_Stacks[itr];

            if (stack_ptr->wx == (uint8_t)unit_wx &&
                stack_ptr->wy == (uint8_t)unit_wy &&
                stack_ptr->wp == (uint8_t)unit_wp)
            {
                stack_exists = 1;

                /* If unit is part of an existing stack, verify movement capabilities */
                if (!Unit_Has_AirTravel(unit_idx) && !Unit_Has_WaterTravel(unit_idx))
                {
                    stack_ptr->abilities = AICAP_None;
                }
            }
        }

        if (stack_exists == ST_FALSE)
        {
            city_defender = ST_FALSE;
            /* Check if unit is currently inside a city */
            for (itr = 0; itr < _cities; itr++)
            {
                curr_city = &_CITIES[itr];
                if (curr_city->wx == (uint8_t)unit_wx &&
                    curr_city->wy == (uint8_t)unit_wy &&
                    curr_city->wp == (uint8_t)unit_wp)
                {
                    city_defender = ST_TRUE;
                }
            }
            /* If not already in a stack and not in a city, record as a new neutral stack */
            if (city_defender == ST_FALSE)
            {
                stack_ptr = &AI_Own_Stacks[AI_Own_Stack_Count];
                stack_ptr->wx = (uint8_t)unit_wx;
                stack_ptr->wy = (uint8_t)unit_wy;
                stack_ptr->wp = (uint8_t)unit_wp;
                stack_ptr->unit_status = curr_unit->Status;
                /* Determine if the stack has non-standard movement capability */
                if (!Unit_Has_AirTravel(unit_idx) && !Unit_Has_WaterTravel(unit_idx))
                {
                    stack_ptr->abilities = AICAP_None;
                }
                else
                {
                    stack_ptr->abilities = AICAP_LandOnly;
                }
                AI_Own_Stack_Count++;
            }
        }
    }
}
