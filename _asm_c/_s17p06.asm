TITLE _s17p.asm 
; ST_TXT.H

.MODEL LARGE, C

.CODE
;segment seg017 byte public 'CODE' use16
;	 assume cs:seg017
;	 ;org 0Ah
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc VGA_SetOutlineColor far
Color_Index= word ptr 6
push bp
mov bp, sp
mov ax, [bp+Color_Index]
mov [VGA_Outline_Color], ax
pop bp
retf
endp VGA_SetOutlineColor

;ends seg017

end
