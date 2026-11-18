# F:\WLIBS\fonts\sysfont.c

Module 232 of the debug info of ORION2.EXE.

Ranges: 1:0x00134348 (linear 0x00144348), 1,411 bytes, 2:0x0000CA58 (linear 0x0017CA58), 17,176 bytes, 2:0x00048E7C (linear 0x001B8E7C), 11,520 bytes

#### Global symbols

| Address | Kind | Name | Chunk file |
|---|---|---|---|
| 1:0x00134348 (linear 0x00144348) | code | Load_System_Font_ | `regions/07-object-1/001C9654-001C9E53.txt` |
| 1:0x0013475C (linear 0x0014475C) | code | Load_System_Mouse_ | `regions/07-object-1/001C9654-001C9E53.txt` |
| 2:0x0000CA58 (linear 0x0017CA58) | data | _mouse_cursor_color | `regions/08-object-2/001FAE54-001FB653.txt` |
| 2:0x0000CA5C (linear 0x0017CA5C) | data | _column_lookup | `regions/08-object-2/001FAE54-001FB653.txt` |
| 2:0x0000CAA8 (linear 0x0017CAA8) | data | _thin_column_lookup | `regions/08-object-2/001FAE54-001FB653.txt` |
| 2:0x0000CAF4 (linear 0x0017CAF4) | data | _default_system_mouse_list | `regions/08-object-2/001FAE54-001FB653.txt` |
| 2:0x0000CB00 (linear 0x0017CB00) | data | _font_pixel_data | `regions/08-object-2/001FAE54-001FB653.txt` |
| 2:0x00010B00 (linear 0x00180B00) | data | _default_mouse_image | `regions/08-object-2/001FEE54-001FF653.txt` |
| 2:0x00010D40 (linear 0x00180D40) | data | _default_palette_colors | `regions/08-object-2/001FEE54-001FF653.txt` |
| 2:0x00048E7C (linear 0x001B8E7C) | data | _default_mouse_palette | - |

#### Module variables

| Address | Name | Type |
|---|---|---|
| 2:0x00048E7C (linear 0x001B8E7C) | default_mouse_palette | struct s_mouse_palette |
| 2:0x00010B00 (linear 0x00180B00) | default_mouse_image | char[576] |
| 2:0x0000CB00 (linear 0x0017CB00) | font_pixel_data | int[16][256] |
| 2:0x00010D40 (linear 0x00180D40) | default_palette_colors | char[3][16] |
| 2:0x0000CAF4 (linear 0x0017CAF4) | default_system_mouse_list | struct s_mouse_list |
| 2:0x0000CA58 (linear 0x0017CA58) | mouse_cursor_color | int |
| 2:0x0000CAA8 (linear 0x0017CAA8) | thin_column_lookup | int[19] |
| 2:0x0000CA5C (linear 0x0017CA5C) | column_lookup | int[19] |

#### Load_System_Font

- Address: 1:0x00134348 (linear 0x00144348), 1,044 bytes, in `regions/07-object-1/001C9654-001C9E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| data_offset | [EBP-0x1C] | int |
| char_num | [EBP-0xC] | int |
| row | [EBP-0x18] | int |
| pos | [EBP-0x14] | int |
| base_data | [EBP-0x10] | int |
| data | [EBP-0x3C] | int[8] |
| i | [EBP-0x8] | int |
| skip_count | [EBP-0x4] | int |

#### Load_System_Mouse

- Address: 1:0x0013475C (linear 0x0014475C), 367 bytes, in `regions/07-object-1/001C9654-001C9E53.txt`
- Type: void (void)
- Prologue 24 bytes, epilogue 9 bytes, return address at frame offset 0x18

| Local | Location | Type |
|---|---|---|
| i | [EBP-0x10] | int |
| j | [EBP-0xC] | int |
| white_color | [EBP-0x8] | int |
| black_color | [EBP-0x4] | int |

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
| 23 | typedef | size_t | name for unsigned int (#9) |
| 24 | pointer |  | near386; of char (#10) |
| 25 | array | __va_list | 0..0; of char * (#24) |
| 26 | typedef | __va_list | name for char *[1] (#25) |
| 27 | scalar | long | int 4 bytes |
| 28 | typedef | fpos_t | name for long (#27) |
| 29 | typedef | fpos_t | name for long (#27) |
| 30 | array |  | 0..575; of char (#10) |
| 31 | array |  | 0..19; of char[576] (#30) |
| 32 | struct | struct s_mouse_palette | 1 fields, 11520 bytes |
| 33 | typedef | s_mouse_palette | struct name for struct #32 (#32) |
| 34 | array |  | 0..575; of char (#10) |
| 35 | array |  | 0..15; of int (#8) |
| 36 | array |  | 0..255; of int[16] (#35) |
| 37 | array |  | 0..2; of char (#10) |
| 38 | array |  | 0..15; of char[3] (#37) |
| 39 | struct | struct s_mouse_list | 6 fields, 12 bytes |
| 40 | typedef | s_mouse_list | struct name for struct #39 (#39) |
| 41 | array |  | 0..18; of int (#8) |
| 42 | array |  | 0..18; of int (#8) |
| 43 | scalar | void | void 1 bytes |
| 44 | procedure |  | near386; returns void () |
| 45 | array |  | 0..7; of int (#8) |

##### struct s_mouse_palette (#32, 1 fields, 11520 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| mouse_images | offset 0 | char[576][20] |

##### struct s_mouse_list (#39, 6 fields, 12 bytes)

| Field | Offset / bits | Type |
|---|---|---|
| image_num | offset 0 | SWORD |
| center_offset | offset 2 | SWORD |
| x1 | offset 4 | SWORD |
| y1 | offset 6 | SWORD |
| x2 | offset 8 | SWORD |
| y2 | offset 10 | SWORD |

#### Line numbers

| Line | Address | File offset |
|---|---|---|
| 332 | 1:0x00134348 (linear 0x00144348) | 0x001C999C |
| 336 | 1:0x00134360 (linear 0x00144360) | 0x001C99B4 |
| 338 | 1:0x0013436A (linear 0x0014436A) | 0x001C99BE |
| 340 | 1:0x0013436F (linear 0x0014436F) | 0x001C99C3 |
| 342 | 1:0x00134380 (linear 0x00144380) | 0x001C99D4 |
| 343 | 1:0x0013438F (linear 0x0014438F) | 0x001C99E3 |
| 344 | 1:0x00134398 (linear 0x00144398) | 0x001C99EC |
| 345 | 1:0x001343A1 (linear 0x001443A1) | 0x001C99F5 |
| 347 | 1:0x001343AA (linear 0x001443AA) | 0x001C99FE |
| 349 | 1:0x001343D0 (linear 0x001443D0) | 0x001C9A24 |
| 351 | 1:0x001343D7 (linear 0x001443D7) | 0x001C9A2B |
| 353 | 1:0x001343F4 (linear 0x001443F4) | 0x001C9A48 |
| 355 | 1:0x00134403 (linear 0x00144403) | 0x001C9A57 |
| 357 | 1:0x0013441D (linear 0x0014441D) | 0x001C9A71 |
| 359 | 1:0x0013443D (linear 0x0014443D) | 0x001C9A91 |
| 361 | 1:0x00134464 (linear 0x00144464) | 0x001C9AB8 |
| 362 | 1:0x00134474 (linear 0x00144474) | 0x001C9AC8 |
| 363 | 1:0x00134484 (linear 0x00144484) | 0x001C9AD8 |
| 364 | 1:0x00134494 (linear 0x00144494) | 0x001C9AE8 |
| 366 | 1:0x001344A4 (linear 0x001444A4) | 0x001C9AF8 |
| 367 | 1:0x001344AB (linear 0x001444AB) | 0x001C9AFF |
| 368 | 1:0x001344B2 (linear 0x001444B2) | 0x001C9B06 |
| 370 | 1:0x001344C9 (linear 0x001444C9) | 0x001C9B1D |
| 371 | 1:0x001344D9 (linear 0x001444D9) | 0x001C9B2D |
| 373 | 1:0x001344DB (linear 0x001444DB) | 0x001C9B2F |
| 375 | 1:0x001344E1 (linear 0x001444E1) | 0x001C9B35 |
| 376 | 1:0x001344F1 (linear 0x001444F1) | 0x001C9B45 |
| 378 | 1:0x001344F4 (linear 0x001444F4) | 0x001C9B48 |
| 379 | 1:0x001344FF (linear 0x001444FF) | 0x001C9B53 |
| 380 | 1:0x00134502 (linear 0x00144502) | 0x001C9B56 |
| 382 | 1:0x00134509 (linear 0x00144509) | 0x001C9B5D |
| 383 | 1:0x0013450B (linear 0x0014450B) | 0x001C9B5F |
| 384 | 1:0x00134516 (linear 0x00144516) | 0x001C9B6A |
| 385 | 1:0x00134519 (linear 0x00144519) | 0x001C9B6D |
| 386 | 1:0x0013451E (linear 0x0014451E) | 0x001C9B72 |
| 387 | 1:0x0013452A (linear 0x0014452A) | 0x001C9B7E |
| 388 | 1:0x00134536 (linear 0x00144536) | 0x001C9B8A |
| 389 | 1:0x00134542 (linear 0x00144542) | 0x001C9B96 |
| 392 | 1:0x00134547 (linear 0x00144547) | 0x001C9B9B |
| 394 | 1:0x0013454C (linear 0x0014454C) | 0x001C9BA0 |
| 395 | 1:0x0013455B (linear 0x0014455B) | 0x001C9BAF |
| 396 | 1:0x00134564 (linear 0x00144564) | 0x001C9BB8 |
| 397 | 1:0x0013456D (linear 0x0014456D) | 0x001C9BC1 |
| 399 | 1:0x00134576 (linear 0x00144576) | 0x001C9BCA |
| 401 | 1:0x0013459C (linear 0x0014459C) | 0x001C9BF0 |
| 403 | 1:0x001345A3 (linear 0x001445A3) | 0x001C9BF7 |
| 405 | 1:0x001345C0 (linear 0x001445C0) | 0x001C9C14 |
| 407 | 1:0x001345CF (linear 0x001445CF) | 0x001C9C23 |
| 409 | 1:0x001345E9 (linear 0x001445E9) | 0x001C9C3D |
| 411 | 1:0x00134609 (linear 0x00144609) | 0x001C9C5D |
| 413 | 1:0x00134630 (linear 0x00144630) | 0x001C9C84 |
| 414 | 1:0x00134640 (linear 0x00144640) | 0x001C9C94 |
| 415 | 1:0x00134650 (linear 0x00144650) | 0x001C9CA4 |
| 416 | 1:0x00134660 (linear 0x00144660) | 0x001C9CB4 |
| 417 | 1:0x00134670 (linear 0x00144670) | 0x001C9CC4 |
| 418 | 1:0x00134680 (linear 0x00144680) | 0x001C9CD4 |
| 419 | 1:0x00134690 (linear 0x00144690) | 0x001C9CE4 |
| 420 | 1:0x001346A0 (linear 0x001446A0) | 0x001C9CF4 |
| 422 | 1:0x001346B0 (linear 0x001446B0) | 0x001C9D04 |
| 423 | 1:0x001346B7 (linear 0x001446B7) | 0x001C9D0B |
| 424 | 1:0x001346BE (linear 0x001446BE) | 0x001C9D12 |
| 426 | 1:0x001346D5 (linear 0x001446D5) | 0x001C9D29 |
| 427 | 1:0x001346E5 (linear 0x001446E5) | 0x001C9D39 |
| 429 | 1:0x001346E7 (linear 0x001446E7) | 0x001C9D3B |
| 431 | 1:0x001346ED (linear 0x001446ED) | 0x001C9D41 |
| 432 | 1:0x001346FD (linear 0x001446FD) | 0x001C9D51 |
| 434 | 1:0x00134700 (linear 0x00144700) | 0x001C9D54 |
| 435 | 1:0x0013470B (linear 0x0014470B) | 0x001C9D5F |
| 436 | 1:0x0013470E (linear 0x0014470E) | 0x001C9D62 |
| 438 | 1:0x00134715 (linear 0x00144715) | 0x001C9D69 |
| 439 | 1:0x00134717 (linear 0x00144717) | 0x001C9D6B |
| 440 | 1:0x00134722 (linear 0x00144722) | 0x001C9D76 |
| 441 | 1:0x00134725 (linear 0x00144725) | 0x001C9D79 |
| 442 | 1:0x0013472A (linear 0x0014472A) | 0x001C9D7E |
| 443 | 1:0x00134736 (linear 0x00144736) | 0x001C9D8A |
| 444 | 1:0x00134742 (linear 0x00144742) | 0x001C9D96 |
| 445 | 1:0x0013474E (linear 0x0014474E) | 0x001C9DA2 |
| 447 | 1:0x00134753 (linear 0x00144753) | 0x001C9DA7 |
| 450 | 1:0x0013475C (linear 0x0014475C) | 0x001C9DB0 |
| 455 | 1:0x00134774 (linear 0x00144774) | 0x001C9DC8 |
| 456 | 1:0x0013477C (linear 0x0014477C) | 0x001C9DD0 |
| 458 | 1:0x00134784 (linear 0x00144784) | 0x001C9DD8 |
| 459 | 1:0x0013479B (linear 0x0014479B) | 0x001C9DEF |
| 461 | 1:0x001347B2 (linear 0x001447B2) | 0x001C9E06 |
| 462 | 1:0x001347D0 (linear 0x001447D0) | 0x001C9E24 |
| 463 | 1:0x001347EE (linear 0x001447EE) | 0x001C9E42 |
| 465 | 1:0x001347F2 (linear 0x001447F2) | 0x001C9E46 |
| 466 | 1:0x0013480C (linear 0x0014480C) | 0x001C9E60 |
| 468 | 1:0x00134826 (linear 0x00144826) | 0x001C9E7A |
| 469 | 1:0x0013483F (linear 0x0014483F) | 0x001C9E93 |
| 470 | 1:0x00134852 (linear 0x00144852) | 0x001C9EA6 |
| 472 | 1:0x00134854 (linear 0x00144854) | 0x001C9EA8 |
| 474 | 1:0x00134864 (linear 0x00144864) | 0x001C9EB8 |
| 475 | 1:0x0013487B (linear 0x0014487B) | 0x001C9ECF |
| 476 | 1:0x00134891 (linear 0x00144891) | 0x001C9EE5 |
| 477 | 1:0x00134893 (linear 0x00144893) | 0x001C9EE7 |
| 480 | 1:0x001348A9 (linear 0x001448A9) | 0x001C9EFD |
| 482 | 1:0x001348B3 (linear 0x001448B3) | 0x001C9F07 |
| 484 | 1:0x001348C2 (linear 0x001448C2) | 0x001C9F16 |

