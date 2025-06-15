/*
    WIZARDS.EXE
        ovr133
*/

#ifndef SPELLS133_H
#define SPELLS133_H

#include "MOX/MOX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr133
*/

// WZD o133p01
void Apply_Warp_Creature(int16_t battle_unit_idx);

// WZD o133p02  WIZ_Wrack()

// WZD o133p03
void Apply_Call_Lightning(int16_t player_idx);

// WZD o133p04
void BU_LifeDrain__WIP(int16_t target_idx, int16_t damage_types[], int16_t caster_idx);

// WZD o133p05
void Wall_Rise(int16_t spell_idx, int16_t caster_idx);

// WZD o133p06
void CMB_CounterMessage__STUB(int16_t caster_idx, int16_t type, int16_t spell_idx, char * title);

// WZD o133p07
int16_t WIZ_DispelAttempt__STUB(int16_t dispel_strength, int16_t spell_cast, int16_t player_idx, int16_t magic_realm);

// WZD o133p08
void Animate_Cracks_Call(int16_t cgx, int16_t cgy, int16_t caster_idx);

// WZD o133p09
void TILE_BoltFromAbove__WIP(int16_t cgx, int16_t cgy, int16_t spell_idx, int16_t caster_idx);

// WZD o133p10
void TILE_CombatSpellAnim__WIP(int16_t cgx, int16_t cgy, int16_t Anim_Size, int16_t caster_idx, int16_t spell_idx);

// WZD o133p11  BU_Teleport()

// WZD o133p12  BU_TunnelTo()

// WZD o133p13
void BU_CombatSummon__SEGRAX(int16_t battle_unit_idx, int16_t cgx, int16_t cgy, int16_t spell_idx, int16_t player_idx);

// WZD o133p14
void Animate_Lightning_Bolt(int16_t cgx, int16_t cgy, int16_t caster_idx);

// WZD o133p15
void Magic_Vortex_Create(int16_t player_idx, int16_t cgx, int16_t cgy);

// WZD o133p16
void CMB_SetVortexCursor__SEGRAX(int Vortex_Index);

// WZD o133p17
void CMB_VortexPlayerMove__SEGRAX(int Vortex_Index);

// WZD o133p18
void CMB_ProcessVortices__SEGRAX(void);

// WZD o133p19
void CMB_VortexMovement__SEGRAX(int Vortex_Index, int Next_X, int Next_Y);

// WZD o133p20
void Cast_Call_Chaos__WIP(int16_t caster_idx, int16_t anims_on);

// WZD o133p21
void Apply_Call_Chaos__WIP(int16_t player_idx, int16_t effects[]);



#ifdef __cplusplus
}
#endif


#endif  /* SPELLS133_H */
