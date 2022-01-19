TITLE _s01p08.asm GAME_LoadSpellData

.MODEL LARGE, C

.CODE
; Segment type: Pure code
;segment seg001 byte public 'CODE' use16
;    assume cs:seg001
;    org 9
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GAME_LoadSpellData

proc GAME_LoadSpellData far
push bp
mov bp, sp
mov ax, 24h
push ax
mov ax, 0D7h
push ax
xor ax, ax
push ax
xor ax, ax
push ax
mov ax, offset cnst_SPELLDAT_File
push ax
call LBXR_LoadSingle
add sp, 0Ah
push ax
call MK_FAR
pop cx
mov [word ptr TBL_Spell_Data@+2], dx
mov [word ptr TBL_Spell_Data@], ax
pop bp
retf
endp GAME_LoadSpellData

;ends seg001

end
