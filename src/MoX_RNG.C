/*
    WIZARDS.EXE
    seg022

    MoO2  Module: random
        unsigned integer (4 bytes) random_seed
        Random()
        Set_Random_Seed()
        Get_Random_Seed()
        Randomize()

*/
#include "MoX_RNG.H"

#include "MoX_TYPE.H"

#include "MoM_PFL.H"  /* Get_System_Clock_Counter() */


/*
    ¿ "seed" vs. "feedback polynomial" vs. "taps" ?

*/
/*
    LFSR Bits
        initial:  0011010101101000
        Warp Node: 9B 9F 0F 00  0F9F9Bh  1023899d  11111001111110011011b
        ¿ Clock Ticks ?
*/
/*
3568
13672
0011010101101000

35683568
896021864
00110101011010000011010101101000

Per drake178
01101010000000000000000000000001
6A000001
1778384897

01101010
6A
106

*/
/*
    System Clock Counter

    incremented approximately 18.206 times per second
*/


// WZD dseg:7846  68 35  LFSR_LO_bits dw 3568h
// WZD dseg:7848  68 35  LFSR_HI_bits dw 3568h
// uint16_t LFSR_LO_bits = 0x3568;
// uint16_t LFSR_HI_bits = 0x3568;
// WZD dseg:7846  68 35 68 35
uint32_t random_seed = 0x35683568;

// WZD dseg:784A 01 00 02 00 04 00 08 00 10 00 20 00 40 00 80 00 Test_Bit_Words dw 1                     ; DATA XREF: MEM_TestBit_Far+1Cr ...
// WZD dseg:784A                                                 dw 10b
// WZD dseg:784A                                                 dw 100b
// WZD dseg:784A                                                 dw 1000b
// WZD dseg:784A                                                 dw 10000b
// WZD dseg:784A                                                 dw 100000b
// WZD dseg:784A                                                 dw 1000000b
// WZD dseg:784A                                                 dw 10000000b

// ...

// WZD dseg:7862 52 4E 44 20 6E 6F 20 30 27 73                   cnst_RND_Error db 'RND no 0',27h,'s'    ; DATA XREF: RNG_Random+Eo






/*
    WZD  seg022
*/

// WZD s22p05
void Set_Random_Seed(uint32_t n)
{
    random_seed = n;
}


// WZD s22p06
uint32_t Get_Random_Seed(void)
{
    return random_seed;
}


// WZD s22p07
void Randomize(void)
{
    uint32_t Clock_Ticks;
    Clock_Ticks = Get_System_Clock_Counter();
    // MoO2  timer_value = time();
    random_seed = Clock_Ticks;
}


// WZD s22p08
int16_t Random(int16_t max)
{
    int16_t itr;
    int16_t result;

    // The number of bits in the shift register.
    #define N 16

    // The initial state of the shift register.
    uint16_t state = 0xACE1;

    // The feedback polynomial.
    // uint16_t feedback = 0xB400;
    uint16_t feedback = 0x6A00;

    // The output bit.
    uint16_t output;

    // The next state of the shift register.
    uint16_t next_state;


    // Initialize the shift register.
    // state = 0xACE1;
    state = random_seed;


    if(max == 0)
    {
        // GAME_QuitProgram(cnst_RND_Error);  /* "RND no 0's" */
        // MoO2  return 1;
    }

    output = 0;

    // Loop through 8-bits
    itr = 9;
    while(itr--)
    {
        // Generate the output bit.
        output = state & 1;

        // Shift the shift register.
        next_state = (state >> 1) ^ (output & feedback);

        // Update the state of the shift register.
        state = next_state;

    }

    result = output % max;

    return result;
}



/*
http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html

/ * 16-bit xorshift PRNG * /

unsigned xs = 1;

unsigned xorshift( )
{
    xs ^= xs << 7;
    xs ^= xs >> 9;
    xs ^= xs << 8;
    return xs;
}
*/

/*
; returns a random number between 1 and Max_Value
; uses a 32bit linear feedback shift register read 9
; bits at a time (Max_Value shouldn't exceed 512)
; Attributes: bp-based frame

; int __cdecl __far RNG_Random(int Max_Value)
proc RNG_Random far

Output_Shift_Bits= word ptr -4
Return_Value= word ptr -2
Max_Value= word ptr  6

push    bp
mov     bp, sp
sub     sp, 4
push    si
push    di


cmp     [bp+Max_Value], 0
jnz     short loc_1D5C0
mov     ax, offset cnst_RND_Error       ; "RND no 0's"
push    ax                              ; Exit_Message@
call    GAME_QuitProgram                ; shuts down the mouse, sound, and EMM systems, returns
                                        ; the VGA to text mode, and quits the game with the
                                        ; passed message - using a direct DOS interrupt rather
                                        ; than the standard library exit functions

loc_1D5C0:
mov     [bp+Output_Shift_Bits], 0
push    si
push    di
mov     si, [LFSR_LO_bits]
mov     di, [LFSR_HI_bits]
mov     cx, 9

loc_1D5D2:
mov     ax, si                          ; di:si = full register
mov     bx, si                          ; using bit denominators   1...16.17...32,
mov     dx, di                          ; the source bit will be #32, while

shr     dx, 1
rcr     bx, 1
xor     ax, bx                          ; this is bit 31,

shr     dx, 1
rcr     bx, 1
xor     ax, bx                          ; bit 30,

shr     dx, 1
rcr     bx, 1
shr     dx, 1
rcr     bx, 1
xor     ax, bx                          ; bit 28,
shr     dx, 1
rcr     bx, 1
shr     dx, 1
rcr     bx, 1
xor     ax, bx                          ; bit 26,
shr     dx, 1
xor     al, dh                          ; and bit 1
mov     dx, ax
shr     dx, 1
rcl     [bp+Output_Shift_Bits], 1
shr     ax, 1
rcr     di, 1
rcr     si, 1

loop    loc_1D5D2                       ; di:si = full register
cmp     si, 0
jnz     short loc_1D618
cmp     di, 0
jnz     short loc_1D618
mov     si, 30BEh
loc_1D618:                              ; CODE XREF: RNG_Random+66j ...
mov     [LFSR_LO_bits], si
mov     [LFSR_HI_bits], di
pop     di
pop     si
mov     ax, [bp+Output_Shift_Bits]
cwd
idiv    [bp+Max_Value]
inc     dx
mov     [bp+Return_Value], dx
mov     ax, [bp+Return_Value]
*/
