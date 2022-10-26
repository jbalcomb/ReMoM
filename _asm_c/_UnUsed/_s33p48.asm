
proc UU_GUI_ProcBufferClick far
push    ds
push    es
push    si
push    di
mov    ax, [UU_GUI_ClickBuffer@]
mov    es, ax
mov    si, [UU_GUI_ClickBuffNext]
mov    ax, [UU_GUI_ClickBufferX]
cmp    ax, si
jnz    short loc_24D52


loc_24D49:
push    cs
call    near ptr UU_GUI_ClearBufferClick

pop    di
pop    si
pop    es
pop    ds
retf

; ---------------------------------------------------------------------------

loc_24D52:
mov    cx, [UU_GUI_ClickBufCount]
shl    cx, 1
mov    di, si
shr    di, 1
add    di, cx
xor    bx, bx
mov    ax, [es:si]
cmp    ax, -1
jz    short loc_24D49

mov    bl, [es:di]
xor    cx, cx
add    di, [UU_GUI_ClickBufCount]
mov    cl, [es:di]
push    cx
push    bx
push    ax
push    cs
call    near ptr UU_MOUSE_GenerateClick

cmp    cl, 0
jnz    short loc_24D84

push    cs
call    near ptr MOUSE_GetClick


loc_24D84:
push    cs
call    near ptr MOUSE_MoveCursor

mov    ax, [GUI_WindowCount]
cmp    ax, 2
jb    short loc_24D94

push    cs
call    near ptr GUI_FindWindow


loc_24D94:
push    cs
call    near ptr VGA_RestoreCursrArea

push    cs

loc_24D99:
call    near ptr GUI_SaveCursorArea

push    cs
call    near ptr GUI_DisplayCursor


loc_24DA0:
add    sp, 6
inc    si
inc    si

loc_24DA5:
mov    [UU_GUI_ClickBuffNext],    si
pop    di
pop    si
pop    es
pop    ds
retf

endp UU_GUI_ProcBufferClick

;ends seg033

end
