
#include "TST_GameState.H"

#include "Mox_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"
#include "MoX_Data.H"

#include "STU_DBG.H"



void Validate_Square_Explored(int16_t world_x, int16_t world_y, int16_t world_p)
{

}

void Validate_Square_Scouted(int16_t world_x, int16_t world_y, int16_t world_p)
{
    int16_t itr_square_scouted;
    int16_t square_is_scouted;
    int16_t itr_world_x;
    int16_t itr_world_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Validate_Square_Scouted(world_x = %d, world_y = %d, world_p = %d)\n", __FILE__, __LINE__, world_x, world_y, world_p);
#endif

    // square_is_scouted = Check_Square_Scouted(entity_world_x, entity_world_y, world_plane);

// #ifdef STU_DEBUG
//                 dbg_prn("entities_on_movement_map[%d] = %d\n", entity_table_idx, entity_idx);
// #endif


#ifdef STU_DEBUG
    if( (world_x == 18) && (world_y == 11) && (world_p == 0) )
    {
        dbg_prn("DEBUG: [%s, %d]: Yay - Test Case Target\n", __FILE__, __LINE__);
    }
    else
    {
        dbg_prn("DEBUG: [%s, %d]: Nay - Test Case Target\n", __FILE__, __LINE__);
    }
#endif

    if( (world_x == 18) && (world_y == 11) && (world_p == 0) )
    {
        for(itr_square_scouted = 0; itr_square_scouted < 300; itr_square_scouted++)
        {
#ifdef STU_DEBUG
            dbg_prn("square_scouted_p0[%d]: 0x%02X\n", itr_square_scouted, square_scouted_p0[itr_square_scouted]);
#endif
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Validate_Square_Scouted(world_x = %d, world_y = %d, world_p = %d)\n", __FILE__, __LINE__, world_x, world_y, world_p);
#endif
}

void Export_Entities_On_Movement_Map(int16_t world_x, int16_t world_y, int16_t world_p)
{

}

void Validate_Entities_On_Movement_Map(int16_t world_x, int16_t world_y, int16_t world_p)
{
    int16_t itr_map_x;
    int16_t itr_map_y;
    int16_t entity_table_idx;
    int16_t entity_idx;
    // int16_t entity_map_x;
    // int16_t entity_map_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Validate_Entities_On_Movement_Map(world_x = %d, world_y = %d, world_p = %d)\n", __FILE__, __LINE__, world_x, world_y, world_p);
#endif

#ifdef STU_DEBUG
    if( (world_x == 18) && (world_y == 11) && (world_p == 0) )
    {
        dbg_prn("DEBUG: [%s, %d]: Yay - Test Case Target\n", __FILE__, __LINE__);
    }
    else
    {
        dbg_prn("DEBUG: [%s, %d]: Nay - Test Case Target\n", __FILE__, __LINE__);
    }
#endif

    if( (world_x == 18) && (world_y == 11) && (world_p == 0) )
    {
        // entity_map_y = 12 - world_y;
        // entity_map_x = 18 - world_x;

        // 18,12  Unit  Yay-Draw
        entity_idx = entities_on_movement_map[ ( ((12 - world_y) * MAP_WIDTH) + (18 - world_x) ) ];
        if(entity_idx == ST_UNDEFINED)
        {
#ifdef STU_DEBUG
            dbg_prn("FAILURE: Yay-Draw: World(18,11) Unit(18,12)\n");
#endif
        }
        else
        {
#ifdef STU_DEBUG
            dbg_prn("SUCCESS: Yay-Draw: World(18,11) Unit(18,12)\n");
#endif
        }
        // 25,11  Unit  Nay-Draw
        entity_idx = entities_on_movement_map[ ( ((11 - world_y) * MAP_WIDTH) + (25 - world_x) ) ];
        if(entity_idx != ST_UNDEFINED)
        {
#ifdef STU_DEBUG
            dbg_prn("FAILURE: Nay-Draw: World(18,11) Unit(25,11)\n");
#endif
        }
        else
        {
#ifdef STU_DEBUG
            dbg_prn("SUCCESS: Nay-Draw: World(18,11) Unit(25,11)\n");
#endif
        }


        for(itr_map_y = 0; itr_map_y < MAP_HEIGHT; itr_map_y++)
        {
            for(itr_map_x = 0; itr_map_x < MAP_WIDTH; itr_map_x++)
            {
                entity_table_idx = (itr_map_y * MAP_WIDTH) + itr_map_x;
                entity_idx = entities_on_movement_map[entity_table_idx];
#ifdef STU_DEBUG
                dbg_prn("entities_on_movement_map[%d] = %d\n", entity_table_idx, entity_idx);
#endif
            }
        }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Validate_Entities_On_Movement_Map(world_x = %d, world_y = %d, world_p = %d)\n", __FILE__, __LINE__, world_x, world_y, world_p);
#endif

}

// void TST_Validate_State_Movement_Map(int16_t world_x, int16_t world_y, int16_t world_p)
// {
//     int16_t itr_world_x;
//     int16_t itr_world_y;
// 
//     if(world_x == 18 && world_y == 11 && world_p == 0)
//     {
//         for(itr_world_y = 0; itr_world_y < world_y + MAP_HEIGHT; itr_world_y++)
//         {
//             for(itr_world_x = 0; itr_world_x < MAP_WIDTH; itr_world_x++)
//             {
// 
// 
//             }
//         }
//     }
// }

// void Initialize_State_Validation_Data(void)
// {
//     struct s_TST_State_Movement_Map Game_States[(18*11)] =
//     {
//         
//     }
// 
// }
