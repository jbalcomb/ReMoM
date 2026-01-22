/*
    MAGIC.EXE
    ovr056
*/

#ifndef INITGAME_H
#define INITGAME_H

#include "../../MoX/src/MOX_TYPE.h"

#ifdef __cplusplus
extern "C" {
#endif



/*
    MAGIC.EXE
        ovr056
*/

// MGC o56p1
// WIZ_SetProfiles()

// MGC o56p2
void NEWG_FinalizeTables__WIP(void);

// MGC o56p3
void Init_Diplomatic_Relations(void);

// MGC o56p4
void Initialize_Items(void);

// MGC o56p5
void Init_Summoning_Circle_And_Spell_Of_Mastery(void);

// MGC o56p6
void Init_Magic_Personalities_Objectives(void);

// MGC o56p7
void AI_WIZ_StrategyReset__WIP(void);

// MGC o56p8
void Init_Players(void);

// MGC o56p9
void Init_Computer_Players_Wizard_Profile(void);

// MGC o56p10
void Init_Computer_Players_Spell_Library(void);

// MGC o56p11
void Initialize_Events(void);

// MGC o56p12
void Init_Heroes(void);

// MGC o56p13
void Initialize_Messages(void);

// MGC o56p14
// UU_ITEM_SetHeroSlots()

// MGC o56p15
// WIZ_ConsolidateBooks()
void Consolidate_Spell_Book_Realms(int16_t player_idx);

// MGC o56p16
int16_t Player_Nominal_Skill(int16_t player_idx);

// MGC o56p17
// RNG_WeightedPick16()



#ifdef __cplusplus
}
#endif

#endif  /* INITGAME_H */
