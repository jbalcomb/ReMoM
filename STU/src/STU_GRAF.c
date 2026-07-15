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

    /* 2. Player builds also look beside the executable (portable ZIP layout). */
    if(profile == STU_GRAF_PLAYER)
    {
        if(STU_GRAF_Executable_Dir(exe_dir, sizeof(exe_dir)))
        {
            STU_GRAF_Add_Search_Dir(exe_dir);
        }
    }

    /* 3. Current working directory (legacy default; last so it never shadows an
          explicit override). */
    STU_GRAF_Add_Search_Dir(".");

    LOG_INFO(LOG_CAT_GENERAL, "STU_GRAF: %s profile, %d search dir(s)",
             (profile == STU_GRAF_PLAYER) ? "PLAYER" : "HEADLESS",
             g_search_dir_count);
}
