// _s26p04a.c VGA_PageFlip
// ST_VGA.H

#include "STU_DBG.H"

extern unsigned int g_DrawScreenPage_SgmtAddr;
extern unsigned int g_RenderScreenPage;

extern void VGA_WaitSync(void);
extern void VGA_SetDrawFrame(void);

void VGA_PageFlip(void)
{
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: VGA_PageFlip()\n", __FILE__, __LINE__);
#endif
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] g_RenderScreenPage: %u\n", __FILE__, __LINE__, g_RenderScreenPage);
#endif

asm push es
asm push ds
asm push si
asm push di
asm mov ax, 1
asm sub ax, [g_RenderScreenPage]
asm mov [g_RenderScreenPage], ax
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
//asm call VGA_SetDrawFrame
asm pop di
asm pop si
asm pop ds
asm pop es
//asm ret

    VGA_WaitSync();
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] g_RenderScreenPage: %u\n", __FILE__, __LINE__, g_RenderScreenPage);
#endif
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] g_DrawScreenPage_SgmtAddr: 0x%04X\n", __FILE__, __LINE__, g_DrawScreenPage_SgmtAddr);
#endif
    VGA_SetDrawFrame();  // _s26p02.asm
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] g_DrawScreenPage_SgmtAddr: 0x%04X\n", __FILE__, __LINE__, g_DrawScreenPage_SgmtAddr);
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: VGA_PageFlip()\n", __FILE__, __LINE__);
#endif
}