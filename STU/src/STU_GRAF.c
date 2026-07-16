/*
    STU_GRAF -- Game Resource Archive File locator (implementation)

    See STU_GRAF.h for the layering rationale.  This file is portable C with
    platform #ifdefs (same posture as STU_LOG's SEH/POSIX split) and has no SDL
    dependency: executable-directory resolution uses the raw OS calls directly
    (/proc/self/exe, GetModuleFileNameA, _NSGetExecutablePath).
*/

#include "STU_GRAF.h"

#include "../../ext/stu_compat.h"  /* stu_fopen_ci() */
#include "STU_LOG.h"

#include <string.h>
#include <stdlib.h>   /* getenv */
#include <ctype.h>    /* tolower */

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <stdint.h>
#include <mach-o/dyld.h>  /* _NSGetExecutablePath */
#else
#include <unistd.h>       /* readlink, ssize_t */
#endif

#define STU_GRAF_MAX_DIRS 8
#define STU_GRAF_PATH_MAX 1024

static char g_search_dirs[STU_GRAF_MAX_DIRS][STU_GRAF_PATH_MAX];
static int  g_search_dir_count = 0;

/* True if the name is already a path (absolute or contains a separator), in
   which case STU_GRAF must not prepend a search directory. */
static int graf_name_is_path(const char * name)
{
    if(name == NULL || name[0] == '\0')
    {
        return 0;
    }
    if(name[0] == '/' || name[0] == '\\')
    {
        return 1;
    }
    if(strchr(name, '/') != NULL || strchr(name, '\\') != NULL)
    {
        return 1;
    }
#if defined(_WIN32)
    if(name[1] == ':')  /* drive-letter path, e.g. C:\... */
    {
        return 1;
    }
#endif
    return 0;
}

/* Join a search dir and a bare name into out, inserting a single separator. */
static void graf_join(char * out, size_t cap, const char * dir, const char * name)
{
    size_t dlen = strlen(dir);
    int has_sep = (dlen > 0 && (dir[dlen - 1] == '/' || dir[dlen - 1] == '\\'));
    if(has_sep)
    {
        snprintf(out, cap, "%s%s", dir, name);
    }
    else
    {
        snprintf(out, cap, "%s/%s", dir, name);
    }
}

/* Compose "<base>/<sub>/" into out (trailing separator).  Returns 1 on success. */
static int graf_compose_dir(char * out, size_t cap, const char * base, const char * sub)
{
    int n = snprintf(out, cap, "%s/%s/", base, sub);
    return (n > 0 && (size_t)n < cap) ? 1 : 0;
}

/* Case-insensitive compare of the first len chars of s against a NUL-terminated
   key; true only when they are equal AND key is exactly len chars long. */
static int graf_key_is(const char * s, size_t len, const char * key)
{
    size_t i;
    for(i = 0; i < len; i++)
    {
        if(key[i] == '\0')
        {
            return 0;
        }
        if(tolower((unsigned char)s[i]) != tolower((unsigned char)key[i]))
        {
            return 0;
        }
    }
    return key[len] == '\0';
}

void STU_GRAF_Reset(void)
{
    g_search_dir_count = 0;
}

void STU_GRAF_Add_Search_Dir(const char * dir)
{
    if(dir == NULL || dir[0] == '\0')
    {
        return;
    }
    if(g_search_dir_count >= STU_GRAF_MAX_DIRS)
    {
        LOG_WARN(LOG_CAT_GENERAL, "STU_GRAF: search path full, ignoring '%s'", dir);
        return;
    }
    if(strlen(dir) + 1 > STU_GRAF_PATH_MAX)
    {
        LOG_WARN(LOG_CAT_GENERAL, "STU_GRAF: search dir too long, ignoring");
        return;
    }
    stu_strcpy(g_search_dirs[g_search_dir_count], dir);
    g_search_dir_count++;
}

FILE * STU_GRAF_Open_Asset(const char * name, const char * mode)
{
    int i;
    char full[STU_GRAF_PATH_MAX];

    /* Names that are already paths, or an unconfigured search path, resolve
       exactly as before: relative to the current working directory. */
    if(graf_name_is_path(name) || g_search_dir_count == 0)
    {
        return stu_fopen_ci(name, mode);
    }

    for(i = 0; i < g_search_dir_count; i++)
    {
        FILE * fp;

        /* "." means the current working directory -- pass the bare name so the
           byte-for-byte legacy path is preserved (important for the matchup /
           determinism tooling that runs from a data-bearing CWD). */
        if(strcmp(g_search_dirs[i], ".") == 0)
        {
            fp = stu_fopen_ci(name, mode);
        }
        else
        {
            graf_join(full, sizeof(full), g_search_dirs[i], name);
            fp = stu_fopen_ci(full, mode);
        }

        if(fp != NULL)
        {
            LOG_DEBUG(LOG_CAT_GENERAL, "STU_GRAF: '%s' resolved via '%s'",
                      name, g_search_dirs[i]);
            return fp;
        }
    }

    LOG_INFO(LOG_CAT_GENERAL, "STU_GRAF: '%s' not found on any search path", name);
    return NULL;
}

int STU_GRAF_Executable_Dir(char * out, size_t cap)
{
    char path[STU_GRAF_PATH_MAX];
    char * last_sep;
    size_t len;

#if defined(_WIN32)
    DWORD n = GetModuleFileNameA(NULL, path, (DWORD)sizeof(path));
    if(n == 0 || n >= sizeof(path))
    {
        return 0;
    }
#elif defined(__APPLE__)
    uint32_t bufsize = (uint32_t)sizeof(path);
    if(_NSGetExecutablePath(path, &bufsize) != 0)
    {
        return 0;
    }
    path[sizeof(path) - 1] = '\0';
#else
    ssize_t n = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if(n <= 0)
    {
        return 0;
    }
    path[n] = '\0';
#endif

    /* Truncate after the last path separator, keeping it as the trailing sep. */
    last_sep = strrchr(path, '/');
#if defined(_WIN32)
    {
        char * bs = strrchr(path, '\\');
        if(bs != NULL && (last_sep == NULL || bs > last_sep))
        {
            last_sep = bs;
        }
    }
#endif
    if(last_sep == NULL)
    {
        return 0;
    }
    last_sep[1] = '\0';

    len = strlen(path);
    if(len + 1 > cap)
    {
        return 0;
    }
    memcpy(out, path, len + 1);
    return 1;
}

int STU_GRAF_User_Config_Dir(char * out, size_t cap)
{
#if defined(_WIN32)
    const char * base = getenv("APPDATA");
    if(base == NULL || base[0] == '\0')
    {
        return 0;
    }
    return graf_compose_dir(out, cap, base, "ReMoM");
#elif defined(__APPLE__)
    const char * home = getenv("HOME");
    if(home == NULL || home[0] == '\0')
    {
        return 0;
    }
    return graf_compose_dir(out, cap, home, "Library/Application Support/ReMoM");
#else
    char fallback[STU_GRAF_PATH_MAX];
    const char * base = getenv("XDG_CONFIG_HOME");
    if(base == NULL || base[0] == '\0')
    {
        const char * home = getenv("HOME");
        if(home == NULL || home[0] == '\0')
        {
            return 0;
        }
        snprintf(fallback, sizeof(fallback), "%s/.config", home);
        base = fallback;
    }
    return graf_compose_dir(out, cap, base, "ReMoM");
#endif
}

int STU_GRAF_User_Cache_Dir(char * out, size_t cap)
{
#if defined(_WIN32)
    const char * base = getenv("LOCALAPPDATA");
    if(base == NULL || base[0] == '\0')
    {
        return 0;
    }
    return graf_compose_dir(out, cap, base, "ReMoM/cache");
#elif defined(__APPLE__)
    const char * home = getenv("HOME");
    if(home == NULL || home[0] == '\0')
    {
        return 0;
    }
    return graf_compose_dir(out, cap, home, "Library/Caches/ReMoM");
#else
    char fallback[STU_GRAF_PATH_MAX];
    const char * base = getenv("XDG_CACHE_HOME");
    if(base == NULL || base[0] == '\0')
    {
        const char * home = getenv("HOME");
        if(home == NULL || home[0] == '\0')
        {
            return 0;
        }
        snprintf(fallback, sizeof(fallback), "%s/.cache", home);
        base = fallback;
    }
    return graf_compose_dir(out, cap, base, "ReMoM");
#endif
}

int STU_GRAF_Read_Game_Data_From_Ini(const char * ini_path, char * out, size_t cap)
{
    FILE * fp;
    char line[STU_GRAF_PATH_MAX];
    int in_paths = 0;
    int found = 0;

    fp = fopen(ini_path, "r");
    if(fp == NULL)
    {
        return 0;
    }

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        char * p = line;
        char * eq;
        char * key_end;
        char * val;
        char * end;

        while(*p == ' ' || *p == '\t')
        {
            p++;
        }
        if(*p == '#' || *p == ';' || *p == '\n' || *p == '\r' || *p == '\0')
        {
            continue;
        }

        if(*p == '[')
        {
            char * close = strchr(p, ']');
            in_paths = (close != NULL &&
                        graf_key_is(p + 1, (size_t)(close - (p + 1)), "paths"));
            continue;
        }
        if(!in_paths)
        {
            continue;
        }

        eq = strchr(p, '=');
        if(eq == NULL)
        {
            continue;
        }
        key_end = eq;
        while(key_end > p && (key_end[-1] == ' ' || key_end[-1] == '\t'))
        {
            key_end--;
        }
        if(!graf_key_is(p, (size_t)(key_end - p), "game_data"))
        {
            continue;
        }

        /* Value = everything after '=', trimmed; keep embedded spaces (paths). */
        val = eq + 1;
        while(*val == ' ' || *val == '\t')
        {
            val++;
        }
        end = val + strlen(val);
        while(end > val &&
              (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\n' || end[-1] == '\r'))
        {
            end--;
        }
        *end = '\0';
        if(val[0] == '"' && end > val + 1 && end[-1] == '"')
        {
            end[-1] = '\0';
            val++;
        }

        if(val[0] != '\0' && strlen(val) + 1 <= cap)
        {
            stu_strcpy(out, val);
            found = 1;
        }
        break;
    }

    fclose(fp);
    return found;
}

/* Add the user's configured game-data dir (from <config>/ReMoM.ini) if present. */
static void graf_add_config_game_data(void)
{
    char cfg_dir[STU_GRAF_PATH_MAX];
    char ini_path[STU_GRAF_PATH_MAX];
    char game_data[STU_GRAF_PATH_MAX];

    if(!STU_GRAF_User_Config_Dir(cfg_dir, sizeof(cfg_dir)))
    {
        return;
    }
    graf_join(ini_path, sizeof(ini_path), cfg_dir, "ReMoM.ini");
    if(STU_GRAF_Read_Game_Data_From_Ini(ini_path, game_data, sizeof(game_data)))
    {
        STU_GRAF_Add_Search_Dir(game_data);
    }
}

/* Add the per-user cache dir (holds any ReMoM-modified copies). */
static void graf_add_cache_dir(void)
{
    char cache_dir[STU_GRAF_PATH_MAX];
    if(STU_GRAF_User_Cache_Dir(cache_dir, sizeof(cache_dir)))
    {
        STU_GRAF_Add_Search_Dir(cache_dir);
    }
}

void STU_GRAF_Init(STU_GRAF_Profile profile)
{
    const char * env_dir;
    char exe_dir[STU_GRAF_PATH_MAX];

    STU_GRAF_Reset();

    /* 1. Explicit override. */
    env_dir = getenv("REMOM_DATA_DIR");
    if(env_dir != NULL && env_dir[0] != '\0')
    {
        STU_GRAF_Add_Search_Dir(env_dir);
    }

    /* Player builds add the full stack.  Order matters: cache holds any
       ReMoM-modified copies and must shadow the originals, so it precedes the
       configured install and the portable (exe-dir) layout. */
    if(profile == STU_GRAF_PLAYER)
    {
        graf_add_cache_dir();          /* 2. XDG_CACHE_HOME/ReMoM (modified copies) */
        graf_add_config_game_data();   /* 3. [Paths] game_data (user's MoM install) */
        if(STU_GRAF_Executable_Dir(exe_dir, sizeof(exe_dir)))
        {
            STU_GRAF_Add_Search_Dir(exe_dir);  /* 4. beside the executable */
        }
    }

    /* Current working directory (legacy default; last so it never shadows an
       explicit override).  For HEADLESS this is the only entry besides an
       optional REMOM_DATA_DIR -- keeps the test / matchup harness deterministic. */
    STU_GRAF_Add_Search_Dir(".");

    LOG_INFO(LOG_CAT_GENERAL, "STU_GRAF: %s profile, %d search dir(s)",
             (profile == STU_GRAF_PLAYER) ? "PLAYER" : "HEADLESS",
             g_search_dir_count);
}
