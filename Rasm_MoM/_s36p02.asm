TITLE _s36p02.asm SND_Init

.MODEL LARGE, C

.CODE
;segment    SOUND byte public 'CODE' use16
;    assume cs:SOUND
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC SND_Init

proc SND_Init far

    Use_Digi_DMA        = word ptr -32h
    Use_Digi_IRQ        = word ptr -30h
    Use_Digi_IO         = word ptr -2Eh
    Prev_Digi_IRQ       = word ptr -2Ch
    Prev_Digi_DMA       = word ptr -2Ah
    Prev_Digi_IO        = word ptr -28h
    Prev_MIDI_DMA       = word ptr -26h
    Prev_MIDI_IRQ       = word ptr -24h
    Prev_MIDI_IO        = word ptr -22h
    Use_MIDI_DMA        = word ptr -20h
    Use_MIDI_IRQ        = word ptr -1Eh
    Use_MIDI_IO         = word ptr -1Ch
    State_Table_Size    = dword ptr -1Ah
    Timbre_Cache        = dword ptr -16h
    Have_Digi_Settings  = word ptr -12h
    Sound_Data_Space    = word ptr -10h
    State_Tbl_Seg       = word ptr -0Eh
    Timbre_Cache_Seg    = word ptr -0Ah
    Have_MIDI_Settings  = word ptr -8
    GMIDI_Timbre_Init   = word ptr -6
    Roland_Timbre_Init  = word ptr -4
    SB_Type             = word ptr -2
    MIDI_Drv            = word ptr  6
    Sound_Channels      = word ptr  8
    MIDI_IO             = word ptr  0Ah
    MIDI_IRQ            = word ptr  0Ch
    MIDI_DMA            = word ptr  0Eh
    Digi_Drv            = word ptr  10h
    Digi_IO             = word ptr  12h
    Digi_IRQ            = word ptr  14h
    Digi_DMA            = word ptr  16h

    push bp
    mov  bp, sp
    sub  sp, 32h
    push si
    push di

    mov si, [bp+MIDI_Drv]
    mov [bp+Have_MIDI_Settings], 0
    mov [bp+Have_Digi_Settings], 0
    mov [SND_MIDI_Drv_Index], si
    mov ax, [bp+Digi_Drv]
    mov [SND_Digi_Drv_Index], ax
    cmp [SND_AIL_running], 0
    jz  short loc_2FD41

loc_2FD3B:
    mov    ax, 1
    jmp    loc_303AF
loc_2FD41:
    db 83h,7Eh,0Ah,0FFh ; <BAD>cmp       [bp+MIDI_IO], -1
    jnz    short loc_2FD53
    db 83h,7Eh,0Ch,0FFh ; <BAD>cmp       [bp+MIDI_IRQ], -1
    jnz    short loc_2FD53
    db 83h,7Eh,0Eh,0FFh ; <BAD>cmp       [bp+MIDI_DMA], -1
    jz    short loc_2FD6A
loc_2FD53:
    mov    [bp+Have_MIDI_Settings], 1
    mov    ax, [bp+MIDI_IO]
    mov    [bp+Use_MIDI_IO], ax
    mov    ax, [bp+MIDI_IRQ]
    mov    [bp+Use_MIDI_IRQ], ax
    mov    ax, [bp+MIDI_DMA]
    mov    [bp+Use_MIDI_DMA], ax
loc_2FD6A:
    db 83h,7Eh,12h,0FFh ; <BAD>cmp       [bp+Digi_IO], -1
    jnz    short loc_2FD7C
    db 83h,7Eh,14h,0FFh ; <BAD>cmp       [bp+Digi_IRQ], -1
    jnz    short loc_2FD7C
    db 83h,7Eh,16h,0FFh ; <BAD>cmp       [bp+Digi_DMA], -1
    jz    short loc_2FD93
loc_2FD7C:
    mov    [bp+Have_Digi_Settings], 1
    mov    ax, [bp+Digi_IO]
    mov    [bp+Use_Digi_IO], ax
    mov    ax, [bp+Digi_IRQ]
    mov    [bp+Use_Digi_IRQ], ax
    mov    ax, [bp+Digi_DMA]
    mov    [bp+Use_Digi_DMA], ax
loc_2FD93:
    or    si, si
    jnz    short loc_2FDA7
    cmp    [bp+Digi_Drv], SND_None
    jnz    short loc_2FDA7
loc_2FD9D:
    mov    [SND_Driver_Count], 0
    jmp    short loc_2FD3B
    jmp    short loc_2FDEF
loc_2FDA7:
    cmp    si, SND_Speaker
    jnz    short loc_2FDB4
    mov    [SND_Driver_Count], 1
    jmp    short loc_2FDEF
loc_2FDB4:
    cmp    [bp+Sound_Channels], 1
    jnz    short loc_2FDC4
    cmp    [bp+Digi_Drv], 0
    jnz    short loc_2FDC4
    jmp    short loc_2FD9D
    jmp    short loc_2FDCA
loc_2FDC4:
    mov    [SND_Driver_Count], 1
loc_2FDCA:
    cmp    [bp+Sound_Channels], 2
    jnz    short loc_2FDDF
    cmp    [SND_MIDI_Drv_Index], SND_Speaker
    jle    short loc_2FDDF
    mov    [SND_Driver_Count], 2
    jmp    short loc_2FDEF
loc_2FDDF:
    cmp    [bp+Digi_Drv], SND_None
    jnz    short loc_2FDE9
    jmp    short loc_2FD9D
    jmp    short loc_2FDEF
loc_2FDE9:
    mov    [SND_Driver_Count], 1
loc_2FDEF:
    cmp    [SND_MIDI_Drv_Index], SND_SBPro
    jz    short loc_2FDFD
    cmp    [SND_MIDI_Drv_Index], SND_SBPro2
    jnz    short loc_2FE47
loc_2FDFD:
    nop
    push    cs
    call    near ptr _AIL_board_ID
    mov    [bp+SB_Type], ax
    cmp    [bp+SB_Type], 0
    jnz    short loc_2FE14
    xor    ax, ax
    push    ax
    nop
    push    cs
    call    near ptr SND_Error
    pop    cx
loc_2FE14:
    mov    bx, [bp+SB_Type]
    dec    bx
    cmp    bx, 3
    ja    short loc_2FE47
    shl    bx, 1
    jmp    [cs:off_303B5+bx]
loc_2FE24:
    mov    [SND_MIDI_Drv_Index], SND_SB
    mov    si, SND_SB
    jmp    short loc_2FE47
loc_2FE2F:
    jmp    short loc_2FE24
loc_2FE31:
    mov    [SND_MIDI_Drv_Index], SND_SBPro
    mov    si, SND_SBPro
    jmp    short loc_2FE47
loc_2FE3C:
    mov    [SND_MIDI_Drv_Index], SND_SBPro2
    mov    si, SND_SBPro2
    jmp    short $+2
loc_2FE47:
    cmp    [SND_MIDI_Drv_Index], SND_Speaker
    jle    short loc_2FE6D
    mov    ax, si
    dec    ax
    shl    ax, 1
    push    ax
    mov    ax, offset cnst_SNDDRV_FileName
    push    ax
    call    LBXE_LoadSingle
    pop    cx
    pop    cx
    push    ax
    call    MK_FAR
    pop    cx
    mov    [word ptr SND_AIL_MIDI_Driver@+2], dx
    mov    [word ptr SND_AIL_MIDI_Driver@], ax
loc_2FE6D:
    cmp    [SND_Digi_Drv_Index], SND_None
    jle    short loc_2FE95
    mov    ax, [SND_Digi_Drv_Index]
    dec    ax
    shl    ax, 1
    inc    ax
    push    ax
    mov    ax, offset cnst_SNDDRV_FileName
    push    ax
    call    LBXE_LoadSingle
    pop    cx
    pop    cx
    push    ax
    call    MK_FAR
    pop    cx
    mov    [word ptr SND_AIL_Digi_Driver@+2], dx
    mov    [word ptr SND_AIL_Digi_Driver@], ax
loc_2FE95:
    call    _AIL_startup
    cmp    [SND_MIDI_Drv_Index], SND_Speaker
    jle    short loc_2FEB3
    push    [word ptr SND_AIL_MIDI_Driver@+2]
    push    [word ptr SND_AIL_MIDI_Driver@]
    call    _AIL_register_driver
    pop    cx
    pop    cx
    mov    [SND_MIDI_Drv_handle], ax
loc_2FEB3:
    cmp    [SND_Digi_Drv_Index], SND_None
    jle    short loc_2FECC
    push    [word ptr SND_AIL_Digi_Driver@+2]
    push    [word ptr SND_AIL_Digi_Driver@]
    call    _AIL_register_driver
    pop    cx
    pop    cx
    mov    [SND_Digi_Drv_Handle], ax
loc_2FECC:
    db 83h,3Eh,80h,4Dh,0FFh    ; <BAD>cmp     [SND_MIDI_Drv_handle], -1
    jnz    short loc_2FEF7
    cmp    [SND_MIDI_Drv_Index], SND_Speaker
    jle    short loc_2FEF7
    xor    ax, ax
    xor    dx, dx
    push    ax
    push    dx
    call    _AIL_shutdown
    pop    cx
    pop    cx
    mov    [SND_Driver_Count], 0
    mov    ax, SND_driver_error
    push    ax
    nop
    push    cs
    call    near ptr SND_Error
    pop    cx
loc_2FEF7:
    db 83h,3Eh,82h,4Dh,0FFh    ; <BAD>cmp     [SND_Digi_Drv_Handle], -1
    jnz    short loc_2FF22
    cmp    [SND_Digi_Drv_Index], SND_None
    jle    short loc_2FF22
    xor    ax, ax
    xor    dx, dx
    push    ax
    push    dx
    call    _AIL_shutdown
    pop    cx
    pop    cx
    mov    [SND_Driver_Count], 0
    mov    ax, SND_invalid_Digi_driver
    push    ax
    nop
    push    cs
    call    near ptr SND_Error
    pop    cx
loc_2FF22:
    cmp    [SND_MIDI_Drv_Index], SND_Speaker
    jle    short loc_2FF3A
    push    [SND_MIDI_Drv_handle]
    call    _AIL_describe_driver
    pop    cx
    mov    [word ptr SND_MIDI_drvr_desc+2], dx
    mov    [word ptr SND_MIDI_drvr_desc], ax
loc_2FF3A:
    cmp    [SND_Digi_Drv_Index], 0
    jle    short loc_2FF52
    push    [SND_Digi_Drv_Handle]
    call    _AIL_describe_driver
    pop    cx
    mov    [word ptr SND_Digi_drvr_desc+2], dx
    mov    [word ptr SND_Digi_drvr_desc], ax
loc_2FF52:
    les    bx, [SND_MIDI_drvr_desc]
    cmp    [es:bx+AIL_drvr_desc.drvr_type], 3
    jz    short loc_2FF81
    cmp    [SND_MIDI_Drv_Index], SND_Speaker
    jle    short loc_2FF81
    xor    ax, ax
    xor    dx, dx
    push    ax
    push    dx
    call    _AIL_shutdown
    pop    cx
    pop    cx
    mov    [SND_Driver_Count], 0
    mov    ax, SND_Invalid_XMIDI_driver
    push    ax
    nop
    push    cs
    call    near ptr SND_Error
    pop    cx
loc_2FF81:
    les    bx, [SND_Digi_drvr_desc]
    cmp    [es:bx+AIL_drvr_desc.drvr_type], 2
    jz    short loc_2FFB0
    cmp    [SND_Digi_Drv_Index], SND_None
    jle    short loc_2FFB0
    xor    ax, ax
    xor    dx, dx
    push    ax
    push    dx
    call    _AIL_shutdown
    pop    cx
    pop    cx
    mov    [SND_Driver_Count], 0
    mov    ax, SND_not_DSP_driver
    push    ax
    nop
    push    cs
    call    near ptr SND_Error
    pop    cx
loc_2FFB0:
    cmp    [bp+Have_MIDI_Settings], 0
    jz    short loc_3000A
    db 83h,7Eh,0Eh,0FFh ; <BAD>cmp       [bp+MIDI_DMA], -1
    jz    short loc_2FFD2
    les    bx, [SND_MIDI_drvr_desc]
    mov    ax, [es:bx+AIL_drvr_desc.default_DMA]
    mov    [bp+Prev_MIDI_DMA], ax
    les    bx, [SND_MIDI_drvr_desc]
    mov    ax, [bp+Use_MIDI_DMA]
    mov    [es:bx+AIL_drvr_desc.default_DMA], ax
loc_2FFD2:
    db 83h,7Eh,0Ah,0FFh ; <BAD>cmp       [bp+MIDI_IO], -1
    jz    short loc_2FFEE
    les    bx, [SND_MIDI_drvr_desc]
    mov    ax, [es:bx+AIL_drvr_desc.default_IO]
    mov    [bp+Prev_MIDI_IO], ax
    les    bx, [SND_MIDI_drvr_desc]
    mov    ax, [bp+Use_MIDI_IO]
    mov    [es:bx+AIL_drvr_desc.default_IO], ax
loc_2FFEE:
    db 83h,7Eh,0Ch,0FFh ; <BAD>cmp       [bp+MIDI_IRQ], -1
    jz    short loc_3000A
    les    bx, [SND_MIDI_drvr_desc]
    mov    ax, [es:bx+AIL_drvr_desc.default_IRQ]
    mov    [bp+Prev_MIDI_IRQ], ax
    les    bx, [SND_MIDI_drvr_desc]
    mov    ax, [bp+Use_MIDI_IRQ]
    mov    [es:bx+AIL_drvr_desc.default_IRQ], ax
loc_3000A:
    cmp    [bp+Have_Digi_Settings], 0
    jz    short loc_30064
    db 83h,7Eh,16h,0FFh ; <BAD>cmp       [bp+Digi_DMA], -1
    jz    short loc_3002C
    les    bx, [SND_Digi_drvr_desc]
    mov    ax, [es:bx+AIL_drvr_desc.default_DMA]
    mov    [bp+Prev_Digi_DMA], ax
    les    bx, [SND_Digi_drvr_desc]
    mov    ax, [bp+Use_Digi_DMA]
    mov    [es:bx+AIL_drvr_desc.default_DMA], ax
loc_3002C:
    db 83h,7Eh,12h,0FFh ; <BAD>cmp       [bp+Digi_IO], -1
    jz    short loc_30048
    les    bx, [SND_Digi_drvr_desc]
    mov    ax, [es:bx+AIL_drvr_desc.default_IO]
    mov    [bp+Prev_Digi_IO], ax
    les    bx, [SND_Digi_drvr_desc]
    mov    ax, [bp+Use_Digi_IO]
    mov    [es:bx+AIL_drvr_desc.default_IO], ax
loc_30048:
    db 83h,7Eh,14h,0FFh ; <BAD>cmp       [bp+Digi_IRQ], -1
    jz    short loc_30064
    les    bx, [SND_Digi_drvr_desc]
    mov    ax, [es:bx+AIL_drvr_desc.default_IRQ]
    mov    [bp+Prev_Digi_IRQ], ax
    les    bx, [SND_Digi_drvr_desc]
    mov    ax, [bp+Use_Digi_IRQ]
    mov    [es:bx+AIL_drvr_desc.default_IRQ], ax
loc_30064:
    cmp    [SND_MIDI_Drv_Index], SND_Speaker
    jg    short loc_3006E
    jmp    loc_30134
loc_3006E:
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DRQ]
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DMA]
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IRQ]
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IO]
    push    [SND_MIDI_Drv_handle]
    call    _AIL_detect_device
    add    sp, 0Ah
    or    ax, ax
    jz    short loc_300A1
    jmp    loc_30134
loc_300A1:
    cmp    [bp+Have_MIDI_Settings], 0
    jz    short loc_30117
    les    bx, [SND_MIDI_drvr_desc]
    mov    ax, [bp+Prev_MIDI_DMA]
    mov    [es:bx+AIL_drvr_desc.default_DMA], ax
    les    bx, [SND_MIDI_drvr_desc]
    mov    ax, [bp+Prev_MIDI_IO]
loc_300B9:
    mov    [es:bx+AIL_drvr_desc.default_IO], ax
    les    bx, [SND_MIDI_drvr_desc]
    mov    ax, [bp+Prev_MIDI_IRQ]
    mov    [es:bx+AIL_drvr_desc.default_IRQ], ax
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DRQ]
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DMA]
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IRQ]
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IO]
    push    [SND_MIDI_Drv_handle]
    call    _AIL_detect_device
    add    sp, 0Ah
    or    ax, ax
    jnz    short loc_30115
    xor    ax, ax
    xor    dx, dx
    push    ax
    push    dx
    call    _AIL_shutdown
    pop    cx
    pop    cx
    mov    [SND_Driver_Count], 0
    mov    ax, SND_Invalid_Music_IO
    push    ax
    nop
    push    cs
    call    near ptr SND_Error
    pop    cx
loc_30115:
    jmp    short loc_30134
loc_30117:
    xor    ax, ax
    xor    dx, dx
    push    ax
    push    dx
    call    _AIL_shutdown
    pop    cx
    pop    cx
    mov    [SND_Driver_Count], 0
    mov    ax, SND_Bad_Music_IO
    push    ax
    nop
    push    cs
    call    near ptr SND_Error
    pop    cx
loc_30134:
    cmp    [SND_Digi_Drv_Index], SND_None
    jg    short loc_3013E
    jmp    loc_30204
loc_3013E:
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DRQ]
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DMA]
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IRQ]
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IO]
    push    [SND_Digi_Drv_Handle]
    call    _AIL_detect_device
    add    sp, 0Ah
    or    ax, ax
    jz    short loc_30171
    jmp    loc_30204
loc_30171:
    cmp    [bp+Have_Digi_Settings], 0
    jz    short loc_301E7
    les    bx, [SND_Digi_drvr_desc]
    mov    ax, [bp+Prev_Digi_DMA]
    mov    [es:bx+AIL_drvr_desc.default_DMA], ax
    les    bx, [SND_Digi_drvr_desc]
    mov    ax, [bp+Prev_Digi_IO]
    mov    [es:bx+AIL_drvr_desc.default_IO], ax
    les    bx, [SND_Digi_drvr_desc]
    mov    ax, [bp+Prev_Digi_IRQ]
    mov    [es:bx+AIL_drvr_desc.default_IRQ], ax
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DRQ]
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DMA]
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IRQ]
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IO]
    push    [SND_Digi_Drv_Handle]
    call    _AIL_detect_device
    add    sp, 0Ah
    or    ax, ax
    jnz    short loc_301E5
    xor    ax, ax
    xor    dx, dx
    push    ax
    push    dx
    call    _AIL_shutdown
    pop    cx
    pop    cx
    mov    [SND_Driver_Count], 0
    mov    ax, SND_Invalid_Digi_IO
    push    ax
    nop
    push    cs
    call    near ptr SND_Error
    pop    cx
loc_301E5:
    jmp    short loc_30204
loc_301E7:
    xor    ax, ax
    xor    dx, dx
    push    ax
    push    dx
    call    _AIL_shutdown
    pop    cx
    pop    cx
    mov    [SND_Driver_Count], 0
    mov    ax, SND_Bad_Digi_IO
    push    ax
    nop
    push    cs
    call    near ptr SND_Error
    pop    cx
loc_30204:
    cmp    [SND_Digi_Drv_Index], SND_None
    jle    short loc_30237
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DRQ]
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DMA]
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IRQ]
    les    bx, [SND_Digi_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IO]
    push    [SND_Digi_Drv_Handle]
    call    _AIL_init_driver
    add    sp, 0Ah
loc_30237:
    cmp    [SND_MIDI_Drv_Index], SND_Speaker
    jle    short loc_3026A
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DRQ]
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_DMA]
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IRQ]
    les    bx, [SND_MIDI_drvr_desc]
    push    [es:bx+AIL_drvr_desc.default_IO]
    push    [SND_MIDI_Drv_handle]
    call    _AIL_init_driver
    add    sp, 0Ah
loc_3026A:
    push    [SND_MIDI_Drv_handle]
    call    _AIL_state_tbl_size
    pop    cx
    mov    [word ptr bp+State_Table_Size+2], 0
    mov    [word ptr bp+State_Table_Size],    ax
    mov    dx, [word ptr bp+State_Table_Size+2]
    mov    ax, [word ptr bp+State_Table_Size]
    mov    cl, 4
    call    LXURSH@
    inc    ax
    push    ax
    call    LBX_Alloc_Space
    pop    cx
    mov    [bp+State_Tbl_Seg], ax
    push    [bp+State_Tbl_Seg]
    call    MK_FAR
    pop    cx
    mov    [word ptr SND_AIL_State_Table+2], dx
    mov    [word ptr SND_AIL_State_Table],    ax
    mov    ax, offset cnst_tbank_prefix
    push    ax
    mov    ax, offset SND_Timbre_Bank_File
    push    ax
    call    strcpy
    pop    cx
    pop    cx
    mov    ax, [word ptr SND_MIDI_drvr_desc]
    add    ax, 4
    push    [word ptr SND_MIDI_drvr_desc+2]
    push    ax
    xor    ax, ax
    push    ax
    mov    ax, offset SND_tbank_suffix
    push    ax
    call    STR_Copy_Far
    add    sp, 8
    mov    ax, offset SND_tbank_suffix
    push    ax
    mov    ax, offset SND_Timbre_Bank_File
    push    ax
    call    strcat
    pop    cx
    pop    cx
    push    [SND_MIDI_Drv_handle]
    call    _AIL_t_cache_size
    pop    cx
    mov    di, ax
    cmp    di, 2000
    jbe    short loc_302F1
    mov    di, 2000
loc_302F1:
    or    di, di
    jz    short loc_3032D
    mov    ax, di
    xor    dx, dx
    mov    cl, 4
    call    LXURSH@
    inc    ax
    push    ax
    call    LBX_Alloc_Space
    pop    cx
    mov    [bp+Timbre_Cache_Seg], ax
    push    [bp+Timbre_Cache_Seg]
    call    MK_FAR
    pop    cx
    mov    [word ptr bp+Timbre_Cache+2], dx
    mov    [word ptr bp+Timbre_Cache], ax
    push    di
    push    [word ptr bp+Timbre_Cache+2]
    push    [word ptr bp+Timbre_Cache]
    push    [SND_MIDI_Drv_handle]
    call    _AIL_define_t_cache
    add    sp, 8
loc_3032D:
    mov    [SND_AIL_running], 1
    mov    ax, 14h
    push    ax
    call    LBX_Alloc_Space
    pop    cx
    mov    [bp+Sound_Data_Space], ax
    push    [bp+Sound_Data_Space]
    call    MK_FAR
    pop    cx
    mov    [word ptr SND_TLoad_Buffer+2], dx
    mov    [word ptr SND_TLoad_Buffer], ax
    cmp    [SND_MIDI_Drv_Index], SND_Roland
    jnz    short loc_3037E
    mov    ax, 14h
    push    ax
    mov    ax, offset cnst_SNDDRV_FileName
    push    ax
    call    LBXE_LoadSingle
    pop    cx
    pop    cx
    mov    [bp+Roland_Timbre_Init], ax
    push    [bp+Roland_Timbre_Init]
    nop
    push    cs
    call    near ptr SND_PlayFile
    pop    cx
    jmp    short $+2
loc_30374:
    nop
    push    cs
    call    near ptr SND_Get_Seq_Status
    cmp    ax, 2
    jnz    short loc_30374
loc_3037E:
    cmp    [SND_MIDI_Drv_Index], 0Ah
    jnz    short loc_303AC
    mov    ax, 15h
    push    ax
    mov    ax, offset cnst_SNDDRV_FileName
    push    ax
    call    LBXE_LoadSingle
    pop    cx
    pop    cx
    mov    [bp+GMIDI_Timbre_Init],    ax
    push    [bp+GMIDI_Timbre_Init]
    nop
    push    cs
    call    near ptr SND_PlayFile
    pop    cx
    jmp    short $+2
loc_303A2:
    nop
    push    cs
    call    near ptr SND_Get_Seq_Status
    cmp    ax, 2
    jnz    short loc_303A2
loc_303AC:
    jmp    loc_2FD3B
loc_303AF:
    pop    di
    pop    si
    mov    sp, bp
    pop    bp
    retf

endp SND_Init
; ---------------------------------------------------------------------------
off_303B5 dw offset loc_2FE24
dw offset loc_2FE31
dw offset loc_2FE2F
dw offset loc_2FE3C

;ends SOUND

end
