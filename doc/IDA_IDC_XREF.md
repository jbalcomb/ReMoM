


Elsewhere, ...
    seg000:02C6 02 80                                           __MMODEL dw 8002h                       ; far code, near data (medium)
    seg000:02C6                                                                                         ; declared in c0.asm





SEEALSO:  gDrive > SimTexUni > IDA_IDC_XREF.gDoc




## How manage addresses for inclusion or exclusion?
DSEG vs. DATA and BSS  (Initialized and Uninitialized)
extraneous compiler string optimizer cross-references
XREFs from BCPP3 library functions?
XREFs from AIL library functions?

function with tests for each address?
return TRUE for 'exclude'
`stu_exclude_address.idc`

separate functions for each range of addresses?
    pass begin and end


Is there a name for the block between DATA and BSS?
    ¿ "_bdata_start" / "edata start" ?

dseg:009A 54 EA                                           __heapbase dw 0EA54h
dseg:009C 54 EA                                           __brklvl dw 0EA54h                      ; DATA XREF: __brk+10w ...

seg000:02C4 00 00                                           DGROUP@ dw 0     



EA54
59988

8890
34960

59988 - 34960 = 25028  61C4




DSEG
    dseg:0000   294A0   36AA0
    dseg:EA54   37EF4   454F4

DATA
    dseg:0000   294A0   36AA0
    dseg:84A8   31948   3EF48

BSS
    dseg:8890   31D30   3F330
    dseg:EA54   37EF4   454F4


exclude BCPP start of DATA
exclude _AIL_
exclude SOUND
exclude BCPP in-beween
    ?
exclude BCPP end of BSS


DSEG        [0x00036AA0, 0x000454F4)
DATA        [0x00036AA0, 0x0003EF48)
BSS         [0x0003F330 ,0x000454F4)

DSEG        [0x00036AA0, 0x000454F4)
    DATA        [0x00036AA0, 0x0003EF48)
        ...

        ...
        SOUND
            dseg:82A8   31748   3ED48   
            ?
        BCPP_2  31948   3EF48   dseg:84A8

    BSS         [0x0003F330 ,0x000454F4)



dseg:888A 01 01 93 01 43 32                               ExitStart _start_exit_proc <1, 1, __OVREXIT>
dseg:888A                                                                                         ; DATA XREF: __cleanup+7o ...
dseg:888A                                                                                         ; defined as segment _EXIT_ in c0.asm
dseg:8890
dseg:8890
dseg:8890
dseg:8890                                                 ¿ BEGIN: ?
dseg:8890
dseg:8890 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Arcanus CONT_TBL <0>                ; DATA XREF: start+C7o ...
dseg:8890 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; _bdata_start

...
...
...

dseg:E918 00 00                                           SND_TBank_Handle dw 0                   ; DATA XREF: SND_PlayFile+F3w ...
dseg:E918
dseg:E918                                                 END: AIL
dseg:E918
dseg:E91A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+_atexittbl dd 20h dup(         0)                  ; 0

...
...
...

dseg:EA52 00                                              static_uchar_putc db 0                  ; DATA XREF: fputc+Bw ...
dseg:EA52                                                                                         ; declared in putc.c (fputc function)
dseg:EA53 00                                              align 2
dseg:EA53                                                 ends dseg
dseg:EA53
seg162:0000                                                 ; ===========================================================================








dseg:82A6 5F 00                                           GUI_EditCursor db '_',0                 ; DATA XREF: Draw_Fields+117Do
dseg:82A6
dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard
dseg:82A6
dseg:82A6
dseg:82A6
dseg:82A8 00 00                                           SND_MIDI_Drv_Index dw 0                 ; DATA XREF: Audio_Init+15w ...



dseg:848D 43 6F 75 6C 64 20 6E 6F 74 20 6C 6F 61 64 20 73+cnst_SND_Error10 db 'Could not load sound file',0
dseg:848D 6F 75 6E 64 20 66 69 6C 65 00                                                           ; DATA XREF: SND_Error:loc_323FFo
dseg:84A7 00                                              align 2
dseg:84A8 00 00                                           _atexitcnt dw 0                         ; DATA XREF: RU_atexit+3r ...
dseg:84A8                                                                                         ; declared in atexit.c


seg000:00BF FB                                              sti
seg000:00C0 33 C0                                           xor     ax, ax
seg000:00C2 2E 8E 06 C4 02                                  mov     es, [cs:DGROUP@]                ; declared in c0.asm
seg000:00C7 BF 90 88                                        mov     di, offset TBL_Arcanus          ; _bdata_start
seg000:00CA B9 54 EA                                        mov     cx, 0EA54h                      ; edata start
seg000:00CD 2B CF                                           sub     cx, di
seg000:00CF FC                                              cld
seg000:00D0 F3 AA                                           rep stosb










dseg:0000                                                 ; ===========================================================================
dseg:0000
dseg:0000                                                 ; Segment type: Pure data
dseg:0000                                                 segment dseg para public 'DATA' use16
dseg:0000                                                 assume cs:dseg
dseg:0000 00 00 00 00                                     NullPtr_Logic_Var db 4 dup(0)           ; DATA XREF: dseg:IDK_race_name_GTE_16o ...
dseg:0000                                                                                         ; start of c0.asm _DATA

Down o dseg:IDK_race_name_GTE_16 dw offset NullPtr_Logic_Var             ; word ptr IDK_race_name_GTE_16 + (unit_race * 2)
Down o dseg:IDK_race_name_GTE_16 dw offset NullPtr_Logic_Var             ; word ptr IDK_race_name_GTE_16 + (unit_race * 2)
Down o dseg:IDK_race_name_GTE_16 dw offset NullPtr_Logic_Var             ; word ptr IDK_race_name_GTE_16 + (unit_race * 2)


Down r ArmyList_Screen_Draw+589       push    [IDK_race_name_GTE_16+bx]               
Down r Unit_Statistics_Popup_Draw+453 push    [IDK_race_name_GTE_16+bx]       ; string
Down r Unit_Statistics_Popup_Draw+475 push    [IDK_race_name_GTE_16+bx]               

21 DW
42 bytes

dseg:1EBA 00 00 00 00 D8 26 04 00 0B 00 11 00 22 00 0D 00+IDK_race_name_GTE_16 dw offset NullPtr_Logic_Var
dseg:1EBA 00 00 00 00 00 00 03 00 FE FF 02 00 89 20 DE 26+                                        ; DATA XREF: ArmyList_Screen_Draw+589r ...
dseg:1EBA E5 26 EC 26 F4 26 9C 21 FA 26                   dw offset NullPtr_Logic_Var             ; word ptr IDK_race_name_GTE_16 + (unit_race * 2)
dseg:1EBA                                                 dw offset cnst_Troll
dseg:1EBA                                                 dw offset Copyright_Msg
dseg:1EBA                                                 dw offset Copyright_Msg+7
dseg:1EBA                                                 dw offset Copyright_Msg+0Dh
dseg:1EBA                                                 dw offset Copyright_Msg+1Eh
dseg:1EBA                                                 dw offset Copyright_Msg+9
dseg:1EBA                                                 dw offset NullPtr_Logic_Var
dseg:1EBA                                                 dw offset NullPtr_Logic_Var
dseg:1EBA                                                 dw offset NullPtr_Logic_Var
dseg:1EBA                                                 dw offset NullPtr_Logic_Var+3
dseg:1EBA                                                 dw 0FFFEh
dseg:1EBA                                                 dw offset NullPtr_Logic_Var+2
dseg:1EBA                                                 dw offset cnst_ZeroString_3
dseg:1EBA                                                 dw offset cnst_Rlm5_Arcane
dseg:1EBA                                                 dw offset cnst_Rlm0_Nature
dseg:1EBA                                                 dw offset cnst_Rlm1_Sorcery
dseg:1EBA                                                 dw offset cnst_Rlm2_Chaos
dseg:1EBA                                                 dw offset cnst_Rlm3_Life
dseg:1EBA                                                 dw offset cnst_Rlm4_Death

dseg:1EBA 00 00 00 00 D8 26 04 00 0B 00 11 00 22 00 0D 00+IDK_race_name_GTE_16 dw 0               ; DATA XREF: ArmyList_Screen_Draw+589r ...
dseg:1EBA 00 00 00 00 00 00 03 00 FE FF 02 00 89 20 DE 26+dw 0                                    ; word ptr IDK_race_name_GTE_16 + (unit_race * 2)
dseg:1EBA E5 26 EC 26 F4 26 9C 21 FA 26                   dw 26D8h
dseg:1EBA                                                 dw 4
dseg:1EBA                                                 dw 0Bh
dseg:1EBA                                                 dw 11h
dseg:1EBA                                                 dw 22h
dseg:1EBA                                                 dw 0Dh
dseg:1EBA                                                 dw 0
dseg:1EBA                                                 dw 0
dseg:1EBA                                                 dw 0
dseg:1EBA                                                 dw 3
dseg:1EBA                                                 dw 0FFFEh
dseg:1EBA                                                 dw 2
dseg:1EBA                                                 dw 2089h
dseg:1EBA                                                 dw 26DEh
dseg:1EBA                                                 dw 26E5h
dseg:1EBA                                                 dw 26ECh
dseg:1EBA                                                 dw 26F4h
dseg:1EBA                                                 dw 219Ch
dseg:1EBA                                                 dw 26FAh









Up o __checknull+1F mov     dx, offset NullCheck_Msg        ; "Null pointer assignment\r\n"

dseg:0000                                                 ; ===========================================================================
dseg:0000
dseg:0000                                                 ; Segment type: Pure data
dseg:0000                                                 segment dseg para public 'DATA' use16
dseg:0000                                                 assume cs:dseg
dseg:0000 00 00 00 00                                     NullPtr_Logic_Var db 4 dup(0)           ; start of c0.asm _DATA
dseg:0004 42 6F 72 6C 61 6E 64 20 43 2B 2B 20 2D 20 43 6F+Copyright_Msg db 'Borland C++ - Copyright 1991 Borland Intl.',0
dseg:002F 4E 75 6C 6C 20 70 6F 69 6E 74 65 72 20 61 73 73+NullCheck_Msg db 'Null pointer assignment',0Dh,0Ah
dseg:002F 69 67 6E 6D 65 6E 74 0D 0A                                                              ; DATA XREF: __checknull+1Fo
dseg:0048 44 69 76 69 64 65 20 65 72 72 6F 72 0D 0A       ZeroDiv_Msg db 'Divide error',0Dh,0Ah   ; DATA XREF: ZeroDivision:loc_101AAo
dseg:0056 41 62 6E 6F 72 6D 61 6C 20 70 72 6F 67 72 61 6D+Abort_Msg db 'Abnormal program termination',0Dh,0Ah
dseg:0056 20 74 65 72 6D 69 6E 61 74 69 6F 6E 0D 0A                                               ; DATA XREF: _abort+3o
dseg:0074 00 00 00 00                                     _Int0Vector dd 0                        ; DATA XREF: SaveVectors:loc_101B6w ...
dseg:0078 00 00 00 00                                     _Int4Vector dd 0                        ; DATA XREF: SaveVectors+13w ...
dseg:007C 00 00 00 00                                     _Int5Vector dd 0                        ; DATA XREF: SaveVectors+20w ...
dseg:0080 00 00 00 00                                     _Int6Vector dd 0                        ; DATA XREF: SaveVectors+2Dw ...
dseg:0084 00 00                                           __C0argc dw 0                           ; DATA XREF: start:loc_10154r
dseg:0086 00 00                                           __C0argv dw 0                           ; DATA XREF: start+150r
dseg:0088 00 00                                           __C0environ dw 0                        ; DATA XREF: start+14Cr ...
dseg:008A 00 00                                           _envLng dw 0                            ; DATA XREF: start+46w ...
dseg:008C 00 00                                           _envseg dw 0                            ; DATA XREF: start:loc_1001Dw ...
dseg:008E 00 00                                           _envSize dw 0                           ; DATA XREF: start:loc_10055w ...
dseg:0090 00 00                                           _psp dw 0                               ; DATA XREF: start:loc_10019w ...
dseg:0092 00 00                                           _version dw 0                           ; DATA XREF: start:loc_10016w ...
dseg:0094 00 00                                           errno dw 0                              ; DATA XREF: __IOerror:ser_endw ...
dseg:0096 00 00 00 00                                     _StartTime dd 0                         ; DATA XREF: start+124w ...
dseg:009A 54 EA                                           __heapbase dw 0EA54h
dseg:009C 54 EA                                           __brklvl dw 0EA54h                      ; DATA XREF: __brk+10w ...
dseg:009E 00 00 00 00                                     _heapbase dd 0                          ; DATA XREF: _brk+7r ...
dseg:00A2 00 00 00 00                                     _brklvl dd 0                            ; DATA XREF: normalize+22w ...
dseg:00A6 00 00 00 00                                     _heaptop dd 0                           ; DATA XREF: normalize+69w ...
dseg:00A6                                                                                         ; end of c0.asm _DATA
dseg:00AA
dseg:00AA
dseg:00AA                                                 BEGIN: Data Segment - Initialized Data
dseg:00AA
dseg:00AA







Down r Draw_Unit_StatFig+BF   mov     al, [COL_Banners_P0+bx]         ; AL = *(COL_Banners + (banner_idx * 5) + itr_color_remap)
Down r Draw_Unit_StatFig+156  mov     al, [COL_Banners_P0+bx]                                                                   
Down r Draw_Unit_Figure+D1    mov     al, [COL_Banners_P0+bx]         ; ¿ looks like a 2D array ?                               
Down r LBX_IMG_BannerPaint+29 mov     al, [COL_Banners_P0+bx]                                                                   
Down o Draw_Map_Towers+100    add     ax, offset COL_Banners_P0       ; BUG: parameter mismatch, passing a pointer instead      


IDC Refs:
ordinary code or data to from
ordinary code to from
ordinary data to from

addr_has_xref                       1   Rfirst(); RfirstB(); Dfirst(); DfirstB();
addr_has_xref_code                  0   Rfirst(); RfirstB();
addr_has_xref_code_nontrival        0   Rfirst0(); RfirstB0();
addr_has_xref_code_from             0   Rfirst();
addr_has_xref_code_from_nontrival   0   Rfirst0();
addr_has_xref_code_to               0   RfirstB();
addr_has_xref_code_to_nontrival     0   RfirstB0();
addr_has_xef_data                   1   Dfirst(); DfirstB();
addr_has_xef_data_from              0   Dfirst();
addr_has_xef_data_to                1   DfirstB();

0x00036B4A, COL_Banners_P0, COL_Banners_P0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1
0x00036B4B, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B4C, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B4D, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B4E, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B4F, COL_Banners_P1, COL_Banners_P1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B50, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B51, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B52, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B53, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B54, COL_Banners_P2, COL_Banners_P2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B55, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B56, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B57, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B58, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B59, COL_Banners_P3, COL_Banners_P3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B5A, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B5B, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B5C, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B5D, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B5E, COL_Banners_P4, COL_Banners_P4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B5F, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B60, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B61, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B62, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B63, COL_Banners_P5, COL_Banners_P5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B64, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B65, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B66, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B67, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0




Down r OVL_CartographRedraw+D      mov     al, [COL_Cartographer+1]
Down r Cartographer_Screen_Draw+D  mov     al, [COL_Cartographer+1]
Down r OVL_CartographRedraw+15     mov     al, [COL_Cartographer+2]
Down r Cartographer_Screen_Draw+15 mov     al, [COL_Cartographer+2]
Down r OVL_CartographRedraw+22     mov     al, [COL_Cartographer+4]
Down r Cartographer_Screen_Draw+22 mov     al, [COL_Cartographer+4]
Down r OVL_CartographRedraw+2A     mov     al, [COL_Cartographer+5]
Down r Cartographer_Screen_Draw+2A mov     al, [COL_Cartographer+5]

0x00036B68, COL_Cartographer, COL_Cartographer, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B69, , , 1, 0, 0, 0, 0, 0, 0, 1, 0, 1
0x00036B6A, , , 1, 0, 0, 0, 0, 0, 0, 1, 0, 1
0x00036B6B, , , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
0x00036B6C, , , 1, 0, 0, 0, 0, 0, 0, 1, 0, 1
0x00036B6D, , , 1, 0, 0, 0, 0, 0, 0, 1, 0, 1
