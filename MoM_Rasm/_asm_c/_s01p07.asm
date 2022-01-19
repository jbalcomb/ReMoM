TITLE _s01p07.asm GAME_LoadTerrstat

.MODEL LARGE, C

.CODE
; Segment type: Pure code
;segment seg001 byte public 'CODE' use16
;    assume cs:seg001
;    org 9
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GAME_LoadTerrstat

proc GAME_LoadTerrstat far
push bp
mov bp, sp
mov ax, 6
push ax
mov ax, 302h
push ax
xor ax, ax
push ax
xor ax, ax
push ax
mov ax, offset cnst_TERRSTAT_File
push ax
call LBXR_LoadSingle
add sp, 0Ah
push ax
call MK_FAR
pop cx
mov [word ptr TBL_Moves_Per_Tile+2], dx
mov [word ptr TBL_Moves_Per_Tile], ax
pop bp
retf
endp GAME_LoadTerrstat

;ends seg001

end
