/**
 * @file MAPGEN.c
 * @brief New-game world generation and map setup routines.
 *
 * Provides map construction logic used during game initialization,
 * including landmass and terrain generation, node/lair placement,
 * river/shore setup, neutral city and road generation, and related
 * support utilities for initial world state.
 */

 /*
    MAGIC.EXE
    ovr051
    ...
    ovr054
    ovr055

MoO2
    Module: HOMEGEN
    Module: INITGAME
    Module: MAPGEN
*/

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"  /* CLAUDE 2026-06-01: LOG_INFO for [LOC_PICK] iteration trace */
#include "../../STU/src/STU_VLD.h"

#include "../../ext/stu_compat.h"

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/Mouse.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_BITS.h"  /* GET_2B_OFS() */
#include "../../MoX/src/MOX_DAT.h"  /* _players[]; enum e_SCOUT_BITS{} */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/Video.h"
#include "../../MoX/src/special.h"

#include "INITGAME.h"
#include "NEXTTURN.h"
#include "RACETYPE.h"
#include "Terrain.h"
#include "TerrType.h"
#include "UNITTYPE.h"
#include "MGC_DATA.h"  /* terrain_stats_table[] */

#include <assert.h>     /* assert() */
#include <string.h>     /* memcpy() memset(), stu_strcat(), stu_strcpy(), stricmp() */

#include "MAPGEN.h"

#include "../../MoM/src/MovePath.h"  /* struct s_MOVE_MODE_COST_MAPS */
#include "../../MoX/src/EMS/EMS.h"   /* CLAUDE  SAMB_ptr EMMDATAH_Map(void) prototype (was implicit -> C4013) */
// MOM_Data.c./h but also MovePath.h
// WZD dseg:9CAC
// drake178: TBL_MoveMaps_EMS
extern struct s_MOVE_MODE_COST_MAPS * movement_mode_cost_maps;



// forward declare; non-static to match the definition and the MAPGEN.h prototype (was erroneously static -> C4211)
void Build_Connectivity_Graph(int8_t * move_map, uint8_t * result_map);





// MGC  dseg:8EA4
extern int16_t NEWG_Clicked_Race;

// MGC  dseg:8DD6
// drake78: IMG_NEWG_MapBuildBG
/*
    loaded in GAME_New_Screen_7()
    used in Init_New_Game()
*/
extern SAMB_ptr newgame_BUILDWOR_map_build_bar_seg;



// MGC  dseg:3334                                                 BEGIN:  ovr051 - Initialized Data  (MAPGEN)

// MGC  dseg:3334
/*
Direction arrays (dx, dy): These define movement vectors for South, West, North, East, and none.
    down, left, up, right, center
    South, West, North, East, None
NOTE: both are used in Generate_Landmasses()

dir_chg_tbl_wx__1()
used for Node Auras
used for River Paths

*/
int16_t dir_chg_step_tbl_wx[5] = { 0, -1, 0, 1, 0 };
int16_t dir_chg_step_tbl_wy[5] = { 1, 0, -1, 0, 0 };

// MGC  dseg:3348
// {1:W,3:E}
int16_t upstream[4] = { 2, 3, 0, 1 };  // ¿ N, E, S, W ?

// MGC  dseg:3350
// int16_t TILE_Cardinal_XMod2[5] = { 0, -1, 0, 1, 0 };
// int16_t TILE_Cardinal_YMod2[5] = { 1, 0, -1, 0, 0 };

int16_t dir_chg_start_tbl_wx[5] = { 0, -1, 0, 1, 0 };
int16_t dir_chg_start_tbl_wy[5] = { 1, 0, -1, 0, 0 };
// TODO  figure out which should be start or step
int16_t dir_chg_tbl_wx[5] = { 0, -1, 0, 1, 0 };
int16_t dir_chg_tbl_wy[5] = { 1, 0, -1, 0, 0 };

// MGC  dseg:3364
struct s_mouse_list mouselist_mapgen[1] =
{
    { crsr_Hourglass, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX }
};

// MGC  dseg:3370 26 34 2C 34 37 34 43 34 50 34 60 34 68 34 77 34+UU_EZ_Names@_Array dw offset UU_cnst_Tower, offset UU_cnst_ChaosNode, offset UU_cnst_NatureNode, offset UU_cnst_SorceryNode, offset UU_cnst_Cave, offset UU_cnst_Dungeon, offset UU_cnst_AncientTmpl, offset UU_cnst_Keep, offset UU_cnst_Lair, offset UU_cnst_Ruins, offset UU_cnst_FallenTmpl ; "tower"

// MGC  dseg:3386
int16_t TILE_River_Types[16][4] = 
{
    { tte_Grasslands, tte_Grasslands, tte_Grasslands, tte_Grasslands },
    { _River1000,     _River1000,     _River1000,     _River1000     },
    { _River0100,     _River0100,     _River0100,     _River0100     },
    { _River1100_1,   _River1100_2,   _River1100_3,   _River1100_1   },
    { _River0010,     _River0010,     _River0010,     _River0010     },
    { _River1010_1,   _River1010_2,   _River1010_3,   _River1010_1   },
    { _River0110_1,   _River0110_2,   _River0110_3,   _River0110_1   },
    { _River1110_1,   _River1110_2,   _River1110_3,   _River1110_4   },
    { _River0001,     _River0001,     _River0001,     _River0001     },
    { _River1001_1,   _River1001_2,   _River1001_3,   _River1001_1   },
    { _River0101_1,   _River0101_2,   _River0101_3,   _River0101_1   },
    { _River1101_1,   _River1101_2,   _River1101_3,   _River1101_4   },
    { _River0011_1,   _River0011_2,   _River0011_3,   _River0011_1   },
    { _River1011_1,   _River1011_2,   _River1011_3,   _River1011_4   },
    { _River0111_1,   _River0111_2,   _River0111_3,   _River0111_4   },
    { _River1111_1,   _River1111_2,   _River1111_3,   _River1111_4   }
};

// MGC  dseg:3406
char builddat_lbx_file__MGC_ovr051[] = "BUILDDAT";

// MGC  dseg:340F
char str_BuildingTheWorlds[] = "Building The Worlds...";

// MGC  dseg:3426 74 6F 77 65 72 00                               UU_cnst_Tower db 'tower',0              ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:342C 63 68 61 6F 73 20 6E 6F 64 65 00                UU_cnst_ChaosNode db 'chaos node',0     ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3437 6E 61 74 75 72 65 20 6E 6F 64 65 00             UU_cnst_NatureNode db 'nature node',0   ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3443 73 6F 72 63 65 72 79 20 6E 6F 64 65 00          UU_cnst_SorceryNode db 'sorcery node',0 ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3450 6D 79 73 74 65 72 69 6F 75 73 20 63 61 76 65 00 UU_cnst_Cave db 'mysterious cave',0     ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3460 64 75 6E 67 65 6F 6E 00                         UU_cnst_Dungeon db 'dungeon',0          ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3468 61 6E 63 69 65 6E 74 20 74 65 6D 70 6C 65 00    UU_cnst_AncientTmpl db 'ancient temple',0
// MGC  dseg:3477 61 62 61 6E 64 6F 6E 65 64 20 6B 65 65 70 00    UU_cnst_Keep db 'abandoned keep',0      ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3486 6D 6F 6E 73 74 65 72 20 6C 61 69 72 00          UU_cnst_Lair db 'monster lair',0        ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3493 61 6E 63 69 65 6E 74 20 72 75 69 6E 73 00       UU_cnst_Ruins db 'ancient ruins',0      ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:34A1 66 61 6C 6C 65 6E 20 74 65 6D 70 6C 65 00       UU_cnst_FallenTmpl db 'fallen temple',0 ; DATA XREF: dseg:UU_EZ_Names@_Arrayo

// MGC  dseg:34AF
char terrtype_lbx_file__MGC_ovr051[] = "TERRTYPE";

// MGC  dseg:34B8
char cityname_lbx_file__MGC_ovr051[] = "CITYNAME";
// // WZD dseg:6EC7
// char cityname_lbx_file__ovr143[] = "CITYNAME";

// MGC  dseg:34C1 00 00 00                                        align 4

// MGC  dseg:34C1                                                 END:  ovr051 - Initialized Data  (MAPGEN)





// MGC  dseg:8EB0                                                 BEGIN:  ovr051 - Uninitialized Data  (MAPGEN)

// MGC  dseg:8EB0
/*
no idea why SimTex made this 200, when there are only 60 landmasses
only used in Generate_Neutral_Cities()
populated with enum e_RACE_TYPE values, randomly selected from the groups of races per plane
used as the 'default race' per plane, with a three in four chance
*/
int16_t m_landmasses_default_race[200];

// MGC  dseg:9040
/*
set to 1 in Init_Landmasses(), because 0 means no landmass
incremented in TILE_SetLandMass()
is the landmass_idx assigned in _landmasses
*/
int16_t m_landmasses_ctr;

// MGC  dseg:9042 00 00                                           dw 0
// MGC  dseg:9044 00 00                                           dw 0
// MGC  dseg:9046 00 00                                           dw 0
// MGC  dseg:9048 00 00                                           dw 0
// MGC  dseg:904A 00 00                                           dw 0
// MGC  dseg:904A
// MGC  dseg:904A                                                 END:  ovr051 - Uninitialized Data  (MAPGEN)



// MGC 31D1:9118                                                 BEGIN:  ovr054 - Uninitialized Data

// MGC 31D1:9118
int16_t shortest_path_backtrace[100];
// MGC 31D1:91E0
uint8_t shortest_path_cost_to_reach[225];           /* 1-byte, unsigned;  read is mov ah, 0 (zero-extend, not cbw), cost comparison is jbe (unsigned) */
// MGC 31D1:92C1 00                                              align 2
// MGC 31D1:92C2
int16_t shortest_path_came_from[225];

// MGC 31D1:92C2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+END:  ovr054 - Uninitialized Data





// MGC o51p01
// drake178: NEWG_CreateWorld()
/*
NEWG_CreateWorld()
¿ ~ MoO2 Module MAPGEN ?
¿ MoO2  Module: INITGAME  Init_New_Game_() ?
¿ MoO2  Module: HOMEGEN  Generate_Home_Worlds_() ?
creates the basic profiles of the AI wizards, then
generates the game map for both planes, drawing and
updating a progress bar in the process
BUG: may freeze if tower or fortress generation fails
due to there not being enough valid locations - also
inherits numerous smaller issues from the subroutines
and their order of execution
*/
/*

*/
/**
 * @brief Creates and initializes the new-game world state.
 *
 * @details
 * Executes the full world-generation pipeline for both planes, including
 * landmass setup, terrain base/special passes, node/tower/lair generation,
 * home and neutral city creation, roads, river/desert/randomization passes,
 * movement/path map setup, and initial exploration flags.
 *
 * During generation, this function updates the "Building The Worlds..."
 * progress display and prepares map-generation resources such as
 * `bldg_data_table`.
 *
 * Side effects include extensive mutation of global game data structures
 * (map squares, sites, cities, units, movement costs, and exploration state).
 *
 * @note This is an orchestrator routine; it relies on many subordinate
 *       generation functions and does not return an explicit status value.
 *
 * @warning Historical behavior notes indicate potential long-running or
 *          stalled generation in edge cases when valid placement locations
 *          are insufficient.
 *
 * @return void
 *
 * @see Generate_Home_Cities
 * @see Generate_Neutral_Cities
 * @see Generate_Terrain_Specials
 * @see Generate_Roads
 * @see Init_Square_Explored
 */

/* CLAUDE: capture the world map (a [GD] dump) so it can be byte-compared against
 * OG -- Generate_Towers reads p_world_map, so this tells us whether the map
 * going INTO the tower/lair phase already differs, or whether Generate_Towers
 * reads an identical map+seed and still draws differently.  One [GD] line per
 * (plane,row): "[GD] <point> _world_maps[<plane>].y<NN> = csv int16 tiles". */
void gd_dump_world_map(const char* point)
{
    int plane, y, x, q;
    char row[WORLD_WIDTH * 8];
    for(plane = 0; plane < NUM_PLANES; plane++) {
        for(y = 0; y < WORLD_HEIGHT; y++) {
            q = 0;
            for(x = 0; x < WORLD_WIDTH; x++) {
                int16_t v = p_world_map[plane][y][x];
                q += snprintf(row + q, sizeof(row) - q, x ? ",%d" : "%d", (int)v);
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _world_maps[%d].y%02d = %s",
                      point, plane, y, row);
        }
        STU_Log_Flush_All();   /* drain the async ring between planes (flood fix) */
    }
}

/* CLAUDE: capture _landmasses (uint8 landmass-id per tile) for OG byte-compare. */
void gd_dump_landmasses(const char* point)
{
    int plane, y, x, q;
    char row[WORLD_WIDTH * 5];
    for(plane = 0; plane < NUM_PLANES; plane++) {
        for(y = 0; y < WORLD_HEIGHT; y++) {
            q = 0;
            for(x = 0; x < WORLD_WIDTH; x++) {
                int v = GET_LANDMASS(x, y, plane);
                q += snprintf(row + q, sizeof(row) - q, x ? ",%d" : "%d", v);
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _landmasses[%d].y%02d = %s",
                      point, plane, y, row);
        }
        STU_Log_Flush_All();
    }
}

/* CLAUDE: capture _map_square_terrain_specials (uint8 terrain-special id per tile) for OG byte-compare. */
void gd_dump_terrain_specials(const char* point)
{
    int plane, y, x, q;
    char row[WORLD_WIDTH * 5];
    for(plane = 0; plane < NUM_PLANES; plane++) {
        for(y = 0; y < WORLD_HEIGHT; y++) {
            q = 0;
            for(x = 0; x < WORLD_WIDTH; x++) {
                int v = GET_TERRAIN_SPECIAL(x, y, plane);
                q += snprintf(row + q, sizeof(row) - q, x ? ",%d" : "%d", v);
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _map_square_terrain_specials[%d].y%02d = %s",
                      point, plane, y, row);
        }
        STU_Log_Flush_All();
    }
}

/* CLAUDE: capture _map_square_flags (uint8 per-square flag bits) for OG byte-compare. */
void gd_dump_map_square_flags(const char* point)
{
    int plane, y, x, q;
    char row[WORLD_WIDTH * 5];
    for(plane = 0; plane < NUM_PLANES; plane++) {
        for(y = 0; y < WORLD_HEIGHT; y++) {
            q = 0;
            for(x = 0; x < WORLD_WIDTH; x++) {
                int v = GET_MAP_SQUARE_FLAG(x, y, plane);
                q += snprintf(row + q, sizeof(row) - q, x ? ",%d" : "%d", v);
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _map_square_flags[%d].y%02d = %s",
                      point, plane, y, row);
        }
        STU_Log_Flush_All();
    }
}

/* CLAUDE: capture _square_explored (uint8 per-square explored flag) for OG byte-compare. */
static void gd_dump_square_explored(const char* point)
{
    int plane, y, x, q;
    char row[WORLD_WIDTH * 5];
    for(plane = 0; plane < NUM_PLANES; plane++) {
        for(y = 0; y < WORLD_HEIGHT; y++) {
            q = 0;
            for(x = 0; x < WORLD_WIDTH; x++) {
                int v = GET_SQUARE_EXPLORED(x, y, plane);
                q += snprintf(row + q, sizeof(row) - q, x ? ",%d" : "%d", v);
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _square_explored[%d].y%02d = %s",
                      point, plane, y, row);
        }
        STU_Log_Flush_All();
    }
}

/* CLAUDE: capture movement_mode_cost_maps (int8 move cost per square, 6 modes x
 * 2 planes) for OG byte-compare.  One [GD] line per (plane, mode, row). */
static void gd_dump_movement_mode_cost_maps(const char* point)
{
    int plane, m, y, x, q;
    char row[WORLD_WIDTH * 5];
    static const char* const mode_names[6] = {
        "UU_MvMd", "walking", "forester", "mountaineer", "swimming", "sailing"
    };
    for(plane = 0; plane < NUM_PLANES; plane++) {
        struct s_MOVE_COST_MAP* modes[6] = {
            &movement_mode_cost_maps[plane].UU_MvMd,
            &movement_mode_cost_maps[plane].walking,
            &movement_mode_cost_maps[plane].forester,
            &movement_mode_cost_maps[plane].mountaineer,
            &movement_mode_cost_maps[plane].swimming,
            &movement_mode_cost_maps[plane].sailing,
        };
        for(m = 0; m < 6; m++) {
            for(y = 0; y < WORLD_HEIGHT; y++) {
                q = 0;
                for(x = 0; x < WORLD_WIDTH; x++) {
                    int v = (int)modes[m]->moves2[(y * WORLD_WIDTH) + x];
                    q += snprintf(row + q, sizeof(row) - q, x ? ",%d" : "%d", v);
                }
                LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s movement_mode_cost_maps[%d].%s_y%02d = %s",
                          point, plane, mode_names[m], y, row);
            }
        }
        STU_Log_Flush_All();
    }
}

/* CLAUDE: capture connectivity_grid_land / connectivity_grid_sea (path grids, NUM_PLANES x 96 uint8) for
 * OG byte-compare.  SAMB_ptr = unsigned char*; one [GD] line per (table,plane). */
static void gd_dump_one_uu_tbl(const char* point, const char* name, const unsigned char* tbl)
{
    int plane, i, q;
    char row[96 * 5];
    for(plane = 0; plane < NUM_PLANES; plane++) {
        q = 0;
        for(i = 0; i < 96; i++) {
            int v = (int)tbl[(plane * 96) + i];
            q += snprintf(row + q, sizeof(row) - q, i ? ",%d" : "%d", v);
        }
        LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s %s[%d].vals = %s", point, name, plane, row);
    }
}

static void gd_dump_uu_tbls(const char* point)
{
    gd_dump_one_uu_tbl(point, "connectivity_grid_land", (unsigned char *)connectivity_grid_land);
    gd_dump_one_uu_tbl(point, "connectivity_grid_sea", (unsigned char *)connectivity_grid_sea);
    STU_Log_Flush_All();
}

void Init_New_Game(void)
{
    int16_t rivers = 0;
    int16_t tries = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    bldg_data_table = (struct s_BLDG_DATA *)LBX_Load_Data(builddat_lbx_file__MGC_ovr051, 0, 0, 36, 52);
    
    Set_Mouse_List(1, mouselist_mapgen);

    Copy_On_To_Off_Page();
    
    Set_Page_Off();
    
    FLIC_Draw(75, 67, newgame_BUILDWOR_map_build_bar_seg);

    Set_Font_Style(3, 3, ST_NULL, ST_NULL);

    Print(88, 76, str_BuildingTheWorlds);  // "Building The Worlds..."
    
    Clear_Fields();

    Deactivate_Auto_Function();
    
    Toggle_Pages();
    
    Copy_On_To_Off_Page();
    
    Init_Computer_Players();

    Draw_Building_The_Worlds(5);

    Init_Landmasses(ARCANUS_PLANE);

    Draw_Building_The_Worlds(10);

    Init_Landmasses(MYRROR_PLANE);

    gd_dump_landmasses("102_Init_Landmasses_L");   /* CLAUDE: _landmasses after Init_Landmasses */

    Draw_Building_The_Worlds(15);

    Generate_Landmasses(ARCANUS_PLANE);

    Draw_Building_The_Worlds(20);

    Generate_Landmasses(MYRROR_PLANE);

    gd_dump_landmasses("103_Generate_Landmasses_L");   /* CLAUDE: _landmasses after Generate_Landmasses */
    gd_dump_world_map("104_Landmasses_W");   /* CLAUDE: bisect map-divergence checkpoint */

    Draw_Building_The_Worlds(25);

    Translate_Heightmap_To_Base_Terrain_Types(ARCANUS_PLANE);

    Draw_Building_The_Worlds(30);

    Translate_Heightmap_To_Base_Terrain_Types(MYRROR_PLANE);

    gd_dump_world_map("105_BaseTerrain_W");   /* CLAUDE: bisect map-divergence checkpoint */

    Draw_Building_The_Worlds(35);

    Generate_Climate_Terrain_Types(ARCANUS_PLANE);

    Draw_Building_The_Worlds(40);

    Generate_Climate_Terrain_Types(MYRROR_PLANE);

    /* CLAUDE: full map now built (both planes climate-terraformed) -- capture
     * for OG byte-compare before Nodes/Towers/Lairs run. */
    gd_dump_world_map("106_ClimateTerrain_W");

    Draw_Building_The_Worlds(45);

    Generate_Nodes();

    gd_dump_nodes("107_Generate_Nodes_N");   /* CLAUDE: _NODES at end of Generate_Nodes */

    Draw_Building_The_Worlds(50);

    Rebalance_Node_Types(ARCANUS_PLANE);
    Rebalance_Node_Types(MYRROR_PLANE);

    gd_dump_nodes("108_Rebalance_Node_Types_N");   /* CLAUDE: _NODES after Rebalance */

    Draw_Building_The_Worlds(55);

    /* CLAUDE: map state immediately BEFORE Generate_Towers -- does the tower
     * loop read an identical map (so the divergence is internal to it) or not? */
    gd_dump_world_map("109_PreTowers_W");

    Generate_Towers();

    gd_dump_towers("110_Generate_Towers_T");   /* CLAUDE: _TOWERS after Generate_Towers */

    Draw_Building_The_Worlds(60);

    Extend_Islands(ARCANUS_PLANE);
    Extend_Islands(MYRROR_PLANE);

    gd_dump_landmasses("111_Extend_Islands_L");
    gd_dump_world_map("112_Extend_Islands_W");

    Generate_Lairs();

    gd_dump_lairs("113_Generate_Lairs_LR");   /* CLAUDE: _LAIRS after Generate_Lairs */

    Draw_Building_The_Worlds(65);

    _units = 0;

    Generate_Home_Cities();

    gd_dump_cities("114_Generate_Home_Cities_C");
    gd_dump_units("115_Generate_Home_Cities_U");
    

#ifdef STU_DEBUG
    assert(Validate_All_Cities() == ST_TRUE);
#endif

    Draw_Building_The_Worlds(70);

    Generate_Neutral_Cities(ARCANUS_PLANE);
    Generate_Neutral_Cities(MYRROR_PLANE);

    gd_dump_cities("116_Generate_Neutral_Cities_C");
    gd_dump_units("117_Generate_Neutral_Cities_U");

#ifdef STU_DEBUG
    assert(Validate_All_Neutral_Cities() == ST_TRUE);
#endif

    Draw_Building_The_Worlds(75);

    Generate_Terrain_Specials(ARCANUS_PLANE);
    Generate_Terrain_Specials(MYRROR_PLANE);
    gd_dump_terrain_specials("118_Generate_Terrain_Specials_TS");
    gd_dump_world_map("119_Generate_Terrain_Specials_W");

    Generate_Roads(ARCANUS_PLANE);
    Generate_Roads(MYRROR_PLANE);
    gd_dump_map_square_flags("120_Generate_Roads_F");

    Draw_Building_The_Worlds(80);

    Simtex_Autotiling();
    gd_dump_world_map("121_Simtex_Autotiling_W");

    Draw_Building_The_Worlds(85);

    for(rivers = 0; rivers < NUM_RIVERS; rivers++)
    {
        for(tries = 0; ((tries < 2000) && (Generate_River(ARCANUS_PLANE) == ST_FALSE)); tries++) { }
        for(tries = 0; ((tries < 2000) && (Generate_River(MYRROR_PLANE)  == ST_FALSE)); tries++) { }
    }
    gd_dump_world_map("122_Generate_River_W");

    River_Autotile(ARCANUS_PLANE);
    River_Autotile(MYRROR_PLANE);
    gd_dump_world_map("123_River_Autotile_W");

    Desert_Autotile();
    gd_dump_landmasses("124_Desert_Autotile_L");
    gd_dump_world_map("125_Desert_Autotile_W");

    Draw_Building_The_Worlds(85);

    Shuffle_Terrains();
    gd_dump_world_map("126_Shuffle_Terrains_W");

    Movement_Mode_Cost_Maps(ARCANUS_PLANE);
    Movement_Mode_Cost_Maps(MYRROR_PLANE);
    gd_dump_movement_mode_cost_maps("127_Movement_Mode_Cost_Maps_M");

    Draw_Building_The_Worlds(90);

    Build_Connectivity_Graphs(ARCANUS_PLANE);
    Build_Connectivity_Graphs(MYRROR_PLANE);
    /* CLAUDE 2026-06-24: lifecycle trace; companion logs at ALLOC.c (allocate) and LOADSAVE.c (after stu_fread).  See ALLOC.c for the why. */
    {
        char row[96 * 5];
        int plane, i, q;
        const unsigned char * tbls[2] = { (const unsigned char *)connectivity_grid_land, (const unsigned char *)connectivity_grid_sea };
        const char *         names[2] = { "connectivity_grid_land", "connectivity_grid_sea" };
        int t;
        LOG_INFO(LOG_CAT_GENERAL, "[connectivity_grids] event=calc  (after Build_Connectivity_Graphs for both planes)");
        for(t = 0; t < 2; t++)
        {
            for(plane = 0; plane < NUM_PLANES; plane++)
            {
                q = 0;
                for(i = 0; i < 96; i++)
                {
                    q += snprintf(row + q, sizeof(row) - q, i ? ",%d" : "%d", (int)tbls[t][(plane * 96) + i]);
                }
                LOG_TRACE(LOG_CAT_GENERAL, "[connectivity_grids] event=calc table=%s plane=%d bytes=%s", names[t], plane, row);
            }
        }
    }
    gd_dump_uu_tbls("128_CRP_NEWG_CreatePathGrids_U");

    Draw_Building_The_Worlds(95);

    Init_Square_Explored();
    gd_dump_square_explored("129_Init_Square_Explored_E");

    Animate_Oceans();
    gd_dump_world_map("130_Animate_Oceans_W");

    Set_Upper_Lair_Guardian_Count();
    gd_dump_lairs("131_Set_Upper_Lair_Guardian_Count_LR");

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}

// MGC o51p02
/**
 * @brief Mirrors initial lair guardian stack counts into the upper nibble.
 *
 * @details
 * Iterates over all lair records and transforms each guardian count byte into
 * a packed "had and left" representation by copying the current lower 4-bit
 * value into the upper 4 bits:
 * - `guard1_count = guard1_count | (guard1_count << 4)`
 * - `guard2_count = guard2_count | (guard2_count << 4)`
 *
 * This preserves the current count in the low nibble (remaining guardians)
 * while storing the same value in the high nibble (initial guardians).
 *
 * @return void
 *
 * @note Mutates `_LAIRS[0..NUM_LAIRS-1]` in place.
 * @note Intended for post-generation initialization of encounter tracking.
 */
void Set_Upper_Lair_Guardian_Count(void)
{
    int16_t itr_lairs = 0;
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    for(itr_lairs = 0; itr_lairs < NUM_LAIRS; itr_lairs++)
    {
        _LAIRS[itr_lairs].guard1_count = (_LAIRS[itr_lairs].guard1_count + (_LAIRS[itr_lairs].guard1_count << 4));
        _LAIRS[itr_lairs].guard2_count = (_LAIRS[itr_lairs].guard2_count + (_LAIRS[itr_lairs].guard2_count << 4));
    }
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
}


// MGC o51p03
/**
 * @brief Attempts to expand isolated node/tower island map squares into nearby land.
 *
 * @details
 * Scans interior map squares of the selected plane and identifies qualifying origin
 * map squares (sorcery/nature/chaos nodes or towers) that are surrounded by ocean in
 * cardinal directions. For qualifying map squares, performs a randomized series of
 * conversion attempts on nearby offsets to reduce one-map square island isolation.
 *
 * Candidate conversions are filtered by occupancy checks (no tower/node on the
 * target), terrain-neighborhood checks, and internal conversion rules.
 *
 * @param wp World plane index to process.
 *
 * @return void
 *
 * @warning Historical notes indicate known switch/indexing defects in this
 *          routine that can reduce the expected number of successful adds.
 * @note Uses RNG-driven branching; results vary between runs and map states.
 *
 * @see Square_Is_Ocean_NewGame
 * @see Square_Has_Tower_NewGame
 * @see Square_Is_Node_NewGame
 */
void Extend_Islands(int16_t wp)
{
    int16_t Grid_Index = 0;
    int16_t convert = 0;
    int16_t rnd_wy = 0;
    int16_t rnd_wx = 0;
    int16_t attempts = 0;
    int16_t square_has_tower = 0;
    int16_t itr_towers = 0;
    int16_t itr = 0;  // DNE in Dasm
    int16_t terrain_type = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr_wy = 1; itr_wy < (WORLD_HEIGHT - 1); itr_wy++)
    {
        for(itr_wx = 1; itr_wx < (WORLD_WIDTH - 1); itr_wx++)
        {
            terrain_type = p_world_map[wp][itr_wy][itr_wx];
            square_has_tower = ST_FALSE;
            for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
            {
                if(
                    (_TOWERS[itr_towers].wy == itr_wy)
                    &&
                    (_TOWERS[itr_towers].wx == itr_wx)
                )
                {
                    square_has_tower = ST_TRUE;
                }
            }
            if(
                (terrain_type == tt_SorceryNode)
                ||
                (terrain_type == tt_NatureNode)
                ||
                (terrain_type == tt_ChaosNode)
                ||
                (square_has_tower == ST_TRUE)
            )
            {
                if(
                    (Square_Is_Ocean_NewGame(itr_wx, (itr_wy - 1), wp) == ST_TRUE)
                    &&
                    (Square_Is_Ocean_NewGame((itr_wx - 1), itr_wy, wp) == ST_TRUE)
                    &&
                    (Square_Is_Ocean_NewGame((itr_wx + 1), itr_wy, wp) == ST_TRUE)
                    &&
                    (Square_Is_Ocean_NewGame(itr_wx, (itr_wy + 1), wp) == ST_TRUE)
                )
                {
                    if(Random(3) > 1)  // 66.666% chance to attempt conversion
                    {
                        attempts = (1 + Random(8));
                        for(itr = 0; itr < attempts; itr++)
                        {
                            rnd_wx = (Random(3) - 2); // { -1, 0, 1}
                            rnd_wy = (Random(3) - 2); // { -1, 0, 1}
                            if(
                                (rnd_wx != 0)
                                ||
                                (rnd_wy != 0)
                            )
                            {
                                if(
                                    (Square_Has_Tower_NewGame((itr_wx + rnd_wx), (itr_wy + rnd_wy)) == ST_FALSE)
                                    &&
                                    (Square_Is_Node_NewGame((itr_wx + rnd_wx), (itr_wy + rnd_wy), wp) == ST_FALSE)
                                )
                                {
                                    /*
                                        OGBUG (preserved, faithful to Dasm):
                                        Grid_Index was meant to map the 3x3 neighborhood (current + 8 adjacent
                                        squares) to {1,...,9}, which needs ((rnd_wy + 1) * 3) + (rnd_wx + 1) + 1.
                                        The OG instead uses the formula below, whose range is {-3,...,5}, NOT
                                        {1,...,9}  (BUG observed by drake189).

                                        Effect: only rnd values that land on Grid_Index 2/3/4/5 reach the switch,
                                        so an island extends only toward E (case 2), S (case 4), and conditionally
                                        SW (case 3).  N/NE/NW/W produce a negative Grid_Index (no matching case,
                                        skipped) and SE produces case 5 (never converts).  switch cases 1 and 6-9
                                        are therefore dead code.  Do NOT "fix" the formula -- preserve as-is.
                                    */
                                    Grid_Index = ((rnd_wy * 3) + rnd_wx + 1);  // {-3,...,3} + {-1,0,1} + 1 = {-3,...,5}
                                    convert = ST_FALSE;
                                    /*
                                        The Dasm dispatches on (Grid_Index - 1) through a jump table guarded by an
                                        unsigned `cmp 8; jbe`.  Switching on Grid_Index directly with cases 1-9
                                        (below) is equivalent: case N == Dasm jump-table index (N - 1), and a
                                        negative Grid_Index falls through to no conversion in both forms.
                                    */
                                    switch(Grid_Index)
                                    {
                                        case 0:
                                        {
                                            // DNE
                                        } break;
                                        case 1:
                                        {
                                            if(
                                                (p_world_map[wp][(itr_wy + rnd_wy + 1)][(itr_wx + rnd_wx)] == tt_Ocean)
                                                &&
                                                (p_world_map[wp][(itr_wy + rnd_wy)][(itr_wx + rnd_wx + 1)] == tt_Ocean)
                                            )
                                            {
                                                convert = ST_FALSE;
                                            }
                                            else
                                            {
                                                convert = ST_TRUE;
                                            }
                                        } break;
                                        case 2: { convert = ST_TRUE;  } break;
                                        case 3:
                                        {
                                            if(
                                                (p_world_map[wp][(itr_wy + rnd_wy + 1)][(itr_wx + rnd_wx)] == tt_Ocean)
                                                &&
                                                (p_world_map[wp][(itr_wy + rnd_wy)][(itr_wx + rnd_wx - 1)] == tt_Ocean)
                                            )
                                            {
                                                convert = ST_FALSE;
                                            }
                                            else
                                            {
                                                convert = ST_TRUE;
                                            }
                                        } break;
                                        case 4: { convert = ST_TRUE;  } break;
                                        case 5: { convert = ST_FALSE; } break;  /* intended "current square" (never convert); under the OGBUG, Grid_Index 5 is actually the SE neighbor */
                                        case 6: { convert = ST_TRUE;  } break;
                                        case 7:
                                        {
                                            if(
                                                (p_world_map[wp][(itr_wy + rnd_wy - 1)][(itr_wx + rnd_wx)] == tt_Ocean)
                                                &&
                                                (p_world_map[wp][(itr_wy + rnd_wy)][(itr_wx + rnd_wx + 1)] == tt_Ocean)
                                            )
                                            {
                                                convert = ST_FALSE;
                                            }
                                            else
                                            {
                                                convert = ST_TRUE;
                                            }
                                        } break;
                                        case 8: { convert = ST_TRUE;  } break;
                                        case 9:
                                        {
                                            if(
                                                (p_world_map[wp][(itr_wy + rnd_wy - 1)][(itr_wx + rnd_wx)] == tt_Ocean)
                                                &&
                                                (p_world_map[wp][(itr_wy + rnd_wy)][(itr_wx + rnd_wx - 1)] == tt_Ocean)
                                            )
                                            {
                                                convert = ST_FALSE;
                                            }
                                            else
                                            {
                                                convert = ST_TRUE;
                                            }
                                        } break;
                                    }
                                    if(convert == ST_TRUE)
                                    {
                                        Build_Landmass(wp, (itr_wx + rnd_wx), (itr_wy + rnd_wy));
                                        p_world_map[wp][(itr_wy + rnd_wy)][(itr_wx + rnd_wx)] = tt_Grasslands1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p04
/**
 * @brief Places all towers of wizardry and ensures tower tiles are land on both planes.
 *
 * @details
 * Repeatedly samples candidate map squares and accepts a location only when all
 * placement constraints pass. Accepted constraints include minimum separation
 * from previously placed towers and minimum distance from all existing nodes.
 *
 * The candidate acceptance gate intentionally preserves original behavior:
 * a location is retried when the sampled square is ocean on both planes and
 * the random escape roll does not trigger. As a result, rare ocean placements
 * can still be accepted via the escape path.
 *
 * Once a valid location is found, the function writes tower coordinates and
 * owner state into `_TOWERS`, calls `Build_Landmass()` for Arcanus and Myrror
 * at that coordinate, and sets both plane tiles to `tt_Grasslands1`.
 *
 * @return void
 *
 * @note Mutates global tower records and world terrain state.
 * @note Preserves historical retry/relaxation behavior where `tries` grows,
 *       then relaxes `min_distance` after threshold pressure.
 * @warning Uses RNG-driven placement and may perform many retries before
 *          placing all towers.
 *
 * @see Build_Landmass
 * @see Delta_XY_With_Wrap
 */
void Generate_Towers(void)
{
    int16_t tries = 0;  /* CI: OG leaves this auto stack local [bp-8] uninitialized; injected from capture below (gd_ci "tries"). */
    int16_t min_distance = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t itr1 = 0;
    int16_t itr2 = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    /* CI: OG leaves `tries` (auto stack local [bp-8]) uninitialized; it comes up
     * as leftover stack memory (13173 in the captured run) and drives an immediate
     * min_distance relaxation.  Inject the captured value instead of hardcoding it. */
    {
        long ci_tries;
        if(gd_ci_get("tries", "gen_towers", &ci_tries, 1) == 1) {
            tries = (int16_t)ci_tries;
        }
    }

    min_distance = 10;

    itr1 = 0;
    while(itr1 < NUM_TOWERS)
    {

        tries++;  /* OGBUG  Tries is uninitialized in the OG (stack leftover 13173, not 0) — see init above */
        if(tries > 500)
        {
            tries = 450;
            min_distance--;
        }

        /* Generate random coordinates within map margins */
        wx = (2 + Random(54));
        wy = (2 + Random(34));

        /* Check if selected square is Ocean on both planes (Arcanus and Myrror) */
        if(
            (p_world_map[ARCANUS_PLANE][wy][wx] == tt_Ocean)
            &&
            (p_world_map[MYRROR_PLANE][wy][wx] == tt_Ocean)
        )
        {
            if(Random(40) > 1)  /* 2.5%  1:40 */
            {
                continue;
            }
        }

        /* Distance check against previously placed towers */
        for(itr2 = 0; itr2 < itr1; itr2++)
        {
            if(Delta_XY_With_Wrap(wx, wy, _TOWERS[itr2].wx, _TOWERS[itr2].wy, WORLD_WIDTH) < min_distance)
            {
                break;
            }
        }
        if(itr2 < itr1)
        {
            continue;
        }   /* too close to a tower -> re-roll */

        /* Distance check against special Nodes */
        for(itr2 = 0; itr2 < NUM_NODES; itr2++)
        {
            if(Delta_XY_With_Wrap(wx, wy, _NODES[itr2].wx, _NODES[itr2].wy, WORLD_WIDTH) < 4)
            {
                break;
            }
        }
        if(itr2 < NUM_NODES)
        {
            continue;
        }  /* too close to a node -> re-roll */

        /* Successfully found a location; update tower data */
        _TOWERS[itr1].wx = (int8_t)wx;
        _TOWERS[itr1].wy = (int8_t)wy;
        _TOWERS[itr1].owner_idx = ST_UNDEFINED;

        /* Ensure tower is placed on land by generating landmass around it */
        Build_Landmass(ARCANUS_PLANE, wx, wy);
        Build_Landmass(MYRROR_PLANE, wx, wy);

        /* Set the specific tower tile to Grasslands on both planes */
        p_world_map[ARCANUS_PLANE][wy][wx] = tt_Grasslands1;
        p_world_map[MYRROR_PLANE][wy][wx] = tt_Grasslands1;

        itr1++;
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


/**
 * @brief Places each wizard's starting fortress and capital, then creates starting garrison units.
 *
 * @details
 * Runs the home-city placement phase of world generation for all players. The routine repeatedly
 * searches for valid fortress coordinates on the appropriate plane for each wizard, rejecting map
 * squares that are ocean, too close to previously placed fortresses, or too close to nodes, towers,
 * or lairs. It also rejects locations whose estimated maximum city size is too low for the intended
 * starting city population threshold.
 *
 * After all fortress locations are accepted, the function initializes one capital city per active
 * player in `_CITIES`, assigns the human-selected race to the human player, chooses AI starting races,
 * initializes core city state and starting buildings, assigns a random race-appropriate city name,
 * and finally creates the initial spearmen/swordsmen garrison units at each capital.
 *
 * @return void
 *
 * @note The plane selection is driven by each player's `myrran` flag.
 * @note This routine mutates global fortress, city, and unit state including `_FORTRESSES`,
 * `_CITIES`, `_cities`, and the new-game unit table via `Create_Unit_NewGame()`.
 * @warning The placement logic intentionally uses retry loops and distance relaxation; historical
 * behavior notes in the body indicate known quirks and bug-prone edge cases in some checks.
 *
 * @see City_Maximum_Size_NewGame
 * @see Random_City_Name_By_Race_NewGame
 * @see Create_Unit_NewGame
 */
void Generate_Home_Cities(void)
{
    int16_t max_pop_failures = 0;
    int16_t minimum_site_distance = 0;  // used for _NODES[], _TOWERS[], _LAIRS[]
    int16_t minimum_fortress_distance = 0;
    int16_t itr = 0;
    int16_t Invalid = 0;
    int16_t Tries_Per_Distance = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t wp = 0;
    int16_t niu_fortresses[NUM_PLANES] = { 0, 0 };  // tracks fortress record populations? 1:1 with player_idx in the players loop?
    int16_t player_idx = 0;
    int16_t unit_type = 0;
    int16_t bldg_idx = 0;
    int16_t DBG_pop_min = 0;
    int16_t DBG_pop_max = 0;
// #ifdef STU_DEBUG
    int16_t DBG_Invalid_Reason = 0;
    int16_t DBG_Invalid_Reason_1_Count = 0;  // Ocean
    int16_t DBG_Invalid_Reason_2_Count = 0;  // Fortress
    int16_t DBG_Invalid_Reason_3_Count = 0;  // Node
    int16_t DBG_Invalid_Reason_4_Count = 0;  // Tower
    int16_t DBG_Invalid_Reason_5_Count = 0;  // Lair
    int16_t DBG_Invalid_Reason_6_Count = 0;  // Max Pop
    int16_t DBG_Loop_Location[7] = { 0, 0, 0, 0, 0, 0, 0 };
// #endif

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    minimum_fortress_distance = 16;
    minimum_site_distance = 8;

    while(1)
    {
Loop_Distances:

        minimum_fortress_distance--;
        minimum_site_distance--;
        SETMIN(minimum_site_distance, 1);
        SETMIN(minimum_fortress_distance, 10);
        Tries_Per_Distance = 0;
        niu_fortresses[ARCANUS_PLANE] = 0;
        niu_fortresses[MYRROR_PLANE] = 0;
        max_pop_failures = 0;
        DBG_Invalid_Reason_1_Count = 0;
        DBG_Invalid_Reason_2_Count = 0;
        DBG_Invalid_Reason_3_Count = 0;
        DBG_Invalid_Reason_4_Count = 0;
        DBG_Invalid_Reason_5_Count = 0;
        DBG_Invalid_Reason_6_Count = 0;

        while(1)
        {
Loop_MaxPopTries:

            /* OGBUG  there are only at most 5 players, should iter over _num_players */
            for(player_idx = 0; player_idx < NUM_PLAYERS; player_idx++)
            {

                if(Tries_Per_Distance < 1000)
                {
// jump back here if the location is invalid
// jump back here if the max pop is too low
Loop_Location_1:
                    Tries_Per_Distance++;
                    wp = ARCANUS_PLANE;
                    if(_players[player_idx].myrran != ST_FALSE)
                    {
                        wp = MYRROR_PLANE;
                    }
                    wx = (2 + Random((WORLD_WIDTH  - 6)));  // { 3, ..., 56}
                    wy = (2 + Random((WORLD_HEIGHT - 6)));  // { 3, ..., 36}

                    Invalid = ST_FALSE;
                    DBG_Invalid_Reason = 0;

                    if(GET_TERRAIN_TYPE(wx, wy, wp) == tt_Ocean)
                    {
                        Invalid = ST_TRUE;
                        DBG_Invalid_Reason = 1;
                        DBG_Invalid_Reason_1_Count++;
                    }

                    for(bldg_idx = 0; bldg_idx < player_idx; bldg_idx++)
                    {
                        if(_FORTRESSES[bldg_idx].wp == wp)
                        {
                            if(Delta_XY_With_Wrap(wx, wy, _FORTRESSES[bldg_idx].wx, _FORTRESSES[bldg_idx].wy, WORLD_WIDTH) < minimum_fortress_distance)
                            {
                                Invalid = ST_TRUE;
                                DBG_Invalid_Reason = 2;
                                DBG_Invalid_Reason_2_Count++;
                            }
                        }
                    }

                    /* ¿ OGBUG  because the distance is halved, same map square is not excluded ? */
                    for(bldg_idx = 0; bldg_idx < NUM_NODES; bldg_idx++)
                    {
                        if(_NODES[bldg_idx].wp == wp)
                        {

                            if(Delta_XY_With_Wrap(wx, wy, _NODES[bldg_idx].wx, _NODES[bldg_idx].wy, WORLD_WIDTH) < (minimum_site_distance / 2))
                            {
                                Invalid = ST_TRUE;
                                DBG_Invalid_Reason = 3;
                                DBG_Invalid_Reason_3_Count++;
                            }
                        }
                    }

                    /* ¿ OGBUG  because the distance is halved, same map square is not excluded ? */
                    for(bldg_idx = 0; bldg_idx < NUM_TOWERS; bldg_idx++)
                    {

                        if(Range(wx, wy, _TOWERS[bldg_idx].wx, _TOWERS[bldg_idx].wy) < (minimum_site_distance / 2))
                        {

                            Invalid = ST_TRUE;
                            DBG_Invalid_Reason = 4;
                            DBG_Invalid_Reason_4_Count++;

                        }

                    }

                    /* ¿ OGBUG  because the distance is halved, same map square is not excluded ? */
                    for(bldg_idx = 0; bldg_idx < NUM_LAIRS; bldg_idx++)
                    {
                        if(_LAIRS[bldg_idx].wp == wp)
                        {

                            if(Delta_XY_With_Wrap(wx, wy, _LAIRS[bldg_idx].wx, _LAIRS[bldg_idx].wy, WORLD_WIDTH) < (minimum_site_distance / 2))
                            {
                                Invalid = ST_TRUE;
                                DBG_Invalid_Reason = 5;
                                DBG_Invalid_Reason_5_Count++;
                            }
                        }
                    }

                    /* CLAUDE 2026-06-01: per-iteration trace of the
                     * location-pick retry loop.  Emitted on every
                     * iteration whether the picked (wx,wy) was valid
                     * or not.  Use when ReMoM's New Game iterates this
                     * loop more times than OG MAGIC.EXE did and you
                     * need to see, iteration by iteration, what
                     * Random()-picked location was rejected and which
                     * of the five bound checks rejected it.
                     *
                     * Fields:
                     *   player=N     placement attempt is for which player slot
                     *   try=N        Tries_Per_Distance at this iteration
                     *   wx,wy,wp     the Random-picked location
                     *   invalid=0/1  rejected this iteration?
                     *   reason=N     1=Ocean, 2=NearFortress, 3=NearNode,
                     *                4=NearTower, 5=NearLair, 6=PopTooLow,
                     *                0=accepted
                     */
                    LOG_INFO(LOG_CAT_GENERAL, "[LOC_PICK] player=%d try=%d wp=%d wx=%d wy=%d invalid=%d reason=%d", (int)player_idx, (int)Tries_Per_Distance, (int)wp, (int)wx, (int)wy, (int)Invalid, (int)DBG_Invalid_Reason);

                    if(
                        (Tries_Per_Distance < 1000)
                        &&
                        (Invalid == ST_TRUE)
                    )
                    {
                        DBG_Loop_Location[DBG_Invalid_Reason]++;
                        goto Loop_Location_1;
                    }

                    // if((8 - (player_idx / 3)) < City_Maximum_Size_NewGame(wx, wy, wp))
                    // NOTE(JimBalcomb,20260328): this calculation could just be making it easier to find a valid location as more players are placed
                    DBG_pop_min = (8 - (player_idx / 3));
                    DBG_pop_max = City_Maximum_Size_NewGame(wx, wy, wp);
                    /* CLAUDE */ // BUG FIX: comparison was inverted — was rejecting valid locations and accepting invalid ones
                    // check if the estimated maximum city population for this map square is too low to support the home city population (absolute minimum is 7)
                    if(DBG_pop_min > DBG_pop_max)
                    {
                        DBG_Invalid_Reason = 6;
                        DBG_Invalid_Reason_6_Count++;
                        max_pop_failures++;
                        if(max_pop_failures > 500)
                        {
                            LOG_INFO(LOG_CAT_GENERAL, "[LOC_GIVEUP_POP] player=%d max_pop_failures>%d  min_fortress_dist=%d min_site_dist=%d  reasons: Ocean=%d Fortress=%d Node=%d Tower=%d Lair=%d PopTooLow=%d", (int)player_idx, 500, (int)minimum_fortress_distance, (int)minimum_site_distance, (int)DBG_Invalid_Reason_1_Count, (int)DBG_Invalid_Reason_2_Count, (int)DBG_Invalid_Reason_3_Count, (int)DBG_Invalid_Reason_4_Count, (int)DBG_Invalid_Reason_5_Count, (int)DBG_Invalid_Reason_6_Count);
                            STU_DEBUG_BREAK();
                            goto Loop_MaxPopTries;
                        }
                        else
                        {
                            DBG_Loop_Location[DBG_Invalid_Reason]++;
                            goto Loop_Location_1;
                        }
                    }
                    else
                    {

                        _FORTRESSES[player_idx].wx = (int8_t)wx;
                        _FORTRESSES[player_idx].wy = (int8_t)wy;
                        _FORTRESSES[player_idx].wp = (int8_t)wp;
                        _FORTRESSES[player_idx].active = ST_TRUE;
                        niu_fortresses[wp]++;

                    }

                }

            }

            if(Tries_Per_Distance >= 1000)
            {
                /* CLAUDE 2026-06-14: failed to place at this distance — log reasons
                   then shrink min distances and try again at Loop_Distances. */
                LOG_INFO(LOG_CAT_GENERAL, "[LOC_GIVEUP_DIST] tries>=%d at player=%d  min_fortress_dist=%d min_site_dist=%d  reasons: Ocean=%d Fortress=%d Node=%d Tower=%d Lair=%d PopTooLow=%d", 1000, (int)player_idx, (int)minimum_fortress_distance, (int)minimum_site_distance, (int)DBG_Invalid_Reason_1_Count, (int)DBG_Invalid_Reason_2_Count, (int)DBG_Invalid_Reason_3_Count, (int)DBG_Invalid_Reason_4_Count, (int)DBG_Invalid_Reason_5_Count, (int)DBG_Invalid_Reason_6_Count);
                goto Loop_Distances;
            }



// @@Do_The_Cities:



            _cities = 0;  /* NOTE: first settings of _cities */

            for(itr = 0; itr < _num_players; itr++)
            {

/*
    BEGIN:  City Race
*/
                if(itr == 0)
                {
                    _CITIES[_cities].race = (int8_t)NEWG_Clicked_Race;
                }
                else
                {
                    if(_players[itr].myrran != ST_FALSE)
                    {
                        switch((Random(5) - 1))
                        {
                            case 0: { _CITIES[_cities].race = rt_Beastmen;  } break;
                            case 1: { _CITIES[_cities].race = rt_Dark_Elf;  } break;
                            case 2: { _CITIES[_cities].race = rt_Draconian; } break;
                            case 3: { _CITIES[_cities].race = rt_Dwarf;     } break;
                            case 4: { _CITIES[_cities].race = rt_Troll;     } break;
                        }
                    }
                    else
                    {
                        if(_difficulty <= god_Normal)
                        {
                            switch((Random(9) - 1))
                            {
                                case 0: { _CITIES[_cities].race = rt_Barbarian; } break;
                                case 1: { _CITIES[_cities].race = rt_Gnoll;     } break;
                                case 2: { _CITIES[_cities].race = rt_Halfling;  } break;
                                case 3: { _CITIES[_cities].race = rt_High_Elf;  } break;
                                case 4: { _CITIES[_cities].race = rt_High_Men;  } break;
                                case 5: { _CITIES[_cities].race = rt_Klackon;   } break;
                                case 6: { _CITIES[_cities].race = rt_Lizardman; } break;
                                case 7: { _CITIES[_cities].race = rt_Nomad;     } break;
                                case 8: { _CITIES[_cities].race = rt_Orc;       } break;
                            }
                        }
                        else
                        {
                            switch((Random(5) - 1))
                            {
                                case 0: { _CITIES[_cities].race = rt_Klackon;  } break;
                                case 1: { _CITIES[_cities].race = rt_High_Elf; } break;
                                case 2: { _CITIES[_cities].race = rt_Halfling; } break;
                                case 3: { _CITIES[_cities].race = rt_High_Men; } break;
                                case 4: { _CITIES[_cities].race = rt_Nomad;    } break;
                            }
                        }
                    }
                    /* OGBUG  wx/wy/wp here are leftover from the last fortress placed in the location loop, NOT this city's own square (_FORTRESSES[itr]); so for every city except the last-placed one, the High Elf forest requirement is tested against the wrong tile. Faithful to the asm (loc_44375 pushes [bp+wx]/[bp+wy]/[bp+wp]). */
                    if(
                        (_CITIES[_cities].race == rt_High_Elf)
                        &&
                        (Square_Is_Forest_NewGame(wx, wy, wp) == ST_FALSE)
                    )
                    {
                        /* OGBUG  taking this jump lands in a bad state: the location loop has already run to completion, so player_idx == NUM_PLAYERS (6) here (this loop iterates itr, not player_idx). Re-entering Loop_Location_1 does a junk location pick for the nonexistent 6th slot (_players[6] / _FORTRESSES[6] — the B1 over-iteration) before the whole city loop restarts at _cities=0. Faithful to the asm (jmp @@Loop_Location with player_idx left at 6). */
                        goto Loop_Location_1;
                    }
                }
/*
    END:  City Race
*/

                _CITIES[_cities].wx = _FORTRESSES[itr].wx;
                _CITIES[_cities].wy = _FORTRESSES[itr].wy;
                _CITIES[_cities].wp = _FORTRESSES[itr].wp;
                _CITIES[_cities].owner_idx = (int8_t)itr;
                _CITIES[_cities].size = 1;  // CTY_Hamlet ... ~== (4 / 4)
                _CITIES[_cities].population = 4;
                _CITIES[_cities].bldg_cnt = 0;
                _CITIES[_cities].Prod_Accu = 0;
                _CITIES[_cities].Pop_10s = 0;
                if(itr == HUMAN_PLAYER_IDX)
                {
                    _CITIES[_cities].construction = bt_Housing;
                }
                else
                {
                    _CITIES[_cities].construction = bt_AUTOBUILD;
                }
                _CITIES[_cities].enchantments[WALL_OF_FIRE] = ST_FALSE;
                _CITIES[_cities].enchantments[WALL_OF_DARKNESS] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[CHAOS_RIFT] = ST_FALSE;
                _CITIES[_cities].enchantments[DARK_RITUALS] = ST_FALSE;
                _CITIES[_cities].enchantments[EVIL_PRESENCE] = ST_FALSE;
                _CITIES[_cities].enchantments[CURSED_LANDS] = ST_FALSE;
                _CITIES[_cities].enchantments[PESTILENCE] = ST_FALSE;
                _CITIES[_cities].enchantments[CLOUD_OF_SHADOW] = ST_FALSE;
                _CITIES[_cities].enchantments[ALTAR_OF_BATTLE] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[FAMINE] = ST_FALSE;
                _CITIES[_cities].enchantments[FLYING_FORTRESS] = ST_FALSE;
                _CITIES[_cities].enchantments[DEATH_WARD] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[CHAOS_WARD] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[LIFE_WARD] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[SORCERY_WARD] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[NATURE_WARD] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[NATURES_EYE] = ST_FALSE;
                _CITIES[_cities].enchantments[EARTH_GATE] = ST_FALSE;
                _CITIES[_cities].enchantments[STREAM_OF_LIFE] = ST_FALSE;
                _CITIES[_cities].enchantments[GAIAS_BLESSING] = ST_FALSE;
                _CITIES[_cities].enchantments[INSPIRATIONS] = ST_FALSE;
                _CITIES[_cities].enchantments[PROSPERITY] = ST_FALSE;
                _CITIES[_cities].enchantments[ASTRAL_GATE] = ST_FALSE;
                _CITIES[_cities].enchantments[HEAVENLY_LIGHT] = ST_FALSE;
                _CITIES[_cities].enchantments[CONSECRATION] = ST_FALSE;
                _CITIES[_cities].enchantments[NIGHTSHADE] = ST_FALSE;
                _CITIES[_cities].farmer_count = 3;
                for(bldg_idx = 0; bldg_idx < NUM_BUILDINGS; bldg_idx++)
                {
                    _CITIES[_cities].bldg_status[bldg_idx] = bs_NotBuilt;
                }
                _CITIES[_cities].bldg_status[bt_NONE] = bs_Replaced;
                _CITIES[_cities].bldg_status[bt_Smithy] = bs_Built;
                _CITIES[_cities].bldg_status[bt_Barracks] = bs_Built;
                _CITIES[_cities].bldg_status[bt_BuildersHall] = bs_Built;

                Random_City_Name_By_Race_NewGame(_CITIES[_cities].race, _CITIES[_cities].name);

                _cities++;

            }

            // if we get here, we're Done-Done?!?
            goto Done_Done;

        }

    }



Done_Done:

    LOG_INFO(LOG_CAT_GENERAL, "Done_Done:");

    for(itr = 0; itr < _num_players; itr++)
    {
        switch(_CITIES[itr].race)
        {
            case rt_Barbarian: { unit_type = ut_BarbSpearmen;   } break;
            case rt_Beastmen:  { unit_type = ut_BeastSpearmen;  } break;
            case rt_Dark_Elf:  { unit_type = ut_DrowSpearmen;   } break;
            case rt_Draconian: { unit_type = ut_DracSpearmen;   } break;
            case rt_Dwarf:     { unit_type = ut_DwarfSwordsmen; } break;
            case rt_Gnoll:     { unit_type = ut_GnollSpearmen;  } break;
            case rt_Halfling:  { unit_type = ut_HflngSpearmen;  } break;
            case rt_High_Elf:  { unit_type = ut_HElfSpearmen;   } break;
            case rt_High_Men:  { unit_type = ut_HMenSpearmen;   } break;
            case rt_Klackon:   { unit_type = ut_KlckSpearmen;   } break;
            case rt_Lizardman: { unit_type = ut_LizSpearmen;    } break;
            case rt_Nomad:     { unit_type = ut_NmdSpearmen;    } break;
            case rt_Orc:       { unit_type = ut_OrcSpearmen;    } break;
            case rt_Troll:     { unit_type = ut_TrlSpearmen;    } break;
        }
        if(_CITIES[itr].race != rt_Dwarf)
        {
            Create_Unit_NewGame(unit_type, itr, _CITIES[itr].wx, _CITIES[itr].wy, _CITIES[itr].wp, itr);
        }
    }
    for(itr = 0; itr < _num_players; itr++)
    {
        switch(_CITIES[itr].race)
        {
            case rt_Barbarian: { unit_type = ut_BarbSwordsmen;  } break;
            case rt_Beastmen:  { unit_type = ut_BeastSwordsmen; } break;
            case rt_Dark_Elf:  { unit_type = ut_DrowSwordsmen;  } break;
            case rt_Draconian: { unit_type = ut_DracSwordsmen;  } break;
            case rt_Dwarf:     { unit_type = ut_DwarfSwordsmen; } break;
            case rt_Gnoll:     { unit_type = ut_GnollSwordsmen; } break;
            case rt_Halfling:  { unit_type = ut_HflngSwordsmen; } break;
            case rt_High_Elf:  { unit_type = ut_HElfSwordsmen;  } break;
            case rt_High_Men:  { unit_type = ut_HMenSwordsmen;  } break;
            case rt_Klackon:   { unit_type = ut_KlckSwordsmen;  } break;
            case rt_Lizardman: { unit_type = ut_LizSwordsmen;   } break;
            case rt_Nomad:     { unit_type = ut_NmdSwordsmen;   } break;
            case rt_Orc:       { unit_type = ut_OrcSwordsmen;   } break;
            case rt_Troll:     { unit_type = ut_TrlSwordsmen;   } break;
        }
        if(_CITIES[itr].race == rt_Dwarf)
        {
            Create_Unit_NewGame(unit_type, itr, _CITIES[itr].wx, _CITIES[itr].wy, _CITIES[itr].wp, itr);
        }
        Create_Unit_NewGame(unit_type, itr, _CITIES[itr].wx, _CITIES[itr].wy, _CITIES[itr].wp, itr);
    }

    // @@Done

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}
/* GEMINI */
#if 0
void Generate_Home_City__GEMINI(void)
{
    int max_pop_failures;           /* [bp-18h] */
    int minimum_site_distance;      /* [bp-16h] */
    int minimum_fortress_distance;  /* [bp-14h] */
    int itr;                        /* [bp-12h] */
    int Invalid;                    /* [bp-10h] */
    int Tries_Per_Distance;         /* [bp-0Eh] */
    int wy;                         /* [bp-0Ch] */
    int wx;                         /* [bp-0Ah] */
    int wp;                         /* [bp-08h] */
    int UU_Fortresses[2];           /* [bp-06h] */
    int player_idx;                 /* [bp-02h] */
    int unit_type;                  /* itr2 */
    int bldg_idx;                   /* itr1 */

    minimum_fortress_distance = 16;
    minimum_site_distance = 8;

@@Loop_Distances:
    minimum_fortress_distance--;
    minimum_site_distance--;

    if(minimum_site_distance < 1)
    {
        minimum_site_distance = 1;
    }

    if(minimum_fortress_distance < 10)
    {
        minimum_fortress_distance = 10;
    }

    Tries_Per_Distance = 0;
    UU_Fortresses[0] = 0;
    UU_Fortresses[1] = 0;
    max_pop_failures = 0;

@@Loop_CapitalTries:
    player_idx = 0;

@@Loop_Players:
    while(player_idx < 6)
    {
@@Loop_Location:
        Tries_Per_Distance++;
        wp = 0;

        /* Check if player starts on Myrror */
        if(_players[player_idx].myrran != e_ST_FALSE)
        {
            wp = 1;
        }

        wx = Random(54) + 2;
        wy = Random(34) + 2;
        Invalid = e_ST_FALSE;

        /* Check if terrain is Ocean */
        if(_world_maps[wp * (WORLD_WIDTH * e_WORLD_HEIGHT) + wy * WORLD_WIDTH + wx] == tt_Ocean1)
        {
            Invalid = e_ST_TRUE;
        }

        /* Check distance to other players' fortresses */
        if(Invalid == e_ST_FALSE)
        {
            for(bldg_idx = 0; bldg_idx < player_idx; bldg_idx++)
            {
                if(_FORTRESSES[bldg_idx].wp == wp)
                {
                    if(Delta_XY_With_Wrap(wx, wy, _FORTRESSES[bldg_idx].wx, _FORTRESSES[bldg_idx].wy, WORLD_WIDTH) < minimum_fortress_distance)
                    {
                        Invalid = e_ST_TRUE;
                        break;
                    }
                }
            }
        }

        /* Check distance to Magic Nodes */
        if(Invalid == e_ST_FALSE)
        {
            for(bldg_idx = 0; bldg_idx < NUM_NODES; bldg_idx++)
            {
                if(_NODES[bldg_idx].wp == wp)
                {
                    if(Delta_XY_With_Wrap(wx, wy, _NODES[bldg_idx].wx, _NODES[bldg_idx].wy, WORLD_WIDTH) < (minimum_site_distance / 2))
                    {
                        Invalid = e_ST_TRUE;
                        break;
                    }
                }
            }
        }

        /* Check distance to Towers of Wizardry */
        if(Invalid == e_ST_FALSE)
        {
            for(bldg_idx = 0; bldg_idx < NUM_TOWERS; bldg_idx++)
            {
                /* Towers exist on both planes, no wp check? */
                if(Range(wx, wy, _TOWERS[bldg_idx].wx, _TOWERS[bldg_idx].wy) < (minimum_site_distance / 2))
                {
                    Invalid = e_ST_TRUE;
                    break;
                }
            }
        }

        /* Check distance to Lairs */
        if(Invalid == e_ST_FALSE)
        {
            for(bldg_idx = 0; bldg_idx < NUM_LAIRS; bldg_idx++)
            {
                if(_LAIRS[bldg_idx].wp == wp)
                {
                    if(Delta_XY_With_Wrap(wx, wy, _LAIRS[bldg_idx].wx, _LAIRS[bldg_idx].wy, WORLD_WIDTH) < (minimum_site_distance / 2))
                    {
                        Invalid = e_ST_TRUE;
                        break;
                    }
                }
            }
        }

        if(Tries_Per_Distance < 1000 && Invalid == e_ST_TRUE)
        {
            goto @@Loop_Location;
        }

        /* Verify city maximum potential size */
        if(City_Maximum_Size_NewGame(wx, wy, wp) < (8 - (player_idx / 3)))
        {
            max_pop_failures++;
            if(max_pop_failures <= 500)
            {
                goto @@Loop_Location;
            }
            else
            {
                goto @@Loop_CapitalTries;
            }
        }

@@AddFortress:
        _FORTRESSES[player_idx].wx = (unsigned char)wx;
        _FORTRESSES[player_idx].wy = (unsigned char)wy;
        _FORTRESSES[player_idx].wp = (unsigned char)wp;
        _FORTRESSES[player_idx].active = e_ST_TRUE;

        UU_Fortresses[wp]++;
        player_idx++;

        if(Tries_Per_Distance >= 1000)
        {
            break;
        }
    }

    if(Tries_Per_Distance >= 1000)
    {
        goto @@Loop_Distances;
    }

@@Do_The_Cities:
    _cities = 0;
    for(itr = 0; itr < _num_players; itr++)
    {
        /* Determine Race for the city */
        if(itr == 0)
        {
            _CITIES[_cities].race = (unsigned char)NEWG_Clicked_Race;
        }
        else
        {
            if(_players[itr].myrran != 0)
            {
                /* Myrran AI Races */
                switch (Random(5) - 1)
                {
                case 0: _CITIES[_cities].race = rt_Beastmen; break;
                case 1: _CITIES[_cities].race = rt_Dark_Elf; break;
                case 2: _CITIES[_cities].race = rt_Draconian; break;
                case 3: _CITIES[_cities].race = rt_Dwarf; break;
                case 4: _CITIES[_cities].race = rt_Troll; break;
                }
            }
            else if(_difficulty > god_Normal)
            {
                /* Arcanus Hard AI Races */
                switch (Random(5) - 1)
                {
                case 0: _CITIES[_cities].race = rt_Klackon; break;
                case 1: _CITIES[_cities].race = rt_High_Elf; break;
                case 2: _CITIES[_cities].race = rt_Halfling; break;
                case 3: _CITIES[_cities].race = rt_High_Man; break;
                case 4: _CITIES[_cities].race = rt_Nomad; break;
                }
            }
            else
            {
                /* Arcanus Normal AI Races */
                switch (Random(9) - 1)
                {
                case 0: _CITIES[_cities].race = rt_Barbarian; break;
                case 1: _CITIES[_cities].race = rt_Gnoll; break;
                case 2: _CITIES[_cities].race = rt_Halfling; break;
                case 3: _CITIES[_cities].race = rt_High_Elf; break;
                case 4: _CITIES[_cities].race = rt_High_Man; break;
                case 5: _CITIES[_cities].race = rt_Klackon; break;
                case 6: _CITIES[_cities].race = rt_Lizardman; break;
                case 7: _CITIES[_cities].race = rt_Nomad; break;
                case 8: _CITIES[_cities].race = rt_Orc; break;
                }
            }

            /* High Elves must start in a Forest */
            if(_CITIES[_cities].race == rt_High_Elf)
            {
                if(!Square_Is_Forest_NewGame(_FORTRESSES[itr].wx, _FORTRESSES[itr].wy, _FORTRESSES[itr].wp))
                {
                    /* Failed forest check, restart placement for all players */
                    goto @@Loop_Location;
                }
            }
        }

        /* Initialize City Structure */
        _CITIES[_cities].wx = _FORTRESSES[itr].wx;
        _CITIES[_cities].wy = _FORTRESSES[itr].wy;
        _CITIES[_cities].wp = _FORTRESSES[itr].wp;
        _CITIES[_cities].owner_idx = (unsigned char)itr;
        _CITIES[_cities].size = CTY_Hamlet;
        _CITIES[_cities].population = 4;
        _CITIES[_cities].Building_Count = 0;
        _CITIES[_cities].Prod_Accu = 0;
        _CITIES[_cities].Pop_10s = 0;

        if(itr == 0)
        {
            _CITIES[_cities].construction = bt_Housing;
        }
        else
        {
            _CITIES[_cities].construction = bt_AUTOBUILD;
        }

        /* Clear Enchantments */
        _CITIES[_cities].Enchants.Wall_of_Fire = 0;
        _CITIES[_cities].Enchants.Wall_of_Darkness = 0;
        _CITIES[_cities].Enchants._Chaos_Rift = 0;
        _CITIES[_cities].Enchants.Dark_Rituals = 0;
        _CITIES[_cities].Enchants._Evil_Presence = 0;
        _CITIES[_cities].Enchants._Cursed_Lands = 0;
        _CITIES[_cities].Enchants._Pestilence = 0;
        _CITIES[_cities].Enchants.Cloud_of_Shadow = 0;
        _CITIES[_cities].Enchants.Altar_of_Battle = 0;
        _CITIES[_cities].Enchants._Famine = 0;
        _CITIES[_cities].Enchants.Flying_Fortress = 0;
        _CITIES[_cities].Enchants.Death_Ward = 0;
        _CITIES[_cities].Enchants.Chaos_Ward = 0;
        _CITIES[_cities].Enchants.Life_Ward = 0;
        _CITIES[_cities].Enchants.Sorcery_Ward = 0;
        _CITIES[_cities].Enchants.Nature_Ward = 0;
        _CITIES[_cities].Enchants.Natures_Eye = 0;
        _CITIES[_cities].Enchants.Earth_Gate = 0;
        _CITIES[_cities].Enchants.Stream_of_Life = 0;
        _CITIES[_cities].Enchants.Gaias_Blessing = 0;
        _CITIES[_cities].Enchants.Inspirations = 0;
        _CITIES[_cities].Enchants.Prosperity = 0;
        _CITIES[_cities].Enchants.Astral_Gate = 0;
        _CITIES[_cities].Enchants.Heavenly_Light = 0;
        _CITIES[_cities].Enchants.Consecration = 0;
        _CITIES[_cities].Nightshade = 0;
        _CITIES[_cities].farmer_count = 3;

        for(bldg_idx = 0; bldg_idx < NUM_BUILDINGS; bldg_idx++)
        {
            _CITIES[_cities].Buildings[bldg_idx] = bs_NotBuilt;
        }
        _CITIES[_cities].Buildings[0] = bs_Replaced; /* None/PlaceHolder */
        _CITIES[_cities].Buildings[bt_Smithy] = bs_Built;
        _CITIES[_cities].Buildings[bt_Barracks] = bs_Built;
        _CITIES[_cities].Buildings[bt_BuildersHall] = bs_Built;

        Random_City_Name_By_Race_NewGame(_CITIES[_cities].race, &_CITIES[_cities]);

        _cities++;
    }

    /* Create Starting Units - Loop 1 (Spearmen/Early Unit) */
    for(itr = 0; itr < _num_players; itr++)
    {
        switch (_CITIES[itr].race)
        {
        case rt_Barbarian: unit_type = _BarbSpearmen; break;
        case rt_Beastmen:  unit_type = _BeastSpearmen; break;
        case rt_Dark_Elf:  unit_type = _DrowSpearmen; break;
        case rt_Draconian: unit_type = _DracSpearmen; break;
        case rt_Dwarf:     unit_type = _DwarfSwordsmen; break;
        case rt_Gnoll:     unit_type = _GnollSpearmen; break;
        case rt_Halfling:  unit_type = _HflngSpearmen; break;
        case rt_High_Elf:  unit_type = _HElfSpearmen; break;
        case rt_High_Man:  unit_type = _HMenSpearmen; break;
        case rt_Klackon:   unit_type = _KlckSpearmen; break;
        case rt_Lizardman: unit_type = _LizSpearmen; break;
        case rt_Nomad:     unit_type = _NmdSpearmen; break;
        case rt_Orc:       unit_type = _OrcSpearmen; break;
        case rt_Troll:     unit_type = _TrlSpearmen; break;
        default:           unit_type = _OrcSpearmen; break;
        }

        /* Dwarves don't get spearmen in the first pass */
        if(_CITIES[itr].race != rt_Dwarf)
        {
            Create_Unit_NewGame(unit_type, itr, _CITIES[itr].wx, _CITIES[itr].wy, _CITIES[itr].wp, itr);
        }
    }

    /* Create Starting Units - Loop 2 (Swordsmen/Second Unit) */
    for(itr = 0; itr < _num_players; itr++)
    {
        switch (_CITIES[itr].race)
        {
        case rt_Barbarian: unit_type = ut_BarbSwordsmen; break;
        case rt_Beastmen:  unit_type = _BeastSwordsmen; break;
        case rt_Dark_Elf:  unit_type = _DrowSwordsmen; break;
        case rt_Draconian: unit_type = _DracSwordsmen; break;
        case rt_Dwarf:     unit_type = _DwarfSwordsmen; break;
        case rt_Gnoll:     unit_type = _GnollSwordsmen; break;
        case rt_Halfling:  unit_type = _HflngSwordsmen; break;
        case rt_High_Elf:  unit_type = _HElfSwordsmen; break;
        case rt_High_Man:  unit_type = _HMenSwordsmen; break;
        case rt_Klackon:   unit_type = _KlckSwordsmen; break;
        case rt_Lizardman: unit_type = _LizSwordsmen; break;
        case rt_Nomad:     unit_type = _NmdSwordsmen; break;
        case rt_Orc:       unit_type = _OrcSwordsmen; break;
        case rt_Troll:     unit_type = _TrlSwordsmen; break;
        default:           unit_type = _OrcSwordsmen; break;
        }

        /* Dwarves get two swordsmen */
        if(_CITIES[itr].race == rt_Dwarf)
        {
            Create_Unit_NewGame(unit_type, itr, _CITIES[itr].wx, _CITIES[itr].wy, _CITIES[itr].wp, itr);
        }

        Create_Unit_NewGame(unit_type, itr, _CITIES[itr].wx, _CITIES[itr].wy, _CITIES[itr].wp, itr);
    }

@@Done:
    return;
}
#endif


// MGC o51p06
/**
 * @brief Clears all landmass identifiers for a single world plane.
 *
 * @details
 * Resets `m_landmasses_ctr` to `1` and iterates over every map square on
 * the selected plane, writing landmass id `0` via `SET_LANDMASS()`.
 *
 * This is an early world-generation step used before landmass assignment and
 * consolidation passes.
 *
 * @param wp World plane index to clear (for example Arcanus or Myrror).
 *
 * @return void
 *
 * @note The function clears only the specified plane; callers are responsible
 *       for invoking it separately per plane when needed.
 */
void Init_Landmasses(int16_t wp)
{
    int16_t wy = 0;
    int16_t wx = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    m_landmasses_ctr = 1;
    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            SET_LANDMASS(wx, wy, wp, 0);  /* landmass_idx of NO_LANDMASS */
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p07
/**
 * @brief Rebalances node types on one plane by converting excess Sorcery nodes.
 *
 * @details
 * Counts Chaos, Nature, and Sorcery nodes on the selected plane, computes how
 * many Sorcery nodes should be converted, then performs randomized in-place
 * conversions from Sorcery to Chaos and Sorcery to Nature.
 *
 * Plane-specific targets are:
 * - Arcanus: if Sorcery exceeds 9, attempt to raise Chaos and Nature to at least 6 each.
 * - Myrror: if Sorcery exceeds 4, attempt to raise Chaos and Nature to at least 3 each.
 *
 * Conversion is done by repeatedly picking random node indices and converting
 * only nodes that are both on the selected plane and currently Sorcery. For
 * each successful conversion, the node type in `_NODES[]` and the map square in
 * `p_world_map` are updated, and `Build_Landmass()` is called for the node
 * coordinate.
 *
 * @param wp World plane index to process (for example Arcanus or Myrror).
 *
 * @return void
 *
 * @note This routine mutates global node records and terrain map state.
 * @note Preserves original historical behavior where `Excess_Sorcery` is not
 *       decremented while computing conversion counts.
 * @warning Uses randomized index selection (`Random(30) - 1`), so selection
 *          order and retries are nondeterministic.
 *
 * @see Build_Landmass
 */
void Rebalance_Node_Types(int16_t wp)
{
    int16_t Excess_Sorcery = 0;
    int16_t Nature_Convert = 0;
    int16_t Chaos_Convert = 0;
    int16_t node_idx = 0;
    int16_t Nature_Count = 0;
    int16_t Sorcery_Count = 0;
    int16_t Chaos_Count = 0;
    int16_t random_node_idx = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    Chaos_Count = 0;
    Sorcery_Count = 0;
    Nature_Count = 0;
    for(node_idx = 0; node_idx < NUM_NODES; node_idx++)
    {
        if(_NODES[node_idx].wp == wp)
        {
            switch(_NODES[node_idx].type)
            {
                case nt_Sorcery: { Sorcery_Count++; } break;
                case nt_Nature:  { Nature_Count++;  } break;
                case nt_Chaos:   { Chaos_Count++;   } break;
            }
        }
    }


    /* Phase 2 */
    Chaos_Convert = 0;
    Nature_Convert = 0;
    if(wp == ARCANUS_PLANE)
    {
        /* OGBUG does not decrement the excess sorcery count */
        if(Sorcery_Count > 9)
        {
            Excess_Sorcery = (Sorcery_Count - 9);
        }
        else
        {
            Excess_Sorcery = 0;
        }
        while((Chaos_Count < 6) && (Excess_Sorcery > 0))
        {
            Chaos_Count++;
            Chaos_Convert++;
        }
        while((Nature_Count < 6) && (Excess_Sorcery > 0))
        {
            Nature_Count++;
            Nature_Convert++;
        }
    }
    else  /* MYRROR_PLANE */
    {
        /* OGBUG does not decrement the excess sorcery count */
        if(Sorcery_Count > 4)
        {
            Excess_Sorcery = (Sorcery_Count - 4);
        }
        else
        {
            Excess_Sorcery = 0;
        }
        while((Chaos_Count < 3) && (Excess_Sorcery > 0))
        {
            Chaos_Count++;
            Chaos_Convert++;
        }
        while((Nature_Count < 3) && (Excess_Sorcery > 0))
        {
            Nature_Count++;
            Nature_Convert++;
        }
    }

    /* Convert Sorcery nodes to Chaos nodes until target reached */
    while(Chaos_Convert > 0)
    {
        random_node_idx = (Random(30) - 1);
        if(_NODES[random_node_idx].wp == wp)
        {
            if(_NODES[random_node_idx].type == nt_Sorcery)
            {
                _NODES[random_node_idx].type = nt_Chaos;
                p_world_map[wp][_NODES[random_node_idx].wy][_NODES[random_node_idx].wx] = tt_ChaosNode;
                // ; entirely unnecessary, already done before
                Build_Landmass(wp, _NODES[random_node_idx].wx, _NODES[random_node_idx].wy);
                Chaos_Convert--;
            }
        }
    }

    /* Convert Sorcery nodes to Nature nodes until target reached */
    while(Nature_Convert > 0)
    {
        random_node_idx = (Random(30) - 1);
        if(_NODES[random_node_idx].wp == wp)
        {
            if(_NODES[random_node_idx].type == nt_Sorcery)
            {
                _NODES[random_node_idx].type = nt_Nature;
                p_world_map[wp][_NODES[random_node_idx].wy][_NODES[random_node_idx].wx] = tt_NatureNode;
                // ; entirely unnecessary, already done before
                Build_Landmass(wp, _NODES[random_node_idx].wx, _NODES[random_node_idx].wy);
                Nature_Convert--;
            }
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    
}


// MGC o51p08
// drake178: NEWG_SetSpecLands()
/*
; PATCHED / rewritten in the worldgen customizer to
; include the inland lake fill inherited from v1.50
; (except with non-grassland-only terrain), and allow
; customizing both max tundra reach and desert and
; swamp creation attempts
;
; creates random patches of Tundra, Desert, and Swamp,
; converting existing land based on various conditions
*/
/*
sets tt_Tundra1, tt_Desert1, tt_Swamp1
*/
/**
 * @brief Applies special terrain passes (tundra, desert, swamp) on one world plane.
 *
 * @details
 * Performs three procedural conversion phases on already-initialized land terrain:
 * - Polar tundra pass: scans rows near the north/south edges and probabilistically
 *   converts `tt_Grasslands1` and `tt_Forest1` tiles to `tt_Tundra1`, with stronger
 *   conversion chance closer to the map edge.
 * - Desert patch pass: attempts up to 8 interior-origin patches. If an origin square is
 *   forest, it is converted to desert and short random walks expand nearby non-ocean
 *   tiles into `tt_Desert1`.
 * - Swamp patch pass: attempts up to 8 interior-origin patches. If an origin square is
 *   forest, local random walks convert encountered forest tiles to `tt_Swamp1`.
 *
 * Coordinates are wrapped at map boundaries, so neighborhood walks continue across
 * edges. All changes are applied in-place through p_world_map.
 *
 * @param wp World plane index to modify (for example Arcanus or Myrror).
 *
 * @return void
 *
 * @note Uses multiple random draws (`Random`) and is intentionally non-deterministic
 *       unless the global RNG state is fixed by the caller.
 * @warning Mutates terrain tiles directly on the selected plane.
 */
void Generate_Climate_Terrain_Types(int16_t wp)
{
    int16_t new_direction = 0;  /* ¿ DEDU  c&p error ? looks like? */
    int16_t Steps_To_Take = 0;
    int16_t dir_chg = 0;  /* (Random(4) - 1), used to index dir_chg_tbl_wx/wy */
    int16_t direction = 0;
    int16_t curr_wy = 0;
    int16_t curr_wx = 0;
    int16_t itr = 0;
    int16_t base_wy = 0;
    int16_t base_wx = 0;
    int16_t Steps_Taken = 0;
    int16_t next_wx = 0;
    int16_t next_wy = 0;
    int16_t itr_wx = 0;
    int16_t itr_wy = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

/*
    BEGIN:  tt_Tundra1
*/
    /* North and South Pole Tundra Generation */
    for(itr_wy = 2; itr_wy < 8; itr_wy++)
    {
        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
        {
            if(
                (p_world_map[wp][itr_wy][itr_wx] == tt_Grasslands1)
                ||
                (p_world_map[wp][itr_wy][itr_wx] == tt_Forest1)
            )
            {
                if((2 + Random(8)) >= itr_wy)  /* { 3, ..., 10 } >= { 2, ..., 7 } */
                {
                    p_world_map[wp][itr_wy][itr_wx] = tt_Tundra1;
                }
            }
            if(
                (p_world_map[wp][(WORLD_YMAX - itr_wy)][itr_wx] == tt_Grasslands1)
                ||
                (p_world_map[wp][(WORLD_YMAX - itr_wy)][itr_wx] == tt_Forest1)
            )
            {
                if((2 + Random(8)) >= itr_wy)
                {
                    p_world_map[wp][(WORLD_YMAX - itr_wy)][itr_wx] = tt_Tundra1;
                }
            }
        }
    }
/*
    END:  tt_Tundra1
*/

/*
    BEGIN:  tt_Desert1
*/
    /* Desert Patch Generation */
    for(itr = 0; itr < 8; itr++)
    {
        base_wx = (2 + Random((WORLD_WIDTH  - (3 * 2))));  // {  3, ..., 55 }
        base_wy = (8 + Random((WORLD_HEIGHT - (8 * 2))));  // {  9, ..., 25 }
        // if(p_world_map[wp][base_wy][base_wx] == tt_Forest1)
        if(p_world_map[wp][base_wy][base_wx] == tt_Forest1)
        {
            p_world_map[wp][base_wy][base_wx] = tt_Desert1;
        }
        for(direction = 0; direction < 5; direction++)
        {
            curr_wx = (base_wx + dir_chg_tbl_wx[direction]);
            curr_wy = (base_wy + dir_chg_tbl_wy[direction]);
            new_direction = ST_UNDEFINED;  /* OGBUG not used in this block - c&p error? */
            Steps_To_Take = (4 + Random(6));  // {5, ..., 10}
            for(Steps_Taken = 0; Steps_Taken < Steps_To_Take; Steps_Taken++)
            {
                dir_chg = (Random(4) - 1);  // ¿ choose next direction with anti-straight-line bias ?
                next_wx = (curr_wx + dir_chg_tbl_wx[dir_chg]);
                next_wy = (curr_wy + dir_chg_tbl_wy[dir_chg]);
                if(next_wx >= WORLD_WIDTH)  { next_wx -= WORLD_WIDTH;  }
                if(next_wy >= WORLD_HEIGHT) { next_wy -= WORLD_HEIGHT; }
                if(next_wx < 0) { next_wx += WORLD_WIDTH;  }
                if(next_wy < 0) { next_wy += WORLD_HEIGHT; }
                curr_wx = next_wx;
                curr_wy = next_wy;
                if(p_world_map[wp][curr_wy][curr_wx] != tt_Ocean)
                {
                    p_world_map[wp][curr_wy][curr_wx] = tt_Desert1;
                }
            }
        }
    }
/*
    END:  tt_Desert1
*/

/*
    BEGIN:  tt_Swamp1
*/
    /* Swamp Patch Generation */
    for(itr = 0; itr < 8; itr++)
    {
        do
        {
            base_wx = ( 1 + Random( (WORLD_WIDTH  - ( 2 * 2)) ) );  /* {  2, ..., 57 } */
            base_wy = (10 + Random( (WORLD_HEIGHT - (10 * 2)) ) );  /* { 11, ..., 30 } */
        } while((base_wy >= 35) && (base_wy <= 45));  /* OGBUG  out of bounds of the world map; was probably meant as a equitorial band? */
        if(p_world_map[wp][base_wy][base_wx] == tt_Forest1)
        {
            p_world_map[wp][base_wy][base_wx] = tt_Swamp1;
        }
        for(direction = 0; direction < 5; direction++)
        {
            curr_wx = (base_wx + dir_chg_tbl_wx[direction]);
            curr_wy = (base_wy + dir_chg_tbl_wy[direction]);
            new_direction = ST_UNDEFINED;  /* OGBUG not used in this block - c&p error? */
            Steps_To_Take = (2 + Random(3));  /* {3, ..., 5} */
            for(Steps_Taken = 0; Steps_Taken < Steps_To_Take; Steps_Taken++)
            {
                dir_chg = (Random(4) - 1);  // ¿ choose next direction with anti-straight-line bias ?
                next_wx = (curr_wx + dir_chg_tbl_wx[dir_chg]);
                next_wy = (curr_wy + dir_chg_tbl_wy[dir_chg]);
                if(next_wx >= WORLD_WIDTH)  { next_wx -= WORLD_WIDTH;  }
                if(next_wy >= WORLD_HEIGHT) { next_wy -= WORLD_HEIGHT; }
                if(next_wx < 0) { next_wx += WORLD_WIDTH;  }
                if(next_wy < 0) { next_wy += WORLD_HEIGHT; }
                curr_wx = next_wx;
                curr_wy = next_wy;
                if(p_world_map[wp][curr_wy][curr_wx] == tt_Forest1)
                {
                    p_world_map[wp][curr_wy][curr_wx] = tt_Swamp1;
                }
            }
        }
    }
/*
    END:  tt_Swamp1
*/

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}

// MGC o51p09
// drake178: NEWG_SetBaseLands()
/*
sets tt_Ocean, tt_Grasslands1, tt_Forest1, tt_Hills1, tt_Mountain1
*/
/**
 * @brief Translates per-square heightmap hit counts into base terrain types.
 *
 * @details
 * Interprets each map square value as an intermediate worldgen hit count and
 * rewrites it to a base terrain class on the selected plane.
 *
 * Value mapping used by this routine:
 * - 0: unchanged (reserved as no-landmass / ocean marker),
 * - 1: grassland by default, with a 25% chance to become forest,
 * - 2-3: forest,
 * - 4-5: hills,
 * - 6+: mountains.
 *
 * After terrain translation, polar tundra placement is delegated to
 * `Add_Tundra()`.
 *
 * @param wp World plane index to process.
 *
 * @return void
 *
 * @note This function mutates terrain data in-place for the selected plane.
 * @warning Assumes input values are pre-normalized worldgen density/hit markers,
 *          not finalized terrain enum values.
 * @see Add_Tundra
 */
void Translate_Heightmap_To_Base_Terrain_Types(int16_t wp)
{
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;  // _SI_

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
    {
        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
        {
            if(p_world_map[wp][itr_wy][itr_wx] == 0)  // HERE: 0 means NO_LANDMASS, which becomes tt_Ocean
            {
                continue;
            }
        
            // NOTE(JimBalcomb,20260222): this is not how the branching looks in the Dasm
            if(p_world_map[wp][itr_wy][itr_wx] >= 6)
            {
                p_world_map[wp][itr_wy][itr_wx] = tt_Mountain1;
            }
            else if(p_world_map[wp][itr_wy][itr_wx] >= 4)
            {
                p_world_map[wp][itr_wy][itr_wx] = tt_Hills1;
            }
            else if(p_world_map[wp][itr_wy][itr_wx] >= 2)
            {
                p_world_map[wp][itr_wy][itr_wx] = tt_Forest1;
            }
            else if(Random(4) == 1)
            {
                p_world_map[wp][itr_wy][itr_wx] = tt_Forest1;
            }
            else
            {
                p_world_map[wp][itr_wy][itr_wx] = tt_Grasslands1;
            }
        }
    }

    Add_Tundra(wp);

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}
/*
second half of basic terrain generation
adds Tundra to the top and bottom two rows
*/
/**
 * @brief Applies polar tundra terrain on a world plane.
 *
 * @details
 * Sets every square on the top and bottom map rows to tundra. It then performs
 * two independent randomized passes (north inner row and south inner row):
 * for each X position, there is a 25% chance to paint a short horizontal tundra
 * strip of length 0-3 tiles, clamped at the right edge of the map.
 *
 * @param wp World plane index to update.
 *
 * @return void
 *
 * @note Randomness affects only rows `WORLD_YMIN + 1` and `WORLD_YMAX - 1`;
 *       the outer polar rows are always fully tundra after this call.
 * @warning This function mutates terrain data in-place for the selected plane.
 */
void Add_Tundra(int16_t wp)
{
    int16_t num = 0;
    int16_t itr = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;  // _SI_

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
    {
        p_world_map[wp][WORLD_YMIN][itr_wx] = tt_Tundra1;
        p_world_map[wp][WORLD_YMAX][itr_wx] = tt_Tundra1;
        if(Random(4) == 1)  // 25%
        {
            num = Random(4);
            for(itr = 0; ((itr < num) && ((itr_wx + itr) < WORLD_WIDTH)); itr++)
            {
                p_world_map[wp][(WORLD_YMIN + 1)][(itr_wx + itr)] = tt_Tundra1;
            }
        }
        if(Random(4) == 1)  // 25%
        {
            num = Random(4);
            for(itr = 0; ((itr < num) && ((itr_wx + itr) < WORLD_WIDTH)); itr++)
            {
                p_world_map[wp][(WORLD_YMAX - 1)][(itr_wx + itr)] = tt_Tundra1;
            }
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}

// MGC o51p10
/*
¿ populates _landmass[] ?
per plane
~ MoO2 "map"
*/
/*
Gemini:
We aren't going to waste cycles on complex noise functions; we’re going to use a "Random Walk" or "Drunkard’s Walk" algorithm. It’s memory-efficient and creates organic-looking blobs.
The Walk: For each of the 8 landmasses, pick a random starting $x, y$. For $N$ steps (your size parameter), move one square in a random cardinal direction.
The Accumulation: Every time a walker lands on a square, increment its value in the array.
The Render: Iterate through the array. Low counts stay Ocean; high counts become Mountains or Deserts.
Terrain Logic Table
By tracking the number of "hits," we are essentially creating a heightmap or a "density map." The more a walker revisits a spot, the more "significant" that terrain becomes.
*/
/**
 * @brief Creates base land distribution for the selected world plane.
 *
 * @details
 * Initializes the target plane to ocean, partitions map space into coarse
 * sections, and then generates continent shapes using randomized origin and
 * directional growth logic until a target land amount is reached.
 *
 * During generation, map square state is updated through terrain/landmass helpers,
 * and section tracking (`map_sections`) is used to spread origins.
 *
 * @param wp World plane index to generate (for example Arcanus or Myrror).
 *
 * @return void
 *
 * @note Target land quantity is influenced by global game settings (such as
 *       `_landsize`) and random rolls.
 * @warning This routine mutates global map buffers for the selected plane and
 *          is intended for new-game map construction order.
 *
 * @see Init_Landmasses
 * @see NEWG_SetBaseLands__WIP
 * @see Build_Landmass
 */
void Generate_Landmasses(int16_t wp)
{
    int16_t map_sections[5][5] = { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } };
    int16_t base_wy = 0;
    int16_t base_wx = 0;
    int16_t direction = 0;  /* used to index dir_chg_tbl_wx/wy */
    int16_t direction_change_count = 0;
    int16_t section_height = 0;
    int16_t section_width = 0;
    int16_t n_sections = 0;
    int16_t straight_run_length = 0;
    int16_t new_direction = 0;
    int16_t dir_chg = 0;  /* (Random(4) - 1), used to index dir_chg_tbl_wx/wy */
    int16_t steps_walked = 0;
    int16_t segment_length = 0;
    int16_t next_wy = 0;
    int16_t next_wx = 0;
    int16_t curr_wy = 0;
    int16_t curr_wx = 0;
    int16_t n_needed = 0;
    int16_t n_generated = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);


    /* Phase 1: Init */
    /* Initialize map plane to zero (Ocean) */
    for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
    {
        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
        {
            p_world_map[wp][itr_wy][itr_wx] = tt_Ocean;
        }
    }
    /* Initialize section grid for seed distribution */
    for(itr_wy = 0; itr_wy < 5; itr_wy++)
    {
        for(itr_wx = 0; itr_wx < 5; itr_wx++)
        {
            map_sections[itr_wy][itr_wx] = 0;
        }
    }
    n_sections = 8;  /* a spatial-spread quota */
    section_width  = 16;  // DEDU  shouldn't this be WORLD_WIDTH / 5 = 12 ?  alt. world map was 16 * 5 = 80 wide at some point?  or 48 or x / 2,3,4 32,48,64
    section_height = 11;  // DEDU shouldn't this be WORLD_HEIGHT / 5 =  8 ?  alt. world map was 11 * 5 = 55 high at some point?  or 33 or y / 2,3,4 22,33,44
    n_generated = 0;
    /* Set target land budget based on world settings */
    /* MoO2  ~ galaxy size, _NUM_STARS */
    switch(_landsize)
    {
        case gol_Small:  { n_needed = 360; } break;  // ¿ 3 * 120 ?            ¿ 360 of 2400 is 15% ?
        case gol_Medium: { n_needed = 480; } break;  // ¿ 4 * 120 ?  + 1 of 3  ¿ 480 of 2400 is 20% ?
        case gol_Large:  { n_needed = 720; } break;  // ¿ 6 * 120 ?  + 2 of 4  ¿ 720 of 2400 is 30% ?
    }


    /* Phase 2: Main landmass generation loop */
    /* MoO2  ~ _num_maps_to_generate, _n_maps_generated */
    while(n_generated <= n_needed)
    {
        new_direction = ST_UNDEFINED;
        straight_run_length = 1;
        while(1)
        {
            base_wx = (6 + Random(46));  // { 7, ..., 52}
            base_wy = (6 + Random(26));  // { 7, ..., 32}
            assert(base_wx >= 7);
            assert(base_wy >= 7);
            if(n_sections <= 0)
            {
                break;
            }
            if(map_sections[(base_wy / section_height)][(base_wx / section_width)] != 1)  // { 7, ..., 32 } / 11 = { 0, ..., 2 }; { 7, ..., 52 } / 16 = { 0, ..., 3 }
            {
                map_sections[(base_wy / section_height)][(base_wx / section_width)] = 1;  // 8 of 12 sections? with portions unavailable?
                n_sections--;
                break;
            }
        }
        direction_change_count = (2 + Random(3));  // {3, ..., 5}
        for(direction = 0; direction < direction_change_count; direction++)
        {
            switch(_landsize)
            {
                case gol_Small:  { segment_length = ( 5 + Random(10)); } break;
                case gol_Medium: { segment_length = (10 + Random(10)); } break;
                case gol_Large:  { segment_length = (20 + Random(10)); } break;
            }
            curr_wx = (base_wx + dir_chg_start_tbl_wx[direction]);
            curr_wy = (base_wy + dir_chg_start_tbl_wy[direction]);
            steps_walked = 0;
            while((steps_walked < segment_length) && (n_generated <= n_needed))
            {
                if(p_world_map[wp][curr_wy][curr_wx] == tt_Ocean)
                {
                    n_generated++;
                }
                p_world_map[wp][curr_wy][curr_wx] = (p_world_map[wp][curr_wy][curr_wx] + 1);

                /* Call procedural expansion logic */
                Build_Landmass(wp, curr_wx, curr_wy);
                
                /* Direction-retry: re-roll until a direction passes the anti-straight bias AND stays in bounds */

                /* Direction selection with persistence logic */
                while(1)
                {
                    dir_chg = (Random(4) - 1);  // ¿ choose next direction with anti-straight-line bias ?
                    if(dir_chg == new_direction)
                    {
                        /* Probability to keep going in same direction decreases over time */
                        if(Random((straight_run_length * 2)) != 1)
                        {
                            continue;  /* Pick a different direction */
                        }
                        else
                        {
                            straight_run_length++;
                        }
                    }
                    else
                    {
                        straight_run_length = 1;  // different direction, reset bias weight
                    }
                    new_direction = dir_chg;
                    next_wx = (curr_wx + dir_chg_step_tbl_wx[dir_chg]);
                    next_wy = (curr_wy + dir_chg_step_tbl_wy[dir_chg]);
                    /* Bounds check (padding for map edges) */
                    if(
                        (next_wx <  (WORLD_XSTART + 2))
                        ||
                        (next_wy <  (WORLD_YSTART + 4))
                        ||
                        (next_wx >= (WORLD_WIDTH - 2))
                        ||
                        (next_wy >= (WORLD_HEIGHT - 4))
                    )
                    {
                        /* Out of bounds, reset and try again from current square */
                        new_direction = ST_UNDEFINED;
                    }
                    else
                    {
                        curr_wx = next_wx;
                        curr_wy = next_wy;
                        steps_walked++;
                        break;  /* only 'Exit Condition' of main loop */
                    }
                }
            }
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p11
/**
 * @brief Generates all node locations, auras, and node types for both planes.
 *
 * @details
 * Places the full set of world nodes in two passes: the first pass creates the
 * 16 Arcanus nodes, and the second pass creates the Myrror nodes. Each node is
 * assigned a randomized candidate location inside a coarse section grid, then
 * rejected and retried if it is too close to the world edge, conflicts with an
 * existing node, or fails aura overlap validation.
 *
 * For each accepted node, the function records its coordinates and plane,
 * assigns a random power value, builds the node aura footprint with
 * `Make_Aura()`, and then derives the node type with `Set_Node_Type()` after
 * confirming the aura does not overlap an earlier node via `Aura_Overlap()`.
 *
 * Arcanus node placement uses the local Arcanus terrain map as the primary
 * acceptance check, while Myrror placement preserves the historical behavior
 * of probing against the Arcanus plane map when deciding whether a square is
 * allowed. This routine is intentionally RNG-driven and may loop until a valid
 * placement is found.
 *
 * @return void
 *
 * @note Mutates the global `_NODES[]` table in place.
 * @note The helper `somehow1` and `Attempt_Myrror` labels are retry anchors
 *       used by the original control flow.
 *
 * @see Make_Aura
 * @see Aura_Overlap
 * @see Set_Node_Type
 */
void Generate_Nodes(void)
{
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t base_wy = 0;
    int16_t base_wx = 0;
    int16_t itr = 0;
    int16_t itr2 = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr = 0; itr < 16; itr++)
    {
        while(1)
        {
somehow1:
            base_wx = ((itr % 5) * 12);  // { 0, 12, 24, 36, 48 }
            base_wy = ((itr / 5) * 10);  // { 0, 10, 20 , 30 }
            wx = (base_wx + (Random(24) - 1));  // { 0, ..., 71 }
            wy = (base_wy + (Random(20) - 1));  // { 0, ..., 49 }
            if(wx >= WORLD_WIDTH)  { wx -= WORLD_WIDTH;  }
            if(wy >= WORLD_HEIGHT) { wy -= WORLD_HEIGHT; }
            if(wx <  WORLD_XSTART) { wx += WORLD_WIDTH;  }
            if(wy <  WORLD_YSTART) { wy += WORLD_HEIGHT; }
            // BUGBUG  should be consistent? 2 or 3 from poles? 2 or 3 from edges?
            if(
                (wx < 3)
                ||
                (wy < 2)
                ||
                (wx >= (WORLD_WIDTH - 3))
                ||
                (wy >= (WORLD_HEIGHT - 3))
            )
            {
                continue;
            }
            if(
                (p_world_map[ARCANUS_PLANE][wy][wx] != tt_Ocean)
                ||
                (Random(40) == 1)  // 2.5%
            )
            {
                for(itr2 = 0; itr2 < itr; itr2++)
                {
                    if(Delta_XY_With_Wrap(wx, wy, _NODES[itr2].wx, _NODES[itr2].wy, WORLD_WIDTH) < 3)
                    {
                        goto somehow1;
                    }
                }
                _NODES[itr].wx = (int8_t)wx;
                _NODES[itr].wy = (int8_t)wy;
                _NODES[itr].wp = ARCANUS_PLANE;
                _NODES[itr].flags = 0;
                _NODES[itr].owner_idx = ST_UNDEFINED;
                _NODES[itr].power = (4 + Random(6));
                Make_Aura(_NODES[itr].power, &_NODES[itr].Aura_Xs[0], &_NODES[itr].Aura_Ys[0], wx, wy);
                if(Aura_Overlap(itr) != ST_TRUE)
                {
                    Set_Node_Type(_NODES[itr].power, &_NODES[itr].Aura_Xs[0], &_NODES[itr].Aura_Ys[0], _NODES[itr].wp, &_NODES[itr].type);
                    break;
                }
            }
        }
    }
    /* Myrror Nodes Generation (Indices 16-29) */
    for(itr = 16; itr < NUM_NODES; itr++)
    {
        while(1)
        {
Attempt_Myrror:
            /* OGBUG  base offset, should be (itr - 16), not - 20 */
            base_wx = (((itr - 20) % 5) * 12);  
            base_wy = (((itr - 20) / 5) * 20);
            wx = (base_wx + (Random(24) - 1));
            wy = (base_wy + (Random(40) - 1));
            if(wx >= WORLD_WIDTH)  { wx -= WORLD_WIDTH;  }
            if(wy >= WORLD_HEIGHT) { wy -= WORLD_HEIGHT; }
            if(wx <  WORLD_XSTART) { wx += WORLD_WIDTH;  }
            if(wy <  WORLD_YSTART) { wy += WORLD_HEIGHT; }
            /* OGBUG  north-pole margin is 2 (wy<2) while west/east/south are 3 — asm cmp wy,2 / cmp wy,37; preserved */
            if(
                (wx < 3)
                ||
                (wy < 2)
                ||
                (wx >= (WORLD_WIDTH - 3))
                ||
                (wy >= (WORLD_HEIGHT - 3))
            )
            {
                continue;
            }
            if(
                (p_world_map[ARCANUS_PLANE][wy][wx] != tt_Ocean)  /* OGBUG  should be MYRROR_PLANE, not ARCANUS_PLANE */
                ||
                (Random(25) == 1)  // 4.0%
            )
            {
                for(itr2 = 0; itr2 < itr; itr2++)
                {
                    if(_NODES[itr2].wp == MYRROR_PLANE)
                    {
                        if(Delta_XY_With_Wrap(wx, wy, _NODES[itr2].wx, _NODES[itr2].wy, WORLD_WIDTH) < 3)
                        {
                            goto Attempt_Myrror;
                        }
                    }
                }
                _NODES[itr].wx = (int8_t)wx;
                _NODES[itr].wy = (int8_t)wy;
                _NODES[itr].wp = MYRROR_PLANE;
                _NODES[itr].flags = 0;
                _NODES[itr].owner_idx = ST_UNDEFINED;
                _NODES[itr].power = (9 + Random(11));
                Make_Aura(_NODES[itr].power, &_NODES[itr].Aura_Xs[0], &_NODES[itr].Aura_Ys[0], wx, wy);
                if(Aura_Overlap(itr) != ST_TRUE)
                {
                    Set_Node_Type(_NODES[itr].power, &_NODES[itr].Aura_Xs[0], &_NODES[itr].Aura_Ys[0], _NODES[itr].wp, &_NODES[itr].type);
                    break;
                }
            }
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p12
void Make_Aura(int16_t power, int8_t * wx_array, int8_t * wy_array, int16_t wx, int16_t wy)
{
    int16_t invalid = 0;
    int16_t dir_chg_wy = 0;
    int16_t dir_chg_wx = 0;
    int16_t curr_wy = 0;
    int16_t curr_wx = 0;
    int16_t itr = 0;
    int16_t itr2 = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    wx_array[0] = (int8_t)wx;
    wy_array[0] = (int8_t)wy;
    /* ¿ 9, because within 1 map square ? */
    for(itr = 1; ((itr < power) && (itr < 9)); itr++)
    {
        do
        {
            dir_chg_wx = (Random(4) - 1);
            dir_chg_wy = (Random(4) - 1);
            curr_wx = (wx + dir_chg_tbl_wx[dir_chg_wx]);
            curr_wy = (wy + dir_chg_tbl_wy[dir_chg_wy]);
            invalid = ST_FALSE;
            for(itr2 = 0; itr2 < itr; itr2++)
            {
                if(
                    (wx_array[itr2] == curr_wx)
                    &&
                    (wy_array[itr2] == curr_wy)
                )
                {
                    invalid = ST_TRUE;
                }
            }
            if(
                (curr_wx <= WORLD_XMIN)
                ||
                (curr_wy <= WORLD_YMIN)
                ||
                (curr_wx >= WORLD_XMAX)
                ||
                (curr_wy >= WORLD_YMAX)
            )
            {
                invalid = ST_TRUE;
            }
        } while(invalid == ST_TRUE);
        wx_array[itr] = (int8_t)curr_wx;
        wy_array[itr] = (int8_t)curr_wy;
    }
    for(itr = 9; itr < power; itr++)
    {
        do
        {
            dir_chg_wx = (Random(4) - 1);
            dir_chg_wy = (Random(4) - 1);
            curr_wx = (wx + (dir_chg_tbl_wx[dir_chg_wx] * Random(2)));
            curr_wy = (wy + (dir_chg_tbl_wy[dir_chg_wy] * Random(2)));
            invalid = ST_FALSE;
            for(itr2 = 0; itr2 < itr; itr2++)
            {
                if(
                    (wx_array[itr2] == curr_wx)
                    &&
                    (wy_array[itr2] == curr_wy)
                )
                {
                    invalid = ST_TRUE;
                }
            }
            if(
                (curr_wx <= WORLD_XMIN)
                ||
                (curr_wy <= WORLD_YMIN)
                ||
                (curr_wx >= WORLD_XMAX)
                ||
                (curr_wy >= WORLD_YMAX)
            )
            {
                invalid = ST_TRUE;
            }
        } while(invalid == ST_TRUE);
        wx_array[itr] = (int8_t)curr_wx;
        wy_array[itr] = (int8_t)curr_wy;
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p13
int16_t Aura_Overlap(int16_t node_idx)
{
    int16_t wy_array[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t power = 0;
    int16_t itr2 = 0;
    int16_t itr3 = 0;
    int16_t itr1 = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    power = _NODES[node_idx].power;
    for(itr1 = 0; itr1 < power; itr1++)
    {
        wx_array[itr1] = _NODES[node_idx].Aura_Xs[itr1];
        wy_array[itr1] = _NODES[node_idx].Aura_Ys[itr1];
    }
    for(itr1 = 0; itr1 < node_idx; itr1++)
    {
        for(itr2 = 0; _NODES[itr1].power > itr2; itr2++)
        {
            for(itr3 = 0; itr3 < power; itr3++)
            {
                if(
                    (wx_array[itr3] == _NODES[itr1].Aura_Xs[itr2])
                    &&
                    (wy_array[itr3] == _NODES[itr1].Aura_Ys[itr2])
                )
                {
                    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
                    return ST_TRUE;
                }
            }
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    return ST_FALSE;
}


// MGC o51p14
void Set_Node_Type(int16_t power, int8_t * wx_array, int8_t * wy_array, int16_t wp, int8_t * type)
{
    int16_t nature_bias = 0;
    int16_t chaos_bias = 0;
    int16_t sorcery_bias = 0;
    int16_t itr = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    sorcery_bias = 0;
    chaos_bias = 0;
    nature_bias = 0;

    for(itr = 0; itr < power; itr++)
    {
        if(p_world_map[wp][wy_array[itr]][wx_array[itr]] == tt_Ocean)
        {
            sorcery_bias++;
        }
        if(
            (p_world_map[wp][wy_array[itr]][wx_array[itr]] == tt_Mountain1)
            ||
            (p_world_map[wp][wy_array[itr]][wx_array[itr]] == tt_Desert1)
        )
        {
            chaos_bias++;
        }
        if(
            (p_world_map[wp][wy_array[itr]][wx_array[itr]] == tt_Grasslands1)
            ||
            (p_world_map[wp][wy_array[itr]][wx_array[itr]] == tt_Forest1)
        )
        {
            nature_bias++;
        }
    }
    /* NOTE(drake178): changed in v1.1 */
    sorcery_bias = Random(25);
    chaos_bias   = Random(15);
    nature_bias  = Random(15);
    if(
        (chaos_bias > sorcery_bias)
        &&
        (chaos_bias > nature_bias)
    )
    {
        p_world_map[wp][wy_array[0]][wx_array[0]] = tt_ChaosNode;
        *type = nt_Chaos;
        Build_Landmass(wp, wx_array[0], wy_array[0]);
    }
    else
    {
        if(sorcery_bias > nature_bias)
        {
            p_world_map[wp][wy_array[0]][wx_array[0]] = tt_SorceryNode;
            *type = nt_Sorcery;
        }
        else
        {
            p_world_map[wp][wy_array[0]][wx_array[0]] = tt_NatureNode;
            *type = nt_Nature;
        }
        Build_Landmass(wp, wx_array[0], wy_array[0]);
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p15
/**
 * @brief Assigns a landmass id to a map square and merges overlapping landmasses.
 *
 * @details
 * Inspects the center map square plus its 8 neighbors for existing non-zero
 * landmass ids, collecting overlaps into a local scratch array.
 *
 * If no overlaps are found, assigns a new id from `m_landmasses_ctr` to the
 * map square and increments the counter.
 *
 * If overlaps are found, uses the first collected id as the primary landmass,
 * rewrites any conflicting overlapping ids in the local overlap list, then
 * scans the entire selected plane and remaps all map squares that still carry each
 * conflicting id to the primary id. Finally, sets the target map square to the
 * primary id.
 *
 * This function is part of map-generation continent construction and updates
 * global `_landmasses` data for the selected plane.
 *
 * @param wp World plane index (for example Arcanus or Myrror).
 * @param wx World X coordinate of the map square.
 * @param wy World Y coordinate of the map square.
 *
 * @return void
 *
 * @note Overlap resolution is deterministic for a given local neighborhood:
 *       the first encountered overlap id becomes the merge target.
 * @warning Merge resolution includes a full `WORLD_WIDTH * WORLD_HEIGHT`
 *          scan of the plane for each conflicting id, which is costlier than
 *          purely local updates.
 */
void Build_Landmass(int16_t wp, int16_t wx, int16_t wy)
{
    int16_t overlap_landmasses[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t conflict_landmass_idx = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    int16_t itr2 = 0;
    int16_t itr = 0;
    int16_t overlap_landmass_first = 0;  /* landmass_idx of the first landmass added to the array */
    int16_t overlap_ctr = 0;  // _CX_

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    overlap_landmass_first = 0;

    overlap_ctr = 0;

    /*
        check all nine map squares for a landmass, and add any found into a local scratch array
        { center, left-up, up, right-up, left, right, left-down, down, right-down }
    */
    if(GET_LANDMASS(wx, wy, wp) != 0)
    {
        overlap_landmasses[overlap_ctr] = GET_LANDMASS(wx, wy, wp);
        overlap_ctr++;
    }
    if(GET_LANDMASS((wx - 1), (wy - 1), wp) != 0)
    {
        overlap_landmasses[overlap_ctr] = GET_LANDMASS((wx - 1), (wy - 1), wp);
        overlap_ctr++;
    }
    if(GET_LANDMASS(wx, (wy - 1),wp) != 0)
    {
        overlap_landmasses[overlap_ctr] = GET_LANDMASS(wx, (wy - 1), wp);
        overlap_ctr++;
    }
    if(GET_LANDMASS((wx + 1), (wy - 1), wp) != 0)
    {
        overlap_landmasses[overlap_ctr] = GET_LANDMASS((wx + 1), (wy - 1), wp);
        overlap_ctr++;
    }
    if(GET_LANDMASS((wx - 1), wy, wp) != 0)
    {
        overlap_landmasses[overlap_ctr] = GET_LANDMASS((wx - 1), wy, wp);
        overlap_ctr++;
    }
    if(GET_LANDMASS((wx + 1), wy, wp) != 0)
    {
        overlap_landmasses[overlap_ctr] = GET_LANDMASS((wx + 1), wy, wp);
        overlap_ctr++;
    }
    if(GET_LANDMASS((wx - 1), (wy + 1), wp) != 0)
    {
        overlap_landmasses[overlap_ctr] = GET_LANDMASS((wx - 1), (wy + 1), wp);
        overlap_ctr++;
    }
    if(GET_LANDMASS(wx, (wy + 1), wp) != 0)
    {
        overlap_landmasses[overlap_ctr] = GET_LANDMASS(wx, (wy + 1), wp);
        overlap_ctr++;
    }
    if(GET_LANDMASS((wx + 1), (wy + 1), wp) != 0)
    {
        overlap_landmasses[overlap_ctr] = GET_LANDMASS((wx + 1), (wy + 1), wp);
        overlap_ctr++;
    }

    /*
        add new landmass if no overlaps, otherwise consolidate any overlapping landmasses into the primary landmass
    */
    if(overlap_ctr == 0)
    {
        SET_LANDMASS(wx, wy, wp, (uint8_t)m_landmasses_ctr);  // assign the landmass_idx to the map square
        m_landmasses_ctr++;  // next landmass_idx
    }
    else
    {
        overlap_landmass_first = overlap_landmasses[0];  /* DEDU  Is there a more sophisticated option for selecting the primary landmass? */
        for(itr = 1; itr < overlap_ctr; itr++)
        {
            if(overlap_landmasses[itr] != overlap_landmass_first)
            {
                conflict_landmass_idx = overlap_landmasses[itr];
                for(itr2 = itr; itr2 < overlap_ctr; itr2++)
                {
                    if(overlap_landmasses[itr2] == conflict_landmass_idx)
                    {
                        overlap_landmasses[itr2] = overlap_landmass_first;
                    }
                }
                for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
                {
                    for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
                    {
                        if(GET_LANDMASS(itr_wx, itr_wy, wp) == conflict_landmass_idx)
                        {
                            SET_LANDMASS(itr_wx, itr_wy, wp, (uint8_t)overlap_landmass_first);
                        }
                    }
                }
            }
        }
        SET_LANDMASS(wx, wy, wp, (uint8_t)overlap_landmass_first);
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p16
/**
 * @brief Populates the global encounter table with towers, nodes, and random lairs.
 *
 * @details
 * Builds initial `_LAIRS` records in staged groups during world generation:
 *
 * 1) Creates one tower encounter per Arcanus tower (`NUM_TOWERS`) with fixed
 *    tower lair type and randomized reward budget.
 * 2) Duplicates those tower records into the corresponding Myrror entries by
 *    copying records and changing only plane (`wp`).
 * 3) Creates one node encounter per node (`NUM_NODES`) using node power/type to
 *    derive guardian tier and budget.
 * 4) Attempts to create `strong_lair_count` random strong encounters (currently
 *    25) on non-ocean squares, with plane-dependent strength scaling.
 * 5) Attempts to create 32 random weak encounters on non-ocean squares, again
 *    with plane-dependent budget scaling.
 * 6) Marks the final 3 trailing records as empty (`intact = ST_FALSE`).
 *
 * Placement checks use `Delta_XY_With_Wrap` against previously populated lair
 * records to avoid close clustering when possible.
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 *
 * @note Mutates global encounter state in `_LAIRS` and depends on preexisting
 *       `_TOWERS`, `_NODES`, map terrain, and global settings such as `_magic`.
 * @note Individual random lair attempts may fail placement; the loop advances
 *       regardless, so realized count can be below attempted count.
 *
 * @see Create_Lair
 * @see Square_Is_Ocean_NewGame
 * @see Delta_XY_With_Wrap
 */
void Generate_Lairs(void)
{
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t itr2 = 0;
    int16_t wp = 0;
    int16_t strong_lair_count = 0;
    int16_t itr = 0;
    int16_t niu_create_lair_parameter = 0;
    int16_t tail_start = 0;
    int16_t generate_lair_budget_value = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    strong_lair_count = NUM_STRONG_LAIRS;

    for(itr = 0; itr < NUM_TOWERS; itr++)
    {
        Create_Lair(itr, ARCANUS_PLANE, _TOWERS[itr].wx, _TOWERS[itr].wy, 3, 0, (650 + (50 * Random(11))));
    }

    for(itr = NUM_TOWERS; itr < (2 * NUM_TOWERS); itr++)
    {
        memcpy(&_LAIRS[itr], &_LAIRS[(itr - NUM_TOWERS)], sizeof(struct s_LAIR));
        _LAIRS[itr].wp = MYRROR_PLANE;
    }

    for(itr = 0; itr < NUM_NODES; itr++)
    {
        niu_create_lair_parameter = (((((_NODES[itr].power - 1) * _magic) / 2) / 6) + 1);
        SETMAX(niu_create_lair_parameter, 4);
        SETMIN(niu_create_lair_parameter, 1);
        generate_lair_budget_value = ((((_NODES[itr].power * _NODES[itr].power) * (4 + Random(11))) * (_magic + 1)) / 2);
        Create_Lair(((2 * NUM_TOWERS) + itr), _NODES[itr].wp, _NODES[itr].wx, _NODES[itr].wy, niu_create_lair_parameter, (_NODES[itr].type + 1), generate_lair_budget_value);
    }

    // ; create 25 random strong encounters on non-ocean map squares

    itr = 0;
    while(itr < strong_lair_count)
    {

        wp = (Random(2) - 1);  /* { 0:Arcanus, 1:Myrror } */
        wx = (2 + Random((WORLD_WIDTH  - 2 - 2 - 2)));  // 54
        wy = (2 + Random((WORLD_HEIGHT - 2 - 2 - 2)));  // 34

        if(Square_Is_Ocean_NewGame(wx, wy, wp) == ST_TRUE)
        {
            continue;
        }
    
        for(itr2 = 0; ((itr + (2 * NUM_TOWERS) + NUM_NODES) > itr2); itr2++)
        {
            if(Delta_XY_With_Wrap(wx, wy, _LAIRS[itr2].wx, _LAIRS[itr2].wy, WORLD_WIDTH) < 2)
            {
                break;
            }
        }
        
        if(((2 * NUM_TOWERS) + NUM_NODES + itr) <= itr2)
        {
            if(wp == ARCANUS_PLANE)
            {
                generate_lair_budget_value = (50 + (Random(29) * 50));
            }
            else
            {
                generate_lair_budget_value = (100 + (Random(24) * 100));
            }
            Create_Lair(((2 * NUM_TOWERS) + NUM_NODES + itr), wp, wx, wy, generate_lair_budget_value, 4, generate_lair_budget_value);
            itr++;
        }

    }

    // ; create 32 weak random encounters on non-ocean map squares

    itr = 0;
    while(itr < NUM_WEAK_LAIRS)
    {

        wp = (Random(2) - 1);  // { 0:Arcanus, 1:Myrror }
        wx = (2 + Random((WORLD_WIDTH  - 2 - 2 - 2)));  // 54
        wy = (2 + Random((WORLD_HEIGHT - 2 - 2 - 2)));  // 34

        if(Square_Is_Ocean_NewGame(wx, wy, wp) == ST_TRUE)
        {
            continue;
        }

        for(itr2 = 0; (((2 * NUM_TOWERS) + NUM_NODES + strong_lair_count + itr) > itr2); itr2++)
        {
            if(Delta_XY_With_Wrap(wx, wy, _LAIRS[itr2].wx, _LAIRS[itr2].wy, WORLD_WIDTH) < 2)
            {
                break;
            }
        }

        if(((2 * NUM_TOWERS) + NUM_NODES + strong_lair_count + itr) <= itr2)
        {
            if(wp == ARCANUS_PLANE)
            {
                generate_lair_budget_value = (Random(10) * 10);
            }
            else
            {
                generate_lair_budget_value = (Random(20) * 10);
            }
            Create_Lair(((2 * NUM_TOWERS) + NUM_NODES + strong_lair_count + itr), wp, wx, wy, generate_lair_budget_value, 4, generate_lair_budget_value);
            itr++;
        }

    }
    
    /* Final Loop: Deactivate specific lair slots (cleanup/reserved) */
    tail_start = ((2 * NUM_TOWERS) + NUM_NODES + strong_lair_count + NUM_WEAK_LAIRS);
    for(itr = tail_start; (itr < (tail_start + 3)) && (itr < NUM_LAIRS); itr++)
    {
        _LAIRS[itr].intact = ST_FALSE;
    }

#ifdef STU_DEBUG
    assert(Validate_All_Lairs() == ST_TRUE);
#endif

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p17
/**
 * @brief Initializes one lair/encounter record with guards and treasure.
 *
 * @details
 * Populates `_LAIRS[lair_idx]` using the provided placement/type/budget inputs,
 * then derives guardian stacks and loot by race/type-biased random selection.
 *
 * Current behavior summary:
 * 1) Scales incoming `budget` by game difficulty.
 * 2) Writes lair coordinates/plane/intact state and resolves encounter type
 *    (`lt_Tower`, node types, or randomized lair type for default branch).
 * 3) Clears loot/guardian fields.
 * 4) Chooses guardian race bias from explicit lair type rules.
 * 5) Picks primary guardian unit (`guard1_*`) by searching unit table for the
 *    highest affordable non-transport unit of selected race within a divided
 *    budget; then computes count and reduces remaining budget.
 * 6) Optionally picks secondary guardian unit (`guard2_*`) under remaining
 *    budget and 9-unit combined cap.
 * 7) Recomputes an effective treasure budget (`IDK`) from guardian value with
 *    additional scaling by difficulty and plane.
 * 8) Applies randomized loot outcomes (gold, mana, items, prisoner, spells,
 *    spellbook/retort special) with per-case qualifiers and budget deductions.
 * 9) If special reward tier exceeds 4, clears other treasure channels.
 *
 * @param lair_idx Index into `_LAIRS` to initialize.
 * @param wp World plane index for the lair location.
 * @param wx World X coordinate of lair location.
 * @param wy World Y coordinate of lair location.
 * @param niu_create_lair_parameter unknown parameter (currently unused).
 * @param lair_type New-game lair type selector used for encounter type/race
 *                  rules (tower/node/random category paths).
 * @param budget Base encounter budget before internal scaling.
 *
 * @return void
 *
 * @note Mutates `_LAIRS[lair_idx]` in place and depends on global tables such
 *       as `_unit_type_table` and current `_difficulty`/`_magic` settings.
 * @warning Several known quirks/bugs in current logic are preserved (including
 *          assignment-in-condition paths and some fall-through behavior).
 */
void Create_Lair(int16_t lair_idx, int16_t wp, int16_t wx, int16_t wy, int16_t niu_create_lair_parameter, int16_t lair_type, int16_t budget)
{
    int16_t tries = 0;
    int16_t divided_budget = 0;
    int16_t highest_cost = 0;
    int16_t selected_unit_type = 0;
    int16_t Price = 0;
    int16_t race_type = 0;
    int16_t unit_type = 0;
    int16_t loot_budget = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

#ifdef STU_DEBUG
    assert(lair_idx >= 0);
    assert(lair_idx < NUM_LAIRS);
    assert(wx >= 0);
    assert(wx < WORLD_WIDTH);
    assert(wy >= 0);
    assert(wy < WORLD_HEIGHT);
    assert(wp >= 0);
    assert(wp < NUM_PLANES);
#endif

    budget = ((budget * (_difficulty + 1)) / 4);
    _LAIRS[lair_idx].wx = (int8_t)wx;
    _LAIRS[lair_idx].wy = (int8_t)wy;
    _LAIRS[lair_idx].wp = (int8_t)wp;
    _LAIRS[lair_idx].intact = ST_TRUE;
    switch(lair_type)
    {
        case 0:                { _LAIRS[lair_idx].type = lt_Tower;                    } break;
        case (nt_Sorcery + 1): { _LAIRS[lair_idx].type = lt_Sorcery_Node;             } break;
        case (nt_Nature  + 1): { _LAIRS[lair_idx].type = lt_Nature_Node;              } break;
        case (nt_Chaos   + 1): { _LAIRS[lair_idx].type = lt_Chaos_Node;               } break;
        default:               { _LAIRS[lair_idx].type = (lt_Cave + (Random(7) - 1)); } break;
    }

    _LAIRS[lair_idx].Loot_Gold = 0;
    _LAIRS[lair_idx].Loot_Mana = 0;
    _LAIRS[lair_idx].Spell_n_Special = 0;
    _LAIRS[lair_idx].Item_Count = 0;
    _LAIRS[lair_idx].Misc_Flags = 0;
    _LAIRS[lair_idx].guard1_unit_type = 0;
    _LAIRS[lair_idx].guard2_unit_type = 0;
    _LAIRS[lair_idx].guard1_count = 0;
    _LAIRS[lair_idx].guard2_count = 0;
    if(lair_type == 1)
    {
        race_type = rt_Sorcery;
    }
    else if(lair_type == 3)
    {
        race_type = rt_Chaos;
    }
    else if(lair_type == 2)
    {
        race_type = rt_Nature;
    }
    else if(lair_type == 0)  /* New Game Lair Type - Tower */
    {
        switch((Random(6) - 1))
        {
            case 0:
            case 1: { race_type = rt_Death;   } break;
            case 2: { race_type = rt_Chaos;   } break;
            case 3: { race_type = rt_Life;    } break;
            case 4: { race_type = rt_Nature;  } break;
            case 5: { race_type = rt_Sorcery; } break;
        }
    }
    else if(
        (_LAIRS[lair_idx].type == lt_Ancient_Temple)
        ||
        (_LAIRS[lair_idx].type == lt_Ruins)
        ||
        (_LAIRS[lair_idx].type == lt_Fallen_Temple)
    )
    {
        switch((Random(4) - 1))
        {
            case 0:
            case 1:
            case 2: { race_type = rt_Death; } break;
            case 3: { race_type = rt_Life; } break;
        }
    }
    else
    {
        switch((Random(5) - 1))
        {
            case 0: { race_type = rt_Death;  }  /* OGBUG:  falls through, missing break */
            case 1: { race_type = rt_Death;  }  /* OGBUG:  falls through, missing break */
            case 2: { race_type = rt_Chaos;  }  /* OGBUG:  falls through, missing break */
            case 3: { race_type = rt_Chaos;  }  /* OGBUG:  falls through, missing break */
            case 4: { race_type = rt_Nature; }  /* OGBUG:  falls through, missing break */
        }
    }
    /* Pick Guard 1 */
    tries = 0;
    do
    {
        divided_budget = (budget / Random(4));
        highest_cost = 0;
        selected_unit_type = ST_UNDEFINED;
        for(unit_type = 150; unit_type < NUM_UNIT_TYPES; unit_type++)
        {
            if(
                (_unit_type_table[unit_type].race_type == race_type)
                &&
                (_unit_type_table[unit_type].Transport == 0)
                &&
                (_unit_type_table[unit_type].cost > highest_cost)
                &&
                (_unit_type_table[unit_type].cost < divided_budget)
            )
            {
                highest_cost = _unit_type_table[unit_type].cost;
                selected_unit_type = unit_type;
            }
        }
        tries++;
    } while((selected_unit_type == ST_UNDEFINED) && (tries < 200));
    if(tries < 200)
    {
        _LAIRS[lair_idx].guard1_unit_type = (uint8_t)selected_unit_type;
        _LAIRS[lair_idx].guard1_count = (budget / _unit_type_table[_LAIRS[lair_idx].guard1_unit_type].cost);
        if(
            (_LAIRS[lair_idx].guard1_count > 1)
            &&
            (Random(2) == 1)
        )
        {
            _LAIRS[lair_idx].guard1_count -= 1;
        }
        SETMAX(_LAIRS[lair_idx].guard1_count, 8);
        budget -= (_unit_type_table[_LAIRS[lair_idx].guard1_unit_type].cost * _LAIRS[lair_idx].guard1_count);
    }

    /* Pick Guard 2 */
    if(
        (tries < 200)
        &&
        (_LAIRS[lair_idx].guard1_count != 9)  /* OGBUG  not possible, we just set the max to 8 */
        &&
        (_LAIRS[lair_idx].guard1_count != 0)
    )
    {
        tries = 0;
        do
        {
            divided_budget = (budget / Random((10 - _LAIRS[lair_idx].guard1_count)));
            highest_cost = 0;
            selected_unit_type = ST_UNDEFINED;
            for(unit_type = 150; unit_type < NUM_UNIT_TYPES; unit_type++)
            {
                if(
                    (_unit_type_table[unit_type].race_type == race_type)
                    &&
                    (_unit_type_table[unit_type].Transport == 0)
                    &&
                    (_unit_type_table[unit_type].cost > highest_cost)
                    &&
                    (_unit_type_table[unit_type].cost < divided_budget)
                )
                {
                    if(_LAIRS[lair_idx].guard1_unit_type != unit_type)
                    {
                        highest_cost = _unit_type_table[unit_type].cost;
                        selected_unit_type = unit_type;
                    }
                }
            }
            tries++;
        } while((selected_unit_type == ST_UNDEFINED) && (tries < 200));
        if(tries < 200)
        {
            _LAIRS[lair_idx].guard2_unit_type = (uint8_t)selected_unit_type;
            _LAIRS[lair_idx].guard2_count = (budget / _unit_type_table[_LAIRS[lair_idx].guard2_unit_type].cost);
            if((_LAIRS[lair_idx].guard1_count + _LAIRS[lair_idx].guard2_count) > 9)
            {
                _LAIRS[lair_idx].guard2_count = (9 - _LAIRS[lair_idx].guard1_count);
            }
        }
    }

    if(_LAIRS[lair_idx].guard1_unit_type == 0)
    {
        _LAIRS[lair_idx].guard1_count = 0;
    }
    if(_LAIRS[lair_idx].guard2_unit_type == 0)
    {
        _LAIRS[lair_idx].guard2_count = 0;
    }
    
    loot_budget = (_unit_type_table[_LAIRS[lair_idx].guard1_unit_type].cost * _LAIRS[lair_idx].guard1_count);
    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=init_guard1                          value=%d  (g1_cost=%d * g1_count=%d)",
        (int)lair_idx, (int)loot_budget,
        (int)_unit_type_table[_LAIRS[lair_idx].guard1_unit_type].cost,
        (int)_LAIRS[lair_idx].guard1_count);
    // KNOWNBUG: this is not always the smaller part of the treasure budget
    loot_budget += ((_unit_type_table[_LAIRS[lair_idx].guard2_unit_type].cost * _LAIRS[lair_idx].guard2_count) / 2);
    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=add_guard2                           value=%d  (g2_cost=%d * g2_count=%d / 2)",
        (int)lair_idx, (int)loot_budget,
        (int)_unit_type_table[_LAIRS[lair_idx].guard2_unit_type].cost,
        (int)_LAIRS[lair_idx].guard2_count);
    if(_difficulty != god_Impossible)
    {
        loot_budget = ((loot_budget * 4) / (_difficulty + 1));
        LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=difficulty_scale                 value=%d  (* 4 / (difficulty+1) where difficulty=%d)",
            (int)lair_idx, (int)loot_budget, (int)_difficulty);
    }
    if(wp == ARCANUS_PLANE)
    {
        loot_budget = ((loot_budget * (49 + Random(76))) / 100);
        LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=plane_scale_arcanus              value=%d  (* (49 + Random(76)) / 100)",
            (int)lair_idx, (int)loot_budget);
    }
    else
    {
        loot_budget = ((loot_budget * (75 + Random(100))) / 100);
        LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=plane_scale_myrror               value=%d  (* (75 + Random(100)) / 100)",
            (int)lair_idx, (int)loot_budget);
    }
    SETMIN(loot_budget, 50);
    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=floor_50                             value=%d  (SETMIN max(value,50))",
        (int)lair_idx, (int)loot_budget);
    if(lair_type == 0)  // New Game Lair Type - Temple
    {
        Price = Random(4);
        loot_budget -= (((Price * Price) * 50) - 100);
        _LAIRS[lair_idx].Spell_n_Special = (int8_t)Price;
        LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=temple_subtract                  value=%d  (-= (Price^2 * 50) - 100 where Price=%d)",
            (int)lair_idx, (int)loot_budget, (int)Price);
    }

    while(loot_budget >= 50)
    {
        switch((Random(15) - 1))
        {
            case 0:
            case 1:
            {
                int16_t raw_price = (int16_t)(Random(20) * 10);
                int16_t budget_at_clamp = loot_budget;
                Price = raw_price;
                SETMAX(Price, loot_budget);
                Price = ((Price / 10) * 10);
                loot_budget -= 200;
                _LAIRS[lair_idx].Loot_Gold += Price;
                LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_0_1_gold              value=%d  (-= 200, Loot_Gold=%d, raw_Random20x10=%d, budget_at_clamp=%d, clamped=%d)",
                    (int)lair_idx, (int)loot_budget, (int)Price,
                    (int)raw_price, (int)budget_at_clamp, (raw_price > budget_at_clamp) ? 1 : 0);
            } break;
            case 2:
            case 3:
            {
                int16_t raw_price = (int16_t)(Random(20) * 10);
                int16_t budget_at_clamp = loot_budget;
                Price = raw_price;
                SETMAX(Price, loot_budget);
                Price = ((Price / 10) * 10);
                loot_budget -= 200;
                _LAIRS[lair_idx].Loot_Mana += Price;
                LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_2_3_mana              value=%d  (-= 200, Loot_Mana=%d, raw_Random20x10=%d, budget_at_clamp=%d, clamped=%d)",
                    (int)lair_idx, (int)loot_budget, (int)Price,
                    (int)raw_price, (int)budget_at_clamp, (raw_price > budget_at_clamp) ? 1 : 0);
            } break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            {
                // ; magical item
                // ; qualify:  300
                // ; value:    400 - 3000
                // ; max:      3
                // ; rounded down to the nearest 10 that fits the budget
                if(
                    (_LAIRS[lair_idx].Item_Count < 3)
                    &&
                    (loot_budget >= 300)
                )
                {
                    int16_t raw_price = (int16_t)(300 + (Random(27) * 100));
                    int16_t budget_at_clamp = loot_budget;
                    Price = raw_price;
                    SETMAX(Price, loot_budget);
                    Price = ((Price / 10) * 10);
                    loot_budget -= Price;
                    _LAIRS[lair_idx].Item_Values[_LAIRS[lair_idx].Item_Count] = Price;
                    _LAIRS[lair_idx].Item_Count += 1;
                    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_4_5_6_7_8_item       value=%d  (-= Price=%d, Item_Count=%d, raw_300pRandom27x100=%d, budget_at_clamp=%d, clamped=%d)",
                        (int)lair_idx, (int)loot_budget, (int)Price, (int)_LAIRS[lair_idx].Item_Count,
                        (int)raw_price, (int)budget_at_clamp, (raw_price > budget_at_clamp) ? 1 : 0);
                }
                else
                {
                    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_4_5_6_7_8_item_skip  value=%d  (no qualify: Item_Count=%d, value<300)",
                        (int)lair_idx, (int)loot_budget, (int)_LAIRS[lair_idx].Item_Count);
                }
            } break;
            case 9:  /* Misc Flags (e.g. Prisoner/Hero) */
            {
                // ; prisoner hero
                // ;
                // ; qualify:  400
                // ; value:    1000
                // ; max:      1
                if(
                    (loot_budget >= 400)
                    &&
                    (_LAIRS[lair_idx].Misc_Flags == 0)  /* Prisoner */
                )
                {
                    _LAIRS[lair_idx].Misc_Flags = 1;  /* Prisoner */
                    loot_budget -= 1000;
                    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_9_prisoner          value=%d  (-= 1000, Misc_Flags=Prisoner)",
                        (int)lair_idx, (int)loot_budget);
                }
                else
                {
                    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_9_prisoner_skip     value=%d  (no qualify: value<400 or Misc_Flags!=0)",
                        (int)lair_idx, (int)loot_budget);
                }
            } break;
            case 10:
            case 11:
            case 12:
            {
                // ; spell
                // ; qualify:  rarity * rarity * 50
                // ; value:    rarity * rarity * 50
                // ; max:      1
                // ; if there is already a spell in the hoard, the new
                // ; rarity is added to the old one, allowing higher
                // ; rarities to be awarded cheaper than they should be
                Price = Random(4);
                if(
                    (((Price * Price) * 50) <= loot_budget)
                    &&
                    (_LAIRS[lair_idx].Spell_n_Special < 4)
                )
                {
                    loot_budget -= ((Price * Price) * 50);
                    _LAIRS[lair_idx].Spell_n_Special += Price;
                    SETMAX(_LAIRS[lair_idx].Spell_n_Special, 4);
                    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_10_11_12_spell      value=%d  (-= (Price^2 * 50) where Price=%d, Spell_n_Special=%d)",
                        (int)lair_idx, (int)loot_budget, (int)Price, (int)_LAIRS[lair_idx].Spell_n_Special);
                }
                else
                {
                    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_10_11_12_spell_skip value=%d  (no qualify: Price=%d, Spell_n_Special=%d)",
                        (int)lair_idx, (int)loot_budget, (int)Price, (int)_LAIRS[lair_idx].Spell_n_Special);
                }
            } break;
            case 13:
            case 14:
            {
                // ; spellbook / retort
                // ; qualify:  1000
                // ; value:    3000
                // ; max:      2
                // ; always 2 if there's at least 2k budget remaining
                // ; (discards all other treasure in the end)
                Price = Random(4);
                if(
                    (Price == 1)
                    &&
                    (loot_budget >= 3000)
                )
                {
                    _LAIRS[lair_idx].Spell_n_Special = 6;
                    loot_budget -= 3000;
                    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_13_14_retort        value=%d  (-= 3000, Spell_n_Special=6, Price==1 path)",
                        (int)lair_idx, (int)loot_budget);
                }
                else if(loot_budget >= 2000)
                {
                    _LAIRS[lair_idx].Spell_n_Special = 6;
                    loot_budget -= 3000;
                    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_13_14_retort        value=%d  (-= 3000, Spell_n_Special=6, value>=2000 path)",
                        (int)lair_idx, (int)loot_budget);
                }
                else if(loot_budget >= 1000)
                {
                    _LAIRS[lair_idx].Spell_n_Special = 5;
                    loot_budget -= 3000;
                    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_13_14_spellbook     value=%d  (-= 3000, Spell_n_Special=5, value>=1000 path)",
                        (int)lair_idx, (int)loot_budget);
                }
                else
                {
                    LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=switch_case_13_14_skip          value=%d  (no qualify: value<1000)",
                        (int)lair_idx, (int)loot_budget);
                }
            } break;
        }
        LOG_INFO(LOG_CAT_GENERAL, "[LAIR_VAL] lair=%d step=final                                 value=%d",
            (int)lair_idx, (int)loot_budget);
    }

    /* Final capping and cleanup: if reward is very high, it may replace other loot */
    if(_LAIRS[lair_idx].Spell_n_Special > 4)
    {
        _LAIRS[lair_idx].Loot_Gold = 0;
        _LAIRS[lair_idx].Loot_Mana = 0;
        _LAIRS[lair_idx].Item_Count = 0;
        _LAIRS[lair_idx].Misc_Flags = 0;  // clear Prisoner
    }

#ifdef STU_DEBUG
    assert(Validate_Lair_Record(lair_idx) == ST_TRUE);
#endif

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p18
// drake178: UU_UNIT_RandomRacial()
// UU_UNIT_RandomRacial()


// MGC o51p19
/**
 * @brief Autotiles desert-class squares across both world planes.
 *
 * @details
 * This pass consumes TERRTYPE lookup data and applies desert-shape logic in two
 * phases:
 * - Phase 1 (`_1Desert` landmass fixup): for each `_1Desert` square, attempts to
 *   copy a non-zero neighboring landmass id into `_landmasses`.
 * - Phase 2 (desert neighborhood analysis): for each desert-class square, builds
 *   an 8-bit neighborhood mask and evaluates the corresponding TERRTYPE entry.
 *
 * Neighborhood mask bit order:
 * - `1`   = NW
 * - `2`   = N
 * - `4`   = NE
 * - `8`   = E
 * - `16`  = SE
 * - `32`  = S
 * - `64`  = SW
 * - `128` = W
 *
 * Mask bits are only added when the neighbor is inside map bounds and
 * `Square_Is_Desert_NewGame(...) == ST_FALSE` for that neighbor. A mask of
 * zero is normalized to `tt_Desert1`.
 *
 * Write-back behavior:
 * - For `mask > 0`, writes `tt_Desert_Fst + (terrtype[mask] - 2)` (= `290 + terrtype[mask]`).
 * - For `mask == 0`, writes `tt_Desert1`.
 *
 * Key local variables:
 * - `terrtype`: near-memory buffer loaded from `TERRTYPE.LBX` record 0.
 * - `mask`: accumulated 8-neighbor non-desert bitmask.
 * - `wp`, `wy`, `wx`: plane and square coordinates used for full-map scans.
 * - `neighbor_idx`: flat index of the neighbor square in `_landmasses` (Phase 1).
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 * No explicit return value. Updates global world-generation state in place.
 *
 * @note Relies on global arrays (`p_world_map`, `_landmasses`) and helper
 *       functions/macros used by map generation.
 */
void Desert_Autotile(void)
{
    int16_t * terrtype = 0;
    int16_t wy_offset = 0;
    int16_t wx_offset = 0;
    int16_t mask = 0;
    int16_t wp = 0;
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t neighbor_idx = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    terrtype = (int16_t *)Near_Allocate_First((5 * 512));
    LBX_Load_Data_Static(terrtype_lbx_file__MGC_ovr051, 0, (SAMB_ptr)terrtype, 0, 5, 512);


    /* Phase 1: Update landmasses for dry lakes */
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                if(p_world_map[wp][wy][wx] == _1Desert)  /* these were created by River_Autotile() */
                {
                    for(wy_offset = -1; wy_offset < 2; wy_offset++)
                    {
                        for(wx_offset = -1; wx_offset < 2; wx_offset++)
                        {
                            neighbor_idx = ((wp * WORLD_SIZE) + ((wy + wy_offset) * WORLD_WIDTH) + (wx + wx_offset));
                            if(_landmasses[neighbor_idx] != 0)  /* NO_LANDMASS */
                            {   
                                _landmasses[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] = _landmasses[neighbor_idx];
                            }
                        }
                    }
                }
            }
        }
    }


    /* Phase 2: Compute autotiling mask and update world maps */
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                if(
                    (p_world_map[wp][wy][wx] == tt_Desert1)
                    ||
                    (p_world_map[wp][wy][wx] == tt_Desert2)
                    ||
                    (p_world_map[wp][wy][wx] == tt_Desert3)
                    ||
                    (p_world_map[wp][wy][wx] == tt_Desert4)
                    ||
                    (p_world_map[wp][wy][wx] == _1Desert)  /* stand-alone Desert */
                )
                {
                    mask = 0;
                    if(Square_Is_Desert_NewGame((wx - 1), (wy - 1), wp) == ST_FALSE)  /* NW */
                    {
                        if(((wx - 1) >= 0) && ((wy - 1) >= 0))
                        {
                            mask += 1;
                        }
                    }
                    if(Square_Is_Desert_NewGame((wx    ), (wy - 1), wp) == ST_FALSE)  /* N */
                    {
                        if((wy - 1) >= 0)
                        {
                            mask += 2;
                        }
                    }
                    if(Square_Is_Desert_NewGame((wx + 1), (wy - 1), wp) == ST_FALSE)  /* NE */
                    {
                        if(((wx + 1) < WORLD_WIDTH) && ((wy - 1) >= 0))
                        {
                            mask += 4;
                        }
                    }
                    if(Square_Is_Desert_NewGame((wx + 1), (wy    ), wp) == ST_FALSE)  /* E */
                    {
                        if((wx + 1) < WORLD_WIDTH)
                        {
                            mask += 8;
                        }
                    }
                    if(Square_Is_Desert_NewGame((wx + 1), (wy + 1), wp) == ST_FALSE)  /* SE */
                    {
                        if(((wx + 1) < WORLD_WIDTH) && ((wy + 1) < WORLD_HEIGHT))
                        {
                            mask += 16;
                        }
                    }
                    if(Square_Is_Desert_NewGame((wx    ), (wy + 1), wp) == ST_FALSE)  /* S */
                    {
                        if((wy + 1) < WORLD_HEIGHT)
                        {
                            mask += 32;
                        }
                    }
                    if(Square_Is_Desert_NewGame((wx - 1), (wy + 1), wp) == ST_FALSE)  /* SW */
                    {
                        if(((wx - 1) >= 0) && ((wy + 1) < WORLD_HEIGHT))
                        {
                            mask += 64;
                        }
                    }
                    if(Square_Is_Desert_NewGame((wx - 1), (wy    ), wp) == ST_FALSE)  /* W */
                    {
                        if((wx - 1) >= 0)
                        {
                            mask += 128;
                        }
                    }

                    if(mask > 0)
                    {
                        /* NOTE(JimBalcomb,20260626) terrtype[] values are 2-based */
                        p_world_map[wp][wy][wx] = (tt_Desert_Fst + (terrtype[mask] - 2));
                    }
                    else
                    {
                        p_world_map[wp][wy][wx] = tt_Desert1;
                    }

                }
            }
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p20
/*
Looks to be a form of Bitmask Autotiling.
This is a popular method of smoothing that assigns binary values to neighboring positions.
The process:
* Each neighbor position gets a bit value (1, 2, 4, 8, etc.)
* The sum creates a unique index for square selection
* Supports up to 256 different square configurations (8-bit)
It looks like SimTex then had tables of specific terrain types maps to those sums, in TERRTYPE.LBX.

TODO  turn TERRTYPE.LBX into C code
sanity check the values, see if you can prove that the data for Hills got mangled

...Shore...single base terrain type...distinct Shore subtypes
...Lake

*/
/**
 * @brief Applies SimTex-style terrain autotiling passes for ocean and major land classes.
 *
 * @details
 * Loads TERRTYPE lookup data and runs multiple full-map smoothing passes on both
 * planes, converting base terrain classes into edge/corner-aware subtype tiles.
 *
 * Processing order is fixed and stateful:
 * 1) Ocean pass: computes neighborhood masks against adjacent land classes and
 *    remaps ocean/shore presentation tiles.
 * 2) Mountain pass: computes class-local adjacency masks to select mountain
 *    subtype variants.
 * 3) Tundra pass: computes tundra adjacency masks and applies tundra subtype
 *    remaps.
 * 4) Hills pass: computes hill adjacency masks and applies hill subtype remaps.
 *
 * The routine mutates `p_world_map` in place and uses TERRTYPE records loaded
 * into near-memory (`terrtype`) for mask-to-subtype mapping.
 *
 * Before autotiling, CI support may inject captured OG overrun bytes so edge
 * reads that intentionally preserve OG out-of-bounds behavior reproduce OG
 * results deterministically.
 *
 * @return void
 *
 * @note Iterates `wp` over all planes and scans every map square in each pass.
 * @note Preserves OG-compatible edge behavior used by the current implementation,
 *       including south-edge overrun-sensitive reads.
 * @warning Because passes are sequential and in-place, earlier pass outputs may
 *          influence mask inputs in later passes.
 */
void Simtex_Autotiling(void)
{
    int16_t * terrtype = 0;
    int16_t adj_wx = 0;
    int16_t mask = 0;
    int16_t wp = 0;
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t terrain_subtype_index = 0;  // DNE in Dasm

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    terrtype = (int16_t *)Near_Allocate_First((5 * 512));
    LBX_Load_Data_Static(terrtype_lbx_file__MGC_ovr051, 0, (SAMB_ptr)terrtype, 0, 5, 512);

/*
    BEGIN:  Ocean
*/
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                if(p_world_map[wp][wy][wx] == tte_Ocean)
                {
                    mask = 0;
                    // NW: {-1,-1}
                    adj_wx = (wx - 1);
                    if(adj_wx < 0) { adj_wx += WORLD_WIDTH; }
                    if(
                        (p_world_map[wp][(wy - 1)][adj_wx] >= tte_Grasslands)
                        &&
                        (p_world_map[wp][(wy - 1)][adj_wx] <= tte_Hills)
                        &&
                        ((wy - 1) >= 0)
                    )
                    {
                        mask += 1;
                    }
                    // N: {0,-1}
                    if(
                        (p_world_map[wp][(wy - 1)][wx] >= tte_Grasslands)
                        &&
                        (p_world_map[wp][(wy - 1)][wx] <= tte_Hills)
                        &&
                        ((wy - 1) >= 0)
                    )
                    {
                        mask += 2;
                    }
                    // NE: {+1,-1}
                    adj_wx = (wx + 1);
                    if(adj_wx >= WORLD_WIDTH) { adj_wx -= WORLD_WIDTH; }
                    if(
                        (p_world_map[wp][(wy - 1)][adj_wx] >= tte_Grasslands)
                        &&
                        (p_world_map[wp][(wy - 1)][adj_wx] <= tte_Hills)
                        &&
                        ((wy - 1) >= 0)
                    )
                    {
                        mask += 4;
                    }
                    // E: {+1,0}
                    adj_wx = (wx + 1);
                    if(adj_wx >= WORLD_WIDTH) { adj_wx -= WORLD_WIDTH; }
                    if(
                        (p_world_map[wp][wy][adj_wx] >= tte_Grasslands)
                        &&
                        (p_world_map[wp][wy][adj_wx] <= tte_Hills)
                    )
                    {
                        mask += 8;
                    }
                    // SE: {+1,+1}
                    adj_wx = (wx + 1);
                    if(adj_wx >= WORLD_WIDTH) { adj_wx -= WORLD_WIDTH; }
                    if(
                        (p_world_map[wp][(wy + 1)][adj_wx] >= tte_Grasslands)
                        &&
                        (p_world_map[wp][(wy + 1)][adj_wx] <= tte_Hills)
                        &&
                        (wy < WORLD_HEIGHT)  /* OGBUG  should be (wy + 1) */
                    )
                    {
                        mask += 16;
                    }
                    // S: {0,+1}
                    if(
                        (p_world_map[wp][(wy + 1)][wx] >= tte_Grasslands)
                        &&
                        (p_world_map[wp][(wy + 1)][wx] <= tte_Hills)
                        &&
                        (wy < WORLD_HEIGHT)  /* OGBUG  should be (wy + 1) */
                    )
                    {
                        mask += 32;
                    }
                    // SW: {-1,+1}
                    adj_wx = (wx - 1);
                    if(adj_wx < 0) { adj_wx += WORLD_WIDTH; }
                    if(
                        (p_world_map[wp][(wy + 1)][adj_wx] >= tte_Grasslands)
                        &&
                        (p_world_map[wp][(wy + 1)][adj_wx] <= tte_Hills)
                        &&
                        (wy < WORLD_HEIGHT)  /* OGBUG  should be (wy + 1) */
                    )
                    {
                        mask += 64;
                    }
                    // W: {-1,0}
                    adj_wx = (wx - 1);
                    if(adj_wx < 0) { adj_wx += WORLD_WIDTH; }
                    if(
                        (p_world_map[wp][wy][adj_wx] >= tte_Grasslands)
                        &&
                        (p_world_map[wp][wy][adj_wx] <= tte_Hills)
                    )
                    {
                        mask += 128;
                    }
                    if(mask > 0)
                    {
                        terrain_subtype_index = terrtype[mask];
                        p_world_map[wp][wy][wx] = terrain_subtype_index;
                    }
                }
            }
        }
    }
/*
    END:  Ocean
*/

/*
    BEGIN:  Mountain
*/
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                if(p_world_map[wp][wy][wx] != tte_Mountain)
                {
                    continue;
                }
                mask = 0;
                // NW: {-1,-1}
                if(
                    (
                        (p_world_map[wp][(wy - 1)][(wx - 1)] == tte_Mountain)
                        ||
                        (
                            (p_world_map[wp][(wy - 1)][(wx - 1)] >= _Mount0010)
                            &&
                            (p_world_map[wp][(wy - 1)][(wx - 1)] <= _Mount1001)
                        )
                    )
                    &&
                    ((wx - 1) >= 0)
                    &&
                    ((wy - 1) >= 0)
                )
                {
                    mask += 1;
                }
                // N: {0,-1}
                if(
                    (
                        (p_world_map[wp][(wy - 1)][wx] == tte_Mountain)
                        ||
                        (
                            (p_world_map[wp][(wy - 1)][wx] >= _Mount0010)
                            &&
                            (p_world_map[wp][(wy - 1)][wx] <= _Mount1001)
                        )
                    )
                    &&
                    ((wy - 1) >= 0)
                )
                {
                    mask += 2;
                }
                // NE: {1,-1}
                if(
                    (
                        (p_world_map[wp][(wy - 1)][(wx + 1)] == tte_Mountain)
                        ||
                        (
                            (p_world_map[wp][(wy - 1)][(wx + 1)] >= _Mount0010)
                            &&
                            (p_world_map[wp][(wy - 1)][(wx + 1)] <= _Mount1001)
                        )
                    )
                    &&
                    ((wx + 1) < WORLD_WIDTH)
                    &&
                    ((wy - 1) >= 0)
                )
                {
                    mask += 4;
                }
                // E: {1,0}
                if(
                    (
                        (p_world_map[wp][wy][(wx + 1)] == tte_Mountain)
                        ||
                        (
                            (p_world_map[wp][wy][(wx + 1)] >= _Mount0010)
                            &&
                            (p_world_map[wp][wy][(wx + 1)] <= _Mount1001)
                        )
                    )
                    &&
                    ((wx + 1) < WORLD_WIDTH)
                )
                {
                    mask += 8;
                }
                // SE: {1,1}
                if(
                    (
                        (p_world_map[wp][(wy + 1)][(wx + 1)] == tte_Mountain)
                        ||
                        (
                            (p_world_map[wp][(wy + 1)][(wx + 1)] >= _Mount0010)
                            &&
                            (p_world_map[wp][(wy + 1)][(wx + 1)] <= _Mount1001)
                        )
                    )
                    &&
                    ((wx + 1) < WORLD_WIDTH)
                    &&
                    (wy < WORLD_HEIGHT)  /* OGBUG should be (wy + 1) */
                )
                {

                    mask += 16;

                }
                // S: {0,1}
                if(
                    (
                        (p_world_map[wp][(wy + 1)][wx] == tte_Mountain)
                        ||
                        (
                            (p_world_map[wp][(wy + 1)][wx] >= _Mount0010)
                            &&
                            (p_world_map[wp][(wy + 1)][wx] <= _Mount1001)
                        )
                    )
                    &&
                    (wy < WORLD_HEIGHT)  /* OGBUG should be (wy + 1) */
                )
                {

                    mask += 32;

                }
                // SW: {-1,1}
                if(
                    (
                        (p_world_map[wp][(wy + 1)][(wx - 1)] == tte_Mountain)
                        ||
                        (
                            (p_world_map[wp][(wy + 1)][(wx - 1)] >= _Mount0010)
                            &&
                            (p_world_map[wp][(wy + 1)][(wx - 1)] <= _Mount1001)
                        )
                    )
                    &&
                    ((wx - 1) >= 0)
                    &&
                    (wy < WORLD_HEIGHT)  /* OGBUG should be (wy + 1) */
                )
                {
                    mask += 64;
                }
                // W: {-1,0}
                if(
                    (
                        (p_world_map[wp][wy][(wx - 1)] == tte_Mountain)
                        ||
                        (
                            (p_world_map[wp][wy][(wx - 1)] >= _Mount0010)
                            &&
                            (p_world_map[wp][wy][(wx - 1)] <= _Mount1001)
                        )
                    )
                    &&
                    ((wx - 1) >= 0)
                )
                {
                    mask += 128;
                }
                if(mask > 0)
                {
                    terrain_subtype_index = terrtype[(256 + mask)];
                    p_world_map[wp][wy][wx] = terrain_subtype_index;
                }
            }
        }
    }
/*
    END:  Mountain
*/
/*
    BEGIN:  Tundra
*/
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                if(p_world_map[wp][wy][wx] != tte_Tundra)
                {
                    continue;
                }
                mask = 0;
                // NW: {-1,-1}
                adj_wx = (wx - 1);
                if(adj_wx < 0)
                {
                    adj_wx += WORLD_WIDTH;
                }
                if(
                    (p_world_map[wp][(wy - 1)][adj_wx] != tte_Tundra)
                    &&
                    (
                        (p_world_map[wp][(wy - 1)][adj_wx] < _Tundra00001000)
                        ||
                        (p_world_map[wp][(wy - 1)][adj_wx] > _TerType_Count)
                    )
                    &&
                    ((wy - 1) >= 0)
                )
                {
                    mask += 1;
                }
                // N: {0,-1}
                if(
                    (p_world_map[wp][(wy - 1)][wx] != tte_Tundra)
                    &&
                    (
                        (p_world_map[wp][(wy - 1)][wx] < _Tundra00001000)
                        ||
                        (p_world_map[wp][(wy - 1)][wx] > _TerType_Count)
                    )
                    &&
                    ((wy - 1) >= 0)
                )
                {
                    mask += 2;
                }
                // NE: {+1,-1}
                adj_wx = (wx + 1);
                if(adj_wx >= WORLD_WIDTH)
                {
                    adj_wx -= WORLD_WIDTH;
                }
                if(
                    (p_world_map[wp][(wy - 1)][adj_wx] != tte_Tundra)
                    &&
                    (
                        (p_world_map[wp][(wy - 1)][adj_wx] < _Tundra00001000)
                        ||
                        (p_world_map[wp][(wy - 1)][adj_wx] > _TerType_Count)
                    )
                    &&
                    ((wy - 1) >= 0)
                )
                {
                    mask += 4;
                }
                // E: {+1,0}
                adj_wx = (wx + 1);
                if(adj_wx >= WORLD_WIDTH)
                {
                    adj_wx -= WORLD_WIDTH;
                }
                if(
                    (p_world_map[wp][wy][adj_wx] != tte_Tundra)
                    &&
                    (
                        (p_world_map[wp][wy][adj_wx] < _Tundra00001000)
                        ||
                        (p_world_map[wp][wy][adj_wx] > _TerType_Count)
                    )
                    &&
                    ((wx + 1) < WORLD_WIDTH)
                )
                {
                    mask += 8;
                }
                // SE: {+1,+1}
                adj_wx = (wx + 1);
                if(adj_wx >= WORLD_WIDTH)
                {
                    adj_wx -= WORLD_WIDTH;
                }
                /* OGBUG  AVRL  overflows p_world_map, because of wy=39+1 on wp=1 */
                if(
                    (p_world_map[wp][(wy + 1)][adj_wx] != tte_Tundra)
                    &&
                    (
                        (p_world_map[wp][(wy + 1)][adj_wx] < _Tundra00001000)
                        ||
                        (p_world_map[wp][(wy + 1)][adj_wx] > _TerType_Count)
                    )
                    &&
                    (wy < WORLD_HEIGHT)  /* OGBUG  ((wy + 1) < WORLD_HEIGHT) */
                )
                {
                    mask += 16;
                }
                // S: {0,+1}
                /* OGBUG  AVRL  overflows p_world_map, because of wy=39+1 on wp=1 */
                if(
                    (p_world_map[wp][(wy + 1)][wx] != tte_Tundra)
                    &&
                    (
                        (p_world_map[wp][(wy + 1)][wx] < _Tundra00001000)
                        ||
                        (p_world_map[wp][(wy + 1)][wx] > _TerType_Count)
                    )
                    &&
                    (wy < WORLD_HEIGHT)  /* OGBUG  ((wy + 1) < WORLD_HEIGHT) */
                )
                {
                    mask += 32;
                }
                // SW: {-1,+1}
                adj_wx = (wx - 1);
                if(adj_wx < 0)
                {
                    adj_wx += WORLD_WIDTH;
                }
                /* OGBUG  AVRL  overflows p_world_map, because of wy=39+1 on wp=1 */
                if(
                    (p_world_map[wp][(wy + 1)][adj_wx] != tte_Tundra)
                    &&
                    (
                        (p_world_map[wp][(wy + 1)][adj_wx] < _Tundra00001000)
                        ||
                        (p_world_map[wp][(wy + 1)][adj_wx] > _TerType_Count)
                    )
                    &&
                    (wy < WORLD_HEIGHT)  /* OGBUG  ((wy + 1) < WORLD_HEIGHT) */
                )
                {
                    mask += 64;
                }
                // W: {-1,0}
                adj_wx = (wx - 1);
                if(adj_wx < 0)
                {
                    adj_wx += WORLD_WIDTH;
                }
                if(
                    (p_world_map[wp][wy][adj_wx] != tte_Tundra)
                    &&
                    (
                        (p_world_map[wp][wy][adj_wx] < _Tundra00001000)
                        ||
                        (p_world_map[wp][wy][adj_wx] > _TerType_Count)
                    )
                )
                {
                    mask += 128;
                }
                if(mask > 0)
                {
                    p_world_map[wp][wy][wx] = (600 + terrtype[mask]);
                }
            }
        }
    }
 /*
     END:  Tundra
 */

 /*
  BEGIN:  Hills
*/
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                if(p_world_map[wp][wy][wx] != tte_Hills)
                {
                    continue;
                }
                mask = 0;
                // NW: {-1,-1}
                if(
                    (
                        (p_world_map[wp][(wy - 1)][(wx - 1)] == tte_Hills)
                        ||
                        (
                            (p_world_map[wp][(wy - 1)][(wx - 1)] >= _Hills_0010)
                            &&
                            (p_world_map[wp][(wy - 1)][(wx - 1)] <= _1Hills2)
                        )
                    )
                    &&
                    ((wx - 1) >= 0)
                    &&
                    ((wy - 1) >= 0)
                )
                {
                    mask += 1;
                }
                // N: {0,-1}
                if(
                    (
                        (p_world_map[wp][(wy - 1)][wx] == tte_Hills)
                        ||
                        (
                            (p_world_map[wp][(wy - 1)][wx] >= _Hills_0010)
                            &&
                            (p_world_map[wp][(wy - 1)][wx] <= _1Hills2)
                        )
                    )
                    &&
                    ((wy - 1) >= 0)
                )
                {
                    mask += 2;
                }
                // NE: {+1,-1}
                if(
                    (
                        (p_world_map[wp][(wy - 1)][(wx + 1)] == tte_Hills)
                        ||
                        (
                            (p_world_map[wp][(wy - 1)][(wx + 1)] >= _Hills_0010)
                            &&
                            (p_world_map[wp][(wy - 1)][(wx + 1)] <= _1Hills2)
                        )
                    )
                    &&
                    ((wx + 1) < WORLD_WIDTH)
                    &&
                    ((wy - 1) >= 0)
                )
                {
                    mask += 4;
                }
                // E: {+1,0}
                if(
                    (
                        (p_world_map[wp][wy][(wx + 1)] == tte_Hills)
                        ||
                        (
                            (p_world_map[wp][wy][(wx + 1)] >= _Hills_0010)
                            &&
                            (p_world_map[wp][wy][(wx + 1)] <= _1Hills2)
                        )
                    )
                    &&
                    ((wx + 1) < WORLD_WIDTH)
                )
                {
                    mask += 8;
                }
                // SE: {+1,+1}
                if(
                    (
                        (p_world_map[wp][(wy + 1)][(wx + 1)] == tte_Hills)
                        ||
                        (
                            (p_world_map[wp][(wy + 1)][(wx + 1)] >= _Hills_0010)
                            &&
                            (p_world_map[wp][(wy + 1)][(wx + 1)] <= _1Hills2)
                        )
                    )
                    &&
                    ((wx + 1) < WORLD_WIDTH)
                    &&
                    (wy < WORLD_HEIGHT)  /* OGBUG  ((wy + 1) < WORLD_HEIGHT) */
                )
                {
                    mask += 16;
                }
                // S: {0,+1}
                if(
                    (
                        (p_world_map[wp][(wy + 1)][wx] == tte_Hills)
                        ||
                        (
                            (p_world_map[wp][(wy + 1)][wx] >= _Hills_0010)
                            &&
                            (p_world_map[wp][(wy + 1)][wx] <= _1Hills2)
                        )
                    )
                    &&
                    (wy < WORLD_HEIGHT)  /* OGBUG  ((wy + 1) < WORLD_HEIGHT) */
                )
                {
                    mask += 32;
                }
                // SW: {-1,+1}
                if(
                    (
                        (p_world_map[wp][(wy + 1)][(wx - 1)] == tte_Hills)
                        ||
                        (
                            (p_world_map[wp][(wy + 1)][(wx - 1)] >= _Hills_0010)
                            &&
                            (p_world_map[wp][(wy + 1)][(wx - 1)] <= _1Hills2)
                        )
                    )
                    &&
                    ((wx - 1) >= 0)
                    &&
                    (wy < WORLD_HEIGHT)  /* OGBUG  ((wy + 1) < WORLD_HEIGHT) */
                )
                {
                    mask += 64;
                }
                // W: {-1,0}
                if(
                    (
                        (p_world_map[wp][wy][(wx - 1)] == tte_Hills)
                        ||
                        (
                            (p_world_map[wp][wy][(wx - 1)] >= _Hills_0010)
                            &&
                            (p_world_map[wp][wy][(wx - 1)] <= _1Hills2)
                        )
                    )
                    &&
                    ((wx - 1) >= 0)
                )
                {
                mask += 128;
                }

                /* OGBUG  no-cardinal conversion causes the square to become Grassland instead e.g., Hills pass writes 16 + terrtype[256+mask] → 0xA4 + 16 = 0xB4 = tt_Grasslands4. The lone hill becomes grassland. */
                if(mask > 0)
                {
                    p_world_map[wp][wy][wx] = (16 + terrtype[(256 + mask)]);
                }

            }
        }
    }
/*
    END:  Hills
*/

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}

// MGC o51p21
/**
 * @brief Randomizes visual terrain variants for selected base terrain classes.
 *
 * @details
 * Iterates every square on both world planes and remaps specific terrain types
 * to alternate graphic variants of the same class. This pass is cosmetic-style
 * post-processing intended to break up visual repetition after core terrain
 * generation/autotiling.
 *
 * Terrain classes processed:
 * - Grasslands: randomizes among `tte_Grasslands`, `tte_Grasslands2`,
 *   `tte_Grasslands3`, and `tte_Grasslands4`.
 * - Forest: randomizes among `tte_Forest1`, `tt_Forest2`, and `tt_Forest3`.
 * - Desert: randomizes among `tt_Desert1` through `tt_Desert4`.
 * - Swamp: randomizes among `tt_Swamp1` through `tt_Swamp3`.
 * - Tundra: attempts to randomize among tundra variants, preserving original
 *   historical bug behavior.
 *
 * The routine performs all changes in place via `p_world_map[wp][wy][wx]` and
 * uses `Random(...)` for per-square variant selection.
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 * No explicit return value. Mutates world-map terrain tiles in place.
 *
 * @note Processes both planes (`wp` in `[0, NUM_PLANES)`) and all world squares.
 * @note Preserves OG behavior where a TERRTYPE record is loaded but not used by
 *       this function body.
 * @warning Preserves OGBUG in tundra shuffle logic: the switch cases are
 *          `1, 2, 3, 4` while `shuffle = Random(3)`; this leaves one intended
 *          tundra variant path unreachable and keeps case-2 as a no-op.
 */
void Shuffle_Terrains(void)
{
    int16_t * terrtype = 0;
    int16_t shuffle = 0;
    int16_t wp = 0;
    int16_t wx = 0;
    int16_t wy = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    /* OGBUG  this terrtype load is never used (is in Desert_Autile()) */
    terrtype = (int16_t *)Near_Allocate_First(5 * 512);
    LBX_Load_Data_Static(terrtype_lbx_file__MGC_ovr051, 0, (SAMB_ptr)terrtype, 0, 5, 512);

    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                if(p_world_map[wp][wy][wx] == tte_Grasslands)
                {
                    shuffle = Random(4);
                    switch(shuffle)
                    {
                        case 1:
                        {
                            p_world_map[wp][wy][wx] = tte_Grasslands;
                        } break;
                        case 2:
                        {
                            p_world_map[wp][wy][wx] = tte_Grasslands4;
                        } break;
                        case 3:
                        {
                            p_world_map[wp][wy][wx] = tte_Grasslands2;
                        } break;
                        case 4:
                        {
                            p_world_map[wp][wy][wx] = tte_Grasslands3;
                        } break;
                    }
                }
                if(p_world_map[wp][wy][wx] == tte_Forest1)
                {
                    shuffle = Random(3);
                    switch(shuffle)
                    {
                        case 1:
                        {
                            p_world_map[wp][wy][wx] = tte_Forest1;
                        } break;
                        case 2:
                        {
                            p_world_map[wp][wy][wx] = tt_Forest2;
                        } break;
                        case 3:
                        {
                            p_world_map[wp][wy][wx] = tt_Forest3;
                        } break;
                    }
                }
                if(p_world_map[wp][wy][wx] == tt_Desert1)
                {
                    shuffle = Random(4);
                    switch(shuffle)
                    {
                        case 1:
                        {
                            p_world_map[wp][wy][wx] = tt_Desert1;
                        } break;
                        case 2:
                        {
                            p_world_map[wp][wy][wx] = tt_Desert2;
                        } break;
                        case 3:
                        {
                            p_world_map[wp][wy][wx] = tt_Desert3;
                        } break;
                        case 4:
                        {
                            p_world_map[wp][wy][wx] = tt_Desert4;
                        } break;
                    }
                }
                if(p_world_map[wp][wy][wx] == tt_Swamp1)
                {
                    shuffle = Random(3);
                    switch(shuffle)
                    {
                        case 1:
                        {
                            p_world_map[wp][wy][wx] = tt_Swamp1;
                        } break;
                        case 2:
                        {
                            p_world_map[wp][wy][wx] = tt_Swamp2;
                        } break;
                        case 3:
                        {
                            p_world_map[wp][wy][wx] = tt_Swamp3;
                        } break;
                    }
                }
                if(p_world_map[wp][wy][wx] == tt_Tundra1)  /* OGBUG  Tundra shuffle switch should be 1,2,3 not 1,3,4 */
                {
                    shuffle = Random(3);
                    switch(shuffle)
                    {
                        case 1:
                        {
                            p_world_map[wp][wy][wx] = tt_Tundra1;
                        } break;
                        case 2:
                        {
                            // DNE in Dasm
                        } break;
                        case 3:
                        {
                            p_world_map[wp][wy][wx] = tt_Tundra2;
                        } break;
                        case 4:
                        {
                            p_world_map[wp][wy][wx] = tt_Tundra3;
                        } break;
                    }
                }
            }
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p22
/**
 * @brief Attempts to generate one valid river path on the specified world plane.
 *
 * @param wp World plane index to modify.
 *
 * @return int16_t
 * @retval ST_TRUE  River path generation succeeded and placeholder river tiles were written.
 * @retval ST_FALSE River path generation failed due to invalid start square, blocked routing,
 *                  excessive retries, invalid outflow, or insufficient path length.
 *
 * @details
 * Step-by-step behavior:
 * 1) Picks a random interior base square (`4..WORLD_WIDTH-4`,`4..WORLD_HEIGHT-4`).
 * 2) Rejects immediately if the base square is terrain-special, ocean-adjacent
 *    (including center check), mountain, hills, node, or already river.
 * 3) Initializes path buffers with the base square and chooses one random
 *    downstream cardinal direction.
 * 4) Repeats routing attempts until an outflow is found or retry limits fail:
 *    - prefers continuing downstream, with occasional alternate direction picks;
 *    - forbids selecting the direct upstream opposite when deviating;
 *    - rejects candidate next squares that are special, mountain, hills, node,
 *      or desert;
 *    - accepts squares that are valid and appends them to the path.
 * 5) Marks outflow when the accepted next square touches ocean in cardinal
 *    directions or merges into an existing river.
 * 6) Enforces hard limits: more than 30 attempts, path length > 28, or outflow
 *    reached with length < 4 all fail.
 * 7) Validates the endpoint and its cardinal neighborhood with
 *    `Square_Is_Bad_River_Mouth`; any invalid side fails the whole path.
 * 8) On success, writes `1000` river placeholders for each stored path square.
 *
 * Final river graphic selection and shore outlet shaping are deferred to
 * `River_Autotile()`.
 *
 * @note
 * This function mutates global map state (`p_world_map`) and relies on multiple
 * global terrain-query helpers and direction tables.
 *
 * @see River_Autotile
 */
int16_t Generate_River(int16_t wp)
{
    int16_t niu_directions_array[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* OGBUG  as coded, completely useless */
    int16_t wy_array[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t end_wy = 0;
    int16_t end_wx = 0;
    int16_t niu_prev_dir = 0;  /* OGBUG  as coded, completely useless */
    int16_t next_wy = 0;
    int16_t next_wx = 0;
    int16_t attemps = 0;
    int16_t reached_mouth = 0;
    int16_t direction = 0;
    int16_t itr = 0;
    int16_t same_dir = 0;  /* OGBUG  as coded, should just be a 'first run' flag; looks like new_direction from Generate_Landmasses(), c&p error? */
    int16_t downstream = 0;
    int16_t base_wy = 0;
    int16_t base_wx = 0;
    int16_t length = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    base_wx = (4 + Random((WORLD_WIDTH  - 8)));  /* start 4 from left, end 4 from right */
    base_wy = (4 + Random((WORLD_HEIGHT - 8)));  /* start 4 from top, end 4 from bottom */

    /* bugged: 162, 163, 165, 180, */
    // tte_Grasslands  = 0xA2, /* 162  AKA tt_Grasslands1 */
    // tte_Forest1     = 0xA3, /* 163  AKA tt_Forest1     */
    // _AllDesert1     = 0xA5, /* 165  AKA tt_Desert1     */
    // tte_Grasslands4 = 0xB4, /* 180  AKA tt_Grasslands4 */
    if(_map_square_terrain_specials[((wp * WORLD_SIZE) + (base_wy * WORLD_WIDTH) + base_wx)] != 0) { return ST_FALSE; }
    if(Square_Is_Ocean_NewGame((base_wx - 1), (base_wy - 1), wp) == ST_TRUE) { return ST_FALSE; } /* NW */
    if(Square_Is_Ocean_NewGame((base_wx - 1), (base_wy    ), wp) == ST_TRUE) { return ST_FALSE; } /* W  */
    if(Square_Is_Ocean_NewGame((base_wx - 1), (base_wy + 1), wp) == ST_TRUE) { return ST_FALSE; } /* SW */
    if(Square_Is_Ocean_NewGame((base_wx    ), (base_wy - 1), wp) == ST_TRUE) { return ST_FALSE; } /* N  */
    if(Square_Is_Ocean_NewGame((base_wx    ), (base_wy    ), wp) == ST_TRUE) { return ST_FALSE; } /* C  */
    if(Square_Is_Ocean_NewGame((base_wx    ), (base_wy + 1), wp) == ST_TRUE) { return ST_FALSE; } /* S  */
    if(Square_Is_Ocean_NewGame((base_wx + 1), (base_wy - 1), wp) == ST_TRUE) { return ST_FALSE; } /* NE */
    if(Square_Is_Ocean_NewGame((base_wx + 1), (base_wy    ), wp) == ST_TRUE) { return ST_FALSE; } /* W  */
    if(Square_Is_Ocean_NewGame((base_wx + 1), (base_wy + 1), wp) == ST_TRUE) { return ST_FALSE; } /* SE */
    if(Square_Is_Mountain_NewGame(base_wx, base_wy, wp) == ST_TRUE) { return ST_FALSE; }
    if(Square_Is_Hills_NewGame(   base_wx, base_wy, wp) == ST_TRUE) { return ST_FALSE; }
    if(Square_Is_Node_NewGame(    base_wx, base_wy, wp) == ST_TRUE) { return ST_FALSE; }
    if(Square_Is_River_NewGame(   base_wx, base_wy, wp) == ST_TRUE) { return ST_FALSE; }

    wx_array[0] = base_wx;
    wy_array[0] = base_wy;
    length = 1;
    reached_mouth = ST_FALSE;
    attemps = 0;
    downstream = (Random(4) - 1);
    same_dir = ST_UNDEFINED;
    niu_directions_array[0] = downstream;

    while(reached_mouth == ST_FALSE)
    {

        /* Determine next direction */
        if(
            (Random(2) > 1)
            ||
            (same_dir == ST_UNDEFINED)
        )
        {
            direction = downstream;
        }
        else
        {
            /* Randomly pick direction that isn't the opposite of downstream */
            do
            {
                direction = (Random(4) - 1);
            } while(upstream[downstream] == direction);
        }

        niu_directions_array[length] = direction;

        same_dir = direction;

        /* Calculate next tile coordinates based on direction */
        next_wx = wx_array[(length - 1)] + dir_chg_tbl_wx[direction];
        next_wy = wy_array[(length - 1)] + dir_chg_tbl_wy[direction];

        niu_prev_dir = direction;

        attemps++;
        if(attemps > 30)
        {
            LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
            return ST_FALSE;
        }

        /* Ensure next tile is clear of terrain specials */
        if(_map_square_terrain_specials[((wp * WORLD_SIZE) + (next_wy * WORLD_WIDTH) + next_wx)] != 0) { continue; }
        /* Check for invalid terrain on next tile */
        if(Square_Is_Mountain_NewGame(next_wx, next_wy, wp) == ST_TRUE) { continue; }
        if(Square_Is_Hills_NewGame(   next_wx, next_wy, wp) == ST_TRUE) { continue; }
        if(Square_Is_Node_NewGame(    next_wx, next_wy, wp) == ST_TRUE) { continue; }
        if(Square_Is_Desert_NewGame(  next_wx, next_wy, wp) == ST_TRUE) { continue; }

        /* If we hit another river, we have an outflow */
        if(Square_Is_River_NewGame(next_wx, next_wy, wp) == ST_TRUE) { reached_mouth = ST_TRUE; }

        wx_array[length] = next_wx;
        wy_array[length] = next_wy;

        length++;

        if(Square_Is_Ocean_NewGame((next_wx    ), (next_wy - 1), wp) == ST_TRUE) { reached_mouth = ST_TRUE; }
        if(Square_Is_Ocean_NewGame((next_wx - 1), (next_wy    ), wp) == ST_TRUE) { reached_mouth = ST_TRUE; }
        if(Square_Is_Ocean_NewGame((next_wx    ), (next_wy    ), wp) == ST_TRUE) { reached_mouth = ST_TRUE; }
        if(Square_Is_Ocean_NewGame((next_wx + 1), (next_wy    ), wp) == ST_TRUE) { reached_mouth = ST_TRUE; }
        if(Square_Is_Ocean_NewGame((next_wx    ), (next_wy + 1), wp) == ST_TRUE) { reached_mouth = ST_TRUE; }

        if(length > 28)
        {
            LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
            return ST_FALSE;
        }

        same_dir = direction;

    }

    /* Post-generation validation */
    if(length < 4)
    {
        LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
        return ST_FALSE;
    }

    /* Ensure outflow point isn't invalid */
    end_wx = wx_array[(length - 1)];
    end_wy = wy_array[(length - 1)];
    if(Square_Is_Bad_River_Mouth((end_wx    ), (end_wy - 1), wp) == ST_TRUE)  /* N */
    {
        LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
        return ST_FALSE;
    }
    if(Square_Is_Bad_River_Mouth((end_wx - 1), (end_wy    ), wp) == ST_TRUE)  /* W */
    {
        LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
        return ST_FALSE;
    }
    if(Square_Is_Bad_River_Mouth((end_wx    ), (end_wy    ), wp) == ST_TRUE)  /* C */
    {
        LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
        return ST_FALSE;
    }
    if(Square_Is_Bad_River_Mouth((end_wx + 1), (end_wy    ), wp) == ST_TRUE)  /* E */
    {
        LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
        return ST_FALSE;
    }
    if(Square_Is_Bad_River_Mouth((end_wx    ), (end_wy + 1), wp) == ST_TRUE)  /* S */
    {
        LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
        return ST_FALSE;
    }

    /* Write river tiles to world map */
    for(itr = 0; itr < length; itr++)
    {
        p_world_map[wp][wy_array[itr]][wx_array[itr]] = tt_River_Placeholder;
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    return ST_TRUE;
}


// MGC o51p23
/**
 * @brief Finalizes river tiles and applies river/shore/lake post-processing for one plane.
 *
 * @param wp World plane index to process.
 *
 * @details
 * Step-by-step behavior:
 * 1) Iterates all squares in the selected plane.
 * 2) Reads `terrain_type` from the current square; if `Square_Is_River_NewGame`
 *    says true, treats it as `TT_RIVER_PLACEHOLDER` for this pass.
 * 3) For placeholder squares, builds a cardinal river mask (N/E/S/W) where each
 *    bit is set when the neighbor is ocean or river.
 * 4) Uses `TILE_River_Types[mask][Random(4)-1]` to pick a concrete river square
 *    and writes it back to `p_world_map`.
 * 5) Rebuilds a river-only cardinal mask (river neighbors only) for downstream
 *    lake and shore/outlet handling.
 * 6) If the original `terrain_type` was `_1Lake`, applies lake normalization:
 *    - no inflow -> `_1Desert`;
 *    - single inflow -> directional `_1LakeRiv_*`;
 *    - multi-inflow -> keeps one inflow, converts selected neighbors to
 *      `tte_Grasslands`, and rewinds `wx/wy` to revisit local topology.
 * 7) Applies multiple shore-group conversion rules that map eligible shore tiles
 *    to river-outlet shore variants based on `river_mask`; unsupported
 *    combinations may be reduced by converting adjacent river tiles to
 *    `tte_Grasslands` and rewinding iteration.
 *
 * This routine is the river/lake/shore topology cleanup pass after river paths
 * were placed.
 *
 * @note
 * This function has no return value and mutates `p_world_map` in place.
 * It is intended to run after river path placement has marked placeholder river
 * squares.
 *
 * @see Generate_River
 */
void River_Autotile(int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t river_mask = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t DBG_river_type = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    /* CLAUDE  All tile writes below use p_world_map[wp][wy][wx] (int16_t (*)[40][60], word-strided), matching the asm's `shl ax,1` / `mov [word ptr es:bx], ...` writes. The Gemini source wrote 52 of these as `_world_maps[wp*WORLD_SIZE + ...]` -- but _world_maps is uint8_t* (byte heap), so that byte-indexes a word map (half offset) and truncates values > 255 (e.g. _1Desert == 0x134; MSVC C4305). Converted all 52 to p_world_map. */

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {

        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {

            terrain_type = p_world_map[wp][wy][wx];  // NOTE can't used the MOD version here because we have to check for the 1000 river placeholder value

            if(Square_Is_River_NewGame(wx, wy, wp) == ST_TRUE)
            {
                terrain_type = TT_RIVER_PLACEHOLDER;
            }

            if(terrain_type == TT_RIVER_PLACEHOLDER)
            {
                river_mask = 0;
                if((Square_Is_Ocean_NewGame((wx    ), (wy - 1), wp) == ST_TRUE) || (Square_Is_River_NewGame((wx    ), (wy - 1), wp) == ST_TRUE)) { river_mask += 1; }  /* N */
                if((Square_Is_Ocean_NewGame((wx + 1), (wy    ), wp) == ST_TRUE) || (Square_Is_River_NewGame((wx + 1), (wy    ), wp) == ST_TRUE)) { river_mask += 2; }  /* E */
                if((Square_Is_Ocean_NewGame((wx    ), (wy + 1), wp) == ST_TRUE) || (Square_Is_River_NewGame((wx    ), (wy + 1), wp) == ST_TRUE)) { river_mask += 4; }  /* S */
                if((Square_Is_Ocean_NewGame((wx - 1), (wy    ), wp) == ST_TRUE) || (Square_Is_River_NewGame((wx - 1), (wy    ), wp) == ST_TRUE)) { river_mask += 8; }  /* W */
                // p_world_map[wp][wy][wx] = TILE_River_Types[river_mask][(Random(4) - 1)];
                DBG_river_type = TILE_River_Types[river_mask][(Random(4) - 1)];
                p_world_map[wp][wy][wx] = DBG_river_type;
            }

            river_mask = 0;

            if(Square_Is_River_NewGame((wx    ), (wy - 1), wp) == ST_TRUE) { river_mask += 1; }  /* N */
            if(Square_Is_River_NewGame((wx + 1), (wy    ), wp) == ST_TRUE) { river_mask += 2; }  /* E */
            if(Square_Is_River_NewGame((wx    ), (wy + 1), wp) == ST_TRUE) { river_mask += 4; }  /* S */
            if(Square_Is_River_NewGame((wx - 1), (wy    ), wp) == ST_TRUE) { river_mask += 8; }  /* W */

            if(terrain_type == _1Lake)  /* @@ Block_1 */
            {
                // NOTE(drake178): if the square has no river flowing into it, turn it into a desert ($134); if it has one, use the corresponding single lake square; but if it has more than one, convert surrounding tiles to grasslands until only one river inflow remains, and backtrack the loop variables to compensate since previous river tiles may need adjusting
                switch (river_mask)
                {
                case 0:
                    p_world_map[wp][wy][wx] = _1Desert;
                    break;
                case 1:
                    p_world_map[wp][wy][wx] = _1LakeRiv_N;
                    break;
                case 2:
                    p_world_map[wp][wy][wx] = _1LakeRiv_E;
                    break;
                case 3:
                    p_world_map[wp][wy][wx] = _1LakeRiv_N;
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 4:
                    p_world_map[wp][wy][wx] = _1LakeRiv_S;
                    break;
                case 5:
                    p_world_map[wp][wy][wx] = _1LakeRiv_N;
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 6:
                    p_world_map[wp][wy][wx] = _1LakeRiv_S;
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 7:
                    p_world_map[wp][wy][wx] = _1LakeRiv_N;
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 8:
                    p_world_map[wp][wy][wx] = _1LakeRiv_W;
                    break;
                case 9:
                    p_world_map[wp][wy][wx] = _1LakeRiv_N;
                    p_world_map[wp][wy][wx - 1] = tte_Grasslands;
                    wx -= 2;
                    wy--;
                    break;
                case 10:
                    p_world_map[wp][wy][wx] = _1LakeRiv_W;
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 11:
                    p_world_map[wp][wy][wx] = _1LakeRiv_N;
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    p_world_map[wp][wy][wx - 1] = tte_Grasslands;
                    wx -= 2;
                    wy--;
                    break;
                case 12:
                    p_world_map[wp][wy][wx] = _1LakeRiv_W;
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx -= 2;
                    wy--;
                    break;
                case 13:
                    p_world_map[wp][wy][wx] = _1LakeRiv_N;
                    p_world_map[wp][wy][wx - 1] = tte_Grasslands;
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx -= 2;
                    wy--;
                    break;
                case 14:
                    p_world_map[wp][wy][wx] = _1LakeRiv_W;
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx -= 2;
                    wy--;
                    break;
                case 15:
                    p_world_map[wp][wy][wx] = _1LakeRiv_N;
                    p_world_map[wp][wy][wx - 1] = tte_Grasslands;
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx -= 2;
                    wy--;
                    break;
                }
            }
            else if(terrain_type < _Shore10000000)  /* @@Block_2 */
            {
                // ¿ DEDU first Shore block/group except Lake? The game Civilation calls these a "watershed"
                // NOTE(drake178): if the square has river inflow from its only valid cardinal direction, turn it into the corresponding river outlet shore square
                // Dasm (terrain_type - 3)
                switch (terrain_type)
                {
                case 3:
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore00001R00;
                    break;
                case 4:
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore00001R10;
                    break;
                case 5:
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore00000R10;
                    break;
                case 10:
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore000R1000;
                    break;
                case 11:
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore00000R00;
                    break;
                case 12:
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore0000001R;
                    break;
                case 16:
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore001R1000;
                    break;
                case 17:
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore000R0000;
                    break;
                case 19:
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore0000000R;
                    break;
                case 20:
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore1000001R;
                    break;
                case 21:
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore001R0000;
                    break;
                case 22:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore0R000000;
                    break;
                case 23:
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore1000000R;
                    break;
                case 28:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore0R100000;
                    break;
                case 29:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore1R100000;
                    break;
                case 30:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore1R000000;
                    break;
                }
            }
            else if(terrain_type >= _Shore11000001 && terrain_type <= _Shore11101110)  /* @@Block_3 */
            {
                // NOTE(drake178): if the square has a river inflow from either or both of its two valid sides, turn it into a corresponding single or double river outlet
                // Dasm (terrain_type - 34)  ; switch 16 cases
                switch (terrain_type)
                {
                case 34:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore1R000001;
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore1100000R;
                    if(river_mask == 9) p_world_map[wp][wy][wx] = _Shore1R00000R;
                    break;
                case 35:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore1R100001;
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore1110000R;
                    if(river_mask == 9) p_world_map[wp][wy][wx] = _Shore1R10000R;
                    break;
                case 36:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore1R000011;
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore1100001R;
                    if(river_mask == 9) p_world_map[wp][wy][wx] = _Shore1R00001R;
                    break;
                case 37:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore1R100011;
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore1110001R;
                    if(river_mask == 9) p_world_map[wp][wy][wx] = _Shore1R10001R;
                    break;
                case 38:
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore000R1100;
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore00011R00;
                    if(river_mask == 6) p_world_map[wp][wy][wx] = _Shore000R1R00;
                    break;
                case 39:
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore001R1100;
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore00111R00;
                    if(river_mask == 6) p_world_map[wp][wy][wx] = _Shore000R1R10;
                    break;
                case 40:
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore000R1110;
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore00011R10;
                    if(river_mask == 6) p_world_map[wp][wy][wx] = _Shore001R1R00;
                    break;
                case 41:
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore001R1110;
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore00111R10;
                    if(river_mask == 6) p_world_map[wp][wy][wx] = _Shore001R1R10;
                    break;
                case 42:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore0R110000;
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore011R0000;
                    if(river_mask == 3) p_world_map[wp][wy][wx] = _Shore0R1R0000;
                    break;
                case 43:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore0R111000;
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore011R1000;
                    if(river_mask == 3) p_world_map[wp][wy][wx] = _Shore0R1R1000;
                    break;
                case 44:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore1R110000;
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore111R0000;
                    if(river_mask == 3) p_world_map[wp][wy][wx] = _Shore1R1R0000;
                    break;
                case 45:
                    if(river_mask == 1) p_world_map[wp][wy][wx] = _Shore1R111000;
                    if(river_mask == 2) p_world_map[wp][wy][wx] = _Shore111R1000;
                    if(river_mask == 3) p_world_map[wp][wy][wx] = _Shore1R1R1000;
                    break;
                case 46:
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore00000R11;
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore0000011R;
                    if(river_mask == 12) p_world_map[wp][wy][wx] = _Shore00000R1R;
                    break;
                case 47:
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore00001R11;
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore0000111R;
                    if(river_mask == 12) p_world_map[wp][wy][wx] = _Shore00001R1R;
                    break;
                case 48:
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore10000R11;
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore1000011R;
                    if(river_mask == 12) p_world_map[wp][wy][wx] = _Shore10000R1R;
                    break;
                case 49:
                    if(river_mask == 4) p_world_map[wp][wy][wx] = _Shore10001R11;
                    if(river_mask == 8) p_world_map[wp][wy][wx] = _Shore1000111R;
                    if(river_mask == 12) p_world_map[wp][wy][wx] = _Shore10001R1R;
                    break;
                }
            }
            else if(terrain_type >= _Shore11101110 && terrain_type <= _Shore01000100)  /* @@Block_4 */
            {
                if(river_mask == 1)
                {
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1R101110 - _Shore11101110);
                }
                else if(river_mask == 4)
                {
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore11101R10 - _Shore11101110);
                }
                else if(river_mask == 5)
                {
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1R101110 - _Shore11101110);
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx--;
                    wy--;
                }
            }
            else if(terrain_type >= _Shore10010011 && terrain_type <= _Shore00111001)  /* @@Block_5 */
            {
                if(river_mask == 2)
                {
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore100R0011 - _Shore10010011);
                }
                else if(river_mask == 8)
                {
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1001001R - _Shore10010011);
                }
                else if(river_mask == 10)
                {
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1001001R - _Shore10010011);
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx--;
                    wy--;
                }
            }
            else if(terrain_type == _Shore00011111 || terrain_type == _Shore10011111 || terrain_type == _Shore00111111 || terrain_type == _Shore10111111)  /* @@Block_6 */
            {
                switch (river_mask)
                {
                case 2:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore000R1111 - _Shore00011111);
                    break;
                case 4:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore00011R11 - _Shore00011111);
                    break;
                case 8:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore0001111R - _Shore00011111);
                    break;
                case 6:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore00011R11 - _Shore00011111);
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 10:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore0001111R - _Shore00011111);
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 12:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore0001111R - _Shore00011111);
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 14:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore00011R11 - _Shore00011111);
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                }
            }
            else if(terrain_type == _Shore11000111 || terrain_type == _Shore11100111 || terrain_type == _Shore11001111 || terrain_type == _Shore11101111)  /* @@Block_7 */
            {
                switch (river_mask)
                {
                case 1:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1R000111 - _Shore11000111);
                    break;
                case 4:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore11000R11 - _Shore11000111);
                    break;
                case 8:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1100011R - _Shore11000111);
                    break;
                case 5:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1R000111 - _Shore11000111);
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 9:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1R000111 - _Shore11000111);
                    p_world_map[wp][wy][wx - 1] = tte_Grasslands;
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx -= 2;
                    wy--;
                    break;
                case 12:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1100011R - _Shore11000111);
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 13:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1R000111 - _Shore11000111);
                    p_world_map[wp][wy][wx - 1] = tte_Grasslands;
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx -= 2;
                    wy--;
                    break;
                }
            }
            else if(terrain_type == _Shore11110001 || terrain_type == _Shore11111001 || terrain_type == _Shore11110011 || terrain_type == _Shore11111011)  /* @@Block_8 */
            {
                switch (river_mask)
                {
                case 1:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1R110001 - _Shore11110001);
                    break;
                case 2:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore111R0001 - _Shore11110001);
                    break;
                case 8:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1111000R - _Shore11110001);
                    break;
                case 3:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1R110001 - _Shore11110001);
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 9:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1R110001 - _Shore11110001);
                    p_world_map[wp][wy][wx - 1] = tte_Grasslands;
                    wx -= 2;
                    wy--;
                    break;
                case 10:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1111000R - _Shore11110001);
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 11:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore1R110001 - _Shore11110001);
                    p_world_map[wp][wy][wx - 1] = tte_Grasslands;
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx -= 2;
                    wy--;
                    break;
                }
            }
            else if(terrain_type == _Shore01111100 || terrain_type == _Shore01111110 || terrain_type == _Shore11111100 || terrain_type == _Shore11111110)  /* @@Block_9 */
            {
                switch (river_mask)
                {
                case 1:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore0R111100 - _Shore01111100);
                    break;
                case 2:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore011R1100 - _Shore01111100);
                    break;
                case 4:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore01111R00 - _Shore01111100);
                    break;
                case 3:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore0R111100 - _Shore01111100);
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 5:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore0R111100 - _Shore01111100);
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 6:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore01111R00 - _Shore01111100);
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                case 7:
                    p_world_map[wp][wy][wx] = terrain_type + (_Shore0R111100 - _Shore01111100);
                    p_world_map[wp][wy][wx + 1] = tte_Grasslands;
                    p_world_map[wp][wy + 1][wx] = tte_Grasslands;
                    wx--;
                    wy--;
                    break;
                }
            }

        }

    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p24
/**
 * @brief Generates up to 15 neutral cities on the selected world plane.
 *
 * @details
 * This new-game pass chooses candidate map squares, validates placement
 * constraints, creates neutral-city records, initializes buildings and
 * enchantments, then spawns a starting garrison and assigns a race-specific
 * city name.
 *
 * High-level behavior:
 * - Initializes `m_landmasses_default_race[1..199]` with randomized default
 *   races for the selected plane (`wp`).
 * - Attempts to place 15 cities, with a global retry limit of 1000 attempts.
 * - Rejects candidate squares that are ocean/shore or too close to existing
 *   capitals, cities, nodes, towers, or lairs.
 * - Chooses city race from plane-specific random tables, then overrides with
 *   the landmass default race 75% of the time.
 * - Initializes city ownership, population/size, enchantment flags,
 *   construction state, and building status.
 * - Applies race build restrictions, then validates building prerequisites.
 * - Selects best recruitable melee/ranged units and creates an initial
 *   neutral garrison.
 * - Assigns a random city name by race and increments the global city count.
 *
 * @param wp World plane index to populate (`ARCANUS_PLANE` or
 *           `MYRROR_PLANE`).
 *
 * @return void
 *
 * @note Mutates global game state including `_CITIES`, `_cities`,
 *       `m_landmasses_default_race`, and unit stacks created via
 *       `Create_Unit_NewGame(...)`.
 * @note Returns early if placement attempts exceed internal limits.
 * @warning Preserves known legacy quirks documented in-code (for example,
 *          race-table oddities and building-prerequisite edge cases).
 */
void Generate_Neutral_Cities(int16_t wp)
{
    int16_t location_is_forest_square = 0;
    int16_t tries = 0;
    int16_t Best_Ranged_Unit = 0;
    int16_t Best_Melee_Unit = 0;
    int16_t itr3 = 0;
    int16_t reject = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t itr2 = 0;
    int16_t itr1 = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);


    /* Phase 1: Init */
    tries = 0;


    /* Phase 2: Initialize landmass default races based on plane */
    /* 0x High Elves, 2x High Men */
    for(itr1 = 1; itr1 < 200; itr1++)
    {
        if(wp == ARCANUS_PLANE)
        {
            switch((Random(NUM_RACES_ARCANUS) - 1))
            {
                case  0: { m_landmasses_default_race[itr1] = rt_Barbarian; } break;
                case  1: { m_landmasses_default_race[itr1] = rt_Gnoll;     } break;
                case  2: { m_landmasses_default_race[itr1] = rt_Halfling;  } break;
                case  3: { m_landmasses_default_race[itr1] = rt_High_Men;  } break;  /* NOTE: no Forest check, no High Elves */
                case  4: { m_landmasses_default_race[itr1] = rt_High_Men;  } break;
                case  5: { m_landmasses_default_race[itr1] = rt_Klackon;   } break;
                case  6: { m_landmasses_default_race[itr1] = rt_Lizardman; } break;
                case  7: { m_landmasses_default_race[itr1] = rt_Nomad;     } break;
                case  8: { m_landmasses_default_race[itr1] = rt_Orc;       } break;
                case  9: { m_landmasses_default_race[itr1] = rt_Barbarian; } break;
                case 10: { m_landmasses_default_race[itr1] = rt_High_Men;  } break;
                case 11: { m_landmasses_default_race[itr1] = rt_Nomad;     } break;
                case 12: { m_landmasses_default_race[itr1] = rt_High_Men;  } break;
            }

        }
        else  /* MYRROR_PLANE */
        {
            switch((Random(NUM_RACES_MYRROR) - 1))
            {
                case 0: { m_landmasses_default_race[itr1] = rt_Beastmen;  } break;
                case 1: { m_landmasses_default_race[itr1] = rt_Dark_Elf;  } break;
                case 2: { m_landmasses_default_race[itr1] = rt_Draconian; } break;
                case 3: { m_landmasses_default_race[itr1] = rt_Dwarf;     } break;
                case 4: { m_landmasses_default_race[itr1] = rt_Troll;     } break;
            }
        }
    }


    /* Phase 3: Place 15 Neutral Cities */
    for(itr1 = 0; itr1 < 15; itr1++)
    {
        if(tries >= 1000) { goto Done; }
        do
        {
            tries++;
            wx = (3 + Random(54));
            wy = (3 + Random(34));
            /* Phase ?: Sanity Checks */
            /* IDA  Orange */
            reject = ST_FALSE;

            /* Check terrain validity */
            if(TERRAIN_TYPE(wx, wy, wp) == tt_Ocean)
            {
                reject = ST_TRUE;
            }
            /* OGBUG  there are no such map squares yet at this stage */
            if((TERRAIN_TYPE(wx, wy, wp) >= tt_Shore1_Fst) && (TERRAIN_TYPE(wx, wy, wp) <= tt_Shore1_Lst)
            )
            {
                reject = ST_TRUE;
            }

            /* Distance checks */
            for(itr2 = 0; itr2 < _num_players; itr2++)
            {
                if(Range(wx, wy, _FORTRESSES[itr2].wx, _FORTRESSES[itr2].wy) < 5)
                {
                    reject = ST_TRUE;
                }
            }
            for(itr2 = 0; itr2 < _cities; itr2++)
            {
                if(_CITIES[itr2].wp == wp)
                {
                    if(Delta_XY_With_Wrap(wx, wy, _CITIES[itr2].wx, _CITIES[itr2].wy, WORLD_WIDTH) < 4)
                    {
                        reject = ST_TRUE;
                    }
                }
            }
            for(itr2 = 0; itr2 < NUM_NODES; itr2++)
            {
                if(_NODES[itr2].wp == wp)
                {
                    if(Range(wx, wy, _NODES[itr2].wx, _NODES[itr2].wy) < 4)
                    {
                        reject = ST_TRUE;
                    }
                }
            }
            for(itr2 = 0; itr2 < NUM_TOWERS; itr2++)
            {
                if(Range(wx, wy, _TOWERS[itr2].wx, _TOWERS[itr2].wy) < 4)
                {
                    reject = ST_TRUE;
                }
            }
            for(itr2 = 0; itr2 < NUM_LAIRS; itr2++)
            {
                if(_LAIRS[itr2].wp == wp)
                {
                    if(Range(wx, wy, _LAIRS[itr2].wx, _LAIRS[itr2].wy) < 4)
                    {
                        reject = ST_TRUE;
                    }
                }
            }

        } while(reject == ST_TRUE && tries < 1000);

        if(tries >= 1000) { goto Done; }


        /* Phase 4: Initialize City Data */
        location_is_forest_square = Square_Is_Forest_NewGame(wx, wy, wp);
        if(wp == ARCANUS_PLANE)
        {
            switch((Random(NUM_RACES_ARCANUS) - 1))
            {
                case 0: { _CITIES[_cities].race = rt_Barbarian; } break;
                case 1: { _CITIES[_cities].race = rt_Gnoll;     } break;
                case 2: { _CITIES[_cities].race = rt_Halfling;  } break;
                // case 3: { _CITIES[_cities].race = rt_High_Elf;  } break;
                case 3: {
                    if(location_is_forest_square == ST_TRUE)
                    {
                        _CITIES[_cities].race = rt_High_Elf;
                    }
                    else
                    {
                        _CITIES[_cities].race = rt_High_Men;
                    }
                } break;
                case 4: { _CITIES[_cities].race = rt_High_Men;  } break;
                case 5: { _CITIES[_cities].race = rt_Klackon;   } break;
                case 6: { _CITIES[_cities].race = rt_Lizardman; } break;
                case 7: { _CITIES[_cities].race = rt_Nomad;     } break;
                case 8: { _CITIES[_cities].race = rt_Orc;       } break;
                // WTF?
                case  9: { _CITIES[_cities].race = rt_Barbarian; } break;
                case 10: { _CITIES[_cities].race = rt_High_Men;  } break;
                case 11: { _CITIES[_cities].race = rt_Nomad;     } break;
                case 12: { _CITIES[_cities].race = rt_High_Men;  } break;
            }

        }
        else
        {
            switch((Random(NUM_RACES_MYRROR) - 1))
            {
                case 0: { _CITIES[_cities].race = rt_Beastmen;  } break;
                case 1: { _CITIES[_cities].race = rt_Dark_Elf;  } break;
                case 2: { _CITIES[_cities].race = rt_Draconian; } break;
                case 3: { _CITIES[_cities].race = rt_Dwarf;     } break;
                case 4: { _CITIES[_cities].race = rt_Troll;     } break;
            }
        }

        /* 75% chance of default race */
        if(Random(4) > 1)
        {
            _CITIES[_cities].race = (int8_t)m_landmasses_default_race[GET_LANDMASS(wx, wy, wp)];
        }

        _CITIES[_cities].wx = (int8_t)wx;
        _CITIES[_cities].wy = (int8_t)wy;
        _CITIES[_cities].wp = (int8_t)wp;
        _CITIES[_cities].owner_idx = NEUTRAL_PLAYER_IDX;
        _CITIES[_cities].population = (1 + ((_difficulty + 1) / 3) + Random(4));  /* OGBUG  ignores terrain */
        if((_difficulty > god_Normal) && (Random(5) == 1))
        {
            _CITIES[_cities].population = (((_difficulty + 1) / 3) + Random(10));  /* OGBUG  ignores terrain */
        }
        _CITIES[_cities].size = (_CITIES[_cities].population / 4);
        _CITIES[_cities].bldg_cnt = 0;
        _CITIES[_cities].enchantments[WALL_OF_DARKNESS] = ST_FALSE;
        _CITIES[_cities].enchantments[WALL_OF_FIRE] = ST_FALSE;
        _CITIES[_cities].enchantments[CHAOS_RIFT] = ST_FALSE;
        _CITIES[_cities].enchantments[DARK_RITUALS] = ST_FALSE;
        _CITIES[_cities].enchantments[EVIL_PRESENCE] = ST_FALSE;
        _CITIES[_cities].enchantments[CURSED_LANDS] = ST_FALSE;
        _CITIES[_cities].enchantments[PESTILENCE] = ST_FALSE;
        _CITIES[_cities].enchantments[CLOUD_OF_SHADOW] = ST_FALSE;
        _CITIES[_cities].enchantments[ALTAR_OF_BATTLE] = ST_FALSE;
        _CITIES[_cities].enchantments[FAMINE] = ST_FALSE;
        _CITIES[_cities].enchantments[FLYING_FORTRESS] = ST_FALSE;
        _CITIES[_cities].enchantments[DEATH_WARD] = ST_FALSE;
        _CITIES[_cities].enchantments[CHAOS_WARD] = ST_FALSE;
        _CITIES[_cities].enchantments[LIFE_WARD] = ST_FALSE;
        _CITIES[_cities].enchantments[SORCERY_WARD] = ST_FALSE;
        _CITIES[_cities].enchantments[NATURE_WARD] = ST_FALSE;
        _CITIES[_cities].enchantments[NATURES_EYE] = ST_FALSE;
        _CITIES[_cities].enchantments[EARTH_GATE] = ST_FALSE;
        _CITIES[_cities].enchantments[STREAM_OF_LIFE] = ST_FALSE;
        _CITIES[_cities].enchantments[GAIAS_BLESSING] = ST_FALSE;
        _CITIES[_cities].enchantments[INSPIRATIONS] = ST_FALSE;
        _CITIES[_cities].enchantments[PROSPERITY] = ST_FALSE;
        _CITIES[_cities].enchantments[ASTRAL_GATE] = ST_FALSE;
        _CITIES[_cities].enchantments[HEAVENLY_LIGHT] = ST_FALSE;
        _CITIES[_cities].enchantments[CONSECRATION] = ST_FALSE;
        _CITIES[_cities].enchantments[NIGHTSHADE] = ST_FALSE;
        _CITIES[_cities].construction = bt_AUTOBUILD;
        for(itr2 = 0; itr2 < NUM_BUILDINGS; itr2++)
        {
            _CITIES[_cities].bldg_status[itr2] = bs_NotBuilt;
        }
        /* Grant buildings based on population (waterfall switch logic) */
        _CITIES[_cities].bldg_status[bt_NONE] = bs_Replaced;
        switch(_CITIES[_cities].population - 2)
        {
            case 24:
            case 23:
            case 22:
            case 21:
            case 20:
            case 19:
            case 18:
            case 17:
            case 16:
            case 15:
            case 14:
            case 13:
            case 12: { _CITIES[_cities].bldg_status[bt_Shrine] = bs_Built; }
            case 11: { _CITIES[_cities].bldg_status[bt_ArmorersGuild] = bs_Built; }
            case 10: { _CITIES[_cities].bldg_status[bt_FightersGuild] = bs_Built; }
            case  9:
            case  8: { _CITIES[_cities].bldg_status[bt_CityWalls] = bs_Built; }
            case  7:
            case  6: { _CITIES[_cities].bldg_status[bt_Stable] = bs_Built; }
            case  5: { _CITIES[_cities].bldg_status[bt_Granary] = bs_Built; }
            case  4:
            case  3: { _CITIES[_cities].bldg_status[bt_Armory] = bs_Built; }
            case  2: { _CITIES[_cities].bldg_status[bt_BuildersHall] = bs_Built; }
            case  1: { _CITIES[_cities].bldg_status[bt_Smithy] = bs_Built; }
            case  0: { _CITIES[_cities].bldg_status[bt_Barracks] = bs_Built; }
        }

        /* Prune race-restricted buildings */
        for(itr3 = 0; itr3 < _race_type_table[_CITIES[_cities].race].cant_build_count; itr3++)
        {
            _CITIES[_cities].bldg_status[_race_type_table[_CITIES[_cities].race].cant_build[itr3]] = bs_NotBuilt;
        }

        /* Enforce building prerequisites */
        /* OGBUG  buildings with a terrain requirement will be randomly removed through a read out of the structure bounds, whereas if they also have a building req, that will be ignored completely */
        for(itr2 = 1; itr2 < NUM_BUILDINGS; itr2++)
        {
            if(_CITIES[_cities].bldg_status[itr2] == bs_Built)
            {
                /* ¿ BUGBUG  code should not be same on both branches ?  maybe, was two different fields, then got merged into a union? ...or, should just be checking reqd_bldg_2, because the first one is reqd_terrain and not reqd_bldg_1 */
                if(bldg_data_table[itr2].reqd_bldg_1 > 100)  // bldg_idx  >= 100 is Terrain Type
                {
                    if(_CITIES[_cities].bldg_status[bldg_data_table[itr2].reqd_terrain] == bs_NotBuilt)  /* OGBUG  OOB AVRL */
                    {
                        _CITIES[_cities].bldg_status[itr2] = bs_NotBuilt;
                    }
                }
                else
                {
                    if(
                        (_CITIES[_cities].bldg_status[bldg_data_table[itr2].reqd_bldg_1] == bs_NotBuilt)
                        ||
                        (_CITIES[_cities].bldg_status[bldg_data_table[itr2].reqd_bldg_2] == bs_NotBuilt)
                    )
                    {
                        _CITIES[_cities].bldg_status[itr2] = bs_NotBuilt;
                    }
                }
            }
        }

        /* Handle building upgrades (mark lower tier as Replaced) */
        /* OGBUG only handles one level, misses prereqs that have prereqs */
        for(itr2 = 1; itr2 < NUM_BUILDINGS; itr2++)
        {
            if(
                (bldg_data_table[itr2].replace_bldg > bt_NONE)
                &&
                (_CITIES[_cities].bldg_status[itr2] == bs_Built)  // ; ignores replaced buildings
                &&
                (_CITIES[_cities].bldg_status[bldg_data_table[itr2].replace_bldg] == bs_Built)
            )
            {
                _CITIES[_cities].bldg_status[bldg_data_table[itr2].replace_bldg] = bs_Replaced;
            }
        }

/*
    BEGIN:  Garrison
*/

        /* Phase 5: Select Garrison Units */
        Best_Melee_Unit = 0;
        for(itr2 = ut_BarbSwordsmen; itr2 < ut_Magic_Spirit; itr2++)
        {

            if(
                (_unit_type_table[itr2].race_type == _CITIES[_cities].race)
                &&
                (
                    (_unit_type_table[itr2].reqd_bldg_1 == 0)
                    ||
                    (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_1] == bs_Built)
                    ||
                    (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_1] == bs_Replaced)
                )
                &&
                (
                    (_unit_type_table[itr2].reqd_bldg_2 == 0)
                    ||
                    (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_2] == bs_Built)
                    ||
                    (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_2] == bs_Replaced)
                )
                &&
                ((_unit_type_table[itr2].Abilities & UA_CREATEOUTPOST) == 0)
                &&
                (_unit_type_table[itr2].Construction == 0)
                &&
                (_unit_type_table[itr2].Transport == 0)
                &&
                (
                    (_unit_type_table[itr2].Ranged_Type == rat_UNDEF)
                    ||
                    (_unit_type_table[itr2].Ranged_Type >= srat_Thrown)
                )
            )
            {
                Best_Melee_Unit = itr2;
            }
        }
        Best_Ranged_Unit = 0;
        for(itr2 = ut_BarbSwordsmen; itr2 < ut_Magic_Spirit; itr2++)
        {
            if(
                (_unit_type_table[itr2].race_type == _CITIES[_cities].race)
                &&
                (
                    (_unit_type_table[itr2].reqd_bldg_1 == 0)
                    ||
                    (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_1] == bs_Built)
                    ||
                    (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_1] == bs_Replaced)
                )
                &&
                (
                    (_unit_type_table[itr2].reqd_bldg_2 == 0)
                    ||
                    (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_2] == bs_Built)
                    ||
                    (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_2] == bs_Replaced)
                )
                &&
                ((_unit_type_table[itr2].Abilities & UA_CREATEOUTPOST) == 0)
                &&
                (_unit_type_table[itr2].Construction == 0)
                &&
                (_unit_type_table[itr2].Transport == 0)
                &&
                (
                    (_unit_type_table[itr2].Ranged_Type > rat_NONE)
                    &&
                    (_unit_type_table[itr2].Ranged_Type < srat_Thrown)
                )
            )
            {
                Best_Ranged_Unit = itr2;
            }
        }


        /* Phase 6: Spawn Garrison Units */
        /* OGBUG  dark elves will have no such units, resulting in half the intended starting garrison */
        for(itr2 = 0; (((_CITIES[_cities].population / 4) > itr2) && (itr2 < MAX_STACK)); itr2++)
        {
            Create_Unit_NewGame(Best_Melee_Unit, NEUTRAL_PLAYER_IDX, _CITIES[_cities].wx, _CITIES[_cities].wy, _CITIES[_cities].wp, _cities);
        }
        if(Best_Ranged_Unit == 0)
        {
            /* OGBUG  dark elves will have no such units, resulting in half the intended starting garrison */
            for(itr2 = 0; (((_CITIES[_cities].population / 4) > itr2) && (itr2 < MAX_STACK)); itr2++)
            {
                Create_Unit_NewGame(Best_Melee_Unit, NEUTRAL_PLAYER_IDX, _CITIES[_cities].wx, _CITIES[_cities].wy, _CITIES[_cities].wp, _cities);
            }
        }
        else
        {
            // ; WARNING: the sawmill is not included in the default
            // ;  building list, and the shrine can't be reached with
            // ;  the generated populations, so there won't ever be
            // ;  a ranged attack unit for any race other than dark
            // ;  elves, who only have these and no close combat ones
            for(itr2 = 0; (((_CITIES[_cities].population / 4) > itr2) && (itr2 < MAX_STACK)); itr2++)
            {
                Create_Unit_NewGame(Best_Ranged_Unit, NEUTRAL_PLAYER_IDX, _CITIES[_cities].wx, _CITIES[_cities].wy, _CITIES[_cities].wp, _cities);
            }
        }

/*
    END:  Garrison
*/

        /* Phase 7: Finalize City */
        Random_City_Name_By_Race_NewGame(_CITIES[_cities].race, _CITIES[_cities].name);
        _cities++;

    }

Done:
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p25
void Random_City_Name_By_Race_NewGame(int16_t race_idx, char * name)
{
    char buffer[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t city_name_idx = 0;
    int16_t attempts = 0;
    char * city_names_buffer = 0;
    int16_t itr_cities = 0;
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    city_names_buffer = (char *)Near_Allocate_First(SZ_CITY_NAME_RECORD);
    LBX_Load_Data_Static(cityname_lbx_file__MGC_ovr051, 0, (SAMB_ptr)city_names_buffer, race_idx, 1, SZ_CITY_NAME_RECORD);
    attempts = 0;
attempt:
    attempts++;
    city_name_idx = (Random(20) - 1);
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        stu_strcpy(buffer, _CITIES[itr_cities].name);
        if(stu_stricmp(buffer, &city_names_buffer[(city_name_idx * LEN_CITY_NAME)]) == 0)
        {
            if(attempts < 200)
            {
                goto attempt;
            }
            else
            {
                LBX_Load_Data_Static(cityname_lbx_file__MGC_ovr051, 0, (SAMB_ptr)city_names_buffer, (Random(NUM_RACES) - 1), 1, SZ_CITY_NAME_RECORD);
            }
        }
    }
    stu_strcpy(name, &city_names_buffer[(city_name_idx * LEN_CITY_NAME)]);
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
}


// MGC o51p26
/**
 * @brief Generates overland road flags between nearby neutral cities on a world plane.
 *
 * @details
 * Clears all map-square flags on the selected plane, then iterates city pairs and attempts
 * to build a road path between each valid pair. A pair is considered valid only when both
 * cities are on the target plane, both are neutral, on the same landmass, and within
 * short range. The path returned by `Path_Wrap()` is rejected if any traversed square is
 * ocean or shore terrain. Accepted paths set `MSF_ROAD`, and on Myrror also set `MSF_EROAD`.
 *
 * After pair processing, each city square is marked with road flags so city locations are always road-connected tiles.
 *
 * @param wp World plane index (`ARCANUS_PLANE` or `MYRROR_PLANE`) to process.
 *
 * @note Uses fixed-size temporary path buffers of 70 elements.
 * @note Contains known behavior bugs preserved for compatibility (see in-code `NOTE` markers).
 * @return This function does not return a value.
 */
void Generate_Roads(int16_t wp)
{
    int8_t path_wy_array[70] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t path_wx_array[70] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t dst_wy = 0;
    int16_t dst_wx = 0;
    int16_t src_wy = 0;
    int16_t src_wx = 0;
    int16_t city_idx = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t path_idx = 0;
    int16_t path_length = 0;
    int16_t reject = 0;
    int16_t dst_city_idx = 0;
    int16_t src_city_idx = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);


    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            SET_MAP_SQUARE_FLAG(wx, wy, wp, 0);
        }
    }
    for(src_city_idx = 0; src_city_idx < _cities; src_city_idx++)
    {
        src_wx = _CITIES[src_city_idx].wx;
        src_wy = _CITIES[src_city_idx].wy;
        for(dst_city_idx = (src_city_idx + 1); dst_city_idx < _cities; dst_city_idx++)
        {
            dst_wx = _CITIES[dst_city_idx].wx;
            dst_wy = _CITIES[dst_city_idx].wy;
            if(
                (_CITIES[src_city_idx].wp != wp)
                ||
                (_CITIES[dst_city_idx].wp != wp)
                ||
                (_CITIES[src_city_idx].owner_idx != NEUTRAL_PLAYER_IDX)
                ||
                (_CITIES[dst_city_idx].owner_idx != NEUTRAL_PLAYER_IDX)
                ||
                (GET_LANDMASS(src_wx, src_wy, wp) != GET_LANDMASS(dst_wx, dst_wy, wp))
                ||
                (Range(src_wx, src_wy, dst_wx, dst_wy) >= 11)  /* OGBUG  should used Delta_XY_With_Wrap() */
            )
            {
                continue;
            }
            path_length = Path_Wrap(src_wx, src_wy, dst_wx, dst_wy, &path_wx_array[0], &path_wy_array[0], WORLD_WIDTH);
            reject = 0;
            path_idx = 0;
            // NOTE(drake178): invalidate the road if it would pass through an ocean or shore map square
            // NOTE(drake178): contains exclusions for map square types that will never be present on the map at this stage of the process
            while(((path_length - 1) > path_idx) && (reject == ST_FALSE))
            {
                wx = path_wx_array[path_idx];
                wy = path_wy_array[path_idx];
                if(
                    (GET_TERRAIN_TYPE(wx, wy, wp) >= tt_Shore1_Fst)
                    &&
                    (GET_TERRAIN_TYPE(wx, wy, wp) <= tt_Shore1_Lst)
                )
                {
                    reject = ST_TRUE;
                }
                if(GET_TERRAIN_TYPE(wx, wy, wp) == tt_Ocean)
                {
                    reject = ST_TRUE;
                }
                /* OGBUG  this batch should start at $C5 (not that any of these can be present on the map at this stage) */
                if(
                    (GET_TERRAIN_TYPE(wx, wy, wp) >= _Shore00001R10)
                    &&
                    (GET_TERRAIN_TYPE(wx, wy, wp) <= tt_Shore2F_end)
                )
                {
                    reject = ST_TRUE;
                }
                if(
                    (GET_TERRAIN_TYPE(wx, wy, wp) >= tt_Shore2_1st)
                    &&
                    (GET_TERRAIN_TYPE(wx, wy, wp) <= tt_OceanAnim)
                )
                {
                    reject = ST_TRUE;
                }
                path_idx++;
            }
            if(reject == ST_FALSE)
            {
                for(path_idx = 0; ((path_length - 1) > path_idx); path_idx++)
                {
                    wx = path_wx_array[path_idx];
                    wy = path_wy_array[path_idx];
                    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) | MSF_ROAD));
                    if(wp == MYRROR_PLANE)
                    {
                        SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) | MSF_EROAD));
                    }
                }
            }
        }
    }
    /* OGBUG  ignores the passed plane value, and instead processes every city */
    /* OGBUG  Myrran roads will have normal roads as the map square coordinates upgraded are not set up before doing so */
    for(city_idx = 0; city_idx < _cities; city_idx++)
    {
        SET_MAP_SQUARE_FLAG(
            _CITIES[city_idx].wx,
            _CITIES[city_idx].wy,
            _CITIES[city_idx].wp,
            (
                GET_MAP_SQUARE_FLAG(
                    _CITIES[city_idx].wx,
                    _CITIES[city_idx].wy,
                    _CITIES[city_idx].wp
                )
                |
                MSF_ROAD
            )
        );
/*
https://masterofmagic.fandom.com/wiki/Roads?veaction=edit&section=19
Roads beneath captured neutral cities and original Fortress sites on Myrror lack the characteristic pulsing gold animation of Enchanted Roads (see picture). The New Game sequence fails to correctly implement the enchantment for Roads beneath towns.
*/
        if(wp == MYRROR_PLANE)
        {
            /* OGBUG  these are not the city coordinates */
            /* OGBUG  at exactly wx=60,wy=40,wp=1 this causes an AVRL */
/*
If no city pair qualifies on Myrror (no roads built), wx and wy retain (60, 40) from the flag-clear loop's exit.
SET_MAP_SQUARE_FLAG(60, 40, 1, …) indexes _map_square_flags[(1*2400)+(40*60)+60 = 4860] — past the logical 4800.
_map_square_flags is allocated with + WORLD_WIDTH + 1 padding (ALLOC.c, STU_WRLD.c) = 4880 bytes, which covers index 4860, so the OOB lands in padding instead of faulting.
*/
            SET_MAP_SQUARE_FLAG(
                wx,
                wy,
                wp,
                (
                    GET_MAP_SQUARE_FLAG(
                        wx,
                        wy,
                        wp
                    )
                    |
                    MSF_EROAD
                )
            );
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p27
/**
 * @brief Builds land and sea connectivity graphs for one world plane.
 *
 * @details
 * Maps EMM data access, then generates two connectivity graph tables for the
 * selected plane using the precomputed movement-cost maps:
 * - Walking connectivity is built from `movement_mode_cost_maps[wp].walking`
 *   into `connectivity_grid_land`.
 * - Sailing connectivity is built from `movement_mode_cost_maps[wp].sailing`
 *   into `connectivity_grid_sea`.
 *
 * The routine is a thin wrapper around `Build_Connectivity_Graph(...)` and
 * writes plane-local results into fixed 96-byte slices per plane.
 *
 * @param wp World plane index to process.
 *
 * @return void
 *
 * @note Expects movement-mode cost maps for the selected plane to be already
 *       initialized (for example by `Movement_Mode_Cost_Maps`).
 * @note Mutates global connectivity buffers `connectivity_grid_land` and `connectivity_grid_sea`.
 *
 * @see Movement_Mode_Cost_Maps
 * @see Build_Connectivity_Graph
 */
void Build_Connectivity_Graphs(int16_t wp)
{
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    EMMDATAH_Map();
    Build_Connectivity_Graph(&movement_mode_cost_maps[wp].walking.moves2[0], (uint8_t *)&connectivity_grid_land[(wp * 96)]);
    Build_Connectivity_Graph(&movement_mode_cost_maps[wp].sailing.moves2[0], (uint8_t *)&connectivity_grid_sea[(wp * 96)]);
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
}


// MGC o51p28
/**
 * @brief Generates terrain specials for one world plane during new game setup.
 *
 * @details
 * Clears all per-square terrain-special values and map-square flags on the selected
 * plane, then samples candidate squares on a coarse grid and assigns specials based
 * on resulting terrain type.
 *
 * Candidate placement behavior:
 * - Uses a plane-dependent stride/radius (`4` on Arcanus, `3` on Myrror).
 * - Picks a randomized square near each stride anchor.
 * - Skips squares already occupied by cities, lairs, or towers.
 * - Skips edge-restricted coordinates using the original conditional logic.
 * - For grassland (or forest at 50% chance), may convert terrain into
 *   mountain, hills, or swamp before assigning a special.
 *
 * Special assignment behavior after any terrain conversion:
 * - Forest -> `TS_WILDGAME`
 * - Mountain -> result of `Mountain_Terrain_Special(wp)`
 * - Hills -> result of `Hills_Terrain_Special(wp)`
 * - Swamp -> `ts_Nightshade`
 * - Desert -> result of `Desert_Terrain_Special(wp)`
 *
 * @param wp World plane index to process.
 *
 * @return void
 *
 * @note Uses random rolls and is non-deterministic unless RNG state is fixed by caller.
 * @warning Mutates terrain types, terrain specials, and map-square flags in place.
 * @see Mountain_Terrain_Special
 * @see Hills_Terrain_Special
 * @see Desert_Terrain_Special
 */
void Generate_Terrain_Specials(int16_t wp)
{
    int16_t radius = 0;
    int16_t square_has_city = 0;
    int16_t square_has_site = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    int16_t itr = 0;
    int16_t wy = 0;
    int16_t wx = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);


    /* Clear specials and flags map squares for the selected plane */
    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            SET_TERRAIN_SPECIAL(wx, wy, wp, 0);
            // AVWL  SET_MAP_SQUARE_FLAG(wx, wy, wp, 0);
            _map_square_flags[
                (
                    (wp * WORLD_SIZE)
                    +
                    (wy * WORLD_WIDTH)
                    +
                    wx
                )
            ] = 0;

        }
    }

    /* Set grid scan radius depending on the plane (Arcanus vs Myrror) */
    if(wp == ARCANUS_PLANE)
    {
        radius = 4;
    }
    else
    {
        radius = 3;
    }

    /* Grid-based scanning to place terrain specials */
    for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy += radius)     /* {0,...,36}; {0,...,39} */
    {
        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx += radius)  /* {0,...,56}; {0,...,57} */
        {
            /* Add random jitter inside the grid cell */
            wy = (itr_wy + Random((radius * 2)));  /* OGBUG  could be 39 + random(8) = 46, which is out of bounds */
            wx = (itr_wx + Random((radius * 2)));  /* OGBUG  could be 59 + random(8) = 67, which is out of bounds */
            /* OGBUG  OOB AVRL  e.g., wx=63,wy=45,p=1,offset = 5163 ACCESS VIOLATION READING LOCATION!!!!! */

            /* Sanity check location */
            square_has_city = ST_FALSE;
            square_has_site = ST_FALSE;
            for(itr = 0; itr < NUM_LAIRS; itr++)
            {
                if(
                    (_LAIRS[itr].wp == wp)
                    &&
                    (_LAIRS[itr].wx == wx)
                    &&
                    (_LAIRS[itr].wy == wy)
                )
                {
                    square_has_site = ST_TRUE;
                }
            }
            for(itr = 0; itr < NUM_TOWERS; itr++)
            {
                if(
                    (_TOWERS[itr].wx == wx)
                    &&
                    (_TOWERS[itr].wy == wy)
                )
                {
                    square_has_site = ST_TRUE;
                }
            }
            for(itr = 0; itr < _cities; itr++)
            {
                if(
                    (_CITIES[itr].wp == wp)
                    &&
                    (_CITIES[itr].wx == wx)
                    &&
                    (_CITIES[itr].wy == wy)
                )
                {
                    square_has_city = ST_TRUE;
                }
            }
            if(
                (GET_TERRAIN_SPECIAL(wx, wy, wp) != ts_NONE)
                ||
                (square_has_city != ST_FALSE)
                ||
                (square_has_site != ST_FALSE)
                ||
                (
                    (wy <= (WORLD_YMIN + 2))  /* OGBUG  should all be ||, not && */
                    &&
                    (wy >= (WORLD_HEIGHT - 2))
                    &&
                    (wx <= (WORLD_XMIN + 2))
                    &&
                    (wx >= (WORLD_WIDTH - 2))
                )
            )
            {
                continue;
            }
            /* Chance to transform Grassland/Forest into Mountains/Hills/Swamp */
            if(
                (Square_Is_Grassland_NewGame(wx, wy, wp) == ST_TRUE)
                ||
                (
                    (Square_Is_Forest_NewGame(wx, wy, wp) == ST_TRUE)
                    &&
                    (Random(2) == 1)  // 50%
                )
            )
            {
                switch((Random(7) - 1))
                {
                    case 0:
                    case 1:
                    case 2: { p_world_map[wp][wy][wx] = tt_Mountain1;    } break;
                    case 3:
                    case 4:
                    case 5: { p_world_map[wp][wy][wx] =  tt_Hills1;      } break;
                    case 6: { p_world_map[wp][wy][wx] =  tt_Swamp1;      } break;
                }
            }

            if(Square_Is_Forest_NewGame(  wx, wy, wp)  == ST_TRUE)  { SET_TERRAIN_SPECIAL(wx, wy, wp, TS_WILDGAME                     ); }
            if(Square_Is_Mountain_NewGame(wx, wy, wp)  == ST_TRUE)  { SET_TERRAIN_SPECIAL(wx, wy, wp, Mountain_Terrain_Special(wp)    ); }
            if(Square_Is_Hills_NewGame(   wx, wy, wp)  == ST_TRUE)  { SET_TERRAIN_SPECIAL(wx, wy, wp, Hills_Terrain_Special(wp)       ); }
            if(Square_Is_Swamp_NewGame(   wx, wy, wp)  == ST_TRUE)  { SET_TERRAIN_SPECIAL(wx, wy, wp, TS_NIGHTSHADE                   ); }
            if(Square_Is_Desert_NewGame(  wx, wy, wp)  == ST_TRUE)  { SET_TERRAIN_SPECIAL(wx, wy, wp, Desert_Terrain_Special(wp)      ); }
        }
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p29
// drake178: NEWG_DesertSpecials()
/*
; returns a random desert map square terrain special based on
; the plane
; Arcanus         Myrror
;   2:3    gems    1:5
;   1:3    quork   3:5
;   -      crysx   1:5
*/
/*

*/
int16_t Desert_Terrain_Special(int16_t wp)
{
    int16_t _DI_ = 0;  // _DI_
    int16_t terrain_special = 0;  // _SI_

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    if(wp == ARCANUS_PLANE)
    {
        _DI_ = Random(9);
    }
    else
    {
        _DI_ = (5 + Random(5));
    }

    switch((_DI_ - 1))  // - 1;  switch 10 cases
    {
        case  0:
        case  1:
        case  2:
        case  3:
        case  4:
        case  5: { terrain_special = ts_Gems; } break;   // min Myrror roll
        case  6: 
        case  7: 
        case  8: { terrain_special = ts_QuorkCrystals; } break;  // max Arcanus roll 
        case  9: { terrain_special = ts_CrysxCrystals; } break;

    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    return terrain_special;

}


// MGC o51p30
// drake178: NEWG_HillSpecials()
/*
; returns a random hill map square terrain special based on
; the plane
; Arcanus          Myrror
;   6:18    iron    1:10
;   4:18   silver   1:10
;   4:18    gold    4:10
;   3:18    coal    1:10
;   1:18   mithril  2:10
;   -    adamantium 1:10
*/
/*
pg 182  (PDF Page 191)
Hills
-----------------------
Iron        33.3    10
Coal        16.7    10
Silver      22.2    10
Gold        22.2    40
Mithril      5.6    20
Adamantium   0      10

Arcanus:
{0, ..., 17}
Iron    = (5 + 1):18 = 6:18 6/18 = 33.3
Silver  = (3 + 1):18 = 4:18 4/18 = 22.2
Coal    = (2 + 1):18 = 3:18 3/18 = 16.7
Gold    =              4:18 4/18 = 22.2
Mithril =              1:18 1/18 =  5.6
Adamantium  N/A

Myrror:
{10, ..., 19}
Iron       = 1:10  10
Silver     = 1:10  10
Coal       = 1:10  10
Gold       = 4:10  40
Mithril    = 2:10  20
Adamantium = 1:10  10

*/
int16_t Hills_Terrain_Special(int16_t wp)
{
    int16_t _DI_ = 0;  // _DI_
    int16_t terrain_special = 0;  // _SI_

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    if(wp == ARCANUS_PLANE)
    {
        _DI_ = Random(18);
    }
    else
    {
        _DI_ = (10 + Random(10));
    }

    switch((_DI_ - 1))  // - 1;  switch 20 cases
    {
        case  0:
        case  1:
        case  2:
        case  3:
        case  4: { terrain_special = ts_Iron; } break;
        case  5:
        case  6:
        case  7: { terrain_special = ts_Silver; } break;
        case  8:
        case  9: { terrain_special = ts_Coal; } break;
        case 10: { terrain_special = ts_Iron; } break;  // min Myrror roll
        case 11: { terrain_special = ts_Coal; } break;
        case 12: { terrain_special = ts_Silver; } break;
        case 13: 
        case 14: 
        case 15: 
        case 16: { terrain_special = ts_Gold; } break;
        case 17:
        case 18: { terrain_special = ts_Mithril; } break;  // max Arcanus roll 
        case 19: { terrain_special = ts_Adamantium; } break;
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    return terrain_special;

}


// MGC o51p31
// drake178: NEWG_MntnSpecials()
/*
; returns a random mountain map square terrain special based
; on the plane
; Arcanus          Myrror
;   5:18    coal    1:10
;   4:18    iron    1:10
;   3:18   silver   1:10
;   3:18    gold    2:10
;   3:18   mithril  3:10
;   -    adamantium 2:10
*/
/*

*/
int16_t Mountain_Terrain_Special(int16_t wp)
{
    int16_t _DI_ = 0;  // _DI_
    int16_t terrain_special = 0;  // _SI_

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    if(wp == ARCANUS_PLANE)
    {
        _DI_ = Random(18);
    }
    else
    {
        _DI_ = (10 + Random(10));
    }

    switch((_DI_ - 1))  // - 1;  switch 20 cases
    {
        case  0:
        case  1:
        case  2: { terrain_special = ts_Iron; } break;
        case  3:
        case  4: { terrain_special = ts_Silver; } break;
        case  5: 
        case  6: 
        case  7: 
        case  8: { terrain_special = ts_Coal; } break;
        case  9: { terrain_special = ts_Gold; } break;
        case 10: { terrain_special = ts_Iron; } break;  // min Myrror roll
        case 11: { terrain_special = ts_Silver; } break;
        case 12: { terrain_special = ts_Coal; } break;
        case 13: 
        case 14: { terrain_special = ts_Gold; } break;
        case 15: 
        case 16: 
        case 17: { terrain_special = ts_Mithril; } break;  // max Arcanus roll
        case 18: 
        case 19: { terrain_special = ts_Adamantium; } break;
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    return terrain_special;

}


// MGC o51p32
/**
 * @brief Initializes fog-of-war exploration state for a new game.
 *
 * @details
 * Clears exploration data for every square on both world planes, then reveals
 * the initial scouting area around the human player's starting fortress.
 *
 * Reveal pattern behavior:
 * - Marks the 3x3 block centered on the human fortress as fully explored.
 * - Marks surrounding ring squares with partial corner-bit visibility to form
 *   the classic starting vision shape.
 * - Uses Set_Square_Explored_Bits(...) for all reveal writes so out-of-range
 *   coordinates are ignored safely.
 *
 * The function mutates global exploration state via SET_SQUARE_EXPLORED and
 * Set_Square_Explored_Bits, and reads fortress coordinates from
 * _FORTRESSES[HUMAN_PLAYER_IDX].
 *
 * @return void
 */
void Init_Square_Explored(void)
{
    int16_t wx_offset = 0;
    int16_t wp = 0;
    int16_t wy = 0;
    int16_t wx = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    /* set the whole map for both worlds to 'unexplored' */
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                SET_SQUARE_EXPLORED(wx,wy,wp,ST_FALSE);
            }
        }
    }

    /*
        Wow...
        Just, make explored around the human players starting city...
    */
    wp = _FORTRESSES[HUMAN_PLAYER_IDX].wp;
    wx = _FORTRESSES[HUMAN_PLAYER_IDX].wx;
    wy = _FORTRESSES[HUMAN_PLAYER_IDX].wy;

    // ; mark the 3 by 3 area centered around the capital as fully explored
    // ; WARNING: the corners will be overwritten below too
    for(wx_offset = -1; wx_offset <= 1; wx_offset++)
    {
        Set_Square_Explored_Bits(wp, (wx + wx_offset), wy, (SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight));
    }
    for(wx_offset = -1; wx_offset <= 1; wx_offset++)
    {
        Set_Square_Explored_Bits(wp, (wx + wx_offset), (wy - 1), (SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight));
    }
    for(wx_offset = -1; wx_offset <= 1; wx_offset++)
    {
        Set_Square_Explored_Bits(wp, (wx + wx_offset), (wy + 1), (SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight));
    }

    Set_Square_Explored_Bits(wp, (wx - 1), (wy - 2),  SCT_BottomRight);
    Set_Square_Explored_Bits(wp,  wx,      (wy - 2), (SCT_BottomLeft | SCT_BottomRight));
    Set_Square_Explored_Bits(wp, (wx + 1), (wy - 2),  SCT_BottomLeft);
    Set_Square_Explored_Bits(wp, (wx - 1), (wy + 2),  SCT_TopRight);
    Set_Square_Explored_Bits(wp,  wx,      (wy + 2), (SCT_TopLeft | SCT_TopRight));
    /* S2E */
    Set_Square_Explored_Bits(wp, (wx + 1), (wy + 2),  SCT_TopLeft);
    Set_Square_Explored_Bits(wp, (wx - 2), (wy - 1),  SCT_BottomRight);
    Set_Square_Explored_Bits(wp, (wx - 2),  wy,      (SCT_TopRight | SCT_BottomRight));
    Set_Square_Explored_Bits(wp, (wx - 2), (wy + 1),  SCT_TopRight);
    Set_Square_Explored_Bits(wp, (wx + 2), (wy - 1),  SCT_BottomLeft);
    Set_Square_Explored_Bits(wp, (wx + 2), (wy + 1),  SCT_TopLeft);
    Set_Square_Explored_Bits(wp, (wx + 2),  wy,      (SCT_BottomLeft | SCT_TopLeft));
    /* NW*/
    Set_Square_Explored_Bits(wp, (wx - 1), (wy - 1), (SCT_BottomLeft | SCT_TopRight | SCT_BottomRight));
    /* SW */
    Set_Square_Explored_Bits(wp, (wx - 1), (wy + 1), (SCT_TopLeft | SCT_TopRight | SCT_BottomRight));
    /* SE */
    Set_Square_Explored_Bits(wp, (wx + 1), (wy + 1), (SCT_BottomLeft | SCT_TopLeft | SCT_TopRight));
    /* NE */
    Set_Square_Explored_Bits(wp, (wx + 1), (wy - 1), (SCT_BottomLeft | SCT_TopLeft | SCT_BottomRight));

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p33
// drake178: TILE_SetScouting()
/*
; sets the pased scouting flags for the specified map square,
; or does nothing if the coordinates are out of range
; (the plane is not boundary checked though)
*/
/*

NOT Set_Map_Square_Explored_Flags_XYP_Range()
NOT j_Set_Map_Square_Explored_Flags_XYP()

*/
void Set_Square_Explored_Bits(int16_t wp, int16_t wx, int16_t wy, int16_t bits)
{

    if(
        (wx >= 0)
        &&
        (wx < WORLD_WIDTH)
        &&
        (wy >= 0)
        &&
        (wy < WORLD_HEIGHT)
    )
    {

        SET_SQUARE_EXPLORED(wx,wy,wp,bits);

    }

}


// MGC o51p34
/*
    populates movement_mode_cost_maps[]
    from terrain_stats_table[], then roads and eroads
SEEALSO:  CITYCALC.c  Make_Road()
*/
/**
 * @brief Builds per-tile movement-cost maps for a world plane.
 *
 * @details
 * Recomputes `movement_mode_cost_maps[wp]` from terrain data for every square
 * on the selected plane, then applies road overrides.
 *
 * Base fill behavior per square:
 * - Reads terrain id from `p_world_map[wp][y][x]`.
 * - Loads six half-move costs from `terrain_stats_table` into movement-mode
 *   maps: `UU_MvMd`, `walking`, `forester`, `mountaineer`, `swimming`, and
 *   `sailing`.
 *
 * Road override behavior:
 * - If `MSF_ROAD` is present on a square, land-capable movement modes are
 *   reduced to fast travel values.
 * - On Arcanus, roaded land-capable modes are set to `1`.
 * - On non-Arcanus planes, roaded land-capable modes are set to `0`
 *   (enchanted-road behavior).
 * - `sailing` is forced to `ST_UNDEFINED` on road squares.
 *
 * The routine maps EMM data first via `EMMDATAH_Map()` and mutates global
 * movement-cost tables in place.
 *
 * @param wp World plane index whose movement maps are rebuilt.
 *
 * @return void
 *
 * @note Expects terrain and map-flag state to be initialized before call.
 * @note Uses half-move units (`moves2`) in all output maps.
 */
void Movement_Mode_Cost_Maps(int16_t wp)
{
    int16_t terrain_type_idx = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    EMMDATAH_Map();
    for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
    {
        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
        {
            terrain_type_idx = p_world_map[wp][itr_wy][itr_wx];
            movement_mode_cost_maps[wp].walking.moves2[    ((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 1)];
            movement_mode_cost_maps[wp].UU_MvMd.moves2[    ((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 0)];
            movement_mode_cost_maps[wp].forester.moves2[   ((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 2)];
            movement_mode_cost_maps[wp].mountaineer.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 3)];
            movement_mode_cost_maps[wp].swimming.moves2[   ((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 4)];
            movement_mode_cost_maps[wp].sailing.moves2[    ((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 5)];
            /* Road logic adjustment */
            if((MAP_SQUARE_FLAG(itr_wx,itr_wy,wp) & MSF_ROAD) != 0)
            {
                if(wp != ARCANUS_PLANE)
                {
                    /* Myrror: Enchanted roads reduce cost to 0 (free movement increment) */
                    movement_mode_cost_maps[wp].walking.moves2[    ((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
                    movement_mode_cost_maps[wp].UU_MvMd.moves2[    ((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
                    movement_mode_cost_maps[wp].forester.moves2[   ((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
                    movement_mode_cost_maps[wp].mountaineer.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
                    movement_mode_cost_maps[wp].swimming.moves2[   ((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
                }
                else
                {
                    /* Arcanus: Standard roads reduce cost to 1 (half movement point) */
                    movement_mode_cost_maps[wp].walking.moves2[    ((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;
                    movement_mode_cost_maps[wp].UU_MvMd.moves2[    ((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;
                    movement_mode_cost_maps[wp].forester.moves2[   ((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;
                    movement_mode_cost_maps[wp].mountaineer.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;
                    movement_mode_cost_maps[wp].swimming.moves2[   ((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;
                }
                /* Roads are impassable for Sailing units */
                movement_mode_cost_maps[wp].sailing.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = ST_UNDEFINED;
            }
        }
    }
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
}


// MGC o51p35
/**
 * @brief Checks whether a square is an invalid river-mouth terrain for endpoint validation.
 *
 * @details
 * Reads the map-square terrain on the specified plane, normalizes it with
 * `TerType_Count` modulo, and returns `ST_TRUE` only when the resulting terrain
 * id falls within the blocked shore subtype range
 * `_Shore10111000.._Shore10101111`.
 *
 * This helper is used by river generation endpoint checks to reject river mouths
 * that terminate on incompatible shore topology.
 *
 * @param wx World X coordinate to test.
 * @param wy World Y coordinate to test.
 * @param wp World plane index containing the square.
 *
 * @return int16_t
 * @retval ST_TRUE  The square terrain is in the disallowed river-mouth range.
 * @retval ST_FALSE The square terrain is acceptable for river-mouth validation.
 *
 * @note Accesses `p_world_map` directly and assumes caller-provided coordinates
 *       are valid for the selected plane.
 */
int16_t Square_Is_Bad_River_Mouth(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    terrain_type = (TERRAIN_TYPE(wx, wy, wp) % NUM_TERRAIN_TYPES);
    if((terrain_type >= _Shore10111000) && (terrain_type <= _Shore10101111)) { return ST_TRUE; }
    return ST_FALSE;
}


// MGC o51p36
// drake178: NEWG_UpdateProgress()
// ~ MoO2  Module: MISC  Draw_Advancing_Fill_Message_()
/*
PATCHED in the worldgen customizer to use a created
background image and always redraw, allowing the
progress bar to be reset if necessary
updates the progress bar to the specified percentage
before doing a simple page flip
WARNING: can't go backwards with this setup
*/
/*

*/
/**
 * @brief Updates and draws the world-generation progress bar.
 *
 * @details
 * Renders progress into the "Building The Worlds" bar region on the offscreen
 * page and then flips pages to present the updated frame. Progress is mapped
 * to a fixed-width bar where `100` fills the full range and intermediate
 * values fill proportionally.
 *
 * For `percent == 0`, the function returns early after setting the off page,
 * performing no fill or page toggle.
 *
 * @param percent Progress percentage expected in the range `0..100`.
 *
 * @return void
 *
 * @note Values greater than or equal to `100` are clamped to full-bar fill.
 * @warning This routine is designed for forward-progress updates; visual
 *          behavior for regressions depends on caller sequencing.
 */
void Draw_Building_The_Worlds(int16_t percent)
{
    int16_t width = 0;
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    Set_Page_Off();
    if(percent == 0)
    {
        return;
    }
    if(percent >= 100)
    {
        Fill(90, 89, 225, 94, 172);
    }
    else
    {
        width = ((percent * 136) / 100);
        Fill(90, 89, (90 + width), 94, 172);
    }
    Toggle_Pages();
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
}

// MGC o51p37
// drake178: UNIT_Create()
/*
*/
/*

*/
void Create_Unit_NewGame(int16_t unit_type, int16_t player_idx, int16_t wx, int16_t wy, int16_t wp, int16_t city_idx)
{
    int16_t WTF = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    if(unit_type == 0)
    {
        LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
        return;
    }

    _UNITS[_units].wx = (int8_t)wx;
    _UNITS[_units].wy = (int8_t)wy;
    _UNITS[_units].wp = (int8_t)wp;
    _UNITS[_units].owner_idx = (int8_t)player_idx;
    _UNITS[_units].moves2_max = _unit_type_table[unit_type].Move_Halves;
    _UNITS[_units].type = (uint8_t)unit_type;
    _UNITS[_units].Hero_Slot = ST_UNDEFINED;
    // DNE  _UNITS[_units].in_tower = ST_FALSE;
    // _UNITS[_units].Finished = ST_TRUE;
    _UNITS[_units].Finished = ST_FALSE;
    // _UNITS[_units].moves2 = 0;
    _UNITS[_units].moves2 = _unit_type_table[unit_type].Move_Halves;
    _UNITS[_units].Sight_Range = _unit_type_table[unit_type].Sight;
    _UNITS[_units].dst_wx = 0;
    _UNITS[_units].dst_wy = 0;
    _UNITS[_units].Status = us_Ready;
    _UNITS[_units].Level = 0;
    _UNITS[_units].XP = 0;
    _UNITS[_units].Damage = 0;
    _UNITS[_units].Draw_Priority = 0;
    _UNITS[_units].enchantments = 0;
    // DNE  _UNITS[_units].mutations = 0;
    _UNITS[_units].Move_Failed = ST_FALSE;
    _UNITS[_units].Rd_Constr_Left = ST_UNDEFINED;
    _UNITS[_units].mutations = 0;

    if(_players[player_idx].alchemy != ST_FALSE)
    {
        _UNITS[_units].mutations = wq_Magic;                        
    }

    // WTF?
    if(city_idx >= 0)
    {

        // ; ; PATCHED here to prevent units starting with XP

        if(_CITIES[WTF].bldg_status[bt_FightersGuild] > bs_Replaced)
        {

            _UNITS[_units].XP = TBL_Experience[UL_REGULAR];

            _UNITS[_units].Level = 1;

        }

        if(_CITIES[WTF].bldg_status[bt_WarCollege] > bs_Replaced)
        {

            _UNITS[_units].XP = TBL_Experience[UL_VETERAN];

            _UNITS[_units].Level = 2;

        }

        if(_UNITS[_units].Level > 3)
        {

            _UNITS[_units].XP = TBL_Experience[UL_ELITE];

            _UNITS[_units].Level = 3;

        }

        if(_players[player_idx].warlord > ST_FALSE)
        {

            _UNITS[_units].Level += 1;

        }

    }

    _units++;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o51p38
/**
 * @brief Applies randomized ocean animation square swaps on both world planes.
 *
 * @details
 * Iterates every map square on `ARCANUS_PLANE` and `MYRROR_PLANE` and performs
 * a probabilistic conversion for ocean-type squares:
 * - If a square equals `tt_Ocean`, then with a 1-in-5 chance (`Random(5) == 1`)
 *   it is replaced with `tt_OceanAnim`.
 * - If a square equals raw value `762`, then with a 1-in-5 chance it is
 *   replaced with raw value `1363`.
 *
 * The second branch is a legacy/special-case path that uses hardcoded terrain
 * indices and is retained as implemented.
 *
 * Key local variables:
 * - `wp`: world plane iterator (`0 .. NUM_PLANES-1`).
 * - `wy`: world Y coordinate iterator (`0 .. WORLD_HEIGHT-1`).
 * - `wx`: world X coordinate iterator (`0 .. WORLD_WIDTH-1`).
 *
 * @param void This function takes no parameters.
 *
 * @return void
 * No value is returned. The routine mutates `p_world_map` in place.
 *
 * @note This routine depends on global world-map storage and the global RNG
 *       stream (`Random()`), so exact animated-square selection varies per run.
 */
void Animate_Oceans(void)
{
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t wp = 0;
    
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);


    /* OGBUG  the wy/wx loops use `<=` so the final iteration reads (and possibly
     *   writes) one row/column past the declared world map bounds.  The extra
     *   reads land in the WORLD_OVERFLOW padding at the tail of the _world_maps
     *   allocation (7 * WORLD_WIDTH int16_t cells) — always allocated, but past
     *   the [WORLD_HEIGHT][WORLD_WIDTH] bound the typed p_world_map advertises.
     *   Access via a flat int16_t* keeps GCC's -Waggressive-loop-optimizations
     *   quiet while preserving OG's OOB read/write behavior (RNG-relevant —
     *   the OOB cells drive Random(5) calls that must match OG's stream).
     *   OG (preserved):    ((int16_t*)_world_maps)[wp*WORLD_SIZE + wy*WORLD_WIDTH + wx]
     *   Correct loops:     for(wy = 0; wy < WORLD_HEIGHT; wy++) / wx < WORLD_WIDTH  */
    int16_t * flat_map = (int16_t *)_world_maps;
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy <= WORLD_HEIGHT; wy++)  /* OGBUG  OOB AVRL; should be <, not <=; overruns by 1 */
        {
            for(wx = 0; wx <= WORLD_WIDTH; wx++)  /* OGBUG  OOB AVRL; should be <, not <=; overruns by 1 */
            {
                int32_t map_idx = ((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx);
                /* Check for Ocean on Arcanus plane */
                if(flat_map[map_idx] == tte_Ocean)
                {
                    if(Random(5) == 1)  /* 1:5  20% */
                    {
                        flat_map[map_idx] = tte_OceanAnim;
                    }
                }
                /* OGBUG conflicting condition - will always jump (myrran square types are only valid in graphics) - coded as if Myrror has its own terrain type indices */
                if(flat_map[map_idx] == (TerType_Count + tte_Ocean))
                {
                    if(Random(5) == 1)  /* 1:5  20% */
                    {
                        flat_map[map_idx] = (TerType_Count + tte_OceanAnim);
                    }
                }
            }
        }
    }
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
}


// MGC o51p39
int16_t Square_Is_Ocean_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    terrain_type = (TERRAIN_TYPE(wx, wy, wp) % NUM_TERRAIN_TYPES);
    if(terrain_type  == tt_BugGrass)   { return ST_FALSE; }
    if(terrain_type  < tt_Grasslands1) { return ST_TRUE;  }
    if((terrain_type > tt_RiverM_end) && (terrain_type < tt_Rivers_1st)) { return ST_TRUE; }
    if((terrain_type > tt_Desert_Lst) && (terrain_type < tt_4WRiver1))   { return ST_TRUE; }
    if((terrain_type > tt_4WRiver5)   && (terrain_type < tt_Tundra_1st)) { return ST_TRUE; }
    return ST_FALSE;
}


// MGC o51p40
int16_t Square_Is_Node_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t itr_nodes = 0;
    for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
    {
        if(
            (_NODES[itr_nodes].wx == wx)
            &&
            (_NODES[itr_nodes].wy == wy)
            &&
            (_NODES[itr_nodes].wp == wp)
        )
        {
            return ST_TRUE;
        }
    }
    return ST_FALSE;
}


// MGC o51p41
int16_t Square_Has_Tower_NewGame(int16_t wx, int16_t wy)
{
    int16_t itr_towers = 0;
    for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
    {
        if(
            (_TOWERS[itr_towers].wx == wx)
            &&
            (_TOWERS[itr_towers].wy == wy)
        )
        {
            return ST_TRUE;
        }
    }
    return ST_FALSE;
}


// MGC o51p42
// drake178: Square_Is_Forest_NewGame()
/*

*/
/*

~==
Terrain.c
// WZD s161p23
// drake178: TILE_IsVisibleForest()
// AKA Square_Is_Explored_Forest()
int16_t Square_Is_Forest(int16_t wx, int16_t wy, int16_t wp)

*/
int16_t Square_Is_Forest_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_

    terrain_type = (TERRAIN_TYPE(wx, wy, wp) % NUM_TERRAIN_TYPES);

    if(
        (terrain_type == tt_Forest1)
        ||
        (terrain_type == tt_Forest2)
        ||
        (terrain_type == tt_Forest3)
    )
    {

        return ST_TRUE;

    }
    else
    {

        return ST_FALSE;

    }

}


// MGC o51p43
int16_t Square_Is_River_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_river = 0;  // DNE in Dasm
    /* OGBUG  `% TerType_Count` converts 1000 (0x3E8) tt_River_Placeholder into $EE (fortunately actually a river) (1000 mod 762 = 238  0xEE) */
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if((terrain_type > tt_Forest3     ) && (terrain_type < tt_Lake2         )) { is_river = ST_TRUE; }  /* OGBUG  this should be tt_Lake1 ($C5), not tt_Lake2 */
    if((terrain_type > tt_Shore2F_end ) && (terrain_type < tt_Mountains_Fst )) { is_river = ST_TRUE; }
    if((terrain_type > tt_Shore2_end  ) && (terrain_type < tt_Shore3_1st    )) { is_river = ST_TRUE; }
    if(terrain_type == TT_RIVER_PLACEHOLDER) { is_river = ST_TRUE; }
    return is_river;
}


// MGC o51p44
/* 
Checks if the normalized terrain type matches specific shore or river ranges.
Returns 1 (TRUE) if in ranges:
- (200, 233)    - Lakes / Rivers
- (451, 468)    - Desert Rivers
- (472, 601)    - Anim Ocean / Other Rivers
...a "river outlet" is a shore-classified tile that is a river tile rather than coastline.
*/
int16_t Square_Is_River_Mouth_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_river_mouth = 0;  // DNE in Dasm
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if(terrain_type > _1LakeRiv_S && terrain_type < _River1100_3)     { is_river_mouth = ST_TRUE; }
    if(terrain_type > _Desert10101111 && terrain_type < _River1111_1) { is_river_mouth = ST_TRUE; }
    if(terrain_type > _River1111_5 && terrain_type < tte_OceanAnim)   { is_river_mouth = ST_TRUE; }
    return is_river_mouth;
}


// MGC o51p45
/* 
Checks if the normalized terrain type matches specific shore or river ranges.
Returns 1 (TRUE) if in ranges:
- (1, 162)      - Land/Shore tiles before Grasslands
- (200, 233)    - Lakes / Rivers
- (451, 468)    - Desert Rivers
- (472, 601)    - Anim Ocean / Other Rivers
...vs. Square_Is_Shoreline()?
*/
int16_t Square_Is_Shore_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_shore = 0;  // DNE in Dasm
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if(terrain_type > _Land && terrain_type < tte_Grasslands)         { is_shore = ST_TRUE; }
    if(terrain_type > _1LakeRiv_S && terrain_type < _River1100_3)     { is_shore = ST_TRUE; }
    if(terrain_type > _Desert10101111 && terrain_type < _River1111_1) { is_shore = ST_TRUE; }
    if(terrain_type > _River1111_5 && terrain_type < tte_OceanAnim)   { is_shore = ST_TRUE; }
    return is_shore;
}


// MGC o51p46
int16_t Square_Is_Mountain_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    terrain_type = (TERRAIN_TYPE(wx, wy, wp) % NUM_TERRAIN_TYPES);
    if(
        (terrain_type == tt_Mountain1)
        ||
        (
            (terrain_type >= tt_Mountains_Fst)
            &&
            (terrain_type <= tt_Mountains_Lst)
        )
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }
}

// MGC o51p47
int16_t Square_Is_Hills_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    terrain_type = (TERRAIN_TYPE(wx, wy, wp) % NUM_TERRAIN_TYPES);
    if(
        (terrain_type == tt_Hills1)
        ||
        (
            (terrain_type >= tt_Hills_Fst)
            &&
            (terrain_type <= tt_Hills_Lst)
        )
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }
}


// MGC o51p48
int16_t Square_Is_Swamp_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    terrain_type = (TERRAIN_TYPE(wx, wy, wp) % NUM_TERRAIN_TYPES);
    if(
        (terrain_type == tt_Swamp2)
        ||
        (terrain_type == tt_Swamp3)
        ||
        (terrain_type == tt_Swamp1)
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }
}

// MGC o51p49
int16_t Square_Is_Desert_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    terrain_type = (TERRAIN_TYPE(wx, wy, wp) % NUM_TERRAIN_TYPES);
    if(
        (terrain_type == tt_Desert1)
        ||
        (terrain_type == tt_Desert2)
        ||
        (terrain_type == tt_Desert3)
        ||
        (terrain_type == tt_Desert4)
        ||
        (
            (terrain_type >= tt_Desert_Fst)
            &&
            (terrain_type <= tt_Desert_Lst)
        )
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }
}


// MGC o51p50
int16_t Square_Is_Grassland_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    terrain_type = (TERRAIN_TYPE(wx, wy, wp) % NUM_TERRAIN_TYPES);
    if(
        (terrain_type == tt_Grasslands1)
        ||
        (terrain_type == tt_Grasslands2)
        ||
        (terrain_type == tt_Grasslands3)
        ||
        (terrain_type == tt_Grasslands4)
        ||
        (terrain_type == tt_BugGrass)
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }
}


// MGC o51p51
// drake178: TILE_GetFood()
/*
; returns the amount of food provided by the map square based
; on its type, in units of 1/2
;
; BUG: swamps do not yield the described amount of 1/2,
;  while nature nodes yield less than in WIZARDS.EXE
*/
/*
returns food, not food2

Terrain.c
// WZD s161p03
int16_t Square_Food_x2(int16_t wx, int16_t wy, int16_t wp)

traced with terrain_type == tt_Grasslands1

*/
int16_t Square_Food2_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _SI_
    int16_t food_units = 0;  // _CX_

    terrain_type = p_world_map[wp][wy][wx];

    if(
        (terrain_type == tt_Ocean)
        ||
        (terrain_type >= tt_OceanAnim)
    )
    {
        return 0;
    }
    else if(terrain_type == tt_BugGrass)  // NOTE: sames {tt_Grasslands1, ..., tt_Grasslands3}
    {
        return 3;
    }
    else if(terrain_type < tt_Grasslands1)  // >= tt_Shore1_Fst  <= tt_Shore1_Lst
    {
        return 1;
    }
    else if(terrain_type > tt_4WRiver5)  // >= tt_Shore3_1st
    {
        return 1;
    }
    else if(terrain_type > tt_Shore2_end)  // >= tt_4WRiver1
    {
        return 4;
    }
    else if(terrain_type > tt_Desert_Lst)  // >= tt_Shore2_1st
    {
        return 1;
    }
    else if(terrain_type > tt_Hills_Lst)  // >= tt_Desert_Fst
    {
        return 0;
    }
    else if(terrain_type > tt_Mountains_Lst)  // >= tt_Hills_Fst
    {
        return 1;
    }
    else if(terrain_type > tt_Rivers_end)  // >= tt_Mountains_Fst
    {
        return 0;
    }
    else if(terrain_type > tt_Forest3)  // >= tt_RiverM_1st
    {
        return 4;
    }
    else
    {

// switch(terrain_type - tt_Grasslands1)
// ; switch 23 cases
// jt_sq_ng_tt_fd_00_10_11_18
// jt_sq_ng_tt_fd_01_21_22
// jt_sq_ng_tt_fd_02
// jt_sq_ng_tt_fd_03_12_13_14
// jt_sq_ng_tt_fd_04_15_16
// jt_sq_ng_tt_fd_05_19_20
// jt_sq_ng_tt_fd_06
// jt_sq_ng_tt_fd_07
// jt_sq_ng_tt_fd_08
// jt_sq_ng_tt_fd_09
// jt_sq_ng_tt_fd_17
        switch(terrain_type)
        {

            case tt_Grasslands1:
            case tt_Grasslands2:
            case tt_Grasslands3:
            case tt_Grasslands4:
            {
                food_units = 3;
            } break;

            case tt_Forest1:
            case tt_Forest2:
            case tt_Forest3:
            {
                food_units = 1;
            } break;

            case tt_Mountain1:
            {
                food_units = 0;
            } break;

            case tt_Desert1:
            case tt_Desert2:
            case tt_Desert3:
            case tt_Desert4:
            {
                food_units = 0;
            } break;

            case tt_Swamp1:  // BUGBUG: manual says 1/2 food
            case tt_Swamp2:  // BUGBUG: manual says 1/2 food
            case tt_Swamp3:  // BUGBUG: manual says 1/2 food
            {
                food_units = 0;
            } break;

            case tt_Tundra1:
            case tt_Tundra2:
            case tt_Tundra3:
            {
                food_units = 0;
            } break;

            case tt_SorceryNode:
            {
                food_units = 4;
            } break;

            case tt_NatureNode:
            {
                food_units = 4;  // BUGBUG  is 5 in Terrain.c // WZD s161p03  int16_t Square_Food_x2(int16_t wx, int16_t wy, int16_t wp)
            } break;

            case tt_ChaosNode:
            {
                food_units = 0;
            } break;

            case tt_Hills1:
            {
                food_units = 1;
            } break;

            case tt_Volcano:
            {
                food_units = 0;
            } break;

        }

    }

    return food_units;

}


// MGC o51p52
/*
vs. CITYCALC  City_Maximum_Size()?
*/
int16_t City_Maximum_Size_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t city_area_squares = 0;
    int16_t city_area_food_units = 0;
    int16_t itr = 0;
    int16_t maximum_size = 0;
    city_area_squares = Get_Useable_City_Area_NewGame(wx, wy, wp, &wx_array[0], &wy_array[0]);
    city_area_food_units = 0;
    for(itr = 0; itr < city_area_squares; itr++)
    {
        city_area_food_units += Square_Food2_NewGame(wx_array[itr], wy_array[itr], wp);
    }
    /* N/A - Granary, Farmer's Market, Forester's Guild */
    /* OGBUG  missing City_Food_WildGame() (but no TS's yet) */
    maximum_size = (city_area_food_units / 2);
    return maximum_size;  /* 2 food units ("food2") === 1 population unit */
}


// MGC o51p53
/*
~==  WZD o142p08  CITYCALC.c  int16_t Get_Useable_City_Area(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t *wx_array, int16_t *wy_array)
*/
/**
 * @brief Builds the list of usable map squares in a city's catchment area.
 *
 * @details
 * Evaluates the standard 5x5 city work area centered on (`city_wx`, `city_wy`) with
 * trimmed corners (rows at y-offset -2 and +2 include x-offsets -1..+1 only), yielding
 * up to 21 valid squares. Rows outside vertical map bounds are skipped.
 *
 * For each accepted square, world X is wrapped horizontally to keep coordinates inside
 * map bounds, and the resulting coordinates are written to the output arrays at the same
 * index. The function returns the number of entries written.
 *
 * @param city_wx City world X coordinate.
 * @param city_wy City world Y coordinate.
 * @param city_wp City world plane index.
 * @param wx_array Output array receiving usable square X coordinates.
 * @param wy_array Output array receiving usable square Y coordinates.
 *
 * @return Number of usable city-area squares stored in the output arrays.
 *
 * @note The current implementation does not filter out corrupted tiles; related checks
 *       are present as disabled legacy (`DNE`) comments.
 * @warning Caller must provide arrays large enough to hold all emitted squares
 *          (typically `CITY_AREA_SIZE`).
 */
int16_t Get_Useable_City_Area_NewGame(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t * wx_array, int16_t * wy_array)
{
    // DNE  uint8_t * terrain_flags_table_row = 0;
    int16_t square_y = 0;
    int16_t square_x_max = 0;
    int16_t square_x_min = 0;
    int16_t map_square_count = 0;  // _SI_
    int16_t itr_city_area_squares = 0;  // _CX_
    int16_t itr_world_x = 0;  // _DI_
    int16_t square_x = 0;  // _DX_

    map_square_count = 0;

    for(itr_city_area_squares = -2; itr_city_area_squares <= 2; itr_city_area_squares++)
    {

        square_y = (city_wy + itr_city_area_squares);

        if(
            (square_y >= 0)
            &&
            (square_y < WORLD_HEIGHT) )
        {

            square_x_min = -2;
            square_x_max =  2;

            if(
                (itr_city_area_squares == -2)
                ||
                (itr_city_area_squares ==  2)
            )
            {

                square_x_min = -1;
                square_x_max =  1;

            }

            // DNE  terrain_flags_table_row = (uint8_t *)&_map_square_flags[(city_wp * WORLD_SIZE) + (square_y * WORLD_WIDTH)];

            itr_world_x = square_x_min;

            while(itr_world_x <= square_x_max)
            {

                square_x = city_wx + itr_world_x;

                if(square_x < 0)
                {
                    square_x += WORLD_WIDTH;
                }

                if(square_x > WORLD_WIDTH)  // BUGBUG should be >= ?
                {
                    square_x -= WORLD_WIDTH;
                }

                // DNE  if( (*(terrain_flags_table_row + square_x) & MSF_CORRUPTION) == 0)
                // DNE  {
                    wx_array[map_square_count] = square_x;
                    wy_array[map_square_count] = square_y;
                    map_square_count++;
                // DNE  }

                itr_world_x++;

            }

        }

    }

    return map_square_count;

}


// MGC o51p54
// drake178: UU_Empty_Tile_Fn()
void o51p54_empty_function(void)
{
// push    bp
// mov     bp, sp
// pop     bp
// retf
}



/*

    MAGIC.EXE  ovr054

*/

// MGC o54p01
// drake178: Find_Shortest_Path()
/*
; the arrays created by this function are never used
; by the game, and can be repurposed even though they
; are part of the save game file
;
; a brute force shortest path algorithm that fills the
; passed arrays with the result, returning the length
; of the path in tiles, or 0 if none found
;
; WARNING: uses global arrays limited to a maximum map
; size of 225 tiles and path length of 100, with no
; wrapping possible on either axis
*/
/*
    Structure map — see doc/PathFinding/MoM-MovePath-Compare.md

    All three MoM shortest-path solvers share a 5-step skeleton (that doc, "The shared skeleton").
    This variant (NIU, fully parameterized) maps to it as follows; the inline [Prelude] / [Skeleton step N]
    labels below mark each section:

        [Prelude]        rebuild 9 edge/corner adjacency-offset tables from Wdt (recomputed every call)
        [Skeleton 2]     bail if target / source tile is impassable
        [Skeleton 1]     init parallel arrays: predecessor (shortest_path_came_from) = self, cost (Costs) = INF, source = 0
        [Skeleton 3]     relaxation sweep to fixed point — single fixed-direction raster (while a_cost_was_updated)
        [Skeleton 4]     back-trace target -> predecessor self-link; success iff self-link == source
        [Skeleton 5]     reverse collected indices and convert 1-D index -> (x, y)
*/
int16_t Find_Shortest_Path(int16_t src_x, int16_t src_y, int16_t dst_x, int16_t dst_y, int16_t Wdt, int16_t Hgt, uint8_t * movepath_cost_map, uint8_t * mvpth_x, uint8_t * mvpth_y)
{
    int16_t Bottom_Row_Adjacents[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Top_Row_Adjacents[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Bottom_Right_Adjacents[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Bottom_Left_Adjacents[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Top_Right_Adjacents[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Top_Left_Adjacents[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Right_Col_Adjacents[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Left_Col_Adjacents[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Mid_Tile_Adjacents[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t path_length = 0;            /* path length (hop count) */
    uint8_t new_cost_to_reach = 0;      /* candidate new cost */
    int16_t grid_cell_count = 0;        /* total tile count */
    int16_t move_cost = 0;              /* current tile entry cost */
    int16_t Inner_Col_Height = 0;
    int16_t Adjacent_Tile_Cost = 0;
    int16_t Inner_Row_Length = 0;
    int16_t Row_LoopVar = 0;            /* row / column iterator */
    int16_t Col_LoopVar = 0;            /* row / column iterator */
    int16_t Current_Source = 0;         /* predecessor snapshot (Δ-detect) */
    int16_t Adjacent_Tile_Offset = 0;   /* neighbour offset / index */
    int16_t a_cost_was_updated = 0;     /* relaxation "changed" flag */  /* Early Exit: Bellman-Ford can terminate early if a full pass occurs without changing any distances. This saves significant CPU cycles. */
    int16_t itr = 0;
    int16_t cx = 0;
    int16_t ctr = 0;  /* current tile 1-D index */

    /* [Prelude]  Rebuild the 9 edge/corner adjacency-offset tables from Wdt — recomputed every call (MoM-MovePath-Compare.md, "Find_Shortest_Path"). */
    /* Initializing Adjacency lists offsets based on Wdt */
    Mid_Tile_Adjacents[0] = -Wdt - 1;
    Mid_Tile_Adjacents[1] = -Wdt;
    Mid_Tile_Adjacents[2] = -Wdt + 1;
    Mid_Tile_Adjacents[3] = -1;
    Mid_Tile_Adjacents[4] = 1;
    Mid_Tile_Adjacents[5] = Wdt - 1;
    Mid_Tile_Adjacents[6] = Wdt;
    Mid_Tile_Adjacents[7] = Wdt + 1;

    Left_Col_Adjacents[0] = -Wdt;
    Left_Col_Adjacents[1] = -Wdt + 1;
    Left_Col_Adjacents[2] = 1;
    Left_Col_Adjacents[3] = Wdt;
    Left_Col_Adjacents[4] = Wdt + 1;

    Right_Col_Adjacents[0] = -Wdt - 1;
    Right_Col_Adjacents[1] = -Wdt;
    Right_Col_Adjacents[2] = -1;
    Right_Col_Adjacents[3] = Wdt;
    Right_Col_Adjacents[4] = Wdt - 1;

    Top_Left_Adjacents[0] = 1;
    Top_Left_Adjacents[1] = Wdt;
    Top_Left_Adjacents[2] = Wdt + 1;

    Top_Right_Adjacents[0] = -1;
    Top_Right_Adjacents[1] = Wdt;
    Top_Right_Adjacents[2] = Wdt - 1;

    Bottom_Left_Adjacents[0] = 1;
    Bottom_Left_Adjacents[1] = -Wdt;
    Bottom_Left_Adjacents[2] = -Wdt + 1;

    Bottom_Right_Adjacents[0] = -1;
    Bottom_Right_Adjacents[1] = -Wdt;
    Bottom_Right_Adjacents[2] = -Wdt - 1;

    Top_Row_Adjacents[0] = -1;
    Top_Row_Adjacents[1] = 1;
    Top_Row_Adjacents[2] = Wdt;
    Top_Row_Adjacents[3] = Wdt - 1;
    Top_Row_Adjacents[4] = Wdt + 1;

    Bottom_Row_Adjacents[0] = -1;
    Bottom_Row_Adjacents[1] = 1;
    Bottom_Row_Adjacents[2] = -Wdt;
    Bottom_Row_Adjacents[3] = -Wdt - 1;
    Bottom_Row_Adjacents[4] = -Wdt + 1;

    path_length = 0;

    /* [Skeleton step 2]  Bail if the target or source tile is impassable (MoM-MovePath-Compare.md, "The shared skeleton"). */
    /* Verify target tile is passable */
    if(movepath_cost_map[dst_y * Wdt + dst_x] == INF)
    {
        return 0;
    }

    /* Verify source tile is passable */
    if(movepath_cost_map[src_y * Wdt + src_x] == INF)
    {
        return 0;
    }

    /* [Skeleton step 1]  Two parallel arrays: predecessor shortest_path_came_from[] init to self; cost shortest_path_cost_to_reach[] init INF, source = 0 (MoM-MovePath-Compare.md, "The shared skeleton"). */
    grid_cell_count = Wdt * Hgt;

    for(itr = 0; itr < grid_cell_count; itr++)
    {
        shortest_path_came_from[itr] = itr;
    }

    for(itr = 0; itr < grid_cell_count; itr++)
    {
        shortest_path_cost_to_reach[itr] = INF;
    }

    shortest_path_cost_to_reach[src_y * Wdt + src_x] = 0;

    /* [Skeleton step 3]  Relaxation sweep to fixed point — single fixed-direction raster (MoM-MovePath-Compare.md, "The shared skeleton" / "Find_Shortest_Path").
       Per pass the raster walks: top-left corner, top row, top-right corner, then each middle row (left column, inner tiles, right column), bottom-left corner, bottom row, bottom-right corner. */
    a_cost_was_updated = ST_TRUE;
    while(a_cost_was_updated == ST_TRUE)
    {
        a_cost_was_updated = 0;
        Inner_Row_Length = Wdt - 2;
        Inner_Col_Height = Hgt - 2;
        ctr = 0;

        /* Top-Left Corner (ctr = 0) */
        move_cost = movepath_cost_map[ctr];
        if(move_cost != INF)
        {
            Current_Source = shortest_path_came_from[ctr];
            for(cx = 0; cx < 3; cx++)
            {
                Adjacent_Tile_Offset = ctr + Top_Left_Adjacents[cx];
                Adjacent_Tile_Cost = shortest_path_cost_to_reach[Adjacent_Tile_Offset];
                if(Adjacent_Tile_Cost != INF)
                {
                    new_cost_to_reach = (uint8_t)(Adjacent_Tile_Cost + move_cost);
                    if(shortest_path_cost_to_reach[ctr] > new_cost_to_reach)
                    {
                        shortest_path_came_from[ctr] = Adjacent_Tile_Offset;
                        shortest_path_cost_to_reach[ctr] = new_cost_to_reach;
                        if(shortest_path_came_from[ctr] != Current_Source)
                        {
                            a_cost_was_updated = 1;
                        }
                    }
                }
            }
        }
        ctr++;

        /* Top Row Inner Tiles (ctr = 1 to Wdt - 2) */
        for(Row_LoopVar = 0; Row_LoopVar < Inner_Row_Length; Row_LoopVar++) {
            move_cost = movepath_cost_map[ctr];
            if(move_cost != INF) {
                Current_Source = shortest_path_came_from[ctr];
                for(cx = 0; cx < 5; cx++) {
                    Adjacent_Tile_Offset = ctr + Top_Row_Adjacents[cx];
                    Adjacent_Tile_Cost = shortest_path_cost_to_reach[Adjacent_Tile_Offset];
                    if(Adjacent_Tile_Cost != INF) {
                        new_cost_to_reach = (unsigned char)(Adjacent_Tile_Cost + move_cost);
                        if(shortest_path_cost_to_reach[ctr] > new_cost_to_reach) {
                            shortest_path_came_from[ctr] = Adjacent_Tile_Offset;
                            shortest_path_cost_to_reach[ctr] = new_cost_to_reach;
                            if(shortest_path_came_from[ctr] != Current_Source) {
                                a_cost_was_updated = 1;
                            }
                        }
                    }
                }
            }
            ctr++;
        }

        /* Top-Right Corner (ctr = Wdt - 1) */
        move_cost = movepath_cost_map[ctr];
        if(move_cost != INF) {
            Current_Source = shortest_path_came_from[ctr];
            for(cx = 0; cx < 3; cx++) {
                Adjacent_Tile_Offset = ctr + Top_Right_Adjacents[cx];
                Adjacent_Tile_Cost = shortest_path_cost_to_reach[Adjacent_Tile_Offset];
                if(Adjacent_Tile_Cost != INF) {
                    new_cost_to_reach = (unsigned char)(Adjacent_Tile_Cost + move_cost);
                    if(shortest_path_cost_to_reach[ctr] > new_cost_to_reach) {
                        shortest_path_came_from[ctr] = Adjacent_Tile_Offset;
                        shortest_path_cost_to_reach[ctr] = new_cost_to_reach;
                        if(shortest_path_came_from[ctr] != Current_Source) {
                            a_cost_was_updated = 1;
                        }
                    }
                }
            }
        }
        ctr++;

        /* Middle Rows (row 1 to Hgt - 2) */
        for(Col_LoopVar = 0; Col_LoopVar < Inner_Col_Height; Col_LoopVar++) {
            /* Left Column */
            move_cost = movepath_cost_map[ctr];
            if(move_cost != INF) {
                Current_Source = shortest_path_came_from[ctr];
                for(cx = 0; cx < 5; cx++) {
                    Adjacent_Tile_Offset = ctr + Left_Col_Adjacents[cx];
                    Adjacent_Tile_Cost = shortest_path_cost_to_reach[Adjacent_Tile_Offset];
                    if(Adjacent_Tile_Cost != INF) {
                        new_cost_to_reach = (unsigned char)(Adjacent_Tile_Cost + move_cost);
                        if(shortest_path_cost_to_reach[ctr] > new_cost_to_reach) {
                            shortest_path_came_from[ctr] = Adjacent_Tile_Offset;
                            shortest_path_cost_to_reach[ctr] = new_cost_to_reach;
                            if(shortest_path_came_from[ctr] != Current_Source) {
                                a_cost_was_updated = 1;
                            }
                        }
                    }
                }
            }
            ctr++;

            /* Inner Row Tiles */
            for(Row_LoopVar = 0; Row_LoopVar < Inner_Row_Length; Row_LoopVar++) {
                move_cost = movepath_cost_map[ctr];
                if(move_cost != INF) {
                    Current_Source = shortest_path_came_from[ctr];
                    for(cx = 0; cx < 8; cx++) {
                        Adjacent_Tile_Offset = ctr + Mid_Tile_Adjacents[cx];
                        Adjacent_Tile_Cost = shortest_path_cost_to_reach[Adjacent_Tile_Offset];
                        if(Adjacent_Tile_Cost != INF) {
                            new_cost_to_reach = (unsigned char)(Adjacent_Tile_Cost + move_cost);
                            if(shortest_path_cost_to_reach[ctr] > new_cost_to_reach) {
                                shortest_path_came_from[ctr] = Adjacent_Tile_Offset;
                                shortest_path_cost_to_reach[ctr] = new_cost_to_reach;
                                if(shortest_path_came_from[ctr] != Current_Source) {
                                    a_cost_was_updated = 1;
                                }
                            }
                        }
                    }
                }
                ctr++;
            }

            /* Right Column */
            move_cost = movepath_cost_map[ctr];
            if(move_cost != INF) {
                Current_Source = shortest_path_came_from[ctr];
                for(cx = 0; cx < 5; cx++) {
                    Adjacent_Tile_Offset = ctr + Right_Col_Adjacents[cx];
                    Adjacent_Tile_Cost = shortest_path_cost_to_reach[Adjacent_Tile_Offset];
                    if(Adjacent_Tile_Cost != INF) {
                        new_cost_to_reach = (unsigned char)(Adjacent_Tile_Cost + move_cost);
                        if(shortest_path_cost_to_reach[ctr] > new_cost_to_reach) {
                            shortest_path_came_from[ctr] = Adjacent_Tile_Offset;
                            shortest_path_cost_to_reach[ctr] = new_cost_to_reach;
                            if(shortest_path_came_from[ctr] != Current_Source) {
                                a_cost_was_updated = 1;
                            }
                        }
                    }
                }
            }
            ctr++;
        }

        /* Bottom-Left Corner */
        move_cost = movepath_cost_map[ctr];
        if(move_cost != INF) {
            Current_Source = shortest_path_came_from[ctr];
            for(cx = 0; cx < 3; cx++) {
                Adjacent_Tile_Offset = ctr + Bottom_Left_Adjacents[cx];
                Adjacent_Tile_Cost = shortest_path_cost_to_reach[Adjacent_Tile_Offset];
                if(Adjacent_Tile_Cost != INF) {
                    new_cost_to_reach = (unsigned char)(Adjacent_Tile_Cost + move_cost);
                    if(shortest_path_cost_to_reach[ctr] > new_cost_to_reach) {
                        shortest_path_came_from[ctr] = Adjacent_Tile_Offset;
                        shortest_path_cost_to_reach[ctr] = new_cost_to_reach;
                        if(shortest_path_came_from[ctr] != Current_Source) {
                            a_cost_was_updated = 1;
                        }
                    }
                }
            }
        }
        ctr++;

        /* Bottom Row Inner Tiles */
        for(Row_LoopVar = 0; Row_LoopVar < Inner_Row_Length; Row_LoopVar++) {
            move_cost = movepath_cost_map[ctr];
            if(move_cost != INF) {
                Current_Source = shortest_path_came_from[ctr];
                for(cx = 0; cx < 5; cx++) {
                    Adjacent_Tile_Offset = ctr + Bottom_Row_Adjacents[cx];
                    Adjacent_Tile_Cost = shortest_path_cost_to_reach[Adjacent_Tile_Offset];
                    if(Adjacent_Tile_Cost != INF) {
                        new_cost_to_reach = (unsigned char)(Adjacent_Tile_Cost + move_cost);
                        if(shortest_path_cost_to_reach[ctr] > new_cost_to_reach) {
                            shortest_path_came_from[ctr] = Adjacent_Tile_Offset;
                            shortest_path_cost_to_reach[ctr] = new_cost_to_reach;
                            if(shortest_path_came_from[ctr] != Current_Source) {
                                a_cost_was_updated = 1;
                            }
                        }
                    }
                }
            }
            ctr++;
        }

        /* Bottom-Right Corner */
        move_cost = movepath_cost_map[ctr];
        if(move_cost != INF) {
            Current_Source = shortest_path_came_from[ctr];
            for(cx = 0; cx < 3; cx++) {
                Adjacent_Tile_Offset = ctr + Bottom_Right_Adjacents[cx];
                Adjacent_Tile_Cost = shortest_path_cost_to_reach[Adjacent_Tile_Offset];
                if(Adjacent_Tile_Cost != INF) {
                    new_cost_to_reach = (unsigned char)(Adjacent_Tile_Cost + move_cost);
                    if(shortest_path_cost_to_reach[ctr] > new_cost_to_reach) {
                        shortest_path_came_from[ctr] = Adjacent_Tile_Offset;
                        shortest_path_cost_to_reach[ctr] = new_cost_to_reach;
                        if(shortest_path_came_from[ctr] != Current_Source) {
                            a_cost_was_updated = ST_TRUE;
                        }
                    }
                }
            }
        }
        ctr++;
    }

    /* [Skeleton step 4]  Back-trace from the target through predecessor links until a self-link; success iff that self-link == source (MoM-MovePath-Compare.md, "The shared skeleton"). */
    path_length = 0;
    ctr = dst_y * Wdt + dst_x;

    while(shortest_path_came_from[ctr] != ctr)
    {
        shortest_path_backtrace[path_length] = ctr;
        ctr = shortest_path_came_from[ctr];
        path_length++;
    }

    if(ctr != src_y * Wdt + src_x)
    {
        return 0;
    }

    /* [Skeleton step 5]  Reverse the collected indices and convert each 1-D tile index back to (x, y) (MoM-MovePath-Compare.md, "The shared skeleton"). */
    for(itr = 0; itr < path_length; itr++) {
        mvpth_x[itr] = (unsigned char)(shortest_path_backtrace[path_length - 1 - itr] % Wdt);
        mvpth_y[itr] = (unsigned char)(shortest_path_backtrace[path_length - 1 - itr] / Wdt);
    }

    return path_length;

}


// MGC o54p02
// drake178: UU_SPATH_Segmented()
// UU_SPATH_Segmented()
// MGC o54p03
// drake178: UU_SPATH_15by15()
// UU_SPATH_15by15()
// MGC o54p04
// drake178: UU_SPATH_Segments()
// UU_SPATH_Segments()
// MGC o54p05
// drake178: UU_SPATH_GetBorderTiles()
// UU_SPATH_GetBorderTiles()



/*

    MAGIC.EXE  ovr055

*/

// MGC o55p01
/*
12x8 of 5x5 = 60x40
*/
/**
 * @brief Builds a coarse section-to-section connectivity map from a tile movement map.
 *
 * @details
 * Partitions the 60x40 world into a 12x8 grid of 5x5 sections, then computes
 * directional passability between neighboring sections using local path checks.
 *
 * For each section, the routine evaluates cardinal exits and writes direction
 * bits into one byte in `result_map`:
 * - `1` = north
 * - `2` = east
 * - `4` = south
 * - `8` = west
 *
 * Connectivity checks are performed in two stages:
 * 1) Fast boundary-adjacency probes along the relevant section edge.
 * 2) Validation pathing with `Find_Shortest_Path(...)` over temporary local
 *    windows (`TopBottom_Section_Map` and `LeftRight_Section_Map`) to ensure an
 *    actual traversable route exists between opposite edge tiles.
 *
 * Horizontal wrap is preserved when building left/right windows for sections at
 * the world edges. After the primary pass, reciprocal neighbor bits are filled
 * into adjacent section cells.
 *
 * @param move_map Per-tile movement-cost map (world-sized, 60x40). Tiles with
 *                 value `-1` are treated as impassable.
 * @param result_map Output section graph buffer (12x8 = 96 bytes), one flags
 *                   byte per section.
 *
 * @return void
 *
 * @note Mutates `result_map` in place and relies on `Find_Shortest_Path` for
 *       route validation.
 * @warning Preserves OG behavior, including the known east-reciprocal bound bug
 *          in the final symmetry pass.
 *
 * @see Build_Connectivity_Graphs
 * @see Find_Shortest_Path
 */
void Build_Connectivity_Graph(int8_t * move_map, uint8_t * result_map)
{
    /*
        CLAUDE
        uint8_t (was int8_t) to match Find_Shortest_Path's uint8_t* movepath_cost_map/mvpth_x/mvpth_y params (C4057).
        Section maps hold unsigned cost bytes (impassable 0xFF); Retn arrays hold small coords.
    */
    uint8_t mvpth_y[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t mvpth_x[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t LeftRight_Section_Map[35] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t TopBottom_Section_Map[35] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t Passable_Direction_Flags = 0;   /* per-section result byte */
    int16_t dst_y = 0;
    int16_t dst_x = 0;
    int16_t Adjacent_Section_X = 0; /* wrapped neighbour column */
    int16_t path_length = 0;
    int16_t Y_Modifier = 0;     /* section-build offsets */
    int16_t src_y = 0;
    int16_t src_x = 0;
    int16_t X_LoopVar = 0;      /* edge-scan iterators */
    int16_t Y_LoopVar = 0;      /* edge-scan iterators */
    int16_t Y_GridIndex_2 = 0;  /* reciprocal-pass iterators */
    int16_t X_GridIndex_2 = 0;  /* reciprocal-pass iterators */
    int16_t X_GridBase = 0;     /* section pixel-base (× 5) (col) */
    int16_t X_GridIndex = 0;    /* section-grid iterators (12×8) */
    int16_t Y_GridIndex = 0;    /* section-grid iterators (12×8) */
    int16_t itr1 = 0;           /* section pixel-base (× 5) (row) */
    int16_t itr2 = 0;           /* section-build offsets */

    itr1 = 0;

    /* 12x8 * 5x5 */
    for(Y_GridIndex = 0; Y_GridIndex < 8; Y_GridIndex++)
    {
        X_GridBase = 0;
        for(X_GridIndex = 0; X_GridIndex < 12; X_GridIndex++)
        {
            Passable_Direction_Flags = 0;
            if(Y_GridIndex != 0)
            {
                if(move_map[itr1 * WORLD_WIDTH + X_GridBase] != (char)-1)
                {
                    if(move_map[(itr1 - 1) * WORLD_WIDTH + X_GridBase] != (char)-1 ||
                        move_map[(itr1 - 1) * WORLD_WIDTH + X_GridBase + 1] != (char)-1)
                    {
                        Passable_Direction_Flags |= 1;
                    }
                }
                for(X_LoopVar = X_GridBase + 1; X_LoopVar < X_GridBase + 4 && !(Passable_Direction_Flags & 1); X_LoopVar++)
                {
                    if(move_map[itr1 * WORLD_WIDTH + X_LoopVar] != (char)-1)
                    {
                        if(move_map[(itr1 - 1) * WORLD_WIDTH + X_LoopVar - 1] != (char)-1 ||
                            move_map[(itr1 - 1) * WORLD_WIDTH + X_LoopVar] != (char)-1 ||
                            move_map[(itr1 - 1) * WORLD_WIDTH + X_LoopVar + 1] != (char)-1)
                        {
                            Passable_Direction_Flags |= 1;
                        }
                    }
                }
                if(move_map[itr1 * WORLD_WIDTH + X_GridBase + 4] != (char)-1 && !(Passable_Direction_Flags & 1))
                {
                    if(move_map[(itr1 - 1) * WORLD_WIDTH + X_GridBase + 4] != (char)-1 ||
                        move_map[(itr1 - 1) * WORLD_WIDTH + X_GridBase + 3] != (char)-1)
                    {
                        Passable_Direction_Flags |= 1;
                    }
                }
                if(Passable_Direction_Flags & 1)
                {
                    Passable_Direction_Flags |= 0x10;
                }
                path_length = 0;
                for(Y_Modifier = 0; Y_Modifier < 7; Y_Modifier++)
                {
                    for(itr2 = 0; itr2 < 5; itr2++)
                    {
                        TopBottom_Section_Map[Y_Modifier * 5 + itr2] = move_map[(itr1 + Y_Modifier - 1) * WORLD_WIDTH + X_GridBase + itr2];
                    }
                }
                if(Y_GridIndex == 7)
                {
                    for(itr2 = 0; itr2 < 5; itr2++)
                    {
                        TopBottom_Section_Map[30 + itr2] = (char)-1;
                    }
                }
                for(src_x = 0; src_x < 5 && path_length == 0; src_x++)
                {
                    for(dst_x = 0; dst_x < 5 && path_length == 0; dst_x++)
                    {
                        path_length += Find_Shortest_Path(src_x, 4, dst_x, 0, 5, 7, TopBottom_Section_Map, mvpth_x, mvpth_y);
                    }
                }
                if(path_length == 0)
                {
                    Passable_Direction_Flags &= ~1;
                }
            }

            if(Y_GridIndex != 7)
            {
                if(move_map[(itr1 + 4) * WORLD_WIDTH + X_GridBase] != (char)-1)
                {
                    if(move_map[(itr1 + 5) * WORLD_WIDTH + X_GridBase] != (char)-1 ||
                        move_map[(itr1 + 5) * WORLD_WIDTH + X_GridBase + 1] != (char)-1)
                    {
                        Passable_Direction_Flags |= 4;
                    }
                }
                for(X_LoopVar = X_GridBase + 1; X_LoopVar < X_GridBase + 4 && !(Passable_Direction_Flags & 4); X_LoopVar++)
                {
                    if(move_map[(itr1 + 4) * WORLD_WIDTH + X_LoopVar] != (char)-1)
                    {
                        if(move_map[(itr1 + 5) * WORLD_WIDTH + X_LoopVar - 1] != (char)-1 ||
                            move_map[(itr1 + 5) * WORLD_WIDTH + X_LoopVar] != (char)-1 ||
                            move_map[(itr1 + 5) * WORLD_WIDTH + X_LoopVar + 1] != (char)-1)
                        {
                            Passable_Direction_Flags |= 4;
                        }
                    }
                }
                if(move_map[(itr1 + 4) * WORLD_WIDTH + X_GridBase + 4] != (char)-1 && !(Passable_Direction_Flags & 4))
                {
                    if(move_map[(itr1 + 5) * WORLD_WIDTH + X_GridBase + 4] != (char)-1 ||
                        move_map[(itr1 + 5) * WORLD_WIDTH + X_GridBase + 3] != (char)-1)
                    {
                        Passable_Direction_Flags |= 4;
                    }
                }
                if(Passable_Direction_Flags & 4)
                {
                    Passable_Direction_Flags |= 0x40;
                }
                path_length = 0;
                for(Y_Modifier = 0; Y_Modifier < 7; Y_Modifier++)
                {
                    for(itr2 = 0; itr2 < 5; itr2++)
                    {
                        TopBottom_Section_Map[Y_Modifier * 5 + itr2] = move_map[(itr1 + Y_Modifier - 1) * WORLD_WIDTH + X_GridBase + itr2];
                    }
                }
                if(Y_GridIndex == 0)
                {
                    for(itr2 = 0; itr2 < 5; itr2++)
                    {
                        TopBottom_Section_Map[itr2] = (char)-1;
                    }
                }
                for(src_x = 0; src_x < 5 && path_length == 0; src_x++)
                {
                    for(dst_x = 0; dst_x < 5 && path_length == 0; dst_x++)
                    {
                        path_length += Find_Shortest_Path(src_x, 2, dst_x, 6, 5, 7, TopBottom_Section_Map, mvpth_x, mvpth_y);
                    }
                }
                if(path_length == 0)
                {
                    Passable_Direction_Flags &= ~4;
                }
            }

            Adjacent_Section_X = X_GridBase - 1;
            if(Adjacent_Section_X < 0)
            {
                Adjacent_Section_X += WORLD_WIDTH;
            }
            if(move_map[itr1 * WORLD_WIDTH + X_GridBase] != (char)-1)
            {
                if(move_map[itr1 * WORLD_WIDTH + Adjacent_Section_X] != (char)-1 ||
                    move_map[(itr1 + 1) * WORLD_WIDTH + Adjacent_Section_X] != (char)-1)
                {
                    Passable_Direction_Flags |= 8;
                }
            }
            for(Y_LoopVar = itr1 + 1; Y_LoopVar < itr1 + 4 && !(Passable_Direction_Flags & 8); Y_LoopVar++)
            {
                if(move_map[Y_LoopVar * WORLD_WIDTH + X_GridBase] != (char)-1)
                {
                    if(move_map[(Y_LoopVar - 1) * WORLD_WIDTH + Adjacent_Section_X] != (char)-1 ||
                        move_map[Y_LoopVar * WORLD_WIDTH + Adjacent_Section_X] != (char)-1 ||
                        move_map[(Y_LoopVar + 1) * WORLD_WIDTH + Adjacent_Section_X] != (char)-1)
                    {
                        Passable_Direction_Flags |= 8;
                    }
                }
            }
            if(move_map[(itr1 + 4) * WORLD_WIDTH + X_GridBase] != (char)-1 && !(Passable_Direction_Flags & 8))
            {
                if(move_map[(itr1 + 4) * WORLD_WIDTH + Adjacent_Section_X] != (char)-1 ||
                    move_map[(itr1 + 3) * WORLD_WIDTH + Adjacent_Section_X] != (char)-1)
                {
                    Passable_Direction_Flags |= 8;
                }
            }
            if(Passable_Direction_Flags & 8)
            {
                Passable_Direction_Flags |= 0x80;
            }
            path_length = 0;
            for(Y_Modifier = 0; Y_Modifier < 5; Y_Modifier++)
            {
                for(itr2 = 0; itr2 < 7; itr2++)
                {
                    LeftRight_Section_Map[Y_Modifier * 7 + itr2] = move_map[(itr1 + Y_Modifier) * WORLD_WIDTH + X_GridBase + itr2 - 1];
                }
            }
            if(X_GridIndex == 0)
            {
                for(Y_Modifier = 0; Y_Modifier < 5; Y_Modifier++)
                {
                    LeftRight_Section_Map[Y_Modifier * 7] = move_map[(itr1 + Y_Modifier) * WORLD_WIDTH + X_GridBase + 59];
                }
            }
            for(src_y = 0; src_y < 5 && path_length == 0; src_y++)
            {
                for(dst_y = 0; dst_y < 5 && path_length == 0; dst_y++)
                {
                    path_length += Find_Shortest_Path(4, src_y, 0, dst_y, 7, 5, LeftRight_Section_Map, mvpth_x, mvpth_y);
                }
            }
            if(path_length == 0)
            {
                Passable_Direction_Flags &= ~8;
            }

            Adjacent_Section_X = X_GridBase + 1;
            if(Adjacent_Section_X >= WORLD_WIDTH)
            {
                Adjacent_Section_X -= WORLD_WIDTH;
            }
            if(move_map[itr1 * WORLD_WIDTH + X_GridBase] != (char)-1)
            {
                if(move_map[itr1 * WORLD_WIDTH + Adjacent_Section_X] != (char)-1 ||
                    move_map[(itr1 + 1) * WORLD_WIDTH + Adjacent_Section_X] != (char)-1)
                {
                    Passable_Direction_Flags |= 2;
                }
            }
            for(Y_LoopVar = itr1 + 1; Y_LoopVar < itr1 + 4 && !(Passable_Direction_Flags & 2); Y_LoopVar++)
            {
                if(move_map[Y_LoopVar * WORLD_WIDTH + X_GridBase] != (char)-1)
                {
                    if(move_map[(Y_LoopVar - 1) * WORLD_WIDTH + Adjacent_Section_X] != (char)-1 ||
                        move_map[Y_LoopVar * WORLD_WIDTH + Adjacent_Section_X] != (char)-1 ||
                        move_map[(Y_LoopVar + 1) * WORLD_WIDTH + Adjacent_Section_X] != (char)-1)
                    {
                        Passable_Direction_Flags |= 2;
                    }
                }
            }
            if(move_map[(itr1 + 4) * WORLD_WIDTH + X_GridBase] != (char)-1 && !(Passable_Direction_Flags & 2))
            {
                if(move_map[(itr1 + 4) * WORLD_WIDTH + Adjacent_Section_X] != (char)-1 ||
                    move_map[(itr1 + 3) * WORLD_WIDTH + Adjacent_Section_X] != (char)-1)
                {
                    Passable_Direction_Flags |= 2;
                }
            }
            if(Passable_Direction_Flags & 2)
            {
                Passable_Direction_Flags |= 0x20;
            }
            path_length = 0;
            for(Y_Modifier = 0; Y_Modifier < 5; Y_Modifier++)
            {
                for(itr2 = 0; itr2 < 7; itr2++)
                {
                    LeftRight_Section_Map[Y_Modifier * 7 + itr2] = move_map[(itr1 + Y_Modifier) * WORLD_WIDTH + X_GridBase + itr2 - 1];
                }
            }
            if(X_GridIndex == 11)
            {
                for(Y_Modifier = 0; Y_Modifier < 5; Y_Modifier++)
                {
                    LeftRight_Section_Map[Y_Modifier * 7 + 6] = move_map[(itr1 + Y_Modifier) * WORLD_WIDTH];
                }
            }
            for(src_y = 0; src_y < 5 && path_length == 0; src_y++)
            {
                for(dst_y = 0; dst_y < 5 && path_length == 0; dst_y++)
                {
                    path_length += Find_Shortest_Path(2, src_y, 6, dst_y, 7, 5, LeftRight_Section_Map, mvpth_x, mvpth_y);
                }
            }
            if(path_length == 0)
            {
                Passable_Direction_Flags &= ~2;
            }

            result_map[Y_GridIndex * 12 + X_GridIndex] = (char)Passable_Direction_Flags;
            X_GridBase += 5;
        }
        itr1 += 5;
    }

    for(Y_GridIndex_2 = 0; Y_GridIndex_2 < 8; Y_GridIndex_2++)
    {
        for(X_GridIndex_2 = 0; X_GridIndex_2 < 12; X_GridIndex_2++)
        {
            if((result_map[Y_GridIndex_2 * 12 + X_GridIndex_2] & 1) && Y_GridIndex_2 != 0)
            {
                result_map[(Y_GridIndex_2 - 1) * 12 + X_GridIndex_2] |= 4;
            }
            if((result_map[Y_GridIndex_2 * 12 + X_GridIndex_2] & 4) && Y_GridIndex_2 < 7)
            {
                result_map[(Y_GridIndex_2 + 1) * 12 + X_GridIndex_2] |= 1;
            }
            if((result_map[Y_GridIndex_2 * 12 + X_GridIndex_2] & 8) && X_GridIndex_2 != 0)
            {
                result_map[Y_GridIndex_2 * 12 + X_GridIndex_2 - 1] |= 2;
            }
            /* OGBUG  East column bound tests the wrong loop variable: the asm (ovr055, loc_5227A) does `cmp [bp+Y_GridIndex_2], 0Bh; jge` -- it compares the ROW index Y_GridIndex_2 to 11, not the COLUMN index X_GridIndex_2. Since Y_GridIndex_2 is in [0,7], `< 11` is always true, so the East->West reciprocal bit is written even for the rightmost column (X_GridIndex_2 == 11), where the target index Y*12+12 == (Y+1)*12+0 leaks a West bit into column 0 of the next row down. Faithful to the OG; harmless because connectivity_grid_land/connectivity_grid_sea are NIU (never read for gameplay). Do not "fix". */
            if((result_map[Y_GridIndex_2 * 12 + X_GridIndex_2] & 2) && Y_GridIndex_2 < 11)
            {
                result_map[Y_GridIndex_2 * 12 + X_GridIndex_2 + 1] |= 8;
            }
        }
    }

}
