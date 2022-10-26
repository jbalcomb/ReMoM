TITLE _s10p13.asm LBX_GetEntryData

.MODEL LARGE, C

.CODE
;segment seg010 byte public 'CODE' use16
;    assume cs:seg010
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC LBX_GetEntryData

proc LBX_GetEntryData

    FileName_With_Path= byte ptr -70h
    LBX_File_Name= byte ptr -20h
    File_Offset= dword ptr -0Ch
    Next_Entry_Offset= dword ptr -8
    Header_Offset= word ptr -4
    Load_Format= word ptr -2
    LBX_FileName@= word ptr 6
    Entry_Index= word ptr 8
    File_Off@= word ptr 0Ah
    Entry_Size@= word ptr 0Ch
    Fmt_Type= word ptr 0Eh

    push bp
    mov  bp, sp
    sub  sp, 70h
    push si
    push di

    mov si, [bp+LBX_FileName@]
    mov di, [bp+Entry_Index]
    or di, di
    jge short loc_15C6A
    push di
    mov ax, LBXErr_not_found
    push ax
    push si
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15C6A:
    cmp [LBX_LdHdr_Exists], 0
    jnz short loc_15C84
    mov [LBX_LdHdr_Exists], 1
    mov ax, 20h
    push ax
    call LBX_farmalloc
    pop cx
    mov [LBX_LastLoadHdr@], ax

loc_15C84:
    push si
    nop
    push cs
    call near ptr STR_StripFileExt
    pop cx
    cmp [bp+Fmt_Type], 0
    jz short loc_15C98
    cmp [G_LBX_Force_LoadFmt], 2
    jnz short loc_15C9F

loc_15C98:
    mov [bp+Load_Format], 0
    jmp short loc_15CA4

loc_15C9F:
    mov [bp+Load_Format], 1

loc_15CA4:
    ;db 83h,3Eh,68h,3Eh,0FFh ; <BAD>cmp [LBX_LastLoadHnd], 0FFFFh
    cmp [LBX_LastLoadHnd], 0FFFFh
    jz short loc_15CC6
    mov ax, offset LBX_LastLoadName
    push ax
    push si
    call stricmp
    pop cx
    pop cx
    or ax, ax
    jnz short loc_15CC6
    mov ax, [LBX_LastLoadFmt]
    cmp ax, [bp+Load_Format]
    jnz short loc_15CC6
    jmp loc_15DF4
    loc_15CC6:
    mov ax, [bp+Load_Format]
    mov [LBX_LastLoadFmt], ax
    ;db 83h,3Eh,68h,3Eh,0FFh ; <BAD>cmp [LBX_LastLoadHnd], 0FFFFh
    cmp [LBX_LastLoadHnd], 0FFFFh
    jz short loc_15CDD
    push [LBX_LastLoadHnd]
    call DISK_CloseFile
    pop cx

loc_15CDD:
    push si
    mov ax, offset LBX_LastLoadName
    push ax
    call strcpy
    pop cx
    pop cx
    push si
    lea ax, [bp+LBX_File_Name]
    push ax
    call strcpy
    pop cx
    pop cx
    mov ax, offset cnst_LBX_Ext
    push ax
    lea ax, [bp+LBX_File_Name]
    push ax
    call strcat
    pop cx
    pop cx
    lea ax, [bp+LBX_File_Name]
    push ax
    call DISK_OpenFile
    pop cx
    mov [LBX_LastLoadHnd], ax
    cmp [LBX_LastLoadHnd], 0
    jnz short loc_15D6D
    cmp [LBX_LoadPath], 0
    jnz short loc_15D2D
    push di
    mov ax, LBXErr_not_found
    push ax
    push si
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15D2D:
    mov ax, offset LBX_LoadPath
    push ax
    lea ax, [bp+FileName_With_Path]
    push ax
    call strcpy
    pop cx
    pop cx
    lea ax, [bp+LBX_File_Name]
    push ax
    lea ax, [bp+FileName_With_Path]
    push ax
    call strcat
    pop cx
    pop cx
    lea ax, [bp+FileName_With_Path]
    push ax
    call DISK_OpenFile
    pop cx
    mov [LBX_LastLoadHnd], ax
    cmp [LBX_LastLoadHnd], 0
    jnz short loc_15D6D
    push di
    mov ax, LBXErr_not_found
    push ax
    push si
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15D6D:
    cmp [LBX_LastLoadFmt], 0
    jz short loc_15D80
    mov [word ptr bp+File_Offset+2], 0
    mov [word ptr bp+File_Offset], 200h
    jmp short loc_15D8A

loc_15D80:
    mov [word ptr bp+File_Offset+2], 0
    mov [word ptr bp+File_Offset], 0

loc_15D8A:
    push [LBX_LastLoadHnd]
    push [word ptr bp+File_Offset+2]
    push [word ptr bp+File_Offset]
    call DISK_LSeek
    add sp, 6
    or ax, ax
    jnz short loc_15DAD
    push di
    mov ax, LBXErr_corrupted
    push ax
    push si
    nop
    push cs
    call near ptr LBX_Error
    jmp short loc_15DBE

loc_15DAD:
    push [LBX_LastLoadHnd]
    mov ax, 200h
    push ax
    push [LBX_LastLoadHdr@]
    call DISK_ReadToSeg

loc_15DBE:
    add sp, 6
    mov ax, 2
    push ax
    push [LBX_LastLoadHdr@]
    call PEEK_Word
    pop cx
    pop cx
    cmp ax, LBX_FileSig
    jz short loc_15DE3
    push di
    mov ax, LBXErr_bad_header
    push ax
    push si
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15DE3:
    xor ax, ax
    push ax
    push [LBX_LastLoadHdr@]
    call PEEK_Word
    pop cx
    pop cx
    mov [LBX_LastLoadECount], ax

loc_15DF4:
    cmp di, [LBX_LastLoadECount]
    jl short loc_15E08
    push di
    mov ax, LBXErr_entries_exceeded
    push ax
    push si
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15E08:
    mov ax, di
    mov cl, 2
    shl ax, cl
    add ax, 8
    mov [bp+Header_Offset], ax
    push [bp+Header_Offset]
    push [LBX_LastLoadHdr@]
    call PEEK_Dword
    pop cx
    pop cx
    mov bx, [bp+File_Off@]
    mov [bx+2], dx
    mov [bx], ax
    mov ax, [bp+Header_Offset]
    add ax, 4
    push ax
    push [LBX_LastLoadHdr@]
    call PEEK_Dword
    pop cx
    pop cx
    mov [word ptr bp+Next_Entry_Offset+2], dx
    mov [word ptr bp+Next_Entry_Offset], ax
    mov bx, [bp+File_Off@]
    mov ax, [word ptr bp+Next_Entry_Offset+2]
    mov dx, [word ptr bp+Next_Entry_Offset]
    sub dx, [bx]
    sbb ax, [bx+2]
    mov bx, [bp+Entry_Size@]
    mov [bx+2], ax
    mov [bx], dx
    mov ax, [LBX_LastLoadHnd]

    jmp short $+2
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp LBX_GetEntryData

;ends seg010

end
