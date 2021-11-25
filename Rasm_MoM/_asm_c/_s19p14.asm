TITLE _s19p.asm VGA_TextDraw_Init

.MODEL LARGE, C

EXTRN UU_gsa_VGA_TextDraw_Enabled:WORD
EXTRN gsa_VGA_TextLine_Lefts:WORD
EXTRN gsa_VGA_TextLine_Rights:WORD
EXTRN gsa_VGA_TextLine_Starts:WORD
EXTRN gsa_VGA_TextLine_Tops:WORD

EXTRN SA_Allocate_Space:PROC
EXTRN MK_FP_SgmtAddr:PROC


.CODE
;segment seg019 byte public 'CODE' use16
;    assume cs:seg019
;    ;org 0Dh
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_TextDraw_Init

proc VGA_TextDraw_Init far

    push bp
    mov bp, sp
    push si

    mov [UU_VGA_TextDraw_Enabled], 1

    mov ax, 8                           ; 8 paragraphs = 8 * 16 bytes = 128 bytes
    push ax
    call SA_Allocate_Space
    pop cx

    mov si, ax

    push si
    call MK_FP_SgmtAddr
    pop cx

    mov [word ptr VGA_TextLine_Lefts+2], dx
    mov [word ptr VGA_TextLine_Lefts], ax

    mov ax, 8
    push ax
    call SA_Allocate_Space
    pop cx

    mov si, ax

    push si
    call MK_FP_SgmtAddr
    pop cx

    mov [word ptr VGA_TextLine_Rights+2], dx
    mov [word ptr VGA_TextLine_Rights], ax

    mov ax, 8
    push ax
    call SA_Allocate_Space
    pop cx

    mov si, ax

    push si
    call MK_FP_SgmtAddr
    pop cx

    mov [word ptr VGA_TextLine_Tops+2], dx
    mov [word ptr VGA_TextLine_Tops], ax

    mov ax, 8
    push ax
    call SA_Allocate_Space
    pop cx

    mov si, ax

    push si
    call MK_FP_SgmtAddr
    pop cx

    mov [word ptr VGA_TextLine_Starts+2], dx
    mov [word ptr VGA_TextLine_Starts], ax

    pop si
    pop bp
    ret

endp VGA_TextDraw_Init

;ends seg019

end
