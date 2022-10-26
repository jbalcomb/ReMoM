TITLE _s13p04.asm EMM_Load_LBX

.MODEL LARGE, C

.CODE
;segment seg013 byte public 'CODE' use16
;    assume cs:seg013
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_Load_LBX

proc EMM_Load_LBX

    Handle_Name= byte ptr -3Eh
    LBX_FileName= byte ptr -2Ah
    Bytes_Copied= dword ptr -16h
    Bytes_To_Copy= dword ptr -12h
    Target_Logical_Page= word ptr -0Eh
    Amt_Bytes= word ptr -0Ch
    Load_Segment= word ptr -0Ah
    Page_Count= word ptr -8
    Handle_Exists= word ptr -6
    EMM_Handle= word ptr -4
    File_Handle= word ptr -2
    FileName@= word ptr 6
    Reserved= word ptr 8

    push bp
    mov  bp, sp
    sub  sp, 3Eh
    push si
    push di

    push [bp+FileName@]
    call STR_StripFileExt
    pop cx
    push [bp+FileName@]
    lea ax, [bp+Handle_Name]
    push ax
    call strcpy
    pop cx
    pop cx
    xor si, si
    jmp short loc_1662F
loc_1662E:
    inc si
loc_1662F:
    cmp [bp+si+Handle_Name], 0
    jnz short loc_1662E
    mov di, si
    jmp short loc_1663E
loc_16639:
    mov [bp+di+Handle_Name], 0
    inc di
loc_1663E:
    cmp di, 9
    jl short loc_16639
    mov [bp+Handle_Exists], 0
    xor si, si
    jmp short loc_1666C
loc_1664C:
    lea ax, [bp+Handle_Name]
    push ax
    mov ax, si
    mov dx, 0Ch
    imul dx
    add ax, offset EMM_Table.HandleName
    push ax
    call stricmp
    pop cx
    pop cx
    or ax, ax
    jnz short loc_1666B
    mov [bp+Handle_Exists], 1
loc_1666B:
    inc si
loc_1666C:
    cmp si, [EMM_Open_Handles]
    jl short loc_1664C
    cmp [bp+Handle_Exists], 0
    jz short loc_1668B
    xor ax, ax
    push ax
    mov ax, LBXErr_load_reg_first
    push ax
    lea ax, [bp+Handle_Name]
    push ax
    call LBX_Error
    add sp, 6
loc_1668B:
    cmp [EMM_OK], 0
    jnz short loc_16695
    jmp loc_16813
loc_16695:
    cmp [EMM_Open_Handles], 28h
    jl short loc_1669F
    jmp loc_16813
loc_1669F:
    lea ax, [bp+Handle_Name]
    push ax
    lea ax, [bp+LBX_FileName]
    push ax
    call strcpy
    pop cx
    pop cx
    mov ax, offset cnst_LBX_Ext2
    push ax
    lea ax, [bp+LBX_FileName]
    push ax
    call strcat
    pop cx
    pop cx
    lea ax, [bp+LBX_FileName]
    push ax
    call DISK_GetFileSize
    pop cx
    mov [word ptr bp+Bytes_To_Copy+2], dx
    mov [word ptr bp+Bytes_To_Copy], ax
    mov ax, [word ptr bp+Bytes_To_Copy]
    or ax, [word ptr bp+Bytes_To_Copy+2]
    jnz short loc_166D8
    jmp loc_16813
loc_166D8:
    xor ax, ax
    mov dx, 4000h
    push ax
    push dx
    push [word ptr bp+Bytes_To_Copy+2]
    push [word ptr bp+Bytes_To_Copy]
    call LDIV@
    mov [bp+Page_Count], ax
    xor ax, ax
    mov dx, 4000h
    push ax
    push dx
    push [word ptr bp+Bytes_To_Copy+2]
    push [word ptr bp+Bytes_To_Copy]
    call LMOD@
    or ax, dx
    jz short loc_16706
    inc [bp+Page_Count]
loc_16706:
    cmp [bp+Reserved], 0
    jnz short loc_16721
    call EMM_GetFreePageCnt
    mov dx, [EMM_Reserved_Left]
    add dx, [bp+Page_Count]
    cmp ax, dx
    jnb short loc_16721
    xor ax, ax
loc_1671E:
    jmp loc_16819
loc_16721:
    push [bp+Reserved]
    lea ax, [bp+Handle_Name]
    push ax
    push [bp+Page_Count]
    nop
    push cs
    call near ptr EMM_CreateHandle
    add sp, 6
    mov [bp+EMM_Handle], ax
    cmp [bp+EMM_Handle], 0
    jnz short loc_1673F
    jmp loc_167FB
loc_1673F:
    call EMM_GetPageFrame
    mov [bp+Load_Segment], ax
    mov [bp+Target_Logical_Page], 0
    cmp [bp+Load_Segment], 0
    jnz short loc_16755
    jmp loc_167F9
loc_16755:
    lea ax, [bp+LBX_FileName]
    push ax
    call DISK_OpenFile
    pop cx
    mov [bp+File_Handle], ax
    cmp [bp+File_Handle], 0
    jnz short loc_1676B
    jmp loc_167F9
loc_1676B:
    mov [word ptr bp+Bytes_Copied+2], 0
    mov [word ptr bp+Bytes_Copied], 0
    mov [bp+Amt_Bytes], 4000h
    jmp short loc_167AF
loc_1677C:
    sub [word ptr bp+Bytes_To_Copy], 4000h
    sbb [word ptr bp+Bytes_To_Copy+2], 0
    push [bp+Target_Logical_Page]
    push [bp+EMM_Handle]
    call EMM_MapFourPages
    pop cx
    pop cx
    push [bp+File_Handle]
    push [bp+Amt_Bytes]
    push [bp+Load_Segment]
    call DISK_ReadToSeg
    add sp, 6
    inc [bp+Target_Logical_Page]
    add [word ptr bp+Bytes_Copied], 4000h
    adc [word ptr bp+Bytes_Copied+2], 0
loc_167AF:
    cmp [word ptr bp+Bytes_To_Copy+2], 0
    jg short loc_1677C
    jl short loc_167BE
    cmp [word ptr bp+Bytes_To_Copy], 4000h
    jnb short loc_1677C
loc_167BE:
    cmp [word ptr bp+Bytes_To_Copy+2], 0
    jl short loc_167F0
    jg short loc_167CC
    cmp [word ptr bp+Bytes_To_Copy], 0
    jbe short loc_167F0
loc_167CC:
    mov ax, [word ptr bp+Bytes_To_Copy]
    mov [bp+Amt_Bytes], ax
    push [bp+Target_Logical_Page]
    push [bp+EMM_Handle]
    call EMM_MapFourPages
    pop cx
    pop cx
    push [bp+File_Handle]
    push [bp+Amt_Bytes]
    push [bp+Load_Segment]
    call DISK_ReadToSeg
    add sp, 6
loc_167F0:
    push [bp+File_Handle]
    call DISK_CloseFile
    pop cx
loc_167F9:
    jmp short loc_16813
loc_167FB:
    cmp [bp+Reserved], 1
    jnz short loc_16813
    xor ax, ax
    push ax
    mov ax, LBXErr_EM_swap_fail
    push ax
    push [bp+FileName@]
    call LBX_Error
    add sp, 6
loc_16813:
    mov ax, [bp+EMM_Handle]
    jmp loc_1671E

loc_16819:
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp EMM_Load_LBX

;ends seg013

end
