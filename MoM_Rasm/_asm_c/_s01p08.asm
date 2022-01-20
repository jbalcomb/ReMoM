
proc GAME_LoadSpellData	far
push	bp
mov	bp, sp
mov	ax, 24h
push	ax
mov	ax, 0D7h
push	ax
xor	ax, ax
push	ax
xor	ax, ax
push	ax
mov	ax, offset cnst_SPELLDAT_File
push	ax
call	LBXR_LoadSingle

add	sp, 0Ah
push	ax
call	MK_FAR

pop	cx
mov	[word ptr TBL_Spell_Data@+2], dx
mov	[word ptr TBL_Spell_Data@], ax
pop	bp
retf

endp GAME_LoadSpellData
