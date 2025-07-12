/*
    AI - Build / "Grand Vizier"

    WIZARDS.EXE
        ovr157
*/

#ifndef AIBUILD_H
#define AIBUILD_H

#include "MOX/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr157
*/

// WZD o157p01
void Player_All_Colony_Autobuild(int16_t player_idx);

// WZD o157p02
void Player_Colony_Autobuild_CP(int16_t player_idx, int16_t city_idx);

// WZD o157p03
void Player_Colony_Autobuild_NP(int16_t city_idx);

// WZD o157p04
void AI_Player_City_Buy_Production(int16_t player_idx, int16_t city_idx);

// WZD o157p05
// drake178: CTY_GrandVizier()
void Player_Colony_Autobuild_HP(int16_t city_idx);



#ifdef __cplusplus
}
#endif

#endif  /* AIBUILD_H */
