; _s24p0.asm 

.MODEL LARGE, C

.CODE

;segment    seg024 byte public 'CODE' use16
;    assume cs:seg024
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 


proc UU_VGA_SaveDrawSection far

Left= word ptr    6
Top= word ptr  8
Right= word ptr     0Ah
Bottom=    word ptr  0Ch
IMG_Seg= word ptr  0Eh

push    bp
mov    bp, sp
push    si
push    di
call    VGA_SetDrawFrame

mov    ax, [bp+Right]
sub    ax, [bp+Left]
inc    ax
mov    si, ax
mov    ax, [bp+Bottom]
sub    ax, [bp+Top]
inc    ax
mov    di, ax
or    si, si
jge    short loc_1E019

mov    ax, si
neg    ax
mov    si, ax


loc_1E019:
or    di, di
jge    short loc_1E023

mov    ax, di
neg    ax
mov    di, ax


loc_1E023:
push    [bp+IMG_Seg]
push    di
push    si
call    LBX_Image_Prepare

add    sp, 6
push    [bp+Bottom]
push    [bp+Right]
push    [bp+Top]
push    [bp+Left]
mov    ax, [bp+IMG_Seg]
inc    ax
push    ax
nop
push    cs
call    near ptr UU_VGA_CopyToLBX

add    sp, 0Ah
pop    di
pop    si
pop    bp
retf

endp UU_VGA_SaveDrawSection

;ends seg024

end
