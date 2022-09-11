// TITLE _s27a.c FLIC_Draw_...
// ST_FLIC.H

#include <DOS.H>

#include "ST_HEAD.H"

#include "ST_EMM.H"
#include "ST_FLIC.H"
#include "ST_VGA.H"

#include "STU_BITS.H"
#include "STU_DBG.H"

//unsigned char VGA_WriteMapMasks3[4] = { 0x01, 0x02, 0x04, 0x08 };

// Code Segment Variables from Dasm
// static unsigned int cseg_Flic_Width;
// static unsigned int cseg_Flic_EmmHandleNumber;
// static unsigned int cseg_Flic_EmmLogicalPageIndex;
// static unsigned int cseg_Flic_EmmLogicalPageOffset;
// // static unsigned int tmpFlicHdrEmmLogicalPageOffset;
// // static unsigned int tmpFlicHdrEmmHandleNumber;
// // static unsigned int tmpFlicHdrWidth_mdl;
// // static unsigned int tmp_SI_EmmPhysOfst;
// // static unsigned int mdl_DS_ORIG;
// // static unsigned int mdl_ES_ORIG;
// // static unsigned int mdl_SI_ORIG;
// // static unsigned int mdl_DI_ORIG;
// // static unsigned int mdl_DS_CURR;
// // static unsigned int mdl_ES_CURR;
// // static unsigned int mdl_SI_CURR;
// // static unsigned int mdl_DI_CURR;


/*
    Refactor:

    FLIC_EMM_MapNextPages_A()
        Pass By Register
            AX, BX, SI

*/

/*
FlicHdr_EmmHandleNumber
FlicHdr_EmmLogicalPageIndex
FlicHdr_EmmLogicalPageOffset
FlicHdr                         0x00
FlicHdr_Width                   0x00
FlicHdr_Height                  0x02
FlicHdr_CurrentFrame            0x04
FlicHdr_FrameCount              0x06
FlicHdr_LoopFrame               0x08
FlicHdr_EmmHandleNumber         0x0A
FlicHdr_EmmLogicalPageIndex     0x0B
FlicHdr_EmmLogicalPageOffset    0x0C
FlicHdr_PaletteDataOffset       0x0E
FlicHdr_FrameType               0x10

*/


// VGA_DrawBitmap
// VGA_DrawBitmap_R
// VGA_DrawEMSBitmap
// VGA_MapNextEMMPages
// FLIC_Draw_EMM_R
// FLIC_Draw
// FLIC_Draw_R
// FLIC_Draw_EMM
// FLIC_EMM_MapNextPages
// FLIC_Draw_EMM_R


// ; draws an LBX image into the current draw segment,
// ; column by column, decoding its RLE along the way

// _s27p01
void FLIC_Draw_A(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg)
{

    unsigned int tmp_AX;
    unsigned int tmp_DS;

// //asm push bp
// //asm mov  bp, sp
// asm push si
// asm push di
// asm push es
// asm push ds

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: FLIC_Draw_A(ScreenPage_X=%d, ScreenPage_Y=%d, FlicWidth=%d, Img_Off=0x%04X, Img_Seg=0x%04X)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, FlicWidth, Img_Off, Img_Seg);
#endif

asm mov dx, e_SC_INDEX
asm mov al, e_SC_MAPMASK
asm out dx, al
//    VGAMAPMASK()

asm mov ax, [ScreenPage_Y]
asm mov bx, ax
asm shl ax, 1
asm shl ax, 1
asm add ax, bx                              //; * 5 as a segment address = * 80 total bytes which,
                                            //; since each byte is 4 pixels, equals the draw row
asm add ax, [gsa_DSP_Addr]
asm mov es, ax

asm mov ax, [ScreenPage_X]
asm mov bx, ax
asm shr bx, 1
asm shr bx, 1
asm mov di, bx //; X / 4 = pixel offset in video memory
asm and ax, 3 //; X mod 4 = plane index of the pixel
asm mov si, offset g_VGA_WriteMapMasks3 //; should use dseg:41d0
asm add si, ax
asm lodsb
asm mov ah, al //; ah = map mask for the first pixel
asm mov si, [Img_Off]
asm mov cx, [FlicWidth]
asm mov bx, [Img_Seg]
asm mov ds, bx
asm mov bx, cx


LoopLines:

//     tmp_AX = _AX;
//     tmp_DS = _DS;
// asm mov ax, seg DGROUP
// asm mov ds, ax
// asm assume ds:DGROUP
// asm push ax
// asm push bx
// asm push cx
// asm push dx
// asm push ds
// asm push si
// asm push es
// asm push di
//     dlvfprintf("DEBUG: [%s, %d] AX: %04Xh %ud)\n", __FILE__, __LINE__, tmp_AX, tmp_AX);
//     dlvfprintf("DEBUG: [%s, %d] BX: %04Xh %ud)\n", __FILE__, __LINE__, _BX, _BX);
//     dlvfprintf("DEBUG: [%s, %d] CX: %04Xh %ud)\n", __FILE__, __LINE__, _CX, _CX);
//     dlvfprintf("DEBUG: [%s, %d] DX: %04Xh %ud)\n", __FILE__, __LINE__, _DX, _DX);
//     dlvfprintf("DEBUG: [%s, %d] DS: %04Xh %ud)\n", __FILE__, __LINE__, tmp_DS, tmp_DS);
//     dlvfprintf("DEBUG: [%s, %d] SI: %04Xh %ud)\n", __FILE__, __LINE__, _SI, _SI);
//     dlvfprintf("DEBUG: [%s, %d] ES: %04Xh %ud)\n", __FILE__, __LINE__, _ES, _ES);
//     dlvfprintf("DEBUG: [%s, %d] DI: %04Xh %ud)\n", __FILE__, __LINE__, _DI, _DI);
// asm pop di
// asm pop es
// asm pop si
// asm pop ds
// asm pop dx
// asm pop cx
// asm pop bx
// asm pop ax
// // AX = (ScreenPage_X & 3)
// // DS = BX = [Img_Seg]
//     _AX = tmp_AX;
//     _DS = tmp_DS;
// asm assume ds:nothing


asm push di
//  DIFF: FLIC_Draw_EMM
//      cmp  si, 0C000h
//      jb   short @@LoopLinesContinue
//      call FLIC_EMM_MapNextPages  
//      @@LoopLinesContinue:
FLIC_RLE()
//asm mov dx, e_SC_DATA // 0x03C5
//asm mov al, ah
//asm out dx, al
////VGAWRITEPLANE()
//
//asm lodsb
//asm cmp al, 0FFh
//asm jz SkipColumn
//
//asm mov dh, al
//asm lodsb
//asm mov dl, al
//asm cmp dh, 0
//asm js ULC_Run_Count
//
//ULC_Copy:
//asm mov cl, [si+1]
//asm xor ch, ch
//asm mov al, cl
//asm shl cx, 1
//asm shl cx, 1
//asm add cl, al
//asm adc ch, 0
//asm shl cx, 1
//asm shl cx, 1
//asm shl cx, 1
//asm shl cx, 1
//asm add di, cx
//asm lodsb
//asm mov cl, al
//asm xor ch, ch
//asm inc si
//asm dec dx
//asm dec dx
//asm sub dx, cx
//
//ULC_Copy_Do:
//asm movsb
//asm add di, 4Fh
//asm loop ULC_Copy_Do
//
//asm cmp dx, 1
//asm jns ULC_Copy
//
//SkipColumn:
//asm pop di
//asm dec bx
//asm jz Done
//
//asm shl ah, 1
//asm cmp ah, 9
//asm js EndOfColumn_YaySkip
//
//asm mov ah, 1
//asm inc di
//
//EndOfColumn_YaySkip:
//asm jmp LoopLines
//
//ULC_Run_Count:
//asm and dx, 7FFFh
//
//ULC_Run:
//asm mov cl, [si+1]
//asm xor ch, ch
//asm mov al, cl
//asm shl cx, 1
//asm shl cx, 1
//asm add cl, al
//asm adc ch, 0
//asm shl cx, 1
//asm shl cx, 1
//asm shl cx, 1
//asm shl cx, 1
//asm add di, cx
//asm lodsb
//asm mov cl, al
//asm xor ch, ch
//asm inc si
//asm dec dx
//asm dec dx
//asm sub dx, cx
//
//LoopNegRun:
//asm lodsb
//asm cmp al, 0E0h
//asm jnb PalIdxGTE224
//
//CopyByte:
//asm stosb
//asm add di, 4Fh
//asm loop LoopNegRun
//
//asm cmp dx, 1
//asm jns ULC_Run
//asm jmp SkipColumn
//
//PalIdxGTE224:
//asm and al, 1Fh
//asm inc al
//asm push cx
//asm mov cl, al
//asm xor ch, ch
//asm lodsb
//
//RepeatByte:
//asm stosb
//asm add di, 4Fh
//asm loop RepeatByte
//
//asm pop cx
//asm dec cx
//asm loop LoopNegRun
//
//asm cmp dx, 1
//asm jns ULC_Run
//
//asm pop di
//asm dec bx
//asm jz Done
//
//asm shl ah, 1
//asm cmp ah, 9
//asm js EndOfColumn_NaySkip
//
//asm mov ah, 1
//asm inc di
//
//EndOfColumn_NaySkip:
//asm jmp LoopLines
//
//Done:
asm mov ax, seg DGROUP
asm mov ds, ax
asm assume ds:DGROUP
#ifdef DEBUG
    // dlvfprintf("DEBUG: [%s, %d] END: FLIC_Draw_A(ScreenPage_X=%d, ScreenPage_Y=%d, FlicWidth=%d, Img_Off=0x%03X, Img_Seg=0x%04X)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, FlicWidth, Img_Off, Img_Seg);
    dlvfprintf("DEBUG: [%s, %d] END: FLIC_Draw_A()\n", __FILE__, __LINE__);
#endif

// asm pop ds
// asm pop es
// asm pop di
// asm pop si
// //asm pop bp
// //asm ret
// 
//     return;
}


// // ; draws an LBX image into the current draw segment,
// // ; column by column, decoding its RLE along the way,
// // ; and processing any shaded colors by reading the
// // ; current screen pixel and replacing with it from
// // ; the predefined Replacement_Colors@ block

// // _s27p02
// void FLIC_Draw_A_R(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg)
// {

// asm push bp
// asm mov bp, sp
// asm push si
// asm push di
// asm push es
// asm push ds

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] BEGIN: FLIC_Draw_A_R(ScreenPage_X=%d, ScreenPage_Y=%d, FlicWidth=%d, Img_Off=0x%03X, Img_Seg=0x%04X)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, FlicWidth, Img_Off, Img_Seg);
// #endif

// asm mov ax, [ScreenPage_Y]
// asm mov bx, ax
// asm shl ax, 1
// asm shl ax, 1
// asm add ax, bx //; * 5 as a segment address = * 80 total bytes which,
//     //; since each byte is 4 pixels, equals the draw row
// asm add ax, [gsa_DSP_Addr]
// asm mov es, ax
// asm mov ax, [ScreenPage_X]
// asm mov bx, ax
// asm shr bx, 1
// asm shr bx, 1
// asm mov di, bx //; X / 4 = pixel offset in video memory
//     //; X mod 4 = plane index of the pixel
// asm and ax, 3
// asm mov bl, al
// asm shl bl, 1
// asm shl bl, 1
// asm shl bl, 1
// asm shl bl, 1
// asm mov si, offset VGA_WriteMapMasks3 //; should use dseg:41d0
// asm add si, ax
// asm lodsb
// asm mov ah, al
// asm or ah, bl //; low nibble = map mask for the first pixel
//     //; high nibble = read map for the same
// asm mov si, [Img_Off]
// asm mov cx, [FlicWidth]
// asm mov bx, [Img_Seg]
// asm mov ds, bx
// asm mov bx, cx

// loc_1E8F3:
// asm push di
// asm mov dx, 3CEh
// asm mov al, 4
// asm out dx, al //; EGA: graph 1 and 2 addr reg:
//     //; read map select.
//     //; Data bits 0-2 select map # for read mode 00.
// asm inc dx
// asm mov al, ah
// asm shr al, 1
// asm shr al, 1
// asm shr al, 1
// asm shr al, 1 //; set the read plane for the pixel column
// asm out dx, al //; EGA port: graphics controller data register
//asm mov dx, e_SC_INDEX    //; 0x03C4
//asm mov al, e_SC_MAPMASK  //; 0x02
//asm out dx, al
//    VGAMAPMASK()

// asm inc dx
// asm mov al, ah
// asm and al, 0Fh //; set the write plane for the pixel column
// asm out dx, al //; EGA port: sequencer data register
// asm lodsb //; byte #1
// asm cmp al, 0FFh //; empty column (transparent)
// asm jz short loc_1E958
// asm mov dh, al
// asm lodsb //; byte #2
// asm mov dl, al //; dx = encoded column data length in bytes (15 bits,
//     //; the sign bit indicates whether there are repeats)
// asm cmp dh, 0
// asm jns short loc_1E924 //; chunk byte #2 (skip count)
// asm jmp loc_1E9B5

// loc_1E924: //; chunk byte #2 (skip count)
// asm mov cl, [si+1]
// asm xor ch, ch
// asm mov al, cl
// asm shl cx, 1
// asm shl cx, 1
// asm add cl, al
// asm adc ch, 0
// asm shl cx, 1
// asm shl cx, 1
// asm shl cx, 1
// asm shl cx, 1
// asm add di, cx
// asm lodsb //; chunk byte #1 (process count)
// asm mov cl, al
// asm xor ch, ch
// asm inc si
// asm dec dx
// asm dec dx
// asm sub dx, cx

// loc_1E948:
// asm lodsb
// asm cmp al, 0E8h
// asm jnb short loc_1E97E
// asm stosb
// asm add di, 4Fh //; same plane, one pixel down
// asm loop loc_1E948
// asm cmp dx, 1
// asm jns short loc_1E924 //; chunk byte #2 (skip count)

// loc_1E958:
// asm pop di
// asm dec bx
// asm jz short loc_1E978
// asm mov cl, ah
// asm and cl, 0F0h //; read plane
// asm add cl, 10h //; -> next
// asm and ah, 0Fh //; write plane
// asm shl ah, 1 //; -> next
// asm cmp ah, 9 //; wrapped around?
// asm js short loc_1E973
// asm mov cl, 0 //; reset the read map,
// asm mov ah, 1 //; reset the write mask,
// asm inc di //; and increase the memory offset

// loc_1E973:
// asm or ah, cl
// asm jmp loc_1E8F3

// loc_1E978:
// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] END: FLIC_Draw_A_R(ScreenPage_X=%d, ScreenPage_Y=%d, FlicWidth=%d, Img_Off=0x%03X, Img_Seg=0x%04X)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, FlicWidth, Img_Off, Img_Seg);
// #endif

// asm pop ds
// asm pop es
// asm pop di
// asm pop si
// asm pop bp
// asm ret

// loc_1E97E:
// asm sub al, 0E8h
// asm push si
// asm push ds
//     //;mov si, seg dseg
// asm mov si, seg DGROUP
// asm mov ds, si

// asm mov si, ax
// asm and si, 0FFh
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm add si, [gsa_ReplacementColors]         //; 180h LBX_Alloc_Space paragraphs
//                         //; each 256 byte (100h) block represents a different entry
//                         //; within a block, every byte contains a color index
//                         //; closest to the combination of the original color
//                         //; at that index, and the color and percent specified
//                         //; from the palette image when the block is filled out
//                         //; FLIC_Draw_R and other _R functions use these to
//                         //; replace bytes read from a source image file
// asm mov ds, si //; ds = paragraph of the block defined by (al-$E8)
// asm mov al, [di]
// asm mov si, ax
// asm and si, 0FFh
// asm lodsb
// asm pop ds
// asm pop si
// asm stosb
// asm add di, 4Fh //; same plane, one pixel down
// asm loop loc_1E948
// asm cmp dx, 1
// asm js short loc_1E958
// asm jmp loc_1E924 //; chunk byte #2 (skip count)

// loc_1E9B5:
// asm and dx, 7FFFh

// loc_1E9B9: //; chunk byte #2 (skip count)
// asm mov cl, [si+1]
// asm xor ch, ch
// asm mov al, cl
// asm shl cx, 1
// asm shl cx, 1
// asm add cl, al
// asm adc ch, 0
// asm shl cx, 1
// asm shl cx, 1
// asm shl cx, 1
// asm shl cx, 1
// asm add di, cx
// asm lodsb //; chunk byte #1 (process count)
// asm mov cl, al
// asm xor ch, ch
// asm inc si
// asm dec dx
// asm dec dx
// asm sub dx, cx

// loc_1E9DD:
// asm lodsb
// asm cmp al, 0E0h
// asm jnb short loc_1EA31
// asm cmp al, 0E8h
// asm jnb short loc_1E9FA
// asm stosb
// asm add di, 4Fh //; same plane, one pixel down
// asm loop loc_1E9DD
// asm cmp dx, 1
// asm jns short loc_1E9B9 //; chunk byte #2 (skip count)
// asm jmp loc_1E958
// asm pop ds
// asm pop es
// asm pop di
// asm pop si
// asm pop bp
// asm ret

// loc_1E9FA:
// asm sub al, 0E8h
// asm push si
// asm push ds
//     //;mov si, seg dseg
// asm mov si, seg DGROUP
// asm mov ds, si
// asm mov si, ax
// asm and si, 0FFh
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm add si, [gsa_ReplacementColors]         //; 180h LBX_Alloc_Space paragraphs
//                         //; each 256 byte (100h) block represents a different entry
//                         //; within a block, every byte contains a color index
//                         //; closest to the combination of the original color
//                         //; at that index, and the color and percent specified
//                         //; from the palette image when the block is filled out
//                         //; FLIC_Draw_R and other _R functions use these to
//                         //; replace bytes read from a source image file
// asm mov ds, si //; ds = paragraph of the block defined by (al-$E8)
// asm mov al, [di]
// asm mov si, ax
// asm and si, 0FFh
// asm lodsb
// asm pop ds
// asm pop si
// asm stosb
// asm add di, 4Fh //; same plane, one pixel down
// asm loop loc_1E9DD
// asm cmp dx, 1
// asm jns short loc_1E9B5
// asm jmp loc_1E958

// loc_1EA31:
// asm and al, 1Fh
// asm inc al
// asm push cx
// asm mov cl, al
// asm xor ch, ch
// asm lodsb
// asm cmp al, 0E8h
// asm jnb short loc_1EA54

// loc_1EA3F:
// asm stosb
// asm add di, 4Fh //; same plane, one pixel down
// asm loop loc_1EA3F
// asm pop cx
// asm dec cx
// asm loop loc_1E9DD
// asm cmp dx, 1
// asm jns short loc_1EA51
// asm jmp loc_1E958

// loc_1EA51: //; chunk byte #2 (skip count)
// asm jmp loc_1E9B9

// loc_1EA54:
// asm sub al, 0E8h
// asm push si
// asm push ds
//     //;mov si, seg dseg
// asm mov si, seg DGROUP
// asm mov ds, si
// asm mov si, ax
// asm and si, 0FFh
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm add si, [gsa_ReplacementColors]         //; 180h LBX_Alloc_Space paragraphs
//                         //; each 256 byte (100h) block represents a different entry
//                         //; within a block, every byte contains a color index
//                         //; closest to the combination of the original color
//                         //; at that index, and the color and percent specified
//                         //; from the palette image when the block is filled out
//                         //; FLIC_Draw_R and other _R functions use these to
//                         //; replace bytes read from a source image file
// asm mov ds, si //; ds = paragraph of the block defined by (al-$E8)

// loc_1EA71:
// asm mov al, [di]
// asm mov si, ax
// asm and si, 0FFh
// asm movsb
// asm add di, 4Fh //; same plane, one pixel down
// asm loop loc_1EA71
// asm pop ds
// asm pop si
// asm pop cx
// asm dec cx
// asm dec cx
// asm jz short loc_1EA8A
// asm jmp loc_1E9DD

// loc_1EA8A:
// asm cmp dx, 1
// asm jns short loc_1EA51
// asm jmp loc_1E958

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] END: FLIC_Draw_A_R(ScreenPage_X=%d, ScreenPage_Y=%d, FlicWidth=%d, Img_Off=0x%03X, Img_Seg=0x%04X)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, FlicWidth, Img_Off, Img_Seg);
// #endif

// asm pop ds
// asm pop es
// asm pop di
// asm pop si
// asm pop bp
// asm ret
// }


// ; draws an LBX image into the current draw segment,
// ; column by column, decoding its RLE along the way,
// ; and processing any shaded colors by reading the
// ; current screen pixel and replacing it from the
// ; predefined Replacement_Colors@ block
// ; maps the image into the EMS page frame, but will
// ; fail on images > 32k if they start 3 pages in

/*

    s01p03
    GAME_LoadMainImages()
        |-> gsa_MAINSCRN_0_AnimatedLogo = SAMB_data* LBXE_LoadSingle("MAINSCRN", 0)
            LBXE_LoadSingle() |-> LBX_Load_Entry() |-> EMM_LBX_Load_Entry() |-> EMM_LBX_HdrOnly()
                DEBUG: [_s10p10c.c, 173]: END: 
                    LBX_Load_Entry(LbxName = MAINSCRN, LbxEntry = 0, SAMB_head = 0x0000, LoadType = 0, FormatType = 0) 
                    { SAMB_data = 0x1ADA }

    s01p06c.c
    void SCREEN_Menu_Draw(void)
        |-> FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);  // NOTE(JimBalcomb): This is the first call to FLIC_Draw_XY()
    
    s28p11a.c
    void FLIC_Draw_XY(int Left, int Top, unsigned int FlicHdr_SgmtAddr)
        |-> FLIC_Draw_EMM_A2(tmp_SI, tmp_DI, FlicHdr_SgmtAddr, Frame_Index);
*/
/*
    Q: The parameter here absolutely has to be a 'SAMB_data' of 'FLIC_HEADER', correct?
    A:  

PS. "SAMB" is Space-Alloc Memory Block  (~SimTex Memory Manager)
*/

// _s27p03
void FLIC_Draw_EMM_A3(int ScreenPage_X, int ScreenPage_Y, unsigned int SAMB_data_FLIC_HDR, int Frame_Index)
{

    unsigned int SrcSgmt;
    unsigned int SrcOfst;
    unsigned int DstSgmt;
    unsigned int DstOfst;
    byte _FAR *fptr_Dst_Byte;
    byte _FAR *fptr_Src_Byte;
    word _FAR *fptr_Dst_Word;
    word _FAR *fptr_Src_Word;
    void _FAR *fptr_Src;
    word fh_Width;
    byte fh_EmmHandleNumber;
    byte fh_EmmLogicalPageIndex;
    word fh_EmmLogicalPageOffset;
    word fh_FrameDataOffset_LoWord;
    word fh_FrameDataOffset_HiWord;
    byte tmp_EmmLogicalPageIndex;
    word tmp_EmmLogicalPageOffset;
    word tmp_EmmFlicFrameDataOffset;
    byte fh_Shading;
    word cseg_FlicHdrWidth;
    byte cseg_EmmLogicalPageIndex;
    byte cseg_EmmHandleNumber;
    byte VgaWriteMapMask;
    byte _FAR * fptr_DrawScreenPageScanlineOffset;
    byte _FAR * fptr_EmmPageFrameBaseAddress;
    word rle_ColumnCount;
    // 1oom::lbxgfx.c::lbxgfx_draw_pixels_fmt0()  static void lbxgfx_draw_pixels_fmt0(uint8_t *pixbuf, uint16_t w, uint8_t *data, uint16_t pitch)
    // 1oom::lbxgfx.c::lbxgfx_draw_pixels_fmt0()  uint8_t *q;
    // 1oom::lbxgfx.c::lbxgfx_draw_pixels_fmt0()  uint8_t b, /*dh*/mode, /*dl*/len_total, len_run;
    byte *data;
    byte pitch = 80;  // ? pitch AKA stride ? address per row ? != bytes/pixels ? pixels/bytes per address ?
    byte *q;  // ? decoded buffer index ?
    byte rle_baito;  // ? encoded buffer index ?
    byte /*dh*/op_code_mode;
    byte /*dl*/len_total;
    byte len_run;
    unsigned int itr_Src_Byte;
    byte Src_Byte;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: FLIC_Draw_EMM_A3(ScreenPage_X=%d, ScreenPage_Y=%d, SAMB_data_FLIC_HDR=0x%04X, Frame_Index=%d)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, SAMB_data_FLIC_HDR, Frame_Index);
#endif

    SrcSgmt = SAMB_data_FLIC_HDR;
    DstSgmt = SAMB_data_FLIC_HDR;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] SrcSgmt: 0x%4X\n", __FILE__, __LINE__, SrcSgmt);
    dlvfprintf("DEBUG: [%s, %d] DstSgmt: 0x%4X\n", __FILE__, __LINE__, DstSgmt);
#endif

    /*
        Read EMM Handle, Logical Page, Offset
    */
    // // SrcOfst = FlicHdr_EmmHandleNumber
    // fh_EmmHandleNumber = (byte)MK_FP(SrcSgmt, FlicHdr_EmmHandleNumber);
    // fh_EmmLogicalPageIndex = (byte)MK_FP(SrcSgmt, FlicHdr_EmmLogicalPageIndex);
    // fh_EmmLogicalPageOffset = (word)MK_FP(SrcSgmt, FlicHdr_EmmLogicalPageOffset);
    fptr_Src_Byte = (byte _FAR *)MK_FP(SrcSgmt, 0);
    fptr_Src_Word = (word _FAR *)MK_FP(SrcSgmt, 0);
    fptr_Src = (void _FAR *)MK_FP(SrcSgmt, 0);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fptr_Src_Word: %Fp\n", __FILE__, __LINE__, fptr_Src_Word);
    dlvfprintf("DEBUG: [%s, %d] fptr_Src_Byte: %Fp\n", __FILE__, __LINE__, fptr_Src_Byte);
#endif

    //fh_Width = fptr_Src_Word[0];                  // FlicHdr_Width                MAINSCRN_LBX_000,0: 320
    //fh_Width = *(fptr_Src_Word+0);
    //fh_Width = *( (word const *) (fptr_Src_Byte+0x00) );
    //fh_Width = *( (word  far* )MK_FP( SAMB_data_FLIC_HDR, 0x00 ));
    fh_Width = FPEEKW(SAMB_data_FLIC_HDR, 0x00);
    cseg_FlicHdrWidth = fh_Width;
        // mov ax, [bp+SAMB_data_FLIC_HDR];  mov ds, ax;  mov ax, [ds:e_FlicHdr];  mov [cs:tmpFlicHdrWidth], ax
    //fh_EmmHandleNumber = fptr_Src_Byte[10];       // FlicHdr_EmmHandleNumber      MAINSCRN_LBX_000,0: 6
    //fh_EmmHandleNumber = *( (byte const *) (fptr_Src_Byte+0x10) );
    //fh_EmmHandleNumber = *( (byte const *) &(fptr_Src_Byte[10]) );
    //fh_EmmHandleNumber = *(fptr_Src_Byte+10);
    //fh_EmmHandleNumber = *((byte _FAR *)MK_FP(SAMB_data_FLIC_HDR, 10));
    //fh_EmmHandleNumber = *( (byte  far* )MK_FP( SAMB_data_FLIC_HDR, 0x0A ));
    fh_EmmHandleNumber = FPEEKB(SAMB_data_FLIC_HDR,0x0A);
    // mov  si, 0Ah  lodsb  mov dx, ax
    //fh_EmmLogicalPageIndex = fptr_Src_Byte[11];   // FlicHdr_EmmLogicalPageIndex  MAINSCRN_LBX_000,0: 0
    //fh_EmmLogicalPageIndex = *( (byte const *) (fptr_Src_Byte+0x11) );
    //fh_EmmHandleNumber = *( (byte const *) ( &( (fptr_Src_Byte)[11] ) ) );
    //fh_EmmLogicalPageIndex = *( (byte  far* )MK_FP( SAMB_data_FLIC_HDR, 0x0B ));
    fh_EmmLogicalPageIndex = FPEEKB(SAMB_data_FLIC_HDR,0x0B);
        // lodsb  mov bx, ax
    //fh_EmmLogicalPageOffset = fptr_Src_Word[6];   // FlicHdr_EmmLogicalPageOffset MAINSCRN_LBX_000,0: 0x02C0  C0 02 00 00
    //fh_EmmLogicalPageOffset = *( (word const *) (fptr_Src_Byte+0x12) );
    //fh_EmmHandleNumber = *( (word const *) ( &( (fptr_Src_Byte)[12] ) ) );
    //fh_EmmHandleNumber = *((word _FAR *)MK_FP(SAMB_data_FLIC_HDR, 12));
    //fh_EmmLogicalPageOffset = *( (word  far* )MK_FP( SAMB_data_FLIC_HDR, 0x0C ));
    fh_EmmLogicalPageOffset = FPEEKW(SAMB_data_FLIC_HDR,0x0C);
    //fh_EmmLogicalPageOffset = *(((word _FAR *)(fptr_Src))+0x0C);
        // HERE: lodsw;  mov si, ax;  BELOW: mov di, si
    tmp_EmmLogicalPageOffset = fh_EmmLogicalPageOffset;
        // ABOVE: lodsw;  mov si, ax;  HERE: mov di, si

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fh_Width: 0x%04X (%uu) (%dd)\n", __FILE__, __LINE__, fh_Width, fh_Width, fh_Width);
    dlvfprintf("DEBUG: [%s, %d] fh_EmmHandleNumber: 0x%02X (%u) (%d) (%ud)\n", __FILE__, __LINE__, fh_EmmHandleNumber, fh_EmmHandleNumber, fh_EmmHandleNumber, fh_EmmHandleNumber);
    dlvfprintf("DEBUG: [%s, %d] fh_EmmLogicalPageIndex: 0x%02X (%u) (%d) (%ud)\n", __FILE__, __LINE__, fh_EmmLogicalPageIndex, fh_EmmLogicalPageIndex, fh_EmmLogicalPageIndex, fh_EmmLogicalPageIndex);
    dlvfprintf("DEBUG: [%s, %d] fh_EmmLogicalPageOffset: 0x%04X (%u) (%d) (%ud)\n", __FILE__, __LINE__, fh_EmmLogicalPageOffset, fh_EmmLogicalPageOffset, fh_EmmLogicalPageOffset, fh_EmmLogicalPageOffset);
#endif

    EMM_MapMulti4(fh_EmmLogicalPageIndex, fh_EmmHandleNumber);
    // TODO(JimBalcomb): add test for result of EMM_MapMulti4()
    /*
        ~=
        g_EMM_Log2Phys_Map[0] = fh_EmmLogicalPageIndex
        g_EMM_Log2Phys_Map[1] = fh_EmmLogicalPageIndex + 1
        g_EMM_Log2Phys_Map[2] = fh_EmmLogicalPageIndex + 2
        g_EMM_Log2Phys_Map[3] = fh_EmmLogicalPageIndex + 3
    */

    /*
        set DS:SI to get FLIC Frame Offset, low-word and high-word
    */
    /*
        1oom: lbxgfx.h    #define lbxgfx_get_offs0c(_data_) GET_LE_16( &((_data_)[0x0c] )
        1oom: bits.h    #define GET_LE_16(_p_) ( *((uint16_t const *)(_p_) )
    */
    SrcSgmt = EMM_PageFrame_Base_Address;
    // // SrcOfst = FlicHdr_FrameOffsetTable + (4 * Frame_Index);
    // SrcOfst = 8 + (4 * Frame_Index);
        // SrcOfst = FLIC_HDR.EmmLogicalPageOffset; SrcOfst = SrcOfst + (4 * Frame_Index); SrcOfst = SrcOfst + ofsFlicFrameOffsetTable;
        // AKA SrcOfst = FLIC_HDR.EmmLogicalPageOffset + (4 * Frame_Index) + ofsFlicFrameOffsetTable;
    SrcOfst = fh_EmmLogicalPageOffset + (4 * Frame_Index) + FlicHdr_FrameOffsetTable;  // ? 0x02d2 ? offset in LBX File in EMM to FLIC File Frame Offset Table for Frame_Index?
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] SrcSgmt: 0x%04X\n", __FILE__, __LINE__, SrcSgmt);
    dlvfprintf("DEBUG: [%s, %d] SrcOfst: 0x%04X\n", __FILE__, __LINE__, SrcOfst);
#endif
    fptr_Src_Word = (word _FAR *)MK_FP(SrcSgmt, SrcOfst);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fptr_Src_Word: %Fp\n", __FILE__, __LINE__, fptr_Src_Word);
#endif
    // Ref:  J:\STU\devel\STU-MoM_Rasm\MoM_Rasm\_s21p07c.c
    // // ffh_image_offset = (unsigned int)*SrcSgmt[SrcOfst++];
    // // ffh_color_count = (unsigned char)*SrcSgmt[SrcOfst++];
    // // ffh_first_color = (unsigned char)*SrcSgmt[SrcOfst++];
    // fptr_Src = (unsigned int _FAR *)MK_FP(SrcSgmt, SrcOfst);
    // ffh_image_offset = fptr_Src[0];
    // ffh_color_count = fptr_Src[1];
    // ffh_first_color = fptr_Src[2];
    // Ref:  https://atrevida.comprenica.com/atrtut10.html
    //      unsigned char *pointer_within_sprite = sprite + 4;
    //      unsigned char far *video_mem_pointer = ptr_to_video_segment + (y << 8) + (y << 6) + x;
    //          /* Plot pixel: */
    //          *(video_mem_pointer++) = *(pointer_within_sprite++);
    //      video_mem_pointer += x_incr;  /* Jump down to next row, first column */

    // fh_FrameDataOffset_Loword = (word)MK_FP(SrcSgmt, SrcOfst);
    // fh_FrameDataOffset_Loword = (word)MK_FP(SrcSgmt, SrcOfst+2);
    fh_FrameDataOffset_LoWord = fptr_Src_Word[0];
    fh_FrameDataOffset_HiWord = fptr_Src_Word[1];
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fh_FrameDataOffset_LoWord: 0x%04X\n", __FILE__, __LINE__, fh_FrameDataOffset_LoWord);  // MAINSCRN_LBX_000,0: 6E 03
    dlvfprintf("DEBUG: [%s, %d] fh_FrameDataOffset_HiWord: 0x%04X\n", __FILE__, __LINE__, fh_FrameDataOffset_HiWord);  // MAINSCRN_LBX_000,0: 00 00
#endif

/*
    Move the bottom four bits of DX into the top four bits of AX, while discarding the bottom four bits of AX
    Somehow, this /calculates/ the EMM Logical Page Index and EMM Logical Page Offset
*/
    _ES = SAMB_data_FLIC_HDR;
    _DX = fh_FrameDataOffset_LoWord;
    _AX = fh_FrameDataOffset_HiWord;
asm     inc  dx
asm     adc  ax, 0
asm     shl  dx, 1
asm     rcl  ax, 1
asm     shl  dx, 1
asm     rcl  ax, 1
asm     shr  dx, 1
asm     shr  dx, 1
asm     add  al, [es:FlicHdr_EmmLogicalPageIndex]     // DstSgmt = SAMB_data_FLIC_HDR;
asm     adc  ah, 0
asm     add  dx, [es:FlicHdr_EmmLogicalPageOffset]    // DstSgmt = SAMB_data_FLIC_HDR;
    tmp_EmmLogicalPageIndex = _AX;
    // SrcOfst = _DX;  // Offset in EMM to FLIC Frame Data
    tmp_EmmFlicFrameDataOffset = _DX;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] cseg_EmmLogicalPageIndex: 0x%04X\n", __FILE__, __LINE__, cseg_EmmLogicalPageIndex);
    dlvfprintf("DEBUG: [%s, %d] tmp_EmmFlicFrameDataOffset: 0x%04X\n", __FILE__, __LINE__, tmp_EmmFlicFrameDataOffset);
#endif
//   cmp  dx, 0C000h
//   jb   short @@EmsMapMem
//   sub  dx, 0C000h
//   inc  ax
//   inc  ax
//   inc  ax
//@@EmsMapMem:
//   mov  [cs:cseg_EmmLgclPg], ax
//   mov  si, dx                          ; SI = Offset in EMM
    //if ( tmp_EmmFlicFrameDataOffset > (3 * EMM_PAGE_SIZE) )  // Constant out of range
    if ( tmp_EmmFlicFrameDataOffset > 0xC000 )
    {
        tmp_EmmFlicFrameDataOffset = tmp_EmmFlicFrameDataOffset - 0xC000;  // (3 * EMM_PAGE_SIZE)
        tmp_EmmLogicalPageIndex = tmp_EmmLogicalPageIndex - 3;
    }
    cseg_EmmLogicalPageIndex = tmp_EmmLogicalPageIndex;


/*
    1oom: lbxgfx.h    #define lbxgfx_get_format(_data_) ((_data_)[0x11])
*/
/*
    MAINSCRN_LBX_000, gsa_MAINSCRN_0_AnimatedLogo
    SrcSgmt: 0xE000, SrcOfst: 0x02D1, fptr_Src_Byte: E000:02D1
*/
    SrcOfst = tmp_EmmLogicalPageOffset + FlicHdr_Shading;
        // mov bx, e_FlicHdr_Shading;  add bx, di;  mov bl, [bx];  xor bh, bh;  mov di, bx;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] SrcSgmt: 0x%04X\n", __FILE__, __LINE__, SrcSgmt);
    dlvfprintf("DEBUG: [%s, %d] SrcOfst: 0x%04X\n", __FILE__, __LINE__, SrcOfst);
#endif
    fptr_Src_Byte = (byte _FAR *)MK_FP(SrcSgmt, SrcOfst);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fptr_Src_Byte: %Fp\n", __FILE__, __LINE__, fptr_Src_Byte);
#endif

    fh_Shading = fptr_Src_Byte[0];
        // mov bx, e_FlicHdr_Shading;  add bx, di;  mov bl, [bx];  xor bh, bh;  mov di, bx;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fh_Shading: 0x%02X (%uu) (%dd) (%huhu) (%hdhd) (%cc)\n", __FILE__, __LINE__, fh_Shading, fh_Shading, fh_Shading, fh_Shading, fh_Shading, fh_Shading);
#endif

//    for (itr_Src_Byte = 0; itr_Src_Byte < 96; itr_Src_Byte++)
//    {
//        Src_Byte = fptr_Src_Byte[itr_Src_Byte];
//        dlvfprintf("DEBUG: [%s, %d] Src_Byte[%u]: 0x%02X (%uu) (%dd) (%huhu) (%hdhd) (%cc)\n", __FILE__, __LINE__, itr_Src_Byte, Src_Byte, Src_Byte, Src_Byte, Src_Byte, Src_Byte, Src_Byte);
//    }


// mov  bx, ax                          ; BX = EMM Logical Page Index

    cseg_EmmHandleNumber = fh_EmmHandleNumber;
        // mov dl, [es:FLIC_HDR.EMM_Handle_Number];  xor dh, dh;  mov [cs:cseg_EmmHndl], dx;
dlvfprintf("DEBUG: [%s, %d] cseg_EmmHandleNumber: 0x%02X\n", __FILE__, __LINE__, cseg_EmmHandleNumber);
dlvfprintf("DEBUG: [%s, %d] cseg_EmmLogicalPageIndex: 0x%02X\n", __FILE__, __LINE__, cseg_EmmLogicalPageIndex);

    _DX = cseg_EmmHandleNumber;
    _BX = cseg_EmmLogicalPageIndex;
    _AL = 0x00;
    _AH = 0x44;
    geninterrupt(0x67);
        // push bx; push dx; mov ax, 4400h;  int 67h;
    _DX = cseg_EmmHandleNumber;
    _BX = cseg_EmmLogicalPageIndex + 1;
    _AL = 0x01;
    _AH = 0x44;
    geninterrupt(0x67);
        // mov ax, 4401h;  pop dx;  pop bx;  inc bx;  int 67h;

    if ( !(fh_Shading == 0) )
    {
        // FLIC_Draw_EMM_R(ScreenPage_X, ScreenPage_Y, SAMB_data_FLIC_HDR);
    }
        // cmp di, 0;  jz @@NayShading
    else
    {
        /* ==================================================================================================================================================================
            ╔═══════╦═════════════════════╗
            ║ BEGIN ║  De-RLE, Write VRAM ║
            ╠═══════╬═════════════════════╣
            ║       ║                     ║
            ╚═══════╩═════════════════════╝
            ╔═══════╦═════════════════════╗
            ║ BEGIN ║  De-RLE, Write VRAM ║
            ║═══════╩═════════════════════╩════════════════╗
            ║                                              ║
            ║                                              ║
            ║                                              ║
            ╚══════════════════════════════════════════════╝
        ================================================================================================================================================================== */
        /* ╔══════════════════════════════════════════════════════════════════╗
        ╔══╝                                                                  ║
        ║                                                                     ║
        ║                                                                     ║
        ║                                                                  ╔══╝
        ╚══════════════════════════════════════════════════════════════════╝ */

        /* ╔══════════════════════════════════════════════════════════════════╗
        ╔══╝                                                                  ║
        ╠═════════════════════════════════════════════════════════════════════╣
        ║                                                                     ║
        ║                                                                  ╔══╝
        ╚══════════════════════════════════════════════════════════════════╝ */

        // TODO(JimBalcomb): figure out if DS being reset here is indicative of a code boundary - macro, jump into proc, C|Asm
            // @@NayShading:
            // mov     ax, seg dseg
            // mov     ds, ax

        outportb(e_SC_INDEX, e_SC_MAPMASK);
            // mov dx, e_SC_INDEX;  mov al, e_SC_MAPMASK;  out dx, al;

        DstSgmt = gsa_DSP_Addr + ( (ScreenPage_Y * 4) + ScreenPage_Y);
        // mov ax, [bp+ScreenPage_Y];  mov bx, ax;  shl ax, 1;  shl ax, 1;  add ax, bx;  add ax, [gsa_DSP_Addr];  mov es, ax;
        DstOfst = (ScreenPage_X >> 2);  // ~== math.floor(ScreenPage_X / 4)
            // mov ax, [bp+Left];  mov bx, ax;  shr bx, 1;  shr bx, 1;  mov di, bx
        VgaWriteMapMask = g_VGA_WriteMapMasks3[(ScreenPage_X & 0x03)];  // ~== x modulo 4  (x % 4, x|4)
            // and ax, 3;  mov bx, offset VGA_WriteMapMasks3;  add bx, ax; mov ah, [bx]
        SrcSgmt = EMM_PageFrame_Base_Address;
            // mov bx, [EMM_PageFrame_Base_Address];  mov ds, bx;
        SrcOfst = tmp_EmmFlicFrameDataOffset;

        // fptr_DrawScreenPageScanlineOffset = (gsa_DSP_Addr << 16) + ( ScreenPage_Y * (((320/4)/16)) ); // _ES = {A000,A400} + (Y*(((320/4)/16))
        fptr_DrawScreenPageScanlineOffset = (gsa_DSP_Addr * 65536) + ( ScreenPage_Y * (((320/4)/16)) );
        // fptr_EmmPageFrameBaseAddress = (EMM_PageFrame_Base_Address << 16);
        fptr_EmmPageFrameBaseAddress = (EMM_PageFrame_Base_Address * 65536);

        #ifdef DEBUG
            dlvfprintf("DEBUG: [%s, %d] fptr_DrawScreenPageScanlineOffset: %Fp\n", __FILE__, __LINE__, fptr_DrawScreenPageScanlineOffset);
            dlvfprintf("DEBUG: [%s, %d] fptr_EmmPageFrameBaseAddress: %Fp\n", __FILE__, __LINE__, fptr_EmmPageFrameBaseAddress);
        #endif
        
        /*
            Column Count / Line Count
        */
        rle_ColumnCount = cseg_FlicHdrWidth;
            // mov  bx, [cs:tmpFlicHdrWidth]

        #ifdef DEBUG
            dlvfprintf("DEBUG: [%s, %d] SrcSgmt: 0x%04X\n", __FILE__, __LINE__, SrcSgmt);
            dlvfprintf("DEBUG: [%s, %d] SrcOfst: 0x%04X\n", __FILE__, __LINE__, SrcOfst);
            dlvfprintf("DEBUG: [%s, %d] DstSgmt: 0x%04X\n", __FILE__, __LINE__, DstSgmt);
            dlvfprintf("DEBUG: [%s, %d] DstOfst: 0x%04X\n", __FILE__, __LINE__, DstOfst);
        #endif
        fptr_Src_Byte = (byte _FAR *)MK_FP(SrcSgmt, SrcOfst);
        fptr_Dst_Byte = (byte _FAR *)MK_FP(DstSgmt, DstOfst);
        #ifdef DEBUG
            dlvfprintf("DEBUG: [%s, %d] fptr_Src_Byte: %Fp\n", __FILE__, __LINE__, fptr_Src_Byte);
            dlvfprintf("DEBUG: [%s, %d] fptr_Dst_Byte: %Fp\n", __FILE__, __LINE__, fptr_Dst_Byte);
        #endif

            // @@LoopLines:
            //     push di
            //     cmp  si, 0C000h
            //     jb   short @@LoopLinesContinue
            //     call FLIC_EMM_MapNextPages   
            // @@LoopLinesContinue:

        //
        // 
        //
        while (rle_ColumnCount--)
        {
            #ifdef DEBUG
                dlvfprintf("DEBUG: [%s, %d] rle_ColumnCount: 0x%02X (%uu) (%dd) (%huhu) (%hdhd) (%cc)\n", __FILE__, __LINE__, rle_ColumnCount, rle_ColumnCount, rle_ColumnCount, rle_ColumnCount, rle_ColumnCount, rle_ColumnCount);
            #endif

        #ifdef DEBUG
            dlvfprintf("DEBUG: [%s, %d] DstOfst: 0x%04X\n", __FILE__, __LINE__, DstOfst);
            dlvfprintf("DEBUG: [%s, %d] VgaWriteMapMask: 0x%02X\n", __FILE__, __LINE__, VgaWriteMapMask);
        #endif
            outportb(e_SC_INDEX, VgaWriteMapMask);  // set the write plane for the pixel column
                // mov dx, e_SC_DATA;  mov al, ah;  out dx, al

            // 1oom::lbxgfx.c::lbxgfx_draw_pixels_fmt0()  q = pixbuf++;
            // 1oom::lbxgfx.c::lbxgfx_draw_pixels_fmt0()  b = *data++;
            q = fptr_Dst_Byte++;
            rle_baito = *fptr_Src_Byte++;
            #ifdef DEBUG
                dlvfprintf("DEBUG: [%s, %d] rle_baito: 0x%02X (%uu) (%dd) (%huhu) (%hdhd) (%cc)\n", __FILE__, __LINE__, rle_baito, rle_baito, rle_baito, rle_baito, rle_baito, rle_baito);
                dlvfprintf("DEBUG: [%s, %d] fptr_Src_Byte: %Fp\n", __FILE__, __LINE__, fptr_Src_Byte);
                dlvfprintf("DEBUG: [%s, %d] fptr_Dst_Byte: %Fp\n", __FILE__, __LINE__, fptr_Dst_Byte);
            #endif

            if (rle_baito == 0xFF) { /* skip column */
                goto NextColumn;
            }
            op_code_mode = rle_baito;
            len_total = *data++;
            if ((op_code_mode & 0x80) == 0) { /* regular data */
                do {
                    len_run = *data++;
                    q += pitch * *data++;
                    len_total -= len_run + 2;
                    do {
                        *q = *data++;
                        q += pitch;
                    } while (--len_run);
                } while (len_total >= 1);
            } else {    /* compressed data */
                do {
                    len_run = *data++;
                    q += pitch * *data++;
                    len_total -= len_run + 2;
                    do {
                        rle_baito = *data++;
                        if (rle_baito >= 224) { /* b-224 pixels, same color */
                            byte len_compr;
                            len_compr = rle_baito - 224;
                            --len_run;
                            rle_baito = *data++;
                            while (len_compr) {
                                *q = rle_baito;
                                q += pitch;
                                --len_compr;
                            }
                        } else {
                            *q = rle_baito;
                            q += pitch;
                        }
                    } while (--len_run);
                } while (len_total >= 1);
            }

            NextColumn:
            VgaWriteMapMask = VgaWriteMapMask * 2;  // {1,2,4,8} * 2 = {2,4,8,16}
            #ifdef DEBUG
                dlvfprintf("DEBUG: [%s, %d] VgaWriteMapMask: 0x%02X\n", __FILE__, __LINE__, VgaWriteMapMask);
            #endif
            if (VgaWriteMapMask >= 9)
            {
                VgaWriteMapMask = 1;
                DstOfst++;
            }

        }
    }



#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: FLIC_Draw_EMM_A3(ScreenPage_X=%d, ScreenPage_Y=%d, SAMB_data_FLIC_HDR=0x%03X, Frame_Index=%d)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, SAMB_data_FLIC_HDR, Frame_Index);
#endif

}



// // ; changes the EMM page mapping such that the previous
// // ; last page now becomes the first one
// // ; preserves ax and bx, reduces si by $C000 (3 pages)
// 
// // _s27p04
// void FLIC_EMM_MapNextPages_A(void)
// {
//     cseg_Flic_EmmLogicalPageIndex += 3;
//     EMM_MapMulti4(cseg_Flic_EmmLogicalPageIndex, cseg_Flic_EmmHandleNumber);  // ST_EMM.H  _s12p12c.c
//     cseg_Flic_EmmLogicalPageOffset -= 0xC000;  //; 3 * sizeof(EMM Logical Page)  3*16KB = 48KB
// }
// // _S27A.ASM
// // assume	cs:_S27A_TEXT
// // _FLIC_EMM_MapNextPages_A	proc	far
// //     push bp
// //     mov  bp,sp
// //     add  word ptr DGROUP:cseg_Flic_EmmLogicalPageIndex,3
// //     push word ptr DGROUP:cseg_Flic_EmmHandleNumber
// //     push word ptr DGROUP:cseg_Flic_EmmLogicalPageIndex
// //     call far ptr _EMM_MapMulti4
// //     pop  cx
// //     pop  cx
// //     sub  word ptr DGROUP:cseg_Flic_EmmLogicalPageOffset,49152
// //     pop  bp
// //     ret	
// // _FLIC_EMM_MapNextPages_A	endp


// // ; draws an LBX image into the current draw segment,
// // ; column by column, decoding its RLE along the way,
// // ; and processing any shaded colors by reading the
// // ; current screen pixel and replacing with it from
// // ; the predefined Replacement_Colors@ block
// // ;
// // ; the image must already be mapped into the EMS page
// // ; frame, and si needs to point to the image data

// // _s27p05
// void FLIC_Draw_EMM_R_A(int ScreenPage_X, int ScreenPage_Y, unsigned int Img_Seg)
// {

// asm push bp
// asm mov  bp, sp

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] BEGIN: FLIC_Draw_EMM_R_A(ScreenPage_X=%d, ScreenPage_Y=%d, Img_Seg=0x%03X)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, Img_Seg);
// #endif

//     //;mov ax, seg dseg
// asm mov ax, seg DGROUP
// asm mov ds, ax

// asm mov ax, [ScreenPage_Y]
// asm mov bx, ax
// asm shl ax, 1
// asm shl ax, 1
// asm add ax, bx //; * 5 as a segment address = * 80 total bytes which,
//     //; since each byte is 4 pixels, equals the draw row
// asm add ax, [gsa_DSP_Addr]
// asm mov es, ax
// asm mov ax, [ScreenPage_X]
// asm mov bx, ax
// asm shr bx, 1
// asm shr bx, 1
// asm mov di, bx //; X / 4 = pixel offset in video memory
//     //; X mod 4 = plane index of the pixel
// asm and ax, 3
// asm mov cl, al
// asm shl cl, 1
// asm shl cl, 1
// asm shl cl, 1
// asm shl cl, 1
// asm mov bx, offset VGA_WriteMapMasks3 //; should use dseg:41d0
// asm add bx, ax
// asm mov al, [bx]
// asm mov ah, al
// asm or ah, cl //; low nibble = map mask for the first pixel
//     //; high nibble = read map for the same
// asm mov bx, [EMM_PageFrame_Base_Address] //; contains the segment address of the EMS page frame
// asm mov ds, bx
// asm mov bx, [tmpFlicHdrWidth]
// Loop48KB:
// asm push di
// asm cmp si, 0C000h
// asm jb short loc_1ECD9
// asm call FLIC_EMM_MapNextPages_A //; changes the EMM page mapping such that the previous
//     //; last page now becomes the first one
//     //; preserves ax and bx, reduces si by $C000 (3 pages)
// loc_1ECD9:
// asm mov dx, e_GC_INDEX
// asm mov al, e_GC_READMAP
// asm out dx, al //; EGA: graph 1 and 2 addr reg:
//     //; read map select.
//     //; Data bits 0-2 select map # for read mode 00.

// asm inc dx //; _DX = GC_DATA
// asm mov al, ah
// asm shr al, 1
// asm shr al, 1
// asm shr al, 1
// asm shr al, 1 //; set the read plane for the pixel column
// asm out dx, al //; EGA port: graphics controller data register

//asm mov dx, e_SC_INDEX    //; 0x03C4
//asm mov al, e_SC_MAPMASK  //; 0x02
//asm out dx, al
//    VGAMAPMASK()


// asm inc dx //; _DX = SC_DATA
// asm mov al, ah
// asm and al, 0Fh //; set the write plane for the pixel column
// asm out dx, al //; EGA port: sequencer data register

// asm lodsb //; byte #1
// asm cmp al, 0FFh //; empty column (transparent)
// asm jz short loc_1ED3D

// asm mov dh, al
// asm lodsb //; byte #2
// asm mov dl, al                              //; dx = encoded column data length in bytes (15 bits,
//                                             //; the sign bit indicates whether there are repeats)
// asm cmp dh, 0
// asm jns short loc_1ED09 //; chunk byte #2 (skip count)
// asm jmp loc_1ED96

// loc_1ED09: //; chunk byte #2 (skip count)
// asm mov cl, [si+1]
// asm xor ch, ch
// asm mov al, cl
// asm shl cx, 1
// asm shl cx, 1
// asm add cl, al
// asm adc ch, 0
// asm shl cx, 1
// asm shl cx, 1
// asm shl cx, 1
// asm shl cx, 1
// asm add di, cx
// asm lodsb //; chunk byte #1 (process count)
// asm mov cl, al
// asm xor ch, ch
// asm inc si
// asm dec dx
// asm dec dx
// asm sub dx, cx
// loc_1ED2D:
// asm lodsb
// asm cmp al, 0E8h
// asm jnb short loc_1ED5F
// asm stosb
// asm add di, 4Fh //; same plane, one pixel down
// asm loop loc_1ED2D
// asm cmp dx, 1
// asm jns short loc_1ED09 //; chunk byte #2 (skip count)
// loc_1ED3D:
// asm pop di
// asm dec bx
// asm jz short loc_1ED5D
// asm mov cl, ah
// asm and cl, 0F0h //; read plane
// asm add cl, 10h //; -> next
// asm and ah, 0Fh //; write plane
// asm shl ah, 1 //; -> next
// asm cmp ah, 9 //; wrapped around?
// asm js short loc_1ED58
// asm mov cl, 0 //; reset the read map,
// asm mov ah, 1 //; reset the write mask,
// asm inc di //; and increase the memory offset
// loc_1ED58:
// asm or ah, cl
// asm jmp Loop48KB
// loc_1ED5D:
// asm pop bp
// asm ret

// loc_1ED5F:
// asm sub al, 0E8h
// asm push si
// asm push ds
//     //;mov si, seg dseg
// asm mov si, seg DGROUP
// asm mov ds, si
// asm mov si, ax
// asm and si, 0FFh
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm add si, [gsa_ReplacementColors] //; 180h LBX_Alloc_Space paragraphs
//     //; each 256 byte (100h) block represents a different entry
//     //;
//     //; within a block, every byte contains a color index
//     //; closest to the combination of the original color
//     //; at that index, and the color and percent specified
//     //; from the palette image when the block is filled out
//     //;
//     //; FLIC_Draw_R and other _R functions use these to
//     //; replace bytes read from a source image file
// asm mov ds, si //; ds = paragraph of the block defined by (al-$E8)
// asm mov al, [di]
// asm mov si, ax
// asm and si, 0FFh
// asm lodsb
// asm pop ds
// asm pop si
// asm stosb
// asm add di, 4Fh //; 'O' //; same plane, one pixel down
// asm loop loc_1ED2D
// asm cmp dx, 1
// asm js short loc_1ED3D
// asm jmp loc_1ED09 //; chunk byte #2 (skip count)
// loc_1ED96:
// asm and dx, 7FFFh
// loc_1ED9A: //; chunk byte #2 (skip count)
// asm mov cl, [si+1]
// asm xor ch, ch
// asm mov al, cl
// asm shl cx, 1
// asm shl cx, 1
// asm add cl, al
// asm adc ch, 0
// asm shl cx, 1
// asm shl cx, 1
// asm shl cx, 1
// asm shl cx, 1
// asm add di, cx
// asm lodsb //; chunk byte #1 (process count)
// asm mov cl, al
// asm xor ch, ch
// asm inc si
// asm dec dx
// asm dec dx
// asm sub dx, cx
// loc_1EDBE:
// asm lodsb
// asm cmp al, 0E0h
// asm jnb short loc_1EE0E
// asm cmp al, 0E8h
// asm jnb short loc_1EDD7
// asm stosb
// asm add di, 4Fh //; same plane, one pixel down
// asm loop loc_1EDBE
// asm cmp dx, 1
// asm jns short loc_1ED9A //; chunk byte #2 (skip count)
// asm jmp loc_1ED3D
// asm pop bp
// asm ret

// loc_1EDD7:
// asm sub al, 0E8h
// asm push si
// asm push ds
//     //;mov si, seg dseg
// asm mov si, seg DGROUP
// asm mov ds, si

// asm mov si, ax
// asm and si, 0FFh
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm add si, [gsa_ReplacementColors] //; 180h LBX_Alloc_Space paragraphs
//     //; each 256 byte (100h) block represents a different entry
//     //;
//     //; within a block, every byte contains a color index
//     //; closest to the combination of the original color
//     //; at that index, and the color and percent specified
//     //; from the palette image when the block is filled out
//     //;
//     //; FLIC_Draw_R and other _R functions use these to
//     //; replace bytes read from a source image file

// asm mov ds, si //; ds = paragraph of the block defined by (al-$E8)
// asm mov al, [di]
// asm mov si, ax
// asm and si, 0FFh
// asm lodsb
// asm pop ds
// asm pop si
// asm stosb
// asm add di, 4Fh //; same plane, one pixel down
// asm loop loc_1EDBE
// asm cmp dx, 1
// asm jns short loc_1ED96
// asm jmp loc_1ED3D
// loc_1EE0E:
// asm and al, 1Fh
// asm inc al
// asm push cx
// asm mov cl, al
// asm xor ch, ch
// asm lodsb
// asm cmp al, 0E8h
// asm jnb short loc_1EE31
// LoopColumn:
// asm stosb
// asm add di, 4Fh //; same plane, one pixel down
// asm loop LoopColumn
// asm pop cx
// asm dec cx
// asm loop loc_1EDBE
// asm cmp dx, 1
// asm jns short loc_1EE2E
// asm jmp loc_1ED3D
// loc_1EE2E: //; chunk byte #2 (skip count)
// asm jmp loc_1ED9A
// loc_1EE31:
// asm sub al, 0E8h
// asm push si
// asm push ds
//     //;mov si, seg dseg
// asm mov si, seg DGROUP
// asm mov ds, si
// asm mov si, ax
// asm and si, 0FFh
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm shl si, 1
// asm add si, [gsa_ReplacementColors] //; 180h LBX_Alloc_Space paragraphs
//     //; each 256 byte (100h) block represents a different entry
//     //;
//     //; within a block, every byte contains a color index
//     //; closest to the combination of the original color
//     //; at that index, and the color and percent specified
//     //; from the palette image when the block is filled out
//     //;
//     //; FLIC_Draw_R and other _R functions use these to
//     //; replace bytes read from a source image file
// asm mov ds, si //; ds = paragraph of the block defined by (al-$E8)
// loc_1EE4E:
// asm mov al, [di]
// asm mov si, ax
// asm and si, 0FFh
// asm movsb
// asm add di, 4Fh //; same plane, one pixel down
// asm loop loc_1EE4E

// asm pop ds
// asm pop si
// asm pop cx
// asm dec cx
// asm dec cx
// asm jz short loc_1EE67
// loc_1EE64:
// asm jmp loc_1EDBE
// loc_1EE67:
// asm cmp dx, 1
// loc_1EE6A:
// asm jns short loc_1EE2E
// loc_1EE6C:
// asm jmp loc_1ED3D

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] END: FLIC_Draw_EMM_R_A(ScreenPage_X=%d, ScreenPage_Y=%d, Img_Seg=0x%03X)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, Img_Seg);
// #endif

// asm pop bp
// asm ret
// }
