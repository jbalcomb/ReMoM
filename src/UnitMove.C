
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
// WZD o71p03 UNIT_IsFlying      ();

// WZD o71p04
// AKA UNIT_IsWindWalker()
// "HasWindWalking", because manual says that is the name of the "Movement Type"
int16_t UNIT_HasWindWalking(int16_t unit_idx)
{
    int16_t tmp_unit_type;
    int16_t has_wind_walking;
    int16_t tmp_unit_enchantments_loword;
    int16_t tmp_unit_enchantments_hiword;

    tmp_unit_type = _UNITS[unit_idx].type;

    has_wind_walking = ST_FALSE;

    // TODO(JimBalcomb,20230618): figure out why DASM is using DX:AX ? DWORD/long ? ? macro ?
    tmp_unit_enchantments_loword = _UNITS[unit_idx].Enchants_LO;  // // ; enum UE_FLAGS_L
    tmp_unit_enchantments_hiword = _UNITS[unit_idx].Enchants_HI;  // // ; enum UE_FLAGS_H
    // UE_Wind_Walking 0x1  Ab_WindWalking 0x1000
    if( ((tmp_unit_enchantments_hiword & 0x1) == ST_TRUE) || ((_unit_type_table[tmp_unit_type].Abilities & 0x1000) == ST_TRUE) )
    {
        has_wind_walking = ST_TRUE;
    }

    return has_wind_walking;
}
// WZD o71p05 UNIT_IsOceanFaring ();
// WZD o71p06 UNIT_IsSailing     ();
// WZD o71p07 UNIT_IsSwimming    ();
// WZD o71p08 UNIT_ReturnZero    ();
// WZD o71p09 UNIT_HasItemSwim   ();
// WZD o71p010 UNIT_HasItemFlight ();
// WZD o71p011 UNIT_IsInvisible   ();
// WZD o71p012 UNIT_HasEnduranceUE();
// WZD o71p013 UNIT_HasPlanarItem ();
// WZD o71p014 UNIT_IsNonCorporeal();
