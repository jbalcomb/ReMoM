TITLE _s34p.asm 

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
;    assume cs:seg034
;    ;org 0Eh
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc CRP_GUI_KeyInputOnly far
Input_Key= byte ptr -0Bh
Valid_Char= word ptr -0Ah
Keyed_Control= word ptr -8
Pointer_Y= word ptr -6
Pointer_X= word ptr -4
Control_At_Pointer= word ptr -2
push bp
mov bp, sp
sub sp, 0Ch
push si
push di
mov ax, [MOUSE_Emu_X]
mov [bp+Pointer_X], ax
mov ax, [MOUSE_Emu_Y]
mov [bp+Pointer_Y], ax
call VGA_RestoreCursrArea
push [MOUSE_Emu_Y]
push [MOUSE_Emu_X]
call GUI_SaveCursorArea
pop cx
pop cx
push [MOUSE_Emu_Y]
push [MOUSE_Emu_X]
call GUI_DisplayCursor
pop cx
pop cx
push [bp+Pointer_Y]
push [bp+Pointer_X]
call GUI_GetCursorIndex
pop cx
pop cx
call GUI_GetCursorOffset
mov [GUI_Cursor_Offset], ax
mov [bp+Control_At_Pointer], 0
nop
push cs
call near ptr GUI_MouseOnControl
mov [bp+Control_At_Pointer], ax
mov [bp+Keyed_Control], 0
mov [bp+Input_Key], 0
call KBD_CheckBuffer
or ax, ax
jnz short loc_26094
jmp loc_26680
loc_26094:
lea ax, [bp+Keyed_Control]
push ax
nop
push cs
call near ptr GUI_NoTriggerKeys
pop cx
mov [bp+Input_Key], al
cmp [bp+Input_Key], KP_F11
jnz short loc_260AC
call DBG_Quit
loc_260AC:
cmp [bp+Input_Key], KP_F12
jnz short loc_260F5
call VGA_RestoreCursrArea
call DBG_ScreenDump
call MOUSE_GetY
push ax
call MOUSE_GetX
push ax
call GUI_SaveCursorArea
pop cx
pop cx
call MOUSE_GetY
push ax
call MOUSE_GetX
push ax
call GUI_DisplayCursor
pop cx
pop cx
call MOUSE_GetY
push ax
call MOUSE_GetX
push ax
call MOUSE_MoveCursor
pop cx
pop cx
loc_260F5:
cmp [bp+Input_Key], KP_Esc
jnz short loc_26101
mov ax, -1
loc_260FE:
jmp loc_26695
loc_26101:
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov al, [es:bx+GUI_CTRL.Hotkey]
cmp al, [bp+Input_Key]
jz short loc_2611B
jmp loc_262B8
loc_2611B:
cmp [bp+Input_Key], 0
jnz short loc_26124
jmp loc_26690
loc_26124:
cmp [bp+Keyed_Control], 0
jnz short loc_2612D
jmp loc_262A9
loc_2612D:
mov ax, [MOUSE_Emu_X]
mov [bp+Pointer_X], ax
mov ax, [MOUSE_Emu_Y]
mov [bp+Pointer_Y], ax
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Right]
push ax
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
pop ax
sub ax, [es:bx+GUI_CTRL.Left]
cwd
sub ax, dx
sar ax, 1
push ax
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Left]
pop dx
add ax, dx
mov [MOUSE_Emu_X], ax
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Bottom]
push ax
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
pop ax
sub ax, [es:bx+GUI_CTRL.Top]
cwd
sub ax, dx
sar ax, 1
push ax
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Top]
pop dx
add ax, dx
mov [MOUSE_Emu_Y], ax
cmp [MOUSE_Emu_X], 0
jl short loc_261CE
cmp [MOUSE_Emu_X], 319
jle short loc_261D4
loc_261CE:
mov ax, [bp+Pointer_X]
mov [MOUSE_Emu_X], ax
loc_261D4:
cmp [MOUSE_Emu_Y], 0
jl short loc_261E3
cmp [MOUSE_Emu_Y], 199
jle short loc_261E9
loc_261E3:
mov ax, [bp+Pointer_Y]
mov [MOUSE_Emu_Y], ax
loc_261E9:
push [MOUSE_Emu_Y]
push [MOUSE_Emu_X]
push [bp+Keyed_Control]
nop
push cs
call near ptr GUI_SetFocus
add sp, 6
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Ctrl_Type]
cmp ax, Ctrl_ToggleButton
jz short loc_26260
cmp ax, Ctrl_LockableButton
jz short loc_26220
cmp ax, Ctrl_ClickLink
jz short loc_2625A
jmp loc_262A7
loc_26220:
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov bx, [es:bx+GUI_CTRL.Param2]
cmp [word ptr bx], 0
jnz short loc_2624F
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov bx, [es:bx+GUI_CTRL.Param2]
mov [word ptr bx], 1
jmp short loc_26258
loc_2624F:
mov [GUI_FocusedControl], -1
jmp loc_26690
loc_26258:
jmp short loc_262A9
loc_2625A:
mov ax, [bp+Keyed_Control]
jmp loc_2646B
loc_26260:
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov bx, [es:bx+GUI_CTRL.Param2]
cmp [word ptr bx], 0
jnz short loc_2628F
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov bx, [es:bx+GUI_CTRL.Param2]
mov [word ptr bx], 1
jmp short loc_262A5
loc_2628F:
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov bx, [es:bx+GUI_CTRL.Param2]
mov [word ptr bx], 0
loc_262A5:
jmp short loc_262A9
loc_262A7:
jmp short $+2
loc_262A9:
mov [GUI_FocusedControl], -1
mov ax, [bp+Keyed_Control]
jmp loc_260FE
jmp loc_26680
loc_262B8:
cmp [bp+Input_Key], 0Ch
jz short loc_262C1
jmp loc_2653E
loc_262C1:
mov ax, [MOUSE_Emu_X]
mov [bp+Pointer_X], ax
mov ax, [MOUSE_Emu_Y]
mov [bp+Pointer_Y], ax
nop
push cs
call near ptr GUI_MouseOnControl
mov si, ax
or si, si
jg short loc_262DB
jmp loc_263D4
loc_262DB:
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditSelect
jnz short loc_2635E
cmp [GUI_InEditSelect], 0
jnz short loc_2632A
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
push [es:bx+GUI_CTRL.Param0]
mov ax, offset GUI_EditString
push ax
call strcpy
pop cx
pop cx
mov [GUI_EditAnimStage], 0
mov [GUI_EditCursorOn], 0
mov [GUI_InEditSelect], 1
mov [GUI_Active_EditSlct], si
jmp short loc_2635C
loc_2632A:
cmp [GUI_Active_EditSlct], si
jz short loc_2635C
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
push [es:bx+GUI_CTRL.Param0]
mov ax, offset GUI_EditString
push ax
call strcpy
pop cx
pop cx
mov [GUI_EditAnimStage], 0
mov [GUI_EditCursorOn], 0
mov [GUI_Active_EditSlct], si
loc_2635C:
jmp short loc_263D4
loc_2635E:
cmp [GUI_InEditSelect], 0
jz short loc_263D4
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditSelect
jz short loc_263D4
xor di, di
jmp short loc_2637E
loc_2637D:
inc di
loc_2637E:
cmp [GUI_EditString+di], 0
jz short loc_26398
mov ax, di
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Param5], di
jg short loc_2637D
loc_26398:
cmp GUI_EditString[di-1], '_'
jnz short loc_263A0
dec di
loc_263A0:
mov [GUI_EditString+di], 0
mov ax, offset GUI_EditString
push ax
mov ax, [GUI_Active_EditSlct]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
push [es:bx+GUI_CTRL.Param0]
call strcpy
pop cx
pop cx
mov [GUI_EditAnimStage], 0
mov [GUI_InEditSelect], 0
mov [GUI_Active_EditSlct], -1
loc_263D4:
or si, si
jnz short loc_263DB
jmp loc_264D3
loc_263DB:
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_Slidebar
jz short loc_26414
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditSelect
jz short loc_26414
push [MOUSE_Emu_Y]
push [MOUSE_Emu_X]
push si
nop
push cs
call near ptr GUI_SetFocus
add sp, 6
loc_26414:
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Ctrl_Type]
cmp ax, Ctrl_ToggleButton
jz short loc_2647D
cmp ax, Ctrl_LockableButton
jz short loc_26437
cmp ax, Ctrl_ClickLink
jz short loc_26469
jmp loc_264C2
loc_26437:
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov bx, [es:bx+GUI_CTRL.Param2]
cmp [word ptr bx], 0
jnz short loc_26464
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov bx, [es:bx+GUI_CTRL.Param2]
mov [word ptr bx], 1
jmp short loc_26467
loc_26464:
jmp loc_2624F
loc_26467:
jmp short loc_264C4
loc_26469:
mov ax, si
loc_2646B:
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Param0]
jmp loc_260FE
loc_2647D:
mov ax, [bp+Keyed_Control]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov bx, [es:bx+GUI_CTRL.Param2]
cmp [word ptr bx], 0
jnz short loc_264AB
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov bx, [es:bx+GUI_CTRL.Param2]
mov [word ptr bx], 1
jmp short loc_264C0
loc_264AB:
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov bx, [es:bx+GUI_CTRL.Param2]
mov [word ptr bx], 0
loc_264C0:
jmp short loc_264C4
loc_264C2:
jmp short $+2
loc_264C4:
cmp [GUI_ClickActivate], 0
jnz short loc_264D0
nop
push cs
call near ptr GUI_1TickRedraw
loc_264D0:
jmp loc_26583
loc_264D3:
cmp [GUI_DialogDirections], 0
jz short loc_2653E
mov si, 1
jmp short loc_26538
loc_264DF:
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_DialogLine
jnz short loc_26537
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov bx, [es:bx+GUI_CTRL.Param2]
mov ax, [bx]
push ax
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
pop ax
cmp ax, [es:bx+GUI_CTRL.Param1]
jnz short loc_26537
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Selectable], 0
jz short loc_26537
mov [GUI_FocusedControl], -1
jmp short loc_26583
loc_26537:
inc si
loc_26538:
cmp si, [GUI_Control_Count]
jl short loc_264DF
loc_2653E:
cmp [bp+Input_Key], '+'
jz short loc_2654A
cmp [bp+Input_Key], '-'
jnz short loc_2658B
loc_2654A:
nop
push cs
call near ptr GUI_MouseOnControl
mov si, ax
or si, si
jz short loc_2658B
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_Slidebar
jnz short loc_26588
cmp [bp+Input_Key], '+'
jnz short loc_26576
push si
nop
push cs
call near ptr GUI_SlideUp
pop cx
loc_26576:
cmp [bp+Input_Key], '-'
jnz short loc_26583
push si
nop
push cs
call near ptr GUI_SlideDown
pop cx
loc_26583:
mov ax, si
jmp loc_260FE
loc_26588:
jmp loc_26690
loc_2658B:
cmp [GUI_InEditSelect], 0
jg short loc_26595
jmp loc_26680
loc_26595:
cmp [bp+Input_Key], 0Bh
jnz short loc_2659E
jmp loc_26647
loc_2659E:
mov [bp+Valid_Char], 0
cmp [bp+Input_Key], 40h
jle short loc_265AF
cmp [bp+Input_Key], 5Dh
jl short loc_265C7
loc_265AF:
cmp [bp+Input_Key], 2Dh
jle short loc_265BB
cmp [bp+Input_Key], 3Bh
jl short loc_265C7
loc_265BB:
cmp [bp+Input_Key], 20h
jz short loc_265C7
cmp [bp+Input_Key], 2Dh
jnz short loc_265CE
loc_265C7:
mov [bp+Valid_Char], 1
jmp short loc_265DF
loc_265CE:
cmp [bp+Input_Key], 60h
jle short loc_265DF
cmp [bp+Input_Key], 7Bh
jge short loc_265DF
mov [bp+Valid_Char], 1
loc_265DF:
cmp [bp+Valid_Char], 1
jnz short loc_26645
xor si, si
jmp short loc_265EA
loc_265E9:
inc si
loc_265EA:
cmp [GUI_EditString+si], '_'
jz short loc_2660F
cmp [GUI_EditString+si], 0
jz short loc_2660F
mov ax, [GUI_Active_EditSlct]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Param5]
dec ax
cmp ax, si
jg short loc_265E9
loc_2660F:
mov ax, [GUI_Active_EditSlct]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Param5]
dec ax
cmp ax, si
jle short loc_26634
mov al, [bp+Input_Key]
mov [GUI_EditString+si], al
mov GUI_EditString[si+1], 0
jmp short loc_26639
loc_26634:
mov [GUI_EditString+si], 0
loc_26639:
mov [GUI_EditCursorOn], 0
mov [GUI_EditAnimStage], 0
loc_26645:
jmp short loc_26680
loc_26647:
xor si, si
jmp short loc_2664C
loc_2664B:
inc si
loc_2664C:
cmp [GUI_EditString+si], 0
jz short loc_26671
cmp [GUI_EditString+si], '_'
jz short loc_26671
mov ax, [GUI_Active_EditSlct]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
mov ax, [es:bx+GUI_CTRL.Param5]
dec ax
cmp ax, si
jg short loc_2664B
loc_26671:
or si, si
jle short loc_26680
mov GUI_EditString[si-1], 0
mov [GUI_EditAnimStage], 0
loc_26680:
call MOUSE_GetX
mov [MOUSE_Emu_X], ax
call MOUSE_GetY
mov [MOUSE_Emu_Y], ax
loc_26690:
xor ax, ax
jmp loc_260FE
loc_26695:
pop di
pop si
mov sp, bp
pop bp
retf
endp CRP_GUI_KeyInputOnly

;ends seg034

end
