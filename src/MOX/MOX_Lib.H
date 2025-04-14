#ifndef MOX_LIB_H
#define MOX_LIB_H

#include "MOM_Data.H"
#include "MOX_BASE.H"
#include "MOX_BITS.H"
#include "MOX_DEF.H"
#include "MOX_TYPE.H"

#include "../MOM_DEF.H"

#include "MOX_T4.H"  // ~ MOX & MOX2 //MoO2

#include "Allocate.H"
#include "CFG.H"
#include "DBG.H"
#include "DIR.H"
#include "DOS.H"
#include "EMM.H"
#include "EXIT.H"
#include "Fields.H"
#include "file_ani.H"
#include "FLIC_Draw.H"
#include "Fonts.H"
#include "GENDRAW.H"
#include "Graphics.H"
#include "Input.H"
#include "IO_Buffer.H"
#include "Keyboard.H"
#include "LBX_Load.H"
#include "LOADSAVE.H"
#include "Mouse.H"
#include "paragrph.H"
#include "random.H"
#include "Timer.H"
#include "Util.H"
#include "Video.H"
#include "Video2.H"

#ifdef STU_DEBUG
// #include "STU/STU_DBG.H"    /* DLOG() */
// #include "STU_DBG.H"    /* DLOG() */
#include "../STU/STU_DBG.H"    /* DLOG() */
#endif

#include <assert.h>
#include <malloc.h>     /* malloc() */
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* itoa() */
#include <string.h>     /* strcat(), strcpy() */

#endif  /* MOX_LIB_H */

// Allocate.C 
// #include "MOX_TYPE.H"   /* SAMB_ptr, etc. */
// #include "MOX_BITS.H"   /* GET_2B_OFS() etc. */
// #include "MOX_BASE.H"    /* ST_FAILURE, etc. */
// #include "MOX_DBG.H"    /* Check_Release_Version() */
// #include "Allocate.H"
// #include <malloc.h>     /* malloc() */
// #include <stdlib.h>     /* itoa() */
// #include <string.h>     /* strcat(), strcpy() */
