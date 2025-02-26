/*
    AI - Data

    WIZARDS.EXE
        ¿ ovr145 ?
        ¿ ovr164 ?
*/

#include "MOX/MOX_DAT.H"  /* _players[] */

#include "MOM.H"



/*
    WIZARDS.EXE  ovr145
*/

// WZD o145p0





/*
    WIZARDS.EXE  ov1r64
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
// drake178: AI_SetNeutralTargets()
// AI_SetNeutralTargets()

// WZD o164p05
// drake178: AI_Turn_NearAllocs()
// ¿ MoO2  Module: AIDATA  Compute_AI_Data_() ? ¿ ... Deallocate_AI_Data_() ?
/*
; allocates a total of 3,942 bytes of the LBX near
; buffer to various AI arrays
;
; RE-EXPLORE and MOVE
*/
/*

*/
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

    // AI_Arc_Cont_Own_Str = (int16_t *)Near_Allocate_Next(120);
    // AI_Myr_Cont_Own_Str = (int16_t *)Near_Allocate_Next(120);
    AI_Cont_Own_Str[0] = (int16_t *)Near_Allocate_Next(120);
    AI_Cont_Own_Str[1] = (int16_t *)Near_Allocate_Next(120);

    // CRP_AI_Arc_Cont_Nme_Str = Near_Allocate_Next(120);
    // CRP_AI_Myr_Cont_Nme_Str = Near_Allocate_Next(120);
    CRP_AI_Cont_Nme_Str[0] = (int16_t *)Near_Allocate_Next(120);
    CRP_AI_Cont_Nme_Str[1] = (int16_t *)Near_Allocate_Next(120);

    // AI_Arc_Cont_Own_Val = Near_Allocate_Next(120);
    // AI_Myr_Cont_Own_Val = Near_Allocate_Next(120);
    AI_Cont_Own_Val[0] = (int16_t *)Near_Allocate_Next(120);
    AI_Cont_Own_Val[1] = (int16_t *)Near_Allocate_Next(120);

    // AI_Arc_Cont_Nme_Val = Near_Allocate_Next(120);
    // AI_Myr_Cont_Nme_Val = Near_Allocate_Next(120);
    AI_Cont_Nme_Val[0] = (int16_t *)Near_Allocate_Next(120);
    AI_Cont_Nme_Val[1] = (int16_t *)Near_Allocate_Next(120);

    // AI_Arc_ContBalances = CRP_AI_Arc_Cont_Nme_Str;
    // AI_Myr_ContBalances = CRP_AI_Myr_Cont_Nme_Str;
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

            if(spell_data_table[_players[player_idx].casting_spell_idx].Casting_Cost >= spell_data_table[spl_Cruel_Unminding].Casting_Cost)
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
// drake178: AI_GetNeutralStacks()
// AI_GetNeutralStacks()
