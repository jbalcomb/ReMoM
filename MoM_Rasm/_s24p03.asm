; _s24p0.asm 

.MODEL LARGE, C

.CODE

;segment    seg024 byte public 'CODE' use16
;    assume cs:seg024
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 


proc VGA_GetDACBlock far

DAC_Reg= word ptr -2
DAC_BLK= word ptr  6
Buffer@= word ptr  8

push    bp
mov    bp, sp
sub    sp, 2
mov    ax, [bp+DAC_BLK]
mov    cl, 6
shl    ax, cl
mov    [bp+DAC_Reg], ax
push    es
mov    ah, 10h
mov    al, 17h
mov    bx, [bp+DAC_Reg]
mov    cx, (offset NullCheck_Msg+11h)
mov    dx, ds
mov    es, dx
assume es:dseg
mov    dx, [bp+Buffer@]
int    10h

pop    es
assume es:nothing
mov    sp, bp
pop    bp
retf

endp VGA_GetDACBlock

;ends seg024

end
