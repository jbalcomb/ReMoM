# F:\WLIBS\special\special.c

Module 179 of the debug info of ORION2.EXE.

Ranges: 1:0x0012432D (linear 0x0013432D), 2,560 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0012432D (linear 0x0013432D) | code | Interpolate_Line_ | `regions/07-object-1/001B9654-001B9E53.txt` |
| 1:0x001244B0 (linear 0x001344B0) | code | Absolute_Interpolate_Line_ | `regions/07-object-1/001B9654-001B9E53.txt` |
| 1:0x00124637 (linear 0x00134637) | code | Range_ | `regions/07-object-1/001B9654-001B9E53.txt` |
| 1:0x001246C4 (linear 0x001346C4) | code | Delta_XY_With_Wrap_ | `regions/07-object-1/001B9654-001B9E53.txt` |
| 1:0x00124769 (linear 0x00134769) | code | Path_ | `regions/07-object-1/001B9654-001B9E53.txt` |
| 1:0x0012498E (linear 0x0013498E) | code | Path_Wrap_ | `regions/07-object-1/001B9E54-001BA653.txt` |
| 1:0x00124C92 (linear 0x00134C92) | code | isqrt_ | `regions/07-object-1/001B9E54-001BA653.txt` |

#### Interpolate_Line

- Address: 1:0x0012432D (linear 0x0013432D), 387 bytes, in `regions/07-object-1/001B9654-001B9E53.txt`
- Type: void (short *, short *, short, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x | [EBP-0x1C] | short * |
| y | [EBP-0x18] | short * |
| end_x | [EBP-0x10] | short |
| end_y | [EBP-0x14] | short |
| step_size | [EBP+0x10] | short |
| delta_x | [EBP-0xC] | short |
| delta_y | [EBP-0x8] | short |
| angle | [EBP-0x4] | short |

#### Absolute_Interpolate_Line

- Address: 1:0x001244B0 (linear 0x001344B0), 391 bytes, in `regions/07-object-1/001B9654-001B9E53.txt`
- Type: void (short *, short *, short, short, short, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x | [EBP-0x1C] | short * |
| y | [EBP-0x18] | short * |
| source_x | [EBP-0x14] | short |
| source_y | [EBP-0x10] | short |
| end_x | [EBP+0x10] | short |
| end_y | [EBP+0x14] | short |
| step_size | [EBP+0x18] | short |
| delta_x | [EBP-0xC] | short |
| delta_y | [EBP-0x8] | short |
| angle | [EBP-0x4] | short |

#### Range

- Address: 1:0x00124637 (linear 0x00134637), 141 bytes, in `regions/07-object-1/001B9654-001B9E53.txt`
- Type: short (short, short, short, short); returns in AX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x20] | short |
| y1 | [EBP-0x1C] | short |
| x2 | [EBP-0x18] | short |
| y2 | [EBP-0x14] | short |
| delta_x | [EBP-0x10] | short |
| delta_y | [EBP-0xC] | short |
| range | [EBP-0x8] | short |

#### Delta_XY_With_Wrap

- Address: 1:0x001246C4 (linear 0x001346C4), 165 bytes, in `regions/07-object-1/001B9654-001B9E53.txt`
- Type: short (short, short, short, short, short); returns in AX
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x18] | short |
| y1 | [EBP-0x24] | short |
| x2 | [EBP-0x20] | short |
| y2 | [EBP-0x1C] | short |
| wrap_x | [EBP+0x10] | short |
| delta_x1 | [EBP-0x14] | short |
| delta_x2 | [EBP-0x10] | short |
| delta_y | [EBP-0xC] | short |
| range | [EBP-0x8] | short |

#### Path

- Address: 1:0x00124769 (linear 0x00134769), 549 bytes, in `regions/07-object-1/001B9654-001B9E53.txt`
- Type: short (short, short, short, short, char *, char *); returns in AX
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x14] | short |
| y1 | [EBP-0x10] | short |
| x2 | [EBP-0xC] | short |
| y2 | [EBP-0x8] | short |
| path_string_x | [EBP+0x10] | char * |
| path_string_y | [EBP+0x14] | char * |
| length | [EBP-0x2C] | int |
| delta_x | [EBP-0x3C] | int |
| delta_y | [EBP-0x38] | int |
| sptr | [EBP-0x34] | int |
| sign_x | [EBP-0x30] | int |
| sign_y | [EBP-0x18] | int |
| cur_x | [EBP-0x28] | int |
| cur_y | [EBP-0x24] | int |
| remainder | [EBP-0x20] | unsigned long |
| delta_add | [EBP-0x1C] | unsigned long |

#### Path_Wrap

- Address: 1:0x0012498E (linear 0x0013498E), 772 bytes, in `regions/07-object-1/001B9E54-001BA653.txt`
- Type: short (short, short, short, short, signed char *, signed char *, short); returns in AX
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x14] | short |
| y1 | [EBP-0x8] | short |
| x2 | [EBP-0x10] | short |
| y2 | [EBP-0xC] | short |
| path_string_x | [EBP+0x10] | signed char * |
| path_string_y | [EBP+0x14] | signed char * |
| MAP_X | [EBP+0x18] | short |
| length | [EBP-0x30] | int |
| delta_x | [EBP-0x44] | int |
| delta_y | [EBP-0x40] | int |
| sptr | [EBP-0x3C] | int |
| sign_x | [EBP-0x38] | int |
| sign_y | [EBP-0x1C] | int |
| cur_x | [EBP-0x24] | int |
| cur_y | [EBP-0x2C] | int |
| remainder | [EBP-0x28] | unsigned long |
| delta_add | [EBP-0x34] | unsigned long |
| i | [EBP-0x20] | int |
| base_delta_x | [EBP-0x18] | int |

#### Block at 1:0x00124CE6 (linear 0x00134CE6), 44 bytes

| Local | Location | Type |
|---|---|---|
| mid | [EBP-0x14] | SDWORD |

#### isqrt

- Address: 1:0x00124C92 (linear 0x00134C92), 155 bytes, in `regions/07-object-1/001B9E54-001BA653.txt`
- Type: SDWORD (UDWORD); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| n | [EBP-0x10] | UDWORD |
| lo | [EBP-0xC] | SDWORD |
| hi | [EBP-0x8] | SDWORD |

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
| 30 | typedef | SDWORD | name for int (#8) |
| 31 | scalar | void | void 1 bytes |
| 32 | pointer |  | near386; of short (#6) |
| 33 | procedure |  | near386; returns void (short *, short *, short, short, short) |
| 34 | procedure |  | near386; returns void (short *, short *, short, short, short, short, short) |
| 35 | procedure |  | near386; returns short (short, short, short, short) |
| 36 | procedure |  | near386; returns short (short, short, short, short, short) |
| 37 | procedure |  | near386; returns short (short, short, short, short, char *, char *) |
| 38 | scalar | unsigned long | unsigned 4 bytes |
| 39 | pointer |  | near386; of signed char (#4) |
| 40 | procedure |  | near386; returns short (short, short, short, short, signed char *, signed char *, short) |
| 41 | procedure |  | near386; returns SDWORD (UDWORD) |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 8 | 1:0x0012432D (linear 0x0013432D) | 0x001B9981 |
| 12 | 1:0x0012434E (linear 0x0013434E) | 0x001B99A2 |
| 14 | 1:0x00124374 (linear 0x00134374) | 0x001B99C8 |
| 15 | 1:0x00124380 (linear 0x00134380) | 0x001B99D4 |
| 17 | 1:0x0012438C (linear 0x0013438C) | 0x001B99E0 |
| 18 | 1:0x001243AC (linear 0x001343AC) | 0x001B9A00 |
| 19 | 1:0x001243D2 (linear 0x001343D2) | 0x001B9A26 |
| 20 | 1:0x001243D4 (linear 0x001343D4) | 0x001B9A28 |
| 23 | 1:0x001243E4 (linear 0x001343E4) | 0x001B9A38 |
| 25 | 1:0x001243F0 (linear 0x001343F0) | 0x001B9A44 |
| 26 | 1:0x00124405 (linear 0x00134405) | 0x001B9A59 |
| 28 | 1:0x0012441A (linear 0x0013441A) | 0x001B9A6E |
| 30 | 1:0x0012441C (linear 0x0013441C) | 0x001B9A70 |
| 31 | 1:0x00124431 (linear 0x00134431) | 0x001B9A85 |
| 34 | 1:0x00124446 (linear 0x00134446) | 0x001B9A9A |
| 36 | 1:0x00124452 (linear 0x00134452) | 0x001B9AA6 |
| 37 | 1:0x00124467 (linear 0x00134467) | 0x001B9ABB |
| 39 | 1:0x0012447C (linear 0x0013447C) | 0x001B9AD0 |
| 41 | 1:0x0012447E (linear 0x0013447E) | 0x001B9AD2 |
| 42 | 1:0x00124493 (linear 0x00134493) | 0x001B9AE7 |
| 45 | 1:0x001244A8 (linear 0x001344A8) | 0x001B9AFC |
| 53 | 1:0x001244B0 (linear 0x001344B0) | 0x001B9B04 |
| 59 | 1:0x001244D1 (linear 0x001344D1) | 0x001B9B25 |
| 61 | 1:0x001244F7 (linear 0x001344F7) | 0x001B9B4B |
| 62 | 1:0x00124500 (linear 0x00134500) | 0x001B9B54 |
| 64 | 1:0x0012450A (linear 0x0013450A) | 0x001B9B5E |
| 65 | 1:0x00124513 (linear 0x00134513) | 0x001B9B67 |
| 68 | 1:0x0012451D (linear 0x0013451D) | 0x001B9B71 |
| 70 | 1:0x0012452A (linear 0x0013452A) | 0x001B9B7E |
| 72 | 1:0x00124533 (linear 0x00134533) | 0x001B9B87 |
| 73 | 1:0x0012453C (linear 0x0013453C) | 0x001B9B90 |
| 75 | 1:0x00124551 (linear 0x00134551) | 0x001B9BA5 |
| 77 | 1:0x00124553 (linear 0x00134553) | 0x001B9BA7 |
| 78 | 1:0x0012455C (linear 0x0013455C) | 0x001B9BB0 |
| 80 | 1:0x00124571 (linear 0x00134571) | 0x001B9BC5 |
| 82 | 1:0x001245AA (linear 0x001345AA) | 0x001B9BFE |
| 84 | 1:0x001245AF (linear 0x001345AF) | 0x001B9C03 |
| 86 | 1:0x001245B8 (linear 0x001345B8) | 0x001B9C0C |
| 87 | 1:0x001245C1 (linear 0x001345C1) | 0x001B9C15 |
| 89 | 1:0x001245D6 (linear 0x001345D6) | 0x001B9C2A |
| 91 | 1:0x001245D8 (linear 0x001345D8) | 0x001B9C2C |
| 92 | 1:0x001245E1 (linear 0x001345E1) | 0x001B9C35 |
| 94 | 1:0x001245F6 (linear 0x001345F6) | 0x001B9C4A |
| 97 | 1:0x0012462F (linear 0x0013462F) | 0x001B9C83 |
| 106 | 1:0x00124637 (linear 0x00134637) | 0x001B9C8B |
| 110 | 1:0x00124658 (linear 0x00134658) | 0x001B9CAC |
| 111 | 1:0x00124661 (linear 0x00134661) | 0x001B9CB5 |
| 113 | 1:0x0012466A (linear 0x0013466A) | 0x001B9CBE |
| 114 | 1:0x00124674 (linear 0x00134674) | 0x001B9CC8 |
| 116 | 1:0x0012467E (linear 0x0013467E) | 0x001B9CD2 |
| 117 | 1:0x0012469D (linear 0x0013469D) | 0x001B9CF1 |
| 119 | 1:0x001246B5 (linear 0x001346B5) | 0x001B9D09 |
| 120 | 1:0x001246BB (linear 0x001346BB) | 0x001B9D0F |
| 127 | 1:0x001246C4 (linear 0x001346C4) | 0x001B9D18 |
| 131 | 1:0x001246E5 (linear 0x001346E5) | 0x001B9D39 |
| 132 | 1:0x001246EE (linear 0x001346EE) | 0x001B9D42 |
| 134 | 1:0x001246F7 (linear 0x001346F7) | 0x001B9D4B |
| 135 | 1:0x00124701 (linear 0x00134701) | 0x001B9D55 |
| 137 | 1:0x0012470B (linear 0x0013470B) | 0x001B9D5F |
| 138 | 1:0x00124714 (linear 0x00134714) | 0x001B9D68 |
| 139 | 1:0x00124720 (linear 0x00134720) | 0x001B9D74 |
| 140 | 1:0x00124722 (linear 0x00134722) | 0x001B9D76 |
| 141 | 1:0x0012472E (linear 0x0013472E) | 0x001B9D82 |
| 143 | 1:0x00124738 (linear 0x00134738) | 0x001B9D8C |
| 144 | 1:0x00124747 (linear 0x00134747) | 0x001B9D9B |
| 146 | 1:0x00124758 (linear 0x00134758) | 0x001B9DAC |
| 147 | 1:0x0012475E (linear 0x0013475E) | 0x001B9DB2 |
| 156 | 1:0x00124769 (linear 0x00134769) | 0x001B9DBD |
| 161 | 1:0x0012478A (linear 0x0013478A) | 0x001B9DDE |
| 162 | 1:0x00124797 (linear 0x00134797) | 0x001B9DEB |
| 164 | 1:0x001247A4 (linear 0x001347A4) | 0x001B9DF8 |
| 166 | 1:0x001247AA (linear 0x001347AA) | 0x001B9DFE |
| 167 | 1:0x001247AD (linear 0x001347AD) | 0x001B9E01 |
| 169 | 1:0x001247B4 (linear 0x001347B4) | 0x001B9E08 |
| 171 | 1:0x001247BD (linear 0x001347BD) | 0x001B9E11 |
| 173 | 1:0x001247C3 (linear 0x001347C3) | 0x001B9E17 |
| 174 | 1:0x001247C6 (linear 0x001347C6) | 0x001B9E1A |
| 176 | 1:0x001247CD (linear 0x001347CD) | 0x001B9E21 |
| 178 | 1:0x001247D6 (linear 0x001347D6) | 0x001B9E2A |
| 180 | 1:0x001247DD (linear 0x001347DD) | 0x001B9E31 |
| 181 | 1:0x001247E4 (linear 0x001347E4) | 0x001B9E38 |
| 183 | 1:0x001247EB (linear 0x001347EB) | 0x001B9E3F |
| 185 | 1:0x001247F7 (linear 0x001347F7) | 0x001B9E4B |
| 187 | 1:0x001247FE (linear 0x001347FE) | 0x001B9E52 |
| 188 | 1:0x0012480C (linear 0x0013480C) | 0x001B9E60 |
| 190 | 1:0x00124812 (linear 0x00134812) | 0x001B9E66 |
| 192 | 1:0x0012481A (linear 0x0013481A) | 0x001B9E6E |
| 194 | 1:0x00124820 (linear 0x00134820) | 0x001B9E74 |
| 195 | 1:0x00124826 (linear 0x00134826) | 0x001B9E7A |
| 197 | 1:0x0012482F (linear 0x0013482F) | 0x001B9E83 |
| 198 | 1:0x00124832 (linear 0x00134832) | 0x001B9E86 |
| 201 | 1:0x00124839 (linear 0x00134839) | 0x001B9E8D |
| 202 | 1:0x00124844 (linear 0x00134844) | 0x001B9E98 |
| 203 | 1:0x0012484F (linear 0x0013484F) | 0x001B9EA3 |
| 204 | 1:0x00124852 (linear 0x00134852) | 0x001B9EA6 |
| 206 | 1:0x00124854 (linear 0x00134854) | 0x001B9EA8 |
| 208 | 1:0x00124856 (linear 0x00134856) | 0x001B9EAA |
| 210 | 1:0x0012485E (linear 0x0013485E) | 0x001B9EB2 |
| 212 | 1:0x00124864 (linear 0x00134864) | 0x001B9EB8 |
| 213 | 1:0x0012486A (linear 0x0013486A) | 0x001B9EBE |
| 215 | 1:0x00124873 (linear 0x00134873) | 0x001B9EC7 |
| 216 | 1:0x00124877 (linear 0x00134877) | 0x001B9ECB |
| 218 | 1:0x0012487E (linear 0x0013487E) | 0x001B9ED2 |
| 219 | 1:0x00124889 (linear 0x00134889) | 0x001B9EDD |
| 220 | 1:0x00124894 (linear 0x00134894) | 0x001B9EE8 |
| 221 | 1:0x00124897 (linear 0x00134897) | 0x001B9EEB |
| 224 | 1:0x00124899 (linear 0x00134899) | 0x001B9EED |
| 226 | 1:0x0012489E (linear 0x0013489E) | 0x001B9EF2 |
| 228 | 1:0x001248AA (linear 0x001348AA) | 0x001B9EFE |
| 229 | 1:0x001248B1 (linear 0x001348B1) | 0x001B9F05 |
| 231 | 1:0x001248BF (linear 0x001348BF) | 0x001B9F13 |
| 233 | 1:0x001248C5 (linear 0x001348C5) | 0x001B9F19 |
| 235 | 1:0x001248CD (linear 0x001348CD) | 0x001B9F21 |
| 237 | 1:0x001248D3 (linear 0x001348D3) | 0x001B9F27 |
| 238 | 1:0x001248D9 (linear 0x001348D9) | 0x001B9F2D |
| 240 | 1:0x001248E2 (linear 0x001348E2) | 0x001B9F36 |
| 241 | 1:0x001248E5 (linear 0x001348E5) | 0x001B9F39 |
| 244 | 1:0x001248EC (linear 0x001348EC) | 0x001B9F40 |
| 245 | 1:0x001248F7 (linear 0x001348F7) | 0x001B9F4B |
| 246 | 1:0x00124902 (linear 0x00134902) | 0x001B9F56 |
| 247 | 1:0x00124905 (linear 0x00134905) | 0x001B9F59 |
| 249 | 1:0x00124907 (linear 0x00134907) | 0x001B9F5B |
| 251 | 1:0x00124909 (linear 0x00134909) | 0x001B9F5D |
| 253 | 1:0x00124911 (linear 0x00134911) | 0x001B9F65 |
| 255 | 1:0x00124917 (linear 0x00134917) | 0x001B9F6B |
| 256 | 1:0x0012491D (linear 0x0013491D) | 0x001B9F71 |
| 258 | 1:0x00124926 (linear 0x00134926) | 0x001B9F7A |
| 259 | 1:0x0012492A (linear 0x0013492A) | 0x001B9F7E |
| 262 | 1:0x00124931 (linear 0x00134931) | 0x001B9F85 |
| 263 | 1:0x0012493C (linear 0x0013493C) | 0x001B9F90 |
| 264 | 1:0x00124947 (linear 0x00134947) | 0x001B9F9B |
| 265 | 1:0x0012494A (linear 0x0013494A) | 0x001B9F9E |
| 268 | 1:0x0012494C (linear 0x0013494C) | 0x001B9FA0 |
| 270 | 1:0x0012494E (linear 0x0013494E) | 0x001B9FA2 |
| 272 | 1:0x00124956 (linear 0x00134956) | 0x001B9FAA |
| 273 | 1:0x0012495C (linear 0x0013495C) | 0x001B9FB0 |
| 275 | 1:0x00124962 (linear 0x00134962) | 0x001B9FB6 |
| 276 | 1:0x0012496D (linear 0x0013496D) | 0x001B9FC1 |
| 277 | 1:0x00124978 (linear 0x00134978) | 0x001B9FCC |
| 278 | 1:0x0012497B (linear 0x0013497B) | 0x001B9FCF |
| 281 | 1:0x0012497D (linear 0x0013497D) | 0x001B9FD1 |
| 282 | 1:0x00124983 (linear 0x00134983) | 0x001B9FD7 |
| 308 | 1:0x0012498E (linear 0x0013498E) | 0x001B9FE2 |
| 315 | 1:0x001249AF (linear 0x001349AF) | 0x001BA003 |
| 316 | 1:0x001249E3 (linear 0x001349E3) | 0x001BA037 |
| 318 | 1:0x00124A17 (linear 0x00134A17) | 0x001BA06B |
| 319 | 1:0x00124A24 (linear 0x00134A24) | 0x001BA078 |
| 321 | 1:0x00124A31 (linear 0x00134A31) | 0x001BA085 |
| 323 | 1:0x00124A37 (linear 0x00134A37) | 0x001BA08B |
| 324 | 1:0x00124A3A (linear 0x00134A3A) | 0x001BA08E |
| 326 | 1:0x00124A41 (linear 0x00134A41) | 0x001BA095 |
| 328 | 1:0x00124A4A (linear 0x00134A4A) | 0x001BA09E |
| 330 | 1:0x00124A50 (linear 0x00134A50) | 0x001BA0A4 |
| 331 | 1:0x00124A53 (linear 0x00134A53) | 0x001BA0A7 |
| 333 | 1:0x00124A5A (linear 0x00134A5A) | 0x001BA0AE |
| 335 | 1:0x00124A63 (linear 0x00134A63) | 0x001BA0B7 |
| 337 | 1:0x00124A6A (linear 0x00134A6A) | 0x001BA0BE |
| 338 | 1:0x00124A71 (linear 0x00134A71) | 0x001BA0C5 |
| 340 | 1:0x00124A78 (linear 0x00134A78) | 0x001BA0CC |
| 342 | 1:0x00124A84 (linear 0x00134A84) | 0x001BA0D8 |
| 344 | 1:0x00124A8B (linear 0x00134A8B) | 0x001BA0DF |
| 346 | 1:0x00124A99 (linear 0x00134A99) | 0x001BA0ED |
| 348 | 1:0x00124A9F (linear 0x00134A9F) | 0x001BA0F3 |
| 350 | 1:0x00124AA7 (linear 0x00134AA7) | 0x001BA0FB |
| 352 | 1:0x00124AAD (linear 0x00134AAD) | 0x001BA101 |
| 353 | 1:0x00124AB3 (linear 0x00134AB3) | 0x001BA107 |
| 355 | 1:0x00124ABC (linear 0x00134ABC) | 0x001BA110 |
| 356 | 1:0x00124ABF (linear 0x00134ABF) | 0x001BA113 |
| 359 | 1:0x00124AC6 (linear 0x00134AC6) | 0x001BA11A |
| 360 | 1:0x00124AD1 (linear 0x00134AD1) | 0x001BA125 |
| 361 | 1:0x00124ADC (linear 0x00134ADC) | 0x001BA130 |
| 362 | 1:0x00124ADF (linear 0x00134ADF) | 0x001BA133 |
| 364 | 1:0x00124AE1 (linear 0x00134AE1) | 0x001BA135 |
| 366 | 1:0x00124AE3 (linear 0x00134AE3) | 0x001BA137 |
| 368 | 1:0x00124AEB (linear 0x00134AEB) | 0x001BA13F |
| 370 | 1:0x00124AF1 (linear 0x00134AF1) | 0x001BA145 |
| 371 | 1:0x00124AF7 (linear 0x00134AF7) | 0x001BA14B |
| 373 | 1:0x00124B00 (linear 0x00134B00) | 0x001BA154 |
| 374 | 1:0x00124B04 (linear 0x00134B04) | 0x001BA158 |
| 377 | 1:0x00124B0B (linear 0x00134B0B) | 0x001BA15F |
| 378 | 1:0x00124B16 (linear 0x00134B16) | 0x001BA16A |
| 379 | 1:0x00124B21 (linear 0x00134B21) | 0x001BA175 |
| 380 | 1:0x00124B24 (linear 0x00134B24) | 0x001BA178 |
| 383 | 1:0x00124B26 (linear 0x00134B26) | 0x001BA17A |
| 385 | 1:0x00124B2B (linear 0x00134B2B) | 0x001BA17F |
| 387 | 1:0x00124B37 (linear 0x00134B37) | 0x001BA18B |
| 388 | 1:0x00124B3E (linear 0x00134B3E) | 0x001BA192 |
| 390 | 1:0x00124B4C (linear 0x00134B4C) | 0x001BA1A0 |
| 392 | 1:0x00124B52 (linear 0x00134B52) | 0x001BA1A6 |
| 394 | 1:0x00124B5A (linear 0x00134B5A) | 0x001BA1AE |
| 396 | 1:0x00124B60 (linear 0x00134B60) | 0x001BA1B4 |
| 397 | 1:0x00124B66 (linear 0x00134B66) | 0x001BA1BA |
| 399 | 1:0x00124B6F (linear 0x00134B6F) | 0x001BA1C3 |
| 400 | 1:0x00124B72 (linear 0x00134B72) | 0x001BA1C6 |
| 403 | 1:0x00124B79 (linear 0x00134B79) | 0x001BA1CD |
| 404 | 1:0x00124B84 (linear 0x00134B84) | 0x001BA1D8 |
| 405 | 1:0x00124B8F (linear 0x00134B8F) | 0x001BA1E3 |
| 406 | 1:0x00124B92 (linear 0x00134B92) | 0x001BA1E6 |
| 408 | 1:0x00124B94 (linear 0x00134B94) | 0x001BA1E8 |
| 410 | 1:0x00124B96 (linear 0x00134B96) | 0x001BA1EA |
| 412 | 1:0x00124B9E (linear 0x00134B9E) | 0x001BA1F2 |
| 414 | 1:0x00124BA4 (linear 0x00134BA4) | 0x001BA1F8 |
| 415 | 1:0x00124BAA (linear 0x00134BAA) | 0x001BA1FE |
| 417 | 1:0x00124BB3 (linear 0x00134BB3) | 0x001BA207 |
| 418 | 1:0x00124BB7 (linear 0x00134BB7) | 0x001BA20B |
| 421 | 1:0x00124BBE (linear 0x00134BBE) | 0x001BA212 |
| 422 | 1:0x00124BC9 (linear 0x00134BC9) | 0x001BA21D |
| 423 | 1:0x00124BD4 (linear 0x00134BD4) | 0x001BA228 |
| 424 | 1:0x00124BD7 (linear 0x00134BD7) | 0x001BA22B |
| 427 | 1:0x00124BD9 (linear 0x00134BD9) | 0x001BA22D |
| 429 | 1:0x00124BDB (linear 0x00134BDB) | 0x001BA22F |
| 431 | 1:0x00124BE3 (linear 0x00134BE3) | 0x001BA237 |
| 432 | 1:0x00124BE9 (linear 0x00134BE9) | 0x001BA23D |
| 434 | 1:0x00124BEF (linear 0x00134BEF) | 0x001BA243 |
| 435 | 1:0x00124BFA (linear 0x00134BFA) | 0x001BA24E |
| 436 | 1:0x00124C05 (linear 0x00134C05) | 0x001BA259 |
| 437 | 1:0x00124C08 (linear 0x00134C08) | 0x001BA25C |
| 442 | 1:0x00124C0A (linear 0x00134C0A) | 0x001BA25E |
| 443 | 1:0x00124C25 (linear 0x00134C25) | 0x001BA279 |
| 445 | 1:0x00124C43 (linear 0x00134C43) | 0x001BA297 |
| 446 | 1:0x00124C5C (linear 0x00134C5C) | 0x001BA2B0 |
| 448 | 1:0x00124C81 (linear 0x00134C81) | 0x001BA2D5 |
| 449 | 1:0x00124C87 (linear 0x00134C87) | 0x001BA2DB |
| 452 | 1:0x00124C92 (linear 0x00134C92) | 0x001BA2E6 |
| 457 | 1:0x00124CAD (linear 0x00134CAD) | 0x001BA301 |
| 458 | 1:0x00124CB6 (linear 0x00134CB6) | 0x001BA30A |
| 460 | 1:0x00124CBF (linear 0x00134CBF) | 0x001BA313 |
| 462 | 1:0x00124CC8 (linear 0x00134CC8) | 0x001BA31C |
| 463 | 1:0x00124CCF (linear 0x00134CCF) | 0x001BA323 |
| 465 | 1:0x00124CD6 (linear 0x00134CD6) | 0x001BA32A |
| 467 | 1:0x00124CD8 (linear 0x00134CD8) | 0x001BA32C |
| 468 | 1:0x00124CDF (linear 0x00134CDF) | 0x001BA333 |
| 473 | 1:0x00124CE6 (linear 0x00134CE6) | 0x001BA33A |
| 475 | 1:0x00124CF8 (linear 0x00134CF8) | 0x001BA34C |
| 476 | 1:0x00124D04 (linear 0x00134D04) | 0x001BA358 |
| 477 | 1:0x00124D0A (linear 0x00134D0A) | 0x001BA35E |
| 478 | 1:0x00124D0C (linear 0x00134D0C) | 0x001BA360 |
| 480 | 1:0x00124D12 (linear 0x00134D12) | 0x001BA366 |
| 482 | 1:0x00124D1B (linear 0x00134D1B) | 0x001BA36F |
| 483 | 1:0x00124D21 (linear 0x00134D21) | 0x001BA375 |

