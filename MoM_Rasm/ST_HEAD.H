#ifndef ST_HEAD_H
#define ST_HEAD_H


// #if !defined( __DEFS_H )
// #include <_defs.h>
// #endif


#if defined(__DOS16__)
#include <ALLOC.H>  /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */    
#include <CONIO.H>
// #include <CTYPE.H>
// #include <DIR.H>
#include <DOS.H>        /* FP_OFF(), FP_SEG(), MK_FP() */
// #include <ERRNO.H>
#include <FCNTL.H>
#include <IO.H>
// #include <MEM.H>
// #include <PROCESS.H>
#include <STDIO.H>      /* printf() */
#include <STDLIB.H>     /* ltoa() */
#include <STRING.H>
#include <SYS\STAT.H>
// #include <VALUES.H>
#endif

#include "ST_DEF.H"

#include "ST_EMM.H"
#include "ST_GUI.H"
#include "ST_LBX.H"
#include "ST_SA.H"
#include "ST_TXT.H"
#include "ST_VID.H"


#endif /* ST_HEAD_H */
