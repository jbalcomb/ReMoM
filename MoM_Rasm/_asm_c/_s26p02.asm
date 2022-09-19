TITLE _s26p02.asm VGA_SetDrawFrame
; ST_VGA.H

.MODEL LARGE, C

EXTRN gsa_DSP_Addr:WORD
EXTRN g_RSP_Idx:WORD

.CODE
;segment seg026 byte public 'CODE' use16
;    assume cs:seg026
;    ;org 0Ah
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_Set_DSP_Addr

proc VGA_Set_DSP_Addr
    
    mov ax, 1
    sub ax, [g_RSP_Idx]
    mov ah, al
    xor al, al
    shl ax, 1
    shl ax, 1
    add ax, 0A000h                          ; e_GRAPHICS_MODE_VIDEO_RAM
    mov [gsa_DSP_Addr], ax
    ret
    
endp VGA_Set_DSP_Addr

;ends seg026

end
