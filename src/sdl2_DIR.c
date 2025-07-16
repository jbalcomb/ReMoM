#include "MOX/DIR.h"

#include "MOX/Mouse.h"
#include "MOX/MOX_DEF.h"
#include "MOX/MOX_TYPE.h"

#include "MOM_PFL.h"

// minwinbase.h  WIN32_FIND_DATAA
// winnt.h  HANDLE
// fileapi.h  GetFileSize()
#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#endif

#include <stdio.h>

#include <SDL.h>



// WZD s03p05
int32_t LOF(char * file_name)
{
    HANDLE hFile;
    int32_t size;

    Save_Mouse_State();

    hFile = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    // //Open file for reading in binary
    // SDL_RWops * file = SDL_RWFromFile( "33_file_reading_and_writing/nums.bin", "r+b" );
    // // NULL on failure; SDL_GetError() for more information
    // //File does not exist
    // if( file == NULL )
    // {
    //     printf( "Warning: Unable to open file! SDL Error: %s\n", SDL_GetError() );
    //     
    //     //Create file for writing
    //     file = SDL_RWFromFile( "33_file_reading_and_writing/nums.bin", "w+b" );
    // }

    if(hFile == INVALID_HANDLE_VALUE)
    {
        return 0;  // ST_FAILURE
    }

    if(hFile && hFile != INVALID_HANDLE_VALUE)
    {

        DWORD lodwFileSize = 0;
        DWORD hidwFileSize = 0;

        lodwFileSize = GetFileSize(hFile, &hidwFileSize);

        if(lodwFileSize == INVALID_FILE_SIZE && 0 == hidwFileSize)
        {
            size = 0;  // ST_FAILURE
        }
        else
        {
            size = (long)lodwFileSize;
        }

        CloseHandle(hFile);

    }
    else
    {
        size = 0;  // ST_FAILURE
    }

    Restore_Mouse_State();

    return size;

}

// WZD s04p01
/*
    returns ST_SUCCESS(0), ST_FAILURE(1)
*/
int16_t DIR(char * match_string, char * found_file)
{
    int16_t st_status;
    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind;
    char * rvr_cFileName;

    Save_Mouse_State();

    // NIU TBD TODO if(match_string == NULL) { findnext() }

    hFind = FindFirstFileA(match_string, &FindFileData);
    if(hFind == INVALID_HANDLE_VALUE)
    {
        found_file[0] = '\0';
        st_status = ST_FAILURE;
    }
    else
    {
        rvr_cFileName = FindFileData.cFileName;
        while(*rvr_cFileName)
        {
            *(found_file++) = *(rvr_cFileName++);
        }
        *found_file = '\0';
        st_status = ST_SUCCESS;
    }

    Restore_Mouse_State();

    return st_status;
}
