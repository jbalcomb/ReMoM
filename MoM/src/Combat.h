/*
    Combat

    WIZARDS.EXE
        ovr090
        ovr091  ¿ MoO2  COMBINIT ?
        ovr096
        ovr098  ¿ MoO2  COMBINIT ?
        ovr099  ¿ MoO2  Module: CMBTDRW1 ?
        ovr103
        ovr105
        ovr110  ¿ Strategic Combat ?
        ovr111  ¿ AITP.* ?
        ovr112  ¿ LBX CMBMAGIC ?
        ovr116  MoO2  COMBINIT
        ovr122
        ovr123
        ovr124
        ovr139
        ovr153
        ovr154
        ovr155
        ovr163

*/

#ifndef COMBAT_H
#define COMBAT_H

#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_TYPE.h"



// _ai_battlefield_city_walls
#define BATTLEFIELD_CITY_WALL_STONE     0x1 /* 0b00000001 */
#define BATTLEFIELD_CITY_WALL_FIRE      0x2 /* 0b00000010 */
#define BATTLEFIELD_CITY_WALL_DARKNESS  0x4 /* 0b00000100 */

enum e_COMBAT_ENTITY_MISSILE_TYPE
{
    msl_Lightning   =  0,
    msl_Arrow       =  1,
    msl_Fireball    =  2,
    msl_Illusion    =  3,
    msl_Rocks       =  4,
    msl_Sling       =  5,
    msl_Deathbolt   =  6,
    msl_Icebolt     =  7,
    msl_Scatter     =  8,
    msl_Priest      =  9,
    msl_Drow        = 10,
    msl_Shimmer     = 11,
    msl_Web         = 12,
    msl_Green       = 13,
    msl_Cloud       = 14
};

enum e_COMBAT_ENTITY_MISSILE_DIRECTION
{
    cemd_Up         = 0,
    cemd_UpRight    = 1,
    cemd_Right      = 2,
    cemd_DownRight  = 3,
    cemd_Down       = 4,
    cemd_DownLeft   = 5,
    cemd_Left       = 6,
    cemd_UpLeft     = 7
};

// sizeof:   Eh  14d
struct s_MISSILE
{
    /* 00 */ int16_t src_screen_x;
    /* 02 */ int16_t src_screen_y;
    /* 04 */ int16_t dst_screen_x;
    /* 06 */ int16_t dst_screen_y;
    /* 08 */ int16_t type;              /* enum e_COMBAT_ENTITY_MISSILE_TYPE */
    /* 0A */ int16_t travel_percent;
    /* 0C */ int16_t direction;         /* enum e_COMBAT_ENTITY_MISSILE_DIRECTION */
    /* 0E */
};

// WHERE IS THIS COMING FROM!?!  Unterminated '#pragma pack (push, ...)' at end of file
// #pragma pack(push)
// #pragma pack(2)
struct s_CLANGD
{
    /* 00 */  int16_t wtf;
    /* 02 */  int16_t mgfy;
    /* 04 */
};
// #pragma pack(pop)

//  ; (sizeof=0x4, standard type)
// #pragma pack(push)
// #pragma pack(2)
struct s_COMBAT_ENCHANTMENT_ICON_DATA
{
    /* 00 */  int16_t icon_idx;
    /* 02 */  int16_t help_idx;
    /* 04 */
};
// #pragma pack(pop)

//  ; (sizeof=0x4, standard type)
// #pragma pack(push)
// #pragma pack(2)
struct s_COMBAT_ENCHANTMENT_ICON
{
    /* 00 */ SAMB_ptr icon_seg;
    /* 02 */ int16_t help_idx;
    /* 04 */
};
// #pragma pack(pop)


extern SAMB_ptr DBG_EmmHndl_FIGUREX;
extern SAMB_ptr DBG_figure_pict_base_seg;





#define COMBAT_GRID_X1(_x_,_y_)     ( Get_Combat_Grid_Cell_X( ( (_x_) + 4), ((_y_) + 4) ) )
#define COMBAT_GRID_Y1(_x_,_y_)     ( Get_Combat_Grid_Cell_Y( ( (_x_) + 4), ((_y_) + 4) ) )

#define CALC_ROW_START_CGX  row_start_cgx = (itr_y / 2)
#define CALC_ROW_START_CGY  row_start_cgy = (((itr_y + 1) / 2) + 9)

#define CALC_CGX       cgx = (row_start_cgx + itr_x)
#define CALC_CGY       cgy = (row_start_cgy - itr_x)

#define CALC_SCREEN_X   screen_x = ((itr_x * 32) - ((itr_y & 0x1) * 16))
#define CALC_SCREEN_Y   screen_y = ((itr_y * 8) - 8)


#define COMBAT_GRID_CELL_WIDTH       32  // in pixels
#define COMBAT_GRID_CELL_HEIGHT      16  // in pixels

#define COMBAT_GRID_WIDTH   21  // in Cells
#define COMBAT_GRID_HEIGHT  22  // in Cells
#define COMBAT_GRID_COLS    21  // in Cells
#define COMBAT_GRID_ROWS    22  // in Cells
/* Global Grid Dimensions */
#define COMBAT_GRID_CELLS  (COMBAT_GRID_WIDTH * COMBAT_GRID_HEIGHT) /* 21 * 22 = 462 */

#define COMBAT_GRID_XMIN     0  // in Cells
#define COMBAT_GRID_XMAX    21  // in Cells
#define COMBAT_GRID_YMIN     0  // in Cells
#define COMBAT_GRID_YMAX    22  // in Cells

#define COMBAT_STRUCTURE_CGX  6
#define COMBAT_STRUCTURE_CGY 11
#define COMBAT_STRUCTURE_IDX ((COMBAT_STRUCTURE_CGY * COMBAT_GRID_WIDTH)   + COMBAT_STRUCTURE_CGX)
#define COMBAT_GRID_CITY_AREA_WIDTH     4
#define COMBAT_GRID_CITY_AREA_HEIGHT    4
/*

CMB_DrawMap__WIP()
    if
        (battlefield->center_square_structure == CS_SorceryNode)
        (battlefield->center_square_structure == CS_ChaosNode)
        (battlefield->center_square_structure == CS_Outpost)
        && (cgx == 6) && (cgy == 11)
    if
        (battlefield->center_square_structure == CS_City)
        (battlefield->center_square_structure == CS_Fortress)
        && (cgx == 8) && (cgy == 13)

*/
/*
Generate_Combat_Map()
                battlefield->Wall_Sections[5] = 0;
                battlefield->Wall_Sections[9] = 0;
                battlefield->Wall_Sections[6] = 0;
                battlefield->Wall_Sections[10] = 0;

*/




/*

    BEGIN:  Combat Map Path Finding

*/

#define IN_RANGE(x, lower, upper) ((x) >= (lower) && (x) <= (upper))
#define GTELT(_value_, _lower_bound_, _upper_bound_) ((_value_) >= (_lower_bound_) && (_value_) < (_upper_bound_))

#define COMBAT_GRID_CELL_COUNT  (COMBAT_GRID_WIDTH * COMBAT_GRID_HEIGHT)  /* (21 * 22) = 462 */

#define NUMBER_OF_VERTICES      COMBAT_GRID_CELL_COUNT
#define TERMINAL_VERTEX         ((destination_cgy * COMBAT_GRID_WIDTH) + destination_cgx) 

/* CLAUDE  INF / IS_INF moved to the shared MOX_TYPE.h (already included by Combat.h) so all three shortest-path solvers share one sentinel. */


/*
    reduce the loops over the adjacent cells to a single macro
    iter 0-8 or iter 0-4 & 4-8

NEW_PATH_COST()
...the the portion inside each iteration, because that just needs itr_adjacent

NEW_PATH_COST_0()
NEW_PATH_COST_1()
NEW_PATH_COST_2()
...treats the three adjacent cell offsets array as one array - adjacent_offsets[(({0,1,2} * 8) + itr_adjacent)]

NEW_PATH_COST_ANY()
...takes a parameter, which just reduces NEW_PATH_COST_0(), NEW_PATH_COST_1(), and NEW_PATH_COST_2()

what I want is all of this...
            move_cost = _cmbt_movepath_cost_map[ctr];  // moves2 cost of cell, given terrain and movement mode
            if(!IS_INF(move_cost))
            {
                current_origin = _cmbt_path_data[ctr];
                for(itr_adjacent = 0; itr_adjacent < 4; itr_adjacent++)
                {
                    adjacent_idx = adjacent_offsets[1][itr_adjacent];
                    NEW_PATH_COST()
                }
                for(itr_adjacent = 4; itr_adjacent < 8; itr_adjacent++)
                {
                    adjacent_idx = adjacent_offsets[1][itr_adjacent];
                    NEW_PATH_COST()
                }
            }
            ctr++;

NEW_PATH_COST_ALL()
...uses NEW_PATH_COST_ANY()


*/
#define NEW_PATH_COST() {                                               \
    if((adjacent_idx >= 0) && (adjacent_idx < COMBAT_GRID_CELL_COUNT))  \
    {                                                                   \
        adjacent_path_cost = _cmbt_mvpth_c[adjacent_idx];               \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(_cmbt_mvpth_c[ctr] > potential_path_cost)                \
            {                                                           \
                _cmbt_path_data[ctr] = adjacent_idx;                    \
                _cmbt_mvpth_c[ctr] = (uint8_t)potential_path_cost;      \
                if(_cmbt_path_data[ctr] != current_origin)              \
                {                                                       \
                    a_cost_was_updated = ST_TRUE;                       \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}


#define NEW_PATH_COST_0() {                                             \
    adjacent_idx = adjacent_offsets[((0 * 8) + itr_adjacent)];          \
    if((adjacent_idx >= 0) && (adjacent_idx < COMBAT_GRID_CELL_COUNT))  \
    {                                                                   \
        adjacent_path_cost = _cmbt_mvpth_c[adjacent_idx];               \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(_cmbt_mvpth_c[ctr] > potential_path_cost)                \
            {                                                           \
                _cmbt_path_data[ctr] = adjacent_idx;                    \
                _cmbt_mvpth_c[ctr] = (uint8_t)potential_path_cost;      \
                if(_cmbt_path_data[ctr] != current_origin)              \
                {                                                       \
                    a_cost_was_updated = ST_TRUE;                       \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#define NEW_PATH_COST_1() {                                             \
    adjacent_idx = adjacent_offsets[((1 * 8) + itr_adjacent)];          \
    if((adjacent_idx >= 0) && (adjacent_idx < COMBAT_GRID_CELL_COUNT))  \
    {                                                                   \
        adjacent_path_cost = _cmbt_mvpth_c[adjacent_idx];               \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(_cmbt_mvpth_c[ctr] > potential_path_cost)                \
            {                                                           \
                _cmbt_path_data[ctr] = adjacent_idx;                    \
                _cmbt_mvpth_c[ctr] = (uint8_t)potential_path_cost;      \
                if(_cmbt_path_data[ctr] != current_origin)              \
                {                                                       \
                    a_cost_was_updated = ST_TRUE;                       \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#define NEW_PATH_COST_2() {                                             \
    adjacent_idx = adjacent_offsets[((2 * 8) + itr_adjacent)];          \
    if((adjacent_idx >= 0) && (adjacent_idx < COMBAT_GRID_CELL_COUNT))  \
    {                                                                   \
        adjacent_path_cost = _cmbt_mvpth_c[adjacent_idx];               \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(_cmbt_mvpth_c[ctr] > potential_path_cost)                \
            {                                                           \
                _cmbt_path_data[ctr] = adjacent_idx;                    \
                _cmbt_mvpth_c[ctr] = (uint8_t)potential_path_cost;      \
                if(_cmbt_path_data[ctr] != current_origin)              \
                {                                                       \
                    a_cost_was_updated = ST_TRUE;                       \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#define NEW_PATH_COST_ANY(_value_) {                                            \
    adjacent_idx = (ctr + adjacent_offsets[(((_value_) * 8) + itr_adjacent)]);  \
    if(adjacent_idx == ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx))               \
    {                                                                           \
        STU_DEBUG_BREAK();                                                      \
    }                                                                           \
    if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))                          \
    {                                                                           \
        adjacent_path_cost = _cmbt_mvpth_c[adjacent_idx];                       \
        if(!IS_INF(adjacent_path_cost))                                         \
        {                                                                       \
            potential_path_cost = adjacent_path_cost + move_cost;               \
            existing_path_cost = _cmbt_mvpth_c[ctr];                            \
            if(existing_path_cost > potential_path_cost)                        \
            {                                                                   \
                _cmbt_path_data[ctr] = adjacent_idx;                            \
                _cmbt_mvpth_c[ctr] = (uint8_t)potential_path_cost;              \
                new_next_cell_index = _cmbt_path_data[ctr];                     \
                if(new_next_cell_index != current_origin)                       \
                {                                                               \
                    unstable = ST_TRUE;                                         \
                }                                                               \
            }                                                                   \
        }                                                                       \
    }                                                                           \
}

#define NEW_PATH_COST_ALL(_value_) {                            \
    move_cost = _cmbt_movepath_cost_map[ctr];                   \
    if(!IS_INF(move_cost))                                      \
    {                                                           \
        current_origin = _cmbt_path_data[ctr];                  \
        for(itr_adjacent = 0; itr_adjacent < 4; itr_adjacent++) \
        {                                                       \
            NEW_PATH_COST_ANY(itr_adjacent)                     \
        }                                                       \
        for(itr_adjacent = 4; itr_adjacent < 8; itr_adjacent++) \
        {                                                       \
            NEW_PATH_COST_ANY(itr_adjacent)                     \
        }                                                       \
    }                                                           \
    ctr++;                                                      \
}



/*

    END:  Combat Map Path Finding

*/






/*

Combat Battle Unit Figure Picture Cache

Claim_EMS_Page_For_Figure_Set()
Combat_Figure_Compose()
Combat_Screen_Map_Compose_Figures()
Combat_Figure_Load()


Claim_EMS_Page_For_Figure_Set()
    logical_page = ((bufpi * 3) / 2);
    if((bufpi & 0x1) == 0)
    {
        offset = 0;
    }
    else
    {
        offset = (512 * SZ_PARAGRAPH_B);  // 512 PR, 8192 B; 512 / 56 = 9.14 ... ¿ 2 sets of 9 figure pictures ?
    }
    offset += (bufpi * (56 * SZ_PARAGRAPH_B));  // 28 * 30 = 840 / 16 = 52.5 PR per figure picture
    EMS_PFBA = (EmmHndl_FIGUREX + (logical_page * SZ_EMM_LOGICAL_PAGE));

Combat_Figure_Compose()
    if((bufpi & 0x1) == 0)
    {
        offset = 0;
    }
    else
    {
        offset = (512 * SZ_PARAGRAPH_B);  // 512 PR, 8192 B; 512 / 56 = 9.14 ... ¿ 2 sets of 9 figure pictures ?
    }
    offset += (bufpi * (56 * SZ_PARAGRAPH_B));
    Claim_EMS_Page_For_Figure_Set(bufpi);
    figure_pointer_seg = Allocate_First_Block((EMS_PFBA + offset), 33);
    ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;

Combat_Screen_Map_Compose_Figures()
        Claim_EMS_Page_For_Figure_Set(bufpi);
        if((bufpi & 1) == 0)
        {
            offset = 0;
        }
        else
        {
            offset = (512 * SZ_PARAGRAPH_B);
        }
        offset += (bufpi * (56 * SZ_PARAGRAPH_B));
        figure_pointer_seg = Allocate_First_Block((EMS_PFBA + offset), 33);
        ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;

Combat_Figure_Load()
    Claim_EMS_Page_For_Figure_Set(bufpi);
    if((bufpi & 0x1) == 0)
    {
        offset = 0;
    }
    else
    {
        offset = (512 * SZ_PARAGRAPH_B);  // 512 PR, 8192 B; 512 / 56 = 9.14 ... ¿ 2 sets of 9 figure pictures ?
    }
    offset += (bufpi * (56 * SZ_PARAGRAPH_B));
    figure_pointer_seg = Allocate_First_Block((EMS_PFBA + offset), 33);  // 16-byte header + 8 8-byte pointers = (16 + (8 * 8)) = (16 + (64)) / 16 = 5
    ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;

Pieces & Parts

    1)  Claim_EMS_Page_For_Figure_Set(bufpi);

    2)  if((bufpi & 0x1) == 0)
        {
            offset = 0;
        }
        else
        {
            offset = (512 * SZ_PARAGRAPH_B);  // 512 PR, 8192 B; 512 / 56 = 9.14 ... ¿ 2 sets of 9 figure pictures ?
        }
        offset += (bufpi * (56 * SZ_PARAGRAPH_B));

    3)  figure_pointer_seg = Allocate_First_Block((EMS_PFBA + offset), 33);
        ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;

Claim_EMS_Page_For_Figure_Set()
    2
Combat_Figure_Compose()
    2, 1, 3
Combat_Screen_Map_Compose_Figures()
    1, 2, 3
Combat_Figure_Load()
    1, 2, 3

*/
// 28 * 30 = 840 * 9 = 7560 / 16 = 472.5
// offsets 56 paragraphs for each figure picture; 56 * = 896 - 840 = 56 / 3.5
// 
// segrax:  offset = (bufpi & 1) ? 512 : 0;
/*
bufpi
¿ three different macros for a (real) reason ?
    see above; different usages and orders

bufpi
    even or odd
    offset

*/
#define FIGUREX_MAP         Claim_EMS_Page_For_Figure_Set(bufpi);
#define FIGUREX_OFFSET      {                                               \
    offset = (bufpi & 0x1) ? (512 * SZ_PARAGRAPH_B) : (0 * SZ_PARAGRAPH_B); \
    offset += (bufpi * (56 * SZ_PARAGRAPH_B));                              \
}
#define FIGUREX_POINTER     {                                                       \
    figure_pict_set_seg = (SAMB_ptr *)Allocate_First_Block((EMS_PFBA + offset), 5); \
}

/*
    Global Battle Unit Macros

        Unit Special Abilities
        Unit Mutations
        Unit Enchantments
*/

#define BU_LUCKY()      ( ( battle_unit->Attribs_1 & USA_LUCKY     ) != 0 )
#define BU_CASTER_20()  ( ( battle_unit->Attribs_1 & USA_CASTER_20 ) != 0 )
#define BU_CASTER_40()  ( ( battle_unit->Attribs_1 & USA_CASTER_40 ) != 0 )



/*
    Tactical Combat Macros
*/

#define DAMAGE_TYPE_1   1   /* ¿ ? */
#define DAMAGE_TYPE_2   2   /* ¿ ? */
#define DAMAGE_TYPE_3   3    /* ¿ ? */

/*
    Combat Battle Unit Macros

        Unit Special Abilities
        Unit Mutations
        Unit Enchantments

attacker_battle_unit_idx
defender_battle_unit_idx

battle_units[].Attack_Flags
battle_units[].Attribs_1

battle_units[].Attack_Flags
    Att_ArmorPrc, Att_1stStrike, Att_Poison, Att_LifeSteal, Att_DoomDmg, Att_Destruct, Att_Illusion, Att_StnTouch, _noeffect100, Att_DthTouch, Att_PwrDrain, Att_DsplEvil, Att_AREAFLAG, Att_DMGLIMIT, Att_EldrWeap, Att_WarpLghtn

battle_units[].Attribs_1
    USA_IMMUNITY_FIRE, USA_IMMUNITY_STONING, USA_IMMUNITY_MISSILES, USA_IMMUNITY_ILLUSION, USA_IMMUNITY_COLD, USA_IMMUNITY_MAGIC, USA_IMMUNITY_DEATH, USA_IMMUNITY_POISON, USA_IMMUNITY_WEAPON, USA_FLYING, USA_LUCKY, USA_SUMMON_DEMON_1, USA_SUMMON_DEMON_2, USA_CASTER_20, USA_CASTER_40, USA_RACE_SPECIFIC_UNIT

*/
#define ATT_POISON_ATTACK       ((battle_units[attacker_battle_unit_idx].Attack_Flags & Att_Poison) != 0)
#define DEF_POISON_IMMUNITY     ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_POISON) == 0)



/*
combat_results_scroll_message
~== Combat_Screen()  end_of_combat_message_type ... passed to End_Of_Combat()
~== Strategic_Combat()  end_of_combat_message_type ... passed to End_Of_Combat()
*/
enum e_COMBAT_SCROLL_MESSAGE_TYPE
{
    csmt_Victory           =  1,  // Victory, Tactical      CMBWIN  MESSAGE.LBX 5
    csmt_Defeat            =  2,  // Defeat, Tactical       CMBLOSE MESSAGE.LBX 6
    csmt_PlayerFled        =  3,  // Fled, Tactical         CMBLOSE MESSAGE.LBX 7 + GUI_NearMsgString paragraph
    csmt_TurnLimit         =  4,  // Draw                   CMBLOSE MESSAGE.LBX 8
    csmt_EnemyFled         =  5,  // Enemy Fled             CMBWIN  MESSAGE.LBX 9
    csmt_VictoryStrategic  =  6,  // Victory, Strategic     CMBWIN  MESSAGE.LBX 5
    csmt_DefeatStrategic   =  7,  // Defeat, Strategic      CMBLOSE MESSAGE.LBX 6
    csmt_NIU_08            =  8,  // DNE / NIU
    csmt_NIU_09            =  9,  // DNE / NIU
    csmt_NIU_10            = 10,  // DNE / NIU
    csmt_NIU_11            = 11,  // DNE / NIU
    csmt_CityLost          = 12   // City Lost              CMBLOSE "[city name] has been conquered"
};


enum e_COMBAT_ATTACK_MODE
{
    am_Melee            = 0,
    am_ThrownOrBreath   = 1,
    am_Ranged           = 2
};

/*
    _combat_environ
*/
enum e_COMBAT_ENVIRONMENT_TYPE
{
    cnv_Enemy_Stack     = 0,
    cnv_Enemy_City      = 1,
    cnv_Lair            = 5
};

/*
e_COMBAT_STRUCTURE
vs.
e_COMBAT_LOCATION_TYPE
vs.
enum e_CENTRAL_STRUCTURES

Generate_Combat_Map()
...switches on 

location type                           central structure
clt_Cave (8) and clt_MonsterLair (12)   CS_Cave
clt_Dungeon (9) and clt_Ruins (13)      CS_Dungeon
clt_AncientTemple (10)                  CS_Temple
clt_Keep (11)                           CS_Fort
clt_FallenTemple (14)                   CS_Ruins

*/
enum e_COMBAT_STRUCTURE
{
    cs_NONE              = 0,
    cs_City              = 1,
    cs_SorceryNode       = 2,
    cs_NatureNode        = 3,
    cs_ChaosNode         = 4,
    cs_UNKOWN            = 5,
    cs_OceanTerrainType  = 6
};

enum e_COMBAT_LOCATION_TYPE
{
    clt_OpenField       =  0, 
    clt_Outpost         =  1, 
    clt_City            =  2, 
    clt_Fortress        =  3,   /* Fortress City */
    clt_Tower           =  4, 
    clt_ChaosNode       =  5, 
    clt_NatureNode      =  6, 
    clt_SorceryNode     =  7, 
    clt_Cave            =  8, 
    clt_Dungeon         =  9, 
    clt_AncientTemple   = 10, 
    clt_Keep            = 11, 
    clt_MonsterLair     = 12, 
    clt_Ruins           = 13, 
    clt_FallenTemple    = 14
};


enum e_CENTRAL_STRUCTURES
{
    CS_None         =  0, 
    CS_Outpost      =  1, 
    CS_City         =  2, 
    CS_Fortress     =  3, 
    CS_Dungeon      =  4, 
    CS_Tower        =  5, 
    CS_Cave         =  6, 
    CS_Temple       =  7, 
    CS_Fort         =  8, 
    CS_SorceryNode  =  9, 
    CS_ChaosNode    = 10, 
    CS_NatureNode   = 11, 
    CS_Ruins        = 12
};


/*
    BEGIN: Combat Terrain
*/

/*
convert overland map terrain into combat map terrain

'Combat Terrain Set'
    Load_Combat_Terrain_Pictures()
    Generate_Combat_Map()

¿ overlaps with code using e_COMBAT_LOCATION_TYPE ?
*/

enum e_COMBAT_TERRAIN_SET
{
    cts_Water      = 0, 
    cts_Plains     = 1, /* only one that gets Carve_River_Terrain() */
    cts_Hills      = 2, 
    cts_Desert     = 3, 
    cts_Mountains  = 4, 
    cts_Tundra     = 5, 
    cts_Forest     = 6
};

enum e_COMBAT_TERRAIN_GROUP
{
    ctg_Grass      = 0, 
    CTG_Rough      = 1,  /* Rough (Dirt) */
    ctg_Dirt       = 2, 
    ctg_River      = 3, 
    ctg_Ocean      = 4, 
    ctg_NIU_5      = 5  // not used in Set_Movement_Cost_Maps()
};

enum e_BATTLEFIELD_TERRAIN_TYPE
{

    btt_Ocean_First  = 48,
    btt_Ocean_Last   = 51,

    btt_Cloud_First  = 52,
    btt_Cloud_Last   = 55,

    btt_River_First = 56,
};

enum CTILE_GFX
{
    CTILE_Base1  = 0,
    CTILE_Base2  = 1,
    CTILE_Base3  = 2,
    CTILE_Base4  = 3,

    CTILE_Dirt1  = 4,
    CTILE_Dirt2  = 5,
    CTILE_Dirt3  = 6,
    CTILE_Dirt4  = 7,

    CTILE_DownRightD1  = 8,
    CTILE_DownRightD2  = 9,
    CTILE_DownD1  = 10,
    CTILE_DownD2  = 11,
    CTILE_DownD_RightD1  = 12,
    CTILE_DownD_RightD2  = 13,
    CTILE_DownLeftD1  = 14,
    CTILE_DownLeftD2  = 15,
    CTILE_DownD_LeftD1  = 16,
    CTILE_DownD_LeftD2  = 17,
    CTILE_LeftD1  = 18,
    CTILE_LeftD2  = 19,
    CTILE_UpLeftD1  = 20,
    CTILE_UpLeftD2  = 21,
    CTILE_UpD_LeftD1  = 22,
    CTILE_UpD_LeftD2  = 23,
    CTILE_UpD1  = 24,
    CTILE_UpD2  = 25,
    CTILE_UpRightD1  = 26,
    CTILE_UpRightD2  = 27,
    CTILE_UpD_RightD1  = 28,
    CTILE_UpD_RightD2  = 29,
    CTILE_RightD1  = 30,
    CTILE_RightD2  = 31,

    CTILE_DownRough  = 32,  /* starting index for when middle is ctg_Dirt */
    CTILE_UpDownRough  = 33,
    CTILE_RightRough  = 34,
    CTILE_LeftRightRough  = 35,
    CTILE_CrossRough  = 36,
    CTILE_UpRough  = 37,
    CTILE_LeftRough  = 38,
    CTILE_DownRightRough  = 39,
    CTILE_DownLeftRightRough  = 40,
    CTILE_DownLeftRough  = 41,
    CTILE_UpDownRightRough  = 42,
    CTILE_UpDownLeftRough  = 43,
    CTILE_UpRightRough  = 44,
    CTILE_UpLeftRightRough  = 45,
    CTILE_UpLeftRough  = 46,
    CTILE_SingleRough  = 47,

    CTILE_Ocean1  = 48,
    CTILE_Ocean2  = 49,
    CTILE_Ocean3  = 50,
    CTILE_Ocean4  = 51,
    
    CTILE_Type5_1  = 52,
    CTILE_Type5_2  = 53,
    CTILE_Type5_3  = 54,
    CTILE_Type5_4  = 55,

    CTILE_LeftRightRiver1  = 56,
    CTILE_UpDownRiver1  = 57,
    CTILE_UpLeftRiver1  = 58,
    CTILE_UpRightRiver1  = 59,
    CTILE_DownRightRiver1  = 60,
    CTILE_DownLeftRiver1  = 61,
    CTILE_LeftRightRiver2  = 62,
    CTILE_UpDownRiver2  = 63,
    CTILE_UpLeftRiver2  = 64,
    CTILE_UpRightRiver2  = 65,
    CTILE_DownRightRiver2  = 66,
    CTILE_DownLeftRiver2  = 67
};
/*
    END: Combat Terrain
*/

enum E_COMBAT_ENTITY_TYPE
{
    cet_NONE               = 0,
    cet_Battle_Unit_Figure = 1,
    cet_Curse              = 2,
    cet_Missiles           = 3,
    cet_Tree_Or_Rock       = 4
};


/*
    ¿ Meaning / Usage ?

*/
enum e_BATTLE_UNIT_STATUS
{
    bus_Active      = 0,  /* ~== Alive; checked for 'attackable' target */
    bus_Recalled    = 1,  /* ¿ ? */
    bus_Fleeing     = 2,  /* the progressive tense is meaningful, this is a transitory state */
    bus_Uninvolved  = 3,  /* ~ "can not engage" used for 'Floating Island', Transports, etc.; included in 'Flee'; EoC, set to bus_Active; */
    bus_Dead        = 4,  /* ¿ ? */
    bus_Drained     = 5,  /* ¿ ? */
    bus_Gone        = 6   /* ¿ ? ¿ ~== _UNITS[].wp == 9 ? ¿ excluded from 'Raise Dead' */  /* ~== unsummoned, banished, disintegrated, stoned, cracks called */
};

/*
(battle_units[battle_unit_idx].Move_Flags & MV_FLYING)
etc.
*/
enum e_BATTLE_UNIT_MOVEMENT_MODE
{
    bumm_Ground0    = 0,
    bumm_Ground1    = 1,
    bumm_Flight     = 2,
    bumm_Ground3    = 3,
    bumm_Sailing    = 4,
    bumm_Swimming   = 5,
    bumm_Teleport          = 6,
    bumm_Tunnel          = 7
};

enum e_BATTLE_UNIT_ACTION
{
    bua_Ready           =   0,
    bua_Finished        =   4,
    bua_Wait            =   5,
    /* ¿ BEGIN:  computer-player actions ? */
    bua_Stab     = 100,
    bua_Shoot    = 101,
    bua_MoveAndShoot       = 102,
    bua_MoveAndStab     = 103,
    BUA_DoomBolt        = 104,
    BUA_Fireball        = 105,
    bua_Healing         = 106,
    BUA_UseItem         = 107,
    BUA_CastSpell       = 108,
    BUA_SummonDemon     = 109,
    bua_WebSpell        = 110,
    bua_Flee            = 150,
    /* ¿ END:  computer-player actions ? */
    BUA_No_Spells       = 333
};

/*
applies a special effect to the battle unit figure bitmap is loaded into the GUI_SmallWork_IMG@ allocation
1: greyscale (Black Sleep)
2: bluescale (unused?)
3: redscale (Warp Creature)
4: clear (Invisibility, spotted)
5: strip (Invisibility, undetected)
*/
enum e_BATTLE_UNIT_FIGURE_EFFECT
{
    bufe_NONE               = 0,
    bufe_Black_Sleep        = 1,  /* greyscale */
    bufe_2                  = 2,  /* reverse-greyscale, tint 104 */
    bufe_Warp_Creature      = 3,  /* reverse-greyscale, tint  40 */
    bufe_Invisible_Revealed = 4,  /* flat silhouette */
    bufe_Invisible_Hidden   = 5   /* sprite transparent */
};

/*
Begin_Combat_Turn() uses `test .. jz`
*/
enum e_BATTLE_UNIT_EFFECT
{
    bue_Vertigo           = 0x1,
    bue_Confusion         = 0x2,
    bue_Whirlwind         = 0x4,
    bue_Mind_Storm        = 0x8,
    bue_Shatter           = 0x10,
    bue_Weakness          = 0x20,
    bue_Black_Sleep       = 0x40,
    bue_Warped_Attack     = 0x80,
    bue_Warped_Defense    = 0x100,
    bue_Warped_Resist     = 0x200,
    bue_Mind_Twist        = 0x400,
    bue_Haste             = 0x800,
    bue_Web               = 0x1000,
    bue_Creature_Binding  = 0x2000,
    bue_Possession        = 0x4000,
    bue_NoEffect          = 0x8000
};

/*

if(combat_enchantments[CALL_LIGHTNING_DFNDR] != 0)


like with Update_Combat_Enchantments_Icon_And_Help()
// even/odd - attacker/defender
if((itr % 2) == 0)
    _combat_enchantments_attacker[attacker_enchantment_count].icon_seg = combat_enchantment_icon_segs[combat_enchantment_icon_data[idx].icon_idx];
    _combat_enchantments_attacker[attacker_enchantment_count].help_idx = combat_enchantment_icon_data[idx].help_idx;
else
    _combat_enchantments_defender[defender_enchantment_count].icon_seg = combat_enchantment_icon_segs[combat_enchantment_icon_data[idx].icon_idx];
    _combat_enchantments_defender[defender_enchantment_count].help_idx = combat_enchantment_icon_data[idx].help_idx;

*/

#define TRUE_LIGHT_ATTKR 0
#define TRUE_LIGHT_DFNDR 1
#define DARKNESS_ATTKR 2
#define DARKNESS_DFNDR 3
#define WARP_REALITY_ATTKR 4
#define WARP_REALITY_DFNDR 5
#define BLACK_PRAYER_ATTKR 6
#define BLACK_PRAYER_DFNDR 7
#define WRACK_ATTKR 8
#define WRACK_DFNDR 9
#define METAL_FIRES_ATTKR 10
#define METAL_FIRES_DFNDR 11
#define PRAYER_ATTKR 12
#define PRAYER_DFNDR 13
#define HIGH_PRAYER_ATTKR 14
#define HIGH_PRAYER_DFNDR 15
#define TERROR_ATTKR 16
#define TERROR_DFNDR 17
#define CALL_LIGHTNING_ATTKR 18
#define CALL_LIGHTNING_DFNDR 19
#define COUNTER_MAGIC_ATTKR 20
#define COUNTER_MAGIC_DFNDR 21
#define MASS_INVISIBILITY_ATTKR 22
#define MASS_INVISIBILITY_DFNDR 23
#define ENTANGLE_ATTKR 24
#define ENTANGLE_DFNDR 25
#define MANA_LEAK_ATTKR 26
#define MANA_LEAK_DFNDR 27
#define BLUR_ATTKR 28
#define BLUR_DFNDR 29

struct s_COMBAT_ENCHANTMENT_STATUS
{
    /* 00 */ int8_t Attkr;  // even /  left
    /* 01 */ int8_t Dfndr;  //  odd / right
    /* 02 */
};

// sizeof:  1Eh  30d  (15*2)
struct s_COMBAT_ENCHANTMENTS
{
    /* 00 */  struct s_COMBAT_ENCHANTMENT_STATUS True_Light;
    /* 02 */  struct s_COMBAT_ENCHANTMENT_STATUS Darkness;
    /* 04 */  struct s_COMBAT_ENCHANTMENT_STATUS Warp_Reality;
    /* 06 */  struct s_COMBAT_ENCHANTMENT_STATUS Black_Prayer;
    /* 08 */  struct s_COMBAT_ENCHANTMENT_STATUS Wrack;
    /* 0A */  struct s_COMBAT_ENCHANTMENT_STATUS Metal_Fires;
    /* 0C */  struct s_COMBAT_ENCHANTMENT_STATUS Prayer;
    /* 0E */  struct s_COMBAT_ENCHANTMENT_STATUS High_Prayer;
    /* 10 */  struct s_COMBAT_ENCHANTMENT_STATUS Terror;
    /* 12 */  struct s_COMBAT_ENCHANTMENT_STATUS Call_Lightning;
    /* 14 */  struct s_COMBAT_ENCHANTMENT_STATUS Counter_Magic;
    /* 16 */  struct s_COMBAT_ENCHANTMENT_STATUS Mass_Invisibility;
    /* 18 */  struct s_COMBAT_ENCHANTMENT_STATUS Entangle;
    /* 1A */  struct s_COMBAT_ENCHANTMENT_STATUS Mana_Leak;
    /* 1C */  struct s_COMBAT_ENCHANTMENT_STATUS Blur;
    /* 1E */
};

// CLAUDE  /*
// CLAUDE  spell_data_table[spell_idx].cmbt_ench_idx  (AKA spell_data_table[spell_idx].Param0)
// CLAUDE  MoO2 and manual say "side"
// CLAUDE  CC says "it's reasonable to defer it to whenever ovr111/ovr112 get reviewed, since those are the files that force the flat view."
// CLAUDE  ...
// CLAUDE  "the flat view"
// CLAUDE      e.g.,
// CLAUDE          AITP_Combat_Spell()
// CLAUDE              if(combat_enchantments[*(int16_t *)&spell_data_table[spell_idx].Param0 + Unit_Resist] > 0)
// CLAUDE  
// CLAUDE  */
// CLAUDE  
// CLAUDE  /* WIZARDS.inc:  struc s_COMBAT_ENCHANTMENT_STATUS  (sizeof=0x2) */
// CLAUDE  struct s_COMBAT_ENCHANTMENT_STATUS
// CLAUDE  {
// CLAUDE      /* 00 */  int8_t Attkr;
// CLAUDE      /* 01 */  int8_t Dfndr;
// CLAUDE  };
// CLAUDE  
// CLAUDE  /* WIZARDS.inc:  struc s_COMBAT_ENCHANTMENTS  (sizeof=0x1E) */
// CLAUDE  struct s_COMBAT_ENCHANTMENTS
// CLAUDE  {
// CLAUDE      /* 00 */  struct s_COMBAT_ENCHANTMENT_STATUS True_Light;
// CLAUDE      /* 02 */  struct s_COMBAT_ENCHANTMENT_STATUS Darkness;
// CLAUDE      /* 04 */  struct s_COMBAT_ENCHANTMENT_STATUS Warp_Reality;
// CLAUDE      /* 06 */  struct s_COMBAT_ENCHANTMENT_STATUS Black_Prayer;
// CLAUDE      /* 08 */  struct s_COMBAT_ENCHANTMENT_STATUS Wrack;
// CLAUDE      /* 0A */  struct s_COMBAT_ENCHANTMENT_STATUS Metal_Fires;
// CLAUDE      /* 0C */  struct s_COMBAT_ENCHANTMENT_STATUS Prayer;
// CLAUDE      /* 0E */  struct s_COMBAT_ENCHANTMENT_STATUS High_Prayer;
// CLAUDE      /* 10 */  struct s_COMBAT_ENCHANTMENT_STATUS Terror;
// CLAUDE      /* 12 */  struct s_COMBAT_ENCHANTMENT_STATUS Call_Lightning;
// CLAUDE      /* 14 */  struct s_COMBAT_ENCHANTMENT_STATUS Counter_Magic;
// CLAUDE      /* 16 */  struct s_COMBAT_ENCHANTMENT_STATUS Mass_Invisibility;
// CLAUDE      /* 18 */  struct s_COMBAT_ENCHANTMENT_STATUS Entangle;
// CLAUDE      /* 1A */  struct s_COMBAT_ENCHANTMENT_STATUS Mana_Leak;
// CLAUDE      /* 1C */  struct s_COMBAT_ENCHANTMENT_STATUS Blur;
// CLAUDE  };
// CLAUDE  
// CLAUDE  #define NUM_COMBAT_ENCHANTMENT_SLOTS 30  /* 15 enchantments x { Attkr, Dfndr } */
// CLAUDE  
// CLAUDE  union u_COMBAT_ENCHANTMENTS
// CLAUDE  {
// CLAUDE      struct s_COMBAT_ENCHANTMENTS by_name;       /* effect code:  ->by_name.Terror.Attkr */
// CLAUDE      int8_t slot[NUM_COMBAT_ENCHANTMENT_SLOTS];  /* spell/AI code: ->slot[TERROR_ATTKR]  */
// CLAUDE  };


//  sizeof: 19h  25d
struct s_CTY_ENCH
{
    /* 0x00 */ int8_t Wall_of_Fire;
    /* 0x01 */ int8_t Chaos_Rift;     // ¿ why'd drake178 start the name with an underscore ?
    /* 0x02 */ int8_t Dark_Rituals;
    /* 0x03 */ int8_t Evil_Presence;  // ¿ why'd drake178 start the name with an underscore ?
    /* 0x04 */ int8_t Cursed_Lands;   // ¿ why'd drake178 start the name with an underscore ?
    /* 0x05 */ int8_t Pestilence;     // ¿ why'd drake178 start the name with an underscore ?
    /* 0x06 */ int8_t Cloud_of_Shadow;
    /* 0x07 */ int8_t Famine;
    /* 0x08 */ int8_t Flying_Fortress;
    /* 0x09 */ int8_t Nature_Ward;
    /* 0x0A */ int8_t Sorcery_Ward;
    /* 0x0B */ int8_t Chaos_Ward;
    /* 0x0C */ int8_t Life_Ward;
    /* 0x0D */ int8_t Death_Ward;
    /* 0x0E */ int8_t Natures_Eye;
    /* 0x0F */ int8_t Earth_Gate;
    /* 0x10 */ int8_t Stream_of_Life;
    /* 0x11 */ int8_t Gaias_Blessing;
    /* 0x12 */ int8_t Inspirations;
    /* 0x13 */ int8_t Prosperity;
    /* 0x14 */ int8_t Astral_Gate;
    /* 0x15 */ int8_t Heavenly_Light;
    /* 0x16 */ int8_t Consecration;
    /* 0x17 */ int8_t Wall_of_Darkness;
    /* 0x18 */ int8_t Altar_of_Battle;
    /* 0x19 */
};


// sizeof: 0x1598  5528d  348 paragraphs = 5568 bytes
#pragma pack(push)
#pragma pack(2)
struct s_BATTLEFIELD
{
    /* 0x0000 */  int16_t  terrain_type[COMBAT_GRID_CELL_COUNT];    /* 21 x 22 */
    /* 0x039C */  int8_t   terrain_group[COMBAT_GRID_CELL_COUNT];   /* 1-byte, unsigned */
    /* 0x056A */  int8_t   roads[COMBAT_GRID_CELL_COUNT];
    /* 0x0738 */  int8_t   MoveCost_Ground[COMBAT_GRID_CELL_COUNT];
    /* 0x0906 */  int8_t   MoveCost_Teleport[COMBAT_GRID_CELL_COUNT];
    /* 0x0AD4 */  int8_t   MoveCost_Ground2[COMBAT_GRID_CELL_COUNT];
    /* 0x0CA2 */  int8_t   MoveCost_Sailing[COMBAT_GRID_CELL_COUNT];
    /* 0x0E70 */  int16_t  tree_count;
    /* 0x0E72 */  int16_t  Tree_DrawXs[100];
    /* 0x0F3A */  int16_t  Tree_DrawYs[100];
    /* 0x1002 */  int16_t  Tree_Indices[100];
    /* 0x10CA */  int16_t  rock_count;
    /* 0x10CC */  int16_t  rock_sx_array[100];
    /* 0x1194 */  int16_t  rock_sy_array[100];
    /* 0x125C */  SAMB_ptr rock_pict_segs[100];
    /* 0x1324 */  int8_t   muds[COMBAT_GRID_CELL_COUNT];    /* {F,T} combat map square is 'mud' */
    /* 0x14F2 */  int16_t  center_square_structure; // enum e_CENTRAL_STRUCTURES
    /* 0x14F4 */  int16_t  house_cnt;
    /* 0x14F6 */  int16_t  house_cgxs[16];      // [4][4]
    /* 0x1516 */  int16_t  house_cgys[16];      // [4][4]
    /* 0x1536 */  SAMB_ptr house_pict_segs[16]; // [4][4]  ¿ assigned pict seg from [3][15] house types ?
    /* 0x1556 */  int16_t  walled;              // {F,T} city has stone wall;  ... used to set _ai_battlefield_city_walls |= 0x1;, so must be specifcally 'City Walls'/'Wall of Stone'
    /* 0x1558 */  int16_t  walls[4][4];         /* [4][4] as {4,4,4,4}; state/status {0:none,1:good,2:bad}  spl_Wall_Of_Stone sets 1, spl_Disrupt sets 2 */
    /* 0x1578 */  int16_t  wall_of_fire;        // {F,T}
    /* 0x157A */  int16_t  wall_of_darkness;    // {F,T}
    /* 0x157C */  int16_t  wp;
    /* 0x157E */  int8_t   city_enchantments[NUM_CITY_ENCHANTMENTS];  /* 1-byte, signed */
    /* 0x1597 */  int8_t   Nightshades;  // ~ enchantment, but count, rather than player num bitfield
    /* 0x1598 */
};
#pragma pack(pop)


// ; (sizeof=0x1E)
// sizeof 40, due to union / pointer size ... in-mem, union looks to be 12 bytes
struct s_COMBAT_ENTITY
{
    /* 0x00 */ int16_t draw_x;  /* only used by Combat_Screen_Map_Draw_Entities();  e.g., battle unit figure screen x;  screen x for Draw_Picture_Windowed() */
    /* 0x02 */ int16_t draw_y;  /* only used by Combat_Screen_Map_Draw_Entities();  e.g., battle unit figure screen y;  screen y for Draw_Picture_Windowed() */
    /* 0x04 */ // int16_t IMG_Seg_or_Id;
    union {
        int64_t seg_or_idx;
        int16_t index;
        SAMB_ptr pict_seg;
    };
    /* 0x06 */ int16_t frame_num;
    /* 0x08 */ int16_t draw_y_shift;  /* only used by Combat_Screen_Map_Draw_Entities(), only subtracted, never added;  curiously hard-coded value, per entity type;  screen y offset for Draw_Picture_Windowed() */
    /* 0x0A */ int16_t draw_x_shift;  /* only used by Combat_Screen_Map_Draw_Entities(), only subtracted, never added;  curiously hard-coded value, per entity type;  screen x offset for Draw_Picture_Windowed() */
    /* 0x0C */ int16_t entity_type;  /* enum E_COMBAT_ENTITY_TYPE  ¿ entity type ?  1 is battle unit figure */
    /* 0x0E */ int16_t owner_idx;  /* battle_unit[].controller_idx */
    /* 0x10 */ int16_t niu_figure_set_idx;
    /* 0x12 */ int16_t outline_magic_realm;  /* unit enchantment outline magic realm */
    /* 0x14 */ int16_t Unused_14h;
    /* 0x16 */ int16_t Blood_Frame;
    /* 0x18 */ int16_t gibs;
    /* 0x1A */ uint32_t draw_order_value;
    /* 0x1E */
};


// ; (sizeof=0x10, standard type)
struct RNGD_ANIM
{
    /* 00 */ int16_t Up;
    /* 02 */ int16_t UpRight;
    /* 04 */ int16_t Right;
    /* 06 */ int16_t DnRight;
    /* 08 */ int16_t Down;
    /* 0A */ int16_t DnLeft;
    /* 0C */ int16_t Left;
    /* 0E */ int16_t UpLeft;
    /* 10 */
};


// ; (sizeof=0x12, standard type)
struct ATK_ICONS
{
    /* 00 */ int16_t Melee;
    /* 02 */ int16_t Ranged_Magic;
    /* 04 */ int16_t Mithril;
    /* 06 */ int16_t Magic_Weapon;
    /* 08 */ int16_t Adamantium;
    /* 0A */ int16_t Ranged_Bow;
    /* 0C */ int16_t Ranged_Rock;
    /* 0E */ int16_t Fire_Breath;
    /* 10 */ int16_t Thrown_Attack;
    /* 12 */
};


// ; (sizeof=0x6, standard type)
struct CMOVE_ICONS
{
    /* 00 */ int16_t Ground_Movement;
    /* 02 */ int16_t Flying_Movement;
    /* 04 */ int16_t Water_Movement;
    /* 06 */
};


// ; (sizeof=0x1E, standard type)
struct STAT_ICONS
{
    /* 00 */ int16_t Melee;
    /* 02 */ int16_t Ranged_Magic;
    /* 04 */ int16_t Mithril;
    /* 06 */ int16_t Magic_Weapon;
    /* 08 */ int16_t Adamantium;
    /* 0A */ int16_t Ranged_Bow;
    /* 0C */ int16_t Ranged_Rock;
    /* 0E */ int16_t Fire_Breath;
    /* 10 */ int16_t Thrown_Attack;
    /* 12 */ int16_t Defense;
    /* 14 */ int16_t Hits;
    /* 16 */ int16_t Ground_Movement;
    /* 18 */ int16_t Flying_Movement;
    /* 1A */ int16_t Water_Movement;
    /* 1C */ int16_t Resist;
    /* 1E */
};


// ; (sizeof=0x24, standard type)
struct CSCAN_ICONS
{
    /* 00 */ struct STAT_ICONS Stat;
    /* 1E */ int16_t White_XP;
    /* 20 */ int16_t Gold_XP;
    /* 22 */ int16_t Red_XP;
    /* 24 */
};

// ; (sizeof = 0xC)
// segrax did this? before/after I did struct s_MAGIC_VORTEX?
struct s_CMB_Vortex
{
    /* 00 */ int16_t cgx;
    /* 02 */ int16_t cgy;
    /* 04 */ int16_t Prev_or_Next_X;
    /* 06 */ int16_t Prev_or_Next_Y;
    /* 08 */ int16_t Move_Stage;
    /* 0A */ int16_t owner_idx;
};

// sizeof=0x1E
struct s_CMB_InfoItem
{
    /* 00 */ SAMB_ptr icon_seg;
    /* 02 */ int16_t help_idx;  // enum HELP_TXT
    /* 04 */ char Name[26];
    /* 1E */ 
};


//  ; (sizeof=0xC)
struct s_MAGIC_VORTEX
{
    /* 00 */ int16_t cgx;
    /* 02 */ int16_t cgy;
    /* 04 */ int16_t move_cgx;  // ; prev when stationary, next while moving
    /* 06 */ int16_t move_cgy;  // ; prev when stationary, next while moving
    /* 08 */ int16_t stage;
    /* 0A */ int16_t owner_idx;
    /* 0C */
};






// sizeof: 6Eh  110d
#pragma pack(push)
#pragma pack(2)
struct s_BATTLE_UNIT
{
    /* 0x00 */  int8_t   melee;             /* ~ "melee attack strength" */
    /* 0x01 */  int8_t   ranged;            /* ~ "ranged attack strength" */
    /* 0x02 */  int8_t   ranged_type;       /* 1-byte, signed */  /* ~ "ranged attack type" */
    /* 0x03 */  int8_t   ammo;
    /* 0x04 */  int8_t   tohit;
    /* 0x05 */  int8_t   defense;
    /* 0x06 */  int8_t   resist;
    /* 0x07 */  int8_t   movement_points;   /* ¿ moves2 ? */
    /* 0x08 */  int16_t  cost;
    /* 0x0A */  int8_t   upkeep;
    /* 0x0B */  int8_t   race;              /* 1-byte, signed;  enum e_RACE_TYPE;  -16 == Magic/Spellbook Realm */
    /* 0x0C */  /* int8_t Unused_0Ch; */                          // reqd_bldg_1 & hero_portrait
    union {
                int8_t   reqd_bldg_1;
                int8_t   hero_portrait_idx;
    };
    /* 0x0D */  /* int8_t figure_cnt; */                         // reqd_bldg_2 & hero_type
    union {
                int8_t   figure_cnt;   /* DEDU  range of values ? */
                int8_t   reqd_bldg_2;
                int8_t   hero_type;
    };
    /* 0x0E */
    union {
        int64_t seg_or_idx;
        int16_t bufpi;  /* used to index picture cache */
        SAMB_ptr pict_seg;
    };
    /* 0x10 */  int8_t   hits;                  /* Hit-Points ¿ Per Figure ? */
    /* 0x11 */  int8_t   scout_range;
    /* 0x12 */  int8_t   carry_capacity;
    /* 0x13 */  int8_t   figure_max;
    /* 0x14 */  int8_t   Construction;
    /* 0x15 */  int8_t   Spec_Att_Attrib;       // -abs() is resistance_modifier
    /* 0x16 */  uint16_t Move_Flags;            // ; enum MOVEFLAGS
    /* 0x17 */  /* 2-byte alignment padding */
    /* 0x18 */  int16_t  Attribs_1;             // any reason these arent a uint32?  ; enum ATTRIB_1
    /* 0x1A */  int8_t   Attribs_2;             // any reason these arent a uint32?  ¿ ?
    /* 0x1B */  int8_t   Unused_1Bh;            // any reason these arent a uint32?  ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 0x1C */  uint16_t Abilities;             // ; enum ABL_FLAGS  ~ 'Unit Abilities'
    /* 0x1E */  uint16_t attack_attributes;     /* defs Att_  enum e_ATTACK_FLAGS  AKA ATK_FLAGS */
    /* 0x20 */  int16_t  Sound;
    /* 0x22 */  int16_t  combat_effects;        /* enum e_BATTLE_UNIT_EFFECT */
    /* 0x24 */  int8_t   melee_tohit;
    /* 0x25 */  int8_t   ranged_tohit;
    /* 0x26 */  int8_t   toblock;
    /* 0x27 */  int8_t   Weapon_Plus1;          /* ~ Magic Weapon ... `if(!= 0)` */
    /* 0x28 */  uint16_t melee_attack_attributes;   /* defs Att_  enum e_ATTACK_FLAGS  AKA ATK_FLAGS */
    /* 0x2A */  uint16_t ranged_attack_attributes;  /* defs Att_  enum e_ATTACK_FLAGS  AKA ATK_FLAGS */
    /* 0x2C */  uint32_t item_enchantments;     // Item Powers, as Unit Enchantments  enum e_UNIT_ENCHANTMENTS ... macro UE_...
    /* 0x30 */  int16_t  unit_idx;
    /* 0x32 */  int8_t   Extra_Hits;
    /* 0x33 */  int8_t   Web_HP;
    /* 0x34 */  int8_t   status;                /* enum e_BATTLE_UNIT_STATUS */
    /* 0x35 */  int8_t   controller_idx;        // Player Number of the Controlling Wizard  (was owner_idx, but conflicts with _UNITS[unit_idx].owner_idx)
    /* 0x36 */  uint8_t  damage[3];             // Regular_Dmg, Undeath_Dmg, Irreversible_Dmg
    /* 0x39 */  int8_t   front_figure_damage;
    /* 0x3A */  uint32_t enchantments;          // Unit Enchantments
    /* 0x3E */  int8_t   Suppression;
    /* 0x3F */  uint8_t  mana_max;
    /* 0x40 */  uint8_t  mana;                  /* 1-byte, unsigned */
    /* 0x41 */  int8_t   item_charges;
    /* 0x42 */  int8_t   Poison_Strength;
    /* 0x43 */  int8_t   target_battle_unit_idx;    /* 1-byte, signed */
    /* 0x44 */  int16_t  cgx;                   /* combat grid x coordinate */
    /* 0x46 */  int16_t  cgy;                   /* combat grid y coordinate */
    /* 0x48 */  int16_t  target_cgx;            /* set by Deploy_Battle_Units()  ¿ used for facing direction ? */
    /* 0x4A */  int16_t  target_cgy;            /* set by Deploy_Battle_Units()  ¿ used for facing direction ? */
    /* 0x4C */  int16_t  move_anim_ctr;         /* ¿ ~ MoO2 ship_frame ?  movement animation counter/increment  {0,1,2,3,4,5,6,7} */
    /* 0x4E */  int16_t  Atk_FigLoss;
    /* 0x50 */  int16_t  outline_magic_realm;   /* enchantment_magic_realm ... enum e_MAGIC_REALM{} ... index into enchantment_outline_colors[], used for unit outline color */
    /* 0x52 */  int16_t  mid_move;              /* {F,T} unit is mid-move; selects walk vs idle animation frame; set to ST_FALSE in Switch_Active_Battle_Unit() */
    /* 0x54 */  int16_t  action;                /* enum e_BATTLE_UNIT_ACTION */
    /* 0x56 */  int8_t   Confusion_State;       /* {0: , 1: , 2: }  ~== stand around and do nothing while looking foolish and confused, move randomly, attack allies, attack enemies for combat winner, 2 means it currently belong to the other player/opponent */
    /* 0x57 */  /* 2-byte alignment padding */
    /* 0x58 */  int16_t  gibs;
    /* 0x5A */  int16_t  Unknown_5A;
    /* 0x5C */  int16_t  animate_idle;
    /* 0x5E */  int16_t  Melee_Anim;            /* {0,1,2}; not just {F,T}; set in CMB_MeleeAnim() */
    /* 0x60 */  int16_t  figure_effect;         /* enum e_BATTLE_UNIT_FIGURE_EFFECT;  passed to Combat_Figure_Effect() for BU figure bitmap composition */
    /* 0x62 */  int16_t  animate_move_as_idle;  /* IIF flight animation ... if((battle_units[battle_unit_idx].Attribs_1 & USA_FLYING) != 0) */
    /* 0x64 */  int8_t   Gold_Melee;
    /* 0x65 */  int8_t   Gold_Ranged;
    /* 0x66 */  int8_t   Gold_Defense;
    /* 0x67 */  int8_t   Gold_Resist;
    /* 0x68 */  int8_t   Gold_Hits;
    /* 0x69 */  int8_t   Grey_Melee;
    /* 0x6A */  int8_t   Grey_Ranged;
    /* 0x6B */  int8_t   Grey_Defense;
    /* 0x6C */  int8_t   Grey_Resist;
    /* 0x6D */  int8_t   Grey_Hits;
    /* 0x6E */
};
#pragma pack(pop)


#pragma pack(push)
#pragma pack(2)
struct s_CMBT_DATA
{
    /* 0x00 */  int8_t melee;
    /* 0x01 */  int8_t ranged;
    /* 0x02 */  int8_t ranged_type;
    /* 0x03 */  int8_t ammo;
    /* 0x04 */  int8_t tohit;
    /* 0x05 */  int8_t defense;
    /* 0x06 */  int8_t resist;
    /* 0x07 */  int8_t movement_points;
    /* 0x08 */  int8_t cost;
    /* 0x0A */  int8_t upkeep;
    /* 0x0B */  int8_t race;                                // ; enum Race_Code
    /* 0x0C */  int8_t Unused_0Ch;
    /* 0x0D */  int8_t figure_cnt;
    /* 0x0E */  int16_t bufpi;
    /* 0x10 */  int8_t hits;
    /* 0x11 */  int8_t scout_range;
    /* 0x12 */  int8_t carry_capacity;
    /* 0x13 */  int8_t figure_max;
    /* 0x14 */  int8_t Construction;
    /* 0x15 */  int8_t Spec_Att_Attrib;
    /* 0x16 */  int8_t Move_Flags;                          // ; enum MOVEFLAGS
    /* 0x17 */  int8_t Unused_17h;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 0x18 */  int16_t Attribs_1;                          // ; enum ATTRIB_1
    /* 0x1A */  int8_t Attribs_2;
    /* 0x1B */  int8_t Unused_1Bh;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 0x1C */  int16_t Abilities;                          // ; enum ABL_FLAGS
    /* 0x1E */  int16_t Attack_Flags;                       // ; enum ATK_FLAGS
    /* 0x20 */  int16_t Sound;

};
#pragma pack(pop)





// WZD dseg:7080
extern int16_t frame_anim_cycle;

// WZD dseg:708A
extern int16_t cmbt_cell_effect_frame;

// WZD dseg:708E
extern int16_t cmbt_cell_effect_active;

// WZD dseg:7116
extern int16_t ai_human_hostility;

// WZD dseg:C418
extern int16_t _combat_city_damage;

// WZD dseg:9294
// drake178: RP_AI_UnsetRealmVar1
extern int16_t g_ai_combat_unset_realm_flags;

// WZD dseg:9296
// drake178: RP_AI_UnsetRealmVar2
extern int16_t g_ai_combat_unset_realm_flags_2;

// WZD dseg:9298
// drake178: AI_DEFR_UnitRealms
extern int16_t g_ai_combat_defender_unit_realms[5];

// WZD dseg:92A2
// drake178: AI_ATKR_UnitRealms
extern int16_t g_ai_combat_attacker_unit_realms[5];

// WZD dseg:92AC
// drake178: AI_DEFR_RealmFlags
extern int16_t g_ai_combat_defender_realm_flags;

// WZD dseg:92AE
// drake178: AI_ATKR_RealmFlags
extern int16_t g_ai_combat_attacker_realm_flags;

// WZD dseg:C41A
extern SAMB_ptr sound_silent_seg;
extern uint32_t sound_silent_seg_size;  // DNE in Dasm

// WZD dseg:C432
extern int16_t _auto_combat_flag;

/* CLAUDE: test-support -- forces _auto_combat_flag back on after Combat_Screen()'s entry reset; see Combat.c. */
extern int16_t g_cmbt_force_auto_combat;

// WZD dseg:C47C
extern int16_t _combat_wp;

// WZD dseg:C47E
extern int16_t _combat_wy;

// WZD dseg:C480
extern int16_t _combat_wx;


// WZD dseg:C520
extern int16_t _combat_structure;

// WZD dseg:C522
extern struct s_mouse_list * _combat_mouse_grid;

// WZD dseg:C524
extern int8_t * g_combat_grid_action_map[COMBAT_GRID_HEIGHT];

// WZD dseg:C582
extern int16_t _active_battle_unit;

// WZD dseg:C584
extern int16_t _combat_defender_player;

// WZD dseg:C586
extern int16_t _combat_attacker_player;

// WZD dseg:C588
extern int16_t _combat_total_unit_count;



// WZD dseg:C7AA                                                 ¿ BEGIN:  ovr103 ?

// WZD dseg:C7B2
extern SAMB_ptr battle_unit_scratch_seg;

// WZD dseg:C7C2
extern int16_t _combat_node_type;

// WZD dseg:C7E6                                                 ¿ END:  ovr103 ?



// WZD dseg:C8A8
// WZD dseg:C8AA
/*
¿ only used by/for TILE_DispelMagic__WIP() ?
*/
extern int16_t * CMB_NearDispel_UCs;
extern int16_t * CMB_NearDispel_UEs;

// WZD dseg:C972
extern int16_t _combat_winner;

/* CLAUDE: test-support -- exposes the per-battle turn counter (defined Combat.c) for HeMoM's combat results dump. */
extern int16_t _combat_turn;

// WZD dseg:C974
extern int16_t destroy_combat_city;

// WZD dseg:C976
extern int16_t CMB_Gold_Reward;

// WZD dseg:C978
extern int16_t CMB_Buildings_Lost;

// WZD dseg:C97A
extern int16_t * CMB_LostBuildings;

// WZD dseg:C97C
extern int16_t CMB_Population_Lost;

// WZD dseg:C97E
extern int16_t combat_results_scroll_message;

// WZD dseg:CF9A
extern SAMB_ptr EmmHndl_TILEXXX;

// WZD dseg:CF9C
extern SAMB_ptr EmmHndl_FIGUREX;

// WZD dseg:CFBE
extern SAMB_ptr cmbtcity_blood_segs[5];

// WZD dseg:CFCA
extern SAMB_ptr combat_curse_entity_seg[8];

// WZD dseg:CFEE
extern SAMB_ptr cmbtcity_river_segs[12];

// WZD dseg:D006
extern SAMB_ptr chriver_river_segs[12];

// WZD dseg:D01E
extern SAMB_ptr cmbtcity_roadgrid_segs[28];

// WZD dseg:D056
extern SAMB_ptr cmbtcity_ocean_segs[4];

// WZD dseg:D05E
extern SAMB_ptr chriver_chaoswat_segs[4];

// WZD dseg:D066
extern SAMB_ptr cmbtcity_cmbcloud_segs[4];

// WZD dseg:D06E
extern SAMB_ptr cmbtcity_flotisle_seg;

// WZD dseg:D070
extern SAMB_ptr cmbtcity_mud_seg;

// WZD dseg:D072
extern SAMB_ptr IMG_CMB_SorcNode;

// WZD dseg:D074
extern SAMB_ptr IMG_CMB_NatNode;

// WZD dseg:D076
// NEWCODE  extern SAMB_ptr chriver_volca_segs[8];
extern SAMB_ptr chriver_volca_segs[9];

// WZD dseg:D088
extern SAMB_ptr IMG_CMB_DarkWall[14];

// WZD dseg:D0A4
extern SAMB_ptr IMG_CMB_FireWall[14];

// WZD dseg:D0C0
extern SAMB_ptr IMG_CMB_StoneWalls[3][12];

// WZD dseg:D108
extern SAMB_ptr IMG_CMB_DirtTile;
// WZD dseg:D10A
extern SAMB_ptr IMG_CMB_RoadGrid;
// WZD dseg:D10C
extern SAMB_ptr IMG_CMB_Fort;
// WZD dseg:D10E
extern SAMB_ptr IMG_CMB_Ruins;
// WZD dseg:D110
extern SAMB_ptr IMG_CMB_Temple;
// WZD dseg:D112
extern SAMB_ptr IMG_CMB_Dungeon;
// WZD dseg:D114
extern SAMB_ptr IMG_CMB_Tower;
// WZD dseg:D116
extern SAMB_ptr IMG_CMB_Cave;
// WZD dseg:D118
extern SAMB_ptr IMG_CMB_Outpost;
// WZD dseg:D11A
extern SAMB_ptr IMG_CMB_Fortress;

// WZD dseg:D11C
extern SAMB_ptr _combat_house_picts_segs[15];

// WZD dseg:D13A
extern SAMB_ptr cmbt_cell_effect_seg;

// WZD dseg:D13C
extern SAMB_ptr magic_vortex_seg;

// WZD dseg:D13E
extern int16_t * _cmbt_mvpth_y;

// WZD dseg:D140
extern int16_t * _cmbt_mvpth_x;

// WZD dseg:D142
extern int16_t CMB_Path_Length;

// WZD dseg:D144
extern int16_t * _cmbt_path_data;
union Combat_Routing_Data {
    int16_t * came_from_idx;  /* Used by Path_Find  */  /* The pred(v) tree for Path_Find */
    int16_t * is_reachable;   /* Used by Path_Valid */  /* The boolean overlay for Path_Valid */
};
extern union Combat_Routing_Data _cmbt_path_route;

// WZD dseg:D146
extern uint8_t * _cmbt_mvpth_c;

// WZD dseg:D148
extern uint8_t * _cmbt_movepath_cost_map;

// WZD dseg:D14A
// WZD dseg:D14C
extern int16_t cmbt_cell_effect_cgy;
extern int16_t cmbt_cell_effect_cgx;

// WZD dseg:D14E
extern struct s_MAGIC_VORTEX * _vortexes;

// WZD dseg:D152
extern int16_t _vortex_count;

// WZD dseg:D154
// Y U EXPOSE ME !?!  extern SAMB_ptr _missiles;

// WZD dseg:D15A
extern struct s_COMBAT_ENTITY * combat_grid_entities;

// WZD dseg:D188
extern SAMB_ptr frame_active_seg;

// WZD dseg:D18A
extern SAMB_ptr frame_scanned_seg;

// WZD dseg:D18C
// extern RNGD_ANIM * CMB_RangedAtx_GFX[15];
extern SAMB_ptr CMB_RangedAtx_GFX[15][8];

// WZD dseg:D27C
extern SAMB_ptr CMB_Damage_GFX;





#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr090
*/

// WZD s90p01
int16_t Combat_Screen(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[]);



/*
    WIZARDS.EXE  ovr091
*/

// WZD s91p01
void NIU_Battle_Unit_Load_Figure_Pictures(int16_t battle_unit_idx);

// WZD s91p02
void Begin_Combat_Turn(void);

// WZD s91p03
void Set_Movement_Cost_Map(int16_t battle_unit_idx);

// WZD s91p04
void Move_Battle_Unit(int16_t battle_unit_idx, int16_t target_cgx, int16_t target_cgy);

// WZD s91p05
void Switch_Active_Battle_Unit(int16_t battle_unit_idx);

// WZD s91p06
void Battle_Unit_Action(int16_t _battle_unit_idx, int16_t cgx, int16_t cgy);

// WZD s91p07
void Assign_Combat_Grids(void);

// WZD s91p08
int16_t Target_Is_Visible(int16_t battle_unit_idx);

// WZD s91p09
void Add_City_Damage_From_Battle_Units_Within(void);

// WZD s91p10
int16_t Battle_Unit_Instant_Movement_Mode(int16_t battle_unit_idx);



/*
    WIZARDS.EXE  ovr096
*/

// WZD s96p01
int16_t Combat(int16_t attacker_player_idx, int16_t defender_player_idx, int16_t troop_count, int16_t troops[]);

// WZD s96p02
void Lair_Make_Guardians(int16_t lair_idx);

// WZD s96p03
int16_t Lair_Combat_Do(int16_t lair_idx, int16_t player_idx);

// WZD s96p04
// UU_IDK_CMB_s7DE08()

// WZD s96p05
void Update_Defender_Hostility(int16_t attacker_player_idx, int16_t defender_player_idx);

// WZD s96p06
// UU_IDK_Main_Screen_Draw()




/*
    WIZARDS.EXE  ovr098
*/

// WZD o98p01
void Update_Combat_Enchantments_Icon_And_Help(void);

// WZD o98p02
int16_t Battle_Unit_Has_Spell_Ability(int16_t battle_unit_idx);

// WZD o98p03
// NIU  int16_t NIU_Who_Has_More_Leadership(void);

// WZD o98p04
void Auto_Cast_Spell_And_Do_Combat_Turn(int16_t player_idx);

// WZD o98p05
void Combat_Next_Turn(void);

// WZD o98p06
void Retreat_From_Combat(int16_t player_idx);

// WZD o98p07
int16_t Player_City_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

// WZD o98p08
int16_t Process_Retreating_Units(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

// WZD o98p09
int16_t Unit_Try_To_Move(int16_t wx, int16_t wy, int16_t wp, int16_t unit_idx, int16_t troop_count);

// WZD o98p10
void Assign_Mouse_Images(void);

// WZD o98p11
int16_t Ranged_Mouse_Image(int16_t src_battle_unit_idx, int16_t dst_battle_unit_idx);

// WZD o98p12
void Add_Combat_Enchantment_Fields(void);

// WZD o98p13
int16_t Battle_Unit_Movement_Mode(int16_t battle_unit_idx);

// WZD o98p14
// static void Prepare_Battle_Unit_Summons(int16_t player_idx, int16_t unit_idx, int16_t cgx, int16_t cgy);

// WZD o98p15
void Prepare_Battle_Unit(int16_t battle_unit_idx, int16_t player_idx, int16_t unit_idx, int16_t cgx, int16_t cgy);

// WZD o98p16
int16_t Prepare_All_Battle_Units(int16_t troop_count, int16_t troops[]);

// WZD o98p17
int16_t Battle_Unit_Is_Airborne(int16_t battle_unit_idx);

// WZD o98p18
int16_t Battle_Unit_Has_Flight(int16_t battle_unit_idx);

// WZD o98p19
int16_t Check_Attack_Melee(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx);

// WZD o98p20
SAMB_ptr Get_Battle_Unit_Move_Sound_Buffer(int16_t battle_unit_idx, /* HACK */ uint32_t * sound_buffer_size);

// WZD o98p21
void Turn_Off_Auto_Combat(void);

// WZD o98p22
void Build_Flee_Loss_Message(int16_t troop_count, int16_t troop_list[]);



/*
    WIZARDS.EXE  ovr099
*/

// WZD o99p01
void Combat_Screen_Draw(void);

// WZD o99p02
void Draw_Active_Unit_Window(void);

// WZD o99p03
void Tactical_Combat_Draw_Buttons(void);

// WZD o99p04
void Draw_Spell_Information_Window(void);

// WZD o99p05
void Draw_Active_Unit_Stats_And_Icons(void);

// WZD o99p06
void Combat_Grid_Entities(void);

// WZD o99p07
void Draw_Active_Unit_Damage_Bar(int16_t battle_unit_idx, int16_t x, int16_t y);



/*
    WIZARDS.EXE  ovr103
*/

// WZD s103p01
void Combat_Information_Window(void);

// WZD s103p02
void Combat_Information_Window_Draw(void);

// WZD s103p03
void Combat_Info_Effects(void);

// WZD s103p04
void Combat_Info_Effects_Base(void);

// WZD s103p05
int16_t Combat_Info_Effects_Count(void);

// WZD s103p06
void Combat_Node_Type(void);

// WZD s103p07
void Battle_Unit_Compose_Bitmap(int16_t battle_unit_idx);

// WZD s103p08
void Next_Battle_Unit(int16_t player_idx);

// WZD s103p09
int16_t Next_Battle_Unit_Nearest_Available(int16_t player_idx);

// WZD s103p10
void Combat_Cast_Spell_Error(int16_t type);

// WZD s103p11
void Move_Confused(int16_t battle_unit_idx);

// WZD s103p12
void Combat_Screen_Load_Resources(void);

// WZD s103p13
void Allocate_Combat_Near_Buffers(void);

// WZD s103p14
void Combat_Cast_Spell_With_Caster(int16_t caster_id);



/*
    WIZARDS.EXE  ovr105
*/

// WZD o105p01
int16_t Check_For_Winner(void);

// WZD o105p02
void Combat_Figure_Banner_Color(int16_t player_idx);

// WZD o105p03
void Combat_Figure_Active_Red_Outline(int16_t battle_unit_idx);

// WZD o105p04
void Combat_Figure_Effect(int16_t effect);

// WZD o105p05
void Combat_Unit_Enchantment_Outline_Set(int16_t battle_unit_idx);

// WZD o105p06
void Combat_Unit_Enchantment_Outline_Draw(int16_t enchantment_magic_realm);

// WZD o105p07
void Update_Sees_Illusions(void);

// WZD o105p08
 void Battle_Unit_Set_Invisibility_Effect(int16_t battle_unit_idx);

// WZD o105p09
int16_t Battle_Unit_Curse_Effects(int16_t battle_unit_idx);

// WZD o105p10
void Battle_Unit_Set_Animation_Flags(int16_t battle_unit_idx);

// WZD o105p11
int16_t Battle_Unit_Melee_Attack_Icon(int16_t battle_unit_idx);

// WZD o105p12
int16_t Battle_Unit_Ranged_Attack_Icon(int16_t battle_unit_idx);

// WZD o105p13
void Draw_Combat_Unit_Display(void);



/*
    WIZARDS.EXE  ovr110
*/

// WZD o110p01
void Strategic_Combat_Allocate(void);

// WZD o110p02
int16_t Strategic_Combat(int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[]);

// WZD o110p03
void Battle_Unit_Heal(int16_t battle_unit_idx, int16_t Healing, int16_t TempHits);

// WZD o110p04
int16_t Get_Effective_Hits(int16_t hits, int16_t defense);

// WZD o110p05
int16_t Get_Effective_Ranged_Strength(int16_t ranged, int16_t figures, int16_t attack_flags);

// WZD o110p06
int16_t Get_Effective_Melee_Strength(int16_t melee, int16_t thrown, int16_t figures, int16_t attack_flags, int16_t ranged_type);



/*
    WIZARDS.EXE  ovr111
*/

// WZD o111p01
int16_t AITP_Combat_Spell(int16_t spell_idx, int16_t player_idx, int16_t * target_wx, int16_t * target_wy);

// WZD 111p02
int16_t AITP_DarknessLight(int16_t Spell_Index);

// WZD 111p03
int16_t AITP_Healing(int16_t player_idx);

// WZD 111p04
int16_t AITP_WarpWood(int16_t player_idx);

// WZD 111p05
int16_t AITP_WarpCreature(int16_t player_idx);

// WZD 111p06
// UU15_AITP_Disintegrate()

// WZD 111p07
int16_t AITP_DispelMagic(int16_t player_idx);

// WZD 111p08
void Cast_Spell_On_Battle_Unit(int16_t spell_idx, int16_t target_idx, int16_t caster_idx, int16_t target_cgx, int16_t target_cgy, int16_t Mana, int16_t Anims, int16_t UU1, int16_t UU2);

// WZD 111p09
int16_t AITP_HolyWord(int16_t player_idx);



/*
    WIZARDS.EXE  ovr113
*/

// WZD o113p01
void Combat_Screen_Assign_Mouse_Images(void);

// WZD o113p02
// drake178: CMB_DrawTargetScreen()
void Combat_Spell_Target_Screen_Draw(void);

// WZD o113p03
int16_t Combat_Spell_Target_Screen(int16_t spell_idx, int16_t * target_cgx, int16_t * target_cgy);

// WZD o113p04
void Ranged_Animation(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t Target_Damage_Sum, int16_t cgx, int16_t cgy);

// WZD o113p05
void Melee_Animation(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t attacker_damage, int16_t defender_damage, int16_t cgx, int16_t cgy);

// WZD o113p06
void Compute_Battle_Unit_Damage_From_Spell(uint16_t spell_idx, uint16_t battle_unit_idx, int16_t damage_types[], int16_t attack_override_flag);

// WZD o113p07
void Battle_Unit_Commit_Damage(int16_t battle_unit_idx, int16_t damage_types[]);

// WZD o113p08
int16_t Check_Attack_Ranged(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx);

// WZD o113p09
void Deploy_Battle_Units(int16_t player_idx);

// WZD o113p10
void Summon_Demon(int16_t caster_idx);

// WZD o113p11
int16_t Check_Attack_Melee_City_Wall(int16_t src_battle_unit_idx, int16_t dst_battle_unit_idx);

// WZD o113p12
void Apply_Mana_Leak(void);

// WZD o113p13
void GUI_DrawNearMessage(void);

// WZD o113p14
void Combat_Cast_Spell_Message(int16_t caster_idx, int16_t spell_idx);

// WZD o113p15
int16_t Battle_Unit_Pict_Open(void);

// WZD o113p16
int16_t Eliminated_Opponent(void);



/*
    WIZARDS.EXE  ovr122
*/

// WZD o122p01
int16_t Combat_Roll_Damage_Dealt(int16_t attack_strength, int16_t to_hit);

// WZD o122p02
int16_t Combat_Roll_Damage_Blocked(int16_t defense, int16_t to_block);

// WZD o122p03
int16_t Combat_Resistance_Check(struct s_BATTLE_UNIT battle_unit, int16_t resistance_modifier, int16_t magic_realm);

// WZD o122p04
int16_t Combat_Effective_Resistance(struct s_BATTLE_UNIT battle_unit, int16_t magic_realm);

// WZD o122p05
// static int16_t Battle_Unit_Attack_Immunities(int16_t battle_unit_idx, int16_t attack_mode);

// WZD o122p06
void Battle_Unit_Attack_Target(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t defender_damage_types[], int16_t attacker_damage_types[], int16_t ranged_attack_flag, int16_t resolve_for_real);

// WZD o122p07
void Battle_Unit_Process_Attack(int16_t attacker_battle_unit_idx, int16_t figure_count, int16_t defender_battle_unit_idx, int16_t attack_mode, int16_t damage_types[], int16_t is_counter_attack, int16_t resolve_for_real);

// WZD o122p08
int16_t Battle_Unit_Defense_Special(int16_t battle_unit_idx, int16_t attack_type, int16_t attack_immunities, int16_t attack_attributes, int16_t magic_realm);

// WZD o122p09
int16_t Battle_Unit_Attack_Magic_Realm(int16_t attack_type, int16_t battle_unit_idx);

// WZD o122p10
void Calc_Battlefield_Bonuses(int16_t combat_structure);

// WZD o122p11
void Battle_Unit_Attack(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t cgx, int16_t cgy);

// WZD o122p12
int16_t Range_To_Battle_Unit(int16_t BU_1, int16_t BU_2);

// WZD o122p13
int16_t Target_Unit_Value(int16_t attacker_idx, int16_t target_idx, int16_t has_ranged_attack);

// WZD o122p14
int16_t Battle_Unit_Has_Ranged_Attack(int16_t battle_unit_idx);

// WZD o122p15
int16_t Apply_Fear_Attack(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx);




/*
    WIZARDS.EXE  ovr123
*/

// WZD o123p01
void End_Of_Combat(int16_t player_idx, int16_t * item_count, int16_t item_list[], int16_t MsgType);

// WZD o123p02
int16_t Find_Undead_Creator_Type(int16_t player_idx);

// WZD o123p03
void Combat_Results_Scroll(void);

// WZD o123p04
void Combat_Results_Scroll_Draw(void);

// WZD o123p05
int16_t Combat_Results_Scroll_Text(void);

// WZD o123p06
int16_t City_Gold(int16_t city_idx);

// WZD o123p07
int16_t Rampage_Combat_City(void);



/*
    WIZARDS.EXE  ovr124
*/

// WZD o124p01
int16_t Total_Ranged_Attack_Strength(int16_t player_idx);

// WZD o124p02
void Update_Move_Map_City_Area_Restrictions(int16_t battle_unit_idx);

// WZD o124p03
void AI_RestrictToCity__WIP(void);

// WZD o124p04
int16_t Combat_Structure(int16_t wx, int16_t wy, int16_t wp, int16_t set_city_flag);

// WZD o124p05
int16_t Undeployable_Battle_Units_On_Water(int16_t player_idx);

// WZD o124p06
int16_t Get_Player_Mode(int16_t player_idx);

// WZD o124p07
SAMB_ptr Reload_Battle_Unit_Move_Sound(int16_t battle_unit_idx, /* HACK */ uint32_t * sound_seg_size);

// WZD o124p08
SAMB_ptr Reload_Melee_Sound(int16_t battle_unit_idx, /* HACK */ uint32_t * sound_seg_size);

// WZD o124p09
void Combat_Load_Spell_Sound_Effect(int16_t spell_idx);

// WZD o124p10
void Check_Wall_Of_Fire_Attack(int16_t battle_unit_idx);

// WZD o124p11
void Set_Gibs(int16_t battle_unit_idx, int16_t Damage);

// WZD o124p12
void Clear_Gibs(int16_t battle_unit_idx);

// WZD o124p13
int16_t Battle_Unit_Is_Within_City(int16_t battle_unit_idx);

// WZD o124p14
int16_t Combat_Grid_Cell_Has_City_Wall(int16_t cgc2, int16_t cgc1);

// WZD o124p15
int16_t Battle_Unit_Is_Summoned_Creature(int16_t battle_unit_idx);

// WZD o124p16
int16_t Battle_Unit_Moves2(int16_t battle_unit_idx);

// WZD o124p17
void Combat_City_Capture(int16_t troop_count, int16_t * troops);

// WZD o124p18
int16_t Retreat_Check(int16_t player_idx);

// WZD o124p19
int16_t Raze_Check(int16_t player_idx, int16_t city_idx);

// WZD o124p20
int16_t Raze_City_Prompt(char * message);

// WZD o124p21
void Raze_City_Prompt_Draw(void);



/*
    WIZARDS.EXE  ovr139
*/

// WZD ovr139p01
int16_t AI_SelectCmbtSpell(int16_t caster_id);

// WZD ovr139p02
int16_t AI_EvaluateCmbtSpell(int16_t player_idx, int16_t spell_idx, int16_t threat_idx);

// WZD ovr139p03
int16_t Effective_Battle_Unit_Strength(int16_t battle_unit_idx);

// WZD ovr139p04
void AI_SetCombatRealms(void);

// WZD ovr139p05
void AI_UnitThreatRealms(int16_t player_idx, int16_t * realm_array);

// WZD ovr139p06
void AI_CombatSpellList(int16_t caster_id, int16_t * spell_list, int16_t range_mod);



/*
    WIZARDS.EXE  ovr153
*/

// WZD ovr153p01
void NX_IDK_CombatInit_Tactical(int16_t wx, int16_t wy, int16_t wp);

// WZD ovr153p02
void Combat_Screen_Map_Draw(void);

// WZD ovr153p03
void Combat_Screen_Map_Draw_Entities(void);

// WZD ovr153p04
void NIU_Gibs_Frames(void);

// WZD ovr153p05
void Spawn_Tree_Entities(void);

// WZD ovr153p06
void Spawn_Rock_Entities(void);

// WZD ovr153p07
void Spawn_Structure_Entities(void);

// WZD ovr153p08
void Spawn_Stone_Wall_Entities(int16_t flag);

// WZD ovr153p09
void Spawn_Dark_Wall_Entities(int16_t flag);

// WZD ovr153p10
void Spawn_Fire_Wall_Entities(int16_t flag);

// WZD ovr153p11
void Spawn_Vortex_Entities(void);

// WZD ovr153p12
void Spawn_Figure_Entity(int64_t bufpi, int16_t cgx, int16_t cgy, int16_t target_cgx, int16_t target_cgy, int16_t move_anim_ctr, int16_t current_figure, int16_t figure_count, int16_t controller_idx, int16_t outline_magic_realm, int16_t gibs, int16_t frame_num, int16_t figures_lost, int16_t gibs_bank);

// WZD ovr153p13
void Spawn_Curse_Entity(int16_t cgx, int16_t cgy, int16_t target_cgx, int16_t target_cgy, int16_t move_anim_ctr, int16_t combat_curse_entity_idx);

// WZD ovr153p14
void Clear_Combat_Grid_Entities(void);

// WZD ovr153p15
void Combat_Figure_Compose(int16_t bufpi, int16_t figure_set_idx, int16_t player_idx, int16_t enchantment_magic_realm, int16_t frame_num);

// WZD ovr153p16
void Spawn_Missile_Entities(void);

// WZD ovr153p17
void Battle_Unit_Figure_Position(int16_t figure_count, int16_t current_figure, int16_t * figure_x, int16_t * figure_y);

// WZD ovr153p18
void Combat_Grid_Entity_Create(int16_t draw_x, int16_t draw_y, int64_t seg_or_idx, int16_t draw_x_shift, int16_t draw_y_shift, int16_t Frame, int16_t entity_type, int16_t controller_idx, int16_t niu_figure_set_idx, int16_t outline_magic_realm, int16_t gibs, int16_t UU_14h, int16_t BldFrm);

// WZD ovr153p19
void Set_Entity_Draw_Order(void);

// WZD ovr153p20
void Claim_EMS_Page_For_Figure_Set(int16_t figure_index);

// WZD ovr153p21
void Map_Tile_EMS_Page_As_Sandbox(void);

// WZD ovr153p22
void o153p22_empty_function(void);

// WZD ovr153p23
void Combat_Screen_Map_Compose_Figures(void);

// WZD ovr153p24
void o153p24_empty_function(void);



/*
    WIZARDS.EXE  ovr154
*/

// WZD ovr154p01
void Build_Battlefield(int16_t wx, int16_t wy, int16_t wp);

// WZD ovr154p02
void Generate_Combat_Map(
    int16_t location_type, 
    int16_t house_type, 
    int16_t roads_array[], 
    int16_t enchanted_roads_flag, 
    int16_t cts, 
    int16_t rivers_array[], 
    int16_t flying_fortress_flag, 
    int16_t attacker_floating_island_flag, 
    int16_t defender_floating_island_flag, 
    int16_t city_walls, 
    int16_t city_population, 
    int16_t magic_walls
);

// WZD ovr154p03
void Scatter_Tree_Scenery(int16_t TerrType, int16_t combat_location_type);

// WZD ovr154p04
void Scatter_Rock_Scenery(int16_t TerrType, int16_t combat_location_type);

// WZD ovr154p05
void Build_Road_Network(int16_t location_type, int16_t * roads_array);

// WZD ovr154p06
void Carve_River_Terrain(int16_t * RiverMatrix);

// WZD ovr154p07
void Merge_Dirt_Patches(void);

// WZD ovr154p08
void Remove_Rough_Terrain(int16_t combat_location_type);

// WZD ovr154p09
void Set_Terrain_Tile_Types(void);

// WZD ovr154p10
void Set_Movement_Cost_Maps(int16_t location_type, int16_t city_walls);

// WZD ovr154p11
void Scatter_Terrain_Patches(int16_t ctg, int16_t patch_count, int16_t length_span, int16_t length_base);

// WZD ovr154p12
void Combat_Grid_Screen_Coordinates(int16_t cgx, int16_t cgy, int16_t cgx_subcell_offset, int16_t cgy_subcell_offset, int16_t * screen_x, int16_t * screen_y);

// WZD ovr154p13
void Apply_Earth_To_Mud(int16_t cgx, int16_t cgy);

// WZD ovr154p14
int16_t Get_Combat_Grid_Cell_X(int16_t screen_x, int16_t screen_y);

// WZD ovr154p15
int16_t Get_Combat_Grid_Cell_Y(int16_t screen_x, int16_t screen_y);

// WZD ovr154p16
void Screen_To_Combat_Grid_Cell_X_And_Offset(int16_t screen_x, int16_t screen_y, int16_t * Tile_X, int16_t * InTile_X);

// WZD ovr154p17
void Screen_To_Combat_Grid_Cell_Y_And_Offset(int16_t screen_x, int16_t screen_y, int16_t * Tile_Y, int16_t * InTile_Y);

// WZD ovr154p18
void Combat_Cache_Read(void);

// WZD ovr154p19
void Combat_Cache_Write(void);



/*
    WIZARDS.EXE  ovr163
*/

// WZD ovr163p01
void Load_Combat_Terrain_Pictures(int16_t cts, int16_t wp);

// WZD ovr163p02
void Combat_Compose_Background(void);

// WZD ovr163p03
void Wall_Rise_Load(int16_t wall_type);

// WZD ovr163p04
void Allocate_Combat_Base_Blocks(void);

// WZD ovr163p05
void Make_Missiles(int16_t missile_count, int16_t Targets, int16_t src_wx, int16_t src_wy, int16_t dst_wx, int16_t dst_wy, int16_t type);

// WZD ovr163p06
int16_t Combat_Figure_Load(int16_t unit_type, int16_t bufpi);



#ifdef __cplusplus
}
#endif

#endif  /* COMBAT_H */
