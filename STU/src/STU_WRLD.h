#pragma once

#include <stdint.h>
#include "../../MoM/src/TerrType.h" /* TerType_Count */
#include "../../MoX/src/MOM_DEF.h"  /* NUM_LANDMASSES */

#define LANDMASS_INDEX_MAX 256

    // ; convert each non-ocean map square into a basic land based
    // ; on the amount of "hits" it got during the continent
    // ; generation
    // ;    1: Grassland ($A2) with 3/4 chance, or Forest
    // ;  2-3: Forest ($A3)
    // ;  4-5: Hills ($AB)
    // ;   6+: Mountains ($A4)
/*
Drunkard's Walk
Height-Map
translate from height to terrain type
0 === Ocean  (was NO_LANDMASS, means it never got hit by a walker)
1 === Grassland 75%, Forest 25%
2-3 === Forest
4-5 === Hills
6+ === Mountains

*/



/*
    s_Worldmap_Map_Square_Hit_Count

    Tracks the count of map squares for each terrain type in a world map plane.
    - terrain_type_count: array indexed by terrain type (see TerrType.h)
    - total_count: total number of map squares processed

    // sizeof: 104h 260d (assuming TerType_Count == 256)
*/
#pragma pack(push, 2)
struct s_Worldmap_Map_Square_Hit_Count
{
    int32_t terrain_type_count[TerType_Count]; /* 0x0000 */
    int32_t total_count;                       /* 0x0100 */
};
#pragma pack(pop)

/*
    s_Terrain_Specials_Map_Square_Hit_Count

    Tracks the count of map squares for each terrain special byte value in a world map plane.
    The byte is a mixed-use bitfield: low nibble (bits 3:0) is the mineral type (0=none, 1-9),
    high nibble holds flags (TS_HUNTERSLODGE=0x10, TS_UNKNOWN_20H=0x20, TS_WILDGAME=0x40, TS_NIGHTSHADE=0x80).
    - special_value_count: array indexed by raw terrain special byte (0x00-0xFF)
    - total_count: total number of map squares processed
    - special_squares: count of squares with any terrain special (value != TS_NONE)

    // sizeof: 40Ch 1036d
*/
#pragma pack(push, 2)
struct s_Terrain_Specials_Map_Square_Hit_Count
{
    int32_t special_value_count[256]; /* 0x0000 */
    int32_t total_count;              /* 0x0400 */
    int32_t special_squares;          /* 0x0404 */
};
#pragma pack(pop)

/*
    s_Landmass_Map_Square_Hit_Count

    Tracks the count of map squares for each landmass index in a world map plane.
    - hit_count: array indexed by landmass index (0-255)
    - total_count: total number of map squares processed
    - land_squares: total_count minus ocean squares (hit_count[0])

    // sizeof: 408h 1032d
*/
#pragma pack(push, 2)
struct s_Landmass_Map_Square_Hit_Count
{
    int32_t hit_count[LANDMASS_INDEX_MAX]; /* 0x0000 */
    int32_t total_count;                   /* 0x0400 */
    int32_t land_squares;                  /* 0x0404 */
};
#pragma pack(pop)

/*
    s_Landmass_Walk_Stats

    Statistics for landmass walker simulation.
    - trial_count: total walker steps
    - hit_count: number of squares visited at least once
    - sum: sum of all visit counts

    // sizeof: 0Ch 12d
*/
#pragma pack(push, 2)
struct s_Landmass_Walk_Stats
{
    int32_t trial_count; /* 0x0000 */
    int32_t hit_count;   /* 0x0004 */
    int32_t sum;         /* 0x0008 */
};
#pragma pack(pop)

uint8_t Get_Landmass(int16_t wx, int16_t wy, int16_t wp);

void Terrain_Specials_Statistics(int map_idx);
void Display_Terrain_Specials_Statistics(int map_idx);
void Display_Terrain_Specials_Histogram(int map_idx);
void Print_Terrain_Specials_Map(int map_idx);
