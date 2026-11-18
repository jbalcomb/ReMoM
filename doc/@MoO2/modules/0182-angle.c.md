# F:\WLIBS\special\angle.c

Module 182 of the debug info of ORION2.EXE.

Ranges: 1:0x00128385 (linear 0x00138385), 2,059 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x00128405 (linear 0x00138405) | static, code | _angle_to_sin | `regions/07-object-1/001BD654-001BDE53.txt` |
| 1:0x001284B9 (linear 0x001384B9) | code | Get_Angle_ | `regions/07-object-1/001BD654-001BDE53.txt` |
| 1:0x00128615 (linear 0x00138615) | code | Sin_ | `regions/07-object-1/001BD654-001BDE53.txt` |
| 1:0x00128776 (linear 0x00138776) | code | Cos_ | `regions/07-object-1/001BD654-001BDE53.txt` |
| 1:0x001287B2 (linear 0x001387B2) | code | Rotate_Coordinates_ | `regions/07-object-1/001BD654-001BDE53.txt` |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 1:0x00128405 (linear 0x00138405) | angle_to_sin | unsigned short[90] |

#### Block at 1:0x00128549 (linear 0x00138549), 28 bytes

| Local | Location | Type |
|---|---|---|
| old_x | [EBP-0x18] | int |

#### Block at 1:0x0012856D (linear 0x0013856D), 28 bytes

| Local | Location | Type |
|---|---|---|
| old_x | [EBP-0x1C] | int |

#### Block at 1:0x001285AB (linear 0x001385AB), 35 bytes

| Local | Location | Type |
|---|---|---|
| quotient | [EBP-0x20] | unsigned long |

#### Block at 1:0x001285D0 (linear 0x001385D0), 28 bytes

| Local | Location | Type |
|---|---|---|
| quotient | [EBP-0x24] | unsigned long |

#### Block at 1:0x001285A3 (linear 0x001385A3), 73 bytes

| Local | Location | Type |
|---|---|---|
| quotient_to_angle | address 1:0x00128385 | unsigned char[128] |

#### Block at 1:0x0012852E (linear 0x0013852E), 214 bytes

| Local | Location | Type |
|---|---|---|
| correction | [EBP-0x14] | int |

#### Get_Angle

- Address: 1:0x001284B9 (linear 0x001384B9), 348 bytes, in `regions/07-object-1/001BD654-001BDE53.txt`
- Type: short (int, int); returns in AX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| x | [EBP-0x10] | int |
| y | [EBP-0xC] | int |
| result | [EBP-0x8] | short |

#### Sin

- Address: 1:0x00128615 (linear 0x00138615), 353 bytes, in `regions/07-object-1/001BD654-001BDE53.txt`
- Type: int (short, int); returns in EAX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| angle | [EBP-0x4] | short |
| length | [EBP-0xC] | int |
| result | [EBP-0x10] | int |

#### Cos

- Address: 1:0x00128776 (linear 0x00138776), 60 bytes, in `regions/07-object-1/001BD654-001BDE53.txt`
- Type: int (short, int); returns in EAX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| angle | [EBP-0x4] | short |
| length | [EBP-0xC] | int |

#### Block at 1:0x001288C0 (linear 0x001388C0), 36 bytes

| Local | Location | Type |
|---|---|---|
| new_x | [EBP-0x24] | int |

#### Block at 1:0x00128A53 (linear 0x00138A53), 35 bytes

| Local | Location | Type |
|---|---|---|
| new_x | [EBP-0x28] | int |

#### Block at 1:0x00128B0D (linear 0x00138B0D), 112 bytes

| Local | Location | Type |
|---|---|---|
| new_x | [EBP-0x2C] | int |
| new_y | [EBP-0x30] | int |

#### Rotate_Coordinates

- Address: 1:0x001287B2 (linear 0x001387B2), 978 bytes, in `regions/07-object-1/001BD654-001BDE53.txt`
- Type: void (short, int *, int *)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| angle | [EBP-0x4] | short |
| x | [EBP-0x1C] | int * |
| y | [EBP-0x18] | int * |
| sin_angle_x | [EBP-0x14] | int |
| sin_angle_y | [EBP-0x20] | int |
| cos_angle_x | [EBP-0xC] | int |
| cos_angle_y | [EBP-0x8] | int |
| trivial | [EBP-0x10] | int |

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
| 30 | array |  | 0..89; of unsigned short (#7) |
| 31 | scalar | unsigned long | unsigned 4 bytes |
| 32 | array |  | 0..127; of unsigned char (#5) |
| 33 | procedure |  | near386; returns short (int, int) |
| 34 | procedure |  | near386; returns int (short, int) |
| 35 | scalar | void | void 1 bytes |
| 36 | pointer |  | near386; of int (#8) |
| 37 | procedure |  | near386; returns void (short, int *, int *) |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 43 | 1:0x001284B9 (linear 0x001384B9) | 0x001BDB0D |
| 45 | 1:0x001284D6 (linear 0x001384D6) | 0x001BDB2A |
| 47 | 1:0x001284DD (linear 0x001384DD) | 0x001BDB31 |
| 48 | 1:0x001284E3 (linear 0x001384E3) | 0x001BDB37 |
| 49 | 1:0x001284E9 (linear 0x001384E9) | 0x001BDB3D |
| 50 | 1:0x001284F0 (linear 0x001384F0) | 0x001BDB44 |
| 51 | 1:0x001284F8 (linear 0x001384F8) | 0x001BDB4C |
| 52 | 1:0x001284FF (linear 0x001384FF) | 0x001BDB53 |
| 53 | 1:0x00128501 (linear 0x00138501) | 0x001BDB55 |
| 56 | 1:0x00128508 (linear 0x00138508) | 0x001BDB5C |
| 57 | 1:0x00128513 (linear 0x00138513) | 0x001BDB67 |
| 58 | 1:0x00128519 (linear 0x00138519) | 0x001BDB6D |
| 59 | 1:0x00128520 (linear 0x00138520) | 0x001BDB74 |
| 60 | 1:0x00128522 (linear 0x00138522) | 0x001BDB76 |
| 62 | 1:0x00128529 (linear 0x00138529) | 0x001BDB7D |
| 65 | 1:0x0012852E (linear 0x0013852E) | 0x001BDB82 |
| 66 | 1:0x00128534 (linear 0x00138534) | 0x001BDB88 |
| 67 | 1:0x0012853A (linear 0x0013853A) | 0x001BDB8E |
| 68 | 1:0x0012853D (linear 0x0013853D) | 0x001BDB91 |
| 69 | 1:0x00128540 (linear 0x00138540) | 0x001BDB94 |
| 70 | 1:0x00128547 (linear 0x00138547) | 0x001BDB9B |
| 71 | 1:0x00128549 (linear 0x00138549) | 0x001BDB9D |
| 72 | 1:0x0012854F (linear 0x0013854F) | 0x001BDBA3 |
| 73 | 1:0x00128555 (linear 0x00138555) | 0x001BDBA9 |
| 74 | 1:0x0012855E (linear 0x0013855E) | 0x001BDBB2 |
| 75 | 1:0x00128565 (linear 0x00138565) | 0x001BDBB9 |
| 77 | 1:0x00128567 (linear 0x00138567) | 0x001BDBBB |
| 78 | 1:0x0012856D (linear 0x0013856D) | 0x001BDBC1 |
| 79 | 1:0x00128573 (linear 0x00138573) | 0x001BDBC7 |
| 80 | 1:0x0012857C (linear 0x0013857C) | 0x001BDBD0 |
| 81 | 1:0x00128582 (linear 0x00138582) | 0x001BDBD6 |
| 82 | 1:0x00128589 (linear 0x00138589) | 0x001BDBDD |
| 83 | 1:0x0012858B (linear 0x0013858B) | 0x001BDBDF |
| 89 | 1:0x00128592 (linear 0x00138592) | 0x001BDBE6 |
| 90 | 1:0x0012859A (linear 0x0013859A) | 0x001BDBEE |
| 91 | 1:0x001285A1 (linear 0x001385A1) | 0x001BDBF5 |
| 108 | 1:0x001285A3 (linear 0x001385A3) | 0x001BDBF7 |
| 109 | 1:0x001285AB (linear 0x001385AB) | 0x001BDBFF |
| 110 | 1:0x001285B9 (linear 0x001385B9) | 0x001BDC0D |
| 111 | 1:0x001285CE (linear 0x001385CE) | 0x001BDC22 |
| 112 | 1:0x001285D0 (linear 0x001385D0) | 0x001BDC24 |
| 113 | 1:0x001285DE (linear 0x001385DE) | 0x001BDC32 |
| 114 | 1:0x001285EC (linear 0x001385EC) | 0x001BDC40 |
| 117 | 1:0x001285F2 (linear 0x001385F2) | 0x001BDC46 |
| 118 | 1:0x001285FD (linear 0x001385FD) | 0x001BDC51 |
| 120 | 1:0x00128604 (linear 0x00138604) | 0x001BDC58 |
| 122 | 1:0x0012860A (linear 0x0013860A) | 0x001BDC5E |
| 147 | 1:0x00128615 (linear 0x00138615) | 0x001BDC69 |
| 152 | 1:0x00128632 (linear 0x00138632) | 0x001BDC86 |
| 153 | 1:0x00128638 (linear 0x00138638) | 0x001BDC8C |
| 154 | 1:0x0012863F (linear 0x0013863F) | 0x001BDC93 |
| 156 | 1:0x00128646 (linear 0x00138646) | 0x001BDC9A |
| 158 | 1:0x00128651 (linear 0x00138651) | 0x001BDCA5 |
| 159 | 1:0x00128658 (linear 0x00138658) | 0x001BDCAC |
| 161 | 1:0x00128663 (linear 0x00138663) | 0x001BDCB7 |
| 163 | 1:0x0012866C (linear 0x0013866C) | 0x001BDCC0 |
| 164 | 1:0x00128673 (linear 0x00138673) | 0x001BDCC7 |
| 168 | 1:0x0012867A (linear 0x0013867A) | 0x001BDCCE |
| 169 | 1:0x00128685 (linear 0x00138685) | 0x001BDCD9 |
| 170 | 1:0x0012868E (linear 0x0013868E) | 0x001BDCE2 |
| 171 | 1:0x001286AF (linear 0x001386AF) | 0x001BDD03 |
| 172 | 1:0x001286BA (linear 0x001386BA) | 0x001BDD0E |
| 173 | 1:0x001286C0 (linear 0x001386C0) | 0x001BDD14 |
| 174 | 1:0x001286C2 (linear 0x001386C2) | 0x001BDD16 |
| 176 | 1:0x001286EC (linear 0x001386EC) | 0x001BDD40 |
| 177 | 1:0x001286F1 (linear 0x001386F1) | 0x001BDD45 |
| 178 | 1:0x001286FC (linear 0x001386FC) | 0x001BDD50 |
| 179 | 1:0x00128720 (linear 0x00138720) | 0x001BDD74 |
| 180 | 1:0x0012872D (linear 0x0013872D) | 0x001BDD81 |
| 181 | 1:0x00128736 (linear 0x00138736) | 0x001BDD8A |
| 182 | 1:0x00128738 (linear 0x00138738) | 0x001BDD8C |
| 185 | 1:0x00128765 (linear 0x00138765) | 0x001BDDB9 |
| 186 | 1:0x0012876B (linear 0x0013876B) | 0x001BDDBF |
| 188 | 1:0x00128776 (linear 0x00138776) | 0x001BDDCA |
| 190 | 1:0x00128793 (linear 0x00138793) | 0x001BDDE7 |
| 191 | 1:0x001287A7 (linear 0x001387A7) | 0x001BDDFB |
| 206 | 1:0x001287B2 (linear 0x001387B2) | 0x001BDE06 |
| 212 | 1:0x001287D1 (linear 0x001387D1) | 0x001BDE25 |
| 215 | 1:0x001287D8 (linear 0x001387D8) | 0x001BDE2C |
| 217 | 1:0x001287E3 (linear 0x001387E3) | 0x001BDE37 |
| 218 | 1:0x001287EA (linear 0x001387EA) | 0x001BDE3E |
| 219 | 1:0x001287F5 (linear 0x001387F5) | 0x001BDE49 |
| 221 | 1:0x001287FE (linear 0x001387FE) | 0x001BDE52 |
| 222 | 1:0x00128805 (linear 0x00138805) | 0x001BDE59 |
| 225 | 1:0x0012880C (linear 0x0013880C) | 0x001BDE60 |
| 226 | 1:0x0012881B (linear 0x0013881B) | 0x001BDE6F |
| 227 | 1:0x00128828 (linear 0x00138828) | 0x001BDE7C |
| 228 | 1:0x0012882F (linear 0x0013882F) | 0x001BDE83 |
| 229 | 1:0x00128836 (linear 0x00138836) | 0x001BDE8A |
| 230 | 1:0x00128838 (linear 0x00138838) | 0x001BDE8C |
| 231 | 1:0x00128852 (linear 0x00138852) | 0x001BDEA6 |
| 232 | 1:0x0012886C (linear 0x0013886C) | 0x001BDEC0 |
| 233 | 1:0x0012888F (linear 0x0013888F) | 0x001BDEE3 |
| 235 | 1:0x001288B2 (linear 0x001388B2) | 0x001BDF06 |
| 236 | 1:0x001288B7 (linear 0x001388B7) | 0x001BDF0B |
| 237 | 1:0x001288C0 (linear 0x001388C0) | 0x001BDF14 |
| 238 | 1:0x001288CB (linear 0x001388CB) | 0x001BDF1F |
| 239 | 1:0x001288D5 (linear 0x001388D5) | 0x001BDF29 |
| 240 | 1:0x001288DD (linear 0x001388DD) | 0x001BDF31 |
| 241 | 1:0x001288E4 (linear 0x001388E4) | 0x001BDF38 |
| 242 | 1:0x001288E9 (linear 0x001388E9) | 0x001BDF3D |
| 243 | 1:0x0012890C (linear 0x0013890C) | 0x001BDF60 |
| 244 | 1:0x0012892F (linear 0x0013892F) | 0x001BDF83 |
| 245 | 1:0x00128950 (linear 0x00138950) | 0x001BDFA4 |
| 248 | 1:0x00128971 (linear 0x00138971) | 0x001BDFC5 |
| 249 | 1:0x00128976 (linear 0x00138976) | 0x001BDFCA |
| 250 | 1:0x00128985 (linear 0x00138985) | 0x001BDFD9 |
| 251 | 1:0x00128990 (linear 0x00138990) | 0x001BDFE4 |
| 252 | 1:0x0012899B (linear 0x0013899B) | 0x001BDFEF |
| 253 | 1:0x001289A6 (linear 0x001389A6) | 0x001BDFFA |
| 254 | 1:0x001289AD (linear 0x001389AD) | 0x001BE001 |
| 255 | 1:0x001289B2 (linear 0x001389B2) | 0x001BE006 |
| 256 | 1:0x001289CE (linear 0x001389CE) | 0x001BE022 |
| 257 | 1:0x001289EF (linear 0x001389EF) | 0x001BE043 |
| 258 | 1:0x00128A19 (linear 0x00138A19) | 0x001BE06D |
| 260 | 1:0x00128A43 (linear 0x00138A43) | 0x001BE097 |
| 261 | 1:0x00128A48 (linear 0x00138A48) | 0x001BE09C |
| 262 | 1:0x00128A53 (linear 0x00138A53) | 0x001BE0A7 |
| 263 | 1:0x00128A5B (linear 0x00138A5B) | 0x001BE0AF |
| 264 | 1:0x00128A67 (linear 0x00138A67) | 0x001BE0BB |
| 265 | 1:0x00128A6F (linear 0x00138A6F) | 0x001BE0C3 |
| 266 | 1:0x00128A76 (linear 0x00138A76) | 0x001BE0CA |
| 267 | 1:0x00128A7B (linear 0x00138A7B) | 0x001BE0CF |
| 268 | 1:0x00128AA5 (linear 0x00138AA5) | 0x001BE0F9 |
| 269 | 1:0x00128ACF (linear 0x00138ACF) | 0x001BE123 |
| 270 | 1:0x00128AE9 (linear 0x00138AE9) | 0x001BE13D |
| 274 | 1:0x00128B03 (linear 0x00138B03) | 0x001BE157 |
| 275 | 1:0x00128B0D (linear 0x00138B0D) | 0x001BE161 |
| 276 | 1:0x00128B16 (linear 0x00138B16) | 0x001BE16A |
| 277 | 1:0x00128B1F (linear 0x00138B1F) | 0x001BE173 |
| 278 | 1:0x00128B25 (linear 0x00138B25) | 0x001BE179 |
| 279 | 1:0x00128B3B (linear 0x00138B3B) | 0x001BE18F |
| 280 | 1:0x00128B3D (linear 0x00138B3D) | 0x001BE191 |
| 282 | 1:0x00128B4E (linear 0x00138B4E) | 0x001BE1A2 |
| 283 | 1:0x00128B54 (linear 0x00138B54) | 0x001BE1A8 |
| 284 | 1:0x00128B6A (linear 0x00138B6A) | 0x001BE1BE |
| 285 | 1:0x00128B6C (linear 0x00138B6C) | 0x001BE1C0 |
| 287 | 1:0x00128B7D (linear 0x00138B7D) | 0x001BE1D1 |

