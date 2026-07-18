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
void Conquest_Animation(int16_t city_owner_idx, int16_t player_idx);

// WZD 093p03
void Conquest_Animation_Draw(void);

// WZD 093p04
void Conquest_Animation_Draw_Conquerors_Approach(void);

// WZD 093p05
void Conquest_Animation_Draw_Setup_Zap_Scene(void);

// WZD 093p06
void Conquest_Animation_Draw_Zapped_Wizard(int16_t flag);

// WZD 093p07
void Conquest_Animation_Draw_Zap_Strike(void);

// WZD 093p08
void Conquest_Animation_Draw_End_Pose(void);

// WZD 093p09
int16_t CP_Is_Dead(void);

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
void Return_Animation(int16_t player_idx);

// WZD 093p16
void Return_Animation_Draw(void);



#ifdef __cplusplus
}
#endif



#endif /* WZD_O093_H */
