TITLE _s19p11.asm VGA_GetFontHeight

.MODEL LARGE, C

.CODE
;segment seg019 byte public 'CODE' use16
;    assume cs:seg019
;    ;org 0Dh
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_GetFontHeight

proc VGA_GetFontHeight
Font_Height= word ptr -2
push bp
mov bp, sp
sub sp, 2
mov ax, FONT_HEADER.Font_Height
push ax
push [Font_Data_Seg@]
call PEEK_Word
pop cx
pop cx
mov [bp+Font_Height], ax
mov ax, [bp+Font_Height]
jmp short $+2
mov sp, bp
pop bp
ret
endp VGA_GetFontHeight

;ends seg019

end
