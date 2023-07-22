#include <windows.h>
#include "MoX.H"  /* ~ MoX - Common */
#include "PoC_Screen.H"
#include "PoC_PFL.H"
#include "PoC_MsWin.hpp"
#include "MoM_main.H"  /* Screen_Control() */
#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

// #define POC_MOM_FONT_FILE "FONTS.LBX"
// #define POC_GAME_FONT_FILE POC_MOM_FONT_FILE
// #define PO_DEFAULT_FONT_FILE "FONTS.LBX"
char POC_MOM_FONT_FILE[] = "FONTS.LBX";
char * POC_GAME_FONT_FILE = &POC_MOM_FONT_FILE[0];  // Create a Pointer to the Character-Array (string) at compile time.

/* compile-time flag */  // #define VIDEO_MODE
#define VIDEO_MODE 3     // vm_Mode_Y         320 x 200 x 1Bpp   6bpp  RGB
// #define VIDEO_MODE 5  // vm_Hi_Res_Hi_Col  640 x 400 x 4Bpp  24bpp  XBGR



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef STU_DEBUG
    Debug_Log_Startup();
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WinMain()\n", __FILE__, __LINE__);
#endif

    /*
        Initialize MoX - SimTex Game Engine (STGE)
        Initialize PFL - The Platform-Layer
    */
    Init_Drivers(VIDEO_MODE, POC_MOM_FONT_FILE);
    Init_Platform(hInstance, nCmdShow);


    // Release_Version();
    // Set_Global_ESC();


    // MoO2  Module: ALLOC  Allocate_Data_Space()
    // MoM_Init_Tables();


    // Load_SAVE_GAM(8);


    // MoO2  Module: LOADER  Load_Pictures()
    // ~== Load_ MGC/WZD _Resources()
    // Load_MGC_Resources();
    // Load_WZD_Resources();
    Load_PoC_Resources();


    Load_Palette(0, -1, 0);
    // VGA_SetShades_Grey0()
    // Set_Button_Down_Offsets(1, 1);
    // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1);  // (color_num, red_min, green_min, blue_min, red_max, green_max, blue_max, step_value)
    Apply_Palette();
    // Fade_In()
    // _current_screen = scr_Main_Screen
    // _players.Banner+17E8h = BNR_Brown
    // Clear_Fields()
    current_screen = scr_PoC_Screen;


    // Loaded_Game_Update_WZD()


    // MoO2  Module: MoX2  Screen_Control()
    // MGC: Menu_Screen_Control()
    // WZD: Main_Screen_Control()
    Screen_Control();


    // MGC: Save_SAVE_GAM(8);  WZD: s01p15_Empty_pFxn()
    // Exit_With_Size()

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: WinMain()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    Debug_Log_Shutdown();
#endif

    return 0;
}
