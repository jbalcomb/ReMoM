/*
    City Calculations

    WIZARDS.EXE
        ovr120
        ovr142
*/

#include "MoM.H"
#include "CITYCALC.H"



/*
    WIZARDS.EXE  ovr120
*/


// WZD o120p01
// drake178: CTY_Recalculate()
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Do_City_Calculations()\n", __FILE__, __LINE__);
#endif

    // drake178: ; maps the EMM Data block into the page frame
    // TODO  EMM_Map_DataH();

    _CITIES[city_idx].food_units            = City_Food_Production(city_idx);
    _CITIES[city_idx].production_units      = City_Production_Production(city_idx);
    _CITIES[city_idx].gold_units            = City_Gold_Production(city_idx);
    _CITIES[city_idx].building_maintenance  = City_Gold_Mainanence(city_idx);
    _CITIES[city_idx].research_units        = City_Research_Production(city_idx);
    _CITIES[city_idx].mana_units            = City_Mana_Production(city_idx);

    if( (_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX) && (_CITIES[city_idx].owner_idx != NEUTRAL_PLAYER_IDX) )
    {
        _CITIES[city_idx].food_units = ((_CITIES[city_idx].food_units * difficulty_modifiers_table[_difficulty].food) / 100);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Do_City_Calculations()\n", __FILE__, __LINE__);
#endif

}


// WZD o120p02
// drake178: WIZ_SetPowerBases()
/*
    sets
    _players[].magic_power

    nodes
        conjunctions
        mastery

    sum of s_CITY.mana_units

    _players[].Volcano_Count

    _players.Spell_Cast Spell_Of_Return

*/
void Players_Update_Magic_Power(void)
{
    int16_t UU_players_power_base_array[6];
    int16_t node_owner_idx;

    int16_t itr;  // _SI_
    int16_t node_magic_power_points;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Players_Update_Magic_Power()\n", __FILE__, __LINE__);
#endif

    for(itr = 0; itr < _num_players; itr++)
    {
        UU_players_power_base_array[itr] = 0;
    }

    if(*(events_table + 48) != 2)  /* s_EVENT_DATA.Mana_Short.Status */
    {
        for(itr = 0; itr < NUM_NODES; itr++)
        {
            if(TBL_Nodes[itr].Owner > -1)
            {
                node_owner_idx = TBL_Nodes[itr].Owner;

                if( (TBL_Nodes[itr].Meld_Flags & 0x01 /* M_Warped */) != 1)
                {
                    node_magic_power_points = (TBL_Nodes[itr].Power * _magic);
                    
                    /*
                        Conjunction - Chaos (Red)
                    */
                    if(*(events_table + 42) == 2)  /* s_EVENT_DATA.Conjunction_Chaos.Status */
                    {
                        if(TBL_Nodes[itr].Node_Type == 0)  /* NODE_Sorcery */
                        {
                            node_magic_power_points = (node_magic_power_points / 2);
                        }
                        if(TBL_Nodes[itr].Node_Type == 1)  /* NODE_Nature */
                        {
                            node_magic_power_points = (node_magic_power_points / 2);
                        }
                        if(TBL_Nodes[itr].Node_Type == 2)  /* NODE_Chaos */
                        {
                            node_magic_power_points = (node_magic_power_points * 2);
                        }
                    }

                    /*
                        Conjunction - Sorcery (Blue)
                    */
                    if(*(events_table + 46) == 2)  /* s_EVENT_DATA.Conjunction_Sorcery.Status */
                    {
                        if(TBL_Nodes[itr].Node_Type == 0)  /* NODE_Sorcery */
                        {
                            node_magic_power_points = (node_magic_power_points * 2);
                        }
                        if(TBL_Nodes[itr].Node_Type == 1)  /* NODE_Nature */
                        {
                            node_magic_power_points = (node_magic_power_points / 2);
                        }
                        if(TBL_Nodes[itr].Node_Type == 2)  /* NODE_Chaos */
                        {
                            node_magic_power_points = (node_magic_power_points / 2);
                        }
                    }

                    /*
                        Conjunction - Nature (Green)
                    */
                    if(*(events_table + 44) == 2)  /* s_EVENT_DATA.Conjunction_Nature.Status */
                    {
                        if(TBL_Nodes[itr].Node_Type == 0)  /* NODE_Sorcery */
                        {
                            node_magic_power_points = (node_magic_power_points / 2);
                        }
                        if(TBL_Nodes[itr].Node_Type == 1)  /* NODE_Nature */
                        {
                            node_magic_power_points = (node_magic_power_points * 2);
                        }
                        if(TBL_Nodes[itr].Node_Type == 2)  /* NODE_Chaos */
                        {
                            node_magic_power_points = (node_magic_power_points / 2);
                        }
                    }

                    /*
                        Sorcery Mastery
                    */
                    if(
                        (TBL_Nodes[itr].Node_Type == 0)  /* NODE_Sorcery */ &&
                        (_players[node_owner_idx].sorcery_mastery != ST_FALSE)
                    )
                    {
                        node_magic_power_points = (node_magic_power_points * 2);
                    }

                    /*
                        Chaos Mastery
                    */
                    if(
                        (TBL_Nodes[itr].Node_Type == 2)  /* NODE_Chaos */ &&
                        (_players[node_owner_idx].chaos_mastery != ST_FALSE)
                    )
                    {
                        node_magic_power_points = (node_magic_power_points * 2);
                    }

                    /*
                        Nature Mastery
                    */
                    if(
                        (TBL_Nodes[itr].Node_Type == 1)  /* NODE_Nature */ &&
                        (_players[node_owner_idx].nature_mastery != ST_FALSE)
                    )
                    {
                        node_magic_power_points = (node_magic_power_points * 2);
                    }

                    /*
                        Node Mastery
                    */
                    if(_players[node_owner_idx].node_mastery != ST_FALSE)
                    {
                        node_magic_power_points = (node_magic_power_points * 2);
                    }

                    _players[node_owner_idx].Power_Base += node_magic_power_points;

                }
                else
                {
                    _players[node_owner_idx].Power_Base -= 5;
                }
            }
        }  /* END: for(itr = 0; itr < NUMTBL_Nodes; itr++) */

        for(itr = 1; itr < _num_players; itr++)
        {
            _players[itr].Power_Base = ((_players[itr].Power_Base * difficulty_modifiers_table[_difficulty].mana) / 100);
        }

        for(itr = 0; itr < _cities; itr++)
        {
            if(
                (_CITIES[itr].owner_idx > -1) &&
                (_CITIES[itr].owner_idx != NEUTRAL_PLAYER_IDX)
            )
            {
                _players[_CITIES[itr].owner_idx].Power_Base += _CITIES[itr].mana_units;
            }
        }

        for(itr = 0; itr < _num_players; itr++)
        {
            if(_players[itr].Spell_Cast != 0xD6)  /* Spell_Of_Return */
            {
                _players[itr].Power_Base += _players[itr].Volcano_Count;

                if(_players[itr].Power_Base < 0)
                {
                    _players[itr].Power_Base = 0;
                }
            }
            else
            {
                _players[itr].Power_Base = 0;
            }
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Players_Update_Magic_Power()\n", __FILE__, __LINE__);
#endif

}


// WZD o120p03
// drake178: UNIT_GetGoldUpkeep()
int16_t Unit_Gold_Upkeep(int16_t unit_idx)
{
    int16_t unit_gold_upkeep;
    int16_t unit_owner_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Gold_Upkeep()\n", __FILE__, __LINE__);
#endif

    unit_owner_idx = _UNITS[unit_idx].owner_idx;

    unit_gold_upkeep = 0;

    if( (_unit_type_table[_UNITS[unit_idx].type].Abilities & 0x01 /* Ab_Summoned */) == 0)
    {
        if(_UNITS[unit_idx].type > 0x22 /* _Chosen */)
        {
            unit_gold_upkeep += _unit_type_table[_UNITS[unit_idx].type].Upkeep;
        }
        else
        {
            // Yay Hero, Nay Noble, Nay Toren The Chosen
            if(
                (((((struct s_HERO *)( (p0_heroes + (unit_owner_idx * sizeof(p0_heroes))) + (_UNITS[unit_idx].type * sizeof(struct s_HERO)) ))->Abilities_HI) & 0x2000 /* Ab_Noble */) == 0) &&
                (_UNITS[unit_idx].type != 0x22/* _Chosen */)
            )
            {
                unit_gold_upkeep += _unit_type_table[_UNITS[unit_idx].type].Upkeep;
            }
        }
    }

    if((_UNITS[unit_idx].Mutations & 0x20) != 0)  /* R_Undead */
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Unit_Gold_Upkeep() { unit_gold_upkeep = %d }\n", __FILE__, __LINE__, unit_gold_upkeep);
#endif

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Player_Armies_Gold_Upkeep()\n", __FILE__, __LINE__);
#endif
    
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Player_Armies_Gold_Upkeep() { gold_upkeep_cost = %d }\n", __FILE__, __LINE__, gold_upkeep_cost);
#endif

    return gold_upkeep_cost;
}

// WZD o120p05
/*
    Calculate Normal Units Food Maintenance
    Normal Units
    Maintenance Cost / Upkeep Cost
    Food
    (normal units - one food unit)
*/
int16_t WIZ_ArmyUpkeep_Food(int16_t player_idx)
{
    int16_t food_upkeep_cost;
    int16_t itr_units;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WIZ_ArmyUpkeep_Food()\n", __FILE__, __LINE__);
#endif

    food_upkeep_cost = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == player_idx)
        {
            if(
                ((_unit_type_table[_UNITS[itr_units].type].Abilities & 0x1 /* Ab_Summoned */) == 0) &&
                (_UNITS[itr_units].type > 0x22 /* _Chosen */) &&
                ((_UNITS[itr_units].Mutations & 0x20 /* R_Undead */) == 0)
            )
            {
                food_upkeep_cost++;
            }
        }
    }



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: WIZ_ArmyUpkeep_Food()\n", __FILE__, __LINE__);
#endif

    return food_upkeep_cost;
}


// WZD o120p06
// drake178: WIZ_GetNUCounts()
void Players_Normal_Units(int16_t normal_units[])
{

    int16_t itr_players;  // _SI_
    int16_t itr_units;  // _SI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Players_Normal_Units()\n", __FILE__, __LINE__);
#endif

    for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
    {
        normal_units[itr_players] = 0;
    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            ((_unit_type_table[_UNITS[itr_units].type].Abilities & 0x01 /* Ab_Summoned */) == 0) &&
            (_UNITS[itr_units].type > 0x22 /* _Chosen */) &&
            (_UNITS[itr_units].owner_idx < _num_players) &&
            ((_UNITS[itr_units].Mutations & 0x20 /* R_Undead */) == 0)
        )
        {
            normal_units[_UNITS[itr_units].owner_idx]++;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Players_Normal_Units()\n", __FILE__, __LINE__);
#endif

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
    int16_t Mana;
    int16_t food_income;
    int16_t gold_income;
    int16_t mana_expense;
    int16_t food_expense;
    int16_t gold_expense;

    int16_t itr_cities;
    int16_t itr_heroes;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Player_Resource_Income_Total()\n", __FILE__, __LINE__);
#endif

    Player_Magic_Power_Income_Total(&Mana, &food_income, &gold_income, player_idx);

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
            if(_players[player_idx].Heroes[itr_heroes].Unit_Index > -1)
            {
                
                // *( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) )
                // fp_heroes[player_idx]
                // _UNITS[].type
                // _players[player_idx].Heroes[itr_heroes].Unit_Index
                // ((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) ))->Abilities_HI
                // ((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) ))->Abilities_LO
                if( ((((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) ))->Abilities_HI) & 0x2000) != 0 )  /* Ab_Noble */
                {
                    gold_income += 10;
                }
            }
        }



        *food_total = food_income - food_expense;
        // mov bx, [bp+food]; mov ax, [bx]; cwd; sub ax, dx; sar ax, 1; add [bp+gold_income], ax
        gold_income += (*food_total > 0) ? *food_total / 2 : 0;
        *gold_total = gold_income - gold_expense;

        *mana_total = Mana - mana_expense;

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Player_Resource_Income_Total()\n", __FILE__, __LINE__);
#endif

}

// WZD o120p08
/*
    Calculate Mana Maintenance Cost
        Unit Enchantments
        City Enchantment
        Overland Enchantments
        -50% for Channeler
*/
int16_t WIZ_TotalUpkeep_Mana(int16_t player_idx)
{
    int16_t mana_upkeep_cost;
    int16_t itr_units;
    int16_t itr_cities;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WIZ_TotalUpkeep_Mana()\n", __FILE__, __LINE__);
#endif

    mana_upkeep_cost = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == player_idx)
        {
            mana_upkeep_cost += UNIT_GetManaUpkeep(itr_units);
        }
    }

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        mana_upkeep_cost += CTY_EnchantsUpkeep(itr_cities, player_idx);
    }

    mana_upkeep_cost += WIZ_GlobalsUpkeep(player_idx);

    if(_players[player_idx].channeler > 0)
    {
        mana_upkeep_cost = (mana_upkeep_cost / 2);  /* -50% */
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: WIZ_TotalUpkeep_Mana()\n", __FILE__, __LINE__);
#endif

    return mana_upkeep_cost;
}

// WZD o120p09
/*

    +50% Undead Fantastic Unit
*/
int16_t UNIT_GetManaUpkeep(int16_t unit_idx)
{
    int16_t unit_mana_upkeep_cost;
    int16_t Enchant_Bit;
    // int16_t Enchants_LO;
    // int16_t Enchants_HI;
    uint32_t Enchants;
    int16_t itr_unit_enchantments;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WIZ_TotalUpkeep_Mana()\n", __FILE__, __LINE__);
#endif

    unit_mana_upkeep_cost = 0;

    // Enchants_HI = _UNITS[unit_idx].Enchants_HI;
    // Enchants_LO = _UNITS[unit_idx].Enchants_LO;
    Enchants = ( (((uint32_t)_UNITS[unit_idx].Enchants_HI) << 16) & ((uint32_t)_UNITS[unit_idx].Enchants_LO) );

    if((_unit_type_table[_UNITS[unit_idx].type].Abilities & 0x01) != 0) /* Ab_Summoned */
    {
        if((_UNITS[unit_idx].Mutations & 0x20) != 0)  /* R_Undead */
        {
            unit_mana_upkeep_cost += ((_unit_type_table[_UNITS[unit_idx].type].Upkeep * 3) / 2);  /* + 50% */
        }
        else
        {
            unit_mana_upkeep_cost += _unit_type_table[_UNITS[unit_idx].type].Upkeep;
        }
    }

    // ¿ Torin The Chosen has no mana upkeep ?  ¿ why not just get it from the table ?
    if((_UNITS[unit_idx].type & 0x22) == 0)  /* _Chosen */
    {
        unit_mana_upkeep_cost += _unit_type_table[_UNITS[unit_idx].type].Upkeep;
    }

    if(
        (_UNITS[unit_idx].owner_idx < _num_players) &&
        (_players[_UNITS[unit_idx].owner_idx].conjurer > 0)
    )
    {
        unit_mana_upkeep_cost = (((unit_mana_upkeep_cost * 3) + 3) / 4);
    }

    for(itr_unit_enchantments = 0; itr_unit_enchantments < NUM_UNIT_ENCHANTMENTS; itr_unit_enchantments++)
    {
        // Enchant_Bit = (Enchants_LO & 0x1);
        Enchant_Bit = (Enchants & 0x1);

        if(Enchant_Bit == 1)
        {
            unit_mana_upkeep_cost += unit_enchantment_upkeep_table[itr_unit_enchantments];

// mov     ax, [bp+Enchants_HI]
// mov     dx, [bp+Enchants_LO]
// sar     ax, 1
// rcr     dx, 1
// mov     [bp+Enchants_HI], ax
// mov     [bp+Enchants_LO], dx
            // Enchants_LO >>= Enchants_LO;
            Enchants = (Enchants >> 1);

        }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: WIZ_TotalUpkeep_Mana()\n", __FILE__, __LINE__);
#endif

    return unit_mana_upkeep_cost;
}

// WZD o120p10
int16_t CTY_EnchantsUpkeep(int16_t city_idx, int16_t player_idx)
{
    int16_t city_mana_upkeep_cost;
    uint8_t * CITY_Enchants;
    int16_t itr_city_enchantments;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: CTY_EnchantsUpkeep()\n", __FILE__, __LINE__);
#endif

    city_mana_upkeep_cost = 0;

    CITY_Enchants = _CITIES[city_idx].enchantments;

    // NOTE: the city_enchantment_upkeep_table has the 25 enchantments plus Nightshade
    for(itr_city_enchantments = 0; itr_city_enchantments < NUM_CITY_ENCHANTMENTS; itr_city_enchantments++)
    {
        if(CITY_Enchants[itr_city_enchantments] == (player_idx + 1))
        {
            city_mana_upkeep_cost += city_enchantment_upkeep_table[itr_city_enchantments];
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: CTY_EnchantsUpkeep()\n", __FILE__, __LINE__);
#endif

    return city_mana_upkeep_cost;
}

// WZD o120p11
int16_t WIZ_GlobalsUpkeep(int16_t player_idx)
{
    int16_t global_mana_upkeep_cost;
    uint8_t * _player_Globals;
    int16_t itr_overland_enchantments;

    _player_Globals = _players[player_idx].Globals;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WIZ_GlobalsUpkeep()\n", __FILE__, __LINE__);
#endif

    global_mana_upkeep_cost = 0;


    for(itr_overland_enchantments = 0; itr_overland_enchantments < NUM_OVERLAND_ENCHANTMENTS; itr_overland_enchantments++)
    {
        if(_player_Globals[itr_overland_enchantments] > 0)
        {
            global_mana_upkeep_cost += overland_enchantment_upkeep_table[itr_overland_enchantments];
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: WIZ_GlobalsUpkeep()\n", __FILE__, __LINE__);
#endif

    return global_mana_upkeep_cost;
}


// WZD s142p12
// drake178: CTY_GetGold()
int16_t City_Gold_Production(int16_t city_idx)
{
    int16_t wy_array[25];
    int16_t wx_array[25];
    int16_t gold_modifier;
    int16_t are_dwarf;
    int16_t have_miners_guild;
    int16_t city_wp;
    int16_t city_owner_idx;
    int16_t itr;
    int16_t useable_map_squares;

    int16_t gold_units;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: City_Gold_Production()\n", __FILE__, __LINE__);
#endif

    if(_CITIES[city_idx].population == 0)
    {
        gold_units = 0;
    }
    else
    {
        city_owner_idx = _CITIES[city_idx].owner_idx;

        city_wp = _CITIES[city_idx].world_plane;

        useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

        gold_modifier = Map_Square_Gold_Bonus(CITYX(), CITYY(), CITYP()) + City_Road_Trade_Bonus(city_idx);

        if(_CITIES[city_idx].race == 0x0B /* R_Nomad */)
        {
            gold_modifier += 50;
        }

        // Maximum Road Connection Trade Bonus
        if( (_CITIES[city_idx].population * 3) < gold_modifier)
        {
            gold_modifier = (_CITIES[city_idx].population * 3);
        }

        if(
            (_CITIES[city_idx].buildings[MINERS_GUILD] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[MINERS_GUILD] == 0x00 /* B_Replaced */)
        )
        {
            have_miners_guild = ST_TRUE;
        }
        else
        {
            have_miners_guild = ST_FALSE;
        }

        if(
            (_CITIES[city_idx].buildings[BANK] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[BANK] == 0x00 /* B_Replaced */)
        )
        {
            gold_modifier += 50;
        }

        if(
            (_CITIES[city_idx].buildings[MARKETPLACE] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[MARKETPLACE] == 0x00 /* B_Replaced */)
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
            gold_units = (((_CITIES[city_idx].population - City_Rebel_Count(city_idx)) * _players[city_owner_idx].tax_rate) /2);
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

            gold_units += Map_Square_Gold_Income(CITYX(), CITYY(), CITYP(), have_miners_guild, are_dwarf);
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: City_Gold_Production() { gold_units = %d }\n", __FILE__, __LINE__, gold_units);
#endif

    return gold_units;
}


// WZD o120p13
// WZD o120p14
// WZD o120p15
// WZD o120p16


// WZD o120p17
/*
    in:
        player_idx
    in-out:
        mana, skill, research
    out:
        N/A
    

*/
void Get_Power_Incomes_Base(int16_t * Mana, int16_t * Skill, int16_t * Research, int16_t player_idx)
{
    int16_t City_Research;
    int16_t Research_Income;
    int16_t Skill_Income;
    int16_t Mana_Income;
    int16_t Power_Base;

    int16_t itr_cities;
    int16_t itr_heroes;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_Power_Incomes_Base()\n", __FILE__, __LINE__);
#endif

    Power_Base = _players[player_idx].Power_Base;

    Mana_Income = (((Power_Base * 100) + 50) / _players[player_idx].Mana_Pnct);
    Skill_Income = (((Power_Base * 100) + 50) / _players[player_idx].Skill_Pcnt);
    Research_Income = Power_Base - Mana_Income - Skill_Income;

    if( (_players[player_idx].Research_Pcnt == 0) && (Research_Income > 0) )
    {
        if(_players[player_idx].Skill_Pcnt == 0)
        {
            Mana_Income += Research_Income;
        }
        else
        {
            Skill_Income += Research_Income;
        }
        Research_Income = 0;
    }

    if(_players[player_idx].mana_focusing > 0)
    {
        Mana_Income += (Mana_Income / 4);  /* +25% */
    }

    City_Research = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            City_Research += _CITIES[itr_cities].research_units;
        }
    }

    Research_Income += City_Research;

    for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
    {
        if(_players[player_idx].Heroes[itr_heroes].Unit_Index > -1)
        {
            
            // *( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) )
            // fp_heroes[player_idx]
            // _UNITS[].type
            // _players[player_idx].Heroes[itr_heroes].Unit_Index
            // ((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) ))->Abilities_HI
            // ((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) ))->Abilities_LO
            if( ((((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) ))->Abilities_HI) & 0x20) != 0 )  /* Ab_Sage */
            {
                Research_Income += (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].Level * 3);
            }
            if( ((((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) ))->Abilities_HI) & 0x40) != 0 )  /* Ab_Sage2 */
            {
                Research_Income += (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].Level * 9);
            }
        }
    }

    *Mana = Mana_Income;
    *Skill = Skill_Income;
    *Research = Research_Income;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Get_Power_Incomes_Base()\n", __FILE__, __LINE__);
#endif

}

// WZD o120p18
/*
Get_Power_Incomes()
XREF:
j_Get_Power_Incomes()
Get_Incomes()

j_Get_Power_Incomes()
XREF:
    Magic_Screen_Draw()
    WIZ_PowerIncomes()
    AI_Research_Picker()

Get_Incomes()
XREF:
    Main_Screen()
    Main_Screen_Draw_Summary_Window()
    AI_Tax_And_Farmers()
    AI_OVL_SpellPicker()
    AI_Balance_Upkeep()
*/
/*


    NOT *base* spell research, because it includes the research bonus, given the specific spell and special abilities
*/
void Get_Power_Incomes(int16_t * Mana, int16_t * Research, int16_t * Skill, int16_t player_idx)
{
    int16_t Base_Skill_Income;
    int16_t Base_Research_Income;
    int16_t Base_Mana_Income;
    int16_t Research_Bonus;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_Power_Incomes()\n", __FILE__, __LINE__);
#endif


    if( (_players[player_idx].Spell_Cast & 0xD6 /* Spell_Of_Return */) != 0 )
    {
        *Mana = 0;
        *Research = 0;
        *Skill = 0;
    }
    else
    {
        Get_Power_Incomes_Base(&Base_Research_Income, &Base_Skill_Income, &Base_Mana_Income, player_idx);
            // ; converts and fills out the return values with the
            // ; Mana, Skill, and Research Point incomes, adding
            // ; RPs from cities and Heroes, but not applying any
            // ; retorts to any of the incomes except Mana Focusing

        Research_Bonus = Get_Research_Bonus(player_idx, _players[player_idx].Researching);
            // ; calculates and returns the research bonus percentage
            // ; that applies to the target spell based on the
            // ; wizard's profile traits (spellbooks & retorts)

        if(Base_Research_Income < 0)
        {
            Base_Research_Income = 0;
        }
        if(Base_Mana_Income < 0)
        {
            Base_Mana_Income = 0;
        }
        if(Base_Skill_Income < 0)
        {
            Base_Skill_Income = 0;
        }

        Base_Research_Income = (Base_Research_Income + ( (Base_Research_Income * Research_Bonus) / 100));

        *Mana = Base_Mana_Income;
        *Research = Base_Research_Income;
        *Skill = Base_Skill_Income;

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Get_Power_Incomes()\n", __FILE__, __LINE__);
#endif

}

// WZD o120p19
int16_t Get_Research_Bonus(int16_t player_idx, int16_t spell_idx)
{
    int16_t Spell_Realm;

    int16_t research_bonus;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_Research_Bonus()\n", __FILE__, __LINE__);
#endif

    research_bonus = 0;

    Spell_Realm = spell_data_table[spell_idx].Realm;

    if(_players[player_idx].sage_master > 0)
    {
        research_bonus += 25;
    }

    if( (_players[player_idx].conjurer > 0) && ( (spell_data_table[spell_idx].Type & 0x00 /* Summoning_Spell */) != 0) )
    {
        research_bonus += 25;
    }

    switch(Spell_Realm)
    {
        case 0:  /* Nature */
        {
            DLOG("switch(Spell_Realm)  case 0:");
            if(_players[player_idx].nature_mastery > 0)
            {
                research_bonus += 15;
            }
            if(_players[player_idx].spellrank_nature > 7)
            {
                research_bonus += ((_players[player_idx].spellrank_nature - 7) * 10);
            }
        } break;
        case 1:  /* Sorcery */
        {
            DLOG("switch(Spell_Realm)  case 1:");
            if(_players[player_idx].sorcery_mastery > 0)
            {
                research_bonus += 15;
            }
            if(_players[player_idx].spellrank_sorcery > 7)
            {
                research_bonus += ((_players[player_idx].spellrank_sorcery - 7) * 10);
            }
        } break;
        case 2:  /* Chaos */
        {
            DLOG("switch(Spell_Realm)  case 2:");
            if(_players[player_idx].chaos_mastery > 0)
            {
                research_bonus += 15;
            }
            if(_players[player_idx].spellrank_chaos > 7)
            {
                research_bonus += ((_players[player_idx].spellrank_chaos - 7) * 10);
            }
        } break;
        case 3:  /* Life */
        {
            DLOG("switch(Spell_Realm)  case 3:");
            if(_players[player_idx].spellrank_life > 7)
            {
                research_bonus += ((_players[player_idx].spellrank_life - 7) * 10);
            }
        } break;
        case 4:  /* Death */
        {
            DLOG("switch(Spell_Realm)  case 4:");
            if(_players[player_idx].spellrank_death > 7)
            {
                research_bonus += ((_players[player_idx].spellrank_death - 7) * 10);
            }
        } break;
        case 5:  /* Arcane */
        {
            DLOG("switch(Spell_Realm)  case 5:");
            if(_players[player_idx].runemaster > 0)
            {
                research_bonus += 25;
            }
        } break;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Get_Research_Bonus()\n", __FILE__, __LINE__);
#endif

    return research_bonus;
}

// WZD o120p20
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Player_Fame()\n", __FILE__, __LINE__);
#endif

    fame_points = 0;

    for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
    {
        if(_players[player_idx].Heroes[itr_heroes].Unit_Index > -1)
        {
            hero_unit_idx = _players[player_idx].Heroes[itr_heroes].Unit_Index;
            hero_unit_type = _UNITS[hero_unit_idx].type;
// TODO              // p_heroes + (player_idx * 35 hero structs) + (type * sizeof s_HERO) + offset in struct to Abilities_LO
// TODO              if( ((((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) ))->Abilities_LO) & 0x08) != 0 )  /* Ab_Legendary */
// TODO              {
// TODO                  fame_points += ((_UNITS[hero_unit_idx].Level + 1) * 3);
// TODO              }
// TODO              if( ((((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) ))->Abilities_LO) & 0x10) != 0 )  /* Ab_Legendary2 */
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

    fame_points += _players[player_idx].Fame;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Player_Fame() { fame_points = %d }\n", __FILE__, __LINE__, fame_points);
#endif

    return fame_points;
}


// WZD o120p21


// WZD o120p22
// drake178: CTY_GetGoldUpkeep()
int16_t City_Gold_Mainanence(int16_t city_idx)
{
    int16_t gold_units;  // _DI_
    int16_t itr;  //  _SI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: City_Gold_Mainanence()\n", __FILE__, __LINE__);
#endif

    gold_units = 0;

    for(itr = 0; itr < NUM_BUILDINGS; itr++)
    {
        if(
            (_CITIES[city_idx].buildings[itr] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[itr] == 0x00 /* B_Replaced */)
        )
        {
            if(build_data_table[itr].maintenance_cost != 0)
            {
                gold_units += abs(build_data_table[itr].maintenance_cost);
            }
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: City_Gold_Mainanence()\n", __FILE__, __LINE__);
#endif

    return gold_units;
}


// WZD o120p23
// WZD o120p24
// WZD o120p25
// WZD o120p26




/*
    WIZARDS.EXE  ovr120
*/

// WZD s142p01
// WZD s142p02
// WZD s142p03
// WZD s142p04
// WZD s142p05

// WZD s142p06
// drake178: CTY_GetTileFood()
int16_t City_Food_Terrain(int16_t city_idx)
{
    int16_t wy_array[25];
    int16_t wx_array[25];
    int16_t city_wp;
    int16_t useable_map_squares;
    
    int16_t food2_units;  // _DI_
    int16_t itr;  // _SI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: City_Food_Terrain()\n", __FILE__, __LINE__);
#endif

    city_wp = _CITIES[city_idx].world_plane;

    // NOTE: Accounts for 'Corruption'
    // useable_map_squares = Get_Useable_City_Area(_CITIES[city_idx].world_x, _CITIES[city_idx].world_y, city_wp, &wx_array[0], &wy_array[0]);
    useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

    food2_units = 0;

    for(itr = 0; itr < useable_map_squares; itr++)
    {
        food2_units += Map_Square_Food2(wx_array[itr], wy_array[itr], city_wp);
    }

    if(_CITIES[city_idx].enchantments[GAIAS_BLESSING] != ST_FALSE)
    {
        food2_units = ((food2_units * 3) / 2);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: City_Food_Terrain()\n", __FILE__, __LINE__);
#endif

    return food2_units / 4;
}

// WZD s142p07
// drake178: CTY_GetWildGameFood()
int16_t City_Food_WildGame(int16_t city_idx)
{
    int16_t wy_array[25];
    int16_t wx_array[25];
    int16_t bit_index;
    int16_t city_wp;
    int16_t useable_map_squares;
    
    int16_t food_units;  // _DI_
    int16_t itr;  // _SI_
    uint8_t * bit_field;  // _DX_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: City_Food_WildGame()\n", __FILE__, __LINE__);
#endif

    city_wp = _CITIES[city_idx].world_plane;

    // NOTE: Accounts for 'Corruption'
    // useable_map_squares = Get_Useable_City_Area(_CITIES[city_idx].world_x, _CITIES[city_idx].world_y, city_wp, &wx_array[0], &wy_array[0]);
    useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

    food_units = 0;

    for(itr = 0; itr < useable_map_squares; itr++)
    {
        if( (*((uint8_t *)&TBL_Terr_Specials[((city_wp * WORLD_SIZE) + (wx_array[itr] * WORLD_WIDTH) + wx_array[itr])]) & 0x40 /* TS_Wild_Game */) != 0)
        {
            bit_index = ((wy_array[itr] * WORLD_WIDTH) + wx_array[itr]);
            bit_field = (square_shared_bits + (city_wp * WORLD_SIZE) );
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: City_Food_WildGame()\n", __FILE__, __LINE__);
#endif

    return food_units;

}


// WZD s142p08
// drake178: CTY_GetCatchment()
// does like Draw_Map_Roads() with itr_world_x and curr_world_x
/*
    in-out wx_array
    in-out wy_array
    returns count of elements in arrays  AKA map_square_count

    NOTE: Accounts for 'Corruption'
*/
int16_t Get_Useable_City_Area(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t *wx_array, int16_t *wy_array)
{
    int16_t TFlags_RowBase;
    uint8_t * terrain_flags_table_row;
    int16_t square_y;
    int16_t itr_world_x;
    int16_t square_x_max;
    int16_t square_x_min;
    int16_t map_square_count;

    int16_t itr_city_area_squares;
    int16_t square_x;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_Useable_City_Area()\n", __FILE__, __LINE__);
#endif

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

            terrain_flags_table_row = (uint8_t *)&TBL_Terrain_Flags[(city_wp * WORLD_SIZE) + (square_y * WORLD_WIDTH)];

            itr_world_x = square_x_min;

            while(itr_world_x < square_x_max)
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
                if( (*(terrain_flags_table_row + square_x) & 0x20 == 0) ) /* TF_Corruption */
                {
                    wx_array[map_square_count] = square_x;
                    wy_array[map_square_count] = square_y;
                    map_square_count++;
                }
                itr_world_x++;
            }
        }
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Get_Useable_City_Area()\n", __FILE__, __LINE__);
#endif

    return map_square_count;
}


// WZD s142p09
// drake178: CTY_GetFood()
int16_t City_Food_Production(int16_t city_idx)
{
    int16_t city_area_food_units;

    int16_t food_units;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: City_Food_Production()\n", __FILE__, __LINE__);
#endif

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
            (_CITIES[city_idx].buildings[ANIMISTS_GUILD] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[ANIMISTS_GUILD] == 0x00 /* B_Replaced */)
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
            (_CITIES[city_idx].buildings[FORESTERS_GUILD] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[FORESTERS_GUILD] == 0x00 /* B_Replaced */)
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
            food_units += ((food_units - city_area_food_units) / 2);
        }

        if(
            (_CITIES[city_idx].buildings[GRANARY] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[GRANARY] == 0x00 /* B_Replaced */)
        )
        {
            food_units += 2;
        }

        if(
            (_CITIES[city_idx].buildings[FARMERS_MARKET] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[FARMERS_MARKET] == 0x00 /* B_Replaced */)
        )
        {
            food_units += 2;
        }

        food_units += City_Food_WildGame(city_idx);

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: City_Food_Production()\n", __FILE__, __LINE__);
#endif

    return food_units;
}


// WZD s142p10
// WZD s142p11

// WZD s142p11
// drake178: CTY_GetProd()
int16_t City_Production_Production(int16_t city_idx)
{
    int16_t wy_array[25];
    int16_t wx_array[25];
    int16_t production2_per_worker;
    int16_t have_gaias_blessing;
    int16_t city_wp;
    int16_t City_Owner;
    int16_t production_units;
    int16_t worker_count;
    int16_t itr;
    int16_t useable_map_squares;

    int16_t production_modifier;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: City_Production_Production()\n", __FILE__, __LINE__);
#endif

    if(_CITIES[city_idx].population == 0)
    {
        production_units = 0;
    }
    else
    {
        city_wp = _CITIES[city_idx].world_plane;

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

        production_units = ((((worker_count * production2_per_worker) + _CITIES[city_idx].farmer_count) + 1) /2);

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
            production_modifier += Map_Square_Production_Bonus(wx_array[itr], wy_array[itr], city_wp, have_gaias_blessing);
        }

        if(
            (_CITIES[city_idx].buildings[FORESTERS_GUILD] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[FORESTERS_GUILD] == 0x00 /* B_Replaced */)
        )
        {
            production_modifier += 25;
        }

        if(
            (_CITIES[city_idx].buildings[SAWMILL] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[SAWMILL] == 0x00 /* B_Replaced */)
        )
        {
            production_modifier += 25;
        }

        if(
            (_CITIES[city_idx].buildings[MINERS_GUILD] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[MINERS_GUILD] == 0x00 /* B_Replaced */)
        )
        {
            production_modifier += 50;
        }

        if(
            (_CITIES[city_idx].buildings[MECHANICIANS_GUILD] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[MECHANICIANS_GUILD] == 0x00 /* B_Replaced */)
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: City_Production_Production()\n", __FILE__, __LINE__);
#endif

    return production_units;
}


// WZD s142p13
// drake178: CTY_GetResearch()
int16_t City_Research_Production(int16_t city_idx)
{
    int16_t city_owner_idx;

    int16_t research_units;  // _SI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: City_Production_Production()\n", __FILE__, __LINE__);
#endif

    city_owner_idx = _CITIES[city_idx].owner_idx;

    if(
        (_CITIES[city_idx].population == 0) &&
        (city_owner_idx != NEUTRAL_PLAYER_IDX)
    )
    {

        research_units = 0;

        if(
            (_CITIES[city_idx].buildings[LIBRARY] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[LIBRARY] == 0x00 /* B_Replaced */)
        )
        {
            research_units += 2;
        }

        if(
            (_CITIES[city_idx].buildings[SAGES_GUILD] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[SAGES_GUILD] == 0x00 /* B_Replaced */)
        )
        {
            research_units += 3;
        }

        if(
            (_CITIES[city_idx].buildings[UNIVERSITY] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[UNIVERSITY] == 0x00 /* B_Replaced */)
        )
        {
            research_units += 5;
        }

        if(
            (_CITIES[city_idx].buildings[WIZARDS_GUILD] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[WIZARDS_GUILD] == 0x00 /* B_Replaced */)
        )
        {
            research_units += 8;
        }

    }
    else
    {
        research_units = 0;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: City_Production_Production()\n", __FILE__, __LINE__);
#endif

    return research_units;
}


// WZD s142p14
// drake178: CTY_GetPower()
int16_t City_Mana_Production(int16_t city_idx)
{
    int16_t wy_array[25];
    int16_t wx_array[25];
    int16_t spell_ranks;
    int16_t have_miners_guild;
    int16_t are_dwarf;
    int16_t city_wp;
    int16_t useable_map_squares;
    int16_t building_magic_power;
    int16_t building_magic_power_modifier;
    int16_t city_owner_idx;
    int16_t itr;

    int16_t mana_units;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: City_Mana_Production()\n", __FILE__, __LINE__);
#endif

    city_owner_idx = _CITIES[city_idx].owner_idx;

    // s_EVENT_DATA.Mana_Short.Status
    if(
        // (*((int16_t *)(&events_table + 0x60)) != 2) &&
        (*(events_table + 48) != 2) &&
        (_CITIES[city_idx].population == 0) &&
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
            (_CITIES[city_idx].buildings[SHRINE] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[SHRINE] == 0x00 /* B_Replaced */)
        )
        {
            building_magic_power += 1;
        }

        if(
            (_CITIES[city_idx].buildings[TEMPLE] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[TEMPLE] == 0x00 /* B_Replaced */)
        )
        {
            building_magic_power += 2;
        }

        if(
            (_CITIES[city_idx].buildings[PARTHENON] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[PARTHENON] == 0x00 /* B_Replaced */)
        )
        {
            building_magic_power += 3;
        }

        if(
            (_CITIES[city_idx].buildings[CATHEDRAL] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[CATHEDRAL] == 0x00 /* B_Replaced */)
        )
        {
            building_magic_power += 4;
        }

        building_magic_power = ((building_magic_power * building_magic_power_modifier) / 100);

        mana_units = building_magic_power;

        if(
            (_CITIES[city_idx].buildings[ANIMISTS_GUILD] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[ANIMISTS_GUILD] == 0x00 /* B_Replaced */)
        )
        {
            mana_units += 3;
        }

        if(
            (_CITIES[city_idx].buildings[WIZARDS_GUILD] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[WIZARDS_GUILD] == 0x00 /* B_Replaced */)
        )
        {
            mana_units += 3;
        }

        city_wp = _CITIES[city_idx].world_plane;

        if(_CITIES[city_idx].race = 0x02 /* R_Dark_Elf */)
        {
            mana_units += _CITIES[city_idx].population;
        }

        if(
            (_CITIES[city_idx].race = 0x07 /* R_High_Elf */) ||
            (_CITIES[city_idx].race = 0x01 /* R_Beastman */) ||
            (_CITIES[city_idx].race = 0x03 /* R_Draconian */)
        )
        {
            mana_units += (_CITIES[city_idx].population / 2);
        }

        if(
            (_FORTRESSES[city_owner_idx].world_x == _CITIES[city_idx].world_x) &&
            (_FORTRESSES[city_owner_idx].world_y == _CITIES[city_idx].world_y) &&
            (_FORTRESSES[city_owner_idx].world_plane == city_wp)
        )
        {
            spell_ranks = (
                _players[city_owner_idx].spellrank_nature + 
                _players[city_owner_idx].spellrank_sorcery +
                _players[city_owner_idx].spellrank_chaos +
                _players[city_owner_idx].spellrank_life +
                _players[city_owner_idx].spellrank_death
            );

            if(city_wp != 0)
            {
                mana_units += spell_ranks + 5;
            }
            else
            {
                mana_units += spell_ranks;
            }

            if(_CITIES[city_idx].race == 0x04 /* R_Dwarf */)
            {
                are_dwarf = ST_TRUE;
            }

            if(
                (_CITIES[city_idx].buildings[MINERS_GUILD] == 0x01 /* B_Built */) ||
                (_CITIES[city_idx].buildings[MINERS_GUILD] == 0x00 /* B_Replaced */)
            )
            {
                have_miners_guild = ST_TRUE;
            }

            useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

            for(itr = 0; itr < useable_map_squares; itr++)
            {
                mana_units += Map_Square_Magic_Power(wx_array[itr], wy_array[itr], city_wp, have_miners_guild, are_dwarf);
            }

            if(
                (_players[city_owner_idx].spellrank_death != 0) &&
                // (events_table->Good_Moon.Status == 2)
            (*(events_table + 38) == 2)
            )
            {
                mana_units = (mana_units - (building_magic_power / 2));
            }

            if(
                (_players[city_owner_idx].spellrank_life != 0) &&
                // (events_table->Bad_Moon.Status == 2)
            (*(events_table + 40) == 2)
            )
            {
                mana_units = (mana_units - (building_magic_power / 2));
            }

            if(mana_units < 0)
            {
                mana_units = 0;
            }

            if(
                (_players[city_owner_idx].spellrank_life != 0) &&
                // (events_table->Good_Moon.Status == 2)
            (*(events_table + 38) == 2)
            )
            {
                mana_units += building_magic_power;
            }

            if(
                (_players[city_owner_idx].spellrank_death != 0) &&
                // (events_table->Bad_Moon.Status == 2)
            (*(events_table + 40) == 2)
            )
            {
                mana_units += building_magic_power;
            }

        }

    }
    else
    {
        mana_units = 0;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: City_Mana_Production()\n", __FILE__, __LINE__);
#endif

    return mana_units;
}


// WZD s142p15
// WZD s142p16
// WZD s142p17
// WZD s142p18
// WZD s142p19
// WZD s142p20
// WZD s142p21
// WZD s142p22
// WZD s142p23
// WZD s142p24
// WZD s142p25
// WZD s142p26

// WZD s142p27
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
    int16_t itr_num_players;
    int16_t city_owner_idx;
    int16_t rebel_count;

    int16_t itr_units;  // DNE in Dasm

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: City_Map_Square_Is_Shared()\n", __FILE__, __LINE__);
#endif

    city_owner_idx = _CITIES[city_idx].owner_idx;
    city_wx = _CITIES[city_idx].world_x;
    city_wy = _CITIES[city_idx].world_y;
    city_wp = _CITIES[city_idx].world_plane;

    unrest_percent = tax_unrest_pct_table[_players[city_owner_idx].tax_rate];
    unrest_percent = TBL_Unrest[_players[city_owner_idx].Cptl_Race][_CITIES[city_idx].race];

    if(_CITIES[city_idx].enchantments[FAMINE] != ST_FALSE)
    {
        unrest_percent += 25;
    }

    rebel_count = ((_CITIES[city_idx].population * unrest_percent) / 100);

    Pacify = 0;

    if(
        (_CITIES[city_idx].enchantments[EVIL_PRESENCE] != ST_FALSE) &&
        (_players[city_owner_idx].spellrank_death == 0)
    )
    {

        Pacify_Religion = 0;

        if(
            (_CITIES[city_idx].buildings[SHRINE] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[SHRINE] == 0x00 /* B_Replaced */)
        )
        {
            Pacify_Religion += 1;
        }

        if(
            (_CITIES[city_idx].buildings[TEMPLE] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[TEMPLE] == 0x00 /* B_Replaced */)
        )
        {
            Pacify_Religion += 1;
        }

        if(
            (_CITIES[city_idx].buildings[PARTHENON] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[PARTHENON] == 0x00 /* B_Replaced */)
        )
        {
            Pacify_Religion += 1;
        }

        if(
            (_CITIES[city_idx].buildings[CATHEDRAL] == 0x01 /* B_Built */) ||
            (_CITIES[city_idx].buildings[CATHEDRAL] == 0x00 /* B_Replaced */)
        )
        {
            Pacify_Religion += 1;
        }

        if(
            (_players[city_owner_idx].infernal_power != ST_FALSE) ||
            (_players[city_owner_idx].divine_power != ST_FALSE)
        )
        {
            Pacify_Religion += (Pacify_Religion / 2);
        }

        Pacify += Pacify_Religion;
    }

    if(
        (_CITIES[city_idx].buildings[ORACLE] == 0x01 /* B_Built */)
    )
    {
        Pacify += 2;
    }

    if(
        (_CITIES[city_idx].buildings[ANIMISTS_GUILD] == 0x01 /* B_Built */)
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
                (_UNITS[itr_units].world_x == city_wx) &&
                (_UNITS[itr_units].world_y == city_wy) &&
                (_UNITS[itr_units].world_plane == city_wp) &&
                (_UNITS[itr_units].type != 0x9A /* U_Magic_Spirit */)
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


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: City_Map_Square_Is_Shared()\n", __FILE__, __LINE__);
#endif

    return rebel_count;
}


// WZD s142p28
// WZD s142p29


// WZD s142p30
// drake178: TILE_IsShared
/*
    ~== Check_Square_Scouted() in Explore.C for square_scouted_p0/p1

*/
int16_t City_Map_Square_Is_Shared(int16_t city_wx, int16_t city_wy, int16_t city_wp)
{
    int16_t bit_index;

    uint8_t * bit_field;
    int16_t is_shared;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: City_Map_Square_Is_Shared()\n", __FILE__, __LINE__);
#endif

    bit_index = ((city_wy * WORLD_WIDTH) + city_wx);

    bit_field = (square_shared_bits + (city_wp * WORLD_SIZE) );

    if(Test_Bit_Field(bit_index, bit_field) == ST_FALSE)
    {
        is_shared = ST_FALSE;
    }
    else
    {
        is_shared = ST_TRUE;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: City_Map_Square_Is_Shared()\n", __FILE__, __LINE__);
#endif

    return is_shared;
}


// WZD s142p31
// WZD s142p32
// WZD s142p33
// WZD s142p34
// WZD s142p35
// WZD s142p36
