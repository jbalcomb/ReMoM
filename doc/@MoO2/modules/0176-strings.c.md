# F:\WLIBS\memory\strings.c

Module 176 of the debug info of ORION2.EXE.

Ranges: 1:0x00122F5C (linear 0x00132F5C), 731 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x00122F5C (linear 0x00132F5C) | code | String_To_Upper_ | `regions/07-object-1/001B7E54-001B8653.txt` |
| 1:0x00122FCD (linear 0x00132FCD) | code | String_To_Lower_ | `regions/07-object-1/001B7E54-001B8653.txt` |
| 1:0x0012303E (linear 0x0013303E) | code | Trim_ | `regions/07-object-1/001B8654-001B8E53.txt` |
| 1:0x001230D2 (linear 0x001330D2) | code | Swap_ | `regions/07-object-1/001B8654-001B8E53.txt` |
| 1:0x00123111 (linear 0x00133111) | code | Swap_Short_ | `regions/07-object-1/001B8654-001B8E53.txt` |
| 1:0x00123154 (linear 0x00133154) | code | Swap_Char_ | `regions/07-object-1/001B8654-001B8E53.txt` |
| 1:0x00123193 (linear 0x00133193) | code | Swap_Long_ | `regions/07-object-1/001B8654-001B8E53.txt` |
| 1:0x001231D2 (linear 0x001331D2) | code | Delete_Structure_ | `regions/07-object-1/001B8654-001B8E53.txt` |

#### String_To_Upper

- Address: 1:0x00122F5C (linear 0x00132F5C), 113 bytes, in `regions/07-object-1/001B7E54-001B8653.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| name | [EBP-0x8] | char * |
| i | [EBP-0x4] | int |

#### String_To_Lower

- Address: 1:0x00122FCD (linear 0x00132FCD), 113 bytes, in `regions/07-object-1/001B7E54-001B8653.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| name | [EBP-0x8] | char * |
| i | [EBP-0x4] | int |

#### Trim

- Address: 1:0x0012303E (linear 0x0013303E), 148 bytes, in `regions/07-object-1/001B8654-001B8E53.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| name | [EBP-0x10] | char * |
| i | [EBP-0xC] | int |
| start_pos | [EBP-0x8] | int |
| end_pos | [EBP-0x4] | int |

#### Swap

- Address: 1:0x001230D2 (linear 0x001330D2), 63 bytes, in `regions/07-object-1/001B8654-001B8E53.txt`
- Type: void (int *, int *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| value1 | [EBP-0xC] | int * |
| value2 | [EBP-0x8] | int * |
| temp | [EBP-0x4] | int |

#### Swap_Short

- Address: 1:0x00123111 (linear 0x00133111), 67 bytes, in `regions/07-object-1/001B8654-001B8E53.txt`
- Type: void (short *, short *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| value1 | [EBP-0xC] | short * |
| value2 | [EBP-0x8] | short * |
| temp | [EBP-0x4] | short |

#### Swap_Char

- Address: 1:0x00123154 (linear 0x00133154), 63 bytes, in `regions/07-object-1/001B8654-001B8E53.txt`
- Type: void (char *, char *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| value1 | [EBP-0xC] | char * |
| value2 | [EBP-0x8] | char * |
| temp | [EBP-0x4] | char |

#### Swap_Long

- Address: 1:0x00123193 (linear 0x00133193), 63 bytes, in `regions/07-object-1/001B8654-001B8E53.txt`
- Type: void (long *, long *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| value1 | [EBP-0xC] | long * |
| value2 | [EBP-0x8] | long * |
| temp | [EBP-0x4] | long |

#### Delete_Structure

- Address: 1:0x001231D2 (linear 0x001331D2), 101 bytes, in `regions/07-object-1/001B8654-001B8E53.txt`
- Type: void (short, char *, int, short)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| record_num | [EBP-0x8] | short |
| address | [EBP-0xC] | char * |
| struct_size | [EBP-0x10] | int |
| total_records | [EBP-0x4] | short |

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
| 30 | scalar | void | void 1 bytes |
| 31 | procedure |  | near386; returns void (char *) |
| 32 | pointer |  | near386; of int (#8) |
| 33 | procedure |  | near386; returns void (int *, int *) |
| 34 | pointer |  | near386; of short (#6) |
| 35 | procedure |  | near386; returns void (short *, short *) |
| 36 | procedure |  | near386; returns void (char *, char *) |
| 37 | pointer |  | near386; of long (#16) |
| 38 | procedure |  | near386; returns void (long *, long *) |
| 39 | procedure |  | near386; returns void (short, char *, int, short) |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 11 | 1:0x00122F5C (linear 0x00132F5C) | 0x001B85B0 |
| 16 | 1:0x00122F77 (linear 0x00132F77) | 0x001B85CB |
| 17 | 1:0x00122F93 (linear 0x00132F93) | 0x001B85E7 |
| 18 | 1:0x00122FC4 (linear 0x00132FC4) | 0x001B8618 |
| 27 | 1:0x00122FCD (linear 0x00132FCD) | 0x001B8621 |
| 32 | 1:0x00122FE8 (linear 0x00132FE8) | 0x001B863C |
| 33 | 1:0x00123004 (linear 0x00133004) | 0x001B8658 |
| 34 | 1:0x00123035 (linear 0x00133035) | 0x001B8689 |
| 43 | 1:0x0012303E (linear 0x0013303E) | 0x001B8692 |
| 48 | 1:0x00123059 (linear 0x00133059) | 0x001B86AD |
| 49 | 1:0x00123060 (linear 0x00133060) | 0x001B86B4 |
| 50 | 1:0x00123077 (linear 0x00133077) | 0x001B86CB |
| 52 | 1:0x00123085 (linear 0x00133085) | 0x001B86D9 |
| 53 | 1:0x0012308C (linear 0x0013308C) | 0x001B86E0 |
| 56 | 1:0x0012309C (linear 0x0013309C) | 0x001B86F0 |
| 57 | 1:0x001230A0 (linear 0x001330A0) | 0x001B86F4 |
| 59 | 1:0x001230BA (linear 0x001330BA) | 0x001B870E |
| 60 | 1:0x001230C3 (linear 0x001330C3) | 0x001B8717 |
| 61 | 1:0x001230C7 (linear 0x001330C7) | 0x001B871B |
| 63 | 1:0x001230C9 (linear 0x001330C9) | 0x001B871D |
| 69 | 1:0x001230D2 (linear 0x001330D2) | 0x001B8726 |
| 73 | 1:0x001230EF (linear 0x001330EF) | 0x001B8743 |
| 74 | 1:0x001230F7 (linear 0x001330F7) | 0x001B874B |
| 75 | 1:0x00123101 (linear 0x00133101) | 0x001B8755 |
| 76 | 1:0x00123109 (linear 0x00133109) | 0x001B875D |
| 81 | 1:0x00123111 (linear 0x00133111) | 0x001B8765 |
| 85 | 1:0x0012312E (linear 0x0013312E) | 0x001B8782 |
| 86 | 1:0x00123137 (linear 0x00133137) | 0x001B878B |
| 87 | 1:0x00123143 (linear 0x00133143) | 0x001B8797 |
| 88 | 1:0x0012314C (linear 0x0013314C) | 0x001B87A0 |
| 93 | 1:0x00123154 (linear 0x00133154) | 0x001B87A8 |
| 97 | 1:0x00123171 (linear 0x00133171) | 0x001B87C5 |
| 98 | 1:0x00123179 (linear 0x00133179) | 0x001B87CD |
| 99 | 1:0x00123183 (linear 0x00133183) | 0x001B87D7 |
| 100 | 1:0x0012318B (linear 0x0013318B) | 0x001B87DF |
| 106 | 1:0x00123193 (linear 0x00133193) | 0x001B87E7 |
| 110 | 1:0x001231B0 (linear 0x001331B0) | 0x001B8804 |
| 111 | 1:0x001231B8 (linear 0x001331B8) | 0x001B880C |
| 112 | 1:0x001231C2 (linear 0x001331C2) | 0x001B8816 |
| 113 | 1:0x001231CA (linear 0x001331CA) | 0x001B881E |
| 119 | 1:0x001231D2 (linear 0x001331D2) | 0x001B8826 |
| 122 | 1:0x001231F3 (linear 0x001331F3) | 0x001B8847 |
| 124 | 1:0x00123200 (linear 0x00133200) | 0x001B8854 |
| 128 | 1:0x00123231 (linear 0x00133231) | 0x001B8885 |

