TITLE _s13p27.asm EMM_LBX_AllocNext

.MODEL LARGE, C

EXTRN EMM_Map4_EMMDATAH:PROC
EXTRN farpokew:PROC

EXTRN g_EMM_PageFrame_Base_Address:WORD
EXTRN g_EMMDATAH_Level:WORD

.CODE
;segment seg013 byte public 'CODE' use16
;    assume cs:seg013
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_LBX_AllocNext

proc EMM_LBX_AllocNext

    Free_EMM_Data_Paras = word ptr -2
    nparas = word ptr 6

    push bp
    mov  bp, sp
    sub  sp, 2
    push si
    push di

    mov si, [bp+nparas]
    push cs
    ; call near ptr EMM_Map4_EMMDATAH
    call EMM_Map4_EMMDATAH
    mov ax, [g_EMM_PageFrame_Base_Address]
    add ax, [g_EMMDATAH_Level]
    mov di, ax
    push cs
    ; call near ptr EMM_Map4_EMMDATAH
    call EMM_Map4_EMMDATAH
    mov ax, si
    inc ax
    mov si, ax
    mov ax, 0FFAh
    sub ax, [g_EMMDATAH_Level]
    mov [bp+Free_EMM_Data_Paras], ax
    cmp [bp+Free_EMM_Data_Paras], si
    jge short loc_17B3B
    mov ax, si
    sub ax, [bp+Free_EMM_Data_Paras]
    push ax
    mov ax, Alloc_EMM_too_small
    push ax
    call LBX_Alloc_Error
    pop cx
    pop cx
loc_17B3B:
    mov ax, LBX_MemSig1
    push ax
    mov ax, LBX_Alloc_Space_Header.MemSig1
    push ax
    push di
    call farpokew
    add sp, 6
    mov ax, LBX_MemSig2
    push ax
    mov ax, LBX_Alloc_Space_Header.MemSig2
    push ax
    push di
    call farpokew
    add sp, 6
    mov ax, si
    dec ax
    push ax
    mov ax, LBX_Alloc_Space_Header.Size_Paras
    push ax
    push di
    call farpokew
    add sp, 6
    mov ax, 1
    push ax
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push di
    call farpokew
    add sp, 6
    mov ax, 1
    push ax
    mov ax, LBX_Alloc_Space_Header.Marked_Paras
    push ax
    push di
    call farpokew
    add sp, 6
    mov ax, si
    inc ax
    add [g_EMMDATAH_Level], ax
    mov ax, di
    inc ax
    jmp short $+2
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp EMM_LBX_AllocNext

;ends seg013

end
