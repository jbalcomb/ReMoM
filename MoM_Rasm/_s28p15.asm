TITLE _s28p15.asm FLIC_ResetFrame
; ST_FLIC.H

.MODEL LARGE, C

EXTRN farpokew:PROC

.CODE
;segment seg028 byte public 'CODE' use16
;	 assume cs:seg028
;	 ;org 1
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC FLIC_ResetFrame

proc FLIC_ResetFrame

    FlicHdr_SgmtAddr = word ptr 6

    push bp
    mov  bp, sp

    xor ax, ax
    push ax
    ;mov ax, FLIC_HDR.Current_Frame
    mov ax, 04h  ; 04h 4d  FLIC_HDR.Current_Frame
    push ax
    push [bp+FlicHdr_SgmtAddr]
    call farpokew
    add sp, 6
    
    pop bp
    ret

endp FLIC_ResetFrame

;ends seg028

end
