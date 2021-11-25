TITLE _s34p65.asm GUI_Init

.MODEL LARGE, C

.CODE
;segment    seg034 byte public 'CODE' use16
;    assume cs:seg034
;    ;org 0Eh
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC GUI_Init

proc GUI_Init far

Input_Type= word ptr  6

push    bp
mov    bp, sp
mov    ax, 165h
push    ax
call    LBX_Alloc_Space

pop    cx
push    ax
call    MK_FAR

pop    cx
mov    [word ptr GUI_Control_Table@+2], dx
mov    [word ptr GUI_Control_Table@], ax
mov    ax, [bp+Input_Type]
or    ax, ax
jz    short loc_2B8DF

cmp    ax, 1
jz    short loc_2B907

cmp    ax, 2
jz    short loc_2B94F

jmp    short loc_2B951

; ---------------------------------------------------------------------------

loc_2B8DF:
call    CRP_MOUSE_SetUsable

mov    [MOUSE_Hardware], 0
mov    [MOUSE_Emu_X], 158
mov    [MOUSE_Emu_Y], 100
mov    ax, 100
push    ax
mov    ax, 158
push    ax
call    MOUSE_MoveCursor

pop    cx
pop    cx
jmp    short loc_2B953

; ---------------------------------------------------------------------------

loc_2B907:
mov    ax, offset GUI_VirtualScreen
push    ax
mov    ax, 1
push    ax
call    GUI_SetWindows

pop    cx
pop    cx
call    MOUSE_Init

mov    [MOUSE_Hardware], ax
cmp    [MOUSE_Hardware], 0
jnz    short loc_2B947

call    CRP_MOUSE_SetUsable

mov    [MOUSE_Emu_X], 158
mov    [MOUSE_Emu_Y], 100
mov    ax, 100
push    ax
mov    ax, 158
push    ax
call    MOUSE_MoveCursor

pop    cx
pop    cx
jmp    short loc_2B94D

; ---------------------------------------------------------------------------

loc_2B947:
mov    [MOUSE_Hardware], 1


loc_2B94D:
jmp    short loc_2B953

; ---------------------------------------------------------------------------

loc_2B94F:
jmp    short loc_2B8DF

; ---------------------------------------------------------------------------

loc_2B951:
jmp    short $+2


loc_2B953:
mov    ax, 100
push    ax
mov    ax, 158
push    ax
call    VGA_SaveCursorArea

pop    cx
pop    cx
mov    [GUI_Delay], 0
mov    [GUI_FocusedControl], -1
mov    [GUI_EscapeOverride], 0
push    cs
call    near ptr GUI_Clear

pop    bp
retf

endp GUI_Init

;ends seg034

end
