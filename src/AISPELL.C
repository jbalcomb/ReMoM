/*
    AI Spell

    WIZARDS.EXE
    ovr156

MoO2
Module: AITECH
*/

#include "MoX/MOX_DAT.H"  /* _players[] */

#include "MoM.H"



/*
    WIZARDS.EXE ovr156
*/

// WZD o156p01
// drake178: AI_Research_Picker()
// ~ MoO2  Module: AITECH  AI_Tech_Select_()
/*
; selects the spell to research from the wizard's list
; of candidates using a weighted random roll,
; prioritizing combat spells in research groups from
; which the player does not yet have a known spell
;
; contains multipe BUGs that prevent research-related
; profile traits from properly affecting the outcome
*/
void AI_Research_Picker__STUB(int16_t player_idx)
{
    /* Dasm does itr 90, so this must be 90 bytes + 1 for 2-byte alignment */
    int16_t Possessed_Categories[91] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Unused_Var = 0;
    uint32_t Costs_32bit[NUM_RESEARCH_SPELLS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Candidate_Costs[NUM_RESEARCH_SPELLS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Highest_Cost = 0;
    int16_t Weight_Total = 0;
    int16_t Cnd_Category = 0;
    int16_t Skill = 0;
    int16_t Mana = 0;
    int16_t Research = 0;
    int16_t Loop_Var = 0;
    int16_t Research_Bonus = 0;
    int16_t Candidate_Count = 0;
    int16_t Original_Cost = 0;
    int16_t selection = 0;
    int16_t * ptr__players_spellranks = 0;
    uint16_t * ptr__players_research_spells = 0;
    int16_t itr_candidates = 0;  // _DI_

    ptr__players_research_spells = &_players[player_idx].research_spells[0];

    ptr__players_spellranks = &_players[player_idx].spellranks[0];

    Candidate_Count = 0;

    for(itr_candidates = 0; itr_candidates < NUM_RESEARCH_SPELLS; itr_candidates++)
    {

        if(ptr__players_research_spells[itr_candidates] != 0)
        {
            Candidate_Count++;
        }

    }

    if(Candidate_Count == 0)
    {

        _players[player_idx].researching_spell_idx = spl_NONE;

        _players[player_idx].research_ratio = 0;
        _players[player_idx].mana_ratio = 50;
        _players[player_idx].skill_ratio = 50;

        return;

    }


    // ...
    // ...
    // ...


    // ; chooses a random item from a list of 32bit weighted
    // ; chances; condensing the weights such that the total
    // ; fits into a single call of the 9-bit RNG (max 512),
    // ; using repeated divisions by 2 if necessary
    // TODO  Picked_Candidate = Get_Weighted_Choice_Long(&Costs_32bit[0], Candidate_Count);

    selection = Random(Candidate_Count);

    _players[player_idx].researching_spell_idx = ptr__players_research_spells[selection];

}


// WZD o156p02
// drake178: AI_OVL_SpellPicker()
/*
; selects the next overland spell to cast for the
; specified wizard
;
; multiple BUGs and unexplored sub-functions,
; RE-EXPLORE!
*/
/*



*/
void AI_Spell_Select__STUB(int16_t player_idx)
{
    int16_t Mana_Income = 0;
    int16_t Food_Income = 0;
    int16_t Gold_Income = 0;
    uint8_t * ptr_players_spells_known = 0;
    int16_t Mana_Per_Turn = 0;
    int16_t spellbook_page_spell_index = 0;
    int16_t Selected_Category = 0;
    int16_t spell_idx = 0;  // _DI_


    ptr_players_spells_known = &_players[player_idx].spells_list[0];


AI_Compute_Spells_Info(player_idx);

// TODO  Selected_Category = AI_OVL_SplCat_Picker(player_idx);
// ; selects a spell category for the AI to use next:
// ;   0 - no spell
// ;   1 - summon
// ;   2 - unit enchantment
// ;   3 - city enchantment
// ;   4 - disenchant / true
// ;   5 - disjunction / true
// ;   6 - summoning circle
// ;   7 - overland curse / damage
// ;   8 - suppression global
// ;   9 - global enchantment
// ;  10 - spell of mastery
// ;
// ; contains a number of BUGs - re-examine and fix later
Selected_Category = 0;

    switch(Selected_Category)
    {

        case 0:
        {

            spell_idx = 0;

        } break;

        case 1:
        {

        } break;

        case 2:
        {

        } break;

        case 3:
        {

        } break;

        case 4:
        {

        } break;

        case 5:
        {

        } break;

        case 6:
        {

        } break;

        case 7:
        {

        } break;

        case 8:
        {

        } break;

        case 9:
        {

        } break;

        case 10:
        {

        } break;

    }


    Player_Resource_Income_Total(player_idx, &Gold_Income, &Food_Income, &Mana_Income);

    Mana_Per_Turn = ((_players[player_idx].mana_reserve / 10) + Mana_Income);

    if((spell_data_table[spell_idx].Casting_Cost / 50) > Mana_Per_Turn)
    {

        spell_idx = 0;

    }


    /* TODO */ WIZ_SetOverlandSpell__WIP(player_idx, spell_idx, spellbook_page_spell_index);


}


// WZD o156p03
// AI_OVL_SplCat_Picker()

// WZD o156p04
// drake178: AI_OVL_GetSpellList()
/*
; recalculates the overland spells available to the
; selected player, setting AI_OVL_SpellCount,
; AI_OVL_Spell_Cats, and AI_OVL_SpellList accordingly
*/
/*



*/
void AI_Compute_Spells_Info(int16_t player_idx)
{
    int16_t spell_idx = 0;
    int16_t itr = 0;
    int16_t itr_spells = 0;  // _DI_
    int16_t itr_realms = 0;  // _SI_
    int16_t ai_spell_group = 0;  // _CX_

    CRP_AI_OVL_SpellCount = 0;  // ; redundant - calculated but never used


    for(itr = 0; itr < 90; itr++)
    {

        AI_OVL_Spell_Cats[itr] = 0;

    }

    for(itr_realms = 0; itr_realms < NUM_MAGIC_REALMS; itr_realms++)
    {

        for(itr_spells = 0; itr_spells < NUM_SPELLS_PER_MAGIC_REALM; itr_spells++)
        {

            if(_players[player_idx].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == 2 /* S_Known */)
            {

                spell_idx = ((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells);

                ai_spell_group = spell_data_table[spell_idx].AI_Group;

                if(ai_spell_group ==  5) { continue; }  // ; combat damage spells
                if(ai_spell_group == 24) { continue; }
                if(ai_spell_group == 47) { continue; }
                if(ai_spell_group == 70) { continue; }
                if(ai_spell_group == 12) { continue; }
                if(ai_spell_group == 31) { continue; }
                if(ai_spell_group == 55) { continue; }
                if(ai_spell_group == 77) { continue; }
                if(ai_spell_group == 16) { continue; }
                if(ai_spell_group == 35) { continue; }
                if(ai_spell_group == 59) { continue; }
                if(ai_spell_group == 81) { continue; }
                if(ai_spell_group == 17) { continue; }
                if(ai_spell_group == 36) { continue; }
                if(ai_spell_group == 60) { continue; }
                if(ai_spell_group == 82) { continue; }
                if(ai_spell_group ==  1) { continue; }  // ; Dispel Magic
                if(ai_spell_group == 20) { continue; }
                if(ai_spell_group == 43) { continue; }
                if(ai_spell_group == 19) { continue; }
                if(ai_spell_group ==  0) { continue; }  // ¿ NONE ?

                // ; redundant - filled out but never used
                // ; redundant - calculated but never used
                CRP_AI_OVL_SpellList[CRP_AI_OVL_SpellCount] = spell_idx;

                AI_OVL_Spell_Cats[ai_spell_group] = 1;

                CRP_AI_OVL_SpellCount++;

            }

        }

    }

}


// WZD o156p05
// AI_OVL_PickSummon()

// WZD o156p06
// AI_OVL_PickUnitBuff()

// WZD o156p07
// AI_OVL_PickRealmSupr()

// WZD o156p08
// AI_OVL_PickGlobal()

// WZD o156p09
// AI_OVL_PickCurse()

// WZD o156p10
// AI_OVL_PickCityBuff()

// WZD o156p11
// SPL_IsLifeSupressed()

// WZD o156p12
// SPL_IsChsSuppressed()

// WZD o156p13
// SPL_IsDthSuppressed()

// WZD o156p14
// CRP_SPL_IsNatSuppressed()

// WZD o156p15
// AI_OVL_PickDise()

// WZD o156p16
// AI_OVL_PickDisj()

// WZD o156p17
// AITP_WallofStone()

// WZD o156p18
// AITP_Transmute()

// WZD o156p19
// AITP_ChangeTerrain()

// WZD o156p20
// AITP_MoveFortress()

// WZD o156p21
// AITP_FlyingFortress()

// WZD o156p22
// AITP_EarthGate()

// WZD o156p23
// AITP_GaiasBlessing()

// WZD o156p24
// AITP_WallofFire()

// WZD o156p25
// AITP_WallofDarkness()

// WZD o156p26
// AITP_HeavenlyLight()

// WZD o156p27
// AITP_AltarofBattle()

// WZD o156p28
// AITP_StreamofLife()

// WZD o156p29
// AITP_Inspirations()

// WZD o156p30
// drake178: AITP_Prosperity()

// WZD o156p31
// drake178: AITP_AstralGate()

// WZD o156p32
// drake178: AITP_DarkRituals()

// WZD o156p33
// drake178: AITP_CloudofShadow()

// WZD o156p34
// drake178: AITP_SpellWard()

// WZD o156p35
// drake178: AITP_Consecration()

// WZD o156p36
// drake178: UU_DBG_GetKnownSpells()

// WZD o156p37
// drake178: CRP_DBG_SpellTargetError()

// WZD o156p38
// drake178: AI_ReleaseGlobals()
/*
releases unwanted global enchantments or, if at zero
mana, all global enchantments

WARNING: this may not be the best thing to do...
*/
/*

*/
void AI_Sanity_Check_Overland_Enchantments(int16_t player_idx)
{

    if(
        (_players[player_idx].Globals[ETERNAL_NIGHT] != 0)
        &&
        (_players[player_idx].Prim_Realm != sbr_Death)
        &&
        (_players[player_idx].Sec_Realm != sbr_Death)
    )
    {

        _players[player_idx].Globals[ETERNAL_NIGHT] = 0;

    }

    if(
        (_players[player_idx].Globals[EVIL_OMENS] != 0)
        &&
        (_players[player_idx].Prim_Realm == sbr_Life)
        &&
        (_players[player_idx].Sec_Realm == sbr_Life)
        &&
        (_players[player_idx].Prim_Realm == sbr_Nature)
        &&
        (_players[player_idx].Sec_Realm == sbr_Nature)
    )
    {

        _players[player_idx].Globals[EVIL_OMENS] = 0;

    }

    if(
        (_players[player_idx].Globals[CHAOS_SURGE] != 0)
        &&
        (_players[player_idx].Prim_Realm != sbr_Chaos)
        &&
        (_players[player_idx].Sec_Realm != sbr_Chaos)
    )
    {

        _players[player_idx].Globals[CHAOS_SURGE] = 0;

    }

    if(
        (_players[player_idx].Globals[TRANQUILITY] != 0)
        &&
        (_players[player_idx].Prim_Realm == sbr_Chaos)
        &&
        (_players[player_idx].Sec_Realm == sbr_Chaos)
    )
    {

        _players[player_idx].Globals[TRANQUILITY] = 0;

    }

    if(
        (_players[player_idx].Globals[LIFE_FORCE] != 0)
        &&
        (_players[player_idx].Prim_Realm == sbr_Chaos)
        &&
        (_players[player_idx].Sec_Realm == sbr_Chaos)
    )
    {

        _players[player_idx].Globals[LIFE_FORCE] = 0;

    }

    if(_players[player_idx].mana_reserve == 0)
    {

        _players[player_idx].Globals[ETERNAL_NIGHT] = 0;
        _players[player_idx].Globals[EVIL_OMENS] = 0;
        _players[player_idx].Globals[ZOMBIE_MASTERY] = 0;
        _players[player_idx].Globals[AURA_OF_MAJESTY] = 0;
        _players[player_idx].Globals[WIND_MASTERY] = 0;
        _players[player_idx].Globals[SUPPRESS_MAGIC] = 0;
        _players[player_idx].Globals[TIME_STOP] = 0;
        _players[player_idx].Globals[NATURES_AWARENESS] = 0;
        _players[player_idx].Globals[NATURES_WRATH] = 0;
        _players[player_idx].Globals[HERB_MASTERY] = 0;
        _players[player_idx].Globals[CHAOS_SURGE] = 0;
        _players[player_idx].Globals[DOOM_MASTERY] = 0;
        _players[player_idx].Globals[GREAT_WASTING] = 0;
        _players[player_idx].Globals[METEOR_STORM] = 0;
        _players[player_idx].Globals[ARMAGEDDON] = 0;
        _players[player_idx].Globals[TRANQUILITY] = 0;
        _players[player_idx].Globals[LIFE_FORCE] = 0;
        _players[player_idx].Globals[CRUSADE] = 0;
        _players[player_idx].Globals[JUST_CAUSE] = 0;
        _players[player_idx].Globals[HOLY_ARMS] = 0;
        _players[player_idx].Globals[PLANAR_SEAL] = 0;
        _players[player_idx].Globals[CHARM_OF_LIFE] = 0;
        _players[player_idx].Globals[DETECT_MAGIC] = 0;
        _players[player_idx].Globals[AWARENESS] = 0;
    }

}


// WZD o156p39
// drake178: sub_E7CC9()

// WZD o156p40
// drake178: AI_CanTargetCities()

// WZD o156p41
// drake178: sub_E8070()

// WZD o156p42
// drake178: sub_E8448()

// WZD o156p43
// drake178: sub_E882B()

// WZD o156p44
// drake178: sub_E88F3()

// WZD o156p45
// drake178: AITP_OVL_TileSpells()

// WZD o156p46
// AITP_OVL_HarmStack()

// WZD o156p47
// AITP_FloatingIsland()

// WZD o156p48
// AITP_EnchantRoad()

// WZD o156p49
// AITP_Disenchant()

// WZD o156p50
// AITP_HarmTerrain()

// WZD o156p51
// sub_E9FA9()

// WZD o156p52
// AITP_PlaneShift()

// WZD o156p53
// AITP_NaturesCures()

// WZD o156p54
// sub_EA43C()

// WZD o156p55
// sub_EA4FE()

// WZD o156p56
// AI_MyrrorAdvantage()

// WZD o156p57
// TILE_IsOcean()

// WZD o156p58
// sub_EA61E()

// WZD o156p59
// WIZ_SummonInFortress()
