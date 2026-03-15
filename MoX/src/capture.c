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

#include "capture.h"



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
            strcpy(scanline_buffer, "0"); /* cnst_Scrdmp_0 */
            itoa(file_number, conversion_str, 10);
            strcat(scanline_buffer, conversion_str);
        }
        else
        {
            itoa(file_number, scanline_buffer, 10);
        }

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
/* GEMINI + Copilot-GPT54 */
/**
 * @brief Copies one 64-entry block of VGA DAC palette data into a caller-supplied buffer.
 *
 * The VGA DAC holds 256 colour registers, each described by three 6-bit component
 * values (red, green, blue). This function divides the 256-entry palette into four
 * contiguous blocks of 64 entries and copies the block identified by @p dac_block
 * into @p buffer in packed R-G-B order:
 *
 *   block 0 → palette entries   0 –  63
 *   block 1 → palette entries  64 – 127
 *   block 2 → palette entries 128 – 191
 *   block 3 → palette entries 192 – 255
 *
 * The source of the data is the global `current_palette` array, which stores the
 * entire 768-byte (256 × 3) palette in the same packed R-G-B format.
 *
 * Out-of-range block indices are silently clamped to the valid range [0, 3].
 * If @p buffer is NULL the function returns immediately without accessing memory.
 *
 * @param dac_block  Zero-based block index in the range [0, 3]. Values below 0 are
 *                   treated as 0; values above 3 are treated as 3.
 * @param buffer     Destination byte array. Must point to a buffer of at least
 *                   192 bytes (64 colours × 3 bytes per colour). The written
 *                   layout is: R0, G0, B0, R1, G1, B1, …, R63, G63, B63.
 *                   Passing NULL is safe; the function is a no-op in that case.
 */
void VGA_GetDACBlock(short int dac_block, uint8_t *buffer)
{
    short int start_color;
    short int offset;

    if (buffer == NULL) {
        return;
    }

    if (dac_block < 0) {
        dac_block = 0;
    }
    if (dac_block > 3) {
        dac_block = 3;
    }

    start_color = dac_block << 6;
    offset = start_color * 3;

    memcpy(buffer, current_palette + offset, 64 * 3);
}


// WZD s24p04
/* GEMINI + Copilot-GPT54 */
/**
 * @brief Copies one full scanline of the active video page into a caller buffer.
 *
 * Reads the row identified by @p line from `current_video_page` and writes
 * `SCREEN_WIDTH` bytes into @p scanline_buffer. Each byte is a palette index
 * for a single pixel in 8-bit indexed mode.
 *
 * Safety checks performed by this function:
 * - Returns immediately if @p scanline_buffer is NULL.
 * - Returns immediately if @p line is outside [0, SCREEN_HEIGHT - 1].
 *
 * @param line            Zero-based scanline index to copy.
 * @param scanline_buffer Destination buffer that must be large enough to hold
 *                        `SCREEN_WIDTH` bytes.
 *
 * @return This function returns no value.
 */

void VGA_ReadScreenLine(short int line, char *scanline_buffer)
{
    short int x;
    uint8_t * src;

    if (scanline_buffer == NULL) {
        return;
    }

    if (line < 0 || line >= SCREEN_HEIGHT) {
        return;
    }

    src = current_video_page + (line * SCREEN_WIDTH);

    for (x = 0; x < SCREEN_WIDTH; ++x) {
        scanline_buffer[x] = (char)src[x];
    }
}


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
/* GEMINI + Copilot-GPT54 */
/**
 * @brief Captures a rectangular screen region into a picture buffer.
 *
 * Switches to the off-page context, computes rectangle dimensions from inclusive
 * corner coordinates, allocates/initializes the destination picture via
 * `Create_Picture()`, then copies pixel data from the current video page into the
 * picture payload via `Capture_Screen_Block()`.
 *
 * Width and height are normalized to non-negative values before calling
 * `Create_Picture()`. Pixel data is stored starting at `pict_seg + SZ_FLIC_HDR`,
 * leaving room for the picture header.
 *
 * @param x1       Left source coordinate (inclusive).
 * @param y1       Top source coordinate (inclusive).
 * @param x2       Right source coordinate (inclusive).
 * @param y2       Bottom source coordinate (inclusive).
 * @param pict_seg Destination picture buffer segment/pointer where the picture
 *                 header and payload are written.
 *
 * @return This function returns no value.
 */
void Screen_Picture_Capture(int16_t x1, int16_t y1, int16_t x2, int16_t y2, byte_ptr pict_seg)
{
    int16_t width;
    int16_t height;

    Set_Page_Off();

    width = x2 - x1 + 1;
    height = y2 - y1 + 1;

    if (width < 0)
    {
        width = -width;
    }

    if (height < 0)
    {
        height = -height;
    }

    Create_Picture(width, height, pict_seg);
    Capture_Screen_Block(pict_seg + SZ_FLIC_HDR, x1, y1, x2, y2);
}


// WZD s24p08
// MoO2  Module: shear  Capture_Screen_Block_()  Address: 01:0014791B
/* GEMINI + Copilot-GPT54 */
/**
 * @brief Copies a rectangular region from the active screen page into a linear frame buffer.
 *
 * Computes rectangle dimensions from inclusive corner coordinates and copies pixels
 * row-by-row from `current_video_page` into @p frame_data in packed linear order.
 * The first byte written corresponds to (x1, y1), followed by increasing x, then
 * subsequent rows in increasing y.
 *
 * This routine performs no clipping or bounds validation; callers are expected to
 * pass coordinates that stay within the valid screen area and a destination buffer
 * large enough to hold `((x2 - x1 + 1) * (y2 - y1 + 1))` bytes.
 *
 * @param frame_data Destination buffer for captured pixels.
 * @param x1         Left coordinate of the source rectangle (inclusive).
 * @param y1         Top coordinate of the source rectangle (inclusive).
 * @param x2         Right coordinate of the source rectangle (inclusive).
 * @param y2         Bottom coordinate of the source rectangle (inclusive).
 *
 * @return This function returns no value.
 */
void Capture_Screen_Block(byte_ptr frame_data, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    byte_ptr buffer;
    byte_ptr screen_page;
    int16_t width;
    int16_t height;
    int16_t itr_width;
    int16_t itr_height;

    width = x2 - x1 + 1;
    height = y2 - y1 + 1;

    buffer = frame_data;
    screen_page = current_video_page + (y1 * SCREEN_WIDTH) + x1;

    for (itr_height = 0; itr_height < height; ++itr_height) {
        for (itr_width = 0; itr_width < width; ++itr_width) {
            *buffer++ = *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width);
        }
    }
}
