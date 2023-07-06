/*
    WIZARDS.EXE
    ovr092
*/
#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

#include "MoX_Data.H"
#include "Mox_Util.H"

#include "Explore.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif



// WZD o92p01
// drake178: TILE_VisibilityClear
void Clear_Square_Scouted_Flags(int16_t world_p)
{
    int16_t bit_field_size;
    int16_t itr_bit_field;

    bit_field_size = WORLD_SIZE / 8;

    if(world_p == 0)
    {
        for(itr_bit_field = 0; itr_bit_field < bit_field_size; itr_bit_field++)
        {
            square_scouted_p0[itr_bit_field] = 0;
        }
    }
    else
    {
        for(itr_bit_field = 0; itr_bit_field < bit_field_size; itr_bit_field++)
        {
            square_scouted_p1[itr_bit_field] = 0;
        }
    }
}

// WZD o92p02
// drake178: TILE_IsVisible
int16_t Check_Square_Scouted(int16_t world_x, int16_t world_y, int16_t world_p)
{
    int16_t world_square_idx;
    int16_t square_scouted_flag;
    int16_t square_explored_flag;
    int16_t square_is_scouted;

    square_is_scouted = ST_FALSE;

    world_square_idx = world_y * WORLD_WIDTH + world_x;

    if(world_p == 0)
    {
        square_scouted_flag = Test_Bit_Field(world_square_idx, square_scouted_p0);
    }
    else
    {
        square_scouted_flag = Test_Bit_Field(world_square_idx, square_scouted_p1);
    }

    square_explored_flag = Check_Square_Explored(world_x, world_y, world_p);

    if(square_scouted_flag == ST_TRUE && square_explored_flag == ST_TRUE)
    {
        square_is_scouted = ST_TRUE;
    }

//     if(
//         ((world_x == 18) && (world_y == 12) && (world_p == 0)) ||
//         ((world_x == 19) && (world_y == 15) && (world_p == 0)) ||
//         ((world_x == 20) && (world_y == 15) && (world_p == 0)) ||
//         ((world_x == 19) && (world_y == 16) && (world_p == 0)) ||
//         ((world_x == 20) && (world_y == 16) && (world_p == 0)) ||
//         ((world_x == 19) && (world_y == 17) && (world_p == 0)) ||
//         ((world_x == 18) && (world_y == 18) && (world_p == 0)) ||
//         ((world_x == 19) && (world_y == 18) && (world_p == 0)) ||
//         ((world_x == 18) && (world_y == 21) && (world_p == 0)) ||
//         ((world_x == 25) && (world_y == 11) && (world_p == 0)) ||
//         ((world_x == 30) && (world_y == 11) && (world_p == 0)) ||
//         ((world_x == 24) && (world_y == 13) && (world_p == 0)) ||
//         ((world_x == 26) && (world_y == 13) && (world_p == 0)) ||
//         ((world_x == 24) && (world_y == 16) && (world_p == 0)) ||
//         ((world_x == 25) && (world_y == 16) && (world_p == 0)) ||
//         ((world_x == 29) && (world_y == 16) && (world_p == 0)) ||
//         ((world_x == 25) && (world_y == 17) && (world_p == 0)) ||
//         ((world_x == 25) && (world_y == 18) && (world_p == 0)) ||
//         ((world_x == 23) && (world_y == 19) && (world_p == 0)) ||
//         ((world_x == 23) && (world_y == 14) && (world_p == 0)) ||
//         ((world_x == 23) && (world_y == 17) && (world_p == 0)) ||
//         ((world_x == 22) && (world_y == 20) && (world_p == 0))
//     )
//     {
// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d]: world_x, world_y, world_p: %d %d %d\n", __FILE__, __LINE__, world_x, world_y, world_p);
//         dbg_prn("DEBUG: [%s, %d]: square_scouted_flag: %d\n", __FILE__, __LINE__, square_scouted_flag);
//         dbg_prn("DEBUG: [%s, %d]: square_explored_flag: %d\n", __FILE__, __LINE__, square_explored_flag);
//         dbg_prn("DEBUG: [%s, %d]: square_is_scouted: %d\n", __FILE__, __LINE__, square_is_scouted);
// #endif
//     }
/*
    Map X,Y:    18,11

    Cities:     19,12
                18,17
                23,14
                20,19
    
    Units:      18,12
                19,15
                20,15
                19,16
                20,16
                19,17
                18,18
                19,18
                18,21
                !25,11
                !30,11
                !24,13
                !26,13
                !24,16
                !25,16
                !29,16
                !25,17
                !25,18
                !23,19
*/

    return square_is_scouted;
}

// WZD o92p03
// drake178: TILE_MarkVisible
void Set_Square_Scouted(int16_t world_x, int16_t world_y, int16_t world_p)
{
    int16_t world_square_idx;

    int16_t bit_idx;
    uint8_t * bit_field;
    int16_t byte_idx;
    uint16_t bit_field_byte;
    int16_t byte_bit_idx;
    uint16_t bit_field_test_bit;
    uint16_t new_bit_field_byte;

    world_square_idx = (world_y * WORLD_WIDTH) + world_x;
// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d]: world_square_idx = ((%d * %d) + %d) =  %d\n", __FILE__, __LINE__, world_y, WORLD_WIDTH, world_x, ((world_y * WORLD_WIDTH) + world_x));
// #endif
    bit_idx = world_square_idx;
    bit_field = square_scouted_p0;
    byte_idx = (bit_idx >> 3);
    bit_field_byte = (uint16_t)bit_field[byte_idx];
    byte_bit_idx = (bit_idx & 0x07);
    // bit_field[byte_idx] = (bit_field_byte | bit_field_test_bits[byte_bit_idx]);
    bit_field_test_bit = bit_field_test_bits[byte_bit_idx];
    new_bit_field_byte = (bit_field_byte | bit_field_test_bits[byte_bit_idx]);
// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d]: world_square_idx: %d\n", __FILE__, __LINE__, world_square_idx);
//         dbg_prn("DEBUG: [%s, %d]: bit_idx: %d\n", __FILE__, __LINE__, bit_idx);
//         dbg_prn("DEBUG: [%s, %d]: square_scouted_p0: %p\n", __FILE__, __LINE__, square_scouted_p0);
//         dbg_prn("DEBUG: [%s, %d]: bit_field: %p\n", __FILE__, __LINE__, bit_field);
//         dbg_prn("DEBUG: [%s, %d]: byte_idx: %d\n", __FILE__, __LINE__, byte_idx);
//         dbg_prn("DEBUG: [%s, %d]: bit_field_byte: 0x%02X\n", __FILE__, __LINE__, bit_field_byte);
//         dbg_prn("DEBUG: [%s, %d]: byte_bit_idx: %d\n", __FILE__, __LINE__, byte_bit_idx);
//         dbg_prn("DEBUG: [%s, %d]: bit_field_test_bit: 0x%04X\n", __FILE__, __LINE__, bit_field_test_bit);
//         dbg_prn("DEBUG: [%s, %d]: new_bit_field_byte: 0x%02X\n", __FILE__, __LINE__, new_bit_field_byte);
// #endif


    if(world_p == 0)
    {
        Set_Bit_Field(world_square_idx, square_scouted_p0);
    }
    else
    {
        Set_Bit_Field(world_square_idx, square_scouted_p1);
    }
}

// WZD o92p04
// drake178: UU_TILE_ClearVisible
void UU_Clear_Square_Scouted(int16_t world_x, int16_t world_y, int16_t world_p)
{
    int16_t world_square_idx;

    world_square_idx = (world_y * WORLD_WIDTH) + world_x;

    if(world_p == 0)
    {
        Clear_Bit_Field(world_square_idx, square_scouted_p0);
    }
    else
    {
        Clear_Bit_Field(world_square_idx, square_scouted_p1);
    }
}

// WZD o92p05
// drake178: TILE_VisibilityReset
void Update_Scouted_And_Contacted(void)
{
    int16_t itr_planes;
    int16_t itr_units;
    int16_t itr_cities;
    int16_t curr_world_p;
    int16_t scout_level;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Update_Scouted_And_Contacted()\n", __FILE__, __LINE__);
#endif

    for(itr_planes = 0; itr_planes < NUM_PLANES; itr_planes++)
    {
        curr_world_p = itr_planes;
        Clear_Square_Scouted_Flags(itr_planes);

        if(_players[_human_player_idx].Globals.Nature_Awareness == ST_FALSE)
        {
            for(itr_units = 0; itr_units < _units; itr_units++)
            {
// #ifdef STU_DEBUG
//     if( (_UNITS[itr_units].world_x == 20) && (_UNITS[itr_units].world_y == 16) )
//     {
//     dbg_prn("DEBUG: [%s, %d]: (_UNITS[itr_units].owner_idx == _human_player_idx): %d\n", __FILE__, __LINE__, (_UNITS[itr_units].owner_idx == _human_player_idx));
//     dbg_prn("DEBUG: [%s, %d]: (_UNITS[itr_units].world_plane == curr_world_p): %d\n", __FILE__, __LINE__, (_UNITS[itr_units].world_plane == curr_world_p));
//     dbg_prn("DEBUG: [%s, %d]: (_UNITS[itr_units].In_Tower == ST_TRUE): %d\n", __FILE__, __LINE__, (_UNITS[itr_units].In_Tower == ST_TRUE));
//     dbg_prn("DEBUG: [%s, %d]: ( (_UNITS[itr_units].world_plane == curr_world_p) || (_UNITS[itr_units].In_Tower == ST_TRUE) ): %d\n", __FILE__, __LINE__, ( (_UNITS[itr_units].world_plane == curr_world_p) || (_UNITS[itr_units].In_Tower == ST_TRUE) ));
//     dbg_prn("DEBUG: [%s, %d]: ( (_UNITS[itr_units].owner_idx == _human_player_idx) && ((_UNITS[itr_units].world_plane == curr_world_p) || (_UNITS[itr_units].In_Tower == ST_TRUE)) ): %d\n", __FILE__, __LINE__, ( (_UNITS[itr_units].owner_idx == _human_player_idx) && ((_UNITS[itr_units].world_plane == curr_world_p) || (_UNITS[itr_units].In_Tower == ST_TRUE)) ));
//     }
// #endif
                if( (_UNITS[itr_units].owner_idx == _human_player_idx) && ((_UNITS[itr_units].world_plane == curr_world_p) || (_UNITS[itr_units].In_Tower == ST_TRUE)) )
                {
                    scout_level = _UNITS[itr_units].Sight_Range;
                    // TODO  UNIT_IsFlying
                    // TODO  UNIT_HasWindWalking
                    // TODO  UNIT_ReturnZero
                    // TODO  UNIT_HasItemFlight
                    // if scout_level < 2 scout_level = 2
                    if(_UNITS[itr_units].Status == 1)  /* US_Patrol */
                    {
                        scout_level++;
                    }

                    Set_Square_Scouted_Flags(_UNITS[itr_units].world_x, _UNITS[itr_units].world_y, curr_world_p, scout_level);

                }
                
            }

            for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            {
                if( (_CITIES[itr_cities].owner_idx == _human_player_idx) && ( (_CITIES[itr_cities].world_plane == curr_world_p) || (_CITIES[itr_cities].world_plane == 2) ) )
                {
                    scout_level = 2;

                    if(_CITIES[itr_cities].buildings.City_Walls != -1)  /* B_Not_Built */
                    {
                        scout_level = 3;
                    }
                    if(_CITIES[itr_cities].enchantments.Natures_Eye != ST_FALSE)
                    {
                        scout_level = 5;
                    }
                    // BUG: Overrides Nature's Eye
                    if(_CITIES[itr_cities].buildings.Oracle != -1)  /* B_Not_Built */
                    {
                        scout_level = 4;
                    }

                    Set_Square_Scouted_Flags(_CITIES[itr_cities].world_x, _CITIES[itr_cities].world_y, curr_world_p, scout_level);
                }
            }

            for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            {
                if( (_CITIES[itr_cities].owner_idx != _human_player_idx) && (_CITIES[itr_cities].owner_idx != NEUTRAL_PLAYER_IDX) && ( (_CITIES[itr_cities].world_plane == curr_world_p) || (_CITIES[itr_cities].world_plane == 2) ) )
                {
                    if( (Check_Square_Scouted(_CITIES[itr_cities].world_x, _CITIES[itr_cities].world_y, curr_world_p)) && (_CITIES[itr_cities].owner_idx < 5) )
                    {
                        _players[_human_player_idx].Dipl.Contacted[_CITIES[itr_cities].owner_idx];
                        _players[_CITIES[itr_cities].owner_idx].Dipl.Contacted[_human_player_idx];
                    }
                }
            }

            for(itr_units = 0; itr_units < _units; itr_units++)
            {
                // TODO  UNIT_ContactHuman(itr_units, curr_world_p);
            }

        }
        else
        {
            // TODO  if(_players[_human_player_idx].Globals.Nature_Awareness != ST_FALSE)
        }


    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Update_Scouted_And_Contacted()\n", __FILE__, __LINE__);
#endif
}

// WZD o92p06
// drake178: TILE_ApplySightRange
/*
    within the scout range, sets all explored squares as scouted squares
*/
int16_t Set_Square_Scouted_Flags(int16_t world_x, int16_t world_y, int16_t world_p, int16_t scout_range)
{
    int16_t return_value;
    int16_t start_world_x;
    int16_t start_world_y;
    int16_t itr_world_x;
    int16_t itr_world_y;
    int16_t curr_world_x;
    int16_t square_is_explored;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Set_Square_Scouted_Flags(world_x = %d, world_y = %d, world_p = %d, scout_range = %d)\n", __FILE__, __LINE__, world_x, world_y, world_p, scout_range);
#endif

    if(scout_range == 0)
    {
        return_value = 0;
    }
    else
    {
        start_world_y = world_y - scout_range;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: start_world_y: %d\n", __FILE__, __LINE__, start_world_y);
// #endif
        if(start_world_y < 0)
        {
            start_world_y = 0;
        }
        start_world_x = world_x - scout_range;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: start_world_x: %d\n", __FILE__, __LINE__, start_world_x);
// #endif
        if(start_world_x < 0)
        {
            start_world_x = start_world_x + WORLD_WIDTH;
        }

        // TODO review the ranging here - maybe should be while loop, to exit 1 iteration earlier
        for(itr_world_y = start_world_y; (itr_world_y < (start_world_y + (scout_range * 3) - 1)) && (itr_world_y < WORLD_HEIGHT); itr_world_y++)
        {
            for(itr_world_x = start_world_x; itr_world_x < (start_world_x + (scout_range * 3) - 1); itr_world_x++)
            {
                if(itr_world_x < WORLD_WIDTH)
                {
                    curr_world_x = itr_world_x;
                }
                else
                {
                    curr_world_x = itr_world_x - WORLD_WIDTH;
                }
                square_is_explored = Check_Square_Explored(curr_world_x, itr_world_y, world_p);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: square_is_explored: %d\n", __FILE__, __LINE__, square_is_explored);
// #endif
                if(square_is_explored != ST_FALSE)
                {
                    Set_Square_Scouted(curr_world_x, itr_world_y, world_p);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: Check_Square_Scouted(): %d\n", __FILE__, __LINE__, Check_Square_Scouted(curr_world_x, itr_world_y, world_p));
// #endif
                }
            }
        }
        return_value = itr_world_y;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Set_Square_Scouted_Flags(world_x = %d, world_y = %d, world_p = %d, scout_range = %d)\n", __FILE__, __LINE__, world_x, world_y, world_p, scout_range);
#endif

    return return_value;
}

// WZD o92p07
// drake178: UNIT_ContactHuman

// WZD o92p08
int16_t Check_Square_Explored(int16_t world_x, int16_t world_y, int16_t world_p)
{
    uint8_t square_explored_flag;
    int16_t square_is_explored;

    square_is_explored = ST_FALSE;

    square_explored_flag = TBL_Scouting[( (world_p * WORLD_SIZE) + (world_y * WORLD_WIDTH) + world_x )];

    if(square_explored_flag != ST_FALSE)
    {
        square_is_explored = ST_TRUE;
    }

    return square_is_explored;
}
