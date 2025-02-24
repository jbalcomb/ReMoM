#ifndef EXPLORE_H
#define EXPLORE_H

/*
    WIZARDS.EXE
    ovr092

*/

#include "MoX/MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


// WZD o92p01
// drake178: TILE_VisibilityClear
void Clear_Square_Scouted_Flags(int16_t world_p);

// WZD o92p02
// drake178: TILE_IsVisible
int16_t Check_Square_Scouted(int16_t world_x, int16_t world_y, int16_t world_p);

// WZD o92p03
// drake178: TILE_MarkVisible
void Set_Square_Scouted(int16_t world_x, int16_t world_y, int16_t world_p);

// WZD o92p04
// drake178: UU_TILE_ClearVisible
void UU_Clear_Square_Scouted(int16_t world_x, int16_t world_y, int16_t world_p);

// WZD o92p05
// drake178: TILE_VisibilityReset
void Update_Scouted_And_Contacted__WIP(void);

// WZD o92p06
// drake178: TILE_ApplySightRange
int16_t Set_Square_Scouted_Flags(int16_t world_x, int16_t world_y, int16_t world_p, int16_t scout_range);

// WZD o92p07
// drake178: UNIT_ContactHuman

// WZD o92p08
int16_t Check_Square_Explored(int16_t world_x, int16_t world_y, int16_t world_plane);


#ifdef __cplusplus
}
#endif


#endif  /* EXPLORE_H */
