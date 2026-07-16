/*
    STU_GRAF -- Game Resource Archive File locator

    Shared home for OS/platform path-and-place resolution and the game-data open
    functions.  The reconstructed game code opens resources by bare name (e.g.
    "FONTS.LBX"); STU_GRAF is the layer that knows those names may live in one of
    several directories now (an installed/portable player no longer has to run
    from the data folder).  It resolves a bare name against an ordered search
    path and opens the first match via stu_fopen_ci(), leaving the ext/ compat
    shim a pure libc-portability leaf and keeping this policy out of the SDL /
    Win32 backends.

    Phase 1 scope: the read-only asset search path (REMOM_DATA_DIR -> executable
    directory -> CWD) plus STU_GRAF_Open_Asset().  The writable-user family,
    config/cache search entries, XDG dir resolution, and seeding arrive in later
    phases.
*/

#ifndef STU_GRAF_H
#define STU_GRAF_H

#include <stdio.h>   /* FILE   */
#include <stddef.h>  /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

/* Selects the default data-location policy applied by STU_GRAF_Init(). */
typedef enum
{
    STU_GRAF_PLAYER   = 0,  /* installed player build: env -> exe-dir -> CWD */
    STU_GRAF_HEADLESS = 1   /* HeMoM / tests / matchup: env -> CWD          */
} STU_GRAF_Profile;

/* Build the standard ordered read search path for the given profile.  Resets
   any existing path first, so it is safe to call more than once. */
void STU_GRAF_Init(STU_GRAF_Profile profile);

/* Low-level search-path management (used by STU_GRAF_Init and by tests). */
void STU_GRAF_Reset(void);
void STU_GRAF_Add_Search_Dir(const char * dir);

/* Open a read-only game-resource file by bare name (e.g. "FONTS.LBX").  Walks
   the search path in order and returns the first match (opened case-insensitively
   via stu_fopen_ci); NULL if not found on any entry.  A name that already carries
   a path separator, or an empty search path, is opened directly -- identical to
   the pre-STU_GRAF (CWD-relative) behavior. */
FILE * STU_GRAF_Open_Asset(const char * name, const char * mode);

/* Absolute directory of the running executable, with a trailing separator.
   Returns 1 on success, 0 on failure. */
int STU_GRAF_Executable_Dir(char * out, size_t cap);

/* Per-user config / cache directories (with a trailing separator), resolved
   from the platform conventions: XDG on Linux, %APPDATA% / %LOCALAPPDATA% on
   Windows, ~/Library on macOS.  Return 1 on success.  Public so STU_LOG and
   tests can reuse them. */
int STU_GRAF_User_Config_Dir(char * out, size_t cap);
int STU_GRAF_User_Cache_Dir(char * out, size_t cap);

/* Read the [Paths] game_data value from the given INI file.  Returns 1 and
   fills out on success, 0 otherwise (missing file/section/key).  Section and
   key match case-insensitively.  Exposed for testing. */
int STU_GRAF_Read_Game_Data_From_Ini(const char * ini_path, char * out, size_t cap);

#ifdef __cplusplus
}
#endif

#endif /* STU_GRAF_H */
