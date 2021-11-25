TITLE _s33p01.asm GUI_SetWindows

.MODEL LARGE, C

.CODE
;segment    seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC GUI_SetWindows

proc GUI_SetWindows far

    W_Count  = word ptr  6
    W_Array@ = word ptr  8

    push    bp
    mov    bp, sp
    push    ds
    mov    ax, seg    dseg
    mov    ds, ax
    mov    cx, [bp+W_Count]
    mov    [GUI_WindowCount], cx
    sub    ax, ax
    mov    bx, [bp+W_Array@]
    mov    [GUI_Windows@],    bx
    dec    cx
    jz    short loc_23F74
    mov    dx, cx
    mov    ax, 0Ch
    mul    dx
    add    bx, ax
loc_23F63:
    sub    ax, ax
    cmp    ax, [bx+GUI_WINDOW.Left]
    jnz    short loc_23F6F
    cmp    ax, [bx+GUI_WINDOW.Top]
    jz    short loc_23F74
loc_23F6F:
    sub    bx, 0Ch
    loop    loc_23F63
loc_23F74:
    mov    ax, [bx+GUI_WINDOW.Cursor_Offset]
    mov    [GUI_CursorOffset], ax
    mov    ax, [bx+GUI_WINDOW.Cursor_Index]
    mov    [GUI_CurrentCursor], ax
    pop    ds
    pop    bp
    retf

endp GUI_SetWindows

;ends seg033

end
