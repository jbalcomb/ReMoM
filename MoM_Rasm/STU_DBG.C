
/*
https://olegkutkov.me/2019/03/25/simple-logger-with-stdout-files-and-syslog-support-for-c-projects-in-linux/
*/

#include <DOS.H>
// #include <CONIO.H>   /* getch(), kbhit() */
#include <STDARG.H>  /* va_list; va_arg(), va_end(), va_start() */
#include <STDIO.H>   /* FILE; fclose(), fopen() */
#include <STDLIB.H>  /* exit() */

#include "STU_DBG.H"
#include "STU_UTIL.H"


char Debug_Log_FileName[] = "DEBUG.LOG";
FILE * Debug_Log_File = NULL;
char Debug_Log_ISO8601_DateTime[21] = "1583-01-01T00:00:00Z";  // earliest possible, compliant ISO-8601 DateTime/timestamp


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
    fflush(Debug_Log_File);
}

void Debug_Log_Shutdown(void)
{
    get_datetime(&Debug_Log_ISO8601_DateTime);
    fprintf(Debug_Log_File, "[%s] DEBUG: %s\n", Debug_Log_ISO8601_DateTime, "END: Debug Log");
    fflush(Debug_Log_File);
    fclose(Debug_Log_File);
}

// "dbg" as in "debug"; "prn" as in "print"/"printf";
void dbg_prn(const char * fmt, ...)
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
