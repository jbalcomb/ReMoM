// _s05p01c.c Quit_MemBug
// in MOM_DEF.H

#include "MOM_HEAD.H"


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

    itoa(g_EMM_Pages_Reserved, TmpConvStr20, 10);  // "EMM: blocks"

    strcat(TmpMsgStr120, TmpConvStr20);
    
    strcat(TmpMsgStr120, cnst_Quit_Report6);

    Quit(TmpMsgStr120);

}
