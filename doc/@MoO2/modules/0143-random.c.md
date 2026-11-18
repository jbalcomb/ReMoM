# F:\WLIBS\special\random.c

Module 143 of the debug info of ORION2.EXE.

Ranges: 1:0x001147A0 (linear 0x001247A0), 267 bytes, 2:0x00041E34 (linear 0x001B1E34), 4 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x001147A0 (linear 0x001247A0) | code | Random_ | `regions/07-object-1/001A9654-001A9E53.txt` |
| 1:0x00114820 (linear 0x00124820) | code | Set_Random_Seed_ | `regions/07-object-1/001A9E54-001AA653.txt` |
| 1:0x0011484C (linear 0x0012484C) | code | Get_Random_Seed_ | `regions/07-object-1/001A9E54-001AA653.txt` |
| 1:0x00114878 (linear 0x00124878) | code | Randomize_ | `regions/07-object-1/001A9E54-001AA653.txt` |
| 2:0x00041E34 (linear 0x001B1E34) | static, data | _random_seed | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x00041E34 (linear 0x001B1E34) | random_seed | unsigned long |

#### Random

- Address: 1:0x001147A0 (linear 0x001247A0), 128 bytes, in `regions/07-object-1/001A9654-001A9E53.txt`
- Type: SDWORD (SDWORD); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| n | [EBP-0x14] | SDWORD |
| result | [EBP-0x10] | SDWORD |
| quotient | [EBP-0xC] | unsigned long |
| limit | [EBP-0x8] | unsigned long |

#### Set_Random_Seed

- Address: 1:0x00114820 (linear 0x00124820), 44 bytes, in `regions/07-object-1/001A9E54-001AA653.txt`
- Type: void (UDWORD)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| n | [EBP-0x4] | UDWORD |

#### Get_Random_Seed

- Address: 1:0x0011484C (linear 0x0012484C), 44 bytes, in `regions/07-object-1/001A9E54-001AA653.txt`
- Type: UDWORD (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Randomize

- Address: 1:0x00114878 (linear 0x00124878), 51 bytes, in `regions/07-object-1/001A9E54-001AA653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| timer_value | [EBP-0x4] | time_t |

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
| 13 | scalar | unsigned long | unsigned 4 bytes |
| 14 | typedef | time_t | name for unsigned long (#13) |
| 15 | typedef | clock_t | name for unsigned long (#13) |
| 16 | typedef | time_t | name for unsigned long (#13) |
| 17 | typedef | BYTE | name for unsigned char (#5) |
| 18 | typedef | SBYTE | name for signed char (#4) |
| 19 | typedef | UBYTE | name for unsigned char (#5) |
| 20 | typedef | WORD | name for unsigned short (#7) |
| 21 | typedef | SWORD | name for short (#6) |
| 22 | typedef | UWORD | name for unsigned short (#7) |
| 23 | typedef | DWORD | name for unsigned int (#9) |
| 24 | typedef | SDWORD | name for int (#8) |
| 25 | typedef | UDWORD | name for unsigned int (#9) |
| 26 | typedef | BOOL | name for unsigned char (#5) |
| 27 | typedef | SampleHandle | name for int (#8) |
| 28 | procedure |  | near386; returns SDWORD (SDWORD) |
| 29 | scalar | unsigned long | unsigned 4 bytes |
| 30 | scalar | void | void 1 bytes |
| 31 | procedure |  | near386; returns void (UDWORD) |
| 32 | procedure |  | near386; returns UDWORD (void) |
| 33 | procedure |  | near386; returns void (void) |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 54 | 1:0x001147A0 (linear 0x001247A0) | 0x001A9DF4 |
| 57 | 1:0x001147BB (linear 0x001247BB) | 0x001A9E0F |
| 58 | 1:0x001147C8 (linear 0x001247C8) | 0x001A9E1C |
| 59 | 1:0x001147D2 (linear 0x001247D2) | 0x001A9E26 |
| 60 | 1:0x001147D8 (linear 0x001247D8) | 0x001A9E2C |
| 63 | 1:0x001147E1 (linear 0x001247E1) | 0x001A9E35 |
| 64 | 1:0x001147F5 (linear 0x001247F5) | 0x001A9E49 |
| 65 | 1:0x00114800 (linear 0x00124800) | 0x001A9E54 |
| 69 | 1:0x0011480D (linear 0x0012480D) | 0x001A9E61 |
| 70 | 1:0x00114814 (linear 0x00124814) | 0x001A9E68 |
| 72 | 1:0x00114820 (linear 0x00124820) | 0x001A9E74 |
| 74 | 1:0x0011483B (linear 0x0012483B) | 0x001A9E8F |
| 75 | 1:0x00114843 (linear 0x00124843) | 0x001A9E97 |
| 77 | 1:0x0011484C (linear 0x0012484C) | 0x001A9EA0 |
| 79 | 1:0x00114864 (linear 0x00124864) | 0x001A9EB8 |
| 80 | 1:0x0011486C (linear 0x0012486C) | 0x001A9EC0 |
| 82 | 1:0x00114878 (linear 0x00124878) | 0x001A9ECC |
| 85 | 1:0x00114890 (linear 0x00124890) | 0x001A9EE4 |
| 86 | 1:0x0011489A (linear 0x0012489A) | 0x001A9EEE |
| 87 | 1:0x001148A2 (linear 0x001248A2) | 0x001A9EF6 |

