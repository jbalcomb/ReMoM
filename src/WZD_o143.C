/*
    WIZARDS.EXE
        ovr143
*/

#include "MoX.H"



// WZD dseg:6EAC BEGIN:  ovr143 - Initialized Data

// WZD dseg:6EAC TBL_Conv_Grassland dw _Grasslands1, _Grasslands2, _Grasslands3
// WZD dseg:6EB2 TBL_Conv_Forest dw _Forest1, _Forest2, _Forest3 ; adjusted in the TERRSTAT patch (A3 -> B4)
// WZD dseg:6EB8 UU_TBL_Conv_Desert dw _AllDesert2, _AllDesert3, _AllDesert3 ; adjusted in the TERRSTAT patch (AE -> A3)
// WZD dseg:6EB8                                         ; the last 4 bytes can be repurposed
// WZD dseg:6EBE terrtype_lbx_file__ovr143 db 'TERRTYPE',0

// WZD dseg:6EC7
char cityname_lbx_file__ovr143[] = "CITYNAME";

// WZD dseg:6EC7 END:  ovr143 - Initialized Data



/*
    WIZARDS.EXE  ovr143
*/

// WZD o143p01
// TILE_MakeVolcano()

// WZD o143p02
// TILE_CountVolcanoes()

// WZD o143p03
// TILE_MakeGrassland()

// WZD o143p04
// TILE_MakeForest()

// WZD o143p05
// RP_TILE_MakeDesert()

// WZD o143p06
// TILE_MakeMountain()

// WZD o143p07
// TILE_MakeHill()

// WZD o143p08
// TILE_AdjustMapFlow()

// WZD o143p09
// TILE_AdjacentFlow()

// WZD o143p10
// TILE_GetRoughType()

// WZD o143p11
// TILE_GenerateOre()

// WZD o143p12
/*
    for each name record
    loops through every city
    if it finds a match, it goes to the next name record

*/
void Random_City_Name_By_Race(int16_t race_idx, char * name)
{
    char Local_Name_String[20];
    int16_t city_name_idx;
    int16_t attempts;
    char * city_names_buffer;  // _DI_
    int16_t itr_cities;  // _SI_
    /* hack */ char hack_new_name[20];
    /* hack */ char * city_names_buffer_ptr;

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
            city_names_buffer_ptr = &city_names_buffer[(city_name_idx * 14)];
            if (stricmp(Local_Name_String, city_names_buffer_ptr) != 0)
            {
                continue;
            }

            if(attempts < 200)
            {
                break;
            }

            LBX_Load_Data_Static(cityname_lbx_file__ovr143, 0, (SAMB_ptr)city_names_buffer, (Random(14) - 1), 1, 280);

        }
    }
    
    // strcpy(name, city_names_buffer[(city_name_idx * 14)]);
    city_names_buffer_ptr = &city_names_buffer[(city_name_idx * 14)];
    strcpy(name, city_names_buffer_ptr);
}


// WZD o143p13
// EVNT_TargetDepletion()

// WZD o143p14
// EVNT_FindNewMineral()

// WZD o143p15
// TILE_ReplaceMinerals()

// WZD o143p16
// TILE_RemoveMinerals()


// WZD o143p17
// drake178: CTY_ResetRoadConns()
void Reset_City_Road_Connection_Bitfields(void)
{
    int16_t candidate_list[NUM_CITIES];
    int16_t UU_var1A;
    int16_t has_road_connection;
    int16_t City_Tile_Index;
    int16_t candidate_index;
    int16_t candidate_count;
    int16_t UU_var10;
    int16_t Y_Loop_Var;
    int16_t X_Loop_Var;
    int16_t city_wy;
    int16_t city_wx;
    int16_t city_wp;
    int16_t city_landmass_idx;
    int16_t dst_city_idx;
    int16_t itr_cities;  // _DI_

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

        city_landmass_idx = TBL_Landmasses[((city_wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)];

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
                    if((TBL_Terrain_Flags[((city_wp * WORLD_SIZE) + ((city_wy + Y_Loop_Var) * WORLD_WIDTH) + (city_wx + X_Loop_Var))] & 0x08) != 0)  // TF_Road
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
                    if(TBL_Landmasses[((_CITIES[dst_city_idx].wp * WORLD_SIZE) + (_CITIES[dst_city_idx].wy * WORLD_WIDTH) + _CITIES[dst_city_idx].wx)] == city_landmass_idx)
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
                        if((TBL_Terrain_Flags[((city_wp * WORLD_SIZE) + (Y_Loop_Var * WORLD_WIDTH) + X_Loop_Var)] & 0x08) == 0)  // TF_Road
                        {
                            movepath_cost_map->moves2[((Y_Loop_Var * WORLD_WIDTH) + X_Loop_Var)] = -1;
                        }
                        else
                        {
                            if(TBL_Landmasses[((city_wp * WORLD_SIZE) + (Y_Loop_Var * WORLD_WIDTH) + X_Loop_Var)] == city_landmass_idx)
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
// TILE_ResetRoadConns()

// WZD o143p19
// OVL_GetRoadPath()
