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

#include "MoM.H"
#include "UnitMove.H"
#include "MainScr.H"
#include "MovePath.H"


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
    int16_t troops[9];
    int16_t stack_has_no_active_units;
    int16_t troop_count;
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Active_Stack_Movement_Modes()\n", __FILE__, __LINE__);
#endif

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Active_Stack_Movement_Modes()\n", __FILE__, __LINE__);
#endif
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
                    ((_UNITS[unit_idx].enchantments & UE_PATHFINDING) != 0) ||
                    ((item_enchantments & UE_PATHFINDING) != 0)
                )
            )
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if(
                (l_movement_modes_array[itr_modes] == MV_MOUNTAINEER) &&
                (
                    ((_UNITS[unit_idx].enchantments & UE_PATHFINDING) != 0) ||
                    ((item_enchantments & UE_PATHFINDING) != 0)
                )
            )
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if(
                (movement_mode_flags[itr_modes] == MV_FLYING) &&
                (
                    ((_UNITS[unit_idx].enchantments & UE_WINDWALKING) != 0) ||
                    ((_unit_type_table[unit_type].Abilities & UA_WINDWALKING) != 0)
                )
            )
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if(
                (movement_mode_flags[itr_modes] == MV_FLYING) &&
                ((_UNITS[unit_idx].enchantments & (UE_WINDWALKING | UE_FLIGHT)) != 0) &&
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
                    ((_UNITS[unit_idx].enchantments & UE_WRAITHFORM) != 0) ||
                    ((item_enchantments & UE_WRAITHFORM) != 0)
                )
                {
                    Units_With_Same--;
                }

                if(
                    (l_movement_modes_array[itr_modes] == MV_FLYING) &&  // BUGBUG:  drake178: this jump is supposed to skip all 4 of the following blocks, not just the first one (misplaced parentheses in the original code)
                    ((_unit_type_table[unit_type].Abilities & UA_NONCORPOREAL) != 0) ||
                    ((_UNITS[unit_idx].enchantments & UE_WRAITHFORM) != 0) ||
                    ((item_enchantments & UE_WRAITHFORM) != 0)
                )
                {
                    Swimming_Units--;
                }
            }

            if
            (
                (l_movement_modes_array[itr_modes] == MV_SWIMMING) &&
                (
                    ((_UNITS[unit_idx].enchantments & UE_WATERWALKING) != 0) ||
                    ((item_enchantments & UE_WATERWALKING) != 0)
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
                    ((_UNITS[unit_idx].enchantments & UE_WRAITHFORM) != 0) ||
                    ((item_enchantments & UE_WRAITHFORM) != 0)
                )
            )
            {
                Units_With_Same++;
                Swimming_Units++;  // BUGBUG:  drake178: natural swimmers with water walking now count as 2
            }

            if(
                (l_movement_modes_array[itr_modes] == MV_FLYING) &&
                (
                    ((_UNITS[unit_idx].enchantments & (UE_WINDWALKING | UE_FLIGHT)) != 0) ||
                    ((item_enchantments & (UE_WINDWALKING | UE_FLIGHT)) != 0) ||
                    ((_UNITS[unit_idx].mutations & CC_FLIGHT) != 0)
                )
            )
            {
                Units_With_Same++;
                Flying_Units++;

                if(
                    ((_UNITS[unit_idx].enchantments & UE_WATERWALKING) != 0) ||
                    ((item_enchantments & UE_WATERWALKING) != 0) ||
                    ((_unit_type_table[unit_type].Abilities & UA_NONCORPOREAL) != 0) ||
                    ((_UNITS[unit_idx].enchantments & UE_WRAITHFORM) != 0) ||
                    ((item_enchantments & UE_WRAITHFORM) != 0)
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
            ((_UNITS[unit_idx].enchantments & UE_WRAITHFORM) != 0)  // BUGBUG:  drake178: ignores the item power
            /* || ((item_enchantments & UE_WRAITHFORM) != 0) */
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
        ((_UNITS[unit_idx].enchantments & UE_WINDWALKING) != 0)
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
int16_t Unit_Has_WindWalking(int16_t unit_idx)
{
    int16_t has_windwalking;

    has_windwalking = ST_FALSE;

    if(
        ((_UNITS[unit_idx].enchantments & UE_WINDWALKING) == ST_TRUE)
        ||
        ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_WINDWALKING) == ST_TRUE)
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

    if((_UNITS[unit_idx].enchantments & UE_WATERWALKING) != 0)
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

    if((_UNITS[unit_idx].enchantments & UE_WATERWALKING) != 0)
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
    uint32_t UU_item_enchantments;
    int16_t has_watertravel_item;

    if(_UNITS[unit_idx].Hero_Slot != -1)
    {
        UU_item_enchantments = BU_Apply_Item_Powers(unit_idx, global_battle_unit);

        if(
            ( (global_battle_unit->item_enchantments & UE_WINDWALKING) != 0) ||
            ( (global_battle_unit->item_enchantments & UE_FLIGHT) != 0)
        )
        {
            has_watertravel_item = ST_TRUE;
        }

    }
    else
    {
        has_watertravel_item = ST_FALSE;
    }

    return has_watertravel_item;
}


// WZD o71p010
int16_t Unit_Has_AirTravel_Item(int16_t unit_idx)
{
    uint32_t UU_item_enchantments;
    int16_t has_airtravel_item = ST_FALSE;

    if(_UNITS[unit_idx].Hero_Slot != -1)
    {
        UU_item_enchantments = BU_Apply_Item_Powers(unit_idx, global_battle_unit);

        if(
            ( (global_battle_unit->item_enchantments & UE_WINDWALKING) != 0) ||
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
/*
¿ BUG ? effectively only counts for non-ghero units, as it does not check items

XREF:
    j_Unit_Has_Endurance()
        Road_Build_Screen()
        Move_Units()
        AI_UNIT_BuildRoad()
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
    uint32_t UU_item_enchantments;

    if(_UNITS[unit_idx].Hero_Slot != ST_UNDEFINED)
    {

        UU_item_enchantments = BU_Apply_Item_Powers(unit_idx, global_battle_unit);

        if(
            ( (global_battle_unit->item_enchantments & UE_PLANARTRAVEL) != 0)
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
// int16_t UNIT_IsNonCorporeal(int16_t unit_idx);



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
int16_t Make_Move_Path(int16_t MvMd_0, int16_t MvMd_1, int16_t MvMd_2, int16_t MvMd_3, int16_t MvMd_4, int16_t MvMd_5, int16_t src_wx, int16_t src_wy, int16_t dst_wx, int16_t dst_wy, int16_t wp, int8_t mvpth_x[], int8_t mvpth_y[], int8_t mvpth_c[], int16_t UU_flag, int16_t UU_moves2, int16_t boatrider_count, int16_t troop_count, int16_t player_idx)
{
    int16_t ext_y2;
    int16_t ext_x2;
    int16_t ext_y1;
    int16_t ext_x1;
    int16_t dst_world_map_idx;
    int16_t itr_wy;
    int16_t itr_wx;
    int16_t itr;  // _SI_
    int16_t path_length;  // _DI_
    int16_t * ptr_reached_from;  // DNE in Dasm
    int16_t reached_from;  // DNE in Dasm

    // DONT  EMM_Map_DataH();  // ; maps the EMM Data block into the page frame

    UU_flag = ST_TRUE;
    UU_moves2 = 8;

    // DONT  CRP_UNIT_OverlandPath = ST_UNDEFINED;  // ; an index into OvlMovePaths_EMS@

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

        Update_MovePathMap(&movepath_cost_map->moves2[0], boatrider_count, troop_count, wp, player_idx, dst_wx, dst_wy, src_wx, src_wy);

        if(movepath_cost_map->moves2[((dst_wy * WORLD_WIDTH) + dst_wx)] == -1)
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

    } while (UU_flag == ST_FALSE);


    if(UU_flag == ST_FALSE)
    {

        Move_Path_Find(src_wx, src_wy, movepath_cost_map);

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

        for(itr = 0; itr < path_length; itr++)
        {
            itr_wx = (movepath_cost_map->Reverse_Path[((path_length - 1) - itr)] % WORLD_WIDTH);
            itr_wy = (movepath_cost_map->Reverse_Path[((path_length - 1) - itr)] / WORLD_WIDTH);
            mvpth_x[itr] = itr_wx;
            mvpth_y[itr] = itr_wy;
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


// WZD o148p04
// TILE_ExtendRange()

// WZD o148p05
// sub_D601B()

// WZD o148p06
// drake178: STK_SetOvlMoveMap()
void Init_MovePathMap(int16_t MvMd_0, int16_t MvMd_1, int16_t MvMd_2, int16_t MvMd_3, int16_t MvMd_4, int16_t MvMd_5, int16_t wp)
{
    int8_t * movemap_ptr;
    int8_t * terrain_flags_ptr;
    int16_t map_squares;
    int16_t var_6;
    int16_t road_bits;
    int16_t itr_squares;
    int16_t itr_wy;
    int16_t itr_wx;  // _DI_

    // TODO  EMM_Map_DataH()

    if(MvMd_5 == ST_TRUE) { goto Flying; }
    if(MvMd_4 == ST_TRUE) { goto Sailing; }
    if((MvMd_1 == ST_TRUE) && (MvMd_2 == ST_TRUE)) { goto Yay_Pathfinding; } else { goto Nay_Pathfinding; }


Flying:
{

        Set_Memory(&movepath_cost_map->moves2[0], 10080, 2);  // drake178: BUG: we only use 2400 (960h) bytes of this!
        terrain_flags_ptr = (int8_t *)&_map_square_flags[wp * WORLD_SIZE];
        movemap_ptr = (int8_t *)&movepath_cost_map->moves2;
        map_squares = WORLD_SIZE;
        road_bits = (TF_Road | TF_Enc_Road);
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

    memcpy(&movepath_cost_map->moves2[0], &movement_mode_cost_maps->sailing.moves2[0], WORLD_SIZE);

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
                if(movement_mode_cost_maps->walking.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] == 0)
                {
                    movepath_cost_map->moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
                }
                else if(movement_mode_cost_maps->walking.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] == -1)
                {
                    movepath_cost_map->moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = -1;
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
        memcpy(&movepath_cost_map->moves2[0], &movement_mode_cost_maps->forester.moves2[0], WORLD_SIZE);
        if(MvMd_3 == ST_TRUE)
        {
            Copy_Memory_Less_Than(&movepath_cost_map->moves2[0], &movement_mode_cost_maps->swimming.moves2[0], WORLD_SIZE);
        }
        if(MvMd_2 == ST_TRUE)
        {
            Copy_Memory_Less_Than(&movepath_cost_map->moves2[0], &movement_mode_cost_maps->mountaineer.moves2[0], WORLD_SIZE);
        }
    }
    else if(MvMd_2 == ST_TRUE)
    {
        memcpy(&movepath_cost_map->moves2[0], &movement_mode_cost_maps->mountaineer.moves2[0], WORLD_SIZE);
        if(MvMd_3 == ST_TRUE)
        {
            Copy_Memory_Less_Than(&movepath_cost_map->moves2[0], &movement_mode_cost_maps->swimming.moves2[0], WORLD_SIZE);
        }
        if(MvMd_2 == ST_TRUE)
        {
            Copy_Memory_Less_Than(&movepath_cost_map->moves2[0], &movement_mode_cost_maps->mountaineer.moves2[0], WORLD_SIZE);
        }

    }
    else if(MvMd_3 == ST_TRUE)
    {
        memcpy(&movepath_cost_map->moves2[0], &movement_mode_cost_maps->swimming.moves2[0], WORLD_SIZE);
    }
    else
    {
        memcpy(&movepath_cost_map->moves2[0], &movement_mode_cost_maps->walking.moves2[0], WORLD_SIZE);
    }

    goto Done;

}


Done:

    return;
}


// WZD o148p07
// OVL_ClearUnitPath()

// WZD o148p08
// OVL_StoreLongPath()
