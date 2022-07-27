TITLE _s33p11.asm MD_INT_SetMvBtns
; ST_GUI.H

.MODEL LARGE, C

SEGMENT _S33P12_TEXT Word Public 'CODE'
ENDS _S33P12_TEXT

EXTRN g_MouseDriverInstalled:WORD
EXTRN g_MD_CursorDraw:WORD

EXTRN MD_INT_Handler:PROC

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MD_INT_SetMvBtns

proc MD_INT_SetMvBtns

    push si
    push di
    push es
    push ds

;    mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax

    cmp [g_MouseDriverInstalled], 0
    jz short @@Done

    cli
    
;    mov ax, seg seg033
    mov ax, seg _S33P12_TEXT
    mov es, ax
;    assume es:seg033
    assume es:_S33P12_TEXT

    mov ax, 14h
    mov cx, 00101011b
    mov dx, offset MD_INT_Handler
    int 33h

    ;mov ax, 0FFFFh  ; ? artifact ?

    mov [g_MD_CursorDraw], 1
    mov ax, 3
    int 33h
    sti

@@Done:
    pop ds
    pop es
    assume es:nothing
    pop di
    pop si
    ret

endp MD_INT_SetMvBtns


;ends seg033

end
