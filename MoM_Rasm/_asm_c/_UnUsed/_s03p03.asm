TITLE _s03p.asm 

.MODEL LARGE, C

.CODE
;segment	seg003 byte public 'CODE' use16
;	assume cs:seg003
;	assume es:nothing, ss:nothing, ds:dseg,	fs:nothing, gs:nothing

PUBLIC 

proc RP_DISK_GetDefaultDrive far
push	es
push	ds
push	si
push	di
mov	ah, 19h
int	21h
xor	ah, ah
pop	di
pop	si
pop	ds
pop	es
retf
endp RP_DISK_GetDefaultDrive

;ends seg003

end
