
#include "STU.H"
#include "MoX.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif
// #include <assert.h>

#include "MAIN_LBX_000.H"
#include "MAIN_LBX_000_00.H"

extern int16_t _unit_stack_count;




void TST_Validate_Font_Color_Block(void)
{
    uint8_t color_index;
    uint8_t palette_index;

    for(color_index = 0; color_index < 16; color_index++)
    {
        palette_index = GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLORS + color_index);
    }
    
}

void TST_Print_Active_Stack(void)
{
    int itr_unit_stack_count;

    // for(itr_unit_stack_count = 0; itr_unit_stack_count < 9; itr_unit_stack_count++)
    for(itr_unit_stack_count = 0; itr_unit_stack_count < _unit_stack_count; itr_unit_stack_count++)
    {
        // _unit_stack[itr_unit_stack_count].unit_idx
        // _unit_stack[itr_unit_stack_count].active
    }

}
void TST_Validate_Entities_On_Map_Window(void)
{

}

void TST_Validate_Allocate_Data_Space(void)
{
    int16_t itr;
    uint8_t * ptr;

    ptr = (uint8_t *)_CITIES;
    for(itr = 0; itr < CITIES_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)_world_maps;
    for(itr = 0; itr < WORLD_MAPS_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)UU_TBL_1;
    for(itr = 0; itr < UU_TBL_1_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)UU_TBL_2;
    for(itr = 0; itr < UU_TBL_2_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)TBL_Landmasses;
    for(itr = 0; itr < LANDMASSES_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)TBL_Terr_Specials;
    for(itr = 0; itr < TERRAIN_SPECIALS_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)TBL_Terrain_Flags;
    for(itr = 0; itr < TERRAIN_FLAGS_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)TBL_Scouting;
    for(itr = 0; itr < UNEXPLORED_AREA_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)movement_mode_cost_maps;
    for(itr = 0; itr < MOVEMAPS_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)p0_heroes;
    for(itr = 0; itr < PLAYER_HEROES_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }
    ptr = (uint8_t *)p1_heroes;
    for(itr = 0; itr < PLAYER_HEROES_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }
    ptr = (uint8_t *)p2_heroes;
    for(itr = 0; itr < PLAYER_HEROES_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }
    ptr = (uint8_t *)p3_heroes;
    for(itr = 0; itr < PLAYER_HEROES_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }
    ptr = (uint8_t *)p4_heroes;
    for(itr = 0; itr < PLAYER_HEROES_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }
    ptr = (uint8_t *)p5_heroes;
    for(itr = 0; itr < PLAYER_HEROES_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)_UNITS;
    for(itr = 0; itr < UNITS_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)_NODES;
    for(itr = 0; itr < NODES_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)_FORTRESSES;
    for(itr = 0; itr < FORTRESSES_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)_TOWERS;
    for(itr = 0; itr < TOWERS_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)_LAIRS;
    for(itr = 0; itr < LAIRS_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }

    ptr = (uint8_t *)events_table;
    for(itr = 0; itr < EVENTS_TABLE_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }
    
    ptr = (uint8_t *)TBL_Hero_Names;
    for(itr = 0; itr < HERO_NAMES_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }
    
    ptr = (uint8_t *)TBL_Items;
    for(itr = 0; itr < ITEMS_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }
    
    ptr = (uint8_t *)TBL_Premade_Items;
    for(itr = 0; itr < PREMADE_ITEMS_RECORD_SIZE; itr++)
    {
        ptr[itr] = 0;
    }
    
}

int Validate_MAIN_LBX_000(uint8_t * main_background)
{
    int itr;
    int valid;

    valid = STU_TRUE;

    for(itr = 0; itr < 12303; itr++)
    {
// #ifdef STU_DEBUG
//     dbg_prn("0x%02X  0x%02X\n", *(main_background + itr), MAIN_LBX_000[itr]);
// #endif

        if( *(main_background + itr) != MAIN_LBX_000[itr] )
        {
            valid = STU_FALSE;
            break;
        }
    }

    return valid;
}

int Validate_MAIN_LBX_000_00(uint8_t * main_background_frame_data)
{
    int itr;
    int valid;

    valid = STU_TRUE;

    for(itr = 0; itr < 12276; itr++)
    {
// #ifdef STU_DEBUG
//     dbg_prn("0x%02X  0x%02X\n", *(main_background_frame_data + itr), MAIN_LBX_000_00[itr]);
// #endif

        if(*(main_background_frame_data + itr) != MAIN_LBX_000_00[itr])
        {
            valid = STU_FALSE;
            break;
        }
    }

    return valid;
}
