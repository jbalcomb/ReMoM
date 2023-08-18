/*
    WIZARDS.EXE
    ovr116
*/

#include "MoX.H"



// WZD o116p04
int16_t UNIT_GetHitsPerFig(int16_t unit_idx)
{
    int8_t Unit_Level;
    int16_t Charm_of_Life_Bonus;
    int16_t hit_points;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_GetHitsPerFig(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
#endif

    Unit_Level = _UNITS[unit_idx].Level;

    // TODO  if _UNITS[unit_idx].Exchants & 0x100  (000100000000b)  (256d)  
    // looks like AX:DX 32-bit value bit-field  DD  Long
    /* UE_Heroism  (0x100) */
    if( (_UNITS[unit_idx].Enchants_HI & 0x100) != 0)
    {
        if(Unit_Level < 3)
        {
            Unit_Level = 3;
        }
    }

    hit_points = _unit_type_table[_UNITS[unit_idx].type].Hits;

    /* UE_Black_Channels  (0x10) */
    if( (_UNITS[unit_idx].Enchants_LO & 0x10) != 0)
    {
        hit_points += 1;
    }

    /* UE_Lionheart  (0x400) */
    if( (_UNITS[unit_idx].Enchants_HI & 0x400) != 0)
    {
        hit_points += 3;
    }

    /*
        BEGIN: Unit is Hero-Unit
    */
    if(_UNITS[unit_idx].Hero_Slot > -1)
    {
        if(Unit_Level > 0)
        {
            hit_points += 1;
        }

// mov     ax, [bp+Unit_Index]
// mov     cl, 5
// shl     ax, cl
// les     bx, [_UNITS]                    ; 7ECh LBX_Alloc_Space paragraphs
// add     bx, ax
// mov     al, [es:bx+s_UNIT.owner_idx]
// cbw
// mov     cl, 2
// shl     ax, cl
// mov     bx, ax
// les     bx, [p0_heroes+bx]              ; 1Ch LBX_Alloc_Sp
        // TODO(JimBalcomb,20230814): triple-check this logic of *indexing* p_heroes by (owner_idx * 4)  Array of Pointers!?! p_heroes[6];  p_heroes[player/owner_idx]

        /* Ab_Constitution  (0x1000)*/
        // p0_heroes[_UNITS[unit_idx].type].
        if( (p_heroes[_UNITS[unit_idx].owner_idx]->Abilities_LO & 0x1000) != 0 )
        {
            hit_points += Unit_Level + 1;
        }

        /* Ab_Constitution2  (0x2000) */
        if( (p_heroes[_UNITS[unit_idx].owner_idx]->Abilities_LO & 0x2000) != 0 )
        {
            // TODO double check the calculation in the Dasm, it has the `CWD; SUB AX,DX` thing
            hit_points += (((Unit_Level + 1) * 3) / 2);
        }

        if(Unit_Level > 1)
        {
            hit_points += 1;
        }
        if(Unit_Level > 3)
        {
            hit_points += 1;
        }

    }
    /*
        END: Unit is Hero-Unit
    */

    if(Unit_Level > 2)
    {
        hit_points += 1;
    }

    if(Unit_Level > 4)
    {
        hit_points += 1;
    }
    if(Unit_Level > 5)
    {
        hit_points += 1;
    }
    if(Unit_Level > 6)
    {
        hit_points += 1;
    }
    if(Unit_Level > 7)
    {
        hit_points += 1;
    }

    if(_players[_UNITS[unit_idx].owner_idx].Globals.Charm_of_Life > 0)
    {
        Charm_of_Life_Bonus = hit_points / 4;
        if(Charm_of_Life_Bonus < 1)
        {
            Charm_of_Life_Bonus = 1;
        }
        hit_points += Charm_of_Life_Bonus;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: UNIT_GetHitsPerFig(unit_idx = %d)  { hit_points = %d }\n", __FILE__, __LINE__, unit_idx, hit_points);
#endif

    return hit_points;

}
