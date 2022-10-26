TITLE _s10p11.asm LBX_RecordLoader

.MODEL LARGE, C

.CODE
;segment seg010 byte public 'CODE' use16
;    assume cs:seg010
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC LBX_RecordLoader

proc LBX_RecordLoader far

    LBX_NameWithPath = byte ptr -84h
    LBX_FileName = byte ptr -34h
    Load_Size_Bytes = dword ptr -20h
    Next_Entry_In_File = dword ptr -1Ch
    Entry_Offset_In_File = dword ptr -18h
    File_Offset = dword ptr -14h
    LBX_Entry_RecSize = word ptr -10h
    Bytes_To_Copy = word ptr -0Eh
    Load_To_Seg = word ptr -0Ch
    Amt_Paragraphs = word ptr -0Ah
    LBX_Entry_RecCount = word ptr -8
    LBX_Format_Type = word ptr -6
    EntryTableOffset = word ptr -4
    SAMB_data = word ptr -2
    LbxName = word ptr 6
    LbxEntryIndex = word ptr 8
    SAMB_head = word ptr 0Ah
    LoadType = word ptr 0Ch
    RecFirst = word ptr 0Eh
    RecCount = word ptr 10h
    RecSize = word ptr 12h

    push bp
    mov  bp, sp
    sub  sp, 84h
    push si
    push di

    mov di, [bp+LbxName]
    mov si, [bp+LbxEntryIndex]
    or si, si
    jge short loc_15474

    push si
    mov ax, LBXErr_not_found
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15474:
    cmp [LBX_LdHdr_Exists], 0
    jnz short loc_1548E
    mov [LBX_LdHdr_Exists], 1
    mov ax, 20h
    push ax
    call LBX_farmalloc
    pop cx
    mov [LBX_LastLoadHdr@], ax

loc_1548E:
    push di
    nop
    push cs
    call near ptr STR_StripFileExt
    pop cx

    push [bp+RecSize]
    push [bp+RecCount]
    push [bp+RecFirst]
    push [bp+LoadType]
    push [bp+SAMB_head]
    push si
    push di
    call EMM_LBX_RecLoader
    add sp, 0Eh
    mov [bp+SAMB_data], ax
    mov [bp+LBX_Format_Type], 0
    cmp [bp+SAMB_data], 0
    jz short loc_154BF
    jmp @@Done

loc_154BF:
    db 83h,3Eh,68h,3Eh,0FFh ; <BAD>cmp [LBX_LastLoadHnd], 0FFFFh
    jz short loc_154E1
    mov ax, EntryTableOffset LBX_LastLoadName
    push ax
    push di
    call stricmp
    pop cx
    pop cx
    or ax, ax
    jnz short loc_154E1
    mov ax, [LBX_LastLoadFmt]
    cmp ax, [bp+LBX_Format_Type]
    jnz short loc_154E1
    jmp loc_15614

loc_154E1:
    mov ax, [bp+LBX_Format_Type]
    mov [LBX_LastLoadFmt], ax
    db 83h,3Eh,68h,3Eh,0FFh ; <BAD>cmp [LBX_LastLoadHnd], 0FFFFh
    jz short loc_154F8
    push [LBX_LastLoadHnd]
    call DISK_CloseFile
    pop cx

loc_154F8:
    push di
    mov ax, EntryTableOffset LBX_LastLoadName
    push ax
    call strcpy
    pop cx
    pop cx
    push di
    lea ax, [bp+LBX_FileName]
    push ax
    call strcpy
    pop cx
    pop cx
    mov ax, EntryTableOffset cnst_LBX_Ext
    push ax
    lea ax, [bp+LBX_FileName]
    push ax
    call strcat
    pop cx
    pop cx
    lea ax, [bp+LBX_FileName]
    push ax
    call DISK_OpenFile
    pop cx
    mov [LBX_LastLoadHnd], ax
    cmp [LBX_LastLoadHnd], 0
    jnz short loc_1558B
    cmp [LBX_LoadPath], 0
    jnz short loc_15548
    push si
    mov ax, LBXErr_not_found
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15548:
    mov ax, EntryTableOffset LBX_LoadPath
    push ax
    lea ax, [bp+LBX_NameWithPath]
    push ax
    call strcpy
    pop cx
    pop cx
    lea ax, [bp+LBX_FileName]
    push ax
    lea ax, [bp+LBX_NameWithPath]
    push ax
    call strcat
    pop cx
    pop cx
    lea ax, [bp+LBX_NameWithPath]
    push ax
    call DISK_OpenFile
    pop cx
    mov [LBX_LastLoadHnd], ax
    cmp [LBX_LastLoadHnd], 0
    jnz short loc_1558B
    push si
    mov ax, LBXErr_not_found
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_1558B:
    cmp [LBX_LastLoadHnd], 0
    jnz short loc_155A0
    push si
    mov ax, LBXErr_not_found
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_155A0:
    mov [word ptr bp+File_Offset+2], 0
    mov [word ptr bp+File_Offset], 0
    push [LBX_LastLoadHnd]
    push [word ptr bp+File_Offset+2]
    push [word ptr bp+File_Offset]
    call DISK_LSeek
    add sp, 6
    or ax, ax
    jnz short loc_155CD
    push si
    mov ax, LBXErr_corrupted
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    jmp short loc_155DE

loc_155CD:
    push [LBX_LastLoadHnd]
    mov ax, 200h
    push ax
    push [LBX_LastLoadHdr@]
    call DISK_ReadToSeg

loc_155DE:
    add sp, 6
    mov ax, 2
    push ax
    push [LBX_LastLoadHdr@]
    call PEEK_Word
    pop cx
    pop cx
    cmp ax, LBX_FileSig
    jz short loc_15603
    push si
    mov ax, LBXErr_bad_header
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15603:
    xor ax, ax
    push ax
    push [LBX_LastLoadHdr@]
    call PEEK_Word
    pop cx
    pop cx
    mov [LBX_LastLoadECount], ax

loc_15614:
    cmp si, [LBX_LastLoadECount]
    jl short loc_15628
    push si
    mov ax, LBXErr_entries_exceeded
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15628:
    mov ax, si
    mov cl, 2
    shl ax, cl
    add ax, 8
    mov [bp+EntryTableOffset], ax
    
    push [bp+EntryTableOffset]
    push [LBX_LastLoadHdr@]
    call farpeekdw
    pop cx
    pop cx
    mov [word ptr bp+Entry_Offset_In_File+2], dx
    mov [word ptr bp+Entry_Offset_In_File], ax
    mov ax, [bp+EntryTableOffset]
    add ax, 4
    push ax
    push [LBX_LastLoadHdr@]
    call farpeekdw
    pop cx
    pop cx
    mov [word ptr bp+Next_Entry_In_File+2], dx
    mov [word ptr bp+Next_Entry_In_File], ax
    mov ax, [word ptr bp+Next_Entry_In_File+2]
    mov dx, [word ptr bp+Next_Entry_In_File]
    sub dx, [word ptr bp+Entry_Offset_In_File]
    sbb ax, [word ptr bp+Entry_Offset_In_File+2]
    mov [word ptr bp+Load_Size_Bytes+2], ax
    mov [word ptr bp+Load_Size_Bytes], dx
    push [LBX_LastLoadHnd]
    push [word ptr bp+Entry_Offset_In_File+2]
    push [word ptr bp+Entry_Offset_In_File]
    call DISK_LSeek
    add sp, 6
    or ax, ax
    jnz short loc_15699
    push si
    mov ax, LBXErr_corrupted
    push ax
    mov ax, EntryTableOffset LBX_LastLoadName
    push ax
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15699:
    push [LBX_LastLoadHnd]
    mov ax, 2
    push ax
    lea ax, [bp+LBX_Entry_RecCount]
    push ax
    call DISK_ReadToData
    add sp, 6
    push [LBX_LastLoadHnd]
    mov ax, 2
    push ax
    lea ax, [bp+LBX_Entry_RecSize]
    push ax
    call DISK_ReadToData
    add sp, 6
    cmp [bp+LBX_Entry_RecSize], 0
    jz short loc_156CF
    mov ax, [bp+RecSize]
    cmp ax, [bp+LBX_Entry_RecSize]
    jz short loc_156E0

loc_156CF:
    push si
    mov ax, LBXErr_recsize_mismatch
    push ax
    mov ax, EntryTableOffset LBX_LastLoadName
    push ax
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_156E0:
    mov ax, [bp+RecFirst]
    add ax, [bp+RecCount]
    cmp ax, [bp+LBX_Entry_RecCount]
    jbe short loc_156FC
    push si
    mov ax, LBXErr_records_exceeded
    push ax
    mov ax, EntryTableOffset LBX_LastLoadName
    push ax
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_156FC:
    mov bx, [bp+RecFirst]
    xor cx, cx
    mov ax, [bp+LBX_Entry_RecSize]
    xor dx, dx
    call LXMUL@
    add ax, 4
    adc dx, 0
    add [word ptr bp+Entry_Offset_In_File], ax
    adc [word ptr bp+Entry_Offset_In_File+2], dx
    push [LBX_LastLoadHnd]
    push [word ptr bp+Entry_Offset_In_File+2]
    push [word ptr bp+Entry_Offset_In_File]
    call DISK_LSeek
    add sp, 6
    or ax, ax
    jnz short loc_1573E
    push si
    mov ax, LBXErr_corrupted
    push ax
    mov ax, EntryTableOffset LBX_LastLoadName
    push ax
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_1573E:
    mov bx, [bp+RecCount]
    xor cx, cx
    mov ax, [bp+LBX_Entry_RecSize]
    xor dx, dx
    call LXMUL@
    mov [word ptr bp+Load_Size_Bytes+2], dx
    mov [word ptr bp+Load_Size_Bytes], ax
    xor ax, ax
    mov dx, 10h
    push ax
    push dx
    push [word ptr bp+Load_Size_Bytes+2]
    push [word ptr bp+Load_Size_Bytes]
    call LDIV@
    inc ax
    mov [bp+Amt_Paragraphs], ax
    mov ax, [bp+LoadType]
    or ax, ax
    jz short loc_15780
    cmp ax, 1
    jz short loc_157A3
    cmp ax, 2
    jnz short loc_1577D
    jmp loc_15812

loc_1577D:
    jmp loc_15885

loc_15780:
    push [bp+Amt_Paragraphs]
    call CRP_LBX_farmalloc2
    pop cx
    mov [bp+SAMB_data], ax
    cmp [bp+SAMB_data], 0
    jnz short loc_157A0
    push si
    mov ax, LBXErr_low_RAM
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_157A0:
    jmp loc_15885

loc_157A3:
    push [bp+SAMB_head]
    call LBX_Alloc_Validate
    pop cx
    or ax, ax
    jnz short loc_157BE
    push si
    mov ax, LBXErr_corrupted
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_157BE:
    mov ax, 8
    push ax
    push [bp+SAMB_head]
    call PEEK_Word
    pop cx
    pop cx
    dec ax
    cmp ax, [bp+Amt_Paragraphs]
    jnb short loc_157F5
    mov ax, 8
    push ax
    push [bp+SAMB_head]
    call PEEK_Word
    pop cx
    pop cx
    mov dx, [bp+Amt_Paragraphs]
    sub dx, ax
    inc dx
    push dx
    push si
    mov ax, LBXErr_alloc_fail
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 8

loc_157F5:
    mov ax, [bp+SAMB_head]
    inc ax
    mov [bp+SAMB_data], ax
    mov ax, [bp+Amt_Paragraphs]
    inc ax
    push ax

loc_15801:
    mov ax, 0Ah
    push ax
    push [bp+SAMB_head]
    call POKE_Word
    add sp, 6
    jmp short loc_15885

loc_15812:
    push [bp+SAMB_head]
    call LBX_Alloc_Validate
    pop cx
    or ax, ax
    jnz short loc_1582D
    push si
    mov ax, LBXErr_corrupted
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_1582D:
    push [bp+SAMB_head]
    call LBX_Alloc_GetFree
    pop cx
    cmp ax, [bp+Amt_Paragraphs]
    jnb short loc_15858
    push [bp+SAMB_head]
    call LBX_Alloc_GetFree
    pop cx
    mov dx, [bp+Amt_Paragraphs]
    sub dx, ax
    push dx
    push si
    mov ax, LBXErr_reload_fail
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 8

loc_15858:
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push [bp+SAMB_head]
    call PEEK_Word
    pop cx
    pop cx
    mov dx, [bp+SAMB_head]
    add dx, ax
    mov [bp+SAMB_data], dx
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push [bp+SAMB_head]
    call PEEK_Word
    pop cx
    pop cx
    mov dx, [bp+Amt_Paragraphs]
    add dx, ax
    push dx
    jmp loc_15801

loc_15885:
    mov ax, [bp+SAMB_data]
    mov [bp+Load_To_Seg], ax
    mov [bp+Bytes_To_Copy], 8000h
    jmp short loc_158C4

loc_15892:
    sub [word ptr bp+Load_Size_Bytes], 8000h
    sbb [word ptr bp+Load_Size_Bytes+2], 0
    push [LBX_LastLoadHnd]
    push [bp+Bytes_To_Copy]
    push [bp+Load_To_Seg]
    call DISK_ReadToSeg
    add sp, 6
    or ax, ax
    jnz short loc_158BF
    push si
    mov ax, LBXErr_corrupted
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_158BF:
    add [bp+Load_To_Seg], 800h

loc_158C4:
    cmp [word ptr bp+Load_Size_Bytes+2], 0
    jg short loc_15892
    jl short loc_158D3
    cmp [word ptr bp+Load_Size_Bytes], 8000h
    jnb short loc_15892

loc_158D3:
    cmp [word ptr bp+Load_Size_Bytes+2], 0
    jl short @@Done
    jg short loc_158E1
    cmp [word ptr bp+Load_Size_Bytes], 0
    jbe short @@Done

loc_158E1:
    mov ax, [word ptr bp+Load_Size_Bytes]
    mov [bp+Bytes_To_Copy], ax
    push [LBX_LastLoadHnd]
    push [bp+Bytes_To_Copy]
    push [bp+Load_To_Seg]
    call DISK_ReadToSeg
    add sp, 6
    or ax, ax
    jnz short @@Done
    push si
    mov ax, LBXErr_corrupted
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

@@Done:
    call RAM_Update_WorstFree
    mov ax, [bp+SAMB_data]

    jmp short $+2
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp LBX_RecordLoader

;ends seg010

end
