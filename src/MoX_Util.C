/*
    WIZARDS.EXE
    seg022

    MoO2:
        ...
        Module: random
        ...
        Module: struct
        ...
        Module: strings
        ...
*/

#include "MoX_TYPE.H"
#include "MoX_DEF.H"

// WZD dseg:784A 01 00 02 00 04 00 08 00 10 00 20 00 40 00 80 00 bit_field_test_bits dw 1, 10b, 100b, 1000b, 10000b, 100000b, 1000000b, 10000000b
// WZD dseg:784A
uint16_t bit_field_test_bits[8] = {
    0x0001,
    0x0002,
    0x0004,
    0x0008,
    0x0010,
    0x0020,
    0x0040,
    0x0080
};



/*
    WZD seg022
*/


// WZD s22p09
// drake178: STR_ToUpper()
// MoO2  Module: strings  String_To_Upper()
void String_To_Upper(char * string)
{
    int16_t itr;

    itr = 0;
    while(string[itr] != '\0')
    {
        if(string[itr] >= 'a')
        {
            if(string[itr] <= 'z')
            {
                string[itr] = string[itr] + ('A' - 'a');
            }
        }
        itr++;
    }

}


// WZD s22p23
int16_t Test_Bit_Field(int16_t bit_idx, uint8_t * bit_field)
{
    int16_t bit_is_set;
    int16_t byte_idx;
    int16_t bit_field_byte;
    int16_t byte_bit_idx;

    // mov     dx, [bp+bit_idx]
    // mov     ax, dx
    // mov     cl, 3
    // sar     ax, cl                          ; ¿ AX = bit_idx >> 3 ?

    // Dasm uses SAR - not sure of SAR vs. SHR
    byte_idx = (bit_idx >> 3);  // ~== (bit_idx / 8)

    // les     bx, [bp+bit_field]
    // add     bx, ax
    // mov     al, [es:bx]
    // cbw                                     ; ~ AX = (int16)AL

    bit_field_byte = (int16_t)bit_field[byte_idx];

    // mov     bx, dx
    // and     bx, 111b
    // shl     bx, 1  // ~ sizeof(bit_field_test_bits[0])

    byte_bit_idx = (bit_idx & 0x07);

    // test    [bit_field_test_bits+bx], ax

    if( (bit_field_test_bits[byte_bit_idx] & bit_field_byte) != 0)
    {
        bit_is_set = ST_TRUE;
    }
    else
    {
        bit_is_set = ST_FALSE;
    }

    return bit_is_set;
}

// WZD s22p24
void Set_Bit_Field(int16_t bit_idx, uint8_t * bit_field)
{
    int16_t byte_idx;
    uint16_t bit_field_byte;
    int16_t byte_bit_idx;

    // mov     dx, [bp+bit_idx]
    // mov     ax, dx
    // mov     cl, 3
    // sar     ax, cl                          ; ~ divide by 8 (bits per byte) ; to get index into byte array

    byte_idx = (bit_idx >> 3);

    // mov     si, ax
    // les     bx, [bp+bit_field]
    // add     bx, si
    // mov     al, [es:bx]                     ; AL = bit_field[byte_idx]
    // NOTE: no CBW, as seen in Test_Bit_Field()

    bit_field_byte = (uint16_t)bit_field[byte_idx];

    // mov     bx, dx
    // and     bx, 7
    // shl     bx, 1  // ~ sizeof(bit_field_test_bits[0])

    byte_bit_idx = (bit_idx & 0x07);

    // or      al, [byte ptr bit_field_test_bits+bx]
    // les     bx, [bp+bit_field]
    // add     bx, si
    // mov     [es:bx], al
    bit_field[byte_idx] = (bit_field_byte | bit_field_test_bits[byte_bit_idx]);
}

// WZD s22p25
void Clear_Bit_Field(int16_t bit_idx, uint8_t * bit_field)
{
    int16_t byte_idx;
    int16_t bit_field_byte;
    int16_t byte_bit_idx;

    // mov     si, [bp+bit_idx]
    // mov     ax, si
    // mov     cl, 3
    // sar     ax, cl

    byte_idx = (bit_idx >> 3);

    // mov     di, ax
    // les     bx, [bp+bit_field]
    // add     bx, di
    // mov     al, [es:bx]
    // NOTE: no CBW, as seen in Test_Bit_Field()

    bit_field_byte = (int16_t)bit_field[byte_idx];

    // mov     bx, si
    // and     bx, 7
    // shl     bx, 1  // ~ sizeof(bit_field_test_bits[0])

    byte_bit_idx = (bit_idx & 0x07);

    // mov     dl, 0FFh
    // sub     dl, [byte ptr bit_field_test_bits+bx]
    // and     al, dl
    // les     bx, [bp+bit_field]
    // add     bx, di
    // mov     [es:bx], al

    bit_field[byte_idx] = (bit_field_byte & (0xFF - bit_field_test_bits[byte_bit_idx]));
}