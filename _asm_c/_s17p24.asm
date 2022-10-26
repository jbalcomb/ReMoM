TITLE _s17p24.asm VGA_DrawCentered
; ST_TXT.H

.MODEL LARGE, C

.CODE
;segment seg017 byte public 'CODE' use16
;	 assume cs:seg017
;	 ;org 0Ah
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_DrawCentered

proc VGA_DrawCentered

Return_Value= word ptr -4
String_Width= word ptr -2
Center= word ptr 6
Top= word ptr 8
String= word ptr 0Ah

push bp
mov bp, sp
sub sp, 4
push si
mov si, [bp+String]
push si
call VGA_GetStringWidth
pop cx
mov [bp+String_Width], ax
push si
push [bp+Top]
mov ax, [bp+String_Width]
cwd
sub ax, dx
sar ax, 1
mov dx, [bp+Center]
sub dx, ax
push dx
nop
push cs
call near ptr VGA_DrawTextLine
add sp, 6
mov [bp+Return_Value], ax
mov ax, [bp+Return_Value]
jmp short $+2
pop si
mov sp, bp
pop bp
ret

endp VGA_DrawCentered

;ends seg017

end
