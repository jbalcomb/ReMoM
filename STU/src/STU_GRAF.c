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
#include "STU_HASH.h"  /* STU_SHA256_Stream() -- data-compatibility pass */

#include <string.h>
#include <stdlib.h>   /* getenv */
#include <ctype.h>    /* tolower */
#include <errno.h>    /* EEXIST  */

#if defined(_WIN32)
#include <direct.h>   /* _mkdir  */
#else
#include <sys/stat.h> /* mkdir   */
#include <sys/types.h>
#endif

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

/* Selected by STU_GRAF_Init(); gates the writable user-family.  Defaults to
   HEADLESS so any code that never calls STU_GRAF_Init() -- HeMoM, the unit-test
   binaries, the matchup harness -- keeps the original CWD-relative behavior.
   The user-data redirect is therefore inert until ReMoMber opts in with
   STU_GRAF_Init(STU_GRAF_PLAYER). */
static STU_GRAF_Profile g_profile = STU_GRAF_HEADLESS;

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
    g_search_dir_count = 0;  /* profile is owned by STU_GRAF_Init(), not reset here */
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

int STU_GRAF_User_Data_Dir(char * out, size_t cap)
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
    const char * base = getenv("XDG_DATA_HOME");
    if(base == NULL || base[0] == '\0')
    {
        const char * home = getenv("HOME");
        if(home == NULL || home[0] == '\0')
        {
            return 0;
        }
        snprintf(fallback, sizeof(fallback), "%s/.local/share", home);
        base = fallback;
    }
    return graf_compose_dir(out, cap, base, "ReMoM");
#endif
}

/* Create a single directory; success is "now exists" (EEXIST counts). */
static int graf_mkdir_one(const char * path)
{
#if defined(_WIN32)
    if(_mkdir(path) == 0)
    {
        return 1;
    }
#else
    if(mkdir(path, 0777) == 0)
    {
        return 1;
    }
#endif
    return (errno == EEXIST) ? 1 : 0;
}

/* Recursively create every component of dir (like `mkdir -p`).  Intermediate
   failures are tolerated; the return reflects whether the final dir exists. */
static int graf_mkpath(const char * dir)
{
    char tmp[STU_GRAF_PATH_MAX];
    size_t len;
    size_t i;

    if(dir == NULL || dir[0] == '\0')
    {
        return 0;
    }
    len = strlen(dir);
    if(len + 1 > sizeof(tmp))
    {
        return 0;
    }
    memcpy(tmp, dir, len + 1);
    /* Drop any trailing separator so the loop's final component is the dir. */
    while(len > 1 && (tmp[len - 1] == '/' || tmp[len - 1] == '\\'))
    {
        tmp[--len] = '\0';
    }

    for(i = 1; i < len; i++)
    {
        if(tmp[i] == '/' || tmp[i] == '\\')
        {
            char sep = tmp[i];
            tmp[i] = '\0';
            /* Skip roots like "C:" that mkdir can't (and needn't) create. */
            if(!(i == 2 && tmp[1] == ':'))
            {
                graf_mkdir_one(tmp);
            }
            tmp[i] = sep;
        }
    }
    return graf_mkdir_one(tmp);
}

int STU_GRAF_User_State_Dir(char * out, size_t cap)
{
    /* Placed after graf_mkpath so it can create the dir on resolution -- the log
       writer (STU_LOG) opens files here but does not itself mkdir. */
#if defined(_WIN32)
    const char * base = getenv("LOCALAPPDATA");
    if(base == NULL || base[0] == '\0')
    {
        return 0;
    }
    if(!graf_compose_dir(out, cap, base, "ReMoM/logs"))
    {
        return 0;
    }
#elif defined(__APPLE__)
    const char * home = getenv("HOME");
    if(home == NULL || home[0] == '\0')
    {
        return 0;
    }
    if(!graf_compose_dir(out, cap, home, "Library/Logs/ReMoM"))
    {
        return 0;
    }
#else
    char fallback[STU_GRAF_PATH_MAX];
    const char * base = getenv("XDG_STATE_HOME");
    if(base == NULL || base[0] == '\0')
    {
        const char * home = getenv("HOME");
        if(home == NULL || home[0] == '\0')
        {
            return 0;
        }
        snprintf(fallback, sizeof(fallback), "%s/.local/state", home);
        base = fallback;
    }
    if(!graf_compose_dir(out, cap, base, "ReMoM"))
    {
        return 0;
    }
#endif
    graf_mkpath(out);  /* ensure it exists for the log writer */
    return 1;
}

FILE * STU_GRAF_Open_User(const char * name, const char * mode)
{
    char dir[STU_GRAF_PATH_MAX];
    char full[STU_GRAF_PATH_MAX];

    /* A name that is already a path is honored verbatim. */
    if(graf_name_is_path(name))
    {
        return stu_fopen_ci(name, mode);
    }

    /* HEADLESS (HeMoM / tests / matchup) keeps the original CWD-relative
       behavior so the determinism tooling is byte-for-byte unaffected. */
    if(g_profile == STU_GRAF_HEADLESS)
    {
        return stu_fopen_ci(name, mode);
    }

    if(!STU_GRAF_User_Data_Dir(dir, sizeof(dir)))
    {
        /* No HOME/APPDATA -- degrade to legacy CWD behavior rather than fail. */
        LOG_WARN(LOG_CAT_GENERAL,
                 "STU_GRAF: no user-data dir; opening '%s' in CWD", name);
        return stu_fopen_ci(name, mode);
    }

    /* Ensure the destination exists for writes/appends. */
    if(mode != NULL &&
       (mode[0] == 'w' || mode[0] == 'a' || mode[0] == 'W' || mode[0] == 'A'))
    {
        graf_mkpath(dir);
    }

    graf_join(full, sizeof(full), dir, name);
    return stu_fopen_ci(full, mode);
}

int STU_GRAF_User_DIR(const char * name, char * found)
{
    FILE * fp = STU_GRAF_Open_User(name, "rb");
    if(fp == NULL)
    {
        if(found != NULL)
        {
            found[0] = '\0';
        }
        return STU_GRAF_DIR_ABSENT;
    }
    fclose(fp);
    if(found != NULL)
    {
        /* Mirror MoX DIR(): report the requested name as the match. */
        stu_strcpy(found, name);
    }
    return STU_GRAF_DIR_FOUND;
}

long STU_GRAF_User_LOF(const char * name)
{
    long size = 0;
    FILE * fp = STU_GRAF_Open_User(name, "rb");
    if(fp != NULL)
    {
        if(fseek(fp, 0, SEEK_END) == 0)
        {
            long pos = ftell(fp);
            if(pos >= 0)
            {
                size = pos;
            }
        }
        fclose(fp);
    }
    return size;
}

int STU_GRAF_Seed_User_File(const char * name)
{
    char dir[STU_GRAF_PATH_MAX];
    char full[STU_GRAF_PATH_MAX];
    FILE * src;
    FILE * dst;
    char buf[4096];
    size_t n;

    /* No seeding under HEADLESS: reads already resolve to the CWD copy. */
    if(g_profile == STU_GRAF_HEADLESS)
    {
        return 1;
    }

    if(!STU_GRAF_User_Data_Dir(dir, sizeof(dir)))
    {
        return 0;
    }
    graf_mkpath(dir);
    graf_join(full, sizeof(full), dir, name);

    /* Already seeded?  Never clobber the user's working copy. */
    dst = stu_fopen_ci(full, "rb");
    if(dst != NULL)
    {
        fclose(dst);
        return 1;
    }

    /* Pull the original from the read-only search path (never modified). */
    src = STU_GRAF_Open_Asset(name, "rb");
    if(src == NULL)
    {
        LOG_INFO(LOG_CAT_GENERAL,
                 "STU_GRAF: nothing to seed for '%s' (no original found)", name);
        return 0;
    }
    dst = stu_fopen_ci(full, "wb");
    if(dst == NULL)
    {
        fclose(src);
        LOG_WARN(LOG_CAT_GENERAL, "STU_GRAF: cannot create user copy of '%s'", name);
        return 0;
    }
    while((n = fread(buf, 1, sizeof(buf), src)) > 0)
    {
        if(fwrite(buf, 1, n, dst) != n)
        {
            LOG_ERROR(LOG_CAT_GENERAL, "STU_GRAF: short write seeding '%s'", name);
            fclose(src);
            fclose(dst);
            return 0;
        }
    }
    fclose(src);
    fclose(dst);
    LOG_INFO(LOG_CAT_GENERAL, "STU_GRAF: seeded user copy of '%s'", name);
    return 1;
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

/* True if `dir` looks like a Master of Magic install (has the signature LBX). */
static int graf_dir_has_signature(const char * dir)
{
    char path[STU_GRAF_PATH_MAX];
    FILE * fp;
    if(dir == NULL || dir[0] == '\0')
    {
        return 0;
    }
    graf_join(path, sizeof(path), dir, "FONTS.LBX");
    fp = stu_fopen_ci(path, "rb");
    if(fp != NULL)
    {
        fclose(fp);
        return 1;
    }
    return 0;
}

const char * STU_GRAF_First_Game_Data_Dir(const char * const * candidates)
{
    int i;
    if(candidates == NULL)
    {
        return NULL;
    }
    for(i = 0; candidates[i] != NULL; i++)
    {
        if(graf_dir_has_signature(candidates[i]))
        {
            return candidates[i];
        }
    }
    return NULL;
}

/* Best-effort: probe well-known install locations and add the first real one to
   the search path, so a standard install (or a dev's repo checkout) is found
   with zero configuration.  Non-standard installs use REMOM_DATA_DIR or the
   ReMoM.ini game_data path.  PLAYER only (see STU_GRAF_Init). */
static void graf_add_autodetected_game_data(void)
{
    const char * hit;

    /* Developer convenience, checked first: a repo checkout keeps the game data
       in ./assets (which also matches the checksum manifest exactly).  Relative
       to the CWD, so it works on every OS. */
    static const char * const dev_cands[] =
    {
        "assets", "../assets", "../../assets", NULL
    };

#if defined(_WIN32)
    char b0[STU_GRAF_PATH_MAX];
    const char * pf86 = getenv("ProgramFiles(x86)");
    const char * cands[6];
    int n = 0;
    cands[n++] = "C:\\GOG Games\\Master of Magic";
    if(pf86 != NULL && pf86[0] != '\0')
    {
        snprintf(b0, sizeof(b0),
                 "%s\\Steam\\steamapps\\common\\Master of Magic Classic", pf86);
        cands[n++] = b0;
    }
    cands[n++] = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Master of Magic Classic";
    cands[n++] = "C:\\MPS\\MAGIC";
    cands[n] = NULL;
#elif defined(__APPLE__)
    char b0[STU_GRAF_PATH_MAX];
    char b1[STU_GRAF_PATH_MAX];
    const char * home = getenv("HOME");
    const char * cands[3];
    int n = 0;
    if(home != NULL && home[0] != '\0')
    {
        snprintf(b0, sizeof(b0), "%s/GOG Games/Master of Magic", home);
        cands[n++] = b0;
        snprintf(b1, sizeof(b1),
                 "%s/Library/Application Support/Steam/steamapps/common/Master of Magic Classic", home);
        cands[n++] = b1;
    }
    cands[n] = NULL;
#else
    char b0[STU_GRAF_PATH_MAX];
    char b1[STU_GRAF_PATH_MAX];
    char b2[STU_GRAF_PATH_MAX];
    char b3[STU_GRAF_PATH_MAX];
    const char * home = getenv("HOME");
    const char * cands[5];
    int n = 0;
    if(home != NULL && home[0] != '\0')
    {
        snprintf(b0, sizeof(b0), "%s/GOG Games/Master of Magic", home);
        cands[n++] = b0;
        snprintf(b1, sizeof(b1), "%s/Games/Master of Magic", home);
        cands[n++] = b1;
        snprintf(b2, sizeof(b2),
                 "%s/.steam/steam/steamapps/common/Master of Magic Classic", home);
        cands[n++] = b2;
        snprintf(b3, sizeof(b3),
                 "%s/.local/share/Steam/steamapps/common/Master of Magic Classic", home);
        cands[n++] = b3;
    }
    cands[n] = NULL;
#endif

    /* Prefer a dev repo checkout (./assets) over an installed copy, then fall
       back to the OS install locations. */
    hit = STU_GRAF_First_Game_Data_Dir(dev_cands);
    if(hit == NULL)
    {
        hit = STU_GRAF_First_Game_Data_Dir(cands);
    }
    if(hit != NULL)
    {
        LOG_INFO(LOG_CAT_GENERAL, "STU_GRAF: auto-detected game data at '%s'", hit);
        STU_GRAF_Add_Search_Dir(hit);
    }
}

void STU_GRAF_Init(STU_GRAF_Profile profile)
{
    const char * env_dir;
    char exe_dir[STU_GRAF_PATH_MAX];

    STU_GRAF_Reset();
    g_profile = profile;

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
        graf_add_autodetected_game_data(); /* 5. common GOG/Steam/CD install paths */
    }

    /* Current working directory (legacy default; last so it never shadows an
       explicit override).  For HEADLESS this is the only entry besides an
       optional REMOM_DATA_DIR -- keeps the test / matchup harness deterministic. */
    STU_GRAF_Add_Search_Dir(".");

    LOG_INFO(LOG_CAT_GENERAL, "STU_GRAF: %s profile, %d search dir(s)",
             (profile == STU_GRAF_PLAYER) ? "PLAYER" : "HEADLESS",
             g_search_dir_count);
}

/* ---- Data-compatibility pass (checksum manifest) ------------------------- */

/* A version-tag containing this token counts as a supported distribution.  A
   file that only matches hashes without it is flagged "wrong version". */
#define STU_GRAF_SUPPORTED_VERSION "v1.31"
#define STU_GRAF_COMPAT_MAX_FILES  256

typedef struct
{
    char name[64];
    char actual_hash[65];
    int  present;      /* file resolved on the read search path */
    int  recognized;   /* matched some manifest hash (any version) */
    int  supported;    /* matched a hash tagged with the supported version */
} graf_compat_file;

/* Case-insensitive equality of two NUL-terminated strings. */
static int graf_str_ieq(const char * a, const char * b)
{
    while(*a != '\0' && *b != '\0')
    {
        if(tolower((unsigned char)*a) != tolower((unsigned char)*b))
        {
            return 0;
        }
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

/* Case-insensitive: is needle a substring of hay? */
static int graf_ci_contains(const char * hay, const char * needle)
{
    size_t nlen = strlen(needle);
    if(nlen == 0)
    {
        return 1;
    }
    for(; *hay != '\0'; hay++)
    {
        size_t i = 0;
        while(i < nlen && hay[i] != '\0' &&
              tolower((unsigned char)hay[i]) == tolower((unsigned char)needle[i]))
        {
            i++;
        }
        if(i == nlen)
        {
            return 1;
        }
    }
    return 0;
}

int STU_GRAF_Check_Compat_Against(const STU_LBX_Manifest_Entry * manifest,
                                  STU_GRAF_Compat_Report * report)
{
    graf_compat_file files[STU_GRAF_COMPAT_MAX_FILES];
    int nfiles = 0;
    int e;
    int i;

    if(report == NULL)
    {
        return 0;
    }
    memset(report, 0, sizeof(*report));

    if(manifest == NULL || manifest[0].name == NULL)
    {
        /* Empty embedded manifest -> not authored yet; stay silent. */
        LOG_INFO(LOG_CAT_GENERAL, "STU_GRAF: empty compat manifest; skipping data check");
        return 0;
    }
    report->manifest_found = 1;

    for(e = 0; manifest[e].name != NULL; e++)
    {
        const char * name = manifest[e].name;
        const char * hash = manifest[e].sha256;
        const char * tag  = (manifest[e].version != NULL) ? manifest[e].version : "";
        graf_compat_file * rec = NULL;

        if(hash == NULL || name[0] == '\0')
        {
            continue;  /* malformed row */
        }

        for(i = 0; i < nfiles; i++)
        {
            if(graf_str_ieq(files[i].name, name))
            {
                rec = &files[i];
                break;
            }
        }
        if(rec == NULL)
        {
            FILE * afp;
            if(nfiles >= STU_GRAF_COMPAT_MAX_FILES)
            {
                LOG_WARN(LOG_CAT_GENERAL,
                         "STU_GRAF: compat manifest exceeds %d files; truncating check",
                         STU_GRAF_COMPAT_MAX_FILES);
                break;
            }
            rec = &files[nfiles++];
            stu_strcpy(rec->name, name);
            rec->actual_hash[0] = '\0';
            rec->present = 0;
            rec->recognized = 0;
            rec->supported = 0;

            /* Hash the installed file once, on first sight of its name. */
            afp = STU_GRAF_Open_Asset(name, "rb");
            if(afp != NULL)
            {
                if(STU_SHA256_Stream(afp, rec->actual_hash))
                {
                    rec->present = 1;
                }
                fclose(afp);
            }
        }

        if(rec->present && graf_str_ieq(rec->actual_hash, hash))
        {
            rec->recognized = 1;
            if(graf_ci_contains(tag, STU_GRAF_SUPPORTED_VERSION))
            {
                rec->supported = 1;
            }
        }
    }

    for(i = 0; i < nfiles; i++)
    {
        graf_compat_file * rec = &files[i];
        const char * why;
        size_t used;
        size_t need;

        if(!rec->present)
        {
            continue;  /* not installed -> not this pass's concern */
        }
        report->files_checked++;
        if(rec->supported)
        {
            report->ok_count++;
            continue;
        }

        report->problem_count++;
        why = rec->recognized ? " (wrong/unsupported version)" : " (unrecognized)";
        LOG_WARN(LOG_CAT_GENERAL, "STU_GRAF: data compat: %s%s", rec->name, why);

        used = strlen(report->summary);
        need = strlen(rec->name) + strlen(why) + 2;
        if(used + need < sizeof(report->summary))
        {
            if(used > 0)
            {
                stu_strcat(report->summary, "; ");
            }
            stu_strcat(report->summary, rec->name);
            stu_strcat(report->summary, why);
        }
    }

    LOG_INFO(LOG_CAT_GENERAL,
             "STU_GRAF: compat pass -- %d checked, %d ok, %d problem(s)",
             report->files_checked, report->ok_count, report->problem_count);
    return report->problem_count;
}

int STU_GRAF_Check_Data_Compat(STU_GRAF_Compat_Report * report)
{
    return STU_GRAF_Check_Compat_Against(g_lbx_manifest, report);
}
