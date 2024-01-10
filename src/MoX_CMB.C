/*
    WIZARDS.EXE
        ovr098
        ovr116
*/

#include "MoM.H"



/*
    WIZARDS.EXE  ovr098
*/


// WZD o98p01
// drake178: CMB_CE_Refresh()
// CMB_CE_Refresh()

// WZD o98p02
// drake178: BU_HasSpellAbility()
// BU_HasSpellAbility()

// WZD o98p03
// drake178: sub_7EC81()
// sub_7EC81()

// WZD o98p04
// drake178: AI_CMB_PlayTurn()
// AI_CMB_PlayTurn()

// WZD o98p05
// drake178: CMB_ProgressTurnFlow()
// CMB_ProgressTurnFlow()

// WZD o98p06
// drake178: WIZ_FleeCombat()
// WIZ_FleeCombat()


// WZD o98p07
// drake178: TILE_HasCityOfPlayer()
int16_t Player_City_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx)
{
    int16_t city_idx;  // _SI_
    int16_t itr_cities;  // _CX_

    city_idx = -1;

    for(itr_cities = 0; ((itr_cities < _cities) && (city_idx == -1)); itr_cities++)
    {
        if(
            (_CITIES[itr_cities].wp == wp) &&
            (_CITIES[itr_cities].owner_idx == player_idx) &&
            (_CITIES[itr_cities].wy == wy) &&
            (_CITIES[itr_cities].wx == wx)
        )
        {
            city_idx = itr_cities;
        }
    }

    return city_idx;
}


// WZD o98p08
// drake178: TILE_FleeTo()
// TILE_FleeTo()

// WZD o98p09
// drake178: TILE_MoveUnitTo()
// TILE_MoveUnitTo()

// WZD o98p10
// drake178: CMB_SetActionCursor()
// CMB_SetActionCursor()

// WZD o98p11
// drake178: BU_GetRangedCursor()
// BU_GetRangedCursor()

// WZD o98p12
// drake178: CMB_CreateCELabels()
// CMB_CreateCELabels()

// WZD o98p13
// drake178: BU_GetCmbtMoveType()
// BU_GetCmbtMoveType()

// WZD o98p14
// drake178: UNIT_SummonToBattle()
// UNIT_SummonToBattle()

// WZD o98p15
// drake178: BU_UnitLoadToBattle()
// BU_UnitLoadToBattle()

// WZD o98p16
// drake178: CMB_Units_Init()
// CMB_Units_Init()

// WZD o98p17
// drake178: BU_CheckFlight()
// BU_CheckFlight()

// WZD o98p18
// drake178: BU_IsFlying()
// BU_IsFlying()

// WZD o98p19
// drake178: BU_MeleeFlightCheck()
// BU_MeleeFlightCheck()

// WZD o98p20
// drake178: BU_PrepMoveSound()
// BU_PrepMoveSound()

// WZD o98p21
// drake178: G_CMB_Auto_OFF()
// G_CMB_Auto_OFF()

// WZD o98p22
// drake178: STK_ComposeFleeLost()
// STK_ComposeFleeLost()



/*
    WIZARDS.EXE  ovr116
*/

// WZD o116p01
// drake178: UNIT_BU_ApplyItems()
uint32_t UNIT_BU_ApplyItems(int16_t unit_idx, struct s_STRATEGIC_UNIT * strategic_unit)
{
    int16_t hero_slot_idx;
    int16_t unit_owner_idx;
    uint32_t Item_Flags;

    int16_t itr_item_count;
    int16_t item_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_BU_ApplyItems()\n", __FILE__, __LINE__);
#endif

    strategic_unit->Item_UEs = 0;
    strategic_unit->Melee_ATK_Flags = 0;
    strategic_unit->Ranged_ATK_Flags = 0;

    unit_owner_idx = _UNITS[unit_idx].owner_idx;

    hero_slot_idx = _UNITS[unit_idx].Hero_Slot;


    if(hero_slot_idx == ST_UNDEFINED)
    {
        goto Return_Zero;
    }
    else
    {
        Item_Flags = 0;

        for(itr_item_count = 0; itr_item_count < 3; itr_item_count++)
        {

            if(_players[unit_owner_idx].Heroes[hero_slot_idx].Items[itr_item_count] != ST_UNDEFINED)
            {
                item_idx = _players[unit_owner_idx].Heroes[hero_slot_idx].Items[itr_item_count];

                BU_Item_To_UEFlags(item_idx, strategic_unit);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: strategic_unit->Item_UEs: 0x%08X\n", __FILE__, __LINE__, strategic_unit->Item_UEs);
#endif



            }

        }

        goto Return_Flags;
    }

    goto Done;

Return_Zero:
    Item_Flags = 0;
    goto Done;

Return_Flags:
// les     bx, [bp+strategic_unit@]
// mov     ax, [word ptr bp+Return_Flags+2]
// mov     dx, [word ptr bp+Return_Flags]
// or      dx, [word ptr es:bx+BU_REC.Item_UEs]
// or      ax, [word ptr es:bx+(BU_REC.Item_UEs+2)]
// mov     [word ptr bp+Return_Flags+2], ax
// mov     [word ptr bp+Return_Flags], dx
// mov     dx, [word ptr bp+Return_Flags+2]
// mov     ax, [word ptr bp+Return_Flags]

Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_BU_ApplyItems()\n", __FILE__, __LINE__);
#endif

    return Item_Flags;
}

// WZD o116p02
// drake178: BU_Item_To_UEFlags()
void BU_Item_To_UEFlags(int16_t item_idx, struct s_STRATEGIC_UNIT * strategic_unit)
{
    uint32_t item_enchants;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: BU_Item_To_UEFlags()\n", __FILE__, __LINE__);
#endif

    // IP_Cloak_Of_Fear

    // IP_Wraith_Form

    // IP_Regeneration

    // IP_Pathfinding

    if((TBL_Items[item_idx].Powers & 0x01000000) != 0)  /* IP_Water_Walking */
    {
        item_enchants = item_enchants | 0x01000000;  /* UE_Water_Walking */
    }

    // IP_Resist_Elements

    // IP_Elemental_Armour

    // IP_Endurance

    if((TBL_Items[item_idx].Powers & 0x80000000) != 0)  /* IP_Invisibility */
    {
        item_enchants = item_enchants | 0x80000000;  /* UE_Flight */
    }

    if((TBL_Items[item_idx].Powers & 0x00000002) != 0)  /* IP_Flight */
    {
        item_enchants = item_enchants | 0x00000002;  /* UE_Flight */
    }

    // IP_Resist_Magic

    // IP_Guardian_Wind

    // IP_Magic_Immunity

    // IP_True_Sight

    // IP_Bless

    // IP_Lion_Heart

    if((TBL_Items[item_idx].Powers & 0x00001000) != 0)  /* IP_Planar_Travel */
    {
        item_enchants = item_enchants | 0x00001000;  /* UE_Planar_Travel */
    }

    // IP_Righteousness

    // IP_Invulnerability

    // IP_Holy_Avenger


// les     bx, [bp+strategic_unit@]
// mov     ax, [word ptr es:bx+(BU_REC.Item_UEs+2)]
// mov     dx, [word ptr es:bx+BU_REC.Item_UEs]
// or      dx, [word ptr bp+item_enchants]
// or      ax, [word ptr bp+item_enchants+2]
// les     bx, [bp+strategic_unit@]
// mov     [word ptr es:bx+(BU_REC.Item_UEs+2)], ax
// mov     [word ptr es:bx+BU_REC.Item_UEs], dx

    strategic_unit->Item_UEs = item_enchants;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: BU_Item_To_UEFlags()\n", __FILE__, __LINE__);
#endif

}


// WZD o116p03


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

    // if(_players[_UNITS[unit_idx].owner_idx].Globals.Charm_of_Life > 0)
    if(_players[_UNITS[unit_idx].owner_idx].Globals[CHARM_OF_LIFE] > 0)
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

// WZD o116p05

// WZD o116p06
void UNIT_Create_BURecord(int16_t unit_idx, struct s_STRATEGIC_UNIT * strategic_unit)
{
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_Create_BURecord(unit_idx = %d, strategic_unit = %p)\n", __FILE__, __LINE__, unit_idx, strategic_unit);
#endif

    memcpy(strategic_unit, &_unit_type_table[_UNITS[unit_idx].type].Melee, sizeof(struct s_UNIT_TYPE));

    strategic_unit->Combat_Effects = 0;
    strategic_unit->Melee_To_Hit = 0;
    strategic_unit->Ranged_To_Hit = 0;
    strategic_unit->tohit = 0;
    strategic_unit->To_Block = 0;
    strategic_unit->Weapon_Plus1 = 0;
    strategic_unit->Melee_ATK_Flags = 0;
    strategic_unit->Ranged_ATK_Flags = 0;
    strategic_unit->Item_UEs = 0;
    strategic_unit->Extra_Hits = 0;
    strategic_unit->unit_idx = unit_idx;
    strategic_unit->Web_HP = 0;
    strategic_unit->Gold_Melee = 0;
    strategic_unit->Gold_Ranged = 0;
    strategic_unit->Gold_Defense = 0;
    strategic_unit->Gold_Resist = 0;
    strategic_unit->Gold_Hits = 0;
    strategic_unit->Grey_Melee = 0;
    strategic_unit->Grey_Ranged = 0;
    strategic_unit->Grey_Defense = 0;
    strategic_unit->Grey_Resist = 0;
    strategic_unit->Fig_IMG_Index = -1;
    strategic_unit->Status = 0;
    strategic_unit->owner_idx = _UNITS[unit_idx].owner_idx;
    for(itr = 0; itr < 3; itr++)
    {
        // strategic_unit->Regular_Dmg[itr] = 0;
        *((&strategic_unit->Regular_Dmg) + itr) = 0;
    }
    strategic_unit->Unit_Enchants = 0;
    strategic_unit->Suppression = 0;
    strategic_unit->mana_max = 0;
    strategic_unit->Item_Charges = 0;
    strategic_unit->Target_BU = -1;
    strategic_unit->Poison_Strength = 0;
    // ; returns the unit's gold Upkeep Cost, 0 for Noble
    // ; Heroes, Torin, and undead units; 1/2 for AI units
    // ; on Impossible; and 3/4 for AI units on Hard
    strategic_unit->upkeep = Unit_Gold_Upkeep(unit_idx);
    if( (strategic_unit->Attack_Flags & 0x04 /* Att_Poison */) != 0)
    {
        strategic_unit->Poison_Strength = strategic_unit->Spec_Att_Attrib;
    }
    if( (strategic_unit->ranged_type & 0x68 /* SR_MultiGaze */) != 0)
    {
        strategic_unit->Spec_Att_Attrib = 0;
    }

    BU_SetBaseStats(strategic_unit);

    strategic_unit->mana = strategic_unit->mana_max;
    strategic_unit->Regular_Dmg = _UNITS[unit_idx].Damage;





#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: UNIT_Create_BURecord(unit_idx = %d, strategic_unit = %p)\n", __FILE__, __LINE__, unit_idx, strategic_unit);
#endif

}

// WZD o116p07
/*

    calls out for Level Stats, Hero Ability Stats, Item Stats, Enchantment Stats
    
    ...
    Chaos Surge
    ...

    ...
    movement_points
    Hits
*/
void BU_SetBaseStats(struct s_STRATEGIC_UNIT * strategic_unit)
{
    int16_t unit_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: BU_SetBaseStats(strategic_unit = %p)\n", __FILE__, __LINE__, strategic_unit);
#endif

    unit_idx = strategic_unit->unit_idx;

    // ...
    // ...
    // ...

    // TODO  UNIT_BU_LevelStats()

    // ...
    // ...
    // ...

    // if(_UNITS[unit_idx].Hero_Slot > -1)
    // TODO  UNIT_BU_HeroAbStats()
    // TODO  UNIT_BU_ApplyItems()

    // ...
    // ...
    // ...

    // Weapon Quality

    // ...
    // ...
    // ...

    // Chaos Surge

    // ...
    // ...
    // ...
    
    // ...
    // ...
    // ...
    
    // ...
    // ...
    // ...

    // TODO  BU_ApplyEnchants()    

    // ...
    // ...
    // ...
    

    strategic_unit->movement_points = UNIT_GetHalfMoves_WIP(unit_idx);
// mov     ax, _SI_unit_idx
// mov     cl, 5
// shl     ax, cl
// les     bx, [_UNITS]                    ; 7ECh LBX_Alloc_Space paragraphs
// add     bx, ax
// mov     ax, [es:bx+s_UNIT.Enchants_HI]
// mov     dx, [es:bx+s_UNIT.Enchants_LO]
// les     bx, [bp+fp_CombatUnit]
// or      dx, [word ptr es:bx+BU_REC.Item_UEs]
// or      ax, [word ptr es:bx+(BU_REC.Item_UEs+2)]
// mov     [bp+Enchants_HO], ax
// mov     [bp+Enchants_LO], dx
// 
// 
// les     bx, [bp+fp_CombatUnit]
// cmp     [es:bx+BU_REC.movement_points], 3 ; BUG: value not in halves, but if it was, it would
//                                         ; cause a display bug below
//                                         ; this entire check is completely redundant, as the
//                                         ; above function already incorporates it, while any
//                                         ; overland movement calculations will IGNORE this
//                                         ; value, causing a discrepancy between the displayed
//                                         ; and the actual movement allowance of the unit
//                                         ; it could also only ever trigger if an opposing Wind
//                                         ; Mastery has reduced the moves of a ship with Flight
// jge     short @@Done_Almost
// mov     ax, [bp+Enchants_HO]
// mov     dx, [bp+Enchants_LO]
// and     dx, 0
// and     ax, UE_Flight
// or      dx, ax
// jz      short @@Done_Almost
// les     bx, [bp+fp_CombatUnit]          ; BUG: could override Wind Mastery malus overland, but
//                                         ; only for display, causing a discrepancy
// mov     [es:bx+BU_REC.movement_points], 6


    strategic_unit->hits = UNIT_GetHitsPerFig(unit_idx);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: BU_SetBaseStats(strategic_unit = %p)\n", __FILE__, __LINE__, strategic_unit);
#endif

}


// WZD o116p08
// WZD o116p09
// WZD o116p10
// WZD o116p11
// WZD o116p12
