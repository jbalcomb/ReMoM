// _s26p02c.c VGA_SetDrawFrame
// ST_VGA.H

#include "ST_VGA.H"


void VGA_Set_DSP_Addr(void)
{

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: VGA_Set_DSP_Addr()\n", __FILE__, __LINE__);
#endif

    // ~== asm { mov  ax, 1; sub  ax, [g_RSP_Idx]; mov  ah, al; xor  al, al; shl  ax, 1; shl  ax, 1; add  ax, VRAM_BASE; }
    // gsa_DSP_Addr = VRAM_BASE + ( ((((WORD)(1 - g_RSP_Idx)) << 8) & 0xFF00) << 2 );
    gsa_DSP_Addr = VRAM_BASE + ( (1 - g_RSP_Idx) << 10 );

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dlvfprintf("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dlvfprintf("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: VGA_Set_DSP_Addr()\n", __FILE__, __LINE__);
#endif

}
