/*
    WIZARDS.EXE
        ovr137  ¿ ~ Spell Animation(s) ?
*/

#ifndef SPELLS137_H
#define SPELLS137_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:CA40
extern SAMB_ptr IMG_SBK_SliderBar;
#define m_itemmake_icon_window_right_arrow_seg IMG_SBK_SliderBar
extern SAMB_ptr IMG_SBK_SliderDot;
#define m_itemmake_icon_window_left_arrow_seg IMG_SBK_SliderDot

// WZD dseg:CA4C
extern int16_t _osc_leave_screen;

// WZD dseg:CA4E
extern int16_t _osc_need_target_flag;

// WZD dseg:CA50
/*
AKA  _osc_bldg_idx
AKA  _osc_city_idx
AKA  _osc_player_idx
AKA  _osc_spell_Idx
SmlBook XtraMana & CmbBook XtraMana
    spellbook field index; used to position the popup window
*/
// Spells137.c  extern int16_t _temp_sint_1;
// #define _osc_player_idx _temp_sint_1
// #define _osc_spell_idx _temp_sint_1
// #define _osc_city_idx _temp_sint_1
// #define _osc_bldg_idx _temp_sint_1

// WZD dseg:CA52
extern int16_t _ce_bldg_idx;



/*
    WIZARDS.EXE  ovr137
*/

// WZD o137p01
void Summon_Animation_Load(int16_t unit_type, int16_t player_idx);

// WZD o137p02
void Summon_Animation_Draw(void);

// WZD o137p03
void Summon_Animation(int16_t unit_type, int16_t rarity, int16_t player_idx);

// WZD o137p04
void Cast_Spell_City_Enchantment_Animation_Load(int16_t spell_idx,int16_t  player_idx);

// WZD o137p05
// IDK_City_sBEE75()
void Cast_Spell_City_Enchantment_Animation_Draw(void);

// WZD o137p06
void Cast_Spell_City_Enchantment_Animation_1__WIP(int16_t city_idx, int16_t spell_idx, int16_t player_idx);

// WZD o137p07
void Cast_Spell_City_Enchantment_Animation_2__WIP(int16_t city_idx, int16_t spell_idx, int16_t player_idx);

// WZD o137p08
void OVL_LoadGlobalAnim(int16_t spell_idx, int16_t player_idx);

// WZD o137p09
void OVL_DrawGlobalAnim(void);

// WZD o137p10
void WIZ_GlobalSpellAnim(int16_t player_idx, int16_t spell_idx);

// WZD o137p11
// static void Target_Wizard_Screen_Load(int16_t spell_idx);

// WZD o137p12
// static void Target_Wizard_Screen_Draw(void);

// WZD o137p13
int16_t Target_Wizard_Screen(int16_t spell_idx);



#ifdef __cplusplus
}
#endif


#endif  /* SPELLS137_H */
