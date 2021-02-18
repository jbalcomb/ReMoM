

;; Segment type:	Pure code
;segment	seg001 byte public 'CODE' use16
;    assume cs:seg001
;    ;org 9
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing,	gs:nothing
;    assume es:nothing, ss:nothing, fs:nothing, gs:nothing
;    assume ds:deg001

segment deg001 word public 'DATA'
;    bios_time_lw dw	0
;    bios_time_hw dw	0
ends deg001



;seg000 segment byte public 'CODE' use16 
;seg000 ends
;
;seg001 segment para public use16
;seg001 ends
;
;seg002 segment para public use16
;seg002 ends
;
;seg003 segment use16 stack
;seg003 ends
