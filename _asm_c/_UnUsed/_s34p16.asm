TITLE _s34p.asm 

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
;    assume cs:seg034
;    ;org 0Eh
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc UU_GUI_GetControlHeight far
Control_Height= word ptr -2
Ctrl_Index= word ptr 6
push bp
mov bp, sp
sub sp, 2
mov cx, [bp+Ctrl_Index]
or cx, cx
jge short loc_25FC3
mov ax, cx
neg ax
mov cx, ax
loc_25FC3:
cmp cx, [GUI_Control_Count]
jl short loc_25FCD
xor ax, ax
loc_25FCB:
jmp short loc_25FFA
loc_25FCD:
mov ax, cx
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Bottom]
push ax
mov ax, cx
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
pop ax
sub ax, [es:bx+GUI_CTRL.Top]
inc ax
mov [bp+Control_Height], ax
mov ax, [bp+Control_Height]
jmp short loc_25FCB
loc_25FFA:
mov sp, bp
pop bp
retf
endp UU_GUI_GetControlHeight


;ends seg034

end
