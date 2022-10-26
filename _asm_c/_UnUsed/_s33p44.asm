TITLE _s33p.asm 

.MODEL LARGE, C

.CODE
;segment    seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 

proc UU_GUI_ClearBufferClick far
mov    ax, [UU_GUI_ClickBuffer@]
mov    es, ax
mov    ax, -1
mov    di, [UU_GUI_ClickBufferX]
shr    di, 1
mov    bx, [UU_GUI_ClickBufCount]
cmp    di, bx
jz    short loc_24CA9

stosw


loc_24CA9:
mov    [UU_GUI_ClickBuffVar1],    0
retf

endp UU_GUI_ClearBufferClick

;ends seg033

end
