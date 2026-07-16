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

    Two path families live here, and must not be conflated:
      - the read-only asset search path (REMOM_DATA_DIR -> cache -> [Paths]
        game_data -> exe-dir -> CWD) via STU_GRAF_Open_Asset(); and
      - the writable per-user family (STU_GRAF_User_Data_Dir / _Open_User /
        _User_DIR / _User_LOF / _Seed_User_File) rooted at XDG_DATA_HOME, which
        holds the mutable CONFIG.MOM/MAGIC.SET/save copies seeded on first run.
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

/* Per-user *data* directory (with a trailing separator): the writable home for
   the game's mutable files (CONFIG.MOM working copy, MAGIC.SET, save games).
   XDG_DATA_HOME/ReMoM on Linux, %APPDATA%\ReMoM on Windows,
   ~/Library/Application Support/ReMoM on macOS.  Returns 1 on success. */
int STU_GRAF_User_Data_Dir(char * out, size_t cap);

/* Per-user *state* directory (with a trailing separator): the writable home for
   logs.  XDG_STATE_HOME/ReMoM on Linux, %LOCALAPPDATA%\ReMoM\logs on Windows,
   ~/Library/Logs/ReMoM on macOS.  Unlike the other resolvers this also *creates*
   the directory (the log writer opens files here but does not itself mkdir).
   Returns 1 on success. */
int STU_GRAF_User_State_Dir(char * out, size_t cap);

/* Open a writable per-user file by bare name (e.g. "MAGIC.SET", "SAVE1.GAM"),
   resolved into the user-data dir (created on demand for write modes).  Same
   signature as stu_fopen_ci, so the reconstructed write sites swap
   stu_fopen_ci -> this.  A name that already carries a path is opened directly.
   Under the HEADLESS profile (HeMoM / tests / matchup) this degrades to a plain
   CWD-relative open -- byte-identical to the pre-swap behavior -- as does a
   PLAYER build that cannot resolve a user-data dir. */
FILE * STU_GRAF_Open_User(const char * name, const char * mode);

/* Return values for STU_GRAF_User_DIR: chosen to match MoX's DIR() contract
   (ST_SUCCESS = -1 found, ST_FAILURE = 1 absent) so reconstructed call sites
   swap DIR() -> STU_GRAF_User_DIR() with no change to their comparisons.  (STU
   does not include the MoX headers, hence the local names.) */
#define STU_GRAF_DIR_FOUND  (-1)
#define STU_GRAF_DIR_ABSENT ( 1)

/* User-data-aware equivalents of MoX's DIR()/LOF(), so an existence / length
   check beside a swapped open resolves to the same place the open uses (the
   user-data dir for PLAYER, the CWD for HEADLESS) rather than always the CWD.
   STU_GRAF_User_DIR fills found (name on hit, "" on miss) and returns
   STU_GRAF_DIR_FOUND / STU_GRAF_DIR_ABSENT.  STU_GRAF_User_LOF returns the byte
   length, or 0 when absent -- identical to LOF(). */
int  STU_GRAF_User_DIR(const char * name, char * found);
long STU_GRAF_User_LOF(const char * name);

/* First-run seeding: if <user-data>/<name> is absent, copy it there from the
   read-only search path (STU_GRAF_Open_Asset).  Returns 1 when the user copy
   exists afterwards (already-present or freshly seeded), 0 otherwise.  The
   original on the search path is never modified. */
int STU_GRAF_Seed_User_File(const char * name);

/* Read the [Paths] game_data value from the given INI file.  Returns 1 and
   fills out on success, 0 otherwise (missing file/section/key).  Section and
   key match case-insensitively.  Exposed for testing. */
int STU_GRAF_Read_Game_Data_From_Ini(const char * ini_path, char * out, size_t cap);

/* Return the first directory in the NULL-terminated `candidates` list that looks
   like a Master of Magic install (contains FONTS.LBX), or NULL if none do.  The
   PLAYER profile calls this over a built-in list of common GOG/Steam/CD install
   locations to auto-detect the game data with zero configuration.  Exposed for
   testing. */
const char * STU_GRAF_First_Game_Data_Dir(const char * const * candidates);

/* One known-good hash for one file+distribution.  The known-good manifest is a
   NULL-terminated array of these, COMPILED INTO the binary (no external file to
   go missing or drift) -- see lbx_manifest.c, regenerated by the `lbx_hashes`
   tool.  Multiple rows per `name` (one per known distribution) are expected; a
   `version` tag containing "v1.31" marks a supported distribution. */
typedef struct
{
    const char * name;      /* e.g. "FONTS.LBX"                      */
    const char * sha256;    /* lower/upper-case hex, 64 chars        */
    const char * version;   /* e.g. "v1.31-floppy-1995"; NULL = end  */
} STU_LBX_Manifest_Entry;

/* The compiled-in known-good manifest (lbx_manifest.c).  Terminated by an entry
   whose name is NULL.  An all-terminator (empty) table means "no manifest yet"
   -- the compat pass then does nothing. */
extern const STU_LBX_Manifest_Entry g_lbx_manifest[];

/* Result of the (non-blocking) data-compatibility pass. */
typedef struct
{
    int  manifest_found;  /* 0 = embedded manifest is empty -> pass skipped */
    int  files_checked;   /* installed files the manifest had an entry for */
    int  ok_count;        /* matched a SUPPORTED (v1.31) known-good hash */
    int  problem_count;   /* unrecognized hash, or matched an unsupported version */
    char summary[512];    /* one problem file per "; " for the warning dialog/log */
} STU_GRAF_Compat_Report;

/* Compare the installed game data against the compiled-in known-good manifest
   (g_lbx_manifest).  For each manifest-listed file that resolves on the read
   search path, SHA-256 it and classify it: matches a v1.31 hash (ok), matches
   only an older/unsupported hash (problem: wrong version), or matches nothing
   (problem: unrecognized).

   Non-blocking by contract: this only *reports*.  Fills *report and returns
   problem_count.  An empty embedded manifest leaves manifest_found = 0 and is a
   silent no-op -- so a build whose manifest hasn't been authored yet never
   nags. */
int STU_GRAF_Check_Data_Compat(STU_GRAF_Compat_Report * report);

/* Same, but against a caller-supplied manifest array (NULL-terminated).  The
   public entry point above is just this called with g_lbx_manifest; exposed so
   tests can drive it with a synthetic manifest. */
int STU_GRAF_Check_Compat_Against(const STU_LBX_Manifest_Entry * manifest,
                                  STU_GRAF_Compat_Report * report);

#ifdef __cplusplus
}
#endif

#endif /* STU_GRAF_H */
