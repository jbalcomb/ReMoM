/*
    WIZARDS.EXE
        ovr143
*/

#ifndef WZD_O143_H
#define WZD_O143_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr143
*/

// WZD o143p01
// TILE_MakeVolcano()

// WZD o143p02
// TILE_CountVolcanoes()

// WZD o143p03
// TILE_MakeGrassland()

// WZD o143p04
// TILE_MakeForest()

// WZD o143p05
// RP_TILE_MakeDesert()

// WZD o143p06
// TILE_MakeMountain()

// WZD o143p07
// TILE_MakeHill()

// WZD o143p08
// TILE_AdjustMapFlow()

// WZD o143p09
// TILE_AdjacentFlow()

// WZD o143p10
// TILE_GetRoughType()

// WZD o143p11
// TILE_GenerateOre()

// WZD o143p12
void Random_City_Name_By_Race(int16_t race, char * name);

// WZD o143p13
// EVNT_TargetDepletion()

// WZD o143p14
// EVNT_FindNewMineral()

// WZD o143p15
// TILE_ReplaceMinerals()

// WZD o143p16
// TILE_RemoveMinerals()

// WZD o143p17
// drake178: CTY_ResetRoadConns()
void Reset_City_Road_Connection_Bitfields(void);

// WZD o143p18
void TILE_ResetRoadConns(int16_t wx, int16_t wy, int16_t wp);

// WZD o143p19
int16_t OVL_GetRoadPath(int16_t src_wx, int16_t src_wy, int16_t dst_wx, int16_t dst_wy, int16_t wp, int8_t * path_string_x, int8_t * path_string_y);


#ifdef __cplusplus
}
#endif



#endif /* WZD_O143_H */
