
#include <Windows.h>

#include "win_PFL.hpp"

#include "STU/STU.h"

#include "MOX/MOM_Data.h"
#include "MOX/MOX_BASE.h"
#include "MOX/MOX_TYPE.h"
#include "MOX/CFG.h"
#include "MOX/EMM.h"
#include "MOX/EXIT.h"
#include "MOX/Fields.h"
#include "MOX/Fonts.h"
#include "MOX/Graphics.h"
#include "MOX/MOX_SET.h"
#include "MOX/SOUND.h"
#include "MOX/Timer.h"

#include "ALLOC.h"
/* NEWCODE */  // #include "CREDITS.h"
#include "Init.h"
/* NEWCODE */  // #include "INTRO.h"
#include "LOADER.h"
#include "MOM_DBG.h"
#include "MOM_SCR.h"
#include "Settings.h"

#ifdef STU_DEBUG
#include "STU/STU_DBG.h"    /* DLOG() */
#endif



// #define MOM_FONT_FILE "FONTS.LBX"
// #define GAME_FONT_FILE MOM_FONT_FILE
char MOM_FONT_FILE[] = "FONTS.LBX";
char * GAME_FONT_FILE = &MOM_FONT_FILE[0];  // Create a Pointer to the Character-Array (string) at compile time.
// char mom_font_file[] = "FONTS.LBX";
// char* game_font_file = &mom_font_file[0];   // Create a pointer to the string at compile time.



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    char found_file[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char file_name[40] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t DIGI_ID = 0;
    int16_t MIDI_ID = 0;
    int16_t DIGI_DMA = 0;
    int16_t DIGI_IRQ = 0;
    int16_t DIGI_IO = 0;
    int16_t DIGI_DRV = 0;
    int16_t MIDI_IRQ = 0;
    int16_t MIDI_IO = 0;
    int16_t itr_savegams = 0;
    FILE *  file_handle = 0;
    int16_t EMS_Pages_Left = 0;
    int16_t MIDI_DMA = 0;  // _DI_
    int16_t MIDI_DRV = 0;  // _SI_
    SAMB_ptr main_menu_music_seg = 0;  // MGC


    int input_type = 0;
#ifdef STU_DEBUG
    int itr_remap_pal_num = 0;
    int itr_remap_pal_num_index = 0;
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

    /* WZD */ Load_MAGIC_SET();

    MIDI_IO = config_mom.MIDI_IO;
    MIDI_IRQ = config_mom.MIDI_IRQ;
    MIDI_DMA = ST_UNDEFINED;
    MIDI_ID = config_mom.MIDI_ID;

    switch(MIDI_ID)
    {
        case 0:
        {
            MIDI_DRV = SND_NONE;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 1:
        {
            MIDI_DRV = SND_Speaker;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 16:
        {
            MIDI_DRV = SND_AdLib;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 18:
        {
            MIDI_DRV = SND_SB;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 24:
        {
            MIDI_DRV = SND_SBPro;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 25:
        {
            MIDI_DRV = SND_PAS;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 32:
        {
            MIDI_DRV = SND_SBPro2;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 33:
        {
            MIDI_DRV = SND_PAS16;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 34:
        {
            MIDI_DRV = SND_ALG;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 48:
        {
            MIDI_DRV = SND_GMIDI;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 49:
        {
            MIDI_DRV = SND_Roland;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 56:
        {
            MIDI_DRV = SND_GMIDI;
            MIDI_DMA = ST_UNDEFINED;
        } break;
    }

    if(MIDI_DRV == SND_Roland)
    {
        /* NEWCODE */  // DOS_PrintString__STUB(str_Initializing_Roland_Drivers);  // "Initializing Roland Drivers...$"
        Mark_Time();
        Release_Time(20);
    }

    DIGI_IO = config_mom.DIGI_IO;
    DIGI_IRQ = config_mom.DIGI_IRQ;
    DIGI_DMA = config_mom.DIGI_DMA;
    DIGI_ID = config_mom.DIGI_ID;

    switch(DIGI_ID)
    {
        case 0:
        {
            DIGI_DRV = SND_NONE;
            DIGI_IO = ST_UNDEFINED;
            DIGI_IRQ = ST_UNDEFINED;
            DIGI_DMA = ST_UNDEFINED;
        } break;
        case 1:
        {
            DIGI_DRV = SND_Speaker;
            DIGI_IO = ST_UNDEFINED;
            DIGI_IRQ = ST_UNDEFINED;
            DIGI_DMA = ST_UNDEFINED;
        } break;
        case 16:
        {
            DIGI_DRV = SND_NONE;
            DIGI_IO = ST_UNDEFINED;
            DIGI_IRQ = ST_UNDEFINED;
            DIGI_DMA = ST_UNDEFINED;
        } break;
        case 18:
        {
            DIGI_DRV = SND_SB;
        } break;
        case 24:
        {
            DIGI_DRV = SND_SBPro;
        } break;
        case 25:
        {
            DIGI_DRV = SND_PAS;
            DIGI_IO = ST_UNDEFINED;
        } break;
        case 32:  /* 0x20 */
        {
            DIGI_DRV = SND_SBPro2;
            MIDI_IRQ = DIGI_IRQ;
            MIDI_DMA = DIGI_DMA;
        } break;
        case 33:  /* 0x21 */
        {
            DIGI_DRV = SND_PAS16;
            DIGI_IO = ST_UNDEFINED;
        } break;
        case 34:  /* 0x22 */
        {
            DIGI_DRV = SND_ALG;
        } break;
        case 48:
        case 49:
        {
            DIGI_DRV = SND_Roland;
            DIGI_IO = ST_UNDEFINED;
            DIGI_IRQ = ST_UNDEFINED;
            DIGI_DMA = ST_UNDEFINED;
        } break;
        case 56:
        {
            DIGI_DRV = SND_GMIDI;
            DIGI_IO = ST_UNDEFINED;
            DIGI_IRQ = ST_UNDEFINED;
            DIGI_DMA = ST_UNDEFINED;
        } break;
    }

    if(MIDI_DRV == SND_AdLib)
    {
        DIGI_DRV = SND_AdLib;
    }

    /*
        MIDI Driver
        SFX Driver
    */
    if(
        (magic_set.sound_channels > 2)
        ||
        (magic_set.sound_channels < 0)
    )
    {
        magic_set.sound_channels = 0;
    }



    // TODO  MoO2  Check_For_Saved_Games()
    // for(itr_savegams = 1; itr_savegams < 9; itr_savegams++)



    // EMS_SetMinKB(2700);;
    // RAM_SetMinKB(583);


    magic_set.input_type = 1;
    magic_set.sound_channels = 2;
    // Init_Drivers(magic_set.input_type, magic_set.sound_channels, &font_file, MIDI_DRV, MIDI_IO, MIDI_IRQ, MIDI_DMA, SFX_DRV, SFX_IO, SFX_IRQ, SFX_DMA);
    /*
        Initialize MoX - SimTex Game Engine (STGE)
        Initialize PFL - The Platform-Layer
    */
    // Init_Drivers(VIDEO_MODE, MOM_FONT_FILE);
    Init_Drivers(magic_set.input_type, magic_set.sound_channels, MOM_FONT_FILE, MIDI_DRV, MIDI_IO, MIDI_IRQ, MIDI_DMA, DIGI_DRV, DIGI_IO, DIGI_IRQ, DIGI_DMA);



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
    Set_Button_Down_Offsets(1, 1);
    Cycle_Palette_Color__STUB(198, 40, 0, 0, 63, 0, 0, 1);  // (color_num, red_min, green_min, blue_min, red_max, green_max, blue_max, step_value)
    Apply_Palette();
    // Fade_In()
    // _current_screen = scr_Main_Screen
    current_screen = scr_Main_Menu_Screen;
    // _players.Banner+17E8h = BNR_Brown
    // Clear_Fields()
    // DONT  ...WZD assumes SAVE8.GAM was loaded...  Loaded_Game_Update_WZD()
    // GAME_SoM_Cast_By = ST_UNDEFINED



    // MoO2: Draw_Logos()
    // TODO  c0argv == 'J','E','N','N','Y'
    // TODO  j_GAME_PlayIntro()
    /* NEWCODE */  // Draw_Logos();



    // MoO2: Init_Credits() <-| Load_Credits() <-| Main_Menu_Screen()
    // TODO  j_Init_Credits()
    /* NEWCODE */  // Init_Credits();

    Stop_Music__STUB();
    main_menu_music_seg = LBX_Load(music_lbx__main, MUSIC_Main_Menu);
    if(magic_set.background_music == ST_TRUE)
    {
        Play_Sound__WIP(main_menu_music_seg);
    }



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
