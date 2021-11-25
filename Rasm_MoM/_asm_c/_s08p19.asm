TITLE _s08p19.asm LBX_Alloc_Error

.MODEL LARGE, C

EXTRN RAM_MinKbytes:WORD
EXTRN cnst_Alloc_Error3:BYTE
EXTRN cnst_Alloc_Error4:BYTE
EXTRN cnst_Alloc_Error11:BYTE
EXTRN cnst_Alloc_Error12:BYTE
EXTRN cnst_Alloc_Error13:BYTE
EXTRN cnst_Alloc_Error21:BYTE
EXTRN cnst_Alloc_Error22:BYTE
EXTRN cnst_Alloc_Error23:BYTE
EXTRN cnst_Alloc_Error51:BYTE
EXTRN cnst_Alloc_Error52:BYTE

EXTRN itoa:PROC
EXTRN strcat:PROC
EXTRN strcpy:PROC
EXTRN DBG_IsDisabled:PROC
EXTRN GAME_QuitProgram:PROC

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg008 byte public 'CODE' use16
;    assume cs:seg008
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

proc LBX_Alloc_Error far

    Conversion_String = byte ptr -8Ch
    Error_String = byte ptr -78h
    Error_Type = word ptr 6
    Error_Value = word ptr 8

    push bp
    mov bp, sp
    sub sp, 8Ch
    push si

    mov si, [bp+Error_Value]

    call DBG_IsDisabled

    or ax, ax
    jz short loc_14CD0

    mov ax, offset cnst_Alloc_Error51
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

    mov ax, offset cnst_Alloc_Error52
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    lea ax, [bp+Error_String]
    push ax
    call GAME_QuitProgram
    pop cx

loc_14CD0:
    mov bx, [bp+Error_Type]
    dec bx
    cmp bx, 3
    jbe short loc_14CDC

    jmp loc_14DFA

loc_14CDC:
    shl bx, 1
    jmp [cs:off_14E09+bx]

loc_14CE3:
    mov ax, offset cnst_Alloc_Error11
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
    push si
    call itoa
    add sp, 6

loc_14D04:
    mov ax, offset cnst_Alloc_Error12
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

loc_14D13:
    lea ax, [bp+Conversion_String]
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_Alloc_Error13
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    jmp loc_14DFA

loc_14D35:
    mov ax, offset cnst_Alloc_Error21
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcpy
    pop cx
    pop cx

    mov ax, offset cnst_Alloc_Error22
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_Alloc_Error23
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
    push si
    call itoa
    add sp, 6

    jmp short loc_14D13

loc_14D76:
    mov ax, offset cnst_Alloc_Error3
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
    push si
    call itoa
    add sp, 6

    mov ax, offset cnst_Alloc_Error22
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    jmp loc_14D04

loc_14DA9:
    mov ax, offset cnst_Alloc_Error21
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcpy
    pop cx
    pop cx

    mov ax, offset cnst_Alloc_Error4
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_Alloc_Error22
    push ax
    lea ax, [bp+Error_String]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_Alloc_Error23
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
    push si
    call itoa
    add sp, 6

    jmp loc_14D13

loc_14DFA:
    lea ax, [bp+Error_String]
    push ax
    call GAME_QuitProgram
    pop cx

    pop si
    mov sp, bp
    pop bp
    ret

endp LBX_Alloc_Error

off_14E09   dw offset loc_14CE3
            dw offset loc_14D35
            dw offset loc_14D76
            dw offset loc_14DA9

;ends seg008

end
