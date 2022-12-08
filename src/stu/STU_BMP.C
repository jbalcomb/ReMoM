
/*

win_BMP_00
http://www.brackeen.com/vga/bitmaps.html
http://www.brackeen.com/vga/source/bc31/bitmap.c.html
    / * Ignore the palette information for now. See palette.c for code to read the palette info. * /
http://www.brackeen.com/vga/source/bc31/palette.c.html

win_BMP_01
C:\devel\Win32BitmapViewer\Win32BitmapViewer\Win32BitmapViewer.cpp
C:\devel\Win32BitmapViewer\Win32BitmapViewer\Win32_LoadBmpFileBits.cpp
C:\devel\Win32BitmapViewer\Win32BitmapViewer\Win32_szBmpFileName.cpp
C:\devel\Win32BitmapViewer\Win32BitmapViewer\Win32_setbuffer.cpp

win_BMP_02
https://croakingkero.com/tutorials/load_bmp/


win_BMP_03
http://www.winprog.org/tutorial/bitmaps.html
http://www.winprog.org/tutorial/transparency.html


*/

/*

    Program-Flow
    
    Load File
        file name, file buffer, file buffer size
    
    Load BMP
        BMP buffer, BMP buffer size
        dimensions
        BMP Header



*/

#include "MoX_TYPE.H"

extern uint8_t g_Video_Back_Buffer[];
extern uint8_t g_Palette_XBGR[];

#include <stdio.h>          /* FILE; fclose(), fopen(), fread(), fwrite(), printf(); */


struct s_BMP_BITMAP_FILE_HEADER
{
    short    bfType;             /* Specifies the type of file. It must be BM. */
    unsigned int   bfSize;             /* Specifies the size in DWORDs of the file.  */
    unsigned short    bfReserved1;        /* Is reserved and must be set to zero. */
    unsigned short    bfReserved2;        /* Is reserved and must be set to zero. */
    unsigned int   bfOffBits;          /* Specifies, in bytes, the offset from the beginning of the file (BOF) to the image data (bitmap) */
};

struct s_BMP_BITMAP_INFO_HEADER{
    int   biSize;              /* Specifies the number of bytes required by the BITMAPINFOHEADER structure. */
    int   biWidth;             /* Specifies the width of the bitmap in pixels. */
    int   biHeight;            /* Specifies the height of the bitmap in pixels. */
    short    biPlanes;            /* Specifies the number of planes for the target device and must be set to 1. */
    short    biBitCount;          /* Specifies the number of bits per pixel. This value must be 1, 4, 8, or 24. */
    int   biCompression;       /* Specifies the type of compression for a compressed bitmap. */
    int   biSizeImage;         /* Specifies the size in bytes of the image. */
    int   biXPelsPerMeter;     /* */
    int   biYPelsPerMeter;     /* */
    int   biClrUsed;           /* */
    int   biClrImportant;      /* */
};


void STU_Export_VBB_To_BMP32(void)
{
    char filename[13] = "VBB32.BMP";
    FILE * fileptr;
    int itr;
    struct s_BMP_BITMAP_FILE_HEADER VBB_BMPINFOHEADER;
    struct s_BMP_BITMAP_INFO_HEADER VBB_BMPINFOHEADERFILE;
    unsigned int color;
    unsigned char vbb_byte;
    // byte pixel_XBGR[4] = {1,2,3,4};
    unsigned int * p_XBGR;
    unsigned int long_XBGR;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: STU_Export_VBB_To_BMP32()\n", __FILE__, __LINE__);
#endif

    // VBB_BMPINFOHEADER.bfType = BMP_MAGIC;
    // VBB_BMPINFOHEADER.bfType = 0x4D42;
    // VBB_BMPINFOHEADER.bfType = (short)0x4D42;
    VBB_BMPINFOHEADER.bfType = 19778;
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
    dbg_prn("DEBUG: [%s, %d] sizeof(VBB_BMPINFOHEADER.bfType): %d\n", __FILE__, __LINE__, sizeof(VBB_BMPINFOHEADER.bfType));
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
    // fwrite(&VBB_BMPINFOHEADER, 14, 1, fileptr);
    fwrite(&VBB_BMPINFOHEADER.bfType,       2, 1, fileptr);
    fwrite(&VBB_BMPINFOHEADER.bfSize,       4, 1, fileptr);
    fwrite(&VBB_BMPINFOHEADER.bfReserved1,  2, 1, fileptr);
    fwrite(&VBB_BMPINFOHEADER.bfReserved2,  2, 1, fileptr);
    fwrite(&VBB_BMPINFOHEADER.bfOffBits,    4, 1, fileptr);
    
    fwrite(&VBB_BMPINFOHEADERFILE, 40, 1, fileptr);

    // color = (unsigned long) 0xFF << 24 | (unsigned long) 0xCC << 16 | (unsigned long) 0x33 << 8 | (unsigned long) 0x00;
    // for(itr = 0; itr < 64000; itr++)
    // {
    //     fwrite(&color, sizeof(color), 1, fileptr);
    // }

    p_XBGR = (uint32_t *)g_Palette_XBGR;
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

        vbb_byte = *(g_Video_Back_Buffer + itr);
        long_XBGR = *(p_XBGR + vbb_byte);
        fwrite(&long_XBGR, sizeof(long_XBGR), 1, fileptr);

    }

    fclose(fileptr);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: STU_Export_VBB_To_BMP32()\n", __FILE__, __LINE__);
#endif

}
