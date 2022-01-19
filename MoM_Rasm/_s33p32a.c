// _s33p32c.c VGA_SaveCursorArea_RSP
// ST_GUI.H

#include "ST_GUI.H"
#include "ST_VGA.H"

void VGA_SaveCursorArea_RSP(int X_Pos, int Y_Pos)
{
    char Width_Bytes;
    int Height_Lines;
    int ScreenPage_Offset;
    unsigned int tmp_DS = -1;
    int tmp_X_Pos = -1;
    int tmp_Width_Bytes = -1;
    int tmp_ScreenPageOffset = -1;
    int tmp_Y_pos = -1;
    int tmp_Height_Lines = -1;

//    dlvfprintf("DEBUG: %s %d BEGIN: VGA_SaveCursorArea_RSP()\n", __FILE__, __LINE__);

asm    push si
asm    push di
asm    push es
asm    push ds

//asm    mov ax, seg dseg
asm    mov ax, seg DGROUP
asm    mov ds, ax
//asm    assume ds:dseg
asm    assume ds:DGROUP

asm    mov ax, ds
asm    mov es, ax
//asm    assume es:dseg
asm    assume es:DGROUP

asm    mov ax, [g_RenderScreenPage]
asm    mov ah, al                              //; (_AX << 8)
asm    sub al, al                              //; (_AX & 0xFF00)
asm    shl ax, 1
asm    shl ax, 1                               //; (_AX << 2) == (_AX * 2^2) == (_AX * 4)
asm    add ax, 0A000h                          //; VGA - Graphics Mode - Video Memory Segment Address / Base
asm    mov ds, ax

asm    mov si, [X_Pos]
asm    shr si, 1
asm    shr si, 1                               //; (_SI >> 2) == (_SI / 2^2) == (_SI / 4)
asm    mov ax, si
asm    mov bx, 3
asm    add ax, bx
asm    sub ax, 50h                             //; _AX = (_AX - 80)  SCREEN_WIDTH_PER_PLANE
asm    js short loc_24831                      //; Jump If (80 > _AX)  Jump near if sign (SF=1)
asm    sub bx, ax                              //; _BX = (_BX - _AX) ... But, we just made _AX be negative, so _BX = (_BX + _AX)

loc_24831:
asm    mov ax, [Y_Pos]
asm    mov cx, 50h                             //; 80d 50h SCREEN_WIDTH_PER_PLANE
asm    mul cx                                  //; _AX = (_AX * _CX)
asm    add si, ax
asm    sub di, di
asm    mov [word ptr es:g_Cursor_Save_Main+di], si    //; ScreenPage Offset
asm    add di, 2
asm    mov es:g_Cursor_Save_Main[di], bx              //; Width_Bytes
asm    add di, 2
asm    mov [Width_Bytes], bl
asm    mov ax, [Y_Pos]
asm    mov bx, 10h                                  //; 16 ? Cursor Dimension ? Width ?
asm    add ax, bx
asm    sub ax, 0C8h                                 //; 200 == SCREEN_WIDTH_PIXELS
asm    js short loc_2485F
asm    sub bx, ax

loc_2485F:
asm    mov es:g_Cursor_Save_Main[di], bx              //; Height_Lines
asm    add di, 2
asm    mov [Height_Lines], bx
asm    mov [ScreenPage_Offset], si

asm    sub bl, bl
asm    mov al, 4                                //; 00000100 Read Map Select
asm    mov dx, 3CEh                             //; GC_INDEX - 
asm    out dx, al
asm    inc dx                                   //; GC_DATA

LoopsPlanes:
asm    mov al, bl                               //; bitplane - 0: intensity; 1: read; 2: green; 3: blue;
asm    out dx, al
asm    sub bh, bh

LoopLines:
asm    mov cx, [Height_Lines]

LoopWords:
asm    lodsw
// asm    mov word ptr es:g_Cursor_Save_Main.Saved_Image[di-6], ax
asm    mov word ptr es:g_Cursor_Save_Main[di-6], ax
asm    add di, 2
asm    add si, 4Eh                              //; Line-Delta  78 bytes to next line, because LODS-Word
asm    loop LoopWords

asm    mov si, [ScreenPage_Offset]
asm    inc bh
asm    sub ah, ah
asm    mov al, bh
asm    shl ax, 1
asm    add si, ax
asm    cmp bh, [Width_Bytes]
asm    jnz short LoopLines

asm    inc bl

asm    mov si, [ScreenPage_Offset]
asm    cmp bl, 4
asm    jnz short LoopsPlanes

asm    pop ds
asm    pop es
asm    pop di
asm    pop si

//    dlvfprintf("DEBUG: %s %d END: VGA_SaveCursorArea_RSP()\n", __FILE__, __LINE__);

}
