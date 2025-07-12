
// minwinbase.h  WIN32_FIND_DATAA
// winnt.h  HANDLE
// fileapi.h  GetFileSize()
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "MOM.h"

#ifdef STU_DEBUG
#include "STU/STU_DBG.h"    /* DLOG() */
#endif



// MGC s03p05
// WZD s03p05
int32_t LOF(char * file_name)
{
    HANDLE hFile;
    int32_t size;

    Save_Mouse_State();

    // HANDLE CreateFileA(
    //   [in]           LPCSTR                lpFileName,
    //   [in]           DWORD                 dwDesiredAccess,
    //   [in]           DWORD                 dwShareMode,
    //   [in, optional] LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    //   [in]           DWORD                 dwCreationDisposition,
    //   [in]           DWORD                 dwFlagsAndAttributes,
    //   [in, optional] HANDLE                hTemplateFile
    // );
    // dwDesiredAccess (GENERIC_READ | GENERIC_WRITE)
    hFile = CreateFileA(
                file_name,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
    );
    if (hFile == INVALID_HANDLE_VALUE)
    {
#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: FAILURE CreateFile(%s): %d\n", __FILE__, __LINE__, file_name, GetLastError());
        // ERROR_FILE_NOT_FOUND
        // 2 (0x2)
#endif
        return 0;  // ST_FAILURE
    }

    if (hFile && hFile != INVALID_HANDLE_VALUE)
    {

        DWORD lodwFileSize = 0;
        DWORD hidwFileSize = 0;

        lodwFileSize = GetFileSize(hFile, &hidwFileSize);


        // TODO  WTF? warning C4047: '==': 'void *' differs in levels of indirection from 'DWORD'
        // if(lodwFileSize == INVALID_FILE_SIZE && NULL == hidwFileSize)
        if (lodwFileSize == INVALID_FILE_SIZE && 0 == hidwFileSize)
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

// MGC s04p01
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
