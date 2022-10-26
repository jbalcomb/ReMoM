
; ? VGADATAH ?
; 5 EMM Pages, 5 * 64 = 81,920 bytes

; argFilename = "FONTS.LBX"
; strcpy(Font_FileName, argFilename)    ; Font_FileName[16] dseg:A7E0

;Font_Data_Seg@
;Border_Data_Seg@
;LBX_Palette_Seg@
;UU_DAC_Save_Seg
;Replacement_Colors@
;VGA_FILEH_Hdr
;Intensity_Scale_Tbl@
;VGA_DAC_Segment
;VGA_DACmod_Segment@

;LBXE_LoadSingle
;LBX_Alloc_Space
;POKE_Byte
;VGA_TextDraw_Init

;LBXE_LoadSingle(char *LbxFileName, int EntryIndex)
;Font_Data_Seg = LBXE_LoadSingle(FONTS.LBX, 0)
;Border_Data_Seg = LBXE_LoadSingle(FONTS.LBX, 1)


proc VGA_DAC_Init far

    argFilename = word ptr 6

    push bp
    mov bp, sp
    push si
    push di
    mov di, [bp+argFilename]
    push di
    mov ax, offset Font_FileName
    push ax
    call strcpy
    pop cx
    pop cx

    xor ax, ax
    push ax
    push di
    call LBXE_LoadSingle
    pop cx
    pop cx

    mov [Font_Data_Seg@], ax

    mov ax, 1
    push ax
    push di
    call LBXE_LoadSingle
    pop cx
    pop cx

    mov [Border_Data_Seg@], ax

    mov ax, 15Ch
    push ax
    call LBX_Alloc_Space
    pop cx

    mov [LBX_Palette_Seg@], ax

    mov ax, 40h
    push ax
    call LBX_Alloc_Space
    pop cx

    mov [VGA_DAC_Segment], ax

    mov ax, [VGA_DAC_Segment]
    add ax, 30h
    mov [VGA_DACmod_Segment@], ax
    mov ax, 30h
    push ax
    call LBX_Alloc_Space
    pop cx

    mov [UU_DAC_Save_Seg], ax

    mov ax, 180h
    push ax
    call LBX_Alloc_Space
    pop cx

    mov [Replacement_Colors@], ax

    mov ax, 2
    push ax
    call LBX_Alloc_Space
    pop cx

    mov [VGA_FILEH_Hdr], ax
    mov ax, 60h
    push ax
    call LBX_Alloc_Space
    pop cx
    
    mov [Intensity_Scale_Tbl@], ax
    call VGA_TextDraw_Init

    xor si, si
    jmp short loc_17F06

    ; ---------------------------------------------------------------------------

loc_17EF5:
    xor ax, ax
    push ax
    push si
    push [VGA_DAC_Segment]
    call POKE_Byte
    add sp, 6

    inc si

loc_17F06:
    cmp si, 300h
    jl short loc_17EF5

    xor si, si
    jmp short loc_17F22

    ; ---------------------------------------------------------------------------

loc_17F10:
    mov ax, 1
    push ax
    push si
    push [VGA_DACmod_Segment@]
    call POKE_Byte
    add sp, 6

    inc si

loc_17F22:
    cmp si, 100h
    jl short loc_17F10

    pop di
    pop si
    pop bp
    retf

endp VGA_DAC_Init
