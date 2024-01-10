/*
    WIZARDS.EXE
        ovr098
        ovr116
*/

#ifndef MOX_CMB_H
#define MOX_CMB_H



#ifdef __cplusplus
extern "C" {
#endif



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
int16_t Player_City_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

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
/*
    drake178:
    converts item powers into attack and enchantment flags,
      applying them to the battle unit along with any stat boosts from items
      with the flags being zeroed instead if no effects apply or the unit is not a hero
    returns the item unit enchantment flags
*/
uint32_t UNIT_BU_ApplyItems(int16_t unit_idx, struct s_STRATEGIC_UNIT * strategic_unit);

// WZD o116p02
// drake178: BU_Item_To_UEFlags()
/*
    ¿ "Spell-Specific Artifact Enchantments" ?

*/
void BU_Item_To_UEFlags(int16_t item_idx, struct s_STRATEGIC_UNIT * strategic_unit);

// WZD o116p03

// WZD o116p04
int16_t UNIT_GetHitsPerFig(int16_t unit_idx);

// WZD o116p05

// WZD o116p06
void UNIT_Create_BURecord(int16_t unit_idx, struct s_STRATEGIC_UNIT * BattleUnit);

// WZD o116p07
void BU_SetBaseStats(struct s_STRATEGIC_UNIT * BattleUnit);

// WZD o116p08
// WZD o116p09
// WZD o116p10
// WZD o116p11
// WZD o116p12



#ifdef __cplusplus
}
#endif


#endif  /* MOX_CMB_H */
