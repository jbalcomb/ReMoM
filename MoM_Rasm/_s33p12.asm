TITLE _s33p12.asm MD_INT_Handler
; ST_GUI.H

.MODEL LARGE, C

EXTRN g_GUI_WindowCount:WORD
EXTRN g_MD_CurrentX:WORD
EXTRN g_MD_CurrentY:WORD
EXTRN g_MD_CursorDraw:WORD
EXTRN g_MD_INT_InProcess:WORD

EXTRN CRH_Draw_RSP:PROC
EXTRN GUI_FindWindow:PROC
EXTRN CRL_Save_RSP:PROC
EXTRN MD_SaveClick:PROC
EXTRN CRL_Restore_RSP:PROC

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MD_INT_Handler

proc MD_INT_Handler

    tmp_GC_Register = word ptr -0Ah
    tmp_GC_BITMASK = word ptr -8
    tmp_GC_READMAP = word ptr -6
    tmp_SC_MAPMASK = word ptr -4

    push bp
    mov bp, sp
    sub sp, 0Ah

    cli
    pushf
    push ds
    push ax
    cld

;    mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax

    mov ax, cx
    shr ax, 1
    mov [g_MD_CurrentX], ax
    mov [g_MD_CurrentY], dx
    
    cmp [g_MD_INT_InProcess], 0
    jz short @@DoDrawCursor
    jmp @@Done

@@DoDrawCursor:
    push si
    push di
    push es
    push bx
    push cx
    push dx

    mov [g_MD_INT_InProcess], 1

    push bx
    push dx
    shr cx, 1
    push cx
    ;push cs
    ;call near ptr MD_SaveClick
    ;nop
    call MD_SaveClick
    add sp, 6

    cmp [g_MD_CursorDraw], 0
    jnz short @@DoDrawCursor1
    jmp @@SkipCursorDraw

@@DoDrawCursor1:
    mov [g_MD_CursorDraw], 0
    mov bx, dx                              ; mouse_y

    mov dx, 3C4h    ; SC_INDEX
    mov ax, 2       ; SC_MAPMASK
    out dx, al

    inc dx          ; SC_DATA
    in al, dx

    mov [bp+tmp_SC_MAPMASK], ax

    mov dx, 3CEh    ; GC_INDEX
    in al, dx

    mov [bp+tmp_GC_Register], ax

    mov ax, 4       ; GC_READMAP
    out dx, al

    inc dx          ; GC_DATA
    in al, dx

    mov [bp+tmp_GC_READMAP], ax

    mov dx, 3CEh    ; GC_INDEX
    mov ax, 8       ; GC_BITMASK
    out dx, al

    inc dx          ; GC_DATA
    in al, dx

    mov [bp+tmp_GC_BITMASK], ax

    mov al, 0FFh
    out dx, al

    push bx
    push cx

    mov ax, [g_GUI_WindowCount]
    cmp ax, 2
    jb short @@DoDrawCursor2

    ; TODO(JimBalcomb): figure out when, where, why GUI_FindWindow() is triggered here
    ;push cs
    ;call near ptr GUI_FindWindow
    call GUI_FindWindow

@@DoDrawCursor2:
    ;push cs
    ;call near ptr VGA_RestoreCursrArea
    ;nop
    call CRL_Restore_RSP ; _s33p34

    ;push cs
    ;call near ptr CRL_Save_RSP
    ;nop
    call CRL_Save_RSP ; _s33p30

    ;push cs
    ;call near ptr CRH_Draw_RSP
    ;nop
    call CRH_Draw_RSP ; _s33p37
    add sp, 4
    
    mov dx, 3C4h    ; SC_INDEX
    mov ax, 2       ; SC_MAPMASK
    out dx, al

    inc dx          ; SC_DATA
    mov ax, [bp+tmp_SC_MAPMASK]
    out dx, al

    mov dx, 3CEh    ; GC_INDEX
    mov ax, 4       ; GC_READMAP
    out dx, al

    inc dx          ; GC_DATA
    mov ax, [bp+tmp_GC_READMAP]
    out dx, al

    mov dx, 3CEh    ; GC_INDEX
    mov ax, 8       ; GC_BITMASK
    out dx, al

    inc dx          ; GC_DATA
    mov ax, [bp+tmp_GC_BITMASK]
    out dx, al

    mov dx, 3CEh    ; GC_INDEX
    mov ax, [bp+tmp_GC_Register]
    out dx, al

    mov [g_MD_CursorDraw], 1
    
@@SkipCursorDraw:
    pop dx
    pop cx
    pop bx
    pop es
    pop di
    pop si
    mov [g_MD_INT_InProcess], 0

@@Done:
    pop ax
    pop ds
    popf
    sti
    mov sp, bp
    pop bp
    ret

endp MD_INT_Handler

;ends seg033

end
