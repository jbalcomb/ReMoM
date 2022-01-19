TITLE _s33p.asm 

.MODEL LARGE, C

.CODE
;segment    seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 


proc UU_GUI_GetClickBuffVar1 far
mov    ax, [UU_GUI_ClickBuffVar1]
retf

endp UU_GUI_GetClickBuffVar1

;ends seg033

end
