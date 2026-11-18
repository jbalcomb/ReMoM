# F:\WLIBS\graphics\graphics.c

Module 159 of the debug info of ORION2.EXE.

Ranges: 1:0x00118AB6 (linear 0x00128AB6), 4,538 bytes, 2:0x0000C584 (linear 0x0017C584), 16 bytes, 2:0x00044294 (linear 0x001B4294), 8 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x00118AB6 (linear 0x00128AB6) | code | Set_Window_ | `regions/07-object-1/001ADE54-001AE653.txt` |
| 1:0x00118B49 (linear 0x00128B49) | code | Save_Window_ | `regions/07-object-1/001ADE54-001AE653.txt` |
| 1:0x00118B98 (linear 0x00128B98) | code | Restore_Window_ | `regions/07-object-1/001ADE54-001AE653.txt` |
| 1:0x00118BE7 (linear 0x00128BE7) | code | Reset_Window_ | `regions/07-object-1/001ADE54-001AE653.txt` |
| 1:0x00118C32 (linear 0x00128C32) | code | Fill_ | `regions/07-object-1/001ADE54-001AE653.txt` |
| 1:0x00118EDE (linear 0x00128EDE) | code | Interlaced_Fill_ | `regions/07-object-1/001ADE54-001AE653.txt` |
| 1:0x00119130 (linear 0x00129130) | code | Tri_Interlaced_Fill_ | `regions/07-object-1/001AE654-001AEE53.txt` |
| 1:0x00119399 (linear 0x00129399) | code | Gray_Scale_Fill_ | `regions/07-object-1/001AE654-001AEE53.txt` |
| 1:0x00119514 (linear 0x00129514) | code | Interlaced_Gray_Scale_Fill_ | `regions/07-object-1/001AE654-001AEE53.txt` |
| 1:0x0011972D (linear 0x0012972D) | code | Dot_ | `regions/07-object-1/001AE654-001AEE53.txt` |
| 1:0x001197C3 (linear 0x001297C3) | code | Box_ | `regions/07-object-1/001AE654-001AEE53.txt` |
| 1:0x001198DE (linear 0x001298DE) | code | Darken_Fill_ | `regions/07-object-1/001AEE54-001AF653.txt` |
| 1:0x0011991D (linear 0x0012991D) | code | Tint_Fill_ | `regions/07-object-1/001AEE54-001AF653.txt` |
| 1:0x00119B84 (linear 0x00129B84) | code | Gradient_Fill_ | `regions/07-object-1/001AEE54-001AF653.txt` |
| 2:0x0000C584 (linear 0x0017C584) | data | _fill_mask_data | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x00044294 (linear 0x001B4294) | data | _old_screen_window_x1 | - |
| 2:0x00044296 (linear 0x001B4296) | data | _old_screen_window_x2 | - |
| 2:0x00044298 (linear 0x001B4298) | data | _old_screen_window_y1 | - |
| 2:0x0004429A (linear 0x001B429A) | data | _old_screen_window_y2 | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x0000C584 (linear 0x0017C584) | fill_mask_data | unsigned int[4] |
| 2:0x00044294 (linear 0x001B4294) | old_screen_window_x1 | short |
| 2:0x00044296 (linear 0x001B4296) | old_screen_window_x2 | short |
| 2:0x00044298 (linear 0x001B4298) | old_screen_window_y1 | short |
| 2:0x0004429A (linear 0x001B429A) | old_screen_window_y2 | short |

#### Set_Window

- Address: 1:0x00118AB6 (linear 0x00128AB6), 147 bytes, in `regions/07-object-1/001ADE54-001AE653.txt`
- Type: void (short, short, short, short)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x10] | short |
| y1 | [EBP-0xC] | short |
| x2 | [EBP-0x8] | short |
| y2 | [EBP-0x4] | short |

#### Save_Window

- Address: 1:0x00118B49 (linear 0x00128B49), 79 bytes, in `regions/07-object-1/001ADE54-001AE653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Restore_Window

- Address: 1:0x00118B98 (linear 0x00128B98), 79 bytes, in `regions/07-object-1/001ADE54-001AE653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Reset_Window

- Address: 1:0x00118BE7 (linear 0x00128BE7), 75 bytes, in `regions/07-object-1/001ADE54-001AE653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Fill

- Address: 1:0x00118C32 (linear 0x00128C32), 684 bytes, in `regions/07-object-1/001ADE54-001AE653.txt`
- Type: void (short, short, short, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0xC] | short |
| y1 | [EBP-0x10] | short |
| x2 | [EBP-0x4] | short |
| y2 | [EBP-0x8] | short |
| color | [EBP+0x10] | short |
| y | [EBP-0x4C] | int |
| pos | [EBP-0x48] | int |
| line_add | [EBP-0x44] | int |
| center_columns | [EBP-0x40] | int |
| start_x | [EBP-0x3C] | int |
| end_x | [EBP-0x38] | int |
| x | [EBP-0x34] | int |
| long_color | [EBP-0x60] | unsigned int |
| start_mask | [EBP-0x5C] | unsigned int |
| end_mask | [EBP-0x58] | unsigned int |
| start_color | [EBP-0x54] | unsigned int |
| end_color | [EBP-0x50] | unsigned int |
| column_mask | [EBP-0x1C] | unsigned int |
| value | [EBP-0x18] | unsigned int |
| long_current_video_page | [EBP-0x30] | unsigned int * |
| start_width | [EBP-0x2C] | int |
| center_width | [EBP-0x28] | int |
| end_width | [EBP-0x24] | int |
| line_start | [EBP-0x20] | int |
| column_height | [EBP-0x14] | int |

#### Interlaced_Fill

- Address: 1:0x00118EDE (linear 0x00128EDE), 594 bytes, in `regions/07-object-1/001ADE54-001AE653.txt`
- Type: void (short, short, short, short, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x4] | short |
| y1 | [EBP-0x8] | short |
| x2 | [EBP-0x10] | short |
| y2 | [EBP-0xC] | short |
| color1 | [EBP+0x10] | short |
| color2 | [EBP+0x14] | short |
| y | [EBP-0x4C] | int |
| pos | [EBP-0x48] | int |
| line_add | [EBP-0x44] | int |
| center_columns | [EBP-0x40] | int |
| start_x | [EBP-0x3C] | int |
| end_x | [EBP-0x38] | int |
| x | [EBP-0x64] | int |
| long_color | [EBP-0x60] | unsigned int |
| start_mask | [EBP-0x5C] | unsigned int |
| end_mask | [EBP-0x58] | unsigned int |
| start_color | [EBP-0x54] | unsigned int |
| end_color | [EBP-0x20] | unsigned int |
| column_mask | [EBP-0x50] | unsigned int |
| value | [EBP-0x18] | unsigned int |
| long_current_video_page | [EBP-0x14] | unsigned int * |
| start_width | [EBP-0x30] | int |
| center_width | [EBP-0x2C] | int |
| end_width | [EBP-0x28] | int |
| line_start | [EBP-0x24] | int |
| column_height | [EBP-0x1C] | int |
| combined_color | [EBP-0x34] | unsigned int |

#### Tri_Interlaced_Fill

- Address: 1:0x00119130 (linear 0x00129130), 617 bytes, in `regions/07-object-1/001AE654-001AEE53.txt`
- Type: void (short, short, short, short, short, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x4] | short |
| y1 | [EBP-0x8] | short |
| x2 | [EBP-0x10] | short |
| y2 | [EBP-0xC] | short |
| color1 | [EBP+0x10] | short |
| color2 | [EBP+0x14] | short |
| color3 | [EBP+0x18] | short |
| y | [EBP-0x48] | int |
| pos | [EBP-0x44] | int |
| line_add | [EBP-0x40] | int |
| center_columns | [EBP-0x3C] | int |
| start_x | [EBP-0x38] | int |
| end_x | [EBP-0x64] | int |
| x | [EBP-0x60] | int |
| long_color | [EBP-0x5C] | unsigned int |
| start_mask | [EBP-0x58] | unsigned int |
| end_mask | [EBP-0x54] | unsigned int |
| start_color | [EBP-0x20] | unsigned int |
| end_color | [EBP-0x50] | unsigned int |
| column_mask | [EBP-0x18] | unsigned int |
| value | [EBP-0x4C] | unsigned int |
| long_current_video_page | [EBP-0x30] | unsigned int * |
| start_width | [EBP-0x2C] | int |
| center_width | [EBP-0x28] | int |
| end_width | [EBP-0x24] | int |
| line_start | [EBP-0x14] | int |
| column_height | [EBP-0x1C] | int |
| combined_color | [EBP-0x34] | unsigned int |

#### Gray_Scale_Fill

- Address: 1:0x00119399 (linear 0x00129399), 379 bytes, in `regions/07-object-1/001AE654-001AEE53.txt`
- Type: void (short, short, short, short, char *, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x4] | short |
| y1 | [EBP-0x8] | short |
| x2 | [EBP-0x10] | short |
| y2 | [EBP-0xC] | short |
| color_array | [EBP+0x10] | char * |
| color_count | [EBP+0x14] | short |
| inverse_flag | [EBP+0x18] | short |
| y | [EBP-0x4C] | int |
| pos | [EBP-0x48] | int |
| line_add | [EBP-0x44] | int |
| center_columns | [EBP-0x40] | int |
| start_x | [EBP-0x3C] | int |
| end_x | [EBP-0x64] | int |
| x | [EBP-0x60] | int |
| i | [EBP-0x5C] | int |
| long_color | [EBP-0x58] | unsigned int |
| start_mask | [EBP-0x28] | unsigned int |
| end_mask | [EBP-0x24] | unsigned int |
| start_color | [EBP-0x54] | unsigned int |
| end_color | [EBP-0x1C] | unsigned int |
| column_mask | [EBP-0x50] | unsigned int |
| value | [EBP-0x34] | unsigned int |
| long_current_video_page | [EBP-0x30] | unsigned int * |
| start_width | [EBP-0x2C] | int |
| center_width | [EBP-0x18] | int |
| end_width | [EBP-0x14] | int |
| line_start | [EBP-0x20] | int |
| column_height | [EBP-0x38] | int |

#### Interlaced_Gray_Scale_Fill

- Address: 1:0x00119514 (linear 0x00129514), 537 bytes, in `regions/07-object-1/001AE654-001AEE53.txt`
- Type: void (short, short, short, short, char *, char *, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x4] | short |
| y1 | [EBP-0xC] | short |
| x2 | [EBP-0x8] | short |
| y2 | [EBP-0x10] | short |
| color_array1 | [EBP+0x10] | char * |
| color_array2 | [EBP+0x14] | char * |
| color_count | [EBP+0x18] | short |
| inverse_flag | [EBP+0x1C] | short |
| y | [EBP-0x64] | int |
| pos | [EBP-0x60] | int |
| line_add | [EBP-0x5C] | int |
| center_columns | [EBP-0x58] | int |
| start_x | [EBP-0x54] | int |
| end_x | [EBP-0x50] | int |
| x | [EBP-0x4C] | int |
| i | [EBP-0x7C] | int |
| long_color | [EBP-0x78] | unsigned int |
| start_mask | [EBP-0x74] | unsigned int |
| end_mask | [EBP-0x70] | unsigned int |
| start_color | [EBP-0x38] | unsigned int |
| end_color | [EBP-0x34] | unsigned int |
| column_mask | [EBP-0x30] | unsigned int |
| value | [EBP-0x2C] | unsigned int |
| long_current_video_page | [EBP-0x6C] | unsigned int * |
| start_width | [EBP-0x24] | int |
| center_width | [EBP-0x68] | int |
| end_width | [EBP-0x44] | int |
| line_start | [EBP-0x40] | int |
| column_height | [EBP-0x3C] | int |
| frame_offset | [EBP-0x14] | char * |
| frame_offset_table | [EBP-0x20] | long * |
| source_start | [EBP-0x1C] | char * |
| target_start | [EBP-0x18] | char * |
| width | [EBP-0x28] | int |
| height | [EBP-0x48] | int |

#### Dot

- Address: 1:0x0011972D (linear 0x0012972D), 150 bytes, in `regions/07-object-1/001AE654-001AEE53.txt`
- Type: void (short, short, short)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| x | [EBP-0xC] | short |
| y | [EBP-0x8] | short |
| color | [EBP-0x4] | short |

#### Box

- Address: 1:0x001197C3 (linear 0x001297C3), 283 bytes, in `regions/07-object-1/001AE654-001AEE53.txt`
- Type: void (short, short, short, short, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x | [EBP-0xC] | short |
| y | [EBP-0x8] | short |
| width | [EBP-0x10] | short |
| height | [EBP-0x4] | short |
| thickness | [EBP+0x10] | short |
| color | [EBP+0x14] | short |
| i | [EBP-0x14] | int |

#### Darken_Fill

- Address: 1:0x001198DE (linear 0x001298DE), 63 bytes, in `regions/07-object-1/001AEE54-001AF653.txt`
- Type: void (short, short, short, short)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x10] | short |
| y1 | [EBP-0xC] | short |
| x2 | [EBP-0x8] | short |
| y2 | [EBP-0x4] | short |

#### Tint_Fill

- Address: 1:0x0011991D (linear 0x0012991D), 615 bytes, in `regions/07-object-1/001AEE54-001AF653.txt`
- Type: void (short, short, short, short, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0xC] | short |
| y1 | [EBP-0x10] | short |
| x2 | [EBP-0x4] | short |
| y2 | [EBP-0x8] | short |
| color | [EBP+0x10] | short |
| y | [EBP-0x4C] | int |
| pos | [EBP-0x48] | int |
| line_add | [EBP-0x44] | int |
| center_columns | [EBP-0x40] | int |
| start_x | [EBP-0x3C] | int |
| end_x | [EBP-0x38] | int |
| x | [EBP-0x34] | int |
| long_color | [EBP-0x60] | unsigned int |
| start_mask | [EBP-0x5C] | unsigned int |
| end_mask | [EBP-0x58] | unsigned int |
| start_color | [EBP-0x54] | unsigned int |
| end_color | [EBP-0x50] | unsigned int |
| column_mask | [EBP-0x1C] | unsigned int |
| value | [EBP-0x18] | unsigned int |
| long_current_video_page | [EBP-0x30] | unsigned int * |
| start_width | [EBP-0x2C] | int |
| center_width | [EBP-0x28] | int |
| end_width | [EBP-0x24] | int |
| line_start | [EBP-0x20] | int |
| column_height | [EBP-0x14] | int |

#### Gradient_Fill

- Address: 1:0x00119B84 (linear 0x00129B84), 235 bytes, in `regions/07-object-1/001AEE54-001AF653.txt`
- Type: void (int, int, int, int, int, char *, int, int, int)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x10] | int |
| y1 | [EBP-0xC] | int |
| x2 | [EBP-0x8] | int |
| y2 | [EBP-0x4] | int |
| fill_type | [EBP+0x10] | int |
| color_array | [EBP+0x14] | char * |
| color_count | [EBP+0x18] | int |
| ripple_count | [EBP+0x1C] | int |
| seed | [EBP+0x20] | int |

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
| 12 | typedef | BYTE | name for unsigned char (#5) |
| 13 | typedef | SBYTE | name for signed char (#4) |
| 14 | typedef | UBYTE | name for unsigned char (#5) |
| 15 | typedef | WORD | name for unsigned short (#7) |
| 16 | typedef | SWORD | name for short (#6) |
| 17 | typedef | UWORD | name for unsigned short (#7) |
| 18 | typedef | DWORD | name for unsigned int (#9) |
| 19 | typedef | SDWORD | name for int (#8) |
| 20 | typedef | UDWORD | name for unsigned int (#9) |
| 21 | typedef | BOOL | name for unsigned char (#5) |
| 22 | typedef | SampleHandle | name for int (#8) |
| 23 | array |  | 0..3; of unsigned int (#9) |
| 24 | scalar | void | void 1 bytes |
| 25 | procedure |  | near386; returns void (short, short, short, short) |
| 26 | procedure |  | near386; returns void () |
| 27 | procedure |  | near386; returns void (short, short, short, short, short) |
| 28 | pointer |  | near386; of unsigned int (#9) |
| 29 | procedure |  | near386; returns void (short, short, short, short, short, short) |
| 30 | procedure |  | near386; returns void (short, short, short, short, short, short, short) |
| 31 | pointer |  | near386; of char (#10) |
| 32 | procedure |  | near386; returns void (short, short, short, short, char *, short, short) |
| 33 | procedure |  | near386; returns void (short, short, short, short, char *, char *, short, short) |
| 34 | scalar | long | int 4 bytes |
| 35 | pointer |  | near386; of long (#34) |
| 36 | procedure |  | near386; returns void (short, short, short) |
| 37 | procedure |  | near386; returns void (int, int, int, int, int, char *, int, int, int) |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 26 | 1:0x00118AB6 (linear 0x00128AB6) | 0x001AE10A |
| 28 | 1:0x00118AD7 (linear 0x00128AD7) | 0x001AE12B |
| 29 | 1:0x00118AE5 (linear 0x00128AE5) | 0x001AE139 |
| 30 | 1:0x00118AF3 (linear 0x00128AF3) | 0x001AE147 |
| 31 | 1:0x00118B09 (linear 0x00128B09) | 0x001AE15D |
| 33 | 1:0x00118B1F (linear 0x00128B1F) | 0x001AE173 |
| 34 | 1:0x00118B28 (linear 0x00128B28) | 0x001AE17C |
| 35 | 1:0x00118B31 (linear 0x00128B31) | 0x001AE185 |
| 36 | 1:0x00118B3A (linear 0x00128B3A) | 0x001AE18E |
| 37 | 1:0x00118B43 (linear 0x00128B43) | 0x001AE197 |
| 39 | 1:0x00118B49 (linear 0x00128B49) | 0x001AE19D |
| 41 | 1:0x00118B61 (linear 0x00128B61) | 0x001AE1B5 |
| 42 | 1:0x00118B6D (linear 0x00128B6D) | 0x001AE1C1 |
| 43 | 1:0x00118B79 (linear 0x00128B79) | 0x001AE1CD |
| 44 | 1:0x00118B85 (linear 0x00128B85) | 0x001AE1D9 |
| 45 | 1:0x00118B91 (linear 0x00128B91) | 0x001AE1E5 |
| 46 | 1:0x00118B98 (linear 0x00128B98) | 0x001AE1EC |
| 48 | 1:0x00118BB0 (linear 0x00128BB0) | 0x001AE204 |
| 49 | 1:0x00118BBC (linear 0x00128BBC) | 0x001AE210 |
| 50 | 1:0x00118BC8 (linear 0x00128BC8) | 0x001AE21C |
| 51 | 1:0x00118BD4 (linear 0x00128BD4) | 0x001AE228 |
| 52 | 1:0x00118BE0 (linear 0x00128BE0) | 0x001AE234 |
| 55 | 1:0x00118BE7 (linear 0x00128BE7) | 0x001AE23B |
| 57 | 1:0x00118BFF (linear 0x00128BFF) | 0x001AE253 |
| 58 | 1:0x00118C08 (linear 0x00128C08) | 0x001AE25C |
| 59 | 1:0x00118C11 (linear 0x00128C11) | 0x001AE265 |
| 60 | 1:0x00118C1E (linear 0x00128C1E) | 0x001AE272 |
| 61 | 1:0x00118C2B (linear 0x00128C2B) | 0x001AE27F |
| 66 | 1:0x00118C32 (linear 0x00128C32) | 0x001AE286 |
| 73 | 1:0x00118C53 (linear 0x00128C53) | 0x001AE2A7 |
| 75 | 1:0x00118C5B (linear 0x00128C5B) | 0x001AE2AF |
| 76 | 1:0x00118C6F (linear 0x00128C6F) | 0x001AE2C3 |
| 78 | 1:0x00118C83 (linear 0x00128C83) | 0x001AE2D7 |
| 80 | 1:0x00118C94 (linear 0x00128C94) | 0x001AE2E8 |
| 82 | 1:0x00118CAC (linear 0x00128CAC) | 0x001AE300 |
| 84 | 1:0x00118CB1 (linear 0x00128CB1) | 0x001AE305 |
| 86 | 1:0x00118CC9 (linear 0x00128CC9) | 0x001AE31D |
| 89 | 1:0x00118CCE (linear 0x00128CCE) | 0x001AE322 |
| 90 | 1:0x00118CE3 (linear 0x00128CE3) | 0x001AE337 |
| 91 | 1:0x00118CF8 (linear 0x00128CF8) | 0x001AE34C |
| 92 | 1:0x00118D0D (linear 0x00128D0D) | 0x001AE361 |
| 95 | 1:0x00118D22 (linear 0x00128D22) | 0x001AE376 |
| 97 | 1:0x00118D40 (linear 0x00128D40) | 0x001AE394 |
| 98 | 1:0x00118D47 (linear 0x00128D47) | 0x001AE39B |
| 99 | 1:0x00118D50 (linear 0x00128D50) | 0x001AE3A4 |
| 101 | 1:0x00118D59 (linear 0x00128D59) | 0x001AE3AD |
| 102 | 1:0x00118D6D (linear 0x00128D6D) | 0x001AE3C1 |
| 104 | 1:0x00118D81 (linear 0x00128D81) | 0x001AE3D5 |
| 106 | 1:0x00118D8D (linear 0x00128D8D) | 0x001AE3E1 |
| 108 | 1:0x00118D97 (linear 0x00128D97) | 0x001AE3EB |
| 109 | 1:0x00118DAA (linear 0x00128DAA) | 0x001AE3FE |
| 110 | 1:0x00118DB4 (linear 0x00128DB4) | 0x001AE408 |
| 112 | 1:0x00118DC1 (linear 0x00128DC1) | 0x001AE415 |
| 114 | 1:0x00118DC7 (linear 0x00128DC7) | 0x001AE41B |
| 115 | 1:0x00118DCE (linear 0x00128DCE) | 0x001AE422 |
| 118 | 1:0x00118DD1 (linear 0x00128DD1) | 0x001AE425 |
| 120 | 1:0x00118DD7 (linear 0x00128DD7) | 0x001AE42B |
| 121 | 1:0x00118DDE (linear 0x00128DDE) | 0x001AE432 |
| 124 | 1:0x00118DE1 (linear 0x00128DE1) | 0x001AE435 |
| 125 | 1:0x00118DF1 (linear 0x00128DF1) | 0x001AE445 |
| 127 | 1:0x00118DFF (linear 0x00128DFF) | 0x001AE453 |
| 129 | 1:0x00118E05 (linear 0x00128E05) | 0x001AE459 |
| 130 | 1:0x00118E23 (linear 0x00128E23) | 0x001AE477 |
| 132 | 1:0x00118E46 (linear 0x00128E46) | 0x001AE49A |
| 134 | 1:0x00118E48 (linear 0x00128E48) | 0x001AE49C |
| 135 | 1:0x00118E4E (linear 0x00128E4E) | 0x001AE4A2 |
| 138 | 1:0x00118E6D (linear 0x00128E6D) | 0x001AE4C1 |
| 139 | 1:0x00118E73 (linear 0x00128E73) | 0x001AE4C7 |
| 141 | 1:0x00118E92 (linear 0x00128E92) | 0x001AE4E6 |
| 143 | 1:0x00118E94 (linear 0x00128E94) | 0x001AE4E8 |
| 145 | 1:0x00118EAA (linear 0x00128EAA) | 0x001AE4FE |
| 147 | 1:0x00118ED6 (linear 0x00128ED6) | 0x001AE52A |
| 155 | 1:0x00118EDE (linear 0x00128EDE) | 0x001AE532 |
| 164 | 1:0x00118EFF (linear 0x00128EFF) | 0x001AE553 |
| 166 | 1:0x00118F0F (linear 0x00128F0F) | 0x001AE563 |
| 169 | 1:0x00118F17 (linear 0x00128F17) | 0x001AE56B |
| 171 | 1:0x00118F24 (linear 0x00128F24) | 0x001AE578 |
| 172 | 1:0x00118F39 (linear 0x00128F39) | 0x001AE58D |
| 173 | 1:0x00118F4E (linear 0x00128F4E) | 0x001AE5A2 |
| 174 | 1:0x00118F63 (linear 0x00128F63) | 0x001AE5B7 |
| 176 | 1:0x00118F78 (linear 0x00128F78) | 0x001AE5CC |
| 178 | 1:0x00118F85 (linear 0x00128F85) | 0x001AE5D9 |
| 183 | 1:0x00118F92 (linear 0x00128F92) | 0x001AE5E6 |
| 189 | 1:0x00118FB0 (linear 0x00128FB0) | 0x001AE604 |
| 190 | 1:0x00118FC4 (linear 0x00128FC4) | 0x001AE618 |
| 192 | 1:0x00118FD8 (linear 0x00128FD8) | 0x001AE62C |
| 194 | 1:0x00118FE4 (linear 0x00128FE4) | 0x001AE638 |
| 196 | 1:0x00118FEE (linear 0x00128FEE) | 0x001AE642 |
| 197 | 1:0x00119001 (linear 0x00129001) | 0x001AE655 |
| 198 | 1:0x0011900B (linear 0x0012900B) | 0x001AE65F |
| 200 | 1:0x00119018 (linear 0x00129018) | 0x001AE66C |
| 202 | 1:0x0011901E (linear 0x0012901E) | 0x001AE672 |
| 203 | 1:0x00119025 (linear 0x00129025) | 0x001AE679 |
| 206 | 1:0x00119028 (linear 0x00129028) | 0x001AE67C |
| 208 | 1:0x0011902E (linear 0x0012902E) | 0x001AE682 |
| 209 | 1:0x00119035 (linear 0x00129035) | 0x001AE689 |
| 212 | 1:0x00119038 (linear 0x00129038) | 0x001AE68C |
| 213 | 1:0x00119048 (linear 0x00129048) | 0x001AE69C |
| 215 | 1:0x00119056 (linear 0x00129056) | 0x001AE6AA |
| 217 | 1:0x0011905C (linear 0x0012905C) | 0x001AE6B0 |
| 218 | 1:0x00119079 (linear 0x00129079) | 0x001AE6CD |
| 220 | 1:0x0011909B (linear 0x0012909B) | 0x001AE6EF |
| 222 | 1:0x0011909D (linear 0x0012909D) | 0x001AE6F1 |
| 223 | 1:0x001190A3 (linear 0x001290A3) | 0x001AE6F7 |
| 226 | 1:0x001190C1 (linear 0x001290C1) | 0x001AE715 |
| 227 | 1:0x001190C7 (linear 0x001290C7) | 0x001AE71B |
| 229 | 1:0x001190E5 (linear 0x001290E5) | 0x001AE739 |
| 231 | 1:0x001190E7 (linear 0x001290E7) | 0x001AE73B |
| 233 | 1:0x001190FD (linear 0x001290FD) | 0x001AE751 |
| 235 | 1:0x00119128 (linear 0x00129128) | 0x001AE77C |
| 244 | 1:0x00119130 (linear 0x00129130) | 0x001AE784 |
| 253 | 1:0x00119151 (linear 0x00129151) | 0x001AE7A5 |
| 255 | 1:0x0011916A (linear 0x0012916A) | 0x001AE7BE |
| 257 | 1:0x00119172 (linear 0x00129172) | 0x001AE7C6 |
| 258 | 1:0x00119186 (linear 0x00129186) | 0x001AE7DA |
| 260 | 1:0x0011919A (linear 0x0012919A) | 0x001AE7EE |
| 262 | 1:0x001191A7 (linear 0x001291A7) | 0x001AE7FB |
| 263 | 1:0x001191BC (linear 0x001291BC) | 0x001AE810 |
| 264 | 1:0x001191D1 (linear 0x001291D1) | 0x001AE825 |
| 265 | 1:0x001191E6 (linear 0x001291E6) | 0x001AE83A |
| 268 | 1:0x001191FB (linear 0x001291FB) | 0x001AE84F |
| 270 | 1:0x00119219 (linear 0x00129219) | 0x001AE86D |
| 271 | 1:0x0011922D (linear 0x0012922D) | 0x001AE881 |
| 273 | 1:0x00119241 (linear 0x00129241) | 0x001AE895 |
| 275 | 1:0x0011924D (linear 0x0012924D) | 0x001AE8A1 |
| 277 | 1:0x00119257 (linear 0x00129257) | 0x001AE8AB |
| 278 | 1:0x0011926A (linear 0x0012926A) | 0x001AE8BE |
| 279 | 1:0x00119274 (linear 0x00129274) | 0x001AE8C8 |
| 281 | 1:0x00119281 (linear 0x00129281) | 0x001AE8D5 |
| 283 | 1:0x00119287 (linear 0x00129287) | 0x001AE8DB |
| 284 | 1:0x0011928E (linear 0x0012928E) | 0x001AE8E2 |
| 287 | 1:0x00119291 (linear 0x00129291) | 0x001AE8E5 |
| 289 | 1:0x00119297 (linear 0x00129297) | 0x001AE8EB |
| 290 | 1:0x0011929E (linear 0x0012929E) | 0x001AE8F2 |
| 293 | 1:0x001192A1 (linear 0x001292A1) | 0x001AE8F5 |
| 294 | 1:0x001192B1 (linear 0x001292B1) | 0x001AE905 |
| 296 | 1:0x001192BF (linear 0x001292BF) | 0x001AE913 |
| 298 | 1:0x001192C5 (linear 0x001292C5) | 0x001AE919 |
| 299 | 1:0x001192E2 (linear 0x001292E2) | 0x001AE936 |
| 301 | 1:0x00119304 (linear 0x00129304) | 0x001AE958 |
| 303 | 1:0x00119306 (linear 0x00129306) | 0x001AE95A |
| 304 | 1:0x0011930C (linear 0x0012930C) | 0x001AE960 |
| 307 | 1:0x0011932A (linear 0x0012932A) | 0x001AE97E |
| 308 | 1:0x00119330 (linear 0x00129330) | 0x001AE984 |
| 310 | 1:0x0011934E (linear 0x0012934E) | 0x001AE9A2 |
| 312 | 1:0x00119350 (linear 0x00129350) | 0x001AE9A4 |
| 314 | 1:0x00119366 (linear 0x00129366) | 0x001AE9BA |
| 316 | 1:0x00119391 (linear 0x00129391) | 0x001AE9E5 |
| 325 | 1:0x00119399 (linear 0x00129399) | 0x001AE9ED |
| 333 | 1:0x001193BA (linear 0x001293BA) | 0x001AEA0E |
| 335 | 1:0x001193C2 (linear 0x001293C2) | 0x001AEA16 |
| 336 | 1:0x001193D6 (linear 0x001293D6) | 0x001AEA2A |
| 338 | 1:0x001193EA (linear 0x001293EA) | 0x001AEA3E |
| 340 | 1:0x001193F7 (linear 0x001293F7) | 0x001AEA4B |
| 341 | 1:0x0011940C (linear 0x0012940C) | 0x001AEA60 |
| 342 | 1:0x00119421 (linear 0x00129421) | 0x001AEA75 |
| 343 | 1:0x00119436 (linear 0x00129436) | 0x001AEA8A |
| 345 | 1:0x0011944B (linear 0x0012944B) | 0x001AEA9F |
| 346 | 1:0x00119469 (linear 0x00129469) | 0x001AEABD |
| 349 | 1:0x00119473 (linear 0x00129473) | 0x001AEAC7 |
| 351 | 1:0x00119480 (linear 0x00129480) | 0x001AEAD4 |
| 353 | 1:0x00119495 (linear 0x00129495) | 0x001AEAE9 |
| 354 | 1:0x001194AF (linear 0x001294AF) | 0x001AEB03 |
| 356 | 1:0x001194CC (linear 0x001294CC) | 0x001AEB20 |
| 358 | 1:0x0011950C (linear 0x0012950C) | 0x001AEB60 |
| 367 | 1:0x00119514 (linear 0x00129514) | 0x001AEB68 |
| 380 | 1:0x00119535 (linear 0x00129535) | 0x001AEB89 |
| 381 | 1:0x00119549 (linear 0x00129549) | 0x001AEB9D |
| 383 | 1:0x0011955D (linear 0x0012955D) | 0x001AEBB1 |
| 385 | 1:0x0011956A (linear 0x0012956A) | 0x001AEBBE |
| 386 | 1:0x0011957F (linear 0x0012957F) | 0x001AEBD3 |
| 387 | 1:0x00119594 (linear 0x00129594) | 0x001AEBE8 |
| 388 | 1:0x001195A9 (linear 0x001295A9) | 0x001AEBFD |
| 390 | 1:0x001195BE (linear 0x001295BE) | 0x001AEC12 |
| 391 | 1:0x001195DC (linear 0x001295DC) | 0x001AEC30 |
| 393 | 1:0x001195E6 (linear 0x001295E6) | 0x001AEC3A |
| 395 | 1:0x001195F0 (linear 0x001295F0) | 0x001AEC44 |
| 397 | 1:0x001195FD (linear 0x001295FD) | 0x001AEC51 |
| 398 | 1:0x0011960B (linear 0x0012960B) | 0x001AEC5F |
| 400 | 1:0x00119619 (linear 0x00129619) | 0x001AEC6D |
| 402 | 1:0x0011962A (linear 0x0012962A) | 0x001AEC7E |
| 403 | 1:0x00119644 (linear 0x00129644) | 0x001AEC98 |
| 404 | 1:0x00119665 (linear 0x00129665) | 0x001AECB9 |
| 407 | 1:0x001196A4 (linear 0x001296A4) | 0x001AECF8 |
| 408 | 1:0x001196BE (linear 0x001296BE) | 0x001AED12 |
| 409 | 1:0x001196DF (linear 0x001296DF) | 0x001AED33 |
| 411 | 1:0x00119725 (linear 0x00129725) | 0x001AED79 |
| 423 | 1:0x0011972D (linear 0x0012972D) | 0x001AED81 |
| 425 | 1:0x0011974C (linear 0x0012974C) | 0x001AEDA0 |
| 427 | 1:0x00119759 (linear 0x00129759) | 0x001AEDAD |
| 428 | 1:0x00119765 (linear 0x00129765) | 0x001AEDB9 |
| 429 | 1:0x00119771 (linear 0x00129771) | 0x001AEDC5 |
| 430 | 1:0x0011977D (linear 0x0012977D) | 0x001AEDD1 |
| 432 | 1:0x00119789 (linear 0x00129789) | 0x001AEDDD |
| 434 | 1:0x0011979F (linear 0x0012979F) | 0x001AEDF3 |
| 435 | 1:0x001197BC (linear 0x001297BC) | 0x001AEE10 |
| 441 | 1:0x001197C3 (linear 0x001297C3) | 0x001AEE17 |
| 445 | 1:0x001197E4 (linear 0x001297E4) | 0x001AEE38 |
| 447 | 1:0x00119801 (linear 0x00129801) | 0x001AEE55 |
| 448 | 1:0x00119831 (linear 0x00129831) | 0x001AEE85 |
| 450 | 1:0x00119861 (linear 0x00129861) | 0x001AEEB5 |
| 451 | 1:0x00119899 (linear 0x00129899) | 0x001AEEED |
| 452 | 1:0x001198D1 (linear 0x001298D1) | 0x001AEF25 |
| 453 | 1:0x001198D6 (linear 0x001298D6) | 0x001AEF2A |
| 455 | 1:0x001198DE (linear 0x001298DE) | 0x001AEF32 |
| 457 | 1:0x001198FF (linear 0x001298FF) | 0x001AEF53 |
| 458 | 1:0x00119917 (linear 0x00129917) | 0x001AEF6B |
| 464 | 1:0x0011991D (linear 0x0012991D) | 0x001AEF71 |
| 471 | 1:0x0011993E (linear 0x0012993E) | 0x001AEF92 |
| 473 | 1:0x00119946 (linear 0x00129946) | 0x001AEF9A |
| 474 | 1:0x0011995A (linear 0x0012995A) | 0x001AEFAE |
| 476 | 1:0x0011996E (linear 0x0012996E) | 0x001AEFC2 |
| 478 | 1:0x0011997B (linear 0x0012997B) | 0x001AEFCF |
| 479 | 1:0x00119990 (linear 0x00129990) | 0x001AEFE4 |
| 480 | 1:0x001199A5 (linear 0x001299A5) | 0x001AEFF9 |
| 481 | 1:0x001199BA (linear 0x001299BA) | 0x001AF00E |
| 484 | 1:0x001199CF (linear 0x001299CF) | 0x001AF023 |
| 486 | 1:0x001199ED (linear 0x001299ED) | 0x001AF041 |
| 487 | 1:0x001199F4 (linear 0x001299F4) | 0x001AF048 |
| 488 | 1:0x001199FD (linear 0x001299FD) | 0x001AF051 |
| 490 | 1:0x00119A06 (linear 0x00129A06) | 0x001AF05A |
| 491 | 1:0x00119A1A (linear 0x00129A1A) | 0x001AF06E |
| 493 | 1:0x00119A2E (linear 0x00129A2E) | 0x001AF082 |
| 495 | 1:0x00119A3A (linear 0x00129A3A) | 0x001AF08E |
| 497 | 1:0x00119A44 (linear 0x00129A44) | 0x001AF098 |
| 498 | 1:0x00119A57 (linear 0x00129A57) | 0x001AF0AB |
| 499 | 1:0x00119A61 (linear 0x00129A61) | 0x001AF0B5 |
| 501 | 1:0x00119A6D (linear 0x00129A6D) | 0x001AF0C1 |
| 502 | 1:0x00119A7D (linear 0x00129A7D) | 0x001AF0D1 |
| 504 | 1:0x00119A8B (linear 0x00129A8B) | 0x001AF0DF |
| 506 | 1:0x00119A91 (linear 0x00129A91) | 0x001AF0E5 |
| 507 | 1:0x00119AAF (linear 0x00129AAF) | 0x001AF103 |
| 509 | 1:0x00119AD3 (linear 0x00129AD3) | 0x001AF127 |
| 511 | 1:0x00119AD5 (linear 0x00129AD5) | 0x001AF129 |
| 512 | 1:0x00119ADB (linear 0x00129ADB) | 0x001AF12F |
| 514 | 1:0x00119B03 (linear 0x00129B03) | 0x001AF157 |
| 515 | 1:0x00119B09 (linear 0x00129B09) | 0x001AF15D |
| 518 | 1:0x00119B30 (linear 0x00129B30) | 0x001AF184 |
| 520 | 1:0x00119B32 (linear 0x00129B32) | 0x001AF186 |
| 522 | 1:0x00119B46 (linear 0x00129B46) | 0x001AF19A |
| 524 | 1:0x00119B7C (linear 0x00129B7C) | 0x001AF1D0 |
| 532 | 1:0x00119B84 (linear 0x00129B84) | 0x001AF1D8 |
| 535 | 1:0x00119BA5 (linear 0x00129BA5) | 0x001AF1F9 |
| 537 | 1:0x00119BE7 (linear 0x00129BE7) | 0x001AF23B |
| 539 | 1:0x00119BEC (linear 0x00129BEC) | 0x001AF240 |
| 540 | 1:0x00119C0A (linear 0x00129C0A) | 0x001AF25E |
| 542 | 1:0x00119C0C (linear 0x00129C0C) | 0x001AF260 |
| 543 | 1:0x00119C24 (linear 0x00129C24) | 0x001AF278 |
| 545 | 1:0x00119C26 (linear 0x00129C26) | 0x001AF27A |
| 546 | 1:0x00119C44 (linear 0x00129C44) | 0x001AF298 |
| 581 | 1:0x00119C46 (linear 0x00129C46) | 0x001AF29A |
| 587 | 1:0x00119C67 (linear 0x00129C67) | 0x001AF2BB |

