
#include "STU.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif
// #include <assert.h>

#include "MAIN_LBX_000.H"
#include "MAIN_LBX_000_00.H"

extern int16_t _unit_stack_count;



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
