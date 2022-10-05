// seg024.C
// ST_DBG

#include "ST_DBG.H"

#include "ST_TYPE.H"
#include "ST_DEF.H"

#include "ST_VGA.H"  /* gsa_DSP_Addr */
// extern unsigned int gsa_DSP_Addr;                   // dseg:41C4

#include <STDIO.H>   /* FILE; fclose(), fopen(), fwrite(); */
#include <STRING.H>  /* strcat(), strcpy() */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

int Debug_Disabled = 1;                  // dseg:434E    XREF: DBG_Quit; DBG_ScreenDump; DBG_Disable; DBG_IsDisabled

/*
FLI - Original Animator Flic Files

This animation file format is limited to 320x200 resolution.
It is the main animation file format of the original Animator,
  and is still used by Animator Pro for creating 320x200 animations.
The file structure is very similar to that of a FLC file.
A FLI file does not contain a prefix chunk,
  and does not use FLI_PSTAMP or FLI_SS2 data encoding in the frame chunks.

The FLI file header

The file header for a FLI file is a subset of the FLC file header.
It is defined as follows:

Offset  Length  Name            Description

		 0      4     size      The size of the entire animation file, including this file header.
		 4      2     magic     File format identifier.  Always hex AF11.
		 6      2     frames    Number of frames in the flic.  This count does not include the ring frame.  FLI files have a maximum length of 4000 frames.
		 8      2     width     Screen width in pixels. This is always 320 in a FLI file.
		10      2     height    Screen height in pixels. This is always 200 in a FLI file.
		12      2     depth     Bits per pixel (always 8).
		14      2     flags     Always zero in a FLI file.
		16      2     speed     Number of jiffies to delay between each frame during playback.  A jiffy is 1/70 of a second.
		18    110     reserved  Unused space, set to zeroes.

The FLI frame chunks

One or more frame chunks immediately follow the FLI file header.
The frame chunks in a FLI file are identical to those in a FLC file,
  except that postage stamp image (FLI_PSTAMP)
  and word-runlength-compression (FLI_SS2) data chunks
  never appear in FLI files.

...

The FLC frame chunks

Frame chunks contain the pixel and color data for the animation.
A frame chunk may contain multiple subordinate chunks,
  each containing a different type of data for the current frame.
Each frame chunk starts with a 16-byte header that describes the contents of the frame:

Offset  Length  Name            Description

		 0      4     size      The size of the frame chunk, including this header and all subordinate chunks that follow.
		 4      2     type      Frame chunk identifier.  Always hex F1FA.
		 6      2     chunks    Number of subordinate chunks in the frame chunk.
		 8      8     reserved  Unused space, set to zeroes.


*/



// s24p02
void DBG_ScreenDump(void)
{
    char Conversion_String[16];
    char ScreenDump_File[16];
    char Buffer[350];
    word Scrdmp_File_Number;
    word Word_Field_Value;
    dword Dword_Field_Value;
    FILE * fp;
    int itr_Frame_Chunks;
    int itr_VGA_DAC_Blocks;
    int itr_ScreenLines;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: DBG_ScreenDump()\n", __FILE__, __LINE__);
#endif

    // if( Debug_Disabled == ST_TRUE )
    // {
    //     return;
    // }

    // VGA_SetDirectDraw();
    // MOUSE_Disable_CDraw();

    Scrdmp_File_Number = 0;
    strcpy(ScreenDump_File, "SCRDMP00.FLI");
    while( DISK_FileFind(ScreenDump_File, Buffer) != 0 )  // -1 success, 0 failure
    {
        Scrdmp_File_Number++;
        if( Scrdmp_File_Number < 10 )
        {
            strcpy(Buffer, "0");
            itoa(Scrdmp_File_Number, Conversion_String, 10);
            strcat(Buffer, Conversion_String);
        }
        else
        {
            itoa(Scrdmp_File_Number, Buffer, 10);
        }
        strcpy(ScreenDump_File, "SCRDMP");
        strcat(ScreenDump_File, Buffer);
        strcat(ScreenDump_File, ".FLI");
    }

    fp = fopen(ScreenDump_File, "wb");

    /*
        FLI Header
    */
    Dword_Field_Value = 64944; // size
    fwrite(&Dword_Field_Value, 4, 1, fp);
    Word_Field_Value = 0xAF11;  // magic
    fwrite(&Word_Field_Value, 2, 1, fp);
    Word_Field_Value = 1;  // frames
    fwrite(&Word_Field_Value, 2, 1, fp);
    Word_Field_Value = 320;  // width
    fwrite(&Word_Field_Value, 2, 1, fp);
    Word_Field_Value = 200;  // height
    fwrite(&Word_Field_Value, 2, 1, fp);
    Word_Field_Value = 8;  // depth
    fwrite(&Word_Field_Value, 2, 1, fp);
    Word_Field_Value = 0;  // flags
    fwrite(&Word_Field_Value, 2, 1, fp);
    Word_Field_Value = 5;  // speed
    fwrite(&Word_Field_Value, 2, 1, fp);
    Word_Field_Value = 0;  // reserved
    for(itr_Frame_Chunks = 0; itr_Frame_Chunks < 55; itr_Frame_Chunks++)
    {
        fwrite(&Word_Field_Value, 2, 1, fp);
    }
    /*
        FLI Chunk
    */
    Dword_Field_Value = 64800;  // size
    fwrite(&Dword_Field_Value, 4, 1, fp);
    Word_Field_Value = 0xF1FA;  // type
    fwrite(&Word_Field_Value, 2, 1, fp);
    Word_Field_Value = 2;  // chunks
    fwrite(&Word_Field_Value, 2, 1, fp);
    Dword_Field_Value = 0;  // reserved
    fwrite(&Dword_Field_Value, 4, 1, fp);
    fwrite(&Dword_Field_Value, 4, 1, fp);
    /*
        Data Chunk
    */
    Dword_Field_Value = 778;  // size
    fwrite(&Dword_Field_Value, 4, 1, fp);
    Word_Field_Value = 11;  // type    (Type Value 11    FLI_COLOR    64-level color palette info)
    fwrite(&Word_Field_Value, 2, 1, fp);
    Word_Field_Value = 1;  // (size-6) data      The color or pixel data.
    fwrite(&Word_Field_Value, 2, 1, fp);
    Word_Field_Value = 0;  // 
    fwrite(&Word_Field_Value, 2, 1, fp);
    for(itr_VGA_DAC_Blocks = 0; itr_VGA_DAC_Blocks < 4; itr_VGA_DAC_Blocks++)
    {
        VGA_GetDACBlock(itr_VGA_DAC_Blocks, Buffer);
        fwrite(&Buffer, 192, 1, fp);
    }
    /*

    */
    Dword_Field_Value = 64006;  // size
    fwrite(&Dword_Field_Value, 4, 1, fp);
    Word_Field_Value = 16;  // type    (Type Value 16    FLI_COPY    No compression)
    fwrite(&Word_Field_Value, 2, 1, fp);
    for(itr_ScreenLines = 0; itr_ScreenLines < 200; itr_ScreenLines++)
    {
        VGA_ReadScreenLine(itr_ScreenLines, &Buffer);
        fwrite(&Buffer, 320, 1, fp);
    }
    /*
        Frame Chunk - ? Loop Frame ?
    */
    Dword_Field_Value = 16;  // size
    fwrite(&Dword_Field_Value, 4, 1, fp);
    Word_Field_Value = 0xF1FA;  // type - Frame Chunk
    fwrite(&Word_Field_Value, 2, 1, fp);
    Word_Field_Value = 0;  // chunks
    fwrite(&Word_Field_Value, 2, 1, fp);
    Dword_Field_Value = 0;  // reserved
    fwrite(&Dword_Field_Value, 4, 1, fp);
    fwrite(&Dword_Field_Value, 4, 1, fp);

    fclose(fp);

    // MOUSE_CDraw_Restore();
    // VGA_SetDrawFrame();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: DBG_ScreenDump()\n", __FILE__, __LINE__);
#endif

}

// _s24p03c.c 
void VGA_GetDACBlock(unsigned int DAC_BLK, unsigned char * Buffer)
{
    unsigned int DAC_Reg;
    DAC_Reg = DAC_BLK * 64;
    _ES = FP_SEG(Buffer);   // ES:DX = pointer to buffer for color registers
    _DX = FP_OFF(Buffer);
    _BX = DAC_Reg;          // BX = first color register to read
    _CX = 64;               // CX = number of color registers to read
    _AL = 0x17;             // AL = 17  read block of DAC color registers
    _AH = 0x10;             // INT 10,10 - Set/Get Palette Registers (EGA/VGA)
    geninterrupt(0x10);     // INT 10 - Video BIOS Services
}

// s24p04
void VGA_ReadScreenLine(unsigned int ScreenLine, unsigned char * Buffer)
{
    unsigned int SrcSgmt;
    unsigned int SrcOfst;
    unsigned int DstSgmt;
    unsigned int DstOfst;
    int ScreenWidth_bytes;
    int itr_ScreenWidth_bytes;

    DstOfst = FP_OFF(Buffer);
    SrcOfst = ScreenLine * 80;
    DstSgmt = _DS;
    SrcSgmt = gsa_DSP_Addr;

    ScreenWidth_bytes = 80;
    while(ScreenWidth_bytes-- > 0)
    {
        outportb(0x3CE, 4);
        outportb(0x3CF, 0);
        *((unsigned char *)MK_FP(DstSgmt, DstOfst)) = *((unsigned char *)MK_FP(SrcSgmt, SrcOfst));  // ~== MOVSB
        SrcSgmt--;
        outportb(0x3CE, 4);
        outportb(0x3CF, 1);
        *((unsigned char *)MK_FP(DstSgmt, DstOfst)) = *((unsigned char *)MK_FP(SrcSgmt, SrcOfst));  // ~== MOVSB
        SrcSgmt--;
        outportb(0x3CE, 4);
        outportb(0x3CF, 2);
        *((unsigned char *)MK_FP(DstSgmt, DstOfst)) = *((unsigned char *)MK_FP(SrcSgmt, SrcOfst));  // ~== MOVSB
        SrcSgmt--;
        outportb(0x3CE, 4);
        outportb(0x3CF, 2);
        *((unsigned char *)MK_FP(DstSgmt, DstOfst)) = *((unsigned char *)MK_FP(SrcSgmt, SrcOfst));  // ~== MOVSB
        SrcSgmt--;
    }
}

// s24p05
void DBG_Disable(void)
{
    Debug_Disabled = 1;
}

// s24p06
int DBG_IsDisabled(void)
{
    return Debug_Disabled;
}
