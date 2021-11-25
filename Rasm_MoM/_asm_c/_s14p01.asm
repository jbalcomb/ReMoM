TITLE   _s14p01.ASM Hardware_Init

.MODEL LARGE, C


EXTRN VGA_SetModeY:PROC ; _s15p01.asm
EXTRN VGA_DAC_Init:PROC ; _s14p03.asm
EXTRN EMS_Startup:PROC ; _s13p01.asm


segment	seg014 byte public 'CODE' use16
    assume cs:seg014
    ;org 0Dh
    ;assume es:nothing, ss:nothing, ds:dseg,	fs:nothing, gs:nothing

PUBLIC Hardware_Init

proc Hardware_Init far

    Input_Type= word ptr  6
    Snd_Chnls= word	ptr  8
    FontFile= word	ptr  0Ah
    M_Drv= word ptr	 0Ch
    M_IO= word ptr	0Eh
    M_IRQ= word ptr	 10h
    M_DMA= word ptr	 12h
    D_Drv= word ptr	 14h
    D_IO= word ptr	16h
    D_IRQ= word ptr	 18h
    D_DMA= word ptr	 1Ah

    push  bp
    mov   bp, sp
    push si
    push ds

    ;mov	si, [bp+Input_Type]

;    call	CRP_Empty_Exit_Fn2

    call  EMS_Startup

    call  VGA_SetModeY

;#if 0
;    cmp	si, 0FFFFh
;    jnz	short loc_17DCA
;#endif

;#if 0
;    mov	ax, offset cnst_FONT_File2 ; dseg:41BA
;    push	ax
;    nop
;    push	cs
;    call	near ptr VGA_DAC_Init
;    pop	cx
;
;    mov	ax, 0FFFFh
;    push	ax
;    mov	ax, 0FFFFh
;    push	ax
;    mov	ax, 0FFFFh
;    push	ax
;    mov	ax, 0FFFFh
;    push	ax
;    mov	ax, 0FFFFh
;    push	ax
;    mov	ax, 0FFFFh
;    push	ax
;    mov	ax, 0FFFFh
;    push	ax
;    mov	ax, 0FFFFh
;    push	ax
;    mov	ax, 0FFFFh
;    push	ax
;    xor	ax, ax
;    push	ax
;    xor	ax, ax
;    push	ax
;    call	SND_Init
;    add	sp, 16h
;
;    mov	ax, 1
;    push	ax
;
;    jmp	short loc_17DF7
;#endif

loc_17DCA:

    push	[bp+FontFile]
    ;nop
    ;push	cs
    call VGA_DAC_Init
    add sp, 02h                         ;pop cx

;#if 0
;    push	[bp+D_DMA]
;    push	[bp+D_IRQ]
;    push	[bp+D_IO]
;    push	[bp+D_Drv]
;    push	[bp+M_DMA]
;    push	[bp+M_IRQ]
;    push	[bp+M_IO]
;    push	[bp+Snd_Chnls]
;    push	[bp+M_Drv]
;    call	SND_Init
;    add	sp, 12h
;#endif

    ;push	si

loc_17DF7:

    ;call	GUI_Init
    ;add sp, 02h                         ;pop cx

    ;call	RNG_TimerSeed

    ;call	VGA_SetDrawFrame

    pop  ds
    pop	si
    pop	bp
    
    ret

endp Hardware_Init

ends seg014

end
