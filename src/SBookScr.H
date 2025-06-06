/*
    Spellbook Screen

    WIZARDS.EXE
        ovr134
    
    SPELLSCR.LBX
*/

#ifndef SBOOKSCR_H
#define SBOOKSCR_H

#include "MOM.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr134
*/

// WZD o134p01
// PRIVATE  void Spell_Screen_Load(void);

// WZD o134p02
void Spellbook_Screen_Draw(void);

// WZD o134p03
void Spellbook_Screen(void);

// WZD o134p04
int16_t WIZ_SetOverlandSpell__WIP(int16_t player_idx, int16_t spell_idx, int16_t spellbook_page_spell_index);

// WZD o134p05
// sub_B9837()

// WZD o134p06
void Full_Draw_Main_Screen(void);

// WZD o134p07
void OVL_MosaicFlip__STUB(void);

// WZD o134p08
void Fizzle_Notification(int16_t player_idx, int16_t counter_player_idx, int16_t spell_idx, char * counter_spell_name);

// WZD o134p09
// sub_B9D14()
void Cast_Spell_City_Enchantment_Animation_Load_Music(int16_t spell_idx, int16_t good_bad);

// WZD o134p10
int16_t Spell_Animation_Load_Graphics__WIP(int16_t spell_idx);

// WZD o134p11
void Spell_Animation_Load_Sound_Effect__WIP(void);

// WZD o134p12
void Spell_Animation_Screen__WIP(int16_t wx, int16_t wy, int16_t wp);



#ifdef __cplusplus
}
#endif


#endif  /* SBOOKSCR_H */
