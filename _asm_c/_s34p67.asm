TITLE _s34p67.asm GUI_SetDelay

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_SetDelay

proc GUI_SetDelay

    Input_Delay = word ptr 6

    push bp
    mov bp, sp
    mov ax, [bp+Input_Delay]
    mov [GUI_Delay], ax
    call MOUSE_GetClick
    call CRP_MOUSE_GetSecClick
    pop bp
    ret

endp GUI_SetDelay

;ends seg034

end
