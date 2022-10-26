TITLE _s03p.asm 

.MODEL LARGE, C

.CODE
;segment	seg003 byte public 'CODE' use16
;	assume cs:seg003
;	assume es:nothing, ss:nothing, ds:dseg,	fs:nothing, gs:nothing

PUBLIC 

proc UU_DISK_DeleteFile	far
FileName@= word	ptr 6
push	bp
mov	bp, sp
push	es
push	di
push	ds
push	si
mov	dx, [bp+FileName@]
mov	ah, 41h
int	21h
xor	ah, ah
pop	si
pop	ds
pop	di
pop	es
pop	bp
retf
endp UU_DISK_DeleteFile

;ends seg003

end
