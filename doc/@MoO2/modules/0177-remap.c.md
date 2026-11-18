# F:\WLIBS\palette\remap.c

Module 177 of the debug info of ORION2.EXE.

Ranges: 1:0x00123237 (linear 0x00133237), 3,135 bytes, 2:0x00002ECC (linear 0x00172ECC), 124 bytes, 2:0x0004479C (linear 0x001B479C), 256 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x00123237 (linear 0x00133237) | code | Update_Glass_Remap_Colors_ | `regions/07-object-1/001B8654-001B8E53.txt` |
| 1:0x001234BB (linear 0x001334BB) | code | Update_Glass_Remap_Color_Range_ | `regions/07-object-1/001B8654-001B8E53.txt` |
| 1:0x0012372A (linear 0x0013372A) | code | Sort_Palette_List_ | `regions/07-object-1/001B8654-001B8E53.txt` |
| 1:0x001238C9 (linear 0x001338C9) | code | Find_Closest_Color_ | `regions/07-object-1/001B8E54-001B9653.txt` |
| 1:0x00123BAE (linear 0x00133BAE) | code | Get_Palette_Animation_Color_ | `regions/07-object-1/001B8E54-001B9653.txt` |
| 1:0x00123C9C (linear 0x00133C9C) | code | Calculate_Picture_Remap_Colors_ | `regions/07-object-1/001B8E54-001B9653.txt` |
| 1:0x00123D16 (linear 0x00133D16) | code | Create_Picture_Remap_Palette_ | `regions/07-object-1/001B8E54-001B9653.txt` |
| 1:0x00123DE1 (linear 0x00133DE1) | code | Get_Remap_Palette_Color_ | `regions/07-object-1/001B8E54-001B9653.txt` |
| 1:0x00123E16 (linear 0x00133E16) | code | Get_Palette_RGB_ | `regions/07-object-1/001B8E54-001B9653.txt` |
| 2:0x0004479C (linear 0x001B479C) | data | _picture_remap_color_list | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x0004479C (linear 0x001B479C) | picture_remap_color_list | unsigned char[256] |

#### Update_Glass_Remap_Colors

- Address: 1:0x00123237 (linear 0x00133237), 644 bytes, in `regions/07-object-1/001B8654-001B8E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x20] | int |
| j | [EBP-0x1C] | int |
| tint_red | [EBP-0x18] | int |
| tint_green | [EBP-0x14] | int |
| tint_blue | [EBP-0x10] | int |
| red | [EBP-0xC] | int |
| green | [EBP-0x8] | int |
| blue | [EBP-0x4] | int |

#### Update_Glass_Remap_Color_Range

- Address: 1:0x001234BB (linear 0x001334BB), 623 bytes, in `regions/07-object-1/001B8654-001B8E53.txt`
- Type: void (short, short)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| start_color | [EBP-0x8] | short |
| end_color | [EBP-0x4] | short |
| i | [EBP-0x20] | int |
| j | [EBP-0x1C] | int |
| tint_red | [EBP-0x18] | int |
| tint_green | [EBP-0x28] | int |
| tint_blue | [EBP-0x24] | int |
| red | [EBP-0xC] | int |
| green | [EBP-0x14] | int |
| blue | [EBP-0x10] | int |

#### Sort_Palette_List

- Address: 1:0x0012372A (linear 0x0013372A), 415 bytes, in `regions/07-object-1/001B8654-001B8E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x14] | int |
| j | [EBP-0x10] | int |
| count | [EBP-0xC] | int |
| index | [EBP-0x8] | int |
| found | [EBP-0x4] | char |

#### Find_Closest_Color

- Address: 1:0x001238C9 (linear 0x001338C9), 741 bytes, in `regions/07-object-1/001B8E54-001B9653.txt`
- Type: short (char, char, char); returns in AX
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| red | [EBP-0x4] | char |
| green | [EBP-0xC] | char |
| blue | [EBP-0x10] | char |
| i | [EBP-0x58] | int |
| pos | [EBP-0x54] | int |
| closest_value | [EBP-0x50] | int |
| dd_red | [EBP-0x4C] | int |
| dd_green | [EBP-0x34] | int |
| dd_blue | [EBP-0x44] | int |
| value | [EBP-0x40] | int |
| count | [EBP-0x3C] | int |
| min_red | [EBP-0x38] | int |
| max_red | [EBP-0x48] | int |
| min_green | [EBP-0x60] | int |
| max_green | [EBP-0x5C] | int |
| min_blue | [EBP-0x28] | int |
| max_blue | [EBP-0x30] | int |
| closest_distance | [EBP-0x2C] | int |
| distance | [EBP-0x20] | int |
| hits | [EBP-0x260] | unsigned short[255] |
| hit_count | [EBP-0x18] | short |
| total_hits | [EBP-0x1C] | short |
| closest_color | [EBP-0x24] | int |
| found_color | [EBP-0x8] | char |

#### Get_Palette_Animation_Color

- Address: 1:0x00123BAE (linear 0x00133BAE), 238 bytes, in `regions/07-object-1/001B8E54-001B9653.txt`
- Type: void (char *, int, char *, char *, char *)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| picture | [EBP-0x1C] | char * |
| color_index | [EBP-0x18] | int |
| red | [EBP-0x14] | char * |
| green | [EBP-0x10] | char * |
| blue | [EBP+0x10] | char * |
| start_color | [EBP-0xC] | int |
| index | [EBP-0x8] | int |
| new_palette | [EBP-0x4] | char * |

#### Calculate_Picture_Remap_Colors

- Address: 1:0x00123C9C (linear 0x00133C9C), 122 bytes, in `regions/07-object-1/001B8E54-001B9653.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| picture | [EBP-0x4] | char * |

#### Create_Picture_Remap_Palette

- Address: 1:0x00123D16 (linear 0x00133D16), 203 bytes, in `regions/07-object-1/001B8E54-001B9653.txt`
- Type: void (char *, char *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| new_palette | [EBP-0x20] | char * |
| remaped_palette_colors | [EBP-0x1C] | char * |
| green | [EBP-0x8] | char |
| blue | [EBP-0x4] | char |
| red | [EBP-0xC] | char |
| i | [EBP-0x10] | int |
| color | [EBP-0x18] | int |
| index | [EBP-0x14] | int |

#### Get_Remap_Palette_Color

- Address: 1:0x00123DE1 (linear 0x00133DE1), 53 bytes, in `regions/07-object-1/001B8E54-001B9653.txt`
- Type: char (char); returns in AL
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| color_num | [EBP-0x8] | char |

#### Get_Palette_RGB

- Address: 1:0x00123E16 (linear 0x00133E16), 96 bytes, in `regions/07-object-1/001B8E54-001B9653.txt`
- Type: void (char, char *, char *, char *)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| palette_num | [EBP-0x4] | char |
| red | [EBP-0xC] | char * |
| green | [EBP-0x10] | char * |
| blue | [EBP-0x8] | char * |

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
| 30 | array |  | 0..255; of unsigned char (#5) |
| 31 | scalar | void | void 1 bytes |
| 32 | procedure |  | near386; returns void (void) |
| 33 | procedure |  | near386; returns void (short, short) |
| 34 | procedure |  | near386; returns void () |
| 35 | procedure |  | near386; returns short (char, char, char) |
| 36 | array |  | 0..254; of unsigned short (#7) |
| 37 | procedure |  | near386; returns void (char *, int, char *, char *, char *) |
| 38 | procedure |  | near386; returns void (char *) |
| 39 | procedure |  | near386; returns void (char *, char *) |
| 40 | procedure |  | near386; returns char (char) |
| 41 | procedure |  | near386; returns void (char, char *, char *, char *) |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 24 | 1:0x00123237 (linear 0x00133237) | 0x001B888B |
| 28 | 1:0x0012324F (linear 0x0013324F) | 0x001B88A3 |
| 29 | 1:0x00123256 (linear 0x00133256) | 0x001B88AA |
| 30 | 1:0x0012325D (linear 0x0013325D) | 0x001B88B1 |
| 31 | 1:0x00123264 (linear 0x00133264) | 0x001B88B8 |
| 33 | 1:0x0012326B (linear 0x0013326B) | 0x001B88BF |
| 35 | 1:0x00123270 (linear 0x00133270) | 0x001B88C4 |
| 39 | 1:0x0012328D (linear 0x0013328D) | 0x001B88E1 |
| 41 | 1:0x001232A7 (linear 0x001332A7) | 0x001B88FB |
| 44 | 1:0x001232D1 (linear 0x001332D1) | 0x001B8925 |
| 45 | 1:0x001232F6 (linear 0x001332F6) | 0x001B894A |
| 46 | 1:0x00123318 (linear 0x00133318) | 0x001B896C |
| 47 | 1:0x00123341 (linear 0x00133341) | 0x001B8995 |
| 48 | 1:0x00123373 (linear 0x00133373) | 0x001B89C7 |
| 49 | 1:0x0012339E (linear 0x0013339E) | 0x001B89F2 |
| 51 | 1:0x001233C9 (linear 0x001333C9) | 0x001B8A1D |
| 52 | 1:0x001233DE (linear 0x001333DE) | 0x001B8A32 |
| 53 | 1:0x001233F3 (linear 0x001333F3) | 0x001B8A47 |
| 55 | 1:0x00123408 (linear 0x00133408) | 0x001B8A5C |
| 57 | 1:0x0012342B (linear 0x0013342B) | 0x001B8A7F |
| 59 | 1:0x00123430 (linear 0x00133430) | 0x001B8A84 |
| 61 | 1:0x0012343F (linear 0x0013343F) | 0x001B8A93 |
| 63 | 1:0x00123451 (linear 0x00133451) | 0x001B8AA5 |
| 65 | 1:0x00123467 (linear 0x00133467) | 0x001B8ABB |
| 68 | 1:0x001234A8 (linear 0x001334A8) | 0x001B8AFC |
| 70 | 1:0x001234AD (linear 0x001334AD) | 0x001B8B01 |
| 71 | 1:0x001234B2 (linear 0x001334B2) | 0x001B8B06 |
| 79 | 1:0x001234BB (linear 0x001334BB) | 0x001B8B0F |
| 83 | 1:0x001234D8 (linear 0x001334D8) | 0x001B8B2C |
| 85 | 1:0x001234DD (linear 0x001334DD) | 0x001B8B31 |
| 87 | 1:0x001234ED (linear 0x001334ED) | 0x001B8B41 |
| 89 | 1:0x0012350A (linear 0x0013350A) | 0x001B8B5E |
| 91 | 1:0x0012351D (linear 0x0013351D) | 0x001B8B71 |
| 93 | 1:0x0012353A (linear 0x0013353A) | 0x001B8B8E |
| 95 | 1:0x0012354D (linear 0x0013354D) | 0x001B8BA1 |
| 97 | 1:0x00123567 (linear 0x00133567) | 0x001B8BBB |
| 98 | 1:0x0012358C (linear 0x0013358C) | 0x001B8BE0 |
| 99 | 1:0x001235AE (linear 0x001335AE) | 0x001B8C02 |
| 100 | 1:0x001235D7 (linear 0x001335D7) | 0x001B8C2B |
| 101 | 1:0x00123609 (linear 0x00133609) | 0x001B8C5D |
| 102 | 1:0x00123634 (linear 0x00133634) | 0x001B8C88 |
| 104 | 1:0x0012365F (linear 0x0013365F) | 0x001B8CB3 |
| 105 | 1:0x00123674 (linear 0x00133674) | 0x001B8CC8 |
| 106 | 1:0x00123689 (linear 0x00133689) | 0x001B8CDD |
| 108 | 1:0x0012369E (linear 0x0013369E) | 0x001B8CF2 |
| 110 | 1:0x001236C1 (linear 0x001336C1) | 0x001B8D15 |
| 112 | 1:0x001236C3 (linear 0x001336C3) | 0x001B8D17 |
| 115 | 1:0x00123704 (linear 0x00133704) | 0x001B8D58 |
| 117 | 1:0x00123706 (linear 0x00133706) | 0x001B8D5A |
| 120 | 1:0x00123718 (linear 0x00133718) | 0x001B8D6C |
| 122 | 1:0x0012371D (linear 0x0013371D) | 0x001B8D71 |
| 123 | 1:0x00123722 (linear 0x00133722) | 0x001B8D76 |
| 132 | 1:0x0012372A (linear 0x0013372A) | 0x001B8D7E |
| 137 | 1:0x00123742 (linear 0x00133742) | 0x001B8D96 |
| 138 | 1:0x00123749 (linear 0x00133749) | 0x001B8D9D |
| 140 | 1:0x00123756 (linear 0x00133756) | 0x001B8DAA |
| 141 | 1:0x0012375A (linear 0x0013375A) | 0x001B8DAE |
| 142 | 1:0x00123760 (linear 0x00133760) | 0x001B8DB4 |
| 144 | 1:0x00123783 (linear 0x00133783) | 0x001B8DD7 |
| 146 | 1:0x001237A5 (linear 0x001337A5) | 0x001B8DF9 |
| 147 | 1:0x001237AB (linear 0x001337AB) | 0x001B8DFF |
| 149 | 1:0x001237AF (linear 0x001337AF) | 0x001B8E03 |
| 150 | 1:0x001237B1 (linear 0x001337B1) | 0x001B8E05 |
| 152 | 1:0x001237B9 (linear 0x001337B9) | 0x001B8E0D |
| 154 | 1:0x001237D1 (linear 0x001337D1) | 0x001B8E25 |
| 155 | 1:0x001237E3 (linear 0x001337E3) | 0x001B8E37 |
| 156 | 1:0x001237E5 (linear 0x001337E5) | 0x001B8E39 |
| 157 | 1:0x001237F1 (linear 0x001337F1) | 0x001B8E45 |
| 159 | 1:0x001237F4 (linear 0x001337F4) | 0x001B8E48 |
| 161 | 1:0x001237F6 (linear 0x001337F6) | 0x001B8E4A |
| 162 | 1:0x00123802 (linear 0x00133802) | 0x001B8E56 |
| 164 | 1:0x00123805 (linear 0x00133805) | 0x001B8E59 |
| 166 | 1:0x0012380A (linear 0x0013380A) | 0x001B8E5E |
| 168 | 1:0x00123814 (linear 0x00133814) | 0x001B8E68 |
| 170 | 1:0x0012382E (linear 0x0013382E) | 0x001B8E82 |
| 171 | 1:0x00123832 (linear 0x00133832) | 0x001B8E86 |
| 173 | 1:0x00123856 (linear 0x00133856) | 0x001B8EAA |
| 175 | 1:0x00123877 (linear 0x00133877) | 0x001B8ECB |
| 176 | 1:0x0012387B (linear 0x0013387B) | 0x001B8ECF |
| 178 | 1:0x00123891 (linear 0x00133891) | 0x001B8EE5 |
| 179 | 1:0x00123893 (linear 0x00133893) | 0x001B8EE7 |
| 181 | 1:0x00123899 (linear 0x00133899) | 0x001B8EED |
| 184 | 1:0x001238B1 (linear 0x001338B1) | 0x001B8F05 |
| 186 | 1:0x001238B6 (linear 0x001338B6) | 0x001B8F0A |
| 187 | 1:0x001238C0 (linear 0x001338C0) | 0x001B8F14 |
| 191 | 1:0x001238C9 (linear 0x001338C9) | 0x001B8F1D |
| 204 | 1:0x001238E8 (linear 0x001338E8) | 0x001B8F3C |
| 205 | 1:0x001238F0 (linear 0x001338F0) | 0x001B8F44 |
| 206 | 1:0x001238F8 (linear 0x001338F8) | 0x001B8F4C |
| 207 | 1:0x00123900 (linear 0x00133900) | 0x001B8F54 |
| 208 | 1:0x00123908 (linear 0x00133908) | 0x001B8F5C |
| 209 | 1:0x00123910 (linear 0x00133910) | 0x001B8F64 |
| 210 | 1:0x00123918 (linear 0x00133918) | 0x001B8F6C |
| 212 | 1:0x0012391F (linear 0x0013391F) | 0x001B8F73 |
| 214 | 1:0x00123923 (linear 0x00133923) | 0x001B8F77 |
| 216 | 1:0x0012392D (linear 0x0013392D) | 0x001B8F81 |
| 217 | 1:0x00123933 (linear 0x00133933) | 0x001B8F87 |
| 218 | 1:0x00123937 (linear 0x00133937) | 0x001B8F8B |
| 219 | 1:0x00123939 (linear 0x00133939) | 0x001B8F8D |
| 220 | 1:0x00123940 (linear 0x00133940) | 0x001B8F94 |
| 221 | 1:0x00123946 (linear 0x00133946) | 0x001B8F9A |
| 222 | 1:0x0012394A (linear 0x0013394A) | 0x001B8F9E |
| 223 | 1:0x0012394C (linear 0x0013394C) | 0x001B8FA0 |
| 225 | 1:0x00123953 (linear 0x00133953) | 0x001B8FA7 |
| 226 | 1:0x00123959 (linear 0x00133959) | 0x001B8FAD |
| 227 | 1:0x0012395D (linear 0x0013395D) | 0x001B8FB1 |
| 228 | 1:0x0012395F (linear 0x0013395F) | 0x001B8FB3 |
| 229 | 1:0x00123966 (linear 0x00133966) | 0x001B8FBA |
| 230 | 1:0x0012396C (linear 0x0013396C) | 0x001B8FC0 |
| 231 | 1:0x00123970 (linear 0x00133970) | 0x001B8FC4 |
| 232 | 1:0x00123972 (linear 0x00133972) | 0x001B8FC6 |
| 233 | 1:0x00123979 (linear 0x00133979) | 0x001B8FCD |
| 234 | 1:0x0012397F (linear 0x0013397F) | 0x001B8FD3 |
| 235 | 1:0x00123983 (linear 0x00133983) | 0x001B8FD7 |
| 236 | 1:0x00123985 (linear 0x00133985) | 0x001B8FD9 |
| 237 | 1:0x0012398C (linear 0x0013398C) | 0x001B8FE0 |
| 238 | 1:0x00123992 (linear 0x00133992) | 0x001B8FE6 |
| 239 | 1:0x00123996 (linear 0x00133996) | 0x001B8FEA |
| 240 | 1:0x00123998 (linear 0x00133998) | 0x001B8FEC |
| 242 | 1:0x0012399F (linear 0x0013399F) | 0x001B8FF3 |
| 243 | 1:0x001239A6 (linear 0x001339A6) | 0x001B8FFA |
| 244 | 1:0x001239AD (linear 0x001339AD) | 0x001B9001 |
| 246 | 1:0x001239B4 (linear 0x001339B4) | 0x001B9008 |
| 259 | 1:0x001239D1 (linear 0x001339D1) | 0x001B9025 |
| 261 | 1:0x001239EE (linear 0x001339EE) | 0x001B9042 |
| 262 | 1:0x00123A45 (linear 0x00133A45) | 0x001B9099 |
| 264 | 1:0x00123A9C (linear 0x00133A9C) | 0x001B90F0 |
| 265 | 1:0x00123AC7 (linear 0x00133AC7) | 0x001B911B |
| 266 | 1:0x00123AF2 (linear 0x00133AF2) | 0x001B9146 |
| 268 | 1:0x00123B1D (linear 0x00133B1D) | 0x001B9171 |
| 270 | 1:0x00123B39 (linear 0x00133B39) | 0x001B918D |
| 272 | 1:0x00123B57 (linear 0x00133B57) | 0x001B91AB |
| 273 | 1:0x00123B5D (linear 0x00133B5D) | 0x001B91B1 |
| 276 | 1:0x00123B73 (linear 0x00133B73) | 0x001B91C7 |
| 277 | 1:0x00123B78 (linear 0x00133B78) | 0x001B91CC |
| 278 | 1:0x00123B7B (linear 0x00133B7B) | 0x001B91CF |
| 280 | 1:0x00123B81 (linear 0x00133B81) | 0x001B91D5 |
| 282 | 1:0x00123B89 (linear 0x00133B89) | 0x001B91DD |
| 284 | 1:0x00123B8F (linear 0x00133B8F) | 0x001B91E3 |
| 286 | 1:0x00123B98 (linear 0x00133B98) | 0x001B91EC |
| 287 | 1:0x00123B9D (linear 0x00133B9D) | 0x001B91F1 |
| 288 | 1:0x00123BA4 (linear 0x00133BA4) | 0x001B91F8 |
| 289 | 1:0x00123BAE (linear 0x00133BAE) | 0x001B9202 |
| 294 | 1:0x00123BCF (linear 0x00133BCF) | 0x001B9223 |
| 296 | 1:0x00123BD7 (linear 0x00133BD7) | 0x001B922B |
| 297 | 1:0x00123BEA (linear 0x00133BEA) | 0x001B923E |
| 299 | 1:0x00123BF4 (linear 0x00133BF4) | 0x001B9248 |
| 302 | 1:0x00123C13 (linear 0x00133C13) | 0x001B9267 |
| 303 | 1:0x00123C1E (linear 0x00133C1E) | 0x001B9272 |
| 305 | 1:0x00123C29 (linear 0x00133C29) | 0x001B927D |
| 307 | 1:0x00123C51 (linear 0x00133C51) | 0x001B92A5 |
| 308 | 1:0x00123C5A (linear 0x00133C5A) | 0x001B92AE |
| 310 | 1:0x00123C5E (linear 0x00133C5E) | 0x001B92B2 |
| 311 | 1:0x00123C6C (linear 0x00133C6C) | 0x001B92C0 |
| 312 | 1:0x00123C7A (linear 0x00133C7A) | 0x001B92CE |
| 314 | 1:0x00123C88 (linear 0x00133C88) | 0x001B92DC |
| 316 | 1:0x00123C8A (linear 0x00133C8A) | 0x001B92DE |
| 318 | 1:0x00123C94 (linear 0x00133C94) | 0x001B92E8 |
| 320 | 1:0x00123C9C (linear 0x00133C9C) | 0x001B92F0 |
| 322 | 1:0x00123CB7 (linear 0x00133CB7) | 0x001B930B |
| 324 | 1:0x00123CBF (linear 0x00133CBF) | 0x001B9313 |
| 325 | 1:0x00123CD2 (linear 0x00133CD2) | 0x001B9326 |
| 327 | 1:0x00123CDC (linear 0x00133CDC) | 0x001B9330 |
| 330 | 1:0x00123CFB (linear 0x00133CFB) | 0x001B934F |
| 331 | 1:0x00123D0D (linear 0x00133D0D) | 0x001B9361 |
| 333 | 1:0x00123D16 (linear 0x00133D16) | 0x001B936A |
| 338 | 1:0x00123D33 (linear 0x00133D33) | 0x001B9387 |
| 340 | 1:0x00123D38 (linear 0x00133D38) | 0x001B938C |
| 342 | 1:0x00123D5F (linear 0x00133D5F) | 0x001B93B3 |
| 344 | 1:0x00123D7F (linear 0x00133D7F) | 0x001B93D3 |
| 345 | 1:0x00123D88 (linear 0x00133D88) | 0x001B93DC |
| 346 | 1:0x00123D94 (linear 0x00133D94) | 0x001B93E8 |
| 347 | 1:0x00123DA0 (linear 0x00133DA0) | 0x001B93F4 |
| 349 | 1:0x00123DAC (linear 0x00133DAC) | 0x001B9400 |
| 350 | 1:0x00123DC4 (linear 0x00133DC4) | 0x001B9418 |
| 351 | 1:0x00123DD7 (linear 0x00133DD7) | 0x001B942B |
| 352 | 1:0x00123DD9 (linear 0x00133DD9) | 0x001B942D |
| 354 | 1:0x00123DE1 (linear 0x00133DE1) | 0x001B9435 |
| 356 | 1:0x00123DFC (linear 0x00133DFC) | 0x001B9450 |
| 357 | 1:0x00123E0A (linear 0x00133E0A) | 0x001B945E |
| 358 | 1:0x00123E16 (linear 0x00133E16) | 0x001B946A |
| 360 | 1:0x00123E37 (linear 0x00133E37) | 0x001B948B |
| 361 | 1:0x00123E4A (linear 0x00133E4A) | 0x001B949E |
| 362 | 1:0x00123E5D (linear 0x00133E5D) | 0x001B94B1 |
| 363 | 1:0x00123E70 (linear 0x00133E70) | 0x001B94C4 |

