#include <Windows.h>

#include "win_PFL.hpp"

#include "MoM.H"  /* Screen_Control() */
#include "MoX.H"  /* ~ MoX - Common */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif



// #define MOM_FONT_FILE "FONTS.LBX"
// #define GAME_FONT_FILE MOM_FONT_FILE
char MOM_FONT_FILE[] = "FONTS.LBX";
char * GAME_FONT_FILE = &MOM_FONT_FILE[0];  // Create a Pointer to the Character-Array (string) at compile time.
// char mom_font_file[] = "FONTS.LBX";
// char* game_font_file = &mom_font_file[0];   // Create a pointer to the string at compile time.



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    char found_file[30];
    int input_type;
#ifdef STU_DEBUG
    int itr_remap_pal_num;
    int itr_remap_pal_num_index;
#endif
#ifdef STU_DEBUG
    Debug_Log_Startup();
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WinMain()\n", __FILE__, __LINE__);
#endif

    Test_Run();


    
    // EMM_Pages_Reserved = 158;


    // if (DIR("CONFIG.MOM", found_file) == 0)
    // {
    //     Exit_With_Message("Run INSTALL to configure MASTER OF MAGIC.");
    // }

    // MoO2  Load_Game_Settings()
    Load_CONFIG_MOM();
    Load_MAGIC_SET();


    /*
        MIDI Driver
        SFX Driver
    */
    // if (magic_set.Snd_Channels > 2 || magic_set.Snd_Channels < 0)
    // {
    //     magic_set.Snd_Channels = 0;
    // }
    // magic_set.Input_Type = 1;


    // TODO  MoO2  Check_For_Saved_Games()


    // EMS_SetMinKB(2700);;
    // RAM_SetMinKB(583);


    // magic_set.Input_Type = 1;
    // magic_set.Snd_Channels = 2;
    // Init_Drivers(magic_set.Input_Type, magic_set.Snd_Channels, &font_file, MIDI_DRV, MIDI_IO, MIDI_IRQ, MIDI_DMA, SFX_DRV, SFX_IO, SFX_IRQ, SFX_DMA);

    /*
        Initialize MoX - SimTex Game Engine (STGE)
        Initialize PFL - The Platform-Layer
    */
    Init_Drivers(VIDEO_MODE, MOM_FONT_FILE);
    Init_Platform(hInstance, nCmdShow);


    Release_Version();
    Enable_Cancel();


    // MoO2  Module: ALLOC  Allocate_Data_Space()
    // MoO2 Allocate_Data_Space()
    // MoM_Tables_Init(6100);  // MGC  6100 PR * 16 B = 97600 bytes
    // MoM_Tables_Init(4600);  // WZD  4600 PR * 16 B = 73600 bytes
    Allocate_Data_Space(6100);
    // IDK(JimBalcomb,20240620): Load_SAVE_GAM(-1);  // Why did I move this here, out of scr_Continue? ~ WZD start-up is 'Continue'?
    // game data hacks are in Screen_Control() under scr_Continue, after Loaded_Game_Update()

    // WZD: Load_SAVE_GAM(8);


    // MoO2  Module: LOADER  Load_Pictures()
    // ~== Load_ MGC/WZD _Resources()
    Load_MGC_Resources();  // MGC s01p04
    Load_WZD_Resources();  // WZD o52p01


    Load_Palette(0, -1, 0);
    Calculate_Remap_Colors();
    // Set_Button_Down_Offsets(1, 1);
    // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1);  // (color_num, red_min, green_min, blue_min, red_max, green_max, blue_max, step_value)
    Apply_Palette();
    // Fade_In()
    // _current_screen = scr_Main_Screen
    current_screen = scr_Main_Menu_Screen;
    // _players.Banner+17E8h = BNR_Brown
    // Clear_Fields()
    // Loaded_Game_Update_WZD()
    // GAME_SoM_Cast_By = ST_UNDEFINED



    // MoO2: Draw_Logos()
    // TODO  c0argv == 'J','E','N','N','Y'
    // TODO  j_GAME_PlayIntro()
    // MoO2: Init_Credits() <-| Load_Credits() <-| Main_Menu_Screen()
    // TODO  j_Init_Credits()
    // TODO  SND_Stop_Music()
    // TODO  MainMenu_Music_Seg = LBX_Load(music_lbx, MUSIC_Main_Menu)
    // TODO  SND_PlayFile(MainMenu_Music_Seg)



    // MoO2  Module: MoX2  Screen_Control()
    // MGC: Menu_Screen_Control()
    // WZD: Main_Screen_Control()
    Screen_Control();


    // MGC: Save_SAVE_GAM(8);  WZD: s01p15_Empty_pFxn();
    // Exit_With_Size()

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: WinMain()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    Debug_Log_Shutdown();
#endif

    return 0;
}
