# F:\WLIBS\allocate\allocate.c

Module 136 of the debug info of ORION2.EXE.

Ranges: 1:0x00100B89 (linear 0x00110B89), 2,382 bytes, 2:0x00002418 (linear 0x00172418), 248 bytes, 2:0x0000C378 (linear 0x0017C378), 8 bytes, 2:0x000356C4 (linear 0x001A56C4), 320 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x00100B89 (linear 0x00110B89) | code | Allocate_Space_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100C29 (linear 0x00110C29) | code | Get_Max_Block_Space_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100C62 (linear 0x00110C62) | code | Allocate_Dos_Space_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100CEE (linear 0x00110CEE) | code | Allocate_First_Block_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100D3C (linear 0x00110D3C) | code | Allocate_Next_Block_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100DFE (linear 0x00110DFE) | code | Allocate_Far_Space_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100E36 (linear 0x00110E36) | code | Allocate_Dos_Data_Space_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100EC3 (linear 0x00110EC3) | code | Allocation_Error_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100F3A (linear 0x00110F3A) | code | Reallocation_Error_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100F89 (linear 0x00110F89) | code | Linear_Free_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100FE7 (linear 0x00110FE7) | code | Check_Free_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00101090 (linear 0x00111090) | code | Dos_Free_ | `regions/07-object-1/00196654-00196E53.txt` |
| 1:0x00101131 (linear 0x00111131) | code | Allocate_Space_No_Header_ | `regions/07-object-1/00196654-00196E53.txt` |
| 1:0x00101188 (linear 0x00111188) | code | Allocate_First_Block_No_Header_ | `regions/07-object-1/00196654-00196E53.txt` |
| 1:0x001011D6 (linear 0x001111D6) | code | Allocate_Next_Block_No_Header_ | `regions/07-object-1/00196654-00196E53.txt` |
| 1:0x0010127C (linear 0x0011127C) | code | Mark_Block_ | `regions/07-object-1/00196654-00196E53.txt` |
| 1:0x001012B4 (linear 0x001112B4) | code | Release_Block_ | `regions/07-object-1/00196654-00196E53.txt` |
| 1:0x001012EC (linear 0x001112EC) | code | Push_Block_ | `regions/07-object-1/00196654-00196E53.txt` |
| 1:0x00101358 (linear 0x00111358) | code | Save_Pushed_Blocks_ | `regions/07-object-1/00196654-00196E53.txt` |
| 1:0x00101392 (linear 0x00111392) | code | Restore_Pushed_Blocks_ | `regions/07-object-1/00196654-00196E53.txt` |
| 1:0x001013CC (linear 0x001113CC) | code | Pop_Block_ | `regions/07-object-1/00196654-00196E53.txt` |
| 1:0x00101430 (linear 0x00111430) | code | Check_For_Pushed_Block_ | `regions/07-object-1/00196654-00196E53.txt` |
| 1:0x00101499 (linear 0x00111499) | code | Get_Remaining_Block_Space_ | `regions/07-object-1/00196654-00196E53.txt` |
| 2:0x0000C378 (linear 0x0017C378) | data | _mark_stack_pos | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x0000C37C (linear 0x0017C37C) | data | _called | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x000356C4 (linear 0x001A56C4) | data | _mark_stack | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x000356C4 (linear 0x001A56C4) | mark_stack | struct s_mark_stack[40] |
| 2:0x0000C378 (linear 0x0017C378) | mark_stack_pos | int |
| 2:0x0000C37C (linear 0x0017C37C) | called | int |

#### Allocate_Space

- Address: 1:0x00100B89 (linear 0x00110B89), 160 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: char * (int); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| size | [EBP-0x14] | int |
| name | [EBP-0x10] | char * |
| header | [EBP-0xC] | long * |
| lsize | [EBP-0x8] | int |

#### Get_Max_Block_Space

- Address: 1:0x00100C29 (linear 0x00110C29), 57 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: int (long *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0xC] | long * |
| header | [EBP-0x8] | long * |

#### Allocate_Dos_Space

- Address: 1:0x00100C62 (linear 0x00110C62), 140 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: char * (int); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| size | [EBP-0x14] | int |
| name | [EBP-0x10] | char * |
| header | [EBP-0xC] | long * |
| lsize | [EBP-0x8] | int |

#### Allocate_First_Block

- Address: 1:0x00100CEE (linear 0x00110CEE), 78 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: char * (long *, int); returns in EAX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0x14] | long * |
| size | [EBP-0x10] | int |
| name | [EBP-0xC] | char * |
| header | [EBP-0x8] | long * |

#### Allocate_Next_Block

- Address: 1:0x00100D3C (linear 0x00110D3C), 194 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: char * (long *, int); returns in EAX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0x1C] | long * |
| size | [EBP-0x18] | int |
| name | [EBP-0x14] | char * |
| header | [EBP-0x10] | long * |
| block_header | [EBP-0xC] | long * |
| lsize | [EBP-0x8] | int |

#### Allocate_Far_Space

- Address: 1:0x00100DFE (linear 0x00110DFE), 56 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: char * (int); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| size | [EBP-0xC] | int |
| name | [EBP-0x8] | char * |

#### Allocate_Dos_Data_Space

- Address: 1:0x00100E36 (linear 0x00110E36), 141 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: char * (int); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| size | [EBP-0xC] | int |
| name | [EBP-0x8] | char * |
| i | [EBP-0x10] | int |
| inregs | [EBP-0x48] | union REGS |
| outregs | [EBP-0x2C] | union REGS |

#### Allocation_Error

- Address: 1:0x00100EC3 (linear 0x00110EC3), 119 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: void (int)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| size | [EBP-0x10] | int |
| i | [EBP-0xC] | int |
| max_far_size | [EBP-0x8] | int |
| temp | [EBP-0x4] | char * |

#### Reallocation_Error

- Address: 1:0x00100F3A (linear 0x00110F3A), 79 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: void (int, int, int)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| size | [EBP-0x18] | int |
| short_by | [EBP-0x14] | int |
| max | [EBP-0x10] | int |
| i | [EBP-0xC] | int |
| max_far_size | [EBP-0x8] | int |
| temp | [EBP-0x4] | char * |

#### Linear_Free

- Address: 1:0x00100F89 (linear 0x00110F89), 94 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x8] | int |
| inregs | [EBP-0x24] | union REGS |
| outregs | [EBP-0x40] | union REGS |
| buffer | [EBP-0x7C] | int[15] |

#### Check_Free

- Address: 1:0x00100FE7 (linear 0x00110FE7), 169 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x10] | int |
| max_far_size | [EBP-0xC] | int |
| temp | [EBP-0x8] | char * |

#### Dos_Free

- Address: 1:0x00101090 (linear 0x00111090), 161 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| select | [EBP-0xC] | int |
| i | [EBP-0x8] | int |
| inregs | [EBP-0x44] | union REGS |
| outregs | [EBP-0x28] | union REGS |

#### Allocate_Space_No_Header

- Address: 1:0x00101131 (linear 0x00111131), 87 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: char * (int); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| size | [EBP-0xC] | int |
| name | [EBP-0x8] | char * |

#### Allocate_First_Block_No_Header

- Address: 1:0x00101188 (linear 0x00111188), 78 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: char * (long *, int); returns in EAX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0x14] | long * |
| size | [EBP-0x10] | int |
| name | [EBP-0xC] | char * |
| header | [EBP-0x8] | long * |

#### Allocate_Next_Block_No_Header

- Address: 1:0x001011D6 (linear 0x001111D6), 166 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: char * (long *, int); returns in EAX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0x1C] | long * |
| size | [EBP-0x18] | int |
| name | [EBP-0x14] | char * |
| header | [EBP-0x10] | long * |
| block_header | [EBP-0xC] | long * |
| lsize | [EBP-0x8] | int |

#### Mark_Block

- Address: 1:0x0010127C (linear 0x0011127C), 56 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: void (long *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0x8] | long * |
| header | [EBP-0x4] | long * |

#### Release_Block

- Address: 1:0x001012B4 (linear 0x001112B4), 56 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: void (long *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0x8] | long * |
| header | [EBP-0x4] | long * |

#### Push_Block

- Address: 1:0x001012EC (linear 0x001112EC), 108 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: void (long *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0x8] | long * |
| header | [EBP-0x4] | long * |

#### Save_Pushed_Blocks

- Address: 1:0x00101358 (linear 0x00111358), 58 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: void (void *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| saved_blocks | [EBP-0x4] | void * |

#### Restore_Pushed_Blocks

- Address: 1:0x00101392 (linear 0x00111392), 58 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: void (void *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| saved_blocks | [EBP-0x4] | void * |

#### Pop_Block

- Address: 1:0x001013CC (linear 0x001113CC), 100 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: void (long *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0x8] | long * |
| header | [EBP-0x4] | long * |

#### Check_For_Pushed_Block

- Address: 1:0x00101430 (linear 0x00111430), 105 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: short (long *); returns in AX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0xC] | long * |
| i | [EBP-0x8] | short |

#### Get_Remaining_Block_Space

- Address: 1:0x00101499 (linear 0x00111499), 62 bytes, in `regions/07-object-1/00196654-00196E53.txt`
- Type: int (long *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0xC] | long * |
| header | [EBP-0x8] | long * |

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
| 19 | typedef | wchar_t | name for unsigned short (#7) |
| 20 | typedef | wchar_t | name for unsigned short (#7) |
| 21 | scalar | void | void 1 bytes |
| 22 | procedure |  | near386; returns void () |
| 23 | pointer | onexit_t | near386; of void (void) (#22) |
| 24 | typedef | onexit_t | name for void (void) * (#23) |
| 25 | scalar | unsigned long | unsigned 4 bytes |
| 26 | typedef | time_t | name for unsigned long (#25) |
| 27 | typedef | ino_t | name for unsigned int (#9) |
| 28 | typedef | dev_t | name for int (#8) |
| 29 | typedef | off_t | name for long (#16) |
| 30 | typedef | clock_t | name for unsigned long (#25) |
| 31 | typedef | time_t | name for unsigned long (#25) |
| 32 | typedef | BYTE | name for unsigned char (#5) |
| 33 | typedef | SBYTE | name for signed char (#4) |
| 34 | typedef | UBYTE | name for unsigned char (#5) |
| 35 | typedef | WORD | name for unsigned short (#7) |
| 36 | typedef | SWORD | name for short (#6) |
| 37 | typedef | UWORD | name for unsigned short (#7) |
| 38 | typedef | DWORD | name for unsigned int (#9) |
| 39 | typedef | SDWORD | name for int (#8) |
| 40 | typedef | UDWORD | name for unsigned int (#9) |
| 41 | typedef | BOOL | name for unsigned char (#5) |
| 42 | typedef | SampleHandle | name for int (#8) |
| 43 | pointer |  | near386; of void (#21) |
| 44 | struct | struct s_mark_stack | 2 fields, 8 bytes |
| 45 | typedef | s_mark_stack | struct name for struct #44 (#44) |
| 46 | array |  | 0..39; of struct s_mark_stack (#45) |
| 47 | procedure |  | near386; returns char * (int) |
| 48 | pointer |  | near386; of long (#16) |
| 49 | procedure |  | near386; returns int (long *) |
| 50 | procedure |  | near386; returns char * (long *, int) |
| 51 | struct | struct DWORDREGS | 7 fields, 28 bytes |
| 52 | typedef | DWORDREGS | struct name for struct #51 (#51) |
| 53 | struct | struct WORDREGS | 13 fields, 28 bytes |
| 54 | typedef | WORDREGS | struct name for struct #53 (#53) |
| 55 | struct | struct BYTEREGS | 12 fields, 16 bytes |
| 56 | typedef | BYTEREGS | struct name for struct #55 (#55) |
| 57 | struct | union REGS | 3 fields, 28 bytes |
| 58 | typedef | REGS | union name for struct #57 (#57) |
| 59 | procedure |  | near386; returns void (int) |
| 60 | procedure |  | near386; returns void (int, int, int) |
| 61 | procedure |  | near386; returns int () |
| 62 | array |  | 0..14; of int (#8) |
| 63 | procedure |  | near386; returns void (long *) |
| 64 | procedure |  | near386; returns void (void *) |
| 65 | procedure |  | near386; returns short (long *) |

##### struct s_mark_stack (#44, 2 fields, 8 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| block | offset 0 | void * |
| mark | offset 4 | int |

##### struct DWORDREGS (#51, 7 fields, 28 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| eax | offset 0 | unsigned int |
| ebx | offset 4 | unsigned int |
| ecx | offset 8 | unsigned int |
| edx | offset 12 | unsigned int |
| esi | offset 16 | unsigned int |
| edi | offset 20 | unsigned int |
| cflag | offset 24 | unsigned int |

##### struct WORDREGS (#53, 13 fields, 28 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| ax | offset 0 | unsigned short |
| _1 | offset 2 | unsigned short |
| bx | offset 4 | unsigned short |
| _2 | offset 6 | unsigned short |
| cx | offset 8 | unsigned short |
| _3 | offset 10 | unsigned short |
| dx | offset 12 | unsigned short |
| _4 | offset 14 | unsigned short |
| si | offset 16 | unsigned short |
| _5 | offset 18 | unsigned short |
| di | offset 20 | unsigned short |
| _6 | offset 22 | unsigned short |
| cflag | offset 24 | unsigned int |

##### struct BYTEREGS (#55, 12 fields, 16 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| al | offset 0 | unsigned char |
| ah | offset 1 | unsigned char |
| _1 | offset 2 | unsigned short |
| bl | offset 4 | unsigned char |
| bh | offset 5 | unsigned char |
| _2 | offset 6 | unsigned short |
| cl | offset 8 | unsigned char |
| ch | offset 9 | unsigned char |
| _3 | offset 10 | unsigned short |
| dl | offset 12 | unsigned char |
| dh | offset 13 | unsigned char |
| _4 | offset 14 | unsigned short |

##### union REGS (#57, 3 fields, 28 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| h | offset 0 | struct BYTEREGS |
| w | offset 0 | struct WORDREGS |
| x | offset 0 | struct DWORDREGS |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 39 | 1:0x00100B89 (linear 0x00110B89) | 0x001961DD |
| 45 | 1:0x00100BA4 (linear 0x00110BA4) | 0x001961F8 |
| 46 | 1:0x00100BAA (linear 0x00110BAA) | 0x001961FE |
| 48 | 1:0x00100BC2 (linear 0x00110BC2) | 0x00196216 |
| 49 | 1:0x00100BD0 (linear 0x00110BD0) | 0x00196224 |
| 50 | 1:0x00100BE4 (linear 0x00110BE4) | 0x00196238 |
| 52 | 1:0x00100BF2 (linear 0x00110BF2) | 0x00196246 |
| 54 | 1:0x00100BF8 (linear 0x00110BF8) | 0x0019624C |
| 55 | 1:0x00100C01 (linear 0x00110C01) | 0x00196255 |
| 56 | 1:0x00100C0A (linear 0x00110C0A) | 0x0019625E |
| 58 | 1:0x00100C14 (linear 0x00110C14) | 0x00196268 |
| 59 | 1:0x00100C1D (linear 0x00110C1D) | 0x00196271 |
| 66 | 1:0x00100C29 (linear 0x00110C29) | 0x0019627D |
| 69 | 1:0x00100C44 (linear 0x00110C44) | 0x00196298 |
| 71 | 1:0x00100C4D (linear 0x00110C4D) | 0x001962A1 |
| 72 | 1:0x00100C56 (linear 0x00110C56) | 0x001962AA |
| 76 | 1:0x00100C62 (linear 0x00110C62) | 0x001962B6 |
| 82 | 1:0x00100C7D (linear 0x00110C7D) | 0x001962D1 |
| 84 | 1:0x00100C83 (linear 0x00110C83) | 0x001962D7 |
| 87 | 1:0x00100C9B (linear 0x00110C9B) | 0x001962EF |
| 89 | 1:0x00100CA9 (linear 0x00110CA9) | 0x001962FD |
| 91 | 1:0x00100CB7 (linear 0x00110CB7) | 0x0019630B |
| 93 | 1:0x00100CBD (linear 0x00110CBD) | 0x00196311 |
| 94 | 1:0x00100CC6 (linear 0x00110CC6) | 0x0019631A |
| 95 | 1:0x00100CCF (linear 0x00110CCF) | 0x00196323 |
| 97 | 1:0x00100CD9 (linear 0x00110CD9) | 0x0019632D |
| 98 | 1:0x00100CE2 (linear 0x00110CE2) | 0x00196336 |
| 104 | 1:0x00100CEE (linear 0x00110CEE) | 0x00196342 |
| 109 | 1:0x00100D0B (linear 0x00110D0B) | 0x0019635F |
| 111 | 1:0x00100D14 (linear 0x00110D14) | 0x00196368 |
| 113 | 1:0x00100D1D (linear 0x00110D1D) | 0x00196371 |
| 114 | 1:0x00100D2B (linear 0x00110D2B) | 0x0019637F |
| 115 | 1:0x00100D31 (linear 0x00110D31) | 0x00196385 |
| 124 | 1:0x00100D3C (linear 0x00110D3C) | 0x00196390 |
| 130 | 1:0x00100D59 (linear 0x00110D59) | 0x001963AD |
| 131 | 1:0x00100D5F (linear 0x00110D5F) | 0x001963B3 |
| 133 | 1:0x00100D77 (linear 0x00110D77) | 0x001963CB |
| 135 | 1:0x00100D80 (linear 0x00110D80) | 0x001963D4 |
| 137 | 1:0x00100DAF (linear 0x00110DAF) | 0x00196403 |
| 138 | 1:0x00100DB5 (linear 0x00110DB5) | 0x00196409 |
| 140 | 1:0x00100DBD (linear 0x00110DBD) | 0x00196411 |
| 142 | 1:0x00100DC8 (linear 0x00110DC8) | 0x0019641C |
| 144 | 1:0x00100DCE (linear 0x00110DCE) | 0x00196422 |
| 145 | 1:0x00100DD7 (linear 0x00110DD7) | 0x0019642B |
| 146 | 1:0x00100DE0 (linear 0x00110DE0) | 0x00196434 |
| 148 | 1:0x00100DEA (linear 0x00110DEA) | 0x0019643E |
| 149 | 1:0x00100DF3 (linear 0x00110DF3) | 0x00196447 |
| 152 | 1:0x00100DFE (linear 0x00110DFE) | 0x00196452 |
| 157 | 1:0x00100E19 (linear 0x00110E19) | 0x0019646D |
| 160 | 1:0x00100E24 (linear 0x00110E24) | 0x00196478 |
| 161 | 1:0x00100E2A (linear 0x00110E2A) | 0x0019647E |
| 163 | 1:0x00100E36 (linear 0x00110E36) | 0x0019648A |
| 172 | 1:0x00100E51 (linear 0x00110E51) | 0x001964A5 |
| 173 | 1:0x00100E5E (linear 0x00110E5E) | 0x001964B2 |
| 175 | 1:0x00100E6A (linear 0x00110E6A) | 0x001964BE |
| 177 | 1:0x00100E7C (linear 0x00110E7C) | 0x001964D0 |
| 178 | 1:0x00100E83 (linear 0x00110E83) | 0x001964D7 |
| 181 | 1:0x00100E89 (linear 0x00110E89) | 0x001964DD |
| 183 | 1:0x00100E99 (linear 0x00110E99) | 0x001964ED |
| 184 | 1:0x00100E9F (linear 0x00110E9F) | 0x001964F3 |
| 185 | 1:0x00100EA6 (linear 0x00110EA6) | 0x001964FA |
| 186 | 1:0x00100EA8 (linear 0x00110EA8) | 0x001964FC |
| 188 | 1:0x00100EB1 (linear 0x00110EB1) | 0x00196505 |
| 189 | 1:0x00100EB7 (linear 0x00110EB7) | 0x0019650B |
| 191 | 1:0x00100EC3 (linear 0x00110EC3) | 0x00196517 |
| 196 | 1:0x00100EDE (linear 0x00110EDE) | 0x00196532 |
| 198 | 1:0x00100EE3 (linear 0x00110EE3) | 0x00196537 |
| 199 | 1:0x00100EF1 (linear 0x00110EF1) | 0x00196545 |
| 200 | 1:0x00100EFF (linear 0x00110EFF) | 0x00196553 |
| 201 | 1:0x00100F13 (linear 0x00110F13) | 0x00196567 |
| 202 | 1:0x00100F27 (linear 0x00110F27) | 0x0019657B |
| 203 | 1:0x00100F31 (linear 0x00110F31) | 0x00196585 |
| 205 | 1:0x00100F3A (linear 0x00110F3A) | 0x0019658E |
| 210 | 1:0x00100F59 (linear 0x00110F59) | 0x001965AD |
| 212 | 1:0x00100F5E (linear 0x00110F5E) | 0x001965B2 |
| 213 | 1:0x00100F78 (linear 0x00110F78) | 0x001965CC |
| 214 | 1:0x00100F82 (linear 0x00110F82) | 0x001965D6 |
| 216 | 1:0x00100F89 (linear 0x00110F89) | 0x001965DD |
| 224 | 1:0x00100FA1 (linear 0x00110FA1) | 0x001965F5 |
| 226 | 1:0x00100FB0 (linear 0x00110FB0) | 0x00196604 |
| 227 | 1:0x00100FB7 (linear 0x00110FB7) | 0x0019660B |
| 229 | 1:0x00100FBD (linear 0x00110FBD) | 0x00196611 |
| 231 | 1:0x00100FCD (linear 0x00110FCD) | 0x00196621 |
| 232 | 1:0x00100FDB (linear 0x00110FDB) | 0x0019662F |
| 257 | 1:0x00100FE7 (linear 0x00110FE7) | 0x0019663B |
| 262 | 1:0x00100FFF (linear 0x00110FFF) | 0x00196653 |
| 263 | 1:0x00101006 (linear 0x00111006) | 0x0019665A |
| 264 | 1:0x00101011 (linear 0x00111011) | 0x00196665 |
| 266 | 1:0x00101019 (linear 0x00111019) | 0x0019666D |
| 268 | 1:0x0010101F (linear 0x0011101F) | 0x00196673 |
| 269 | 1:0x00101026 (linear 0x00111026) | 0x0019667A |
| 270 | 1:0x00101031 (linear 0x00111031) | 0x00196685 |
| 271 | 1:0x00101039 (linear 0x00111039) | 0x0019668D |
| 272 | 1:0x0010103B (linear 0x0011103B) | 0x0019668F |
| 273 | 1:0x00101042 (linear 0x00111042) | 0x00196696 |
| 274 | 1:0x0010104D (linear 0x0011104D) | 0x001966A1 |
| 276 | 1:0x00101055 (linear 0x00111055) | 0x001966A9 |
| 278 | 1:0x0010105B (linear 0x0011105B) | 0x001966AF |
| 279 | 1:0x00101062 (linear 0x00111062) | 0x001966B6 |
| 280 | 1:0x0010106D (linear 0x0011106D) | 0x001966C1 |
| 281 | 1:0x00101075 (linear 0x00111075) | 0x001966C9 |
| 283 | 1:0x00101077 (linear 0x00111077) | 0x001966CB |
| 284 | 1:0x00101084 (linear 0x00111084) | 0x001966D8 |
| 288 | 1:0x00101090 (linear 0x00111090) | 0x001966E4 |
| 295 | 1:0x001010A8 (linear 0x001110A8) | 0x001966FC |
| 297 | 1:0x001010C0 (linear 0x001110C0) | 0x00196714 |
| 298 | 1:0x001010C7 (linear 0x001110C7) | 0x0019671B |
| 300 | 1:0x001010CD (linear 0x001110CD) | 0x00196721 |
| 301 | 1:0x001010DD (linear 0x001110DD) | 0x00196731 |
| 302 | 1:0x001010E3 (linear 0x001110E3) | 0x00196737 |
| 304 | 1:0x001010E9 (linear 0x001110E9) | 0x0019673D |
| 308 | 1:0x001010FF (linear 0x001110FF) | 0x00196753 |
| 309 | 1:0x00101106 (linear 0x00111106) | 0x0019675A |
| 310 | 1:0x0010110C (linear 0x0011110C) | 0x00196760 |
| 313 | 1:0x0010111C (linear 0x0011111C) | 0x00196770 |
| 314 | 1:0x0010111E (linear 0x0011111E) | 0x00196772 |
| 315 | 1:0x00101125 (linear 0x00111125) | 0x00196779 |
| 319 | 1:0x00101131 (linear 0x00111131) | 0x00196785 |
| 323 | 1:0x0010114C (linear 0x0011114C) | 0x001967A0 |
| 324 | 1:0x00101157 (linear 0x00111157) | 0x001967AB |
| 325 | 1:0x00101168 (linear 0x00111168) | 0x001967BC |
| 327 | 1:0x00101176 (linear 0x00111176) | 0x001967CA |
| 328 | 1:0x0010117C (linear 0x0011117C) | 0x001967D0 |
| 333 | 1:0x00101188 (linear 0x00111188) | 0x001967DC |
| 338 | 1:0x001011A5 (linear 0x001111A5) | 0x001967F9 |
| 340 | 1:0x001011AE (linear 0x001111AE) | 0x00196802 |
| 342 | 1:0x001011B7 (linear 0x001111B7) | 0x0019680B |
| 343 | 1:0x001011C5 (linear 0x001111C5) | 0x00196819 |
| 344 | 1:0x001011CB (linear 0x001111CB) | 0x0019681F |
| 346 | 1:0x001011D6 (linear 0x001111D6) | 0x0019682A |
| 352 | 1:0x001011F3 (linear 0x001111F3) | 0x00196847 |
| 353 | 1:0x001011F9 (linear 0x001111F9) | 0x0019684D |
| 355 | 1:0x00101211 (linear 0x00111211) | 0x00196865 |
| 357 | 1:0x0010121A (linear 0x0011121A) | 0x0019686E |
| 359 | 1:0x00101233 (linear 0x00111233) | 0x00196887 |
| 360 | 1:0x00101239 (linear 0x00111239) | 0x0019688D |
| 362 | 1:0x00101241 (linear 0x00111241) | 0x00196895 |
| 364 | 1:0x00101249 (linear 0x00111249) | 0x0019689D |
| 366 | 1:0x0010124F (linear 0x0011124F) | 0x001968A3 |
| 367 | 1:0x00101258 (linear 0x00111258) | 0x001968AC |
| 368 | 1:0x00101261 (linear 0x00111261) | 0x001968B5 |
| 370 | 1:0x0010126B (linear 0x0011126B) | 0x001968BF |
| 371 | 1:0x00101271 (linear 0x00111271) | 0x001968C5 |
| 379 | 1:0x0010127C (linear 0x0011127C) | 0x001968D0 |
| 383 | 1:0x00101297 (linear 0x00111297) | 0x001968EB |
| 384 | 1:0x001012A0 (linear 0x001112A0) | 0x001968F4 |
| 385 | 1:0x001012AB (linear 0x001112AB) | 0x001968FF |
| 391 | 1:0x001012B4 (linear 0x001112B4) | 0x00196908 |
| 395 | 1:0x001012CF (linear 0x001112CF) | 0x00196923 |
| 396 | 1:0x001012D8 (linear 0x001112D8) | 0x0019692C |
| 397 | 1:0x001012E3 (linear 0x001112E3) | 0x00196937 |
| 404 | 1:0x001012EC (linear 0x001112EC) | 0x00196940 |
| 408 | 1:0x00101307 (linear 0x00111307) | 0x0019695B |
| 409 | 1:0x00101310 (linear 0x00111310) | 0x00196964 |
| 411 | 1:0x0010131A (linear 0x0011131A) | 0x0019696E |
| 412 | 1:0x00101323 (linear 0x00111323) | 0x00196977 |
| 413 | 1:0x00101335 (linear 0x00111335) | 0x00196989 |
| 415 | 1:0x00101349 (linear 0x00111349) | 0x0019699D |
| 416 | 1:0x0010134F (linear 0x0011134F) | 0x001969A3 |
| 417 | 1:0x00101358 (linear 0x00111358) | 0x001969AC |
| 419 | 1:0x00101373 (linear 0x00111373) | 0x001969C7 |
| 420 | 1:0x00101389 (linear 0x00111389) | 0x001969DD |
| 421 | 1:0x00101392 (linear 0x00111392) | 0x001969E6 |
| 423 | 1:0x001013AD (linear 0x001113AD) | 0x00196A01 |
| 424 | 1:0x001013C3 (linear 0x001113C3) | 0x00196A17 |
| 431 | 1:0x001013CC (linear 0x001113CC) | 0x00196A20 |
| 435 | 1:0x001013E7 (linear 0x001113E7) | 0x00196A3B |
| 436 | 1:0x001013F0 (linear 0x001113F0) | 0x00196A44 |
| 438 | 1:0x001013FA (linear 0x001113FA) | 0x00196A4E |
| 440 | 1:0x00101400 (linear 0x00111400) | 0x00196A54 |
| 441 | 1:0x00101414 (linear 0x00111414) | 0x00196A68 |
| 442 | 1:0x00101427 (linear 0x00111427) | 0x00196A7B |
| 444 | 1:0x00101430 (linear 0x00111430) | 0x00196A84 |
| 448 | 1:0x0010144B (linear 0x0011144B) | 0x00196A9F |
| 449 | 1:0x00101468 (linear 0x00111468) | 0x00196ABC |
| 451 | 1:0x00101486 (linear 0x00111486) | 0x00196ADA |
| 452 | 1:0x0010148D (linear 0x0011148D) | 0x00196AE1 |
| 459 | 1:0x00101499 (linear 0x00111499) | 0x00196AED |
| 462 | 1:0x001014B4 (linear 0x001114B4) | 0x00196B08 |
| 469 | 1:0x001014BD (linear 0x001114BD) | 0x00196B11 |
| 470 | 1:0x001014CB (linear 0x001114CB) | 0x00196B1F |

