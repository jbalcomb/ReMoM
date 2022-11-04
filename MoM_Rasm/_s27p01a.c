// TITLE _s27a.c FLIC_Draw_...
// ST_FLIC.H

#include "ST_DEF.H"

#include "ST_EMM.H"
#include "ST_FLIC.H"
#include "ST_VGA.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

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

// s27p01
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
