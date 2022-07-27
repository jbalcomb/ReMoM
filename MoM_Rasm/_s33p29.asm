TITLE _s33p.asm MD_SaveClick
; in ST_GUI.H

.MODEL LARGE, C

EXTRN g_MD_ClickBtns:WORD
EXTRN g_MD_ClickRec1:WORD
EXTRN g_MD_ClickRec2:WORD
EXTRN g_MD_ClickX:WORD
EXTRN g_MD_ClickY:WORD

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MD_SaveClick

proc MD_SaveClick far

    X_Pos = word ptr 6
    Y_Pos = word ptr 8
    Buttons = word ptr 0Ah

    push bp
    mov  bp, sp
    push ds
    push ds
    
;    mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax

    mov ax, [bp+Buttons]
    and ax, 3
    cmp ax, 0
    jnz short @@YayButtonPressed

@@NayButtonPressed:
    pop ds
    pop ds
    pop bp
    ret

@@YayButtonPressed:
    mov [g_MD_ClickBtns], ax
    mov [g_MD_ClickRec1], 1
    mov [g_MD_ClickRec2], 1
    mov ax, [bp+X_Pos]
    mov [g_MD_ClickX], ax
    mov ax, [bp+Y_Pos]
    mov [g_MD_ClickY], ax
    
    sub ax, ax
    
    pop ds
    pop ds
    pop bp
    ret

endp MD_SaveClick

;ends seg033

end
