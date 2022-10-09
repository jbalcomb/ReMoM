
#define __WIN32__ 1

#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
// #undef _UNICODE
// #undef _MBCS
#include <windows.h>
#include <memoryapi.h>
#include <stdio.h>

// #include "TST_SMLM.C"

#include "MoX_DBG.C"
#include "MoX_EXIT.C"
#include "MoX_LBX.C"
#include "MoX_MoM.C"
#include "MoX_SA.C"  /* SA_Allocate_MemBlk() */
#include "MoX_QUIT.C"
/* RAM_MIN_KB */
/* RAM_SetMinKB() */


/*
Win32PoC.cpp
C:\Program Files (x86)\Windows Kits\10\include\10.0.19041.0\um\winnt.h(173): fatal error C1189: #error:  "No Target Architecture"
#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
#include <windows.h>
or
#define _AMD64_ 1
or
-DCMAKE_GENERATOR_PLATFORM=x64
...
#if defined(_WIN64)
#if defined(_AMD64_)
...
#elif defined(_IA64_)
...
#else
#error "No Target Architecture"
#endif
*/

/*
https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
VirtualAlloc function (memoryapi.h)
LPVOID VirtualAlloc(
  [in, optional] LPVOID lpAddress,
  [in]           SIZE_T dwSize,
  [in]           DWORD  flAllocationType,
  [in]           DWORD  flProtect
);
Parameters
...
[in] dwSize
The size of the region, in bytes. If the lpAddress parameter is NULL, this value is rounded up to the next page boundary. Otherwise, the allocated pages include all pages containing one or more bytes in the range from lpAddress to lpAddress+dwSize. This means that a 2-byte range straddling a page boundary causes both pages to be included in the allocated region.
...
*/

#define SZ_DOS_CONVENTIONAL_MEMORY   1048576  //   1 MB
#define SZ_DOS_VGA_VIDEO_MEMORY       262144  // 256 KB
#define SZ_DOS_EXPANDED_MEMORY      33554432  //  32 MB
// sz segment, sz paragrph, sz page, sz block, ...

void DosMemEmu(void)
{
    unsigned char * SRAM;
    unsigned char * VRAM;
    unsigned char * EMM;

    SRAM = (unsigned char *)VirtualAlloc(NULL, 1048576, MEM_COMMIT | MEM_RESERVE, NULL);  // 1 MB ((1024 * 1024) * 1)
    VRAM = SRAM + 0xA000;  // i.e., mapped into SRAM at 0xA000:0000
    EMM = (unsigned char *)VirtualAlloc(NULL, 33554432, MEM_COMMIT | MEM_RESERVE, NULL);  // 32 MB ((1024 * 1024) * 32)
}

void main(void)
{
    SAMB_ptr SAMB_data;
    char LBX_Name[16] = "MAINSCRN";
    int LBX_Index = 0;
    char LBX_File_Name[20];
    unsigned int item_count;
    unsigned char baito;
    unsigned char header;
    unsigned int itr_header;
    unsigned char baito0;
    unsigned char baito1;
    unsigned char baito2;
    unsigned char baito3;
    unsigned int wordo0;
    unsigned int wordo1;
    unsigned int wordo2;
    unsigned int wordo3;
    unsigned int wordo0s;
    unsigned int wordo1s;
    unsigned int wordo2s;
    unsigned int wordo3s;
    unsigned int wordo0s1;
    unsigned int wordo2s3;
    unsigned int wordo01s;
    unsigned int wordo23s;

    // TST_SMLM();  // Test - Segmented Memory vs. Linear Memory


    if( LBX_Header_Allocated == ST_FALSE )
    {
        LBX_Header_Allocated = ST_TRUE;
        g_LBX_Header = SA_Allocate_MemBlk(SZ_LBX_HDR_PR);
        printf("g_LBX_Header: %p\n", g_LBX_Header);
    }

    printf("LBX_Name: %s\n", LBX_Name);
    strcpy(g_LBX_Name, LBX_Name);
    printf("g_LBX_Name: %s\n", g_LBX_Name);
    strcpy(LBX_File_Name, LBX_Name);
    printf("LBX_File_Name: %s\n", LBX_File_Name);
    strcat(LBX_File_Name, LBX_File_Extension);
    printf("LBX_File_Name: %s\n", LBX_File_Name);

    g_LBX_File_Pointer = fopen(LBX_File_Name, "rb");

    // size_t fread(void *buffer, size_t size, size_t count, FILE *stream);
    if ( g_LBX_File_Pointer == NULL )
    {
        printf("FAILURE: fopen()\n");
        abort();
    }
    else
    {
        printf("SUCCESS: fopen()\n");
    }    
    for ( itr_header = 0; itr_header < 512; itr_header++ )
    {
        fread(&header, 1, 1, g_LBX_File_Pointer);
        // printf("header: 0x%02X\n", header);
    }
    fseek(g_LBX_File_Pointer, 0, 0);

    item_count = fread(g_LBX_Header, 512, 1, g_LBX_File_Pointer);
    printf("item_count: %u\n", item_count);

    // if ( g_LBX_File_Pointer )
    // {
        printf("Closing file...\n");
        if ( fclose(g_LBX_File_Pointer) == 0 )
        {
            printf("SUCCESS: fclose()\n");
        }
    // }

    /*
        MAINSCRN.LBX
         0  1  2  3  4  5  6  7
            1     2     3     4
        06 00 AD FE 00 00 00 00
        0006  FEAD  0000  0000
    */
    baito0 = g_LBX_Header[0];  // 0x06
    baito1 = g_LBX_Header[1];  // 0x00
    baito2 = g_LBX_Header[2];  // 0xAD
    baito3 = g_LBX_Header[3];  // 0xFE

    wordo0 = g_LBX_Header[0];  // 0x0006
    wordo1 = g_LBX_Header[1];  // 0x0000
    wordo2 = g_LBX_Header[2];  // 0x00AD
    wordo3 = g_LBX_Header[3];  // 0x00FE

    wordo0s = g_LBX_Header[0] << 8;  // 0x0600
    wordo1s = g_LBX_Header[1] << 8;  // 0x0000
    wordo2s = g_LBX_Header[2] << 8;  // 0xAD00
    wordo3s = g_LBX_Header[3] << 8;  // 0xFE00

    wordo0s1 = g_LBX_Header[0] << 8 | g_LBX_Header[1];  // 0x0600
    wordo2s3 = g_LBX_Header[2] << 8 | g_LBX_Header[3];  // 0xADFE
    
    wordo01s = g_LBX_Header[0] | g_LBX_Header[1] << 8;  // 0x0006
    wordo23s = g_LBX_Header[2] | g_LBX_Header[3] << 8;  // 0xFEAD
    
    printf("baito0: 0x%02X\n", baito0);
    printf("baito1: 0x%02X\n", baito1);
    printf("baito2: 0x%02X\n", baito2);
    printf("baito3: 0x%02X\n", baito3);

    printf("wordo0: 0x%04X\n", wordo0);
    printf("wordo1: 0x%04X\n", wordo1);
    printf("wordo2: 0x%04X\n", wordo2);
    printf("wordo3: 0x%04X\n", wordo3);

    printf("wordo0s: 0x%04X\n", wordo0s);
    printf("wordo1s: 0x%04X\n", wordo1s);
    printf("wordo2s: 0x%04X\n", wordo2s);
    printf("wordo3s: 0x%04X\n", wordo3s);

    printf("wordo0s1: 0x%04X\n", wordo0s1);
    printf("wordo2s3: 0x%04X\n", wordo2s3);

    printf("wordo01s: 0x%04X\n", wordo01s);
    printf("wordo23s: 0x%04X\n", wordo23s);

    // if ( farpeekw(gsa_LBX_Header, 2) != LBX_MAGSIG )
    // {
    //     LBX_Error(tmp_LbxName, 0x07, tmp_LbxEntry, NULL);  /* LBXErr_bad_header */
    // }

    // ".LBX [entry "
    // "] "
    // " is not an LBX file"

    if ( wordo23s != 0xFEAD )
    {
        // "<LBX_Name>" ".LBX [entry " "<LBX_Index>" "] " " is not an LBX file"
        printf("<LBX_Name>.LBX [entry <LBX_Index>]  is not an LBX file\n");
    }
    else
    {
        printf("SUCCESS: ( wordo23s == 0xFEAD )\n");
    }

    if ( wordo23s != LBX_MAGSIG )
    {
        // "<LBX_Name>" ".LBX [entry " "<LBX_Index>" "] " " is not an LBX file"
        printf("<LBX_Name>.LBX [entry <LBX_Index>]  is not an LBX file\n");
    }
    else
    {
        printf("SUCCESS: ( wordo23s == LBX_MAGSIG )\n");
    }

    // extern uint8_t *bytes;
    // uint32_t myInt1 = bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);

    printf("SUCCESS: Win32PoC!!\n");

}
