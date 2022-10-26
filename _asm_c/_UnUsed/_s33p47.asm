TITLE _s33p.asm 

.MODEL LARGE, C

.CODE
;segment    seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 


proc UU_GUI_AddBufferClick far

X_Pos= word ptr     6
Y_Pos= word ptr     8
Buttons= word ptr  0Ah

push    bp
mov    bp, sp
push    ds
push    es
push    si
push    di
mov    ax, [UU_GUI_ClickBuffer@]
mov    es, ax
mov    si, [UU_GUI_ClickBufferX]
mov    cx, [UU_GUI_ClickBufCount]
shl    cx, 1
cmp    si, cx
jnz    short loc_24D0B

push    cs
call    near ptr UU_GUI_ClearBufferClick

pop    di
pop    si
pop    es
pop    ds
pop    bp
retf

; ---------------------------------------------------------------------------

loc_24D0B:
mov    di, si
shr    di, 1
add    di, cx
mov    ax, [bp+X_Pos]
mov    [es:si], ax
mov    ax, [bp+Y_Pos]
xor    ah, ah
mov    [es:di], al
mov    ax, [bp+Buttons]
add    di, [UU_GUI_ClickBufCount]
mov    [es:di], al
inc    si
inc    si
mov    [UU_GUI_ClickBufferX], si
pop    di
pop    si
pop    es
pop    ds
pop    bp
retf

endp UU_GUI_AddBufferClick

;ends seg033

end
