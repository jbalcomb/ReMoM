TITLE _s33p.asm 

.MODEL LARGE, C

.CODE
;segment    seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 


proc UU_VGA_RestoreToFrame3 far

Width_Bytes= byte ptr -6
Height_Lines= word ptr -4
VGA_Offset= word ptr -2

push    bp
mov    bp, sp
sub    sp, 6
push    si
push    di
push    es
push    ds
mov    ax, seg    dseg
mov    ds, ax
cmp    [GUI_PrevCursor], 0
jnz    short loc_24A0D

pop    ds
pop    es
pop    di
pop    si
mov    sp, bp
pop    bp
retf

; ---------------------------------------------------------------------------

loc_24A0D:
sub    si, si
mov    di, [word ptr Cursor_Save_Main.VGA_Offset+si]
add    si, 2
mov    [bp+VGA_Offset], di
mov    ax, Cursor_Save_Main[si]
add    si, 2
mov    [bp+Width_Bytes], al
mov    ax, Cursor_Save_Main[si]
add    si, 2
mov    [bp+Height_Lines], ax
mov    ax, 0A800h
mov    es, ax
assume es:nothing
mov    bl, 1
mov    dx, 3C4h
mov    al, 2
out    dx, al
inc    dx


loc_24A3B:
mov    al, bl
out    dx, al
sub    bh, bh


loc_24A40:
mov    cx, [bp+Height_Lines]


loc_24A43:
mov    ax, Cursor_Save_Main[si]
stosw
add    si, 2
add    di, 4Eh
loop    loc_24A43

mov    di, [bp+VGA_Offset]
inc    bh
sub    ah, ah
mov    al, bh
shl    ax, 1
add    di, ax
cmp    bh, [bp+Width_Bytes]
jnz    short loc_24A40

shl    bl, 1
mov    di, [bp+VGA_Offset]
cmp    bl, 10h
jnz    short loc_24A3B

pop    ds
pop    es
assume es:nothing
pop    di
pop    si
mov    sp, bp
pop    bp
retf

endp UU_VGA_RestoreToFrame3



;ends seg033

end
