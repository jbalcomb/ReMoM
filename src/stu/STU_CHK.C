
#include "STU.H"
#include "STU_TYPE.H"
#include "STU_DEF.H"
#include "STU_DBG.H"
#include "STU_UTIL.H"   /* get_datetime() */
#include "STU_CHK.H"

#include "..\MoM.H"



int16_t game_data_captured = STU_FALSE;



/*
// WZD dseg:9CDC
uint8_t * _world_maps;

Allocate_Data_Space()
    _world_maps = (uint8_t *)Allocate_Next_Block(World_Data, 602);  // 602 PR, 9632 B

GET_TERRAIN_TYPE()
SET_TERRAIN_TYPE()

*/

/*
// WZD dseg:9CBC  
struct s_CITY * _CITIES;

Allocate_Data_Space()
    _CITIES = (struct s_CITY*)Allocate_First_Block(World_Data, 714);  // 714 PR, 11424 B

*/

uint8_t STU_CHK__world_maps[(NUM_PLANES * WORLD_WIDTH * WORLD_HEIGHT)];



void Capture_Game_Data(void)
{
    int16_t x = 0;
    int16_t y = 0;
    int16_t p = 0;

    for(p = 0; p < NUM_PLANES; p++)
    {
        for(y = 0; y < WORLD_HEIGHT; y++)
        {
            for(x = 0; x < WORLD_WIDTH; x++)
            {
                STU_CHK__world_maps[((p * WORLD_SIZE) + (y * WORLD_HEIGHT) + x)] = _world_maps[((p * WORLD_SIZE) + (y * WORLD_HEIGHT) + x)];
            }
        }
    }

    game_data_captured = STU_TRUE;

}

void Check_Game_Data(void)
{
    int16_t x = 0;
    int16_t y = 0;
    int16_t p = 0;

    if(game_data_captured == STU_FALSE)
    {
        return;
    }

    for(p = 0; p < NUM_PLANES; p++)
    {
        for(y = 0; y < WORLD_HEIGHT; y++)
        {
            for(x = 0; x < WORLD_WIDTH; x++)
            {
                if(_world_maps[((p * WORLD_SIZE) + (y * WORLD_HEIGHT) + x)] != STU_CHK__world_maps[((p * WORLD_SIZE) + (y * WORLD_HEIGHT) + x)])
                {
                    __debugbreak();
                }
            }
        }
    }

}

void Release_Game_Data(void)
{

    game_data_captured = STU_FALSE;

}