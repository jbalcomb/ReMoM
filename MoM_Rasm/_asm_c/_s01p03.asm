
proc GAME_MainMenu far

Read_Buffer= byte ptr -20h
Event_Return_Value= word ptr -2

push	bp
mov	bp, sp
sub	sp, 20h
push	si
push	di
lea	ax, [bp+Read_Buffer]
push	ax
mov	ax, offset cnst_ConfigFile
push	ax
call	DISK_FileFind

pop	cx
pop	cx
or	ax, ax
jnz	short loc_13664

mov	ax, offset cnst_ConfigErr
push	ax
call	GAME_QuitProgram

; ---------------------------------------------------------------------------
pop	cx


loc_13664:
mov	ax, offset cnst_RB
push	ax
mov	ax, offset cnst_ConfigFile
push	ax

loc_1366C:
call	fopen

pop	cx
pop	cx
mov	si, ax
push	si
mov	ax, 1
push	ax
mov	ax, 12h
push	ax
mov	ax, offset Config_Data
push	ax
call	fread

add	sp, 8
push	si
call	_fclose

pop	cx
xor	di, di
mov	[NEWG_ScreenChangeFade], 1
mov	[GUI_MainScreenJump], -1
call	GUI_SetEscOverride

jmp	short loc_1370C

; ---------------------------------------------------------------------------

loc_136A6:
xor	ax, ax
push	ax
call	VGA_SetOutlineColor

pop	cx
call	GUI_ClearHelp

nop
push	cs
call	near ptr GAME_MainEventLoop

mov	[bp+Event_Return_Value], ax
call	GUI_ClearHelp

mov	[NEWG_ScreenChangeFade], 0
mov	bx, [bp+Event_Return_Value]
cmp	bx, 4
ja	short loc_1370C

shl	bx, 1
jmp	[cs:off_13720+bx]


loc_136D6:
call	SND_Stop_Music

mov	ax, 8
push	ax
call	j_GAME_WizardsLaunch

; ---------------------------------------------------------------------------
pop	cx


loc_136E5:
mov	[GUI_LoadSave_State], -1
jmp	short loc_1370C

; ---------------------------------------------------------------------------

loc_136ED:
call	j_GAME_LoadSaveScreen

jmp	short loc_1370C

; ---------------------------------------------------------------------------

loc_136F4:
call	j_GAME_New_Create

jmp	short loc_136E5

; ---------------------------------------------------------------------------

loc_136FB:
mov	di, 1
jmp	short loc_136E5

; ---------------------------------------------------------------------------

loc_13700:
call	j_GAME_Hall_of_Fame

call	j_GAME_PrepareCredits

jmp	short $+2


loc_1370C:
or	di, di
jz	short loc_136A6

mov	ax, offset cnst_QUIT_Message
push	ax
call	GAME_QuitProgram

; ---------------------------------------------------------------------------
pop	cx
pop	di
pop	si
mov	sp, bp
pop	bp
retf

endp GAME_MainMenu

; ---------------------------------------------------------------------------
off_13720 dw offset loc_136D6
dw offset loc_136ED
dw offset loc_136F4
dw offset loc_136FB
dw offset loc_13700
