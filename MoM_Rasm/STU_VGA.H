#ifndef STU_DBG_H
#define STU_DBG_H


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
#define SC_MAPMASK			2
// ...
#define GC_MODE				5
// ...
#define SCREENSEG		0xa000



// J:\STU\DBWD\developc\_root42\letscode-breakout-master\vga.h

// #include "types.h"
typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;


#define NUM_COLORS 256

/* VGA ports */
#define PALETTE_INDEX 0x3C8
#define PALETTE_DATA 0x3C9
#define INPUT_STATUS 0x03DA
#define MISC_OUTPUT 0x3C2
#define AC_WRITE 0x3C0
#define AC_READ 0x3C1
#define SC_INDEX 0x3C4
#define SC_DATA 0x3C5
#define GC_INDEX 0x03CE
#define GC_DATA 0x03CF
#define CRTC_INDEX 0x03D4
#define CRTC_DATA 0x03D5

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

void BAD__STU_VGA_RAM_Dump_1(void);
void STU_VGA_RAM_Dump(void);

void STU_VGA_DAC_Dump(char *PaletteFilename);


#ifdef __cplusplus
}
#endif

#endif /* STU_DBG_H */
