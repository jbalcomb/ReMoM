
#include "MoM.H"

/*
UnitMove.C

WIZARDS.EXE
ovr071

Movement Points

Movement Types

? Planar Travel ?


Elsewhere, ...
    ? Unit vs. Stack ?
    UNIT_
    STAK_

*/



/*
    WIZARDS.EXE  ovr071
*/

// WZD o71p01
// drake178: OVL_GetMoveTypes()
/*
    assumes _unit_stack_count
    wrapper for STK_GetMoveTypes()
*/
// void Active_Stack_Movement_Modes(struct s_Movement_Modes * movement_mode_flags)
void Active_Stack_Movement_Modes(int16_t movement_mode_flags[])
{
    int16_t stack[9];
    int16_t stack_has_no_active_units;
    int16_t unit_count;
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

    unit_count = 0;

    for(itr = 0; itr < _unit_stack_count; itr++)
    {
        if(_unit_stack[itr].active == ST_TRUE)
        {
            stack[unit_count] = _unit_stack[itr].unit_idx;
            unit_count++;
        }
    }

    Stack_Movement_Modes(movement_mode_flags, &stack[0], unit_count);

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
// void STK_GetMoveTypes(int16_t * movement_mode_flags, int_16_t stack, int16_t unit_count)
// void Stack_Movement_Modes(struct s_Movement_Modes * movement_mode_flags, int16_t * stack_array, int16_t stack_size)
void Stack_Movement_Modes(int16_t movement_mode_flags[], int16_t * stack_array, int16_t stack_size)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Stack_Movement_Modes()\n", __FILE__, __LINE__);
#endif





#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Stack_Movement_Modes()\n", __FILE__, __LINE__);
#endif
}


// WZD o71p03
int16_t UNIT_HasAirTravel(int16_t unit_idx)
{
    int16_t unit_type;
    int16_t has_air_travel;
    int16_t tmp_unit_enchantments_loword;
    int16_t tmp_unit_enchantments_hiword;

    has_air_travel = ST_FALSE;

    unit_type = _UNITS[unit_idx].type;

    if((_unit_type_table[unit_type].Move_Flags & M_Flying) != 0)
    {
        has_air_travel = ST_TRUE;
    }

    tmp_unit_enchantments_loword = _UNITS[unit_idx].Enchants_LO;
    tmp_unit_enchantments_hiword = _UNITS[unit_idx].Enchants_HI;

    if(
        ((tmp_unit_enchantments_hiword & UE_WINDWALKING) != 0) || 
        ((tmp_unit_enchantments_hiword & UE_FLIGHT) != 0)
    )
    {
        has_air_travel = ST_TRUE;
    }
    
    if( (_UNITS[unit_idx].Mutations & CC_Flight) != 0)
    {
        has_air_travel = ST_TRUE;
    }

    return has_air_travel;
}

// WZD o71p04
int16_t UNIT_HasWindWalking(int16_t unit_idx)
{
    int16_t has_wind_walking;
    int16_t tmp_unit_enchantments_loword;
    int16_t tmp_unit_enchantments_hiword;

    has_wind_walking = ST_FALSE;

    // TODO(JimBalcomb,20230618): figure out why DASM is using DX:AX ? DWORD/long ? ? macro ?
    tmp_unit_enchantments_loword = _UNITS[unit_idx].Enchants_LO;  // // ; enum UE_FLAGS_L
    tmp_unit_enchantments_hiword = _UNITS[unit_idx].Enchants_HI;  // // ; enum UE_FLAGS_H

    if(
        ((tmp_unit_enchantments_hiword & UE_WINDWALKING) == ST_TRUE) ||
        ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_WINDWALKING) == ST_TRUE)
    )
    {
        has_wind_walking = ST_TRUE;
    }

    return has_wind_walking;
}


// WZD o71p05
int16_t UNIT_HasWaterTravel(int16_t unit_idx)
{
    int16_t unit_type;
    int16_t has_water_travel;
    int16_t tmp_unit_enchantments_loword;
    int16_t tmp_unit_enchantments_hiword;

    has_water_travel = ST_FALSE;

    unit_type = _UNITS[unit_idx].type;

    if((_unit_type_table[unit_type].Move_Flags & M_Sailing) != 0)
    {
        has_water_travel = ST_TRUE;
    }
    
    if((_unit_type_table[unit_type].Move_Flags & M_Swimming) != 0)
    {
        has_water_travel = ST_TRUE;
    }

    tmp_unit_enchantments_loword = _UNITS[unit_idx].Enchants_LO;
    tmp_unit_enchantments_hiword = _UNITS[unit_idx].Enchants_HI;

    if(
        ((tmp_unit_enchantments_loword & UE_WATERWALKING) != 0)
    )
    {
        has_water_travel = ST_TRUE;
    }
    
    return has_water_travel;
}


// WZD o71p06
// int16_t UNIT_IsSailing(int16_t unit_idx);
// WZD o71p07
// int16_t UNIT_IsSwimming(int16_t unit_idx);
// WZD o71p08
// int16_t UNIT_ReturnZero(int16_t unit_idx);

// WZD o71p09
int16_t UNIT_HasWaterTravelItem(int16_t unit_idx)
{
    uint32_t UU_item_enchantments;
    int16_t has_water_travel_item;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_HasWaterTravelItem(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
#endif


    if(_UNITS[unit_idx].Hero_Slot != -1)
    {
        UU_item_enchantments = UNIT_BU_ApplyItems(unit_idx, Active_Unit);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: UU_item_enchantments: 0x%08X\n", __FILE__, __LINE__, unit_idx, UU_item_enchantments);
#endif

    // // tmp_item_enchantments_loword = Active_Unit->BU_REC.Item_UEs_L;
    // // tmp_item_enchantments_hiword = Active_Unit->BU_REC.Item_UEs_H;
    // uint32_t items_enchantments = Active_Unit->BU_REC.Item_UEs;

    if(
        ( (Active_Unit->Item_UEs & UE_WINDWALKING) != 0) ||
        ( (Active_Unit->Item_UEs & UE_FLIGHT) != 0)
    )
    {
        has_water_travel_item = ST_TRUE;
    }
    }
    else
    {
        has_water_travel_item = ST_FALSE;
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: UNIT_HasWaterTravelItem(unit_idx = %d) { has_water_travel_item = %d }\n", __FILE__, __LINE__, unit_idx, has_water_travel_item);
#endif

    return has_water_travel_item;
}


// WZD o71p010
int16_t UNIT_HasAirTravelItem(int16_t unit_idx)
{
    uint32_t UU_item_enchantments;
    int16_t has_air_travel_item;

    // // int16_t tmp_item_enchantments_loword;
    // // int16_t tmp_item_enchantments_hiword;
    // uint32_t items_enchantments;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_HasAirTravelItem(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
#endif

    if(_UNITS[unit_idx].Hero_Slot != -1)
    {
        UU_item_enchantments = UNIT_BU_ApplyItems(unit_idx, Active_Unit);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: UU_item_enchantments: 0x%08X\n", __FILE__, __LINE__, unit_idx, UU_item_enchantments);
#endif

    // // tmp_item_enchantments_loword = Active_Unit->BU_REC.Item_UEs_L;
    // // tmp_item_enchantments_hiword = Active_Unit->BU_REC.Item_UEs_H;
    // uint32_t items_enchantments = Active_Unit->BU_REC.Item_UEs;

    if(
        ( (Active_Unit->Item_UEs & UE_WINDWALKING) != 0) ||
        ( (Active_Unit->Item_UEs & UE_FLIGHT) != 0)
    )
    {
        has_air_travel_item = ST_TRUE;
    }
    }
    else
    {
        has_air_travel_item = ST_FALSE;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: UNIT_HasAirTravelItem(unit_idx = %d) { has_air_travel_item = %d }\n", __FILE__, __LINE__, unit_idx, has_air_travel_item);
#endif

    return has_air_travel_item;
}

// WZD o71p011
int16_t UNIT_HasInvisibility(int16_t unit_idx)
{
    int16_t has_invisibility;
    // int32_t Unused_PowerFlags;
    int16_t tmp_unit_enchantments_loword;
    int16_t tmp_unit_enchantments_hiword;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_HasInvisibility(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
// #endif

    has_invisibility = ST_FALSE;

    // NOTE: In the Dasm, this looks if( || || ) {return 1} else {return 0}

    if(_UNITS[unit_idx].Hero_Slot != ST_FALSE)
    {
        // Unused_PowerFlags = j_UNIT_BU_ApplyItems(Active_Unit@);

        // les     bx, [Active_Unit@]              ; 8 LBX_Alloc_Space paragraphs (128 bytes) ; contains a single battle unit record (110 bytes)
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
//     dbg_prn("DEBUG: [%s, %d]: END: UNIT_HasInvisibility(unit_idx = %d)  { has_invisibility = %d }\n", __FILE__, __LINE__, unit_idx, has_invisibility);
// #endif

    return has_invisibility;
}

// WZD o71p012
// int16_t UNIT_HasEnduranceUE(int16_t unit_idx);

// WZD o71p013
int16_t Unit_Has_Planar_Travel_Item(int16_t unit_idx)
{
    int16_t has_planar_travel_item;
    uint32_t UU_item_enchantments;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Unit_Has_Planar_Travel_Item(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
#endif

    if(_UNITS[unit_idx].Hero_Slot != ST_UNDEFINED)
    {

        UU_item_enchantments = UNIT_BU_ApplyItems(unit_idx, Active_Unit);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: UU_item_enchantments: 0x%08X\n", __FILE__, __LINE__, unit_idx, UU_item_enchantments);
#endif

        if(
            ( (Active_Unit->Item_UEs & UE_PLANARTRAVEL) != 0)
        )
        {
            has_planar_travel_item = ST_TRUE;
        }
        else
        {
            has_planar_travel_item = ST_FALSE;
        }
        
    }
    else
    {
        has_planar_travel_item = ST_FALSE;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Unit_Has_Planar_Travel_Item(unit_idx = %d)  { has_planar_travel_item = %d }\n", __FILE__, __LINE__, unit_idx, has_planar_travel_item);
#endif

    return has_planar_travel_item;
}

// WZD o71p014
// int16_t UNIT_IsNonCorporeal(int16_t unit_idx);
