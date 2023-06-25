#ifndef EXPLORE_H
#define EXPLORE_H

/*
    WIZARDS.EXE
    ovr092

*/

#include "MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


// WZD o97p01
// TILE_VisibilityClear

// WZD o97p02
int16_t TILE_IsVisible(int16_t world_x, int16_t world_y, int16_t world_plane);


// WZD o97p08
int16_t TILE_IsExplored(int16_t world_x, int16_t world_y, int16_t world_plane);


#ifdef __cplusplus
}
#endif


#endif  /* EXPLORE_H */
