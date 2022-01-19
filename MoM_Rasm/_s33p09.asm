TITLE _s33p09.asm MOUSE_Reset
; ST_GUI.H

.MODEL LARGE, C

EXTRN g_MouseDriverInstalled:WORD
EXTRN g_MOUSE_CursorDraw:WORD
EXTRN g_MOUSE_ClickRec1:WORD
EXTRN g_MOUSE_ClickRec2:WORD
EXTRN g_MOUSE_InitX:WORD
EXTRN g_MOUSE_InitY:WORD

EXTRN MOUSE_WaitRelease:PROC
EXTRN MOUSE_GetX:PROC
EXTRN MOUSE_GetY:PROC

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MOUSE_Reset

proc MOUSE_Reset far

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
    ;call near ptr MOUSE_WaitRelease
    call MOUSE_WaitRelease

    mov [g_MOUSE_CursorDraw], 0
    mov [g_MOUSE_ClickRec1], 0
    mov [g_MOUSE_ClickRec2], 0

    ;push cs
    ;call near ptr MOUSE_GetX
    ;nop
    call MOUSE_GetX

    mov [g_MOUSE_InitX], ax

    ;push cs
    ;call near ptr MOUSE_GetY
    ;nop
    call MOUSE_GetY

    mov [g_MOUSE_InitY], ax

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

endp MOUSE_Reset

;ends seg033

end
