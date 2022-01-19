TITLE _s03p.asm 

.MODEL LARGE, C

.CODE
;segment	seg003 byte public 'CODE' use16
;	assume cs:seg003
;	assume es:nothing, ss:nothing, ds:dseg,	fs:nothing, gs:nothing

PUBLIC 

proc UU_DISK_CheckDrive	far
Drive_Num= word	ptr 6
push	bp
mov	bp, sp
push	es
push	ds
push	si
push	di
mov	ax, [bp+Drive_Num]
push	ax
mov	ah, 19h
int	21h
pop	dx
push	ax
push	dx
mov	al, dl
mov	ah, 0Eh
int	21h
mov	ah, 19h
int	21h
pop	bx
pop	dx
cmp	al, bl
jnz	short loc_14150
mov	bx, 0FFFFh
jmp	short loc_14153
align 2
loc_14150:
mov	bx, 0
loc_14153:
push	bx
mov	ah, 0Eh
int	21h
pop	ax
pop	di
pop	si
pop	ds
pop	es
pop	bp
retf
endp UU_DISK_CheckDrive

;ends seg003

end
