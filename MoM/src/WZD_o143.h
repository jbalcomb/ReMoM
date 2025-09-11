/*
    WIZARDS.EXE
        ovr143
*/

#ifndef WZD_O143_H
#define WZD_O143_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr143
*/

// WZD o143p01
void Set_Terrain_Type_Volcano_With_Owner(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

// WZD o143p02
void Volcano_Counts(void);

// WZD o143p03
void Set_Terrain_Type_Grasslands(int16_t wx, int16_t wy, int16_t wp);

// WZD o143p04
void Set_Terrain_Type_Forest(int16_t wx, int16_t wy, int16_t wp);

// WZD o143p05
// RP_TILE_MakeDesert()

// WZD o143p06
void Set_Terrain_Type_Mountain(int16_t wx, int16_t wy, int16_t wp);

// WZD o143p07
void Set_Terrain_Type_Hills(int16_t wx, int16_t wy, int16_t wp);

// WZD o143p08
// TILE_AdjustMapFlow()

// WZD o143p09
// TILE_AdjacentFlow()

// WZD o143p10
// TILE_GetRoughType()

// WZD o143p11
void TILE_GenerateOre__STUB(int16_t wx, int16_t wy, int16_t wp);

// WZD o143p12
void Random_City_Name_By_Race(int16_t race, char * name);

// WZD o143p13
int16_t EVNT_TargetDepletion__STUB(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp, int16_t * terrain_special);

// WZD o143p14
int16_t EVNT_FindNewMineral__STUB(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp, int16_t * terrain_special);

// WZD o143p15
void TILE_ReplaceMinerals__STUB(int16_t wx, int16_t wy, int16_t wp, int16_t terain_special);

// WZD o143p16
void Clear_Terrain_Specials(int16_t wx, int16_t wy, int16_t wp);

// WZD o143p17
void Reset_City_Road_Connection_Bitfields(void);

// WZD o143p18
void TILE_ResetRoadConns(int16_t wx, int16_t wy, int16_t wp);

// WZD o143p19
int16_t OVL_GetRoadPath(int16_t src_wx, int16_t src_wy, int16_t dst_wx, int16_t dst_wy, int16_t wp, int8_t * path_string_x, int8_t * path_string_y);


#ifdef __cplusplus
}
#endif



#endif /* WZD_O143_H */
