TITLE _s13p32.asm EMM_GetLowString

.MODEL LARGE

EXTRN C EMS_MinKB:WORD ; NOTE: ONLY Xref'd from EMM_SetMinKB() and EMS_GetLowString()

EXTRN _itoa:PROC
EXTRN _strcat:PROC
EXTRN _strcpy:PROC

;segment dseg para public 'DATA' use16
.DATA
;dseg:4184 59 6F 75 20 6D 75 73 74 20 68+    cnst_EMMErr_TooLow1 db 'You must have at least ',0
cnst_EMMErr_TooLow1 db 'You must have at least ',0
;dseg:419C 4B 20 6F 66 20 65 78 70 61 6E+    cnst_EMMErr_TooLow2 db 'K of expanded memory.',0
cnst_EMMErr_TooLow2 db 'K of expanded memory.',0
;ends dseg

segment seg013 byte public 'CODE' use16
    assume cs:seg013
    ;org 7
    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC C EMS_GetLowString

proc EMS_GetLowString far

    Temp_String= byte ptr -14h
    Str_Buffer= word ptr 6

    push bp
    mov bp, sp
    sub sp, 14h
    push si

    mov  si, [bp+Str_Buffer]
    
    mov  ax, 0Ah
    push ax
    lea  ax, [bp+Temp_String]
    push ax
    push [EMS_MinKB]
    call _itoa
    add  sp, 6

    mov  ax, offset cnst_EMMErr_TooLow1
    push ax
    push si
    call _strcpy
    add  sp, 04h                         ;pop cx, pop cx

    lea  ax, [bp+Temp_String]
    push ax
    push si
    call _strcat
    add  sp, 04h                         ;pop cx, pop cx

    mov  ax, offset cnst_EMMErr_TooLow2
    push ax
    push si
    call _strcat
    add  sp, 04h                         ;pop cx, pop cx

    pop  si
    mov  sp, bp
    pop  bp
    ret

endp EMS_GetLowString

ends seg013

end
