/*
    Terrain
    Terrain Types
    Terrain Type Attributes


    WIZARDS.EXE
        ovr161
    
*/

#ifndef TERRAIN_H
#define TERRAIN_H

#include "MOM.H"

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
    ts_Mithril        =   6,  // "TERRAIN SPECIAL ... Mithril Ore"
    ts_Adamantium     =   7,  // "TERRAIN SPECIAL ... Adamantium"
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
/*
Meh. I put stuff in MoX_DEF.H that is mean to have access to this.
*/
// 0x01
// 0x02
// 0x04
#define MSF_ROAD        0x08
#define TF_Enc_Road     0x10
#define MSF_CORRUPTION  0x20
#define MSF_UNKNOWN_40H 0x40
#define TF_NoRoad80     0x80  // ~== -1 ST_UNDEFINED
#define TF_AnyRoad      0x18    // (MSF_ROAD | TF_Enc_Road)



// WZD s161p01
void Map_Square_Clear_Corruption(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p02
// TILE_IsNode          

// WZD s161p03
int16_t Square_Food2(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p04
int16_t Square_Production_Bonus(int16_t wx, int16_t wy, int16_t wp, int16_t have_gaias_blessing);

// WZD s161p05
int16_t Square_Gold_Bonus(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p06
int16_t City_Road_Trade_Bonus(int16_t city_idx);

// WZD s161p07
int16_t Square_Is_River(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p08
int16_t Square_Gold_Income(int16_t wx, int16_t wy, int16_t wp, int16_t have_miners_guild, int16_t are_dwarf);

// WZD s161p08
// TILE_GetSilverGold   

// WZD s161p09
// TILE_GetGoldOreGold  

// WZD s161p10
// TILE_GetGemGold      

// WZD s161p11
int16_t Square_Magic_Power(int16_t wx, int16_t wy, int16_t wp, int16_t have_miners_guild, int16_t are_dwarf);

// WZD s161p12
int16_t Square_Mithril_Power(int16_t wx, int16_t wy, int16_t wp, int16_t have_minersguild, int16_t are_dwarves);

// WZD s161p13
int16_t Square_Adamantium_Power(int16_t wx, int16_t wy, int16_t wp, int16_t have_minersguild, int16_t are_dwarves);

// WZD s161p14
// TILE_GetQuorkPower   

// WZD s161p15
// TILE_GetCrysxPower   

// WZD s161p16
int16_t Square_Has_Mithril(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p17
int16_t Square_Has_Adamantium(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p18
int16_t Square_Unit_Cost_Reduction(int16_t wx, int16_t wy, int16_t wp, int16_t have_minersguild, int16_t are_dwarves);

// WZD s161p19
int16_t City_Best_Weapon(int16_t city_idx);

// WZD s161p20
int16_t Turns_To_Build_Road(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p21
// UU_TILE_GetUnsdMPCost

// WZD s161p22
int16_t Square_Is_Shoreline(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p23
int16_t Square_Is_Forest(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p24
int16_t Square_Is_Sailable(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p25
int16_t Square_Is_Ocean(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p26
int16_t Map_Square_Is_Embarkable(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p27
// TILE_BuildingReqType 

// WZD s161p28
int16_t Square_Is_Hills(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p29
int16_t Square_Is_Mountain(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p30
int16_t Square_Is_Desert(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p31
int16_t Square_Is_Tundra(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p32
int16_t Square_Is_Swamp(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p33
int16_t Square_Is_Grasslands(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p34
int16_t Square_Is_Volcano(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p35
// drake178: TILE_IsCorrupted()

// WZD s161p36
// drake178: UU_TILE_IsRes_40h()

// WZD s161p37
void All_City_Nightshade_Count(void);

// WZD s161p38
int16_t Map_Square_Is_Land(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p39
int16_t Square_Is_OceanLike(int16_t wx, int16_t wy, int16_t wp);

// WZD s161p40
// UU_TILE_BotchedResFn 



#ifdef __cplusplus
}
#endif

#endif  /* TERRAIN_H */
