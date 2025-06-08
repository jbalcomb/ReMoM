
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



Rotate Through Carry (RCL, RCR) incorporate the carry flag as part of the rotation.  
The carry flag is used as the "carry" for the rotation,  
  moving from the most significant bit to the carry flag (for left rotate)  
  or from the carry flag to the least significant bit (for right rotate).  

## RCL
_DX_ = 10000001;
_DX_ <<= 1;
_DX_ == 00000010; _CX_ == 1;


## RCR  Rotate Carry Right

rcr     bx, 1

shr     ax, 1   if the LSB is set, it's move to CF
rcr     dx, 1   if CF is set, it's move to the MSB, if the LSB is set, it's move to CF
mov     ax, dx


    dx >>= 1;
    if (ax & 1) { dx |= 0x8000; }
    ax = dx;
                    // shr     ax, 1
        dx >>= 1;   // rcr     dx, 1
        if(ax & 1) { dx |= 0x8000; }
        ax = dx;    // mov     ax, dx



## SHL
shl     dx, 1
_DX_ <<= 1;

## SHR
shr     dx, 1
_DX_ >>= 1;

## XOR
xor     ax, bx
_AX_ ^= _BX_;

xor     al, dh
(_AX_ & 0x0F) ^= (_DX_ & 0x0F);






Bit Rotation: A rotation (or circular shift) is an operation similar to shift except that the bits that fall off at one end are put back to the other end. 
In left rotation, the bits that fall off at left end are put back at right end. 
In right rotation, the bits that fall off at right end are put back at left end.

Example: 

Let n is stored using 8 bits. Left rotation of n = 11100101 by 3 makes n = 00101111 (Left shifted by 3 and first 3 bits are put back in last ). If n is stored using 16 bits or 32 bits then left rotation of n (000...11100101) becomes 00..0011100101000. 
Right rotation of n = 11100101 by 3 makes n = 10111100 (Right shifted by 3 and last 3 bits are put back in first ) if n is stored using 8 bits. If n is stored using 16 bits or 32 bits then right rotation of n (000...11100101) by 3 becomes 101000..0011100. 




If by “right rotate” you actually mean the circular right shift of the bits of an unsigned integer, it’s pretty straightforward to implement it in C in terms of right shift, left shift, bitwise AND and bitwise OR, once you know how many bits that unsigned integer is wide.

#include <stdint.h> 
... 
 
uint32_t ror32(uint32_t value) { 
    return ((value & 1) << 31) | (value >> 1); 
} 
The “algorithm” for the simple case of a circular right shift of 1 bit (really, just an expression) is quite straightforward:

take the least significant bit with (value & 1) and shift it left with ( … << 31) so it becomes the most significant bit for that integer size;
shift right the value by one position with (value >> 1) thus discarding the least significant bit;
do a boolean OR of the values obtained at the above points;
The cases where you want to perform a circular right shift of more than 1 bit at once are a little bit trickier, but the general idea is the same: take the least count significant bits, shift them left by size - count and do a bitwise or with the most significant bits shifted right by count.





MoO1
MoM
MoO2



mov     si, [random_seed_LO]
mov     di, [random_seed_HI]
mov     cx, 9
loc_1D5D2:
mov     ax, si                       ; di:si = full register
mov     bx, si                       ; using bit denominators   1...16.17...32,
mov     dx, di                       ; the source bit will be #32, while
shr     dx, 1
rcr     bx, 1
xor     ax, bx                       ; this is bit 31,
shr     dx, 1
rcr     bx, 1
xor     ax, bx                       ; bit 30,
shr     dx, 1
rcr     bx, 1
shr     dx, 1
rcr     bx, 1
xor     ax, bx                       ; bit 28,
shr     dx, 1
rcr     bx, 1
shr     dx, 1
rcr     bx, 1
xor     ax, bx                       ; bit 26,
shr     dx, 1
xor     al, dh                       ; and bit 1
mov     dx, ax
shr     dx, 1
rcl     [bp+result], 1
shr     ax, 1
rcr     di, 1
rcr     si, 1
loop    loc_1D5D2
cmp     si, 0
jnz     short loc_1D618
cmp     di, 0
jnz     short loc_1D618
mov     si, 30BEh
loc_1D618:
mov     [random_seed_LO], si
mov     [random_seed_HI], di





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

