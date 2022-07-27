
/*
https://olegkutkov.me/2019/03/25/simple-logger-with-stdout-files-and-syslog-support-for-c-projects-in-linux/
*/

#include <DOS.H>
#include <CONIO.H>   /* getch(), kbhit() */
#include <STDARG.H>  /* va_list; va_arg(), va_end(), va_start() */
#include <STDIO.H>   /* FILE; fclose(), fopen() */
#include <STDLIB.H>

#include "STU_UTIL.H"


char Debug_Log_FileName[] = "DEBUG.LOG";
FILE *Debug_Log_File = NULL;
char Debug_Log_ISO8601_DateTime[21] = "1583-01-01T00:00:00Z";  // earliest possible, compliant ISO-8601 DateTime/timestamp
char DBG_LBX_Filename[13] = {0};
unsigned int DBG_LBX_EntryIndex = 0;

unsigned int DBG_MAINSCRN_000;
unsigned int DBG_MAINSCRN_005;


unsigned int dbg_AX_Orig;
unsigned int dbg_BX_Orig;
unsigned int dbg_CX_Orig;
unsigned int dbg_DX_Orig;
unsigned int dbg_SI_Orig;
unsigned int dbg_DI_Orig;
unsigned int dbg_DS_Orig;
unsigned int dbg_ES_Orig;
unsigned int dbg_AX_Curr;
unsigned int dbg_BX_Curr;
unsigned int dbg_CX_Curr;
unsigned int dbg_DX_Curr;
unsigned int dbg_SI_Curr;
unsigned int dbg_DI_Curr;
unsigned int dbg_DS_Curr;
unsigned int dbg_ES_Curr;


extern char DBG_CallingFunction[256];

unsigned char DBG_gflag_MD_Enabled = 0;
unsigned char DBG_gflag_CSR_Enabled = 0;
unsigned char DBG_gflag_MD_INT_Handler_Installed = 0;
unsigned char DBG_gflag_MD_INT_Handler_MvOnly = 0;
unsigned char DBG_gflag_MD_INT_Handler_MvBtns = 0;
// CDraw called from SCRN or MD_INT ?


/*
STDARG.H
typedef void _FAR *va_list;

#define __size(x) ((sizeof(x)+sizeof(int)-1) & ~(sizeof(int)-1))

#ifdef  __cplusplus
#define va_start(ap, parmN) (ap = ...)
#else
#define va_start(ap, parmN) ((void)((ap) = (va_list)((char _FAR *)(&parmN)+__size(parmN))))
#endif

#define va_arg(ap, type) (*(type _FAR *)(((*(char _FAR *_FAR *)&(ap))+=__size(type))-(__size(type))))
#define va_end(ap)          ((void)0)
*/

/*
STDIO.H
int     _Cdecl _FARFUNC fprintf(FILE _FAR *__stream, const char _FAR *__format, ...);
int     _Cdecl          printf(const char _FAR *__format, ...);
int     _Cdecl _FARFUNC vfprintf(FILE _FAR *__stream, const char _FAR *__format, void _FAR *__arglist);
int     _CType          vprintf(const char _FAR *__format, void _FAR *__arglist);
int     _Cdecl _FARFUNC vsprintf(char _FAR *__buffer, const char _FAR *__format, void _FAR *__arglist);
*/
// NOTE(JimBalcomb): This should no longer be needed, as of 2022-01-10
// int     _Cdecl          printf(const char _FAR *__format, ...)
// //void printf(const char _FAR *__format, ...)
// {
//     va_list args;
//     va_start(args, __format);
//     vfprintf(Debug_Log_File, __format, args);
//     va_end(args);
// }

// extern "C" int fprintf (FILE *__restrict __stream, __const char *__restrict __format, ...)
// {
//     va_list args;
//     va_start(args,__format);
//     int return_status = 0;
//     if (is_gui && (__stream == stdout || __stream == stderr)) {
//         return_status = showMessageInGui(NULL, __format, args);
//     }
//     else {
//         return_status = vfprintf(__stream, __format, args);
//     }
//     va_end(args);
//     return return_status;
// }

void dbg_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void Debug_Log_Startup(void)
{
    Debug_Log_File = fopen(Debug_Log_FileName, "w");

    if(Debug_Log_File == NULL)
    {
        printf("DEBUG [FATAL]: Unable to open log file: %s", Debug_Log_FileName);
        exit(1);
    }

    get_datetime(&Debug_Log_ISO8601_DateTime);

    fprintf(Debug_Log_File, "[%s] DEBUG: %s\n", Debug_Log_ISO8601_DateTime, "BEGIN: Debug Log");
}

void Debug_Log_Shutdown(void)
{
    get_datetime(&Debug_Log_ISO8601_DateTime);
    fprintf(Debug_Log_File, "[%s] DEBUG: %s\n", Debug_Log_ISO8601_DateTime, "END: Debug Log");
    fclose(Debug_Log_File);
}

void print_to_file(const int level, const char* message)
{

    int res = fprintf(Debug_Log_File,
            "[%s] %s\n"
                , "ERROR"
                , message );

    if (res == -1) {
        printf("Unable to write to log file!");
        return;
    }
}
void log_generic(const int level, const char* format, va_list args)
{
    char buffer[256];
    vsprintf(buffer, format, args);
    print_to_file(level, buffer);
}
void log_error(char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_generic(0, format, args);
    va_end(args);
}

void dlvfprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(Debug_Log_File, fmt, args);
    va_end(args);
    /*
        flush output buffer to disk, in case of crash

        fysnc() flush kernel buffers, as well as userspace buffers, on a POSIX system (i.e., NOT Windows)
    */
   fflush(Debug_Log_File);
}


/*
the_c_programming_language_2_1988.pdf
    Page 89 (PDF Page 103)
        ...defined as macros to
        avoid the run-time overhead of a function call per character processed.
        ...
        The preprocessor operator ## provides a way to concatenate actual arguments
        during macro expansion. If a parameter in the replacement text is adjacent
        to a ##, the parameter is replaced by the actual argument, the ## and surrounding
        white space are removed, and the result is re-scanned. For example,
        the macro paste concatenates its two arguments:
            #define paste(front, back) front ## back
        so paste (name, 1) creates the token name1.
        The rules for nested uses of ## are arcane; further details may be found in
        Appendix A.
    Page 254 (PDF Page 268)
    Appendix B
    B7. Variable Argument Lists: <stdarg.h>
    ...
    macro preprocessor                  88, 228-233
    macros with arguments               89
    va_list, va_start,va_arg, va_end    155, 174, 245, 254

BESTWAY:
    https://stackoverflow.com/questions/27663053/variadic-macros-alternative-in-ansi-c
    https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c/1644898#1644898

BCPP31LR, 583/598
https://www.google.com/search?client=firefox-b-1-d&q=C+code+write+debug+to+log+file
https://stackoverflow.com/questions/21758136/write-debug-messages-in-c-to-a-file
https://stackoverflow.com/questions/4288130/debugging-in-c-for-log-file
https://stackoverflow.com/questions/7400418/writing-a-log-file-in-c-c
https://olegkutkov.me/2019/03/25/simple-logger-with-stdout-files-and-syslog-support-for-c-projects-in-linux/
https://www.codeproject.com/Questions/97485/how-to-write-log-file-in-C
https://www.cs.colostate.edu/~fsieker/misc/debug/DEBUG.html
*/
// # define DEBUG
// # if defined DEBUG
// # define LOG(fmt, ...)   debug_log(fmt, __VA_ARGS__);
// # else
// # define LOG             /* empty when debugging disabled */
// # endif
// /*
//     debug_log("you have %d attempts left", numAttempts);
// */
// void debug_log(char *fmt, ...)
// {
// # ifdef DEBUG
//     FILE* pFile = fopen("stu.log", "a");
//     if(pFile != NULL)
//     {
//         va_list args;
//         va_start(args, fmt);
//         vfprintf(pFile, fmt, args);
//         va_end(args);
//         fclose(pFile);
//     }
// # endif
// }
// void debug_print(char *file, unsigned int line, char *fmt, ...)
// {
//     va_list argptr;
//     printf("DEBUG: (%s,%d): ", file, line);
//     va_start(argptr, fmt);
//     vprintf(fmt, argptr);
//     va_end (argptr);
// }
