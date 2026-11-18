# F:\WLIBS\smack\smckplay.c

Module 174 of the debug info of ORION2.EXE.

Ranges: 1:0x0012259F (linear 0x0013259F), 1,285 bytes, 2:0x0000C5F0 (linear 0x0017C5F0), 8 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x0012259F (linear 0x0013259F) | code | RADMALLOC | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x00122614 (linear 0x00132614) | code | RADFREE | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x00122646 (linear 0x00132646) | code | Open_Smacker_Animation_ | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x0012273E (linear 0x0013273E) | code | Set_Smacker_Animation_Frame_ | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x00122762 (linear 0x00132762) | code | Get_Smacker_Animation_Height_ | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x001227A3 (linear 0x001327A3) | code | Get_Smacker_Animation_Width_ | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x001227E4 (linear 0x001327E4) | code | Get_Smacker_Animation_Total_Frames_ | `regions/07-object-1/001B7654-001B7E53.txt` |
| 1:0x00122825 (linear 0x00132825) | code | Get_Smacker_Animation_Frame_ | `regions/07-object-1/001B7E54-001B8653.txt` |
| 1:0x00122869 (linear 0x00132869) | code | Draw_Smacker_Animation_ | `regions/07-object-1/001B7E54-001B8653.txt` |
| 1:0x001229D4 (linear 0x001329D4) | code | Close_Smacker_Animation_ | `regions/07-object-1/001B7E54-001B8653.txt` |
| 1:0x00122A11 (linear 0x00132A11) | code | Set_Smacker_Palette_ | `regions/07-object-1/001B7E54-001B8653.txt` |
| 2:0x0000C5F0 (linear 0x0017C5F0) | data | _smk | `regions/08-object-2/001FA654-001FAE53.txt` |
| 2:0x0000C5F4 (linear 0x0017C5F4) | data | _lastkey | `regions/08-object-2/001FA654-001FAE53.txt` |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x0000C5F0 (linear 0x0017C5F0) | smk | Smack * |
| 2:0x0000C5F4 (linear 0x0017C5F4) | lastkey | unsigned int |

#### radmalloc

- Address: 1:0x0012259F (linear 0x0013259F), 117 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: void * (unsigned long); returns in EAX
- Prologue 21 bytes, epilogue 8 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| numbytes | [EBP+0x10] | unsigned long |
| temp | [EBP-0xC] | unsigned char * |
| i | [EBP-0x4] | unsigned char |

#### radfree

- Address: 1:0x00122614 (linear 0x00132614), 50 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: void (void *)
- Prologue 21 bytes, epilogue 6 bytes, return address at frame offset 0xC

| Local | Location | Type |
|---|---|---|
| ptr | [EBP+0x10] | void * |

#### Open_Smacker_Animation

- Address: 1:0x00122646 (linear 0x00132646), 248 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: char (char *, char, unsigned int); returns in AL
- Prologue 22 bytes, epilogue 7 bytes, return address at frame offset 0x10
- Parameter locations: EAX, EDX, EBX

| Local | Location | Type |
|---|---|---|
| name | [EBP-0x14] | char * |
| sound_flag | [EBP-0x8] | char |
| user_flags | [EBP-0x10] | unsigned int |
| flags | [EBP-0xC] | int |
| filename | [EBP-0x78] | char[100] |

#### Set_Smacker_Animation_Frame

- Address: 1:0x0012273E (linear 0x0013273E), 36 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: void (int)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18
- Parameter locations: EAX

| Local | Location | Type |
|---|---|---|
| frame_num | [EBP-0x4] | int |

#### Get_Smacker_Animation_Height

- Address: 1:0x00122762 (linear 0x00132762), 65 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Get_Smacker_Animation_Width

- Address: 1:0x001227A3 (linear 0x001327A3), 65 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Get_Smacker_Animation_Total_Frames

- Address: 1:0x001227E4 (linear 0x001327E4), 65 bytes, in `regions/07-object-1/001B7654-001B7E53.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Get_Smacker_Animation_Frame

- Address: 1:0x00122825 (linear 0x00132825), 68 bytes, in `regions/07-object-1/001B7E54-001B8653.txt`
- Type: int (void); returns in EAX
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

No local variables.

#### Draw_Smacker_Animation

- Address: 1:0x00122869 (linear 0x00132869), 363 bytes, in `regions/07-object-1/001B7E54-001B8653.txt`
- Type: void (int, int)
- Prologue 23 bytes, epilogue 8 bytes, return address at frame offset 0x14
- Parameter locations: EAX, EDX

| Local | Location | Type |
|---|---|---|
| x | [EBP-0x8] | int |
| y | [EBP-0x4] | int |

#### Close_Smacker_Animation

- Address: 1:0x001229D4 (linear 0x001329D4), 61 bytes, in `regions/07-object-1/001B7E54-001B8653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Set_Smacker_Palette

- Address: 1:0x00122A11 (linear 0x00132A11), 147 bytes, in `regions/07-object-1/001B7E54-001B8653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x8] | int |
| c | [EBP-0x4] | unsigned char * |

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
| 19 | scalar | void | void 1 bytes |
| 20 | procedure |  | near386; returns void (void) |
| 21 | pointer | SmackTimerSetupType | near386; of void (void) (#20) |
| 22 | typedef | SmackTimerSetupType | name for void (void) * (#21) |
| 23 | scalar | unsigned long | unsigned 4 bytes |
| 24 | procedure |  | near386; returns unsigned long (void) |
| 25 | pointer | SmackTimerReadType | near386; of unsigned long (void) (#24) |
| 26 | typedef | SmackTimerReadType | name for unsigned long (void) * (#25) |
| 27 | pointer | SmackTimerDoneType | near386; of void (void) (#20) |
| 28 | typedef | SmackTimerDoneType | name for void (void) * (#27) |
| 29 | typedef | time_t | name for unsigned long (#23) |
| 30 | typedef | clock_t | name for unsigned long (#23) |
| 31 | typedef | time_t | name for unsigned long (#23) |
| 32 | typedef | UBYTE | name for unsigned char (#5) |
| 33 | typedef | UWORD | name for unsigned short (#7) |
| 34 | typedef | ULONG | name for unsigned long (#23) |
| 35 | typedef | BYTE | name for char (#10) |
| 36 | typedef | WORD | name for short (#6) |
| 37 | typedef | LONG | name for long (#16) |
| 38 | pointer | CALLBACK | near386; of void (#19) |
| 39 | typedef | CALLBACK | name for void * (#38) |
| 40 | typedef | REALFAR | name for ULONG (#34) |
| 41 | typedef | HTIMER | name for LONG (#37) |
| 42 | array |  | 0..6; of unsigned long (#23) |
| 43 | array |  | 0..6; of unsigned long (#23) |
| 44 | array |  | 0..771; of unsigned char (#5) |
| 45 | struct | struct SmackTag | 25 fields, 916 bytes |
| 46 | typedef | SmackTag | struct name for struct #45 (#45) |
| 47 | typedef | Smack | name for struct SmackTag (#46) |
| 48 | pointer |  | near386; of Smack (#47) |
| 49 | procedure |  | near386; returns void * (unsigned long) |
| 50 | pointer |  | near386; of unsigned char (#5) |
| 51 | procedure |  | near386; returns void (void *) |
| 52 | procedure |  | near386; returns char (char *, char, unsigned int) |
| 53 | array |  | 0..99; of char (#10) |
| 54 | procedure |  | near386; returns void (int) |
| 55 | procedure |  | near386; returns int (void) |
| 56 | procedure |  | near386; returns void (int, int) |
| 57 | procedure |  | near386; returns void (void) |

##### struct SmackTag (#45, 25 fields, 916 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| Version | offset 0 | unsigned long |
| Width | offset 4 | unsigned long |
| Height | offset 8 | unsigned long |
| Frames | offset 12 | unsigned long |
| MSPerFrame | offset 16 | unsigned long |
| SmackerType | offset 20 | unsigned long |
| LargestInTrack | offset 24 | unsigned long[7] |
| tablesize | offset 52 | unsigned long |
| codesize | offset 56 | unsigned long |
| absize | offset 60 | unsigned long |
| detailsize | offset 64 | unsigned long |
| typesize | offset 68 | unsigned long |
| TrackType | offset 72 | unsigned long[7] |
| extra | offset 100 | unsigned long |
| NewPalette | offset 104 | unsigned long |
| Palette | offset 108 | unsigned char[772] |
| FrameNum | offset 880 | unsigned long |
| LastRectx | offset 884 | unsigned long |
| LastRecty | offset 888 | unsigned long |
| LastRectw | offset 892 | unsigned long |
| LastRecth | offset 896 | unsigned long |
| OpenFlags | offset 900 | unsigned long |
| LeftOfs | offset 904 | unsigned long |
| TopOfs | offset 908 | unsigned long |
| addr32 | offset 912 | unsigned long |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 32769 | 1:0x0012259F (linear 0x0013259F) | 0x001B7BF3 |
| 32774 | 1:0x001225B4 (linear 0x001325B4) | 0x001B7C08 |
| 32775 | 1:0x001225BA (linear 0x001325BA) | 0x001B7C0E |
| 32776 | 1:0x001225C3 (linear 0x001325C3) | 0x001B7C17 |
| 32778 | 1:0x001225E6 (linear 0x001325E6) | 0x001B7C3A |
| 32779 | 1:0x001225F2 (linear 0x001325F2) | 0x001B7C46 |
| 32780 | 1:0x001225FA (linear 0x001325FA) | 0x001B7C4E |
| 32782 | 1:0x00122603 (linear 0x00132603) | 0x001B7C57 |
| 32783 | 1:0x00122609 (linear 0x00132609) | 0x001B7C5D |
| 32785 | 1:0x00122614 (linear 0x00132614) | 0x001B7C68 |
| 32787 | 1:0x00122629 (linear 0x00132629) | 0x001B7C7D |
| 32788 | 1:0x00122640 (linear 0x00132640) | 0x001B7C94 |
| 22 | 1:0x00122646 (linear 0x00132646) | 0x001B7C9A |
| 27 | 1:0x00122665 (linear 0x00132665) | 0x001B7CB9 |
| 28 | 1:0x00122670 (linear 0x00132670) | 0x001B7CC4 |
| 30 | 1:0x0012267D (linear 0x0013267D) | 0x001B7CD1 |
| 32 | 1:0x00122687 (linear 0x00132687) | 0x001B7CDB |
| 34 | 1:0x0012268E (linear 0x0013268E) | 0x001B7CE2 |
| 36 | 1:0x00122690 (linear 0x00132690) | 0x001B7CE4 |
| 39 | 1:0x00122697 (linear 0x00132697) | 0x001B7CEB |
| 41 | 1:0x0012269D (linear 0x0013269D) | 0x001B7CF1 |
| 42 | 1:0x001226B1 (linear 0x001326B1) | 0x001B7D05 |
| 44 | 1:0x001226BA (linear 0x001326BA) | 0x001B7D0E |
| 46 | 1:0x001226C3 (linear 0x001326C3) | 0x001B7D17 |
| 50 | 1:0x001226C9 (linear 0x001326C9) | 0x001B7D1D |
| 51 | 1:0x001226D6 (linear 0x001326D6) | 0x001B7D2A |
| 52 | 1:0x001226E1 (linear 0x001326E1) | 0x001B7D35 |
| 56 | 1:0x001226F5 (linear 0x001326F5) | 0x001B7D49 |
| 58 | 1:0x0012270A (linear 0x0013270A) | 0x001B7D5E |
| 62 | 1:0x00122721 (linear 0x00132721) | 0x001B7D75 |
| 64 | 1:0x0012272A (linear 0x0013272A) | 0x001B7D7E |
| 66 | 1:0x00122730 (linear 0x00132730) | 0x001B7D84 |
| 67 | 1:0x00122734 (linear 0x00132734) | 0x001B7D88 |
| 68 | 1:0x0012273E (linear 0x0013273E) | 0x001B7D92 |
| 71 | 1:0x00122759 (linear 0x00132759) | 0x001B7DAD |
| 72 | 1:0x00122762 (linear 0x00132762) | 0x001B7DB6 |
| 74 | 1:0x0012277A (linear 0x0013277A) | 0x001B7DCE |
| 75 | 1:0x00122783 (linear 0x00132783) | 0x001B7DD7 |
| 76 | 1:0x0012278C (linear 0x0013278C) | 0x001B7DE0 |
| 77 | 1:0x00122797 (linear 0x00132797) | 0x001B7DEB |
| 78 | 1:0x001227A3 (linear 0x001327A3) | 0x001B7DF7 |
| 80 | 1:0x001227BB (linear 0x001327BB) | 0x001B7E0F |
| 81 | 1:0x001227C4 (linear 0x001327C4) | 0x001B7E18 |
| 82 | 1:0x001227CD (linear 0x001327CD) | 0x001B7E21 |
| 83 | 1:0x001227D8 (linear 0x001327D8) | 0x001B7E2C |
| 84 | 1:0x001227E4 (linear 0x001327E4) | 0x001B7E38 |
| 86 | 1:0x001227FC (linear 0x001327FC) | 0x001B7E50 |
| 87 | 1:0x00122805 (linear 0x00132805) | 0x001B7E59 |
| 89 | 1:0x0012280E (linear 0x0013280E) | 0x001B7E62 |
| 91 | 1:0x00122819 (linear 0x00132819) | 0x001B7E6D |
| 92 | 1:0x00122825 (linear 0x00132825) | 0x001B7E79 |
| 94 | 1:0x0012283D (linear 0x0013283D) | 0x001B7E91 |
| 95 | 1:0x00122846 (linear 0x00132846) | 0x001B7E9A |
| 97 | 1:0x0012284F (linear 0x0013284F) | 0x001B7EA3 |
| 98 | 1:0x0012285D (linear 0x0013285D) | 0x001B7EB1 |
| 99 | 1:0x00122869 (linear 0x00132869) | 0x001B7EBD |
| 102 | 1:0x00122886 (linear 0x00132886) | 0x001B7EDA |
| 105 | 1:0x00122893 (linear 0x00132893) | 0x001B7EE7 |
| 108 | 1:0x001228A2 (linear 0x001328A2) | 0x001B7EF6 |
| 110 | 1:0x001228AD (linear 0x001328AD) | 0x001B7F01 |
| 115 | 1:0x001228B2 (linear 0x001328B2) | 0x001B7F06 |
| 116 | 1:0x001228DA (linear 0x001328DA) | 0x001B7F2E |
| 119 | 1:0x001228E5 (linear 0x001328E5) | 0x001B7F39 |
| 121 | 1:0x001228FA (linear 0x001328FA) | 0x001B7F4E |
| 122 | 1:0x00122947 (linear 0x00132947) | 0x001B7F9B |
| 124 | 1:0x00122995 (linear 0x00132995) | 0x001B7FE9 |
| 126 | 1:0x001229A6 (linear 0x001329A6) | 0x001B7FFA |
| 127 | 1:0x001229AB (linear 0x001329AB) | 0x001B7FFF |
| 129 | 1:0x001229C1 (linear 0x001329C1) | 0x001B8015 |
| 131 | 1:0x001229CC (linear 0x001329CC) | 0x001B8020 |
| 132 | 1:0x001229D4 (linear 0x001329D4) | 0x001B8028 |
| 134 | 1:0x001229EC (linear 0x001329EC) | 0x001B8040 |
| 136 | 1:0x001229F5 (linear 0x001329F5) | 0x001B8049 |
| 137 | 1:0x00122A00 (linear 0x00132A00) | 0x001B8054 |
| 138 | 1:0x00122A0A (linear 0x00132A0A) | 0x001B805E |
| 140 | 1:0x00122A11 (linear 0x00132A11) | 0x001B8065 |
| 145 | 1:0x00122A29 (linear 0x00132A29) | 0x001B807D |
| 146 | 1:0x00122A36 (linear 0x00132A36) | 0x001B808A |
| 148 | 1:0x00122A50 (linear 0x00132A50) | 0x001B80A4 |
| 149 | 1:0x00122A5D (linear 0x00132A5D) | 0x001B80B1 |
| 150 | 1:0x00122A71 (linear 0x00132A71) | 0x001B80C5 |
| 151 | 1:0x00122A85 (linear 0x00132A85) | 0x001B80D9 |
| 156 | 1:0x00122A99 (linear 0x00132A99) | 0x001B80ED |
| 159 | 1:0x00122A9B (linear 0x00132A9B) | 0x001B80EF |

