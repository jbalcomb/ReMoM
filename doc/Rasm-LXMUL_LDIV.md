

... LXMUL@ ... LDIV@ ...
¿ ((#3 * #2) / #1) ?

¿ ((#3 * #2) % #1) ?

#2 * #3 / #1

LXMUL@()
LDIV@()

MACRO



AI_Alchemy__WIP()
xor     ax, ax
mov     dx, 10
push    ax
push    dx                              ; divisor
mov     cx, [word ptr bp+Resource_Total+2]
mov     bx, [word ptr bp+Resource_Total]
xor     dx, dx
mov     ax, 7
call    LXMUL@
push    dx
push    ax                              ; dividend
call    LDIV@
mov     [word ptr bp+Desired_Mana+2], dx
mov     [word ptr bp+Desired_Mana], ax

mov     [word ptr bp+Resource_Total], cx
xor     ax, ax
mov     dx, 10
push    ax
push    dx                              ; divisor
mov     cx, [word ptr bp+Resource_Total+2]
mov     bx, [word ptr bp+Resource_Total]
xor     dx, dx
mov     ax, 9
call    LXMUL@
push    dx
push    ax                              ; dividend
call    LDIV@
mov     [word ptr bp+Desired_Gold+2], dx
mov     [word ptr bp+Desired_Gold], ax

Desired_Mana = (Resource_Total * 7 / 10);  // mana = total * .7
Desired_Gold = (Resource_Total - Desired_Mana);
e.g., 100 * 7 / 10 = 70; 100 - 70 = 30;
Desired_Gold = (Resource_Total * 9 / 10);  // gold = total * .9
Desired_Mana = (Resource_Total - Desired_Gold);
e.g., 100 * 9 / 10 = 90; 100 - 90 = 10;

#2 * #3 / #1


Determine_Event()
xor     ax, ax
mov     dx, 100
push    ax
push    dx                              ; divisor
mov     ax, _DI_player_idx
mov     dx, size s_WIZARD
imul    dx
mov     bx, ax
mov     ax, [_players.gold_reserve+bx]
cwd
push    ax
push    dx
mov     ax, 21
push    ax                              ; n
call    Random
pop     cx
cwd
add     ax, 29
adc     dx, 0
pop     cx
pop     bx
call    LXMUL@
push    dx
push    ax                              ; dividend
call    LDIV@
mov     [bp+Conjunction_Active], ax

Conjunction_Active = ((_players[player_idx].gold_reserve * 100) / (29 + Random(21)));

#2 * #1 / #3




WIZ_RecordHistory()
loc_C9936:
mov     ax, [bp+max_curr_magic]
cwd
push    dx
push    ax
mov     ax, _SI_itr
mov     dx, size s_WIZARD
imul    dx
mov     bx, ax
mov     ax, [_players.Astr.Magic_Power+bx]
cwd
push    ax
push    dx
xor     dx, dx
mov     ax, 200
pop     cx
pop     bx
call    LXMUL@
push    dx
push    ax
call    LDIV@
push    ax
mov     ax, _SI_itr
mov     dx, size s_WIZARD
imul    dx
mov     bx, ax
pop     ax
mov     [_players.Astr.Magic_Power+bx], ax

_players[itr_players].Astr.Magic_Power = (max_curr_magic * _players[itr_players].Astr.Magic_Power) / 200);



ITEM_GetViewDrawPos()
mov     ax, _SI_FullAt
cwd
push    dx
push    ax                              ; divisor
mov     ax, [ITEM_ViewGrowStage]
cwd
push    ax
mov     ax, _DI_Draw_Left
sub     ax, [bp+OrigX]
push    dx
cwd
pop     cx
pop     bx
call    LXMUL@
push    dx
push    ax                              ; dividend
call    LDIV@
add     ax, [bp+OrigX]
mov     bx, [bp+Left@]
mov     [bx], ax




...
push    dx
push    ax
...ax
..dx
push    ax
push    dx
...dx
...ax
pop     cx
pop     bx
call    LXMUL@          ; dx:ax = dx:ax * cx:bx
push    dx
push    ax
call    LDIV@           ; ax quotient, dx remainder
mov     [bp+var], ax



### Strategic_Combat()

ovr110:0C94 33 C0                                           xor     ax, ax
ovr110:0C96 BA 64 00                                        mov     dx, 100
ovr110:0C99 50                                              push    ax
ovr110:0C9A 52                                              push    dx                              ; divisor
ovr110:0C9B 8B 46 A4                                        mov     ax, [bp+var_5C]
ovr110:0C9E 99                                              cwd
ovr110:0C9F 50                                              push    ax
ovr110:0CA0 52                                              push    dx
ovr110:0CA1 8B 56 E0                                        mov     dx, [word ptr bp+IDK_ranged_threat_attacker+2]
ovr110:0CA4 8B 46 DE                                        mov     ax, [word ptr bp+IDK_ranged_threat_attacker]
ovr110:0CA7 59                                              pop     cx
ovr110:0CA8 5B                                              pop     bx
ovr110:0CA9 9A FE 05 00 10                                  call    LXMUL@
ovr110:0CAE 52                                              push    dx
ovr110:0CAF 50                                              push    ax                              ; dividend
ovr110:0CB0 9A 34 06 00 10                                  call    LDIV@
ovr110:0CB5 01 46 9C                                        add     [bp+var_64], ax
...
AX = 0; DX = 100; push AX; push DX;
    ¿ 'push' is for push/pop or for passing parameters on the stack ?
AX = var_5C; AX:DX = AX; push AX; push DX;
    sign-extending *cast* of 2-byte value to 4-byte value
    ¿ 'push' is for push/pop or for passing parameters on the stack ?
AX:DX = IDK_ranged_threat_attacker
pop CX; pop BX;
    ¿ popping previous push of AX:DX = var_5C ?
LXMUL()
    AX:DX = AX:DX * BX:CX
        ¿ AX:DX is IDK_ranged_threat_attacker ?
        ¿ BX:CX is var_5C ?
LDIV()
    AX:DX = AX:DX / BX:CX
        ¿ AX:DX is (IDK_ranged_threat_attacker * var_5C) ?
    ...
        push divisor, push dividend
...
¿ LXMUL() uses registers, LDIV() uses stack ?
so, ...
    pushes 100 on to the stack
    pushes var_5C on to the stack
    sets AX:DX to IDK_ranged_threat_attacker
    pops var_5C off from the stack
    sets BX:CX to var_5C
    LXMUL() uses AX:DX and BX:CX
    pushes the return value of LXMUL() on to the stack  (quotient)
    LDIV() uses stack  ```mov bp,sp; mov ax,10[bp]; mov dx,12[bp]; mov bx,14[bp]; mov cx,16[bp];```





attacker_mana_multiplied = ((10 * _players[_combat_attacker_player].mana_reserve) / Combat_Casting_Cost_Multiplier(_combat_attacker_player));


ovr110:0767                                                 loc_89017:                              ; CODE XREF: Strategic_Combat__STUB+699j
ovr110:0767 FF 36 86 C5                                     push    [_combat_attacker_player]       ; Player_Index
ovr110:076B 9A 25 00 3A 35                                  call    j_Combat_Casting_Cost_Multiplier
ovr110:0770 59                                              pop     cx
ovr110:0771 99                                              cwd
ovr110:0772 52                                              push    dx
ovr110:0773 50                                              push    ax                              ; divisor
ovr110:0774 A1 86 C5                                        mov     ax, [_combat_attacker_player]
ovr110:0777 BA C8 04                                        mov     dx, size s_WIZARD
ovr110:077A F7 EA                                           imul    dx
ovr110:077C 8B D8                                           mov     bx, ax
ovr110:077E 8B 87 26 A1                                     mov     ax, [_players.mana_reserve+bx]
ovr110:0782 99                                              cwd
ovr110:0783 50                                              push    ax
ovr110:0784 52                                              push    dx
ovr110:0785 33 D2                                           xor     dx, dx
ovr110:0787 B8 0A 00                                        mov     ax, 10                          ; 30/10 ~== 300%/100% ~== 3x
ovr110:078A 59                                              pop     cx
ovr110:078B 5B                                              pop     bx
ovr110:078C 9A FE 05 00 10                                  call    LXMUL@
ovr110:0791 52                                              push    dx
ovr110:0792 50                                              push    ax                              ; dividend
ovr110:0793 9A 34 06 00 10                                  call    LDIV@
ovr110:0798 89 46 B8                                        mov     [bp+var_48], ax
ovr110:0798
ovr110:0798                                                 ((#1 * #2) / #3)


