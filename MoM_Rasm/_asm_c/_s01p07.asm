
proc GAME_LoadTerrstat far
push	bp
mov	bp, sp
mov	ax, 6
push	ax
mov	ax, 302h
push	ax
xor	ax, ax
push	ax
xor	ax, ax
push	ax
mov	ax, offset cnst_TERRSTAT_File
push	ax
call	LBXR_LoadSingle

add	sp, 0Ah
push	ax
call	MK_FAR

pop	cx
mov	[word ptr TBL_Moves_Per_Tile+2], dx
mov	[word ptr TBL_Moves_Per_Tile], ax
pop	bp
retf

endp GAME_LoadTerrstat
