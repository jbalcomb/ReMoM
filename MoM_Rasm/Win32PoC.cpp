
#define STU_DEBUG 1

#define __WIN32__ 1

#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
// #undef _UNICODE
// #undef _MBCS
#include <windows.h>
#include <memoryapi.h>
#include <stdio.h>

#include "MoX_MoM.H"

// #include "MoX_MAINMENU.C"

#include "win_DIR.C"

// #include "MoX_DBG.C"
// #include "MoX_EXIT.C"
// #include "MoX_FLIC.C"
// #include "MoX_Fonts.C"
// #include "MoX_LBX.C"
// #include "MoX_MoM.C"
// #include "MoX_Palette.C"
#include "MoX_SA.C"
// #include "MoX_QUIT.C"

#ifdef STU_DEBUG
#include "STU_DBG.C"
#include "STU_UTIL.C"   /* get_datetime() */
#endif


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

/*
On the IBM PC, all the address space up to 640 KiB was available for RAM.
This part of the address space is called "conventional memory" since it is accessible to all versions of DOS automatically on startup.
Segment 0, the first 64 KiB of conventional memory, is also called low memory area.
upper memory area (UMA) refers to the address space between 640 KiB and 1024 KiB (0xA0000–0xFFFFF)
high memory area (HMA) is the RAM area consisting of the first 64 KiB, minus 16 bytes, of the extended memory on an IBM PC/AT or compatible microcomputer.
*/
#define SZ_INTEL_8088_MEMORY         1048576  //   1 MB  (1024 * 1024) (2^20 bytes)
#define SZ_DOS_CONVENTIONAL_MEMORY    655360  // 640 KB  ( 640 * 1024)
#define SZ_DOS_UPPER_MEMORY           393216  // 384 KB  ( 384 * 1024)
#define SZ_DOS_VGA_VIDEO_MEMORY       262144  // 256 KB
#define SZ_DOS_EXPANDED_MEMORY      33554432  //  32 MB
// sz segment, sz paragraph, sz page, sz block, ...
#define SZ_FLIC_FRAME_BUFFER      320*200*1   // 320 * 200 = 64000 * 1 =  64,000
#define SZ_PALETTE_BUFFER             256*3   // 256 * RGB = 768 bytes
#define SZ_BITMAP_BUFFER          320*200*4   // 320 * 200 = 64000 * 4 = 256,000
// ? #define SZ_PALETTE_RGB_BUFFER         256*3   // 256 *  RGB =  768 bytes
// ? #define SZ_PALETTE_XRGB_BUFFER        256*4   // 256 * XRGB = 1024 bytes


unsigned char * g_FLIC_Frame_Buffer;
unsigned char * g_Palette_Buffer;
unsigned char * g_Bitmap_Buffer;


void DosMemEmu(void)
{
    unsigned char * SRAM;
    unsigned char * VRAM;
    unsigned char * EMM;

    SRAM = (unsigned char *) VirtualAlloc(NULL, SZ_INTEL_8088_MEMORY, MEM_COMMIT | MEM_RESERVE, NULL);  //  1 MB ((1024 * 1024) *  1)
    VRAM = SRAM + 0xA000;  // i.e., mapped into SRAM at 0xA000:0000
    EMM  = (unsigned char *) VirtualAlloc(NULL, SZ_DOS_EXPANDED_MEMORY,     MEM_COMMIT | MEM_RESERVE, NULL);  // 32 MB ((1024 * 1024) * 32)
}


void Allocate_FLIC_Frame_Buffer(void)
{
    g_FLIC_Frame_Buffer = (unsigned char *) VirtualAlloc(NULL, SZ_FLIC_FRAME_BUFFER, MEM_COMMIT | MEM_RESERVE, NULL);  //  1 MB ((1024 * 1024) *  1)
}

void Clear_FLIC_Frame_Buffer(void)
{
    memset((void *)g_FLIC_Frame_Buffer, 0, SZ_FLIC_FRAME_BUFFER);
}

void Allocate_Palette_Buffer(void)
{
    g_Palette_Buffer = (unsigned char *) VirtualAlloc(NULL, SZ_PALETTE_BUFFER, MEM_COMMIT | MEM_RESERVE, NULL);  //  1 MB ((1024 * 1024) *  1)
}

void Clear_Palette_Buffer(void)
{
    memset((void *)g_Palette_Buffer, 0, SZ_PALETTE_BUFFER);
}

void Allocate_Bitmap_Buffer(void)
{
    g_Bitmap_Buffer = (unsigned char *) VirtualAlloc(NULL, SZ_BITMAP_BUFFER, MEM_COMMIT | MEM_RESERVE, NULL);  //  1 MB ((1024 * 1024) *  1)
}

void Clear_Bitmap_Buffer(void)
{
    memset((void *)g_Bitmap_Buffer, 0, SZ_BITMAP_BUFFER);
}

void Copy_FLIC_Frame_To_Bitmap()
{
    int itr;

    for ( itr = 0; itr < SZ_FLIC_FRAME_BUFFER; itr++ )
    {
        *(g_Bitmap_Buffer + itr + 0) = *(g_Palette_Buffer + *(g_FLIC_Frame_Buffer + itr) + 0);
        *(g_Bitmap_Buffer + itr + 1) = *(g_Palette_Buffer + *(g_FLIC_Frame_Buffer + itr) + 1);
        *(g_Bitmap_Buffer + itr + 2) = *(g_Palette_Buffer + *(g_FLIC_Frame_Buffer + itr) + 3);
        *(g_Bitmap_Buffer + itr + 3) = 0xFF;
    }

}


void main(void)
{
    SAMB_ptr SAMB_head;
    int is_valid;
    SAMB_ptr SAMB_data;
    char file_name[16] = "MAINSCRN";
    int entry_num = 0;
    char font_file[] = GAME_FONT_FILE;
    char match_string[16] = "MAGIC.SET";
    char found_file[16] = {0};
    long file_size = -1;

    Debug_Log_Startup();

    // TST_SMLM();  // Test - Segmented Memory vs. Linear Memory

//    SAMB_data = MoX_Explore_LBX_Load_Entry(file_name, entry_num, 0, 0, 0);
//    dbg_prn("SAMB_data: %p\n", SAMB_data);
    
    // // void Allocation_Error(int error_num, int blocks)
    // Debug_Disabled = 0;
    // Allocation_Error(0, 0);
    // Debug_Disabled = 1;
    // Allocation_Error(1, 111);
    // Allocation_Error(2, 111);
    // Allocation_Error(3, 111);
    // Allocation_Error(4, 111);
    // exit(EXIT_FAILURE);

    // // void Error_Handler(char * file_name, int error_num, int entry_num, int pages)
    // Error_Handler(GAME_FONT_FILE, 1, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 2, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 3, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 4, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 5, 0, 111);
    // // DNE Error_Handler(GAME_FONT_FILE, 6, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 7, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 8, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 9, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 10, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 11, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 12, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 13, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 14, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 15, 0, 111);
    // Error_Handler(GAME_FONT_FILE, 16, 0, 111);

    SAMB_head = Allocate_Space(348);
    dbg_prn("SAMB_head: %p\n", SAMB_head);    
    is_valid = Check_Allocation(SAMB_head);
    dbg_prn("is_valid: %s\n", (is_valid == 0 ? "ST_FAILURE" : "ST_SUCCESS") );

//     // font_ptr = Farload(GAME_FONT_FILE, 0);
//     font_ptr = Farload(font_file, 0);
//     dbg_prn("font_ptr: %p\n", font_ptr);    
// 
//     // Load_Font_File(GAME_FONT_FILE);
//     dbg_prn("Load_Font_File(font_file);");
//     Load_Font_File(font_file);
// 
//     Load_Palette(0, -1, 0);
// 
//     // GAME_LoadMainImages();
//         // MoO2  _bitmap, _mainmenu_last_seg, _mainmenu_first_seg, _main_menu_background_seg, _mainmenu
//     mainmenu_top = Farload(mainscrn_lbx_file, 0);
//     mainmenu_bot = Farload(mainscrn_lbx_file, 5);
//     mainmenu_c   = Farload(vortex_lbx_file, 1);
//     mainmenu_h   = Farload(vortex_lbx_file, 2);
//     mainmenu_q   = Farload(vortex_lbx_file, 3);
//     mainmenu_n   = Farload(vortex_lbx_file, 4);
//     mainmenu_l   = Farload(vortex_lbx_file, 5);
// 
//     // void GAME_MainMenu(void)
//     //      |-> SCREEN_Menu()
// 
//     // int SCREEN_Menu(void)
//     Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View
// 
//     // SCREEN_Menu_Draw()
//     // MenuArea_X_Left = 123;
//     // MenuArea_Y_Top = 141;
//     // if ( MouseOver_ControlIndex == GUI_Quit_Lbl_Index )
//     //     FLIC_Reset_CurrentFrame(gsa_VORTEX_3_MenuQuitToDOS);
//     // else
//     //     FLIC_Set_CurrentFrame(gsa_VORTEX_3_MenuQuitToDOS, 1);
//     // FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 48), gsa_VORTEX_3_MenuQuitToDOS);
//     // FLIC_Draw_XY(123, (141 + 48), mainmenu_q);
//     // s28p11      FLIC_Draw_XY            FLIC_Draw_XY(int Left, int Top, unsigned int FlicHdr_SgmtAddr)         VGA_DrawLBXImage
//     // s28p11   void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header);
//     // MoO2  Reset_Animation_Frame() {} WORD picture[4] = 0 }
//     FLIC_Draw(123, (141 + 48), mainmenu_q);


    // Allocate_FLIC_Frame_Buffer();
    // Clear_FLIC_Frame_Buffer();
    // Allocate_Palette_Buffer();
    // Clear_Palette_Buffer();
    // Allocate_Bitmap_Buffer();
    // Clear_Bitmap_Buffer();


    // MoX_DIR.H  win_DIR.C
    // int DIR(const char * match_string, char * found_file);
    // long LOF(char * file_name);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] match_string: %s\n", __FILE__, __LINE__, match_string);
    dbg_prn("DEBUG: [%s, %d] found_file: %s\n", __FILE__, __LINE__, found_file);
#endif
    DIR(match_string, found_file);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] match_string: %s\n", __FILE__, __LINE__, match_string);
    dbg_prn("DEBUG: [%s, %d] found_file: %s\n", __FILE__, __LINE__, found_file);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] file_size: %ld\n", __FILE__, __LINE__, file_size);
#endif
    file_size = LOF(found_file);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] file_size: %ld\n", __FILE__, __LINE__, file_size);
#endif



    Debug_Log_Shutdown();
}
