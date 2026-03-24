#ifndef EXPLORE_H
#define EXPLORE_H

/*
    WIZARDS.EXE
    ovr092

*/

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


// WZD o92p01
void Clear_Square_Scouted_Flags(int16_t wp);

// WZD o92p02
int16_t Check_Square_Scouted(int16_t wx, int16_t wy, int16_t wp);

// WZD o92p03
void Set_Square_Scouted(int16_t wx, int16_t wy, int16_t wp);

// WZD o92p04
void Clear_Square_Scouted(int16_t wx, int16_t wy, int16_t wp);

// WZD o92p05
void Update_Scouted_And_Contacted(void);

// WZD o92p06
void Set_Square_Scouted_Flags(int16_t wx, int16_t wy, int16_t wp, int16_t scout_range);

// WZD o92p07
void Contact_Other_Player(int16_t unit_idx, int16_t wp);

// WZD o92p08
int16_t Check_Square_Explored(int16_t wx, int16_t wy, int16_t world_plane);


#ifdef __cplusplus
}
#endif


#endif  /* EXPLORE_H */
