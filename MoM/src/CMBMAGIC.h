/*
    Combat

    WIZARDS.EXE
        ovr112  ¿ CMBMAGIC ?

*/

#ifndef CMBMAGIC_H
#define CMBMAGIC_H

#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_TYPE.h"



// WZD dseg:C896                                                 ¿ BEGIN:  ovr112 ?

// WZD dseg:C896
extern int16_t * _battlefield_leadership;
extern int16_t * _battlefield_resistall;
extern int16_t * _battlefield_holybonus;

// WZD dseg:C8A2
extern int16_t * CMB_IDK_4PR;
// WZD dseg:C8A6
extern int16_t _combat_spell_target_type;
// WZD dseg:C8A8
// int16_t * CMB_NearDispel_UCs;
// WZD dseg:C8AA
// int16_t * CMB_NearDispel_UEs;
// WZD dseg:C8AC
// uint16_t _combat_caster_idx;

// WZD dseg:C8AC                                                 ¿ END:  ovr112 ?



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr112
*/

// WZD o112p01
void Combat_Compose_Spellbook_Background(void);

// WZD o112p02
void Combat_Spellbook_Screen_Draw(void);

// WZD o112p03
int16_t Combat_Cast_Spell(int16_t caster_id, int16_t wx, int16_t wy, int16_t wp);

// WZD o112p04
int16_t Combat_Spellbook_Screen(int16_t caster_idx, int16_t * selected_spell);

// WZD o112p05
int16_t Do_Legal_Spell_Check__WIP(int16_t spell_idx);

// WZD o112p06
int16_t Spell_Resistance_Modifier(int16_t spell_idx);

// WZD o112p07
int16_t Combat_Casting_Cost_Multiplier(int16_t player_idx);

// WZD o112p08
int16_t AITP_EarthToMud(int16_t player_idx, int16_t * target_cgx, int16_t * target_cgy);

// WZD o112p09
int16_t AITP_Disrupt(int16_t player_idx, int16_t * target_cgx, int16_t * target_cgy);

// WZD o112p10
int16_t AITP_CracksCall(int16_t player_idx, int16_t * target_cgx, int16_t * target_cgy);

// WZD o112p11
// UU_AITP_WordofRecall()

// WZD o112p12
int16_t AITP_RecallHero(int16_t player_idx);



#ifdef __cplusplus
}
#endif

#endif  /* CMBMAGIC_H */
