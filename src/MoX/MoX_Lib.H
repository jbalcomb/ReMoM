#ifndef MOX_LIB_H
#define MOX_LIB_H

#include "MoX_BASE.H"
#include "MoX_BITS.H"
#include "MoX_DEF.H"
#include "MoX_TYPE.H"

#include "..\MoM_DEF.H"
#include "..\MoM_Data.H"

#include "MoX_Temp.H"

#include "Allocate.H"
#include "Exit.H"
#include "Fields.H"
#include "FLIC_Draw.H"
#include "Fonts.H"
#include "GENDRAW.H"
#include "Graphics.H"
#include "Input.H"
#include "Keyboard.H"
#include "LBX_Load.H"
#include "Mouse.H"
#include "MoX_DBG.H"
#include "Video.H"
#include "Video2.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"    /* DLOG() */
#endif

#include <assert.h>
#include <malloc.h>     /* malloc() */
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* itoa() */
#include <string.h>     /* strcat(), strcpy() */

#endif  /* MOX_LIB_H */
