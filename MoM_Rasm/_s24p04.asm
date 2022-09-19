; _s24p0.asm 

.MODEL LARGE, C

.CODE

;segment    seg024 byte public 'CODE' use16
;    assume cs:seg024
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 


proc VGA_ReadScreenLine    far

VGA_Offset= word ptr  6
Buffer@= word ptr  8

push    bp
mov    bp, sp
push    si
push    di
push    si
push    di
push    ds
push    es
mov    di, [bp+Buffer@]
mov    ax, [bp+VGA_Offset]
mov    dx, 50h
mul    dx
mov    si, ax
mov    ax, ds
mov    es, ax
assume es:dseg
mov    ax, [VGA_DrawFrame@]
mov    ds, ax
mov    cx, 50h


loc_1DFA5:
mov    al, 4
mov    dx, 3CEh
out    dx, al
inc    dx

loc_1DFAC:
mov    al, 0
out    dx, al
movsb
dec    si
mov    al, 4
mov    dx, 3CEh
out    dx, al
inc    dx
mov    al, 1
out    dx, al
movsb
dec    si
mov    al, 4
mov    dx, 3CEh
out    dx, al
inc    dx
mov    al, 2
out    dx, al
movsb
dec    si
mov    al, 4
mov    dx, 3CEh
out    dx, al
inc    dx
mov    al, 3
out    dx, al
movsb
loop    loc_1DFA5

pop    es
assume es:nothing
pop    ds
pop    di
pop    si
pop    di
pop    si
pop    bp
retf

endp VGA_ReadScreenLine

;ends seg024

end
