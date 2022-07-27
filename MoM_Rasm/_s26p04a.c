// _s26p04a.c VGA_PageFlip
// ST_VGA.H

#include "STU_DBG.H"

extern unsigned int gsa_DSP_Addr;
extern unsigned int g_RSP_Idx;

extern void VGA_WaitSync(void);
extern void VGA_Set_DSP_Addr(void);

void VGA_PageFlip(void)
{
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: VGA_PageFlip()\n", __FILE__, __LINE__);
#endif
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] g_RSP_Idx: %u\n", __FILE__, __LINE__, g_RSP_Idx);
#endif

asm push es
asm push ds
asm push si
asm push di
asm mov ax, 1
asm sub ax, [g_RSP_Idx]
asm mov [g_RSP_Idx], ax
asm shl ax, 1
asm shl ax, 1
asm shl ax, 1
asm shl ax, 1
asm shl ax, 1
asm shl ax, 1
asm mov bx, ax
asm mov dx, 0x03D4  //; CRTC_INDEX
asm mov al, 0x0C    //; CRTC_START_ADDRESS_HIGH
asm out dx, al
asm inc dx
asm mov al, bl
asm out dx, al
//casm call VGA_WaitSync
//asm call VGA_Set_DSP_Addr
asm pop di
asm pop si
asm pop ds
asm pop es
//asm ret

    VGA_WaitSync();
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] g_RSP_Idx: %u\n", __FILE__, __LINE__, g_RSP_Idx);
#endif
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif
    VGA_Set_DSP_Addr();  // _s26p02.asm
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: VGA_PageFlip()\n", __FILE__, __LINE__);
#endif
}