// _s28p02a.c FLIC_Prepare
// ST_FLIC.H

#include "ST_TYPE.H"
#include "ST_DEF.H"
#include "ST_FLIC.H"
#include "ST_SA.H"  /* SAMB_addr; farpokew(); */


/*
    XREF:
        GAME_PrepareCredits()
        GAME_DrawCredits()
*/
void FLIC_Prepare(int FlicWidth, int FlicHeight, SAMB_addr FlicHdr_SgmtAddr)
{
    unsigned int FlicSize_B;
    unsigned int Dst_Sgmt;
    unsigned int Dst_Ofst;
    unsigned char * pDst;
    int itr_FlicSize_B;

// asm     mov si, [FlicWidth]
// asm     push si
// asm     xor ax, ax
// asm     push ax
// asm     push [FlicHdr_SgmtAddr]
// asm     call farpokew
// asm     add sp, 6
    farpokew(FlicHdr_SgmtAddr, 0x00, FlicWidth);

// asm     push [FlicHeight]
// asm     mov ax, 0x02  // ; 02h 2d  FLIC_HDR.Height
// asm     push ax
// asm     push [FlicHdr_SgmtAddr]
// asm     call farpokew
// asm     add sp, 6
    farpokew(FlicHdr_SgmtAddr, 0x02, FlicHeight);

// asm     mov ax, 0xDE0A  // ; DE0A  Magic/Signature: e_FLIC_Decoded
// asm     push ax
// asm     mov ax, 0x04  // ; 04h 4d  FLIC_HDR.Current_Frame
// asm     push ax
// asm     push [FlicHdr_SgmtAddr]
// asm     call farpokew
// asm     add sp, 6
    farpokew(FlicHdr_SgmtAddr, 0x04, 0xDE0A);  // DE0Ah 56842d; DEh 222d; 0Ah 10d; 0ADEh 2782d;

// asm     xor ax, ax
// asm     push ax
// asm     mov ax, 0x06  // ; 06h 6d  FLIC_HDR.Frame_Count
// asm     push ax
// asm     push [FlicHdr_SgmtAddr]
// asm     call farpokew
// asm     add sp, 6
    farpokew(FlicHdr_SgmtAddr, 0x06, 0x00);

// asm     xor ax, ax
// asm     push ax
// asm     mov ax, 0x08  // ; 08h 8d  FLIC_HDR.Loop_Frame
// asm     push ax
// asm     push [bp+FlicHdr_SgmtAddr]
// asm     call farpokew
// asm     add sp, 6
    farpokew(FlicHdr_SgmtAddr, 0x08, 0x00);

// asm     mov ax, si
// asm     imul WORD PTR [FlicHeight]
// asm     mov [FlicSize_B], ax
    FlicSize_B = FlicWidth * FlicHeight;

// asm     push es
// asm     push di
// asm     mov ax, [FlicHdr_SgmtAddr]
// asm     mov es, ax
// asm     mov di, 0x10  // ; 10h 16d
// asm     mov al, 0
// asm     mov cx, [FlicSize_B]
// asm     rep stosb
// asm     pop di
// asm     pop es
    Dst_Sgmt = FlicHdr_SgmtAddr;
    Dst_Ofst = 0x10;
    pDst = (unsigned char *) MK_FP(Dst_Sgmt,Dst_Ofst);
    for(itr_FlicSize_B = 0; itr_FlicSize_B < FlicSize_B; itr_FlicSize_B++)
    {
        *pDst = 0;
    }

}
