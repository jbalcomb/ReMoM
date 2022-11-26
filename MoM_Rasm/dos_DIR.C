
#include "ST_GUI.H"     /* MD_CDraw_Disable(), MD_CDraw_Restore() */

#include <dir.h>        /* */
#include <dos.h>        /* */
#include <fcntl.h>      /* O_RDONLY, O_BINARY */
#include <io.h>         /* filelength() */
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fwrite(); */

// MGC s03p05
// WZD s03p05
long LOF(const char * file_name)
{
    int handle;
    long size;

#ifdef DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: LOF(file_name = %s)\n", __FILE__, __LINE__, file_name);
#endif

    MD_CDraw_Disable();


    handle = open(file_name, O_RDONLY | O_BINARY);

    size = filelength(handle);

    close(handle);

    if(size == -1)
    {
        size = 0;
    }


    MD_CDraw_Restore();

#ifdef DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: DIR(file_name = %s) { size = %ld } \n", __FILE__, __LINE__, file_name, size);
#endif

    return size;
}


// MGC s04p01
// WZD s04p01
int DIR(const char * match_string, char * found_file)
{
    int st_status;
    struct ffblk ffblk;
    char * rvr_ff_name;

#ifdef DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: DIR(match_string = %s, found_file = %s)\n", __FILE__, __LINE__, match_string, found_file);
#endif

    MD_CDraw_Disable();


    // NIU TBD TODO if(match_string == NULL) { findnext() }

    if(findfirst(match_string, &ffblk, 0) != 0)
    {
        found_file[0] = '\0';
        st_status = 0;  // ST_FAILURE
    }
    else
    {
        rvr_ff_name = ffblk.ff_name;
        // while(*ffblk.ff_name)
        while(*rvr_ff_name)
        {
            // *(found_file++) = *(ffblk.ff_name++);  // Error: LValue required
            *(found_file++) = *(rvr_ff_name++);
        }
        *found_file = '\0';
        st_status = 1;  // ST_SUCCESS
    }


    MD_CDraw_Restore();

#ifdef DEBUG
    dbg_prn("DEBUG: [%s, %d] END: DIR(match_string = %s, found_file = %s)\n", __FILE__, __LINE__, match_string, found_file);
#endif

    return st_status;

}
