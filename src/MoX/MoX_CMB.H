/*
    WIZARDS.EXE
        ovr098
        ovr116
        ovr163
*/

#ifndef MOX_CMB_H
#define MOX_CMB_H





/*
    Battle Unit Macros

        Unit Special Abilities
        Unit Mutations
        Unit Enchantments
*/

#define BU_LUCKY()      ( ( battle_unit->Attribs_1 & USA_LUCKY     ) != 0 )
#define BU_CASTER_20()  ( ( battle_unit->Attribs_1 & USA_CASTER_20 ) != 0 )
#define BU_CASTER_40()  ( ( battle_unit->Attribs_1 & USA_CASTER_40 ) != 0 )



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
void Retreat_From_Combat(int16_t player_idx);

// WZD o98p07
// drake178: TILE_HasCityOfPlayer()
int16_t Player_City_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

// WZD o98p08
int16_t Process_Retreating_Units(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

// WZD o98p09
int16_t Unit_Try_To_Move(int16_t wx, int16_t wy, int16_t wp, int16_t unit_idx, int16_t troop_count);

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
void STK_ComposeFleeLost__STUB(int16_t troop_count, int16_t troop_list[]);



/*
    WIZARDS.EXE  ovr116
*/

// WZD o116p01
uint32_t BU_Apply_Item_Powers(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p02
void BU_Apply_Item_Enchantments(int16_t item_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p03
void BU_Apply_Item_Attack_Specials(uint16_t * attack_flags, int16_t item_idx);

// WZD o116p04
int16_t Unit_Hit_Points(int16_t unit_idx);

// WZD o116p05
int16_t Battle_Unit_Hit_Points(struct s_BATTLE_UNIT * battle_unit);

// WZD o116p06
void Load_Battle_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * BattleUnit);

// WZD o116p07
void BU_Init_Battle_Unit(struct s_BATTLE_UNIT * BattleUnit);

// WZD o116p08
/*
s_BATTLE_UNIT.Move_Flags
s_BATTLE_UNIT.Attribs_1
s_BATTLE_UNIT.Abilities

*/
void BU_Apply_Specials(struct s_BATTLE_UNIT * battle_unit, uint32_t battle_unit_enchantments, uint8_t unit_mutations);

// WZD o116p09
void BU_Apply_Level(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p10
void BU_Init_Hero_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p11
void BU_Apply_Battlefield_Effects__WIP(struct s_BATTLE_UNIT * battle_unit);

// WZD o116p12
// sub_9A43E()



/*
    WIZARDS.EXE  ovr163
*/

// WZD ovr163p01
// CMB_LoadTerrainGFX()

// WZD ovr163p02
// CMB_ComposeBackgrnd()

// WZD ovr163p03
// CMB_LoadWallRiseGFX()

// WZD ovr163p04
void CMB_BaseAllocs(void);

// WZD ovr163p05
// CMB_SetProjectiles()

// WZD ovr163p06
// EMM_LoadFigureGFX()



#ifdef __cplusplus
}
#endif


#endif  /* MOX_CMB_H */
