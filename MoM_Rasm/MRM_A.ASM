
    Ideal

    p386n
    
    MODEL large

;       External References

EXTRN _fclose:FAR
EXTRN _fopen:FAR
EXTRN _fread:FAR
EXTRN _printf:FAR

;EXTRN _f040101_DosFindFile:FAR
;EXTRN _f050207_EXIT_CleanUp:FAR
;EXTRN _f060206_DOS_PrintAndExit:FAR

;       Miscellaneous Equates

ExeSignature    equ     05A4Dh

;       Data Segment(s)

;DATA segment word public 'DATA' ; make sure class='DATA' (default)
;DATA ends
;
;dseg segment word public 'DATA' ; make sure class='DATA' (default)
;    ; C strings must consist of bytes/chars and end with byte 0
;    format_specifier_signed_decimal_integer    DB "%d", 0
;dseg ends
;DSEG segment word public 'DATA' ; make sure class='DATA' (default)
;    ; C strings must consist of bytes/chars and end with byte 0
;    format_specifier_unsigned_decimal_integer    DB "%u", 0
;DSEG ends
;DATA segment word public 'DATA' ; make sure class='DATA' (default)
;    ;int     DW "%d" ; 'int' is a reserved word,
;    ; C strings must consist of bytes/chars and end with byte 0
;    format_specifier_string    DB "%s", 0
;DATA ends
segment deg001 word public 'DATA'
    format_specifier_signed_decimal_integer    DB "%d",0
    format_specifier_unsigned_decimal_integer    DB "%u",0
    format_specifier_string    DB "%s",0
    crlf DB "",13,10,0
    ;dseg:27FF
    strCONFIG_MOM db 'CONFIG.MOM',0
    ;dseg:280A
    strRunInstall db 'Run INSTALL to configure MASTER OF MAGIC.',0Ah,0Ah,0
    ;dseg:2836
    strRBr1 db 'rb',0
    ;dseg:87A4
    arrConfigMomSettings dw 10 dup(?)		       ; (sizeof=0x14)
    DearPrintfPleaseStopHere db 0
ends deg001

; Segment type:	Pure code
segment	seg001 byte public 'CODE' use16
    assume cs:seg001
    ;org 9
    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing,	gs:nothing
    assume es:nothing, ss:nothing, fs:nothing, gs:nothing
    assume ds:deg001
    
PUBLIC _f010109_main

; Attributes: bp-based frame

proc _f010109_main far

varFileName= byte ptr -5Ch
varDestination=	byte ptr -3Eh
var_16=	word ptr -16h
var_14=	word ptr -14h
var_12=	word ptr -12h
var_10=	word ptr -10h
var_E= word ptr	-0Eh
var_C= word ptr	-0Ch
var_A= word ptr	-0Ah
var_8= word ptr	-8
var_Itr_SaveGam= word ptr -6
varFilePtrStream= word ptr -4
var_2= word ptr	-2
argc= word ptr	6
argv= dword ptr	 8
envp= dword ptr	 0Ch

    push bp
    mov	bp, sp
    sub	sp, 5Ch
    
    push si
    push di
    
    mov	ax, seg strRBr1
    push ax
    mov	ax, offset strRBr1
    push ax
    push    seg format_specifier_string
    push    offset format_specifier_string
    call	far ptr _printf
    add sp,8                        ; discard the parameters
    push    seg crlf
    push    offset crlf
    push    seg format_specifier_string
    push    offset format_specifier_string
    call	far ptr _printf
    add sp,8                        ; discard the parameters
    
    mov	ax, seg strCONFIG_MOM
    push ax
    mov	ax, offset strCONFIG_MOM
    push ax
    push    seg format_specifier_string
    push    offset format_specifier_string
    call	far ptr _printf
    add sp,8                        ; discard the parameters
    push    seg crlf
    push    offset crlf
    push    seg format_specifier_string
    push    offset format_specifier_string
    call	far ptr _printf
    add sp,8                        ; discard the parameters
    
    ;mov	[ST_HeapBase], offset _heapbase@
    lea	ax, [bp+varFileName]
    push ax
    mov	ax, offset strCONFIG_MOM
    push ax
    call far ptr _f040101_DosFindFile
    
    pop	cx
    pop	cx
    
    or	ax, ax
    jnz	short OPEN_READ_ConfigMom

MISSING_CONFIG_MOM:
    mov	ax, offset strRunInstall
    push ax
    call far ptr _f050207_EXIT_CleanUp

loc_13233:
    pop	cx

OPEN_READ_ConfigMom:
    mov	ax, offset strRBr1
    push ax
    push    seg format_specifier_string
    push    offset format_specifier_string
    call	far ptr _printf
    add sp,6                        ; discard the parameters
    
    mov	ax, offset strCONFIG_MOM
    push ax
    push    seg format_specifier_string
    push    offset format_specifier_string
    call	far ptr _printf
    add sp,6                        ; discard the parameters
    
    call _fopen
    
    pop	cx
    pop	cx
    
    mov	[bp+varFilePtrStream], ax
    push [bp+varFilePtrStream]
    mov	ax, 1
    push ax
    mov	ax, 18
    push ax
    mov	ax, offset arrConfigMomSettings
    push ax
    call _fread
    add	sp, 8
    
    push [bp+varFilePtrStream]
    call _fclose
    
    pop	cx
    
    ;call j_ST_MagicSet_ReadCreateWrite
    
    ;mov	ax, [arrConfigMomSettings]
    mov ax, offset arrConfigMomSettings
    push    ax
    push    seg format_specifier_unsigned_decimal_integer
    push    offset format_specifier_unsigned_decimal_integer
    call	far ptr _printf
    add sp,6                        ; discard the parameters
    push    seg crlf
    push    offset crlf
    push    seg format_specifier_string
    push    offset format_specifier_string
    call	far ptr _printf
    add sp,8                        ; discard the parameters
    
    mov	ax, [arrConfigMomSettings]
    mov	[bp+var_8], ax
    mov	ax, [arrConfigMomSettings+4]
    mov	[bp+var_A], ax
    mov	di, 0FFFFh
    mov	ax, [arrConfigMomSettings+2]

;varCfgMom_02:
;    mov	[bp+var_14], ax
;    mov	cx, 12
;    mov	bx, offset idk_enum_w1360A
;
;loc_13286:
;    mov	ax, [cs:bx]
;    cmp	ax, [bp+var_14]
;    jz	short loc_13295
;
;loc_1328E:
;    add	bx, 2
;    loop loc_13286
;    jmp	short loc_132DE
;
;loc_13295:
;    jmp	[word ptr cs:bx+18h]
;
;loc_13299:
;    xor	si, si
;
;loc_1329B:
;    mov	[bp+var_8], 0FFFFh
;
;loc_132A0:
;    mov	[bp+var_A], 0FFFFh
;
;loc_132A5:
;    mov	di, 0FFFFh
;    jmp	short loc_132DE
;
;loc_132AA:
;    mov	si, 1
;    jmp	short loc_1329B
;
;loc_132AF:
;    mov	si, 2
;    jmp	short loc_1329B
;
;loc_132B4:
;    mov	si, 3
;    jmp	short loc_132A0
;
;loc_132B9:
;    mov	si, 4
;    jmp	short loc_132A0
;
;loc_132BE:
;    mov	si, 5
;    jmp	short loc_132A0
;
;loc_132C3:
;    mov	si, 6
;    jmp	short loc_1329B
;
;loc_132C8:
;    mov	si, 7
;    jmp	short loc_1329B
;
;loc_132CD:
;    mov	si, 8
;    jmp	short loc_1329B
;
;loc_132D2:
;    jmp	short $+2
;
;loc_132D4:
;    mov	si, 9
;    jmp	short loc_132A5
;
;loc_132D9:
;    mov	si, 10
;    jmp	short loc_132A5
;
;loc_132DE:
;    cmp	si, SNDRV_9_ROLAND
;    jl	short loc_132FC
;
;INIT_ROLAND_DRIVERS:
;    mov	ax, offset strInitRoland
;    push ax
;    call DOS_PrintString
;    pop	cx
;    call f020105_bios_timeofday
;    mov	ax, 2
;    push ax
;    call f020305_idk_SleepWaitPause
;    pop	cx
;
;loc_132FC:
;    mov	ax, [arrConfigMomSettings+6]
;    mov	[bp+var_E], ax
;    mov	ax, [arrConfigMomSettings+0Ah]
;    mov	[bp+var_10], ax
;    mov	ax, [arrConfigMomSettings+0Ch]
;    mov	[bp+var_12], ax
;    mov	ax, [arrConfigMomSettings+8]
;    mov	[bp+var_16], ax
;    mov	cx, 12
;    mov	bx, offset idk_enum_w135DA
;
;loc_1331A:
;    mov	ax, [cs:bx]
;    cmp	ax, [bp+var_16]
;    jz	short loc_13329
;    add	bx, 2
;    loop loc_1331A
;
;loc_13327:
;    jmp	short loc_13394
;
;loc_13329:
;    jmp	[word ptr cs:bx+18h]
;
;loc_1332D:
;    mov	[bp+var_C], 0
;
;loc_13332:
;    mov	[bp+var_E], 0FFFFh
;    mov	[bp+var_10], 0FFFFh
;    mov	[bp+var_12], 0FFFFh
;    jmp	short loc_13394
;
;loc_13343:
;    mov	[bp+var_C], 1
;    jmp	short loc_13332
;
;loc_1334A:
;    jmp	short loc_1332D
;
;loc_1334C:
;    mov	[bp+var_C], 3
;    jmp	short loc_13394
;
;loc_13353:
;    mov	[bp+var_C], 4
;    jmp	short loc_13394
;
;loc_1335A:
;    mov	[bp+var_C], 5
;    mov	ax, [bp+var_10]
;    mov	[bp+var_A], ax
;    mov	di, [bp+var_12]
;    jmp	short loc_13394
;
;loc_1336A:
;    mov	[bp+var_C], 6
;    jmp	short loc_13376
;
;loc_13371:
;    mov	[bp+var_C], 7
;
;loc_13376:
;    mov	[bp+var_E], 0FFFFh
;    jmp	short loc_13394
;
;loc_1337D:
;    mov	[bp+var_C], 8
;    jmp	short loc_13394
;
;loc_13384:
;    jmp	short $+2
;
;loc_13386:
;    mov	[bp+var_C], 9
;    jmp	short loc_13332
;
;loc_1338D:
;    mov	[bp+var_C], 0Ah
;    jmp	short loc_13332
;
;loc_13394:
;    cmp	si, 2
;    jnz	short loc_1339E
;    mov	[bp+var_C], 2
;
;loc_1339E:
;    cmp	[LBX_FONTS_w3A508], 2
;    jg	short loc_133AC
;    cmp	[LBX_FONTS_w3A508], 0
;    jge	short loc_133B2
;
;loc_133AC:
;    mov	[LBX_FONTS_w3A508], 0
;
;loc_133B2:
;    mov	[LBX_FONTS_w3A50A], 1
;    mov	[bp+var_Itr_SaveGam], 1
;    jmp	LOOP_x9_SaveGam
;
;loc_133C0:
;    mov	bx, [bp+var_Itr_SaveGam]
;    shl	bx, 1
;    cmp	[MagSet_22+bx],	0
;    jnz	short SAVEGAM
;    jmp	ITR_SaveGam
;
;SAVEGAM:
;    mov	ax, 10
;    push ax
;    lea	ax, [bp+varFileName]
;    push ax
;    push [bp+var_Itr_SaveGam]
;    call _itoa
;    add	sp, 6
;    mov	ax, offset strSAVE
;    push ax
;    lea	ax, [bp+varDestination]
;    push ax
;    call _strcpy
;    pop	cx
;    pop	cx
;    lea	ax, [bp+varFileName]
;    push ax
;    lea	ax, [bp+varDestination]
;    push ax
;    call _strcat
;    pop	cx
;    pop	cx
;    mov	ax, offset strGAM
;    push ax
;    lea	ax, [bp+varDestination]
;    push ax
;    call _strcat
;    pop	cx
;    pop	cx
;    lea	ax, [bp+varFileName]
;    push ax
;    lea	ax, [bp+varDestination]
;    push ax
;    call ST_DosFindFile
;    pop	cx
;    pop	cx
;    cmp	[bp+varFileName], 0
;    jnz	short ITR_SaveGam
;    mov	bx, [bp+var_Itr_SaveGam]
;    shl	bx, 1
;    mov	[MagSet_22+bx],	0
;    mov	ax, 8311
;    push ax
;    mov	ax, [bp+var_Itr_SaveGam]
;    mov	dx, 20
;    imul dx
;    add	ax, offset MagSet_23
;    push ax
;    call _strcpy
;    pop	cx
;    pop	cx
;    mov	ax, offset strWBr1
;    push ax
;    mov	ax, offset strMAGIC_SETr1
;    push ax
;    call _fopen
;    pop	cx
;    pop	cx
;    mov	[bp+varFilePtrStream], ax
;    push [bp+varFilePtrStream]
;    mov	ax, 1
;    push ax
;    mov	ax, 466
;    push ax
;    mov	ax, offset MagSet_01
;    push ax
;    call _fwrite
;    add	sp, 8
;    push [bp+varFilePtrStream]
;    call _fclose
;    pop	cx
;
;ITR_SaveGam:
;    inc	[bp+var_Itr_SaveGam]
;
;LOOP_x9_SaveGam:
;    cmp	[bp+var_Itr_SaveGam], 9
;    jge	short loc_13484
;    jmp	loc_133C0
;
;loc_13484:
;    mov	ax, 2700
;    push ax
;    call setRequiredEMS
;    pop	cx
;    mov	ax, 583
;    push ax
;    call ST_SetRequiredMEM
;    pop	cx
;    mov	[LBX_FONTS_w3A50A], 1
;    mov	[LBX_FONTS_w3A508], 2
;    push [bp+var_12]
;    push [bp+var_10]
;    push [bp+var_E]
;    push [bp+var_C]
;    push di
;    push [bp+var_A]
;    push [bp+var_8]
;    push si
;    mov	ax, offset strFONTSLBXr1
;    push ax
;    push [LBX_FONTS_w3A508]
;    push [LBX_FONTS_w3A50A]
;    call Load_Fonts_SndDrv
;    add	sp, 16h
;    call ST_SetDebugModeTrue
;    mov	ax, offset unk_334E4
;    push ax
;    call j_idk_Ems_TileFigureCont_s553B0
;    pop	cx
;    call SetFlag_1_CfgMom_w36A54
;    mov	ax, -1
;    push ax
;    xor	ax, ax
;    push ax
;    call ILSe_prepare_palette
;    pop	cx
;    pop	cx
;    call idk_SetPalette_s1C4F4
;    mov	bx, [word ptr bp+argv]
;    mov	bx, [bx+2]
;    cmp	[byte ptr bx], 74
;    jnz	short loc_13530
;
;loc_134FE:
;    mov	bx, [word ptr bp+argv]
;    mov	bx, [bx+2]
;    cmp	[byte ptr bx+1], 69
;    jnz	short loc_13530
;
;loc_1350A:
;    mov	bx, [word ptr bp+argv]
;    mov	bx, [bx+2]
;    cmp	[byte ptr bx+2], 78
;    jnz	short loc_13530
;    mov	bx, [word ptr bp+argv]
;    mov	bx, [bx+2]
;    cmp	[byte ptr bx+3], 'N'
;    jnz	short loc_13530
;    mov	bx, [word ptr bp+argv]
;    mov	bx, [bx+2]
;    cmp	[byte ptr bx+4], 'Y'
;    jnz	short loc_13530
;    jmp	short loc_13535
;
;loc_13530:
;    call j_ST_LoadMusicSoundfxIntro
;
;loc_13535:
;    nop
;    push cs
;    call near ptr ILSe_load_vortex_animation_images
;    nop
;    push cs
;    call near ptr fncLoadTERRSTAT
;    nop
;    push cs
;    call near ptr LBX_SpellDat
;    call sub_31CD5
;    xor	ax, ax
;    push ax
;    mov	ax, 199
;    push ax
;    mov	ax, 319
;    push ax
;    xor	ax, ax
;    push ax
;    xor	ax, ax
;    push ax
;    call idk_VGA_ScData_s186BC
;    add	sp, 10
;    call idk_VGA_s1E4BA
;    xor	ax, ax
;    push ax
;    mov	ax, 199
;    push ax
;    mov	ax, 319
;    push ax
;    xor	ax, ax
;    push ax
;    xor	ax, ax
;    push ax
;    call idk_VGA_ScData_s186BC
;    add	sp, 10
;    call VGA_MathThenSetVarToVgaLoc
;    call AIL_someSequence
;    mov	ax, 104
;    push ax
;    mov	ax, offset strMUSIC
;    push ax
;    call LBX_Load_Entry_0_0_0
;    pop	cx
;    pop	cx
;    mov	[bp+var_2], ax
;    cmp	[MagSet_02], 1
;    jnz	short KICK_OFF
;    push [bp+var_2]
;    call ST_LoadSound
;    pop	cx
;
;KICK_OFF:
;    mov	ax, -1
;    push ax
;    xor	ax, ax
;    push ax
;    call ILSe_prepare_palette
;    pop	cx
;    pop	cx
;    call idk_SetPalette_s1C4F4
;    nop
;    push cs
;    call near ptr ILSe_RUN
;    mov	ax, 8
;    push ax
;    call j_OVR_SaveGame_MagicSet
;    pop	cx
;    call ST_ExitWithMemDiag

    pop	di
    pop	si
    mov	sp, bp
    pop	bp
    retf

endp _f010109_main ; sp-analysis failed

;    align 2
;    
;idk_enum_w135DA	dw	0,     1,   10h,   12h
;    dw	  18h,	 19h,	20h,   21h
;    dw	  22h,	 30h,	31h,   38h
;    dw offset loc_1332D
;    dw offset loc_13343
;    dw offset loc_1334A
;    dw offset loc_1334C
;    dw offset loc_13353
;    dw offset loc_1336A
;    dw offset loc_1335A
;    dw offset loc_13371
;    dw offset loc_1337D
;    dw offset loc_13384
;    dw offset loc_13386
;    dw offset loc_1338D
;idk_enum_w1360A	dw	0,     1,   10h,   12h
;    dw	  18h,	 19h,	20h,   21h
;    dw	  22h,	 30h,	31h,   38h
;    dw offset loc_13299
;    dw offset loc_132AA
;    dw offset loc_132AF
;    dw offset loc_132B4
;    dw offset loc_132B9
;    dw offset loc_132C3
;    dw offset loc_132BE
;    dw offset loc_132C8
;    dw offset loc_132CD
;    dw offset loc_132D2
;    dw offset loc_132D4
;    dw offset loc_132D9
;

ends seg001





; Segment type:	Pure code
segment	seg004 byte public 'CODE' use16
    assume cs:seg004
    ;org 0Ch
    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing,	gs:nothing

;EXTRN _fclose:FAR
;EXTRN _fopen:FAR
;EXTRN _fread:FAR
;EXTRN _printf:FAR
;
;;EXTRN _f040101_DosFindFile:FAR
;EXTRN _f050207_EXIT_CleanUp:FAR
;EXTRN _f060206_DOS_PrintAndExit:FAR


struc BCpp30_DOSH_dosSearchInfo	; (sizeof=0x2B)
    ds_drive db ?
    ds_pattern db 13 dup(?)
    ds_reserved db 7 dup(?)
    ds_attrib db ?
    ds_time	dw ?
    ds_date	dw ?
    ds_size	dd ?
    ds_nameZ db 13 dup(?)
ends BCpp30_DOSH_dosSearchInfo


PUBLIC _f040101_DosFindFile

proc _f040101_DosFindFile far

varDosDtaPspAsciizFileNameAddr=	word ptr -4
varDTA_ES= word	ptr -2

argCharPtrFileName= dword ptr  6
argCharPtrDestinationBuffer= dword ptr	0Ah

    push bp
    mov	bp, sp
    sub	sp, 4
    push es
    push di
    push ds
    push si
    push ds
    
    ;call Disable_Set_FLAG_Mouse_Skip
    
    mov	ah, 2Fh
    int	21h
    ;add	bx, BCpp30_DOSH_dosSearchInfo.ds_nameZ
    ;add	bx, [BCpp30_DOSH_dosSearchInfo.ds_nameZ]
    add	bx, 1Eh
    mov	[bp+varDosDtaPspAsciizFileNameAddr], bx
    mov	bx, es
    mov	[bp+varDTA_ES],	bx
    mov	si, [word ptr bp+argCharPtrFileName]
    lodsb
    cmp	al, 0
    jz	short FIND_NEXT

FIND_FIRST:
    push ds
    mov	ah, 4Eh
    mov	cx, 0
    mov	dx, [word ptr bp+argCharPtrFileName]
    int	21h
    pop	ds
    jb	short FILE_NOT_FOUND

j_FILE_FOUND:
    jmp	short FILE_FOUND
    ;align 2

FIND_NEXT:
    mov	ah, 4Fh
    int	21h
    jb	short FILE_NOT_FOUND

FILE_FOUND:
    mov	ax, ds
    mov	es, ax
    ;assume es:dseg
    mov	di, [word ptr bp+argCharPtrFileName+2]
    mov	si, [bp+varDosDtaPspAsciizFileNameAddr]
    mov	ax, [bp+varDTA_ES]
    mov	ds, ax

ASCIIZ_COPY_SI_TO_DI:
    lodsb
    stosb
    cmp	al, 0
    jnz	short ASCIIZ_COPY_SI_TO_DI

RET_FILE_FOUND:
    mov	ax, -1
    pop	ds
    push ax
    ;call Restore_Set_FLAG_Mouse_Skip
    pop	ax
    pop	si
    pop	ds
    pop	di
    pop	es
    ;assume es:nothing
    mov	sp, bp
    pop	bp
    retf

FILE_NOT_FOUND:
    mov	al, 0
    mov	si, [word ptr bp+argCharPtrFileName+2]
    mov	[si], al
    mov	ax, 0
    pop	ds
    push ax
    ;call Restore_Set_FLAG_Mouse_Skip
    pop	ax
    pop	si
    pop	ds
    pop	di
    pop	es
    mov	sp, bp
    pop	bp
    retf
    
endp _f040101_DosFindFile

;byte_1420C db 6, 57h, 1Eh, 56h,	0BAh, 2	dup(0)
;    db 0B4h, 36h, 0CDh,	21h, 3Dh, 2 dup(0FFh)
;    db 74h, 7, 0F7h, 0E1h, 0F7h, 0E3h, 0EBh
;    db 7, 90h, 0BAh, 2 dup(0FFh), 0B8h,	2 dup(0FFh)
;    db 5Eh, 1Fh, 5Fh, 7, 0CBh

ends seg004






; Segment type:	Pure code
segment	seg005 byte public 'CODE' use16
    assume cs:seg005
    ;org 0Eh
    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing,	gs:nothing

;EXTRN _fclose:FAR
;EXTRN _fopen:FAR
;EXTRN _fread:FAR
;EXTRN _printf:FAR
;
;EXTRN _f040101_DosFindFile:FAR
;;EXTRN _f050207_EXIT_CleanUp:FAR
;EXTRN _f060206_DOS_PrintAndExit:FAR


PUBLIC _f050207_EXIT_CleanUp

proc _f050207_EXIT_CleanUp far

arg0_PrintString= word ptr  6

    push bp
    mov	bp, sp
    
    ;call AIL_someSequence
    ;call DOS_MouseReset
    ;call Call_AIL_shutdown
    ;call Chk_RelMemHandle
    ;call idk_PushPop_EsDsSiDi
    ;call ST_SetVideoMode_TEXT
    
    push [bp+arg0_PrintString]
    call far ptr _f060206_DOS_PrintAndExit
    
    pop	cx
    pop	bp
    retf

endp _f050207_EXIT_CleanUp

ends seg005






; Segment type:	Pure code
segment	seg006 byte public 'CODE' use16
    assume cs:seg006
    ;org 0Eh
    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing,	gs:nothing

;EXTRN _fclose:FAR
;EXTRN _fopen:FAR
;EXTRN _fread:FAR
;EXTRN _printf:FAR
;
;EXTRN _f040101_DosFindFile:FAR
;EXTRN _f050207_EXIT_CleanUp:FAR
;;EXTRN _f060206_DOS_PrintAndExit:FAR


PUBLIC _f060206_DOS_PrintAndExit

proc _f060206_DOS_PrintAndExit far

arg0_PrintString= dword	ptr  6

    push bp
    mov	bp, sp

loc_145E7:
    mov	si, [word ptr bp+arg0_PrintString]

LOOP_LoadSB:
    lodsb
    cmp	al, 0
    jnz	short LOOP_LoadSB
    dec	si
    mov	[byte ptr si], 0Dh
    inc	si
    mov	[byte ptr si], 0Ah
    inc	si
    mov	[byte ptr si], 0Dh
    inc	si
    mov	[byte ptr si], 0Ah
    inc	si
    mov	[byte ptr si], '$'
    inc	si
    mov	dx, [word ptr bp+arg0_PrintString]
    mov	ah, 9
    int	21h
    mov	ah, 4Ch
    int	21h

loc_1460F:
    pop	bp

locret_14610:
    retf

endp _f060206_DOS_PrintAndExit

ends seg006

end
