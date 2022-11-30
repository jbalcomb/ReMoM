
#include "Mox_TYPE.H"

#include "MoX_EXIT.H"

/*
SND_Stop_Music()
MD_Reset()
SND_Shutdown()
EMM_Shutdown()
printf()
QuitToDOS()
*/

#if defined(__DOS16__)
#include "ST_DEF.H"     /* DOS.H: geninterrupt() */
#include "ST_QUIT.H"    /* VGA_SetTextMode() */
#include <CONIO.H>      /* getch() */
#endif
#if defined(__WIN32__)
#include <debugapi.h>  /* OutputDebugString() */
#endif

int16_t MoX_MemFreeWorst_KB = 64000;                    // dseg:3CC2  0xFA00  ? Bytes vs. Paragrphs ? 62.5 KB vs 4000 Pr ? sizeof VGA 320x200 ?


// MGC s05p02
#if defined(__DOS16__)
void MoX_Exit_With_Message(char * string)
{
    //SND_Stop_Music
    MD_Reset();
    //SND_Shutdown
    EMM_Shutdown();
    //CRP_Empty_Exit_Fn3
    // TODO(JimBalcomb): add DrawText("Press any key to continue...")
    getch();  // DEBUG(JimBalcomb): suspend operations, so we can have a look-see at the resultant graphics-mode screen

    VGA_SetTextMode();
    //DOS_QuitWithMsg("argQuitMessage"); // works
    //DOS_QuitWithMsg(argQuitMessage); works when called directly with a char*
    //DOS_QuitWithMsg(argQuitMessage); works when called through GAME_QuitProgram with ""
    //DOS_QuitWithMsg(argQuitMessage); // prints garbage when called through GAME_QuitProgram with char*
    //DOS_QuitWithMsg(&argQuitMessage); // prints garbage
    //DOS_QuitWithMsg(*argQuitMessage); // Error Type mismatch  
    QuitToDOS(string);  // works, on account of now being in C, and using printf()
}
#endif
#if defined(__WIN32__)
void MoX_Exit_With_Message(char * string)
{
    // Reset_System();
    // OutputDebugString(string);  // argument char * incompatible LPCWSTR
    OutputDebugStringA(string);
    // ? WM_QUIT ?
}
#endif


// MGC s05p05
#if defined(__DOS16__)
void MoX_Update_MemFreeWorst_KB(void)
{
    uint16_t MemFree_KB;
    MemFree_KB = MoX_dos_memfree_kb();
    if (MemFree_KB < MoX_MemFreeWorst_KB)
    {
        MoX_MemFreeWorst_KB = MemFree_KB;
    }
}
#endif
#if defined(__WIN32__)
void MoX_Update_MemFreeWorst_KB(void)
{
    MoX_MemFreeWorst_KB = 0;
}
#endif


/*
    MGC  seg006
*/

// TBD  // MGCs06p01
// TBD  #if defined(__DOS16__)
// TBD  void VGA_SetTextMode(void)
// TBD  {
// TBD  asm mov ax, 0x03
// TBD  asm int 0x10
// TBD  }
// TBD  #endif
// TBD  #if defined(__WIN32__)
// TBD  void VGA_SetTextMode(void)
// TBD  {
// TBD  
// TBD  }
// TBD  #endif
// TBD  // MGC s06p02
// TBD  #if defined(__DOS16__)
// TBD  void QuitToDOS(char * quitmsg)
// TBD  {
// TBD      printf("%s\n\n", quitmsg);
// TBD      _AH = 0x4C;  // INT 21,4C - Terminate Process With Return Code
// TBD      geninterrupt(0x21);  // DOS_INT
// TBD  }
// TBD  #endif
// TBD  #if defined(__WIN32__)
// TBD  void QuitToDOS(char * quitmsg)
// TBD  {
// TBD  
// TBD  }
// TBD  #endif

// MGC s06p04
#if defined(__DOS16__)
uint16_t MoX_dos_memfree_kb(void)
{
    _BX = 0xFFFF;
    _AH = 0x48;             // ALLOCMEM
    geninterrupt(0x21);     // DOS_INT
    return( (_BX >> 6) );   // divide by 2^6 = 64  (e.g. 256 pages * 16 = 4096 bytes; 4096 bytes / 64 = 4 KB )
}
#endif
#if defined(__WIN32__)
uint16_t MoX_dos_memfree_kb(void)
{
    return 0;
}
#endif
