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
#include "../../MoX/src/special.h"

#include "AIDUDES.h"
#include "CITYCALC.h"
#include "NEXTTURN.h"
#include "SBookScr.h"
#include "Spellbook.h"
#include "SPELLDEF.h"
#include "Terrain.h"
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
        if(_players[player_idx].Prim_Realm == 1 || _players[player_idx].Prim_Realm == sbr_Chaos) { /* Death or Chaos */
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
            spell_idx = AI_Select_Spell_Group_Unit_Enchantment(player_idx);
            break;
        case 3:
            spell_idx = AI_Select_Spell_Group_City_Enchantment(player_idx);
            break;
        case 4:
            spell_idx = AI_Select_Spell_Group_Disenchant(player_idx);
            break;
        case 5:
            spell_idx = AI_Select_Spell_Group_Disjunction(player_idx);
            break;
        case 6:
            spell_idx = spl_Summoning_Circle;
            break;
        case 7:
            spell_idx = AI_Select_Spell_Group_Attack(player_idx);
            break;
        case 8:
            spell_idx = AI_Select_Spell_Group_Suppression(player_idx);
            break;
        case 9:
            spell_idx = AI_Select_Spell_Group_Global(player_idx);
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
  9 - overland enchantment
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
 *   5. Disjunction spells (counters opponent's overland enchantments)
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
 *   - Known spells and active overland enchantments
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
 * @note `modifiers[9]` (overland enchantments) is capped at a maximum value of 300 after
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
    niu_summon_landmass_idx = _landmasses[summon_wp * 960 + _players[player_idx].summon_wy * WORLD_WIDTH + _players[player_idx].summon_wx];
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

        fortress_landmass_idx = _landmasses[_FORTRESSES[player_idx].wp * 960 + _FORTRESSES[player_idx].wy * WORLD_WIDTH + _FORTRESSES[player_idx].wx];

        for(itr = 0; itr < _units; itr++)
        {
            if(_UNITS[itr].owner_idx == 0)
            {
                if(((unsigned int *)&_UNITS[itr].enchantments)[0] != 0 || ((unsigned int *)&_UNITS[itr].enchantments)[1] != 0)
                {
                    if(_landmasses[_UNITS[itr].wp * 960 + _UNITS[itr].wy * WORLD_WIDTH + _UNITS[itr].wx] == fortress_landmass_idx)
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
            if(_players[itr].Globals[METEOR_STORMS] != 0) modifiers[5] += 100;
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
        _players[player_idx].cp_target_3 = Get_Weighted_Choice(&Opponent_Weights[0], _cp_hostile_opponent_count);
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
    if(players_spell_list[spl_Meteor_Storms] == sls_Known && _players[player_idx].Globals[METEOR_STORMS] == 0) modifiers[9] += 100;
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
        if(_ai_reevaluate_summoning_circle_countdown[player_idx] < 0) {
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

    return Get_Weighted_Choice(&modifiers[0], 11);
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
        if(ai_transport_count < 10 && (_turn / WORLD_WIDTH) > ai_transport_count) {
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
    if(SPL_IsLifeSupressed() == ST_TRUE) {
        AI_OVL_SplPriorities[23] = (AI_OVL_SplPriorities[23] * 2) / 3;
        AI_OVL_SplPriorities[24] = (AI_OVL_SplPriorities[24] * 2) / 3;
        AI_OVL_SplPriorities[25] = (AI_OVL_SplPriorities[25] * 2) / 3;
        AI_OVL_SplPriorities[26] = (AI_OVL_SplPriorities[26] * 2) / 3;
        AI_OVL_SplPriorities[35] = (AI_OVL_SplPriorities[35] * 2) / 3;
        AI_OVL_SplPriorities[36] = (AI_OVL_SplPriorities[36] * 2) / 3;
        AI_OVL_SplPriorities[40] = (AI_OVL_SplPriorities[40] * 2) / 3;
    }

    if(CRP_SPL_IsNatSuppressed() == ST_TRUE) {
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

    if(SPL_IsDthSuppressed() == ST_TRUE) {
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

    if(SPL_IsChsSuppressed() == ST_TRUE) {
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

    choice = Get_Weighted_Choice(&AI_OVL_SplPriorities[0], 50);

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
int16_t AI_Select_Spell_Group_Unit_Enchantment(int16_t player_idx)
{
    int16_t choice = 0;
    uint8_t * players_spell_list = NULL;
    int16_t itr = 0;

    /* Treat spell list as 1-based index by shifting pointer */
    players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

    for(itr = 0; itr < 50; itr++) {
        AI_OVL_SplPriorities[itr] = 0;
    }

    if(players_spell_list[spl_Resist_Elements] == sls_Known) {
        AI_OVL_SplPriorities[1] = 5;
    }
    if(players_spell_list[spl_Stone_Skin] == sls_Known) {
        AI_OVL_SplPriorities[2] = 15;
    }
    if(players_spell_list[spl_Elemental_Armor] == sls_Known) {
        AI_OVL_SplPriorities[3] = 5;
    }
    if(players_spell_list[spl_Iron_Skin] == sls_Known) {
        AI_OVL_SplPriorities[4] = 30;
    }
    if(players_spell_list[spl_Regeneration] == sls_Known) {
        AI_OVL_SplPriorities[5] = 10;
    }
    if(players_spell_list[spl_Resist_Magic] == sls_Known) {
        AI_OVL_SplPriorities[6] = 5;
    }
    if(players_spell_list[spl_Blur] == sls_Known) {
        AI_OVL_SplPriorities[7] = 0;
    }
    if(players_spell_list[spl_Magic_Immunity] == sls_Known) {
        AI_OVL_SplPriorities[9] = 20;
    }
    if(players_spell_list[spl_Flame_Blade] == sls_Known) {
        AI_OVL_SplPriorities[10] = 30;
    }
    if(players_spell_list[spl_Eldritch_Weapon] == sls_Known) {
        AI_OVL_SplPriorities[11] = 15;
    }
    if(players_spell_list[spl_Immolation] == sls_Known) {
        AI_OVL_SplPriorities[12] = 10;
    }
    if(players_spell_list[spl_Holy_Weapon] == sls_Known) {
        AI_OVL_SplPriorities[13] = 10;
    }
    if(players_spell_list[spl_Bless] == sls_Known) {
        AI_OVL_SplPriorities[14] = 5;
    }
    if(players_spell_list[spl_Holy_Armor] == sls_Known) {
        AI_OVL_SplPriorities[15] = 10;
    }
    if(players_spell_list[spl_Heroism] == sls_Known) {
        AI_OVL_SplPriorities[16] = 10;
    }
    if(players_spell_list[spl_True_Sight] == sls_Known) {
        AI_OVL_SplPriorities[17] = 5;
    }
    if(players_spell_list[spl_Invulnerability] == sls_Known) {
        AI_OVL_SplPriorities[18] = 20;
    }
    if(players_spell_list[spl_Lionheart] == sls_Known) {
        AI_OVL_SplPriorities[19] = 30;
    }
    if(players_spell_list[spl_Righteousness] == sls_Known) {
        AI_OVL_SplPriorities[20] = 5;
    }
    if(players_spell_list[spl_Cloak_Of_Fear] == sls_Known) {
        AI_OVL_SplPriorities[21] = 10;
    }
    if(players_spell_list[spl_Wraith_Form] == sls_Known) {
        AI_OVL_SplPriorities[23] = 5;
    }
    if(players_spell_list[spl_Water_Walking] == sls_Known) {
        AI_OVL_SplPriorities[24] = 30;
    }
    if(players_spell_list[spl_Path_Finding] == sls_Known) {
        AI_OVL_SplPriorities[25] = 5;
    }
    if(players_spell_list[spl_Entangle] == sls_Known) {
        AI_OVL_SplPriorities[26] = 0;
    }
    if(players_spell_list[spl_Flight] == sls_Known) {
        AI_OVL_SplPriorities[27] = 25;
    }
    if(players_spell_list[spl_Wind_Walking] == sls_Known) {
        AI_OVL_SplPriorities[28] = 50;
    }
    if(players_spell_list[spl_Endurance] == sls_Known) {
        AI_OVL_SplPriorities[29] = 10;
    }
    /* OGBUG: fails to check all wizards, should iter over _num_players */
    if(players_spell_list[spl_Plane_Shift] == sls_Known &&
        _players[0].Globals[PLANAR_SEAL] == 0 &&
        _players[1].Globals[PLANAR_SEAL] == 0 &&
        _players[2].Globals[PLANAR_SEAL] == 0 &&
        _players[3].Globals[PLANAR_SEAL] == 0) {
        AI_OVL_SplPriorities[30] = 20;
    }
    if(players_spell_list[spl_Planar_Travel] == sls_Known) {
        AI_OVL_SplPriorities[31] = 5;
    }
    if(players_spell_list[spl_Natures_Cures] == sls_Known) {
        AI_OVL_SplPriorities[32] = 5;
    }
    if(players_spell_list[spl_Enchant_Road] == sls_Known) {
        AI_OVL_SplPriorities[33] = 10;
    }
    if(players_spell_list[spl_Spell_Lock] == sls_Known) {
        AI_OVL_SplPriorities[34] = 10;
    }
    if(players_spell_list[spl_Invisibility] == sls_Known) {
        AI_OVL_SplPriorities[35] = 40;
    }
    if(players_spell_list[spl_Chaos_Channels] == sls_Known &&
        _players[player_idx].Globals[DOOM_MASTERY] == 0) {
        AI_OVL_SplPriorities[36] = 25;
    }
    if(players_spell_list[spl_Lycanthropy] == sls_Known &&
        _players[player_idx].Globals[ZOMBIE_MASTERY] == 0) {
        AI_OVL_SplPriorities[37] = 25;
    }
    if(players_spell_list[spl_Black_Channels] == sls_Known) {
        AI_OVL_SplPriorities[38] = 25;
    }
    if(players_spell_list[spl_Guardian_Wind] == sls_Known) {
        AI_OVL_SplPriorities[39] = 10;
    }
    if(players_spell_list[spl_Giant_Strength] == sls_Known) {
        AI_OVL_SplPriorities[40] = 10;
    }

    if(SPL_IsLifeSupressed() == ST_TRUE) {
        for(itr = 13; itr <= 20; itr++) {
            AI_OVL_SplPriorities[itr] = (AI_OVL_SplPriorities[itr] * 2) / 3;
        }
        AI_OVL_SplPriorities[29] = (AI_OVL_SplPriorities[29] * 2) / 3; /* spl_Endurance */
        AI_OVL_SplPriorities[30] = (AI_OVL_SplPriorities[30] * 2) / 3; /* spl_Plane_Shift */
        AI_OVL_SplPriorities[31] = (AI_OVL_SplPriorities[31] * 2) / 3; /* spl_Planar_Travel */
    }

    if(CRP_SPL_IsNatSuppressed() == ST_TRUE) {
        for(itr = 1; itr <= 5; itr++) {
            AI_OVL_SplPriorities[itr] = (AI_OVL_SplPriorities[itr] * 2) / 3;
        }
        AI_OVL_SplPriorities[24] = (AI_OVL_SplPriorities[24] * 2) / 3; /* spl_Water_Walking */
        AI_OVL_SplPriorities[25] = (AI_OVL_SplPriorities[25] * 2) / 3; /* spl_Path_Finding */
        AI_OVL_SplPriorities[26] = (AI_OVL_SplPriorities[26] * 2) / 3; /* spl_Entangle */
        AI_OVL_SplPriorities[32] = (AI_OVL_SplPriorities[32] * 2) / 3; /* spl_Natures_Cures */
        AI_OVL_SplPriorities[40] = AI_OVL_SplPriorities[40] / 2;       /* spl_Giant_Strength */
    }

    if(SPL_IsDthSuppressed() == ST_TRUE) {
        for(itr = 21; itr <= 23; itr++) {
            if(AI_OVL_SplPriorities[itr] < 20) {
                AI_OVL_SplPriorities[itr] = 0;
            } else if(AI_OVL_SplPriorities[itr] < 50) {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 3;
            } else {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 2;
            }
        }
        /* index 37 (spl_Lycanthropy) */
        if(AI_OVL_SplPriorities[37] < 20) {
            AI_OVL_SplPriorities[37] = 0;
        } else if(AI_OVL_SplPriorities[37] < 50) {
            AI_OVL_SplPriorities[37] = AI_OVL_SplPriorities[37] / 3;
        } else {
            AI_OVL_SplPriorities[37] = AI_OVL_SplPriorities[37] / 2;
        }
        /* index 38 (spl_Black_Channels) */
        if(AI_OVL_SplPriorities[38] < 20) {
            AI_OVL_SplPriorities[38] = 0;
        } else if(AI_OVL_SplPriorities[38] < 50) {
            AI_OVL_SplPriorities[38] = AI_OVL_SplPriorities[38] / 3;
        } else {
            AI_OVL_SplPriorities[38] = AI_OVL_SplPriorities[38] / 2;
        }
    }

    if(SPL_IsChsSuppressed() == ST_TRUE) {
        for(itr = 10; itr <= 12; itr++) {
            if(AI_OVL_SplPriorities[itr] < 20) {
                AI_OVL_SplPriorities[itr] = 0;
            } else if(AI_OVL_SplPriorities[itr] < 50) {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 3;
            } else {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 2;
            }
        }
        /* index 36 (spl_Chaos_Channels) */
        if(AI_OVL_SplPriorities[36] < 20) {
            AI_OVL_SplPriorities[36] = 0;
        } else if(AI_OVL_SplPriorities[36] < 50) {
            AI_OVL_SplPriorities[36] = AI_OVL_SplPriorities[36] / 3;
        } else {
            AI_OVL_SplPriorities[36] = AI_OVL_SplPriorities[36] / 2;
        }
    }

    choice = Get_Weighted_Choice(&AI_OVL_SplPriorities[0], 50);

    switch(choice)
    {
        case 1:
            return spl_Resist_Elements;
        case 2:
            return spl_Stone_Skin;
        case 3:
            return spl_Elemental_Armor;
        case 4:
            return spl_Iron_Skin;
        case 5:
            return spl_Regeneration;
        case 6:
            return spl_Resist_Magic;
        case 7:
            return 0;
        case 8:
            return 0;
        case 9:
            return spl_Magic_Immunity;
        case 10:
            return spl_Flame_Blade;
        case 11:
            return spl_Eldritch_Weapon;
        case 12:
            return spl_Immolation;
        case 13:
            return spl_Holy_Weapon;
        case 14:
            return spl_Bless;
        case 15:
            return spl_Holy_Armor;
        case 16:
            return spl_Heroism;
        case 17:
            return spl_True_Sight;
        case 18:
            return spl_Invulnerability;
        case 19:
            return spl_Lionheart;
        case 20:
            return spl_Righteousness;
        case 21:
            return spl_Cloak_Of_Fear;
        case 22:
            return 0;
        case 23:
            return spl_Wraith_Form;
        case 24:
            return spl_Water_Walking;
        case 25:
            return spl_Path_Finding;
        case 26:
            return spl_Entangle;
        case 27:
            return spl_Flight;
        case 28:
            return spl_Wind_Walking;
        case 29:
            return spl_Endurance;
        case 30:
            return spl_Plane_Shift;
        case 31:
            return spl_Planar_Travel;
        case 32:
            return spl_Natures_Cures;
        case 33:
            return spl_Enchant_Road;
        case 34:
            return spl_Spell_Lock;
        case 35:
            return spl_Invisibility;
        case 36:
            return spl_Chaos_Channels;
        case 37:
            return spl_Lycanthropy;
        case 38:
            return spl_Black_Channels;
        case 39:
            return spl_Guardian_Wind;
        case 40:
            return spl_Giant_Strength;
        default:
            return 0;
    }

}

// WZD o156p07
int16_t AI_Select_Spell_Group_Suppression(int16_t player_idx)
{
    int16_t choice = 0;
    int16_t itr = 0;
    uint8_t * players_spell_list = NULL;

    /* Treat spell list as 1-based index by shifting pointer */
    players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

    for(itr = 0; itr < 50; itr++)
    {
        AI_OVL_SplPriorities[itr] = 0;
    }

    if(players_spell_list[spl_Natures_Wrath] == sls_Known)
    {
        AI_OVL_SplPriorities[1] = spell_data_table[spl_Natures_Wrath].casting_cost / 10;
    }

    if(players_spell_list[spl_Life_Force] == sls_Known)
    {
        AI_OVL_SplPriorities[2] = spell_data_table[spl_Life_Force].casting_cost / 10;
    }

    if(players_spell_list[spl_Tranquility] == sls_Known)
    {
        AI_OVL_SplPriorities[3] = spell_data_table[spl_Tranquility].casting_cost / 10;
    }

    if(players_spell_list[spl_Evil_Omens] == sls_Known)
    {
        AI_OVL_SplPriorities[4] = spell_data_table[spl_Evil_Omens].casting_cost / 10;
    }

    if(SPL_IsLifeSupressed() == ST_TRUE)
    {
        for(itr = 2; itr <= 3; itr++)
        {
            AI_OVL_SplPriorities[itr] = (AI_OVL_SplPriorities[itr] * 2) / 3;
        }
    }

    if(CRP_SPL_IsNatSuppressed() == ST_TRUE)
    {
        for(itr = 1; itr <= 1; itr++)
        {
            AI_OVL_SplPriorities[itr] = (AI_OVL_SplPriorities[itr] * 2) / 3;
        }
    }

    if(SPL_IsDthSuppressed() == ST_TRUE)
    {
        for(itr = 4; itr <= 4; itr++)
        {
            if(AI_OVL_SplPriorities[itr] < 20)
            {
                AI_OVL_SplPriorities[itr] = 0;
            }
            else if(AI_OVL_SplPriorities[itr] < 50)
            {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 3;
            }
            else
            {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 2;
            }
        }
    }

    /* Perform weighted selection */
    choice = Get_Weighted_Choice(&AI_OVL_SplPriorities[0], 50);

    /* Map selection back to spell identifier */
    switch(choice)
    {
        case 1:
            return spl_Natures_Wrath;
        case 2:
            return spl_Life_Force;
        case 3:
            return spl_Tranquility;
        case 4:
            return spl_Evil_Omens;
        default:
            return 0;
    }

}

// WZD o156p08
int16_t AI_Select_Spell_Group_Global(int16_t player_idx)
{
    int16_t choice = 0;
    uint8_t * players_spell_list = NULL;
    int16_t itr = 0;

    /* Treat spell list as 1-based index by shifting pointer */
    players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

    for(itr = 0; itr < 50; itr++)
    {
        AI_OVL_SplPriorities[itr] = 0;
    }

    if(players_spell_list[spl_Herb_Mastery] == sls_Known && _players[player_idx].Globals[HERB_MASTERY] == 0)
    {
        AI_OVL_SplPriorities[1] = spell_data_table[spl_Herb_Mastery].casting_cost / 10;
    }

    if(players_spell_list[spl_Wind_Mastery] == sls_Known && _players[player_idx].Globals[WIND_MASTERY] == 0)
    {
        AI_OVL_SplPriorities[2] = spell_data_table[spl_Wind_Mastery].casting_cost / 10;
    }

    if(players_spell_list[spl_Aura_Of_Majesty] == sls_Known && _players[player_idx].Globals[AURA_OF_MAJESTY] == 0)
    {
        AI_OVL_SplPriorities[3] = spell_data_table[spl_Aura_Of_Majesty].casting_cost / 10;
    }

    if(players_spell_list[spl_Great_Unsummoning] == sls_Known)
    {
        AI_OVL_SplPriorities[4] = spell_data_table[spl_Great_Unsummoning].casting_cost / 10;
    }

    if(players_spell_list[spl_Suppress_Magic] == sls_Known && _players[player_idx].Globals[SUPPRESS_MAGIC] == 0)
    {
        AI_OVL_SplPriorities[5] = spell_data_table[spl_Suppress_Magic].casting_cost / 10;
    }

    if(players_spell_list[spl_Spell_Binding] == sls_Known)
    {
        AI_OVL_SplPriorities[6] = spell_data_table[spl_Spell_Binding].casting_cost / 10;
    }

    if(players_spell_list[spl_Great_Wasting] == sls_Known && _players[player_idx].Globals[GREAT_WASTING] == 0)
    {
        AI_OVL_SplPriorities[7] = spell_data_table[spl_Great_Wasting].casting_cost / 10;
    }

    if(players_spell_list[spl_Chaos_Surge] == sls_Known && _players[player_idx].Globals[CHAOS_SURGE] == 0)
    {
        AI_OVL_SplPriorities[8] = spell_data_table[spl_Chaos_Surge].casting_cost / 10;
    }

    if(players_spell_list[spl_Doom_Mastery] == sls_Known && _players[player_idx].Globals[DOOM_MASTERY] == 0)
    {
        AI_OVL_SplPriorities[9] = spell_data_table[spl_Doom_Mastery].casting_cost / 10;
    }

    if(players_spell_list[spl_Meteor_Storms] == sls_Known && _players[player_idx].Globals[METEOR_STORMS] == 0)
    {
        AI_OVL_SplPriorities[10] = spell_data_table[spl_Meteor_Storms].casting_cost / 10;
    }

    if(players_spell_list[spl_Armageddon] == sls_Known && _players[player_idx].Globals[ARMAGEDDON] == 0)
    {
        AI_OVL_SplPriorities[11] = spell_data_table[spl_Armageddon].casting_cost / 10;
    }

    if(players_spell_list[spl_Just_Cause] == sls_Known && _players[player_idx].Globals[JUST_CAUSE] == 0)
    {
        AI_OVL_SplPriorities[12] = spell_data_table[spl_Just_Cause].casting_cost / 10;
    }

    if(players_spell_list[spl_Planar_Seal] == sls_Known &&
        _players[player_idx].Globals[PLANAR_SEAL] == 0 &&
        AI_Has_More_Myrror_Cities(player_idx) != 0)
    {
        AI_OVL_SplPriorities[13] = spell_data_table[spl_Planar_Seal].casting_cost / 20;
    }

    if(players_spell_list[spl_Holy_Arms] == sls_Known && _players[player_idx].Globals[HOLY_ARMS] == 0)
    {
        AI_OVL_SplPriorities[14] = spell_data_table[spl_Holy_Arms].casting_cost / 10;
    }

    if(players_spell_list[spl_Charm_Of_Life] == sls_Known && _players[player_idx].Globals[CHARM_OF_LIFE] == 0)
    {
        AI_OVL_SplPriorities[15] = spell_data_table[spl_Charm_Of_Life].casting_cost / 10;
    }

    if(players_spell_list[spl_Crusade] == sls_Known && _players[player_idx].Globals[CRUSADE] == 0)
    {
        AI_OVL_SplPriorities[16] = spell_data_table[spl_Crusade].casting_cost / 10;
    }

    if(players_spell_list[spl_Eternal_Night] == sls_Known && _players[player_idx].Globals[ETERNAL_NIGHT] == 0)
    {
        AI_OVL_SplPriorities[17] = spell_data_table[spl_Eternal_Night].casting_cost / 10;
    }

    if(players_spell_list[spl_Death_Wish] == sls_Known)
    {
        AI_OVL_SplPriorities[18] = spell_data_table[spl_Death_Wish].casting_cost / 10;
    }

    if(players_spell_list[spl_Zombie_Mastery] == sls_Known && _players[player_idx].Globals[ZOMBIE_MASTERY] == 0)
    {
        AI_OVL_SplPriorities[19] = spell_data_table[spl_Zombie_Mastery].casting_cost / 10;
    }

    if(players_spell_list[spl_Awareness] == sls_Known && _players[player_idx].Globals[AWARENESS] == 0)
    {
        AI_OVL_SplPriorities[20] = spell_data_table[spl_Awareness].casting_cost / 10;
    }

    if(SPL_IsLifeSupressed() == 1)
    {
        for(itr = 12; itr <= 16; itr++)
        {
            AI_OVL_SplPriorities[itr] = (AI_OVL_SplPriorities[itr] * 2) / 3;
        }
    }

    if(CRP_SPL_IsNatSuppressed() == 1)
    {
        for(itr = 1; itr <= 1; itr++)
        {
            AI_OVL_SplPriorities[itr] = (AI_OVL_SplPriorities[itr] * 2) / 3;
        }
    }

    if(SPL_IsDthSuppressed() == 1)
    {
        for(itr = 17; itr <= 19; itr++)
        {
            if(AI_OVL_SplPriorities[itr] < 20)
            {
                AI_OVL_SplPriorities[itr] = 0;
            }
            else if(AI_OVL_SplPriorities[itr] < 50)
            {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 3;
            }
            else
            {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 2;
            }
        }
    }

    if(SPL_IsChsSuppressed() == 1)
    {
        for(itr = 7; itr <= 11; itr++)
        {
            if(AI_OVL_SplPriorities[itr] < 20)
            {
                AI_OVL_SplPriorities[itr] = 0;
            }
            else if(AI_OVL_SplPriorities[itr] < 50)
            {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 3;
            }
            else
            {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 2;
            }
        }
    }

    choice = Get_Weighted_Choice(AI_OVL_SplPriorities, 50);

    switch (choice)
    {
        case 1:
            return spl_Herb_Mastery;
        case 2:
            return spl_Wind_Mastery;
        case 3:
            return spl_Aura_Of_Majesty;
        case 4:
            return spl_Great_Unsummoning;
        case 5:
            return spl_Suppress_Magic;
        case 6:
            return spl_Spell_Binding;
        case 7:
            return spl_Great_Wasting;
        case 8:
            return spl_Chaos_Surge;
        case 9:
            return spl_Doom_Mastery;
        case 10:
            return spl_Meteor_Storms;
        case 11:
            return spl_Armageddon;
        case 12:
            return spl_Just_Cause;
        case 13:
            return spl_Planar_Seal;
        case 14:
            return spl_Holy_Arms;
        case 15:
            return spl_Charm_Of_Life;
        case 16:
            return spl_Crusade;
        case 17:
            return spl_Eternal_Night;
        case 18:
            return spl_Death_Wish;
        case 19:
            return spl_Zombie_Mastery;
        case 20:
            return spl_Awareness;
        default:
            return spl_NONE;
    }

}

// WZD o156p09
/**
 * @brief Selects an overland attack spell for the AI player to cast.
 *
 * @details
 * Evaluates known attack spells available to the AI player and assigns weighted
 * priorities based on spell casting costs. The function evaluates 19 overland
 * offensive spells organized into three primary categories:
 *
 * **Direct Damage Spells** (nature/chaos realms):
 *   - Ice Storm, Earthquake, Fire Storm, Black Wind (cost / 10)
 *   - Stasis (cost / 5)
 *
 * **Mana/Special Spells** (universal):
 *   - Spell Blast (cost / 2)
 *   - Time Stop (cost / 10, requires 3000+ mana reserve)
 *   - Chaos Rift (cost / 3), Call The Void (cost / 2)
 *   - Warp Node (cost / 3)
 *   - Drain Power (cost / 5)
 *
 * **Corruption/City-Targeting Spells** (death realm):
 *   - Corruption (cost / 5), Raise Volcano (cost / 10)
 *   - Famine, Pestilence, Cursed Lands, Cruel Unminding (cost / 5)
 *   - Subversion, Evil Presence (cost / 10 or 5, cost-dependent)
 *
 * Priority weights are derived from spell casting costs via division (lower divisor
 * = higher weight). City-targeting spells are nullified if the player has no
 * targetable cities. Special conditions apply: Time Stop requires sufficient mana
 * reserve (3000+), and Raise Volcano is blocked if ARMAGEDDON global is active.
 *
 * Realm suppression modifiers reduce weights:
 *   - Nature suppression: Ice Storm and Earthquake (2/3 reduction)
 *   - Death suppression: Famine through Cruel Unminding (tiered: 0/<20, /3/<50, /2/>=50)
 *   - Chaos suppression: Fire Storm through Call The Void (same tiered reductions)
 *
 * Uses weighted random selection to pick the final spell, then maps the choice
 * index to a concrete spell via switch statement. Note: priority index 9 is
 * skipped; indices jump from 8 to 10.
 *
 * @param player_idx Index of the AI-controlled player evaluating attack options.
 *
 * @return int16_t Spell index of the selected attack spell (e.g., spl_Ice_Storm,
 *                  spl_Earthquake, spl_Time_Stop), or spl_NONE if no attack
 *                  spell is available or all priorities are zero.
 *
 * @note The global array `AI_OVL_SplPriorities[]` is reinitialized on each call
 *       and contains weighted entries indexed by spell category (1–20, skipping 9).
 * @note City-targeting spells are disabled entirely if AI_Has_Targetable_City()
 *       returns false; this affects 10 spells: Evil Presence, Cursed Lands,
 *       Pestilence, Famine, Earthquake, Chaos Rift, Corruption, Raise Volcano,
 *       Call The Void, and Warp Node.
 * @note Time Stop is only available if mana_reserve >= 3000; without sufficient
 *       mana, its priority remains 0.
 * @note Raise Volcano is only available if _players[player_idx].Globals[ARMAGEDDON] == 0;
 *       if ARMAGEDDON is active, Raise Volcano's priority remains 0.
 * @note Death suppression (tiered reduction) applies only to spells with priority >= 1;
 *       spells with priority < 20 are zeroed, 20-49 are divided by 3, >= 50 divided by 2.
 * @note Chaos suppression uses the same tiered reduction pattern as Death suppression
 *       for Fire Storm through Call The Void (indices 7–11).
 * @warning The switch statement includes a gap at index 9 (no case 9), reflecting
 *          the original jump-table structure in the disassembly.
 */
int16_t AI_Select_Spell_Group_Attack(int16_t player_idx)
{
    int16_t choice = 0;
    uint8_t * players_spell_list = NULL;
    int16_t itr = 0;

    /* Treat spell list as 1-based index by shifting pointer */
    players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

    for(itr = 0; itr < 50; itr++)
    {
        AI_OVL_SplPriorities[itr] = 0;
    }

    /* 1. Ice Storm */
    if(players_spell_list[spl_Ice_Storm] == sls_Known)
    {
        AI_OVL_SplPriorities[1] = spell_data_table[spl_Ice_Storm].casting_cost / 10;
    }

    /* 2. Earthquake */
    if(players_spell_list[spl_Earthquake] == sls_Known)
    {
        AI_OVL_SplPriorities[2] = spell_data_table[spl_Earthquake].casting_cost / 10;
    }

    /* 3. Spell Blast */
    if(players_spell_list[spl_Spell_Blast] == sls_Known)
    {
        AI_OVL_SplPriorities[3] = spell_data_table[spl_Spell_Blast].casting_cost / 2;
    }

    /* 4. Stasis */
    if(players_spell_list[spl_Stasis] == sls_Known)
    {
        AI_OVL_SplPriorities[4] = spell_data_table[spl_Stasis].casting_cost / 5;
    }

    /* 5. Time Stop */
    if(players_spell_list[spl_Time_Stop] == sls_Known)
    {
        if(_players[player_idx].mana_reserve >= 3000)
        {
            AI_OVL_SplPriorities[5] = spell_data_table[spl_Time_Stop].casting_cost / 10;
        }
    }

    /* 6. Corruption */
    if(players_spell_list[spl_Corruption] == sls_Known)
    {
        AI_OVL_SplPriorities[6] = spell_data_table[spl_Corruption].casting_cost / 5;
    }

    /* 7. Fire Storm */
    if(players_spell_list[spl_Fire_Storm] == sls_Known)
    {
        AI_OVL_SplPriorities[7] = spell_data_table[spl_Fire_Storm].casting_cost / 10;
    }

    /* 8. Raise Volcano */
    if(players_spell_list[spl_Raise_Volcano] == sls_Known)
    {
        if(_players[player_idx].Globals[ARMAGEDDON] == 0)
        {
            AI_OVL_SplPriorities[8] = spell_data_table[spl_Raise_Volcano].casting_cost / 10;
        }
    }

    /* 10. Chaos Rift */
    if(players_spell_list[spl_Chaos_Rift] == sls_Known)
    {
        AI_OVL_SplPriorities[10] = spell_data_table[spl_Chaos_Rift].casting_cost / 3;
    }

    /* 11. Call The Void */
    if(players_spell_list[spl_Call_The_Void] == sls_Known)
    {
        AI_OVL_SplPriorities[11] = spell_data_table[spl_Call_The_Void].casting_cost / 2;
    }

    /* 12. Famine */
    if(players_spell_list[spl_Famine] == sls_Known)
    {
        AI_OVL_SplPriorities[12] = spell_data_table[spl_Famine].casting_cost / 5;
    }

    /* 13. Warp Node */
    if(players_spell_list[spl_Warp_Node] == sls_Known)
    {
        AI_OVL_SplPriorities[13] = spell_data_table[spl_Warp_Node].casting_cost / 3;
    }

    /* 14. Black Wind */
    if(players_spell_list[spl_Black_Wind] == sls_Known)
    {
        AI_OVL_SplPriorities[14] = spell_data_table[spl_Black_Wind].casting_cost / 10;
    }

    /* 15. Drain Power */
    if(players_spell_list[spl_Drain_Power] == sls_Known)
    {
        AI_OVL_SplPriorities[15] = spell_data_table[spl_Drain_Power].casting_cost / 5;
    }

    /* 16. Evil Presence */
    if(players_spell_list[spl_Evil_Presence] == sls_Known)
    {
        AI_OVL_SplPriorities[16] = spell_data_table[spl_Evil_Presence].casting_cost / 5;
    }

    /* 17. Subversion */
    if(players_spell_list[spl_Subversion] == sls_Known)
    {
        AI_OVL_SplPriorities[17] = spell_data_table[spl_Subversion].casting_cost / 10;
    }

    /* 18. Pestilence */
    if(players_spell_list[spl_Pestilence] == sls_Known)
    {
        AI_OVL_SplPriorities[18] = spell_data_table[spl_Pestilence].casting_cost / 2;
    }

    /* 19. Cursed Lands */
    if(players_spell_list[spl_Cursed_Lands] == sls_Known)
    {
        AI_OVL_SplPriorities[19] = spell_data_table[spl_Cursed_Lands].casting_cost / 5;
    }

    /* 20. Cruel Unminding */
    if(players_spell_list[spl_Cruel_Unminding] == sls_Known)
    {
        AI_OVL_SplPriorities[20] = spell_data_table[spl_Cruel_Unminding].casting_cost / 5;
    }

    /* Validate if player is capable of targeting cities */
    if(AI_Has_Targetable_City(player_idx) == 0)
    {
        AI_OVL_SplPriorities[16] = 0;
        AI_OVL_SplPriorities[19] = 0;
        AI_OVL_SplPriorities[18] = 0;
        AI_OVL_SplPriorities[12] = 0;
        AI_OVL_SplPriorities[2] = 0;
        AI_OVL_SplPriorities[10] = 0;
        AI_OVL_SplPriorities[6] = 0;
        AI_OVL_SplPriorities[8] = 0;
        AI_OVL_SplPriorities[11] = 0;
        AI_OVL_SplPriorities[13] = 0;
    }

    /* Apply suppressions to realm-specific curse selections */
    if(CRP_SPL_IsNatSuppressed() == ST_TRUE)
    {
        for(itr = 1; itr <= 2; itr++)
        {
            AI_OVL_SplPriorities[itr] = (AI_OVL_SplPriorities[itr] * 2) / 3;
        }
    }

    if(SPL_IsDthSuppressed() == ST_TRUE)
    {
        for(itr = 12; itr <= 20; itr++)
        {
            if(AI_OVL_SplPriorities[itr] < 20)
            {
                AI_OVL_SplPriorities[itr] = 0;
            }
            else if(AI_OVL_SplPriorities[itr] < 50)
            {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 3;
            }
            else
            {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 2;
            }
        }
    }

    if(SPL_IsChsSuppressed() == ST_TRUE)
    {
        for(itr = 7; itr <= 11; itr++)
        {
            if(AI_OVL_SplPriorities[itr] < 20)
            {
                AI_OVL_SplPriorities[itr] = 0;
            }
            else if(AI_OVL_SplPriorities[itr] < 50)
            {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 3;
            }
            else
            {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 2;
            }
        }
    }

    /* Perform weighted selection */
    choice = Get_Weighted_Choice(&AI_OVL_SplPriorities[0], 50);

    /* Map selection back to spell identifier */
    switch(choice)
    {
        case 1:  return spl_Ice_Storm;
        case 2:  return spl_Earthquake;
        case 3:  return spl_Spell_Blast;
        case 4:  return spl_Stasis;
        case 5:  return spl_Time_Stop;
        case 6:  return spl_Corruption;
        case 7:  return spl_Fire_Storm;
        case 8:  return spl_Raise_Volcano;
        case 10: return spl_Chaos_Rift;
        case 11: return spl_Call_The_Void;
        case 12: return spl_Famine;
        case 13: return spl_Warp_Node;
        case 14: return spl_Black_Wind;
        case 15: return spl_Drain_Power;
        case 16: return spl_Evil_Presence;
        case 17: return spl_Subversion;
        case 18: return spl_Pestilence;
        case 19: return spl_Cursed_Lands;
        case 20: return spl_Cruel_Unminding;
        default: return spl_NONE;
    }

}

// WZD o156p10
/**
 * @brief Selects a city-enchantment spell for the AI to cast.
 *
 * @details
 * Evaluates known city-enchantment spells available to the AI player and assigns
 * weighted priorities based on target viability and game state. The function:
 *
 * - Iterates through known city-enchantment spells (Wall of Stone, Transmute,
 *   Change Terrain, Move Fortress, Earth Gate, Gaias Blessing, Flying Fortress,
 *   Wall of Fire, Heavenly Light, Stream of Life, Inspirations, Prosperity,
 *   Astral Gate, Dark Rituals, Cloud of Shadow, Spell Ward, Consecration,
 *   Wall of Darkness, Altar of Battle)
 *
 * - Calls spell-specific targeting routines (AITP_*) to determine if each spell
 *   has a valid target on the current game state
 *
 * - Assigns priorities in the global `AI_OVL_SplPriorities[]` array based on
 *   target validity and turn count (e.g., Wall of Stone = _turn / 6,
 *   Gaias Blessing = 300 static, Inspirations/Prosperity = 300)
 *
 * - Applies suppression modifiers if Life, Nature, Death, or Chaos realms have
 *   global suppression active, reducing relevant spell weights by 2/3 or tiered
 *   reductions (1/3 or 1/2) depending on priority threshold
 *
 * - Uses weighted random selection to pick the final spell category from the
 *   priority array
 *
 * The switch statement following the weighted choice maps category indices to
 * concrete spell indices for dispatch to the casting system. Note the slot-4 gap
 * (no case 4) reflecting the original jump-table structure.
 *
 * @param player_idx Index of the AI-controlled player evaluating city enchantments.
 *
 * @return int16_t Spell index of the selected city-enchantment spell, or 0 if
 *                  no valid enchantment target is available.
 *
 * @note The global array `AI_OVL_SplPriorities[]` is reinitialized on each call
 *       and contains weighted entries indexed by spell category (1–20).
 * @note All city-enchantment spells rely on separate AITP_* targeting functions;
 *       if a targeting function returns 0, that spell's priority remains 0.
 * @note Suppression checks via SPL_IsLifeSupressed(), CRP_SPL_IsNatSuppressed(),
 *       SPL_IsDthSuppressed(), and SPL_IsChsSuppressed() reduce weights for spells
 *       incompatible with active global suppressions:
 *       - Life suppression affects Heavenly Light, Stream of Life, Inspirations,
 *         Prosperity, Spell Ward, and Consecration
 *       - Nature suppression affects Wall of Stone through Gaias Blessing
 *       - Death suppression affects Dark Rituals and Wall of Darkness
 *       - Chaos suppression affects Wall of Fire
 * @note The switch statement includes a slot-4 gap (no case 4) for historical
 *       compatibility with the original assembly jump table.
 */
int16_t AI_Select_Spell_Group_City_Enchantment(int16_t player_idx)
{
    int16_t choice = 0;
    int16_t Target_Realm = 0;
    int16_t target_wp = 0;
    int16_t target_city_idx = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t itr = 0;
    uint8_t * players_spell_list = NULL;

    /* Treat spell list as 1-based index by shifting pointer */
    players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

    for(itr = 0; itr < 50; itr++) {
        AI_OVL_SplPriorities[itr] = 0;
    }

    if(players_spell_list[spl_Wall_Of_Stone] == sls_Known) {
        if(AITP_Wall_Of_Stone(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[1] = _turn / 6;
        }
    }

    if(players_spell_list[spl_Transmute] == sls_Known) {
        if(AITP_Transmute(player_idx, &target_wx, &target_wy, &target_wp) == 1) {
            AI_OVL_SplPriorities[2] = 200;
        }
    }

    if(players_spell_list[spl_Change_Terrain] == sls_Known) {
        if(AITP_Change_Terrain(player_idx, &target_wx, &target_wy, &target_wp) == 1) {
            AI_OVL_SplPriorities[3] = 50;
        }
    }

    if(players_spell_list[spl_Move_Fortress] == sls_Known) {
        if(AITP_Move_Fortress(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[5] = _turn / 15;
        }
    }

    if(players_spell_list[spl_Earth_Gate] == sls_Known) {
        if(AITP_Earth_Gate(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[6] = _turn / 15;
        }
    }

    if(players_spell_list[spl_Gaias_Blessing] == sls_Known) {
        if(AITP_Gaias_Blessing(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[7] = 300;
        }
    }

    if(players_spell_list[spl_Flying_Fortress] == sls_Known) {
        if(AITP_Flying_Fortress(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[8] = 100;
        }
    }

    if(players_spell_list[spl_Wall_Of_Fire] == sls_Known) {
        if(AITP_Wall_Of_Fire(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[9] = _turn / 20;
        }
    }

    if(players_spell_list[spl_Heavenly_Light] == sls_Known) {
        if(AITP_Heavenly_Light(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[10] = _turn / 25;
        }
    }

    if(players_spell_list[spl_Stream_Of_Life] == sls_Known) {
        if(AITP_Stream_Of_Life(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[11] = _turn / 10;
        }
    }

    if(players_spell_list[spl_Inspirations] == sls_Known) {
        if(AITP_Inspirations(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[12] = 300;
        }
    }

    if(players_spell_list[spl_Prosperity] == sls_Known) {
        if(AITP_Prosperity(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[13] = 300;
        }
    }

    if(players_spell_list[spl_Astral_Gate] == sls_Known) {
        if(AITP_Astral_Gate(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[14] = 0;
        }
    }

    if(players_spell_list[spl_Dark_Rituals] == sls_Known) {
        if(AITP_Dark_Rituals(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[15] = 100;
        }
    }

    if(players_spell_list[spl_Cloud_Of_Shadow] == sls_Known) {
        if(AITP_Cloud_Of_Shadow(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[16] = _turn / 20;
        }
    }

    if(players_spell_list[spl_Spell_Ward] == sls_Known) {
        if(AITP_Spell_Ward(player_idx, &target_city_idx, &Target_Realm) == 1) {
            AI_OVL_SplPriorities[17] = 100;
        }
    }

    if(players_spell_list[spl_Consecration] == sls_Known) {
        if(AITP_Consecration(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[18] = 100;
        }
    }

    if(players_spell_list[spl_Wall_Of_Darkness] == sls_Known) {
        if(AITP_Wall_Of_Darkness(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[19] = _turn / 20;
        }
    }

    if(players_spell_list[spl_Altar_Of_Battle] == sls_Known) {
        if(AITP_Altar_Of_Battle(player_idx, &target_city_idx) == 1) {
            AI_OVL_SplPriorities[20] = 50;
        }
    }

    if(SPL_IsLifeSupressed() == ST_TRUE) {
        for(itr = 10; itr <= 14; itr++) {
            AI_OVL_SplPriorities[itr] = (AI_OVL_SplPriorities[itr] * 2) / 3;
        }
        AI_OVL_SplPriorities[18] = (AI_OVL_SplPriorities[18] * 2) / 3;
        AI_OVL_SplPriorities[20] = (AI_OVL_SplPriorities[20] * 2) / 3;
    }

    if(CRP_SPL_IsNatSuppressed() == ST_TRUE) {
        for(itr = 1; itr <= 7; itr++) {
            AI_OVL_SplPriorities[itr] = (AI_OVL_SplPriorities[itr] * 2) / 3;
        }
    }

    if(SPL_IsDthSuppressed() == ST_TRUE) {
        for(itr = 15; itr <= 16; itr++) {
            if(AI_OVL_SplPriorities[itr] < 20) {
                AI_OVL_SplPriorities[itr] = 0;
            } else if(AI_OVL_SplPriorities[itr] < 50) {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 3;
            } else {
                AI_OVL_SplPriorities[itr] = AI_OVL_SplPriorities[itr] / 2;
            }
        }
        /* index 19 (spl_Wall_of_Darkness) */
        if(AI_OVL_SplPriorities[19] < 20) {
            AI_OVL_SplPriorities[19] = 0;
        } else if(AI_OVL_SplPriorities[19] < 50) {
            AI_OVL_SplPriorities[19] = AI_OVL_SplPriorities[19] / 3;
        } else {
            AI_OVL_SplPriorities[19] = AI_OVL_SplPriorities[19] / 2;
        }
    }

    if(SPL_IsChsSuppressed() == ST_TRUE) {
        /* index 9 (spl_Wall_Of_Fire) */
        if(AI_OVL_SplPriorities[9] < 20) {
            AI_OVL_SplPriorities[9] = 0;
        } else if(AI_OVL_SplPriorities[9] < 50) {
            AI_OVL_SplPriorities[9] = AI_OVL_SplPriorities[9] / 3;
        } else {
            AI_OVL_SplPriorities[9] = AI_OVL_SplPriorities[9] / 2;
        }
    }

    choice = Get_Weighted_Choice(&AI_OVL_SplPriorities[0], 50);

    switch(choice)
    {
        case 1:
            return spl_Wall_Of_Stone;
        case 2:
            return spl_Transmute;
        case 3:
            return spl_Change_Terrain;
        case 5:
            return spl_Move_Fortress;
        case 6:
            return spl_Earth_Gate;
        case 7:
            return spl_Gaias_Blessing;
        case 8:
            return spl_Flying_Fortress;
        case 9:
            return spl_Wall_Of_Fire;
        case 10:
            return spl_Heavenly_Light;
        case 11:
            return spl_Stream_Of_Life;
        case 12:
            return spl_Inspirations;
        case 13:
            return spl_Prosperity;
        case 14:
            return spl_Astral_Gate;
        case 15:
            return spl_Dark_Rituals;
        case 16:
            return spl_Cloud_Of_Shadow;
        case 17:
            return spl_Spell_Ward;
        case 18:
            return spl_Consecration;
        case 19:
            return spl_Wall_Of_Darkness;
        case 20:
            return spl_Altar_Of_Battle;
        default:
            return 0;
    }

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
/**
 * @brief Selects a disenchantment spell for the AI to cast.
 *
 * @details
 * Evaluates known disenchantment spells available to the AI player and selects
 * the highest-priority available spell. The selection follows a fixed priority
 * order:
 *
 * 1. Disenchant_True (target-specific disenchantment)
 * 2. Disenchant_Area (area-of-effect disenchantment)
 *
 * The function does not apply weighted prioritization; it simply checks each
 * spell in priority order and returns the first one the player has learned.
 * If neither disenchantment spell is known, the function returns spl_NONE.
 *
 * @param player_idx Index of the AI-controlled player evaluating disenchantment
 *                   spell availability.
 *
 * @return int16_t Spell index of the selected disenchantment spell
 *                  (spl_Disenchant_True or spl_Disenchant_Area), or spl_NONE
 *                  if no disenchantment spell is available.
 *
 * @note The player's spell list is treated as 1-based indexed by shifting the
 *       pointer one position back: `spells_list - 1`.
 * @note This function does not use the global `AI_OVL_SplPriorities[]` array
 *       or weighted selection; it applies a deterministic priority check.
 * @note Both disenchantment spells are checked in order; Disenchant_True is
 *       preferred over Disenchant_Area.
 */
int16_t AI_Select_Spell_Group_Disenchant(int16_t player_idx)
{
    uint8_t * players_spell_list = NULL;

    players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

    if(players_spell_list[spl_Disenchant_True] == sls_Known)
    {
        return spl_Disenchant_True;
    }

    if(players_spell_list[spl_Disenchant_Area] == sls_Known)
    {
        return spl_Disenchant_Area;
    }

    return spl_NONE;

}

// WZD o156p16
/**
 * @brief Selects a disjunction spell for the AI to cast.
 *
 * @details
 * Evaluates known disjunction spells available to the AI player and selects
 * the highest-priority available spell. The selection follows a fixed priority
 * order:
 *
 * 1. Disjunction_True (target-specific disjunction)
 * 2. Disjunction (standard disjunction)
 *
 * The function does not apply weighted prioritization; it simply checks each
 * spell in priority order and returns the first one the player has learned.
 * If neither disjunction spell is known, the function returns spl_NONE.
 *
 * @param player_idx Index of the AI-controlled player evaluating disjunction
 *                   spell availability.
 *
 * @return int16_t Spell index of the selected disjunction spell
 *                  (spl_Disjunction_True or spl_Disjunction), or spl_NONE
 *                  if no disjunction spell is available.
 *
 * @note The player's spell list is treated as 1-based indexed by shifting the
 *       pointer one position back: `spells_list - 1`.
 * @note This function does not use the global `AI_OVL_SplPriorities[]` array
 *       or weighted selection; it applies a deterministic priority check.
 * @note Both disjunction spells are checked in order; Disjunction_True is
 *       preferred over the standard Disjunction spell.
 */
int16_t AI_Select_Spell_Group_Disjunction(int16_t player_idx)
{
    uint8_t * players_spell_list = NULL;

    /* Treat spell list as 1-based index by shifting pointer */
    players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

    if(players_spell_list[spl_Disjunction_True] == sls_Known)
    {
        return spl_Disjunction_True;
    }

    if(players_spell_list[spl_Disjunction] == sls_Known)
    {
        return spl_Disjunction;
    }

    return spl_NONE;
}


// WZD o156p17
/**
 * @brief AI target picker for Wall of Stone — selects the highest-value owned city without walls.
 *
 * @details
 * This function identifies the best candidate city for casting the Wall of Stone spell.
 * The spell grants city walls to a target city, so the AI must find a valid recipient.
 *
 * The selection algorithm:
 * 1. Iterates through all cities on the map
 * 2. Filters for cities owned by @p player_idx
 * 3. Further filters for cities with no walls already built
 *    (i.e., @c _CITIES[].bldg_status[bt_CityWalls] == bs_NotBuilt)
 * 4. Scores each candidate using the city's value from @c _ai_all_own_city_values[]
 * 5. Selects the city with the highest value and stores its index in @p city_idx
 *
 * Wall of Stone is most beneficial for high-value cities (e.g., production hubs, trade
 * centers) as the spell grants defensive walls. By targeting the highest-value city,
 * the AI prioritizes protecting its most important urban centers.
 *
 * @param player_idx    Index of the AI player casting Wall of Stone.
 * @param[out] city_idx Pointer to receive the index of the selected city in @c _CITIES[].
 *                      Only written when the function returns ST_TRUE.
 *
 * @return ST_TRUE if a valid target city (owned by the player, with no walls) was found
 *         and @p city_idx was populated; ST_FALSE if no valid target exists (e.g., all
 *         player cities already have walls, or the player has no cities).
 *
 * @note The selection is deterministic: given the same game state and city values,
 *       the same city will always be selected.
 * @note The city value is sourced from @c _ai_all_own_city_values[], which should be
 *       pre-computed or updated before calling this function. If this array is stale,
 *       targeting may be suboptimal.
 * @note A city with @c bs_NotBuilt status for walls is the only valid target for this
 *       spell; cities that have walls (or are under construction) are skipped.
 *
 * @see AI_Select_Spell_Group_City_Enchantment(), _ai_all_own_city_values[]
 */
int16_t AITP_Wall_Of_Stone(int16_t player_idx, int16_t * city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].bldg_status[bt_CityWalls] == bs_NotBuilt)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *city_idx = best_city_idx;
        return ST_TRUE;
    }

}


// WZD o156p18
/**
 * @brief AI target picker for Transmute — selects the highest-value terrain square within
 *        city catchments that can be converted.
 *
 * @details
 * This function identifies the best candidate square for casting the Transmute spell.
 * The spell converts terrain features (e.g., coal, iron, mithril) or terrain types,
 * so the AI must find a valid, high-value target square within its city catchments.
 *
 * The selection algorithm:
 * 1. Iterates through all cities on the map
 * 2. Filters for cities owned by @p player_idx
 * 3. For each player city, scans a 5×5 catchment area (wy_offset ∈ [-2, +2],
 *    wx_offset ∈ [-2, +2] with wrapping on the x-axis)
 * 4. Within each catchment, checks for transmutable terrain features (coal, iron, mithril, etc.)
 * 5. Scores each valid candidate square using its terrain value
 * 6. Selects the square with the highest value and stores its coordinates
 *
 * The Transmute spell is most beneficial for squares containing valuable minerals or
 * improving terrain. By targeting the highest-value square in the AI's city areas,
 * the function prioritizes economically significant transmutations.
 *
 * **Coordinate Wrapping:**
 * - The y-axis (north–south) does not wrap; squares with wy < 0 or wy >= WORLD_HEIGHT
 *   are skipped.
 * - The x-axis (east–west) wraps: negative wx is converted to (wx + WORLD_WIDTH),
 *   and wx >= WORLD_WIDTH is converted to (wx - WORLD_WIDTH).
 *
 * @param player_idx        Index of the AI player casting Transmute.
 * @param[out] targeted_wx  Pointer to receive the x-coordinate of the selected square.
 *                          Only written when the function returns ST_TRUE.
 * @param[out] targeted_wy  Pointer to receive the y-coordinate of the selected square.
 *                          Only written when the function returns ST_TRUE.
 * @param[out] targeted_wp  Pointer to receive the plane index of the selected square.
 *                          Only written when the function returns ST_TRUE.
 *
 * @return ST_TRUE if a valid target square (with transmutable terrain features) was found
 *         and the output pointers were populated; ST_FALSE if no valid target exists
 *         (e.g., no player cities, no transmutable terrain in catchments).
 *
 * @note The selection is deterministic: given the same game state and terrain values,
 *       the same square will always be selected.
 * @note The terrain value for each candidate square is sourced from the terrain value
 *       system; if this system is not pre-computed or is stale, targeting may be suboptimal.
 * @note Only terrain with specific special features (coal, iron, mithril, etc.) visible
 *       via @c GET_TERRAIN_SPECIAL() are considered valid targets.
 * @note The x-axis wraps around the world; this means a city on the eastern edge has
 *       squares from the western edge in its catchment.
 * @note (OGBUG) Terrain special features are not true bitflags; some are single-bit
 *       values that may not combine properly with bitwise AND operations.
 *
 * @see AI_Select_Spell_Group_City_Enchantment(), GET_TERRAIN_SPECIAL()
 */
int16_t AITP_Transmute(int16_t player_idx, int16_t * targeted_wx, int16_t * targeted_wy, int16_t * targeted_wp)
{
    int16_t target_wp = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t city_wp = 0;
    int16_t wy_offset = 0;
    int16_t wx_offset = 0;
    int16_t highest_value = 0;
    int16_t city_idx = 0;
    int16_t itr_cities = 0;
    int16_t city_area_wy = 0;
    int16_t city_area_wx = 0;

    city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            city_wp = _CITIES[itr_cities].wp;
            for(wy_offset = -2; wy_offset < 3; wy_offset++)
            {
                city_area_wy = _CITIES[itr_cities].wy + wy_offset;
                if(city_area_wy >= 0 && city_area_wy < WORLD_HEIGHT)
                {
                    for(wx_offset = -2; wx_offset < 3; wx_offset++)
                    {
                        city_area_wx = _CITIES[itr_cities].wx + wx_offset;
                        if(city_area_wx < 0)
                        {
                            city_area_wx += WORLD_WIDTH;
                        }
                        if(city_area_wx >= WORLD_WIDTH)
                        {
                            city_area_wx -= WORLD_WIDTH;
                        }

                        if(
                            ((GET_TERRAIN_SPECIAL(city_area_wx, city_area_wy, city_wp) & TS_COAL) != 0) /* OGBUG  this is not a bitflag */
                            ||
                            ((GET_TERRAIN_SPECIAL(city_area_wx, city_area_wy, city_wp) & TS_IRON) != 0)  /* OGBUG  this is not a bitflag */
                            ||
                            ((GET_TERRAIN_SPECIAL(city_area_wx, city_area_wy, city_wp) & TS_SILVER) != 0)  /* OGBUG  this is not a bitflag */
                        )
                        {
                            if(_ai_all_own_city_values[itr_cities] > highest_value)
                            {
                                city_idx = itr_cities;
                                highest_value = _ai_all_own_city_values[itr_cities];
                                target_wx = city_area_wx;
                                target_wy = city_area_wy;
                                target_wp = city_wp;
                            }
                        }
                    }
                }
            }
        }
    }

    if(city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_wx = target_wx;
        *targeted_wy = target_wy;
        *targeted_wp = target_wp;
        return ST_TRUE;
    }

}

// WZD o156p19
/**
 * @brief AI target picker for Change Terrain — selects the highest-value city with
 *        a desert or swamp square in its catchment.
 *
 * @details
 * This function identifies the best candidate square for casting the Change Terrain spell.
 * The spell converts terrain types (e.g., desert to grassland, swamp to grassland),
 * so the AI must find a valid, high-value city that can benefit from terrain conversion.
 *
 * The selection algorithm:
 * 1. Iterates through all cities on the map
 * 2. Filters for cities owned by @p player_idx
 * 3. For each player city, scans a 5×5 catchment area (wy_offset ∈ [-2, +2],
 *    wx_offset ∈ [-2, +2] with wrapping on the x-axis)
 * 4. Within each catchment, checks for terrain squares that are either desert or swamp
 *    (via @c Square_Is_Desert() and @c Square_Is_Swamp())
 * 5. Scores each valid candidate using the city's value from @c _ai_all_own_city_values[]
 *    (NOT the terrain square's value)
 * 6. Selects the square from the highest-value city and stores its coordinates
 *
 * The Change Terrain spell is most beneficial for high-value cities with poor terrain.
 * By targeting deserts or swamps in high-value city areas, the AI can improve land
 * productivity and economic output.
 *
 * **Coordinate Wrapping:**
 * - The y-axis (north–south) is bounds-checked: squares with wy < WORLD_YMIN or
 *   wy >= WORLD_HEIGHT are skipped.
 * - The x-axis (east–west) wraps: negative wx is converted to (wx + WORLD_WIDTH),
 *   and wx >= WORLD_WIDTH is converted to (wx - WORLD_WIDTH).
 *
 * @param player_idx        Index of the AI player casting Change Terrain.
 * @param[out] targeted_wx  Pointer to receive the x-coordinate of the selected square.
 *                          Only written when the function returns ST_TRUE.
 * @param[out] targeted_wy  Pointer to receive the y-coordinate of the selected square.
 *                          Only written when the function returns ST_TRUE.
 * @param[out] targeted_wp  Pointer to receive the plane index of the selected square.
 *                          Only written when the function returns ST_TRUE.
 *
 * @return ST_TRUE if a valid target square (desert or swamp in a player city's catchment)
 *         was found and the output pointers were populated; ST_FALSE if no valid target
 *         exists (e.g., no player cities, no desert/swamp terrain in catchments).
 *
 * @note The selection is deterministic: given the same game state and city values,
 *       the same square will always be selected.
 * @note The city value (from @c _ai_all_own_city_values[]) is used for scoring,
 *       not the individual terrain square's value. This means the function selects
 *       based on city importance, not terrain importance.
 * @note Only desert and swamp terrain types are considered valid targets; all other
 *       terrain types (grassland, mountain, forest, etc.) are skipped.
 * @note The x-axis wraps around the world; cities on the eastern edge have squares
 *       from the western edge in their catchment.
 * @note (OGBUG) The catchment corner squares (offsets [-2,-2], [-2,+2], [+2,-2], [+2,+2])
 *       should arguably be excluded as they do not directly affect the city, but they are
 *       currently included in the evaluation. This may lead to suboptimal targeting.
 *
 * @see AI_Select_Spell_Group_City_Enchantment(), Square_Is_Desert(), Square_Is_Swamp()
 */
/*
OGBUG  treats catchment corners as affecting the city, should skip corners
¿ OGBUG  only targets deserts and swamps ?
*/
int16_t AITP_Change_Terrain(int16_t player_idx, int16_t * targeted_wx, int16_t * targeted_wy, int16_t * targeted_wp)
{
    int16_t target_wp = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t city_area_wy = 0;
    int16_t city_wp = 0;
    int16_t wy_offset = 0;
    int16_t wx_offset = 0;
    int16_t highest_value = 0;
    int16_t target_city_idx = 0;
    int16_t itr_cities = 0;
    int16_t city_area_wx = 0;

    target_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            city_wp = _CITIES[itr_cities].wp;
            for(wy_offset = -2; wy_offset < 3; wy_offset++)
            {
                city_area_wy = _CITIES[itr_cities].wy + wy_offset;
                if(city_area_wy >= WORLD_YMIN && city_area_wy < WORLD_HEIGHT)
                {
                    for(wx_offset = -2; wx_offset < 3; wx_offset++)
                    {
                        city_area_wx = _CITIES[itr_cities].wx + wx_offset;
                        if(city_area_wx < 0)
                        {
                            city_area_wx += WORLD_WIDTH;
                        }
                        if(city_area_wx >= WORLD_WIDTH)
                        {
                            city_area_wx -= WORLD_WIDTH;
                        }

                        if(
                            Square_Is_Desert(city_area_wx, city_area_wy, city_wp) == ST_TRUE
                            ||
                            Square_Is_Swamp(city_area_wx, city_area_wy, city_wp) == ST_TRUE
                        )
                        {
                            if(_ai_all_own_city_values[itr_cities] > highest_value)
                            {
                                target_city_idx = itr_cities;
                                highest_value = _ai_all_own_city_values[itr_cities];
                                target_wx = city_area_wx;
                                target_wy = city_area_wy;
                                target_wp = city_wp;
                            }
                        }
                    }
                }
            }
        }
    }

    if(target_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_wx = target_wx;
        *targeted_wy = target_wy;
        *targeted_wp = target_wp;
        return ST_TRUE;
    }

}


// WZD o156p20
/**
 * @brief AI target picker for Move Fortress — locates a city with stronger garrison
 *        to relocate the fortress to.
 *
 * @details
 * This function identifies the best candidate city for casting the Move Fortress spell.
 * The spell relocates the player's fortress to a new city location, and the AI should
 * move it to a location with higher military strength (stronger garrison).
 *
 * The selection algorithm:
 * 1. Locates the current fortress city by matching coordinates between @c _FORTRESSES[]
 *    and @c _CITIES[] (comparing wx, wy, and wp fields)
 * 2. Records the current fortress garrison strength from @c _ai_all_own_garrison_strengths[]
 * 3. Iterates through all player-owned cities on the map
 * 4. Filters out the fortress city itself (skips self-comparison)
 * 5. For each other player city, compares its garrison strength to the fortress garrison
 * 6. Selects the city with the highest garrison strength that exceeds the fortress strength
 * 7. Returns the index of that city if found
 *
 * The Move Fortress spell is most beneficial when the AI can relocate to a city with
 * higher military capacity, thereby strengthening overall fortress defense and positioning.
 *
 * @param player_idx      Index of the AI player casting Move Fortress.
 * @param[out] city_idx   Pointer to receive the index of the target city in @c _CITIES[].
 *                        Only written when the function returns ST_TRUE.
 *
 * @return ST_TRUE if a valid target city (owned by the player, with garrison strength
 *         greater than the fortress) was found and @p city_idx was populated; ST_FALSE
 *         if no such target exists (all other player cities have weaker or equal garrison).
 *
 * @note The selection is deterministic: given the same game state and garrison values,
 *       the same city will always be selected (the highest-garrison city found during
 *       iteration).
 * @note The fortress location is determined by coordinate matching (@c _FORTRESSES[])
 *       against @c _CITIES[]; this assumes the fortress is always at a city location.
 * @note Garrison strength is sourced from @c _ai_all_own_garrison_strengths[], which
 *       should be pre-computed or updated before calling this function. If this array
 *       is stale, targeting may be suboptimal.
 * @note The comparison is strict (>) rather than (>=); the target city garrison must
 *       be strictly greater than the fortress garrison to be selected.
 * @note Only player-owned cities (@c owner_idx == player_idx) are considered; neutral
 *       or enemy cities are skipped.
 *
 * @see AI_Select_Spell_Group_City_Enchantment(), _FORTRESSES[], _ai_all_own_garrison_strengths[]
 */
int16_t AITP_Move_Fortress(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t fortress_city = 0;
    int16_t garrison_strength = 0;
    int16_t target_city_idx = 0;
    int16_t itr_cities = 0;

    /* Locate the player's fortress city */
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].wx == _FORTRESSES[player_idx].wx &&
            _CITIES[itr_cities].wy == _FORTRESSES[player_idx].wy &&
            _CITIES[itr_cities].wp == _FORTRESSES[player_idx].wp)
        {
            fortress_city = itr_cities;
        }
    }

    target_city_idx = ST_UNDEFINED;
    garrison_strength = _ai_all_own_garrison_strengths[fortress_city];

    /* Find the player's city with the strongest garrison (excluding the fortress itself)
       that is stronger than the current fortress garrison. */
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(itr_cities == fortress_city)
        {
            continue;
        }

        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_ai_all_own_garrison_strengths[itr_cities] > garrison_strength)
            {
                target_city_idx = itr_cities;
                garrison_strength = _ai_all_own_garrison_strengths[itr_cities];
            }
        }
    }

    if(target_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = target_city_idx;
        return ST_TRUE;
    }

}

// WZD o156p21
/**
 * @brief AI target picker for Flying Fortress — verifies if the fortress has the
 *        Flying Fortress enchantment and returns the fortress city if it does.
 *
 * @details
 * This function validates the fortress's eligibility for the Flying Fortress spell
 * by checking whether the fortress city currently has the Flying Fortress enchantment
 * active. Unlike other city-enchantment spells that target cities WITHOUT an enchantment,
 * this function validates the presence of an existing enchantment.
 *
 * The validation algorithm:
 * 1. Locates the player's fortress city by matching coordinates between @c _FORTRESSES[]
 *    and @c _CITIES[] (comparing wx, wy, and wp fields)
 * 2. Checks if the fortress city has the Flying Fortress enchantment active
 *    (i.e., @c _CITIES[fortress_city_idx].enchantments[FLYING_FORTRESS] != ST_FALSE)
 * 3. If the enchantment is present, returns the fortress city index as the valid target
 * 4. If the enchantment is absent, returns FALSE (no valid target)
 *
 * The Flying Fortress spell enchantment is related to fortress mobility or capabilities.
 * This function validates the current enchantment state of the fortress, which may be used
 * to prevent duplicate applications or to verify prerequisites for other fortress-related spells.
 *
 * @param player_idx         Index of the AI player evaluating the fortress spell.
 * @param[out] targeted_city_idx Pointer to receive the index of the fortress city in @c _CITIES[].
 *                           Only written when the function returns ST_TRUE (enchantment present).
 *
 * @return ST_TRUE if the player's fortress city is located and currently has the
 *         Flying Fortress enchantment active (enchantments[FLYING_FORTRESS] != ST_FALSE),
 *         and @p targeted_city_idx was populated; ST_FALSE if the fortress does not have
 *         the enchantment or if no fortress is found.
 *
 * @note Unlike most AITP_* targeting functions that select cities WITHOUT an enchantment,
 *       this function returns TRUE when the enchantment IS PRESENT. The logic is inverted
 *       from the standard enchantment-seeking functions.
 * @note The fortress location is determined by coordinate matching (@c _FORTRESSES[])
 *       against @c _CITIES[]; this assumes the fortress is always at a city location.
 * @note Only the player's fortress is checked; other cities are not considered as targets.
 * @note This function validates the enchantment presence rather than selecting a target city;
 *       it may be used for validation or state-checking rather than spell casting.
 *
 * @see AI_Select_Spell_Group_City_Enchantment(), _FORTRESSES[]
 */
int16_t AITP_Flying_Fortress(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t fortress_city_idx = 0;
    int16_t target_city_idx = 0;
    int16_t itr_cities = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].wx == _FORTRESSES[player_idx].wx &&
            _CITIES[itr_cities].wy == _FORTRESSES[player_idx].wy &&
            _CITIES[itr_cities].wp == _FORTRESSES[player_idx].wp)
        {
            fortress_city_idx = itr_cities;
        }
    }

    if(_CITIES[fortress_city_idx].enchantments[FLYING_FORTRESS] != ST_FALSE)
    {
        target_city_idx = fortress_city_idx;
    }
    else
    {
        target_city_idx = ST_UNDEFINED;
    }

    if(target_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = target_city_idx;
        return ST_TRUE;
    }

}

// WZD o156p22
/**
 * @brief AI target picker for Earth Gate — selects the highest-value owned city without
 *        the Earth Gate enchantment.
 *
 * @details
 * This function identifies the best candidate city for casting the Earth Gate spell.
 * The spell grants an Earth Gate enchantment to a target city, which allows rapid troop
 * movement between gated cities. The AI should apply this enchantment to its most valuable
 * cities to create a strategic transportation network.
 *
 * The selection algorithm:
 * 1. Iterates through all cities on the map
 * 2. Filters for cities owned by @p player_idx
 * 3. Further filters for cities without the Earth Gate enchantment already active
 *    (i.e., @c _CITIES[].enchantments[EARTH_GATE] == ST_FALSE)
 * 4. Scores each candidate using the city's value from @c _ai_all_own_city_values[]
 * 5. Selects the city with the highest value and stores its index in @p targeted_city_idx
 *
 * The Earth Gate spell is most beneficial for high-value cities (e.g., production hubs,
 * strategic locations) as it enables rapid reinforcement and troop redirection. By targeting
 * the highest-value city without the enchantment, the AI prioritizes expanding its
 * transportation network through key economic and military centers.
 *
 * @param player_idx         Index of the AI player casting Earth Gate.
 * @param[out] targeted_city_idx Pointer to receive the index of the selected city in @c _CITIES[].
 *                           Only written when the function returns ST_TRUE.
 *
 * @return ST_TRUE if a valid target city (owned by the player, without the Earth Gate
 *         enchantment) was found and @p targeted_city_idx was populated; ST_FALSE if
 *         no valid target exists (e.g., all player cities already have Earth Gate, or
 *         the player has no cities).
 *
 * @note The selection is deterministic: given the same game state and city values,
 *       the same city will always be selected.
 * @note The city value is sourced from @c _ai_all_own_city_values[], which should be
 *       pre-computed or updated before calling this function. If this array is stale,
 *       targeting may be suboptimal.
 * @note A city with @c enchantments[EARTH_GATE] == ST_FALSE is the only valid target
 *       for this spell; cities that already have the Earth Gate enchantment are skipped.
 * @note Only player-owned cities are considered; neutral or enemy cities are excluded.
 *
 * @see AI_Select_Spell_Group_City_Enchantment(), _ai_all_own_city_values[]
 */
int16_t AITP_Earth_Gate(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].enchantments[EARTH_GATE] == ST_FALSE)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}

// WZD o156p23
/**
 * @brief AI target picker for Gaia's Blessing — selects the highest-value owned city without
 *        the Gaia's Blessing enchantment.
 *
 * @details
 * This function identifies the best candidate city for casting the Gaia's Blessing spell.
 * The spell grants a Gaia's Blessing enchantment to a target city, which provides
 * various benefits such as improved terrain production, population growth, or environmental
 * stability. The AI should apply this enchantment to its most valuable cities to maximize
 * overall economic and strategic benefit.
 *
 * The selection algorithm:
 * 1. Iterates through all cities on the map
 * 2. Filters for cities owned by @p player_idx
 * 3. Further filters for cities without the Gaia's Blessing enchantment already active
 *    (i.e., @c _CITIES[].enchantments[GAIAS_BLESSING] == ST_FALSE)
 * 4. Scores each candidate using the city's value from @c _ai_all_own_city_values[]
 * 5. Selects the city with the highest value and stores its index in @p targeted_city_idx
 *
 * The Gaia's Blessing spell is most beneficial for high-value cities (e.g., production
 * hubs, population centers, strategic locations) as the blessing enhances overall
 * productivity. By targeting the highest-value city without the enchantment, the AI
 * prioritizes improving its most important urban centers.
 *
 * @param player_idx         Index of the AI player casting Gaia's Blessing.
 * @param[out] targeted_city_idx Pointer to receive the index of the selected city in @c _CITIES[].
 *                           Only written when the function returns ST_TRUE.
 *
 * @return ST_TRUE if a valid target city (owned by the player, without the Gaia's Blessing
 *         enchantment) was found and @p targeted_city_idx was populated; ST_FALSE if
 *         no valid target exists (e.g., all player cities already have Gaia's Blessing, or
 *         the player has no cities).
 *
 * @note The selection is deterministic: given the same game state and city values,
 *       the same city will always be selected.
 * @note The city value is sourced from @c _ai_all_own_city_values[], which should be
 *       pre-computed or updated before calling this function. If this array is stale,
 *       targeting may be suboptimal.
 * @note A city with @c enchantments[GAIAS_BLESSING] == ST_FALSE is the only valid target
 *       for this spell; cities that already have the Gaia's Blessing enchantment are skipped.
 * @note Only player-owned cities are considered; neutral or enemy cities are excluded.
 *
 * @see AI_Select_Spell_Group_City_Enchantment(), _ai_all_own_city_values[]
 */
int16_t AITP_Gaias_Blessing(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].enchantments[GAIAS_BLESSING] == ST_FALSE)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}

// WZD o156p24
/**
 * @brief AI target picker for Wall of Fire — selects the highest-value owned city without
 *        the Wall of Fire enchantment.
 *
 * @details
 * This function identifies the best candidate city for casting the Wall of Fire spell.
 * The spell grants a Wall of Fire enchantment to a target city, which provides defensive
 * protection or offensive enhancement (fire-based). The AI should apply this enchantment
 * to its most valuable cities to maximize defensive capabilities or strategic benefits.
 *
 * The selection algorithm:
 * 1. Iterates through all cities on the map
 * 2. Filters for cities owned by @p player_idx
 * 3. Further filters for cities without the Wall of Fire enchantment already active
 *    (i.e., @c _CITIES[].enchantments[WALL_OF_FIRE] == ST_FALSE)
 * 4. Scores each candidate using the city's value from @c _ai_all_own_city_values[]
 * 5. Selects the city with the highest value and stores its index in @p targeted_city_idx
 *
 * The Wall of Fire spell is most beneficial for high-value cities (e.g., production hubs,
 * strategic locations, capitals) as the fire wall provides defensive protection or enhanced
 * offensive capabilities. By targeting the highest-value city without the enchantment,
 * the AI prioritizes protecting its most important urban centers.
 *
 * @param player_idx         Index of the AI player casting Wall of Fire.
 * @param[out] targeted_city_idx Pointer to receive the index of the selected city in @c _CITIES[].
 *                           Only written when the function returns ST_TRUE.
 *
 * @return ST_TRUE if a valid target city (owned by the player, without the Wall of Fire
 *         enchantment) was found and @p targeted_city_idx was populated; ST_FALSE if
 *         no valid target exists (e.g., all player cities already have Wall of Fire, or
 *         the player has no cities).
 *
 * @note The selection is deterministic: given the same game state and city values,
 *       the same city will always be selected.
 * @note The city value is sourced from @c _ai_all_own_city_values[], which should be
 *       pre-computed or updated before calling this function. If this array is stale,
 *       targeting may be suboptimal.
 * @note A city with @c enchantments[WALL_OF_FIRE] == ST_FALSE is the only valid target
 *       for this spell; cities that already have the Wall of Fire enchantment are skipped.
 * @note Only player-owned cities are considered; neutral or enemy cities are excluded.
 *
 * @see AI_Select_Spell_Group_City_Enchantment(), _ai_all_own_city_values[]
 */
int16_t AITP_Wall_Of_Fire(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].enchantments[WALL_OF_FIRE] == ST_FALSE)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}


// WZD o156p25
int16_t AITP_Wall_Of_Darkness(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].enchantments[WALL_OF_DARKNESS] == ST_FALSE)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}


// WZD o156p26
/**
 * @brief AI target picker for Heavenly Light — selects the highest-value owned city without
 *        the Heavenly Light enchantment.
 *
 * @details
 * This function identifies the best candidate city for casting the Heavenly Light spell.
 * The spell grants a Heavenly Light enchantment to a target city, which provides
 * beneficial effects (illumination, morale, or combat bonuses for defending units).
 * The AI should apply this enchantment to its most valuable cities to maximize the
 * benefit of the enchantment.
 *
 * The selection algorithm:
 * 1. Iterates through all cities on the map
 * 2. Filters for cities owned by @p player_idx
 * 3. Further filters for cities without the Heavenly Light enchantment already active
 *    (i.e., @c _CITIES[].enchantments[HEAVENLY_LIGHT] == ST_FALSE)
 * 4. Scores each candidate using the city's value from @c _ai_all_own_city_values[]
 * 5. Selects the city with the highest value and stores its index in @p targeted_city_idx
 *
 * The Heavenly Light spell is most beneficial for high-value cities (e.g., production
 * hubs, population centers, strategic positions) as the enchantment provides combat
 * and garrison advantages. By targeting the highest-value city without the enchantment,
 * the AI prioritizes fortifying its most important urban centers.
 *
 * @param player_idx             Index of the AI player casting Heavenly Light.
 * @param[out] targeted_city_idx Pointer to receive the index of the selected city in @c _CITIES[].
 *                               Only written when the function returns ST_TRUE.
 *
 * @return ST_TRUE if a valid target city (owned by the player, without the Heavenly Light
 *         enchantment) was found and @p targeted_city_idx was populated; ST_FALSE if
 *         no valid target exists (e.g., all player cities already have Heavenly Light, or
 *         the player has no cities).
 *
 * @note The selection is deterministic: given the same game state and city values,
 *       the same city will always be selected.
 * @note The city value is sourced from @c _ai_all_own_city_values[], which should be
 *       pre-computed or updated before calling this function. If this array is stale,
 *       targeting may be suboptimal.
 * @note A city with @c enchantments[HEAVENLY_LIGHT] == ST_FALSE is the only valid target
 *       for this spell; cities that already have the Heavenly Light enchantment are skipped.
 * @note Only player-owned cities are considered; neutral or enemy cities are excluded.
 *
 * @see AI_Select_Spell_Group_City_Enchantment(), _ai_all_own_city_values[]
 */
int16_t AITP_Heavenly_Light(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].enchantments[HEAVENLY_LIGHT] == ST_FALSE)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}

// WZD o156p27
int16_t AITP_Altar_Of_Battle(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].enchantments[ALTAR_OF_BATTLE] == ST_FALSE)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}


// WZD o156p28
/**
 * @brief AI target picker for Stream of Life — selects the highest-value owned city without
 *        the Stream of Life enchantment.
 *
 * @details
 * This function identifies the best candidate city for casting the Stream of Life spell.
 * The spell grants a Stream of Life enchantment to a target city, which provides population
 * growth or food/production benefits to that city. The AI should apply this enchantment to
 * its most valuable cities to maximize long-term economic and population benefits.
 *
 * The selection algorithm:
 * 1. Iterates through all cities on the map
 * 2. Filters for cities owned by @p player_idx
 * 3. Further filters for cities without the Stream of Life enchantment already active
 *    (i.e., @c _CITIES[].enchantments[STREAM_OF_LIFE] == ST_FALSE)
 * 4. Scores each candidate using the city's value from @c _ai_all_own_city_values[]
 * 5. Selects the city with the highest value and stores its index in @p targeted_city_idx
 *
 * The Stream of Life spell is most beneficial for high-value cities (e.g., population
 * centers, production hubs, food producers) as the enchantment enhances city growth and
 * productivity. By targeting the highest-value city without the enchantment, the AI
 * prioritizes strengthening its most important urban centers.
 *
 * @param player_idx             Index of the AI player casting Stream of Life.
 * @param[out] targeted_city_idx Pointer to receive the index of the selected city in @c _CITIES[].
 *                               Only written when the function returns ST_TRUE.
 *
 * @return ST_TRUE if a valid target city (owned by the player, without the Stream of Life
 *         enchantment) was found and @p targeted_city_idx was populated; ST_FALSE if
 *         no valid target exists (e.g., all player cities already have Stream of Life, or
 *         the player has no cities).
 *
 * @note The selection is deterministic: given the same game state and city values,
 *       the same city will always be selected.
 * @note The city value is sourced from @c _ai_all_own_city_values[], which should be
 *       pre-computed or updated before calling this function. If this array is stale,
 *       targeting may be suboptimal.
 * @note A city with @c enchantments[STREAM_OF_LIFE] == ST_FALSE is the only valid target
 *       for this spell; cities that already have the Stream of Life enchantment are skipped.
 * @note Only player-owned cities are considered; neutral or enemy cities are excluded.
 *
 * @see AI_Select_Spell_Group_City_Enchantment(), _ai_all_own_city_values[]
 */
int16_t AITP_Stream_Of_Life(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].enchantments[STREAM_OF_LIFE] == ST_FALSE)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}


// WZD o156p29
int16_t AITP_Inspirations(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].enchantments[INSPIRATIONS] == ST_FALSE)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}

// WZD o156p30
int16_t AITP_Prosperity(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].enchantments[PROSPERITY] == ST_FALSE)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}

// WZD o156p31
int16_t AITP_Astral_Gate(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].enchantments[ASTRAL_GATE] == ST_FALSE)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}

// WZD o156p32
int16_t AITP_Dark_Rituals(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cites = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cites = 0; itr_cites < _cities; itr_cites++)
    {
        if(_CITIES[itr_cites].owner_idx == player_idx)
        {
            if(_CITIES[itr_cites].enchantments[DARK_RITUALS] == ST_FALSE)
            {
                if(
                    _CITIES[itr_cites].bldg_status[bt_Temple] == bs_Built
                    ||
                    _CITIES[itr_cites].bldg_status[bt_Temple] == bs_Replaced
                )
                {
                    if(_ai_all_own_city_values[itr_cites] > highest_value)
                    {
                        best_city_idx = itr_cites;
                        highest_value = _ai_all_own_city_values[itr_cites];
                    }
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}

// WZD o156p33
int16_t AITP_Cloud_Of_Shadow(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_CITIES[itr_cities].enchantments[CLOUD_OF_SHADOW] == ST_FALSE)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}

// WZD o156p34
/* ¿ OGBUG  the fallback random option can return a realm against which the selected city already has a ward ? */
int16_t AITP_Spell_Ward(int16_t player_idx, int16_t * targeted_city_idx, int16_t * magic_realm)
{
    int16_t Target_Realm = 0;
    int16_t human_player_secondary_realm = 0;
    int16_t human_player_primary_realm = 0;
    int16_t highest_value = 0;
    int16_t itr_cities = 0;
    int16_t best_city_idx = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;
    human_player_primary_realm = _players[HUMAN_PLAYER_IDX].Prim_Realm;
    human_player_secondary_realm = _players[HUMAN_PLAYER_IDX].Sec_Realm;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(_ai_all_own_city_values[itr_cities] > highest_value)
            {
                if(human_player_primary_realm == sbr_Nature && _CITIES[itr_cities].enchantments[NATURE_WARD] == ST_FALSE)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                    Target_Realm = sbr_Nature;
                }
                else if(human_player_primary_realm == sbr_Chaos && _CITIES[itr_cities].enchantments[CHAOS_WARD] == ST_FALSE)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                    Target_Realm = sbr_Chaos;
                }
                else if(human_player_primary_realm == sbr_Sorcery && _CITIES[itr_cities].enchantments[SORCERY_WARD] == ST_FALSE)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                    Target_Realm = sbr_Sorcery;
                }
                else if(human_player_primary_realm == sbr_Death && _CITIES[itr_cities].enchantments[DEATH_WARD] == ST_FALSE)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                    Target_Realm = sbr_Death;
                }
                else if(human_player_primary_realm == sbr_Life && _CITIES[itr_cities].enchantments[LIFE_WARD] == ST_FALSE)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                    Target_Realm = sbr_Life;
                }
                else if(human_player_secondary_realm == sbr_Nature && _CITIES[itr_cities].enchantments[NATURE_WARD] == ST_FALSE)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                    Target_Realm = sbr_Nature;
                }
                else if(human_player_secondary_realm == sbr_Chaos && _CITIES[itr_cities].enchantments[CHAOS_WARD] == ST_FALSE)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                    Target_Realm = sbr_Chaos;
                }
                else if(human_player_secondary_realm == sbr_Sorcery && _CITIES[itr_cities].enchantments[SORCERY_WARD] == ST_FALSE)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                    Target_Realm = sbr_Sorcery;
                }
                else if(human_player_secondary_realm == sbr_Death && _CITIES[itr_cities].enchantments[DEATH_WARD] == ST_FALSE)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                    Target_Realm = sbr_Death;
                }
                else if(human_player_secondary_realm == sbr_Life && _CITIES[itr_cities].enchantments[LIFE_WARD] == ST_FALSE)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                    Target_Realm = sbr_Life;
                }
                else if(Random(10) == 1)
                {
                    best_city_idx = itr_cities;
                    highest_value = _ai_all_own_city_values[itr_cities];
                    Target_Realm = Random(5) - 1;
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        *magic_realm = Target_Realm;
        return ST_TRUE;
    }

}


// WZD o156p35
int16_t AITP_Consecration(int16_t player_idx, int16_t * targeted_city_idx)
{
    int16_t highest_value = 0;
    int16_t human_player_primary_realm = 0;
    int16_t human_player_secondary_realm = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    best_city_idx = ST_UNDEFINED;
    highest_value = 0;

    human_player_primary_realm = _players[0].Prim_Realm;
    human_player_secondary_realm = _players[0].Sec_Realm;

    if(human_player_primary_realm == sbr_Death ||
        human_player_secondary_realm == sbr_Death ||
        human_player_primary_realm == sbr_Chaos ||
        human_player_secondary_realm == sbr_Chaos ||
        Random(10) == 1)  /* 1:10; 10% chance */
    {
        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        {
            if(_CITIES[itr_cities].owner_idx == player_idx)
            {
                if(_ai_all_own_city_values[itr_cities] > highest_value)
                {
                    if(_CITIES[itr_cities].enchantments[CONSECRATION] == ST_FALSE)
                    {
                        best_city_idx = itr_cities;
                        highest_value = _ai_all_own_city_values[itr_cities];
                    }
                }
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_city_idx = best_city_idx;
        return ST_TRUE;
    }

}


// WZD o156p36
// drake178: UU_DBG_GetKnownSpells()


// WZD o156p37
void Cast_Spell_Target_Error(int16_t spell_idx)
{
    if(Check_Release_Version() == ST_TRUE)
    {
        return;
    }
    stu_strcpy(near_buffer, spell_data_table[spell_idx].name);
    stu_strcat(near_buffer, CRP_AI_SpellTargetError);  /* " could not be found for CP." */
    Exit_With_Message(near_buffer);
}


// WZD o156p38
// drake178: AI_ReleaseGlobals()
/*
releases unwanted overland enchantments or, if at zero
mana, all overland enchantments

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
        _players[player_idx].Globals[METEOR_STORMS] = 0;
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
            case spl_Wall_Of_Stone:    { return AITP_Wall_Of_Stone(player_idx, city_idx);    } break;
            case spl_Move_Fortress:    { return AITP_Move_Fortress(player_idx, city_idx);    } break;
            case spl_Earth_Gate:       { return AITP_Earth_Gate(player_idx, city_idx);       } break;
            case spl_Flying_Fortress:  { return AITP_Flying_Fortress(player_idx, city_idx);  } break;
            case spl_Wall_Of_Fire:     { return AITP_Wall_Of_Fire(player_idx, city_idx);     } break;
            case spl_Altar_Of_Battle:  { return AITP_Altar_Of_Battle(player_idx, city_idx);  } break;
            case spl_Heavenly_Light:   { return AITP_Heavenly_Light(player_idx, city_idx);   } break;
            case spl_Inspirations:     { return AITP_Inspirations(player_idx, city_idx);     } break;
            case spl_Stream_Of_Life:   { return AITP_Stream_Of_Life(player_idx, city_idx);   } break;
            case spl_Astral_Gate:      { return AITP_Astral_Gate(player_idx, city_idx);      } break;
            case spl_Prosperity:       { return AITP_Prosperity(player_idx, city_idx);       } break;
            case spl_Consecration:     { return AITP_Consecration(player_idx, city_idx);     } break;
            case spl_Cloud_Of_Shadow:  { return AITP_Cloud_Of_Shadow(player_idx, city_idx);  } break;
            case spl_Summoning_Circle: { return AITP_Summoning_Circle(player_idx, city_idx); } break;
            case spl_Dark_Rituals:     { return AITP_Dark_Rituals(player_idx, city_idx);     } break;
            case spl_Gaias_Blessing:   { return AITP_Gaias_Blessing(player_idx, city_idx);   } break;
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
/**
 * @brief Determines whether the AI player has access to at least one targetable city.
 *
 * @details
 * Iterates through all cities on the map and tests whether the specified player
 * has established contact (diplomatic knowledge) with at least one city using the
 * Test_Bit_Field() function on the city's contacts field. A player with targetable
 * cities can cast city-targeting spells such as city enchantments and curses.
 *
 * @param player_idx Index of the AI-controlled player being evaluated.
 *
 * @return ST_TRUE if the player has contact with at least one city (i.e., has
 *         targetable cities available); ST_FALSE if the player has no contact
 *         with any city on the map.
 *
 * @note The function uses Test_Bit_Field() to check the contacts bitfield
 *       for each city; this tests whether the player has discovered or made
 *       contact with that city.
 * @note This function is commonly called before spell-targeting operations
 *       to validate that the player can actually target a city for curses
 *       or enchantments (see AI_Select_Spell_Group_Attack for example usage).
 * @note The global `_cities` variable specifies the total number of cities
 *       to iterate through; iteration stops immediately upon finding the first
 *       city with which the player has contact.
 */
int16_t AI_Has_Targetable_City(int16_t player_idx)
{
    int16_t itr_cities = 0;
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(Test_Bit_Field(player_idx, (uint8_t *)&_CITIES[itr_cities].contacts) != ST_FALSE)
        {
            return ST_TRUE;
        }
    }
    return ST_FALSE;
}

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
/**
 * @brief AI target picker for Disjunction, Disjunction True, and Spell Binding — selects which opponent's
 *        active overland enchantment to dispel or steal.
 *
 * @details
 * Scores every active overland enchantment held by every opponent wizard and returns the player index and
 * global-enchantment slot index of the single highest-scoring candidate. Scoring logic differs by spell:
 *
 * **spl_Spell_Binding** — scores all opponents' globals with fixed threat weights, with two realm-sensitive
 * adjustments:
 *   - ETERNAL_NIGHT: score 30 if the caster is Death realm (i.e., it helps them), else 100.
 *   - CHAOS_SURGE: score 30 if the caster is Chaos realm, else 100.
 *
 * **spl_Disjunction / spl_Disjunction_True** — scores with higher, realm-reactive weights:
 *   - ETERNAL_NIGHT: 200 if caster is Life realm, else 50.
 *   - EVIL_OMENS: 200 if caster is Life or Nature realm, else 0 (irrelevant to remove).
 *   - NATURES_WRATH: 200 if caster is Death or Chaos realm, else 0.
 *   - HERB_MASTERY: 0 if caster is Death realm, else 50.
 *   - CHAOS_SURGE: 0 if caster is Chaos realm, else 100.
 *   - TRANQUILITY: 200 if caster is Chaos realm, else 0.
 *   - LIFE_FORCE: 200 if caster is Chaos realm, else 0.
 *   - SUPPRESS_MAGIC: always 250 (highest fixed weight).
 *
 * Both branches share a common set of non-realm-sensitive scores (e.g., ZOMBIE_MASTERY = 50,
 * DOOM_MASTERY = 50, CHARM_OF_LIFE = WORLD_WIDTH, ARMAGEDDON = 40, METEOR_STORMS = 30).
 *
 * After scoring, the function finds the single (opponent, global) pair with the maximum score and writes
 * the results to the output pointers. If no opponent has any scoreable global active, the outputs are
 * left untouched and the function returns ST_FALSE.
 *
 * For any other value of @p spell_idx the function calls Cast_Spell_Target_Error() and returns ST_FALSE.
 *
 * @param[out] targeted_player_idx  Receives the index of the opponent whose overland enchantment was chosen.
 *                                  Set to ST_UNDEFINED on entry; written only when a valid target is found.
 * @param[out] targeted_spell_idx   Receives the Globals[] slot index (e.g., SUPPRESS_MAGIC, ETERNAL_NIGHT)
 *                                  of the chosen enchantment. Set to ST_UNDEFINED on entry.
 * @param[in]  spell_idx            The spell being resolved: spl_Spell_Binding, spl_Disjunction, or
 *                                  spl_Disjunction_True. Any other value triggers Cast_Spell_Target_Error().
 * @param[in]  player_idx           Index of the AI player casting the spell. Used for realm-based score
 *                                  adjustments and to skip self when iterating opponents.
 *
 * @return ST_TRUE if a valid target (player + global slot) was identified and written to the output
 *         pointers; ST_FALSE if no opponent holds a scoreable overland enchantment, or if @p spell_idx
 *         is unrecognised.
 *
 * @note The internal scoring table `target_spell_scores[6][25]` uses @c int8_t.
 * @bug (OGBUG) `target_spell_scores` should be @c uint8_t rather than @c int8_t. No score currently
 *      reaches 128, so overflow does not occur in practice, but the type mismatch is present in the
 *      original disassembly.
 * @note The Spell Binding branch iterates up to NUM_PLAYERS (compile-time constant) when scanning for
 *       the maximum, but only fills scores for _num_players (runtime count); entries beyond the live
 *       player count remain zero and do not influence the result.
 * @note The Disjunction/Disjunction True branch correctly limits its scan to _num_players for both
 *       filling and maximum-finding.
 */
int16_t AITP_Disjunction(int16_t * targeted_player_idx, int16_t * targeted_spell_idx, int16_t spell_idx, int16_t player_idx)
{
    /* OGBUG  target_spell_scores should be uint8_t, not int8_t */
    int8_t target_spell_scores[6][25] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };  /* 1-byte, signed */
    int16_t max_score = 0;
    int16_t target_player_idx = 0;
    int16_t target_spell_idx = 0;
    int16_t spell_itr = 0;
    int16_t itr_players = 0;
    int16_t itr = 0;

    for(itr = 0; itr < NUM_PLAYERS; itr++)
    {
        for(spell_itr = 0; spell_itr < 25; spell_itr++)
        {
            target_spell_scores[itr][spell_itr] = 0;
        }
    }

    if(spell_idx == spl_Spell_Binding)
    {
        for(itr = 0; itr < _num_players; itr++)
        {
            if(itr == player_idx)
            {
                continue;
            }

            itr_players = itr;

            /* ETERNAL_NIGHT */
            if(_players[itr_players].Globals[ETERNAL_NIGHT] != 0)
            {
                if(_players[player_idx].Prim_Realm == sbr_Death || _players[player_idx].Sec_Realm == sbr_Death)
                {
                    target_spell_scores[itr_players][ETERNAL_NIGHT] = 30;
                }
                else
                {
                    target_spell_scores[itr_players][ETERNAL_NIGHT] = 100;
                }
            }

            /* EVIL_OMENS */
            if(_players[itr_players].Globals[EVIL_OMENS] != 0)
            {
                target_spell_scores[itr_players][EVIL_OMENS] = 10;
            }

            /* Zombie Mastery */
            if(_players[itr_players].Globals[ZOMBIE_MASTERY] != 0)
            {
                target_spell_scores[itr_players][ZOMBIE_MASTERY] = 50;
            }

            /* Aura of Majesty */
            if(_players[itr_players].Globals[AURA_OF_MAJESTY] != 0)
            {
                target_spell_scores[itr_players][AURA_OF_MAJESTY] = 20;
            }

            /* Wind Mastery */
            if(_players[itr_players].Globals[WIND_MASTERY] != 0)
            {
                target_spell_scores[itr_players][WIND_MASTERY] = 15;
            }

            /* Suppress Magic */
            if(_players[itr_players].Globals[SUPPRESS_MAGIC] != 0)
            {
                target_spell_scores[itr_players][SUPPRESS_MAGIC] = 100;
            }

            /* Time Stop */
            if(_players[itr_players].Globals[TIME_STOP] != 0)
            {
                target_spell_scores[itr_players][TIME_STOP] = 10;
            }

            /* Nature's Awareness */
            if(_players[itr_players].Globals[NATURES_AWARENESS] != 0)
            {
                target_spell_scores[itr_players][NATURES_AWARENESS] = 10;
            }

            /* Nature's Wrath */
            if(_players[itr_players].Globals[NATURES_WRATH] != 0)
            {
                target_spell_scores[itr_players][NATURES_WRATH] = 20;
            }

            /* Herb Mastery */
            if(_players[itr_players].Globals[HERB_MASTERY] != 0)
            {
                target_spell_scores[itr_players][HERB_MASTERY] = 20;
            }

            /* Chaos Surge */
            if(_players[itr_players].Globals[CHAOS_SURGE] != 0)
            {
                if(_players[player_idx].Prim_Realm == sbr_Chaos || _players[player_idx].Sec_Realm == sbr_Chaos)
                {
                    target_spell_scores[itr_players][CHAOS_SURGE] = 30;
                }
                else
                {
                    target_spell_scores[itr_players][CHAOS_SURGE] = 100;
                }
            }

            /* Doom Mastery */
            if(_players[itr_players].Globals[DOOM_MASTERY] != 0)
            {
                target_spell_scores[itr_players][DOOM_MASTERY] = 50;
            }

            /* Great Wasting */
            if(_players[itr_players].Globals[GREAT_WASTING] != 0)
            {
                target_spell_scores[itr_players][GREAT_WASTING] = 20;
            }

            /* Meteor Storm */
            if(_players[itr_players].Globals[METEOR_STORMS] != 0)
            {
                target_spell_scores[itr_players][METEOR_STORMS] = 30;
            }

            /* Armageddon */
            if(_players[itr_players].Globals[ARMAGEDDON] != 0)
            {
                target_spell_scores[itr_players][ARMAGEDDON] = 40;
            }

            /* Tranquility */
            if(_players[itr_players].Globals[TRANQUILITY] != 0)
            {
                target_spell_scores[itr_players][TRANQUILITY] = 30;
            }

            /* Life Force */
            if(_players[itr_players].Globals[LIFE_FORCE] != 0)
            {
                target_spell_scores[itr_players][LIFE_FORCE] = 30;
            }

            /* Crusade */
            if(_players[itr_players].Globals[CRUSADE] != 0)
            {
                target_spell_scores[itr_players][CRUSADE] = 20;
            }

            /* Just Cause */
            if(_players[itr_players].Globals[JUST_CAUSE] != 0)
            {
                target_spell_scores[itr_players][JUST_CAUSE] = 15;
            }

            /* Holy Arms */
            if(_players[itr_players].Globals[HOLY_ARMS] != 0)
            {
                target_spell_scores[itr_players][HOLY_ARMS] = 30;
            }

            /* Planar Seal */
            if(_players[itr_players].Globals[PLANAR_SEAL] != 0)
            {
                target_spell_scores[itr_players][PLANAR_SEAL] = 10;
            }

            /* Charm of Life */
            if(_players[itr_players].Globals[CHARM_OF_LIFE] != 0)
            {
                target_spell_scores[itr_players][CHARM_OF_LIFE] = WORLD_WIDTH;
            }

            /* Detect Magic */
            if(_players[itr_players].Globals[DETECT_MAGIC] != 0)
            {
                target_spell_scores[itr_players][DETECT_MAGIC] = 1;
            }

            /* Awareness */
            if(_players[itr_players].Globals[AWARENESS] != 0)
            {
                target_spell_scores[itr_players][AWARENESS] = 1;
            }
        }

        max_score = 0;
        target_spell_idx = ST_UNDEFINED;
        target_player_idx = ST_UNDEFINED;

        for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
        {
            for(itr = 0; itr < 25; itr++)
            {
                if(target_spell_scores[itr_players][itr] > max_score)
                {
                    target_player_idx = itr_players;
                    max_score = target_spell_scores[itr_players][itr];
                    target_spell_idx = itr;
                }
            }
        }

        if(target_spell_idx == ST_UNDEFINED)
        {
            return ST_FALSE;
        }
        else
        {
            *targeted_player_idx = target_player_idx;
            *targeted_spell_idx = target_spell_idx;
            return ST_TRUE;
        }
    }
    else if(spell_idx == spl_Disjunction || spell_idx == spl_Disjunction_True)
    {
        for(itr = 0; itr < _num_players; itr++)
        {
            if(itr == player_idx)
            {
                continue;
            }

            itr_players = itr;

            /* ETERNAL_NIGHT */
            if(_players[itr_players].Globals[ETERNAL_NIGHT] != 0)
            {
                if(_players[player_idx].Prim_Realm == sbr_Life || _players[player_idx].Sec_Realm == sbr_Life)
                {
                    target_spell_scores[itr_players][ETERNAL_NIGHT] = (int8_t)200;  /* OGBUG  target_spell_scores should be uint8_t, not int8_t */
                }
                else
                {
                    target_spell_scores[itr_players][ETERNAL_NIGHT] = 50;
                }
            }

            /* EVIL_OMENS */
            if(_players[itr_players].Globals[EVIL_OMENS] != 0)
            {
                if(_players[player_idx].Prim_Realm == sbr_Life || _players[player_idx].Sec_Realm == sbr_Life ||
                    _players[player_idx].Prim_Realm == sbr_Nature || _players[player_idx].Sec_Realm == sbr_Nature)
                {
                    target_spell_scores[itr_players][EVIL_OMENS] = (int8_t)200;  /* OGBUG  target_spell_scores should be uint8_t, not int8_t */
                }
                else
                {
                    target_spell_scores[itr_players][EVIL_OMENS] = 0;
                }
            }

            /* Zombie Mastery */
            if(_players[itr_players].Globals[ZOMBIE_MASTERY] != 0)
            {
                target_spell_scores[itr_players][ZOMBIE_MASTERY] = 50;
            }

            /* Aura of Majesty */
            if(_players[itr_players].Globals[AURA_OF_MAJESTY] != 0)
            {
                target_spell_scores[itr_players][AURA_OF_MAJESTY] = 20;
            }

            /* Wind Mastery */
            if(_players[itr_players].Globals[WIND_MASTERY] != 0)
            {
                target_spell_scores[itr_players][WIND_MASTERY] = 15;
            }

            /* Suppress Magic */
            if(_players[itr_players].Globals[SUPPRESS_MAGIC] != 0)
            {
                target_spell_scores[itr_players][SUPPRESS_MAGIC] = (int8_t)250;  /* OGBUG  target_spell_scores should be uint8_t, not int8_t */
            }

            /* Time Stop */
            if(_players[itr_players].Globals[TIME_STOP] != 0)
            {
                target_spell_scores[itr_players][TIME_STOP] = 10;
            }

            /* Nature's Awareness */
            if(_players[itr_players].Globals[NATURES_AWARENESS] != 0)
            {
                target_spell_scores[itr_players][NATURES_AWARENESS] = 10;
            }

            /* Nature's Wrath */
            if(_players[itr_players].Globals[NATURES_WRATH] != 0)
            {
                if(_players[player_idx].Prim_Realm == sbr_Death || _players[player_idx].Sec_Realm == sbr_Death ||
                    _players[player_idx].Prim_Realm == sbr_Chaos || _players[player_idx].Sec_Realm == sbr_Chaos)
                {
                    target_spell_scores[itr_players][NATURES_WRATH] = (int8_t)200;  /* OGBUG  target_spell_scores should be uint8_t, not int8_t */
                }
                else
                {
                    target_spell_scores[itr_players][NATURES_WRATH] = 0;
                }
            }

            /* Herb Mastery */
            if(_players[itr_players].Globals[HERB_MASTERY] != 0)
            {
                if(_players[player_idx].Prim_Realm == sbr_Death)
                {
                    target_spell_scores[itr_players][HERB_MASTERY] = 0;
                }
                else
                {
                    target_spell_scores[itr_players][HERB_MASTERY] = 50;
                }
            }

            /* Chaos Surge */
            if(_players[itr_players].Globals[CHAOS_SURGE] != 0)
            {
                if(_players[player_idx].Prim_Realm == sbr_Chaos || _players[player_idx].Sec_Realm == sbr_Chaos)
                {
                    target_spell_scores[itr_players][CHAOS_SURGE] = 0;
                }
                else
                {
                    target_spell_scores[itr_players][CHAOS_SURGE] = 100;
                }
            }

            /* Doom Mastery */
            if(_players[itr_players].Globals[DOOM_MASTERY] != 0)
            {
                target_spell_scores[itr_players][DOOM_MASTERY] = 50;
            }

            /* Great Wasting */
            if(_players[itr_players].Globals[GREAT_WASTING] != 0)
            {
                target_spell_scores[itr_players][GREAT_WASTING] = 20;
            }

            /* Meteor Storm */
            if(_players[itr_players].Globals[METEOR_STORMS] != 0)
            {
                target_spell_scores[itr_players][METEOR_STORMS] = 30;
            }

            /* Armageddon */
            if(_players[itr_players].Globals[ARMAGEDDON] != 0)
            {
                target_spell_scores[itr_players][ARMAGEDDON] = 40;
            }

            /* Tranquility */
            if(_players[itr_players].Globals[TRANQUILITY] != 0)
            {
                if(_players[player_idx].Prim_Realm == sbr_Chaos || _players[player_idx].Sec_Realm == sbr_Chaos)
                {
                    target_spell_scores[itr_players][TRANQUILITY] = (int8_t)200;  /* OGBUG  target_spell_scores should be uint8_t, not int8_t */
                }
                else
                {
                    target_spell_scores[itr_players][TRANQUILITY] = 0;
                }
            }

            /* Life Force */
            if(_players[itr_players].Globals[LIFE_FORCE] != 0)
            {
                if(_players[player_idx].Prim_Realm == sbr_Chaos || _players[player_idx].Sec_Realm == sbr_Chaos)
                {
                    target_spell_scores[itr_players][LIFE_FORCE] = (int8_t)200;  /* OGBUG  target_spell_scores should be uint8_t, not int8_t */
                }
                else
                {
                    target_spell_scores[itr_players][LIFE_FORCE] = 0;
                }
            }

            /* Crusade */
            if(_players[itr_players].Globals[CRUSADE] != 0)
            {
                target_spell_scores[itr_players][CRUSADE] = 20;
            }

            /* Just Cause */
            if(_players[itr_players].Globals[JUST_CAUSE] != 0)
            {
                target_spell_scores[itr_players][JUST_CAUSE] = 15;
            }

            /* Holy Arms */
            if(_players[itr_players].Globals[HOLY_ARMS] != 0)
            {
                target_spell_scores[itr_players][HOLY_ARMS] = 30;
            }

            /* Planar Seal */
            if(_players[itr_players].Globals[PLANAR_SEAL] != 0)
            {
                target_spell_scores[itr_players][PLANAR_SEAL] = 10;
            }

            /* Charm of Life */
            if(_players[itr_players].Globals[CHARM_OF_LIFE] != 0)
            {
                target_spell_scores[itr_players][CHARM_OF_LIFE] = WORLD_WIDTH;
            }

            /* Detect Magic */
            if(_players[itr_players].Globals[DETECT_MAGIC] != 0)
            {
                target_spell_scores[itr_players][DETECT_MAGIC] = 1;
            }

            /* Awareness */
            if(_players[itr_players].Globals[AWARENESS] != 0)
            {
                target_spell_scores[itr_players][AWARENESS] = 1;
            }
        }

        max_score = 0;
        target_spell_idx = ST_UNDEFINED;
        target_player_idx = ST_UNDEFINED;

        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            for(itr = 0; itr < 25; itr++)
            {
                if(target_spell_scores[itr_players][itr] > max_score)
                {
                    target_player_idx = itr_players;
                    max_score = target_spell_scores[itr_players][itr];
                    target_spell_idx = itr;
                }
            }
        }

        if(target_spell_idx == ST_UNDEFINED)
        {
            return ST_FALSE;
        }
        else
        {
            *targeted_player_idx = target_player_idx;
            *targeted_spell_idx = target_spell_idx;
            return ST_TRUE;
        }
    }
    else
    {
        Cast_Spell_Target_Error(spell_idx);
        return ST_FALSE;
    }

}


// WZD o156p45
int16_t Get_Map_Square_Target_For_Spell(int16_t spell_target_type, int16_t * wx, int16_t * wy, int16_t * wp, int16_t spell_idx, int16_t player_idx)
{

    int16_t niu_variable = 0;
    int16_t return_value = 0;  // DNE in Dasm

    switch(spell_idx)
    {
        case spl_Transmute:
        {
            return_value = AITP_Transmute(player_idx, wx, wy, wp);
        } break;
        case spl_Change_Terrain:
        {
            return_value = AITP_Change_Terrain(player_idx, wx, wy, wp);
        } break;
        case spl_Ice_Storm:
        case spl_Fire_Storm:
        case spl_Black_Wind:
        case spl_Stasis:
        {
            return_value = AITP_Attack_Stack(player_idx, wx, wy, wp);
        } break;
        case spl_Floating_Island:
        {
            return_value = AITP_Floating_Island(player_idx, wx, wy, wp);
        } break;
        case spl_Enchant_Road:
        {
            return_value = AITP_Enchant_Road(player_idx, wx, wy, wp);
        } break;
        case spl_Disenchant_True:
        case spl_Disenchant_Area:
        {
            return_value = AITP_Disenchant(player_idx, wx, wy, wp);
        } break;
        case spl_Corruption:
        case spl_Raise_Volcano:
        {
            return_value = AITP_Attack_Terrain(player_idx, wx, wy, wp, spell_idx);
        } break;
        case spl_Plane_Shift:
        {
            return_value = AITP_Plane_Shift(player_idx, wx, wy, wp);
        } break;
        case spl_Natures_Cures:
        {
            return_value = AITP_Natures_Cures(player_idx, wx, wy, wp);
        } break;
        default:
        {
            Cast_Spell_Target_Error(spell_idx);  // "[spell name] could not be found for CP."
            /* OGBUG  IDGI, but leaves the passed in parameter in AX which means it's the return value  `mov ax, [bp+spell_target_type]; mov [bp+niu_variable], ax;` */
            return_value = spell_target_type;
            niu_variable = return_value;
        } break;
    }

    return return_value;

}


// WZD o156p46
/* OGBUG  ignores invisibility */
/* OGBUG  ignores the sight range of units (using 2) */
int16_t AITP_Attack_Stack(int16_t player_idx, int16_t * targeted_wx, int16_t * targeted_wy, int16_t * targeted_wp)
{
    int16_t target_wx = 0;
    int16_t target_wy = 0;
    int16_t target_wp = 0;
    int16_t highest_value = 0;
    int16_t scouting_radious = 0;
    int16_t stack_is_visible = 0;
    int16_t enemy_stack_wx = 0;
    int16_t enemy_stack_wy = 0;
    int16_t enemy_stack_wp = 0;
    int16_t itr_enemy_stack_count = 0;
    int16_t itr_cities = 0;

    target_wx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_enemy_stack_count = 0; itr_enemy_stack_count < _ai_all_enemy_stack_count; itr_enemy_stack_count++)
    {
        /* OGBUG  should be signed, not unsigned */
        if((uint16_t)_ai_all_enemy_stacks[itr_enemy_stack_count].value > (uint16_t)highest_value)
        {
            stack_is_visible = ST_FALSE;
            enemy_stack_wx = _ai_all_enemy_stacks[itr_enemy_stack_count].wx;
            enemy_stack_wy = _ai_all_enemy_stacks[itr_enemy_stack_count].wy;
            enemy_stack_wp = _ai_all_enemy_stacks[itr_enemy_stack_count].wp;

            for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            {
                if(stack_is_visible != ST_FALSE)
                {
                    break;
                }

                if(_CITIES[itr_cities].owner_idx == player_idx)
                {
                    if(_CITIES[itr_cities].wp == enemy_stack_wp)
                    {
                        scouting_radious = 2;
                        if(_CITIES[itr_cities].bldg_status[bt_CityWalls] == bs_Built)
                        {
                            scouting_radious = 3;
                        }
                        if(_CITIES[itr_cities].bldg_status[bt_Oracle] == bs_Built)
                        {
                            scouting_radious = 5;
                        }

                        /* OGBUG  should use itr_cities, not itr_enemy_stack_count */
                        if(Delta_XY_With_Wrap(enemy_stack_wx, enemy_stack_wy, _CITIES[itr_enemy_stack_count].wx, _CITIES[itr_enemy_stack_count].wy, WORLD_WIDTH) <= scouting_radious)
                        {
                            highest_value = _ai_all_enemy_stacks[itr_enemy_stack_count].value;
                            target_wx = enemy_stack_wx;
                            target_wy = enemy_stack_wy;
                            target_wp = enemy_stack_wp;
                            stack_is_visible = ST_TRUE;
                        }
                    }
                }
            }

            if(stack_is_visible == ST_FALSE)
            {
                scouting_radious = 2;
                for(itr_cities = 0; itr_cities < _units; itr_cities++)
                {
                    if(stack_is_visible != ST_FALSE)
                    {
                        break;
                    }

                    if(_UNITS[itr_cities].owner_idx == player_idx)
                    {
                        if(_UNITS[itr_cities].wp == enemy_stack_wp)
                        {
                            /* OGBUG  should use itr_cities, not itr_enemy_stack_count */
                            if(Delta_XY_With_Wrap(enemy_stack_wx, enemy_stack_wy, _UNITS[itr_enemy_stack_count].wx, _UNITS[itr_enemy_stack_count].wy, WORLD_WIDTH) <= scouting_radious)
                            {
                                highest_value = _ai_all_enemy_stacks[itr_enemy_stack_count].value;
                                target_wx = enemy_stack_wx;
                                target_wy = enemy_stack_wy;
                                target_wp = enemy_stack_wp;
                                stack_is_visible = ST_TRUE;
                            }
                        }
                    }
                }
            }
        }
    }

    if(target_wx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_wx = target_wx;
        *targeted_wy = target_wy;
        *targeted_wp = target_wp;
        return ST_TRUE;
    }

}


// WZD o156p47
int16_t AITP_Floating_Island(int16_t player_idx, int16_t * targeted_wx, int16_t * targeted_wy, int16_t * targeted_wp)
{
    int16_t city_idx = 0;
    int16_t tries = 0;
    int16_t niu_player_idx = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t is_vertical_axis = 0;
    int16_t city_wp = 0;
    int16_t is_negative_offset = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t found_target = 0;
    int16_t itr_squares = 0;


    /* Phase 1: Init */
    niu_player_idx = player_idx;
    found_target = ST_FALSE;

    // ; make 200 attempts to find a random city that belongs to the player
    for(tries = 0; ((tries < 200) && (found_target == ST_FALSE)); tries++)
    {
        city_idx = (Random(_cities) - 1);
        if(_CITIES[city_idx].owner_idx == player_idx)
        {
            found_target = ST_TRUE;
            city_wp = _CITIES[city_idx].wp;
            city_wx = _CITIES[city_idx].wx;
            city_wy = _CITIES[city_idx].wy;
        }
    }

    if(found_target == ST_FALSE)
    {
        return ST_FALSE;
    }

    // ; find the closest ocean square within 10 squares in any of the cardinal directions (straight only)

    itr_squares = 0;
    is_negative_offset = ST_TRUE;
    is_vertical_axis = ST_FALSE;
    found_target = ST_FALSE;

    while((found_target == ST_FALSE) && (itr_squares < 10))
    {
        if(is_negative_offset == ST_FALSE)  /* +itr_squares : EAST / SOUTH (down-right) */
        {
            if(is_vertical_axis == ST_FALSE)  /* EAST  (city_wx + itr_squares, city_wy) */
            {
                if(Square_Is_Legal_For_Floating_Island((city_wx + itr_squares), city_wy, city_wp) == ST_TRUE)
                {
                    found_target = ST_TRUE;
                    target_wx = (city_wx + itr_squares);
                    target_wy = city_wy;
                }
                is_vertical_axis = ST_TRUE;
            }
            else  /* (is_vertical_axis == ST_TRUE) : SOUTH (city_wx, city_wy + itr_squares) */
            {
                if(Square_Is_Legal_For_Floating_Island(city_wx, (city_wy + itr_squares), city_wp) == ST_TRUE)
                {
                    found_target = ST_TRUE;
                    target_wx = city_wx;
                    target_wy = (city_wy + itr_squares);
                }
                is_vertical_axis = ST_FALSE;
                is_negative_offset = ST_TRUE;
            }
        }
        else  /* (is_negative_offset == ST_TRUE) : -itr_squares : WEST / NORTH (up-left) */
        {
            if(is_vertical_axis == ST_FALSE)  /* WEST  (city_wx - itr_squares, city_wy) */
            {
                if(Square_Is_Legal_For_Floating_Island((city_wx - itr_squares), city_wy, city_wp) == ST_TRUE)
                {
                    found_target = ST_TRUE;
                    target_wx = (city_wx - itr_squares);
                    target_wy = city_wy;
                }
                is_vertical_axis = ST_TRUE;
            }
            else  /* (is_vertical_axis == ST_TRUE) : NORTH (city_wx, city_wy - itr_squares) */
            {
                if(Square_Is_Legal_For_Floating_Island(city_wx, (city_wy - itr_squares), city_wp) == ST_TRUE)
                {
                    found_target = ST_TRUE;
                    target_wx = city_wx;
                    target_wy = (city_wy - itr_squares);
                }
                is_vertical_axis = ST_FALSE;
                itr_squares++;
                is_negative_offset = ST_FALSE;
            }
        }
    }

    if(found_target == ST_FALSE)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_wx = target_wx;
        *targeted_wy = target_wy;
        *targeted_wp = city_wp;
        return ST_TRUE;
    }

}


// WZD o156p48
int16_t AITP_Enchant_Road(int16_t player_idx, int16_t * targeted_wx, int16_t * targeted_wy, int16_t * targeted_wp)
{
    int16_t niu_player_index = 0;
    int16_t enchanted_road = 0;
    int16_t have_road = 0;
    int16_t strongest_garrison = 0;
    int16_t garrison_strength = 0;
    int16_t wy_offset = 0;
    int16_t wx_offset = 0;
    int16_t best_city_idx = 0;
    int16_t itr_cities = 0;

    niu_player_index = player_idx;
    best_city_idx = ST_UNDEFINED;
    strongest_garrison = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        garrison_strength = _ai_all_own_garrison_strengths[itr_cities];
        if(garrison_strength > strongest_garrison)
        {
            have_road = ST_FALSE;
            enchanted_road = ST_FALSE;
            for(wy_offset = -2; wy_offset < 3; wy_offset++)
            {
                for(wx_offset = -2; wx_offset < 3; wx_offset++)
                {
                    if(wy_offset == 0 && wx_offset == 0)
                    {
                        continue;
                    }
                    /* OGBUG  doesn't add wy_offset and wx_offset, so it only checks city center square */
                    if((GET_MAP_SQUARE_FLAG(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, _CITIES[itr_cities].wp) & MSF_ROAD) != 0)
                    {
                        have_road = ST_TRUE;
                        if((GET_MAP_SQUARE_FLAG(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, _CITIES[itr_cities].wp) & MSF_EROAD) != 0)
                        {
                            enchanted_road = ST_TRUE;
                        }
                    }
                }
            }
            if(have_road == ST_TRUE && enchanted_road == ST_FALSE)
            {
                best_city_idx = itr_cities;
                strongest_garrison = garrison_strength;
            }
        }
    }

    if(best_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_wx = _CITIES[best_city_idx].wx;
        *targeted_wy = _CITIES[best_city_idx].wy;
        *targeted_wp = _CITIES[best_city_idx].wp;
        return ST_TRUE;
    }

}

// WZD o156p49
int16_t AITP_Disenchant(int16_t player_idx, int16_t * targeted_wx, int16_t * targeted_wy, int16_t * targeted_wp)
{
    int16_t fortress_landmass_idx = 0;
    int16_t highest_cost_unit_idx = 0;
    int16_t target_city_idx = 0;
    int16_t target_node_idx = 0;
    int16_t highest_cost = 0;
    int16_t itr = 0;

    for(itr = 0; itr < _cities; itr++)
    {
        if(_CITIES[itr].owner_idx == HUMAN_PLAYER_IDX)
        {
            if(_CITIES[itr].enchantments[FLYING_FORTRESS] != 0)
            {
                if(Random(2) == 1)
                {
                    *targeted_wx = _CITIES[itr].wx;
                    *targeted_wy = _CITIES[itr].wy;
                    *targeted_wp = _CITIES[itr].wp;
                    return ST_TRUE;
                }
            }
        }
    }

    target_node_idx = ST_UNDEFINED;
    highest_cost = 0;
    for(itr = 0; itr < NUM_NODES; itr++)
    {
        if(_NODES[itr].owner_idx == player_idx)
        {
            if((_NODES[itr].flags & NF_WARPED) != 0)
            {
                if(_NODES[itr].power > highest_cost)
                {
                    target_node_idx = itr;
                    highest_cost = _NODES[itr].power;
                }
            }
        }
    }

    if(target_node_idx != ST_UNDEFINED)
    {
        *targeted_wx = _NODES[target_node_idx].wx;
        *targeted_wy = _NODES[target_node_idx].wy;
        *targeted_wp = _NODES[target_node_idx].wp;
        return ST_TRUE;
    }

    target_city_idx = ST_UNDEFINED;
    highest_cost = 0;
    for(itr = 0; itr < _cities; itr++)
    {
        if(_CITIES[itr].owner_idx == player_idx)
        {
            if(
                _CITIES[itr].enchantments[CURSED_LANDS] != 0
                ||
                _CITIES[itr].enchantments[FAMINE] != 0
                ||
                _CITIES[itr].enchantments[CHAOS_RIFT] != 0
                ||
                _CITIES[itr].enchantments[EVIL_PRESENCE] != 0
            )
            {
                if(_ai_all_own_city_values[itr] > highest_cost)
                {
                    target_city_idx = itr;
                    highest_cost = _ai_all_own_city_values[itr];
                }
            }
        }
    }

    if(target_city_idx != ST_UNDEFINED)
    {
        *targeted_wx = _CITIES[target_city_idx].wx;
        *targeted_wy = _CITIES[target_city_idx].wy;
        *targeted_wp = _CITIES[target_city_idx].wp;
        return ST_TRUE;
    }

    /* OGBUG  checks landmass without plane */
    fortress_landmass_idx = _landmasses[_FORTRESSES[player_idx].wp * WORLD_SIZE + _FORTRESSES[player_idx].wy * WORLD_WIDTH + _FORTRESSES[player_idx].wx];
    highest_cost_unit_idx = ST_UNDEFINED;
    highest_cost = 0;

    for(itr = 0; itr < _units; itr++)
    {
        if(_UNITS[itr].owner_idx == HUMAN_PLAYER_IDX)
        {
            if(_UNITS[itr].enchantments != 0)
            {
                if(_landmasses[_UNITS[itr].wp * WORLD_SIZE + _UNITS[itr].wy * WORLD_WIDTH + _UNITS[itr].wx] == fortress_landmass_idx)
                {
                    if(_unit_type_table[_UNITS[itr].type].cost > highest_cost)
                    {
                        highest_cost_unit_idx = itr;
                        highest_cost = _unit_type_table[_UNITS[itr].type].cost;
                    }
                }
            }
        }
    }

    if(highest_cost_unit_idx != ST_UNDEFINED)
    {
        /* OGBUG: The original code uses target_city_idx instead of highest_cost_unit_idx here.
         * Since target_city_idx is guaranteed to be -1 if we reached this block,
         * this results in an out-of-bounds read of _UNITS[-1]. */
        *targeted_wx = _UNITS[target_city_idx].wx;  /* OGBUG  OOB AVWL  target_city_idx is -1 */
        *targeted_wy = _UNITS[target_city_idx].wy;  /* OGBUG  OOB AVWL  target_city_idx is -1 */
        *targeted_wp = _UNITS[target_city_idx].wp;  /* OGBUG  OOB AVWL  target_city_idx is -1 */
        return ST_TRUE;
    }

    return ST_FALSE;
}


// WZD o156p50
int16_t AITP_Attack_Terrain(int16_t player_idx, int16_t * targeted_wx, int16_t * targeted_wy, int16_t * targeted_wp, int16_t spell_idx)
{
    int16_t highest_value = 0;
    int16_t target_city_idx = 0;
    int16_t wx_offset = 0;
    int16_t wy_offset = 0;
    int16_t found_target = 0;
    int16_t tries = 0;
    int16_t target_wx = 0;
    int16_t target_wy = 0;
    int16_t city_area_wy = 0;
    int16_t target_wp = 0;
    int16_t city_wp = 0;
    int16_t reject = 0;
    int16_t itr_cities = 0;
    int16_t city_area_wx = 0;

    target_city_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            continue;
        }

        if(_CITIES[itr_cities].owner_idx == NEUTRAL_PLAYER_IDX)
        {
            continue;
        }

        if(_ai_all_enemy_city_values[itr_cities] <= highest_value)
        {
            continue;
        }

        if(Test_Bit_Field(player_idx, &_CITIES[itr_cities].contacts) == 0)
        {
            continue;
        }

        city_wp = _CITIES[itr_cities].wp;
        tries = 0;
        found_target = 0;

        for(tries = 0; tries < 25 && found_target == 0; tries++)
        {
            wx_offset = Random(5) - 3;
            wy_offset = Random(5) - 3;

            /* Check diagonal corners (±2, ±2) */
            if((wx_offset == -2 && wy_offset == -2) ||
                (wx_offset == -2 && wy_offset == 2) ||
                (wx_offset == 2 && wy_offset == -2) ||
                (wx_offset == 2 && wy_offset == 2))
            {
                reject = ST_TRUE;
            }
            else
            {
                reject = ST_FALSE;
            }

            if(reject != ST_FALSE)
            {
                continue;
            }

            city_area_wx = _CITIES[itr_cities].wx + wx_offset;
            city_area_wy = _CITIES[itr_cities].wy + wy_offset;

            if(city_area_wx <= WORLD_XMIN || city_area_wx >= WORLD_WIDTH)
            {
                continue;
            }

            if(city_area_wy <= WORLD_YMIN || city_area_wy >= WORLD_HEIGHT)
            {
                continue;
            }

            if(Square_Is_Land(city_area_wx, city_area_wy, city_wp) != ST_TRUE)
            {
                continue;
            }

            if(Square_Is_Volcano(city_area_wx, city_area_wy, city_wp) != ST_FALSE)
            {
                continue;
            }

            if(Square_Has_Corruption(city_area_wx, city_area_wy, city_wp) != ST_FALSE)
            {
                continue;
            }

            if(spell_idx != spl_Corruption)
            {
                if(Square_Is_Mountain(city_area_wx, city_area_wy, city_wp) != ST_FALSE)
                {
                    continue;
                }
                if(Square_Is_Hills(city_area_wx, city_area_wy, city_wp) != ST_FALSE)
                {
                    continue;
                }
                if(Square_Is_River(city_area_wx, city_area_wy, city_wp) != ST_FALSE)
                {
                    continue;
                }
                if(Square_Is_Node(city_area_wx, city_area_wy, city_wp) != ST_FALSE)
                {
                    continue;
                }
            }

            target_city_idx = itr_cities;
            highest_value = _ai_all_enemy_city_values[itr_cities];
            target_wx = city_area_wx;
            target_wy = city_area_wy;
            target_wp = city_wp;
            found_target = ST_TRUE;
        }
    }

    if(target_city_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_wx = target_wx;
        *targeted_wy = target_wy;
        *targeted_wp = city_wp;  /* OGBUG  should return target_wp, not city_wp - last city_wp */
        return ST_TRUE;
    }
}


// WZD o156p51
// drake178: sub_E9FA9()


// WZD o156p52
int16_t AITP_Plane_Shift(int16_t player_idx, int16_t * targeted_wx, int16_t * targeted_wy, int16_t * targeted_wp)
{
    int highest_value;
    int stack_value;
    int itr_cities;
    int itr_stacks;
    int best_stack_idx;

    itr_stacks = player_idx;  /* OGBUG  pointless */

    best_stack_idx = ST_UNDEFINED;
    highest_value = 0;

    for(itr_stacks = 0; itr_stacks < _ai_all_own_stack_count; itr_stacks++)
    {

        stack_value = _ai_all_own_stacks[itr_stacks].value;

        if((_ai_all_own_stacks[itr_stacks].abilities & AICAP_Settler) == 0)  /* OGBUG  condition is inverted, only considers Settlers instead of never */
        {
            stack_value = 0;
        }

        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        {
            if(_ai_all_own_stacks[itr_stacks].wx == _CITIES[itr_cities].wx &&
                _ai_all_own_stacks[itr_stacks].wy == _CITIES[itr_cities].wy)
            {
                stack_value = 0;
            }
        }

        if(_ai_all_own_stacks[itr_stacks].wp == ARCANUS_PLANE)
        {
            if(stack_value > highest_value)
            {
                /* OGBUG  should test if square is occupieable */
                if((_ai_all_own_stacks[itr_stacks].abilities & AICAP_Transport) != 0)
                {
                    if(Square_Is_Sailable(_ai_all_own_stacks[itr_stacks].wx, _ai_all_own_stacks[itr_stacks].wy, MYRROR_PLANE) == ST_TRUE)
                    {
                        best_stack_idx = itr_stacks;
                        highest_value = stack_value;
                    }
                }
                else
                {
                    if(Square_Is_OceanLike(_ai_all_own_stacks[itr_stacks].wx, _ai_all_own_stacks[itr_stacks].wy, MYRROR_PLANE) == ST_FALSE)
                    {
                        best_stack_idx = itr_stacks;
                        highest_value = stack_value;
                    }
                }
            }
        }
        /* OGBUG  missing MYRROR_PLANE -> ARCANUS_PLANE block */
    }

    if(best_stack_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_wx = _ai_all_own_stacks[best_stack_idx].wx;
        *targeted_wy = _ai_all_own_stacks[best_stack_idx].wy;
        *targeted_wp = _ai_all_own_stacks[best_stack_idx].wp;
        return ST_TRUE;
    }

}


// WZD o156p53
int16_t AITP_Natures_Cures(int16_t player_idx, int16_t * targeted_wx, int16_t * targeted_wy, int16_t * targeted_wp)
{
    int16_t highest_damage = 0;
    int16_t unit_damage = 0;
    int16_t best_unit_idx = 0;
    int16_t itr_units = 0;

    best_unit_idx = ST_UNDEFINED;
    highest_damage = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == player_idx)
        {
            unit_damage = _UNITS[itr_units].Damage;

            if(unit_damage > highest_damage)
            {
                best_unit_idx = itr_units;
                highest_damage = unit_damage;
            }
        }
    }

    if(best_unit_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        *targeted_wx = _UNITS[best_unit_idx].wx;
        *targeted_wy = _UNITS[best_unit_idx].wy;
        *targeted_wp = _UNITS[best_unit_idx].wp;
        return ST_TRUE;
    }

}


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

    return AITP_Spell_Ward(player_idx, city_idx, magic_realm);

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
int16_t AITP_Summoning_Circle(int16_t player_idx, int16_t * city_idx)
{
    return 0;
}


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
