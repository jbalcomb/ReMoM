TITLE _s34p85.asm GUI_SimplePageFlip

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_SimplePageFlip

proc GUI_SimplePageFlip

    push bp
    mov  bp, sp
    push si
    push di

    call MOUSE_Disable_CDraw
    call MOUSE_GetX
    mov si, ax
    call MOUSE_GetY
    mov di, ax
    push cs
    call near ptr GUI_DrawControls
    push di
    push si
    call GUI_FindWindow
    pop cx
    pop cx
    push di
    push si
    call GUI_SaveDrawCrsrArea
    pop cx
    pop cx
    push di
    push si
    call GUI_DrawCursor
    pop cx
    pop cx
    call VGA_PageFlip
    call VGA_RestoreDrawCArea
    call GUI_CursorSaveCopy
    call MOUSE_CDraw_Restore

    pop di
    pop si
    pop bp
    ret

endp GUI_SimplePageFlip

;ends seg034

end
