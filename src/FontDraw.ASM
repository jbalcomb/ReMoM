
/*
MoO2

Module: fontdraw
    code (0 bytes) Print_Character_No_Alias_ASM_
    Address: 01:00144A06

    code (0 bytes) Print_Character_ASM_
    Address: 01:001449CC

*/

; =============== S U B R O U T I N E =======================================
;
; _EAX = screen_start
; _EDX = data_offset
; _EBX = font_header
; _ECX = height

; CODE XREF: Print_Character+69p

Print_Character_ASM_ proc near

    pusha
    push esi
    push edi
    
    mov esi, edx        ; data_offset
    mov edi, eax        ; screen_start

    xor eax, eax

@@While_Width:
    mov edx, edi        ; screen_pos = screen_start
    lodsb
    cmp eax, 80h
    jz  short @@NextColumn

loc_1449DF:
    js  short loc_1449EE
    and ax, 7Fh
    add edi, eax
    lodsb
    cmp al, 80h
    jnz short loc_1449DF
    jmp short @@NextColumn

loc_1449EE:
    mov al, [eax+ebx-1]
    stosb
    lodsb
    cmp al, 80h
    jnz short loc_1449DF

@@NextColumn:
    mov edi, edx
    add edi, dword ptr screen_pixel_width2
    loop @@While_Width

@@Done:
    pop edi
    pop esi
    popa
    retn

Print_Character_ASM_ endp


; CODE XREF: Print_Character_No_Alias+69p

Print_Character_No_Alias_ASM_ proc near
    pusha
    push esi
    push edi

    mov esi, edx
    mov edi, eax

    xor eax, eax

@@LoopString:
    mov edx, edi
    lodsb               ; AX = DS:SI++
    cmp eax, 80h
    jz  short @@NextCharacter

loc_144A19:
    js  short loc_144A28
    and ax, 7Fh
    add edi, eax
    lodsb               ; AX = DS:SI++
    cmp al, 80h
    jnz short loc_144A19
    jmp short @@NextCharacter

loc_144A28:
    cmp al, 0
    jz  short loc_144A32
    mov al, [eax+ebx-1]
    stosb               ; ES:DI++ = AX
    dec edi

loc_144A32:
    inc edi
    lodsb
    cmp al, 80h
    jnz short loc_144A19

@@NextCharacter:
    mov edi, edx
    add edi, dword ptr screen_pixel_width2
    loop @@LoopString

@@Done:
    pop edi
    pop esi
    popa
    retn

Print_Character_No_Alias_ASM_ endp
