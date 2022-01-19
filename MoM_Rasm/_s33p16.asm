TITLE _s33p16.asm KBD_CheckBuffer
; ST_GUI.H

.MODEL LARGE, C

EXTRN MOUSE_Disable_CDraw:PROC
EXTRN MOUSE_CDraw_Restore:PROC

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC KBD_CheckBuffer

proc KBD_CheckBuffer

    push ds
    push es
    
    ;push cs
    ;call near ptr MOUSE_Disable_CDraw
    
    mov ah, 11h
    int 16h
    jz short KbdBufEmpty
    
    mov ax, 1  ; ST_TRUE
    
    ;push cs
    ;call near ptr MOUSE_CDraw_Restore
    
    pop es
    pop ds
    ret
    
KbdBufEmpty:
    mov ax, 0  ; ST_FALSE
    
    ;push cs
    ;call near ptr MOUSE_CDraw_Restore
    
    pop es
    pop ds
    ret

endp KBD_CheckBuffer

;ends seg033

end
