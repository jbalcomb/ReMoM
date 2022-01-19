TITLE _s33p10.asm MOUSE_INTSet_MvOnly
; ST_GUI.H

.MODEL LARGE, C

SEGMENT _S33P12_TEXT Word Public 'CODE'
ENDS _S33P12_TEXT

EXTRN g_MouseDriverInstalled:WORD
EXTRN g_MOUSE_CursorDraw:WORD

EXTRN MOUSE_INT_Handler:PROC

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MOUSE_INTSet_MvOnly

proc MOUSE_INTSet_MvOnly

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
    mov cx, 1
    mov dx, offset MOUSE_INT_Handler
    int 33h
    mov ax, -1
    mov [g_MOUSE_CursorDraw], 1
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

endp MOUSE_INTSet_MvOnly


;ends seg033

end
