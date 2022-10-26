; _s21p07.asm FLIC_LoadPalette
; ST_VGA.H

.MODEL LARGE, C

EXTRN g_EMM_PageFrame_Base_Address:WORD
EXTRN gsa_Palette:WORD

.CODE
;segment seg021 byte public 'CODE' use16
;    assume cs:seg021
;    ;org 8
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

; Orig_Target_Red dw 0
; Orig_Target_Blu dw 0
; Orig_Target_Grn dw 0
; Pass1_DstColor_1 dw 0
; Pass1_Count dw 0
; Pass1_SrcColor1 dw 0
; Pass2_DstColor_1 dw 0
; Pass2_Count dw 0
; Pass2_SrcColor_1 dw 0
; Color_Wave_Start dw 0
; Color_Wave_HalfSize dw 0
; Color_Wave_Step dw 0
; Picked_Color dw 0
; Smallest_Diff dw 0
; Total_Target_Red db 0
; algn_1C4E5:
; align 2
; Total_Target_Grn db 0
; algn_1C4E7:
; align 2
; Total_Target_Blu db 0
; algn_1C4E9:
; align 2
; Orig_Multiplier dw 0
; Target_Color_Index dw 0
; Perc_Target_Red db 0
; algn_1C4EF:
; align 2
; Perc_Target_Grn db 0
; align 2
; Perc_Target_Blu db 0
; align 2

PUBLIC FLIC_LoadPalette

proc FLIC_LoadPalette

    FlicHdr_SgmtAddr= word ptr 6
    Frame_Index= word ptr 8

    push bp
    mov  bp, sp
    push si
    push di
    push es
    push ds

    mov cx, [bp+Frame_Index]

    mov ax, [bp+FlicHdr_SgmtAddr]
    mov ds, ax
    assume ds:nothing

    mov dx, [gsa_Palette]
    mov es, dx

    ;mov bl, [ds:LBX_IMG_HDR.EMM_Handle]
    mov bl, [ds:0Ah]  ; 0Ah 10d  LBX_IMG_HDR.EMM_Handle

    cmp bl, 0
    jnz short @@HaveEmmHndlNbr

    mov di, 0

@@PaletteDataOffset:
    mov si, 0Eh ; e_FlicHdr_PaletteDataOffset
    add si, di
    lodsw

    add ax, di  ; Offset = Memory Offset + Palette Data Offset
    mov si, ax
    mov bx, ax
    cmp cx, 0   ; Frame_Index
    jz short @@YayFrameZero

    cmp byte ptr [bx+6], 0  ; PaletteDataOffset + 6 
    jz short @@YayFrameZero

@@NayFrameZero:
    dec cx      ; ? Frame Number - 1 = Frame Index ?
    shl cx, 1
    shl cx, 1
    add si, cx
    add si, 8   ; ? Frame Offset to Frame Data ?
    lodsw       ; AX = [DS:SI]

    mov dx, ax
    add dx, di  ; DX = 
    lodsw

    cmp ah, 0
    jz short @@Done_DoNothing

    mov cl, ah
    xor ch, ch
    xor ah, ah
    mov di, ax
    mov si, dx
    jmp short @@SetDestinations

    ;db 90h

@@Done_DoNothing:
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

@@YayFrameZero:
    lodsw
    mov dx, ax
    add dx, di
    lodsw
    mov di, ax
    lodsw
    mov cx, ax
    mov si, dx
    jmp short @@SetDestinations

    ;db 90h

    assume ds:nothing

@@HaveEmmHndlNbr:
    ;mov di, [ds:LBX_IMG_HDR.EMM_Data_Offset]
    mov di, [ds:0Ch]  ; 0Ch 12d  LBX_IMG_HDR.EMM_Data_Offset
    xor bh, bh
    mov dx, bx

    push dx

    mov ax, 4400h
    ;mov bl, [ds:LBX_IMG_HDR.EMM_Logical_Page]
    mov bl, [ds:0Bh]  ; 0Bh 11d  LBX_IMG_HDR.EMM_Logical_Page
    xor bh, bh
    int 67h

    pop dx

    ;mov bl, [ds:LBX_IMG_HDR.EMM_Logical_Page]
    mov bl, [ds:0Bh]  ; 0Bh 11d  LBX_IMG_HDR.EMM_Logical_Page
    xor bh, bh
    mov ax, 4401h
    int 67h

    ;mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax
    ;assume ds:dseg
    assume ds:DGROUP

    mov ax, [g_EMM_PageFrame_Base_Address]
    mov ds, ax
    jmp short @@PaletteDataOffset

@@SetDestinations:
    mov bx, di
    shl di, 1
    add di, bx
    add bx, 300h    ; 300h 768d  Offset to Palette Change Flags in gsa_Palette

@@CopyColorAndSetFlag:
    movsw           ; Copy Red & Green
    movsb           ; Copy Blue
    mov byte ptr [es:bx], 1 ; Set Palette Change Flag
    inc bx
    loop @@CopyColorAndSetFlag

@@Done:
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

endp FLIC_LoadPalette

;ends seg021

end
