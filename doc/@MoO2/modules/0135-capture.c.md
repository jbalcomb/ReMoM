# F:\WLIBS\flicplay\capture.c

Module 135 of the debug info of ORION2.EXE.

Ranges: 1:0x001001F0 (linear 0x001101F0), 2,457 bytes, 2:0x00002400 (linear 0x00172400), 24 bytes, 2:0x0000C374 (linear 0x0017C374), 4 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x001001F0 (linear 0x001101F0) | code | Screen_Flic_Capture_ | `regions/07-object-1/00195654-00195E53.txt` |
| 1:0x00100AFC (linear 0x00110AFC) | static, code | B_Key_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100B34 (linear 0x00110B34) | code | Release_Version_ | `regions/07-object-1/00195E54-00196653.txt` |
| 1:0x00100B5C (linear 0x00110B5C) | code | Check_Release_Version_ | `regions/07-object-1/00195E54-00196653.txt` |
| 2:0x0000C374 (linear 0x0017C374) | data | _release_flag | `regions/08-object-2/001FA654-001FAE53.txt` |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x0000C374 (linear 0x0017C374) | release_flag | short |

#### Screen_Flic_Capture

- Address: 1:0x001001F0 (linear 0x001101F0), 2,316 bytes, in `regions/07-object-1/00195654-00195E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| ronnie | [EBP-0x34] | FILE * |
| inregs | [EBP-0x60] | union REGS |
| outregs | [EBP-0x7C] | union REGS |
| mode | [EBP-0x2C] | int |
| color | [EBP-0x4] | unsigned char |
| old_bank | [EBP-0x10] | char |
| start_x | [EBP-0x24] | int |
| start_y | [EBP-0x14] | int |
| dumb_number | [EBP-0x1C] | unsigned int |
| offset | [EBP-0x28] | unsigned long |
| i | [EBP-0x30] | long |
| new_bank | [EBP-0x20] | int |
| flag | [EBP-0x8] | char |
| header | [EBP-0x11C] | struct s_pcx_header |
| count | [EBP-0xC] | char |
| palette | [EBP-0x41C] | unsigned char[768] |
| file_name | [EBP-0x9C] | char[30] |
| file_buffer2 | [EBP-0x44] | char[15] |
| file_num | [EBP-0x18] | int |

#### B_Key

- Address: 1:0x00100AFC (linear 0x00110AFC), 56 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Release_Version

- Address: 1:0x00100B34 (linear 0x00110B34), 40 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 7 bytes, return address at frame offset 0x18

No local variables.

#### Check_Release_Version

- Address: 1:0x00100B5C (linear 0x00110B5C), 45 bytes, in `regions/07-object-1/00195E54-00196653.txt`
- Type: short (void); returns in AX
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
| 13 | pointer |  | near386; of char (#10) |
| 14 | array | __va_list | 0..0; of char * (#13) |
| 15 | typedef | __va_list | name for char *[1] (#14) |
| 16 | scalar | long | int 4 bytes |
| 17 | typedef | fpos_t | name for long (#16) |
| 18 | typedef | fpos_t | name for long (#16) |
| 19 | scalar | unsigned long | unsigned 4 bytes |
| 20 | typedef | time_t | name for unsigned long (#19) |
| 21 | typedef | ino_t | name for unsigned int (#9) |
| 22 | typedef | dev_t | name for int (#8) |
| 23 | typedef | off_t | name for long (#16) |
| 24 | typedef | BYTE | name for unsigned char (#5) |
| 25 | typedef | SBYTE | name for signed char (#4) |
| 26 | typedef | UBYTE | name for unsigned char (#5) |
| 27 | typedef | WORD | name for unsigned short (#7) |
| 28 | typedef | SWORD | name for short (#6) |
| 29 | typedef | UWORD | name for unsigned short (#7) |
| 30 | typedef | DWORD | name for unsigned int (#9) |
| 31 | typedef | SDWORD | name for int (#8) |
| 32 | typedef | UDWORD | name for unsigned int (#9) |
| 33 | typedef | BOOL | name for unsigned char (#5) |
| 34 | typedef | SampleHandle | name for int (#8) |
| 35 | scalar | void | void 1 bytes |
| 36 | procedure |  | near386; returns void () |
| 37 | pointer |  | near386; of unsigned char (#5) |
| 38 | struct | struct __iobuf | 8 fields, 26 bytes |
| 39 | typedef | __iobuf | struct name for struct #38 (#38) |
| 40 | typedef | FILE | name for struct __iobuf (#39) |
| 41 | pointer |  | near386; of FILE (#40) |
| 42 | struct | struct DWORDREGS | 7 fields, 28 bytes |
| 43 | typedef | DWORDREGS | struct name for struct #42 (#42) |
| 44 | struct | struct WORDREGS | 13 fields, 28 bytes |
| 45 | typedef | WORDREGS | struct name for struct #44 (#44) |
| 46 | struct | struct BYTEREGS | 12 fields, 16 bytes |
| 47 | typedef | BYTEREGS | struct name for struct #46 (#46) |
| 48 | struct | union REGS | 3 fields, 28 bytes |
| 49 | typedef | REGS | union name for struct #48 (#48) |
| 50 | array |  | 0..47; of char (#10) |
| 51 | array |  | 0..53; of char (#10) |
| 52 | struct | struct s_pcx_header | 18 fields, 128 bytes |
| 53 | typedef | s_pcx_header | struct name for struct #52 (#52) |
| 54 | array |  | 0..767; of unsigned char (#5) |
| 55 | array |  | 0..29; of char (#10) |
| 56 | array |  | 0..14; of char (#10) |
| 57 | procedure |  | near386; returns void (void) |
| 58 | procedure |  | near386; returns short () |

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

##### struct DWORDREGS (#42, 7 fields, 28 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| eax | offset 0 | unsigned int |
| ebx | offset 4 | unsigned int |
| ecx | offset 8 | unsigned int |
| edx | offset 12 | unsigned int |
| esi | offset 16 | unsigned int |
| edi | offset 20 | unsigned int |
| cflag | offset 24 | unsigned int |

##### struct WORDREGS (#44, 13 fields, 28 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| ax | offset 0 | unsigned short |
| _1 | offset 2 | unsigned short |
| bx | offset 4 | unsigned short |
| _2 | offset 6 | unsigned short |
| cx | offset 8 | unsigned short |
| _3 | offset 10 | unsigned short |
| dx | offset 12 | unsigned short |
| _4 | offset 14 | unsigned short |
| si | offset 16 | unsigned short |
| _5 | offset 18 | unsigned short |
| di | offset 20 | unsigned short |
| _6 | offset 22 | unsigned short |
| cflag | offset 24 | unsigned int |

##### struct BYTEREGS (#46, 12 fields, 16 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| al | offset 0 | unsigned char |
| ah | offset 1 | unsigned char |
| _1 | offset 2 | unsigned short |
| bl | offset 4 | unsigned char |
| bh | offset 5 | unsigned char |
| _2 | offset 6 | unsigned short |
| cl | offset 8 | unsigned char |
| ch | offset 9 | unsigned char |
| _3 | offset 10 | unsigned short |
| dl | offset 12 | unsigned char |
| dh | offset 13 | unsigned char |
| _4 | offset 14 | unsigned short |

##### union REGS (#48, 3 fields, 28 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| h | offset 0 | struct BYTEREGS |
| w | offset 0 | struct WORDREGS |
| x | offset 0 | struct DWORDREGS |

##### struct s_pcx_header (#52, 18 fields, 128 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| id | offset 0 | char |
| version | offset 1 | char |
| format | offset 2 | char |
| bitspixelplane | offset 3 | char |
| xmin | offset 4 | short |
| ymin | offset 6 | short |
| xmax | offset 8 | short |
| ymax | offset 10 | short |
| hdpi | offset 12 | short |
| vdpi | offset 14 | short |
| egapalette | offset 16 | char[48] |
| reserved | offset 64 | char |
| numberofplanes | offset 65 | char |
| charslineplane | offset 66 | short |
| paletteinfo | offset 68 | short |
| hscreensize | offset 70 | short |
| vscreensize | offset 72 | short |
| filler | offset 74 | char[54] |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 361 | 1:0x001001F0 (linear 0x001101F0) | 0x00195844 |
| 382 | 1:0x00100208 (linear 0x00110208) | 0x0019585C |
| 384 | 1:0x00100217 (linear 0x00110217) | 0x0019586B |
| 385 | 1:0x0010021E (linear 0x0011021E) | 0x00195872 |
| 386 | 1:0x00100224 (linear 0x00110224) | 0x00195878 |
| 388 | 1:0x00100229 (linear 0x00110229) | 0x0019587D |
| 389 | 1:0x00100230 (linear 0x00110230) | 0x00195884 |
| 390 | 1:0x0010023F (linear 0x0011023F) | 0x00195893 |
| 392 | 1:0x00100258 (linear 0x00110258) | 0x001958AC |
| 394 | 1:0x0010025B (linear 0x0011025B) | 0x001958AF |
| 396 | 1:0x00100261 (linear 0x00110261) | 0x001958B5 |
| 397 | 1:0x0010026E (linear 0x0011026E) | 0x001958C2 |
| 398 | 1:0x0010027E (linear 0x0011027E) | 0x001958D2 |
| 400 | 1:0x0010028C (linear 0x0011028C) | 0x001958E0 |
| 401 | 1:0x0010028E (linear 0x0011028E) | 0x001958E2 |
| 403 | 1:0x001002A1 (linear 0x001102A1) | 0x001958F5 |
| 404 | 1:0x001002AC (linear 0x001102AC) | 0x00195900 |
| 405 | 1:0x001002BD (linear 0x001102BD) | 0x00195911 |
| 406 | 1:0x001002CD (linear 0x001102CD) | 0x00195921 |
| 410 | 1:0x001002D2 (linear 0x001102D2) | 0x00195926 |
| 412 | 1:0x001002E5 (linear 0x001102E5) | 0x00195939 |
| 413 | 1:0x001002F4 (linear 0x001102F4) | 0x00195948 |
| 414 | 1:0x00100303 (linear 0x00110303) | 0x00195957 |
| 415 | 1:0x0010030A (linear 0x0011030A) | 0x0019595E |
| 416 | 1:0x0010030E (linear 0x0011030E) | 0x00195962 |
| 417 | 1:0x0010031E (linear 0x0011031E) | 0x00195972 |
| 418 | 1:0x00100324 (linear 0x00110324) | 0x00195978 |
| 420 | 1:0x00100332 (linear 0x00110332) | 0x00195986 |
| 423 | 1:0x0010033B (linear 0x0011033B) | 0x0019598F |
| 424 | 1:0x00100342 (linear 0x00110342) | 0x00195996 |
| 425 | 1:0x00100349 (linear 0x00110349) | 0x0019599D |
| 426 | 1:0x00100350 (linear 0x00110350) | 0x001959A4 |
| 427 | 1:0x00100357 (linear 0x00110357) | 0x001959AB |
| 428 | 1:0x00100360 (linear 0x00110360) | 0x001959B4 |
| 429 | 1:0x00100369 (linear 0x00110369) | 0x001959BD |
| 430 | 1:0x00100372 (linear 0x00110372) | 0x001959C6 |
| 431 | 1:0x0010037B (linear 0x0011037B) | 0x001959CF |
| 432 | 1:0x00100384 (linear 0x00110384) | 0x001959D8 |
| 433 | 1:0x0010038D (linear 0x0011038D) | 0x001959E1 |
| 434 | 1:0x0010039F (linear 0x0011039F) | 0x001959F3 |
| 435 | 1:0x001003A6 (linear 0x001103A6) | 0x001959FA |
| 436 | 1:0x001003AD (linear 0x001103AD) | 0x00195A01 |
| 437 | 1:0x001003B6 (linear 0x001103B6) | 0x00195A0A |
| 438 | 1:0x001003BF (linear 0x001103BF) | 0x00195A13 |
| 439 | 1:0x001003C8 (linear 0x001103C8) | 0x00195A1C |
| 440 | 1:0x001003D1 (linear 0x001103D1) | 0x00195A25 |
| 441 | 1:0x001003E3 (linear 0x001103E3) | 0x00195A37 |
| 442 | 1:0x001003FB (linear 0x001103FB) | 0x00195A4F |
| 443 | 1:0x00100402 (linear 0x00110402) | 0x00195A56 |
| 445 | 1:0x0010040F (linear 0x0011040F) | 0x00195A63 |
| 449 | 1:0x00100475 (linear 0x00110475) | 0x00195AC9 |
| 451 | 1:0x00100477 (linear 0x00110477) | 0x00195ACB |
| 453 | 1:0x00100481 (linear 0x00110481) | 0x00195AD5 |
| 454 | 1:0x00100485 (linear 0x00110485) | 0x00195AD9 |
| 455 | 1:0x0010049A (linear 0x0011049A) | 0x00195AEE |
| 457 | 1:0x001004AF (linear 0x001104AF) | 0x00195B03 |
| 460 | 1:0x001004B1 (linear 0x001104B1) | 0x00195B05 |
| 462 | 1:0x001004C2 (linear 0x001104C2) | 0x00195B16 |
| 463 | 1:0x001004D7 (linear 0x001104D7) | 0x00195B2B |
| 465 | 1:0x001004DB (linear 0x001104DB) | 0x00195B2F |
| 467 | 1:0x001004DD (linear 0x001104DD) | 0x00195B31 |
| 468 | 1:0x001004E1 (linear 0x001104E1) | 0x00195B35 |
| 469 | 1:0x001004F6 (linear 0x001104F6) | 0x00195B4A |
| 472 | 1:0x0010050B (linear 0x0011050B) | 0x00195B5F |
| 473 | 1:0x00100518 (linear 0x00110518) | 0x00195B6C |
| 474 | 1:0x0010051D (linear 0x0011051D) | 0x00195B71 |
| 475 | 1:0x00100520 (linear 0x00110520) | 0x00195B74 |
| 476 | 1:0x00100529 (linear 0x00110529) | 0x00195B7D |
| 477 | 1:0x00100530 (linear 0x00110530) | 0x00195B84 |
| 479 | 1:0x0010053D (linear 0x0011053D) | 0x00195B91 |
| 483 | 1:0x001005AB (linear 0x001105AB) | 0x00195BFF |
| 485 | 1:0x001005AD (linear 0x001105AD) | 0x00195C01 |
| 487 | 1:0x001005B7 (linear 0x001105B7) | 0x00195C0B |
| 488 | 1:0x001005BB (linear 0x001105BB) | 0x00195C0F |
| 489 | 1:0x001005D0 (linear 0x001105D0) | 0x00195C24 |
| 491 | 1:0x001005E5 (linear 0x001105E5) | 0x00195C39 |
| 494 | 1:0x001005E7 (linear 0x001105E7) | 0x00195C3B |
| 496 | 1:0x001005F8 (linear 0x001105F8) | 0x00195C4C |
| 497 | 1:0x0010060D (linear 0x0011060D) | 0x00195C61 |
| 499 | 1:0x00100611 (linear 0x00110611) | 0x00195C65 |
| 501 | 1:0x00100613 (linear 0x00110613) | 0x00195C67 |
| 502 | 1:0x00100617 (linear 0x00110617) | 0x00195C6B |
| 503 | 1:0x0010062C (linear 0x0011062C) | 0x00195C80 |
| 506 | 1:0x00100641 (linear 0x00110641) | 0x00195C95 |
| 507 | 1:0x0010064E (linear 0x0011064E) | 0x00195CA2 |
| 508 | 1:0x00100653 (linear 0x00110653) | 0x00195CA7 |
| 509 | 1:0x00100656 (linear 0x00110656) | 0x00195CAA |
| 510 | 1:0x0010065F (linear 0x0011065F) | 0x00195CB3 |
| 511 | 1:0x00100666 (linear 0x00110666) | 0x00195CBA |
| 513 | 1:0x00100673 (linear 0x00110673) | 0x00195CC7 |
| 517 | 1:0x001006E1 (linear 0x001106E1) | 0x00195D35 |
| 519 | 1:0x001006E3 (linear 0x001106E3) | 0x00195D37 |
| 521 | 1:0x001006ED (linear 0x001106ED) | 0x00195D41 |
| 522 | 1:0x001006F1 (linear 0x001106F1) | 0x00195D45 |
| 523 | 1:0x00100706 (linear 0x00110706) | 0x00195D5A |
| 525 | 1:0x0010071B (linear 0x0011071B) | 0x00195D6F |
| 529 | 1:0x0010071D (linear 0x0011071D) | 0x00195D71 |
| 531 | 1:0x0010072E (linear 0x0011072E) | 0x00195D82 |
| 532 | 1:0x00100743 (linear 0x00110743) | 0x00195D97 |
| 534 | 1:0x00100747 (linear 0x00110747) | 0x00195D9B |
| 536 | 1:0x00100749 (linear 0x00110749) | 0x00195D9D |
| 537 | 1:0x0010074D (linear 0x0011074D) | 0x00195DA1 |
| 538 | 1:0x00100762 (linear 0x00110762) | 0x00195DB6 |
| 541 | 1:0x00100777 (linear 0x00110777) | 0x00195DCB |
| 542 | 1:0x00100784 (linear 0x00110784) | 0x00195DD8 |
| 543 | 1:0x00100789 (linear 0x00110789) | 0x00195DDD |
| 544 | 1:0x0010078C (linear 0x0011078C) | 0x00195DE0 |
| 546 | 1:0x00100795 (linear 0x00110795) | 0x00195DE9 |
| 547 | 1:0x0010079C (linear 0x0011079C) | 0x00195DF0 |
| 549 | 1:0x001007A9 (linear 0x001107A9) | 0x00195DFD |
| 553 | 1:0x00100817 (linear 0x00110817) | 0x00195E6B |
| 554 | 1:0x00100819 (linear 0x00110819) | 0x00195E6D |
| 556 | 1:0x00100823 (linear 0x00110823) | 0x00195E77 |
| 557 | 1:0x00100827 (linear 0x00110827) | 0x00195E7B |
| 558 | 1:0x0010083C (linear 0x0011083C) | 0x00195E90 |
| 560 | 1:0x00100851 (linear 0x00110851) | 0x00195EA5 |
| 563 | 1:0x00100853 (linear 0x00110853) | 0x00195EA7 |
| 565 | 1:0x00100864 (linear 0x00110864) | 0x00195EB8 |
| 566 | 1:0x00100879 (linear 0x00110879) | 0x00195ECD |
| 568 | 1:0x0010087D (linear 0x0011087D) | 0x00195ED1 |
| 570 | 1:0x0010087F (linear 0x0011087F) | 0x00195ED3 |
| 571 | 1:0x00100883 (linear 0x00110883) | 0x00195ED7 |
| 572 | 1:0x00100898 (linear 0x00110898) | 0x00195EEC |
| 575 | 1:0x001008AD (linear 0x001108AD) | 0x00195F01 |
| 576 | 1:0x001008BA (linear 0x001108BA) | 0x00195F0E |
| 577 | 1:0x001008BF (linear 0x001108BF) | 0x00195F13 |
| 578 | 1:0x001008C2 (linear 0x001108C2) | 0x00195F16 |
| 580 | 1:0x001008CB (linear 0x001108CB) | 0x00195F1F |
| 581 | 1:0x001008D2 (linear 0x001108D2) | 0x00195F26 |
| 583 | 1:0x001008DF (linear 0x001108DF) | 0x00195F33 |
| 587 | 1:0x0010094D (linear 0x0011094D) | 0x00195FA1 |
| 588 | 1:0x0010094F (linear 0x0011094F) | 0x00195FA3 |
| 590 | 1:0x00100959 (linear 0x00110959) | 0x00195FAD |
| 591 | 1:0x0010095D (linear 0x0011095D) | 0x00195FB1 |
| 592 | 1:0x00100972 (linear 0x00110972) | 0x00195FC6 |
| 594 | 1:0x00100987 (linear 0x00110987) | 0x00195FDB |
| 598 | 1:0x00100989 (linear 0x00110989) | 0x00195FDD |
| 600 | 1:0x0010099A (linear 0x0011099A) | 0x00195FEE |
| 601 | 1:0x001009AF (linear 0x001109AF) | 0x00196003 |
| 603 | 1:0x001009B3 (linear 0x001109B3) | 0x00196007 |
| 605 | 1:0x001009B5 (linear 0x001109B5) | 0x00196009 |
| 606 | 1:0x001009B9 (linear 0x001109B9) | 0x0019600D |
| 607 | 1:0x001009CE (linear 0x001109CE) | 0x00196022 |
| 611 | 1:0x001009E3 (linear 0x001109E3) | 0x00196037 |
| 612 | 1:0x001009F0 (linear 0x001109F0) | 0x00196044 |
| 614 | 1:0x001009F5 (linear 0x001109F5) | 0x00196049 |
| 616 | 1:0x00100A0F (linear 0x00110A0F) | 0x00196063 |
| 617 | 1:0x00100A2B (linear 0x00110A2B) | 0x0019607F |
| 618 | 1:0x00100A47 (linear 0x00110A47) | 0x0019609B |
| 619 | 1:0x00100A63 (linear 0x00110A63) | 0x001960B7 |
| 620 | 1:0x00100A65 (linear 0x00110A65) | 0x001960B9 |
| 621 | 1:0x00100A69 (linear 0x00110A69) | 0x001960BD |
| 622 | 1:0x00100A7E (linear 0x00110A7E) | 0x001960D2 |
| 623 | 1:0x00100A96 (linear 0x00110A96) | 0x001960EA |
| 624 | 1:0x00100A9E (linear 0x00110A9E) | 0x001960F2 |
| 625 | 1:0x00100AA3 (linear 0x00110AA3) | 0x001960F7 |
| 626 | 1:0x00100AA8 (linear 0x00110AA8) | 0x001960FC |
| 627 | 1:0x00100AB7 (linear 0x00110AB7) | 0x0019610B |
| 628 | 1:0x00100AC6 (linear 0x00110AC6) | 0x0019611A |
| 629 | 1:0x00100ACD (linear 0x00110ACD) | 0x00196121 |
| 630 | 1:0x00100AD4 (linear 0x00110AD4) | 0x00196128 |
| 631 | 1:0x00100ADC (linear 0x00110ADC) | 0x00196130 |
| 632 | 1:0x00100AEC (linear 0x00110AEC) | 0x00196140 |
| 634 | 1:0x00100AF3 (linear 0x00110AF3) | 0x00196147 |
| 636 | 1:0x00100AFC (linear 0x00110AFC) | 0x00196150 |
| 638 | 1:0x00100B14 (linear 0x00110B14) | 0x00196168 |
| 639 | 1:0x00100B1E (linear 0x00110B1E) | 0x00196172 |
| 640 | 1:0x00100B28 (linear 0x00110B28) | 0x0019617C |
| 641 | 1:0x00100B2D (linear 0x00110B2D) | 0x00196181 |
| 804 | 1:0x00100B34 (linear 0x00110B34) | 0x00196188 |
| 806 | 1:0x00100B4C (linear 0x00110B4C) | 0x001961A0 |
| 807 | 1:0x00100B55 (linear 0x00110B55) | 0x001961A9 |
| 809 | 1:0x00100B5C (linear 0x00110B5C) | 0x001961B0 |
| 811 | 1:0x00100B74 (linear 0x00110B74) | 0x001961C8 |
| 812 | 1:0x00100B7D (linear 0x00110B7D) | 0x001961D1 |

