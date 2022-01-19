/* VGA_HW.H */

typedef unsigned char byte;

/*

VGA Hardware

*/

#define SCREENWIDTH		80			// default screen width in bytes
#define MAXSCANLINES	200			// size of ylookup table

#define VIDEO_RAM_TEXT_MODE     0xB800
#define VIDEO_RAM_GRAPHCIS_MODE 0xA000

#define VGA_PAGE_1 0xA000
#define VGA_PAGE_2 0xA400
#define VGA_PAGE_3 0xA800
#define VGA_PAGE_4 0xAC00

#define VGA_PAGE_1_OFS 0x0000
#define VGA_PAGE_2_OFS 0x4000
#define VGA_PAGE_3_OFS 0x8000
#define VGA_PAGE_4_OFS 0xC000


/*
J:\STU\DBWD\wolf3d\WOLFSRC\WL_DEF.H
*/
#define SCREENBWIDE		80
#define SCREENSIZE		(SCREENBWIDE*208)
#define PAGE1START		0
#define PAGE2START		(SCREENSIZE)
#define PAGE3START		(SCREENSIZE*2u)
#define	FREESTART		(SCREENSIZE*3u)


/*
J:\STU\DBWD\wolf3d\WOLFSRC\ID_VL.C
*/
/*byte		far	palette1[256][3],far palette2[256][3];*/
byte			palette1[256][3],    palette2[256][3];
/*
=============================================================================

							PIXEL OPS

=============================================================================
*/

byte	pixmasks[4] = {1,2,4,8};
byte	leftmasks[4] = {15,14,12,8};
byte	rightmasks[4] = {1,3,7,15};
