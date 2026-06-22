/*
    AI Spell

    WIZARDS.EXE
    ovr156

MoO2
Module: AITECH
*/

#include "../../STU/src/AI_METRICS.h"
#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"

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
#include "NEXTTURN.h"
#include "SBookScr.h"
#include "Spellbook.h"
#include "SPELLDEF.h"
#include "TerrType.h"
#include "UNITTYPE.h"

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



// WZD 36AA:D386                                                 BEGIN:  ovr156 - Uninitialized Data

// WZD 36AA:D386
int16_t AI_OVL_SplPriorities[50];
// WZD 36AA:D3EA
int16_t AI_NodeOpportunity;

// WZD 36AA:D3EA                                                 END:  ovr156 - Uninitialized Data




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

    for(itr = 0; itr < 8; itr++) {
        if(research_spells[itr] != 0) {
            candidate_count++;
        }
    }

    if(candidate_count == 0) {
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_AIMOVE, "[AI_RESEARCH] player[%d] candidate_count=0, no spells to research", player_idx);
#endif
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
    for(itr = 0; itr < candidate_count; itr++) {
        spell_idx = research_spells[itr];

        if(spell_idx == spl_Spell_Of_Mastery) {
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
    for(itr = 0; itr <= 90; itr++) {
        possessed_categories[itr] = 0;
    }

    for(itr = 0; itr < 6; itr++) { /* 6 Realms */
        for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_REALM; itr2++) {
            if(_players[player_idx].spells_list[((itr * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] == sls_Known) {
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
    for(itr = 0; itr < candidate_count; itr++) {
        spell_idx = research_spells[itr];
        cnd_category = spell_data_table[spell_idx].AI_Group;
        
        weights_long[itr] = candidate_costs[itr];
        original_cost = candidate_costs[itr];

        /* If we already have a spell in this category, heavily penalize weight (by multiplying cost) */
        if(possessed_categories[cnd_category] == ST_TRUE) {
            weights_long[itr] *= 4;
        } 
        else {
            /* Prioritize first spell in specific combat/utility categories */
            if(cnd_category == SGRP_CombatSummon_1 || cnd_category == SGRP_CombatSummon_2 || cnd_category == SGRP_CombatSummon_3 || cnd_category == SGRP_IDK) {
                weights_long[itr] /= 10;
            }
            
            /* Various priority categories (Combat, Summons, City Buffs) */
            if(cnd_category == SGRP_CombatHarm_1  || cnd_category == SGRP_CombatCurse || cnd_category == SGRP_CombatHarm_2 || cnd_category == SGRP_CombatHarm_3 ||
                cnd_category == SGRP_CE_1 || cnd_category == SGRP_CE_2 || cnd_category == SGRP_CE_3 || cnd_category == SGRP_CE_4 ||
                cnd_category == SGRP_Disrupt || cnd_category == SGRP_CombatMisc || cnd_category == SGRP_Haste || cnd_category == SGRP_AnimateDead ||
                cnd_category == SGRP_CombatSummon_1 || cnd_category == SGRP_CombatSummon_2 || cnd_category == SGRP_CombatSummon_3 || cnd_category == SGRP_IDK) {
                weights_long[itr] /= 5;
            }
        }

        /* Militarist objective prioritizes specific combat categories */
        if(_players[player_idx].Objective == OBJ_Militarist) {
            if(cnd_category == SGRP_CombatSummon_1 || cnd_category == SGRP_CombatSummon_2 || cnd_category == SGRP_CombatSummon_3 || cnd_category == SGRP_IDK) {
                weights_long[itr] /= 2;
            }
        }

        /* Specific category prioritization for first-time acquisition */
        if(possessed_categories[cnd_category] == 0) {
            if(cnd_category == SGRP_CombatSummon_1) weights_long[itr] /= 10;
            if(cnd_category == SGRP_CombatSummon_2) weights_long[itr] /= 7;
            if(cnd_category == SGRP_CombatSummon_3) weights_long[itr] /= 5;
            if(cnd_category == SGRP_IDK) weights_long[itr] /= 3;
        }

        /* Realm-specific counter-spell prioritization (Consecration, Spell Ward) */
        if(_players[player_idx].Prim_Realm == 1 || _players[player_idx].Prim_Realm == 2) { /* Death or Chaos */
            if(spell_idx == spl_Consecration || spell_idx == spl_Spell_Ward) {
                weights_long[itr] /= 2;
            }
        } else {
            if(spell_idx == spl_Spell_Ward) { /* Spell Ward */
                weights_long[itr] /= 2;
            }
        }

        /* Utility/Meta-magic prioritization */
        if(spell_idx == spl_Disenchant_Area)  weights_long[itr] /= 2;
        if(spell_idx == spl_Disjunction)      weights_long[itr] /= 2;
        if(spell_idx == spl_Disenchant_True)  weights_long[itr] /= 5;
        if(spell_idx == spl_Disjunction_True) weights_long[itr] /= 5;
        if(spell_idx == spl_Summon_Hero)      weights_long[itr] /= 5;

        /* De-prioritize Summoning Circle */
        if(spell_idx == spl_Summoning_Circle) { 
            weights_long[itr] <<= 1;
        }

        /* If we already have this category, de-prioritize more */
        if(possessed_categories[cnd_category] == 1) {
            weights_long[itr] *= 3;
        }

        /* High cost vs current research income checks */
        if((candidate_costs[itr] / 20) > research_income) {
            weights_long[itr] *= 2;
        }
        if((candidate_costs[itr] / 50) > research_income) {
            weights_long[itr] *= 5;
        }

        /* Minimum weight clamping */
        if(weights_long[itr] < 1L && original_cost > 0) {
            weights_long[itr] = 1L;
        }

        /* Categories with group 0 are not researchable or invalid candidates */
        if(spell_data_table[spell_idx].AI_Group == 0) {
            weights_long[itr] = 0L;
        }

        if(weights_long[itr] > highest_cost) {
            highest_cost = weights_long[itr];
        }
    }

    /* Convert costs to selection weights: Weight = 52 - (AdjCost * 50 / Highest) */
    weights_long_total = 0L;
    for(itr = 0; itr < candidate_count; itr++) {
        if(weights_long[itr] != 0L) {
            w = 52L - (weights_long[itr] * 50L / highest_cost);
            weights_long[itr] = (w < 1L) ? 1L : w;
        }
        weights_long_total += weights_long[itr];
    }

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "[AI_RESEARCH] player[%d] candidates=%d highest_cost=%d research_income=%d", player_idx, candidate_count, highest_cost, research_income);
    for(itr = 0; itr < candidate_count; itr++) {
        LOG_DEBUG(LOG_CAT_AIMOVE, "[AI_RESEARCH] player[%d]   [%d] spell=%3d cost=%4d adj_cost=%4d weight=%4ld category=%d %s", player_idx, itr, research_spells[itr], (research_spells[itr] == spl_Spell_Of_Mastery) ? _players[player_idx].som_research_cost : spell_data_table[research_spells[itr]].research_cost, candidate_costs[itr], weights_long[itr], spell_data_table[research_spells[itr]].AI_Group, possessed_categories[spell_data_table[research_spells[itr]].AI_Group] ? "(have)" : "");
    }
    LOG_DEBUG(LOG_CAT_AIMOVE, "[AI_RESEARCH] player[%d] weights_total=%d", player_idx, weights_long_total);
#endif

    /* Perform weighted random roll */
    selection = Get_Weighted_Choice_Long(&weights_long[0], candidate_count);

    _players[player_idx].researching_spell_idx = research_spells[selection];

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "[AI_RESEARCH] player[%d] SELECTED [%d] spell=%d", player_idx, selection, _players[player_idx].researching_spell_idx);
#endif

    AI_Metrics_Emit_Decision(_turn, player_idx, "RESEARCH",
        player_idx,
        _players[player_idx].researching_spell_idx,
        spell_data_table[_players[player_idx].researching_spell_idx].name,
        (int16_t)weights_long[selection],
        (int16_t)candidate_count);

    return _players[player_idx].researching_spell_idx;
}


// WZD o156p02
/**
 * @brief Selects the AI wizard's next overland spell to cast.
 *
 * This routine rebuilds the AI's current overland spell list, asks the spell
 * category picker which class of spell to prefer, and then resolves that
 * category into a concrete spell index using the category-specific picker.
 * After a spell is chosen, it performs a coarse affordability check using the
 * wizard's perceived mana-per-turn income. If the spell is estimated to take
 * too long to cast, the selection is cancelled and the AI casts nothing.
 *
 * The final spell index is handed off to Cast_Spell_Overland_Do().
 *
 * @param player_idx Index of the AI-controlled player whose spell choice is
 *                   being evaluated.
 *
 * @return This function does not return a value. It delegates the selected
 *         spell to Cast_Spell_Overland_Do() after applying the mana check.
 *
 * @note The local spellbook-page index is currently hard-coded to zero.
 * @note The function relies on AI_Compute_Spells_Info() and AI_Select_Spell_Group()
 *       to populate the available-spell set and high-level category decision.
 */
void AI_Spell_Select(int16_t player_idx)
{
    int16_t mana_income = 0;
    int16_t food_income = 0;
    int16_t gold_income = 0;
    uint8_t * players_spell_list = NULL;
    int16_t mana_per_turn = 0;
    int16_t spellbook_page_spell_index = 0;
    int16_t spell_category = 0;
    int16_t spell_idx = 0;

    /* Treat spell list as 1-based index by shifting pointer */
    players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

    AI_Compute_Spells_Info(player_idx);

    spell_category = AI_Select_Spell_Group(player_idx);

    switch(spell_category)
    {
        case 0:
            spell_idx = 0;
            break;
        case 1:
            spell_idx = AI_Select_Spell_Group_Summon(player_idx);
            break;
        case 2:
            spell_idx = AI_OVL_PickUnitBuff(player_idx);
            break;
        case 3:
            spell_idx = AI_OVL_PickCityBuff(player_idx);
            break;
        case 4:
            spell_idx = AI_OVL_PickDise(player_idx);
            break;
        case 5:
            spell_idx = AI_OVL_PickDisj(player_idx);
            break;
        case 6:
            spell_idx = spl_Summoning_Circle;
            break;
        case 7:
            spell_idx = AI_OVL_PickCurse(player_idx);
            break;
        case 8:
            spell_idx = AI_OVL_PickRealmSupr(player_idx);
            break;
        case 9:
            spell_idx = AI_OVL_PickGlobal(player_idx);
            break;
        case 10:
            spell_idx = spl_Spell_Of_Mastery;
            break;
        default:
            spell_idx = 0;
            break;
    }

    /* Calculate perceived mana availability: 10% of reserve + current turn income */
    /* Turn-based cost check: If spell takes more than ~50 turns of "perceived" mana, cancel it */
    Player_Resource_Income_Total(player_idx, &gold_income, &food_income, &mana_income);
    mana_per_turn = ((_players[player_idx].mana_reserve / 10) + mana_income);
    if((spell_data_table[spell_idx].casting_cost / 50) > mana_per_turn)
    {
        spell_idx = 0;
    }

    Cast_Spell_Overland_Do(player_idx, spell_idx, spellbook_page_spell_index);

}


// WZD o156p03
/*
selects a spell category for the AI to use next:
  0 - no spell
  1 - summon
  2 - unit enchantment
  3 - city enchantment
  4 - disenchant / true
  5 - disjunction / true
  6 - summoning circle
  7 - overland curse / damage
  8 - suppression global
  9 - global enchantment
 10 - spell of mastery
*/
/**
 * @brief Selects the high-level overland spell category the AI should prioritize casting.
 *
 * @details
 * This function evaluates the AI player's current game state and computes weighted modifier values
 * for 11 distinct overland spell categories:
 *   0. Early-game bonus (turns < 50)
 *   1. Summoning (including hero/unit summoning priority)
 *   2. Unit buffs and movement enhancements (turn-dependent)
 *   3. City buffs and protective spells
 *   4. Disenchant spells (targets specific enchantments)
 *   5. Disjunction spells (counters opponent's global enchantments)
 *   6. Summoning Circle
 *   7. Curse spells (offense against hostile opponents)
 *   8. Realm supremacy (specific realm-based curses and enhancements)
 *   9. Global enchantments (Awareness, Wind Mastery, etc.)
 *   10. Spell of Mastery
 *
 * Modifiers are adjusted based on:
 *   - Current turn count
 *   - Player traits (conjurer, channeler)
 *   - Fortress vs. summon city alignment
 *   - Friendly vs. hostile army values
 *   - Known spells and active global enchantments
 *   - Opponent diplomatic status and realm composition
 *   - Mana reserves
 *   - Various strategic conditions
 *
 * The function returns a weighted random choice from the modifier array, ensuring that
 * categories with higher modifiers are more likely to be selected.
 *
 * @param player_idx Index of the AI player whose spell category preference is being evaluated.
 *
 * @return Index (0–10) of the selected spell category to prioritize. The selection is weighted
 *         by the computed modifiers, so high-priority categories are more likely to be chosen.
 *
 * @note The function modifies the global variable `_players[player_idx].cp_target_3` to
 *       store the selected opponent target index if hostile opponents exist.
 * @note Some modifiers are hard-coded (e.g., offset 0x53 in Historian for Death Wish),
 *       reflecting legacy assembly code patterns.
 * @note `modifiers[9]` (global enchantments) is capped at a maximum value of 300 after
 *       all individual checks to prevent over-weighting.
 */
int16_t AI_Select_Spell_Group(int16_t player_idx)
{
    int16_t modifiers[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Opponent_Weights[6] = { 0, 0, 0, 0, 0, 0 };
    uint8_t * players_spell_list = 0;
    int32_t Hostile_Army_Value = 0;
    int32_t Own_Army_Value = 0;
    int16_t Own_City_Count = 0;
    int16_t fortress_landmass_idx = 0;
    int16_t Have_Spirit = 0;
    int16_t summon_city_idx = 0;
    int16_t summon_wp = 0;
    int16_t niu_summon_landmass_idx = 0;
    int16_t itr = 0;

    /* Treat spell list as 1-based index by shifting pointer */
    players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

    for(itr = 0; itr <= 10; itr++)
    {
        modifiers[itr] = 0;
    }

    if(_turn < 50)
    {
        modifiers[0] = (50 - _turn);
    }

    summon_wp = _players[player_idx].summon_wp;
    niu_summon_landmass_idx = _landmasses[summon_wp * 960 + _players[player_idx].summon_wy * 60 + _players[player_idx].summon_wx];
    summon_city_idx = 0;

    for(itr = 0; itr < _cities; itr++)
    {
        if(
            (_CITIES[itr].wx == _players[player_idx].summon_wx)
            &&
            (_CITIES[itr].wy == _players[player_idx].summon_wy)
            &&
            (_CITIES[itr].wp == _players[player_idx].summon_wp)
        )
        {
            summon_city_idx = itr;
        }
    }

    modifiers[1] = 100;
    modifiers[1] += (6 - Player_Hero_Count(player_idx)) * 10;

    if(_players[player_idx].magic_power_strategy == 3)
    {
        modifiers[1] += 30;
    }

    Have_Spirit = ST_FALSE;
    AI_NodeOpportunity = ST_FALSE;

    for(itr = 0; itr < _units; itr++)
    {
        if(_UNITS[itr].owner_idx == player_idx && _UNITS[itr].wp == summon_wp)
        {
            if(_unit_type_table[_UNITS[itr].type].Abilities & UA_MELD)
            {
                Have_Spirit = ST_TRUE;
            }
        }
    }

    if(Have_Spirit != ST_TRUE)
    {
        for(itr = 0; itr < NUM_NODES; itr++)
        {
            if(_NODES[itr].wp == summon_wp && _NODES[itr].owner_idx != player_idx)
            {
                AI_NodeOpportunity = ST_TRUE;
            }
        }
        if(AI_NodeOpportunity == ST_TRUE)
        {
            modifiers[1] += 30;
        }
    }

    Own_Army_Value = 0;
    for(itr = 0; itr < _ai_all_own_stack_count; itr++)
    {
        Own_Army_Value += _ai_all_own_stacks[itr].value;
    }

    Hostile_Army_Value = 0;
    for(itr = 0; itr < _ai_all_enemy_stack_count; itr++)
    {
        Hostile_Army_Value += _ai_all_enemy_stacks[itr].value;
    }

    if(_players[player_idx].conjurer != 0 || _players[player_idx].channeler != 0)
    {
        modifiers[1] = (modifiers[1] * 3) / 2;
    }

    if(Player_Summon_City_Is_Fortress_City(player_idx) == 0)
    {
        modifiers[1] *= 5;
    }

    if(players_spell_list[spl_Disenchant_Area] == sls_Known || players_spell_list[spl_Disenchant_True] == sls_Known)
    {
        for(itr = 0; itr < _cities; itr++)
        {
            if(_CITIES[itr].owner_idx == player_idx)
            {
                if(_CITIES[itr].enchantments[CHAOS_RIFT] != 0) modifiers[4] += 40;
                if(_CITIES[itr].enchantments[EVIL_PRESENCE] != 0) modifiers[4] += 40;
                if(_CITIES[itr].enchantments[CURSED_LANDS] != 0) modifiers[4] += 40;
                if(_CITIES[itr].enchantments[PESTILENCE] != 0) modifiers[4] += 40;
                if(_CITIES[itr].enchantments[FAMINE] != 0) modifiers[4] += 40;
            }
            else if(_CITIES[itr].owner_idx == 0)
            {
                if(_CITIES[itr].enchantments[FLYING_FORTRESS] != 0) modifiers[4] += 300;
            }
        }

        for(itr = 0; itr < NUM_NODES; itr++)
        {
            if(_NODES[itr].owner_idx == player_idx)
            {
                if(_NODES[itr].flags & NF_WARPED) modifiers[4] += 100;
            }
        }

        fortress_landmass_idx = _landmasses[_FORTRESSES[player_idx].wp * 960 + _FORTRESSES[player_idx].wy * 60 + _FORTRESSES[player_idx].wx];

        for(itr = 0; itr < _units; itr++)
        {
            if(_UNITS[itr].owner_idx == 0)
            {
                if(((unsigned int *)&_UNITS[itr].enchantments)[0] != 0 || ((unsigned int *)&_UNITS[itr].enchantments)[1] != 0)
                {
                    if(_landmasses[_UNITS[itr].wp * 960 + _UNITS[itr].wy * 60 + _UNITS[itr].wx] == fortress_landmass_idx)
                    {
                        modifiers[4] += 5;
                    }
                }
            }
        }
    }

    if(players_spell_list[spl_Disjunction] == sls_Known || players_spell_list[spl_Disjunction_True] == sls_Known)
    {
        for(itr = 0; itr < _num_players; itr++)
        {
            if(itr == player_idx)
            {
                continue;
            }

            if(_players[player_idx].Prim_Realm != sbr_Death && _players[itr].Globals[ETERNAL_NIGHT] != 0)
            {
                modifiers[5] += 100;
            }
            if(_players[itr].Globals[EVIL_OMENS] != 0)
            {
                if(_players[itr].Prim_Realm == sbr_Life || _players[itr].Prim_Realm == sbr_Nature)
                {
                    modifiers[5] += 300;
                }
            }
            if(_players[itr].Globals[EVIL_OMENS] != 0)
            {
                if(_players[itr].Sec_Realm == sbr_Life || _players[itr].Sec_Realm == sbr_Nature)
                {
                    modifiers[5] += 150;
                }
            }
            if(_players[itr].Globals[ZOMBIE_MASTERY] != 0) modifiers[5] += 100;
            if(_players[itr].Globals[AURA_OF_MAJESTY] != 0) modifiers[5] += 25;
            if(_players[itr].Globals[WIND_MASTERY] != 0) modifiers[5] += 25;
            if(_players[itr].Globals[SUPPRESS_MAGIC] != 0) modifiers[5] += 2000;
            if(_players[itr].Globals[AURA_OF_MAJESTY] != 0) modifiers[5] += 25;
            
            if(_players[itr].Globals[NATURES_WRATH] != 0) {
                if(_players[itr].Prim_Realm == sbr_Death || _players[itr].Prim_Realm == sbr_Chaos) {
                    modifiers[5] += 400;
                }
            }
            if(_players[itr].Globals[NATURES_WRATH] != 0) {
                if(_players[itr].Sec_Realm == sbr_Death || _players[itr].Sec_Realm == sbr_Chaos) {
                    modifiers[5] += 200;
                }
            }
            
            if(_players[itr].Globals[HERB_MASTERY] != 0) modifiers[5] += 50;
            if(_players[itr].Globals[CHAOS_SURGE] != 0) {
                if(_players[itr].Prim_Realm != sbr_Chaos && _players[itr].Sec_Realm != sbr_Chaos) {
                    modifiers[5] += 100;
                }
            }
            if(_players[itr].Globals[DOOM_MASTERY] != 0) modifiers[5] += 200;
            if(_players[itr].Globals[GREAT_WASTING] != 0) modifiers[5] += 50;
            if(_players[itr].Globals[METEOR_STORM] != 0) modifiers[5] += 100;
            if(_players[itr].Globals[ARMAGEDDON] != 0) modifiers[5] += 200;
            
            if(_players[itr].Globals[TRANQUILITY] != 0) {
                if(_players[itr].Prim_Realm == sbr_Chaos) modifiers[5] += 500;
            }
            if(_players[itr].Globals[TRANQUILITY] != 0) {
                if(_players[itr].Sec_Realm == sbr_Chaos) modifiers[5] += 250;
            }
            if(_players[itr].Globals[LIFE_FORCE] != 0) {
                if(_players[itr].Prim_Realm == sbr_Death) modifiers[5] += 500;
            }
            if(_players[itr].Globals[LIFE_FORCE] != 0) {
                if(_players[itr].Sec_Realm == sbr_Death) modifiers[5] += 250;
            }
            
            if(_players[itr].Globals[CRUSADE] != 0) modifiers[5] += 100;
            if(_players[itr].Globals[JUST_CAUSE] != 0) modifiers[5] += 10;
            if(_players[itr].Globals[HOLY_ARMS] != 0) modifiers[5] += 100;
            if(_players[itr].Globals[PLANAR_SEAL] != 0)
            {
                if(itr == 0 || Random(5) == 1)
                {
                    modifiers[5] += 200;
                }
            }
            if(_players[itr].Globals[PLANAR_SEAL] != 0) modifiers[5] += 25;
            if(_players[itr].Globals[CHARM_OF_LIFE] != 0) modifiers[5] += 200;
        }
    }

    if(players_spell_list[spl_Spell_Of_Mastery] == sls_Known)
    {
        modifiers[10] = 500;
    }

    if(_cp_hostile_opponent_count > 0)
    {
        for(itr = 0; itr < NUM_PLAYERS; itr++)
        {
            Opponent_Weights[itr] = 0;
        }
        for(itr = 0; itr < _cp_hostile_opponent_count; itr++)
        {
            Opponent_Weights[itr] = (
                (100 - _players[player_idx].Dipl.Visible_Rel[_cp_hostile_opponents[itr]]) -
                (_players[player_idx].Dipl.Hidden_Rel[_cp_hostile_opponents[itr]] * 2)
            );
            SETMIN(Opponent_Weights[itr],0);
            if(_players[player_idx].Dipl.Dipl_Status[_cp_hostile_opponents[itr]] >= 3)
            {
                Opponent_Weights[itr] += 50;
            }
            if(_cp_hostile_opponents[itr] == 0)
            {
                Opponent_Weights[itr] *= (_difficulty + 1);
            }
        }
        _players[player_idx].cp_target_3 = Get_Weighted_Choice(Opponent_Weights, _cp_hostile_opponent_count);
    }
    else
    {
        _players[player_idx].cp_target_3 = ST_UNDEFINED;
    }

    if(_players[player_idx].cp_target_3 != ST_UNDEFINED)
    {
        /* Checks trait flags via hardcoded offset to trait array */
        if(g_ai_spell_group_flags[SGRP_Corruption] == 1 ||
            g_ai_spell_group_flags[SGRP_Famine] == 1 ||
            g_ai_spell_group_flags[SGRP_RaiseVolcano] == 1 ||
            g_ai_spell_group_flags[SGRP_DestroyCity] == 1 ||
            g_ai_spell_group_flags[SGRP_WarpNode] == 1 ||
            g_ai_spell_group_flags[SGRP_Stasis] == 1 ||
            g_ai_spell_group_flags[9] == 1 ||
            g_ai_spell_group_flags[SGRP_OVL_Damage] == 1 ||
            g_ai_spell_group_flags[SGRP_Earthquake] == 1 ||
            g_ai_spell_group_flags[0x4A] == 1 ||
            g_ai_spell_group_flags[SGRP_TimeStop] == 1 ||
            ((unsigned char *)&_players[player_idx].Historian)[0x53] == 1 ||
            g_ai_spell_group_flags[SGRP_DeathWish] == 1 ||
            g_ai_spell_group_flags[SGRP_HarmWizard] == 1) {

            modifiers[7] = (Opponent_Weights[_players[player_idx].cp_target_3] + TBL_AI_PRS_War_Mod[_players[player_idx].Personality]) / 4;
        }
    }

    if(modifiers[7] < 0) modifiers[7] = 0;

    if(g_ai_spell_group_flags[SGRP_WallsRituals] == 1 || g_ai_spell_group_flags[0x1B] == 1)
    {
        if(_players[HUMAN_PLAYER_IDX].Dipl.Contacted[player_idx] == ST_TRUE)
        {
            modifiers[3] = 50;
        }
        else
        {
            modifiers[3] = 5;
        }
    }

    if(g_ai_spell_group_flags[SGRP_CityBuff_1] == 1 ||
        g_ai_spell_group_flags[SGRP_CityBuff_3] == 1 ||
        g_ai_spell_group_flags[SGRP_TerrainMod] == 1 ||
        g_ai_spell_group_flags[SGRP_CityBuff_2] == 1
    )
    {
        modifiers[3] = 150;
    }

    if(g_ai_spell_group_flags[SGRP_MoveBuff_1] == 1 ||
        g_ai_spell_group_flags[SGRP_MoveBuff_2] == 1 ||
        g_ai_spell_group_flags[SGRP_MoveBuff_3] == 1 ||
        g_ai_spell_group_flags[SGRP_Gates] == 1
    )
    {
        if(_turn < 50) modifiers[2] = 0;
        else if(_turn < 200) modifiers[2] = 100;
        else modifiers[2] = 200;
    }

    if(g_ai_spell_group_flags[SGRP_AttackBuff_1] == 1 ||
        g_ai_spell_group_flags[SGRP_AttackBuff_2] == 1 ||
        g_ai_spell_group_flags[0x32] == 1 ||
        g_ai_spell_group_flags[0x48] == 1 ||
        g_ai_spell_group_flags[SGRP_DefenseBuff_1] == 1 ||
        g_ai_spell_group_flags[SGRP_ImmolInvuln] == 1 ||
        g_ai_spell_group_flags[SGRP_DefenseBuff_2] == 1 ||
        g_ai_spell_group_flags[SGRP_RegenMassInvis] == 1 ||
        g_ai_spell_group_flags[SGRP_Heals] == 1
    )
    {
        if(_turn > 20 && _turn < 200)
        {
            modifiers[2] += (250 - _turn) / 2;
        }
        else
        {
            modifiers[2] += 25;
        }
    }

    if(players_spell_list[spl_Herb_Mastery] == sls_Known && _players[player_idx].Globals[HERB_MASTERY] == 0) modifiers[9] += 100;

    if(players_spell_list[spl_Natures_Wrath] == sls_Known && _players[player_idx].Globals[NATURES_WRATH] == 0)
    {
        /* Assembly checks against human player's realms (player 0) */
        if(_players[0].Prim_Realm == sbr_Death || _players[0].Sec_Realm == sbr_Death ||
            _players[0].Prim_Realm == sbr_Chaos || _players[0].Sec_Realm == sbr_Chaos) {
            modifiers[8] += 100;
        }
    }

    if(players_spell_list[spl_Wind_Mastery] == sls_Known && _players[player_idx].Globals[WIND_MASTERY] == 0) modifiers[9] += 75;
    if(players_spell_list[spl_Awareness] == sls_Known && _players[player_idx].Globals[AWARENESS] == 0) modifiers[9] += 500;
    if(players_spell_list[spl_Aura_Of_Majesty] == sls_Known && _players[player_idx].Globals[AURA_OF_MAJESTY] == 0) modifiers[9] += 100;
    if(players_spell_list[spl_Suppress_Magic] == sls_Known && _players[player_idx].Globals[SUPPRESS_MAGIC] == 0) modifiers[8] += 100;
    if(players_spell_list[spl_Spell_Binding] == sls_Known) modifiers[9] += 100;
    if(players_spell_list[spl_Great_Wasting] == sls_Known && _players[player_idx].Globals[GREAT_WASTING] == 0) modifiers[9] += 100;
    if(players_spell_list[spl_Chaos_Surge] == sls_Known && _players[player_idx].Globals[CHAOS_SURGE] == 0) modifiers[9] += 100;
    if(players_spell_list[spl_Doom_Mastery] == sls_Known && _players[player_idx].Globals[DOOM_MASTERY] == 0) modifiers[9] += 100;
    if(players_spell_list[spl_Meteor_Storms] == sls_Known && _players[player_idx].Globals[METEOR_STORM] == 0) modifiers[9] += 100;
    if(players_spell_list[spl_Armageddon] == sls_Known && _players[player_idx].Globals[ARMAGEDDON] == 0) modifiers[9] += 100;
    if(players_spell_list[spl_Just_Cause] == sls_Known && _players[player_idx].Globals[JUST_CAUSE] == 0) modifiers[9] += 100;
    if(players_spell_list[spl_Holy_Arms] == sls_Known && _players[player_idx].Globals[HOLY_ARMS] == 0) modifiers[9] += 100;
    if(players_spell_list[spl_Charm_Of_Life] == sls_Known && _players[player_idx].Globals[CHARM_OF_LIFE] == 0) modifiers[9] += 100;
    if(players_spell_list[spl_Crusade] == sls_Known && _players[player_idx].Globals[CRUSADE] == 0) modifiers[9] += 100;
    if(players_spell_list[spl_Great_Unsummoning] == sls_Known) modifiers[9] += 100;
    if(players_spell_list[spl_Death_Wish] == sls_Known) modifiers[9] += 100;

    if(players_spell_list[spl_Life_Force] == sls_Known && _players[player_idx].Globals[LIFE_FORCE] == 0)
    {
        if(_players[0].Prim_Realm == sbr_Death || _players[0].Sec_Realm == sbr_Death) modifiers[8] += 100;
    }
    if(players_spell_list[spl_Tranquility] == sls_Known && _players[player_idx].Globals[TRANQUILITY] == 0)
    {
        if(_players[0].Prim_Realm == sbr_Chaos || _players[0].Sec_Realm == sbr_Chaos) modifiers[8] += 100;
    }

    if(players_spell_list[spl_Eternal_Night] == sls_Known && _players[player_idx].Globals[ETERNAL_NIGHT] == 0) modifiers[9] += 100;

    if(players_spell_list[spl_Evil_Omens] == sls_Known && _players[player_idx].Globals[EVIL_OMENS] == 0) {
        if(_players[0].Prim_Realm == sbr_Life || _players[0].Sec_Realm == sbr_Life ||
            _players[0].Prim_Realm == sbr_Nature || _players[0].Sec_Realm == sbr_Nature) {
            modifiers[8] += 100;
        }
    }

    if(players_spell_list[spl_Zombie_Mastery] == sls_Known && _players[player_idx].Globals[ZOMBIE_MASTERY] == 0) modifiers[9] += 100;

    if(modifiers[9] > 400) modifiers[9] = 300;

    if(players_spell_list[spl_Planar_Seal] == sls_Known && _players[player_idx].Globals[PLANAR_SEAL] == 0)
    {
        if(AI_Has_More_Myrror_Cities(player_idx) == 1) modifiers[9] += 50;
    }

    if(_players[player_idx].mana_reserve < 200)
    {
        modifiers[9] = 0;
        modifiers[8] = 0;
    }

    if(players_spell_list[spl_Summoning_Circle] == sls_Known)
    {
        if(AI_SCircle_Reevals[player_idx] < 0) {
            Own_City_Count = 0;
            for(itr = 0; itr < _cities; itr++) {
                if(_CITIES[itr].owner_idx == player_idx)
                {
                    Own_City_Count++;
                }
            }
            if(Own_City_Count > 0)
            {
                modifiers[6] = 100;
            }
        }
    }

    modifiers[0] = 1;

    return Get_Weighted_Choice(modifiers, 11);
}


// WZD o156p04
/**
 * @brief Builds the AI overland-casting spell list and category-availability flags.
 *
 * @details
 * Scans every spell slot across all magic realms for the specified player and
 * keeps only spells marked `sls_Known`. For each known spell, this routine
 * reads `spell_data_table[spell_idx].AI_Group` and filters out combat-only,
 * dispel/lock, scouting, and specific legacy groups that are not intended for
 * overland category selection. Accepted spells are appended to
 * `g_niu_ai_spell_info_list[]`, while `g_ai_spell_group_flags[group]` is set to
 * `ST_TRUE` so downstream pickers can quickly test category presence.
 *
 * Before scanning, the function resets global outputs by clearing
 * `g_niu_ai_spell_info_count` and initializing `g_ai_spell_group_flags[0..SGRP_SoM]`
 * to `ST_FALSE`.
 *
 * @param player_idx Index of the player whose known-spell table is evaluated.
 *
 * @return This function returns no value.
 *
 * @note `spell_idx` is computed as a 1-based global spell id:
 *       `(realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_in_realm + 1`.
 * @warning The function writes to global buffers (`g_niu_ai_spell_info_list`,
 *          `g_ai_spell_group_flags`, `g_niu_ai_spell_info_count`) and assumes
 *          they are sized for all accepted entries.
 */
void AI_Compute_Spells_Info(int16_t player_idx)
{
    int16_t spell_idx = 0;
    int16_t itr = 0;
    int16_t itr_spells = 0;
    int16_t itr_realms = 0;
    int16_t ai_spell_group = 0;

    g_niu_ai_spell_info_count = 0;

    for(itr = 0; itr <= SGRP_SoM ; itr++)
    {
        g_ai_spell_group_flags[itr] = ST_FALSE;
    }

    for(itr_realms = 0; itr_realms < NUM_MAGIC_REALMS; itr_realms++)
    {

        for(itr_spells = 0; itr_spells < NUM_SPELLS_PER_MAGIC_REALM; itr_spells++)
        {

            if(_players[player_idx].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] != sls_Known)
            {
                continue;
            }
            
            spell_idx = ((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells + 1);

            ai_spell_group = spell_data_table[spell_idx].AI_Group;

            /* Filter out combat-specific and special utility spell groups */
            if(ai_spell_group == SGRP_CombatHarm_1)   { continue; }
            if(ai_spell_group == SGRP_CombatCurse)    { continue; }
            if(ai_spell_group == SGRP_CombatHarm_2)   { continue; }
            if(ai_spell_group == SGRP_CombatHarm_3)   { continue; }
            if(ai_spell_group == SGRP_Disrupt)        { continue; }
            if(ai_spell_group == SGRP_CombatMisc)     { continue; }
            if(ai_spell_group == SGRP_Haste)          { continue; }
            if(ai_spell_group == SGRP_AnimateDead)    { continue; }
            if(ai_spell_group == SGRP_CombatSummon_1) { continue; }
            if(ai_spell_group == SGRP_CombatSummon_2) { continue; }
            if(ai_spell_group == SGRP_CombatSummon_3) { continue; }
            if(ai_spell_group == 81) { continue; }
            if(ai_spell_group == SGRP_CE_1)           { continue; }
            if(ai_spell_group == SGRP_CE_2)           { continue; }
            if(ai_spell_group == SGRP_CE_3)           { continue; }
            if(ai_spell_group == SGRP_CE_4)           { continue; }
            if(ai_spell_group == SGRP_Dispel)         { continue; }
            if(ai_spell_group == SGRP_DispelTrue)     { continue; }
            if(ai_spell_group == 43) { continue; }
            if(ai_spell_group == SGRP_SpellLock)      { continue; }
            if(ai_spell_group == SGRP_Scouting)       { continue; }

            g_niu_ai_spell_info_list[g_niu_ai_spell_info_count] = (unsigned char)spell_idx;

            /* Mark this spell category as available for the AI */
            g_ai_spell_group_flags[ai_spell_group] = ST_TRUE;

            g_niu_ai_spell_info_count++;

        }

    }

}


// WZD o156p05
/**
 * @brief Selects an overland summoning spell for the AI player to cast.
 *
 * @details
 * This function evaluates which summoning spell the AI should prioritize based on:
 *   - Known summoning spells in the player's spellbook
 *   - Spell casting costs (normalized by division and squared for non-linear weighting)
 *   - Special unit types (War Bears, Sprites, Giants, Dragons, Demons, etc.)
 *   - Hero summoning capability (via Summon Hero, Conjurer trait, or Channeler trait)
 *   - Special enchantment spells (Resurrection, Guardian Spirit, Magic Spirit, Enchant Item, Create Artifact)
 *   - Current hero count vs. available hero slots
 *
 * The function populates the global `AI_OVL_SplPriorities[]` array with weighted values
 * for each summoning spell, then uses weighted random selection to pick one. Spell costs
 * are squared to apply non-linear scaling, making higher-cost spells disproportionately
 * more likely (with the exception of Storm Giant at index 11, which is not squared due
 * to a historical quirk in the original disassembly).
 *
 * If the AI player has no valid summoning spell available, or if mana constraints prevent
 * casting, the function returns 0 (no spell).
 *
 * @param player_idx Index of the AI-controlled player evaluating summoning options.
 *
 * @return The spell index of the selected summoning spell (e.g., spl_War_Bears, spl_Colossus),
 *         or 0 if no summoning spell should be cast.
 *
 * @note The global array `AI_OVL_SplPriorities[]` is used for weight computation and is
 *       reused across calls; it must be re-initialized at the start of this function.
 * @note Storm Giant (index 11) is intentionally NOT squared due to a quirk in the original
 *       disassembly code; all other summons are squared for non-linear weighting.
 * @note Hero summoning is conditional on trait presence and available hero slots.
 * @note Floating Island summon has additional logic based on transport count and turn number.
 * @warning Some indices (26, 30) in the priority array are reserved or have special handling
 *          that may reflect legacy spell organization.
 */
int16_t AI_Select_Spell_Group_Summon(int16_t player_idx)
{
    int16_t choice = 0;
    int16_t have_the_chosen = 0;
    int16_t unit_idx = 0;
    uint8_t * players_spell_list = NULL;
    int16_t spell_idx = 0;  // DNE in Dasm

    /* Treat spell list as 1-based index by shifting pointer */
    players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

    /* Initialize AI_OVL_SplPriorities array to 0 */
    for(unit_idx = 0; unit_idx < 50; unit_idx++)
    {
        AI_OVL_SplPriorities[unit_idx] = 0;
    }

    if(players_spell_list[spl_War_Bears] == sls_Known) {
        AI_OVL_SplPriorities[1] = spell_data_table[spl_War_Bears].casting_cost / 10;
    }
    if(players_spell_list[spl_Sprites] == sls_Known) {
        AI_OVL_SplPriorities[2] = spell_data_table[spl_Sprites].casting_cost / 10;
    }
    if(players_spell_list[spl_Giant_Spiders] == sls_Known) {
        AI_OVL_SplPriorities[3] = spell_data_table[spl_Giant_Spiders].casting_cost / 10;
    }
    if(players_spell_list[spl_Basilisk] == sls_Known) {
        AI_OVL_SplPriorities[4] = spell_data_table[spl_Basilisk].casting_cost / 10;
    }
    if(players_spell_list[spl_Stone_Giant] == sls_Known) {
        AI_OVL_SplPriorities[5] = spell_data_table[spl_Stone_Giant].casting_cost / 10;
    }
    if(players_spell_list[spl_Gorgons] == sls_Known) {
        AI_OVL_SplPriorities[6] = spell_data_table[spl_Gorgons].casting_cost / 10;
    }
    if(players_spell_list[spl_Behemoth] == sls_Known) {
        AI_OVL_SplPriorities[7] = spell_data_table[spl_Behemoth].casting_cost / 10;
    }
    if(players_spell_list[spl_Colossus] == sls_Known) {
        AI_OVL_SplPriorities[8] = spell_data_table[spl_Colossus].casting_cost / 10;
    }
    if(players_spell_list[spl_Great_Wyrm] == sls_Known) {
        AI_OVL_SplPriorities[9] = spell_data_table[spl_Great_Wyrm].casting_cost / 10;
    }
    if(players_spell_list[spl_Floating_Island] == sls_Known) {
        if(ai_transport_count < 10 && (_turn / 60) > ai_transport_count) {
            AI_OVL_SplPriorities[10] = spell_data_table[spl_Floating_Island].casting_cost / 5;
        }
    }
    if(players_spell_list[spl_Storm_Giant] == sls_Known) {
        AI_OVL_SplPriorities[11] = spell_data_table[spl_Storm_Giant].casting_cost / 10;
    }
    if(players_spell_list[spl_Djinn] == sls_Known) {
        AI_OVL_SplPriorities[12] = spell_data_table[spl_Djinn].casting_cost / 10;
    }
    if(players_spell_list[spl_Sky_Drake] == sls_Known) {
        AI_OVL_SplPriorities[13] = spell_data_table[spl_Sky_Drake].casting_cost / 10;
    }
    if(players_spell_list[spl_Hell_Hounds] == sls_Known) {
        AI_OVL_SplPriorities[14] = spell_data_table[spl_Hell_Hounds].casting_cost / 10;
    }
    if(players_spell_list[spl_Gargoyles] == sls_Known) {
        AI_OVL_SplPriorities[15] = spell_data_table[spl_Gargoyles].casting_cost / 10;
    }
    if(players_spell_list[spl_Doom_Bat] == sls_Known) {
        AI_OVL_SplPriorities[16] = spell_data_table[spl_Doom_Bat].casting_cost / 10;
    }
    if(players_spell_list[spl_Chaos_Spawn] == sls_Known) {
        AI_OVL_SplPriorities[17] = spell_data_table[spl_Chaos_Spawn].casting_cost / 10;
    }
    if(players_spell_list[spl_Fire_Giant] == sls_Known) {
        AI_OVL_SplPriorities[18] = spell_data_table[spl_Fire_Giant].casting_cost / 10;
    }
    if(players_spell_list[spl_Chimeras] == sls_Known) {
        AI_OVL_SplPriorities[19] = spell_data_table[spl_Chimeras].casting_cost / 10;
    }
    if(players_spell_list[spl_Efreet] == sls_Known) {
        AI_OVL_SplPriorities[20] = spell_data_table[spl_Efreet].casting_cost / 10;
    }
    if(players_spell_list[spl_Hydra] == sls_Known) {
        AI_OVL_SplPriorities[21] = spell_data_table[spl_Hydra].casting_cost / 10;
    }
    if(players_spell_list[spl_Great_Drake] == sls_Known) {
        AI_OVL_SplPriorities[22] = spell_data_table[spl_Great_Drake].casting_cost / 10;
    }
    if(players_spell_list[spl_Unicorns] == sls_Known) {
        AI_OVL_SplPriorities[23] = spell_data_table[spl_Unicorns].casting_cost / 10;
    }
    if(players_spell_list[spl_Angel] == sls_Known) {
        AI_OVL_SplPriorities[24] = spell_data_table[spl_Angel].casting_cost / 10;
    }
    if(players_spell_list[spl_Arch_Angel] == sls_Known) {
        AI_OVL_SplPriorities[25] = spell_data_table[spl_Arch_Angel].casting_cost / 10;
    }

    AI_OVL_SplPriorities[26] = 0;

    if(players_spell_list[spl_Skeletons] == sls_Known) {
        AI_OVL_SplPriorities[27] = spell_data_table[spl_Skeletons].casting_cost / 10;
    }
    if(players_spell_list[spl_Ghouls] == sls_Known) {
        AI_OVL_SplPriorities[28] = spell_data_table[spl_Ghouls].casting_cost / 10;
    }
    if(players_spell_list[spl_Night_Stalker] == sls_Known) {
        AI_OVL_SplPriorities[29] = spell_data_table[spl_Night_Stalker].casting_cost / 10;
    }
    /* Note: Index 30 is skipped */
    if(players_spell_list[spl_Wraiths] == sls_Known) {
        AI_OVL_SplPriorities[31] = spell_data_table[spl_Wraiths].casting_cost / 10;
    }
    if(players_spell_list[spl_Shadow_Demons] == sls_Known) {
        AI_OVL_SplPriorities[32] = spell_data_table[spl_Shadow_Demons].casting_cost / 10;
    }
    if(players_spell_list[spl_Death_Knights] == sls_Known) {
        AI_OVL_SplPriorities[33] = spell_data_table[spl_Death_Knights].casting_cost / 10;
    }
    if(players_spell_list[spl_Demon_Lord] == sls_Known) {
        AI_OVL_SplPriorities[34] = spell_data_table[spl_Demon_Lord].casting_cost / 10;
    }

    /* Square weights for non-linear scale choice */
    AI_OVL_SplPriorities[1] = AI_OVL_SplPriorities[1] * AI_OVL_SplPriorities[1];
    AI_OVL_SplPriorities[2] = AI_OVL_SplPriorities[2] * AI_OVL_SplPriorities[2];
    AI_OVL_SplPriorities[3] = AI_OVL_SplPriorities[3] * AI_OVL_SplPriorities[3];
    AI_OVL_SplPriorities[4] = AI_OVL_SplPriorities[4] * AI_OVL_SplPriorities[4];
    AI_OVL_SplPriorities[5] = AI_OVL_SplPriorities[5] * AI_OVL_SplPriorities[5];
    AI_OVL_SplPriorities[6] = AI_OVL_SplPriorities[6] * AI_OVL_SplPriorities[6];
    AI_OVL_SplPriorities[7] = AI_OVL_SplPriorities[7] * AI_OVL_SplPriorities[7];
    AI_OVL_SplPriorities[8] = AI_OVL_SplPriorities[8] * AI_OVL_SplPriorities[8];
    AI_OVL_SplPriorities[9] = AI_OVL_SplPriorities[9] * AI_OVL_SplPriorities[9];
    AI_OVL_SplPriorities[10] = AI_OVL_SplPriorities[10] * AI_OVL_SplPriorities[10];
    /* Note: AI_OVL_SplPriorities[11] (Storm Giant) is NOT squared in the original disassembly */
    AI_OVL_SplPriorities[12] = AI_OVL_SplPriorities[12] * AI_OVL_SplPriorities[12];
    AI_OVL_SplPriorities[13] = AI_OVL_SplPriorities[13] * AI_OVL_SplPriorities[13];
    AI_OVL_SplPriorities[14] = AI_OVL_SplPriorities[14] * AI_OVL_SplPriorities[14];
    AI_OVL_SplPriorities[15] = AI_OVL_SplPriorities[15] * AI_OVL_SplPriorities[15];
    AI_OVL_SplPriorities[16] = AI_OVL_SplPriorities[16] * AI_OVL_SplPriorities[16];
    AI_OVL_SplPriorities[17] = AI_OVL_SplPriorities[17] * AI_OVL_SplPriorities[17];
    AI_OVL_SplPriorities[18] = AI_OVL_SplPriorities[18] * AI_OVL_SplPriorities[18];
    AI_OVL_SplPriorities[19] = AI_OVL_SplPriorities[19] * AI_OVL_SplPriorities[19];
    AI_OVL_SplPriorities[20] = AI_OVL_SplPriorities[20] * AI_OVL_SplPriorities[20];
    AI_OVL_SplPriorities[21] = AI_OVL_SplPriorities[21] * AI_OVL_SplPriorities[21];
    AI_OVL_SplPriorities[22] = AI_OVL_SplPriorities[22] * AI_OVL_SplPriorities[22];
    AI_OVL_SplPriorities[23] = AI_OVL_SplPriorities[23] * AI_OVL_SplPriorities[23];
    AI_OVL_SplPriorities[24] = AI_OVL_SplPriorities[24] * AI_OVL_SplPriorities[24];
    AI_OVL_SplPriorities[25] = AI_OVL_SplPriorities[25] * AI_OVL_SplPriorities[25];
    AI_OVL_SplPriorities[26] = AI_OVL_SplPriorities[26] * AI_OVL_SplPriorities[26];
    AI_OVL_SplPriorities[27] = AI_OVL_SplPriorities[27] * AI_OVL_SplPriorities[27];
    AI_OVL_SplPriorities[28] = AI_OVL_SplPriorities[28] * AI_OVL_SplPriorities[28];
    AI_OVL_SplPriorities[29] = AI_OVL_SplPriorities[29] * AI_OVL_SplPriorities[29];
    /* Note: AI_OVL_SplPriorities[30] is NOT squared */
    AI_OVL_SplPriorities[31] = AI_OVL_SplPriorities[31] * AI_OVL_SplPriorities[31];
    AI_OVL_SplPriorities[32] = AI_OVL_SplPriorities[32] * AI_OVL_SplPriorities[32];
    AI_OVL_SplPriorities[33] = AI_OVL_SplPriorities[33] * AI_OVL_SplPriorities[33];
    AI_OVL_SplPriorities[34] = AI_OVL_SplPriorities[34] * AI_OVL_SplPriorities[34];

    if(players_spell_list[spl_Resurrection] == sls_Known) {
        if(Player_Hero_Count(player_idx) < NUM_HEROES) {
            if(Player_Dead_Hero_Count(player_idx) > 0) {
                AI_OVL_SplPriorities[35] = 100;
            }
        }
    }

    AI_OVL_SplPriorities[36] = 0;
    AI_OVL_SplPriorities[37] = 0;
    if(players_spell_list[spl_Guardian_Spirit] == sls_Known)
    {
        if(AI_NodeOpportunity == ST_TRUE)
        {
            AI_OVL_SplPriorities[36] = 200;
        }
        else
        {
            AI_OVL_SplPriorities[36] = 0;
        }
    }
    else if(players_spell_list[spl_Magic_Spirit] == sls_Known)
    {
        if(AI_NodeOpportunity == ST_TRUE)
        {
            AI_OVL_SplPriorities[37] = 100;
        }
        else
        {
            AI_OVL_SplPriorities[37] = 0;
        }
    }

    if(players_spell_list[spl_Enchant_Item] == sls_Known)
    {
        if(_players[player_idx].artificer != 0)
        {
            AI_OVL_SplPriorities[38] = 30;
        }
        else
        {
            AI_OVL_SplPriorities[38] = 20;
        }
    }

    if(players_spell_list[spl_Create_Artifact] == sls_Known)
    {
        if(_players[player_idx].artificer != 0)
        {
            AI_OVL_SplPriorities[39] = 40;
        }
        else
        {
            AI_OVL_SplPriorities[39] = 30;
        }
    }

    have_the_chosen = ST_FALSE;
    for(unit_idx = 0; unit_idx < _units; unit_idx++)
    {
        if(_UNITS[unit_idx].owner_idx == player_idx && _UNITS[unit_idx].type == ut_BarbSpearmen)  /* OGBUG  should be The Chosen, Barbarian Spearmen */
        {
            have_the_chosen = ST_TRUE;
        }
    }

    if(Player_Hero_Count(player_idx) < NUM_HERO_SLOTS)
    {
        /* OGBUG  test inverted, should be `have_the_chosen != ST_TRUE` */
        if(players_spell_list[spl_Incarnation] == sls_Known && have_the_chosen == ST_TRUE)
        {
            AI_OVL_SplPriorities[40] = 500;
        }
        else if(players_spell_list[spl_Summon_Champion] == sls_Known)
        {
            AI_OVL_SplPriorities[41] = 400;
        }
        else if(players_spell_list[spl_Summon_Hero] == sls_Known)
        {
            AI_OVL_SplPriorities[42] = 200;
        }
    }

    if(players_spell_list[spl_Cockatrices] == sls_Known) {
        AI_OVL_SplPriorities[43] = spell_data_table[spl_Cockatrices].casting_cost / 5;
    }

    /* Apply global spell suppression modifiers */
    if(SPL_IsLifeSupressed() == 1) {
        AI_OVL_SplPriorities[23] = (AI_OVL_SplPriorities[23] * 2) / 3;
        AI_OVL_SplPriorities[24] = (AI_OVL_SplPriorities[24] * 2) / 3;
        AI_OVL_SplPriorities[25] = (AI_OVL_SplPriorities[25] * 2) / 3;
        AI_OVL_SplPriorities[26] = (AI_OVL_SplPriorities[26] * 2) / 3;
        AI_OVL_SplPriorities[35] = (AI_OVL_SplPriorities[35] * 2) / 3;
        AI_OVL_SplPriorities[36] = (AI_OVL_SplPriorities[36] * 2) / 3;
        AI_OVL_SplPriorities[40] = (AI_OVL_SplPriorities[40] * 2) / 3;
    }

    if(CRP_SPL_IsNatSuppressed() == 1) {
        AI_OVL_SplPriorities[1] = (AI_OVL_SplPriorities[1] * 2) / 3;
        AI_OVL_SplPriorities[2] = (AI_OVL_SplPriorities[2] * 2) / 3;
        AI_OVL_SplPriorities[3] = (AI_OVL_SplPriorities[3] * 2) / 3;
        AI_OVL_SplPriorities[4] = (AI_OVL_SplPriorities[4] * 2) / 3;
        AI_OVL_SplPriorities[5] = (AI_OVL_SplPriorities[5] * 2) / 3;
        AI_OVL_SplPriorities[6] = (AI_OVL_SplPriorities[6] * 2) / 3;
        AI_OVL_SplPriorities[7] = (AI_OVL_SplPriorities[7] * 2) / 3;
        AI_OVL_SplPriorities[8] = (AI_OVL_SplPriorities[8] * 2) / 3;
        AI_OVL_SplPriorities[9] = (AI_OVL_SplPriorities[9] * 2) / 3;
        AI_OVL_SplPriorities[10] = (AI_OVL_SplPriorities[10] * 2) / 3;
        AI_OVL_SplPriorities[43] = (AI_OVL_SplPriorities[43] * 2) / 3;
    }

    if(SPL_IsDthSuppressed() == 1) {
        for(unit_idx = 27; unit_idx <= 34; unit_idx++) {
            if(AI_OVL_SplPriorities[unit_idx] < 20) {
                AI_OVL_SplPriorities[unit_idx] = 0;
            } else if(AI_OVL_SplPriorities[unit_idx] < 50) {
                AI_OVL_SplPriorities[unit_idx] = AI_OVL_SplPriorities[unit_idx] / 3;
            } else {
                AI_OVL_SplPriorities[unit_idx] = AI_OVL_SplPriorities[unit_idx] / 2;
            }
        }
    }

    if(SPL_IsChsSuppressed() == 1) {
        for(unit_idx = 14; unit_idx <= 22; unit_idx++) {
            if(AI_OVL_SplPriorities[unit_idx] < 20) {
                AI_OVL_SplPriorities[unit_idx] = 0;
            } else if(AI_OVL_SplPriorities[unit_idx] < 50) {
                AI_OVL_SplPriorities[unit_idx] = AI_OVL_SplPriorities[unit_idx] / 3;
            } else {
                AI_OVL_SplPriorities[unit_idx] = AI_OVL_SplPriorities[unit_idx] / 2;
            }
        }
    }

    choice = Get_Weighted_Choice(AI_OVL_SplPriorities, 50);

    switch(choice - 1)
    {
        case  0: { spell_idx = spl_War_Bears;     } break;
        case  1: { spell_idx = spl_Sprites;       } break;
        case  2: { spell_idx = spl_Giant_Spiders; } break;
        case  3: { spell_idx = spl_Basilisk; } break;
        case  4: { spell_idx = spl_Stone_Giant; } break;
        case  5: { spell_idx = spl_Gorgons; } break;
        case  6: { spell_idx = spl_Behemoth; } break;
        case  7: { spell_idx = spl_Colossus; } break;
        case  8: { spell_idx = spl_Great_Wyrm; } break;
        case  9: { spell_idx = spl_Floating_Island; } break;
        case 10: { spell_idx = spl_Storm_Giant; } break;
        case 11: { spell_idx = spl_Djinn; } break;
        case 12: { spell_idx = spl_Sky_Drake; } break;
        case 13: { spell_idx = spl_Hell_Hounds; } break;
        case 14: { spell_idx = spl_Gargoyles; } break;
        case 15: { spell_idx = spl_Doom_Bat; } break;
        case 16: { spell_idx = spl_Chaos_Spawn; } break;
        case 17: { spell_idx = spl_Fire_Giant; } break;
        case 18: { spell_idx = spl_Chimeras; } break;
        case 19: { spell_idx = spl_Efreet; } break;
        case 20: { spell_idx = spl_Hydra; } break;
        case 21: { spell_idx = spl_Great_Drake; } break;
        case 22: { spell_idx = spl_Unicorns; } break;
        case 23: { spell_idx = spl_Angel; } break;
        case 24: { spell_idx = spl_Arch_Angel; } break;
        case 25: { spell_idx = spl_Skeletons; } break;
        case 26: { spell_idx = spl_Skeletons; } break;
        case 27: { spell_idx = spl_Ghouls; } break;
        case 28: { spell_idx = spl_Night_Stalker; } break;
        case 29: { spell_idx = 0; } break;
        case 30: { spell_idx = spl_Wraiths; } break;
        case 31: { spell_idx = spl_Shadow_Demons; } break;
        case 32: { spell_idx = spl_Death_Knights; } break;
        case 33: { spell_idx = spl_Demon_Lord; } break;
        case 34: { spell_idx = spl_Resurrection; } break;
        case 35: { spell_idx = spl_Guardian_Spirit; } break;
        case 36: { spell_idx = spl_Magic_Spirit; } break;
        case 37: { spell_idx = spl_Enchant_Item; } break;
        case 38: { spell_idx = spl_Create_Artifact; } break;
        case 39: { spell_idx = spl_Incarnation; } break;
        case 40: { spell_idx = spl_Summon_Champion; } break;
        case 41: { spell_idx = spl_Summon_Hero; } break;
        case 42: { spell_idx = spl_Cockatrices; } break;
        default: { spell_idx = 0; } break;
    }

    return spell_idx;
}


// WZD o156p06
int16_t AI_OVL_PickUnitBuff(int16_t player_idx)
{
    return 0;
}

// WZD o156p07
int16_t AI_OVL_PickRealmSupr(int16_t player_idx)
{
    return 0;
}

// WZD o156p08
int16_t AI_OVL_PickGlobal(int16_t player_idx)
{
    return 0;
}

// WZD o156p09
int16_t AI_OVL_PickCurse(int16_t player_idx)
{
    return 0;
}

// WZD o156p10
int16_t AI_OVL_PickCityBuff(int16_t player_idx)
{
    return 0;
}


// WZD o156p11
int16_t SPL_IsLifeSupressed(void)
{
    int16_t itr_players = 0;
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[itr_players].Globals[EVIL_OMENS] != ST_FALSE)
        {
            return ST_TRUE;
        }
    }
    return ST_FALSE;
}

// WZD o156p12
int16_t SPL_IsChsSuppressed(void)
{
    int16_t itr_players = 0;
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[itr_players].Globals[NATURES_WRATH] != ST_FALSE)
        {
            return ST_TRUE;
        }
    }
    return ST_FALSE;
}


// WZD o156p13
int16_t SPL_IsDthSuppressed(void)
{
    int16_t itr_players = 0;
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[itr_players].Globals[LIFE_FORCE] != ST_FALSE)
        {
            return ST_TRUE;
        }
        if(_players[itr_players].Globals[NATURES_WRATH] != ST_FALSE)
        {
            return ST_TRUE;
        }
    }
    return ST_FALSE;
}


// WZD o156p14
int16_t CRP_SPL_IsNatSuppressed(void)
{
    int16_t itr_players = 0;
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[itr_players].Globals[EVIL_OMENS] != ST_FALSE)
        {
            return ST_TRUE;
        }
    }
    return ST_FALSE;
}


// WZD o156p15
int16_t AI_OVL_PickDise(int16_t player_idx)
{
    return 0;
}

// WZD o156p16
int16_t AI_OVL_PickDisj(int16_t player_idx)
{
    return 0;
}

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
        Cast_Spell_Overland()
        Cast_Spell_Overland()

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
                (_ai_all_enemy_city_values[itr_cities] > best_enemy_city_value)
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

                            best_enemy_city_value = _ai_all_enemy_city_values[itr_cities];

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
AI target picker for overland square spells
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
; closest ocean square within 10 tiles of a random city
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

    // ; find the closest ocean square within 10 tiles in any of
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
/**
 * @brief Determines whether the AI player has a Myrror-side city-count advantage over the human player.
 *
 * Counts cities owned by the human player on Arcanus and Myrror, then counts cities owned by the
 * specified AI player on Arcanus and Myrror. The function currently evaluates only the Myrror-side
 * comparison to decide whether a Myrror advantage exists.
 *
 * @param player_idx Index of the AI player being evaluated.
 *
 * @return ST_TRUE if the AI player's Myrror city count is greater than the human player's Myrror city count;
 *         otherwise ST_FALSE.
 *
 * @note human_arcanus_count and player_arcanus_count are collected but not used by the current return condition.
 */
int16_t AI_Has_More_Myrror_Cities(int16_t player_idx)
{
    int16_t myrror_count = 0;
    int16_t human_arcanus_count = 0;
    int16_t human_myrror_count = 0;
    int16_t player_arcanus_count = 0;
    int16_t player_myrror_count = 0;
    int16_t city_idx = 0;

    player_arcanus_count = 0;
    player_myrror_count = 0;

    for(city_idx = 0; city_idx < _cities; city_idx++)
    {
        if(_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
        {
            if(_CITIES[city_idx].wp == ARCANUS_PLANE)
            {
                human_arcanus_count++;
            }
            else
            {
                human_myrror_count++;
            }
        }
        else if(_CITIES[city_idx].owner_idx == player_idx)
        {
            if(_CITIES[city_idx].wp == ARCANUS_PLANE)
            {
                player_arcanus_count++;
            }
            else
            {
                player_myrror_count++;
            }
        }
    }

    if(player_myrror_count > human_myrror_count)
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}


// WZD o156p57
// drake178: TILE_IsOcean()
/*
; returns 1 if the square is a shore, ocean, or lake, or
; 0 otherwise
; differs from TILE_IsAISailable and TILE_IsAISailable2
; in that this also returns 0 for river-outflow single
; square lakes
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
/**
 * @brief Determines whether the player's designated summon city is the same as their fortress city.
 *
 * Iterates through all cities on the map, identifying both the player's fortress city and their
 * designated summon city. Returns true only if both cities exist and are at the same location.
 *
 * @param player_idx Index of the player to evaluate.
 *
 * @return ST_TRUE if the player has both a valid fortress city and summon city, and they are located
 *         at the same coordinates; otherwise ST_FALSE.
 *
 * @note Uses ST_UNDEFINED to indicate that a city was not found. The function compares plane (wp),
 *       x-coordinate (wx), and y-coordinate (wy) to determine city identity.
 */
int16_t Player_Summon_City_Is_Fortress_City(int16_t player_idx)
{
    int16_t summon_city_idx = 0;
    int16_t fortress_city_idx = 0;
    int16_t city_idx = 0;

    fortress_city_idx = ST_UNDEFINED;
    summon_city_idx = ST_UNDEFINED;

    for(city_idx = 0; city_idx < _cities; city_idx++)
    {
        /* Check if city is player's fortress city */
        if(
            (_CITIES[city_idx].wx == _FORTRESSES[player_idx].wx)
            &&
            (_CITIES[city_idx].wy == _FORTRESSES[player_idx].wy)
            &&
            (_CITIES[city_idx].wp == _FORTRESSES[player_idx].wp)
        )
        {
            fortress_city_idx = city_idx;
        }

        /* Check if city is player's summon city */
        if(
            (_CITIES[city_idx].wx == _players[player_idx].summon_wx)
            &&
            (_CITIES[city_idx].wy == _players[player_idx].summon_wy)
            &&
            (_CITIES[city_idx].wp == _players[player_idx].summon_wp)
        )
        {
            summon_city_idx = city_idx;
        }
    }

    if(
        (fortress_city_idx != ST_UNDEFINED)
        &&
        (summon_city_idx != ST_UNDEFINED)
        &&
        (summon_city_idx == fortress_city_idx)
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}
