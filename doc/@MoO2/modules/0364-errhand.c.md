# F:\WLIBS\compress\errhand.c

Module 364 of the debug info of ORION2.EXE.

Ranges: 1:0x00153233 (linear 0x00163233), 86 bytes, 2:0x00004C6C (linear 0x00174C6C), 16 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x00153233 (linear 0x00163233) | code | fatal_error_ | `regions/07-object-1/001E8654-001E8E53.txt` |

#### fatal_error

- Address: 1:0x00153233 (linear 0x00163233), 86 bytes, in `regions/07-object-1/001E8654-001E8E53.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| fmt | [EBP+0x1C] | char * |
| error_msg | [EBP-0x2C] | char[40] |
| argptr | [EBP-0x4] | va_list |

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
| 25 | array | va_list | 0..0; of char * (#13) |
| 26 | typedef | va_list | name for char *[1] (#25) |
| 27 | procedure |  | near386; returns void (char *) |
| 28 | array |  | 0..39; of char (#10) |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 12 | 1:0x00153233 (linear 0x00163233) | 0x001E8887 |
| 18 | 1:0x0015324B (linear 0x0016324B) | 0x001E889F |
| 19 | 1:0x00153251 (linear 0x00163251) | 0x001E88A5 |
| 20 | 1:0x00153263 (linear 0x00163263) | 0x001E88B7 |
| 21 | 1:0x00153271 (linear 0x00163271) | 0x001E88C5 |
| 22 | 1:0x00153278 (linear 0x00163278) | 0x001E88CC |
| 23 | 1:0x00153280 (linear 0x00163280) | 0x001E88D4 |

