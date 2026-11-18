# F:\WLIBS\fonts\paragrph.c

Module 208 of the debug info of ORION2.EXE.

Ranges: 1:0x0012E246 (linear 0x0013E246), 3,091 bytes, 2:0x0000C9EC (linear 0x0017C9EC), 4 bytes, 2:0x00048AF4 (linear 0x001B8AF4), 836 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0012E246 (linear 0x0013E246) | code | Print_Paragraph_ | `regions/07-object-1/001C3654-001C3E53.txt` |
| 1:0x0012E47F (linear 0x0013E47F) | code | Mark_Paragraph_ | `regions/07-object-1/001C3654-001C3E53.txt` |
| 1:0x0012E87F (linear 0x0013E87F) | code | Get_Paragraph_Max_Height_ | `regions/07-object-1/001C3E54-001C4653.txt` |
| 1:0x0012E8FA (linear 0x0013E8FA) | code | Get_Paragraph_Max_Width_ | `regions/07-object-1/001C3E54-001C4653.txt` |
| 1:0x0012EA9E (linear 0x0013EA9E) | code | Reset_Paragraph_Exclusions_ | `regions/07-object-1/001C3E54-001C4653.txt` |
| 1:0x0012EAC6 (linear 0x0013EAC6) | code | Set_Paragraph_Exclusion_Area_ | `regions/07-object-1/001C3E54-001C4653.txt` |
| 1:0x0012EB55 (linear 0x0013EB55) | code | Set_Paragraph_Exclusion_Picture_ | `regions/07-object-1/001C3E54-001C4653.txt` |
| 1:0x0012EBFE (linear 0x0013EBFE) | code | Remove_Paragraph_Marks_ | `regions/07-object-1/001C3E54-001C4653.txt` |
| 1:0x0012EC54 (linear 0x0013EC54) | code | Print_Paragraph_To_Bitmap_ | `regions/07-object-1/001C3E54-001C4653.txt` |
| 2:0x0000C9EC (linear 0x0017C9EC) | data | _loaded_paragraph_space_flag | `regions/08-object-2/001FAE54-001FB653.txt` |
| 2:0x0000C9EE (linear 0x0017C9EE) | data | _exclusion_count | `regions/08-object-2/001FAE54-001FB653.txt` |
| 2:0x00048AF4 (linear 0x001B8AF4) | data | _exclusion_area_x1 | - |
| 2:0x00048AFC (linear 0x001B8AFC) | data | _exclusion_area_x2 | - |
| 2:0x00048B04 (linear 0x001B8B04) | data | _exclusion_area_y1 | - |
| 2:0x00048B0C (linear 0x001B8B0C) | data | _exclusion_area_y2 | - |
| 2:0x00048B14 (linear 0x001B8B14) | data | _paragraph_line_y_start | - |
| 2:0x00048BDC (linear 0x001B8BDC) | data | _paragraph_line_x_start | - |
| 2:0x00048CA4 (linear 0x001B8CA4) | data | _paragraph_line_offset | - |
| 2:0x00048D6C (linear 0x001B8D6C) | data | _paragraph_line_x_end | - |
| 2:0x00048E34 (linear 0x001B8E34) | data | _paragraph_max_lines | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x00048AF4 (linear 0x001B8AF4) | exclusion_area_x1 | short[4] |
| 2:0x00048AFC (linear 0x001B8AFC) | exclusion_area_x2 | short[4] |
| 2:0x00048B04 (linear 0x001B8B04) | exclusion_area_y1 | short[4] |
| 2:0x00048B0C (linear 0x001B8B0C) | exclusion_area_y2 | short[4] |
| 2:0x00048B14 (linear 0x001B8B14) | paragraph_line_y_start | short[100] |
| 2:0x00048BDC (linear 0x001B8BDC) | paragraph_line_x_start | short[100] |
| 2:0x00048CA4 (linear 0x001B8CA4) | paragraph_line_offset | short[100] |
| 2:0x00048D6C (linear 0x001B8D6C) | paragraph_line_x_end | short[100] |
| 2:0x0000C9EC (linear 0x0017C9EC) | loaded_paragraph_space_flag | short |
| 2:0x00048E34 (linear 0x001B8E34) | paragraph_max_lines | short |
| 2:0x0000C9EE (linear 0x0017C9EE) | exclusion_count | short |

#### Print_Paragraph

- Address: 1:0x0012E246 (linear 0x0013E246), 569 bytes, in `regions/07-object-1/001C3654-001C3E53.txt`
- Type: void (short, short, short, char *, short)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x | [EBP-0xC] | short |
| y | [EBP-0x10] | short |
| max_width | [EBP-0x8] | short |
| string | [EBP-0x14] | char * |
| print_type | [EBP+0x10] | short |
| i | [EBP-0x4] | short |

#### Mark_Paragraph

- Address: 1:0x0012E47F (linear 0x0013E47F), 1,024 bytes, in `regions/07-object-1/001C3654-001C3E53.txt`
- Type: int (short, short, short, char *); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x | [EBP-0x58] | short |
| y | [EBP-0x4C] | short |
| full_width | [EBP-0x54] | short |
| string | [EBP-0x68] | char * |
| error | [EBP-0x3C] | short |
| str_ptr | [EBP-0x38] | short |
| font_height | [EBP-0x50] | short |
| i | [EBP-0x40] | short |
| spacing | [EBP-0x48] | short |
| line_feed | [EBP-0x44] | short |
| line_num | [EBP-0xC] | short |
| terminated_flag | [EBP-0x8] | short |
| xmin | [EBP-0x4] | short |
| ypos | [EBP-0x34] | short |
| xmax | [EBP-0x30] | short |
| skip_flag | [EBP-0x2C] | short |
| blank_pos | [EBP-0x28] | short |
| temp_ptr | [EBP-0x24] | short |
| character | [EBP-0x20] | short |
| tab | [EBP-0x1C] | short |
| max_width | [EBP-0x18] | short |
| x_override | [EBP-0x14] | short |
| pseg | [EBP-0x10] | short |
| string_widths | [EBP-0x5C] | unsigned char * |
| buffer | [EBP-0x64] | char[4] |

#### Get_Paragraph_Max_Height

- Address: 1:0x0012E87F (linear 0x0013E87F), 123 bytes, in `regions/07-object-1/001C3E54-001C4653.txt`
- Type: short (short, char *); returns in AX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| max_width | [EBP-0x1C] | short |
| string | [EBP-0x20] | char * |
| line_feed | [EBP-0x18] | short |
| font_height | [EBP-0x14] | short |
| height | [EBP-0x10] | short |
| i | [EBP-0xC] | short |
| ymax | [EBP-0x8] | short |

#### Get_Paragraph_Max_Width

- Address: 1:0x0012E8FA (linear 0x0013E8FA), 420 bytes, in `regions/07-object-1/001C3E54-001C4653.txt`
- Type: short (short, char *, short); returns in AX
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| max_width | [EBP-0x34] | short |
| string | [EBP-0x38] | char * |
| print_type | [EBP-0x30] | short |
| width | [EBP-0x2C] | short |
| xmax | [EBP-0x28] | short |
| i | [EBP-0x24] | short |
| x1 | [EBP-0x20] | short |
| x2 | [EBP-0x1C] | short |
| xc | [EBP-0x18] | short |
| string_width | [EBP-0x14] | short |
| x1min | [EBP-0x10] | short |
| x2max | [EBP-0xC] | short |
| horizontal_spacing | [EBP-0x8] | short |

#### Reset_Paragraph_Exclusions

- Address: 1:0x0012EA9E (linear 0x0013EA9E), 40 bytes, in `regions/07-object-1/001C3E54-001C4653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Set_Paragraph_Exclusion_Area

- Address: 1:0x0012EAC6 (linear 0x0013EAC6), 143 bytes, in `regions/07-object-1/001C3E54-001C4653.txt`
- Type: void (short, short, short, short)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0x10] | short |
| y1 | [EBP-0xC] | short |
| x2 | [EBP-0x8] | short |
| y2 | [EBP-0x4] | short |

#### Set_Paragraph_Exclusion_Picture

- Address: 1:0x0012EB55 (linear 0x0013EB55), 169 bytes, in `regions/07-object-1/001C3E54-001C4653.txt`
- Type: void (short, short, void *)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| x1 | [EBP-0xC] | short |
| y1 | [EBP-0x10] | short |
| picture | [EBP-0x14] | void * |
| ddx | [EBP-0x8] | short |
| ddy | [EBP-0x4] | short |

#### Remove_Paragraph_Marks

- Address: 1:0x0012EBFE (linear 0x0013EBFE), 86 bytes, in `regions/07-object-1/001C3E54-001C4653.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| string | [EBP-0x8] | char * |
| i | [EBP-0x4] | int |

#### Print_Paragraph_To_Bitmap

- Address: 1:0x0012EC54 (linear 0x0013EC54), 517 bytes, in `regions/07-object-1/001C3E54-001C4653.txt`
- Type: void (short, short, short, char *, short, void *)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| x | [EBP-0x4] | short |
| y | [EBP-0xC] | short |
| max_width | [EBP-0x8] | short |
| string | [EBP-0x10] | char * |
| print_type | [EBP+0x10] | short |
| bitmap_seg | [EBP+0x14] | void * |
| i | [EBP-0x14] | int |

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
| 23 | array |  | 0..3; of short (#6) |
| 24 | array |  | 0..3; of short (#6) |
| 25 | array |  | 0..3; of short (#6) |
| 26 | array |  | 0..3; of short (#6) |
| 27 | array |  | 0..99; of short (#6) |
| 28 | array |  | 0..99; of short (#6) |
| 29 | array |  | 0..99; of short (#6) |
| 30 | array |  | 0..99; of short (#6) |
| 31 | scalar | void | void 1 bytes |
| 32 | pointer |  | near386; of char (#10) |
| 33 | procedure |  | near386; returns void (short, short, short, char *, short) |
| 34 | procedure |  | near386; returns int (short, short, short, char *) |
| 35 | pointer |  | near386; of unsigned char (#5) |
| 36 | array |  | 0..3; of char (#10) |
| 37 | procedure |  | near386; returns short (short, char *) |
| 38 | procedure |  | near386; returns short (short, char *, short) |
| 39 | procedure |  | near386; returns void () |
| 40 | procedure |  | near386; returns void (short, short, short, short) |
| 41 | pointer |  | near386; of void (#31) |
| 42 | procedure |  | near386; returns void (short, short, void *) |
| 43 | procedure |  | near386; returns void (char *) |
| 44 | procedure |  | near386; returns void (short, short, short, char *, short, void *) |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 30 | 1:0x0012E246 (linear 0x0013E246) | 0x001C389A |
| 34 | 1:0x0012E267 (linear 0x0013E267) | 0x001C38BB |
| 36 | 1:0x0012E27B (linear 0x0013E27B) | 0x001C38CF |
| 38 | 1:0x0012E29B (linear 0x0013E29B) | 0x001C38EF |
| 40 | 1:0x0012E2D6 (linear 0x0013E2D6) | 0x001C392A |
| 42 | 1:0x0012E2F7 (linear 0x0013E2F7) | 0x001C394B |
| 44 | 1:0x0012E32C (linear 0x0013E32C) | 0x001C3980 |
| 45 | 1:0x0012E331 (linear 0x0013E331) | 0x001C3985 |
| 47 | 1:0x0012E366 (linear 0x0013E366) | 0x001C39BA |
| 48 | 1:0x0012E36B (linear 0x0013E36B) | 0x001C39BF |
| 50 | 1:0x0012E3BD (linear 0x0013E3BD) | 0x001C3A11 |
| 51 | 1:0x0012E3C2 (linear 0x0013E3C2) | 0x001C3A16 |
| 52 | 1:0x0012E3D3 (linear 0x0013E3D3) | 0x001C3A27 |
| 55 | 1:0x0012E42A (linear 0x0013E42A) | 0x001C3A7E |
| 56 | 1:0x0012E42C (linear 0x0013E42C) | 0x001C3A80 |
| 61 | 1:0x0012E461 (linear 0x0013E461) | 0x001C3AB5 |
| 63 | 1:0x0012E466 (linear 0x0013E466) | 0x001C3ABA |
| 65 | 1:0x0012E46E (linear 0x0013E46E) | 0x001C3AC2 |
| 66 | 1:0x0012E477 (linear 0x0013E477) | 0x001C3ACB |
| 68 | 1:0x0012E47F (linear 0x0013E47F) | 0x001C3AD3 |
| 76 | 1:0x0012E4A0 (linear 0x0013E4A0) | 0x001C3AF4 |
| 78 | 1:0x0012E4A7 (linear 0x0013E4A7) | 0x001C3AFB |
| 79 | 1:0x0012E4B0 (linear 0x0013E4B0) | 0x001C3B04 |
| 81 | 1:0x0012E4B7 (linear 0x0013E4B7) | 0x001C3B0B |
| 82 | 1:0x0012E4BE (linear 0x0013E4BE) | 0x001C3B12 |
| 83 | 1:0x0012E4C7 (linear 0x0013E4C7) | 0x001C3B1B |
| 84 | 1:0x0012E4D0 (linear 0x0013E4D0) | 0x001C3B24 |
| 86 | 1:0x0012E4D9 (linear 0x0013E4D9) | 0x001C3B2D |
| 88 | 1:0x0012E4DF (linear 0x0013E4DF) | 0x001C3B33 |
| 90 | 1:0x0012E4EF (linear 0x0013E4EF) | 0x001C3B43 |
| 92 | 1:0x0012E4F6 (linear 0x0013E4F6) | 0x001C3B4A |
| 94 | 1:0x0012E4FF (linear 0x0013E4FF) | 0x001C3B53 |
| 95 | 1:0x0012E505 (linear 0x0013E505) | 0x001C3B59 |
| 97 | 1:0x0012E50F (linear 0x0013E50F) | 0x001C3B63 |
| 99 | 1:0x0012E511 (linear 0x0013E511) | 0x001C3B65 |
| 100 | 1:0x0012E517 (linear 0x0013E517) | 0x001C3B6B |
| 101 | 1:0x0012E521 (linear 0x0013E521) | 0x001C3B75 |
| 104 | 1:0x0012E528 (linear 0x0013E528) | 0x001C3B7C |
| 106 | 1:0x0012E54F (linear 0x0013E54F) | 0x001C3BA3 |
| 109 | 1:0x0012E5B2 (linear 0x0013E5B2) | 0x001C3C06 |
| 111 | 1:0x0012E5D8 (linear 0x0013E5D8) | 0x001C3C2C |
| 112 | 1:0x0012E5FB (linear 0x0013E5FB) | 0x001C3C4F |
| 114 | 1:0x0012E60E (linear 0x0013E60E) | 0x001C3C62 |
| 116 | 1:0x0012E61E (linear 0x0013E61E) | 0x001C3C72 |
| 118 | 1:0x0012E623 (linear 0x0013E623) | 0x001C3C77 |
| 120 | 1:0x0012E62E (linear 0x0013E62E) | 0x001C3C82 |
| 122 | 1:0x0012E638 (linear 0x0013E638) | 0x001C3C8C |
| 123 | 1:0x0012E63F (linear 0x0013E63F) | 0x001C3C93 |
| 125 | 1:0x0012E646 (linear 0x0013E646) | 0x001C3C9A |
| 128 | 1:0x0012E64C (linear 0x0013E64C) | 0x001C3CA0 |
| 130 | 1:0x0012E65E (linear 0x0013E65E) | 0x001C3CB2 |
| 132 | 1:0x0012E66C (linear 0x0013E66C) | 0x001C3CC0 |
| 134 | 1:0x0012E675 (linear 0x0013E675) | 0x001C3CC9 |
| 135 | 1:0x0012E686 (linear 0x0013E686) | 0x001C3CDA |
| 137 | 1:0x0012E695 (linear 0x0013E695) | 0x001C3CE9 |
| 139 | 1:0x0012E697 (linear 0x0013E697) | 0x001C3CEB |
| 142 | 1:0x0012E6C8 (linear 0x0013E6C8) | 0x001C3D1C |
| 143 | 1:0x0012E6D1 (linear 0x0013E6D1) | 0x001C3D25 |
| 146 | 1:0x0012E6D7 (linear 0x0013E6D7) | 0x001C3D2B |
| 147 | 1:0x0012E6DA (linear 0x0013E6DA) | 0x001C3D2E |
| 149 | 1:0x0012E6DF (linear 0x0013E6DF) | 0x001C3D33 |
| 151 | 1:0x0012E6EA (linear 0x0013E6EA) | 0x001C3D3E |
| 153 | 1:0x0012E6F7 (linear 0x0013E6F7) | 0x001C3D4B |
| 154 | 1:0x0012E70C (linear 0x0013E70C) | 0x001C3D60 |
| 155 | 1:0x0012E721 (linear 0x0013E721) | 0x001C3D75 |
| 156 | 1:0x0012E736 (linear 0x0013E736) | 0x001C3D8A |
| 157 | 1:0x0012E74B (linear 0x0013E74B) | 0x001C3D9F |
| 159 | 1:0x0012E752 (linear 0x0013E752) | 0x001C3DA6 |
| 161 | 1:0x0012E75B (linear 0x0013E75B) | 0x001C3DAF |
| 162 | 1:0x0012E76F (linear 0x0013E76F) | 0x001C3DC3 |
| 163 | 1:0x0012E780 (linear 0x0013E780) | 0x001C3DD4 |
| 164 | 1:0x0012E789 (linear 0x0013E789) | 0x001C3DDD |
| 167 | 1:0x0012E78D (linear 0x0013E78D) | 0x001C3DE1 |
| 169 | 1:0x0012E796 (linear 0x0013E796) | 0x001C3DEA |
| 170 | 1:0x0012E7AA (linear 0x0013E7AA) | 0x001C3DFE |
| 171 | 1:0x0012E7BB (linear 0x0013E7BB) | 0x001C3E0F |
| 172 | 1:0x0012E7C1 (linear 0x0013E7C1) | 0x001C3E15 |
| 173 | 1:0x0012E7C7 (linear 0x0013E7C7) | 0x001C3E1B |
| 175 | 1:0x0012E7CB (linear 0x0013E7CB) | 0x001C3E1F |
| 177 | 1:0x0012E7D1 (linear 0x0013E7D1) | 0x001C3E25 |
| 179 | 1:0x0012E7D3 (linear 0x0013E7D3) | 0x001C3E27 |
| 180 | 1:0x0012E7E8 (linear 0x0013E7E8) | 0x001C3E3C |
| 181 | 1:0x0012E7FD (linear 0x0013E7FD) | 0x001C3E51 |
| 182 | 1:0x0012E812 (linear 0x0013E812) | 0x001C3E66 |
| 183 | 1:0x0012E827 (linear 0x0013E827) | 0x001C3E7B |
| 185 | 1:0x0012E82E (linear 0x0013E82E) | 0x001C3E82 |
| 187 | 1:0x0012E837 (linear 0x0013E837) | 0x001C3E8B |
| 188 | 1:0x0012E841 (linear 0x0013E841) | 0x001C3E95 |
| 193 | 1:0x0012E848 (linear 0x0013E848) | 0x001C3E9C |
| 194 | 1:0x0012E862 (linear 0x0013E862) | 0x001C3EB6 |
| 196 | 1:0x0012E86A (linear 0x0013E86A) | 0x001C3EBE |
| 197 | 1:0x0012E86F (linear 0x0013E86F) | 0x001C3EC3 |
| 198 | 1:0x0012E876 (linear 0x0013E876) | 0x001C3ECA |
| 201 | 1:0x0012E87F (linear 0x0013E87F) | 0x001C3ED3 |
| 205 | 1:0x0012E89C (linear 0x0013E89C) | 0x001C3EF0 |
| 207 | 1:0x0012E8A4 (linear 0x0013E8A4) | 0x001C3EF8 |
| 209 | 1:0x0012E8B4 (linear 0x0013E8B4) | 0x001C3F08 |
| 210 | 1:0x0012E8CA (linear 0x0013E8CA) | 0x001C3F1E |
| 212 | 1:0x0012E8E1 (linear 0x0013E8E1) | 0x001C3F35 |
| 214 | 1:0x0012E8E9 (linear 0x0013E8E9) | 0x001C3F3D |
| 215 | 1:0x0012E8EF (linear 0x0013E8EF) | 0x001C3F43 |
| 218 | 1:0x0012E8FA (linear 0x0013E8FA) | 0x001C3F4E |
| 223 | 1:0x0012E919 (linear 0x0013E919) | 0x001C3F6D |
| 225 | 1:0x0012E929 (linear 0x0013E929) | 0x001C3F7D |
| 226 | 1:0x0012E930 (linear 0x0013E930) | 0x001C3F84 |
| 229 | 1:0x0012E937 (linear 0x0013E937) | 0x001C3F8B |
| 231 | 1:0x0012E96A (linear 0x0013E96A) | 0x001C3FBE |
| 233 | 1:0x0012E985 (linear 0x0013E985) | 0x001C3FD9 |
| 235 | 1:0x0012E9A6 (linear 0x0013E9A6) | 0x001C3FFA |
| 236 | 1:0x0012E9B6 (linear 0x0013E9B6) | 0x001C400A |
| 237 | 1:0x0012E9BF (linear 0x0013E9BF) | 0x001C4013 |
| 239 | 1:0x0012E9C4 (linear 0x0013E9C4) | 0x001C4018 |
| 240 | 1:0x0012E9D4 (linear 0x0013E9D4) | 0x001C4028 |
| 241 | 1:0x0012E9DD (linear 0x0013E9DD) | 0x001C4031 |
| 243 | 1:0x0012E9DF (linear 0x0013E9DF) | 0x001C4033 |
| 244 | 1:0x0012EA0B (linear 0x0013EA0B) | 0x001C405F |
| 245 | 1:0x0012EA21 (linear 0x0013EA21) | 0x001C4075 |
| 246 | 1:0x0012EA37 (linear 0x0013EA37) | 0x001C408B |
| 248 | 1:0x0012EA39 (linear 0x0013EA39) | 0x001C408D |
| 249 | 1:0x0012EA49 (linear 0x0013EA49) | 0x001C409D |
| 253 | 1:0x0012EA59 (linear 0x0013EA59) | 0x001C40AD |
| 254 | 1:0x0012EA68 (linear 0x0013EA68) | 0x001C40BC |
| 255 | 1:0x0012EA77 (linear 0x0013EA77) | 0x001C40CB |
| 256 | 1:0x0012EA7C (linear 0x0013EA7C) | 0x001C40D0 |
| 258 | 1:0x0012EA84 (linear 0x0013EA84) | 0x001C40D8 |
| 260 | 1:0x0012EA8E (linear 0x0013EA8E) | 0x001C40E2 |
| 261 | 1:0x0012EA94 (linear 0x0013EA94) | 0x001C40E8 |
| 265 | 1:0x0012EA9E (linear 0x0013EA9E) | 0x001C40F2 |
| 267 | 1:0x0012EAB6 (linear 0x0013EAB6) | 0x001C410A |
| 268 | 1:0x0012EABF (linear 0x0013EABF) | 0x001C4113 |
| 270 | 1:0x0012EAC6 (linear 0x0013EAC6) | 0x001C411A |
| 272 | 1:0x0012EAE7 (linear 0x0013EAE7) | 0x001C413B |
| 274 | 1:0x0012EAF4 (linear 0x0013EAF4) | 0x001C4148 |
| 275 | 1:0x0012EB09 (linear 0x0013EB09) | 0x001C415D |
| 276 | 1:0x0012EB1E (linear 0x0013EB1E) | 0x001C4172 |
| 277 | 1:0x0012EB33 (linear 0x0013EB33) | 0x001C4187 |
| 278 | 1:0x0012EB48 (linear 0x0013EB48) | 0x001C419C |
| 280 | 1:0x0012EB4F (linear 0x0013EB4F) | 0x001C41A3 |
| 282 | 1:0x0012EB55 (linear 0x0013EB55) | 0x001C41A9 |
| 286 | 1:0x0012EB74 (linear 0x0013EB74) | 0x001C41C8 |
| 287 | 1:0x0012EB7D (linear 0x0013EB7D) | 0x001C41D1 |
| 289 | 1:0x0012EB87 (linear 0x0013EB87) | 0x001C41DB |
| 291 | 1:0x0012EB94 (linear 0x0013EB94) | 0x001C41E8 |
| 292 | 1:0x0012EBA9 (linear 0x0013EBA9) | 0x001C41FD |
| 293 | 1:0x0012EBBE (linear 0x0013EBBE) | 0x001C4212 |
| 294 | 1:0x0012EBD7 (linear 0x0013EBD7) | 0x001C422B |
| 295 | 1:0x0012EBF0 (linear 0x0013EBF0) | 0x001C4244 |
| 297 | 1:0x0012EBF7 (linear 0x0013EBF7) | 0x001C424B |
| 302 | 1:0x0012EBFE (linear 0x0013EBFE) | 0x001C4252 |
| 306 | 1:0x0012EC19 (linear 0x0013EC19) | 0x001C426D |
| 308 | 1:0x0012EC20 (linear 0x0013EC20) | 0x001C4274 |
| 310 | 1:0x0012EC2B (linear 0x0013EC2B) | 0x001C427F |
| 311 | 1:0x0012EC46 (linear 0x0013EC46) | 0x001C429A |
| 312 | 1:0x0012EC49 (linear 0x0013EC49) | 0x001C429D |
| 313 | 1:0x0012EC4B (linear 0x0013EC4B) | 0x001C429F |
| 362 | 1:0x0012EC54 (linear 0x0013EC54) | 0x001C42A8 |
| 366 | 1:0x0012EC75 (linear 0x0013EC75) | 0x001C42C9 |
| 368 | 1:0x0012EC89 (linear 0x0013EC89) | 0x001C42DD |
| 370 | 1:0x0012ECBA (linear 0x0013ECBA) | 0x001C430E |
| 372 | 1:0x0012ECDB (linear 0x0013ECDB) | 0x001C432F |
| 374 | 1:0x0012ED10 (linear 0x0013ED10) | 0x001C4364 |
| 375 | 1:0x0012ED15 (linear 0x0013ED15) | 0x001C4369 |
| 377 | 1:0x0012ED4A (linear 0x0013ED4A) | 0x001C439E |
| 378 | 1:0x0012ED4F (linear 0x0013ED4F) | 0x001C43A3 |
| 380 | 1:0x0012EDA0 (linear 0x0013EDA0) | 0x001C43F4 |
| 381 | 1:0x0012EDA5 (linear 0x0013EDA5) | 0x001C43F9 |
| 382 | 1:0x0012EDB3 (linear 0x0013EDB3) | 0x001C4407 |
| 385 | 1:0x0012EE04 (linear 0x0013EE04) | 0x001C4458 |
| 386 | 1:0x0012EE06 (linear 0x0013EE06) | 0x001C445A |
| 390 | 1:0x0012EE3B (linear 0x0013EE3B) | 0x001C448F |
| 393 | 1:0x0012EE40 (linear 0x0013EE40) | 0x001C4494 |
| 395 | 1:0x0012EE48 (linear 0x0013EE48) | 0x001C449C |
| 396 | 1:0x0012EE51 (linear 0x0013EE51) | 0x001C44A5 |

