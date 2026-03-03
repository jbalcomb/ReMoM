/*
    STU Terrain
*/

#include "STU_TERR.h"
#include "STU_TYPE.h"
#include "../../MoM/src/TerrType.h"
#include "../../MoX/src/MOX_DEF.h"

// Terrain_Type_To_Char()

char Terrain_Type_Name(int16_t terrain_type)
{
    switch(terrain_type)
    {
        case tt_Ocean:      return "Ocean";            /*   0  0x0 */
        case tt_BugGrass:    return "BugGrass";         /*   1  0x1 */

        case tt_Shore1_Fst:  return "Shore1_Fst";       /*   2  0x2 */
        case tt_Shore1_Lst:  return "tt_Shore1_Lst";    /* 161  0xA1 */

        case tt_Grasslands1: return "Grasslands";       /* 162  0xA2 */
        case tt_Forest1:     return "Forest";           /* 163  0xA3 */
        case tt_Mountain1:   return "Mountain";         /* 164  0xA4 */
        case tt_Desert1:     return "Desert";           /* 165  0xA5 */
        case tt_Swamp1:      return "Swamp";            /* 166  0xA6 */
        case tt_Tundra1:     return "Tundra";           /* 167  0xA7 */
        case tt_SorceryNode: return "tt_SorceryNode";   /* 168  0xA8 */
        case tt_NatureNode:  return "tt_NatureNode";    /* 169  0xA9 */
        case tt_ChaosNode:   return "tt_ChaosNode";     /* 170  0xAA */
        case tt_Hills1:      return "Hills";            /* 171  0xAB */
        case tt_Grasslands2: return "tt_Grasslands2";   /* 172  0xAC */
        case tt_Grasslands3: return "tt_Grasslands3";   /* 173  0xAD */
        case tt_Desert2:     return "tt_Desert2";       /* 174  0xAE */
        case tt_Desert3:     return "tt_Desert3";       /* 175  0xAF */
        case tt_Desert4:     return "tt_Desert4";       /* 176  0xB0 */
        case tt_Swamp2:      return "tt_Swamp2";        /* 177  0xB1 */
        case tt_Swamp3:      return "tt_Swamp3";        /* 178  0xB2 */
        case tt_Volcano:     return "tt_Volcano";       /* 179  0xB3 */
        case tt_Grasslands4: return "tt_Grasslands4";   /* 180  0xB4 */
        case tt_Tundra2:     return "tt_Tundra2";       /* 181  0xB5 */
        case tt_Tundra3:     return "tt_Tundra3";       /* 182  0xB6 */
        case tt_Forest2:     return "tt_Forest2";       /* 183  0xB7 */
        case tt_Forest3:     return "tt_Forest3";       /* 184  0xB8 */

        default:            return "Unknown";
    }
}

char Terrain_Group_Name(int16_t terrain_type)
{
    switch(terrain_type)
    {
        case tt_Ocean:      return "Ocean";            /*   0  0x0 */
        case tt_BugGrass:    return "BugGrass";         /*   1  0x1 */

        case tt_Shore1_Fst:  return "Shore1_Fst";       /*   2  0x2 */
        case tt_Shore1_Lst:  return "tt_Shore1_Lst";    /* 161  0xA1 */

        case tt_Grasslands1: return "Grasslands";       /* 162  0xA2 */
        case tt_Forest1:     return "Forest";           /* 163  0xA3 */
        case tt_Mountain1:   return "Mountain";         /* 164  0xA4 */
        case tt_Desert1:     return "Desert";           /* 165  0xA5 */
        case tt_Swamp1:      return "Swamp";            /* 166  0xA6 */
        case tt_Tundra1:     return "Tundra";           /* 167  0xA7 */
        case tt_SorceryNode: return "tt_SorceryNode";   /* 168  0xA8 */
        case tt_NatureNode:  return "tt_NatureNode";    /* 169  0xA9 */
        case tt_ChaosNode:   return "tt_ChaosNode";     /* 170  0xAA */
        case tt_Hills1:      return "Hills";            /* 171  0xAB */
        case tt_Grasslands2: return "tt_Grasslands2";   /* 172  0xAC */
        case tt_Grasslands3: return "tt_Grasslands3";   /* 173  0xAD */
        case tt_Desert2:     return "tt_Desert2";       /* 174  0xAE */
        case tt_Desert3:     return "tt_Desert3";       /* 175  0xAF */
        case tt_Desert4:     return "tt_Desert4";       /* 176  0xB0 */
        case tt_Swamp2:      return "tt_Swamp2";        /* 177  0xB1 */
        case tt_Swamp3:      return "tt_Swamp3";        /* 178  0xB2 */
        case tt_Volcano:     return "tt_Volcano";       /* 179  0xB3 */
        case tt_Grasslands4: return "tt_Grasslands4";   /* 180  0xB4 */
        case tt_Tundra2:     return "tt_Tundra2";       /* 181  0xB5 */
        case tt_Tundra3:     return "tt_Tundra3";       /* 182  0xB6 */
        case tt_Forest2:     return "tt_Forest2";       /* 183  0xB7 */
        case tt_Forest3:     return "tt_Forest3";       /* 184  0xB8 */

        default:            return "Unknown";
    }
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

void Find_River_Path_Candidates(int16_t wp)
{
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t terrain_type = 0;

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {
            // 2. validate location
            if(_map_square_terrain_specials[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] != 0) { continue; }
            if(Square_Is_Ocean_NewGame((wx - 1), (wy - 1), wp) == ST_TRUE) { continue; } /* NW */
            if(Square_Is_Ocean_NewGame((wx - 1), (wy    ), wp) == ST_TRUE) { continue; } /* W  */
            if(Square_Is_Ocean_NewGame((wx - 1), (wy + 1), wp) == ST_TRUE) { continue; } /* SW */
            if(Square_Is_Ocean_NewGame((wx    ), (wy - 1), wp) == ST_TRUE) { continue; } /* N */
            if(Square_Is_Ocean_NewGame((wx    ), (wy    ), wp) == ST_TRUE) { continue; } /* C */
            if(Square_Is_Ocean_NewGame((wx    ), (wy + 1), wp) == ST_TRUE) { continue; } /* S  */
            if(Square_Is_Ocean_NewGame((wx + 1), (wy - 1), wp) == ST_TRUE) { continue; } /* NE */
            if(Square_Is_Ocean_NewGame((wx + 1), (wy    ), wp) == ST_TRUE) { continue; } /* W */
            if(Square_Is_Ocean_NewGame((wx + 1), (wy + 1), wp) == ST_TRUE) { continue; } /* SE */
            if(Square_Is_Mountain_NewGame(wx, wy, wp) == ST_TRUE) { continue; }
            if(Square_Is_Hills_NewGame(wx, wy, wp) == ST_TRUE) { continue; }
            if(Square_Is_Node_NewGame(wx, wy, wp) == ST_TRUE) { continue; }
            if(Square_Is_River_NewGame(wx, wy, wp) == ST_TRUE) { continue; }
            terrain_type = p_world_map[wp][wy][wx];

        }
    }






}