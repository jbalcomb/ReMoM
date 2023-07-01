#ifndef STU_DBG_H
#define STU_DBG_H

#include <stdio.h>



/*
    ¿ Game-State Debug Variables ?
*/
extern int DBG_Draw_Map_Cities;
/*
    ¿ Game-State Debug Variables ?
*/



extern FILE * Debug_Log_File;

#ifdef STU_DEBUG
#define DLOG(_str_) { do { fprintf(Debug_Log_File, "DEBUG: [%s, %d]: %s\n", __FILE__, __LINE__, (_str_)); fflush(Debug_Log_File); } while(0); }
#else
/* emptiness, when debugging is not enabled */
#define DLOG
#endif



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

void Debug_Log_Startup(void);
void Debug_Log_Shutdown(void);

void dbg_prn(const char * fmt, ...);


#ifdef __cplusplus
}
#endif

#endif /* STU_DBG_H */
