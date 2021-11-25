TITLE _s13p01.asm EMM_Startup

.MODEL LARGE

EMM_Record STRUC
HandleName db 9	dup(?)
Reserved db ?
Handle dw ?
EMM_Record ENDS

EXTRN C EMS_Reserved_Left:WORD
EXTRN C EmmHndlNbr_Yomoma:WORD
EXTRN C EMS_Open_Handles:WORD
EXTRN C EmmHndlNm_Yomoma:BYTE
EXTRN C EmmHndlNm_Yomoma2:BYTE
EXTRN C cnst_EMM_VGAH_Name:BYTE
EXTRN C EMM_VGAFILE_H:WORD
EXTRN C cnst_EMM_DataH_Name:BYTE
EXTRN C EMM_DATA_H:WORD
EXTRN C EMM_Data_Level:WORD
EXTRN C EMM_Table:EMM_Record
EXTRN C EMS_OK:WORD

EXTRN _strcpy:PROC
EXTRN _stricmp:PROC
EXTRN C EMS_Init:PROC ; _s12p01.asm
EXTRN C EMS_GetHandleCount:PROC ; _s12p02.asm
EXTRN C EMS_GetHandleName:PROC ; _s12p06.asm 
EXTRN C EMS_CreateHandle:PROC ; _s13p20.asm
;EXTRN C EMS_BuildEmsErrStr:PROC ; _s13p32.asm
EXTRN C EMS_BuildEmsErrStr:PROC ; _s13p32.cpp
EXTRN C EMS_GetFreePageCnt:PROC ; _s12p03.asm
EXTRN _GAME_QuitProgram:PROC ; _s05p02.asm

;segment dseg para public 'DATA' use16
.DATA
;ends dseg

segment seg013 byte public 'CODE' use16
    assume cs:seg013
    ;assume ds:dseg
    ;org 7
    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC C EMS_Startup

    LOCALS
proc EMS_Startup far

    varTmpStr100= byte ptr -66h
    varEmsOpenHandleCount= word ptr -2

    push bp
    mov  bp, sp
    sub  sp, 66h
    push si

    call EMS_Init
    or  ax, ax
    jnz @@EMM_Detected
    jmp @@EMM_Fail

@@EMM_Detected:

    call EMS_GetHandleCount
    mov [bp+varEmsOpenHandleCount], ax

; - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    mov [EmmHndlNbr_Yomoma], 0

    mov si, 1
    jmp @@LoopActiveHandles

@@GetHandleName:
    push si
    lea  ax, [bp+varTmpStr100]
    push ax
    call EMS_GetHandleName
    add sp, 04h                         ;pop cx, pop cx

    or ax, ax
    jz @@NextHandle

    lea ax, [bp+varTmpStr100]
    push ax
    mov ax, offset EmmHndlNm_Yomoma     ; EmmHndlNm_Yomoma db 'YO MOMA',0
    push ax
    call _stricmp
    add sp, 04h                         ;pop cx, pop cx

    or ax, ax
    jnz @@NextHandle

    mov [EmmHndlNbr_Yomoma], si         ; Found "YO MOMA"

@@NextHandle:
    inc si

@@LoopActiveHandles:
    cmp si, [bp+varEmsOpenHandleCount]
    jge short @@DoOurEmmHandles

    cmp [EmmHndlNbr_Yomoma], 0
    jz short @@GetHandleName

@@DoOurEmmHandles:
    mov [EMS_Open_Handles], 0
    cmp [EmmHndlNbr_Yomoma], 0
    jnz short @@HadFoundYomomaHndlName

;@@NeedYomomaHndl
    mov ax, 0 ; xor ax, ax              ; ? EMS Handle Reserved Flag ?
    push ax
    mov ax, offset EmmHndlNm_Yomoma2    ; EMM Handle Name
    push ax
    mov ax, 1                           ; EMS Logical Page Count
    push ax
    ;nop
    ;push cs
    call EMS_CreateHandle
    add sp, 6

    mov [EmmHndlNbr_Yomoma], ax
    jmp short @@DoneYomomaHndl

@@HadFoundYomomaHndlName:
    mov ax, offset EmmHndlNm_Yomoma
    push ax
    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch                         ; 12 bytes
    imul dx                             ; AX = DX * AX
    add ax, offset EMM_Table.HandleName
    push ax
    call _strcpy
    add sp, 04h                         ;pop cx, pop cx

    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch                         ; 12 bytes
    imul dx                             ; AX = DX * AX
    mov bx, ax
    mov [EMM_Table.Reserved+bx], 0
    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch                         ; 12 bytes
    imul dx                             ; AX = DX * AX
    mov dx, [EmmHndlNbr_Yomoma]
    mov bx, ax
    mov [EMM_Table.Handle+bx], dx
    mov [EMS_Open_Handles], 1

; - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

@@DoneYomomaHndl:
    cmp [EmmHndlNbr_Yomoma], 0
    jg short @@Success_CrtHndl_Yomoma

;@@Failure_CrtHndl_Yomoma
    lea ax, [bp+varTmpStr100]
    push ax
    ;nop
    ;push cs
    ; call EMS_BuildEmsErrStr
    call EMS_BuildEmsErrStr
    add sp, 02h                         ;pop cx

    lea ax, [bp+varTmpStr100]
    push ax
    call _GAME_QuitProgram
    add sp, 02h                         ;pop cx

@@Success_CrtHndl_Yomoma:
    mov [EMS_OK], 1
    mov ax, [EmmHndlNbr_Yomoma]
    inc ax
    mov si, ax
    jmp short loc_1656C

loc_1651D:
    push si
    lea ax, [bp+varTmpStr100]
    push ax
    call EMS_GetHandleName
    add sp, 04h                         ;pop cx, pop cx

    or ax, ax
    jz @@NextHandle2

    cmp [bp+varTmpStr100], 0
    jz @@NextHandle2

    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch
    imul dx
    mov bx, ax
    mov [EMM_Table.Reserved+bx], 0
    lea ax, [bp+varTmpStr100]
    push ax
    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch
    imul dx
    add ax, offset EMM_Table.HandleName
    push ax
    call _strcpy
    add sp, 04h                         ;pop cx, pop cx

    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch
    imul dx
    mov bx, ax
    mov [EMM_Table.Handle+bx], si
    inc [EMS_Open_Handles]

@@NextHandle2:
    inc si

loc_1656C:
    cmp si, 32h
    jge short loc_16578

    cmp [EMS_Open_Handles], 28h
    jl short loc_1651D

loc_16578:
    call EMS_GetFreePageCnt
    cmp ax, [EMS_Reserved_Left]
    jge short @@DoBaseGameAllocs

; - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    lea ax, [bp+varTmpStr100]
    push ax
    ;nop
    ;push cs
    ; call EMS_BuildEmsErrStr
    call EMS_BuildEmsErrStr
    add sp, 02h                         ;pop cx

    lea ax, [bp+varTmpStr100]
    push ax
    call _GAME_QuitProgram
    add sp, 02h                         ;pop cx

; - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

@@DoBaseGameAllocs:
    mov ax, 1
    push ax
    mov ax, offset cnst_EMM_VGAH_Name
    push ax
    mov ax, 5
    push ax
    ;nop
    ;push cs
    call EMS_CreateHandle
    add sp, 6

    mov [EMM_VGAFILE_H], ax
    
    mov ax, 1
    push ax
    mov ax, offset cnst_EMM_DataH_Name
    push ax
    mov ax, 4
    push ax
    ;nop
    ;push cs
    call EMS_CreateHandle
    add sp, 6

    mov [EMM_DATA_H], ax
    mov [EMM_Data_Level], 0
    jmp short @@done

; - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

@@EMM_Fail:
    lea ax, [bp+varTmpStr100]
    push ax
    ;nop
    ;push cs
    ; call EMS_BuildEmsErrStr
    call EMS_BuildEmsErrStr
    add sp, 02h                         ;pop cx

    lea ax, [bp+varTmpStr100]
    push ax
    call _GAME_QuitProgram
    add sp, 02h                         ;pop cx

; - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

@@done:

    pop si
    mov sp, bp
    pop bp
    ret

endp EMS_Startup

ends seg013

end
