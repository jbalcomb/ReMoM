// I AM A CHANGE!!!

// #ifndef _STU_SDL2
// #define _STU_SDL2
// #endif

// #ifndef STU_DEBUG
// #define STU_DEBUG
// #endif

// #ifndef STU_TRACE
// #define STU_TRACE
// #endif

/* CLAUDE */  /* removed — SDL's own headers define HAVE_LIBC correctly */
/* #ifndef _WINDLL */
/* #define HAVE_LIBC */
/* #endif */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef STU_DEBUG
#include "../STU/src/STU_DBG.h"
#include "../STU/src/STU_WRLD.h"
#endif

#include "../MoX/src/capture.h"
#include "../MoX/src/CFG.h"
#include "../MoX/src/DOS.h"
#include "../MoX/src/EMS/EMS.h"
#include "../MoX/src/EXIT.h"
#include "../MoX/src/Fields.h"
#include "../MoX/src/Fonts.h"
#include "../MoX/src/Graphics.h"
#include "../MoX/src/LBX_Load.h"
#include "../MoX/src/MOX_BASE.h"
#include "../MoX/src/MOM_DAT.h"
#include "../MoX/src/MOX_DEF.h"
#include "../MoX/src/MOX_SET.h"
#include "../MoX/src/MOX_T4.h"
#include "../MoX/src/MOX_TYPE.h"
#include "../MoX/src/random.h"
#include "../MoX/src/SOUND.h"
#include "../MoX/src/Timer.h"

#include "../MoM/src/ALLOC.h"
#include "../MoM/src/CREDITS.h"
#include "../MoM/src/Init.h"
#include "../MoM/src/INTRO.h"
#include "../MoM/src/LOADER.h"
#include "../MoM/src/MOM_SCR.h"
#include "../MoM/src/Settings.h"

// Reassigns a file pointer. More secure versions of the functions are available; see freopen_s, _wfreopen_s.
#include <stdio.h>
// #define _CRT_SECURE_NO_WARNINGS  // '_CRT_SECURE_NO_WARNINGS' previously declared on the command line
// #define _CRT_SECURE_NO_DEPRECATE
// #define _CRT_NONSTDC_NO_DEPRECATE
// #include <stdio.h>
// 4>C:\STU\devel\ReMoM\src\ReMoM.c(57,9): warning C4005: '_CRT_SECURE_NO_WARNINGS': macro redefinition



#include <stdlib.h>
#include <string.h>

#include "../ext/stu_compat.h"
#include "../platform/include/Platform.h"
#include "../platform/include/Platform_Replay.h"
#include "ReMoM_Init.h"
#include "HeMoM_Player.h"

/* COPILOT */ /* SDL_main.h redefines main() on some platforms (macOS, iOS, Android).
                      We handle our own main(), so tell SDL not to intercept it.
                      Skip SDL headers entirely for the native Win32 backend. */
/* #ifdef _WIN32 */
/* #define SDL_MAIN_HANDLED */
/* #ifdef USE_SDL3 */
/* #include <SDL3/SDL_main.h> */
/* #else */
/* #include <SDL.h> */
/* #endif */
/* #endif */
#if defined(_WIN32) && !defined(_STU_WIN)
#define SDL_MAIN_HANDLED
#ifdef USE_SDL3
#if __has_include(<SDL3/SDL_main.h>)
#include <SDL3/SDL_main.h>
#endif
#else
#if __has_include(<SDL.h>)
#include <SDL.h>
#endif
#endif
#endif

/* <direct.h> replaced by stu_compat.h */


int MOM_main(int argc, char** argv);
void Startup_Platform(void);
void Shutdown_Platform(void);

/* --continue: skip Main Menu, go straight to scr_Continue (WIZARDS.EXE path). */
static int remom_continue_flag = 0;

int16_t remom_start_id;
// make Valgrind says something else, maybe
void ReMoM_Report_Startup_Platform(void)
{
    remom_start_id = Random(42);
    fprintf(stderr, "ReMoM: Starting up platform layer...  (%d)\n", remom_start_id);
}

#ifdef STU_DEBUG
/* CLAUDE */
/**
 * Print and log the current working directory.
 */
static void Debug_Print_Working_Directory(void)
{
    char working_directory[4096];
    const char * platform_name;

#if defined(_WIN32)
    platform_name = "Win32";
#elif defined(__APPLE__)
    platform_name = "macOS";
#elif defined(__linux__)
    platform_name = "Linux";
#else
    platform_name = "Unknown";
#endif

#ifdef _WIN32
    char buffer[MAX_PATH] = { 0 };
    if(GetCurrentDirectoryA(MAX_PATH, buffer) != 0)
    {
        printf("GetCurrentDirectoryA(): %s\n", buffer);
    }
#endif

    if (stu_getcwd(working_directory, sizeof(working_directory)) != NULL)
    {
        fprintf(stderr, "CWD (%s): %s\n", platform_name, working_directory);
        dbg_prn("CWD (%s): %s\n", platform_name, working_directory);
    }
    else
    {
        printf("FATAL: stu_getcwd()\n");
        fprintf(stderr, "FATAL: stu_getcwd() failed\n");
        dbg_prn("FATAL: stu_getcwd() failed\n");
        stu_debugbreak();
    }
}
#endif



// #define MOM_FONT_FILE "FONTS.LBX"
// #define GAME_FONT_FILE MOM_FONT_FILE
char MOM_FONT_FILE[] = "FONTS.LBX";
char * GAME_FONT_FILE = &MOM_FONT_FILE[0];  // Create a Pointer to the Character-Array (string) at compile time.
// char mom_font_file[] = "FONTS.LBX";
// char* game_font_file = &mom_font_file[0];   // Create a pointer to the string at compile time.

char CONFIG_FILE[] = "CONFIG.MOM";

/* HACK */  char soundfx_lbx__remom[] = "SOUNDFX";
/* HACK */  char introsfx_lbx__remom[] = "INTROSFX";



/* CLAUDE: Callback for replay field hit-test logging.
   Registered with the replay system so Replay.c doesn't need to include Fields.h. */
static void Replay_Log_Field_Hit(void *log, int mouse_x, int mouse_y)
{
    FILE *fp = (FILE *)log;
    int fi;
    for(fi = 0; fi < fields_count; fi++)
    {
        if(mouse_x >= p_fields[fi].x1 && mouse_x <= p_fields[fi].x2 && mouse_y >= p_fields[fi].y1 && mouse_y <= p_fields[fi].y2)
        {
            fprintf(fp, "  field[%d]=(%d,%d)-(%d,%d)", fi, p_fields[fi].x1, p_fields[fi].y1, p_fields[fi].x2, p_fields[fi].y2);
            break;
        }
    }
}



/**
 * @brief Main entry point of the ReMoM application.
 * 
 * @param argc The number of command-line arguments passed to the program.
 * @param argv An array of pointers to null-terminated strings representing the command-line arguments.
 *             argv[0] typically contains the program name.
 * 
 * @return int Exit status code. Typically returns 0 on success and non-zero on failure.
 */
// int SDL_main(int argc, char* argv[])
int main(int argc, char * argv[])
{

    ReMoM_Report_Startup_Platform();

#ifdef STU_DEBUG
#ifdef _WIN32
    int itr = 0;
#endif
#endif
#ifdef STU_DEBUG
    Debug_Log_Startup();
    dbg_prn("DEBUG: [%s, %d]: BEGIN: ReMoM main()\n", __FILE__, __LINE__);
#endif
#ifdef STU_DEBUG
    Trace_Log_Startup();
    trc_prn("TRACE: [%s, %d]: BEGIN: ReMoM main()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
#ifdef _WIN32
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    HWND Handle = GetConsoleWindow();
    #pragma warning(suppress : 6031)  // Return value ignored: 'called-function' could return unexpected value
    freopen("CON", "w", stdout);
    #pragma warning(suppress : 6031)  // Return value ignored: 'called-function' could return unexpected value
    freopen("CON", "w", stderr);

//     // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/freopen-wfreopen?view=msvc-170
//     FILE *stream;
//     // Reassign "stderr" to "freopen.out":
//     stream = freopen( "freopen.out", "w", stderr ); // C4996
//     // Note: freopen is deprecated; consider using freopen_s instead
//     if( stream == NULL )
//         fprintf( stdout, "error on freopen\n" );
//     else
//     {
//         fprintf( stdout, "successfully reassigned\n" );
//         fflush( stdout );
//         fprintf( stream, "This will go to the file 'freopen.out'\n" );
//         fclose( stream );
//     }
//     system( "type freopen.out" );

    printf("Hello from the console!\n");
#endif
#endif

#ifdef STU_DEBUG
#ifdef _WIN32
    printf("argc: %d\n", argc);
    dbg_prn("argc: %d\n", argc);
    for(itr = 0; itr < argc; itr++)
    {
        printf("argv[%d]: %s\n", itr, argv[itr]);
        dbg_prn("argv[%d]: %s\n", itr, argv[itr]);
    }
#endif
#endif

#ifdef STU_DEBUG
    Debug_Print_Working_Directory();
#endif

    /* --headless: set SDL video driver to offscreen before SDL init.
       Must be parsed before Startup_Platform() so the environment variable
       takes effect. Has no effect on the pure headless (USE_HEADLESS) backend. */
#ifndef USE_HEADLESS
    {
#ifdef STU_DEBUG
    printf("DEBUG: [%s, %d]: #ifndef USE_HEADLESS\n", __FILE__, __LINE__);
    dbg_prn("DEBUG: [%s, %d]: #ifndef USE_HEADLESS\n", __FILE__, __LINE__);
#endif
        int argi;
        for (argi = 1; argi < argc; argi++)
        {
            if (stu_strcmp(argv[argi], "--headless") == 0)
            {
#ifdef USE_SDL3
                stu_putenv("SDL_VIDEODRIVER=offscreen");
#else
                stu_putenv("SDL_VIDEODRIVER=dummy");
#endif
                fprintf(stderr, "[headless] SDL video driver set to offscreen\n");
                break;
            }
        }
    }
#endif

#ifdef STU_DEBUG
    printf("DEBUG: [%s, %d]: BEFORE: Startup_Platform()\n", __FILE__, __LINE__);
    dbg_prn("DEBUG: [%s, %d]: BEFORE: Startup_Platform()\n", __FILE__, __LINE__);
#endif
    Startup_Platform();
#ifdef STU_DEBUG
    printf("DEBUG: [%s, %d]: AFTER: Startup_Platform()\n", __FILE__, __LINE__);
    dbg_prn("DEBUG: [%s, %d]: AFTER: Startup_Platform()\n", __FILE__, __LINE__);
#endif

    /* CLAUDE: Register engine callbacks for replay before parsing CLI flags. */
    Platform_Replay_Register_Random_Seed_Callbacks(Get_Random_Seed, Set_Random_Seed);
    Platform_Replay_Register_Field_Log_Callback(Replay_Log_Field_Hit);

    /* CLAUDE: Record, Replay, and Startup Mode CLI flags. */
    {
        int argi;
        for(argi = 1; argi < argc; argi++)
        {
            if(stu_strcmp(argv[argi], "--record") == 0 && (argi + 1) < argc)
            {
                argi++;
                Platform_Record_Start(argv[argi]);
            }
            else if(stu_strcmp(argv[argi], "--replay") == 0 && (argi + 1) < argc)
            {
                argi++;
                Platform_Replay_Start(argv[argi]);
            }
            else if(stu_strcmp(argv[argi], "--continue") == 0)
            {
#ifdef STU_DEBUG
                printf("DEBUG: [%s, %d]: --continue flag detected\n", __FILE__, __LINE__);
                dbg_prn("DEBUG: [%s, %d]: --continue flag detected\n", __FILE__, __LINE__);
#endif
                remom_continue_flag = 1;
            }
            else if(stu_strcmp(argv[argi], "--demo") == 0)
            {
                /* Optional filename: --demo [FILE.RMR], defaults to DEMO.RMR */
                if((argi + 1) < argc && argv[argi + 1][0] != '-')
                {
                    argi++;
                    Platform_Replay_Start(argv[argi]);
                }
                else
                {
                    Platform_Replay_Start("DEMO.RMR");
                }
            }
            else if(strcmp(argv[argi], "--continue") == 0)
            {
                remom_continue_flag = ST_TRUE;
            }
            else if(strcmp(argv[argi], "--scenario") == 0 && (argi + 1) < argc)
            {
                argi++;
                if(HeMoM_Player_Load_Scenario(argv[argi]) == 0)
                {
                    Platform_Register_Frame_Callback(HeMoM_Player_Frame);
                }
            }
        }
    }

// /* HACK */  #ifdef STU_DEBUG
// /* HACK */      Simulate_World_Map_Generation();
// /* HACK */      Exit_With_Message("Simulated world map generation complete. Exiting.");
// /* HACK */  #else
    MOM_main(argc, argv);
// /* HACK */  #endif

    /* CLAUDE: Stop any active recording/replay before shutdown. */
    if(Platform_Record_Active())
    {
        Platform_Record_Stop();
    }
    if(Platform_Replay_Active())
    {
        Platform_Replay_Stop();
    }
    HeMoM_Player_Shutdown();

    Shutdown_Platform();

#ifdef STU_DEBUG
#ifdef _WIN32
    FreeConsole();
#endif
#endif

#ifdef STU_DEBUG
#endif

#ifdef STU_DEBUG
    trc_prn("TRACE: [%s, %d]: END: ReMoM main()\n", __FILE__, __LINE__);
    Trace_Log_Shutdown();
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: ReMoM main()\n", __FILE__, __LINE__);
    Debug_Log_Shutdown();
#endif

    return 0;

}


/*
This is an amalgamation of the original main functions from both the MAGIC.EXE and WIZARDS.EXE binaries.
1. It loads the CONFIG.MOM file, generated by the INSTALL.EXE program.
2. It configures the sound drivers based on the settings in CONFIG.MOM.
3. It configures the input setttings.

* It initializes the hardware for input, sound, and extended memory.

* Release_Version()?

* Disable_Cancel()?

* Allocate memory for the game data

* Reset the color palette

* Draw Logos
* Load resources for the 'Main Menu Screen' and 'New Game Screen' - Load_MGC_Resources(); Load_TERRSTAT(); Load_SPELLDAT();
* Initialize the credits



*/
int MOM_main(int argc, char** argv)
{
    SAMB_ptr main_menu_music_seg;  // MGC
    uint32_t main_menu_music_seg_size = 0;  // DNE in Dasm

    /* Core engine initialization (shared with HeMoM) */
    ReMoM_Init_Engine();

    // DOMSDOS  MS-DOS has some area for the program execution that lets you get away with the AVRL here?  ... why argv[1] instead of argv[2]?
    /* OG-MoM: `MAGIC.EXE JENNY` skipped the intro logos */
    /* MS-DOS allowed checking argv[1] even if there wasn't an argument? ... if(!(argv[1][0] == 'J' && argv[1][1] == 'E' && argv[1][2] == 'N' && argv[1][3] == 'N' && argv[1][4] == 'Y') */
    int skip_intro = (argc > 1 && strcmp(argv[1], "JENNY") == 0);
    if (!skip_intro && !remom_continue_flag)
    {
        Draw_Logos();
    }

    Init_Credits();

    // BEGIN: ~== Main Menu Screen
    CLS();
    CLS_ON();
    // DOMSDOS  Stop_Music__STUB();
    main_menu_music_seg = LBX_Load(music_lbx__main, MUSIC_Main_Menu);
    main_menu_music_seg_size = lbxload_entry_length;
    if(magic_set.background_music == ST_TRUE)
    {
        Play_Sound(main_menu_music_seg, main_menu_music_seg_size);
    }
    Load_Palette(0, ST_UNDEFINED, 0);
    Apply_Palette();
    // END: ~== Main Menu Screen

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
    // WZD Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1);  // (color_num, red_min, green_min, blue_min, red_max, green_max, blue_max, step_value)
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


    if (remom_continue_flag)
    {
        /* --continue: skip Main Menu, load SAVE9.GAM, enter game (WIZARDS.EXE path). */
        current_screen = scr_Continue;
    }
    else
    {
        current_screen = scr_Main_Menu_Screen;
    }

    Screen_Control();

    return 0;

}
