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
// ¿ MoO2 Clear_Structure_() ? ¿ +/- Clear_Structure_() vs. Copy_Structure_Aligned_() ?
// C:\STU\developp\1oom\src\util.c
// void util_table_remove_item_keep_order(int itemi, void *tbl, int itemsz, int itemnum)
/*
    if ((itemi < 0) || (itemi >= itemnum)) {
        return;
    }
    if (itemi < (itemnum - 1)) {
        memmove(tbl + itemi * itemsz, tbl + (itemi + 1) * itemsz, (itemnum - 1 - itemi) * itemsz);
    }
    memset(tbl + (itemnum - 1) * itemsz, 0, itemsz);
*/
/*

; pulls a record out of a table, shifting what follows downwards, and setting the last one to all zeroes

*/
void Clear_Structure(int16_t record_index, uint8_t * records, int16_t record_size, int16_t record_count)
{

    int16_t shift_count;
    int16_t record_offset;
    uint8_t * dst;
    uint8_t * src;
    uint16_t cnt;

    record_offset = (record_index * record_size);

    shift_count = ((record_count - record_index - 1) * record_size);

    if(shift_count < 0)
    {
        return;
    }

    if(shift_count == 0)
    {
        // mov ax, ds;  mov es, ax;  // assume es:dseg
        // mov ax, [bp+record_count];  dec ax;  mov bx, [bp+record_size];  mul bx;  add ax, [bp+records];  mov di, ax;
        // mov cx, [bp+record_size];  mov al, 0; rep stosb;
        dst = (records + ((record_count - 1) * record_size));
        cnt = record_size;
        while(cnt != 0)
        {
            *dst++ = 0;
            cnt--;
        }
    }
    else
    {
        // TODO  compare these to memset() and memmove() from BCPP30 - probably in-lined
        // ~ source: borlandc\crtl\clib\memcpy.c
        // mov ax, ds;  mov es, ax;  // assume es:dseg
        // mov di, [bp+record_offset];  add di, [bp+records];
        // mov si, di;  add si, [bp+record_size];
        // mov cx, [bp+shift_count];  rep movsb;
        dst = (records + ((record_count - 1) * record_size));
        src = (dst + record_size);
        cnt = shift_count;
        cnt = record_size;
        while(cnt != 0)
        {
            *dst++ = *src++;
            cnt--;
        }
        // mov al, 0;  mov cx, [bp+record_size];  rep stosb;
        cnt = record_size;
        while(cnt != 0)
        {
            *dst++ = 0;
            cnt--;
        }
    }

}


// WZD s22p16
// MoO2  Module: strings  Delete_Structure() |-> Module: struct  Copy_Structure_()
/*
    looks like assembly code; may be MS C; translated 1:1 in MoO2
    
            MEM_PullDynRecord(itr_cities, &_CITIES[0], _cities);
            // pulls a record out of a dynamic table, shifting what follows downwards
            // uses a near address if the segment passed is 0
            // does not zero the last record (which should be invalidated by a table count already)
int RecIndex, char far *Table@, int RecSize, int RecTotal)
*/
void Delete_Structure(int16_t record_num, uint8_t * address, int16_t struct_size, int16_t total_records)
{
    int16_t byte_count;
    int16_t record_offset;
    uint8_t * dst_records; // target record
    uint8_t * src_records; // record after target record
    // src
    // dst
    // src_ofst = (struct_size * record_num) + struct_size
    // dst_ofst = (struct_size * record_num)

    // if(sgmt == 0) { sgmt = DS; )

    // if((record_num + 1) >= total_records) { return 0; }  // check if the record is the last record, because there would be no records to down-shift

    record_offset = (struct_size * record_num);  // offset to the start of the record to be eliminated by the down-shift

    byte_count = (((total_records - record_num) - 1) * struct_size);  // count of byte to move(copy) down

    if(byte_count != 0)
    {
        dst_records = address + record_offset;
        src_records = dst_records + struct_size;

        // ~== Copy_Structure_()
        while(byte_count--)
        {
            *dst_records++ = *src_records++;
        }
    }

}


// WZD s22p17
// drake178: MEM_CopyIfLess_Far()
void Copy_Memory_Less_Than(int8_t * src, int8_t * dst, int16_t n)
{
    int8_t byte;

    if(n != 0)
    {
        while(n--)
        {
            byte = *src++;
            if(*dst >= byte)
            {
                *dst++ = byte;
                dst--;
            }
            dst++;            
        }
    }
}


// WZD s22p18 MEM_Clear_Far()

// WZD s22p19
// MoO2  Module: struct  Set_Memory_()
void Set_Memory(int8_t * s, int16_t n, int16_t c)
{
    if(n != 0) { while(n--) { *s++ = c; } }
}


// WZD s22p20
// TODO  see if there is an example of this is the MS C lib or journal
// ¿ MoO2  struct  Swap_Short() ?
void Swap_Short(int16_t * word1, int16_t * word2)
{
    int16_t tmp_word;

    tmp_word = *word1;
    *word1 = *word2;
    *word2 = tmp_word;
}


// WZD s22p21 MEM_SwapBytes()

// WZD s22p22 _fmemcpy()
// Copy_Memory()

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


// WZD s22p26 MEM_TestBit_Near()
// WZD s22p27 MEM_SetBit_Near()
// WZD s22p28 UU_MEM_ClearBit_Near()
// WZD s22p29 UU_DBG_SetSelectSetting()
// WZD s22p30 UU_DBG_SelectDialog()
// WZD s22p31 RP_VGA_GrowOutFlip()
// WZD s22p32 RP_LBX_IMG_CpyDrawFrame()
