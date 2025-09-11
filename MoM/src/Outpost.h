/*
    WIZARDS.EXE
        ovr077

    Moo2
        Module: ¿ ?

*/

#ifndef OUTPOST_H
#define OUTPOST_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE ovr077
*/

// WZD o077p01
// TILE_Settle()
int16_t Create_Outpost(int16_t outpost_wx, int16_t outpost_wy, int16_t outpost_wp, int16_t unit_race, int16_t unit_owner, int16_t unit_idx);

// WZD o077p02
void Outpost_Screen(int16_t flag);

// WZD o077p03
void Outpost_Screen_Draw(void);

// WZD o077p04
void Outpost_Screen_Load(void);

// WZD o077p05
void Outpost_Garrison_Picture_Coords(int16_t slot, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2);

// WZD o077p06
void Change_City_Name_Popup(int16_t city_idx, int16_t player_idx);

// WZD o077p07
void Change_City_Name_Popup_Draw(void);

// WZD o077p08
int16_t Map_Square_Survey(int16_t wx, int16_t wy, int16_t wp);

// WZD o077p09
void Cast_Awareness(int16_t player_idx);



#ifdef __cplusplus
}
#endif

#endif  /* OUTPOST_H */
