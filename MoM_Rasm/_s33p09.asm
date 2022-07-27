TITLE _s33p09.asm MD_Reset
; ST_GUI.H

.MODEL LARGE, C

EXTRN g_MouseDriverInstalled:WORD
EXTRN g_MD_CursorDraw:WORD
EXTRN g_MD_ClickRec1:WORD
EXTRN g_MD_ClickRec2:WORD
EXTRN g_MD_InitX:WORD
EXTRN g_MD_InitY:WORD

EXTRN MD_WaitRelease:PROC
EXTRN MD_GetX:PROC
EXTRN MD_GetY:PROC

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MD_Reset

proc MD_Reset far

    push si
    push di
    push es
    push ds
    
;    mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax
    
    cmp [g_MouseDriverInstalled], 0
    jz short @@Done

    ;push cs
    ;call near ptr MD_WaitRelease
    call MD_WaitRelease

    mov [g_MD_CursorDraw], 0
    mov [g_MD_ClickRec1], 0
    mov [g_MD_ClickRec2], 0

    ;push cs
    ;call near ptr MD_GetX
    ;nop
    call MD_GetX

    mov [g_MD_InitX], ax

    ;push cs
    ;call near ptr MD_GetY
    ;nop
    call MD_GetY

    mov [g_MD_InitY], ax

    cli

    mov ax, 21h  ; INT 33,21 - Reset Mouse Software
    int 33h

    sti

@@Done:
    mov [g_MouseDriverInstalled], 0
    
    pop ds
    pop es
    pop di
    pop si
    ret

endp MD_Reset

;ends seg033

end
