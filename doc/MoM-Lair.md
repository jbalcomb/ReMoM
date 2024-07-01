



Lair_Combat()
    |-> Do_Lair_Confirm()
        |-> Lair_Confirm()
            |-> Lair_Confirm_Draw()
    |-> Lair_Combat_Do()
        |-> Combat()
    |-> Lair_Clear()



ovr083:0164
Lair_Combat_STUB()

ovr083:01B9 8B C6                                           mov     ax, _SI_lair_idx
ovr083:01BB BA 18 00                                        mov     dx, size s_LAIR
ovr083:01BE F7 EA                                           imul    dx
ovr083:01C0 8B 16 C0 9C                                     mov     dx, [word ptr LAIRS]
ovr083:01C4 03 D0                                           add     dx, ax
ovr083:01C6 83 C2 0F                                        add     dx, s_LAIR.Misc_Flags
ovr083:01C9 FF 36 C2 9C                                     push    [word ptr LAIRS+2]
ovr083:01CD 52                                              push    dx                              ; bit_field
ovr083:01CE B8 01 00                                        mov     ax, 1
ovr083:01D1 50                                              push    ax                              ; bit_idx
ovr083:01D2 9A 01 05 4D 1D                                  call    Set_Bit_Field                   ; sets the specified bit of a target far bitfield
ovr083:01D7 83 C4 06                                        add     sp, 6



ovr083:0164
Lair_Combat_STUB()

ovr083:026D 8B C6                                           mov     ax, _SI_lair_idx
ovr083:026F BA 18 00                                        mov     dx, size s_LAIR
ovr083:0272 F7 EA                                           imul    dx
ovr083:0274 8B 16 C0 9C                                     mov     dx, [word ptr LAIRS]
ovr083:0278 03 D0                                           add     dx, ax
ovr083:027A 83 C2 0F                                        add     dx, s_LAIR.Misc_Flags
ovr083:027D FF 36 C2 9C                                     push    [word ptr LAIRS+2]
ovr083:0281 52                                              push    dx                              ; bit_field
ovr083:0282 B8 02 00                                        mov     ax, 2
ovr083:0285 50                                              push    ax                              ; bit_idx
ovr083:0286 9A 01 05 4D 1D                                  call    Set_Bit_Field                   ; sets the specified bit of a target far bitfield



// WZD s22p24
void Set_Bit_Field(int16_t bit_idx, uint8_t * bit_field)

...calculate address+offset to _LAIRS.Misc_Flags
...pass far pointer / segment and offset
...pass bit index of 1


MoX_Data.C
Line 1710
// WZD dseg:9CC0
// drake178: TBL_Encounters
// AKA TBL_Lairs
struct s_LAIR * _LAIRS;

MoX_Data.H
Line 1003
// sizeof:  18h  24d
#pragma pack(push)
#pragma pack(2)
struct s_LAIR
{
    /* 00 */  int8_t   wx;
    /* 01 */  int8_t   wy;
    /* 02 */  int8_t   wp;
    /* 03 */  int8_t   Intact;  // {F,T} ... set to false by Lair_Clear() ... Draw_Map_Lairs() only *draws* if true
    /* 04 */  int8_t   Type;    // {0,...,} ... Draw_Map_Lairs() only *draws* if > 3 {0,...3} are tower and nodes
    /* 05 */  uint8_t  guard1_unit_type;
    /* 06 */  uint8_t  guard1_count;       // ¿ code only checks low-nibble for count != 0 ?
    /* 07 */  uint8_t  guard2_unit_type;
    /* 08 */  uint8_t  guard2_count;       // ¿ code only checks low-nibble for count != 0 ?
    /* 09 */  int8_t   Unused_09h;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 0A */  int16_t  Loot_Gold;
    /* 0C */  int16_t  Loot_Mana;
    /* 0E */  int8_t   Spell_n_Special;
    /* 0F */  uint8_t  Misc_Flags; // 7: 6: 5: 4: 3: 2: ¿ attacked ? 1: ¿ visited ? 0: ¿ ?
    /* 10 */  int8_t   Item_Count;
    /* 11 */  int8_t   Unused_11h;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 12 */  int16_t  Item_Values[3];
    /* 18 */
};
#pragma pack(pop)

Misc_Flags
7 6 5 4 3 2 1 0
| | | | | | | ¿ ?
| | | | | | ¿ explorer / viewed / visited ?
| | | | | ¿ attacked (does not include win/lose info) ?
| | | | ¿ ?
| | | ¿ ?
| | ¿ ?
| ¿ ?
¿ ?





s_LAIR.Def1_Had_n_Left

ovr083:04CE
Do_Lair_Confirm

ovr083:056E 8B C7                                           mov     ax, _DI_lair_idx
ovr083:0570 BA 18 00                                        mov     dx, size s_LAIR
ovr083:0573 F7 EA                                           imul    dx
ovr083:0575 C4 1E C0 9C                                     les     bx, [LAIRS]
ovr083:0579 03 D8                                           add     bx, ax
ovr083:057B 26 8A 47 06                                     mov     al, [es:bx+s_LAIR.Def1_Had_n_Left]
ovr083:057F B4 00                                           mov     ah, 0
ovr083:0581 A9 0F 00                                        test    ax, 0Fh



s_LAIR.Defender1_Index

ovr083:04CE
Do_Lair_Confirm

ovr083:059D 8B C7                                           mov     ax, _DI_lair_idx
ovr083:059F BA 18 00                                        mov     dx, size s_LAIR
ovr083:05A2 F7 EA                                           imul    dx
ovr083:05A4 C4 1E C0 9C                                     les     bx, [LAIRS]
ovr083:05A8 03 D8                                           add     bx, ax
ovr083:05AA 26 8A 47 05                                     mov     al, [es:bx+s_LAIR.Defender1_Index]
ovr083:05AE B4 00                                           mov     ah, 0
ovr083:05B0 BA 24 00                                        mov     dx, size s_UNIT_TYPE
ovr083:05B3 F7 EA                                           imul    dx
ovr083:05B5 8B D8                                           mov     bx, ax
ovr083:05B7 80 BF B1 01 01                                  cmp     [_unit_type_table.Figures+bx], 1



s_LAIR.Type is signed 1 byte

ovr083:04CE
Do_Lair_Confirm

ovr083:04E8 8B C7                                           mov     ax, _DI_lair_idx
ovr083:04EA BA 18 00                                        mov     dx, size s_LAIR
ovr083:04ED F7 EA                                           imul    dx
ovr083:04EF C4 1E C0 9C                                     les     bx, [LAIRS]
ovr083:04F3 03 D8                                           add     bx, ax
ovr083:04F5 26 8A 47 04                                     mov     al, [es:bx+s_LAIR.Type]
ovr083:04F9 98                                              cbw
ovr083:04FA 8B F0                                           mov     _SI_lair_type, ax
