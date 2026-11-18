# F:\WLIBS\net\netcode.c

Module 187 of the debug info of ORION2.EXE.

Ranges: 1:0x00128F31 (linear 0x00138F31), 2,766 bytes, 2:0x0000C64C (linear 0x0017C64C), 36 bytes, 2:0x000450A0 (linear 0x001B50A0), 180 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x00128F31 (linear 0x00138F31) | code | _netLobby | `regions/07-object-1/001BDE54-001BE653.txt` |
| 1:0x00128F3B (linear 0x00138F3B) | code | Net_Init_ | `regions/07-object-1/001BDE54-001BE653.txt` |
| 1:0x00129062 (linear 0x00139062) | code | Net_Uninit_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x001290BB (linear 0x001390BB) | code | Net_Update_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x00129127 (linear 0x00139127) | code | Net_Is_Sending_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x00129153 (linear 0x00139153) | code | Net_Send_Message_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x0012920C (linear 0x0013920C) | code | Net_Status_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x00129294 (linear 0x00139294) | code | Net_Get_Users_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x00129358 (linear 0x00139358) | code | Set_Null_Modem_Slave_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x00129387 (linear 0x00139387) | code | Set_Null_Modem_Host_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x001293B6 (linear 0x001393B6) | code | Net_Is_User_Valid_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x00129447 (linear 0x00139447) | code | Net_Host_ID_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x00129476 (linear 0x00139476) | code | Net_Update_Game_List_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x00129659 (linear 0x00139659) | code | Net_Get_Game_Count_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x0012968A (linear 0x0013968A) | code | Net_Get_Game_Name_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x001296F8 (linear 0x001396F8) | code | Net_Get_Game_Status_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x0012974F (linear 0x0013974F) | code | Net_Join_Game_ | `regions/07-object-1/001BE654-001BEE53.txt` |
| 1:0x0012982D (linear 0x0013982D) | code | Net_Create_Game_ | `regions/07-object-1/001BEE54-001BF653.txt` |
| 1:0x001298EA (linear 0x001398EA) | code | Net_Get_Player_Count_ | `regions/07-object-1/001BEE54-001BF653.txt` |
| 1:0x0012994E (linear 0x0013994E) | code | Net_Enable_New_Players_ | `regions/07-object-1/001BEE54-001BF653.txt` |
| 1:0x0012998F (linear 0x0013998F) | code | Net_Disable_New_Players_ | `regions/07-object-1/001BEE54-001BF653.txt` |
| 1:0x001299D0 (linear 0x001399D0) | code | Net_New_Players_Enabled_ | `regions/07-object-1/001BEE54-001BF653.txt` |
| 2:0x0000C64C (linear 0x0017C64C) | data | _netInitialized | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x0000C650 (linear 0x0017C650) | data | _netGameOpen | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x0000C654 (linear 0x0017C654) | data | _globalGameID | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x0000C668 (linear 0x0017C668) | data | _numCurrentGames | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x000450A0 (linear 0x001B50A0) | data | _netGameList | - |
| 2:0x00045150 (linear 0x001B5150) | data | _connection_type | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x000450A0 (linear 0x001B50A0) | netGameList | NetGameData[16] |
| 2:0x00045150 (linear 0x001B5150) | connection_type | int |
| 2:0x0000C668 (linear 0x0017C668) | numCurrentGames | int |
| 2:0x0000C654 (linear 0x0017C654) | globalGameID | char[20] |
| 2:0x0000C650 (linear 0x0017C650) | netGameOpen | int |
| 2:0x0000C64C (linear 0x0017C64C) | netInitialized | int |
| 1:0x00128F31 (linear 0x00138F31) | netLobby | char[10] |

#### Net_Init

- Address: 1:0x00128F3B (linear 0x00138F3B), 295 bytes, in `regions/07-object-1/001BDE54-001BE653.txt`
- Type: int (int, int, void (unsigned short, unsigned short, unsigned short, unsigned char *) *, struct s_modem_comm *); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| type | [EBP-0x14] | int |
| gameID | [EBP-0x10] | int |
| receiveMessage | [EBP-0xC] | void (unsigned short, unsigned short, unsigned short, unsigned char *) * |
| modem_settings | [EBP-0x8] | struct s_modem_comm * |

#### Net_Uninit

- Address: 1:0x00129062 (linear 0x00139062), 89 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Net_Update

- Address: 1:0x001290BB (linear 0x001390BB), 108 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| reentrant | address 2:0x0000C66C | int |

#### Net_Is_Sending

- Address: 1:0x00129127 (linear 0x00139127), 44 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Net_Send_Message

- Address: 1:0x00129153 (linear 0x00139153), 185 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: int (int, unsigned short, void *, unsigned short); returns in EAX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| dest | [EBP-0x14] | int |
| userType | [EBP-0x8] | unsigned short |
| data | [EBP-0x10] | void * |
| dataSize | [EBP-0x4] | unsigned short |

#### Net_Status

- Address: 1:0x0012920C (linear 0x0013920C), 136 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| status | [EBP-0x8] | int |

#### Net_Get_Users

- Address: 1:0x00129294 (linear 0x00139294), 196 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: int (char *); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| userList | [EBP-0x10] | char * |
| i | [EBP-0xC] | int |
| st | [EBP-0x8] | DosStationDesc * |

#### Set_Null_Modem_Slave

- Address: 1:0x00129358 (linear 0x00139358), 47 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Set_Null_Modem_Host

- Address: 1:0x00129387 (linear 0x00139387), 47 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Net_Is_User_Valid

- Address: 1:0x001293B6 (linear 0x001393B6), 145 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: int (int); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| id | [EBP-0xC] | int |
| st | [EBP-0x8] | DosStationDesc * |

#### Net_Host_ID

- Address: 1:0x00129447 (linear 0x00139447), 47 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Block at 1:0x001295AD (linear 0x001395AD), 142 bytes

| Local | Location | Type |
|---|---|---|
| temp | [EBP-0x28] | char[20] |
| open | [EBP-0x2C] | char |

#### Net_Update_Game_List

- Address: 1:0x00129476 (linear 0x00139476), 483 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x14] | int |
| j | [EBP-0x10] | int |
| onlist | [EBP-0xC] | int |
| st | [EBP-0x8] | DosStationDesc * |

#### Net_Get_Game_Count

- Address: 1:0x00129659 (linear 0x00139659), 49 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Net_Get_Game_Name

- Address: 1:0x0012968A (linear 0x0013968A), 110 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: char * (int); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| game | [EBP-0x8] | int |

#### Net_Get_Game_Status

- Address: 1:0x001296F8 (linear 0x001396F8), 87 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: int (int); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| game | [EBP-0x8] | int |

#### Net_Join_Game

- Address: 1:0x0012974F (linear 0x0013974F), 222 bytes, in `regions/07-object-1/001BE654-001BEE53.txt`
- Type: int (char *, char *, char *); returns in EAX
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| gameName | [EBP-0x18] | char * |
| password | [EBP-0x14] | char * |
| playerName | [EBP-0x10] | char * |
| i | [EBP-0xC] | int |
| st | [EBP-0x8] | DosStationDesc * |

#### Net_Create_Game

- Address: 1:0x0012982D (linear 0x0013982D), 189 bytes, in `regions/07-object-1/001BEE54-001BF653.txt`
- Type: int (int, char *, int, char *, char *); returns in EAX
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| gameId | [EBP-0x1C] | int |
| gameName | [EBP-0x18] | char * |
| numPlayers | [EBP-0x14] | int |
| password | [EBP-0x10] | char * |
| playerName | [EBP+0x10] | char * |
| i | [EBP-0xC] | int |
| st | [EBP-0x8] | DosStationDesc * |

#### Net_Get_Player_Count

- Address: 1:0x001298EA (linear 0x001398EA), 100 bytes, in `regions/07-object-1/001BEE54-001BF653.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| retval | [EBP-0xC] | int |
| st | [EBP-0x8] | DosStationDesc * |

#### Net_Enable_New_Players

- Address: 1:0x0012994E (linear 0x0013994E), 65 bytes, in `regions/07-object-1/001BEE54-001BF653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Net_Disable_New_Players

- Address: 1:0x0012998F (linear 0x0013998F), 65 bytes, in `regions/07-object-1/001BEE54-001BF653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Net_New_Players_Enabled

- Address: 1:0x001299D0 (linear 0x001399D0), 47 bytes, in `regions/07-object-1/001BEE54-001BF653.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

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
| 13 | scalar | unsigned long | unsigned 4 bytes |
| 14 | typedef | time_t | name for unsigned long (#13) |
| 15 | typedef | clock_t | name for unsigned long (#13) |
| 16 | typedef | time_t | name for unsigned long (#13) |
| 17 | typedef | ushort | name for unsigned short (#7) |
| 18 | typedef | ulong | name for unsigned long (#13) |
| 19 | typedef | uint | name for unsigned int (#9) |
| 20 | typedef | uchar | name for unsigned char (#5) |
| 21 | typedef | boolean | name for int (#8) |
| 22 | scalar | float | float 4 bytes |
| 23 | typedef | bigScalar | name for float (#22) |
| 24 | typedef | smallScalar | name for float (#22) |
| 25 | pointer |  | near386; of char (#10) |
| 26 | array | __va_list | 0..0; of char * (#25) |
| 27 | typedef | __va_list | name for char *[1] (#26) |
| 28 | scalar | long | int 4 bytes |
| 29 | typedef | fpos_t | name for long (#28) |
| 30 | typedef | fpos_t | name for long (#28) |
| 31 | array |  | 0..9; of char (#10) |
| 32 | struct | NetGameData | 2 fields, 11 bytes |
| 33 | typedef | NetGameData | name for struct #32 (#32) |
| 34 | array |  | 0..15; of NetGameData (#33) |
| 35 | array |  | 0..19; of char (#10) |
| 36 | array |  | 0..9; of char (#11) |
| 37 | scalar | void | void 1 bytes |
| 38 | pointer |  | near386; of unsigned char (#5) |
| 39 | procedure |  | near386; returns void (unsigned short, unsigned short, unsigned short, unsigned char *) |
| 40 | pointer |  | near386; of void (unsigned short, unsigned short, unsigned short, unsigned char *) (#39) |
| 41 | struct | struct s_modem_comm | 0 fields, 0 bytes |
| 42 | typedef | s_modem_comm | struct name for struct #41 (#41) |
| 43 | pointer |  | near386; of struct s_modem_comm (#42) |
| 44 | procedure |  | near386; returns int (int, int, void (unsigned short, unsigned short, unsigned short, unsigned char *) *, struct s_modem_comm *) |
| 45 | procedure |  | near386; returns void () |
| 46 | procedure |  | near386; returns int () |
| 47 | pointer |  | near386; of void (#37) |
| 48 | procedure |  | near386; returns int (int, unsigned short, void *, unsigned short) |
| 49 | procedure |  | near386; returns int (char *) |
| 50 | array |  | 0..9; of uchar (#20) |
| 51 | struct | NetStationDesc | 6 fields, 22 bytes |
| 52 | typedef | NetStationDesc | name for struct #51 (#51) |
| 53 | typedef | dosdesc | struct name for struct #55 (#55) |
| 54 | pointer |  | near386; of struct dosdesc (#53) |
| 55 | struct | struct dosdesc | 4 fields, 34 bytes |
| 56 | typedef | DosStationDesc | name for struct dosdesc (#53) |
| 57 | pointer |  | near386; of DosStationDesc (#56) |
| 58 | procedure |  | near386; returns void (void) |
| 59 | procedure |  | near386; returns int (int) |
| 60 | array |  | 0..19; of char (#10) |
| 61 | procedure |  | near386; returns char * (int) |
| 62 | procedure |  | near386; returns int (char *, char *, char *) |
| 63 | procedure |  | near386; returns int (int, char *, int, char *, char *) |
| 64 | procedure |  | near386; returns int (void) |

##### NetGameData (#32, 2 fields, 11 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| name | offset 0 | char[10] |
| open | offset 10 | char |

##### NetStationDesc (#51, 6 fields, 22 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| stationId | offset 0 | uchar |
| open | offset 1 | uchar |
| gameId | offset 2 | uchar[10] |
| network | offset 12 | int |
| network_node1 | offset 16 | int |
| network_node2 | offset 20 | short |

##### struct dosdesc (#55, 4 fields, 34 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| stationdesc | offset 0 | NetStationDesc |
| lastKeepAliveMsg | offset 22 | ulong |
| stationinittime | offset 26 | long |
| next | offset 30 | struct dosdesc * |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 52 | 1:0x00128F3B (linear 0x00138F3B) | 0x001BE58F |
| 54 | 1:0x00128F5C (linear 0x00138F5C) | 0x001BE5B0 |
| 56 | 1:0x00128F64 (linear 0x00138F64) | 0x001BE5B8 |
| 58 | 1:0x00128F6E (linear 0x00138F6E) | 0x001BE5C2 |
| 59 | 1:0x00128F77 (linear 0x00138F77) | 0x001BE5CB |
| 61 | 1:0x00128F83 (linear 0x00138F83) | 0x001BE5D7 |
| 62 | 1:0x00128F90 (linear 0x00138F90) | 0x001BE5E4 |
| 68 | 1:0x00128F9C (linear 0x00138F9C) | 0x001BE5F0 |
| 70 | 1:0x00128FAB (linear 0x00138FAB) | 0x001BE5FF |
| 71 | 1:0x00128FB5 (linear 0x00138FB5) | 0x001BE609 |
| 74 | 1:0x00128FFF (linear 0x00138FFF) | 0x001BE653 |
| 75 | 1:0x00129009 (linear 0x00139009) | 0x001BE65D |
| 79 | 1:0x00129012 (linear 0x00139012) | 0x001BE666 |
| 83 | 1:0x0012901B (linear 0x0013901B) | 0x001BE66F |
| 87 | 1:0x00129024 (linear 0x00139024) | 0x001BE678 |
| 91 | 1:0x0012902D (linear 0x0013902D) | 0x001BE681 |
| 94 | 1:0x00129036 (linear 0x00139036) | 0x001BE68A |
| 95 | 1:0x00129042 (linear 0x00139042) | 0x001BE696 |
| 97 | 1:0x00129052 (linear 0x00139052) | 0x001BE6A6 |
| 98 | 1:0x00129059 (linear 0x00139059) | 0x001BE6AD |
| 101 | 1:0x00129062 (linear 0x00139062) | 0x001BE6B6 |
| 103 | 1:0x0012907A (linear 0x0013907A) | 0x001BE6CE |
| 105 | 1:0x00129083 (linear 0x00139083) | 0x001BE6D7 |
| 110 | 1:0x0012908C (linear 0x0013908C) | 0x001BE6E0 |
| 111 | 1:0x00129091 (linear 0x00139091) | 0x001BE6E5 |
| 114 | 1:0x0012909B (linear 0x0013909B) | 0x001BE6EF |
| 115 | 1:0x001290AF (linear 0x001390AF) | 0x001BE703 |
| 116 | 1:0x001290B4 (linear 0x001390B4) | 0x001BE708 |
| 119 | 1:0x001290BB (linear 0x001390BB) | 0x001BE70F |
| 123 | 1:0x001290D3 (linear 0x001390D3) | 0x001BE727 |
| 126 | 1:0x001290DC (linear 0x001390DC) | 0x001BE730 |
| 128 | 1:0x001290E6 (linear 0x001390E6) | 0x001BE73A |
| 130 | 1:0x001290EF (linear 0x001390EF) | 0x001BE743 |
| 131 | 1:0x001290F8 (linear 0x001390F8) | 0x001BE74C |
| 133 | 1:0x001290FD (linear 0x001390FD) | 0x001BE751 |
| 134 | 1:0x00129111 (linear 0x00139111) | 0x001BE765 |
| 136 | 1:0x00129116 (linear 0x00139116) | 0x001BE76A |
| 137 | 1:0x00129120 (linear 0x00139120) | 0x001BE774 |
| 140 | 1:0x00129127 (linear 0x00139127) | 0x001BE77B |
| 142 | 1:0x0012913F (linear 0x0013913F) | 0x001BE793 |
| 143 | 1:0x00129147 (linear 0x00139147) | 0x001BE79B |
| 146 | 1:0x00129153 (linear 0x00139153) | 0x001BE7A7 |
| 148 | 1:0x00129174 (linear 0x00139174) | 0x001BE7C8 |
| 150 | 1:0x0012917D (linear 0x0013917D) | 0x001BE7D1 |
| 151 | 1:0x00129186 (linear 0x00139186) | 0x001BE7DA |
| 153 | 1:0x00129192 (linear 0x00139192) | 0x001BE7E6 |
| 154 | 1:0x0012919B (linear 0x0013919B) | 0x001BE7EF |
| 156 | 1:0x001291A2 (linear 0x001391A2) | 0x001BE7F6 |
| 161 | 1:0x001291C1 (linear 0x001391C1) | 0x001BE815 |
| 164 | 1:0x001291CA (linear 0x001391CA) | 0x001BE81E |
| 166 | 1:0x001291DC (linear 0x001391DC) | 0x001BE830 |
| 167 | 1:0x001291F3 (linear 0x001391F3) | 0x001BE847 |
| 170 | 1:0x001291FC (linear 0x001391FC) | 0x001BE850 |
| 171 | 1:0x00129203 (linear 0x00139203) | 0x001BE857 |
| 174 | 1:0x0012920C (linear 0x0013920C) | 0x001BE860 |
| 176 | 1:0x00129224 (linear 0x00139224) | 0x001BE878 |
| 178 | 1:0x0012922B (linear 0x0013922B) | 0x001BE87F |
| 180 | 1:0x00129234 (linear 0x00139234) | 0x001BE888 |
| 181 | 1:0x0012923D (linear 0x0013923D) | 0x001BE891 |
| 183 | 1:0x00129241 (linear 0x00139241) | 0x001BE895 |
| 184 | 1:0x00129255 (linear 0x00139255) | 0x001BE8A9 |
| 186 | 1:0x00129259 (linear 0x00139259) | 0x001BE8AD |
| 187 | 1:0x00129262 (linear 0x00139262) | 0x001BE8B6 |
| 189 | 1:0x00129266 (linear 0x00139266) | 0x001BE8BA |
| 190 | 1:0x0012927A (linear 0x0013927A) | 0x001BE8CE |
| 192 | 1:0x00129282 (linear 0x00139282) | 0x001BE8D6 |
| 193 | 1:0x00129288 (linear 0x00139288) | 0x001BE8DC |
| 196 | 1:0x00129294 (linear 0x00139294) | 0x001BE8E8 |
| 198 | 1:0x001292AF (linear 0x001392AF) | 0x001BE903 |
| 199 | 1:0x001292B6 (linear 0x001392B6) | 0x001BE90A |
| 201 | 1:0x001292BE (linear 0x001392BE) | 0x001BE912 |
| 203 | 1:0x001292C7 (linear 0x001392C7) | 0x001BE91B |
| 205 | 1:0x001292CD (linear 0x001392CD) | 0x001BE921 |
| 207 | 1:0x001292E1 (linear 0x001392E1) | 0x001BE935 |
| 208 | 1:0x001292E7 (linear 0x001392E7) | 0x001BE93B |
| 210 | 1:0x001292F4 (linear 0x001392F4) | 0x001BE948 |
| 212 | 1:0x001292FA (linear 0x001392FA) | 0x001BE94E |
| 213 | 1:0x00129303 (linear 0x00139303) | 0x001BE957 |
| 215 | 1:0x00129305 (linear 0x00139305) | 0x001BE959 |
| 217 | 1:0x0012930D (linear 0x0013930D) | 0x001BE961 |
| 219 | 1:0x0012931F (linear 0x0013931F) | 0x001BE973 |
| 221 | 1:0x00129325 (linear 0x00139325) | 0x001BE979 |
| 222 | 1:0x00129330 (linear 0x00139330) | 0x001BE984 |
| 225 | 1:0x0012933C (linear 0x0013933C) | 0x001BE990 |
| 228 | 1:0x00129345 (linear 0x00139345) | 0x001BE999 |
| 229 | 1:0x0012934C (linear 0x0013934C) | 0x001BE9A0 |
| 232 | 1:0x00129358 (linear 0x00139358) | 0x001BE9AC |
| 234 | 1:0x00129370 (linear 0x00139370) | 0x001BE9C4 |
| 235 | 1:0x00129379 (linear 0x00139379) | 0x001BE9CD |
| 236 | 1:0x00129380 (linear 0x00139380) | 0x001BE9D4 |
| 239 | 1:0x00129387 (linear 0x00139387) | 0x001BE9DB |
| 241 | 1:0x0012939F (linear 0x0013939F) | 0x001BE9F3 |
| 242 | 1:0x001293A8 (linear 0x001393A8) | 0x001BE9FC |
| 243 | 1:0x001293AF (linear 0x001393AF) | 0x001BEA03 |
| 246 | 1:0x001293B6 (linear 0x001393B6) | 0x001BEA0A |
| 248 | 1:0x001293D1 (linear 0x001393D1) | 0x001BEA25 |
| 250 | 1:0x001293D9 (linear 0x001393D9) | 0x001BEA2D |
| 252 | 1:0x001293E2 (linear 0x001393E2) | 0x001BEA36 |
| 254 | 1:0x001293E8 (linear 0x001393E8) | 0x001BEA3C |
| 255 | 1:0x001293F7 (linear 0x001393F7) | 0x001BEA4B |
| 257 | 1:0x00129400 (linear 0x00139400) | 0x001BEA54 |
| 258 | 1:0x00129409 (linear 0x00139409) | 0x001BEA5D |
| 260 | 1:0x0012940B (linear 0x0013940B) | 0x001BEA5F |
| 262 | 1:0x0012941F (linear 0x0013941F) | 0x001BEA73 |
| 264 | 1:0x0012942B (linear 0x0013942B) | 0x001BEA7F |
| 268 | 1:0x00129434 (linear 0x00139434) | 0x001BEA88 |
| 269 | 1:0x0012943B (linear 0x0013943B) | 0x001BEA8F |
| 272 | 1:0x00129447 (linear 0x00139447) | 0x001BEA9B |
| 274 | 1:0x0012945F (linear 0x0013945F) | 0x001BEAB3 |
| 275 | 1:0x0012946A (linear 0x0013946A) | 0x001BEABE |
| 279 | 1:0x00129476 (linear 0x00139476) | 0x001BEACA |
| 284 | 1:0x0012948E (linear 0x0013948E) | 0x001BEAE2 |
| 286 | 1:0x00129496 (linear 0x00139496) | 0x001BEAEA |
| 288 | 1:0x001294A0 (linear 0x001394A0) | 0x001BEAF4 |
| 290 | 1:0x001294AA (linear 0x001394AA) | 0x001BEAFE |
| 291 | 1:0x001294B1 (linear 0x001394B1) | 0x001BEB05 |
| 292 | 1:0x001294CD (linear 0x001394CD) | 0x001BEB21 |
| 294 | 1:0x001294E7 (linear 0x001394E7) | 0x001BEB3B |
| 295 | 1:0x001294EE (linear 0x001394EE) | 0x001BEB42 |
| 296 | 1:0x001294FE (linear 0x001394FE) | 0x001BEB52 |
| 298 | 1:0x00129500 (linear 0x00139500) | 0x001BEB54 |
| 300 | 1:0x0012952F (linear 0x0013952F) | 0x001BEB83 |
| 301 | 1:0x00129548 (linear 0x00139548) | 0x001BEB9C |
| 302 | 1:0x0012955B (linear 0x0013955B) | 0x001BEBAF |
| 305 | 1:0x00129561 (linear 0x00139561) | 0x001BEBB5 |
| 306 | 1:0x0012956A (linear 0x0013956A) | 0x001BEBBE |
| 308 | 1:0x0012956F (linear 0x0013956F) | 0x001BEBC3 |
| 309 | 1:0x0012958E (linear 0x0013958E) | 0x001BEBE2 |
| 314 | 1:0x001295AD (linear 0x001395AD) | 0x001BEC01 |
| 316 | 1:0x001295CC (linear 0x001395CC) | 0x001BEC20 |
| 317 | 1:0x001295DF (linear 0x001395DF) | 0x001BEC33 |
| 319 | 1:0x001295EC (linear 0x001395EC) | 0x001BEC40 |
| 320 | 1:0x00129607 (linear 0x00139607) | 0x001BEC5B |
| 322 | 1:0x0012961B (linear 0x0013961B) | 0x001BEC6F |
| 323 | 1:0x0012962E (linear 0x0013962E) | 0x001BEC82 |
| 325 | 1:0x0012963B (linear 0x0013963B) | 0x001BEC8F |
| 327 | 1:0x00129645 (linear 0x00139645) | 0x001BEC99 |
| 328 | 1:0x0012964D (linear 0x0013964D) | 0x001BECA1 |
| 331 | 1:0x00129659 (linear 0x00139659) | 0x001BECAD |
| 333 | 1:0x00129671 (linear 0x00139671) | 0x001BECC5 |
| 335 | 1:0x00129676 (linear 0x00139676) | 0x001BECCA |
| 336 | 1:0x0012967E (linear 0x0013967E) | 0x001BECD2 |
| 339 | 1:0x0012968A (linear 0x0013968A) | 0x001BECDE |
| 341 | 1:0x001296A5 (linear 0x001396A5) | 0x001BECF9 |
| 343 | 1:0x001296AA (linear 0x001396AA) | 0x001BECFE |
| 344 | 1:0x001296BD (linear 0x001396BD) | 0x001BED11 |
| 346 | 1:0x001296CD (linear 0x001396CD) | 0x001BED21 |
| 348 | 1:0x001296DB (linear 0x001396DB) | 0x001BED2F |
| 349 | 1:0x001296EC (linear 0x001396EC) | 0x001BED40 |
| 352 | 1:0x001296F8 (linear 0x001396F8) | 0x001BED4C |
| 354 | 1:0x00129713 (linear 0x00139713) | 0x001BED67 |
| 356 | 1:0x00129718 (linear 0x00139718) | 0x001BED6C |
| 357 | 1:0x0012972B (linear 0x0013972B) | 0x001BED7F |
| 359 | 1:0x0012973C (linear 0x0013973C) | 0x001BED90 |
| 360 | 1:0x00129743 (linear 0x00139743) | 0x001BED97 |
| 363 | 1:0x0012974F (linear 0x0013974F) | 0x001BEDA3 |
| 366 | 1:0x0012976E (linear 0x0013976E) | 0x001BEDC2 |
| 368 | 1:0x00129776 (linear 0x00139776) | 0x001BEDCA |
| 370 | 1:0x0012977B (linear 0x0013977B) | 0x001BEDCF |
| 371 | 1:0x0012979A (linear 0x0013979A) | 0x001BEDEE |
| 373 | 1:0x001297B1 (linear 0x001397B1) | 0x001BEE05 |
| 374 | 1:0x001297C6 (linear 0x001397C6) | 0x001BEE1A |
| 376 | 1:0x001297CF (linear 0x001397CF) | 0x001BEE23 |
| 378 | 1:0x001297D5 (linear 0x001397D5) | 0x001BEE29 |
| 379 | 1:0x001297E8 (linear 0x001397E8) | 0x001BEE3C |
| 381 | 1:0x001297F6 (linear 0x001397F6) | 0x001BEE4A |
| 382 | 1:0x001297FF (linear 0x001397FF) | 0x001BEE53 |
| 383 | 1:0x00129801 (linear 0x00139801) | 0x001BEE55 |
| 384 | 1:0x0012980E (linear 0x0013980E) | 0x001BEE62 |
| 385 | 1:0x00129817 (linear 0x00139817) | 0x001BEE6B |
| 387 | 1:0x0012981C (linear 0x0013981C) | 0x001BEE70 |
| 388 | 1:0x00129823 (linear 0x00139823) | 0x001BEE77 |
| 391 | 1:0x0012982D (linear 0x0013982D) | 0x001BEE81 |
| 394 | 1:0x0012984E (linear 0x0013984E) | 0x001BEEA2 |
| 396 | 1:0x00129856 (linear 0x00139856) | 0x001BEEAA |
| 398 | 1:0x0012985B (linear 0x0013985B) | 0x001BEEAF |
| 399 | 1:0x00129877 (linear 0x00139877) | 0x001BEECB |
| 400 | 1:0x0012988E (linear 0x0013988E) | 0x001BEEE2 |
| 402 | 1:0x00129899 (linear 0x00139899) | 0x001BEEED |
| 404 | 1:0x001298A6 (linear 0x001398A6) | 0x001BEEFA |
| 406 | 1:0x001298AC (linear 0x001398AC) | 0x001BEF00 |
| 407 | 1:0x001298BF (linear 0x001398BF) | 0x001BEF13 |
| 409 | 1:0x001298CD (linear 0x001398CD) | 0x001BEF21 |
| 410 | 1:0x001298D6 (linear 0x001398D6) | 0x001BEF2A |
| 412 | 1:0x001298D8 (linear 0x001398D8) | 0x001BEF2C |
| 413 | 1:0x001298DF (linear 0x001398DF) | 0x001BEF33 |
| 416 | 1:0x001298EA (linear 0x001398EA) | 0x001BEF3E |
| 418 | 1:0x00129902 (linear 0x00139902) | 0x001BEF56 |
| 419 | 1:0x00129909 (linear 0x00139909) | 0x001BEF5D |
| 421 | 1:0x00129911 (linear 0x00139911) | 0x001BEF65 |
| 423 | 1:0x00129917 (linear 0x00139917) | 0x001BEF6B |
| 424 | 1:0x0012992B (linear 0x0013992B) | 0x001BEF7F |
| 426 | 1:0x00129931 (linear 0x00139931) | 0x001BEF85 |
| 427 | 1:0x0012993A (linear 0x0013993A) | 0x001BEF8E |
| 429 | 1:0x0012993C (linear 0x0013993C) | 0x001BEF90 |
| 430 | 1:0x00129942 (linear 0x00139942) | 0x001BEF96 |
| 433 | 1:0x0012994E (linear 0x0013994E) | 0x001BEFA2 |
| 435 | 1:0x00129966 (linear 0x00139966) | 0x001BEFBA |
| 436 | 1:0x0012996F (linear 0x0013996F) | 0x001BEFC3 |
| 438 | 1:0x00129979 (linear 0x00139979) | 0x001BEFCD |
| 439 | 1:0x00129982 (linear 0x00139982) | 0x001BEFD6 |
| 440 | 1:0x00129988 (linear 0x00139988) | 0x001BEFDC |
| 443 | 1:0x0012998F (linear 0x0013998F) | 0x001BEFE3 |
| 445 | 1:0x001299A7 (linear 0x001399A7) | 0x001BEFFB |
| 446 | 1:0x001299B0 (linear 0x001399B0) | 0x001BF004 |
| 448 | 1:0x001299BA (linear 0x001399BA) | 0x001BF00E |
| 449 | 1:0x001299C3 (linear 0x001399C3) | 0x001BF017 |
| 450 | 1:0x001299C9 (linear 0x001399C9) | 0x001BF01D |
| 453 | 1:0x001299D0 (linear 0x001399D0) | 0x001BF024 |
| 455 | 1:0x001299E8 (linear 0x001399E8) | 0x001BF03C |
| 456 | 1:0x001299F3 (linear 0x001399F3) | 0x001BF047 |

