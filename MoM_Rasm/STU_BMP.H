#ifndef STU_BMP_H
#define STU_BMP_H



#define BMP_MAGIC 0x4D42                    /* "BM" (0x42, 0x4D; "B", "M") */

#define BMP_PALETTE_6BPP_COLOR_COUNT 256
#define BMP_PALETTE_6BPP_BYTES_PER_COLOR 4  /* 8 bits_per_pixel, {x,B,G,R} */

#define BMP_PALETTE_8BPP_COLOR_COUNT 256
#define BMP_PALETTE_8BPP_BYTES_PER_COLOR 4  /* 8 bits_per_pixel, {A,B,G,R} */

/* J:\STU-EduMat\_FileFormats\_FileFormats_OPC\BMP\EasyBMP\EasyBMP_1.06\EasyBMP.h */
#ifndef BMP_XPELS_PER_METER_DEFAULT
/*#define _BMP_XPELSPERMETER_DEFAULT_*/
#define BMP_XPELS_PER_METER_DEFAULT 3780      /* set to a default of 96 dpi */
#endif
#ifndef BMP_YPELS_PER_METER_DEFAULT
/*#define _BMP_YPELSPERMETER_DEFAULT_*/
#define BMP_YPELS_PER_METER_DEFAULT 3780      /* set to a default of 96 dpi */
#endif

typedef char BMP_BYTE;
typedef int BMP_WORD;
typedef long BMP_DWORD;

typedef unsigned char BMP_PALETTE;
#define BMP_PALETTE_SIZE 1024

struct s_BMP_BGRX_QUAD {
    BMP_BYTE    rgbBlue;                    /* Specifies the intensity of blue in the color. */
    BMP_BYTE    rgbGreen;                   /* Specifies the intensity of green in the color. */
    BMP_BYTE    rgbRed;                     /* Specifies the intensity of red in the color. */
    BMP_BYTE    rgbReserved;                /* Is not used and must be set to zero. */
};
typedef struct s_BMP_BGRX_QUAD BMP_BGRX_QUAD;
#define BMP_RGB_QUAD_LENGTH 4

struct s_BMP_BITMAP_FILE_HEADER
{
    BMP_WORD    bfType;             /* Specifies the type of file. It must be BM. */
    BMP_DWORD   bfSize;             /* Specifies the size in DWORDs of the file.  */
    BMP_WORD    bfReserved1;        /* Is reserved and must be set to zero. */
    BMP_WORD    bfReserved2;        /* Is reserved and must be set to zero. */
    BMP_DWORD   bfOffBits;          /* Specifies, in bytes, the offset from the beginning of the file (BOF) to the image data (bitmap) */
};
typedef struct s_BMP_BITMAP_FILE_HEADER BMP_BITMAP_FILE_HEADER;
#define BMP_BITMAP_FILE_HEADER_LENGTH 14

struct s_BMP_BITMAP_INFO_HEADER{
    BMP_DWORD   biSize;              /* Specifies the number of bytes required by the BITMAPINFOHEADER structure. */
    BMP_DWORD   biWidth;             /* Specifies the width of the bitmap in pixels. */
    BMP_DWORD   biHeight;            /* Specifies the height of the bitmap in pixels. */
    BMP_WORD    biPlanes;            /* Specifies the number of planes for the target device and must be set to 1. */
    BMP_WORD    biBitCount;          /* Specifies the number of bits per pixel. This value must be 1, 4, 8, or 24. */
    BMP_DWORD   biCompression;       /* Specifies the type of compression for a compressed bitmap. */
    BMP_DWORD   biSizeImage;         /* Specifies the size in bytes of the image. */
    BMP_DWORD   biXPelsPerMeter;     /* */
    BMP_DWORD   biYPelsPerMeter;     /* */
    BMP_DWORD   biClrUsed;           /* */
    BMP_DWORD   biClrImportant;      /* */
};
typedef struct s_BMP_BITMAP_INFO_HEADER BMP_BITMAP_INFO_HEADER;
#define BMP_BITMAP_INFO_HEADER_LENGTH 40

struct s_BMP_BITMAP_INFO {
    BMP_BITMAP_INFO_HEADER    bmiHeader;    /* Specifies a BITMAPINFOHEADER data structure that contains information about the dimensions and color format of a device-independent bitmap. */
    BMP_BGRX_QUAD             bmiColors[1];  /* Specifies an array of RGBQUAD data structures that define the colors in the bitmap. */
};
typedef struct s_BMP_BITMAP_INFO BMP_BITMAP_INFO;
#define BMP_BITMAP_INFO_LENGTH                  BMP_BITMAP_INFO_HEADER_LENGTH + BMP_PALETTE_SIZE                // 40 + 1024 = 1064

struct s_BMP_FILE {
    BMP_BITMAP_FILE_HEADER * bmp_bitmap_file_header;
    BMP_BITMAP_INFO_HEADER * bmp_bitmap_info_header;

};
typedef struct s_BMP_FILE BMP_FILE;
#define BMP_FILE_LENGTH                         BMP_BITMAP_FILE_HEADER_LENGTH + BMP_BITMAP_INFO_HEADER_LENGTH   // 14 + 40 = 54


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



#ifdef __cplusplus
}
#endif

#endif /* STU_BMP_H */
