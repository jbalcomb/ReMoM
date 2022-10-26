TITLE _s28p16.asm FLIC_GetCurFrame
; ST_FLIC.H

.MODEL LARGE, C

EXTRN farpeekw:PROC

.CODE
;segment seg028 byte public 'CODE' use16
;	 assume cs:seg028
;	 ;org 1
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC FLIC_GetCurFrame

proc FLIC_GetCurFrame

    FlicHdr_SgmtAddr = word ptr 6

    push bp
    mov  bp, sp

    ;mov ax, FLIC_HDR.Current_Frame
    mov ax, 04h  ; 04h 4d  FLIC_HDR.Current_Frame
    push ax
    push [bp+FlicHdr_SgmtAddr]
    call farpeekw
    pop cx
    pop cx

    jmp short $+2

    pop bp
    ret

endp FLIC_GetCurFrame

;ends seg028

end
