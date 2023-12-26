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
    
    if( (_UNITS[unit_idx].Mutations & CC_Flight) != 0)
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
    "Units", not "Stack" - takes unit_array_count, doesn't touch _unit_stack, _unit_stack_count
    ¿ Units_Move_Path() ?

    ¿ UU_PathingVar1:  debug counter for movement path cache hits ?
    ¿ UU_PathingVar2:  debug counter for movement path cache misses ?
    // WZD dseg:CA8A  UU_PathingVar1 dw 0
    // WZD dseg:CA8C  UU_PathingVar2 dw 0
    UU_DBG_MovePatchCache_Hits
    UU_DBG_MovePatchCache_Misses
    DEDUCE: ¿ the usage here, incrementing without initializing, is another clue that variables initialized to zero can/do get put in the Uninitialized Data Data Segment ?



    called from Move_Stack()

push    [bp+player_idx]
push    [bp+unit_array_count]           ; UCnt
push    [bp+Landlubber_Count]           ; LLCnt
push    [bp+movement_points]            ; UU2
mov     ax, 1
push    ax                              ; UU1
mov     ax, offset OVL_Path_Costs
push    ax                              ; RCs@
mov     ax, (offset IDK_MovePath_DestinationY+1)
push    ax                              ; RYs@
mov     ax, (offset IDK_MovePath_DestinationX+1)
push    ax                              ; RXs@
push    [bp+map_plane]                  ; Plane
push    [bp+destination_y]              ; TgtY
push    [bp+destination_x]              ; TgtX
push    [bp+unit_y]                     ; SrcY
push    [bp+unit_x]                     ; SrcX
push    [bp+movement_modes+0Ah]
push    [bp+movement_modes+8]
push    [bp+movement_modes+6]
push    [bp+movement_modes+4]
push    [bp+movement_modes+2]
push    [bp+movement_modes]             ; MTypes
call    j_STK_GetPath    

*/
int16_t STK_GetPath(int16_t MvMd_00, int16_t MvMd_02, int16_t MvMd_04, int16_t MvMd_06, int16_t MvMd_08, int16_t MvMd_0A, int16_t src_x, int16_t src_y, int16_t dst_x, int16_t dst_y, int16_t map_p, uint8_t * RXs, uint8_t * RYs, uint8_t RCs, int16_t UU_bFlag_1, int16_t UU_vFlag_2, int16_t boat_rider_count, int16_t units_count, int16_t player_idx)
{
// Btm_Y= word ptr -0Eh
// Rgt_X= word ptr -0Ch
// Top_Y= word ptr -0Ah
// Lft_X= word ptr -8
// Target_Tile_Index= word ptr -6
// Y_Index= word ptr -4
// X_Index= word ptr -2

    int16_t path_length;  // DNE, in Dasm

    assert(0 && "STK_GetPath() not yet implemented");

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: STK_GetPath()\n", __FILE__, __LINE__);
#endif


    // DONT  EMM_Map_DataH();  // ; maps the EMM Data block into the page frame

    UU_bFlag_1 = ST_TRUE;  // ; unused as argument (overwritten)
    UU_vFlag_2 = 8;        // ; unused as argument (overwritten)

    // DONT  CRP_UNIT_OverlandPath = ST_UNDEFINED;  // ; an index into OvlMovePaths_EMS@


    if(player_idx == HUMAN_PLAYER_IDX)
    {
        UU_bFlag_1 = ST_FALSE;


    }
    else  /* (player_idx != HUMAN_PLAYER_IDX) */
    {
        // DONT  "NOT human player - movement path cache"
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: STK_GetPath()\n", __FILE__, __LINE__);
#endif

    path_length = 0;
    return path_length;
}


// WZD o148p04
// TILE_ExtendRange()
// WZD o148p05
// sub_D601B()
// WZD o148p06
// STK_SetOvlMoveMap()
// WZD o148p07
// OVL_ClearUnitPath()
// WZD o148p08
// OVL_StoreLongPath()
