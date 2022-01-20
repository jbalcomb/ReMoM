TITLE _s01p05.asm GAME_MainEventLoop

.MODEL LARGE, C

.CODE
; Segment type: Pure code
;segment seg001 byte public 'CODE' use16
;    assume cs:seg001
;    org 9
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GAME_MainEventLoop

proc GAME_MainEventLoop

    Found_File_Name= byte ptr -6Ch
    Conversion_String= byte ptr -4Eh
    File_Name= byte ptr -30h
    Load_Hotkey_Index= word ptr -12h
    HallofFame_Hotkey_Index= word ptr -10h
    Escape_Hotkey_Index= word ptr -0Eh
    First_Draw_Done= word ptr -0Ch
    Quit_Hotkey_Index= word ptr -0Ah
    NewGame_Hotkey_Index= word ptr -8
    Continue_Hotkey_Index= word ptr -6
    Continue_Move_Down= word ptr -4
    Loop_Var= word ptr -2

    push bp
    mov  bp, sp
    sub  sp, 6Ch
    push si
    push di

    db 83h,3Eh,0FCh,52h,0FFh ; <BAD>cmp [GUI_MainScreenJump], -1
    jz short loc_1380B
    mov ax, [GUI_MainScreenJump]
    mov [bp+Loop_Var], ax
    mov [GUI_MainScreenJump], -1
    mov ax, [bp+Loop_Var]

loc_13808:
    jmp @@Done

loc_1380B:
    mov [GUI_LoadSave_State], -1
    mov [bp+First_Draw_Done], 0
    mov [GAME_HaveContSave], 0
    mov [GUI_MainMenuSelected], -1
    mov [GAME_HaveSaves], 0
    mov [CRP_Unused_Anim_Var], 0
    xor di, di
    mov [NEWG_ScreenChangeFade], 1
    cmp [NEWG_ScreenChangeFade], 0
    jz short loc_13842
    call j_VGA_Fade_Out

loc_13842:
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
    call VGA_SetDirectDraw
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
    call VGA_SetDrawFrame
    mov ax, -1
    push ax
    mov ax, 2
    push ax
    call VGA_LoadPalette
    pop cx
    pop cx

    call j_GAME_LoadSettings
    mov [HLP_Entry_Table.Entry_Index], -1
    mov [word ptr unk_3A2CA], -1
    mov [word ptr unk_3A2D4], -1
    mov [word ptr unk_3A2DE], -1
    mov [word ptr unk_3A2E8], -1
    mov [GAME_ValidSaveCount], 0
    mov [bp+Loop_Var], 1
    jmp loc_13942

loc_138BE:
    mov ax, offset cnst_SAVE
    push ax
    lea ax, [bp+File_Name]
    push ax
    call strcpy
    pop cx
    pop cx
    mov ax, 0Ah
    push ax
    lea ax, [bp+Conversion_String]
    push ax
    push [bp+Loop_Var]
    call itoa
    add sp, 6
    lea ax, [bp+Conversion_String]
    push ax
    lea ax, [bp+File_Name]
    push ax
    call strcat
    pop cx
    pop cx

    mov ax, offset cnst_SAVE_ext
    push ax
    lea ax, [bp+File_Name]
    push ax
    call strcat
    pop cx
    pop cx

    lea ax, [bp+Found_File_Name]
    push ax
    lea ax, [bp+File_Name]
    push ax
    call DISK_FileFind
    pop cx
    pop cx

    or ax, ax
    jz short loc_13933
    cmp [bp+Loop_Var], 9
    jge short loc_1392B
    mov bx, [GAME_ValidSaveCount]
    shl bx, 1
    mov ax, [bp+Loop_Var]
    dec ax
    mov [GAME_ValidSaves_Main+bx], ax
    inc [GAME_ValidSaveCount]
    jmp short loc_13931

loc_1392B:
    mov [GAME_HaveContSave], 1

loc_13931:
    jmp short loc_1393F

loc_13933:
    mov bx, [GAME_ValidSaveCount]
    shl bx, 1
    mov [GAME_ValidSaves_Main+bx], -1

loc_1393F:
    inc [bp+Loop_Var]

loc_13942:
    cmp [bp+Loop_Var], 0Ah
    jge short loc_1394B
    jmp loc_138BE

loc_1394B:
    cmp [GAME_ValidSaveCount], 0
    jle short loc_13958
    mov [GAME_HaveSaves], 1

loc_13958:
    call GUI_Clear
    mov [bp+Continue_Move_Down], 0
    cmp [GAME_HaveSaves], 0
    jz short loc_139AC
    mov ax, -1
    push ax
    mov ax, offset cnst_ZeroString_4
    push ax
    mov ax, 161
    push ax
    mov ax, 211
    push ax
    mov ax, 150
    push ax
    mov ax, 108
    push ax
    call GUI_CreateClickLabel
    add sp, 0Ch
    mov [GUI_Load_Lbl_Index], ax
    mov [HLP_Entry_Table.Entry_Index], HLP_LOAD
    mov [HLP_Entry_Table.Left], 108
    mov [HLP_Entry_Table.Top], 148
    mov [HLP_Entry_Table.Right], 211
    mov [HLP_Entry_Table.Bottom], 162
    jmp short loc_139B7

loc_139AC:
    mov [GUI_Load_Lbl_Index], -1000
    mov [bp+Continue_Move_Down], 1

loc_139B7:
    cmp [GAME_HaveContSave], 0
    jz short loc_13A21
    mov ax, -1
    push ax
    mov ax, offset cnst_ZeroString_4
    push ax
    mov ax, [bp+Continue_Move_Down]
    mov dx, 12
    imul dx
    add ax, 149
    push ax
    mov ax, 211
    push ax
    mov ax, [bp+Continue_Move_Down]
    mov dx, 12
    imul dx
    add ax, 138
    push ax
    mov ax, 108
    push ax
    call GUI_CreateClickLabel
    add sp, 0Ch
    mov [GUI_Continue_Label], ax
    mov [word ptr unk_3A2CA], HLP_CONTINUE
    mov [word ptr unk_3A2CC], 108
    mov ax, [bp+Continue_Move_Down]
    mov dx, 12
    imul dx
    add ax, 138
    mov [word ptr unk_3A2CE], ax
    mov [word ptr unk_3A2D0], 211
    mov ax, [bp+Continue_Move_Down]
    mov dx, 12
    imul dx
    add ax, 149
    mov [word ptr unk_3A2D2], ax
    jmp short loc_13A27

loc_13A21:
    mov [GUI_Continue_Label], -1000

loc_13A27:
    mov ax, -1
    push ax
    mov ax, offset cnst_ZeroString_4
    push ax
    mov ax, 173
    push ax
    mov ax, 211
    push ax
    mov ax, 162
    push ax
    mov ax, 108
    push ax
    call GUI_CreateClickLabel
    add sp, 0Ch

    mov [GUI_NewGame_Label], ax
    mov [word ptr unk_3A2D4], HLP_NEW_GAME
    mov [word ptr unk_3A2D6], 108
    mov [word ptr unk_3A2D8], 162
    mov [word ptr unk_3A2DA], 211
    mov [word ptr unk_3A2DC], 173
    mov ax, -1
    push ax
    mov ax, offset cnst_ZeroString_4
    push ax
    mov ax, 185
    push ax
    mov ax, 211
    push ax
    mov ax, 174
    push ax
    mov ax, 108
    push ax
    call GUI_CreateClickLabel
    add sp, 0Ch

    mov [GUI_HoF_Lbl_Index], ax
    mov [word ptr unk_3A2DE], HLP_HALL_OF_FAME
    mov [word ptr unk_3A2E0], 108
    mov [word ptr unk_3A2E2], 174
    mov [word ptr unk_3A2E4], 211
    mov [word ptr unk_3A2E6], 185
    mov ax, -1
    push ax
    mov ax, offset cnst_ZeroString_4
    push ax
    mov ax, 199
    push ax
    mov ax, 211
    push ax
    mov ax, 186
    push ax
    mov ax, 108
    push ax
    call GUI_CreateClickLabel
    add sp, 0Ch

    mov [GUI_Quit_Lbl_Index], ax
    mov [word ptr unk_3A2E8], HLP_QUIT_TO_DOS
    mov [word ptr unk_3A2EA], 108
    mov [word ptr unk_3A2EC], 186
    mov [word ptr unk_3A2EE], 211
    mov [word ptr unk_3A2F0], 199
    cmp [GAME_HaveContSave], 0
    jz short loc_13B05
    mov ax, -1
    push ax
    mov ax, offset cnst_HOTKEY_C
    push ax
    call GUI_CreateHotkey
    pop cx
    pop cx

    mov [bp+Continue_Hotkey_Index], ax
    jmp short loc_13B0A

loc_13B05:
    mov [bp+Continue_Hotkey_Index], -1000

loc_13B0A:
    cmp [GAME_HaveSaves], 0
    jz short loc_13B25
    mov ax, -1
    push ax
    mov ax, offset cnst_HOTKEY_L
    push ax
    call GUI_CreateHotkey
    pop cx
    pop cx

    mov [bp+Load_Hotkey_Index], ax
    jmp short loc_13B2A

loc_13B25:
    mov [bp+Load_Hotkey_Index], -1000

loc_13B2A:
    mov ax, -1
    push ax
    mov ax, offset cnst_HOTKEY_N
    push ax
    call GUI_CreateHotkey
    pop cx
    pop cx

    mov [bp+NewGame_Hotkey_Index], ax
    mov ax, -1
    push ax
    mov ax, offset cnst_HOTKEY_H
    push ax
    call GUI_CreateHotkey
    pop cx
    pop cx

    mov [bp+HallofFame_Hotkey_Index], ax
    mov ax, -1
    push ax
    mov ax, offset cnst_HOTKEY_Q
    push ax
    call GUI_CreateHotkey
    pop cx
    pop cx

    mov [bp+Quit_Hotkey_Index], ax
    mov ax, -1
    push ax
    mov ax, offset cnst_HOTKEY_Esc
    push ax
    call GUI_CreateHotkey
    pop cx
    pop cx

    mov [bp+Escape_Hotkey_Index], ax
    mov ax, offset GUI_MainMenuWindow
    push ax
    mov ax, 1
    push ax
    call GUI_SetWindows
    pop cx
    pop cx

    call MOUSE_GetY
    push ax
    call MOUSE_GetX
    push ax
    call GUI_SaveCursorArea
    pop cx
    pop cx

    push [GUI_NewGame_Label]
    call GUI_MouseEMUMoveTo
    pop cx

    push [IMG_MainScr_Title@]
    call LBX_IMG_ResetFrame
    pop cx

    mov [GUI_MainMenuSelected], -1
    mov ax, 2
    push ax
    mov ax, seg seg001
    push ax
    mov ax, offset GAME_DrawMainScreen
    push ax
    call GUI_SetRedrawFn
    add sp, 6

    mov ax, 5
    push ax
    mov ax, offset HLP_Entry_Table
    push ax
    call GUI_SetHelp
    pop cx
    pop cx

    mov ax, 4
    push ax
    call GUI_SetDelay
    pop cx

    jmp loc_13C9D

loc_13BDE:
    call CLK_SaveCounter
    call GUI_GetInput
    mov si, ax
    or si, si
    jz short loc_13BF1
    mov di, 1

loc_13BF1:
    cmp si, [bp+Continue_Hotkey_Index]
    jz short loc_13BFC
    cmp si, [GUI_Continue_Label]
    jnz short loc_13C05

loc_13BFC:
    mov di, 1
    mov [GUI_MainMenuSelected], 0

loc_13C05:
    cmp si, [bp+Load_Hotkey_Index]
    jz short loc_13C10
    cmp si, [GUI_Load_Lbl_Index]
    jnz short loc_13C19

loc_13C10:
    mov di, 1
    mov [GUI_MainMenuSelected], 1

loc_13C19:
    cmp si, [bp+NewGame_Hotkey_Index]
    jz short loc_13C24
    cmp si, [GUI_NewGame_Label]
    jnz short loc_13C2D

loc_13C24:
    mov di, 1
    mov [GUI_MainMenuSelected], 2

loc_13C2D:
    cmp si, [bp+Quit_Hotkey_Index]
    jz short loc_13C3D
    cmp si, [GUI_Quit_Lbl_Index]
    jz short loc_13C3D
    cmp si, [bp+Escape_Hotkey_Index]
    jnz short loc_13C46

loc_13C3D:
    mov di, 1
    mov [GUI_MainMenuSelected], 3

loc_13C46:
    cmp si, [bp+HallofFame_Hotkey_Index]
    jz short loc_13C51
    cmp si, [GUI_HoF_Lbl_Index]
    jnz short loc_13C5A

loc_13C51:
    mov di, 1
    mov [GUI_MainMenuSelected], 4

loc_13C5A:
    db 83h,3Eh,0C4h,52h,0FFh ; <BAD>cmp [GUI_MainMenuSelected], -1
    jnz short loc_13C63
    xor di, di

loc_13C63:
    or di, di
    jnz short loc_13C9D
    nop
    push cs
    call near ptr GAME_DrawMainScreen
    call GUI_SimplePageFlip
    cmp [NEWG_ScreenChangeFade], 0
    jz short loc_13C93
    cmp [bp+First_Draw_Done], 0
    jnz short loc_13C93
    call j_VGA_Fade_In
    call GUI_DisplayToDraw
    mov [bp+First_Draw_Done], 1
    mov [NEWG_ScreenChangeFade], 0

loc_13C93:
    mov ax, 2
    push ax
    call CLK_Wait
    pop cx

loc_13C9D:
    or di, di
    jnz short loc_13CA4
    jmp loc_13BDE

loc_13CA4:
    call GUI_DisableRedraw
    call GUI_ClearHelp
    mov ax, [GUI_MainMenuSelected]
    jmp loc_13808

@@Done:
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp GAME_MainEventLoop

;ends seg001

end
