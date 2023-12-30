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
                item_enchantments = UNIT_BU_ApplyItems(unit_idx, global_strategic_unit);
                item_enchantments = global_strategic_unit->Item_UEs;
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
                    ((_UNITS[unit_idx].Enchants_LO & UE_PATHFINDING) != 0) ||
                    ((item_enchantments & UE_PATHFINDING) != 0)
                )
            )
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if(
                (l_movement_modes_array[itr_modes] == MV_MOUNTAINEER) &&
                (
                    ((_UNITS[unit_idx].Enchants_LO & UE_PATHFINDING) != 0) ||
                    ((item_enchantments & UE_PATHFINDING) != 0)
                )
            )
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if(
                (movement_mode_flags[itr_modes] == MV_FLYING) &&
                (
                    ((_UNITS[unit_idx].Enchants_HI & UE_WINDWALKING) != 0) ||
                    ((_unit_type_table[unit_type].Abilities & UA_WINDWALKING) != 0)
                )
            )
            {
                movement_mode_flags[itr_modes] = ST_TRUE;
            }

            if(
                (movement_mode_flags[itr_modes] == MV_FLYING) &&
                ((_UNITS[unit_idx].Enchants_HI & (UE_WINDWALKING | UE_FLIGHT)) != 0) &&
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
                    ((_UNITS[unit_idx].Enchants_LO & UE_WRAITHFORM) != 0) ||
                    ((item_enchantments & UE_WRAITHFORM) != 0)
                )
                {
                    Units_With_Same--;
                }

                if(
                    (l_movement_modes_array[itr_modes] == MV_FLYING) &&  // BUGBUG:  drake178: this jump is supposed to skip all 4 of the following blocks, not just the first one (misplaced parentheses in the original code)
                    ((_unit_type_table[unit_type].Abilities & UA_NONCORPOREAL) != 0) ||
                    ((_UNITS[unit_idx].Enchants_LO & UE_WRAITHFORM) != 0) ||
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
                    ((_UNITS[unit_idx].Enchants_LO & UE_WATERWALKING) != 0) ||
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
                    ((_UNITS[unit_idx].Enchants_LO & UE_WRAITHFORM) != 0) ||
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
                    ((_UNITS[unit_idx].Enchants_HI & (UE_WINDWALKING | UE_FLIGHT)) != 0) ||
                    ((item_enchantments & (UE_WINDWALKING | UE_FLIGHT)) != 0) ||
                    ((_UNITS[unit_idx].Mutations & CC_FLIGHT) != 0)
                )
            )
            {
                Units_With_Same++;
                Flying_Units++;

                if(
                    ((_UNITS[unit_idx].Enchants_LO & UE_WATERWALKING) != 0) ||
                    ((item_enchantments & UE_WATERWALKING) != 0) ||
                    ((_unit_type_table[unit_type].Abilities & UA_NONCORPOREAL) != 0) ||
                    ((_UNITS[unit_idx].Enchants_LO & UE_WRAITHFORM) != 0) ||
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
            ((_UNITS[unit_idx].Enchants_LO & UE_WRAITHFORM) != 0)  // BUGBUG:  drake178: ignores the item power
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
    int16_t tmp_unit_enchantments_loword;
    int16_t tmp_unit_enchantments_hiword;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Has_AirTravel(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
// #endif

    has_airtravel = ST_FALSE;

    unit_type = _UNITS[unit_idx].type;

    if((_unit_type_table[unit_type].Move_Flags & MV_FLYING) != 0)
    {
        has_airtravel = ST_TRUE;
    }

    tmp_unit_enchantments_loword = _UNITS[unit_idx].Enchants_LO;
    tmp_unit_enchantments_hiword = _UNITS[unit_idx].Enchants_HI;

    if(
        ((tmp_unit_enchantments_hiword & UE_WINDWALKING) != 0) || 
        ((tmp_unit_enchantments_hiword & UE_FLIGHT) != 0)
    )
    {
        has_airtravel = ST_TRUE;
    }
    
    if( (_UNITS[unit_idx].Mutations & CC_FLIGHT) != 0)
    {
        has_airtravel = ST_TRUE;
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Has_AirTravel(unit_idx = %d) { has_airtravel = %d }\n", __FILE__, __LINE__, unit_idx, has_airtravel);
// #endif

    return has_airtravel;
}

// WZD o71p04
int16_t Unit_Has_WindWalking(int16_t unit_idx)
{
    int16_t has_windwalking;
    int16_t tmp_unit_enchantments_loword;
    int16_t tmp_unit_enchantments_hiword;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Has_WindWalking(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
// #endif

    has_windwalking = ST_FALSE;

    // TODO(JimBalcomb,20230618): figure out why DASM is using DX:AX ? DWORD/long ? ? macro ?
    tmp_unit_enchantments_loword = _UNITS[unit_idx].Enchants_LO;  // // ; enum UE_FLAGS_L
    tmp_unit_enchantments_hiword = _UNITS[unit_idx].Enchants_HI;  // // ; enum UE_FLAGS_H

    if(
        ((tmp_unit_enchantments_hiword & UE_WINDWALKING) == ST_TRUE) ||
        ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_WINDWALKING) == ST_TRUE)
    )
    {
        has_windwalking = ST_TRUE;
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Unit_Has_WindWalking(unit_idx = %d) { has_windwalking = %d }\n", __FILE__, __LINE__, unit_idx, has_windwalking);
// #endif

    return has_windwalking;
}


// WZD o71p05
int16_t Unit_Has_WaterTravel(int16_t unit_idx)
{
    int16_t unit_type;
    int16_t has_watertravel;
    int16_t tmp_unit_enchantments_loword;
    int16_t tmp_unit_enchantments_hiword;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Has_WaterTravel(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
// #endif

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

    tmp_unit_enchantments_loword = _UNITS[unit_idx].Enchants_LO;
    tmp_unit_enchantments_hiword = _UNITS[unit_idx].Enchants_HI;

    if(
        ((tmp_unit_enchantments_loword & UE_WATERWALKING) != 0)
    )
    {
        has_watertravel = ST_TRUE;
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Unit_Has_WaterTravel(unit_idx = %d) { has_watertravel = %d }\n", __FILE__, __LINE__, unit_idx, has_watertravel);
// #endif

    return has_watertravel;
}


// WZD o71p06
// int16_t UNIT_IsSailing(int16_t unit_idx);

// WZD o71p07
int16_t Unit_Has_Swimming(int16_t unit_idx)
{
    int16_t has_swimming;
    int16_t unit_type;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Has_Swimming(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
// #endif

    has_swimming = ST_FALSE;

    unit_type = _UNITS[unit_idx].type;

    if( (_unit_type_table[unit_type].Move_Flags & 0x04 /* M_Swimming */) != 0)
    {
        has_swimming = ST_TRUE;
    }

    if( (_UNITS[unit_idx].Enchants_LO & 0x0100 /* UE_Water_Walking */) != 0)
    {
        has_swimming = ST_TRUE;
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Unit_Has_Swimming(unit_idx = %d) { has_swimming = %d }\n", __FILE__, __LINE__, unit_idx, has_swimming);
// #endif

    return has_swimming;
}

// WZD o71p08
// int16_t UNIT_ReturnZero(int16_t unit_idx);

// WZD o71p09
int16_t Unit_Has_WaterTravel_Item(int16_t unit_idx)
{
    uint32_t UU_item_enchantments;
    int16_t has_watertravel_item;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Has_WaterTravel_Item(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
// #endif

    if(_UNITS[unit_idx].Hero_Slot != -1)
    {
        UU_item_enchantments = UNIT_BU_ApplyItems(unit_idx, global_strategic_unit);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: UU_item_enchantments: 0x%08X\n", __FILE__, __LINE__, unit_idx, UU_item_enchantments);
// #endif

    // // tmp_item_enchantments_loword = global_strategic_unit->BU_REC.Item_UEs_L;
    // // tmp_item_enchantments_hiword = global_strategic_unit->BU_REC.Item_UEs_H;
    // uint32_t items_enchantments = global_strategic_unit->BU_REC.Item_UEs;

    if(
        ( (global_strategic_unit->Item_UEs & UE_WINDWALKING) != 0) ||
        ( (global_strategic_unit->Item_UEs & UE_FLIGHT) != 0)
    )
    {
        has_watertravel_item = ST_TRUE;
    }
    }
    else
    {
        has_watertravel_item = ST_FALSE;
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Unit_Has_WaterTravel_Item(unit_idx = %d) { has_watertravel_item = %d }\n", __FILE__, __LINE__, unit_idx, has_watertravel_item);
// #endif

    return has_watertravel_item;
}


// WZD o71p010
int16_t Unit_Has_AirTravel_Item(int16_t unit_idx)
{
    uint32_t UU_item_enchantments;
    int16_t has_airtravel_item;

    // // int16_t tmp_item_enchantments_loword;
    // // int16_t tmp_item_enchantments_hiword;
    // uint32_t items_enchantments;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Has_AirTravel_Item(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
// #endif

    if(_UNITS[unit_idx].Hero_Slot != -1)
    {
        UU_item_enchantments = UNIT_BU_ApplyItems(unit_idx, global_strategic_unit);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: UU_item_enchantments: 0x%08X\n", __FILE__, __LINE__, unit_idx, UU_item_enchantments);
// #endif

    // // tmp_item_enchantments_loword = global_strategic_unit->BU_REC.Item_UEs_L;
    // // tmp_item_enchantments_hiword = global_strategic_unit->BU_REC.Item_UEs_H;
    // uint32_t items_enchantments = global_strategic_unit->BU_REC.Item_UEs;

    if(
        ( (global_strategic_unit->Item_UEs & UE_WINDWALKING) != 0) ||
        ( (global_strategic_unit->Item_UEs & UE_FLIGHT) != 0)
    )
    {
        has_airtravel_item = ST_TRUE;
    }
    }
    else
    {
        has_airtravel_item = ST_FALSE;
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Unit_Has_AirTravel_Item(unit_idx = %d) { has_airtravel_item = %d }\n", __FILE__, __LINE__, unit_idx, has_airtravel_item);
// #endif

    return has_airtravel_item;
}

// WZD o71p011
int16_t Unit_Has_Invisibility(int16_t unit_idx)
{
    int16_t has_invisibility;
    // int32_t Unused_PowerFlags;
    int16_t tmp_unit_enchantments_loword;
    int16_t tmp_unit_enchantments_hiword;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Has_Invisibility(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
// #endif

    has_invisibility = ST_FALSE;

    // NOTE: In the Dasm, this looks if( || || ) {return 1} else {return 0}

    if(_UNITS[unit_idx].Hero_Slot != ST_FALSE)
    {
        // Unused_PowerFlags = j_UNIT_BU_ApplyItems(global_strategic_unit@);

        // les     bx, [global_strategic_unit@]              ; 8 LBX_Alloc_Space paragraphs (128 bytes) ; contains a single battle unit record (110 bytes)
        // mov     ax, [es:bx+BU_REC.Item_UEs_H]
        // mov     dx, [es:bx+BU_REC.Item_UEs_L]
        // and     dx, UE_Invisibility
        // and     ax, 0
        // or      dx, ax
    }

    // TODO(JimBalcomb,20230618): figure out why DASM is using DX:AX ? DWORD/long ? ? macro ?
    tmp_unit_enchantments_loword = _UNITS[unit_idx].Enchants_LO;  // // ; enum UE_FLAGS_L
    tmp_unit_enchantments_hiword = _UNITS[unit_idx].Enchants_HI;  // // ; enum UE_FLAGS_H

    if( (tmp_unit_enchantments_hiword & UE_INVISIBILITY) != 0 )
    {
        // DELETE  DLOG("( (tmp_unit_enchantments_hiword & 0x8000) != 0 )");
        has_invisibility = ST_TRUE;
    }
    else
    {
        // DELETE  DLOG("( (tmp_unit_enchantments_hiword & 0x8000) == 0 )");
    }
    if( (_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_INVISIBILITY) != 0 )
    {
        // DELETE  DLOG("( (_unit_type_table[_UNITS[unit_idx].type].Abilities & 0x40) != 0 )");
        has_invisibility = ST_TRUE;
    }
    else
    {
        // DELETE  DLOG("( (_unit_type_table[_UNITS[unit_idx].type].Abilities & 0x40) == 0 )");
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Unit_Has_Invisibility(unit_idx = %d)  { has_invisibility = %d }\n", __FILE__, __LINE__, unit_idx, has_invisibility);
// #endif

    return has_invisibility;
}

// WZD o71p012
// int16_t UNIT_HasEnduranceUE(int16_t unit_idx);

// WZD o71p013
int16_t Unit_Has_PlanarTravel_Item(int16_t unit_idx)
{
    int16_t has_planartravel_item;
    uint32_t UU_item_enchantments;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Has_PlanarTravel_Item(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
// #endif

    if(_UNITS[unit_idx].Hero_Slot != ST_UNDEFINED)
    {

        UU_item_enchantments = UNIT_BU_ApplyItems(unit_idx, global_strategic_unit);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: UU_item_enchantments: 0x%08X\n", __FILE__, __LINE__, unit_idx, UU_item_enchantments);
// #endif

        if(
            ( (global_strategic_unit->Item_UEs & UE_PLANARTRAVEL) != 0)
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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Unit_Has_PlanarTravel_Item(unit_idx = %d)  { has_planartravel_item = %d }\n", __FILE__, __LINE__, unit_idx, has_planartravel_item);
// #endif

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
int16_t STK_GetPath__WIP(int16_t MvMd_0, int16_t MvMd_1, int16_t MvMd_2, int16_t MvMd_3, int16_t MvMd_4, int16_t MvMd_5, int16_t src_wx, int16_t src_wy, int16_t dst_wx, int16_t dst_wy, int16_t wp, int8_t mvpth_x[], int8_t mvpth_y[], int8_t mvpth_c[], int16_t UU_flag, int16_t UU_moves2, int16_t boatrider_count, int16_t troop_count, int16_t player_idx)
{
// Btm_Y= word ptr -0Eh
// Rgt_X= word ptr -0Ch
// Top_Y= word ptr -0Ah
// Lft_X= word ptr -8
// Target_Tile_Index= word ptr -6
// Y_Index= word ptr -4
// X_Index= word ptr -2

    int16_t path_length;  // DNE, in Dasm

    // DONT  EMM_Map_DataH();  // ; maps the EMM Data block into the page frame

    UU_flag = ST_TRUE;
    UU_moves2 = 8;

    // DONT  CRP_UNIT_OverlandPath = ST_UNDEFINED;  // ; an index into OvlMovePaths_EMS@

    if(player_idx == HUMAN_PLAYER_IDX)
    {
        goto Calc_Move_Path;
    }
    else
    {
        goto Move_Path_Cache;
    }

Move_Path_Cache:
{

    goto Done;
}

Calc_Move_Path:
{
    
    UU_flag = ST_FALSE;

    Init_MovePathMap(MvMd_0, MvMd_1, MvMd_2, MvMd_3, MvMd_4, MvMd_5, wp);

    // HERE: UU_moves2 has been hard-coded to 8, so ALWAYS jumps over
    if((player_idx == HUMAN_PLAYER_IDX) && (UU_moves2 == 1))
    {
        // TODO  min 2 costs
    }

    Update_MovePathMap(&movepath_cost_map->moves2[0], boatrider_count, troop_count, wp, player_idx, dst_wx, dst_wy, src_wx, src_wy);



    goto Done;
}


Done:

    path_length = 0;
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
        terrain_flags_ptr = (int8_t *)TBL_Terrain_Flags[wp * WORLD_SIZE];
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
