/*
    WIZARDS.EXE
        ovr093

MoO2
Module: ERIC
*/

#ifndef WZD_O093_H
#define WZD_O093_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr093
*/

// WZD 093p01
void WIZ_Conquer__WIP(int16_t city_owner_idx, int16_t player_idx, int16_t city_idx);

// WZD 093p02
void WIZ_Conquest__WIP(int16_t city_owner_idx, int16_t player_idx);

// WZD 093p03
void Conquest_Draw__WIP(void);

// WZD 093p04
void GAME_DrawConquerors__STUB(void);

// WZD 093p05
void GAME_Conqest_Scene2__STUB(void);

// WZD 093p06
void GAME_DrawZappedWiz__STUB(int16_t flag);

// WZD 093p07
void GAME_DrawZapping__STUB(void);

// WZD 093p08
void sub_79907__WIP(void);

// WZD 093p09
int16_t GAME_IsWon__STUB(void);

// WZD 093p10
void GAME_PlayVictoryAnim__STUB(int16_t player_idx);

// WZD 093p11
// GAME_Draw_WIN_Anim()

// WZD 093p12
void GAME_LimboFallAnim__STUB(int16_t player_idx);

// WZD 093p13
// GAME_DrawLimboFall()

// WZD 093p14
int16_t WIZ_Banishment__STUB(int16_t loser_idx, int16_t winner_idx);

// WZD 093p15
// GAME_ReturnDialog()

// WZD 093p16
// GAME_DrawReturnAnim()




#ifdef __cplusplus
}
#endif



#endif /* WZD_O093_H */
