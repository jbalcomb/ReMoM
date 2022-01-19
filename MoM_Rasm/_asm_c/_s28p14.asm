TITLE _s28p1.asm FLIC_SetFrame
; ST_FLIC.H

.MODEL LARGE, C

EXTRN farpeekw:PROC
EXTRN farpokew:PROC

.CODE
;segment seg028 byte public 'CODE' use16
;	 assume cs:seg028
;	 ;org 1
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC FLIC_SetFrame

proc FLIC_SetFrame

    Loop_Length       = word ptr -6
    Loop_Frame        = word ptr -4
    Frame_Count       = word ptr -2
    FlicHdr_SgmtAddr  = word ptr 6
    Frame_Index       = word ptr 8

    push bp
    mov  bp, sp
    sub sp, 6
    push si
    push di

    mov di, [bp+FlicHdr_SgmtAddr]

    mov si, [bp+Frame_Index]
    mov ax, si
    and ax, 7FFFh  ; 7FFFh 32767d 0111111111111111b
    mov si, ax

    ;mov ax, FLIC_HDR.Frame_Count
    mov ax, 06h  ; 06h 6d  FLIC_HDR.Frame_Count
    push ax
    push di
    call farpeekw
    pop cx
    pop cx

    mov [bp+Frame_Count], ax

    ;mov ax, LBX_IMG_HDR.Loop_Frame
    mov ax, 08h  ; 08h 8d  LBX_IMG_HDR.Loop_Frame
    push ax
    push di
    call farpeekw
    pop cx
    pop cx

    mov [bp+Loop_Frame], ax
    mov ax, [bp+Frame_Count]
    sub ax, [bp+Loop_Frame]
    mov [bp+Loop_Length], ax

    cmp si, [bp+Frame_Count]
    jb short loc_1FAC0

    mov ax, si
    sub ax, [bp+Frame_Count]
    xor dx, dx
    div [bp+Loop_Length]
    mov ax, [bp+Loop_Frame]
    add ax, dx
    mov si, ax

loc_1FAC0:
    push si
    ;mov ax, LBX_IMG_HDR.Current_Frame
    mov ax, 04h  ; 04h 4d  LBX_IMG_HDR.Current_Frame
    push ax
    push di
    call farpokew
    add sp, 6

    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp FLIC_SetFrame


;ends seg028

end
