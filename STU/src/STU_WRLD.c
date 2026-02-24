/*

Simulation Code for MoM's MAPGEN

Dependencies:
    int16_t m_landmasses_ctr
    uint8_t * _landmasses
    uint8_t * _world_maps
    Generate_Terrain_Specials
        _LAIRS[]
        _TOWERS[]
        _CITIES[]
    Generate_Home_City__WIP()
        _FORTRESSES[]
        _NODES[]
        _TOWERS[]
        _LAIRS[]
        _CITIES[]
        ...Random_City_Name_By_Race_NewGame()
        _cities
        LBX_Load_Data_Static(cityname_lbx_file__MGC_ovr051, 0, (SAMB_ptr)city_names_buffer, race_idx, 1, 280);
        
    Generate_Neutral_Cities__WIP()
        _UNITS[]

Terrain Types:
 - Basic Terrain Types:
    - Ocean
    - Grasslands
    - Forest
    - Mountain
    - Desert
    - Swamp
    - Tundra
    - Hills
 - Other Terrain Types:
    - Animated Ocean
    - Volcano
    - Shore
    - Lake
    - River
    - River Mouth
 - Special Terrain Types:
    - Sorcery Lake  (Node)
    - Nature Forest (Node)
    - Chaos Volcano (Node)

Text-Mode Terrain Symbols:
- . Ocean
- g Grasslands
- f Forest
- m Mountain
- d Desert
- s Swamp
- t Tundra
- h Hills
- v Volcano
- ~ Animated Ocean
- # Shore
- = River
- % River Mouth
- l Lake
- S Sorcery Lake
- N Nature Forest
- C Chaos Volcano
- ? Unknown/Overflow
- B tt_BugGrass  NOTE(drake178): sometimes this is Grasslands, but there are bugs in the code around its usage
*/
/*
MoO2
Module: MAPGEN
    data (0 bytes) _num_maps_to_generate
        Address: 02:001942CC

*/

#include "../../MoM/src/MAPGEN.h"
#include "../../MoM/src/NewGame.h"      /* NEWG_Clicked_Race */
#include "../../MoM/src/RACETYPE.h"     /* rt_High_Men */
#include "../../MoM/src/Terrain.h"      /* TS_NONE, TS_IRON, TS_WILDGAME, ... */
#include "../../MoM/src/TerrType.h"     /* tt_Ocean1 */

#include "../../MoX/src/Allocate.h"     /* Allocate_Space() */
#include "../../MoX/src/LBX_Load.h"     /* LBX_Load_Data() */
#include "../../MoX/src/MOX_BITS.h"     /* GET_2B_OFS() */
#include "../../MoX/src/MOM_Data.h"     /* _landmasses, _world_maps */
#include "../../MoX/src/MOM_DEF.h"      /* WORLD_WIDTH, WORLD_HEIGHT */
#include "../../MoX/src/MOX_DEF.h"      /* GET_TERRAIN_TYPE() */

#include "STU_WRLD.h"

#include <assert.h>  /* assert() */
#include <stdio.h>   /* FILE, fopen, ftell, fprintf, fclose */
#include <memory.h>  /* memset() === vcruntime_string.h */
#include <string.h>  /* memset() */



// MGC  dseg:9040
extern int16_t m_landmasses_ctr;



int max_hit_count_index;

struct s_Landmass_Map_Square_Hit_Count * landmass_map_square_hit_count;

struct s_Worldmap_Map_Square_Hit_Count * worldmap_map_square_hit_count;

struct s_Terrain_Specials_Map_Square_Hit_Count * terrain_specials_map_square_hit_count;

static const char lm_chars[] =
    ".123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "+";

/*************************************
 * Forward Declare Private Functions *
 *************************************/
static uint8_t Get_Landmass(int16_t wx, int16_t wy, int16_t wp);
static int16_t Get_Terrain(int16_t wx, int16_t wy, int16_t wp);
static uint8_t Get_Terrain_Special(int16_t wx, int16_t wy, int16_t wp);
static char Terrain_Type_To_Char(int16_t tt);

/********************
 * Helper Functions *
 ********************/

/*
Get_Landmass

Returns the landmass index for the world-map square at (wx, wy) on plane wp.
*/
static uint8_t Get_Landmass(int16_t wx, int16_t wy, int16_t wp)
{
    return _landmasses[ (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx ];
}

/*
Get_Terrain

_world_maps is a flat, packed byte array that holds one or more world planes laid out end-to-end.  Each map square is stored as a
little-endian 16-bit terrain-type index, so each square occupies exactly 2 bytes.  A single plane is WORLD_WIDTH * WORLD_HEIGHT
squares, which equals WORLD_SIZE squares total, or WORLD_SIZE * 2 bytes per plane.

The byte offset of square (wx, wy) on plane wp is therefore:

    ofs = (wp * WORLD_SIZE   * 2)   -- skip wp complete planes
        + (wy * WORLD_WIDTH  * 2)   -- skip wy complete rows within the selected plane
        + (wx                * 2)   -- skip wx individual squares within the selected row

The factor of 2 appears at every level because each square is 2 bytes wide.

We then read the two bytes at ofs and ofs+1 and reassemble the little-endian 16-bit value by hand:

    lo  = _world_maps[ofs]       -- bits  7:0  of the terrain-type index (low byte, stored first)
    hi  = _world_maps[ofs + 1]   -- bits 15:8  of the terrain-type index (high byte, stored second)
    val = (uint16_t)lo | ((uint16_t)hi << 8)

Both lo and hi must be widened to uint16_t before the shift and OR.  Shifting a uint8_t left by 8 promotes it to int first
(via the usual integer promotions), and on a platform where int is 16 bits the shift would be undefined behaviour.  Casting to
uint16_t first makes the shift well-defined on all conforming targets and keeps the arithmetic in the unsigned domain throughout,
preventing any sign-extension side-effects before the OR.

The final result is cast to int16_t because the rest of the codebase compares terrain-type values against signed enum members
(e_TERRAIN_TYPES / OVL_Tiles_Extended).  Returning a signed value avoids spurious signed/unsigned comparison warnings at every
call site and matches the type used by GET_TERRAIN_TYPE().
*/
static int16_t Get_Terrain(int16_t wx, int16_t wy, int16_t wp)
{
    int     ofs = (wp * WORLD_SIZE * 2) + (wy * WORLD_WIDTH * 2) + (wx * 2);
    uint8_t lo  = _world_maps[ofs];
    uint8_t hi  = _world_maps[ofs + 1];
    return (int16_t)((uint16_t)lo | ((uint16_t)hi << 8));
}

/*
Get_Terrain_Special

Returns the terrain-special byte for the world-map square at (wx, wy) on plane wp.
_map_square_terrain_specials is a flat, packed byte array laid out identically to
_landmasses: one byte per square, one plane after another.

The byte offset is:

    ofs = (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx
*/
static uint8_t Get_Terrain_Special(int16_t wx, int16_t wy, int16_t wp)
{
    return _map_square_terrain_specials[ (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx ];
}

static char Terrain_Type_To_Char(int16_t tt)
{
    if(tt == tt_Ocean1)         return '.';
    if(tt == tt_BugGrass)       return 'B';
    if(tt == TT_Lake)           return 'l';                        /* 0x12 lake within Shore1 range */
    if(tt <= tt_Shore1_Lst)     return '#';                        /* tt_Shore1_Fst .. tt_Shore1_Lst */
    if(tt == tt_Grasslands1)    return 'g';
    if(tt == tt_Forest1)        return 'f';
    if(tt == tt_Mountain1)      return 'm';
    if(tt == tt_Desert1)        return 'd';
    if(tt == tt_Swamp1)         return 's';
    if(tt == tt_Tundra1)        return 't';
    if(tt == tt_SorceryNode)    return 'S';
    if(tt == tt_NatureNode)     return 'N';
    if(tt == tt_ChaosNode)      return 'C';
    if(tt == tt_Hills1)         return 'h';
    if(tt <= tt_Grasslands3)    return 'g';                        /* tt_Grasslands2 .. tt_Grasslands3 */
    if(tt <= tt_Desert4)        return 'd';                        /* tt_Desert2 .. tt_Desert4 */
    if(tt <= tt_Swamp3)         return 's';                        /* tt_Swamp2 .. tt_Swamp3 */
    if(tt == tt_Volcano)        return 'v';
    if(tt == tt_Grasslands4)    return 'g';
    if(tt <= tt_Tundra3)        return 't';                        /* tt_Tundra2 .. tt_Tundra3 */
    if(tt <= tt_Forest3)        return 'f';                        /* tt_Forest2 .. tt_Forest3 */
    if(tt <= TT_RiverM_end)     return '%';                        /* TT_RiverM_1st .. TT_RiverM_end */
    if(tt <= TT_Lake4)          return 'l';                        /* TT_Lake1 .. TT_Lake4 */
    if(tt <= TT_Shore2F_end)    return '#';                        /* TT_Shore2F_1st .. TT_Shore2F_end */
    if(tt <= tt_Rivers_end)     return '=';                        /* TT_Rivers_1st .. tt_Rivers_end */
    if(tt <= tt_Mountains_Lst)  return 'm';                       /* tt_Mountains_Fst .. tt_Mountains_Lst */
    if(tt <= tt_Hills_Lst)      return 'h';                        /* tt_Hills_Fst .. tt_Hills_Lst */
    if(tt <= tt_Desert_Lst)     return 'd';                        /* tt_Desert_Fst .. tt_Desert_Lst */
    if(tt <= TT_Shore2_end)     return '#';                        /* tt_Shore2_1st .. TT_Shore2_end */
    if(tt <= TT_4WRiver5)       return '=';                        /* TT_4WRiver1 .. TT_4WRiver5 */
    if(tt <= TT_Shore3_end)     return '#';                        /* TT_Shore3_1st .. TT_Shore3_end */
    if(tt == tt_Ocean2)         return '~';
    if(tt <= TT_Tundra_Last)    return 't';                        /* tt_Tundra_1st .. TT_Tundra_Last */
    return '?';
}

/*
Collect_Landmass_Walk_Stats

Scans every square of world plane `wp` and accumulates walk statistics from the
terrain-type heightmap left behind by Generate_Landmasses().

Generate_Landmasses() initialises each square to tt_Ocean1 (== 0) and increments
the terrain-type value by 1 each time the drunkard's walker lands on that square.
The resulting terrain-type value is therefore a raw per-square visit count.

  trial_count = sum of all visit counts = total walker steps (every step adds 1
                to exactly one square, so the total equals the scan sum).
  hit_count   = number of squares visited at least once (terrain > tt_Ocean1).
  sum         = same accumulation; stored separately for caller cross-checking.
*/
void Collect_Landmass_Walk_Stats(int16_t wp, struct s_Landmass_Walk_Stats * stats_out)
{
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    int16_t terrain = 0;

    stats_out->trial_count = 0;
    stats_out->hit_count   = 0;
    stats_out->sum         = 0;

    for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
    {
        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
        {
            terrain = GET_TERRAIN_TYPE(itr_wx, itr_wy, wp);
            if(terrain > tt_Ocean1)
            {
                stats_out->hit_count++;
                stats_out->sum += (int32_t)terrain;
            }
        }
    }

    /* each walker step increments exactly one square by 1, so the total is the sum */
    stats_out->trial_count = stats_out->sum;
}


/*
Write_Landmass_Walk_Stats

Appends one data row for plane `wp` to the TSV file at `filepath`.  If the file
is new or empty a header row is written first so the output is ready for import
into a spreadsheet or analysis script.

Derived metrics written alongside the raw counts:
  avg_prob    = hit_count  / trial_count  -- fraction of steps that created new land
  avg_visits  = sum        / hit_count    -- mean visit count per land square (heightmap mean)
*/
void Write_Landmass_Walk_Stats(const char * filepath, int16_t wp, const struct s_Landmass_Walk_Stats * stats)
{
    FILE  * fp            = NULL;
    long    file_pos      = 0L;
    double  avg_prob      = 0.0;
    double  avg_visits    = 0.0;

    fp = fopen(filepath, "a");
    if(fp == NULL)
    {
        return;
    }

    file_pos = ftell(fp);

    if(file_pos == 0L)
    {
        /* new or empty file: write TSV header */
        fprintf(fp, "plane\ttrial_count\thit_count\tsum\tavg_prob\tavg_visits\n");
    }

    if(stats->trial_count > 0)
    {
        avg_prob = (double)stats->hit_count / (double)stats->trial_count;
    }

    if(stats->hit_count > 0)
    {
        avg_visits = (double)stats->sum / (double)stats->hit_count;
    }

    fprintf(fp, "%d\t%d\t%d\t%d\t%.6f\t%.6f\n",
        (int)wp,
        (int)stats->trial_count,
        (int)stats->hit_count,
        (int)stats->sum,
        avg_prob,
        avg_visits
    );

    fclose(fp);
}

/*
per run of the world map generation procedure

count of values per landmass index
count of landmass indices
NOTE: average probability of a given landmass index is calculated after the simulation is complete

given an unknown number of landmass index values, we need a way to dynamicallly track the count of each landmass index value
this is done by maintaining a list of landmass index values and their counts, and updating the counts as we go through the simulation
when we encounter a landmass index value, we check if it is already in the list, if it is, we increment its count, if it is not, we add it to the list with a count of 1

*/
void Landmass_Statistics(int map_idx)
{
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t wp = 0;
    uint8_t hit_count = 0;

    wp = 0;  // TODO: iterate over all planes generated by the simulation

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            hit_count = Get_Landmass(wx, wy, wp);
            landmass_map_square_hit_count[map_idx].hit_count[hit_count]++;
            landmass_map_square_hit_count[map_idx].total_count++;
            if(hit_count > max_hit_count_index)
            {
                max_hit_count_index = hit_count;
            }
        }
    }

    landmass_map_square_hit_count[map_idx].land_squares =
        landmass_map_square_hit_count[map_idx].total_count
        - landmass_map_square_hit_count[map_idx].hit_count[0];

}

void Display_Landmass_Statistics(int map_idx)
{
    int itr = 0;

    printf("%14s  %9s\n", "Landmass Index", "Squares");
    printf("--------------  ---------\n");
    for(itr = 0; itr < LANDMASS_INDEX_MAX; itr++)
    {
        if(itr <= max_hit_count_index)
        {
            printf("%14d  %9d\n", itr, landmass_map_square_hit_count[map_idx].hit_count[itr]);
        }
    }
    printf("--------------  ---------\n");
    printf("%-14s  %9d\n", "Total Count:",  landmass_map_square_hit_count[map_idx].total_count);
    printf("%-14s  %9d\n", "Land Squares:", landmass_map_square_hit_count[map_idx].land_squares);

}

/*
Print_Landmass_Map

Prints the _landmasses array for world plane `wp` as a 60 x 40 ASCII grid to
stdout.  Each cell is mapped to a single printable character:

  0         '.'  (no landmass)
  1 – 9     '1' – '9'
  10 – 35   'A' – 'Z'
  36 – 61   'a' – 'z'
  62+        '+'
*/
void Print_Landmass_Map(int16_t map_idx)
{
    int16_t wx    = 0;
    int16_t wy    = 0;
    int16_t wp    = map_idx;
    uint8_t landmass_idx = 0;
    char    ch    = '\0';

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            landmass_idx = Get_Landmass(wx, wy, wp);
            ch = (landmass_idx < (uint8_t)(sizeof(lm_chars) - 1u)) ? lm_chars[landmass_idx] : '+';
            putchar(ch);
        }
        putchar('\n');
    }
}


/*
World Map Statistics
at each stage of the map generation procedure, we want to collect statistics about the map generation process
stage 1: landmass generation
 - count of map squares for each hit count
stage 2: base land terrain conversion
 - count of map squares for each terrain type
stage 3: special land terrain conversion
 - count of map squares for each terrain type
stage 4: final land terrain conversion
 - count of map squares for each terrain type

*/
void Heightmap_Statistics(int map_idx)
{
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t wp = map_idx;
    uint8_t terrain_idx = 0;

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain_idx = Get_Terrain(wx, wy, wp);
            worldmap_map_square_hit_count[map_idx].terrain_type_count[terrain_idx]++;
            worldmap_map_square_hit_count[map_idx].total_count++;
        }
    }

}

void Display_Heightmap_Statistics(int map_idx)
{
    int    itr          = 0;
    int    count        = 0;
    int    total_count  = 0;
    int    land_squares = 0;
    double pct_total    = 0.0;
    double pct_land     = 0.0;

    total_count  = worldmap_map_square_hit_count[map_idx].total_count;
    land_squares = landmass_map_square_hit_count[map_idx].land_squares;

    printf("%12s  %9s  %7s  %7s\n", "Height", "Count", "% Total", "% Land");
    printf("------------  ---------  -------  -------\n");
    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = worldmap_map_square_hit_count[map_idx].terrain_type_count[itr];
        if(count == 0)
            continue;

        pct_total = (total_count > 0) ? ((double)count / (double)total_count * 100.0) : 0.0;

        if(itr == tt_Ocean1)
        {
            printf("%12d  %9d  %6.2f%%  %7s\n", itr, count, pct_total, "---");
        }
        else
        {
            pct_land = (land_squares > 0) ? ((double)count / (double)land_squares * 100.0) : 0.0;
            printf("%12d  %9d  %6.2f%%  %6.2f%%\n", itr, count, pct_total, pct_land);
        }
    }
    printf("------------  ---------  -------  -------\n");
    printf("%-12s  %9d\n", "Total Count:", total_count);

}

void Display_Heightmap_Histogram(int map_idx)
{
    const int bar_width = 40;
    int  itr       = 0;
    int  j         = 0;
    int  count     = 0;
    int  max_count = 0;
    int  bar_len   = 0;
    char ch        = '\0';
    char bar[41];

    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = worldmap_map_square_hit_count[map_idx].terrain_type_count[itr];
        if(count > max_count)
            max_count = count;
    }

    if(max_count == 0)
        return;

    printf("%c  %3s  %6s  %-40s\n", 'Hits', "Idx", "Count", "Bar");
    printf("-  ---  ------  ----------------------------------------\n");

    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = worldmap_map_square_hit_count[map_idx].terrain_type_count[itr];
        if(count == 0)
            continue;

        ch      = (itr < (int)(sizeof(lm_chars) - 1u)) ? lm_chars[itr] : '+';
        bar_len = (count * bar_width) / max_count;

        for(j = 0; j < bar_len; j++)
            bar[j] = '#';
        for(j = bar_len; j < bar_width; j++)
            bar[j] = ' ';
        bar[bar_width] = '\0';

        printf("%c  %3d  %6d  %s\n", ch, itr, count, bar);
    }
}

void Print_Heightmap_Map(int map_idx)
{
    int16_t wx          = 0;
    int16_t wy          = 0;
    int16_t wp          = map_idx;
    int16_t hit_count = 0;
    char    ch          = '\0';

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            // not actually terrain, just hit count
            hit_count = Get_Terrain(wx, wy, wp);
            // just num to printable char for repr
            ch  = lm_chars[hit_count];
            putchar(ch);
        }
        putchar('\n');
    }
}

/*
World Map Statistics
at each stage of the map generation procedure, we want to collect statistics about the map generation process
stage 1: landmass generation
 - count of map squares for each hit count
stage 2: base land terrain conversion
 - count of map squares for each terrain type
stage 3: special land terrain conversion
 - count of map squares for each terrain type
stage 4: final land terrain conversion
 - count of map squares for each terrain type

*/
void Worldmap_Statistics(int map_idx)
{
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t wp = map_idx;
    uint8_t terrain_idx = 0;

    worldmap_map_square_hit_count[map_idx].terrain_type_count[terrain_idx] = 0;
    worldmap_map_square_hit_count[map_idx].total_count = 0;

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain_idx = Get_Terrain(wx, wy, wp);
            worldmap_map_square_hit_count[map_idx].terrain_type_count[terrain_idx]++;
            worldmap_map_square_hit_count[map_idx].total_count++;
        }
    }

}

void Display_Worldmap_Statistics(int map_idx)
{
    static const struct { int type; const char * name; } terrain_names[] =
    {
        { tt_Ocean1,      "Ocean"      },
        { tt_Grasslands1, "Grasslands" },
        { tt_Forest1,     "Forest"     },
        { tt_Hills1,      "Hills"      },
        { tt_Mountain1,   "Mountain"   },
        { tt_Desert1,     "Desert"     },
        { tt_Swamp1,      "Swamp"      },
        { tt_Tundra1,     "Tundra"     },
        { tt_SorceryNode, "SorceryNode"},
        { tt_NatureNode,  "NatureNode" },
        { tt_ChaosNode,   "ChaosNode"  },
    };
    static const int terrain_names_count = (int)(sizeof(terrain_names) / sizeof(terrain_names[0]));

    int          itr          = 0;
    int          n            = 0;
    int          count        = 0;
    int          total_count  = 0;
    int          land_squares = 0;
    double       pct_total    = 0.0;
    double       pct_land     = 0.0;
    const char * name         = NULL;

    total_count  = worldmap_map_square_hit_count[map_idx].total_count;
    land_squares = landmass_map_square_hit_count[map_idx].land_squares;

    printf("%3s  %-12s  %9s  %7s  %7s\n", "Idx", "Name", "Squares", "% Total", "% Land");
    printf("---  ------------  ---------  -------  -------\n");
    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = worldmap_map_square_hit_count[map_idx].terrain_type_count[itr];
        if(count == 0)
            continue;

        name = "";
        for(n = 0; n < terrain_names_count; n++)
        {
            if(terrain_names[n].type == itr)
            {
                name = terrain_names[n].name;
                break;
            }
        }

        pct_total = (total_count  > 0) ? ((double)count / (double)total_count  * 100.0) : 0.0;

        if(itr == tt_Ocean1)
        {
            printf("%3d  %-12s  %9d  %6.2f%%  %7s\n", itr, name, count, pct_total, "---");
        }
        else
        {
            pct_land = (land_squares > 0) ? ((double)count / (double)land_squares * 100.0) : 0.0;
            printf("%3d  %-12s  %9d  %6.2f%%  %6.2f%%\n", itr, name, count, pct_total, pct_land);
        }
    }
    printf("---  ------------  ---------  -------  -------\n");
    printf("%-18s  %9d\n", "Total Count:", total_count);

}

void Display_Worldmap_Histogram(int map_idx)
{
    const int bar_width = 40;
    int  itr       = 0;
    int  j         = 0;
    int  count     = 0;
    int  max_count = 0;
    int  bar_len   = 0;
    char ch        = '\0';
    char bar[41];

    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = worldmap_map_square_hit_count[map_idx].terrain_type_count[itr];
        if(count > max_count)
            max_count = count;
    }

    if(max_count == 0)
        return;

    printf("%c  %3s  %6s  %-40s\n", 'T', "Idx", "Count", "Bar");
    printf("-  ---  ------  ----------------------------------------\n");

    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = worldmap_map_square_hit_count[map_idx].terrain_type_count[itr];
        if(count == 0)
            continue;

        ch      = Terrain_Type_To_Char((int16_t)itr);
        bar_len = (count * bar_width) / max_count;

        for(j = 0; j < bar_len; j++)
            bar[j] = '#';
        for(j = bar_len; j < bar_width; j++)
            bar[j] = ' ';
        bar[bar_width] = '\0';

        printf("%c  %3d  %6d  %s\n", ch, itr, count, bar);
    }
}

void Print_Worldmap_Map(int map_idx)
{
    int16_t wx          = 0;
    int16_t wy          = 0;
    int16_t wp          = map_idx;
    int16_t terrain_idx = 0;
    char    ch          = '\0';

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain_idx = Get_Terrain(wx, wy, wp);
            ch = Terrain_Type_To_Char(terrain_idx);
            putchar(ch);
        }
        putchar('\n');
    }
}

/*
Terrain_Special_To_Char

Maps a terrain special byte to a single printable ASCII character for map display.

Low nibble (bits 3:0) encodes the mineral type; high nibble holds flag bits.
Mineral-only squares are shown in lowercase; mineral + flags are uppercased.
Flag-only squares (no mineral) show the highest-priority flag letter.
*/
static char Terrain_Special_To_Char(uint8_t ts)
{
    static const char mineral_chars[] = ".icsgemaqqx";   /* index 0-9; index 10 unused */
    uint8_t mineral = ts & 0x0Fu;
    uint8_t flags   = ts & 0xF0u;
    char    ch      = '\0';

    ch = (mineral <= 9u) ? mineral_chars[mineral] : '?';

    if(flags != 0u)
    {
        if(ch == '.')
        {
            /* No mineral: show the highest-priority flag */
            if     (flags & TS_NIGHTSHADE)   { ch = 'N'; }
            else if(flags & TS_WILDGAME)     { ch = 'W'; }
            else if(flags & TS_UNKNOWN_20H)  { ch = '?'; }
            else                             { ch = 'H'; }   /* TS_HUNTERSLODGE */
        }
        else
        {
            /* Mineral present with flags: uppercase the mineral letter */
            ch = (char)(ch - 'a' + 'A');
        }
    }

    return ch;
}

/*
Terrain_Specials_Statistics

Scans every square of world plane `wp` and accumulates a frequency count of each
raw terrain-special byte value encountered.  Results are stored in
terrain_specials_map_square_hit_count[map_idx].
*/
void Terrain_Specials_Statistics(int map_idx)
{
    int16_t wx          = 0;
    int16_t wy          = 0;
    int16_t wp          = map_idx;
    uint8_t terrain_special_idx = 0;

    terrain_specials_map_square_hit_count[map_idx].total_count     = 0;
    terrain_specials_map_square_hit_count[map_idx].special_squares = 0;

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain_special_idx = Get_Terrain_Special(wx, wy, wp);
            
            terrain_specials_map_square_hit_count[map_idx].special_value_count[terrain_special_idx]++;
            terrain_specials_map_square_hit_count[map_idx].total_count++;
            if(terrain_special_idx != TS_NONE)
            {
                terrain_specials_map_square_hit_count[map_idx].special_squares++;
            }
        }
    }
}

/*
Display_Terrain_Specials_Statistics

Prints two tables to stdout for world plane `map_idx`:
  1. Minerals  – aggregated counts per mineral type (low nibble of special byte).
  2. Flags     – aggregated counts per flag bit (high nibble of special byte).
*/
void Display_Terrain_Specials_Statistics(int map_idx)
{
    static const struct { uint8_t value; const char * name; } terrain_special_names[] =
    {
        { TS_NONE,         "None"         },
        { TS_IRON,         "Iron"         },
        { TS_COAL,         "Coal"         },
        { TS_SILVER,       "Silver"       },
        { TS_GOLD,         "Gold"         },
        { TS_GEMS,         "Gems"         },
        { TS_MITHRIL,      "Mithril"      },
        { TS_ADAMANTIUM,   "Adamantium"   },
        { TS_QUORK,        "Quork"        },
        { TS_CRYSX,        "Crysx"        },
        { TS_HUNTERSLODGE, "HuntersLodge" },
        { TS_UNKNOWN_20H,  "Unknown_20h"  },
        { TS_WILDGAME,     "WildGame"     },
        { TS_NIGHTSHADE,   "Nightshade"   },
    };
    static const int terrain_special_names_count = (int)(sizeof(terrain_special_names) / sizeof(terrain_special_names[0]));

    int     itr             = 0;
    int     spec_idx        = 0;
    int     special_value_count = 0;
    int     total_count     = 0;
    int     special_squares = 0;
    double  pct_total       = 0.0;
    double  pct_special     = 0.0;

    total_count     = terrain_specials_map_square_hit_count[map_idx].total_count;
    special_squares = terrain_specials_map_square_hit_count[map_idx].special_squares;

    /* Minerals table */
    printf("%-12s  %9s  %7s  %7s\n", "Special", "Squares", "% Total", "% Spcl");
    printf("------------  ---------  -------  -------\n");
    for(itr = 0; itr < terrain_special_names_count; itr++)
    {
        special_value_count = 0;
        for(spec_idx = 0; spec_idx < 256; spec_idx++)
        {
            if(spec_idx == terrain_special_names[itr].value)
            {
                // special_value_count += terrain_specials_map_square_hit_count[map_idx].special_value_count[spec_idx];
                special_value_count = terrain_specials_map_square_hit_count[map_idx].special_value_count[spec_idx];

                pct_total = (total_count > 0) ? ((double)special_value_count / (double)total_count * 100.0) : 0.0;

                if(terrain_special_names[itr].value == TS_NONE)
                {
                    printf("%-12s  %9d  %6.2f%%  %7s\n", terrain_special_names[itr].name, special_value_count, pct_total, "---");
                }
                else
                {
                    pct_special = (special_squares > 0) ? ((double)special_value_count / (double)special_squares * 100.0) : 0.0;
                    printf("%-12s  %9d  %6.2f%%  %6.2f%%\n", terrain_special_names[itr].name, special_value_count, pct_total, pct_special);
                }

            }
        }

    }
    printf("------------  ---------  -------  -------\n");

    printf("%-16s  %9d\n", "Total Count:",    total_count);
    printf("%-16s  %9d\n", "Special Squares:", special_squares);
}

/*
Display_Terrain_Specials_Histogram

Prints a horizontal bar chart to stdout showing the distribution of mineral types
across all squares in world plane `map_idx`.  Ocean-equivalent (TS_NONE) squares
are omitted so the bars for actual specials are visible at a meaningful scale.
*/
void Display_Terrain_Specials_Histogram(int map_idx)
{
    static const char * mineral_labels[] =
    {
        "None", "Iron", "Coal", "Silver", "Gold", "Gems", "Mithril", "Adamantium", "Quork", "Crysx"
    };
    static const char mineral_chars[] = ".icsgemaaqx";   /* parallel to labels, index 0-9 */

    const int bar_width    = 40;
    int  mineral_counts[10];
    int  m         = 0;
    int  j         = 0;
    int  itr       = 0;
    int  count     = 0;
    int  max_count = 0;
    int  bar_len   = 0;
    char bar[41];

    /* Aggregate counts per mineral type (low nibble) */
    for(m = 0; m < 10; m++)
    {
        mineral_counts[m] = 0;
        for(itr = 0; itr < 256; itr++)
        {
            if((itr & 0x0F) == m)
            {
                mineral_counts[m] += terrain_specials_map_square_hit_count[map_idx].special_value_count[itr];
            }
        }
        /* Skip None (m==0) for scale; find max among actual specials */
        if(m > 0 && mineral_counts[m] > max_count)
        {
            max_count = mineral_counts[m];
        }
    }

    if(max_count == 0)
        return;

    printf("%-12s  %c  %6s  %-40s\n", "Mineral", 'T', "Count", "Bar");
    printf("------------  -  ------  ----------------------------------------\n");

    for(m = 1; m < 10; m++)   /* skip None */
    {
        count = mineral_counts[m];
        if(count == 0)
            continue;

        bar_len = (count * bar_width) / max_count;
        for(j = 0; j < bar_len; j++)
            bar[j] = '#';
        for(j = bar_len; j < bar_width; j++)
            bar[j] = ' ';
        bar[bar_width] = '\0';

        printf("%-12s  %c  %6d  %s\n", mineral_labels[m], mineral_chars[m], count, bar);
    }
}

/*
Print_Terrain_Specials_Map

Prints the _map_square_terrain_specials array for world plane `wp` as a 60 x 40
ASCII grid to stdout.  Each cell is mapped to a single printable character via
Terrain_Special_To_Char():

  TS_NONE (0x00)  '.'
  mineral only    lowercase letter  (i c s g e m a q x)
  mineral + flag  uppercase letter
  flag only       N W ? H  (Nightshade, WildGame, Unknown, HuntersLodge)
*/
void Print_Terrain_Specials_Map(int map_idx)
{
    int16_t wx          = 0;
    int16_t wy          = 0;
    int16_t wp          = map_idx;
    uint8_t terrain_special_idx = 0;
    char    ch          = '\0';

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain_special_idx = Get_Terrain_Special(wx, wy, wp);
            ch = Terrain_Special_To_Char(terrain_special_idx);
            putchar(ch);
        }
        putchar('\n');
    }
}

void Simulate_World_Map_Generation(void)
{
    struct s_Landmass_Walk_Stats walk_stats;
    int simulations_to_run = 0;
    int num_maps_to_generate = 0;
    int itr = 0;

    simulations_to_run = 1;
    num_maps_to_generate = (simulations_to_run * 2);

    landmass_map_square_hit_count = Allocate_Space(num_maps_to_generate * sizeof(struct s_Landmass_Map_Square_Hit_Count));
    if(NULL == landmass_map_square_hit_count)
    {
        Exit_With_Message("FATAL: Allocate_Space() failed for landmass_map_square_hit_count");
        return;
    }
    memset(landmass_map_square_hit_count, 0, num_maps_to_generate * sizeof(struct s_Landmass_Map_Square_Hit_Count));

    worldmap_map_square_hit_count = Allocate_Space(num_maps_to_generate * sizeof(struct s_Worldmap_Map_Square_Hit_Count));
    if(NULL == worldmap_map_square_hit_count)
    {
        Exit_With_Message("FATAL: Allocate_Space() failed for worldmap_map_square_hit_count");
        return;
    }
    memset(worldmap_map_square_hit_count, 0, num_maps_to_generate * sizeof(struct s_Worldmap_Map_Square_Hit_Count));

    terrain_specials_map_square_hit_count = Allocate_Space(num_maps_to_generate * sizeof(struct s_Terrain_Specials_Map_Square_Hit_Count));
    if(NULL == terrain_specials_map_square_hit_count)
    {
        Exit_With_Message("FATAL: Allocate_Space() failed for terrain_specials_map_square_hit_count");
        return;
    }
    memset(terrain_specials_map_square_hit_count, 0, num_maps_to_generate * sizeof(struct s_Terrain_Specials_Map_Square_Hit_Count));

    _landmasses = Allocate_Space(
        num_maps_to_generate
        *
        (
            ((WORLD_WIDTH * WORLD_HEIGHT) * 1)
            / SZ_PARAGRAPH_B
        )
    );
    if(NULL == _landmasses)
    {
        Exit_With_Message("FATAL: malloc() failed for _landmasses");
        return;
    }

    _world_maps = Allocate_Space(
        num_maps_to_generate
        *
        (
            ((WORLD_WIDTH * WORLD_HEIGHT) * 2)
            / SZ_PARAGRAPH_B
        )
    );
    if(NULL == _world_maps)
    {
        Exit_With_Message("FATAL: malloc() failed for _world_maps");
        return;
    }

    _map_square_terrain_specials = Allocate_Space(
        num_maps_to_generate
        *
        ((WORLD_WIDTH * WORLD_HEIGHT) * 1)
        / SZ_PARAGRAPH_B
    );  // 150 PR, 2400 B  (1 byte per square, 1 plane)
    if(NULL == _map_square_terrain_specials)
    {
        Exit_With_Message("FATAL: Allocate_Space() failed for _map_square_terrain_specials");
        return;
    }

    _map_square_flags = Allocate_Space(
        num_maps_to_generate
        *
        ((WORLD_WIDTH * WORLD_HEIGHT) * 1)
        / SZ_PARAGRAPH_B
    );  // 150 PR, 2400 B  (1 byte per square, 1 plane)
    if(NULL == _map_square_flags)
    {
        Exit_With_Message("FATAL: Allocate_Space() failed for _map_square_flags");
        return;
    }

    // transient? only used per-simulation? should be recleared prior?
    _UNITS = (struct s_UNIT *)Allocate_Space(2028);
    memset(_UNITS, 0, (2028 * SZ_PARAGRAPH_B));
    _CITIES     = (struct s_CITY     *)Allocate_Space( ( ( ( NUM_CITIES     * sizeof(struct s_CITY     ) ) / SZ_PARAGRAPH_B ) + 1 ) );
    memset(_CITIES, 0,                                       NUM_CITIES     * sizeof(struct s_CITY     )                            );
    _NODES      = (struct s_NODE     *)Allocate_Space( ( ( ( NUM_NODES      * sizeof(struct s_NODE     ) ) / SZ_PARAGRAPH_B ) + 1 ) );
    memset(_NODES, 0,                                        NUM_NODES      * sizeof(struct s_NODE     )                            );
    _FORTRESSES = (struct s_FORTRESS *)Allocate_Space( ( ( ( NUM_FORTRESSES * sizeof(struct s_FORTRESS ) ) / SZ_PARAGRAPH_B ) + 1 ) );
    memset(_FORTRESSES, 0,                                   NUM_FORTRESSES * sizeof(struct s_FORTRESS )                            );
    _TOWERS     = (struct s_TOWER    *)Allocate_Space( ( ( ( NUM_TOWERS     * sizeof(struct s_TOWER    ) ) / SZ_PARAGRAPH_B ) + 1 ) );
    memset(_TOWERS, 0,                                       NUM_TOWERS     * sizeof(struct s_TOWER    )                            );
    _LAIRS      = (struct s_LAIR     *)Allocate_Space( ( ( ( NUM_LAIRS      * sizeof(struct s_LAIR     ) ) / SZ_PARAGRAPH_B ) + 1 ) );
    memset(_LAIRS, 0,                                        NUM_LAIRS      * sizeof(struct s_LAIR     )                            );
    
    // needed by Generate_Neutral_Cities__WIP()
    // MGC dseg:3406 42 55 49 4C 44 44 41 54 00                      builddat_lbx_file__MGC_ovr051 db 'BUILDDAT',0
    // Init_New_Game()
    // bldg_data_table = (struct s_BLDG_DATA *)LBX_Load_Data(builddat_lbx_file__MGC_ovr051, 0, 0, 36, 52);
    // WZD dseg:2A12
    char builddat_lbx_file[] = "BUILDDAT.LBX";
    // WZD o52p17
    // void Load_BUILDDAT(void)
    // MOM_Data  WZD dseg:938C
    // MOM_Data  struct s_BLDG_DATA * bldg_data_table;
    bldg_data_table = (struct s_BLDG_DATA *)LBX_Load_Data(builddat_lbx_file, 0, 0, 36, 52);
    assert(bldg_data_table != NULL);
    assert(bldg_data_table[bt_NONE].name != NULL);
    assert(bldg_data_table[bt_NONE].maintenance_cost == 0);
    


    // ¿ DO NOT NEED  Init_Computer_Players() ?



    for(itr = 0; itr < simulations_to_run; itr++)
    {

        memset(_UNITS, 0, (2028 * SZ_PARAGRAPH_B));
        memset(_CITIES, 0,                                       NUM_CITIES     * sizeof(struct s_CITY     )                            );
        memset(_NODES, 0,                                        NUM_NODES      * sizeof(struct s_NODE     )                            );
        memset(_FORTRESSES, 0,                                   NUM_FORTRESSES * sizeof(struct s_FORTRESS )                            );
        memset(_TOWERS, 0,                                       NUM_TOWERS     * sizeof(struct s_TOWER    )                            );
        memset(_LAIRS, 0,                                        NUM_LAIRS      * sizeof(struct s_LAIR     )                            );

        Init_Landmasses(itr + ARCANUS_PLANE);

        Init_Landmasses(itr + MYRROR_PLANE);

        Generate_Landmasses((itr + ARCANUS_PLANE));  // post: Landmass Index, Square Count

        Generate_Landmasses((itr + MYRROR_PLANE));  // post: Landmass Index, Square Count

        // Landmass_Statistics((itr + ARCANUS_PLANE));
        // Display_Landmass_Statistics((itr + ARCANUS_PLANE));
        // // Display_Landmass_Histogram((itr + ARCANUS_PLANE));
        // Print_Landmass_Map((itr + ARCANUS_PLANE));

        // Heightmap_Statistics((itr + ARCANUS_PLANE));
        // Display_Heightmap_Statistics((itr + ARCANUS_PLANE));
        // // no useful information  Display_Heightmap_Histogram((itr + ARCANUS_PLANE));
        // Print_Heightmap_Map((itr + ARCANUS_PLANE));

        Translate_Heightmap_To_Base_Terrain_Types((itr + ARCANUS_PLANE));

        Worldmap_Statistics((itr + ARCANUS_PLANE));
        Display_Worldmap_Statistics((itr + ARCANUS_PLANE));
        Display_Worldmap_Histogram((itr + ARCANUS_PLANE));
        Print_Worldmap_Map((itr + ARCANUS_PLANE));

        Generate_Climate_Terrain_Types((itr + ARCANUS_PLANE));

        Worldmap_Statistics((itr + ARCANUS_PLANE));
        Display_Worldmap_Statistics((itr + ARCANUS_PLANE));
        Display_Worldmap_Histogram((itr + ARCANUS_PLANE));
        Print_Worldmap_Map((itr + ARCANUS_PLANE));

        // Generate_Nodes();
        // NEWG_EqualizeNodes__WIP(MYRROR_PLANE);
        // Generate_Towers();
        // NEWG_TileIsleExtend__WIP(ARCANUS_PLANE);
        // Generate_Lairs();
        // DONT  Generate_Home_City__WIP();
        // Generate_Neutral_Cities__WIP(MYRROR_PLANE);



        Generate_Nodes();
        Generate_Towers();
        Generate_Lairs();
        _units = 0;
        _num_players = 1;
        NEWG_Clicked_Race = rt_High_Men;
        Generate_Home_City__WIP();
        Generate_Neutral_Cities__WIP((itr + ARCANUS_PLANE));
        Generate_Neutral_Cities__WIP((itr + MYRROR_PLANE));



        Generate_Terrain_Specials((itr + ARCANUS_PLANE));

        Generate_Terrain_Specials((itr + MYRROR_PLANE));
        
        Worldmap_Statistics((itr + ARCANUS_PLANE));
        Display_Worldmap_Statistics((itr + ARCANUS_PLANE));
        Display_Worldmap_Histogram((itr + ARCANUS_PLANE));
        Print_Worldmap_Map((itr + ARCANUS_PLANE));
        
        Worldmap_Statistics((itr + MYRROR_PLANE));
        Display_Worldmap_Statistics((itr + MYRROR_PLANE));
        Display_Worldmap_Histogram((itr + MYRROR_PLANE));
        Print_Worldmap_Map((itr + MYRROR_PLANE));

        Terrain_Specials_Statistics((itr + ARCANUS_PLANE));
        Display_Terrain_Specials_Statistics((itr + ARCANUS_PLANE));
        // Display_Terrain_Specials_Histogram((itr + ARCANUS_PLANE));
        // Print_Terrain_Specials_Map((itr + ARCANUS_PLANE));

        Terrain_Specials_Statistics((itr + MYRROR_PLANE));
        Display_Terrain_Specials_Statistics((itr + MYRROR_PLANE));
        // Display_Terrain_Specials_Histogram((itr + MYRROR_PLANE));
        // Print_Terrain_Specials_Map((itr + MYRROR_PLANE));

        // Generate_Roads(ARCANUS_PLANE);
        // NEWG_CreateShores__STUB();
        // for(IDK1 = 0; IDK1 < 10; IDK1++)
        // {
        //     for(IDK2 = 0; ((IDK2 < 2000) && (NEWG_CreateRiver__STUB(0) != 0)); IDK2++) { }
        //     for(IDK2 = 0; ((IDK2 < 2000) && (NEWG_CreateRiver__STUB(1) != 0)); IDK2++) { }
        // }
        // NEWG_SetRiverTiles__STUB(ARCANUS_PLANE);
        // NEWG_SetDeserts__STUB();
        // NEWG_RandomizeTiles__STUB();
        // Movement_Mode_Cost_Maps(ARCANUS_PLANE);
        // CRP_NEWG_CreatePathGrids__WIP(MYRROR_PLANE);
        // Init_Square_Explored();
        // NEWG_AnimateOceans__STUB();
        // Set_Upper_Lair_Guardian_Count();

    }

    /*
        display statistics for entire simulation run
         - average probability of each terrain type
         - histogram
    */

}
