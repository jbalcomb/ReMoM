# F:\WLIBS\palette\reduce.c

Module 191 of the debug info of ORION2.EXE.

Ranges: 1:0x00129AF3 (linear 0x00139AF3), 4,587 bytes, 2:0x00002F94 (linear 0x00172F94), 168 bytes, 2:0x00045154 (linear 0x001B5154), 6,660 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x00129AF3 (linear 0x00139AF3) | code | Reduce_Palette_ | `regions/07-object-1/001BEE54-001BF653.txt` |
| 1:0x00129D7E (linear 0x00139D7E) | code | Split_Cube_ | `regions/07-object-1/001BEE54-001BF653.txt` |
| 1:0x0012A4A9 (linear 0x0013A4A9) | code | Sort_Cube_Points_ | `regions/07-object-1/001BF654-001BFE53.txt` |
| 1:0x0012A711 (linear 0x0013A711) | code | Find_Longest_Axis_ | `regions/07-object-1/001BF654-001BFE53.txt` |
| 1:0x0012A7CC (linear 0x0013A7CC) | code | Initial_Cube_ | `regions/07-object-1/001BF654-001BFE53.txt` |
| 1:0x0012A84F (linear 0x0013A84F) | code | Find_Next_Lowest_Cube_Level_ | `regions/07-object-1/001BFE54-001C0653.txt` |
| 1:0x0012A993 (linear 0x0013A993) | code | Shrink_Cube_ | `regions/07-object-1/001BFE54-001C0653.txt` |
| 1:0x0012AAC1 (linear 0x0013AAC1) | code | Clear_Reduced_Palette_Groups_ | `regions/07-object-1/001BFE54-001C0653.txt` |
| 1:0x0012AAE9 (linear 0x0013AAE9) | code | Add_Reduced_Palette_Group_ | `regions/07-object-1/001BFE54-001C0653.txt` |
| 1:0x0012AC01 (linear 0x0013AC01) | code | Compress_Reduced_Palette_Groups_ | `regions/07-object-1/001BFE54-001C0653.txt` |
| 2:0x00045154 (linear 0x001B5154) | data | _color_weight | - |
| 2:0x00045354 (linear 0x001B5354) | data | _cube_list | - |
| 2:0x00045F54 (linear 0x001B5F54) | data | _HistPtr | - |
| 2:0x00046354 (linear 0x001B6354) | data | _reduced_palette_list | - |
| 2:0x00046B54 (linear 0x001B6B54) | data | _ncubes | - |
| 2:0x00046B56 (linear 0x001B6B56) | data | _reduced_palette_count | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x00045154 (linear 0x001B5154) | color_weight | unsigned char[512] |
| 2:0x00045354 (linear 0x001B5354) | cube_list | struct s_cube[256] |
| 2:0x00045F54 (linear 0x001B5F54) | HistPtr | unsigned short[512] |
| 2:0x00046354 (linear 0x001B6354) | reduced_palette_list | unsigned char[4][512] |
| 2:0x00046B54 (linear 0x001B6B54) | ncubes | unsigned short |
| 2:0x00046B56 (linear 0x001B6B56) | reduced_palette_count | short |

#### Reduce_Palette

- Address: 1:0x00129AF3 (linear 0x00139AF3), 651 bytes, in `regions/07-object-1/001BEE54-001BF653.txt`
- Type: void (unsigned char[4] *, short, short, short)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| palette_list | [EBP-0x38] | unsigned char[4] * |
| start_color | [EBP-0x34] | short |
| total_color_count | [EBP-0x30] | short |
| reduced_color_count | [EBP-0x2C] | short |
| split_cube | [EBP-0x28] | short |
| i | [EBP-0x24] | short |
| j | [EBP-0x20] | short |
| axis | [EBP-0x1C] | short |
| color_error | [EBP-0x18] | short |
| best_color | [EBP-0x14] | short |
| dr | [EBP-0x10] | short |
| db | [EBP-0xC] | short |
| dg | [EBP-0x8] | short |
| best_error | [EBP-0x4] | short |

#### Split_Cube

- Address: 1:0x00129D7E (linear 0x00139D7E), 1,835 bytes, in `regions/07-object-1/001BEE54-001BF653.txt`
- Type: void (unsigned char[4] *, short, short, short)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| palette_list | [EBP-0x28] | unsigned char[4] * |
| split_pos | [EBP-0x20] | short |
| axis | [EBP-0xC] | short |
| total_color_count | [EBP-0x1C] | short |
| slice_point | [EBP-0x18] | unsigned short |
| i | [EBP-0x14] | unsigned short |
| running_total | [EBP-0x10] | unsigned short |
| total_weight | [EBP-0x24] | int |
| temp_weight1 | [EBP-0x8] | unsigned char |
| temp_weight2 | [EBP-0x4] | unsigned char |

#### Sort_Cube_Points

- Address: 1:0x0012A4A9 (linear 0x0013A4A9), 616 bytes, in `regions/07-object-1/001BF654-001BFE53.txt`
- Type: void (unsigned char[4] *, short *, short, short)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| palette_list | [EBP-0x1C] | unsigned char[4] * |
| points | [EBP-0x18] | short * |
| count | [EBP-0x14] | short |
| axis | [EBP-0x10] | short |
| i | [EBP-0xC] | short |
| j | [EBP-0x8] | short |
| changed_flag | [EBP-0x4] | short |

#### Find_Longest_Axis

- Address: 1:0x0012A711 (linear 0x0013A711), 187 bytes, in `regions/07-object-1/001BF654-001BFE53.txt`
- Type: short (short); returns in AX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| split_pos | [EBP-0x14] | short |
| lr | [EBP-0xC] | unsigned char |
| lg | [EBP-0x4] | unsigned char |
| lb | [EBP-0x8] | unsigned char |

#### Initial_Cube

- Address: 1:0x0012A7CC (linear 0x0013A7CC), 131 bytes, in `regions/07-object-1/001BF654-001BFE53.txt`
- Type: void (unsigned char[4] *, short)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| palette_list | [EBP-0xC] | unsigned char[4] * |
| total_color_count | [EBP-0x8] | short |
| i | [EBP-0x4] | short |

#### Find_Next_Lowest_Cube_Level

- Address: 1:0x0012A84F (linear 0x0013A84F), 324 bytes, in `regions/07-object-1/001BFE54-001C0653.txt`
- Type: short (void); returns in AX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| level | [EBP-0x20] | short |
| i | [EBP-0x1C] | short |
| next_split | [EBP-0x18] | short |
| largest_cube | [EBP-0x14] | short |
| lr | [EBP-0x4] | unsigned char |
| lg | [EBP-0xC] | unsigned char |
| lb | [EBP-0x8] | unsigned char |

#### Shrink_Cube

- Address: 1:0x0012A993 (linear 0x0013A993), 302 bytes, in `regions/07-object-1/001BFE54-001C0653.txt`
- Type: void (struct s_cube *, unsigned char[4] *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| cube | [EBP-0x1C] | struct s_cube * |
| palette_list | [EBP-0x18] | unsigned char[4] * |
| r | [EBP-0x8] | unsigned char |
| g | [EBP-0x4] | unsigned char |
| b | [EBP-0xC] | unsigned char |
| i | [EBP-0x14] | short |
| color_num | [EBP-0x10] | short |

#### Clear_Reduced_Palette_Groups

- Address: 1:0x0012AAC1 (linear 0x0013AAC1), 40 bytes, in `regions/07-object-1/001BFE54-001C0653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Add_Reduced_Palette_Group

- Address: 1:0x0012AAE9 (linear 0x0013AAE9), 280 bytes, in `regions/07-object-1/001BFE54-001C0653.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| picture | [EBP-0x10] | char * |
| index | [EBP-0x4] | short |
| i | [EBP-0x8] | short |
| palette_data | [EBP-0xC] | char * |

#### Compress_Reduced_Palette_Groups

- Address: 1:0x0012AC01 (linear 0x0013AC01), 221 bytes, in `regions/07-object-1/001BFE54-001C0653.txt`
- Type: void (short, short)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| start_color | [EBP-0xC] | short |
| color_count | [EBP-0x8] | short |
| i | [EBP-0x4] | short |

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
| 12 | typedef | size_t | name for unsigned int (#9) |
| 13 | pointer |  | near386; of char (#10) |
| 14 | array | __va_list | 0..0; of char * (#13) |
| 15 | typedef | __va_list | name for char *[1] (#14) |
| 16 | scalar | long | int 4 bytes |
| 17 | typedef | fpos_t | name for long (#16) |
| 18 | typedef | fpos_t | name for long (#16) |
| 19 | typedef | BYTE | name for unsigned char (#5) |
| 20 | typedef | SBYTE | name for signed char (#4) |
| 21 | typedef | UBYTE | name for unsigned char (#5) |
| 22 | typedef | WORD | name for unsigned short (#7) |
| 23 | typedef | SWORD | name for short (#6) |
| 24 | typedef | UWORD | name for unsigned short (#7) |
| 25 | typedef | DWORD | name for unsigned int (#9) |
| 26 | typedef | SDWORD | name for int (#8) |
| 27 | typedef | UDWORD | name for unsigned int (#9) |
| 28 | typedef | BOOL | name for unsigned char (#5) |
| 29 | typedef | SampleHandle | name for int (#8) |
| 30 | array |  | 0..511; of unsigned char (#5) |
| 31 | struct | struct s_cube | 9 fields, 12 bytes |
| 32 | typedef | s_cube | struct name for struct #31 (#31) |
| 33 | array |  | 0..255; of struct s_cube (#32) |
| 34 | array |  | 0..511; of unsigned short (#7) |
| 35 | array |  | 0..3; of unsigned char (#5) |
| 36 | array |  | 0..511; of unsigned char[4] (#35) |
| 37 | scalar | void | void 1 bytes |
| 38 | array |  | 0..3; of unsigned char (#5) |
| 39 | pointer |  | near386; of unsigned char[4] (#38) |
| 40 | procedure |  | near386; returns void (unsigned char[4] *, short, short, short) |
| 41 | array |  | 0..3; of unsigned char (#5) |
| 42 | pointer |  | near386; of unsigned char[4] (#41) |
| 43 | procedure |  | near386; returns void (unsigned char[4] *, short, short, short) |
| 44 | array |  | 0..3; of unsigned char (#5) |
| 45 | pointer |  | near386; of unsigned char[4] (#44) |
| 46 | pointer |  | near386; of short (#6) |
| 47 | procedure |  | near386; returns void (unsigned char[4] *, short *, short, short) |
| 48 | procedure |  | near386; returns short (short) |
| 49 | array |  | 0..3; of unsigned char (#5) |
| 50 | pointer |  | near386; of unsigned char[4] (#49) |
| 51 | procedure |  | near386; returns void (unsigned char[4] *, short) |
| 52 | procedure |  | near386; returns short (void) |
| 53 | pointer |  | near386; of struct s_cube (#32) |
| 54 | array |  | 0..3; of unsigned char (#5) |
| 55 | pointer |  | near386; of unsigned char[4] (#54) |
| 56 | procedure |  | near386; returns void (struct s_cube *, unsigned char[4] *) |
| 57 | procedure |  | near386; returns void () |
| 58 | procedure |  | near386; returns void (char *) |
| 59 | procedure |  | near386; returns void (short, short) |

##### struct s_cube (#31, 9 fields, 12 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| lower | offset 0 | short |
| upper | offset 2 | short |
| level | offset 4 | short |
| rmin | offset 6 | unsigned char |
| rmax | offset 7 | unsigned char |
| gmin | offset 8 | unsigned char |
| gmax | offset 9 | unsigned char |
| bmin | offset 10 | unsigned char |
| bmax | offset 11 | unsigned char |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 53 | 1:0x00129AF3 (linear 0x00139AF3) | 0x001BF147 |
| 58 | 1:0x00129B14 (linear 0x00139B14) | 0x001BF168 |
| 59 | 1:0x00129B1F (linear 0x00139B1F) | 0x001BF173 |
| 61 | 1:0x00129B29 (linear 0x00139B29) | 0x001BF17D |
| 62 | 1:0x00129B34 (linear 0x00139B34) | 0x001BF188 |
| 64 | 1:0x00129B3E (linear 0x00139B3E) | 0x001BF192 |
| 66 | 1:0x00129B4A (linear 0x00139B4A) | 0x001BF19E |
| 68 | 1:0x00129B5F (linear 0x00139B5F) | 0x001BF1B3 |
| 69 | 1:0x00129B67 (linear 0x00139B67) | 0x001BF1BB |
| 71 | 1:0x00129B70 (linear 0x00139B70) | 0x001BF1C4 |
| 73 | 1:0x00129B7C (linear 0x00139B7C) | 0x001BF1D0 |
| 75 | 1:0x00129BC1 (linear 0x00139BC1) | 0x001BF215 |
| 76 | 1:0x00129BD5 (linear 0x00139BD5) | 0x001BF229 |
| 78 | 1:0x00129BDA (linear 0x00139BDA) | 0x001BF22E |
| 80 | 1:0x00129BFE (linear 0x00139BFE) | 0x001BF252 |
| 81 | 1:0x00129C11 (linear 0x00139C11) | 0x001BF265 |
| 82 | 1:0x00129C4F (linear 0x00139C4F) | 0x001BF2A3 |
| 83 | 1:0x00129C90 (linear 0x00139C90) | 0x001BF2E4 |
| 84 | 1:0x00129CD5 (linear 0x00139CD5) | 0x001BF329 |
| 86 | 1:0x00129CDA (linear 0x00139CDA) | 0x001BF32E |
| 88 | 1:0x00129CE7 (linear 0x00139CE7) | 0x001BF33B |
| 89 | 1:0x00129CFA (linear 0x00139CFA) | 0x001BF34E |
| 90 | 1:0x00129D20 (linear 0x00139D20) | 0x001BF374 |
| 91 | 1:0x00129D48 (linear 0x00139D48) | 0x001BF39C |
| 92 | 1:0x00129D70 (linear 0x00139D70) | 0x001BF3C4 |
| 93 | 1:0x00129D73 (linear 0x00139D73) | 0x001BF3C7 |
| 94 | 1:0x00129D78 (linear 0x00139D78) | 0x001BF3CC |
| 96 | 1:0x00129D7E (linear 0x00139D7E) | 0x001BF3D2 |
| 103 | 1:0x00129D9F (linear 0x00139D9F) | 0x001BF3F3 |
| 105 | 1:0x00129DD2 (linear 0x00139DD2) | 0x001BF426 |
| 107 | 1:0x00129E0A (linear 0x00139E0A) | 0x001BF45E |
| 108 | 1:0x00129E6A (linear 0x00139E6A) | 0x001BF4BE |
| 109 | 1:0x00129E6F (linear 0x00139E6F) | 0x001BF4C3 |
| 110 | 1:0x00129E89 (linear 0x00139E89) | 0x001BF4DD |
| 111 | 1:0x00129ED0 (linear 0x00139ED0) | 0x001BF524 |
| 113 | 1:0x00129ED5 (linear 0x00139ED5) | 0x001BF529 |
| 114 | 1:0x00129F0F (linear 0x00139F0F) | 0x001BF563 |
| 115 | 1:0x00129F50 (linear 0x00139F50) | 0x001BF5A4 |
| 116 | 1:0x00129F67 (linear 0x00139F67) | 0x001BF5BB |
| 118 | 1:0x00129F78 (linear 0x00139F78) | 0x001BF5CC |
| 119 | 1:0x00129F94 (linear 0x00139F94) | 0x001BF5E8 |
| 120 | 1:0x00129F99 (linear 0x00139F99) | 0x001BF5ED |
| 122 | 1:0x00129F9E (linear 0x00139F9E) | 0x001BF5F2 |
| 124 | 1:0x00129FD6 (linear 0x00139FD6) | 0x001BF62A |
| 125 | 1:0x0012A036 (linear 0x0013A036) | 0x001BF68A |
| 126 | 1:0x0012A03B (linear 0x0013A03B) | 0x001BF68F |
| 127 | 1:0x0012A055 (linear 0x0013A055) | 0x001BF6A9 |
| 128 | 1:0x0012A09C (linear 0x0013A09C) | 0x001BF6F0 |
| 130 | 1:0x0012A0A1 (linear 0x0013A0A1) | 0x001BF6F5 |
| 131 | 1:0x0012A0DB (linear 0x0013A0DB) | 0x001BF72F |
| 132 | 1:0x0012A11C (linear 0x0013A11C) | 0x001BF770 |
| 133 | 1:0x0012A133 (linear 0x0013A133) | 0x001BF787 |
| 135 | 1:0x0012A144 (linear 0x0013A144) | 0x001BF798 |
| 136 | 1:0x0012A160 (linear 0x0013A160) | 0x001BF7B4 |
| 137 | 1:0x0012A165 (linear 0x0013A165) | 0x001BF7B9 |
| 139 | 1:0x0012A16A (linear 0x0013A16A) | 0x001BF7BE |
| 141 | 1:0x0012A1A2 (linear 0x0013A1A2) | 0x001BF7F6 |
| 142 | 1:0x0012A202 (linear 0x0013A202) | 0x001BF856 |
| 143 | 1:0x0012A207 (linear 0x0013A207) | 0x001BF85B |
| 144 | 1:0x0012A221 (linear 0x0013A221) | 0x001BF875 |
| 145 | 1:0x0012A268 (linear 0x0013A268) | 0x001BF8BC |
| 147 | 1:0x0012A26D (linear 0x0013A26D) | 0x001BF8C1 |
| 148 | 1:0x0012A2A7 (linear 0x0013A2A7) | 0x001BF8FB |
| 149 | 1:0x0012A2E8 (linear 0x0013A2E8) | 0x001BF93C |
| 150 | 1:0x0012A2FF (linear 0x0013A2FF) | 0x001BF953 |
| 152 | 1:0x0012A310 (linear 0x0013A310) | 0x001BF964 |
| 153 | 1:0x0012A32C (linear 0x0013A32C) | 0x001BF980 |
| 157 | 1:0x0012A331 (linear 0x0013A331) | 0x001BF985 |
| 158 | 1:0x0012A338 (linear 0x0013A338) | 0x001BF98C |
| 159 | 1:0x0012A36D (linear 0x0013A36D) | 0x001BF9C1 |
| 161 | 1:0x0012A385 (linear 0x0013A385) | 0x001BF9D9 |
| 163 | 1:0x0012A395 (linear 0x0013A395) | 0x001BF9E9 |
| 164 | 1:0x0012A3A6 (linear 0x0013A3A6) | 0x001BF9FA |
| 166 | 1:0x0012A3BA (linear 0x0013A3BA) | 0x001BFA0E |
| 168 | 1:0x0012A3DC (linear 0x0013A3DC) | 0x001BFA30 |
| 169 | 1:0x0012A3DF (linear 0x0013A3DF) | 0x001BFA33 |
| 171 | 1:0x0012A3F3 (linear 0x0013A3F3) | 0x001BFA47 |
| 173 | 1:0x0012A3F5 (linear 0x0013A3F5) | 0x001BFA49 |
| 174 | 1:0x0012A40B (linear 0x0013A40B) | 0x001BFA5F |
| 175 | 1:0x0012A42C (linear 0x0013A42C) | 0x001BFA80 |
| 177 | 1:0x0012A44D (linear 0x0013A44D) | 0x001BFAA1 |
| 178 | 1:0x0012A45E (linear 0x0013A45E) | 0x001BFAB2 |
| 180 | 1:0x0012A46C (linear 0x0013A46C) | 0x001BFAC0 |
| 181 | 1:0x0012A482 (linear 0x0013A482) | 0x001BFAD6 |
| 183 | 1:0x0012A49C (linear 0x0013A49C) | 0x001BFAF0 |
| 184 | 1:0x0012A4A3 (linear 0x0013A4A3) | 0x001BFAF7 |
| 186 | 1:0x0012A4A9 (linear 0x0013A4A9) | 0x001BFAFD |
| 190 | 1:0x0012A4CA (linear 0x0013A4CA) | 0x001BFB1E |
| 192 | 1:0x0012A4FD (linear 0x0013A4FD) | 0x001BFB51 |
| 193 | 1:0x0012A504 (linear 0x0013A504) | 0x001BFB58 |
| 195 | 1:0x0012A52A (linear 0x0013A52A) | 0x001BFB7E |
| 196 | 1:0x0012A531 (linear 0x0013A531) | 0x001BFB85 |
| 198 | 1:0x0012A54F (linear 0x0013A54F) | 0x001BFBA3 |
| 200 | 1:0x0012A57F (linear 0x0013A57F) | 0x001BFBD3 |
| 201 | 1:0x0012A59B (linear 0x0013A59B) | 0x001BFBEF |
| 203 | 1:0x0012A5A2 (linear 0x0013A5A2) | 0x001BFBF6 |
| 204 | 1:0x0012A5A4 (linear 0x0013A5A4) | 0x001BFBF8 |
| 205 | 1:0x0012A5A9 (linear 0x0013A5A9) | 0x001BFBFD |
| 207 | 1:0x0012A5AE (linear 0x0013A5AE) | 0x001BFC02 |
| 208 | 1:0x0012A5B5 (linear 0x0013A5B5) | 0x001BFC09 |
| 210 | 1:0x0012A5DB (linear 0x0013A5DB) | 0x001BFC2F |
| 211 | 1:0x0012A5E2 (linear 0x0013A5E2) | 0x001BFC36 |
| 213 | 1:0x0012A600 (linear 0x0013A600) | 0x001BFC54 |
| 215 | 1:0x0012A630 (linear 0x0013A630) | 0x001BFC84 |
| 216 | 1:0x0012A64C (linear 0x0013A64C) | 0x001BFCA0 |
| 218 | 1:0x0012A653 (linear 0x0013A653) | 0x001BFCA7 |
| 219 | 1:0x0012A655 (linear 0x0013A655) | 0x001BFCA9 |
| 220 | 1:0x0012A65A (linear 0x0013A65A) | 0x001BFCAE |
| 222 | 1:0x0012A65F (linear 0x0013A65F) | 0x001BFCB3 |
| 223 | 1:0x0012A666 (linear 0x0013A666) | 0x001BFCBA |
| 225 | 1:0x0012A68C (linear 0x0013A68C) | 0x001BFCE0 |
| 226 | 1:0x0012A693 (linear 0x0013A693) | 0x001BFCE7 |
| 228 | 1:0x0012A6B1 (linear 0x0013A6B1) | 0x001BFD05 |
| 230 | 1:0x0012A6E1 (linear 0x0013A6E1) | 0x001BFD35 |
| 231 | 1:0x0012A6FD (linear 0x0013A6FD) | 0x001BFD51 |
| 233 | 1:0x0012A704 (linear 0x0013A704) | 0x001BFD58 |
| 234 | 1:0x0012A706 (linear 0x0013A706) | 0x001BFD5A |
| 239 | 1:0x0012A70B (linear 0x0013A70B) | 0x001BFD5F |
| 242 | 1:0x0012A711 (linear 0x0013A711) | 0x001BFD65 |
| 246 | 1:0x0012A72C (linear 0x0013A72C) | 0x001BFD80 |
| 247 | 1:0x0012A749 (linear 0x0013A749) | 0x001BFD9D |
| 248 | 1:0x0012A766 (linear 0x0013A766) | 0x001BFDBA |
| 250 | 1:0x0012A783 (linear 0x0013A783) | 0x001BFDD7 |
| 251 | 1:0x0012A79E (linear 0x0013A79E) | 0x001BFDF2 |
| 253 | 1:0x0012A7B9 (linear 0x0013A7B9) | 0x001BFE0D |
| 254 | 1:0x0012A7C0 (linear 0x0013A7C0) | 0x001BFE14 |
| 258 | 1:0x0012A7CC (linear 0x0013A7CC) | 0x001BFE20 |
| 262 | 1:0x0012A7E9 (linear 0x0013A7E9) | 0x001BFE3D |
| 264 | 1:0x0012A815 (linear 0x0013A815) | 0x001BFE69 |
| 265 | 1:0x0012A81E (linear 0x0013A81E) | 0x001BFE72 |
| 266 | 1:0x0012A828 (linear 0x0013A828) | 0x001BFE7C |
| 268 | 1:0x0012A831 (linear 0x0013A831) | 0x001BFE85 |
| 269 | 1:0x0012A83E (linear 0x0013A83E) | 0x001BFE92 |
| 271 | 1:0x0012A847 (linear 0x0013A847) | 0x001BFE9B |
| 273 | 1:0x0012A84F (linear 0x0013A84F) | 0x001BFEA3 |
| 278 | 1:0x0012A867 (linear 0x0013A867) | 0x001BFEBB |
| 280 | 1:0x0012A87C (linear 0x0013A87C) | 0x001BFED0 |
| 281 | 1:0x0012A8A0 (linear 0x0013A8A0) | 0x001BFEF4 |
| 282 | 1:0x0012A8C2 (linear 0x0013A8C2) | 0x001BFF16 |
| 285 | 1:0x0012A8DA (linear 0x0013A8DA) | 0x001BFF2E |
| 286 | 1:0x0012A8F7 (linear 0x0013A8F7) | 0x001BFF4B |
| 287 | 1:0x0012A914 (linear 0x0013A914) | 0x001BFF68 |
| 289 | 1:0x0012A931 (linear 0x0013A931) | 0x001BFF85 |
| 291 | 1:0x0012A93C (linear 0x0013A93C) | 0x001BFF90 |
| 292 | 1:0x0012A944 (linear 0x0013A944) | 0x001BFF98 |
| 294 | 1:0x0012A94A (linear 0x0013A94A) | 0x001BFF9E |
| 296 | 1:0x0012A955 (linear 0x0013A955) | 0x001BFFA9 |
| 297 | 1:0x0012A95D (linear 0x0013A95D) | 0x001BFFB1 |
| 299 | 1:0x0012A963 (linear 0x0013A963) | 0x001BFFB7 |
| 301 | 1:0x0012A96E (linear 0x0013A96E) | 0x001BFFC2 |
| 302 | 1:0x0012A976 (linear 0x0013A976) | 0x001BFFCA |
| 304 | 1:0x0012A97C (linear 0x0013A97C) | 0x001BFFD0 |
| 306 | 1:0x0012A981 (linear 0x0013A981) | 0x001BFFD5 |
| 308 | 1:0x0012A987 (linear 0x0013A987) | 0x001BFFDB |
| 310 | 1:0x0012A993 (linear 0x0013A993) | 0x001BFFE7 |
| 315 | 1:0x0012A9B0 (linear 0x0013A9B0) | 0x001C0004 |
| 316 | 1:0x0012A9BE (linear 0x0013A9BE) | 0x001C0012 |
| 317 | 1:0x0012A9CC (linear 0x0013A9CC) | 0x001C0020 |
| 319 | 1:0x0012A9DA (linear 0x0013A9DA) | 0x001C002E |
| 321 | 1:0x0012A9FC (linear 0x0013A9FC) | 0x001C0050 |
| 323 | 1:0x0012AA0C (linear 0x0013AA0C) | 0x001C0060 |
| 324 | 1:0x0012AA1C (linear 0x0013AA1C) | 0x001C0070 |
| 325 | 1:0x0012AA30 (linear 0x0013AA30) | 0x001C0084 |
| 327 | 1:0x0012AA44 (linear 0x0013AA44) | 0x001C0098 |
| 328 | 1:0x0012AA54 (linear 0x0013AA54) | 0x001C00A8 |
| 329 | 1:0x0012AA68 (linear 0x0013AA68) | 0x001C00BC |
| 331 | 1:0x0012AA7C (linear 0x0013AA7C) | 0x001C00D0 |
| 332 | 1:0x0012AA8C (linear 0x0013AA8C) | 0x001C00E0 |
| 333 | 1:0x0012AAA0 (linear 0x0013AAA0) | 0x001C00F4 |
| 334 | 1:0x0012AAB4 (linear 0x0013AAB4) | 0x001C0108 |
| 335 | 1:0x0012AAB9 (linear 0x0013AAB9) | 0x001C010D |
| 340 | 1:0x0012AAC1 (linear 0x0013AAC1) | 0x001C0115 |
| 342 | 1:0x0012AAD9 (linear 0x0013AAD9) | 0x001C012D |
| 343 | 1:0x0012AAE2 (linear 0x0013AAE2) | 0x001C0136 |
| 345 | 1:0x0012AAE9 (linear 0x0013AAE9) | 0x001C013D |
| 350 | 1:0x0012AB04 (linear 0x0013AB04) | 0x001C0158 |
| 352 | 1:0x0012AB0C (linear 0x0013AB0C) | 0x001C0160 |
| 353 | 1:0x0012AB1F (linear 0x0013AB1F) | 0x001C0173 |
| 355 | 1:0x0012AB29 (linear 0x0013AB29) | 0x001C017D |
| 358 | 1:0x0012AB48 (linear 0x0013AB48) | 0x001C019C |
| 360 | 1:0x0012AB53 (linear 0x0013AB53) | 0x001C01A7 |
| 362 | 1:0x0012AB76 (linear 0x0013AB76) | 0x001C01CA |
| 363 | 1:0x0012AB7F (linear 0x0013AB7F) | 0x001C01D3 |
| 364 | 1:0x0012AB9B (linear 0x0013AB9B) | 0x001C01EF |
| 365 | 1:0x0012ABB7 (linear 0x0013ABB7) | 0x001C020B |
| 366 | 1:0x0012ABD3 (linear 0x0013ABD3) | 0x001C0227 |
| 368 | 1:0x0012ABDA (linear 0x0013ABDA) | 0x001C022E |
| 369 | 1:0x0012ABE9 (linear 0x0013ABE9) | 0x001C023D |
| 370 | 1:0x0012ABF3 (linear 0x0013ABF3) | 0x001C0247 |
| 371 | 1:0x0012ABF8 (linear 0x0013ABF8) | 0x001C024C |
| 373 | 1:0x0012AC01 (linear 0x0013AC01) | 0x001C0255 |
| 377 | 1:0x0012AC1E (linear 0x0013AC1E) | 0x001C0272 |
| 378 | 1:0x0012AC2A (linear 0x0013AC2A) | 0x001C027E |
| 380 | 1:0x0012AC43 (linear 0x0013AC43) | 0x001C0297 |
| 382 | 1:0x0012AC60 (linear 0x0013AC60) | 0x001C02B4 |
| 383 | 1:0x0012AC74 (linear 0x0013AC74) | 0x001C02C8 |
| 384 | 1:0x0012AC94 (linear 0x0013AC94) | 0x001C02E8 |
| 385 | 1:0x0012ACB4 (linear 0x0013ACB4) | 0x001C0308 |
| 386 | 1:0x0012ACD4 (linear 0x0013ACD4) | 0x001C0328 |
| 387 | 1:0x0012ACD6 (linear 0x0013ACD6) | 0x001C032A |

