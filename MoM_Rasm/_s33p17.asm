TITLE _s33p17.asm KD_GetKeyCode
; ST_GUI.H

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC KD_GetKeyCode

; enum EKEYCODES
EKey_Num7_Home  = 47h
EKey_Num8_Up  = 48h
EKey_Num9_PgUp  = 49h
EKey_NumMinus  = 4Ah
EKey_Num4_Left  = 4Bh
EKey_Num5  = 4Ch
EKey_Num6_Right  = 4Dh
EKey_NumPlus  = 4Eh
EKey_Num1_End  = 4Fh
EKey_Num2_Down  = 50h
EKey_Num3_PgDn  = 51h
EKey_Esc  = 11Bh
EKey_NumMult  = 92Ah
EKey_Backspace  = 0E08h
EKey_Tab  = 0F09h
EKey_Alt_q  = 1000h
EKey_Alt_w  = 1100h
EKey_Alt_e  = 1200h
EKey_Alt_r  = 1300h
EKey_Alt_t  = 1400h
EKey_Alt_y  = 1500h
EKey_Alt_u  = 1600h
EKey_Alt_i  = 1700h
EKey_Alt_o  = 1800h
EKey_Alt_p  = 1900h
EKey_Enter  = 1C0Dh
EKey_Alt_a  = 1E00h
EKey_Alt_s  = 1F00h
EKey_Ctrl_s  = 1F13h
EKey_Alt_d  = 2000h
EKey_Alt_f  = 2100h
EKey_Alt_g  = 2200h
EKey_Alt_h  = 2300h
EKey_Alt_j  = 2400h
EKey_Alt_k  = 2500h
EKey_Alt_l  = 2600h
EKey_Alt_z  = 2C00h
EKey_Alt_x  = 2D00h
EKey_Alt_c  = 2E00h
EKey_Alt_v  = 2F00h
EKey_Alt_b  = 3000h
EKey_Alt_n  = 3100h
EKey_Alt_m  = 3200h
EKey_F1  = 3B00h
EKey_F2  = 3C00h
EKey_F3  = 3D00h
EKey_F4  = 3E00h
EKey_F5  = 3F00h
EKey_F6  = 4000h
EKey_F7  = 4100h
EKey_F8  = 4200h
EKey_F9  = 4300h
EKey_F10  = 4400h
EKey_Home  = 47E0h
EKey_Up  = 48E0h
EKey_PgUp  = 49E0h
EKey_Left  = 4BE0h
EKey_Right  = 4DE0h
EKey_End  = 4FE0h
EKey_Down  = 50E0h
EKey_PgDn  = 51E0h
EKey_Ins  = 52E0h
EKey_Del  = 53E0h
EKey_Ctrl_Num4  = 7300h
EKey_Ctrl_Left  = 73E0h
EKey_Ctrl_Num6  = 7400h
EKey_Ctrl_Right  = 74E0h
EKey_Ctrl_Num1  = 7500h
EKey_Ctrl_Num3  = 7600h
EKey_Ctrl_Num7  = 7700h
EKey_Ctrl_Num9  = 8400h
EKey_F11  = 8500h
EKey_F12  = 8600h
EKey_Ctrl_Num8  = 8D00h
EKey_Ctrl_Up  = 8DE0h
EKey_Ctrl_Num2  = 9100h
EKey_Ctrl_Down  = 91E0h
EKey_NumEnter  = 0E00Dh


; enum Key_Press
KP_NumMult  = 0
KP_Left  = 1
KP_Right  = 2
KP_Up  = 3
KP_Down  = 4
KP_RightUp  = 5
KP_RightDown  = 6
KP_LeftUp  = 7
KP_LeftDown  = 8
KP_Insert  = 9
KP_Delete  = 0Ah
KP_Backspace  = 0Bh
KP_Enter  = 0Ch
KP_Tab  = 0Dh
KP_F1  = 0Eh
KP_F2  = 0Fh
KP_F3  = 10h
KP_F4  = 11h
KP_F5  = 12h
KP_F6  = 13h
KP_F7  = 14h
KP_F8  = 15h
KP_F9  = 16h
KP_F10  = 17h
; ? 18h ?
KP_PgUp  = 19h
KP_Home  = 1Ah
KP_Esc  = 1Bh ; this happens to be its actual ascii value
KP_End  = 1Ch
KP_PgDn  = 1Dh
KP_Center  = 1Eh
; ? 1Fh ?
; ... 31-42
; ? 2Ah ?
KP_NumPlus  = 2Bh
; ? 2Bh ?
KP_NumMinus  = 2Dh
; ? 2Eh ?
; ... 46-127
; ? 7Fh ?
KP_Ctrl_Right  = 80h
KP_Ctrl_Left  = 81h
KP_Ctrl_Up  = 82h
KP_Ctrl_Down  = 83h
KP_Ctrl_LeftUp  = 84h
KP_Ctrl_RightUp  = 85h
KP_Ctrl_LeftDown  = 86h
KP_Ctrl_RightDown  = 87h
; ? 88h ?
; ... 136-143
; ? 8Fh ?
KP_Ctrl_s  = 90h
KP_F11  = 91h
KP_F12  = 92h
; ? 93h ?
; ... 147-159
; ? 9Fh ?
KP_Alt_a  = 0A0h
KP_Alt_b  = 0A1h
KP_Alt_c  = 0A2h
KP_Alt_d  = 0A3h
KP_Alt_e  = 0A4h
KP_Alt_f  = 0A5h
KP_Alt_g  = 0A6h
KP_Alt_h  = 0A7h
KP_Alt_i  = 0A8h
KP_Alt_j  = 0A9h
KP_Alt_k  = 0AAh
KP_Alt_l  = 0ABh
KP_Alt_m  = 0ACh
KP_Alt_n  = 0ADh
KP_Alt_o  = 0AEh
KP_Alt_p  = 0AFh
KP_Alt_q  = 0B0h
KP_Alt_r  = 0B1h
KP_Alt_s  = 0B2h
KP_Alt_t  = 0B3h
KP_Alt_u  = 0B4h
KP_Alt_v  = 0B5h
KP_Alt_w  = 0B6h
KP_Alt_x  = 0B7h
KP_Alt_y  = 0B8h
KP_Alt_z  = 0B9h
; ? 0BAh ?
; ... 186-255
; ? 0FFh ?


Key_Translator struc ; (sizeof=0x4, standard type)
KBD_Read_Value dw ?                     ; enum EKEYCODES
Input_Value dw ?                        ; enum Key_Press
Key_Translator ends

Alt_Keycodes Key_Translator <EKey_Alt_a, KP_Alt_a>
Key_Translator <EKey_Alt_b, KP_Alt_b>
Key_Translator <EKey_Alt_c, KP_Alt_c>
Key_Translator <EKey_Alt_d, KP_Alt_d>
Key_Translator <EKey_Alt_e, KP_Alt_e>
Key_Translator <EKey_Alt_f, KP_Alt_f>
Key_Translator <EKey_Alt_g, KP_Alt_g>
Key_Translator <EKey_Alt_h, KP_Alt_h>
Key_Translator <EKey_Alt_i, KP_Alt_i>
Key_Translator <EKey_Alt_j, KP_Alt_j>
Key_Translator <EKey_Alt_k, KP_Alt_k>
Key_Translator <EKey_Alt_l, KP_Alt_l>
Key_Translator <EKey_Alt_m, KP_Alt_m>
Key_Translator <EKey_Alt_n, KP_Alt_n>
Key_Translator <EKey_Alt_o, KP_Alt_o>
Key_Translator <EKey_Alt_p, KP_Alt_p>
Key_Translator <EKey_Alt_q, KP_Alt_q>
Key_Translator <EKey_Alt_r, KP_Alt_r>
Key_Translator <EKey_Alt_s, KP_Alt_s>
Key_Translator <EKey_Alt_t, KP_Alt_t>
Key_Translator <EKey_Alt_u, KP_Alt_u>
Key_Translator <EKey_Alt_v, KP_Alt_v>
Key_Translator <EKey_Alt_w, KP_Alt_w>
Key_Translator <EKey_Alt_x, KP_Alt_x>
Key_Translator <EKey_Alt_y, KP_Alt_y>
Key_Translator <EKey_Alt_z, KP_Alt_z>

proc KD_GetKeyCode

    push es
    push ds

    mov ah, 10h
    int 16h

    cmp ax, EKey_Esc
    jnz short loc_242DE

    mov ax, KP_Esc
    pop ds
    pop es
    ret

loc_242DE:
    cmp ax, EKey_Left
    jnz short loc_242E9
    mov ax, KP_Left
    pop ds
    pop es
    ret

loc_242E9:
    cmp ax, EKey_Right
    jnz short loc_242F4
    mov ax, KP_Right
    pop ds
    pop es
    ret

loc_242F4:
    cmp ax, EKey_Up
    jnz short loc_242FF
    mov ax, KP_Up
    pop ds
    pop es
    ret

loc_242FF:
    cmp ax, EKey_Down
    jnz short loc_2430A
    mov ax, KP_Down
    pop ds
    pop es
    ret

loc_2430A:
    cmp ax, EKey_PgUp
    jnz short loc_24315
    mov ax, KP_RightUp
    pop ds
    pop es
    ret

loc_24315:
    cmp ax, EKey_PgDn
    jnz short loc_24320
    mov ax, KP_RightDown
    pop ds
    pop es
    ret

loc_24320:
    cmp ax, EKey_Home
    jnz short loc_2432B
    mov ax, KP_LeftUp
    pop ds
    pop es
    ret

loc_2432B:
    cmp ax, EKey_End
    jnz short loc_24336
    mov ax, KP_LeftDown
    pop ds
    pop es
    ret

loc_24336:
    cmp ax, EKey_Ins
    jnz short loc_24341

loc_2433B:
    mov ax, KP_Insert
    pop ds
    pop es
    ret

loc_24341:
    cmp ax, EKey_Del
    jnz short loc_2434C
    mov ax, KP_Delete
    pop ds
    pop es
    ret

loc_2434C:
    cmp ax, EKey_Backspace
    jnz short loc_24357
    mov ax, KP_Backspace
    pop ds
    pop es
    ret

loc_24357:
    cmp ax, EKey_Enter
    jnz short loc_24362
    mov ax, KP_Enter
    pop ds
    pop es
    ret

loc_24362:
    cmp ax, EKey_Tab
    jnz short loc_2436D
    mov ax, KP_Tab
    pop ds
    pop es
    ret

loc_2436D:
    cmp ax, EKey_F1
    jnz short loc_24378
    mov ax, KP_F1
    pop ds
    pop es
    ret

loc_24378:
    cmp ax, EKey_F2
    jnz short loc_24383
    mov ax, KP_F2
    pop ds
    pop es
    ret

loc_24383:
    cmp ax, EKey_F3
    jnz short loc_2438E
    mov ax, KP_F3
    pop ds
    pop es
    ret

loc_2438E:
    cmp ax, EKey_F4
    jnz short loc_24399
    mov ax, KP_F4
    pop ds
    pop es
    ret

loc_24399:
    cmp ax, EKey_F5
    jnz short loc_243A4
    mov ax, KP_F5
    pop ds
    pop es
    ret

loc_243A4:
    cmp ax, EKey_F6
    jnz short loc_243AF
    mov ax, KP_F6
    pop ds
    pop es
    ret

loc_243AF:
    cmp ax, EKey_F7
    jnz short loc_243BA
    mov ax, KP_F7
    pop ds
    pop es
    ret

loc_243BA:
    cmp ax, EKey_F8
    jnz short loc_243C5
    mov ax, KP_F8
    pop ds
    pop es
    ret

loc_243C5:
    cmp ax, EKey_F9
    jnz short loc_243D0
    mov ax, KP_F9
    pop ds
    pop es
    ret

loc_243D0:
    cmp ax, EKey_F10
    jnz short loc_243DB
    mov ax, KP_F10
    pop ds
    pop es
    ret

loc_243DB:
    cmp ah, EKey_Num7_Home
    jnz short loc_243E6
    mov ax, KP_Home
    pop ds
    pop es
    ret
loc_243E6:
    cmp ah, EKey_Num9_PgUp
    jnz short loc_243F1
    mov ax, KP_PgUp
    pop ds
    pop es
    ret

loc_243F1:
    cmp ah, EKey_Num1_End
    jnz short loc_243FC
    mov ax, KP_End
    pop ds
    pop es
    ret

loc_243FC:
    cmp ah, EKey_Num3_PgDn
    jnz short loc_24407
    mov ax, KP_PgDn
    pop ds
    pop es
    ret

loc_24407:
    cmp ah, EKey_Num8_Up
    jnz short loc_24412
    mov ax, KP_Up
    pop ds
    pop es
    ret

loc_24412:
    cmp ah, EKey_Num2_Down
    jnz short loc_2441D
    mov ax, KP_Down
    pop ds
    pop es
    ret

loc_2441D:
    cmp ah, EKey_Num4_Left
    jnz short loc_24428
    mov ax, KP_Left
    pop ds
    pop es
    ret

loc_24428:
    cmp ah, EKey_Num6_Right
    jnz short loc_24433
    mov ax, KP_Right
    pop ds
    pop es
    ret

loc_24433:
    cmp ah, EKey_NumPlus
    jnz short loc_2443E
    mov ax, KP_NumPlus
    pop ds
    pop es
    ret

loc_2443E:
    cmp ah, EKey_NumMinus
    jnz short loc_24449
    mov ax, KP_NumMinus
    pop ds
    pop es
    ret

loc_24449:
    cmp ah, EKey_Num5
    jnz short loc_24454
    mov ax, KP_Center
    pop ds
    pop es
    ret

loc_24454:
    cmp ax, EKey_Ctrl_Right
    jnz short loc_2445F
    mov ax, KP_Ctrl_Right
    pop ds
    pop es
    ret

loc_2445F:
    cmp ax, EKey_Ctrl_Left
    jnz short loc_2446A
    mov ax, KP_Ctrl_Left
    pop ds
    pop es
    ret

loc_2446A:
    cmp ax, EKey_Ctrl_Up
    jnz short loc_24475
    mov ax, KP_Ctrl_Up
    pop ds
    pop es
    ret

loc_24475:
    cmp ax, EKey_Ctrl_Down
    jnz short loc_24480
    mov ax, KP_Ctrl_Down
    pop ds
    pop es
    ret

loc_24480:
    cmp ax, EKey_Ctrl_Num6
    jnz short loc_2448B
    mov ax, KP_Ctrl_Right
    pop ds
    pop es
    ret

loc_2448B:
    cmp ax, EKey_Ctrl_Num4
    jnz short loc_24496
    mov ax, KP_Ctrl_Left
    pop ds
    pop es
    ret

loc_24496:
    cmp ax, EKey_Ctrl_Num8
    jnz short loc_244A1
    mov ax, KP_Ctrl_Up
    pop ds
    pop es
    ret

loc_244A1:
    cmp ax, EKey_Ctrl_Num2
    jnz short loc_244AC
    mov ax, KP_Ctrl_Down
    pop ds
    pop es
    ret

loc_244AC:
    cmp ax, EKey_Ctrl_Num7
    jnz short loc_244B7
    mov ax, KP_Ctrl_LeftUp
    pop ds
    pop es
    ret

loc_244B7:
    cmp ax, EKey_Ctrl_Num9
    jnz short loc_244C2
    mov ax, KP_Ctrl_RightUp
    pop ds
    pop es
    ret

loc_244C2:
    cmp ax, EKey_Ctrl_Num1
    jnz short loc_244CD
    mov ax, KP_Ctrl_LeftDown
    pop ds
    pop es
    ret

loc_244CD:
    cmp ax, EKey_Ctrl_Num3
    jnz short loc_244D8
    mov ax, KP_Ctrl_RightDown
    pop ds
    pop es
    ret

loc_244D8:
    cmp ax, EKey_NumMult
    jnz short loc_244E2
    sub ax, ax
    pop ds
    pop es
    ret

loc_244E2:
    cmp ax, EKey_NumEnter
    jnz short loc_244ED
    mov ax, KP_Enter
    pop ds
    pop es
    ret

loc_244ED:
    cmp ax, EKey_Ctrl_s
    jnz short loc_244F8
    mov ax, KP_Ctrl_s
    pop ds
    pop es
    ret

loc_244F8:
    cmp ax, EKey_F11
    jnz short loc_24503
    mov ax, KP_F11
    pop ds
    pop es
    ret

loc_24503:
    cmp ax, EKey_F12
    jnz short loc_2450B
    mov ax, KP_F12

loc_2450B:
    mov bx, offset Alt_Keycodes
    mov dx, 6Ah
    add dx, 4

loc_24514:
    mov cx, [cs:bx+Key_Translator.KBD_Read_Value]
    cmp ax, cx
    jz short loc_24524
    add bx, 4
    cmp bx, dx
    jz short @@Done
    jmp short loc_24514

loc_24524:
    inc bx
    inc bx
    mov ax, [cs:bx]

@@Done:
    pop ds
    pop es
    ret

endp KD_GetKeyCode

;ends seg033

end
