
// minwinbase.h  WIN32_FIND_DATAA
// winnt.h  HANDLE
// fileapi.h  GetFileSize()
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "MoX_TYPE.H"

// #include "ST_GUI.H"     /* MD_CDraw_Disable(), MD_CDraw_Restore() */



// MGC s03p05
// WZD s03p05
int32_t LOF(char * file_name)
{
    HANDLE hFile;
    int32_t size;

#ifdef DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: LOF(file_name = %s)\n", __FILE__, __LINE__, file_name);
#endif

    // MD_CDraw_Disable();

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
    hFile = CreateFile(
                file_name,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
    );

    DWORD lodwFileSize = 0;
    DWORD hidwFileSize = 0;

    lodwFileSize = GetFileSize(hFile, &hidwFileSize);

    // if(hfile && hfile != INVALID_HANDLE_VALUE)
    
    // TODO  WTF? warning C4047: '==': 'void *' differs in levels of indirection from 'DWORD'
    if(lodwFileSize == INVALID_FILE_SIZE && NULL == hidwFileSize)
    {
        size = 0;  // ST_FAILURE
    }
    else
    {
        size = (long)lodwFileSize;
    }

    CloseHandle(hFile);


    // MD_CDraw_Restore();

#ifdef DEBUG
    dbg_prn("DEBUG: [%s, %d] END: LOF(file_name = %s) { size = %ld } \n", __FILE__, __LINE__, file_name, size);
#endif

    return size;

}

// MGC s04p01
// WZD s04p01
int16_t DIR(char * match_string, char * found_file)
{
    int16_t st_status;
    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind;
    char * rvr_cFileName;

#ifdef DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: DIR(match_string = %s, found_file = %s)\n", __FILE__, __LINE__, match_string, found_file);
#endif

    // MD_CDraw_Disable();


    // NIU TBD TODO if(match_string == NULL) { findnext() }

    hFind = FindFirstFileA(match_string, &FindFileData);
    if(hFind == INVALID_HANDLE_VALUE)
    {
        found_file[0] = '\0';
        st_status = 0;  // ST_FAILURE
    }
    else
    {
        rvr_cFileName = FindFileData.cFileName;
        while(*rvr_cFileName)
        {
            *(found_file++) = *(rvr_cFileName++);
        }
        *found_file = '\0';
        st_status = 1;  // ST_SUCCESS
    }


    // MD_CDraw_Restore();

#ifdef DEBUG
    dbg_prn("DEBUG: [%s, %d] END: DIR(match_string = %s, found_file = %s)\n", __FILE__, __LINE__, match_string, found_file);
#endif

    return st_status;

}
