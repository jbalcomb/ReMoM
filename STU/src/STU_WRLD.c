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
    Generate_Home_City()
        _FORTRESSES[]
        _NODES[]
        _TOWERS[]
        _LAIRS[]
        _CITIES[]
        ...Random_City_Name_By_Race_NewGame()
        _cities
        LBX_Load_Data_Static(cityname_lbx_file__MGC_ovr051, 0, (SAMB_ptr)city_names_buffer, race_idx, 1, 280);
        
    Generate_Neutral_Cities()
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

#ifdef STU_DEBUG
#include "STU_DBG.h"
#endif

#include "../../MoM/src/MAPGEN.h"       /* Square_Has_Tower_NewGame() */
#include "../../MoM/src/NewGame.h"      /* NEWG_Clicked_Race */
#include "../../MoM/src/RACETYPE.h"     /* rt_High_Men */
#include "../../MoM/src/Terrain.h"      /* TS_NONE, TS_IRON, TS_WILDGAME, ... */
#include "../../MoM/src/TerrType.h"     /* tt_Ocean1 */

#include "../../MoX/src/Allocate.h"     /* Allocate_Space() */
#include "../../MoX/src/EXIT.h"         /* Exit_With_Message() */
#include "../../MoX/src/LBX_Load.h"     /* LBX_Load_Data() */
#include "../../MoX/src/MOX_BITS.h"     /* GET_2B_OFS() */
#include "../../MoX/src/MOM_DAT.h"     /* _landmasses, _world_maps */
#include "../../MoX/src/MOM_DEF.h"      /* WORLD_WIDTH, WORLD_HEIGHT */
#include "../../MoX/src/MOX_DEF.h"      /* GET_TERRAIN_TYPE() */
#include "../../MoX/src/MOX_SET.h"      /* magic_set */

#include "STU_WRLD.h"

#include <assert.h>  /* assert() */
#include <stdio.h>   /* FILE, fopen, ftell, fprintf, fclose */
#include <stdlib.h>  /* malloc, free */
#include <memory.h>  /* memset() === vcruntime_string.h */
#include <string.h>  /* memset() */
// Routine  malloc; Required header  <stdlib.h> and <malloc.h>;


// MGC  dseg:9040
extern int16_t m_landmasses_ctr;



int32_t trial_run_count;
struct s_Simulation_Data * simulation_data;

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

static uint8_t Get_Landmass(int16_t wx, int16_t wy, int16_t wp)         { return _landmasses[ (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx ];                  }
static int16_t Get_Terrain(int16_t wx, int16_t wy, int16_t wp)          { return p_world_map[wp][wy][wx];                                                     }
static uint8_t Get_Terrain_Special(int16_t wx, int16_t wy, int16_t wp)  { return _map_square_terrain_specials[ (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx ]; }

static const struct { int type; const char * name; } terrain_type_names[] =
{
    { tt_Ocean,      "Ocean"      },
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
static const int terrain_type_names_count = (int)(sizeof(terrain_type_names) / sizeof(terrain_type_names[0]));

static const char * Terrain_Type_Name(int type)
{
    int n = 0;
    for(n = 0; n < terrain_type_names_count; n++)
    {
        if(terrain_type_names[n].type == type)
            return terrain_type_names[n].name;
    }
    return "";
}

static const char * terrain_group_names[] =
{
    "Ocean",       /* 0 */
    "Grasslands",  /* 1 */
    "Forest",      /* 2 */
    "Hills",       /* 3 */
    "Mountain",    /* 4 */
    "Desert",      /* 5 */
    "Swamp",       /* 6 */
    "Tundra",      /* 7 */
    "River",       /* 8 */
    "Shore",       /* 9 */
};
static const int terrain_group_names_count = (int)(sizeof(terrain_group_names) / sizeof(terrain_group_names[0]));

static const char * Terrain_Group_Name(int group)
{
    if(group >= 0 && group < terrain_group_names_count)
        return terrain_group_names[group];
    return "";
}

static char Terrain_Type_To_Char(int16_t tt)
{
    if(tt == tt_Ocean)         return '.';
    if(tt == tt_BugGrass)       return 'B';
    if(tt == tt_Lake)           return 'l';                        /* 0x12 lake within Shore1 range */
    if(tt <= tt_Shore1_Lst)     return '`';                        /* tt_Shore1_Fst .. tt_Shore1_Lst */
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
    if(tt <= tt_RiverM_end)     return '=';                        /* tt_RiverM_1st .. tt_RiverM_end */
    if(tt <= tt_Lake4)          return 'l';                        /* tt_Lake1 .. tt_Lake4 */
    if(tt <= tt_Shore2F_end)    return '`';                        /* tt_Shore2F_1st .. tt_Shore2F_end */
    if(tt <= tt_Rivers_end)     return '=';                        /* tt_Rivers_1st .. tt_Rivers_end */
    if(tt <= tt_Mountains_Lst)  return 'm';                        /* tt_Mountains_Fst .. tt_Mountains_Lst */
    if(tt <= tt_Hills_Lst)      return 'h';                        /* tt_Hills_Fst .. tt_Hills_Lst */
    if(tt <= tt_Desert_Lst)     return 'd';                        /* tt_Desert_Fst .. tt_Desert_Lst */
    if(tt <= tt_Shore2_end)     return '`';                        /* tt_Shore2_1st .. tt_Shore2_end */
    if(tt <= tt_4WRiver5)       return '=';                        /* tt_4WRiver1 .. tt_4WRiver5 */
    if(tt <= tt_Shore3_end)     return '`';                        /* tt_Shore3_1st .. tt_Shore3_end */
    if(tt == tt_OceanAnim)      return '~';
    if(tt <= tt_Tundra_Last)    return 't';                        /* tt_Tundra_1st .. tt_Tundra_Last */
    return '?';
}

/* { 0:Oceans, 1:Grasslands, 2:Forests, 3:Hills, 4:Mountains, 5:Deserts, 6:Swamps, 7:Tundras, 8:River, 9:Shore } */
static int16_t Get_Terrain_Group(int16_t tt)
{
    if(tt == tt_Ocean)         return 0;
    if(tt == tt_BugGrass)       return 1;
    if(tt == tt_Lake)           return 9;                       /* 0x12 lake within Shore1 range */
    if(tt <= tt_Shore1_Lst)     return 9;                       /* tt_Shore1_Fst .. tt_Shore1_Lst */
    if(tt == tt_Grasslands1)    return 1;
    if(tt == tt_Forest1)        return 2;
    if(tt == tt_Mountain1)      return 4;
    if(tt == tt_Desert1)        return 5;
    if(tt == tt_Swamp1)         return 6;
    if(tt == tt_Tundra1)        return 7;
    if(tt == tt_SorceryNode)    return 1;
    if(tt == tt_NatureNode)     return 1;
    if(tt == tt_ChaosNode)      return 1;
    if(tt == tt_Hills1)         return 3;
    if(tt <= tt_Grasslands3)    return 1;                       /* tt_Grasslands2 .. tt_Grasslands3 */
    if(tt <= tt_Desert4)        return 5;                       /* tt_Desert2 .. tt_Desert4 */
    if(tt <= tt_Swamp3)         return 6;                       /* tt_Swamp2 .. tt_Swamp3 */
    if(tt == tt_Volcano)        return 4;
    if(tt == tt_Grasslands4)    return 1;
    if(tt <= tt_Tundra3)        return 7;                       /* tt_Tundra2 .. tt_Tundra3 */
    if(tt <= tt_Forest3)        return 2;                       /* tt_Forest2 .. tt_Forest3 */
    if(tt <= tt_RiverM_end)     return 8;                       /* tt_RiverM_1st .. tt_RiverM_end */
    if(tt <= tt_Lake4)          return 9;                       /* tt_Lake1 .. tt_Lake4 */
    if(tt <= tt_Shore2F_end)    return 9;                       /* tt_Shore2F_1st .. tt_Shore2F_end */
    if(tt <= tt_Rivers_end)     return 8;                       /* tt_Rivers_1st .. tt_Rivers_end */
    if(tt <= tt_Mountains_Lst)  return 4;                       /* tt_Mountains_Fst .. tt_Mountains_Lst */
    if(tt <= tt_Hills_Lst)      return 3;                       /* tt_Hills_Fst .. tt_Hills_Lst */
    if(tt <= tt_Desert_Lst)     return 5;                       /* tt_Desert_Fst .. tt_Desert_Lst */
    if(tt <= tt_Shore2_end)     return 9;                       /* tt_Shore2_1st .. tt_Shore2_end */
    if(tt <= tt_4WRiver5)       return 8;                       /* tt_4WRiver1 .. tt_4WRiver5 */
    if(tt <= tt_Shore3_end)     return 9;                       /* tt_Shore3_1st .. tt_Shore3_end */
    if(tt == tt_OceanAnim)         return 0;                       /* Animated Ocean */
    if(tt <= tt_Tundra_Last)    return 7;                       /* tt_Tundra_1st .. tt_Tundra_Last */
    return -1;
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
per run of the world map generation procedure

count of values per landmass index
count of landmass indices
NOTE: average probability of a given landmass index is calculated after the simulation is complete

given an unknown number of landmass index values, we need a way to dynamicallly track the count of each landmass index value
this is done by maintaining a list of landmass index values and their counts, and updating the counts as we go through the simulation
when we encounter a landmass index value, we check if it is already in the list, if it is, we increment its count, if it is not, we add it to the list with a count of 1

*/
static void Landmass_Statistics(int sim_idx, int16_t wp)
{
    int16_t wx = 0;
    int16_t wy = 0;
    uint8_t hit_count = 0;

    memset(&simulation_data[sim_idx].landmass_stats[wp], 0, sizeof(struct s_Landmass_Squares_Stats));

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            hit_count = Get_Landmass(wx, wy, wp);
            simulation_data[sim_idx].landmass_stats[wp].hit_count[hit_count]++;
            simulation_data[sim_idx].landmass_stats[wp].total_count++;
            if(hit_count > simulation_data[sim_idx].landmass_stats[wp].max_hit_count_index)
            {
                simulation_data[sim_idx].landmass_stats[wp].max_hit_count_index = hit_count;
            }
        }
    }

    simulation_data[sim_idx].landmass_stats[wp].land_squares =
        simulation_data[sim_idx].landmass_stats[wp].total_count
        - simulation_data[sim_idx].landmass_stats[wp].hit_count[0];

}

static void Display_Landmass_Statistics(int sim_idx, int16_t wp)
{
    int itr = 0;

    printf("%14s  %9s\n", "Landmass Index", "Squares");
    printf("--------------  ---------\n");
    for(itr = 0; itr < LANDMASS_INDEX_MAX; itr++)
    {
        if(itr <= simulation_data[sim_idx].landmass_stats[wp].max_hit_count_index)
        {
            printf("%14d  %9d\n", itr, simulation_data[sim_idx].landmass_stats[wp].hit_count[itr]);
        }
    }
    printf("--------------  ---------\n");
    printf("%-14s  %9d\n", "Total Count:",  simulation_data[sim_idx].landmass_stats[wp].total_count);
    printf("%-14s  %9d\n", "Land Squares:", simulation_data[sim_idx].landmass_stats[wp].land_squares);

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
static void Print_Landmass_Map(int16_t map_idx)
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
static void Heightmap_Statistics(int sim_idx, int16_t wp)
{
    int16_t wx = 0;
    int16_t wy = 0;
    uint8_t terrain_idx = 0;

    memset(&simulation_data[sim_idx].worldmap_stats[wp], 0, sizeof(struct s_Worldmap_Squares_Stats));

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain_idx = (uint8_t)Get_Terrain(wx, wy, wp);
            simulation_data[sim_idx].worldmap_stats[wp].terrain_type_count[terrain_idx]++;
            simulation_data[sim_idx].worldmap_stats[wp].total_count++;
        }
    }

}

static void Display_Heightmap_Statistics(int sim_idx, int16_t wp)
{
    int    itr          = 0;
    int    count        = 0;
    int    total_count  = 0;
    int    land_squares = 0;
    double pct_total    = 0.0;
    double pct_land     = 0.0;

    total_count  = simulation_data[sim_idx].worldmap_stats[wp].total_count;
    land_squares = simulation_data[sim_idx].landmass_stats[wp].land_squares;

    printf("%12s  %9s  %7s  %7s\n", "Height", "Count", "% Total", "% Land");
    printf("------------  ---------  -------  -------\n");
    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = simulation_data[sim_idx].worldmap_stats[wp].terrain_type_count[itr];
        if(count == 0)
            continue;

        pct_total = (total_count > 0) ? ((double)count / (double)total_count * 100.0) : 0.0;

        if(itr == tt_Ocean)
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
    printf("%-12s  %9d\n", "Land Squares:", land_squares);

}

static void Display_Heightmap_Histogram(int sim_idx, int16_t wp)
{
    const int bar_width = 40;
    int  itr       = 0;
    int  j         = 0;
    int  count     = 0;
    int  max_count = 0;
    int  bar_len   = 0;
    char ch        = '\0';
    char bar[41] = { 0 };

    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = simulation_data[sim_idx].worldmap_stats[wp].terrain_type_count[itr];
        if(count > max_count)
            max_count = count;
    }

    if(max_count == 0)
        return;

    printf("%s  %3s  %6s  %-40s\n", "Hits", "Idx", "Count", "Bar");
    printf("-  ---  ------  ----------------------------------------\n");

    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = simulation_data[sim_idx].worldmap_stats[wp].terrain_type_count[itr];
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

static void Print_Heightmap_Map(int sim_idx, int16_t wp)
{
    int16_t wx          = 0;
    int16_t wy          = 0;
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

static void Worldmap_Statistics(int sim_idx, int16_t wp)
{
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t terrain_idx = 0;

    memset(&simulation_data[sim_idx].worldmap_stats[wp], 0, sizeof(struct s_Worldmap_Squares_Stats));
    simulation_data[sim_idx].tower_stats[wp].tower_count = 0;

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            if(Square_Has_Tower_NewGame(wx, wy) == ST_TRUE)
            {
                simulation_data[sim_idx].tower_stats[wp].tower_count++;
            }

            terrain_idx = Get_Terrain(wx, wy, wp);

            simulation_data[sim_idx].worldmap_stats[wp].terrain_type_count[terrain_idx]++;
            simulation_data[sim_idx].worldmap_stats[wp].total_count++;
        }
    }

}

static void Display_Worldmap_Statistics(int sim_idx, int16_t wp)
{
    int          itr          = 0;
    int          count        = 0;
    int          total_count  = 0;
    int          land_squares = 0;
    double       pct_total    = 0.0;
    double       pct_land     = 0.0;
    const char * name         = NULL;
    const char * group        = NULL;
    char         ch           = '\0';

    total_count  = simulation_data[sim_idx].worldmap_stats[wp].total_count;
    land_squares = simulation_data[sim_idx].landmass_stats[wp].land_squares;

    printf("%c  %3s  %-12s  %-12s  %9s  %7s  %7s\n", 'T', "Idx", "Name", "Group", "Squares", "% Total", "% Land");
    printf("-  ---  ------------  ------------  ---------  -------  -------\n");
    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = simulation_data[sim_idx].worldmap_stats[wp].terrain_type_count[itr];
        if(count == 0)
            continue;

        name  = Terrain_Type_Name(itr);
        group = Terrain_Group_Name(Get_Terrain_Group(itr));
        ch    = Terrain_Type_To_Char((int16_t)itr);

        pct_total = (total_count  > 0) ? ((double)count / (double)total_count  * 100.0) : 0.0;

        if(itr == tt_Ocean)
        {
            printf("%c  %3d  %-12s  %-12s  %9d  %6.2f%%  %7s\n", ch, itr, name, group, count, pct_total, "---");
        }
        else
        {
            pct_land = (land_squares > 0) ? ((double)count / (double)land_squares * 100.0) : 0.0;
            printf("%c  %3d  %-12s  %-12s  %9d  %6.2f%%  %6.2f%%\n", ch, itr, name, group, count, pct_total, pct_land);
        }
    }
    printf("---  ------------  ------------  ---------  -------  -------\n");
    printf("%-18s  %9d\n", "Total Count:", total_count);
    printf("%-18s  %9d\n", "Land Squares:", land_squares);

}

static void Display_Worldmap_Histogram(int sim_idx, int16_t wp)
{
    const int bar_width = 40;
    int  itr       = 0;
    int  j         = 0;
    int  count     = 0;
    int  max_count = 0;
    int  bar_len   = 0;
    char ch        = '\0';
    char bar[41] = { 0 };

    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = simulation_data[sim_idx].worldmap_stats[wp].terrain_type_count[itr];
        if(count > max_count)
            max_count = count;
    }

    if(max_count == 0)
        return;

    printf("%c  %3s  %6s  %-40s\n", 'T', "Idx", "Count", "Bar");
    printf("-  ---  ------  ----------------------------------------\n");

    for(itr = 0; itr < TerType_Count; itr++)
    {
        count = simulation_data[sim_idx].worldmap_stats[wp].terrain_type_count[itr];
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

static void Print_Worldmap_Map(int sim_idx, int16_t wp)
{
    int16_t wx          = 0;
    int16_t wy          = 0;
    int16_t terrain_idx = 0;
    char    ch          = '\0';

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain_idx = Get_Terrain(wx, wy, wp);
            ch = Terrain_Type_To_Char(terrain_idx);
            if(Square_Has_Tower_NewGame(wx, wy) == ST_TRUE)
            {
                ch = 'T';
            }
            putchar(ch);
        }
        putchar('\n');
    }
}

/*
Terrain_Specials_Statistics

Scans every square of world plane `wp` and accumulates a frequency count of each
raw terrain-special byte value encountered.  Results are stored in
simulation_data[sim_idx].specials_stats[wp].
*/
static void Terrain_Specials_Statistics(int sim_idx, int wp)
{
    int16_t wx          = 0;
    int16_t wy          = 0;
    uint8_t terrain_special_idx = 0;

    simulation_data[sim_idx].specials_stats[wp].total_count     = 0;
    simulation_data[sim_idx].specials_stats[wp].special_squares = 0;

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain_special_idx = Get_Terrain_Special(wx, wy, wp);

            simulation_data[sim_idx].specials_stats[wp].terrain_special_square_count[terrain_special_idx]++;
            simulation_data[sim_idx].specials_stats[wp].total_count++;
            if(terrain_special_idx != TS_NONE)
            {
                simulation_data[sim_idx].specials_stats[wp].special_squares++;
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
static void Display_Terrain_Specials_Statistics(int sim_idx, int16_t wp)
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
    int     terrain_special_square_count = 0;
    int     total_count     = 0;
    int     special_squares = 0;
    double  pct_total       = 0.0;
    double  pct_special     = 0.0;

    total_count     = simulation_data[sim_idx].specials_stats[wp].total_count;
    special_squares = simulation_data[sim_idx].specials_stats[wp].special_squares;

    /* Minerals table */
    printf("%-12s  %9s  %7s  %7s\n", "Special", "Squares", "% Total", "% Spcl");
    printf("------------  ---------  -------  -------\n");
    for(itr = 0; itr < terrain_special_names_count; itr++)
    {
        terrain_special_square_count = 0;
        for(spec_idx = 0; spec_idx < 256; spec_idx++)
        {
            if(spec_idx == terrain_special_names[itr].value)
            {
                terrain_special_square_count = simulation_data[sim_idx].specials_stats[wp].terrain_special_square_count[spec_idx];

                pct_total = (total_count > 0) ? ((double)terrain_special_square_count / (double)total_count * 100.0) : 0.0;

                if(terrain_special_names[itr].value == TS_NONE)
                {
                    printf("%-12s  %9d  %6.2f%%  %7s\n", terrain_special_names[itr].name, terrain_special_square_count, pct_total, "---");
                }
                else
                {
                    pct_special = (special_squares > 0) ? ((double)terrain_special_square_count / (double)special_squares * 100.0) : 0.0;
                    printf("%-12s  %9d  %6.2f%%  %6.2f%%\n", terrain_special_names[itr].name, terrain_special_square_count, pct_total, pct_special);
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
static void Display_Terrain_Specials_Histogram(int sim_idx, int16_t wp)
{
    static const char * mineral_labels[] =
    {
        "None", "Iron", "Coal", "Silver", "Gold", "Gems", "Mithril", "Adamantium", "Quork", "Crysx"
    };
    static const char mineral_chars[] = ".icsgemaaqx";   /* parallel to labels, index 0-9 */

    const int bar_width = 40;
    int  mineral_counts[10] = { 0 };
    int  m         = 0;
    int  j         = 0;
    int  itr       = 0;
    int  count     = 0;
    int  max_count = 0;
    int  bar_len   = 0;
    char bar[41] = { 0 };

    /* Aggregate counts per mineral type (low nibble) */
    for(m = 0; m < 10; m++)
    {
        mineral_counts[m] = 0;
        for(itr = 0; itr < 256; itr++)
        {
            if((itr & 0x0F) == m)
            {
                mineral_counts[m] += simulation_data[sim_idx].specials_stats[wp].terrain_special_square_count[itr];
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
Evaluate_Terrain_Special_Distribution

Evaluates the distribution of terrain specials across the terrain types that
can bear specials: Desert, Hills, Mountain, Forest, and Swamp.

For each terrain category, counts the total number of squares and the number
of those squares that have a non-zero terrain special.  Prints a table
comparing the actual special frequency against the expected rate:
  Arcanus: 1:17  (5.88%)
  Myrror:  1:10  (10.00%)


*/
static void Evaluate_Terrain_Special_Distribution(int sim_idx, int16_t wp)
{
    enum { CAT_DESERT, CAT_HILLS, CAT_MOUNTAIN, CAT_FOREST, CAT_SWAMP, CAT_COUNT };
    static const char * cat_names[CAT_COUNT] =
    {
        "Desert", "Hills", "Mountain", "Forest", "Swamp"
    };

    int      cat_total[CAT_COUNT];
    int      cat_special[CAT_COUNT];
    int      all_total    = 0;
    int      all_special  = 0;
    int      cat          = -1;
    int16_t  wx           = 0;
    int16_t  wy           = 0;
    int16_t  terrain      = 0;
    uint8_t  ts           = 0;
    char     tt_ch        = '\0';
    int      i            = 0;
    double   expected_pct = 0.0;
    double   actual_pct   = 0.0;
    int      expected_div = 0;
    const char * plane_name = NULL;

    if(wp == MYRROR_PLANE)
    {
        expected_pct = 10.0;
        expected_div = 10;
        plane_name   = "Myrror";
    }
    else
    {
        expected_pct = 100.0 / 17.0;
        expected_div = 17;
        plane_name   = "Arcanus";
    }

    memset(cat_total,   0, sizeof(cat_total));
    memset(cat_special, 0, sizeof(cat_special));

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain = Get_Terrain(wx, wy, wp);
            tt_ch   = Terrain_Type_To_Char(terrain);

            switch(tt_ch)
            {
                case 'd': cat = CAT_DESERT;   break;
                case 'h': cat = CAT_HILLS;    break;
                case 'm': cat = CAT_MOUNTAIN; break;
                case 'f': cat = CAT_FOREST;   break;
                case 's': cat = CAT_SWAMP;    break;
                default:  cat = -1;            break;
            }

            if(cat < 0)
            {
                continue;
            }

            cat_total[cat]++;
            all_total++;

            ts = Get_Terrain_Special(wx, wy, wp);
            if(ts != TS_NONE)
            {
                cat_special[cat]++;
                all_special++;
            }
        }
    }

    printf("\nTerrain Special Distribution (%s)  Expected: 1:%d (%.2f%%)\n",
        plane_name, expected_div, expected_pct);
    printf("%-10s  %9s  %8s  %7s  %8s  %8s\n",
        "Terrain", "Squares", "Specials", "Ratio", "% Actual", "% Expect");
    printf("----------  ---------  --------  -------  --------  --------\n");

    for(i = 0; i < CAT_COUNT; i++)
    {
        if(cat_total[i] == 0)
        {
            printf("%-10s  %9d  %8d  %7s  %8s  %7.2f%%\n",
                cat_names[i], 0, 0, "---", "---", expected_pct);
            continue;
        }

        actual_pct = (double)cat_special[i] / (double)cat_total[i] * 100.0;

        if(cat_special[i] > 0)
        {
            printf("%-10s  %9d  %8d   1:%-4.1f  %7.2f%%  %7.2f%%\n",
                cat_names[i],
                cat_total[i],
                cat_special[i],
                (double)cat_total[i] / (double)cat_special[i],
                actual_pct,
                expected_pct);
        }
        else
        {
            printf("%-10s  %9d  %8d  %7s  %7.2f%%  %7.2f%%\n",
                cat_names[i], cat_total[i], 0, "---", actual_pct, expected_pct);
        }
    }

    printf("----------  ---------  --------  -------  --------  --------\n");

    if(all_total > 0 && all_special > 0)
    {
        actual_pct = (double)all_special / (double)all_total * 100.0;
        printf("%-10s  %9d  %8d   1:%-4.1f  %7.2f%%  %7.2f%%\n",
            "Total", all_total, all_special,
            (double)all_total / (double)all_special,
            actual_pct, expected_pct);
    }
    else
    {
        actual_pct = 0.0;
        printf("%-10s  %9d  %8d  %7s  %7.2f%%  %7.2f%%\n",
            "Total", all_total, all_special, "---", actual_pct, expected_pct);
    }
}

/*
Evaluate_Desert_Special_Distribution

Evaluates which mineral types appear on Desert squares and their relative
frequency among all desert specials.

Expected distribution of specials among desert squares that received one:
  Arcanus: 66.7% Gems, 33.3% Quork
  Myrror:  20.0% Gems, 40.0% Quork, 20.0% Crysx
*/
static void Evaluate_Desert_Special_Distribution(int sim_idx, int16_t wp)
{
    static const struct { uint8_t mineral; const char * name; } mineral_names[] =
    {
        { TS_IRON,       "Iron"       },
        { TS_COAL,       "Coal"       },
        { TS_SILVER,     "Silver"     },
        { TS_GOLD,       "Gold"       },
        { TS_GEMS,       "Gems"       },
        { TS_MITHRIL,    "Mithril"    },
        { TS_ADAMANTIUM, "Adamantium" },
        { TS_QUORK,      "Quork"      },
        { TS_CRYSX,      "Crysx"      },
    };
    static const int mineral_names_count = (int)(sizeof(mineral_names) / sizeof(mineral_names[0]));

    int      mineral_count[10];
    int      desert_total     = 0;
    int      desert_special   = 0;
    int16_t  wx               = 0;
    int16_t  wy               = 0;
    int16_t  terrain          = 0;
    uint8_t  ts               = 0;
    uint8_t  mineral          = 0;
    int      i                = 0;
    double   actual_pct       = 0.0;
    double   expected_pct     = 0.0;
    int      is_myrror        = 0;
    const char * plane_name   = NULL;

    is_myrror = (wp == MYRROR_PLANE);
    plane_name = is_myrror ? "Myrror" : "Arcanus";

    memset(mineral_count, 0, sizeof(mineral_count));

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain = Get_Terrain(wx, wy, wp);
            if(Terrain_Type_To_Char(terrain) != 'd')
            {
                continue;
            }

            desert_total++;

            ts = Get_Terrain_Special(wx, wy, wp);
            if(ts == TS_NONE)
            {
                continue;
            }

            desert_special++;
            mineral = ts & 0x0Fu;
            if(mineral <= 9u)
            {
                mineral_count[mineral]++;
            }
        }
    }

    printf("\nDesert Special Distribution (%s)  Desert: %d  With Special: %d\n",
        plane_name, desert_total, desert_special);
    printf("%-12s  %6s  %8s  %8s\n", "Mineral", "Count", "% Actual", "% Expect");
    printf("------------  ------  --------  --------\n");

    for(i = 0; i < mineral_names_count; i++)
    {
        expected_pct = -1.0;
        if(is_myrror)
        {
            if(mineral_names[i].mineral == TS_GEMS)  expected_pct = 20.0;
            if(mineral_names[i].mineral == TS_QUORK) expected_pct = 40.0;
            if(mineral_names[i].mineral == TS_CRYSX) expected_pct = 20.0;
        }
        else
        {
            if(mineral_names[i].mineral == TS_GEMS)  expected_pct = 66.7;
            if(mineral_names[i].mineral == TS_QUORK) expected_pct = 33.3;
            if(mineral_names[i].mineral == TS_CRYSX) expected_pct =  0.0;
        }

        if(mineral_count[mineral_names[i].mineral] == 0 && expected_pct < 0.0)
        {
            continue;
        }

        actual_pct = (desert_special > 0)
            ? ((double)mineral_count[mineral_names[i].mineral] / (double)desert_special * 100.0)
            : 0.0;

        if(expected_pct >= 0.0)
        {
            printf("%-12s  %6d  %7.1f%%  %7.1f%%\n",
                mineral_names[i].name,
                mineral_count[mineral_names[i].mineral],
                actual_pct, expected_pct);
        }
        else
        {
            printf("%-12s  %6d  %7.1f%%  %8s\n",
                mineral_names[i].name,
                mineral_count[mineral_names[i].mineral],
                actual_pct, "---");
        }
    }

}

/*
Evaluate_Hills_Special_Distribution

Evaluates which mineral types appear on Hills squares and their relative
frequency among all hills specials.

Expected distribution of specials among hills squares that received one:
  Arcanus: 33.3% Iron, 16.7% Coal, 22.2% Silver, 22.2% Gold, 5.6% Mithril, 0% Adamantium
  Myrror:  10.0% Iron, 10.0% Coal, 10.0% Silver, 40.0% Gold, 20.0% Mithril, 10.0% Adamantium
*/
static void Evaluate_Hills_Special_Distribution(int sim_idx, int16_t wp)
{
    static const struct { uint8_t mineral; const char * name; } mineral_names[] =
    {
        { TS_IRON,       "Iron"       },
        { TS_COAL,       "Coal"       },
        { TS_SILVER,     "Silver"     },
        { TS_GOLD,       "Gold"       },
        { TS_GEMS,       "Gems"       },
        { TS_MITHRIL,    "Mithril"    },
        { TS_ADAMANTIUM, "Adamantium" },
        { TS_QUORK,      "Quork"      },
        { TS_CRYSX,      "Crysx"      },
    };
    static const int mineral_names_count = (int)(sizeof(mineral_names) / sizeof(mineral_names[0]));

    int      mineral_count[10];
    int      hills_total      = 0;
    int      hills_special    = 0;
    int16_t  wx               = 0;
    int16_t  wy               = 0;
    int16_t  terrain          = 0;
    uint8_t  ts               = 0;
    uint8_t  mineral          = 0;
    int      i                = 0;
    double   actual_pct       = 0.0;
    double   expected_pct     = 0.0;
    int      is_myrror        = 0;
    const char * plane_name   = NULL;

    is_myrror = (wp == MYRROR_PLANE);
    plane_name = is_myrror ? "Myrror" : "Arcanus";

    memset(mineral_count, 0, sizeof(mineral_count));

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain = Get_Terrain(wx, wy, wp);
            if(Terrain_Type_To_Char(terrain) != 'h')
            {
                continue;
            }

            hills_total++;

            ts = Get_Terrain_Special(wx, wy, wp);
            if(ts == TS_NONE)
            {
                continue;
            }

            hills_special++;
            mineral = ts & 0x0Fu;
            if(mineral <= 9u)
            {
                mineral_count[mineral]++;
            }
        }
    }

    printf("\nHills Special Distribution (%s)  Hills: %d  With Special: %d\n",
        plane_name, hills_total, hills_special);
    printf("%-12s  %6s  %8s  %8s\n", "Mineral", "Count", "% Actual", "% Expect");
    printf("------------  ------  --------  --------\n");

    for(i = 0; i < mineral_names_count; i++)
    {
        expected_pct = -1.0;
        if(is_myrror)
        {
            if(mineral_names[i].mineral == TS_IRON)       expected_pct = 10.0;
            if(mineral_names[i].mineral == TS_COAL)       expected_pct = 10.0;
            if(mineral_names[i].mineral == TS_SILVER)     expected_pct = 10.0;
            if(mineral_names[i].mineral == TS_GOLD)       expected_pct = 40.0;
            if(mineral_names[i].mineral == TS_MITHRIL)    expected_pct = 20.0;
            if(mineral_names[i].mineral == TS_ADAMANTIUM) expected_pct = 10.0;
        }
        else
        {
            if(mineral_names[i].mineral == TS_IRON)       expected_pct = 33.3;
            if(mineral_names[i].mineral == TS_COAL)       expected_pct = 16.7;
            if(mineral_names[i].mineral == TS_SILVER)     expected_pct = 22.2;
            if(mineral_names[i].mineral == TS_GOLD)       expected_pct = 22.2;
            if(mineral_names[i].mineral == TS_MITHRIL)    expected_pct =  5.6;
            if(mineral_names[i].mineral == TS_ADAMANTIUM) expected_pct =  0.0;
        }

        if(mineral_count[mineral_names[i].mineral] == 0 && expected_pct < 0.0)
        {
            continue;
        }

        actual_pct = (hills_special > 0)
            ? ((double)mineral_count[mineral_names[i].mineral] / (double)hills_special * 100.0)
            : 0.0;

        if(expected_pct >= 0.0)
        {
            printf("%-12s  %6d  %7.1f%%  %7.1f%%\n",
                mineral_names[i].name,
                mineral_count[mineral_names[i].mineral],
                actual_pct, expected_pct);
        }
        else
        {
            printf("%-12s  %6d  %7.1f%%  %8s\n",
                mineral_names[i].name,
                mineral_count[mineral_names[i].mineral],
                actual_pct, "---");
        }
    }

}

/*
Evaluate_Mountain_Special_Distribution

Evaluates which mineral types appear on Mountain squares and their relative
frequency among all mountain specials.

Expected distribution of specials among mountain squares that received one:
  Arcanus: 22.2% Iron, 27.7% Coal, 16.7% Silver, 16.7% Gold, 16.7% Mithril, 0% Adamantium
  Myrror:  10.0% Iron, 10.0% Coal, 10.0% Silver, 20.0% Gold, 30.0% Mithril, 20.0% Adamantium
*/
static void Evaluate_Mountain_Special_Distribution(int sim_idx, int16_t wp)
{
    static const struct { uint8_t mineral; const char * name; } mineral_names[] =
    {
        { TS_IRON,       "Iron"       },
        { TS_COAL,       "Coal"       },
        { TS_SILVER,     "Silver"     },
        { TS_GOLD,       "Gold"       },
        { TS_GEMS,       "Gems"       },
        { TS_MITHRIL,    "Mithril"    },
        { TS_ADAMANTIUM, "Adamantium" },
        { TS_QUORK,      "Quork"      },
        { TS_CRYSX,      "Crysx"      },
    };
    static const int mineral_names_count = (int)(sizeof(mineral_names) / sizeof(mineral_names[0]));

    int      mineral_count[10];
    int      mountain_total    = 0;
    int      mountain_special  = 0;
    int16_t  wx               = 0;
    int16_t  wy               = 0;
    int16_t  terrain          = 0;
    uint8_t  ts               = 0;
    uint8_t  mineral          = 0;
    int      i                = 0;
    double   actual_pct       = 0.0;
    double   expected_pct     = 0.0;
    int      is_myrror        = 0;
    const char * plane_name   = NULL;

    is_myrror = (wp == MYRROR_PLANE);
    plane_name = is_myrror ? "Myrror" : "Arcanus";

    memset(mineral_count, 0, sizeof(mineral_count));

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            terrain = Get_Terrain(wx, wy, wp);
            if(Terrain_Type_To_Char(terrain) != 'm')
            {
                continue;
            }

            mountain_total++;

            ts = Get_Terrain_Special(wx, wy, wp);
            if(ts == TS_NONE)
            {
                continue;
            }

            mountain_special++;
            mineral = ts & 0x0Fu;
            if(mineral <= 9u)
            {
                mineral_count[mineral]++;
            }
        }
    }

    printf("\nMountain Special Distribution (%s)  Mountain: %d  With Special: %d\n",
        plane_name, mountain_total, mountain_special);
    printf("%-12s  %6s  %8s  %8s\n", "Mineral", "Count", "% Actual", "% Expect");
    printf("------------  ------  --------  --------\n");

    for(i = 0; i < mineral_names_count; i++)
    {
        expected_pct = -1.0;
        if(is_myrror)
        {
            if(mineral_names[i].mineral == TS_IRON)       expected_pct = 10.0;
            if(mineral_names[i].mineral == TS_COAL)       expected_pct = 10.0;
            if(mineral_names[i].mineral == TS_SILVER)     expected_pct = 10.0;
            if(mineral_names[i].mineral == TS_GOLD)       expected_pct = 20.0;
            if(mineral_names[i].mineral == TS_MITHRIL)    expected_pct = 30.0;
            if(mineral_names[i].mineral == TS_ADAMANTIUM) expected_pct = 20.0;
        }
        else
        {
            if(mineral_names[i].mineral == TS_IRON)       expected_pct = 22.2;
            if(mineral_names[i].mineral == TS_COAL)       expected_pct = 27.7;
            if(mineral_names[i].mineral == TS_SILVER)     expected_pct = 16.7;
            if(mineral_names[i].mineral == TS_GOLD)       expected_pct = 16.7;
            if(mineral_names[i].mineral == TS_MITHRIL)    expected_pct = 16.7;
            if(mineral_names[i].mineral == TS_ADAMANTIUM) expected_pct =  0.0;
        }

        if(mineral_count[mineral_names[i].mineral] == 0 && expected_pct < 0.0)
        {
            continue;
        }

        actual_pct = (mountain_special > 0)
            ? ((double)mineral_count[mineral_names[i].mineral] / (double)mountain_special * 100.0)
            : 0.0;

        if(expected_pct >= 0.0)
        {
            printf("%-12s  %6d  %7.1f%%  %7.1f%%\n",
                mineral_names[i].name,
                mineral_count[mineral_names[i].mineral],
                actual_pct, expected_pct);
        }
        else
        {
            printf("%-12s  %6d  %7.1f%%  %8s\n",
                mineral_names[i].name,
                mineral_count[mineral_names[i].mineral],
                actual_pct, "---");
        }
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
static void Print_Terrain_Specials_Map(int sim_idx, int16_t wp)
{
    int16_t wx          = 0;
    int16_t wy          = 0;
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

/*
Record Outcomes:
* total map square count
* land map square count
* map square count per terrain type
* map square count per terrain type group
* map square count per terrain special type

Research Notes:
need runs/trials/iterations of a simulation
Average (Relative Frequency)
divide the total number of successful outcomes by the total number of trials
essentially calculating the arithmetic mean of the outcomes

Quantitative Metrics
* Standard Error and Confidence Intervals
* Gelman-Rubin Statistic (R-hat)
* Convergence Bands

Which data?

    uint8_t         landmasses[NUM_PLANES][WORLD_HEIGHT][WORLD_WIDTH];        
    int16_t         world_map[NUM_PLANES][WORLD_HEIGHT][WORLD_WIDTH];         
    uint8_t         terrain_specials[NUM_PLANES][WORLD_HEIGHT][WORLD_WIDTH];  
    uint8_t         map_flags[NUM_PLANES][WORLD_HEIGHT][WORLD_WIDTH];         
    struct s_TOWER  towers[NUM_PLANES][NUM_TOWERS];                           
    struct s_NODE   nodes[NUM_PLANES][NUM_NODES];                             

    struct s_Landmass_Squares_Stats                 landmass_stats[NUM_PLANES];
    struct s_Worldmap_Squares_Stats                 worldmap_stats[NUM_PLANES];
    struct s_Terrain_Special_Squares_Stats          specials_stats[NUM_PLANES];
    struct s_Tower_Map_Square_Hit_Count             tower_stats[NUM_PLANES];

*/
static void Gather_Stats(int32_t sim_idx)
{
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t wp = 0;
    int16_t tt = 0;
    int16_t tg = 0;
    uint8_t ts = 0;
    int32_t total_square_count = 0;
    int32_t land_square_count = 0;
    int32_t tower_idx = 0;

    trial_run_count++;

    memset(&simulation_data[sim_idx].worldmap_stats[wp], 0, sizeof(struct s_Worldmap_Squares_Stats));
    memset(&simulation_data[sim_idx].tower_stats[wp],    0, sizeof(struct s_Tower_Map_Square_Hit_Count));
    memset(&simulation_data[sim_idx].specials_stats[wp], 0, sizeof(struct s_Terrain_Special_Squares_Stats));

    tower_idx = 0;
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                if(Square_Has_Tower_NewGame(wx, wy) == ST_TRUE)
                {
                    simulation_data[sim_idx].tower_stats[wp].wx[tower_idx] = wx;
                    simulation_data[sim_idx].tower_stats[wp].wy[tower_idx] = wy;
                    simulation_data[sim_idx].tower_stats[wp].tower_count = (tower_idx + 1);
                    tower_idx++;
                }
                tt = Get_Terrain(wx, wy, wp);
                simulation_data[sim_idx].worldmap_stats[wp].terrain_type_count[tt]++;
                tg = Get_Terrain_Group(tt);
#ifdef STU_DEBUG
                if(tg == -1) { STU_DEBUG_BREAK(); }
#endif
                simulation_data[sim_idx].worldmap_stats[wp].terrain_group_count[tg]++;
                simulation_data[sim_idx].worldmap_stats[wp].total_count++;
            }
        }
    }
    
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        total_square_count = simulation_data[sim_idx].worldmap_stats[wp].total_count;
        land_square_count = 0;
        for(tt = 0; tt < TerType_Count; tt++)
        {
            if(tt != tt_Ocean)
            {
                land_square_count += simulation_data[sim_idx].worldmap_stats[wp].terrain_type_count[tt];
            }
        }
    }

    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                ts = Get_Terrain_Special(wx, wy, wp);
                simulation_data[sim_idx].specials_stats[wp].terrain_special_square_count[ts]++;
                simulation_data[sim_idx].specials_stats[wp].total_count++;
                if(ts != ts_NONE)
                {
                    simulation_data[sim_idx].specials_stats[wp].special_squares++;
                }
            }
        }
    }

}

/*
    Calculate_Average_Terrain_Probability

    Sums terrain-type counts from worldmap_stats across all simulation runs
    for plane `wp`, then prints the average count and average probability
    (as a percentage of total squares) for each terrain type that appeared
    at least once.
*/
static void Calculate_Average_Terrain_Probability(int num_simulations, int16_t wp)
{
    int32_t      sum_counts[TerType_Count];
    int          itr            = 0;
    int          sim_idx        = 0;
    int32_t      total_squares  = 0;
    double       avg_count      = 0.0;
    double       avg_prob       = 0.0;
    const char * name           = NULL;

    memset(sum_counts, 0, sizeof(sum_counts));

    for(sim_idx = 0; sim_idx < num_simulations; sim_idx++)
    {
        for(itr = 0; itr < TerType_Count; itr++)
        {
            sum_counts[itr] += simulation_data[sim_idx].worldmap_stats[wp].terrain_type_count[itr];
        }
    }

    total_squares = num_simulations * WORLD_SIZE;

    printf("\nAverage Terrain Probability (%s, %d simulations)\n", (wp == ARCANUS_PLANE) ? "Arcanus" : "Myrror", num_simulations);
    printf("%3s  %-12s  %9s  %10s\n", "Idx", "Name", "Avg Count", "Avg Prob %");
    printf("---  ------------  ---------  ----------\n");

    for(itr = 0; itr < TerType_Count; itr++)
    {
        if(sum_counts[itr] == 0)
            continue;

        name = Terrain_Type_Name(itr);

        avg_count = (double)sum_counts[itr] / (double)num_simulations;
        avg_prob  = (total_squares > 0) ? ((double)sum_counts[itr] / (double)total_squares * 100.0) : 0.0;

        printf("%3d  %-12s  %9.1f  %9.2f%%\n", itr, name, avg_count, avg_prob);
    }

    printf("---  ------------  ---------  ----------\n");
    printf("%-24s  %9d\n", "Squares per sim:", WORLD_SIZE);
    printf("%-24s  %9d\n", "Simulations:", num_simulations);
}

/*
    Calculate_Average_Terrain_Group_Probability

    Sums terrain-group counts from worldmap_stats across all simulation runs
    for plane `wp`, then prints the average count and average probability
    (as a percentage of total squares) for each terrain group that appeared
    at least once.
*/
static void Calculate_Average_Terrain_Group_Probability(int num_simulations, int16_t wp)
{
    int32_t      sum_counts[NUM_TERRAIN_GROUPS];
    int          itr            = 0;
    int          sim_idx        = 0;
    int32_t      total_squares  = 0;
    int32_t      land_squares   = 0;
    double       avg_count      = 0.0;
    double       avg_prob       = 0.0;
    double       avg_land_prob  = 0.0;
    const char * name           = NULL;

    memset(sum_counts, 0, sizeof(sum_counts));

    for(sim_idx = 0; sim_idx < num_simulations; sim_idx++)
    {
        for(itr = 0; itr < terrain_group_names_count; itr++)
        {
            sum_counts[itr] += simulation_data[sim_idx].worldmap_stats[wp].terrain_group_count[itr];
        }
    }

    total_squares = num_simulations * WORLD_SIZE;
    land_squares = total_squares - sum_counts[0];

    printf("\nAverage Terrain Group Probability (%s, %d simulations)\n", (wp == ARCANUS_PLANE) ? "Arcanus" : "Myrror", num_simulations);
    printf("%3s  %-12s  %9s  %10s  %10s\n", "Grp", "Name", "Avg Count", "Avg Prob %", "Avg Land %");
    printf("---  ------------  ---------  ----------  ----------\n");

    for(itr = 0; itr < terrain_group_names_count; itr++)
    {
        if(sum_counts[itr] == 0)
            continue;

        name = Terrain_Group_Name(itr);

        avg_count = (double)sum_counts[itr] / (double)num_simulations;
        avg_prob  = (total_squares > 0) ? ((double)sum_counts[itr] / (double)total_squares * 100.0) : 0.0;

        if(itr == 0)
        {
            printf("%3d  %-12s  %9.1f  %9.2f%%  %10s\n", itr, name, avg_count, avg_prob, "---");
        }
        else
        {
            avg_land_prob = (land_squares > 0) ? ((double)sum_counts[itr] / (double)land_squares * 100.0) : 0.0;
            printf("%3d  %-12s  %9.1f  %9.2f%%  %9.2f%%\n", itr, name, avg_count, avg_prob, avg_land_prob);
        }
    }

    printf("---  ------------  ---------  ----------  ----------\n");
    printf("%-24s  %9d\n", "Squares per sim:", WORLD_SIZE);
    printf("%-24s  %9d\n", "Simulations:", num_simulations);
}

/*
    Calculate_Average_Terrain_Special_Probability

    Sums terrain-special counts from specials_stats across all simulation runs
    for plane `wp`, then prints the average count and average probability
    for each known terrain special (minerals and flags).
*/
static void Calculate_Average_Terrain_Special_Probability(int num_simulations, int16_t wp)
{
    static const struct { uint8_t value; const char * name; } special_names[] =
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
    static const int special_names_count = (int)(sizeof(special_names) / sizeof(special_names[0]));

    int32_t  sum_counts[NUM_TERRAIN_SPECIAL_TYPES];
    int32_t  sum_special_squares = 0;
    int      itr                 = 0;
    int      sim_idx             = 0;
    int32_t  total_squares       = 0;
    int32_t  count               = 0;
    double   avg_count           = 0.0;
    double   avg_prob            = 0.0;
    double   avg_spcl_prob       = 0.0;

    memset(sum_counts, 0, sizeof(sum_counts));

    for(sim_idx = 0; sim_idx < num_simulations; sim_idx++)
    {
        for(itr = 0; itr < NUM_TERRAIN_SPECIAL_TYPES; itr++)
        {
            sum_counts[itr] += simulation_data[sim_idx].specials_stats[wp].terrain_special_square_count[itr];
        }
        sum_special_squares += simulation_data[sim_idx].specials_stats[wp].special_squares;
    }

    total_squares = num_simulations * WORLD_SIZE;

    printf("\nAverage Terrain Special Probability (%s, %d simulations)\n", (wp == ARCANUS_PLANE) ? "Arcanus" : "Myrror", num_simulations);
    printf("%-12s  %9s  %10s  %10s\n", "Special", "Avg Count", "Avg Prob %", "Avg Spcl %");
    printf("------------  ---------  ----------  ----------\n");

    for(itr = 0; itr < special_names_count; itr++)
    {
        count = sum_counts[special_names[itr].value];

        avg_count = (double)count / (double)num_simulations;
        avg_prob  = (total_squares > 0) ? ((double)count / (double)total_squares * 100.0) : 0.0;

        if(special_names[itr].value == TS_NONE)
        {
            printf("%-12s  %9.1f  %9.2f%%  %10s\n", special_names[itr].name, avg_count, avg_prob, "---");
        }
        else
        {
            avg_spcl_prob = (sum_special_squares > 0) ? ((double)count / (double)sum_special_squares * 100.0) : 0.0;
            printf("%-12s  %9.1f  %9.2f%%  %9.2f%%\n", special_names[itr].name, avg_count, avg_prob, avg_spcl_prob);
        }
    }

    printf("------------  ---------  ----------  ----------\n");
    printf("%-24s  %9d\n", "Squares per sim:", WORLD_SIZE);
    printf("%-24s  %9.1f\n", "Avg special squares:", (double)sum_special_squares / (double)num_simulations);
    printf("%-24s  %9d\n", "Simulations:", num_simulations);
}

static void Allocate_Simulation(int simulations_to_run)
{
    size_t size;
    // // simulation_data = (struct s_Simulation_Data *)Allocate_Space((simulations_to_run * sizeof(struct s_Simulation_Data)) / SZ_PARAGRAPH_B + 1);
    // // if(NULL == simulation_data)     { Exit_With_Message("FATAL: Allocate_Space() failed for simulation_data"); return; }
    // simulation_data = (struct s_Simulation_Data *)malloc(simulations_to_run * sizeof(struct s_Simulation_Data));
    // memset(simulation_data, 0, simulations_to_run * sizeof(struct s_Simulation_Data));
    size = (simulations_to_run * sizeof(struct s_Simulation_Data));
    simulation_data = malloc(size);
    if (!simulation_data)
    {
        Exit_With_Message("FATAL: malloc() failed for simulation_data");
        return;
    }
    memset(simulation_data, 0, size);
}

static void Allocate_Game_Data(void)
{
    _landmasses                  = (uint8_t           *)Allocate_Space(NUM_PLANES * (((WORLD_WIDTH * WORLD_HEIGHT) * 1) / SZ_PARAGRAPH_B));
    // _world_maps               = (uint8_t           *)Allocate_Space( NUM_PLANES * (((WORLD_WIDTH * WORLD_HEIGHT) * 2)                  / SZ_PARAGRAPH_B));
    //_world_maps                = (uint8_t           *)Allocate_Space((NUM_PLANES * (((WORLD_WIDTH * WORLD_HEIGHT) * 2) + WORLD_OVERRUN) / SZ_PARAGRAPH_B));
    _world_maps                  = (uint8_t           *)Allocate_Space(( ((((NUM_PLANES * WORLD_SIZE) + WORLD_OVERRUN) * sizeof(int16_t)) / SZ_PARAGRAPH_B) + 2) );
    // _map_square_terrain_speci = (uint8_t           *)Allocate_Space(NUM_PLANES * (((WORLD_WIDTH * WORLD_HEIGHT) * 1) / SZ_PARAGRAPH_B));
    _map_square_terrain_specials = (uint8_t           *)Allocate_Space(((((NUM_PLANES * WORLD_SIZE) + WORLD_OVERRUN) * sizeof(uint8_t)) / SZ_PARAGRAPH_B) + 2);
    _map_square_flags            = (uint8_t           *)Allocate_Space(NUM_PLANES * (((WORLD_WIDTH * WORLD_HEIGHT) * 1) / SZ_PARAGRAPH_B));
    _UNITS                       = (struct s_UNIT     *)Allocate_Space(2028);
    _CITIES                      = (struct s_CITY     *)Allocate_Space((((NUM_CITIES     * sizeof(struct s_CITY    )) / SZ_PARAGRAPH_B) + 1));
    _NODES                       = (struct s_NODE     *)Allocate_Space((((NUM_NODES      * sizeof(struct s_NODE    )) / SZ_PARAGRAPH_B) + 1));
    _FORTRESSES                  = (struct s_FORTRESS *)Allocate_Space((((NUM_FORTRESSES * sizeof(struct s_FORTRESS)) / SZ_PARAGRAPH_B) + 1));
    _TOWERS                      = (struct s_TOWER    *)Allocate_Space((((NUM_TOWERS     * sizeof(struct s_TOWER   )) / SZ_PARAGRAPH_B) + 1));
    _LAIRS                       = (struct s_LAIR     *)Allocate_Space((((NUM_LAIRS_102  * sizeof(struct s_LAIR    )) / SZ_PARAGRAPH_B) + 1));
    p_world_map = (int16_t (*)[WORLD_HEIGHT][WORLD_WIDTH])_world_maps;
}

static void Clear_Game_Data(void)
{
    memset(_landmasses,                   0,  (NUM_PLANES * (((WORLD_WIDTH * WORLD_HEIGHT) * 1) / SZ_PARAGRAPH_B)));
    memset(_world_maps,                   0,  (NUM_PLANES * (((WORLD_WIDTH * WORLD_HEIGHT) * 2) / SZ_PARAGRAPH_B)));
    memset(_map_square_terrain_specials,  0,  (NUM_PLANES * (((WORLD_WIDTH * WORLD_HEIGHT) * 1) / SZ_PARAGRAPH_B)));
    memset(_map_square_flags,             0,  (NUM_PLANES * (((WORLD_WIDTH * WORLD_HEIGHT) * 1) / SZ_PARAGRAPH_B)));
    memset(_UNITS,                        0,  (2028 * SZ_PARAGRAPH_B)                              );
    memset(_CITIES,                       0,  (NUM_CITIES     * sizeof(struct s_CITY     ))        );
    memset(_NODES,                        0,  (NUM_NODES      * sizeof(struct s_NODE     ))        );
    memset(_FORTRESSES,                   0,  (NUM_FORTRESSES * sizeof(struct s_FORTRESS ))        );
    memset(_TOWERS,                       0,  (NUM_TOWERS     * sizeof(struct s_TOWER    ))        );
    memset(_LAIRS,                        0,  (NUM_LAIRS_102  * sizeof(struct s_LAIR     ))        );
}

static void New_Game_Screen_Mock(void)
{
    FILE * file_pointer = 0;
    char builddat_lbx_file[] = "BUILDDAT.LBX";

    bldg_data_table = (struct s_BLDG_DATA *)LBX_Load_Data(builddat_lbx_file, 0, 0, 36, 52);
    assert(bldg_data_table != NULL);
    assert(bldg_data_table[bt_NONE].name != NULL);
    assert(bldg_data_table[bt_NONE].maintenance_cost == 0);

    magic_set.LandSize = gol_Large;
    magic_set.MagicPower = gom_Normal;
    magic_set.Opponents = goo_Two;
    magic_set.Difficulty = god_Normal;
    
    file_pointer = fopen("MAGIC.SET", "wb");
    fwrite(&magic_set, sizeof(struct s_MAGIC_SET), 1, file_pointer);
    fclose(file_pointer);

    _landsize = magic_set.LandSize;
    _magic = magic_set.MagicPower;
    _num_players = (magic_set.Opponents + 1);
    _difficulty = magic_set.Difficulty;

    NEWG_Clicked_Race = rt_Orc;

}

/* capture game data from transient globals into simulation_data[itr] */
static void Capture_Game_Data(int sim_idx)
{

    memcpy(simulation_data[sim_idx].landmasses[ARCANUS_PLANE], (uint8_t *)_landmasses + (ARCANUS_PLANE * WORLD_SIZE), WORLD_SIZE);
    memcpy(simulation_data[sim_idx].landmasses[MYRROR_PLANE],  (uint8_t *)_landmasses + (MYRROR_PLANE  * WORLD_SIZE), WORLD_SIZE);

    memcpy(simulation_data[sim_idx].world_map[ARCANUS_PLANE], (uint8_t *)_world_maps + (ARCANUS_PLANE * WORLD_SIZE_DW), WORLD_SIZE_DW);
    memcpy(simulation_data[sim_idx].world_map[MYRROR_PLANE],  (uint8_t *)_world_maps + (MYRROR_PLANE  * WORLD_SIZE_DW), WORLD_SIZE_DW);

    memcpy(simulation_data[sim_idx].terrain_specials[ARCANUS_PLANE], (uint8_t *)_map_square_terrain_specials + (ARCANUS_PLANE * WORLD_SIZE), WORLD_SIZE);
    memcpy(simulation_data[sim_idx].terrain_specials[MYRROR_PLANE],  (uint8_t *)_map_square_terrain_specials + (MYRROR_PLANE  * WORLD_SIZE), WORLD_SIZE);

    memcpy(simulation_data[sim_idx].map_flags[ARCANUS_PLANE], (uint8_t *)_map_square_flags + (ARCANUS_PLANE * WORLD_SIZE), WORLD_SIZE);
    memcpy(simulation_data[sim_idx].map_flags[MYRROR_PLANE],  (uint8_t *)_map_square_flags + (MYRROR_PLANE  * WORLD_SIZE), WORLD_SIZE);

    memcpy(simulation_data[sim_idx].towers[ARCANUS_PLANE], _TOWERS, NUM_TOWERS * sizeof(struct s_TOWER));
    memcpy(simulation_data[sim_idx].towers[MYRROR_PLANE],  _TOWERS, NUM_TOWERS * sizeof(struct s_TOWER));

    memcpy(simulation_data[sim_idx].nodes[ARCANUS_PLANE], _NODES, NUM_NODES * sizeof(struct s_NODE));
    memcpy(simulation_data[sim_idx].nodes[MYRROR_PLANE],  _NODES, NUM_NODES * sizeof(struct s_NODE));

}

static void Capture_Simulation_Data(int sim_idx)
{

    Capture_Game_Data(sim_idx);  // superflous?

    Landmass_Statistics(sim_idx, ARCANUS_PLANE);
    Landmass_Statistics(sim_idx, MYRROR_PLANE);

    Worldmap_Statistics(sim_idx, ARCANUS_PLANE);
    Worldmap_Statistics(sim_idx, MYRROR_PLANE);

    Terrain_Specials_Statistics(sim_idx, ARCANUS_PLANE);
    Terrain_Specials_Statistics(sim_idx, MYRROR_PLANE);

};

void Simulate_World_Map_Generation(void)
{
    int16_t rivers = 0;
    int16_t tries = 0;
    int simulations_to_run = 0;
    int num_maps_to_generate = 0;
    int itr = 0;

    simulations_to_run = 3;  // max is 32,767?
    num_maps_to_generate = (simulations_to_run * 2);

    Allocate_Simulation(simulations_to_run);
    Allocate_Game_Data();
    New_Game_Screen_Mock();

    for(itr = 0; itr < simulations_to_run; itr++)
    {

        Clear_Game_Data();

        Init_Landmasses(ARCANUS_PLANE);
        Init_Landmasses(MYRROR_PLANE);

        Generate_Landmasses(ARCANUS_PLANE);
        Generate_Landmasses(MYRROR_PLANE);

        Translate_Heightmap_To_Base_Terrain_Types(ARCANUS_PLANE);
        Translate_Heightmap_To_Base_Terrain_Types(MYRROR_PLANE);

        Generate_Climate_Terrain_Types(ARCANUS_PLANE);
        Generate_Climate_Terrain_Types(MYRROR_PLANE);

        Generate_Nodes();

        Rebalance_Node_Types(ARCANUS_PLANE);
        Rebalance_Node_Types(MYRROR_PLANE);

        Generate_Towers();

        Extend_Islands(ARCANUS_PLANE);
        Extend_Islands(MYRROR_PLANE);

        Generate_Lairs();
        
        _units = 0;
        Generate_Home_City();
        Generate_Neutral_Cities(ARCANUS_PLANE);
        Generate_Neutral_Cities(MYRROR_PLANE);

        Generate_Terrain_Specials(ARCANUS_PLANE);
        Generate_Terrain_Specials(MYRROR_PLANE);

        Generate_Roads(ARCANUS_PLANE);
        Generate_Roads(MYRROR_PLANE);

        Simex_Autotiling();

        for(rivers = 0; rivers < NUM_RIVERS; rivers++)
        {
            for(tries = 0; ((tries < 2000) && (River_Path(ARCANUS_PLANE) != 0)); tries++) { }
            for(tries = 0; ((tries < 2000) && (River_Path(MYRROR_PLANE)  != 0)); tries++) { }
        }

        River_Terrain(ARCANUS_PLANE);
        River_Terrain(MYRROR_PLANE);

        Desert_Autotile();

        Shuffle_Terrains();

        // Movement_Mode_Cost_Maps(ARCANUS_PLANE);

        // CRP_NEWG_CreatePathGrids__WIP(MYRROR_PLANE);

        // Init_Square_Explored();

        Animate_Oceans();

        // Set_Upper_Lair_Guardian_Count();



        
        Worldmap_Statistics(itr, ARCANUS_PLANE);
        Display_Worldmap_Statistics(itr, ARCANUS_PLANE);
        // Display_Worldmap_Histogram(itr, ARCANUS_PLANE);
        Print_Worldmap_Map(itr, ARCANUS_PLANE);
        Worldmap_Statistics(itr, MYRROR_PLANE);
        Display_Worldmap_Statistics(itr, MYRROR_PLANE);
        // Display_Worldmap_Histogram(itr, MYRROR_PLANE);
        Print_Worldmap_Map(itr, MYRROR_PLANE);



        // Capture_Simulation_Data(itr);

        // printf("Simulation # %d\n", (1 + itr));

        // Gather_Stats(itr);

    }

    /*
        display statistics for entire simulation run
         - average probability of each terrain type
         - histogram
    */

    // Calculate_Average_Terrain_Probability(simulations_to_run, ARCANUS_PLANE);
    // Calculate_Average_Terrain_Probability(simulations_to_run, MYRROR_PLANE);
    // Calculate_Average_Terrain_Group_Probability(simulations_to_run, ARCANUS_PLANE);
    // Calculate_Average_Terrain_Group_Probability(simulations_to_run, MYRROR_PLANE);
    // Calculate_Average_Terrain_Special_Probability(simulations_to_run, ARCANUS_PLANE);
    // Calculate_Average_Terrain_Special_Probability(simulations_to_run, MYRROR_PLANE);

}
