/*
    WIZARDS.EXE
        ovr137  ¿ ~ Spell Animation(s) ?
*/

#ifndef SPELLS137_H
#define SPELLS137_H

#include "MOX/MOX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr137
*/

// WZD o137p01
void IDK_SummonAnim_Load(int16_t unit_type, int16_t player_idx);

// WZD o137p02
void IDK_SummonAnim_Draw(void);

// WZD o137p03
void IDK_SummonAnim(int16_t unit_type, int16_t magic_realm_spell_idx, int16_t player_idx);

// WZD o137p04
// sub_BECD9()

// WZD o137p05
// IDK_City_sBEE75()

// WZD o137p06
void IDK_Spell_Cityscape_1(int16_t city_idx, int16_t spell_idx, int16_t player_idx);

// WZD o137p07
void IDK_Spell_Cityscape_1(int16_t city_idx, int16_t spell_idx, int16_t player_idx);

// WZD o137p08
void OVL_LoadGlobalAnim(int16_t spell_idx, int16_t player_idx);

// WZD o137p09
void OVL_DrawGlobalAnim(void);

// WZD o137p10
void WIZ_GlobalSpellAnim(int16_t player_idx, int16_t spell_idx);

// WZD o137p11
// IDK_SplScr_sBFAA5()

// WZD o137p12
// Spell_Target_Wizard_Screen_Draw()

// WZD o137p13
// Spell_Target_Wizard_Screen()



#ifdef __cplusplus
}
#endif


#endif  /* SPELLS137_H */
