# F:\WLIBS\net\comcore.c

Module 274 of the debug info of ORION2.EXE.

Ranges: 1:0x0013DDF3 (linear 0x0014DDF3), 2,749 bytes, 2:0x00011174 (linear 0x00181174), 1,412 bytes, 2:0x0004BC40 (linear 0x001BBC40), 88 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0013DDF3 (linear 0x0014DDF3) | code | ComInit_ | `regions/07-object-1/001D2E54-001D3653.txt` |
| 1:0x0013DF7A (linear 0x0014DF7A) | code | ComEnd_ | `regions/07-object-1/001D2E54-001D3653.txt` |
| 1:0x0013E02A (linear 0x0014E02A) | code | ComDoComms_ | `regions/07-object-1/001D3654-001D3E53.txt` |
| 1:0x0013E069 (linear 0x0014E069) | code | ComSendUserMessage_ | `regions/07-object-1/001D3654-001D3E53.txt` |
| 1:0x0013E1F2 (linear 0x0014E1F2) | code | GetMessages_ | `regions/07-object-1/001D3654-001D3E53.txt` |
| 1:0x0013E30F (linear 0x0014E30F) | code | HandleUserMessage_ | `regions/07-object-1/001D3654-001D3E53.txt` |
| 1:0x0013E429 (linear 0x0014E429) | code | ComReleaseComms_ | `regions/07-object-1/001D3654-001D3E53.txt` |
| 1:0x0013E46B (linear 0x0014E46B) | code | SendKeepAlive_ | `regions/07-object-1/001D3654-001D3E53.txt` |
| 1:0x0013E579 (linear 0x0014E579) | code | HandleKeepAlive_ | `regions/07-object-1/001D3654-001D3E53.txt` |
| 1:0x0013E6C8 (linear 0x0014E6C8) | code | CheckKeepAlive_ | `regions/07-object-1/001D3654-001D3E53.txt` |
| 1:0x0013E75A (linear 0x0014E75A) | code | SendPDU_ | `regions/07-object-1/001D3654-001D3E53.txt` |
| 1:0x0013E7F3 (linear 0x0014E7F3) | code | hostInDescStation_ | `regions/07-object-1/001D3654-001D3E53.txt` |
| 1:0x0013E853 (linear 0x0014E853) | code | setHostLastKeepAlive_ | `regions/07-object-1/001D3E54-001D4653.txt` |
| 2:0x00011174 (linear 0x00181174) | data | _checkAliveTime | `regions/08-object-2/001FF654-001FFE53.txt` |
| 2:0x00011178 (linear 0x00181178) | data | _netStations | `regions/08-object-2/001FF654-001FFE53.txt` |
| 2:0x0004BC40 (linear 0x001BBC40) | data | _inqReq | - |
| 2:0x0004BC48 (linear 0x001BBC48) | data | _sendReq | - |
| 2:0x0004BC58 (linear 0x001BBC58) | data | _pollReq | - |
| 2:0x0004BC68 (linear 0x001BBC68) | data | _recFunc | - |
| 2:0x0004BC6C (linear 0x001BBC6C) | data | _keepAliveTime | - |
| 2:0x0004BC70 (linear 0x001BBC70) | data | _userReceiveBuffer | - |
| 2:0x0004BC74 (linear 0x001BBC74) | data | _cBuffLength | - |
| 2:0x0004BC78 (linear 0x001BBC78) | data | _pollBuffer | - |
| 2:0x0004BC7C (linear 0x001BBC7C) | data | _commsInitTime | - |
| 2:0x0004BC80 (linear 0x001BBC80) | data | _sendBuffer | - |
| 2:0x0004BC84 (linear 0x001BBC84) | data | _openReq | - |
| 2:0x0004BC96 (linear 0x001BBC96) | data | _recBufferLength | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x0004BC40 (linear 0x001BBC40) | inqReq | NetInqRequest |
| 2:0x0004BC48 (linear 0x001BBC48) | sendReq | NetSendRequest |
| 2:0x0004BC58 (linear 0x001BBC58) | pollReq | NetPollRequest |
| 2:0x00011178 (linear 0x00181178) | netStations | NetStationDesc[64] |
| 2:0x0004BC68 (linear 0x001BBC68) | recFunc | void (ushort, ushort, uchar *) * |
| 2:0x0004BC6C (linear 0x001BBC6C) | keepAliveTime | ulong |
| 2:0x0004BC70 (linear 0x001BBC70) | userReceiveBuffer | uchar * |
| 2:0x0004BC74 (linear 0x001BBC74) | cBuffLength | uint |
| 2:0x0004BC78 (linear 0x001BBC78) | pollBuffer | uchar * |
| 2:0x0004BC7C (linear 0x001BBC7C) | commsInitTime | ulong |
| 2:0x00011174 (linear 0x00181174) | checkAliveTime | ulong |
| 2:0x0004BC80 (linear 0x001BBC80) | sendBuffer | uchar * |
| 2:0x0004BC84 (linear 0x001BBC84) | openReq | NetOpenRequest |
| 2:0x0004BC96 (linear 0x001BBC96) | recBufferLength | ushort |

#### ComInit

- Address: 1:0x0013DDF3 (linear 0x0014DDF3), 391 bytes, in `regions/07-object-1/001D2E54-001D3653.txt`
- Type: short (ushort, uchar *, char, void (ushort, ushort, ushort, uchar *) *); returns in AX
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| maxMessageLength | [EBP-0xC] | ushort |
| gameID | [EBP-0x14] | uchar * |
| mode | [EBP-0x4] | char |
| receiveFunc | [EBP-0x18] | void (ushort, ushort, ushort, uchar *) * |
| status | [EBP-0x10] | int |

#### ComEnd

- Address: 1:0x0013DF7A (linear 0x0014DF7A), 176 bytes, in `regions/07-object-1/001D2E54-001D3653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### ComDoComms

- Address: 1:0x0013E02A (linear 0x0014E02A), 63 bytes, in `regions/07-object-1/001D3654-001D3E53.txt`
- Type: short (void); returns in AX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### ComSendUserMessage

- Address: 1:0x0013E069 (linear 0x0014E069), 393 bytes, in `regions/07-object-1/001D3654-001D3E53.txt`
- Type: short (uchar *, ushort, ushort, ushort, uchar, ushort); returns in AX
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| messageData | [EBP-0x24] | uchar * |
| messageLength | [EBP-0x4] | ushort |
| whoTo | [EBP-0x10] | ushort |
| userMessageNum | [EBP-0xC] | ushort |
| messagePriority | [EBP+0x10] | uchar |
| userType | [EBP+0x14] | ushort |
| pdu | [EBP-0x44] | PDUUserType |
| source | [EBP-0x20] | char * |
| dest | [EBP-0x1C] | char * |
| dx | [EBP-0x14] | ushort |
| netStatus | [EBP-0x18] | int |

#### GetMessages

- Address: 1:0x0013E1F2 (linear 0x0014E1F2), 285 bytes, in `regions/07-object-1/001D3654-001D3E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| pduhdr | [EBP-0x10] | PDUHeaderType * |
| loopcount | [EBP-0x4] | ushort |
| count | [EBP-0x8] | int |
| start | [EBP-0xC] | int |

#### HandleUserMessage

- Address: 1:0x0013E30F (linear 0x0014E30F), 282 bytes, in `regions/07-object-1/001D3654-001D3E53.txt`
- Type: uchar (uchar *); returns in AL
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| messageBuffer | [EBP-0x14] | uchar * |
| source | [EBP-0x10] | uchar * |
| sx | [EBP-0x8] | ushort |
| pdu | [EBP-0xC] | PDUUserType * |

#### ComReleaseComms

- Address: 1:0x0013E429 (linear 0x0014E429), 66 bytes, in `regions/07-object-1/001D3654-001D3E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### SendKeepAlive

- Address: 1:0x0013E46B (linear 0x0014E46B), 270 bytes, in `regions/07-object-1/001D3654-001D3E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| netstatus | [EBP-0x8] | int |
| pduptr | [EBP-0x4] | PDUKeepAliveType * |

#### HandleKeepAlive

- Address: 1:0x0013E579 (linear 0x0014E579), 335 bytes, in `regions/07-object-1/001D3654-001D3E53.txt`
- Type: void (PDUKeepAliveType *)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| pduptr | [EBP-0x8] | PDUKeepAliveType * |
| st | [EBP-0x4] | DosStationDesc * |

#### CheckKeepAlive

- Address: 1:0x0013E6C8 (linear 0x0014E6C8), 146 bytes, in `regions/07-object-1/001D3654-001D3E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| st | [EBP-0x4] | DosStationDesc * |

#### SendPDU

- Address: 1:0x0013E75A (linear 0x0014E75A), 153 bytes, in `regions/07-object-1/001D3654-001D3E53.txt`
- Type: void (char *, ushort, ushort, ushort, int, int)
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC
- Parameter locations: EAX, EDX, EBX, ECX

| Local | Location | Type |
|---|---|---|
| buffer | [EBP-0x14] | char * |
| PDUType | [EBP-0x4] | ushort |
| compression | [EBP-0xC] | ushort |
| size | [EBP-0x8] | ushort |
| destination | [EBP+0x10] | int |
| priority | [EBP+0x14] | int |
| pduptr | [EBP-0x10] | PDUHeaderType * |

#### hostInDescStation

- Address: 1:0x0013E7F3 (linear 0x0014E7F3), 96 bytes, in `regions/07-object-1/001D3654-001D3E53.txt`
- Type: DosStationDesc * (ushort); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| hostID | [EBP-0x4] | ushort |
| st | [EBP-0xC] | DosStationDesc * |

#### setHostLastKeepAlive

- Address: 1:0x0013E853 (linear 0x0014E853), 93 bytes, in `regions/07-object-1/001D3E54-001D4653.txt`
- Type: void (ushort)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| hostID | [EBP-0x4] | ushort |
| st | [EBP-0x8] | DosStationDesc * |

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
| 27 | typedef | clock_t | name for unsigned long (#25) |
| 28 | typedef | time_t | name for unsigned long (#25) |
| 29 | typedef | ushort | name for unsigned short (#7) |
| 30 | typedef | ulong | name for unsigned long (#25) |
| 31 | typedef | uint | name for unsigned int (#9) |
| 32 | typedef | uchar | name for unsigned char (#5) |
| 33 | typedef | boolean | name for int (#8) |
| 34 | scalar | float | float 4 bytes |
| 35 | typedef | bigScalar | name for float (#34) |
| 36 | typedef | smallScalar | name for float (#34) |
| 37 | typedef | uchar | name for unsigned char (#5) |
| 38 | array |  | 0..9; of uchar (#32) |
| 39 | struct | NetStationDesc | 6 fields, 22 bytes |
| 40 | typedef | NetStationDesc | name for struct #39 (#39) |
| 41 | pointer |  | near386; of NetStationDesc (#40) |
| 42 | struct | NetInqRequest | 2 fields, 8 bytes |
| 43 | typedef | NetInqRequest | name for struct #42 (#42) |
| 44 | struct | NetSendRequest | 4 fields, 16 bytes |
| 45 | typedef | NetSendRequest | name for struct #44 (#44) |
| 46 | struct | NetPollRequest | 4 fields, 16 bytes |
| 47 | typedef | NetPollRequest | name for struct #46 (#46) |
| 48 | array |  | 0..63; of NetStationDesc (#40) |
| 49 | pointer |  | near386; of uchar (#32) |
| 50 | procedure |  | near386; returns void (ushort, ushort, uchar *) |
| 51 | pointer |  | near386; of void (ushort, ushort, uchar *) (#50) |
| 52 | array |  | 0..9; of char (#10) |
| 53 | struct | NetOpenRequest | 3 fields, 18 bytes |
| 54 | typedef | NetOpenRequest | name for struct #53 (#53) |
| 55 | procedure |  | near386; returns void (ushort, ushort, ushort, uchar *) |
| 56 | pointer |  | near386; of void (ushort, ushort, ushort, uchar *) (#55) |
| 57 | procedure |  | near386; returns short (ushort, uchar *, char, void (ushort, ushort, ushort, uchar *) *) |
| 58 | procedure |  | near386; returns void (void) |
| 59 | procedure |  | near386; returns short (void) |
| 60 | procedure |  | near386; returns short (uchar *, ushort, ushort, ushort, uchar, ushort) |
| 61 | array |  | 0..9; of uchar (#32) |
| 62 | struct | struct pduheader | 8 fields, 28 bytes |
| 63 | typedef | pduheader | struct name for struct #62 (#62) |
| 64 | typedef | PDUHeaderType | name for struct pduheader (#63) |
| 65 | struct | struct userpdu | 2 fields, 30 bytes |
| 66 | typedef | userpdu | struct name for struct #65 (#65) |
| 67 | typedef | PDUUserType | name for struct userpdu (#66) |
| 68 | pointer |  | near386; of PDUHeaderType (#64) |
| 69 | procedure |  | near386; returns uchar (uchar *) |
| 70 | pointer |  | near386; of PDUUserType (#67) |
| 71 | struct | struct keepalivepdu | 3 fields, 34 bytes |
| 72 | typedef | keepalivepdu | struct name for struct #71 (#71) |
| 73 | typedef | PDUKeepAliveType | name for struct keepalivepdu (#72) |
| 74 | pointer |  | near386; of PDUKeepAliveType (#73) |
| 75 | procedure |  | near386; returns void (PDUKeepAliveType *) |
| 76 | typedef | dosdesc | struct name for struct #78 (#78) |
| 77 | pointer |  | near386; of struct dosdesc (#76) |
| 78 | struct | struct dosdesc | 4 fields, 34 bytes |
| 79 | typedef | DosStationDesc | name for struct dosdesc (#76) |
| 80 | pointer |  | near386; of DosStationDesc (#79) |
| 81 | procedure |  | near386; returns void (char *, ushort, ushort, ushort, int, int) |
| 82 | procedure |  | near386; returns DosStationDesc * (ushort) |
| 83 | procedure |  | near386; returns void (ushort) |

##### NetStationDesc (#39, 6 fields, 22 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| stationId | offset 0 | uchar |
| open | offset 1 | uchar |
| gameId | offset 2 | uchar[10] |
| network | offset 12 | int |
| network_node1 | offset 16 | int |
| network_node2 | offset 20 | short |

##### NetInqRequest (#42, 2 fields, 8 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| stationCount | offset 0 | int |
| stationTablePtr | offset 4 | NetStationDesc * |

##### NetSendRequest (#44, 4 fields, 16 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| messageDestination | offset 0 | int |
| messageDataBuffer | offset 4 | char * |
| messageDataCount | offset 8 | int |
| messagePriority | offset 12 | int |

##### NetPollRequest (#46, 4 fields, 16 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| messageOriginator | offset 0 | int |
| messageDataBuffer | offset 4 | char * |
| messageDataCount | offset 8 | int |
| messageCount | offset 12 | int |

##### NetOpenRequest (#53, 3 fields, 18 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| stationId | offset 0 | int |
| gameId | offset 4 | char[10] |
| maxMessageSize | offset 14 | int |

##### struct pduheader (#62, 8 fields, 28 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| PDUType | offset 0 | ushort |
| PDULength | offset 2 | ushort |
| sourceHost | offset 4 | ushort |
| destinationType | offset 6 | ushort |
| gameName | offset 8 | uchar[10] |
| network | offset 18 | int |
| network_node1 | offset 22 | int |
| network_node2 | offset 26 | short |

##### struct userpdu (#65, 2 fields, 30 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| hdr | offset 0 | PDUHeaderType |
| userEventType | offset 28 | ushort |

##### struct keepalivepdu (#71, 3 fields, 34 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| hdr | offset 0 | PDUHeaderType |
| open | offset 28 | ushort |
| commInitTime | offset 30 | long |

##### struct dosdesc (#78, 4 fields, 34 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| stationdesc | offset 0 | NetStationDesc |
| lastKeepAliveMsg | offset 22 | ulong |
| stationinittime | offset 26 | long |
| next | offset 30 | struct dosdesc * |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 97 | 1:0x0013DDF3 (linear 0x0014DDF3) | 0x001D3447 |
| 106 | 1:0x0013DE14 (linear 0x0014DE14) | 0x001D3468 |
| 107 | 1:0x0013DE22 (linear 0x0014DE22) | 0x001D3476 |
| 109 | 1:0x0013DE2C (linear 0x0014DE2C) | 0x001D3480 |
| 111 | 1:0x0013DE32 (linear 0x0014DE32) | 0x001D3486 |
| 112 | 1:0x0013DE3C (linear 0x0014DE3C) | 0x001D3490 |
| 114 | 1:0x0013DE44 (linear 0x0014DE44) | 0x001D3498 |
| 115 | 1:0x0013DE46 (linear 0x0014DE46) | 0x001D349A |
| 117 | 1:0x0013DE52 (linear 0x0014DE52) | 0x001D34A6 |
| 119 | 1:0x0013DE60 (linear 0x0014DE60) | 0x001D34B4 |
| 120 | 1:0x0013DE72 (linear 0x0014DE72) | 0x001D34C6 |
| 121 | 1:0x0013DE7B (linear 0x0014DE7B) | 0x001D34CF |
| 123 | 1:0x0013DE87 (linear 0x0014DE87) | 0x001D34DB |
| 124 | 1:0x0013DE99 (linear 0x0014DE99) | 0x001D34ED |
| 125 | 1:0x0013DEA2 (linear 0x0014DEA2) | 0x001D34F6 |
| 127 | 1:0x0013DEAE (linear 0x0014DEAE) | 0x001D3502 |
| 128 | 1:0x0013DEB8 (linear 0x0014DEB8) | 0x001D350C |
| 130 | 1:0x0013DEC2 (linear 0x0014DEC2) | 0x001D3516 |
| 131 | 1:0x0013DED4 (linear 0x0014DED4) | 0x001D3528 |
| 132 | 1:0x0013DEDD (linear 0x0014DEDD) | 0x001D3531 |
| 134 | 1:0x0013DEE9 (linear 0x0014DEE9) | 0x001D353D |
| 136 | 1:0x0013DEF3 (linear 0x0014DEF3) | 0x001D3547 |
| 138 | 1:0x0013DEFF (linear 0x0014DEFF) | 0x001D3553 |
| 140 | 1:0x0013DF07 (linear 0x0014DF07) | 0x001D355B |
| 142 | 1:0x0013DF11 (linear 0x0014DF11) | 0x001D3565 |
| 143 | 1:0x0013DF1E (linear 0x0014DF1E) | 0x001D3572 |
| 145 | 1:0x0013DF29 (linear 0x0014DF29) | 0x001D357D |
| 147 | 1:0x0013DF37 (linear 0x0014DF37) | 0x001D358B |
| 148 | 1:0x0013DF3C (linear 0x0014DF3C) | 0x001D3590 |
| 151 | 1:0x0013DF45 (linear 0x0014DF45) | 0x001D3599 |
| 153 | 1:0x0013DF51 (linear 0x0014DF51) | 0x001D35A5 |
| 154 | 1:0x0013DF5B (linear 0x0014DF5B) | 0x001D35AF |
| 155 | 1:0x0013DF65 (linear 0x0014DF65) | 0x001D35B9 |
| 157 | 1:0x0013DF6A (linear 0x0014DF6A) | 0x001D35BE |
| 158 | 1:0x0013DF71 (linear 0x0014DF71) | 0x001D35C5 |
| 161 | 1:0x0013DF7A (linear 0x0014DF7A) | 0x001D35CE |
| 163 | 1:0x0013DF92 (linear 0x0014DF92) | 0x001D35E6 |
| 164 | 1:0x0013DF9C (linear 0x0014DF9C) | 0x001D35F0 |
| 166 | 1:0x0013DFA5 (linear 0x0014DFA5) | 0x001D35F9 |
| 167 | 1:0x0013DFAF (linear 0x0014DFAF) | 0x001D3603 |
| 168 | 1:0x0013DFB9 (linear 0x0014DFB9) | 0x001D360D |
| 170 | 1:0x0013DFC3 (linear 0x0014DFC3) | 0x001D3617 |
| 171 | 1:0x0013DFCD (linear 0x0014DFCD) | 0x001D3621 |
| 172 | 1:0x0013DFD7 (linear 0x0014DFD7) | 0x001D362B |
| 174 | 1:0x0013DFE1 (linear 0x0014DFE1) | 0x001D3635 |
| 175 | 1:0x0013DFEB (linear 0x0014DFEB) | 0x001D363F |
| 177 | 1:0x0013DFF5 (linear 0x0014DFF5) | 0x001D3649 |
| 179 | 1:0x0013DFFF (linear 0x0014DFFF) | 0x001D3653 |
| 180 | 1:0x0013E004 (linear 0x0014E004) | 0x001D3658 |
| 181 | 1:0x0013E00B (linear 0x0014E00B) | 0x001D365F |
| 183 | 1:0x0013E015 (linear 0x0014E015) | 0x001D3669 |
| 185 | 1:0x0013E01A (linear 0x0014E01A) | 0x001D366E |
| 186 | 1:0x0013E023 (linear 0x0014E023) | 0x001D3677 |
| 189 | 1:0x0013E02A (linear 0x0014E02A) | 0x001D367E |
| 191 | 1:0x0013E042 (linear 0x0014E042) | 0x001D3696 |
| 193 | 1:0x0013E047 (linear 0x0014E047) | 0x001D369B |
| 195 | 1:0x0013E04C (linear 0x0014E04C) | 0x001D36A0 |
| 197 | 1:0x0013E051 (linear 0x0014E051) | 0x001D36A5 |
| 199 | 1:0x0013E056 (linear 0x0014E056) | 0x001D36AA |
| 200 | 1:0x0013E05D (linear 0x0014E05D) | 0x001D36B1 |
| 237 | 1:0x0013E069 (linear 0x0014E069) | 0x001D36BD |
| 250 | 1:0x0013E08A (linear 0x0014E08A) | 0x001D36DE |
| 251 | 1:0x0013E095 (linear 0x0014E095) | 0x001D36E9 |
| 253 | 1:0x0013E0A1 (linear 0x0014E0A1) | 0x001D36F5 |
| 254 | 1:0x0013E0B8 (linear 0x0014E0B8) | 0x001D370C |
| 256 | 1:0x0013E0DA (linear 0x0014E0DA) | 0x001D372E |
| 257 | 1:0x0013E0E0 (linear 0x0014E0E0) | 0x001D3734 |
| 258 | 1:0x0013E0E7 (linear 0x0014E0E7) | 0x001D373B |
| 259 | 1:0x0013E0F1 (linear 0x0014E0F1) | 0x001D3745 |
| 260 | 1:0x0013E0F8 (linear 0x0014E0F8) | 0x001D374C |
| 261 | 1:0x0013E0FF (linear 0x0014E0FF) | 0x001D3753 |
| 263 | 1:0x0013E111 (linear 0x0014E111) | 0x001D3765 |
| 264 | 1:0x0013E11C (linear 0x0014E11C) | 0x001D3770 |
| 265 | 1:0x0013E126 (linear 0x0014E126) | 0x001D377A |
| 267 | 1:0x0013E12E (linear 0x0014E12E) | 0x001D3782 |
| 268 | 1:0x0013E135 (linear 0x0014E135) | 0x001D3789 |
| 270 | 1:0x0013E13B (linear 0x0014E13B) | 0x001D378F |
| 271 | 1:0x0013E151 (linear 0x0014E151) | 0x001D37A5 |
| 273 | 1:0x0013E155 (linear 0x0014E155) | 0x001D37A9 |
| 275 | 1:0x0013E18F (linear 0x0014E18F) | 0x001D37E3 |
| 277 | 1:0x0013E195 (linear 0x0014E195) | 0x001D37E9 |
| 278 | 1:0x0013E1AC (linear 0x0014E1AC) | 0x001D3800 |
| 280 | 1:0x0013E1B2 (linear 0x0014E1B2) | 0x001D3806 |
| 282 | 1:0x0013E1BD (linear 0x0014E1BD) | 0x001D3811 |
| 284 | 1:0x0013E1D0 (linear 0x0014E1D0) | 0x001D3824 |
| 285 | 1:0x0013E1D6 (linear 0x0014E1D6) | 0x001D382A |
| 287 | 1:0x0013E1E0 (linear 0x0014E1E0) | 0x001D3834 |
| 288 | 1:0x0013E1E7 (linear 0x0014E1E7) | 0x001D383B |
| 291 | 1:0x0013E1F2 (linear 0x0014E1F2) | 0x001D3846 |
| 294 | 1:0x0013E20A (linear 0x0014E20A) | 0x001D385E |
| 295 | 1:0x0013E211 (linear 0x0014E211) | 0x001D3865 |
| 296 | 1:0x0013E218 (linear 0x0014E218) | 0x001D386C |
| 298 | 1:0x0013E220 (linear 0x0014E220) | 0x001D3874 |
| 300 | 1:0x0013E22E (linear 0x0014E22E) | 0x001D3882 |
| 303 | 1:0x0013E241 (linear 0x0014E241) | 0x001D3895 |
| 306 | 1:0x0013E247 (linear 0x0014E247) | 0x001D389B |
| 309 | 1:0x0013E24E (linear 0x0014E24E) | 0x001D38A2 |
| 311 | 1:0x0013E254 (linear 0x0014E254) | 0x001D38A8 |
| 313 | 1:0x0013E25C (linear 0x0014E25C) | 0x001D38B0 |
| 316 | 1:0x0013E27E (linear 0x0014E27E) | 0x001D38D2 |
| 318 | 1:0x0013E286 (linear 0x0014E286) | 0x001D38DA |
| 321 | 1:0x0013E28B (linear 0x0014E28B) | 0x001D38DF |
| 323 | 1:0x0013E290 (linear 0x0014E290) | 0x001D38E4 |
| 326 | 1:0x0013E295 (linear 0x0014E295) | 0x001D38E9 |
| 329 | 1:0x0013E2A5 (linear 0x0014E2A5) | 0x001D38F9 |
| 331 | 1:0x0013E2AB (linear 0x0014E2AB) | 0x001D38FF |
| 333 | 1:0x0013E2AD (linear 0x0014E2AD) | 0x001D3901 |
| 335 | 1:0x0013E2BE (linear 0x0014E2BE) | 0x001D3912 |
| 336 | 1:0x0013E2E7 (linear 0x0014E2E7) | 0x001D393B |
| 338 | 1:0x0013E2E9 (linear 0x0014E2E9) | 0x001D393D |
| 341 | 1:0x0013E2F9 (linear 0x0014E2F9) | 0x001D394D |
| 346 | 1:0x0013E301 (linear 0x0014E301) | 0x001D3955 |
| 350 | 1:0x0013E306 (linear 0x0014E306) | 0x001D395A |
| 360 | 1:0x0013E30F (linear 0x0014E30F) | 0x001D3963 |
| 364 | 1:0x0013E32A (linear 0x0014E32A) | 0x001D397E |
| 366 | 1:0x0013E330 (linear 0x0014E330) | 0x001D3984 |
| 367 | 1:0x0013E340 (linear 0x0014E340) | 0x001D3994 |
| 369 | 1:0x0013E349 (linear 0x0014E349) | 0x001D399D |
| 372 | 1:0x0013E383 (linear 0x0014E383) | 0x001D39D7 |
| 374 | 1:0x0013E38C (linear 0x0014E38C) | 0x001D39E0 |
| 375 | 1:0x0013E392 (linear 0x0014E392) | 0x001D39E6 |
| 376 | 1:0x0013E399 (linear 0x0014E399) | 0x001D39ED |
| 378 | 1:0x0013E3D3 (linear 0x0014E3D3) | 0x001D3A27 |
| 380 | 1:0x0013E3EF (linear 0x0014E3EF) | 0x001D3A43 |
| 381 | 1:0x0013E3F8 (linear 0x0014E3F8) | 0x001D3A4C |
| 383 | 1:0x0013E419 (linear 0x0014E419) | 0x001D3A6D |
| 384 | 1:0x0013E41D (linear 0x0014E41D) | 0x001D3A71 |
| 387 | 1:0x0013E429 (linear 0x0014E429) | 0x001D3A7D |
| 389 | 1:0x0013E441 (linear 0x0014E441) | 0x001D3A95 |
| 390 | 1:0x0013E44B (linear 0x0014E44B) | 0x001D3A9F |
| 391 | 1:0x0013E455 (linear 0x0014E455) | 0x001D3AA9 |
| 393 | 1:0x0013E45F (linear 0x0014E45F) | 0x001D3AB3 |
| 394 | 1:0x0013E464 (linear 0x0014E464) | 0x001D3AB8 |
| 397 | 1:0x0013E46B (linear 0x0014E46B) | 0x001D3ABF |
| 402 | 1:0x0013E483 (linear 0x0014E483) | 0x001D3AD7 |
| 405 | 1:0x0013E494 (linear 0x0014E494) | 0x001D3AE8 |
| 408 | 1:0x0013E4A1 (linear 0x0014E4A1) | 0x001D3AF5 |
| 409 | 1:0x0013E4A9 (linear 0x0014E4A9) | 0x001D3AFD |
| 410 | 1:0x0013E4BE (linear 0x0014E4BE) | 0x001D3B12 |
| 411 | 1:0x0013E4CE (linear 0x0014E4CE) | 0x001D3B22 |
| 412 | 1:0x0013E4DC (linear 0x0014E4DC) | 0x001D3B30 |
| 413 | 1:0x0013E4E8 (linear 0x0014E4E8) | 0x001D3B3C |
| 414 | 1:0x0013E4F4 (linear 0x0014E4F4) | 0x001D3B48 |
| 416 | 1:0x0013E502 (linear 0x0014E502) | 0x001D3B56 |
| 417 | 1:0x0013E50A (linear 0x0014E50A) | 0x001D3B5E |
| 418 | 1:0x0013E516 (linear 0x0014E516) | 0x001D3B6A |
| 419 | 1:0x0013E51F (linear 0x0014E51F) | 0x001D3B73 |
| 421 | 1:0x0013E528 (linear 0x0014E528) | 0x001D3B7C |
| 422 | 1:0x0013E532 (linear 0x0014E532) | 0x001D3B86 |
| 423 | 1:0x0013E543 (linear 0x0014E543) | 0x001D3B97 |
| 424 | 1:0x0013E54D (linear 0x0014E54D) | 0x001D3BA1 |
| 426 | 1:0x0013E560 (linear 0x0014E560) | 0x001D3BB4 |
| 427 | 1:0x0013E566 (linear 0x0014E566) | 0x001D3BBA |
| 428 | 1:0x0013E570 (linear 0x0014E570) | 0x001D3BC4 |
| 431 | 1:0x0013E579 (linear 0x0014E579) | 0x001D3BCD |
| 435 | 1:0x0013E594 (linear 0x0014E594) | 0x001D3BE8 |
| 437 | 1:0x0013E5A8 (linear 0x0014E5A8) | 0x001D3BFC |
| 439 | 1:0x0013E5BC (linear 0x0014E5BC) | 0x001D3C10 |
| 441 | 1:0x0013E5C2 (linear 0x0014E5C2) | 0x001D3C16 |
| 444 | 1:0x0013E5FE (linear 0x0014E5FE) | 0x001D3C52 |
| 446 | 1:0x0013E600 (linear 0x0014E600) | 0x001D3C54 |
| 447 | 1:0x0013E60D (linear 0x0014E60D) | 0x001D3C61 |
| 448 | 1:0x0013E61E (linear 0x0014E61E) | 0x001D3C72 |
| 450 | 1:0x0013E62A (linear 0x0014E62A) | 0x001D3C7E |
| 451 | 1:0x0013E654 (linear 0x0014E654) | 0x001D3CA8 |
| 454 | 1:0x0013E665 (linear 0x0014E665) | 0x001D3CB9 |
| 456 | 1:0x0013E675 (linear 0x0014E675) | 0x001D3CC9 |
| 457 | 1:0x0013E684 (linear 0x0014E684) | 0x001D3CD8 |
| 458 | 1:0x0013E692 (linear 0x0014E692) | 0x001D3CE6 |
| 459 | 1:0x0013E6A5 (linear 0x0014E6A5) | 0x001D3CF9 |
| 461 | 1:0x0013E6BF (linear 0x0014E6BF) | 0x001D3D13 |
| 464 | 1:0x0013E6C8 (linear 0x0014E6C8) | 0x001D3D1C |
| 466 | 1:0x0013E6E0 (linear 0x0014E6E0) | 0x001D3D34 |
| 468 | 1:0x0013E6E8 (linear 0x0014E6E8) | 0x001D3D3C |
| 470 | 1:0x0013E6FD (linear 0x0014E6FD) | 0x001D3D51 |
| 472 | 1:0x0013E707 (linear 0x0014E707) | 0x001D3D5B |
| 474 | 1:0x0013E70D (linear 0x0014E70D) | 0x001D3D61 |
| 476 | 1:0x0013E720 (linear 0x0014E720) | 0x001D3D74 |
| 477 | 1:0x0013E735 (linear 0x0014E735) | 0x001D3D89 |
| 480 | 1:0x0013E746 (linear 0x0014E746) | 0x001D3D9A |
| 481 | 1:0x0013E74F (linear 0x0014E74F) | 0x001D3DA3 |
| 483 | 1:0x0013E751 (linear 0x0014E751) | 0x001D3DA5 |
| 486 | 1:0x0013E75A (linear 0x0014E75A) | 0x001D3DAE |
| 489 | 1:0x0013E77B (linear 0x0014E77B) | 0x001D3DCF |
| 491 | 1:0x0013E781 (linear 0x0014E781) | 0x001D3DD5 |
| 492 | 1:0x0013E78A (linear 0x0014E78A) | 0x001D3DDE |
| 493 | 1:0x0013E798 (linear 0x0014E798) | 0x001D3DEC |
| 494 | 1:0x0013E7A2 (linear 0x0014E7A2) | 0x001D3DF6 |
| 495 | 1:0x0013E7AC (linear 0x0014E7AC) | 0x001D3E00 |
| 497 | 1:0x0013E7C1 (linear 0x0014E7C1) | 0x001D3E15 |
| 498 | 1:0x0013E7C9 (linear 0x0014E7C9) | 0x001D3E1D |
| 499 | 1:0x0013E7D4 (linear 0x0014E7D4) | 0x001D3E28 |
| 501 | 1:0x0013E7DC (linear 0x0014E7DC) | 0x001D3E30 |
| 502 | 1:0x0013E7EB (linear 0x0014E7EB) | 0x001D3E3F |
| 505 | 1:0x0013E7F3 (linear 0x0014E7F3) | 0x001D3E47 |
| 507 | 1:0x0013E80E (linear 0x0014E80E) | 0x001D3E62 |
| 509 | 1:0x0013E816 (linear 0x0014E816) | 0x001D3E6A |
| 511 | 1:0x0013E81C (linear 0x0014E81C) | 0x001D3E70 |
| 512 | 1:0x0013E82D (linear 0x0014E82D) | 0x001D3E81 |
| 514 | 1:0x0013E835 (linear 0x0014E835) | 0x001D3E89 |
| 515 | 1:0x0013E83E (linear 0x0014E83E) | 0x001D3E92 |
| 517 | 1:0x0013E840 (linear 0x0014E840) | 0x001D3E94 |
| 518 | 1:0x0013E847 (linear 0x0014E847) | 0x001D3E9B |
| 521 | 1:0x0013E853 (linear 0x0014E853) | 0x001D3EA7 |
| 523 | 1:0x0013E86E (linear 0x0014E86E) | 0x001D3EC2 |
| 525 | 1:0x0013E876 (linear 0x0014E876) | 0x001D3ECA |
| 527 | 1:0x0013E87C (linear 0x0014E87C) | 0x001D3ED0 |
| 529 | 1:0x0013E88D (linear 0x0014E88D) | 0x001D3EE1 |
| 530 | 1:0x0013E89A (linear 0x0014E89A) | 0x001D3EEE |
| 533 | 1:0x0013E89C (linear 0x0014E89C) | 0x001D3EF0 |
| 534 | 1:0x0013E8A5 (linear 0x0014E8A5) | 0x001D3EF9 |
| 535 | 1:0x0013E8A7 (linear 0x0014E8A7) | 0x001D3EFB |

