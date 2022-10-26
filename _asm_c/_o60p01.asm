TITLE _o60p01.asm GAME_DrawCredits
; MGC_DEF.H

.MODEL LARGE, C

.CODE
;segment ovr060 para public 'OVERLAY' use16
;	 assume cs:ovr060
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GAME_DrawCredits

proc GAME_DrawCredits far

Role_String_Width= word ptr -1Ch
Loop_Var= word ptr -1Ah
Credit_FadeIn_Colors= byte ptr -18h
Credit_FadeOut_Colors= byte ptr -0Eh
Color_Array= byte ptr -4
push bp
mov bp, sp
sub sp, 1Ch
loc_56246:
push si
push di
inc [GAME_CredsRollTimer]
cmp [GAME_CredsRollTimer], 30000
jle short loc_5625A
mov [GAME_CredsRollTimer], 70
loc_5625A:
cmp [GAME_CredsRollTimer], 70
jge short loc_56266
xor ax, ax
jmp loc_5640D
loc_56266:
mov [bp+Color_Array], 44h
mov [bp+Loop_Var], 0
jmp short loc_56283
loc_56271:
mov al, [byte ptr bp+Loop_Var]
add al, 0E0h
lea dx, [bp+Credit_FadeOut_Colors]
mov bx, [bp+Loop_Var]
add bx, dx
mov [bx], al
inc [bp+Loop_Var]
loc_56283:
cmp [bp+Loop_Var], 0Ah
jl short loc_56271
mov [bp+Loop_Var], 0
jmp short loc_562A2
loc_56290:
mov al, 0E9h
sub al, [byte ptr bp+Loop_Var]
lea dx, [bp+Credit_FadeIn_Colors]
mov bx, [bp+Loop_Var]
add bx, dx
mov [bx], al
inc [bp+Loop_Var]
loc_562A2:
cmp [bp+Loop_Var], 0Ah
jl short loc_56290
mov [bp+Credit_FadeIn_Colors], 0E8h
push [GUI_Credits_Text_IMG]
mov ax, 122
push ax
mov ax, 280
push ax
call LBX_Image_Prepare
add sp, 6
mov ax, 137
push ax
mov ax, 319
push ax
mov ax, 40
push ax
xor ax, ax
push ax
call VGA_SetDrawWindow
add sp, 8
mov si, [GAME_CreditTextTop]
mov [bp+Loop_Var], 0
jmp loc_563BE
loc_562E3:
cmp si, 99
jl short loc_562EB
jmp loc_563BB
loc_562EB:
mov ax, [GAME_FirstShownCred]
add ax, [bp+Loop_Var]
cmp ax, 48
jl short loc_562F9
jmp loc_563BB
loc_562F9:
mov [bp+Color_Array+1], 0E0h
cmp si, 10
jge short loc_56311
mov bx, 10
sub bx, si
lea ax, [bp+Credit_FadeOut_Colors]
add bx, ax
mov al, [bx]
mov [bp+Color_Array+1], al
loc_56311:
cmp si, 88
jle short loc_5632A
mov ax, si
add ax, -88
mov bx, 10
sub bx, ax
lea ax, [bp+Credit_FadeIn_Colors]
add bx, ax
mov al, [bx]
mov [bp+Color_Array+1], al
loc_5632A:
lea ax, [bp+Color_Array]
push ax
mov ax, 2
push ax
call VGA_SetFontnColor
pop cx
pop cx
push [GUI_Credits_Text_IMG]
mov bx, [GAME_FirstShownCred]
add bx, [bp+Loop_Var]
mov cl, 2
shl bx, cl
push [word ptr TBL_Credits_Texts.Role+bx]
push si
xor ax, ax
push ax
call LBX_DrawTextLine
add sp, 8
mov bx, [GAME_FirstShownCred]
add bx, [bp+Loop_Var]
mov cl, 2
shl bx, cl
push [word ptr TBL_Credits_Texts.Role+bx]
call VGA_GetStringWidth
pop cx
mov [bp+Role_String_Width], ax
cmp [bp+Role_String_Width], 1
jle short loc_56398
mov ax, [bp+Role_String_Width]
inc ax
mov di, ax
jmp short loc_56393
loc_5637E:
push [GUI_Credits_Text_IMG]
mov ax, offset cnst_Credits_DOT
push ax
push si
push di
call LBX_DrawTextLine
add sp, 8
add di, 4
loc_56393:
cmp di, 102
jl short loc_5637E
loc_56398:
push [GUI_Credits_Text_IMG]
mov bx, [GAME_FirstShownCred]
add bx, [bp+Loop_Var]
mov cl, 2
shl bx, cl
push [TBL_Credits_Texts.Name+bx]
push si
mov ax, 105
push ax
call LBX_DrawTextLine
add sp, 8
add si, 10
loc_563BB:
inc [bp+Loop_Var]
loc_563BE:
cmp [bp+Loop_Var], 10
jge short loc_563C7
jmp loc_562E3
loc_563C7:
dec [GAME_CreditTextTop]
cmp [GAME_CreditTextTop], 1
jge short loc_563F4
inc [GAME_FirstShownCred]
add [GAME_CreditTextTop], 10
cmp [GAME_FirstShownCred], 48
jle short loc_563F4
mov [GAME_CredsRollTimer], 0
mov [GAME_FirstShownCred], 0
mov [GAME_CreditTextTop], 95
loc_563F4:
push [GUI_Credits_Text_IMG]
mov ax, 35
push ax
mov ax, 85
push ax
call VGA_WndOverlayLBX
add sp, 6
call VGA_ResetDrawWindow
loc_5640D:
pop di
pop si
mov sp, bp
pop bp
retf

endp GAME_DrawCredits

;ends ovr060

end
