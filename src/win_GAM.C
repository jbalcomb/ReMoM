
#include "MoX.H"

#include <windows.h>



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

// BOOL ReadFile(
//   [in]                HANDLE       hFile,
//   [out]               LPVOID       lpBuffer,
//   [in]                DWORD        nNumberOfBytesToRead,
//   [out, optional]     LPDWORD      lpNumberOfBytesRead,
//   [in, out, optional] LPOVERLAPPED lpOverlapped
// );

// BOOL ReadFileEx(
//   [in]            HANDLE                          hFile,
//   [out, optional] LPVOID                          lpBuffer,
//   [in]            DWORD                           nNumberOfBytesToRead,
//   [in, out]       LPOVERLAPPED                    lpOverlapped,
//   [in]            LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
// );


// WZD o50p02
void Load_SAVE_GAM(int16_t save_gam_idx)
{
    HANDLE file_handle;
    DWORD nBytesRead = 0;

    
    if(save_gam_idx == ST_UNDEFINED)
    {
        strcpy(file_name, "SAVETEST.GAM");
    }
    else
    {
#pragma warning(suppress : 4996)
        itoa(save_gam_idx, a_idx, 10);
        strcpy(file_name, "SAVE");
        strcat(file_name, a_idx);
        strcat(file_name,".GAM");
    }

    file_size = LOF(file_name);

    if(file_size != 57764)
    {
        file_size_flag = ST_TRUE;
    }

    file_handle = CreateFileA( file_name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file_handle == INVALID_HANDLE_VALUE)
    {
        DLOG("(file_handle == INVALID_HANDLE_VALUE)");
#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: FAILURE CreateFile(%s): %d\n", __FILE__, __LINE__, file_name, GetLastError());
        // ERROR_FILE_NOT_FOUND
        // 2 (0x2)
#endif
        return 0;  // ST_FAILURE
    }

    if (file_handle && file_handle != INVALID_HANDLE_VALUE)
    {
        DLOG("(file_handle && file_handle != INVALID_HANDLE_VALUE)");
        
    // Attempt a synchronous read operation.
    bResult = ReadFile(file_handle, &inBuffer, nBytesToRead, &nBytesRead, NULL);

    // Check for eof.
    if (bResult &&  nBytesRead == 0) 
    {
        // at the end of the file
    }

    if( FALSE == ReadFileEx(file_handle, p0_heroes, 12*35, &ol, FileIOCompletionRoutine) )
    {
        DisplayError(TEXT("ReadFile"));
        printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
        CloseHandle(hFile);
        return;
    }
    SleepEx(5000, TRUE);
    dwBytesRead = g_BytesTransferred;
    // This is the section of code that assumes the file is ANSI text. 
    // Modify this block for other data types if needed.

    if (dwBytesRead > 0 && dwBytesRead <= BUFFERSIZE-1)
    {
        ReadBuffer[dwBytesRead]='\0'; // NULL character

        _tprintf(TEXT("Data read from %s (%d bytes): \n"), argv[1], dwBytesRead);
        printf("%s\n", ReadBuffer);
    }
    else if (dwBytesRead == 0)
    {
        _tprintf(TEXT("No data read from file %s\n"), argv[1]);
    }
    else
    {
        printf("\n ** Unexpected value for dwBytesRead ** \n");
    }



        CloseHandle(hFile);

    }
}
