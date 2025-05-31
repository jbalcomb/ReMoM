
~ LFSR (Linear Feedback Shift Register)

RCL
RCR
SHL
SHR
XOR

```asm
mov     al, ah
xor     ah, ah
```
_AX_ >>= 8;
_AX_ &= 0x0F;

¿ what to do with setting _DS_ or _ES_ to _CS_ ?



## RCL


## RCR
// rcr     bx, 1
rcr     bx, 1

// shr     ax, 1
// rcr     dx, 1
// mov     ax, dx
    dx >>= 1;
    if (ax & 1) { dx |= 0x8000; }
    ax = dx;
                    // shr     ax, 1
        dx >>= 1;   // rcr     dx, 1
        if(ax & 1) { dx |= 0x8000; }
        ax = dx;    // mov     ax, dx



## SHL
// shl     dx, 1
shl     dx, 1
_DX_ <<= 1;

## SHR
// shr     dx, 1
shr     dx, 1
_DX_ >>= 1;

## XOR
// xor     ax, bx
xor     ax, bx
_AX_ ^= _BX_;

// xor     al, dh
xor     al, dh
(_AX_ & 0x0F) ^= (_DX_ & 0x0F);





MoO1
MoM
MoO2






1oom
rnd.c
uint16_t rnd_0_nm1(uint16_t n, uint32_t *seed)
{
    uint32_t r = *seed;
    r ^= (r << 13);
    r ^= (r >> 17);
    r ^= (r << 5);
    *seed = r;
    return (r >> 16) % n;
}



// WZD s16p07
// drake178: RNG_Direct_LFSR()
// MoO2  
// 1oom  rnd.c  rnd_bitfiddle()
uint16_t rnd_bitfiddle(uint16_t ax)
{
    int16_t loops = 0;  // _CX_
    uint16_t bx;
    uint16_t dx;

    if (ax == 0) {
        return 0x35c8;
    }

    loops = 8;

// mov     dx, ax
// mov     bx, ax                          ; using 1..16 indexing:
// shr     bx, 1
// xor     ax, bx                          ; bit #15
// shr     bx, 1
// xor     ax, bx                          ; bit #14
// shr     bx, 1
// shr     bx, 1
// xor     ax, bx                          ; bit #12
// shr     bx, 1
// shr     bx, 1
// xor     ax, bx                          ; bit #10
// shr     bx, 1
// xor     al, bh                          ; bit #5
// shr     ax, 1
// rcr     dx, 1
// mov     ax, dx
    do {
        dx = ax;
        bx = ax;
        bx >>= 1;
        ax ^= bx;
        bx >>= 1;
        ax ^= bx;
        bx >>= 2;
        ax ^= bx;
        bx >>= 2;
        ax ^= bx;
        bx >>= 1;
        ax ^= bx;
        dx >>= 1;
        if (ax & 1) { dx |= 0x8000; }
        ax = dx;
    } while (--loops);
    return ax;
}



// WZD s33p19
/*
drake178:
    a secondary linear feedback shift register
    using the same feedback polynomial as RNG_Random,
    also shifted 9 states per call,
    but actually returning the lowest order 10 bits of the resulting state (1-1023)
    rather than the shifted out return value
*/
int16_t RNG_GFX_Random__NOP(int16_t max)
{
    int16_t UU_result = 0;

    return max;

//     if(max == 0)
//     {
//         max = 1;
//     }
// 
//     UU_result = 0;
// 
// mov     si, [cs:Local_LFSR_LO]
// mov     di, [cs:Local_LFSR_HO]
// mov     cx, 9
// 
// 
// loc_24F9E:
// mov     ax, si
// mov     bx, si
// mov     dx, di
// shr     dx, 1
// rcr     bx, 1
// xor     ax, bx
// shr     dx, 1
// rcr     bx, 1
// xor     ax, bx
// shr     dx, 1
// rcr     bx, 1
// shr     dx, 1
// rcr     bx, 1
// xor     ax, bx
// shr     dx, 1
// rcr     bx, 1
// shr     dx, 1
// rcr     bx, 1
// xor     ax, bx
// shr     dx, 1
// xor     al, dh
// mov     dx, ax
// shr     dx, 1
// rcl     [bp+UU_result], 1
// shr     ax, 1
// rcr     di, 1
// rcr     si, 1
// loop    loc_24F9E
// 
// cmp     si, 0
// jnz     short loc_24FE4
// cmp     di, 0
// jnz     short loc_24FE4
// mov     si, 12478
// 
// loc_24FE4:
// mov     [cs:Local_LFSR_LO], si
// mov     [cs:Local_LFSR_HO], di
// mov     ax, si
// and     ax, 1023
// 
// loc_24FF3:
// cmp     ax, [bp+max]
// jb      short @@Done
// sub     ax, [bp+max]
// jmp     short loc_24FF3

}

