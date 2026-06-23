/*
    Video
    VGA

    MAGIC.EXE
        ¿ DNE ?

    WIZARDS.EXE
        seg026
        seg027
        seg028
*/

#include "MOX_DEF.h"
#include "Mouse.h"
#include "Video.h"
#include "Video2.h"

#include "../../platform/include/Platform.h"  /* Platform_Palette_Update(), Platform_Video_Update() */

#include <string.h>     /* memcpy() */



/*
video_page_buffer
emulates 256KB of IBM-PC VRAM
[4] of 64KB

*/

// WZD dseg:76EE
int16_t draw_page_num = 0;

uint8_t * current_video_page;
// uint8_t * video_page_buffer[4];  // On, Off, Back, ¿ Back2 ?
uint8_t * video_memory;
uint8_t * video_page_buffer[4];  // On, Off, Back, ¿ Back2 ? {0A000h,0A400h,0A800h,0AC00h}
uint8_t * draw_page;

/* CLAUDE: optional per-pass delay (ms) for PageFlip_Dissolve so the mosaic transition is visible on
   fast hardware / in demos.  Default 0 reproduces the original instant (VRAM-write-paced) behavior. */
int16_t g_pageflip_dissolve_pass_delay = 0;



/*
    WIZARDS.EXE  seg026
*/

// seg026:000E 02 02 02 01 01 03 00 03 03 02 00 01 03 03 01 03+Mosaic_Rnd256_Bits db 2, 2, 2, 1, 1, 3, 0, 3, 3, 2, 0, 1, 3, 3, 1, 3, 3, 0, 3, 1, 2, 3, 0, 2, 1, 1, 2, 2, 1, 1, 3, 3, 2, 3, 3, 0, 2, 3, 0, 2, 0, 3, 2, 0, 2, 0, 0, 3, 1, 0, 1, 1, 1, 3, 3, 1, 1, 0, 0, 2, 1, 1, 2, 1, 0, 0, 0, 3, 3, 1, 1, 2, 2, 1, 2, 2, 0, 2, 3, 1, 2, 2, 2, 0, 0
// seg026:000E 03 00 03 01 02 03 00 02 01 01 02 02 01 01 03 03+                                        ; DATA XREF: PageFlip_Dissolve+CCr ...
// seg026:000E 02 03 03 00 02 03 00 02 00 03 02 00 02 00 00 03+db 0, 1, 1, 0, 2, 0, 3, 2, 2, 1, 0, 3, 1, 2, 3, 0, 0, 1, 0, 2, 2, 3, 2, 1, 1, 2, 2, 0, 3, 0, 0, 2, 0, 0, 3, 3, 1, 2, 1, 0, 0, 1, 3, 1, 1, 1, 0, 0, 2, 3, 1, 0, 3, 3, 0, 1, 0, 0, 0, 1, 3, 1, 3, 3, 2, 2, 3, 2, 0, 1, 1, 0, 3, 0, 2, 0, 2, 1, 1, 3, 1, 2, 1, 3, 1
// seg026:000E 01 00 01 01 01 03 03 01 01 00 00 02 01 01 02 01+db 0, 1, 3, 3, 1, 1, 3, 2, 3, 2, 3, 1, 1, 2, 0, 2, 3, 3, 2, 2, 0, 2, 3, 3, 3, 2, 2, 0, 2, 0, 1, 0, 1, 3, 2, 1, 2, 2, 0, 1, 0, 2, 1, 1, 3, 0, 3, 3, 3, 0, 3, 0, 2, 1, 1, 0, 0, 2, 1, 2, 3, 3, 1, 0, 1, 3, 0, 2, 3, 0, 2, 1, 2, 3, 0, 2, 2, 0, 2, 3, 1, 0, 3, 3, 3
// seg026:000E 00 00 00 03 03 01 01 02 02 01 02 02 00 02 03 01+db 0
// seg026:010E 00 03 1E 12 1C 16 1D 0C 22 2F 1F 20 07 31 2E 0E+Mosaic_Rnd50_Tbl db 0, 3, 1Eh, 12h, 1Ch, 16h, 1Dh, 0Ch, 22h, 2Fh, 1Fh, 20h, 7, 31h, 2Eh, 0Eh, 26h, 2Bh, 23h, 28h, 0Bh, 9, 24h, 4, 21h, 1Ah, 27h, 0Fh, 5, 15h, 2, 10h, 19h, 0Ah, 30h, 0Dh, 18h, 25h, 11h, 13h, 2Ch, 6, 8, 14h, 1, 17h, 29h, 1Bh, 2Dh, 2Ah
// seg026:010E 26 2B 23 28 0B 09 24 04 21 1A 27 0F 05 15 02 10+                                        ; DATA XREF: PageFlip_Dissolve+7Er ...
// seg026:0140 02 09 1E 26 0D 1B 1F 01 13 08 18 24 25 1A 00 0B+Mosaic_Rnd40_Tbl db 2, 9, 1Eh, 26h, 0Dh, 1Bh, 1Fh, 1, 13h, 8, 18h, 24h, 25h, 1Ah, 0, 0Bh, 17h, 20h, 16h, 0Ch, 10h, 14h, 1Dh, 12h, 1Ch, 7, 0Ah, 23h, 0Eh, 19h, 5, 21h, 0Fh, 15h, 4, 11h, 6, 3, 27h, 22h
// seg026:0140 17 20 16 0C 10 14 1D 12 1C 07 0A 23 0E 19 05 21+                                        ; DATA XREF: PageFlip_Dissolve+B0r
// seg026:0168 00                                              db    0
// seg026:0169 00                                              db    0
// seg026:016A 00                                              db    0
// seg026:016B 00                                              db    0
// seg026:016C 00 00                                           Mosaic_InitVar dw 0                     ; DATA XREF: PageFlip_Dissolve+49w ...
// seg026:016E 00 00                                           Mosaic_Rnd40_Index dw 0                 ; DATA XREF: PageFlip_Dissolve+6Bw ...
// seg026:0170 00 00                                           Mosaic_Init64Times dw 0                 ; DATA XREF: PageFlip_Dissolve+67w ...
// seg026:0172 00 00                                           Mosaic_Rnd50_Index dw 0                 ; DATA XREF: PageFlip_Dissolve:loc_1EFD6w ...
// seg026:0174 00 00                                           VGA_ScanLine_Offset dw 0                ; DATA XREF: RP_VGA_CutRight+3Bw ...
// seg026:0176 00 00                                           VGA_Write_Map dw 0                      ; DATA XREF: RP_VGA_CutRight:loc_1F2D5w ...
// seg026:0178 00 00                                           VGA_Read_Map dw 0                       ; DATA XREF: RP_VGA_CutRight+49w ...
// seg026:017A 00 00                                           VGA_SlideInColumn dw 0                  ; DATA XREF: UU_VGA_InterleaveIn+10w ...
// seg026:017C 00 00                                           VGA_Line_Count dw 0                     ; DATA XREF: UU_VGA_CrossSlideOut+14w ...
// seg026:017E 00 00                                           VGA_Start_Offset dw 0                   ; DATA XREF: UU_VGA_CrossSlideOut+Dw ...
// seg026:0180 00 00                                           db 2 dup(0)
// seg026:0182 00 00                                           VGA_Horz_Slope dw 0                     ; DATA XREF: UU_VGA_CopyLine+8Dw ...
// seg026:0184 00 00                                           VGA_Vert_Slope dw 0                     ; DATA XREF: UU_VGA_CopyLine+75w ...
// seg026:0186 00 00                                           VGA_Y_Dir dw 0                          ; DATA XREF: UU_VGA_CopyLine+59w ...
// seg026:0188 00 00                                           VGA_Line_Length dw 0                    ; DATA XREF: UU_VGA_CopyLine+70w ...
// seg026:018A 00 00                                           VGA_Display_Seg dw 0                    ; DATA XREF: PageFlip_Dissolve+31w ...
// seg026:018C 00 00                                           VGA_Draw_Seg dw 0                       ; DATA XREF: PageFlip_Dissolve+3Ew ...
// seg026:018E 00 00                                           VGA_Read_Mask dw 0                      ; DATA XREF: UU_VGA_CopyLine+102w ...
// seg026:0190 00 00                                           VGA_Line_Offset dw 0                    ; DATA XREF: VGA_ShiftCopyFromF3+27w ...
// seg026:0192 00 00                                           VGA_Origin_Offset dw 0                  ; DATA XREF: VGA_ShiftCopyFromF3+36w ...

int8_t Mosaic_Rnd256_Bits[256] ={
    2, 2, 2, 1, 1, 3, 0, 3, 3, 2, 0, 1, 3, 3, 1, 3,
    3, 0, 3, 1, 2, 3, 0, 2, 1, 1, 2, 2, 1, 1, 3, 3,
    2, 3, 3, 0, 2, 3, 0, 2, 0, 3, 2, 0, 2, 0, 0, 3,
    1, 0, 1, 1, 1, 3, 3, 1, 1, 0, 0, 2, 1, 1, 2, 1,
    0, 0, 0, 3, 3, 1, 1, 2, 2, 1, 2, 2, 0, 2, 3, 1,
    2, 2, 2, 0, 0, 0, 1, 1, 0, 2, 0, 3, 2, 2, 1, 0,
    3, 1, 2, 3, 0, 0, 1, 0, 2, 2, 3, 2, 1, 1, 2, 2,
    0, 3, 0, 0, 2, 0, 0, 3, 3, 1, 2, 1, 0, 0, 1, 3,
    1, 1, 1, 0, 0, 2, 3, 1, 0, 3, 3, 0, 1, 0, 0, 0,
    1, 3, 1, 3, 3, 2, 2, 3, 2, 0, 1, 1, 0, 3, 0, 2,
    0, 2, 1, 1, 3, 1, 2, 1, 3, 1, 0, 1, 3, 3, 1, 1,
    3, 2, 3, 2, 3, 1, 1, 2, 0, 2, 3, 3, 2, 2, 0, 2,
    3, 3, 3, 2, 2, 0, 2, 0, 1, 0, 1, 3, 2, 1, 2, 2,
    0, 1, 0, 2, 1, 1, 3, 0, 3, 3, 3, 0, 3, 0, 2, 1,
    1, 0, 0, 2, 1, 2, 3, 3, 1, 0, 1, 3, 0, 2, 3, 0,
    2, 1, 2, 3, 0, 2, 2, 0, 2, 3, 1, 0, 3, 3, 3, 0
};
int8_t Mosaic_Rnd50_Tbl[50] = {
     0,  3, 30, 18, 28, 22, 29, 12, 34, 47,
    31, 32,  7, 49, 46, 14, 38, 43, 35, 40,
    11,  9, 36,  4, 33, 26, 39, 15,  5, 21,
     2, 16, 25, 10, 48, 13, 24, 37, 17, 19,
    44,  6,  8, 20,  1, 23, 41, 27, 45, 42
};
int8_t Mosaic_Rnd40_Tbl[40] = {
     2,  9, 30, 38, 13, 27, 31,  1, 19,  8,
    24, 36, 37, 26,  0, 11, 23, 32, 22, 12,
    16, 20, 29, 18, 28,  7, 10, 35, 14, 25,
     5, 33, 15, 21,  4, 17,  6,  3, 39, 34
};

// WZD s26p01

// 1EDD:0194                                                 proc PageFlip_Dissolve far           ; CODE XREF: PageFlip_FX+2CP ...
// 1EDD:0194                                                 UU_y2= word ptr -12h
// 1EDD:0194                                                 UU_x2= word ptr -10h
// 1EDD:0194                                                 UU_y1= word ptr -0Eh
// 1EDD:0194                                                 UU_x1= word ptr -0Ch
// 1EDD:0194 55                                                  push bp
// 1EDD:0195 8B EC                                               mov bp, sp
// 1EDD:0197 83 EC 12                                            sub sp, 12h
// 1EDD:019A 06                                                  push es
// 1EDD:019B 57                                                  push di
// 1EDD:019C 56                                                  push si
// 1EDD:019D 1E                                                  push ds
// 1EDD:019D
// 1EDD:019D
// 1EDD:019E 0E                                                  push cs
// 1EDD:019F E8 1F 09                                            call near ptr Toggle_Pages_No_Draw_Fields_Pre
// 1EDD:01A2 90                                                  nop
// 1EDD:01A3 C7 46 F4 00 00                                      mov [bp+UU_x1], 0
// 1EDD:01A8 C7 46 F0 3F 01                                      mov [bp+UU_x2], 319
// 1EDD:01AD C7 46 F2 00 00                                      mov [bp+UU_y1], 0
// 1EDD:01B2 C7 46 EE 31 00                                      mov [bp+UU_y2], 49
// 1EDD:01B2
// 1EDD:01B2
// 1EDD:01B7 A1 EE 76                                            mov ax, [draw_page_num]
// 1EDD:01BA 8A E0                                               mov ah, al
// 1EDD:01BC 32 C0                                               xor al, al
// 1EDD:01BE D1 E0                                               shl ax, 1
// 1EDD:01C0 D1 E0                                               shl ax, 1
// 1EDD:01C2 05 00 A0                                            add ax, 0A000h
// 1EDD:01C5 2E A3 8A 01                                         mov [cs:VGA_Display_Seg], ax
// 1EDD:01C5
// 1EDD:01C5
// 1EDD:01C9 BB 00 A4                                            mov bx, 0A400h
// 1EDD:01CC 2B D8                                               sub bx, ax
// 1EDD:01CE 81 C3 00 A0                                         add bx, 0A000h
// 1EDD:01D2 2E 89 1E 8C 01                                      mov [cs:VGA_Draw_Seg], bx
// 1EDD:01D2
// 1EDD:01D2
// 1EDD:01D7 2E A1 8A 01                                         mov ax, [cs:VGA_Display_Seg]
// 1EDD:01DB 8E C0                                               mov es, ax
// 1EDD:01DB
// 1EDD:01DB
// 1EDD:01DD 2E C7 06 6C 01 04 00                                mov [cs:Mosaic_InitVar], 4
// 1EDD:01DD
// 1EDD:01DD
// 1EDD:01E4 2E A1 8C 01                                         mov ax, [cs:VGA_Draw_Seg]
// 1EDD:01E8 8E D8                                               mov ds, ax
// 1EDD:01EA                                                 loc_1EFBA:                              ; CODE XREF: PageFlip_Dissolve+177j
// 1EDD:01EA 2E A1 6C 01                                         mov ax, [cs:Mosaic_InitVar]
// 1EDD:01EE 48                                                  dec ax
// 1EDD:01EF D1 E0                                               shl ax, 1
// 1EDD:01F1 D1 E0                                               shl ax, 1
// 1EDD:01F3 D1 E0                                               shl ax, 1
// 1EDD:01F5 D1 E0                                               shl ax, 1
// 1EDD:01F7 D1 E0                                               shl ax, 1
// 1EDD:01F9 D1 E0                                               shl ax, 1
// 1EDD:01FB 2E A3 70 01                                         mov [cs:Mosaic_Init64Times], ax
// 1EDD:01FB
// 1EDD:01FB
// 1EDD:01FF 2E C7 06 6E 01 27 00                                mov [cs:Mosaic_Rnd40_Index], 39
// 1EDD:0206                                                 loc_1EFD6:                              ; CODE XREF: PageFlip_Dissolve+16Dj
// 1EDD:0206 2E C7 06 72 01 31 00                                mov [cs:Mosaic_Rnd50_Index], 49
// 1EDD:020D                                                 loc_1EFDD:                              ; CODE XREF: PageFlip_Dissolve+163j
// 1EDD:020D 2E 8B 1E 72 01                                      mov bx, [cs:Mosaic_Rnd50_Index]
// 1EDD:0212 2E 8A 9F 0E 01                                      mov bl, [cs:Mosaic_Rnd50_Tbl+bx]
// 1EDD:0217 32 FF                                               xor bh, bh
// 1EDD:0219 8B CB                                               mov cx, bx
// 1EDD:021B 8B D3                                               mov dx, bx
// 1EDD:021D D1 E3                                               shl bx, 1
// 1EDD:021F D1 E3                                               shl bx, 1
// 1EDD:0221 03 D9                                               add bx, cx
// 1EDD:0223 D1 E3                                               shl bx, 1
// 1EDD:0225 D1 E3                                               shl bx, 1
// 1EDD:0227 D1 E3                                               shl bx, 1
// 1EDD:0229 D1 E3                                               shl bx, 1
// 1EDD:022B 8B C3                                               mov ax, bx                          ; = bx * 80
// 1EDD:022D 2E 03 0E 6E 01                                      add cx, [cs:Mosaic_Rnd40_Index]
// 1EDD:0232 83 F9 28                                            cmp cx, 40
// 1EDD:0235 78 0B                                               js  short loc_1F012
// 1EDD:0237 83 E9 28                                            sub cx, 40
// 1EDD:0237
// 1EDD:0237
// 1EDD:023A 83 F9 28                                            cmp cx, 40
// 1EDD:023D 78 03                                               js  short loc_1F012
// 1EDD:023F 83 E9 28                                            sub cx, 40
// 1EDD:0242                                                 loc_1F012:                              ; CODE XREF: PageFlip_Dissolve+A1j ...
// 1EDD:0242 8B D9                                               mov bx, cx
// 1EDD:0244 2E 8A 9F 40 01                                      mov bl, [cs:Mosaic_Rnd40_Tbl+bx]
// 1EDD:0249 32 FF                                               xor bh, bh
// 1EDD:024B 8B CB                                               mov cx, bx
// 1EDD:024D D1 E3                                               shl bx, 1
// 1EDD:024F 03 C3                                               add ax, bx
// 1EDD:0251 8B F0                                               mov si, ax                          ; rnd(50) * 80 + rnd(40) * 2
// 1EDD:0253 8B F8                                               mov di, ax
// 1EDD:0255 03 CA                                               add cx, dx
// 1EDD:0257 2E 03 0E 70 01                                      add cx, [cs:Mosaic_Init64Times]     ; rnd(50) + rnd(40) + 64*(3..2..1)
// 1EDD:025C 32 ED                                               xor ch, ch
// 1EDD:025E 8B D9                                               mov bx, cx
// 1EDD:0260 2E 8A A7 0E 00                                      mov ah, [cs:Mosaic_Rnd256_Bits+bx]
// 1EDD:0265 FE C3                                               inc bl
// 1EDD:0267 BA CE 03                                            mov dx, 3CEh
// 1EDD:026A B0 04                                               mov al, 4
// 1EDD:026C EE                                                  out dx, al                          ; EGA: graph 1 and 2 addr reg:
// 1EDD:026C                                                                                         ; read map select.
// 1EDD:026C                                                                                         ; Data bits 0-2 select map # for read mode 00.
// 1EDD:026D 42                                                  inc dx
// 1EDD:026E 8A C4                                               mov al, ah
// 1EDD:0270 EE                                                  out dx, al                          ; EGA port: graphics controller data register
// 1EDD:0271 8A CC                                               mov cl, ah
// 1EDD:0273 B4 01                                               mov ah, 1
// 1EDD:0275 80 F9 00                                            cmp cl, 0
// 1EDD:0278 74 04                                               jz  short loc_1F04E
// 1EDD:027A                                                 loc_1F04A:                              ; CODE XREF: PageFlip_Dissolve+E8j
// 1EDD:027A D0 E4                                               shl ah, 1
// 1EDD:027C E2 FC                                               loop loc_1F04A
// 1EDD:027E                                                 loc_1F04E:                              ; CODE XREF: PageFlip_Dissolve+E4j
// 1EDD:027E BA C4 03                                            mov dx, 3C4h
// 1EDD:0281 B0 02                                               mov al, 2
// 1EDD:0283 EE                                                  out dx, al                          ; EGA: sequencer address reg
// 1EDD:0283                                                                                         ; map mask: data bits 0-3 enable writes to bit planes 0-3
// 1EDD:0284 42                                                  inc dx
// 1EDD:0285 8A C4                                               mov al, ah
// 1EDD:0287 EE                                                  out dx, al                          ; EGA port: sequencer data register
// 1EDD:0288 A4                                                  movsb
// 1EDD:0289 81 C6 3F 1F                                         add si, 7999                        ; move down half a screen
// 1EDD:028D 81 C7 3F 1F                                         add di, 7999
// 1EDD:0291 A4                                                  movsb
// 1EDD:0292 81 EE A1 0F                                         sub si, 4001                        ; move back a quarter screen
// 1EDD:0296 81 EF A1 0F                                         sub di, 4001
// 1EDD:029A A4                                                  movsb
// 1EDD:029B 81 C6 3F 1F                                         add si, 7999                        ; move down half a screen
// 1EDD:029F 81 C7 3F 1F                                         add di, 7999
// 1EDD:02A3 A4                                                  movsb
// 1EDD:02A4 81 EE E0 2E                                         sub si, 12000                       ; back to the start
// 1EDD:02A8 81 EF E0 2E                                         sub di, 12000
// 1EDD:02AC 2E 8A A7 0E 00                                      mov ah, [cs:Mosaic_Rnd256_Bits+bx]
// 1EDD:02B1 FE C3                                               inc bl
// 1EDD:02B3 BA CE 03                                            mov dx, 3CEh
// 1EDD:02B6 B0 04                                               mov al, 4
// 1EDD:02B8 EE                                                  out dx, al                          ; EGA: graph 1 and 2 addr reg:
// 1EDD:02B8                                                                                         ; read map select.
// 1EDD:02B8                                                                                         ; Data bits 0-2 select map # for read mode 00.
// 1EDD:02B9 42                                                  inc dx
// 1EDD:02BA 8A C4                                               mov al, ah
// 1EDD:02BC EE                                                  out dx, al                          ; EGA port: graphics controller data register
// 1EDD:02BD 8A CC                                               mov cl, ah
// 1EDD:02BF B4 01                                               mov ah, 1
// 1EDD:02C1 80 F9 00                                            cmp cl, 0
// 1EDD:02C4 74 04                                               jz  short loc_1F09A
// 1EDD:02C6                                                 loc_1F096:                              ; CODE XREF: PageFlip_Dissolve+134j
// 1EDD:02C6 D0 E4                                               shl ah, 1
// 1EDD:02C8 E2 FC                                               loop loc_1F096
// 1EDD:02CA                                                 loc_1F09A:                              ; CODE XREF: PageFlip_Dissolve+130j
// 1EDD:02CA BA C4 03                                            mov dx, 3C4h
// 1EDD:02CD B0 02                                               mov al, 2
// 1EDD:02CF EE                                                  out dx, al                          ; EGA: sequencer address reg
// 1EDD:02CF                                                                                         ; map mask: data bits 0-3 enable writes to bit planes 0-3
// 1EDD:02D0 42                                                  inc dx
// 1EDD:02D1 8A C4                                               mov al, ah
// 1EDD:02D3 EE                                                  out dx, al                          ; EGA port: sequencer data register
// 1EDD:02D4 A4                                                  movsb
// 1EDD:02D5 81 C6 3F 1F                                         add si, 7999                        ; move down half a screen
// 1EDD:02D9 81 C7 3F 1F                                         add di, 7999
// 1EDD:02DD A4                                                  movsb
// 1EDD:02DE 81 EE A1 0F                                         sub si, 4001                        ; move back a quarter screen
// 1EDD:02E2 81 EF A1 0F                                         sub di, 4001
// 1EDD:02E6 A4                                                  movsb
// 1EDD:02E7 81 C6 3F 1F                                         add si, 7999                        ; move down half a screen
// 1EDD:02EB 81 C7 3F 1F                                         add di, 7999
// 1EDD:02EF A4                                                  movsb
// 1EDD:02F0 2E FF 0E 72 01                                      dec [cs:Mosaic_Rnd50_Index]
// 1EDD:02F5 78 03                                               js  short loc_1F0CA
// 1EDD:02F7 E9 13 FF                                            jmp loc_1EFDD
// 1EDD:02FA                                                 ; ---------------------------------------------------------------------------
// 1EDD:02FA                                                 loc_1F0CA:                              ; CODE XREF: PageFlip_Dissolve+161j
// 1EDD:02FA 2E FF 0E 6E 01                                      dec [cs:Mosaic_Rnd40_Index]
// 1EDD:02FF 78 03                                               js  short loc_1F0D4
// 1EDD:0301 E9 02 FF                                            jmp loc_1EFD6
// 1EDD:0304                                                 ; ---------------------------------------------------------------------------
// 1EDD:0304                                                 loc_1F0D4:                              ; CODE XREF: PageFlip_Dissolve+16Bj
// 1EDD:0304 2E FF 0E 6C 01                                      dec [cs:Mosaic_InitVar]
// 1EDD:0309 74 03                                               jz  short @@Done
// 1EDD:030B E9 DC FE                                            jmp loc_1EFBA
// 1EDD:030E                                                 ; ---------------------------------------------------------------------------
// 1EDD:030E                                                 @@Done:                                 ; CODE XREF: PageFlip_Dissolve+175j
// 1EDD:030E 0E                                                  push cs
// 1EDD:030F E8 BC 07                                            call near ptr Toggle_Pages_No_Draw_Fields
// 1EDD:030F
// 1EDD:030F
// 1EDD:0312 90                                                  nop
// 1EDD:0312
// 1EDD:0312
// 1EDD:0313 1F                                                  pop ds
// 1EDD:0314 5E                                                  pop si
// 1EDD:0315 5F                                                  pop di
// 1EDD:0316 07                                                  pop es
// 1EDD:0317 8B E5                                               mov sp, bp
// 1EDD:0319 5D                                                  pop bp
// 1EDD:031A CB                                                  retf
// 1EDD:031A                                                 endp PageFlip_Dissolve

/* CLAUDE */
/*
    Mosaic dissolve page-flip transition.

    Reconstruction of the Mode-X original (disassembly above) for ReMoM's linear 8bpp framebuffer model.
    The original programmed the VGA Graphics Controller (read map select, port 3CEh) and Sequencer (map mask, port 3C4h) to copy one bit-plane of a byte at a time; on a linear
    320x200 buffer each of those plane-selected byte copies collapses to a single pixel copy at x = bytecol*4 + plane.  All the port I/O, the +7999/-4001 quadrant addressing and the
    rep-movsb disappear; what remains is a pseudo-random per-pixel reveal of the freshly-drawn off page onto the visible page.

    Per grid cell (and per pass) the original touches two adjacent Mode-X bytes (offset O and O+1), each across four 50-row bands (O, O+4000, O+8000, O+12000 => rows r, r+50, r+100,
    r+150).  That yields, per cell+pass, one pixel in the left half and one in the right half of the 8-pixel-wide block, in each of the four bands:
        left  : x = col_idx*8     + Mosaic_Rnd256_Bits[bits_index]
        right : x = col_idx*8 + 4 + Mosaic_Rnd256_Bits[bits_index + 1]
    with bits_index = (col_idx + rnd50_row + init64) & 0xFF.

    Mosaic_Rnd256_Bits has the property that every group of four entries spaced 64 apart is a permutation of {0,1,2,3}; since the four passes use init64 in {0,64,128,192}, each pixel is
    written exactly once across the four passes (4 * 40 * 50 * 8 == 64000 == 320*200) -- a complete, grainy, four-step dissolve.

    The original relied on slow VGA VRAM write latency for pacing and issued no delay; here the visible page is pushed to the window once per pass (Platform_Video_Update), giving the
    four grain steps without any platform-specific timing calls.
*/
void PageFlip_Dissolve(void)
{
    int16_t pass;
    int16_t init64;
    int16_t col_step;
    int16_t row_step;
    int16_t rnd50_row;
    int16_t rnd40_col;
    int16_t col_idx;
    uint8_t bits_index;
    int16_t x_left;
    int16_t x_right;
    int16_t band;
    int32_t row_offset;
    uint8_t * src;   /* freshly-drawn off page */
    uint8_t * dst;   /* visible page           */

    Toggle_Pages_No_Draw_Fields_Pre();

    /* off page -> visible page; dst is the page Platform_Video_Update() presents */
    src = video_page_buffer[1 - draw_page_num];
    dst = video_page_buffer[draw_page_num];

    for(pass = 4; pass >= 1; pass--)                        /* Mosaic_InitVar 4..1 */
    {
        init64 = (pass - 1) << 6;                           /* Mosaic_Init64Times: 192,128,64,0 */

        for(col_step = 39; col_step >= 0; col_step--)       /* Mosaic_Rnd40_Index 39..0 */
        {
            for(row_step = 49; row_step >= 0; row_step--)   /* Mosaic_Rnd50_Index 49..0 */
            {
                rnd50_row = Mosaic_Rnd50_Tbl[row_step];
                rnd40_col = (rnd50_row + col_step) % 40;
                col_idx = Mosaic_Rnd40_Tbl[rnd40_col];

                bits_index = (uint8_t)(col_idx + rnd50_row + init64);
                x_left = col_idx * 8 + Mosaic_Rnd256_Bits[bits_index];
                x_right = col_idx * 8 + 4 + Mosaic_Rnd256_Bits[(uint8_t)(bits_index + 1)];

                for(band = 0; band < 4; band++)             /* the four 50-row bands */
                {
                    row_offset = (int32_t)(rnd50_row + band * 50) * 320;
                    dst[row_offset + x_left] = src[row_offset + x_left];
                    dst[row_offset + x_right] = src[row_offset + x_right];
                }
            }
        }

        Platform_Video_Update();   /* show this pass's grain accumulating */

        /* CLAUDE: optional pacing so the 4 grain passes are perceptible; 0 = original behavior */
        if(g_pageflip_dissolve_pass_delay > 0)
        {
            Platform_Sleep_Millies(g_pageflip_dissolve_pass_delay);
        }
    }

    Toggle_Pages_No_Draw_Fields();

}

// WZD s26p02
// UU_VGA_SlowMosaicFlip()

// WZD s26p03
// RP_VGA_CutRight()

// WZD s26p04
// UU_VGA_InterleaveOut()

// WZD s26p05
// UU_VGA_InterleaveIn()

// WZD s26p06
// UU_VGA_CrossSlideOut()

// WZD s26p07
// UU_VGA_CrossSlideIn()

// WZD s26p08
// UU_VGA_SplitCutOut()

// WZD s26p09
// UU_VGA_CopyLine()

// WZD s26p10
/*
prepares for a GUI_PageFlip by saving the data segment and the cursor redraw state, then zeroing out the latter
*/
void Toggle_Pages_No_Draw_Fields_Pre(void)
{

// mov     ax, ds
// mov     [GUI_DS_Save], ax

    Save_Mouse_State();

}


// WZD s26p11
/*
exchanges the current draw and display frames by resetting the VGA read start address, and swaps cursors between the two images
requires a prior call to Toggle_Pages_No_Draw_Fields_Pre

same as Toggle_Pages(), except doesn't call Draw_Fields()

GUI_PageFlip()
Toggle_Pages_No_Draw_Fields();
*/
void Toggle_Pages_No_Draw_Fields(void)
{
    int16_t mx;  // DNE in Dasm
    int16_t my;  // DNE in Dasm

// mov     ax, seg dseg
// mov     ds, ax
// mov     ax, [GUI_DS_Save]
// mov     ds, ax
    
    mx = Pointer_X();
    my = Pointer_Y();

    Check_Mouse_Shape(mx, my);
    Save_Mouse_Off_Page(mx, my);
    Draw_Mouse_Off_Page(mx, my);

    Page_Flip();

    Restore_Mouse_Off_Page();
    Copy_Mouse_Off_To_Mouse_Back();
    Restore_Mouse_State();

}


// WZD s26p12
// UU_VGA_ColWipeLeft()

// WZD s26p13
// UU_VGA_ColWipeRight()

// WZD s26p14
// drake178: VGA_ShiftCopyFromF3()
/* GEMINI */
void VGA_ShiftCopyFromF3(int16_t x, int16_t y)
{
    int row;
    
    /* 1. Shifted Copy */
    /* Copy row by row, skipping the bottom-most 'y' rows */
    for (row = 0; row < SCREEN_HEIGHT - y; row++) {
        /* dest: current_page + offset row + offset x */
        /* src:  page2 + normal row */
        memcpy(current_video_page + ((row + y) * SCREEN_WIDTH) + x,
               video_page_buffer[2] + (row * SCREEN_WIDTH),
               SCREEN_WIDTH - x);
    }

    /* 2. Gap Cleanup */
    /* If you have a custom Fill() function, use it here. Otherwise: */
    
    if (x > 0) {
        /* Black out the left edge */
        for (row = 0; row < SCREEN_HEIGHT; row++) {
            memset(current_video_page + (row * SCREEN_WIDTH), 0, x);
        }
    }

    if (y > 0) {
        /* Black out the top edge */
        memset(current_video_page, 0, SCREEN_WIDTH * y);
    }
}


/*
    WIZARDS.EXE  seg027
*/

// WZD s27p01
// drake178: VGA_PartCopyFromF3()
/*
; copies the contents of the 3rd VGA frame ($A800) into
; the current draw frame with the top teft of the
; saved screen matching the specified coordinates, and
; clears any borders created at the top or left side
*/
void VGA_PartCopyFromF3(int16_t x, int16_t y)
{

/*
; copies the contents of the 3rd VGA frame ($A800) into
; the current draw frame starting at the specified
; location
*/
    VGA_ShiftCopyFromF3(x,y);

    /* cleanup tearing */
//     if(x != 0)
//     {
//         Fill(SCREEN_XMIN, SCREEN_YMIN, (x - 1), SCREEN_YMAX, ST_TRANSPARENT);
//     }
// 
//     if(y != 0)
//     {
//         Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, (y - 1), ST_TRANSPARENT);
//     }

}


// WZD s27p02
// UU_VGA_SliceFlip()

// WZD s27p03
// UU_VGA_SquaresFlip()

// WZD s27p04
// UU_VGA_CopyRect()

// WZD s27p05
// UU_VGA_InterleavedFlip()

// WZD s27p06
// UU_VGA_CrossSlideFlip()

// WZD s27p07
// UU_VGA_WipeLeft()

// WZD s27p08
// UU_VGA_WipeRight()



/*
    WIZARDS.EXE  seg028
*/

// Set_Page_On()
// Set_Page_Off()
// Check_Default_Video_Page()
// Page_Flip()
// Copy_On_To_Off_Page()
// Copy_On_To_Off_YM()
// Copy_On_To_Off_NM()
// Copy_Off_To_Back()
// Copy_Back_To_Off()
// Copy_Off_To_Page4()
// Copy_Page4_To_Off()
// UU_VGA_CopyToDraw()


// WZD s28p01
void Set_Page_On(void)
{

    /*
        ~== VRAM + ((draw_page_num) * 4)  {0xA000, 0xA400}
    */

    current_video_page = video_page_buffer[(draw_page_num)];

    // // current_page_flag = 0;
    // current_video_page = off_page_buffer;
    // // copy_to_on_page_flag = 1;

}


// WZD s28p02
void Set_Page_Off(void)
{

    /*
        ~== VRAM + ((1 - draw_page_num) * 4)  {0xA000, 0xA400}
    */

    current_video_page = video_page_buffer[(1 - draw_page_num)];

    // // current_page_flag = 1;
    // current_video_page = off_page_buffer;
    // // copy_to_on_page_flag = 0;

}


// WZD s28p03
void Check_Default_Video_Page(void)
{

}



// WZD s28p04
/*
    switch on/off video memory page
    ...update what is displayer to the user

MS-DOS/IBM-PC: next hardware video refresh reads from different hardware video memory
MS Win: next WM_PAINT event, blit different video back buffer  (manually generate a WM_PAINT event)
SDL2: manually or next SDL_WINDOWEVENT_EXPOSED, blit, copy, and render different video back buffer

*/
void Page_Flip(void)
{

    // MS-DOS & IBM-PC VGA
        // CRT Controller - Address
        // VGA_WaitSync();
    // MS Windows
        // pointer to buffer to render/present
    draw_page_num = (1 - draw_page_num);  // NOTE: this is the only code that changes 'draw_page_num'
    // 1oom/Kilgore  draw_page_num ^= 1;
    draw_page = video_page_buffer[draw_page_num];
    // DOMSDOS  VGA_WaitSync()
    Set_Page_Off();
    // HERE: what was drawn should now be displayed
#ifdef _STU_DOS

#endif
#ifdef _STU_LIN

#endif
#ifdef _STU_MAC

#endif
// #ifdef _STU_SDL2
    Platform_Palette_Update();
    Platform_Video_Update();
// #endif
#ifdef _STU_WIN
    Pump_Paints();
#endif


}



// WZD s28p05
// MoO2  Copy_Off_To_On_Page()
void Copy_On_To_Off_Page(void)
{

    if(Get_Pointer_Image_Number() != crsr_None)
    {
        Copy_On_To_Off_YM();
    }
    else
    {
        Copy_On_To_Off_NM();
    }

}

// WZD s28p06
/*
    ¿¿¿
        copy 4000 bytes at time, so 1/4 memory plane per itr?
        ¿ related to width of cursor image ?


    ???
*/
void Copy_On_To_Off_YM(void)
{
    int16_t mouse_x;
    int16_t mouse_y;
    uint8_t * src;
    uint8_t * dst;
    int16_t itr;
    int16_t itr_rep_movsb;
    int16_t tmp_mouse_x;
    int16_t tmp_mouse_y;

    Save_Mouse_State();

    mouse_x = Pointer_X();
    mouse_y = Pointer_Y();


    src = (uint8_t *)video_page_buffer[draw_page_num];
    dst = (uint8_t *)video_page_buffer[1 - draw_page_num];


    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / (screen_pixel_size / 4)))
    {
        itr_rep_movsb = 0;
        while(itr_rep_movsb++ < (screen_pixel_size / 4))
        {
            *dst++ = *src++;
        }

        Restore_Mouse_Off_Page();

        tmp_mouse_x = Pointer_X();
        tmp_mouse_y = Pointer_Y();
        if( (tmp_mouse_x != mouse_x) || (tmp_mouse_y != mouse_y) )
        {
            Check_Mouse_Shape(tmp_mouse_x, tmp_mouse_y);
            Restore_Mouse_On_Page();
            Save_Mouse_On_Page(tmp_mouse_x, tmp_mouse_y);
            Check_Mouse_Shape(tmp_mouse_x, tmp_mouse_y);
            Draw_Mouse_On_Page(tmp_mouse_x, tmp_mouse_y);
            Set_Pointer_Position(tmp_mouse_x, tmp_mouse_y);
        }

    }

    Restore_Mouse_Off_Page();

    Restore_Mouse_State();

}

// WZD s28p07
// TODO  this should probably just be Copy_On_To_Off(), since it's just like the other Copy_On/Offs
void Copy_On_To_Off_NM(void)
{
    uint8_t * src;
    uint8_t * dst;
    uint16_t itr;

    Save_Mouse_State();

    src = (uint8_t *)video_page_buffer[draw_page_num];
    dst = (uint8_t *)video_page_buffer[1 - draw_page_num];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < screen_pixel_size)
    {
        *dst++ = *src++;
    }

    Restore_Mouse_State();
}

// WZD s28p08
// copy VGA RAM 0xA000 to VGA RAM 0xA800 - frame-buffer 0 to frame-buffer 2
void Copy_Off_To_Back(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[1 - draw_page_num];
    dst = (uint16_t *)video_page_buffer[2];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }
}

// WZD s28p09
// copy VGA RAM 0xA800 to VGA RAM 0xA000 - frame-buffer 2 to frame-buffer 0
void Copy_Back_To_Off(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[2];
    dst = (uint16_t *)video_page_buffer[1 - draw_page_num];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }
}

// WZD s28p10
// drake178: VGA_SaveDraw_Frame4()
// copy VGA RAM 0xA000 to VGA RAM 0xAC00 - frame-buffer 0 to frame-buffer 3
void Copy_Off_To_Page4(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[1 - draw_page_num];
    dst = (uint16_t *)video_page_buffer[3];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }

}

// WZD s28p11
// drake178: VGA_LoadDraw_Frame4()
// copy VGA RAM 0xAC00 to VGA RAM 0xA000 - frame-buffer 3 to frame-buffer 0
void Copy_Page4_To_Off(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[3];
    dst = (uint16_t *)video_page_buffer[1 - draw_page_num];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }
}
