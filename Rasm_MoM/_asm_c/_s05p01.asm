TITLE _s05p01.asm GAME_QuitWithReport

.MODEL LARGE, C

segment dseg para public 'DATA' use16
ends dseg


segment seg005 byte public 'CODE' use16
    assume cs:seg005
    ;org 0Eh
    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GAME_QuitWithReport

proc GAME_QuitWithReport far

    Conversion_String= byte ptr -92h
    Message_String= byte ptr -7Eh
    Free_Near_Heap_Bytes= word ptr -6
    Smallest_Largest_Free_Block= word ptr -4
    Current_Largest_Free_Block= word ptr -2

    push bp
    mov bp, sp
    sub sp, 92h

    call RAM_GetFreeBlockSize

    mov [bp+Current_Largest_Free_Block], ax
    mov ax, [RAM_WorstFreeBlocks]
    mov [bp+Smallest_Largest_Free_Block], ax
    call coreleft

    mov [bp+Free_Near_Heap_Bytes], ax
    mov ax, offset cnst_Quit_Report1
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcpy
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_Quit_Report2
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, 0Ah
    push ax
    lea ax, [bp+Conversion_String]
    push ax
    mov ax, 0 ; xor ax, ax
    push ax
    push [bp+Free_Near_Heap_Bytes]
    call ltoa
    add sp, 8

    lea ax, [bp+Conversion_String]
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_Quit_Report3
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_Quit_Report2
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, 0Ah
    push ax
    lea ax, [bp+Conversion_String]
    push ax
    push [bp+Current_Largest_Free_Block]
    call itoa
    add sp, 6

    lea ax, [bp+Conversion_String]
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_Quit_Report4
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_Quit_Report2
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, 0Ah
    push ax
    lea ax, [bp+Conversion_String]
    push ax
    push [bp+Smallest_Largest_Free_Block]
    call itoa
    add sp, 6

    lea ax, [bp+Conversion_String]
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_Quit_Report5
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, 0Ah
    push ax
    lea ax, [bp+Conversion_String]
    push ax
    push [EMM_Reserved_Left]
    call itoa
    add sp, 6

    lea ax, [bp+Conversion_String]
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_Quit_Report6
    push ax
    lea ax, [bp+Message_String]
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    lea ax, [bp+Message_String]
    push ax
    ;nop
    ;push cs
    call near ptr GAME_QuitProgram
    add sp, 02h                         ; pop cx

    mov sp, bp
    pop bp
    ret

endp GAME_QuitWithReport

ends seg005

end
