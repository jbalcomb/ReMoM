TITLE   _s12p.asm 

.MODEL LARGE

segment dseg para public 'DATA' use16
ends dseg

segment	seg012 byte public 'CODE' use16
    assume cs:seg012
    ;org 6
    ;assume es:nothing, ss:nothing, ds:dseg

proc EMM_Map4Pages far

First_Log_Page=	word ptr  6
EMM_Handle= word ptr  8

push	bp
mov	bp, sp
push	ax
push	bx
push	cx
push	dx
push	si
push	ds
mov	bx, [bp+First_Log_Page]
mov	dx, [bp+EMM_Handle]
mov	ax, seg	dseg
mov	ds, ax
mov	[EMM_Log2Phys_Map.Logical_Page], bx
inc	bx
mov	[EMM_Log2Phys_Map.Logical_Page+4], bx
inc	bx
mov	[EMM_Log2Phys_Map.Logical_Page+8], bx
inc	bx
mov	[EMM_Log2Phys_Map.Logical_Page+0Ch], bx
inc	bx
mov	cx, 4
mov	si, offset EMM_Log2Phys_Map

loc_1642A:
mov	ax, 5000h
int	67h

pop	ds

loc_16430:
pop	si
pop	dx
pop	cx
pop	bx
pop	ax
pop	bp

locret_16436:
retf

endp EMM_Map4Pages

ends seg012

end
