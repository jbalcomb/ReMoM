# E:\WLIBS\modem\simmodem.c

Module 203 of the debug info of ORION2.EXE.

Ranges: 1:0x0012CC5B (linear 0x0013CC5B), 1,611 bytes, 2:0x000030D0 (linear 0x001730D0), 32 bytes, 2:0x0000C97C (linear 0x0017C97C), 8 bytes, 2:0x00047388 (linear 0x001B7388), 8 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0012CC5B (linear 0x0013CC5B) | code | Init_Modem_ | `regions/07-object-1/001C1E54-001C2653.txt` |
| 1:0x0012CCC5 (linear 0x0013CCC5) | code | Uninit_Modem_ | `regions/07-object-1/001C1E54-001C2653.txt` |
| 1:0x0012CCF7 (linear 0x0013CCF7) | code | Modem_Output_ | `regions/07-object-1/001C1E54-001C2653.txt` |
| 1:0x0012CD54 (linear 0x0013CD54) | code | Modem_Read_Char_ | `regions/07-object-1/001C1E54-001C2653.txt` |
| 1:0x0012CDA5 (linear 0x0013CDA5) | code | Modem_Answer_ | `regions/07-object-1/001C1E54-001C2653.txt` |
| 1:0x0012CE01 (linear 0x0013CE01) | code | Modem_Dial_Number_ | `regions/07-object-1/001C1E54-001C2653.txt` |
| 1:0x0012CED8 (linear 0x0013CED8) | code | Get_Modem_Error_ | `regions/07-object-1/001C1E54-001C2653.txt` |
| 1:0x0012CFDD (linear 0x0013CFDD) | code | Modem_Connected_ | `regions/07-object-1/001C1E54-001C2653.txt` |
| 1:0x0012D028 (linear 0x0013D028) | code | Modem_Hang_Up_ | `regions/07-object-1/001C2654-001C2E53.txt` |
| 1:0x0012D0F1 (linear 0x0013D0F1) | code | Modem_Reset_ | `regions/07-object-1/001C2654-001C2E53.txt` |
| 1:0x0012D190 (linear 0x0013D190) | code | Modem_Write_Char_ | `regions/07-object-1/001C2654-001C2E53.txt` |
| 1:0x0012D1C7 (linear 0x0013D1C7) | code | Modem_Open_Port_ | `regions/07-object-1/001C2654-001C2E53.txt` |
| 1:0x0012D266 (linear 0x0013D266) | code | Modem_Close_Port_ | `regions/07-object-1/001C2654-001C2E53.txt` |
| 2:0x0000C97C (linear 0x0017C97C) | data | _open_game_port | `regions/08-object-2/001FAE54-001FB653.txt` |
| 2:0x0000C980 (linear 0x0017C980) | data | _modemInitialized | `regions/08-object-2/001FAE54-001FB653.txt` |
| 2:0x00047388 (linear 0x001B7388) | data | _modemUserRecFunc | - |
| 2:0x0004738C (linear 0x001B738C) | data | _connected_to_id | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x00047388 (linear 0x001B7388) | modemUserRecFunc | void (unsigned short, unsigned short, unsigned short, unsigned char *) * |
| 2:0x0000C97C (linear 0x0017C97C) | open_game_port | PORT * |
| 2:0x0004738C (linear 0x001B738C) | connected_to_id | char |
| 2:0x0000C980 (linear 0x0017C980) | modemInitialized | char |

#### Init_Modem

- Address: 1:0x0012CC5B (linear 0x0013CC5B), 106 bytes, in `regions/07-object-1/001C1E54-001C2653.txt`
- Type: int (struct s_modem_comm *, void (unsigned short, unsigned short, unsigned short, unsigned char *) *); returns in EAX
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| settings | [EBP-0xC] | struct s_modem_comm * |
| receiveMessage | [EBP-0x8] | void (unsigned short, unsigned short, unsigned short, unsigned char *) * |

#### Uninit_Modem

- Address: 1:0x0012CCC5 (linear 0x0013CCC5), 50 bytes, in `regions/07-object-1/001C1E54-001C2653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Modem_Output

- Address: 1:0x0012CCF7 (linear 0x0013CCF7), 93 bytes, in `regions/07-object-1/001C1E54-001C2653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| modem_char | [EBP-0x8] | int |
| modem_output | [EBP-0x4] | int |

#### Modem_Read_Char

- Address: 1:0x0012CD54 (linear 0x0013CD54), 81 bytes, in `regions/07-object-1/001C1E54-001C2653.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| modem_char | [EBP-0x4] | char |

#### Modem_Answer

- Address: 1:0x0012CDA5 (linear 0x0013CDA5), 92 bytes, in `regions/07-object-1/001C1E54-001C2653.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Modem_Dial_Number

- Address: 1:0x0012CE01 (linear 0x0013CE01), 215 bytes, in `regions/07-object-1/001C1E54-001C2653.txt`
- Type: int (struct s_modem_init_string *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| init | [EBP-0xC] | struct s_modem_init_string * |
| dial_string | [EBP-0x3C] | char[45] |
| input_buffer | [EBP-0x90] | char[81] |
| ms_counter | [EBP-0x8] | int |

#### Get_Modem_Error

- Address: 1:0x0012CED8 (linear 0x0013CED8), 261 bytes, in `regions/07-object-1/001C1E54-001C2653.txt`
- Type: void (char *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| modem_error | [EBP-0x10] | char * |
| temp_string | [EBP-0x18] | char[8] |
| esc | [EBP-0xC] | int |
| start_time | [EBP-0x8] | unsigned int |
| time_out | [EBP-0x4] | unsigned int |

#### Modem_Connected

- Address: 1:0x0012CFDD (linear 0x0013CFDD), 75 bytes, in `regions/07-object-1/001C1E54-001C2653.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Modem_Hang_Up

- Address: 1:0x0012D028 (linear 0x0013D028), 201 bytes, in `regions/07-object-1/001C2654-001C2E53.txt`
- Type: int (struct s_modem_init_string *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| init | [EBP-0x8] | struct s_modem_init_string * |

#### Modem_Reset

- Address: 1:0x0012D0F1 (linear 0x0013D0F1), 159 bytes, in `regions/07-object-1/001C2654-001C2E53.txt`
- Type: int (struct s_modem_init_string *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| init | [EBP-0x8] | struct s_modem_init_string * |

#### Modem_Write_Char

- Address: 1:0x0012D190 (linear 0x0013D190), 55 bytes, in `regions/07-object-1/001C2654-001C2E53.txt`
- Type: void (char)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| key | [EBP-0x4] | char |

#### Modem_Open_Port

- Address: 1:0x0012D1C7 (linear 0x0013D1C7), 159 bytes, in `regions/07-object-1/001C2654-001C2E53.txt`
- Type: int (struct s_modem_comm *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| settings | [EBP-0x8] | struct s_modem_comm * |

#### Modem_Close_Port

- Address: 1:0x0012D266 (linear 0x0013D266), 64 bytes, in `regions/07-object-1/001C2654-001C2E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

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
| 19 | scalar | unsigned long | unsigned 4 bytes |
| 20 | typedef | time_t | name for unsigned long (#19) |
| 21 | typedef | clock_t | name for unsigned long (#19) |
| 22 | typedef | time_t | name for unsigned long (#19) |
| 23 | typedef | wchar_t | name for unsigned short (#7) |
| 24 | typedef | ptrdiff_t | name for int (#8) |
| 25 | scalar | void | void 1 bytes |
| 26 | procedure |  | near386; returns void (char *) |
| 27 | pointer | PORT_DUMPER | near386; of void (char *) (#26) |
| 28 | typedef | PORT_DUMPER | name for void (char *) * (#27) |
| 29 | procedure |  | near386; returns void (char) |
| 30 | pointer | CHAR_PRINTER | near386; of void (char) (#29) |
| 31 | typedef | CHAR_PRINTER | name for void (char) * (#30) |
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
| 43 | pointer |  | near386; of unsigned char (#5) |
| 44 | procedure |  | near386; returns void (unsigned short, unsigned short, unsigned short, unsigned char *) |
| 45 | pointer |  | near386; of void (unsigned short, unsigned short, unsigned short, unsigned char *) (#44) |
| 46 | pointer |  | far386; of char (#10) |
| 47 | struct | struct _tag_farptr | 2 fields, 6 bytes |
| 48 | typedef | _tag_farptr | struct name for struct #47 (#47) |
| 49 | struct | GF_FARPTR32 | 2 fields, 6 bytes |
| 50 | typedef | GF_FARPTR32 | name for struct #49 (#49) |
| 51 | typedef | _tag_port | struct name for struct #106 (#106) |
| 52 | pointer |  | near386; of struct _tag_port (#51) |
| 53 | enum | DRIVER_TYPE | 20 constants, int 1 bytes |
| 54 | typedef | DRIVER_TYPE | name for enum #53 (#53) |
| 55 | procedure |  | near386; returns int (struct _tag_port *) |
| 56 | pointer |  | near386; of int (struct _tag_port *) (#55) |
| 57 | pointer |  | near386; of int (struct _tag_port *) (#55) |
| 58 | procedure |  | near386; returns int (struct _tag_port *, int) |
| 59 | pointer |  | near386; of int (struct _tag_port *, int) (#58) |
| 60 | pointer |  | near386; of int (struct _tag_port *) (#55) |
| 61 | procedure |  | near386; returns int (struct _tag_port *, long, char, int, int) |
| 62 | pointer |  | near386; of int (struct _tag_port *, long, char, int, int) (#61) |
| 63 | pointer |  | near386; of int (struct _tag_port *, int) (#58) |
| 64 | pointer |  | near386; of int (struct _tag_port *, int) (#58) |
| 65 | pointer |  | near386; of int (struct _tag_port *, int) (#58) |
| 66 | pointer |  | near386; of int (struct _tag_port *, int) (#58) |
| 67 | pointer |  | near386; of int (struct _tag_port *, int) (#58) |
| 68 | procedure |  | near386; returns long (struct _tag_port *) |
| 69 | pointer |  | near386; of long (struct _tag_port *) (#68) |
| 70 | pointer |  | near386; of long (struct _tag_port *) (#68) |
| 71 | pointer |  | near386; of long (struct _tag_port *) (#68) |
| 72 | pointer |  | near386; of long (struct _tag_port *) (#68) |
| 73 | pointer |  | near386; of int (struct _tag_port *) (#55) |
| 74 | procedure |  | near386; returns int (struct _tag_port *, char *, unsigned int) |
| 75 | pointer |  | near386; of int (struct _tag_port *, char *, unsigned int) (#74) |
| 76 | pointer |  | near386; of int (struct _tag_port *, char *, unsigned int) (#74) |
| 77 | procedure |  | near386; returns int (struct _tag_port *, PORT_DUMPER) |
| 78 | pointer |  | near386; of int (struct _tag_port *, PORT_DUMPER) (#77) |
| 79 | pointer |  | near386; of int (struct _tag_port *, int) (#58) |
| 80 | pointer |  | near386; of int (struct _tag_port *) (#55) |
| 81 | pointer |  | near386; of int (struct _tag_port *) (#55) |
| 82 | pointer |  | near386; of int (struct _tag_port *) (#55) |
| 83 | pointer |  | near386; of int (struct _tag_port *, int) (#58) |
| 84 | procedure |  | near386; returns void (struct _tag_port *) |
| 85 | pointer |  | near386; of void (struct _tag_port *) (#84) |
| 86 | pointer |  | far386; of void (#25) |
| 87 | typedef | PORT | name for struct _tag_port (#51) |
| 88 | pointer |  | near386; of PORT (#87) |
| 89 | procedure |  | near386; returns int (PORT *) |
| 90 | pointer |  | near386; of int (PORT *) (#89) |
| 91 | pointer |  | near386; of int (PORT *) (#89) |
| 92 | pointer |  | near386; of void (#25) |
| 93 | procedure |  | near386; returns void * (void *, char *, char *) |
| 94 | pointer |  | near386; of void * (void *, char *, char *) (#93) |
| 95 | procedure |  | near386; returns int (void *) |
| 96 | pointer |  | near386; of int (void *) (#95) |
| 97 | procedure |  | near386; returns char * (int) |
| 98 | pointer |  | near386; of char * (int) (#97) |
| 99 | enum | enum trigger_level, TRIGGER_LEVEL | 5 constants, unsigned 1 bytes |
| 100 | typedef | trigger_level | enum name for enum #99 (#99) |
| 101 | typedef | TRIGGER_LEVEL | name for enum #99 (#99) |
| 102 | pointer |  | near386; of void (char) (#29) |
| 103 | struct | struct GFINSTANCEDATAtag | 19 fields, 72 bytes |
| 104 | typedef | GFINSTANCEDATAtag | struct name for struct #103 (#103) |
| 105 | pointer |  | far386; of struct GFINSTANCEDATAtag (#104) |
| 106 | struct | struct _tag_port | 33 fields, 135 bytes |
| 107 | pointer |  | near386; of PORT (#87) |
| 108 | struct | struct s_modem_comm | 5 fields, 17 bytes |
| 109 | typedef | s_modem_comm | struct name for struct #108 (#108) |
| 110 | pointer |  | near386; of struct s_modem_comm (#109) |
| 111 | pointer |  | near386; of void (unsigned short, unsigned short, unsigned short, unsigned char *) (#44) |
| 112 | procedure |  | near386; returns int (struct s_modem_comm *, void (unsigned short, unsigned short, unsigned short, unsigned char *) *) |
| 113 | procedure |  | near386; returns void () |
| 114 | procedure |  | near386; returns int () |
| 115 | array |  | 0..29; of char (#10) |
| 116 | array |  | 0..29; of char (#10) |
| 117 | array |  | 0..29; of char (#10) |
| 118 | array |  | 0..39; of char (#10) |
| 119 | struct | struct s_modem_init_string | 5 fields, 131 bytes |
| 120 | typedef | s_modem_init_string | struct name for struct #119 (#119) |
| 121 | pointer |  | near386; of struct s_modem_init_string (#120) |
| 122 | procedure |  | near386; returns int (struct s_modem_init_string *) |
| 123 | array |  | 0..44; of char (#10) |
| 124 | array |  | 0..80; of char (#10) |
| 125 | array |  | 0..7; of char (#10) |
| 126 | procedure |  | near386; returns int (struct s_modem_comm *) |

##### struct _tag_farptr (#47, 2 fields, 6 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| offset | offset 0 | unsigned int |
| selector | offset 4 | unsigned short |

##### GF_FARPTR32 (#49, 2 fields, 6 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| s | offset 0 | struct _tag_farptr |
| p | offset 0 | char * |

##### DRIVER_TYPE (#53, 20 constants, int 1 bytes)

| Constant | Value |
|---|---|
| OTHER | 18 |
| NASI | 17 |
| MICROSOFT_WIN32S | 16 |
| MICROSOFT_WIN32 | 15 |
| MICROSOFT_WINDOWS | 14 |
| STARGATE | 13 |
| ARNET | 12 |
| SPARKLE | 11 |
| RATIONAL_SYSTEMS_DOS_16M | 10 |
| PHAR_LAP_286 | 9 |
| MODEM_ASSIST | 8 |
| GREENLEAF_FAST | 7 |
| DIGIBOARD_UNIVERSAL | 6 |
| DIGIBOARD_PCXE | 5 |
| DIGIBOARD_COMXI | 4 |
| FOSSIL | 3 |
| EXTENDED_BIOS | 2 |
| BIOS | 1 |
| GREENLEAF | 0 |
| OUT_OF_MEMORY | -1 |

##### enum trigger_level, TRIGGER_LEVEL (#99, 5 constants, unsigned 1 bytes)

| Constant | Value |
|---|---|
| TRIGGER_14 | 193 |
| TRIGGER_08 | 129 |
| TRIGGER_04 | 65 |
| TRIGGER_01 | 1 |
| TRIGGER_DISABLE | 0 |

##### struct GFINSTANCEDATAtag (#103, 19 fields, 72 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| _PortIdleFunctionPtr | offset 0 | int (PORT *) * |
| _AbortModemFunctionPtr | offset 4 | int (PORT *) * |
| _XferFileOpenFunctionPtr | offset 8 | void * (void *, char *, char *) * |
| _AbortXferFunctionPtr | offset 12 | int (void *) * |
| _AbortedTransfer | offset 16 | int |
| _xfer_last_status | offset 20 | void * |
| _DefaultAbortKey | offset 24 | int |
| _UserErrorNameFunctionPtr | offset 28 | char * (int) * |
| Default16550TriggerLevel | offset 32 | TRIGGER_LEVEL |
| DefaultFast16550TriggerLevel | offset 33 | TRIGGER_LEVEL |
| hm_delay_value | offset 34 | long |
| hm_match_string | offset 38 | char * |
| hm_character_printer | offset 44 | void (char) * |
| _hm_abort_key | offset 48 | int |
| _aserror | offset 52 | int |
| _DefaultRXBufferSize | offset 56 | unsigned int |
| _DefaultTXBufferSize | offset 60 | unsigned int |
| hTask | offset 64 | int |
| nRefCount | offset 68 | int |

##### struct _tag_port (#106, 33 fields, 135 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| driver | offset 0 | GF_FARPTR32 |
| next_port | offset 6 | struct _tag_port * |
| handle | offset 10 | int |
| status | offset 14 | int |
| driver_type | offset 18 | DRIVER_TYPE |
| dialing_method | offset 19 | int |
| count | offset 23 | unsigned int |
| read_char | offset 27 | int (struct _tag_port *) * |
| peek_char | offset 31 | int (struct _tag_port *) * |
| write_char | offset 35 | int (struct _tag_port *, int) * |
| port_close | offset 39 | int (struct _tag_port *) * |
| port_set | offset 43 | int (struct _tag_port *, long, char, int, int) * |
| use_xon_xoff | offset 47 | int (struct _tag_port *, int) * |
| use_rts_cts | offset 51 | int (struct _tag_port *, int) * |
| use_dtr_dsr | offset 55 | int (struct _tag_port *, int) * |
| set_dtr | offset 59 | int (struct _tag_port *, int) * |
| set_rts | offset 63 | int (struct _tag_port *, int) * |
| space_free_in_TX_buffer | offset 67 | long (struct _tag_port *) * |
| space_used_in_TX_buffer | offset 71 | long (struct _tag_port *) * |
| space_free_in_RX_buffer | offset 75 | long (struct _tag_port *) * |
| space_used_in_RX_buffer | offset 79 | long (struct _tag_port *) * |
| clear_TX_buffer | offset 83 | int (struct _tag_port *) * |
| write_buffer | offset 87 | int (struct _tag_port *, char *, unsigned int) * |
| read_buffer | offset 91 | int (struct _tag_port *, char *, unsigned int) * |
| dump_port_status | offset 95 | int (struct _tag_port *, PORT_DUMPER) * |
| send_break | offset 99 | int (struct _tag_port *, int) * |
| get_modem_status | offset 103 | int (struct _tag_port *) * |
| get_line_status | offset 107 | int (struct _tag_port *) * |
| clear_line_status | offset 111 | int (struct _tag_port *) * |
| block | offset 115 | int (struct _tag_port *, int) * |
| clear_error | offset 119 | void (struct _tag_port *) * |
| user_data | offset 123 | void * |
| lpThis | offset 129 | struct GFINSTANCEDATAtag * |

##### struct s_modem_comm (#108, 5 fields, 17 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| port_number | offset 0 | int |
| baud_rate | offset 4 | long |
| parity | offset 8 | char |
| word_length | offset 9 | int |
| stop_bits | offset 13 | int |

##### struct s_modem_init_string (#119, 5 fields, 131 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| clear | offset 0 | char[30] |
| init | offset 30 | char[30] |
| hang_up | offset 60 | char[30] |
| number | offset 90 | char[40] |
| tone_dialing | offset 130 | char |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 16 | 1:0x0012CC5B (linear 0x0013CC5B) | 0x001C22AF |
| 19 | 1:0x0012CC78 (linear 0x0013CC78) | 0x001C22CC |
| 21 | 1:0x0012CC84 (linear 0x0013CC84) | 0x001C22D8 |
| 23 | 1:0x0012CC8D (linear 0x0013CC8D) | 0x001C22E1 |
| 25 | 1:0x0012CC9B (linear 0x0013CC9B) | 0x001C22EF |
| 27 | 1:0x0012CCA4 (linear 0x0013CCA4) | 0x001C22F8 |
| 29 | 1:0x0012CCAC (linear 0x0013CCAC) | 0x001C2300 |
| 31 | 1:0x0012CCB3 (linear 0x0013CCB3) | 0x001C2307 |
| 32 | 1:0x0012CCBA (linear 0x0013CCBA) | 0x001C230E |
| 33 | 1:0x0012CCC5 (linear 0x0013CCC5) | 0x001C2319 |
| 35 | 1:0x0012CCDD (linear 0x0013CCDD) | 0x001C2331 |
| 39 | 1:0x0012CCE6 (linear 0x0013CCE6) | 0x001C233A |
| 40 | 1:0x0012CCEB (linear 0x0013CCEB) | 0x001C233F |
| 41 | 1:0x0012CCF0 (linear 0x0013CCF0) | 0x001C2344 |
| 43 | 1:0x0012CCF7 (linear 0x0013CCF7) | 0x001C234B |
| 48 | 1:0x0012CD0F (linear 0x0013CD0F) | 0x001C2363 |
| 49 | 1:0x0012CD16 (linear 0x0013CD16) | 0x001C236A |
| 51 | 1:0x0012CD29 (linear 0x0013CD29) | 0x001C237D |
| 52 | 1:0x0012CD3A (linear 0x0013CD3A) | 0x001C238E |
| 53 | 1:0x0012CD42 (linear 0x0013CD42) | 0x001C2396 |
| 54 | 1:0x0012CD49 (linear 0x0013CD49) | 0x001C239D |
| 55 | 1:0x0012CD4B (linear 0x0013CD4B) | 0x001C239F |
| 57 | 1:0x0012CD54 (linear 0x0013CD54) | 0x001C23A8 |
| 61 | 1:0x0012CD6C (linear 0x0013CD6C) | 0x001C23C0 |
| 63 | 1:0x0012CD75 (linear 0x0013CD75) | 0x001C23C9 |
| 65 | 1:0x0012CD88 (linear 0x0013CD88) | 0x001C23DC |
| 68 | 1:0x0012CD99 (linear 0x0013CD99) | 0x001C23ED |
| 70 | 1:0x0012CDA5 (linear 0x0013CDA5) | 0x001C23F9 |
| 72 | 1:0x0012CDBD (linear 0x0013CDBD) | 0x001C2411 |
| 75 | 1:0x0012CDC6 (linear 0x0013CDC6) | 0x001C241A |
| 76 | 1:0x0012CDCF (linear 0x0013CDCF) | 0x001C2423 |
| 77 | 1:0x0012CDD6 (linear 0x0013CDD6) | 0x001C242A |
| 78 | 1:0x0012CDE5 (linear 0x0013CDE5) | 0x001C2439 |
| 80 | 1:0x0012CDEE (linear 0x0013CDEE) | 0x001C2442 |
| 81 | 1:0x0012CDF5 (linear 0x0013CDF5) | 0x001C2449 |
| 82 | 1:0x0012CE01 (linear 0x0013CE01) | 0x001C2455 |
| 89 | 1:0x0012CE1C (linear 0x0013CE1C) | 0x001C2470 |
| 90 | 1:0x0012CE25 (linear 0x0013CE25) | 0x001C2479 |
| 93 | 1:0x0012CE2C (linear 0x0013CE2C) | 0x001C2480 |
| 95 | 1:0x0012CE39 (linear 0x0013CE39) | 0x001C248D |
| 96 | 1:0x0012CE46 (linear 0x0013CE46) | 0x001C249A |
| 97 | 1:0x0012CE52 (linear 0x0013CE52) | 0x001C24A6 |
| 98 | 1:0x0012CE62 (linear 0x0013CE62) | 0x001C24B6 |
| 99 | 1:0x0012CE6E (linear 0x0013CE6E) | 0x001C24C2 |
| 101 | 1:0x0012CE81 (linear 0x0013CE81) | 0x001C24D5 |
| 103 | 1:0x0012CE8D (linear 0x0013CE8D) | 0x001C24E1 |
| 105 | 1:0x0012CE8F (linear 0x0013CE8F) | 0x001C24E3 |
| 108 | 1:0x0012CE9E (linear 0x0013CE9E) | 0x001C24F2 |
| 109 | 1:0x0012CEA8 (linear 0x0013CEA8) | 0x001C24FC |
| 111 | 1:0x0012CEBC (linear 0x0013CEBC) | 0x001C2510 |
| 114 | 1:0x0012CEC5 (linear 0x0013CEC5) | 0x001C2519 |
| 115 | 1:0x0012CECC (linear 0x0013CECC) | 0x001C2520 |
| 117 | 1:0x0012CED8 (linear 0x0013CED8) | 0x001C252C |
| 123 | 1:0x0012CEF3 (linear 0x0013CEF3) | 0x001C2547 |
| 124 | 1:0x0012CEFA (linear 0x0013CEFA) | 0x001C254E |
| 126 | 1:0x0012CF02 (linear 0x0013CF02) | 0x001C2556 |
| 127 | 1:0x0012CF0D (linear 0x0013CF0D) | 0x001C2561 |
| 129 | 1:0x0012CF14 (linear 0x0013CF14) | 0x001C2568 |
| 131 | 1:0x0012CF1E (linear 0x0013CF1E) | 0x001C2572 |
| 133 | 1:0x0012CF35 (linear 0x0013CF35) | 0x001C2589 |
| 135 | 1:0x0012CF3D (linear 0x0013CF3D) | 0x001C2591 |
| 137 | 1:0x0012CF72 (linear 0x0013CF72) | 0x001C25C6 |
| 140 | 1:0x0012CF74 (linear 0x0013CF74) | 0x001C25C8 |
| 142 | 1:0x0012CF8F (linear 0x0013CF8F) | 0x001C25E3 |
| 143 | 1:0x0012CF96 (linear 0x0013CF96) | 0x001C25EA |
| 144 | 1:0x0012CFA1 (linear 0x0013CFA1) | 0x001C25F5 |
| 147 | 1:0x0012CFA3 (linear 0x0013CFA3) | 0x001C25F7 |
| 149 | 1:0x0012CFB0 (linear 0x0013CFB0) | 0x001C2604 |
| 150 | 1:0x0012CFB7 (linear 0x0013CFB7) | 0x001C260B |
| 151 | 1:0x0012CFC2 (linear 0x0013CFC2) | 0x001C2616 |
| 153 | 1:0x0012CFC4 (linear 0x0013CFC4) | 0x001C2618 |
| 155 | 1:0x0012CFCF (linear 0x0013CFCF) | 0x001C2623 |
| 158 | 1:0x0012CFD4 (linear 0x0013CFD4) | 0x001C2628 |
| 159 | 1:0x0012CFDD (linear 0x0013CFDD) | 0x001C2631 |
| 161 | 1:0x0012CFF5 (linear 0x0013CFF5) | 0x001C2649 |
| 163 | 1:0x0012CFFE (linear 0x0013CFFE) | 0x001C2652 |
| 165 | 1:0x0012D00C (linear 0x0013D00C) | 0x001C2660 |
| 169 | 1:0x0012D015 (linear 0x0013D015) | 0x001C2669 |
| 170 | 1:0x0012D01C (linear 0x0013D01C) | 0x001C2670 |
| 171 | 1:0x0012D028 (linear 0x0013D028) | 0x001C267C |
| 173 | 1:0x0012D043 (linear 0x0013D043) | 0x001C2697 |
| 177 | 1:0x0012D050 (linear 0x0013D050) | 0x001C26A4 |
| 178 | 1:0x0012D05A (linear 0x0013D05A) | 0x001C26AE |
| 179 | 1:0x0012D064 (linear 0x0013D064) | 0x001C26B8 |
| 180 | 1:0x0012D06E (linear 0x0013D06E) | 0x001C26C2 |
| 181 | 1:0x0012D078 (linear 0x0013D078) | 0x001C26CC |
| 182 | 1:0x0012D084 (linear 0x0013D084) | 0x001C26D8 |
| 183 | 1:0x0012D094 (linear 0x0013D094) | 0x001C26E8 |
| 184 | 1:0x0012D0A0 (linear 0x0013D0A0) | 0x001C26F4 |
| 185 | 1:0x0012D0AD (linear 0x0013D0AD) | 0x001C2701 |
| 186 | 1:0x0012D0B9 (linear 0x0013D0B9) | 0x001C270D |
| 187 | 1:0x0012D0C9 (linear 0x0013D0C9) | 0x001C271D |
| 189 | 1:0x0012D0D5 (linear 0x0013D0D5) | 0x001C2729 |
| 191 | 1:0x0012D0DE (linear 0x0013D0DE) | 0x001C2732 |
| 193 | 1:0x0012D0E5 (linear 0x0013D0E5) | 0x001C2739 |
| 194 | 1:0x0012D0F1 (linear 0x0013D0F1) | 0x001C2745 |
| 196 | 1:0x0012D10C (linear 0x0013D10C) | 0x001C2760 |
| 198 | 1:0x0012D115 (linear 0x0013D115) | 0x001C2769 |
| 199 | 1:0x0012D11F (linear 0x0013D11F) | 0x001C2773 |
| 200 | 1:0x0012D129 (linear 0x0013D129) | 0x001C277D |
| 201 | 1:0x0012D133 (linear 0x0013D133) | 0x001C2787 |
| 202 | 1:0x0012D13F (linear 0x0013D13F) | 0x001C2793 |
| 203 | 1:0x0012D14C (linear 0x0013D14C) | 0x001C27A0 |
| 204 | 1:0x0012D158 (linear 0x0013D158) | 0x001C27AC |
| 205 | 1:0x0012D168 (linear 0x0013D168) | 0x001C27BC |
| 206 | 1:0x0012D174 (linear 0x0013D174) | 0x001C27C8 |
| 208 | 1:0x0012D17D (linear 0x0013D17D) | 0x001C27D1 |
| 210 | 1:0x0012D184 (linear 0x0013D184) | 0x001C27D8 |
| 212 | 1:0x0012D190 (linear 0x0013D190) | 0x001C27E4 |
| 214 | 1:0x0012D1AB (linear 0x0013D1AB) | 0x001C27FF |
| 215 | 1:0x0012D1BE (linear 0x0013D1BE) | 0x001C2812 |
| 216 | 1:0x0012D1C7 (linear 0x0013D1C7) | 0x001C281B |
| 218 | 1:0x0012D1E2 (linear 0x0013D1E2) | 0x001C2836 |
| 220 | 1:0x0012D1F1 (linear 0x0013D1F1) | 0x001C2845 |
| 221 | 1:0x0012D1F8 (linear 0x0013D1F8) | 0x001C284C |
| 222 | 1:0x0012D202 (linear 0x0013D202) | 0x001C2856 |
| 224 | 1:0x0012D20C (linear 0x0013D20C) | 0x001C2860 |
| 225 | 1:0x0012D235 (linear 0x0013D235) | 0x001C2889 |
| 227 | 1:0x0012D240 (linear 0x0013D240) | 0x001C2894 |
| 228 | 1:0x0012D24A (linear 0x0013D24A) | 0x001C289E |
| 230 | 1:0x0012D253 (linear 0x0013D253) | 0x001C28A7 |
| 231 | 1:0x0012D25A (linear 0x0013D25A) | 0x001C28AE |
| 233 | 1:0x0012D266 (linear 0x0013D266) | 0x001C28BA |
| 235 | 1:0x0012D27E (linear 0x0013D27E) | 0x001C28D2 |
| 237 | 1:0x0012D287 (linear 0x0013D287) | 0x001C28DB |
| 238 | 1:0x0012D295 (linear 0x0013D295) | 0x001C28E9 |
| 240 | 1:0x0012D29F (linear 0x0013D29F) | 0x001C28F3 |

