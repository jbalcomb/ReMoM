TITLE _s34p.asm 

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC

proc UU_GUI_SetSelectable far
Ctrl_Index= word ptr 6
push bp
mov bp, sp
mov cx, [bp+Ctrl_Index]
mov ax, cx
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_DialogLine
jnz short loc_2BA3F
mov ax, cx
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov [es:bx+GUI_CTRL.Selectable], 1
loc_2BA3F:
pop bp
retf
endp UU_GUI_SetSelectable

;ends seg034

end
