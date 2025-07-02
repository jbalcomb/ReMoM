/*
    WIZARDS.EXE
        ovr128
*/

#include "CITYCALC.H"
#include "MOM_DEF.H"
#include "MOX/Util.H"
#include "Spells128.H"

#include "MOX/Allocate.H"
#include "MOX/MOM_Data.H"
#include "MOX/MOX_DAT.H"
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"
#include "MOX/random.H"

#include "AISPELL.H"
#include "DIPLOMAC.H"
#include "Spellbook.H"



/*
    WIZARDS.EXE  ovr128
*/

// WZD o128p01
// drake178: WIZ_RefreshResearch()
/*
; fills any empty slots in the research candidate list
; if possible, and ensures that if the wizard has any
; candidates available, they are researching something
; by calling AI_Research_Picker or GUI_Research_Dialog
; if that is not the case
; returns the player's research candidate count
*/
/*

XREF:
    WIZ_AddSpellRank__WIP()
    j_Player_Research_Spells()
        Player_Research_Spells()
        PreInit_Overland()
        WIZ_LearnSpell__WIP()
*/
int16_t Player_Research_Spells(int16_t player_idx)
{
    int16_t * research_list = 0;
    int16_t skip_som = 0;
    int16_t spell_realm_idx = 0;
    int16_t spell_realm = 0;
    int16_t spells_cnt = 0;
    int16_t research_list_idx = 0;
    int16_t spells_max = 0;
    int16_t itr = 0; // _SI_


    spells_max = NUM_RESEARCH_SPELLS;

    m_spell_list_count = 0;

    spells_cnt = 0;

    skip_som = ST_FALSE;

    research_list = (int16_t *)Near_Allocate_First(200);


    Build_Research_List(player_idx, &research_list[0]);


    if(_players[player_idx].spells_list[(spl_Spell_Of_Mastery - 1)] == sls_Known)
    {
        skip_som = ST_TRUE;
    }
    else
    {
        for(itr = 0; itr < spells_max; itr++)
        {
            if(_players[player_idx].research_spells[itr] > spl_NONE)
            {
                spells_cnt++;
                if(_players[player_idx].research_spells[itr] == spl_Spell_Of_Mastery)
                {
                    skip_som = ST_TRUE;
                }
            }
        }
    }

    while((spells_cnt < spells_max) && (m_spell_list_count > 0))
    {
        research_list_idx = (Random(m_spell_list_count) - 1);

        spells_cnt++;

        for(itr = 0; itr < spells_max; itr++)
        {
            if(_players[player_idx].research_spells[itr] == spl_NONE)
            {
                _players[player_idx].research_spells[itr] = research_list[research_list_idx];
                spell_realm_idx = ((research_list[research_list_idx] - 1) % NUM_SPELLS_PER_MAGIC_REALM);
                spell_realm = ((research_list[research_list_idx] - 1) / NUM_SPELLS_PER_MAGIC_REALM);
                _players[player_idx].spells_list[((spell_realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_realm_idx)] = sls_Researchable;
                Build_Research_List(player_idx, &research_list[0]);
            }
        }
    }


    spells_cnt = 0;

    for(itr = 0; itr < spells_max; itr++)
    {
        if(_players[player_idx].research_spells[itr] > spl_NONE)
        {
            spells_cnt++;
        }
    }


    if(
        (spells_cnt < spells_max)
        &&
        (m_spell_list_count == 0)
        &&
        (skip_som == ST_FALSE)
    )
    {
        if(_players[player_idx].research_spells[itr] == spl_NONE)
        {
            _players[player_idx].research_spells[itr] = spl_Spell_Of_Mastery;
        }
    }


    Sort_Research_List(player_idx, spells_cnt);


    if(
        (_players[player_idx].researching_spell_idx == spl_NONE)
        &&
        (spells_cnt > 0)
        &&
        (_turn > 1)
    )
    {
        if(player_idx == HUMAN_PLAYER_IDX)
        {

            Spell_Research_Select();

        }
        else
        {

            AI_Research_Picker__STUB(player_idx);

        }

    }

    return spells_cnt;
    
}


// WZD o128p02
// drake178: WIZ_GetResearchList()
/*
; compiles a list of the lowest rarity spells from each realm available to the specified player
*/
/*

*/
void Build_Research_List(int16_t player_idx, int16_t research_list[])
{
    int16_t rarity;
    int16_t itr_realms;  // _SI_
    int16_t flag;  // _DI_
    int16_t itr_spells;  // _CX_

    m_spell_list_count = 0;

    // itr 5 realms
    for(itr_realms = 0; itr_realms <= 4; itr_realms++)
    {

        flag = ST_FALSE;

        // itr 40 spells per realm
        for(itr_spells = 0; itr_spells < NUM_SPELLS_PER_MAGIC_REALM; itr_spells++)
        {

            if(flag == ST_FALSE)
            {
                rarity = (itr_spells / 10);  // updates until the first 'knowable'
            }

            if((itr_spells / 10) <= rarity)
            {

                if(_players[player_idx].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == sls_Knowable)
                {

                    research_list[m_spell_list_count] = ((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + (itr_spells + 1));

                    m_spell_list_count++;

                    flag = ST_TRUE;

                }

            }

        }

    }

    flag = ST_FALSE;

    for(itr_spells = 0; itr_spells < 12; itr_spells++)
    {

        if(
            (flag != ST_TRUE)
            ||
            (
                (itr_spells != 3)
                &&
                (itr_spells != 8)
            )
        )
        {

            if(_players[player_idx].spells_list[((sbr_Arcane * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == sls_Knowable)
            {

                research_list[(m_spell_list_count)] = ((sbr_Arcane * NUM_SPELLS_PER_MAGIC_REALM) + (itr_spells + 1));

                m_spell_list_count++;

                flag = ST_TRUE;

            }
            
        }

    }

}


// WZD o128p03
int16_t WIZ_RollSpellReward(int16_t player_idx, int16_t rarity)
{
    int16_t Invalid_Realms[NUM_MAGIC_TYPES] = { 0, 0, 0, 0, 0 };
    int16_t Max_Rarity = 0;
    int16_t Spell_Found = 0;
    int16_t Random_Result = 0;
    int16_t Valid_Realm_Count = 0;
    int16_t In_Realm_Index = 0;
    int16_t Added_Rarity = 0;
    int16_t itr;  // _SI_
    int16_t magic_realm = 0;  // _DI_

    Valid_Realm_Count = 0;
    Spell_Found = 0;

    // ¿ BUG: the arcane realm is evaluated by the presence of the Alchemy or Warlord retorts instead of being marked as always available ?
    // DNM, because no item powers have mt_Arcane? which wouldn't make sense anyway?
    for(itr = 0; itr < NUM_MAGIC_TYPES; itr++)
    {
        // ¿ ; conflicting condition - will never jump ?
        if(
            (itr == NUM_MAGIC_TYPES)
            ||
            (_players[player_idx].spellranks[itr] == 0)
        )
        {
            Valid_Realm_Count++;
            Invalid_Realms[itr] = ST_FALSE;
        }
        else
        {
            Invalid_Realms[itr] = ST_TRUE;
        }
    }

    while((Valid_Realm_Count > 0) && (Spell_Found == ST_FALSE))
    {
        Random_Result = Random(Valid_Realm_Count);  // {0,1,2,3,4,5,6}

        itr = 0;
        magic_realm = 0;
        while(Random_Result != itr)
        {
            if(Invalid_Realms[magic_realm] == ST_FALSE)
            {
                itr++;
            }
            magic_realm++;
        }

        magic_realm--;

        if(magic_realm == 5)  /* _Arcane ... enum Realm_Byte*/
        {
            Max_Rarity = 3;
        }
        else if(_players[player_idx].spellranks[magic_realm] == 1)
        {
            Max_Rarity = 2;
        }
        else if(_players[player_idx].spellranks[magic_realm] == 2)
        {
            Max_Rarity = 3;
        }
        else
        {
            Max_Rarity = 4;
        }

        for(Added_Rarity = 0; ((Added_Rarity < 4) && (Spell_Found == ST_FALSE)); Added_Rarity++)
        {
            Random_Result = (Random(10) - 1);

            for(itr = 0; itr < 10; itr++)
            {
                if(Spell_Found == ST_FALSE)
                {
                    if(magic_realm == 5)  /* _Arcane */
                    {
                        In_Realm_Index = (Random(11) - 1);
                    }
                    else
                    {
                        // TODO(JimBalcomb,20240531): no way this is right - run in through the Struggle-Mode debugger
                        In_Realm_Index = (((Random_Result + itr) % 10) + ((((rarity + Added_Rarity) - 1) / Max_Rarity) * 10));
                    }

                    if(_players[player_idx].spells_list[((magic_realm * 40) + In_Realm_Index)] == 2)  /* S_Known ... enum Research_Status */
                    {
                        Spell_Found = ST_TRUE;
                        In_Realm_Index = (((magic_realm * 40) + In_Realm_Index) + 1);
                    }

                }
            }

        }

        Valid_Realm_Count--;
    }

    if(Spell_Found == ST_FALSE)
    {
        In_Realm_Index = 0;
    }

    // @@Done:
    return In_Realm_Index;
}

// WZD o128p04
// MoO2  Module: NEWTECH  Calc_Tech_Value_() |-> Get_Competition_Player_Tech_Value_()
/*
; returns -1 if the spell can't be learned based on the
; wizard's spellbooks; 0 if it's already known; its
; research cost if its knowable; 90% of that if it's
; already researchable; or 150% of it is not knowable,
; but learnable
*/
/*
player_idx is listen_player

*/
int16_t Calc_Spell_Value(int16_t player_idx, int16_t spell_idx)
{
    int16_t spell_realm_idx = 0;
    int16_t spell_value = 0;  // _SI_
    int16_t magic_realm = 0;  // _CX_

    spell_value = spell_data_table[spell_idx].research_cost;

    magic_realm = spell_data_table[spell_idx].magic_realm;

    spell_realm_idx = ((spell_idx - 1) / NUM_SPELLS_PER_MAGIC_REALM);  // ¿ thinks it's getting spell_num, rather than spell_idx ?

    if(
        (
            (magic_realm != sbr_Arcane)
            &&
            (_players[player_idx].spellranks[magic_realm] == 0)
        )
        ||
        (
            (magic_realm != sbr_Arcane)
            &&
            (_players[player_idx].spellranks[magic_realm] == 1)
            &&
            (spell_realm_idx >= (2 * NUM_SPELLS_PER_MAGIC_RARITY))
        )
        ||
        (
            (magic_realm != sbr_Arcane)
            &&
            (_players[player_idx].spellranks[magic_realm] == 2)
            &&
            (spell_realm_idx >= (3 * NUM_SPELLS_PER_MAGIC_RARITY))
        )
    )
    {

        spell_value = ST_UNDEFINED;

    }
    else
    {

        if(_players[player_idx].spells_list[((magic_realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_realm_idx)] == sls_Known)
        {

            spell_value = 0;

        }
        else
        {

            if(_players[player_idx].spells_list[((magic_realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_realm_idx)] == sls_Researchable)
            {

                spell_value -= (spell_value / 10);  // - 10%

            }

            if(_players[player_idx].spells_list[((magic_realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_realm_idx)] == sls_Unknown)
            {

                spell_value += (spell_value / 2);  // + 50%

            }

        }

    }

    return spell_value;

}


// WZD o128p05
// MoO2  Module: NPCDIPLO  Get_Exchange_Tech_List_();  Get_Differential_Tech_List_();
int16_t Get_Differential_Spell_List(int16_t player1, int16_t player2, int16_t min_value, int16_t spell_list[])
{
    /* ¿ BUGBUG  6 * 10 ? */
    uint8_t candidate_list[60] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t candidate_idx = 0;
    int16_t spell_realm_idx = 0;
    int16_t spell_idx = 0;
    int16_t spell_count = 0;
    int16_t spell_value = 0;
    int16_t itr = 0;
    int16_t magic_realm = 0;  // _SI_
    int16_t candidate_count = 0;  // _DI_

    for(itr = 0; itr < 10; itr++)
    {

        spell_list[itr] = 0;

    }

// create a list of spells that are in a realm that both
// wizards have at least one book of, and are known by
// the second wizard but not the first
// 
// BUG: if there are more than 60 possible spells,
// wrong ones may be awarded, and if there are more than
// 74, the game will crash if the 74th has an index of
// 128 or more

    candidate_count = 0;

    for(itr = spl_Earth_To_Mud; itr < spl_Spell_Of_Return; itr++)
    {

        magic_realm = spell_data_table[itr].magic_realm;

        spell_realm_idx = ((itr - 1) / NUM_SPELLS_PER_MAGIC_REALM);  // ¿ thinks it's getting spell_num, rather than spell_idx ?

        if(
            (
                (magic_realm != sbr_Arcane)
                &&
                (_players[player1].spellranks[magic_realm] == 0)
            )
        )
        {

            itr += 39;

        }
        else
        {

            if(
                (_players[player2].spells_list[((magic_realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_realm_idx)] == sls_Known)
                &&
                (_players[player1].spells_list[((magic_realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_realm_idx)] != sls_Known)
            )
            {

                candidate_list[candidate_count] = itr;

                candidate_count++;

            }

        }

    }

    spell_count = 0;

    if(candidate_count > 0)
    {

        for(itr = 0; ((itr < 100) && (spell_count < 10)); itr++)
        {

            candidate_idx = (Random(candidate_count) - 1);

            spell_idx = candidate_idx[candidate_list];

            candidate_list[candidate_idx] = 0;
            
            if(spell_idx != 0)
            {

                spell_value = Calc_Spell_Value(player1, spell_idx);

                if(spell_value >= min_value)
                {

                    // BUGBUG  dead code  ...or, should get spell_num, not use spell_idx?
                    magic_realm = spell_data_table[spell_idx].magic_realm;

                    spell_realm_idx = ((spell_idx - 1) / NUM_SPELLS_PER_MAGIC_REALM);  // ¿ thinks it's getting spell_num, rather than spell_idx ?

                    spell_list[spell_count] = spell_idx;

                    spell_count++;

                }

            }

        }

    }




    return spell_count;

}

// WZD o128p06
// drake178: sub_AC19E()
/*

*/
void Change_Relations_For_Bad_City_Spell(int16_t player_idx, int16_t spell_idx, int16_t city_idx)
{
    int16_t sw_spell_idx = 0;

    sw_spell_idx = spell_idx;

    switch(sw_spell_idx)
    {

        case spl_Earthquake:  // 0x1A  26
        {
            Change_Relations__WIP(-9, player_idx, _CITIES[city_idx].owner_idx, 13, city_idx, spell_idx);
        } break;

        case spl_Corruption:  // 0x55  85
        {
            Change_Relations__WIP(-25, player_idx, _CITIES[city_idx].owner_idx, 13, city_idx, spell_idx);
        } break;

        case spl_Raise_Volcano:  // 0x62  98
        {
            Change_Relations__WIP(-50, player_idx, _CITIES[city_idx].owner_idx, 13, city_idx, spell_idx);

        } break;

        case spl_Chaos_Rift:  // 0x6E  110
        {
            Change_Relations__WIP(-5, player_idx, _CITIES[city_idx].owner_idx, 13, city_idx, spell_idx);

        } break;

        case spl_Call_The_Void:  // 0x77  119
        {
            Change_Relations__WIP(-20, player_idx, _CITIES[city_idx].owner_idx, 13, city_idx, spell_idx);

        } break;

        case spl_Evil_Presence:  // 0xB7  183
        {
            Change_Relations__WIP(-5, player_idx, _CITIES[city_idx].owner_idx, 13, city_idx, spell_idx);

        } break;

        case spl_Famine:  // 0xBD  189
        {
            Change_Relations__WIP(-5, player_idx, _CITIES[city_idx].owner_idx, 13, city_idx, spell_idx);

        } break;

        case spl_Cursed_Lands:  // 0xBE  190
        {
            Change_Relations__WIP(-5, player_idx, _CITIES[city_idx].owner_idx, 13, city_idx, spell_idx);

        } break;

        case spl_Pestilence:  // 0xC4  196
        {
            Change_Relations__WIP(-8, player_idx, _CITIES[city_idx].owner_idx, 13, city_idx, spell_idx);
            
        } break;

    }

}

// WZD o128p07
// drake178: WIZ_GE_Diplomacy()
/*
; initiates a diplomatic reaction towards the specified
; player for the selected global spell from every other
; wizard
;
; WARNING: the per-turn divisors make the action values
; very small or even zero
*/
/*
spl_Spell_Of_Mastery    - 50  10
spl_Eternal_Night       - 12  12
spl_Evil_Omens          - 20  12
spl_Zombie_Mastery      - 14  12
spl_Aura_Of_Majesty     + 10   0
spl_Wind_Mastery        -  4  12
spl_Suppress_Magic      - 25  12
spl_Time_Stop              0  12
spl_Natures_Awareness       0  12
spl_Natures_Wrath       - 20  12
spl_Herb_Mastery           0  12
spl_Chaos_Surge         - 10  12
spl_Doom_Mastery        -  8  12
spl_Great_Wasting       - 20  12
spl_Meteor_Storm        - 15  12
spl_Armageddon          - 25  12
spl_Tranquility         - 20  12
spl_Life_Force          - 20  12
spl_Crusade             - 10  12
spl_Just_Cause             0  12
spl_Holy_Arms           -  5  12

*/
/*
    updates diplomatic relations for all other wizards, with the caster of the overland enchantment, based on which spell is being cast

XREF:
    Cast_Spell_Overland_Do()
    Cast_Spell_Overland__WIP()
    WIZ_ProcessGlobals()


*/
void Change_Relations_For_Enchantments(int16_t player_idx, int16_t spell_idx, int16_t divisor)
{
    int16_t itr_players = 0;  // _DI_

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(itr_players != player_idx)
        {

            if(spell_idx == spl_Spell_Of_Mastery)
            {
                Change_Relations__WIP((-50 / divisor), player_idx, itr_players, 10, 0, spell_idx);
            }

            if(spell_idx == spl_Eternal_Night)
            {
                Change_Relations__WIP((-12 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(
                (spell_idx == spl_Evil_Omens)
                &&
                (
                    (_players[itr_players].spellranks[sbr_Life] > 0)
                    ||
                    (_players[itr_players].spellranks[sbr_Nature] > 0)
                )
            )
            {
                Change_Relations__WIP((-20 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Zombie_Mastery)
            {
                Change_Relations__WIP((-14 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Aura_Of_Majesty)
            {
                Change_Relations__WIP((10 / divisor), player_idx, itr_players, 0, 0, spell_idx);
            }

            if(spell_idx == spl_Wind_Mastery)
            {
                Change_Relations__WIP((-4 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Suppress_Magic)
            {
                Change_Relations__WIP((-4 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Time_Stop)
            {
                Change_Relations__WIP((0 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Natures_Awareness)
            {
                Change_Relations__WIP((0 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(
                (spell_idx == spl_Natures_Wrath)
                &&
                (
                    (_players[itr_players].spellranks[sbr_Chaos] > 0)
                    ||
                    (_players[itr_players].spellranks[sbr_Death] > 0)
                )
            )
            {
                Change_Relations__WIP((-20 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Herb_Mastery)
            {
                Change_Relations__WIP((0 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(
                (spell_idx == spl_Chaos_Surge)
                &&
                (_players[itr_players].spellranks[sbr_Chaos] == 0)
            )
            {
                Change_Relations__WIP((-10 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Doom_Mastery)
            {
                Change_Relations__WIP((-8 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Great_Wasting)
            {
                Change_Relations__WIP((-20 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Meteor_Storms)
            {
                Change_Relations__WIP((-15 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Armageddon)
            {
                Change_Relations__WIP((-25 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(
                (spell_idx == spl_Tranquility)
                &&
                (_players[itr_players].spellranks[sbr_Chaos] > 0)
            )
            {
                Change_Relations__WIP((-20 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(
                (spell_idx == spl_Life_Force)
                &&
                (_players[itr_players].spellranks[sbr_Death] > 0)
            )
            {
                Change_Relations__WIP((-20 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Crusade)
            {
                Change_Relations__WIP((-10 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Just_Cause)
            {
                Change_Relations__WIP((0 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Holy_Arms)
            {
                Change_Relations__WIP((-5 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

        }

    }

}


// WZD o128p08
// drake178: WIZ_ResearchSort()
/*
; sorts the wizard's research candidates in ascending
; order by estimated research time or, if the research
; income is 0, by research cost
; contains a BUG that results in some retorts not
; being applied properly, especially Conjurer
*/
/*
OON XREF:  Player_Research_Spells()
*/
void Sort_Research_List(int16_t player_idx, int16_t count)
{
    int16_t realm_research_incomes[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t cost2 = 0;
    int16_t cost1 = 0;
    int16_t research_idx2 = 0;
    int16_t spell_idx2 = 0;
    int16_t research_points = 0;
    int16_t research_bonus_percentage = 0;
    int16_t itr = 0;
    int16_t research_idx1 = 0;  // _SI_
    int16_t spell_idx1 = 0;  // DNE in Dasm

    Players_Update_Magic_Power();

    Player_Magic_Power_Distribution(&research_bonus_percentage, &research_bonus_percentage, &research_points, player_idx);

    // ~== SBK_BuildSpellbook__WIP()
    for(itr = 0; itr < NUM_MAGIC_REALMS; itr++)
    {

        // ; BUG: research bonus calculated using an arbitrary
        // ; spell of the realm, which won't always include all
        // ; relevant bonuses, and may also add wrong ones
        research_bonus_percentage = Player_Spell_Research_Bonus(HUMAN_PLAYER_IDX, ((itr * NUM_SPELLS_PER_MAGIC_REALM) + 1));
        realm_research_incomes[itr] = ((research_points * research_bonus_percentage) / 100);
    }

    for(research_idx2 = 1; research_idx2 < count; research_idx2++)
    {

        spell_idx2 = _players[player_idx].research_spells[research_idx2];

        research_idx1 = (research_idx2 - 1);

        spell_idx1 = _players[player_idx].research_spells[research_idx1];  // DNE in Dasm

        if(realm_research_incomes[spell_data_table[spell_idx1].magic_realm] == 0)
        {
    
            cost1 = spell_data_table[spell_idx1].research_cost;

        }
        else
        {

            cost1 = (spell_data_table[spell_idx1].research_cost / realm_research_incomes[spell_data_table[spell_idx1].magic_realm]);

        }

        if(realm_research_incomes[spell_data_table[spell_idx2].magic_realm] == 0)
        {

            cost2 = spell_data_table[spell_idx2].research_cost;

        }
        else
        {

            cost2 = (spell_data_table[spell_idx2].research_cost / realm_research_incomes[spell_idx2]);

        }

        while((research_idx1 > -1) && (cost1 > cost2))
        {

            _players[player_idx].research_spells[(research_idx1 + 1)] = _players[player_idx].research_spells[research_idx1];

            spell_idx1 = _players[player_idx].research_spells[research_idx1];  // DNE in Dasm

            research_idx1--;

            if(research_idx1 > -1)
            {

                if(realm_research_incomes[spell_data_table[spell_idx1].magic_realm] == 0)
                {
            
                    cost1 = spell_data_table[spell_idx1].research_cost;

                }
                else
                {

                    cost1 = (spell_data_table[spell_idx1].research_cost / realm_research_incomes[spell_data_table[spell_idx1].magic_realm]);

                }

            }

        }

        _players[player_idx].research_spells[(research_idx1 + 1)] = spell_idx2;

    }

}

// WZD o128p09
// drake178: WIZ_AddSpellbook()
/*
awards the selected wizard a spellbook of the
specified realm, adding knowable spells as necessary,
and ensuring that they have research candidates if
possible

BUG: contains an outdated rarity per book table
BUG: if called with 11 books and missing very rare
 spells, the book the book is converted to a
 different realm instead
BUG: if called with 12 books and missing spells,
 memory corruption occurs and the effect is undefined

00000000 struc PerBook_Spells ; (sizeof=0x8, standard type)
00000000 Common dw ?
00000002 Uncommon dw ?
00000004 Rare dw ?
00000006 Very_Rare dw ?
00000008 ends PerBook_Spells

*/
void WIZ_AddSpellRank__WIP(int16_t player_idx, int16_t magic_realm)
{
    int16_t realm_spells[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Book_Spell_Table[10][4] = {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    };
    int16_t random_realm_spell = 0;
    int16_t Existing_Books = 0;
    int16_t In_Rarity_Index = 0;
    int16_t realm_spell_count = 0;  // _SI_
    int16_t itr = 0;  // _DI_

    Book_Spell_Table[0][0] = 3;
    Book_Spell_Table[0][1] = 1;
    Book_Spell_Table[0][2] = 0;
    Book_Spell_Table[0][3] = 0;
    Book_Spell_Table[1][0] = 2;
    Book_Spell_Table[1][1] = 1;
    Book_Spell_Table[1][2] = 1;
    Book_Spell_Table[1][3] = 1;  // ¿ ; BUG: should be 0 ?
    Book_Spell_Table[2][0] = 1;
    Book_Spell_Table[2][1] = 1;
    Book_Spell_Table[2][2] = 1;
    Book_Spell_Table[2][3] = 1;
    Book_Spell_Table[3][0] = 1;
    Book_Spell_Table[3][1] = 1;
    Book_Spell_Table[3][2] = 1;
    Book_Spell_Table[3][3] = 1;
    Book_Spell_Table[4][0] = 1;
    Book_Spell_Table[4][1] = 1;
    Book_Spell_Table[4][2] = 1;
    Book_Spell_Table[4][3] = 1;
    Book_Spell_Table[5][0] = 1;
    Book_Spell_Table[5][1] = 1;
    Book_Spell_Table[5][2] = 1;
    Book_Spell_Table[5][3] = 1;
    Book_Spell_Table[6][0] = 1;
    Book_Spell_Table[6][1] = 2;
    Book_Spell_Table[6][2] = 1;
    Book_Spell_Table[6][3] = 1;
    Book_Spell_Table[7][0] = 0;
    Book_Spell_Table[7][1] = 2;
    Book_Spell_Table[7][2] = 1;
    Book_Spell_Table[7][3] = 1;
    Book_Spell_Table[8][0] = 0;
    Book_Spell_Table[8][1] = 0;
    Book_Spell_Table[8][2] = 2;
    Book_Spell_Table[8][3] = 1;
    Book_Spell_Table[9][0] = 0;
    Book_Spell_Table[9][1] = 0;
    Book_Spell_Table[9][2] = 2;  // ¿ ; BUG: should be 1 ?
    Book_Spell_Table[9][3] = 1;  // ¿ ; BUG: should be 3 ?

    Existing_Books = _players[player_idx].spellranks[magic_realm];

    // ¿ ; BUG: values greater than 10 corrupt memory ?
    if(Existing_Books != 10)
    {
        for(itr = 0; itr < 4; itr++)
        {
            realm_spell_count = 0;
            
            for(In_Rarity_Index = 0; In_Rarity_Index < 10; In_Rarity_Index++)
            {
                // DEDU  sizeof() 40 and 10 for spells magic realm and rarity
                if(_players[player_idx].spells_list[((magic_realm * 40) + (itr * 10) + In_Rarity_Index)] == 0)  /* S_Unknown */
                {
                    realm_spells[realm_spell_count] = ((itr * 10) + In_Rarity_Index);
                    realm_spell_count++;
                }
            }

            while(
                (realm_spell_count > 0)
                &&
                (Book_Spell_Table[Existing_Books][itr] > 0)
            )
            {
                random_realm_spell = (Random(realm_spell_count) - 1);

                _players[player_idx].spells_list[((magic_realm * 40) + realm_spells[random_realm_spell])] = 1;  /* S_Knowable */

                Clear_Structure(random_realm_spell, (uint8_t *)&realm_spells[0], sizeof(realm_spells[0]), realm_spell_count);

                realm_spell_count--;

                Book_Spell_Table[Existing_Books][itr]--;
            }
        }
    }

    _players[player_idx].spellranks[magic_realm]++;

    Player_Research_Spells(player_idx);

}


// WZD o128p10
// sub_ACACC()

// WZD o128p11
// WIZ_ConquestSpells()
