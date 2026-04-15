#include "Platform.h"

#include <stdlib.h>     /* abs(); stu_itoa(); malloc(); */
#include <string.h>     /* memcpy() memset(), stu_strcat(), stu_strcpy(), stricmp() */



// WZD s13p01
void EMS_Startup(void)
{

    _VGAFILEH_seg = (uint8_t *)malloc((16384 * 5));

    memset(_VGAFILEH_seg, 0, (16384 * 5));

}
