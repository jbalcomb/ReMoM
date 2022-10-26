TITLE   _s10p.asm 

.MODEL LARGE

segment dseg para public 'DATA' use16
ends dseg

segment	seg010 byte public 'CODE' use16
    assume cs:seg010
    ;org 6
    ;assume es:nothing, ss:nothing, ds:dseg,	fs:nothing, gs:nothing


proc UU_LBX_SetPath far

LBX_Path@= word	ptr  6

push	bp
mov	bp, sp
push	[bp+LBX_Path@]
mov	ax, offset LBX_LoadPath
push	ax
call	strcpy

pop	cx
pop	cx
pop	bp
retf

endp UU_LBX_SetPath



ends seg010

end
