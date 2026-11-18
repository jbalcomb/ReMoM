# F:\WLIBS\lbx\farload.c

Module 152 of the debug info of ORION2.EXE.

Ranges: 1:0x00116ABD (linear 0x00126ABD), 2,894 bytes, 2:0x00002674 (linear 0x00172674), 376 bytes, 2:0x0000C544 (linear 0x0017C544), 56 bytes, 2:0x00043A6C (linear 0x001B3A6C), 2,068 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x00116ABD (linear 0x00126ABD) | code | Farload_ | `regions/07-object-1/001ABE54-001AC653.txt` |
| 1:0x00116AFD (linear 0x00126AFD) | code | Far_Reload_ | `regions/07-object-1/001ABE54-001AC653.txt` |
| 1:0x00116B42 (linear 0x00126B42) | code | Far_Reload_Next_ | `regions/07-object-1/001ABE54-001AC653.txt` |
| 1:0x00116B87 (linear 0x00126B87) | code | Farload_Data_ | `regions/07-object-1/001ABE54-001AC653.txt` |
| 1:0x00116BDD (linear 0x00126BDD) | code | Far_Reload_Data_ | `regions/07-object-1/001ABE54-001AC653.txt` |
| 1:0x00116C37 (linear 0x00126C37) | code | Far_Reload_Next_Data_ | `regions/07-object-1/001ABE54-001AC653.txt` |
| 1:0x00116C91 (linear 0x00126C91) | code | Farload_Data_Static_ | `regions/07-object-1/001ABE54-001AC653.txt` |
| 1:0x00116CEB (linear 0x00126CEB) | code | Farload_Library_ | `regions/07-object-1/001ABE54-001AC653.txt` |
| 1:0x00116F3B (linear 0x00126F3B) | code | Farload_Library_Data_ | `regions/07-object-1/001ABE54-001AC653.txt` |
| 1:0x00117233 (linear 0x00127233) | code | Get_Farload_Data_Record_Count_ | `regions/07-object-1/001AC654-001ACE53.txt` |
| 1:0x001173DC (linear 0x001273DC) | code | Error_Handler_ | `regions/07-object-1/001AC654-001ACE53.txt` |
| 1:0x00117593 (linear 0x00127593) | code | Set_Alternate_Path_ | `regions/07-object-1/001AC654-001ACE53.txt` |
| 2:0x0000C544 (linear 0x0017C544) | data | _secondary_drive_path | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x0000C576 (linear 0x0017C576) | data | _farload_file_flag | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x00043A6C (linear 0x001B3A6C) | data | _lbx_header | - |
| 2:0x0004426C (linear 0x001B426C) | data | _farload_fptr | - |
| 2:0x00044270 (linear 0x001B4270) | data | _farload_file_name | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x00043A6C (linear 0x001B3A6C) | lbx_header | struct struct_lbx_header |
| 2:0x0004426C (linear 0x001B426C) | farload_fptr | FILE * |
| 2:0x0000C576 (linear 0x0017C576) | farload_file_flag | int |
| 2:0x0000C544 (linear 0x0017C544) | secondary_drive_path | char[50] |
| 2:0x00044270 (linear 0x001B4270) | farload_file_name | char[15] |

#### Farload

- Address: 1:0x00116ABD (linear 0x00126ABD), 64 bytes, in `regions/07-object-1/001ABE54-001AC653.txt`
- Type: char * (char *, int); returns in EAX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x10] | char * |
| entry_num | [EBP-0xC] | int |
| pointer | [EBP-0x8] | char * |

#### Far_Reload

- Address: 1:0x00116AFD (linear 0x00126AFD), 69 bytes, in `regions/07-object-1/001ABE54-001AC653.txt`
- Type: char * (char *, int, char *); returns in EAX
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x14] | char * |
| entry_num | [EBP-0x10] | int |
| base_seg | [EBP-0xC] | char * |
| pointer | [EBP-0x8] | char * |

#### Far_Reload_Next

- Address: 1:0x00116B42 (linear 0x00126B42), 69 bytes, in `regions/07-object-1/001ABE54-001AC653.txt`
- Type: char * (char *, int, char *); returns in EAX
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x14] | char * |
| entry_num | [EBP-0x10] | int |
| base_seg | [EBP-0xC] | char * |
| pointer | [EBP-0x8] | char * |

#### Farload_Data

- Address: 1:0x00116B87 (linear 0x00126B87), 86 bytes, in `regions/07-object-1/001ABE54-001AC653.txt`
- Type: char * (char *, int, int, int, int); returns in EAX
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x18] | char * |
| entry_num | [EBP-0x14] | int |
| start_rec | [EBP-0x10] | int |
| num_recs | [EBP-0xC] | int |
| record_size | [EBP+0x10] | int |
| pointer | [EBP-0x8] | char * |

#### Far_Reload_Data

- Address: 1:0x00116BDD (linear 0x00126BDD), 90 bytes, in `regions/07-object-1/001ABE54-001AC653.txt`
- Type: char * (char *, int, char *, int, int, int); returns in EAX
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x18] | char * |
| entry_num | [EBP-0x14] | int |
| base_seg | [EBP-0x10] | char * |
| start_rec | [EBP-0xC] | int |
| num_recs | [EBP+0x10] | int |
| record_size | [EBP+0x14] | int |
| pointer | [EBP-0x8] | char * |

#### Far_Reload_Next_Data

- Address: 1:0x00116C37 (linear 0x00126C37), 90 bytes, in `regions/07-object-1/001ABE54-001AC653.txt`
- Type: char * (char *, int, char *, int, int, int); returns in EAX
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x18] | char * |
| entry_num | [EBP-0x14] | int |
| base_seg | [EBP-0x10] | char * |
| start_rec | [EBP-0xC] | int |
| num_recs | [EBP+0x10] | int |
| record_size | [EBP+0x14] | int |
| pointer | [EBP-0x8] | char * |

#### Farload_Data_Static

- Address: 1:0x00116C91 (linear 0x00126C91), 90 bytes, in `regions/07-object-1/001ABE54-001AC653.txt`
- Type: char * (char *, int, char *, int, int, int); returns in EAX
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x18] | char * |
| entry_num | [EBP-0x14] | int |
| base_seg | [EBP-0x10] | char * |
| start_rec | [EBP-0xC] | int |
| num_recs | [EBP+0x10] | int |
| record_size | [EBP+0x14] | int |
| pointer | [EBP-0x8] | char * |

#### Farload_Library

- Address: 1:0x00116CEB (linear 0x00126CEB), 592 bytes, in `regions/07-object-1/001ABE54-001AC653.txt`
- Type: char * (char *, int, char *, int); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x44] | char * |
| entry_num | [EBP-0x30] | int |
| base_seg | [EBP-0x40] | char * |
| allocation_type | [EBP-0x3C] | int |
| file_index | [EBP-0x38] | int |
| space_remaining | [EBP-0x34] | int |
| current_extended_flag | [EBP-0xC] | int |
| alternate_set | [EBP-0x2C] | int |
| num_blocks | [EBP-0x28] | unsigned int |
| current_seg | [EBP-0x24] | unsigned int |
| read_size | [EBP-0x20] | unsigned int |
| foffset | [EBP-0x1C] | long |
| entry_start | [EBP-0x18] | long |
| entry_end | [EBP-0x14] | long |
| entry_length | [EBP-0x10] | long |
| full_file_path | [EBP-0x80] | char[60] |
| pointer | [EBP-0x8] | char * |

#### Farload_Library_Data

- Address: 1:0x00116F3B (linear 0x00126F3B), 760 bytes, in `regions/07-object-1/001ABE54-001AC653.txt`
- Type: char * (char *, int, char *, int, unsigned int, unsigned short, unsigned short); returns in EAX
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x44] | char * |
| entry_num | [EBP-0x4C] | int |
| base_seg | [EBP-0x48] | char * |
| allocation_type | [EBP-0x38] | int |
| start_rec | [EBP+0x10] | unsigned int |
| num_recs | [EBP+0x14] | unsigned short |
| record_size | [EBP+0x18] | unsigned short |
| allocation_seg | [EBP-0x10] | int |
| file_index | [EBP-0x34] | int |
| space_remaining | [EBP-0x30] | int |
| current_extended_flag | [EBP-0x2C] | int |
| max_records | [EBP-0x8] | unsigned short |
| rec_size | [EBP-0x4] | unsigned short |
| num_blocks | [EBP-0x20] | unsigned int |
| current_seg | [EBP-0x28] | unsigned int |
| read_size | [EBP-0x18] | unsigned int |
| foffset | [EBP-0x40] | long |
| entry_start | [EBP-0x1C] | long |
| entry_end | [EBP-0x3C] | long |
| entry_length | [EBP-0x24] | long |
| full_file_path | [EBP-0x88] | char[60] |
| pointer | [EBP-0xC] | char * |

#### Get_Farload_Data_Record_Count

- Address: 1:0x00117233 (linear 0x00127233), 425 bytes, in `regions/07-object-1/001AC654-001ACE53.txt`
- Type: short (char *, int); returns in AX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x44] | char * |
| entry_num | [EBP-0x30] | int |
| allocation_seg | [EBP-0x40] | int |
| file_index | [EBP-0x3C] | int |
| space_remaining | [EBP-0x38] | int |
| current_extended_flag | [EBP-0x34] | int |
| max_records | [EBP-0xC] | unsigned short |
| rec_size | [EBP-0x8] | unsigned short |
| num_blocks | [EBP-0x28] | unsigned int |
| current_seg | [EBP-0x24] | unsigned int |
| read_size | [EBP-0x20] | unsigned int |
| foffset | [EBP-0x1C] | long |
| entry_start | [EBP-0x18] | long |
| entry_end | [EBP-0x14] | long |
| entry_length | [EBP-0x10] | long |
| full_file_path | [EBP-0x80] | char[60] |
| pointer | [EBP-0x2C] | char * |

#### Error_Handler

- Address: 1:0x001173DC (linear 0x001273DC), 439 bytes, in `regions/07-object-1/001AC654-001ACE53.txt`
- Type: void (char *, int, int)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0xC] | char * |
| error_num | [EBP-0x8] | int |
| entry_num | [EBP-0x4] | int |
| buffer | [EBP-0x98] | char[120] |
| buffer2 | [EBP-0x20] | char[20] |

#### Set_Alternate_Path

- Address: 1:0x00117593 (linear 0x00127593), 120 bytes, in `regions/07-object-1/001AC654-001ACE53.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| alternate | [EBP-0xC] | char * |
| i | [EBP-0x8] | int |
| last_char | [EBP-0x4] | int |

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
| 30 | array |  | 0..509; of long (#27) |
| 31 | struct | struct struct_lbx_header | 5 fields, 2048 bytes |
| 32 | typedef | struct_lbx_header | struct name for struct #31 (#31) |
| 33 | pointer |  | near386; of unsigned char (#5) |
| 34 | struct | struct __iobuf | 8 fields, 26 bytes |
| 35 | typedef | __iobuf | struct name for struct #34 (#34) |
| 36 | typedef | FILE | name for struct __iobuf (#35) |
| 37 | pointer |  | near386; of FILE (#36) |
| 38 | array |  | 0..49; of char (#10) |
| 39 | array |  | 0..14; of char (#10) |
| 40 | procedure |  | near386; returns char * (char *, int) |
| 41 | procedure |  | near386; returns char * (char *, int, char *) |
| 42 | procedure |  | near386; returns char * (char *, int, int, int, int) |
| 43 | procedure |  | near386; returns char * (char *, int, char *, int, int, int) |
| 44 | procedure |  | near386; returns char * (char *, int, char *, int) |
| 45 | array |  | 0..59; of char (#10) |
| 46 | procedure |  | near386; returns char * (char *, int, char *, int, unsigned int, unsigned short, unsigned short) |
| 47 | array |  | 0..59; of char (#10) |
| 48 | procedure |  | near386; returns short (char *, int) |
| 49 | array |  | 0..59; of char (#10) |
| 50 | scalar | void | void 1 bytes |
| 51 | procedure |  | near386; returns void (char *, int, int) |
| 52 | array |  | 0..119; of char (#10) |
| 53 | array |  | 0..19; of char (#10) |
| 54 | procedure |  | near386; returns void (char *) |

##### struct struct_lbx_header (#31, 5 fields, 2048 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| entries | offset 0 | short |
| magic_number | offset 2 | unsigned short |
| file_type | offset 4 | short |
| dummy | offset 6 | short |
| offsets | offset 8 | long[510] |

##### struct __iobuf (#34, 8 fields, 26 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| _ptr | offset 0 | unsigned char * |
| _cnt | offset 4 | int |
| _base | offset 8 | unsigned char * |
| _flag | offset 12 | unsigned int |
| _handle | offset 16 | int |
| _bufsize | offset 20 | unsigned int |
| _ungotten | offset 24 | unsigned char |
| _tmpfchar | offset 25 | unsigned char |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 60 | 1:0x00116ABD (linear 0x00126ABD) | 0x001AC111 |
| 64 | 1:0x00116ADA (linear 0x00126ADA) | 0x001AC12E |
| 65 | 1:0x00116AEC (linear 0x00126AEC) | 0x001AC140 |
| 66 | 1:0x00116AF2 (linear 0x00126AF2) | 0x001AC146 |
| 68 | 1:0x00116AFD (linear 0x00126AFD) | 0x001AC151 |
| 72 | 1:0x00116B1C (linear 0x00126B1C) | 0x001AC170 |
| 73 | 1:0x00116B32 (linear 0x00126B32) | 0x001AC186 |
| 74 | 1:0x00116B38 (linear 0x00126B38) | 0x001AC18C |
| 76 | 1:0x00116B42 (linear 0x00126B42) | 0x001AC196 |
| 80 | 1:0x00116B61 (linear 0x00126B61) | 0x001AC1B5 |
| 81 | 1:0x00116B77 (linear 0x00126B77) | 0x001AC1CB |
| 82 | 1:0x00116B7D (linear 0x00126B7D) | 0x001AC1D1 |
| 89 | 1:0x00116B87 (linear 0x00126B87) | 0x001AC1DB |
| 93 | 1:0x00116BA8 (linear 0x00126BA8) | 0x001AC1FC |
| 95 | 1:0x00116BCC (linear 0x00126BCC) | 0x001AC220 |
| 96 | 1:0x00116BD2 (linear 0x00126BD2) | 0x001AC226 |
| 98 | 1:0x00116BDD (linear 0x00126BDD) | 0x001AC231 |
| 102 | 1:0x00116BFE (linear 0x00126BFE) | 0x001AC252 |
| 104 | 1:0x00116C26 (linear 0x00126C26) | 0x001AC27A |
| 105 | 1:0x00116C2C (linear 0x00126C2C) | 0x001AC280 |
| 107 | 1:0x00116C37 (linear 0x00126C37) | 0x001AC28B |
| 111 | 1:0x00116C58 (linear 0x00126C58) | 0x001AC2AC |
| 113 | 1:0x00116C80 (linear 0x00126C80) | 0x001AC2D4 |
| 114 | 1:0x00116C86 (linear 0x00126C86) | 0x001AC2DA |
| 116 | 1:0x00116C91 (linear 0x00126C91) | 0x001AC2E5 |
| 120 | 1:0x00116CB2 (linear 0x00126CB2) | 0x001AC306 |
| 122 | 1:0x00116CDA (linear 0x00126CDA) | 0x001AC32E |
| 123 | 1:0x00116CE0 (linear 0x00126CE0) | 0x001AC334 |
| 126 | 1:0x00116CEB (linear 0x00126CEB) | 0x001AC33F |
| 135 | 1:0x00116D0C (linear 0x00126D0C) | 0x001AC360 |
| 136 | 1:0x00116D12 (linear 0x00126D12) | 0x001AC366 |
| 138 | 1:0x00116D22 (linear 0x00126D22) | 0x001AC376 |
| 140 | 1:0x00116D40 (linear 0x00126D40) | 0x001AC394 |
| 141 | 1:0x00116D49 (linear 0x00126D49) | 0x001AC39D |
| 143 | 1:0x00116D53 (linear 0x00126D53) | 0x001AC3A7 |
| 144 | 1:0x00116D60 (linear 0x00126D60) | 0x001AC3B4 |
| 146 | 1:0x00116D72 (linear 0x00126D72) | 0x001AC3C6 |
| 148 | 1:0x00116D7B (linear 0x00126D7B) | 0x001AC3CF |
| 149 | 1:0x00116D84 (linear 0x00126D84) | 0x001AC3D8 |
| 151 | 1:0x00116D94 (linear 0x00126D94) | 0x001AC3E8 |
| 152 | 1:0x00116DA1 (linear 0x00126DA1) | 0x001AC3F5 |
| 153 | 1:0x00116DAE (linear 0x00126DAE) | 0x001AC402 |
| 155 | 1:0x00116DC0 (linear 0x00126DC0) | 0x001AC414 |
| 156 | 1:0x00116DC9 (linear 0x00126DC9) | 0x001AC41D |
| 159 | 1:0x00116DD9 (linear 0x00126DD9) | 0x001AC42D |
| 161 | 1:0x00116DF3 (linear 0x00126DF3) | 0x001AC447 |
| 162 | 1:0x00116E02 (linear 0x00126E02) | 0x001AC456 |
| 164 | 1:0x00116E12 (linear 0x00126E12) | 0x001AC466 |
| 167 | 1:0x00116E1C (linear 0x00126E1C) | 0x001AC470 |
| 168 | 1:0x00116E29 (linear 0x00126E29) | 0x001AC47D |
| 170 | 1:0x00116E51 (linear 0x00126E51) | 0x001AC4A5 |
| 171 | 1:0x00116E60 (linear 0x00126E60) | 0x001AC4B4 |
| 172 | 1:0x00116E6F (linear 0x00126E6F) | 0x001AC4C3 |
| 174 | 1:0x00116E78 (linear 0x00126E78) | 0x001AC4CC |
| 176 | 1:0x00116E87 (linear 0x00126E87) | 0x001AC4DB |
| 179 | 1:0x00116E9E (linear 0x00126E9E) | 0x001AC4F2 |
| 180 | 1:0x00116EA9 (linear 0x00126EA9) | 0x001AC4FD |
| 181 | 1:0x00116EAF (linear 0x00126EAF) | 0x001AC503 |
| 182 | 1:0x00116EBF (linear 0x00126EBF) | 0x001AC513 |
| 185 | 1:0x00116EC1 (linear 0x00126EC1) | 0x001AC515 |
| 186 | 1:0x00116ECF (linear 0x00126ECF) | 0x001AC523 |
| 187 | 1:0x00116ED5 (linear 0x00126ED5) | 0x001AC529 |
| 188 | 1:0x00116EE5 (linear 0x00126EE5) | 0x001AC539 |
| 191 | 1:0x00116EE7 (linear 0x00126EE7) | 0x001AC53B |
| 192 | 1:0x00116EF5 (linear 0x00126EF5) | 0x001AC549 |
| 193 | 1:0x00116EFB (linear 0x00126EFB) | 0x001AC54F |
| 194 | 1:0x00116F0B (linear 0x00126F0B) | 0x001AC55F |
| 197 | 1:0x00116F0D (linear 0x00126F0D) | 0x001AC561 |
| 200 | 1:0x00116F16 (linear 0x00126F16) | 0x001AC56A |
| 202 | 1:0x00116F2C (linear 0x00126F2C) | 0x001AC580 |
| 203 | 1:0x00116F32 (linear 0x00126F32) | 0x001AC586 |
| 207 | 1:0x00116F3B (linear 0x00126F3B) | 0x001AC58F |
| 218 | 1:0x00116F5C (linear 0x00126F5C) | 0x001AC5B0 |
| 220 | 1:0x00116F72 (linear 0x00126F72) | 0x001AC5C6 |
| 222 | 1:0x00116F90 (linear 0x00126F90) | 0x001AC5E4 |
| 224 | 1:0x00116FA3 (linear 0x00126FA3) | 0x001AC5F7 |
| 225 | 1:0x00116FB0 (linear 0x00126FB0) | 0x001AC604 |
| 227 | 1:0x00116FC2 (linear 0x00126FC2) | 0x001AC616 |
| 229 | 1:0x00116FCB (linear 0x00126FCB) | 0x001AC61F |
| 230 | 1:0x00116FD4 (linear 0x00126FD4) | 0x001AC628 |
| 232 | 1:0x00116FE4 (linear 0x00126FE4) | 0x001AC638 |
| 233 | 1:0x00116FF4 (linear 0x00126FF4) | 0x001AC648 |
| 234 | 1:0x00117004 (linear 0x00127004) | 0x001AC658 |
| 236 | 1:0x00117019 (linear 0x00127019) | 0x001AC66D |
| 237 | 1:0x00117022 (linear 0x00127022) | 0x001AC676 |
| 240 | 1:0x00117032 (linear 0x00127032) | 0x001AC686 |
| 242 | 1:0x0011704C (linear 0x0012704C) | 0x001AC6A0 |
| 243 | 1:0x0011705B (linear 0x0012705B) | 0x001AC6AF |
| 245 | 1:0x0011706B (linear 0x0012706B) | 0x001AC6BF |
| 248 | 1:0x00117075 (linear 0x00127075) | 0x001AC6C9 |
| 249 | 1:0x00117082 (linear 0x00127082) | 0x001AC6D6 |
| 251 | 1:0x00117092 (linear 0x00127092) | 0x001AC6E6 |
| 252 | 1:0x001170A1 (linear 0x001270A1) | 0x001AC6F5 |
| 253 | 1:0x001170B0 (linear 0x001270B0) | 0x001AC704 |
| 255 | 1:0x001170B9 (linear 0x001270B9) | 0x001AC70D |
| 257 | 1:0x001170C8 (linear 0x001270C8) | 0x001AC71C |
| 258 | 1:0x001170E0 (linear 0x001270E0) | 0x001AC734 |
| 260 | 1:0x001170F8 (linear 0x001270F8) | 0x001AC74C |
| 261 | 1:0x00117103 (linear 0x00127103) | 0x001AC757 |
| 263 | 1:0x0011710C (linear 0x0012710C) | 0x001AC760 |
| 264 | 1:0x00117127 (linear 0x00127127) | 0x001AC77B |
| 266 | 1:0x00117155 (linear 0x00127155) | 0x001AC7A9 |
| 269 | 1:0x0011716E (linear 0x0012716E) | 0x001AC7C2 |
| 272 | 1:0x00117180 (linear 0x00127180) | 0x001AC7D4 |
| 274 | 1:0x00117197 (linear 0x00127197) | 0x001AC7EB |
| 275 | 1:0x001171A2 (linear 0x001271A2) | 0x001AC7F6 |
| 276 | 1:0x001171A8 (linear 0x001271A8) | 0x001AC7FC |
| 277 | 1:0x001171B8 (linear 0x001271B8) | 0x001AC80C |
| 279 | 1:0x001171BA (linear 0x001271BA) | 0x001AC80E |
| 280 | 1:0x001171C8 (linear 0x001271C8) | 0x001AC81C |
| 281 | 1:0x001171CE (linear 0x001271CE) | 0x001AC822 |
| 282 | 1:0x001171DE (linear 0x001271DE) | 0x001AC832 |
| 284 | 1:0x001171E0 (linear 0x001271E0) | 0x001AC834 |
| 285 | 1:0x001171EE (linear 0x001271EE) | 0x001AC842 |
| 286 | 1:0x001171F4 (linear 0x001271F4) | 0x001AC848 |
| 287 | 1:0x00117204 (linear 0x00127204) | 0x001AC858 |
| 289 | 1:0x00117206 (linear 0x00127206) | 0x001AC85A |
| 293 | 1:0x0011720C (linear 0x0012720C) | 0x001AC860 |
| 295 | 1:0x00117222 (linear 0x00127222) | 0x001AC876 |
| 296 | 1:0x00117228 (linear 0x00127228) | 0x001AC87C |
| 306 | 1:0x00117233 (linear 0x00127233) | 0x001AC887 |
| 315 | 1:0x00117250 (linear 0x00127250) | 0x001AC8A4 |
| 317 | 1:0x00117266 (linear 0x00127266) | 0x001AC8BA |
| 319 | 1:0x00117284 (linear 0x00127284) | 0x001AC8D8 |
| 321 | 1:0x00117297 (linear 0x00127297) | 0x001AC8EB |
| 322 | 1:0x001172A4 (linear 0x001272A4) | 0x001AC8F8 |
| 324 | 1:0x001172B6 (linear 0x001272B6) | 0x001AC90A |
| 326 | 1:0x001172BF (linear 0x001272BF) | 0x001AC913 |
| 327 | 1:0x001172C8 (linear 0x001272C8) | 0x001AC91C |
| 329 | 1:0x001172D8 (linear 0x001272D8) | 0x001AC92C |
| 330 | 1:0x001172E5 (linear 0x001272E5) | 0x001AC939 |
| 331 | 1:0x001172F2 (linear 0x001272F2) | 0x001AC946 |
| 333 | 1:0x00117304 (linear 0x00127304) | 0x001AC958 |
| 334 | 1:0x0011730D (linear 0x0012730D) | 0x001AC961 |
| 337 | 1:0x0011731D (linear 0x0012731D) | 0x001AC971 |
| 339 | 1:0x00117337 (linear 0x00127337) | 0x001AC98B |
| 340 | 1:0x00117346 (linear 0x00127346) | 0x001AC99A |
| 342 | 1:0x00117356 (linear 0x00127356) | 0x001AC9AA |
| 345 | 1:0x00117360 (linear 0x00127360) | 0x001AC9B4 |
| 346 | 1:0x0011736D (linear 0x0012736D) | 0x001AC9C1 |
| 348 | 1:0x0011737D (linear 0x0012737D) | 0x001AC9D1 |
| 349 | 1:0x0011738C (linear 0x0012738C) | 0x001AC9E0 |
| 350 | 1:0x0011739B (linear 0x0012739B) | 0x001AC9EF |
| 352 | 1:0x001173A4 (linear 0x001273A4) | 0x001AC9F8 |
| 354 | 1:0x001173B3 (linear 0x001273B3) | 0x001ACA07 |
| 356 | 1:0x001173CB (linear 0x001273CB) | 0x001ACA1F |
| 357 | 1:0x001173D1 (linear 0x001273D1) | 0x001ACA25 |
| 366 | 1:0x001173DC (linear 0x001273DC) | 0x001ACA30 |
| 371 | 1:0x001173FB (linear 0x001273FB) | 0x001ACA4F |
| 372 | 1:0x00117409 (linear 0x00127409) | 0x001ACA5D |
| 373 | 1:0x00117419 (linear 0x00127419) | 0x001ACA6D |
| 374 | 1:0x00117429 (linear 0x00127429) | 0x001ACA7D |
| 375 | 1:0x00117437 (linear 0x00127437) | 0x001ACA8B |
| 377 | 1:0x00117447 (linear 0x00127447) | 0x001ACA9B |
| 379 | 1:0x001174C2 (linear 0x001274C2) | 0x001ACB16 |
| 380 | 1:0x001174D7 (linear 0x001274D7) | 0x001ACB2B |
| 381 | 1:0x001174EC (linear 0x001274EC) | 0x001ACB40 |
| 382 | 1:0x00117505 (linear 0x00127505) | 0x001ACB59 |
| 383 | 1:0x00117517 (linear 0x00127517) | 0x001ACB6B |
| 384 | 1:0x00117527 (linear 0x00127527) | 0x001ACB7B |
| 385 | 1:0x00117529 (linear 0x00127529) | 0x001ACB7D |
| 386 | 1:0x00117539 (linear 0x00127539) | 0x001ACB8D |
| 387 | 1:0x0011753B (linear 0x0012753B) | 0x001ACB8F |
| 388 | 1:0x0011754B (linear 0x0012754B) | 0x001ACB9F |
| 389 | 1:0x0011754D (linear 0x0012754D) | 0x001ACBA1 |
| 390 | 1:0x0011755D (linear 0x0012755D) | 0x001ACBB1 |
| 391 | 1:0x0011755F (linear 0x0012755F) | 0x001ACBB3 |
| 392 | 1:0x0011756F (linear 0x0012756F) | 0x001ACBC3 |
| 393 | 1:0x00117571 (linear 0x00127571) | 0x001ACBC5 |
| 395 | 1:0x00117581 (linear 0x00127581) | 0x001ACBD5 |
| 397 | 1:0x0011758C (linear 0x0012758C) | 0x001ACBE0 |
| 400 | 1:0x00117593 (linear 0x00127593) | 0x001ACBE7 |
| 403 | 1:0x001175AE (linear 0x001275AE) | 0x001ACC02 |
| 404 | 1:0x001175BB (linear 0x001275BB) | 0x001ACC0F |
| 405 | 1:0x001175E0 (linear 0x001275E0) | 0x001ACC34 |
| 406 | 1:0x001175F3 (linear 0x001275F3) | 0x001ACC47 |
| 407 | 1:0x00117602 (linear 0x00127602) | 0x001ACC56 |

