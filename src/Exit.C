// C:\devel\STU-MoM_Rasm\MoM_Rasm\MoX_EXIT-.C
// C:\devel\STU-MoM_Rasm\MoM_Rasm\MoX_EXIT.C


#include "MoX.H"



// C:\devel\STU-MoM_Rasm\MoM_Rasm\MoX_EXIT-.C

#include "MoX_EXIT.H"

#include "MoX_EMM.H"  /* EMM_Pages_Reserved */
#include "MoX_QUIT.H"  /* dos_memfree_kb() */


#include <stdlib.h>
#include <stdio.h>  /* printf() */


int MemFreeWorst_KB = 64000;                    // dseg:3CC2  0xFA00  ? Bytes vs. Paragrphs ? 62.5 KB vs 4000 Pr ? sizeof VGA 320x200 ?
char * cnst_Quit_Report1 = "Data";              // dseg:3CC4
char * cnst_Quit_Report2 = " Free: ";           // dseg:3CC9
char * cnst_Quit_Report3 = " bytes   Memory";   // dseg:3CD1
char * cnst_Quit_Report4 = "k   Worst";         // dseg:3CE1
char * cnst_Quit_Report5 = "k   EMM: ";         // dseg:3CEB
char * cnst_Quit_Report6 = " blocks";           // dseg:3CF5



// s05p01
#if defined(__DOS16__)
void Exit_MemBug(void)
{
    char TmpConvStr20[20];
    char TmpMsgStr120[120];
    unsigned int Current_FreeNearHeap_B;
    unsigned int Worst_LargestFreeBlock_KB;
    unsigned int Current_LargestFreeBlock_KB;
    
    Current_LargestFreeBlock_KB = dos_memfree_kb();
    Worst_LargestFreeBlock_KB = MemFreeWorst_KB;
    Current_FreeNearHeap_B = coreleft();
    strcpy(TmpMsgStr120, cnst_Quit_Report1);
    strcat(TmpMsgStr120, cnst_Quit_Report2);
    ltoa(Current_FreeNearHeap_B, TmpConvStr20, 10);  // Data Free: bytes
    strcat(TmpMsgStr120, TmpConvStr20);
    strcat(TmpMsgStr120, cnst_Quit_Report3);
    strcat(TmpMsgStr120, cnst_Quit_Report2);
    itoa(Current_LargestFreeBlock_KB, TmpConvStr20, 10);  // "Memory Free: k"
    strcat(TmpMsgStr120, TmpConvStr20);
    strcat(TmpMsgStr120, cnst_Quit_Report4);
    strcat(TmpMsgStr120, cnst_Quit_Report2);
    itoa(Worst_LargestFreeBlock_KB, TmpConvStr20, 10);  // "Worst Free k"
    strcat(TmpMsgStr120, TmpConvStr20);
    strcat(TmpMsgStr120, cnst_Quit_Report5);
    itoa(EMM_Pages_Reserved, TmpConvStr20, 10);  // "EMM: blocks"
    strcat(TmpMsgStr120, TmpConvStr20);
    strcat(TmpMsgStr120, cnst_Quit_Report6);

    Exit(TmpMsgStr120);
}
#endif
#if defined(__WIN32__)
void Exit_MemBug(void)
{

}
#endif


// s05p02
#if defined(__DOS16__)
void Exit(char * argQuitMessage)
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
    QuitToDOS(argQuitMessage);  // works, on account of now being in C, and using printf()
}
#endif
#if defined(__WIN32__)
void Exit(char * argQuitMessage)
{

}
#endif

void MoX_Exit_With_Message(char * string)
{
    // Reset_System();
    printf("%s\n", string);
    // exit(EXIT_FAILURE);
}

// s05p05
#if defined(__DOS16__)
void Update_MemFreeWorst_KB(void)
{
    unsigned int MemFree_KB;
    MemFree_KB = dos_memfree_kb();
    if (MemFree_KB < MemFreeWorst_KB)
    {
        MemFreeWorst_KB = MemFree_KB;
    }
}
#endif
#if defined(__WIN32__)
void Update_MemFreeWorst_KB(void)
{

}
#endif



// C:\devel\STU-MoM_Rasm\MoM_Rasm\MoX_EXIT.C

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
