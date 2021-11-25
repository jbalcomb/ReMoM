TITLE _s13p11.asm EMM_LBX_EntryLoader

.MODEL LARGE

LBX_Alloc_Space_Header

segment dseg para public 'DATA' use16
ends dseg

LXLSH@
LDIV@
LMOD@
CRP_LBX_farmalloc2
_LBX_Alloc_Validate
_LBX_Alloc_GetFree
EMM_MapnRead
EMM_CheckHandleOpen
_EMM_LBX_HdrOnly
_LBX_Error
_PEEK_Word

segment seg013 byte public 'CODE' use16
    assume cs:seg013
    ;org 7
    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC _EMM_LBX_EntryLoader

proc _EMM_LBX_EntryLoader far

    Header_Offset= dword ptr -18h
    Total_Bytes_Left= dword ptr -14h
    Entry_Offset= dword ptr -10h
    Amt_Paragraphs= word ptr -0Ch
    Load_Segment= word ptr -0Ah
    Bytes_To_Copy= word ptr -8
    Resource_Type= word ptr -6
    Entry_Count= word ptr -4
    EMM_Handle= word ptr -2
    Hnd_Name= word ptr 6
    Entry_Index= word ptr 8
    LBX_Alloc_Seg= word ptr 0Ah
    Load_Type= word ptr 0Ch
    Format= word ptr 0Eh

    push bp
    mov bp, sp
    sub sp, 18h
    push si
    push di

    mov si, [bp+LBX_Alloc_Seg]
    xor di, di
    push [bp+Hnd_Name]
    nop
    push cs
    call near ptr _EMM_CheckHandleOpen
    pop cx
    mov [bp+EMM_Handle], ax
    cmp [bp+EMM_Handle], 0
    jnz short loc_16CC9
    jmp loc_16F3A
loc_16CC9:
    cmp [LBX_LastLoadReserved], 1
    jnz short loc_16D24
    push [bp+EMM_Handle]
    mov ax, 2
    push ax
    xor ax, ax
    mov dx, 6
    push ax
    push dx
    xor ax, ax
    push ax
    lea ax, [bp+Resource_Type]
    push ax
    call _EMM_MapnRead
    add sp, 0Ch
    cmp [bp+Resource_Type], 0
    jz short loc_16D07
    push [bp+Entry_Index]
    mov ax, LBXErr_cantload_reserved
    push ax
    push [bp+Hnd_Name@]
    call _LBX_Error
    add sp, 6
    jmp short loc_16D21
loc_16D07:
    push [bp+Format]
    push [bp+Load_Type]
    push si
    push [bp+Entry_Index]
    push [bp+Hnd_Name@]
    push [bp+EMM_Handle]
    nop
    push cs
    call _EMM_LBX_HdrOnly
    add sp, 0Ch
    mov di, ax
loc_16D21:
    jmp loc_16F3A
    loc_16D24:
    push [bp+EMM_Handle]
    mov ax, 2
    push ax
    xor ax, ax
    push ax
    xor ax, ax
    push ax
    xor ax, ax
    push ax
    lea ax, [bp+Entry_Count]
    push ax
    call _EMM_MapnRead
    add sp, 0Ch
    mov ax, [bp+Entry_Index]
    cmp ax, [bp+Entry_Count]
    jl short loc_16D4D
loc_16D48:
    xor ax, ax
loc_16D4A:
    jmp loc_16F3F
loc_16D4D:
    cmp [bp+Format], 0
    jnz short loc_16D63
    mov ax, [bp+Entry_Index]
    cwd
    mov cl, 2
    call LXLSH@
    add ax, 8
    jmp short loc_16D71
loc_16D63:
    mov ax, [bp+Entry_Index]
    cwd
    mov cl, 2
    call LXLSH@
    add ax, 208h
loc_16D71:
    adc dx, 0
    mov [word ptr bp+Header_Offset+2], dx
    mov [word ptr bp+Header_Offset], ax
    push [bp+EMM_Handle]
    mov ax, 4
    push ax
    push [word ptr bp+Header_Offset+2]
    push [word ptr bp+Header_Offset]
    xor ax, ax
    push ax
    lea ax, [bp+Entry_Offset]
    push ax
    call _EMM_MapnRead
    add sp, 0Ch
    push [bp+EMM_Handle]
    mov ax, 4
    push ax
    mov ax, [word ptr bp+Header_Offset+2]
    mov dx, [word ptr bp+Header_Offset]
    add dx, 4
    adc ax, 0
    push ax
    push dx
    xor ax, ax
    push ax
    lea ax, [bp+Total_Bytes_Left]
    push ax
    call _EMM_MapnRead
    add sp, 0Ch
    mov ax, [word ptr bp+Entry_Offset+2]
    mov dx, [word ptr bp+Entry_Offset]
    sub [word ptr bp+Total_Bytes_Left], dx
    sbb [word ptr bp+Total_Bytes_Left+2], ax
    cmp [word ptr bp+Total_Bytes_Left+2], 0
    jge short loc_16DCF
    jmp loc_16F3A
loc_16DCF:
    jg short loc_16DDA
    cmp [word ptr bp+Total_Bytes_Left], 0
    ja short loc_16DDA
    jmp loc_16F3A
loc_16DDA:
    xor ax, ax
    mov dx, 10h
    push ax
    push dx
    push [word ptr bp+Total_Bytes_Left+2]
    push [word ptr bp+Total_Bytes_Left]
    call LDIV@
    mov [bp+Amt_Paragraphs], ax
    test [bp+Amt_Paragraphs], 0Fh
    jz short loc_16DF9
    inc [bp+Amt_Paragraphs]
loc_16DF9:
    mov ax, [bp+Load_Type]
    or ax, ax
    jz short loc_16E0D
    cmp ax, 1
    jz short loc_16E22
    cmp ax, 2
    jz short loc_16E5E
    jmp loc_16EAC
loc_16E0D:
    push [bp+Amt_Paragraphs]
    call CRP_LBX_farmalloc2
    pop cx
    mov di, ax
    or di, di
    jnz short loc_16E1F
    jmp loc_16D48
loc_16E1F:
    jmp loc_16EAC
loc_16E22:
    push si
    call _LBX_Alloc_Validate
    pop cx
    or ax, ax
    jnz short loc_16E30
    jmp loc_16D48
loc_16E30:
    mov ax, LBX_Alloc_Space_Header.Size_Paras
    push ax
    push si
    call _PEEK_Word
    pop cx
    pop cx
    dec ax
    cmp ax, [bp+Amt_Paragraphs]
    jnb short loc_16E45
    jmp loc_16D48
loc_16E45:
    mov ax, si
    inc ax
    mov di, ax
    mov ax, [bp+Amt_Paragraphs]
    inc ax
    push ax
loc_16E4F:
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push si
    call _POKE_Word
    add sp, 6
    jmp short loc_16EAC
loc_16E5E:
    or si, si
    jnz short loc_16E67
    mov ax, 1
    jmp short loc_16E69
loc_16E67:
    xor ax, ax
loc_16E69:
    push ax
    call LBX_Alloc_Validate
    pop cx
    or ax, ax
    jz short loc_16E77
    jmp loc_16D48
loc_16E77:
    push si
    call _LBX_Alloc_GetFree
    pop cx
    cmp ax, [bp+Amt_Paragraphs]
    jnb short loc_16E86
    jmp loc_16D48
loc_16E86:
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push si
    call _PEEK_Word
    pop cx
    pop cx
    mov dx, si
    add dx, ax
    mov di, dx
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push si
    call _PEEK_Word
    pop cx
    pop cx
    mov dx, [bp+Amt_Paragraphs]
    add dx, ax
    push dx
    jmp short loc_16E4F
loc_16EAC:
    or di, di
    jnz short loc_16EB3
    jmp loc_16F3A
loc_16EB3:
    mov [bp+Load_Segment], di
    mov [bp+Bytes_To_Copy], 8000h
    jmp short loc_16EEE
loc_16EBD:
    sub [word ptr bp+Total_Bytes_Left], 8000h
    sbb [word ptr bp+Total_Bytes_Left+2], 0
    push [bp+EMM_Handle]
    push [bp+Bytes_To_Copy]
    push [word ptr bp+Entry_Offset+2]
    push [word ptr bp+Entry_Offset]
    push [bp+Load_Segment]
    xor ax, ax
    push ax
    call _EMM_MapnRead
    add sp, 0Ch
    add [word ptr bp+Entry_Offset], 8000h
    adc [word ptr bp+Entry_Offset+2], 0
    add [bp+Load_Segment], 800h
loc_16EEE:
    cmp [word ptr bp+Total_Bytes_Left+2], 0
    jg short loc_16EBD
    jnz short loc_16EFD
    cmp [word ptr bp+Total_Bytes_Left], 8000h
    ja short loc_16EBD
loc_16EFD:
    cmp [word ptr bp+Total_Bytes_Left+2], 0
    jl short loc_16F3A
    jg short loc_16F0B
    cmp [word ptr bp+Total_Bytes_Left], 0
    jbe short loc_16F3A
loc_16F0B:
    xor ax, ax
    mov dx, 8000h
    push ax
    push dx
    push [word ptr bp+Total_Bytes_Left+2]
    push [word ptr bp+Total_Bytes_Left]
    call LMOD@
    mov [bp+Bytes_To_Copy], ax
    push [bp+EMM_Handle]
    push [bp+Bytes_To_Copy]
    push [word ptr bp+Entry_Offset+2]
    push [word ptr bp+Entry_Offset]
    push [bp+Load_Segment]
    xor ax, ax
    push ax
    call _EMM_MapnRead
    add sp, 0Ch
loc_16F3A:
    mov ax, di
    jmp loc_16D4A
loc_16F3F:
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp _EMM_LBX_EntryLoader

ends seg013

end
