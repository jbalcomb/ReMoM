TITLE _s33p.asm 

.MODEL LARGE, C

.CODE
;segment    seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 


proc UU_GUI_ClickBuffReset far
mov    [UU_GUI_ClickBuffNext],    0
retf

endp UU_GUI_ClickBuffReset

;ends seg033

end
