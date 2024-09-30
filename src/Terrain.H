/*
    Terrain
    Terrain Types
    Terrain Type Attributes


    WIZARDS.EXE
        ovr161
    
*/

#ifndef TERRAIN_H
#define TERRAIN_H

#include "MoM.H"

#ifdef __cplusplus
extern "C" {
#endif


/*
IDK
mixed-use bitfield
4-bit value
4 bits
*/
enum enum_Terrain_Specials
{
    ts_Iron           =   1,
    ts_Coal           =   2,
    ts_Silver         =   3,
    ts_Gold           =   4,
    ts_Gems           =   5,
    ts_Mithril        =   6,
    ts_Adamantium     =   7,
    ts_QuorkCrystals  =   8,
    ts_CrysxCrystals  =   9,
    ts_HuntersLodge   =  16,
    ts_Unknown_20     =  32,
    ts_Wild_Game      =  64,
    ts_Nightshade     = 128
};

#define TS_IRON             1  // 0001
#define TS_COAL             2  // 0020
#define TS_SILVER           3  // 0011
#define TS_GOLD             4  // 0100
#define TS_GEM              5  // 0101
#define TS_MITHRIL          6  // 0110
#define TS_ADAMANTIUM       7  // 0111
#define TS_QUORK            8  // 1000
#define TS_CRYSX            9  // 1001
#define TS_HUNTERSLODGE  0x10  // 0001
#define TS_UNKNOWN_20H   0x20  // 0010
#define TS_WILDGAME      0x40  // 0100
#define TS_NIGHTSHADE    0x80  // 1000


// drake178: enum Terrain_Flags
// 0x01
// 0x02
// 0x04
#define TF_Road         0x08
#define TF_Enc_Road     0x10
#define TF_Corruption   0x20
#define TF_Unk_40       0x40
#define TF_NoRoad80     0x80  // ~== -1 ST_UNDEFINED
#define TF_AnyRoad      0x18



// WZD s161p01
void Map_Square_Clear_Corruption(int16_t wx, int16_t wy, int16_t wp);

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
// drake178: TILE_GetSpecialPower()
int16_t Map_Square_Magic_Power(int16_t wx, int16_t wy, int16_t wp, int16_t have_miners_guild, int16_t are_dwarf);

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
// drake178: TILE_GetNUCostReduce()
int16_t Terrain_Unit_Cost_Reduction(int16_t wx, int16_t wy, int16_t wp, int16_t have_minersguild, int16_t are_dwarves);

// WZD s161p19
int16_t City_Best_Weapon(int16_t city_idx);

// WZD s161p20
int16_t Turns_To_Build_Road(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p21
// UU_TILE_GetUnsdMPCost

// WZD s161p22
// TILE_IsAISailable    

// WZD s161p23
// drake178: TILE_IsVisibleForest 
int16_t Terrain_Is_Explored_Forest(int16_t wx, int16_t wy, int16_t wp);

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
// drake178: TILE_IsHills()
int16_t Terrain_Is_Hills(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p29
// drake178: TILE_IsMountains()
int16_t Terrain_Is_Mountain(int16_t wx, int16_t wy, int16_t wp);

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
void All_City_Nightshade_Count(void);

// WZD s161p38
// TILE_IsLand          

// WZD s161p39
// drake178: TILE_IsAISailable2()
int16_t Terrain_Is_Ocean(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p40
// UU_TILE_BotchedResFn 



#ifdef __cplusplus
}
#endif

#endif  /* TERRAIN_H */
