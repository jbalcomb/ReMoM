# F:\WLIBS\compress\arith1e.c

Module 329 of the debug info of ORION2.EXE.

Ranges: 1:0x0014CA26 (linear 0x0015CA26), 4,264 bytes, 2:0x00004804 (linear 0x00174804), 116 bytes, 2:0x00012C40 (linear 0x00182C40), 8 bytes, 2:0x00051840 (linear 0x001C1840), 1,040 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0014CA26 (linear 0x0015CA26) | code | Compress_Data_ | `regions/07-object-1/001E1E54-001E2653.txt` |
| 1:0x0014CB10 (linear 0x0015CB10) | code | Uncompress_Data_ | `regions/07-object-1/001E1E54-001E2653.txt` |
| 1:0x0014CBEE (linear 0x0015CBEE) | code | buffer_remove_symbol_from_stream_ | `regions/07-object-1/001E1E54-001E2653.txt` |
| 1:0x0014CD2E (linear 0x0015CD2E) | code | buffer_initialize_arithmetic_decoder_ | `regions/07-object-1/001E1E54-001E2653.txt` |
| 1:0x0014CD9C (linear 0x0015CD9C) | code | buffer_encode_symbol_ | `regions/07-object-1/001E1E54-001E2653.txt` |
| 1:0x0014CF06 (linear 0x0015CF06) | code | buffer_flush_arithmetic_encoder_ | `regions/07-object-1/001E1E54-001E2653.txt` |
| 1:0x0014CF81 (linear 0x0015CF81) | code | CompressFile_ | `regions/07-object-1/001E1E54-001E2653.txt` |
| 1:0x0014D12A (linear 0x0015D12A) | code | ExpandFile_ | `regions/07-object-1/001E2654-001E2E53.txt` |
| 1:0x0014D2E5 (linear 0x0015D2E5) | code | initialize_model_ | `regions/07-object-1/001E2654-001E2E53.txt` |
| 1:0x0014D399 (linear 0x0015D399) | code | uninit_model_ | `regions/07-object-1/001E2654-001E2E53.txt` |
| 1:0x0014D3E7 (linear 0x0015D3E7) | code | update_model_ | `regions/07-object-1/001E2654-001E2E53.txt` |
| 1:0x0014D4E1 (linear 0x0015D4E1) | code | convert_int_to_symbol_ | `regions/07-object-1/001E2654-001E2E53.txt` |
| 1:0x0014D5B2 (linear 0x0015D5B2) | code | get_symbol_scale_ | `regions/07-object-1/001E2654-001E2E53.txt` |
| 1:0x0014D5F1 (linear 0x0015D5F1) | code | convert_symbol_to_int_ | `regions/07-object-1/001E2654-001E2E53.txt` |
| 1:0x0014D68B (linear 0x0015D68B) | code | initialize_arithmetic_encoder_ | `regions/07-object-1/001E2654-001E2E53.txt` |
| 1:0x0014D6C6 (linear 0x0015D6C6) | code | flush_arithmetic_encoder_ | `regions/07-object-1/001E2654-001E2E53.txt` |
| 1:0x0014D741 (linear 0x0015D741) | code | encode_symbol_ | `regions/07-object-1/001E2654-001E2E53.txt` |
| 1:0x0014D8AB (linear 0x0015D8AB) | code | get_current_count_ | `regions/07-object-1/001E2E54-001E3653.txt` |
| 1:0x0014D920 (linear 0x0015D920) | code | initialize_arithmetic_decoder_ | `regions/07-object-1/001E2E54-001E3653.txt` |
| 1:0x0014D98E (linear 0x0015D98E) | code | remove_symbol_from_stream_ | `regions/07-object-1/001E2E54-001E3653.txt` |
| 2:0x00012C40 (linear 0x00182C40) | data | _CompressionName | `regions/08-object-2/00200E54-00201653.txt` |
| 2:0x00012C44 (linear 0x00182C44) | data | _Usage | `regions/08-object-2/00200E54-00201653.txt` |
| 2:0x00051840 (linear 0x001C1840) | data | _underflow_bits | - |
| 2:0x00051844 (linear 0x001C1844) | data | _totals | - |
| 2:0x00051C48 (linear 0x001C1C48) | static, data | _high | - |
| 2:0x00051C4A (linear 0x001C1C4A) | static, data | _low | - |
| 2:0x00051C4C (linear 0x001C1C4C) | static, data | _code | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x00012C40 (linear 0x00182C40) | CompressionName | char * |
| 2:0x00012C44 (linear 0x00182C44) | Usage | char * |
| 2:0x00051840 (linear 0x001C1840) | underflow_bits | long |
| 2:0x00051844 (linear 0x001C1844) | totals | int *[257] |
| 2:0x00051C48 (linear 0x001C1C48) | high | unsigned short |
| 2:0x00051C4A (linear 0x001C1C4A) | low | unsigned short |
| 2:0x00051C4C (linear 0x001C1C4C) | code | unsigned short |

#### Compress_Data

- Address: 1:0x0014CA26 (linear 0x0015CA26), 234 bytes, in `regions/07-object-1/001E1E54-001E2653.txt`
- Type: void (char *, int, char *, int *)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| input | [EBP-0x24] | char * |
| size | [EBP-0x18] | int |
| output_buffer | [EBP-0x20] | char * |
| output_size | [EBP-0x1C] | int * |
| s | [EBP-0x2C] | SYMBOL |
| c | [EBP-0x14] | int |
| context | [EBP-0x10] | int |
| escaped | [EBP-0xC] | int |
| counter | [EBP-0x8] | int |
| output | [EBP-0x4] | BIT_BUFFER * |

#### Uncompress_Data

- Address: 1:0x0014CB10 (linear 0x0015CB10), 222 bytes, in `regions/07-object-1/001E1E54-001E2653.txt`
- Type: void (char *, int, char *, int *)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| input_buffer | [EBP-0x28] | char * |
| input_size | [EBP-0x1C] | int |
| output | [EBP-0x24] | char * |
| output_size | [EBP-0x20] | int * |
| s | [EBP-0x30] | SYMBOL |
| count | [EBP-0x18] | int |
| c | [EBP-0x14] | int |
| context | [EBP-0x10] | int |
| last_context | [EBP-0xC] | int |
| input | [EBP-0x8] | BIT_BUFFER * |
| counter | [EBP-0x4] | int |

#### buffer_remove_symbol_from_stream

- Address: 1:0x0014CBEE (linear 0x0015CBEE), 320 bytes, in `regions/07-object-1/001E1E54-001E2653.txt`
- Type: void (BIT_BUFFER *, SYMBOL *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| stream | [EBP-0xC] | BIT_BUFFER * |
| s | [EBP-0x8] | SYMBOL * |
| range | [EBP-0x4] | long |

#### buffer_initialize_arithmetic_decoder

- Address: 1:0x0014CD2E (linear 0x0015CD2E), 110 bytes, in `regions/07-object-1/001E1E54-001E2653.txt`
- Type: void (BIT_BUFFER *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| stream | [EBP-0x8] | BIT_BUFFER * |
| i | [EBP-0x4] | int |

#### buffer_encode_symbol

- Address: 1:0x0014CD9C (linear 0x0015CD9C), 362 bytes, in `regions/07-object-1/001E1E54-001E2653.txt`
- Type: void (BIT_BUFFER *, SYMBOL *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| stream | [EBP-0xC] | BIT_BUFFER * |
| s | [EBP-0x8] | SYMBOL * |
| range | [EBP-0x4] | long |

#### buffer_flush_arithmetic_encoder

- Address: 1:0x0014CF06 (linear 0x0015CF06), 123 bytes, in `regions/07-object-1/001E1E54-001E2653.txt`
- Type: void (BIT_BUFFER *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| stream | [EBP-0x4] | BIT_BUFFER * |

#### CompressFile

- Address: 1:0x0014CF81 (linear 0x0015CF81), 425 bytes, in `regions/07-object-1/001E1E54-001E2653.txt`
- Type: void (FILE *, BIT_FILE *, int, char * *)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| input | [EBP-0x10] | FILE * |
| output | [EBP-0x1C] | BIT_FILE * |
| argc | [EBP-0x18] | int |
| argv | [EBP-0x14] | char * * |
| s | [EBP-0x24] | SYMBOL |
| c | [EBP-0xC] | int |
| context | [EBP-0x8] | int |
| escaped | [EBP-0x4] | int |

#### ExpandFile

- Address: 1:0x0014D12A (linear 0x0015D12A), 443 bytes, in `regions/07-object-1/001E2654-001E2E53.txt`
- Type: void (BIT_FILE *, FILE *, int, char * *)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| input | [EBP-0x14] | BIT_FILE * |
| output | [EBP-0x20] | FILE * |
| argc | [EBP-0x1C] | int |
| argv | [EBP-0x18] | char * * |
| s | [EBP-0x28] | SYMBOL |
| count | [EBP-0x10] | int |
| c | [EBP-0xC] | int |
| context | [EBP-0x8] | int |
| last_context | [EBP-0x4] | int |

#### initialize_model

- Address: 1:0x0014D2E5 (linear 0x0015D2E5), 180 bytes, in `regions/07-object-1/001E2654-001E2E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| context | [EBP-0x8] | int |
| i | [EBP-0x4] | int |

#### uninit_model

- Address: 1:0x0014D399 (linear 0x0015D399), 78 bytes, in `regions/07-object-1/001E2654-001E2E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| context | [EBP-0x8] | int |
| i | [EBP-0x4] | int |

#### update_model

- Address: 1:0x0014D3E7 (linear 0x0015D3E7), 250 bytes, in `regions/07-object-1/001E2654-001E2E53.txt`
- Type: void (int, int)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| symbol | [EBP-0xC] | int |
| context | [EBP-0x8] | int |
| i | [EBP-0x4] | int |

#### convert_int_to_symbol

- Address: 1:0x0014D4E1 (linear 0x0015D4E1), 209 bytes, in `regions/07-object-1/001E2654-001E2E53.txt`
- Type: int (int, int, SYMBOL *); returns in EAX
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| c | [EBP-0x10] | int |
| context | [EBP-0xC] | int |
| s | [EBP-0x8] | SYMBOL * |

#### get_symbol_scale

- Address: 1:0x0014D5B2 (linear 0x0015D5B2), 63 bytes, in `regions/07-object-1/001E2654-001E2E53.txt`
- Type: void (int, SYMBOL *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| context | [EBP-0x8] | int |
| s | [EBP-0x4] | SYMBOL * |

#### convert_symbol_to_int

- Address: 1:0x0014D5F1 (linear 0x0015D5F1), 154 bytes, in `regions/07-object-1/001E2654-001E2E53.txt`
- Type: int (int, int, SYMBOL *); returns in EAX
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| count | [EBP-0x14] | int |
| context | [EBP-0x10] | int |
| s | [EBP-0xC] | SYMBOL * |
| c | [EBP-0x8] | int |

#### initialize_arithmetic_encoder

- Address: 1:0x0014D68B (linear 0x0015D68B), 59 bytes, in `regions/07-object-1/001E2654-001E2E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### flush_arithmetic_encoder

- Address: 1:0x0014D6C6 (linear 0x0015D6C6), 123 bytes, in `regions/07-object-1/001E2654-001E2E53.txt`
- Type: void (BIT_FILE *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| stream | [EBP-0x4] | BIT_FILE * |

#### encode_symbol

- Address: 1:0x0014D741 (linear 0x0015D741), 362 bytes, in `regions/07-object-1/001E2654-001E2E53.txt`
- Type: void (BIT_FILE *, SYMBOL *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| stream | [EBP-0xC] | BIT_FILE * |
| s | [EBP-0x8] | SYMBOL * |
| range | [EBP-0x4] | long |

#### get_current_count

- Address: 1:0x0014D8AB (linear 0x0015D8AB), 117 bytes, in `regions/07-object-1/001E2E54-001E3653.txt`
- Type: short (SYMBOL *); returns in AX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| s | [EBP-0x10] | SYMBOL * |
| range | [EBP-0xC] | long |
| count | [EBP-0x8] | short |

#### initialize_arithmetic_decoder

- Address: 1:0x0014D920 (linear 0x0015D920), 110 bytes, in `regions/07-object-1/001E2E54-001E3653.txt`
- Type: void (BIT_FILE *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| stream | [EBP-0x8] | BIT_FILE * |
| i | [EBP-0x4] | int |

#### remove_symbol_from_stream

- Address: 1:0x0014D98E (linear 0x0015D98E), 320 bytes, in `regions/07-object-1/001E2E54-001E3653.txt`
- Type: void (BIT_FILE *, SYMBOL *)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| stream | [EBP-0xC] | BIT_FILE * |
| s | [EBP-0x8] | SYMBOL * |
| range | [EBP-0x4] | long |

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
| 25 | pointer |  | near386; of int (#8) |
| 26 | array |  | 0..256; of int * (#25) |
| 27 | procedure |  | near386; returns void (char *, int, char *, int *) |
| 28 | struct | SYMBOL | 3 fields, 6 bytes |
| 29 | typedef | SYMBOL | name for struct #28 (#28) |
| 30 | struct | struct bit_buffer | 4 fields, 13 bytes |
| 31 | typedef | bit_buffer | struct name for struct #30 (#30) |
| 32 | typedef | BIT_BUFFER | name for struct bit_buffer (#31) |
| 33 | pointer |  | near386; of BIT_BUFFER (#32) |
| 34 | pointer |  | near386; of SYMBOL (#29) |
| 35 | procedure |  | near386; returns void (BIT_BUFFER *, SYMBOL *) |
| 36 | procedure |  | near386; returns void (BIT_BUFFER *) |
| 37 | pointer |  | near386; of unsigned char (#5) |
| 38 | struct | struct __iobuf | 8 fields, 26 bytes |
| 39 | typedef | __iobuf | struct name for struct #38 (#38) |
| 40 | typedef | FILE | name for struct __iobuf (#39) |
| 41 | pointer |  | near386; of FILE (#40) |
| 42 | struct | struct bit_file | 4 fields, 13 bytes |
| 43 | typedef | bit_file | struct name for struct #42 (#42) |
| 44 | typedef | BIT_FILE | name for struct bit_file (#43) |
| 45 | pointer |  | near386; of BIT_FILE (#44) |
| 46 | pointer |  | near386; of char * (#13) |
| 47 | procedure |  | near386; returns void (FILE *, BIT_FILE *, int, char * *) |
| 48 | procedure |  | near386; returns void (BIT_FILE *, FILE *, int, char * *) |
| 49 | procedure |  | near386; returns void (int, int) |
| 50 | procedure |  | near386; returns int (int, int, SYMBOL *) |
| 51 | procedure |  | near386; returns void (int, SYMBOL *) |
| 52 | procedure |  | near386; returns void (BIT_FILE *) |
| 53 | procedure |  | near386; returns void (BIT_FILE *, SYMBOL *) |
| 54 | procedure |  | near386; returns short (SYMBOL *) |

##### SYMBOL (#28, 3 fields, 6 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| low_count | offset 0 | unsigned short |
| high_count | offset 2 | unsigned short |
| scale | offset 4 | unsigned short |

##### struct bit_buffer (#30, 4 fields, 13 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| buffer | offset 0 | char * |
| mask | offset 4 | unsigned char |
| rack | offset 5 | int |
| pacifier_counter | offset 9 | int |

##### struct __iobuf (#38, 8 fields, 26 bytes)

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

##### struct bit_file (#42, 4 fields, 13 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| file | offset 0 | FILE * |
| mask | offset 4 | unsigned char |
| rack | offset 5 | int |
| pacifier_counter | offset 9 | int |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 90 | 1:0x0014CA26 (linear 0x0015CA26) | 0x001E207A |
| 99 | 1:0x0014CA47 (linear 0x0015CA47) | 0x001E209B |
| 105 | 1:0x0014CA51 (linear 0x0015CA51) | 0x001E20A5 |
| 107 | 1:0x0014CA5C (linear 0x0015CA5C) | 0x001E20B0 |
| 108 | 1:0x0014CA63 (linear 0x0015CA63) | 0x001E20B7 |
| 109 | 1:0x0014CA6A (linear 0x0015CA6A) | 0x001E20BE |
| 110 | 1:0x0014CA6F (linear 0x0015CA6F) | 0x001E20C3 |
| 113 | 1:0x0014CA74 (linear 0x0015CA74) | 0x001E20C8 |
| 115 | 1:0x0014CA7C (linear 0x0015CA7C) | 0x001E20D0 |
| 117 | 1:0x0014CA83 (linear 0x0015CA83) | 0x001E20D7 |
| 119 | 1:0x0014CA85 (linear 0x0015CA85) | 0x001E20D9 |
| 120 | 1:0x0014CA92 (linear 0x0015CA92) | 0x001E20E6 |
| 122 | 1:0x0014CA95 (linear 0x0015CA95) | 0x001E20E9 |
| 124 | 1:0x0014CAA6 (linear 0x0015CAA6) | 0x001E20FA |
| 126 | 1:0x0014CAB1 (linear 0x0015CAB1) | 0x001E2105 |
| 128 | 1:0x0014CAB7 (linear 0x0015CAB7) | 0x001E210B |
| 129 | 1:0x0014CAC7 (linear 0x0015CAC7) | 0x001E211B |
| 131 | 1:0x0014CAD2 (linear 0x0015CAD2) | 0x001E2126 |
| 135 | 1:0x0014CADB (linear 0x0015CADB) | 0x001E212F |
| 136 | 1:0x0014CAE6 (linear 0x0015CAE6) | 0x001E213A |
| 137 | 1:0x0014CAEC (linear 0x0015CAEC) | 0x001E2140 |
| 138 | 1:0x0014CAEE (linear 0x0015CAEE) | 0x001E2142 |
| 139 | 1:0x0014CAF6 (linear 0x0015CAF6) | 0x001E214A |
| 140 | 1:0x0014CB05 (linear 0x0015CB05) | 0x001E2159 |
| 141 | 1:0x0014CB0A (linear 0x0015CB0A) | 0x001E215E |
| 150 | 1:0x0014CB10 (linear 0x0015CB10) | 0x001E2164 |
| 160 | 1:0x0014CB31 (linear 0x0015CB31) | 0x001E2185 |
| 165 | 1:0x0014CB3B (linear 0x0015CB3B) | 0x001E218F |
| 167 | 1:0x0014CB46 (linear 0x0015CB46) | 0x001E219A |
| 168 | 1:0x0014CB4D (linear 0x0015CB4D) | 0x001E21A1 |
| 169 | 1:0x0014CB54 (linear 0x0015CB54) | 0x001E21A8 |
| 170 | 1:0x0014CB59 (linear 0x0015CB59) | 0x001E21AD |
| 173 | 1:0x0014CB61 (linear 0x0015CB61) | 0x001E21B5 |
| 175 | 1:0x0014CB67 (linear 0x0015CB67) | 0x001E21BB |
| 176 | 1:0x0014CB72 (linear 0x0015CB72) | 0x001E21C6 |
| 177 | 1:0x0014CB7E (linear 0x0015CB7E) | 0x001E21D2 |
| 178 | 1:0x0014CB8F (linear 0x0015CB8F) | 0x001E21E3 |
| 179 | 1:0x0014CB9A (linear 0x0015CB9A) | 0x001E21EE |
| 180 | 1:0x0014CBA0 (linear 0x0015CBA0) | 0x001E21F4 |
| 182 | 1:0x0014CBA9 (linear 0x0015CBA9) | 0x001E21FD |
| 184 | 1:0x0014CBB2 (linear 0x0015CBB2) | 0x001E2206 |
| 185 | 1:0x0014CBBD (linear 0x0015CBBD) | 0x001E2211 |
| 186 | 1:0x0014CBC0 (linear 0x0015CBC0) | 0x001E2214 |
| 187 | 1:0x0014CBCB (linear 0x0015CBCB) | 0x001E221F |
| 188 | 1:0x0014CBD1 (linear 0x0015CBD1) | 0x001E2225 |
| 189 | 1:0x0014CBD3 (linear 0x0015CBD3) | 0x001E2227 |
| 190 | 1:0x0014CBDB (linear 0x0015CBDB) | 0x001E222F |
| 191 | 1:0x0014CBE3 (linear 0x0015CBE3) | 0x001E2237 |
| 192 | 1:0x0014CBE8 (linear 0x0015CBE8) | 0x001E223C |
| 201 | 1:0x0014CBEE (linear 0x0015CBEE) | 0x001E2242 |
| 207 | 1:0x0014CC0B (linear 0x0015CC0B) | 0x001E225F |
| 208 | 1:0x0014CC22 (linear 0x0015CC22) | 0x001E2276 |
| 210 | 1:0x0014CC56 (linear 0x0015CC56) | 0x001E22AA |
| 219 | 1:0x0014CC8C (linear 0x0015CC8C) | 0x001E22E0 |
| 224 | 1:0x0014CCB0 (linear 0x0015CCB0) | 0x001E2304 |
| 225 | 1:0x0014CCDD (linear 0x0015CCDD) | 0x001E2331 |
| 226 | 1:0x0014CCE4 (linear 0x0015CCE4) | 0x001E2338 |
| 227 | 1:0x0014CCEB (linear 0x0015CCEB) | 0x001E233F |
| 228 | 1:0x0014CCF2 (linear 0x0015CCF2) | 0x001E2346 |
| 232 | 1:0x0014CCF4 (linear 0x0015CCF4) | 0x001E2348 |
| 233 | 1:0x0014CCF6 (linear 0x0015CCF6) | 0x001E234A |
| 234 | 1:0x0014CCFD (linear 0x0015CCFD) | 0x001E2351 |
| 235 | 1:0x0014CD04 (linear 0x0015CD04) | 0x001E2358 |
| 236 | 1:0x0014CD0B (linear 0x0015CD0B) | 0x001E235F |
| 237 | 1:0x0014CD12 (linear 0x0015CD12) | 0x001E2366 |
| 238 | 1:0x0014CD21 (linear 0x0015CD21) | 0x001E2375 |
| 239 | 1:0x0014CD26 (linear 0x0015CD26) | 0x001E237A |
| 247 | 1:0x0014CD2E (linear 0x0015CD2E) | 0x001E2382 |
| 251 | 1:0x0014CD49 (linear 0x0015CD49) | 0x001E239D |
| 252 | 1:0x0014CD52 (linear 0x0015CD52) | 0x001E23A6 |
| 254 | 1:0x0014CD69 (linear 0x0015CD69) | 0x001E23BD |
| 255 | 1:0x0014CD70 (linear 0x0015CD70) | 0x001E23C4 |
| 256 | 1:0x0014CD7F (linear 0x0015CD7F) | 0x001E23D3 |
| 257 | 1:0x0014CD81 (linear 0x0015CD81) | 0x001E23D5 |
| 258 | 1:0x0014CD8A (linear 0x0015CD8A) | 0x001E23DE |
| 259 | 1:0x0014CD93 (linear 0x0015CD93) | 0x001E23E7 |
| 271 | 1:0x0014CD9C (linear 0x0015CD9C) | 0x001E23F0 |
| 279 | 1:0x0014CDB9 (linear 0x0015CDB9) | 0x001E240D |
| 280 | 1:0x0014CDD0 (linear 0x0015CDD0) | 0x001E2424 |
| 282 | 1:0x0014CE04 (linear 0x0015CE04) | 0x001E2458 |
| 296 | 1:0x0014CE3A (linear 0x0015CE3A) | 0x001E248E |
| 298 | 1:0x0014CE5E (linear 0x0015CE5E) | 0x001E24B2 |
| 300 | 1:0x0014CE76 (linear 0x0015CE76) | 0x001E24CA |
| 302 | 1:0x0014CE7F (linear 0x0015CE7F) | 0x001E24D3 |
| 303 | 1:0x0014CE98 (linear 0x0015CE98) | 0x001E24EC |
| 304 | 1:0x0014CE9E (linear 0x0015CE9E) | 0x001E24F2 |
| 306 | 1:0x0014CEA0 (linear 0x0015CEA0) | 0x001E24F4 |
| 312 | 1:0x0014CEA2 (linear 0x0015CEA2) | 0x001E24F6 |
| 314 | 1:0x0014CECC (linear 0x0015CECC) | 0x001E2520 |
| 315 | 1:0x0014CED2 (linear 0x0015CED2) | 0x001E2526 |
| 316 | 1:0x0014CED9 (linear 0x0015CED9) | 0x001E252D |
| 318 | 1:0x0014CEE0 (linear 0x0015CEE0) | 0x001E2534 |
| 320 | 1:0x0014CEE2 (linear 0x0015CEE2) | 0x001E2536 |
| 323 | 1:0x0014CEE4 (linear 0x0015CEE4) | 0x001E2538 |
| 324 | 1:0x0014CEEB (linear 0x0015CEEB) | 0x001E253F |
| 325 | 1:0x0014CEF2 (linear 0x0015CEF2) | 0x001E2546 |
| 326 | 1:0x0014CEF9 (linear 0x0015CEF9) | 0x001E254D |
| 327 | 1:0x0014CEFE (linear 0x0015CEFE) | 0x001E2552 |
| 335 | 1:0x0014CF06 (linear 0x0015CF06) | 0x001E255A |
| 337 | 1:0x0014CF21 (linear 0x0015CF21) | 0x001E2575 |
| 338 | 1:0x0014CF39 (linear 0x0015CF39) | 0x001E258D |
| 339 | 1:0x0014CF3F (linear 0x0015CF3F) | 0x001E2593 |
| 341 | 1:0x0014CF4E (linear 0x0015CF4E) | 0x001E25A2 |
| 342 | 1:0x0014CF67 (linear 0x0015CF67) | 0x001E25BB |
| 343 | 1:0x0014CF69 (linear 0x0015CF69) | 0x001E25BD |
| 344 | 1:0x0014CF78 (linear 0x0015CF78) | 0x001E25CC |
| 347 | 1:0x0014CF81 (linear 0x0015CF81) | 0x001E25D5 |
| 358 | 1:0x0014CFA2 (linear 0x0015CFA2) | 0x001E25F6 |
| 359 | 1:0x0014CFA9 (linear 0x0015CFA9) | 0x001E25FD |
| 360 | 1:0x0014CFAE (linear 0x0015CFAE) | 0x001E2602 |
| 362 | 1:0x0014CFB3 (linear 0x0015CFB3) | 0x001E2607 |
| 363 | 1:0x0014D015 (linear 0x0015D015) | 0x001E2669 |
| 364 | 1:0x0014D01B (linear 0x0015D01B) | 0x001E266F |
| 365 | 1:0x0014D022 (linear 0x0015D022) | 0x001E2676 |
| 366 | 1:0x0014D033 (linear 0x0015D033) | 0x001E2687 |
| 367 | 1:0x0014D03E (linear 0x0015D03E) | 0x001E2692 |
| 368 | 1:0x0014D044 (linear 0x0015D044) | 0x001E2698 |
| 369 | 1:0x0014D054 (linear 0x0015D054) | 0x001E26A8 |
| 371 | 1:0x0014D05F (linear 0x0015D05F) | 0x001E26B3 |
| 373 | 1:0x0014D068 (linear 0x0015D068) | 0x001E26BC |
| 374 | 1:0x0014D073 (linear 0x0015D073) | 0x001E26C7 |
| 375 | 1:0x0014D079 (linear 0x0015D079) | 0x001E26CD |
| 376 | 1:0x0014D07E (linear 0x0015D07E) | 0x001E26D2 |
| 377 | 1:0x0014D086 (linear 0x0015D086) | 0x001E26DA |
| 378 | 1:0x0014D101 (linear 0x0015D101) | 0x001E2755 |
| 379 | 1:0x0014D10B (linear 0x0015D10B) | 0x001E275F |
| 380 | 1:0x0014D124 (linear 0x0015D124) | 0x001E2778 |
| 389 | 1:0x0014D12A (linear 0x0015D12A) | 0x001E277E |
| 401 | 1:0x0014D14B (linear 0x0015D14B) | 0x001E279F |
| 402 | 1:0x0014D152 (linear 0x0015D152) | 0x001E27A6 |
| 403 | 1:0x0014D157 (linear 0x0015D157) | 0x001E27AB |
| 405 | 1:0x0014D15F (linear 0x0015D15F) | 0x001E27B3 |
| 407 | 1:0x0014D165 (linear 0x0015D165) | 0x001E27B9 |
| 408 | 1:0x0014D170 (linear 0x0015D170) | 0x001E27C4 |
| 409 | 1:0x0014D17C (linear 0x0015D17C) | 0x001E27D0 |
| 410 | 1:0x0014D18D (linear 0x0015D18D) | 0x001E27E1 |
| 411 | 1:0x0014D198 (linear 0x0015D198) | 0x001E27EC |
| 412 | 1:0x0014D19E (linear 0x0015D19E) | 0x001E27F2 |
| 413 | 1:0x0014D1A7 (linear 0x0015D1A7) | 0x001E27FB |
| 415 | 1:0x0014D1B4 (linear 0x0015D1B4) | 0x001E2808 |
| 416 | 1:0x0014D22B (linear 0x0015D22B) | 0x001E287F |
| 417 | 1:0x0014D236 (linear 0x0015D236) | 0x001E288A |
| 418 | 1:0x0014D23C (linear 0x0015D23C) | 0x001E2890 |
| 419 | 1:0x0014D241 (linear 0x0015D241) | 0x001E2895 |
| 420 | 1:0x0014D2BC (linear 0x0015D2BC) | 0x001E2910 |
| 421 | 1:0x0014D2C6 (linear 0x0015D2C6) | 0x001E291A |
| 422 | 1:0x0014D2DF (linear 0x0015D2DF) | 0x001E2933 |
| 434 | 1:0x0014D2E5 (linear 0x0015D2E5) | 0x001E2939 |
| 439 | 1:0x0014D2FD (linear 0x0015D2FD) | 0x001E2951 |
| 440 | 1:0x0014D317 (linear 0x0015D317) | 0x001E296B |
| 441 | 1:0x0014D332 (linear 0x0015D332) | 0x001E2986 |
| 442 | 1:0x0014D341 (linear 0x0015D341) | 0x001E2995 |
| 443 | 1:0x0014D353 (linear 0x0015D353) | 0x001E29A7 |
| 444 | 1:0x0014D360 (linear 0x0015D360) | 0x001E29B4 |
| 445 | 1:0x0014D362 (linear 0x0015D362) | 0x001E29B6 |
| 446 | 1:0x0014D37C (linear 0x0015D37C) | 0x001E29D0 |
| 447 | 1:0x0014D390 (linear 0x0015D390) | 0x001E29E4 |
| 448 | 1:0x0014D399 (linear 0x0015D399) | 0x001E29ED |
| 453 | 1:0x0014D3B1 (linear 0x0015D3B1) | 0x001E2A05 |
| 455 | 1:0x0014D3CB (linear 0x0015D3CB) | 0x001E2A1F |
| 456 | 1:0x0014D3DC (linear 0x0015D3DC) | 0x001E2A30 |
| 457 | 1:0x0014D3DE (linear 0x0015D3DE) | 0x001E2A32 |
| 471 | 1:0x0014D3E7 (linear 0x0015D3E7) | 0x001E2A3B |
| 477 | 1:0x0014D404 (linear 0x0015D404) | 0x001E2A58 |
| 478 | 1:0x0014D41E (linear 0x0015D41E) | 0x001E2A72 |
| 479 | 1:0x0014D436 (linear 0x0015D436) | 0x001E2A8A |
| 481 | 1:0x0014D452 (linear 0x0015D452) | 0x001E2AA6 |
| 482 | 1:0x0014D46C (linear 0x0015D46C) | 0x001E2AC0 |
| 483 | 1:0x0014D48F (linear 0x0015D48F) | 0x001E2AE3 |
| 484 | 1:0x0014D4B4 (linear 0x0015D4B4) | 0x001E2B08 |
| 486 | 1:0x0014D4D9 (linear 0x0015D4D9) | 0x001E2B2D |
| 497 | 1:0x0014D4E1 (linear 0x0015D4E1) | 0x001E2B35 |
| 502 | 1:0x0014D500 (linear 0x0015D500) | 0x001E2B54 |
| 503 | 1:0x0014D51A (linear 0x0015D51A) | 0x001E2B6E |
| 504 | 1:0x0014D537 (linear 0x0015D537) | 0x001E2B8B |
| 505 | 1:0x0014D556 (linear 0x0015D556) | 0x001E2BAA |
| 506 | 1:0x0014D565 (linear 0x0015D565) | 0x001E2BB9 |
| 507 | 1:0x0014D56E (linear 0x0015D56E) | 0x001E2BC2 |
| 508 | 1:0x0014D587 (linear 0x0015D587) | 0x001E2BDB |
| 509 | 1:0x0014D5A1 (linear 0x0015D5A1) | 0x001E2BF5 |
| 510 | 1:0x0014D5A8 (linear 0x0015D5A8) | 0x001E2BFC |
| 518 | 1:0x0014D5B2 (linear 0x0015D5B2) | 0x001E2C06 |
| 522 | 1:0x0014D5CF (linear 0x0015D5CF) | 0x001E2C23 |
| 523 | 1:0x0014D5E9 (linear 0x0015D5E9) | 0x001E2C3D |
| 531 | 1:0x0014D5F1 (linear 0x0015D5F1) | 0x001E2C45 |
| 538 | 1:0x0014D610 (linear 0x0015D610) | 0x001E2C64 |
| 539 | 1:0x0014D63D (linear 0x0015D63D) | 0x001E2C91 |
| 540 | 1:0x0014D63F (linear 0x0015D63F) | 0x001E2C93 |
| 541 | 1:0x0014D65E (linear 0x0015D65E) | 0x001E2CB2 |
| 542 | 1:0x0014D67B (linear 0x0015D67B) | 0x001E2CCF |
| 543 | 1:0x0014D681 (linear 0x0015D681) | 0x001E2CD5 |
| 558 | 1:0x0014D68B (linear 0x0015D68B) | 0x001E2CDF |
| 560 | 1:0x0014D6A3 (linear 0x0015D6A3) | 0x001E2CF7 |
| 561 | 1:0x0014D6AC (linear 0x0015D6AC) | 0x001E2D00 |
| 562 | 1:0x0014D6B5 (linear 0x0015D6B5) | 0x001E2D09 |
| 563 | 1:0x0014D6BF (linear 0x0015D6BF) | 0x001E2D13 |
| 570 | 1:0x0014D6C6 (linear 0x0015D6C6) | 0x001E2D1A |
| 573 | 1:0x0014D6E1 (linear 0x0015D6E1) | 0x001E2D35 |
| 574 | 1:0x0014D6F9 (linear 0x0015D6F9) | 0x001E2D4D |
| 575 | 1:0x0014D6FF (linear 0x0015D6FF) | 0x001E2D53 |
| 576 | 1:0x0014D70E (linear 0x0015D70E) | 0x001E2D62 |
| 577 | 1:0x0014D729 (linear 0x0015D729) | 0x001E2D7D |
| 578 | 1:0x0014D738 (linear 0x0015D738) | 0x001E2D8C |
| 590 | 1:0x0014D741 (linear 0x0015D741) | 0x001E2D95 |
| 599 | 1:0x0014D75E (linear 0x0015D75E) | 0x001E2DB2 |
| 600 | 1:0x0014D775 (linear 0x0015D775) | 0x001E2DC9 |
| 602 | 1:0x0014D7A9 (linear 0x0015D7A9) | 0x001E2DFD |
| 613 | 1:0x0014D7DF (linear 0x0015D7DF) | 0x001E2E33 |
| 614 | 1:0x0014D803 (linear 0x0015D803) | 0x001E2E57 |
| 615 | 1:0x0014D81B (linear 0x0015D81B) | 0x001E2E6F |
| 616 | 1:0x0014D824 (linear 0x0015D824) | 0x001E2E78 |
| 617 | 1:0x0014D83D (linear 0x0015D83D) | 0x001E2E91 |
| 618 | 1:0x0014D843 (linear 0x0015D843) | 0x001E2E97 |
| 624 | 1:0x0014D845 (linear 0x0015D845) | 0x001E2E99 |
| 625 | 1:0x0014D871 (linear 0x0015D871) | 0x001E2EC5 |
| 626 | 1:0x0014D877 (linear 0x0015D877) | 0x001E2ECB |
| 627 | 1:0x0014D87E (linear 0x0015D87E) | 0x001E2ED2 |
| 628 | 1:0x0014D885 (linear 0x0015D885) | 0x001E2ED9 |
| 629 | 1:0x0014D887 (linear 0x0015D887) | 0x001E2EDB |
| 630 | 1:0x0014D889 (linear 0x0015D889) | 0x001E2EDD |
| 631 | 1:0x0014D890 (linear 0x0015D890) | 0x001E2EE4 |
| 632 | 1:0x0014D897 (linear 0x0015D897) | 0x001E2EEB |
| 633 | 1:0x0014D89E (linear 0x0015D89E) | 0x001E2EF2 |
| 634 | 1:0x0014D8A3 (linear 0x0015D8A3) | 0x001E2EF7 |
| 644 | 1:0x0014D8AB (linear 0x0015D8AB) | 0x001E2EFF |
| 650 | 1:0x0014D8C6 (linear 0x0015D8C6) | 0x001E2F1A |
| 651 | 1:0x0014D8DD (linear 0x0015D8DD) | 0x001E2F31 |
| 653 | 1:0x0014D90E (linear 0x0015D90E) | 0x001E2F62 |
| 654 | 1:0x0014D914 (linear 0x0015D914) | 0x001E2F68 |
| 662 | 1:0x0014D920 (linear 0x0015D920) | 0x001E2F74 |
| 667 | 1:0x0014D93B (linear 0x0015D93B) | 0x001E2F8F |
| 668 | 1:0x0014D944 (linear 0x0015D944) | 0x001E2F98 |
| 669 | 1:0x0014D95B (linear 0x0015D95B) | 0x001E2FAF |
| 670 | 1:0x0014D962 (linear 0x0015D962) | 0x001E2FB6 |
| 671 | 1:0x0014D971 (linear 0x0015D971) | 0x001E2FC5 |
| 672 | 1:0x0014D973 (linear 0x0015D973) | 0x001E2FC7 |
| 673 | 1:0x0014D97C (linear 0x0015D97C) | 0x001E2FD0 |
| 674 | 1:0x0014D985 (linear 0x0015D985) | 0x001E2FD9 |
| 682 | 1:0x0014D98E (linear 0x0015D98E) | 0x001E2FE2 |
| 691 | 1:0x0014D9AB (linear 0x0015D9AB) | 0x001E2FFF |
| 692 | 1:0x0014D9C2 (linear 0x0015D9C2) | 0x001E3016 |
| 694 | 1:0x0014D9F6 (linear 0x0015D9F6) | 0x001E304A |
| 703 | 1:0x0014DA2C (linear 0x0015DA2C) | 0x001E3080 |
| 708 | 1:0x0014DA50 (linear 0x0015DA50) | 0x001E30A4 |
| 709 | 1:0x0014DA7D (linear 0x0015DA7D) | 0x001E30D1 |
| 710 | 1:0x0014DA84 (linear 0x0015DA84) | 0x001E30D8 |
| 711 | 1:0x0014DA8B (linear 0x0015DA8B) | 0x001E30DF |
| 712 | 1:0x0014DA92 (linear 0x0015DA92) | 0x001E30E6 |
| 716 | 1:0x0014DA94 (linear 0x0015DA94) | 0x001E30E8 |
| 717 | 1:0x0014DA96 (linear 0x0015DA96) | 0x001E30EA |
| 718 | 1:0x0014DA9D (linear 0x0015DA9D) | 0x001E30F1 |
| 719 | 1:0x0014DAA4 (linear 0x0015DAA4) | 0x001E30F8 |
| 720 | 1:0x0014DAAB (linear 0x0015DAAB) | 0x001E30FF |
| 721 | 1:0x0014DAB2 (linear 0x0015DAB2) | 0x001E3106 |
| 722 | 1:0x0014DAC1 (linear 0x0015DAC1) | 0x001E3115 |
| 723 | 1:0x0014DAC6 (linear 0x0015DAC6) | 0x001E311A |

