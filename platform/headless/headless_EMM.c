/**
 * headless_EMM.c — EMS memory allocation (headless implementation).
 *
 * Allocates the VGA file buffer using malloc, replacing the DOS EMS page frame.
 */

#include "../../platform/include/Platform.h"

#include <stdlib.h>
#include <string.h>



/* WZD s13p01 */
void EMS_Startup(void)
{
    _VGAFILEH_seg = (uint8_t *)malloc((16384 * 5));

    memset(_VGAFILEH_seg, 0, (16384 * 5));
}
