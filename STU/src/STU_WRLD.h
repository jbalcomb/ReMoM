#ifndef STU_WRLD_H
#define STU_WRLD_H

#include <stdint.h>
#include "../../MoM/src/TerrType.h" /* TerType_Count */
#include "../../MoX/src/MOM_DEF.h"  /* NUM_LANDMASSES, WORLD_WIDTH, WORLD_HEIGHT, NUM_TOWERS, NUM_NODES */
#include "../../MoX/src/MOM_DAT.h" /* struct s_TOWER, struct s_NODE */

#define LANDMASS_INDEX_MAX          256
#define NUM_TERRAIN_GROUPS           16  /* Oceans, Grasslands, Forests, Hills, Mountains, Deserts, Swamps, Tundras */
#define NUM_TERRAIN_SPECIAL_TYPES   256

/*
    s_Landmass_Squares_Stats

    Tracks the count of map squares for each landmass index in a world map plane.
    - hit_count: array indexed by landmass index (0-255)
    - total_count: total number of map squares processed
    - land_squares: total_count minus ocean squares (hit_count[0])

    // sizeof: 408h 1032d
*/
#pragma pack(push, 2)
struct s_Landmass_Squares_Stats
{
    int32_t hit_count[LANDMASS_INDEX_MAX]; /* 0x0000 */
    int32_t total_count;                   /* 0x0400 */
    int32_t land_squares;                  /* 0x0404 */
    int32_t max_hit_count_index;           /* 0x0408 - highest landmass index encountered (for reporting) */
};
#pragma pack(pop)

/*
    s_Worldmap_Squares_Stats

    Tracks the count of map squares for each terrain type in a world map plane.
    - terrain_type_count: array indexed by terrain type (see TerrType.h)
    - total_count: total number of map squares processed

    // sizeof: 104h 260d (assuming TerType_Count == 256)
*/
#pragma pack(push, 2)
struct s_Worldmap_Squares_Stats
{
    int32_t terrain_type_count[NUM_TERRAIN_TYPES];
    int32_t terrain_group_count[NUM_TERRAIN_GROUPS];
    int32_t total_count;
};
#pragma pack(pop)

/*
    s_Tower_Map_Square_Hit_Count

    Tracks tower squares found during world map statistics collection.
    One instance per plane, consistent with the other stats structs.
    Towers have no plane component (s_TOWER stores only wx, wy with no wp),
    so the same towers appear on both planes.
    - tower_count: number of tower squares on this plane

    // sizeof: 04h 4d
*/
#pragma pack(push, 2)
struct s_Tower_Map_Square_Hit_Count
{
    int32_t tower_count;
    int16_t wx[NUM_TOWERS];
    int16_t wy[NUM_TOWERS];
};
#pragma pack(pop)

/*
    s_Terrain_Special_Squares_Stats

    Tracks the count of map squares for each terrain special byte value in a world map plane.
    The byte is a mixed-use bitfield: low nibble (bits 3:0) is the mineral type (0=none, 1-9),
    high nibble holds flags (TS_HUNTERSLODGE=0x10, TS_UNKNOWN_20H=0x20, TS_WILDGAME=0x40, TS_NIGHTSHADE=0x80).
    - terrain_special_square_count: array indexed by raw terrain special byte (0x00-0xFF)
    - total_count: total number of map squares processed
    - special_squares: count of squares with any terrain special (value != TS_NONE)

    // sizeof: 40Ch 1036d
*/
#pragma pack(push, 2)
struct s_Terrain_Special_Squares_Stats
{
    int32_t terrain_special_square_count[NUM_TERRAIN_SPECIAL_TYPES];  /* count of each terrain special squares */
    int32_t total_count;  /* DUPE  total count of world map squares */
    int32_t special_squares;  /* count of squares with any terrain special (value != ts_NONE) */
};
#pragma pack(pop)

/*
    s_Simulation_Data

    Per-simulation snapshot of game data and derived statistics.
    Each simulation produces data for both planes (Arcanus and Myrror);
    some game functions do not support wp, so both planes are always
    captured together.  All per-plane arrays are indexed [0]=Arcanus, [1]=Myrror.

    Game data is copied from the transient globals after each simulation run
    so that previous runs can be revisited for analysis.
*/
#pragma pack(push, 2)
struct s_Simulation_Data
{

    /* game data — snapshot from globals before next run overwrites them */
    uint8_t         landmasses[NUM_PLANES][WORLD_HEIGHT][WORLD_WIDTH];        /* 1 byte/square  */
    int16_t         world_map[NUM_PLANES][WORLD_HEIGHT][WORLD_WIDTH];         /* 2 bytes/square */
    uint8_t         terrain_specials[NUM_PLANES][WORLD_HEIGHT][WORLD_WIDTH];  /* 1 byte/square  */
    uint8_t         map_flags[NUM_PLANES][WORLD_HEIGHT][WORLD_WIDTH];         /* 1 byte/square  */
    struct s_TOWER  towers[NUM_PLANES][NUM_TOWERS];                           /* duplicated — towers have no wp */
    struct s_NODE   nodes[NUM_PLANES][NUM_NODES];                             /* duplicated — nodes have no wp  */

    /* statistics — derived from the game data above */
    struct s_Landmass_Squares_Stats        landmass_stats[NUM_PLANES];
    struct s_Worldmap_Squares_Stats        worldmap_stats[NUM_PLANES];
    struct s_Terrain_Special_Squares_Stats specials_stats[NUM_PLANES];
    struct s_Tower_Map_Square_Hit_Count    tower_stats[NUM_PLANES];
};
#pragma pack(pop)
extern struct s_Simulation_Data * simulation_data;



#ifdef __cplusplus
extern "C" {
#endif

void Simulate_World_Map_Generation(void);

#ifdef __cplusplus
}
#endif

#endif /* STU_WRLD_H */
