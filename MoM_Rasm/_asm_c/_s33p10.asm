TITLE _s33p10.asm MD_INT_SetMvOnly
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

PUBLIC MD_INT_SetMvOnly

proc MD_INT_SetMvOnly

    push si
    push di
    push es
    push ds

    mov ax, seg DGROUP
    mov ds, ax

    cmp [g_MouseDriverInstalled], 0
    jz short @@Done

    cli
    ;/*
    ;s33p07 MD_Init()
    ;unsigned short c_sgmt;
    ;unsigned short c_ofst;
    ;c_sgmt = FP_SEG(MD_INT_Handler);     // _s33p12
    ;c_ofst = FP_OFF(MD_INT_Handler);
    ;asm {
    ;    mov ax, c_sgmt
    ;    mov es, ax
    ;    mov dx, c_ofst
    ;    mov ax, 0x0C
    ;    mov cx, 1                           // User Interrupt Mask: 00000001 Cursor Position Change
    ;    int 0x33
    ;}
    ;*/
    mov ax, seg _S33P12_TEXT
    mov es, ax
    assume es:_S33P12_TEXT
    mov ax, 14h
    mov cx, 1       ;// User Interrupt Mask: 00000001 Cursor Position Change
    mov dx, offset MD_INT_Handler
    int 33h
    ;mov ax, -1  ; ? artifact ?
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

endp MD_INT_SetMvOnly


;ends seg033

end
