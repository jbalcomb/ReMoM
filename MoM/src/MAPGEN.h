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
void NEWG_CreateWorld__WIP(void);

// MGC o51p02
// NEWG_EZ_MarkHadnLeft()

// MGC o51p03
void NEWG_TileIsleExtend__WIP(int16_t wp);

// MGC o51p04
void NEWG_CreateTowers__WIP(void);

// MGC o51p05
void Generate_Home_City(void);

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
void NEWG_CreateNodes__WIP(void);

// MGC o51p12
void NEWG_CreateNodeAura__WIP(int16_t power, int8_t * Aura_Xs, int8_t * Aura_Ys, int16_t wx, int16_t wy);

// MGC o51p13
int16_t NODE_IsAuraUnique__WIP(int16_t node_idx);

// MGC o51p14
void NEWG_SetNodeType__WIP(int16_t power, int8_t * Aura_Xs, int8_t * Aura_Ys, int16_t wp, int8_t * type);

// MGC o51p15
void TILE_SetLandMass__WIP(int16_t wp, int16_t wx, int16_t wy);

// MGC o51p16
void NEWG_CreateEZs__WIP(void);

// MGC o51p17
void NEWG_CreateEncounter__WIP(int16_t lair_idx, int16_t wp, int16_t wx, int16_t wy, int16_t UU, int16_t lair_type, int16_t budget);

// MGC o51p18
// UU_UNIT_RandomRacial()

// MGC o51p19
// NEWG_SetDeserts()

// MGC o51p20
// NEWG_CreateShores()

// MGC o51p21
// NEWG_RandomizeTiles()

// MGC o51p22
// NEWG_CreateRiver()

// MGC o51p23
// NEWG_SetRiverTiles()

// MGC o51p24
// NEWG_CreateNeutrals()

// MGC o51p25
void Random_City_Name_By_Race_NewGame(int16_t race_type, char * name);

// MGC o51p26
// NEWG_CreateRoads()

// MGC o51p27
// CRP_NEWG_CreatePathGrids()

// MGC o51p28
// NEWG_CreateSpecials()

// MGC o51p29
// NEWG_DesertSpecials()

// MGC o51p30
// NEWG_HillSpecials()

// MGC o51p31
// NEWG_MntnSpecials()

// MGC o51p32
// NEWG_SetScoutingMaps()

// MGC o51p33
// TILE_SetScouting()

// MGC o51p34
// NEWG_SetMoveMaps()

// MGC o51p35
// TILE_InvalidOutflow()

// MGC o51p36
void Draw_Building_The_Worlds(int16_t percent);

// MGC o51p37
void Create_Unit_NewGame(int16_t unit_type, int16_t player_idx, int16_t wx, int16_t wy, int16_t wp, int16_t city_idx);

// MGC o51p38
// NEWG_AnimateOceans()

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
// TILE_IsMountains()

// MGC o51p47
// TILE_IsHills()

// MGC o51p48
// TILE_IsSwamp()

// MGC o51p49
// TILE_IsDesert()

// MGC o51p50
// TILE_IsGrasslands()

// MGC o51p51
int16_t Square_Food2_NewGame(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p52
int16_t City_Maximum_Size_NewGame(int16_t wx, int16_t wy, int16_t wp);

// MGC o51p53
int16_t Get_Useable_City_Area_NewGame(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t * wx_array, int16_t * wy_array);

// MGC o51p54
void o51p54_empty_function(void);



#ifdef __cplusplus
}
#endif

#endif  /* MAPGEN_H */
