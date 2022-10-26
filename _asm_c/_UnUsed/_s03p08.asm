TITLE _s03p.asm 

.MODEL LARGE, C

.CODE
;segment	seg003 byte public 'CODE' use16
;	assume cs:seg003
;	assume es:nothing, ss:nothing, ds:dseg,	fs:nothing, gs:nothing

PUBLIC 

proc UU_DISK_GetCurrentDir far
Dest@= word ptr	 6
push	bp
mov	bp, sp
push	si
push	di
push	es
push	ds
mov	ah, 19h
int	21h
mov	si, [bp+Dest@]
add	al, 41h
mov	ah, 3Ah
mov	[si], ax
add	si, 2
mov	al, 5Ch
mov	[si], al
loc_1417A:
add	si, 1
mov	dx, 0
loc_14180:
mov	ah, 47h
int	21h
xor	ah, ah
loc_14186:
pop	ds
pop	es
pop	di
pop	si
pop	bp
retf
endp UU_DISK_GetCurrentDir

;ends seg003

end
