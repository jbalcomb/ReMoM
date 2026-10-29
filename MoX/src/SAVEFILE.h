/*
    SAVEFILE -- ReMoM's own save-game format.

    A save is a header followed by tagged sections.  Each section records its tag, its record size and its record
    count, and holds the in-memory array as-is (written with sizeof), so a struct can grow without anyone converting
    it to an older layout.  Reading checks every known section's record size and count against this build and
    refuses a mismatch by name; unknown sections are skipped, so mods can add their own.

        header:   char magic[8] = "ReMoMSav", uint32_t version, uint32_t section_count
        section:  char tag[8], uint32_t record_size, uint32_t record_count, record_size * record_count bytes

    The 1.31 .GAM layout is only read, by Load_SAVE_GAM() in LOADSAVE.c, to import saves made by 1.31 or by
    earlier ReMoM builds.
*/

#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SAVEFILE_MAGIC          "ReMoMSav"
#define SAVEFILE_MAGIC_LEN      8
#define SAVEFILE_VERSION        1
#define SAVEFILE_TAG_LEN        8

/* 1 if fp starts with the ReMoM save header (the file position is restored), else 0. */
int Save_File_Is_ReMoM(FILE * fp);

/* Writes the whole game state.  Returns 0 on success, -1 on a write error (logged). */
int Save_File_Write(FILE * fp);

/* Reads a ReMoM save into the game state.  Returns 0 on success, -1 if the file is damaged or was written by a build
   whose structures differ (logged, naming the section).  file_name is only used in messages. */
int Save_File_Read(FILE * fp, const char * file_name);

/* Adds a section to every save written and read from now on (for mod data).  tag is exactly SAVEFILE_TAG_LEN
   characters and must stay valid; data must hold record_size * record_count bytes.  A save without the section loads
   with the data zeroed.  Re-registering a tag replaces it.  Returns 0 on success, -1 if the table is full. */
int Save_File_Register_Section(const char * tag, void * data, uint32_t record_size, uint32_t record_count);

/* Removes every section added with Save_File_Register_Section(). */
void Save_File_Unregister_Sections(void);

/* Imports the city section of a 1.31-layout save (100 records of 114 bytes) into _CITIES.  Returns 0 on success. */
int Save_File_Import_Cities_V131(FILE * fp);

#ifdef __cplusplus
}
#endif

#endif  /* SAVEFILE_H */
