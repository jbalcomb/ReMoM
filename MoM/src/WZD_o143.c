/*
    WIZARDS.EXE
        ovr143
*/

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"

#include "../../ext/stu_compat.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"  /* SET_TERRAIN_TYPE */
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/Util.h"
#include "../../MoX/src/paragrph.h"
#include "../../MoX/src/EMS/EMS.h"

#include "MovePath.h"
#include "Terrain.h"
#include "TerrType.h"

#include <string.h>

#include "WZD_o143.h"



// WZD dseg:6EAC BEGIN:  ovr143 - Initialized Data

// WZD dseg:6EAC
int16_t rnd_grasslands[3] = { tte_Grasslands, tte_Grasslands2, tte_Grasslands3 };
// WZD dseg:6EB2
int16_t rnd_forests[3] = { tte_Forest1, _Forest2, _Forest3 };
// WZD dseg:6EB8
int16_t rnd_deserts[3] = { _AllDesert2, _AllDesert3, _AllDesert3 };

// WZD dseg:6EBE 
char terrtype_lbx_file__ovr143[] = "TERRTYPE";

// WZD dseg:6EC7
char cityname_lbx_file__ovr143[] = "CITYNAME";

// WZD dseg:6EC7 END:  ovr143 - Initialized Data



// WZD dseg:CA76                                                 BEGIN:  ovr143 - Uninitialized Data

// WZD dseg:CA76
/*
TERRTYPE.LBX, 000
5 records * 512 size = 2560 bytes
*/
int16_t * terrtype_seg;

// WZD dseg:CA76                                                 END:  ovr143 - Uninitialized Data




/*
    WIZARDS.EXE  ovr143
*/

// WZD o143p01
// drake178: TILE_MakeVolcano()
// MoO2  DNE, but "...generate..." // "...set..."
/*
; turns the square into a volcano, adjusting the flow of
; adjacent squares accordingly
;
; BUG: fails to create minerals
*/
void Set_Terrain_Type_Volcano_With_Owner(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx)
{

    /* OGBUG  movement_mode_cost_maps is not updated: the square keeps the old terrain's movement costs for the rest of the game (the cost maps are built by MAGIC.EXE's world generation and saved with the game; in WIZARDS.EXE only Make_Road, Make_Road_Enchanted and City_Remove_Road write them) */
    SET_TERRAIN_TYPE(wx, wy, wp, tt_Volcano);

    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] = (_map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] | (player_idx + 1) );

    Adjust_Map_Flow(wx, wy, wp);

    // NOTE: as best I can tell, it does actually do exactly this silliness
    *(_map_square_terrain_specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx) &= 0x2F;  /* 0b00101111  clears bits 5,7,8 ...keeps TS_WILDGAME? */
    *(_map_square_terrain_specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx) = 0;

    // ; attempts to generate a new mineral type terrain special on the square with a 20% chance
    // ; BUG: clears the result after setting it
    Generate_Ore(wx, wy, wp);

}


// WZD o143p02
void Volcano_Counts(void)
{
    int16_t volcano_count_array[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t player_idx = 0;
    int16_t itr_wy = 0;
    int16_t itr_players = 0;
    int16_t itr_wx = 0;
    int16_t itr_wp = 0;
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        volcano_count_array[itr_players] = 0;
    }
    for(itr_wp = 0; itr_wp < NUM_PLANES; itr_wp++)
    {
        for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
        {
            for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
            {
                player_idx = (GET_MAP_SQUARE_FLAG(itr_wx, itr_wy, itr_wp) & 7 /*0b00000111*/);
                if(player_idx > HUMAN_PLAYER_IDX)
                {
                    volcano_count_array[(player_idx - 1)] += 1;
                }
            }
        }
    }
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].volcanoes = volcano_count_array[itr_players];
    }
}


// WZD o143p03
void Set_Terrain_Type_Grasslands(int16_t wx, int16_t wy, int16_t wp)
{

    /* OGBUG  movement_mode_cost_maps is not updated: the square keeps the old terrain's movement costs for the rest of the game (the cost maps are built by MAGIC.EXE's world generation and saved with the game; in WIZARDS.EXE only Make_Road, Make_Road_Enchanted and City_Remove_Road write them) */
    /* OGBUG  Random(2) - 1 is 0..1: the third entry, tte_Grasslands3 (0xAD), is never picked (Set_Terrain_Type_Forest() uses Random(3) - 1 and all three forests) */
    SET_TERRAIN_TYPE(wx, wy, wp, rnd_grasslands[(Random(2) - 1)]);

    Adjust_Map_Flow(wx, wy, wp);

    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) & 0xF8 /*0b11111000*/));  // clear the volcano player idx

}


// WZD o143p04
void Set_Terrain_Type_Forest(int16_t wx, int16_t wy, int16_t wp)
{

    /* OGBUG  movement_mode_cost_maps is not updated: the square keeps the old terrain's movement costs for the rest of the game (the cost maps are built by MAGIC.EXE's world generation and saved with the game; in WIZARDS.EXE only Make_Road, Make_Road_Enchanted and City_Remove_Road write them) */
    SET_TERRAIN_TYPE(wx, wy, wp, rnd_forests[(Random(3) - 1)]);

    Adjust_Map_Flow(wx, wy, wp);

    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) & 0xF8 /*0b11111000*/));  // clear the volcano player idx

}


// WZD o143p05
// drake178: RP_TILE_MakeDesert()
static void Set_Terrain_Type_Desert(int16_t wx, int16_t wy, int16_t wp)
{

    SET_TERRAIN_TYPE(wx, wy, wp, rnd_deserts[(Random(3) - 1)]);

    Adjust_Map_Flow(wx, wy, wp);

    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) & 0xF8 /*0b11111000*/));  // clear the volcano player idx

}


// WZD o143p06
// drake178: TILE_MakeMountain()
/*
; turns the square into a mountain, adjusting the flow of adjacent squares accordingly
*/
void Set_Terrain_Type_Mountain(int16_t wx, int16_t wy, int16_t wp)
{

    /* OGBUG  movement_mode_cost_maps is not updated: the square keeps the old terrain's movement costs for the rest of the game (the cost maps are built by MAGIC.EXE's world generation and saved with the game; in WIZARDS.EXE only Make_Road, Make_Road_Enchanted and City_Remove_Road write them) */
    SET_TERRAIN_TYPE(wx, wy, wp, tte_Mountain);

    Adjust_Map_Flow(wx, wy, wp);

    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) & 0xF8 /*0b11111000*/));  // clear the volcano player idx

}


// WZD o143p07
void Set_Terrain_Type_Hills(int16_t wx, int16_t wy, int16_t wp)
{

    /* OGBUG  movement_mode_cost_maps is not updated: the square keeps the old terrain's movement costs for the rest of the game (the cost maps are built by MAGIC.EXE's world generation and saved with the game; in WIZARDS.EXE only Make_Road, Make_Road_Enchanted and City_Remove_Road write them) */
    SET_TERRAIN_TYPE(wx, wy, wp, tte_Hills);

    Adjust_Map_Flow(wx, wy, wp);

    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) & 0xF8 /*0b11111000*/));  // clear the volcano player idx

}


// WZD o143p08
void Adjust_Map_Flow(int16_t wx, int16_t wy, int16_t wp)
{
    terrtype_seg = (int16_t *)Near_Allocate_First(2560);

    LBX_Load_Data_Static(terrtype_lbx_file__ovr143, 0, (SAMB_ptr)terrtype_seg, 0, 5, 512);

    /* OGBUG  wx - 1 / wx + 1 are passed without wrapping: Adjacent_Flow() classifies the wrapped square but writes at (wy * WORLD_WIDTH) + wx, so for wx = 0 the blend of (59, y) lands on (59, y - 1), for wx = 59 that of (0, y) on (0, y + 1); at the plane corners it lands on the other plane's corner square or just outside _world_maps (its block header / tail slack, as in the OG) */
    Adjacent_Flow(wx, wy, wp);
    Adjacent_Flow(wx - 1, wy - 1, wp);
    Adjacent_Flow(wx, wy - 1, wp);
    Adjacent_Flow(wx + 1, wy - 1, wp);
    Adjacent_Flow(wx - 1, wy, wp);
    Adjacent_Flow(wx + 1, wy, wp);
    Adjacent_Flow(wx - 1, wy + 1, wp);
    Adjacent_Flow(wx, wy + 1, wp);
    Adjacent_Flow(wx + 1, wy + 1, wp);
}


// WZD o143p09
void Adjacent_Flow(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t adj_mask = 0;

    if(wy < 0 || wy >= WORLD_HEIGHT)
    {
        return;
    }

    /* Rough type 3: Desert */
    if(Get_Rough_Type(wx, wy, wp) == rtt_Desert)
    {
        adj_mask = 0;

        if(Get_Rough_Type(wx - 1, wy - 1, wp) != rtt_Desert)
        {
            adj_mask++;
        }
        if(Get_Rough_Type(wx, wy - 1, wp) != rtt_Desert)
        {
            adj_mask += 0x02;
        }
        if(Get_Rough_Type(wx + 1, wy - 1, wp) != rtt_Desert)
        {
            adj_mask += 0x04;
        }
        if(Get_Rough_Type(wx + 1, wy, wp) != rtt_Desert)
        {
            adj_mask += 0x08;
        }
        if(Get_Rough_Type(wx + 1, wy + 1, wp) != rtt_Desert)
        {
            adj_mask += 0x10;
        }
        if(Get_Rough_Type(wx, wy + 1, wp) != rtt_Desert)
        {
            adj_mask += 0x20;
        }
        if(Get_Rough_Type(wx - 1, wy + 1, wp) != rtt_Desert)
        {
            adj_mask += 0x40;
        }
        if(Get_Rough_Type(wx - 1, wy, wp) != rtt_Desert)
        {
            adj_mask += 0x80;
        }

        if(adj_mask > 0)
        {
            p_world_map[wp][wy][wx] =
                terrtype_seg[adj_mask] + (_Desert00001000 - 2);
        }
        else
        {
            p_world_map[wp][wy][wx] = _AllDesert1;
        }
    }

    /* Rough type 6: Tundra */
    if(Get_Rough_Type(wx, wy, wp) == rtt_Tundra)
    {
        adj_mask = 0;

        if(Get_Rough_Type(wx - 1, wy - 1, wp) != rtt_Tundra)
        {
            adj_mask++;
        }
        if(Get_Rough_Type(wx, wy - 1, wp) != rtt_Tundra)
        {
            adj_mask += 0x02;
        }
        if(Get_Rough_Type(wx + 1, wy - 1, wp) != rtt_Tundra)
        {
            adj_mask += 0x04;
        }
        if(Get_Rough_Type(wx + 1, wy, wp) != rtt_Tundra)
        {
            adj_mask += 0x08;
        }
        if(Get_Rough_Type(wx + 1, wy + 1, wp) != rtt_Tundra)
        {
            adj_mask += 0x10;
        }
        if(Get_Rough_Type(wx, wy + 1, wp) != rtt_Tundra)
        {
            adj_mask += 0x20;
        }
        if(Get_Rough_Type(wx - 1, wy + 1, wp) != rtt_Tundra)
        {
            adj_mask += 0x40;
        }
        if(Get_Rough_Type(wx - 1, wy, wp) != rtt_Tundra)
        {
            adj_mask += 0x80;
        }

        if(adj_mask > 0)
        {
            p_world_map[wp][wy][wx] = terrtype_seg[adj_mask] + (_Tundra00001000 - 2);
        }
        else
        {
            p_world_map[wp][wy][wx] = _AllTundra2;
        }
    }

    /* Rough type 5: Mountain */
    if(Get_Rough_Type(wx, wy, wp) == rtt_Mountain)
    {
        adj_mask = 0;

        if(Get_Rough_Type(wx - 1, wy - 1, wp) == rtt_Mountain)
        {
            adj_mask++;
        }
        if(Get_Rough_Type(wx, wy - 1, wp) == rtt_Mountain)
        {
            adj_mask += 0x02;
        }
        if(Get_Rough_Type(wx + 1, wy - 1, wp) == rtt_Mountain)
        {
            adj_mask += 0x04;
        }
        if(Get_Rough_Type(wx + 1, wy, wp) == rtt_Mountain)
        {
            adj_mask += 0x08;
        }
        if(Get_Rough_Type(wx + 1, wy + 1, wp) == rtt_Mountain)
        {
            adj_mask += 0x10;
        }
        if(Get_Rough_Type(wx, wy + 1, wp) == rtt_Mountain)
        {
            adj_mask += 0x20;
        }
        if(Get_Rough_Type(wx - 1, wy + 1, wp) == rtt_Mountain)
        {
            adj_mask += 0x40;
        }
        if(Get_Rough_Type(wx - 1, wy, wp) == rtt_Mountain)
        {
            adj_mask += 0x80;
        }

        if(adj_mask > 0)
        {
            p_world_map[wp][wy][wx] = terrtype_seg[256 + adj_mask];
        }
        else
        {
            p_world_map[wp][wy][wx] = tte_Mountain;
        }
    }

    EMMDATAH_Map();

    /* Rough type 4: Hills */
    if(Get_Rough_Type(wx, wy, wp) == rtt_Hill)
    {
        adj_mask = 0;

        if(Get_Rough_Type(wx - 1, wy - 1, wp) == rtt_Hill)
        {
            adj_mask++;
        }
        if(Get_Rough_Type(wx, wy - 1, wp) == rtt_Hill)
        {
            adj_mask += 0x02;
        }
        if(Get_Rough_Type(wx + 1, wy - 1, wp) == rtt_Hill)
        {
            adj_mask += 0x04;
        }
        if(Get_Rough_Type(wx + 1, wy, wp) == rtt_Hill)
        {
            adj_mask += 0x08;
        }
        if(Get_Rough_Type(wx + 1, wy + 1, wp) == rtt_Hill)
        {
            adj_mask += 0x10;
        }
        if(Get_Rough_Type(wx, wy + 1, wp) == rtt_Hill)
        {
            adj_mask += 0x20;
        }
        if(Get_Rough_Type(wx - 1, wy + 1, wp) == rtt_Hill)
        {
            adj_mask += 0x40;
        }
        if(Get_Rough_Type(wx - 1, wy, wp) == rtt_Hill)
        {
            adj_mask += 0x80;
        }

        if(adj_mask > 0)
        {
            if(terrtype_seg[256 + adj_mask] >= _Mount0010)
            {
                p_world_map[wp][wy][wx] = terrtype_seg[256 + adj_mask] + (_Hills_0010 - _Mount0010);
            }
            else
            {
                p_world_map[wp][wy][wx] = tte_Hills;
            }
        }
        else
        {
            p_world_map[wp][wy][wx] = tte_Hills;
        }
    }

}


// WZD o143p10
int16_t Get_Rough_Type(int16_t wx, int16_t wy, int16_t wp)
{
    uint16_t tile_word = 0;
    int16_t terrain_type = 0;

    if(wx >= WORLD_WIDTH)
    {
        wx -= WORLD_WIDTH;
    }

    if(wx < 0)
    {
        wx += WORLD_WIDTH;
    }

    if(wy < 0 || wy >= WORLD_HEIGHT)
    {
        return rtt_NONE;
    }

    tile_word = *(uint16_t *)(_world_maps + (wp * (WORLD_SIZE * 2)) + (wy * (WORLD_WIDTH * 2)) + (wx << 1));
    terrain_type = (tile_word % _TerType_Count);

    if(terrain_type >= tt_Tundra_1st)
    {
        return rtt_Tundra;
    }

    if(terrain_type == tt_BugGrass)
    {
        return rtt_Lush;
    }

    if(terrain_type >= tt_Shore3_1st)
    {
        return rtt_Ocean;
    }

    if(terrain_type >= tt_4WRiver1)
    {
        return rtt_Lush;
    }

    if(terrain_type >= tt_Shore2_1st)
    {
        return rtt_Ocean;
    }

    if(terrain_type >= tt_Desert_Fst)
    {
        return rtt_Desert;
    }

    if(terrain_type >= tt_Hills_Fst)
    {
        return rtt_Hill;
    }

    if(terrain_type >= tt_Mountains_Fst)
    {
        return rtt_Mountain;
    }

    if(terrain_type >= tt_Rivers_1st)
    {
        return rtt_Lush;
    }

    if(terrain_type >= tt_Lake1)
    {
        return rtt_Ocean;
    }

    if(terrain_type >= tt_RiverM_1st)
    {
        return rtt_Lush;
    }

    if(terrain_type < tt_Grasslands1)
    {
        return rtt_Ocean;
    }

    switch(terrain_type)
    {
        case tt_Grasslands1:
        case tt_Grasslands2:
        case tt_Grasslands3:
        case tt_Grasslands4:
        {
            return rtt_Lush;
        } break;
        case tt_Forest1:
        case tt_Forest2:
        case tt_Forest3:
        {
            return rtt_Lush;
        } break;
        case tt_Mountain1:
        {
            return rtt_Mountain;
        } break;
        case tt_Desert1:
        case tt_Desert2:
        case tt_Desert3:
        case tt_Desert4:
        {
            return rtt_Desert;
        } break;
        case tt_Swamp1:
        case tt_Swamp2:
        case tt_Swamp3:
        {
            return rtt_Lush;
        } break;
        case tt_Tundra1:
        case tt_Tundra2:
        case tt_Tundra3:
        {
            return rtt_Tundra;
        } break;
        case tt_SorceryNode:
        {
            return rtt_NONE;
        } break;
        case tt_NatureNode:
        {
            return rtt_NONE;
        } break;
        case tt_ChaosNode:
        {
            return rtt_NONE;
        } break;
        case tt_Volcano:
        {
            return rtt_NONE;
        } break;
        case tt_Hills1:
        {
            return rtt_Hill;
        } break;
        default:
        {
            return rtt_NONE;
        } break;
    }

}


// WZD o143p11
/*
DEDU: Probably meant to return terrain_special?
styled like others, with returns in the switch cases?
...
should use:
#pragma warning(push)
#pragma warning(disable : 4715)  // not all control paths return a value -- faithful to the asm: only the Random(5) early-out sets AX
int16_t Generate_Ore(int16_t wx, int16_t wy, int16_t wp)
{
    ...
}
#pragma warning(pop)
Other compilers:
GCC/Clang: they'll warn with -Wreturn-type, but neither has -Werror in CMakeLists.txt, so it stays a warning.
Clang: it may also warn about the unknown #pragma warning. That's not an error either, and the other #pragma warning lines in the repo aren't guarded with #ifdef _MSC_VER, so this matches the existing style.
*/
int16_t Generate_Ore(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t ore_idx = 0;
    int16_t terrain_special = 0;

    if(Random(5) > 1)
    {
        /* NOTE(JimBalcomb,20261104): The asm's Random(5) > 1 early-out is xor ax, ax then jmp @@Done, which is a return 0. */
        return ts_NONE;
    }

    ore_idx = Random(20);
    terrain_special = ts_NONE;

    switch(ore_idx)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        terrain_special = ts_Iron;
        break;

    case 6:
    case 7:
    case 8:
        terrain_special = ts_Coal;
        break;

    case 9:
    case 10:
    case 11:
        terrain_special = ts_Silver;
        break;

    case 12:
    case 13:
    case 14:
        terrain_special = ts_Gold;
        break;

    case 15:
    case 16:
        terrain_special = ts_Gems;
        break;

    case 17:
    case 18:
        terrain_special = ts_Mithril;
        break;

    case 19:
    case 20:
        terrain_special = ts_Adamantium;
        break;

    default:
        break;
    }

    /* OGBUG: & 0xE0 keeps only the flag bits (wild game / nightshade / ...); every mineral (ts_Iron..ts_Adamantium = 1-7) is masked off, so no mineral is ever created. Meant: (byte & 0xE0) + terrain_special */
    _map_square_terrain_specials[(wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx] =
        ((_map_square_terrain_specials[(wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx] + terrain_special) & 0xE0);

    /* HACK */  return ts_NONE;
}


// WZD o143p12
/*
    for each name record
    loops through every city
    if it finds a match, it goes to the next name record
*/
void Random_City_Name_By_Race(int16_t race_idx, char * name)
{
    char Local_Name_String[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t city_name_idx = 0;
    int16_t attempts = 0;
    char * city_names_buffer = 0;
    int16_t itr_cities = 0;

    TRACE_ENTER();

    city_names_buffer = (char *)Near_Allocate_First(280);
    LBX_Load_Data_Static(cityname_lbx_file__ovr143, 0, (SAMB_ptr)city_names_buffer, race_idx, 1, 280);

    attempts = 0;
    while(1)
    {
        attempts++;
        city_name_idx = Random(20) - 1;

        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        {
            stu_strcpy((char *)Local_Name_String, (char *)&_CITIES[itr_cities]);
            if(stu_stricmp(Local_Name_String, (char *)(city_names_buffer + city_name_idx * LEN_CITY_NAME)) == 0)
            {
                if(attempts >= 200)
                {
                    LBX_Load_Data_Static(cityname_lbx_file__ovr143, 0, (SAMB_ptr)city_names_buffer, Random(14) - 1, 1, 280);
                }
                else
                {
                    break;
                }
            }
        }

        if(itr_cities == _cities)
        {
            break;
        }
    }

    stu_strcpy(name, (char *)(city_names_buffer + city_name_idx * LEN_CITY_NAME));
    
    TRACE_EXIT();

}


// WZD o143p13
int16_t Depletion_Event(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp, int16_t * terrain_special)
{
    int16_t tries = 0;
    int16_t city_idx = 0;
    int16_t result_city_idx = 0;
    int16_t City_Tries = 0;
    result_city_idx = ST_UNDEFINED;
    /* OGBUG  should use `(result_city_idx == ST_UNDEFINED)`, not `(result_city_idx == ST_FALSE)` */
    for(City_Tries = 0; ((City_Tries < 100) && (result_city_idx == ST_FALSE)); City_Tries++)
    {
        city_idx = Random(_cities) - 1;
        if(_CITIES[city_idx].owner_idx == player_idx)
        {
            for(tries = 0; (tries < 50) && (result_city_idx == ST_UNDEFINED); tries++)
            {
                *wx = Random(5) - 3;
                *wy = Random(5) - 3;

                if(((*wx == -2) && (*wy == -2)) ||
                    ((*wx ==  2) && (*wy == -2)) ||
                    ((*wx == -2) && (*wy ==  2)) ||
                    ((*wx ==  2) && (*wy ==  2)))
                {
                    continue;
                }
                *wx += _CITIES[city_idx].wx;
                *wy += _CITIES[city_idx].wy;
                if(*wx < 0)
                {
                    *wx += WORLD_WIDTH;
                }
                if(*wx >= WORLD_WIDTH)
                {
                    *wx -= WORLD_WIDTH;
                }
                if(*wy <= 0)
                {
                    continue;
                }
                if(*wy >= WORLD_HEIGHT)
                {
                    continue;
                }
                *wp = _CITIES[city_idx].wp;
                if((_map_square_terrain_specials[(*wp * WORLD_SIZE) + (*wy * WORLD_WIDTH) + *wx] & 0x0F) != 0)
                {
                    if((_map_square_terrain_specials[(*wp * WORLD_SIZE) + (*wy * WORLD_WIDTH) + *wx] & 0x0F) < ts_QuorkCrystals)
                    {
                        result_city_idx = city_idx;
                        *terrain_special = (_map_square_terrain_specials[(*wp * WORLD_SIZE) + (*wy * WORLD_WIDTH) + *wx] & 0x0F);
                    }
                }
            }
        }
    }
    return result_city_idx;
}

// WZD o143p14
int16_t New_Mineral_Event(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp, int16_t * terrain_special)
{
    int16_t tries = 0;
    int16_t city_idx = 0;
    int16_t result_city_idx = 0;
    int16_t City_Tries = 0;
    result_city_idx = ST_UNDEFINED;
    /* OGBUG  should use `(result_city_idx == ST_UNDEFINED)`, not `(result_city_idx == ST_FALSE)` */
    for(City_Tries = 0; ((City_Tries < 100) && (result_city_idx == ST_FALSE)); City_Tries++)
    {
        city_idx = Random(_cities) - 1;
        if(_CITIES[city_idx].owner_idx == player_idx)
        {
            for(tries = 0; (tries < 50) && (result_city_idx == ST_UNDEFINED); tries++)
            {
                *wx = Random(5) - 3;
                *wy = Random(5) - 3;
                /* Skip the 4 corner offsets of the 5x5 city catchment area */
                if(
                    ((*wx == -2) && (*wy == -2)) ||
                    ((*wx ==  2) && (*wy == -2)) ||
                    ((*wx == -2) && (*wy ==  2)) ||
                    ((*wx ==  2) && (*wy ==  2))
                )
                {
                    continue;
                }
                *wx += _CITIES[city_idx].wx;
                *wy += _CITIES[city_idx].wy;
                if(*wx < 0)
                {
                    *wx += WORLD_WIDTH;
                }
                if(*wx >= WORLD_WIDTH)
                {
                    *wx -= WORLD_WIDTH;
                }
                if(*wy <= 0)
                {
                    continue;
                }
                if(*wy >= WORLD_HEIGHT)
                {
                    continue;
                }
                *wp = _CITIES[city_idx].wp;
                if(
                    (Get_Rough_Type(*wx, *wy, *wp) == rtt_Hill)
                    ||
                    (Get_Rough_Type(*wx, *wy, *wp) == rtt_Mountain)
                )
                {
                    if((_map_square_terrain_specials[(*wp * WORLD_SIZE) + (*wy * WORLD_WIDTH) + *wx] & 0x0F) == 0)
                    {
                        result_city_idx = city_idx;

                        if(Random(4) == ST_TRUE)
                        {
                            *terrain_special = TS_COAL;
                        }
                        else
                        {
                            *terrain_special = Random(4) + TS_SILVER;
                        }
                    }
                }
            }
        }
    }
    return result_city_idx;
}


// WZD o143p15
// drake178: TILE_ReplaceMinerals()
/*
; removes all minerals from the square, replacing them
; with the passed one, but leaving Wild Games and
; Nightshades intact
*/
/*

*/
void Replace_Minerals(int16_t wx, int16_t wy, int16_t wp, int16_t terain_special)
{
    _map_square_terrain_specials[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] = 
        (_map_square_terrain_specials[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] & 0xF0) | terain_special;

}


// WZD o143p16
// drake178: TILE_RemoveMinerals()
/*
; removes any minerals from the selected square (but not
; Wild Game or Nightshade)
; returns the new terrain special flags for the square
*/
/*
    clears the low-nibble of _map_square_terrain_specials[]
*/
void Clear_Terrain_Specials(int16_t wx, int16_t wy, int16_t wp)
{
    _map_square_terrain_specials[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] &= 0xF0;
}


// WZD o143p17
// drake178: CTY_ResetRoadConns()
void Reset_City_Road_Connection_Bitfields(void)
{
    int16_t candidate_list[NUM_CITIES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t UU_var1A = 0;
    int16_t has_road_connection = 0;
    int16_t City_Tile_Index = 0;
    int16_t candidate_index = 0;
    int16_t candidate_count = 0;
    int16_t UU_var10 = 0;
    int16_t Y_Loop_Var = 0;
    int16_t X_Loop_Var = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t city_wp = 0;
    int16_t city_landmass_idx = 0;
    int16_t dst_city_idx = 0;
    int16_t itr_cities = 0;  // _DI_

    EMMDATAH_Map();

    UU_var1A = 0;
    UU_var10 = WORLD_SIZE;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        candidate_index = 0;
        if(((_cities / 8) + 1) > candidate_index)
        {
            _CITIES[itr_cities].road_connections[candidate_index] = 0;
            candidate_index++;
        }
    }

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        city_wp = _CITIES[itr_cities].wp;
        city_wx = _CITIES[itr_cities].wx;
        city_wy = _CITIES[itr_cities].wy;

        city_landmass_idx = _landmasses[((city_wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)];

        // check if city has any road connection - any adjacent square has the road bit set
        // ¿ BUGBUG:  does not handle world wrapping ?
        has_road_connection = ST_FALSE;
        for(Y_Loop_Var = -1; Y_Loop_Var < 2; Y_Loop_Var++)
        {
            for(X_Loop_Var = -1; X_Loop_Var < 2; X_Loop_Var++)
            {
                if(
                    (X_Loop_Var != 0)
                    ||
                    (Y_Loop_Var != 0)
                )
                {
                    /* OGBUG  x is not wrapped: at city_wx 0 / 59 the dx = -1 / +1 neighbours read the previous / next row */
                    if((_map_square_flags[((city_wp * WORLD_SIZE) + ((city_wy + Y_Loop_Var) * WORLD_WIDTH) + (city_wx + X_Loop_Var))] & 0x08) != 0)  // MSF_ROAD
                    {
                        has_road_connection = ST_TRUE;
                    }
                }
            }
        }

        if(has_road_connection == ST_TRUE)
        {
            // build list of possibly connected cities
            candidate_count = 0;
            for(dst_city_idx = (itr_cities + 1); dst_city_idx < _cities; dst_city_idx++)
            {
                if(_CITIES[itr_cities].wp == _CITIES[dst_city_idx].wp)
                {
                    if(_landmasses[((_CITIES[dst_city_idx].wp * WORLD_SIZE) + (_CITIES[dst_city_idx].wy * WORLD_WIDTH) + _CITIES[dst_city_idx].wx)] == city_landmass_idx)
                    {
                        candidate_list[candidate_count] = dst_city_idx;
                        candidate_count++;
                    }
                }
            }

            if(candidate_count > 0)
            {
                // build a movepath_cost_map
                // map squares without a road are set to impassible (-1)
                // map squares with a road and one the same landmass are set to 1 moves2
                // ¿ BUGBUG:  map squares with a road but no on the same land mass ?
                for(Y_Loop_Var = 0; Y_Loop_Var < WORLD_HEIGHT; Y_Loop_Var++)
                {
                    for(X_Loop_Var = 0; X_Loop_Var < WORLD_WIDTH; X_Loop_Var++)
                    {
                        if((_map_square_flags[((city_wp * WORLD_SIZE) + (Y_Loop_Var * WORLD_WIDTH) + X_Loop_Var)] & 0x08) == 0)  // MSF_ROAD
                        {
                            movepath_cost_map->moves2[((Y_Loop_Var * WORLD_WIDTH) + X_Loop_Var)] = -1;
                        }
                        else
                        {
                            if(_landmasses[((city_wp * WORLD_SIZE) + (Y_Loop_Var * WORLD_WIDTH) + X_Loop_Var)] == city_landmass_idx)
                            {
                                movepath_cost_map->moves2[((Y_Loop_Var * WORLD_WIDTH) + X_Loop_Var)] = 1;
                            }
                            // ¿ ignore roads on other landmasses ?
                        }
                    }
                }

                Move_Path_Find(city_wx, city_wy, movepath_cost_map);

                for(candidate_index = 0; candidate_index < candidate_count; candidate_index++)
                {
                    City_Tile_Index = ((_CITIES[candidate_list[candidate_index]].wy * WORLD_WIDTH) + _CITIES[candidate_list[candidate_index]].wx);

                    // if the reach_from value is not the same as the index it means there was a path which means it is connected
                    if(movepath_cost_map->Reach_From[City_Tile_Index] != City_Tile_Index)
                    {
                        // set the road connect bits from boths sides
                        Set_Bit_Field(candidate_list[candidate_index], (uint8_t *)&_CITIES[itr_cities].road_connections[0]);
                        Set_Bit_Field(itr_cities, (uint8_t *)&_CITIES[candidate_list[candidate_index]].road_connections[0]);
                    }

                }

            }

        }

    }

}


// WZD o143p18
// drake178: TILE_ResetRoadConns()
/*
; resets the road connection bitfields of every city
; that is on the same landmass as the specified square
; using the overland pathfinding algorithm
*/
void Reset_Landmass_Road_Connection_Bitfields(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t dst_city_list[NUM_CITIES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t square_landmass = 0;
    int16_t has_adjacent_road = 0;
    int16_t city_square_idx = 0;
    int16_t itr = 0;
    int16_t dst_city_count = 0;
    int16_t niu_world_size = 0;
    int16_t itr_y = 0;
    int16_t itr_x = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t city_wp = 0;
    int16_t city_landmass = 0;
    int16_t dst_city_idx = 0;
    int16_t itr_cities = 0;

    EMMDATAH_Map();

    /* OGBUG  dead store: niu_world_size is never read (asm: mov [bp+UU_var10], e_WORLD_SIZE) */
    niu_world_size = WORLD_SIZE;

    square_landmass = _landmasses[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];


    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        city_wp = _CITIES[itr_cities].wp;
        city_wx = _CITIES[itr_cities].wx;
        city_wy = _CITIES[itr_cities].wy;

        city_landmass = _landmasses[((city_wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)];
                
        if(city_landmass == square_landmass)
        {
            for(itr = 0; itr < ((NUM_CITIES + 7) / 8); itr++)
            {
                _CITIES[itr_cities].road_connections[itr] = 0;
            }
        }
    }

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        city_wp = _CITIES[itr_cities].wp;
        city_wx = _CITIES[itr_cities].wx;
        city_wy = _CITIES[itr_cities].wy;

        city_landmass = _landmasses[((city_wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)];

        if(city_landmass == square_landmass)
        {
            has_adjacent_road = ST_FALSE;
            for(itr_y = -1; itr_y < 2; itr_y++)
            {
                for(itr_x = -1; itr_x < 2; itr_x++)
                {
                    if(
                        (itr_x != 0)
                        ||
                        (itr_y != 0)
                    )
                    {
                        /* OGBUG  x is not wrapped: at city_wx 0 / 59 the dx = -1 / +1 neighbours read the previous / next row */
                        if((_map_square_flags[((city_wp * WORLD_SIZE) + ((city_wy + itr_y) * WORLD_WIDTH) + (city_wx + itr_x))] & MSF_ROAD) != 0)
                        {
                            has_adjacent_road = ST_TRUE;
                        }
                    }
                }
            }

            if(has_adjacent_road == ST_TRUE)
            {
                dst_city_count = 0;
                for(dst_city_idx = (itr_cities + 1); dst_city_idx < _cities; dst_city_idx++)
                {
                    if(
                        (_CITIES[itr_cities].wp == _CITIES[dst_city_idx].wp)
                        &&
                        (_landmasses[((city_wp * WORLD_SIZE) + (_CITIES[dst_city_idx].wy * WORLD_WIDTH) + _CITIES[dst_city_idx].wx)] == city_landmass)
                    )
                    {
                        dst_city_list[dst_city_count] = dst_city_idx;
                        dst_city_count++;
                    }
                }

                if(dst_city_count > 0)
                {
                    for(itr_y = 0; itr_y < WORLD_HEIGHT; itr_y++)
                    {
                        for(itr_x = 0; itr_x < WORLD_WIDTH; itr_x++)
                        {
                            if((_map_square_flags[((city_wp * WORLD_SIZE) + ((itr_y)*WORLD_WIDTH) + (itr_x))] & MSF_ROAD) != 0)
                            {
                                if(_landmasses[((city_wp * WORLD_SIZE) + (itr_y * WORLD_WIDTH) + itr_x)] == city_landmass)
                                {
                                    movepath_cost_map->moves2[((itr_y * WORLD_WIDTH) + itr_x)] = 1;
                                }
                            }
                            else
                            {
                                movepath_cost_map->moves2[((itr_y * WORLD_WIDTH) + itr_x)] = -1;
                            }
                        }
                    }
                }

                Move_Path_Find(city_wx, city_wy, movepath_cost_map);

                for(itr = 0; itr < dst_city_count; itr++)
                {
                    city_square_idx = ((_CITIES[dst_city_list[itr]].wy * WORLD_WIDTH) + _CITIES[dst_city_list[itr]].wx);
                    // if(movepath_cost_map->Reach_From[city_square_idx] == city_square_idx)
                    if(movepath_cost_map->Reach_From[city_square_idx] != city_square_idx)  /* CLAUDE */  /* REBUG */  /* asm ovr143/TILE_ResetRoadConns.asm: cmp Reach_From, city_square_idx / jz skip - the bits are set when the city square was reached */
                    {
                        Set_Bit_Field(dst_city_list[itr], (uint8_t*)&_CITIES[itr_cities].road_connections[0]);
                        Set_Bit_Field(itr_cities, (uint8_t*)&_CITIES[dst_city_list[itr]].road_connections[0]);
                    }
                }

            }

        }

    }

}


// WZD o143p19
// drake178: OVL_GetRoadPath()
/*
; calculates a road building path between the selected
; squares, treating roads as 1 MP, everything else as 2
; returns the path length along with the coordinates,
; or 0 if a valid path can not be found
*/
/*
'Main Screen'
Move_Units()
Prep_Road_Path:
OVL_GetRoadPath(XPos, YPos, destination_x, destination_y, map_p, &movepath_x_array[2], &movepath_y_array[2]);
*/
int16_t OVL_GetRoadPath(int16_t src_wx, int16_t src_wy, int16_t dst_wx, int16_t dst_wy, int16_t wp, int8_t * path_string_x, int8_t * path_string_y)
{
    int16_t Tile_Offset = 0;
    int16_t itr_world_height = 0;
    int16_t itr_world_width = 0;
    int16_t landmass_num = 0;
    int16_t itr1 = 0;
    int16_t itr2 = 0;
    // _DI_itr1__path_length

    EMMDATAH_Map();

    landmass_num = _landmasses[((wp * WORLD_SIZE) + (src_wy * WORLD_WIDTH) + src_wx)];

    if(landmass_num == 0)  /* ; ? 0: None / No Land ? */
    {
        return 0;
    }                

    if(_landmasses[((wp * WORLD_SIZE) + (dst_wy * WORLD_WIDTH) + dst_wx)] != landmass_num)
    {
        return 0;
    }
    
    for(itr_world_height = 0; itr_world_height < WORLD_HEIGHT; itr_world_height++)
    {
        for(itr_world_width = 0; itr_world_width < WORLD_WIDTH; itr_world_width++)
        {
            if(_landmasses[((wp * WORLD_SIZE) + (itr_world_height * WORLD_WIDTH) + itr_world_width)] == landmass_num)
            {
                // if((_map_square_flags[((itr_world_height * WORLD_WIDTH) + itr_world_width)] & MSF_ROAD) != 0)
                if((_map_square_flags[((wp * WORLD_SIZE) + (itr_world_height * WORLD_WIDTH) + itr_world_width)] & MSF_ROAD) != 0)  /* CLAUDE */  /* REBUG */  /* asm: _map_square_flags + wp * e_WORLD_SIZE */
                {
                    movepath_cost_map->moves2[((itr_world_height * WORLD_WIDTH) + itr_world_width)] = 1;
                }
                else
                {
                    movepath_cost_map->moves2[((itr_world_height * WORLD_WIDTH) + itr_world_width)] = 2;
                }
            }
            else
            {
                movepath_cost_map->moves2[((itr_world_height * WORLD_WIDTH) + itr_world_width)] = -1;  // ¿ impassible ?
            }
        }
    }

    if(movepath_cost_map->moves2[((dst_wy * WORLD_WIDTH) + dst_wx)] == -1)
    {
        return 0;
    }

    Move_Path_Find(src_wx, src_wy, &movepath_cost_map[0]);

    itr1 = 0;

    Tile_Offset = ((dst_wy * WORLD_WIDTH) + dst_wx);

    // while(movepath_cost_map->Reach_From[Tile_Offset] == Tile_Offset)
    while(movepath_cost_map->Reach_From[Tile_Offset] != Tile_Offset)  /* CLAUDE */  /* REBUG */  /* asm: cmp ax, [bp+Tile_Offset] / jnz loc_CD80D */
    {
        movepath_cost_map->Reverse_Path[itr1] = Tile_Offset;
        Tile_Offset = movepath_cost_map->Reach_From[Tile_Offset];
        itr1++;
    }

    for(itr2 = 0; itr2 < itr1; itr2++)
    {
        itr_world_width = (movepath_cost_map->Reverse_Path[(itr1 - 1 - itr2)] % WORLD_WIDTH);
        itr_world_height = (movepath_cost_map->Reverse_Path[(itr1 - 1 - itr2)] / WORLD_WIDTH);
        path_string_x[itr2] = (int8_t)itr_world_width;
        path_string_y[itr2] = (int8_t)itr_world_height;
    }

    return itr1;
}
