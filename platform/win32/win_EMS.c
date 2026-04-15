/**
 * win_EMS.c — EMS memory allocation (Win32 implementation).
 *
 * Allocates the VGA file buffer using malloc, replacing the DOS EMS page frame.
 */

#include "../../platform/include/Platform.h"

#include <stdlib.h>
#include <string.h>



// DELETEME  /* WZD s13p01 */
// DELETEME  void EMS_Startup(void)
// DELETEME  {
// DELETEME      /*
// DELETEME          ovr159
// DELETEME          // call    j_EMM_Map_VGAFILEH1_5           ; maps in pages 1 - 5 of the VGAFILEH EMM handle ; returns the page frame segment
// DELETEME      */
// DELETEME      _VGAFILEH_seg = (uint8_t *)malloc((16384 * 5));
// DELETEME      memset(_VGAFILEH_seg, 0, (16384 * 5));
// DELETEME  }
