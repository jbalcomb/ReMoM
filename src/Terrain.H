/*
    Terrain
    Terrain Types
    Terrain Type Attributes


    WIZARDS.EXE
        ovr161
    
*/

#ifndef TERRAIN_H
#define TERRAIN_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



// WZD s161p01
// TILE_ClearCorruption 
// WZD s161p02
// TILE_IsNode          

// WZD s161p03
// drake178: TILE_GetFood()
int16_t Map_Square_Food2(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p04
// drake178: TILE_GetProd()
int16_t Map_Square_Production_Bonus(int16_t wx, int16_t wy, int16_t wp, int16_t have_gaias_blessing);

// WZD s161p05
// drake178: TILE_GetWaterGold()
int16_t Map_Square_Gold_Bonus(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p06
// drake178: CTY_GetRoadGold()
int16_t City_Road_Trade_Bonus(int16_t city_idx);

// WZD s161p07
// drake178: TILE_IsRiver()
int16_t Terrain_Is_River(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p08
// drake178: TILE_GetSpecialGold()
int16_t Map_Square_Gold_Income(int16_t wx, int16_t wy, int16_t wp, int16_t have_miners_guild, int16_t are_dwarf);

// WZD s161p08
// TILE_GetSilverGold   
// WZD s161p09
// TILE_GetGoldOreGold  
// WZD s161p10
// TILE_GetGemGold      
// WZD s161p11
// TILE_GetSpecialPower 
// WZD s161p12
// TILE_GetMithrilPower 
// WZD s161p13
// TILE_GetAdamntmPower 
// WZD s161p14
// TILE_GetQuorkPower   
// WZD s161p15
// TILE_GetCrysxPower   
// WZD s161p16
// TILE_HasMithril      
// WZD s161p17
// TILE_HasAdamantium   
// WZD s161p18
// TILE_GetNUCostReduce 
// WZD s161p19
// CTY_GetWeaponQuality 
// WZD s161p20
// TILE_GetRoadBldTime  
// WZD s161p21
// UU_TILE_GetUnsdMPCost
// WZD s161p22
// TILE_IsAISailable    
// WZD s161p23
// TILE_IsVisibleForest 

// WZD s161p24
// drake178: TILE_IsSailable()
int16_t Terrain_Is_Sailable(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p25
// TILE_IsDeepOcean     
// WZD s161p26
// G_TILE_IsAIEmbarkable
// WZD s161p27
// TILE_BuildingReqType 
// WZD s161p28
// TILE_IsHills         
// WZD s161p29
// TILE_IsMountains     
// WZD s161p30
// TILE_IsDesert        
// WZD s161p31
// TILE_IsTundra        
// WZD s161p32
// TILE_IsSwamp         
// WZD s161p33
// TILE_IsGrasslands    
// WZD s161p34
// TILE_IsVolcano       
// WZD s161p35
// TILE_IsCorrupted     
// WZD s161p36
// UU_TILE_IsRes_40h    
// WZD s161p37
// CTY_CountNightshades 
// WZD s161p38
// TILE_IsLand          
// WZD s161p39
// TILE_IsAISailable2   
// WZD s161p40
// UU_TILE_BotchedResFn 



#ifdef __cplusplus
}
#endif

#endif  /* TERRAIN_H */
