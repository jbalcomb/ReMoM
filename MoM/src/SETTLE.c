/*
    WIZARDS.EXE
        ovr100

    Moo2
        Module: COLONIZE

*/

#include "../../STU/src/AI_METRICS.h"
#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"

#include "CITYCALC.h"
#include "Combat.h"
#include "Lair.h"
#include "MainScr.h"
#include "Outpost.h"
#include "Terrain.h"
#include "UnitMove.h"   // WTFMATE
#include "UNITSTK.h"
#include "UNITTYPE.h"   // WTFMATE
#include "WZD_o059.h"
#include "WZD_o143.h"   /* OVL_GetRoadPath() */

#include "SETTLE.h"



/*
o100p04
IDK_Settle_s82061()
o100p05
Army_Do_Settle()
o77p01
Create_Outpost()


Module: COLONIZE
    Make_New_Colony_Or_Outpost_
        Address: 01:000E5EB3
    Make_New_Colony_
        Address: 01:000E6071
    Make_New_Outpost_
        Address: 01:000E607F
    Allocate_New_Colony_Slots_
        Address: 01:000E64F4

Module: MAINPUPS
    New_Colony_Confirmation_Box_
        Address: 01:000C9FC4

*/
/*
Program-Flow: Special Unit Action - Settle 

    IDK_Settle_s82061()
        |-> STK_GetMovableUnits()
        |-> Army_Do_Settle()
            |-> Create_Outpost(unit_x, unit_y, unit_p, unit_race, unit_owner_idx, unit_idx)
                |-> TILE_CanBeSettled()
                |-> CTY_CreateEmpty()

Warn0(cnst_MaxCitiesError);  / * "No more outposts may be built." * /


Has Unit Type Ability 'Create Outpost' and NOT Unit Mutation - Undead

Manual:
**Build** outposts: settlers can build outposts (click on *build button*)
on map squares that are at least three map squares from any
other city.
...select the build option...
...build an outpost...

Game-Data:
"No more outposts may be built."
"New Outpost Founded"
"Outpost Of "
"New Outpost"
*/




// WZD o100p01
/* MoO2  Module: AIMOVE  Move_AI_() */
/**
 * @brief Processes one AI player's unit orders for the current turn slice.
 *
 * Saves the current map-view coordinates and plane, then iterates all units
 * owned by @p player_idx and dispatches behavior based on each unit's
 * `Status` (move, goto, build road, meld, settle, or seek transport). While
 * processing movement-capable orders, it captures pre-action state and emits
 * per-unit outcome metrics through AI_Metrics_Emit_Unit_Outcome().
 *
 * If Time Stop is active, only the casting player is allowed to proceed;
 * other players return immediately without processing units.
 *
 * @param player_idx Index of the AI player whose units should be processed.
 *
 * @return This function does not return a value. It may update unit orders,
 *         positions, statuses, and movement-failure flags through delegated
 *         unit-action routines.
 *
 * @note Regardless of movement results, the function restores global map view
 *       coordinates (`_map_x`, `_map_y`, `_prev_world_x`, `_prev_world_y`) and
 *       `_map_plane` to their saved pre-processing values before exiting the
 *       normal path.
 */
void gd_dump_units(const char* point);   /* CLAUDE: GD capture (defined in INITGAME.c) */

void AI_Execute_Orders(int16_t player_idx)
{
    int16_t l_map_plane = 0;
    int16_t l_map_y = 0;
    int16_t l_map_x = 0;
    int16_t unit_idx = 0;
    static int gd614_done = 0, gd615_done = 0;   /* CLAUDE: 614/615 fire-once gates */

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    /* CLAUDE: GD point 614 -- _UNITS BEFORE AI_Execute_Orders, the per-AI-player order
     * EXECUTION pass (dispatches each unit by Status: move/goto/road/meld/settle/ferry
     * via AI_Unit_Army_Do_*; this is where dst/Status actually get applied).  Pairs with
     * 615 to isolate this call's effect on _UNITS (the 911 movement divergence).
     * player_idx scalar records WHICH player.  Fire once (first AI player). */
    if(!gd614_done) { gd614_done = 1;
        LOG_DEBUG(LOG_CAT_GENERAL, "[GD] 614_AI_Execute_Orders_Entry_U player_idx = %d", (int)player_idx);
        gd_dump_units("614_AI_Execute_Orders_Entry_U"); }

    /* Store current map state to restore later */
    l_map_x = _map_x;
    l_map_y = _map_y;
    l_map_plane = _map_plane;

    /* ¿ OGBUG  This is in and only in a per-player-loop that already checks for Time-Stop ? */

    /* Time Stop Logic Check */
    if(g_timestop_player_num > 0)
    {
        /* If Time Stop is active, only the casting player can move units */
        if((player_idx + 1) != g_timestop_player_num)
        {
            /* CLAUDE: GD 615 on the Time-Stop early-return path too -- OG's far-return
             * fires regardless of the internal path, so both sides dump here.  Shared
             * gd615_done gate -> fires once total (this OR the normal exit). */
            if(!gd615_done) { gd615_done = 1; gd_dump_units("615_AI_Execute_Orders_Return_U"); }
            return;
        }
    }

    /* Iterate through all units in the game world */
    for (unit_idx = 0; unit_idx < _units; unit_idx++)
    {

        /* Check if unit belongs to the player currently processing AI moves */
        if(_UNITS[unit_idx].owner_idx == (int8_t)player_idx)
        {

            /* AI_METRICS */    /* Snapshot state before movement for metrics */
            /* AI_METRICS */    int16_t pre_wx = _UNITS[unit_idx].wx;
            /* AI_METRICS */    int16_t pre_wy = _UNITS[unit_idx].wy;
            /* AI_METRICS */    int16_t pre_status = _UNITS[unit_idx].Status;
            /* AI_METRICS */    int16_t pre_dst_wx = _UNITS[unit_idx].dst_wx;
            /* AI_METRICS */    int16_t pre_dst_wy = _UNITS[unit_idx].dst_wy;

            /* Process unit orders based on its current Status */
            /* Switch based on unit status (2 to 16) */
            /* Logic: bx = status - 2; if(bx > 14) skip; */
            switch(_UNITS[unit_idx].Status)
            {
                case us_BuildRoad:
                {
                    AI_Unit_Army_Do_Road(unit_idx);
                    AI_Unit_Army_Do_Move(unit_idx);
                } break;
                case us_GOTO:
                {
                    AI_Unit_Army_Do_Move(unit_idx);
                } break;
                case us_Meld:
                {
                    AI_Unit_Army_Do_Meld(unit_idx);
                } break;
                case us_Settle:
                {
                    AI_Unit_Army_Do_Settle(unit_idx);
                } break;
                case us_Ferry:
                {
                    AI_Unit_Army_Do_Ferry(unit_idx);
                } break;
                case us_Move:
                {
                    AI_Unit_Army_Do_Move(unit_idx);
                } break;
                default:
                    /* Cases 0-1, 4-8, 12-15 are skipped */
                    break;
            }

            /* AI_METRICS */    /* Emit unit outcome for non-idle units */
            /* AI_METRICS */    if(pre_status == us_GOTO || pre_status == us_Move || pre_status == us_BuildRoad)
            /* AI_METRICS */    {
            /* AI_METRICS */        AI_Metrics_Emit_Unit_Outcome(_turn, player_idx, unit_idx,
            /* AI_METRICS */            _UNITS[unit_idx].type, pre_status,
            /* AI_METRICS */            pre_wx, pre_wy,
            /* AI_METRICS */            pre_dst_wx, pre_dst_wy,
            /* AI_METRICS */            _UNITS[unit_idx].wx, _UNITS[unit_idx].wy,
            /* AI_METRICS */            _UNITS[unit_idx].Move_Failed);
            /* AI_METRICS */    }

        }

    }

    /* Restore map view coordinates and plane to state before AI movement */
    _map_x = l_map_x;
    _map_y = l_map_y;
    _prev_world_x = _map_x;
    _prev_world_y = _map_y;
    _map_plane = l_map_plane;
 
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    /* CLAUDE: GD point 615 -- _UNITS AFTER AI_Execute_Orders (normal exit).  Shared
     * gd615_done gate with the Time-Stop early-return path.  Fire once. */
    if(!gd615_done) { gd615_done = 1; gd_dump_units("615_AI_Execute_Orders_Return_U"); }

}


// WZD o100p02
/**
 * @brief Executes AI meld handling for one unit's current stack square.
 *
 * If the referenced unit has already finished its turn, this routine exits
 * immediately. Otherwise it gathers the owning player's full army stack at the
 * unit's current coordinates, delegates node-resolution and meld consumption to
 * Army_Do_Meld(), and then resets every unit in that gathered stack to
 * @c us_Ready.
 *
 * @param unit_idx Index of the AI unit whose square is used to gather the
 *                 stack for meld processing.
 *
 * @return This function does not return a value. It may update unit statuses
 *         for all units on the square and may indirectly modify node state via
 *         Army_Do_Meld().
 *
 * @note The current implementation preserves legacy behavior that marks the
 *       entire gathered stack as @c us_Ready after meld handling.
 */
void AI_Unit_Army_Do_Meld(int16_t unit_idx)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t unit_owner_idx = 0;
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t troop_count = 0;
    int16_t itr_troops = 0;

    if(_UNITS[unit_idx].Finished == ST_TRUE)
    {
        return;
    }

    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_wp = _UNITS[unit_idx].wp;
    unit_owner_idx = _UNITS[unit_idx].owner_idx;

    Player_Army_At_Square(unit_wx, unit_wy, unit_wp, unit_owner_idx, &troop_count, troops);

    Army_Do_Meld(troop_count, troops);

    for (itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        _UNITS[troops[itr_troops]].Status = us_Ready;
    }

}


// WZD o100p03
void AI_Unit_Army_Do_Settle(int16_t unit_idx)
{
    int16_t troops[MAX_STACK] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t unit_owner_idx = 0;
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t troop_count = 0;
    int16_t itr_troops = 0;

    if(_UNITS[unit_idx].Finished ==ST_TRUE)
    {
        return;
    }

    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_wp = _UNITS[unit_idx].wp;
    unit_owner_idx = _UNITS[unit_idx].owner_idx;

    Player_Army_At_Square(unit_wx, unit_wy, unit_wp, unit_owner_idx, &troop_count, troops);

    Army_Do_Settle(troop_count, troops);

    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        _UNITS[troops[itr_troops]].Status = us_Ready;
    }

}


// WZD o100p04
int16_t Active_Army_Do_Settle(void)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t troop_count = 0;
    Active_Unit_Stack(&troop_count, &troops[0]);
    return Army_Do_Settle(troop_count, &troops[0]);
}

// WZD o100p05
int16_t Army_Do_Settle(int16_t troop_count, int16_t troops[])
{
    int16_t unit_owner = 0;
    int16_t unit_race = 0;
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t stack_has_settler = 0;
    int16_t unit_type = 0;
    int16_t itr_troops = 0;
    int16_t unit_idx = 0;

    stack_has_settler = ST_FALSE;

    for(itr_troops = 0; ((itr_troops < troop_count) && (stack_has_settler == ST_FALSE)); itr_troops++)
    {
        unit_idx = troops[itr_troops];
        unit_type = _UNITS[unit_idx].type;
        if(
            ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
            &&
            ((_UNITS[unit_idx].mutations & UM_UNDEAD) == 0)
        )
        {
            stack_has_settler = ST_TRUE;
            break;
        }
    }

    if(stack_has_settler != ST_TRUE)
    {
        return ST_FALSE;
    }

    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_wp = _UNITS[unit_idx].wp;
    unit_owner = _UNITS[unit_idx].owner_idx;
    unit_race = _unit_type_table[unit_type].race_type;
    
    if(Create_Outpost(unit_wx, unit_wy, unit_wp, unit_race, unit_owner, unit_idx) == ST_TRUE)
    {
        return ST_TRUE;
    }

    return ST_FALSE;

}


// WZD o100p06
/**
 * @brief Attempts to move one AI-controlled unit stack toward its destination.
 *
 * This routine skips processing when the unit is already marked finished for
 * the turn. Otherwise, it rebuilds the reduced-map artifacts used by movement,
 * snapshots the current global map-view coordinates, and forwards movement to
 * Stack_Move_To() using the unit's owner, destination tile, and plane.
 *
 * The movement result from Stack_Move_To() is returned unchanged.
 *
 * @param unit_idx Index into the global _UNITS array for the unit whose stack
 *                 should execute movement.
 *
 * @return ST_FALSE if the unit is already finished; otherwise the status value
 *         returned by Stack_Move_To() indicating movement outcome.
 *
 * @note This function has side effects beyond unit position changes:
 *       - Recreates reduced-map state via Allocate_Reduced_Map() and
 *         MainScr_Create_Reduced_Map_Picture().
 *       - Reads and writes global map-view coordinates (_map_x, _map_y) as
 *         part of legacy movement-call setup.
 */
int16_t AI_Unit_Army_Do_Move(int16_t unit_idx)
{
    int16_t return_value = 0;
    int16_t niu_unit_wy = 0;
    int16_t niu_unit_wx = 0;
    int16_t unit_owner_idx = 0;
    int16_t unit_wp = 0;
    int16_t l_map_y = 0;
    int16_t l_map_x = 0;
    int16_t unit_dst_wy = 0;
    int16_t unit_dst_wx = 0;

    /* Check if unit is already finished */
    if(_UNITS[unit_idx].Finished == ST_TRUE)
    {
        return ST_FALSE;
    }

    /* ~== MainScr.c  Main_Screen_Reset();  WTF? multiplayer/hotseat? */
    Allocate_Reduced_Map();
    MainScr_Create_Reduced_Map_Picture();

    /* Extract unit data to local variables */
    unit_dst_wx = (int16_t)_UNITS[unit_idx].dst_wx;
    unit_dst_wy = (int16_t)_UNITS[unit_idx].dst_wy;
    unit_wp = (int16_t)_UNITS[unit_idx].wp;
    unit_owner_idx = (int16_t)_UNITS[unit_idx].owner_idx;
    niu_unit_wx = (int16_t)_UNITS[unit_idx].wx;
    niu_unit_wy = (int16_t)_UNITS[unit_idx].wy;

    /* Logic involves temporary map coordinate storage */
    /* Assembly shows redundant assignment back to globals before passing pointers */
    l_map_x = _map_x;
    l_map_y = _map_y;
    _map_x = l_map_x;
    _map_y = l_map_y;

    /* Attempt to move the stack */
    /* DEDU  maybe, probably was just `return Stack_Move_To();` */
    return_value = Stack_Move_To(
        unit_owner_idx, 
        unit_idx, 
        unit_dst_wx, 
        unit_dst_wy, 
        &l_map_x, 
        &l_map_y, 
        unit_wp
    );

    return return_value;
}


// WZD o100p07
// drake178: sub_8227A()
// sub_8227A()


// WZD o100p08
// drake178: sub_82377()
// sub_82377()


// WZD o100p09
/**
 * @brief Relocates a unit from an invalid/full tile to a nearby legal square, or removes it.
 *
 * @details
 * This routine attempts to "push off" a unit by searching a local 3x3 neighborhood
 * around the unit's current coordinates for the first square that can accept it.
 * A candidate square is considered valid when:
 * - it is not the current combat square,
 * - it has no lair,
 * - it has no city owned by the same player,
 * - and @c Unit_Space_At_Square() reports available space (> 0).
 *
 * Search behavior:
 * 1. Capture unit position/owner.
 * 2. Compute starting scan coordinates near (wx - 1, wy - 1), with boundary
 *    adjustment and x-wrap handling.
 * 3. Scan up to a 3x3 region (row-major order), stopping at the first valid square.
 * 4. If found, move the unit to that target square and call @c Units_In_Tower()
 *    for post-move tower handling.
 * 5. If no legal square is found, dismiss the unit via @c Kill_Unit(unit_idx, 1).
 *
 * @param unit_idx Index of the unit in @c _UNITS[] to evict.
 *
 * @return This function returns no value.
 *
 * @note The search is first-fit; it does not evaluate "best" destination quality.
 * @note X coordinates wrap around world width; Y coordinates are clipped by explicit checks.
 * @note (OGBUG) Pre-adjusting start coordinates (instead of validating per-test
 *       coordinates) shifts the effective scan rectangle near map edges.
 * @note (OGBUG) Towers of Wizardry can be selected as push destinations due to
 *       legacy post-move handling behavior.
 *
 * @see Unit_Space_At_Square(), Units_In_Tower(), Kill_Unit(), Square_Has_Lair(), Player_City_At_Square()
 */
void Evict_Unit(int16_t unit_idx)
{
    int16_t troops[2] = { 0, 0 };
    int16_t target_square_wy = 0;
    int16_t target_square_wx = 0;
    int16_t unit_owner_idx = 0;
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t unit_spaces = 0;
    int16_t found_square = 0;
    int16_t curr_wx = 0;
    int16_t range = 0;
    int16_t wx_start = 0;
    int16_t wy_start = 0;
    int16_t itr_wx = 0;
    int16_t itr_wy = 0;
    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_wp = _UNITS[unit_idx].wp;
    unit_owner_idx = _UNITS[unit_idx].owner_idx;
    wy_start = (unit_wy - 1);
    if(wy_start < 0)
    {
        wy_start = 0;  /* OGBUG:  drake178: BUG: range checking must be performed on the test coordinates, doing it here shifts the rectangle */
    }
    wx_start = (unit_wx - 1);
    if(wx_start < 0)
    {
        wx_start += WORLD_WIDTH;  /* OGBUG:  drake178: BUG: range checking must be performed on the test coordinates, doing it here shifts the rectangle */
    }
    range = 3;
    found_square = ST_FALSE;
    unit_spaces = 0;
    for(itr_wy = wy_start; ((itr_wy < (wy_start + range)) && (found_square == ST_FALSE)); itr_wy++)
    {
        if(itr_wy < WORLD_HEIGHT)
        {
            for(itr_wx = wx_start; itr_wx < (wx_start + range); itr_wx++)
            {
                if(found_square == ST_FALSE)
                {
                    if(itr_wx < WORLD_WIDTH)
                    {
                        curr_wx = itr_wx;
                    }
                    else
                    {
                        curr_wx = (itr_wx - WORLD_WIDTH);
                    }
                    if(
                        (curr_wx != _combat_wx)
                        ||
                        (itr_wy != _combat_wy)
                    )
                    {
                        if(
                            (Square_Has_Lair(curr_wx, itr_wy, unit_wp) == -1) &&
                            (Player_City_At_Square(curr_wx, itr_wy, unit_wp, unit_owner_idx) == -1)
                        )
                        {
                            unit_spaces = Unit_Space_At_Square(curr_wx, itr_wy, unit_wp, unit_owner_idx, unit_idx);
                            if(unit_spaces > 0)
                            {
                                found_square = ST_TRUE;
                                target_square_wx = curr_wx;
                                target_square_wy = itr_wy;
                            }
                        }
                    }
                }
            }
        }
    }
    if(unit_spaces > 0)
    {
        _UNITS[unit_idx].wx = (int8_t)target_square_wx;
        _UNITS[unit_idx].wy = (int8_t)target_square_wy;
        troops[0] = unit_idx;
        Units_In_Tower(1, &troops[0], _UNITS[unit_idx].wp);  /* OGBUG  drake178: Towers of Wizardry are not valid push destinations */
    }
    else
    {
        Kill_Unit(unit_idx, 1);
    }

}


// WZD o100p10
/*
Return:
0, if Unit's movement mode does not support the terrain type or there are already 9 units on the square
{1, ..., 9}, count of Units that could move to the target map square
~==
== 0, False
>= 1, True
*/
int16_t Unit_Space_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t unit_idx)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t trooper_idx = 0;
    int16_t have_windwalker = 0;
    int16_t unit_spaces = 0;
    int16_t troop_count = 0;
    int16_t itr = 0;  // _SI_
    int16_t is_legal_move = 0;  // _DI_

    Army_At_Square_2(wx, wy, wp, &troop_count, &troops[0]);

    if(
        (troop_count > 0) &&
        (
            (_UNITS[troops[0]].owner_idx != player_idx) ||
            (troop_count >= MAX_STACK)
        )
    )
    {
        goto Done_Return_Zero;
    }

    have_windwalker = ST_FALSE;

    for(itr = 0; ((itr < NUM_TOWERS) && (have_windwalker == ST_FALSE)); itr++)
    {
        if(
            (_TOWERS[itr].wx == wx)
            &&
            (_TOWERS[itr].wy == wy)
        )
        {
            goto Done_Return_Zero;
        }
    }

    is_legal_move = ST_FALSE;

    unit_spaces = 0;

    if(Square_Is_Sailable(wx, wy, wp) == ST_TRUE)
    {
        have_windwalker = ST_FALSE;
        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
        {
            is_legal_move = ST_TRUE;
            have_windwalker = ST_TRUE;
        }

        if(have_windwalker == ST_FALSE)
        {
            for(itr = 0; ((itr < troop_count) && (have_windwalker == ST_FALSE)); itr++)
            {
                trooper_idx = troops[itr];

                if(trooper_idx != -1)
                {
                    if(Unit_Has_WindWalking(trooper_idx) == ST_TRUE)
                    {
                        is_legal_move = ST_TRUE;
                        have_windwalker = ST_TRUE;
                    }
                }
            }
        }

        if(is_legal_move == ST_FALSE)
        {
            if(
                (Unit_Has_AirTravel(unit_idx) == ST_TRUE) ||
                (Unit_Has_WaterTravel(unit_idx) == ST_TRUE)
            )
            {
                is_legal_move = ST_TRUE;
            }
        }
    }
    else
    {
        if((_unit_type_table[_UNITS[unit_idx].type].Move_Flags & MV_SAILING) != 0)
        {
            if(Unit_Has_AirTravel(unit_idx) != ST_TRUE)
            {
                is_legal_move = ST_FALSE;
            }
            else
            {
                is_legal_move = ST_TRUE;
            }
        }
        else
        {
            is_legal_move = ST_TRUE;
        }
    }

    if(is_legal_move == ST_TRUE)
    {
        unit_spaces = (MAX_STACK - troop_count);
    }

    goto Done;


Done_Return_Zero:
    unit_spaces = 0;
    goto Done;


Done:

    return unit_spaces;
}


// WZD o100p11
// drake178: AI_UNIT_SeekTransprt()
/*
; decrease the transport waiting counter of the unit
; unless it is already marked as finished, changing its
; status back to ready if it either ran out, or there
; are also other units on its square
;
; RE-EXAMINE in more context!
*/
/*

*/
void AI_Unit_Army_Do_Ferry(uint16_t unit_idx)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t unit_owner_idx = 0;
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t troop_count = 0;

    /* Check if unit is already finished its turn */
    if(_UNITS[unit_idx].Finished == ST_TRUE)
    {
        return;
    }

    /* Extract unit data for army search */
    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_wp = _UNITS[unit_idx].wp;
    unit_owner_idx = _UNITS[unit_idx].owner_idx;

    /* Get list of units at the current location belonging to the owner */
    Player_Army_At_Square(unit_wx, unit_wy, unit_wp, unit_owner_idx, &troop_count, troops);

    /* 
     * In this AI context, dst_wx is repurposed as a counter/wait timer. 
     * The AI decrements it while waiting for transport or rendezvous.
     */
    _UNITS[unit_idx].dst_wx -= 1;

    /* 
     * Transition back to 'Ready' state if:
     * 1. The wait timer (dst_wx) has expired (< 1)
     * 2. There is more than one unit at the square (regrouped with army)
     */
    if(_UNITS[unit_idx].dst_wx < 1 || troop_count > 1)
    {
        _UNITS[unit_idx].Status = us_Ready;
        _UNITS[unit_idx].dst_wx = 0;
    }

}


// WZD o100p12
void AI_Unit_Army_Do_Road(int16_t unit_idx)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t construction_points = 0;
    int16_t troop_count = 0;
    int16_t unit_owner_idx = 0;
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t unit_dst_wy = 0;
    int16_t unit_dst_wx = 0;
    int16_t path_length = 0;
    int16_t itr_troops = 0;

    if(_UNITS[unit_idx].Finished == ST_TRUE)
    {
        return;
    }

    /* Extract unit data to local variables */
    /* Extract unit and target data */
    unit_wx = (int16_t)_UNITS[unit_idx].wx;
    unit_wy = (int16_t)_UNITS[unit_idx].wy;
    unit_wp = (int16_t)_UNITS[unit_idx].wp;
    unit_dst_wx = (int16_t)_UNITS[unit_idx].dst_wx;
    unit_dst_wy = (int16_t)_UNITS[unit_idx].dst_wy;
    unit_owner_idx = (int16_t)_UNITS[unit_idx].owner_idx;

    /* Get all units in the same square */
    Player_Army_At_Square(unit_wx, unit_wy, unit_wp, unit_owner_idx, &troop_count, troops);

    /* Calculate road path from current position to destination */
    path_length = OVL_GetRoadPath(unit_wx, unit_wy, unit_dst_wx, unit_dst_wy, unit_wp, &movepath_x_array[2], &movepath_y_array[2]);

    construction_points = 0;

    /* First pass: Calculate total construction power of the stack */
    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        if(_UNITS[troops[itr_troops]].Status == us_BuildRoad)
        {
            /* Add unit's base construction capability from the type table */
            construction_points += _unit_type_table[_UNITS[troops[itr_troops]].type].Construction;

            /* If unit has 'Endurance' enchantment/ability, it contributes more */
            if(Unit_Has_Endurance(troops[itr_troops]) == ST_TRUE)
            {
                construction_points++;
            }
        }
    }

    /* Second pass: Initialize road building status for all participating units */
    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        if(_UNITS[troops[itr_troops]].Status == us_BuildRoad)
        {
            /* Transition status to GOTO (AI will move and build along the path) */
            _UNITS[troops[itr_troops]].Status = us_GOTO;

            /* Calculate base turns/points required for the next segment in the path */
            _UNITS[troops[itr_troops]].Rd_Constr_Left = Turns_To_Build_Road((int)movepath_x_array[2], (int)movepath_y_array[2], unit_wp);

            /* Divide total required points by stack's construction capacity */
            _UNITS[troops[itr_troops]].Rd_Constr_Left = _UNITS[troops[itr_troops]].Rd_Constr_Left / construction_points;

            /* Store the starting point of the current road segment */
            _UNITS[troops[itr_troops]].Rd_From_X = unit_wx;
            _UNITS[troops[itr_troops]].Rd_From_Y = unit_wy;
        }
    }

}


// WZD o100p13
// drake178: TILE_CanShiftPlanes()
/*
; returns 1 if the stack on the square can shift to the
; other plane, or 0 otherwise
;
; contains multiple BUGs, many of which have to do with
; Non-Corporeal units
*/
/*

*/
int16_t Map_Square_Troops_Can_Plane_Shift(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t troops2[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wp2 = 0;
    int16_t Stack_Owner = 0;
    int16_t unit_dx = 0;
    int16_t Have_Windwalker = 0;
    int16_t UU_Local_0 = 0;
    int16_t troop_count2 = 0;
    int16_t troop_count = 0;
    int16_t itr_troops = 0;  // _SI_
    int16_t can_do = 0;  // _DI_


    if(Check_Planar_Seal() == ST_TRUE)
    {

        return ST_FALSE;

    }


    Army_At_Square_3(wx, wy, wp, &troop_count, &troops[0]);


    if(troop_count < 1)
    {

        return ST_FALSE;

    }


    Stack_Owner = _UNITS[troops[0]].owner_idx;


    if(
        (troop_count > 1)
        &&
        (
            (_UNITS[troops[0]].owner_idx != Stack_Owner)
            ||
            (troop_count >= MAX_STACK)
        )
    )
    {

        return ST_FALSE;

    }


    wp2 = (1 - wp);

    Army_At_Square_3(wx, wy, wp2, &troop_count2, &troops2[0]);


    if(
        (troop_count2 > 0)
        &&
        (
            (_UNITS[troops[0]].owner_idx != Stack_Owner)
            ||
            (troop_count > MAX_STACK)
            ||
            ((troop_count2 + troop_count) > MAX_STACK)
        )
    )
    {

        return ST_FALSE;

    }


// ; BUG: prevents shifting into the player's own cities,
// ; and yet doesn't prevent shifting into enemy cities

    if(
        (Square_Has_Lair(wx, wy, wp2) != ST_UNDEFINED)
        ||
        (Player_City_At_Square(wx, wy, wp2, Stack_Owner) != ST_UNDEFINED)
    )
    {

        return ST_FALSE;

    }


    can_do = 0;

    UU_Local_0 = 0;

    if(Square_Is_Sailable(wx, wy, wp2) != ST_TRUE)
    {

        can_do = ST_TRUE;

        for(itr_troops = 0; ((itr_troops < troop_count) & (can_do == ST_TRUE)); itr_troops++)
        {

            if(
                ((_unit_type_table[_UNITS[troops[itr_troops]].type].Move_Flags & MV_SAILING) == 0)
                ||
                (Unit_Has_AirTravel(troops[itr_troops]) != ST_TRUE)  // ; BUG: ignores Non-Corporeal
            )
            {

                can_do = ST_FALSE;

            }
            else
            {

                can_do = ST_TRUE;

            }

        }

    }
    else
    {

        Have_Windwalker = ST_FALSE;

        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
        {

            if(Unit_Has_WindWalking(troops[itr_troops]) == ST_TRUE)
            {

                can_do = ST_TRUE;

                Have_Windwalker = ST_TRUE;

            }

        }

        if(Have_Windwalker == ST_FALSE)
        {

            Have_Windwalker = ST_FALSE;

            for(itr_troops = 0; ((itr_troops < troop_count2) & (Have_Windwalker == ST_FALSE)); itr_troops++)
            {

                unit_dx = troops2[itr_troops];

                // ; conflicting condition - will never jump
                if(unit_dx != ST_UNDEFINED)
                {

                    if(Unit_Has_WindWalking(unit_dx) == ST_TRUE)
                    {

                        can_do = ST_TRUE;

                        Have_Windwalker = ST_TRUE;
                        
                    }

                }

            }

            if(Have_Windwalker == ST_FALSE)
            {

                for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                {

                    if(
                        (Unit_Has_AirTravel(troops[itr_troops]) != ST_TRUE)
                        ||
                        (Unit_Has_AirTravel_Item(troops[itr_troops]) != ST_TRUE)
                        ||
                        (Unit_Has_Swimming(troops[itr_troops]) != ST_TRUE)  // ; BUG: ignores Non-Corporeal
                        ||
                        (Unit_Has_WaterTravel_Item(troops[itr_troops]) != ST_TRUE)  // ; BUG: ignores Non-Corporeal (Wraithform item)
                    )
                    {

                        can_do = ST_FALSE;  // Dasm looks like `return;`

                    }
                    else
                    {

                        can_do = ST_TRUE;

                    }

                }

            }

        }

    }

    return can_do;

}
