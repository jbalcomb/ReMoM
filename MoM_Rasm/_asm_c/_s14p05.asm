TITLE _s14p05.asm VGA_ResetDrawWindow

.MODEL LARGE, C

.CODE
;segment seg014 byte public 'CODE' use16
;    assume cs:seg014
;    ;org 0Dh
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_ResetDrawWindow

proc VGA_ResetDrawWindow far
push bp
mov bp, sp
mov [VGA_Min_X], 0 ; ~= MinX
mov [VGA_Max_X], 319 ; ~= MaxX - 1
mov [VGA_Min_Y], 0 ; ~= MinY
mov [VGA_Max_Y], 199 ; ~= MaxY - 1
pop bp
retf
endp VGA_ResetDrawWindow

;ends seg014

end
