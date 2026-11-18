# F:\WLIBS\cache\buffer.c

Module 156 of the debug info of ORION2.EXE.

Ranges: 1:0x001178A3 (linear 0x001278A3), 4,014 bytes, 2:0x000027EC (linear 0x001727EC), 124 bytes, 2:0x0000C580 (linear 0x0017C580), 4 bytes, 2:0x00044280 (linear 0x001B4280), 20 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x001178A3 (linear 0x001278A3) | code | Flush_Buffer_Space_ | `regions/07-object-1/001ACE54-001AD653.txt` |
| 1:0x0011792B (linear 0x0012792B) | code | Get_Remaining_Buffer_Space_ | `regions/07-object-1/001ACE54-001AD653.txt` |
| 1:0x00117969 (linear 0x00127969) | code | Free_Buffer_Space_ | `regions/07-object-1/001ACE54-001AD653.txt` |
| 1:0x001179AF (linear 0x001279AF) | code | Allocate_Buffer_Space_ | `regions/07-object-1/001ACE54-001AD653.txt` |
| 1:0x00117B43 (linear 0x00127B43) | code | Clear_Buffer_Reload_ | `regions/07-object-1/001ACE54-001AD653.txt` |
| 1:0x00117C27 (linear 0x00127C27) | code | Buffer_Reload_ | `regions/07-object-1/001ACE54-001AD653.txt` |
| 1:0x0011816C (linear 0x0012816C) | code | Farload_Buffer_Library_ | `regions/07-object-1/001AD654-001ADE53.txt` |
| 1:0x0011857C (linear 0x0012857C) | code | Get_Next_Buffer_Address_ | `regions/07-object-1/001AD654-001ADE53.txt` |
| 1:0x001186BB (linear 0x001286BB) | code | Get_Lowest_Priority_ | `regions/07-object-1/001AD654-001ADE53.txt` |
| 1:0x001187CB (linear 0x001287CB) | code | Check_For_Valid_Segs_ | `regions/07-object-1/001AD654-001ADE53.txt` |
| 2:0x0000C580 (linear 0x0017C580) | data | _global_load_count | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x00044280 (linear 0x001B4280) | data | _buffer_space_ptr | - |
| 2:0x00044284 (linear 0x001B4284) | data | _buffer_space_size | - |
| 2:0x00044288 (linear 0x001B4288) | data | _buffer_space_seg | - |
| 2:0x0004428C (linear 0x001B428C) | data | _file_buffer | - |
| 2:0x00044290 (linear 0x001B4290) | data | _file_buffer_count | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x00044280 (linear 0x001B4280) | buffer_space_ptr | char * |
| 2:0x00044284 (linear 0x001B4284) | buffer_space_size | int |
| 2:0x0000C580 (linear 0x0017C580) | global_load_count | unsigned int |
| 2:0x00044288 (linear 0x001B4288) | buffer_space_seg | int |
| 2:0x0004428C (linear 0x001B428C) | file_buffer | struct s_file_buffer * |
| 2:0x00044290 (linear 0x001B4290) | file_buffer_count | short |

#### Flush_Buffer_Space

- Address: 1:0x001178A3 (linear 0x001278A3), 136 bytes, in `regions/07-object-1/001ACE54-001AD653.txt`
- Type: unsigned int (char *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| base_seg | [EBP-0x10] | char * |
| i | [EBP-0xC] | int |
| j | [EBP-0x8] | int |

#### Get_Remaining_Buffer_Space

- Address: 1:0x0011792B (linear 0x0012792B), 62 bytes, in `regions/07-object-1/001ACE54-001AD653.txt`
- Type: unsigned int (char *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| base_seg | [EBP-0x10] | char * |
| header | [EBP-0xC] | long * |
| block_header | [EBP-0x8] | long * |

#### Free_Buffer_Space

- Address: 1:0x00117969 (linear 0x00127969), 70 bytes, in `regions/07-object-1/001ACE54-001AD653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Allocate_Buffer_Space

- Address: 1:0x001179AF (linear 0x001279AF), 404 bytes, in `regions/07-object-1/001ACE54-001AD653.txt`
- Type: char * (int); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| size | [EBP-0x1C] | int |
| name | [EBP-0x18] | char * |
| header | [EBP-0x14] | long * |
| lsize | [EBP-0x10] | int |
| i | [EBP-0xC] | int |
| j | [EBP-0x8] | int |

#### Clear_Buffer_Reload

- Address: 1:0x00117B43 (linear 0x00127B43), 228 bytes, in `regions/07-object-1/001ACE54-001AD653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x8] | int |
| j | [EBP-0x4] | int |

#### Buffer_Reload

- Address: 1:0x00117C27 (linear 0x00127C27), 1,349 bytes, in `regions/07-object-1/001ACE54-001AD653.txt`
- Type: char * (char *, int, char *); returns in EAX
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x20] | char * |
| entry_num | [EBP-0x1C] | int |
| base_seg | [EBP-0x18] | char * |
| i | [EBP-0x14] | int |
| file_found | [EBP-0x4] | char |
| file_number | [EBP-0x8] | char |
| check_buffer | [EBP-0xC] | long * |

#### Farload_Buffer_Library

- Address: 1:0x0011816C (linear 0x0012816C), 1,040 bytes, in `regions/07-object-1/001AD654-001ADE53.txt`
- Type: char * (char *, int, char *, int); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x50] | char * |
| entry_num | [EBP-0x38] | int |
| base_seg | [EBP-0x4C] | char * |
| allocation_type | [EBP-0x48] | int |
| entry_start | [EBP-0x44] | long |
| entry_end | [EBP-0x40] | long |
| entry_length | [EBP-0x3C] | long |
| full_file_path | [EBP-0x8C] | char[60] |
| pointer | [EBP-0x34] | char * |
| deleted_count | [EBP-0x30] | int |
| delete_file_buffer | [EBP-0x2C] | int |
| delete_picture_buffer | [EBP-0x28] | int |
| next_file_buffer | [EBP-0x24] | int |
| next_picture_buffer | [EBP-0x20] | int |
| deleted_block_size | [EBP-0x1C] | int |
| moved_block_size | [EBP-0x18] | int |
| i | [EBP-0x14] | int |
| j | [EBP-0x10] | int |
| last_address | [EBP-0xC] | char * |
| block_header | [EBP-0x8] | long * |

#### Get_Next_Buffer_Address

- Address: 1:0x0011857C (linear 0x0012857C), 319 bytes, in `regions/07-object-1/001AD654-001ADE53.txt`
- Type: void (int, int, int *, int *, char *)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| file_buffer_number | [EBP-0x20] | int |
| picture_buffer_number | [EBP-0x1C] | int |
| next_file_buffer | [EBP-0x18] | int * |
| next_picture_buffer | [EBP-0x14] | int * |
| block | [EBP+0x10] | char * |
| i | [EBP-0x10] | int |
| j | [EBP-0xC] | int |
| next_address | [EBP-0x8] | char * |
| block_header | [EBP-0x4] | long * |

#### Get_Lowest_Priority

- Address: 1:0x001186BB (linear 0x001286BB), 272 bytes, in `regions/07-object-1/001AD654-001ADE53.txt`
- Type: void (int *, int *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| file_buffer_number | [EBP-0x18] | int * |
| picture_buffer_number | [EBP-0x14] | int * |
| i | [EBP-0x10] | int |
| j | [EBP-0xC] | int |
| lowest_priority | [EBP-0x8] | unsigned int |
| lowest_seg | [EBP-0x4] | char * |

#### Check_For_Valid_Segs

- Address: 1:0x001187CB (linear 0x001287CB), 134 bytes, in `regions/07-object-1/001AD654-001ADE53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| j | [EBP-0x8] | int |
| fucked_up | [EBP-0x4] | char |

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
| 13 | typedef | wchar_t | name for unsigned short (#7) |
| 14 | typedef | wchar_t | name for unsigned short (#7) |
| 15 | scalar | void | void 1 bytes |
| 16 | procedure |  | near386; returns void () |
| 17 | pointer | onexit_t | near386; of void (void) (#16) |
| 18 | typedef | onexit_t | name for void (void) * (#17) |
| 19 | pointer |  | near386; of char (#10) |
| 20 | array | va_list | 0..0; of char * (#19) |
| 21 | typedef | va_list | name for char *[1] (#20) |
| 22 | array | __va_list | 0..0; of char * (#19) |
| 23 | typedef | __va_list | name for char *[1] (#22) |
| 24 | scalar | long | int 4 bytes |
| 25 | typedef | fpos_t | name for long (#24) |
| 26 | typedef | fpos_t | name for long (#24) |
| 27 | typedef | BYTE | name for unsigned char (#5) |
| 28 | typedef | SBYTE | name for signed char (#4) |
| 29 | typedef | UBYTE | name for unsigned char (#5) |
| 30 | typedef | WORD | name for unsigned short (#7) |
| 31 | typedef | SWORD | name for short (#6) |
| 32 | typedef | UWORD | name for unsigned short (#7) |
| 33 | typedef | DWORD | name for unsigned int (#9) |
| 34 | typedef | SDWORD | name for int (#8) |
| 35 | typedef | UDWORD | name for unsigned int (#9) |
| 36 | typedef | BOOL | name for unsigned char (#5) |
| 37 | typedef | SampleHandle | name for int (#8) |
| 38 | scalar | unsigned long | unsigned 4 bytes |
| 39 | typedef | time_t | name for unsigned long (#38) |
| 40 | typedef | clock_t | name for unsigned long (#38) |
| 41 | typedef | time_t | name for unsigned long (#38) |
| 42 | array |  | 0..15; of char (#10) |
| 43 | struct | struct s_picture | 4 fields, 11 bytes |
| 44 | typedef | s_picture | struct name for struct #43 (#43) |
| 45 | array |  | 0..409; of struct s_picture (#44) |
| 46 | struct | struct s_file_buffer | 3 fields, 4530 bytes |
| 47 | typedef | s_file_buffer | struct name for struct #46 (#46) |
| 48 | pointer |  | near386; of struct s_file_buffer (#47) |
| 49 | procedure |  | near386; returns unsigned int (char *) |
| 50 | pointer |  | near386; of long (#24) |
| 51 | procedure |  | near386; returns void (void) |
| 52 | procedure |  | near386; returns char * (int) |
| 53 | procedure |  | near386; returns char * (char *, int, char *) |
| 54 | procedure |  | near386; returns char * (char *, int, char *, int) |
| 55 | array |  | 0..59; of char (#10) |
| 56 | pointer |  | near386; of int (#8) |
| 57 | procedure |  | near386; returns void (int, int, int *, int *, char *) |
| 58 | procedure |  | near386; returns void (int *, int *) |

##### struct s_picture (#43, 4 fields, 11 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| loaded_flag | offset 0 | char |
| seg_number | offset 1 | short |
| load_count | offset 3 | unsigned int |
| seg_address | offset 7 | char * |

##### struct s_file_buffer (#46, 3 fields, 4530 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| lbx_name | offset 0 | char[16] |
| picture | offset 16 | struct s_picture[410] |
| picture_count | offset 4526 | int |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 84 | 1:0x001178A3 (linear 0x001278A3) | 0x001ACEF7 |
| 87 | 1:0x001178BE (linear 0x001278BE) | 0x001ACF12 |
| 89 | 1:0x001178D5 (linear 0x001278D5) | 0x001ACF29 |
| 91 | 1:0x001178EF (linear 0x001278EF) | 0x001ACF43 |
| 92 | 1:0x0011790A (linear 0x0012790A) | 0x001ACF5E |
| 93 | 1:0x0011790C (linear 0x0012790C) | 0x001ACF60 |
| 94 | 1:0x0011790E (linear 0x0012790E) | 0x001ACF62 |
| 95 | 1:0x00117918 (linear 0x00127918) | 0x001ACF6C |
| 96 | 1:0x0011791F (linear 0x0012791F) | 0x001ACF73 |
| 97 | 1:0x0011792B (linear 0x0012792B) | 0x001ACF7F |
| 102 | 1:0x00117946 (linear 0x00127946) | 0x001ACF9A |
| 103 | 1:0x0011794F (linear 0x0012794F) | 0x001ACFA3 |
| 104 | 1:0x0011795D (linear 0x0012795D) | 0x001ACFB1 |
| 105 | 1:0x00117969 (linear 0x00127969) | 0x001ACFBD |
| 107 | 1:0x00117981 (linear 0x00127981) | 0x001ACFD5 |
| 109 | 1:0x0011798A (linear 0x0012798A) | 0x001ACFDE |
| 110 | 1:0x00117994 (linear 0x00127994) | 0x001ACFE8 |
| 111 | 1:0x0011799E (linear 0x0012799E) | 0x001ACFF2 |
| 113 | 1:0x001179A8 (linear 0x001279A8) | 0x001ACFFC |
| 114 | 1:0x001179AF (linear 0x001279AF) | 0x001AD003 |
| 122 | 1:0x001179CA (linear 0x001279CA) | 0x001AD01E |
| 124 | 1:0x001179D6 (linear 0x001279D6) | 0x001AD02A |
| 126 | 1:0x001179E0 (linear 0x001279E0) | 0x001AD034 |
| 128 | 1:0x001179E2 (linear 0x001279E2) | 0x001AD036 |
| 129 | 1:0x001179E9 (linear 0x001279E9) | 0x001AD03D |
| 132 | 1:0x001179EF (linear 0x001279EF) | 0x001AD043 |
| 135 | 1:0x00117A07 (linear 0x00127A07) | 0x001AD05B |
| 138 | 1:0x00117A15 (linear 0x00127A15) | 0x001AD069 |
| 140 | 1:0x00117A23 (linear 0x00127A23) | 0x001AD077 |
| 142 | 1:0x00117A29 (linear 0x00127A29) | 0x001AD07D |
| 143 | 1:0x00117A32 (linear 0x00127A32) | 0x001AD086 |
| 144 | 1:0x00117A3B (linear 0x00127A3B) | 0x001AD08F |
| 146 | 1:0x00117A45 (linear 0x00127A45) | 0x001AD099 |
| 148 | 1:0x00117A54 (linear 0x00127A54) | 0x001AD0A8 |
| 150 | 1:0x00117A67 (linear 0x00127A67) | 0x001AD0BB |
| 152 | 1:0x00117A70 (linear 0x00127A70) | 0x001AD0C4 |
| 154 | 1:0x00117A8A (linear 0x00127A8A) | 0x001AD0DE |
| 156 | 1:0x00117AA4 (linear 0x00127AA4) | 0x001AD0F8 |
| 157 | 1:0x00117ABC (linear 0x00127ABC) | 0x001AD110 |
| 158 | 1:0x00117AD8 (linear 0x00127AD8) | 0x001AD12C |
| 159 | 1:0x00117AF3 (linear 0x00127AF3) | 0x001AD147 |
| 160 | 1:0x00117AF5 (linear 0x00127AF5) | 0x001AD149 |
| 161 | 1:0x00117B0E (linear 0x00127B0E) | 0x001AD162 |
| 164 | 1:0x00117B13 (linear 0x00127B13) | 0x001AD167 |
| 165 | 1:0x00117B1B (linear 0x00127B1B) | 0x001AD16F |
| 166 | 1:0x00117B26 (linear 0x00127B26) | 0x001AD17A |
| 171 | 1:0x00117B2E (linear 0x00127B2E) | 0x001AD182 |
| 172 | 1:0x00117B37 (linear 0x00127B37) | 0x001AD18B |
| 174 | 1:0x00117B43 (linear 0x00127B43) | 0x001AD197 |
| 178 | 1:0x00117B5B (linear 0x00127B5B) | 0x001AD1AF |
| 180 | 1:0x00117B75 (linear 0x00127B75) | 0x001AD1C9 |
| 182 | 1:0x00117B8F (linear 0x00127B8F) | 0x001AD1E3 |
| 183 | 1:0x00117BA7 (linear 0x00127BA7) | 0x001AD1FB |
| 184 | 1:0x00117BC3 (linear 0x00127BC3) | 0x001AD217 |
| 185 | 1:0x00117BDE (linear 0x00127BDE) | 0x001AD232 |
| 186 | 1:0x00117BE0 (linear 0x00127BE0) | 0x001AD234 |
| 187 | 1:0x00117BF8 (linear 0x00127BF8) | 0x001AD24C |
| 188 | 1:0x00117C0F (linear 0x00127C0F) | 0x001AD263 |
| 189 | 1:0x00117C14 (linear 0x00127C14) | 0x001AD268 |
| 191 | 1:0x00117C1E (linear 0x00127C1E) | 0x001AD272 |
| 193 | 1:0x00117C27 (linear 0x00127C27) | 0x001AD27B |
| 201 | 1:0x00117C46 (linear 0x00127C46) | 0x001AD29A |
| 203 | 1:0x00117C4F (linear 0x00127C4F) | 0x001AD2A3 |
| 205 | 1:0x00117C5A (linear 0x00127C5A) | 0x001AD2AE |
| 208 | 1:0x00117C64 (linear 0x00127C64) | 0x001AD2B8 |
| 210 | 1:0x00117C72 (linear 0x00127C72) | 0x001AD2C6 |
| 215 | 1:0x00117C7C (linear 0x00127C7C) | 0x001AD2D0 |
| 217 | 1:0x00117C80 (linear 0x00127C80) | 0x001AD2D4 |
| 219 | 1:0x00117C9E (linear 0x00127C9E) | 0x001AD2F2 |
| 221 | 1:0x00117CB9 (linear 0x00127CB9) | 0x001AD30D |
| 222 | 1:0x00117CBD (linear 0x00127CBD) | 0x001AD311 |
| 223 | 1:0x00117CC3 (linear 0x00127CC3) | 0x001AD317 |
| 225 | 1:0x00117CC5 (linear 0x00127CC5) | 0x001AD319 |
| 226 | 1:0x00117CC7 (linear 0x00127CC7) | 0x001AD31B |
| 228 | 1:0x00117CD5 (linear 0x00127CD5) | 0x001AD329 |
| 230 | 1:0x00117D07 (linear 0x00127D07) | 0x001AD35B |
| 232 | 1:0x00117D2F (linear 0x00127D2F) | 0x001AD383 |
| 234 | 1:0x00117D55 (linear 0x00127D55) | 0x001AD3A9 |
| 235 | 1:0x00117D5B (linear 0x00127D5B) | 0x001AD3AF |
| 236 | 1:0x00117D7C (linear 0x00127D7C) | 0x001AD3D0 |
| 240 | 1:0x00117DA0 (linear 0x00127DA0) | 0x001AD3F4 |
| 241 | 1:0x00117DA6 (linear 0x00127DA6) | 0x001AD3FA |
| 242 | 1:0x00117DC7 (linear 0x00127DC7) | 0x001AD41B |
| 243 | 1:0x00117DF6 (linear 0x00127DF6) | 0x001AD44A |
| 244 | 1:0x00117E13 (linear 0x00127E13) | 0x001AD467 |
| 247 | 1:0x00117E37 (linear 0x00127E37) | 0x001AD48B |
| 249 | 1:0x00117E3C (linear 0x00127E3C) | 0x001AD490 |
| 251 | 1:0x00117E5C (linear 0x00127E5C) | 0x001AD4B0 |
| 253 | 1:0x00117E66 (linear 0x00127E66) | 0x001AD4BA |
| 254 | 1:0x00117E6C (linear 0x00127E6C) | 0x001AD4C0 |
| 255 | 1:0x00117EA0 (linear 0x00127EA0) | 0x001AD4F4 |
| 256 | 1:0x00117ED8 (linear 0x00127ED8) | 0x001AD52C |
| 257 | 1:0x00117F1D (linear 0x00127F1D) | 0x001AD571 |
| 258 | 1:0x00117F4F (linear 0x00127F4F) | 0x001AD5A3 |
| 259 | 1:0x00117F68 (linear 0x00127F68) | 0x001AD5BC |
| 262 | 1:0x00117FA5 (linear 0x00127FA5) | 0x001AD5F9 |
| 264 | 1:0x00117FB3 (linear 0x00127FB3) | 0x001AD607 |
| 266 | 1:0x00117FBD (linear 0x00127FBD) | 0x001AD611 |
| 267 | 1:0x00117FC3 (linear 0x00127FC3) | 0x001AD617 |
| 268 | 1:0x00117FE0 (linear 0x00127FE0) | 0x001AD634 |
| 269 | 1:0x00118000 (linear 0x00128000) | 0x001AD654 |
| 270 | 1:0x0011803B (linear 0x0012803B) | 0x001AD68F |
| 271 | 1:0x0011807A (linear 0x0012807A) | 0x001AD6CE |
| 272 | 1:0x001180C7 (linear 0x001280C7) | 0x001AD71B |
| 273 | 1:0x00118100 (linear 0x00128100) | 0x001AD754 |
| 274 | 1:0x0011811C (linear 0x0012811C) | 0x001AD770 |
| 275 | 1:0x00118123 (linear 0x00128123) | 0x001AD777 |
| 276 | 1:0x00118162 (linear 0x00128162) | 0x001AD7B6 |
| 278 | 1:0x0011816C (linear 0x0012816C) | 0x001AD7C0 |
| 295 | 1:0x0011818D (linear 0x0012818D) | 0x001AD7E1 |
| 297 | 1:0x001181A3 (linear 0x001281A3) | 0x001AD7F7 |
| 299 | 1:0x001181C1 (linear 0x001281C1) | 0x001AD815 |
| 301 | 1:0x001181D4 (linear 0x001281D4) | 0x001AD828 |
| 302 | 1:0x001181E1 (linear 0x001281E1) | 0x001AD835 |
| 304 | 1:0x001181F3 (linear 0x001281F3) | 0x001AD847 |
| 306 | 1:0x001181FC (linear 0x001281FC) | 0x001AD850 |
| 307 | 1:0x00118205 (linear 0x00128205) | 0x001AD859 |
| 309 | 1:0x00118215 (linear 0x00128215) | 0x001AD869 |
| 310 | 1:0x00118225 (linear 0x00128225) | 0x001AD879 |
| 311 | 1:0x00118235 (linear 0x00128235) | 0x001AD889 |
| 313 | 1:0x0011824A (linear 0x0012824A) | 0x001AD89E |
| 314 | 1:0x00118253 (linear 0x00128253) | 0x001AD8A7 |
| 316 | 1:0x00118263 (linear 0x00128263) | 0x001AD8B7 |
| 318 | 1:0x0011827D (linear 0x0012827D) | 0x001AD8D1 |
| 319 | 1:0x0011828C (linear 0x0012828C) | 0x001AD8E0 |
| 321 | 1:0x0011829C (linear 0x0012829C) | 0x001AD8F0 |
| 324 | 1:0x001182A6 (linear 0x001282A6) | 0x001AD8FA |
| 325 | 1:0x001182B3 (linear 0x001282B3) | 0x001AD907 |
| 327 | 1:0x001182C3 (linear 0x001282C3) | 0x001AD917 |
| 328 | 1:0x001182D2 (linear 0x001282D2) | 0x001AD926 |
| 329 | 1:0x001182E1 (linear 0x001282E1) | 0x001AD935 |
| 331 | 1:0x001182EA (linear 0x001282EA) | 0x001AD93E |
| 334 | 1:0x001182F9 (linear 0x001282F9) | 0x001AD94D |
| 335 | 1:0x00118300 (linear 0x00128300) | 0x001AD954 |
| 337 | 1:0x00118307 (linear 0x00128307) | 0x001AD95B |
| 339 | 1:0x00118310 (linear 0x00128310) | 0x001AD964 |
| 340 | 1:0x0011831E (linear 0x0012831E) | 0x001AD972 |
| 342 | 1:0x00118331 (linear 0x00128331) | 0x001AD985 |
| 343 | 1:0x0011833F (linear 0x0012833F) | 0x001AD993 |
| 345 | 1:0x00118349 (linear 0x00128349) | 0x001AD99D |
| 346 | 1:0x0011834C (linear 0x0012834C) | 0x001AD9A0 |
| 347 | 1:0x00118357 (linear 0x00128357) | 0x001AD9AB |
| 349 | 1:0x0011836C (linear 0x0012836C) | 0x001AD9C0 |
| 351 | 1:0x00118375 (linear 0x00128375) | 0x001AD9C9 |
| 352 | 1:0x0011837B (linear 0x0012837B) | 0x001AD9CF |
| 354 | 1:0x00118383 (linear 0x00128383) | 0x001AD9D7 |
| 356 | 1:0x0011838C (linear 0x0012838C) | 0x001AD9E0 |
| 358 | 1:0x001183A9 (linear 0x001283A9) | 0x001AD9FD |
| 360 | 1:0x001183AB (linear 0x001283AB) | 0x001AD9FF |
| 364 | 1:0x001183DE (linear 0x001283DE) | 0x001ADA32 |
| 366 | 1:0x001183E7 (linear 0x001283E7) | 0x001ADA3B |
| 368 | 1:0x001183EE (linear 0x001283EE) | 0x001ADA42 |
| 370 | 1:0x001183F0 (linear 0x001283F0) | 0x001ADA44 |
| 376 | 1:0x0011840D (linear 0x0012840D) | 0x001ADA61 |
| 401 | 1:0x00118414 (linear 0x00128414) | 0x001ADA68 |
| 404 | 1:0x00118421 (linear 0x00128421) | 0x001ADA75 |
| 405 | 1:0x00118457 (linear 0x00128457) | 0x001ADAAB |
| 407 | 1:0x00118478 (linear 0x00128478) | 0x001ADACC |
| 409 | 1:0x001184A5 (linear 0x001284A5) | 0x001ADAF9 |
| 411 | 1:0x001184FA (linear 0x001284FA) | 0x001ADB4E |
| 413 | 1:0x00118514 (linear 0x00128514) | 0x001ADB68 |
| 414 | 1:0x00118516 (linear 0x00128516) | 0x001ADB6A |
| 418 | 1:0x0011851B (linear 0x0012851B) | 0x001ADB6F |
| 419 | 1:0x00118534 (linear 0x00128534) | 0x001ADB88 |
| 423 | 1:0x0011853C (linear 0x0012853C) | 0x001ADB90 |
| 426 | 1:0x00118541 (linear 0x00128541) | 0x001ADB95 |
| 428 | 1:0x00118547 (linear 0x00128547) | 0x001ADB9B |
| 432 | 1:0x00118557 (linear 0x00128557) | 0x001ADBAB |
| 435 | 1:0x0011856D (linear 0x0012856D) | 0x001ADBC1 |
| 436 | 1:0x00118573 (linear 0x00128573) | 0x001ADBC7 |
| 438 | 1:0x0011857C (linear 0x0012857C) | 0x001ADBD0 |
| 445 | 1:0x0011859D (linear 0x0012859D) | 0x001ADBF1 |
| 447 | 1:0x001185A6 (linear 0x001285A6) | 0x001ADBFA |
| 448 | 1:0x001185AC (linear 0x001285AC) | 0x001ADC00 |
| 449 | 1:0x001185B4 (linear 0x001285B4) | 0x001ADC08 |
| 451 | 1:0x001185BD (linear 0x001285BD) | 0x001ADC11 |
| 453 | 1:0x001185DE (linear 0x001285DE) | 0x001ADC32 |
| 455 | 1:0x0011860B (linear 0x0012860B) | 0x001ADC5F |
| 457 | 1:0x0011867E (linear 0x0012867E) | 0x001ADCD2 |
| 458 | 1:0x00118699 (linear 0x00128699) | 0x001ADCED |
| 459 | 1:0x001186A1 (linear 0x001286A1) | 0x001ADCF5 |
| 461 | 1:0x001186A9 (linear 0x001286A9) | 0x001ADCFD |
| 462 | 1:0x001186AE (linear 0x001286AE) | 0x001ADD02 |
| 463 | 1:0x001186B3 (linear 0x001286B3) | 0x001ADD07 |
| 465 | 1:0x001186BB (linear 0x001286BB) | 0x001ADD0F |
| 472 | 1:0x001186D8 (linear 0x001286D8) | 0x001ADD2C |
| 473 | 1:0x001186DF (linear 0x001286DF) | 0x001ADD33 |
| 476 | 1:0x001186E6 (linear 0x001286E6) | 0x001ADD3A |
| 478 | 1:0x00118707 (linear 0x00128707) | 0x001ADD5B |
| 480 | 1:0x00118734 (linear 0x00128734) | 0x001ADD88 |
| 482 | 1:0x00118774 (linear 0x00128774) | 0x001ADDC8 |
| 483 | 1:0x0011878E (linear 0x0012878E) | 0x001ADDE2 |
| 484 | 1:0x00118796 (linear 0x00128796) | 0x001ADDEA |
| 485 | 1:0x0011879E (linear 0x0012879E) | 0x001ADDF2 |
| 487 | 1:0x001187B9 (linear 0x001287B9) | 0x001ADE0D |
| 488 | 1:0x001187BE (linear 0x001287BE) | 0x001ADE12 |
| 489 | 1:0x001187C3 (linear 0x001287C3) | 0x001ADE17 |
| 492 | 1:0x001187CB (linear 0x001287CB) | 0x001ADE1F |
| 495 | 1:0x001187E3 (linear 0x001287E3) | 0x001ADE37 |
| 497 | 1:0x001187E7 (linear 0x001287E7) | 0x001ADE3B |
| 499 | 1:0x00118809 (linear 0x00128809) | 0x001ADE5D |
| 501 | 1:0x00118824 (linear 0x00128824) | 0x001ADE78 |
| 503 | 1:0x00118842 (linear 0x00128842) | 0x001ADE96 |
| 506 | 1:0x00118846 (linear 0x00128846) | 0x001ADE9A |
| 507 | 1:0x00118848 (linear 0x00128848) | 0x001ADE9C |

