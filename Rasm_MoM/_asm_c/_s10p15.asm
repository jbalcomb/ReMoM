TITLE _s10p15.asm LBX_Error

.MODEL LARGE

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg010 byte public 'CODE' use16
;    assume cs:seg010
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC LBX_Error

proc LBX_Error far

    Conversion_String   = byte ptr -8Ch
    Error_String        = byte ptr -78h
    LBX_FileName        = word ptr 6
    LBX_Error_Index     = word ptr 8
    LBX_Entry_Index     = word ptr 0Ah
    LBX_Alloc_Short_By  = word ptr 0Ch

    push bp
    mov  bp, sp
    sub  sp, 8Ch
    push si

    mov si, [bp+LBX_FileName]
    push si
    lea ax, [bp+Error_String]
    push ax
    call strcpy
    pop cx
    pop cx

    mov ax, 0Ah
    push ax
    lea ax, [bp+Conversion_String]
    push ax
    push [bp+LBX_Entry_Index]
    call itoa
    add sp, 6

    mov ax, offset cnst_LBXErr_Common1
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    lea ax, [bp+Conversion_String]
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_LBXErr_Common2
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov bx, [bp+LBX_Error_Index]
    dec bx
    cmp bx, 0Fh
    jbe short loc_15EDB
    jmp loc_16006

loc_15EDB:
    shl bx, 1
    jmp [cs:off_16015+bx]

loc_15EE2:
    mov ax, offset cnst_LBX_Error1

loc_15EE5:
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    jmp loc_16006

loc_15EF4:
    mov ax, offset cnst_LBX_Error2
    jmp short loc_15EE5

loc_15EF9:
    mov ax, offset cnst_LBX_Error31
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcpy
    pop cx
    pop cx

    mov ax, 0Ah
    push ax
    lea ax, [bp+Conversion_String]
    push ax
    push [RAM_MinKbytes]
    call itoa
    add sp, 6

    lea ax, [bp+Conversion_String]
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_LBX_Error32
    jmp short loc_15EE5

loc_15F32:
    mov ax, offset cnst_LBX_Error4
    jmp short loc_15EE5

loc_15F37:
    mov ax, offset cnst_LBX_Error51
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, 0Ah
    push ax
    lea ax, [bp+Conversion_String]
    push ax
    push [bp+LBX_Alloc_Short_By]
    call itoa
    add sp, 6

    lea ax, [bp+Conversion_String]
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_LBX_Error52
    jmp loc_15EE5

loc_15F70:
    mov ax, offset cnst_LBX_Error7
    jmp loc_15EE5

loc_15F76:
    mov ax, offset cnst_LBX_Error8
    jmp loc_15EE5

loc_15F7C:
    mov ax, offset cnst_LBX_Error9
    jmp loc_15EE5

loc_15F82:
    mov ax, offset cnst_LBX_ErrorA
    jmp loc_15EE5

loc_15F88:
    push si
    lea ax, [bp+Error_String]
    push ax
    call strcpy
    pop cx
    pop cx

    mov ax, offset cnst_LBX_Ext
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_LBX_ErrorB
    jmp loc_15EE5

loc_15FA9:
    mov ax, offset cnst_LBX_ErrorC
    jmp loc_15EE5

loc_15FAF:
    mov ax, offset cnst_LBX_ErrorD
    jmp loc_15EE5

loc_15FB5:
    mov ax, offset cnst_LBX_ErrorC
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_LBX_ErrorE
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

loc_15FD3:
    mov ax, offset cnst_LBX_ErrorF1
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_LBX_ErrorC
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_LBX_ErrorE
    jmp loc_15EE5

loc_15FF7:
    mov ax, offset cnst_LBX_ErrorG
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

loc_16006:
    lea ax, [bp+Error_String]
    push ax
    call GAME_QuitProgram
    pop cx

    pop si
    mov sp, bp
    pop bp
    ret

endp LBX_Error

; ---------------------------------------------------------------------------
off_16015   dw offset loc_15EE2
            dw offset loc_15EF4
            dw offset loc_15EF9
            dw offset loc_15F32
            dw offset loc_15F37
            dw offset loc_16006
            dw offset loc_15F70
            dw offset loc_15F76
            dw offset loc_15F7C
            dw offset loc_15F82
            dw offset loc_15F88
            dw offset loc_15FA9
            dw offset loc_15FAF
            dw offset loc_15FB5
            dw offset loc_15FD3
            dw offset loc_15FF7

;ends seg010

end
