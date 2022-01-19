TITLE _s17p.asm 
; ST_TXT.H

.MODEL LARGE, C

.CODE
;segment seg017 byte public 'CODE' use16
;	 assume cs:seg017
;	 ;org 0Ah
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc UU_VGA_DisableAAPixels far
push bp
mov bp, sp
mov [VGA_Skip_AA_Pixels], 1
pop bp
retf
endp UU_VGA_DisableAAPixels

;ends seg017

end
