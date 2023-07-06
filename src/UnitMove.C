
#include "MoX_DEF.H"
#include "MoX_TYPE.H"

#include "MoX_Data.H"

#include "UnitType.H"

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

// WZD o71p01 OVL_GetMoveTypes   ();
// WZD o71p02 STK_GetMoveTypes   ();

// WZD o71p03
// int16_t UNIT_IsFlying(int16_t unit_idx);

// WZD o71p04
// AKA UNIT_IsWindWalker()
// "HasWindWalking", because manual says that is the name of the "Movement Type"
int16_t UNIT_HasWindWalking(int16_t unit_idx)
{
    int16_t has_wind_walking;
    int16_t tmp_unit_enchantments_loword;
    int16_t tmp_unit_enchantments_hiword;

    has_wind_walking = ST_FALSE;

    // TODO(JimBalcomb,20230618): figure out why DASM is using DX:AX ? DWORD/long ? ? macro ?
    tmp_unit_enchantments_loword = _UNITS[unit_idx].Enchants_LO;  // // ; enum UE_FLAGS_L
    tmp_unit_enchantments_hiword = _UNITS[unit_idx].Enchants_HI;  // // ; enum UE_FLAGS_H
    // UE_Wind_Walking 0x1  Ab_WindWalking 0x1000
    if( ((tmp_unit_enchantments_hiword & 0x1) == ST_TRUE) || ((_unit_type_table[_UNITS[unit_idx].type].Abilities & 0x1000) == ST_TRUE) )
    {
        has_wind_walking = ST_TRUE;
    }

    return has_wind_walking;
}

// WZD o71p05
// int16_t UNIT_IsOceanFaring(int16_t unit_idx);
// WZD o71p06
// int16_t UNIT_IsSailing(int16_t unit_idx);
// WZD o71p07
// int16_t UNIT_IsSwimming(int16_t unit_idx);
// WZD o71p08
// int16_t UNIT_ReturnZero(int16_t unit_idx);
// WZD o71p09
// int16_t UNIT_HasItemSwim(int16_t unit_idx);
// WZD o71p010
// int16_t UNIT_HasItemFlight(int16_t unit_idx);

// WZD o71p011
int16_t UNIT_HasInvisibility(int16_t unit_idx)
{
    int16_t has_invisibility;
    // int32_t Unused_PowerFlags;
    int16_t tmp_unit_enchantments_loword;
    int16_t tmp_unit_enchantments_hiword;

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
    // UE_Invisibility 0x8000  Ab_Invisibility 0x40
    if( ((tmp_unit_enchantments_hiword & 0x8000) != 0) || ((_unit_type_table[_UNITS[unit_idx].type].Abilities & 0x40) != 0) )
    {
        has_invisibility = ST_TRUE;
    }


    return has_invisibility;
}

// WZD o71p012
// int16_t UNIT_HasEnduranceUE(int16_t unit_idx);
// WZD o71p013
// int16_t UNIT_HasPlanarItem (int16_t unit_idx);
// WZD o71p014
// int16_t UNIT_IsNonCorporeal(int16_t unit_idx);
