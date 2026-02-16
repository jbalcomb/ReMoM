/*
    MAGIC.EXE
    ovr051
*/

#ifndef MAPGEN_H
#define MAPGEN_H

#include "../../MoX/src/MOX_TYPE.h"

#ifdef __cplusplus
extern "C" {
#endif



/*
    MAGIC.EXE
    ovr051
*/

// MGC o51p01
void Init_New_Game(void);

// MGC o51p02
void Set_Upper_Lair_Guardian_Count(void);

// MGC o51p03
void NEWG_TileIsleExtend__WIP(int16_t wp);

// MGC o51p04
void Generate_Towers(void);

// MGC o51p05
void Generate_Home_City__WIP(void);

// MGC o51p06
void NEWG_ClearLandmasses__WIP(int16_t wp);

// MGC o51p07
void NEWG_EqualizeNodes__WIP(int16_t wp);

// MGC o51p08
void NEWG_SetSpecLands__WIP(int16_t wp);

// MGC o51p09
void NEWG_SetBaseLands__WIP(int16_t wp);

// MGC o51p10
void NEWG_CreateLands__WIP(int16_t wp);

// MGC o51p11
void Generate_Nodes(void);

// MGC o51p12
void NEWG_CreateNodeAura__WIP(int16_t power, int8_t * Aura_Xs, int8_t * Aura_Ys, int16_t wx, int16_t wy);

// MGC o51p13
int16_t NODE_IsAuraUnique__WIP(int16_t node_idx);

// MGC o51p14
void NEWG_SetNodeType__WIP(int16_t power, int8_t * Aura_Xs, int8_t * Aura_Ys, int16_t wp, int8_t * type);

// MGC o51p15
void TILE_SetLandMass__WIP(int16_t wp, int16_t wx, int16_t wy);

// MGC o51p16
void Generate_Lairs(void);

// MGC o51p17
void NEWG_CreateEncounter__WIP(int16_t lair_idx, int16_t wp, int16_t wx, int16_t wy, int16_t UU, int16_t lair_type, int16_t budget);

// MGC o51p18
// UU_UNIT_RandomRacial()

// MGC o51p19
void NEWG_SetDeserts__STUB(void);

// MGC o51p20
void NEWG_CreateShores__STUB(void);

// MGC o51p21
void NEWG_RandomizeTiles__STUB(void);

// MGC o51p22
int16_t NEWG_CreateRiver__STUB(int16_t wp);

// MGC o51p23
void NEWG_SetRiverTiles__STUB(int16_t wp);

// MGC o51p24
void Generate_Neutral_Cities__WIP(int16_t wp);

// MGC o51p25
void Random_City_Name_By_Race_NewGame(int16_t race_type, char * name);

// MGC o51p26
void Generate_Roads(int16_t wp);

// MGC o51p27
void CRP_NEWG_CreatePathGrids__WIP(int16_t wp);

// MGC o51p28
void Generate_Terrain_Specials(int16_t wp);

// MGC o51p29
int16_t Desert_Terrain_Special(int16_t wp);

// MGC o51p30
int16_t Hills_Terrain_Special(int16_t wp);

// MGC o51p31
int16_t Mountain_Terrain_Special(int16_t wp);

// MGC o51p32
void Init_Square_Explored(void);

// MGC o51p33
void Set_Square_Explored_Bits(int16_t wp, int16_t wx, int16_t wy, int16_t bits);

// MGC o51p34
void Movement_Mode_Cost_Maps(int16_t wp);

// MGC o51p35
// TILE_InvalidOutflow()

// MGC o51p36
void Draw_Building_The_Worlds(int16_t percent);

// MGC o51p37
void Create_Unit_NewGame(int16_t unit_type, int16_t player_idx, int16_t wx, int16_t wy, int16_t wp, int16_t city_idx);

// MGC o51p38
void NEWG_AnimateOceans__STUB(void);

// MGC o51p39
int16_t TILE_IsOcean(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p40
int16_t TILE_HasNode(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p41
int16_t TILE_HasTower(int16_t wx, int16_t wy);

// MGC o51p42
int16_t Square_Is_Forest_NewGame(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p43
// TILE_IsRiver()

// MGC o51p44
// UU_TILE_IsRiverOutlet()

// MGC o51p45
// UU_TILE_IsShore()

// MGC o51p46
int16_t Square_Is_Mountain_NewGame(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p47
int16_t Square_Is_Hills_NewGame(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p48
int16_t Square_Is_Swamp_NewGame(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p49
int16_t Square_Is_Desert_NewGame(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p50
int16_t Square_Is_Grassland_NewGame(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p51
int16_t Square_Food2_NewGame(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p52
int16_t City_Maximum_Size_NewGame(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p53
int16_t Get_Useable_City_Area_NewGame(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t * wx_array, int16_t * wy_array);

// MGC o51p54
void o51p54_empty_function(void);



/*

    MAGIC.EXE  ovr054

*/

// MGC o54p01
// CRP_SPATH_Arbitrary()
// MGC o54p02
// UU_SPATH_Segmented()
// MGC o54p03
// UU_SPATH_15by15()
// MGC o54p04
// UU_SPATH_Segments()
// MGC o54p05
// UU_SPATH_GetBorderTiles()



/*

    MAGIC.EXE  ovr055

*/

// MGC o55p01
void CRP_NEWG_CreatePathGrid__STUB(void * moves2, void * move_path_chunks);



#ifdef __cplusplus
}
#endif

#endif  /* MAPGEN_H */
