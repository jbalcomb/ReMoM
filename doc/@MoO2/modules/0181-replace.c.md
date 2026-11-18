# F:\WLIBS\animate\replace.c

Module 181 of the debug info of ORION2.EXE.

Ranges: 1:0x0012719F (linear 0x0013719F), 4,582 bytes, 2:0x00002F48 (linear 0x00172F48), 76 bytes, 2:0x0004489C (linear 0x001B489C), 2,052 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0012719F (linear 0x0013719F) | code | Replace_Color_ | `regions/07-object-1/001BC654-001BCE53.txt` |
| 1:0x0012743E (linear 0x0013743E) | code | Replace_Color_All_ | `regions/07-object-1/001BC654-001BCE53.txt` |
| 1:0x001276CF (linear 0x001376CF) | code | No_Replace_Mask_Msg_ | `regions/07-object-1/001BC654-001BCE53.txt` |
| 1:0x001276F8 (linear 0x001376F8) | code | Replace_Color_Gradient_ | `regions/07-object-1/001BC654-001BCE53.txt` |
| 1:0x00127A03 (linear 0x00137A03) | code | Replace_Gradient_Range_ | `regions/07-object-1/001BCE54-001BD653.txt` |
| 1:0x00127D86 (linear 0x00137D86) | code | Replace_Color_Range_ | `regions/07-object-1/001BCE54-001BD653.txt` |
| 1:0x001280E0 (linear 0x001380E0) | code | Establish_Color_Gradient_ | `regions/07-object-1/001BD654-001BDE53.txt` |
| 1:0x001281B5 (linear 0x001381B5) | code | Shift_Palette_Range_ | `regions/07-object-1/001BD654-001BDE53.txt` |
| 1:0x00128229 (linear 0x00138229) | code | Gray_Scale_Off_Page_ | `regions/07-object-1/001BD654-001BDE53.txt` |
| 1:0x001282BF (linear 0x001382BF) | code | Interlaced_Gray_Scale_Off_Page_ | `regions/07-object-1/001BD654-001BDE53.txt` |
| 2:0x0004489C (linear 0x001B489C) | data | _graphics_buffer | - |
| 2:0x0004509C (linear 0x001B509C) | data | _palette_color_list | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x0004489C (linear 0x001B489C) | graphics_buffer | unsigned char[2048] |
| 2:0x0004509C (linear 0x001B509C) | palette_color_list | unsigned char * |

#### Replace_Color

- Address: 1:0x0012719F (linear 0x0013719F), 671 bytes, in `regions/07-object-1/001BC654-001BCE53.txt`
- Type: void (void *, short, short)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| picture | [EBP-0x88] | void * |
| color_to_replace | [EBP-0x30] | short |
| replacement_color | [EBP-0x2C] | short |
| palette_num | [EBP-0x7C] | int |
| start | [EBP-0x78] | int |
| count | [EBP-0x74] | int |
| palette_pos | [EBP-0x70] | int |
| pict_size | [EBP-0x6C] | int |
| frame_num | [EBP-0x14] | short |
| current_palette_long | [EBP-0x64] | long * |
| palette_data | [EBP-0x84] | long |
| frame_offset | [EBP-0x80] | char * |
| frame_offset_table | [EBP-0x58] | long * |
| bitmap_size | [EBP-0x54] | int |
| pos | [EBP-0x60] | int |
| screen_pos | [EBP-0x5C] | int |
| line_add | [EBP-0x48] | int |
| i | [EBP-0x44] | int |
| height | [EBP-0x20] | short |
| width | [EBP-0x1C] | short |
| data | [EBP-0x4] | unsigned char |
| screen_data | [EBP-0x8] | unsigned char |
| x | [EBP-0x50] | int |
| y | [EBP-0x4C] | int |
| buffer_pos_word | [EBP-0x38] | int |
| buffer_pos | [EBP-0x68] | int |
| packet_pos | [EBP-0x40] | int |
| data_count | [EBP-0x10] | short |
| skip_count | [EBP-0x18] | short |
| store_type | [EBP-0x28] | short |
| line_skip | [EBP-0x24] | short |
| frame_data_word | [EBP-0x3C] | unsigned short * |
| bank_num | [EBP-0xC] | short |
| frame_data | [EBP-0x34] | char * |

#### Replace_Color_All

- Address: 1:0x0012743E (linear 0x0013743E), 657 bytes, in `regions/07-object-1/001BC654-001BCE53.txt`
- Type: void (void *, short)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| picture | [EBP-0x84] | void * |
| replacement_color | [EBP-0x2C] | short |
| palette_num | [EBP-0x7C] | int |
| start | [EBP-0x78] | int |
| count | [EBP-0x74] | int |
| palette_pos | [EBP-0x70] | int |
| pict_size | [EBP-0x6C] | int |
| frame_num | [EBP-0x14] | short |
| current_palette_long | [EBP-0x64] | long * |
| palette_data | [EBP-0x60] | long |
| frame_offset | [EBP-0x80] | char * |
| frame_offset_table | [EBP-0x58] | long * |
| bitmap_size | [EBP-0x54] | int |
| pos | [EBP-0x50] | int |
| screen_pos | [EBP-0x5C] | int |
| line_add | [EBP-0x48] | int |
| i | [EBP-0x44] | int |
| height | [EBP-0x20] | short |
| width | [EBP-0x1C] | short |
| data | [EBP-0x4] | unsigned char |
| screen_data | [EBP-0x8] | unsigned char |
| x | [EBP-0x40] | int |
| y | [EBP-0x4C] | int |
| buffer_pos_word | [EBP-0x68] | int |
| buffer_pos | [EBP-0x34] | int |
| packet_pos | [EBP-0x30] | int |
| data_count | [EBP-0x10] | short |
| skip_count | [EBP-0x18] | short |
| store_type | [EBP-0x28] | short |
| line_skip | [EBP-0x24] | short |
| frame_data_word | [EBP-0x3C] | unsigned short * |
| bank_num | [EBP-0xC] | short |
| frame_data | [EBP-0x38] | char * |

#### No_Replace_Mask_Msg

- Address: 1:0x001276CF (linear 0x001376CF), 41 bytes, in `regions/07-object-1/001BC654-001BCE53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Replace_Color_Gradient

- Address: 1:0x001276F8 (linear 0x001376F8), 779 bytes, in `regions/07-object-1/001BC654-001BCE53.txt`
- Type: void (void *, char *, short, short)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| picture | [EBP-0x8C] | void * |
| color_array | [EBP-0x88] | char * |
| color_count | [EBP-0x30] | short |
| reverse_flag | [EBP-0x2C] | short |
| palette_num | [EBP-0x7C] | int |
| start | [EBP-0x78] | int |
| count | [EBP-0x74] | int |
| palette_pos | [EBP-0x70] | int |
| pict_size | [EBP-0x6C] | int |
| frame_num | [EBP-0x14] | short |
| current_palette_long | [EBP-0x64] | long * |
| palette_data | [EBP-0x84] | long |
| frame_offset | [EBP-0x80] | char * |
| frame_offset_table | [EBP-0x58] | long * |
| bitmap_size | [EBP-0x54] | int |
| pos | [EBP-0x60] | int |
| screen_pos | [EBP-0x5C] | int |
| line_add | [EBP-0x48] | int |
| i | [EBP-0x44] | int |
| height | [EBP-0x20] | short |
| width | [EBP-0x1C] | short |
| data | [EBP-0x4] | unsigned char |
| screen_data | [EBP-0x8] | unsigned char |
| x | [EBP-0x50] | int |
| y | [EBP-0x4C] | int |
| buffer_pos_word | [EBP-0x38] | int |
| buffer_pos | [EBP-0x68] | int |
| packet_pos | [EBP-0x40] | int |
| data_count | [EBP-0x10] | short |
| skip_count | [EBP-0x18] | short |
| store_type | [EBP-0x28] | short |
| line_skip | [EBP-0x24] | short |
| frame_data_word | [EBP-0x3C] | unsigned short * |
| bank_num | [EBP-0xC] | short |
| frame_data | [EBP-0x34] | char * |

#### Replace_Gradient_Range

- Address: 1:0x00127A03 (linear 0x00137A03), 899 bytes, in `regions/07-object-1/001BCE54-001BD653.txt`
- Type: void (void *, char *, short, short, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| picture | [EBP-0x90] | void * |
| color_array | [EBP-0x8C] | char * |
| color_count | [EBP-0x2C] | short |
| reverse_flag | [EBP-0x28] | short |
| min_color | [EBP+0x10] | short |
| max_color | [EBP+0x14] | short |
| palette_num | [EBP-0x78] | int |
| start | [EBP-0x74] | int |
| count | [EBP-0x70] | int |
| palette_pos | [EBP-0x6C] | int |
| pict_size | [EBP-0x68] | int |
| frame_num | [EBP-0x30] | short |
| color_pos | [EBP-0x34] | short |
| current_palette_long | [EBP-0x80] | long * |
| palette_data | [EBP-0x58] | long |
| frame_offset | [EBP-0x7C] | char * |
| frame_offset_table | [EBP-0x64] | long * |
| bitmap_size | [EBP-0x60] | int |
| pos | [EBP-0x48] | int |
| screen_pos | [EBP-0x44] | int |
| line_add | [EBP-0x88] | int |
| i | [EBP-0x84] | int |
| height | [EBP-0x24] | short |
| width | [EBP-0x20] | short |
| data | [EBP-0x8] | unsigned char |
| screen_data | [EBP-0x4] | unsigned char |
| x | [EBP-0x54] | int |
| y | [EBP-0x4C] | int |
| buffer_pos_word | [EBP-0x38] | int |
| buffer_pos | [EBP-0x3C] | int |
| packet_pos | [EBP-0x40] | int |
| data_count | [EBP-0x18] | short |
| skip_count | [EBP-0x14] | short |
| store_type | [EBP-0x10] | short |
| line_skip | [EBP-0xC] | short |
| frame_data_word | [EBP-0x5C] | unsigned short * |
| bank_num | [EBP-0x1C] | short |
| frame_data | [EBP-0x50] | char * |

#### Replace_Color_Range

- Address: 1:0x00127D86 (linear 0x00137D86), 858 bytes, in `regions/07-object-1/001BCE54-001BD653.txt`
- Type: void (void *, char *, short, short)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| picture | [EBP-0x94] | void * |
| color_array | [EBP-0x90] | char * |
| start_color | [EBP-0x34] | short |
| color_count | [EBP-0x2C] | short |
| palette_num | [EBP-0x84] | int |
| start | [EBP-0x80] | int |
| count | [EBP-0x7C] | int |
| palette_pos | [EBP-0x78] | int |
| pict_size | [EBP-0x74] | int |
| frame_num | [EBP-0x28] | short |
| color_pos | [EBP-0x24] | short |
| current_palette_long | [EBP-0x68] | long * |
| palette_data | [EBP-0x8C] | long |
| frame_offset | [EBP-0x88] | char * |
| frame_offset_table | [EBP-0x5C] | long * |
| bitmap_size | [EBP-0x58] | int |
| pos | [EBP-0x54] | int |
| screen_pos | [EBP-0x64] | int |
| line_add | [EBP-0x4C] | int |
| i | [EBP-0x48] | int |
| height | [EBP-0x30] | short |
| width | [EBP-0x18] | short |
| data | [EBP-0x8] | unsigned char |
| screen_data | [EBP-0x4] | unsigned char |
| x | [EBP-0x3C] | int |
| y | [EBP-0x50] | int |
| buffer_pos_word | [EBP-0x40] | int |
| buffer_pos | [EBP-0x70] | int |
| packet_pos | [EBP-0x6C] | int |
| data_count | [EBP-0x20] | short |
| skip_count | [EBP-0x1C] | short |
| store_type | [EBP-0x10] | short |
| line_skip | [EBP-0x38] | short |
| frame_data_word | [EBP-0x60] | unsigned short * |
| bank_num | [EBP-0xC] | short |
| end_color | [EBP-0x14] | short |
| frame_data | [EBP-0x44] | char * |

#### Establish_Color_Gradient

- Address: 1:0x001280E0 (linear 0x001380E0), 213 bytes, in `regions/07-object-1/001BD654-001BDE53.txt`
- Type: void (short, short)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| color_count | [EBP-0x8] | short |
| reverse_flag | [EBP-0xC] | short |
| i | [EBP-0x10] | int |
| color_value | [EBP-0x4] | char |

#### Shift_Palette_Range

- Address: 1:0x001281B5 (linear 0x001381B5), 116 bytes, in `regions/07-object-1/001BD654-001BDE53.txt`
- Type: void (void *, int)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| picture | [EBP-0x20] | void * |
| new_start | [EBP-0x1C] | int |
| palette_num | [EBP-0x18] | int |
| start | [EBP-0x14] | int |
| count | [EBP-0x10] | int |
| palette_pos | [EBP-0xC] | int |
| current_palette_long | [EBP-0x8] | long * |
| palette_data | [EBP-0x4] | long |

#### Gray_Scale_Off_Page

- Address: 1:0x00128229 (linear 0x00138229), 150 bytes, in `regions/07-object-1/001BD654-001BDE53.txt`
- Type: void (short, short, short)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| start_color | [EBP-0xC] | short |
| color_count | [EBP-0x4] | short |
| inverse_flag | [EBP-0x8] | short |
| i | [EBP-0x10] | int |

#### Interlaced_Gray_Scale_Off_Page

- Address: 1:0x001282BF (linear 0x001382BF), 198 bytes, in `regions/07-object-1/001BD654-001BDE53.txt`
- Type: void (short, short, short, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| start_color1 | [EBP-0xC] | short |
| color_count1 | [EBP-0x10] | short |
| start_color2 | [EBP-0x8] | short |
| color_count2 | [EBP-0x4] | short |
| inverse_flag | [EBP+0x10] | short |
| i | [EBP-0x14] | int |

#### Types

| # | Kind | Name | Details |
|---|---|---|---|
| - | cue table |  | source file table (C++) |
| 1 | scope | struct |  |
| 2 | scope | union |  |
| 3 | scope | enum |  |
| 4 | scalar | signed char | int 1 bytes |
| 5 | scalar | unsigned char | unsigned 1 bytes |
| 6 | scalar | short | int 2 bytes |
| 7 | scalar | unsigned short | unsigned 2 bytes |
| 8 | scalar | int | int 4 bytes |
| 9 | scalar | unsigned int | unsigned 4 bytes |
| 10 | typedef | char | name for unsigned char (#5) |
| 11 | typedef | char | name for unsigned char (#5) |
| 12 | typedef | BYTE | name for unsigned char (#5) |
| 13 | typedef | SBYTE | name for signed char (#4) |
| 14 | typedef | UBYTE | name for unsigned char (#5) |
| 15 | typedef | WORD | name for unsigned short (#7) |
| 16 | typedef | SWORD | name for short (#6) |
| 17 | typedef | UWORD | name for unsigned short (#7) |
| 18 | typedef | DWORD | name for unsigned int (#9) |
| 19 | typedef | SDWORD | name for int (#8) |
| 20 | typedef | UDWORD | name for unsigned int (#9) |
| 21 | typedef | BOOL | name for unsigned char (#5) |
| 22 | typedef | SampleHandle | name for int (#8) |
| 23 | typedef | size_t | name for unsigned int (#9) |
| 24 | pointer |  | near386; of char (#10) |
| 25 | array | __va_list | 0..0; of char * (#24) |
| 26 | typedef | __va_list | name for char *[1] (#25) |
| 27 | scalar | long | int 4 bytes |
| 28 | typedef | fpos_t | name for long (#27) |
| 29 | typedef | fpos_t | name for long (#27) |
| 30 | array |  | 0..2047; of unsigned char (#5) |
| 31 | pointer |  | near386; of unsigned char (#5) |
| 32 | scalar | void | void 1 bytes |
| 33 | pointer |  | near386; of void (#32) |
| 34 | procedure |  | near386; returns void (void *, short, short) |
| 35 | pointer |  | near386; of long (#27) |
| 36 | pointer |  | near386; of unsigned short (#7) |
| 37 | procedure |  | near386; returns void (void *, short) |
| 38 | procedure |  | near386; returns void () |
| 39 | procedure |  | near386; returns void (void *, char *, short, short) |
| 40 | procedure |  | near386; returns void (void *, char *, short, short, short, short) |
| 41 | procedure |  | near386; returns void (short, short) |
| 42 | procedure |  | near386; returns void (void *, int) |
| 43 | procedure |  | near386; returns void (short, short, short) |
| 44 | procedure |  | near386; returns void (short, short, short, short, short) |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 25 | 1:0x0012719F (linear 0x0013719F) | 0x001BC7F3 |
| 42 | 1:0x001271C1 (linear 0x001371C1) | 0x001BC815 |
| 44 | 1:0x001271CC (linear 0x001371CC) | 0x001BC820 |
| 46 | 1:0x001271FF (linear 0x001371FF) | 0x001BC853 |
| 48 | 1:0x0012720A (linear 0x0013720A) | 0x001BC85E |
| 51 | 1:0x00127221 (linear 0x00137221) | 0x001BC875 |
| 53 | 1:0x00127250 (linear 0x00137250) | 0x001BC8A4 |
| 54 | 1:0x0012725C (linear 0x0013725C) | 0x001BC8B0 |
| 55 | 1:0x00127262 (linear 0x00137262) | 0x001BC8B6 |
| 56 | 1:0x0012726B (linear 0x0013726B) | 0x001BC8BF |
| 57 | 1:0x00127277 (linear 0x00137277) | 0x001BC8CB |
| 59 | 1:0x0012727E (linear 0x0013727E) | 0x001BC8D2 |
| 61 | 1:0x0012728B (linear 0x0013728B) | 0x001BC8DF |
| 62 | 1:0x00127299 (linear 0x00137299) | 0x001BC8ED |
| 64 | 1:0x001272A8 (linear 0x001372A8) | 0x001BC8FC |
| 66 | 1:0x001272AC (linear 0x001372AC) | 0x001BC900 |
| 67 | 1:0x001272BA (linear 0x001372BA) | 0x001BC90E |
| 69 | 1:0x001272BC (linear 0x001372BC) | 0x001BC910 |
| 70 | 1:0x001272C5 (linear 0x001372C5) | 0x001BC919 |
| 72 | 1:0x001272CC (linear 0x001372CC) | 0x001BC920 |
| 74 | 1:0x001272D5 (linear 0x001372D5) | 0x001BC929 |
| 75 | 1:0x001272E5 (linear 0x001372E5) | 0x001BC939 |
| 77 | 1:0x001272F0 (linear 0x001372F0) | 0x001BC944 |
| 78 | 1:0x001272F3 (linear 0x001372F3) | 0x001BC947 |
| 79 | 1:0x001272F6 (linear 0x001372F6) | 0x001BC94A |
| 80 | 1:0x001272F8 (linear 0x001372F8) | 0x001BC94C |
| 82 | 1:0x00127309 (linear 0x00137309) | 0x001BC95D |
| 83 | 1:0x0012730E (linear 0x0013730E) | 0x001BC962 |
| 85 | 1:0x00127313 (linear 0x00137313) | 0x001BC967 |
| 88 | 1:0x0012732B (linear 0x0013732B) | 0x001BC97F |
| 89 | 1:0x00127344 (linear 0x00137344) | 0x001BC998 |
| 90 | 1:0x00127351 (linear 0x00137351) | 0x001BC9A5 |
| 91 | 1:0x0012735E (linear 0x0013735E) | 0x001BC9B2 |
| 94 | 1:0x00127363 (linear 0x00137363) | 0x001BC9B7 |
| 96 | 1:0x0012736A (linear 0x0013736A) | 0x001BC9BE |
| 98 | 1:0x00127387 (linear 0x00137387) | 0x001BC9DB |
| 99 | 1:0x00127390 (linear 0x00137390) | 0x001BC9E4 |
| 100 | 1:0x00127399 (linear 0x00137399) | 0x001BC9ED |
| 102 | 1:0x001273A3 (linear 0x001373A3) | 0x001BC9F7 |
| 104 | 1:0x001273A7 (linear 0x001373A7) | 0x001BC9FB |
| 106 | 1:0x001273B9 (linear 0x001373B9) | 0x001BCA0D |
| 108 | 1:0x001273C2 (linear 0x001373C2) | 0x001BCA16 |
| 109 | 1:0x001273C6 (linear 0x001373C6) | 0x001BCA1A |
| 110 | 1:0x001273E0 (linear 0x001373E0) | 0x001BCA34 |
| 111 | 1:0x001273F0 (linear 0x001373F0) | 0x001BCA44 |
| 113 | 1:0x001273FD (linear 0x001373FD) | 0x001BCA51 |
| 115 | 1:0x00127404 (linear 0x00137404) | 0x001BCA58 |
| 116 | 1:0x0012740D (linear 0x0013740D) | 0x001BCA61 |
| 117 | 1:0x00127416 (linear 0x00137416) | 0x001BCA6A |
| 119 | 1:0x00127420 (linear 0x00137420) | 0x001BCA74 |
| 120 | 1:0x00127424 (linear 0x00137424) | 0x001BCA78 |
| 121 | 1:0x00127426 (linear 0x00137426) | 0x001BCA7A |
| 122 | 1:0x0012742B (linear 0x0013742B) | 0x001BCA7F |
| 124 | 1:0x0012742D (linear 0x0013742D) | 0x001BCA81 |
| 126 | 1:0x00127432 (linear 0x00137432) | 0x001BCA86 |
| 127 | 1:0x00127437 (linear 0x00137437) | 0x001BCA8B |
| 130 | 1:0x0012743E (linear 0x0013743E) | 0x001BCA92 |
| 148 | 1:0x0012745E (linear 0x0013745E) | 0x001BCAB2 |
| 150 | 1:0x00127469 (linear 0x00137469) | 0x001BCABD |
| 152 | 1:0x0012749F (linear 0x0013749F) | 0x001BCAF3 |
| 154 | 1:0x001274AA (linear 0x001374AA) | 0x001BCAFE |
| 157 | 1:0x001274C1 (linear 0x001374C1) | 0x001BCB15 |
| 159 | 1:0x001274F0 (linear 0x001374F0) | 0x001BCB44 |
| 160 | 1:0x001274FC (linear 0x001374FC) | 0x001BCB50 |
| 161 | 1:0x00127502 (linear 0x00137502) | 0x001BCB56 |
| 162 | 1:0x0012750B (linear 0x0013750B) | 0x001BCB5F |
| 163 | 1:0x00127517 (linear 0x00137517) | 0x001BCB6B |
| 165 | 1:0x0012751E (linear 0x0013751E) | 0x001BCB72 |
| 167 | 1:0x0012752B (linear 0x0013752B) | 0x001BCB7F |
| 168 | 1:0x00127539 (linear 0x00137539) | 0x001BCB8D |
| 170 | 1:0x00127548 (linear 0x00137548) | 0x001BCB9C |
| 172 | 1:0x0012754C (linear 0x0013754C) | 0x001BCBA0 |
| 173 | 1:0x0012755A (linear 0x0013755A) | 0x001BCBAE |
| 175 | 1:0x0012755C (linear 0x0013755C) | 0x001BCBB0 |
| 176 | 1:0x00127565 (linear 0x00137565) | 0x001BCBB9 |
| 178 | 1:0x0012756C (linear 0x0013756C) | 0x001BCBC0 |
| 180 | 1:0x00127575 (linear 0x00137575) | 0x001BCBC9 |
| 181 | 1:0x00127580 (linear 0x00137580) | 0x001BCBD4 |
| 183 | 1:0x0012758B (linear 0x0013758B) | 0x001BCBDF |
| 184 | 1:0x0012758E (linear 0x0013758E) | 0x001BCBE2 |
| 185 | 1:0x00127591 (linear 0x00137591) | 0x001BCBE5 |
| 186 | 1:0x00127593 (linear 0x00137593) | 0x001BCBE7 |
| 188 | 1:0x001275A4 (linear 0x001375A4) | 0x001BCBF8 |
| 189 | 1:0x001275A9 (linear 0x001375A9) | 0x001BCBFD |
| 191 | 1:0x001275AE (linear 0x001375AE) | 0x001BCC02 |
| 193 | 1:0x001275C6 (linear 0x001375C6) | 0x001BCC1A |
| 194 | 1:0x001275DF (linear 0x001375DF) | 0x001BCC33 |
| 195 | 1:0x001275EA (linear 0x001375EA) | 0x001BCC3E |
| 196 | 1:0x001275F7 (linear 0x001375F7) | 0x001BCC4B |
| 199 | 1:0x001275FC (linear 0x001375FC) | 0x001BCC50 |
| 201 | 1:0x00127603 (linear 0x00137603) | 0x001BCC57 |
| 203 | 1:0x00127620 (linear 0x00137620) | 0x001BCC74 |
| 204 | 1:0x00127629 (linear 0x00137629) | 0x001BCC7D |
| 205 | 1:0x00127632 (linear 0x00137632) | 0x001BCC86 |
| 207 | 1:0x0012763C (linear 0x0013763C) | 0x001BCC90 |
| 209 | 1:0x00127640 (linear 0x00137640) | 0x001BCC94 |
| 211 | 1:0x0012764E (linear 0x0013764E) | 0x001BCCA2 |
| 213 | 1:0x00127657 (linear 0x00137657) | 0x001BCCAB |
| 214 | 1:0x0012765B (linear 0x0013765B) | 0x001BCCAF |
| 215 | 1:0x00127675 (linear 0x00137675) | 0x001BCCC9 |
| 216 | 1:0x00127680 (linear 0x00137680) | 0x001BCCD4 |
| 218 | 1:0x0012768D (linear 0x0013768D) | 0x001BCCE1 |
| 220 | 1:0x00127694 (linear 0x00137694) | 0x001BCCE8 |
| 221 | 1:0x0012769D (linear 0x0013769D) | 0x001BCCF1 |
| 222 | 1:0x001276A6 (linear 0x001376A6) | 0x001BCCFA |
| 224 | 1:0x001276B0 (linear 0x001376B0) | 0x001BCD04 |
| 225 | 1:0x001276B4 (linear 0x001376B4) | 0x001BCD08 |
| 226 | 1:0x001276B6 (linear 0x001376B6) | 0x001BCD0A |
| 227 | 1:0x001276BB (linear 0x001376BB) | 0x001BCD0F |
| 229 | 1:0x001276BD (linear 0x001376BD) | 0x001BCD11 |
| 231 | 1:0x001276C2 (linear 0x001376C2) | 0x001BCD16 |
| 232 | 1:0x001276C7 (linear 0x001376C7) | 0x001BCD1B |
| 235 | 1:0x001276CF (linear 0x001376CF) | 0x001BCD23 |
| 237 | 1:0x001276E7 (linear 0x001376E7) | 0x001BCD3B |
| 238 | 1:0x001276F1 (linear 0x001376F1) | 0x001BCD45 |
| 242 | 1:0x001276F8 (linear 0x001376F8) | 0x001BCD4C |
| 259 | 1:0x0012771F (linear 0x0013771F) | 0x001BCD73 |
| 261 | 1:0x0012772A (linear 0x0013772A) | 0x001BCD7E |
| 263 | 1:0x00127737 (linear 0x00137737) | 0x001BCD8B |
| 265 | 1:0x0012776B (linear 0x0013776B) | 0x001BCDBF |
| 267 | 1:0x00127776 (linear 0x00137776) | 0x001BCDCA |
| 270 | 1:0x0012778D (linear 0x0013778D) | 0x001BCDE1 |
| 272 | 1:0x001277BC (linear 0x001377BC) | 0x001BCE10 |
| 273 | 1:0x001277C8 (linear 0x001377C8) | 0x001BCE1C |
| 274 | 1:0x001277CE (linear 0x001377CE) | 0x001BCE22 |
| 275 | 1:0x001277D7 (linear 0x001377D7) | 0x001BCE2B |
| 276 | 1:0x001277E3 (linear 0x001377E3) | 0x001BCE37 |
| 278 | 1:0x001277EA (linear 0x001377EA) | 0x001BCE3E |
| 280 | 1:0x001277F7 (linear 0x001377F7) | 0x001BCE4B |
| 281 | 1:0x00127805 (linear 0x00137805) | 0x001BCE59 |
| 283 | 1:0x00127814 (linear 0x00137814) | 0x001BCE68 |
| 285 | 1:0x00127818 (linear 0x00137818) | 0x001BCE6C |
| 286 | 1:0x00127826 (linear 0x00137826) | 0x001BCE7A |
| 288 | 1:0x00127828 (linear 0x00137828) | 0x001BCE7C |
| 289 | 1:0x00127831 (linear 0x00137831) | 0x001BCE85 |
| 291 | 1:0x00127838 (linear 0x00137838) | 0x001BCE8C |
| 293 | 1:0x00127841 (linear 0x00137841) | 0x001BCE95 |
| 294 | 1:0x0012784C (linear 0x0013784C) | 0x001BCEA0 |
| 296 | 1:0x00127878 (linear 0x00137878) | 0x001BCECC |
| 297 | 1:0x0012787B (linear 0x0013787B) | 0x001BCECF |
| 298 | 1:0x0012787E (linear 0x0013787E) | 0x001BCED2 |
| 299 | 1:0x00127880 (linear 0x00137880) | 0x001BCED4 |
| 301 | 1:0x00127891 (linear 0x00137891) | 0x001BCEE5 |
| 302 | 1:0x00127896 (linear 0x00137896) | 0x001BCEEA |
| 304 | 1:0x0012789B (linear 0x0013789B) | 0x001BCEEF |
| 307 | 1:0x001278B3 (linear 0x001378B3) | 0x001BCF07 |
| 308 | 1:0x001278CC (linear 0x001378CC) | 0x001BCF20 |
| 309 | 1:0x001278D7 (linear 0x001378D7) | 0x001BCF2B |
| 310 | 1:0x00127905 (linear 0x00137905) | 0x001BCF59 |
| 313 | 1:0x0012790A (linear 0x0013790A) | 0x001BCF5E |
| 315 | 1:0x00127911 (linear 0x00137911) | 0x001BCF65 |
| 317 | 1:0x0012792E (linear 0x0013792E) | 0x001BCF82 |
| 318 | 1:0x00127937 (linear 0x00137937) | 0x001BCF8B |
| 319 | 1:0x00127940 (linear 0x00137940) | 0x001BCF94 |
| 321 | 1:0x0012794A (linear 0x0013794A) | 0x001BCF9E |
| 323 | 1:0x0012794E (linear 0x0013794E) | 0x001BCFA2 |
| 325 | 1:0x00127960 (linear 0x00137960) | 0x001BCFB4 |
| 327 | 1:0x00127969 (linear 0x00137969) | 0x001BCFBD |
| 328 | 1:0x0012796D (linear 0x0013796D) | 0x001BCFC1 |
| 329 | 1:0x00127987 (linear 0x00137987) | 0x001BCFDB |
| 330 | 1:0x00127992 (linear 0x00137992) | 0x001BCFE6 |
| 332 | 1:0x001279C0 (linear 0x001379C0) | 0x001BD014 |
| 334 | 1:0x001279C7 (linear 0x001379C7) | 0x001BD01B |
| 335 | 1:0x001279D0 (linear 0x001379D0) | 0x001BD024 |
| 336 | 1:0x001279D9 (linear 0x001379D9) | 0x001BD02D |
| 338 | 1:0x001279E3 (linear 0x001379E3) | 0x001BD037 |
| 339 | 1:0x001279E7 (linear 0x001379E7) | 0x001BD03B |
| 340 | 1:0x001279EC (linear 0x001379EC) | 0x001BD040 |
| 341 | 1:0x001279F1 (linear 0x001379F1) | 0x001BD045 |
| 343 | 1:0x001279F3 (linear 0x001379F3) | 0x001BD047 |
| 345 | 1:0x001279F8 (linear 0x001379F8) | 0x001BD04C |
| 346 | 1:0x001279FD (linear 0x001379FD) | 0x001BD051 |
| 350 | 1:0x00127A03 (linear 0x00137A03) | 0x001BD057 |
| 367 | 1:0x00127A2A (linear 0x00137A2A) | 0x001BD07E |
| 369 | 1:0x00127A35 (linear 0x00137A35) | 0x001BD089 |
| 371 | 1:0x00127A42 (linear 0x00137A42) | 0x001BD096 |
| 373 | 1:0x00127A77 (linear 0x00137A77) | 0x001BD0CB |
| 375 | 1:0x00127A82 (linear 0x00137A82) | 0x001BD0D6 |
| 378 | 1:0x00127A99 (linear 0x00137A99) | 0x001BD0ED |
| 380 | 1:0x00127AC8 (linear 0x00137AC8) | 0x001BD11C |
| 381 | 1:0x00127AD4 (linear 0x00137AD4) | 0x001BD128 |
| 382 | 1:0x00127ADA (linear 0x00137ADA) | 0x001BD12E |
| 383 | 1:0x00127AE3 (linear 0x00137AE3) | 0x001BD137 |
| 384 | 1:0x00127AEF (linear 0x00137AEF) | 0x001BD143 |
| 386 | 1:0x00127AF6 (linear 0x00137AF6) | 0x001BD14A |
| 388 | 1:0x00127B03 (linear 0x00137B03) | 0x001BD157 |
| 389 | 1:0x00127B11 (linear 0x00137B11) | 0x001BD165 |
| 391 | 1:0x00127B20 (linear 0x00137B20) | 0x001BD174 |
| 393 | 1:0x00127B24 (linear 0x00137B24) | 0x001BD178 |
| 394 | 1:0x00127B32 (linear 0x00137B32) | 0x001BD186 |
| 396 | 1:0x00127B37 (linear 0x00137B37) | 0x001BD18B |
| 397 | 1:0x00127B40 (linear 0x00137B40) | 0x001BD194 |
| 399 | 1:0x00127B47 (linear 0x00137B47) | 0x001BD19B |
| 401 | 1:0x00127B50 (linear 0x00137B50) | 0x001BD1A4 |
| 403 | 1:0x00127B5B (linear 0x00137B5B) | 0x001BD1AF |
| 404 | 1:0x00127B77 (linear 0x00137B77) | 0x001BD1CB |
| 405 | 1:0x00127B8B (linear 0x00137B8B) | 0x001BD1DF |
| 407 | 1:0x00127B9F (linear 0x00137B9F) | 0x001BD1F3 |
| 408 | 1:0x00127BA2 (linear 0x00137BA2) | 0x001BD1F6 |
| 409 | 1:0x00127BA5 (linear 0x00137BA5) | 0x001BD1F9 |
| 410 | 1:0x00127BA7 (linear 0x00137BA7) | 0x001BD1FB |
| 412 | 1:0x00127BB8 (linear 0x00137BB8) | 0x001BD20C |
| 413 | 1:0x00127BBD (linear 0x00137BBD) | 0x001BD211 |
| 415 | 1:0x00127BC2 (linear 0x00137BC2) | 0x001BD216 |
| 418 | 1:0x00127BDA (linear 0x00137BDA) | 0x001BD22E |
| 419 | 1:0x00127BFF (linear 0x00137BFF) | 0x001BD253 |
| 421 | 1:0x00127C0D (linear 0x00137C0D) | 0x001BD261 |
| 422 | 1:0x00127C2C (linear 0x00137C2C) | 0x001BD280 |
| 423 | 1:0x00127C40 (linear 0x00137C40) | 0x001BD294 |
| 424 | 1:0x00127C57 (linear 0x00137C57) | 0x001BD2AB |
| 425 | 1:0x00127C59 (linear 0x00137C59) | 0x001BD2AD |
| 428 | 1:0x00127C5E (linear 0x00137C5E) | 0x001BD2B2 |
| 430 | 1:0x00127C65 (linear 0x00137C65) | 0x001BD2B9 |
| 432 | 1:0x00127C82 (linear 0x00137C82) | 0x001BD2D6 |
| 433 | 1:0x00127C8B (linear 0x00137C8B) | 0x001BD2DF |
| 434 | 1:0x00127C94 (linear 0x00137C94) | 0x001BD2E8 |
| 436 | 1:0x00127C9E (linear 0x00137C9E) | 0x001BD2F2 |
| 438 | 1:0x00127CA2 (linear 0x00137CA2) | 0x001BD2F6 |
| 440 | 1:0x00127CB4 (linear 0x00137CB4) | 0x001BD308 |
| 442 | 1:0x00127CBD (linear 0x00137CBD) | 0x001BD311 |
| 443 | 1:0x00127CC1 (linear 0x00137CC1) | 0x001BD315 |
| 444 | 1:0x00127CE7 (linear 0x00137CE7) | 0x001BD33B |
| 446 | 1:0x00127CF5 (linear 0x00137CF5) | 0x001BD349 |
| 447 | 1:0x00127D14 (linear 0x00137D14) | 0x001BD368 |
| 448 | 1:0x00127D28 (linear 0x00137D28) | 0x001BD37C |
| 449 | 1:0x00127D3F (linear 0x00137D3F) | 0x001BD393 |
| 451 | 1:0x00127D41 (linear 0x00137D41) | 0x001BD395 |
| 453 | 1:0x00127D48 (linear 0x00137D48) | 0x001BD39C |
| 454 | 1:0x00127D51 (linear 0x00137D51) | 0x001BD3A5 |
| 455 | 1:0x00127D5A (linear 0x00137D5A) | 0x001BD3AE |
| 457 | 1:0x00127D64 (linear 0x00137D64) | 0x001BD3B8 |
| 458 | 1:0x00127D68 (linear 0x00137D68) | 0x001BD3BC |
| 459 | 1:0x00127D6D (linear 0x00137D6D) | 0x001BD3C1 |
| 461 | 1:0x00127D72 (linear 0x00137D72) | 0x001BD3C6 |
| 463 | 1:0x00127D74 (linear 0x00137D74) | 0x001BD3C8 |
| 465 | 1:0x00127D79 (linear 0x00137D79) | 0x001BD3CD |
| 466 | 1:0x00127D7E (linear 0x00137D7E) | 0x001BD3D2 |
| 470 | 1:0x00127D86 (linear 0x00137D86) | 0x001BD3DA |
| 487 | 1:0x00127DAD (linear 0x00137DAD) | 0x001BD401 |
| 488 | 1:0x00127DB7 (linear 0x00137DB7) | 0x001BD40B |
| 490 | 1:0x00127DC2 (linear 0x00137DC2) | 0x001BD416 |
| 492 | 1:0x00127DF7 (linear 0x00137DF7) | 0x001BD44B |
| 494 | 1:0x00127E02 (linear 0x00137E02) | 0x001BD456 |
| 497 | 1:0x00127E1C (linear 0x00137E1C) | 0x001BD470 |
| 499 | 1:0x00127E4B (linear 0x00137E4B) | 0x001BD49F |
| 500 | 1:0x00127E57 (linear 0x00137E57) | 0x001BD4AB |
| 501 | 1:0x00127E60 (linear 0x00137E60) | 0x001BD4B4 |
| 502 | 1:0x00127E69 (linear 0x00137E69) | 0x001BD4BD |
| 503 | 1:0x00127E75 (linear 0x00137E75) | 0x001BD4C9 |
| 505 | 1:0x00127E7C (linear 0x00137E7C) | 0x001BD4D0 |
| 507 | 1:0x00127E89 (linear 0x00137E89) | 0x001BD4DD |
| 508 | 1:0x00127E97 (linear 0x00137E97) | 0x001BD4EB |
| 510 | 1:0x00127EA6 (linear 0x00137EA6) | 0x001BD4FA |
| 512 | 1:0x00127EAA (linear 0x00137EAA) | 0x001BD4FE |
| 513 | 1:0x00127EB8 (linear 0x00137EB8) | 0x001BD50C |
| 515 | 1:0x00127EBD (linear 0x00137EBD) | 0x001BD511 |
| 516 | 1:0x00127EC6 (linear 0x00137EC6) | 0x001BD51A |
| 518 | 1:0x00127ECD (linear 0x00137ECD) | 0x001BD521 |
| 520 | 1:0x00127ED6 (linear 0x00137ED6) | 0x001BD52A |
| 522 | 1:0x00127EE4 (linear 0x00137EE4) | 0x001BD538 |
| 523 | 1:0x00127EF4 (linear 0x00137EF4) | 0x001BD548 |
| 524 | 1:0x00127EFD (linear 0x00137EFD) | 0x001BD551 |
| 525 | 1:0x00127F06 (linear 0x00137F06) | 0x001BD55A |
| 527 | 1:0x00127F25 (linear 0x00137F25) | 0x001BD579 |
| 528 | 1:0x00127F28 (linear 0x00137F28) | 0x001BD57C |
| 529 | 1:0x00127F2B (linear 0x00137F2B) | 0x001BD57F |
| 530 | 1:0x00127F2D (linear 0x00137F2D) | 0x001BD581 |
| 532 | 1:0x00127F3E (linear 0x00137F3E) | 0x001BD592 |
| 533 | 1:0x00127F43 (linear 0x00137F43) | 0x001BD597 |
| 535 | 1:0x00127F48 (linear 0x00137F48) | 0x001BD59C |
| 538 | 1:0x00127F60 (linear 0x00137F60) | 0x001BD5B4 |
| 539 | 1:0x00127F79 (linear 0x00137F79) | 0x001BD5CD |
| 541 | 1:0x00127F87 (linear 0x00137F87) | 0x001BD5DB |
| 542 | 1:0x00127F97 (linear 0x00137F97) | 0x001BD5EB |
| 543 | 1:0x00127FA0 (linear 0x00137FA0) | 0x001BD5F4 |
| 544 | 1:0x00127FA9 (linear 0x00137FA9) | 0x001BD5FD |
| 545 | 1:0x00127FC8 (linear 0x00137FC8) | 0x001BD61C |
| 546 | 1:0x00127FCA (linear 0x00137FCA) | 0x001BD61E |
| 549 | 1:0x00127FCF (linear 0x00137FCF) | 0x001BD623 |
| 551 | 1:0x00127FD6 (linear 0x00137FD6) | 0x001BD62A |
| 553 | 1:0x00127FF3 (linear 0x00137FF3) | 0x001BD647 |
| 554 | 1:0x00127FFF (linear 0x00137FFF) | 0x001BD653 |
| 555 | 1:0x00128008 (linear 0x00138008) | 0x001BD65C |
| 557 | 1:0x00128012 (linear 0x00138012) | 0x001BD666 |
| 559 | 1:0x00128016 (linear 0x00138016) | 0x001BD66A |
| 561 | 1:0x00128028 (linear 0x00138028) | 0x001BD67C |
| 563 | 1:0x00128034 (linear 0x00138034) | 0x001BD688 |
| 564 | 1:0x00128038 (linear 0x00138038) | 0x001BD68C |
| 565 | 1:0x00128052 (linear 0x00138052) | 0x001BD6A6 |
| 567 | 1:0x0012805D (linear 0x0013805D) | 0x001BD6B1 |
| 568 | 1:0x0012806A (linear 0x0013806A) | 0x001BD6BE |
| 569 | 1:0x00128073 (linear 0x00138073) | 0x001BD6C7 |
| 570 | 1:0x0012807C (linear 0x0013807C) | 0x001BD6D0 |
| 571 | 1:0x00128098 (linear 0x00138098) | 0x001BD6EC |
| 573 | 1:0x0012809A (linear 0x0013809A) | 0x001BD6EE |
| 575 | 1:0x001280A1 (linear 0x001380A1) | 0x001BD6F5 |
| 576 | 1:0x001280AD (linear 0x001380AD) | 0x001BD701 |
| 577 | 1:0x001280B6 (linear 0x001380B6) | 0x001BD70A |
| 579 | 1:0x001280C0 (linear 0x001380C0) | 0x001BD714 |
| 580 | 1:0x001280C4 (linear 0x001380C4) | 0x001BD718 |
| 581 | 1:0x001280C9 (linear 0x001380C9) | 0x001BD71D |
| 583 | 1:0x001280CE (linear 0x001380CE) | 0x001BD722 |
| 585 | 1:0x001280D0 (linear 0x001380D0) | 0x001BD724 |
| 587 | 1:0x001280D5 (linear 0x001380D5) | 0x001BD729 |
| 588 | 1:0x001280DA (linear 0x001380DA) | 0x001BD72E |
| 591 | 1:0x001280E0 (linear 0x001380E0) | 0x001BD734 |
| 596 | 1:0x001280FD (linear 0x001380FD) | 0x001BD751 |
| 598 | 1:0x00128107 (linear 0x00138107) | 0x001BD75B |
| 600 | 1:0x00128124 (linear 0x00138124) | 0x001BD778 |
| 604 | 1:0x00128162 (linear 0x00138162) | 0x001BD7B6 |
| 606 | 1:0x00128186 (linear 0x00138186) | 0x001BD7DA |
| 607 | 1:0x0012818F (linear 0x0013818F) | 0x001BD7E3 |
| 608 | 1:0x001281A8 (linear 0x001381A8) | 0x001BD7FC |
| 609 | 1:0x001281AD (linear 0x001381AD) | 0x001BD801 |
| 612 | 1:0x001281B5 (linear 0x001381B5) | 0x001BD809 |
| 618 | 1:0x001281D2 (linear 0x001381D2) | 0x001BD826 |
| 620 | 1:0x001281DA (linear 0x001381DA) | 0x001BD82E |
| 623 | 1:0x001281F7 (linear 0x001381F7) | 0x001BD84B |
| 624 | 1:0x0012820B (linear 0x0013820B) | 0x001BD85F |
| 626 | 1:0x00128215 (linear 0x00138215) | 0x001BD869 |
| 627 | 1:0x00128221 (linear 0x00138221) | 0x001BD875 |
| 630 | 1:0x00128229 (linear 0x00138229) | 0x001BD87D |
| 634 | 1:0x00128248 (linear 0x00138248) | 0x001BD89C |
| 636 | 1:0x00128252 (linear 0x00138252) | 0x001BD8A6 |
| 638 | 1:0x0012825F (linear 0x0013825F) | 0x001BD8B3 |
| 639 | 1:0x00128279 (linear 0x00138279) | 0x001BD8CD |
| 641 | 1:0x00128289 (linear 0x00138289) | 0x001BD8DD |
| 644 | 1:0x001282B8 (linear 0x001382B8) | 0x001BD90C |
| 646 | 1:0x001282BF (linear 0x001382BF) | 0x001BD913 |
| 651 | 1:0x001282E0 (linear 0x001382E0) | 0x001BD934 |
| 653 | 1:0x001282EA (linear 0x001382EA) | 0x001BD93E |
| 655 | 1:0x001282F7 (linear 0x001382F7) | 0x001BD94B |
| 656 | 1:0x00128311 (linear 0x00138311) | 0x001BD965 |
| 658 | 1:0x00128321 (linear 0x00138321) | 0x001BD975 |
| 660 | 1:0x00128331 (linear 0x00138331) | 0x001BD985 |
| 662 | 1:0x0012833E (linear 0x0013833E) | 0x001BD992 |
| 663 | 1:0x00128358 (linear 0x00138358) | 0x001BD9AC |
| 665 | 1:0x00128368 (linear 0x00138368) | 0x001BD9BC |
| 666 | 1:0x0012837D (linear 0x0013837D) | 0x001BD9D1 |

