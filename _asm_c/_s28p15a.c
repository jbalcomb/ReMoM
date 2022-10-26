// _s28p15a.c FLIC_ResetFrame
// ST_FLIC.H

#include "ST_TYPE.H"
#include "ST_DEF.H"
#include "ST_FLIC.H"
#include "ST_SA.H"  /* SAMB_addr; farpokew(); */


void FLIC_ResetFrame(SAMB_addr FlicHdr_SgmtAddr)
{
// asm    xor ax, ax
// asm    push ax
// asm    mov ax, 0x04  // ; 04h 4d  FLIC_HDR.Current_Frame
// asm    push ax
// asm    push [FlicHdr_SgmtAddr]
// asm    call farpokew
// asm    add sp, 6
    farpokew(FlicHdr_SgmtAddr, 0x04, 0x00);
}
