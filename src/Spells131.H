/*
    WIZARDS.EXE
        ovr131
*/

#ifndef SPELLS131_H
#define SPELLS131_H

#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif





/*
    WIZARDS.EXE  ovr131
*/

// WZD o131p01
// drake178: CMB_Disenchant()
// CMB_Disenchant()

// WZD o131p02
// drake178: TILE_DispelMagic()
void Cast_Dispel_Magic(int16_t cgx, int16_t cgy, int16_t caster_idx, int16_t Dispel_Str, int16_t  * D_Count);

// WZD o131p03
// drake178: CMB_ApplyCracksCall()
// CMB_ApplyCracksCall()

// WZD o131p04
// drake178: CMB_BattlefieldSpell()
void CMB_BattlefieldSpell__WIP(int16_t player_idx, int16_t spell_idx, int16_t Anims, int16_t caster_idx);

// WZD o131p05
// drake178: WIZ_FlameStrike()
// WIZ_FlameStrike()

// WZD o131p06
// drake178: WIZ_HolyWord()
// WIZ_HolyWord()

// WZD o131p07
// drake178: WIZ_DeathSpell()
// WIZ_DeathSpell()

// WZD o131p08
void Combat_Spell_Animation__WIP(int16_t cgx, int16_t cgy, int16_t spell_idx, int16_t player_idx, int16_t Anims, int16_t caster_idx);

// WZD o131p09
void Cast_Raise_Dead(int16_t player_idx, int16_t caster_idx, int16_t target_cgx, int16_t target_cgy);

// WZD o131p10
void Cast_Animate_Dead(int16_t player_idx, int16_t caster_idx);



#ifdef __cplusplus
}
#endif


#endif  /* SPELLS131_H */
