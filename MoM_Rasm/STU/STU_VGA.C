// STU_VGA.C
// STU_VGA.H
// SEE: VGA_DAC.md
// http://digiassn.blogspot.com/2006/04/cdos-video-programming-with-turbo-c.html

#include "STU_VGA.H"

#include "Mox_TYPE.H"
#include "ST_DEF.H"

#include "ST_VGA.H"

// TODO(JimBalcomb,20221111): remvoe all inclusions of DOS.H  (it is included ST_DEF.H)
#include <DOS.H>
#include <STDIO.H>      /* FILE; fclose(), fopen(), fread(), fwrite(), printf(); */


#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifndef WORD
typedef unsigned short WORD;
#endif
#ifndef _FAR
#ifdef __VSCODE__
#define _FAR
#else
#define _FAR far
#endif
#endif

unsigned char STU_VGA_WriteMapMasks[4] = {0x01, 0x02, 0x04, 0x08}; // dseg:4372


#define BMP_MAGIC 0x4D42                    /* "BM" (0x42, 0x4D; "B", "M") */

struct s_BMP_BITMAP_FILE_HEADER
{
    int    bfType;             /* Specifies the type of file. It must be BM. */
    long   bfSize;             /* Specifies the size in DWORDs of the file.  */
    int    bfReserved1;        /* Is reserved and must be set to zero. */
    int    bfReserved2;        /* Is reserved and must be set to zero. */
    long   bfOffBits;          /* Specifies, in bytes, the offset from the beginning of the file (BOF) to the image data (bitmap) */
};

struct s_BMP_BITMAP_INFO_HEADER{
    long   biSize;              /* Specifies the number of bytes required by the BITMAPINFOHEADER structure. */
    long   biWidth;             /* Specifies the width of the bitmap in pixels. */
    long   biHeight;            /* Specifies the height of the bitmap in pixels. */
    int    biPlanes;            /* Specifies the number of planes for the target device and must be set to 1. */
    int    biBitCount;          /* Specifies the number of bits per pixel. This value must be 1, 4, 8, or 24. */
    long   biCompression;       /* Specifies the type of compression for a compressed bitmap. */
    long   biSizeImage;         /* Specifies the size in bytes of the image. */
    long   biXPelsPerMeter;     /* */
    long   biYPelsPerMeter;     /* */
    long   biClrUsed;           /* */
    long   biClrImportant;      /* */
};

// #define VBB_BMP_FILE_SIZE  (14 + 40 + (320 * 200))
// #define VBB_BMP_IMAGE_SIZE (320 * 200)
// // ? #pragma pack(push,1)
// struct s_BMP_BITMAP_FILE_HEADER VBB_BMPINFOHEADER = 
// {
//     BMP_MAGIC,                      /* Specifies the type of file. It must be BM. */
//     64054L,                         /* Specifies the size in DWORDs of the file.  */
//     0,                              /* Is reserved and must be set to zero. */
//     0,                              /* Is reserved and must be set to zero. */
//     54                              /* Specifies, in bytes, the offset from the beginning of the file (BOF) to the image data (bitmap) */
// };
// // ? #pragma pack(pop)
// // bmp->bmp_bitmap_file_header->bfOffBits = 14 + 40 + 1024;
// 
// // ? #pragma pack(push,1)
// struct s_BMP_BITMAP_INFO_HEADER VBB_BMPINFOHEADERFILE =
// {
//     BMP_BITMAP_INFO_HEADER_LENGTH,  /* Specifies the number of bytes required by the BITMAPINFOHEADER structure. */
//     320,                            /* Specifies the width of the bitmap in pixels. */
//     -200,                           /* Specifies the height of the bitmap in pixels. */
//     1,                              /* Specifies the number of planes for the target device and must be set to 1. */
//     8,                              /* Specifies the number of bits per pixel. This value must be 1, 4, 8, or 24. */
//     0,                              /* Specifies the type of compression for a compressed bitmap. */
//     VBB_BMP_IMAGE_SIZE,             /* Specifies the size in bytes of the image. */
//     BMP_XPELS_PER_METER_DEFAULT,    /* */
//     BMP_YPELS_PER_METER_DEFAULT,    /* */
//     256,                            /* */
//     256                             /* */
// };
// // ? #pragma pack(pop)
// // bmp->bmp_bitmap_info_header->biSizeImage = element_count; /* (4294967296 - 1) - (14 + 40) - (256 * 4);  / * 2^32 - Headers - Palette * / */


/*

maybe, maybe not VGA Mode 'Mode-Y' specific

MoM
_s21p01c.c  VGA_DAC_Write()

*/
/*

    VGA Ports:

    PALETTE_INDEX   0x3C8
    PALETTE_DATA    0x3C9

    INPUT_STATUS
    VRETRACE
*/
/*
    Vertical Retrace / Vertical Blank / Display Enable

*/
/*
    VGA DAC
    256 colors
    RGB
    3 bytes per color

*/
/*

PEL Address Read Mode Register (port 3C7h)
PEL Address Write Mode Register (port 3C8h)
PEL Data Register (port 3C9h)

the PEL Address Read Mode Register (port 3C7h) is used to specify the palette entry to be read
the values are read from the PEL Data Register
the PEL Address Read Mode Register auto-increments after each triplet is read
...
Output the value of the first color entry to be read to the PEL Address Read Mode Register.
Read the PEL Data Register to obtain the red component value.
Read the PEL Data Register to obtain the green component value.
Read the PEL Data Register to obtain the blue component value.
If more colors are to be read, repeat steps 4-6.
...
*/

/*
    VGA Screen Save
    320 * 200 = 64000 bytes
    320 bytes at a time
    4 planes, 80 bytes

    Read Mode
    Plane


    VGA Graphics Controller I/O port, 0x03CE
        GC_INDEX  0x03CE
        GC_DATA   0x03CF
    Graphics Controller Register
        Index 05h -- Graphics Mode Register
            Read Mode
            Write Mode


    The Mode Register (graphics controller register number 5) must be set in read mode 0 to read bytes from a given individual plane.
    Then, the Read Map Select Register (register 4) must be set up with the number of the bit plane to be read.

    Once the bit plane has been moved, the mode register is reset to its normal default of read mode 1, and the read map select register to its default.



    The Graphics Registers are accessed via a pair of registers,
        the Graphics Address Register and the Graphics Data Register.
    The Address Register is located at port 3CEh and the Data Register is located at port 3CFh.
    Index 00h -- Set/Reset Register
    Index 01h -- Enable Set/Reset Register
    Index 02h -- Color Compare Register
    Index 03h -- Data Rotate Register
    Index 04h -- Read Map Select Register
    Index 05h -- Graphics Mode Register
    Index 06h -- Miscellaneous Graphics Register
    Index 07h -- Color Don't Care Register
    Index 08h -- Bit Mask Register

    Read Map Select Register (Index 04h)
        Read Map Select
            This value of this field is used in Read Mode 0 to specify the display memory plane to transfer data from.
            Due to the arrangement of video memory, this field must be modified four times to read one or more pixels values in the planar video modes.

    Graphics Mode Register (Index 05h)
        Read Mode
            This field selects between two read modes, simply known as Read Mode 0, and Read Mode 1, based upon the value of this field:
            0b -- Read Mode 0: In this mode, a byte from one of the four planes is returned on read operations.
                The plane from which the data is returned is determined by the value of the Read Map Select field.
        Write Mode
            This field selects between four write modes, simply known as Write Modes 0-3, based upon the value of this field:



    How Read_VGA_Plane Works

    The Read_VGA_Plane procedure is by far the simpler of the matched pair of MASM video plane handling routines.
    It accepts a plane number and byte count from the calling program and fills an externally defined array with the bits from the adapter plane.
    The plane number, in the range O to 3, corresponds to the number of the bit plane on the VGA adapter.
    The byte count is simply the total number of bytes in the adapter plane.
    Since register programming is the same for VGA and EGA graphics modes, varying the byte count allows Read_VGA_Plane to work with any EGA or VGA color mode.
    
    The VGA Graphics Controller I/O port, 03CEh, provides access to a set of registers that determine how the VGA card operates.
    It is necessary to program only two graphics controller registers to read from a given video plane.
    The Mode Register (graphics controller register number 5) must be set in read mode 0 to read bytes from a given individual plane.
    Then, the Read Map Select Register (register 4) must be set up with the number of the bit plane to be read.
    A single out instruction will do in either case, feeding a pair of bytes that select the register number and pass the value to be loaded into the register.
    Once the byte count (CX register) and the array address (ES:DI) have been set up, a repeated byte move (REP MOVSB) reads each byte from the bit plane.
    If you encounter the VGA bus timing problems described earlier, use a slower byte-at-a-time loop instead.
    Once the bit plane has been moved, the mode register is reset to its normal default of read mode 1, and the read map select register to its default.

*/

/*
      VRAM Base:   0xA000
    Screen-Page: + {0x0000,0x0400}
      Scan-Line: + Y * (320/4)

*/




                    /* ╔══════════════════════════════════════════════════════════════════╗
                    ╔══╝  256-Color VGA Programming in C - David Brackeen                 ║
                    ╠═════════════════════════════════════════════════════════════════════╣
                    ║                                                                     ║
                    ║                                                                     ║
                    ║                                                                     ║
                    ║                                                                     ║
                    ║                                                                  ╔══╝
                    ╚══════════════════════════════════════════════════════════════════╝ */

/*
http://www.brackeen.com/vga/source/bc31/unchain.c.html
*/

// ...
#define INPUT_STATUS_1      0x03DA
// ...

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define SCREEN_SIZE         (word)(SCREEN_WIDTH*SCREEN_HEIGHT)
#define NUM_COLORS          256       /* number of colors in mode 0x13 */

#define VERTICAL_RETRACE              /* comment out this line for more accurate timing */


/**************************************************************************
 *  set_mode                                                              *
 *     Sets the video mode.                                               *
 **************************************************************************/

void set_mode(byte mode)
{
  union REGS regs;

  // regs.h.ah = SET_MODE;
  regs.h.ah = 0x00;
  regs.h.al = mode;
  // int86(VIDEO_INT, &regs, &regs);
  int86(0x10, &regs, &regs);
}

/**************************************************************************
 *  set_unchained_mode                                                    *
 *    resets VGA mode 0x13 to unchained mode to access all 256K of memory *
 **************************************************************************/

void set_unchained_mode(void)
{
  word i;
  dword *ptr=(dword *)VGA;            /* used for faster screen clearing */

  outportb(SC_INDEX,  MEMORY_MODE);       /* turn off chain-4 mode */
  outportb(SC_DATA,   0x06);

  outport(SC_INDEX, ALL_PLANES);        /* set map mask to all 4 planes */

  for(i=0;i<0x4000;i++)               /* clear all 256K of memory */
    *ptr++ = 0;

  outportb(CRTC_INDEX,UNDERLINE_LOCATION);/* turn off long mode */
  outportb(CRTC_DATA, 0x00);

  outportb(CRTC_INDEX,MODE_CONTROL);      /* turn on byte mode */
  outportb(CRTC_DATA, 0xe3);
}

/**************************************************************************
 *  page_flip                                                             *
 *    switches the pages at the appropriate time and waits for the        *
 *    vertical retrace.                                                   *
 **************************************************************************/

void page_flip(word *page1,word *page2)
{
  word high_address,low_address;
  word temp;

  temp=*page1;
  *page1=*page2;
  *page2=temp;

  high_address = HIGH_ADDRESS | (*page1 & 0xff00);
  low_address  = LOW_ADDRESS  | (*page1 << 8);

  #ifdef VERTICAL_RETRACE
    while ((inp(INPUT_STATUS_1) & DISPLAY_ENABLE));
  #endif
  outport(CRTC_INDEX, high_address);
  outport(CRTC_INDEX, low_address);
  #ifdef VERTICAL_RETRACE
    while (!(inp(INPUT_STATUS_1) & VRETRACE));
  #endif
}

/**************************************************************************
 *  show_buffer                                                           *
 *    displays a memory buffer on the screen                              *
 **************************************************************************/

void show_buffer(byte *buffer)
{
  #ifdef VERTICAL_RETRACE
    while ((inp(INPUT_STATUS_1) & VRETRACE));
    while (!(inp(INPUT_STATUS_1) & VRETRACE));
  #endif
  memcpy(VGA,buffer,SCREEN_SIZE);
}

/**************************************************************************
 *  set_palette                                                           *
 *    Sets all 256 colors of the palette.                                 *
 **************************************************************************/

void set_palette(byte *palette)
{
  int i;

  outportb(PALETTE_INDEX,0);              /* tell the VGA that palette data
                                         is coming. */
  for(i=0;i<256*3;i++)
    outportb(PALETTE_DATA,palette[i]);    /* write the data */
}

/**************************************************************************
 *  plot_pixel                                                            *
 *    Plots a pixel in unchained mode                                     *
 **************************************************************************/

void plot_pixel(int x,int y,byte color)
{
  outportb(SC_INDEX, MAP_MASK);          /* select plane */
  outportb(SC_DATA,  1 << (x&3) );

  VGA[(y<<6)+(y<<4)+(x>>2)]=color;
}

// set_mode(VGA_256_COLOR_MODE);       /* set the video mode. */
// set_palette(bmp.palette);
// ...
// set_unchained_mode();               /* set unchained mode */
// ...
// set_mode(TEXT_MODE);                /* set the video mode back to text mode. */

// start = *my_clock;                  /* record the starting time. */
// t2 = (*my_clock - start) / 18.2;    /* calculate how long it took. */



void Fill_VRAM(byte palette_index)
{
    int itr_x;
    int itr_y;
    byte_ptr screen_pos;
    byte mask;
    byte_ptr bbuff;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Fill_VRAM(palette_index = 0x%02X)\n", __FILE__, __LINE__, palette_index);
#endif


    outportb(e_SC_INDEX, e_SC_MAPMASK);
    mask = STU_VGA_WriteMapMasks[0];  // 0b00000011  ~== x modulo 4  (x % 4, x|4)

    screen_pos = MK_PTR_SGMT(p_DSP, (0 * (((320/4)/16))), (0 / 4));
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] screen_pos: %p\n", __FILE__, __LINE__, screen_pos);
#endif

    itr_x = 320;
    itr_y = 200;

    while(itr_x--)
    {
        bbuff = screen_pos;
        outportb(e_SC_DATA, mask);
        mask = mask * 2;  // {1,2,4,8} * 2 = {2,4,8,16}
        if(mask >= 9)  /* 0b00010000 */
        {
            mask = 1;  /* 0b00000001 */
            screen_pos++;
        }

        while(itr_y--)
        {
            *bbuff = palette_index;
            bbuff += WIDTH_STRIDE;
        }

    }  /* while (width--) */


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Fill_VRAM(palette_index = 0x%02X)\n", __FILE__, __LINE__, palette_index);
#endif

}

void Fill_Video_Back_Buffer(byte palette_index, byte_ptr video_back_buffer)
{
    int itr_x;
    int itr_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Fill_Video_Back_Buffer(palette_index = 0x%02X, video_back_buffer = %p)\n", __FILE__, __LINE__, palette_index, video_back_buffer);
#endif

    for(itr_y = 0; itr_y < 200; itr_y++)
    {
        for(itr_x = 0; itr_x < 320; itr_x++)
        {
            *(video_back_buffer + (itr_y * 320 + itr_x)) = palette_index;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Fill_Video_Back_Buffer(palette_index = 0x%02X, video_back_buffer = %p)\n", __FILE__, __LINE__, palette_index, video_back_buffer);
#endif

}

void Draw_Video_Back_Buffer(int x_start, int y_start, int width, int height, byte_ptr video_back_buffer)
{
    byte_ptr vbuf_pos;
    byte_ptr vbuf;
    byte_ptr vram_pos;
    byte_ptr vram;
    byte mask;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Draw_Video_Back_Buffer(x_start = %d, y_start = %d, width = %d, height = %d, video_back_buffer = %p)\n", __FILE__, __LINE__, x_start, y_start, width, height, video_back_buffer);
#endif

    // vbuf = video_back_buffer;
    vbuf_pos = (byte_ptr) MK_FP(FP_SEG(video_back_buffer),0);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] vbuf: %p\n", __FILE__, __LINE__, vbuf);
#endif

    outportb(e_SC_INDEX, e_SC_MAPMASK);
    mask = STU_VGA_WriteMapMasks[(x_start & 0x03)];  // 0b00000011  ~== x modulo 4  (x % 4, x|4)

    vram_pos = MK_PTR_SGMT(p_DSP, (y_start * (((320/4)/16))), (x_start / 4));
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] vram_pos: %p\n", __FILE__, __LINE__, vram_pos);
#endif

    while(width--)
    {
        vbuf = vbuf_pos;
        vram = vram_pos;
// #ifdef STU_DEBUG
// HERE: vbuf should be previous vbuf + 200
//     dbg_prn("DEBUG: [%s, %d] vram: %p\n", __FILE__, __LINE__, vram);
// #endif
        outportb(e_SC_DATA, mask);
        mask = mask * 2;  // {1,2,4,8} * 2 = {2,4,8,16}
        if(mask >= 9)  /* 0b00010000 */
        {
            mask = 1;  /* 0b00000001 */
            vbuf_pos++;
            vram_pos++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] vram_pos: %p\n", __FILE__, __LINE__, vram_pos);
// #endif
        }

        while(height--)
        {
            *vram = *vbuf;
            vbuf += 320;
            vram += 80;
        }

    }  /* while (width--) */

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Draw_Video_Back_Buffer(x_start = %d, y_start = %d, width = %d, height = %d, video_back_buffer = %p)\n", __FILE__, __LINE__, x_start, y_start, width, height, video_back_buffer);
#endif
}

void STU_Export_Palette_XBGR(void)
{
    char filename[13] = "PAL_XBGR.BIN";
    FILE * fileptr;
    int itr;
    byte baito;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: STU_Export_Palette_XBGR()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] p_Palette_XBGR: %p\n", __FILE__, __LINE__, p_Palette_XBGR);
#endif


    fileptr = fopen(filename, "wb");
    if(fileptr == NULL)
    {
        abort();
    }

    fwrite(p_Palette_XBGR, 1024, 1, fileptr);

    fclose(fileptr);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: STU_Export_Palette_XBGR()\n", __FILE__, __LINE__);
#endif

}

void STU_Export_VBB_To_BIN(void)
{
    char filename[13] = "VBB.BIN";
    FILE * fileptr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: STU_Export_VBB_To_BIN()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer: %p\n", __FILE__, __LINE__, video_back_buffer);
#endif


    fileptr = fopen(filename, "wb");
    if(fileptr == NULL)
    {
        abort();
    }

    fwrite(video_back_buffer, 64000, 1, fileptr);

    fclose(fileptr);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: STU_Export_VBB_To_BIN()\n", __FILE__, __LINE__);
#endif

}

void STU_Export_VBB_To_BMP(void)
{

    char filename[13] = "VBB.BMP";
    FILE * fileptr;
    int itr;
    unsigned long int bmp_image_size;
    byte color_table_index;
    struct s_BMP_BITMAP_FILE_HEADER VBB_BMPINFOHEADER;
    struct s_BMP_BITMAP_INFO_HEADER VBB_BMPINFOHEADERFILE;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: STU_Export_VBB_To_BMP()\n", __FILE__, __LINE__);
#endif

    // VBB_BMPINFOHEADER.bfType = BMP_MAGIC;
    VBB_BMPINFOHEADER.bfType = 0x4D42;
    VBB_BMPINFOHEADER.bfSize = 65078;
    VBB_BMPINFOHEADER.bfReserved1 = 0;
    VBB_BMPINFOHEADER.bfReserved2 = 0;
    VBB_BMPINFOHEADER.bfOffBits = 1078;

    VBB_BMPINFOHEADERFILE.biSize = 40;
    VBB_BMPINFOHEADERFILE.biWidth = 320;
    VBB_BMPINFOHEADERFILE.biHeight = -200;
    VBB_BMPINFOHEADERFILE.biPlanes = 1;
    VBB_BMPINFOHEADERFILE.biBitCount = 8;
    VBB_BMPINFOHEADERFILE.biCompression = 0;
    VBB_BMPINFOHEADERFILE.biSizeImage = 64000;
    // VBB_BMPINFOHEADERFILE.biSizeImage = 0;
    VBB_BMPINFOHEADERFILE.biXPelsPerMeter = 0;
    VBB_BMPINFOHEADERFILE.biYPelsPerMeter = 0;
    VBB_BMPINFOHEADERFILE.biClrUsed = 0;
    VBB_BMPINFOHEADERFILE.biClrImportant = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfType: %04X\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfType);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfSize: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfSize, VBB_BMPINFOHEADER.bfSize);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfReserved1: %04X\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfReserved1);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfReserved2: %04X\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfReserved2);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfOffBits: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfOffBits, VBB_BMPINFOHEADER.bfOffBits);

    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biSize: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biSize, VBB_BMPINFOHEADERFILE.biSize);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biWidth: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biWidth, VBB_BMPINFOHEADERFILE.biWidth);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biHeight: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biHeight, VBB_BMPINFOHEADERFILE.biHeight);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biPlanes: %04X (%d)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biPlanes, VBB_BMPINFOHEADERFILE.biPlanes);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biBitCount: %04X (%d)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biBitCount, VBB_BMPINFOHEADERFILE.biBitCount);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biCompression: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biCompression, VBB_BMPINFOHEADERFILE.biCompression);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biSizeImage: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biSizeImage, VBB_BMPINFOHEADERFILE.biSizeImage);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biXPelsPerMeter: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biXPelsPerMeter, VBB_BMPINFOHEADERFILE.biXPelsPerMeter);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biYPelsPerMeter: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biYPelsPerMeter, VBB_BMPINFOHEADERFILE.biYPelsPerMeter);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biClrUsed: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biClrUsed, VBB_BMPINFOHEADERFILE.biClrUsed);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biClrImportant: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biClrImportant, VBB_BMPINFOHEADERFILE.biClrImportant);
#endif


    fileptr = fopen(filename, "wb");
    // if(fileptr == NULL) { }

    // fwrite(&VBB_BMPINFOHEADER, sizeof(VBB_BMPINFOHEADER), 1, fileptr);
    // fwrite(&VBB_BMPINFOHEADERFILE, sizeof(VBB_BMPINFOHEADERFILE), 1, fileptr);
    fwrite(&VBB_BMPINFOHEADER, 14, 1, fileptr);
    fwrite(&VBB_BMPINFOHEADERFILE, 40, 1, fileptr);

    fwrite(p_Palette_XBGR, (256*4), 1, fileptr);

    // bmp_image_size = VBB_BMPINFOHEADERFILE.biWidth * -VBB_BMPINFOHEADERFILE.biHeight;
    // color_table_index = MAGENTA;
    // for(itr = 0; itr < bmp_image_size; itr++)
    // {
    //     fwrite(&color_table_index, sizeof(color_table_index), 1, fileptr);
    // }

    fwrite(video_back_buffer, 64000, 1, fileptr);

    fclose(fileptr);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: STU_Export_VBB_To_BMP()\n", __FILE__, __LINE__);
#endif

}


void STU_Export_VBB_To_BMP32(void)
{

    char filename[13] = "VBB32.BMP";
    FILE * fileptr;
    long itr;
    struct s_BMP_BITMAP_FILE_HEADER VBB_BMPINFOHEADER;
    struct s_BMP_BITMAP_INFO_HEADER VBB_BMPINFOHEADERFILE;
    unsigned long color;
    byte vbb_byte;
    // byte pixel_XBGR[4] = {1,2,3,4};
    unsigned long * p_XBGR;
    unsigned long long_XBGR;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: STU_Export_VBB_To_BMP32()\n", __FILE__, __LINE__);
#endif

    // VBB_BMPINFOHEADER.bfType = BMP_MAGIC;
    VBB_BMPINFOHEADER.bfType = 0x4D42;
    VBB_BMPINFOHEADER.bfSize = 64054;
    VBB_BMPINFOHEADER.bfReserved1 = 0;
    VBB_BMPINFOHEADER.bfReserved2 = 0;
    VBB_BMPINFOHEADER.bfOffBits = 54;

    VBB_BMPINFOHEADERFILE.biSize = 40;
    VBB_BMPINFOHEADERFILE.biWidth = 320;
    VBB_BMPINFOHEADERFILE.biHeight = -200;
    VBB_BMPINFOHEADERFILE.biPlanes = 1;
    VBB_BMPINFOHEADERFILE.biBitCount = 32;
    VBB_BMPINFOHEADERFILE.biCompression = 0;
    VBB_BMPINFOHEADERFILE.biSizeImage = 64000;
    VBB_BMPINFOHEADERFILE.biXPelsPerMeter = 0;
    VBB_BMPINFOHEADERFILE.biYPelsPerMeter = 0;
    VBB_BMPINFOHEADERFILE.biClrUsed = 0;
    VBB_BMPINFOHEADERFILE.biClrImportant = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfType: %04X\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfType);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfSize: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfSize, VBB_BMPINFOHEADER.bfSize);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfReserved1: %04X\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfReserved1);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfReserved2: %04X\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfReserved2);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfOffBits: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfOffBits, VBB_BMPINFOHEADER.bfOffBits);

    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biSize: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biSize, VBB_BMPINFOHEADERFILE.biSize);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biWidth: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biWidth, VBB_BMPINFOHEADERFILE.biWidth);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biHeight: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biHeight, VBB_BMPINFOHEADERFILE.biHeight);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biPlanes: %04X (%d)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biPlanes, VBB_BMPINFOHEADERFILE.biPlanes);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biBitCount: %04X (%d)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biBitCount, VBB_BMPINFOHEADERFILE.biBitCount);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biCompression: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biCompression, VBB_BMPINFOHEADERFILE.biCompression);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biSizeImage: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biSizeImage, VBB_BMPINFOHEADERFILE.biSizeImage);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biXPelsPerMeter: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biXPelsPerMeter, VBB_BMPINFOHEADERFILE.biXPelsPerMeter);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biYPelsPerMeter: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biYPelsPerMeter, VBB_BMPINFOHEADERFILE.biYPelsPerMeter);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biClrUsed: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biClrUsed, VBB_BMPINFOHEADERFILE.biClrUsed);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biClrImportant: %08lX (%ld)\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biClrImportant, VBB_BMPINFOHEADERFILE.biClrImportant);
#endif


    fileptr = fopen(filename, "wb");
    // if(fileptr == NULL) { }

    // fwrite(&VBB_BMPINFOHEADER, sizeof(VBB_BMPINFOHEADER), 1, fileptr);
    // fwrite(&VBB_BMPINFOHEADERFILE, sizeof(VBB_BMPINFOHEADERFILE), 1, fileptr);
    fwrite(&VBB_BMPINFOHEADER, 14, 1, fileptr);
    fwrite(&VBB_BMPINFOHEADERFILE, 40, 1, fileptr);

    // color = (unsigned long) 0xFF << 24 | (unsigned long) 0xCC << 16 | (unsigned long) 0x33 << 8 | (unsigned long) 0x00;
    // for(itr = 0; itr < 64000; itr++)
    // {
    //     fwrite(&color, sizeof(color), 1, fileptr);
    // }

    p_XBGR = p_Palette_XBGR;
    for(itr = 0; itr < 64000; itr++)
    {
        // *(pixel_XBGR + 0) = *(p_Palette_XBGR + *(video_back_buffer + itr) + 0);
        // *(pixel_XBGR + 1) = *(p_Palette_XBGR + *(video_back_buffer + itr) + 1);
        // *(pixel_XBGR + 2) = *(p_Palette_XBGR + *(video_back_buffer + itr) + 2);
        // *(pixel_XBGR + 3) = *(p_Palette_XBGR + *(video_back_buffer + itr) + 3);
        // fwrite(pixel_XBGR, sizeof(pixel_XBGR), 1, fileptr);

        // vbb_byte = *(video_back_buffer + itr);
        // // *(p_Palette_XBGR + vbb_byte + 0);
        // // color = (unsigned long) *(p_Palette_XBGR + vbb_byte + 0) << 24 | (unsigned long) *(p_Palette_XBGR + vbb_byte + 1) << 16 | (unsigned long) *(p_Palette_XBGR + vbb_byte + 2) << 8 | (unsigned long) 0x00;
        // // // color = (unsigned long) *(p_Palette_XBGR + vbb_byte + 0) << 24 | (unsigned long) *(p_Palette_XBGR + vbb_byte + 1) << 16 | (unsigned long) *(p_Palette_XBGR + vbb_byte + 2) << 8 | (unsigned long) *(p_Palette_XBGR + vbb_byte + 3);
        // color = (unsigned long) 0x00 << 24 | (unsigned long) *(p_Palette_XBGR + vbb_byte + 0) << 16 | (unsigned long) *(p_Palette_XBGR + vbb_byte + 1) << 8 | (unsigned long) *(p_Palette_XBGR + vbb_byte + 2);
        // fwrite(&color, sizeof(color), 1, fileptr);

        vbb_byte = *(video_back_buffer + itr);
        long_XBGR = *(p_XBGR + vbb_byte);
        fwrite(&long_XBGR, sizeof(long_XBGR), 1, fileptr);

    }

    fclose(fileptr);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: STU_Export_VBB_To_BMP32()\n", __FILE__, __LINE__);
#endif

}

void STU_Export_DSP_To_BMP(void)
{

    char filename[13] = "VBB.BMP";
    FILE * fileptr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: STU_Export_DSP_To_BMP()\n", __FILE__, __LINE__);
#endif


    fileptr = fopen(filename, "wb");


    fclose(fileptr);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: STU_Export_DSP_To_BMP()\n", __FILE__, __LINE__);
#endif

}

void STU_VGA_RAM_Read_ScanLine(WORD ScanLine, BYTE * ScanLine_Buffer)
{
    WORD Src_Sgmt;
    WORD Src_Ofst;
    WORD Dst_Sgmt;
    WORD Dst_Ofst;
    BYTE _FAR * fp_Src;
    BYTE _FAR * fp_Dst;
    unsigned int itr_ScanLine_Bytes;
    WORD sa_DSP;
    BYTE _FAR * fp_VRAM;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: STU_VGA_RAM_Read_ScanLine(ScanLine = %u, ScanLine_Buffer = <ADR>)\n", __FILE__, __LINE__, ScanLine);
#endif

    sa_DSP = 0xA000;
    Src_Sgmt = sa_DSP;  // Segment Address of the 'Display Screen-Page'
    Src_Ofst = ScanLine * 80;
    Dst_Sgmt = FP_SEG(ScanLine_Buffer);
    Dst_Ofst = FP_OFF(ScanLine_Buffer);
    fp_VRAM = (BYTE _FAR *)MK_FP(Src_Sgmt,Src_Ofst);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] ScanLine: %u\n", __FILE__, __LINE__, ScanLine);
    dbg_prn("DEBUG: [%s, %d] sa_DSP: 0x%04X\n", __FILE__, __LINE__, sa_DSP);
    dbg_prn("DEBUG: [%s, %d] Src_Sgmt: 0x%04X\n", __FILE__, __LINE__, Src_Sgmt);
    dbg_prn("DEBUG: [%s, %d] Src_Ofst: 0x%04X\n", __FILE__, __LINE__, Src_Ofst);
    dbg_prn("DEBUG: [%s, %d] Dst_Sgmt: 0x%04X\n", __FILE__, __LINE__, Dst_Sgmt);
    dbg_prn("DEBUG: [%s, %d] Dst_Ofst: 0x%04X\n", __FILE__, __LINE__, Dst_Ofst);
    dbg_prn("DEBUG: [%s, %d] fp_VRAM: %Fp\n", __FILE__, __LINE__, fp_VRAM);
#endif

#ifdef STU_DEBUG
    if(ScanLine == 1)
    {
        dbg_prn("DEBUG: [%s, %d] ScanLine: %u\n", __FILE__, __LINE__, ScanLine);

        for(itr_ScanLine_Bytes = 0; itr_ScanLine_Bytes < 80; itr_ScanLine_Bytes++)
        {
            outportb(GC_INDEX,0x04);  // 0x03CE, 0x04; Graphics Controller, Read Map Select Register
            outportb(GC_DATA,0x00);   // 0x03CF, 0x00; Memory Plane Map Index
            dbg_prn("DEBUG: [%s, %d] *fp_VRAM: 0x%02X\n", __FILE__, __LINE__, *fp_VRAM);
            outportb(GC_INDEX,0x04);  // 0x03CE, 0x04; Graphics Controller, Read Map Select Register
            outportb(GC_DATA,0x01);   // 0x03CF, 0x00; Memory Plane Map Index
            dbg_prn("DEBUG: [%s, %d] *fp_VRAM: 0x%02X\n", __FILE__, __LINE__, *fp_VRAM);
            outportb(GC_INDEX,0x04);  // 0x03CE, 0x04; Graphics Controller, Read Map Select Register
            outportb(GC_DATA,0x02);   // 0x03CF, 0x00; Memory Plane Map Index
            dbg_prn("DEBUG: [%s, %d] *fp_VRAM: 0x%02X\n", __FILE__, __LINE__, *fp_VRAM);
            outportb(GC_INDEX,0x04);  // 0x03CE, 0x04; Graphics Controller, Read Map Select Register
            outportb(GC_DATA,0x03);   // 0x03CF, 0x00; Memory Plane Map Index
            dbg_prn("DEBUG: [%s, %d] *fp_VRAM: 0x%02X\n", __FILE__, __LINE__, *fp_VRAM);
            fp_VRAM++;
        }
    }
#endif

    for(itr_ScanLine_Bytes = 0; itr_ScanLine_Bytes < 80; itr_ScanLine_Bytes++)
    {
        outportb(GC_INDEX,0x04);  // 0x03CE, 0x04; Graphics Controller, Read Map Select Register
        outportb(GC_DATA,0x00);   // 0x03CF, 0x00; Memory Plane Map Index
        // *((BYTE _FAR *)MK_FP(Dst_Sgmt,Dst_Ofst)) = *((BYTE _FAR *)MK_FP(Src_Sgmt,Src_Ofst));
        // *ScanLine_Buffer = *((BYTE _FAR *)MK_FP(Src_Sgmt,Src_Ofst));
        *ScanLine_Buffer++ = *fp_VRAM;
        // Src_Ofst--;
        outportb(GC_INDEX,0x04);  // 0x03CE, 0x04; Graphics Controller, Read Map Select Register
        outportb(GC_DATA,0x01);   // 0x03CF, 0x00; Memory Plane Map Index
        // *((BYTE _FAR *)MK_FP(Dst_Sgmt,Dst_Ofst)) = *((BYTE _FAR *)MK_FP(Src_Sgmt,Src_Ofst));
        // *ScanLine_Buffer = *((BYTE _FAR *)MK_FP(Src_Sgmt,Src_Ofst));
        *ScanLine_Buffer++ = *fp_VRAM;
        // Src_Ofst--;
        outportb(GC_INDEX,0x04);  // 0x03CE, 0x04; Graphics Controller, Read Map Select Register
        outportb(GC_DATA,0x02);   // 0x03CF, 0x00; Memory Plane Map Index
        // *((BYTE _FAR *)MK_FP(Dst_Sgmt,Dst_Ofst)) = *((BYTE _FAR *)MK_FP(Src_Sgmt,Src_Ofst));
        // *ScanLine_Buffer = *((BYTE _FAR *)MK_FP(Src_Sgmt,Src_Ofst));
        *ScanLine_Buffer++ = *fp_VRAM;
        // Src_Ofst--;
        outportb(GC_INDEX,0x04);  // 0x03CE, 0x04; Graphics Controller, Read Map Select Register
        outportb(GC_DATA,0x03);   // 0x03CF, 0x00; Memory Plane Map Index
        // *((BYTE _FAR *)MK_FP(Dst_Sgmt,Dst_Ofst)) = *((BYTE _FAR *)MK_FP(Src_Sgmt,Src_Ofst));
        // *ScanLine_Buffer = *((BYTE _FAR *)MK_FP(Src_Sgmt,Src_Ofst));
        *ScanLine_Buffer++ = *fp_VRAM;
        // Src_Ofst--;
        fp_VRAM++;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: STU_VGA_RAM_Read_ScanLine(ScanLine = %u, ScanLine_Buffer = <ADR>)\n", __FILE__, __LINE__, ScanLine);
#endif
}

// void STU_VGA_RAM_Dump(char * filename)
void STU_VGA_RAM_Dump(void)
{
    WORD itr_ScanLines;
    BYTE ScanLine_Buffer[320];
    char filename[13] = "VGARAM.BIN";
    FILE * fileptr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: STU_VGA_RAM_Dump()\n", __FILE__, __LINE__);
#endif

    fileptr = fopen(filename, "wb");

    /*
        Set the 'VGA Graphics Controller' 'Graphics Mode Register' to 'Read Mode 0'
    */
    // outportb(GC_INDEX,0x05);
    // outportb(GC_DATA,0x00);
    outport(GC_INDEX,0x0005);  // 0x03CE, 0x05, 0x00; Graphics Controller, Graphics Mode Register, Read Mode 0

    //STU_VGA_DAC_Read(&palette);
    for(itr_ScanLines = 0; itr_ScanLines < 200; itr_ScanLines++)
    {
        STU_VGA_RAM_Read_ScanLine(itr_ScanLines, (BYTE *)&ScanLine_Buffer);  // TODO(JimBalcomb,20221004): Suspicious Pointer Conversion!!!!!
        fwrite(&ScanLine_Buffer, 320, 1, fileptr);
    }

    fclose(fileptr);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: STU_VGA_RAM_Dump()\n", __FILE__, __LINE__);
#endif

}

// void STU_VGA_RAM_Dump(char * filename)
void BAD__STU_VGA_RAM_Dump_1(void)
{
    // unsigned int itr_scanlines;
    // unsigned int itr;
    // unsigned char scanline[320];
    char filename[13] = "VGARAM.BIN";
    FILE * fileptr;
    // unsigned char Plane;
    unsigned int Width;
    unsigned int Height;
    unsigned char itr_planes;
    unsigned int itr_width;
    unsigned int itr_height;
    unsigned char baito;
    word ScreenPage_Offset;
    word Src_Sgmt;
    word Src_Ofst;
    word * pScreenPage;
    int ScreenPage_Idx;
    int Row_Offset;
    int Column_Offset;
    int CR_X;
    int CR_Y;
    unsigned int itr_count;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: STU_VGA_RAM_Dump_1()\n", __FILE__, __LINE__);
#endif

    fileptr = fopen(filename, "wb");

    Width = 320;
    Height = 200;

    ScreenPage_Idx = 0;
    CR_X = 0;
    CR_Y = 0;

    Src_Sgmt = VRAM_BASE + (((ScreenPage_Idx << 8) & 0xFF00) * 4);  // ScreenPage Segment Address
    // gsa_DSP_Addr = VRAM_BASE + ( (g_RSP_Idx) << 10 );

    pScreenPage = (word *) MK_FP(Src_Sgmt, 0);

    Row_Offset = (CR_X >> 2);   // PIXELS_PER_ADDRESS  e.g., 32 /4 = 8
    Column_Offset = CR_Y * 80;  // SCREEN_WIDTH_PER_PLANE  e.g., 20 * 80 - 1600
    ScreenPage_Offset = Column_Offset + Row_Offset;  // e.g., 1600 + 8 = 1608


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] Width: %u\n", __FILE__, __LINE__, Width);
    dbg_prn("DEBUG: [%s, %d] Height: %u\n", __FILE__, __LINE__, Height);
    dbg_prn("DEBUG: [%s, %d] CR_X: %d\n", __FILE__, __LINE__, CR_X);
    dbg_prn("DEBUG: [%s, %d] CR_Y: %d\n", __FILE__, __LINE__, CR_Y);
    dbg_prn("DEBUG: [%s, %d] ScreenPage_Idx: %d\n", __FILE__, __LINE__, ScreenPage_Idx);
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] Src_Sgmt: 0x%04X\n", __FILE__, __LINE__, Src_Sgmt);
    dbg_prn("DEBUG: [%s, %d] pScreenPage: %Fp\n", __FILE__, __LINE__, pScreenPage);
    dbg_prn("DEBUG: [%s, %d] Row_Offset: %d\n", __FILE__, __LINE__, Row_Offset);
    dbg_prn("DEBUG: [%s, %d] Column_Offset: %d\n", __FILE__, __LINE__, Column_Offset);
    dbg_prn("DEBUG: [%s, %d] ScreenPage_Offset: %u\n", __FILE__, __LINE__, ScreenPage_Offset);

#endif


    /*
        Loops: Planes, Width, Height
    */

    outportb(GC_INDEX, GC_READMAP);  // GC_INDEX 0x03CE, GC_READMAP 0x04

    itr_count = 0;
    for(itr_planes = 0; itr_planes < 4; itr_planes++)
    {
        outportb(GC_DATA, itr_planes);
        Src_Ofst = ScreenPage_Offset;
        itr_width = 0;
        while(itr_width < Width)
        {
            for(itr_height = 0; itr_height < Height; itr_height++)
            {
                itr_count++;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] itr_count: %d  itr_plane: %d  itr_width: %d  itr_height: %d\n", __FILE__, __LINE__, itr_count, itr_planes, itr_width, itr_height);
#endif
                // baito = ((unsigned int)*((unsigned char *)pScreenPage + Src_Ofst + 0)) | (unsigned int)((unsigned int)*((unsigned char *)pScreenPage + Src_Ofst + 1) << 8);
                baito = *(pScreenPage + Src_Ofst);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] %Fp[0x%04X (%u)][%d]: baito: 0x%02X\n", __FILE__, __LINE__, pScreenPage, Src_Ofst, Src_Ofst, itr_planes, baito);
#endif
                fwrite(&baito, 1, 1, fileptr);

                Src_Ofst += 80;
            }
            itr_width++;
            Src_Ofst = ScreenPage_Offset + (itr_width * 2);  // + {0,2,4}
        }
    }

    fclose(fileptr);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: STU_VGA_RAM_Dump_1()\n", __FILE__, __LINE__);
#endif

}

void STU_VGA_DAC_Read(unsigned char *palette)
{
    unsigned int itr;

    outport( 0x3C7, 0 );

    for ( itr = 0; itr < 768; itr++ )
    {
        palette[itr] = inportb( 0x3C9 );
    }
}

void STU_VGA_DAC_Write(unsigned char *palette)
{
    unsigned int itr;

    outport( 0x3C8, 0 );

    for ( itr = 0; itr < 768; itr++ )
    {
        outport( 0x3C9, palette[itr] );
    }
}

void STU_VGA_DAC_Dump(char *PaletteFilename)
{
    unsigned int itr;
    unsigned char palette[768];
    //char PaletteFilename[13] = "VGADAC.BIN";
    FILE *fdPaletteFile;

    fdPaletteFile = fopen(PaletteFilename, "wb");

    outport( 0x3C7, 0 );

    //STU_VGA_DAC_Read(&palette);
    for ( itr = 0; itr < 768; itr++ )
    {
        palette[itr] = inportb( 0x3C9 );
    }

    fwrite(palette, 768, 1, fdPaletteFile);

    fclose(fdPaletteFile);
    
}

// 6bpp, 8bpp
void DAC2PAL(unsigned char *DAC, unsigned char *PAL)
{
    unsigned int itr;
    unsigned char dac6bpp;
    unsigned char pal8bpp;
    
    for ( itr = 0; itr < 768; itr++ )
    {
        dac6bpp = DAC[itr];
        pal8bpp = (dac6bpp << 2);
        PAL[itr] = pal8bpp;
    }

}

// 8bpp, 6bpp
void PAL2DAC(unsigned char *PAL, unsigned char *DAC)
{

}


// J:\STU\DBWD\developc\wolf3d\WOLFSRC\ID_VL.C

//===========================================================================

/*
=================
=
= VL_ClearVideo
=
= Fill the entire video buffer with a given color
=
=================
*/

void VL_ClearVideo (byte color)
{
asm	mov	dx,GC_INDEX
asm	mov	al,GC_MODE
asm	out	dx,al
asm	inc	dx
asm	in	al,dx
asm	and	al,0xfc				// write mode 0 to store directly to video
asm	out	dx,al

asm	mov	dx,SC_INDEX
asm	mov	ax,SC_MAPMASK+15*256  // sets all four plane bits  0x02 + (15 * 256) = 0x02 + (0x0F * 0x0100) = 0x02 + (0x0F00) = 0x0F02  0000 1111 0000 0010  i.e., (0x1111 << 8) | 0x02
asm	out	dx,ax				// write through all four planes

asm	mov	ax,SCREENSEG
asm	mov	es,ax
asm	mov	al,[color]
asm	mov	ah,al
asm	mov	cx,0x8000			// 0x8000 words, clearing 8 video bytes/word
asm	xor	di,di
asm	rep	stosw
}



// J:\STU\DBWD\developc\_root42\letscode-breakout-master\vga.c

#define SET_MODE 0x00
#define VIDEO_INT 0x10
#define VGA_256_COLOR_MODE 0x13
#define TEXT_MODE 0x03

byte far * const VGA=(byte far *)0xA0000000L;

/* dimensions of each page and offset */
word vga_width = 320;
word vga_height = 200;
word vga_page[4];
word vga_current_page = 0;
word vga_x_pan = 0;
byte vga_x_pel_pan = 0;
word vga_y_pan = 0;

// ...

// ? C&P from David Brackeen - 256-Color VGA Programming in C ?
// void set_mode(byte mode)
// {
//     union REGS regs;
//     regs.h.ah = SET_MODE;
//     regs.h.al = mode;
//     int86( VIDEO_INT, &regs, &regs );
// }

void update_page_offsets()
{
  vga_page[0] = 0;
  vga_page[1] = ((dword)vga_width*vga_height) / 4;
  vga_page[2] = vga_page[1] * 2;
  vga_page[3] = vga_page[1] * 3;
}

void set_mode_y()
{
    set_mode( VGA_256_COLOR_MODE );
    update_page_offsets();
    /* disable chain 4 */
    outportb( SC_INDEX, MEMORY_MODE );
    outportb( SC_DATA, 0x06 );
    /* disable doubleword mode */
    outportb( CRTC_INDEX, UNDERLINE_LOCATION );
    outportb( CRTC_DATA, 0x00 );
    /* disable word mode */
    outportb( CRTC_INDEX, MODE_CONTROL );
    outportb( CRTC_DATA, 0xE3 );
    /* clear all VGA mem */
    outportb( SC_INDEX, MAP_MASK );
    outportb( SC_DATA, 0xFF );
    /* write 2^16 nulls */
    memset( VGA + 0x0000, 0, 0x8000 ); /* 0x10000 / 2 = 0x8000 */
    memset( VGA + 0x8000, 0, 0x8000 ); /* 0x10000 / 2 = 0x8000 */
}
