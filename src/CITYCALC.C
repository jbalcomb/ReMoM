/*
    City Calculations

    WIZARDS.EXE
        ovr120
*/

#include "MoM.H"
#include "CITYCALC.H"



/*
    WIZARDS.EXE  ovr120
*/


// WZD o120p01
// CTY_Recalculate()

// WZD o120p02

// WZD o120p03
int16_t UNIT_GetGoldUpkeep(int16_t unit_idx)
{
    int16_t unit_gold_upkeep;
    int16_t unit_owner_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_GetGoldUpkeep()\n", __FILE__, __LINE__);
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
    dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_GetGoldUpkeep()\n", __FILE__, __LINE__);
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
int16_t WIZ_ArmyUpkeep_Gold(int16_t player_idx)
{
    int16_t fame_points;
    int16_t gold_upkeep_cost;
    int16_t itr_units;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WIZ_ArmyUpkeep_Gold()\n", __FILE__, __LINE__);
#endif
    
    gold_upkeep_cost = 0;

    fame_points = WIZ_GetFame(player_idx);

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        gold_upkeep_cost += UNIT_GetGoldUpkeep(itr_units);
    }

    gold_upkeep_cost -= fame_points;

    if(gold_upkeep_cost < 0)
    {
        gold_upkeep_cost = 0;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: WIZ_ArmyUpkeep_Gold()\n", __FILE__, __LINE__);
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

// WZD o120p07
void Get_Incomes(int16_t player_idx, int16_t * gold, int16_t * food, int16_t * mana)
{
    int16_t Mana;
    int16_t City_Food_Surplus;
    int16_t City_Gold_Balance;
    int16_t Mana_Upkeep;
    int16_t Food_Upkeep;
    int16_t Gold_Upkeep;

    int16_t itr_cities;
    int16_t itr_heroes;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_Incomes()\n", __FILE__, __LINE__);
#endif

    Get_Power_Incomes(&Mana, &City_Food_Surplus, &City_Gold_Balance, player_idx);
// ; a wrapper for WIZ_GetPowerIncomes that also applies
// ; research bonuses based on the current research
// ; target, zeroes out any negative incomes, and zeroes
// ; out all incomes while casting the Spell of Return
// ;
// ; the passed parameter order is different for the two!


    Gold_Upkeep = 0;
    City_Gold_Balance = 0;
    Food_Upkeep = 0;
    City_Food_Surplus = 0;
    Mana_Upkeep = 0;


    if((g_TimeStop_PlayerNum - 1) == player_idx)
    {
        *food = 0;
        *gold = 0;
        *mana = -200;
    }
    else
    {
        Mana_Upkeep = WIZ_TotalUpkeep_Mana(player_idx);
            // ; calculates and returns the player's total mana
            // ; upkeep value, including units and their enchantments,
            // ; city enchantments, and global enchantments; halved
            // ; if they have the channeler retort

        Gold_Upkeep = WIZ_ArmyUpkeep_Gold(player_idx);
            // ; returns the gold upkeep sum of the player's armies, minus their total Fame

        Food_Upkeep = WIZ_ArmyUpkeep_Food(player_idx);
            // ; returns the amount of normal units under the player's control (which is their food upkeep)

        if(player_idx != HUMAN_PLAYER_IDX)
        {
            Mana_Upkeep = ((difficulty_modifiers_table[_difficulty].maintenance * Mana_Upkeep) / 100);
            Gold_Upkeep = ((difficulty_modifiers_table[_difficulty].maintenance * Gold_Upkeep) / 100);
            Food_Upkeep = ((difficulty_modifiers_table[_difficulty].maintenance * Food_Upkeep) / 100);
        }


        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        {
            if(_CITIES[itr_cities].owner_idx == player_idx)
            {
                City_Gold_Balance += (_CITIES[itr_cities].Gold - _CITIES[itr_cities].Upkeep);
                City_Food_Surplus += (_CITIES[itr_cities].Food - _CITIES[itr_cities].Pop_K);
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
                    City_Gold_Balance += 10;
                }
            }
        }



        *food = City_Food_Surplus - Food_Upkeep;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: City_Food_Surplus: %d\n", __FILE__, __LINE__, City_Food_Surplus);
    dbg_prn("DEBUG: [%s, %d]: Food_Upkeep: %d\n", __FILE__, __LINE__, Food_Upkeep);
    dbg_prn("DEBUG: [%s, %d]: *food: %d\n", __FILE__, __LINE__, *food);
#endif

        // mov bx, [bp+food]; mov ax, [bx]; cwd; sub ax, dx; sar ax, 1; add [bp+City_Gold_Balance], ax
        City_Gold_Balance += (*food > 0) ? 0 : *food / 2;
        *gold = City_Gold_Balance + Gold_Upkeep;

        *mana = Mana - Mana_Upkeep;

    }



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Get_Incomes()\n", __FILE__, __LINE__);
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


    for(itr_overland_enchantments = 0; itr_overland_enchantments < NUM_CITY_ENCHANTMENTS; itr_overland_enchantments++)
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

// WZD o120p12
// WZD o120p13
// WZD o120p14
// WZD o120p15
// WZD o120p16

// WZD o120p17
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
            City_Research += _CITIES[itr_cities].Research;
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
int16_t WIZ_GetFame(int16_t player_idx)
{
    int16_t hero_unit_idx;
    int16_t fame_points;
    int16_t itr_heroes;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WIZ_GetFame()\n", __FILE__, __LINE__);
#endif

    fame_points = 0;

    for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
    {
        if(_players[player_idx].Heroes[itr_heroes].Unit_Index > -1)
        {
            hero_unit_idx = _players[player_idx].Heroes[itr_heroes].Unit_Index;
            if( ((((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) ))->Abilities_LO) & 0x08) != 0 )  /* Ab_Legendary */
            {
                fame_points += ((_UNITS[hero_unit_idx].Level + 1) * 3);
            }
            if( ((((struct s_HERO *)( (p0_heroes + (player_idx * sizeof(p0_heroes))) + (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].type * sizeof(struct s_HERO)) ))->Abilities_LO) & 0x10) != 0 )  /* Ab_Legendary2 */
            {
                fame_points += (((_UNITS[hero_unit_idx].Level + 1) * 9) / 2);
            }
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
    dbg_prn("DEBUG: [%s, %d]: END: WIZ_GetFame()\n", __FILE__, __LINE__);
#endif

    return fame_points;
}

// WZD o120p21
// WZD o120p22
// WZD o120p23
// WZD o120p24
// WZD o120p25
// WZD o120p26
