/*
    WIZARDS.EXE
    seg001
    seg022

MoO2
    Module: random
        unsigned integer (4 bytes) random_seed
        Random()
        Set_Random_Seed()
        Get_Random_Seed()
        Randomize()

*/

#include "EXIT.h"
#include "random.h"

#include "MOX_BITS.h"
#include "MOX_TYPE.h"



// WZD dseg:7846
// WZD dseg:7846                                                 BEGIN: seg022 - Initialized Data
// WZD dseg:7846
// WZD dseg:7846 68 35 68 35                                     random_seed dd 35683568h                ; DATA XREF: Set_Random_Seed+6w ...
// WZD dseg:784A 01 00 02 00 04 00 08 00 10 00 20 00 40 00 80 00 bit_field_test_bits dw 1, 10b, 100b, 1000b, 10000b, 100000b, 1000000b, 10000000b
// WZD dseg:785A                                                 seg022
// WZD dseg:785A 64 00                                           UU_DBG_OptionBoxColor dw 64h            ; DATA XREF: UU_DBG_SetSelectSetting+Cw ...
// WZD dseg:785C                                                 seg022
// WZD dseg:785C 32 00                                           UU_DBG_UnknownOValue dw 32h             ; DATA XREF: UU_DBG_SetSelectSetting+12w
// WZD dseg:785E                                                 seg022
// WZD dseg:785E 00 00                                           UU_DBG_OptionsFontColor dw 0            ; DATA XREF: UU_DBG_SetSelectSetting+18w ...
// WZD dseg:7860                                                 seg022
// WZD dseg:7860 00 00                                           UU_DBG_OptionsFont dw 0                 ; DATA XREF: UU_DBG_SetSelectSetting+6w ...
// WZD dseg:7862                                                 seg022
// WZD dseg:7862 52 4E 44 20 6E 6F 20 30 27 73                   cnst_RND_Error db 'RND no 0',27h,'s'    ; DATA XREF: Random:loc_1D5B6o
// WZD dseg:786C                                                 seg022
// WZD dseg:786C 00                                              cnst_ZeroString_2 db 0                  ; DATA XREF: UU_DBG_SelectDialog+138o ...
// WZD dseg:786D                                                 seg022
// WZD dseg:786D 4E 4F 5F 48 45 4C 50 00                         UU_cnst_NoHelp db 'NO_HELP',0           ; DATA XREF: UU_DBG_SelectDialog+134o ...
// WZD dseg:7875 00                                              align 2
// WZD dseg:7875
// WZD dseg:7875                                                 END:  seg022 - Initialized Data
// WZD dseg:7875



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
uint16_t random_seed_LO = 0x3568;
uint16_t random_seed_HI = 0x3568;


// WZD dseg:784A 01 00 02 00 04 00 08 00 10 00 20 00 40 00 80 00 Test_Bit_Words dw 1                     ; DATA XREF: MEM_TestBit_Far+1Cr ...
// WZD dseg:784A                                                 dw 10b
// WZD dseg:784A                                                 dw 100b
// WZD dseg:784A                                                 dw 1000b
// WZD dseg:784A                                                 dw 10000b
// WZD dseg:784A                                                 dw 100000b
// WZD dseg:784A                                                 dw 1000000b
// WZD dseg:784A                                                 dw 10000000b

// ...

// WZD dseg:7862 52 4E 44 20 6E 6F 20 30 27 73                   
char cnst_RND_Error[] = "RND no 0's";





/*
    WZD  seg001
*/

// WZD s01p07
/*
drake178: RNG_WeightedPick16()
chooses a random item from a list of 16-bit weighted chances
condensing the weights such that the total fits into a single call of the 9-bit PRNG (max 512),
using repeated divisions by 2 if necessary
*/
// MoO2  Module: KEN  Get_Weighted_Choice_()
int16_t Get_Weighted_Choice(int16_t * weight_array, int16_t weight_count)
{
    int16_t Condense_Loop_Var;
    int16_t Picked_List_Item;
    int16_t Weights_Remainder;
    int16_t itr;
    int16_t tmp_pick;  // _DI_
    int16_t return_value;  // _AX_  DNE in Dasm

    itr = 0;  // ¿ DNE in Dasm ?
    
    do
    {
        tmp_pick = 0;

        tmp_pick += weight_array[itr];

        if(tmp_pick < 512)
        {
            itr++;
        }
        else
        {
            for(Condense_Loop_Var = 0; Condense_Loop_Var < weight_count; Condense_Loop_Var++)
            {
                weight_array[Condense_Loop_Var] = (weight_array[Condense_Loop_Var] / 2);
            }
        }

    } while (itr < weight_count);

    if(tmp_pick == 0)
    {
        return_value = 0;
    }
    else
    {
        Weights_Remainder = (Random(tmp_pick) - weight_array[0]);
        Picked_List_Item = 0;
        while((Weights_Remainder > 0) && (weight_count - 1) > Picked_List_Item)
        {
            Picked_List_Item++;
            Weights_Remainder -= weight_array[Picked_List_Item];
        }
        return_value = Picked_List_Item;
    }

    return return_value;
}


// WZD s01p08
// drake178: RNG_WeightedPick32()
// MoO2  Module: KEN  Get_Weighted_Choice_Long_()


// WZD s01p09
// drake178: UU_RNG_HighestPick16()
// MoO2  Module: KEN  Get_Weighted_Best_Choice_()

// WZD s01p10
// drake178: UU_RNG_HighestPick32()
// MoO2  Module: KEN  Get_Weighted_Choice_Best_Long_()



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
    Clock_Ticks = Read_System_Clock_Timer();
    // MoO2  timer_value = time();
    random_seed = Clock_Ticks;
}


// WZD s22p08
/*
1oom
uint16_t rnd_0_nm1(uint16_t n, uint32_t *seed)
{
    uint32_t r = *seed;
    r ^= (r << 13);
    r ^= (r >> 17);
    r ^= (r << 5);
    *seed = r;
    return (r >> 16) % n;
}
uint16_t rnd_1_n(uint16_t n, uint32_t *seed)
{
    return 1 + rnd_0_nm1(n, seed);
}
*/
// int16_t The_Old_Random(int16_t n)
int16_t Random(int16_t n)
{
    uint16_t result;
    uint32_t r = random_seed;

    r ^= (r << 13);
    r ^= (r >> 17);
    r ^= (r << 5);
    
    random_seed = r;

    result = (r >> 16) % n;

    result += 1;

    return result;
}



uint16_t _AX_ = 0;
uint16_t _BX_ = 0;
uint16_t _CX_ = 0;
uint16_t _DX_ = 0;
// void * _SI_ = NULL;
// void * _DI_ = NULL;
uint16_t _SI_ = 0;
uint16_t _DI_ = 0;
uint16_t _CF_ = 0;
uint16_t _CFI_ = 0;
uint16_t _CFO_ = 0;

#define RCL(_val_) {                            \
    if(_val_ & 0x8000) { _CFO_ = 0x0001; }      \
    _val_ <<= 1;                                \
    if(_CFI_ == 0x0001) { _val_ |= 0x0001; }    \
    _CFI_ = _CFO_;                              \
    _CFO_ = 0x0000;                             \
}

#define RCR(_val_) {                            \
    if(_val_ & 0x0001) { _CFO_ = 0x0001; }      \
    _val_ >>= 1;                                \
    if(_CFI_ == 0x0001) { _val_ |= 0x8000; }    \
    _CFI_ = _CFO_;                              \
    _CFO_ = 0x0000;                             \
}

#define SHL(_val_) {                            \
    if(_val_ & 0x8000) { _CFO_ = 0x0001; }      \
    _val_ <<= 1;                                \
    _CFI_ = _CFO_;                              \
    _CFO_ = 0x0000;                             \
}

#define SHR(_val_) {                            \
    if(_val_ & 0x0001) { _CFO_ = 0x0001; }      \
    _val_ >>= 1;                                \
    _CFI_ = _CFO_;                              \
    _CFO_ = 0x0000;                             \
}

int16_t The_New_Random(int16_t n)
{
    uint16_t result = 0;
    uint16_t return_value = 0;

    if(n == 0)
    {
        Exit_With_Message(cnst_RND_Error);  // "RND no 0's"
    }

    
    result = 0;


// push    si
// push    di


    // // mov     si, [word ptr random_seed]
    // // mov     di, [word ptr random_seed+2]
    // _SI_ = random_seed_LO;
    // _DI_ = random_seed_HI;
    _SI_ = GET_2B_OFS(&random_seed, 0);
    _DI_ = GET_2B_OFS(&random_seed, 2);

    // mov     cx, 9
    _CX_ = 9;

// loc_1D5D2:
do {

    _AX_ = _SI_;    // mov     ax, si
    _BX_ = _SI_;    // mov     bx, si
    _DX_ = _DI_;    // mov     dx, di

    SHR(_DX_);      // shr     dx, 1
    RCR(_BX_);      // rcr     bx, 1
    _AX_ ^= _BX_;   // xor     ax, bx
    SHR(_DX_);      // shr     dx, 1
    RCR(_BX_);      // rcr     bx, 1
    _AX_ ^= _BX_;   // xor     ax, bx
    SHR(_DX_);      // shr     dx, 1
    RCR(_BX_)       // rcr     bx, 1
    SHR(_DX_);      // shr     dx, 1
    RCR(_BX_)       // rcr     bx, 1
    _AX_ ^= _BX_;   // xor     ax, bx
    SHR(_DX_);      // shr     dx, 1
    RCR(_BX_);      // rcr     bx, 1
    SHR(_DX_);      // shr     dx, 1
    RCR(_BX_);      // rcr     bx, 1
    _AX_ ^= _BX_;   // xor     ax, bx
    SHR(_DX_);      // shr     dx, 1
    _AX_ = (_AX_ & 0x00FF) ^ (_DX_ & 0xFF00);   // xor     al, dh
    _DX_ = _AX_;    // mov     dx, ax
    SHR(_DX_);      // shr     dx, 1
    RCL(result);    // rcl     [bp+result], 1
    SHR(_AX_);      // shr     ax, 1
    RCR(_DI_);      // rcr     di, 1
    RCR(_SI_);      // rcr     si, 1

} while (--_CX_ != 0);
// loop    loc_24F9E


// cmp     si, 0
// jnz     short loc_24FE4
// cmp     di, 0
// jnz     short loc_24FE4
// mov     si, 12478
if(
    (_SI_ == 0)
    &&
    (_DI_ == 0)
)
{
    _SI_ = 0x30BE;
}

// loc_1D618:
    // // mov     [word ptr random_seed], si
    // // mov     [word ptr random_seed+2], di
    // random_seed_LO = _SI_;
    // random_seed_HI = _DI_;
    SET_2B_OFS(&random_seed, 0, _SI_);
    SET_2B_OFS(&random_seed, 2, _DI_);


// pop     di
// pop     si


// mov     ax, [bp+result]
// cwd
// idiv    [bp+n]
// inc     dx
// mov     [bp+return_value], dx

    return_value = (( result % n ) + 1);


    return return_value;

}
