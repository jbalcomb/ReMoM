TITLE _s13p14.asm EMM_LBXR_DirectLoad

.MODEL LARGE, C

.CODE
;segment seg013 byte public 'CODE' use16
;    assume cs:seg013
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_LBXR_DirectLoad

proc EMM_LBXR_DirectLoad

    Header_Offset= dword ptr -18h
    Total_Bytes_Left= dword ptr -14h
    Entry_Offset= dword ptr -10h
    File_Record_Count= word ptr -0Ch
    File_Record_Size= word ptr -0Ah
    Unused_Para_Cnt= word ptr -8
    Amt_Bytes= word ptr -6
    File_Entry_Count= word ptr -4
    Unused_Local= word ptr -2
    Hnd_Name@= word ptr 6
    Entry_Index= word ptr 8
    LoadTo@= word ptr 0Ah
    FirstRec= word ptr 0Ch
    RecCount= word ptr 0Eh
    RecSize= word ptr 10h

    push bp
    mov  bp, sp
    sub  sp, 18h
    push si
    push di

    mov di, [bp+Entry_Index]
    mov [bp+Unused_Local], 0
    push [bp+Hnd_Name@]
    nop
    push cs
    call near ptr EMM_CheckHandleOpen
    pop cx
    mov si, ax
    or si, si
    jnz short loc_1742A
    jmp loc_175A2

loc_1742A:
    push si
    mov ax, 2
    push ax
    xor ax, ax
    push ax
    xor ax, ax
    push ax
    xor ax, ax
    push ax
    lea ax, [bp+File_Entry_Count]
    push ax
    call EMM_MapnRead
    add sp, 0Ch
    cmp di, [bp+File_Entry_Count]
    jl short loc_1744E

loc_17449:
    xor ax, ax

loc_1744B:
    jmp loc_175AB

loc_1744E:
    mov ax, di
    cwd
    mov cl, 2
    call LXLSH@
    add ax, 8
    adc dx, 0
    mov [word ptr bp+Header_Offset+2], dx
    mov [word ptr bp+Header_Offset], ax
    push si
    mov ax, 4
    push ax
    push [word ptr bp+Header_Offset+2]
    push [word ptr bp+Header_Offset]
    xor ax, ax
    push ax
    lea ax, [bp+Entry_Offset]
    push ax
    call EMM_MapnRead
    add sp, 0Ch
    push si
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
    call EMM_MapnRead
    add sp, 0Ch
    mov ax, [word ptr bp+Entry_Offset+2]
    mov dx, [word ptr bp+Entry_Offset]
    sub [word ptr bp+Total_Bytes_Left], dx
    sbb [word ptr bp+Total_Bytes_Left+2], ax
    cmp [word ptr bp+Total_Bytes_Left+2], 0
    jge short loc_174B5
    jmp loc_175A0

loc_174B5:
    jg short loc_174C0
    cmp [word ptr bp+Total_Bytes_Left], 0
    ja short loc_174C0
    jmp loc_175A0

loc_174C0:
    mov ax, [word ptr bp+Entry_Offset+2]
    mov dx, [word ptr bp+Entry_Offset]
    mov [word ptr bp+Header_Offset+2], ax
    mov [word ptr bp+Header_Offset], dx
    push si
    mov ax, 2
    push ax
    push [word ptr bp+Header_Offset+2]
    push [word ptr bp+Header_Offset]
    xor ax, ax
    push ax
    lea ax, [bp+File_Record_Count]
    push ax
    call EMM_MapnRead
    add sp, 0Ch
    push si
    mov ax, 2
    push ax
    mov ax, [word ptr bp+Header_Offset+2]
    mov dx, [word ptr bp+Header_Offset]
    add dx, 2
    adc ax, 0
    push ax
    push dx
    xor ax, ax
    push ax
    lea ax, [bp+File_Record_Size]
    push ax
    call EMM_MapnRead
    add sp, 0Ch
    cmp [bp+File_Record_Size], 0
    jz short loc_17516
    mov ax, [bp+RecSize]
    cmp ax, [bp+File_Record_Size]
    jz short loc_17519

loc_17516:
    jmp loc_17449

loc_17519:
    mov ax, [bp+FirstRec]
    add ax, [bp+RecCount]
    cmp ax, [bp+File_Record_Count]
    jbe short loc_17527
    jmp loc_17449

loc_17527:
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
    mov dx, 10h
    push ax
    push dx
    push [word ptr bp+Total_Bytes_Left+2]
    push [word ptr bp+Total_Bytes_Left]
    call LDIV@
    inc ax
    mov [bp+Unused_Para_Cnt], ax
    xor ax, ax
    mov dx, 8000h
    push ax
    push dx
    push [word ptr bp+Total_Bytes_Left+2]
    push [word ptr bp+Total_Bytes_Left]
    call LMOD@
    mov [bp+Amt_Bytes], ax
    push si
    push [bp+Amt_Bytes]
    push [word ptr bp+Entry_Offset+2]
    push [word ptr bp+Entry_Offset]
    xor ax, ax
    push ax
    push [bp+LoadTo@]
    call EMM_MapnRead
    add sp, 0Ch

loc_175A0:
    jmp short loc_175A5
loc_175A2:
    jmp loc_17449
loc_175A5:
    mov ax, 0FFFFh
    jmp loc_1744B

loc_175AB:
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp EMM_LBXR_DirectLoad

;ends seg013

end
