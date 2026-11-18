# F:\WLIBS\lbx\makelbx.c

Module 278 of the debug info of ORION2.EXE.

Ranges: 1:0x0014001F (linear 0x0015001F), 2,316 bytes, 2:0x00004174 (linear 0x00174174), 156 bytes, 2:0x0004C0F0 (linear 0x001BC0F0), 17,104 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0014001F (linear 0x0015001F) | code | LBX_Open_File_ | `regions/07-object-1/001D5654-001D5E53.txt` |
| 1:0x0014016F (linear 0x0015016F) | code | LBX_Add_File_ | `regions/07-object-1/001D5654-001D5E53.txt` |
| 1:0x00140394 (linear 0x00150394) | code | LBX_Add_Data_File_ | `regions/07-object-1/001D5654-001D5E53.txt` |
| 1:0x001405B5 (linear 0x001505B5) | code | LBX_Extract_File_ | `regions/07-object-1/001D5654-001D5E53.txt` |
| 1:0x00140794 (linear 0x00150794) | code | Read_LBX_File_Type_ | `regions/07-object-1/001D5654-001D5E53.txt` |
| 1:0x001407FD (linear 0x001507FD) | code | Read_LBX_File_Entry_Count_ | `regions/07-object-1/001D5654-001D5E53.txt` |
| 1:0x00140863 (linear 0x00150863) | code | Repeat_LBX_File_ | `regions/07-object-1/001D5E54-001D6653.txt` |
| 2:0x0004C0F0 (linear 0x001BC0F0) | static, data | _lbx_buffer | - |
| 2:0x00050358 (linear 0x001C0358) | static, data | _current_lbx_file_name | - |
| 2:0x000503BC (linear 0x001C03BC) | static, data | _max_lbx_file_count | - |
| 2:0x000503BE (linear 0x001C03BE) | static, data | _current_lbx_entry | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x0004C0F0 (linear 0x001BC0F0) | lbx_buffer | unsigned char[17000] |
| 2:0x00050358 (linear 0x001C0358) | current_lbx_file_name | char[100] |
| 2:0x000503BC (linear 0x001C03BC) | max_lbx_file_count | short |
| 2:0x000503BE (linear 0x001C03BE) | current_lbx_entry | short |

#### LBX_Open_File

- Address: 1:0x0014001F (linear 0x0015001F), 336 bytes, in `regions/07-object-1/001D5654-001D5E53.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| lbx_name | [EBP-0x14] | char * |
| LBX_fptr | [EBP-0x1C] | FILE * |
| i | [EBP-0x18] | int |
| magic_number | [EBP-0x4] | int |
| lbx_file_count | [EBP-0x10] | int |
| file_count | [EBP-0xC] | int |
| temp | [EBP-0x8] | long |
| buffer | [EBP-0x11C] | char[256] |

#### LBX_Add_File

- Address: 1:0x0014016F (linear 0x0015016F), 549 bytes, in `regions/07-object-1/001D5654-001D5E53.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| entry_file_name | [EBP-0x30] | char * |
| LBX_fptr | [EBP-0x34] | FILE * |
| entry_fptr | [EBP-0x14] | FILE * |
| LBX_len | [EBP-0x2C] | unsigned long |
| entry_len | [EBP-0x28] | unsigned long |
| foffset | [EBP-0x24] | unsigned long |
| next_offset | [EBP-0x20] | unsigned long |
| temp | [EBP-0x1C] | unsigned long |
| file_pos | [EBP-0x18] | unsigned long |
| temp_buffer | [EBP-0x98] | char[100] |
| i | [EBP-0x10] | int |
| file_size | [EBP-0xC] | unsigned int |
| current_pos | [EBP-0x8] | unsigned int |
| buffer_lbx | [EBP-0x4] | unsigned char * |

#### LBX_Add_Data_File

- Address: 1:0x00140394 (linear 0x00150394), 545 bytes, in `regions/07-object-1/001D5654-001D5E53.txt`
- Type: void (char *, long, int, int)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| entry_file_name | [EBP-0x2C] | char * |
| starting_offset | [EBP-0x3C] | long |
| record_count | [EBP-0x38] | int |
| record_size | [EBP-0x34] | int |
| LBX_fptr | [EBP-0x30] | FILE * |
| entry_fptr | [EBP-0xC] | FILE * |
| LBX_len | [EBP-0x28] | long |
| entry_len | [EBP-0x24] | long |
| foffset | [EBP-0x20] | long |
| next_offset | [EBP-0x1C] | long |
| temp | [EBP-0x18] | long |
| file_pos | [EBP-0x14] | long |
| entry_length | [EBP-0x10] | long |
| temp_buffer | [EBP-0xA0] | char[100] |
| i | [EBP-0x8] | int |
| sizer | [EBP-0x4] | int |

#### LBX_Extract_File

- Address: 1:0x001405B5 (linear 0x001505B5), 479 bytes, in `regions/07-object-1/001D5654-001D5E53.txt`
- Type: void (char *, int)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| source_file_name | [EBP-0x2C] | char * |
| entry_num | [EBP-0x28] | int |
| LBX_fptr | [EBP-0x30] | FILE * |
| source_fptr | [EBP-0x34] | FILE * |
| LBX_len | [EBP-0x10] | long |
| entry_len | [EBP-0xC] | long |
| foffset | [EBP-0x8] | long |
| next_offset | [EBP-0x24] | long |
| temp | [EBP-0x20] | long |
| file_pos | [EBP-0x1C] | long |
| start_pos | [EBP-0x18] | long |
| end_pos | [EBP-0x14] | long |
| temp_buffer | [EBP-0xD8] | char[100] |
| entry_name | [EBP-0x54] | char[30] |
| entry_description | [EBP-0x74] | char[30] |
| i | [EBP-0x4] | int |

#### Read_LBX_File_Type

- Address: 1:0x00140794 (linear 0x00150794), 105 bytes, in `regions/07-object-1/001D5654-001D5E53.txt`
- Type: int (char *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| lbx_name | [EBP-0x10] | char * |
| LBX_fptr | [EBP-0xC] | FILE * |
| file_type | [EBP-0x8] | int |

#### Read_LBX_File_Entry_Count

- Address: 1:0x001407FD (linear 0x001507FD), 102 bytes, in `regions/07-object-1/001D5654-001D5E53.txt`
- Type: int (char *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| lbx_name | [EBP-0x10] | char * |
| LBX_fptr | [EBP-0xC] | FILE * |
| file_count | [EBP-0x8] | int |

#### Repeat_LBX_File

- Address: 1:0x00140863 (linear 0x00150863), 200 bytes, in `regions/07-object-1/001D5E54-001D6653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| LBX_fptr | [EBP-0x28] | FILE * |
| entry_fptr | [EBP-0x1C] | FILE * |
| LBX_len | [EBP-0x24] | long |
| entry_len | [EBP-0x20] | long |
| foffset | [EBP-0x8] | long |
| next_offset | [EBP-0x18] | long |
| temp | [EBP-0x14] | long |
| file_pos | [EBP-0x10] | long |
| coffset | [EBP-0xC] | long |
| temp_buffer | [EBP-0x8C] | char[100] |
| i | [EBP-0x4] | int |

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
| 30 | array |  | 0..16999; of unsigned char (#5) |
| 31 | array |  | 0..99; of char (#10) |
| 32 | scalar | void | void 1 bytes |
| 33 | procedure |  | near386; returns void (char *) |
| 34 | pointer |  | near386; of unsigned char (#5) |
| 35 | struct | struct __iobuf | 8 fields, 26 bytes |
| 36 | typedef | __iobuf | struct name for struct #35 (#35) |
| 37 | typedef | FILE | name for struct __iobuf (#36) |
| 38 | pointer |  | near386; of FILE (#37) |
| 39 | array |  | 0..255; of char (#10) |
| 40 | scalar | unsigned long | unsigned 4 bytes |
| 41 | array |  | 0..99; of char (#10) |
| 42 | procedure |  | near386; returns void (char *, long, int, int) |
| 43 | array |  | 0..99; of char (#10) |
| 44 | procedure |  | near386; returns void (char *, int) |
| 45 | array |  | 0..99; of char (#10) |
| 46 | array |  | 0..29; of char (#10) |
| 47 | array |  | 0..29; of char (#10) |
| 48 | procedure |  | near386; returns int (char *) |
| 49 | procedure |  | near386; returns void () |
| 50 | array |  | 0..99; of char (#10) |

##### struct __iobuf (#35, 8 fields, 26 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| _ptr | offset 0 | unsigned char * |
| _cnt | offset 4 | int |
| _base | offset 8 | unsigned char * |
| _flag | offset 12 | unsigned int |
| _handle | offset 16 | int |
| _bufsize | offset 20 | unsigned int |
| _ungotten | offset 24 | unsigned char |
| _tmpfchar | offset 25 | unsigned char |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 9 | 1:0x0014001F (linear 0x0015001F) | 0x001D5673 |
| 16 | 1:0x0014003A (linear 0x0015003A) | 0x001D568E |
| 18 | 1:0x00140047 (linear 0x00150047) | 0x001D569B |
| 19 | 1:0x0014006A (linear 0x0015006A) | 0x001D56BE |
| 20 | 1:0x0014007D (linear 0x0015007D) | 0x001D56D1 |
| 21 | 1:0x00140089 (linear 0x00150089) | 0x001D56DD |
| 23 | 1:0x00140098 (linear 0x00150098) | 0x001D56EC |
| 25 | 1:0x001400BF (linear 0x001500BF) | 0x001D5713 |
| 27 | 1:0x001400CF (linear 0x001500CF) | 0x001D5723 |
| 28 | 1:0x001400D6 (linear 0x001500D6) | 0x001D572A |
| 30 | 1:0x001400EB (linear 0x001500EB) | 0x001D573F |
| 31 | 1:0x001400F2 (linear 0x001500F2) | 0x001D5746 |
| 33 | 1:0x00140107 (linear 0x00150107) | 0x001D575B |
| 35 | 1:0x0014011E (linear 0x0015011E) | 0x001D5772 |
| 37 | 1:0x00140135 (linear 0x00150135) | 0x001D5789 |
| 39 | 1:0x0014013E (linear 0x0015013E) | 0x001D5792 |
| 41 | 1:0x00140155 (linear 0x00150155) | 0x001D57A9 |
| 43 | 1:0x0014015E (linear 0x0015015E) | 0x001D57B2 |
| 44 | 1:0x00140166 (linear 0x00150166) | 0x001D57BA |
| 47 | 1:0x0014016F (linear 0x0015016F) | 0x001D57C3 |
| 56 | 1:0x0014018A (linear 0x0015018A) | 0x001D57DE |
| 58 | 1:0x001401A3 (linear 0x001501A3) | 0x001D57F7 |
| 60 | 1:0x001401AA (linear 0x001501AA) | 0x001D57FE |
| 61 | 1:0x001401B7 (linear 0x001501B7) | 0x001D580B |
| 62 | 1:0x001401C9 (linear 0x001501C9) | 0x001D581D |
| 63 | 1:0x001401D4 (linear 0x001501D4) | 0x001D5828 |
| 65 | 1:0x001401E6 (linear 0x001501E6) | 0x001D583A |
| 66 | 1:0x001401F8 (linear 0x001501F8) | 0x001D584C |
| 70 | 1:0x00140208 (linear 0x00150208) | 0x001D585C |
| 71 | 1:0x00140219 (linear 0x00150219) | 0x001D586D |
| 72 | 1:0x00140226 (linear 0x00150226) | 0x001D587A |
| 74 | 1:0x0014023B (linear 0x0015023B) | 0x001D588F |
| 75 | 1:0x00140244 (linear 0x00150244) | 0x001D5898 |
| 79 | 1:0x00140259 (linear 0x00150259) | 0x001D58AD |
| 80 | 1:0x00140260 (linear 0x00150260) | 0x001D58B4 |
| 82 | 1:0x00140270 (linear 0x00150270) | 0x001D58C4 |
| 83 | 1:0x0014027D (linear 0x0015027D) | 0x001D58D1 |
| 85 | 1:0x00140286 (linear 0x00150286) | 0x001D58DA |
| 86 | 1:0x0014029B (linear 0x0015029B) | 0x001D58EF |
| 87 | 1:0x001402B0 (linear 0x001502B0) | 0x001D5904 |
| 88 | 1:0x001402BB (linear 0x001502BB) | 0x001D590F |
| 90 | 1:0x001402C2 (linear 0x001502C2) | 0x001D5916 |
| 91 | 1:0x001402D8 (linear 0x001502D8) | 0x001D592C |
| 93 | 1:0x001402DA (linear 0x001502DA) | 0x001D592E |
| 95 | 1:0x001402E0 (linear 0x001502E0) | 0x001D5934 |
| 96 | 1:0x001402F3 (linear 0x001502F3) | 0x001D5947 |
| 97 | 1:0x00140306 (linear 0x00150306) | 0x001D595A |
| 100 | 1:0x0014030C (linear 0x0015030C) | 0x001D5960 |
| 101 | 1:0x00140314 (linear 0x00150314) | 0x001D5968 |
| 103 | 1:0x00140326 (linear 0x00150326) | 0x001D597A |
| 104 | 1:0x0014032D (linear 0x0015032D) | 0x001D5981 |
| 105 | 1:0x00140339 (linear 0x00150339) | 0x001D598D |
| 107 | 1:0x00140350 (linear 0x00150350) | 0x001D59A4 |
| 108 | 1:0x00140357 (linear 0x00150357) | 0x001D59AB |
| 109 | 1:0x00140366 (linear 0x00150366) | 0x001D59BA |
| 111 | 1:0x0014037B (linear 0x0015037B) | 0x001D59CF |
| 112 | 1:0x00140383 (linear 0x00150383) | 0x001D59D7 |
| 113 | 1:0x0014038B (linear 0x0015038B) | 0x001D59DF |
| 116 | 1:0x00140394 (linear 0x00150394) | 0x001D59E8 |
| 124 | 1:0x001403B5 (linear 0x001503B5) | 0x001D5A09 |
| 126 | 1:0x001403CE (linear 0x001503CE) | 0x001D5A22 |
| 128 | 1:0x001403D5 (linear 0x001503D5) | 0x001D5A29 |
| 129 | 1:0x001403E2 (linear 0x001503E2) | 0x001D5A36 |
| 131 | 1:0x001403ED (linear 0x001503ED) | 0x001D5A41 |
| 132 | 1:0x001403FC (linear 0x001503FC) | 0x001D5A50 |
| 134 | 1:0x00140406 (linear 0x00150406) | 0x001D5A5A |
| 136 | 1:0x00140410 (linear 0x00150410) | 0x001D5A64 |
| 137 | 1:0x00140422 (linear 0x00150422) | 0x001D5A76 |
| 141 | 1:0x00140432 (linear 0x00150432) | 0x001D5A86 |
| 142 | 1:0x00140443 (linear 0x00150443) | 0x001D5A97 |
| 143 | 1:0x00140450 (linear 0x00150450) | 0x001D5AA4 |
| 145 | 1:0x00140465 (linear 0x00150465) | 0x001D5AB9 |
| 146 | 1:0x00140471 (linear 0x00150471) | 0x001D5AC5 |
| 152 | 1:0x00140486 (linear 0x00150486) | 0x001D5ADA |
| 153 | 1:0x0014048D (linear 0x0015048D) | 0x001D5AE1 |
| 154 | 1:0x0014049D (linear 0x0015049D) | 0x001D5AF1 |
| 156 | 1:0x001404AA (linear 0x001504AA) | 0x001D5AFE |
| 157 | 1:0x001404BF (linear 0x001504BF) | 0x001D5B13 |
| 159 | 1:0x001404D4 (linear 0x001504D4) | 0x001D5B28 |
| 161 | 1:0x001404DD (linear 0x001504DD) | 0x001D5B31 |
| 162 | 1:0x001404F4 (linear 0x001504F4) | 0x001D5B48 |
| 163 | 1:0x0014050B (linear 0x0015050B) | 0x001D5B5F |
| 164 | 1:0x00140512 (linear 0x00150512) | 0x001D5B66 |
| 166 | 1:0x00140514 (linear 0x00150514) | 0x001D5B68 |
| 168 | 1:0x0014051A (linear 0x0015051A) | 0x001D5B6E |
| 169 | 1:0x00140520 (linear 0x00150520) | 0x001D5B74 |
| 170 | 1:0x00140535 (linear 0x00150535) | 0x001D5B89 |
| 173 | 1:0x0014054A (linear 0x0015054A) | 0x001D5B9E |
| 174 | 1:0x00140551 (linear 0x00150551) | 0x001D5BA5 |
| 175 | 1:0x0014055D (linear 0x0015055D) | 0x001D5BB1 |
| 177 | 1:0x00140574 (linear 0x00150574) | 0x001D5BC8 |
| 178 | 1:0x0014057B (linear 0x0015057B) | 0x001D5BCF |
| 179 | 1:0x0014058A (linear 0x0015058A) | 0x001D5BDE |
| 181 | 1:0x0014059F (linear 0x0015059F) | 0x001D5BF3 |
| 182 | 1:0x001405A7 (linear 0x001505A7) | 0x001D5BFB |
| 183 | 1:0x001405AF (linear 0x001505AF) | 0x001D5C03 |
| 194 | 1:0x001405B5 (linear 0x001505B5) | 0x001D5C09 |
| 202 | 1:0x001405D2 (linear 0x001505D2) | 0x001D5C26 |
| 204 | 1:0x001405EB (linear 0x001505EB) | 0x001D5C3F |
| 206 | 1:0x001405F2 (linear 0x001505F2) | 0x001D5C46 |
| 207 | 1:0x001405FF (linear 0x001505FF) | 0x001D5C53 |
| 209 | 1:0x00140611 (linear 0x00150611) | 0x001D5C65 |
| 210 | 1:0x00140621 (linear 0x00150621) | 0x001D5C75 |
| 211 | 1:0x0014062D (linear 0x0015062D) | 0x001D5C81 |
| 212 | 1:0x0014063A (linear 0x0015063A) | 0x001D5C8E |
| 213 | 1:0x0014064F (linear 0x0015064F) | 0x001D5CA3 |
| 215 | 1:0x00140664 (linear 0x00150664) | 0x001D5CB8 |
| 216 | 1:0x0014066D (linear 0x0015066D) | 0x001D5CC1 |
| 220 | 1:0x0014067A (linear 0x0015067A) | 0x001D5CCE |
| 221 | 1:0x0014068B (linear 0x0015068B) | 0x001D5CDF |
| 222 | 1:0x00140698 (linear 0x00150698) | 0x001D5CEC |
| 224 | 1:0x001406AD (linear 0x001506AD) | 0x001D5D01 |
| 225 | 1:0x001406B6 (linear 0x001506B6) | 0x001D5D0A |
| 229 | 1:0x001406CB (linear 0x001506CB) | 0x001D5D1F |
| 230 | 1:0x001406D2 (linear 0x001506D2) | 0x001D5D26 |
| 232 | 1:0x001406E2 (linear 0x001506E2) | 0x001D5D36 |
| 234 | 1:0x001406EB (linear 0x001506EB) | 0x001D5D3F |
| 235 | 1:0x00140702 (linear 0x00150702) | 0x001D5D56 |
| 236 | 1:0x00140719 (linear 0x00150719) | 0x001D5D6D |
| 237 | 1:0x00140720 (linear 0x00150720) | 0x001D5D74 |
| 239 | 1:0x00140722 (linear 0x00150722) | 0x001D5D76 |
| 241 | 1:0x00140728 (linear 0x00150728) | 0x001D5D7C |
| 242 | 1:0x0014073D (linear 0x0015073D) | 0x001D5D91 |
| 245 | 1:0x00140752 (linear 0x00150752) | 0x001D5DA6 |
| 246 | 1:0x00140759 (linear 0x00150759) | 0x001D5DAD |
| 247 | 1:0x00140765 (linear 0x00150765) | 0x001D5DB9 |
| 249 | 1:0x0014077C (linear 0x0015077C) | 0x001D5DD0 |
| 250 | 1:0x00140784 (linear 0x00150784) | 0x001D5DD8 |
| 251 | 1:0x0014078C (linear 0x0015078C) | 0x001D5DE0 |
| 260 | 1:0x00140794 (linear 0x00150794) | 0x001D5DE8 |
| 265 | 1:0x001407AF (linear 0x001507AF) | 0x001D5E03 |
| 266 | 1:0x001407BF (linear 0x001507BF) | 0x001D5E13 |
| 267 | 1:0x001407CE (linear 0x001507CE) | 0x001D5E22 |
| 268 | 1:0x001407E3 (linear 0x001507E3) | 0x001D5E37 |
| 270 | 1:0x001407EB (linear 0x001507EB) | 0x001D5E3F |
| 271 | 1:0x001407F1 (linear 0x001507F1) | 0x001D5E45 |
| 274 | 1:0x001407FD (linear 0x001507FD) | 0x001D5E51 |
| 279 | 1:0x00140818 (linear 0x00150818) | 0x001D5E6C |
| 280 | 1:0x00140828 (linear 0x00150828) | 0x001D5E7C |
| 281 | 1:0x00140834 (linear 0x00150834) | 0x001D5E88 |
| 282 | 1:0x00140849 (linear 0x00150849) | 0x001D5E9D |
| 284 | 1:0x00140851 (linear 0x00150851) | 0x001D5EA5 |
| 285 | 1:0x00140857 (linear 0x00150857) | 0x001D5EAB |
| 291 | 1:0x00140863 (linear 0x00150863) | 0x001D5EB7 |
| 298 | 1:0x0014087B (linear 0x0015087B) | 0x001D5ECF |
| 299 | 1:0x00140888 (linear 0x00150888) | 0x001D5EDC |
| 303 | 1:0x0014089A (linear 0x0015089A) | 0x001D5EEE |
| 304 | 1:0x001408AB (linear 0x001508AB) | 0x001D5EFF |
| 306 | 1:0x001408B8 (linear 0x001508B8) | 0x001D5F0C |
| 308 | 1:0x001408CD (linear 0x001508CD) | 0x001D5F21 |
| 309 | 1:0x001408D3 (linear 0x001508D3) | 0x001D5F27 |
| 312 | 1:0x001408E8 (linear 0x001508E8) | 0x001D5F3C |
| 313 | 1:0x001408EF (linear 0x001508EF) | 0x001D5F43 |
| 314 | 1:0x001408F6 (linear 0x001508F6) | 0x001D5F4A |
| 315 | 1:0x00140903 (linear 0x00150903) | 0x001D5F57 |
| 319 | 1:0x0014091A (linear 0x0015091A) | 0x001D5F6E |
| 320 | 1:0x00140922 (linear 0x00150922) | 0x001D5F76 |

