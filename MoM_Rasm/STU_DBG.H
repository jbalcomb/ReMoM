#ifndef STU_DBG_H
#define STU_DBG_H

#include <STDARG.H>  /* va_list; va_arg(), va_end(), va_start() */
#include <STDIO.H>   /* FILE; fclose(), fopen() */


extern char Debug_Log_FileName[];
extern FILE *Debug_Log_File;
extern char DBG_LBX_Filename[];
extern unsigned int DBG_LBX_EntryIndex;

//#define DEBUG 1
#ifdef DEBUG
// //#define DBGLOG(fmt, ...) // no-workie
// // #define DBGLOG(fmt, ...) dlvfprintf(fmt, __VA_ARGS__);
// // #define lDebug(level, fmt, ...) \
// //     do { \
// //         /* if (DEBUG_ENABLED && (debugLevel >= level)) */ \
// //         fprintf((debugFile ? debugFile : stderr), "DEBUG %s[%d] %s() " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
// //     } while(0)
// /// ...a sequence of comma-separated expressions in parentheses can be treated as a single argument
// //#define LOG(args) (printf("LOG: %s:%d ", __FILE__, __LINE__), printf args)
// //#define DBGLOG(args) dlvfprintf(args)
// #define TRACE(x) do { if (DEBUG) dbg_printf x; } while (0)
#define LOGA1(arg1) { \
    do { \
        if (DEBUG) fprintf(Debug_Log_File, "DEBUG: [%s, %d]: %s \n", __FILE__, __LINE__, arg1 ); \
       } while(0) \
}
#else
#define DBGLOG  /* emptiness, when debugging is not enabled */
#endif

// #define DBGLOG() dlvprintf()
// #define LOG(fmt, ...)   dlvfprintf(fmt, __VA_ARGS__);
//#define printf dlvfprintf // no-workie

#ifdef DEBUG
#define HERE(argString) { \
    do { \
        fprintf(Debug_Log_File, "DEBUG: [%s, %d]: %s \n", __FILE__, __LINE__, argString ); \
        fflush(Debug_Log_File); \
    } while(0); \
}
// e.g., DBGLOG_S16("FLIC_HDR.Current_Frame = %u", tmp_SI);
#define DBGLOG_S16(fmt, argFile, argLine, s_int_16) { \
    do { \
        fprintf(Debug_Log_File, fmt, s_int_16 ); \
        fflush(Debug_Log_File); \
    } while(0); \
}
#else
/* emptiness, when debugging is not enabled */
#define HERE  
#define DBGLOG_S16
#endif

// void test_here(const char *argString)
// {
//     do
//     {
// //        if (DEBUG)
// //        {
//             fprintf(Debug_Log_File, "DEBUG: [%s, %d]: %s \n", __FILE__, __LINE__, argString );
// //        }
//     } while(0);
// }


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

int     _Cdecl          printf(const char _FAR *__format, ...);

void dbg_printf(const char *fmt, ...);

void Debug_Log_Startup(void);
void Debug_Log_Shutdown(void);

void dlvfprintf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* STU_DBG_H */
