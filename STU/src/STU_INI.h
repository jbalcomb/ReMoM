/*
    STU_INI -- keeping a player's settings file up to date

    The file operations behind "one ReMoM.ini" (doc/@AI_Plans/PLAN-ReMoM-Ini.md), kept generic: no game names here, the
    caller supplies the file names and the list of settings.

      - Which file is in effect: the one next to the program if it exists, else the one in the config folder. Whole files,
        never merged.
      - Adding settings a newer release knows about: each missing one is appended to its section, commented out at its
        default with its explanation, after a timestamped backup of the file. A key counts as present whether it is live or
        commented out, so a setting the player turned off by commenting it out is not added a second time.
      - Composing a whole file from the settings: a new file (everything commented out) or the example file (defaults live).

    Silent: nothing here logs, because it runs before the log file is open (the settings file says where the log goes).
    Every function reports what it did, and the caller logs it once logging is up.

    Reading values is STU_GRAF_Read_Ini_Value().
*/

#ifndef STU_INI_H
#define STU_INI_H

#include <stddef.h>  /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

/* One setting. `value` is the default; when `value_is_example` is set it is only an example of the kind of value (a folder,
   say) and has no default, so it is written commented out even in the example file. `explanation` may hold several lines
   separated by '\n'; each is written as a "; " comment line above the setting. */
typedef struct
{
    const char * section;
    const char * key;
    const char * value;
    const char * explanation;
    int          value_is_example;
} STU_INI_Setting;

/* Return values. */
#define STU_INI_UNCHANGED  ( 0)  /* nothing to do: every setting present, or the file already has this text */
#define STU_INI_CHANGED    ( 1)  /* the file was written (and, for STU_INI_Add_Missing, backed up first) */
#define STU_INI_ERROR      (-1)  /* could not read, back up, or write; the file is left as it was */

/* 1 if a file exists at path (and can be opened for reading), else 0. */
int STU_INI_File_Exists(const char * path);

/* 1 if `key` appears in [section] of the file at path, live ("key = v") or commented out ("; key = v", "# key = v"). Section
   and key compare case-insensitively. 0 if not, or if the file cannot be read. */
int STU_INI_Has_Key(const char * path, const char * section, const char * key);

/* The same test against text already in memory. */
int STU_INI_Text_Has_Key(const char * text, const char * section, const char * key);

/* Add every setting from `settings` that the file at `path` does not have (STU_INI_Has_Key), commented out, at the end of its
   section; a missing section is appended to the end of the file with its settings. Lines already in the file are not changed
   and the file's line endings are kept. Before writing, the file is copied to "<stem>_backup_YYYYMMDDHHMMSS<ext>" in the same
   folder; that path is put in backup_path_out (empty when nothing was added). added_out, if not NULL, gets how many settings
   were added. Returns STU_INI_UNCHANGED, STU_INI_CHANGED or STU_INI_ERROR. */
int STU_INI_Add_Missing(const char * path, const STU_INI_Setting * settings, int count, char * backup_path_out, size_t backup_cap, int * added_out);

/* Compose a whole settings file: `header` (each line written as a "; " comment; may be NULL), then each section in the order
   it first appears in `settings`, each setting with its explanation. commented_out = 1: every setting commented out (a new
   player file); 0: defaults live, examples commented out (the example file). Lines end with `eol` ("\r\n" or "\n").
   Returns a malloc'd string the caller frees, or NULL if out of memory. */
char * STU_INI_Compose(const char * header, const STU_INI_Setting * settings, int count, int commented_out, const char * eol);

/* Write `text` to path unless the file already holds exactly that text. Creates the file if needed (not its folder).
   Returns STU_INI_UNCHANGED, STU_INI_CHANGED or STU_INI_ERROR. */
int STU_INI_Write_If_Different(const char * path, const char * text);

/* The line ending new files use on this platform: "\r\n" on Windows, "\n" elsewhere. */
const char * STU_INI_Native_Eol(void);

#ifdef __cplusplus
}
#endif

#endif /* STU_INI_H */
