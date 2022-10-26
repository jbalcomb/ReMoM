TITLE _s10p04.asm LBXR_LoadSingle

.MODEL LARGE, C

.CODE
;segment	seg010 byte public 'CODE' use16
;    assume cs:seg010
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC LBXR_LoadSingle

proc LBXR_LoadSingle far

FileName@= word	ptr 6
Entry_Index= word ptr 8
FirstRec= word ptr 0Ah
RecCount= word ptr 0Ch
RecSize= word ptr 0Eh

push	bp
mov	bp, sp
push	[bp+RecSize]
push	[bp+RecCount]
push	[bp+FirstRec]
xor	ax, ax
push	ax
xor	ax, ax
push	ax
push	[bp+Entry_Index]
push	[bp+FileName@]
nop
push	cs
call	near ptr LBX_RecordLoader
add	sp, 0Eh
pop	bp
retf

endp LBXR_LoadSingle

;ends seg010

end
