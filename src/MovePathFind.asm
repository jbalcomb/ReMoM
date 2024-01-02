

WORLD_WIDTH  EQU  60
WORLD_SIZE_DD  EQU  1200
WORLD_SIZE_DW  EQU  2400
WORLD_SIZE_DB  EQU  4800


OVL_Path_Results struc ; (sizeof=0x2780)
Tile_Costs db 2400 dup(?)
Reach_Costs db 2400 dup(?)
Reach_From dw 2400 dup(?)
Reverse_Path dw 256 dup(?)
OVL_Path_Results ends


_DATA SEGMENT
CS_Value_Change  DB  1
CS_Row_Start     DW  0
ofst_movemap     DW  0
_DATA ENDS


_TEXT SEGMENT

PUBLIC Move_Path_Find_ASM

; ¿ Prologue & Epiloge ?
; ¿ is leaf or non-leaf function ?
; ¿ allocate stack space for local variables ?
; ¿ allocate stack space for parameter homing area ?
; ¿ Shadow Space ?
; ¿ 16-byte alignment ?

; Move_Path_Find_C(int16_t src_wx, int16_t src_wy, int8_t far * movepath_cost_map)

; Move_Path_Find_ASM PROC wx:WORD,wy:WORD,movepath_cost_map:QWORD
; mov    wx, cx
; mov    wy, dx
; mov     movepath_cost_map, r8
; mov  ax, wy
; mov  bx, WORLD_WIDTH
; mul  bx
; mov  CS_Row_Start,  ax
; ret
; Move_Path_Find_ASM ENDP

Move_Path_Find_ASM PROC

wx = word ptr  -2
wy = word ptr  -4
movepath_cost_map = qword ptr -12

push   rbp                     ; save frame pointer
mov    rbp, rsp                ; fix stack pointer
sub    rsp, (2 + 2 + 8 + 4)    ; allocate shadow register area for parameters and do 16-byte stack alignment

; save the parameters that were passed through registers into the Parameter Homing Area that was allocated by the caller function
mov     word ptr [rbp-2], cx
mov     word ptr [rbp-4], dx
mov    qword ptr [rbp-8], r8
mov    [rbp+wx], cx
mov    [rbp+wy], dx
mov    [rbp+movepath_cost_map], r8


mov  rax, 0
mov  ax, [rbp+wy]
mov  bx, WORLD_WIDTH
mul  bx
mov  CS_Row_Start,  ax

add  ax, [rbp+wx]
movzx  rdx, ax
; mov  ax, word ptr [rbp+movepath_cost_map]
; mov  ds, ax
; mov  es, ax
mov rdi, [rbp+movepath_cost_map]
add rdi, OVL_Path_Results.Reach_Costs
mov  cx, 1200
mov  ax, -1
rep stosw

; mov    di, OVL_Path_Results.Reach_From
mov rdi, [rbp+movepath_cost_map]
add rdi, OVL_Path_Results.Reach_Costs
mov    cx, WORLD_SIZE_DD
mov    ax, 0
loc_D5147:
stosw
inc    ax
loop    loc_D5147

; mov    di, dx
; add    di, OVL_Path_Results.Reach_Costs
mov rdi, [rbp+movepath_cost_map]
add    rdi, rdx
add    rdi, OVL_Path_Results.Reach_Costs
mov    ax, 0  ; set the cost to 0, for the src world map square idx
stosb

loc_D5155:
mov    CS_Value_Change, 0
; mov    si, 0
mov		rsi, [rbp+movepath_cost_map]
mov		ofst_movemap, 0
; mov    bx, OVL_Path_Results.Reach_Costs
mov		rbx, [rbp+movepath_cost_map]
add		rbx, OVL_Path_Results.Reach_Costs
; mov    di, OVL_Path_Results.Reach_From
mov		rdi, [rbp+movepath_cost_map]
add		rdi, OVL_Path_Results.Reach_From
add    si, CS_Row_Start
mov		cx, CS_Row_Start
add    ofst_movemap, cx
add    bx, CS_Row_Start
add    di, CS_Row_Start
add    di, CS_Row_Start

loc_D5178:
mov    ch, 0
lodsb
inc		ofst_movemap
cmp    al, 0FFh
jnz    short loc_D5182
jmp    loc_D525E

loc_D5182:
inc    ch
mov    dl, al
mov    al, [rbx-1]
add    al, dl
jb    short loc_D51A1
cmp    al, [rbx]
jnb    short loc_D51A1
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -2
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D51A1:
mov    al, [rbx-3Ch]
add    al, dl
jb    short loc_D51BC
cmp    al, [rbx]
jnb    short loc_D51BC
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Dh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D51BC:
mov    al, [rbx-3Bh]
add    al, dl
jb    short loc_D51D7
cmp    al, [rbx]
jnb    short loc_D51D7
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Ch
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D51D7:
mov    al, [rbx+3Bh]
add    al, dl
jb    short loc_D51F2
cmp    al, [rbx]
jnb    short loc_D51F2
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ah
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D51F2:
mov    al, [rbx+1]
add    al, dl
jb    short loc_D520D
cmp    al, [rbx]
jnb    short loc_D520D
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D520D:
mov    al, [rbx+77h]
add    al, dl
jb    short loc_D5228
cmp    al, [rbx]
jnb    short loc_D5228
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 76h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5228:
mov    al, [rbx+3Ch]
add    al, dl
jb    short loc_D5243
cmp    al, [rbx]
jnb    short loc_D5243
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Bh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5243:
mov    al, [rbx+3Dh]
add    al, dl
jb    short loc_D525E
cmp    al, [rbx]
jnb    short loc_D525E
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ch
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D525E:
inc    bx
inc    di
inc    di
mov    cl, 29
loc_D5263:
lodsb
inc		ofst_movemap
cmp    al, 0FFh
jnz    short loc_D526B
jmp    loc_D5347
loc_D526B:
inc    ch
mov    dl, al
mov    al, [rbx-3Dh]
add    al, dl
jb    short loc_D528A
cmp    al, [rbx]
jnb    short loc_D528A
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Eh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D528A:
mov    al, [rbx-3Ch]
add    al, dl
jb    short loc_D52A5
cmp    al, [rbx]
jnb    short loc_D52A5
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Dh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D52A5:
mov    al, [rbx-3Bh]
add    al, dl
jb    short loc_D52C0
cmp    al, [rbx]
jnb    short loc_D52C0
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Ch
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D52C0:
mov    al, [rbx-1]
add    al, dl
jb    short loc_D52DB
cmp    al, [rbx]
jnb    short loc_D52DB
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -2
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D52DB:
mov    al, [rbx+1]
add    al, dl
jb    short loc_D52F6
cmp    al, [rbx]
jnb    short loc_D52F6
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D52F6:
mov    al, [rbx+3Bh]
add    al, dl
jb    short loc_D5311
cmp    al, [rbx]
jnb    short loc_D5311
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ah
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5311:
mov    al, [rbx+3Ch]
add    al, dl
jb    short loc_D532C
cmp    al, [rbx]
jnb    short loc_D532C
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Bh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D532C:
mov    al, [rbx+3Dh]
add    al, dl
jb    short loc_D5347
cmp    al, [rbx]
jnb    short loc_D5347
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ch
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5347:
inc    bx
inc    di
inc    di
lodsb
inc		ofst_movemap
cmp    al, 0FFh
jnz    short loc_D5352
jmp    loc_D542E
loc_D5352:
inc    ch
mov    dl, al
mov    al, [rbx-3Dh]
add    al, dl
jb    short loc_D5371
cmp    al, [rbx]
jnb    short loc_D5371
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Eh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5371:
mov    al, [rbx-3Ch]
add    al, dl
jb    short loc_D538C
cmp    al, [rbx]
jnb    short loc_D538C
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Dh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D538C:
mov    al, [rbx-3Bh]
add    al, dl
jb    short loc_D53A7
cmp    al, [rbx]
jnb    short loc_D53A7
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Ch
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D53A7:
mov    al, [rbx-1]
add    al, dl
jb    short loc_D53C2
cmp    al, [rbx]
jnb    short loc_D53C2
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -2
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D53C2:
mov    al, [rbx+1]
add    al, dl
jb    short loc_D53DD
cmp    al, [rbx]
jnb    short loc_D53DD
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D53DD:
mov    al, [rbx+3Bh]
add    al, dl
jb    short loc_D53F8
cmp    al, [rbx]
jnb    short loc_D53F8
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ah
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D53F8:
mov    al, [rbx+3Ch]
add    al, dl
jb    short loc_D5413
cmp    al, [rbx]
jnb    short loc_D5413
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Bh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5413:
mov    al, [rbx+3Dh]
add    al, dl
jb    short loc_D542E
cmp    al, [rbx]
jnb    short loc_D542E
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ch
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D542E:
inc    bx
inc    di
inc    di
dec    cl
jz    short loc_D5438
jmp    loc_D5263
loc_D5438:
lodsb
inc		ofst_movemap
cmp    al, 0FFh
jnz    short loc_D5440
jmp    loc_D551C
loc_D5440:
inc    ch
mov    dl, al
mov    al, [rbx-3Dh]
add    al, dl
jb    short loc_D545F
cmp    al, [rbx]
jnb    short loc_D545F
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Eh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D545F:
mov    al, [rbx-3Ch]
add    al, dl
jb    short loc_D547A
cmp    al, [rbx]
jnb    short loc_D547A
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Dh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D547A:
mov    al, [rbx-77h]
add    al, dl
jb    short loc_D5495
cmp    al, [rbx]
jnb    short loc_D5495
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -78h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5495:
mov    al, [rbx-1]
add    al, dl
jb    short loc_D54B0
cmp    al, [rbx]
jnb    short loc_D54B0
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -2
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D54B0:
mov    al, [rbx-3Bh]
add    al, dl
jb    short loc_D54CB
cmp    al, [rbx]
jnb    short loc_D54CB
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Ch
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D54CB:
mov    al, [rbx+3Bh]
add    al, dl
jb    short loc_D54E6
cmp    al, [rbx]
jnb    short loc_D54E6
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ah
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D54E6:
mov    al, [rbx+3Ch]
add    al, dl
jb    short loc_D5501
cmp    al, [rbx]
jnb    short loc_D5501
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Bh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5501:
mov    al, [rbx+1]
add    al, dl
jb    short loc_D551C
cmp    al, [rbx]
jnb    short loc_D551C
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D551C:
inc    bx
inc    di
inc    di
cmp    ch, 0
jz    short loc_D552D
; cmp    si, 2320
cmp		ofst_movemap, 2320
jns    short loc_D552D
jmp    loc_D5178

loc_D552D:
; mov    si, 0
mov rsi, [rbp+movepath_cost_map]
mov		ofst_movemap, 0
; mov    bx, OVL_Path_Results.Reach_Costs
mov rbx, [rbp+movepath_cost_map]
add rbx, OVL_Path_Results.Reach_Costs
; mov    di, OVL_Path_Results.Reach_From
mov rdi, [rbp+movepath_cost_map]
add rdi, OVL_Path_Results.Reach_From
add    si, CS_Row_Start
mov		cx, CS_Row_Start
add		ofst_movemap, cx
add    bx, CS_Row_Start
add    di, CS_Row_Start
add    di, CS_Row_Start

loc_D554A:
mov    ch, 0
lodsb
inc		ofst_movemap
cmp    al, 0FFh
jnz    short loc_D5554
jmp    loc_D5630
loc_D5554:
inc    ch
mov    dl, al
mov    al, [rbx-1]
add    al, dl
jb    short loc_D5573
cmp    al, [rbx]
jnb    short loc_D5573
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -2
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5573:
mov    al, [rbx-3Ch]
add    al, dl
jb    short loc_D558E
cmp    al, [rbx]
jnb    short loc_D558E
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Dh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D558E:
mov    al, [rbx-3Bh]
add    al, dl
jb    short loc_D55A9
cmp    al, [rbx]
jnb    short loc_D55A9
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, -3Ch
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D55A9:
mov    al, [rbx+3Bh]
add    al, dl
jb    short loc_D55C4
cmp    al, [rbx]
jnb    short loc_D55C4
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ah
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D55C4:
mov    al, [rbx+1]
add    al, dl
jb    short loc_D55DF
cmp    al, [rbx]
jnb    short loc_D55DF
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D55DF:
mov    al, [rbx+77h]
add    al, dl
jb    short loc_D55FA
cmp    al, [rbx]
jnb    short loc_D55FA
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 76h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D55FA:
mov    al, [rbx+3Ch]
add    al, dl
jb    short loc_D5615
cmp    al, [rbx]
jnb    short loc_D5615
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Bh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5615:
mov    al, [rbx+3Dh]
add    al, dl
jb    short loc_D5630
cmp    al, [rbx]
jnb    short loc_D5630
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ch
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5630:
inc    bx
inc    di
inc    di
mov    cl, 29
loc_D5635:
lodsb
inc		ofst_movemap
cmp    al, 0FFh
jnz    short loc_D563D
jmp    loc_D5719
loc_D563D:
inc    ch
mov    dl, al
mov    al, [rbx-3Dh]
add    al, dl
jb    short loc_D565C
cmp    al, [rbx]
jnb    short loc_D565C
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFC2h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D565C:
mov    al, [rbx-3Ch]
add    al, dl
jb    short loc_D5677
cmp    al, [rbx]
jnb    short loc_D5677
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFC3h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5677:
mov    al, [rbx-3Bh]
add    al, dl
jb    short loc_D5692
cmp    al, [rbx]
jnb    short loc_D5692
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFC4h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5692:
mov    al, [rbx-1]
add    al, dl
jb    short loc_D56AD
cmp    al, [rbx]
jnb    short loc_D56AD
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFFEh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D56AD:
mov    al, [rbx+1]
add    al, dl
jb    short loc_D56C8
cmp    al, [rbx]
jnb    short loc_D56C8
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D56C8:
mov    al, [rbx+3Bh]
add    al, dl
jb    short loc_D56E3
cmp    al, [rbx]
jnb    short loc_D56E3
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ah
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D56E3:
mov    al, [rbx+3Ch]
add    al, dl
jb    short loc_D56FE
cmp    al, [rbx]
jnb    short loc_D56FE
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Bh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D56FE:
mov    al, [rbx+3Dh]
add    al, dl
jb    short loc_D5719
cmp    al, [rbx]
jnb    short loc_D5719
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ch
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5719:
inc    bx
inc    di
inc    di
lodsb
inc		ofst_movemap
cmp    al, 0FFh
jnz    short loc_D5724
jmp    loc_D5800
loc_D5724:
inc    ch
mov    dl, al
mov    al, [rbx-3Dh]
add    al, dl
jb    short loc_D5743
cmp    al, [rbx]
jnb    short loc_D5743
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFC2h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5743:
mov    al, [rbx-3Ch]
add    al, dl
jb    short loc_D575E
cmp    al, [rbx]
jnb    short loc_D575E
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFC3h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D575E:
mov    al, [rbx-3Bh]
add    al, dl
jb    short loc_D5779
cmp    al, [rbx]
jnb    short loc_D5779
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFC4h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5779:
mov    al, [rbx-1]
add    al, dl
jb    short loc_D5794
cmp    al, [rbx]
jnb    short loc_D5794
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFFEh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5794:
mov    al, [rbx+1]
add    al, dl
jb    short loc_D57AF
cmp    al, [rbx]
jnb    short loc_D57AF
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D57AF:
mov    al, [rbx+3Bh]
add    al, dl
jb    short loc_D57CA
cmp    al, [rbx]
jnb    short loc_D57CA
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ah
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D57CA:
mov    al, [rbx+3Ch]
add    al, dl
jb    short loc_D57E5
cmp    al, [rbx]
jnb    short loc_D57E5
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Bh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D57E5:
mov    al, [rbx+3Dh]
add    al, dl
jb    short loc_D5800
cmp    al, [rbx]
jnb    short loc_D5800
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ch
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5800:
inc    bx
inc    di
inc    di
dec    cl
jz    short loc_D580A
jmp    loc_D5635
loc_D580A:
lodsb
inc		ofst_movemap
cmp    al, 0FFh
jnz    short loc_D5812
jmp    loc_D58EE
loc_D5812:
inc    ch
mov    dl, al
mov    al, [rbx-3Dh]
add    al, dl
jb    short loc_D5831
cmp    al, [rbx]
jnb    short loc_D5831
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFC2h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5831:
mov    al, [rbx-3Ch]
add    al, dl
jb    short loc_D584C
cmp    al, [rbx]
jnb    short loc_D584C
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFC3h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D584C:
mov    al, [rbx-77h]
add    al, dl
jb    short loc_D5867
cmp    al, [rbx]
jnb    short loc_D5867
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FF88h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5867:
mov    al, [rbx-1]
add    al, dl
jb    short loc_D5882
cmp    al, [rbx]
jnb    short loc_D5882
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFFEh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D5882:
mov    al, [rbx-3Bh]
add    al, dl
jb    short loc_D589D
cmp    al, [rbx]
jnb    short loc_D589D
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0FFC4h
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D589D:
mov    al, [rbx+3Bh]
add    al, dl
jb    short loc_D58B8
cmp    al, [rbx]
jnb    short loc_D58B8
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Ah
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D58B8:
mov    al, [rbx+3Ch]
add    al, dl
jb    short loc_D58D3
cmp    al, [rbx]
jnb    short loc_D58D3
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 3Bh
stosw
dec    di
dec    di
mov    CS_Value_Change, 1
loc_D58D3:
mov    al, [rbx+1]
add    al, dl
jb    short loc_D58EE
cmp    al, [rbx]
jnb    short loc_D58EE
mov    [rbx], al
; mov    ax, si
mov		ax, ofst_movemap
add    ax, 0
stosw
dec    di
dec    di
mov    CS_Value_Change, 1

loc_D58EE:
inc    bx
inc    di
inc    di

sub    si, 120
sub		ofst_movemap, 120
sub    bx, 120
sub    di, 240

cmp    ch, 0
jz    short loc_D5908
; cmp    si, 60
cmp		ofst_movemap, 60
js    short loc_D5908
jmp    loc_D554A

loc_D5908:
cmp    CS_Value_Change, 1
jnz    short @@Done
jmp    loc_D5155
@@Done:



; epilog. restore stack pointer
mov rsp, rbp
pop rbp

ret

Move_Path_Find_ASM ENDP


_TEXT ENDS

END

