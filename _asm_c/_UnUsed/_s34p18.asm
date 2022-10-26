TITLE _s34p.asm 

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
;    assume cs:seg034
;    ;org 0Eh
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc UU_GUI_GetLastClickY far
push bp
mov bp, sp
mov ax, [GUI_Processed_LastY]
jmp short $+2
pop bp
retf
endp UU_GUI_GetLastClickY
proc UU_GUI_GetLastClickBtns far
push bp
mov bp, sp
mov ax, [GUI_Processed_Btns]
jmp short $+2
pop bp
retf
endp UU_GUI_GetLastClickBtns
proc GUI_GetMouseFocus far
push bp
mov bp, sp
mov ax, [GUI_MouseFocusCtrl]
jmp short $+2
pop bp
retf
endp GUI_GetMouseFocus


;ends seg034

end
