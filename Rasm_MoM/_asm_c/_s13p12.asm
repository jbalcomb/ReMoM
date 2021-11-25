TITLE _s13p12.asm EMM_LBX_HdrOnly

.MODEL LARGE, C

LBX_Alloc_Space_Header
LBX_IMG_HDR

EXTRN CRP_LBX_farmalloc2:PROC
EXTRN EMM_MapnRead:PROC
EXTRN farpeekw:PROC
EXTRN farpokeb:PROC
EXTRN farpokew:PROC
EXTRN LBX_Alloc_Validate:PROC
EXTRN LBX_Error:PROC

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg013 byte public 'CODE' use16
;    assume cs:seg013
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_LBX_HdrOnly

proc EMM_LBX_HdrOnly far

    Header_Offset       = dword ptr -10h
    Entry_Offset        = dword ptr -0Ch
    First_Logical_Page  =  word ptr -8
    Data_Offset         =  word ptr -6
    Header_Size         =  word ptr -4
    Entry_Count         =  word ptr -2
    EMM_Handle          =  word ptr 6
    Handle_Name         =  word ptr 8
    LBX_Entry_Index     =  word ptr 0Ah
    Target_Seg          =  word ptr 0Ch
    Load_Type           =  word ptr 0Eh

    push bp
    mov bp, sp
    sub sp, 10h
    push si
    push di

    mov si, [bp+Target_Seg]
    xor di, di

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
    call EMM_MapnRead
    add sp, 0Ch

    mov ax, [bp+LBX_Entry_Index]
    cmp ax, [bp+Entry_Count]
    jl short loc_16F88

    push [bp+LBX_Entry_Index]
    mov ax, LBXErr_entries_exceeded
    push ax
    push [bp+Handle_Name]
    call LBX_Error
    add sp, 6

loc_16F88:
    mov ax, [bp+LBX_Entry_Index]
    cwd
    mov cl, 2
    call LXLSH@

    add ax, 8
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
    call EMM_MapnRead
    add sp, 0Ch

    mov [bp+Header_Size], 1
    mov ax, [bp+Load_Type]
    or ax, ax
    jz short loc_16FD7

    cmp ax, 1
    jz short loc_16FFB

    cmp ax, 2
    jnz short loc_16FD4

    jmp loc_17059

loc_16FD4:
    jmp loc_170C9

loc_16FD7:
    push [bp+Header_Size]
    call CRP_LBX_farmalloc2
    pop cx
    
    mov di, ax
    or di, di
    jnz short loc_16FF8

    push [bp+LBX_Entry_Index]
    mov ax, LBXErr_low_RAM
    push ax
    push [bp+Handle_Name]
    call LBX_Error
    add sp, 6

loc_16FF8:
    jmp loc_170C9

loc_16FFB:
    push si
    call LBX_Alloc_Validate
    pop cx

    or ax, ax
    jnz short loc_17018

    push [bp+LBX_Entry_Index]
    mov ax, LBXErr_alloc_fail
    push ax
    push [bp+Handle_Name]
    call LBX_Error
    add sp, 6

loc_17018:
    mov ax, LBX_Alloc_Space_Header.Size_Paras
    push ax
    push si
    call farpeekw
    pop cx
    pop cx

    dec ax
    cmp ax, [bp+Header_Size]
    jnb short loc_17040

    mov ax, 1
    push ax
    push [bp+LBX_Entry_Index]
    mov ax, 5
    push ax
    push [bp+Handle_Name]
    call LBX_Error
    add sp, 8

loc_17040:
    mov ax, si
    inc ax
    mov di, ax
    mov ax, [bp+Header_Size]
    inc ax
    push ax

loc_1704A:
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push si
    call farpokew
    add sp, 6

    jmp short loc_170C9

loc_17059:
    or si, si
    jnz short loc_17062

    mov ax, 1
    jmp short loc_17064

loc_17062:
    xor ax, ax

loc_17064:
    push ax
    call LBX_Alloc_Validate
    pop cx

    or ax, ax
    jz short loc_17081

    push [bp+LBX_Entry_Index]
    mov ax, LBXErr_alloc_fail
    push ax
    push [bp+Handle_Name]
    call LBX_Error
    add sp, 6

loc_17081:
    push si
    call LBX_Alloc_GetFree
    pop cx

    cmp ax, [bp+Header_Size]
    jnb short loc_170A3

    mov ax, 1
    push ax
    push [bp+LBX_Entry_Index]
    mov ax, LBXErr_reload_fail
    push ax
    push [bp+Handle_Name]
    call LBX_Error
    add sp, 8

loc_170A3:
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push si
    call farpeekw
    pop cx
    pop cx

    mov dx, si
    add dx, ax
    mov di, dx
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push si
    call farpeekw
    pop cx
    pop cx

    mov dx, [bp+Header_Size]
    add dx, ax
    push dx
    jmp short loc_1704A

loc_170C9:
    xor ax, ax
    mov dx, 4000h
    push ax
    push dx
    push [word ptr bp+Entry_Offset+2]
    push [word ptr bp+Entry_Offset]
    call LDIV@

    mov [bp+First_Logical_Page], ax
    xor ax, ax
    mov dx, 4000h
    push ax
    push dx
    push [word ptr bp+Entry_Offset+2]
    push [word ptr bp+Entry_Offset]
    call LMOD@

    mov [bp+Data_Offset], ax
    push [bp+EMM_Handle]
    mov ax, 10h
    push ax
    push [word ptr bp+Entry_Offset+2]
    push [word ptr bp+Entry_Offset]
    push di
    xor ax, ax
    push ax
    call EMM_MapnRead
    add sp, 0Ch

    push [bp+EMM_Handle]
    mov ax, LBX_IMG_HDR.EMM_Handle
    push ax
    push di
    call farpokeb
    add sp, 6

    push [bp+First_Logical_Page]
    mov ax, LBX_IMG_HDR.EMM_Logical_Page
    push ax
    push di
    call farpokeb
    add sp, 6

    push [bp+Data_Offset]
    mov ax, LBX_IMG_HDR.EMM_Data_Offset
    push ax
    push di
    call farpokew
    add sp, 6

    mov ax, di
    jmp short $+2

    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp EMM_LBX_HdrOnly

;ends seg013

end
