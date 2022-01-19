// _s12p12c.c EMM_MapMulti4
// ST_EMM.H

#include "ST_EMM.H"


void EMM_MapMulti4(int argFirstLogicalPage, int argEmsHandle)
{
asm push bp
asm mov  bp, sp

asm push ax
asm push bx
asm push cx
asm push dx
asm push si
asm push ds

asm mov bx, [argFirstLogicalPage]
asm mov dx, [argEmsHandle]
//asm mov ax, seg dseg
asm mov ax, seg DGROUP
asm mov ds, ax
//asm mov [g_EMM_Log2Phys_Map.Logical_Page], bx
asm mov [g_EMM_Log2Phys_Map+0x00], bx
asm inc bx
//asm mov [g_EMM_Log2Phys_Map.Logical_Page+4], bx
asm mov [g_EMM_Log2Phys_Map+0x04], bx
asm inc bx
//asm mov [g_EMM_Log2Phys_Map.Logical_Page+8], bx
asm mov [g_EMM_Log2Phys_Map+0x08], bx
asm inc bx
//asm mov [g_EMM_Log2Phys_Map.Logical_Page+0Ch], bx
asm mov [g_EMM_Log2Phys_Map+0x0C], bx
asm inc bx
asm mov cx, 4
asm mov si, offset g_EMM_Log2Phys_Map
asm mov ax, 5000h
asm int 67h

asm pop ds
asm pop si
asm pop dx
asm pop cx
asm pop bx
asm pop ax

asm pop bp
}
