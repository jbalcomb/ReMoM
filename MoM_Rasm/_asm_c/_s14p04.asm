TITLE _s14p04.asm VGA_SetDrawWindow

.MODEL LARGE, C

.CODE
;segment seg014 byte public 'CODE' use16
;    assume cs:seg014
;    ;org 0Dh
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_SetDrawWindow

proc VGA_SetDrawWindow
Min_X = word ptr 6
Min_Y = word ptr 8
Max_X = word ptr 0Ah
Max_Y = word ptr 0Ch
push bp
mov bp, sp
push si
push di
mov dx, [bp+Min_X]
mov bx, [bp+Min_Y]
mov cx, [bp+Max_X]
mov si, [bp+Max_Y]
or dx, dx
jge short loc_17F43
xor dx, dx
loc_17F43:
or bx, bx
jge short loc_17F49
xor bx, bx
loc_17F49:
cmp cx, 319
jle short loc_17F52
mov cx, 319
loc_17F52:
cmp si, 199
jle short loc_17F5B
mov si, 199
loc_17F5B:
cmp dx, cx
jle short loc_17F65
mov di, cx
mov cx, dx
mov dx, di
loc_17F65:
cmp bx, si
jle short loc_17F6F
mov di, si
mov si, bx
mov bx, di
loc_17F6F:
mov [VGA_Min_X], dx
mov [VGA_Max_X], cx
mov [VGA_Min_Y], bx
mov [VGA_Max_Y], si
pop di
pop si
pop bp
retf
endp VGA_SetDrawWindow

;ends seg014

end
