#include "../../MoX/src/EMM.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"

#include "MOM_PFL.h"

#include <stdlib.h>     /* abs(); SDL_itoa(); malloc(); */
#include <string.h>     /* memcpy() memset(), strcat(), strcpy(), stricmp() */



// WZD s13p01
void EMS_Startup(void)
{

    _VGAFILEH_seg = (byte_ptr)malloc((16384 * 5));

    memset(_VGAFILEH_seg, 0, (16384 * 5));

}
