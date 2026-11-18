# F:\WLIBS\animate\file_ani.c

Module 171 of the debug info of ORION2.EXE.

Ranges: 1:0x0011C607 (linear 0x0012C607), 4,846 bytes, 2:0x00002B54 (linear 0x00172B54), 704 bytes, 2:0x0000C5E0 (linear 0x0017C5E0), 8 bytes, 2:0x000442E8 (linear 0x001B42E8), 112 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0011C607 (linear 0x0012C607) | code | Open_File_Animation_ | `regions/07-object-1/001B1654-001B1E53.txt` |
| 1:0x0011C7CC (linear 0x0012C7CC) | code | Draw_File_Animation_ | `regions/07-object-1/001B1654-001B1E53.txt` |
| 1:0x0011CAD6 (linear 0x0012CAD6) | code | Draw_Multi_File_Animation_Stencil_ | `regions/07-object-1/001B1E54-001B2653.txt` |
| 1:0x0011CD2D (linear 0x0012CD2D) | code | Draw_File_Animation_Stencil_ | `regions/07-object-1/001B1E54-001B2653.txt` |
| 1:0x0011CF6D (linear 0x0012CF6D) | code | Draw_File_Animation_On_ | `regions/07-object-1/001B1E54-001B2653.txt` |
| 1:0x0011D1C5 (linear 0x0012D1C5) | code | Open_Multi_File_Animation_ | `regions/07-object-1/001B2654-001B2E53.txt` |
| 1:0x0011D408 (linear 0x0012D408) | code | Draw_Multi_File_Animation_ | `regions/07-object-1/001B2654-001B2E53.txt` |
| 1:0x0011D6B2 (linear 0x0012D6B2) | code | Set_File_Animation_Frame_ | `regions/07-object-1/001B2654-001B2E53.txt` |
| 1:0x0011D70B (linear 0x0012D70B) | code | Get_File_Animation_Frame_ | `regions/07-object-1/001B2654-001B2E53.txt` |
| 1:0x0011D745 (linear 0x0012D745) | code | Get_Multi_File_Animation_Frame_ | `regions/07-object-1/001B2654-001B2E53.txt` |
| 1:0x0011D78E (linear 0x0012D78E) | code | Reset_File_Animation_Buffer_ | `regions/07-object-1/001B2654-001B2E53.txt` |
| 1:0x0011D7B7 (linear 0x0012D7B7) | code | Set_File_Animation_Buffer_ | `regions/07-object-1/001B2654-001B2E53.txt` |
| 1:0x0011D7E3 (linear 0x0012D7E3) | code | Draw_Multi_File_Animation_Palette_ | `regions/07-object-1/001B2654-001B2E53.txt` |
| 1:0x0011D84E (linear 0x0012D84E) | code | Draw_File_Animation_Palette_ | `regions/07-object-1/001B2E54-001B3653.txt` |
| 1:0x0011D88F (linear 0x0012D88F) | code | Set_Multi_File_Animation_Frame_ | `regions/07-object-1/001B2E54-001B3653.txt` |
| 2:0x0000C5E0 (linear 0x0017C5E0) | data | _file_animation_opened_flag | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x0000C5E2 (linear 0x0017C5E2) | data | _file_animation_opened_flag_multi | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x000442E8 (linear 0x001B42E8) | data | _file_animation_length | - |
| 2:0x000442EC (linear 0x001B42EC) | data | _multi_file_animation_names | - |
| 2:0x00044310 (linear 0x001B4310) | data | _file_animation_fptr_multi | - |
| 2:0x0004431C (linear 0x001B431C) | data | _file_lbx_header | - |
| 2:0x00044320 (linear 0x001B4320) | data | _file_animation_header | - |
| 2:0x00044324 (linear 0x001B4324) | data | _file_animation_buffer | - |
| 2:0x00044328 (linear 0x001B4328) | data | _file_animation_start | - |
| 2:0x0004432C (linear 0x001B432C) | data | _file_animation_end_multi | - |
| 2:0x00044338 (linear 0x001B4338) | data | _file_animation_fptr | - |
| 2:0x0004433C (linear 0x001B433C) | data | _file_animation_length_multi | - |
| 2:0x00044348 (linear 0x001B4348) | data | _file_animation_start_multi | - |
| 2:0x00044354 (linear 0x001B4354) | data | _file_animation_end | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x000442E8 (linear 0x001B42E8) | file_animation_length | long |
| 2:0x000442EC (linear 0x001B42EC) | multi_file_animation_names | char[12][3] |
| 2:0x00044310 (linear 0x001B4310) | file_animation_fptr_multi | FILE *[3] |
| 2:0x0004431C (linear 0x001B431C) | file_lbx_header | struct struct_lbx_header * |
| 2:0x00044320 (linear 0x001B4320) | file_animation_header | struct s_animation_header * |
| 2:0x00044324 (linear 0x001B4324) | file_animation_buffer | char * |
| 2:0x00044328 (linear 0x001B4328) | file_animation_start | long |
| 2:0x0004432C (linear 0x001B432C) | file_animation_end_multi | long[3] |
| 2:0x00044338 (linear 0x001B4338) | file_animation_fptr | FILE * |
| 2:0x0004433C (linear 0x001B433C) | file_animation_length_multi | long[3] |
| 2:0x00044348 (linear 0x001B4348) | file_animation_start_multi | long[3] |
| 2:0x00044354 (linear 0x001B4354) | file_animation_end | long |
| 2:0x0000C5E2 (linear 0x0017C5E2) | file_animation_opened_flag_multi | short[3] |
| 2:0x0000C5E0 (linear 0x0017C5E0) | file_animation_opened_flag | short |

#### Open_File_Animation

- Address: 1:0x0011C607 (linear 0x0012C607), 453 bytes, in `regions/07-object-1/001B1654-001B1E53.txt`
- Type: void (char *, short)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x44] | char * |
| entry_num | [EBP-0xC] | short |
| i | [EBP-0x48] | int |
| x2 | [EBP-0x4] | short |
| y2 | [EBP-0x10] | short |
| clip | [EBP-0x8] | short |
| frame_list | [EBP-0x38] | long |
| frame_offset | [EBP-0x34] | char * |
| frame_offset_table | [EBP-0x30] | long * |
| file_index | [EBP-0x1C] | int |
| space_remaining | [EBP-0x28] | int |
| current_extended_flag | [EBP-0x4C] | int |
| num_blocks | [EBP-0x20] | unsigned int |
| current_seg | [EBP-0x2C] | unsigned int |
| read_size | [EBP-0x40] | unsigned int |
| foffset | [EBP-0x3C] | long |
| entry_start | [EBP-0x18] | long |
| entry_end | [EBP-0x24] | long |
| entry_length | [EBP-0x14] | long |
| full_file_path | [EBP-0x88] | char[60] |

#### Draw_File_Animation

- Address: 1:0x0011C7CC (linear 0x0012C7CC), 778 bytes, in `regions/07-object-1/001B1654-001B1E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x2C] | int |
| x | [EBP-0x28] | int |
| y | [EBP-0x24] | int |
| x2 | [EBP-0xC] | short |
| y2 | [EBP-0x8] | short |
| clip | [EBP-0x4] | short |
| frame_list | [EBP-0x14] | long |
| frame_length | [EBP-0x10] | long |
| frame_offset | [EBP-0x20] | char * |
| frame_offset_table | [EBP-0x1C] | long * |
| file_offset | [EBP-0x18] | long |

#### Draw_Multi_File_Animation_Stencil

- Address: 1:0x0011CAD6 (linear 0x0012CAD6), 599 bytes, in `regions/07-object-1/001B1E54-001B2653.txt`
- Type: void (short)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| file_num | [EBP-0xC] | short |
| i | [EBP-0x2C] | int |
| x | [EBP-0x28] | int |
| y | [EBP-0x24] | int |
| x2 | [EBP-0x8] | short |
| y2 | [EBP-0x4] | short |
| clip | [EBP-0x10] | short |
| frame_list | [EBP-0x18] | long |
| frame_length | [EBP-0x20] | long |
| frame_offset | [EBP-0x14] | char * |
| frame_offset_table | [EBP-0x30] | long * |
| file_offset | [EBP-0x1C] | long |

#### Draw_File_Animation_Stencil

- Address: 1:0x0011CD2D (linear 0x0012CD2D), 576 bytes, in `regions/07-object-1/001B1E54-001B2653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x30] | int |
| x | [EBP-0x2C] | int |
| y | [EBP-0x28] | int |
| x2 | [EBP-0xC] | short |
| y2 | [EBP-0x8] | short |
| clip | [EBP-0x4] | short |
| frame_list | [EBP-0x18] | long |
| frame_length | [EBP-0x14] | long |
| frame_offset | [EBP-0x10] | char * |
| frame_offset_table | [EBP-0x24] | long * |
| file_offset | [EBP-0x20] | long |
| saved_refresh | [EBP-0x1C] | int |

#### Draw_File_Animation_On

- Address: 1:0x0011CF6D (linear 0x0012CF6D), 600 bytes, in `regions/07-object-1/001B1E54-001B2653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x30] | int |
| x | [EBP-0x2C] | int |
| y | [EBP-0x28] | int |
| x2 | [EBP-0xC] | short |
| y2 | [EBP-0x8] | short |
| clip | [EBP-0x4] | short |
| frame_list | [EBP-0x18] | long |
| frame_length | [EBP-0x14] | long |
| frame_offset | [EBP-0x10] | char * |
| frame_offset_table | [EBP-0x24] | long * |
| file_offset | [EBP-0x20] | long |
| saved_refresh | [EBP-0x1C] | int |

#### Open_Multi_File_Animation

- Address: 1:0x0011D1C5 (linear 0x0012D1C5), 579 bytes, in `regions/07-object-1/001B2654-001B2E53.txt`
- Type: void (char *, short, short)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| file_name | [EBP-0x2C] | char * |
| entry_num | [EBP-0x10] | short |
| file_num | [EBP-0xC] | short |
| i | [EBP-0x48] | int |
| x2 | [EBP-0x4] | short |
| y2 | [EBP-0x14] | short |
| clip | [EBP-0x8] | short |
| frame_list | [EBP-0x38] | long |
| frame_offset | [EBP-0x34] | char * |
| frame_offset_table | [EBP-0x30] | long * |
| file_index | [EBP-0x1C] | int |
| space_remaining | [EBP-0x50] | int |
| current_extended_flag | [EBP-0x4C] | int |
| num_blocks | [EBP-0x20] | unsigned int |
| current_seg | [EBP-0x44] | unsigned int |
| read_size | [EBP-0x40] | unsigned int |
| foffset | [EBP-0x3C] | long |
| entry_start | [EBP-0x28] | long |
| entry_end | [EBP-0x24] | long |
| entry_length | [EBP-0x18] | long |
| full_file_path | [EBP-0x8C] | char[60] |

#### Draw_Multi_File_Animation

- Address: 1:0x0011D408 (linear 0x0012D408), 682 bytes, in `regions/07-object-1/001B2654-001B2E53.txt`
- Type: void (short)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| file_num | [EBP-0xC] | short |
| i | [EBP-0x2C] | int |
| x | [EBP-0x28] | int |
| y | [EBP-0x24] | int |
| x2 | [EBP-0x8] | short |
| y2 | [EBP-0x4] | short |
| clip | [EBP-0x10] | short |
| frame_list | [EBP-0x18] | long |
| frame_length | [EBP-0x20] | long |
| frame_offset | [EBP-0x14] | char * |
| frame_offset_table | [EBP-0x30] | long * |
| file_offset | [EBP-0x1C] | long |

#### Set_File_Animation_Frame

- Address: 1:0x0011D6B2 (linear 0x0012D6B2), 89 bytes, in `regions/07-object-1/001B2654-001B2E53.txt`
- Type: void (short)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| frame_num | [EBP-0x4] | short |

#### Get_File_Animation_Frame

- Address: 1:0x0011D70B (linear 0x0012D70B), 58 bytes, in `regions/07-object-1/001B2654-001B2E53.txt`
- Type: short (void); returns in AX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Get_Multi_File_Animation_Frame

- Address: 1:0x0011D745 (linear 0x0012D745), 73 bytes, in `regions/07-object-1/001B2654-001B2E53.txt`
- Type: short (short); returns in AX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| file_num | [EBP-0x8] | short |

#### Reset_File_Animation_Buffer

- Address: 1:0x0011D78E (linear 0x0012D78E), 41 bytes, in `regions/07-object-1/001B2654-001B2E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Set_File_Animation_Buffer

- Address: 1:0x0011D7B7 (linear 0x0012D7B7), 44 bytes, in `regions/07-object-1/001B2654-001B2E53.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| buffer | [EBP-0x4] | char * |

#### Draw_Multi_File_Animation_Palette

- Address: 1:0x0011D7E3 (linear 0x0012D7E3), 107 bytes, in `regions/07-object-1/001B2654-001B2E53.txt`
- Type: void (int)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| file_num | [EBP-0x4] | int |

#### Draw_File_Animation_Palette

- Address: 1:0x0011D84E (linear 0x0012D84E), 65 bytes, in `regions/07-object-1/001B2E54-001B3653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Set_Multi_File_Animation_Frame

- Address: 1:0x0011D88F (linear 0x0012D88F), 102 bytes, in `regions/07-object-1/001B2E54-001B3653.txt`
- Type: void (short, short)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| file_num | [EBP-0x8] | short |
| frame_num | [EBP-0x4] | short |

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
| 30 | array |  | 0..11; of char (#10) |
| 31 | array |  | 0..2; of char[12] (#30) |
| 32 | pointer |  | near386; of unsigned char (#5) |
| 33 | struct | struct __iobuf | 8 fields, 26 bytes |
| 34 | typedef | __iobuf | struct name for struct #33 (#33) |
| 35 | typedef | FILE | name for struct __iobuf (#34) |
| 36 | pointer |  | near386; of FILE (#35) |
| 37 | array |  | 0..2; of FILE * (#36) |
| 38 | array |  | 0..510; of long (#16) |
| 39 | struct | struct struct_lbx_header | 5 fields, 2052 bytes |
| 40 | typedef | struct_lbx_header | struct name for struct #39 (#39) |
| 41 | pointer |  | near386; of struct struct_lbx_header (#40) |
| 42 | array |  | 0..509; of char * (#13) |
| 43 | struct | struct s_animation_header | 9 fields, 2052 bytes |
| 44 | typedef | s_animation_header | struct name for struct #43 (#43) |
| 45 | pointer |  | near386; of struct s_animation_header (#44) |
| 46 | array |  | 0..2; of long (#16) |
| 47 | array |  | 0..2; of long (#16) |
| 48 | array |  | 0..2; of long (#16) |
| 49 | array |  | 0..2; of short (#6) |
| 50 | scalar | void | void 1 bytes |
| 51 | procedure |  | near386; returns void (char *, short) |
| 52 | pointer |  | near386; of long (#16) |
| 53 | array |  | 0..59; of char (#10) |
| 54 | procedure |  | near386; returns void () |
| 55 | procedure |  | near386; returns void (short) |
| 56 | procedure |  | near386; returns void (char *, short, short) |
| 57 | array |  | 0..59; of char (#10) |
| 58 | procedure |  | near386; returns short (void) |
| 59 | procedure |  | near386; returns short (short) |
| 60 | procedure |  | near386; returns void (char *) |
| 61 | procedure |  | near386; returns void (int) |
| 62 | procedure |  | near386; returns void (short, short) |

##### struct __iobuf (#33, 8 fields, 26 bytes)

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

##### struct struct_lbx_header (#39, 5 fields, 2052 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| entries | offset 0 | short |
| magic_number | offset 2 | unsigned short |
| file_type | offset 4 | short |
| dummy | offset 6 | short |
| offsets | offset 8 | long[511] |

##### struct s_animation_header (#43, 9 fields, 2052 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| ddx | offset 0 | short |
| ddy | offset 2 | short |
| current_frame | offset 4 | short |
| total_frames | offset 6 | short |
| repeat_frame | offset 8 | char |
| multi_frame_full_store | offset 9 | char |
| group_count | offset 10 | char |
| mask | offset 11 | char |
| frame_offsets | offset 12 | char *[510] |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 70 | 1:0x0011C607 (linear 0x0012C607) | 0x001B1C5B |
| 83 | 1:0x0011C624 (linear 0x0012C624) | 0x001B1C78 |
| 85 | 1:0x0011C62E (linear 0x0012C62E) | 0x001B1C82 |
| 87 | 1:0x0011C646 (linear 0x0012C646) | 0x001B1C9A |
| 89 | 1:0x0011C653 (linear 0x0012C653) | 0x001B1CA7 |
| 90 | 1:0x0011C65D (linear 0x0012C65D) | 0x001B1CB1 |
| 94 | 1:0x0011C667 (linear 0x0012C667) | 0x001B1CBB |
| 97 | 1:0x0011C679 (linear 0x0012C679) | 0x001B1CCD |
| 99 | 1:0x0011C682 (linear 0x0012C682) | 0x001B1CD6 |
| 100 | 1:0x0011C68B (linear 0x0012C68B) | 0x001B1CDF |
| 102 | 1:0x0011C69C (linear 0x0012C69C) | 0x001B1CF0 |
| 103 | 1:0x0011C6AC (linear 0x0012C6AC) | 0x001B1D00 |
| 105 | 1:0x0011C6BA (linear 0x0012C6BA) | 0x001B1D0E |
| 107 | 1:0x0011C6CF (linear 0x0012C6CF) | 0x001B1D23 |
| 109 | 1:0x0011C6D8 (linear 0x0012C6D8) | 0x001B1D2C |
| 113 | 1:0x0011C6E9 (linear 0x0012C6E9) | 0x001B1D3D |
| 114 | 1:0x0011C6F2 (linear 0x0012C6F2) | 0x001B1D46 |
| 116 | 1:0x0011C70C (linear 0x0012C70C) | 0x001B1D60 |
| 117 | 1:0x0011C721 (linear 0x0012C721) | 0x001B1D75 |
| 119 | 1:0x0011C732 (linear 0x0012C732) | 0x001B1D86 |
| 120 | 1:0x0011C740 (linear 0x0012C740) | 0x001B1D94 |
| 122 | 1:0x0011C751 (linear 0x0012C751) | 0x001B1DA5 |
| 123 | 1:0x0011C765 (linear 0x0012C765) | 0x001B1DB9 |
| 124 | 1:0x0011C77A (linear 0x0012C77A) | 0x001B1DCE |
| 126 | 1:0x0011C783 (linear 0x0012C783) | 0x001B1DD7 |
| 127 | 1:0x0011C78B (linear 0x0012C78B) | 0x001B1DDF |
| 128 | 1:0x0011C793 (linear 0x0012C793) | 0x001B1DE7 |
| 130 | 1:0x0011C79B (linear 0x0012C79B) | 0x001B1DEF |
| 132 | 1:0x0011C7AA (linear 0x0012C7AA) | 0x001B1DFE |
| 133 | 1:0x0011C7C4 (linear 0x0012C7C4) | 0x001B1E18 |
| 141 | 1:0x0011C7CC (linear 0x0012C7CC) | 0x001B1E20 |
| 150 | 1:0x0011C7E4 (linear 0x0012C7E4) | 0x001B1E38 |
| 151 | 1:0x0011C7EB (linear 0x0012C7EB) | 0x001B1E3F |
| 153 | 1:0x0011C7F2 (linear 0x0012C7F2) | 0x001B1E46 |
| 155 | 1:0x0011C7FC (linear 0x0012C7FC) | 0x001B1E50 |
| 156 | 1:0x0011C80F (linear 0x0012C80F) | 0x001B1E63 |
| 158 | 1:0x0011C814 (linear 0x0012C814) | 0x001B1E68 |
| 160 | 1:0x0011C81F (linear 0x0012C81F) | 0x001B1E73 |
| 161 | 1:0x0011C83C (linear 0x0012C83C) | 0x001B1E90 |
| 164 | 1:0x0011C868 (linear 0x0012C868) | 0x001B1EBC |
| 166 | 1:0x0011C881 (linear 0x0012C881) | 0x001B1ED5 |
| 167 | 1:0x0011C889 (linear 0x0012C889) | 0x001B1EDD |
| 168 | 1:0x0011C898 (linear 0x0012C898) | 0x001B1EEC |
| 170 | 1:0x0011C8AE (linear 0x0012C8AE) | 0x001B1F02 |
| 171 | 1:0x0011C8B7 (linear 0x0012C8B7) | 0x001B1F0B |
| 174 | 1:0x0011C8DE (linear 0x0012C8DE) | 0x001B1F32 |
| 176 | 1:0x0011C907 (linear 0x0012C907) | 0x001B1F5B |
| 178 | 1:0x0011C92D (linear 0x0012C92D) | 0x001B1F81 |
| 179 | 1:0x0011C93D (linear 0x0012C93D) | 0x001B1F91 |
| 180 | 1:0x0011C94D (linear 0x0012C94D) | 0x001B1FA1 |
| 181 | 1:0x0011C95A (linear 0x0012C95A) | 0x001B1FAE |
| 182 | 1:0x0011C962 (linear 0x0012C962) | 0x001B1FB6 |
| 183 | 1:0x0011C964 (linear 0x0012C964) | 0x001B1FB8 |
| 184 | 1:0x0011C96C (linear 0x0012C96C) | 0x001B1FC0 |
| 185 | 1:0x0011C975 (linear 0x0012C975) | 0x001B1FC9 |
| 186 | 1:0x0011C988 (linear 0x0012C988) | 0x001B1FDC |
| 188 | 1:0x0011C98A (linear 0x0012C98A) | 0x001B1FDE |
| 191 | 1:0x0011C994 (linear 0x0012C994) | 0x001B1FE8 |
| 193 | 1:0x0011C9AB (linear 0x0012C9AB) | 0x001B1FFF |
| 195 | 1:0x0011C9D1 (linear 0x0012C9D1) | 0x001B2025 |
| 196 | 1:0x0011C9E1 (linear 0x0012C9E1) | 0x001B2035 |
| 197 | 1:0x0011C9F1 (linear 0x0012C9F1) | 0x001B2045 |
| 198 | 1:0x0011C9FE (linear 0x0012C9FE) | 0x001B2052 |
| 199 | 1:0x0011CA06 (linear 0x0012CA06) | 0x001B205A |
| 200 | 1:0x0011CA08 (linear 0x0012CA08) | 0x001B205C |
| 201 | 1:0x0011CA10 (linear 0x0012CA10) | 0x001B2064 |
| 202 | 1:0x0011CA19 (linear 0x0012CA19) | 0x001B206D |
| 203 | 1:0x0011CA2C (linear 0x0012CA2C) | 0x001B2080 |
| 205 | 1:0x0011CA2E (linear 0x0012CA2E) | 0x001B2082 |
| 211 | 1:0x0011CA38 (linear 0x0012CA38) | 0x001B208C |
| 212 | 1:0x0011CA41 (linear 0x0012CA41) | 0x001B2095 |
| 214 | 1:0x0011CA54 (linear 0x0012CA54) | 0x001B20A8 |
| 216 | 1:0x0011CA69 (linear 0x0012CA69) | 0x001B20BD |
| 218 | 1:0x0011CA83 (linear 0x0012CA83) | 0x001B20D7 |
| 220 | 1:0x0011CA8E (linear 0x0012CA8E) | 0x001B20E2 |
| 222 | 1:0x0011CA90 (linear 0x0012CA90) | 0x001B20E4 |
| 227 | 1:0x0011CAA3 (linear 0x0012CAA3) | 0x001B20F7 |
| 229 | 1:0x0011CAA5 (linear 0x0012CAA5) | 0x001B20F9 |
| 230 | 1:0x0011CABA (linear 0x0012CABA) | 0x001B210E |
| 233 | 1:0x0011CACD (linear 0x0012CACD) | 0x001B2121 |
| 240 | 1:0x0011CAD6 (linear 0x0012CAD6) | 0x001B212A |
| 249 | 1:0x0011CAF1 (linear 0x0012CAF1) | 0x001B2145 |
| 250 | 1:0x0011CAF8 (linear 0x0012CAF8) | 0x001B214C |
| 252 | 1:0x0011CAFF (linear 0x0012CAFF) | 0x001B2153 |
| 254 | 1:0x0011CB15 (linear 0x0012CB15) | 0x001B2169 |
| 255 | 1:0x0011CB27 (linear 0x0012CB27) | 0x001B217B |
| 257 | 1:0x0011CB31 (linear 0x0012CB31) | 0x001B2185 |
| 258 | 1:0x0011CB44 (linear 0x0012CB44) | 0x001B2198 |
| 260 | 1:0x0011CB49 (linear 0x0012CB49) | 0x001B219D |
| 262 | 1:0x0011CB54 (linear 0x0012CB54) | 0x001B21A8 |
| 263 | 1:0x0011CB77 (linear 0x0012CB77) | 0x001B21CB |
| 267 | 1:0x0011CBA3 (linear 0x0012CBA3) | 0x001B21F7 |
| 269 | 1:0x0011CBBC (linear 0x0012CBBC) | 0x001B2210 |
| 271 | 1:0x0011CBC4 (linear 0x0012CBC4) | 0x001B2218 |
| 272 | 1:0x0011CBDB (linear 0x0012CBDB) | 0x001B222F |
| 274 | 1:0x0011CBF8 (linear 0x0012CBF8) | 0x001B224C |
| 275 | 1:0x0011CC01 (linear 0x0012CC01) | 0x001B2255 |
| 278 | 1:0x0011CC3B (linear 0x0012CC3B) | 0x001B228F |
| 281 | 1:0x0011CC5D (linear 0x0012CC5D) | 0x001B22B1 |
| 282 | 1:0x0011CC6B (linear 0x0012CC6B) | 0x001B22BF |
| 284 | 1:0x0011CC6D (linear 0x0012CC6D) | 0x001B22C1 |
| 285 | 1:0x0011CC77 (linear 0x0012CC77) | 0x001B22CB |
| 288 | 1:0x0011CC79 (linear 0x0012CC79) | 0x001B22CD |
| 289 | 1:0x0011CC83 (linear 0x0012CC83) | 0x001B22D7 |
| 292 | 1:0x0011CC85 (linear 0x0012CC85) | 0x001B22D9 |
| 298 | 1:0x0011CC8F (linear 0x0012CC8F) | 0x001B22E3 |
| 299 | 1:0x0011CC98 (linear 0x0012CC98) | 0x001B22EC |
| 302 | 1:0x0011CCAB (linear 0x0012CCAB) | 0x001B22FF |
| 304 | 1:0x0011CCC0 (linear 0x0012CCC0) | 0x001B2314 |
| 306 | 1:0x0011CCDA (linear 0x0012CCDA) | 0x001B232E |
| 308 | 1:0x0011CCE5 (linear 0x0012CCE5) | 0x001B2339 |
| 310 | 1:0x0011CCE7 (linear 0x0012CCE7) | 0x001B233B |
| 315 | 1:0x0011CCFA (linear 0x0012CCFA) | 0x001B234E |
| 317 | 1:0x0011CCFC (linear 0x0012CCFC) | 0x001B2350 |
| 318 | 1:0x0011CD11 (linear 0x0012CD11) | 0x001B2365 |
| 320 | 1:0x0011CD24 (linear 0x0012CD24) | 0x001B2378 |
| 323 | 1:0x0011CD2D (linear 0x0012CD2D) | 0x001B2381 |
| 333 | 1:0x0011CD45 (linear 0x0012CD45) | 0x001B2399 |
| 335 | 1:0x0011CD4D (linear 0x0012CD4D) | 0x001B23A1 |
| 338 | 1:0x0011CD57 (linear 0x0012CD57) | 0x001B23AB |
| 339 | 1:0x0011CD5E (linear 0x0012CD5E) | 0x001B23B2 |
| 341 | 1:0x0011CD65 (linear 0x0012CD65) | 0x001B23B9 |
| 343 | 1:0x0011CD6F (linear 0x0012CD6F) | 0x001B23C3 |
| 345 | 1:0x0011CD8A (linear 0x0012CD8A) | 0x001B23DE |
| 348 | 1:0x0011CD92 (linear 0x0012CD92) | 0x001B23E6 |
| 349 | 1:0x0011CDA5 (linear 0x0012CDA5) | 0x001B23F9 |
| 351 | 1:0x0011CDAA (linear 0x0012CDAA) | 0x001B23FE |
| 353 | 1:0x0011CDB5 (linear 0x0012CDB5) | 0x001B2409 |
| 354 | 1:0x0011CDD2 (linear 0x0012CDD2) | 0x001B2426 |
| 357 | 1:0x0011CDFE (linear 0x0012CDFE) | 0x001B2452 |
| 359 | 1:0x0011CE17 (linear 0x0012CE17) | 0x001B246B |
| 360 | 1:0x0011CE1F (linear 0x0012CE1F) | 0x001B2473 |
| 361 | 1:0x0011CE2E (linear 0x0012CE2E) | 0x001B2482 |
| 367 | 1:0x0011CE44 (linear 0x0012CE44) | 0x001B2498 |
| 369 | 1:0x0011CE57 (linear 0x0012CE57) | 0x001B24AB |
| 371 | 1:0x0011CE61 (linear 0x0012CE61) | 0x001B24B5 |
| 373 | 1:0x0011CE73 (linear 0x0012CE73) | 0x001B24C7 |
| 376 | 1:0x0011CE95 (linear 0x0012CE95) | 0x001B24E9 |
| 377 | 1:0x0011CEA3 (linear 0x0012CEA3) | 0x001B24F7 |
| 380 | 1:0x0011CEA5 (linear 0x0012CEA5) | 0x001B24F9 |
| 381 | 1:0x0011CEAF (linear 0x0012CEAF) | 0x001B2503 |
| 384 | 1:0x0011CEB1 (linear 0x0012CEB1) | 0x001B2505 |
| 385 | 1:0x0011CEBB (linear 0x0012CEBB) | 0x001B250F |
| 388 | 1:0x0011CEBD (linear 0x0012CEBD) | 0x001B2511 |
| 395 | 1:0x0011CEC7 (linear 0x0012CEC7) | 0x001B251B |
| 396 | 1:0x0011CED0 (linear 0x0012CED0) | 0x001B2524 |
| 398 | 1:0x0011CEE3 (linear 0x0012CEE3) | 0x001B2537 |
| 400 | 1:0x0011CEF8 (linear 0x0012CEF8) | 0x001B254C |
| 402 | 1:0x0011CF12 (linear 0x0012CF12) | 0x001B2566 |
| 404 | 1:0x0011CF1D (linear 0x0012CF1D) | 0x001B2571 |
| 406 | 1:0x0011CF1F (linear 0x0012CF1F) | 0x001B2573 |
| 411 | 1:0x0011CF32 (linear 0x0012CF32) | 0x001B2586 |
| 413 | 1:0x0011CF34 (linear 0x0012CF34) | 0x001B2588 |
| 414 | 1:0x0011CF49 (linear 0x0012CF49) | 0x001B259D |
| 417 | 1:0x0011CF5C (linear 0x0012CF5C) | 0x001B25B0 |
| 418 | 1:0x0011CF64 (linear 0x0012CF64) | 0x001B25B8 |
| 420 | 1:0x0011CF6D (linear 0x0012CF6D) | 0x001B25C1 |
| 430 | 1:0x0011CF85 (linear 0x0012CF85) | 0x001B25D9 |
| 432 | 1:0x0011CF8D (linear 0x0012CF8D) | 0x001B25E1 |
| 435 | 1:0x0011CF97 (linear 0x0012CF97) | 0x001B25EB |
| 436 | 1:0x0011CF9E (linear 0x0012CF9E) | 0x001B25F2 |
| 438 | 1:0x0011CFA5 (linear 0x0012CFA5) | 0x001B25F9 |
| 440 | 1:0x0011CFAF (linear 0x0012CFAF) | 0x001B2603 |
| 442 | 1:0x0011CFCA (linear 0x0012CFCA) | 0x001B261E |
| 445 | 1:0x0011CFD2 (linear 0x0012CFD2) | 0x001B2626 |
| 446 | 1:0x0011CFE5 (linear 0x0012CFE5) | 0x001B2639 |
| 448 | 1:0x0011CFEA (linear 0x0012CFEA) | 0x001B263E |
| 450 | 1:0x0011CFF5 (linear 0x0012CFF5) | 0x001B2649 |
| 451 | 1:0x0011D012 (linear 0x0012D012) | 0x001B2666 |
| 454 | 1:0x0011D03E (linear 0x0012D03E) | 0x001B2692 |
| 456 | 1:0x0011D057 (linear 0x0012D057) | 0x001B26AB |
| 457 | 1:0x0011D05F (linear 0x0012D05F) | 0x001B26B3 |
| 458 | 1:0x0011D06E (linear 0x0012D06E) | 0x001B26C2 |
| 464 | 1:0x0011D084 (linear 0x0012D084) | 0x001B26D8 |
| 466 | 1:0x0011D097 (linear 0x0012D097) | 0x001B26EB |
| 468 | 1:0x0011D0A1 (linear 0x0012D0A1) | 0x001B26F5 |
| 470 | 1:0x0011D0B3 (linear 0x0012D0B3) | 0x001B2707 |
| 473 | 1:0x0011D0D5 (linear 0x0012D0D5) | 0x001B2729 |
| 474 | 1:0x0011D0E3 (linear 0x0012D0E3) | 0x001B2737 |
| 477 | 1:0x0011D0E5 (linear 0x0012D0E5) | 0x001B2739 |
| 478 | 1:0x0011D0EF (linear 0x0012D0EF) | 0x001B2743 |
| 481 | 1:0x0011D0F1 (linear 0x0012D0F1) | 0x001B2745 |
| 482 | 1:0x0011D0FB (linear 0x0012D0FB) | 0x001B274F |
| 485 | 1:0x0011D0FD (linear 0x0012D0FD) | 0x001B2751 |
| 492 | 1:0x0011D107 (linear 0x0012D107) | 0x001B275B |
| 493 | 1:0x0011D110 (linear 0x0012D110) | 0x001B2764 |
| 495 | 1:0x0011D123 (linear 0x0012D123) | 0x001B2777 |
| 497 | 1:0x0011D138 (linear 0x0012D138) | 0x001B278C |
| 498 | 1:0x0011D155 (linear 0x0012D155) | 0x001B27A9 |
| 499 | 1:0x0011D160 (linear 0x0012D160) | 0x001B27B4 |
| 500 | 1:0x0011D162 (linear 0x0012D162) | 0x001B27B6 |
| 501 | 1:0x0011D177 (linear 0x0012D177) | 0x001B27CB |
| 504 | 1:0x0011D18A (linear 0x0012D18A) | 0x001B27DE |
| 505 | 1:0x0011D18C (linear 0x0012D18C) | 0x001B27E0 |
| 506 | 1:0x0011D1A1 (linear 0x0012D1A1) | 0x001B27F5 |
| 508 | 1:0x0011D1B4 (linear 0x0012D1B4) | 0x001B2808 |
| 509 | 1:0x0011D1BC (linear 0x0012D1BC) | 0x001B2810 |
| 516 | 1:0x0011D1C5 (linear 0x0012D1C5) | 0x001B2819 |
| 528 | 1:0x0011D1E4 (linear 0x0012D1E4) | 0x001B2838 |
| 530 | 1:0x0011D1F7 (linear 0x0012D1F7) | 0x001B284B |
| 532 | 1:0x0011D20F (linear 0x0012D20F) | 0x001B2863 |
| 534 | 1:0x0011D227 (linear 0x0012D227) | 0x001B287B |
| 536 | 1:0x0011D23B (linear 0x0012D23B) | 0x001B288F |
| 539 | 1:0x0011D24D (linear 0x0012D24D) | 0x001B28A1 |
| 541 | 1:0x0011D267 (linear 0x0012D267) | 0x001B28BB |
| 543 | 1:0x0011D27B (linear 0x0012D27B) | 0x001B28CF |
| 544 | 1:0x0011D284 (linear 0x0012D284) | 0x001B28D8 |
| 546 | 1:0x0011D295 (linear 0x0012D295) | 0x001B28E9 |
| 547 | 1:0x0011D2A5 (linear 0x0012D2A5) | 0x001B28F9 |
| 549 | 1:0x0011D2B3 (linear 0x0012D2B3) | 0x001B2907 |
| 551 | 1:0x0011D2D0 (linear 0x0012D2D0) | 0x001B2924 |
| 553 | 1:0x0011D2E0 (linear 0x0012D2E0) | 0x001B2934 |
| 559 | 1:0x0011D2F1 (linear 0x0012D2F1) | 0x001B2945 |
| 561 | 1:0x0011D300 (linear 0x0012D300) | 0x001B2954 |
| 563 | 1:0x0011D321 (linear 0x0012D321) | 0x001B2975 |
| 564 | 1:0x0011D336 (linear 0x0012D336) | 0x001B298A |
| 566 | 1:0x0011D347 (linear 0x0012D347) | 0x001B299B |
| 567 | 1:0x0011D355 (linear 0x0012D355) | 0x001B29A9 |
| 569 | 1:0x0011D366 (linear 0x0012D366) | 0x001B29BA |
| 570 | 1:0x0011D37B (linear 0x0012D37B) | 0x001B29CF |
| 571 | 1:0x0011D390 (linear 0x0012D390) | 0x001B29E4 |
| 573 | 1:0x0011D399 (linear 0x0012D399) | 0x001B29ED |
| 574 | 1:0x0011D3A9 (linear 0x0012D3A9) | 0x001B29FD |
| 575 | 1:0x0011D3B9 (linear 0x0012D3B9) | 0x001B2A0D |
| 577 | 1:0x0011D3C9 (linear 0x0012D3C9) | 0x001B2A1D |
| 579 | 1:0x0011D3E0 (linear 0x0012D3E0) | 0x001B2A34 |
| 581 | 1:0x0011D401 (linear 0x0012D401) | 0x001B2A55 |
| 589 | 1:0x0011D408 (linear 0x0012D408) | 0x001B2A5C |
| 598 | 1:0x0011D423 (linear 0x0012D423) | 0x001B2A77 |
| 599 | 1:0x0011D42A (linear 0x0012D42A) | 0x001B2A7E |
| 601 | 1:0x0011D431 (linear 0x0012D431) | 0x001B2A85 |
| 603 | 1:0x0011D447 (linear 0x0012D447) | 0x001B2A9B |
| 604 | 1:0x0011D459 (linear 0x0012D459) | 0x001B2AAD |
| 606 | 1:0x0011D463 (linear 0x0012D463) | 0x001B2AB7 |
| 607 | 1:0x0011D476 (linear 0x0012D476) | 0x001B2ACA |
| 609 | 1:0x0011D47B (linear 0x0012D47B) | 0x001B2ACF |
| 611 | 1:0x0011D486 (linear 0x0012D486) | 0x001B2ADA |
| 612 | 1:0x0011D4A9 (linear 0x0012D4A9) | 0x001B2AFD |
| 616 | 1:0x0011D4D5 (linear 0x0012D4D5) | 0x001B2B29 |
| 618 | 1:0x0011D4EE (linear 0x0012D4EE) | 0x001B2B42 |
| 620 | 1:0x0011D4F6 (linear 0x0012D4F6) | 0x001B2B4A |
| 621 | 1:0x0011D50D (linear 0x0012D50D) | 0x001B2B61 |
| 623 | 1:0x0011D52A (linear 0x0012D52A) | 0x001B2B7E |
| 624 | 1:0x0011D533 (linear 0x0012D533) | 0x001B2B87 |
| 627 | 1:0x0011D56F (linear 0x0012D56F) | 0x001B2BC3 |
| 629 | 1:0x0011D595 (linear 0x0012D595) | 0x001B2BE9 |
| 630 | 1:0x0011D5A5 (linear 0x0012D5A5) | 0x001B2BF9 |
| 631 | 1:0x0011D5B5 (linear 0x0012D5B5) | 0x001B2C09 |
| 632 | 1:0x0011D5C2 (linear 0x0012D5C2) | 0x001B2C16 |
| 633 | 1:0x0011D5CA (linear 0x0012D5CA) | 0x001B2C1E |
| 634 | 1:0x0011D5CC (linear 0x0012D5CC) | 0x001B2C20 |
| 635 | 1:0x0011D5D4 (linear 0x0012D5D4) | 0x001B2C28 |
| 636 | 1:0x0011D5DD (linear 0x0012D5DD) | 0x001B2C31 |
| 637 | 1:0x0011D5F0 (linear 0x0012D5F0) | 0x001B2C44 |
| 639 | 1:0x0011D5F2 (linear 0x0012D5F2) | 0x001B2C46 |
| 644 | 1:0x0011D5FC (linear 0x0012D5FC) | 0x001B2C50 |
| 645 | 1:0x0011D605 (linear 0x0012D605) | 0x001B2C59 |
| 647 | 1:0x0011D618 (linear 0x0012D618) | 0x001B2C6C |
| 649 | 1:0x0011D62D (linear 0x0012D62D) | 0x001B2C81 |
| 650 | 1:0x0011D64A (linear 0x0012D64A) | 0x001B2C9E |
| 651 | 1:0x0011D655 (linear 0x0012D655) | 0x001B2CA9 |
| 652 | 1:0x0011D657 (linear 0x0012D657) | 0x001B2CAB |
| 653 | 1:0x0011D66C (linear 0x0012D66C) | 0x001B2CC0 |
| 656 | 1:0x0011D67F (linear 0x0012D67F) | 0x001B2CD3 |
| 657 | 1:0x0011D681 (linear 0x0012D681) | 0x001B2CD5 |
| 658 | 1:0x0011D696 (linear 0x0012D696) | 0x001B2CEA |
| 659 | 1:0x0011D6A9 (linear 0x0012D6A9) | 0x001B2CFD |
| 669 | 1:0x0011D6B2 (linear 0x0012D6B2) | 0x001B2D06 |
| 671 | 1:0x0011D6CD (linear 0x0012D6CD) | 0x001B2D21 |
| 673 | 1:0x0011D6D9 (linear 0x0012D6D9) | 0x001B2D2D |
| 674 | 1:0x0011D6E8 (linear 0x0012D6E8) | 0x001B2D3C |
| 676 | 1:0x0011D6F5 (linear 0x0012D6F5) | 0x001B2D49 |
| 677 | 1:0x0011D702 (linear 0x0012D702) | 0x001B2D56 |
| 679 | 1:0x0011D70B (linear 0x0012D70B) | 0x001B2D5F |
| 681 | 1:0x0011D723 (linear 0x0012D723) | 0x001B2D77 |
| 683 | 1:0x0011D72D (linear 0x0012D72D) | 0x001B2D81 |
| 684 | 1:0x0011D739 (linear 0x0012D739) | 0x001B2D8D |
| 686 | 1:0x0011D745 (linear 0x0012D745) | 0x001B2D99 |
| 689 | 1:0x0011D760 (linear 0x0012D760) | 0x001B2DB4 |
| 691 | 1:0x0011D776 (linear 0x0012D776) | 0x001B2DCA |
| 692 | 1:0x0011D782 (linear 0x0012D782) | 0x001B2DD6 |
| 695 | 1:0x0011D78E (linear 0x0012D78E) | 0x001B2DE2 |
| 697 | 1:0x0011D7A6 (linear 0x0012D7A6) | 0x001B2DFA |
| 698 | 1:0x0011D7B0 (linear 0x0012D7B0) | 0x001B2E04 |
| 700 | 1:0x0011D7B7 (linear 0x0012D7B7) | 0x001B2E0B |
| 702 | 1:0x0011D7D2 (linear 0x0012D7D2) | 0x001B2E26 |
| 703 | 1:0x0011D7DA (linear 0x0012D7DA) | 0x001B2E2E |
| 706 | 1:0x0011D7E3 (linear 0x0012D7E3) | 0x001B2E37 |
| 708 | 1:0x0011D7FE (linear 0x0012D7FE) | 0x001B2E52 |
| 709 | 1:0x0011D811 (linear 0x0012D811) | 0x001B2E65 |
| 710 | 1:0x0011D823 (linear 0x0012D823) | 0x001B2E77 |
| 712 | 1:0x0011D82D (linear 0x0012D82D) | 0x001B2E81 |
| 713 | 1:0x0011D840 (linear 0x0012D840) | 0x001B2E94 |
| 715 | 1:0x0011D845 (linear 0x0012D845) | 0x001B2E99 |
| 716 | 1:0x0011D84E (linear 0x0012D84E) | 0x001B2EA2 |
| 718 | 1:0x0011D866 (linear 0x0012D866) | 0x001B2EBA |
| 719 | 1:0x0011D870 (linear 0x0012D870) | 0x001B2EC4 |
| 720 | 1:0x0011D883 (linear 0x0012D883) | 0x001B2ED7 |
| 721 | 1:0x0011D888 (linear 0x0012D888) | 0x001B2EDC |
| 723 | 1:0x0011D88F (linear 0x0012D88F) | 0x001B2EE3 |
| 725 | 1:0x0011D8AC (linear 0x0012D8AC) | 0x001B2F00 |
| 727 | 1:0x0011D8C4 (linear 0x0012D8C4) | 0x001B2F18 |
| 728 | 1:0x0011D8D3 (linear 0x0012D8D3) | 0x001B2F27 |
| 730 | 1:0x0011D8E0 (linear 0x0012D8E0) | 0x001B2F34 |
| 731 | 1:0x0011D8ED (linear 0x0012D8ED) | 0x001B2F41 |

