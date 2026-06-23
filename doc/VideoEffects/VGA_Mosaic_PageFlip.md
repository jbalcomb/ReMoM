VGA_Mosaic_PageFlip.md

OVL_MosaicFlip()  ==>  Dissolve_Main_Screen()


mosaic
dissolve
"grain mapping"




## PageFlip_Dissolve

PageFlip_Dissolve()
    |-> Toggle_Pages_No_Draw_Fields_Pre()
    ...
    |-> Toggle_Pages_No_Draw_Fields()

## PageFlip_FX
    switch(PageFlipEffect)
    1: RP_VGA_CutRight()  ...GUI_PageFlipPrep(); GUI_PageFlip();
    3: PageFlip_Dissolve()
    4: RP_VGA_GrowOutFlip()  ... Toggle_Pages()

MGC
    PageFlip_Dissolve
        PageFlip_FX+2C             call    PageFlip_Dissolve
        Chancellor_Screen__WIP+324 call    PageFlip_Dissolve
        Hire_Hero_Popup+2C0        call    PageFlip_Dissolve
        Dissolve_Main_Screen+12    call    PageFlip_Dissolve

    Dissolve_Main_Screen()
        j_PageFlip_Dissolve     jmp     Dissolve_Main_Screen         
        Spellbook_Screen+833       call    near ptr Dissolve_Main_Screen
        Cast_Spell_Overland_Do+457 call    near ptr Dissolve_Main_Screen
        Cast_Spell_Overland_Do+495 call    near ptr Dissolve_Main_Screen

    j_PageFlip_Dissolve()
        Show_Event_Message:loc_6C6A2                     call    j_PageFlip_Dissolve
        Hero_LevelUp_Popup+30C                           call    j_PageFlip_Dissolve
        Player_Process_Item_Pool:loc_87AE6               call    j_PageFlip_Dissolve
        Player_Process_Item_Pool+5C                      call    j_PageFlip_Dissolve
        Merchant_Popup+105                               call    j_PageFlip_Dissolve
        Cast_Earth_Lore+15F                              call    j_PageFlip_Dissolve
        Cast_Transmute:loc_B0BEE                         call    j_PageFlip_Dissolve
        Cast_Enchant_Road:loc_B1460                      call    j_PageFlip_Dissolve
        Spell_Target_Global_Enchantment_Screen:@@Done    call    j_PageFlip_Dissolve
        IDK_SummonAnim+110                               call    j_PageFlip_Dissolve
        Cast_Spell_City_Enchantment_Animation_2__WIP+185 call    j_PageFlip_Dissolve
        WIZ_GlobalSpellAnim+1CA                          call    j_PageFlip_Dissolve
        Target_Wizard_Screen:loc_C0676                   call    j_PageFlip_Dissolve





/* GEMINI *?
You have uncovered the legendary SimTex "Mosaic Fade" (or dissolve) transition!

When you open a new window or switch to a new view, the screen doesn't just cut to black; it pixelates into "static" and dissolves into the new image.

The raw assembly looks incredibly complex because it is fighting the 16-bit hardware.
Here is exactly what those magic tables are doing, how the assembly works, and how to write a perfectly clean modern C/SDL2 equivalent.

The Secret of the Magic Tables
At the top of the file, you see three arrays of hardcoded numbers:
Mosaic_Rnd50_Tbl: A shuffled array of numbers from 0 to 49.
Mosaic_Rnd40_Tbl: A shuffled array of numbers from 0 to 39.
Mosaic_Rnd256_Bits: A massive array of 0s, 1s, 2s, and 3s.

The Grid:
The developers conceptually divided the 320x200 VGA screen into a grid.
320 pixels wide / 40 columns = 8 pixels wide.
200 pixels tall / 50 rows = 4 pixels tall.
This created a grid of 2,000 tiny 8x4 "blocks".

Instead of drawing the screen top-to-bottom, the engine loops through the Rnd50 and Rnd40 tables to pick grid coordinates in a completely scattered, pseudo-random order.

The 4 Passes (Mosaic_InitVar):
Because drawing chunky 8x4 blocks looked too blocky, the engine runs the entire loop 4 times (Mosaic_InitVar counting down from 4). On each pass, it uses the Rnd256_Bits table to randomly pick one of the four Mode X VGA hardware memory planes, writing just a fraction of the pixels in that block. This creates the beautiful "TV Static" grain effect.

The Modern SDL2 Translation
In modern SDL2, we completely ignore the hardware memory planes and the 4-pass loop. Modern GPUs don't allow you to write 4 pixels to the screen and pause; they draw entire textures at once.

To recreate this effect in SDL2, we can just copy those 8x4 blocks in the exact pseudo-random order the original programmers dictated.

Here is the modern C equivalent using standard SDL functions:

Why this is better for a modern port
In the original MS-DOS code, the movsb instructions pushed individual bytes directly to the video card's physical RAM, which was slow enough to naturally act as a visual transition.

By using SDL_RenderCopy with small SDL_Rect targets, you leverage the modern GPU to handle the copying instantly, while controlling the "fade" speed via standard OS timers (SDL_Delay). You keep the exact visual feel of the 1990s without getting trapped in the convoluted math of hardware memory planes!





1EDD:000E                                                 ; Segment type: Pure code
1EDD:000E                                                 segment seg026 byte public 'CODE' use16
1EDD:000E                                                     assume cs:seg026
1EDD:000E                                                     ;org 0Eh
1EDD:000E                                                     assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing
1EDD:000E 02 02 02 01 01 03 00 03 03 02 00 01 03 03 01 03+Mosaic_Rnd256_Bits db 2, 2, 2, 1, 1, 3, 0, 3, 3, 2, 0, 1, 3, 3, 1, 3, 3, 0, 3, 1, 2, 3, 0, 2, 1, 1, 2, 2, 1, 1, 3, 3, 2, 3, 3, 0, 2, 3, 0, 2, 0, 3, 2, 0, 2, 0, 0, 3, 1, 0, 1, 1, 1, 3, 3, 1, 1, 0, 0, 2, 1, 1, 2, 1, 0, 0, 0, 3, 3, 1, 1, 2, 2, 1, 2, 2, 0, 2, 3, 1, 2, 2, 2, 0
1EDD:000E 03 00 03 01 02 03 00 02 01 01 02 02 01 01 03 03+                                        ; DATA XREF: PageFlip_Dissolve+CCr ...
1EDD:000E 02 03 03 00 02 03 00 02 00 03 02 00 02 00 00 03+    db 0, 0, 1, 1, 0, 2, 0, 3, 2, 2, 1, 0, 3, 1, 2, 3, 0, 0, 1, 0, 2, 2, 3, 2, 1, 1, 2, 2, 0, 3, 0, 0, 2, 0, 0, 3, 3, 1, 2, 1, 0, 0, 1, 3, 1, 1, 1, 0, 0, 2, 3, 1, 0, 3, 3, 0, 1, 0, 0, 0, 1, 3, 1, 3, 3, 2, 2, 3, 2, 0, 1, 1, 0, 3, 0, 2, 0, 2, 1, 1, 3, 1, 2, 1
1EDD:000E 01 00 01 01 01 03 03 01 01 00 00 02 01 01 02 01+    db 3, 1, 0, 1, 3, 3, 1, 1, 3, 2, 3, 2, 3, 1, 1, 2, 0, 2, 3, 3, 2, 2, 0, 2, 3, 3, 3, 2, 2, 0, 2, 0, 1, 0, 1, 3, 2, 1, 2, 2, 0, 1, 0, 2, 1, 1, 3, 0, 3, 3, 3, 0, 3, 0, 2, 1, 1, 0, 0, 2, 1, 2, 3, 3, 1, 0, 1, 3, 0, 2, 3, 0, 2, 1, 2, 3, 0, 2, 2, 0, 2, 3, 1, 0
1EDD:000E 00 00 00 03 03 01 01 02 02 01 02 02 00 02 03 01+    db 3, 3, 3, 0
1EDD:010E 00 03 1E 12 1C 16 1D 0C 22 2F 1F 20 07 31 2E 0E+Mosaic_Rnd50_Tbl db 0, 3, 1Eh, 12h, 1Ch, 16h, 1Dh, 0Ch, 22h, 2Fh, 1Fh, 20h, 7, 31h, 2Eh, 0Eh, 26h, 2Bh, 23h, 28h, 0Bh, 9, 24h, 4, 21h, 1Ah, 27h, 0Fh, 5, 15h, 2, 10h, 19h, 0Ah, 30h, 0Dh, 18h, 25h, 11h, 13h, 2Ch, 6, 8, 14h, 1, 17h, 29h, 1Bh, 2Dh, 2Ah
1EDD:010E 26 2B 23 28 0B 09 24 04 21 1A 27 0F 05 15 02 10+                                        ; DATA XREF: PageFlip_Dissolve+7Er ...
1EDD:0140 02 09 1E 26 0D 1B 1F 01 13 08 18 24 25 1A 00 0B+Mosaic_Rnd40_Tbl db 2, 9, 1Eh, 26h, 0Dh, 1Bh, 1Fh, 1, 13h, 8, 18h, 24h, 25h, 1Ah, 0, 0Bh, 17h, 20h, 16h, 0Ch, 10h, 14h, 1Dh, 12h, 1Ch, 7, 0Ah, 23h, 0Eh, 19h, 5, 21h, 0Fh, 15h, 4, 11h, 6, 3, 27h, 22h
1EDD:0140 17 20 16 0C 10 14 1D 12 1C 07 0A 23 0E 19 05 21+                                        ; DATA XREF: PageFlip_Dissolve+B0r
1EDD:0168 00                                                  db    0
1EDD:0169 00                                                  db    0
1EDD:016A 00                                                  db    0
1EDD:016B 00                                                  db    0
1EDD:016C 00 00                                           Mosaic_InitVar dw 0                     ; DATA XREF: PageFlip_Dissolve+49w ...
1EDD:016E 00 00                                           Mosaic_Rnd40_Index dw 0                 ; DATA XREF: PageFlip_Dissolve+6Bw ...
1EDD:0170 00 00                                           Mosaic_Init64Times dw 0                 ; DATA XREF: PageFlip_Dissolve+67w ...
1EDD:0172 00 00                                           Mosaic_Rnd50_Index dw 0                 ; DATA XREF: PageFlip_Dissolve:loc_1EFD6w ...
1EDD:0174 00 00                                           VGA_ScanLine_Offset dw 0                ; DATA XREF: RP_VGA_CutRight+3Bw ...
1EDD:0176 00 00                                           VGA_Write_Map dw 0                      ; DATA XREF: RP_VGA_CutRight:loc_1F2D5w ...
1EDD:0178 00 00                                           VGA_Read_Map dw 0                       ; DATA XREF: RP_VGA_CutRight+49w ...
1EDD:017A 00 00                                           VGA_SlideInColumn dw 0                  ; DATA XREF: UU_VGA_InterleaveIn+10w ...
1EDD:017C 00 00                                           VGA_Line_Count dw 0                     ; DATA XREF: UU_VGA_CrossSlideOut+14w ...
1EDD:017E 00 00                                           VGA_Start_Offset dw 0                   ; DATA XREF: UU_VGA_CrossSlideOut+Dw ...
1EDD:0180 00 00                                               db 2 dup(0)
1EDD:0182 00 00                                           VGA_Horz_Slope dw 0                     ; DATA XREF: UU_VGA_CopyLine:loc_1F777w ...
1EDD:0184 00 00                                           VGA_Vert_Slope dw 0                     ; DATA XREF: UU_VGA_CopyLine+75w ...
1EDD:0186 00 00                                           VGA_Y_Dir dw 0                          ; DATA XREF: UU_VGA_CopyLine+59w ...
1EDD:0188 00 00                                           VGA_Line_Length dw 0                    ; DATA XREF: UU_VGA_CopyLine+70w ...
1EDD:018A 00 00                                           VGA_Display_Seg dw 0                    ; DATA XREF: PageFlip_Dissolve+31w ...
1EDD:018C 00 00                                           VGA_Draw_Seg dw 0                       ; DATA XREF: PageFlip_Dissolve+3Ew ...
1EDD:018E 00 00                                           VGA_Read_Mask dw 0                      ; DATA XREF: UU_VGA_CopyLine+102w ...
1EDD:0190 00 00                                           VGA_Line_Offset dw 0                    ; DATA XREF: VGA_ShiftCopyFromF3+27w ...
1EDD:0192 00 00                                           VGA_Origin_Offset dw 0                  ; DATA XREF: VGA_ShiftCopyFromF3+36w ...
1EDD:0194                                                 ; =============== S U B R O U T I N E =======================================
1EDD:0194                                                 ; mosaic-style screen transition effect with a page
1EDD:0194                                                 ; flip at the end, after replacing 3/4ths of the screen
1EDD:0194                                                 ; Attributes: bp-based frame
1EDD:0194                                                 ; int __far PageFlip_Dissolve(void)
1EDD:0194                                                 proc PageFlip_Dissolve far           ; CODE XREF: PageFlip_FX+2CP ...
1EDD:0194                                                 UU_y2= word ptr -12h
1EDD:0194                                                 UU_x2= word ptr -10h
1EDD:0194                                                 UU_y1= word ptr -0Eh
1EDD:0194                                                 UU_x1= word ptr -0Ch
1EDD:0194 55                                                  push bp
1EDD:0195 8B EC                                               mov bp, sp
1EDD:0197 83 EC 12                                            sub sp, 12h
1EDD:019A 06                                                  push es
1EDD:019B 57                                                  push di
1EDD:019C 56                                                  push si
1EDD:019D 1E                                                  push ds
1EDD:019D
1EDD:019D
1EDD:019E 0E                                                  push cs
1EDD:019F E8 1F 09                                            call near ptr Toggle_Pages_No_Draw_Fields_Pre
1EDD:01A2 90                                                  nop
1EDD:01A3 C7 46 F4 00 00                                      mov [bp+UU_x1], 0
1EDD:01A8 C7 46 F0 3F 01                                      mov [bp+UU_x2], 319
1EDD:01AD C7 46 F2 00 00                                      mov [bp+UU_y1], 0
1EDD:01B2 C7 46 EE 31 00                                      mov [bp+UU_y2], 49
1EDD:01B2
1EDD:01B2
1EDD:01B7 A1 EE 76                                            mov ax, [draw_page_num]
1EDD:01BA 8A E0                                               mov ah, al
1EDD:01BC 32 C0                                               xor al, al
1EDD:01BE D1 E0                                               shl ax, 1
1EDD:01C0 D1 E0                                               shl ax, 1
1EDD:01C2 05 00 A0                                            add ax, 0A000h
1EDD:01C5 2E A3 8A 01                                         mov [cs:VGA_Display_Seg], ax
1EDD:01C5
1EDD:01C5
1EDD:01C9 BB 00 A4                                            mov bx, 0A400h
1EDD:01CC 2B D8                                               sub bx, ax
1EDD:01CE 81 C3 00 A0                                         add bx, 0A000h
1EDD:01D2 2E 89 1E 8C 01                                      mov [cs:VGA_Draw_Seg], bx
1EDD:01D2
1EDD:01D2
1EDD:01D7 2E A1 8A 01                                         mov ax, [cs:VGA_Display_Seg]
1EDD:01DB 8E C0                                               mov es, ax
1EDD:01DB
1EDD:01DB
1EDD:01DD 2E C7 06 6C 01 04 00                                mov [cs:Mosaic_InitVar], 4
1EDD:01DD
1EDD:01DD
1EDD:01E4 2E A1 8C 01                                         mov ax, [cs:VGA_Draw_Seg]
1EDD:01E8 8E D8                                               mov ds, ax
1EDD:01EA                                                 loc_1EFBA:                              ; CODE XREF: PageFlip_Dissolve+177j
1EDD:01EA 2E A1 6C 01                                         mov ax, [cs:Mosaic_InitVar]
1EDD:01EE 48                                                  dec ax
1EDD:01EF D1 E0                                               shl ax, 1
1EDD:01F1 D1 E0                                               shl ax, 1
1EDD:01F3 D1 E0                                               shl ax, 1
1EDD:01F5 D1 E0                                               shl ax, 1
1EDD:01F7 D1 E0                                               shl ax, 1
1EDD:01F9 D1 E0                                               shl ax, 1
1EDD:01FB 2E A3 70 01                                         mov [cs:Mosaic_Init64Times], ax
1EDD:01FB
1EDD:01FB
1EDD:01FF 2E C7 06 6E 01 27 00                                mov [cs:Mosaic_Rnd40_Index], 39
1EDD:0206                                                 loc_1EFD6:                              ; CODE XREF: PageFlip_Dissolve+16Dj
1EDD:0206 2E C7 06 72 01 31 00                                mov [cs:Mosaic_Rnd50_Index], 49
1EDD:020D                                                 loc_1EFDD:                              ; CODE XREF: PageFlip_Dissolve+163j
1EDD:020D 2E 8B 1E 72 01                                      mov bx, [cs:Mosaic_Rnd50_Index]
1EDD:0212 2E 8A 9F 0E 01                                      mov bl, [cs:Mosaic_Rnd50_Tbl+bx]
1EDD:0217 32 FF                                               xor bh, bh
1EDD:0219 8B CB                                               mov cx, bx
1EDD:021B 8B D3                                               mov dx, bx
1EDD:021D D1 E3                                               shl bx, 1
1EDD:021F D1 E3                                               shl bx, 1
1EDD:0221 03 D9                                               add bx, cx
1EDD:0223 D1 E3                                               shl bx, 1
1EDD:0225 D1 E3                                               shl bx, 1
1EDD:0227 D1 E3                                               shl bx, 1
1EDD:0229 D1 E3                                               shl bx, 1
1EDD:022B 8B C3                                               mov ax, bx                          ; = bx * 80
1EDD:022D 2E 03 0E 6E 01                                      add cx, [cs:Mosaic_Rnd40_Index]
1EDD:0232 83 F9 28                                            cmp cx, 40
1EDD:0235 78 0B                                               js  short loc_1F012
1EDD:0237 83 E9 28                                            sub cx, 40
1EDD:0237
1EDD:0237
1EDD:023A 83 F9 28                                            cmp cx, 40
1EDD:023D 78 03                                               js  short loc_1F012
1EDD:023F 83 E9 28                                            sub cx, 40
1EDD:0242                                                 loc_1F012:                              ; CODE XREF: PageFlip_Dissolve+A1j ...
1EDD:0242 8B D9                                               mov bx, cx
1EDD:0244 2E 8A 9F 40 01                                      mov bl, [cs:Mosaic_Rnd40_Tbl+bx]
1EDD:0249 32 FF                                               xor bh, bh
1EDD:024B 8B CB                                               mov cx, bx
1EDD:024D D1 E3                                               shl bx, 1
1EDD:024F 03 C3                                               add ax, bx
1EDD:0251 8B F0                                               mov si, ax                          ; rnd(50) * 80 + rnd(40) * 2
1EDD:0253 8B F8                                               mov di, ax
1EDD:0255 03 CA                                               add cx, dx
1EDD:0257 2E 03 0E 70 01                                      add cx, [cs:Mosaic_Init64Times]     ; rnd(50) + rnd(40) + 64*(3..2..1)
1EDD:025C 32 ED                                               xor ch, ch
1EDD:025E 8B D9                                               mov bx, cx
1EDD:0260 2E 8A A7 0E 00                                      mov ah, [cs:Mosaic_Rnd256_Bits+bx]
1EDD:0265 FE C3                                               inc bl
1EDD:0267 BA CE 03                                            mov dx, 3CEh
1EDD:026A B0 04                                               mov al, 4
1EDD:026C EE                                                  out dx, al                          ; EGA: graph 1 and 2 addr reg:
1EDD:026C                                                                                         ; read map select.
1EDD:026C                                                                                         ; Data bits 0-2 select map # for read mode 00.
1EDD:026D 42                                                  inc dx
1EDD:026E 8A C4                                               mov al, ah
1EDD:0270 EE                                                  out dx, al                          ; EGA port: graphics controller data register
1EDD:0271 8A CC                                               mov cl, ah
1EDD:0273 B4 01                                               mov ah, 1
1EDD:0275 80 F9 00                                            cmp cl, 0
1EDD:0278 74 04                                               jz  short loc_1F04E
1EDD:027A                                                 loc_1F04A:                              ; CODE XREF: PageFlip_Dissolve+E8j
1EDD:027A D0 E4                                               shl ah, 1
1EDD:027C E2 FC                                               loop loc_1F04A
1EDD:027E                                                 loc_1F04E:                              ; CODE XREF: PageFlip_Dissolve+E4j
1EDD:027E BA C4 03                                            mov dx, 3C4h
1EDD:0281 B0 02                                               mov al, 2
1EDD:0283 EE                                                  out dx, al                          ; EGA: sequencer address reg
1EDD:0283                                                                                         ; map mask: data bits 0-3 enable writes to bit planes 0-3
1EDD:0284 42                                                  inc dx
1EDD:0285 8A C4                                               mov al, ah
1EDD:0287 EE                                                  out dx, al                          ; EGA port: sequencer data register
1EDD:0288 A4                                                  movsb
1EDD:0289 81 C6 3F 1F                                         add si, 7999                        ; move down half a screen
1EDD:028D 81 C7 3F 1F                                         add di, 7999
1EDD:0291 A4                                                  movsb
1EDD:0292 81 EE A1 0F                                         sub si, 4001                        ; move back a quarter screen
1EDD:0296 81 EF A1 0F                                         sub di, 4001
1EDD:029A A4                                                  movsb
1EDD:029B 81 C6 3F 1F                                         add si, 7999                        ; move down half a screen
1EDD:029F 81 C7 3F 1F                                         add di, 7999
1EDD:02A3 A4                                                  movsb
1EDD:02A4 81 EE E0 2E                                         sub si, 12000                       ; back to the start
1EDD:02A8 81 EF E0 2E                                         sub di, 12000
1EDD:02AC 2E 8A A7 0E 00                                      mov ah, [cs:Mosaic_Rnd256_Bits+bx]
1EDD:02B1 FE C3                                               inc bl
1EDD:02B3 BA CE 03                                            mov dx, 3CEh
1EDD:02B6 B0 04                                               mov al, 4
1EDD:02B8 EE                                                  out dx, al                          ; EGA: graph 1 and 2 addr reg:
1EDD:02B8                                                                                         ; read map select.
1EDD:02B8                                                                                         ; Data bits 0-2 select map # for read mode 00.
1EDD:02B9 42                                                  inc dx
1EDD:02BA 8A C4                                               mov al, ah
1EDD:02BC EE                                                  out dx, al                          ; EGA port: graphics controller data register
1EDD:02BD 8A CC                                               mov cl, ah
1EDD:02BF B4 01                                               mov ah, 1
1EDD:02C1 80 F9 00                                            cmp cl, 0
1EDD:02C4 74 04                                               jz  short loc_1F09A
1EDD:02C6                                                 loc_1F096:                              ; CODE XREF: PageFlip_Dissolve+134j
1EDD:02C6 D0 E4                                               shl ah, 1
1EDD:02C8 E2 FC                                               loop loc_1F096
1EDD:02CA                                                 loc_1F09A:                              ; CODE XREF: PageFlip_Dissolve+130j
1EDD:02CA BA C4 03                                            mov dx, 3C4h
1EDD:02CD B0 02                                               mov al, 2
1EDD:02CF EE                                                  out dx, al                          ; EGA: sequencer address reg
1EDD:02CF                                                                                         ; map mask: data bits 0-3 enable writes to bit planes 0-3
1EDD:02D0 42                                                  inc dx
1EDD:02D1 8A C4                                               mov al, ah
1EDD:02D3 EE                                                  out dx, al                          ; EGA port: sequencer data register
1EDD:02D4 A4                                                  movsb
1EDD:02D5 81 C6 3F 1F                                         add si, 7999                        ; move down half a screen
1EDD:02D9 81 C7 3F 1F                                         add di, 7999
1EDD:02DD A4                                                  movsb
1EDD:02DE 81 EE A1 0F                                         sub si, 4001                        ; move back a quarter screen
1EDD:02E2 81 EF A1 0F                                         sub di, 4001
1EDD:02E6 A4                                                  movsb
1EDD:02E7 81 C6 3F 1F                                         add si, 7999                        ; move down half a screen
1EDD:02EB 81 C7 3F 1F                                         add di, 7999
1EDD:02EF A4                                                  movsb
1EDD:02F0 2E FF 0E 72 01                                      dec [cs:Mosaic_Rnd50_Index]
1EDD:02F5 78 03                                               js  short loc_1F0CA
1EDD:02F7 E9 13 FF                                            jmp loc_1EFDD
1EDD:02FA                                                 ; ---------------------------------------------------------------------------
1EDD:02FA                                                 loc_1F0CA:                              ; CODE XREF: PageFlip_Dissolve+161j
1EDD:02FA 2E FF 0E 6E 01                                      dec [cs:Mosaic_Rnd40_Index]
1EDD:02FF 78 03                                               js  short loc_1F0D4
1EDD:0301 E9 02 FF                                            jmp loc_1EFD6
1EDD:0304                                                 ; ---------------------------------------------------------------------------
1EDD:0304                                                 loc_1F0D4:                              ; CODE XREF: PageFlip_Dissolve+16Bj
1EDD:0304 2E FF 0E 6C 01                                      dec [cs:Mosaic_InitVar]
1EDD:0309 74 03                                               jz  short @@Done
1EDD:030B E9 DC FE                                            jmp loc_1EFBA
1EDD:030E                                                 ; ---------------------------------------------------------------------------
1EDD:030E                                                 @@Done:                                 ; CODE XREF: PageFlip_Dissolve+175j
1EDD:030E 0E                                                  push cs
1EDD:030F E8 BC 07                                            call near ptr Toggle_Pages_No_Draw_Fields
1EDD:030F
1EDD:030F
1EDD:0312 90                                                  nop
1EDD:0312
1EDD:0312
1EDD:0313 1F                                                  pop ds
1EDD:0314 5E                                                  pop si
1EDD:0315 5F                                                  pop di
1EDD:0316 07                                                  pop es
1EDD:0317 8B E5                                               mov sp, bp
1EDD:0319 5D                                                  pop bp
1EDD:031A CB                                                  retf
1EDD:031A                                                 endp PageFlip_Dissolve
