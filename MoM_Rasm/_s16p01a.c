// _s16p01a.c VGA_DrawFilledRect
// ST_VGA.H

extern unsigned int gsa_DSP_Addr;
extern unsigned char g_VGA_LeftBits;
extern unsigned char g_VGA_RightBits;

void VGA_DrawFilledRect(int x1, int y1, int x2, int y2, unsigned char color)
{
    int Line_Count;

    // dlvfprintf("DEBUG: [%s, %d] BEGIN: VGA_DrawFilledRect(x1=%d, y1=%d, x2=%d, y2=%d, color=%u)\n", __FILE__, __LINE__, x1, y1, x2, y2, color);

//asm push bp
//asm mov bp, sp
//asm sub sp, 4
asm push ds
asm push di
asm push si
asm push es
asm mov dx, 0x03C4 //; SC_INDEX
asm mov al, 0x02  //; SC_MAP_MASK
asm out dx, al
asm mov ax, [x2]
asm mov bx, [x1]

// asm push ax
// asm push bx
//     dlvfprintf("DEBUG: [%s, %d] AX: 0x%04X %u\n", __FILE__, __LINE__, _AX, _AX);
// asm pop bx
// asm pop ax

// asm push ax
// asm push bx
//     dlvfprintf("DEBUG: [%s, %d] BX: 0x%04X %u\n", __FILE__, __LINE__, _BX, _BX);
// asm pop bx
// asm pop ax

asm shr ax, 1
asm shr ax, 1
asm shr bx, 1
asm shr bx, 1

// asm push ax
// asm push bx
//     dlvfprintf("DEBUG: [%s, %d] AX: 0x%04X %u\n", __FILE__, __LINE__, _AX, _AX);
// asm pop bx
// asm pop ax

// asm push ax
// asm push bx
//     dlvfprintf("DEBUG: [%s, %d] BX: 0x%04X %u\n", __FILE__, __LINE__, _BX, _BX);
// asm pop bx
// asm pop ax

asm cmp ax, bx
asm jnz short NayDivFour

YayDivFour:

// asm push ax
// asm push bx
//     dlvfprintf("DEBUG: [%s, %d] Jmp YayDivFour:\n", __FILE__, __LINE__);
// asm pop bx
// asm pop ax

asm mov di, ax
asm mov si, offset g_VGA_LeftBits
asm mov ax, [x1]
asm and ax, 3
asm add si, ax
asm lodsb
asm mov bl, al
asm mov si, offset g_VGA_RightBits
asm mov ax, [x2]
asm and ax, 3
asm add si, ax
asm lodsb
asm and bl, al
asm mov dx, 0x03C5 //; SC_DATA
asm mov al, bl
asm out dx, al
asm mov ax, [y1]
asm mov dx, ax
asm mov cx, [y2]
asm inc cx
asm sub cx, ax
asm shl dx, 1
asm shl dx, 1
asm add ax, dx
asm add ax, [gsa_DSP_Addr]
asm mov es, ax
asm mov al, [color]
LoopAllBytes:
asm stosb
asm add di, 0x4F  //; Line-Delta - 79 bytes (80 - 1)
asm loop LoopAllBytes
// asm pop es
// asm pop si
// asm pop di
// asm pop ds
// //asm mov sp, bp
// //asm pop bp
// //asm ret
asm jmp Done


NayDivFour:

// asm push ax
// asm push bx
//     dlvfprintf("DEBUG: [%s, %d] Jmp NayDivFour:\n", __FILE__, __LINE__);
// asm pop bx
// asm pop ax

asm mov dx, [y2]
asm mov bx, [y1]
asm sub dx, bx
asm inc dx
asm mov [Line_Count], dx
asm mov ax, bx
asm shl ax, 1
asm shl ax, 1
asm add ax, bx
asm add ax, [gsa_DSP_Addr]
asm mov es, ax
asm mov di, [x1]
asm mov bx, di
asm shr di, 1
asm shr di, 1
asm and bx, 3

// asm push ax
// asm push bx
//     dlvfprintf("DEBUG: [%s, %d] BX: 0x%04X %u\n", __FILE__, __LINE__, _BX, _BX);
// asm pop bx
// asm pop ax

asm add bx, offset g_VGA_LeftBits
asm mov al, [bx]

// asm push ax
// asm push bx
//     dlvfprintf("DEBUG: [%s, %d] AL: 0x%02X %u\n", __FILE__, __LINE__, _AL, _AL);
// asm pop bx
// asm pop ax

asm mov dx, 0x03C5 //; SC_DATA
asm out dx, al
asm mov cx, [Line_Count]
asm mov al, [color]
LoopLeftBytes:
asm stosb
asm add di, 0x4F  //; Line-Delta - 79 bytes (80 - 1)
asm loop LoopLeftBytes
asm mov di, [x2]
asm mov bx, di
asm shr di, 1
asm shr di, 1
asm and bx, 3

// asm push ax
// asm push bx
//     dlvfprintf("DEBUG: [%s, %d] BX: 0x%04X %u\n", __FILE__, __LINE__, _BX, _BX);
// asm pop bx
// asm pop ax

asm add bx, offset g_VGA_RightBits
asm mov al, [bx]

// asm push ax
// asm push bx
//     dlvfprintf("DEBUG: [%s, %d] AL: 0x%02X %u\n", __FILE__, __LINE__, _AL, _AL);
// asm pop bx
// asm pop ax

asm mov dx, 0x03C5 //; SC_DATA
asm out dx, al
asm mov cx, [Line_Count]
asm mov al, [color]
LoopRightBytes:
asm stosb
asm add di, 0x4F  //; Line-Delta - 79 bytes (80 - 1)
asm loop LoopRightBytes
asm mov al, 0xF  //; SC_MAP_MASK - All 4 Planes

// asm push ax
// asm push bx
//     dlvfprintf("DEBUG: [%s, %d] AL: 0x%02X %u\n", __FILE__, __LINE__, _AL, _AL);
// asm pop bx
// asm pop ax

asm mov dx, 0x03C5 //; SC_DATA
asm out dx, al
asm mov bx, [x2]
asm shr bx, 1
asm shr bx, 1
asm mov ax, [x1]
asm shr ax, 1
asm shr ax, 1
asm sub bx, ax
asm dec bx
asm jz short Done

asm mov di, ax
asm inc di
asm mov dx, 0x50  //; Line-Width - 80 bytes  (AKA SCREENWIDTH - Bytes Per Memory Plane)
asm sub dx, bx
asm mov si, [Line_Count]
asm mov al, [color]

LoopMiddleBytes:
asm mov cx, bx
asm rep stosb
asm add di, dx
asm dec si
asm jnz short LoopMiddleBytes

Done:
asm pop es
asm pop si
asm pop di
asm pop ds
//asm mov sp, bp
//asm pop bp
//asm ret

    // dlvfprintf("DEBUG: [%s, %d] END: VGA_DrawFilledRect()\n", __FILE__, __LINE__);
}