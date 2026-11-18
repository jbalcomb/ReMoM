# F:\WLIBS\animate\clipped.c

Module 266 of the debug info of ORION2.EXE.

Ranges: 1:0x0013861D (linear 0x0014861D), 4,540 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0013861D (linear 0x0014861D) | code | Clipped_Draw_Bitmap_Sprite_ | `regions/07-object-1/001CD654-001CDE53.txt` |
| 1:0x001387E0 (linear 0x001487E0) | code | Clipped_Draw_Glassed_Bitmap_Sprite_ | `regions/07-object-1/001CD654-001CDE53.txt` |
| 1:0x001389F4 (linear 0x001489F4) | code | Clipped_Draw_Animated_Sprite_ | `regions/07-object-1/001CDE54-001CE653.txt` |
| 1:0x00138C33 (linear 0x00148C33) | code | Clipped_Draw_Glassed_Animated_Sprite_ | `regions/07-object-1/001CDE54-001CE653.txt` |
| 1:0x00138F57 (linear 0x00148F57) | code | Clipped_Draw_Stenciled_Animated_Sprite_ | `regions/07-object-1/001CDE54-001CE653.txt` |
| 1:0x00139378 (linear 0x00149378) | code | AClipped_Draw_Stenciled_Animated_Sprite_ | `regions/07-object-1/001CE654-001CEE53.txt` |

#### Clipped_Draw_Bitmap_Sprite

- Address: 1:0x0013861D (linear 0x0014861D), 451 bytes, in `regions/07-object-1/001CD654-001CDE53.txt`
- Type: void (SWORD, SWORD, UBYTE *)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x4] | SWORD |
| y1 | [EBP-0x8] | SWORD |
| src | [EBP-0x14] | UBYTE * |
| i | [EBP-0x1C] | SDWORD |
| dst | [EBP-0x18] | UBYTE * |
| cwidth | [EBP-0xC] | SDWORD |
| cheight | [EBP-0x10] | SDWORD |

#### Clipped_Draw_Glassed_Bitmap_Sprite

- Address: 1:0x001387E0 (linear 0x001487E0), 532 bytes, in `regions/07-object-1/001CD654-001CDE53.txt`
- Type: void (SDWORD, SDWORD, UBYTE *)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x28] | SDWORD |
| y1 | [EBP-0x24] | SDWORD |
| src | [EBP-0x20] | UBYTE * |
| i | [EBP-0x1C] | SDWORD |
| j | [EBP-0x18] | SDWORD |
| ts | [EBP-0x14] | UBYTE * |
| td | [EBP-0x10] | UBYTE * |
| dst | [EBP-0xC] | UBYTE * |
| cwidth | [EBP-0x8] | SDWORD |
| cheight | [EBP-0x4] | SDWORD |

#### Clipped_Draw_Animated_Sprite

- Address: 1:0x001389F4 (linear 0x001489F4), 575 bytes, in `regions/07-object-1/001CDE54-001CE653.txt`
- Type: void (SDWORD, SDWORD, UBYTE *)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x3C] | SDWORD |
| y1 | [EBP-0x38] | SDWORD |
| frame | [EBP-0x34] | UBYTE * |
| x | [EBP-0x30] | SDWORD |
| y | [EBP-0x2C] | SDWORD |
| cx1 | [EBP-0x28] | SDWORD |
| cy1 | [EBP-0x24] | SDWORD |
| cx2 | [EBP-0x20] | SDWORD |
| cy2 | [EBP-0x1C] | SDWORD |
| dst | [EBP-0x18] | UBYTE * |
| src | [EBP-0x14] | UBYTE * |
| line_ptr | [EBP-0x10] | UBYTE * |
| data | [EBP-0xC] | SDWORD |
| skip | [EBP-0x8] | SDWORD |
| bottom | [EBP-0x4] | SDWORD |

#### Clipped_Draw_Glassed_Animated_Sprite

- Address: 1:0x00138C33 (linear 0x00148C33), 804 bytes, in `regions/07-object-1/001CDE54-001CE653.txt`
- Type: void (SDWORD, SDWORD, UBYTE *)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x48] | SDWORD |
| y1 | [EBP-0x44] | SDWORD |
| frame | [EBP-0x40] | UBYTE * |
| i | [EBP-0x3C] | SDWORD |
| x | [EBP-0x38] | SDWORD |
| y | [EBP-0x34] | SDWORD |
| cx1 | [EBP-0x30] | SDWORD |
| cy1 | [EBP-0x2C] | SDWORD |
| cx2 | [EBP-0x28] | SDWORD |
| cy2 | [EBP-0x24] | SDWORD |
| dst | [EBP-0x20] | UBYTE * |
| src | [EBP-0x1C] | UBYTE * |
| line_ptr | [EBP-0x18] | UBYTE * |
| ts | [EBP-0x14] | UBYTE * |
| td | [EBP-0x10] | UBYTE * |
| data | [EBP-0xC] | SDWORD |
| skip | [EBP-0x8] | SDWORD |
| bottom | [EBP-0x4] | SDWORD |

#### Clipped_Draw_Stenciled_Animated_Sprite

- Address: 1:0x00138F57 (linear 0x00148F57), 1,057 bytes, in `regions/07-object-1/001CDE54-001CE653.txt`
- Type: void (short, short, unsigned char *)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x50] | short |
| y1 | [EBP-0x54] | short |
| frame_data | [EBP-0x9C] | unsigned char * |
| bitmap_size | [EBP-0x98] | int |
| pos | [EBP-0x94] | int |
| screen_pos | [EBP-0x90] | int |
| x | [EBP-0x8C] | int |
| y | [EBP-0x88] | int |
| i | [EBP-0x84] | int |
| screen_start | [EBP-0x80] | int |
| buffer_pos | [EBP-0x7C] | int |
| buffer_pos_word | [EBP-0x78] | int |
| packet_end | [EBP-0xA0] | int |
| height | [EBP-0x20] | short |
| data_count | [EBP-0x1C] | short |
| skip_count | [EBP-0x18] | short |
| store_type | [EBP-0x14] | short |
| line_skip | [EBP-0x10] | short |
| width | [EBP-0xC] | short |
| x2 | [EBP-0x8] | short |
| y2 | [EBP-0x4] | short |
| xx1 | [EBP-0x24] | short |
| yy1 | [EBP-0x4C] | short |
| skip_x | [EBP-0x48] | short |
| start_x | [EBP-0x44] | short |
| skip_y | [EBP-0x40] | short |
| start_y | [EBP-0x3C] | short |
| actual_width | [EBP-0x38] | short |
| actual_height | [EBP-0x34] | short |
| skip_add | [EBP-0x30] | short |
| clipped_count | [EBP-0x2C] | short |
| clipped_add | [EBP-0x28] | short |
| data | [EBP-0x58] | unsigned long |
| frame_data_word | [EBP-0x70] | unsigned short * |
| buffer_add | [EBP-0x6C] | int |
| screen_pixel_start | [EBP-0x68] | int |
| screen_pixel_end | [EBP-0x64] | int |
| drawn_flag | [EBP-0x74] | int |
| screen_line_add | [EBP-0x60] | int |
| screen_line_start | [EBP-0x5C] | int |
| screen_line_end | [EBP-0xA4] | int |

#### AClipped_Draw_Stenciled_Animated_Sprite

- Address: 1:0x00139378 (linear 0x00149378), 1,121 bytes, in `regions/07-object-1/001CE654-001CEE53.txt`
- Type: void (short, short, unsigned char *)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x50] | short |
| y1 | [EBP-0x58] | short |
| frame_data | [EBP-0xA0] | unsigned char * |
| bitmap_size | [EBP-0x9C] | int |
| pos | [EBP-0x98] | int |
| screen_pos | [EBP-0x94] | int |
| x | [EBP-0x90] | int |
| y | [EBP-0x8C] | int |
| i | [EBP-0x88] | int |
| screen_start | [EBP-0x84] | int |
| buffer_pos | [EBP-0x80] | int |
| buffer_pos_word | [EBP-0xA8] | int |
| packet_end | [EBP-0xA4] | int |
| height | [EBP-0x20] | short |
| data_count | [EBP-0x1C] | short |
| skip_count | [EBP-0x18] | short |
| store_type | [EBP-0x14] | short |
| line_skip | [EBP-0x10] | short |
| width | [EBP-0xC] | short |
| x2 | [EBP-0x8] | short |
| y2 | [EBP-0x4] | short |
| x_flag | [EBP-0x54] | short |
| xx1 | [EBP-0x24] | short |
| yy1 | [EBP-0x4C] | short |
| skip_x | [EBP-0x48] | short |
| start_x | [EBP-0x44] | short |
| skip_y | [EBP-0x40] | short |
| start_y | [EBP-0x3C] | short |
| actual_width | [EBP-0x38] | short |
| actual_height | [EBP-0x34] | short |
| skip_add | [EBP-0x30] | short |
| clipped_count | [EBP-0x2C] | short |
| clipped_add | [EBP-0x28] | short |
| data | [EBP-0x7C] | unsigned long |
| frame_data_word | [EBP-0x74] | unsigned short * |
| buffer_add | [EBP-0x70] | int |
| x_pos1 | [EBP-0x6C] | int |
| y_pos | [EBP-0x68] | int |
| x_pos2 | [EBP-0x78] | int |
| screen_line_add | [EBP-0x60] | int |
| screen_line_start | [EBP-0x5C] | int |
| screen_line_end | [EBP-0x64] | int |

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
| 23 | scalar | void | void 1 bytes |
| 24 | pointer |  | near386; of UBYTE (#14) |
| 25 | procedure |  | near386; returns void (SWORD, SWORD, UBYTE *) |
| 26 | procedure |  | near386; returns void (SDWORD, SDWORD, UBYTE *) |
| 27 | pointer |  | near386; of unsigned char (#5) |
| 28 | procedure |  | near386; returns void (short, short, unsigned char *) |
| 29 | scalar | unsigned long | unsigned 4 bytes |
| 30 | pointer |  | near386; of unsigned short (#7) |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 11 | 1:0x0013861D (linear 0x0014861D) | 0x001CDC71 |
| 14 | 1:0x0013863C (linear 0x0014863C) | 0x001CDC90 |
| 15 | 1:0x00138657 (linear 0x00148657) | 0x001CDCAB |
| 16 | 1:0x001386C0 (linear 0x001486C0) | 0x001CDD14 |
| 18 | 1:0x0013872D (linear 0x0014872D) | 0x001CDD81 |
| 20 | 1:0x00138739 (linear 0x00148739) | 0x001CDD8D |
| 21 | 1:0x0013874B (linear 0x0014874B) | 0x001CDD9F |
| 24 | 1:0x0013875D (linear 0x0014875D) | 0x001CDDB1 |
| 26 | 1:0x00138769 (linear 0x00148769) | 0x001CDDBD |
| 27 | 1:0x00138786 (linear 0x00148786) | 0x001CDDDA |
| 30 | 1:0x0013879E (linear 0x0014879E) | 0x001CDDF2 |
| 32 | 1:0x001387B7 (linear 0x001487B7) | 0x001CDE0B |
| 33 | 1:0x001387C5 (linear 0x001487C5) | 0x001CDE19 |
| 34 | 1:0x001387D0 (linear 0x001487D0) | 0x001CDE24 |
| 35 | 1:0x001387D7 (linear 0x001487D7) | 0x001CDE2B |
| 36 | 1:0x001387D9 (linear 0x001487D9) | 0x001CDE2D |
| 39 | 1:0x001387E0 (linear 0x001487E0) | 0x001CDE34 |
| 45 | 1:0x001387FF (linear 0x001487FF) | 0x001CDE53 |
| 46 | 1:0x00138815 (linear 0x00148815) | 0x001CDE69 |
| 47 | 1:0x00138874 (linear 0x00148874) | 0x001CDEC8 |
| 49 | 1:0x001388D7 (linear 0x001488D7) | 0x001CDF2B |
| 51 | 1:0x001388E4 (linear 0x001488E4) | 0x001CDF38 |
| 52 | 1:0x001388F2 (linear 0x001488F2) | 0x001CDF46 |
| 55 | 1:0x00138900 (linear 0x00148900) | 0x001CDF54 |
| 57 | 1:0x0013890D (linear 0x0014890D) | 0x001CDF61 |
| 58 | 1:0x00138928 (linear 0x00148928) | 0x001CDF7C |
| 61 | 1:0x0013893C (linear 0x0014893C) | 0x001CDF90 |
| 63 | 1:0x00138958 (linear 0x00148958) | 0x001CDFAC |
| 64 | 1:0x0013895E (linear 0x0014895E) | 0x001CDFB2 |
| 65 | 1:0x00138964 (linear 0x00148964) | 0x001CDFB8 |
| 66 | 1:0x00138989 (linear 0x00148989) | 0x001CDFDD |
| 68 | 1:0x00138991 (linear 0x00148991) | 0x001CDFE5 |
| 69 | 1:0x001389A2 (linear 0x001489A2) | 0x001CDFF6 |
| 70 | 1:0x001389AC (linear 0x001489AC) | 0x001CE000 |
| 71 | 1:0x001389AE (linear 0x001489AE) | 0x001CE002 |
| 72 | 1:0x001389D4 (linear 0x001489D4) | 0x001CE028 |
| 74 | 1:0x001389D6 (linear 0x001489D6) | 0x001CE02A |
| 75 | 1:0x001389E1 (linear 0x001489E1) | 0x001CE035 |
| 76 | 1:0x001389E8 (linear 0x001489E8) | 0x001CE03C |
| 77 | 1:0x001389ED (linear 0x001489ED) | 0x001CE041 |
| 80 | 1:0x001389F4 (linear 0x001489F4) | 0x001CE048 |
| 82 | 1:0x00138A13 (linear 0x00148A13) | 0x001CE067 |
| 83 | 1:0x00138A19 (linear 0x00148A19) | 0x001CE06D |
| 84 | 1:0x00138A1F (linear 0x00148A1F) | 0x001CE073 |
| 85 | 1:0x00138A25 (linear 0x00148A25) | 0x001CE079 |
| 86 | 1:0x00138A2B (linear 0x00148A2B) | 0x001CE07F |
| 87 | 1:0x00138A32 (linear 0x00148A32) | 0x001CE086 |
| 88 | 1:0x00138A39 (linear 0x00148A39) | 0x001CE08D |
| 89 | 1:0x00138A41 (linear 0x00148A41) | 0x001CE095 |
| 95 | 1:0x00138A4A (linear 0x00148A4A) | 0x001CE09E |
| 96 | 1:0x00138A57 (linear 0x00148A57) | 0x001CE0AB |
| 98 | 1:0x00138A62 (linear 0x00148A62) | 0x001CE0B6 |
| 99 | 1:0x00138A6F (linear 0x00148A6F) | 0x001CE0C3 |
| 101 | 1:0x00138A7A (linear 0x00148A7A) | 0x001CE0CE |
| 102 | 1:0x00138A92 (linear 0x00148A92) | 0x001CE0E6 |
| 104 | 1:0x00138A9D (linear 0x00148A9D) | 0x001CE0F1 |
| 105 | 1:0x00138AB7 (linear 0x00148AB7) | 0x001CE10B |
| 107 | 1:0x00138AC2 (linear 0x00148AC2) | 0x001CE116 |
| 108 | 1:0x00138AC8 (linear 0x00148AC8) | 0x001CE11C |
| 110 | 1:0x00138AD8 (linear 0x00148AD8) | 0x001CE12C |
| 112 | 1:0x00138AF5 (linear 0x00148AF5) | 0x001CE149 |
| 114 | 1:0x00138B27 (linear 0x00148B27) | 0x001CE17B |
| 116 | 1:0x00138B33 (linear 0x00148B33) | 0x001CE187 |
| 117 | 1:0x00138B3C (linear 0x00148B3C) | 0x001CE190 |
| 118 | 1:0x00138B40 (linear 0x00148B40) | 0x001CE194 |
| 119 | 1:0x00138B49 (linear 0x00148B49) | 0x001CE19D |
| 121 | 1:0x00138B4D (linear 0x00148B4D) | 0x001CE1A1 |
| 123 | 1:0x00138B53 (linear 0x00148B53) | 0x001CE1A7 |
| 124 | 1:0x00138B5D (linear 0x00148B5D) | 0x001CE1B1 |
| 125 | 1:0x00138B63 (linear 0x00148B63) | 0x001CE1B7 |
| 126 | 1:0x00138B69 (linear 0x00148B69) | 0x001CE1BD |
| 128 | 1:0x00138B6F (linear 0x00148B6F) | 0x001CE1C3 |
| 130 | 1:0x00138B89 (linear 0x00148B89) | 0x001CE1DD |
| 131 | 1:0x00138B8F (linear 0x00148B8F) | 0x001CE1E3 |
| 133 | 1:0x00138B95 (linear 0x00148B95) | 0x001CE1E9 |
| 135 | 1:0x00138B9D (linear 0x00148B9D) | 0x001CE1F1 |
| 136 | 1:0x00138BA6 (linear 0x00148BA6) | 0x001CE1FA |
| 137 | 1:0x00138BAF (linear 0x00148BAF) | 0x001CE203 |
| 138 | 1:0x00138BB8 (linear 0x00148BB8) | 0x001CE20C |
| 141 | 1:0x00138BC1 (linear 0x00148BC1) | 0x001CE215 |
| 143 | 1:0x00138BCC (linear 0x00148BCC) | 0x001CE220 |
| 144 | 1:0x00138BD6 (linear 0x00148BD6) | 0x001CE22A |
| 146 | 1:0x00138BE8 (linear 0x00148BE8) | 0x001CE23C |
| 147 | 1:0x00138BF0 (linear 0x00148BF0) | 0x001CE244 |
| 149 | 1:0x00138BFE (linear 0x00148BFE) | 0x001CE252 |
| 150 | 1:0x00138C04 (linear 0x00148C04) | 0x001CE258 |
| 151 | 1:0x00138C0A (linear 0x00148C0A) | 0x001CE25E |
| 153 | 1:0x00138C10 (linear 0x00148C10) | 0x001CE264 |
| 154 | 1:0x00138C12 (linear 0x00148C12) | 0x001CE266 |
| 156 | 1:0x00138C18 (linear 0x00148C18) | 0x001CE26C |
| 157 | 1:0x00138C21 (linear 0x00148C21) | 0x001CE275 |
| 158 | 1:0x00138C27 (linear 0x00148C27) | 0x001CE27B |
| 159 | 1:0x00138C2C (linear 0x00148C2C) | 0x001CE280 |
| 162 | 1:0x00138C33 (linear 0x00148C33) | 0x001CE287 |
| 165 | 1:0x00138C52 (linear 0x00148C52) | 0x001CE2A6 |
| 166 | 1:0x00138C58 (linear 0x00148C58) | 0x001CE2AC |
| 167 | 1:0x00138C5E (linear 0x00148C5E) | 0x001CE2B2 |
| 168 | 1:0x00138C64 (linear 0x00148C64) | 0x001CE2B8 |
| 169 | 1:0x00138C6A (linear 0x00148C6A) | 0x001CE2BE |
| 170 | 1:0x00138C71 (linear 0x00148C71) | 0x001CE2C5 |
| 171 | 1:0x00138C78 (linear 0x00148C78) | 0x001CE2CC |
| 172 | 1:0x00138C80 (linear 0x00148C80) | 0x001CE2D4 |
| 180 | 1:0x00138C89 (linear 0x00148C89) | 0x001CE2DD |
| 181 | 1:0x00138C96 (linear 0x00148C96) | 0x001CE2EA |
| 183 | 1:0x00138CA1 (linear 0x00148CA1) | 0x001CE2F5 |
| 184 | 1:0x00138CAE (linear 0x00148CAE) | 0x001CE302 |
| 186 | 1:0x00138CB9 (linear 0x00148CB9) | 0x001CE30D |
| 187 | 1:0x00138CD3 (linear 0x00148CD3) | 0x001CE327 |
| 189 | 1:0x00138CDE (linear 0x00148CDE) | 0x001CE332 |
| 190 | 1:0x00138CF8 (linear 0x00148CF8) | 0x001CE34C |
| 192 | 1:0x00138D03 (linear 0x00148D03) | 0x001CE357 |
| 193 | 1:0x00138D09 (linear 0x00148D09) | 0x001CE35D |
| 195 | 1:0x00138D19 (linear 0x00148D19) | 0x001CE36D |
| 197 | 1:0x00138D36 (linear 0x00148D36) | 0x001CE38A |
| 199 | 1:0x00138D68 (linear 0x00148D68) | 0x001CE3BC |
| 201 | 1:0x00138D74 (linear 0x00148D74) | 0x001CE3C8 |
| 202 | 1:0x00138D7D (linear 0x00148D7D) | 0x001CE3D1 |
| 203 | 1:0x00138D81 (linear 0x00148D81) | 0x001CE3D5 |
| 204 | 1:0x00138D8A (linear 0x00148D8A) | 0x001CE3DE |
| 206 | 1:0x00138D8E (linear 0x00148D8E) | 0x001CE3E2 |
| 208 | 1:0x00138D94 (linear 0x00148D94) | 0x001CE3E8 |
| 209 | 1:0x00138D9E (linear 0x00148D9E) | 0x001CE3F2 |
| 210 | 1:0x00138DA4 (linear 0x00148DA4) | 0x001CE3F8 |
| 211 | 1:0x00138DAA (linear 0x00148DAA) | 0x001CE3FE |
| 213 | 1:0x00138DB0 (linear 0x00148DB0) | 0x001CE404 |
| 215 | 1:0x00138DCA (linear 0x00148DCA) | 0x001CE41E |
| 216 | 1:0x00138DD0 (linear 0x00148DD0) | 0x001CE424 |
| 218 | 1:0x00138DD6 (linear 0x00148DD6) | 0x001CE42A |
| 220 | 1:0x00138DDE (linear 0x00148DDE) | 0x001CE432 |
| 221 | 1:0x00138DE7 (linear 0x00148DE7) | 0x001CE43B |
| 222 | 1:0x00138DF0 (linear 0x00148DF0) | 0x001CE444 |
| 223 | 1:0x00138DF9 (linear 0x00148DF9) | 0x001CE44D |
| 226 | 1:0x00138E02 (linear 0x00148E02) | 0x001CE456 |
| 227 | 1:0x00138E08 (linear 0x00148E08) | 0x001CE45C |
| 229 | 1:0x00138E0E (linear 0x00148E0E) | 0x001CE462 |
| 231 | 1:0x00138E1D (linear 0x00148E1D) | 0x001CE471 |
| 233 | 1:0x00138E2B (linear 0x00148E2B) | 0x001CE47F |
| 234 | 1:0x00138E54 (linear 0x00148E54) | 0x001CE4A8 |
| 236 | 1:0x00138E5C (linear 0x00148E5C) | 0x001CE4B0 |
| 237 | 1:0x00138E6D (linear 0x00148E6D) | 0x001CE4C1 |
| 238 | 1:0x00138E77 (linear 0x00148E77) | 0x001CE4CB |
| 239 | 1:0x00138E79 (linear 0x00148E79) | 0x001CE4CD |
| 240 | 1:0x00138E9F (linear 0x00148E9F) | 0x001CE4F3 |
| 243 | 1:0x00138EA1 (linear 0x00148EA1) | 0x001CE4F5 |
| 244 | 1:0x00138EB0 (linear 0x00148EB0) | 0x001CE504 |
| 245 | 1:0x00138ED5 (linear 0x00148ED5) | 0x001CE529 |
| 247 | 1:0x00138EDD (linear 0x00148EDD) | 0x001CE531 |
| 248 | 1:0x00138EEE (linear 0x00148EEE) | 0x001CE542 |
| 249 | 1:0x00138EF8 (linear 0x00148EF8) | 0x001CE54C |
| 250 | 1:0x00138EFA (linear 0x00148EFA) | 0x001CE54E |
| 251 | 1:0x00138F20 (linear 0x00148F20) | 0x001CE574 |
| 253 | 1:0x00138F22 (linear 0x00148F22) | 0x001CE576 |
| 254 | 1:0x00138F28 (linear 0x00148F28) | 0x001CE57C |
| 255 | 1:0x00138F2E (linear 0x00148F2E) | 0x001CE582 |
| 257 | 1:0x00138F34 (linear 0x00148F34) | 0x001CE588 |
| 258 | 1:0x00138F36 (linear 0x00148F36) | 0x001CE58A |
| 260 | 1:0x00138F3C (linear 0x00148F3C) | 0x001CE590 |
| 261 | 1:0x00138F45 (linear 0x00148F45) | 0x001CE599 |
| 262 | 1:0x00138F4B (linear 0x00148F4B) | 0x001CE59F |
| 263 | 1:0x00138F50 (linear 0x00148F50) | 0x001CE5A4 |
| 266 | 1:0x00138F57 (linear 0x00148F57) | 0x001CE5AB |
| 273 | 1:0x00138F79 (linear 0x00148F79) | 0x001CE5CD |
| 276 | 1:0x00138F80 (linear 0x00148F80) | 0x001CE5D4 |
| 277 | 1:0x00138F8B (linear 0x00148F8B) | 0x001CE5DF |
| 279 | 1:0x00138F97 (linear 0x00148F97) | 0x001CE5EB |
| 281 | 1:0x00138FA3 (linear 0x00148FA3) | 0x001CE5F7 |
| 282 | 1:0x00138FAA (linear 0x00148FAA) | 0x001CE5FE |
| 284 | 1:0x00138FB0 (linear 0x00148FB0) | 0x001CE604 |
| 286 | 1:0x00138FB2 (linear 0x00148FB2) | 0x001CE606 |
| 287 | 1:0x00138FBE (linear 0x00148FBE) | 0x001CE612 |
| 290 | 1:0x00138FC7 (linear 0x00148FC7) | 0x001CE61B |
| 292 | 1:0x00138FD3 (linear 0x00148FD3) | 0x001CE627 |
| 293 | 1:0x00138FDA (linear 0x00148FDA) | 0x001CE62E |
| 295 | 1:0x00138FE0 (linear 0x00148FE0) | 0x001CE634 |
| 297 | 1:0x00138FE2 (linear 0x00148FE2) | 0x001CE636 |
| 298 | 1:0x00138FEE (linear 0x00148FEE) | 0x001CE642 |
| 301 | 1:0x00138FF7 (linear 0x00148FF7) | 0x001CE64B |
| 302 | 1:0x00139001 (linear 0x00149001) | 0x001CE655 |
| 304 | 1:0x0013900B (linear 0x0014900B) | 0x001CE65F |
| 305 | 1:0x00139021 (linear 0x00149021) | 0x001CE675 |
| 307 | 1:0x00139030 (linear 0x00149030) | 0x001CE684 |
| 308 | 1:0x00139046 (linear 0x00149046) | 0x001CE69A |
| 311 | 1:0x00139055 (linear 0x00149055) | 0x001CE6A9 |
| 312 | 1:0x00139069 (linear 0x00149069) | 0x001CE6BD |
| 314 | 1:0x00139076 (linear 0x00149076) | 0x001CE6CA |
| 318 | 1:0x00139085 (linear 0x00149085) | 0x001CE6D9 |
| 320 | 1:0x0013908E (linear 0x0014908E) | 0x001CE6E2 |
| 321 | 1:0x00139097 (linear 0x00149097) | 0x001CE6EB |
| 324 | 1:0x001390A6 (linear 0x001490A6) | 0x001CE6FA |
| 325 | 1:0x001390BC (linear 0x001490BC) | 0x001CE710 |
| 327 | 1:0x001390C5 (linear 0x001490C5) | 0x001CE719 |
| 329 | 1:0x001390CC (linear 0x001490CC) | 0x001CE720 |
| 331 | 1:0x001390D8 (linear 0x001490D8) | 0x001CE72C |
| 333 | 1:0x001390E6 (linear 0x001490E6) | 0x001CE73A |
| 334 | 1:0x00139104 (linear 0x00149104) | 0x001CE758 |
| 336 | 1:0x00139117 (linear 0x00149117) | 0x001CE76B |
| 337 | 1:0x0013911B (linear 0x0014911B) | 0x001CE76F |
| 339 | 1:0x0013911D (linear 0x0014911D) | 0x001CE771 |
| 341 | 1:0x00139129 (linear 0x00149129) | 0x001CE77D |
| 342 | 1:0x00139141 (linear 0x00149141) | 0x001CE795 |
| 344 | 1:0x0013914A (linear 0x0014914A) | 0x001CE79E |
| 346 | 1:0x00139154 (linear 0x00149154) | 0x001CE7A8 |
| 347 | 1:0x00139160 (linear 0x00149160) | 0x001CE7B4 |
| 349 | 1:0x00139175 (linear 0x00149175) | 0x001CE7C9 |
| 350 | 1:0x0013917E (linear 0x0014917E) | 0x001CE7D2 |
| 352 | 1:0x00139185 (linear 0x00149185) | 0x001CE7D9 |
| 354 | 1:0x00139195 (linear 0x00149195) | 0x001CE7E9 |
| 355 | 1:0x001391A3 (linear 0x001491A3) | 0x001CE7F7 |
| 357 | 1:0x001391B2 (linear 0x001491B2) | 0x001CE806 |
| 359 | 1:0x001391B6 (linear 0x001491B6) | 0x001CE80A |
| 361 | 1:0x001391C1 (linear 0x001491C1) | 0x001CE815 |
| 362 | 1:0x001391CB (linear 0x001491CB) | 0x001CE81F |
| 363 | 1:0x001391D1 (linear 0x001491D1) | 0x001CE825 |
| 364 | 1:0x0013920E (linear 0x0014920E) | 0x001CE862 |
| 365 | 1:0x0013921B (linear 0x0014921B) | 0x001CE86F |
| 366 | 1:0x00139224 (linear 0x00149224) | 0x001CE878 |
| 367 | 1:0x00139230 (linear 0x00149230) | 0x001CE884 |
| 368 | 1:0x00139245 (linear 0x00149245) | 0x001CE899 |
| 369 | 1:0x0013924E (linear 0x0014924E) | 0x001CE8A2 |
| 371 | 1:0x00139255 (linear 0x00149255) | 0x001CE8A9 |
| 373 | 1:0x0013925A (linear 0x0014925A) | 0x001CE8AE |
| 374 | 1:0x00139264 (linear 0x00149264) | 0x001CE8B8 |
| 375 | 1:0x0013926D (linear 0x0014926D) | 0x001CE8C1 |
| 377 | 1:0x0013927F (linear 0x0014927F) | 0x001CE8D3 |
| 379 | 1:0x00139291 (linear 0x00149291) | 0x001CE8E5 |
| 381 | 1:0x0013929C (linear 0x0014929C) | 0x001CE8F0 |
| 382 | 1:0x001392A8 (linear 0x001492A8) | 0x001CE8FC |
| 384 | 1:0x001392B7 (linear 0x001492B7) | 0x001CE90B |
| 385 | 1:0x001392BE (linear 0x001492BE) | 0x001CE912 |
| 388 | 1:0x001392C8 (linear 0x001492C8) | 0x001CE91C |
| 390 | 1:0x001392D7 (linear 0x001492D7) | 0x001CE92B |
| 391 | 1:0x001392E9 (linear 0x001492E9) | 0x001CE93D |
| 393 | 1:0x001392F8 (linear 0x001492F8) | 0x001CE94C |
| 395 | 1:0x0013930A (linear 0x0014930A) | 0x001CE95E |
| 397 | 1:0x00139318 (linear 0x00149318) | 0x001CE96C |
| 399 | 1:0x0013931E (linear 0x0014931E) | 0x001CE972 |
| 400 | 1:0x00139327 (linear 0x00149327) | 0x001CE97B |
| 402 | 1:0x0013932E (linear 0x0014932E) | 0x001CE982 |
| 404 | 1:0x00139337 (linear 0x00149337) | 0x001CE98B |
| 405 | 1:0x00139350 (linear 0x00149350) | 0x001CE9A4 |
| 406 | 1:0x00139353 (linear 0x00149353) | 0x001CE9A7 |
| 407 | 1:0x00139359 (linear 0x00149359) | 0x001CE9AD |
| 410 | 1:0x0013935B (linear 0x0014935B) | 0x001CE9AF |
| 412 | 1:0x0013936C (linear 0x0014936C) | 0x001CE9C0 |
| 413 | 1:0x00139371 (linear 0x00149371) | 0x001CE9C5 |
| 426 | 1:0x00139378 (linear 0x00149378) | 0x001CE9CC |
| 430 | 1:0x0013939A (linear 0x0014939A) | 0x001CE9EE |
| 436 | 1:0x001393A1 (linear 0x001493A1) | 0x001CE9F5 |
| 437 | 1:0x001393AC (linear 0x001493AC) | 0x001CEA00 |
| 439 | 1:0x001393B8 (linear 0x001493B8) | 0x001CEA0C |
| 441 | 1:0x001393C4 (linear 0x001493C4) | 0x001CEA18 |
| 442 | 1:0x001393CB (linear 0x001493CB) | 0x001CEA1F |
| 444 | 1:0x001393D1 (linear 0x001493D1) | 0x001CEA25 |
| 446 | 1:0x001393D3 (linear 0x001493D3) | 0x001CEA27 |
| 447 | 1:0x001393DF (linear 0x001493DF) | 0x001CEA33 |
| 450 | 1:0x001393E8 (linear 0x001493E8) | 0x001CEA3C |
| 452 | 1:0x001393F4 (linear 0x001493F4) | 0x001CEA48 |
| 453 | 1:0x001393FB (linear 0x001493FB) | 0x001CEA4F |
| 455 | 1:0x00139401 (linear 0x00149401) | 0x001CEA55 |
| 457 | 1:0x00139403 (linear 0x00149403) | 0x001CEA57 |
| 458 | 1:0x0013940F (linear 0x0014940F) | 0x001CEA63 |
| 461 | 1:0x00139418 (linear 0x00149418) | 0x001CEA6C |
| 462 | 1:0x00139422 (linear 0x00149422) | 0x001CEA76 |
| 464 | 1:0x0013942C (linear 0x0014942C) | 0x001CEA80 |
| 465 | 1:0x00139442 (linear 0x00149442) | 0x001CEA96 |
| 467 | 1:0x00139451 (linear 0x00149451) | 0x001CEAA5 |
| 468 | 1:0x00139467 (linear 0x00149467) | 0x001CEABB |
| 471 | 1:0x00139476 (linear 0x00149476) | 0x001CEACA |
| 472 | 1:0x0013948A (linear 0x0014948A) | 0x001CEADE |
| 474 | 1:0x00139497 (linear 0x00149497) | 0x001CEAEB |
| 478 | 1:0x001394A6 (linear 0x001494A6) | 0x001CEAFA |
| 480 | 1:0x001394AF (linear 0x001494AF) | 0x001CEB03 |
| 481 | 1:0x001394B8 (linear 0x001494B8) | 0x001CEB0C |
| 483 | 1:0x001394C7 (linear 0x001494C7) | 0x001CEB1B |
| 484 | 1:0x001394E0 (linear 0x001494E0) | 0x001CEB34 |
| 486 | 1:0x001394EC (linear 0x001494EC) | 0x001CEB40 |
| 488 | 1:0x001394F6 (linear 0x001494F6) | 0x001CEB4A |
| 490 | 1:0x00139502 (linear 0x00149502) | 0x001CEB56 |
| 492 | 1:0x00139513 (linear 0x00149513) | 0x001CEB67 |
| 493 | 1:0x00139534 (linear 0x00149534) | 0x001CEB88 |
| 495 | 1:0x0013954A (linear 0x0014954A) | 0x001CEB9E |
| 496 | 1:0x00139551 (linear 0x00149551) | 0x001CEBA5 |
| 498 | 1:0x00139553 (linear 0x00149553) | 0x001CEBA7 |
| 500 | 1:0x0013955F (linear 0x0014955F) | 0x001CEBB3 |
| 501 | 1:0x0013957A (linear 0x0014957A) | 0x001CEBCE |
| 503 | 1:0x00139586 (linear 0x00149586) | 0x001CEBDA |
| 505 | 1:0x00139590 (linear 0x00149590) | 0x001CEBE4 |
| 506 | 1:0x0013959F (linear 0x0014959F) | 0x001CEBF3 |
| 508 | 1:0x001395B4 (linear 0x001495B4) | 0x001CEC08 |
| 510 | 1:0x001395CD (linear 0x001495CD) | 0x001CEC21 |
| 511 | 1:0x001395DA (linear 0x001495DA) | 0x001CEC2E |
| 512 | 1:0x001395E0 (linear 0x001495E0) | 0x001CEC34 |
| 514 | 1:0x001395E7 (linear 0x001495E7) | 0x001CEC3B |
| 516 | 1:0x001395F7 (linear 0x001495F7) | 0x001CEC4B |
| 517 | 1:0x00139608 (linear 0x00149608) | 0x001CEC5C |
| 519 | 1:0x0013961A (linear 0x0014961A) | 0x001CEC6E |
| 521 | 1:0x00139621 (linear 0x00149621) | 0x001CEC75 |
| 523 | 1:0x0013962C (linear 0x0014962C) | 0x001CEC80 |
| 530 | 1:0x00139636 (linear 0x00149636) | 0x001CEC8A |
| 542 | 1:0x00139644 (linear 0x00149644) | 0x001CEC98 |
| 543 | 1:0x00139654 (linear 0x00149654) | 0x001CECA8 |
| 544 | 1:0x0013965B (linear 0x0014965B) | 0x001CECAF |
| 545 | 1:0x00139668 (linear 0x00149668) | 0x001CECBC |
| 546 | 1:0x0013966E (linear 0x0014966E) | 0x001CECC2 |
| 547 | 1:0x00139675 (linear 0x00149675) | 0x001CECC9 |
| 548 | 1:0x00139681 (linear 0x00149681) | 0x001CECD5 |
| 549 | 1:0x00139690 (linear 0x00149690) | 0x001CECE4 |
| 551 | 1:0x001396A5 (linear 0x001496A5) | 0x001CECF9 |
| 553 | 1:0x001396AA (linear 0x001496AA) | 0x001CECFE |
| 554 | 1:0x001396B4 (linear 0x001496B4) | 0x001CED08 |
| 555 | 1:0x001396C3 (linear 0x001496C3) | 0x001CED17 |
| 557 | 1:0x001396D5 (linear 0x001496D5) | 0x001CED29 |
| 558 | 1:0x001396E3 (linear 0x001496E3) | 0x001CED37 |
| 560 | 1:0x001396EA (linear 0x001496EA) | 0x001CED3E |
| 562 | 1:0x001396F9 (linear 0x001496F9) | 0x001CED4D |
| 564 | 1:0x00139704 (linear 0x00149704) | 0x001CED58 |
| 565 | 1:0x00139710 (linear 0x00149710) | 0x001CED64 |
| 567 | 1:0x0013971F (linear 0x0014971F) | 0x001CED73 |
| 568 | 1:0x00139726 (linear 0x00149726) | 0x001CED7A |
| 571 | 1:0x00139730 (linear 0x00149730) | 0x001CED84 |
| 573 | 1:0x0013973C (linear 0x0014973C) | 0x001CED90 |
| 574 | 1:0x0013974E (linear 0x0014974E) | 0x001CEDA2 |
| 576 | 1:0x0013975D (linear 0x0014975D) | 0x001CEDB1 |
| 578 | 1:0x0013976F (linear 0x0014976F) | 0x001CEDC3 |
| 580 | 1:0x0013977D (linear 0x0014977D) | 0x001CEDD1 |
| 582 | 1:0x00139784 (linear 0x00149784) | 0x001CEDD8 |
| 584 | 1:0x00139792 (linear 0x00149792) | 0x001CEDE6 |
| 585 | 1:0x001397AB (linear 0x001497AB) | 0x001CEDFF |
| 586 | 1:0x001397AE (linear 0x001497AE) | 0x001CEE02 |
| 587 | 1:0x001397B4 (linear 0x001497B4) | 0x001CEE08 |
| 588 | 1:0x001397B7 (linear 0x001497B7) | 0x001CEE0B |
| 591 | 1:0x001397B9 (linear 0x001497B9) | 0x001CEE0D |
| 593 | 1:0x001397CD (linear 0x001497CD) | 0x001CEE21 |
| 594 | 1:0x001397D2 (linear 0x001497D2) | 0x001CEE26 |

