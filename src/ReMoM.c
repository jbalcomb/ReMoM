// I AM A CHANGE!!!!

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
#include "../STU/src/AI_METRICS.h"
#include "../STU/src/STU_DBG.h"
#include "../STU/src/STU_WRLD.h"
#endif

#include "../STU/src/STU_LOG.h"
#include "../STU/src/STU_BRAK.h"
#include "../STU/src/STU_GRAF.h"

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
#include "../MoX/src/MOX2.h"  /* CLAUDE: Check_Command_Line_Parameters_() */
#include "../MoX/src/SOUND.h"
#include "../MoX/src/Timer.h"

#include "../MoM/src/ALLOC.h"
#include "../MoM/src/CREDITS.h"
#include "../MoM/src/Init.h"
#include "../MoM/src/INTRO.h"
#include "../MoM/src/LOADER.h"
#include "../MoM/src/MOM_SCR.h"
#include "../MoM/src/Settings.h"

#include "../platform/include/Platform.h"
#include "../platform/include/Platform_Replay.h"
#include "../platform/include/Platform_Capture.h"

#include "../MoX/src/LOADSAVE.h"  /* CLAUDE: g_load_save_gam_name_override, for --load */
#include "../STU/src/STU_TST.h"   /* CLAUDE: g_tst_patch_scenario, for --patch */

#include "ReMoM_Init.h"
#include "Artificial_Human_Player.h"  /* HeMoM_Player_Frame() */

#include "../ext/stu_compat.h"
// Reassigns a file pointer. More secure versions of the functions are available; see freopen_s, _wfreopen_s.
#include <stdio.h>
// #define _CRT_SECURE_NO_WARNINGS  // '_CRT_SECURE_NO_WARNINGS' previously declared on the command line
// #define _CRT_SECURE_NO_DEPRECATE
// #define _CRT_NONSTDC_NO_DEPRECATE
// #include <stdio.h>
// 4>C:\STU\devel\ReMoM\src\ReMoM.c(57,9): warning C4005: '_CRT_SECURE_NO_WARNINGS': macro redefinition
#include <stdlib.h>
#include <string.h>

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

/* CLAUDE: --load <SAVE.GAM> -- same as --continue, but loads a named save instead of SAVE9.GAM.
   Mirrors HeMoM's --load (HeMoM.c) so scripted scenes can start from a known state without first
   navigating the Load Game menu.  It reuses the game's REAL load path: scr_Continue calls
   Load_WZD_Resources() + Load_SAVE_GAM(), and g_load_save_gam_name_override redirects the filename
   inside Load_SAVE_GAM.  Nothing about the load sequence is special-cased. */
static char remom_load_file[260] = { 0 };
static int  remom_load_flag = 0;

#ifdef STU_VALGRIND
int16_t remom_start_id;
// make Valgrind says something else, maybe
void ReMoM_Report_Startup_Platform(void)
{
    // TODO  make this use a C StdLib random function - `remom_start_id = Random(42);`
    LOG_INFO(LOG_CAT_REMOM, "ReMoM: Starting up platform layer...  (%d)", remom_start_id);
}
#endif

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
        LOG_INFO(LOG_CAT_REMOM, "GetCurrentDirectoryA(): %s", buffer);
    }
#endif

    if(stu_getcwd(working_directory, sizeof(working_directory)) != NULL)
    {
        LOG_INFO(LOG_CAT_REMOM, "CWD (%s): %s", platform_name, working_directory);
        LOG_DEBUG(LOG_CAT_GENERAL, "CWD (%s): %s", platform_name, working_directory);
    }
    else
    {
        LOG_INFO(LOG_CAT_REMOM, "FATAL: stu_getcwd()");
        LOG_INFO(LOG_CAT_REMOM, "FATAL: stu_getcwd() failed");
        LOG_DEBUG(LOG_CAT_GENERAL, "FATAL: stu_getcwd() failed");
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

#ifdef STU_VALGRIND
    ReMoM_Report_Startup_Platform();
#endif

#ifdef STU_DEBUG
#ifdef _WIN32
    int itr = 0;
#endif
#endif

    /* Route player logs to the per-user state dir (XDG_STATE_HOME/ReMoM etc.)
       instead of the CWD.  The resolver is standalone (no STU_GRAF_Init needed)
       and creates the dir; hand it to STU_LOG before startup.  HeMoM / tests /
       matchup never do this, so they keep logging to the CWD.  REMOM_LOG_DIR
       overrides; an unwritable dir falls back to the CWD. */
    /* CLAUDE: ReMoM.ini [Paths] log_dir wins over the per-user state dir.  The ini is a dev/modder
       template that is NOT bundled for players, so a repo build (which has the staged ini) logs next
       to the executable while an installed game keeps %LOCALAPPDATA%\ReMoM\logs.  Config declaring
       intent, not the binary sniffing its environment.  Full precedence, unchanged otherwise:
       REMOM_LOG_DIR (env) -> [Paths] log_dir -> per-user state dir -> CWD. */
    {
        char ini_log_dir[1024];
        char log_state_dir[1024];

        if(STU_GRAF_Read_Ini_Value("ReMoM.ini", "Paths", "log_dir", ini_log_dir, sizeof(ini_log_dir)) && (ini_log_dir[0] != '\0'))
        {
            STU_Log_Set_Base_Dir(ini_log_dir);
        }
        else if(STU_GRAF_User_State_Dir(log_state_dir, sizeof(log_state_dir)))
        {
            STU_Log_Set_Base_Dir(log_state_dir);
        }
    }

    STU_Log_Startup("ReMoM.ini");

    /* CLAUDE: install crash reporting right after logging exists.  The handler flushes STU_LOG
       before reporting -- the log is buffered, so an abort otherwise discards the whole run's log.
       Catches abort()/failed assertions as well as CPU faults; see STU/src/STU_BRAK.h. */
    STU_BRAK_Install();
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    /* Resolve the game-data search path (env -> exe-dir -> CWD) before any
       asset load, so the installed / portable player boots without a manual cd. */
    STU_GRAF_Init(STU_GRAF_PLAYER);

    /* CLAUDE: ReMoM.ini [Paths] save_dir overrides the per-user *data* dir (saves, MAGIC.SET).
       Same reasoning as log_dir above: a repo build has the staged ini and keeps its saves next to
       the executable; a player's install has no ini and keeps %APPDATA%\ReMoM.  Set after
       STU_GRAF_Init so it survives the profile selection, and well before any save is opened. */
    {
        char ini_save_dir[1024];
        if(STU_GRAF_Read_Ini_Value("ReMoM.ini", "Paths", "save_dir", ini_save_dir, sizeof(ini_save_dir)) && (ini_save_dir[0] != '\0'))
        {
            STU_GRAF_Set_User_Data_Dir(ini_save_dir);
        }
    }

#ifdef STU_DEBUG
    AI_Metrics_Startup();
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

    printf("Hello from the console!");
#endif
#endif

#ifdef STU_DEBUG
#ifdef _WIN32
    LOG_INFO(LOG_CAT_REMOM, "argc: %d", argc);
    LOG_DEBUG(LOG_CAT_GENERAL, "argc: %d", argc);
    for(itr = 0; itr < argc; itr++)
    {
        LOG_INFO(LOG_CAT_REMOM, "argv[%d]: %s", itr, argv[itr]);
        LOG_DEBUG(LOG_CAT_GENERAL, "argv[%d]: %s", itr, argv[itr]);
    }
#endif
#endif

#ifdef STU_DEBUG
    Debug_Print_Working_Directory();
#endif

    /* CLAUDE: MoO2 pattern -- shared CLI parser sets _cmd_line_* globals
       (currently --seed only; flags will migrate over incrementally). */
    Check_Command_Line_Parameters_(argc, argv);

    /* --headless: set SDL video driver to offscreen before SDL init.
       Must be parsed before Startup_Platform() so the environment variable
       takes effect. */
    {
        int argi;
        for (argi = 1; argi < argc; argi++)
        {
            if(stu_strcmp(argv[argi], "--headless") == 0)
            {
#ifdef USE_SDL3
                stu_putenv("SDL_VIDEODRIVER=offscreen");
#else
                stu_putenv("SDL_VIDEODRIVER=dummy");
#endif
                LOG_INFO(LOG_CAT_REMOM, "[headless] SDL video driver set to offscreen");
                break;
            }
        }
    }

    /* Fail soft if the game data isn't present: show a clear, actionable
       message and exit *before* creating a window (no blank-window flash). */
    if(ReMoM_Preflight_Game_Data() != 0)
    {
        return 1;
    }

    /* --orig-files forces a refresh of the writable per-user files from the
       originals (backing up any existing copies); otherwise it's a first-run
       copy-if-absent of CONFIG.MOM / MAGIC.SET / SAVE1-9.GAM into the user-data
       dir so the engine reads/writes its own copies (the install stays
       read-only).  PLAYER only -- both are no-ops for HeMoM. */
    {
        int argi;
        int orig_files = 0;
        for(argi = 1; argi < argc; argi++)
        {
            if(stu_strcmp(argv[argi], "--orig-files") == 0)
            {
                orig_files = 1;
                break;
            }
        }
        if(orig_files)
        {
            ReMoM_Reseed_User_Files();
        }
        else
        {
            ReMoM_Seed_User_Files();
        }
    }

    /* Non-blocking: warn (once) if the installed data doesn't match a supported
       v1.31 distribution.  Silent no-op until the embedded manifest is authored
       (STU/src/lbx_manifest.c, generated by the lbx_hashes tool). */
    ReMoM_Check_Data_Compat();

    LOG_INFO(LOG_CAT_REMOM, "DEBUG: [%s, %d]: BEFORE: Startup_Platform()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: BEFORE: Startup_Platform()", __FILE__, __LINE__);

    Startup_Platform();

    LOG_INFO(LOG_CAT_REMOM, "DEBUG: [%s, %d]: AFTER: Startup_Platform()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: AFTER: Startup_Platform()", __FILE__, __LINE__);


    /* CLAUDE: Register engine callbacks for replay before parsing CLI flags. */
    Platform_Replay_Register_Random_Seed_Callbacks(Get_Random_Seed, Set_Random_Seed);
    Platform_Replay_Register_Field_Log_Callback(Replay_Log_Field_Hit);

    /* CLAUDE: Record, Replay, and Startup Mode CLI flags. */
    {
        int argi;
        /* CLAUDE: --capture is collected across the whole loop (so --capture-fps may appear on
           either side of it) and started once, after parsing.  See
           doc/#AI_Plans/PRD-Scripted-Demo-Capture.md. */
        const char * capture_dir = NULL;
        int capture_fps = 0;
        for(argi = 1; argi < argc; argi++)
        {
            if(stu_strcmp(argv[argi], "--capture") == 0 && (argi + 1) < argc)
            {

                LOG_INFO(LOG_CAT_REMOM, "DEBUG: [%s, %d]: --capture flag detected", __FILE__, __LINE__);

                argi++;
                capture_dir = argv[argi];
                continue;
            }
            else if(stu_strcmp(argv[argi], "--capture-fps") == 0 && (argi + 1) < argc)
            {

                LOG_INFO(LOG_CAT_REMOM, "DEBUG: [%s, %d]: --capture-fps flag detected", __FILE__, __LINE__);

                argi++;
                capture_fps = atoi(argv[argi]);
                continue;
            }

            if(stu_strcmp(argv[argi], "--record") == 0 && (argi + 1) < argc)
            {

                LOG_INFO(LOG_CAT_REMOM, "DEBUG: [%s, %d]: --record flag detected", __FILE__, __LINE__);
                LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --record flag detected", __FILE__, __LINE__);
                LOG_TRACE(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --record flag detected", __FILE__, __LINE__);

                argi++;
                Platform_Record_Start(argv[argi]);
            }
            else if(stu_strcmp(argv[argi], "--replay") == 0 && (argi + 1) < argc)
            {

                LOG_INFO(LOG_CAT_REMOM, "DEBUG: [%s, %d]: --replay flag detected", __FILE__, __LINE__);
                LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --replay flag detected", __FILE__, __LINE__);
                LOG_TRACE(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --replay flag detected", __FILE__, __LINE__);

                argi++;
                Platform_Replay_Start(argv[argi]);
            }
            else if(stu_strcmp(argv[argi], "--continue") == 0)
            {

                LOG_INFO(LOG_CAT_REMOM, "DEBUG: [%s, %d]: --continue flag detected", __FILE__, __LINE__);
                LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --continue flag detected", __FILE__, __LINE__);
                LOG_TRACE(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --continue flag detected", __FILE__, __LINE__);

                remom_continue_flag = 1;
            }
            else if(stu_strcmp(argv[argi], "--load") == 0 && (argi + 1) < argc)
            {

                LOG_INFO(LOG_CAT_REMOM, "DEBUG: [%s, %d]: --load flag detected", __FILE__, __LINE__);
                LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --load flag detected", __FILE__, __LINE__);
                LOG_TRACE(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --load flag detected", __FILE__, __LINE__);

                argi++;
                stu_strcpy(remom_load_file, argv[argi]);
                remom_load_flag = 1;
                LOG_INFO(LOG_CAT_REMOM, "[ReMoM] CLI: --load \"%s\"", remom_load_file);
                /* CLAUDE: --load trace point 1 of 3 -- just after remom_load_file is assigned. */
                LOG_INFO(LOG_CAT_LOADSAVE, "[--load] ASSIGN remom_load_file: argv[%d]=\"%s\" -> remom_load_file=%p \"%s\" (len=%d) remom_load_flag=%d", argi, argv[argi], (void *)remom_load_file, remom_load_file, (int)strlen(remom_load_file), remom_load_flag);
            }
            else if(stu_strcmp(argv[argi], "--patch") == 0 && (argi + 1) < argc)
            {
                /* CLAUDE: --patch <name> selects a scenario for TST_Patch_Game_Data(), which runs
                   from Loaded_Game_Update() after every save load.  Ships in Release and is inert
                   unless named here -- see doc/#Devel/Scenario-Patching.md. */
                argi++;
                g_tst_patch_scenario = argv[argi];
                LOG_INFO(LOG_CAT_REMOM, "[ReMoM] CLI: --patch \"%s\"", g_tst_patch_scenario);
            }
            else if(stu_strcmp(argv[argi], "--demo") == 0)
            {

                LOG_INFO(LOG_CAT_REMOM, "DEBUG: [%s, %d]: --demo flag detected", __FILE__, __LINE__);
                LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --demo flag detected", __FILE__, __LINE__);
                LOG_TRACE(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --demo flag detected", __FILE__, __LINE__);

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
            else if(strcmp(argv[argi], "--scenario") == 0 && (argi + 1) < argc)
            {

                LOG_INFO(LOG_CAT_REMOM, "DEBUG: [%s, %d]: --scenario flag detected", __FILE__, __LINE__);
                LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --scenario flag detected", __FILE__, __LINE__);
                LOG_TRACE(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: --scenario flag detected", __FILE__, __LINE__);

                argi++;
                if(HeMoM_Player_Load_Scenario(argv[argi]) == 0)
                {
                    Platform_Register_Frame_Callback(HeMoM_Player_Frame);
                }
            }
        }

        /* CLAUDE: start capture last, so its t=0 is as close as possible to the first presented
           frame, and register the finalizer -- the game exits through several paths that never
           return here (Exit_With_Message, the endgame Respawn hack), so atexit is what guarantees
           the WAV header and capture_info.txt get written. */
        if(capture_dir != NULL)
        {
            if(Platform_Capture_Start(capture_dir, capture_fps) == 0)
            {
                atexit(Platform_Capture_Stop);
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
    AI_Metrics_Shutdown();
#endif

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    STU_Log_Shutdown();

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

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    /* Core engine initialization (shared with HeMoM) */
    ReMoM_Init_Engine();

    // DOMSDOS  MS-DOS has some area for the program execution that lets you get away with the AVRL here?  ... why argv[1] instead of argv[2]?
    /* OG-MoM: `MAGIC.EXE JENNY` skipped the intro logos */
    /* MS-DOS allowed checking argv[1] even if there wasn't an argument? ... if(!(argv[1][0] == 'J' && argv[1][1] == 'E' && argv[1][2] == 'N' && argv[1][3] == 'N' && argv[1][4] == 'Y') */
    int skip_intro = (argc > 1 && strcmp(argv[1], "JENNY") == 0);
    /* CLAUDE: remom_load_flag joins remom_continue_flag here -- --load goes straight into a saved
       game, so the intro logos are as unwanted as they are for --continue.  Omitting it meant a
       --load run still sat through the whole logo sequence. */
    if(!skip_intro && !remom_continue_flag && !remom_load_flag)
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
    // WZD     g_EmmHndl_OVERXYZ = 0;  // ¿ NULL ?
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
    // WZD m_magic_winner_idx = ST_UNDEFINED;

    // MGC Menu_Screen_Control();
    // MGC Save_SAVE_GAM(8);
    // MGC Exit_With_Size();

    // WZD Screen_Control();
    // WZD s01p16_empty_function();
    // WZD Exit_With_Size();


    if(remom_load_flag)
    {
        /* CLAUDE: --load <SAVE.GAM>: the same scr_Continue path as --continue, with the filename
           redirected.  scr_Continue does Load_WZD_Resources() + Load_SAVE_GAM(8), and
           g_load_save_gam_name_override replaces the name inside Load_SAVE_GAM (it is consumed on
           first use, so it affects exactly this one load).  The load sequence itself is the game's
           own -- nothing here special-cases it. */
        g_load_save_gam_name_override = remom_load_file;
        /* CLAUDE: --load trace point 2 of 3 -- just after g_load_save_gam_name_override is assigned,
           immediately before entering scr_Continue (which calls Load_SAVE_GAM(8)). */
        LOG_INFO(LOG_CAT_LOADSAVE, "[--load] ASSIGN g_load_save_gam_name_override=%p \"%s\"  (from remom_load_file=%p \"%s\")  -> entering scr_Continue, which calls Load_SAVE_GAM(8)", (void *)g_load_save_gam_name_override, (g_load_save_gam_name_override != NULL) ? g_load_save_gam_name_override : "(null)", (void *)remom_load_file, remom_load_file);
        current_screen = scr_Continue;
    }
    else if(remom_continue_flag)
    {
        /* --continue: skip Main Menu, load SAVE9.GAM, enter game (WIZARDS.EXE path). */
        current_screen = scr_Continue;
    }
    else
    {
        current_screen = scr_Main_Menu_Screen;
    }

    Screen_Control();

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    return 0;

}
