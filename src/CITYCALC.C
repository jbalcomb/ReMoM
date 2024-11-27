/*
    City Calculations

    WIZARDS.EXE
        ovr120
        ovr142
*/

#include "MoM.H"

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

    // drake178: ; maps the EMM Data block into the page frame
    // TODO  EMM_Map_DataH();

    _CITIES[city_idx].food_units            = City_Food_Production(city_idx);
    _CITIES[city_idx].production_units      = City_Production_Production(city_idx);
    _CITIES[city_idx].gold_units            = City_Gold_Production(city_idx);
    _CITIES[city_idx].building_maintenance  = City_Gold_Mainanence(city_idx);
    _CITIES[city_idx].research_units        = City_Research_Production(city_idx);
    _CITIES[city_idx].mana_units            = City_Mana_Production(city_idx);

    if((_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX) && (_CITIES[city_idx].owner_idx != NEUTRAL_PLAYER_IDX))
    {
        _CITIES[city_idx].food_units = ((_CITIES[city_idx].food_units * difficulty_modifiers_table[_difficulty].food) / 100);
    }

}


// WZD o120p02
/*
    sets
    _players[].magic_power

    nodes
        conjunctions
        mastery

    sum of s_CITY.mana_units

    _players[].volcanoes

    _players.casting_spell_idx Spell_Of_Return

*/
void Players_Update_Magic_Power(void)
{
    int16_t NIU_players_power_base_array[NUM_PLAYERS];
    int16_t node_owner_idx;

    int16_t itr;  // _SI_
    int16_t node_magic_power_points;  // _DI_

    for(itr = 0; itr < _num_players; itr++)
    {
        _players[itr].Power_Base = 0;
        NIU_players_power_base_array[itr] = 0;
    }

    if(events_table->Mana_Short_Status == 2)
    {
        return;
    }

    for(itr = 0; itr < NUM_NODES; itr++)
    {
        if(_NODES[itr].owner_idx > -1)
        {
            node_owner_idx = _NODES[itr].owner_idx;

            if((_NODES[itr].Meld_Flags & 0x01) > 0)   /* M_Warped */
            {
                _players[node_owner_idx].Power_Base -= 5;
                continue;
            }

            node_magic_power_points = ((_NODES[itr].power * (_magic + 1)) / 2);

            if(events_table->Conjunction_Chaos_Status == 2)
            {
                if(_NODES[itr].type == nt_Sorcery)
                    node_magic_power_points /= 2;
                if(_NODES[itr].type == nt_Nature)
                    node_magic_power_points /= 2;
                if(_NODES[itr].type == nt_Chaos)
                    node_magic_power_points *= 2;
            }

            if(events_table->Conjunction_Sorcery_Status == 2)
            {
                if(_NODES[itr].type == nt_Sorcery)
                    node_magic_power_points *= 2;
                if(_NODES[itr].type == nt_Nature)
                    node_magic_power_points /= 2;
                if(_NODES[itr].type == nt_Chaos)
                    node_magic_power_points /= 2;
            }

            if(events_table->Conjunction_Nature_Status == 2)
            {
                if(_NODES[itr].type == nt_Sorcery)
                    node_magic_power_points /= 2;
                if(_NODES[itr].type == nt_Nature)
                    node_magic_power_points *= 2;
                if(_NODES[itr].type == nt_Chaos)
                    node_magic_power_points /= 2;
            }

            if((_NODES[itr].type == nt_Sorcery) && (_players[node_owner_idx].sorcery_mastery))
                node_magic_power_points *= 2;
            if((_NODES[itr].type == nt_Chaos) && (_players[node_owner_idx].chaos_mastery))
                node_magic_power_points *= 2;
            if((_NODES[itr].type == nt_Nature) && (_players[node_owner_idx].nature_mastery))
                node_magic_power_points *= 2;
            if(_players[node_owner_idx].node_mastery)
                node_magic_power_points *= 2;

            _players[node_owner_idx].Power_Base += node_magic_power_points;

        }  /* if(_NODES[itr].owner_idx > -1) */
    }  /* for(itr = 0; itr < NUM_NODES; itr++) */

    for(itr = 1; itr < _num_players; itr++)
    {
        _players[itr].Power_Base = ((_players[itr].Power_Base * difficulty_modifiers_table[_difficulty].mana) / 100);  // e.g., * 150 / 100 ~== * 1.5 ~== +50%
    }

    for(itr = 0; itr < _cities; itr++)
    {
        if((_CITIES[itr].owner_idx > -1) && (_CITIES[itr].owner_idx != NEUTRAL_PLAYER_IDX))
        {
            _players[_CITIES[itr].owner_idx].Power_Base += _CITIES[itr].mana_units;
        }
    }

    for(itr = 0; itr < _num_players; itr++)
    {
        if(_players[itr].casting_spell_idx == 214)  /* Spell_Of_Return */
        {
            _players[itr].Power_Base = 0;
        }
        else
        {
            _players[itr].Power_Base += _players[itr].volcanoes;

            SETMIN(_players[itr].Power_Base, 0);
        }
    }

}


// WZD o120p03
int16_t Unit_Gold_Upkeep(int16_t unit_idx)
{
    int16_t unit_gold_upkeep;
    int16_t unit_owner_idx;

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
            // Yay Hero, Nay Noble, Nay Toren The Chosen
            if(
                (!HERO_NOBLE(unit_owner_idx, _UNITS[unit_idx].type))
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
        if (_UNITS[itr_units].owner_idx == player_idx)
        {
            gold_upkeep_cost += Unit_Gold_Upkeep(itr_units);
        }
    }

    gold_upkeep_cost -= fame_points;

    if(gold_upkeep_cost < 0)
    {
        gold_upkeep_cost = 0;
    }

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
void Players_Normal_Units(int16_t normal_units[])
{
    int16_t itr_players;  // _SI_
    int16_t itr_units;  // _SI_

    for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
    {
        normal_units[itr_players] = 0;
    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            ((_unit_type_table[_UNITS[itr_units].type].Abilities & 0x01 /* Ab_Summoned */) == 0) &&
            (_UNITS[itr_units].type > ut_Chosen) &&
            (_UNITS[itr_units].owner_idx < _num_players) &&
            ((_UNITS[itr_units].mutations & 0x20 /* R_Undead */) == 0)
        )
        {
            normal_units[_UNITS[itr_units].owner_idx]++;
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
*/
void Player_Resource_Income_Total(int16_t player_idx, int16_t * gold_total, int16_t * food_total, int16_t * mana_total)
{
    int16_t mana_income;
    int16_t food_income;
    int16_t gold_income;
    int16_t mana_expense;
    int16_t food_expense;
    int16_t gold_expense;

    int16_t itr_cities;
    int16_t itr_heroes;

    Player_Magic_Power_Income_Total(&mana_income, &food_income, &gold_income, player_idx);

    gold_expense = 0;
    gold_income = 0;  // clear the bogus value we just got from Player_Magic_Power_Income_Total()
    food_expense = 0;
    food_income = 0;  // clear the bogus value we just got from Player_Magic_Power_Income_Total()
    mana_expense = 0;


    if((g_TimeStop_PlayerNum - 1) == player_idx)
    {
        *food_total = 0;
        *gold_total = 0;
        *mana_total = -200;
    }
    else
    {
        mana_expense = Player_Armies_And_Enchantments_Mana_Upkeep(player_idx);

        gold_expense = Player_Armies_Gold_Upkeep(player_idx);

        food_expense = Player_Armies_Food_Upkeep(player_idx);

        if(player_idx != HUMAN_PLAYER_IDX)
        {
            mana_expense = ((difficulty_modifiers_table[_difficulty].maintenance * mana_expense) / 100);
            gold_expense = ((difficulty_modifiers_table[_difficulty].maintenance * gold_expense) / 100);
            food_expense = ((difficulty_modifiers_table[_difficulty].maintenance * food_expense) / 100);
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



        *food_total = food_income - food_expense;
        // mov bx, [bp+food]; mov ax, [bx]; cwd; sub ax, dx; sar ax, 1; add [bp+gold_income], ax
        gold_income += (*food_total > 0) ? *food_total / 2 : 0;
        *gold_total = gold_income - gold_expense;

        *mana_total = mana_income - mana_expense;

    }

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
    int16_t mana_upkeep_cost;
    int16_t itr_units;
    int16_t itr_cities;

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
    uint8_t * city_enchantments;
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
/*
    undefines owner and plane, resets level, sets finished
Removal Type:
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

e.g.,
    'Unit Statistics Window' / 'Unit View'
        Remove Enchantment - Fligh, Waterwalking, Windwalking
            |-> Dismiss_Unit(unit_idx)
                |-> Kill_Unit(unit_idx, 1)
*/
void Kill_Unit(int16_t unit_idx, int16_t Rmv_Type)
{
    int16_t itr;
    int16_t unit_owner_idx;

    unit_owner_idx = _UNITS[unit_idx].owner_idx;

    _UNITS[unit_idx].Level = Unit_Base_Level(unit_idx);

    // ¿ removal type 1 is "Dismiss" ?
    if((Rmv_Type == 1) || (_UNITS[unit_idx].type == ut_Chosen))
    {
        _UNITS[unit_idx].Finished = ST_TRUE;

        if(_UNITS[unit_idx].Hero_Slot > -1)
        {
            for(itr = 0; itr < NUM_HERO_ITEMS; itr++)
            {
                if(_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[itr] > -1)
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
                strcpy(hero_names_table[_UNITS[unit_idx].type].name, _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].name);
            }
        }
    }
    else
    {
        _UNITS[unit_idx].Finished = ST_TRUE;

        if(_UNITS[unit_idx].Hero_Slot > -1)
        {
            for(itr = 0; itr < NUM_HERO_ITEMS; itr++)
            {
                _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[itr] = ST_UNDEFINED;
            }

            _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].unit_idx = ST_UNDEFINED;

            if(Rmv_Type != 2)  /* ¿ must be type 0 ? */
            {
                if(_UNITS[unit_idx].owner_idx == HUMAN_PLAYER_IDX)
                {
                    hero_names_table[_UNITS[unit_idx].type].experience_points = _UNITS[unit_idx].XP;
                    strcpy(hero_names_table[_UNITS[unit_idx].type].name, _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].name);
                }

                _HEROES2[_UNITS[unit_idx].owner_idx]->heroes[_UNITS[unit_idx].type].Level = _UNITS[unit_idx].Level;
            }
            else
            {
                _HEROES2[_UNITS[unit_idx].owner_idx]->heroes[_UNITS[unit_idx].type].Level = -20;
            }
        }
    }

    _UNITS[unit_idx].owner_idx = ST_UNDEFINED;

    _UNITS[unit_idx].wp = ST_UNDEFINED;  // ¿ here because `wp = 9` is used for dead combat summon units ?

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
    int16_t itr_units;  // _DX_

    *troop_count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].wx == wx) &&
            (_UNITS[itr_units].wy == wy) &&
            (_UNITS[itr_units].owner_idx != ST_UNDEFINED) &&
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
            if ((_HEROES2[player_idx]->heroes[_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].type].abilities & HSA_SAGE2) != 0)
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
    AI_OVL_SpellPicker()
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

    if( (_players[player_idx].conjurer > 0) && ( (spell_data_table[spell_idx].type & 0x00 /* sdt_Summoning */) != 0) )
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
    int16_t gold_units;  // _DI_
    int16_t itr;  //  _SI_

    gold_units = 0;

    for(itr = 0; itr < NUM_BUILDINGS; itr++)
    {
        if(
            (_CITIES[city_idx].bldg_status[itr] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[itr] == bs_Replaced /* B_Replaced */)
        )
        {
            if(bldg_data_table[itr].maintenance_cost != 0)
            {
                gold_units += abs(bldg_data_table[itr].maintenance_cost);
            }
        }
    }

    return gold_units;
}


// WZD o120p23
// drake178: UNIT_GetLevel()
// MoO2  Module: ERICNET  Calc_Ship_Level_()
/*

*/
int16_t Calc_Unit_Level(int16_t unit_idx)
{
    int16_t itr;
    int16_t level;

    level = _UNITS[unit_idx].Level;

    // TODO  WTF?  if(_UNITS[unit_idx].type < spell_data_table[])
    if(_UNITS[unit_idx].type < FST_FANT)  /* First Fantastic Unit (Magic Spirit) */
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

            // DELETEME  if(_UNITS[unit_idx].XP >= TBL_Experience[UNIT_LEVEL_MAX])
            // DELETEME  {
            // DELETEME      _UNITS[unit_idx].XP = TBL_Experience[UNIT_LEVEL_MAX];
            // DELETEME  }
            SETMAX(_UNITS[unit_idx].XP, TBL_Experience[UNIT_LEVEL_MAX]);
        }
        else
        {
            // DELETEME  if(_UNITS[unit_idx].XP >= TBL_Experience[UL_ELITE])
            // DELETEME  {
            // DELETEME      _UNITS[unit_idx].XP = TBL_Experience[UL_ELITE];
            // DELETEME  }
            SETMAX(_UNITS[unit_idx].XP, TBL_Experience[UNIT_LEVEL_MAX]);
        }

        // ¿ cancel 'Heroism' 'Unit Enchantment' if the Unit has naturally attained the 'Elite Experience Level' ?
        if(
            (level >= UL_ELITE)
            &&
            ((_UNITS[unit_idx].enchantments & UE_HEROISM) != 0)
        )
        {
            _UNITS[unit_idx].enchantments = (_UNITS[unit_idx].enchantments | UE_HEROISM);
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

    // DELETEME  if(level > UNIT_LEVEL_MAX)
    // DELETEME  {
    // DELETEME      level = UNIT_LEVEL_MAX;
    // DELETEME  }
    SETMAX(level, UNIT_LEVEL_MAX);

    return level;
}


// WZD o120p24
// drake178: UNIT_GetBaseLevel()
int16_t Unit_Base_Level(int16_t unit_idx)
{
    int16_t level;  // _DI_
    int16_t itr;  // _DX_

    level = _UNITS[unit_idx].Level;

    // *(spell_data_table + 0x1C64)
    // WZD dseg:912C
    // byte spell_data_table[215][36]
    // 1C64h  7268d
    // 201 * 36 = 7236
    // 7268 - 7236 = 32d  20h
    // ¿ s_SPELL_DATA.Param0 ?  ; unit type, base damage, UE flag, or CE index
    if(_UNITS[unit_idx].type < spell_data_table[201].Param0)
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
// drake178: UNIT_LoggedPushOff()
void UNIT_LoggedPushOff(int16_t unit_idx)
{
    int16_t flag;  // _DI_

    if(_UNITS[unit_idx].owner_idx != HUMAN_PLAYER_IDX)
    {
        flag = ST_FALSE;
    }
    else
    {
        flag = ST_TRUE;
    }
    
    UNIT_PushOffTile(unit_idx);

    if(
        (_UNITS[unit_idx].owner_idx == -1) &&
        (flag == ST_TRUE) &&
        (MSG_UnitLost_Count < 20)
    )
    {
        MSG_UnitLost_Array[MSG_UnitLost_Count].Unit_Type = _UNITS[unit_idx].type;

        MSG_UnitLost_Array[MSG_UnitLost_Count].Cause = 3;
    }

}


// WZD o120p26
// drake178: UNIT_RemoveExcess()
void UNIT_RemoveExcess(int16_t unit_idx)
{
    int16_t troops[10] = { 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB };
    int16_t unit_wp;
    int16_t unit_wy;
    int16_t unit_wx;
    int16_t troop_count;
    int16_t lowest_trooper_idx;
    int16_t trooper_value;
    int16_t lowest_trooper_value;
    int16_t itr_troops;

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

        if (_UNITS[lowest_trooper_idx].owner_idx != NEUTRAL_PLAYER_BANNER_COLOR_IDX)
        {
            UNIT_LoggedPushOff(lowest_trooper_idx);
        }
        else
        {
            Kill_Unit(lowest_trooper_idx, 1);
        }

    }
    
}



/*
    WIZARDS.EXE  ovr142
*/

// WZD o142p01
// drake178: TILE_CreateRoad()
/*
; creates a road on the tile, setting the corresponding
; movement allowances, and removing the enchanted road
; flag if set, but then also enchanting the road if the
; tile is on Myrror
*/
void TILE_CreateRoad(int16_t wx, int16_t wy, int16_t wp)
{
    uint8_t terrain_flags;

    // TODO  EMM_Map_DataH();  // ; maps the EMM Data block into the page frame

    movement_mode_cost_maps[wp].UU_MvMd.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
    movement_mode_cost_maps[wp].walking.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
    movement_mode_cost_maps[wp].forester.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
    movement_mode_cost_maps[wp].mountaineer.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
    movement_mode_cost_maps[wp].swimming.moves2[((wy * WORLD_WIDTH) + wx)] = 1;

    terrain_flags = _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];

    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] |= MSF_ROAD;

    terrain_flags = _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];

    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] &= ~(TF_Enc_Road);

    terrain_flags = _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];

    if(wp == 1)  /* Myrror */
    {
        TILE_EnchantRoad(wx, wy, wp);
    }

    terrain_flags = _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];

}


// WZD o142p02
// drake178: TILE_EnchantRoad()
/*
; changes a regular road on a tile to an enchanted one,
; or does nothing if there is no road on the tile
*/
void TILE_EnchantRoad(int16_t wx, int16_t wy, int16_t wp)
{

    // TODO  EMM_Map_DataH();                   ; maps the EMM Data block into the page frame

    if((_map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] & MSF_ROAD) != 0)
    {

        _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] |= TF_Enc_Road;

        movement_mode_cost_maps[wp].UU_MvMd.moves2[((wy * WORLD_WIDTH) + wx)] = 0;

        movement_mode_cost_maps[wp].walking.moves2[((wy * WORLD_WIDTH) + wx)] = 0;

        movement_mode_cost_maps[wp].forester.moves2[((wy * WORLD_WIDTH) + wx)] = 0;

        movement_mode_cost_maps[wp].mountaineer.moves2[((wy * WORLD_WIDTH) + wx)] = 0;
        
    }

}


// WZD o142p03
// drake178: LD_TILE_ResUpdate()
/*
; Legacy Development function, can be removed
;
; would update some aspect of some resource on a tile
; by tile basis with no return value
*/
void empty_fxn_o142p03(int16_t wx, int16_t wy, int16_t wp)
{
    return;
}

// WZD o142p04
// LD_CTY_ResRefresh()
/*
; Legacy Development function, can be removed
;
; would update some aspect of some resource on every
; tile of every catchment area (including corners)
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
                        empty_fxn_o142p03(city_area_square_wx, city_area_square_wy, city_wp);
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
// drake178: CTY_GetTileFood()
int16_t City_Food_Terrain(int16_t city_idx)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t city_wp = 0;
    int16_t useable_map_squares = 0;
    int16_t food2_units = 0;  // _DI_
    int16_t itr = 0;  // _SI_

    city_wp = _CITIES[city_idx].wp;

    // NOTE: Accounts for 'Corruption'
    // useable_map_squares = Get_Useable_City_Area(_CITIES[city_idx].wx, _CITIES[city_idx].wy, city_wp, &wx_array[0], &wy_array[0]);
    useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

    food2_units = 0;

    for(itr = 0; itr < useable_map_squares; itr++)
    {
        food2_units += Square_Food2(wx_array[itr], wy_array[itr], city_wp);
    }

    if(_CITIES[city_idx].enchantments[GAIAS_BLESSING] != ST_FALSE)
    {
        food2_units = ((food2_units * 3) / 2);
    }

    return (food2_units / 4);
}

// WZD o142p07
// drake178: CTY_GetWildGameFood()
int16_t City_Food_WildGame(int16_t city_idx)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t bit_index = 0;
    int16_t city_wp = 0;
    int16_t useable_map_squares = 0;
    int16_t food_units = 0;  // _DI_
    int16_t itr = 0;  // _SI_
    uint8_t * bit_field = 0;  // _DX_
    uint16_t terrain_specials_offset = 0;
    uint8_t terrain_special = 0;

    city_wp = _CITIES[city_idx].wp;

    // NOTE: Accounts for 'Corruption'
    // useable_map_squares = Get_Useable_City_Area(_CITIES[city_idx].wx, _CITIES[city_idx].wy, city_wp, &wx_array[0], &wy_array[0]);
    useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

    food_units = 0;

    for(itr = 0; itr < useable_map_squares; itr++)
    {
        terrain_specials_offset = ((city_wp * WORLD_SIZE) + (wy_array[itr] * WORLD_WIDTH) + (wx_array[itr]));
        terrain_special = GET_1B_OFS(TBL_Terr_Specials,terrain_specials_offset);

        // if( (*((uint8_t *)&TBL_Terr_Specials[((city_wp * WORLD_SIZE) + (wy_array[itr] * WORLD_WIDTH) + wx_array[itr])]) & 0x40 /* TS_Wild_Game */) != 0)
        if((terrain_special & 0x40) != 0)  /* TS_Wild_Game */
        {
            bit_index = ((wy_array[itr] * WORLD_WIDTH) + wx_array[itr]);
            bit_field = (city_area_shared_bits + (city_wp * WORLD_SIZE) );
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
// drake178: CTY_GetCatchment()
// does like Draw_Map_Roads() with itr_world_x and curr_world_x
/*
    loops over city area 5x5
    checks for corruption

    in-out wx_array
    in-out wy_array
    returns count of elements in arrays  AKA map_square_count

    NOTE: Accounts for 'Corruption'
*/
int16_t Get_Useable_City_Area(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t *wx_array, int16_t *wy_array)
{
    uint8_t * terrain_flags_table_row;
    int16_t square_y;
    int16_t itr_world_x;
    int16_t square_x_max;
    int16_t square_x_min;
    int16_t map_square_count;

    int16_t itr_city_area_squares;  // _DI_
    int16_t square_x;  // _SI_

    map_square_count = 0;

    for(itr_city_area_squares = -2; itr_city_area_squares <= 2; itr_city_area_squares++)
    {
        square_y = city_wy + itr_city_area_squares;

        if( (square_y >= 0) && (square_y < WORLD_HEIGHT) )
        {
            square_x_min = -2;
            square_x_max = 2;

            if(
                (itr_city_area_squares == -2) ||
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
                if(square_x > WORLD_WIDTH)
                {
                    square_x -= WORLD_WIDTH;
                }

                if( (*(terrain_flags_table_row + square_x) & 0x20) == 0 ) /* MSF_CORRUPTION */
                {
                    wx_array[map_square_count] = square_x;
                    wy_array[map_square_count] = square_y;
                    map_square_count++;
                }

                itr_world_x++;
            }
        }
    }

    return map_square_count;
}


// WZD o142p09
// drake178: CTY_GetFood()
int16_t City_Food_Production(int16_t city_idx)
{
    int16_t city_area_food_units;

    int16_t food_units;

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
            _CITIES[city_idx].farmer_count = _CITIES[city_idx].population;
        }

        if(
            (_CITIES[city_idx].race == 0x06 /* R_Halfling */) ||
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Replaced /* B_Replaced */)
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
            (_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Replaced /* B_Replaced */)
        )
        {
            food_units += 2;
        }

        if(_CITIES[city_idx].enchantments[FAMINE] != ST_FALSE)
        {
            food_units = food_units / 2;
        }

        // Over-Farming Adjustment
        if(food_units > city_area_food_units)
        {
            food_units = (city_area_food_units + ((food_units - city_area_food_units) / 2));
        }

        if(
            (_CITIES[city_idx].bldg_status[GRANARY] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[GRANARY] == bs_Replaced /* B_Replaced */)
        )
        {
            food_units += 2;
        }

        if(
            (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Replaced /* B_Replaced */)
        )
        {
            food_units += 3;
        }

        food_units += City_Food_WildGame(city_idx);

    }

    return food_units;
}


// WZD o142p10
// drake178: CTY_GetMaxPop()
/*
; returns the city's maximum population (without cap),
; based on tile food, gaia's blessing, famine,
; granary, and farmer's market
*/
/*

XREF:
    City_Growth_Rate()
    Compute_Base_Map_Square_Values()
    NX_j_City_Maximum_Size()

*/
int16_t City_Maximum_Size(int16_t city_idx)
{
    int16_t maximum_size;  // _SI_

    maximum_size = City_Food_Terrain(city_idx);

    if(_CITIES[city_idx].enchantments[FAMINE] > 0)
    {
        maximum_size = (maximum_size / 2);
    }

    if(
        (_CITIES[city_idx].bldg_status[GRANARY] == bs_Built /* B_Built */)
        ||
        (_CITIES[city_idx].bldg_status[GRANARY] == bs_Replaced /* B_Replaced */)
    )
    {
        maximum_size += 2;
    }

    if(
        (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Built /* B_Built */)
        ||
        (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Replaced /* B_Replaced */)
    )
    {
        maximum_size += 3;
    }

    // BUGBUG  ¿ no FORESTERS_GUILD ?

    maximum_size += City_Food_WildGame(city_idx);

    return maximum_size;
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
    int16_t City_Owner = 0;
    int16_t production_units = 0;
    int16_t worker_count = 0;
    int16_t itr = 0;
    int16_t useable_map_squares = 0;
    int16_t production_modifier = 0;  // _DI_

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
            _CITIES[city_idx].farmer_count = _CITIES[city_idx].population;
        }

        worker_count = _CITIES[city_idx].population - _CITIES[city_idx].farmer_count - City_Rebel_Count(city_idx);

        if(
            (_CITIES[city_idx].race == 0x04 /* R_Dwarf */) ||
            (_CITIES[city_idx].race == 0x09 /* R_Klackon */)
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
            (_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Replaced /* B_Replaced */)
        )
        {
            production_modifier += 25;
        }

        if(
            (_CITIES[city_idx].bldg_status[SAWMILL] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[SAWMILL] == bs_Replaced /* B_Replaced */)
        )
        {
            production_modifier += 25;
        }

        if(
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] == bs_Replaced /* B_Replaced */)
        )
        {
            production_modifier += 50;
        }

        if(
            (_CITIES[city_idx].bldg_status[MECHANICIANS_GUILD] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[MECHANICIANS_GUILD] == bs_Replaced /* B_Replaced */)
        )
        {
            production_modifier += 50;
        }

        if(_CITIES[city_idx].enchantments[INSPIRATIONS] != ST_FALSE)
        {
            production_modifier += 100;
        }

        production_units = ((production_units * production_modifier) / 100);

        if(_CITIES[city_idx].enchantments[CURSED_LANDS] != ST_FALSE)
        {
            production_units = (production_units / 2);
        }

    }

    return production_units;
}



// WZD o142p12
// drake178: CTY_GetGold()
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
    int16_t gold_units = 0;  // _DI_

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
            (_CITIES[city_idx].bldg_status[MERCHANTS_GUILD] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[MERCHANTS_GUILD] == bs_Replaced /* B_Replaced */)
        )
        {
            gold_modifier += 100;
        }

        if(
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] == bs_Replaced /* B_Replaced */)
        )
        {
            have_miners_guild = ST_TRUE;
        }
        else
        {
            have_miners_guild = ST_FALSE;
        }

        if(
            (_CITIES[city_idx].bldg_status[BANK] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[BANK] == bs_Replaced /* B_Replaced */)
        )
        {
            gold_modifier += 50;
        }

        if(
            (_CITIES[city_idx].bldg_status[MARKETPLACE] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[MARKETPLACE] == bs_Replaced /* B_Replaced */)
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
        
        if(_CITIES[city_idx].race != 0x04 /* R_Dwarf */)
        {
            are_dwarf = ST_FALSE;
        }
        else
        {
            are_dwarf = ST_TRUE;
        }

        for(itr = 0; itr < useable_map_squares; itr++)
        {

            gold_units += Square_Gold_Income(wx_array[itr], wy_array[itr], city_wp, have_miners_guild, are_dwarf);

        }

        // not sure why this is adding 100 to the modifier, instead of just doing +=
        gold_units = ((gold_units * (gold_modifier + 100)) / 100);

        if(_CITIES[city_idx].construction == 0x01 /* _Trade_Goods */)
        {
            gold_units += (_CITIES[city_idx].production_units / 2);
        }

        if(gold_units > 255)
        {
            gold_units = 255;
        }

    }

    return gold_units;
}


// WZD o142p13
// drake178: CTY_GetResearch()
int16_t City_Research_Production(int16_t city_idx)
{
    int16_t city_owner_idx;

    int16_t research_units;  // _SI_

    city_owner_idx = _CITIES[city_idx].owner_idx;

    if(
        (_CITIES[city_idx].population != 0) &&
        (city_owner_idx != NEUTRAL_PLAYER_IDX)
    )
    {

        research_units = 0;

        if(
            (_CITIES[city_idx].bldg_status[LIBRARY] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[LIBRARY] == bs_Replaced /* B_Replaced */)
        )
        {
            research_units += 2;
        }

        if(
            (_CITIES[city_idx].bldg_status[SAGES_GUILD] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[SAGES_GUILD] == bs_Replaced /* B_Replaced */)
        )
        {
            research_units += 3;
        }

        if(
            (_CITIES[city_idx].bldg_status[UNIVERSITY] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[UNIVERSITY] == bs_Replaced /* B_Replaced */)
        )
        {
            research_units += 5;
        }

        if(
            (_CITIES[city_idx].bldg_status[WIZARDS_GUILD] == bs_Built /* B_Built */) ||
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
    int16_t mana_units = 0;  // _DI_

    city_owner_idx = _CITIES[city_idx].owner_idx;

    if(
        (events_table->Mana_Short_Status != 2) &&
        (_CITIES[city_idx].population != 0) &&
        (city_owner_idx != NEUTRAL_PLAYER_IDX)
    )
    {
        building_magic_power = 0;
        building_magic_power_modifier = 100;

        if(_players[city_owner_idx].divine_power != ST_FALSE)
        {
            building_magic_power_modifier = 150;
        }

        if(_CITIES[city_idx].enchantments[DARK_RITUALS] != ST_FALSE)
        {
            building_magic_power_modifier = (building_magic_power_modifier * 2);
        }

        if(
            (_CITIES[city_idx].bldg_status[SHRINE] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[SHRINE] == bs_Replaced   /* B_Replaced */)
        )
        {
            building_magic_power += 1;
        }

        if(
            (_CITIES[city_idx].bldg_status[TEMPLE] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[TEMPLE] == bs_Replaced /* B_Replaced */)
        )
        {
            building_magic_power += 2;
        }

        if(
            (_CITIES[city_idx].bldg_status[PARTHENON] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[PARTHENON] == bs_Replaced /* B_Replaced */)
        )
        {
            building_magic_power += 3;
        }

        if(
            (_CITIES[city_idx].bldg_status[CATHEDRAL] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[CATHEDRAL] == bs_Replaced /* B_Replaced */)
        )
        {
            building_magic_power += 4;
        }

        building_magic_power = ((building_magic_power * building_magic_power_modifier) / 100);

        mana_units = building_magic_power;

        if(
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Replaced /* B_Replaced */)
        )
        {
            mana_units += 3;
        }

        if(
            (_CITIES[city_idx].bldg_status[WIZARDS_GUILD] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[WIZARDS_GUILD] == bs_Replaced /* B_Replaced */)
        )
        {
            mana_units += 3;
        }

        city_wp = _CITIES[city_idx].wp;

        if(_CITIES[city_idx].race == 0x02 /* R_Dark_Elf */)
        {
            mana_units += _CITIES[city_idx].population;
        }

        if(
            (_CITIES[city_idx].race == 0x07 /* R_High_Elf */) ||
            (_CITIES[city_idx].race == 0x01 /* R_Beastman */) ||
            (_CITIES[city_idx].race == 0x03 /* R_Draconian */)
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

        if(_CITIES[city_idx].race != 0x04 /* R_Dwarf */)
        {
            are_dwarf = ST_FALSE;
        }
        else
        {
            are_dwarf = ST_TRUE;
        }

        if(
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] != bs_Built /* B_Built */) &&
            (_CITIES[city_idx].bldg_status[MINERS_GUILD] != bs_Replaced /* B_Replaced */)
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

    return mana_units;
}


// WZD o142p15
// drake178: CTY_GetPopGrowth()
/*
; calculates and returns the city's population growth
; contains multiple bugs that can prevent negative
; total growth from being applied in certain cases
*/
/*

XREF:
    j_City_Growth_Rate()
        City_Screen_Draw2__WIP()
        Apply_City_Changes()
*/
int16_t City_Growth_Rate(int16_t city_idx)
{
    int16_t maximum_size;
    int16_t population_growth_modifier;
    int16_t population_growth_rate;  // _DI_

    if(_CITIES[city_idx].population == 0)
    {
        population_growth_rate = 0;
    }
    else
    {
        maximum_size = City_Maximum_Size(city_idx);

        if(_CITIES[city_idx].population == maximum_size)  // BUGBUG  drake178: BUG #1: prevents negative growth at max pop
        {
            population_growth_rate = 0;
        }
        else
        {
            if(maximum_size == 0)
            {
                population_growth_rate = -(_CITIES[city_idx].population);
            }
            else
            {
                if(_CITIES[city_idx].food_units < _CITIES[city_idx].population)
                {
                    population_growth_rate = ((_CITIES[city_idx].food_units - _CITIES[city_idx].population) * 5);
                }
                else
                {
                    // e.g., 20 - 10 = 10 + 1 = 11 / 2 = 5 + {2, 1, 0, -1, -2} = {7, 6, 5, 4, 3}
                    population_growth_rate = ((((maximum_size - _CITIES[city_idx].population) + 1) / 2) + _race_type_table[_CITIES[city_idx].race].Growth_Mod);

                    if(
                        (_CITIES[city_idx].bldg_status[GRANARY] == bs_Built /* B_Built */)
                        ||
                        (_CITIES[city_idx].bldg_status[GRANARY] == bs_Replaced /* B_Replaced */)
                    )
                    {
                        population_growth_rate += 2;
                    }

                    if(
                        (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Built /* B_Built */)
                        ||
                        (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Replaced /* B_Replaced */)
                    )
                    {
                        population_growth_rate += 3;
                    }

                    if(_CITIES[city_idx].population >= MAX_CITY_POPULATION)  // BUGBUG  drake178: BUG #2: prevents negative growth at 25 pop
                    {
                        population_growth_rate = 0;
                    }
                    else
                    {

                        if(_CITIES[city_idx].enchantments[STREAM_OF_LIFE] > 0)
                        {
                            population_growth_rate = (population_growth_rate * 2);
                        }

                        /*
                            Population Boom
                        */
                        if( (events_table->Population_Boom_Status != 0) && (events_table->Population_Boom_Data == city_idx) )
                        {
                            population_growth_rate = (population_growth_rate * 2);
                        }

                        population_growth_modifier = 0;

                        if(_CITIES[city_idx].enchantments[DARK_RITUALS] > 0)
                        {
                            population_growth_modifier -= 25;
                        }

                        if(_CITIES[city_idx].construction == bt_Housing)
                        {

                            if(_CITIES[city_idx].population == 1)
                            {
                                population_growth_modifier += 50;
                            }
                            else
                            {
                                population_growth_modifier += ((((_CITIES[city_idx].population - City_Rebel_Count(city_idx)) - _CITIES[city_idx].farmer_count) * 100) / _CITIES[city_idx].population);
                            }

                            if(
                                (_CITIES[city_idx].bldg_status[SAWMILL] == bs_Built /* B_Built */)
                                ||
                                (_CITIES[city_idx].bldg_status[SAWMILL] == bs_Replaced /* B_Replaced */)
                            )
                            {
                                population_growth_modifier += 10;
                            }

                            if(
                                (_CITIES[city_idx].bldg_status[BUILDERS_HALL] == bs_Built /* B_Built */)
                                ||
                                (_CITIES[city_idx].bldg_status[BUILDERS_HALL] == bs_Replaced /* B_Replaced */)
                            )
                            {
                                population_growth_modifier += 15;
                            }

                            if(_CITIES[city_idx].population >= MAX_CITY_POPULATION)  // BUGBUG  drake178: BUG #3: prevents negative growth at 25 pop if housing is selected as the build project, even with zero workers and no buildings
                            {
                                population_growth_rate = 0;
                            }
                            else
                            {

                                population_growth_rate = (((population_growth_rate * population_growth_modifier) + 100) / 100);

                                if(_CITIES[city_idx].owner_idx == NEUTRAL_PLAYER_IDX)
                                {
                                    population_growth_rate /= 2;

                                    if(_CITIES[city_idx].population >= ((_difficulty + 1) * 2))
                                    {
                                        population_growth_rate = 0;
                                    }
                                }
                                else
                                {
                                    if(_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
                                    {
                                        population_growth_rate = ((population_growth_rate * difficulty_modifiers_table[_difficulty].population_growth) / 100);
                                    }
                                }

                                SETMIN(population_growth_rate, 0);

                            }

                        }

                    }

                }

            }

        }

    }

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


    // TODO  EMM_Map_DataH()


    product_idx = _CITIES[city_idx].construction;

    if(_CITIES[city_idx].race != 4)  /* R_Dwarf */
    {
        are_dwarf = ST_FALSE;
    }
    else
    {
        are_dwarf = ST_TRUE;
    }

    if(product_idx >= 100)
    {
        product_cost = _unit_type_table[(product_idx - 100)].Cost;

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

        if(unit_cost_reduction_applies == ST_TRUE)
        {
            for(itr = 0; itr < useable_map_squares; itr++)
            {
                unit_cost_percent -= Square_Unit_Cost_Reduction(wx_array[itr], wy_array[itr], city_wp, have_miners_guild, are_dwarf);
            }
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
    int16_t G_Tries;
    int16_t Myrror_Available;
    int16_t Arcanus_Available;
    int16_t Unit_Count;
    int16_t Total_Cost;
    int16_t Roll_Total;
    int16_t Merc_Level;
    int16_t player_fame;
    int16_t itr_units;  // _SI_
    int16_t itr_cities;  // _SI_
    int16_t itr_towers;  // _SI_
    int16_t unit_type;  // _DI_
    int16_t return_value;  // _AX_

    Total_Cost = 0;  // HACK:  DNE in Dasm

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

        if(_TOWERS[itr_towers].owner_idx != player_idx)
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
    else if(Roll_Total > 90)
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
            (_unit_type_table[unit_type].Race == rt_Draconian)
            &&
            (_unit_type_table[unit_type].Construction > 0)
        )
        {
            continue;
        }

        if(
            (_unit_type_table[unit_type].Race == rt_Beastman)
            ||
            (_unit_type_table[unit_type].Race == rt_Dark_Elf)
            ||
            (_unit_type_table[unit_type].Race == rt_Draconian)
            ||
            (_unit_type_table[unit_type].Race == rt_Dwarf)
            ||
            (_unit_type_table[unit_type].Race == rt_Troll)
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

        /* ¿ BUGBUG  Total_Cost is uninitialized ? */
        if((_turn / 2) < Total_Cost)
        {
            continue;
        }

        Total_Cost = ((_unit_type_table[unit_type].Cost * (3 + Merc_Level)) / 2);

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
// LD_MAP_TFUnk40_Eval()


// WZD o142p20
// drake178: CTY_GetMinFarmers()
int16_t City_Minimum_Farmers(int16_t city_idx)
{
    int16_t food_per_farmer;
    int16_t Farming_Threshold;
    int16_t city_population;
    int16_t required_farmer_food;
    int16_t minimum_farmer_count;
    int16_t farmer_food;  // _DI_

    // TODO  EMM_Map_DataH()

    city_population = _CITIES[city_idx].population;

    required_farmer_food = (city_population - City_Food_WildGame(city_idx));

    if( (_CITIES[city_idx].bldg_status[GRANARY] == bs_Built) || (_CITIES[city_idx].bldg_status[GRANARY] == bs_Replaced) )
    {
        required_farmer_food -= 2;
    }

    if( (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Built) || (_CITIES[city_idx].bldg_status[FARMERS_MARKET] == bs_Replaced) )
    {
        required_farmer_food -= 3;
    }

    if(required_farmer_food > 0)
    {
            Farming_Threshold = City_Food_Terrain(city_idx);

            if(
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Replaced /* B_Replaced */) ||
            (_CITIES[city_idx].race == rt_Halfling /* R_Halfling */)
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

            if((_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Built) || (_CITIES[city_idx].bldg_status[FORESTERS_GUILD] == bs_Replaced))
            {
                farmer_food += 2;
            }

            if(_CITIES[city_idx].enchantments[FAMINE] != ST_FALSE)
            {
                farmer_food = (farmer_food / 2);
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

    return minimum_farmer_count;
}


// WZD o142p21
// drake178: CTY_OutpostGrowth()
// MoO2  Module: COLCALC  Apply_Colony_Pop_Growth_()
/*
; calculates and records the growth and/or shrinkage of all outposts, although turning into a full city is not in this function
; BUG: this function ignores Wild Games altogether, both as a source of food and as a terrain special
; BUG: difficulty-based outpost growth modifiers are applied to both AI and human player outposts
*/
/*
    adds and/or subtracts to/from _CITIES[].Pop_10s
    if percentage change of grow += {3}
    if percentage change of shrink += {2}
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
    int16_t itr_cities = 0;  // _SI_
    int16_t grow = 0;  // _DI_

    // DONT  EMM_Map_DataH();

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].population == 0)
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
            terrain_special = *(TBL_Terr_Specials + (city_wp * WORLD_SIZE) + (wy_array[itr] * WORLD_WIDTH) + wx_array[itr]);

            if(terrain_special != 0)
            {
                if(
                    (terrain_special == 1)  /* TS_IronOre */
                    ||
                    (terrain_special == 3)  /* TS_SilverOre */
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

        shrink =  5;  // 5%  1:20

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
        
        grow = ((difficulty_modifiers_table[_difficulty].outpost_growth * grow) / 100);

        if(Random(100) <= grow)
        {
            _CITIES[itr_cities].Pop_10s += Random(3);
        }

        if(Random(100) <= shrink)
        {
            _CITIES[itr_cities].Pop_10s += Random(2);
        }
    }

}


// WZD o142p22
// drake178: WIZ_RecordHistory()
// MoO2  Module: BILL  Record_History_()
/*
; calculates and records the astrologer/historian data
; Power = power base - 10
; Army Strength = (gold upkeep + mana upkeep) / 5
; Research = sum of known in-realm indexes / 10
; Historian = (above 3) + ((cities pop_K sum) / 10)
; Astrologer's max scale is 200, Historian's is 160
*/
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
    int16_t itr = 0;  // _SI_
    int16_t itr_players = 0;  // _SI_
    int16_t IDK_max_curr_wiz_pwr = 0;  // _DI_

    // ; fill out the astrologer fields and the local total progress array (sum of all fields)
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].Astr.Magic_Power     = WIZ_Get_Astr_Power__STUB(itr_players);
        _players[itr_players].Astr.Army_Strength   = WIZ_Get_Astr_ArmyStr__STUB(itr_players);
        _players[itr_players].Astr.Spell_Research  = WIZ_Get_Astr_Resrch__STUB(itr_players);
        _players[itr_players].Pop_div_10k          = WIZ_Get_Nation_Size__STUB(itr_players);

        power_of_a_wizard[itr_players] = (_players[itr_players].Astr.Magic_Power + _players[itr_players].Astr.Army_Strength + _players[itr_players].Astr.Spell_Research + _players[itr_players].Pop_div_10k);
    }

    max_curr_magic = 0;
    max_curr_military = 0;
    max_curr_research = 0;
    max_curr_population = 0;

    IDK_max_curr_wiz_pwr = 0;

    // ; find and store the highest values in each category, with di holding the highest total
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(_players[itr_players].Astr.Magic_Power > max_curr_magic)
        {
            max_curr_magic = _players[itr_players].Astr.Magic_Power;
        }
        
        if(_players[itr_players].Astr.Army_Strength > max_curr_military)
        {
            max_curr_military = _players[itr_players].Astr.Army_Strength;
        }
        
        if(_players[itr_players].Astr.Spell_Research > max_curr_research)
        {
            max_curr_research = _players[itr_players].Astr.Spell_Research;
        }
        
        if(_players[itr_players].Pop_div_10k > max_curr_population)
        {
            max_curr_population = _players[itr_players].Pop_div_10k;
        }

        if(power_of_a_wizard[itr_players] > IDK_max_curr_wiz_pwr)
        {
            IDK_max_curr_wiz_pwr = power_of_a_wizard[itr_players];
        }
        
    }

    // adjust power so that the highest is at most 200
    if(max_curr_magic > 200)
    {
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            _players[itr_players].Astr.Magic_Power = ((max_curr_magic * _players[itr_players].Astr.Magic_Power) / 200);
        }
    }

    // adjust army strength so that the highest is at most 200
    if(max_curr_military > 200)
    {
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            _players[itr_players].Astr.Army_Strength = ((max_curr_military * _players[itr_players].Astr.Army_Strength) / 200);
        }
    }

    // adjust research so that the highest is at most 200
    if(max_curr_research > 200)
    {
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            _players[itr_players].Astr.Spell_Research = ((max_curr_research * _players[itr_players].Astr.Spell_Research) / 200);
        }
    }

    // divide the total progress values by 7
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        power_of_a_wizard[itr_players] = (power_of_a_wizard[itr_players] / 7);
    }

    IDK_max_curr_wiz_pwr = (IDK_max_curr_wiz_pwr / 7);

    // adjust the total progress values such that the highest is at most 160
    if(IDK_max_curr_wiz_pwr > 160)
    {
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            power_of_a_wizard[itr_players] = ((IDK_max_curr_wiz_pwr * power_of_a_wizard[itr_players]) / 160);
        }
    }
    

    if(_turn < 288)
    {
        // record the total progress values into the historian
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            _players[itr_players].Historian[_turn] = power_of_a_wizard[itr_players];
        }
    }
    else
    {
        // scroll the historian, then record the total progress values into it
        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            for(itr_history = 0; itr_history < 287; itr_history++)
            {
                _players[itr_players].Historian[(itr_history + 1)] = _players[itr_players].Historian[itr_history];
            }

            _players[itr_players].Historian[287] = power_of_a_wizard[itr_players];
        }
    }

}


// WZD o142p23
// drake178: WIZ_Get_Astr_Power()
int16_t WIZ_Get_Astr_Power__STUB(int16_t player_idx)
{
    return Random(100);
}

// WZD o142p24
// drake178: WIZ_Get_Astr_ArmyStr()
int16_t WIZ_Get_Astr_ArmyStr__STUB(int16_t player_idx)
{
    return Random(100);
}

// WZD o142p25
// drake178: WIZ_Get_Astr_Resrch()
int16_t WIZ_Get_Astr_Resrch__STUB(int16_t player_idx)
{
    return Random(100);
}

// WZD o142p26
// drake178: WIZ_Get_Nation_Size()
int16_t WIZ_Get_Nation_Size__STUB(int16_t player_idx)
{
    int16_t itr_cities;  // _CX_
    int16_t empire_population;  // _SI_

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
    int16_t Pacify_Police;
    int16_t city_wp;
    int16_t Pacify_Religion;
    int16_t unrest_percent;
    int16_t city_wy;
    int16_t city_wx;
    int16_t Pacify;
    // itr_num_players__units
    int16_t itr_num_players;  // itr_num_players__units
    int16_t itr_units;  // itr_num_players__units
    int16_t city_owner_idx;
    int16_t rebel_count;
    int16_t unrest_races;  // DNE in Dasm
    int16_t unrest_taxes;  // DNE in Dasm

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
            (_CITIES[city_idx].bldg_status[SHRINE] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[SHRINE] == bs_Replaced /* B_Replaced */)
        )
        {
            Pacify_Religion += 1;
        }

        if(
            (_CITIES[city_idx].bldg_status[TEMPLE] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[TEMPLE] == bs_Replaced /* B_Replaced */)
        )
        {
            Pacify_Religion += 1;
        }

        if(
            (_CITIES[city_idx].bldg_status[PARTHENON] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[PARTHENON] == bs_Replaced /* B_Replaced */)
        )
        {
            Pacify_Religion += 1;
        }

        if(
            (_CITIES[city_idx].bldg_status[CATHEDRAL] == bs_Built /* B_Built */) ||
            (_CITIES[city_idx].bldg_status[CATHEDRAL] == bs_Replaced /* B_Replaced */)
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

    if(
        (_CITIES[city_idx].bldg_status[ORACLE] == bs_Built /* B_Built */)
    )
    {
        Pacify += 2;
    }

    if(
        (_CITIES[city_idx].bldg_status[ANIMISTS_GUILD] == bs_Built /* B_Built */)
    )
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
                (_UNITS[itr_units].owner_idx == city_owner_idx) &&
                (_UNITS[itr_units].wx == city_wx) &&
                (_UNITS[itr_units].wy == city_wy) &&
                (_UNITS[itr_units].wp == city_wp) &&
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
        _CITIES[city_idx].farmer_count = _CITIES[city_idx].population;
    }

    if(_CITIES[city_idx].farmer_count < 0)
    {
        _CITIES[city_idx].farmer_count = 0;
    }

    return rebel_count;
}


// WZD o142p28
// drake178: TILE_Survey()
/*
; surveys the selected tile, filling out the variables
; at the return pointer locations
; contains one bug that miscalculates wild game food
; when looking to settle (marked)
*/
/*
    "surveyor’s information ... statistics for a potential city built on the selected map square"

    *MaxPop     
    *production_bonus       Square_Production_Bonus()
    *gold_bonus                 City_Road_Trade_Bonus() ... if rt_Nomand += 50
    *unit_cost_reduction    Square_Unit_Cost_Reduction()
    *Gold       
    *Power      
    *have_nightshade     
    *have_mithril     
    *have_adamantium     
    *have_shore     
    *is_unexplored flag {F,T}

Surveyor_Window_Display()
    Compute_Base_Map_Square_Values(l_mx, l_my, _map_plane, &val, &production_bonus, &gold_bonus, &unit_cost_reduction, &gold_units, &magic_units, &have_nightshade, &have_mithril, &have_adamantium, &have_shore, &is_unexplored);
...
"""
City Resources
Maximum Pop 10
Prod Bonus +22%
gold_units Bonus +10%
"""

AI_ProcessSettlers()
    Compute_Base_Map_Square_Values(wx, wy, wp, &maximum_population, &production_bonus, &gold_bonus, &unit_cost_reduction, &gold_units, &magic_units, &have_nightshade, &have_mithril, &have_adamantium, &have_shore, &is_unexplored);


*/
void Compute_Base_Map_Square_Values(int16_t wx, int16_t wy, int16_t wp, int16_t *MaxPop, int16_t *production_bonus, int16_t *gold_bonus, int16_t *unit_cost_reduction, int16_t *gold_units, int16_t *magic_units, int16_t *have_nightshade, int16_t *have_mithril, int16_t *have_adamantium, int16_t *have_shore, int16_t *is_unexplored)
{
    int16_t city_idx;
    int16_t itr;
    int16_t map_square_has_city;
    int16_t Tile_Food;
    int16_t food2_remainder;
    int16_t Bit_Index;
    int16_t itr_wx;
    int16_t itr_wy;  // _DI_
    int16_t curr_wx;  // _SI_

    // DONT  EMM_Map_DataH();  // ; maps the EMM Data block into the page frame

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

    *gold_bonus = Square_Gold_Bonus(wx, wy, wp);

    for(itr_wy = (wy - 2); ((itr_wy < (wy + 3)) && (itr_wy > 0) && (itr_wy < WORLD_HEIGHT)); itr_wy++)
    {

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
                curr_wx = WORLD_WIDTH;
            }

            if(curr_wx > WORLD_WIDTH)
            {
                curr_wx -= WORLD_WIDTH;
            }

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

                *MaxPop = Square_Food2(curr_wx, itr_wy, wp);

                if((GET_TERRAIN_SPECIAL(curr_wx, itr_wy, wp) & ts_Wild_Game) != 0)
                {
                    *MaxPop += 2;  // ; BUG: should be 8 (counting quarter foods here)
                }

                *production_bonus = Square_Production_Bonus(curr_wx, itr_wy, wp, ST_FALSE);

                *unit_cost_reduction = Square_Unit_Cost_Reduction(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE);

                SETMAX(*unit_cost_reduction, 50);  // The bonus is cumulative ... up to a maximum reduction in cost of 50%.

                *gold_units = Square_Gold_Income(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE);

                *magic_units = Square_Magic_Power(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE);

            }
            else
            {

                Tile_Food = Square_Food2(curr_wx, itr_wy, wp);

                if((GET_TERRAIN_SPECIAL(curr_wx, itr_wy, wp) & ts_Wild_Game) != 0)
                {
                    Tile_Food += 2;  // ; BUG: should be 8 (counting quarter foods here)
                }

                *MaxPop += ((Tile_Food + food2_remainder) / 2);

                food2_remainder = ((Tile_Food + food2_remainder) % 2);

                *production_bonus = (Square_Production_Bonus(curr_wx, itr_wy, wp, ST_FALSE) / 2);

                *unit_cost_reduction = (Square_Unit_Cost_Reduction(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE) / 2);

                SETMAX(*unit_cost_reduction, 50);  // The bonus is cumulative ... up to a maximum reduction in cost of 50%.

                *gold_units = (Square_Gold_Income(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE) / 2);

                *magic_units = (Square_Magic_Power(curr_wx, itr_wy, wp, ST_FALSE, ST_FALSE) / 2);

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
                *have_adamantium = ST_TRUE;
                *have_mithril = ST_FALSE;
            }

        }

    }

    // HERE: *MaxPop is ...
    // *MaxPop = Square_Food2(curr_wx, itr_wy, wp);
    // *MaxPop += ((Tile_Food + food2_remainder) / 2);
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

        if((_CITIES[city_idx].population / 3) < *gold_bonus)
        {
            *gold_bonus = (_CITIES[city_idx].population / 3);
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

    for(itr_wy = (wy - 1); ((wy + 2) <= itr_wy); itr_wy++)
    {

        if(
            (itr_wy > 0)
            &&
            (itr_wy < WORLD_HEIGHT)
        )
        {

            for(itr_wx = (wx - 1); (wx + 2) > itr_wx; itr_wx++)
            {

                curr_wx = itr_wx;

                if(curr_wx < 0)
                {
                    curr_wx += WORLD_WIDTH;
                }

                if(curr_wx > WORLD_WIDTH)
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

    // TODO  EMM_Map_DataH();

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
// drake178: TILE_IsShared
/*
    ~== Check_Square_Scouted() in Explore.C for square_scouted_p0/p1
"shared [world] map square"
*/
int16_t City_Area_Square_Is_Shared(int16_t city_area_square_wx, int16_t city_area_square_wy, int16_t city_wp)
{
    int16_t city_area_bit_index;
    int16_t is_shared;  // DNE in Dasm

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

    // TODO  EMM_Map_DataH()

    // movement_mode_cost_maps + (wp * sizeof(struct s_MOVE_MODE_COST_MAPS)) + (wy * WORLD_WIDTH) + wx
    movement_mode_cost_maps[wp].UU_MvMd.moves2[((wy * WORLD_WIDTH) + wx)] = 2;
    movement_mode_cost_maps[wp].walking.moves2[((wy * WORLD_WIDTH) + wx)] = 2;
    movement_mode_cost_maps[wp].forester.moves2[((wy * WORLD_WIDTH) + wx)] = 2;
    movement_mode_cost_maps[wp].mountaineer.moves2[((wy * WORLD_WIDTH) + wx)] = 2;
    movement_mode_cost_maps[wp].swimming.moves2[((wy * WORLD_WIDTH) + wx)] = 2;

    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] &= 0xF7;  // not MSF_ROAD

    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] &= 0xEF;  // not TF_Enc_Road

}


// WZD o142p32
// CTY_Food_Breakdown()

// WZD o142p33
// CTY_Gold_Breakdown()

// WZD o142p34
// CTY_Prod_Breakdown()

// WZD o142p35
// CTY_Rsrc_Breakdown()

// WZD o142p36
// CTY_Pwr_Breakdown()
