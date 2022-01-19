TITLE _s04p01.asm DISK_FileFind

.MODEL LARGE, C

.CODE
;segment seg004 byte public 'CODE' use16
; 	 assume cs:seg004
; 	 ;org 0Ch
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC DISK_FileFind

proc DISK_FileFind

    DTA_FileName_Ptr = dword ptr -4
    FileName_Ptr     = word ptr 6
    Read_Buffer      = word ptr 8

    push bp
    mov  bp, sp
    sub  sp, 4
    push es
    push di
    push ds
    push si

    push ds
    
    ;call MOUSE_Disable_CDraw ; TODO(JimBalcomb): MOUSE_Disable_CDraw

    mov ah, 2Fh
    int 21h

    add bx, 30
    mov word ptr [bp+DTA_FileName_Ptr], bx
    mov bx, es
    mov word ptr [bp+DTA_FileName_Ptr+2], bx
    mov si, [bp+FileName_Ptr]
    lodsb
    cmp al, 0
    jz short loc_141C4

    push ds

    mov ah, 4Eh
    mov cx, 0
    mov dx, [bp+FileName_Ptr]
    int 21h

    pop ds

    jb short loc_141F2
    jmp short loc_141CA

;algn_141C3:
;    align 2

loc_141C4:
    mov ah, 4Fh
    int 21h
    jb short loc_141F2

loc_141CA:
    mov ax, ds
    mov es, ax
    ;assume es:dseg

    mov di, [bp+Read_Buffer]
    mov si, word ptr [bp+DTA_FileName_Ptr]
    mov ax, word ptr [bp+DTA_FileName_Ptr+2]
    mov ds, ax

loc_141D9:
    lodsb
    stosb
    cmp al, 0
    jnz short loc_141D9

    mov ax, -1

    pop ds

    ;push ax
    ;call MOUSE_CDraw_Restore ; TODO(JimBalcomb): MOUSE_CDraw_Restore
    ;pop ax

    pop si
    pop ds
    pop di
    pop es
    assume es:nothing
    mov sp, bp
    pop bp
    ret

loc_141F2:
    mov al, 0
    mov si, [bp+Read_Buffer]
    mov [si], al
    mov ax, 0

    pop ds

    ;push ax
    ;call MOUSE_CDraw_Restore ; TODO(JimBalcomb): MOUSE_CDraw_Restore
    ;pop ax

    pop si
    pop ds
    pop di
    pop es
    mov sp, bp
    pop bp
    ret

endp DISK_FileFind

;ends seg004

end
