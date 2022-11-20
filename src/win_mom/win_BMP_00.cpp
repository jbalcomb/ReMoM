
/*

win_BMP_00
http://www.brackeen.com/vga/bitmaps.html
http://www.brackeen.com/vga/source/bc31/bitmap.c.html
    / * Ignore the palette information for now. See palette.c for code to read the palette info. * /
http://www.brackeen.com/vga/source/bc31/palette.c.html

*/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
// #include <mem.h>
// #include <alloc.h>
#include <malloc.h>


#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define PALETTE_INDEX       0x03c8
#define PALETTE_DATA        0x03c9
#define INPUT_STATUS        0x03da
#define VRETRACE            0x08

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define NUM_COLORS          256       /* number of colors in mode 0x13 */

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

typedef struct tagBITMAP              /* the structure for a bitmap. */
{
    word width;
    word height;
    byte palette[256*3];
    byte *data;
} BITMAP;


/**************************************************************************
 *  fskip                                                                 *
 *     Skips bytes in a file.                                             *
 **************************************************************************/

void fskip(FILE *fp, int num_bytes)
{
    int i;
    for (i=0; i<num_bytes; i++)
        fgetc(fp);
}


/**************************************************************************
 *  load_bmp                                                              *
 *    Loads a bitmap file into memory.                                    *
 **************************************************************************/

void load_bmp(char *file, BITMAP *b)
{
    FILE *fp;
    long index;
    word num_colors;
    int x;

    /* open the file */
    if ((fp = fopen(file,"rb")) == NULL)
    {
        printf("Error opening file %s.\n",file);
        exit(1);
    }

    /* check to see if it is a valid bitmap file */
    if (fgetc(fp)!='B' || fgetc(fp)!='M')
    {
        fclose(fp);
        printf("%s is not a bitmap file.\n",file);
        exit(1);
    }

    /* read in the width and height of the image, and the
        number of colors used; ignore the rest */
    fskip(fp,16);
    fread(&b->width, sizeof(word), 1, fp);
    fskip(fp,2);
    fread(&b->height,sizeof(word), 1, fp);
    fskip(fp,22);
    fread(&num_colors,sizeof(word), 1, fp);
    fskip(fp,6);

    /* assume we are working with an 8-bit file */
    if (num_colors==0) num_colors=256;

    /* try to allocate memory */
    if ((b->data = (byte *) malloc((word)(b->width*b->height))) == NULL)
    {
        fclose(fp);
        printf("Error allocating memory for file %s.\n",file);
        exit(1);
    }

    /* read the palette information */
    for(index=0;index<num_colors;index++)
    {
        b->palette[(int)(index*3+2)] = fgetc(fp) >> 2;
        b->palette[(int)(index*3+1)] = fgetc(fp) >> 2;
        b->palette[(int)(index*3+0)] = fgetc(fp) >> 2;
        x=fgetc(fp);
    }

    /* read the bitmap */
    for(index=(b->height-1)*b->width;index>=0;index-=b->width)
    for(x=0;x<b->width;x++)
        b->data[(word)(index+x)]=(byte)fgetc(fp);

    fclose(fp);
}
