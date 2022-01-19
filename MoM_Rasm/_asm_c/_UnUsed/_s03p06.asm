TITLE _s03p.asm 

.MODEL LARGE, C

.CODE
;segment	seg003 byte public 'CODE' use16
;	assume cs:seg003
;	assume es:nothing, ss:nothing, ds:dseg,	fs:nothing, gs:nothing

PUBLIC 

proc UU_DISK_GetFileDateTime far
DTA_FileTime@= dword ptr -4
FileName@= word	ptr 6
push	bp
mov	bp, sp
sub	sp, 4
push	es
push	di
push	ds
push	si
push	ds
call	MOUSE_Disable_CDraw
mov	ah, 2Fh
int	21h
add	bx, 16h
mov	[word ptr bp+DTA_FileTime@], bx
mov	bx, es
mov	[word ptr bp+DTA_FileTime@+2], bx
push	ds
mov	ah, 4Eh
mov	cx, 0
mov	dx, [bp+FileName@]
int	21h
pop	ds
mov	ax, [word ptr bp+DTA_FileTime@+2]
mov	es, ax
mov	di, [word ptr bp+DTA_FileTime@]
mov	ax, [es:di]
add	di, 2
mov	dx, [es:di]
pop	ds
push	ax
push	dx
call	MOUSE_CDraw_Restore
pop	dx
pop	ax
pop	si
pop	ds
pop	di
pop	es
mov	sp, bp
pop	bp
retf
endp UU_DISK_GetFileDateTime

;ends seg003

end
