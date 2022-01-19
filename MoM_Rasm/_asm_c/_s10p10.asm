TITLE   _s10p10.asm LBX_EntryLoader

.MODEL LARGE, C

EXTRN LBX_LdHdr_Exists:WORD
EXTRN LBX_LastLoadHdr:WORD
EXTRN G_LBX_Force_LoadFmt:WORD

EXTRN strcat:PROC
EXTRN strcpy:PROC
EXTRN stricmp:PROC
EXTRN ST_AllocMem:PROC ;_s08p08.asm
EXTRN StripFileExt:PROC ; _s10p16.asm
EXTRN EMM_LBX_EntryLoader: PROC ; 

.CODE
;segment seg010 byte public 'CODE' use16
;    assume cs:seg010
;    assume ds:dseg
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC LBX_EntryLoader

proc LBX_EntryLoader far

    LBX_NameWithPath= byte ptr -6Ch
    LBX_FileName= byte ptr -30h
    Entry_Size_Bytes= dword ptr -1Ch
    Next_Entry_In_File= dword ptr -18h
    Entry_Off_In_File= dword ptr -14h
    File_Offset= dword ptr -10h
    Bytes_To_Copy= word ptr -0Ch
    Load_To_Seg= word ptr -0Ah
    Entry_Size_Paras= word ptr -8
    LBX_File_Format= word ptr -6
    MemberOffset= word ptr -4
    LBX_Data_Seg= word ptr -2
    LbxFileName = word ptr 6
    LbxEntryIndex = word ptr 8
    LBX_Alloc_Seg = word ptr 0Ah
    Load_Type = word ptr 0Ch    ; 0 - single, 1 - replace, 2 - add, others - crash
    Format_Type = word ptr 0Eh  ; 0 - entry addresses start at 0:8, 1 - entry addresses start at 20:8

    push bp
    mov bp, sp
    sub sp, 6Ch

    push si
    push di

    mov si, [bp+LbxFileName]
    mov di, [bp+LbxEntryIndex]
; --------------------------------------SKIP_CODE: LBX ERROR
;    or di, di
;    jge short loc_1505D
;    push di
;    mov ax, LBXErr_not_found
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 6
;loc_1505D:
; --------------------------------------SKIP_CODE: LBX ERROR
    cmp [LBX_LdHdr_Exists], 0
    jnz short loc_15077
    mov [LBX_LdHdr_Exists], 1
    mov ax, 20h
    push ax
    call ST_AllocMem
    add sp, 02h                         ;pop cx

    mov [LBX_LastLoadHdr], ax
loc_15077:
    push si
    ;nop
    ;push cs
    call near ptr StripFileExt
    add sp, 02h                         ;pop cx

    cmp [bp+Format_Type], 0
    jz short loc_1508B
    cmp [G_LBX_Force_LoadFmt], 2
    jnz short loc_15092
loc_1508B:
    mov [bp+LBX_File_Format], 0
    jmp short loc_15097
loc_15092:
    mov [bp+LBX_File_Format], 1
loc_15097:
    push [bp+LBX_File_Format]
    push [bp+Load_Type]
    push [bp+LBX_Alloc_Seg]
    push di
    push si
    call EMM_LBX_EntryLoader
    add sp, 0Ah
    mov [bp+LBX_Data_Seg], ax
;    cmp [bp+LBX_Data_Seg], 0
;    jz short loc_150B6
;    jmp loc_15443
;loc_150B6:
;    cmp  [LBX_LastLoadHnd], 0FFFFh ; db 83h,3Eh,68h,3Eh,0FFh ; <BAD>cmp     [LBX_LastLoadHnd], 0FFFFh
;    jz short loc_150D8
;    mov ax, offset LBX_LastLoadName
;    push ax
;    push si
;    call _stricmp
;    add sp, 04h                         ;pop cx, pop cx
;    or ax, ax
;    jnz short loc_150D8
;    mov ax, [LBX_LastLoadFmt]
;    cmp ax, [bp+LBX_File_Format]
;    jnz short loc_150D8
;    jmp loc_15206
;loc_150D8:
;    mov ax, [bp+LBX_File_Format]
;    mov [LBX_LastLoadFmt], ax
;    cmp     [LBX_LastLoadHnd], 0FFFFh ; db 83h,3Eh,68h,3Eh,0FFh ; <BAD>cmp     [LBX_LastLoadHnd], 0FFFFh
;    jz short loc_150EF
;    push [LBX_LastLoadHnd]
;    call DISK_CloseFile
;    pop cx
;loc_150EF:
;    push si
;    mov ax, offset LBX_LastLoadName
;    push ax
;    call _strcpy
;    add sp, 04h                         ;pop cx, pop cx
;    push si
;    lea ax, [bp+LBX_FileName]
;    push ax
;    call _strcpy
;    add sp, 04h                         ;pop cx, pop cx
;    mov ax, offset cnst_LBX_Ext
;    push ax
;    lea ax, [bp+LBX_FileName]
;    push ax
;    call _strcat
;    add sp, 04h                         ;pop cx, pop cx
;    lea ax, [bp+LBX_FileName]
;    push ax
;    call DISK_OpenFile
;    add sp, 02h                         ;pop cx
;    mov [LBX_LastLoadHnd], ax
; --------------------------------------SKIP_CODE: LBX ERROR
;    cmp [LBX_LastLoadHnd], 0
;    jnz short loc_1517F
;    cmp [LBX_LoadPath], 0
;    jnz short loc_1513F
;    push di
;    mov ax, LBXErr_not_found
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 6
;loc_1513F:
; --------------------------------------SKIP_CODE: LBX ERROR
    mov ax, offset LBX_LoadPath
    push ax
    lea ax, [bp+LBX_NameWithPath]
    push ax
    call _strcpy
    add sp, 04h                         ;pop cx, pop cx

    lea ax, [bp+LBX_FileName]
    push ax
    lea ax, [bp+LBX_NameWithPath]
    push ax
    call _strcat
    add sp, 04h                         ;pop cx, pop cx

    lea ax, [bp+LBX_NameWithPath]
    push ax
    call DISK_OpenFile
    add sp, 02h                         ;pop cx

    mov [LBX_LastLoadHnd], ax
; --------------------------------------SKIP_CODE: LBX ERROR
;    cmp [LBX_LastLoadHnd], 0
;    jnz short loc_1517F
;    push di
;    mov ax, LBXErr_not_found
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 6
;loc_1517F:
; --------------------------------------SKIP_CODE: LBX ERROR
    cmp [LBX_LastLoadFmt], 0
;    jz short loc_15192
;    mov [word ptr bp+File_Offset+2], 0
;    mov [word ptr bp+File_Offset], 200h
;    jmp short loc_1519C
;loc_15192:
;    mov [word ptr bp+File_Offset+2], 0
;    mov [word ptr bp+File_Offset], 0
;loc_1519C:
;    push [LBX_LastLoadHnd]
;    push [word ptr bp+File_Offset+2]
;    push [word ptr bp+File_Offset]
;    call DISK_LSeek
;    add sp, 6
;    or ax, ax
;    jnz short loc_151BF
;    push di
;    mov ax, LBXErr_corrupted
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    jmp short loc_151D0
;loc_151BF:
;    push [LBX_LastLoadHnd]
;    mov ax, 200h
;    push ax
;    push [LBX_LastLoadHdr@]
;    call DISK_ReadToSeg
;loc_151D0:
;    add sp, 6
;    mov ax, 2
;    push ax
;    push [LBX_LastLoadHdr@]
;    call PEEK_Word
;    add sp, 04h                         ;pop cx, pop cx
;    cmp ax, LBX_FileSig
;    jz short loc_151F5
;    push di
;    mov ax, LBXErr_bad_header
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 6
;loc_151F5:
;    mov  ax, 0                          ;xor ax, ax
;    push ax
;    push [LBX_LastLoadHdr@]
;    call PEEK_Word
;    add sp, 04h                         ;pop cx, pop cx
;    mov [LBX_LastLoadECount], ax
;loc_15206:
;    cmp di, [LBX_LastLoadECount]
;    jl short loc_1521A
;    push di
;    mov ax, LBXErr_entries_exceeded
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 6
;loc_1521A:
;    mov ax, di
;    mov cl, 2
;    shl ax, cl
;    add ax, 8
;    mov [bp+MemberOffset], ax
;    push [bp+MemberOffset]
;    push [LBX_LastLoadHdr@]
;    call PEEK_Dword
;    add sp, 04h                         ;pop cx, pop cx
;    mov [word ptr bp+Entry_Off_In_File+2], dx
;    mov [word ptr bp+Entry_Off_In_File], ax
;    mov ax, [bp+MemberOffset]
;    add ax, 4
;    push ax
;    push [LBX_LastLoadHdr@]
;    call PEEK_Dword
;    add sp, 04h                         ;pop cx, pop cx
;    mov [word ptr bp+Next_Entry_In_File+2], dx
;    mov [word ptr bp+Next_Entry_In_File], ax
;    mov ax, [word ptr bp+Next_Entry_In_File+2]
;    mov dx, [word ptr bp+Next_Entry_In_File]
;    sub dx, [word ptr bp+Entry_Off_In_File]
;    sbb ax, [word ptr bp+Entry_Off_In_File+2]
;    mov [word ptr bp+Entry_Size_Bytes+2], ax
;    mov [word ptr bp+Entry_Size_Bytes], dx
;    push [LBX_LastLoadHnd]
;    push [word ptr bp+Entry_Off_In_File+2]
;    push [word ptr bp+Entry_Off_In_File]
;    call DISK_LSeek
;    add sp, 6
;    or ax, ax
;    jnz short loc_1528B
;    push di
;    mov ax, LBXErr_corrupted
;    push ax
;    mov ax, offset LBX_LastLoadName
;    push ax
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 6
;loc_1528B:
;    xor ax, ax
;    mov dx, 10h
;    push ax
;    push dx
;    push [word ptr bp+Entry_Size_Bytes+2]
;    push [word ptr bp+Entry_Size_Bytes]
;    call LDIV@
;    inc ax
;    mov [bp+Entry_Size_Paras], ax
;    mov ax, [bp+Load_Type]
;    or ax, ax
;    jz short loc_152B8
;    cmp ax, 1
;    jz short loc_152DB
;    cmp ax, 2
;    jnz short loc_152B5
;    jmp loc_1534A
;loc_152B5:
;    jmp loc_153BD
;loc_152B8:
;    push [bp+Entry_Size_Paras]
;    call CRP_ST_AllocMem2
;    add sp, 02h                         ;pop cx
;    mov [bp+LBX_Data_Seg], ax
;    cmp [bp+LBX_Data_Seg], 0
;    jnz short loc_152D8
;    push di
;    mov ax, 3
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 6
;loc_152D8:
;    jmp loc_153BD
;    loc_152DB:
;    push [bp+LBX_Alloc_Seg]
;    call LBX_Alloc_Validate
;    add sp, 02h                         ;pop cx
;    or ax, ax
;    jnz short loc_152F6
;    push di
;    mov ax, LBXErr_corrupted
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 6
;loc_152F6:
;    mov ax, LBX_Alloc_Space_Header.Size_Paras
;    push ax
;    push [bp+LBX_Alloc_Seg]
;    call PEEK_Word
;    add sp, 04h                         ;pop cx, pop cx
;    dec ax
;    cmp ax, [bp+Entry_Size_Paras]
;    jnb short loc_1532D
;    mov ax, LBX_Alloc_Space_Header.Size_Paras
;    push ax
;    push [bp+LBX_Alloc_Seg]
;    call PEEK_Word
;    add sp, 04h                         ;pop cx, pop cx
;    mov dx, [bp+Entry_Size_Paras]
;    sub dx, ax
;    inc dx
;    push dx
;    push di
;    mov ax, LBXErr_alloc_fail
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 8
;loc_1532D:
;    mov ax, [bp+LBX_Alloc_Seg]
;    inc ax
;    mov [bp+LBX_Data_Seg], ax
;    mov ax, [bp+Entry_Size_Paras]
;    inc ax
;    push ax
;loc_15339:
;    mov ax, LBX_Alloc_Space_Header.Used_Paras
;    push ax
;    push [bp+LBX_Alloc_Seg]
;    call POKE_Word
;    add sp, 6
;    jmp short loc_153BD
;loc_1534A:
;    push [bp+LBX_Alloc_Seg]
;    call LBX_Alloc_Validate
;    add sp, 02h                         ;pop cx

;    or ax, ax
;    jnz short loc_15365
;    push di
;    mov ax, LBXErr_corrupted
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 6
;loc_15365:
;    push [bp+LBX_Alloc_Seg]
;    call LBX_Alloc_GetFree
;    add sp, 02h                         ;pop cx
;    cmp ax, [bp+Entry_Size_Paras]
;    jnb short loc_15390
;    push [bp+LBX_Alloc_Seg]
;    call LBX_Alloc_GetFree
;    add sp, 02h                         ;pop cx
;    mov dx, [bp+Entry_Size_Paras]
;    sub dx, ax
;    push dx
;    push di
;    mov ax, LBXErr_reload_fail
;    push ax
;    push si
;    nop
;    push cs
;loc_1538A:
;    call near ptr LBX_Error
;    add sp, 8
;loc_15390:
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push [bp+LBX_Alloc_Seg]
    call PEEK_Word
    add sp, 04h                         ;pop cx, pop cx
    mov dx, [bp+LBX_Alloc_Seg]
    add dx, ax
    mov [bp+LBX_Data_Seg], dx
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push [bp+LBX_Alloc_Seg]
    call PEEK_Word
    add sp, 04h                         ;pop cx, pop cx
    mov dx, [bp+Entry_Size_Paras]
    add dx, ax
    push dx
    jmp loc_15339
loc_153BD:
    mov ax, [bp+LBX_Data_Seg]
    mov [bp+Load_To_Seg], ax
    mov [bp+Bytes_To_Copy], 8000h
    jmp short loc_153FC
loc_153CA:
    sub [word ptr bp+Entry_Size_Bytes], 8000h
    sbb [word ptr bp+Entry_Size_Bytes+2], 0
    push [LBX_LastLoadHnd]
    push [bp+Bytes_To_Copy]
    push [bp+Load_To_Seg]
    call DISK_ReadToSeg
    add sp, 6
; --------------------------------------SKIP_CODE: LBX ERROR
;    or ax, ax
;    jnz short loc_153F7
;    push di
;    mov ax, LBXErr_corrupted
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 6
; --------------------------------------SKIP_CODE: LBX ERROR
loc_153F7:
    add [bp+Load_To_Seg], 800h
loc_153FC:
    cmp [word ptr bp+Entry_Size_Bytes+2], 0
    jg short loc_153CA
    jl short loc_1540B
    cmp [word ptr bp+Entry_Size_Bytes], 8000h
    jnb short loc_153CA
loc_1540B:
    cmp [word ptr bp+Entry_Size_Bytes+2], 0
    jl short loc_15443
    jg short loc_15419
    cmp [word ptr bp+Entry_Size_Bytes], 0
    jbe short loc_15443
loc_15419:
    mov ax, [word ptr bp+Entry_Size_Bytes]
    mov [bp+Bytes_To_Copy], ax
    push [LBX_LastLoadHnd]
    push [bp+Bytes_To_Copy]
    push [bp+Load_To_Seg]
    call DISK_ReadToSeg
    add sp, 6
; --------------------------------------SKIP_CODE: LBX ERROR
;    or ax, ax
;    jnz short loc_15443
;    push di
;    mov ax, LBXErr_corrupted
;    push ax
;    push si
;    nop
;    push cs
;    call near ptr LBX_Error
;    add sp, 6
; --------------------------------------SKIP_CODE: LBX ERROR
loc_15443:
;    call RAM_Update_WorstFree
    mov ax, [bp+LBX_Data_Seg]
;    jmp short $+2

    pop di
    pop si

    mov sp, bp
    pop bp

    ret

endp LBX_EntryLoader

;ends seg010

end
