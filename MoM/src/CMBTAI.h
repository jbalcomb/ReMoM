/*
    Combat

    WIZARDS.EXE
        ovr114  MoO2  Module: CMBTAI

*/

#ifndef CMBTAI_H
#define CMBTAI_H

#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr114
*/

// WZD o114p01
void AI_SetBasicAttacks(int16_t player_idx);

// WZD o114p02
void AI_BU_ProcessAction(int16_t battle_unit_idx, int16_t rally_cgx, int16_t rally_cgy);

// WZD o114p03
void Sort_Battle_Units(int16_t * troop_list, int16_t troop_count);

// WZD o114p04
void AI_GetCombatRallyPt(int16_t battle_unit_idx, int16_t * cgx, int16_t * cgy);

// WZD o114p05
void Auto_Do_Combat_Turn(int16_t player_idx);

// WZD o114p06
int16_t AI_BU_AssignAction(int16_t battle_unit_idx, int16_t no_spells_flag);

// WZD o114p07
int16_t Choose_Target_And_Action(int16_t battle_unit_idx, int16_t * selected_action, int16_t has_ranged_attack);

// WZD o114p08
void Do_Auto_Unit_Turn(int16_t battle_unit_idx, int16_t dst_cgx, int16_t dst_cgy, int16_t target_battle_unit_idx, int16_t rally_cgx, int16_t rally_cgy);

// WZD o114p09
int16_t Auto_Move_Unit(int16_t battle_unit_idx, int16_t Dest_X, int16_t Dest_Y, int16_t target_battle_unit_idx, int16_t Max_X, int16_t Max_Y);



#ifdef __cplusplus
}
#endif

#endif  /* CMBTAI_H */
