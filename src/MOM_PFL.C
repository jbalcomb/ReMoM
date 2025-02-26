
#include "MOM.H"

/*
    non-OG-MoM
        needed for refactor to Platform-Layer
*/

int quit_game_flag;

int key_pressed;  // win_KD.C  Keyboard_Status(), Read_Key()
uint16_t scan_code_char_code;  // win_KD.C  Read_Key()

// DELETE uint8_t g_Palette[768];  // ~== IBM-PC/MS-DOS Video Card's hardware VGA-DAC buffer
// DELETE uint8_t g_Palette_XBGR[1024];





// char game_files[] = {
//     "SAVETEST.GAM";
//     "SAVE1.GAM";
//     "SAVE9.GAM"
// };
// Meh?  #define GAME_FILE_COUNT 14
// Meh?  char * game_file_names[GAME_FILE_COUNT] = {
// Meh?      "CONFIG.MOM",
// Meh?      "FONTS.LBX",
// Meh?      "HLPENTRY.LBX",
// Meh?      "MAGIC.SET",
// Meh?      "MAIN.LBX",
// Meh?      "MAINSCRN.LBX",
// Meh?      "MAPBACK.LBX",
// Meh?      "SAVE1.GAM",
// Meh?      "SAVE9.GAM",
// Meh?      "SAVETEST.GAM",
// Meh?      "TERRAIN.LBX",
// Meh?      "UNITS1.LBX",
// Meh?      "UNITS2.LBX",
// Meh?      "VORTEX.LBX"
// Meh?  };
// Meh?  
// Meh?  char Debug_Message_Buffer[4096];
// Meh?  
// Meh?  void Get_Current_Working_Direct(void)
// Meh?  {
// Meh?  
// Meh?      TCHAR tszBuffer[MAX_PATH];
// Meh?      DWORD dwRet;
// Meh?      dwRet = GetCurrentDirectory(MAX_PATH, tszBuffer);
// Meh?      // if(dwRet == 0) { OutputDebugStringA("FAILURE: GetCurrentDirectory(): function failed\n"); }
// Meh?      // if(dwRet > MAX_PATH) { OutputDebugStringA("FAILURE: GetCurrentDirectory(): buffer too short\n"); }
// Meh?  
// Meh?      char Current_Working_Directory[MAX_PATH];
// Meh?      sprintf(Current_Working_Directory, "%s\n", tszBuffer);
// Meh?      // // OutputDebugStringA(Current_Working_Directory);
// Meh?      // // OutputDebugStringA(tszBuffer);  // TCHAR * vs. LPCSTR
// Meh?      // OutputDebugString(tszBuffer);
// Meh?      // // "J:\STU\devel\STU-MoM_Rasm\data"
// Meh?  #ifdef STU_DEBUG
// Meh?      dbg_prn("DEBUG: [%s, %d]: Current Working Directory: %s\n", __FILE__, __LINE__, Current_Working_Directory);
// Meh?  #endif
// Meh?  
// Meh?  }
// Meh?  
// Meh?  // https://www.learnc.net/c-tutorial/c-file-exists/
// Meh?  // return true if the file specified by the filename exists
// Meh?  bool file_exists(const char* filename)
// Meh?  {
// Meh?      sprintf(Debug_Message_Buffer, "filename: %s\n", filename);
// Meh?      OutputDebugStringA(Debug_Message_Buffer);
// Meh?  
// Meh?      FILE* fp = fopen(filename, "r");
// Meh?      bool is_exist = false;
// Meh?      if (fp != NULL)
// Meh?      {
// Meh?          is_exist = true;
// Meh?          fclose(fp); // close the file
// Meh?      }
// Meh?      return is_exist;
// Meh?  }
// Meh?  
// Meh?  void Check_Game_Files(void)
// Meh?  {
// Meh?      int itr_game_file_count;
// Meh?      char game_file_name[30];
// Meh?  
// Meh?      char Current_Working_Directory[4096];
// Meh?      GetCurrentDirectoryA(MAX_PATH, Current_Working_Directory);
// Meh?  
// Meh?      for (itr_game_file_count = 0; itr_game_file_count < GAME_FILE_COUNT; itr_game_file_count++)
// Meh?      {
// Meh?          // strcpy(game_file_name, game_file_names[itr_game_file_count]);
// Meh?          if(!file_exists(game_file_names[itr_game_file_count]))
// Meh?          {
// Meh?              printf("File %s doesn't exist.", game_file_names[itr_game_file_count]);
// Meh?              OutputDebugStringA("File XXXXXXXX.XXX doesn't exist.\n");
// Meh?              char Error_Message_Buffer[4096];
// Meh?              sprintf(Error_Message_Buffer, "Hey, Buddy. Hey!! The file - %s - doesn't exist in %s.\n", game_file_names[itr_game_file_count], Current_Working_Directory);
// Meh?              OutputDebugStringA(Error_Message_Buffer);
// Meh?  
// Meh?  #ifdef STU_DEBUG
// Meh?              dbg_prn("DEBUG: [%s, %d]: File %s doesn't exist.\n", __FILE__, __LINE__, game_file_names[itr_game_file_count]);
// Meh?  #endif
// Meh?  
// Meh?              exit(1);  // ~EXIT_FAILURE
// Meh?          }
// Meh?      }
// Meh?  }
