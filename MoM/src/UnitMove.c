/*
    WIZARDS.EXE
        ovr071
        ovr148

MoO2
    Module: SHIPMOVE

*/
/*
Movement Points

Movement Types

? Planar Travel ?


Elsewhere, ...
    ? Unit vs. Stack ?
    UNIT_
    STAK_

*/

#include "../../MoX/src/Util.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/paragrph.h"
#include "../../MoX/src/EMS/EMS.h"

#include "MainScr.h"
#include "UNITTYPE.h"
#include "UnitMove.h"

#include "Combat.h"
#include "Terrain.h"
#include "MovePath.h"

#include <string.h>     /* memcpy(), memset() */






// WZD  dseg:6F76                                                 BEGIN:  ovr148 - Initialized Data

// WZD  dseg:6F76
// ; an index into OvlMovePaths_EMS@
int16_t _ai_move_path_idx = ST_UNDEFINED;

// WZD  dseg:6F76                                                 END:  ovr148 - Initialized Data




/*
    WIZARDS.EXE  ovr071
*/

// WZD o71p01
// drake178: OVL_GetMoveTypes()
/*
    wrapper for Army_Movement_Modes()
    that assumes _unit_stack
*/
void Active_Stack_Movement_Modes(int16_t movement_mode_flags[])
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t stack_has_no_active_units = 0;
    int16_t troop_count = 0;
    int16_t itr = 0;

    stack_has_no_active_units = ST_TRUE;

    for(itr = 0; (itr < _unit_stack_count) && (stack_has_no_active_units == ST_TRUE); itr++)
    {
        if(_unit_stack[itr].active != ST_FALSE)
        {
            stack_has_no_active_units = ST_FALSE;
        }
    }

    if(stack_has_no_active_units == ST_TRUE)
    {
        for(itr = 0; itr < _unit_stack_count; itr++)
        {
            _unit_stack[itr].active = ST_TRUE;
        }
    }

    troop_count = 0;

    for(itr = 0; itr < _unit_stack_count; itr++)
    {
        if(_unit_stack[itr].active == ST_TRUE)
        {
            troops[troop_count] = _unit_stack[itr].unit_idx;
            troop_count++;
        }
    }

    Army_Movement_Modes(movement_mode_flags, &troops[0], troop_count);

    if(stack_has_no_active_units == ST_TRUE)
    {
        for(itr = 0; itr < _unit_stack_count; itr++)
        {
            _unit_stack[itr].active = ST_FALSE;
        }
    }

}


// WZD o71p02
// drake178: STK_GetMoveTypes()
void Army_Movement_Modes(int16_t movement_mode_flags[], int16_t troops[], int16_t troop_count)
{
    int16_t l_movement_modes_array[6];
    int16_t Swimming_Units;
    int16_t Flying_Units;
    uint32_t item_enchantments;
    int16_t Units_With_Same;
    int16_t unit_type;
    int16_t unit_idx;
    int16_t itr_troops;
    int16_t itr_modes;  // _SI_

    // SCOPY@(sizeof(12), l_movement_modes_array, MoveType_Flags);
    // memcpy(l_movement_modes_array, MoveFlag_Array, 12);
    memcpy(l_movement_modes_array, &movement_modes_array, 12);

    Flying_Units = 0;
    Swimming_Units = 0;

    for(itr_modes = 0; itr_modes < 6; itr_modes++)
    {
        movement_mode_flags[itr_modes] = ST_FALSE;

        Units_With_Same = 0;
        item_enchantments = 0;
        
        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
        {
            unit_idx = troops[itr_troops];
            unit_type = _UNITS[unit_idx].type;
            item_enchantments = 0;
            if(_UNITS[unit_idx].Hero_Slot != -1)
            {
                item_enchantments = BU_Apply_Item_Powers(unit_idx, global_battle_unit);
                item_enchantments = global_battle_unit->item_enchantments;
            }

            if((_unit_type_table[unit_type].Move_Flags & l_movement_modes_array[itr_modes]) == MV_FORESTER)
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }
            if((_unit_type_table[unit_type].Move_Flags & l_movement_modes_array[itr_modes]) == MV_MOUNTAINEER)
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }
            if((_unit_type_table[unit_type].Move_Flags & l_movement_modes_array[itr_modes]) == MV_SAILING)
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if(
                ((_unit_type_table[unit_type].Move_Flags & movement_mode_flags[itr_modes]) == MV_FLYING) &&
                (_unit_type_table[unit_type].Transport > 0)
            )
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if(
                (l_movement_modes_array[itr_modes] == MV_FORESTER) &&
                (
                    ((_UNITS[unit_idx].enchantments & UE_PATH_FINDING) != 0) ||
                    ((item_enchantments & UE_PATH_FINDING) != 0)
                )
            )
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if(
                (l_movement_modes_array[itr_modes] == MV_MOUNTAINEER) &&
                (
                    ((_UNITS[unit_idx].enchantments & UE_PATH_FINDING) != 0) ||
                    ((item_enchantments & UE_PATH_FINDING) != 0)
                )
            )
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if(
                (movement_mode_flags[itr_modes] == MV_FLYING) &&
                (
                    ((_UNITS[unit_idx].enchantments & UE_WIND_WALKING) != 0) ||
                    ((_unit_type_table[unit_type].Abilities & UA_WINDWALKING) != 0)
                )
            )
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if(
                (movement_mode_flags[itr_modes] == MV_FLYING) &&
                ((_UNITS[unit_idx].enchantments & (UE_WIND_WALKING | UE_FLIGHT)) != 0) &&
                (_unit_type_table[unit_type].Transport > 0)
            )
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if((_unit_type_table[unit_type].Move_Flags & l_movement_modes_array[itr_modes]) == l_movement_modes_array[itr_modes])
            {
                Units_With_Same++;

                if(l_movement_modes_array[itr_modes] == MV_SWIMMING)
                {
                    Swimming_Units++;
                }

                if(l_movement_modes_array[itr_modes] == MV_FLYING)
                {
                    Flying_Units++;  // BUGBUG:  drake178: applying this independently of artificial flight means that units that have both count as 2 flyers, and will each allow a ground unit to be carried along
                }

                if(
                    ((_unit_type_table[unit_type].Abilities & UA_NONCORPOREAL) != 0) ||
                    ((_UNITS[unit_idx].enchantments & UE_WRAITH_FORM) != 0) ||
                    ((item_enchantments & UE_WRAITH_FORM) != 0)
                )
                {
                    Units_With_Same--;
                }

                /* OGBUG  drake178: misplaced parentheses in the original code —
                   the condition is supposed to skip all 4 of the following blocks,
                   not just the first one.  Preserved faithfully via C operator
                   precedence (&& binds tighter than ||).  Explicit parentheses
                   below make that intent visible to the compiler and silence
                   GCC's -Wparentheses; do NOT regroup as ((MV_FLYING || …) && …)
                   — that would be the "fixed" behavior, not OG-faithful. */
                if(
                    (
                        (l_movement_modes_array[itr_modes] == MV_FLYING)
                        &&
                        ((_unit_type_table[unit_type].Abilities & UA_NONCORPOREAL) != 0)
                    )
                    ||
                    ((_UNITS[unit_idx].enchantments & UE_WRAITH_FORM) != 0)
                    ||
                    ((item_enchantments & UE_WRAITH_FORM) != 0)
                )
                {
                    Swimming_Units--;
                }

            }

            if
            (
                (l_movement_modes_array[itr_modes] == MV_SWIMMING) &&
                (
                    ((_UNITS[unit_idx].enchantments & UE_WATER_WALKING) != 0) ||
                    ((item_enchantments & UE_WATER_WALKING) != 0)
                )
            )
            {
                Units_With_Same++;
                Swimming_Units++;  // BUGBUG:  drake178: natural swimmers with water walking now count as 2
            }

            if
            (
                (l_movement_modes_array[itr_modes] == MV_SWIMMING) &&
                (
                    ((_unit_type_table[unit_type].Abilities & UA_NONCORPOREAL) != 0) ||
                    ((_UNITS[unit_idx].enchantments & UE_WRAITH_FORM) != 0) ||
                    ((item_enchantments & UE_WRAITH_FORM) != 0)
                )
            )
            {
                Units_With_Same++;
                Swimming_Units++;  // BUGBUG:  drake178: natural swimmers with water walking now count as 2
            }

            if(
                (l_movement_modes_array[itr_modes] == MV_FLYING) &&
                (
                    ((_UNITS[unit_idx].enchantments & (UE_WIND_WALKING | UE_FLIGHT)) != 0) ||
                    ((item_enchantments & (UE_WIND_WALKING | UE_FLIGHT)) != 0) ||
                    ((_UNITS[unit_idx].mutations & CC_FLIGHT) != 0)
                )
            )
            {
                Units_With_Same++;
                Flying_Units++;

                if(
                    ((_UNITS[unit_idx].enchantments & UE_WATER_WALKING) != 0) ||
                    ((item_enchantments & UE_WATER_WALKING) != 0) ||
                    ((_unit_type_table[unit_type].Abilities & UA_NONCORPOREAL) != 0) ||
                    ((_UNITS[unit_idx].enchantments & UE_WRAITH_FORM) != 0) ||
                    ((item_enchantments & UE_WRAITH_FORM) != 0)
                )
                {
                    Swimming_Units--;
                }
                
            }
        }

        if(Units_With_Same >= troop_count)
        {
            movement_mode_flags[itr_modes] = ST_TRUE;
        }

    }

    if((Flying_Units + Swimming_Units) >= troop_count)
    {
        movement_mode_flags[3] = ST_TRUE;
        if(Flying_Units < 1 || Swimming_Units < 1)
        {
            movement_mode_flags[3] = ST_TRUE;
            movement_mode_flags[5] = ST_TRUE;
            if(Flying_Units == 0)
            {
                movement_mode_flags[5] = ST_FALSE;
            }
            if(Swimming_Units == 0)
            {
                movement_mode_flags[3] = ST_FALSE;
            }
        }
    }

    movement_mode_flags[0] = ST_FALSE;

    Units_With_Same = 0;

    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        unit_idx = troops[itr_troops];
        unit_type = _UNITS[unit_idx].type;
        if(
            ((_unit_type_table[unit_type].Abilities & UA_NONCORPOREAL) != 0) ||
            ((_UNITS[unit_idx].enchantments & UE_WRAITH_FORM) != 0)  // BUGBUG:  drake178: ignores the item power
            /* || ((item_enchantments & UE_WRAITH_FORM) != 0) */
        )
        {
            Units_With_Same++;
        }
    }

    if(Units_With_Same == troop_count)
    {
        movement_mode_flags[1] = ST_TRUE;
        movement_mode_flags[2] = ST_TRUE;
    }

}


// WZD o71p03
int16_t Unit_Has_AirTravel(int16_t unit_idx)
{
    int16_t unit_type;
    int16_t has_airtravel;

    has_airtravel = ST_FALSE;

    unit_type = _UNITS[unit_idx].type;

    if((_unit_type_table[unit_type].Move_Flags & MV_FLYING) != 0)
    {
        has_airtravel = ST_TRUE;
    }

    
    if(
        ((_UNITS[unit_idx].enchantments & UE_WIND_WALKING) != 0)
        ||
        ((_UNITS[unit_idx].enchantments & UE_FLIGHT) != 0)
    )
    {
        has_airtravel = ST_TRUE;
    }
    
    if( (_UNITS[unit_idx].mutations & CC_FLIGHT) != 0)
    {
        has_airtravel = ST_TRUE;
    }

    return has_airtravel;
}

// WZD o71p04
// TODO  DEDU  warning: bitwise comparrison always evaluates to false
int16_t Unit_Has_WindWalking(int16_t unit_idx)
{
    int16_t has_windwalking;

    has_windwalking = ST_FALSE;

    if(
        ((_UNITS[unit_idx].enchantments & UE_WIND_WALKING) != 0)
        ||
        ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_WINDWALKING) != 0)
    )
    {
        has_windwalking = ST_TRUE;
    }

    return has_windwalking;
}


// WZD o71p05
/*
    Movement Mode Sailing
    Movement Mode Swimming
    Unit Enchantment Water Walking
*/
int16_t Unit_Has_WaterTravel(int16_t unit_idx)
{
    int16_t unit_type;
    int16_t has_watertravel;

    has_watertravel = ST_FALSE;

    unit_type = _UNITS[unit_idx].type;

    if((_unit_type_table[unit_type].Move_Flags & MV_SAILING) != 0)
    {
        has_watertravel = ST_TRUE;
    }
    
    if((_unit_type_table[unit_type].Move_Flags & MV_SWIMMING) != 0)
    {
        has_watertravel = ST_TRUE;
    }

    if((_UNITS[unit_idx].enchantments & UE_WATER_WALKING) != 0)
    {
        has_watertravel = ST_TRUE;
    }

    return has_watertravel;
}


// WZD o71p06
/*
    Movement Mode Sailing
*/
int16_t Unit_Has_Sailing(int16_t unit_idx)
{
    int16_t unit_type;
    int16_t has_sailing;

    has_sailing = ST_FALSE;

    unit_type = _UNITS[unit_idx].type;

    if((_unit_type_table[unit_type].Move_Flags & MV_SAILING) != 0)
    {
        has_sailing = ST_TRUE;
    }

    return has_sailing;
}

// WZD o71p07
int16_t Unit_Has_Swimming(int16_t unit_idx)
{
    int16_t has_swimming;
    int16_t unit_type;

    has_swimming = ST_FALSE;

    unit_type = _UNITS[unit_idx].type;

    if( (_unit_type_table[unit_type].Move_Flags & MV_SWIMMING) != 0)
    {
        has_swimming = ST_TRUE;
    }

    if((_UNITS[unit_idx].enchantments & UE_WATER_WALKING) != 0)
    {
        has_swimming = ST_TRUE;
    }

    return has_swimming;
}

// WZD o71p08
// int16_t UNIT_ReturnZero(int16_t unit_idx);

// WZD o71p09
int16_t Unit_Has_WaterTravel_Item(int16_t unit_idx)
{
// Meh.    uint32_t niu_item_enchantments = 0;
// Meh.    int16_t has_watertravel_item = 0;  // DNE in Dasm

// Meh?    has_watertravel_item = ST_FALSE;

// Meh.    if(_UNITS[unit_idx].Hero_Slot != -1)
// Meh.    {
// Meh.        niu_item_enchantments = BU_Apply_Item_Powers(unit_idx, global_battle_unit);
// Meh.
// Meh.        if(
// Meh.            ((global_battle_unit->item_enchantments & UE_WIND_WALKING) != 0)
// Meh.            ||
// Meh.            ((global_battle_unit->item_enchantments & UE_FLIGHT) != 0)
// Meh.            )
// Meh.        {
// Meh.            has_watertravel_item = ST_TRUE;
// Meh.        }
// Meh.
// Meh.    }
// Meh.    else
// Meh.    {
// Meh.        has_watertravel_item = ST_FALSE;
// Meh.    }
// Meh.
// Meh.    return has_watertravel_item;

// Meh?    if(
// Meh?        (_UNITS[unit_idx].Hero_Slot == ST_UNDEFINED)
// Meh?        ||
// Meh?        ((niu_item_enchantments = BU_Apply_Item_Powers(unit_idx, global_battle_unit) & UE_WATER_WALKING) == 0)
// Meh?    )
// Meh?    {
// Meh?        return ST_FALSE;
// Meh?    }
// Meh?    else
// Meh?    {
// Meh?        return ST_TRUE;
// Meh?    }

    if(
        (_UNITS[unit_idx].Hero_Slot == ST_UNDEFINED)
        ||
        ((BU_Apply_Item_Powers(unit_idx, global_battle_unit) & UE_WATER_WALKING) == 0)
        )
    {
        return ST_FALSE;
    }
    else
    {
        return ST_TRUE;
    }

}


// WZD o71p010
int16_t Unit_Has_AirTravel_Item(int16_t unit_idx)
{
    uint32_t niu_item_enchantments;
    int16_t has_airtravel_item = ST_FALSE;

    if(_UNITS[unit_idx].Hero_Slot != -1)
    {
        niu_item_enchantments = BU_Apply_Item_Powers(unit_idx, global_battle_unit);

        if(
            ( (global_battle_unit->item_enchantments & UE_WIND_WALKING) != 0) ||
            ( (global_battle_unit->item_enchantments & UE_FLIGHT) != 0)
        )
        {
            has_airtravel_item = ST_TRUE;
        }
    }

    return has_airtravel_item;
}

// WZD o71p011
int16_t Unit_Has_Invisibility(int16_t unit_idx)
{
    int32_t UU_battle_unit_item_enchantments;

    if(_UNITS[unit_idx].Hero_Slot != ST_UNDEFINED)
    {
        UU_battle_unit_item_enchantments = BU_Apply_Item_Powers(unit_idx, global_battle_unit);
        if((global_battle_unit->item_enchantments & UE_INVISIBILITY) != 0)
        {
            return ST_TRUE;
        }
    }

    if(
        ((_UNITS[unit_idx].enchantments & UE_INVISIBILITY) != 0)
        ||
        ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_INVISIBILITY) != 0)
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }
}

// WZD o71p012
/* OGBUG  does not check hero items */
/**
 * @brief Checks whether a unit currently has the Endurance enchantment.
 *
 * This is a direct unit-enchantment test against @c UE_ENDURANCE on the
 * referenced unit entry in @c _UNITS.
 *
 * @param unit_idx Index of the unit in the global @c _UNITS table.
 *
 * @return @c ST_TRUE when the unit's enchantment bitfield contains
 *         @c UE_ENDURANCE; otherwise @c ST_FALSE.
 */
int16_t Unit_Has_Endurance(int16_t unit_idx)
{
    if((_UNITS[unit_idx].enchantments & UE_ENDURANCE) != 0)
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }
}


// WZD o71p013
int16_t Unit_Has_PlanarTravel_Item(int16_t unit_idx)
{
    int16_t has_planartravel_item;
    uint32_t niu_item_enchantments;

    if(_UNITS[unit_idx].Hero_Slot != ST_UNDEFINED)
    {

        niu_item_enchantments = BU_Apply_Item_Powers(unit_idx, global_battle_unit);

        if(
            ( (global_battle_unit->item_enchantments & UE_PLANAR_TRAVEL) != 0)
        )
        {
            has_planartravel_item = ST_TRUE;
        }
        else
        {
            has_planartravel_item = ST_FALSE;
        }
        
    }
    else
    {
        has_planartravel_item = ST_FALSE;
    }

    return has_planartravel_item;
}

// WZD o71p014
/**
 * @brief Determine whether a unit currently has non-corporeal movement.
 *
 * Performs the full overland check for non-corporeality by testing the unit's
 * innate `UA_NONCORPOREAL` ability, the unit-level `UE_WRAITH_FORM`
 * enchantment, and hero item powers that grant `UE_WRAITH_FORM` after applying
 * item effects into `global_battle_unit`.
 *
 * @param unit_idx Global unit index to evaluate.
 *
 * @return `ST_TRUE` if the unit is naturally non-corporeal or currently gains
 *         Wraith Form from enchantments or hero items; otherwise `ST_FALSE`.
 *
 * @note Hero item powers are only checked when the unit occupies a valid hero
 *       slot.
 */
/* NOTE: this is an actual FULL check, considering natural ability, wraith form enchantment, and item powers */
/* NOTE: AI_Stacks_Wartime_Ocean_Movement_And_Cleanup() considers this to mean 'Seafaring' */
int16_t Unit_Has_NonCorporeal(int16_t unit_idx)
{
    uint32_t niu_item_enchantments = 0;
    int16_t unit_type = 0;
    unit_type = _UNITS[unit_idx].type;
    if((_unit_type_table[unit_type].Abilities & UA_NONCORPOREAL) != 0)
    {
        return ST_TRUE;
    }
    if((_UNITS[unit_idx].enchantments & UE_WRAITH_FORM) != 0)
    {
        return ST_TRUE;
    }
    if(_UNITS[unit_idx].Hero_Slot != ST_UNDEFINED)
    {
        niu_item_enchantments = BU_Apply_Item_Powers(unit_idx, global_battle_unit);
        if((global_battle_unit->item_enchantments & UE_WRAITH_FORM) != 0)
        {
            return ST_TRUE;
        }
    }
    return ST_FALSE;
}



/*
    WIZARDS.EXE  ovr148
*/

// WZD o148p01
// UU_OVL_Return_2()
// WZD o148p02
// UU_OVL_GetLinePath()


// WZD o148p03
// STK_GetPath()
/*
¿ start of overland move-mode/path / pathfinding ?

    populates movepath_cost_map path, x's, y's, and cost's
    returns path length

    Return:
        int16_t movepath_length
            ¿ count of map sqaures from source to destion ?
            returns 0 if Init_MovePathMap() or Update_MovePathMap() determine the destion map square is *impassible*
    
    "Army", not "Stack" - takes troop_count, doesn't touch _unit_stack, _unit_stack_count
    ¿ Units_Move_Path() ?

    ¿ UU_PathingVar1:  debug counter for movement path cache hits ?
    ¿ UU_PathingVar2:  debug counter for movement path cache misses ?
    // WZD dseg:CA8A  UU_PathingVar1 dw 0
    // WZD dseg:CA8C  UU_PathingVar2 dw 0
    UU_DBG_MovePatchCache_Hits
    UU_DBG_MovePatchCache_Misses
    DEDUCE: ¿ the usage here, incrementing without initializing, is another clue that variables initialized to zero can/do get put in the Uninitialized Data Data Segment ?

*/
/*
    Overland shortest-path driver — see doc/PathFinding/MoM-MovePath-Compare.md ("Move_Path_Find — overland").

    The overland solver SPLITS the shared 5-step skeleton across two functions, where Find_Shortest_Path
    (MAPGEN.c) does all five in a single body:
        [Skeleton 1 + 3]  init parallel arrays + relaxation sweep  -> delegated to Move_Path_Find() (MovePath.c)
        [Skeleton 2]      bail if the destination tile is impassable
        [Skeleton 4]      back-trace dst -> Reach_From[] self-link
        [Skeleton 5]      reverse + 1-D index -> (x, y)
    The inline [Skeleton step N] labels below mark steps 2/4/5; steps 1/3 run inside the Move_Path_Find() call.
*/
int16_t Make_Move_Path(int16_t MvMd_0, int16_t MvMd_1, int16_t MvMd_2, int16_t MvMd_3, int16_t MvMd_4, int16_t MvMd_5, int16_t src_wx, int16_t src_wy, int16_t dst_wx, int16_t dst_wy, int16_t wp, int8_t mvpth_x[], int8_t mvpth_y[], int8_t mvpth_c[], int16_t UU_flag, int16_t UU_moves2, int16_t boatrider_count, int16_t troop_count, int16_t player_idx)
{
    int16_t ext_y2 = 0;
    int16_t ext_x2 = 0;
    int16_t ext_y1 = 0;
    int16_t ext_x1 = 0;
    int16_t dst_world_map_idx = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    int16_t itr = 0;  // _SI_
    int16_t path_length = 0;  // _DI_
    int16_t * ptr_reached_from = 0;  // DNE in Dasm
    int16_t reached_from = 0;  // DNE in Dasm

    EMMDATAH_Map();

    UU_flag = ST_TRUE;
    UU_moves2 = 8;

    _ai_move_path_idx = ST_UNDEFINED;

    if(player_idx == HUMAN_PLAYER_IDX)
    {
        UU_flag = ST_FALSE;
        goto Calc_Move_Path;
    }
    else
    {
        goto Move_Path_Cache;
    }


Move_Path_Cache:
{

    // HACK:  hard-coded MovePath Cache Miss 
    goto Calc_Move_Path;

    goto Done;
}


Calc_Move_Path:
{

    do
    {
        Init_MovePathMap(MvMd_0, MvMd_1, MvMd_2, MvMd_3, MvMd_4, MvMd_5, wp);

        // HERE: UU_moves2 has been hard-coded to 8, so ALWAYS jumps over
        // min 2 cost
        if((player_idx == HUMAN_PLAYER_IDX) && (UU_moves2 == 1))
        {
            for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
            {
                for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
                {
                    if(
                        (movepath_cost_map->moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] != ST_UNDEFINED) &&
                        (movepath_cost_map->moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] > 2)
                    )
                    {
                        movepath_cost_map->moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 2;
                    }
                }
            }
        }

        // sets movepath_cost_map->moves2[] from movement_mode_cost_maps[wp].UU_MvMd, walking, forester, mountaineer, swimming, sailing
        Update_MovePathMap(&movepath_cost_map->moves2[0], boatrider_count, troop_count, wp, player_idx, dst_wx, dst_wy, src_wx, src_wy);

        /* [Skeleton step 2]  Bail if the destination tile is impassable (MoM-MovePath-Compare.md, "The shared skeleton"); returns path_length 0 via Done_Return_Zero. */
        if(movepath_cost_map->moves2[((dst_wy * WORLD_WIDTH) + dst_wx)] == (int8_t)INF)  /* CLAUDE  (int8_t)INF == -1; impassable byte 0xFF on this signed moves2 map */
        {
            goto Done_Return_Zero;
        }

        if(UU_flag != ST_TRUE)
        {
            break;
        }


// TODO          // TODO  TILE_ExtendRange(src_wx, src_wy, dst_wx, dst_wy, &ext_x1, &ext_y1, &ext_x2, &ext_y2);
// TODO          // iter 0 < ext_y1, 0 < WORLD_WIDTH
// TODO          // iter ext_2+1 < WORLD_HEIGHT, 0 < WORLD_WIDTH
// TODO          // if(ext_x1 >= ext_x2)
// TODO          // iter ext_y1 < ext_y2,ext_x2 < ext_y1
// TODO          // else
// TODO          // itr ext_y1 < ext_y2, 0 < ext_x1
// TODO  
// TODO          // Move_Path_Find(src_wx, src_wy, (int8_t *)&movepath_cost_map[0]);
// TODO  
// TODO          path_length = 0;
// TODO          dst_world_map_idx = ((dst_wy * WORLD_WIDTH) + dst_wx);
// TODO          while(movepath_cost_map->Reach_From[dst_world_map_idx] != dst_world_map_idx)
// TODO          {
// TODO              movepath_cost_map->Reverse_Path[path_length] = dst_world_map_idx;
// TODO              dst_world_map_idx = movepath_cost_map->Reach_From[dst_world_map_idx];
// TODO              path_length++;
// TODO          }
// TODO  
// TODO          for(itr = 0; itr < path_length; itr++)
// TODO          {
// TODO              itr_wx = (movepath_cost_map->Reverse_Path[((path_length - 1) - itr)] % WORLD_WIDTH);
// TODO              itr_wy = (movepath_cost_map->Reverse_Path[((path_length - 1) - itr)] / WORLD_WIDTH);
// TODO              mvpth_x[itr] = itr_wx;
// TODO              mvpth_y[itr] = itr_wy;
// TODO              mvpth_c[itr] = movepath_cost_map->moves2[((itr_wy * WORLD_WIDTH) + itr_wx)];
// TODO          }
// TODO  
// TODO          if(path_length != 0)
// TODO          {
// TODO              break;
// TODO          }
        
        UU_flag = ST_FALSE;

    } while(UU_flag == ST_FALSE);


    if(UU_flag == ST_FALSE)
    {

        /* [Skeleton steps 1 + 3]  Move_Path_Find runs the array init + relaxation sweep, filling Reach_Costs[]/Reach_From[]. */
        Move_Path_Find(src_wx, src_wy, movepath_cost_map);

        /* [Skeleton step 4]  Back-trace from the destination through Reach_From[] until a self-link, collecting Reverse_Path[] (path_length stays 0 if dst was never reached). */
        path_length = 0;
        dst_world_map_idx = ((dst_wy * WORLD_WIDTH) + dst_wx);
        ptr_reached_from = &movepath_cost_map->Reach_From[dst_world_map_idx];
        reached_from = *ptr_reached_from;
        while(movepath_cost_map->Reach_From[dst_world_map_idx] != dst_world_map_idx)
        {
            movepath_cost_map->Reverse_Path[path_length] = dst_world_map_idx;
            dst_world_map_idx = movepath_cost_map->Reach_From[dst_world_map_idx];
            path_length++;
        }

        /* [Skeleton step 5]  Reverse the collected indices and convert each 1-D index to (x, y) -> mvpth_x/y/c[]. */
        for(itr = 0; itr < path_length; itr++)
        {
            itr_wx = (movepath_cost_map->Reverse_Path[((path_length - 1) - itr)] % WORLD_WIDTH);
            itr_wy = (movepath_cost_map->Reverse_Path[((path_length - 1) - itr)] / WORLD_WIDTH);
            mvpth_x[itr] = (int8_t)itr_wx;
            mvpth_y[itr] = (int8_t)itr_wy;
            mvpth_c[itr] = movepath_cost_map->moves2[((itr_wy * WORLD_WIDTH) + itr_wx)];
        }

    }

    Init_MovePathMap(MvMd_0, MvMd_1, MvMd_2, MvMd_3, MvMd_4, MvMd_5, wp);

    // HERE: UU_moves2 has been hard-coded to 8, so ALWAYS jumps over
    if((player_idx == HUMAN_PLAYER_IDX) && (UU_moves2 == 1))
    {
        // TODO  min 2 costs
    }

    Update_MovePathMap(&movepath_cost_map->moves2[0], boatrider_count, troop_count, wp, player_idx, dst_wx, dst_wy, src_wx, src_wy);

}

    goto Done;


Done_Return_Zero:
    path_length = 0;
    goto Done;


Done:

    return path_length;
}
/* GEMINI */
#if 0
int Make_Move_Path__GEMINI(int MvMd_0, int MvMd_1, int MvMd_2, int MvMd_3, int MvMd_4, int MvMd_5,
                   int src_wx, int src_wy, int dst_wx, int dst_wy, int wp,
                   uint8_t *mvpth_x, uint8_t *mvpth_y, uint8_t *mvpth_c,
                   int UU_flag, int UU_moves2, int boatrider_count, int troop_count, int player_idx)
{
    /* Local variables mapped from stack frame */
    int ext_y2;             /* [bp-0Eh] */
    int ext_x2;             /* [bp-0Ch] */
    int ext_y1;             /* [bp-0Ah] */
    int ext_x1;             /* [bp-08h] */
    int dst_world_map_idx;  /* [bp-06h] */
    int itr_wy;             /* [bp-04h] */
    int itr_wx;             /* [bp-02h] */
    int _SI_itr;            /* si */
    int _DI_path_length;    /* di */

    /* Antigravity */
    EMMDATAH_Map();

    UU_flag = e_ST_TRUE;
    UU_moves2 = 8;

    ai_move_path_idx = e_ST_UNDEFINED;

    if (player_idx == e_HUMAN_PLAYER_IDX)
    {
        UU_flag = e_ST_FALSE;
        goto loc_D5B50;
    }

    for (_SI_itr = 0; _SI_itr < 140 && ai_move_path_table[_SI_itr].src_wx != e_ST_UNDEFINED; _SI_itr++)
    {
        if (ai_move_path_table[_SI_itr].src_wx == src_wx &&
            ai_move_path_table[_SI_itr].src_wy == src_wy &&
            ai_move_path_table[_SI_itr].dst_wx == dst_wx &&
            ai_move_path_table[_SI_itr].dst_wy == dst_wy &&
            ai_move_path_table[_SI_itr].Plane == wp)
        {
            ai_move_path_idx = _SI_itr;
        }
    }

    if (ai_move_path_idx != e_ST_UNDEFINED)
    {
        _fmemcpy(mvpth_x, ai_move_path_table[ai_move_path_idx].wx_array, ai_move_path_table[ai_move_path_idx].Length);
        _fmemcpy(mvpth_y, ai_move_path_table[ai_move_path_idx].wy_array, ai_move_path_table[ai_move_path_idx].Length);
        _fmemcpy(mvpth_c, ai_move_path_table[ai_move_path_idx].cost_array, ai_move_path_table[ai_move_path_idx].Length);
        
        UU_DBG_MovePatchCache_Hits++;
        
        return ai_move_path_table[ai_move_path_idx].Length;
    }
    else
    {
        UU_DBG_MovePatchCache_Misses++;
    }

loc_D5B50:
    Init_MovePathMap(MvMd_0, MvMd_1, MvMd_2, MvMd_3, MvMd_4, MvMd_5, wp);

    if (player_idx == e_HUMAN_PLAYER_IDX && UU_moves2 == 1)
    {
        for (itr_wy = 0; itr_wy < e_WORLD_HEIGHT; itr_wy++)
        {
            for (itr_wx = 0; itr_wx < e_WORLD_WIDTH; itr_wx++)
            {
                if (movepath_cost_map->moves2[itr_wy * e_WORLD_WIDTH + itr_wx] != e_ST_UNDEFINED_DB)
                {
                    if (movepath_cost_map->moves2[itr_wy * e_WORLD_WIDTH + itr_wx] > 2)
                    {
                        movepath_cost_map->moves2[itr_wy * e_WORLD_WIDTH + itr_wx] = 2;
                    }
                }
            }
        }
    }

    Update_MovePathMap((int8_t far *)movepath_cost_map, boatrider_count, troop_count, wp, player_idx, dst_wx, dst_wy, src_wx, src_wy);

    if (movepath_cost_map->moves2[dst_wy * e_WORLD_WIDTH + dst_wx] == e_ST_UNDEFINED_DB)
    {
        return 0;
    }

    if (UU_flag == e_ST_TRUE)
    {
        TILE_ExtendRange(src_wx, src_wy, dst_wx, dst_wy, &ext_x1, &ext_y1, &ext_x2, &ext_y2);

        for (itr_wy = 0; itr_wy < ext_y1; itr_wy++)
        {
            for (itr_wx = 0; itr_wx < e_WORLD_WIDTH; itr_wx++)
            {
                movepath_cost_map->moves2[itr_wy * e_WORLD_WIDTH + itr_wx] = e_ST_UNDEFINED_DB;
            }
        }

        for (itr_wy = ext_y2 + 1; itr_wy < e_WORLD_HEIGHT; itr_wy++)
        {
            for (itr_wx = 0; itr_wx < e_WORLD_WIDTH; itr_wx++)
            {
                movepath_cost_map->moves2[itr_wy * e_WORLD_WIDTH + itr_wx] = e_ST_UNDEFINED_DB;
            }
        }

        if (ext_x1 < ext_x2)
        {
            for (itr_wy = ext_y1; itr_wy <= ext_y2; itr_wy++)
            {
                for (itr_wx = 0; itr_wx < ext_x1; itr_wx++)
                {
                    movepath_cost_map->moves2[itr_wy * e_WORLD_WIDTH + itr_wx] = e_ST_UNDEFINED_DB;
                }
                for (itr_wx = ext_x2 + 1; itr_wx < e_WORLD_WIDTH; itr_wx++)
                {
                    movepath_cost_map->moves2[itr_wy * e_WORLD_WIDTH + itr_wx] = e_ST_UNDEFINED_DB;
                }
            }
        }
        else
        {
            for (itr_wy = ext_y1; itr_wy <= ext_y2; itr_wy++)
            {
                for (itr_wx = ext_x2 + 1; itr_wx < ext_x1; itr_wx++)
                {
                    movepath_cost_map->moves2[itr_wy * e_WORLD_WIDTH + itr_wx] = e_ST_UNDEFINED_DB;
                }
            }
        }

        Move_Path_Find(src_wx, src_wy, movepath_cost_map);

        _DI_path_length = 0;
        dst_world_map_idx = dst_wy * e_WORLD_WIDTH + dst_wx;
        while (movepath_cost_map->Reach_From[dst_world_map_idx] != dst_world_map_idx)
        {
            movepath_cost_map->Reverse_Path[_DI_path_length] = dst_world_map_idx;
            dst_world_map_idx = movepath_cost_map->Reach_From[dst_world_map_idx];
            _DI_path_length++;
        }

        for (_SI_itr = 0; _SI_itr < _DI_path_length; _SI_itr++)
        {
            int16_t map_idx = movepath_cost_map->Reverse_Path[_DI_path_length - 1 - _SI_itr];
            itr_wx = map_idx % e_WORLD_WIDTH;
            itr_wy = map_idx / e_WORLD_WIDTH;
            mvpth_x[_SI_itr] = itr_wx;
            mvpth_y[_SI_itr] = itr_wy;
            mvpth_c[_SI_itr] = movepath_cost_map->moves2[itr_wy * e_WORLD_WIDTH + itr_wx];
        }

        if (_DI_path_length == 0)
        {
            UU_flag = e_ST_FALSE;
            goto loc_D5B50;
        }
    }
    else
    {
        Move_Path_Find(src_wx, src_wy, movepath_cost_map);

        _DI_path_length = 0;
        dst_world_map_idx = dst_wy * e_WORLD_WIDTH + dst_wx;
        while (movepath_cost_map->Reach_From[dst_world_map_idx] != dst_world_map_idx)
        {
            movepath_cost_map->Reverse_Path[_DI_path_length] = dst_world_map_idx;
            dst_world_map_idx = movepath_cost_map->Reach_From[dst_world_map_idx];
            _DI_path_length++;
        }

        for (_SI_itr = 0; _SI_itr < _DI_path_length; _SI_itr++)
        {
            int16_t map_idx = movepath_cost_map->Reverse_Path[_DI_path_length - 1 - _SI_itr];
            itr_wx = map_idx % e_WORLD_WIDTH;
            itr_wy = map_idx / e_WORLD_WIDTH;
            mvpth_x[_SI_itr] = itr_wx;
            mvpth_y[_SI_itr] = itr_wy;
            mvpth_c[_SI_itr] = movepath_cost_map->moves2[itr_wy * e_WORLD_WIDTH + itr_wx];
        }
    }

    return _DI_path_length;
}
#endif


// WZD o148p04
// TILE_ExtendRange()

// WZD o148p05
// sub_D601B()

// WZD o148p06
// drake178: STK_SetOvlMoveMap()
/*
; calculates the movement cost (in half MPs) map for
; the selected plane from the passed movement type
; matrix and stores it into Temp_MoveMap_EMS@
*/
/*

*/
void Init_MovePathMap(int16_t MvMd_0, int16_t MvMd_1, int16_t MvMd_2, int16_t MvMd_3, int16_t MvMd_4, int16_t MvMd_5, int16_t wp)
{
    int8_t * movemap_ptr = 0;
    int8_t * terrain_flags_ptr = 0;
    int16_t map_squares = 0;
    int16_t var_6 = 0;
    int16_t road_bits = 0;
    int16_t itr_squares = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;

    EMMDATAH_Map();

    if(MvMd_5 == ST_TRUE) { goto Flying; }
    if(MvMd_4 == ST_TRUE) { goto Sailing; }
    if((MvMd_1 == ST_TRUE) && (MvMd_2 == ST_TRUE)) { goto Yay_Pathfinding; } else { goto Nay_Pathfinding; }


Flying:
{

        Set_Memory(&movepath_cost_map->moves2[0], 10080, 2);  // drake178: BUG: we only use 2400 (960h) bytes of this!
        terrain_flags_ptr = (int8_t *)&_map_square_flags[wp * WORLD_SIZE];
        movemap_ptr = (int8_t *)&movepath_cost_map->moves2;
        map_squares = WORLD_SIZE;
        road_bits = (MSF_ROAD | MSF_EROAD);
        for(itr_squares = 0; itr_squares < map_squares; itr_squares++)
        {
            if((terrain_flags_ptr[itr_squares] & road_bits) != 0)
            {
                movemap_ptr[itr_squares] = 0;
            }
        }

    goto Done;

}


Sailing:
{

    memcpy(&movepath_cost_map->moves2[0], &movement_mode_cost_maps[wp].sailing.moves2[0], WORLD_SIZE);

    goto Done;

}


Yay_Pathfinding:
{

    if(MvMd_3 == ST_TRUE)
    {
        for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
        {
            for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
            {
                movepath_cost_map->moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;
            }
        }
    }
    else
    {
        for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
        {
            for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
            {
                if(movement_mode_cost_maps[wp].walking.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] == 0)
                {
                    movepath_cost_map->moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
                }
                else if(movement_mode_cost_maps[wp].walking.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] == (int8_t)INF)  // CLAUDE == -1
                {
                    movepath_cost_map->moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = (int8_t)INF;  // CLAUDE == -1
                }
                else
                {
                    movepath_cost_map->moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;
                }

            }
        }
    }

    goto Done;

}


Nay_Pathfinding:
{

    if(MvMd_1 == ST_TRUE)
    {
        memcpy(&movepath_cost_map->moves2[0], &movement_mode_cost_maps[wp].forester.moves2[0], WORLD_SIZE);
        if(MvMd_3 == ST_TRUE)
        {
            Copy_Memory_Less_Than(&movepath_cost_map->moves2[0], &movement_mode_cost_maps[wp].swimming.moves2[0], WORLD_SIZE);
        }
        if(MvMd_2 == ST_TRUE)
        {
            Copy_Memory_Less_Than(&movepath_cost_map->moves2[0], &movement_mode_cost_maps[wp].mountaineer.moves2[0], WORLD_SIZE);
        }
    }
    else if(MvMd_2 == ST_TRUE)
    {
        memcpy(&movepath_cost_map->moves2[0], &movement_mode_cost_maps[wp].mountaineer.moves2[0], WORLD_SIZE);
        if(MvMd_3 == ST_TRUE)
        {
            Copy_Memory_Less_Than(&movepath_cost_map->moves2[0], &movement_mode_cost_maps[wp].swimming.moves2[0], WORLD_SIZE);
        }
        if(MvMd_2 == ST_TRUE)
        {
            Copy_Memory_Less_Than(&movepath_cost_map->moves2[0], &movement_mode_cost_maps[wp].mountaineer.moves2[0], WORLD_SIZE);
        }

    }
    else if(MvMd_3 == ST_TRUE)
    {
        memcpy(&movepath_cost_map->moves2[0], &movement_mode_cost_maps[wp].swimming.moves2[0], WORLD_SIZE);
    }
    else
    {
        memcpy(&movepath_cost_map->moves2[0], &movement_mode_cost_maps[wp].walking.moves2[0], WORLD_SIZE);
    }

    goto Done;

}


Done:

    return;
}


// WZD o148p07
/* GEMINI */
/**
 * Clears the long-range path for a unit on the overland map.
 * This is done by setting the first word (likely the status or length) 
 * of the corresponding MovePath structure to -1.
 * 
 * The function ensures the path index is within the valid range (1 to 139).
 * Note: Index 0 is ignored/treated as invalid.
 */
/*

Loaded_Game_Update()
    / *
        BEGIN:  AI CONT / MOVE
    * /
    for(itr = 0; itr < 100; itr++)
    {
        _ai_move_path_table[itr] = -1;  // DEDU - INF / ST_UNDEFINED?
    }
    _ai_move_path_idx = ST_UNDEFINED;
    ...

*/
void Invalidate_AI_Move_Path(void)
{
    if(_ai_move_path_idx > 0 && _ai_move_path_idx < 140)
    {
        _ai_move_path_table[_ai_move_path_idx].src_wx = -1;  // INF / ST_UNDEFINED?
    }
}

// WZD o148p08
/*
OON XREF:
    Stack_Move_To()
...handles all AI / CP / NPC movement on the overland map, including pathfinding and move execution, and calls this function to store the calculated long-range path for a unit.
*/
void Cache_AI_Move_Path(int16_t player_idx, int16_t src_wx, int16_t src_wy, int16_t dst_wx, int16_t dst_wy, int16_t plane, int8_t * wx_array, int8_t * wy_array, int8_t * cost_array)
{
    int16_t duplicate = 0;
    int16_t dst_idx = 0;
    int16_t i = 0;
    int16_t src_idx = 0;
    struct s_AI_MOVE_PATH * path_ptr = NULL;

    /* Only process for AI players */
    if(player_idx == HUMAN_PLAYER_IDX)
    {
        return;
    }

    src_idx = -1;
    dst_idx = -1;
    
    /* Find the source and destination indices within the provided path arrays */
    for (i = 0; i < 35; i++)
    {
        if(wx_array[i] == src_wx && wy_array[i] == src_wy)
        {
            src_idx = i;
        }

        if(wx_array[i] == dst_wx && wy_array[i] == dst_wy)
        {
            dst_idx = i;
        }

        /* Optimization: break if both found */
        if(src_idx != -1 && dst_idx != -1)
        {
            break;
        }
    }

    /* Valid path must contain both points and move forward in the array */
    if(src_idx == -1 || dst_idx == -1 || dst_idx <= src_idx)
    {
        return;
    }

    /* Find an empty slot in the move path table */
    i = 0;
    while((i < 140) && (_ai_move_path_idx == -1))
    {
        if(_ai_move_path_table[i].src_wx == -1)
        {
            _ai_move_path_idx = i;
        }
        i++;
    }

    /* If no empty slot is found, return */
    if(_ai_move_path_idx == -1)
    {
        return;
    }

    /* Check if this specific path is already stored in the table */
    /* OGBUG: ignores the plane ... `_ai_move_path_table[i]->wp == plane` */
    duplicate = ST_FALSE;
    for (i = 0; i < 140; i++)
    {
        if(_ai_move_path_table[i].src_wx == src_wx &&
            _ai_move_path_table[i].srx_wy == src_wy &&
            _ai_move_path_table[i].dst_wx == dst_wx &&
            _ai_move_path_table[i].dst_wy == dst_wy)
        {
            duplicate = ST_TRUE;
            break;
        }
    }

    if(duplicate == ST_TRUE)
    {
        return;
    }

    /* Record the new path into the allocated slot */
    path_ptr = &_ai_move_path_table[_ai_move_path_idx];

    path_ptr->src_wx = src_wx;
    path_ptr->srx_wy = src_wy;
    path_ptr->dst_wx = dst_wx;
    path_ptr->dst_wy = dst_wy;
    path_ptr->wp = plane;
    path_ptr->length = dst_idx - src_idx;

    /* Copy the step coordinates and costs (skipping the source square itself) */
    memcpy(path_ptr->wx_array,   &wx_array[src_idx + 1],   path_ptr->length);
    memcpy(path_ptr->wy_array,   &wy_array[src_idx + 1],   path_ptr->length);
    memcpy(path_ptr->cost_array, &cost_array[src_idx + 1], path_ptr->length);

}
