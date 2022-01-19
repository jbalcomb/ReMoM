TITLE _s33p.asm 

.MODEL LARGE, C

.CODE
;segment    seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 


proc UU_GUI_FindEmptyClick far
push    es
push    ds
push    si
push    di
mov    [UU_GUI_ClickBufferX], 0
mov    ax, [UU_GUI_ClickBuffer@]
mov    es, ax
mov    ax, -1
xor    di, di
mov    bx, [UU_GUI_ClickBufCount]
shl    bx, 1
jmp    short loc_24CCF

; ---------------------------------------------------------------------------
db 90h
; ---------------------------------------------------------------------------

loc_24CCD:
inc    di
inc    di


loc_24CCF:
cmp    [es:di], ax
jz    short loc_24CD8

cmp    di, bx
jnz    short loc_24CCD


loc_24CD8:
mov    [UU_GUI_ClickBufferX], di
mov    ax, di
pop    di
pop    si
pop    ds
pop    es
retf

endp UU_GUI_FindEmptyClick

;ends seg033

end
