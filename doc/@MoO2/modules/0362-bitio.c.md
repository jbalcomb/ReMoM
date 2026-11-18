# F:\WLIBS\compress\bitio.c

Module 362 of the debug info of ORION2.EXE.

Ranges: 1:0x001522EE (linear 0x001622EE), 3,885 bytes, 2:0x00004C10 (linear 0x00174C10), 92 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x001522EE (linear 0x001622EE) | code | OpenOutputBitBuffer_ | `regions/07-object-1/001E7654-001E7E53.txt` |
| 1:0x00152359 (linear 0x00162359) | code | OpenInputBitBuffer_ | `regions/07-object-1/001E7654-001E7E53.txt` |
| 1:0x001523C4 (linear 0x001623C4) | code | CloseOutputBitBuffer_ | `regions/07-object-1/001E7654-001E7E53.txt` |
| 1:0x0015242D (linear 0x0016242D) | code | CloseInputBitBuffer_ | `regions/07-object-1/001E7654-001E7E53.txt` |
| 1:0x00152459 (linear 0x00162459) | code | BufferOutputBit_ | `regions/07-object-1/001E7654-001E7E53.txt` |
| 1:0x00152559 (linear 0x00162559) | code | BufferOutputBits_ | `regions/07-object-1/001E7654-001E7E53.txt` |
| 1:0x0015267C (linear 0x0016267C) | code | BufferInputBit_ | `regions/07-object-1/001E7654-001E7E53.txt` |
| 1:0x001527A5 (linear 0x001627A5) | code | BufferInputBits_ | `regions/07-object-1/001E7654-001E7E53.txt` |
| 1:0x001528E3 (linear 0x001628E3) | code | OpenOutputBitFile_ | `regions/07-object-1/001E7E54-001E8653.txt` |
| 1:0x0015295F (linear 0x0016295F) | code | OpenInputBitFile_ | `regions/07-object-1/001E7E54-001E8653.txt` |
| 1:0x001529DB (linear 0x001629DB) | code | CloseOutputBitFile_ | `regions/07-object-1/001E7E54-001E8653.txt` |
| 1:0x00152AD0 (linear 0x00162AD0) | code | CloseInputBitFile_ | `regions/07-object-1/001E7E54-001E8653.txt` |
| 1:0x00152B06 (linear 0x00162B06) | code | OutputBit_ | `regions/07-object-1/001E7E54-001E8653.txt` |
| 1:0x00152CA3 (linear 0x00162CA3) | code | OutputBits_ | `regions/07-object-1/001E7E54-001E8653.txt` |
| 1:0x00152E63 (linear 0x00162E63) | code | InputBit_ | `regions/07-object-1/001E7E54-001E8653.txt` |
| 1:0x00153003 (linear 0x00163003) | code | InputBits_ | `regions/07-object-1/001E8654-001E8E53.txt` |
| 1:0x001531B8 (linear 0x001631B8) | code | FilePrintBinary_ | `regions/07-object-1/001E8654-001E8E53.txt` |

#### OpenOutputBitBuffer

- Address: 1:0x001522EE (linear 0x001622EE), 107 bytes, in `regions/07-object-1/001E7654-001E7E53.txt`
- Type: BIT_BUFFER * (char *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| name | [EBP-0xC] | char * |
| bit_buffer | [EBP-0x8] | BIT_BUFFER * |

#### OpenInputBitBuffer

- Address: 1:0x00152359 (linear 0x00162359), 107 bytes, in `regions/07-object-1/001E7654-001E7E53.txt`
- Type: BIT_BUFFER * (char *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| name | [EBP-0xC] | char * |
| bit_buffer | [EBP-0x8] | BIT_BUFFER * |

#### CloseOutputBitBuffer

- Address: 1:0x001523C4 (linear 0x001623C4), 105 bytes, in `regions/07-object-1/001E7654-001E7E53.txt`
- Type: int (BIT_BUFFER *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| bit_buffer | [EBP-0xC] | BIT_BUFFER * |
| size | [EBP-0x8] | int |

#### CloseInputBitBuffer

- Address: 1:0x0015242D (linear 0x0016242D), 44 bytes, in `regions/07-object-1/001E7654-001E7E53.txt`
- Type: void (BIT_BUFFER *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| bit_buffer | [EBP-0x4] | BIT_BUFFER * |

#### BufferOutputBit

- Address: 1:0x00152459 (linear 0x00162459), 256 bytes, in `regions/07-object-1/001E7654-001E7E53.txt`
- Type: void (BIT_BUFFER *, int)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| bit_buffer | [EBP-0x8] | BIT_BUFFER * |
| bit | [EBP-0x4] | int |

#### BufferOutputBits

- Address: 1:0x00152559 (linear 0x00162559), 291 bytes, in `regions/07-object-1/001E7654-001E7E53.txt`
- Type: void (BIT_BUFFER *, unsigned long, int)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| bit_buffer | [EBP-0x10] | BIT_BUFFER * |
| code | [EBP-0xC] | unsigned long |
| count | [EBP-0x8] | int |
| mask | [EBP-0x4] | unsigned long |

#### BufferInputBit

- Address: 1:0x0015267C (linear 0x0016267C), 297 bytes, in `regions/07-object-1/001E7654-001E7E53.txt`
- Type: int (BIT_BUFFER *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| bit_buffer | [EBP-0xC] | BIT_BUFFER * |
| value | [EBP-0x8] | int |

#### BufferInputBits

- Address: 1:0x001527A5 (linear 0x001627A5), 318 bytes, in `regions/07-object-1/001E7654-001E7E53.txt`
- Type: unsigned long (BIT_BUFFER *, int); returns in EAX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| bit_buffer | [EBP-0x14] | BIT_BUFFER * |
| bit_count | [EBP-0x10] | int |
| mask | [EBP-0xC] | unsigned long |
| return_value | [EBP-0x8] | unsigned long |

#### OpenOutputBitFile

- Address: 1:0x001528E3 (linear 0x001628E3), 124 bytes, in `regions/07-object-1/001E7E54-001E8653.txt`
- Type: BIT_FILE * (char *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| name | [EBP-0xC] | char * |
| bit_file | [EBP-0x8] | BIT_FILE * |

#### OpenInputBitFile

- Address: 1:0x0015295F (linear 0x0016295F), 124 bytes, in `regions/07-object-1/001E7E54-001E8653.txt`
- Type: BIT_FILE * (char *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| name | [EBP-0xC] | char * |
| bit_file | [EBP-0x8] | BIT_FILE * |

#### CloseOutputBitFile

- Address: 1:0x001529DB (linear 0x001629DB), 245 bytes, in `regions/07-object-1/001E7E54-001E8653.txt`
- Type: void (BIT_FILE *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| bit_file | [EBP-0x4] | BIT_FILE * |

#### CloseInputBitFile

- Address: 1:0x00152AD0 (linear 0x00162AD0), 54 bytes, in `regions/07-object-1/001E7E54-001E8653.txt`
- Type: void (BIT_FILE *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| bit_file | [EBP-0x4] | BIT_FILE * |

#### OutputBit

- Address: 1:0x00152B06 (linear 0x00162B06), 413 bytes, in `regions/07-object-1/001E7E54-001E8653.txt`
- Type: void (BIT_FILE *, int)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| bit_file | [EBP-0x8] | BIT_FILE * |
| bit | [EBP-0x4] | int |

#### OutputBits

- Address: 1:0x00152CA3 (linear 0x00162CA3), 448 bytes, in `regions/07-object-1/001E7E54-001E8653.txt`
- Type: void (BIT_FILE *, unsigned long, int)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| bit_file | [EBP-0x10] | BIT_FILE * |
| code | [EBP-0xC] | unsigned long |
| count | [EBP-0x8] | int |
| mask | [EBP-0x4] | unsigned long |

#### InputBit

- Address: 1:0x00152E63 (linear 0x00162E63), 416 bytes, in `regions/07-object-1/001E7E54-001E8653.txt`
- Type: int (BIT_FILE *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| bit_file | [EBP-0xC] | BIT_FILE * |
| value | [EBP-0x8] | int |

#### InputBits

- Address: 1:0x00153003 (linear 0x00163003), 437 bytes, in `regions/07-object-1/001E8654-001E8E53.txt`
- Type: unsigned long (BIT_FILE *, int); returns in EAX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| bit_file | [EBP-0x14] | BIT_FILE * |
| bit_count | [EBP-0x10] | int |
| mask | [EBP-0xC] | unsigned long |
| return_value | [EBP-0x8] | unsigned long |

#### FilePrintBinary

- Address: 1:0x001531B8 (linear 0x001631B8), 99 bytes, in `regions/07-object-1/001E8654-001E8E53.txt`
- Type: void (FILE *, unsigned int, int)
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| file | [EBP-0x10] | FILE * |
| code | [EBP-0xC] | unsigned int |
| bits | [EBP-0x8] | int |
| mask | [EBP-0x4] | unsigned int |

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
| 25 | struct | struct bit_buffer | 4 fields, 13 bytes |
| 26 | typedef | bit_buffer | struct name for struct #25 (#25) |
| 27 | typedef | BIT_BUFFER | name for struct bit_buffer (#26) |
| 28 | pointer |  | near386; of BIT_BUFFER (#27) |
| 29 | procedure |  | near386; returns BIT_BUFFER * (char *) |
| 30 | procedure |  | near386; returns int (BIT_BUFFER *) |
| 31 | procedure |  | near386; returns void (BIT_BUFFER *) |
| 32 | procedure |  | near386; returns void (BIT_BUFFER *, int) |
| 33 | scalar | unsigned long | unsigned 4 bytes |
| 34 | procedure |  | near386; returns void (BIT_BUFFER *, unsigned long, int) |
| 35 | procedure |  | near386; returns unsigned long (BIT_BUFFER *, int) |
| 36 | pointer |  | near386; of unsigned char (#5) |
| 37 | struct | struct __iobuf | 8 fields, 26 bytes |
| 38 | typedef | __iobuf | struct name for struct #37 (#37) |
| 39 | typedef | FILE | name for struct __iobuf (#38) |
| 40 | pointer |  | near386; of FILE (#39) |
| 41 | struct | struct bit_file | 4 fields, 13 bytes |
| 42 | typedef | bit_file | struct name for struct #41 (#41) |
| 43 | typedef | BIT_FILE | name for struct bit_file (#42) |
| 44 | pointer |  | near386; of BIT_FILE (#43) |
| 45 | procedure |  | near386; returns BIT_FILE * (char *) |
| 46 | procedure |  | near386; returns void (BIT_FILE *) |
| 47 | procedure |  | near386; returns void (BIT_FILE *, int) |
| 48 | procedure |  | near386; returns void (BIT_FILE *, unsigned long, int) |
| 49 | procedure |  | near386; returns int (BIT_FILE *) |
| 50 | procedure |  | near386; returns unsigned long (BIT_FILE *, int) |
| 51 | procedure |  | near386; returns void (FILE *, unsigned int, int) |

##### struct bit_buffer (#25, 4 fields, 13 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| buffer | offset 0 | char * |
| mask | offset 4 | unsigned char |
| rack | offset 5 | int |
| pacifier_counter | offset 9 | int |

##### struct __iobuf (#37, 8 fields, 26 bytes)

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

##### struct bit_file (#41, 4 fields, 13 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| file | offset 0 | FILE * |
| mask | offset 4 | unsigned char |
| rack | offset 5 | int |
| pacifier_counter | offset 9 | int |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 16 | 1:0x001522EE (linear 0x001622EE) | 0x001E7942 |
| 20 | 1:0x00152309 (linear 0x00162309) | 0x001E795D |
| 21 | 1:0x00152316 (linear 0x00162316) | 0x001E796A |
| 23 | 1:0x0015231C (linear 0x0016231C) | 0x001E7970 |
| 25 | 1:0x00152324 (linear 0x00162324) | 0x001E7978 |
| 26 | 1:0x0015232C (linear 0x0016232C) | 0x001E7980 |
| 27 | 1:0x00152336 (linear 0x00162336) | 0x001E798A |
| 28 | 1:0x0015233D (linear 0x0016233D) | 0x001E7991 |
| 29 | 1:0x00152347 (linear 0x00162347) | 0x001E799B |
| 30 | 1:0x0015234D (linear 0x0016234D) | 0x001E79A1 |
| 31 | 1:0x00152359 (linear 0x00162359) | 0x001E79AD |
| 35 | 1:0x00152374 (linear 0x00162374) | 0x001E79C8 |
| 36 | 1:0x00152381 (linear 0x00162381) | 0x001E79D5 |
| 38 | 1:0x00152387 (linear 0x00162387) | 0x001E79DB |
| 40 | 1:0x0015238F (linear 0x0016238F) | 0x001E79E3 |
| 41 | 1:0x00152397 (linear 0x00162397) | 0x001E79EB |
| 42 | 1:0x001523A1 (linear 0x001623A1) | 0x001E79F5 |
| 43 | 1:0x001523A8 (linear 0x001623A8) | 0x001E79FC |
| 44 | 1:0x001523B2 (linear 0x001623B2) | 0x001E7A06 |
| 45 | 1:0x001523B8 (linear 0x001623B8) | 0x001E7A0C |
| 47 | 1:0x001523C4 (linear 0x001623C4) | 0x001E7A18 |
| 51 | 1:0x001523DF (linear 0x001623DF) | 0x001E7A33 |
| 53 | 1:0x001523F1 (linear 0x001623F1) | 0x001E7A45 |
| 54 | 1:0x00152404 (linear 0x00162404) | 0x001E7A58 |
| 56 | 1:0x0015240A (linear 0x0016240A) | 0x001E7A5E |
| 57 | 1:0x00152413 (linear 0x00162413) | 0x001E7A67 |
| 58 | 1:0x0015241B (linear 0x0016241B) | 0x001E7A6F |
| 59 | 1:0x00152421 (linear 0x00162421) | 0x001E7A75 |
| 61 | 1:0x0015242D (linear 0x0016242D) | 0x001E7A81 |
| 63 | 1:0x00152448 (linear 0x00162448) | 0x001E7A9C |
| 64 | 1:0x00152450 (linear 0x00162450) | 0x001E7AA4 |
| 67 | 1:0x00152459 (linear 0x00162459) | 0x001E7AAD |
| 69 | 1:0x00152476 (linear 0x00162476) | 0x001E7ACA |
| 71 | 1:0x0015247C (linear 0x0016247C) | 0x001E7AD0 |
| 73 | 1:0x0015248A (linear 0x0016248A) | 0x001E7ADE |
| 75 | 1:0x00152490 (linear 0x00162490) | 0x001E7AE4 |
| 77 | 1:0x0015249D (linear 0x0016249D) | 0x001E7AF1 |
| 78 | 1:0x001524B0 (linear 0x001624B0) | 0x001E7B04 |
| 80 | 1:0x001524B6 (linear 0x001624B6) | 0x001E7B0A |
| 82 | 1:0x001524C5 (linear 0x001624C5) | 0x001E7B19 |
| 85 | 1:0x00152540 (linear 0x00162540) | 0x001E7B94 |
| 86 | 1:0x0015254A (linear 0x0016254A) | 0x001E7B9E |
| 88 | 1:0x00152551 (linear 0x00162551) | 0x001E7BA5 |
| 90 | 1:0x00152559 (linear 0x00162559) | 0x001E7BAD |
| 94 | 1:0x00152578 (linear 0x00162578) | 0x001E7BCC |
| 95 | 1:0x00152586 (linear 0x00162586) | 0x001E7BDA |
| 97 | 1:0x00152590 (linear 0x00162590) | 0x001E7BE4 |
| 99 | 1:0x00152598 (linear 0x00162598) | 0x001E7BEC |
| 101 | 1:0x001525A6 (linear 0x001625A6) | 0x001E7BFA |
| 102 | 1:0x001525AC (linear 0x001625AC) | 0x001E7C00 |
| 104 | 1:0x001525B9 (linear 0x001625B9) | 0x001E7C0D |
| 105 | 1:0x001525CC (linear 0x001625CC) | 0x001E7C20 |
| 106 | 1:0x001525D2 (linear 0x001625D2) | 0x001E7C26 |
| 108 | 1:0x001525E1 (linear 0x001625E1) | 0x001E7C35 |
| 110 | 1:0x0015265C (linear 0x0016265C) | 0x001E7CB0 |
| 111 | 1:0x00152666 (linear 0x00162666) | 0x001E7CBA |
| 113 | 1:0x0015266D (linear 0x0016266D) | 0x001E7CC1 |
| 114 | 1:0x00152670 (linear 0x00162670) | 0x001E7CC4 |
| 115 | 1:0x00152675 (linear 0x00162675) | 0x001E7CC9 |
| 117 | 1:0x0015267C (linear 0x0016267C) | 0x001E7CD0 |
| 121 | 1:0x00152697 (linear 0x00162697) | 0x001E7CEB |
| 123 | 1:0x001526AD (linear 0x001626AD) | 0x001E7D01 |
| 124 | 1:0x001526C4 (linear 0x001626C4) | 0x001E7D18 |
| 126 | 1:0x001526CA (linear 0x001626CA) | 0x001E7D1E |
| 128 | 1:0x001526D9 (linear 0x001626D9) | 0x001E7D2D |
| 131 | 1:0x00152754 (linear 0x00162754) | 0x001E7DA8 |
| 132 | 1:0x00152767 (linear 0x00162767) | 0x001E7DBB |
| 133 | 1:0x0015276D (linear 0x0016276D) | 0x001E7DC1 |
| 135 | 1:0x00152776 (linear 0x00162776) | 0x001E7DCA |
| 137 | 1:0x0015277D (linear 0x0016277D) | 0x001E7DD1 |
| 138 | 1:0x00152799 (linear 0x00162799) | 0x001E7DED |
| 140 | 1:0x001527A5 (linear 0x001627A5) | 0x001E7DF9 |
| 145 | 1:0x001527C2 (linear 0x001627C2) | 0x001E7E16 |
| 146 | 1:0x001527D0 (linear 0x001627D0) | 0x001E7E24 |
| 147 | 1:0x001527D7 (linear 0x001627D7) | 0x001E7E2B |
| 149 | 1:0x001527E1 (linear 0x001627E1) | 0x001E7E35 |
| 151 | 1:0x001527F7 (linear 0x001627F7) | 0x001E7E4B |
| 152 | 1:0x0015280E (linear 0x0016280E) | 0x001E7E62 |
| 154 | 1:0x00152814 (linear 0x00162814) | 0x001E7E68 |
| 156 | 1:0x00152823 (linear 0x00162823) | 0x001E7E77 |
| 159 | 1:0x0015289E (linear 0x0016289E) | 0x001E7EF2 |
| 161 | 1:0x001528AE (linear 0x001628AE) | 0x001E7F02 |
| 163 | 1:0x001528B4 (linear 0x001628B4) | 0x001E7F08 |
| 164 | 1:0x001528B7 (linear 0x001628B7) | 0x001E7F0B |
| 165 | 1:0x001528BD (linear 0x001628BD) | 0x001E7F11 |
| 167 | 1:0x001528C6 (linear 0x001628C6) | 0x001E7F1A |
| 169 | 1:0x001528CD (linear 0x001628CD) | 0x001E7F21 |
| 170 | 1:0x001528D2 (linear 0x001628D2) | 0x001E7F26 |
| 171 | 1:0x001528D8 (linear 0x001628D8) | 0x001E7F2C |
| 176 | 1:0x001528E3 (linear 0x001628E3) | 0x001E7F37 |
| 180 | 1:0x001528FE (linear 0x001628FE) | 0x001E7F52 |
| 181 | 1:0x00152910 (linear 0x00162910) | 0x001E7F64 |
| 182 | 1:0x00152916 (linear 0x00162916) | 0x001E7F6A |
| 183 | 1:0x0015291E (linear 0x0016291E) | 0x001E7F72 |
| 184 | 1:0x00152932 (linear 0x00162932) | 0x001E7F86 |
| 185 | 1:0x0015293C (linear 0x0016293C) | 0x001E7F90 |
| 186 | 1:0x00152943 (linear 0x00162943) | 0x001E7F97 |
| 187 | 1:0x0015294D (linear 0x0016294D) | 0x001E7FA1 |
| 188 | 1:0x00152953 (linear 0x00162953) | 0x001E7FA7 |
| 190 | 1:0x0015295F (linear 0x0016295F) | 0x001E7FB3 |
| 194 | 1:0x0015297A (linear 0x0016297A) | 0x001E7FCE |
| 195 | 1:0x0015298C (linear 0x0016298C) | 0x001E7FE0 |
| 196 | 1:0x00152992 (linear 0x00162992) | 0x001E7FE6 |
| 197 | 1:0x0015299A (linear 0x0016299A) | 0x001E7FEE |
| 198 | 1:0x001529AE (linear 0x001629AE) | 0x001E8002 |
| 199 | 1:0x001529B8 (linear 0x001629B8) | 0x001E800C |
| 200 | 1:0x001529BF (linear 0x001629BF) | 0x001E8013 |
| 201 | 1:0x001529C9 (linear 0x001629C9) | 0x001E801D |
| 202 | 1:0x001529CF (linear 0x001629CF) | 0x001E8023 |
| 204 | 1:0x001529DB (linear 0x001629DB) | 0x001E802F |
| 206 | 1:0x001529F6 (linear 0x001629F6) | 0x001E804A |
| 207 | 1:0x00152A0C (linear 0x00162A0C) | 0x001E8060 |
| 208 | 1:0x00152AA7 (linear 0x00162AA7) | 0x001E80FB |
| 209 | 1:0x00152AB5 (linear 0x00162AB5) | 0x001E8109 |
| 210 | 1:0x00152ABF (linear 0x00162ABF) | 0x001E8113 |
| 211 | 1:0x00152AC7 (linear 0x00162AC7) | 0x001E811B |
| 213 | 1:0x00152AD0 (linear 0x00162AD0) | 0x001E8124 |
| 215 | 1:0x00152AEB (linear 0x00162AEB) | 0x001E813F |
| 216 | 1:0x00152AF5 (linear 0x00162AF5) | 0x001E8149 |
| 217 | 1:0x00152AFD (linear 0x00162AFD) | 0x001E8151 |
| 219 | 1:0x00152B06 (linear 0x00162B06) | 0x001E815A |
| 221 | 1:0x00152B23 (linear 0x00162B23) | 0x001E8177 |
| 223 | 1:0x00152B29 (linear 0x00162B29) | 0x001E817D |
| 225 | 1:0x00152B37 (linear 0x00162B37) | 0x001E818B |
| 227 | 1:0x00152B3D (linear 0x00162B3D) | 0x001E8191 |
| 229 | 1:0x00152B4A (linear 0x00162B4A) | 0x001E819E |
| 231 | 1:0x00152BE5 (linear 0x00162BE5) | 0x001E8239 |
| 233 | 1:0x00152BF3 (linear 0x00162BF3) | 0x001E8247 |
| 235 | 1:0x00152BF8 (linear 0x00162BF8) | 0x001E824C |
| 237 | 1:0x00152C0F (linear 0x00162C0F) | 0x001E8263 |
| 240 | 1:0x00152C8A (linear 0x00162C8A) | 0x001E82DE |
| 241 | 1:0x00152C94 (linear 0x00162C94) | 0x001E82E8 |
| 243 | 1:0x00152C9B (linear 0x00162C9B) | 0x001E82EF |
| 245 | 1:0x00152CA3 (linear 0x00162CA3) | 0x001E82F7 |
| 249 | 1:0x00152CC2 (linear 0x00162CC2) | 0x001E8316 |
| 250 | 1:0x00152CD0 (linear 0x00162CD0) | 0x001E8324 |
| 251 | 1:0x00152CDA (linear 0x00162CDA) | 0x001E832E |
| 252 | 1:0x00152CE2 (linear 0x00162CE2) | 0x001E8336 |
| 253 | 1:0x00152CF0 (linear 0x00162CF0) | 0x001E8344 |
| 254 | 1:0x00152CF6 (linear 0x00162CF6) | 0x001E834A |
| 255 | 1:0x00152D03 (linear 0x00162D03) | 0x001E8357 |
| 256 | 1:0x00152D9E (linear 0x00162D9E) | 0x001E83F2 |
| 257 | 1:0x00152DAC (linear 0x00162DAC) | 0x001E8400 |
| 258 | 1:0x00152DC8 (linear 0x00162DC8) | 0x001E841C |
| 259 | 1:0x00152E43 (linear 0x00162E43) | 0x001E8497 |
| 260 | 1:0x00152E4D (linear 0x00162E4D) | 0x001E84A1 |
| 262 | 1:0x00152E54 (linear 0x00162E54) | 0x001E84A8 |
| 263 | 1:0x00152E57 (linear 0x00162E57) | 0x001E84AB |
| 264 | 1:0x00152E5C (linear 0x00162E5C) | 0x001E84B0 |
| 266 | 1:0x00152E63 (linear 0x00162E63) | 0x001E84B7 |
| 270 | 1:0x00152E7E (linear 0x00162E7E) | 0x001E84D2 |
| 271 | 1:0x00152E94 (linear 0x00162E94) | 0x001E84E8 |
| 272 | 1:0x00152F09 (linear 0x00162F09) | 0x001E855D |
| 273 | 1:0x00152F12 (linear 0x00162F12) | 0x001E8566 |
| 274 | 1:0x00152F20 (linear 0x00162F20) | 0x001E8574 |
| 275 | 1:0x00152F37 (linear 0x00162F37) | 0x001E858B |
| 277 | 1:0x00152FB2 (linear 0x00162FB2) | 0x001E8606 |
| 278 | 1:0x00152FC5 (linear 0x00162FC5) | 0x001E8619 |
| 279 | 1:0x00152FCB (linear 0x00162FCB) | 0x001E861F |
| 280 | 1:0x00152FD4 (linear 0x00162FD4) | 0x001E8628 |
| 281 | 1:0x00152FDB (linear 0x00162FDB) | 0x001E862F |
| 282 | 1:0x00152FF7 (linear 0x00162FF7) | 0x001E864B |
| 284 | 1:0x00153003 (linear 0x00163003) | 0x001E8657 |
| 289 | 1:0x00153020 (linear 0x00163020) | 0x001E8674 |
| 290 | 1:0x0015302E (linear 0x0016302E) | 0x001E8682 |
| 291 | 1:0x00153035 (linear 0x00163035) | 0x001E8689 |
| 292 | 1:0x0015303F (linear 0x0016303F) | 0x001E8693 |
| 293 | 1:0x00153055 (linear 0x00163055) | 0x001E86A9 |
| 294 | 1:0x001530CA (linear 0x001630CA) | 0x001E871E |
| 295 | 1:0x001530D3 (linear 0x001630D3) | 0x001E8727 |
| 296 | 1:0x001530E1 (linear 0x001630E1) | 0x001E8735 |
| 297 | 1:0x001530F8 (linear 0x001630F8) | 0x001E874C |
| 299 | 1:0x00153173 (linear 0x00163173) | 0x001E87C7 |
| 300 | 1:0x00153183 (linear 0x00163183) | 0x001E87D7 |
| 301 | 1:0x00153189 (linear 0x00163189) | 0x001E87DD |
| 302 | 1:0x0015318C (linear 0x0016318C) | 0x001E87E0 |
| 303 | 1:0x00153192 (linear 0x00163192) | 0x001E87E6 |
| 304 | 1:0x0015319B (linear 0x0016319B) | 0x001E87EF |
| 305 | 1:0x001531A2 (linear 0x001631A2) | 0x001E87F6 |
| 306 | 1:0x001531A7 (linear 0x001631A7) | 0x001E87FB |
| 307 | 1:0x001531AD (linear 0x001631AD) | 0x001E8801 |
| 309 | 1:0x001531B8 (linear 0x001631B8) | 0x001E880C |
| 313 | 1:0x001531D7 (linear 0x001631D7) | 0x001E882B |
| 314 | 1:0x001531E5 (linear 0x001631E5) | 0x001E8839 |
| 315 | 1:0x001531EB (linear 0x001631EB) | 0x001E883F |
| 316 | 1:0x001531F3 (linear 0x001631F3) | 0x001E8847 |
| 317 | 1:0x00153200 (linear 0x00163200) | 0x001E8854 |
| 318 | 1:0x00153202 (linear 0x00163202) | 0x001E8856 |
| 319 | 1:0x0015320F (linear 0x0016320F) | 0x001E8863 |
| 320 | 1:0x00153212 (linear 0x00163212) | 0x001E8866 |
| 321 | 1:0x00153214 (linear 0x00163214) | 0x001E8868 |

