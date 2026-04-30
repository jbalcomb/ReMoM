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
#include "RACETYPE.h"
#include "Spellbook.h"
#include "UnitMove.h"
#include "UNITTYPE.h"
#include "WZD_o059.h"

#include "AIDATA.h"

#include "../../STU/src/AI_METRICS.h"
#include "../../STU/src/STU_DBG.h"

#include <stdlib.h>



/*
    WIZARDS.EXE  ovr145
*/

// WZD o145p0





/*
    WIZARDS.EXE  ovr164
*/

// WZD o164p01
void NPC_Excess_Garrison(void)
{
    int16_t max_garrison = 0;
    int16_t city_wp = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t cheapest_unit = 0;
    int16_t lowest_value = 0;
    int16_t troop_count = 0;
    int16_t i = 0;
    int16_t j = 0;

    for (i = 0; i < _cities; i++)
    {
        if (_CITIES[i].owner_idx == NEUTRAL_PLAYER_IDX)
        {
            city_wx = _CITIES[i].wx;
            city_wy = _CITIES[i].wy;
            city_wp = _CITIES[i].wp;

            lowest_value = 1000;
            cheapest_unit = ST_UNDEFINED;
            troop_count = 0;

            for (j = 0; j < _units; j++)
            {
                if (_UNITS[j].wp == city_wp && 
                    _UNITS[j].wx == city_wx && 
                    _UNITS[j].wy == city_wy)
                {
                    troop_count++;

                    /* Check cost of unit to find the cheapest in the garrison */
                    if (_unit_type_table[_UNITS[j].type].cost < lowest_value)
                    {
                        lowest_value = _unit_type_table[_UNITS[j].type].cost;
                        cheapest_unit = j;
                    }
                }
            }

            /* Calculate base max garrison from population */
            max_garrison = _CITIES[i].population;

            if (_CITIES[i].bldg_status[GRANARY] == bs_Built || 
                _CITIES[i].bldg_status[GRANARY] == bs_Replaced)
            {
                max_garrison += 2;
            }

            if (_CITIES[i].bldg_status[FARMERS_MARKET] == bs_Built || 
                _CITIES[i].bldg_status[FARMERS_MARKET] == bs_Replaced)
            {
                max_garrison += 2;
            }

            /* If garrison exceeds limits, remove the cheapest unit */
            if (troop_count > max_garrison && cheapest_unit != ST_UNDEFINED)
            {
#ifdef STU_DEBUG
                dbg_prn("AI_NPC: Excess garrison at city %d (%d/%d), removing unit %d (type %d)\n", i, troop_count, max_garrison, cheapest_unit, _UNITS[cheapest_unit].type);
#endif
                AI_Metrics_Emit_NPC_Event(_turn, "GARRISON_CULL", i, city_wx, city_wy, city_wp, 1, 0, 0, 0, troop_count, max_garrison);
                Kill_Unit(cheapest_unit, kt_Normal);
            }
        }
    }
}


// WZD o164p02
void Make_Monsters(void)
{
    int16_t rampage_budget = 0;
    int16_t valid_zone_count = 0;
    int16_t lair_idx = 0;
    int16_t lair_wx = 0;
    int16_t lair_wy = 0;
    int16_t lair_wp = 0;
    int16_t lair_landmass_idx = 0;
    int16_t adjacent_wx = 0;
    int16_t adjacent_wy = 0;
    int16_t tries = 0;
    int16_t rolled_idx = 0;
    int16_t lair_race = 0;
    int16_t n_monsters = 0;
    int16_t ai_home_continent = 0;
    int16_t uu_bogus = 0;
    int16_t monster_type_list[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;
    int16_t city_match_found = 0;
    uint8_t * landmass_ptr = NULL;

    valid_zone_count = 0;

    /* First pass: Count how many lairs are intact and eligible to spawn rampages */
    for (itr = 0; itr < NUM_LAIRS; itr++)
    {
        if (_LAIRS[itr].intact == ST_TRUE && _LAIRS[itr].guard1_unit_type != 0)
        {
            /* If the guards are 'Life' based, they don't rampage */
            if (_unit_type_table[_LAIRS[itr].guard1_unit_type].race_type != rt_Life)
            {
                valid_zone_count++;
            }
        }
    }

    if (valid_zone_count == 0)
    {
        return;
    }

    /* Update the rampage accumulator based on difficulty */
    _players[NEUTRAL_PLAYER_IDX].average_unit_cost += Random(_difficulty + 1);

    /* Check if the accumulator has reached the threshold: (50 - (difficulty * 5)) */
    if (_players[NEUTRAL_PLAYER_IDX].average_unit_cost < (50 - (_difficulty * 5)))
    {
#ifdef STU_DEBUG
        dbg_prn("AI_NPC: Make_Monsters accumulator %d < threshold %d\n", _players[NEUTRAL_PLAYER_IDX].average_unit_cost, (50 - (_difficulty * 5)));
#endif
        return;
    }

    /* Threshold reached: reset and check turn minimum */
#ifdef STU_DEBUG
    dbg_prn("AI_NPC: Make_Monsters accumulator reached threshold, turn=%d\n", _turn);
#endif
    _players[NEUTRAL_PLAYER_IDX].average_unit_cost = 0;
    if (_turn < 50)
    {
#ifdef STU_DEBUG
        dbg_prn("AI_NPC: Make_Monsters skipped (turn %d < 50)\n", _turn);
#endif
        return;
    }

    tries = 0;
    lair_idx = ST_UNDEFINED;

    /* Search for an eligible lair that is on the same continent as a non-neutral city */
    while (lair_idx == ST_UNDEFINED && tries < 1000)
    {
        rolled_idx = (Random(NUM_LAIRS) - 1);

        if (_LAIRS[rolled_idx].intact == ST_TRUE && _LAIRS[rolled_idx].guard1_unit_type != 0)
        {
            if (_unit_type_table[_LAIRS[rolled_idx].guard1_unit_type].race_type != rt_Life)
            {
                /* Check if this lair is on a continent with any player city */
                city_match_found = 0;
                for (itr = 0; itr < _cities; itr++)
                {
                    if (_CITIES[itr].owner_idx != NEUTRAL_PLAYER_IDX)
                    {
                        if (_CITIES[itr].wp == _LAIRS[rolled_idx].wp)
                        {
                            if (_landmasses[_CITIES[itr].wp * WORLD_SIZE + _CITIES[itr].wy * WORLD_WIDTH + _CITIES[itr].wx] ==
                                _landmasses[_LAIRS[rolled_idx].wp * WORLD_SIZE + _LAIRS[rolled_idx].wy * WORLD_WIDTH + _LAIRS[rolled_idx].wx])
                            {
                                lair_idx = rolled_idx;
                                break;
                            }
                        }
                    }
                }

                /* 33% chance to force the rampage to be on the same plane as the human fortress */
                if (lair_idx != ST_UNDEFINED && Random(3) == 2)
                {
                    if (_FORTRESSES[HUMAN_PLAYER_IDX].wp != _LAIRS[rolled_idx].wp)
                    {
                        lair_idx = ST_UNDEFINED; /* Plane mismatch, retry search */
                    }
                }
            }
        }
        tries++;
    }

    if (tries >= 1000 || lair_idx == ST_UNDEFINED)
    {
#ifdef STU_DEBUG
        dbg_prn("AI_NPC: Make_Monsters failed to find eligible lair after %d tries\n", tries);
#endif
        AI_Metrics_Emit_NPC_Event(_turn, "MONSTER_FAIL", -1, 0, 0, 0, 0, 0, 0, 0, _players[NEUTRAL_PLAYER_IDX].average_unit_cost, (50 - (_difficulty * 5)));
        return;
    }

    /* Lair selected, extract map data */
#ifdef STU_DEBUG
    dbg_prn("AI_NPC: Make_Monsters selected lair %d at (%d,%d) plane %d after %d tries\n", lair_idx, _LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, tries);
#endif
    lair_wx = _LAIRS[lair_idx].wx;
    lair_wy = _LAIRS[lair_idx].wy;
    lair_wp = _LAIRS[lair_idx].wp;
    lair_landmass_idx = _landmasses[lair_wp * WORLD_SIZE + lair_wy * WORLD_WIDTH + lair_wx];

    /* Determine if an AI player's fortress is on this continent */
    ai_home_continent = 0;

    /* BUG: The code compares the human landmass (Player 0) without checking the plane first */
    /* This can cause ai_home_continent to be 0 even if an AI is on the same landmass ID on a different plane */
    // slightly different than Raiders, which doesn't branch on the human players home continent
    // here, means we a flag for whether the rampage is on a continent the human is sharing with an AI
    if (_landmasses[_FORTRESSES[HUMAN_PLAYER_IDX].wp * WORLD_SIZE + _FORTRESSES[HUMAN_PLAYER_IDX].wy * WORLD_WIDTH + _FORTRESSES[HUMAN_PLAYER_IDX].wx] != lair_landmass_idx)
    {
        for (itr = 1; itr < _num_players; itr++)
        {
            if (_landmasses[_FORTRESSES[itr].wp * WORLD_SIZE + _FORTRESSES[itr].wy * WORLD_WIDTH + _FORTRESSES[itr].wx] == lair_landmass_idx)
            {
                ai_home_continent = ST_TRUE;
                break;
            }
        }
    }

    /* Determine monster race from the lair's primary guard */
    lair_race = ST_UNDEFINED;
    if (_LAIRS[lair_idx].guard1_count > 0)
    {
        lair_race = _unit_type_table[_LAIRS[lair_idx].guard1_unit_type].race_type;
        if (lair_race < rt_Arcane)
        {
            lair_race = rt_Death;
        }
    }

    if (lair_race == ST_UNDEFINED)
    {
        return;
    }

    /* Find a valid adjacent tile to spawn the stack */
    if (Adjacent_Free_Square(lair_wx, lair_wy, lair_wp, &adjacent_wx, &adjacent_wy) == 0)
    {
        return;
    }

    /* Calculate budget for monsters: ((Rand(diff+1) + Rand(diff+1)) * turn) / 5 */
    rampage_budget = (Random(_difficulty + 1) + Random(_difficulty + 1));
    rampage_budget = (rampage_budget * _turn) / 5;

    /* If the continent belongs to an AI, halve the budget */
    if (ai_home_continent == 1)
    {
        rampage_budget /= 2;
    }

    /* Generate the monster stack based on budget and race */
    n_monsters = Make_Monster_List(rampage_budget, lair_race, &monster_type_list[0]);

#ifdef STU_DEBUG
    dbg_prn("AI_NPC: Make_Monsters spawning %d monsters (budget=%d) at (%d,%d) plane %d\n", n_monsters, rampage_budget, adjacent_wx, adjacent_wy, lair_wp);
#endif
    AI_Metrics_Emit_NPC_Event(_turn, "MONSTER_SPAWN", lair_idx, _LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, lair_wp, n_monsters, rampage_budget, adjacent_wx, adjacent_wy, _players[NEUTRAL_PLAYER_IDX].average_unit_cost, (50 - (_difficulty * 5)));

    /* Create the units on the map */
    for (itr = 0; itr < n_monsters; itr++)
    {
        Create_Unit__WIP(monster_type_list[itr], NEUTRAL_PLAYER_IDX, adjacent_wx, adjacent_wy, lair_wp, -1);
    }

}


// WZD o164p03
/**
 * @brief Attempts to spawn a neutral raider stack from a qualifying neutral city.
 *
 * The routine first checks whether any neutral cities exist, then uses the
 * neutral player's @c casting_cost_original field as a turn-over-turn spawn
 * accumulator. Once that accumulator reaches the threshold, the function makes
 * repeated attempts to choose a neutral city whose continent also contains at
 * least one non-neutral city and an adjacent free land square suitable for
 * spawning raiders.
 *
 * When a valid source city is found, the city's current garrison is sampled to
 * determine how many raiders to create and which unit types may be copied.
 * Raider count is scaled by difficulty, reduced if an AI fortress is present
 * on the same landmass, and further reduced on early-game Myrror. Newly
 * created raiders inherit their unit type from random defenders, excluding
 * transport and outpost-capable units and preserving the original game's local
 * Barbarian-unit exclusion noted in the implementation.
 *
 * After successful creation, a fraction of the original defenders are removed
 * to represent the raiding party leaving the city. If all generation attempts
 * fail after the accumulator triggers, the function instead increments the
 * neutral player's @c average_unit_cost field, which this code uses as the
 * fallback rampaging-monster accumulator.
 *
 * @note This routine has no parameters and no direct return value; it mutates
 *       global player, city, unit, and fortress state.
 * @note Reads from globals including @c _players, @c _CITIES, @c _UNITS,
 *       @c _FORTRESSES, and @c _landmasses, and may call
 *       @c Create_Unit__WIP(), @c Kill_Unit(), @c Army_At_Square_2(), and
 *       @c Adjacent_Free_Square() as part of the spawn attempt.
 */
/* GEMINI */
void Make_Raiders(void)
{
    int16_t itr = 0;
    int16_t city_wx = 0;
    int16_t city_wy = 0;
    int16_t city_wp = 0;
    int16_t unused_local = 0;
    int16_t empty_adjacent_x = 0;
    int16_t empty_adjacent_y = 0;
    int16_t raiders_level_neg = 0;
    int16_t city_landmass_idx = 0;
    int16_t troop_count = 0;
    int16_t raiders_count = 0;
    int16_t have_neutral_city = 0;
    int16_t did_create = 0;
    int16_t tries = 0;
    int16_t rolled_city = 0;
    int16_t have_non_neutral = 0;
    int16_t unit_type = 0;
    int16_t units_created = 0;
    int16_t have_ai_fortress = 0;
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
    int16_t itr2 = 0;
    struct s_CITY * city_ptr = NULL;
    struct s_FORTRESS * fortress_ptr = NULL;
    struct s_UNIT * unit_ptr = NULL;

    have_neutral_city = 0;

    /* Check if any neutral cities exist on the map */
    for (itr = 0; itr < _cities; itr++)
    {
        city_ptr = &_CITIES[itr];
        if (city_ptr->owner_idx == NEUTRAL_PLAYER_IDX)
        {
            have_neutral_city = ST_TRUE;
        }
    }

    if (have_neutral_city == ST_FALSE)
    {
        return;
    }

    /* Update raider spawn accumulator */
    _players[NEUTRAL_PLAYER_IDX].casting_cost_original += Random(_difficulty + 1);

    if (_players[NEUTRAL_PLAYER_IDX].casting_cost_original < 30)
    {
#ifdef STU_DEBUG
        dbg_prn("AI_NPC: Make_Raiders accumulator %d < 30\n", _players[NEUTRAL_PLAYER_IDX].casting_cost_original);
#endif
        return;
    }

    /* Reset raider accumulator and start generation attempts */
#ifdef STU_DEBUG
    dbg_prn("AI_NPC: Make_Raiders accumulator reached 30, attempting spawn\n");
#endif
    _players[NEUTRAL_PLAYER_IDX].casting_cost_original = 0;
    unused_local = 0;
    did_create = 0;

    for (tries = 0; tries < 1000 && did_create == 0; tries++)
    {
        /* BUG: Random(cities) - 1 can result in index -1 */
        rolled_city = Random(_cities) - 1;

        city_ptr = &_CITIES[rolled_city];
        if (city_ptr->owner_idx != NEUTRAL_PLAYER_IDX)
        {
            continue;
        }

        city_wx = city_ptr->wx;
        city_wy = city_ptr->wy;
        city_wp = city_ptr->wp;

        /* Look up landmass index for the city */
        city_landmass_idx = _landmasses[(city_wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx];

        /* Check for presence of AI fortresses on the same landmass */
        have_ai_fortress = 0;
        for (itr = 1; itr < _num_players; itr++)
        {
            fortress_ptr = &_FORTRESSES[itr];
            /* BUG: Uses wp/wy/wx to index landmasses but doesn't strictly verify wp matches city_wp */
            if (_landmasses[(fortress_ptr->wp * WORLD_SIZE) + (fortress_ptr->wy * WORLD_WIDTH) + fortress_ptr->wx] == city_landmass_idx)
            {
                have_ai_fortress = ST_TRUE;
            }
        }

        /* Check if there is at least one non-neutral city on the same landmass */
        have_non_neutral = 0;
        for (itr = 0; itr < _cities; itr++)
        {
            city_ptr = &_CITIES[itr];
            if (city_ptr->owner_idx != NEUTRAL_PLAYER_IDX)
            {
                if (_landmasses[(city_ptr->wp * WORLD_SIZE) + (city_ptr->wy * WORLD_WIDTH) + city_ptr->wx] == city_landmass_idx)
                {
                    have_non_neutral = ST_TRUE;
                    break;
                }
            }
        }

        if (have_non_neutral == ST_FALSE)
        {
            continue;
        }

        /* Find a valid adjacent tile for the raiders to spawn */
        if (Adjacent_Free_Square(city_wx, city_wy, city_wp, &empty_adjacent_x, &empty_adjacent_y) != ST_TRUE)
        {
            continue;
        }

        /* Get the garrison of the neutral city */
        Army_At_Square_2(city_wx, city_wy, city_wp, &troop_count, troops);

        if (troop_count <= 0)
        {
            continue;
        }

        /* Calculate how many raiders should spawn */
        raiders_count = (troop_count * _difficulty) / 6;

        // Reduce raiders by 1/3 if AI fortress is present
        // What the game-play rationale for this?
        // Being nice to the human player, assuming they are already having to deal with a Computer Player being nearby?
        // Being a cheatin' ass bitch on behalf of Computer Player?
        if (have_ai_fortress == ST_TRUE)
        {
            raiders_count = (raiders_count * 2) / 3;
        }

        /* Myrror raiders are reduced in early game */
        // Again, intent? Is life harder on Myrror?
        if (city_wp == MYRROR_PLANE && _turn < 200)
        {
            raiders_count /= 2;
        }

        if (raiders_count < 1)
        {
            raiders_count = 1;
        }

        /* Determine experience level based on game turn */
        if (_turn > 250)
        {
            raiders_level_neg = -4;
        }
        else if (_turn > 120)
        {
            raiders_level_neg = -3;
        }
        else if (_turn > 40)
        {
            raiders_level_neg = -2;
        }
        else
        {
            raiders_level_neg = -1;
        }

        units_created = 0;
        for (itr = 0; itr < raiders_count; itr++)
        {

            unit_type = _UNITS[troops[(Random(troop_count) - 1)]].type;

            /* OGBUG: This comparison excludes barbarian spearmen and swordsmen from raiding */
            if (unit_type <= ut_BarbSwordsmen)
            {
                continue;
            }

            /* Raiders cannot be transports or settlers */
            if (_unit_type_table[unit_type].Transport != 0)
            {
                continue;
            }

            if (_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST)
            {
                continue;
            }

            /* Create the raider unit for the neutral player */
#ifdef STU_DEBUG
            dbg_prn("AI_NPC: Make_Raiders creating unit type %d at (%d,%d) plane %d, level %d\n", unit_type, empty_adjacent_x, empty_adjacent_y, city_wp, raiders_level_neg);
#endif
            Create_Unit__WIP(unit_type, NEUTRAL_PLAYER_IDX, empty_adjacent_x, empty_adjacent_y, city_wp, raiders_level_neg);
            units_created++;
        }

        /* Remove a portion of the original garrison to 'form' the raider party */
        for (itr2 = 0; itr2 < (units_created / 3); itr2++)
        {
            Kill_Unit(troops[itr2], kt_Normal);
        }

#ifdef STU_DEBUG
        dbg_prn("AI_NPC: Make_Raiders spawned %d raiders from city %d at (%d,%d) plane %d, removed %d garrison\n", units_created, rolled_city, city_wx, city_wy, city_wp, (units_created / 3));
#endif
        AI_Metrics_Emit_NPC_Event(_turn, "RAIDER_SPAWN", rolled_city, city_wx, city_wy, city_wp, units_created, raiders_count, 0, 0, _players[NEUTRAL_PLAYER_IDX].casting_cost_original, 30);
        did_create = ST_TRUE;
    }

    /* If raider generation failed, increment the rampaging monster accumulator instead */
    if (did_create == ST_FALSE)
    {
#ifdef STU_DEBUG
        dbg_prn("AI_NPC: Make_Raiders failed after 1000 tries, boosting monster accumulator by 15\n");
#endif
        AI_Metrics_Emit_NPC_Event(_turn, "RAIDER_FAIL", -1, 0, 0, 0, 0, 0, 0, 0, _players[NEUTRAL_PLAYER_IDX].casting_cost_original, 30);
        _players[NEUTRAL_PLAYER_IDX].average_unit_cost += 15;
    }

}


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

#ifdef STU_DEBUG
    dbg_prn("AI_NPC: NPC_Destinations evaluating %d neutral stacks\n", AI_Own_Stack_Count);
#endif

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
#ifdef STU_DEBUG
                    dbg_prn("AI_NPC: Stack %d at (%d,%d) redirected to adjacent city %d at (%d,%d) (val %d > %d)\n", stack_idx, stack_wx, stack_wy, adj_city_idx, _CITIES[adj_city_idx].wx, _CITIES[adj_city_idx].wy, adj_city_val, cur_target_val);
#endif
                    AI_Metrics_Emit_NPC_Event(_turn, "NPC_REDIRECT", stack_idx, stack_wx, stack_wy, stack_wp, 0, adj_city_val, _CITIES[adj_city_idx].wx, _CITIES[adj_city_idx].wy, 0, 0);
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
                if ((stack->abilities & AICAP_LandOnly) != 0) {
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
#ifdef STU_DEBUG
                dbg_prn("AI_NPC: Stack %d at (%d,%d) targeting city %d at (%d,%d) (val %d, examined %d cities)\n", stack_idx, stack_wx, stack_wy, target_city_idx, _CITIES[target_city_idx].wx, _CITIES[target_city_idx].wy, adj_city_val, cities_examined);
#endif
                AI_Metrics_Emit_NPC_Event(_turn, "NPC_TARGET", stack_idx, stack_wx, stack_wy, stack_wp, 0, adj_city_val, _CITIES[target_city_idx].wx, _CITIES[target_city_idx].wy, cities_examined, 0);
                AI_Stack_Set_Destination(stack_idx, _CITIES[target_city_idx].wx, _CITIES[target_city_idx].wy, NEUTRAL_PLAYER_IDX);
                stack->unit_status = us_GOTO;
            } else {
                /* No reachable targets: disband the stack to clear conventional memory */
                /* same as seen in MoO2 ... `if(NPC_Dest_() == ST_UNDEFINED)` */
#ifdef STU_DEBUG
                dbg_prn("AI_NPC: Stack %d at (%d,%d) disbanded (no reachable targets)\n", stack_idx, stack_wx, stack_wy);
#endif
                AI_Metrics_Emit_NPC_Event(_turn, "NPC_DISBAND", stack_idx, stack_wx, stack_wy, stack_wp, 0, 0, 0, 0, 0, 0);
                for (itr = 0; itr < _units; itr++) {
                    if (_UNITS[itr].wx == stack_wx && _UNITS[itr].wy == stack_wy && _UNITS[itr].wp == stack_wp) {
                        Kill_Unit(itr, kt_Normal);
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
