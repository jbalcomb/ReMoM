/*
    AI Spell

    WIZARDS.EXE
    ovr156

MoO2
Module: AITECH
*/

#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/EXIT.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/Util.h"
#include "../../MoX/src/capture.h"
#include "../../MoX/src/random.h"

#include "AIDUDES.h"
#include "CITYCALC.h"
#include "SBookScr.h"
#include "Spellbook.h"
#include "SPELLDEF.h"
#include "TerrType.h"

#include "../../ext/stu_compat.h"

#include <assert.h>
#include <string.h>

#include "AISPELL.h"


// WZD dseg:70F8                                                 BEGIN:  ovr156 - Initialized Data

// WZD dseg:70F8
char CRP_AI_SpellTargetError[] = " could not be found for CP.";

// WZD dseg:7114 00                                              db    0
// WZD dseg:7115 00                                              db    0

// WZD dseg:7115                                                 END:  ovr156 - Initialized Data



/*
    WIZARDS.EXE ovr156
*/

// WZD o156p01
// ~ MoO2  Module: AITECH  AI_Tech_Select_()
/* GEMINI */
int16_t AI_Spell_Research_Select(int16_t player_idx)
{
    int16_t possessed_categories[92] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int32_t weights_long[NUM_RESEARCH_SPELLS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t candidate_costs[NUM_RESEARCH_SPELLS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int32_t highest_cost = 0;
    int32_t weights_long_total = 0;
    int16_t cnd_category = 0;
    int16_t skill_income = 0;
    int16_t mana_income = 0;
    int16_t research_income = 0;
    int16_t itr2 = 0;
    int16_t research_bonus = 0;
    int16_t candidate_count = 0;
    int16_t original_cost = 0;
    int16_t selection = 0;
    int16_t * spellranks = 0;
    uint16_t * research_spells = 0;
    int16_t itr = 0;
    unsigned int spell_idx = 0;
    unsigned int base_cost = 0;
    int16_t known_spell_idx = 0;
    int32_t w = 0;

    research_spells = _players[player_idx].research_spells;

    spellranks = &_players[player_idx].spellranks[0];

    candidate_count = 0;

    for (itr = 0; itr < 8; itr++) {
        if (research_spells[itr] != 0) {
            candidate_count++;
        }
    }

    if (candidate_count == 0) {
        dbg_prn("[AI_RESEARCH] player[%d] candidate_count=0, no spells to research\n", player_idx);
        _players[player_idx].researching_spell_idx = 0;
        _players[player_idx].research_ratio = 0;
        _players[player_idx].mana_ratio = 50;
        _players[player_idx].skill_ratio = 50;
        return 0;
    }

    /* BUG #1: Setting this to index 1 (Earth to Mud) just to satisfy research trait logic */
    _players[player_idx].researching_spell_idx = 1; 
    _players[player_idx].reevaluate_magic_power_countdown = 0;

    AI_Update_Magic_Power(player_idx);
    Player_Magic_Power_Income_Total(&mana_income, &research_income, &skill_income, player_idx);

    /* Calculate adjusted research costs based on traits/bonuses */
    /* OGBUG: #3 research-related traits are now applied twice (if it wasn't for BUGs #1 & #2) */
    /* Gemini rearranged the logic here quite a bit */
    for (itr = 0; itr < candidate_count; itr++) {
        spell_idx = research_spells[itr];

        if (spell_idx == spl_Spell_Of_Mastery) {
            base_cost = _players[player_idx].som_research_cost;
        } else {
            base_cost = spell_data_table[spell_idx].research_cost;
        }
        
        /* OGBUG: passes magic_realm instead of spell index */
        research_bonus = Player_Spell_Research_Bonus(player_idx, spell_data_table[spell_idx].magic_realm);
        
        /* Adjusted Cost = Cost * 100 / (Bonus + 100) */
        candidate_costs[itr] = (unsigned int)((long)base_cost * 100 / (long)(100 + research_bonus));
    }

    /* Determine which spell categories (AI_Groups) the wizard already knows */
    /* NOTE: Dasm definitely shows <= 90 */
    for (itr = 0; itr <= 90; itr++) {
        possessed_categories[itr] = 0;
    }

    for (itr = 0; itr < 6; itr++) { /* 6 Realms */
        for (itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_REALM; itr2++) {
            if (_players[player_idx].spells_list[((itr * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] == sls_Known) {
                known_spell_idx = ((itr * NUM_SPELLS_PER_MAGIC_REALM) + itr2 + 1);
                possessed_categories[spell_data_table[known_spell_idx].AI_Group] = ST_TRUE;
            }
        }
    }

/*
adjust research priority of the candidates by
manipulating their research costs: reduce to improve
their odds of being picked for research, or increase
to lower the odds; store the highest of the results
*/
    highest_cost = 0L;

    /* Assign weights based on category possession and personality */
    for (itr = 0; itr < candidate_count; itr++) {
        spell_idx = research_spells[itr];
        cnd_category = spell_data_table[spell_idx].AI_Group;
        
        weights_long[itr] = candidate_costs[itr];
        original_cost = candidate_costs[itr];

        /* If we already have a spell in this category, heavily penalize weight (by multiplying cost) */
        if (possessed_categories[cnd_category] == ST_TRUE) {
            weights_long[itr] *= 4;
        } 
        else {
            /* Prioritize first spell in specific combat/utility categories */
            if (cnd_category == SGRP_CombatSummon_1 || cnd_category == SGRP_CombatSummon_2 || cnd_category == SGRP_CombatSummon_3 || cnd_category == SGRP_IDK) {
                weights_long[itr] /= 10;
            }
            
            /* Various priority categories (Combat, Summons, City Buffs) */
            if (cnd_category == SGRP_CombatHarm_1  || cnd_category == SGRP_CombatCurse || cnd_category == SGRP_CombatHarm_2 || cnd_category == SGRP_CombatHarm_3 ||
                cnd_category == SGRP_CE_1 || cnd_category == SGRP_CE_2 || cnd_category == SGRP_CE_3 || cnd_category == SGRP_CE_4 ||
                cnd_category == SGRP_Disrupt || cnd_category == SGRP_CombatMisc || cnd_category == SGRP_Haste || cnd_category == SGRP_AnimateDead ||
                cnd_category == SGRP_CombatSummon_1 || cnd_category == SGRP_CombatSummon_2 || cnd_category == SGRP_CombatSummon_3 || cnd_category == SGRP_IDK) {
                weights_long[itr] /= 5;
            }
        }

        /* Militarist objective prioritizes specific combat categories */
        if (_players[player_idx].Objective == OBJ_Militarist) {
            if (cnd_category == SGRP_CombatSummon_1 || cnd_category == SGRP_CombatSummon_2 || cnd_category == SGRP_CombatSummon_3 || cnd_category == SGRP_IDK) {
                weights_long[itr] /= 2;
            }
        }

        /* Specific category prioritization for first-time acquisition */
        if (possessed_categories[cnd_category] == 0) {
            if (cnd_category == SGRP_CombatSummon_1) weights_long[itr] /= 10;
            if (cnd_category == SGRP_CombatSummon_2) weights_long[itr] /= 7;
            if (cnd_category == SGRP_CombatSummon_3) weights_long[itr] /= 5;
            if (cnd_category == SGRP_IDK) weights_long[itr] /= 3;
        }

        /* Realm-specific counter-spell prioritization (Consecration, Spell Ward) */
        if (_players[player_idx].Prim_Realm == 1 || _players[player_idx].Prim_Realm == 2) { /* Death or Chaos */
            if (spell_idx == spl_Consecration || spell_idx == spl_Spell_Ward) {
                weights_long[itr] /= 2;
            }
        } else {
            if (spell_idx == spl_Spell_Ward) { /* Spell Ward */
                weights_long[itr] /= 2;
            }
        }

        /* Utility/Meta-magic prioritization */
        if (spell_idx == spl_Disenchant_Area)  weights_long[itr] /= 2;
        if (spell_idx == spl_Disjunction)      weights_long[itr] /= 2;
        if (spell_idx == spl_Disenchant_True)  weights_long[itr] /= 5;
        if (spell_idx == spl_Disjunction_True) weights_long[itr] /= 5;
        if (spell_idx == spl_Summon_Hero)      weights_long[itr] /= 5;

        /* De-prioritize Summoning Circle */
        if (spell_idx == spl_Summoning_Circle) { 
            weights_long[itr] <<= 1;
        }

        /* If we already have this category, de-prioritize more */
        if (possessed_categories[cnd_category] == 1) {
            weights_long[itr] *= 3;
        }

        /* High cost vs current research income checks */
        if ((candidate_costs[itr] / 20) > research_income) {
            weights_long[itr] *= 2;
        }
        if ((candidate_costs[itr] / 50) > research_income) {
            weights_long[itr] *= 5;
        }

        /* Minimum weight clamping */
        if (weights_long[itr] < 1L && original_cost > 0) {
            weights_long[itr] = 1L;
        }

        /* Categories with group 0 are not researchable or invalid candidates */
        if (spell_data_table[spell_idx].AI_Group == 0) {
            weights_long[itr] = 0L;
        }

        if (weights_long[itr] > highest_cost) {
            highest_cost = weights_long[itr];
        }
    }

    /* Convert costs to selection weights: Weight = 52 - (AdjCost * 50 / Highest) */
    weights_long_total = 0L;
    for (itr = 0; itr < candidate_count; itr++) {
        if (weights_long[itr] != 0L) {
            w = 52L - (weights_long[itr] * 50L / highest_cost);
            weights_long[itr] = (w < 1L) ? 1L : w;
        }
        weights_long_total += weights_long[itr];
    }

    dbg_prn("[AI_RESEARCH] player[%d] candidates=%d highest_cost=%d research_income=%d\n", player_idx, candidate_count, highest_cost, research_income);
    for (itr = 0; itr < candidate_count; itr++) {
        dbg_prn("[AI_RESEARCH] player[%d]   [%d] spell=%3d cost=%4d adj_cost=%4d weight=%4ld category=%d %s\n", player_idx, itr, research_spells[itr], (research_spells[itr] == spl_Spell_Of_Mastery) ? _players[player_idx].som_research_cost : spell_data_table[research_spells[itr]].research_cost, candidate_costs[itr], weights_long[itr], spell_data_table[research_spells[itr]].AI_Group, possessed_categories[spell_data_table[research_spells[itr]].AI_Group] ? "(have)" : "");
    }
    dbg_prn("[AI_RESEARCH] player[%d] weights_total=%d\n", player_idx, weights_long_total);

    /* Perform weighted random roll */
    selection = Get_Weighted_Choice_Long(&weights_long[0], candidate_count);

    _players[player_idx].researching_spell_idx = research_spells[selection];

    dbg_prn("[AI_RESEARCH] player[%d] SELECTED [%d] spell=%d\n", player_idx, selection, _players[player_idx].researching_spell_idx);

    return _players[player_idx].researching_spell_idx;
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

    if((spell_data_table[spell_idx].casting_cost / 50) > Mana_Per_Turn)
    {

        spell_idx = 0;

    }

    Cast_Spell_Overland_Do(player_idx, spell_idx, spellbook_page_spell_index);

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
                CRP_AI_OVL_SpellList[CRP_AI_OVL_SpellCount] = (unsigned char)spell_idx;

                AI_OVL_Spell_Cats[ai_spell_group] = 1;

                CRP_AI_OVL_SpellCount++;

            }

        }

    }

}


// WZD o156p05
// drake178: AI_OVL_PickSummon()

// WZD o156p06
// drake178: AI_OVL_PickUnitBuff()

// WZD o156p07
// drake178: AI_OVL_PickRealmSupr()

// WZD o156p08
// drake178: AI_OVL_PickGlobal()

// WZD o156p09
// drake178: AI_OVL_PickCurse()

// WZD o156p10
// drake178: AI_OVL_PickCityBuff()

// WZD o156p11
// drake178: SPL_IsLifeSupressed()

// WZD o156p12
// drake178: SPL_IsChsSuppressed()

// WZD o156p13
// drake178: SPL_IsDthSuppressed()

// WZD o156p14
// drake178: CRP_SPL_IsNatSuppressed()

// WZD o156p15
// drake178: AI_OVL_PickDise()

// WZD o156p16
// drake178: AI_OVL_PickDisj()

// WZD o156p17
// drake178: AITP_WallofStone()

// WZD o156p18
// drake178: AITP_Transmute()

// WZD o156p19
// drake178: AITP_ChangeTerrain()
/*
; AI target picker for Change Terrain - selects the
; first desert or swamp in the catchment of the highest
; value city with such
; returns 1 if a valid target was found, or 0 otherwise
; BUG: treats catchment corners as affecting the city
; WARNING: only targets deserts and swamps
*/
/*

*/
int16_t AITP_ChangeTerrain__WIP(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp)
{
    int16_t Target_Plane = 0;
    int16_t Target_Y = 0;
    int16_t Target_X = 0;
    int16_t Tile_Y = 0;
    int16_t Plane = 0;
    int16_t Y_Modifier = 0;
    int16_t X_Modifier = 0;
    int16_t Highest_Value = 0;
    int16_t Target_City = 0;
    int16_t itr_cities = 0;  // _SI_

    STU_DEBUG_BREAK();

    Target_City = ST_UNDEFINED;

    Highest_Value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

    }



    if(Target_City == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        return ST_TRUE;
    }

}


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
int16_t AITP_SpellWard__STUB(int16_t player_idx, int16_t * city_idx, int16_t * magic_realm)
{
    *city_idx = ST_UNDEFINED;
    *magic_realm = ST_UNDEFINED;
    return ST_FALSE;
}


// WZD o156p35
// drake178: AITP_Consecration()

// WZD o156p36
// drake178: UU_DBG_GetKnownSpells()

// WZD o156p37
// drake178: CRP_DBG_SpellTargetError()
void Cast_Spell_Target_Error(int16_t spell_idx)
{

    if(Check_Release_Version() == ST_TRUE)
    {
        return;
    }

    _fstrcpy(near_buffer, spell_data_table[spell_idx].name);

    stu_strcat(near_buffer, CRP_AI_SpellTargetError);  // " could not be found for CP."

    Exit_With_Message(near_buffer);

}


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
/*

XREF:
    j_IDK_AI_City_Spell()
        Cast_CallOfTheVoid()
        Cast_Move_Fortress()
        Cast_Earthquake()
        Cast_WallOfStone()
        Cast_SpellOfReturn()
        Cast_Spell_Overland__WIP()
        Cast_Spell_Overland__WIP()

*/
int16_t Pick_Target_For_City_Enchantment__WIP(int16_t spell_target_type, int16_t * city_idx, int16_t spell_idx, int16_t player_idx)
{
    int16_t sw2_spell_idx = 0;
    int16_t sw1_spell_idx = 0;
    int16_t best_enemy_city_value = 0;
    int16_t enemy_city_idx = 0;
    int16_t itr_cities = 0;
    int16_t duped = 0;
    int16_t consecration = 0;

    if(spell_target_type == stt_Friendly_City)
    {
        sw1_spell_idx = spell_idx;
        switch(sw1_spell_idx)
        {
            // case spl_Wall_Of_Stone:    { return AITP_WallofStone(player_idx, city_idx);      } break;
            // case spl_Move_Fortress:    { return AITP_MoveFortress(player_idx, city_idx);     } break;
            // case spl_Earth_Gate:       { return AITP_EarthGate(player_idx, city_idx);        } break;
            // case spl_Flying_Fortress:  { return AITP_FlyingFortress(player_idx, city_idx);   } break;
            // case spl_Wall_Of_Fire:     { return AITP_WallofFire(player_idx, city_idx);       } break;
            // case spl_Heavenly_Light:   { return AITP_HeavenlyLight(player_idx, city_idx);    } break;
            // case spl_Altar_Of_Battle:  { return AITP_AltarofBattle(player_idx, city_idx);    } break;
            // case spl_Inspirations:     { return AITP_Inspirations(player_idx, city_idx);     } break;
            // case spl_Stream_Of_Life:   { return AITP_StreamofLife(player_idx, city_idx);     } break;
            // case spl_Astral_Gate:      { return AITP_AstralGate(player_idx, city_idx);       } break;
            // case spl_Prosperity:       { return AITP_Prosperity(player_idx, city_idx);       } break;
            // case spl_Consecration:     { return AITP_Consecration(player_idx, city_idx);     } break;
            // case spl_Cloud_Of_Shadow:  { return AITP_CloudofShadow(player_idx, city_idx);    } break;
            // case spl_Summoning_Circle: { return AITP_Summoning_Circle(player_idx, city_idx); } break;
            // case spl_Dark_Rituals:     { return AITP_DarkRituals(player_idx, city_idx);      } break;
            // case spl_Gaias_Blessing:   { return AITP_GaiasBlessing(player_idx, city_idx);    } break;
            default: { Cast_Spell_Target_Error(spell_idx); } break;
        }
    }
    else if(spell_target_type == stt_Enemy_City)
    {

        if(
            (spell_idx == spl_Earthquake)
            ||
            (spell_idx == spl_Chaos_Rift)  /* ¿ BUGBUG  Chaos Rift is also dispelled by Consecration ? */
            ||
            (spell_idx == spl_Call_The_Void)
        )
        {

            consecration = ST_FALSE;

        }
        else if(
            (spell_idx == spl_Evil_Presence)
            ||
            (spell_idx == spl_Cursed_Lands)
            ||
            (spell_idx == spl_Pestilence)
            ||
            (spell_idx == spl_Famine)
        )
        {

            consecration = ST_TRUE;

        }
        else
        {

            Cast_Spell_Target_Error(spell_idx);

        }

        enemy_city_idx = ST_UNDEFINED;
        best_enemy_city_value = 0;

        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        {

            if(
                (AI_Enemy_City_Values[itr_cities] > best_enemy_city_value)
                &&
                (_CITIES[itr_cities].owner_idx != NEUTRAL_PLAYER_IDX)
                &&
                (_CITIES[itr_cities].enchantments[CONSECRATION] == 0)
            )
            {

                if(
                    (
                        (consecration == ST_FALSE)
                        &&
                        (_CITIES[itr_cities].enchantments[CHAOS_WARD] != 0)
                    )
                    ||
                    (
                        (consecration == ST_FALSE)
                        &&
                        (_CITIES[itr_cities].enchantments[DEATH_WARD] != 0)
                    )
                )
                {

                    if(
                        (
                            (spell_idx != spl_Evil_Presence)
                            ||
                            (_players[_CITIES[itr_cities].owner_idx].spellranks[sbr_Death] != 0)
                        )
                        &&
                        (Test_Bit_Field(player_idx, &_CITIES[itr_cities].contacts) == 0)
                    )
                    {

                        duped = ST_FALSE;

                        sw2_spell_idx = spell_idx;

                        switch(sw2_spell_idx)
                        {

                            case spl_Earthquake:    // 0x1A   26  spl_Earthquake
                            {
                                // EMPTY
                            } break;

                            case spl_Call_The_Void: // 0x77   119  spl_Call_The_Void
                            {
                                // EMPTY
                            } break;

                            case spl_Chaos_Rift:    // 0x6E  110  spl_Chaos_Rift
                            {
                                if(_CITIES[itr_cities].enchantments[CHAOS_RIFT] != 0)
                                {
                                    duped = ST_TRUE;
                                }
                            } break;

                            case spl_Evil_Presence:  // 0xB7  183  spl_Evil_Presence
                            {
                                if(_CITIES[itr_cities].enchantments[EVIL_PRESENCE] != 0)
                                {
                                    duped = ST_TRUE;
                                }
                            } break;

                            case spl_Famine:  // 0xBD  189  spl_Famine
                            {
                                if(_CITIES[itr_cities].enchantments[FAMINE] != 0)
                                {
                                    duped = ST_TRUE;
                                }
                            } break;

                            case spl_Cursed_Lands:  // 0xBE  190  spl_Cursed_Lands
                            {
                                if(_CITIES[itr_cities].enchantments[EVIL_PRESENCE] != 0)
                                {
                                    duped = ST_TRUE;
                                }
                            } break;

                            case spl_Pestilence:  // 0xC4  196  spl_Pestilence
                            {
                                if(_CITIES[itr_cities].enchantments[EVIL_PRESENCE] != 0)
                                {
                                    duped = ST_TRUE;
                                }
                            } break;

                            default:
                            {
                                // EMPTY
                            } break;

                        }

                        if(duped == ST_FALSE)
                        {

                            best_enemy_city_value = AI_Enemy_City_Values[itr_cities];

                            enemy_city_idx = itr_cities;

                        }

                    }

                }

            }

        }

        if(enemy_city_idx != ST_UNDEFINED)
        {

            *city_idx = enemy_city_idx;

            return ST_TRUE;

        }
        else
        {

            return ST_FALSE;

        }

    }
    else
    {
        Cast_Spell_Target_Error(spell_idx);
    }

#ifdef STU_DEBUG
    STU_DEBUG_BREAK();
#endif

    return ST_FALSE;

}


// WZD o156p40
// drake178: AI_CanTargetCities()

// WZD o156p41
// drake178: sub_E8070()
int16_t IDK_Pick_Target_For_Unit_Enchantment__STUB(int16_t spell_target_type, int16_t * unit_idx, int16_t spell_idx, int16_t player_idx)
{

    return ST_FALSE;

}


// WZD o156p42
// drake178: sub_E8448()

// WZD o156p43
// drake178: sub_E882B()

// WZD o156p44
// drake178: sub_E88F3()
/*

*/
int16_t IDK_AITP_Disjunction__STUB(int16_t * wx, int16_t * wy, int16_t spell_idx, int16_t player_idx)
{
    int8_t var_9E[6][25] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };
    int16_t var_8 = 0;
    int16_t var_6 = 0;
    int16_t var_4 = 0;
    int16_t var_2 = 0;
    int16_t itr = 0;  // _DI_
    int16_t _SI_player_idx = 0;  // _SI_

    for(itr = 0; itr < NUM_PLAYERS; itr++)
    {

        for(var_2 = 0; var_2 < 25; var_2++)
        {

            var_9E[itr][var_2] = 0;

        }

    }

    _SI_player_idx = 0;

    if(spell_idx == spl_Spell_Binding)
    {
        
        for(itr = 0; itr < _num_players; itr++)
        {

        }

    }
    else if(
        (spell_idx == spl_Disjunction)
        ||
        (spell_idx == spl_Disjunction_True)
    )
    {

    }
    else
    {
    
    }







    return ST_FALSE;

}


// WZD o156p45
// drake178: AITP_OVL_TileSpells()
/* 
AI target picker for overland tile spells
returns 1 if a successful target is found, 0 if not,
or the passed return value if the spell is undefined
contains BUGs in every targeting function, and some
select completely inappropriate targets
*/
/*
    calls out to get wx,wy,wp, for 14 spells

*/
int16_t Get_Map_Square_Target_For_Spell(int16_t spell_target_type, int16_t * wx, int16_t * wy, int16_t * wp, int16_t spell_idx, int16_t player_idx)
{
    int16_t Passed_Return = 0;
    int16_t return_value = 0;  // DNE in Dasm

    switch(spell_idx)
    {
        case spl_Transmute:
        {
            STU_DEBUG_BREAK();
            // SPELLY  return_value = AITP_Transmute(player_idx, wx, wy, wp);
        } break;
        case spl_Change_Terrain:
        {
            STU_DEBUG_BREAK();
            /* SPELLY */  return_value = AITP_ChangeTerrain__WIP(player_idx, wx, wy, wp);
        } break;
        case spl_Ice_Storm:
        case spl_Fire_Storm:
        case spl_Black_Wind:
        case spl_Stasis:
        {
            STU_DEBUG_BREAK();
            // SPELLY  return_value AITP_OVL_HarmStack(player_idx, wx, wy, wp);
        } break;
        case spl_Floating_Island:
        {
            return_value = Pick_Target_For_Floating_Island(player_idx, wx, wy, wp);
        } break;
        case spl_Enchant_Road:
        {
            STU_DEBUG_BREAK();
            // SPELLY  return_value = AITP_EnchantRoad(player_idx, wx, wy, wp);
        } break;
        case spl_Disenchant_True:
        case spl_Disenchant_Area:
        {
            STU_DEBUG_BREAK();
            // SPELLY  return_value = AITP_Disenchant(player_idx, wx, wy, wp);
        } break;
        case spl_Corruption:
        case spl_Raise_Volcano:
        {
            STU_DEBUG_BREAK();
            // SPELLY  return_value = AITP_HarmTerrain(player_idx, wx, wy, wp, spell_idx);
        } break;
        case spl_Plane_Shift:
        {
            STU_DEBUG_BREAK();
            // SPELLY  return_value = AITP_PlaneShift(player_idx, wx, wy, wp);
        } break;
        case spl_Natures_Cures:
        {
            STU_DEBUG_BREAK();
            // SPELLY  return_value = AITP_NaturesCures(player_idx, wx, wy, wp);
        } break;
        default:
        {
            Cast_Spell_Target_Error(spell_idx);  // "[spell name] could not be found for CP."
            return_value = spell_target_type;
            Passed_Return = return_value;
        } break;
    }

    return return_value;

}


// WZD o156p46
// drake178: AITP_OVL_HarmStack()


// WZD o156p47
// drake178: AITP_FloatingIsland()
/*
; AI target picker for Floating Island - selects the
; closest ocean tile within 10 tiles of a random city
; in a cardinal direction
; returns 1 if a valid target was found, or 0 otherwise
; BUG: utterly useless targeting
*/
/*

*/
int16_t Pick_Target_For_Floating_Island(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp)
{
    int16_t city_idx = 0;
    int16_t Tries = 0;
    int16_t uu_player_idx = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t Vertical = 0;
    int16_t city_wp = 0;
    int16_t UpLeft = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t check_value = 0;  // _DI_
    int16_t itr_squares = 0;  // _SI_

    uu_player_idx = player_idx;

    check_value = ST_FALSE;

    // ; make 200 attempts to find a random city that belongs
    // ; to the player
    for(Tries = 0; ((Tries < 200) && (check_value == ST_FALSE)); Tries++)
    {

        city_idx = (Random(_cities) - 1);

        if(_CITIES[city_idx].owner_idx == player_idx)
        {

            check_value = ST_TRUE;

            city_wp = _CITIES[city_idx].wp;
            
            city_wx = _CITIES[city_idx].wx;
            
            city_wy = _CITIES[city_idx].wy;

        }

    }

    if(check_value == ST_FALSE)
    {
        return ST_FALSE;
    }

    // ; find the closest ocean tile within 10 tiles in any of
    // ; the cardinal directions (straight only)

    itr_squares = 0;

    UpLeft = ST_TRUE;

    Vertical = ST_FALSE;

    check_value = ST_FALSE;

    while((check_value == ST_FALSE) && (itr_squares < 10))
    {
        if(UpLeft == ST_FALSE)
        {
            if(Vertical == ST_FALSE)
            {
                if(Square_Is_Legal_For_Floating_Island((city_wx + itr_squares), city_wy, city_wp) == ST_TRUE)
                {
                    check_value = ST_TRUE;
                    target_wx = (city_wx + itr_squares);
                    target_wy = city_wy;
                }
                Vertical = ST_TRUE;
            }
            else  /* (Vertical == ST_TRUE) */
            {
                if(Square_Is_Legal_For_Floating_Island(city_wx, (city_wy + itr_squares), city_wp) == ST_TRUE)
                {
                    check_value = ST_TRUE;
                    target_wx = city_wx;
                    target_wy = (city_wy + itr_squares);
                }
                Vertical = ST_TRUE;
            }
        }
        else  /* (UpLeft == ST_TRUE) */
        {
            if(Vertical == ST_FALSE)
            {
                if(Square_Is_Legal_For_Floating_Island((city_wx - itr_squares), city_wy, city_wp) == ST_TRUE)
                {
                    check_value = ST_TRUE;
                    target_wx = (city_wx - itr_squares);
                    target_wy = city_wy;
                }
                Vertical = ST_TRUE;
            }
            else  /* (Vertical == ST_TRUE) */
            {
                if(Square_Is_Legal_For_Floating_Island(city_wx, (city_wy - itr_squares), city_wp) == ST_TRUE)
                {
                    check_value = ST_TRUE;
                    target_wx = city_wx;
                    target_wy = (city_wy - itr_squares);
                }
                Vertical = ST_FALSE;
                itr_squares++;
                UpLeft = ST_FALSE;
            }
        }
    }

    if(check_value == ST_FALSE)
    {
        return ST_FALSE;
    }
    else
    {
        *wx = target_wx;
        *wy = target_wy;
        *wp = city_wp;
        return ST_TRUE;
    }

}


// WZD o156p48
// drake178: AITP_EnchantRoad()

// WZD o156p49
// drake178: AITP_Disenchant()

// WZD o156p50
// drake178: AITP_HarmTerrain()

// WZD o156p51
// drake178: sub_E9FA9()

// WZD o156p52
// drake178: AITP_PlaneShift()

// WZD o156p53
// drake178: AITP_NaturesCures()

// WZD o156p54
// drake178: sub_EA43C()
int16_t IDK_AITP_Target_Wizard__STUB(int16_t * spell_target_idx, int16_t spell_idx, int16_t player_idx)
{

    return ST_FALSE;
    
}


// WZD o156p55
// drake178: sub_EA4FE()
int16_t AITP_SpellWard_Wrapper__STUB(int16_t * city_idx, int16_t * magic_realm, int16_t player_idx)
{

    return AITP_SpellWard__STUB(player_idx, city_idx, magic_realm);

}


// WZD o156p56
// drake178: AI_MyrrorAdvantage()

// WZD o156p57
// drake178: TILE_IsOcean()
/*
; returns 1 if the tile is a shore, ocean, or lake, or
; 0 otherwise
; differs from TILE_IsAISailable and TILE_IsAISailable2
; in that this also returns 0 for river-outflow single
; tile lakes
; differs from TILE_IsSailable in that this returns 0
; for all inland lakes
*/
/*

*/
int16_t Square_Is_Legal_For_Floating_Island(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;  // DNE in Dasm

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        ((terrain_type < _Tundra00001000) && (terrain_type > _River1111_5))
        ||
        ((terrain_type <= _Shore111R1110) && (terrain_type > _Desert10101111))
        ||
        ((terrain_type <= _Shore000R0000) && (terrain_type > _1LakeRiv_S))
        ||
        ((terrain_type <= _Shore10101111) && (terrain_type != tt_BugGrass) && (terrain_type != _1Lake))
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}


// WZD o156p58
// drake178: sub_EA61E()
// int16_t AITP_Summoning_Circle(int16_t player_idx, int16_t * city_idx)

// WZD o156p59
// drake178: WIZ_SummonInFortress()
