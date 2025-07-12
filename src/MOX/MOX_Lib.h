#ifndef MOX_LIB_H
#define MOX_LIB_H

#include "MOM_Data.h"
#include "MOX_BASE.h"
#include "MOX_BITS.h"
#include "MOX_DEF.h"
#include "MOX_TYPE.h"

#include "../MOM_DEF.h"

#include "MOX_T4.h"  // ~ MOX & MOX2 //MoO2

#include "Allocate.h"
#include "CFG.h"
#include "DBG.h"
#include "DIR.h"
#include "DOS.h"
#include "EMM.h"
#include "EXIT.h"
#include "Fields.h"
#include "file_ani.h"
#include "FLIC_Draw.h"
#include "Fonts.h"
#include "GENDRAW.h"
#include "Graphics.h"
#include "Input.h"
#include "IO_Buffer.h"
#include "Keyboard.h"
#include "LBX_Load.h"
#include "LOADSAVE.h"
#include "Mouse.h"
#include "paragrph.h"
#include "random.h"
#include "Timer.h"
#include "Util.h"
#include "Video.h"
#include "Video2.h"

#ifdef STU_DEBUG
// #include "STU/STU_DBG.h"    /* DLOG() */
// #include "STU_DBG.h"    /* DLOG() */
#include "../STU/STU_DBG.h"    /* DLOG() */
#endif

#include <assert.h>
#include <malloc.h>     /* malloc() */
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* itoa() */
#include <string.h>     /* strcat(), strcpy() */

#endif  /* MOX_LIB_H */

// Allocate.C 
// #include "MOX_TYPE.h"   /* SAMB_ptr, etc. */
// #include "MOX_BITS.h"   /* GET_2B_OFS() etc. */
// #include "MOX_BASE.h"    /* ST_FAILURE, etc. */
// #include "MOX_DBG.h"    /* Check_Release_Version() */
// #include "Allocate.h"
// #include <malloc.h>     /* malloc() */
// #include <stdlib.h>     /* itoa() */
// #include <string.h>     /* strcat(), strcpy() */
