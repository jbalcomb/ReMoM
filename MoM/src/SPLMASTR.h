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

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:CA10                                                 BEGIN: ovr136 - Uninitialized Data

// WZD dseg:CA10
// extern int16_t combat_max_extra_casting_cost;

// WZD dseg:CA12
extern int16_t g_spell_scratch_int;
#define g_total_casting_cost g_spell_scratch_int


// WZD dseg:CA14
extern int16_t _osc_anim_ctr;

// WZD dseg:CA16
/*
¿ _osc_spell_idx ? NOT _temp_sint?

XREF:
    Learn_Spell_Animation()
    Cast_Spell_Overland()
    SBK_SliderRedraw()
    SBK_SpellSlider()
    Combat_Spellbook_Mana_Adder_Draw()
    Combat_Spellbook_Mana_Adder_Screen()
    Spell_Target_Global_Enchantment_Screen_Draw()
    OVL_DrawGlobalAnim()
    WIZ_GlobalSpellAnim()
    Spell_Target_Wizard_Screen_Draw()
    Spell_Target_Wizard_Screen()

*/
extern int16_t g_active_spell_idx;

// WZD dseg:CA18
extern int16_t _xtra_mana_pos;

// WZD dseg:CA1A 00 00 00 00 00 00 00 00 00 00                   word_434BA dw 5 dup(0)                  ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+308w ...

// WZD dseg:CA24
extern SAMB_ptr som_twinkle_seg;

// WZD dseg:CA26
extern SAMB_ptr g_spell_screen_backdrop_seg;
#define m_extra_mana_background_seg     g_spell_screen_backdrop_seg
#define m_global_anim_mirror_pane_seg   g_spell_screen_backdrop_seg
#define m_wizard_lab_backdrop_seg       g_spell_screen_backdrop_seg

// WZD dseg:CA28
extern SAMB_ptr GAME_MP_SpellVar_2;
extern int16_t _mana_adder_caster_idx;  // DNE in Dasm

// WZD dseg:CA2A 00 00                                           IMG_OVL_TrgtWizCncl@ dw 0               ; DATA XREF: IDK_SplScr_sBFAA5+50w ...

// WZD dseg:CA2C
/*

*/
extern int16_t _temp_sint_4;
#define _osc_scanned_field _temp_sint_4
#define _osc_summon_unit_type _temp_sint_4

// WZD dseg:CA2E 00 00                                           IDK_SUMMONBK_pict_seg dw 0              ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+17Dw ...

// WZD dseg:CA30
extern SAMB_ptr xtramana_ok_button_seg;

// WZD dseg:CA32 00 00                                           IMG_OVL_TargetWizBG@ dw 0               ; DATA XREF: IDK_SplScr_sBFAA5+39w ...

// WZD dseg:CA34
extern SAMB_ptr spl_anim_compose_seg;

// WZD dseg:CA36
extern SAMB_ptr g_gui_scratch_bitmap;

// WZD dseg:CA36                                                 END: ovr136 - Uninitialized Data



// WZD dseg:CA56                                                 BEGIN: ovr138 - Uninitialized Data

// WZD dseg:CA56
// drake178: m_magic_winner_idx
/*
; HoF will skip turn count points if this is not -1 or
; the index of the human player, and SoM if it is not
; the human player
*/
extern int16_t m_magic_winner_idx;

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
// static void Spell_Of_Mastery_Lose_Load(int16_t wizard_id);

// WZD o138p02
// static void Spell_Of_Mastery_Lose_Draw(void);

// WZD o138p03
void Spell_Of_Mastery_Lose(void);

// WZD o138p04
// static void Cast_Spell_Of_Mastery_Draw(void);

// WZD o138p05
void Cast_Spell_Of_Mastery(int16_t player_idx);

// WZD o138p06
// static void Spell_Of_Mastery_Load(void);

// WZD o138p07
// static void Spell_Of_Mastery_Draw(void);

// WZD o138p08
void Spell_Of_Mastery(int16_t player_idx);

// WZD o138p09
void Undead_Animation_Load(int16_t unit_type);

// WZD o138p10
void Undead_Animation_Draw(void);

// WZD o138p11
void Undead_Animation(int16_t unit_type);



#ifdef __cplusplus
}
#endif

#endif  /* SPLMASTR_H */
