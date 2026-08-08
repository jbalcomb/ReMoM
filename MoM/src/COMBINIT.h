/*
    Combat Initialization

    WIZARDS.EXE
        ovr116  MoO2  COMBINIT
*/

#ifndef COMBINIT_H
#define COMBINIT_H



#include "../../MoX/src/MOX_TYPE.h"  /* int8_t, int16_t, uint16_t, uint32_t */

#include "Combat.h" /* struct s_BATTLE_UNIT */



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr116
*/

// WZD o116p01
uint32_t Battle_Unit_Item_Stats(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p02
void Item_Powers_To_Unit_Enchantments(int16_t item_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p03
void Item_Powers_To_Attack_Attributes(uint16_t * attack_flags, int16_t item_idx);

// WZD o116p04
int16_t Unit_Hit_Points(int16_t unit_idx);

// WZD o116p05
int16_t Battle_Unit_Hit_Points(struct s_BATTLE_UNIT * battle_unit);

// WZD o116p06
void Load_Battle_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p07
void Battle_Unit_Regular_Stats(struct s_BATTLE_UNIT * battle_unit);

// WZD o116p08
void Apply_Enchantment_And_Mutation_Effects(struct s_BATTLE_UNIT * battle_unit, uint32_t battle_unit_enchantments, uint8_t unit_mutations);

// WZD o116p09
void Battle_Unit_Level_Stats(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p10
void Battle_Unit_Hero_Skill_Stats(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p11
void Battle_Unit_Special_Stats(struct s_BATTLE_UNIT * battle_unit);

// WZD o116p12
int16_t NIU_Unit_Has_Spell_Enchantment(int16_t unit_idx, int16_t spell_idx);



#ifdef __cplusplus
}
#endif

#endif  /* COMBINIT_H */
