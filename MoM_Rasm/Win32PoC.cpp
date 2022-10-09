
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

    // TST_SMLM();  // Test - Segmented Memory vs. Linear Memory

    // SAMB_ptr MoX_Explore_LBX_Load_Entry(char * LBX_Name, int LBX_Index, SAMB_ptr SAMB_head, int LoadType, int LbxHdrFmt)
    SAMB_data = MoX_Explore_LBX_Load_Entry(LBX_Name, LBX_Index, 0, 0, 0);
    printf("SAMB_data: %p\n", SAMB_data);
    
}
