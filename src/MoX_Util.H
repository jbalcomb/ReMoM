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

// WZD s22p09
// drake178: STR_ToUpper()
// MoO2  Module: strings  String_To_Upper()
void String_To_Upper(char * string);

// WZD s22p10
// drake178: UU_STR_ToLower()
// MoO2  Module: strings  String_To_Lower()

// WZD s22p23
int16_t Test_Bit_Field(int16_t bit_idx, uint8_t * bit_field);

// WZD s22p24
void Set_Bit_Field(int16_t bit_idx, uint8_t * bit_field);

// WZD s22p25
void Clear_Bit_Field(int16_t bit_idx, uint8_t * bit_field);



#ifdef __cplusplus
}
#endif

#endif  /* MOX_UTIL_H */
