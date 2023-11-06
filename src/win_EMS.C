
#include "MoX.H"

void EMS_Startup(void)
{
    /*
        ovr159
        // call    j_EMM_Map_VGAFILEH1_5           ; maps in pages 1 - 5 of the VGAFILEH EMM handle ; returns the page frame segment
    */

    _VGAFILEH_seg = (uint8_t*)Allocate_Space( (16384 * 5) );
    memset(_VGAFILEH_seg, 0, (16384 * 5));

}