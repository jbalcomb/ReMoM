/*
    Combat

    WIZARDS.EXE
        ovr114  MoO2  Module: CMBTAI

*/

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"   /* LOG_CAT_COMBAT_TEST move-legality logging */

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players, _screen_seg */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/Timer.h"

#include "CMBMAGIC.h"
#include "CMBTDEF.h"
#include "CMBTMVPT.h"
#include "Combat.h"
#include "Spellbook.h"  /* spl_Doom_Bolt */
#include "Spells133.h"  /* Battle_Unit_Teleport, BU_Tunnel */
#include "UNITTYPE.h"   /* rag_Missile */

#include "CMBTAI.h"

#include <assert.h>
#include <stddef.h>  /* NULL */
#include <stdlib.h>  /* abs() */



// WZD dseg:C41E
extern int16_t _defender_sees_illusions;
extern int16_t _attacker_sees_illusions;
// WZD dseg:D142
extern int16_t movement_path_grid_cell_count;
// WZD dseg:7082
extern int16_t uu_combat_movement_variable;



// WZD dseg:C8AE                                                 BEGIN:  ovr114 - CMBTAI

// WZD dseg:C8AE
int16_t _ai_disable_hero_melee_safety_check;
// WZD dseg:C8B0
int16_t _ai_immobile_counter;

// WZD dseg:C8B2
/*
default true, set in combat init
false if enemy has high-powered ranged unit, Magic Vortex, Wrack, Call Lightning, or Mana Leak
*/
int16_t _ai_stay_in_city;

// WZD dseg:C8B4
/*
battlefield city wall bit-field
 0 0 0 0 0 0 0 0
           | | \- stone
           | \--- fire
           \----- darkness
*/
int16_t _ai_battlefield_city_walls;

// WZD dseg:C8B4                                                 END:  ovr114 - CMBTAI



/*
    WIZARDS.EXE  ovr114
*/

// WZD o114p01
/**
 * @brief Chooses each active unit's baseline tactical action for the current combat turn.
 *
 * This routine performs the combat AI's initial pass over one player's active battle units. It
 * assigns each unit a default attack mode based on whether the unit currently has a ranged attack,
 * then compares the overall threat, mana-backed ranged potential, and selected player posture
 * against the opposing side. Using those aggregated values, it upgrades some units from static
 * attack modes into movement-enabled tactics such as bua_AdvanceAndStab or bua_AdvanceAndShoot.
 *
 * The decision process runs in four stages: it initializes default unit actions and threat totals,
 * adds wizard ranged strength for both sides, computes supporting metrics such as ranged_diff and
 * own_melee_threat, and finally refines per-unit actions according to the safety level returned by
 * AI_Player_Mode().
 *
 * @param player_idx The combat controller index whose active units should receive tactical actions.
 *
 * @note The function updates battle_units[].action in place for all active units controlled by the
 *       specified player.
 * @note If either side has zero total effective threat, the routine exits early and leaves the
 *       default attack assignments in place.
 * @note Case 2 logic relies on own_melee_threat when comparing force ratios; the translated code
 *       preserves the surrounding assembly behavior and comments about that assumption.
 */
void AI_Set_All_Action_Modes(int16_t player_idx)
{
    int16_t enemy_player = 0;
    int16_t i = 0;
    int16_t own_ranged_capacity = 0;
    int16_t rnd_offset = 0;
    int16_t enemy_threat_total = 0;
    int16_t own_threat_total = 0;
    int16_t own_melee_threat = 0;
    int16_t ranged_diff = 0;
    int16_t player_mode_level = 0;
    int16_t enemy_mana_and_ranged = 0;
    own_ranged_capacity = 0;
    enemy_mana_and_ranged = 0;
    own_threat_total = 0;
    enemy_threat_total = 0;
    /* Phase 1: Determine initial action and calculate total threats */
    for(i = 0; i < _combat_total_unit_count; i++)
    {
        if(battle_units[i].status != bus_Active)
        {
            continue;
        }
        if(battle_units[i].controller_idx == player_idx)
        {
            if(Battle_Unit_Has_Ranged_Attack(i))
            {
                battle_units[i].action = bua_Shoot;
            }
            else
            {
                battle_units[i].action = bua_Stab;
            }
            own_threat_total += Effective_Battle_Unit_Strength(i);
            own_ranged_capacity += (battle_units[i].mana / 4);
        }
        else
        {
            enemy_threat_total += Effective_Battle_Unit_Strength(i);
            enemy_mana_and_ranged += (battle_units[i].mana / 4);
        }
    }
    /* Phase 2: Incorporate wizard ranged strengths and check early exit */
    own_ranged_capacity += Total_Ranged_Attack_Strength(player_idx);
    if(player_idx == _combat_attacker_player)
    {
        enemy_player = _combat_defender_player;
    }
    else
    {
        enemy_player = _combat_attacker_player;
    }
    enemy_mana_and_ranged += Total_Ranged_Attack_Strength(enemy_player);
    if(own_threat_total == 0 || enemy_threat_total == 0)
    {
        return;
    }
    /* Phase 3: Gather additional metrics for tactical decision */
    player_mode_level = AI_Player_Mode(player_idx);
    ranged_diff = own_ranged_capacity - enemy_mana_and_ranged;
    own_melee_threat = 0;
    for(i = 0; i < _combat_total_unit_count; i++)
    {
        if(battle_units[i].status == bus_Active && battle_units[i].controller_idx == player_idx)
        {
            if(battle_units[i].action == bua_Stab)
            {
                own_melee_threat += Effective_Battle_Unit_Strength(i);
            }
        }
    }
    rnd_offset = (Random(3) - 4); /* {-3,-2,-1} */
    /* Phase 4: Refine actions based on safety level and tactical balance */
    for(i = 0; i < _combat_total_unit_count; i++)
    {
        if(battle_units[i].status != bus_Active || battle_units[i].controller_idx != player_idx)
        {
            continue;
        }
        /* switch 4 cases */
        switch (player_mode_level)
        {
        case 0:
        case 1:
            if(battle_units[i].action == bua_Stab && 
                ranged_diff > rnd_offset && 
                own_ranged_capacity > 0)
            {
                battle_units[i].action = bua_AdvanceAndStab;
            }
            break;
        case 2:
            if(battle_units[i].action == bua_Stab)
            {
                /* Note: Assembly does not check for division by zero at loc_9239D, relying on logic that melee threat exists if this branch is relevant */
                // OGBUG: divide by zero
                if((enemy_threat_total / own_melee_threat) > 2 && 
                    ranged_diff > rnd_offset && 
                    own_ranged_capacity > 0)
                {
                    battle_units[i].action = bua_AdvanceAndStab;
                }
            }
            if(battle_units[i].action == bua_Shoot)
            {
                if(own_melee_threat != 0 && 
                    (enemy_threat_total / own_melee_threat) <= 2 && 
                    ranged_diff >= 0)
                {
                    if(_ai_battlefield_city_walls == 0 || battle_units[i].controller_idx == _combat_attacker_player)
                    {
                        battle_units[i].action = bua_AdvanceAndShoot;
                    }
                }
            }
            break;
        case 3:
            if(battle_units[i].action == bua_Stab)
            {
                if(ranged_diff > 0)
                {
                    battle_units[i].action = bua_AdvanceAndStab;
                }
            }
            else /* here, must be implicitly (battle_units[i].action == bua_Shoot), based on Phase 1 init */
            {
                if(ranged_diff < 0)
                {
                    battle_units[i].action = bua_AdvanceAndShoot;
                }
            }
            break;
        default:
            if(battle_units[i].action == bua_Shoot)
            {
                if(Random(2) == 1)  /* 50% */
                {
                    battle_units[i].action = bua_AdvanceAndShoot;
                }
            }
            break;
        }
    }
}


// WZD o114p02
void AI_Execute_Unit_Action(int16_t battle_unit_idx, int16_t rally_cgx, int16_t rally_cgy)
{
    int16_t proceed_with_melee = 0;  // if you came in with a target, but there is a wall, you might get an override
    int16_t spell_result = 0;
    int16_t some_variable = 0;
    int16_t target_idx = 0;
    struct s_BATTLE_UNIT * bu_ptr = NULL;
    struct s_BATTLE_UNIT * target_ptr = NULL;
    int16_t itr_battle_units = 0;  // DNE in Dasm
    /* If no stage point provided, use the target's current position */
    if(rally_cgx == 0 && rally_cgy == 0)
    {
        bu_ptr = &battle_units[battle_unit_idx];
        target_idx = bu_ptr->target_battle_unit_idx;
        target_ptr = &battle_units[target_idx];
        rally_cgx = target_ptr->cgx;
        rally_cgy = target_ptr->cgy;
    }
    Switch_Active_Battle_Unit(battle_unit_idx);
    _active_battle_unit = battle_unit_idx;
    bu_ptr = &battle_units[battle_unit_idx];
    /* Switch on BATTLE_UNIT_ACTION (starting at 100) */
    switch (bu_ptr->action)
    {
        case bua_Stab:
        {
            if(bu_ptr->melee != 0)
            {
                // if wall, ...
                if(_ai_battlefield_city_walls & BATTLEFIELD_CITY_WALL_STONE)
                {
                    if(bu_ptr->target_battle_unit_idx == ST_UNDEFINED)
                    {
                        proceed_with_melee = ST_FALSE;
                        // if I'm the attacker, I need to find the Gatekeeper
                        if(bu_ptr->controller_idx == _combat_attacker_player)
                        {
                            /* Attacker - Kill the Gatekeeper */
                            /* would try to find the defending unit at the gate, if any - but will never be executed because of the conflicting condition of the target having to be -1 and not being -1 at the same time */
                            /* OGBUG: The original assembly has a conflicting condition loc_925A7 that will never jump as written, likely intended to find a specific target */
                            for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                            {
                                target_ptr = &battle_units[itr_battle_units];
                                if(target_ptr->cgx == CGX_GATE && target_ptr->cgy == CGY_GATE && 
                                    target_ptr->controller_idx != bu_ptr->controller_idx &&
                                    target_ptr->status == bus_Active)
                                {
                                    bu_ptr->target_battle_unit_idx = (int8_t)itr_battle_units;
                                    proceed_with_melee = ST_TRUE;
                                }
                            }
                            if(proceed_with_melee == ST_FALSE)
                            {
                                /* Attempt to move toward central gate area (8,12) */
                                Do_Auto_Unit_Turn(battle_unit_idx, CGX_GATE, CGY_GATE, -666, CGX_GATE, CGY_GATE);
                            }
                        }
                        else
                        {
                            /* Defender - I am the Gatekeeper */
                            Do_Auto_Unit_Turn(battle_unit_idx, CGX_GATE, CGY_GATE, -666, CGX_GATE, CGY_GATE);
                        }
                    }
                    else
                    {
                        proceed_with_melee = ST_TRUE;  // no target, no override needed
                    }
                }
                else
                {
                    proceed_with_melee = ST_TRUE;  // no wall, no override needed
                }
                if(proceed_with_melee == ST_TRUE)
                {
                    /* Special check for neutral attacker/city capture context */
                    if(bu_ptr->target_battle_unit_idx == ST_UNDEFINED &&
                        bu_ptr->controller_idx == NEUTRAL_PLAYER_IDX &&
                        bu_ptr->controller_idx == _combat_attacker_player &&
                        _combat_environ == 1)  /* City-Siege */
                    {
                        Do_Auto_Unit_Turn(battle_unit_idx, 0, 0, ST_UNDEFINED, 0, 0);
                    }
                    else
                    {
                        /* Standard melee loop */
                        some_variable = 220; /* Sentinel for starting movement */
                        while(1)
                        {
                            bu_ptr = &battle_units[battle_unit_idx];
                            some_variable = bu_ptr->movement_points;
                            if(bu_ptr->target_battle_unit_idx < 0) break;
                            target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
                            Do_Auto_Unit_Turn(battle_unit_idx, target_ptr->cgx, target_ptr->cgy, bu_ptr->target_battle_unit_idx, rally_cgx, rally_cgy);
                            bu_ptr = &battle_units[battle_unit_idx];
                            target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
                            /* If target is no longer active, reassign action */
                            if(target_ptr->status != bus_Active)
                            {
                                AI_Set_Unit_Action_Mode(battle_unit_idx, 0);
                            }
                            bu_ptr = &battle_units[battle_unit_idx];
                            if(bu_ptr->movement_points >= some_variable) break;
                            if(bu_ptr->target_battle_unit_idx <= -1) break;
                            if(bu_ptr->movement_points <= 0) break;
                            if(bu_ptr->status != bus_Active) break;
                        }
                    }
                }
            }
            bu_ptr = &battle_units[battle_unit_idx];
            bu_ptr->action = bua_Finished;
        } break;
        case bua_AdvanceAndShoot:
        {
            bu_ptr = &battle_units[battle_unit_idx];
            if(bu_ptr->target_battle_unit_idx >= 0)
            {
                if(Range_To_Battle_Unit(battle_unit_idx, bu_ptr->target_battle_unit_idx) != 1)
                {
                    some_variable = bu_ptr->movement_points;
                    bu_ptr->movement_points = 1; /* Use exactly 1 MP to move closer */
                    target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
                    Do_Auto_Unit_Turn(battle_unit_idx, target_ptr->cgx, target_ptr->cgy, bu_ptr->target_battle_unit_idx, rally_cgx, rally_cgy);
                    /* Restore movement points minus cost of move */
                    bu_ptr = &battle_units[battle_unit_idx];
                    bu_ptr->movement_points = (some_variable + bu_ptr->movement_points - 1);
                }
                /* Shooting loop */
                while(1)
                {
                    Battle_Unit_Attack(battle_unit_idx, bu_ptr->target_battle_unit_idx, 0, 0);
                    bu_ptr = &battle_units[battle_unit_idx];
                    target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
                    if(target_ptr->status != bus_Active)
                    {
                        AI_Set_Unit_Action_Mode(battle_unit_idx, 0);
                    }
                    bu_ptr = &battle_units[battle_unit_idx];
                    if(bu_ptr->movement_points <= 0) break;
                    if(bu_ptr->target_battle_unit_idx <= -1) break;
                }
            }
            bu_ptr->action = bua_Finished;
        } break;
        case bua_Shoot:
        {
            bu_ptr = &battle_units[battle_unit_idx];
            if(bu_ptr->target_battle_unit_idx >= 0)
            {
                some_variable = 220;
                while(1)
                {
                    bu_ptr = &battle_units[battle_unit_idx];
                    some_variable = bu_ptr->movement_points;
                    if(bu_ptr->ranged_type == ST_UNDEFINED)
                    {
                        /* Unit has no ranged attack, try to move to target instead */
                        target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
                        Do_Auto_Unit_Turn(battle_unit_idx, target_ptr->cgx, target_ptr->cgy, bu_ptr->target_battle_unit_idx, rally_cgx, rally_cgy);
                    }
                    else
                    {
                        Battle_Unit_Attack(battle_unit_idx, bu_ptr->target_battle_unit_idx, 0, 0);
                    }
                    bu_ptr = &battle_units[battle_unit_idx];
                    target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
                    if(target_ptr->status != bus_Active)
                    {
                        AI_Set_Unit_Action_Mode(battle_unit_idx, 0);
                    }
                    bu_ptr = &battle_units[battle_unit_idx];
                    if(bu_ptr->movement_points >= some_variable) break;
                    if(bu_ptr->target_battle_unit_idx <= -1) break;
                    if(bu_ptr->movement_points <= 0) break;
                    if(bu_ptr->status != bus_Active) break;
                }
            }
            bu_ptr->action = bua_Finished;
        } break;
        case bua_AdvanceAndStab:
        {
            bu_ptr = &battle_units[battle_unit_idx];
            if(bu_ptr->target_battle_unit_idx >= 0)
            {
                target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
                Do_Auto_Unit_Turn(battle_unit_idx, target_ptr->cgx, target_ptr->cgy, bu_ptr->target_battle_unit_idx, rally_cgx, rally_cgy);
                bu_ptr = &battle_units[battle_unit_idx];
                target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
                if(target_ptr->status != bus_Active)
                {
                    AI_Set_Unit_Action_Mode(battle_unit_idx, 0);
                }
            }
            bu_ptr->action = bua_Finished;
        } break;
        case bua_DoomBolt:
        {
            bu_ptr->Attribs_2 ^= USA_DOOMBOLT;
            target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
            Combat_Cast_Apply_Spell_Effect(spl_Doom_Bolt, bu_ptr->target_battle_unit_idx, battle_unit_idx, target_ptr->cgx, target_ptr->cgy, 0, 1, ST_NULL, ST_NULL);
            bu_ptr->movement_points = 0;
        } break;
        case bua_Fireball:
        {
            bu_ptr->Attribs_2 ^= USA_FIREBALL;
            target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
            Combat_Cast_Apply_Spell_Effect(spl_Fireball, bu_ptr->target_battle_unit_idx, battle_unit_idx, target_ptr->cgx, target_ptr->cgy, 0, 1, ST_NULL, ST_NULL);
            bu_ptr->movement_points = 0;
        } break;
        case bua_Healing:
        {
            /* OGBUG: Choose_Target_And_Action() assigns bua_Healing, but AI_Execute_Unit_Action() doesn't have a case to handle it. */
            STU_DEBUG_BREAK();  /* DNE in Dasm */
        } break;
        case bua_UseItem:
        case bua_CastSpell:
        {
            spell_result = Combat_Cast_Spell(battle_unit_idx, _combat_wx, _combat_wy, _combat_wp);
            if(spell_result == ST_TRUE)
            {
                /* Post-spell reassignment and movement logic */
                AI_Set_Unit_Action_Mode(battle_unit_idx, 1);
                bu_ptr = &battle_units[battle_unit_idx];
                if(bu_ptr->target_battle_unit_idx >= 0)
                {
                    some_variable = 220;
                    while(1)
                    {
                        bu_ptr = &battle_units[battle_unit_idx];
                        some_variable = bu_ptr->movement_points;
                        if(bu_ptr->ranged_type != rat_NONE)
                        {
                            Battle_Unit_Attack(battle_unit_idx, bu_ptr->target_battle_unit_idx, 0, 0);
                        }
                        else
                        {
                            target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
                            Do_Auto_Unit_Turn(battle_unit_idx, target_ptr->cgx, target_ptr->cgy, bu_ptr->target_battle_unit_idx, rally_cgx, rally_cgy);
                        }
                        bu_ptr = &battle_units[battle_unit_idx];
                        target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
                        if(target_ptr->status != bus_Active)
                        {
                            AI_Set_Unit_Action_Mode(battle_unit_idx, 0);
                        }
                        bu_ptr = &battle_units[battle_unit_idx];
                        if(bu_ptr->movement_points >= some_variable) break;
                        if(bu_ptr->target_battle_unit_idx <= -1) break;
                        if(bu_ptr->movement_points <= 0) break;
                        /* Note: check for bus_Active missing in this specific branch in asm */
                    }
                }
            }
            else
            {
                bu_ptr->movement_points = 0;
            }
            bu_ptr = &battle_units[battle_unit_idx];
            bu_ptr->action = bua_Finished;
        } break;
        case bua_SummonDemon:
        {
            Summon_Demon(battle_unit_idx);
            bu_ptr->movement_points = 0;
        } break;
        case bua_WebSpell:
        {
            bu_ptr->Attribs_2 ^= USA_WEB;
            target_ptr = &battle_units[bu_ptr->target_battle_unit_idx];
            Combat_Cast_Apply_Spell_Effect(spl_Web, bu_ptr->target_battle_unit_idx, battle_unit_idx, target_ptr->cgx, target_ptr->cgy, 0, 1, ST_NULL, ST_NULL);
            bu_ptr->movement_points = 0;
        } break;
        default:
        {
            STU_DEBUG_BREAK();  /* Unhandled action mode */
        } break;
    }
    /* Finished processing specific action */
    /* Default handler / Exit clamping */
    if(bu_ptr->movement_points < 0)
    {
        bu_ptr->movement_points = 0;
    }
}


// WZD o114p03
/* The function implements an Insertion Sort to order units by movement points (slowest first) */
/*
Sort_Battle_Units is an insertion sort that orders a list of battle unit indices by their movement_points, from least to most (slowest first).
Purpose in context: It's called by Auto_Do_Combat_Turn before the AI moves its melee units. By sorting slowest-first, the AI moves the least-mobile units first. This matters because the stage point (where units converge) is derived from the last element in the sorted list — the fastest unit. Slow units move toward where the fast unit will end up, and the fast unit goes last so it can adapt to where everyone else already is.
Algorithm: Standard insertion sort. It walks the list starting at index 1, and for each element, shifts larger-valued entries rightward until it finds the correct insertion point.
*/
void Sort_Battle_Units(int16_t * troop_list, int16_t troop_count)
{
    int16_t current_battle_unit_moves = 0;
    int16_t previous_battle_unit_moves = 0;
    int16_t itr = 0;
    int16_t current_battle_unit_idx = 0;
    int16_t previous_list_idx = 0;

    for(itr = 1; itr < troop_count; itr++)
    {
        current_battle_unit_idx = troop_list[itr];
        
        previous_list_idx = itr - 1;

        /* Load movement points for the unit at the previous position */
        previous_battle_unit_moves = battle_units[troop_list[previous_list_idx]].movement_points;

        /* Load movement points for the current unit being inserted */
        current_battle_unit_moves = battle_units[current_battle_unit_idx].movement_points;

        /* Shift elements of troop_list[0..itr-1] that are greater than current_battle_unit_moves */
        while(previous_list_idx >= 0 && previous_battle_unit_moves > current_battle_unit_moves)
        {
            troop_list[previous_list_idx + 1] = troop_list[previous_list_idx];
            
            previous_list_idx--;

            if(previous_list_idx >= 0)
            {
                /* Update previous_battle_unit_moves for the next comparison */
                previous_battle_unit_moves = battle_units[troop_list[previous_list_idx]].movement_points;
            }
        }

        /* Place the current unit in its sorted position */
        troop_list[previous_list_idx + 1] = current_battle_unit_idx;
    }
}


// WZD o114p04
void AI_Stage_Point_For_Unit(int16_t battle_unit_idx, int16_t * cgx, int16_t * cgy)
{
    int16_t itr = 0;
    int16_t target_idx = 0;
    int16_t cost_map_offset = 0;
    uint8_t cost = 0;
    int16_t move_points = 0;
    /* Check if the unit is already executing a direct attack action */
    if(
        battle_units[battle_unit_idx].action == bua_AdvanceAndStab
        || 
        battle_units[battle_unit_idx].action == bua_Shoot)
    {
        *cgx = battle_units[battle_unit_idx].cgx;
        *cgy = battle_units[battle_unit_idx].cgy;
        return;
    }
    /* Initialize the base movement cost map based on terrain */
    Set_Movement_Cost_Map(battle_unit_idx);
    /* Update cost map with unit positions */
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        /* If this unit is our target, mark it as reachable (cost 2) */
        if(battle_units[battle_unit_idx].target_battle_unit_idx == itr)
        {
            cost_map_offset = (battle_units[itr].cgy * COMBAT_GRID_WIDTH) + battle_units[itr].cgx;
            _cmbt_movepath_cost_map[cost_map_offset] = 2;
        }
        /* Otherwise, if the unit is active, it is an obstacle */
        else if(battle_units[itr].status == bus_Active)
        {
            cost_map_offset = (battle_units[itr].cgy * COMBAT_GRID_WIDTH) + battle_units[itr].cgx;
            _cmbt_movepath_cost_map[cost_map_offset] = INF;
        }
    }
    /* Update cost map with magic vortex positions as obstacles */
    for(itr = 0; itr < _vortex_count; itr++)
    {
        cost_map_offset = (_vortexes[itr].cgy * COMBAT_GRID_WIDTH) + _vortexes[itr].cgx;
        _cmbt_movepath_cost_map[cost_map_offset] = INF;
    }
    /* Calculate the path to the current target unit */
    target_idx = battle_units[battle_unit_idx].target_battle_unit_idx;
    /*
        OGBUG: OOB AVRL  If the target_battle_unit_idx is ST_UNDEFINED (-1), the function will attempt to compute a path to an invalid target, which can lead to out-of-bounds access in the cost map and movement path arrays, causing a crash.
                The root cause is higher up: Auto_Do_Combat_Turn() computes a stage point before AI_Set_Unit_Action_Mode() assigns fresh targets, so the chosen melee unit can still have target_battle_unit_idx == ST_UNDEFINED.
    */
    if(target_idx == ST_UNDEFINED)
    {
        *cgx = 0;
        *cgy = 0;
        return;
    }
    Combat_Move_Path_Find(
        battle_units[battle_unit_idx].cgx,
        battle_units[battle_unit_idx].cgy,
        battle_units[target_idx].cgx,
        battle_units[target_idx].cgy
    );
    /* Iterate through path to find the furthest point reachable with current movement points */
    move_points = battle_units[battle_unit_idx].movement_points;
    for(itr = 0; itr < movement_path_grid_cell_count; itr++)
    {
        /* Skip starting square */
        if(itr == 0)
        {
            continue;
        }
        /* Check cost at the path coordinate (itr-1 because arrays are 0-indexed and loop uses dec) */
        cost_map_offset = (_cmbt_mvpth_y[itr - 1] * COMBAT_GRID_WIDTH) + _cmbt_mvpth_x[itr - 1];
        cost = _cmbt_mvpth_c[cost_map_offset];
        /* If movement points are exhausted by this step, stop */
        if(move_points - cost <= 0)
        {
            break;
        }
    }
    /* Set the stage point to the last valid cell found in the path */
    *cgx = _cmbt_mvpth_x[itr - 1];
    *cgy = _cmbt_mvpth_y[itr - 1];
}


// WZD o114p05
/**
 * @brief Assigns and executes combat-turn actions for all battle units controlled by one player.
 *
 * This routine drives the high-level tactical AI for a single side during combat. It first
 * handles a special-case cleanup for stuck neutral attackers, then compares allied and enemy
 * ranged strength to decide whether melee units should coordinate around a stage point or simply
 * advance opportunistically. It also updates the effective city-wall state used by downstream AI
 * logic, including conditions where defenders abandon the safety of city walls because of hostile
 * ranged superiority or active battlefield hazards such as vortexes, Wrack, Call Lightning, or
 * Mana Leak.
 *
 * After the global combat posture is established, the function initializes basic unit actions via
 * AI_Set_All_Action_Modes(), optionally builds and sorts a list of melee-capable units to compute a
 * stage point with AI_Stage_Point_For_Unit(), and then processes units in two passes. The first pass
 * handles active non-sleeping units generally, while the second pass revisits hero units under the
 * special hero melee safety rules controlled by _ai_disable_hero_melee_safety_check.
 *
 * @param player_idx The combat controller index whose battle units should take their turn.
 *
 * @note The stage point is only computed when this side's ranged strength is at least as strong as
 *       the opposing side's ranged strength.
 * @note The function may clear _ai_battlefield_city_walls and force _ai_stay_in_city to false
 *       when the defender should stop sheltering behind city defenses.
 * @note Processing can terminate early if _combat_winner becomes defined while units are acting.
 */
void Auto_Do_Combat_Turn(int16_t player_idx)
{
    int16_t melee_unit_list[MAX_BATTLE_UNIT_SLOT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t attacker_vortex_count = 0;
    int16_t their_ranged_attack_strength = 0;
    int16_t our_ranged_attack_strength = 0;
    int16_t rally_y = 0;
    int16_t rally_x = 0;
    int16_t melee_unit_count = 0;
    int16_t itr_battle_units = 0;
    int16_t itr_vortexes = 0;
    struct s_MAGIC_VORTEX * vortex_ptr = NULL;
    struct s_BATTLE_UNIT * bu_ptr = NULL;
    our_ranged_attack_strength = 0;
    their_ranged_attack_strength = 0;
    /* Handle immobile neutrals if neutral player is the attacker and getting stuck */
    if(player_idx == NEUTRAL_PLAYER_IDX && 
        player_idx == _combat_attacker_player && 
        _ai_immobile_counter > 3)
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            bu_ptr = &battle_units[itr_battle_units];
            if(bu_ptr->status == bus_Active && bu_ptr->controller_idx == NEUTRAL_PLAYER_IDX)
            {
                bu_ptr->status = bus_Dead;
            }
        }
    }
    /* Calculate relative ranged strength */
    our_ranged_attack_strength += Total_Ranged_Attack_Strength(player_idx);
    if(player_idx == _combat_attacker_player)
    {
        their_ranged_attack_strength += Total_Ranged_Attack_Strength(_combat_defender_player);
    }
    else
    {
        their_ranged_attack_strength += Total_Ranged_Attack_Strength(_combat_attacker_player);
    }
    /* Determine active battlefield obstacles (walls) */
    _ai_battlefield_city_walls = 0;
    if(battlefield->walled == ST_TRUE)
    {
        _ai_battlefield_city_walls |= BATTLEFIELD_CITY_WALL_STONE;
    }
    if(battlefield->wall_of_darkness == ST_TRUE)
    {
        _ai_battlefield_city_walls |= BATTLEFIELD_CITY_WALL_DARKNESS;
    }
    if(battlefield->wall_of_fire == ST_TRUE)
    {
        _ai_battlefield_city_walls |= BATTLEFIELD_CITY_WALL_FIRE;
    }
    /* AI Strategy: Should defender stay behind walls? */
    if(player_idx == _combat_defender_player)
    {
        if(_ai_stay_in_city == ST_TRUE)
        {
            attacker_vortex_count = 0;
            if(_vortex_count > 0)
            {
                for(itr_vortexes = 0; itr_vortexes < _vortex_count; itr_vortexes++)
                {
                    vortex_ptr = &_vortexes[itr_vortexes];
                    if(vortex_ptr->owner_idx == _combat_attacker_player)
                    {
                        attacker_vortex_count++;
                    }
                }
            }
            /* Abandon walls if attacker has massive ranged superiority and we have none */
            if(their_ranged_attack_strength > 30 && our_ranged_attack_strength == 0)
            {
                _ai_battlefield_city_walls = 0;
                _ai_stay_in_city = ST_FALSE;
            }
            /* Abandon walls if dangerous environmental spells are active */
            if(attacker_vortex_count > 0 || 
                combat_enchantments[WRACK_ATTKR] > 0 || 
                combat_enchantments[CALL_LIGHTNING_ATTKR] > 0 || 
                (combat_enchantments[MANA_LEAK_ATTKR] > 0 && player_idx < _num_players))
            {
                _ai_battlefield_city_walls = 0;
                _ai_stay_in_city = ST_FALSE;
            }
        }
        else
        {
            _ai_battlefield_city_walls = 0;
        }
    }
    _combat_winner = ST_UNDEFINED;
    /* Initialize attack parameters */
    AI_Set_All_Action_Modes(player_idx);
    /* Determine Rally Point */
    if(our_ranged_attack_strength >= their_ranged_attack_strength)
    {
        melee_unit_count = 0;
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            bu_ptr = &battle_units[itr_battle_units];
            if(bu_ptr->controller_idx == player_idx && bu_ptr->status == bus_Active)
            {
                if(bu_ptr->action == bua_Stab || bu_ptr->action == bua_AdvanceAndStab)
                {
                    melee_unit_list[melee_unit_count] = itr_battle_units;
                    melee_unit_count++;
                }
            }
        }
        Sort_Battle_Units(melee_unit_list, melee_unit_count);
        /* Get stage point based on the median speed melee unit's destination */
        AI_Stage_Point_For_Unit(melee_unit_list[(melee_unit_count - 1) / 2], &rally_x, &rally_y);
    }
    else
    {
        rally_x = 0;
        rally_y = 0;
    }
    _ai_disable_hero_melee_safety_check = ST_FALSE;
    /* Pass 1: Process non-hero units (or all units initially) */
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        bu_ptr = &battle_units[itr_battle_units];
        if(bu_ptr->controller_idx == player_idx && 
            bu_ptr->status == bus_Active && 
            bu_ptr->movement_points > 0 && 
            !(bu_ptr->combat_effects & bue_Black_Sleep))
        {
            Switch_Active_Battle_Unit(itr_battle_units);
            Assign_Combat_Grids();
            AI_Set_Unit_Action_Mode(itr_battle_units, bua_Ready);
            /* If defending within walls, units outside must stage differently */
            if(
                bu_ptr->controller_idx == _combat_defender_player  /* whoever's turn it is, check that it's the defenders turn */
                &&
                _ai_battlefield_city_walls > 0  /* there is a wall of some sort */
                &&
                !Battle_Unit_Is_Within_City(itr_battle_units)  /* unit is outside the city */
            )
            {
                AI_Execute_Unit_Action(itr_battle_units, 0, 0);  /* "use the target enemy's position as the stage point instead." */
            }
            else
            {
                AI_Execute_Unit_Action(itr_battle_units, rally_x, rally_y);  /* in a city with a wall, so behave defensively */
            }
        }
        // OGBUG: this is not set anywhere - Battle_Unit_Commit_Damage does not set it at the right time either
        if(_combat_winner != ST_UNDEFINED) { break; }
    }
    /* Pass 2: Specific logic for Hero units */
    if(_ai_immobile_counter != ST_UNDEFINED) { _ai_disable_hero_melee_safety_check = ST_TRUE; }
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        bu_ptr = &battle_units[itr_battle_units];
        if(bu_ptr->controller_idx == player_idx && bu_ptr->status == bus_Active &&  bu_ptr->movement_points > 0)
        {
            /* Check if the unit is a hero */
            if(_UNITS[bu_ptr->unit_idx].Hero_Slot > -1 && !(bu_ptr->combat_effects & bue_Black_Sleep))
            {
                Switch_Active_Battle_Unit(itr_battle_units);
                Assign_Combat_Grids();
                AI_Set_Unit_Action_Mode(itr_battle_units, bua_Ready);
                AI_Execute_Unit_Action(itr_battle_units, rally_x, rally_y);
            }
        }
        // OGBUG: this is not set anywhere - Battle_Unit_Commit_Damage does not set it at the right time either
        if(_combat_winner != ST_UNDEFINED) { break; }
    }
}


// WZD o114p06
/**
 * @brief Selects and records the tactical action and target for an AI battle unit.
 *
 * The function first rejects inactive units, then delegates primary action and target selection
 * to Choose_Target_And_Action(). Ranged units may be reconsidered as melee units when their
 * selected target is immune to missiles and melee is at least as promising as ranged combat.
 * For ordinary melee and ranged actions, the selected target is subsequently refined by choosing
 * a closer enemy of the same unit type and defense rating, or by following a suitable target
 * already selected by an earlier friendly melee unit.
 *
 * @param battle_unit_idx Index of the battle unit in battle_units.
 * @param no_spells_flag If ST_TRUE, prevents normal spell selection by starting with bua_No_Spells;
 *        otherwise action selection starts with bua_Ready.
 * @return The selected target battle-unit index, or ST_UNDEFINED when the unit is inactive or no
 *         target is selected. Special actions may return a non-unit sentinel such as 99.
 *
 * @note Updates battle_units[battle_unit_idx].action and
 *       battle_units[battle_unit_idx].target_battle_unit_idx in place.
 * @note Target refinement is skipped for attacking units while city-wall state is active, but is
 *       available to defenders and to units when _ai_battlefield_city_walls is ST_FALSE.
 * @see Choose_Target_And_Action()
 * @see Battle_Unit_Has_Ranged_Attack()
 * @see Target_Unit_Value()
 */
int16_t AI_Set_Unit_Action_Mode(int16_t battle_unit_idx, int16_t no_spells_flag)
{
    int16_t player_idx = 0;
    int16_t has_ranged_attack = 0;
    int16_t selected_action = 0;
    int16_t target_battle_unit_idx = 0;
    int16_t itr = 0;
    struct s_BATTLE_UNIT * bu_ptr = NULL;
    struct s_BATTLE_UNIT * target_ptr = NULL;
    struct s_BATTLE_UNIT * itr_bu_ptr = NULL;
    struct s_BATTLE_UNIT * itr_target_ptr = NULL;
    bu_ptr = &battle_units[battle_unit_idx];
    /* Check if unit is active */
    if(bu_ptr->status != bus_Active)
    {
        return ST_UNDEFINED;
    }
    /* Set initial action state based on spellcasting permission */
    if(no_spells_flag == ST_TRUE)
    {
        selected_action = bua_No_Spells;
    }
    else
    {
        selected_action = bua_Ready;
    }
    player_idx = bu_ptr->controller_idx;
    target_battle_unit_idx = ST_UNDEFINED;
    has_ranged_attack = Battle_Unit_Has_Ranged_Attack(battle_unit_idx);
    /* Primary action selection logic (WIP) */
    /* Choose_Target_And_Action updates selected_action via pointer and returns a target index */
    target_battle_unit_idx = Choose_Target_And_Action(battle_unit_idx, &selected_action, has_ranged_attack);
    /* Missile Immunity vs. Melee trade-off logic */
    if(target_battle_unit_idx > ST_UNDEFINED && has_ranged_attack == ST_TRUE)
    {
        /* Check if ranged attack is a physical missile type */
        if((bu_ptr->ranged_type / 10) == rag_Missile) 
        {
            target_ptr = &battle_units[target_battle_unit_idx];
            /* If target is immune to missiles, check if melee is a viable fallback */
            if(target_ptr->Attribs_1 & USA_IMMUNITY_MISSILES) /*  */
            {
                /* If melee potential (melee * 3) is better than ranged (ranged * 2), abandon ranged target */
                if((bu_ptr->melee * 3) >= (bu_ptr->ranged * 2))
                {
                    target_battle_unit_idx = ST_UNDEFINED;
                }
            }
        }
    }
    /* Fallback to melee search if ranged was unsuccessful or rejected */
    if(has_ranged_attack == ST_TRUE && target_battle_unit_idx == ST_UNDEFINED)
    {
        if(selected_action == bua_Ready || selected_action == bua_No_Spells || selected_action == bua_Shoot || selected_action == bua_AdvanceAndShoot)
        {
            /* bua_Ready, bua_No_Spells, bua_Shoot, bua_AdvanceAndShoot */
            has_ranged_attack = ST_FALSE;
            target_battle_unit_idx = Choose_Target_And_Action(battle_unit_idx, &selected_action, has_ranged_attack);
            /* If the AI returned a ranged action despite has_ranged_attack being FALSE, force melee */
            if(selected_action == bua_Shoot || selected_action == bua_AdvanceAndShoot)
            {
                selected_action = bua_Stab;
            }
        }
    }
    /* Assign the final action to the battle unit record */
    if(selected_action == bua_Ready || selected_action == bua_No_Spells)
    {
        if(has_ranged_attack == ST_TRUE)
        {
            bu_ptr->action = bua_Shoot;
        }
        else
        {
            bu_ptr->action = bua_Stab;
        }
    }
    else
    {
        bu_ptr->action = selected_action;
    }
    /* Target Refinement Loop */
    if(bu_ptr->action == bua_Shoot || bu_ptr->action == bua_Stab)
    {
        /* Attackers skip optimization if city walls are present */
        if(bu_ptr->controller_idx == _combat_defender_player || _ai_battlefield_city_walls == ST_FALSE)
        {
            if(target_battle_unit_idx > ST_UNDEFINED && target_battle_unit_idx < MAX_BATTLE_UNIT_SLOT_COUNT)
            {
                /* 1. Look for a closer target of the identical unit type and defense rating */
                target_ptr = &battle_units[target_battle_unit_idx];
                for(itr = 0; itr < _combat_total_unit_count; itr++)
                {
                    itr_bu_ptr = &battle_units[itr];
                    if(itr_bu_ptr->status == bus_Active && itr_bu_ptr->controller_idx != bu_ptr->controller_idx)
                    {
                        if(_UNITS[itr_bu_ptr->unit_idx].type == _UNITS[target_ptr->unit_idx].type &&
                            itr_bu_ptr->defense == target_ptr->defense)
                        {
                            if(Range_To_Battle_Unit(battle_unit_idx, itr) < Range_To_Battle_Unit(battle_unit_idx, target_battle_unit_idx))
                            {
                                target_battle_unit_idx = itr;
                                target_ptr = &battle_units[target_battle_unit_idx];
                            }
                        }
                    }
                }
            }
            /* 2. Focus fire logic: attempt to follow the lead of previous units in the list */
            if(has_ranged_attack == ST_FALSE)
            {
                for(itr = 0; itr < battle_unit_idx; itr++)
                {
                    itr_bu_ptr = &battle_units[itr];
                    /* If a previous friendly unit is targeting the same unit type with same defense, switch to it */
                    if(itr_bu_ptr->status != bus_Active)
                    {
                        continue;
                    }
                    if(itr_bu_ptr->controller_idx != bu_ptr->controller_idx)
                    {
                        continue;
                    }
                    /* OGBUG: battle_units[battle_unit_idx].target_battle_unit_idx can be ST_UNDEFINED which causes OoB when indexing battle_units[] */
                    /* OGBUG: */  if(itr_bu_ptr->target_battle_unit_idx == ST_UNDEFINED)
                    /* OGBUG: */  {
                    /* OGBUG: */      continue;
                    /* OGBUG: */  }
                    /* OGBUG: target_battle_unit_idx can be ST_UNDEFINED which causes OoB when indexing battle_units[] */
                    /* OGBUG: */  if(target_battle_unit_idx == ST_UNDEFINED)
                    /* OGBUG: */  {
                    /* OGBUG: */      continue;
                    /* OGBUG: */  }
                    itr_target_ptr = &battle_units[itr_bu_ptr->target_battle_unit_idx];
                    target_ptr = &battle_units[target_battle_unit_idx];
                    if(_UNITS[itr_target_ptr->unit_idx].type != _UNITS[target_ptr->unit_idx].type)
                    {
                        continue;
                    }
                    if(itr_target_ptr->status != bus_Active)
                    {
                        continue;
                    }
                    if(itr_target_ptr->defense != target_ptr->defense)
                    {
                        continue;
                    }
                    target_battle_unit_idx = itr_bu_ptr->target_battle_unit_idx;
                    target_ptr = &battle_units[target_battle_unit_idx];
                }
            }
        }
    }
    /* Store the refined target */
    bu_ptr->target_battle_unit_idx = (int8_t)target_battle_unit_idx;
    return target_battle_unit_idx;
}


// WZD o114p07
int16_t Choose_Target_And_Action(int16_t battle_unit_idx, int16_t * selected_action, int16_t has_ranged_attack)
{
    int16_t target_y = 0;
    int16_t target_x = 0;
    int16_t spell_idx = 0;
    int16_t ability_chosen = 0;
    int16_t item_spell_idx = 0;
    int16_t target_value = 0;
    int16_t player_unit_count = 0;
    int16_t target_idx = 0;
    int16_t highest_value = 0;
    int16_t target_health = 0;
    int16_t total_health = 0;
    int16_t hero_slot_idx = 0;
    int16_t itr_battle_units = 0;
    struct s_ITEM * item_ptr = NULL;
    struct s_UNIT * unit_ptr = NULL;
    struct s_BATTLE_UNIT * target_bu_ptr = NULL;
    struct s_BATTLE_UNIT * bu_ptr = NULL;
    highest_value = -100;
    target_idx = ST_UNDEFINED;
    ability_chosen = ST_FALSE;
    if(_combat_winner != ST_UNDEFINED)
    {
        return ST_UNDEFINED;
    }
    bu_ptr = &battle_units[battle_unit_idx];
    /* Summon Demon check */
    if((bu_ptr->Attribs_1 & (USA_SUMMON_DEMON_1 | USA_SUMMON_DEMON_2)) != 0)
    {
        if(_combat_total_unit_count < MAX_BATTLE_UNIT_SLOT_COUNT && _units < MAX_UNIT_COUNT)
        {
            player_unit_count = 0;
            for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            {
                if(battle_units[itr_battle_units].controller_idx == bu_ptr->controller_idx && battle_units[itr_battle_units].status == bus_Active)
                {
                    player_unit_count++;
                }
            }
            if(player_unit_count < MAX_STACK)
            {
                *selected_action = bua_SummonDemon;
                ability_chosen = ST_TRUE;
                target_idx = 99;
            }
        }
    }
    /* Doom Bolt check */
    if(
        ((bu_ptr->Attribs_2 & USA_DOOMBOLT) != 0)
        &&
        (ability_chosen == ST_FALSE)
    )
    {
        spell_idx = spl_Doom_Bolt;
        target_idx = AITP_Combat_Spell(spell_idx, bu_ptr->controller_idx, &target_x, &target_y);
        if(target_idx > ST_UNDEFINED)
        {
            highest_value = 30;
            *selected_action = bua_DoomBolt;
            /* Note: ability_chosen is NOT set to TRUE here in asm, but usually implies selection */
        }
    }
    /* Fireball check */
    if(
        ((bu_ptr->Attribs_2 & USA_FIREBALL) != 0)
        &&
        (ability_chosen == ST_FALSE)
    )
    {
        spell_idx = spl_Fireball;
        target_idx = AITP_Combat_Spell(spell_idx, bu_ptr->controller_idx, &target_x, &target_y);
        if(target_idx > ST_UNDEFINED)
        {
            highest_value = 16 - Battle_Unit_Defense_Special(target_idx, rat_Fireball, (USA_IMMUNITY_FIRE | USA_IMMUNITY_MAGIC), 0, sbr_Chaos);
            *selected_action = bua_Fireball;
        }
    }
    /* Healing check */
    if(
        ((bu_ptr->Attribs_2 & USA_HEALING) != 0)
        &&
        (ability_chosen == ST_FALSE)
    )
    {
        spell_idx = spl_Healing;
        target_idx = AITP_Combat_Spell(spell_idx, bu_ptr->controller_idx, &target_x, &target_y);
        if(target_idx > ST_UNDEFINED)
        {
            ability_chosen = ST_TRUE;
            bu_ptr->Attribs_2 ^= USA_HEALING; /* This has the same effect of toggling the healing bit, but is more semantically clear and preserves other bits */
            *selected_action = bua_Healing;
        }
    }
    /* Web check */
    if(
        ((bu_ptr->Attribs_2 & USA_WEB) != 0)
        &&
        (ability_chosen == ST_FALSE)
    )
    {
        spell_idx = spl_Web;
        target_idx = AITP_Combat_Spell(spell_idx, bu_ptr->controller_idx, &target_x, &target_y);
        if(target_idx > ST_UNDEFINED)
        {
            ability_chosen = ST_TRUE;
            *selected_action = bua_WebSpell;
        }
    }
    /* Hero Item check */
    unit_ptr = &_UNITS[bu_ptr->unit_idx];
    if(
        (unit_ptr->Hero_Slot > ST_UNDEFINED)
        &&
        (ability_chosen == ST_FALSE)
    )
    {
        hero_slot_idx = unit_ptr->Hero_Slot;
        // OGBUG: this may not be the hero's original owner
        item_spell_idx = _ITEMS[_players[bu_ptr->controller_idx].Heroes[hero_slot_idx].Items[0]].embed_spell_idx;
        if(item_spell_idx > 0 && bu_ptr->item_charges > 0)
        {
            spell_idx = item_spell_idx;
            target_idx = AITP_Combat_Spell(spell_idx, bu_ptr->controller_idx, &target_x, &target_y);
            if(target_idx > ST_UNDEFINED)
            {
                ability_chosen = ST_TRUE;
                *selected_action = bua_UseItem;
            }
        }
    }
    /* Spellcasting check */
    if(
        (bu_ptr->mana > 2)
        &&
        (ability_chosen == ST_FALSE)
        &&
        (*selected_action != bua_No_Spells)
    )
    {
        if((bu_ptr->ranged - 5) <= Random(15))
        {
            *selected_action = bua_CastSpell;
            ability_chosen = ST_TRUE;
        }
    }
    /* Main Target Search Loop */
    if(ability_chosen == ST_FALSE)
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            target_bu_ptr = &battle_units[itr_battle_units];
            if(target_bu_ptr->controller_idx == bu_ptr->controller_idx)
            {
                continue;
            }
            if(target_bu_ptr->status != bus_Active)
            {
                continue;
            }
            /* Invisibility checks for ranged */
            if(has_ranged_attack == ST_TRUE)
            {
                if((target_bu_ptr->enchantments & UE_INVISIBILITY) != 0) continue;
                if((target_bu_ptr->item_enchantments & UE_INVISIBILITY) != 0) continue;
                if((_UNITS[target_bu_ptr->unit_idx].enchantments & UE_INVISIBILITY) != 0) continue;
                if((target_bu_ptr->Abilities & UA_INVISIBILITY) != 0) continue;
            }
            /* Wall of Darkness / City Walls check */
            if(has_ranged_attack == ST_TRUE)
            {
                if(battlefield->city_enchantments[WALL_OF_DARKNESS] > 0 && !(bu_ptr->Attribs_1 & USA_IMMUNITY_ILLUSION))   /* OGBUG: ignores combat-cast Wall of Darkness */
                {
                    if(!Battle_Unit_Is_Within_City(battle_unit_idx) && Battle_Unit_Is_Within_City(itr_battle_units))
                    {
                        continue;
                    }
                }
            }
            /* If you're blocking the gate, you can still be targeted, so if invisible and NOT blocking the gate, then skip this target */
            if(!Target_Is_Visible(itr_battle_units) && (target_bu_ptr->cgx != CGX_GATE || target_bu_ptr->cgy != CGY_GATE))
            {
                    continue;
            }
            /* Hero Safety logic */
            if(unit_ptr->Hero_Slot > ST_UNDEFINED && has_ranged_attack == ST_FALSE && _ai_disable_hero_melee_safety_check != ST_TRUE)
            {
                /* Skip if hero melee < target defense */
                if(bu_ptr->melee < target_bu_ptr->defense)
                {
                    continue;
                }
                /* Skip if hero defense < (target melee * 2 / 3) */
                if(bu_ptr->defense < (target_bu_ptr->melee * 2 / 3))
                {
                    continue;
                }
                /* If target has Thrown attack, check defense vs ranged */
                if(target_bu_ptr->ranged_type >= srat_Thrown)
                {
                    if(bu_ptr->defense < (target_bu_ptr->ranged * 2 / 3))
                    {
                        continue;
                    }
                }
            }
            /* City Wall obstacle check for defenders */
            if(_ai_battlefield_city_walls > 0 && has_ranged_attack == ST_TRUE)
            {
                if(Range_To_Battle_Unit(battle_unit_idx, itr_battle_units) != 1)
                {
                    if(bu_ptr->controller_idx == _combat_defender_player && Battle_Unit_Is_Within_City(battle_unit_idx))
                    {
                        if(bu_ptr->cgx == CGX_GATE && bu_ptr->cgy == CGY_GATE && (_ai_battlefield_city_walls & 1))
                        {
                            /* Gate is closed/present, don't leave? */
                        }
                        else if(!Battle_Unit_Is_Within_City(itr_battle_units))
                        {
                            continue;
                        }
                    }
                }
            }
            target_value = Target_Unit_Value(battle_unit_idx, itr_battle_units, has_ranged_attack);
            if(has_ranged_attack == ST_TRUE)
            {
                /* Ranged comparison logic */
                if(target_value >= highest_value - 3 && target_value <= highest_value + 3 && target_idx > ST_UNDEFINED)
                {
                    total_health = (target_bu_ptr->figure_cnt * target_bu_ptr->hits) - target_bu_ptr->front_figure_damage;
                    target_health = (battle_units[target_idx].figure_cnt * battle_units[target_idx].hits) - battle_units[target_idx].front_figure_damage;
                    if(total_health < target_health)
                    {
                        if(target_value > highest_value) highest_value = target_value;
                        target_idx = itr_battle_units;
                        *selected_action = bua_Ready;
                    }
                }
                else if(target_value > highest_value)
                {
                    highest_value = target_value;
                    target_idx = itr_battle_units;
                    *selected_action = bua_Ready;
                }
            }
            else
            {
                /* Melee comparison logic */
                if(target_value >= highest_value)
                {
                    if(target_value == highest_value)
                    {
                        /* HACK  no asm counterpart; guards battle_units[-1] when no target has been chosen yet */
                        if(target_idx > ST_UNDEFINED)
                        {
                            total_health = (target_bu_ptr->figure_cnt * target_bu_ptr->hits) - target_bu_ptr->front_figure_damage;
                            target_health = (battle_units[target_idx].figure_cnt * battle_units[target_idx].hits) - battle_units[target_idx].front_figure_damage;
                            /* On equal target value, melee prefers the healthier target. */
                            if(total_health > target_health)
                            {
                                highest_value = target_value;
                                target_idx = itr_battle_units;
                                *selected_action = bua_Ready;
                            }
                        }
                    }
                    else
                    {
                        highest_value = target_value;
                        target_idx = itr_battle_units;
                        *selected_action = bua_Ready;
                    }
                }
            }
        }
    }
    return target_idx;
}


// WZD o114p08
/*
MoO2  Module: CMBTAI  Auto_Move_Ship_() <-| OON XREF:  Module: CMBTAI  Do_Auto_Ship_Turn_() <-| OON XREF:  Module: COMBAT1  Do_Combat_Turn_()
*/
/**
 * @brief Builds a tactical move path for an AI-controlled combat unit and executes fallback city-entry movement when needed.
 *
 * This routine is the high-level movement wrapper used by the combat AI before delegating the
 * actual step execution to Auto_Move_Unit(). It prepares the movement cost map for the acting
 * battle unit, marks other active units and vortex squares as impassable, applies city-movement
 * restrictions, and computes a path toward the requested destination.
 *
 * When the acting unit belongs to the attacker and either has no explicit target or its current
 * destination square is blocked, the function performs a "stomp-around" fallback search over the
 * city interior. It finds the shortest reachable square in the city proper and issues an
 * intermediate Auto_Move_Unit() call toward that square before attempting the originally requested
 * move. After that optional fallback, it always performs a final Auto_Move_Unit() call using the
 * destination and stage constraints passed in by the caller.
 *
 * @param battle_unit_idx Index of the acting battle unit in battle_units.
 * @param dst_cgx Requested combat-grid destination X coordinate.
 * @param dst_cgy Requested combat-grid destination Y coordinate.
 * @param target_battle_unit_idx Target unit index for pathing and occupancy exceptions, or
 *        ST_UNDEFINED when moving without a direct target.
 * @param rally_cgx Rally-point X coordinate used later by Auto_Move_Unit() to constrain quadrant
 *        movement when the destination is not itself the stage point.
 * @param rally_cgy Rally-point Y coordinate used later by Auto_Move_Unit() to constrain quadrant
 *        movement when the destination is not itself the stage point.
 *
 * @note This function mutates the shared combat movement-cost map and movement-path state used by
 *       the lower-level combat pathfinding routines.
 * @note The stomp-around fallback applies only to attacking units.
 * @note The function can invoke Auto_Move_Unit() twice in one call: once for the fallback city
 *       square and once for the originally requested destination.
 */
void Do_Auto_Unit_Turn(int16_t battle_unit_idx, int16_t dst_cgx, int16_t dst_cgy, int16_t target_battle_unit_idx, int16_t rally_cgx, int16_t rally_cgy)
{
    int16_t stomp_cgy = 0;
    int16_t stomp_cgx = 0;
    int16_t path_count = 0;
    int16_t city_area_cgy = 0;
    int16_t city_area_cgx = 0;
    int16_t itr_battle_units = 0;
/*
    BEGIN:   Init Movement Cost Map  (~== Auto_Move_Unit())
*/
    // sets _cmbt_movepath_cost_map[]
    Set_Movement_Cost_Map(battle_unit_idx);
    /*
        mark all squares with units other than the target as
        impassable
    */
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(
            (itr_battle_units != target_battle_unit_idx)
            &&
            (battle_units[itr_battle_units].status == bus_Active)
        )
        {
            _cmbt_movepath_cost_map[((battle_units[itr_battle_units].cgy * COMBAT_GRID_WIDTH) + battle_units[itr_battle_units].cgx)] = INF;
        }
    }
    // sets city squares as impassible
    Update_Move_Map_City_Area_Restrictions(battle_unit_idx);
    if(
        (battle_units[battle_unit_idx].controller_idx == _combat_defender_player)
        &&
        (_ai_stay_in_city == ST_TRUE)
        &&
        (
            (battlefield->wall_of_fire > 0)
            ||
            (battlefield->wall_of_darkness > 0)
        )
        &&
        (Battle_Unit_Is_Within_City(battle_unit_idx) == ST_TRUE)
    )
    {
        // sets city area perimeter as impassible
        Update_Move_Map_City_Perimeter_Restrictions();
    }
    for(itr_battle_units = 0; itr_battle_units < _vortex_count; itr_battle_units++)
    {
        _cmbt_movepath_cost_map[((_vortexes[itr_battle_units].cgy * COMBAT_GRID_WIDTH) + _vortexes[itr_battle_units].cgx)] = INF;
    }
    // sets movement_path_grid_cell_count
    Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, dst_cgx, dst_cgy);
/*
    END:  Init Movement Cost Map  (~== Auto_Move_Unit())
*/
/*
    BEGIN: Stomp-Around
*/
    if(
        (
            (
                (target_battle_unit_idx > ST_UNDEFINED)
                &&
                (_cmbt_movepath_cost_map[((dst_cgy * COMBAT_GRID_WIDTH) + dst_cgx)] == INF)
            )
            ||
            (target_battle_unit_idx == ST_UNDEFINED)
        )
        &&
        (battle_units[battle_unit_idx].controller_idx == _combat_attacker_player)
    )
    {
        // check if there is a path available to any of the city proper squares and if so, find the shortest
        path_count = 999;
        for(city_area_cgx = 5; city_area_cgx < 9; city_area_cgx++)
        {
            for(city_area_cgy = 10; city_area_cgy < 14; city_area_cgy++)
            {
                // sets movement_path_grid_cell_count
                Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, city_area_cgx, city_area_cgy);
                if(
                    (movement_path_grid_cell_count > 0)
                    &&
                    (movement_path_grid_cell_count < path_count)
                )
                {
                    stomp_cgx = city_area_cgx;
                    stomp_cgy = city_area_cgy;
                    path_count = movement_path_grid_cell_count;
                }
            }
        }
        if(path_count < 999)
        {
            // HERE: stomp_cgx,cgy are the nearest, valid stomping coordinates
            Auto_Move_Unit(battle_unit_idx, stomp_cgx, stomp_cgy, ST_UNDEFINED, stomp_cgx, stomp_cgy);
        }
    }
/*
    END: Stomp-Around
*/
    // ¿ DEDU  what do we have here if we did the stomp-around ?
    // HERE: dst_cgx,cgy and rally_cgx,cgy are as passed in
    Auto_Move_Unit(battle_unit_idx, dst_cgx, dst_cgy, target_battle_unit_idx, rally_cgx, rally_cgy);
}


// WZD o114p09
int16_t Auto_Move_Unit(int16_t battle_unit_idx, int16_t dst_cgx, int16_t dst_cgy, int16_t target_battle_unit_idx, int16_t rally_cgx, int16_t rally_cgy)
{
    int16_t move_anim_speed = 0;
    int16_t first_step_index = 0;
    SAMB_ptr move_sound_seg = NULL;
    int16_t move_visible = 0;
    int16_t delta_y = 0;
    int16_t delta_x = 0;
    int16_t min_y = 0;
    int16_t min_x = 0;
    int16_t rp_origin_y_2 = 0;
    int16_t rp_origin_x_2 = 0;
    int16_t first_step = 0;
    int16_t attack_step = 0;
    int16_t last_target_y = 0;
    int16_t last_target_x = 0;
    int16_t origin_y = 0;
    int16_t origin_x = 0;
    int16_t facing_y_offset = 0;
    int16_t facing_x_offset = 0;
    int16_t itr_battle_units = 0;
    int16_t itr_grid = 0;
    uint32_t move_sound_seg_size = 0;
    int8_t movement_points = 0;
    int16_t max_x = 0;
    int16_t max_y = 0;
    if(_auto_combat_flag == ST_TRUE)
    {
        move_anim_speed = 2;
    }
    else
    {
        move_anim_speed = 1;
    }
/*
    BEGIN:  ~== Do_Auto_Unit_Turn()
*/
    Set_Movement_Cost_Map(battle_unit_idx);
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        /* mark all squares with units other than the target as impassable */
        /* OGBUG: this has just been done in the parent function */
        if(
            (itr_battle_units != target_battle_unit_idx)
            &&
            (battle_units[itr_battle_units].status == bus_Active)
        )
        {
            _cmbt_movepath_cost_map[((battle_units[itr_battle_units].cgy * COMBAT_GRID_WIDTH) + battle_units[itr_battle_units].cgx)] = INF;
        }
    }
    Update_Move_Map_City_Area_Restrictions(battle_unit_idx);
    if(
        (battle_units[battle_unit_idx].controller_idx == _combat_defender_player)
        &&
        (_ai_stay_in_city == ST_TRUE)
        &&
        (
            (battlefield->wall_of_fire > 0)
            ||
            (battlefield->wall_of_darkness > 0)
        )
        &&
        (Battle_Unit_Is_Within_City(battle_unit_idx) == ST_TRUE)
    )
    {
        Update_Move_Map_City_Perimeter_Restrictions();
    }
    /* mark all squares with Magic Vortices on them as impassable */
    /* OGBUG: this has just been done in the parent function */
    for(itr_battle_units = 0; itr_battle_units < _vortex_count; itr_battle_units++)
    {
        _cmbt_movepath_cost_map[((_vortexes[itr_battle_units].cgy * COMBAT_GRID_WIDTH) + _vortexes[itr_battle_units].cgx)] = INF;
    }
    // sets movement_path_grid_cell_count
    Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, dst_cgx, dst_cgy);
/*
    END:  ~== Do_Auto_Unit_Turn()
*/
    if(movement_path_grid_cell_count == 0)
    {
        return ST_FALSE;
    }
    if(battle_units[battle_unit_idx].controller_idx != HUMAN_PLAYER_IDX)
    {
        _ai_immobile_counter = -1;
    }
    origin_x = battle_units[battle_unit_idx].cgx;
    origin_y = battle_units[battle_unit_idx].cgy;
    battle_units[battle_unit_idx].target_cgx = dst_cgx;
    battle_units[battle_unit_idx].target_cgy = dst_cgy;
    uu_combat_movement_variable = 0;
    battle_units[battle_unit_idx].mid_move = ST_TRUE;
    rp_origin_x_2 = battle_units[battle_unit_idx].cgx;
    rp_origin_y_2 = battle_units[battle_unit_idx].cgy;
    if(target_battle_unit_idx > -1)
    {

        dst_cgx = battle_units[target_battle_unit_idx].cgx;
        dst_cgy = battle_units[target_battle_unit_idx].cgy;

    }
    /*
        The original reuses the two incoming parameter slots (bp+0Eh, bp+10h) as the movement box's
        max corner.  Production splits that into rally_cgx/rally_cgy (the parameter, read only) and
        max_x/max_y (the box).  The seed below is what makes the split faithful, and it is NOT
        cosmetic: several paths through the quadrant selection never assign the max corner at all --
        branch A with rp_origin_y_2 < dst_cgy leaves Y unwritten, branch B with rp_origin_x_2 <
        dst_cgx leaves X unwritten, and branch C leaves either.  On those paths the original's box
        max IS the rally coordinate, by falling through without a store.  Dropping the seed would
        leave max_x / max_y at 0 and silently shrink the box.
    */
    max_x = rally_cgx;  // DNE in Dasm
    max_y = rally_cgy;  // DNE in Dasm
    if((rally_cgx == dst_cgx) && (rally_cgy == dst_cgy))
    {
        min_x = COMBAT_GRID_XMIN;
        max_x = COMBAT_GRID_XMAX;
        min_y = COMBAT_GRID_YMIN;
        max_y = COMBAT_GRID_YMAX;
    }
    else
    {
        delta_x = abs(rp_origin_x_2 - dst_cgx);
        delta_y = abs(rp_origin_y_2 - dst_cgy);
        if(delta_x < delta_y)
        {
            min_x = COMBAT_GRID_XMIN;
            max_x = COMBAT_GRID_XMAX;
            if(rp_origin_y_2 < dst_cgy)
            {
                min_y = COMBAT_GRID_YMIN;
            }
            else
            {
                min_y = rally_cgy;
                max_y = COMBAT_GRID_YMAX;
            }
        }
        if(delta_x > delta_y)
        {
            min_y = COMBAT_GRID_YMIN;
            max_y = COMBAT_GRID_YMAX;
            if(rp_origin_x_2 < dst_cgx)
            {
                min_x = COMBAT_GRID_XMIN;
            }
            else
            {
                min_x = rally_cgx;
                max_x = COMBAT_GRID_XMAX;
            }
        }
        if(delta_x == delta_y)
        {
            if(rp_origin_x_2 < dst_cgx)
            {
                min_x = COMBAT_GRID_XMIN;
            }
            else
            {
                min_x = rally_cgx;
                max_x = COMBAT_GRID_XMAX;
            }
            if(rp_origin_y_2 < dst_cgy)
            {
                min_y = COMBAT_GRID_YMIN;
            }
            else
            {
                min_y = rally_cgy;
                max_y = COMBAT_GRID_YMAX;
            }
        }
    }
    first_step = ST_TRUE;
    if(
        (
            ((battle_units[battle_unit_idx].enchantments & UE_INVISIBILITY) != 0)
            ||
            ((battle_units[battle_unit_idx].item_enchantments & UE_INVISIBILITY) != 0)
            ||
            ((battle_units[battle_unit_idx].Abilities & UA_INVISIBILITY) != 0)
            ||
            ((_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments & UE_INVISIBILITY) != 0)
        )
        &&
        (battle_units[battle_unit_idx].controller_idx != HUMAN_PLAYER_IDX)
    )
    {
        if(
            (
                (battle_units[battle_unit_idx].controller_idx != _combat_attacker_player)
                &&
                (_attacker_sees_illusions != ST_TRUE)
            )
            ||
            (
                (battle_units[battle_unit_idx].controller_idx != _combat_defender_player)
                &&
                (_defender_sees_illusions != ST_TRUE)
            )
        )
        {
            move_visible = ST_FALSE;
        }
        else
        {
            move_visible = ST_TRUE;
        }
    }
    else
    {
        move_visible = ST_TRUE;
    }
    first_step_index = 0;
    if(
        ((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
        ||
        ((battle_units[battle_unit_idx].Move_Flags & MV_MERGING) != 0)
    )
    {
        if(
            (target_battle_unit_idx > -1)
            &&
            (_cmbt_mvpth_x[(movement_path_grid_cell_count - 1)] == battle_units[target_battle_unit_idx].cgx)
            &&
            (_cmbt_mvpth_y[(movement_path_grid_cell_count - 1)] == battle_units[target_battle_unit_idx].cgy)
        )
        {
            if(movement_path_grid_cell_count > 1)
            {
                first_step_index = (movement_path_grid_cell_count - 2);
            }
        }
        else
        {
            first_step_index = (movement_path_grid_cell_count - 1);
        }
    }
    /* TODO: itr_path = path_start */
    for(itr_grid = first_step_index; itr_grid < movement_path_grid_cell_count; itr_grid++)
    {
        if(
            (first_step != ST_TRUE)
            &
            (battle_units[battle_unit_idx].movement_points <= _cmbt_mvpth_c[((_cmbt_mvpth_y[(itr_grid - 1)] * COMBAT_GRID_WIDTH) + _cmbt_mvpth_x[(itr_grid - 1)])])
        )
        {
            break;
        }
        if(
            (
                (_cmbt_mvpth_x[itr_grid] <= max_x)
                &&
                (_cmbt_mvpth_x[itr_grid] >= min_x)
                &&
                (_cmbt_mvpth_y[itr_grid] <= max_y)
                &&
                (_cmbt_mvpth_y[itr_grid] >= min_y)
            )
            ||
            (
                (_cmbt_mvpth_x[itr_grid] == dst_cgx)
                &&
                (_cmbt_mvpth_y[itr_grid] == dst_cgy)
            )
        )
        {
            first_step = ST_FALSE;
            battle_units[battle_unit_idx].target_cgx = _cmbt_mvpth_x[itr_grid];
            battle_units[battle_unit_idx].target_cgy = _cmbt_mvpth_y[itr_grid];
            attack_step = ST_FALSE;
            if(
                (battle_units[battle_unit_idx].target_cgx == battle_units[target_battle_unit_idx].cgx)
                &&
                (battle_units[battle_unit_idx].target_cgy == battle_units[target_battle_unit_idx].cgy)
                &&
                (battle_units[target_battle_unit_idx].status == bus_Active)
            )
            {
                attack_step = ST_TRUE;
            }
            if(attack_step == ST_TRUE)
            {
                if(battle_units[battle_unit_idx].figure_cnt <= 0)
                {
                    battle_units[battle_unit_idx].movement_points = -2;
                }
                else
                {
                    Battle_Unit_Attack(battle_unit_idx, target_battle_unit_idx, 0, 0);
                }
                itr_grid--;
            }
            else
            {
                // if attacker, no walls, move is into city area, move is not from city area
                if(
                    (battle_units[battle_unit_idx].controller_idx != _combat_defender_player)
                    ||
                    (_ai_battlefield_city_walls != BATTLEFIELD_CITY_WALL_STONE)  /* OGBUG: will only be 1 if there's only a stone wall but no other types */
                    ||
                    (
                        (_cmbt_mvpth_x[itr_grid] >= MIN_CGX_CITY)
                        &&
                        (_cmbt_mvpth_x[itr_grid] <= MAX_CGX_CITY)
                        &&
                        (_cmbt_mvpth_y[itr_grid] >= MIN_CGY_CITY)
                        &&
                        (_cmbt_mvpth_y[itr_grid] <= MAX_CGY_CITY)
                    )
                    ||
                    (Battle_Unit_Is_Within_City(battle_unit_idx) != ST_TRUE)
                )
                {
                    if(move_visible == ST_TRUE)
                    {
                        if(
                            ((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
                            ||
                            ((battle_units[battle_unit_idx].Move_Flags & MV_MERGING) != 0)
                        )
                        {
                            if((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
                            {
                                Battle_Unit_Teleport(battle_unit_idx, _cmbt_mvpth_x[itr_grid], _cmbt_mvpth_y[itr_grid]);
                            }
                            else
                            {
                                Battle_Unit_Tunnel(battle_unit_idx, _cmbt_mvpth_x[itr_grid], _cmbt_mvpth_y[itr_grid]);
                            }
                        }
                        else
                        {
                            if(magic_set.sound_effects == ST_TRUE)
                            {
                                Play_Sound(sound_silent_seg, sound_silent_seg_size);
                                Mark_Block(World_Data);
                                move_sound_seg = Reload_Battle_Unit_Move_Sound(battle_unit_idx, &move_sound_seg_size);
                                Release_Block(World_Data);
                            }
                            else
                            {
                                move_sound_seg = (SAMB_ptr)ST_UNDEFINED;
                            }
                            battle_units[battle_unit_idx].move_anim_ctr = 1;
                            if(move_sound_seg != (SAMB_ptr)ST_UNDEFINED)
                            {
                                Play_Sound(move_sound_seg, move_sound_seg_size);
                            }
                            if(magic_set.movement_animations == ST_TRUE)
                            {
                                for(itr_battle_units = 0; itr_battle_units < MOVE_ANIM_CNT; itr_battle_units += move_anim_speed)
                                {
                                    /* HACK */  Mark_Time();
                                    battle_units[battle_unit_idx].move_anim_ctr += move_anim_speed;
                                    Combat_Screen_Draw();
                                    PageFlip_FX();
                                    /* HACK */  Release_Time(1);
                                }
                            }
                            else
                            {
                                battle_units[battle_unit_idx].move_anim_ctr = MOVE_ANIM_MAX;
                                Combat_Screen_Draw();
                                PageFlip_FX();
                            }
                            if(magic_set.sound_effects == ST_TRUE)
                            {
                                Play_Sound(sound_silent_seg, sound_silent_seg_size);
                            }
                        }
                        battle_units[battle_unit_idx].move_anim_ctr = 0;
                        battle_units[battle_unit_idx].cgx = _cmbt_mvpth_x[itr_grid];
                        battle_units[battle_unit_idx].cgy = _cmbt_mvpth_y[itr_grid];
                    }
                }
            }
        }  /* end of the if( in-cone || == dst ) */
        else
        {
            /* a path cell outside the movement box that is not the destination ends the move loop. */
            break;
        }
    }  /* END: for(itr_grid = first_step_index; itr_grid < movement_path_grid_cell_count; itr_grid++) */
    /*
        END: Move-Path Movement Animation
    */
    battle_units[battle_unit_idx].move_anim_ctr = 0;
    battle_units[battle_unit_idx].mid_move = ST_FALSE;
    if(itr_grid > 1)
    {
        origin_x = _cmbt_mvpth_x[(itr_grid - 2)];
        origin_y = _cmbt_mvpth_y[(itr_grid - 2)];
        last_target_x = _cmbt_mvpth_x[(itr_grid - 1)];
        last_target_y = _cmbt_mvpth_y[(itr_grid - 1)];
    }
    else  /* itr_grid == 0 || 1 */
    {
        last_target_x = dst_cgx;
        last_target_y = dst_cgy;
    }
    facing_x_offset = (last_target_x - origin_x);
    facing_y_offset = (last_target_y - origin_y);
    if(itr_grid != 0)
    {
        battle_units[battle_unit_idx].cgx = _cmbt_mvpth_x[(itr_grid - 1)];
        battle_units[battle_unit_idx].cgy = _cmbt_mvpth_y[(itr_grid - 1)];
        battle_units[battle_unit_idx].target_cgx = (last_target_x + facing_x_offset);
        battle_units[battle_unit_idx].target_cgy = (last_target_y + facing_y_offset);
    }
    if(
        ((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
        ||
        ((battle_units[battle_unit_idx].Move_Flags & MV_MERGING) != 0)
    )
    {
        /* OGBUG: teleporting units can't use roads */
        battle_units[battle_unit_idx].movement_points -= 2;
    }
    else
    {
        movement_points = _cmbt_mvpth_c[((battle_units[battle_unit_idx].cgy * COMBAT_GRID_WIDTH) + battle_units[battle_unit_idx].cgx)];
        battle_units[battle_unit_idx].movement_points -= movement_points;
    }
    /* CLAUDE: test-only combat-legality logging (LOG_CAT_COMBAT_TEST, off by default). Emits the full move path so a checker can verify every step is grid-adjacent and correlate the move to its target.  Auto_Move_Unit is the one frame that holds mover + destination + target. */
    if(STU_Log_Category_Enabled(LOG_CAT_COMBAT_TEST))
    {
        int16_t cmbt_test_k;
        LOG_INFO(LOG_CAT_COMBAT_TEST, "MOVE turn=%d unit=%d target=%d dst=(%d,%d) final=(%d,%d) mp=%d pathlen=%d", _combat_turn, battle_unit_idx, target_battle_unit_idx, dst_cgx, dst_cgy, battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, battle_units[battle_unit_idx].movement_points, movement_path_grid_cell_count);
        for(cmbt_test_k = 0; cmbt_test_k < movement_path_grid_cell_count; cmbt_test_k++)
        {
            LOG_INFO(LOG_CAT_COMBAT_TEST, "MOVE_STEP unit=%d seq=%d cell=(%d,%d)", battle_unit_idx, cmbt_test_k, _cmbt_mvpth_x[cmbt_test_k], _cmbt_mvpth_y[cmbt_test_k]);
        }
    }
    return ST_TRUE;
}
