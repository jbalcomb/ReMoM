/*
    AI Spell

    WIZARDS.EXE
    ovr156

MoO2
Module: AITECH
*/

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
// AI_OVL_SpellPicker()

// WZD o156p03
// AI_OVL_SplCat_Picker()

// WZD o156p04
// AI_OVL_GetSpellList()

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
// AITP_Prosperity()

// WZD o156p31
// AITP_AstralGate()

// WZD o156p32
// AITP_DarkRituals()

// WZD o156p33
// AITP_CloudofShadow()

// WZD o156p34
// AITP_SpellWard()

// WZD o156p35
// AITP_Consecration()

// WZD o156p36
// UU_DBG_GetKnownSpells()

// WZD o156p37
// CRP_DBG_SpellTargetError()

// WZD o156p38
// AI_ReleaseGlobals()

// WZD o156p39
// sub_E7CC9()

// WZD o156p40
// AI_CanTargetCities()

// WZD o156p41
// sub_E8070()

// WZD o156p42
// sub_E8448()

// WZD o156p43
// sub_E882B()

// WZD o156p44
// sub_E88F3()

// WZD o156p45
// AITP_OVL_TileSpells()

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
