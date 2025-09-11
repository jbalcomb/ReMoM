
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/Video.h"
#include "../../MoX/src/Video2.h"

// #ifdef _STU_SDL2
#include "sdl2_PFL.h"
#include <SDL.h>
// #endif

/*
    non-OG-MoM
        needed for refactor to Platform-Layer
*/

int quit_game_flag;

// #ifdef _STU_SDL2
/*
Module: key
char (1 bytes) key_pressed
Address: 02:001B42E4
*/
int8_t key_pressed;
// #endif
// #ifdef _STU_WIN32
// int key_pressed;  // win_KD.C  Keyboard_Status(), Read_Key()
// #endif

uint16_t scan_code_char_code;  // win_KD.C  Read_Key()

// DELETE uint8_t g_Palette[768];  // ~== IBM-PC/MS-DOS Video Card's hardware VGA-DAC buffer
// DELETE uint8_t g_Palette_XBGR[1024];
// ~== IBM-PC/MS-DOS Video Card's hardware VGA-DAC buffer
// #ifdef _STU_SDL2
SDL_Color platform_palette_buffer[256];  // TODO  add manifest constant for platform color count
// #endif
#ifdef _STU_WIN
uint8_t platform_palette_buffer[1024];  // Platform's Shadow Palette: 256 colors * bits/bytes/components per color as required by the platform  e.g., RGB, XBGR, RGBA
#endif



/*
This is platform-layer code.
It is defined here, because it can not be defined in both sdl2_MOM.C and win_MOM.C
It is not used by MOM, MOX, or STU.
*/
// YNM  int16_t platform_mouse_click_x;
// YNM  int16_t platform_mouse_click_y;
// ITRY  int platform_mouse_button_status;
// left, right, middle, x1, x2; wheel;

int lock_mouse_button_status_flag = ST_FALSE;

int platform_mouse_input_enabled = ST_FALSE;  // ¿ ~=== Mouse Driver Installed, for MS-DOS ?



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

/*
    copy MoX software palette to platform *hardware* palette

    MoO2  Refresh_Palett() |-> Store_Palette_Block_()
*/
void Platform_Palette_Update(void)
{
    int itr;

// #ifdef _STU_SDL2
    for (itr = 0; itr < 256; itr++)
    {
        platform_palette_buffer[itr].r = (*(current_palette + (itr * 3) + 0) << 2);
        platform_palette_buffer[itr].g = (*(current_palette + (itr * 3) + 1) << 2);
        platform_palette_buffer[itr].b = (*(current_palette + (itr * 3) + 2) << 2);
        platform_palette_buffer[itr].a = 0xFF;
    }
// #endif
#ifdef _STU_WIN
    for (itr = 0; itr < 256; itr++)  // TODO  ~ #define Color Count
    {
        *(platform_palette_buffer + (itr * 4) + 3) = 0x00;
        *(platform_palette_buffer + (itr * 4) + 2) = (*(current_palette + (itr * 3) + 0) << 2);
        *(platform_palette_buffer + (itr * 4) + 1) = (*(current_palette + (itr * 3) + 1) << 2);
        *(platform_palette_buffer + (itr * 4) + 0) = (*(current_palette + (itr * 3) + 2) << 2);
    }
#endif

}


/*
...just update what the user sees on the screen

¿ always/only 'On-Page' ?

*/
void Platform_Video_Update(void)
{
    uint8_t * dst;
    uint8_t * src;
    int pitch;



    // Why did this get here?  Platform_Palette_Update();  ...should be called on purpose beforehand

    SDL_SetPaletteColors(sdl2_surface_RGB666->format->palette, platform_palette_buffer, 0, 256);



    SDL_LockSurface(sdl2_surface_RGB666);

    pitch = sdl2_surface_RGB666->pitch;
    dst = (uint8_t *)sdl2_surface_RGB666->pixels;
    src = video_page_buffer[draw_page_num];
    for(int y = 0; y < screen_pixel_height; ++y)
    {
        memcpy(dst, src, screen_pixel_width);
        dst += pitch;
        src += screen_pixel_width;
    }

    SDL_UnlockSurface(sdl2_surface_RGB666);



    SDL_LockTexture(sdl2_texture, &sdl2_blit_rect, &sdl2_surface_ARGB8888->pixels, &sdl2_surface_ARGB8888->pitch);

    SDL_LowerBlit(sdl2_surface_RGB666, &sdl2_blit_rect, sdl2_surface_ARGB8888, &sdl2_blit_rect);

    SDL_UnlockTexture(sdl2_texture);

    SDL_RenderCopy(sdl2_renderer, sdl2_texture, NULL, NULL);

    SDL_RenderPresent(sdl2_renderer);

}
