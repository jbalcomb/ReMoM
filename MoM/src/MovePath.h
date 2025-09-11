/*
    WIZARDS.EXE
        ovr147
*/
#ifndef MOVEPATH_H
#define MOVEPATH_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif




// drake178: HMP_COSTS
// sizeof:  960h  2400d
#pragma pack(push)
#pragma pack(2)
struct s_MOVE_COST_MAP
{
    /* 0x0000 */ int8_t moves2[2400];
    /* 0x0960 */
};
#pragma pack(pop)


// drake178: HMP_MAPS
// sizeof:  3840h  14,400
/*
    movement cost per movement mode, in moves2 units
    one *map* per movement mode, as world map - 60x40 - map squares
*/
#pragma pack(push)
#pragma pack(2)
struct s_MOVE_MODE_COST_MAPS
{
    /* 0x0000 */ struct s_MOVE_COST_MAP UU_MvMd;
    /* 0x0960 */ struct s_MOVE_COST_MAP walking;
    /* 0x12C0 */ struct s_MOVE_COST_MAP forester;
    /* 0x1C20 */ struct s_MOVE_COST_MAP mountaineer;
    /* 0x2580 */ struct s_MOVE_COST_MAP swimming;
    /* 0x2EE0 */ struct s_MOVE_COST_MAP sailing;
    /* 0x3840 */
};
#pragma pack(pop)


// drake178: OVL_Path_Results
// sizeof: 2780h  10112d
#pragma pack(push)
#pragma pack(2)
struct s_MOVE_PATH
{
/* 0000 */  int8_t   moves2[2400];
/* 0960 */  uint8_t  Reach_Costs[2400];
/* 12C0 */  int16_t  Reach_From[2400];
/* 2580 */  int16_t  Reverse_Path[256];
/* 2780 */
};
#pragma pack(pop)



/*
    WIZARDS.EXE  ovr147
*/

// drake178: Overland_Pathfinder()
void Move_Path_Find(int16_t wx, int16_t wy, struct s_MOVE_PATH * movepath_cost_map);



#ifdef __cplusplus
}
#endif

#endif  /* MOVEPATH_H */
