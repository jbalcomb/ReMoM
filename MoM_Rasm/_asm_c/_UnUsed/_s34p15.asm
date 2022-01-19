TITLE _s34p.asm 

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
;    assume cs:seg034
;    ;org 0Eh
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc UU_GUI_GetControlWidth far
Control_Width= word ptr -2
Ctrl_Index= word ptr 6
push bp
mov bp, sp
sub sp, 2
mov cx, [bp+Ctrl_Index]
or cx, cx
jge short loc_25F76
mov ax, cx
neg ax
mov cx, ax
loc_25F76:
cmp cx, [GUI_Control_Count]
jl short loc_25F80
xor ax, ax
loc_25F7E:
jmp short loc_25FAC
loc_25F80:
mov ax, cx
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Right]
push ax
mov ax, cx
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
pop ax
sub ax, [es:bx+GUI_CTRL.Left]
inc ax
mov [bp+Control_Width], ax
mov ax, [bp+Control_Width]
jmp short loc_25F7E
loc_25FAC:
mov sp, bp
pop bp
retf
endp UU_GUI_GetControlWidth


;ends seg034

end
