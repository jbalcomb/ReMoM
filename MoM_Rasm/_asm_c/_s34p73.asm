TITLE _s34p73.asm GUI_SetFocus
; ST_GUI.H

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_SetFocus

proc GUI_SetFocus far
Ctrl_Index= word ptr 6
X_Pos= word ptr 8
Y_Pos= word ptr 0Ah
push bp
mov bp, sp
push si
push di
mov si, [bp+Ctrl_Index]
mov di, [bp+X_Pos]
or di, di
jge short loc_2E01C
jmp loc_2E1E1
loc_2E01C:
cmp di, 140h
jl short loc_2E025
jmp loc_2E1E1
loc_2E025:
cmp [bp+Y_Pos], 0
jge short loc_2E02E
jmp loc_2E1E1
loc_2E02E:
cmp [bp+Y_Pos], 0C8h
jl short loc_2E038
jmp loc_2E1E1
loc_2E038:
cmp [MOUSE_Hardware], 0
jnz short loc_2E042
jmp loc_2E19C
loc_2E042:
call MOUSE_Disable_CDraw
cmp si, [GUI_FocusedControl]
jnz short loc_2E050
jmp loc_2E195
loc_2E050:
db 83h,3Eh,22h,4Dh,0FFh ; <BAD>cmp [GUI_FocusedControl], -1
jnz short loc_2E0B2
call VGA_RestoreCursrArea
mov [GUI_FocusedControl], si
mov ax, 1
push ax
push si
push cs
call near ptr GUI_DrawControl
pop cx
pop cx
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditBox
jnz short loc_2E08E
call MOUSE_GetX
mov di, ax
call MOUSE_GetY
mov [bp+Y_Pos], ax
loc_2E08E:
push [bp+Y_Pos]
push di
call GUI_SaveCursorArea
pop cx
pop cx
push [bp+Y_Pos]
push di
call GUI_DisplayCursor
pop cx
pop cx
push [bp+Y_Pos]
push di
call MOUSE_MoveCursor
pop cx
pop cx
jmp loc_2E195
loc_2E0B2:
cmp [GUI_FocusedControl], si
jnz short loc_2E0BB
jmp loc_2E195
loc_2E0BB:
call VGA_RestoreCursrArea
mov ax, [GUI_FocusedControl]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_MStateButton
jnz short loc_2E0E9
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_MStateButton
jnz short loc_2E142
loc_2E0E9:
mov ax, [GUI_FocusedControl]
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_DialogLine
jnz short loc_2E135
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_DialogLine
jnz short loc_2E133
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Selectable], 0
jz short loc_2E133
xor ax, ax
push ax
push [GUI_FocusedControl]
push cs
call near ptr GUI_DrawControl
pop cx
pop cx
loc_2E133:
jmp short loc_2E142
loc_2E135:
xor ax, ax
push ax
push [GUI_FocusedControl]
push cs
call near ptr GUI_DrawControl
pop cx
pop cx
loc_2E142:
mov [GUI_FocusedControl], si
mov ax, 1
push ax
push si
push cs
call near ptr GUI_DrawControl
pop cx
pop cx
mov ax, si
mov dx, 26h
imul dx
les bx, [GUI_Control_Table@]
add bx, ax
cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditBox
jnz short loc_2E174
call MOUSE_GetX
mov di, ax
call MOUSE_GetY
mov [bp+Y_Pos], ax
loc_2E174:
push [bp+Y_Pos]
push di
call GUI_SaveCursorArea
pop cx
pop cx
push [bp+Y_Pos]
push di
call GUI_DisplayCursor
pop cx
pop cx
push [bp+Y_Pos]
push di
call MOUSE_MoveCursor
pop cx
pop cx
loc_2E195:
call MOUSE_CDraw_Restore
jmp short loc_2E1E1
loc_2E19C:
call VGA_RestoreCursrArea
call VGA_SetDirectDraw
mov [GUI_FocusedControl], si
mov ax, 1
push ax
push si
push cs
call near ptr GUI_DrawControl
pop cx
pop cx
mov [GUI_FocusedControl], -1
call VGA_SetDrawFrame
push [bp+Y_Pos]
push di
call GUI_SaveCursorArea
pop cx
pop cx
push [bp+Y_Pos]
push di
call GUI_DisplayCursor
pop cx
pop cx
push [bp+Y_Pos]
push di
call MOUSE_MoveCursor
pop cx
pop cx
loc_2E1E1:
pop di
pop si
pop bp
retf
endp GUI_SetFocus

;ends seg034

end
