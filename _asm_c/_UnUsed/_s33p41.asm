TITLE _s33p41.asm 

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc UU_GUI_ClearClickBuffer far
mov    ax, [UU_GUI_ClickBuffer@]
mov    es, ax
mov    ax, -1
mov    di, 0
mov    cx, [UU_GUI_ClickBufCount]
shl    cx, 1


loc_24C77:
stosw
loop    loc_24C77

mov    ax, 0
mov    [UU_GUI_ClickBufferX], ax
mov    [UU_GUI_ClickBuffNext],    ax
retf

endp UU_GUI_ClearClickBuffer


;ends seg033

end
