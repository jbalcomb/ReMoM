/*
    WIZARDS.EXE
        seg024

MoO2
    Module: capture
    Module: fields

*/

#include "../../ext/stu_compat.h"

#include "DOS.h"
#include "MOX_DEF.h"
#include "MOX_TYPE.h"
#include "Mouse.h"
#include "Video.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "capture.h"

/* COPILOT */ extern uint8_t * current_palette;
/* COPILOT */ extern void Create_Picture(int16_t width, int16_t height, byte_ptr pict_seg);

static void VGA_GetDACBlock(int dac_block, uint8_t *buffer);
static void VGA_ReadScreenLine(int line, char *scanline_buffer);



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
    if(release_flag == ST_TRUE)
    {
        return;
    }
    // TODO  Exit_With_Size();
}

// WZD s24p02
// drake178: DBG_ScreenDump()
/*
MoO2
Module: capture
    function (0 bytes) Screen_Flic_Capture
    Address: 01:001101F0
        Num params: 0
        Return type: void (1 bytes)
        Locals:
            pointer (4 bytes) ronnie
            struct (28 bytes) inregs
            struct (28 bytes) outregs
            signed integer (4 bytes) mode
            char (1 bytes) color
            char (1 bytes) old_bank
            signed integer (4 bytes) start_x
            signed integer (4 bytes) start_y
            unsigned integer (4 bytes) dumb_number
            unsigned integer (4 bytes) offset
            signed integer (4 bytes) i
            signed integer (4 bytes) new_bank
            char (1 bytes) flag
            struct (128 bytes) header
                    char (1 bytes) id
                    char (1 bytes) version
                    char (1 bytes) format
                    char (1 bytes) bitspixelplane
                    signed integer (2 bytes) xmin
                    signed integer (2 bytes) ymin
                    signed integer (2 bytes) xmax
                    signed integer (2 bytes) ymax
                    signed integer (2 bytes) hdpi
                    signed integer (2 bytes) vdpi
                    array (48 bytes) egapalette
                    Num elements:   48, Type:                        unsigned integer (4 bytes)
                    char (1 bytes) reserved
                    char (1 bytes) numberofplanes
                    signed integer (2 bytes) charslineplane
                    signed integer (2 bytes) paletteinfo
                    signed integer (2 bytes) hscreensize
                    signed integer (2 bytes) vscreensize
                    array (54 bytes) filler
                    Num elements:   54, Type:                        unsigned integer (4 bytes)
            char (1 bytes) count
            array (768 bytes) palette
            Num elements:  768, Type:                unsigned integer (4 bytes)
            array (30 bytes) file_name
            Num elements:   30, Type:                unsigned integer (4 bytes)
            array (15 bytes) file_buffer2
            Num elements:   15, Type:                unsigned integer (4 bytes)
            signed integer (4 bytes) file_num

*/
/* GEMINI */
void Screen_Flic_Capture(void)
{
    // char buffer[16];    /* [bp-186h] */
    // char file_name[16];         /* [bp-176h] */
    // char scanline_buffer[320];  /* [bp-166h] */
    // int file_number;            /* [bp-08h] */
    // unsigned int word_field;    /* [bp-06h] */
    // unsigned long dword_field;  /* [bp-04h] */
    // FILE *fp;                   /* _SI_file_pointer */
    // int i;                      /* di */
    char buffer[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char file_name[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char scanline_buffer[SCREEN_WIDTH];
    char var_26[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t file_number = 0;
    uint16_t word_field = 0;
    uint32_t dword_field = 0;
    // int16_t file_handle;  // _SI_
    FILE * file_pointer = NULL;
    int16_t itr = 0;

    if (release_flag == ST_TRUE)
    {
        return;
    }

    Set_Page_On();
    Save_Mouse_State();

    file_number = 0;
    stu_strcpy(file_name, cnst_Scrdmp00_Full);

    /* Loop to find an unused filename SCRDMPxx.FLI */
    while (DIR(file_name, scanline_buffer) != 0)
    {
        file_number++;
        if (file_number < 10)
        {
            stu_strcpy(scanline_buffer, cnst_Scrdmp_0);
            stu_itoa(file_number, buffer, 10);
            stu_strcat(scanline_buffer, buffer);
        }
        else
        {
            stu_itoa(file_number, scanline_buffer, 10);
        }

        stu_strcpy(file_name, "SCRDMP"); /* cnst_Scrdmp_Base */
        stu_strcat(file_name, scanline_buffer);
        stu_strcat(file_name, ".FLI"); /* cnst_Scrdmp_Ext */
    }

    file_pointer = stu_fopen(file_name, "wb"); /* cnst_WB7 */

    /* --- FLI MAIN HEADER --- */
    dword_field = 64944L; /* Total file size: 128 (head) + 64816 (frame) */
    stu_fwrite(&dword_field, 4, 1, file_pointer);

    word_field = 0xAF11; /* FLI Magic */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    word_field = 1; /* Frame count */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    word_field = 320; /* e_SCREEN_WIDTH */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    word_field = 200; /* e_SCREEN_HEIGHT */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    word_field = 8; /* Bits per pixel */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    word_field = 0; /* Flags */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    word_field = 5; /* Speed */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    word_field = 0;
    for (itr = 0; itr < 55; itr++)
    {
        stu_fwrite(&word_field, 2, 1, file_pointer); /* Reserved/Padding */
    }

    /* --- FRAME CHUNK HEADER --- */
    dword_field = 0xFD20; /* Chunk size: 64800 */
    stu_fwrite(&dword_field, 4, 1, file_pointer);

    word_field = 0xF1FA; /* Prefix chunk type */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    word_field = 2; /* Number of sub-chunks */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    dword_field = 0;
    stu_fwrite(&dword_field, 4, 1, file_pointer); /* Reserved */
    stu_fwrite(&dword_field, 4, 1, file_pointer); /* Reserved */

    /* --- SUB-CHUNK 1: COLOR_64 (Palette) --- */
    dword_field = 778; /* Sub-chunk size: 6 (head) + 2 (skip/count) + 768 (data) */
    stu_fwrite(&dword_field, 4, 1, file_pointer);

    word_field = 11; /* Type 11: FLI_COLOR (6-bit DAC) */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    word_field = 1; /* Number of packets */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    word_field = 0; /* Number of colors to skip */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    /* Get all 256 colors in 4 blocks of 64 */
    for (itr = 0; itr < 4; itr++)
    {
        VGA_GetDACBlock(itr, scanline_buffer);
        stu_fwrite(scanline_buffer, 192, 1, file_pointer); /* 64 colors * 3 bytes (R,G,B) */
    }

    /* --- SUB-CHUNK 2: FLI_COPY (Pixel Data) --- */
    dword_field = 64006; /* Sub-chunk size: 6 (head) + 64000 (data) */
    stu_fwrite(&dword_field, 4, 1, file_pointer);

    word_field = 16; /* Type 16: FLI_COPY (Uncompressed pixels) */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    for (itr = 0; itr < 200; itr++)
    {
        VGA_ReadScreenLine(itr, scanline_buffer);
        stu_fwrite(scanline_buffer, 320, 1, file_pointer);
    }

    /* --- FINAL EMPTY PREFIX CHUNK (FLI Footer/Padding) --- */
    dword_field = 16; /* Chunk size */
    stu_fwrite(&dword_field, 4, 1, file_pointer);

    word_field = 0xF1FA; /* Prefix chunk type */
    stu_fwrite(&word_field, 2, 1, file_pointer);

    word_field = 0;
    stu_fwrite(&word_field, 2, 1, file_pointer); /* No sub-chunks */

    dword_field = 0;
    stu_fwrite(&dword_field, 4, 1, file_pointer);
    stu_fwrite(&dword_field, 4, 1, file_pointer);

    fclose(file_pointer);

    Restore_Mouse_State();
    Set_Page_Off();

}


// WZD s24p03
/* COPILOT */
static void VGA_GetDACBlock(int dac_block, uint8_t *buffer)
{
    int start_color;
    int offset;

    if(buffer == NULL)
    {
        return;
    }

    if(dac_block < 0)
    {
        dac_block = 0;
    }

    if(dac_block > 3)
    {
        dac_block = 3;
    }

    start_color = (dac_block << 6);
    offset = (start_color * 3);

    memcpy(buffer, current_palette + offset, (64 * 3));
}


// WZD s24p04
/* COPILOT */
static void VGA_ReadScreenLine(int line, char *scanline_buffer)
{
    int x;
    uint8_t *src;

    if(scanline_buffer == NULL)
    {
        return;
    }

    if((line < 0) || (line >= SCREEN_HEIGHT))
    {
        return;
    }

    src = current_video_page + (line * SCREEN_WIDTH);

    for(x = 0; x < SCREEN_WIDTH; x++)
    {
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
/* GEMINI */
void Screen_Picture_Capture(int16_t x1, int16_t y1, int16_t x2, int16_t y2, byte_ptr pict_seg)
{
    int16_t width;
    int16_t height;

    Set_Page_Off();

    width = x2 - x1 + 1;
    height = y2 - y1 + 1;

    if(width < 0)
    {
        width = -width;
    }

    if(height < 0)
    {
        height = -height;
    }

    Create_Picture(width, height, pict_seg);
    Capture_Screen_Block((pict_seg + SZ_PARAGRAPH_B), x1, y1, x2, y2);
}

// WZD s24p08
// MoO2  Module: shear  Capture_Screen_Block_()  Address: 01:0014791B
/* GEMINI */
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

    for(itr_height = 0; itr_height < height; ++itr_height)
    {
        for(itr_width = 0; itr_width < width; ++itr_width)
        {
            *buffer++ = *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width);
        }
    }
}
