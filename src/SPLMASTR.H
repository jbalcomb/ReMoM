/*
    WIZARDS.EXE
        ovr136
        ovr137
        ovr138

MoO2
    ¿ DNE // N/A ?

*/

#ifndef SPLMASTR_H
#define SPLMASTR_H

#include "MOM.H"

#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:CA56                                                 BEGIN: ovr138 - Uninitialized Data

// WZD dseg:CA56
// drake178: GAME_SoM_Cast_By
/*
; HoF will skip turn count points if this is not -1 or
; the index of the human player, and SoM if it is not
; the human player
*/
extern int16_t GAME_SoM_Cast_By;

// WZD dseg:CA58 00 00                                           spellose_wizard_sphere_seg dw 0                         ; DATA XREF: IDK_SomScr_Lose_Load+B1w ...
// WZD dseg:CA5A 00 00                                           spellose_sphere_seg dw 0                         ; DATA XREF: IDK_SomScr_Lose_Load+9Dw ...

// WZD dseg:CA5A                                                 END: ovr138 - Uninitialized Data




/*
    WIZARDS.EXE  ovr136
*/

// WZD o136p01
// Select_Hero_To_Ressurect()

// WZD o136p02
// SBK_LoadSpellSlider()

// WZD o136p03
// SBK_SliderRedraw()

// WZD o136p04
// SBK_SpellSlider()

// WZD o136p05
// CMB_LoadSpellSlider()

// WZD o136p06
// CMB_SliderRedraw()

// WZD o136p07
int16_t CMB_SpellSlider__STUB(int16_t spell_idx, int16_t Selected_Spell, int16_t caster_idx);

// WZD o136p08
// sub_BD8A8()

// WZD o136p09
// sub_BDBC5()

// WZD o136p10
// IDK_SplBk_OvlEnch_sBDEA0()

// WZD o136p11
// sub_BE356()

// WZD o136p12
// sub_BE656()



/*
    WIZARDS.EXE  ovr138
*/


// WZD o138p01
void Spell_Of_Mastery_Lose_Load(int16_t wizard_id);

// WZD o138p02
void Spell_Of_Mastery_Lose_Draw(void);

// WZD o138p03
void Spell_Of_Mastery_Lose(void);

// WZD o138p04
void SoM_Started_Draw(void);

// WZD o138p05
void SoM_Started(int16_t player_idx);

// WZD o138p06
void Spell_Of_Mastery_Load(void);

// WZD o138p07
void Spell_Of_Mastery_Draw(void);

// WZD o138p08
void Spell_Of_Mastery(int16_t player_idx);

// WZD o138p09
// CMB_LoadUndeadAnim()

// WZD o138p10
// CMB_DrawUndeadAnim()

// WZD o138p11
// CMB_CreateUndeadAnim()



#ifdef __cplusplus
}
#endif

#endif  /* SPLMASTR_H */
