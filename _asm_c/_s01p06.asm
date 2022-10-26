TITLE _s01p06.asm GAME_DrawMainScreen

.MODEL LARGE, C

.CODE
; Segment type: Pure code
;segment seg001 byte public 'CODE' use16
;    assume cs:seg001
;    org 9
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GAME_DrawMainScreen

proc GAME_DrawMainScreen

    Title_Frame_Index = word ptr -8
    Loop_Var = word ptr -6
    Continue_Move_Down = word ptr -4
    Moused_Control = word ptr -2

    push bp
    mov  bp, sp
    sub  sp, 8
    push si
    push di

    mov si, 123
    mov di, 141
    call GUI_MousedControl
    mov [bp+Moused_Control], ax
    call VGA_SetDrawFrame
    xor ax, ax
    push ax
    mov ax, 199
    push ax
    mov ax, 319
    push ax
    xor ax, ax
    push ax
    xor ax, ax
    push ax
    call VGA_DrawFilledRect
    add sp, 0Ah
    push [IMG_MainScr_Title@]
    call LBX_IMG_GetCurFrame
    pop cx
    mov [bp+Title_Frame_Index], ax
    xor ax, ax
    push ax
    push [IMG_MainScr_Title@]
    call LBX_IMG_SetFrame
    pop cx
    pop cx
    mov [bp+Loop_Var], 0
    jmp short loc_13D25

loc_13D10:
    push [IMG_MainScr_Title@]
    xor ax, ax
    push ax
    xor ax, ax
    push ax
    call VGA_DrawLBXImage
    add sp, 6
    inc [bp+Loop_Var]

loc_13D25:
    mov ax, [bp+Loop_Var]
    cmp ax, [bp+Title_Frame_Index]
    jle short loc_13D10
    push [IMG_MainScr_BG@]
    mov ax, 29h
    push ax
    xor ax, ax
    push ax
    call VGA_DrawLBXImage
    add sp, 6
    cmp [NEWG_ScreenChangeFade], 0
    jnz short loc_13D4C
    call j_GAME_DrawCredits

loc_13D4C:
    mov [bp+Continue_Move_Down], 0
    cmp [GAME_HaveSaves], 0
    jz short loc_13D91
    mov ax, [bp+Moused_Control]
    cmp ax, [GUI_Load_Lbl_Index]
    jnz short loc_13D6D
    push [IMG_MainScr_LoadG@]
    call LBX_IMG_ResetFrame
    pop cx
    jmp short loc_13D7C

loc_13D6D:
    mov ax, 1
    push ax
    push [IMG_MainScr_LoadG@]
    call LBX_IMG_SetFrame
    pop cx
    pop cx

loc_13D7C:
    push [IMG_MainScr_LoadG@]
    mov ax, di
    add ax, 0Ch
    push ax
    push si
    call VGA_DrawLBXImage
    add sp, 6
    jmp short loc_13D96

loc_13D91:
    mov [bp+Continue_Move_Down], 1

loc_13D96:
    cmp [GAME_HaveContSave], 0
    jz short loc_13DDB
    mov ax, [bp+Moused_Control]
    cmp ax, [GUI_Continue_Label]
    jnz short loc_13DB2
    push [IMG_MainScr_Cont@]
    call LBX_IMG_ResetFrame
    pop cx
    jmp short loc_13DC1

loc_13DB2:
    mov ax, 1
    push ax
    push [IMG_MainScr_Cont@]
    call LBX_IMG_SetFrame
    pop cx
    pop cx

loc_13DC1:
    push [IMG_MainScr_Cont@]
    mov ax, [bp+Continue_Move_Down]
    mov dx, 0Ch
    imul dx
    mov dx, di
    add dx, ax
    push dx
    push si
    call VGA_DrawLBXImage
    add sp, 6

loc_13DDB:
    mov ax, [bp+Moused_Control]
    cmp ax, [GUI_NewGame_Label]
    jnz short loc_13DF0
    push [IMG_MainScr_NewG@]
    call LBX_IMG_ResetFrame
    pop cx
    jmp short loc_13DFF

loc_13DF0:
    mov ax, 1
    push ax
    push [IMG_MainScr_NewG@]
    call LBX_IMG_SetFrame
    pop cx
    pop cx

loc_13DFF:
    push [IMG_MainScr_NewG@]
    mov ax, di
    add ax, 18h
    push ax
    push si
    call VGA_DrawLBXImage
    add sp, 6
    mov ax, [bp+Moused_Control]
    cmp ax, [GUI_HoF_Lbl_Index]
    jnz short loc_13E27
    push [IMG_MainScr_HoF@]
    call LBX_IMG_ResetFrame
    pop cx
    jmp short loc_13E36

loc_13E27:
    mov ax, 1
    push ax
    push [IMG_MainScr_HoF@]
    call LBX_IMG_SetFrame
    pop cx
    pop cx

loc_13E36:
    push [IMG_MainScr_HoF@]
    mov ax, di
    add ax, 24h
    push ax
    push si
    call VGA_DrawLBXImage
    add sp, 6
    mov ax, [bp+Moused_Control]
    cmp ax, [GUI_Quit_Lbl_Index]
    jnz short loc_13E5E
    push [IMG_MainScr_Quit@]
    call LBX_IMG_ResetFrame
    pop cx
    jmp short loc_13E6D

loc_13E5E:
    mov ax, 1
    push ax
    push [IMG_MainScr_Quit@]
    call LBX_IMG_SetFrame
    pop cx
    pop cx

loc_13E6D:
    push [IMG_MainScr_Quit@]
    mov ax, di
    add ax, 30h
    push ax
    push si
    call VGA_DrawLBXImage
    add sp, 6
    mov ax, [CRP_Unused_Anim_Var]
    inc ax
    mov bx, 20
    cwd
    idiv bx
    mov [CRP_Unused_Anim_Var], dx
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp GAME_DrawMainScreen

;ends seg001

end
