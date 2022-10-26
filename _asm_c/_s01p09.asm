
proc UU_HLP_Load_MainMenu far

push	bp
mov	bp, sp

mov	ax, 0Ah
push	ax
mov	ax, 4
push	ax
xor	ax, ax
push	ax
mov	ax, offset HLP_Entry_Table
push	ax
mov	ax, 25
push	ax
mov	ax, offset cnst_HLPENTRY_File
push	ax
call	LBXR_DirectLoader
add	sp, 0Ch

mov	ax, 4
push	ax
mov	ax, offset HLP_Entry_Table
push	ax
call	GUI_SetHelp
pop	cx
pop	cx

pop	bp
retf

endp UU_HLP_Load_MainMenu
