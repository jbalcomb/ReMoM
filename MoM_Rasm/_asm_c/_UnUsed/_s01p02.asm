TITLE _s01p0.asm 

.MODEL LARGE, C

.CODE
; Segment type: Pure code
;segment seg001 byte public 'CODE' use16
;    assume cs:seg001
;    org 9
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc SND_Empty_Fn far
push bp
mov bp, sp
pop bp
retf
endp SND_Empty_Fn

;ends seg001

end
