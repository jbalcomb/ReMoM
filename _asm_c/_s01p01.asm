TITLE _s01p0.asm 

.MODEL LARGE, C

.CODE
; Segment type: Pure code
;segment seg001 byte public 'CODE' use16
;    assume cs:seg001
;    org 9
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc _main far
Temp_String= byte ptr -5Ch
File_Name= byte ptr -3Eh
SFX_HW= word ptr -16h
MIDI_DRV= word ptr -14h
SFX_DMA= word ptr -12h
SFX_IRQ= word ptr -10h
SFX_IO= word ptr -0Eh
SFX_DRV= word ptr -0Ch
MIDI_IRQ= word ptr -0Ah
MIDI_IO= word ptr -8
LoopVar= word ptr -6
File_Handle= word ptr -4
MainMenu_Music_Seg= word ptr -2
c0argc= word ptr 6
c0argv= word ptr 8
push bp
mov bp, sp
sub sp, 5Ch
push si
push di
mov [EMM_Reserved_Left], 9Eh
lea ax, [bp+Temp_String]
push ax
mov ax, offset cnst_ConfigFile
push ax
call DISK_FileFind
pop cx
pop cx
or ax, ax
jnz short loc_13234
mov ax, offset cnst_ConfigErr
push ax
call GAME_QuitProgram
; ---------------------------------------------------------------------------
pop cx
loc_13234:
mov ax, offset cnst_RB
push ax
mov ax, offset cnst_ConfigFile
push ax
call fopen
pop cx
pop cx
mov [bp+File_Handle], ax
push [bp+File_Handle]
mov ax, 1
push ax
mov ax, 12h
push ax
mov ax, offset Config_Data
push ax
call fread
add sp, 8
push [bp+File_Handle]
call _fclose
pop cx
call j_GAME_LoadSettings
mov ax, [Config_Data.MIDI_IO]
mov [bp+MIDI_IO], ax
mov ax, [Config_Data.MIDI_IRQ]
mov [bp+MIDI_IRQ], ax
mov di, -1
mov ax, [Config_Data.MIDI_HW]
mov [bp+MIDI_DRV], ax
mov cx, 0Ch
mov bx, offset word_1360A
loc_13286:
mov ax, [cs:bx]
cmp ax, [bp+MIDI_DRV]
jz short loc_13295
add bx, 2
loop loc_13286
jmp short loc_132DE
; ---------------------------------------------------------------------------
loc_13295:
jmp [word ptr cs:bx+18h]
loc_13299:
xor si, si
loc_1329B:
mov [bp+MIDI_IO], -1
loc_132A0:
mov [bp+MIDI_IRQ], -1
loc_132A5:
mov di, -1
jmp short loc_132DE
; ---------------------------------------------------------------------------
loc_132AA:
mov si, SND_Speaker
jmp short loc_1329B
; ---------------------------------------------------------------------------
loc_132AF:
mov si, SND_AdLib
jmp short loc_1329B
; ---------------------------------------------------------------------------
loc_132B4:
mov si, SND_SB
jmp short loc_132A0
; ---------------------------------------------------------------------------
loc_132B9:
mov si, SND_SBPro
jmp short loc_132A0
; ---------------------------------------------------------------------------
loc_132BE:
mov si, SND_SBPro2
jmp short loc_132A0
; ---------------------------------------------------------------------------
loc_132C3:
mov si, SND_PAS
jmp short loc_1329B
; ---------------------------------------------------------------------------
loc_132C8:
mov si, SND_PAS16
jmp short loc_1329B
; ---------------------------------------------------------------------------
loc_132CD:
mov si, SND_ALG
jmp short loc_1329B
; ---------------------------------------------------------------------------
loc_132D2:
jmp short $+2
loc_132D4:
mov si, SND_Roland
jmp short loc_132A5
; ---------------------------------------------------------------------------
loc_132D9:
mov si, SND_GMIDI
jmp short loc_132A5
; ---------------------------------------------------------------------------
loc_132DE:
cmp si, SND_Roland
jl short loc_132FC
mov ax, offset cnst_MIDI_Init
push ax
call DOS_PrintString
pop cx
call CLK_SaveCounter
mov ax, 2
push ax
call CLK_Wait
pop cx
loc_132FC:
mov ax, [Config_Data.SFX_IO]
mov [bp+SFX_IO], ax
mov ax, [Config_Data.SFX_IRQ]
mov [bp+SFX_IRQ], ax
mov ax, [Config_Data.SFX_DMA]
mov [bp+SFX_DMA], ax
mov ax, [Config_Data.SFX_HW]
mov [bp+SFX_HW], ax
mov cx, 0Ch
mov bx, offset word_135DA
loc_1331A:
mov ax, [cs:bx]
cmp ax, [bp+SFX_HW]
jz short loc_13329
add bx, 2
loop loc_1331A
jmp short @@loc_13394
; ---------------------------------------------------------------------------
loc_13329:
jmp [word ptr cs:bx+18h]
loc_1332D:
mov [bp+SFX_DRV], SND_None
loc_13332:
mov [bp+SFX_IO], -1
mov [bp+SFX_IRQ], -1
mov [bp+SFX_DMA], -1
jmp short @@loc_13394
; ---------------------------------------------------------------------------
loc_13343:
mov [bp+SFX_DRV], SND_Speaker
jmp short loc_13332
; ---------------------------------------------------------------------------
loc_1334A:
jmp short loc_1332D
; ---------------------------------------------------------------------------
loc_1334C:
mov [bp+SFX_DRV], SND_SB
jmp short @@loc_13394
; ---------------------------------------------------------------------------
loc_13353:
mov [bp+SFX_DRV], SND_SBPro
jmp short @@loc_13394
; ---------------------------------------------------------------------------
loc_1335A:
mov [bp+SFX_DRV], SND_SBPro2
mov ax, [bp+SFX_IRQ]
mov [bp+MIDI_IRQ], ax
mov di, [bp+SFX_DMA]
jmp short @@loc_13394
; ---------------------------------------------------------------------------
loc_1336A:
mov [bp+SFX_DRV], SND_PAS
jmp short loc_13376
; ---------------------------------------------------------------------------
loc_13371:
mov [bp+SFX_DRV], SND_PAS16
loc_13376:
mov [bp+SFX_IO], -1
jmp short @@loc_13394
; ---------------------------------------------------------------------------
loc_1337D:
mov [bp+SFX_DRV], SND_ALG
jmp short @@loc_13394
; ---------------------------------------------------------------------------
loc_13384:
jmp short $+2
loc_13386:
mov [bp+SFX_DRV], SND_Roland
jmp short loc_13332
; ---------------------------------------------------------------------------
loc_1338D:
mov [bp+SFX_DRV], SND_GMIDI
jmp short loc_13332
; ---------------------------------------------------------------------------
@@loc_13394:
cmp si, SND_AdLib
jnz short @@loc_1339E
mov [bp+SFX_DRV], SND_AdLib
@@loc_1339E:
cmp [Settings.Snd_Channels], 2
jg short @@loc_133AC
cmp [Settings.Snd_Channels], 0
jge short @@loc_133B2
@@loc_133AC:
mov [Settings.Snd_Channels], 0
@@loc_133B2:
mov [Settings.Input_Type], 1
mov [bp+LoopVar], 1
jmp @@loc_1347B
; ---------------------------------------------------------------------------
loc_133C0:
mov bx, [bp+LoopVar]
shl bx, 1
cmp Settings.Using_Saves[bx-2], 0
jnz short loc_133CF
jmp loc_13478
; ---------------------------------------------------------------------------
loc_133CF:
mov ax, 0Ah
push ax
lea ax, [bp+Temp_String]
push ax
push [bp+LoopVar]
call itoa
add sp, 6
mov ax, offset cnst_SAVE
push ax
lea ax, [bp+File_Name]
push ax
call strcpy
pop cx
pop cx
lea ax, [bp+Temp_String]
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
lea ax, [bp+Temp_String]
push ax
lea ax, [bp+File_Name]
push ax
call DISK_FileFind
pop cx
pop cx
cmp [bp+Temp_String], 0
jnz short loc_13478
mov bx, [bp+LoopVar]
shl bx, 1
mov Settings.Using_Saves[bx-2], 0
mov ax, offset cnst_ZeroString_4
push ax
mov ax, [bp+LoopVar]
mov dx, 14h
imul dx
add ax, offset Settings.Save_Names-14h
push ax
call strcpy
pop cx
pop cx
mov ax, offset cnst_WB
push ax
mov ax, offset cnst_Set_File
push ax
call fopen
pop cx
pop cx
mov [bp+File_Handle], ax
push [bp+File_Handle]
mov ax, 1
push ax
mov ax, 1D2h
push ax
mov ax, offset Settings
push ax
call fwrite
add sp, 8
push [bp+File_Handle]
call _fclose
pop cx
loc_13478:
inc [bp+LoopVar]
@@loc_1347B:
cmp [bp+LoopVar], 9
jge short loc_13484
jmp loc_133C0
; ---------------------------------------------------------------------------
loc_13484:
mov ax, e_MemMinEms
push ax
call EMM_SetMinKbytes
pop cx
mov ax, e_MemMinConv
push ax
call RAM_SetMinKbytes
pop cx
mov [Settings.Input_Type], 1
mov [Settings.Snd_Channels], 2
push [bp+SFX_DMA]
push [bp+SFX_IRQ]
push [bp+SFX_IO]
push [bp+SFX_DRV]
push di
push [bp+MIDI_IRQ]
push [bp+MIDI_IO]
push si
mov ax, offset cnst_FONT_File
push ax
push [Settings.Snd_Channels]
push [Settings.Input_Type]
call Hardware_Init
add sp, 16h
call DBG_Disable
mov ax, e_GfxBufferSize
push ax
call j_LBX_Tables_Init
pop cx
call GUI_SetEscOverride
mov ax, 0FFFFh
push ax
xor ax, ax
push ax
call VGA_LoadPalette
pop cx
pop cx
call VGA_DAC_Write
mov bx, [bp+c0argv]
mov bx, [bx+2]
cmp [byte ptr bx], 'J'
jnz short @@PlayIntro
mov bx, [bp+c0argv]
mov bx, [bx+2]
cmp [byte ptr bx+1], 'E'
jnz short @@PlayIntro
mov bx, [bp+c0argv]
mov bx, [bx+2]
cmp [byte ptr bx+2], 'N'
jnz short @@PlayIntro
mov bx, [bp+c0argv]
mov bx, [bx+2]
cmp [byte ptr bx+3], 'N'
jnz short @@PlayIntro
mov bx, [bp+c0argv]
mov bx, [bx+2]
cmp [byte ptr bx+4], 'Y'
jnz short @@PlayIntro
jmp short @@PrepareMainMenu
; ---------------------------------------------------------------------------
@@PlayIntro:
call j_GAME_PlayIntro
@@PrepareMainMenu:
nop
push cs
call near ptr GAME_LoadMainImages
nop
push cs
call near ptr GAME_LoadTerrstat
nop
push cs
call near ptr GAME_LoadSpellData
call j_GAME_PrepareCredits
xor ax, ax
push ax
mov ax, e_MaxY_199
push ax
mov ax, e_MaxX_319
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
mov ax, e_MaxY_199
push ax
mov ax, e_MaxX_319
push ax
xor ax, ax
push ax
xor ax, ax
push ax
call VGA_DrawFilledRect
add sp, 0Ah
call VGA_SetDrawFrame
call SND_Stop_Music
mov ax, MUSIC_Main_Menu
push ax
mov ax, offset cnst_MUSIC_File
push ax
call LBXE_LoadSingle
pop cx
pop cx
mov [bp+MainMenu_Music_Seg], ax
cmp [Settings.BG_Music], 1
jnz short @@MainMenu
push [bp+MainMenu_Music_Seg]
call SND_PlayFile
pop cx
@@MainMenu:
mov ax, 0FFFFh
push ax
xor ax, ax
push ax
call VGA_LoadPalette
pop cx
pop cx
call VGA_DAC_Write
nop
push cs
call near ptr GAME_MainMenu
mov ax, 8
push ax
call j_GAME_Save
pop cx
call GAME_QuitWithReport
; ---------------------------------------------------------------------------
pop di
pop si
mov sp, bp
pop bp
retf
endp _main
; ---------------------------------------------------------------------------
align 2
word_135DA dw 0, 1, 10h, 12h
dw 18h, 19h, 20h, 21h
dw 22h, 30h, 31h, 38h
off_135F2 dw offset loc_1332D
dw offset loc_13343
dw offset loc_1334A
dw offset loc_1334C
dw offset loc_13353
dw offset loc_1336A
dw offset loc_1335A
dw offset loc_13371
dw offset loc_1337D
dw offset loc_13384
dw offset loc_13386
dw offset loc_1338D
word_1360A dw 0, 1, 10h, 12h
dw 18h, 19h, 20h, 21h
dw 22h, 30h, 31h, 38h
dw offset loc_13299
dw offset loc_132AA
dw offset loc_132AF
dw offset loc_132B4
dw offset loc_132B9
dw offset loc_132C3
dw offset loc_132BE
dw offset loc_132C8
dw offset loc_132CD
dw offset loc_132D2
dw offset loc_132D4
dw offset loc_132D9

;ends seg001

end
