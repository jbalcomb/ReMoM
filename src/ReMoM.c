
#ifndef _STU_SDL2
#define _STU_SDL2
#endif

#ifndef STU_DEBUG
#define STU_DEBUG
#endif

#ifndef STU_TRACE
#define STU_TRACE
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "MOX/DIR.h"
#include "MOX/MOX_DEF.h"
#include "MOX/capture.h"
#include "MOX/MOX_T4.h"
#include "MOX/EMM.h"
#include "MOX/MOM_Data.h"
#include "MOX/MOX_BASE.h"
#include "MOX/MOX_TYPE.h"
#include "MOX/CFG.h"
#include "MOX/EMM.h"
#include "MOX/Fields.h"
#include "MOX/Fonts.h"
#include "MOX/Graphics.h"
#include "MOX/LBX_Load.h"
#include "MOX/MOX_SET.h"
#include "MOX/SOUND.h"
#include "MOX/Timer.h"

#include "ALLOC.h"
#include "CREDITS.h"
#include "Init.h"
#include "INTRO.h"
#include "LOADER.h"
#include "MOM_SCR.h"
#include "Settings.h"

#include "STU/STU_DBG.h"

#include <stdlib.h>     /* abs(); itoa(); ltoa(); ultoa(); */

#ifndef _WINDLL
#define HAVE_LIBC
#endif

#define SDL_MAIN_HANDLED
// #include "C:\devellib\SDL2-2.32.2\include\SDL.h"
// #include "C:\devellib\SDL2-2.32.2\include\SDL_stdinc.h"
// #include "C:\devellib\SDL2_mixer-2.8.1\include\SDL_mixer.h"
#include <SDL.h>
#include <SDL_mixer.h>



int MOM_main(int argc, char** argv);
void Startup_Platform(void);
void Shudown_Platform(void);



// #define MOM_FONT_FILE "FONTS.LBX"
// #define GAME_FONT_FILE MOM_FONT_FILE
char MOM_FONT_FILE[] = "FONTS.LBX";
char * GAME_FONT_FILE = &MOM_FONT_FILE[0];  // Create a Pointer to the Character-Array (string) at compile time.
// char mom_font_file[] = "FONTS.LBX";
// char* game_font_file = &mom_font_file[0];   // Create a pointer to the string at compile time.

char CONFIG_FILE[] = "CONFIG.MOM";

/* HACK */  char soundfx_lbx__remom[] = "SOUNDFX";
/* HACK */  char introsfx_lbx__remom[] = "INTROSFX";



// int main(int argc, char * argv[])
int SDL_main(int argc, char* argv[])
{
#ifdef STU_DEBUG
    Debug_Log_Startup();
#endif
#ifdef STU_DEBUG
    Trace_Log_Startup();
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: SDL_main()\n", __FILE__, __LINE__);
#endif

    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    HWND Handle = GetConsoleWindow();
    freopen("CON", "w", stdout);
    printf("Hello from the console!\n");

    Startup_Platform();



    MOM_main(argc, argv);



    Shudown_Platform();

    FreeConsole();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: SDL_main()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    Trace_Log_Shutdown();
#endif
#ifdef STU_DEBUG
    Debug_Log_Shutdown();
#endif

    return 0;

}


int MOM_main(int argc, char** argv)
{
    char found_file[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char file_name[LEN_MAIN_SAVE_FILE_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
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
    SAMB_ptr main_menu_music_seg;  // MGC
    uint32_t main_menu_music_seg_size = 0;  // DNE in Dasm

    EMM_Pages_Reserved = 158;

    if(DIR(str_CONFIG_MOM, found_file) == 0)
    {
        
        /* NEWCODE */  // Exit_With_Message(str_CONFIG_MOM_ERROR);

    }

    file_handle = fopen(str_CONFIG_MOM, str_RB);
    fread(&config_mom, sizeof(struct s_CONFIG_MOM_18), 1, file_handle);
    fclose(file_handle);

    Load_MAGIC_SET();

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
        DOS_PrintString__STUB(str_Initializing_Roland_Drivers);
        Mark_Time();
        Release_Time(2);
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
        case 32:
        {
            DIGI_DRV = SND_SBPro2;
            MIDI_IRQ = DIGI_IRQ;
            MIDI_DMA = DIGI_DMA;
        } break;
        case 33:
        {
            DIGI_DRV = SND_PAS16;
            DIGI_IO = ST_UNDEFINED;
        } break;
        case 34:
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
    if((magic_set.sound_channels > 2) || (magic_set.sound_channels < 0))
    {
        magic_set.sound_channels = 0;
    }

    magic_set.input_type = 1;  // NOTE:  OG-MoM v1.31 is hard-coded to 'keyboard and mouse'

    // ¿ MoO2  Check_For_Saved_Games() ?
    for(itr_savegams = 1; itr_savegams < 9; itr_savegams++)
    {
        if(magic_set.Have_Save[itr_savegams] != ST_FALSE)
        {
            itoa(itr_savegams, found_file, 10);
            strcpy(file_name, str_SAVE_NAME);
            strcat(file_name, found_file);
            strcat(file_name, str_SAVE_EXT);
            DIR(file_name, found_file);
            if(found_file[0] == '\0')
            {
                magic_set.Have_Save[(itr_savegams - 1)] = ST_FALSE;
                strcpy(magic_set.Save_Names[(itr_savegams - 1)], empty_string__MAIN);
                file_handle = fopen(str_MAGIC_SET, str_WB);
                // // // // // fwrite(&magic_set[0], sizeof(struct s_MAGIC_SET), 1, file_handle);
                // // // // fwrite((uint8_t *)&magic_set[0], sizeof(struct s_MAGIC_SET), 1, file_handle);
                // // // fwrite(magic_set, 466, 1, file_handle);
                // // fwrite((uint8_t *)magic_set, 466, 1, file_handle);
                // fwrite((void *)magic_set, 466, 1, file_handle);
                fwrite(&magic_set, 466, 1, file_handle);
                fclose(file_handle);
            }
        }
    }

    /* NEWCODE */  // EMM_Set_Minimum(2700);
    /* NEWCODE */  // RAM_SetMinKB(583);
    magic_set.input_type = 1;
    magic_set.sound_channels = 2;
    Init_Drivers(magic_set.input_type, magic_set.sound_channels, MOM_FONT_FILE, MIDI_DRV, MIDI_IO, MIDI_IRQ, MIDI_DMA, DIGI_DRV, DIGI_IO, DIGI_IRQ, DIGI_DMA);
    Release_Version();

    // HERE: MGC === WZD

    // MGC Allocate_Data_Space(6100);
    // MGC Disable_Cancel();
    // WZD Disable_Cancel();
    // WZD Allocate_Data_Space(4600);
    Disable_Cancel();

    Allocate_Data_Space(6100);

    // HERE: MGC ~== WZD

    Load_Palette(0, ST_UNDEFINED, 0);
    Apply_Palette();
    // if(!((argv[1][0] == 'J') && (argv[1][1] == 'E') && (argv[1][1] == 'N') && (argv[1][1] == 'N') && (argv[1][1] == 'Y')))
    if(strcmp(argv[0], "JENNY") != 0)
    {
        /* HACK */  magic_set.sound_effects = ST_TRUE;
        Draw_Logos();
    }

    Load_MGC_Resources();

    // BEGIN: ~== New Game Screen
    Load_TERRSTAT();  // ; ~== Newgame_Screen
    Load_SPELLDAT();  // ; ~== Newgame_Screen
    // END: ~== New Game Screen

    Init_Credits();

    // BEGIN: ~== Main Menu Screen
    CLS();
    CLS_ON();
    // DOMSDOS  Stop_Music__STUB();
    main_menu_music_seg = LBX_Load(music_lbx__main, MUSIC_Main_Menu);
    main_menu_music_seg_size = lbxload_entry_length;
    if(magic_set.background_music == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__WIP(main_menu_music_seg);
        sdl2_Play_Sound__WIP(main_menu_music_seg, main_menu_music_seg_size);
    }
    Load_Palette(0, ST_UNDEFINED, 0);
    Apply_Palette();
    // END: ~== Main Menu Screen

    // WZD Load_SAVE_GAM(8);
    /* WZD */ Load_WZD_Resources();

    // WZD EMS_Pages_Left = STU_INT(0x67, 5901);
    // WZD if(EMS_Pages_Left != 0)
    // WZD {
    // WZD     g_EmmHndl_OVERXYZ = EMM_MakeNamedHandle(EMS_Pages_Left, ehn_OVERXYZ);
    // WZD     EMM_ReleaseHandle(g_EmmHndl_OVERXYZ);
    // WZD     __OvrInitEms(0, 0, EMS_Pages_Left);
    // WZD }
    // WZD else
    // WZD {
    // WZD     g_EmmHndl_OVERXYZ = 0;  // ¿ NULLL ?
    // WZD }

    // WZD Load_Palette(0, ST_UNDEFINED);
    // WZD Calculate_Remap_Colors();
    // WZD Set_Button_Down_Offsets(1, 1,);
    // WZD Cycle_Palette_Color__STUB(198, 40, 0, 0, 63, 0, 0, 1);  // (color_num, red_min, green_min, blue_min, red_max, green_max, blue_max, step_value)
    // WZD Apply_Palette();
    // WZD Fade_In();
    // WZD current_screen = scr_Main_Screen
    // WZD _players[NEUTRAL_PLAYER_IDX].banner_id = 5;  // enum Banner_Color {BNR_Brown = 5 }
    // WZD Clear_Fields();
    // WZD Loaded_Game_Update_WZD();
    // WZD GAME_SoM_Cast_By = ST_UNDEFINED;

    // MGC Menu_Screen_Control();
    // MGC Save_SAVE_GAM(8);
    // MGC Exit_With_Size();

    // WZD Screen_Control();
    // WZD s01p16_empty_function();
    // WZD Exit_With_Size();


    current_screen = scr_Main_Menu_Screen;
    
    Screen_Control();

    return 0;

}
