TITLE _s10p12.asm LBXR_DirectLoader

.MODEL LARGE, C

.CODE
;segment seg010 byte public 'CODE' use16
;    assume cs:seg010
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC LBXR_DirectLoader

proc LBXR_DirectLoader

    Name_With_Path= byte ptr -80h
    LBX_File_Name= byte ptr -30h
    Total_Bytes_Left= dword ptr -1Ch
    Next_Entry_Offset= dword ptr -18h
    Entry_Offset= dword ptr -14h
    File_Offset= dword ptr -10h
    File_Record_Size= word ptr -0Ch
    Amt_Bytes= word ptr -0Ah
    File_Record_Count= word ptr -8
    LBX_Format= word ptr -6
    Entry_Offset@= word ptr -4
    Entry_Offset@_HI16= word ptr -2
    LBX_FileName@= word ptr 6
    Entry_Index= word ptr 8
    LoadTo@= word ptr 0Ah
    FirstRec= word ptr 0Ch
    RecCount= word ptr 0Eh
    RecSize= word ptr 10h

    push bp
    mov  bp, sp
    sub  sp, 80h
    push si
    push di

    mov di, [bp+LBX_FileName@]
    mov si, [bp+Entry_Index]
    or si, si
    jge short loc_1593C
    push si
    mov ax, LBXErr_not_found
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_1593C:
    cmp [LBX_LdHdr_Exists], 0
    jnz short loc_15956
    mov [LBX_LdHdr_Exists], 1
    mov ax, 20h
    push ax
    call LBX_farmalloc
    pop cx
    mov [LBX_LastLoadHdr@], ax

loc_15956:
    push di
    nop
    push cs
    call near ptr STR_StripFileExt
    pop cx
    push [bp+RecSize]
    push [bp+RecCount]
    push [bp+FirstRec]
    push [bp+LoadTo@]
    push si
    push di
    call EMM_LBXR_DirectLoad
    add sp, 0Ch
    mov [bp+Entry_Offset@_HI16], ax
    mov [bp+LBX_Format], 0
    cmp [bp+Entry_Offset@_HI16], 0
    jz short loc_15984
    jmp loc_15C3F

loc_15984:
    db 83h,3Eh,68h,3Eh,0FFh ; <BAD>cmp [LBX_LastLoadHnd], 0FFFFh
    jz short loc_159A6
    mov ax, offset LBX_LastLoadName
    push ax
    push di
    call stricmp
    pop cx
    pop cx
    or ax, ax
    jnz short loc_159A6
    mov ax, [LBX_LastLoadFmt]
    cmp ax, [bp+LBX_Format]
    jnz short loc_159A6
    jmp loc_15AC1

loc_159A6:
    mov ax, [bp+LBX_Format]
    mov [LBX_LastLoadFmt], ax
    db 83h,3Eh,68h,3Eh,0FFh ; <BAD>cmp [LBX_LastLoadHnd], 0FFFFh
    jz short loc_159BD
    push [LBX_LastLoadHnd]
    call DISK_CloseFile
    pop cx

loc_159BD:
    push di
    mov ax, offset LBX_LastLoadName
    push ax
    call strcpy
    pop cx
    pop cx
    push di
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
    jnz short loc_15A4D
    cmp [LBX_LoadPath], 0
    jnz short loc_15A0D
    push si
    mov ax, LBXErr_not_found
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15A0D:
    mov ax, offset LBX_LoadPath
    push ax
    lea ax, [bp+Name_With_Path]
    push ax
    call strcpy
    pop cx
    pop cx
    lea ax, [bp+LBX_File_Name]
    push ax
    lea ax, [bp+Name_With_Path]
    push ax
    call strcat
    pop cx
    pop cx
    lea ax, [bp+Name_With_Path]
    push ax
    call DISK_OpenFile
    pop cx
    mov [LBX_LastLoadHnd], ax
    cmp [LBX_LastLoadHnd], 0
    jnz short loc_15A4D
    push si
    mov ax, LBXErr_not_found
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15A4D:
    mov [word ptr bp+File_Offset+2], 0
    mov [word ptr bp+File_Offset], 0
    push [LBX_LastLoadHnd]
    push [word ptr bp+File_Offset+2]
    push [word ptr bp+File_Offset]
    call DISK_LSeek
    add sp, 6
    or ax, ax
    jnz short loc_15A7A
    push si
    mov ax, LBXErr_corrupted
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    jmp short loc_15A8B

loc_15A7A:
    push [LBX_LastLoadHnd]
    mov ax, 200h
    push ax
    push [LBX_LastLoadHdr@]
    call DISK_ReadToSeg

loc_15A8B:
    add sp, 6
    mov ax, 2
    push ax
    push [LBX_LastLoadHdr@]
    call PEEK_Word
    pop cx
    pop cx
    cmp ax, LBX_FileSig
    jz short loc_15AB0
    push si
    mov ax, LBXErr_bad_header
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15AB0:
    xor ax, ax
    push ax
    push [LBX_LastLoadHdr@]
    call PEEK_Word
    pop cx
    pop cx
    mov [LBX_LastLoadECount], ax

loc_15AC1:
    cmp si, [LBX_LastLoadECount]
    jl short loc_15AD5
    push si
    mov ax, LBXErr_entries_exceeded
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15AD5:
    mov ax, si
    mov cl, 2
    shl ax, cl
    add ax, 8
    mov [bp+Entry_Offset@], ax
    push [bp+Entry_Offset@]
    push [LBX_LastLoadHdr@]
    call PEEK_Dword
    pop cx
    pop cx
    mov [word ptr bp+Entry_Offset+2], dx
    mov [word ptr bp+Entry_Offset], ax
    mov ax, [bp+Entry_Offset@]
    add ax, 4
    push ax
    push [LBX_LastLoadHdr@]
    call PEEK_Dword
    pop cx
    pop cx
    mov [word ptr bp+Next_Entry_Offset+2], dx
    mov [word ptr bp+Next_Entry_Offset], ax
    mov ax, [word ptr bp+Next_Entry_Offset+2]
    mov dx, [word ptr bp+Next_Entry_Offset]
    sub dx, [word ptr bp+Entry_Offset]
    sbb ax, [word ptr bp+Entry_Offset+2]
    mov [word ptr bp+Total_Bytes_Left+2], ax
    mov [word ptr bp+Total_Bytes_Left], dx
    push [LBX_LastLoadHnd]
    push [word ptr bp+Entry_Offset+2]
    push [word ptr bp+Entry_Offset]
    call DISK_LSeek
    add sp, 6
    or ax, ax
    jnz short loc_15B46
    push si
    mov ax, LBXErr_corrupted
    push ax
    mov ax, offset LBX_LastLoadName
    push ax
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15B46:
    push [LBX_LastLoadHnd]
    mov ax, 2
    push ax
    lea ax, [bp+File_Record_Count]
    push ax
    call DISK_ReadToData
    add sp, 6
    push [LBX_LastLoadHnd]
    mov ax, 2
    push ax
    lea ax, [bp+File_Record_Size]
    push ax
    call DISK_ReadToData
    add sp, 6
    cmp [bp+File_Record_Size], 0
    jz short loc_15B7C
    mov ax, [bp+RecSize]
    cmp ax, [bp+File_Record_Size]
    jz short loc_15B8D

loc_15B7C:
    push si
    mov ax, LBXErr_recsize_mismatch
    push ax
    mov ax, offset LBX_LastLoadName
    push ax
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15B8D:
    mov ax, [bp+FirstRec]
    add ax, [bp+RecCount]
    cmp ax, [bp+File_Record_Count]
    jbe short loc_15BA9
    push si
    mov ax, LBXErr_records_exceeded
    push ax
    mov ax, offset LBX_LastLoadName
    push ax
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15BA9:
    mov ax, [bp+FirstRec]
    cwd
    push ax
    mov ax, [bp+File_Record_Size]
    push dx
    xor dx, dx
    pop cx
    pop bx
    call LXMUL@
    add ax, 4
    adc dx, 0
    add [word ptr bp+Entry_Offset], ax
    adc [word ptr bp+Entry_Offset+2], dx
    push [LBX_LastLoadHnd]
    push [word ptr bp+Entry_Offset+2]
    push [word ptr bp+Entry_Offset]
    call DISK_LSeek
    add sp, 6
    or ax, ax
    jnz short loc_15BEE
    push si
    mov ax, LBXErr_corrupted
    push ax
    mov ax, offset LBX_LastLoadName
    push ax
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15BEE:
    mov ax, [bp+RecCount]
    cwd
    push ax
    mov ax, [bp+File_Record_Size]
    push dx
    xor dx, dx
    pop cx
    pop bx
    call LXMUL@
    mov [word ptr bp+Total_Bytes_Left+2], dx
    mov [word ptr bp+Total_Bytes_Left], ax
    xor ax, ax
    mov dx, 60000
    push ax
    push dx
    push [word ptr bp+Total_Bytes_Left+2]
    push [word ptr bp+Total_Bytes_Left]
    call LMOD@
    mov [bp+Amt_Bytes], ax
    push [LBX_LastLoadHnd]
    push [bp+Amt_Bytes]
    push [bp+LoadTo@]
    call DISK_ReadToData
    add sp, 6
    or ax, ax
    jnz short loc_15C3F
    push si
    mov ax, LBXErr_corrupted
    push ax
    push di
    nop
    push cs
    call near ptr LBX_Error
    add sp, 6

loc_15C3F:
    call RAM_Update_WorstFree
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp LBXR_DirectLoader

;ends seg010

end
