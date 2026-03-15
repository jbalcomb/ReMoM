/*
MoM
    WIZARDS.EXE
        seg024
            void F11_Key__WIP(void);
            short int Screen_Flic_Capture(void);
            void VGA_GetDACBlock(short int dac_block, uint8_t *buffer);
            void VGA_ReadScreenLine(short int line, char *scanline_buffer);
            void Release_Version(void);
            int Check_Release_Version(void);
            void Screen_Picture_Capture(int16_t x1, int16_t y1, int16_t x2, int16_t y2, byte_ptr pict_seg);
            void Capture_Screen_Block(byte_ptr frame_data, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

MoO2
    ORION2.EXE
        Module: capture
            function (0 bytes) Screen_Flic_Capture
                Address: 01:001101F0
            function (0 bytes) B_Key
                Address: 01:00110AFC
            function (0 bytes) Release_Version
                Address: 01:00110B34
            function (0 bytes) Check_Release_Version
                Address: 01:00110B5C
        Module: fields
            function (0 bytes) F11_Key
                Address: 01:001172B0

*/

/**
 * @file capture.c
 * @brief Screen capture and picture extraction routines for seg024.
 *
 * This module contains capture-related functionality used to:
 * - generate single-frame FLI screen dumps (`Screen_Flic_Capture`),
 * - read palette blocks and scanlines (`VGA_GetDACBlock`, `VGA_ReadScreenLine`),
 * - copy rectangular screen regions into picture/frame buffers
 *   (`Screen_Picture_Capture`, `Capture_Screen_Block`).
 *
 * It also exposes release-mode capture gating through `release_flag` and helper
 * accessors (`Release_Version`, `Check_Release_Version`).
 */

#include "DOS.h"
#include "FLIC_Draw.h"
#include "MOX_DEF.h"
#include "MOX_TYPE.h"
#include "Mouse.h"
#include "Video.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "capture.h"
#include "MOX_BASE.h"
#include "Video.h"
//void VGA_ReadScreenLine(int line, char *scanline_buffer)
#include <string.h>
#include <stdint.h>
extern uint8_t * current_palette;
//void VGA_GetDACBlock(int dac_block, uint8_t *buffer)
    /* Global variables referenced */
    extern int release_flag; /* byte_XXXXX */
    /* External Function Prototypes (Inferred from RTL/Context) */
    extern void Set_Page_On(void);
    extern void Set_Page_Off(void);
    extern void Save_Mouse_State(void);
    extern void Restore_Mouse_State(void);
    extern int /* far */ DIR(char *match_string, char *found_file);
    extern void /* far */ VGA_GetDACBlock(int block, char *buffer);   /* near call with CS push */
    extern void /* far */ VGA_ReadScreenLine(int line, char *buffer); /* near call with CS push */
//int Screen_Flic_Capture(void)
/* Global variables */
extern unsigned int current_video_page; /* word_41E34 - segment of current VGA buffer */
// int Capture_Screen_Block(unsigned int frame_data_seg, int x1, int y1, int x2, int y2)
/* External functions from WIZARDS.EXE or MAGIC.EXE */
extern void Set_Page_Off(void);
extern void Create_Picture(int width, int height, unsigned int pict_seg);
extern void /* far */ Capture_Screen_Block(unsigned int /* far */ frame_data_seg, int x1, int y1, int x2, int y2);
// int Screen_Picture_Capture(int x1, int y1, int x2, int y2, unsigned int pict_seg)
#include "../../ext/stu_compat.h"

#include "capture.h"

/* COPILOT */ extern uint8_t * current_palette;
/* COPILOT */ extern void Create_Picture(int16_t width, int16_t height, byte_ptr pict_seg);



// WZD dseg:7876                                                 BEGIN : seg024 - Initialized Data

// WZD dseg:7876
int16_t release_flag = ST_FALSE;

// WZD dseg:7878
char cnst_Scrdmp00_Full[] = "SCRDMP00";

// WZD dseg:7880
char cnst_Scrdmp_Ext[] = ".FLI";

// WZD dseg:7885
char cnst_Scrdmp_0[] = "";

// WZD dseg:7887
char cnst_Scrdmp_Base[] = "SCRDMP";

// WZD dseg:788E
char cnst_WB7[] = "wb";

// WZD dseg:7891 00                                              align 2

// WZD dseg:7891                                                 END : seg024 - Initialized Data



/*
    MAGIC.EXE    seg024
    WIZARDS.EXE  seg024
*/

// WZD s24p01
void F11_Key__WIP(void)
{
    if(release_flag != ST_TRUE)
    {
        Exit_With_Size();
    }
}

// WZD s24p02
/* GEMINI */
/**
 * @brief Captures the current screen and palette into a single-frame Autodesk FLI file.
 *
 * Creates a file named `SCRDMPxx.FLI` (first unused index), then writes a complete
 * FLI stream containing:
 * - Main FLI header
 * - One frame chunk with two sub-chunks:
 *   - Color chunk (type 11) populated from `VGA_GetDACBlock()`
 *   - Copy chunk (type 16) populated from `VGA_ReadScreenLine()`
 * - A trailing empty prefix chunk
 *
 * The function temporarily enables the capture page context and saves/restores mouse
 * state around the operation via `Set_Page_On()`, `Save_Mouse_State()`,
 * `Restore_Mouse_State()`, and `Set_Page_Off()`.
 *
 * If `release_flag` is set, capture is skipped and the function returns immediately.
 *
 * @return Always returns 0.
 */
short int Screen_Flic_Capture(void)
{
    char conversion_str[16];
    char file_name[16];
    char scanline_buffer[SCREEN_WIDTH];
    short int file_number;
    unsigned short int word_field;
    unsigned long dword_field;
    FILE *fp;
    short int i;

    if (release_flag == 1 /* e_ST_TRUE */)
    {
        return 0;
    }

    Set_Page_On();
    Save_Mouse_State();

    file_number = 0;
    strcpy(file_name, "SCRDMP00"); /* cnst_Scrdmp00_Full */

    /* Loop to find an unused filename SCRDMPxx.FLI */
    while (DIR(file_name, scanline_buffer) != 0)
    {
        file_number++;
        if (file_number < 10)
        {
            strcpy(scanline_buffer, cnst_Scrdmp_0);  // "0"
            SDL_itoa(Scrdmp_File_Number, Conversion_String, 10);
            strcat(scanline_buffer, Conversion_String);
        }
        else
        {
            SDL_itoa(Scrdmp_File_Number, scanline_buffer, 10);

        strcpy(file_name, "SCRDMP"); /* cnst_Scrdmp_Base */
        strcat(file_name, scanline_buffer);
        strcat(file_name, ".FLI"); /* cnst_Scrdmp_Ext */
    }

    fp = fopen(file_name, "wb"); /* cnst_WB7 */

    /* --- FLI MAIN HEADER --- */
    dword_field = 64944L; /* Total file size: 128 (head) + 64816 (frame) */
    fwrite(&dword_field, 4, 1, fp);

    word_field = 0xAF11; /* FLI Magic */
    fwrite(&word_field, 2, 1, fp);

    word_field = 1; /* Frame count */
    fwrite(&word_field, 2, 1, fp);

    word_field = SCREEN_WIDTH;
    fwrite(&word_field, 2, 1, fp);

    word_field = SCREEN_HEIGHT;
    fwrite(&word_field, 2, 1, fp);

    word_field = 8; /* Bits per pixel */
    fwrite(&word_field, 2, 1, fp);

    word_field = 0; /* Flags */
    fwrite(&word_field, 2, 1, fp);

    word_field = 5; /* Speed */
    fwrite(&word_field, 2, 1, fp);

    word_field = 0;
    for (i = 0; i < 55; i++)
    {
        fwrite(&word_field, 2, 1, fp); /* Reserved/Padding */
    }

    /* --- FRAME CHUNK HEADER --- */
    dword_field = 0xFD20; /* Chunk size: 64800 */
    fwrite(&dword_field, 4, 1, fp);

    word_field = 0xF1FA; /* Prefix chunk type */
    fwrite(&word_field, 2, 1, fp);

    word_field = 2; /* Number of sub-chunks */
    fwrite(&word_field, 2, 1, fp);

    dword_field = 0;
    fwrite(&dword_field, 4, 1, fp); /* Reserved */
    fwrite(&dword_field, 4, 1, fp); /* Reserved */

    /* --- SUB-CHUNK 1: COLOR_64 (Palette) --- */
    dword_field = 778; /* Sub-chunk size: 6 (head) + 2 (skip/count) + 768 (data) */
    fwrite(&dword_field, 4, 1, fp);

    word_field = 11; /* Type 11: FLI_COLOR (6-bit DAC) */
    fwrite(&word_field, 2, 1, fp);

    word_field = 1; /* Number of packets */
    fwrite(&word_field, 2, 1, fp);

    word_field = 0; /* Number of colors to skip */
    fwrite(&word_field, 2, 1, fp);

    /* Get all 256 colors in 4 blocks of 64 */
    for (i = 0; i < 4; i++)
    {
        VGA_GetDACBlock(i, scanline_buffer);
        fwrite(scanline_buffer, 192, 1, fp); /* 64 colors * 3 bytes (R,G,B) */
    }

    /* --- SUB-CHUNK 2: FLI_COPY (Pixel Data) --- */
    dword_field = 64006; /* Sub-chunk size: 6 (head) + 64000 (data) */
    fwrite(&dword_field, 4, 1, fp);

    word_field = 16; /* Type 16: FLI_COPY (Uncompressed pixels) */
    fwrite(&word_field, 2, 1, fp);

    for (i = 0; i < SCREEN_HEIGHT; i++)
    {
        VGA_ReadScreenLine(i, scanline_buffer);
        fwrite(scanline_buffer, SCREEN_WIDTH, 1, fp);
    }

    /* --- FINAL EMPTY PREFIX CHUNK (FLI Footer/Padding) --- */
    dword_field = 16; /* Chunk size */
    fwrite(&dword_field, 4, 1, fp);

    word_field = 0xF1FA; /* Prefix chunk type */
    fwrite(&word_field, 2, 1, fp);

    word_field = 0;
    fwrite(&word_field, 2, 1, fp); /* No sub-chunks */

    dword_field = 0;
    fwrite(&dword_field, 4, 1, fp);
    fwrite(&dword_field, 4, 1, fp);

    fclose(fp);

    Restore_Mouse_State();
    Set_Page_Off();

    return 0;
}

// WZD s24p03
// drake178: VGA_GetDACBlock()

// WZD s24p04
// drake178: VGA_ReadScreenLine()

// WZD s24p05
// MoO2  Module: capture  Release_Version()  Address: 01:00110B34
void Release_Version(void)
{
    release_flag = ST_TRUE;
}

// WZD s24p06
// MoO2  Module: capture  Check_Release_Version()  Address: 01:00110B5C
int Check_Release_Version(void)
{
    return release_flag;
}

// WZD s24p07
// MoO2  Module: capture  Screen_Flic_Capture()  Address: 01:001101F0

// WZD s24p08
// MoO2  Module: shear  Capture_Screen_Block_()  Address: 01:0014791B
