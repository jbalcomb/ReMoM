/*
    Settings_Ini -- the player's ReMoM.ini.  See Settings_Ini.h and doc/@AI_Plans/PLAN-ReMoM-Ini.md.
*/

#include "Settings_Ini.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../STU/src/STU_GRAF.h"
#include "../STU/src/STU_INI.h"
#include "../STU/src/STU_LOG.h"



#define SETTINGS_INI_FILE_NAME      "ReMoM.ini"
#define SETTINGS_EXAMPLE_FILE_NAME  "ReMoM_example.ini"
#define SETTINGS_PATH_CAPACITY      1024

#if defined(_WIN32)
#define SETTINGS_GAME_DATA_EXAMPLE  "C:\\GOG Games\\Master of Magic"
#else
#define SETTINGS_GAME_DATA_EXAMPLE  "/home/you/Games/Master of Magic"
#endif

/* The player settings: what a new ReMoM.ini holds, what is added to an older one, and what ReMoM_example.ini shows. Only the
   settings documented for players (README.md, PLAYING.md); the developer keys in assets/ReMoM.ini are never added to a player's
   file. The defaults here must match the code's: [Video] in src/ReMoM.c. A setting added here reaches every player's file on
   their next start. */
static const STU_INI_Setting settings_ini_settings[] =
{
    {
        "Paths", "game_data", SETTINGS_GAME_DATA_EXAMPLE,
        "game_data: the folder with your Master of Magic files (FONTS.LBX and the other .LBX files).\n"
        "Only needed if ReMoM didn't find them by itself.",
        1
    },
    {
        "Video", "startup_window", "fullscreen",
        "startup_window: how ReMoM starts.  fullscreen | windowed  (default: fullscreen)\n"
        "  fullscreen  covers the whole screen, without changing its resolution.\n"
        "  windowed    a normal window you can move and resize.\n"
        "Alt+Enter switches between them while you play.",
        0
    },
    {
        "Video", "scale_mode", "integer",
        "scale_mode: how the picture is enlarged.  integer | fit_nearest | fit_sharp_bilinear  (default: integer)\n"
        "  integer             the sharpest: every pixel the same size, with black bars around the picture.\n"
        "  fit_nearest         as big as the screen allows, hard-edged pixels, some a little wider than others.\n"
        "  fit_sharp_bilinear  as big as the screen allows, every pixel the same size, edges very slightly soft.",
        0
    },
    {
        "Video", "aspect_ratio", "16:10",
        "aspect_ratio: the picture's shape.  16:10 | 4:3  (default: 16:10)\n"
        "  16:10  square pixels.\n"
        "  4:3    the shape it had on a 1994 monitor: each pixel a little taller than wide.",
        0
    },
};
#define SETTINGS_INI_SETTING_COUNT  ((int)(sizeof(settings_ini_settings) / sizeof(settings_ini_settings[0])))

static const char * settings_ini_header =
    "ReMoM settings.\n"
    "\n"
    "To change a setting, remove the ; at the start of its line and edit the value.\n"
    "ReMoM_example.ini, next to this file, shows every setting at its default.\n"
    "A setting ReMoM doesn't understand is ignored, and the default is used.\n"
    "\n"
    "When a new version of ReMoM adds settings, they are added to this file, commented out,\n"
    "and a copy of the file as it was is kept as ReMoM_backup_<date and time>.ini.";

static const char * settings_example_header =
    "ReMoM_example.ini: every ReMoM setting at its default.\n"
    "\n"
    "ReMoM never reads this file, and replaces it with each new version.\n"
    "Your settings are in ReMoM.ini, next to this file.  To change one, edit it there.";



/* What was found and done for one ReMoM.ini location. */
typedef struct
{
    char path[SETTINGS_PATH_CAPACITY];          /* "" when the folder could not be worked out */
    int  exists;
    int  created;                               /* this run made it */
    int  create_failed;
    int  add_status;                            /* STU_INI_Add_Missing(); STU_INI_UNCHANGED when not run */
    int  added;
    char backup_path[SETTINGS_PATH_CAPACITY];
    char example_path[SETTINGS_PATH_CAPACITY];
    int  example_status;                        /* STU_INI_Write_If_Different(); STU_INI_UNCHANGED when not run */
} settings_ini_location;

static settings_ini_location settings_ini_local;   /* next to the program */
static settings_ini_location settings_ini_config;  /* the config folder */
static const char * settings_ini_in_effect = NULL;
static int settings_ini_program_dir_from_env = 0;



/* "<dir><name>", dir ending in a separator. */
static void Settings_Ini_Join(char * out, size_t capacity, const char * dir, const char * name)
{
    snprintf(out, capacity, "%s%s", dir, name);
}

/* The folder next to the program, ending in a separator: REMOM_PROGRAM_DIR when set (the AppImage launcher sets it to the folder
   holding the .AppImage, since the running binary is inside the read-only image), else the executable's folder. */
static int Settings_Ini_Program_Dir(char * out, size_t capacity)
{
    const char * env = getenv("REMOM_PROGRAM_DIR");
    if((env != NULL) && (env[0] != '\0'))
    {
        size_t length = strlen(env);
        int needs_separator = ((env[length - 1] != '/') && (env[length - 1] != '\\'));
        int n = snprintf(out, capacity, "%s%s", env, (needs_separator != 0) ? "/" : "");
        settings_ini_program_dir_from_env = 1;
        return ((n > 0) && ((size_t)n < capacity)) ? 1 : 0;
    }
    return STU_GRAF_Executable_Dir(out, capacity);
}

/* The settings a release knows about are added to the file, and the example file is written beside it. */
static void Settings_Ini_Update(settings_ini_location * location, const char * dir)
{
    char * example_text;

    location->add_status = STU_INI_Add_Missing(location->path, settings_ini_settings, SETTINGS_INI_SETTING_COUNT, location->backup_path, sizeof(location->backup_path), &location->added);

    Settings_Ini_Join(location->example_path, sizeof(location->example_path), dir, SETTINGS_EXAMPLE_FILE_NAME);
    example_text = STU_INI_Compose(settings_example_header, settings_ini_settings, SETTINGS_INI_SETTING_COUNT, 0, STU_INI_Native_Eol());
    location->example_status = (example_text != NULL) ? STU_INI_Write_If_Different(location->example_path, example_text) : STU_INI_ERROR;
    free(example_text);
}

void Settings_Ini_Startup(void)
{
    char program_dir[SETTINGS_PATH_CAPACITY];
    char config_dir[SETTINGS_PATH_CAPACITY];
    int have_program_dir;
    int have_config_dir;

    memset(&settings_ini_local, 0, sizeof(settings_ini_local));
    memset(&settings_ini_config, 0, sizeof(settings_ini_config));
    settings_ini_in_effect = NULL;
    settings_ini_program_dir_from_env = 0;

    have_program_dir = Settings_Ini_Program_Dir(program_dir, sizeof(program_dir));
    have_config_dir = STU_GRAF_User_Config_Dir(config_dir, sizeof(config_dir));

    if(have_program_dir != 0)
    {
        Settings_Ini_Join(settings_ini_local.path, sizeof(settings_ini_local.path), program_dir, SETTINGS_INI_FILE_NAME);
        settings_ini_local.exists = STU_INI_File_Exists(settings_ini_local.path);
    }
    if(have_config_dir != 0)
    {
        Settings_Ini_Join(settings_ini_config.path, sizeof(settings_ini_config.path), config_dir, SETTINGS_INI_FILE_NAME);
        if(strcmp(settings_ini_config.path, settings_ini_local.path) == 0)
        {
            settings_ini_config.path[0] = '\0';  /* the program is in the config folder: one file, counted once */
            have_config_dir = 0;
        }
        else
        {
            settings_ini_config.exists = STU_INI_File_Exists(settings_ini_config.path);
        }
    }

    /* Neither exists: make the one in the config folder, every setting commented out at its default. */
    if((settings_ini_local.exists == 0) && (settings_ini_config.exists == 0) && (have_config_dir != 0))
    {
        char * new_text = STU_INI_Compose(settings_ini_header, settings_ini_settings, SETTINGS_INI_SETTING_COUNT, 1, STU_INI_Native_Eol());
        if((new_text != NULL) && (STU_GRAF_Make_Path(config_dir) != 0) && (STU_INI_Write_If_Different(settings_ini_config.path, new_text) == STU_INI_CHANGED))
        {
            settings_ini_config.exists = 1;
            settings_ini_config.created = 1;
        }
        else
        {
            settings_ini_config.create_failed = 1;
        }
        free(new_text);
    }

    if(settings_ini_local.exists != 0)
    {
        Settings_Ini_Update(&settings_ini_local, program_dir);
    }
    if(settings_ini_config.exists != 0)
    {
        Settings_Ini_Update(&settings_ini_config, config_dir);
    }

    if(settings_ini_local.exists != 0)
    {
        settings_ini_in_effect = settings_ini_local.path;
    }
    else if(settings_ini_config.exists != 0)
    {
        settings_ini_in_effect = settings_ini_config.path;
    }
}

const char * Settings_Ini_Path(void)
{
    return settings_ini_in_effect;
}

static void Settings_Ini_Log_Location(const settings_ini_location * location)
{
    if(location->exists == 0)
    {
        return;
    }
    if(location->add_status == STU_INI_CHANGED)
    {
        LOG_INFO(LOG_CAT_REMOM, "settings: added %d new setting(s) to %s, commented out; the file as it was is %s", location->added, location->path, location->backup_path);
    }
    else if(location->add_status == STU_INI_ERROR)
    {
        LOG_WARN(LOG_CAT_REMOM, "settings: could not add the new settings to %s (could not read it, back it up, or write it); it is unchanged", location->path);
    }
    if(location->example_status == STU_INI_CHANGED)
    {
        LOG_INFO(LOG_CAT_REMOM, "settings: wrote %s", location->example_path);
    }
    else if(location->example_status == STU_INI_ERROR)
    {
        LOG_WARN(LOG_CAT_REMOM, "settings: could not write %s", location->example_path);
    }
}

void Settings_Ini_Log(void)
{
    const char * local_label = (settings_ini_program_dir_from_env != 0) ? "next to the program, REMOM_PROGRAM_DIR" : "next to the program";

    if(settings_ini_in_effect == NULL)
    {
        LOG_WARN(LOG_CAT_REMOM, "settings: no ReMoM.ini (looked for %s and %s, and could not create the second) -- using the defaults", (settings_ini_local.path[0] != '\0') ? settings_ini_local.path : "(program folder unknown)", (settings_ini_config.path[0] != '\0') ? settings_ini_config.path : "(config folder unknown)");
        return;
    }
    if(settings_ini_in_effect == settings_ini_local.path)
    {
        LOG_INFO(LOG_CAT_REMOM, "settings: using %s (%s)", settings_ini_local.path, local_label);
        if(settings_ini_config.exists != 0)
        {
            LOG_INFO(LOG_CAT_REMOM, "settings: %s is not used while %s exists", settings_ini_config.path, settings_ini_local.path);
        }
    }
    else if(settings_ini_config.created != 0)
    {
        LOG_INFO(LOG_CAT_REMOM, "settings: using %s (config folder; created, every setting at its default)", settings_ini_config.path);
    }
    else
    {
        LOG_INFO(LOG_CAT_REMOM, "settings: using %s (config folder; there is no %s)", settings_ini_config.path, settings_ini_local.path);
    }
    Settings_Ini_Log_Location(&settings_ini_local);
    Settings_Ini_Log_Location(&settings_ini_config);
}
