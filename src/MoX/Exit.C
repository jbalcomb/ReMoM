/*
    WIZARDS.EXE
        seg005
        seg006
    MoO2:
        Module: exit
*/

#include "EXIT.H"



int MemFreeWorst_KB = 64000;                    // dseg:3CC2  0xFA00  ? Bytes vs. Paragrphs ? 62.5 KB vs 4000 Pr ? sizeof VGA 320x200 ?
char * cnst_Quit_Report1 = "Data";              // dseg:3CC4
char * cnst_Quit_Report2 = " Free: ";           // dseg:3CC9
char * cnst_Quit_Report3 = " bytes   Memory";   // dseg:3CD1
char * cnst_Quit_Report4 = "k   Worst";         // dseg:3CE1
char * cnst_Quit_Report5 = "k   EMM: ";         // dseg:3CEB
char * cnst_Quit_Report6 = " blocks";           // dseg:3CF5



// s05p01
// TODO  void Exit_With_Size(void)
// TODO  {
// TODO      char temp_string[20];
// TODO      char string[120];
// TODO      unsigned int Current_FreeNearHeap_B;
// TODO      unsigned int Worst_LargestFreeBlock_KB;
// TODO      unsigned int Current_LargestFreeBlock_KB;
// TODO      
// TODO      Current_LargestFreeBlock_KB = dos_memfree_kb();
// TODO      Worst_LargestFreeBlock_KB = MemFreeWorst_KB;
// TODO      Current_FreeNearHeap_B = coreleft();
// TODO      strcpy(string, cnst_Quit_Report1);
// TODO      strcat(string, cnst_Quit_Report2);
// TODO      ltoa(Current_FreeNearHeap_B, temp_string, 10);  // Data Free: bytes
// TODO      strcat(string, temp_string);
// TODO      strcat(string, cnst_Quit_Report3);
// TODO      strcat(string, cnst_Quit_Report2);
// TODO      itoa(Current_LargestFreeBlock_KB, temp_string, 10);  // "Memory Free: k"
// TODO      strcat(string, temp_string);
// TODO      strcat(string, cnst_Quit_Report4);
// TODO      strcat(string, cnst_Quit_Report2);
// TODO      itoa(Worst_LargestFreeBlock_KB, temp_string, 10);  // "Worst Free k"
// TODO      strcat(string, temp_string);
// TODO      strcat(string, cnst_Quit_Report5);
// TODO      itoa(EMM_Pages_Reserved, temp_string, 10);  // "EMM: blocks"
// TODO      strcat(string, temp_string);
// TODO      strcat(string, cnst_Quit_Report6);
// TODO  
// TODO      Exit_With_Message(string);
// TODO  }


// WZD s05p02
/*
    MoO2
        Reset_System();
        exit(EXIT_FAILURE);
*/
// TODO  void Exit_With_Message(char * string)
// TODO  {
// TODO      // TODO  SND_Stop_Music();
// TODO      // TODO  Reset_System_Mouse();
// TODO      // TODO  SND_Shutdown();
// TODO      // TODO  EMM_ReleaseAll();
// TODO      // TODO  s06p06_Empty_pFxn();
// TODO      // TODO  VGA_SetTextMode();
// TODO      Quit_With_Message(string);
// TODO  }
// TODO  void (*Exit_With_Message)(char* string);



// WZD s05p03
// void Exit_With_Value(int16_t value);

// WZD s05p04
// GAME_EXE_Swap()

// WZD s05p05
// Update_MemFreeWorst_KB()

// WZD s05p06
// UU_VGA_B800Dump()

// WZD s05p07
// UU_VGA_TextModeString()



/*
    WIZARDS.EXE  seg006
*/

// WZD s06p01
// VGA_SetTextMode()

// WZD s06p02
// void Quit_With_Message(char * string);

// WZD s06p03
// UU_DOS_PrintString2()

// WZD s06p04
// RAM_GetFreeBlockSize()

// WZD s06p05
// s06p05_Empty_pFxn()

// WZD s06p06
// s06p06_Empty_pFxn()
