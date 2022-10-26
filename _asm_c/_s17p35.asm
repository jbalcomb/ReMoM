TITLE _s17p35.asm VGA_DrawTextLine
; ST_TXT.H

.MODEL LARGE, C

.CODE
;segment seg017 byte public 'CODE' use16
;	 assume cs:seg017
;	 ;org 0Ah
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_DrawTextLine

proc VGA_DrawTextLine

Left= word ptr 6
Top= word ptr 8
String= word ptr 0Ah

push bp
mov bp, sp

xor ax, ax
push ax
push [bp+String]
push [bp+Top]
push [bp+Left]
nop
push cs
call near ptr VGA_DrawStyledString
add sp, 8

pop bp
ret

endp VGA_DrawTextLine

;ends seg017

end
