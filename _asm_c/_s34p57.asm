TITLE _s34p57.asm GUI_Clear

.MODEL LARGE, C

.CODE
;segment    seg034 byte public 'CODE' use16
;    assume cs:seg034
;    ;org 0Eh
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC GUI_Clear

proc GUI_Clear

    push bp
    mov  bp, sp

    mov [GUI_Control_Count], 1
    mov [GUI_FocusedControl], -1
    mov [GUI_MouseFocusCtrl], 0
    mov [GUI_InEditSelect], 0
    mov [GUI_Active_EditSlct], -1

    pop bp
    ret

endp GUI_Clear

;ends seg034

end
