





Test_Bit_Field
Set_Bit_Field
Clear_Bit_Field






e.g.,

¿ 1-bit per World Sqaure ?
2400 world squares / 8 bits per byte = 300 bytes

19 paragraphs, 304 bytes

Far Ptr Visibility_Arcanus

Visibility_Arcanus = SA_MK_FP0(Allocate_Next_Block(World_Data, 19))
Visibility_Myrror = SA_MK_FP0(Allocate_Next_Block(World_Data, 19))


bit_idx = world_y * WORLD_WIDTH + world_x

Set_Bit_Field(bit_idx, Visibility_Arcanus)



bit_idx >> 3

bit_field[]


world_x = 8
world_y = 10
bit_idx = (10 * 60) + 8 = 608
byte_idx = 608 / 8 = 76
byte = bit_field[byte_id]

¿ bit_field_test_bits[ ((bit_idx & 0x07) << 1) ] ?

608d  001001100000b

  001001100000b
& 000000000111b
  =============
  000000000000b
<< 1
  =============
  000000000000b



world_x = 1
world_y = 2
bit_idx = (2 * 60) + 1 = 121
byte_idx = 121 / 8 = 15

121d  01111001b

  0000000001111001b
& 0000000000000111b
  =================
  0000000000000001b
<< 1
  =================
  0000000000000010b

¿ bit_idx 121 is the 2nd bit in the 15th byte ?
~ byte_bit_idx








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

WZD dseg:784A
01 00 02 00 04 00 08 00 10 00 20 00 40 00 80 00
bit_field_test_bits dw 1, 10b, 100b, 1000b, 10000b, 100000b, 1000000b, 10000000b

0x0001
0x0002
0x0004
0x0008
0x0010
0x0020
0x0040
0x0080

00000001b
00000010b
00000100b
00001000b
00010000b
00100000b
01000000b
10000000b




SAR - Shift Arithmetic Right

```
; int __cdecl __far Set_Bit_Field(int bit_idx, char far *bit_field)
proc Set_Bit_Field far
    bit_idx= word ptr  6
    bit_field= dword ptr  8
push    bp
mov     bp, sp
push    si

mov     dx, [bp+bit_idx]
mov     ax, dx
mov     cl, 3
sar     ax, cl

mov     si, ax
les     bx, [bp+bit_field]
add     bx, si
mov     al, [es:bx]

mov     bx, dx
and     bx, 7
shl     bx, 1
or      al, [byte ptr bit_field_test_bits+bx]
les     bx, [bp+bit_field]
add     bx, si
mov     [es:bx], al

pop     si
pop     bp
retf

endp Set_Bit_Field
```
