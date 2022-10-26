TITLE _s28p02.asm FLIC_Prepare
; ST_FLIC.H

.MODEL LARGE, C

EXTRN farpokew:PROC

.CODE
;segment seg028 byte public 'CODE' use16
;	 assume cs:seg028
;	 ;org 1
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC FLIC_Prepare

proc FLIC_Prepare

    FlicSize_B  = word ptr -2
    FlicWidth             = word ptr 6  ; *Warning* Reserved word used as symbol: WIDTH
    FlicHeight            = word ptr 8
    FlicHdr_SgmtAddr  = word ptr 0Ah

    push bp
    mov  bp, sp
    sub  sp, 2
    push si
    push di

    mov si, [bp+FlicWidth]
    push si
    xor ax, ax
    push ax
    push [bp+FlicHdr_SgmtAddr]
    call farpokew
    add sp, 6

    push [bp+FlicHeight]
    ;mov ax, FLIC_HDR.Height
    mov ax, 02h  ; 02h 2d  FLIC_HDR.Height
    push ax
    push [bp+FlicHdr_SgmtAddr]
    call farpokew
    add sp, 6

    ;mov ax, e_FLIC_Decoded
    mov ax, 0DE0Ah  ; DE0A  Magic/Signature: e_FLIC_Decoded
    push ax
    ;mov ax, FLIC_HDR.Current_Frame
    mov ax, 04h  ; 04h 4d  FLIC_HDR.Current_Frame
    push ax
    push [bp+FlicHdr_SgmtAddr]
    call farpokew
    add sp, 6

    xor ax, ax
    push ax
    ;mov ax, FLIC_HDR.Frame_Count
    mov ax, 06h  ; 06h 6d  FLIC_HDR.Frame_Count
    push ax
    push [bp+FlicHdr_SgmtAddr]
    call farpokew
    add sp, 6

    xor ax, ax
    push ax
    ;mov ax, FLIC_HDR.Loop_Frame
    mov ax, 08h  ; 08h 8d  FLIC_HDR.Loop_Frame
    push ax
    push [bp+FlicHdr_SgmtAddr]
    call farpokew
    add sp, 6

    mov ax, si
    imul [bp+FlicHeight]
    mov [bp+FlicSize_B], ax

    push es
    push di

    mov ax, [bp+FlicHdr_SgmtAddr]
    mov es, ax

    mov di, 10h  ; 10h 16d
    mov al, 0
    mov cx, [bp+FlicSize_B]
    rep stosb

    pop di
    pop es

    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp FLIC_Prepare

end
