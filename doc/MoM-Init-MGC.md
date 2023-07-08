


SEEALSO: C:\STU-DASM\drake178\__MGC_main.asm



MAGIC.EXE
start
_main





proc start far

mov     dx, seg dseg
mov     [cs:DGROUP@], dx                    ; declared in c0.asm
...
...
...
push    [__C0environ]                       ; source: borlandc\lib\startup\c0.asm
push    [__C0argv]                          ; c0argv
push    [__C0argc]                          ; c0argc
call    _main

endp start



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
c0argc= word ptr  6
c0argv= word ptr  8




mov     [g_EMM_Pages_Reserved], 158       ; 158 ? EMS Logical Pages ? !?!2700/16=168?!?


lea     ax, [bp+Temp_String]
push    ax                                ; Read_Buffer
mov     ax, offset CONFIG_MOM             ; "CONFIG.MOM"
push    ax                                ; match_string
call    DIR                               ; executes a FINDFIRST interrupt if a file name is

