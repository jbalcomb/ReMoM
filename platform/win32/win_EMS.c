/**
 * win_EMS.c — EMS memory allocation (Win32 implementation).
 *
 * Allocates the VGA file buffer using malloc, replacing the DOS EMS page frame.
 */

#include "../../platform/include/Platform.h"

#include <stdlib.h>
#include <string.h>



/* WZD s13p01 */
void EMS_Startup(void)
{
    /*
        ovr159
        // call    j_EMM_Map_VGAFILEH1_5           ; maps in pages 1 - 5 of the VGAFILEH EMM handle ; returns the page frame segment
    */

    _VGAFILEH_seg = (uint8_t *)malloc((16384 * 5));

    memset(_VGAFILEH_seg, 0, (16384 * 5));
}
