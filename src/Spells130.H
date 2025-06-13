/*
    WIZARDS.EXE
        ovr130
*/

#ifndef SPELLS130_H
#define SPELLS130_H

#include "MOX/MOX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr130
*/

// WZD o130p01
void Hero_Slot_Types(int16_t unit_type_idx, int16_t item_slots[]);

// WZD o130p02
//Cast_DeathWish()

// WZD o130p03
// drake178: sub_AF56D()

// WZD o130p04
//Cast_BlackWind()

// WZD o130p05
int16_t Apply_Drain_Power(int16_t player_idx);

// WZD o130p06
//sAFA06_Anim_EarthLore()

// WZD o130p07
void Cast_Spell_Binding(int16_t player_idx);

// WZD o130p08
//Cast_ChangeTerain()

// WZD o130p09
//Cast_RaiseVolcano()

// WZD o130p10
//Cast_Transmute()

// WZD o130p11
//Cast_Corruption()

// WZD o130p12
//Cast_WordOfRecall()

// WZD o130p13
//Cast_EnchantRoad()

// WZD o130p14
//Cast_SpellWard()

// WZD o130p15
void Cast_HolyArms(int16_t player_idx);

// WZD o130p16
void Apply_Spell_Consecration(int16_t city_idx);

// WZD o130p17
void Apply_Subversion(int16_t player_idx);

// WZD o130p18
void Cast_NaturesAwareness(int16_t player_idx);



#ifdef __cplusplus
}
#endif

#endif  /* SPELLS130_H */
