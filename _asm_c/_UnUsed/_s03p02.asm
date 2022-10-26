TITLE _s03p.asm 

.MODEL LARGE, C

.CODE
;segment	seg003 byte public 'CODE' use16
;	assume cs:seg003
;	assume es:nothing, ss:nothing, ds:dseg,	fs:nothing, gs:nothing

PUBLIC 

proc RP_DISK_GetSpace far
Drive_Num= word	ptr 6
push	bp
mov	bp, sp
loc_13FF6:
push	es
push	di
push	si
push	ds
mov	al, 24h
mov	ah, 35h
int	21h
mov	[word ptr UU_Int24Vector], bx
mov	[word ptr UU_Int24Vector+2], es
mov	ax, cs
mov	ds, ax
assume ds:seg003
mov	dx, offset RP_Return_Ignore
mov	al, 24h
mov	ah, 25h
int	21h
mov	dx, [bp+Drive_Num]
cmp	dx, 3
js	short loc_14023
mov	dx, 1
jmp	short loc_1402B
nop
loc_14023:
cmp	dx, 0
jnz	short loc_1402B
mov	dx, 1
loc_1402B:
mov	ah, 36h
int	21h
assume ds:nothing
push	ax
mov	dx, [ds:3CBEh]
mov	ds, [word ptr ds:3CC0h]
assume ds:dseg
mov	al, 24h
mov	ah, 25h
int	21h
pop	ax
cmp	ax, 0FFFFh
jz	short loc_14047
mov	ax, 0
loc_14047:
pop	ds
pop	si
pop	di
pop	es
pop	bp
retf
endp RP_DISK_GetSpace

;ends seg003

end
