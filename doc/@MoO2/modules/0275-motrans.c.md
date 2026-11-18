# E:\WLIBS\modem\motrans.c

Module 275 of the debug info of ORION2.EXE.

Ranges: 1:0x0013E8B0 (linear 0x0014E8B0), 2,287 bytes, 2:0x00004010 (linear 0x00174010), 140 bytes, 2:0x000116F8 (linear 0x001816F8), 16 bytes, 2:0x0004BC98 (linear 0x001BBC98), 76 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0013E8B0 (linear 0x0014E8B0) | code | InitModemTransportLayer_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 1:0x0013E961 (linear 0x0014E961) | code | UninitModemTransportLayer_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 1:0x0013E9AE (linear 0x0014E9AE) | code | Send_Receive_Ack_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 1:0x0013EA0E (linear 0x0014EA0E) | code | Send_Receive_Nack_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 1:0x0013EA6E (linear 0x0014EA6E) | code | Modem_Send_Message_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 1:0x0013EAB2 (linear 0x0014EAB2) | code | Modem_Status_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 1:0x0013EAF4 (linear 0x0014EAF4) | code | Send_Modem_Message_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 1:0x0013ED47 (linear 0x0014ED47) | code | Get_Received_Ack_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 1:0x0013EDA2 (linear 0x0014EDA2) | code | Modem_Clear_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 1:0x0013EDD9 (linear 0x0014EDD9) | code | Modem_Update_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 1:0x0013EE75 (linear 0x0014EE75) | code | Receive_Modem_Packet_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 2:0x000116F8 (linear 0x001816F8) | data | _modemTransportInitialized | `regions/08-object-2/001FF654-001FFE53.txt` |
| 2:0x000116F9 (linear 0x001816F9) | data | _mtp_receiving | `regions/08-object-2/001FF654-001FFE53.txt` |
| 2:0x000116FA (linear 0x001816FA) | data | _modem_receive_buffer_full | `regions/08-object-2/001FF654-001FFE53.txt` |
| 2:0x000116FE (linear 0x001816FE) | data | _send_message_num | `regions/08-object-2/001FF654-001FFE53.txt` |
| 2:0x0004BC98 (linear 0x001BBC98) | data | _modem_packet | - |
| 2:0x0004BCB0 (linear 0x001BBCB0) | data | _ronnie | - |
| 2:0x0004BCB4 (linear 0x001BBCB4) | data | _mtp_total_message_sent | - |
| 2:0x0004BCB8 (linear 0x001BBCB8) | data | _mtp_total_packets_sent | - |
| 2:0x0004BCBC (linear 0x001BBCBC) | data | _modem_receive_buffer | - |
| 2:0x0004BCC0 (linear 0x001BBCC0) | data | _mtp_receive_packet | - |
| 2:0x0004BCC4 (linear 0x001BBCC4) | data | _mtp_user_type | - |
| 2:0x0004BCC8 (linear 0x001BBCC8) | data | _modem_receive_buffer_size | - |
| 2:0x0004BCCC (linear 0x001BBCCC) | data | _mtp_message_size | - |
| 2:0x0004BCD0 (linear 0x001BBCD0) | data | _mtp_total_packets | - |
| 2:0x0004BCD4 (linear 0x001BBCD4) | data | _modem_receive_compress_buffer | - |
| 2:0x0004BCD8 (linear 0x001BBCD8) | data | _modem_amount_received | - |
| 2:0x0004BCDC (linear 0x001BBCDC) | data | _mtp_burst_position | - |
| 2:0x0004BCE0 (linear 0x001BBCE0) | data | _modem_transmit_compress_buffer | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x0004BC98 (linear 0x001BBC98) | modem_packet | struct s_modem_packet |
| 2:0x0004BCB0 (linear 0x001BBCB0) | ronnie | FILE * |
| 2:0x000116FE (linear 0x001816FE) | send_message_num | unsigned int |
| 2:0x0004BCB4 (linear 0x001BBCB4) | mtp_total_message_sent | int |
| 2:0x0004BCB8 (linear 0x001BBCB8) | mtp_total_packets_sent | int |
| 2:0x0004BCBC (linear 0x001BBCBC) | modem_receive_buffer | char * |
| 2:0x0004BCC0 (linear 0x001BBCC0) | mtp_receive_packet | int |
| 2:0x0004BCC4 (linear 0x001BBCC4) | mtp_user_type | int |
| 2:0x0004BCC8 (linear 0x001BBCC8) | modem_receive_buffer_size | int |
| 2:0x0004BCCC (linear 0x001BBCCC) | mtp_message_size | int |
| 2:0x0004BCD0 (linear 0x001BBCD0) | mtp_total_packets | int |
| 2:0x0004BCD4 (linear 0x001BBCD4) | modem_receive_compress_buffer | char * |
| 2:0x000116FA (linear 0x001816FA) | modem_receive_buffer_full | int |
| 2:0x0004BCD8 (linear 0x001BBCD8) | modem_amount_received | int |
| 2:0x0004BCDC (linear 0x001BBCDC) | mtp_burst_position | int |
| 2:0x0004BCE0 (linear 0x001BBCE0) | modem_transmit_compress_buffer | char * |
| 2:0x000116F9 (linear 0x001816F9) | mtp_receiving | char |
| 2:0x000116F8 (linear 0x001816F8) | modemTransportInitialized | char |

#### InitModemTransportLayer

- Address: 1:0x0013E8B0 (linear 0x0014E8B0), 177 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: char (int); returns in AL
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| size | [EBP-0x8] | int |

#### UninitModemTransportLayer

- Address: 1:0x0013E961 (linear 0x0014E961), 77 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Send_Receive_Ack

- Address: 1:0x0013E9AE (linear 0x0014E9AE), 96 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: void (unsigned int)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| message_num | [EBP-0x4] | unsigned int |
| ack_modem_packet | [EBP-0x1C] | struct s_modem_packet |

#### Send_Receive_Nack

- Address: 1:0x0013EA0E (linear 0x0014EA0E), 96 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: void (unsigned int)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| message_num | [EBP-0x4] | unsigned int |
| nack_modem_packet | [EBP-0x1C] | struct s_modem_packet |

#### Modem_Send_Message

- Address: 1:0x0013EA6E (linear 0x0014EA6E), 68 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: void (int, unsigned short, void *, unsigned short)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| dest | [EBP-0x10] | int |
| userType | [EBP-0x8] | unsigned short |
| data | [EBP-0xC] | void * |
| dataSize | [EBP-0x4] | unsigned short |

#### Modem_Status

- Address: 1:0x0013EAB2 (linear 0x0014EAB2), 66 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| status | [EBP-0x8] | int |

#### Send_Modem_Message

- Address: 1:0x0013EAF4 (linear 0x0014EAF4), 595 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: int (unsigned short, void *, int); returns in EAX
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| userType | [EBP-0x8] | unsigned short |
| modem_message_buffer | [EBP-0x20] | void * |
| size | [EBP-0xC] | int |
| buffer | [EBP-0x1C] | char * |
| done | [EBP-0x4] | char |
| i | [EBP-0x24] | int |
| space | [EBP-0x10] | int |
| send_modem_packet | [EBP-0x3C] | struct s_modem_packet |
| output_size | [EBP-0x14] | int |

#### Get_Received_Ack

- Address: 1:0x0013ED47 (linear 0x0014ED47), 91 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Modem_Clear

- Address: 1:0x0013EDA2 (linear 0x0014EDA2), 55 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Modem_Update

- Address: 1:0x0013EDD9 (linear 0x0014EDD9), 156 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Receive_Modem_Packet

- Address: 1:0x0013EE75 (linear 0x0014EE75), 810 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| done | [EBP-0x4] | char |
| i | [EBP-0x24] | int |
| crc_check | [EBP-0x20] | int |
| space | [EBP-0x14] | unsigned int |
| size | [EBP-0x18] | int |
| time_out | [EBP-0x1C] | unsigned int |
| old_time | [EBP-0x10] | unsigned int |
| old_space | [EBP-0xC] | unsigned int |
| debug_message | [EBP-0x224] | char[512] |
| last_message_num | address 2:0x00011702 | unsigned int |

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
| 20 | typedef | ptrdiff_t | name for int (#8) |
| 21 | scalar | void | void 1 bytes |
| 22 | procedure |  | near386; returns void (char *) |
| 23 | pointer | PORT_DUMPER | near386; of void (char *) (#22) |
| 24 | typedef | PORT_DUMPER | name for void (char *) * (#23) |
| 25 | procedure |  | near386; returns void (char) |
| 26 | pointer | CHAR_PRINTER | near386; of void (char) (#25) |
| 27 | typedef | CHAR_PRINTER | name for void (char) * (#26) |
| 28 | scalar | unsigned long | unsigned 4 bytes |
| 29 | typedef | time_t | name for unsigned long (#28) |
| 30 | typedef | clock_t | name for unsigned long (#28) |
| 31 | typedef | time_t | name for unsigned long (#28) |
| 32 | typedef | BYTE | name for unsigned char (#5) |
| 33 | typedef | SBYTE | name for signed char (#4) |
| 34 | typedef | UBYTE | name for unsigned char (#5) |
| 35 | typedef | WORD | name for unsigned short (#7) |
| 36 | typedef | SWORD | name for short (#6) |
| 37 | typedef | UWORD | name for unsigned short (#7) |
| 38 | typedef | DWORD | name for unsigned int (#9) |
| 39 | typedef | SDWORD | name for int (#8) |
| 40 | typedef | UDWORD | name for unsigned int (#9) |
| 41 | typedef | BOOL | name for unsigned char (#5) |
| 42 | typedef | SampleHandle | name for int (#8) |
| 43 | struct | struct s_modem_packet | 8 fields, 24 bytes |
| 44 | typedef | s_modem_packet | struct name for struct #43 (#43) |
| 45 | pointer |  | near386; of unsigned char (#5) |
| 46 | struct | struct __iobuf | 8 fields, 26 bytes |
| 47 | typedef | __iobuf | struct name for struct #46 (#46) |
| 48 | typedef | FILE | name for struct __iobuf (#47) |
| 49 | pointer |  | near386; of FILE (#48) |
| 50 | procedure |  | near386; returns char (int) |
| 51 | procedure |  | near386; returns void (void) |
| 52 | procedure |  | near386; returns void (unsigned int) |
| 53 | pointer |  | near386; of void (#21) |
| 54 | procedure |  | near386; returns void (int, unsigned short, void *, unsigned short) |
| 55 | procedure |  | near386; returns int (void) |
| 56 | procedure |  | near386; returns int (unsigned short, void *, int) |
| 57 | array |  | 0..511; of char (#10) |

##### struct s_modem_packet (#43, 8 fields, 24 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| type | offset 0 | char |
| compressed | offset 1 | char |
| packet_number | offset 2 | int |
| max_packets | offset 6 | int |
| size | offset 10 | int |
| packet_crc | offset 14 | int |
| user_type | offset 18 | unsigned short |
| message_num | offset 20 | unsigned int |

##### struct __iobuf (#46, 8 fields, 26 bytes)

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
| 72 | 1:0x0013E8B0 (linear 0x0014E8B0) | 0x001D3F04 |
| 74 | 1:0x0013E8CB (linear 0x0014E8CB) | 0x001D3F1F |
| 76 | 1:0x0013E8D1 (linear 0x0014E8D1) | 0x001D3F25 |
| 78 | 1:0x0013E8DA (linear 0x0014E8DA) | 0x001D3F2E |
| 79 | 1:0x0013E8E7 (linear 0x0014E8E7) | 0x001D3F3B |
| 80 | 1:0x0013E8F4 (linear 0x0014E8F4) | 0x001D3F48 |
| 81 | 1:0x0013E901 (linear 0x0014E901) | 0x001D3F55 |
| 83 | 1:0x0013E91E (linear 0x0014E91E) | 0x001D3F72 |
| 85 | 1:0x0013E924 (linear 0x0014E924) | 0x001D3F78 |
| 86 | 1:0x0013E92C (linear 0x0014E92C) | 0x001D3F80 |
| 87 | 1:0x0013E933 (linear 0x0014E933) | 0x001D3F87 |
| 88 | 1:0x0013E93D (linear 0x0014E93D) | 0x001D3F91 |
| 89 | 1:0x0013E947 (linear 0x0014E947) | 0x001D3F9B |
| 90 | 1:0x0013E951 (linear 0x0014E951) | 0x001D3FA5 |
| 91 | 1:0x0013E955 (linear 0x0014E955) | 0x001D3FA9 |
| 93 | 1:0x0013E961 (linear 0x0014E961) | 0x001D3FB5 |
| 95 | 1:0x0013E979 (linear 0x0014E979) | 0x001D3FCD |
| 97 | 1:0x0013E982 (linear 0x0014E982) | 0x001D3FD6 |
| 99 | 1:0x0013E98C (linear 0x0014E98C) | 0x001D3FE0 |
| 100 | 1:0x0013E996 (linear 0x0014E996) | 0x001D3FEA |
| 101 | 1:0x0013E9A0 (linear 0x0014E9A0) | 0x001D3FF4 |
| 102 | 1:0x0013E9A7 (linear 0x0014E9A7) | 0x001D3FFB |
| 104 | 1:0x0013E9AE (linear 0x0014E9AE) | 0x001D4002 |
| 108 | 1:0x0013E9C9 (linear 0x0014E9C9) | 0x001D401D |
| 109 | 1:0x0013E9CD (linear 0x0014E9CD) | 0x001D4021 |
| 110 | 1:0x0013E9D4 (linear 0x0014E9D4) | 0x001D4028 |
| 111 | 1:0x0013E9DB (linear 0x0014E9DB) | 0x001D402F |
| 112 | 1:0x0013E9E2 (linear 0x0014E9E2) | 0x001D4036 |
| 113 | 1:0x0013E9E9 (linear 0x0014E9E9) | 0x001D403D |
| 117 | 1:0x0013E9EF (linear 0x0014E9EF) | 0x001D4043 |
| 118 | 1:0x0013EA05 (linear 0x0014EA05) | 0x001D4059 |
| 119 | 1:0x0013EA0E (linear 0x0014EA0E) | 0x001D4062 |
| 124 | 1:0x0013EA29 (linear 0x0014EA29) | 0x001D407D |
| 125 | 1:0x0013EA2D (linear 0x0014EA2D) | 0x001D4081 |
| 126 | 1:0x0013EA34 (linear 0x0014EA34) | 0x001D4088 |
| 127 | 1:0x0013EA3B (linear 0x0014EA3B) | 0x001D408F |
| 128 | 1:0x0013EA42 (linear 0x0014EA42) | 0x001D4096 |
| 129 | 1:0x0013EA49 (linear 0x0014EA49) | 0x001D409D |
| 130 | 1:0x0013EA4F (linear 0x0014EA4F) | 0x001D40A3 |
| 131 | 1:0x0013EA65 (linear 0x0014EA65) | 0x001D40B9 |
| 133 | 1:0x0013EA6E (linear 0x0014EA6E) | 0x001D40C2 |
| 135 | 1:0x0013EA8F (linear 0x0014EA8F) | 0x001D40E3 |
| 139 | 1:0x0013EA98 (linear 0x0014EA98) | 0x001D40EC |
| 140 | 1:0x0013EAAC (linear 0x0014EAAC) | 0x001D4100 |
| 141 | 1:0x0013EAB2 (linear 0x0014EAB2) | 0x001D4106 |
| 145 | 1:0x0013EACA (linear 0x0014EACA) | 0x001D411E |
| 147 | 1:0x0013EAD1 (linear 0x0014EAD1) | 0x001D4125 |
| 149 | 1:0x0013EAD5 (linear 0x0014EAD5) | 0x001D4129 |
| 151 | 1:0x0013EADE (linear 0x0014EADE) | 0x001D4132 |
| 153 | 1:0x0013EAE2 (linear 0x0014EAE2) | 0x001D4136 |
| 154 | 1:0x0013EAE8 (linear 0x0014EAE8) | 0x001D413C |
| 155 | 1:0x0013EAF4 (linear 0x0014EAF4) | 0x001D4148 |
| 166 | 1:0x0013EB13 (linear 0x0014EB13) | 0x001D4167 |
| 167 | 1:0x0013EB19 (linear 0x0014EB19) | 0x001D416D |
| 169 | 1:0x0013EB1F (linear 0x0014EB1F) | 0x001D4173 |
| 170 | 1:0x0013EB2E (linear 0x0014EB2E) | 0x001D4182 |
| 172 | 1:0x0013EB3A (linear 0x0014EB3A) | 0x001D418E |
| 174 | 1:0x0013EB43 (linear 0x0014EB43) | 0x001D4197 |
| 175 | 1:0x0013EB4B (linear 0x0014EB4B) | 0x001D419F |
| 176 | 1:0x0013EB5F (linear 0x0014EB5F) | 0x001D41B3 |
| 178 | 1:0x0013EB63 (linear 0x0014EB63) | 0x001D41B7 |
| 180 | 1:0x0013EB65 (linear 0x0014EB65) | 0x001D41B9 |
| 181 | 1:0x0013EB75 (linear 0x0014EB75) | 0x001D41C9 |
| 184 | 1:0x0013EB79 (linear 0x0014EB79) | 0x001D41CD |
| 185 | 1:0x0013EB81 (linear 0x0014EB81) | 0x001D41D5 |
| 187 | 1:0x0013EB87 (linear 0x0014EB87) | 0x001D41DB |
| 188 | 1:0x0013EB91 (linear 0x0014EB91) | 0x001D41E5 |
| 189 | 1:0x0013EB9B (linear 0x0014EB9B) | 0x001D41EF |
| 190 | 1:0x0013EBA3 (linear 0x0014EBA3) | 0x001D41F7 |
| 192 | 1:0x0013EBAD (linear 0x0014EBAD) | 0x001D4201 |
| 194 | 1:0x0013EBB6 (linear 0x0014EBB6) | 0x001D420A |
| 195 | 1:0x0013EBCC (linear 0x0014EBCC) | 0x001D4220 |
| 197 | 1:0x0013EBE0 (linear 0x0014EBE0) | 0x001D4234 |
| 200 | 1:0x0013EBE6 (linear 0x0014EBE6) | 0x001D423A |
| 202 | 1:0x0013EBE8 (linear 0x0014EBE8) | 0x001D423C |
| 204 | 1:0x0013EBFC (linear 0x0014EBFC) | 0x001D4250 |
| 208 | 1:0x0013EC06 (linear 0x0014EC06) | 0x001D425A |
| 209 | 1:0x0013EC0C (linear 0x0014EC0C) | 0x001D4260 |
| 211 | 1:0x0013EC10 (linear 0x0014EC10) | 0x001D4264 |
| 215 | 1:0x0013EC1A (linear 0x0014EC1A) | 0x001D426E |
| 216 | 1:0x0013EC1E (linear 0x0014EC1E) | 0x001D4272 |
| 217 | 1:0x0013EC26 (linear 0x0014EC26) | 0x001D427A |
| 218 | 1:0x0013EC2E (linear 0x0014EC2E) | 0x001D4282 |
| 219 | 1:0x0013EC35 (linear 0x0014EC35) | 0x001D4289 |
| 220 | 1:0x0013EC3D (linear 0x0014EC3D) | 0x001D4291 |
| 222 | 1:0x0013EC46 (linear 0x0014EC46) | 0x001D429A |
| 224 | 1:0x0013EC5A (linear 0x0014EC5A) | 0x001D42AE |
| 226 | 1:0x0013EC6D (linear 0x0014EC6D) | 0x001D42C1 |
| 228 | 1:0x0013EC6F (linear 0x0014EC6F) | 0x001D42C3 |
| 231 | 1:0x0013EC76 (linear 0x0014EC76) | 0x001D42CA |
| 233 | 1:0x0013EC78 (linear 0x0014EC78) | 0x001D42CC |
| 236 | 1:0x0013EC7F (linear 0x0014EC7F) | 0x001D42D3 |
| 237 | 1:0x0013EC86 (linear 0x0014EC86) | 0x001D42DA |
| 239 | 1:0x0013EC9F (linear 0x0014EC9F) | 0x001D42F3 |
| 240 | 1:0x0013ECB5 (linear 0x0014ECB5) | 0x001D4309 |
| 242 | 1:0x0013ECB7 (linear 0x0014ECB7) | 0x001D430B |
| 243 | 1:0x0013ECCD (linear 0x0014ECCD) | 0x001D4321 |
| 244 | 1:0x0013ECE7 (linear 0x0014ECE7) | 0x001D433B |
| 245 | 1:0x0013ECF8 (linear 0x0014ECF8) | 0x001D434C |
| 247 | 1:0x0013ECFE (linear 0x0014ECFE) | 0x001D4352 |
| 248 | 1:0x0013ED0F (linear 0x0014ED0F) | 0x001D4363 |
| 249 | 1:0x0013ED11 (linear 0x0014ED11) | 0x001D4365 |
| 252 | 1:0x0013ED1E (linear 0x0014ED1E) | 0x001D4372 |
| 254 | 1:0x0013ED27 (linear 0x0014ED27) | 0x001D437B |
| 256 | 1:0x0013ED2B (linear 0x0014ED2B) | 0x001D437F |
| 257 | 1:0x0013ED31 (linear 0x0014ED31) | 0x001D4385 |
| 258 | 1:0x0013ED36 (linear 0x0014ED36) | 0x001D438A |
| 259 | 1:0x0013ED3D (linear 0x0014ED3D) | 0x001D4391 |
| 260 | 1:0x0013ED47 (linear 0x0014ED47) | 0x001D439B |
| 262 | 1:0x0013ED5F (linear 0x0014ED5F) | 0x001D43B3 |
| 264 | 1:0x0013ED66 (linear 0x0014ED66) | 0x001D43BA |
| 265 | 1:0x0013ED6B (linear 0x0014ED6B) | 0x001D43BF |
| 267 | 1:0x0013ED86 (linear 0x0014ED86) | 0x001D43DA |
| 269 | 1:0x0013ED8F (linear 0x0014ED8F) | 0x001D43E3 |
| 270 | 1:0x0013ED96 (linear 0x0014ED96) | 0x001D43EA |
| 271 | 1:0x0013EDA2 (linear 0x0014EDA2) | 0x001D43F6 |
| 273 | 1:0x0013EDBA (linear 0x0014EDBA) | 0x001D440E |
| 274 | 1:0x0013EDC4 (linear 0x0014EDC4) | 0x001D4418 |
| 275 | 1:0x0013EDD2 (linear 0x0014EDD2) | 0x001D4426 |
| 276 | 1:0x0013EDD9 (linear 0x0014EDD9) | 0x001D442D |
| 278 | 1:0x0013EDF1 (linear 0x0014EDF1) | 0x001D4445 |
| 283 | 1:0x0013EDFE (linear 0x0014EDFE) | 0x001D4452 |
| 285 | 1:0x0013EE07 (linear 0x0014EE07) | 0x001D445B |
| 287 | 1:0x0013EE10 (linear 0x0014EE10) | 0x001D4464 |
| 292 | 1:0x0013EE19 (linear 0x0014EE19) | 0x001D446D |
| 294 | 1:0x0013EE2C (linear 0x0014EE2C) | 0x001D4480 |
| 295 | 1:0x0013EE31 (linear 0x0014EE31) | 0x001D4485 |
| 297 | 1:0x0013EE3A (linear 0x0014EE3A) | 0x001D448E |
| 298 | 1:0x0013EE64 (linear 0x0014EE64) | 0x001D44B8 |
| 302 | 1:0x0013EE6E (linear 0x0014EE6E) | 0x001D44C2 |
| 304 | 1:0x0013EE75 (linear 0x0014EE75) | 0x001D44C9 |
| 311 | 1:0x0013EE8D (linear 0x0014EE8D) | 0x001D44E1 |
| 317 | 1:0x0013EE94 (linear 0x0014EE94) | 0x001D44E8 |
| 319 | 1:0x0013EE98 (linear 0x0014EE98) | 0x001D44EC |
| 320 | 1:0x0013EEA0 (linear 0x0014EEA0) | 0x001D44F4 |
| 322 | 1:0x0013EEA7 (linear 0x0014EEA7) | 0x001D44FB |
| 324 | 1:0x0013EEB1 (linear 0x0014EEB1) | 0x001D4505 |
| 326 | 1:0x0013EEC2 (linear 0x0014EEC2) | 0x001D4516 |
| 328 | 1:0x0013EECA (linear 0x0014EECA) | 0x001D451E |
| 330 | 1:0x0013EED7 (linear 0x0014EED7) | 0x001D452B |
| 333 | 1:0x0013EEE3 (linear 0x0014EEE3) | 0x001D4537 |
| 335 | 1:0x0013EEE5 (linear 0x0014EEE5) | 0x001D4539 |
| 336 | 1:0x0013EEED (linear 0x0014EEED) | 0x001D4541 |
| 340 | 1:0x0013EEF3 (linear 0x0014EEF3) | 0x001D4547 |
| 342 | 1:0x0013EEFD (linear 0x0014EEFD) | 0x001D4551 |
| 343 | 1:0x0013EF15 (linear 0x0014EF15) | 0x001D4569 |
| 346 | 1:0x0013EF46 (linear 0x0014EF46) | 0x001D459A |
| 348 | 1:0x0013EF5B (linear 0x0014EF5B) | 0x001D45AF |
| 350 | 1:0x0013EF5D (linear 0x0014EF5D) | 0x001D45B1 |
| 352 | 1:0x0013EF72 (linear 0x0014EF72) | 0x001D45C6 |
| 354 | 1:0x0013EF74 (linear 0x0014EF74) | 0x001D45C8 |
| 356 | 1:0x0013EF89 (linear 0x0014EF89) | 0x001D45DD |
| 359 | 1:0x0013EF8B (linear 0x0014EF8B) | 0x001D45DF |
| 360 | 1:0x0013EFA0 (linear 0x0014EFA0) | 0x001D45F4 |
| 361 | 1:0x0013EFAA (linear 0x0014EFAA) | 0x001D45FE |
| 365 | 1:0x0013EFB4 (linear 0x0014EFB4) | 0x001D4608 |
| 367 | 1:0x0013EFD6 (linear 0x0014EFD6) | 0x001D462A |
| 369 | 1:0x0013EFE2 (linear 0x0014EFE2) | 0x001D4636 |
| 370 | 1:0x0013EFE6 (linear 0x0014EFE6) | 0x001D463A |
| 374 | 1:0x0013EFF2 (linear 0x0014EFF2) | 0x001D4646 |
| 377 | 1:0x0013F008 (linear 0x0014F008) | 0x001D465C |
| 379 | 1:0x0013F011 (linear 0x0014F011) | 0x001D4665 |
| 380 | 1:0x0013F01E (linear 0x0014F01E) | 0x001D4672 |
| 381 | 1:0x0013F025 (linear 0x0014F025) | 0x001D4679 |
| 382 | 1:0x0013F02F (linear 0x0014F02F) | 0x001D4683 |
| 384 | 1:0x0013F039 (linear 0x0014F039) | 0x001D468D |
| 385 | 1:0x0013F05D (linear 0x0014F05D) | 0x001D46B1 |
| 386 | 1:0x0013F068 (linear 0x0014F068) | 0x001D46BC |
| 388 | 1:0x0013F06C (linear 0x0014F06C) | 0x001D46C0 |
| 389 | 1:0x0013F073 (linear 0x0014F073) | 0x001D46C7 |
| 391 | 1:0x0013F08F (linear 0x0014F08F) | 0x001D46E3 |
| 392 | 1:0x0013F0AD (linear 0x0014F0AD) | 0x001D4701 |
| 393 | 1:0x0013F0AF (linear 0x0014F0AF) | 0x001D4703 |
| 396 | 1:0x0013F0BA (linear 0x0014F0BA) | 0x001D470E |
| 397 | 1:0x0013F0C4 (linear 0x0014F0C4) | 0x001D4718 |
| 401 | 1:0x0013F0D0 (linear 0x0014F0D0) | 0x001D4724 |
| 403 | 1:0x0013F0E2 (linear 0x0014F0E2) | 0x001D4736 |
| 404 | 1:0x0013F0E9 (linear 0x0014F0E9) | 0x001D473D |
| 406 | 1:0x0013F0F5 (linear 0x0014F0F5) | 0x001D4749 |
| 407 | 1:0x0013F10E (linear 0x0014F10E) | 0x001D4762 |
| 409 | 1:0x0013F12D (linear 0x0014F12D) | 0x001D4781 |
| 411 | 1:0x0013F135 (linear 0x0014F135) | 0x001D4789 |
| 413 | 1:0x0013F137 (linear 0x0014F137) | 0x001D478B |
| 415 | 1:0x0013F14D (linear 0x0014F14D) | 0x001D47A1 |
| 417 | 1:0x0013F15A (linear 0x0014F15A) | 0x001D47AE |
| 418 | 1:0x0013F164 (linear 0x0014F164) | 0x001D47B8 |
| 422 | 1:0x0013F16E (linear 0x0014F16E) | 0x001D47C2 |
| 423 | 1:0x0013F178 (linear 0x0014F178) | 0x001D47CC |
| 425 | 1:0x0013F17E (linear 0x0014F17E) | 0x001D47D2 |
| 428 | 1:0x0013F187 (linear 0x0014F187) | 0x001D47DB |
| 429 | 1:0x0013F18C (linear 0x0014F18C) | 0x001D47E0 |
| 430 | 1:0x0013F193 (linear 0x0014F193) | 0x001D47E7 |

