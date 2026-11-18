/*
    City Calculations

    WIZARDS.EXE
        ovr120
        ovr142
*/

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_GARB.h"
#include "../../STU/src/STU_LOG.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_BITS.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_UPD.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/Util.h"
#include "../../MoX/src/EMS/EMS.h"  /* EMMDATAH_Map() used in Do_City_Calculations */

#include "AIDUDES.h"
#include "Items.h"
#include "MovePath.h"
#include "RACETYPE.h"
#include "SETTLE.h"
#include "Spellbook.h"
#include "EVENTS.h"     /* es_Ongoing */
#include "Terrain.h"
#include "UNITTYPE.h"   // WTFMATE

#include "../../ext/stu_compat.h"

#include <assert.h>
#include <math.h>       /* sqrt() */
#include <stdlib.h>     /* abs(); stu_itoa(); malloc(); */
#include <string.h>     /* memcpy() memset(), stu_strcat(), stu_strcpy(), stricmp() */

#include "CITYCALC.h"



/*
    WIZARDS.EXE  ovr120
*/


// WZD o120p01
/*
    sets
    _CITIES[].food2_units
    _CITIES[].Production
    _CITIES[].Gold
    _CITIES[].Upkeep
    _CITIES[].Research
    _CITIES[].Power
*/
void Do_City_Calculations(int16_t city_idx)
{

    TRACE_ENTER();

    EMMDATAH_Map();
    /* OGBUG  every value is stored as a byte: anything of 256+ wraps mod 256 (e.g. production 280 is stored as 24) */
    CITIES_FOOD_UNITS(city_idx, (int8_t)City_Food_Production(city_idx));
    CITIES_PRODUCTION_UNITS(city_idx, (int8_t)City_Production_Production(city_idx));
    CITIES_GOLD_UNITS(city_idx, (uint8_t)City_Gold_Production(city_idx));
    CITIES_BUILDING_MAINTENANCE(city_idx, (int8_t)City_Gold_Mainanence(city_idx));
    CITIES_RESEARCH_UNITS(city_idx, (int8_t)City_Research_Production(city_idx));
    CITIES_MANA_UNITS(city_idx, (int8_t)City_Mana_Production(city_idx));
    if(
        (_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
        &&
        (_CITIES[city_idx].owner_idx != NEUTRAL_PLAYER_IDX)
    )
    {
        /* OGBUG  the AI difficulty scaling is applied to the stored byte and stored back as a byte: a scaled value of 256+ wraps mod 256 (Impossible x400%: base 64+; Hard x200%: base 128+), e.g. production 70 -> 280 -> 24 */
        // CITIES_FOOD_UNITS(city_idx,           ( ( _CITIES[city_idx].food_units           * difficulty_modifiers_table[_difficulty].food                 ) / 100 ) );
        // CITIES_PRODUCTION_UNITS(city_idx,     ( ( _CITIES[city_idx].production_units     * difficulty_modifiers_table[_difficulty].production           ) / 100 ) );
        // CITIES_MANA_UNITS(city_idx,           ( ( _CITIES[city_idx].mana_units           * difficulty_modifiers_table[_difficulty].mana                 ) / 100 ) );
        // CITIES_GOLD_UNITS(city_idx,           ( ( _CITIES[city_idx].gold_units           * difficulty_modifiers_table[_difficulty].gold                 ) / 100 ) );
        // CITIES_RESEARCH_UNITS(city_idx,       ( ( _CITIES[city_idx].research_units       * difficulty_modifiers_table[_difficulty].research             ) / 100 ) );
        // CITIES_BUILDING_MAINTENANCE(city_idx, ( ( _CITIES[city_idx].building_maintenance * difficulty_modifiers_table[_difficulty].maintenance          ) / 100 ) );
        /* CLAUDE */  /* REBUG */  /* asm: each byte is read zero-extended (mov al, [es:bx+...] / mov ah, 0), not sign-extended */
        CITIES_FOOD_UNITS(city_idx,           ( ( _CITIES[city_idx].food_units           * difficulty_modifiers_table[_difficulty].food                 ) / 100 ) );
        CITIES_PRODUCTION_UNITS(city_idx,     ( ( _CITIES[city_idx].production_units     * difficulty_modifiers_table[_difficulty].production           ) / 100 ) );
        CITIES_MANA_UNITS(city_idx,           ( ( (uint8_t)_CITIES[city_idx].mana_units           * difficulty_modifiers_table[_difficulty].mana                 ) / 100 ) );
        CITIES_GOLD_UNITS(city_idx,           ( ( (uint8_t)_CITIES[city_idx].gold_units           * difficulty_modifiers_table[_difficulty].gold                 ) / 100 ) );
        CITIES_RESEARCH_UNITS(city_idx,       ( ( _CITIES[city_idx].research_units       * difficulty_modifiers_table[_difficulty].research             ) / 100 ) );
        CITIES_BUILDING_MAINTENANCE(city_idx, ( ( _CITIES[city_idx].building_maintenance * difficulty_modifiers_table[_difficulty].maintenance          ) / 100 ) );
    }

    TRACE_EXIT();

}


// WZD o120p02
/**
 * @brief Recomputes each player's base magic power from nodes, cities, and global effects.
 *
 * @details
 * This routine fully rebuilds `_players[].Power_Base` each turn in ordered phases:
 * 1) reset per-player power to 0;
 * 2) early-out during active Mana Short;
 * 3) accumulate node power with warped-node penalties, conjunction modifiers,
 *    and mastery multipliers;
 * 4) apply AI difficulty mana bonus to non-human players;
 * 5) add city mana production for non-neutral city owners;
 * 6) apply final state adjustments (Spell of Return and volcanoes), then clamp
 *    to a minimum of 0.
 *
 * Node power starts from `(node_power * (magic_strength + 1)) / 2` and is then
 * transformed by conjunction and mastery rules. Warped nodes contribute a flat
 * `-5` instead of normal node output.
 *
 * @note A global Mana Short event (status == 2) cancels all node/city power by
 *       returning after the reset phase.
 * @note The difficulty modifier is applied only to players in range
 *       `[1, _num_players)`, preserving human-player baseline.
 *
 * @warning This function mutates global player state in `_players[]` and relies
 *          on globally shared world/event tables.
 */
void Players_Update_Magic_Power(void)
{
    int16_t niu_players_power_base_array[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t node_owner_idx = 0;
    int16_t itr = 0;
    int16_t node_magic_power_points = 0;

    TRACE_ENTER();


    /* Phase 1 */
    for(itr = 0; itr < _num_players; itr++)
    {
        _players[itr].Power_Base = 0;
        niu_players_power_base_array[itr] = 0;
    }


    /* Phase 2 */
    /* Global Mana Short event cancels all node and city power */
    if(events_table->Mana_Short_Status == 2)  /* ¿ 2 means "active" ? */
    {
        TRACE_EXIT();
        return;
    }


    /* Phase 3 */
    /* Calculate Power from Magic Nodes */
    for(itr = 0; itr < NUM_NODES; itr++)
    {

        /* Phase 3a */
        if(_NODES[itr].owner_idx <= ST_UNDEFINED)
        {
            continue;
        }
    
        /* Phase 3b */
        node_owner_idx = _NODES[itr].owner_idx;

        /* Phase 3c */
        /* Warped nodes drain power instead of providing it */
        if((_NODES[itr].flags & NF_WARPED) > 0)
        {
            _players[node_owner_idx].Power_Base -= 5;
            continue;
        }

        /* Phase 3d */
        /* Base Node Power Calculation: (Node Power * (Magic Strength + 1)) / 2 */
        node_magic_power_points = ((_NODES[itr].power * (_magic + 1)) / 2);

        /* Phase 3e: Conjunction Modifiers */
        if(events_table->Conjunction_Chaos_Status == 2)  /* ¿ 2 means "active" ? */
        {
            if(_NODES[itr].type == nt_Sorcery) { node_magic_power_points /= 2; }
            if(_NODES[itr].type == nt_Nature ) { node_magic_power_points /= 2; }
            if(_NODES[itr].type == nt_Chaos  ) { node_magic_power_points *= 2; }
        }
        if(events_table->Conjunction_Sorcery_Status == 2)  /* ¿ 2 means "active" ? */
        {
            if(_NODES[itr].type == nt_Sorcery) { node_magic_power_points *= 2; }
            if(_NODES[itr].type == nt_Nature ) { node_magic_power_points /= 2; }
            if(_NODES[itr].type == nt_Chaos  ) { node_magic_power_points /= 2; }
        }
        if(events_table->Conjunction_Nature_Status == 2)  /* ¿ 2 means "active" ? */
        {
            if(_NODES[itr].type == nt_Sorcery) { node_magic_power_points /= 2; }
            if(_NODES[itr].type == nt_Nature ) { node_magic_power_points *= 2; }
            if(_NODES[itr].type == nt_Chaos  ) { node_magic_power_points /= 2; }
        }

        /* Phase 3f: Mastery Modifiers */
        if((_NODES[itr].type == nt_Sorcery) && (_players[node_owner_idx].sorcery_mastery > ST_FALSE)) { node_magic_power_points *= 2; }
        if((_NODES[itr].type == nt_Chaos  ) && (_players[node_owner_idx].chaos_mastery   > ST_FALSE)) { node_magic_power_points *= 2; }
        if((_NODES[itr].type == nt_Nature ) && (_players[node_owner_idx].nature_mastery  > ST_FALSE)) { node_magic_power_points *= 2; }
        if(_players[node_owner_idx].node_mastery > ST_FALSE) { node_magic_power_points *= 2; }

        /* Phase 3g */
        _players[node_owner_idx].Power_Base += node_magic_power_points;
        
    }  /* for(itr = 0; itr < NUM_NODES; itr++) */


    /* Phase 4 */
    /* Apply AI Difficulty Modifiers (Skip human player 0) */
    /* e.g., * 150 / 100 ~== * 1.5 ~== +50% */
    for(itr = 1; itr < _num_players; itr++)
    {
        _players[itr].Power_Base = ((_players[itr].Power_Base * (difficulty_modifiers_table[_difficulty].mana)) / 100);
    }

    /* Phase 5 */
    /* Calculate Power from Cities */
    for(itr = 0; itr < _cities; itr++)
    {
        if((_CITIES[itr].owner_idx > -1) && (_CITIES[itr].owner_idx != NEUTRAL_PLAYER_IDX))
        {
            _players[_CITIES[itr].owner_idx].Power_Base += _CITIES[itr].mana_units;
        }
    }


    /* Phase 6 */
    /* Final Adjustments: Volcanoes and Special States */
    for(itr = 0; itr < _num_players; itr++)
    {
        if(_players[itr].casting_spell_idx == spl_Spell_Of_Return)
        {
            _players[itr].Power_Base = 0;
        }
        else
        {
            _players[itr].Power_Base += _players[itr].volcanoes;

            SETMIN(_players[itr].Power_Base, 0);
        }
    }
    
    TRACE_EXIT();

}


// WZD o120p03
int16_t Unit_Gold_Upkeep(int16_t unit_idx)
{
    int16_t unit_gold_upkeep = 0;
    int16_t unit_owner_idx = 0;
    unit_owner_idx = _UNITS[unit_idx].owner_idx;
    unit_gold_upkeep = 0;
    if( (_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_FANTASTIC) == 0)
    {
        if(_UNITS[unit_idx].type > ut_Chosen)
        {
            unit_gold_upkeep += _unit_type_table[_UNITS[unit_idx].type].Upkeep;
        }
        else
        {
            // Yay Hero, Nay Noble, Nay 'Toren The Chosen'
            if(
                /* OGBUG  dead hero (owner_idx -1): _HEROES2[-1] is the far pointer combat_enchantments (the dd below _HEROES2 in dseg), so the OG tests a bit of whatever lies */
                /* OGBUG  at combat_enchantments + type * sizeof(s_HERO) */
                // (!HERO_NOBLE(unit_owner_idx, _UNITS[unit_idx].type))
                /* HACK */  (!((unit_owner_idx == ST_UNDEFINED) ? ((Stack_Garbage() & 1) != 0) : HERO_NOBLE(unit_owner_idx, _UNITS[unit_idx].type)))
                &&
                (_UNITS[unit_idx].type != ut_Chosen)
            )
            {
                unit_gold_upkeep += _unit_type_table[_UNITS[unit_idx].type].Upkeep;
            }
        }
    }
    if((_UNITS[unit_idx].mutations & UM_UNDEAD) != 0)
    {
        unit_gold_upkeep = 0;
    }
    if(_UNITS[unit_idx].owner_idx != HUMAN_PLAYER_IDX)
    {
        if(_difficulty == god_Impossible)
        {
            unit_gold_upkeep = (unit_gold_upkeep / 2);  /* -50% */
        }
        if(_difficulty == god_Hard)
        {
            unit_gold_upkeep = ((unit_gold_upkeep * 3) / 4);  /* -25% */
        }
    }
    return unit_gold_upkeep;
}


// WZD o120p04
/*
    Calculate Normal Units Food Maintenance
    Normal Units
    Maintenance Cost / Upkeep Cost
    Gold
    (normal units - one gold piece for every 50 production units)
*/
int16_t Player_Armies_Gold_Upkeep(int16_t player_idx)
{
    int16_t fame_points;
    int16_t gold_upkeep_cost;
    int16_t itr_units;
    
    gold_upkeep_cost = 0;

    fame_points = Player_Fame(player_idx);

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == player_idx)
        {
            gold_upkeep_cost += Unit_Gold_Upkeep(itr_units);
        }
    }

    gold_upkeep_cost -= fame_points;

    SETMIN(gold_upkeep_cost, 0);

    return gold_upkeep_cost;
}

// WZD o120p05
// drake178: WIZ_ArmyUpkeep_Food()
/*
    Calculate Normal Units Food Maintenance
    Normal Units
    Maintenance Cost / Upkeep Cost
    Food
    (normal units - one food unit)
*/
int16_t Player_Armies_Food_Upkeep(int16_t player_idx)
{
    int16_t food_upkeep_cost;
    int16_t itr_units;

    food_upkeep_cost = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == player_idx)
        {
            if(
                ((_unit_type_table[_UNITS[itr_units].type].Abilities & 0x1 /* Ab_Summoned */) == 0) &&
                (_UNITS[itr_units].type > ut_Chosen) &&
                ((_UNITS[itr_units].mutations & 0x20 /* R_Undead */) == 0)
            )
            {
                food_upkeep_cost++;
            }
        }
    }

    return food_upkeep_cost;
}


// WZD o120p06
// drake178: WIZ_GetNUCounts()
/*
*/
/*

*/
void Players_Normal_Units(int16_t normal_units[])
{
    int16_t itr_players = 0;  // _SI_
    int16_t itr_units = 0;  // _SI_

    for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
    {

        normal_units[itr_players] = 0;

    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        /* CLAUDE */  /* REBUG */  /* asm: no skip for dead units (owner_idx -1 / wp -1); the normal_units[-1] write it hid is bounded below */
        // /* HACK */  if(
        // /* HACK */      (_UNITS[itr_units].owner_idx == ST_UNDEFINED)
        // /* HACK */      ||
        // /* HACK */      (_UNITS[itr_units].wp == ST_UNDEFINED)
        // /* HACK */  )
        // /* HACK */  {
        // /* HACK */      continue;
        // /* HACK */  }

        if(
            ((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_FANTASTIC) == 0)
            &&
            (_UNITS[itr_units].type > ut_Chosen)
            &&
            // (_UNITS[itr_units].owner_idx < _num_players)
            (_UNITS[itr_units].owner_idx <= _num_players)  /* CLAUDE */  /* REBUG */  /* asm: cmp ax, [_num_players] / jg @@NextUnit */
            &&
            ((_UNITS[itr_units].mutations & UM_UNDEAD) == 0)
        )
        {

            /* OGBUG  owner_idx -1 (a dead unit) passes the '<= _num_players' test: the OG increments normal_units[-1], the word below the caller's array */
            // normal_units[_UNITS[itr_units].owner_idx] += 1;
            /* HACK */  if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)  /* bounded: no normal_units[-1] write */
            /* HACK */  {
            normal_units[_UNITS[itr_units].owner_idx] += 1;
            /* HACK */  }

        }

    }

}


// WZD o120p07
/*
    Player
    Resources - Gold, Food, Mana

    calls out for player's magic power incomes - mana, research, skill
    calls out for player's (total) upkeep - mana, gold, food
    applies difficulty level modifiers
    loops for all cities' surplus gold and food
    loops for all heroes' 'Noble' special ((hero)unit) ability
    applies 'excess food' 'gold income'
    sets inouts to (income - expense)

    these are the values that get displayed in the 'Summary Window' on the 'Main Screen'

    SEEALSO:  MoM-Calculations.md, MoM-Calculations-Resources.md
*/
void Player_Resource_Income_Total(int16_t player_idx, int16_t * gold_total, int16_t * food_total, int16_t * mana_total)
{
    int16_t mana_income = 0;
    int16_t food_income = 0;
    int16_t gold_income = 0;
    int16_t mana_expense = 0;
    int16_t food_expense = 0;
    int16_t gold_expense = 0;
    int16_t itr_cities = 0;
    int16_t itr_heroes = 0;
    int16_t difficulty_modifier = 0;  // DNE in Dasm

    // just to get mana_income
    Player_Magic_Power_Income_Total(&mana_income, &food_income, &gold_income, player_idx);

    gold_expense = 0;
    gold_income = 0;  // clear the bogus value we just got from Player_Magic_Power_Income_Total()
    food_expense = 0;
    food_income = 0;  // clear the bogus value we just got from Player_Magic_Power_Income_Total()
    mana_expense = 0;

    if((g_timestop_player_num - 1) == player_idx)
    {
        *food_total = 0;
        *gold_total = 0;
        *mana_total = -200;
        return;
    }

    mana_expense = Player_Armies_And_Enchantments_Mana_Upkeep(player_idx);

    gold_expense = Player_Armies_Gold_Upkeep(player_idx);

    food_expense = Player_Armies_Food_Upkeep(player_idx);

    if(player_idx != HUMAN_PLAYER_IDX)
    {

        // mana_expense = ((difficulty_modifiers_table[_difficulty].maintenance * mana_expense) / 100);
        // gold_expense = ((difficulty_modifiers_table[_difficulty].maintenance * gold_expense) / 100);
        // food_expense = ((difficulty_modifiers_table[_difficulty].maintenance * food_expense) / 100);
        difficulty_modifier = difficulty_modifiers_table[_difficulty].maintenance;
        mana_expense = ((difficulty_modifier * mana_expense) / 100);
        gold_expense = ((difficulty_modifier * gold_expense) / 100);
        food_expense = ((difficulty_modifier * food_expense) / 100);

    }


    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        if(_CITIES[itr_cities].owner_idx == player_idx)
        {

            gold_income += (_CITIES[itr_cities].gold_units - _CITIES[itr_cities].building_maintenance);

            food_income += (_CITIES[itr_cities].food_units - _CITIES[itr_cities].population);

        }

    }

    for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
    {

        if(_players[player_idx].Heroes[itr_heroes].unit_idx > -1)
        {

            if((_HEROES2[player_idx]->heroes[_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].type].abilities & HSA_NOBLE) != 0)
            {

                gold_income += 10;

            }

        }

    }

    *food_total = (food_income - food_expense);

    gold_income += (*food_total > 0) ? (*food_total / 2) : 0;

    *gold_total = (gold_income - gold_expense);

    *mana_total = (mana_income - mana_expense);

}

// WZD o120p08
/*
    Calculate Mana Maintenance Cost
        Unit Enchantments
        City Enchantment
        Overland Enchantments
        -50% for Channeler
*/
int16_t Player_Armies_And_Enchantments_Mana_Upkeep(int16_t player_idx)
{
    int16_t mana_upkeep_cost = 0;
    int16_t itr_units = 0;
    int16_t itr_cities = 0;

    mana_upkeep_cost = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(_UNITS[itr_units].owner_idx == player_idx)
        {

            mana_upkeep_cost += Unit_Mana_Upkeep(itr_units);

        }

    }

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        mana_upkeep_cost += Player_City_Enchantments_Upkeep(itr_cities, player_idx);

    }

    mana_upkeep_cost += Player_Overland_Enchantments_Upkeep(player_idx);

    if(_players[player_idx].channeler > 0)
    {

        mana_upkeep_cost = (mana_upkeep_cost / 2);  /* -50% */

    }

    return mana_upkeep_cost;
    
}

// WZD o120p09
// drake178: UNIT_GetManaUpkeep()
/*

    +50% Undead Fantastic Unit
*/
int16_t Unit_Mana_Upkeep(int16_t unit_idx)
{
    int16_t mana_upkeep;
    int16_t Enchant_Bit;
    uint32_t unit_enchantments;
    int16_t itr_unit_enchantments;

    mana_upkeep = 0;

    unit_enchantments = _UNITS[unit_idx].enchantments;

    if((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_FANTASTIC) != 0)
    {
        if((_UNITS[unit_idx].mutations & UM_UNDEAD) != 0)
        {
            mana_upkeep += ((_unit_type_table[_UNITS[unit_idx].type].Upkeep * 3) / 2);  /* + 50% */
        }
        else
        {
            mana_upkeep += _unit_type_table[_UNITS[unit_idx].type].Upkeep;
        }
    }

    // TODO  ¿ Torin The Chosen has no mana upkeep ?  ¿ why not just get it from the table ?
    if((_UNITS[unit_idx].type & 0x22) == 0)  /* _Chosen */
    {
        mana_upkeep += _unit_type_table[_UNITS[unit_idx].type].Upkeep;
    }

    if(
        (_UNITS[unit_idx].owner_idx < _num_players) &&
        (_players[_UNITS[unit_idx].owner_idx].conjurer > 0)
    )
    {
        mana_upkeep = (((mana_upkeep * 3) + 3) / 4);
    }

    for(itr_unit_enchantments = 0; itr_unit_enchantments < NUM_UNIT_ENCHANTMENTS; itr_unit_enchantments++)
    {
        Enchant_Bit = (unit_enchantments & 0x1);

        if(Enchant_Bit == 1)
        {
            mana_upkeep += unit_enchantment_upkeep_table[itr_unit_enchantments];

            // TODO  sup? different than the logic elsewhere of iterating over the bit placement?
// mov     ax, [bp+Enchants_HI]
// mov     dx, [bp+Enchants_LO]
// sar     ax, 1
// rcr     dx, 1
// mov     [bp+Enchants_HI], ax
// mov     [bp+Enchants_LO], dx
            // Enchants_LO >>= Enchants_LO;
            unit_enchantments = (unit_enchantments >> 1);

        }

    }

    return mana_upkeep;
}

// WZD o120p10
// drake178: CTY_EnchantsUpkeep()
int16_t Player_City_Enchantments_Upkeep(int16_t city_idx, int16_t player_idx)
{
    int16_t mana_upkeep;
    int8_t * city_enchantments;
    int16_t itr_city_enchantments;

    mana_upkeep = 0;

    city_enchantments = _CITIES[city_idx].enchantments;

    // NOTE: the city_enchantment_upkeep_table has the 25 enchantments plus Nightshade
    for(itr_city_enchantments = 0; itr_city_enchantments < NUM_CITY_ENCHANTMENTS; itr_city_enchantments++)
    {
        if(city_enchantments[itr_city_enchantments] == (player_idx + 1))
        {
            mana_upkeep += city_enchantment_upkeep_table[itr_city_enchantments];
        }
    }

    return mana_upkeep;
}

// WZD o120p11
// drake178: WIZ_GlobalsUpkeep()
int16_t Player_Overland_Enchantments_Upkeep(int16_t player_idx)
{
    int16_t mana_upkeep;
    uint8_t * overland_enchantments;
    int16_t itr_overland_enchantments;

    overland_enchantments = _players[player_idx].Globals;

    mana_upkeep = 0;

    for(itr_overland_enchantments = 0; itr_overland_enchantments < NUM_OVERLAND_ENCHANTMENTS; itr_overland_enchantments++)
    {
        if(overland_enchantments[itr_overland_enchantments] > 0)
        {
            mana_upkeep += overland_enchantment_upkeep_table[itr_overland_enchantments];
        }
    }

    return mana_upkeep;
}


// WZD o120p12
// drake178: UU_UNIT_OnSameTile()
// UU_UNIT_OnSameTile()

// WZD o120p13
// drake178: UNIT_MarkRemoved()
/*
; marks a unit as dead or dismissed in the unit table
; (owner -1, plane -1), to be cleared up at the end of
; the turn - type 1 is dismissal, 2 is irreversible
;
; BUG: does not set the hero index in the unit table
; to -1, which is used by this same function to
; determine if the unit is an active hero; if called
; again by accident this will likely corrupt memory
*/
/*
    undefines owner and plane, resets level, sets finished
Kill Type:
    0:                undefines hero items, undefines hero unit index, saves level, experience, name
    1: deletes items, undefines hero items, undefines hero unit index, saves level, experience, name
    2:                undefines hero items, undefines hero unit index, sets level to -20
¿ what does it mean for the level to be -20 ?

AFTER IT’S OVER :: Treasure
Artifacts may be picked up from heroes killed in battle
  (unless the heroes died through some mechanism that completely destroyed or banished them
  including disintegration, banishment, unsummoning and holy word).
...banished or unsummoned... ~disintegrated ...otherwise, manual intermingles "killed" and "destroyed"
Meh. ~ "Perma-Death"
¿ "eradicated" ?

""...any items he or she is carrying disappear with the hero"

"The Chosen" ... can be resummoned.

What does the resurrection spell get up to?

After combat, what does it check for rewarding with items from a killed enemy hero?
    _players[].Heroes[].Items[]
    _ITEMS[].cost != 0
    _ITEMS[].cost != -1

e.g.,
    'Unit Statistics Window' / 'Unit View'
        Remove Enchantment - Flight, Waterwalking, Windwalking
            |-> Dismiss_Unit(unit_idx)
                |-> Kill_Unit(unit_idx, 1)
    'AI Next-Turn'
        AI_Kill_Lame_Units()
            |-> Kill_Unit(troops[itr_troops], 0);

    STK_CaptureCity__WIP()
        |-> Kill_Unit(unit_idx, 0)

    Evict_Weakest_Unit()
        |-> Kill_Unit(unit_idx, 1)

*/
/*
kill_type
    e_KILL_TYPE
        kt_Normal, kt_Dismissed, kt_Disappeared
*/
void Kill_Unit(int16_t unit_idx, int16_t kill_type)
{
    int16_t itr;
    int16_t unit_owner_idx;

    unit_owner_idx = _UNITS[unit_idx].owner_idx;

    _UNITS[unit_idx].Level = (int8_t)Unit_Base_Level(unit_idx);

    // ¿ removal type 1 is "Dismiss" ?
    // ...can be resurected or reincarnated
    if((kill_type == kt_Dismissed) || (_UNITS[unit_idx].type == ut_Chosen))
    {

        UNITS_FINISHED(unit_idx, ST_TRUE);

        if(_UNITS[unit_idx].Hero_Slot > -1)
        {

            for(itr = 0; itr < NUM_HERO_ITEMS; itr++)
            {

                if(_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[itr] > ST_UNDEFINED)
                {

                    Remove_Item(_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[itr]);

                }

                _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[itr] = ST_UNDEFINED;

            }

            _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].unit_idx = ST_UNDEFINED;

            _HEROES2[_UNITS[unit_idx].owner_idx]->heroes[_UNITS[unit_idx].type].Level = _UNITS[unit_idx].Level;  // keeps the level data

            if(_UNITS[unit_idx].owner_idx == HUMAN_PLAYER_IDX)
            {

                hero_names_table[_UNITS[unit_idx].type].experience_points = _UNITS[unit_idx].XP;

                stu_strcpy(hero_names_table[_UNITS[unit_idx].type].name, _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].name);

            }

        }
    }
    else  /* ((kill_type != 1) && (_UNITS[unit_idx].type != ut_Chosen)) */
    {

        UNITS_FINISHED(unit_idx, ST_TRUE);

        if(_UNITS[unit_idx].Hero_Slot > -1)
        {

            for(itr = 0; itr < NUM_HERO_ITEMS; itr++)
            {
                _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[itr] = ST_UNDEFINED;
            }

            _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].unit_idx = ST_UNDEFINED;

            if(kill_type != kt_Disappeared)  /* ¿ must be type 0 kt_Normal ? */
            {

                if(_UNITS[unit_idx].owner_idx == HUMAN_PLAYER_IDX)
                {

                    hero_names_table[_UNITS[unit_idx].type].experience_points = _UNITS[unit_idx].XP;

                    stu_strcpy(hero_names_table[_UNITS[unit_idx].type].name, _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].name);

                }

                _HEROES2[_UNITS[unit_idx].owner_idx]->heroes[_UNITS[unit_idx].type].Level = _UNITS[unit_idx].Level;

            }
            else  /* type 2 */
            {

                _HEROES2[_UNITS[unit_idx].owner_idx]->heroes[_UNITS[unit_idx].type].Level = -20;

            }

        }

    }

    UNITS_OWNER(unit_idx, ST_UNDEFINED);

    UNITS_WP(unit_idx, ST_UNDEFINED);  // ¿ here because `wp = 9` is used for dead combat summon units ?

}


// WZD o120p14
// drake178: UU_BYTE_SortDescnd()
// UU_BYTE_SortDescnd()

// WZD o120p15
// drake178: TILE_GetUnits()
/*
    in_out of count and array of units
    same wx, wy, wp
    no Towers
    not Disbanded/Dead
*/
void Army_At_Square_1(int16_t wx, int16_t wy, int16_t wp, int16_t * troop_count, int16_t troops[])
{
    int16_t itr_units = 0;  // _DX_

    *troop_count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(
            (_UNITS[itr_units].wx == wx)
            &&
            (_UNITS[itr_units].wy == wy)
            &&
            (_UNITS[itr_units].owner_idx != ST_UNDEFINED)
            &&
            (_UNITS[itr_units].wp == wp)
        )
        {

            troops[*troop_count] = itr_units;

            *troop_count += 1;

        }

    }
    
}


// WZD o120p16
// drake178: UU_WIZ_GetReligion()
// UU_WIZ_GetReligion()


// WZD o120p17
/*
    in:
        player_idx
    in-out:
        mana, skill, research
    out:
        N/A
    
    skill > mana > research

    mana, skill, research from Magic Power Distribution
    'Mana Focusing' bonus to mana drawn from magic power reserves for mana reserves
    research points bonus for Heroes with 'Sage' Special Ability

*/
void Player_Magic_Power_Distribution(int16_t * mana_points, int16_t * skill_points, int16_t * research_points, int16_t player_idx)
{
    int16_t city_research_points;
    int16_t research_portion;
    int16_t skill_portion;
    int16_t mana_portion;
    int16_t magic_power;

    int16_t itr_cities;
    int16_t itr_heroes;

    magic_power = _players[player_idx].Power_Base;

    mana_portion     = (((magic_power * _players[player_idx].mana_ratio ) + 50) / 100);
    skill_portion    = (((magic_power * _players[player_idx].skill_ratio) + 50) / 100);
    research_portion = magic_power - mana_portion - skill_portion;

    if( (_players[player_idx].research_ratio == 0) && (research_portion > 0) )
    {
        if(_players[player_idx].skill_ratio == 0)
        {
            mana_portion += research_portion;
        }
        else
        {
            skill_portion += research_portion;
        }
        research_portion = 0;
    }

    if(_players[player_idx].mana_focusing > 0)
    {
        mana_portion += (mana_portion / 4);  /* +25% */
    }

    city_research_points = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            city_research_points += _CITIES[itr_cities].research_units;
        }
    }

    research_portion += city_research_points;

    for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
    {
        if(_players[player_idx].Heroes[itr_heroes].unit_idx > -1)
        {
            if((_HEROES2[player_idx]->heroes[_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].type].abilities & HSA_SAGE) != 0 )
            {
                research_portion += (_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].Level * 3);
            }
            if((_HEROES2[player_idx]->heroes[_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].type].abilities & HSA_SAGE2) != 0)
            {
                research_portion += (_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].Level * 9);
            }
        }
    }

    *mana_points = mana_portion;
    *skill_points = skill_portion;
    *research_points = research_portion;

}

// WZD o120p18
// drake178: WIZ_GetPwrIncomes()
/*
Get_Power_Incomes()
XREF:
j_Get_Power_Incomes()
Player_Resource_Income_Total()

j_Get_Power_Incomes()
XREF:
    Magic_Screen_Draw()
    WIZ_PowerIncomes()
    AI_Research_Picker()

Player_Resource_Income_Total()
XREF:
    Main_Screen()
    Main_Screen_Draw_Summary_Window()
    AI_Tax_And_Farmers()
    AI_Spell_Select()
    AI_Balance_Upkeep()
*/
/*
    these values are what gets displayed on the 'Magic Screen' as "MP", "RP", "SP"
    the mana value is also what gets displayed on the 'Main Screen' as "Mana Income"

    NOT *base* spell research, because it includes the research bonus, given the specific spell and special abilities
*/
void Player_Magic_Power_Income_Total(int16_t * mana_total, int16_t * research_total, int16_t * skill_total, int16_t player_idx)
{
    int16_t skill_income;
    int16_t research_income;
    int16_t mana_income;
    int16_t spell_research_bonus;

    if(_players[player_idx].casting_spell_idx == 214 /* Spell_Of_Return */)
    {
        *mana_total = 0;
        *research_total = 0;
        *skill_total = 0;
    }
    else
    {
        Player_Magic_Power_Distribution(&mana_income, &skill_income, &research_income, player_idx);

        spell_research_bonus = Player_Spell_Research_Bonus(player_idx, _players[player_idx].researching_spell_idx);

        SETMIN(research_income, 0);
        SETMIN(mana_income, 0);
        SETMIN(skill_income, 0);

        research_income += ((research_income * spell_research_bonus) / 100);

        *mana_total = mana_income;
        *research_total = research_income;
        *skill_total = skill_income;
    }
}

// WZD o120p19
int16_t Player_Spell_Research_Bonus(int16_t player_idx, int16_t spell_idx)
{
    int16_t magic_realm;
    int16_t research_bonus_percentage;

    research_bonus_percentage = 0;

    magic_realm = spell_data_table[spell_idx].magic_realm;

    if(_players[player_idx].sage_master > 0)
    {
        research_bonus_percentage += 25;
    }

    if((_players[player_idx].conjurer > 0) && (spell_data_table[spell_idx].type == scc_Summoning))
    {
        research_bonus_percentage += 25;
    }

    switch(magic_realm)
    {
        case 0:  /* Nature */
        {
            if(_players[player_idx].nature_mastery > 0)
            {
                research_bonus_percentage += 15;
            }
            if(_players[player_idx].spellranks[sbr_Nature] > 7)
            {
                research_bonus_percentage += ((_players[player_idx].spellranks[sbr_Nature] - 7) * 10);
            }
        } break;
        case 1:  /* Sorcery */
        {
            if(_players[player_idx].sorcery_mastery > 0)
            {
                research_bonus_percentage += 15;
            }
            if(_players[player_idx].spellranks[sbr_Sorcery] > 7)
            {
                research_bonus_percentage += ((_players[player_idx].spellranks[sbr_Sorcery] - 7) * 10);
            }
        } break;
        case 2:  /* Chaos */
        {
            if(_players[player_idx].chaos_mastery > 0)
            {
                research_bonus_percentage += 15;
            }
            if(_players[player_idx].spellranks[sbr_Chaos] > 7)
            {
                research_bonus_percentage += ((_players[player_idx].spellranks[sbr_Chaos] - 7) * 10);
            }
        } break;
        case 3:  /* Life */
        {
            if(_players[player_idx].spellranks[sbr_Life] > 7)
            {
                research_bonus_percentage += ((_players[player_idx].spellranks[sbr_Life] - 7) * 10);
            }
        } break;
        case 4:  /* Death */
        {
            if(_players[player_idx].spellranks[sbr_Death] > 7)
            {
                research_bonus_percentage += ((_players[player_idx].spellranks[sbr_Death] - 7) * 10);
            }
        } break;
        case 5:  /* Arcane */
        {
            if(_players[player_idx].runemaster > 0)
            {
                research_bonus_percentage += 25;
            }
        } break;
    }

    return research_bonus_percentage;
}

// WZD o120p20
// drake178: WIZ_GetFame()
/*
    Calculate Fame Points
        Hero - Legendary
        Hero - Super Legendary
        Global Enchantment - Just Cause
        Wizard's Fame Points
        NOT 'Famous'
*/
int16_t Player_Fame(int16_t player_idx)
{
    int16_t hero_unit_idx;
    int16_t fame_points;
    int16_t itr_heroes;

    int16_t hero_unit_type;  // DNE in Dasm

    fame_points = 0;

    for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
    {
        if(_players[player_idx].Heroes[itr_heroes].unit_idx > -1)
        {
            hero_unit_idx = _players[player_idx].Heroes[itr_heroes].unit_idx;
            hero_unit_type = _UNITS[hero_unit_idx].type;
// TODO              // p_heroes + (player_idx * 35 hero structs) + (type * sizeof s_HERO) + offset in struct to Abilities_LO
// TODO              if( ((((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].type * sizeof(struct s_HERO)) ))->Abilities_LO) & 0x08) != 0 )  /* Ab_Legendary */
// TODO              {
// TODO                  fame_points += ((_UNITS[hero_unit_idx].Level + 1) * 3);
// TODO              }
// TODO              if( ((((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].type * sizeof(struct s_HERO)) ))->Abilities_LO) & 0x10) != 0 )  /* Ab_Legendary2 */
// TODO              {
// TODO                  fame_points += (((_UNITS[hero_unit_idx].Level + 1) * 9) / 2);
// TODO              }
        }
    }

// mov     ax, _DI_player_idx
// mov     dx, 4C8h
// imul    dx
// mov     bx, ax
// cmp     [_players.Globals.Just_Cause+bx], 0
// ...
// cmp     [byte ptr bx-5CA2h], 0
// ...
// ~== ((address of _players + (offset to .Globals.Just_Cause)) + (player_idx * sizeof(s_WIZARD)
    // if(_players[player_idx].Globals.Just_Cause > 0)
    // if(_players[player_idx].Globals[0x12] > 0)  /* Just_Cause */
    if(_players[player_idx].Globals[JUST_CAUSE] > 0)  /* Just_Cause */
    {
        fame_points += 10;
    }

    fame_points += _players[player_idx].fame;

    return fame_points;
}


// WZD o120p21
// MoO2  Module: OFFICER  Hero_Slot_Open_()
/*

*/
int16_t Hero_Slot_Open(int16_t player_idx)
{
    int16_t hero_slot_idx;  // _SI_
    int16_t itr;  // _CX_

    hero_slot_idx = ST_UNDEFINED;

    for(itr = 0; itr < NUM_HERO_SLOTS; itr++)
    {
        if(_players[player_idx].Heroes[itr].unit_idx == ST_UNDEFINED)
        {
            hero_slot_idx = itr;
            break;
        }
    }

    return hero_slot_idx;
}


// WZD o120p22
// drake178: CTY_GetGoldUpkeep()
int16_t City_Gold_Mainanence(int16_t city_idx)
{
    int16_t gold_units = 0;
    int16_t itr = 0;

    TRACE_ENTER();

    gold_units = 0;

    for(itr = 0; itr < NUM_BUILDINGS; itr++)
    {
        if(
            (_CITIES[city_idx].bldg_status[itr] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[itr] == bs_Replaced)
        )
        {
            if(bldg_data_table[itr].maintenance_cost != 0)
            {
                gold_units += abs(bldg_data_table[itr].maintenance_cost);
            }
        }
    }

    TRACE_EXIT();

    return gold_units;
}


// WZD o120p23
// MoO2  Module: ERICNET  Calc_Ship_Level_()
// ~ Unit_Base_Level()?
int16_t Calc_Unit_Level(int16_t unit_idx)
{
    int16_t itr = 0;
    int16_t level = 0;
    level = _UNITS[unit_idx].Level;
    if(_UNITS[unit_idx].type < spell_data_table[spl_Magic_Spirit].unit_type)  /* First Fantastic Unit (Magic Spirit) */
    {
        for(itr = 0; itr < 4; itr++)
        {
            if(_UNITS[unit_idx].XP >= TBL_Experience[itr])
            {
                level = itr;
            }
        }
        if(_UNITS[unit_idx].Hero_Slot > -1)
        {
            for(itr = 4; itr < 9; itr++)
            {
                if(_UNITS[unit_idx].XP >= TBL_Experience[itr])
                {
                    level = itr;
                }
            }
            SETMAX(_UNITS[unit_idx].XP, TBL_Experience[HL_DEMIGOD]);
        }
        else
        {
            SETMAX(_UNITS[unit_idx].XP, TBL_Experience[UL_ELITE]);
        }
        // ¿ cancel 'Heroism' 'Unit Enchantment' if the Unit has naturally attained the 'Elite Experience Level' ?
        if(
            (level >= UL_ELITE)
            &&
            ((_UNITS[unit_idx].enchantments & UE_HEROISM) != 0)
        )
        {
            _UNITS[unit_idx].enchantments = (_UNITS[unit_idx].enchantments ^ UE_HEROISM);
        }
        // apply 'Heroism' 'Unit Enchantment'
        if(
            ((_UNITS[unit_idx].enchantments & UE_HEROISM) != 0)
            &&
            (level < UL_ELITE)
        )
        {
            level = UL_ELITE;
        }
        if(_players[_UNITS[unit_idx].owner_idx].warlord > 0)
        {
            level += 1;
        }
        if(
            (_players[_UNITS[unit_idx].owner_idx].Globals[CRUSADE] > 0)
            &&
            (_UNITS[unit_idx].type < FST_FANT) &&
            ((_UNITS[unit_idx].mutations & UM_UNDEAD) == 0)
        )
        {
            level += 1;
        }
    }
    SETMAX(level, UNIT_LEVEL_MAX);
    return level;
}


// WZD o120p24
int16_t Unit_Base_Level(int16_t unit_idx)
{
    int16_t level = 0;
    int16_t itr = 0;
    level = _UNITS[unit_idx].Level;
    if(_UNITS[unit_idx].type < spell_data_table[spl_Magic_Spirit].unit_type)
    {
        for(itr = 0; itr < 4; itr++)
        {
            if(_UNITS[unit_idx].XP >= TBL_Experience[itr])
            {
                level = itr;
            }
        }
        if(_UNITS[unit_idx].Hero_Slot <= -1)
        {
            if(_UNITS[unit_idx].XP >= TBL_Experience[UL_ELITE])
            {
                _UNITS[unit_idx].XP = TBL_Experience[UL_ELITE];
            }
        }
        else
        {
            for(itr = 4; itr < 9; itr++)
            {
                if(_UNITS[unit_idx].XP >= TBL_Experience[itr])
                {
                    level = itr;
                }
            }
            if(_UNITS[unit_idx].XP >= TBL_Experience[HL_DEMIGOD])
            {
                _UNITS[unit_idx].XP = TBL_Experience[HL_DEMIGOD];
            }
        }
    }
    if(level > UNIT_LEVEL_MAX)
    {
        level = UNIT_LEVEL_MAX;
    }
    return level;
}


// WZD o120p25
void Evict_Unit_With_Message(int16_t unit_idx)
{
    int16_t owner_was_human_player = 0;
    if(_UNITS[unit_idx].owner_idx != HUMAN_PLAYER_IDX)
    {
        owner_was_human_player = ST_FALSE;
    }
    else
    {
        owner_was_human_player = ST_TRUE;
    }
    Evict_Unit(unit_idx);
    if(
        (_UNITS[unit_idx].owner_idx == ST_UNDEFINED)
        &&
        (owner_was_human_player == ST_TRUE)
        &&
        (msg_unit_lost_ctr < 20)
    )
    {
        msg_unit_lost[msg_unit_lost_ctr].Unit_Type = _UNITS[unit_idx].type;
        msg_unit_lost[msg_unit_lost_ctr].Cause = 3;
        msg_unit_lost_ctr++;
    }
}


// WZD o120p26
/**
 * @brief Resolves stack overflow on a square by evicting the weakest unit.
 *
 * @details
 * This helper enforces the maximum stack size after unit placement/movement by
 * inspecting all units on the acting unit's square. If the square contains more than
 * @c MAX_STACK units, the function computes a simple "weakness" score for each
 * unit and removes one candidate.
 *
 * Selection algorithm:
 * 1. Read the acting unit's coordinates (@c wx/@c wy/@c wp).
 * 2. Gather all units on that square via @c Army_At_Square_1().
 * 3. If troop count exceeds @c MAX_STACK, compute score per unit as:
 *    @code
 *    Unit_Gold_Upkeep(unit) + Unit_Mana_Upkeep(unit) + unit_level
 *    @endcode
 * 4. Choose the lowest-scoring unit as the eviction target.
 *    Ties are resolved in favor of the later-iterated unit due to
 *    @c <= comparison in the selection check.
 * 5. Remove the selected unit:
 *    - Non-neutral owner: push off square with logging via @c Evict_Unit_With_Message().
 *    - Neutral owner: dismiss immediately via @c Kill_Unit(..., kt_Dismissed).
 *
 * @param unit_idx Index of the reference unit whose current square is checked for
 *                 stack overflow.
 *
 * @return This function returns no value.
 *
 * @note The temporary troop list is sized to 10 entries, while the stack cap is
 *       defined by @c MAX_STACK.
 * @note The selection metric is a heuristic and does not directly account for
 *       combat abilities beyond upkeep and current level.
 * @note Neutral and non-neutral removals use different paths, which affects
 *       downstream reporting/logging behavior.
 *
 * @see Army_At_Square_1(), Evict_Unit_With_Message(), Kill_Unit(), Unit_Gold_Upkeep(), Unit_Mana_Upkeep()
 */
void Evict_Weakest_Unit(int16_t unit_idx)
{
    int16_t troops[10] = { 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB };
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t troop_count = 0;
    int16_t lowest_trooper_idx = 0;
    int16_t trooper_value = 0;
    int16_t lowest_trooper_value = 0;
    int16_t itr_troops = 0;
    lowest_trooper_idx   = -999;
    lowest_trooper_value =  999;
    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_wp = _UNITS[unit_idx].wp;
    Army_At_Square_1(UNITX(), UNITY(), UNITP(), &troop_count, &troops[0]);
    if(troop_count > MAX_STACK)
    {
        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
        {
            trooper_value = Unit_Gold_Upkeep(troops[itr_troops]) + Unit_Mana_Upkeep(troops[itr_troops]);
            trooper_value += _UNITS[troops[itr_troops]].Level;
            if(trooper_value <= lowest_trooper_value)
            {
                lowest_trooper_value = trooper_value;
                lowest_trooper_idx = troops[itr_troops];
            }
        }
        if(_UNITS[lowest_trooper_idx].owner_idx != NEUTRAL_PLAYER_IDX)
        {
            Evict_Unit_With_Message(lowest_trooper_idx);
        }
        else
        {
            Kill_Unit(lowest_trooper_idx, kt_Dismissed);
        }
    }
}



/*
    WIZARDS.EXE  ovr142
*/

// WZD o142p01
// drake178: TILE_CreateRoad()
/*
; creates a road on the square, setting the corresponding
; movement allowances, and removing the enchanted road
; flag if set, but then also enchanting the road if the
; square is on Myrror
*/
void Make_Road(int16_t wx, int16_t wy, int16_t wp)
{

    EMMDATAH_Map();  // ; maps the EMM Data block into the page frame

    movement_mode_cost_maps[wp].UU_MvMd.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
    movement_mode_cost_maps[wp].walking.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
    movement_mode_cost_maps[wp].forester.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
    movement_mode_cost_maps[wp].mountaineer.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
    movement_mode_cost_maps[wp].swimming.moves2[((wy * WORLD_WIDTH) + wx)] = 1;

    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] |= MSF_ROAD;

    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] &= ~(MSF_EROAD);

    if(wp == MYRROR_PLANE)
    {
        Make_Road_Enchanted(wx, wy, wp);
    }

}


// WZD o142p02
// drake178: TILE_EnchantRoad()
/*
; changes a regular road on a square to an enchanted one,
; or does nothing if there is no road on the square
*/
/*
    changes a normal road to an enchant road

XREF:
    Make_Road()
    j_Make_Road_Enchanted()
        Cast_Enchant_Road()
*/
void Make_Road_Enchanted(int16_t wx, int16_t wy, int16_t wp)
{

    EMMDATAH_Map();

    if((_map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] & MSF_ROAD) != 0)
    {

        _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] |= MSF_EROAD;

        movement_mode_cost_maps[wp].UU_MvMd.moves2[((wy * WORLD_WIDTH) + wx)] = 0;

        movement_mode_cost_maps[wp].walking.moves2[((wy * WORLD_WIDTH) + wx)] = 0;

        movement_mode_cost_maps[wp].forester.moves2[((wy * WORLD_WIDTH) + wx)] = 0;

        movement_mode_cost_maps[wp].mountaineer.moves2[((wy * WORLD_WIDTH) + wx)] = 0;

        movement_mode_cost_maps[wp].swimming.moves2[((wy * WORLD_WIDTH) + wx)] = 0;
        
        // NOT  movement_mode_cost_maps[wp].swimming.moves2[((wy * WORLD_WIDTH) + wx)] = 0;
        
    }

}


// WZD o142p03
// drake178: LD_TILE_ResUpdate()
/*
; Legacy Development function, can be removed
;
; would update some aspect of some resource on a square by square basis with no return value
*/
void o142p03_empty_function(int16_t wx, int16_t wy, int16_t wp)
{
    return;
}

// WZD o142p04
// LD_CTY_ResRefresh()
/*
; Legacy Development function, can be removed
;
; would update some aspect of some resource on every
; square of every catchment area (including corners)
*/
void NOOP_Current_Player_All_City_Areas(void)
{
    int16_t Max_Distance;
    int16_t city_area_square_wy;
    int16_t city_area_x;
    int16_t city_wp;
    int16_t city_wy;
    int16_t city_wx;
    int16_t itr_cities;  // _SI_
    int16_t city_area_square_wx;  // _DI_

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
        {
            city_wx = _CITIES[itr_cities].wx;
            city_wy = _CITIES[itr_cities].wy;
            city_wp = _CITIES[itr_cities].wp;
            if(_CITIES[itr_cities].population >= 8)
            {
                // jmp     short $+2
            }
            Max_Distance = 2;
            
            for((city_area_square_wy = (city_wy - Max_Distance)); ((city_wy + Max_Distance + 1) > city_area_square_wy); city_area_square_wy++)
            {
                if(
                    (city_area_square_wy >= 0)
                    &&
                    (city_area_square_wy < WORLD_HEIGHT)
                )
                {
                    for((city_area_x = (city_wx - Max_Distance));  ((city_wx + Max_Distance + 1) > city_area_x); city_area_x++)
                    {
                        city_area_square_wx = city_area_x;
                        if(city_area_square_wx < 0)
                        {
                            city_area_square_wx += WORLD_WIDTH;
                        }
                        if(city_area_square_wx >= WORLD_WIDTH)
                        {
                            city_area_square_wx = WORLD_WIDTH;
                        }
                        o142p03_empty_function(city_area_square_wx, city_area_square_wy, city_wp);
                    }
                }
            }
        }
    }
}

// WZD o142p05
int16_t City_House_Count(int16_t city_idx)
{
    return _CITIES[city_idx].Pop_10s;
}

// WZD o142p06
int16_t City_Food_Terrain(int16_t city_idx)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t city_wp = 0;
    int16_t useable_map_squares = 0;
    int16_t food_units_x2 = 0;
    int16_t itr = 0;
    city_wp = _CITIES[city_idx].wp;
    // NOTE: Accounts for 'Corruption'
    useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);
    food_units_x2 = 0;
    for(itr = 0; itr < useable_map_squares; itr++)
    {
        food_units_x2 += Square_Food_x2(wx_array[itr], wy_array[itr], city_wp);  /* returns ('food units' * 2) */
    }
    if(_CITIES[city_idx].enchantments[GAIAS_BLESSING] > ST_FALSE)
    {
        food_units_x2 = ((food_units_x2 * 3) / 2);  /* +50% */
    }
    return (food_units_x2 / 2 / 2);  /* food_units_x2 -> food_units -> food2_units */
}


// WZD o142p07
/**
 * @brief Computes additional food provided by Wild Game specials in a city's usable area.
 *
 * @details
 * Scans the city's non-corrupted catchment squares and sums food contributed by
 * the @c TS_WILDGAME terrain special. Each qualifying square contributes either
 * full or reduced value depending on whether the catchment square is shared with
 * another city's area.
 *
 * Processing summary:
 * 1. Resolve the city's world plane.
 * 2. Build the usable catchment coordinate list with @c Get_Useable_City_Area().
 * 3. For each usable square, test @c GET_TERRAIN_SPECIAL(... ) & TS_WILDGAME.
 * 4. Convert the square coordinates to a shared-bitfield index.
 * 5. Add food contribution:
 *    - Unshared square: +2
 *    - Shared square: +1
 *
 * @param city_idx Index of the city whose Wild Game food bonus is being summed.
 *
 * @return Total Wild Game food bonus for the city, in whole food units.
 *
 * @note This routine uses the same shared-area bitfield queried by
 *       @c City_Area_Square_Is_Shared(), but reads it directly via
 *       @c Test_Bit_Field().
 * @note Only squares returned by @c Get_Useable_City_Area() are considered, so
 *       corrupted catchment squares do not contribute.
 * @note The return value is consumed directly by city food and maximum-size
 *       calculations as an additive bonus.
 *
 * @see Get_Useable_City_Area(), City_Area_Square_Is_Shared(), City_Food_Production(),
 *      City_Maximum_Size()
 */
int16_t City_Food_WildGame(int16_t city_idx)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t bit_index = 0;
    int16_t city_wp = 0;
    int16_t useable_map_squares = 0;
    int16_t food_units = 0;
    int16_t itr = 0;
    uint8_t * bit_field = 0;
    city_wp = _CITIES[city_idx].wp;
    useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);
    food_units = 0;
    for(itr = 0; itr < useable_map_squares; itr++)
    {
        if((GET_TERRAIN_SPECIAL(wx_array[itr], wy_array[itr], city_wp) & TS_WILDGAME) != 0)
        {
            bit_index = ((wy_array[itr] * WORLD_WIDTH) + wx_array[itr]);
            bit_field = (city_area_shared_bits + (city_wp * WORLD_SIZE) );
            /* ~== City_Area_Square_Is_Shared() */
            if(Test_Bit_Field(bit_index, bit_field) == ST_FALSE)
            {
                food_units += 2;
            }
            else
            {
                food_units += 1;
            }
        }
    }
    return food_units;
}


// WZD o142p08
/**
 * @brief Collects usable city catchment squares, excluding corrupted squares.
 *
 * @details
 * Builds the coordinate list for the city's standard work area centered at
 * (city_wx, city_wy): a 5x5 footprint with trimmed corners on the top and
 * bottom rows (x offsets -1..+1 when y offset is -2 or +2).
 *
 * Vertical bounds are clamped by skipping rows outside the world height.
 * Horizontal coordinates wrap around world edges. Each candidate square is
 * checked against map-square flags for MSF_CORRUPTION; only non-corrupted
 * squares are emitted into the output arrays.
 *
 * @param city_wx City center world X coordinate.
 * @param city_wy City center world Y coordinate.
 * @param city_wp World plane index containing the city.
 * @param wx_array Output array for accepted square X coordinates.
 * @param wy_array Output array for accepted square Y coordinates.
 *
 * @return Number of usable catchment squares written to the output arrays.
 *
 * @note Maximum output is bounded by CITY_AREA_SIZE for the catchment shape.
 * @note X coordinates wrap at both world edges (square_x < 0 -> +WORLD_WIDTH;
 *       square_x >= WORLD_WIDTH -> -WORLD_WIDTH). Y rows outside the world are
 *       dropped, not wrapped.
 */
int16_t Get_Useable_City_Area(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t *wx_array, int16_t *wy_array)
{
    uint8_t * terrain_flags_table_row = 0;
    int16_t square_y = 0;
    int16_t itr_world_x = 0;
    int16_t square_x_max = 0;
    int16_t square_x_min = 0;
    int16_t map_square_count = 0;
    int16_t itr_city_area_squares = 0;
    int16_t square_x = 0;
    TRACE_ENTER();
    map_square_count = 0;
    for(itr_city_area_squares = -2; itr_city_area_squares <= 2; itr_city_area_squares++)
    {
        square_y = (city_wy + itr_city_area_squares);
        if(
            (square_y >= 0)
            &&
            (square_y < WORLD_HEIGHT)
        )
        {
            square_x_min = -2;
            square_x_max =  2;
            if(
                (itr_city_area_squares == -2)
                ||
                (itr_city_area_squares ==  2)
            )
            {
                square_x_min = -1;
                square_x_max =  1;
            }
            terrain_flags_table_row = (uint8_t *)&_map_square_flags[(city_wp * WORLD_SIZE) + (square_y * WORLD_WIDTH)];
            itr_world_x = square_x_min;
            while(itr_world_x <= square_x_max)
            {
                square_x = city_wx + itr_world_x;
                if(square_x < 0)
                {
                    square_x += WORLD_WIDTH;
                }
                if(square_x >= WORLD_WIDTH)
                {
                    square_x -= WORLD_WIDTH;
                }
                if((*(terrain_flags_table_row + square_x) & MSF_CORRUPTION) == 0)
                {
                    wx_array[map_square_count] = square_x;
                    wy_array[map_square_count] = square_y;
                    map_square_count++;
                }
                itr_world_x++;
            }
        }
    }
    TRACE_EXIT();
    return map_square_count;
}


// WZD o142p09
int16_t City_Food_Production(int16_t city_idx)
{
    int16_t city_area_food_units = 0;
    int16_t food_units = 0;

    TRACE_ENTER();

    if(_CITIES[city_idx].population == 0)
    {

        food_units = 0;

    }
    else
    {

        city_area_food_units = City_Food_Terrain(city_idx);

        // ¿ someone died ?
        if(_CITIES[city_idx].farmer_count > _CITIES[city_idx].population)
        {

            CITIES_FARMER_COUNT(city_idx, _CITIES[city_idx].population);

        }

        if(
            (_CITIES[city_idx].race == rt_Halfling)
            ||
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Replaced)
        )
        {

            food_units = _CITIES[city_idx].farmer_count * 3;

        }
        else
        {

            food_units = _CITIES[city_idx].farmer_count * 2;

        }

        /*
            Forester's Guild
            Famine
            Over-Farming Adjustment
            Granary
            Farmer's Market
            Wild Game
        */

        if(
            (_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Replaced)
        )
        {

            food_units += 2;

        }

        // if(_CITIES[city_idx].enchantments[FAMINE] != ST_FALSE)
        if(_CITIES[city_idx].enchantments[FAMINE] > ST_FALSE)  /* CLAUDE */  /* REBUG */  /* asm: cmp FAMINE, e_ST_FALSE / jle */
        {

            food_units = food_units / 2;

        }

        // Over-Farming Adjustment
        if(food_units > city_area_food_units)
        {

            food_units = (city_area_food_units + ((food_units - city_area_food_units) / 2));

        }

        if(
            (_CITIES[city_idx].bldg_status[GRANARY] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[GRANARY] == bs_Replaced)
        )
        {

            food_units += 2;

        }

        if(
            (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Replaced)
        )
        {

            food_units += 3;

        }

        food_units += City_Food_WildGame(city_idx);

    }

    TRACE_EXIT();

    return food_units;
}


// WZD o142p10
/*
vs. MAPGEN  City_Maximum_Size_NewGame()?
*/
/**
 * @brief Calculates the city's maximum sustainable population from food sources.
 *
 * @details
 * Starts from terrain-derived food capacity and then applies a small set of
 * modifiers that affect how large the city can grow before food limits stop
 * further expansion.
 *
 * Calculation steps:
 * 1. Begin with @c City_Food_Terrain(city_idx).
 * 2. Halve the value if the city is under @c FAMINE.
 * 3. Add +2 if @c GRANARY is built or replaced.
 * 4. Add +3 if @c FARMERS_MARKET is built or replaced.
 * 5. Add Wild Game bonus from @c City_Food_WildGame(city_idx).
 *
 * @param city_idx Index of the city whose maximum size is being evaluated.
 *
 * @return Estimated maximum city population size as used by growth logic.
 *
 * @note This function is consumed by city growth calculations rather than
 *       directly enforcing a hard cap on city population.
 * @note (OGBUG) The routine does not include a @c FORESTERS_GUILD contribution.
 * @note (OGBUG) The final value is returned directly; the in-code note indicates
 *       it may historically have intended an additional division by 2.
 *
 * @see City_Food_Terrain(), City_Food_WildGame(), City_Growth_Rate(),
 *      Compute_Base_Values_For_Map_Square(), City_Maximum_Size_NewGame()
 */
int16_t City_Maximum_Size(int16_t city_idx)
{
    int16_t maximum_size = 0;
    maximum_size = City_Food_Terrain(city_idx);
    if(_CITIES[city_idx].enchantments[FAMINE] > 0)
    {
        maximum_size = (maximum_size / 2);
    }
    if(
        (_CITIES[city_idx].bldg_status[GRANARY] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[GRANARY] == bs_Replaced)
    )
    {
        maximum_size += 2;
    }
    if(
        (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Replaced)
    )
    {
        maximum_size += 3;
    }
    /* OGBUG: ¿ no FORESTERS_GUILD ? */
    maximum_size += City_Food_WildGame(city_idx);
    return maximum_size;  /* OGBUG: be `/ 2` - 2 food === 1 pop */
}


// WZD o142p11
// drake178: CTY_GetProd()
int16_t City_Production_Production(int16_t city_idx)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t production2_per_worker = 0;
    int16_t have_gaias_blessing = 0;
    int16_t city_wp = 0;
    int16_t city_owner_idx = 0;
    int16_t production_units = 0;
    int16_t worker_count = 0;
    int16_t itr = 0;
    int16_t useable_map_squares = 0;
    int16_t production_modifier = 0;

    TRACE_ENTER();

    if(_CITIES[city_idx].population == 0)
    {
        production_units = 0;
    }
    else
    {
        city_wp = _CITIES[city_idx].wp;

        useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

        // ¿ someone died ?
        if(_CITIES[city_idx].farmer_count > _CITIES[city_idx].population)
        {

            CITIES_FARMER_COUNT(city_idx, _CITIES[city_idx].population);

        }

        worker_count = (_CITIES[city_idx].population - _CITIES[city_idx].farmer_count - City_Rebel_Count(city_idx));

        if(
            (_CITIES[city_idx].race == rt_Dwarf)
            ||
            (_CITIES[city_idx].race == rt_Klackon)
        )
        {

            production2_per_worker = 6;

        }
        else
        {

            production2_per_worker = 4;
            
        }

        production_units = ((((worker_count * production2_per_worker) + _CITIES[city_idx].farmer_count) + 1) / 2);

        if(_CITIES[city_idx].enchantments[GAIAS_BLESSING] != ST_FALSE)
        {
            have_gaias_blessing = ST_TRUE;
        }
        else
        {
            have_gaias_blessing = ST_FALSE;
        }

        production_modifier = 100;

        for(itr = 0; itr < useable_map_squares; itr++)
        {
            production_modifier += Square_Production_Bonus(wx_array[itr], wy_array[itr], city_wp, have_gaias_blessing);
        }

        if(
            (_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Replaced)
        )
        {
            production_modifier += 25;
        }

        if(
            (_CITIES[city_idx].bldg_status[SAWMILL] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[SAWMILL] == bs_Replaced)
        )
        {
            production_modifier += 25;
        }

        if(
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] == bs_Replaced)
        )
        {
            production_modifier += 50;
        }

        if(
            (_CITIES[city_idx].bldg_status[MECHANICIANS_GUILD] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[MECHANICIANS_GUILD] == bs_Replaced)
        )
        {
            production_modifier += 50;
        }

        if(_CITIES[city_idx].enchantments[INSPIRATIONS] != ST_FALSE)
        {
            production_modifier += 100;
        }

        production_units = ((production_units * production_modifier) / 100);

        if(_CITIES[city_idx].enchantments[CURSED_LANDS] > 0)
        {
            production_units = ((production_units + 1) / 2);
        }

    }

    TRACE_EXIT();

    return production_units;
}



// WZD o142p12
int16_t City_Gold_Production(int16_t city_idx)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t gold_modifier = 0;
    int16_t are_dwarf = 0;
    int16_t have_miners_guild = 0;
    int16_t city_wp = 0;
    int16_t city_owner_idx = 0;
    int16_t itr = 0;
    int16_t useable_map_squares = 0;
    int16_t gold_units = 0;

    TRACE_ENTER();

    if(_CITIES[city_idx].population == 0)
    {
        gold_units = 0;
    }
    else
    {
        city_owner_idx = _CITIES[city_idx].owner_idx;

        city_wp = _CITIES[city_idx].wp;

        useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

        gold_modifier = Square_Gold_Bonus(CITYX(), CITYY(), CITYP()) + City_Road_Trade_Bonus(city_idx);

        if(_CITIES[city_idx].race == 0x0B /* R_Nomad */)
        {
            gold_modifier += 50;
        }

        // Maximum Trade Bonus  (Shore/River/River-Mouth + Road Connection + Nomad Race)
        if( (_CITIES[city_idx].population * 3) < gold_modifier)
        {
            gold_modifier = (_CITIES[city_idx].population * 3);
        }

        if(
            (_CITIES[city_idx].bldg_status[MERCHANTS_GUILD] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[MERCHANTS_GUILD] == bs_Replaced)
        )
        {
            gold_modifier += 100;
        }

        if(
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] == bs_Replaced)
        )
        {
            have_miners_guild = ST_TRUE;
        }
        else
        {
            have_miners_guild = ST_FALSE;
        }

        if(
            (_CITIES[city_idx].bldg_status[BANK] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[BANK] == bs_Replaced)
        )
        {
            gold_modifier += 50;
        }

        if(
            (_CITIES[city_idx].bldg_status[MARKETPLACE] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[MARKETPLACE] == bs_Replaced)
        )
        {
            gold_modifier += 50;
        }

        if(_CITIES[city_idx].enchantments[PROSPERITY] != ST_FALSE)
        {
            gold_modifier += 100;
        }

        if(city_owner_idx != NEUTRAL_PLAYER_IDX)
        {

            gold_units = (((_CITIES[city_idx].population - City_Rebel_Count(city_idx)) * _players[city_owner_idx].tax_rate) / 2);

        }
        else
        {

            gold_units = (_CITIES[city_idx].population - City_Rebel_Count(city_idx));

        }
        
        if(_CITIES[city_idx].race != rt_Dwarf)
        {
            are_dwarf = ST_FALSE;
        }
        else
        {
            gold_units += gold_units;
            are_dwarf = ST_TRUE;
        }

        for(itr = 0; itr < useable_map_squares; itr++)
        {

            gold_units += Square_Gold_Income(wx_array[itr], wy_array[itr], city_wp, have_miners_guild, are_dwarf);

        }

        // not sure why this is adding 100 to the modifier, instead of just doing +=
        gold_units = ((gold_units * (gold_modifier + 100)) / 100);

        if(_CITIES[city_idx].construction == bt_TradeGoods)
        {
            gold_units += (_CITIES[city_idx].production_units / 2);
        }

        if(gold_units > 255)
        {
            gold_units = 255;
        }

    }

    TRACE_EXIT();

    return gold_units;
}


// WZD o142p13
// drake178: CTY_GetResearch()
int16_t City_Research_Production(int16_t city_idx)
{
    int16_t city_owner_idx = 0;
    int16_t research_units = 0;

    TRACE_ENTER();

    city_owner_idx = _CITIES[city_idx].owner_idx;

    if(
        (_CITIES[city_idx].population != 0) &&
        (city_owner_idx != NEUTRAL_PLAYER_IDX)
    )
    {

        research_units = 0;

        if(
            (_CITIES[city_idx].bldg_status[LIBRARY] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[LIBRARY] == bs_Replaced)
        )
        {
            research_units += 2;
        }

        if(
            (_CITIES[city_idx].bldg_status[SAGES_GUILD] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[SAGES_GUILD] == bs_Replaced)
        )
        {
            research_units += 3;
        }

        if(
            (_CITIES[city_idx].bldg_status[UNIVERSITY] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[UNIVERSITY] == bs_Replaced)
        )
        {
            research_units += 5;
        }

        if(
            (_CITIES[city_idx].bldg_status[WIZARDS_GUILD] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[WIZARDS_GUILD] == bs_Replaced     /* B_Replaced */)
        )
        {
            research_units += 8;
        }

    }
    else
    {
        research_units = 0;
    }

    TRACE_EXIT();

    return research_units;
}


// WZD o142p14
// drake178: CTY_GetPower()
int16_t City_Mana_Production(int16_t city_idx)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t spell_ranks = 0;
    int16_t have_miners_guild = 0;
    int16_t are_dwarf = 0;
    int16_t city_wp = 0;
    int16_t useable_map_squares = 0;
    int16_t building_magic_power = 0;
    int16_t building_magic_power_modifier = 0;
    int16_t city_owner_idx = 0;
    int16_t itr = 0;
    int16_t mana_units = 0;

    TRACE_ENTER();

    city_owner_idx = _CITIES[city_idx].owner_idx;

    if(
        (events_table->Mana_Short_Status != 2) &&
        (_CITIES[city_idx].population != 0) &&
        (city_owner_idx != NEUTRAL_PLAYER_IDX)
    )
    {
        building_magic_power = 0;
        building_magic_power_modifier = 100;

        if(
            (_players[city_owner_idx].divine_power != ST_FALSE)
            ||
            (_players[city_owner_idx].infernal_power != ST_FALSE)
        )
        {
            building_magic_power_modifier = 150;
        }

        if(_CITIES[city_idx].enchantments[DARK_RITUALS] != ST_FALSE)
        {
            building_magic_power_modifier = (building_magic_power_modifier * 2);
        }

        if(
            (_CITIES[city_idx].bldg_status[SHRINE] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[SHRINE] == bs_Replaced)
        )
        {
            building_magic_power += 1;
        }

        if(
            (_CITIES[city_idx].bldg_status[TEMPLE] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[TEMPLE] == bs_Replaced)
        )
        {
            building_magic_power += 2;
        }

        if(
            (_CITIES[city_idx].bldg_status[PARTHENON] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[PARTHENON] == bs_Replaced)
        )
        {
            building_magic_power += 3;
        }

        if(
            (_CITIES[city_idx].bldg_status[CATHEDRAL] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[CATHEDRAL] == bs_Replaced)
        )
        {
            building_magic_power += 4;
        }

        building_magic_power = ((building_magic_power * building_magic_power_modifier) / 100);

        mana_units = building_magic_power;

        if(
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Replaced)
        )
        {
            mana_units += 3;
        }

        if(
            (_CITIES[city_idx].bldg_status[WIZARDS_GUILD] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[WIZARDS_GUILD] == bs_Replaced)
        )
        {
            mana_units += 3;
        }

        city_wp = _CITIES[city_idx].wp;

        if(_CITIES[city_idx].race == rt_Dark_Elf)
        {
            mana_units += _CITIES[city_idx].population;
        }

        if(
            (_CITIES[city_idx].race == rt_High_Elf) ||
            (_CITIES[city_idx].race == rt_Beastmen) ||
            (_CITIES[city_idx].race == rt_Draconian)
        )
        {
            mana_units += (_CITIES[city_idx].population / 2);
        }

        if(
            (_FORTRESSES[city_owner_idx].wx == _CITIES[city_idx].wx) &&
            (_FORTRESSES[city_owner_idx].wy == _CITIES[city_idx].wy) &&
            (_FORTRESSES[city_owner_idx].wp == city_wp)
        )
        {
            spell_ranks = (
                _players[city_owner_idx].spellranks[0] + 
                _players[city_owner_idx].spellranks[1] +
                _players[city_owner_idx].spellranks[2] +
                _players[city_owner_idx].spellranks[3] +
                _players[city_owner_idx].spellranks[4]
            );

            if(city_wp != 0)
            {
                mana_units += spell_ranks + 5;
            }
            else
            {
                mana_units += spell_ranks;
            }
        }

        if(_CITIES[city_idx].race != rt_Dwarf)
        {
            are_dwarf = ST_FALSE;
        }
        else
        {
            are_dwarf = ST_TRUE;
        }

        if(
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] != bs_Built) &&
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] != bs_Replaced)
        )
        {
            have_miners_guild = ST_FALSE;
        }
        else
        {
            have_miners_guild = ST_TRUE;
        }

        useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

        for(itr = 0; itr < useable_map_squares; itr++)
        {
            mana_units += Square_Magic_Power(wx_array[itr], wy_array[itr], city_wp, have_miners_guild, are_dwarf);
        }

        if(
            (_players[city_owner_idx].spellranks[sbr_Death] != 0) &&
            (events_table->Good_Moon_Status == 2)
        )
        {
            mana_units = (mana_units - (building_magic_power / 2));
        }

        if(
            (_players[city_owner_idx].spellranks[sbr_Life] != 0) &&
            (events_table->Bad_Moon_Status == 2)
        )
        {
            mana_units = (mana_units - (building_magic_power / 2));
        }

        if(mana_units < 0)
        {
            mana_units = 0;
        }

        if(
            (_players[city_owner_idx].spellranks[sbr_Life] != 0) &&
            (events_table->Good_Moon_Status == 2)
        )
        {
            mana_units += building_magic_power;
        }

        if(
            (_players[city_owner_idx].spellranks[sbr_Death] != 0) &&
            (events_table->Bad_Moon_Status == 2)
        )
        {
            mana_units += building_magic_power;
        }

    }
    else
    {
        mana_units = 0;
    }

    TRACE_EXIT();

    return mana_units;
}


// WZD o142p15
/**
 * @brief Computes the per-turn population growth value for a city.
 *
 * @details
 * Calculates growth (or shrink) from food capacity, race growth traits,
 * buildings, enchantments, current construction target, ownership, and
 * difficulty scaling. The function first derives a base growth rate, then
 * applies a percentage modifier, and finally applies owner-specific rules.
 *
 * Base growth path:
 * - Returns 0 for outposts (@c population == 0).
 * - Uses @c City_Maximum_Size() as the food-capacity anchor.
 * - Applies immediate decay when food is below population.
 * - Applies race growth modifier and fixed building bonuses
 *   (@c GRANARY, @c FARMERS_MARKET).
 * - Applies event/enchantment multipliers
 *   (@c STREAM_OF_LIFE, Population Boom).
 *
 * Growth modifier path:
 * - Starts at 0 percent.
 * - Applies @c DARK_RITUALS penalty.
 * - If building @c bt_Housing, applies worker-derived bonus and additional
 *   building bonuses (@c SAWMILL, @c BUILDERS_HALL).
 *
 * Ownership and difficulty path:
 * - Neutral cities grow at half rate and are capped by a
 *   difficulty-based size threshold.
 * - AI-owned non-neutral cities apply
 *   @c difficulty_modifiers_table[_difficulty].population_growth scaling.
 * - Final value is clamped so normal growth cannot remain negative.
 *
 * @param city_idx Index into @c _CITIES for the city to evaluate.
 *
 * @return Per-turn population growth value in city population units.
 *         Positive values indicate growth, 0 indicates no growth,
 *         and starvation can produce a negative intermediate value before
 *         final clamping in the normal path.
 *
 * @note Contains documented legacy OGBUG: behaviors around max-population checks
 *       and early exits that can suppress expected negative growth in some
 *       edge cases.
 *
 * @see City_Maximum_Size(), City_Rebel_Count(), difficulty_modifiers_table
 */
int16_t City_Growth_Rate(int16_t city_idx)
{
    int16_t maximum_size = 0;
    int16_t population_growth_modifier = 0;
    int16_t population_growth_rate = 0;
/*
    BEGIN:  population_growth_rate
*/
    if(_CITIES[city_idx].population == 0)
    {
        population_growth_rate = 0;
        goto Done;
    }
    maximum_size = City_Maximum_Size(city_idx);
    /* If population matches maximum size, growth is 0 */
    /* OGBUG  growth stops only when population EQUALS the maximum: a city above its maximum (its food area shrunk) still gets (max - pop + 1) / 2 + race + Granary / Farmers' Market and Housing bonuses and can keep growing; */
    /* OGBUG  and a starving city AT its maximum returns 0 here, before the food < population test, so it does not shrink */
    if(_CITIES[city_idx].population == maximum_size)
    {
        population_growth_rate = 0;
        goto Done;
    }
    /* If maximum size is 0, population decays by current population */
    if(maximum_size == 0)
    {
        population_growth_rate = -(_CITIES[city_idx].population);
        goto Done;
    }
    /* Check for starvation (insufficient food) */
    // if(_CITIES[city_idx].food_units < _CITIES[city_idx].population)
    if(_CITIES[city_idx].food_units < (uint8_t)_CITIES[city_idx].population)  /* CLAUDE */  /* REBUG */  /* asm: cmp al, [es:bx+s_CITY.population] / jnb (unsigned byte compare) */
    {
        // population_growth_rate = ((_CITIES[city_idx].food_units - _CITIES[city_idx].population) * 5);
        population_growth_rate = ((_CITIES[city_idx].food_units - _CITIES[city_idx].population) * 5);  /* CLAUDE */  /* REBUG */  /* asm: food_units zero-extended (mov ah, 0), population cbw */
        goto Done;
    }
    /* Normal population growth calculation */
    // e.g., 20 - 10 = 10 + 1 = 11 / 2 = 5 + {2, 1, 0, -1, -2} = {7, 6, 5, 4, 3}
    population_growth_rate = ((((maximum_size - _CITIES[city_idx].population) + 1) / 2) + _race_type_table[_CITIES[city_idx].race].Growth_Mod);
    /* Granary increases growth by 2 */
    if(
        (_CITIES[city_idx].bldg_status[GRANARY] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[GRANARY] == bs_Replaced)
    )
    {
        population_growth_rate += 2;
    }
    /* Farmer's Market increases growth by 3 */
    if(
        (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Replaced)
    )
    {
        population_growth_rate += 3;
    }
    /* Max population limit for growth is 25 (25,000) */
    /* OGBUG: drake178: BUG #2: prevents negative growth at 25 pop */
    if(_CITIES[city_idx].population >= MAX_CITY_POPULATION)
    {
        population_growth_rate = 0;
        goto Done;
    }
    /* Stream of Life doubles population growth */
    if(_CITIES[city_idx].enchantments[STREAM_OF_LIFE] > 0)
    {
        population_growth_rate = (population_growth_rate * 2);
    }
    /* Population Boom event doubles population growth */
    if( (events_table->Population_Boom_Status != 0) && (events_table->Population_Boom_Data == city_idx) )
    {
        population_growth_rate = (population_growth_rate * 2);
    }
/*
    END:  population_growth_rate
*/
/*
    BEGIN:  population_growth_modifier
*/
    population_growth_modifier = 0;
    /* Dark Rituals reduces growth by 25% */
    if(_CITIES[city_idx].enchantments[DARK_RITUALS] > 0)
    {
        population_growth_modifier -= 25;
    }
    /* Housing production bonus */
    if(_CITIES[city_idx].construction == bt_Housing)
    {
        if(_CITIES[city_idx].population == 1)
        {
            population_growth_modifier += 50;
        }
        else
        {
            /* Add percentage of workers (non-rebels, non-farmers) */
            population_growth_modifier += ((((_CITIES[city_idx].population - City_Rebel_Count(city_idx)) - _CITIES[city_idx].farmer_count) * 100) / _CITIES[city_idx].population);
        }
        /* Sawmill increases housing growth modifier by 10% */
        if(
            (_CITIES[city_idx].bldg_status[SAWMILL] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[SAWMILL] == bs_Replaced)
        )
        {
            population_growth_modifier += 10;
        }
        /* Builders Hall increases housing growth modifier by 15% */
        if(
            (_CITIES[city_idx].bldg_status[BUILDERS_HALL] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[BUILDERS_HALL] == bs_Replaced)
        )
        {
            population_growth_modifier += 15;
        }
        /* Recheck population limit */
        /* OGBUG: drake178: BUG #3: prevents negative growth at 25 pop if housing is selected as the build project, even with zero workers and no buildings */
        if(_CITIES[city_idx].population >= MAX_CITY_POPULATION)
        {
            population_growth_rate = 0;
            goto Done;  /* CLAUDE */  /* REBUG */  /* asm: jmp @@JmpJmpDone_Return_ZERO */
        }
    }
/*
    END:  population_growth_modifier
*/
    /* Apply population growth modifier */
    population_growth_rate = ((population_growth_rate * (population_growth_modifier + 100)) / 100);
    /* Neutral player check */
    if(_CITIES[city_idx].owner_idx == NEUTRAL_PLAYER_IDX)
    {
        population_growth_rate /= 2;
        /* Neutral cities stop growing at difficulty-based population size */
        if(_CITIES[city_idx].population >= ((_difficulty + 1) * 2))
        {
            population_growth_rate = 0;
        }
    }
    else if(_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
    {
        /* AI players growth modifiers based on difficulty */
        population_growth_rate = ((population_growth_rate * difficulty_modifiers_table[_difficulty].population_growth) / 100);
    }
    /* Normal growth cannot be negative (clamped to 0) */
    SETMIN(population_growth_rate, 0);
Done:
    return population_growth_rate;
}


// WZD o142p16
// drake178: CTY_GetProdCost()
// MoO2  Module:  COLCALC  Colony_Current_Product_Cost_()  Colony_Product_Cost_()
int16_t City_Current_Product_Cost(int16_t city_idx)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t have_miners_guild = 0;
    int16_t unit_cost_percent = 0;
    int16_t itr = 0;
    int16_t are_dwarf = 0;
    int16_t unit_cost_reduction_applies = 0;
    int16_t city_wp = 0;
    int16_t useable_map_squares = 0;
    int16_t product_cost = 0;
    int16_t product_idx = 0;  // _DI_


    EMMDATAH_Map();


    product_idx = _CITIES[city_idx].construction;

    // if(_CITIES[city_idx].race != 4)  /* R_Dwarf */
    // {
    //     are_dwarf = ST_FALSE;
    // }
    // else
    // {
    //     are_dwarf = ST_TRUE;
    // }
    /* CLAUDE */  /* REBUG */  /* asm: cmp race, rt_Dwarf / jnz -> FALSE, else TRUE */
    if(_CITIES[city_idx].race == rt_Dwarf)
    {
        are_dwarf = ST_TRUE;
    }
    else
    {
        are_dwarf = ST_FALSE;
    }

    if(product_idx >= 100)
    {
        product_cost = _unit_type_table[(product_idx - 100)].cost;

        city_wp = _CITIES[city_idx].wp;

        useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

        unit_cost_percent = 100;


        /*
            Terrain Special - Military Unit Cost Reduction
            * Applies only to normal units except for magicians, priests, shamans and warlocks.
        */
        unit_cost_reduction_applies = ST_TRUE;

        if((_unit_type_table[(product_idx - 100)].Abilities & UA_PURIFY) != 0)
        {
            unit_cost_reduction_applies = ST_FALSE;
        }

        if(_unit_type_table[(product_idx - 100)].Transport != 0)
        {
            unit_cost_reduction_applies = ST_FALSE;
        }

        if((_unit_type_table[(product_idx - 100)].Attribs_2 & USA_FIREBALL) != 0)
        {
            unit_cost_reduction_applies = ST_FALSE;
        }

        if((_unit_type_table[(product_idx - 100)].Attribs_2 & USA_DOOMBOLT) != 0)
        {
            unit_cost_reduction_applies = ST_FALSE;
        }

        if( (_CITIES[city_idx].bldg_status[MINERS_GUILD] == bs_Built) || (_CITIES[city_idx].bldg_status[MINERS_GUILD] == bs_Replaced) )
        {
            have_miners_guild = ST_TRUE;
        }
        else
        {
            have_miners_guild = ST_FALSE;
        }

        // if(unit_cost_reduction_applies == ST_TRUE)
        // {
        //     for(itr = 0; itr < useable_map_squares; itr++)
        //     {
        //         unit_cost_percent -= Square_Unit_Cost_Reduction(wx_array[itr], wy_array[itr], city_wp, have_miners_guild, are_dwarf);
        //     }
        // }
        // 
        // if(unit_cost_percent < 50)
        // {
        //     unit_cost_percent = 50;
        // }
        // 
        // if(unit_cost_percent != 100)
        // {
        //     product_cost = ((product_cost * unit_cost_percent) / 100);
        // }
        /* CLAUDE */  /* REBUG */  /* asm: cmp unit_cost_reduction_applies, e_ST_TRUE / jnz @@Done - the 50% floor and the percentage are inside this test */
        if(unit_cost_reduction_applies == ST_TRUE)
        {

            for(itr = 0; itr < useable_map_squares; itr++)
            {
                /* OGBUG  passes the real Miners' Guild flag, and Square_Unit_Cost_Reduction doubles the Iron / Coal cut for it (10 / 20) instead of adding 50% */
                unit_cost_percent -= Square_Unit_Cost_Reduction(wx_array[itr], wy_array[itr], city_wp, have_miners_guild, are_dwarf);
            }

            if(unit_cost_percent < 50)
            {
                unit_cost_percent = 50;
            }

            if(unit_cost_percent != 100)
            {
                product_cost = ((product_cost * unit_cost_percent) / 100);
            }

        }

    }
    else
    {
        product_cost = bldg_data_table[product_idx].construction_cost;
    }

    return product_cost;
}


// WZD o142p17
// UU_AI_Fort_Defense()


// WZD o142p18
// drake178: EVNT_GenerateMercs()
/*
; attempts to generate mercenaries that can offer
; themselves up for hire, filling out all but the
; location from the return fields
; returns the level of units offered (won't be used)
; BUG: checks for charismatic only after discarding for not enough gold
*/
/*

    `*amount = 0;` indicates failure

*/
void Generate_Mercenaries(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp, int16_t * amount, int16_t * type, int16_t * cost, int16_t * level)
{
    int16_t G_Tries = 0;
    int16_t Myrror_Available = 0;
    int16_t Arcanus_Available = 0;
    int16_t Unit_Count = 0;
    int16_t Total_Cost = 0;
    int16_t Roll_Total = 0;
    int16_t Merc_Level = 0;
    int16_t player_fame = 0;
    int16_t itr_units = 0;  // _SI_
    int16_t itr_cities = 0;  // _SI_
    int16_t itr_towers = 0;  // _SI_
    int16_t unit_type = 0;  // _DI_
    int16_t return_value = 0;  // _AX_

    /* HACK */  Total_Cost = Stack_Garbage();  /* OGBUG: Total_Cost is uninitialized */

    Myrror_Available = ST_FALSE;
    Arcanus_Available = ST_FALSE;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(_UNITS[itr_units].owner_idx != player_idx)
        {
            continue;
        }

        if(_UNITS[itr_units].wp == 0)
        {
            Arcanus_Available = ST_TRUE;
        }
        else
        {
            Myrror_Available = ST_TRUE;
        }

    }

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        if(_CITIES[itr_cities].owner_idx != player_idx)
        {
            continue;
        }

        if(_CITIES[itr_cities].wp == 0)
        {
            Arcanus_Available = ST_TRUE;
        }
        else
        {
            Myrror_Available = ST_TRUE;
        }

    }


    for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
    {

        if(_TOWERS[itr_towers].owner_idx == player_idx)
        {
            Arcanus_Available = ST_TRUE;
            Myrror_Available = ST_TRUE;
        }

    }

    player_fame = Player_Fame(player_idx);

    Roll_Total = (Random(100) + player_fame);

    if(Roll_Total > 90)
    {
        Merc_Level = 3;
    }
    else if(Roll_Total > 60)
    {
        Merc_Level = 2;
    }
    else
    {
        Merc_Level = 1;
    }

    G_Tries = 0;
    while(1)
    {
        G_Tries++;

        if(
            (G_Tries >= 20)
            ||
            (_turn < 30)
        )
        {
            *amount = 0;
            return;
        }

        unit_type = (Random(198) - 1);

        if((_unit_type_table[unit_type].Abilities & UA_FANTASTIC) != 0)
        {
            continue;
        }

        if((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
        {
            continue;
        }

        if((_unit_type_table[unit_type].Move_Flags & MV_SAILING) != 0)
        {
            continue;
        }

        if(unit_type < ut_Trireme)
        {
            continue;
        }

        if(unit_type == ut_NmdMagicians)
        {
            continue;
        }

        if(
            (_unit_type_table[unit_type].race_type == rt_Draconian)
            &&
            (_unit_type_table[unit_type].Construction > 0)
        )
        {
            continue;
        }

        if(
            (_unit_type_table[unit_type].race_type == rt_Beastmen)
            ||
            (_unit_type_table[unit_type].race_type == rt_Dark_Elf)
            ||
            (_unit_type_table[unit_type].race_type == rt_Draconian)
            ||
            (_unit_type_table[unit_type].race_type == rt_Dwarf)
            ||
            (_unit_type_table[unit_type].race_type == rt_Troll)
        )
        {
            if(Myrror_Available == ST_FALSE)
            {
                continue;
            }
        }
        else
        {
            if(Arcanus_Available == ST_FALSE)
            {
                continue;
            }
        }

        /* OGBUG  Total_Cost is first set below; the first try compares against an uninitialised value, later tries against the previous try's cost */
        if((_turn / 2) < Total_Cost)
        {
            continue;
        }

        Total_Cost = ((_unit_type_table[unit_type].cost * (3 + Merc_Level)) / 2);

        Roll_Total = (Random(100) + player_fame);

        if(Roll_Total > 90)
        {
            Unit_Count = 3;
        }
        else if(Roll_Total > 60)
        {
            Unit_Count = 2;
        }
        else
        {
            Unit_Count = 1;
        }

        Total_Cost *= Unit_Count;

        // drake178: ; BUG: wrong order of operations
        if(_players[player_idx].gold_reserve < Total_Cost)
        {
            Unit_Count = 0;
        }

        if(_players[player_idx].charismatic != 0)
        {
            Total_Cost /= 2;  // 50% half price
        }

        if(player_idx != HUMAN_PLAYER_IDX)
        {
            if(AI_Accept_Mercenaries(player_idx, Total_Cost) == ST_FALSE)
            {
                Unit_Count = 0;
            }
        }

        *amount = Unit_Count;
        *type = unit_type;
        *cost = Total_Cost;
        *level = Merc_Level;
        return;

    }

}


// WZD o142p19
// drake178: LD_MAP_TFUnk40_Eval()
/*
Loaded_Game_Update(): records the first MAX_UNKNOWN_40H_SQUARES squares, Arcanus then Myrror, row by row, that have MSF_UNKNOWN_40H set
    nothing in WIZARDS.EXE reads the recorded squares back
*/
void Record_Unknown_40H_Squares(void)
{
    int16_t itr_wx = 0;
    int16_t itr_wy = 0;
    int16_t itr_wp = 0;  // _CX_

    unknown_40h_square_count = 0;

    for(itr_wp = 0; ((itr_wp < NUM_PLANES) && (unknown_40h_square_count < MAX_UNKNOWN_40H_SQUARES)); itr_wp++)
    {

        for(itr_wy = 0; ((itr_wy < WORLD_HEIGHT) && (unknown_40h_square_count < MAX_UNKNOWN_40H_SQUARES)); itr_wy++)
        {

            for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
            {

                if((_map_square_flags[((itr_wp * WORLD_SIZE) + (itr_wy * WORLD_WIDTH) + itr_wx)] & MSF_UNKNOWN_40H) != 0)
                {

                    if(unknown_40h_square_count < MAX_UNKNOWN_40H_SQUARES)
                    {

                        /* OGBUG  dead stores - nothing reads unknown_40h_square_wx[] / _wy[] / _wp[] or the count */
                        unknown_40h_square_wx[unknown_40h_square_count] = itr_wx;

                        unknown_40h_square_wy[unknown_40h_square_count] = itr_wy;

                        unknown_40h_square_wp[unknown_40h_square_count] = itr_wp;

                        unknown_40h_square_count++;

                    }

                }

            }

        }

    }

}


// WZD o142p20
// drake178: CTY_GetMinFarmers()
/*
calculates and returns the minimum amount of farmers
that the city needs to sustain its population
*/
/*

¿ can be negative ?
Nope.

*/
int16_t City_Minimum_Farmers(int16_t city_idx)
{
    int16_t food_per_farmer = 0;
    int16_t Farming_Threshold = 0;
    int16_t city_population = 0;
    int16_t required_farmer_food = 0;
    int16_t minimum_farmer_count = 0;
    int16_t farmer_food = 0;  // _DI_

    EMMDATAH_Map();

    city_population = _CITIES[city_idx].population;

    required_farmer_food = (city_population - City_Food_WildGame(city_idx));

    if(
        (_CITIES[city_idx].bldg_status[GRANARY] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[GRANARY] == bs_Replaced)
    )
    {

        required_farmer_food -= 2;
        
    }

    if(
        (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Replaced)
    )
    {

        required_farmer_food -= 3;

    }

    if(required_farmer_food > 0)
    {

        Farming_Threshold = City_Food_Terrain(city_idx);

        if(
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Replaced)
            ||
            (_CITIES[city_idx].race == rt_Halfling)
        )
        {

            food_per_farmer = 3;

        }
        else
        {

            food_per_farmer = 2;

        }

        minimum_farmer_count = 0;

        farmer_food = 0;

        while((farmer_food < required_farmer_food) && (minimum_farmer_count < city_population))
        {

            minimum_farmer_count++;

            farmer_food = minimum_farmer_count * food_per_farmer;

            if(
                (_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Built)
                ||
                (_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Replaced)
            )
            {

                farmer_food += 2;

            }

            if(_CITIES[city_idx].enchantments[FAMINE] != ST_FALSE)
            {

                farmer_food /= 2;

            }

            if(farmer_food > Farming_Threshold)
            {

                farmer_food = (Farming_Threshold + ((farmer_food - Farming_Threshold) / 2));

            }

        }

    }
    else
    {

        minimum_farmer_count = 0;

    }

    assert(minimum_farmer_count >= 0);

    return minimum_farmer_count;
}


// WZD o142p21
// MoO2  Module: COLCALC  Apply_Colony_Pop_Growth_()
/*
OGBUG: ignores Wild Game, both as a source of food and as a terrain special
OGBUG: difficulty-based outpost growth modifiers are applied to both AI and human player outposts
*/
/**
 * @brief Applies one-turn growth/shrink updates to all outposts.
 *
 * @details
 * Iterates all entries in @c _CITIES and processes only outposts
 * (cities with @c population == 0). For each outpost, the routine computes a
 * growth chance from terrain food, race growth bonus, selected city enchantments,
 * nearby terrain specials, and global difficulty modifiers; then computes a
 * shrink chance from harmful enchantments.
 *
 * Processing flow per outpost:
 * 1. Start growth from @c City_Food_Terrain(city_idx).
 * 2. Add race outpost-growth bonus
 *    (@c _race_type_table[race].outpost_growth_rate).
 * 3. Add enchantment growth bonuses:
 *    - @c GAIAS_BLESSING: +20
 *    - @c STREAM_OF_LIFE: +10
 * 4. Scan usable city-area squares and add terrain-special bonus:
 *    - @c TS_IRON or @c TS_SILVER: +5 each
 *    - Any other non-zero special: +10
 * 5. Build shrink chance starting at 5, then add:
 *    - @c EVIL_PRESENCE: +5
 *    - @c PESTILENCE: +10
 *    - @c FAMINE: +10
 *    - @c CHAOS_RIFT: +10
 * 6. Apply difficulty scaling to growth chance.
 * 7. Roll stochastic updates:
 *    - If @c Random(100) <= grow, increase @c Pop_10s by @c Random(3).
 *    - If @c Random(100) <= shrink, decrease @c Pop_10s by @c Random(2).
 *
 * @return This function returns no value.
 *
 * @note This routine mutates @c _CITIES[itr_cities].Pop_10s in place.
 * @note Growth and shrink checks are independent random rolls and can both
 *       trigger in the same turn.
 * @note (OGBUG) Terrain-special lookup in the inner loop indexes
 *       @c wx_array/@c wy_array with @c itr_cities instead of @c itr,
 *       causing incorrect reads and potential out-of-bounds access when
 *       @c itr_cities >= CITY_AREA_SIZE.
 * @note (OGBUG) Wild Game is ignored in this outpost-growth path despite being
 *       relevant to food/terrain valuation elsewhere.
 * @note (OGBUG) Difficulty outpost-growth modifiers are applied regardless of
 *       whether the outpost belongs to AI or human.
 *
 * @see City_Food_Terrain(), Get_Useable_City_Area(), GET_TERRAIN_SPECIAL(), Random()
 */
void All_Outpost_Population_Growth(void)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;
    int16_t city_wp = 0;
    int16_t terrain_special = 0;
    int16_t useable_map_squares = 0;
    int16_t shrink = 0;
    int16_t itr_cities = 0;
    int16_t grow = 0;
    EMMDATAH_Map();
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        /* Only process Outposts, not Cities */
        if(_CITIES[itr_cities].population != 0)
        {
            continue;
        }
        grow = City_Food_Terrain(itr_cities);
        grow += _race_type_table[_CITIES[itr_cities].race].outpost_growth_rate;
        if(_CITIES[itr_cities].enchantments[GAIAS_BLESSING] != 0)
        {
            grow += 20;
        }
        if(_CITIES[itr_cities].enchantments[STREAM_OF_LIFE] != 0)
        {
            grow += 10;
        }
        city_wp = CITIESP();
        useable_map_squares = Get_Useable_City_Area(CITIESX(), CITIESY(), city_wp, &wx_array[0], &wy_array[0]);
        for(itr = 0; itr < useable_map_squares; itr++)
        {
            /* OGBUG: should use itr, not itr_cities */
            /* OGBUG: OOB AVRL  itr_cities >= 25 */
            /* OGBUG  for itr_cities >= 25 the OG reads past the 25-entry arrays: wy_array[25 + k] is wx_array[k], wx_array[25..29] are itr / city_wp / terrain_special / useable_map_squares / shrink, then saved bp / return address and beyond */
            // terrain_special = GET_TERRAIN_SPECIAL(wx_array[itr_cities], wy_array[itr_cities], city_wp);
            /* HACK */  /* bounded: for itr_cities >= CITY_AREA_SIZE the stack words the OG reads are replaced by in-range stand-ins */
            /* HACK */  terrain_special = (GET_TERRAIN_SPECIAL(((itr_cities < CITY_AREA_SIZE) ? wx_array[itr_cities] : Stack_Garbage_Range(0, (WORLD_WIDTH - 1))), ((itr_cities < CITY_AREA_SIZE) ? wy_array[itr_cities] : Stack_Garbage_Range(0, (WORLD_HEIGHT - 1))), city_wp) & 0x0F);  /* CLAUDE */  /* asm: mov ah, 0 / and ax, 0Fh (the mineral nibble) */
            /* OGBUG  only the mineral nibble (& 0x0F) is kept, so Wild Game (TS_WILDGAME, 0x40) gives no outpost growth; any other mineral gives +10, iron / silver +5 */
            if(terrain_special != 0)
            {
                if(
                    (terrain_special == TS_IRON)
                    ||
                    (terrain_special == TS_SILVER)
                )
                {
                    grow += 5;

                }
                else
                {
                    grow += 10;
                }
            }
        }
        shrink = 5;  // 5%  1:20
        if(_CITIES[itr_cities].enchantments[EVIL_PRESENCE] != 0)
        {
            shrink += 5;  // 5%  1:20
        }
        if(_CITIES[itr_cities].enchantments[PESTILENCE] != 0)
        {
            shrink += 10;  // 10%  2:20
        }
        if(_CITIES[itr_cities].enchantments[FAMINE] != 0)
        {
            shrink += 10;  // 10%  2:20
        }
        if(_CITIES[itr_cities].enchantments[CHAOS_RIFT] != 0)
        {
            shrink += 10;  // 10%  2:20
        }
        /* OGBUG  the difficulty table's outpost growth (Intro / Easy 100, Normal 150, Hard 200, Impossible 400 %) is applied to EVERY outpost, the human's too: on harder levels the human's outposts grow up to 4x faster */
        grow = (((int32_t)difficulty_modifiers_table[_difficulty].outpost_growth * grow) / 100);
        if(Random(100) <= grow)
        {
            _CITIES[itr_cities].Pop_10s = (_CITIES[itr_cities].Pop_10s + Random(3));
        }
        if(Random(100) <= shrink)
        {
            _CITIES[itr_cities].Pop_10s = (_CITIES[itr_cities].Pop_10s - Random(2));
        }
    }
}


// WZD o142p22
// MoO2  Module: BILL  Record_History_()
/*
calculates and records the 'power of a wizard', for the "History of Wizards’ Power graph"
"The power of a wizard is the combined value of many things including army strength, total magic power drawn from all sources per turn, and spells that wizard has learned."
*/
void Record_History(void)
{
    int16_t power_of_a_wizard[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr_history = 0;
    int16_t max_curr_population = 0;
    int16_t max_curr_research = 0;
    int16_t max_curr_military = 0;
    int16_t max_curr_magic = 0;
    int16_t itr = 0;
    int16_t itr_players = 0;
    int16_t max_curr_wiz_pwr = 0;

    // ; fill out the astrologer fields and the local total progress array (sum of all fields)
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].astrologer.magic_power     = Astrologer_Power(itr_players);
        _players[itr_players].astrologer.army_strength   = Astrologer_Strength(itr_players);
        _players[itr_players].astrologer.spell_research  = Astrologer_Research(itr_players);
        _players[itr_players].empire_mini_pops           = Empire_Population_Points(itr_players);

        power_of_a_wizard[itr_players] = (
            _players[itr_players].astrologer.magic_power
            + _players[itr_players].astrologer.army_strength
            + _players[itr_players].astrologer.spell_research
            + _players[itr_players].empire_mini_pops
        );
    }

    max_curr_magic = 0;
    max_curr_military = 0;
    max_curr_research = 0;
    max_curr_population = 0;
    max_curr_wiz_pwr = 0;

    // ; find and store the highest values in each category, with di holding the highest total
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(_players[itr_players].astrologer.magic_power > max_curr_magic)
        {
            max_curr_magic = _players[itr_players].astrologer.magic_power;
        }
        
        if(_players[itr_players].astrologer.army_strength > max_curr_military)
        {
            max_curr_military = _players[itr_players].astrologer.army_strength;
        }
        
        if(_players[itr_players].astrologer.spell_research > max_curr_research)
        {
            max_curr_research = _players[itr_players].astrologer.spell_research;
        }
        
        if(_players[itr_players].empire_mini_pops > max_curr_population)
        {
            max_curr_population = _players[itr_players].empire_mini_pops;
        }

        if(power_of_a_wizard[itr_players] > max_curr_wiz_pwr)
        {
            max_curr_wiz_pwr = power_of_a_wizard[itr_players];
        }
        
    }

    // adjust power so that the highest is at most 200
    if(max_curr_magic > 200)
    {
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            _players[itr_players].astrologer.magic_power = (((int32_t)_players[itr_players].astrologer.magic_power * 200) / max_curr_magic);
        }
    }

    // adjust army strength so that the highest is at most 200
    if(max_curr_military > 200)
    {
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            _players[itr_players].astrologer.army_strength = (((int32_t)_players[itr_players].astrologer.army_strength * 200) / max_curr_military);
        }
    }

    // adjust research so that the highest is at most 200
    if(max_curr_research > 200)
    {
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            _players[itr_players].astrologer.spell_research = (((int32_t)_players[itr_players].astrologer.spell_research * 200) / max_curr_research);
        }
    }

    // divide the total progress values by 7
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        power_of_a_wizard[itr_players] = (power_of_a_wizard[itr_players] / 7);
    }

    max_curr_wiz_pwr = (max_curr_wiz_pwr / 7);

    // adjust the total progress values such that the highest is at most 160
    if(max_curr_wiz_pwr > 160)
    {
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            power_of_a_wizard[itr_players] = (((int32_t)power_of_a_wizard[itr_players] * 160) / max_curr_wiz_pwr);
        }
    }
    

    if(_turn < 288)
    {
        // record the total progress values into the historian
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            _players[itr_players].history[_turn] = (uint8_t)power_of_a_wizard[itr_players];
        }
    }
    else
    {
        // scroll the historian, then record the total progress values into it
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            for(itr_history = 0; itr_history < 287; itr_history++)
            {
                _players[itr_players].history[itr_history] = _players[itr_players].history[(itr_history + 1)];
            }

            _players[itr_players].history[287] = (uint8_t)power_of_a_wizard[itr_players];
        }
    }

}


// WZD o142p23
int16_t Astrologer_Power(int16_t player_idx)
{
    int16_t power;
    power = _players[player_idx].Power_Base - 10;
    SETMIN(power, 0);
    return power;
}

// WZD o142p24
int16_t Astrologer_Strength(int16_t player_idx)
{
    int32_t total_unit_upkeep = 0;
    int16_t itr_units = 0;
    total_unit_upkeep = 0;
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == player_idx)
        {
            total_unit_upkeep += (Unit_Gold_Upkeep(itr_units) + (2 * Unit_Mana_Upkeep(itr_units)));
        }
    }
    return (total_unit_upkeep / 5);
}

// WZD o142p25
int16_t Astrologer_Research(int16_t player_idx)
{
    int16_t itr_realms = 0;
    int16_t research_total = 0;
    int16_t itr_spell_in_realm = 0;
    research_total = 0;
    for(itr_realms = 0; itr_realms < NUM_MAGIC_REALMS; itr_realms++)
    {
        for(itr_spell_in_realm = 0; itr_spell_in_realm < NUM_SPELLS_PER_MAGIC_REALM; itr_spell_in_realm++)
        {
            if(_players[player_idx].spells_list[(itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spell_in_realm] == sls_Known)
            {
                research_total += (itr_spell_in_realm + 1);
            }
        }
    }
    return (research_total / 8);
}

// WZD o142p26
/* MoO2  Module: COLCALC  Colony_N_Pops_Of_Empire_() */
int16_t Empire_Population_Points(int16_t player_idx)
{
    int16_t itr_cities = 0;
    int16_t empire_population = 0;
    empire_population = 0;
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            empire_population += _CITIES[itr_cities].population;
        }
    }
    return (empire_population / 10);  // ~ "mini population units"
}


// WZD o142p27
// drake178: CTY_GetRebelCount()
int16_t City_Rebel_Count(int16_t city_idx)
{
    int16_t Pacify_Police = 0;
    int16_t city_wp = 0;
    int16_t Pacify_Religion = 0;
    int16_t unrest_percent = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t Pacify = 0;
    // itr_num_players__units
    int16_t itr_num_players = 0;  // itr_num_players__units
    int16_t itr_units = 0;  // itr_num_players__units
    int16_t city_owner_idx = 0;
    int16_t rebel_count = 0;
    int16_t unrest_races = 0;  // DNE in Dasm
    int16_t unrest_taxes = 0;  // DNE in Dasm

    TRACE_ENTER();

    city_owner_idx = _CITIES[city_idx].owner_idx;

    city_wx = _CITIES[city_idx].wx;

    city_wy = _CITIES[city_idx].wy;

    city_wp = _CITIES[city_idx].wp;

    unrest_races = (TBL_Unrest[_players[city_owner_idx].capital_race][_CITIES[city_idx].race] * 10);

    unrest_taxes = tax_unrest_pct_table[_players[city_owner_idx].tax_rate];

    unrest_percent = unrest_races + unrest_taxes;

    if(_CITIES[city_idx].enchantments[FAMINE] != ST_FALSE)
    {
        
        unrest_percent += 25;

    }

    rebel_count = ((_CITIES[city_idx].population * unrest_percent) / 100);

    Pacify = 0;

    if(
        (_CITIES[city_idx].enchantments[EVIL_PRESENCE] == ST_FALSE)
        ||
        (_players[city_owner_idx].spellranks[sbr_Death] != 0)
    )
    {

        Pacify_Religion = 0;

        if(
            (_CITIES[city_idx].bldg_status[SHRINE] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[SHRINE] == bs_Replaced)
        )
        {

            Pacify_Religion += 1;

        }

        if(
            (_CITIES[city_idx].bldg_status[TEMPLE] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[TEMPLE] == bs_Replaced)
        )
        {

            Pacify_Religion += 1;

        }

        if(
            (_CITIES[city_idx].bldg_status[PARTHENON] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[PARTHENON] == bs_Replaced)
        )
        {
            
            Pacify_Religion += 1;

        }

        if(
            (_CITIES[city_idx].bldg_status[CATHEDRAL] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[CATHEDRAL] == bs_Replaced)
        )
        {

            Pacify_Religion += 1;

        }

        if(
            (_players[city_owner_idx].infernal_power != ST_FALSE)
            ||
            (_players[city_owner_idx].divine_power != ST_FALSE)
        )
        {
            Pacify_Religion += (Pacify_Religion / 2);
        }

        Pacify += Pacify_Religion;

    }

    if(_CITIES[city_idx].bldg_status[ORACLE] == bs_Built)
    {

        Pacify += 2;

    }

    if(_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Built)
    {

        Pacify += 1;

    }

    for(itr_num_players = 0; itr_num_players < _num_players; itr_num_players++)
    {

        if(itr_num_players != city_owner_idx)
        {

            if(_players[itr_num_players].Globals[GREAT_WASTING] != ST_FALSE)
            {

                // BUG: ¿ 'Great Wasting' was only supposed to do -1, not be -1 ?
                Pacify = -1;

            }

        }

    }

    for(itr_num_players = 0; itr_num_players < _num_players; itr_num_players++)
    {

        if(itr_num_players != city_owner_idx)
        {

            if(_players[itr_num_players].Globals[ARMAGEDDON] != ST_FALSE)
            {

                // BUG: ¿ 'Great Wasting' was only supposed to do -2, not be -2 ?
                Pacify = -2;

            }

        }

    }


    if(_CITIES[city_idx].enchantments[DARK_RITUALS] != ST_FALSE)
    {

        Pacify -= 1;

    }

    // ¿ also counts as the 'Plague' Event ?
    if(_CITIES[city_idx].enchantments[PESTILENCE] != ST_FALSE)
    {

        Pacify -= 2;

    }

    if(_CITIES[city_idx].enchantments[CURSED_LANDS] != ST_FALSE)
    {

        Pacify -= 1;

    }

    if(_players[city_owner_idx].Globals[JUST_CAUSE] != ST_FALSE)
    {

        Pacify += 1;

    }

    if(_players[city_owner_idx].Globals[GAIAS_BLESSING] != ST_FALSE)
    {

        Pacify += 2;

    }

    if(Pacify < rebel_count)
    {

        Pacify_Police = 0;

        for(itr_units = 0; itr_units < _units; itr_units++)
        {

            if(
                (_UNITS[itr_units].owner_idx == city_owner_idx)
                &&
                (_UNITS[itr_units].wx == city_wx)
                &&
                (_UNITS[itr_units].wy == city_wy)
                &&
                (_UNITS[itr_units].wp == city_wp)
                &&
                (_UNITS[itr_units].type < ut_Magic_Spirit)
            )
            {

                Pacify_Police++;

            }

        }

        Pacify += (Pacify_Police / 2);

    }

    rebel_count -= Pacify;

    if(rebel_count < 0)
    {

        rebel_count = 0;

    }

    if(_players[city_owner_idx].Globals[STREAM_OF_LIFE] != ST_FALSE)
    {

        rebel_count = 0;

    }

    if(_CITIES[city_idx].population < rebel_count)
    {

        rebel_count = _CITIES[city_idx].population;

    }

    if((_CITIES[city_idx].farmer_count + rebel_count) > _CITIES[city_idx].population)
    {

        CITIES_FARMER_COUNT(city_idx, _CITIES[city_idx].population);

    }

    if(_CITIES[city_idx].farmer_count < 0)
    {

        CITIES_FARMER_COUNT(city_idx, 0);

    }

    TRACE_EXIT();

    return rebel_count;

}


// WZD o142p28
/**
 * @brief Computes city-economy baseline values for a map square and its catchment.
 *
 * @details
 * Scans the 5x5 city working area shape (21 squares with trimmed corners) centered
 * on (wx, wy), applies world-edge wrapping on X, skips out-of-bounds Y rows,
 * and aggregates resource-related outputs for map preview/evaluation.
 *
 * Processing summary:
 * - Initializes all output values to zero.
 * - Detects whether an actual city occupies the target square.
 * - For each eligible catchment square:
 *   - Marks `*is_unexplored` if any square is currently unexplored.
 *   - Uses full contribution when the square is unclaimed (or when evaluating an
 *     existing city square), otherwise applies shared-city half contribution logic.
 *   - Accumulates food-derived population potential, production bonus,
 *     unit-cost reduction, gold income, and magic income.
 *   - Sets material/special flags (`nightshade`, `mithril`, `adamantium`).
 * - Converts accumulated food-quarter units into population units and clamps
 *   `*MaxPop` to at most 25.
 * - If a city is present at (wx, wy), overrides `*MaxPop` with
 *   `City_Maximum_Size(city_idx)` and applies city building/enchantment modifiers
 *   to production and gold bonuses.
 * - Scans the surrounding 3x3 explored neighborhood to set `*have_shore` when
 *   at least one adjacent explored square is sailable.
 *
 * @param wx World X coordinate of the evaluated square.
 * @param wy World Y coordinate of the evaluated square.
 * @param wp World plane index.
 * @param MaxPop Output: estimated maximum population potential for the square.
 * @param production_bonus Output: percent-like production modifier contribution.
 * @param gold_bonus Output: percent-like gold modifier contribution.
 * @param unit_cost_reduction Output: cumulative unit cost reduction contribution.
 * @param gold_units Output: additive gold income from terrain specials.
 * @param magic_units Output: additive mana income from terrain specials.
 * @param have_nightshade Output flag: non-zero if any evaluated square has nightshade.
 * @param have_mithril Output flag: non-zero if mithril is present (unless later
 *                     replaced by adamantium handling in this routine).
 * @param have_adamantium Output flag: non-zero if adamantium is present.
 * @param have_shore Output flag: non-zero if an adjacent explored square is sailable.
 * @param is_unexplored Output flag: non-zero if any evaluated catchment square is unexplored.
 *
 * @return void
 *
 * @note This function mutates only the output pointers and does not return a value.
 * @warning Preserves historical behavior and OGBUG-marked logic in the original routine.
 */
void Compute_Base_Values_For_Map_Square(int16_t wx, int16_t wy, int16_t wp, int16_t *MaxPop, int16_t *production_bonus, int16_t *gold_bonus, int16_t *unit_cost_reduction, int16_t *gold_units, int16_t *magic_units, int16_t *have_nightshade, int16_t *have_mithril, int16_t *have_adamantium, int16_t *have_shore, int16_t *is_unexplored)
{
    int16_t city_idx = 0;
    int16_t itr = 0;
    int16_t map_square_has_city = 0;
    int16_t food2_units = 0;
    int16_t food2_remainder = 0;
    int16_t Bit_Index = 0;
    int16_t itr_wx = 0;
    int16_t itr_wy = 0;
    int16_t curr_wx = 0;

    EMMDATAH_Map();

    map_square_has_city = ST_FALSE;

    // look for a city at the provided coordinates, and save its index if there is one
    for(itr = 0; itr < _cities; itr++)
    {
        if(
            (_CITIES[itr].wx == wx)
            &&
            (_CITIES[itr].wy == wy)
            &&
            (_CITIES[itr].wp == wp)
        )
        {
            map_square_has_city = ST_TRUE;
            city_idx = itr;
        }
    }

    /* HACK */  /* CLAUDE */  /* An out-of-range wp (AI_Stacks_Do_Settle() passes an uninitialised one, BQ #150) matches no city above, as in the OG. Every later use is a map-table read, which the OG makes at wp * WORLD_SIZE far outside the tables (garbage values); ReMoM reads a stand-in plane instead. */
    if(
        (wp < ARCANUS_PLANE)
        ||
        (wp > MYRROR_PLANE)
    )
    {
        wp = Stack_Garbage_Range(ARCANUS_PLANE, MYRROR_PLANE);
    }


    /* Initialize output values */
    *MaxPop = 0;
    *production_bonus = 0;
    *gold_bonus = 0;
    *unit_cost_reduction = 0;
    *gold_units = 0;
    *magic_units = 0;
    *have_nightshade = 0;
    *have_mithril = 0;
    *have_adamantium = 0;
    *have_shore = 0;
    *is_unexplored = 0;

    food2_remainder = 0;

    /* Base gold bonus from the target square itself */
    *gold_bonus = Square_Gold_Bonus(wx, wy, wp);

    /* Iterate over the 5x5 city catchment area (21 squares) */
    for(itr_wy = (wy - 2); (itr_wy < (wy + 3)); itr_wy++)
    {
        /* OGBUG  skips map row 0 (itr_wy <= 0), where every other city-area loop accepts itr_wy >= 0: for a site at wy <= 2 the top-row squares are left out of the estimate */
        if(
            // (itr_wy < 0)
            (itr_wy <= 0)  /* CLAUDE */  /* REBUG */  /* asm: or di, di / jg (else skip) */
            ||
            (itr_wy >= WORLD_HEIGHT)
        )
        {
            continue;
        }
        for(itr_wx = (wx - 2); itr_wx < (wx + 3); itr_wx++)
        {

            if(
                (
                    (wy - 2) == itr_wy
                    &&
                    (
                        (wx - 2) == itr_wx
                        ||
                        (wx + 2) == itr_wx
                    )
                )
                ||
                (
                    (wy + 2) == itr_wy
                    &&
                    (
                        (wx - 2) == itr_wx
                        ||
                        (wx + 2) == itr_wx
                    )

                )
            )
            {
                continue;
            }

            curr_wx = itr_wx;

            if(curr_wx < 0)
            {
                curr_wx += WORLD_WIDTH;
            }

            if(curr_wx >= WORLD_WIDTH)
            {
                curr_wx -= WORLD_WIDTH;
            }

            /* OGBUG  _square_explored is the human's map; for an AI caller (AI_Stacks_Do_Settle), squares the human hasn't explored contribute nothing */
            if(SQUARE_UNEXPLORED(curr_wx, itr_wy, wp))
            {
                *is_unexplored = ST_TRUE;
                continue;
            }

            Bit_Index = ((itr_wy * WORLD_WIDTH) + curr_wx);

            if(
                (map_square_has_city != ST_FALSE)
                ||
                (Test_Bit_Field(Bit_Index, &city_area_bits[(wp * WORLD_SIZE)]) == 0)
            )
            {

                // *MaxPop = Square_Food_x2(curr_wx, itr_wy, wp);
                *MaxPop += Square_Food_x2(curr_wx, itr_wy, wp);  /* CLAUDE */  /* REBUG */  /* asm: add [bx], ax */

                if((GET_TERRAIN_SPECIAL(curr_wx, itr_wy, wp) & ts_Wild_Game) != 0)
                {
                    /* OGBUG  Wild Game adds 2 to a sum kept in quarter-food units (Square_Food_x2; *MaxPop / 4 below): +0.5 food where City_Food_WildGame gives +2, so the Surveyor / AI site value undervalues Wild Game 4x */
                    *MaxPop += 2;
                }

                *production_bonus += Square_Production_Bonus(curr_wx, itr_wy, wp, ST_FALSE);

                *unit_cost_reduction += Square_Unit_Cost_Reduction(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE);

                SETMAX(*unit_cost_reduction, 50);  // The bonus is cumulative ... up to a maximum reduction in cost of 50%.

                *gold_units += Square_Gold_Income(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE);

                *magic_units += Square_Magic_Power(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE);

            }
            else
            {

                food2_units = Square_Food_x2(curr_wx, itr_wy, wp);

                if((GET_TERRAIN_SPECIAL(curr_wx, itr_wy, wp) & ts_Wild_Game) != 0)
                {
                    /* OGBUG  Wild Game adds 2 to a sum kept in quarter-food units: +0.5 food where City_Food_WildGame gives +2 */
                    food2_units += 2;
                }

                *MaxPop += ((food2_units + food2_remainder) / 2);

                food2_remainder = ((food2_units + food2_remainder) % 2);

                // *production_bonus = (Square_Production_Bonus(curr_wx, itr_wy, wp, ST_FALSE) / 2);
                *production_bonus += (Square_Production_Bonus(curr_wx, itr_wy, wp, ST_FALSE) / 2);  /* CLAUDE */  /* REBUG */  /* asm: add [bx], ax */

                // *unit_cost_reduction = (Square_Unit_Cost_Reduction(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE) / 2);
                *unit_cost_reduction += (Square_Unit_Cost_Reduction(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE) / 2);  /* CLAUDE */  /* REBUG */  /* asm: add [bx], ax */

                SETMAX(*unit_cost_reduction, 50);  // The bonus is cumulative ... up to a maximum reduction in cost of 50%.

                // *gold_units = (Square_Gold_Income(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE) / 2);
                *gold_units += (Square_Gold_Income(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE) / 2);  /* CLAUDE */  /* REBUG */  /* asm: add [bx], ax */

                // *magic_units = (Square_Magic_Power(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE) / 2);
                *magic_units += (Square_Magic_Power(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE) / 2);  /* CLAUDE */  /* REBUG */  /* asm: loc_CA3EB add [bx], ax */

            }

            if((GET_TERRAIN_SPECIAL(curr_wx, itr_wy, wp) & ts_Nightshade) != 0)
            {
                *have_nightshade = ST_TRUE;
            }

            if(Square_Has_Mithril(curr_wx, itr_wy, wp) == ST_TRUE)
            {
                *have_mithril = ST_TRUE;
            }

            if(Square_Has_Adamantium(curr_wx, itr_wy, wp) == ST_TRUE)
            {
                /* ¿ OGBUG: Why does this unset have_mithril here ? */
                *have_adamantium = ST_TRUE;
                *have_mithril = ST_FALSE;
            }

        }

    }

    // HERE: *MaxPop is ...
    // *MaxPop = Square_Food_x2(curr_wx, itr_wy, wp);
    // *MaxPop += ((food2_units + food2_remainder) / 2);
    *MaxPop = (*MaxPop / 4);  // ¿ 2 food2 per population unit ?
    // HERE: *MaxPop is population

    if(map_square_has_city == ST_TRUE)
    {

        *MaxPop = City_Maximum_Size(city_idx);

        if(
            (_CITIES[city_idx].bldg_status[bt_ForestersGuild] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[bt_ForestersGuild] == bs_Replaced)
        )
        {
            *production_bonus += 25;
        }

        if(
            (_CITIES[city_idx].bldg_status[bt_Sawmill] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[bt_Sawmill] == bs_Replaced)
        )
        {
            *production_bonus += 25;
        }

        if(
            (_CITIES[city_idx].bldg_status[bt_MinersGuild] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[bt_MinersGuild] == bs_Replaced)
        )
        {
            *production_bonus += 50;
        }

        if(
            (_CITIES[city_idx].bldg_status[bt_MechaniciansGuild] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[bt_MechaniciansGuild] == bs_Replaced)
        )
        {
            *production_bonus += 50;
        }

        if(_CITIES[city_idx].enchantments[INSPIRATIONS] > 0)
        {
            *production_bonus += 100;
        }

        if(_CITIES[city_idx].race == rt_Nomad)
        {
            *gold_bonus += 50;
        }

        *gold_bonus += City_Road_Trade_Bonus(city_idx);

        // if((_CITIES[city_idx].population / 3) < *gold_bonus)
        if((_CITIES[city_idx].population * 3) < *gold_bonus)  /* CLAUDE */  /* REBUG */  /* asm: mov dx, 3 / imul dx */
        {
            // *gold_bonus = (_CITIES[city_idx].population / 3);
            *gold_bonus = (_CITIES[city_idx].population * 3);  /* CLAUDE */  /* REBUG */  /* asm: mov dx, 3 / imul dx */
        }

        if(
            (_CITIES[city_idx].bldg_status[bt_MerchantsGuild] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[bt_MerchantsGuild] == bs_Replaced)
        )
        {
            *gold_bonus += 100;
        }

        if(
            (_CITIES[city_idx].bldg_status[bt_Bank] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[bt_Bank] == bs_Replaced)
        )
        {
            *gold_bonus += 50;
        }

        if(
            (_CITIES[city_idx].bldg_status[bt_Marketplace] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[bt_Marketplace] == bs_Replaced)
        )
        {
            *gold_bonus += 50;
        }


        if(_CITIES[city_idx].enchantments[PROSPERITY] > 0)
        {
            *gold_bonus += 100;
        }

    }

    SETMAX(*MaxPop, 25);

    for(itr_wy = (wy - 1); (wy + 2) > itr_wy; itr_wy++)
    {

        /* OGBUG  skips map row 0 here too: a site at wy = 1 does not see a shore square in the top row */
        if(
            // (itr_wy < 0)
            (itr_wy <= 0)  /* CLAUDE */  /* REBUG */  /* asm: or di, di / jle (skip) */
            ||
            (itr_wy >= WORLD_HEIGHT)
        )
        {
            continue;
        }

        for(itr_wx = (wx - 1); (wx + 2) > itr_wx; itr_wx++)
        {

            curr_wx = itr_wx;

            if(curr_wx < 0)
            {
                curr_wx += WORLD_WIDTH;
            }

            if(curr_wx >= WORLD_WIDTH)
            {
                curr_wx -= WORLD_WIDTH;
            }

            if(SQUARE_EXPLORED(curr_wx, itr_wy, wp) != ST_FALSE)
            {

                if(Square_Is_Sailable(curr_wx, itr_wy, wp) != ST_FALSE)
                {
                    *have_shore = ST_TRUE;
                }

            }

        }

    }

}


// WZD o142p29
// drake178: CTY_CatchmentRefresh()
/*
loops over 300, as if to clear 300 bytes worth of bits?
but, does it per plane
so, should be 150 bytes worth of bits-per-plane
but, actually allocated as 150 paragraphs
so, bjorked, but fell into a functional sub-space of the problem-space
*/
void Reset_City_Area_Bitfields(void)
{
    int16_t city_area_bit_index;
    int16_t city_area_square_wy;
    int16_t city_area_square_wx;
    int16_t city_wy;
    int16_t city_wx;
    int16_t city_wp;
    int16_t city_area_y;
    int16_t itr_wp;
    int16_t itr_bits_bytes;  // _SI_  DNE in Dasm
    int16_t itr_cities;  // _SI_
    int16_t city_area_x;  // _DI_

    EMMDATAH_Map();

    // IDA Dasm - Grey-Dark
    for(itr_wp = 0; itr_wp < NUM_PLANES; itr_wp++)
    {
        for(itr_bits_bytes = 0; itr_bits_bytes < WORLD_MAP_BITFIELD_SIZE; itr_bits_bytes++)
        {
            *(city_area_bits        + (itr_wp * WORLD_SIZE) + itr_bits_bytes) = 0;
            *(city_area_shared_bits + (itr_wp * WORLD_SIZE) + itr_bits_bytes) = 0;
        }
    }

    // IDA Dasm - Purple-Grey
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        // city_wx = _CITIES[itr_cities].wx;
        city_wx = CITIESX();
        city_wy = CITIESY();
        city_wp = CITIESP();

        // TODO  There's a loop-around like this elsewhere? Macro? Notes? ¿ MoM-CityArea ?
        for(city_area_y = -2; city_area_y < 3; city_area_y++)
        {
            city_area_square_wy = (city_wy + city_area_y);

            if(
                (city_area_square_wy >= 0) &&
                (city_area_square_wy < WORLD_HEIGHT)
            )
            {
                for(city_area_x = -2; city_area_x < 3; city_area_x++)
                {
                    /* OGBUG  x is clipped, not wrapped: squares across the x=0/59 seam are never marked in city_area_bits / city_area_shared_bits */
                    city_area_square_wx = (city_wx+ city_area_x);

                    if(
                        (city_area_square_wx >= 0) &&
                        (city_area_square_wx < WORLD_WIDTH)
                    )
                    {
                        if(
                            (
                                (city_area_y != -2)
                                ||
                                (
                                    (city_area_x != -2)
                                    &&
                                    (city_area_x !=  2)
                                )
                            )
                            &&
                            (
                                (city_area_y !=  2)
                                ||
                                (
                                    (city_area_x != -2)
                                    &&
                                    (city_area_x !=  2)
                                )
                            )
                        )
                        {

                            city_area_bit_index = ((city_area_square_wy * WORLD_WIDTH) + city_area_square_wx);

                            if(Test_Bit_Field(city_area_bit_index, &city_area_bits[(city_wp * WORLD_SIZE)]) != ST_FALSE)
                            {
                                Set_Bit_Field(city_area_bit_index, &city_area_shared_bits[(city_wp * WORLD_SIZE)]);
                            }
                            // Eh? Should be in an `else { }`
                            Set_Bit_Field(city_area_bit_index, &city_area_bits[(city_wp * WORLD_SIZE)]);

                        }

                    }

                }

            }

        }

    }

}


// WZD o142p30
/*
"shared map square"
*/
/**
 * @brief Tests whether a world map square is marked as shared by city catchments.
 *
 * @details
 * Converts the provided world coordinates into the per-plane bit index used by
 * the city-area shared bitfield and queries that bit via @c Test_Bit_Field().
 *
 * Bit index mapping:
 * @code
 * city_area_bit_index = (city_area_square_wy * WORLD_WIDTH) + city_area_square_wx
 * @endcode
 *
 * The bit is looked up in @c city_area_shared_bits for the selected plane using
 * @c (city_wp * WORLD_SIZE) as the plane offset.
 *
 * @param city_area_square_wx World X coordinate of the square to test.
 * @param city_area_square_wy World Y coordinate of the square to test.
 * @param city_wp World plane index containing the tested square.
 *
 * @return @c ST_TRUE if the square is flagged as shared; otherwise @c ST_FALSE.
 *
 * @note This is a pure query helper; it does not modify bitfields.
 * @note Shared-bit population is performed by @c Reset_City_Area_Bitfields().
 *
 * @see Reset_City_Area_Bitfields(), Test_Bit_Field(), Get_Useable_City_Area()
 */
int16_t City_Area_Square_Is_Shared(int16_t city_area_square_wx, int16_t city_area_square_wy, int16_t city_wp)
{
    int16_t city_area_bit_index = 0;
    int16_t is_shared = 0;
    city_area_bit_index = ((city_area_square_wy * WORLD_WIDTH) + city_area_square_wx);
    if(Test_Bit_Field(city_area_bit_index, &city_area_shared_bits[(city_wp * WORLD_SIZE)]) == ST_FALSE)
    {
        is_shared = ST_FALSE;
    }
    else
    {
        is_shared = ST_TRUE;
    }
    return is_shared;
}


// WZD o142p31
/*
    ~ Remove City :: Remove Road
    unsets road flags - normal, enchanted
    resets movement mode cost map

OON XREF: CTY_Remove()
*/
void City_Remove_Road(int16_t wx, int16_t wy, int16_t wp)
{

    EMMDATAH_Map();

    // movement_mode_cost_maps + (wp * sizeof(struct s_MOVE_MODE_COST_MAPS)) + (wy * WORLD_WIDTH) + wx
    /* OGBUG  sets the square's cost on movement maps 0-4 to 2 (1 move point) whatever its terrain: a city removed on hills / mountains / forest / swamp leaves a square that costs 1 MP for good (the cost maps are saved with the game) */
    movement_mode_cost_maps[wp].UU_MvMd.moves2[((wy * WORLD_WIDTH) + wx)] = 2;
    movement_mode_cost_maps[wp].walking.moves2[((wy * WORLD_WIDTH) + wx)] = 2;
    movement_mode_cost_maps[wp].forester.moves2[((wy * WORLD_WIDTH) + wx)] = 2;
    movement_mode_cost_maps[wp].mountaineer.moves2[((wy * WORLD_WIDTH) + wx)] = 2;
    movement_mode_cost_maps[wp].swimming.moves2[((wy * WORLD_WIDTH) + wx)] = 2;

    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] &= 0xF7;  // not MSF_ROAD

    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] &= 0xEF;  // not TF_Enc_Road

}


// WZD o142p32
// drake178: CTY_Food_Breakdown()
/*
; fills the passed return structure with the food
; income breakdown values of the city
*/
/*
City Screen
Resource Window
Food Breakdown
returns 0 for a city with no population; otherwise the OG returns whatever AX holds (City_Food_WildGame()'s result; no return value)
*/
int16_t City_Food_Breakdown(int16_t city_idx, struct s_FOOD_BREAKDOWN * breakdown)
{
    int16_t farmed_food_total = 0;
    int16_t terrain_food = 0;
    int16_t itr = 0;
    int16_t food_subtotal = 0;

    EMMDATAH_Map();

    for(itr = 0; itr < RESOURCE_BREAKDOWN_LINES; itr++)
    {

        ((int16_t *)breakdown)[itr] = 0;

    }

    if(_CITIES[city_idx].population == 0)
    {

        return 0;

    }

    terrain_food = City_Food_Terrain(city_idx);

    if(_CITIES[city_idx].farmer_count > _CITIES[city_idx].population)
    {

        CITIES_FARMER_COUNT(city_idx, _CITIES[city_idx].population);
        
    }

    if(
        (_CITIES[city_idx].race == rt_Halfling)
        ||
        (
            (_CITIES[city_idx].bldg_status[bt_AnimistsGuild] == bs_Built)
            ||
            (_CITIES[city_idx].bldg_status[bt_AnimistsGuild] == bs_Replaced)
        )
    )
    {

        food_subtotal = (_CITIES[city_idx].farmer_count * 3);

    }
    else
    {

        food_subtotal = (_CITIES[city_idx].farmer_count * 2);

    }

    if(_CITIES[city_idx].enchantments[GAIAS_BLESSING] != 0)
    {

        terrain_food = ((terrain_food * 2) / 3);

    }

    if(food_subtotal > terrain_food)
    {

        // Food_Sub_Sum = (Tile_Food + (Food_Sub_Sum - Tile_Food));
        food_subtotal = (terrain_food + ((food_subtotal - terrain_food) / 2));  /* CLAUDE */  /* REBUG */  /* asm: sub ax, Tile_Food / cwd / sub ax, dx / sar ax, 1 */

    }

    farmed_food_total = food_subtotal;


    if(_CITIES[city_idx].race == rt_Halfling)
    {

        // Food_Sub_Sum = (_CITIES[city_idx].farmer_count / 3);
        food_subtotal = (_CITIES[city_idx].farmer_count * 3);  /* CLAUDE */  /* REBUG */  /* asm: cbw / mov dx, 3 / imul dx */

    }
    else
    {

        // Food_Sub_Sum = (_CITIES[city_idx].farmer_count / 2);
        food_subtotal = (_CITIES[city_idx].farmer_count * 2);  /* CLAUDE */  /* REBUG */  /* asm: cbw / shl ax, 1 */

    }

    /* OGBUG  the Farmers line is halved over Tile_Food, and Tile_Food here has no Gaia's Blessing (it was cut to 2/3 above); City_Food_Production halves only after Foresters' Guild +2 and Famine /2 */
    if(food_subtotal > terrain_food)
    {

        // Food_Sub_Sum = (Tile_Food + (Food_Sub_Sum - Tile_Food));
        food_subtotal = (terrain_food + ((food_subtotal - terrain_food) / 2));  /* CLAUDE */  /* REBUG */  /* asm: sub ax, Tile_Food / cwd / sub ax, dx / sar ax, 1 */

    }

    breakdown->Farmers = food_subtotal;

    breakdown->Animists_Guild = (farmed_food_total - food_subtotal);

    if(
        (_CITIES[city_idx].bldg_status[bt_ForestersGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_ForestersGuild] == bs_Replaced)
    )
    {

        /* OGBUG  shows Foresters' Guild as 1 when the Farmers line is over Tile_Food; City_Food_Production always adds 2 */
        if(food_subtotal > terrain_food)
        {

            breakdown->Foresters_Guild = 1;

        }
        else
        {

            breakdown->Foresters_Guild = 2;

        }

        food_subtotal += 2;

    }

    if(_CITIES[city_idx].enchantments[GAIAS_BLESSING] != 0)
    {

        terrain_food = City_Food_Terrain(city_idx);

        if(
            (_CITIES[city_idx].race == rt_Halfling)
            ||
            (
                (_CITIES[city_idx].bldg_status[bt_AnimistsGuild] == bs_Built)
                ||
                (_CITIES[city_idx].bldg_status[bt_AnimistsGuild] == bs_Replaced)
            )
        )
        {

            // Food_Sub_Sum = (_CITIES[city_idx].farmer_count * 3);
            /* OGBUG  Halflings / Animists' Guild: ADDS farmer_count * 3 on top of the running Farmers (+ Foresters' Guild 2) total instead of setting it, so the Gaia's Blessing line double-counts the farmers */
            food_subtotal += (_CITIES[city_idx].farmer_count * 3);  /* CLAUDE */  /* REBUG */  /* asm: imul dx / add [bp+Food_Sub_Sum], ax */

        }
        else
        {

            food_subtotal = (_CITIES[city_idx].farmer_count * 2);

        }

        if(food_subtotal > terrain_food)
        {

            // Food_Sub_Sum = (Tile_Food + (Food_Sub_Sum - Tile_Food));
            food_subtotal = (terrain_food + ((food_subtotal - terrain_food) / 2));  /* CLAUDE */  /* REBUG */  /* asm: sub ax, Tile_Food / cwd / sub ax, dx / sar ax, 1 */

        }

        breakdown->Gaias_Blessing = (food_subtotal - breakdown->Farmers - breakdown->Animists_Guild);

    }

    /* OGBUG  shows Famine as half of the running total (the Farmers line + Foresters' Guild 2, or the Gaia's Blessing sum), not of the real pre-cap food */
    if(_CITIES[city_idx].enchantments[FAMINE] > 0)
    {

        breakdown->Famine = (food_subtotal / 2);

    }

    /* CLAUDE */  /* asm: bldg_status+1Dh (Granary) -> Granary = 2; was missing */
    if(
        (_CITIES[city_idx].bldg_status[bt_Granary] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_Granary] == bs_Replaced)
    )
    {

        breakdown->Granary = 2;

    }

    if(
        // (_CITIES[city_idx].bldg_status[bt_ForestersGuild] == bs_Built)
        (_CITIES[city_idx].bldg_status[bt_FarmersMarket] == bs_Built)  /* CLAUDE */  /* REBUG */  /* asm: bldg_status+1Eh */
        ||
        // (_CITIES[city_idx].bldg_status[bt_ForestersGuild] == bs_Replaced)
        (_CITIES[city_idx].bldg_status[bt_FarmersMarket] == bs_Replaced)  /* CLAUDE */  /* REBUG */  /* asm: bldg_status+1Eh */
    )
    {

        breakdown->Farmers_Market = 3;

    }

    breakdown->Wild_Game = City_Food_WildGame(city_idx);

    /* HACK */  return 0;  /* OG: no return value - falls off the end with City_Food_WildGame()'s AX */

}


// WZD o142p33
// drake178: CTY_Gold_Breakdown()
/*
City Screen
Resource Window
Gold Breakdown
returns 0 for a city with no population; otherwise the OG returns whatever AX holds (no return value)
*/
int16_t City_Gold_Breakdown(int16_t city_idx, struct s_GOLD_BREAKDOWN * breakdown)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t mineral_gold = 0;
    int16_t trade_percent = 0;
    int16_t are_dwarves = 0;
    int16_t have_miners_guild = 0;
    int16_t city_wp = 0;
    int16_t city_owner_idx = 0;
    int16_t gold_income = 0;
    int16_t useable_map_squares = 0;
    int16_t itr = 0;  // _DI_

    EMMDATAH_Map();

    for(itr = 0; itr < RESOURCE_BREAKDOWN_LINES; itr++)
    {

        ((int16_t *)breakdown)[itr] = 0;

    }

    if(_CITIES[city_idx].population == 0)
    {

        return 0;

    }

    city_owner_idx = _CITIES[city_idx].owner_idx;

    city_wp = _CITIES[city_idx].wp;

    useable_map_squares = Get_Useable_City_Area(_CITIES[city_idx].wx, _CITIES[city_idx].wy, city_wp, &wx_array[0], &wy_array[0]);

    if(city_owner_idx == NEUTRAL_PLAYER_IDX)
    {

        gold_income = (_CITIES[city_idx].population - City_Rebel_Count(city_idx));

    }
    else
    {

        gold_income = (((_CITIES[city_idx].population - City_Rebel_Count(city_idx)) * _players[city_owner_idx].tax_rate) / 2);

    }

    breakdown->Taxes = gold_income;

    if(_CITIES[city_idx].race == rt_Dwarf)
    {

        are_dwarves = ST_TRUE;

    }
    else
    {

        are_dwarves = ST_FALSE;

    }

    mineral_gold = 0;

    for(itr = 0; itr < useable_map_squares; itr++)
    {

        mineral_gold += Square_Silver_Gold_Income(wx_array[itr], wy_array[itr], city_wp, ST_FALSE, are_dwarves);

    }

    breakdown->Silver_Ores = mineral_gold;

    mineral_gold = 0;

    for(itr = 0; itr < useable_map_squares; itr++)
    {

        mineral_gold += Square_Gold_Ore_Gold_Income(wx_array[itr], wy_array[itr], city_wp, ST_FALSE, are_dwarves);

    }

    breakdown->Gold_Ores = mineral_gold;

    mineral_gold = 0;

    for(itr = 0; itr < useable_map_squares; itr++)
    {

        mineral_gold += Square_Gems_Gold_Income(wx_array[itr], wy_array[itr], city_wp, ST_FALSE, are_dwarves);

    }

    breakdown->Gems = mineral_gold;

    if(
        (_CITIES[city_idx].bldg_status[bt_MinersGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_MinersGuild] == bs_Replaced)
    )
    {

        have_miners_guild = ST_TRUE;

    }
    else
    {

        have_miners_guild = ST_FALSE;

    }

    if(have_miners_guild == ST_TRUE)
    {

        breakdown->Miners_Guild = ((breakdown->Silver_Ores + breakdown->Gold_Ores + breakdown->Gems) / 2);

    }

    /* NOTE  gold_income collects the minerals here (and Trade Goods below) but is read only by the Road/River/Ocean, Marketplace and Bank lines; the function never returns it */
    for(itr = 0; itr < useable_map_squares; itr++)
    {

        gold_income += Square_Silver_Gold_Income(wx_array[itr], wy_array[itr], city_wp, have_miners_guild, are_dwarves);

        gold_income += Square_Gold_Ore_Gold_Income(wx_array[itr], wy_array[itr], city_wp, have_miners_guild, are_dwarves);

        gold_income += Square_Gems_Gold_Income(wx_array[itr], wy_array[itr], city_wp, have_miners_guild, are_dwarves);

    }

    if(_CITIES[city_idx].race == rt_Dwarf)
    {

        breakdown->Taxes += breakdown->Taxes;

    }

    trade_percent = (Square_Gold_Bonus(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) + City_Road_Trade_Bonus(city_idx));

    if(_CITIES[city_idx].race == rt_Nomad)
    {

        trade_percent += 50;

    }

    if((_CITIES[city_idx].population * 3) < trade_percent)
    {

        trade_percent = (_CITIES[city_idx].population * 3);

    }

    /* OGBUG  (#43) the Road/River/Ocean, Marketplace and Bank lines apply their percentages to gold_income, which has the taxes once: City_Gold_Production() doubles a Dwarf city's taxes BEFORE the modifiers, so for Dwarves these lines show less than the real gain */
    breakdown->Road_River_Ocean = ((trade_percent * gold_income) / 100);

    trade_percent = 0;  /* OGBUG  dead store - trade_percent is not read again */

    if(
        (_CITIES[city_idx].bldg_status[bt_Marketplace] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_Marketplace] == bs_Replaced)
    )
    {

        breakdown->Marketplace = ((50 * gold_income) / 100);

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_Bank] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_Bank] == bs_Replaced)
    )
    {

        breakdown->Bank = ((50 * gold_income) / 100);

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_MerchantsGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_MerchantsGuild] == bs_Replaced)  /* asm: cmp ..., 0 */
    )
    {

        /* OGBUG  (#43) shows the Taxes line; City_Gold_Production() adds the Merchants' Guild as +100% of the whole base (taxes, x2 for Dwarves, and minerals) */
        breakdown->Merchants_Guild = breakdown->Taxes;

    }

    if(_CITIES[city_idx].enchantments[PROSPERITY] > 0)
    {

        /* OGBUG  (#43) for Dwarves the Taxes line is already doubled, and the line below adds it again: 4 x the base taxes, where City_Gold_Production()'s +100% counts them twice */
        breakdown->Prosperity = (breakdown->Taxes + breakdown->Silver_Ores + breakdown->Gold_Ores + breakdown->Gems + breakdown->Miners_Guild);

        if(_CITIES[city_idx].race == rt_Dwarf)
        {

            breakdown->Prosperity += breakdown->Taxes;

        }

    }

    if(_CITIES[city_idx].construction == bt_TradeGoods)
    {

        gold_income += (_CITIES[city_idx].production_units / 2);  /* OGBUG  dead store - gold_income is not read again (the function never returns it) */

        breakdown->Trade_Goods = (_CITIES[city_idx].production_units / 2);

    }

    /* HACK */  return 0;  /* OG: no return value - falls off the end with whatever AX holds */

}


// WZD o142p34
// drake178: CTY_Prod_Breakdown()
/*
City Screen
Resource Window
Production Breakdown

returns 0 for a city with no population; otherwise the terrain-bonus production (terrain_percent x base_production / 100)
*/
int16_t City_Production_Breakdown(int16_t city_idx, struct s_PRODUCTION_BREAKDOWN * breakdown)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t gaias_terrain_percent = 0;
    int16_t worker_production_x2 = 0;
    int16_t have_gaias_blessing = 0;
    int16_t city_wp = 0;
    int16_t city_owner_idx = 0;
    int16_t base_production = 0;
    int16_t worker_count = 0;
    int16_t itr = 0;
    int16_t terrain_percent = 0;
    int16_t useable_map_squares = 0;

    EMMDATAH_Map();

    for(itr = 0; itr < RESOURCE_BREAKDOWN_LINES; itr++)
    {

        ((int16_t *)breakdown)[itr] = 0;

    }

    if(_CITIES[city_idx].population == 0)
    {

        return 0;

    }

    city_owner_idx = _CITIES[city_idx].owner_idx;  /* OGBUG  dead store - city_owner_idx is never read */

    city_wp = _CITIES[city_idx].wp;

    useable_map_squares = Get_Useable_City_Area(_CITIES[city_idx].wx, _CITIES[city_idx].wy, city_wp, &wx_array[0], &wy_array[0]);

    if(_CITIES[city_idx].farmer_count > _CITIES[city_idx].population)
    {

        CITIES_FARMER_COUNT(city_idx, _CITIES[city_idx].population);

    }

    worker_count = ((_CITIES[city_idx].population - _CITIES[city_idx].farmer_count) - City_Rebel_Count(city_idx));

    if(
        (_CITIES[city_idx].race == rt_Dwarf)
        ||
        (_CITIES[city_idx].race == rt_Klackon)
    )
    {

        worker_production_x2 = 6;

    }
    else
    {

        worker_production_x2 = 4;

    }

    base_production = (((worker_count * worker_production_x2) + _CITIES[city_idx].farmer_count + 1) / 2);

    breakdown->Workers = ((worker_count * worker_production_x2) / 2);

    breakdown->Farmers = (base_production - breakdown->Workers);

    have_gaias_blessing = ST_FALSE;

    terrain_percent = 0;

    for(itr = 0; itr < useable_map_squares; itr++)
    {

        terrain_percent += Square_Production_Bonus(wx_array[itr], wy_array[itr], city_wp, have_gaias_blessing);

    }

    /* OGBUG  (#43) each percentage line (Terrain, Gaia's Blessing, Foresters' Guild, Sawmill, Miners' Guild, Mechanicians' Guild) is truncated on its own, where City_Production_Production() divides the summed modifier once: the lines can add up to less than the real production */
    breakdown->Terrain = ((terrain_percent * base_production) / 100);

    if(_CITIES[city_idx].enchantments[GAIAS_BLESSING] > 0)
    {

        have_gaias_blessing = ST_TRUE;

    }
    else
    {

        have_gaias_blessing = ST_FALSE;

    }

    gaias_terrain_percent = 0;

    for(itr = 0; itr < useable_map_squares; itr++)
    {

        gaias_terrain_percent += Square_Production_Bonus(wx_array[itr], wy_array[itr], city_wp, have_gaias_blessing);

    }

    breakdown->Gaias_Blessing = (((gaias_terrain_percent * base_production) / 100) - breakdown->Terrain);

    if(
        (_CITIES[city_idx].bldg_status[bt_ForestersGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_ForestersGuild] == bs_Replaced)
    )
    {

        breakdown->Foresters_Guild = ((25 * base_production) / 100);

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_Sawmill] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_Sawmill] == bs_Replaced)
    )
    {

        breakdown->Sawmill = ((25 * base_production) / 100);

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_MinersGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_MinersGuild] == bs_Replaced)
    )
    {

        breakdown->Miners_Guild = ((50 * base_production) / 100);

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_MechaniciansGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_MechaniciansGuild] == bs_Replaced)
    )
    {

        breakdown->Mechanicians_Guild = ((50 * base_production) / 100);

    }

    if(_CITIES[city_idx].enchantments[INSPIRATIONS] > 0)
    {

        terrain_percent += 100;

        breakdown->Inspirations = (breakdown->Workers + breakdown->Farmers);

    }

    /* NOTE  the return value is terrain_percent x base_production / 100: the terrain bonus only, with no 100% base (unless Inspirations added it) and no building bonuses */
    base_production = ((terrain_percent * base_production) / 100);

    if(_CITIES[city_idx].enchantments[CURSED_LANDS] > 0)
    {

        /* OGBUG  (#43) shows the line sum / 2, rounded down; City_Production_Production() takes (production + 1) / 2 */
        breakdown->Cursed_Lands = ((breakdown->Workers + breakdown->Farmers + breakdown->Terrain + breakdown->Sawmill + breakdown->Foresters_Guild + breakdown->Miners_Guild + breakdown->Mechanicians_Guild + breakdown->Gaias_Blessing + breakdown->Inspirations) / 2);

    }

    return base_production;

}


// WZD o142p35
// drake178: CTY_Rsrc_Breakdown()
/*
City Screen
Resource Window
Research Breakdown

returns 0 for a city with no population or a neutral city; otherwise the OG returns whatever AX holds (no return value)
*/
int16_t City_Research_Breakdown(int16_t city_idx, int16_t * breakdown)
{
    int16_t city_owner_idx = 0;
    int16_t itr = 0;

    EMMDATAH_Map();

    for(itr = 0; itr < RESOURCE_BREAKDOWN_LINES; itr++)
    {

        breakdown[itr] = 0;

    }

    city_owner_idx = _CITIES[city_idx].owner_idx;

    if(
        (_CITIES[city_idx].population == 0)
        ||
        (city_owner_idx == NEUTRAL_PLAYER_IDX)
    )
    {

        return 0;

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_Library] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_Library] == bs_Replaced)
    )
    {

        breakdown[0] = 2;

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_SagesGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_SagesGuild] == bs_Replaced)
    )
    {

        breakdown[1] = 3;

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_University] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_University] == bs_Replaced)
    )
    {

        breakdown[2] = 5;

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_WizardsGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_WizardsGuild] == bs_Replaced)
    )
    {

        breakdown[3] = 8;

    }

    /* HACK */  return 0;  /* OG: no return value - falls off the end with whatever AX holds */

}


// WZD o142p36
// drake178: CTY_Pwr_Breakdown()
/*
City Screen
Resource Window
Power Breakdown

returns 0 during a Mana Short event, for a city with no population, or for a neutral city; otherwise the OG returns whatever AX holds (no return value)
*/
int16_t City_Power_Breakdown(int16_t city_idx, struct s_POWER_BREAKDOWN * breakdown)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t fortress_power = 0;
    int16_t mineral_power = 0;
    int16_t have_miners_guild = 0;
    int16_t are_dwarves = 0;
    int16_t city_wp = 0;
    int16_t useable_map_squares = 0;
    int16_t religious_power = 0;
    int16_t religion_percent = 0;
    int16_t racial_power = 0;
    int16_t city_owner_idx = 0;
    int16_t itr = 0;

    EMMDATAH_Map();

    for(itr = 0; itr < RESOURCE_BREAKDOWN_LINES; itr++)
    {

        ((int16_t *)breakdown)[itr] = 0;

    }

    city_owner_idx = _CITIES[city_idx].owner_idx;

    if(events_table->Mana_Short_Status == es_Ongoing)
    {

        return 0;

    }

    if(
        (_CITIES[city_idx].population == 0)
        ||
        (city_owner_idx == NEUTRAL_PLAYER_IDX)
    )
    {

        return 0;

    }

    religious_power = 0;

    religion_percent = 100;

    if(
        (_players[city_owner_idx].divine_power != 0)
        ||
        (_players[city_owner_idx].infernal_power != 0)
    )
    {

        religion_percent = 150;

    }

    // /* NOTE  the Shrine line is a flat 1, while Temple / Parthenon / Cathedral are scaled by Divine / Infernal Power; the religious_power total below scales the Shrine too */
    /* OGBUG  (#43) the Shrine line is a flat 1, not x1.5 for Divine / Infernal Power, and the Temple / Parthenon / Cathedral lines are each truncated on their own; the religious_power total below (and City_Mana_Production()) scales the summed buildings once */
    if(
        (_CITIES[city_idx].bldg_status[bt_Shrine] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_Shrine] == bs_Replaced)
    )
    {

        breakdown->Shrine = 1;

        religious_power++;

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_Temple] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_Temple] == bs_Replaced)
    )
    {

        breakdown->Temple = ((religion_percent * 2) / 100);

        religious_power += 2;

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_Parthenon] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_Parthenon] == bs_Replaced)
    )
    {

        breakdown->Parthenon = ((religion_percent * 3) / 100);

        religious_power += 3;

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_Cathedral] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_Cathedral] == bs_Replaced)
    )
    {

        breakdown->Cathedral = ((religion_percent * 4) / 100);

        religious_power += 4;

    }

    religious_power = ((religious_power * religion_percent) / 100);

    if(_CITIES[city_idx].enchantments[DARK_RITUALS] != 0)
    {

        breakdown->Dark_Rituals = religious_power;

        religious_power = (religious_power * 2);  /* OGBUG  dead store - religious_power is not read again */

    }

    if(
        (
            (_players[city_owner_idx].spellranks[sbr_Death] != 0)
            &&
            (events_table->Good_Moon_Status == es_Ongoing)
        )
        ||
        (
            (_players[city_owner_idx].spellranks[sbr_Life] != 0)
            &&
            (events_table->Bad_Moon_Status == es_Ongoing)
        )
    )
    {

        breakdown->Shrine = ((breakdown->Shrine + 1) / 2);

        breakdown->Temple = ((breakdown->Temple + 1) / 2);

        breakdown->Parthenon = ((breakdown->Parthenon + 1) / 2);

        breakdown->Cathedral = ((breakdown->Cathedral + 1) / 2);

        breakdown->Dark_Rituals = ((breakdown->Dark_Rituals + 1) / 2);

    }

    if(
        (
            (_players[city_owner_idx].spellranks[sbr_Life] != 0)
            &&
            (events_table->Good_Moon_Status == es_Ongoing)
        )
        ||
        (
            (_players[city_owner_idx].spellranks[sbr_Death] != 0)
            &&
            (events_table->Bad_Moon_Status == es_Ongoing)
        )
    )
    {

        breakdown->Shrine += breakdown->Shrine;

        breakdown->Temple += breakdown->Temple;

        breakdown->Parthenon += breakdown->Parthenon;

        breakdown->Cathedral += breakdown->Cathedral;

        breakdown->Dark_Rituals += breakdown->Dark_Rituals;

    }

    // /* OGBUG  inverted: the Evil Presence line is filled only when the owner HAS Death books, the one case where Evil Presence takes nothing away; the real religious-power calculation removes religion when the owner has NO Death books */
    /* OGBUG  (#43) a display-only line: City_Mana_Production() has no Evil Presence term (in the asm only City_Rebel_Count() tests it among the turn calculations); and the line is filled only when the owner HAS Death books, the inverse of the spell's rule */
    if(
        (_CITIES[city_idx].enchantments[EVIL_PRESENCE] != 0)
        &&
        (_players[city_owner_idx].spellranks[sbr_Death] != 0)
    )
    {

        breakdown->Evil_Presence = (breakdown->Shrine + breakdown->Temple + breakdown->Parthenon + breakdown->Cathedral + breakdown->Dark_Rituals);

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_AlchemistsGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_AlchemistsGuild] == bs_Replaced)
    )
    {

        breakdown->Alchemists_Guild = 3;

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_WizardsGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_WizardsGuild] == bs_Replaced)
    )
    {

        breakdown->Wizards_Guild = 3;

    }

    city_wp = _CITIES[city_idx].wp;

    racial_power = 0;

    if(_CITIES[city_idx].race == rt_Dark_Elf)
    {

        racial_power += _CITIES[city_idx].population;

    }

    if(
        (_CITIES[city_idx].race == rt_High_Elf)
        ||
        (_CITIES[city_idx].race == rt_Beastmen)
        ||
        (_CITIES[city_idx].race == rt_Draconian)
    )
    {

        racial_power += (_CITIES[city_idx].population / 2);

    }

    breakdown->Townsfolk = racial_power;

    if(
        (_FORTRESSES[city_owner_idx].wx == _CITIES[city_idx].wx)
        &&
        (_FORTRESSES[city_owner_idx].wy == _CITIES[city_idx].wy)
        &&
        (_FORTRESSES[city_owner_idx].wp == city_wp)
    )
    {

        fortress_power = (_players[city_owner_idx].spellranks[sbr_Nature] + _players[city_owner_idx].spellranks[sbr_Sorcery] + _players[city_owner_idx].spellranks[sbr_Chaos] + _players[city_owner_idx].spellranks[sbr_Life] + _players[city_owner_idx].spellranks[sbr_Death]);

        breakdown->Fortress = ((city_wp == ARCANUS_PLANE) ? fortress_power : (fortress_power + 5));

    }

    if(_CITIES[city_idx].race == rt_Dwarf)
    {

        are_dwarves = ST_TRUE;

    }
    else
    {

        are_dwarves = ST_FALSE;

    }

    useable_map_squares = Get_Useable_City_Area(_CITIES[city_idx].wx, _CITIES[city_idx].wy, city_wp, &wx_array[0], &wy_array[0]);

    for(itr = 0; itr < useable_map_squares; itr++)
    {

        breakdown->Mithril += Square_Mithril_Power(wx_array[itr], wy_array[itr], city_wp, ST_FALSE, are_dwarves);

        breakdown->Adamantium += Square_Adamantium_Power(wx_array[itr], wy_array[itr], city_wp, ST_FALSE, are_dwarves);

        breakdown->Quork += Square_Quork_Power(wx_array[itr], wy_array[itr], city_wp, ST_FALSE, are_dwarves);

        breakdown->Crysx += Square_Crysx_Power(wx_array[itr], wy_array[itr], city_wp, ST_FALSE, are_dwarves);

    }

    mineral_power = 0;

    for(itr = 0; itr < useable_map_squares; itr++)
    {

        mineral_power += Square_Magic_Power(wx_array[itr], wy_array[itr], city_wp, ST_FALSE, are_dwarves);

    }

    if(
        (_CITIES[city_idx].bldg_status[bt_MinersGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_MinersGuild] == bs_Replaced)
    )
    {

        have_miners_guild = ST_TRUE;

    }
    else
    {

        have_miners_guild = ST_FALSE;

    }

    racial_power = 0;  /* asm reuses this local for the mineral power with the Miners' Guild */

    for(itr = 0; itr < useable_map_squares; itr++)
    {

        racial_power += Square_Magic_Power(wx_array[itr], wy_array[itr], city_wp, have_miners_guild, are_dwarves);

    }

    breakdown->Miners_Guild = (racial_power - mineral_power);

    /* HACK */  return 0;  /* OG: no return value - falls off the end with whatever AX holds */

}
