#ifndef ST_FLIC_H
#define ST_FLIC_H

// #include "ST_TYPE"
#include "ST_HEAD.H"

/*
J:\STU\DBWD\developc\_AnimatorPro\Pristine-Pro\DOC\FLCFILES.TXT

A frame chunk exists for each frame in the
animation.  In addition, a ring frame follows
all the animation frames.  Each frame chunk
contains color palette information and/or pixel
data.

The ring frame contains delta-compressed
information to loop from the last frame of the
flic back to the first.  It can be helpful to
think of the ring frame as a copy of the first
frame, compressed in a different way.  All flic
files will contain a ring frame, including a
single-frame flic.


		 6      2     frames    Number of
								frames in the
								flic.  This
								count does not
								include the
								ring frame. 
								FLC files have
								a maximum
								length of 4000
								frames.

		 8      2     width     Screen width in
								pixels.

		10      2     height    Screen height
								in pixels.

		80      4     oframe1   Offset from the
								beginning of
								the file to the
								first animation
								frame chunk.
...
		16      4     speed     Number of
								milliseconds to
								delay between
								each frame
								during
								playback.
...
		84      4     oframe2   Offset from the
								beginning of
								the file to the
								second
								animation frame
								chunk.  This
								value is used
								when looping
								from the ring
								frame back to
								the second
								frame during
								playback.

Frame chunks contain the pixel and color data
for the animation.  A frame chunk may contain
multiple subordinate chunks, each containing a
different type of data for the current frame. 
Each frame chunk starts with a 16-byte header
that describes the contents of the frame:

Offset  Length  Name            Description

		 0      4     size      The size of the
								frame chunk,
								including this
								header and all
								subordinate
								chunks that
								follow.

		 4      2     type      Frame chunk
								identifier. 
								Always hex
								F1FA.

		 6      2     chunks    Number of
								subordinate
								chunks in the
								frame chunk.

		 8      8     reserved  Unused space,
								set to zeroes.

Each data chunk within a frame chunk is
formatted as follows:

Offset  Length  Name            Description

		 0       4    size      The size of the
								chunk,
								including this
								header.

		 4       2    type      Data type
								identifier.

		 6   (size-6) data      The color or pixel data.


The type values in the chunk headers indicate
what type of graphics data the chunk contains
and which compression method was used to encode
the data.  The following values (and their
associated mnemonic names) are currently found
in frame data chunks:

	  Value  Name             Description

		4    FLI_COLOR256     256-level color palette info
		7    FLI_SS2          Word-oriented delta compression
	   11    FLI_COLOR        64-level color palette info
	   12    FLI_LC           Byte-oriented delta compression
	   13    FLI_BLACK        Entire frame is color index 0
	   15    FLI_BRUN         Byte run length compression
	   16    FLI_COPY         No compression
	   18    FLI_PSTAMP       Postage stamp sized image

The following sections describe each of these
data encoding methods in detail.


Chunk Type  4 (FLI_COLOR256) - 256-Level Color


The data in this chunk is organized in packets.
The first word following the chunk header is a
count of the number of packets in the chunk. 

Each packet consists of a one-byte color index
skip count, a one-byte color count and three
bytes of color information for each color
defined.

At the start of the chunk, the color index is
assumed to be zero.  Before processing any
colors in a packet, the color index skip count
is added to the current color index.  The
number of colors defined in the packet is
retrieved.  A zero in this byte indicates 256
colors follow.  The three bytes for each color
define the red, green, and blue components of
the color in that order.  Each component can
range from 0 (off) to 255 (full on).  
The data to change colors 2,7,8, and 9 would
appear as follows:

			2                       ; two packets 
			2,1,r,g,b               ; skip 2, change 1 
			4,3,r,g,b,r,g,b,r,g,b   ; skip 4, change 3


Chunk Type 11 (FLI_COLOR) - 64-Level Color

This chunk is identical to FLI_COLOR256 except
that the values for the red, green and blue
components are in the range of 0-63 instead of
0-255.


Chunk Type 15 (FLI_BRUN) - Byte Run Length Compression

This chunk contains the entire image in a compressed format.
  Usually this chunk is used in the first frame of an animation, or within a postage stamp image chunk. 

The data is organized in lines.
Each line contains packets of compressed pixels.
The first line is at the top of the animation, followed by subsequent lines moving downward.
The number of lines in this chunk is given by the height of the animation.

The first byte of each line is a count of packets in the line.
This value is ignored, it is a holdover from the original Animator.
It is possible to generate more than 255 packets on a line.
The width of the animation is now used to drive the decoding of packets on a line;
 continue reading and processing packets until width pixels have been processed, then proceed to the next line.

Each packet consist of a type/size byte, followed by one or more pixels.
If the packet type is negative it is a count of pixels to be copied from the packet to the animation image.
If the packet type is positive it contains a single pixel which is to be replicated;
 the absolute value of the packet type is the number of times the pixel is to be replicated.


Chunk Type 12 (FLI_LC) - Byte Aligned Delta Compression

This chunk contains the differences between the previous frame and this frame.
This compression method was used by the original Animator, but is not created by Animator Pro.
This type of chunk can appear in an Animator Pro file, however, if the file was originally created by Animator, then some (but not all) frames were modified using Animator Pro.

The first 16-bit word following the chunk header contains the position of the first line in the chunk.
This is a count of lines (down from the top of the image) which are unchanged from the prior frame.
The second 16-bit word contains the number of lines in the chunk.
The data for the lines follows these two words.

Each line begins with two bytes.
The first byte contains the starting x position of the data on the line, and the second byte the number of packets for the line.
Unlike BRUN compression, the packet count is significant (because this compression method is only used on 320x200 flics).

Each packet consists of a single byte column skip, followed by a packet type/size byte.
If the packet type is positive it is a count of pixels to be copied from the packet to the animation image.
If the packet type is negative it contains a single pixel which is to be replicated;
 the absolute value of the packet type gives the number of times the pixel is to be replicated.

Note
The negative/positive meaning of the packet type bytes in LC compression is reversed from that used in BRUN compression.  This gives better performance during playback.



FLI - Original Animator Flic Files

This animation file format is limited to 320x200 resolution.
It is the main animation file format of the original Animator, and is still used by Animator Pro for creating 320x200 animations.
The file structure is very similar to that of a FLC file.
A FLI file does not contain a prefix chunk, and does not use FLI_PSTAMP or FLI_SS2 data encoding in the frame chunks.

The FLI file header

The file header for a FLI file is a subset of the FLC file header.
It is defined as follows:

Offset  Length  Name            Description
...
		 6      2     frames    Number of
								frames in the
								flic.  This
								count does not
								include the
								ring frame. 
								FLI files have
								a maximum
								length of 4000
								frames.
...
		16      2     speed     Number of
								jiffies to
								delay between
								each frame
								during
								playback.  A
								jiffy is 1/70
								of a second.
...





J:\STU\DBWD\developc\Animator\Vpaint\QUICKFLI\fli.h
struct fli_head
	{
	LONG size;
	UWORD type;  / * = FLIH_MAGIC or FLIX_MAGIC * /
	UWORD frame_count;
	UWORD width;
	UWORD height;
	UWORD bits_a_pixel;
	WORD flags;
	WORD speed;
	LONG next_head;
	LONG frames_in_table;
	WORD file;
	LONG frame1_off;
	LONG strokes;	/ * how many paint strokes etc. made. * /
	LONG session; / * stokes since file's been loaded. * /
	char reserved[88];
	};

struct fli_frame
	{
	LONG size;
	UWORD type;		/ * = 0xf1fa FLIF_MAGIC * /
	WORD chunks;
	char reserved[8];
	};


DOC/FILES.DOC:  15      FLI_BRUN        byte-run-length compression - first frame only

FLI/COMP_AA.C:  FLI_BRUN,
FLI/COMP_ANI.C: FLI_BRUN,
FLI/FLICOMP.C:/ * given two screens, or one in the case of the types FLI_BRUN and FLI_COLOR_0
FLI/UNCOMP.C:                   case FLI_BRUN:
INC/FLI.H:      FLI_BRUN = 15,
INC/FLI.H:      FPS_BRUN = FLI_BRUN,            / * note this is the same * /
*/

// J:\STU\DBWD\developc\flilib\flisrc\aafli.h (c) 1990
// "FLI", "FLIH", "fli_head", "FLIF", "fli_frame"
struct s_FLIC_HDR //; (sizeof=0x10)
{
    /*00h*/word Width;                             // MAX: 320  aafli.h (1990) typedef struct fli_head { ... USHORT width;
    /*02h*/word Height;                            // MAX: 200
    /*04h*/word Current_Frame;                     // Run-Time
    /*06h*/word Frame_Count;                       //           aafli.h (1990) typedef struct fli_head { ... USHORT frame_count;
    /*08h*/word Loop_Frame;                        // ? i.e., last frame ? ~/Animator "Ring Frame"?
    /*0Ah*/byte EMM_Handle_Number;                 // Run-Time
    /*0Bh*/byte EMM_Logical_Page_Number;           // Run-Time
    /*0Ch*/word EMM_Logical_Page_Offset;           // Run-Time
    /*0Eh*/word Palette_Header_Offset;               // if 0, no palette data

    /*10h*/ // Frame-Type: {0:Delta-Frame, 1:Full-Frame}
    /*11h*/ // ?shading?

    /*12h*/ // Frame Index 0 Offset
    /*14h*/ // Frame Index 0 Offset

    /*16h*/ // Frame 0 Offset - End / Frame 1 Offset - Begin
            // ...
            // Frame n Offset - End
            // Palette Header
};
/*
# FLIC Palette Header
6E 00  FLIC Palette Data Offset
00 00  FLIC Palette Color Index
00 01  FLIC Palette Color Count
00     Flag - Frames have Palettes      used in FLIC_LoadPalette()  _s27p07c.c if not Frame Index 0 and frame_palettes is not 0
00     ?

    FLIC Palette Header //; (sizeof=0x08)
    00h word 
    02h word
    04h word
    06h byte
    07h ?
*/
/*
FLIC_HDR.EMM_Handle_Number
FLIC_HDR.EMM_Logical_Page_Number
FLIC_HDR.EMM_Logical_Page_Offset
*/
#define FlicHdr                         0x00

#define FlicHdr_Width                   0x00
#define FlicHdr_Height                  0x02
#define FlicHdr_CurrentFrame            0x04
#define FlicHdr_FrameCount              0x06
#define FlicHdr_LoopFrame               0x08
#define FlicHdr_EmmHandleNumber         0x0A
#define FlicHdr_EmmLogicalPageIndex     0x0B
#define FlicHdr_EmmLogicalPageOffset    0x0C
#define FlicHdr_PaletteDataOffset       0x0E
#define FlicHdr_FrameType               0x10    // ? GUI_CTRL.Param3 ... used in GUI_DrawControls  if not 0 then set FLIC Frame 1 else set FLIC Frame 0
                                                // FLIC_SetFrame(GUI_CTRL.Param5=FlicHdr_SgmtAddr (_SI + gfp_GUI_Control_Table), Frame_Index=1)

                                                // J:\STU\DBWD\developc\1oom-master\src\lbxgfx.h
                                                // #define lbxgfx_get_indep(_data_) ((_data_)[0x10])
                                                // J:\STU\DBWD\developc\1oom-master\src\gfxaux.c
                                                // void gfx_aux_draw_frame_to(uint8_t *data, struct gfx_aux_s *aux)
                                                // ... if (lbxgfx_get_indep(data)) { f = frame; } else { f = 0; lbxgfx_set_frame_0(data); }
#define FlicHdr_Shading                 0x11    // used in FLIC_Draw_EMM, if 0, continues, else calls FLIC_Draw_EMM_R; used in VGA_DrawLBXImage, if not 0, calls FLIC_Draw_R, else calls FLIC_Draw
#define FlicHdr_FrameOffsetTable        0x12    // 18 bytes from BoF; 4-byte values for offsets from BoF to the FLIC Frame Data


#define FLIC_RLE() { \
    asm mov dx, 3C5h; \
    asm mov al, ah; \
    asm out dx, al; \
    asm lodsb; \
    asm cmp al, 0FFh; \
    asm jz short SkipColumn; \
    asm mov dh, al; \
    asm lodsb; \
    asm mov dl, al; \
    asm cmp dh, 0; \
    asm js short ULC_Run_Count; \
    ULC_Copy: \
    asm mov cl, [si+1]; \
    asm xor ch, ch; \
    asm mov al, cl; \
    asm shl cx, 1; \
    asm shl cx, 1; \
    asm add cl, al; \
    asm adc ch, 0; \
    asm shl cx, 1; \
    asm shl cx, 1; \
    asm shl cx, 1; \
    asm shl cx, 1; \
    asm add di, cx; \
    asm lodsb; \
    asm mov cl, al; \
    asm xor ch, ch; \
    asm inc si; \
    asm dec dx; \
    asm dec dx; \
    asm sub dx, cx; \
    ULC_Copy_Do: \
    asm movsb; \
    asm add di, 4Fh; \
    asm loop ULC_Copy_Do; \
    asm cmp dx, 1; \
    asm jns short ULC_Copy; \
    SkipColumn: \
    asm pop di; \
    asm dec bx; \
    asm jz short Done; \
    asm shl ah, 1; \
    asm cmp ah, 9; \
    asm js short EndOfColumn_YaySkip; \
    asm mov ah, 1; \
    asm inc di; \
    EndOfColumn_YaySkip: \
    asm jmp short LoopLines; \
    ULC_Run_Count: \
    asm and dx, 7FFFh; \
    ULC_Run: \
    asm mov cl, [si+1]; \
    asm xor ch, ch; \
    asm mov al, cl; \
    asm shl cx, 1; \
    asm shl cx, 1; \
    asm add cl, al; \
    asm adc ch, 0; \
    asm shl cx, 1; \
    asm shl cx, 1; \
    asm shl cx, 1; \
    asm shl cx, 1; \
    asm add di, cx; \
    asm lodsb; \
    asm mov cl, al; \
    asm xor ch, ch; \
    asm inc si; \
    asm dec dx; \
    asm dec dx; \
    asm sub dx, cx; \
    LoopNegRun: \
    asm lodsb; \
    asm cmp al, 0E0h; \
    asm jnb short PalIdxGTE224; \
    CopyByte: \
    asm stosb; \
    asm add di, 4Fh; \
    asm loop LoopNegRun; \
    asm cmp dx, 1; \
    asm jns short ULC_Run; \
    asm jmp short SkipColumn; \
    PalIdxGTE224: \
    asm and al, 1Fh; \
    asm inc al; \
    asm push cx; \
    asm mov cl, al; \
    asm xor ch, ch; \
    asm lodsb; \
    RepeatByte: \
    asm stosb; \
    asm add di, 4Fh; \
    asm loop RepeatByte; \
    asm pop cx; \
    asm dec cx; \
    asm loop LoopNegRun; \
    asm cmp dx, 1; \
    asm jns short ULC_Run; \
    asm pop di; \
    asm dec bx; \
    asm jz short Done; \
    asm shl ah, 1; \
    asm cmp ah, 9; \
    asm js short EndOfColumn_NaySkip; \
    asm mov ah, 1; \
    asm inc di; \
    EndOfColumn_NaySkip: \
    asm jmp LoopLines; /* Relative jump out of range by 0038h bytes */\
    Done: \
}
// EndOfColumn_NaySkip: \
// asm jmp short LoopLines;
    // in FLIC_Draw:  Relative jump out of range by 0038h bytes
    // in FLIC_Draw_EMM:  Relative jump out of range by 004Eh bytes
// Done: \


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg027
*/
// _s27.asm
// _s27a.c
// _s27p01
void FLIC_Draw(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg);
void FLIC_Draw_A(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg);
void FLIC_Draw_C(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg);
// _s27p02
void FLIC_Draw_R(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg);
// void FLIC_Draw_R_A(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg);
// _s27p03
//void FLIC_Draw_EMM(int ScreenPage_X, int ScreenPage_Y, unsigned int Img_Seg, int Frame_Index);
//void FLIC_Draw_EMM_A2(int ScreenPage_X, int ScreenPage_Y, unsigned int Img_Seg, int Frame_Index);
void FLIC_Draw_EMM_A3(int ScreenPage_X, int ScreenPage_Y, unsigned int Img_Seg, int Frame_Index);
void FLIC_Draw_EMM_C(int ScreenPage_X, int ScreenPage_Y, unsigned int Img_Seg, int Frame_Index);
// _s27p04
void FLIC_EMM_MapNextPages(void);
void FLIC_EMM_MapNextPages_A(void);
// _s27p05
void FLIC_Draw_EMM_R(int ScreenPage_X, int ScreenPage_Y);
// void FLIC_Draw_EMM_R_A(int ScreenPage_X, int ScreenPage_Y, unsigned int Img_Seg);


/*
    seg028
*/
// _s28p02
void FLIC_Prepare(int Width, int Height, unsigned int SAMB_head);
// _s28p11
void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header);
// _s28p13
//void FLIC_BuildFrame(unsigned int FlicHdr_SgmtAddr, unsigned int Target_Seg);
// _s28p14
void FLIC_SetFrame(SAMB_addr sa_FLIC_Header, int Frame_Index);
// _s28p15
void FLIC_ResetFrame(SAMB_addr sa_FLIC_Header);
// _s28p16
word FLIC_GetCurFrame(SAMB_addr sa_FLIC_Header);


#ifdef __cplusplus
}
#endif

#endif /* ST_FLIC_H */
