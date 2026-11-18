# F:\WLIBS\sound\dllload.c

Module 307 of the debug info of ORION2.EXE.

Ranges: 1:0x00143FCF (linear 0x00153FCF), 3,505 bytes, 2:0x00004274 (linear 0x00174274), 4 bytes, 2:0x00011860 (linear 0x00181860), 12 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x00143FD7 (linear 0x00153FD7) | static, code | _DLL_read | `regions/07-object-1/001D8E54-001D9653.txt` |
| 1:0x0014403E (linear 0x0015403E) | code | _MEM_use_malloc | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x00144072 (linear 0x00154072) | code | _MEM_use_free | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x001440A6 (linear 0x001540A6) | code | _MEM_alloc_lock | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x001440EC (linear 0x001540EC) | code | _MEM_free_lock | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x00144124 (linear 0x00154124) | code | _MEM_alloc_DOS | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x001441D2 (linear 0x001541D2) | code | _MEM_free_DOS | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x0014421D (linear 0x0015421D) | code | _VMM_lock_range | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x001442DF (linear 0x001542DF) | code | _VMM_unlock_range | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x001443A1 (linear 0x001543A1) | code | _VMM_lock | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x001443D5 (linear 0x001543D5) | code | _VMM_unlock | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x00144409 (linear 0x00154409) | code | _DLL_size | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x0014455B (linear 0x0015455B) | code | _DLL_load | `regions/07-object-1/001D9654-001D9E53.txt` |
| 1:0x00144A74 (linear 0x00154A74) | code | _FILE_error | `regions/07-object-1/001D9E54-001DA653.txt` |
| 1:0x00144A9A (linear 0x00154A9A) | code | _FILE_size | `regions/07-object-1/001D9E54-001DA653.txt` |
| 1:0x00144B18 (linear 0x00154B18) | code | _FILE_read | `regions/07-object-1/001D9E54-001DA653.txt` |
| 1:0x00144C26 (linear 0x00154C26) | code | _FILE_write | `regions/07-object-1/001D9E54-001DA653.txt` |
| 1:0x00144CD4 (linear 0x00154CD4) | code | _FILE_append | `regions/07-object-1/001D9E54-001DA653.txt` |
| 2:0x00011860 (linear 0x00181860) | static, data | _disk_err | `regions/08-object-2/001FFE54-00200653.txt` |
| 2:0x00011864 (linear 0x00181864) | data | _MEM_alloc | `regions/08-object-2/001FFE54-00200653.txt` |
| 2:0x00011868 (linear 0x00181868) | data | _MEM_free | `regions/08-object-2/001FFE54-00200653.txt` |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x00011860 (linear 0x00181860) | disk_err | LONG |
| 2:0x00011864 (linear 0x00181864) | MEM_alloc | void * (size_t) * |
| 2:0x00011868 (linear 0x00181868) | MEM_free | void (void *) * |

#### DLL_read

- Address: 1:0x00143FD7 (linear 0x00153FD7), 103 bytes, in `regions/07-object-1/001D8E54-001D9653.txt`
- Type: void * (ULONG, ULONG, ULONG, void *, ULONG); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| src | [EBP+0x10] | ULONG |
| srcoff | [EBP+0x14] | ULONG |
| flags | [EBP+0x18] | ULONG |
| dest | [EBP+0x1C] | void * |
| length | [EBP+0x20] | ULONG |

#### MEM_use_malloc

- Address: 1:0x0014403E (linear 0x0015403E), 52 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: void * (void * (size_t) *); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| fn | [EBP+0x10] | void * (size_t) * |
| old | [EBP-0x8] | void * |

#### MEM_use_free

- Address: 1:0x00144072 (linear 0x00154072), 52 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: void * (void (void *) *); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| fn | [EBP+0x10] | void (void *) * |
| old | [EBP-0x8] | void * |

#### MEM_alloc_lock

- Address: 1:0x001440A6 (linear 0x001540A6), 70 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: void * (size_t); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| size | [EBP+0x10] | size_t |
| ptr | [EBP-0x8] | void * |

#### MEM_free_lock

- Address: 1:0x001440EC (linear 0x001540EC), 56 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: void (void *, size_t)
- Prologue 21 bytes, epilogue 4 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| ptr | [EBP+0x10] | void * |
| size | [EBP+0x14] | size_t |

#### MEM_alloc_DOS

- Address: 1:0x00144124 (linear 0x00154124), 174 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: LONG (ULONG, void * *, ULONG *, ULONG *); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| n_paras | [EBP+0x10] | ULONG |
| protected_ptr | [EBP+0x14] | void * * |
| segment_far_ptr | [EBP+0x18] | ULONG * |
| selector | [EBP+0x1C] | ULONG * |
| inregs | [EBP-0x2C] | union REGS |
| outregs | [EBP-0x48] | union REGS |
| success | [EBP-0x10] | LONG |
| p1 | [EBP-0xC] | void * |
| p2 | [EBP-0x8] | void * |

#### MEM_free_DOS

- Address: 1:0x001441D2 (linear 0x001541D2), 75 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: void (void *, ULONG, ULONG)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| protected_ptr | [EBP+0x10] | void * |
| segment_far_ptr | [EBP+0x14] | ULONG |
| selector | [EBP+0x18] | ULONG |
| inregs | [EBP-0x1C] | union REGS |
| outregs | [EBP-0x38] | union REGS |

#### VMM_lock_range

- Address: 1:0x0014421D (linear 0x0015421D), 194 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: LONG (void *, void *); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| p1 | [EBP+0x10] | void * |
| p2 | [EBP+0x14] | void * |
| success | [EBP-0x10] | LONG |
| inregs | [EBP-0x48] | union REGS |
| outregs | [EBP-0x2C] | union REGS |
| linear | [EBP-0xC] | ULONG |
| len | [EBP-0x8] | ULONG |

#### VMM_unlock_range

- Address: 1:0x001442DF (linear 0x001542DF), 194 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: LONG (void *, void *); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| p1 | [EBP+0x10] | void * |
| p2 | [EBP+0x14] | void * |
| success | [EBP-0x10] | LONG |
| inregs | [EBP-0x48] | union REGS |
| outregs | [EBP-0x2C] | union REGS |
| linear | [EBP-0xC] | ULONG |
| len | [EBP-0x8] | ULONG |

#### VMM_lock

- Address: 1:0x001443A1 (linear 0x001543A1), 52 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: LONG (void *, ULONG); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| start | [EBP+0x10] | void * |
| size | [EBP+0x14] | ULONG |

#### VMM_unlock

- Address: 1:0x001443D5 (linear 0x001543D5), 52 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: LONG (void *, ULONG); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| start | [EBP+0x10] | void * |
| size | [EBP+0x14] | ULONG |

#### DLL_size

- Address: 1:0x00144409 (linear 0x00154409), 338 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: ULONG (void *, ULONG); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| source | [EBP+0x10] | void * |
| flags | [EBP+0x14] | ULONG |
| cword | [EBP-0x1C] | SBYTE[4] |
| src_ptr | [EBP-0x18] | void * |
| LX_hdr | [EBP-0xE0] | LX_header_struct |
| object_table | [EBP-0x34] | object_table_struct |
| lx | [EBP-0x14] | ULONG |
| i | [EBP-0x10] | ULONG |
| LX_offset | [EBP-0xC] | ULONG |
| module_size | [EBP-0x8] | ULONG |

#### DLL_load

- Address: 1:0x0014455B (linear 0x0015455B), 1,305 bytes, in `regions/07-object-1/001D9654-001D9E53.txt`
- Type: void * (void *, ULONG, void *); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| source | [EBP+0x10] | void * |
| flags | [EBP+0x14] | ULONG |
| dll | [EBP+0x18] | void * |
| LX_hdr | [EBP-0x150] | LX_header_struct |
| src_ptr | [EBP-0x48] | void * |
| object_table_pos | [EBP-0x54] | void * |
| object_page_table_pos | [EBP-0x5C] | void * |
| fixup_page_table_pos | [EBP-0x58] | void * |
| object_table | [EBP-0x7C] | object_table_struct |
| object_page_table | [EBP-0x64] | object_page_table_struct |
| cword | [EBP-0x50] | SBYTE[4] |
| dll_ptr | [EBP-0x4C] | UBYTE * |
| object_ptr | [EBP-0xA4] | UBYTE *[10] |
| page_ptr | [EBP-0x2E0] | UBYTE *[100] |
| src | [EBP-0x4] | UBYTE |
| fix_flags | [EBP-0xC] | UBYTE |
| f_object | [EBP-0x8] | UBYTE |
| trg_val | [EBP-0x38] | UBYTE * |
| srcoff | [EBP-0x14] | UWORD |
| trgoff | [EBP-0x10] | UWORD |
| read_size | [EBP-0x2C] | ULONG |
| dll_size | [EBP-0x28] | ULONG |
| i | [EBP-0x24] | ULONG |
| j | [EBP-0x44] | ULONG |
| lx | [EBP-0x40] | ULONG |
| page_count | [EBP-0x3C] | ULONG |
| LX_offset | [EBP-0x20] | ULONG |
| fix_src_ptr | [EBP-0x34] | ULONG * |
| page_offset | [EBP-0x30] | ULONG |
| next_page_offset | [EBP-0x18] | ULONG |

#### FILE_error

- Address: 1:0x00144A74 (linear 0x00154A74), 38 bytes, in `regions/07-object-1/001D9E54-001DA653.txt`
- Type: LONG (void); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

No local variables.

#### FILE_size

- Address: 1:0x00144A9A (linear 0x00154A9A), 126 bytes, in `regions/07-object-1/001D9E54-001DA653.txt`
- Type: LONG (SBYTE *); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| filename | [EBP+0x10] | SBYTE * |
| handle | [EBP-0xC] | LONG |
| len | [EBP-0x8] | LONG |

#### FILE_read

- Address: 1:0x00144B18 (linear 0x00154B18), 270 bytes, in `regions/07-object-1/001D9E54-001DA653.txt`
- Type: void * (SBYTE *, void *); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| filename | [EBP+0x10] | SBYTE * |
| dest | [EBP+0x14] | void * |
| i | [EBP-0x18] | LONG |
| handle | [EBP-0x14] | LONG |
| len | [EBP-0x10] | LONG |
| buf | [EBP-0xC] | SBYTE * |
| mem | [EBP-0x8] | SBYTE * |

#### FILE_write

- Address: 1:0x00144C26 (linear 0x00154C26), 174 bytes, in `regions/07-object-1/001D9E54-001DA653.txt`
- Type: LONG (SBYTE *, void *, ULONG); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| filename | [EBP+0x10] | SBYTE * |
| buf | [EBP+0x14] | void * |
| len | [EBP+0x18] | ULONG |
| i | [EBP-0xC] | LONG |
| handle | [EBP-0x8] | LONG |

#### FILE_append

- Address: 1:0x00144CD4 (linear 0x00154CD4), 169 bytes, in `regions/07-object-1/001D9E54-001DA653.txt`
- Type: LONG (SBYTE *, void *, ULONG); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| filename | [EBP+0x10] | SBYTE * |
| buf | [EBP+0x14] | void * |
| len | [EBP+0x18] | ULONG |
| i | [EBP-0xC] | LONG |
| handle | [EBP-0x8] | LONG |

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
| 12 | scalar | unsigned long | unsigned 4 bytes |
| 13 | typedef | time_t | name for unsigned long (#12) |
| 14 | typedef | size_t | name for unsigned int (#9) |
| 15 | typedef | ino_t | name for unsigned int (#9) |
| 16 | typedef | dev_t | name for int (#8) |
| 17 | scalar | long | int 4 bytes |
| 18 | typedef | off_t | name for long (#17) |
| 19 | pointer |  | near386; of char (#10) |
| 20 | array | __va_list | 0..0; of char * (#19) |
| 21 | typedef | __va_list | name for char *[1] (#20) |
| 22 | typedef | fpos_t | name for long (#17) |
| 23 | typedef | fpos_t | name for long (#17) |
| 24 | typedef | wchar_t | name for unsigned short (#7) |
| 25 | typedef | wchar_t | name for unsigned short (#7) |
| 26 | scalar | void | void 1 bytes |
| 27 | procedure |  | near386; returns void () |
| 28 | pointer | onexit_t | near386; of void (void) (#27) |
| 29 | typedef | onexit_t | name for void (void) * (#28) |
| 30 | typedef | UBYTE | name for unsigned char (#5) |
| 31 | typedef | UWORD | name for unsigned short (#7) |
| 32 | typedef | ULONG | name for unsigned long (#12) |
| 33 | typedef | SBYTE | name for signed char (#4) |
| 34 | typedef | SWORD | name for short (#6) |
| 35 | typedef | LONG | name for long (#17) |
| 36 | pointer |  | near386; of void (#26) |
| 37 | procedure |  | near386; returns void * (size_t) |
| 38 | pointer |  | near386; of void * (size_t) (#37) |
| 39 | procedure |  | near386; returns void (void *) |
| 40 | pointer |  | near386; of void (void *) (#39) |
| 41 | procedure |  | near386; returns void * (ULONG, ULONG, ULONG, void *, ULONG) |
| 42 | pointer |  | near386; of void * (size_t) (#37) |
| 43 | procedure |  | near386; returns void * (void * (size_t) *) |
| 44 | pointer |  | near386; of void (void *) (#39) |
| 45 | procedure |  | near386; returns void * (void (void *) *) |
| 46 | procedure |  | near386; returns void * (size_t) |
| 47 | procedure |  | near386; returns void (void *, size_t) |
| 48 | pointer |  | near386; of void * (#36) |
| 49 | pointer |  | near386; of ULONG (#32) |
| 50 | procedure |  | near386; returns LONG (ULONG, void * *, ULONG *, ULONG *) |
| 51 | struct | struct DWORDREGS | 7 fields, 28 bytes |
| 52 | typedef | DWORDREGS | struct name for struct #51 (#51) |
| 53 | struct | struct WORDREGS | 13 fields, 28 bytes |
| 54 | typedef | WORDREGS | struct name for struct #53 (#53) |
| 55 | struct | struct BYTEREGS | 12 fields, 16 bytes |
| 56 | typedef | BYTEREGS | struct name for struct #55 (#55) |
| 57 | struct | union REGS | 3 fields, 28 bytes |
| 58 | typedef | REGS | union name for struct #57 (#57) |
| 59 | procedure |  | near386; returns void (void *, ULONG, ULONG) |
| 60 | procedure |  | near386; returns LONG (void *, void *) |
| 61 | procedure |  | near386; returns LONG (void *, ULONG) |
| 62 | procedure |  | near386; returns ULONG (void *, ULONG) |
| 63 | array |  | 0..3; of SBYTE (#33) |
| 64 | struct | LX_header_struct | 43 fields, 172 bytes |
| 65 | typedef | LX_header_struct | name for struct #64 (#64) |
| 66 | struct | object_table_struct | 6 fields, 24 bytes |
| 67 | typedef | object_table_struct | name for struct #66 (#66) |
| 68 | procedure |  | near386; returns void * (void *, ULONG, void *) |
| 69 | struct | object_page_table_struct | 3 fields, 8 bytes |
| 70 | typedef | object_page_table_struct | name for struct #69 (#69) |
| 71 | array |  | 0..3; of SBYTE (#33) |
| 72 | pointer |  | near386; of UBYTE (#30) |
| 73 | array |  | 0..9; of UBYTE * (#72) |
| 74 | array |  | 0..99; of UBYTE * (#72) |
| 75 | procedure |  | near386; returns LONG (void) |
| 76 | pointer |  | near386; of SBYTE (#33) |
| 77 | procedure |  | near386; returns LONG (SBYTE *) |
| 78 | procedure |  | near386; returns void * (SBYTE *, void *) |
| 79 | procedure |  | near386; returns LONG (SBYTE *, void *, ULONG) |

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

##### LX_header_struct (#64, 43 fields, 172 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| lxbw | offset 0 | ULONG |
| format_level | offset 4 | ULONG |
| cpu_os_type | offset 8 | ULONG |
| module_version | offset 12 | ULONG |
| module_flags | offset 16 | ULONG |
| module_num_of_pages | offset 20 | ULONG |
| eip_object_num | offset 24 | ULONG |
| eip | offset 28 | ULONG |
| esp_object_num | offset 32 | ULONG |
| esp | offset 36 | ULONG |
| page_size | offset 40 | ULONG |
| page_offset_shift | offset 44 | ULONG |
| fixup_section_size | offset 48 | ULONG |
| fixup_section_chksum | offset 52 | ULONG |
| loader_section_size | offset 56 | ULONG |
| loader_section_chksum | offset 60 | ULONG |
| object_table_off | offset 64 | ULONG |
| num_objects_in_module | offset 68 | ULONG |
| object_page_table_off | offset 72 | ULONG |
| object_iter_pages_off | offset 76 | ULONG |
| resource_table_off | offset 80 | ULONG |
| num_resource_table_entries | offset 84 | ULONG |
| resident_name_table_off | offset 88 | ULONG |
| entry_table_off | offset 92 | ULONG |
| module_directives_off | offset 96 | ULONG |
| num_module_directives | offset 100 | ULONG |
| fixup_page_table_off | offset 104 | ULONG |
| fixup_record_table_off | offset 108 | ULONG |
| import_module_table_off | offset 112 | ULONG |
| num_import_mod_entries | offset 116 | ULONG |
| import_proc_table_off | offset 120 | ULONG |
| per_page_chksum_off | offset 124 | ULONG |
| data_pages_off | offset 128 | ULONG |
| num_preload_pages | offset 132 | ULONG |
| nonres_name_table_off | offset 136 | ULONG |
| nonres_name_table_len | offset 140 | ULONG |
| nonres_name_table_chksum | offset 144 | ULONG |
| auto_ds_object_num | offset 148 | ULONG |
| debug_info_off | offset 152 | ULONG |
| debug_info_len | offset 156 | ULONG |
| numinstance_preload | offset 160 | ULONG |
| numinstance_demand | offset 164 | ULONG |
| heapsize | offset 168 | ULONG |

##### object_table_struct (#66, 6 fields, 24 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| virtual_size | offset 0 | ULONG |
| reloc_base_addr | offset 4 | ULONG |
| object_flags | offset 8 | ULONG |
| page_table_index | offset 12 | ULONG |
| num_page_table_entries | offset 16 | ULONG |
| reserved_space | offset 20 | ULONG |

##### object_page_table_struct (#69, 3 fields, 8 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| page_data_offset | offset 0 | ULONG |
| data_size | offset 4 | UWORD |
| flags | offset 6 | UWORD |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 184 | 1:0x00143FD7 (linear 0x00153FD7) | 0x001D962B |
| 187 | 1:0x00143FEC (linear 0x00153FEC) | 0x001D9640 |
| 189 | 1:0x00143FF5 (linear 0x00153FF5) | 0x001D9649 |
| 190 | 1:0x00144006 (linear 0x00154006) | 0x001D965A |
| 194 | 1:0x00144011 (linear 0x00154011) | 0x001D9665 |
| 195 | 1:0x0014401E (linear 0x0015401E) | 0x001D9672 |
| 197 | 1:0x0014402C (linear 0x0015402C) | 0x001D9680 |
| 199 | 1:0x00144035 (linear 0x00154035) | 0x001D9689 |
| 210 | 1:0x0014403E (linear 0x0015403E) | 0x001D9692 |
| 214 | 1:0x00144053 (linear 0x00154053) | 0x001D96A7 |
| 216 | 1:0x0014405B (linear 0x0015405B) | 0x001D96AF |
| 218 | 1:0x00144063 (linear 0x00154063) | 0x001D96B7 |
| 219 | 1:0x00144069 (linear 0x00154069) | 0x001D96BD |
| 221 | 1:0x00144072 (linear 0x00154072) | 0x001D96C6 |
| 225 | 1:0x00144087 (linear 0x00154087) | 0x001D96DB |
| 227 | 1:0x0014408F (linear 0x0015408F) | 0x001D96E3 |
| 229 | 1:0x00144097 (linear 0x00154097) | 0x001D96EB |
| 230 | 1:0x0014409D (linear 0x0015409D) | 0x001D96F1 |
| 240 | 1:0x001440A6 (linear 0x001540A6) | 0x001D96FA |
| 244 | 1:0x001440BB (linear 0x001540BB) | 0x001D970F |
| 246 | 1:0x001440C7 (linear 0x001540C7) | 0x001D971B |
| 248 | 1:0x001440CD (linear 0x001540CD) | 0x001D9721 |
| 251 | 1:0x001440DD (linear 0x001540DD) | 0x001D9731 |
| 253 | 1:0x001440E3 (linear 0x001540E3) | 0x001D9737 |
| 255 | 1:0x001440EC (linear 0x001540EC) | 0x001D9740 |
| 257 | 1:0x00144101 (linear 0x00154101) | 0x001D9755 |
| 259 | 1:0x00144107 (linear 0x00154107) | 0x001D975B |
| 261 | 1:0x00144117 (linear 0x00154117) | 0x001D976B |
| 263 | 1:0x00144120 (linear 0x00154120) | 0x001D9774 |
| 273 | 1:0x00144124 (linear 0x00154124) | 0x001D9778 |
| 284 | 1:0x00144139 (linear 0x00154139) | 0x001D978D |
| 285 | 1:0x00144140 (linear 0x00154140) | 0x001D9794 |
| 287 | 1:0x00144146 (linear 0x00154146) | 0x001D979A |
| 289 | 1:0x00144156 (linear 0x00154156) | 0x001D97AA |
| 291 | 1:0x0014415C (linear 0x0015415C) | 0x001D97B0 |
| 293 | 1:0x00144163 (linear 0x00154163) | 0x001D97B7 |
| 295 | 1:0x00144165 (linear 0x00154165) | 0x001D97B9 |
| 296 | 1:0x00144170 (linear 0x00154170) | 0x001D97C4 |
| 297 | 1:0x00144181 (linear 0x00154181) | 0x001D97D5 |
| 299 | 1:0x0014418E (linear 0x0015418E) | 0x001D97E2 |
| 368 | 1:0x00144195 (linear 0x00154195) | 0x001D97E9 |
| 370 | 1:0x0014419B (linear 0x0015419B) | 0x001D97EF |
| 371 | 1:0x001441A6 (linear 0x001541A6) | 0x001D97FA |
| 373 | 1:0x001441B3 (linear 0x001541B3) | 0x001D9807 |
| 376 | 1:0x001441C3 (linear 0x001541C3) | 0x001D9817 |
| 377 | 1:0x001441C9 (linear 0x001541C9) | 0x001D981D |
| 385 | 1:0x001441D2 (linear 0x001541D2) | 0x001D9826 |
| 392 | 1:0x001441E7 (linear 0x001541E7) | 0x001D983B |
| 393 | 1:0x001441EE (linear 0x001541EE) | 0x001D9842 |
| 395 | 1:0x001441F9 (linear 0x001541F9) | 0x001D984D |
| 397 | 1:0x00144209 (linear 0x00154209) | 0x001D985D |
| 398 | 1:0x00144210 (linear 0x00154210) | 0x001D9864 |
| 426 | 1:0x00144217 (linear 0x00154217) | 0x001D986B |
| 438 | 1:0x0014421D (linear 0x0015421D) | 0x001D9871 |
| 440 | 1:0x00144232 (linear 0x00154232) | 0x001D9886 |
| 448 | 1:0x00144239 (linear 0x00154239) | 0x001D988D |
| 449 | 1:0x00144255 (linear 0x00154255) | 0x001D98A9 |
| 451 | 1:0x00144275 (linear 0x00154275) | 0x001D98C9 |
| 452 | 1:0x0014427C (linear 0x0015427C) | 0x001D98D0 |
| 453 | 1:0x00144285 (linear 0x00154285) | 0x001D98D9 |
| 454 | 1:0x00144290 (linear 0x00154290) | 0x001D98E4 |
| 455 | 1:0x00144299 (linear 0x00154299) | 0x001D98ED |
| 457 | 1:0x001442A4 (linear 0x001542A4) | 0x001D98F8 |
| 459 | 1:0x001442B4 (linear 0x001542B4) | 0x001D9908 |
| 497 | 1:0x001442D0 (linear 0x001542D0) | 0x001D9924 |
| 498 | 1:0x001442D6 (linear 0x001542D6) | 0x001D992A |
| 510 | 1:0x001442DF (linear 0x001542DF) | 0x001D9933 |
| 512 | 1:0x001442F4 (linear 0x001542F4) | 0x001D9948 |
| 520 | 1:0x001442FB (linear 0x001542FB) | 0x001D994F |
| 521 | 1:0x00144317 (linear 0x00154317) | 0x001D996B |
| 523 | 1:0x00144337 (linear 0x00154337) | 0x001D998B |
| 524 | 1:0x0014433E (linear 0x0015433E) | 0x001D9992 |
| 525 | 1:0x00144347 (linear 0x00154347) | 0x001D999B |
| 526 | 1:0x00144352 (linear 0x00154352) | 0x001D99A6 |
| 527 | 1:0x0014435B (linear 0x0015435B) | 0x001D99AF |
| 529 | 1:0x00144366 (linear 0x00154366) | 0x001D99BA |
| 531 | 1:0x00144376 (linear 0x00154376) | 0x001D99CA |
| 569 | 1:0x00144392 (linear 0x00154392) | 0x001D99E6 |
| 570 | 1:0x00144398 (linear 0x00154398) | 0x001D99EC |
| 579 | 1:0x001443A1 (linear 0x001543A1) | 0x001D99F5 |
| 581 | 1:0x001443B6 (linear 0x001543B6) | 0x001D9A0A |
| 582 | 1:0x001443CC (linear 0x001543CC) | 0x001D9A20 |
| 591 | 1:0x001443D5 (linear 0x001543D5) | 0x001D9A29 |
| 593 | 1:0x001443EA (linear 0x001543EA) | 0x001D9A3E |
| 594 | 1:0x00144400 (linear 0x00154400) | 0x001D9A54 |
| 602 | 1:0x00144409 (linear 0x00154409) | 0x001D9A5D |
| 604 | 1:0x0014441E (linear 0x0015441E) | 0x001D9A72 |
| 611 | 1:0x00144426 (linear 0x00154426) | 0x001D9A7A |
| 613 | 1:0x0014442D (linear 0x0015442D) | 0x001D9A81 |
| 615 | 1:0x00144436 (linear 0x00154436) | 0x001D9A8A |
| 617 | 1:0x0014443C (linear 0x0015443C) | 0x001D9A90 |
| 619 | 1:0x0014443E (linear 0x0015443E) | 0x001D9A92 |
| 621 | 1:0x00144452 (linear 0x00154452) | 0x001D9AA6 |
| 622 | 1:0x00144458 (linear 0x00154458) | 0x001D9AAC |
| 629 | 1:0x00144464 (linear 0x00154464) | 0x001D9AB8 |
| 635 | 1:0x0014447F (linear 0x0015447F) | 0x001D9AD3 |
| 637 | 1:0x0014449C (linear 0x0015449C) | 0x001D9AF0 |
| 643 | 1:0x001444AD (linear 0x001544AD) | 0x001D9B01 |
| 644 | 1:0x001444B5 (linear 0x001544B5) | 0x001D9B09 |
| 651 | 1:0x001444C1 (linear 0x001544C1) | 0x001D9B15 |
| 657 | 1:0x001444E4 (linear 0x001544E4) | 0x001D9B38 |
| 659 | 1:0x001444F0 (linear 0x001544F0) | 0x001D9B44 |
| 661 | 1:0x0014450C (linear 0x0015450C) | 0x001D9B60 |
| 662 | 1:0x00144529 (linear 0x00154529) | 0x001D9B7D |
| 663 | 1:0x0014452F (linear 0x0015452F) | 0x001D9B83 |
| 665 | 1:0x00144531 (linear 0x00154531) | 0x001D9B85 |
| 667 | 1:0x0014453A (linear 0x0015453A) | 0x001D9B8E |
| 674 | 1:0x00144542 (linear 0x00154542) | 0x001D9B96 |
| 676 | 1:0x0014454C (linear 0x0015454C) | 0x001D9BA0 |
| 677 | 1:0x00144552 (linear 0x00154552) | 0x001D9BA6 |
| 685 | 1:0x0014455B (linear 0x0015455B) | 0x001D9BAF |
| 692 | 1:0x00144570 (linear 0x00154570) | 0x001D9BC4 |
| 699 | 1:0x00144578 (linear 0x00154578) | 0x001D9BCC |
| 704 | 1:0x0014457F (linear 0x0015457F) | 0x001D9BD3 |
| 706 | 1:0x00144588 (linear 0x00154588) | 0x001D9BDC |
| 708 | 1:0x0014458E (linear 0x0015458E) | 0x001D9BE2 |
| 710 | 1:0x00144590 (linear 0x00154590) | 0x001D9BE4 |
| 712 | 1:0x001445A4 (linear 0x001545A4) | 0x001D9BF8 |
| 713 | 1:0x001445AA (linear 0x001545AA) | 0x001D9BFE |
| 720 | 1:0x001445B6 (linear 0x001545B6) | 0x001D9C0A |
| 722 | 1:0x001445C9 (linear 0x001545C9) | 0x001D9C1D |
| 724 | 1:0x001445D2 (linear 0x001545D2) | 0x001D9C26 |
| 726 | 1:0x001445DE (linear 0x001545DE) | 0x001D9C32 |
| 728 | 1:0x001445E4 (linear 0x001545E4) | 0x001D9C38 |
| 729 | 1:0x001445ED (linear 0x001545ED) | 0x001D9C41 |
| 731 | 1:0x001445F5 (linear 0x001545F5) | 0x001D9C49 |
| 735 | 1:0x00144601 (linear 0x00154601) | 0x001D9C55 |
| 737 | 1:0x0014460E (linear 0x0015460E) | 0x001D9C62 |
| 743 | 1:0x00144614 (linear 0x00154614) | 0x001D9C68 |
| 749 | 1:0x0014462F (linear 0x0015462F) | 0x001D9C83 |
| 751 | 1:0x0014464C (linear 0x0015464C) | 0x001D9CA0 |
| 757 | 1:0x0014465D (linear 0x0015465D) | 0x001D9CB1 |
| 758 | 1:0x00144666 (linear 0x00154666) | 0x001D9CBA |
| 760 | 1:0x0014466E (linear 0x0015466E) | 0x001D9CC2 |
| 767 | 1:0x0014467A (linear 0x0015467A) | 0x001D9CCE |
| 773 | 1:0x0014469D (linear 0x0015469D) | 0x001D9CF1 |
| 775 | 1:0x001446A9 (linear 0x001546A9) | 0x001D9CFD |
| 777 | 1:0x001446C8 (linear 0x001546C8) | 0x001D9D1C |
| 784 | 1:0x001446E5 (linear 0x001546E5) | 0x001D9D39 |
| 786 | 1:0x001446EB (linear 0x001546EB) | 0x001D9D3F |
| 788 | 1:0x001446F7 (linear 0x001546F7) | 0x001D9D4B |
| 790 | 1:0x00144713 (linear 0x00154713) | 0x001D9D67 |
| 797 | 1:0x00144730 (linear 0x00154730) | 0x001D9D84 |
| 806 | 1:0x00144736 (linear 0x00154736) | 0x001D9D8A |
| 809 | 1:0x0014474A (linear 0x0015474A) | 0x001D9D9E |
| 810 | 1:0x0014475B (linear 0x0015475B) | 0x001D9DAF |
| 812 | 1:0x0014476D (linear 0x0015476D) | 0x001D9DC1 |
| 815 | 1:0x0014477D (linear 0x0015477D) | 0x001D9DD1 |
| 816 | 1:0x00144790 (linear 0x00154790) | 0x001D9DE4 |
| 818 | 1:0x00144796 (linear 0x00154796) | 0x001D9DEA |
| 821 | 1:0x001447EB (linear 0x001547EB) | 0x001D9E3F |
| 825 | 1:0x00144801 (linear 0x00154801) | 0x001D9E55 |
| 827 | 1:0x00144820 (linear 0x00154820) | 0x001D9E74 |
| 829 | 1:0x00144826 (linear 0x00154826) | 0x001D9E7A |
| 830 | 1:0x0014482C (linear 0x0015482C) | 0x001D9E80 |
| 832 | 1:0x00144831 (linear 0x00154831) | 0x001D9E85 |
| 833 | 1:0x00144837 (linear 0x00154837) | 0x001D9E8B |
| 839 | 1:0x0014483C (linear 0x0015483C) | 0x001D9E90 |
| 841 | 1:0x00144848 (linear 0x00154848) | 0x001D9E9C |
| 844 | 1:0x00144865 (linear 0x00154865) | 0x001D9EB9 |
| 846 | 1:0x00144884 (linear 0x00154884) | 0x001D9ED8 |
| 849 | 1:0x001448A1 (linear 0x001548A1) | 0x001D9EF5 |
| 856 | 1:0x001448AD (linear 0x001548AD) | 0x001D9F01 |
| 858 | 1:0x001448B3 (linear 0x001548B3) | 0x001D9F07 |
| 862 | 1:0x001448C4 (linear 0x001548C4) | 0x001D9F18 |
| 864 | 1:0x001448E1 (linear 0x001548E1) | 0x001D9F35 |
| 867 | 1:0x001448FE (linear 0x001548FE) | 0x001D9F52 |
| 873 | 1:0x0014490C (linear 0x0015490C) | 0x001D9F60 |
| 874 | 1:0x00144915 (linear 0x00154915) | 0x001D9F69 |
| 876 | 1:0x0014491D (linear 0x0015491D) | 0x001D9F71 |
| 879 | 1:0x00144929 (linear 0x00154929) | 0x001D9F7D |
| 885 | 1:0x00144947 (linear 0x00154947) | 0x001D9F9B |
| 886 | 1:0x00144950 (linear 0x00154950) | 0x001D9FA4 |
| 888 | 1:0x00144958 (linear 0x00154958) | 0x001D9FAC |
| 891 | 1:0x00144964 (linear 0x00154964) | 0x001D9FB8 |
| 897 | 1:0x00144972 (linear 0x00154972) | 0x001D9FC6 |
| 898 | 1:0x0014497B (linear 0x0015497B) | 0x001D9FCF |
| 900 | 1:0x00144983 (linear 0x00154983) | 0x001D9FD7 |
| 904 | 1:0x0014498F (linear 0x0015498F) | 0x001D9FE3 |
| 906 | 1:0x001449AC (linear 0x001549AC) | 0x001DA000 |
| 908 | 1:0x001449C9 (linear 0x001549C9) | 0x001DA01D |
| 915 | 1:0x001449E6 (linear 0x001549E6) | 0x001DA03A |
| 917 | 1:0x001449F4 (linear 0x001549F4) | 0x001DA048 |
| 918 | 1:0x00144A0C (linear 0x00154A0C) | 0x001DA060 |
| 920 | 1:0x00144A26 (linear 0x00154A26) | 0x001DA07A |
| 925 | 1:0x00144A2E (linear 0x00154A2E) | 0x001DA082 |
| 928 | 1:0x00144A43 (linear 0x00154A43) | 0x001DA097 |
| 931 | 1:0x00144A49 (linear 0x00154A49) | 0x001DA09D |
| 933 | 1:0x00144A4F (linear 0x00154A4F) | 0x001DA0A3 |
| 934 | 1:0x00144A54 (linear 0x00154A54) | 0x001DA0A8 |
| 935 | 1:0x00144A5D (linear 0x00154A5D) | 0x001DA0B1 |
| 937 | 1:0x00144A65 (linear 0x00154A65) | 0x001DA0B9 |
| 938 | 1:0x00144A6B (linear 0x00154A6B) | 0x001DA0BF |
| 946 | 1:0x00144A74 (linear 0x00154A74) | 0x001DA0C8 |
| 948 | 1:0x00144A89 (linear 0x00154A89) | 0x001DA0DD |
| 949 | 1:0x00144A91 (linear 0x00154A91) | 0x001DA0E5 |
| 957 | 1:0x00144A9A (linear 0x00154A9A) | 0x001DA0EE |
| 962 | 1:0x00144AAF (linear 0x00154AAF) | 0x001DA103 |
| 964 | 1:0x00144AB9 (linear 0x00154AB9) | 0x001DA10D |
| 966 | 1:0x00144ACD (linear 0x00154ACD) | 0x001DA121 |
| 968 | 1:0x00144AD3 (linear 0x00154AD3) | 0x001DA127 |
| 969 | 1:0x00144ADD (linear 0x00154ADD) | 0x001DA131 |
| 972 | 1:0x00144AE6 (linear 0x00154AE6) | 0x001DA13A |
| 973 | 1:0x00144AF1 (linear 0x00154AF1) | 0x001DA145 |
| 975 | 1:0x00144B01 (linear 0x00154B01) | 0x001DA155 |
| 976 | 1:0x00144B09 (linear 0x00154B09) | 0x001DA15D |
| 977 | 1:0x00144B0F (linear 0x00154B0F) | 0x001DA163 |
| 985 | 1:0x00144B18 (linear 0x00154B18) | 0x001DA16C |
| 991 | 1:0x00144B2D (linear 0x00154B2D) | 0x001DA181 |
| 993 | 1:0x00144B37 (linear 0x00154B37) | 0x001DA18B |
| 995 | 1:0x00144B46 (linear 0x00154B46) | 0x001DA19A |
| 997 | 1:0x00144B4C (linear 0x00154B4C) | 0x001DA1A0 |
| 998 | 1:0x00144B56 (linear 0x00154B56) | 0x001DA1AA |
| 1001 | 1:0x00144B62 (linear 0x00154B62) | 0x001DA1B6 |
| 1003 | 1:0x00144B88 (linear 0x00154B88) | 0x001DA1DC |
| 1005 | 1:0x00144B8E (linear 0x00154B8E) | 0x001DA1E2 |
| 1006 | 1:0x00144B98 (linear 0x00154B98) | 0x001DA1EC |
| 1009 | 1:0x00144BA4 (linear 0x00154BA4) | 0x001DA1F8 |
| 1011 | 1:0x00144BB8 (linear 0x00154BB8) | 0x001DA20C |
| 1013 | 1:0x00144BBE (linear 0x00154BBE) | 0x001DA212 |
| 1014 | 1:0x00144BC7 (linear 0x00154BC7) | 0x001DA21B |
| 1015 | 1:0x00144BD1 (linear 0x00154BD1) | 0x001DA225 |
| 1018 | 1:0x00144BDA (linear 0x00154BDA) | 0x001DA22E |
| 1020 | 1:0x00144BEB (linear 0x00154BEB) | 0x001DA23F |
| 1022 | 1:0x00144BF3 (linear 0x00154BF3) | 0x001DA247 |
| 1023 | 1:0x00144BFC (linear 0x00154BFC) | 0x001DA250 |
| 1024 | 1:0x00144C06 (linear 0x00154C06) | 0x001DA25A |
| 1027 | 1:0x00144C0F (linear 0x00154C0F) | 0x001DA263 |
| 1028 | 1:0x00144C17 (linear 0x00154C17) | 0x001DA26B |
| 1029 | 1:0x00144C1D (linear 0x00154C1D) | 0x001DA271 |
| 1041 | 1:0x00144C26 (linear 0x00154C26) | 0x001DA27A |
| 1045 | 1:0x00144C3B (linear 0x00154C3B) | 0x001DA28F |
| 1047 | 1:0x00144C45 (linear 0x00154C45) | 0x001DA299 |
| 1049 | 1:0x00144C5E (linear 0x00154C5E) | 0x001DA2B2 |
| 1051 | 1:0x00144C64 (linear 0x00154C64) | 0x001DA2B8 |
| 1052 | 1:0x00144C6E (linear 0x00154C6E) | 0x001DA2C2 |
| 1055 | 1:0x00144C77 (linear 0x00154C77) | 0x001DA2CB |
| 1057 | 1:0x00144C88 (linear 0x00154C88) | 0x001DA2DC |
| 1059 | 1:0x00144C8E (linear 0x00154C8E) | 0x001DA2E2 |
| 1060 | 1:0x00144C98 (linear 0x00154C98) | 0x001DA2EC |
| 1063 | 1:0x00144CA1 (linear 0x00154CA1) | 0x001DA2F5 |
| 1065 | 1:0x00144CA9 (linear 0x00154CA9) | 0x001DA2FD |
| 1066 | 1:0x00144CB3 (linear 0x00154CB3) | 0x001DA307 |
| 1069 | 1:0x00144CBC (linear 0x00154CBC) | 0x001DA310 |
| 1071 | 1:0x00144CC4 (linear 0x00154CC4) | 0x001DA318 |
| 1072 | 1:0x00144CCB (linear 0x00154CCB) | 0x001DA31F |
| 1082 | 1:0x00144CD4 (linear 0x00154CD4) | 0x001DA328 |
| 1086 | 1:0x00144CE9 (linear 0x00154CE9) | 0x001DA33D |
| 1088 | 1:0x00144CF3 (linear 0x00154CF3) | 0x001DA347 |
| 1089 | 1:0x00144D07 (linear 0x00154D07) | 0x001DA35B |
| 1091 | 1:0x00144D0D (linear 0x00154D0D) | 0x001DA361 |
| 1092 | 1:0x00144D17 (linear 0x00154D17) | 0x001DA36B |
| 1095 | 1:0x00144D20 (linear 0x00154D20) | 0x001DA374 |
| 1097 | 1:0x00144D31 (linear 0x00154D31) | 0x001DA385 |
| 1099 | 1:0x00144D37 (linear 0x00154D37) | 0x001DA38B |
| 1100 | 1:0x00144D41 (linear 0x00154D41) | 0x001DA395 |
| 1103 | 1:0x00144D4A (linear 0x00154D4A) | 0x001DA39E |
| 1105 | 1:0x00144D52 (linear 0x00154D52) | 0x001DA3A6 |
| 1106 | 1:0x00144D5C (linear 0x00154D5C) | 0x001DA3B0 |
| 1109 | 1:0x00144D65 (linear 0x00154D65) | 0x001DA3B9 |
| 1111 | 1:0x00144D6D (linear 0x00154D6D) | 0x001DA3C1 |
| 1112 | 1:0x00144D74 (linear 0x00154D74) | 0x001DA3C8 |

