/*
    WIZARDS.EXE
        ovr143
*/

#include "WZD_o143.H"

#include "MOX/MOM_Data.H"
#include "MOX/MOX_BASE.H"
#include "MOX/MOX_DAT.H"  /* _players[] */
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"
#include "MOX/Allocate.H"
#include "MOX/LBX_Load.H"
#include "MOX/random.H"
#include "MOX/Util.H"

#include "MOM_DEF.H"
#include "MovePath.H"
#include "Terrain.H"
#include "TerrType.H"



// WZD dseg:6EAC BEGIN:  ovr143 - Initialized Data

// WZD dseg:6EAC
int16_t rnd_grasslands[3] = { _Grasslands1, _Grasslands2, _Grasslands3 };
// WZD dseg:6EB2
int16_t rnd_forests[3] = { _Forest1, _Forest2, _Forest3 };
// WZD dseg:6EB8
int16_t rnd_deserts[3] = { _AllDesert2, _AllDesert3, _AllDesert3 };

// WZD dseg:6EBE 
char terrtype_lbx_file__ovr143[] = "TERRTYPE";

// WZD dseg:6EC7
char cityname_lbx_file__ovr143[] = "CITYNAME";

// WZD dseg:6EC7 END:  ovr143 - Initialized Data



/*
    WIZARDS.EXE  ovr143
*/

// WZD o143p01
// drake178: TILE_MakeVolcano()
// MoO2  DNE, but "...generate..." // "...set..."
/*
; turns the tile into a volcano, adjusting the flow of
; adjacent tiles accordingly
;
; BUG: does not update movement costs (fixed in the TERRSTAT patch)
; BUG: fails to create minerals
*/
void Set_Terrain_Type_Volcano_With_Owner(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx)
{

    SET_TERRAIN_TYPE(wx, wy, wp, tt_Volcano);

    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] = (_map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] | (player_idx + 1) );

    // TODO  TILE_AdjustMapFlow(wx, wy, wp);

    // NOTE: as best I can tell, it does actually do exactly this silliness
    *(_map_square_terrain_specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx) &= 0x2F;  /* 0b00101111  clears bits 5,7,8 ...keeps TS_WILDGAME? */
    *(_map_square_terrain_specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx) = 0;

    // ; attempts to generate a new mineral type terrain special on the tile with a 20% chance
    // ; BUG: clears the result after setting it
    // SPELLY  TILE_GenerateOre(wx, wy, wp);

}

// WZD o143p02
// drake178: TILE_CountVolcanoes()
/*

XREF:
    j_TILE_CountVolcanoes()
        Cast_ChangeTerain()
        Cast_ChangeTerain()
        Cast_RaiseVolcano()
        WIZ_Armageddon()
        Apply_City_Changes()
        Cool_Off_Volcanoes()

*/
void Volcano_Counts(void)
{
    int16_t volcano_count_array[10];
    int16_t player_idx;
    int16_t itr_wy;
    int16_t itr_players;  // _CX_
    int16_t itr_wx;  // _DI_
    int16_t itr_wp;  // _SI_

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

                // player_idx = (_map_square_flags[((itr_wp * WORLD_SIZE) + (itr_wy * WORLD_WIDTH) + itr_wx)] & 0x7);
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

    SET_TERRAIN_TYPE(wx, wy, wp, rnd_grasslands[(Random(2) - 1)]);  // ¿ BUGBUG   3 types in the arrays, 4 types overall ?

    // TODO  TILE_AdjustMapFlow(wx, wy, wp);

    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) & 0xF8 /*0b11111000*/));  // clear the volcano player idx

}


// WZD o143p04
void Set_Terrain_Type_Forest(int16_t wx, int16_t wy, int16_t wp)
{

    SET_TERRAIN_TYPE(wx, wy, wp, rnd_forests[(Random(3) - 1)]);

    // TODO  TILE_AdjustMapFlow(wx, wy, wp);

    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) & 0xF8 /*0b11111000*/));  // clear the volcano player idx

}


// WZD o143p05
// drake178: RP_TILE_MakeDesert()
void Set_Terrain_Type_Desert(int16_t wx, int16_t wy, int16_t wp)
{

    SET_TERRAIN_TYPE(wx, wy, wp, rnd_deserts[(Random(3) - 1)]);

    // TODO  TILE_AdjustMapFlow(wx, wy, wp);

    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) & 0xF8 /*0b11111000*/));  // clear the volcano player idx

}


// WZD o143p06
// drake178: TILE_MakeMountain()
/*
; turns the tile into a mountain, adjusting the flow of adjacent tiles accordingly
; BUG: does not update movement costs (fixed in the TERRSTAT patch)
*/
void Set_Terrain_Type_Mountain(int16_t wx, int16_t wy, int16_t wp)
{

    SET_TERRAIN_TYPE(wx, wy, wp, tte_1Mountain1);

    // TODO  TILE_AdjustMapFlow(wx, wy, wp);

    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) & 0xF8 /*0b11111000*/));  // clear the volcano player idx

}


// WZD o143p07
void Set_Terrain_Type_Hills(int16_t wx, int16_t wy, int16_t wp)
{

    SET_TERRAIN_TYPE(wx, wy, wp, tte_1Hills1);

    // TODO  TILE_AdjustMapFlow(wx, wy, wp);

    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) & 0xF8 /*0b11111000*/));  // clear the volcano player idx

}


// WZD o143p08
// TILE_AdjustMapFlow()

// WZD o143p09
// TILE_AdjacentFlow()

// WZD o143p10
// TILE_GetRoughType()

// WZD o143p11
// drake178:  TILE_GenerateOre()
/*

XREF:
    Set_Terrain_Type_Volcano()
    NX_j_TILE_GenerateOre()

*/
void TILE_GenerateOre__STUB(int16_t wx, int16_t wy, int16_t wp)
{

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
    char * city_names_buffer = 0;  // _DI_
    int16_t itr_cities = 0;  // _SI_
    /* hack */ char hack_new_name[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    /* hack */ char * city_names_buffer_ptr = 0;

    city_names_buffer = (char *)Near_Allocate_First(280);

    LBX_Load_Data_Static(cityname_lbx_file__ovr143, 0, (SAMB_ptr)city_names_buffer, race_idx, 1, 280);

    for(attempts = 0; attempts < 200; attempts++)
    {

        city_name_idx = (Random(20) - 1);

        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        {

            strcpy(Local_Name_String, _CITIES[itr_cities].name);

            strcpy(hack_new_name, _CITIES[itr_cities].name);

            // if(stricmp(Local_Name_String, city_names_buffer[(city_name_idx * 14)]) != 0)
            city_names_buffer_ptr = &city_names_buffer[(city_name_idx * LEN_CITY_NAME)];

            // TODO  stricmp() vs. strcmp()  if(stricmp(Local_Name_String, city_names_buffer_ptr) != 0)
            if(strcmp(Local_Name_String, city_names_buffer_ptr) != 0)
            {

                continue;

            }

            if(attempts < 200)
            {

                break;

            }

            LBX_Load_Data_Static(cityname_lbx_file__ovr143, 0, (SAMB_ptr)city_names_buffer, (Random(NUM_RACES) - 1), 1, 280);

        }

    }
    
    // strcpy(name, city_names_buffer[(city_name_idx * 14)]);
    city_names_buffer_ptr = &city_names_buffer[(city_name_idx * LEN_CITY_NAME)];

    strcpy(name, city_names_buffer_ptr);

}


// WZD o143p13
// drake178: EVNT_TargetDepletion()
/*
; attempts to pick a target for the depletion event
; from among the target player's cities that have an
; appropriate resource type in their catchment, setting
; the values at the return pointers accordingly
; returns the city index if successful, or -1 otherwise
;
; BUG: never actually executes, instead always
; returning -1 due to a botched comparison
*/
int16_t EVNT_TargetDepletion__STUB(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp, int16_t * terrain_special)
{
    int16_t Tile_Tries;
    int16_t city_idx;
    int16_t result_city_idx;
    int16_t City_Tries;

    result_city_idx = ST_UNDEFINED;

    // ; BUG: will jump the first time and never execute
    for(City_Tries = 0; ((City_Tries < 100) && (result_city_idx == ST_FALSE)); City_Tries++)
    {
        
    }



    return result_city_idx;

}

// WZD o143p14
// drake178: EVNT_FindNewMineral()
/*
; attempts to pick a target for the new mineral event
; from among the target player's cities, setting the
; values at the return pointers accordingly
; returns the city index if successful, or -1 otherwise
;
; BUG: never actually executes, instead always
; returning -1 due to a botched comparison
*/
int16_t EVNT_FindNewMineral__STUB(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp, int16_t * terrain_special)
{
    int16_t Tile_Tries;
    int16_t city_idx;
    int16_t result_city_idx;
    int16_t City_Tries;

    result_city_idx = ST_UNDEFINED;

    // ; BUG: will jump the first time and never execute
    for(City_Tries = 0; ((City_Tries < 100) && (result_city_idx == ST_FALSE)); City_Tries++)
    {
        
    }



    return result_city_idx;

}

// WZD o143p15
// drake178: TILE_ReplaceMinerals()
/*
; removes all minerals from the tile, replacing them
; with the passed one, but leaving Wild Games and
; Nightshades intact
*/
/*

*/
void TILE_ReplaceMinerals__STUB(int16_t wx, int16_t wy, int16_t wp, int16_t terain_special)
{

}


// WZD o143p16
// drake178: TILE_RemoveMinerals()
/*
; removes any minerals from the selected tile (but not
; Wild Game or Nightshade)
; returns the new terrain special flags for the tile
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

    // TODO  EMM_Map_DataH();

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
; that is on the same landmass as the specified tile
; using the overland pathfinding algorithm
*/
void TILE_ResetRoadConns(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t City_List[100];
    int16_t square_landmass;
    int16_t Has_Road_Out;
    int16_t City_Tile_Index;
    int16_t itr_bits;
    int16_t City_Count;
    int16_t UU_var10;
    int16_t Y_Loop_Var;
    int16_t X_Loop_Var;
    int16_t city_wy;
    int16_t city_wx;
    int16_t city_wp;
    int16_t city_landmass;
    int16_t Dest_City_Index;
    int16_t itr_cities;  // _DI_

    // DONT  EMM_Map_DataH()


    UU_var10 = WORLD_SIZE;

    square_landmass = _landmasses[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];


    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        city_wp = _CITIES[itr_cities].wp;
        city_wx = _CITIES[itr_cities].wx;
        city_wy = _CITIES[itr_cities].wy;

        city_landmass = _landmasses[((city_wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)];
                
        if(city_landmass == square_landmass)
        {
            for(itr_bits = 0; itr_bits < 13; itr_bits++)
            {
                _CITIES[itr_cities].road_connections[itr_bits] = 0;
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
            Has_Road_Out = ST_FALSE;
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
                        if((_map_square_flags[((city_wp * WORLD_SIZE) + ((city_wy + Y_Loop_Var) * WORLD_WIDTH) + (city_wx + X_Loop_Var))] & MSF_ROAD) != 0)
                        {
                            Has_Road_Out = ST_TRUE;
                        }
                    }
                }
            }

            if(Has_Road_Out == ST_TRUE)
            {
                City_Count = 0;
                for(Dest_City_Index = (itr_cities + 1); Dest_City_Index < _cities; Dest_City_Index++)
                {
                    if(
                        (_CITIES[itr_cities].wp == _CITIES[Dest_City_Index].wp)
                        &&
                        (_landmasses[((city_wp * WORLD_SIZE) + (_CITIES[Dest_City_Index].wy * WORLD_WIDTH) + _CITIES[Dest_City_Index].wx)] == city_landmass)
                    )
                    {
                        City_List[City_Count] = Dest_City_Index;
                        City_Count++;
                    }
                }

                if (City_Count > 0)
                {
                    for (Y_Loop_Var = 0; Y_Loop_Var < WORLD_HEIGHT; Y_Loop_Var++)
                    {
                        for (X_Loop_Var = 0; X_Loop_Var < WORLD_WIDTH; X_Loop_Var++)
                        {
                            if ((_map_square_flags[((city_wp * WORLD_SIZE) + ((Y_Loop_Var)*WORLD_WIDTH) + (X_Loop_Var))] & MSF_ROAD) != 0)
                            {
                                if (_landmasses[((city_wp * WORLD_SIZE) + (Y_Loop_Var * WORLD_WIDTH) + X_Loop_Var)] == city_landmass)
                                {
                                    movepath_cost_map->moves2[((Y_Loop_Var * WORLD_WIDTH) + X_Loop_Var)] = 1;
                                }
                            }
                            else
                            {
                                movepath_cost_map->moves2[((Y_Loop_Var * WORLD_WIDTH) + X_Loop_Var)] = -1;
                            }
                        }
                    }
                }

                Move_Path_Find(city_wx, city_wy, movepath_cost_map);

                for (itr_bits = 0; itr_bits < City_Count; itr_bits++)
                {
                    City_Tile_Index = ((_CITIES[City_List[itr_bits]].wy * WORLD_WIDTH) + _CITIES[City_List[itr_bits]].wx);
                    if (movepath_cost_map->Reach_From[City_Tile_Index] == City_Tile_Index)
                    {
                        Set_Bit_Field(City_List[itr_bits], (uint8_t*)&_CITIES[itr_cities].road_connections[0]);
                        Set_Bit_Field(itr_cities, (uint8_t*)&_CITIES[City_List[itr_bits]].road_connections[0]);
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
; tiles, treating roads as 1 MP, everything else as 2
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
    int16_t Tile_Offset;
    int16_t itr_world_height;
    int16_t itr_world_width;
    int16_t landmass_num;
    int16_t itr1;  // _DI_
    int16_t itr2;  // _SI_
    // _DI_itr1__path_length

    // DONT  EMM_Map_DataH();                   ; maps the EMM Data block into the page frame

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
                if((_map_square_flags[((itr_world_height * WORLD_WIDTH) + itr_world_width)] & MSF_ROAD) != 0)
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

    while(movepath_cost_map->Reach_From[Tile_Offset] == Tile_Offset)
    {
        movepath_cost_map->Reverse_Path[itr1] = Tile_Offset;
        Tile_Offset = movepath_cost_map->Reach_From[Tile_Offset];
        itr1++;
    }

    for(itr2 = 0; itr2 < itr1; itr2++)
    {
        itr_world_width = (movepath_cost_map->Reverse_Path[(itr1 - 1 - itr2)] % WORLD_WIDTH);
        itr_world_height = (movepath_cost_map->Reverse_Path[(itr1 - 1 - itr2)] / WORLD_WIDTH);
        path_string_x[itr2] = itr_world_width;
        path_string_y[itr2] = itr_world_height;
    }

    return itr1;
}
