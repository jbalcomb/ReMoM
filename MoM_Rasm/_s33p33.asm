TITLE _s33p33.asm GUI_CursorCopy_Bkup2Main
; ST_GUI.H

.MODEL LARGE, C

EXTRN g_Cursor_Save_Main:WORD
EXTRN g_Cursor_Save_Bkup:WORD

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_CursorCopy_Bkup2Main

proc GUI_CursorCopy_Bkup2Main

    push si
    push di
    push es
    push ds

;    mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax
    ;assume ds:dseg
    assume ds:DGROUP
    mov es, ax
    ;assume es:dseg
    assume es:DGROUP

    mov di, offset g_Cursor_Save_Main
    mov si, offset g_Cursor_Save_Bkup
    mov cx, 258h ; 600 words, 1200 bytes
    rep movsw

    pop ds
    pop es
    assume es:nothing
    pop di
    pop si
    ret

endp GUI_CursorCopy_Bkup2Main

;ends seg033

end
