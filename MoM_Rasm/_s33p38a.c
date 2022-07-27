//TITLE _s33p38.asm GUI_DrawCursor_DSP
// ST_CRSR.H

#include "ST_CRSR.H"


void CRH_Draw_DSP(int X_Pos, int Y_Pos)
{
//asm push bp
//asm mov  bp, sp
asm push si
asm push di
asm push es
asm push ds

asm mov ax, seg DGROUP
asm mov ds, ax
asm assume ds:DGROUP

asm mov dx, [g_CRSR_Curr]
asm cmp dx, 0  //; e_Crsr_None
asm jz Done

asm mov bx, [bp+X_Pos]
asm mov cx, [bp+Y_Pos]
asm push cs
asm call CRL_Draw_DSP //; _s33p40
asm nop

Done:
asm pop ds
asm pop es
asm pop di
asm pop si
//asm pop bp
//asm ret
}
