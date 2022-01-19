TITLE _s03p.asm 

.MODEL LARGE, C

.CODE
;segment	seg003 byte public 'CODE' use16
;	assume cs:seg003
;	assume es:nothing, ss:nothing, ds:dseg,	fs:nothing, gs:nothing

PUBLIC 

proc RP_Return_Ignore far
mov	al, 0
iret
endp RP_Return_Ignore

;ends seg003

end
