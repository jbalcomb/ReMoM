# F:\WLIBS\palette\palette.c

Module 173 of the debug info of ORION2.EXE.

Ranges: 1:0x0012182A (linear 0x0013182A), 3,445 bytes, 2:0x0000C5EC (linear 0x0017C5EC), 4 bytes, 2:0x00044390 (linear 0x001B4390), 1,036 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0012182A (linear 0x0013182A) | code | Set_Palette_Colors_ | `regions/07-object-1/001B6E54-001B7653.txt` |
| 1:0x001218D4 (linear 0x001318D4) | code | Set_Palette_Changes_ | `regions/07-object-1/001B6E54-001B7653.txt` |
| 1:0x00121922 (linear 0x00131922) | code | Clear_Palette_Changes_ | `regions/07-object-1/001B6E54-001B7653.txt` |
| 1:0x00121970 (linear 0x00131970) | code | Save_Palette_ | `regions/07-object-1/001B6E54-001B7653.txt` |
| 1:0x001219A3 (linear 0x001319A3) | code | Restore_Palette_ | `regions/07-object-1/001B6E54-001B7653.txt` |
| 1:0x001219E4 (linear 0x001319E4) | code | Set_Palette_Gradient_ | `regions/07-object-1/001B6E54-001B7653.txt` |
| 1:0x00121ACB (linear 0x00131ACB) | code | Set_Glass_Remap_Color_ | `regions/07-object-1/001B6E54-001B7653.txt` |
| 1:0x00121B34 (linear 0x00131B34) | code | Reset_Cycle_Palette_Color_ | `regions/07-object-1/001B6E54-001B7653.txt` |
| 1:0x00121B5D (linear 0x00131B5D) | code | Cycle_Palette_Color_ | `regions/07-object-1/001B6E54-001B7653.txt` |
| 1:0x00121E6B (linear 0x00131E6B) | code | Update_Cycle_ | `regions/07-object-1/001B6E54-001B7653.txt` |
| 1:0x00121F7B (linear 0x00131F7B) | code | Refresh_Palette_ | `regions/07-object-1/001B6E54-001B7653.txt` |
| 1:0x00122065 (linear 0x00132065) | code | Allocate_Glass_Remap_Next_Block_ | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x0012209C (linear 0x0013209C) | code | Save_Glass_Remap_Block_ | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x001220E9 (linear 0x001320E9) | code | Restore_Glass_Remap_Block_ | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x00122137 (linear 0x00132137) | code | RGB_To_HLS_ | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x0012234E (linear 0x0013234E) | code | HLS_To_RGB_ | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x00122461 (linear 0x00132461) | code | Value_ | `regions/07-object-1/001B7654-001B7E53.txt` |
| 2:0x0000C5EC (linear 0x0017C5EC) | data | _cycle_direction_flag | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x00044390 (linear 0x001B4390) | data | _saved_palette | - |
| 2:0x00044790 (linear 0x001B4790) | data | _cycle_color_value | - |
| 2:0x00044794 (linear 0x001B4794) | data | _cycle_step_value | - |
| 2:0x00044798 (linear 0x001B4798) | data | _vsync_speed_flag | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x00044390 (linear 0x001B4390) | saved_palette | unsigned char[1024] |
| 2:0x00044790 (linear 0x001B4790) | cycle_color_value | int |
| 2:0x0000C5EC (linear 0x0017C5EC) | cycle_direction_flag | int |
| 2:0x00044794 (linear 0x001B4794) | cycle_step_value | int |
| 2:0x00044798 (linear 0x001B4798) | vsync_speed_flag | unsigned int |

#### Set_Palette_Colors

- Address: 1:0x0012182A (linear 0x0013182A), 170 bytes, in `regions/07-object-1/001B6E54-001B7653.txt`
- Type: void (short, short, char *)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| start_color | [EBP-0xC] | short |
| color_count | [EBP-0x10] | short |
| color_array | [EBP-0x14] | char * |
| i | [EBP-0x8] | short |
| color_pos | [EBP-0x4] | short |

#### Set_Palette_Changes

- Address: 1:0x001218D4 (linear 0x001318D4), 78 bytes, in `regions/07-object-1/001B6E54-001B7653.txt`
- Type: void (short, short)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| start_color | [EBP-0x8] | short |
| end_color | [EBP-0x4] | short |
| i | [EBP-0xC] | int |

#### Clear_Palette_Changes

- Address: 1:0x00121922 (linear 0x00131922), 78 bytes, in `regions/07-object-1/001B6E54-001B7653.txt`
- Type: void (short, short)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| start_color | [EBP-0x8] | short |
| end_color | [EBP-0x4] | short |
| i | [EBP-0xC] | int |

#### Save_Palette

- Address: 1:0x00121970 (linear 0x00131970), 51 bytes, in `regions/07-object-1/001B6E54-001B7653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Restore_Palette

- Address: 1:0x001219A3 (linear 0x001319A3), 65 bytes, in `regions/07-object-1/001B6E54-001B7653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x4] | int |

#### Set_Palette_Gradient

- Address: 1:0x001219E4 (linear 0x001319E4), 231 bytes, in `regions/07-object-1/001B6E54-001B7653.txt`
- Type: void (int, int, int, int, int, int, int, int)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| start_color | [EBP-0x28] | int |
| color_count | [EBP-0x24] | int |
| red_1 | [EBP-0x20] | int |
| green_1 | [EBP-0x1C] | int |
| blue_1 | [EBP+0x10] | int |
| red_2 | [EBP+0x14] | int |
| green_2 | [EBP+0x18] | int |
| blue_2 | [EBP+0x1C] | int |
| data_segment | [EBP-0x18] | int |
| i | [EBP-0x14] | int |
| red | [EBP-0x10] | int |
| green | [EBP-0xC] | int |
| blue | [EBP-0x8] | int |
| pos | [EBP-0x4] | int |

#### Set_Glass_Remap_Color

- Address: 1:0x00121ACB (linear 0x00131ACB), 105 bytes, in `regions/07-object-1/001B6E54-001B7653.txt`
- Type: void (short, short, short, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| remap_color | [EBP-0xC] | short |
| red | [EBP-0x10] | short |
| green | [EBP-0x4] | short |
| blue | [EBP-0x8] | short |
| intensity | [EBP+0x10] | short |

#### Reset_Cycle_Palette_Color

- Address: 1:0x00121B34 (linear 0x00131B34), 41 bytes, in `regions/07-object-1/001B6E54-001B7653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Cycle_Palette_Color

- Address: 1:0x00121B5D (linear 0x00131B5D), 782 bytes, in `regions/07-object-1/001B6E54-001B7653.txt`
- Type: void (short, short, short, short, short, short, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| color_num | [EBP-0x4] | short |
| red_min | [EBP-0x10] | short |
| green_min | [EBP-0x8] | short |
| blue_min | [EBP-0xC] | short |
| red_max | [EBP+0x10] | short |
| green_max | [EBP+0x14] | short |
| blue_max | [EBP+0x18] | short |
| step_value | [EBP+0x1C] | short |
| delta_red | [EBP-0x28] | int |
| delta_green | [EBP-0x24] | int |
| delta_blue | [EBP-0x14] | int |
| primary_color | [EBP-0x2C] | int |
| store_red | [EBP-0x20] | int |
| store_blue | [EBP-0x1C] | int |
| store_green | [EBP-0x18] | int |

#### Update_Cycle

- Address: 1:0x00121E6B (linear 0x00131E6B), 272 bytes, in `regions/07-object-1/001B6E54-001B7653.txt`
- Type: void (short *, short *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| color_min | [EBP-0x8] | short * |
| color_max | [EBP-0x4] | short * |

#### Refresh_Palette

- Address: 1:0x00121F7B (linear 0x00131F7B), 234 bytes, in `regions/07-object-1/001B6E54-001B7653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x1C] | int |
| pal_num | [EBP-0x18] | int |
| start_color | [EBP-0x14] | int |
| color_count | [EBP-0x10] | int |
| pal_pos | [EBP-0xC] | int |
| already_stored_count | [EBP-0x8] | int |
| pass | [EBP-0x4] | int |

#### Allocate_Glass_Remap_Next_Block

- Address: 1:0x00122065 (linear 0x00132065), 55 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: char * (char *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0x8] | char * |

#### Save_Glass_Remap_Block

- Address: 1:0x0012209C (linear 0x0013209C), 77 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0x4] | char * |

#### Restore_Glass_Remap_Block

- Address: 1:0x001220E9 (linear 0x001320E9), 78 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| block | [EBP-0x4] | char * |

#### RGB_To_HLS

- Address: 1:0x00122137 (linear 0x00132137), 535 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: void (char, char, char, int *, int *, int *)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| red | [EBP-0xC] | char |
| green | [EBP-0x8] | char |
| blue | [EBP-0x4] | char |
| hue | [EBP-0x20] | int * |
| lum | [EBP+0x10] | int * |
| sat | [EBP+0x14] | int * |
| max_value | [EBP-0x28] | int |
| min_value | [EBP-0x24] | int |
| delta | [EBP-0x10] | int |
| fred | [EBP-0x1C] | int |
| fgreen | [EBP-0x18] | int |
| fblue | [EBP-0x14] | int |

#### HLS_To_RGB

- Address: 1:0x0012234E (linear 0x0013234E), 275 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: void (int, int, int, char *, char *, char *)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| hue | [EBP-0x18] | int |
| lum | [EBP-0x14] | int |
| sat | [EBP-0x10] | int |
| red | [EBP-0xC] | char * |
| green | [EBP+0x10] | char * |
| blue | [EBP+0x14] | char * |
| m2 | [EBP-0x8] | int |
| m1 | [EBP-0x4] | int |

#### Value

- Address: 1:0x00122461 (linear 0x00132461), 318 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: char (int, int, int); returns in AL
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| n1 | [EBP-0x1C] | int |
| n2 | [EBP-0x18] | int |
| hue | [EBP-0x14] | int |
| return_value | [EBP-0x8] | char |
| x | [EBP-0xC] | int |
| y | [EBP-0x10] | int |

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
| 30 | typedef | BYTE | name for unsigned char (#5) |
| 31 | typedef | SBYTE | name for signed char (#4) |
| 32 | typedef | UBYTE | name for unsigned char (#5) |
| 33 | typedef | WORD | name for unsigned short (#7) |
| 34 | typedef | SWORD | name for short (#6) |
| 35 | typedef | UWORD | name for unsigned short (#7) |
| 36 | typedef | DWORD | name for unsigned int (#9) |
| 37 | typedef | SDWORD | name for int (#8) |
| 38 | typedef | UDWORD | name for unsigned int (#9) |
| 39 | typedef | BOOL | name for unsigned char (#5) |
| 40 | typedef | SampleHandle | name for int (#8) |
| 41 | array |  | 0..1023; of unsigned char (#5) |
| 42 | procedure |  | near386; returns void (short, short, char *) |
| 43 | procedure |  | near386; returns void (short, short) |
| 44 | procedure |  | near386; returns void (int, int, int, int, int, int, int, int) |
| 45 | procedure |  | near386; returns void (short, short, short, short, short) |
| 46 | procedure |  | near386; returns void (short, short, short, short, short, short, short, short) |
| 47 | pointer |  | near386; of short (#6) |
| 48 | procedure |  | near386; returns void (short *, short *) |
| 49 | procedure |  | near386; returns void (void) |
| 50 | procedure |  | near386; returns char * (char *) |
| 51 | procedure |  | near386; returns void (char *) |
| 52 | pointer |  | near386; of int (#8) |
| 53 | procedure |  | near386; returns void (char, char, char, int *, int *, int *) |
| 54 | procedure |  | near386; returns void (int, int, int, char *, char *, char *) |
| 55 | procedure |  | near386; returns char (int, int, int) |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 36 | 1:0x0012182A (linear 0x0013182A) | 0x001B6E7E |
| 40 | 1:0x00121849 (linear 0x00131849) | 0x001B6E9D |
| 41 | 1:0x00121850 (linear 0x00131850) | 0x001B6EA4 |
| 43 | 1:0x00121872 (linear 0x00131872) | 0x001B6EC6 |
| 44 | 1:0x00121880 (linear 0x00131880) | 0x001B6ED4 |
| 45 | 1:0x00121896 (linear 0x00131896) | 0x001B6EEA |
| 46 | 1:0x00121899 (linear 0x00131899) | 0x001B6EED |
| 47 | 1:0x001218AF (linear 0x001318AF) | 0x001B6F03 |
| 48 | 1:0x001218B2 (linear 0x001318B2) | 0x001B6F06 |
| 49 | 1:0x001218C8 (linear 0x001318C8) | 0x001B6F1C |
| 50 | 1:0x001218CB (linear 0x001318CB) | 0x001B6F1F |
| 51 | 1:0x001218CD (linear 0x001318CD) | 0x001B6F21 |
| 57 | 1:0x001218D4 (linear 0x001318D4) | 0x001B6F28 |
| 61 | 1:0x001218F1 (linear 0x001318F1) | 0x001B6F45 |
| 62 | 1:0x0012190B (linear 0x0013190B) | 0x001B6F5F |
| 63 | 1:0x0012191A (linear 0x0013191A) | 0x001B6F6E |
| 69 | 1:0x00121922 (linear 0x00131922) | 0x001B6F76 |
| 73 | 1:0x0012193F (linear 0x0013193F) | 0x001B6F93 |
| 74 | 1:0x00121959 (linear 0x00131959) | 0x001B6FAD |
| 75 | 1:0x00121968 (linear 0x00131968) | 0x001B6FBC |
| 83 | 1:0x00121970 (linear 0x00131970) | 0x001B6FC4 |
| 85 | 1:0x00121988 (linear 0x00131988) | 0x001B6FDC |
| 86 | 1:0x0012199C (linear 0x0013199C) | 0x001B6FF0 |
| 93 | 1:0x001219A3 (linear 0x001319A3) | 0x001B6FF7 |
| 96 | 1:0x001219BB (linear 0x001319BB) | 0x001B700F |
| 97 | 1:0x001219CF (linear 0x001319CF) | 0x001B7023 |
| 98 | 1:0x001219DB (linear 0x001319DB) | 0x001B702F |
| 106 | 1:0x001219E4 (linear 0x001319E4) | 0x001B7038 |
| 111 | 1:0x00121A05 (linear 0x00131A05) | 0x001B7059 |
| 113 | 1:0x00121A21 (linear 0x00131A21) | 0x001B7075 |
| 114 | 1:0x00121A3E (linear 0x00131A3E) | 0x001B7092 |
| 115 | 1:0x00121A5B (linear 0x00131A5B) | 0x001B70AF |
| 117 | 1:0x00121A78 (linear 0x00131A78) | 0x001B70CC |
| 118 | 1:0x00121A88 (linear 0x00131A88) | 0x001B70DC |
| 119 | 1:0x00121A9A (linear 0x00131A9A) | 0x001B70EE |
| 120 | 1:0x00121AAC (linear 0x00131AAC) | 0x001B7100 |
| 121 | 1:0x00121ABE (linear 0x00131ABE) | 0x001B7112 |
| 122 | 1:0x00121AC3 (linear 0x00131AC3) | 0x001B7117 |
| 130 | 1:0x00121ACB (linear 0x00131ACB) | 0x001B711F |
| 133 | 1:0x00121AEC (linear 0x00131AEC) | 0x001B7140 |
| 134 | 1:0x00121AFC (linear 0x00131AFC) | 0x001B7150 |
| 135 | 1:0x00121B0C (linear 0x00131B0C) | 0x001B7160 |
| 136 | 1:0x00121B1C (linear 0x00131B1C) | 0x001B7170 |
| 137 | 1:0x00121B2C (linear 0x00131B2C) | 0x001B7180 |
| 151 | 1:0x00121B34 (linear 0x00131B34) | 0x001B7188 |
| 157 | 1:0x00121B5D (linear 0x00131B5D) | 0x001B71B1 |
| 164 | 1:0x00121B7E (linear 0x00131B7E) | 0x001B71D2 |
| 165 | 1:0x00121B8B (linear 0x00131B8B) | 0x001B71DF |
| 166 | 1:0x00121B98 (linear 0x00131B98) | 0x001B71EC |
| 167 | 1:0x00121BA5 (linear 0x00131BA5) | 0x001B71F9 |
| 168 | 1:0x00121BAE (linear 0x00131BAE) | 0x001B7202 |
| 169 | 1:0x00121BB7 (linear 0x00131BB7) | 0x001B720B |
| 171 | 1:0x00121BC0 (linear 0x00131BC0) | 0x001B7214 |
| 173 | 1:0x00121BC9 (linear 0x00131BC9) | 0x001B721D |
| 175 | 1:0x00121BD0 (linear 0x00131BD0) | 0x001B7224 |
| 176 | 1:0x00121BE9 (linear 0x00131BE9) | 0x001B723D |
| 177 | 1:0x00121BEB (linear 0x00131BEB) | 0x001B723F |
| 179 | 1:0x00121C04 (linear 0x00131C04) | 0x001B7258 |
| 181 | 1:0x00121C0D (linear 0x00131C0D) | 0x001B7261 |
| 182 | 1:0x00121C17 (linear 0x00131C17) | 0x001B726B |
| 184 | 1:0x00121C31 (linear 0x00131C31) | 0x001B7285 |
| 185 | 1:0x00121C3C (linear 0x00131C3C) | 0x001B7290 |
| 186 | 1:0x00121C47 (linear 0x00131C47) | 0x001B729B |
| 190 | 1:0x00121C50 (linear 0x00131C50) | 0x001B72A4 |
| 192 | 1:0x00121C79 (linear 0x00131C79) | 0x001B72CD |
| 193 | 1:0x00121C81 (linear 0x00131C81) | 0x001B72D5 |
| 195 | 1:0x00121CB6 (linear 0x00131CB6) | 0x001B730A |
| 197 | 1:0x00121CED (linear 0x00131CED) | 0x001B7341 |
| 198 | 1:0x00121CF2 (linear 0x00131CF2) | 0x001B7346 |
| 199 | 1:0x00121CFA (linear 0x00131CFA) | 0x001B734E |
| 201 | 1:0x00121D2F (linear 0x00131D2F) | 0x001B7383 |
| 203 | 1:0x00121D66 (linear 0x00131D66) | 0x001B73BA |
| 204 | 1:0x00121D68 (linear 0x00131D68) | 0x001B73BC |
| 205 | 1:0x00121D70 (linear 0x00131D70) | 0x001B73C4 |
| 207 | 1:0x00121DA5 (linear 0x00131DA5) | 0x001B73F9 |
| 212 | 1:0x00121DDC (linear 0x00131DDC) | 0x001B7430 |
| 214 | 1:0x00121DF6 (linear 0x00131DF6) | 0x001B744A |
| 215 | 1:0x00121E03 (linear 0x00131E03) | 0x001B7457 |
| 216 | 1:0x00121E10 (linear 0x00131E10) | 0x001B7464 |
| 219 | 1:0x00121E1B (linear 0x00131E1B) | 0x001B746F |
| 220 | 1:0x00121E33 (linear 0x00131E33) | 0x001B7487 |
| 221 | 1:0x00121E43 (linear 0x00131E43) | 0x001B7497 |
| 222 | 1:0x00121E53 (linear 0x00131E53) | 0x001B74A7 |
| 223 | 1:0x00121E63 (linear 0x00131E63) | 0x001B74B7 |
| 225 | 1:0x00121E6B (linear 0x00131E6B) | 0x001B74BF |
| 227 | 1:0x00121E88 (linear 0x00131E88) | 0x001B74DC |
| 229 | 1:0x00121E95 (linear 0x00131E95) | 0x001B74E9 |
| 231 | 1:0x00121EA3 (linear 0x00131EA3) | 0x001B74F7 |
| 232 | 1:0x00121EAE (linear 0x00131EAE) | 0x001B7502 |
| 234 | 1:0x00121EBC (linear 0x00131EBC) | 0x001B7510 |
| 235 | 1:0x00121EC8 (linear 0x00131EC8) | 0x001B751C |
| 238 | 1:0x00121ED2 (linear 0x00131ED2) | 0x001B7526 |
| 240 | 1:0x00121ED4 (linear 0x00131ED4) | 0x001B7528 |
| 241 | 1:0x00121EDF (linear 0x00131EDF) | 0x001B7533 |
| 243 | 1:0x00121EED (linear 0x00131EED) | 0x001B7541 |
| 244 | 1:0x00121EF9 (linear 0x00131EF9) | 0x001B754D |
| 248 | 1:0x00121F03 (linear 0x00131F03) | 0x001B7557 |
| 250 | 1:0x00121F05 (linear 0x00131F05) | 0x001B7559 |
| 252 | 1:0x00121F13 (linear 0x00131F13) | 0x001B7567 |
| 253 | 1:0x00121F1E (linear 0x00131F1E) | 0x001B7572 |
| 255 | 1:0x00121F2C (linear 0x00131F2C) | 0x001B7580 |
| 256 | 1:0x00121F38 (linear 0x00131F38) | 0x001B758C |
| 259 | 1:0x00121F42 (linear 0x00131F42) | 0x001B7596 |
| 261 | 1:0x00121F44 (linear 0x00131F44) | 0x001B7598 |
| 262 | 1:0x00121F4F (linear 0x00131F4F) | 0x001B75A3 |
| 264 | 1:0x00121F5D (linear 0x00131F5D) | 0x001B75B1 |
| 265 | 1:0x00121F69 (linear 0x00131F69) | 0x001B75BD |
| 269 | 1:0x00121F73 (linear 0x00131F73) | 0x001B75C7 |
| 273 | 1:0x00121F7B (linear 0x00131F7B) | 0x001B75CF |
| 278 | 1:0x00121F93 (linear 0x00131F93) | 0x001B75E7 |
| 279 | 1:0x00121F9A (linear 0x00131F9A) | 0x001B75EE |
| 281 | 1:0x00121FA1 (linear 0x00131FA1) | 0x001B75F5 |
| 283 | 1:0x00121FAE (linear 0x00131FAE) | 0x001B7602 |
| 285 | 1:0x00121FCD (linear 0x00131FCD) | 0x001B7621 |
| 287 | 1:0x00121FDA (linear 0x00131FDA) | 0x001B762E |
| 288 | 1:0x00121FE0 (linear 0x00131FE0) | 0x001B7634 |
| 289 | 1:0x00121FE7 (linear 0x00131FE7) | 0x001B763B |
| 291 | 1:0x00121FEE (linear 0x00131FEE) | 0x001B7642 |
| 293 | 1:0x00122008 (linear 0x00132008) | 0x001B765C |
| 294 | 1:0x0012200B (linear 0x0013200B) | 0x001B765F |
| 295 | 1:0x0012200E (linear 0x0013200E) | 0x001B7662 |
| 297 | 1:0x00122010 (linear 0x00132010) | 0x001B7664 |
| 299 | 1:0x0012201D (linear 0x0013201D) | 0x001B7671 |
| 302 | 1:0x00122028 (linear 0x00132028) | 0x001B767C |
| 303 | 1:0x0012202D (linear 0x0013202D) | 0x001B7681 |
| 305 | 1:0x00122045 (linear 0x00132045) | 0x001B7699 |
| 307 | 1:0x0012204B (linear 0x0013204B) | 0x001B769F |
| 309 | 1:0x00122050 (linear 0x00132050) | 0x001B76A4 |
| 310 | 1:0x0012205C (linear 0x0013205C) | 0x001B76B0 |
| 314 | 1:0x00122065 (linear 0x00132065) | 0x001B76B9 |
| 316 | 1:0x00122080 (linear 0x00132080) | 0x001B76D4 |
| 317 | 1:0x00122090 (linear 0x00132090) | 0x001B76E4 |
| 319 | 1:0x0012209C (linear 0x0013209C) | 0x001B76F0 |
| 321 | 1:0x001220B7 (linear 0x001320B7) | 0x001B770B |
| 322 | 1:0x001220C9 (linear 0x001320C9) | 0x001B771D |
| 324 | 1:0x001220E0 (linear 0x001320E0) | 0x001B7734 |
| 326 | 1:0x001220E9 (linear 0x001320E9) | 0x001B773D |
| 328 | 1:0x00122104 (linear 0x00132104) | 0x001B7758 |
| 329 | 1:0x00122116 (linear 0x00132116) | 0x001B776A |
| 330 | 1:0x0012212E (linear 0x0013212E) | 0x001B7782 |
| 492 | 1:0x00122137 (linear 0x00132137) | 0x001B778B |
| 497 | 1:0x00122158 (linear 0x00132158) | 0x001B77AC |
| 498 | 1:0x00122172 (linear 0x00132172) | 0x001B77C6 |
| 499 | 1:0x0012218C (linear 0x0013218C) | 0x001B77E0 |
| 501 | 1:0x001221A6 (linear 0x001321A6) | 0x001B77FA |
| 502 | 1:0x001221C2 (linear 0x001321C2) | 0x001B7816 |
| 504 | 1:0x001221DE (linear 0x001321DE) | 0x001B7832 |
| 505 | 1:0x001221FA (linear 0x001321FA) | 0x001B784E |
| 508 | 1:0x00122216 (linear 0x00132216) | 0x001B786A |
| 510 | 1:0x0012222A (linear 0x0013222A) | 0x001B787E |
| 512 | 1:0x00122232 (linear 0x00132232) | 0x001B7886 |
| 513 | 1:0x0012223B (linear 0x0013223B) | 0x001B788F |
| 514 | 1:0x00122244 (linear 0x00132244) | 0x001B7898 |
| 518 | 1:0x00122249 (linear 0x00132249) | 0x001B789D |
| 520 | 1:0x00122254 (linear 0x00132254) | 0x001B78A8 |
| 522 | 1:0x00122272 (linear 0x00132272) | 0x001B78C6 |
| 524 | 1:0x00122274 (linear 0x00132274) | 0x001B78C8 |
| 529 | 1:0x00122297 (linear 0x00132297) | 0x001B78EB |
| 531 | 1:0x001222A0 (linear 0x001322A0) | 0x001B78F4 |
| 533 | 1:0x001222A8 (linear 0x001322A8) | 0x001B78FC |
| 535 | 1:0x001222C1 (linear 0x001322C1) | 0x001B7915 |
| 537 | 1:0x001222CB (linear 0x001322CB) | 0x001B791F |
| 539 | 1:0x001222E9 (linear 0x001322E9) | 0x001B793D |
| 541 | 1:0x001222F3 (linear 0x001322F3) | 0x001B7947 |
| 544 | 1:0x00122311 (linear 0x00132311) | 0x001B7965 |
| 546 | 1:0x0012231C (linear 0x0013231C) | 0x001B7970 |
| 548 | 1:0x00122324 (linear 0x00132324) | 0x001B7978 |
| 550 | 1:0x0012232D (linear 0x0013232D) | 0x001B7981 |
| 551 | 1:0x00122346 (linear 0x00132346) | 0x001B799A |
| 555 | 1:0x0012234E (linear 0x0013234E) | 0x001B79A2 |
| 560 | 1:0x0012236F (linear 0x0013236F) | 0x001B79C3 |
| 562 | 1:0x00122378 (linear 0x00132378) | 0x001B79CC |
| 564 | 1:0x00122394 (linear 0x00132394) | 0x001B79E8 |
| 566 | 1:0x00122396 (linear 0x00132396) | 0x001B79EA |
| 568 | 1:0x001223B4 (linear 0x001323B4) | 0x001B7A08 |
| 570 | 1:0x001223BF (linear 0x001323BF) | 0x001B7A13 |
| 572 | 1:0x001223C5 (linear 0x001323C5) | 0x001B7A19 |
| 574 | 1:0x001223CB (linear 0x001323CB) | 0x001B7A1F |
| 576 | 1:0x001223F4 (linear 0x001323F4) | 0x001B7A48 |
| 578 | 1:0x001223F6 (linear 0x001323F6) | 0x001B7A4A |
| 579 | 1:0x00122410 (linear 0x00132410) | 0x001B7A64 |
| 582 | 1:0x00122412 (linear 0x00132412) | 0x001B7A66 |
| 584 | 1:0x00122414 (linear 0x00132414) | 0x001B7A68 |
| 585 | 1:0x0012242C (linear 0x0013242C) | 0x001B7A80 |
| 586 | 1:0x00122441 (linear 0x00132441) | 0x001B7A95 |
| 590 | 1:0x00122459 (linear 0x00132459) | 0x001B7AAD |
| 591 | 1:0x00122461 (linear 0x00132461) | 0x001B7AB5 |
| 595 | 1:0x00122480 (linear 0x00132480) | 0x001B7AD4 |
| 597 | 1:0x00122489 (linear 0x00132489) | 0x001B7ADD |
| 599 | 1:0x00122490 (linear 0x00132490) | 0x001B7AE4 |
| 601 | 1:0x00122498 (linear 0x00132498) | 0x001B7AEC |
| 604 | 1:0x0012249F (linear 0x0013249F) | 0x001B7AF3 |
| 606 | 1:0x001224A5 (linear 0x001324A5) | 0x001B7AF9 |
| 607 | 1:0x001224BB (linear 0x001324BB) | 0x001B7B0F |
| 608 | 1:0x001224CA (linear 0x001324CA) | 0x001B7B1E |
| 609 | 1:0x001224DD (linear 0x001324DD) | 0x001B7B31 |
| 611 | 1:0x001224E3 (linear 0x001324E3) | 0x001B7B37 |
| 613 | 1:0x001224F6 (linear 0x001324F6) | 0x001B7B4A |
| 615 | 1:0x00122504 (linear 0x00132504) | 0x001B7B58 |
| 617 | 1:0x00122517 (linear 0x00132517) | 0x001B7B6B |
| 619 | 1:0x00122522 (linear 0x00132522) | 0x001B7B76 |
| 620 | 1:0x0012253F (linear 0x0013253F) | 0x001B7B93 |
| 621 | 1:0x0012254E (linear 0x0013254E) | 0x001B7BA2 |
| 622 | 1:0x00122561 (linear 0x00132561) | 0x001B7BB5 |
| 624 | 1:0x00122567 (linear 0x00132567) | 0x001B7BBB |
| 626 | 1:0x0012257A (linear 0x0013257A) | 0x001B7BCE |
| 628 | 1:0x0012257C (linear 0x0013257C) | 0x001B7BD0 |
| 632 | 1:0x0012258F (linear 0x0013258F) | 0x001B7BE3 |
| 633 | 1:0x00122595 (linear 0x00132595) | 0x001B7BE9 |

