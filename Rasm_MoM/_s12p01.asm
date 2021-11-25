TITLE _s12p01.asm EMM_Init

.MODEL LARGE, C

EXTRN g_EMM_PageFrame_Base_Address:WORD
EXTRN g_EMM_device_name:BYTE

;;segment dseg para public 'DATA' use16
;.DATA
;;dseg:40E6
;EMM_device_name db 'EMMXXXX0',0
;;dseg:40E4
;EMM_Page_Frame_Base_Address dw 0
;;ends dseg

.CODE
;segment seg012 byte public 'CODE' use16
;    assume cs:seg012
;    ;assume ds:dseg
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_Init

; checks for the presence of a LIM EMS 4+ manager, and
; if present, gets and sets the page frame address
; returns -1 if successful, 0 on failure
proc EMM_Init far

    push si
    push di
    push es
    push ds

    mov dx, offset g_EMM_device_name
    mov ax, 3D00h
    int 21h

    jb short @@error

    mov bx, ax
    mov ax, 4400h
    int 21h

    jb short @@error

    and dx, 80h ; test bit 7; 0 == File, 1 == Device

    jz short @@error

    mov ax, 4407h
    int 21h

    jb short @@error

    or al, al

    jz short @@error

    mov ah, 3Eh
    int 21h

    jb short @@error

    mov ah, 46h
    int 67h

    cmp ah, 0

    jnz short @@error

    cmp al, 40h

    js short @@error

    mov ah, 41h
    int 67h
    
    cmp ah, 0

    jnz short @@error

    mov [g_EMM_PageFrame_Base_Address], bx
    mov ax, 0FFFFh

    pop ds
    pop es
    pop di
    pop si
    ret

@@error:
    mov ax, 0

    pop ds
    pop es
    pop di
    pop si
    ret

endp EMM_Init

;ends seg012

end
