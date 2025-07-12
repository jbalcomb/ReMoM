
#include "MOM.h"
#include <windows.h>

#include <memory.h>

// WZD s13p01
void EMS_Startup(void)
{
    /*
        ovr159
        // call    j_EMM_Map_VGAFILEH1_5           ; maps in pages 1 - 5 of the VGAFILEH EMM handle ; returns the page frame segment
    */

    // _VGAFILEH_seg = (uint8_t*)Allocate_Space( (16384 * 5) );
    // _VGAFILEH_seg = VirtualAlloc(NULL, (16384 * 5), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    _VGAFILEH_seg = (byte_ptr)malloc((16384 * 5));

    memset(_VGAFILEH_seg, 0, (16384 * 5));

}
