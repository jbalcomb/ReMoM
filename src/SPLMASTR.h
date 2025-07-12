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

#include "MOX/MOX_TYPE.h"



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
int16_t Select_Hero_To_Ressurect(int16_t hero_count, int16_t hero_list[]);

// WZD o136p02
void Spellbook_Mana_Adder_Load(void);

// WZD o136p03
void Spellbook_Mana_Adder_Draw(void);

// WZD o136p04
void Spellbook_Mana_Adder_Screen(int16_t spell_idx, int16_t spellbook_field_idx);

// WZD o136p05
void Combat_Spellbook_Mana_Adder_Load(void);

// WZD o136p06
void Combat_Spellbook_Mana_Adder_Draw(void);

// WZD o136p07
int16_t Combat_Spellbook_Mana_Adder_Screen(int16_t spell_idx, int16_t spellbook_field_idx, int16_t caster_idx);

// WZD o136p08
void Spell_Target_Global_Enchantment_Screen_Load(int16_t spell_idx);

// WZD o136p09
void Spell_Target_Global_Enchantment_Screen_Draw(void);

// WZD o136p10
int16_t Spell_Target_Global_Enchantment_Screen(int16_t spell_idx, int16_t player_idx);

// WZD o136p11
void Spell_Target_Global_Enchantment_Bind__WIP(int16_t field_idx, int16_t player_idx, int16_t target_spell_idx, int16_t target_player_idx);

// WZD o136p12
void Spell_Target_Global_Enchantment_Disjunct__WIP(int16_t field_idx, int16_t player_idx, int16_t target_spell_idx, int16_t target_player_idx);



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
void SoM_Started_Draw__STUB(void);

// WZD o138p05
void SoM_Started__STUB(int16_t player_idx);

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
