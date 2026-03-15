/*
    WIZARDS.EXE
        seg024

MoO2
    Module: capture
    Module: fields

*/
#include "DOS.h"
#include "MOX_DEF.h"
#include "MOX_TYPE.h"
#include "Mouse.h"
#include "Video.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
void Screen_Flic_Capture__STUB(void)
{
    char Conversion_String[16];
    char file_name[16];
    // uint8_t scanline_buffer[SCREEN_WIDTH];
    char scanline_buffer[SCREEN_WIDTH];
    char var_26[30];
    int16_t Scrdmp_File_Number;
    uint16_t Word_Field_Value;
    uint32_t Dword_Field_Value;
    // int16_t file_handle;  // _SI_
    FILE * file_pointer;

    if(release_flag == ST_TRUE)
    {
        return;
    }

    Set_Page_On();
    
    Save_Mouse_State();

    Scrdmp_File_Number = 0;

    strcpy(file_name, cnst_Scrdmp00_Full);  // "SCRDMP00"


    while(DIR(file_name, scanline_buffer) != 0)
    {
        Scrdmp_File_Number++;
        if (Scrdmp_File_Number < 10)
        {
            strcpy(scanline_buffer, cnst_Scrdmp_0);  // "0"
            stu_itoa(Scrdmp_File_Number, Conversion_String, 10);
            strcat(scanline_buffer, Conversion_String);
        }
        else
        {
            stu_itoa(Scrdmp_File_Number, scanline_buffer, 10);

        }
        strcpy(file_name, cnst_Scrdmp_Base);  // "SCRDMP"
        strcat(file_name, scanline_buffer);
        strcat(file_name, cnst_Scrdmp_Ext);  // ".FLI"
    }

    file_pointer = fopen(file_name, cnst_WB7);

    

    // TODO
    // TODO
    // TODO
    // TODO
    // TODO
    


    fclose(file_pointer);
    
    
    Restore_Mouse_State();

    Set_Page_Off();
    
}

/* COPILOT */
short int Screen_Flic_Capture(void)
{
    if(release_flag == ST_TRUE)
    {
        return 0;
    }

    Screen_Flic_Capture__STUB();
    return 0;
}

// WZD s24p03
// drake178: VGA_GetDACBlock()
/* COPILOT */
void VGA_GetDACBlock(int dac_block, uint8_t *buffer)
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
// drake178: VGA_ReadScreenLine()
/* COPILOT */
void VGA_ReadScreenLine(int line, char *scanline_buffer)
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
// drake178: UU_VGA_SaveDrawSection()
// Screen_Picture_Capture()
// MoO2  Module: capture  Screen_Flic_Capture()  Address: 01:001101F0
/* COPILOT */
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
    Capture_Screen_Block(pict_seg + 16, x1, y1, x2, y2);
}

// WZD s24p08
// drake178: UU_VGA_CopyToLBX()
// Capture_Screen_Block()
// MoO2  Module: shear  Capture_Screen_Block_()  Address: 01:0014791B
/* COPILOT */
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
