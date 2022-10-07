// MOM_DEF.H

#include "seg005.H"

#include "MOM_HEAD.H"
#include "MOM_DEF.H"

#include "ST_EMM.H"
#include "ST_GUI.H"


// s05p01
void Quit_MemBug(void)
{
    char TmpConvStr20[20];
    char TmpMsgStr120[120];
    unsigned int Current_FreeNearHeap_B;
    unsigned int Worst_LargestFreeBlock_KB;
    unsigned int Current_LargestFreeBlock_KB;
    
    Current_LargestFreeBlock_KB = dos_memfree_kb();
    Worst_LargestFreeBlock_KB = g_MemFreeWorst_KB;
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

    Quit(TmpMsgStr120);
}

// s05p02
void Quit(char * argQuitMessage)
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

// s05p05
void Update_MemFreeWorst_KB(void)
{
    unsigned int MemFree_KB;
    MemFree_KB = dos_memfree_kb();
    if (MemFree_KB < g_MemFreeWorst_KB)
    {
        g_MemFreeWorst_KB = MemFree_KB;
    }
}
