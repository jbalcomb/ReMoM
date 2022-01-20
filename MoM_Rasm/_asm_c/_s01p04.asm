
proc GAME_LoadMainImages far
push	bp
mov	bp, sp
push	si
mov	ax, offset cnst_MAINSCRN_File
push	ax
call	EMM_LoadResLBX

pop	cx
xor	ax, ax
push	ax
mov	ax, offset cnst_MAINSCRN_File
push	ax
call	LBXE_LoadSingle

pop	cx
pop	cx
mov	[IMG_MainScr_Title@], ax
mov	ax, 1
push	ax
mov	ax, offset cnst_VORTEX_File
push	ax
call	LBXE_LoadSingle

pop	cx
pop	cx
mov	[IMG_MainScr_Cont@], ax
mov	ax, 2
push	ax
mov	ax, offset cnst_VORTEX_File
push	ax
call	LBXE_LoadSingle

pop	cx
pop	cx
mov	[IMG_MainScr_HoF@], ax
mov	ax, 5
push	ax
mov	ax, offset cnst_VORTEX_File
push	ax
call	LBXE_LoadSingle

pop	cx
pop	cx
mov	[IMG_MainScr_LoadG@], ax
mov	ax, 4
push	ax
mov	ax, offset cnst_VORTEX_File
push	ax
call	LBXE_LoadSingle

pop	cx
pop	cx
mov	[IMG_MainScr_NewG@], ax
mov	ax, 3
push	ax
mov	ax, offset cnst_VORTEX_File
push	ax
call	LBXE_LoadSingle

pop	cx
pop	cx
mov	[IMG_MainScr_Quit@], ax
mov	ax, 5
push	ax
mov	ax, offset cnst_MAINSCRN_File
push	ax
call	LBXE_LoadSingle

pop	cx
pop	cx
mov	[IMG_MainScr_BG@], ax
mov	ax, offset cnst_WIZARDS_File
push	ax
call	EMM_LoadResLBX

pop	cx
xor	si, si
jmp	short loc_137D8

; ---------------------------------------------------------------------------

loc_137C3:
push	si
mov	ax, offset cnst_WIZARDS_File
push	ax
call	LBXE_LoadSingle

pop	cx
pop	cx
mov	bx, si
shl	bx, 1
mov	[IMG_Wizards@+bx], ax
inc	si


loc_137D8:
cmp	si, 14
jl	short loc_137C3

mov	ax, offset cnst_SPELLDAT_File
push	ax
call	EMM_LoadResLBX

pop	cx
pop	si
pop	bp
retf

endp GAME_LoadMainImages
