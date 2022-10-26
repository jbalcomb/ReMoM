TITLE _s13p20.asm EMS_CreateHandle

.MODEL LARGE, C

;INCLUDE ST_EMS.INC

.DATA
EMS_Record struc          ; (sizeof=0xC, standard type)
    EmsHndlNm   db 9 dup(?)             ; ~= char EmmHndlNm[EMM_HANDLE_NAME_SIZE_MAX] + '\0'
    Rsrvd       db ?                    ; ~= {0,1}
    EmsHndlNbr  dw ?                    ; ~= FAR PTR WORD / unsigned int
EMS_Record ends

EMS_L2P_Map_Record struc  ; (sizeof=0x4, standard type)
    LogicalPageNbr  dw ?                ; {0, ..., 2048}  32 MB / 16 KB = 2048 logical pages
    PhysicalPageNbr dw ?                ; {0, 1, 2, 3}
EMS_L2P_Map_Record ends

EXTRN EMS_Open_Handles:WORD
EXTRN EMS_Reserved_Left:WORD
EXTRN EMS_Table:EMS_Record
;;dseg:A5F2
;EMS_Table EMS_Record 28h dup(<0>)       ; 40
EXTRN Temp_String:BYTE
EXTRN Tmp_Conv_Str_1:BYTE
EXTRN Tmp_Conv_Str_2:BYTE
EXTRN Tmp_Conv_Str_3:BYTE
EXTRN cnst_EmsErr_ResOut1:BYTE
EXTRN cnst_EmsErr_ResOut2:BYTE
EXTRN cnst_EmsErr_ResOut3:BYTE
EXTRN cnst_EmsErr_Reload1:BYTE
EXTRN cnst_EmsErr_Space:BYTE


segment seg013 byte public 'CODE' use16
    assume cs:seg013
    ;assume ds:dseg
    ;org 7
    ;assume es:nothing, ss:nothing, ds:dseg

;EXTRN itoa:PROC
;EXTRN strcat:PROC
;EXTRN strcpy:PROC
;EXTRN stricmp:PROC
;EXTRN C itoa:PROC
;EXTRN C strcat:PROC
;EXTRN C strcpy:PROC
;EXTRN C stricmp:PROC
EXTRN itoa:PROC
EXTRN strcat:PROC
EXTRN strcpy:PROC
EXTRN stricmp:PROC
EXTRN EMS_MakeNamedHandle:PROC ; _s12p05.asm
EXTRN EMS_GetPageCount:PROC ; _s12p04.asm
EXTRN GAME_QuitProgram:PROC ; _s05p02.asm

PUBLIC EMS_CreateHandle

    LOCALS
proc EMS_CreateHandle far

    Temp_Conv_String= byte ptr -0Ah
    Loaded_Handle= word ptr -4
    LoopVar= word ptr -2
    argEmsLogicalPageCount = word ptr 6
    argEmsHandleName = word ptr 8
    argEmsRsvdFlag = word ptr 0Ah

    push bp
    mov bp, sp
    sub sp, 0Ah
    push si
    push di

    mov si, [bp+argEmsHandleName]
    cmp byte ptr [si], 0
    jnz @@NameNotNull

;@@Error_NoName
    mov ax, 0 ; xor ax, ax

@@JmpDone:
    jmp @@Done

@@NameNotNull:
    mov [bp+Loaded_Handle], 0FFFFh
    mov [bp+LoopVar], 0
    jmp @@LoopHandles

@@CheckName:
    mov ax, [bp+LoopVar]
    mov dx, 0Ch
    imul dx
    add ax, offset EMS_Table.EmsHndlNm
    push ax
    push si
    call stricmp
    add sp, 04h                         ; pop cx, pop cx

    or ax, ax
    jnz @@Continue

;@@FoundHandle
    mov ax, [bp+LoopVar]
    mov [bp+Loaded_Handle], ax

@@Continue:
    inc [bp+LoopVar]

@@LoopHandles:
    mov ax, [bp+LoopVar]
    cmp ax, [EMS_Open_Handles]
    jl @@CheckName

;@@CheckHadFoundHandle
    cmp [bp+Loaded_Handle], 0FFFFh ; db 83h,7Eh,0FCh,0FFh ; <BAD>cmp [bp+Loaded_Handle], -1
    jz @@HadNotFoundHandle

;@@JmpWierd
    jmp @@Wierd

@@HadNotFoundHandle:
    push si
    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch
    imul dx
    add ax, offset EMS_Table.EmsHndlNm
    push ax
    call strcpy
    add sp, 04h                         ; pop cx, pop cx

    cmp [bp+argEmsRsvdFlag], 1
    jnz loc_177F4

    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch
    imul dx
    mov bx, ax
    mov al, [byte ptr EMS_Table.EmsHndlNm+bx]
    neg al
    push ax
    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch
    imul dx
    mov bx, ax
    pop ax
    mov [byte ptr EMS_Table.EmsHndlNm+bx], al

loc_177F4:
    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch
    imul dx
    add ax, offset EMS_Table
    push ax
    push [bp+argEmsLogicalPageCount]
    call EMS_MakeNamedHandle
    add sp, 04h                         ; pop cx, pop cx

    mov di, ax
    or di, di
    jnz @@Success_MkNmHndl

;@@Failure_MkNmHndl
    jmp @@NewJmpJmpJmpDone

@@Success_MkNmHndl:
    push si
    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch
    imul dx
    add ax, offset EMS_Table.EmsHndlNm
    push ax
    call strcpy
    add sp, 04h                         ; pop cx, pop cx

    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch
    imul dx
    mov dl, byte ptr [bp+argEmsRsvdFlag]
    mov bx, ax
    mov [EMS_Table.Rsrvd+bx], dl
    mov ax, [EMS_Open_Handles]
    mov dx, 0Ch
    imul dx
    mov bx, ax
    mov [EMS_Table.EmsHndlNbr+bx], di
    inc [EMS_Open_Handles]
    cmp [bp+argEmsRsvdFlag], 1
    jnz short @@CheckEmsRsvdLeft

;@@@@DecrEmsRsvdLeft
    mov ax, [bp+argEmsLogicalPageCount]
    sub [EMS_Reserved_Left], ax

@@CheckEmsRsvdLeft:
    cmp [EMS_Reserved_Left], 0
    jge @@NewJmpJmpJmpDone

; - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -
;@@OutOfPages

    mov ax, 0Ah
    push ax
    lea ax, [bp+Temp_Conv_String]
    push ax
    mov ax, [EMS_Reserved_Left]
    neg ax
    push ax
    call itoa
    add sp, 6

    mov ax, offset cnst_EmsErr_ResOut1
    push ax
    mov ax, offset Temp_String
    push ax
    call strcpy
    add sp, 04h                         ; pop cx, pop cx

    lea ax, [bp+Temp_Conv_String]
    push ax
    mov ax, offset Temp_String
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_EmsErr_ResOut2
    push ax
    mov ax, offset Temp_String
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    push si
    mov ax, offset Temp_String
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_EmsErr_ResOut3
    push ax
    mov ax, offset Temp_String
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset Temp_String
    push ax
    call GAME_QuitProgram
    add sp, 02h                         ; pop cx

; - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

@@NewJmpJmpJmpDone:
    jmp @@JmpJmpDone

@@Wierd:
    mov ax, [bp+Loaded_Handle]
    mov dx, 0Ch
    imul dx
    mov bx, ax
    mov di, [EMS_Table.EmsHndlNbr+bx]
    push di
    call EMS_GetPageCount
    add sp, 02h                         ; pop cx

    cmp ax, [bp+argEmsLogicalPageCount]
    jl @@NotEnoughEmsPages

;@@JmpJmpJmpDone
    jmp @@JmpJmpDone

; - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

@@NotEnoughEmsPages:
    mov ax, 0Ah
    push ax
    mov ax, offset Tmp_Conv_Str_1
    push ax
    mov ax, [bp+Loaded_Handle]
    mov dx, 0Ch
    imul dx
    mov bx, ax
    push [EMS_Table.EmsHndlNbr+bx]
    call itoa
    add sp, 6

    mov ax, 0Ah
    push ax
    mov ax, offset Tmp_Conv_Str_2
    push ax
    push di
    call itoa
    add sp, 6

    mov ax, 0Ah
    push ax
    mov ax, offset Tmp_Conv_Str_3
    push ax
    push [bp+argEmsLogicalPageCount]
    call itoa
    add sp, 6

    push si
    mov ax, offset Temp_String
    push ax
    call strcpy
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_EmsErr_Reload1
    push ax
    mov ax, offset Temp_String
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset Tmp_Conv_Str_1
    push ax
    mov ax, offset Temp_String
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_EmsErr_Space
    push ax
    mov ax, offset Temp_String
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset Tmp_Conv_Str_2
    push ax
    mov ax, offset Temp_String
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset cnst_EmsErr_Space
    push ax
    mov ax, offset Temp_String
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset Tmp_Conv_Str_3
    push ax
    mov ax, offset Temp_String
    push ax
    call strcat
    add sp, 04h                         ; pop cx, pop cx

    mov ax, offset Temp_String
    push ax
    call GAME_QuitProgram
    add sp, 02h                         ; pop cx

; - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

@@JmpJmpDone:
    mov ax, di
    jmp @@JmpDone

@@Done:
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp EMS_CreateHandle

ends seg013

end
