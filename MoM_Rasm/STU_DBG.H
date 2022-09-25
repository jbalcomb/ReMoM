#ifndef STU_DBG_H
#define STU_DBG_H

// #include <STDARG.H>  /* va_list; va_arg(), va_end(), va_start() */
// #include <STDIO.H>   /* FILE; fclose(), fopen() */
#include <STDIO.H>   /* FILE; */

extern char Debug_Log_FileName[];
extern FILE * Debug_Log_File;


#ifdef DEBUG
#define HERE(argString) { \
    do { \
        fprintf(Debug_Log_File, "DEBUG: [%s, %d]: %s \n", __FILE__, __LINE__, argString ); \
        fflush(Debug_Log_File); \
    } while(0); \
}
#else
/* emptiness, when debugging is not enabled */
#define HERE  
#endif



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

void Debug_Log_Startup(void);
void Debug_Log_Shutdown(void);

void dlvfprintf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* STU_DBG_H */
