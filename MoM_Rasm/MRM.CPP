/*
    MoM-Rasm Magic
*/

#include <alloc.h>  /* sbrk() */
#include <bios. h>
#include <conio.h>
#include <stdio.h> /* printf() */
#include <time.h>

/*#include "_FHEAP.H"*/  /* _heapbase */
#include "DOS.H"

/* NOTE: `extern` indicates to the linker that this identifier (variable) is to be found elsewhere */
/*extern int StartingValue;*/

/* BEGIN: DOS.H */
/* Variables */
extern  int const _Cdecl _8087;
extern  int       _Cdecl _argc;
extern  char    **_Cdecl _argv;
extern  char    **_Cdecl  environ;

extern  unsigned      _Cdecl _psp;
extern  unsigned      _Cdecl _heaplen;
extern  unsigned char _Cdecl _osmajor;
extern  unsigned char _Cdecl _osminor;
extern  unsigned      _Cdecl _stklen;
extern  unsigned      _Cdecl _fpstklen;
extern  unsigned      _Cdecl _version;
extern  unsigned      _Cdecl _osversion;      /* MSC name for _version */
/* END: DOS.H */
extern unsigned _stklen;
extern unsigned _heaplen;
/*extern unsigned _heapbase;*/

extern "C" {
/*extern int DoTotal(void);*/
/*extern void ReqdMem(int argRequiredMem);*/
/*extern int __cdecl __far ST_DosFindFile(char *argCharPtrFileName, char *argCharPtrDestinationBuffer);*/
extern int ST_DosFindFile(char *argCharPtrFileName, char *argCharPtrDestinationBuffer);
extern int f010109_main();
extern int f040101_DosFindFile(char *argCharPtrFileName, char *argCharPtrDestinationBuffer);
extern int f050207_EXIT_CleanUp();
extern int f060206_DOS_PrintAndExit();
}

/* 'global'/file' scope variables */
#define REQUIRED_MEM 583
int Repetitions;  
int g_RequiredMem;
char * strCONFIG_MOM = "CONFIG.MOM";
char * strRunInstall = "Run INSTALL to configure MASTER OF MAGIC.\n\n";


/* Reduce heaplength and stacklength to make a smaller program in memory. */
/*extern unsigned _heaplen = 1024;*/
/*extern unsigned _stklen = 512;*/
/* Set the stack size to be greater than the default. */
/* This declaration must go in the global data area. */
/*extern unsigned _stklen = 54321U;*/


void print_startup_information()
{
    long bios_time;
    _bios_timeofday(_TIME_GETCLOCK, &bios_time);
    clrscr() ;
    cprintf("The number of clock ticks since midnight is:\r\n");
    cprintf("The number of seconds since midnight is:\r\n");
    cprintf("The number of minutes since midnight is:\r\n");
    cprintf("The number of hours since midnight is:\r\n");
    cprintf("\r\nPress any key to quit:");
    while(!kbhit()) {
        _bios_timeofday(_TIME_GETCLOCK, &bios_time);
        gotoxy (50,1);
        cprintf("%lu", bios_time);
        gotoxy(50,2);
        cprintf("%.4f", bios_time / CLK_TCK);
        gotoxy(50,3);
        cprintf("%.4f", bios_time / CLK_TCK / 60);
        gotoxy(50,4);
        cprintf("%.4f", bios_time / CLK_TCK / 3600);
    }
    
    printf("\n\n\n");
    
    void * heapbase;
    printf("The stack length is: %u\n", _stklen);
    printf("The heap length is: %u\n", _heaplen);
    
    heapbase = sbrk(0);
    printf("The heap base is: 0x%x\n", heapbase);
    
    /*
        _psp is the starting address of the program in memory.
        The top of the stack is the end of the program.
        Using _SS and _SP together we can get the end of the stack.
        (_SS + ((_SP/16) - _psp)
    */
    /*
        _psp
            Function    Contains the segment address of the program segment prefix (PSP) for the
                        current program.
            Syntax      extern unsigned int _psp;
            Declared in dos.h
        The PSP is a DOS process descriptor; it contains initial DOS information
        about the program.
        Refer to the DOS Programmer's Reference Manual for more information on
        the PSP.
    */
    printf("The _psp is: 0x%x\n", _psp);
    
    char far * dta;
    dta = getdta();
    printf("The current disk transfer address is: %Fp\n", dta);
    
}

/*int main(int argc, char * argv[], char * envp[])*/
int main()
{
    print_startup_information();
    
    printf("\n\n\n");
    
    char * varFileName;
    int result;
    
    /*
    int i;
    Repetitions = 10;
    StartingValue = 2;
    printf("i: %d\n", i);
    printf("Repetitions: %d\n", Repetitions);
    printf("StartingValue: %d\n", StartingValue);
    printf("DoTotal(): %d\n", DoTotal());
    */
    
    /*
    printf("REQUIRED_MEM: %d\n", REQUIRED_MEM);
    printf("g_RequiredMem: %d\n", g_RequiredMem);
    ReqdMem(REQUIRED_MEM);
    printf("g_RequiredMem: %d\n", g_RequiredMem);
    */
    
    result = 0xFFFF;
    result = ST_DosFindFile(strCONFIG_MOM, varFileName);
    printf("ST_DosFindFile(): %d\n\n", result);
    
    result = f040101_DosFindFile(strCONFIG_MOM, varFileName);
    printf("f040101_DosFindFile(): %d\n\n", result);
    
    /*result = f010109_main();*/
    /*printf("f010109_main(): %d\n\n", result);*/
    f010109_main();
    
    return 0;
}
