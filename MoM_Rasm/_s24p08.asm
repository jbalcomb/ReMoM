; _s24p0.asm 

.MODEL LARGE, C

.CODE

;segment    seg024 byte public 'CODE' use16
;    assume cs:seg024
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 


proc UU_VGA_CopyToLBX far

Read_Offset= word ptr -6
Read_Height= word ptr -4
Read_Width= word ptr -2
IMG_Data_Seg= word ptr    6
Left= word ptr    8
Top= word ptr  0Ah
Right= word ptr     0Ch
Bottom=    word ptr  0Eh

push    bp
mov    bp, sp
sub    sp, 6
push    si
push    di
mov    ax, [bp+Right]
sub    ax, [bp+Left]
inc    ax
mov    [bp+Read_Width], ax
mov    ax, [bp+Bottom]
sub    ax, [bp+Top]
inc    ax
mov    [bp+Read_Height], ax
mov    ax, [bp+Top]
mov    dx, 50h
imul    dx
push    ax
mov    ax, [bp+Left]
mov    bx, 4
cwd
idiv    bx
pop    dx
add    dx, ax
mov    [bp+Read_Offset], dx
push    ds
push    es
push    si
push    di
mov    ax, [bp+IMG_Data_Seg]
mov    es, ax
mov    bx, [bp+Read_Width]
mov    si, [bp+Read_Offset]
mov    ax, [bp+Left]
and    al, 3
mov    ah, al
mov    di, [bp+Read_Height]
mov    dx, [VGA_DrawFrame@]
mov    ds, dx
mov    dx, di
mov    di, 0


loc_1E0A5:
push    si
push    dx
mov    al, 4
mov    dx, 3CEh
out    dx, al
inc    dx
mov    al, ah
out    dx, al
pop    dx
mov    cx, dx


loc_1E0B4:
lodsb
stosb
add    si, 4Fh
loop    loc_1E0B4

pop    si
inc    ah
cmp    ah, 4
js    short loc_1E0C6

mov    ah, 0
inc    si


loc_1E0C6:
dec    bx
jnz    short loc_1E0A5

pop    di

loc_1E0CA:
pop    si
pop    es
pop    ds
pop    di
pop    si

loc_1E0CF:
mov    sp, bp
pop    bp
retf

endp UU_VGA_CopyToLBX

;ends seg024

end
