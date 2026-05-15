#ifndef STU_INIT_H
#define STU_INIT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



/*
    LBX Sub-File Inventory
    Describes one sub-file entry within an LBX archive, for cataloguing what the start-up code loads.
*/
typedef struct s_LBX_SUBFILE_INFO
{
    int index;              /* sub-file index within the LBX */
    int size;               /* size in bytes (0 if unknown) */
    const char * label;     /* short description of what this sub-file contains */
} LBX_SUBFILE_INFO;

/*
    LBX Archive Inventory
    Catalogues one LBX file and the sub-files the start-up code needs from it.
*/
typedef struct s_LBX_ARCHIVE_INFO
{
    const char * filename;          /* e.g. "FONTS.LBX" */
    int subfile_count;              /* number of sub-files catalogued */
    const LBX_SUBFILE_INFO * subfiles;
} LBX_ARCHIVE_INFO;



/*
    Dump a human-readable inventory of the LBX data that Init_Drivers() and the
    early start-up sequence expect to load.
*/
void Dump_Init_Data_Inventory(void);

/*
    Load and validate the start-up data that Init_Drivers() would normally set up.
    Returns 0 on success, non-zero on failure.
*/
int Validate_Init_Data(const char * game_data_path);



#ifdef __cplusplus
}
#endif

#endif /* STU_INIT_H */
