/*
    WIZARDS.EXE
    seg022

    MoO2:
        ...
        Module: random
        ...
        Module: struct
        ...
*/

#ifndef MOX_UTIL_H
#define MOX_UTIL_H

#include "MoX_TYPE.H"



// WZD dseg:784A 01 00 02 00 04 00 08 00 10 00 20 00 40 00 80 00 bit_field_test_bits dw 1, 10b, 100b, 1000b, 10000b, 100000b, 1000000b, 10000000b
// WZD dseg:784A                                                                                         ; DATA XREF: Test_Bit_Field_+1Cr ...
extern uint16_t bit_field_test_bits[];



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    WZD seg022
*/


// WZD s22p01 UU_VGA_Bleep()
// WZD s22p02 UU_KBD_GetKey()
// WZD s22p03 UU_Clock_Wait30()
// WZD s22p04 VGA_FlushFrames()
// WZD s22p05 Set_Random_Seed()
// WZD s22p06 Get_Random_Seed()
// WZD s22p07 Randomize()
// WZD s22p08 Random()

// WZD s22p09
// drake178: STR_ToUpper()
// MoO2  Module: strings  String_To_Upper()
void String_To_Upper(char * string);

// WZD s22p10
// drake178: UU_STR_ToLower()
// MoO2  Module: strings  String_To_Lower()

// WZD s22p11
// drake178: RP_MEM_Copy()
// Copy_Memory_Near()

// WZD s22p12 UU_MEM_CopyIfLess()
// WZD s22p13 UU_MEM_Clear_Near()
// WZD s22p14 UU_MEM_Set_Near()
// WZD s22p15 MEM_PullRecord()
// WZD s22p16 MEM_PullDynRecord()

// WZD s22p17
// drake178: MEM_CopyIfLess_Far()
void Copy_Memory_Less_Than(int8_t * src, int8_t * dst, int16_t n);

// WZD s22p18 MEM_Clear_Far()

// WZD s22p19
// drake178: MEM_Set_Far()
// MoO2  Module: struct  Set_Memory_()
void Set_Memory(int8_t * s, int16_t n, int16_t c);

// WZD s22p20
// MoO2  Module: strings  Swap()
// MoO2  Module: strings  Swap_Short()
void MEM_SwapWord(int16_t * word1, int16_t * word2);

// WZD s22p21 MEM_SwapBytes()
// MoO2  Module: strings  Swap_Char()

// WZD s22p22 _fmemcpy()
// Copy_Memory()

// WZD s22p23
int16_t Test_Bit_Field(int16_t bit_idx, uint8_t * bit_field);

// WZD s22p24
void Set_Bit_Field(int16_t bit_idx, uint8_t * bit_field);

// WZD s22p25
void Clear_Bit_Field(int16_t bit_idx, uint8_t * bit_field);

// WZD s22p26 MEM_TestBit_Near()
// WZD s22p27 MEM_SetBit_Near()
// WZD s22p28 UU_MEM_ClearBit_Near()
// WZD s22p29 UU_DBG_SetSelectSetting()
// WZD s22p30 UU_DBG_SelectDialog()
// WZD s22p31 RP_VGA_GrowOutFlip()
// WZD s22p32 RP_LBX_IMG_CpyDrawFrame()



#ifdef __cplusplus
}
#endif

#endif  /* MOX_UTIL_H */
