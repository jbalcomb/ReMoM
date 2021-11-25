
#include <DOS.H>
#include <CONIO.H>
#include <STDARG.H>
#include <STDIO.H>


/*
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

void wait_for_any_key(void)
{
    while(!kbhit()){continue;}
    getch();
}
