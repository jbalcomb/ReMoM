

SEEALSO:  Rasm-RCL_RCR_SHL_SHR_XOR.md



MoX
Random Number Generator (RNG)
Pseudo Random Number Generator (PRNG)
Linear Feedback Shift Register (LFSR)



MOX/random.C

MOX/FLIC_Draw.C

`grep -I Set_Random_Seed -r`



Cityscape_Window()
    Set_Random_Seed((0x2895 + (city_wx * city_wy)))
    ...deterministic per city







FLIC_Draw.C
// WZD s33p19
RNG_GFX_Random__WIP()
drake178:
    a secondary linear feedback shift register
    using the same feedback polynomial as RNG_Random,
    also shifted 9 states per call,
    but actually returning the lowest order 10 bits of the resulting state (1-1023)
    rather than the shifted out return value

static uint16_t _CS_Local_LFSR_LO = 0x2893;
static uint16_t _CS_Local_LFSR_HI = 0x0A;
...uint32_t 0xA2893  665747  10100010100010010011

int16_t RNG_GFX_Random__WIP(int16_t max)



¿ ~== MOD % ?
`while(!(_AX_ < max)) { _AX_ -= max; }`




## Random()
loc_1D5C0:
mov     [bp+Output_Shift_Bits], 0
push    si
push    di
mov     si, [word ptr random_seed]
mov     di, [word ptr random_seed+2]
mov     cx, 9
@@Loop_Shift_Bits:
mov     ax, si
mov     bx, si
mov     dx, di
shr     dx, 1
rcr     bx, 1
xor     ax, bx
shr     dx, 1
rcr     bx, 1
xor     ax, bx
shr     dx, 1
rcr     bx, 1
shr     dx, 1
rcr     bx, 1
xor     ax, bx
shr     dx, 1
rcr     bx, 1
shr     dx, 1
rcr     bx, 1
xor     ax, bx
shr     dx, 1
xor     al, dh
mov     dx, ax
shr     dx, 1
rcl     [bp+Output_Shift_Bits], 1
shr     ax, 1
rcr     di, 1
rcr     si, 1
loop    @@Loop_Shift_Bits
cmp     si, 0
jnz     short loc_1D618
cmp     di, 0
jnz     short loc_1D618
mov     si, 30BEh                   ; (_SI_ == 0 && _DI_ == 0)
loc_1D618:
mov     [word ptr random_seed], si
mov     [word ptr random_seed+2], di

pop     di
pop     si

mov     ax, [bp+Output_Shift_Bits]
cwd
idiv    [bp+n]
inc     dx                          ; ((Output_Shift_Bits % Max_Value) + 1)
mov     [bp+result], dx
mov     ax, [bp+result]
jmp     short $+2




seg033:09E3 93 28 0A 00                                     
_CS_random_seed dd 0A2893h              

## RNG_GFX_Random__WIP()
loc_24F8C:
mov     [bp+UU_result], 0
mov     si, [cs:_CS_random_seed]
mov     di, [cs:_CS_random_seed+2]
mov     cx, 9
loc_24F9E:
mov     ax, si
mov     bx, si
mov     dx, di
shr     dx, 1
rcr     bx, 1
xor     ax, bx
shr     dx, 1
rcr     bx, 1
xor     ax, bx
shr     dx, 1
rcr     bx, 1
shr     dx, 1
rcr     bx, 1
xor     ax, bx
shr     dx, 1
rcr     bx, 1
shr     dx, 1
rcr     bx, 1
xor     ax, bx
shr     dx, 1
xor     al, dh
mov     dx, ax
shr     dx, 1
rcl     [bp+UU_result], 1
shr     ax, 1
rcr     di, 1
rcr     si, 1
loop    loc_24F9E
cmp     si, 0
jnz     short loc_24FE4
cmp     di, 0
jnz     short loc_24FE4
mov     si, 30BEh
loc_24FE4:
mov     [word ptr cs:_CS_random_seed], si
mov     [word ptr cs:_CS_random_seed+2], di

mov     ax, si
and     ax, 3FFh            ; 001111111111 keep the first ten bits  <= 1023

loc_24FF3:
cmp     ax, [bp+max]
jb      short @@Done
sub     ax, [bp+max]
jmp     short loc_24FF3
@@Done:
