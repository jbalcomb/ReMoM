#ifndef STU_VGA_H
#define STU_VGA_H


#ifndef VRAM_BASE
#define VRAM_BASE 0xA000
#endif

// {GC_INDEX,GC_DATA}, {GC_SETRESET, GC_ENABLESETRESET, GC_COLORCOMPARE, GC_DATAROTATE, GC_READMAP, GC_MODE, GC_MISCELLANEOUS, GC_COLORDONTCARE, GC_BITMASK}
#ifndef GC_INDEX
#define GC_INDEX  0x03CE
#endif
#ifndef GC_DATA
#define GC_DATA   0x03CF
#endif
// #define GC_SETRESET			0
// #define GC_ENABLESETRESET 	1
// #define GC_COLORCOMPARE		2
// #define GC_DATAROTATE		3
#ifndef GC_READMAP
#define GC_READMAP 0x04
#endif
// #define GC_MODE				5
// #define GC_MISCELLANEOUS 	6
// #define GC_COLORDONTCARE 	7
// #define GC_BITMASK			8


// J:\STU\DBWD\developc\wolf3d\WOLFSRC\ID_VL.H
// ...
#define SC_MAPMASK			0x02
// ...
#define GC_MODE				5
// ...
#define SCREENSEG		0xa000



// J:\STU\DBWD\developc\_root42\letscode-breakout-master\vga.h

// #include "types.h"
typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
typedef unsigned char * byte_ptr;


#define NUM_COLORS 256

/* VGA Color Palette */
#define BLACK           0x00
#define BLUE            0x01
#define GREEN           0x02
#define CYAN            0x03
#define RED             0x04
#define MAGENTA         0x05
#define BROWN           0x06
#define WHITE           0x07
#define GRAY            0x08
#define LIGHT_BLUE      0x09
#define LIGHT_GREEN     0x0A
#define LIGHT_CYAN      0x0B
#define LIGHT_RED       0x0C
#define LIGHT_MAGENTA   0x0D
#define YELLOW          0x0E
#define BRIGHT_WHITE    0x0F



/* VGA ports */
/* Address Register, Data Register */
/* Sequencer, Graphics, and CRT Controller Registers */
#define PALETTE_INDEX   0x03C8
#define PALETTE_DATA    0x03C9
#define INPUT_STATUS    0x03DA
#define MISC_OUTPUT     0x03C2
#define AC_WRITE        0x03C0
#define AC_READ         0x03C1
#define SC_INDEX        0x03C4
#define SC_DATA         0x03C5
#define GC_INDEX        0x03CE
#define GC_DATA         0x03CF
#define CRTC_INDEX      0x03D4
#define CRTC_DATA       0x03D5

/* VGA status bits */
#define DISPLAY_ENABLE 0x01
#define VRETRACE 0x08

/* Attribute controller registers */
#define PEL_PANNING 0x13

/* Sequence controller registers */
#define MAP_MASK 0x02
#define ALL_PLANES 0xFF02
#define MEMORY_MODE 0x04

/* CRT controller registers */
#define HIGH_ADDRESS 0x0C
#define LOW_ADDRESS 0x0D
#define LINE_OFFSET 0x13
#define UNDERLINE_LOCATION 0x14
#define MODE_CONTROL 0x17

/* VGA memory pointer, dimensions of each page and offset */
extern byte far * const VGA;
extern word vga_width;
extern word vga_height;
extern word vga_page[4];

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))




#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


void Fill_VRAM(byte palette_index);

void Fill_Video_Back_Buffer(byte palette_index, byte_ptr video_back_buffer);
void Draw_Video_Back_Buffer(int x_start, int y_start, int width, int height, byte_ptr video_back_buffer);

void STU_Export_Palette_XBGR(void);

void STU_Export_VBB_To_BIN(void);
void STU_Export_VBB_To_BMP(void);
void STU_Export_DSP_To_BMP(void);
void STU_Export_VBB_To_BMP32(void);

void BAD__STU_VGA_RAM_Dump_1(void);
void STU_VGA_RAM_Dump(void);

void STU_VGA_DAC_Dump(char *PaletteFilename);


#ifdef __cplusplus
}
#endif

#endif /* STU_VGA_H */
