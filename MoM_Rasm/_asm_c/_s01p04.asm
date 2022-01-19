TITLE _s01p04.asm GAME_LoadMainImages

.MODEL LARGE, C

.CODE
; Segment type: Pure code
;segment seg001 byte public 'CODE' use16
;    assume cs:seg001
;    org 9
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GAME_LoadMainImages

proc GAME_LoadMainImages

    push bp
    mov bp, sp
    push si
    mov ax, offset cnst_MAINSCRN_File
    push ax
    call EMM_LoadResLBX
    pop cx
    xor ax, ax
    push ax
    mov ax, offset cnst_MAINSCRN_File
    push ax
    call LBXE_LoadSingle
    pop cx
    pop cx
    mov [IMG_MainScr_Title@], ax
    mov ax, 1
    push ax
    mov ax, offset cnst_VORTEX_File
    push ax
    call LBXE_LoadSingle
    pop cx
    pop cx
    mov [IMG_MainScr_Cont@], ax
    mov ax, 2
    push ax
    mov ax, offset cnst_VORTEX_File
    push ax
    call LBXE_LoadSingle
    pop cx
    pop cx
    mov [IMG_MainScr_HoF@], ax
    mov ax, 5
    push ax
    mov ax, offset cnst_VORTEX_File
    push ax
    call LBXE_LoadSingle
    pop cx
    pop cx
    mov [IMG_MainScr_LoadG@], ax
    mov ax, 4
    push ax
    mov ax, offset cnst_VORTEX_File
    push ax
    call LBXE_LoadSingle
    pop cx
    pop cx
    mov [IMG_MainScr_NewG@], ax
    mov ax, 3
    push ax
    mov ax, offset cnst_VORTEX_File
    push ax
    call LBXE_LoadSingle
    pop cx
    pop cx
    mov [IMG_MainScr_Quit@], ax
    mov ax, 5
    push ax
    mov ax, offset cnst_MAINSCRN_File
    push ax
    call LBXE_LoadSingle
    pop cx
    pop cx
    mov [IMG_MainScr_BG@], ax
    mov ax, offset cnst_WIZARDS_File
    push ax
    call EMM_LoadResLBX
    pop cx
    xor si, si
    jmp short loc_137D8
loc_137C3:
    push si
    mov ax, offset cnst_WIZARDS_File
    push ax
    call LBXE_LoadSingle
    pop cx
    pop cx
    mov bx, si
    shl bx, 1
    mov [IMG_Wizards@+bx], ax
    inc si
loc_137D8:
    cmp si, 14
    jl short loc_137C3
    mov ax, offset cnst_SPELLDAT_File
    push ax
    call EMM_LoadResLBX
    pop cx
    pop si
    pop bp
    ret

endp GAME_LoadMainImages

;ends seg001

end
