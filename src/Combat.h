/*
    Combat

    WIZARDS.EXE
        ovr090
        ovr091
        ovr096
        ovr098
        ovr099  ¿ MoO2  Module: CMBTDRW1 ?
        ovr103
        ovr105
        ovr110
        ovr111
        ovr112
        ovr114  ¿ MoO2  Module: CMBTAI ?
        ovr116
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

#include "STU/STU_DBG.h"

#include "MOX/MOM_Data.h"
#include "MOX/MOX_TYPE.h"



/*

if(combat_enchantments[CALL_LIGHTNING_DFNDR] != 0)


like with CMB_CE_Refresh__WIP()
// even/odd - attacker/defender
if((itr % 2) == 0)
    CMB_ATKR_CE_Wnd[attacker_enchantment_count].icon_seg = combat_enchantment_icon_segs[combat_enchantment_icon_data[idx].icon_idx];
    CMB_ATKR_CE_Wnd[attacker_enchantment_count].help_idx = combat_enchantment_icon_data[idx].help_idx;
else
    CMB_DEFR_CE_Wnd[defender_enchantment_count].icon_seg = combat_enchantment_icon_segs[combat_enchantment_icon_data[idx].icon_idx];
    CMB_DEFR_CE_Wnd[defender_enchantment_count].help_idx = combat_enchantment_icon_data[idx].help_idx;

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

// _battlefield_city_walls
#define BATTLEFIELD_CITY_WALL_STONE     0x1 /* 0b00000001 */
#define BATTLEFIELD_CITY_WALL_FIRE      0x2 /* 0b00000010 */
#define BATTLEFIELD_CITY_WALL_DARKNESS  0x4 /* 0b00000100 */



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

#define CALC_BASE_CGC2  IDK_base_cgc2 = (itr_y / 2)
#define CALC_BASE_CGC1  IDK_base_cgc1 = (((itr_y + 1) / 2) + 9)

#define CALC_CGX       cgx = (IDK_base_cgc2 + itr_x)
#define CALC_CGY       cgy = (IDK_base_cgc1 - itr_x)

#define CALC_SCREEN_X   screen_x = ((itr_x * 32) - ((itr_y & 0x1) * 16))
#define CALC_SCREEN_Y   screen_y = ((itr_y * 8) - 8)


#define COMBAT_GRID_CELL_WIDTH       32  // in pixels
#define COMBAT_GRID_CELL_HEIGHT      16  // in pixels

#define COMBAT_GRID_WIDTH   21
#define COMBAT_GRID_HEIGHT  22
#define COMBAT_GRID_COLS    21
#define COMBAT_GRID_ROWS    22

#define COMBAT_GRID_XMIN     0
#define COMBAT_GRID_XMAX    21
#define COMBAT_GRID_YMIN     0
#define COMBAT_GRID_YMAX    22

// path finding iters over {(21-2),(22-2)}
// if(battlefield->Central_Structure!= CS_None)
//     CMB_ActiveMoveMap[0xED] = -1;  // EDh  237d  237 / 21 = 11.28571  11 * 21 = 231  237 - 231 = 6  ~ cgx = 6, cgy = 11
#define COMBAT_STRUCTURE_CGX 11
#define COMBAT_STRUCTURE_CGY  6
#define COMBAT_STRUCTURE_IDX ((COMBAT_STRUCTURE_CGY * COMBAT_GRID_WIDTH)   + COMBAT_STRUCTURE_CGX)
#define COMBAT_GRID_CITY_AREA_WIDTH     4
#define COMBAT_GRID_CITY_AREA_HEIGHT    4
/*

CMB_DrawMap__WIP()
    if
        (battlefield->Central_Structure == CS_SorceryNode)
        (battlefield->Central_Structure == CS_ChaosNode)
        (battlefield->Central_Structure == CS_Outpost)
        && (cgx == 6) && (cgy == 11)
    if
        (battlefield->Central_Structure == CS_City)
        (battlefield->Central_Structure == CS_Fortress)
        && (cgx == 8) && (cgy == 13)

*/
/*
Generate_Combat_Map__WIP()
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

#define INF                     0xFF

#define IS_INF(_value_)         ((uint8_t)(_value_) == INF)

//  Combat_Move_Path_Find()
//     // MovePath.C  Move_Path_Find()
//     // for(itr = 0; itr < WORLD_SIZE; itr++) { movepath_cost_map->Reach_From[itr] = itr; }
//     for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++)
//     {
//         CMB_NearBuffer_3[itr] = itr;  /* ¿ reach from ? */
//     }
//  Combat_Move_Path_Valid()
//     // MovePath.C  Move_Path_Find()
//     // for(itr = 0; itr < WORLD_SIZE; itr++) { movepath_cost_map->Reach_From[itr] = itr; }
//     for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++)
//     {
//         CMB_NearBuffer_3[itr] = itr;
//     }
#define SET_REACH_FROM          { for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++) { CMB_NearBuffer_3[itr] = itr; } }

//  Combat_Move_Path_Find()
//      // MovePath.C  Move_Path_Find()
//      // for(itr = 0; itr < WORLD_SIZE; itr++) {movepath_cost_map->Reach_Costs[itr] = 255; }
//      for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++)
//      {
//  
//          CMB_Path_Costs[itr] = 255;  /* ¿ infinity ? */
//  
//      }
//     CMB_Path_Costs[((destination_cgy * COMBAT_GRID_WIDTH) + destination_cgx)] = 0;  /* manually set the current cell's cost to zero */
//  Combat_Move_Path_Valid()
//     // MovePath.C  Move_Path_Find()
//     // for(itr = 0; itr < WORLD_SIZE; itr++) {movepath_cost_map->Reach_Costs[itr] = 255; }
//     for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++)
//     {
// 
//         CMB_Path_Costs[itr] = 255;  // ¿ ~ inifinity ?
// 
//     }
//     CMB_Path_Costs[((destination_cgy * COMBAT_GRID_WIDTH) + destination_cgx)] = 0;  /* manually set the current cell's cost to zero */
#define SET_LENGTH              { for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++) { CMB_Path_Costs[itr] = INF;  CMB_Path_Costs[((source_cgy * COMBAT_GRID_WIDTH) + source_cgx)] = 0; } }

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
            move_cost = CMB_ActiveMoveMap[ctr];  // moves2 cost of cell, given terrain and movement mode
            if(!IS_INF(move_cost))
            {
                old_next_cell_index = CMB_NearBuffer_3[ctr];
                for(itr_adjacent = 0; itr_adjacent < 4; itr_adjacent++)
                {
                    adjacent_idx = CMB_AdjctOfs_NoWest[itr_adjacent];
                    NEW_PATH_COST()
                }
                for(itr_adjacent = 4; itr_adjacent < 8; itr_adjacent++)
                {
                    adjacent_idx = CMB_AdjctOfs_NoWest[itr_adjacent];
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
        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];              \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(CMB_Path_Costs[ctr] > potential_path_cost)               \
            {                                                           \
                CMB_NearBuffer_3[ctr] = adjacent_idx;                   \
                CMB_Path_Costs[ctr] = potential_path_cost;              \
                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)        \
                {                                                       \
                    Map_Changed = ST_TRUE;                              \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}


#define NEW_PATH_COST_0() {                                             \
    adjacent_idx = adjacent_offsets[((0 * 8) + itr_adjacent)];          \
    if((adjacent_idx >= 0) && (adjacent_idx < COMBAT_GRID_CELL_COUNT))  \
    {                                                                   \
        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];              \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(CMB_Path_Costs[ctr] > potential_path_cost)               \
            {                                                           \
                CMB_NearBuffer_3[ctr] = adjacent_idx;                   \
                CMB_Path_Costs[ctr] = potential_path_cost;              \
                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)        \
                {                                                       \
                    Map_Changed = ST_TRUE;                              \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#define NEW_PATH_COST_1() {                                             \
    adjacent_idx = adjacent_offsets[((1 * 8) + itr_adjacent)];          \
    if((adjacent_idx >= 0) && (adjacent_idx < COMBAT_GRID_CELL_COUNT))  \
    {                                                                   \
        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];              \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(CMB_Path_Costs[ctr] > potential_path_cost)               \
            {                                                           \
                CMB_NearBuffer_3[ctr] = adjacent_idx;                   \
                CMB_Path_Costs[ctr] = potential_path_cost;              \
                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)        \
                {                                                       \
                    Map_Changed = ST_TRUE;                              \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#define NEW_PATH_COST_2() {                                             \
    adjacent_idx = adjacent_offsets[((2 * 8) + itr_adjacent)];          \
    if((adjacent_idx >= 0) && (adjacent_idx < COMBAT_GRID_CELL_COUNT))  \
    {                                                                   \
        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];              \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(CMB_Path_Costs[ctr] > potential_path_cost)               \
            {                                                           \
                CMB_NearBuffer_3[ctr] = adjacent_idx;                   \
                CMB_Path_Costs[ctr] = potential_path_cost;              \
                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)        \
                {                                                       \
                    Map_Changed = ST_TRUE;                              \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#define NEW_PATH_COST_ANY(_value_) {                                            \
    adjacent_idx = (ctr + adjacent_offsets[(((_value_) * 8) + itr_adjacent)]);  \
    if(adjacent_idx == ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx))               \
    {                                                                           \
        STU_DEBUG_BREAK();                                                         \
    }                                                                           \
    if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))                          \
    {                                                                           \
        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];                      \
        if(!IS_INF(adjacent_path_cost))                                         \
        {                                                                       \
            potential_path_cost = adjacent_path_cost + move_cost;               \
            existing_path_cost = CMB_Path_Costs[ctr];                           \
            if(existing_path_cost > potential_path_cost)                        \
            {                                                                   \
                CMB_NearBuffer_3[ctr] = adjacent_idx;                           \
                CMB_Path_Costs[ctr] = potential_path_cost;                      \
                new_next_cell_index = CMB_NearBuffer_3[ctr];                    \
                if(new_next_cell_index != old_next_cell_index)                  \
                {                                                               \
                    unstable = ST_TRUE;                                         \
                }                                                               \
            }                                                                   \
        }                                                                       \
    }                                                                           \
}

#define NEW_PATH_COST_ALL(_value_) {                            \
    move_cost = CMB_ActiveMoveMap[ctr];                         \
    if(!IS_INF(move_cost))                                      \
    {                                                           \
        old_next_cell_index = CMB_NearBuffer_3[ctr];            \
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

EMM_FIGUREX_Init__HACK()
USELESS_Combat_Figure_Load_Compose()
Combat_Figure_Compose_USEFULL()
Combat_Figure_Load()


EMM_FIGUREX_Init__HACK()
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

USELESS_Combat_Figure_Load_Compose()
    if((bufpi & 0x1) == 0)
    {
        offset = 0;
    }
    else
    {
        offset = (512 * SZ_PARAGRAPH_B);  // 512 PR, 8192 B; 512 / 56 = 9.14 ... ¿ 2 sets of 9 figure pictures ?
    }
    offset += (bufpi * (56 * SZ_PARAGRAPH_B));
    EMM_FIGUREX_Init__HACK(bufpi);
    figure_pointer_seg = Allocate_First_Block((EMS_PFBA + offset), 33);
    ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;

Combat_Figure_Compose_USEFULL()
        EMM_FIGUREX_Init__HACK(bufpi);
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
    EMM_FIGUREX_Init__HACK(bufpi);
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

    1)  EMM_FIGUREX_Init__HACK(bufpi);

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

EMM_FIGUREX_Init__HACK()
    2
USELESS_Combat_Figure_Load_Compose()
    2, 1, 3
Combat_Figure_Compose_USEFULL()
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
#define FIGUREX_MAP         EMM_FIGUREX_Init__HACK(bufpi);
#define FIGUREX_OFFSET      {                   \
    offset = (bufpi & 0x1) ? 512 : 0;           \
    offset += (bufpi * (56 * SZ_PARAGRAPH_B));  \
}
#define FIGUREX_POINTER     {                                                   \
    /* figure_pict_set_seg = SA_MK_FP0(Allocate_First_Block((EMS_PFBA + offset), 33)); */   \
    figure_pict_set_seg = (SAMB_ptr *)Allocate_First_Block((EMS_PFBA + offset), 33);        \
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
    clt_Fortress        =  3, 
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


enum Central_Structures
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
    Generate_Combat_Map__WIP()

¿ overlaps with code using e_COMBAT_LOCATION_TYPE ?
*/

enum e_COMBAT_TERRAIN_SET
{
    cts_Water      = 0, 
    cts_Plains     = 1, 
    cts_Hills      = 2, 
    cts_Desert     = 3, 
    cts_Mountains  = 4, 
    cts_Tundra     = 5, 
    cts_Forest     = 6
};

enum Combat_TileGroup
{
    CTG_Grass       = 0, 
    CTG_Rough       = 1, 
    CTG_Dirt        = 2, 
    CTG_River       = 3, 
    CTG_DeepWater   = 4, 
    CTG_IDK         = 5  // not used in Set_Movement_Cost_Maps()
};

/*
    END: Combat Terrain
*/


/*
    ¿ Meaning / Usage ?

*/
enum e_BATTLE_UNIT_STATUS
{
    bus_Active      = 0,  /* ~== Alive; checked for 'attackable' target */
    bus_Recalled    = 1,  /* ¿ ? */
    bus_Fleeing     = 2,  /* ¿ ? */
    bus_Uninvolved  = 3,  /* ~ "can not engage" used for 'Floating Island', Transports, etc.; included in 'Flee'; EoC, set to bus_Active; */
    bus_Dead        = 4,  /* ¿ ? */
    bus_Drained     = 5,  /* ¿ ? */
    bus_Gone        = 6   /* ¿ ? ¿ ~== _UNITS[].wp == 9 ? ¿ excluded from 'Raise Dead' */
};

enum e_BATTLE_UNIT_ACTION
{
    bua_Ready           =   0,
    bua_Finished        =   4,
    bua_Wait            =   5,
    /* ¿ BEGIN:  computer-player actions ? */
    BUA_MeleeAttack     = 100,
    BUA_RangedAttack    = 101,
    BUA_MoveNFire       = 102,
    BUA_MoveNAttack     = 103,
    BUA_DoomBolt        = 104,
    BUA_Fireball        = 105,
    bua_Healing         = 106,
    BUA_UseItem         = 107,
    BUA_CastSpell       = 108,
    BUA_SummonDemon     = 109,
    bua_WebSpell        = 110,
    BUA_Flee            = 150,
    /* ¿ END:  computer-player actions ? */
    BUA_No_Spells       = 333
};


/*

CMB_PrepareTurn__WIP() uses `test .. jz`

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


enum e_CMB_ScrollMsg_Type
{
    cmbmsg_AttVictory  =  1,  // ; combat victory
    cmbmsg_AttDefeat   =  2,  // ; combat defeat
    cmbmsg_AttFled     =  3,  // ; combat fled
    cmbmsg_Draw        =  4,  // ; combat draw
    cmbmsg_DefFled     =  5,  // ; combat enemy fled
    cmbmsg_DefVictory  =  6,  // ; combat victory
    cmbmsg_DefDefeat   =  7,  // ; combat defeat
    cmbmsg_IDK_08      =  8,  // ; 8-11 - ??? (no extra text)
    cmbmsg_IDK_09      =  9,
    cmbmsg_IDK_10      = 10,
    cmbmsg_IDK_11      = 11,
    cmbmsg_CityLost    = 12   // ; city lost
};

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

/*
    MoO2 "board"
*/
// sizeof: 0x1598
struct s_BATTLEFIELD
{
    /* 0x0000 */  int8_t terrain_type[COMBAT_GRID_CELL_COUNT];  // 21 x 22
    /* 0x039C */  int8_t terrain_group[COMBAT_GRID_CELL_COUNT];  /* 1-byte, unsigned */
    /* 0x056A */  int8_t roads[COMBAT_GRID_CELL_COUNT];
    /* 0x0738 */  int8_t MoveCost_Ground[COMBAT_GRID_CELL_COUNT];
    /* 0x0906 */  int8_t MoveCost_Teleport[COMBAT_GRID_CELL_COUNT];
    /* 0x0AD4 */  int8_t MoveCost_Ground2[COMBAT_GRID_CELL_COUNT];
    /* 0x0CA2 */  int8_t MoveCost_Sailing[COMBAT_GRID_CELL_COUNT];
    /* 0x0E70 */  int16_t Tree_Count;
    /* 0x0E72 */  int16_t Tree_DrawXs[100];
    /* 0x0F3A */  int16_t Tree_DrawYs[100];
    /* 0x1002 */  int16_t Tree_Indices[100];
    /* 0x10CA */  int16_t Rock_Count;
    /* 0x10CC */  int16_t Rock_DrawXs[100];
    /* 0x1194 */  int16_t Rock_DrawYs[100];
    /* 0x125C */  int16_t Rock_IMG_Segs[100];
    /* 0x1324 */  int8_t muds[COMBAT_GRID_CELL_COUNT];  /* {F,T} combat map square is 'mud' */
    /* 0x14F2 */  int16_t Central_Structure;   // enum Central_Structures
    /* 0x14F4 */  int16_t House_Count;
    /* 0x14F6 */  int16_t House_TileXs[16];    // [4][4]
    /* 0x1516 */  int16_t House_TileYs[16];    // [4][4]
    /* 0x1536 */  SAMB_ptr House_IMG_Segs[16];  // [4][4]  ¿ assigned pict seg from [3][15] house types ?
    /* 0x1556 */  int16_t walled;  // {F,T} city has stone wall;  ... used to set _battlefield_city_walls |= 0x1;, so must be specifcally 'City Walls'/'Wall of Stone'
    /* 0x1558 */  int16_t walls[16];   /* [4][4] as {4,4,4,4}; state/status 0: none, 1:good, 2:bad  spl_Wall_Of_Stone sets 1, spl_Disrupt sets 2 */
    /* 0x1578 */  int16_t wall_of_fire;  // {F,T}
    /* 0x157A */  int16_t wall_of_darkness;  // {F,T}
    /* 0x157C */  int16_t wp;
    /* 0x157E */  // struct s_CTY_ENCH city_enchantments[26];  // int16_t city_enchantments CTY_ENCH
                  int8_t city_enchantments[26];
    /* 0x1597 */  int8_t Nightshades;  // ~ enchantment, but count, rather than player num bitfield
    /* 0x1598 */
};


// ; (sizeof=0x1E)
// sizeof 40, due to union / pointer size ... in-mem, union looks to be 12 bytes
struct s_COMBAT_ENTITY
{
    /* 0x00 */ int16_t draw_x;  /* only used by CMB_DrawEntities__WIP();  e.g., battle unit figure screen x;  screen x for Draw_Picture_Windowed() */
    /* 0x02 */ int16_t draw_y;  /* only used by CMB_DrawEntities__WIP();  e.g., battle unit figure screen y;  screen y for Draw_Picture_Windowed() */
    /* 0x04 */ // int16_t IMG_Seg_or_Id;
    union {
        int64_t seg_or_idx;
        int16_t index;
        SAMB_ptr pict_seg;
    };
    /* 0x06 */ int16_t frame_num;
    /* 0x08 */ int16_t draw_y_shift;  /* only used by CMB_DrawEntities__WIP(), only subtracted, never added;  curiously hard-coded value, per entity type;  screen y offset for Draw_Picture_Windowed() */
    /* 0x0A */ int16_t draw_x_shift;  /* only used by CMB_DrawEntities__WIP(), only subtracted, never added;  curiously hard-coded value, per entity type;  screen x offset for Draw_Picture_Windowed() */
    /* 0x0C */ int16_t entity_type;  /* ¿ entity type ?  1 is battle unit figure */
    /* 0x0E */ int16_t owner_idx;  /* battle_unit[].controller_idx */
    /* 0x10 */ int16_t niu_figure_set_idx;
    /* 0x12 */ int16_t outline_magic_realm;  /* unit enchantment outline magic realm */
    /* 0x14 */ int16_t Unused_14h;
    /* 0x16 */ int16_t Blood_Frame;
    /* 0x18 */ int16_t Blood_Amt;
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





// WZD dseg:7080
extern int16_t frame_anim_cycle;

// WZD dseg:708A
extern int16_t CMB_ChasmAnimStage;

// WZD dseg:708E
extern int16_t CMB_Chasm_Anim;

// WZD dseg:7116
extern int16_t ai_human_hostility;

// WZD dseg:C418
extern int16_t _combat_city_damage;

// WZD dseg:C41A
extern SAMB_ptr SND_CMB_Silence;
extern uint32_t SND_CMB_Silence_size;  // DNE in Dasm

// WZD dseg:C432
extern int16_t _auto_combat_flag;

// WZD dseg:C47C
extern int16_t _combat_wp;

// WZD dseg:C47E
extern int16_t _combat_wy;

// WZD dseg:C480
extern int16_t _combat_wx;

// WZD dseg:C522
extern struct s_mouse_list * _combat_mouse_grid;

// WZD dseg:C524
extern int8_t * CMB_TargetRows[COMBAT_GRID_HEIGHT];

// WZD dseg:C582
extern int16_t _active_battle_unit;

// WZD dseg:C584
extern int16_t _combat_defender_player;

// WZD dseg:C586
extern int16_t _combat_attacker_player;

// WZD dseg:C588
extern int16_t _combat_total_unit_count;

// WZD dseg:C7B2
extern SAMB_ptr IMG_CMB_FX_Figure;

// WZD dseg:C8A8
// WZD dseg:C8AA
/*
¿ only used by/for TILE_DispelMagic__WIP() ?
*/
extern int16_t * CMB_NearDispel_UCs;
extern int16_t * CMB_NearDispel_UEs;

// WZD dseg:C972
extern int16_t CMB_Winner;

// WZD dseg:C974
extern int16_t GAME_RazeCity;

// WZD dseg:C976
extern int16_t CMB_Gold_Reward;

// WZD dseg:C978
extern int16_t CMB_Buildings_Lost;

// WZD dseg:C97A
extern int16_t * CMB_LostBuildings;

// WZD dseg:C97C
extern int16_t CMB_Population_Lost;

// WZD dseg:C97E
extern int16_t CMB_ScrollMsg_Type;

// WZD dseg:CF9A
extern SAMB_ptr EmmHndl_TILEXXX;

// WZD dseg:CF9C
extern SAMB_ptr EmmHndl_FIGUREX;

// WZD dseg:CFBE
extern SAMB_ptr IMG_CMB_Blood[5];

// WZD dseg:CFCA
extern SAMB_ptr IMG_CMB_Curses[8];

// WZD dseg:CFEE
extern SAMB_ptr IMG_CMB_RiverTile[12];

// WZD dseg:D006
extern SAMB_ptr IMG_CMB_RivrNULLs[12];

// WZD dseg:D01E
extern SAMB_ptr IMG_CMB_RoadTiles[28];

// WZD dseg:D056
extern SAMB_ptr IMG_CMB_OceanTile[4];

// WZD dseg:D05E
extern SAMB_ptr IMG_CMB_ChaosOcn[4];

// WZD dseg:D066
extern SAMB_ptr IMG_CMB_Cloud;

// WZD dseg:D06E
extern SAMB_ptr IMG_CMB_FlotIsle;

// WZD dseg:D070
extern SAMB_ptr IMG_CMB_Mud;

// WZD dseg:D072
extern SAMB_ptr IMG_CMB_SorcNode;

// WZD dseg:D074
extern SAMB_ptr IMG_CMB_NatNode;

// WZD dseg:D076
// NEWCODE  extern SAMB_ptr IMG_CMB_Volcano[8];
extern SAMB_ptr IMG_CMB_Volcano[9];

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
extern SAMB_ptr IMG_CMB_Houses[15];

// WZD dseg:D13A
extern SAMB_ptr IMG_GUI_Chasm;

// WZD dseg:D13C
extern SAMB_ptr magic_vortex_seg;

// WZD dseg:D13E
extern SAMB_ptr CMB_Path_Ys;

// WZD dseg:D140
extern SAMB_ptr CMB_Path_Xs;

// WZD dseg:D142
extern int16_t CMB_Path_Length;

// WZD dseg:D144
extern int16_t * CMB_NearBuffer_3;

// WZD dseg:D146
extern uint8_t * CMB_Path_Costs;

// WZD dseg:D148
extern int8_t * CMB_ActiveMoveMap;

// WZD dseg:D14A
// WZD dseg:D14C
extern int16_t CMB_Chasm_Anim_Y;
extern int16_t CMB_Chasm_Anim_X;

// WZD dseg:D14E
extern struct s_MAGIC_VORTEX * _vortexes;

// WZD dseg:D152
extern int16_t _vortex_count;

// WZD dseg:D154
extern SAMB_ptr CMB_Projectiles;

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
// drake178: CMB_TacticalCombat()
int16_t Combat_Screen__WIP(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[]);



/*
    WIZARDS.EXE  ovr091
*/

// WZD s91p01
// drake178: UU_BU_LoadFigureGFX()

// WZD s91p02
void CMB_PrepareTurn__WIP(void);

// WZD s91p03
void Set_Movement_Cost_Map(int16_t battle_unit_idx);

// WZD s91p04
void Move_Battle_Unit__WIP(int16_t battle_unit_idx, int16_t target_cgx, int16_t target_cgy);

// WZD s91p05
void Switch_Active_Battle_Unit(int16_t battle_unit_idx);

// WZD s91p06
void Battle_Unit_Action__WIP(int16_t battle_unit_idx, int16_t x, int16_t y);

// WZD s91p07
void Assign_Combat_Grids(void);

// WZD s91p08
int16_t BU_IsVisible__STUB(int16_t battle_unit_idx);

// WZD s91p09
void Add_City_Damage_From_Battle_Units_Within(void);

// WZD s91p10
int16_t BU_GetInstaMoveType__WIP(int16_t battle_unit_idx);



/*
    WIZARDS.EXE  ovr096
*/

// WZD s96p01
int16_t Combat__WIP(int16_t attacker_player_idx, int16_t defender_player_idx, int16_t troop_count, int16_t troops[]);

// WZD s96p02
// drake178: EZ_CreateGuardStack()
void Lair_Make_Guardians(int16_t lair_idx);

// WZD s96p03
// drake178: EZ_ResolveEntry()
int16_t Lair_Combat_Do(int16_t lair_idx, int16_t player_idx);

// WZD s96p04
// drake178: sub_7DE08()
// UU_IDK_CMB_s7DE08()

// WZD s96p05
// drake178: WIZ_SetHostile()
void Update_Defender_Hostility(int attacker_player_idx, int defender_player_idx);

// WZD s96p06
// UU_IDK_Main_Screen_Draw()




/*
    WIZARDS.EXE  ovr098
*/

// WZD o98p01
// drake178: CMB_CE_Refresh()
void CMB_CE_Refresh__WIP(void);

// WZD o98p02
int16_t BU_HasSpellAbility__WIP(int16_t battle_unit_idx);

// WZD o98p03
// NIU  int16_t NIU_Who_Has_More_Leadership(void);

// WZD o98p04
void AI_CMB_PlayTurn__WIP(int16_t player_idx);

// WZD o98p05
void CMB_ProgressTurnFlow__WIP(void);

// WZD o98p06
// drake178: WIZ_FleeCombat()
void Retreat_From_Combat(int16_t player_idx);

// WZD o98p07
// drake178: TILE_HasCityOfPlayer()
int16_t Player_City_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

// WZD o98p08
int16_t Process_Retreating_Units(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

// WZD o98p09
int16_t Unit_Try_To_Move(int16_t wx, int16_t wy, int16_t wp, int16_t unit_idx, int16_t troop_count);

// WZD o98p10
void Assign_Mouse_Images(void);

// WZD o98p11
int16_t BU_GetRangedCursor__WIP(int16_t src_battle_unit_idx, int16_t dst_battle_unit_idx);

// WZD o98p12
void Add_Combat_Enchantment_Fields(void);

// WZD o98p13
int16_t Battle_Unit_Movement_Icon(int16_t battle_unit_idx);

// WZD o98p14
// static void UNIT_SummonToBattle__SEGRAX(int16_t player_idx, int16_t unit_idx, int16_t cgx, int16_t cgy);

// WZD o98p15
void BU_UnitLoadToBattle__SEGRAX(int16_t battle_unit_idx, int8_t player_idx, int16_t unit_idx, int16_t cgx, int16_t cgy);

// WZD o98p16
int16_t CMB_Units_Init__WIP(int16_t troop_count, int16_t troops[]);

// WZD o98p17
int16_t BU_CheckFlight__WIP(int16_t battle_unit_idx);

// WZD o98p18
int16_t Battle_Unit_Has_Flight(int16_t battle_unit_idx);

// WZD o98p19
int16_t Check_Attack_Melee(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx);

// WZD o98p20
// SAMB_ptr BU_PrepMoveSound__WIP(int16_t battle_unit_idx);
void BU_PrepMoveSound__WIP(int16_t battle_unit_idx, SAMB_ptr * Sound_Data_Seg, uint32_t * Sound_Data_Seg_size);

// WZD o98p21
void Turn_Off_Auto_Combat(void);

// WZD o98p22
void STK_ComposeFleeLost__STUB(int16_t troop_count, int16_t troop_list[]);



/*
    WIZARDS.EXE  ovr099
*/

// WZD o99p01
// WZD o99p02
// WZD o99p03
// WZD o99p04
// WZD o99p05
// WZD o99p06
// WZD o99p07

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
void Combat_Grid_Entities__WIP(void);

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
void BU_CreateImage__SEGRAX(int battle_unit_idx);

// WZD s103p08
void Next_Battle_Unit(int16_t player_idx);

// WZD s103p09
int16_t Next_Battle_Unit_Nearest_Available(int16_t player_idx);

// WZD s103p10
void Combat_Cast_Spell_Error(int16_t type);

// WZD s103p11
void BU_MoveConfused__WIP(int16_t battle_unit_idx);

// WZD s103p12
void CMB_LoadResources__WIP(void);

// WZD s103p13
void CMB_SetNearAllocs__WIP(void);

// WZD s103p14
void Combat_Cast_Spell_With_Caster(int16_t caster_id);



/*
    WIZARDS.EXE  ovr105
*/

// WZD o105p01
int16_t Check_For_Winner__WIP(void);

// WZD o105p02
void Combat_Figure_Banner_Color(int16_t player_idx);

// WZD o105p03
void Combat_Figure_Active_Red_Outline(int16_t battle_unit_idx);

// WZD o105p04
void Combat_Figure_Effect__WIP(int16_t effect);

// WZD o105p05
void Combat_Unit_Enchantment_Outline_Set(int16_t battle_unit_idx);

// WZD o105p06
void Combat_Unit_Enchantment_Outline_Draw(int16_t enchantment_magic_realm);

// WZD o105p07
void Update_Sees_Illusions(void);

// WZD o105p08
 void BU_SetVisibility__WIP(int16_t battle_unit_idx);

// WZD o105p09
int16_t BU_GetCombatEffect__WIP(int16_t battle_unit_idx);

// WZD o105p10
void BU_SetBaseAnims__WIP(int16_t battle_unit_idx);

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
int16_t Strategic_Combat__WIP(int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[]);

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
int16_t AITP_CombatSpell__STUB(int16_t spell_idx, int16_t player_idx, int16_t * Target_X, int16_t * Target_Y);

// WZD 111p02
// AITP_DarknessLight()

// WZD 111p03
// AITP_Healing()

// WZD 111p04
// AITP_WarpWood()

// WZD 111p05
// AITP_WarpCreature()

// WZD 111p06
// UU15_AITP_Disintegrate()

// WZD 111p07
// AITP_DispelMagic()

// WZD 111p08
void Cast_Spell_On_Battle_Unit(int16_t spell_idx, int16_t target_idx, int16_t caster_idx, int16_t target_cgx, int16_t target_cgy, int16_t Mana, int16_t Anims, int16_t UU1, int16_t UU2);

// WZD 111p09
// drake178: AITP_HolyWord() 
// AITP_HolyWord()



/*
    WIZARDS.EXE  ovr112
*/

// WZD o112p01
void CMB_ComposeBookBG__WIP(void);

// WZD o112p02
void Combat_Spellbook_Screen_Draw(void);

// WZD o112p03
int16_t Combat_Cast_Spell__WIP(int16_t caster_id, int16_t wx, int16_t wy, int16_t wp);

// WZD o112p04
int16_t Combat_Spellbook_Screen(int16_t caster_idx, int16_t * selected_spell);

// WZD o112p05
int16_t Do_Legal_Spell_Check__WIP(int16_t spell_idx);

// WZD o112p06
int16_t Spell_Resistance_Modifier(int16_t spell_idx);

// WZD o112p07
int16_t Combat_Casting_Cost_Multiplier(int16_t player_idx);

// WZD o112p08
// AITP_EarthToMud()

// WZD o112p09
// AITP_Disrupt()

// WZD o112p10
// AITP_CracksCall()

// WZD o112p11
// UU_AITP_WordofRecall()

// WZD o112p12
// AITP_RecallHero()



/*
    WIZARDS.EXE  ovr113
*/

// WZD o113p01
void Combat_Screen_Assign_Mouse_Images(void);

// WZD o113p02
// drake178: CMB_DrawTargetScreen()
void Combat_Spell_Target_Screen_Draw(void);

// WZD o113p03
int16_t Combat_Spell_Target_Screen__WIP(int16_t spell_idx, int16_t * target_cgx, int16_t * target_cgy);

// WZD o113p04
void CMB_RangedAnim__STUB(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t Target_Damage_Sum, int16_t cgx, int16_t cgy);

// WZD o113p05
void CMB_MeleeAnim__STUB(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t attacker_damage, int16_t defender_damage, int16_t cgx, int16_t cgy);

// WZD o113p06
void Apply_Battle_Unit_Damage_From_Spell(uint16_t spell_idx, uint16_t battle_unit_idx, int16_t damage_types[], int16_t attack_override_flag);

// WZD o113p07
void BU_ApplyDamage__WIP(int16_t battle_unit_idx, int16_t damage_types[]);

// WZD o113p08
int16_t Check_Attack_Ranged(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx);

// WZD o113p09
void Deploy_Battle_Units(int16_t player_idx);

// WZD o113p10
void BU_SummonDemon__SEGRAX(int16_t caster_idx);

// WZD o113p11
int16_t BU_MeleeWallCheck__WIP(int16_t src_battle_unit_idx, int16_t dst_battle_unit_idx);

// WZD o113p12
void Apply_Mana_Leak(void);

// WZD o113p13
// GUI_DrawNearMessage()

// WZD o113p14
void Combat_Cast_Spell_Message(int16_t caster_idx, int16_t spell_idx);

// WZD o113p15
int16_t Battle_Unit_Pict_Open(void);

// WZD o113p16
int16_t Check_For_Winner(void);

/*
    WIZARDS.EXE  ovr114
*/
// WZD o114p02
void AI_BU_ProcessAction__WIP(int16_t battle_unit_idx, int16_t Rally_X, int16_t Rally_Y);

// WZD o114p06
int16_t AI_BU_AssignAction__WIP(int16_t battle_unit_idx, int16_t no_spells_flag);

// WZD o114p08
void G_AI_BU_MoveOrRampage__WIP(int16_t battle_unit_idx, int16_t Dest_X, int16_t Dest_Y, int16_t target_battle_unit_idx, int16_t Rally_X, int Rally_Y);



/*
    WIZARDS.EXE  ovr116
*/

void AI_SetBasicAttacks__WIP(int16_t player_idx);
// WZD o114p01

// WZD o114p02
void AI_BU_ProcessAction__WIP(int16_t battle_unit_idx, int16_t Rally_X, int16_t Rally_Y);

// WZD o114p03
void BU_SortSlowestFirst__WIP(int16_t Melee_Unit_List[], int16_t Melee_Unit_Count);

// WZD o114p04
void AI_GetCombatRallyPt__WIP(int16_t battle_unit_idx, int16_t * Rally_X, int16_t * Rally_Y);

// WZD o114p05
void AI_MoveBattleUnits__WIP(int16_t player_idx);

// WZD o114p06
int16_t AI_BU_AssignAction__WIP(int16_t battle_unit_idx, int16_t no_spells_flag);

// WZD o114p07
int16_t AI_BU_SelectAction__WIP(int16_t battle_unit_idx, int16_t * selected_action, int16_t has_ranged_attack);

// WZD o114p08
void G_AI_BU_MoveOrRampage__WIP(int16_t battle_unit_idx, int16_t Dest_X, int16_t Dest_Y, int16_t target_battle_unit_idx, int16_t Rally_X, int Rally_Y);

// WZD o114p09
int16_t Auto_Move_Ship(int16_t battle_unit_idx, int16_t Dest_X, int16_t Dest_Y, int16_t target_battle_unit_idx, int16_t Max_X, int16_t Max_Y);



/*
    WIZARDS.EXE  ovr116
*/

// WZD o116p01
uint32_t BU_Apply_Item_Powers(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p02
void BU_Apply_Item_Enchantments(int16_t item_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p03
void BU_Apply_Item_Attack_Specials(uint16_t * attack_flags, int16_t item_idx);

// WZD o116p04
int16_t Unit_Hit_Points(int16_t unit_idx);

// WZD o116p05
int16_t Battle_Unit_Hit_Points(struct s_BATTLE_UNIT * battle_unit);

// WZD o116p06
void Load_Battle_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * BattleUnit);

// WZD o116p07
void BU_Init_Battle_Unit(struct s_BATTLE_UNIT * BattleUnit);

// WZD o116p08
/*
s_BATTLE_UNIT.Move_Flags
s_BATTLE_UNIT.Attribs_1
s_BATTLE_UNIT.Abilities

*/
void BU_Apply_Specials(struct s_BATTLE_UNIT * battle_unit, uint32_t battle_unit_enchantments, uint8_t unit_mutations);

// WZD o116p09
void BU_Apply_Level(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p10
void BU_Init_Hero_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p11
void BU_Apply_Battlefield_Effects__WIP(struct s_BATTLE_UNIT * battle_unit);

// WZD o116p12
// sub_9A43E()



/*
    WIZARDS.EXE  ovr122
*/

// WZD o122p01
int16_t CMB_AttackRoll__SEGRAX(uint16_t attack_strength, uint16_t to_hit);

// WZD o122p02
uint16_t CMB_DefenseRoll__SEGRAX(uint16_t defense, uint16_t to_block);

// WZD o122p03
int16_t Combat_Resistance_Check(struct s_BATTLE_UNIT battle_unit, int16_t resistance_modifier, int16_t magic_realm);

// WZD o122p04
int16_t Combat_Effective_Resistance(struct s_BATTLE_UNIT battle_unit, int16_t magic_realm);

// WZD o122p05
// BU_GetATKImmFlags()

// WZD o122p06
void BU_AttackTarget__WIP(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t Target_Damage_Array[], int16_t Counter_Damage_Array[], int16_t ranged_attack_flag, int16_t SpFx);

// WZD o122p07
void BU_ProcessAttack__WIP(int16_t attacker_battle_unit_idx, int16_t figure_count, int16_t defender_battle_unit_idx, int16_t attack_mode, int16_t damage_array[], int16_t Counter, int16_t SpFx);

// WZD o122p08
int16_t Battle_Unit_Defense_Special(int16_t battle_unit_idx, int16_t attack_type, int16_t attack_immunities, int16_t attack_attributes, int16_t magic_realm);

// WZD o122p09
int16_t Battle_Unit_Attack_Magic_Realm(int16_t attack_type, int16_t battle_unit_idx);

// WZD o122p10
void Calc_Battlefield_Bonuses(int16_t combat_structure);

// WZD o122p11
void Battle_Unit_Attack__WIP(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t cgx, int16_t cgy);

// WZD o122p12
int16_t Range_To_Battle_Unit(int16_t BU_1, int16_t BU_2);

// WZD o122p12
int16_t Range_To_Battle_Unit(int16_t BU_1, int16_t BU_2);

// WZD o122p13
int16_t AI_BU_GetAttackValue__STUB(int16_t battle_unit_idx, int16_t target_battle_unit_idx, int16_t has_ranged_attack);

// WZD o122p14
int16_t Battle_Unit_Has_Ranged_Attack(int16_t battle_unit_idx);

// WZD o122p15
int16_t BU_CauseFear__NOOP(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx);




/*
    WIZARDS.EXE  ovr123
*/

// WZD o123p01
// drake178: CMB_Victory()
void End_Of_Combat__WIP(int16_t player_idx, int16_t * item_count, int16_t item_list[], int16_t MsgType);

// WZD o123p02
// CMB_GetUndeadCreator()

// WZD o123p03
void Combat_Results_Scroll(void);

// WZD o123p04
// ¿ MoO2  Module: COMBFIND  Strategic_Results_Popup_() ?
void Combat_Results_Scroll_Draw(void);

// WZD o123p05
int16_t Combat_Results_Scroll_Text(void);

// WZD o123p06
int16_t CTY_GetConquerGold(int16_t city_idx);

// WZD o123p07
int16_t CTY_RampageVictory(void);



/*
    WIZARDS.EXE  ovr124
*/

// WZD o124p01
int16_t WIZ_GetLastRangedStr__WIP(int16_t player_idx);

// WZD o124p02
void BU_SetCityMovement__WIP(int16_t battle_unit_idx);

// WZD o124p03
void AI_RestrictToCity__WIP(void);

// WZD o124p04
int16_t Combat_Structure(int16_t wx, int16_t wy, int16_t wp, int16_t set_city_flag);

// WZD o124p05
int16_t Undeployable_Battle_Units_On_Water(int16_t player_idx);

// WZD o124p06
int16_t AI_CompareArmies__STUB(int16_t player_idx);

// WZD o124p07
// SAMB_ptr BU_LoadMoveSound__WIP(int16_t battle_unit_idx);
void BU_LoadMoveSound__WIP(int16_t battle_unit_idx, SAMB_ptr * sound_seg, uint32_t * sound_seg_size);

// WZD o124p08
// SAMB_ptr BU_LoadMeleeSound__WIP(int16_t battle_unit_idx);
void BU_LoadMeleeSound__WIP(int16_t battle_unit_idx, SAMB_ptr * sound_seg, uint32_t * sound_seg_size);

// WZD o124p09
void Combat_Load_Spell_Sound_Effect(int16_t spell_idx);

// WZD o124p10
void BU_WallofFire__NOOP(int16_t battle_unit_idx);

// WZD o124p11
// BU_SetBloodAnim()

// WZD o124p12
// BU_ClearBlood()

// WZD o124p13
int16_t Battle_Unit_Is_Within_City(int16_t battle_unit_idx);

// WZD o124p14
int16_t Combat_Grid_Cell_Has_City_Wall(int16_t cgc2, int16_t cgc1);

// WZD o124p15
int16_t Battle_Unit_Is_Summoned_Creature(int16_t battle_unit_idx);

// WZD o124p16
int16_t Battle_Unit_Moves2(int16_t battle_unit_idx);

// WZD o124p17
void STK_CaptureCity__WIP(int16_t troop_count, int16_t troops[]);

// WZD o124p18
int16_t AI_FightorFlight__STUB(int16_t player_idx);

// WZD o124p19
// AI_Raze_Decision()

// WZD o124p20
int16_t Raze_City_Prompt(char * message);

// WZD o124p21
void Raze_City_Prompt_Draw(void);



/*
    WIZARDS.EXE  ovr139
*/

// WZD ovr139p01
//AI_SelectCmbtSpell()

// WZD ovr139p02
//AI_EvaluateCmbtSpell()

// WZD ovr139p03
int16_t Effective_Battle_Unit_Strength(int16_t battle_unit_idx);

// WZD ovr139p04
//AI_SetCombatRealms()

// WZD ovr139p05
//AI_UnitThreatRealms()

// WZD ovr139p06
//AI_CombatSpellList()



/*
    WIZARDS.EXE  ovr153
*/

// WZD ovr153p01
void NX_IDK_CombatInit_Tactical(int16_t wx, int16_t wy, int16_t wp);

// WZD ovr153p02
void Combat_Screen_Map_Draw__WIP(void);

// WZD ovr153p03
void Combat_Screen_Map_Draw_Entities__WIP(void);

// WZD ovr153p04
// drake178: sub_DD536()

// WZD ovr153p05
// drake178: CMB_SpawnTrees()

// WZD ovr153p06
// drake178: CMB_SpawnRocks()

// WZD ovr153p07
// drake178: CMB_SpawnStructures()

// WZD ovr153p08
// drake178: CMB_SpawnStoneWall()

// WZD ovr153p09
// drake178: CMB_SpawnDarkWall()

// WZD ovr153p10
// drake178: CMB_SpawnFireWall()

// WZD ovr153p11
void Combat_Grid_Entity_Create_Vortexes(void);

// WZD ovr153p12
void CMB_SpawnFigure__WIP(int64_t seg_or_idx, int16_t PosX, int16_t PosY, int16_t TarX, int16_t TarY, int16_t MStage, int16_t FigId, int16_t MaxFigs, int16_t owner_idx, int16_t outline_magic_realm, int16_t BldAmt, int16_t UU, int16_t LostFigs, int16_t SrcBld);

// WZD ovr153p13
// drake178: CMB_SpawnUnitCurse()

// WZD ovr153p14
void Clear_Combat_Grid_Entities(void);

// WZD ovr153p15
void USELESS_Combat_Figure_Load_Compose(int16_t figure_index, int16_t figure_set_idx, int16_t player_idx, int16_t enchantment_magic_realm, int16_t Frame);

// WZD ovr153p16
// drake178: CMB_SpawnProjectiles()

// WZD ovr153p17
void Battle_Unit_Figure_Position(int16_t figure_count, int16_t current_figure, int16_t * figure_x, int16_t * figure_y);

// WZD ovr153p18
void Combat_Grid_Entity_Create__WIP(int16_t draw_x, int16_t draw_y, int64_t seg_or_idx, int16_t draw_x_shift, int16_t draw_y_shift, int16_t Frame, int16_t entity_type, int16_t controller_idx, int16_t niu_figure_set_idx, int16_t outline_magic_realm, int16_t BldAmt, int16_t UU_14h, int16_t BldFrm);

// WZD ovr153p19
void Set_Entity_Draw_Order(void);

// WZD ovr153p20
void EMM_FIGUREX_Init__HACK(int16_t figure_index);

// WZD ovr153p21
void EMM_TILEX_Init__HACK(void);

// WZD ovr153p22
// drake178: UU_NullSub_153_1()

// WZD ovr153p23
void Combat_Figure_Compose_USEFULL(void);

// WZD ovr153p24
// drake178: RP_NullSub_153_2()



/*
    WIZARDS.EXE  ovr154
*/

// WZD ovr154p01
void CMB_Terrain_Init__WIP(int16_t wx, int16_t wy, int16_t wp);

// WZD ovr154p02
void Generate_Combat_Map__WIP(
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
// drake178: CMB_TreeGen()

// WZD ovr154p04
// drake178: CMB_RockGen()

// WZD ovr154p05
// drake178: CMB_RoadGen()

// WZD ovr154p06
// drake178: CMB_RiverGen()

// WZD ovr154p07
// drake178: CMB_MergeDirt()

// WZD ovr154p08
// drake178: CMB_RemoveRough()

// WZD ovr154p09
void CMB_TileGen__WIP(int16_t ctt);

// WZD ovr154p10
void Set_Movement_Cost_Maps(int16_t location_type, int16_t city_walls);

// WZD ovr154p11
void Patch_Terrain_Group(int16_t ctg, int16_t count, int16_t max, int16_t min);

// WZD ovr154p12
void Combat_Grid_Screen_Coordinates(int16_t cgx, int16_t cgy, int16_t something_x, int16_t something_y, int16_t * screen_x, int16_t * screen_y);

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
    WIZARDS.EXE  ovr155
*/

// WZD ovr155p01
void Combat_Move_Path_Find(int16_t source_cgx, int16_t source_cgy, int16_t target_cgx, int16_t target_cgy);

// WZD ovr155p02
void Combat_Move_Path_Valid(int16_t cgx, int16_t cgy, int16_t moves2);



/*
    WIZARDS.EXE  ovr163
*/

// WZD ovr163p01
void Load_Combat_Terrain_Pictures(int16_t cts, int16_t wp);

// WZD ovr163p02
void CMB_ComposeBackgrnd__WIP(void);

// WZD ovr163p03
void Wall_Rise_Load(int16_t wall_type);

// WZD ovr163p04
void CMB_BaseAllocs__WIP(void);

// WZD ovr163p05
// CMB_SetProjectiles()

// WZD ovr163p06
int16_t Combat_Figure_Load(int16_t unit_type, int16_t figure_index);



#ifdef __cplusplus
}
#endif

#endif  /* COMBAT_H */
