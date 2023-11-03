

enum name
JSON name
actual text/string in-game

32 bits, 23 actual values
same string for ?
WTF with "Inherent Spell Knowledge"?  (vs. "Innate Spell Ability"?)

Page 79  (PDF Page 84)









dseg:57D0 4C 65 61 64 65 72 73 68 69 70 00                cnst_Leadership_2 db 'Leadership',0     ; DATA XREF: IDK_Hero_Populate_USW_List_Structure+21Co ...
dseg:57DD 4C 65 67 65 6E 64 61 72 79 00                   cnst_Legendary_2 db 'Legendary',0       ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_836E0o
dseg:57E7 42 6C 61 64 65 6D 61 73 74 65 72 00             cnst_Blademaster_2 db 'Blademaster',0   ; should use dseg:4575
dseg:57F3 41 72 6D 73 6D 61 73 74 65 72 00                cnst_Armsmaster_2 db 'Armsmaster',0     ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_83815o
dseg:57FE 43 6F 6E 73 74 69 74 75 74 69 6F 6E 00          cnst_Constitution_2 db 'Constitution',0 ; should use dseg:458c
dseg:580B 4D 69 67 68 74 00                               cnst_Might_2 db 'Might',0               ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_838FDo ...
dseg:5811 41 72 63 61 6E 65 20 50 6F 77 65 72 00          cnst_ArcanePower_2 db 'Arcane Power',0  ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_83909o ...
dseg:581E 53 61 67 65 00                                  cnst_Sage_3 db 'Sage',0                 ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_83915o ...
dseg:5829 50 72 61 79 65 72 6D 61 73 74 65 72 00          cnst_Prayermaster_2 db 'Prayermaster',0 ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_83A0Bo ...
dseg:5836 41 67 69 6C 69 74 79 00                         cnst_Agility_2 db 'Agility',0           ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_83A15o ...
dseg:5841 43 61 73 74 65 72 00                            cnst_Caster_2 db 'Caster',0





Ab_Leadership
Ab_Leadership2
Ab_Legendary
Ab_Legendary2
Ab_Blademaster
Ab_Blademaster2
Ab_Armsmaster
Ab_Armsmaster2
Ab_Constitution
Ab_Constitution2
Ab_Might

Ab_Might2
Ab_Arcane_Power
Ab_Arcane_Power2
Ab_Sage
Ab_Sage2
Ab_Prayermaster
Ab_Prayermaster2
Ab_Agility2
Ab_Lucky
Ab_Charmed
Ab_Noble
Ab_Female
Ab_Agility
















32-bits
24 values

/* 00000001 */ Ab_Leadership  = 1
/* 00000002 */ Ab_Leadership2  = 2
/* 00000004 */
/* 00000008 */ Ab_Legendary  = 8
/* 00000010 */ Ab_Legendary2  = 10h
/* 00000020 */
/* 00000040 */ Ab_Blademaster  = 40h
/* 00000080 */ Ab_Blademaster2  = 80h
/* 00000100 */
/* 00000200 */ Ab_Armsmaster  = 200h
/* 00000400 */ Ab_Armsmaster2  = 400h
/* 00000800 */ 
/* 00001000 */ Ab_Constitution  = 1000h
/* 00002000 */ Ab_Constitution2  = 2000h
/* 00004000 */
/* 00008000 */ Ab_Might  = 8000h
/* 00010000 */ Ab_Might2  = 1
/* 00020000 */ 
/* 00040000 */ Ab_Arcane_Power  = 4
/* 00080000 */ Ab_Arcane_Power2  = 8
/* 00100000 */ 
/* 00200000 */ Ab_Sage  = 20h
/* 00400000 */ Ab_Sage2  = 40h
/* 00800000 */ 
/* 01000000 */ Ab_Prayermaster  = 100h
/* 02000000 */ Ab_Prayermaster2  = 200h
/* 04000000 */ Ab_Agility2  = 400h
/* 08000000 */ Ab_Lucky  = 800h
/* 10000000 */ Ab_Charmed  = 1000h
/* 20000000 */ Ab_Noble  = 2000h
/* 40000000 */ Ab_Female  = 4000h
/* 80000000 */ Ab_Agility  = 8000h


; DATA XREF: USW_CreateFXList+463r ...

WZD dseg:3A50
01 00 00 00 2B 00 03 02 
02 00 00 00 2B 00 03 02
08 00 00 00 2D 00 04 02 
10 00 00 00 2D 00 04 02
40 00 00 00 2F 00 05 02 
80 00 00 00 2F 00 05 02
00 02 00 00 2E 00 06 02 
00 04 00 00 2E 00 06 02
00 10 00 00 32 00 07 02 
00 20 00 00 32 00 07 02
00 80 00 00 34 00 08 02 

00 00 01 00 34 00 08 02
00 00 04 00 36 00 09 02 
00 00 08 00 36 00 09 02
00 00 20 00 3D 00 0A 02 
00 00 40 00 3D 00 0A 02
00 00 00 01 39 00 0B 02 
00 00 00 02 39 00 0B 02
00 00 00 80 8F 00 73 02 
00 00 00 04 8F 00 73 02
00 00 00 08 3A 00 0C 02 
00 00 00 10 3B 00 0D 02
00 00 00 20 3C 00 0E 02                         


USW_HeroAbilities 
USW_HeroAbl <<Ab_Leadership, 0>, 2Bh, 0, HLP_LEADERSHIP>
USW_HeroAbl <<Ab_Leadership2, 0>, 2Bh, 0, HLP_LEADERSHIP>
USW_HeroAbl <<Ab_Legendary, 0>, 2Dh, 0, HLP_LEGENDARY>
USW_HeroAbl <<Ab_Legendary2, 0>, 2Dh, 0, HLP_LEGENDARY>
USW_HeroAbl <<Ab_Blademaster, 0>, 2Fh, 0, HLP_BLADEMASTER>
USW_HeroAbl <<Ab_Blademaster2, 0>, 2Fh, 0, HLP_BLADEMASTER>
USW_HeroAbl <<Ab_Armsmaster, 0>, 2Eh, 0, HLP_ARMSMASTER>
USW_HeroAbl <<Ab_Armsmaster2, 0>, 2Eh, 0, HLP_ARMSMASTER>
USW_HeroAbl <<Ab_Constitution, 0>, 32h, 0, HLP_CONSTITUTION>
USW_HeroAbl <<Ab_Constitution2, 0>, 32h, 0, HLP_CONSTITUTION>
USW_HeroAbl <<Ab_Might, 0>, 34h, 0, HLP_MIGHT>
USW_HeroAbl <<0, Ab_Might2>, 34h, 0, HLP_MIGHT>
USW_HeroAbl <<0, Ab_Arcane_Power>, 36h, 0, HLP_ARCANE_POWER>
USW_HeroAbl <<0, Ab_Arcane_Power2>, 36h, 0, HLP_ARCANE_POWER>
USW_HeroAbl <<0, Ab_Sage>, 3Dh, 0, HLP_SAGE>
USW_HeroAbl <<0, Ab_Sage2>, 3Dh, 0, HLP_SAGE>
USW_HeroAbl <<0, Ab_Prayermaster>, 39h, 0, HLP_PRAYERMASTER>
USW_HeroAbl <<0, Ab_Prayermaster2>, 39h, 0, HLP_PRAYERMASTER>
USW_HeroAbl <<0, Ab_Agility>, 8Fh, 0, HLP_AGILE>
USW_HeroAbl <<0, Ab_Agility2>, 8Fh, 0, HLP_AGILE>
USW_HeroAbl <<0, Ab_Lucky>, 3Ah, 0, HLP_LUCKY_Hero>
USW_HeroAbl <<0, Ab_Charmed>, 3Bh, 0, HLP_CHARMED>
USW_HeroAbl <<0, Ab_Noble>, 3Ch, 0, HLP_NOBLE>









00000001 ; enum Hero_Ability_LO (bitfield)
00000001 Ab_Leadership  = 1
00000002 Ab_Leadership2  = 2
00000008 Ab_Legendary  = 8
00000010 Ab_Legendary2  = 10h
00000040 Ab_Blademaster  = 40h
00000080 Ab_Blademaster2  = 80h
00000200 Ab_Armsmaster  = 200h
00000400 Ab_Armsmaster2  = 400h
00001000 Ab_Constitution  = 1000h
00002000 Ab_Constitution2  = 2000h
00008000 Ab_Might  = 8000h
00008000


00000001 ; enum Hero_Ability_HI (bitfield)
00000001 Ab_Might2  = 1
00000004 Ab_Arcane_Power  = 4
00000008 Ab_Arcane_Power2  = 8
00000020 Ab_Sage  = 20h
00000040 Ab_Sage2  = 40h
00000100 Ab_Prayermaster  = 100h
00000200 Ab_Prayermaster2  = 200h
00000400 Ab_Agility2  = 400h
00000800 Ab_Lucky  = 800h
00001000 Ab_Charmed  = 1000h
00002000 Ab_Noble  = 2000h
00004000 Ab_Female  = 4000h
00008000 Ab_Agility  = 8000h
00008000




IDK_Hero_Populate_USW_List_Structure()
loops on 23
switches on 20




// WZD

dseg:57D0 4C 65 61 64 65 72 73 68 69 70 00                cnst_Leadership_2 db 'Leadership',0     ; DATA XREF: IDK_Hero_Populate_USW_List_Structure+21Co ...
dseg:57D0                                                                                         ; should use dseg:455e
dseg:57DB 7D 00                                           cnst_ClosingBrckts_2 db '}',0           ; DATA XREF: IDK_Hero_Populate_USW_List_Structure+306o ...
dseg:57DB                                                                                         ; should use dseg:4569
dseg:57DD 4C 65 67 65 6E 64 61 72 79 00                   cnst_Legendary_2 db 'Legendary',0       ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_836E0o
dseg:57DD                                                                                         ; should use dseg:456b
dseg:57E7 42 6C 61 64 65 6D 61 73 74 65 72 00             cnst_Blademaster_2 db 'Blademaster',0   ; should use dseg:4575
dseg:57F3 41 72 6D 73 6D 61 73 74 65 72 00                cnst_Armsmaster_2 db 'Armsmaster',0     ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_83815o
dseg:57F3                                                                                         ; should use dseg:4581
dseg:57FE 43 6F 6E 73 74 69 74 75 74 69 6F 6E 00          cnst_Constitution_2 db 'Constitution',0 ; should use dseg:458c
dseg:580B 4D 69 67 68 74 00                               cnst_Might_2 db 'Might',0               ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_838FDo ...
dseg:580B                                                                                         ; should use dseg:4599
dseg:5811 41 72 63 61 6E 65 20 50 6F 77 65 72 00          cnst_ArcanePower_2 db 'Arcane Power',0  ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_83909o ...
dseg:5811                                                                                         ; should use dseg:459f
dseg:581E 53 61 67 65 00                                  cnst_Sage_3 db 'Sage',0                 ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_83915o ...
dseg:581E                                                                                         ; should use dseg:21e3
dseg:5823 20 00                                           cnst_Space_5 db ' ',0                   ; DATA XREF: IDK_Hero_Populate_USW_List_Structure+53Bo ...
dseg:5823                                                                                         ; should use dseg:2ad9
dseg:5825 20 72 70 00                                     cnst_ResearchPts_2 db ' rp',0           ; DATA XREF: IDK_Hero_Populate_USW_List_Structure+5FBo
dseg:5825                                                                                         ; should use dseg:45b3
dseg:5829 50 72 61 79 65 72 6D 61 73 74 65 72 00          cnst_Prayermaster_2 db 'Prayermaster',0 ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_83A0Bo ...
dseg:5829                                                                                         ; should use dseg:45b7
dseg:5836 41 67 69 6C 69 74 79 00                         cnst_Agility_2 db 'Agility',0           ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_83A15o ...
dseg:5836                                                                                         ; should use dseg:45c4
dseg:583E 20 2B 00                                        cnst_SpacePlus_2 db ' +',0              ; DATA XREF: IDK_Hero_Populate_USW_List_Structure+6C1o
dseg:583E                                                                                         ; should use dseg:45da
dseg:5841 43 61 73 74 65 72 00                            cnst_Caster_2 db 'Caster',0             ; DATA XREF: IDK_Hero_Populate_USW_List_Structure:loc_83B4Eo
dseg:5841                                                                                         ; should use dseg:45dd
dseg:5848 20 28 00                                        cnst_SpaceOpenBrace3 db ' (',0          ; should use dseg:2c40
dseg:584B 29 00                                           cnst_ClosingBrace_4 db ')',0            ; should use dseg:3428
dseg:584D 20 6D 70 00                                     cnst_Space_mp_2 db ' mp',0              ; DATA XREF: IDK_Hero_Populate_USW_List_Structure+861o
dseg:584D                                                                                         ; should use dseg:45e4
dseg:5851 00                                              db    0



ovr102:0F2B                                                 endp IDK_Hero_Populate_USW_List_Structure
ovr102:0F2B
ovr102:0F2B                                                 ; ---------------------------------------------------------------------------
ovr102:0F2C 00                                              db 0
ovr102:0F2D 00 00 01 00 02 00 08 00 10 00 40 00 80 00 00 02+sw_HeroAbility_LO dw      0,     1,     2,     8
ovr102:0F2D 00 04 00 10 00 20 00 80 00 00 00 00 00 00 00 00+                                        ; DATA XREF: IDK_Hero_Populate_USW_List_Structure+1E0o
ovr102:0F2D 00 00 00 00 00 00 00 00                         dw    10h,   40h,   80h,  200h          ; low halves of switch values
ovr102:0F2D                                                 dw   400h, 1000h, 2000h, 8000h
ovr102:0F2D                                                 dw      0,     0,     0,     0
ovr102:0F2D                                                 dw      0,     0,     0,     0
ovr102:0F55 00 80 00 00 00 00 00 00 00 00 00 00 00 00 00 00+sw_HeroAbility_HI dw  8000h,     0,     0,     0 ; high halves of switch values
ovr102:0F55 00 00 00 00 00 00 00 00 01 00 04 00 08 00 20 00+dw      0,     0,     0,     0
ovr102:0F55 40 00 00 01 00 02 00 04                         dw      0,     0,     0,     0
ovr102:0F55                                                 dw      1,     4,     8,   20h
ovr102:0F55                                                 dw    40h,  100h,  200h,  400h
ovr102:0F7D A5 0C 81 08 D3 08 3B 09 70 09 B7 09 08 0A 76 0A+off_83CED dw offset loc_83A15           ; jump table for switch statement
ovr102:0F7D A5 0A D4 0A 20 0B 8D 0B 93 0B 99 0B 9F 0B A5 0B+dw offset loc_835F1
ovr102:0F7D ED 0B 9B 0C A0 0C D2 0C                         dw offset loc_83643
ovr102:0F7D                                                 dw offset loc_836AB
ovr102:0F7D                                                 dw offset loc_836E0
ovr102:0F7D                                                 dw offset loc_83727
ovr102:0F7D                                                 dw offset loc_83778
ovr102:0F7D                                                 dw offset loc_837E6
ovr102:0F7D                                                 dw offset loc_83815
ovr102:0F7D                                                 dw offset loc_83844
ovr102:0F7D                                                 dw offset loc_83890
ovr102:0F7D                                                 dw offset loc_838FD
ovr102:0F7D                                                 dw offset loc_83903
ovr102:0F7D                                                 dw offset loc_83909
ovr102:0F7D                                                 dw offset loc_8390F
ovr102:0F7D                                                 dw offset loc_83915
ovr102:0F7D                                                 dw offset loc_8395D
ovr102:0F7D                                                 dw offset loc_83A0B
ovr102:0F7D                                                 dw offset loc_83A10
ovr102:0F7D                                                 dw offset loc_83A42
ovr102:0F7D                                                 ends ovr102






dseg:3A50 01 00 00 00 2B 00 03 02 02 00 00 00 2B 00 03 02+USW_HeroAbilities USW_HeroAbl <<Ab_Leadership, 0>, 2Bh, 0, HLP_LEADERSHIP>
dseg:3A50 08 00 00 00 2D 00 04 02 10 00 00 00 2D 00 04 02+                                        ; DATA XREF: USW_CreateFXList+463r ...
dseg:3A50 40 00 00 00 2F 00 05 02 80 00 00 00 2F 00 05 02+USW_HeroAbl <<Ab_Leadership2, 0>, 2Bh, 0, HLP_LEADERSHIP>
dseg:3A50 00 02 00 00 2E 00 06 02 00 04 00 00 2E 00 06 02+USW_HeroAbl <<Ab_Legendary, 0>, 2Dh, 0, HLP_LEGENDARY>
dseg:3A50 00 10 00 00 32 00 07 02 00 20 00 00 32 00 07 02+USW_HeroAbl <<Ab_Legendary2, 0>, 2Dh, 0, HLP_LEGENDARY>
dseg:3A50 00 80 00 00 34 00 08 02 00 00 01 00 34 00 08 02+USW_HeroAbl <<Ab_Blademaster, 0>, 2Fh, 0, HLP_BLADEMASTER>
dseg:3A50 00 00 04 00 36 00 09 02 00 00 08 00 36 00 09 02+USW_HeroAbl <<Ab_Blademaster2, 0>, 2Fh, 0, HLP_BLADEMASTER>
dseg:3A50 00 00 20 00 3D 00 0A 02 00 00 40 00 3D 00 0A 02+USW_HeroAbl <<Ab_Armsmaster, 0>, 2Eh, 0, HLP_ARMSMASTER>
dseg:3A50 00 00 00 01 39 00 0B 02 00 00 00 02 39 00 0B 02+USW_HeroAbl <<Ab_Armsmaster2, 0>, 2Eh, 0, HLP_ARMSMASTER>
dseg:3A50 00 00 00 80 8F 00 73 02 00 00 00 04 8F 00 73 02+USW_HeroAbl <<Ab_Constitution, 0>, 32h, 0, HLP_CONSTITUTION>
dseg:3A50 00 00 00 08 3A 00 0C 02 00 00 00 10 3B 00 0D 02+USW_HeroAbl <<Ab_Constitution2, 0>, 32h, 0, HLP_CONSTITUTION>
dseg:3A50 00 00 00 20 3C 00 0E 02                         USW_HeroAbl <<Ab_Might, 0>, 34h, 0, HLP_MIGHT>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Might2>, 34h, 0, HLP_MIGHT>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Arcane_Power>, 36h, 0, HLP_ARCANE_POWER>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Arcane_Power2>, 36h, 0, HLP_ARCANE_POWER>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Sage>, 3Dh, 0, HLP_SAGE>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Sage2>, 3Dh, 0, HLP_SAGE>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Prayermaster>, 39h, 0, HLP_PRAYERMASTER>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Prayermaster2>, 39h, 0, HLP_PRAYERMASTER>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Agility>, 8Fh, 0, HLP_AGILE>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Agility2>, 8Fh, 0, HLP_AGILE>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Lucky>, 3Ah, 0, HLP_LUCKY_Hero>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Charmed>, 3Bh, 0, HLP_CHARMED>
dseg:3A50                                                 USW_HeroAbl <<0, Ab_Noble>, 3Ch, 0, HLP_NOBLE>
dseg:3B08 64 40 02 00 0E 00 8C 01 71 40 04 00 04 00 8D 01+USW_Abilities USW_Ability <offset cnst_LargeShield, Ab_LargeShield, 0Eh, 0, HLP_LARGE_SHIELD>
dseg:3B08 7D 40 08 00 0F 00 8E 01 8A 40 10 00 10 00 8F 01+                                        ; DATA XREF: USW_CreateFXList+1002r ...
dseg:3B08 91 40 20 00 11 00 90 01 A0 40 40 00 12 00 91 01+USW_Ability <offset cnst_PlaneShift, Ab_PlaneShift, 4, 0, HLP_PLANE_SHIFT> ; should use dseg:220d
dseg:3B08 AD 40 80 00 13 00 92 01 BB 40 00 01 81 00 93 01+USW_Ability <offset cnst_WallCrusher, Ab_WallCrshr, 0Fh, 0, HLP_WALL_CRUSHER>
dseg:3B08 C6 40 00 02 15 00 94 01 D6 40 00 04 28 00 95 01+USW_Ability <offset cnst_Healer_2, Ab_Healer, 10h, 0, HLP_HEALER>
dseg:3B08 E5 40 00 08 16 00 96 01 F3 40 00 10 17 00 97 01+USW_Ability <offset cnst_CreateOutpost, Ab_CrOutpost, 11h, 0, HLP_CREATE_OUTPOST>
dseg:3B08 00 41 00 20 18 00 98 01 0D 41 00 40 19 00 99 01+USW_Ability <offset cnst_Invisibility, Ab_Invisibility, 12h, 0, HLP_INVISIBILITY>
dseg:3B08 14 41 00 80 30 00 9A 01                         USW_Ability <offset cnst_CreateUndead, Ab_CrUndead, 13h, 0, HLP_CREATE_UNDEAD>
dseg:3B08                                                 USW_Ability <offset cnst_LongRange, Ab_LongRange, 81h, 0, HLP_LONG_RANGE>
dseg:3B08                                                 USW_Ability <offset cnst_LandCorruption, _LandCorruption, 15h, 0, HLP_LAND_CORRUPTION>
dseg:3B08                                                 USW_Ability <offset cnst_MeldWithNode, Ab_Meld, 28h, 0, HLP_MELD_WITH_NODE>
dseg:3B08                                                 USW_Ability <offset cnst_NonCorporeal, Ab_NonCorporeal, 16h, 0, HLP_NON_CORPOREAL>
dseg:3B08                                                 USW_Ability <offset cnst_WindWalking, Ab_WindWalking, 17h, 0, HLP_WIND_WALKING_Ab>
dseg:3B08                                                 USW_Ability <offset cnst_Regeneration, Ab_Regeneration, 18h, 0, HLP_REGENERATION>
dseg:3B08                                                 USW_Ability <offset cnst_Purify, Ab_Purify, 19h, 0, HLP_PURIFY_Ab>
dseg:3B08                                                 USW_Ability <offset cnst_Negate1stStrike, Ab_Negate1stSt, 30h, 0, HLP_NEGATE_FIRST_STRIKE>
dseg:3B80 28 41 00 04 7F 00 9B 01 2E 41 80 00 05 00 9C 01+USW_Attributes_1 USW_Attribute_1 <offset cnst_Lucky, Abl_Lucky, 7Fh, 0, HLP_LUCKY>
dseg:3B80 3E 41 01 00 06 00 9D 01 4C 41 02 00 07 00 9E 01+                                        ; DATA XREF: USW_CreateFXList+12EDr ...
dseg:3B80 5D 41 00 01 08 00 9F 01 6D 41 04 00 09 00 A0 01+USW_Attribute_1 <offset cnst_PoisonImmunity, Imm_Poison, 5, 0, HLP_POISON_IMMUNITY> ; "Lucky"
dseg:3B80 7F 41 08 00 0A 00 A1 01 92 41 10 00 0B 00 A2 01+USW_Attribute_1 <offset cnst_FireImmunity, Imm_Fire, 6, 0, HLP_FIRE_IMMUNITY>
dseg:3B80 A0 41 20 00 0C 00 A3 01 AF 41 40 00 31 00 A4 01 USW_Attribute_1 <offset cnst_StoningImmunity, Imm_Stoning, 7, 0, HLP_STONING_IMMUNITY>
dseg:3B80                                                 USW_Attribute_1 <offset cnst_WeaponImmunity, Imm_Weapon, 8, 0, HLP_WEAPON_IMMUNITY>
dseg:3B80                                                 USW_Attribute_1 <offset cnst_MissileImmunity, Imm_Missiles, 9, 0, HLP_MISSILES_IMMUNITY>
dseg:3B80                                                 USW_Attribute_1 <offset cnst_IllusionsImm, Imm_Illusion, 0Ah, 0, HLP_ILLUSIONS_IMMUNITY>
dseg:3B80                                                 USW_Attribute_1 <offset cnst_ColdImmunity, Imm_Cold, 0Bh, 0, HLP_COLD_IMMUNITY>
dseg:3B80                                                 USW_Attribute_1 <offset cnst_MagicImmunity, Imm_Magic, 0Ch, 0, HLP_MAGIC_IMMUNITY>
dseg:3B80                                                 USW_Attribute_1 <offset cnst_DeathImmunity, Imm_Death, 31h, 0, HLP_DEATH_IMMUNITY>
dseg:3BD0 BE 41 01 00 26 00 A5 01 CC 41 02 00 27 00 A6 01+USW_Attributes_2 USW_Attribute_2 <offset cnst_HealingSpell, Spl_Healing, 26h, 0, HLP_HEALING_SPELL>
dseg:3BD0 DC 41 04 00 29 00 A7 01 EB 41 08 00 20 00 A8 01+                                        ; DATA XREF: USW_CreateFXList+138Dr ...
dseg:3BD0 F6 41 20 00 84 00 A9 01 07 42 10 00 83 00 AA 01+USW_Attribute_2 <offset cnst_FireBallSpell, Spl_Fireball, 27h, 0, HLP_FIRE_BALL_SPELL> ; "Healing "
dseg:3BD0 11 42 40 00 21 00 AB 01 23 42 80 00 22 00 AC 01 USW_Attribute_2 <offset cnst_DoomboltSpell, Spl_DoomBolt, 29h, 0, HLP_DOOMBOLT_SPELL>
dseg:3BD0                                                 USW_Attribute_2 <offset cnst_Immolation, Ab_Immolation, 20h, 0, HLP_IMMOLATION>
dseg:3BD0                                                 USW_Attribute_2 <offset cnst_CauseFearSpell, Ab_CauseFear, 84h, 0, HLP_CAUSE_FEAR_SPELL>
dseg:3BD0                                                 USW_Attribute_2 <offset cnst_WebSpell, Spl_Web, 83h, 0, HLP_WEB_SPELL>
dseg:3BD0                                                 USW_Attribute_2 <offset cnst_Resistance2All, Ab_ResistAll, 21h, 0, HLP_RESISTANCE_TO_ALL>
dseg:3BD0                                                 USW_Attribute_2 <offset cnst_HolyBonus, Ab_HolyBonus, 22h, 0, HLP_HOLY_BONUS>
dseg:3C10 2E 42 01 00 1C 00 AD 01 1B 41 02 00 1D 00 AE 01+USW_ATK_Flags USW_AttackFlag <offset cnst_ArmorPiercing, Att_ArmorPrc, 1Ch, 0, HLP_ARMOR_PIERCING>
dseg:3C10 3D 42 04 00 1E 00 AF 01 44 42 08 00 1F 00 B0 01+                                        ; DATA XREF: USW_CreateFXList+142Fr ...
dseg:3C10 4F 42 10 00 8C 00 B1 01 60 42 20 00 74 00 B2 01+USW_AttackFlag <offset cnst_FirstStrike, Att_1stStrike, 1Dh, 0, HLP_FIRST_STRIKE> ; "First Strike"
dseg:3C10 6C 42 40 00 23 00 B3 01 75 42 80 00 1B 00 B4 01+USW_AttackFlag <offset cnst_Poison, Att_Poison, 1Eh, 0, HLP_POISON>
dseg:3C10 83 42 00 02 8D 00 B5 01 8F 42 00 04 8E 00 B6 01+USW_AttackFlag <offset cnst_LifeSteal, Att_LifeSteal, 1Fh, 0, HLP_LIFE_STEAL>
dseg:3C10 9B 42 00 08 85 00 B7 01                         USW_AttackFlag <offset cnst_AutomaticDmg, Att_DoomDmg, 8Ch, 0, HLP_CHAOS>
dseg:3C10                                                 USW_AttackFlag <offset cnst_Destruction, Att_Destruct, 74h, 0, HLP_DESTRUCTION>
dseg:3C10                                                 USW_AttackFlag <offset cnst_Illusion, Att_Illusion, 23h, 0, HLP_ILLUSION>
dseg:3C10                                                 USW_AttackFlag <offset cnst_StoningTouch, Att_StnTouch, 1Bh, 0, HLP_STONING_TOUCH>
dseg:3C10                                                 USW_AttackFlag <offset cnst_DeathTouch, Att_DthTouch, 8Dh, 0, HLP_CAUSE_DEATH>
dseg:3C10                                                 USW_AttackFlag <offset cnst_PowerDrain, Att_PwrDrain, 8Eh, 0, HLP_POWER_DRAIN>
dseg:3C10                                                 USW_AttackFlag <offset cnst_DispelEvil, Att_DsplEvil, 85h, 0, HLP_DISPEL_EVIL>
dseg:3C68 A7 42 04 3E B8 01 A7 42 08 3F B9 01 A7 42 10 40+USW_ChaosChannels USW_Mutation <offset cnst_ChaosChannel, CC_Defense, 3Eh, HLP_CHAOS_CHANNEL_Def>
dseg:3C68 BA 01                                                                                   ; DATA XREF: USW_CreateFXList+1933r ...
dseg:3C68                                                 USW_Mutation <offset cnst_ChaosChannel, CC_Flight, 3Fh, HLP_CHAOS_CHANNEL_Fly> ; "Chaos Channel"
dseg:3C68                                                 USW_Mutation <offset cnst_ChaosChannel, CC_Breath, 40h, HLP_CHAOS_CHANNEL_Brth>
dseg:3C7A B5 42 64 82 BB 01 BC 42 65 8A BC 01 C8 42 66 89+USW_SR_Attacks USW_SR_Attack <offset cnst_Thrown, SR_Thrown, 82h, HLP_THROWN>
dseg:3C7A BD 01 D2 42 67 1A BE 01 DF 42 68 86 BF 01 E9 42+                                        ; DATA XREF: USW_CreateFXList+1594r ...
dseg:3C7A 69 87 C0 01                                     USW_SR_Attack <offset cnst_FireBreath, SR_FireBreath, 8Ah, HLP_FIRE_BREATH> ; "Thrown"
dseg:3C7A                                                 USW_SR_Attack <offset cnst_Lightning, SR_Lightning, 89h, HLP_LIGHTNING_BREATH>
dseg:3C7A                                                 USW_SR_Attack <offset cnst_StoningGaze, SR_StoneGaze, 1Ah, HLP_STONING_GAZE>
dseg:3C7A                                                 USW_SR_Attack <offset cnst_DoomGaze, SR_MultiGaze, 86h, HLP_DOOM_GAZE>
dseg:3C7A                                                 USW_SR_Attack <offset cnst_DeathGaze, SR_DeathGaze, 87h, HLP_DEATH_GAZE>
dseg:3C9E EB 41 01 00 00 00 75 00 C1 01 F4 42 02 00 00 00+USW_UnitEnchants USW_UnitEnchant <offset cnst_Immolation, <UE_Immolation, 0>, 75h, 0, HLP_IMMOLATION_UE>
dseg:3C9E 76 00 C2 01 02 43 04 00 00 00 80 00 C3 01 0A 43+                                        ; DATA XREF: USW_CreateFXList+1AD1r ...
dseg:3C9E 08 00 00 00 61 00 C4 01 18 43 10 00 00 00 43 00+USW_UnitEnchant <offset cnst_GuardianWind, <UE_Guardian_Wind, 0>, 76h, 0, HLP_GUARDIAN_WIND> ; "Invisibility"
dseg:3C9E C5 01 27 43 20 00 00 00 44 00 C6 01 33 43 40 00+USW_UnitEnchant <offset cnst_Berserk, <UE_Berserk, 0>, 80h, 0, HLP_BERSERK>
dseg:3C9E 00 00 45 00 C7 01 3E 43 80 00 00 00 46 00 C8 01+USW_UnitEnchant <offset cnst_CloakOfFear, <UE_Cloak_of_Fear, 0>, 61h, 0, HLP_CLOAK_OF_FEAR>
dseg:3C9E 4B 43 00 01 00 00 47 00 C9 01 59 43 00 04 00 00+USW_UnitEnchant <offset cnst_BlackChannels, <UE_Black_Channels, 0>, 43h, 0, HLP_BLACK_CHANNELS>
dseg:3C9E 49 00 CA 01 69 43 00 02 00 00 48 00 CB 01 79 43+USW_UnitEnchant <offset cnst_WraithForm, <UE_Wraith_Form, 0>, 44h, 0, HLP_WRAITH_FORM>
dseg:3C9E 00 08 00 00 4A 00 CC 01 84 43 00 10 00 00 4B 00+USW_UnitEnchant <offset cnst_Regenerate, <UE_Regeneration, 0>, 45h, 0, HLP_REGENERATE>
dseg:3C9E CD 01 8E 43 00 20 00 00 4C 00 CE 01 98 43 00 40+USW_UnitEnchant <offset cnst_PathFinding, <UE_Path_Finding, 0>, 46h, 0, HLP_PATHFINDING_UE>
dseg:3C9E 00 00 77 00 CF 01 A0 40 00 80 00 00 4E 00 D0 01+USW_UnitEnchant <offset cnst_WaterWalking, <UE_Water_Walking, 0>, 47h, 0, HLP_WATER_WALKING>
dseg:3C9E F3 40 00 00 01 00 4F 00 D1 01 A3 43 00 00 02 00+USW_UnitEnchant <offset cnst_ElementalArmor, <UE_Elemental_Armor, 0>, 49h, 0, HLP_ELEMENTAL_ARMOR>
dseg:3C9E 50 00 D2 01 AA 43 00 00 04 00 51 00 D3 01 A0 41+USW_UnitEnchant <offset cnst_ResistElements, <UE_Resist_Elements, 0>, 48h, 0, HLP_RESIST_ELEMENTS>
dseg:3C9E 00 00 08 00 52 00 D4 01 B7 43 00 00 10 00 53 00+USW_UnitEnchant <offset cnst_StoneSkin, <UE_Stone_Skin, 0>, 4Ah, 0, HLP_STONE_SKIN>
dseg:3C9E D5 01 C3 43 00 00 20 00 54 00 D6 01 D3 43 00 00+USW_UnitEnchant <offset cnst_IronSkin, <UE_Iron_Skin, 0>, 4Bh, 0, HLP_IRON_SKIN>
dseg:3C9E 40 00 55 00 D7 01 DE 43 00 00 80 00 56 00 D8 01+USW_UnitEnchant <offset cnst_Endurance, <UE_Endurance, 0>, 4Ch, 0, HLP_ENDURANCE>
dseg:3C9E EA 43 00 00 00 01 57 00 D9 01 F2 43 00 00 00 02+USW_UnitEnchant <offset cnst_SpellLock, <UE_Spell_Lock, 0>, 77h, 0, HLP_SPELL_LOCK>
dseg:3C9E 58 00 DA 01 F8 43 00 00 00 04 59 00 DB 01 03 44+USW_UnitEnchant <offset cnst_Invisibility, <UE_Invisibility, 0>, 4Eh, 0, HLP_INVISIBILITY_UE>
dseg:3C9E 00 00 00 08 41 00 DC 01 12 44 00 00 00 10 5B 00+USW_UnitEnchant <offset cnst_WindWalking, <0, UE_Wind_Walking>, 4Fh, 0, HLP_WIND_WALKING_UE>
dseg:3C9E DD 01 20 44 00 00 00 20 5C 00 DE 01 2B 44 00 00+USW_UnitEnchant <offset cnst_Flight, <0, UE_Flight>, 50h, 0, HLP_FLIGHT>
dseg:3C9E 00 40 5D 00 DF 01 39 44 00 00 00 80 5E 00 E0 01 USW_UnitEnchant <offset cnst_ResistMagic, <0, UE_Resist_Magic>, 51h, 0, HLP_RESIST_MAGIC>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_MagicImmunity, <0, UE_Magic_Immunity>, 52h, 0, HLP_MAGIC_IMMUNITY_UE>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_FlameBlade, <0, UE_Flame_Blade>, 53h, 0, HLP_FLAME_BLADE>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_EldritchWeapon, <0, UE_Eldritch_Weapon>, 54h, 0, HLP_ELDRITCH_WEAPON>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_TrueSight, <0, UE_True_Sight>, 55h, 0, HLP_TRUE_SIGHT>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_HolyWeapon, <0, UE_Holy_Weapon>, 56h, 0, HLP_HOLY_WEAPON>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_Heroism, <0, UE_Heroism>, 57h, 0, HLP_HEROISM>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_Bless, <0, UE_Bless>, 58h, 0, HLP_BLESS>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_LionHeart, <0, UE_Lionheart>, 59h, 0, HLP_LION_HEART>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_GiantStrength, <0, UE_Giant_Strength>, 41h, 0, HLP_GIANT_STRENGTH>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_PlanarTravel, <0, UE_Planar_Travel>, 5Bh, 0, HLP_PLANAR_TRAVEL_UE>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_HolyArmor, <0, UE_Holy_Armor>, 5Ch, 0, HLP_HOLY_ARMOR>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_Righteousness, <0, UE_Righteousness>, 5Dh, 0, HLP_RIGHTEOUSNESS>
dseg:3C9E                                                 USW_UnitEnchant <offset cnst_Invulnerability, <0, UE_Invulnerability>, 5Eh, 0, HLP_INVULNERABILITY>
dseg:3DDE 49 44 01 00 00 00 65 00 E1 01 51 44 02 00 00 00+USW_Combat_FX USW_CombatEffect <offset cnst_Vertigo, CEff_Vertigo, 0, 65h, 0, HLP_VERTIGO>
dseg:3DDE 6F 00 E2 01 5B 44 04 00 00 00 70 00 E3 01 65 44+                                        ; DATA XREF: USW_CreateFXList+1879r ...
dseg:3DDE 08 00 00 00 71 00 E4 01 70 44 10 00 00 00 5F 00+USW_CombatEffect <offset cnst_Confusion, CEff_Confusion, 0, 6Fh, 0, HLP_CONFUSION> ; "Vertigo"
dseg:3DDE E5 01 78 44 20 00 00 00 60 00 E6 01 81 44 40 00+USW_CombatEffect <offset cnst_Whirlwind, CEff_Whirlwind, 0, 70h, 0, HLP_WHIRLWIND>
dseg:3DDE 00 00 72 00 E7 01 8D 44 80 00 00 00 7A 00 E8 01+USW_CombatEffect <offset cnst_MindStorm, CEff_Mind_Storm, 0, 71h, 0, HLP_MIND_STORM>
dseg:3DDE 8D 44 00 01 00 00 7B 00 E9 01 8D 44 00 02 00 00+USW_CombatEffect <offset cnst_Shatter, CEff_Shatter, 0, 5Fh, 0, HLP_SHATTER>
dseg:3DDE 7C 00 EA 01 9B 44 00 04 00 00 79 00 EB 01 A6 44+USW_CombatEffect <offset cnst_Weakness, CEff_Weakness, 0, 60h, 0, HLP_WEAKNESS>
dseg:3DDE 00 08 00 00 4D 00 EC 01 AC 44 00 10 00 00 63 00+USW_CombatEffect <offset cnst_BlackSleep, CEff_Black_Sleep, 0, 72h, 0, HLP_BLACK_SLEEP>
dseg:3DDE ED 01 B0 44 00 20 00 00 64 00 EE 01 C1 44 00 40+USW_CombatEffect <offset cnst_WarpCreature, CEff_Warped_Attack, 0, 7Ah, 0, HLP_WARP_CREATURE_Atk>
dseg:3DDE 00 00 62 00 EF 01                               USW_CombatEffect <offset cnst_WarpCreature, CEff_Warped_Defense, 0, 7Bh, 0, HLP_WARP_CREATURE_Def>
dseg:3DDE                                                 USW_CombatEffect <offset cnst_WarpCreature, CEff_Warped_Resist, 0, 7Ch, 0, HLP_WARP_CREATURE_Res>
dseg:3DDE                                                 USW_CombatEffect <offset cnst_MindTwist, CEff_Mind_Twist, 0, 79h, 0, HLP_MANA_LEAK>
dseg:3DDE                                                 USW_CombatEffect <offset cnst_Haste, CEff_Haste, 0, 4Dh, 0, HLP_HASTE>
dseg:3DDE                                                 USW_CombatEffect <offset cnst_Web, CEff_Web, 0, 63h, 0, HLP_WEB>
dseg:3DDE                                                 USW_CombatEffect <offset cnst_CreatureBinding, CEff_Creature_Binding, 0, 64h, 0, HLP_CREATURE_BINDING>
dseg:3DDE                                                 USW_CombatEffect <offset cnst_Possession, CEff_Possession, 0, 62h, 0, HLP_POSSESSION>
dseg:3E74 A0 41 00 00 08 00 00 00 2C 02 33 43 40 00 00 00+USW_Item_UEs USW_ItemPower <offset cnst_MagicImmunity, <0, IP_Magic_Immunity>, 0, HLP_MAGIC_IMMUNITY_I>
dseg:3E74 01 00 2D 02 A6 44 00 40 00 00 02 00 2E 02 D3 43+                                        ; DATA XREF: USW_CreateFXList+3BBr ...
dseg:3E74 00 00 40 00 03 00 2F 02 3E 43 80 00 00 00 04 00+USW_ItemPower <offset cnst_Regenerate, <IP_Regeneration, 0>, 1, HLP_REGENERATE_I> ; "Magic Immunity"
dseg:3E74 30 02 4B 43 00 01 00 00 05 00 31 02 F2 43 00 00+USW_ItemPower <offset cnst_Haste, <IP_Haste, 0>, 2, HLP_HASTE_I>
dseg:3E74 00 02 06 00 32 02 F8 43 00 00 00 04 07 00 33 02+USW_ItemPower <offset cnst_TrueSight, <0, IP_True_Sight>, 3, HLP_TRUE_SIGHT_I>
dseg:3E74 A0 40 00 80 00 00 08 00 34 02 12 44 00 00 00 10+USW_ItemPower <offset cnst_PathFinding, <IP_Pathfinding, 0>, 4, HLP_PATH_FINDING>
dseg:3E74 09 00 35 02 8E 43 00 20 00 00 0A 00 36 02 2B 44+USW_ItemPower <offset cnst_WaterWalking, <IP_Water_Walking, 0>, 5, HLP_WATER_WALKING_I>
dseg:3E74 00 00 00 40 0B 00 37 02 39 44 00 00 00 80 0C 00+USW_ItemPower <offset cnst_Bless, <0, IP_Bless>, 6, HLP_BLESS_I>
dseg:3E74 38 02 69 43 00 02 00 00 0D 00 39 02 59 43 00 04+USW_ItemPower <offset cnst_LionHeart, <0, IP_Lion_Heart>, 7, HLP_LION_HEART_I>
dseg:3E74 00 00 0E 00 3A 02 0A 43 08 00 00 00 0F 00 3B 02+USW_ItemPower <offset cnst_Invisibility, <IP_Invisibility, 0>, 8, HLP_INVISIBILITY_I>
dseg:3E74 A3 43 00 00 02 00 10 00 3C 02 AA 43 00 00 04 00+USW_ItemPower <offset cnst_PlanarTravel, <0, IP_Planar_Travel>, 9, HLP_PLANAR_TRAVEL_I>
dseg:3E74 11 00 3D 02 F4 42 02 00 00 00 15 00 3E 02 03 44+USW_ItemPower <offset cnst_Endurance, <IP_Endurance, 0>, 0Ah, HLP_ENDURANCE_I>
dseg:3E74 00 00 00 08 14 00 3F 02 CB 44 00 00 00 20 12 00+USW_ItemPower <offset cnst_Righteousness, <0, IP_Righteousness>, 0Bh, HLP_RIGHTEOUSNESS_I>
dseg:3E74 40 02                                           USW_ItemPower <offset cnst_Invulnerability, <0, IP_Invulnerability>, 0Ch, HLP_INVULNERABILITY_I>
dseg:3E74                                                 USW_ItemPower <offset cnst_ResistElements, <IP_Resist_Elements, 0>, 0Dh, HLP_RESIST_ELEMENTS_I>
dseg:3E74                                                 USW_ItemPower <offset cnst_ElementalArmor, <IP_Elemental_Armour, 0>, 0Eh, HLP_ELEMENTAL_ARMOR_I>
dseg:3E74                                                 USW_ItemPower <offset cnst_CloakOfFear, <IP_Cloak_Of_Fear, 0>, 0Fh, HLP_CLOAK_OF_FEAR_I>
dseg:3E74                                                 USW_ItemPower <offset cnst_Flight, <0, IP_Flight>, 10h, HLP_FLIGHT_I>
dseg:3E74                                                 USW_ItemPower <offset cnst_ResistMagic, <0, IP_Resist_Magic>, 11h, HLP_RESIST_MAGIC_I>
dseg:3E74                                                 USW_ItemPower <offset cnst_GuardianWind, <IP_Guardian_Wind, 0>, 15h, HLP_GUARDIAN_WIND_I>
dseg:3E74                                                 USW_ItemPower <offset cnst_GiantStrength, <0, IP_Giant_Strength>, 14h, HLP_GIANT_STRENGTH_I>
dseg:3E74                                                 USW_ItemPower <offset cnst_Merging, <0, IP_Merging>, 12h, HLP_MERGING_I>
dseg:3F46 D3 44 DB 44 E3 44 EB 44 F1 44 FD 44             USW_UnitLevels@ dw offset cnst_UnitLevel_1, offset cnst_UnitLevel_2, offset cnst_UnitLevel_3, offset cnst_UnitLevel_4, offset cnst_UnitLevel_5, offset cnst_UnitLevel_6
dseg:3F46                                                                                         ; DATA XREF: USW_CreateFXList+F0r
dseg:3F46                                                                                         ; "Recruit"
dseg:3F52 06 45 0B 45 14 45 1C 45 FD 44 26 45 2B 45 36 45+USW_HeroLevels@ dw offset cnst_HeroLevel_1, offset cnst_HeroLevel_2, offset cnst_HeroLevel_3, offset cnst_HeroLevel_4, offset cnst_UnitLevel_6, offset cnst_HeroLevel_6, offset cnst_HeroLevel_7, offset cnst_HeroLevel_8, offset cnst_HeroLevel_9
dseg:3F52 41 45                                                                                   ; DATA XREF: USW_CreateFXList+D4r ...
dseg:3F52                                                                                         ; "Champion"
dseg:3F64 5D 46 66 46 01 00 00 00 C8 42 76 46 04 00 00 00+ITEM_ViewTexts IVIEW_TEXT <offset cnst_Vampiric, offset cnst_VampiricText, <IP_Vampiric, 0>>
dseg:3F64 87 46 95 46 08 00 00 00 60 42 AE 46 10 00 00 00+                                        ; DATA XREF: ITEM_GetPowerNames+4E4r ...
dseg:3F64 CB 46 D6 46 20 00 00 00 00 41 EC 46 40 00 00 00+IVIEW_TEXT <offset cnst_Lightning, offset cnst_LightningText, <IP_Lightning, 0>> ; BUG: there is no swiftness spell in the game
dseg:3F64 27 46 04 47 80 00 00 00 4B 43 1B 47 00 01 00 00+IVIEW_TEXT <offset cnst_CloakofFear, offset cnst_CoF_Text, <IP_Cloak_Of_Fear, 0>>
dseg:3F64 69 43 34 47 00 02 00 00 59 43 4F 47 00 04 00 00+IVIEW_TEXT <offset cnst_Destruction, offset cnst_DestructionText, <IP_Destruction, 0>>
dseg:3F64 6A 47 6F 47 00 08 00 00 8E 43 7F 47 00 20 00 00+IVIEW_TEXT <offset cnst_Wraithform, offset cnst_WraithformText, <IP_Wraith_Form, 0>>
dseg:3F64 A6 44 94 47 00 40 00 00 A0 40 A6 47 00 80 00 00+IVIEW_TEXT <offset cnst_Regeneration, offset cnst_RegenerateText, <IP_Regeneration, 0>>
dseg:3F64 C2 47 C8 47 00 00 01 00 A3 43 DC 47 00 00 02 00+IVIEW_TEXT <offset cnst_Pathfinding, offset cnst_PF_Text, <IP_Pathfinding, 0>>
dseg:3F64 AA 43 EE 47 00 00 04 00 A0 41 06 48 00 00 08 00+IVIEW_TEXT <offset cnst_WaterWalking, offset cnst_WaterWalkText, <IP_Water_Walking, 0>>
dseg:3F64 8F 42 20 48 00 00 00 01 39 48 46 48 00 00 20 00+IVIEW_TEXT <offset cnst_ResistElements, offset cnst_ResistElemText, <IP_Resist_Elements, 0>>
dseg:3F64 D3 43 65 48 00 00 40 00 7B 48 86 48 00 00 80 00+IVIEW_TEXT <offset cnst_ElementalArmor, offset cnst_ElemArmorText, <IP_Elemental_Armour, 0>>
dseg:3F64 A9 48 B1 48 00 00 10 00 F2 43 C6 48 00 00 00 02+IVIEW_TEXT <offset cnst_Doom, offset cnst_DoomText, <IP_Chaos, 0>>
dseg:3F64 F8 43 D7 48 00 00 00 04 ED 48 F5 48 00 10 00 00+IVIEW_TEXT <offset cnst_Endurance, offset cnst_EnduranceText, <IP_Endurance, 0>>
dseg:3F64 12 44 11 49 00 00 00 10 CB 44 2A 49 00 00 00 20+IVIEW_TEXT <offset cnst_Haste, offset cnst_HasteText, <IP_Haste, 0>>
dseg:3F64 2B 44 3D 49 00 00 00 40 39 44 56 49 00 00 00 80+IVIEW_TEXT <offset cnst_Invisibility, offset cnst_InvisText, <IP_Invisibility, 0>>
dseg:3F64 F4 42 71 49 02 00 00 00 03 44 8A 49 00 00 00 08 IVIEW_TEXT <offset cnst_Death, offset cnst_DeathText, <0, IP_Death>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_Flight, offset cnst_FlightText, <0, IP_Flight>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_ResistMagic, offset cnst_ResMagicText, <0, IP_Resist_Magic>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_MagicImmunity, offset cnst_MagicImmText, <0, IP_Magic_Immunity>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_PowerDrain, offset cnst_PDrainText, <0, IP_Power_Drain>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_HolyAvenger, offset cnst_HolyAvengerText, <0, IP_Holy_Avenger>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_TrueSight, offset cnst_TrueSightText, <0, IP_True_Sight>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_Phantasmal, offset cnst_PhantasmalText, <0, IP_Phantasmal>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_Flaming, offset cnst_FlamingText, <0, IP_Flaming>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_Bless, offset cnst_BlessText, <0, IP_Bless>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_LionHeart, offset cnst_LionheartText, <0, IP_Lion_Heart>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_Stoning, offset cnst_StoningText, <IP_Stoning, 0>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_PlanarTravel, offset cnst_PlanarTrvlText, <0, IP_Planar_Travel>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_Merging, offset cnst_MergingText, <0, IP_Merging>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_Righteousness, offset cnst_RightsnssText, <0, IP_Righteousness>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_Invulnerability, offset cnst_InvulnText, <0, IP_Invulnerability>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_GuardianWind, offset cnst_GWindText, <IP_Guardian_Wind, 0>>
dseg:3F64                                                 IVIEW_TEXT <offset cnst_GiantStrength, offset cnst_GStrText, <0, IP_Giant_Strength>>
dseg:4064 4C 61 72 67 65 20 53 68 69 65 6C 64             cnst_LargeShield db 'Large Shield'      ; DATA XREF: dseg:USW_Abilitieso
dseg:4070 00                                              cnst_ZeroString_18 db 0                 ; DATA XREF: USW_CreateFXList:loc_679A8o ...
dseg:4071 50 6C 61 6E 65 20 53 68 69 66 74 00             cnst_PlaneShift db 'Plane Shift',0      ; DATA XREF: dseg:USW_Abilitieso
dseg:407D 57 61 6C 6C 20 43 72 75 73 68 65 72 00          cnst_WallCrusher db 'Wall Crusher',0    ; DATA XREF: dseg:USW_Abilitieso
dseg:408A 48 65 61 6C 65 72 00                            cnst_Healer_2 db 'Healer',0             ; DATA XREF: dseg:USW_Abilitieso
dseg:408A                                                                                         ; should use dseg:220d
dseg:4091 43 72 65 61 74 65 20 4F 75 74 70 6F 73 74 00    cnst_CreateOutpost db 'Create Outpost',0
dseg:4091                                                                                         ; DATA XREF: dseg:USW_Abilitieso
dseg:40A0 49 6E 76 69 73 69 62 69 6C 69 74 79 00          cnst_Invisibility db 'Invisibility',0   ; DATA XREF: dseg:USW_Abilitieso ...
dseg:40AD 43 72 65 61 74 65 20                            cnst_CreateUndead db 'Create '          ; DATA XREF: dseg:USW_Abilitieso
dseg:40B4 55 6E 64 65 61 64 00                            cnst_Undead db 'Undead',0               ; DATA XREF: USW_CreateFXList+19CEo
dseg:40BB 4C 6F 6E 67 20 52 61 6E 67 65 00                cnst_LongRange db 'Long Range',0        ; DATA XREF: dseg:USW_Abilitieso
dseg:40C6 4C 61 6E 64 20 43 6F 72 72 75 70 74 69 6F 6E 00 cnst_LandCorruption db 'Land Corruption',0
dseg:40C6                                                                                         ; DATA XREF: dseg:USW_Abilitieso
dseg:40D6 4D 65 6C 64 20 57 69 74 68 20 4E 6F 64 65 00    cnst_MeldWithNode db 'Meld With Node',0 ; DATA XREF: dseg:USW_Abilitieso
dseg:40E5 4E 6F 6E 20 43 6F 72 70 6F 72 65 61 6C 00       cnst_NonCorporeal db 'Non Corporeal',0  ; DATA XREF: dseg:USW_Abilitieso
dseg:40F3 57 69 6E 64 20 57 61 6C 6B 69 6E 67 00          cnst_WindWalking db 'Wind Walking',0    ; DATA XREF: dseg:USW_Abilitieso ...
dseg:4100 52 65 67 65 6E 65 72 61 74 69 6F 6E 00          cnst_Regeneration db 'Regeneration',0   ; DATA XREF: dseg:USW_Abilitieso ...
dseg:410D 50 75 72 69 66 79 00                            cnst_Purify db 'Purify',0               ; DATA XREF: dseg:USW_Abilitieso
dseg:4114 4E 65 67 61 74 65 20                            cnst_Negate1stStrike db 'Negate '       ; DATA XREF: dseg:USW_Abilitieso
dseg:411B 46 69 72 73 74 20 53 74 72 69 6B 65 00          cnst_FirstStrike db 'First Strike',0    ; DATA XREF: dseg:USW_ATK_Flagso
dseg:4128 4C 75 63 6B 79 00                               cnst_Lucky db 'Lucky',0                 ; DATA XREF: dseg:USW_Attributes_1o ...
dseg:412E 50 6F 69 73 6F 6E 20 49 6D 6D 75 6E 69 74 79 00 cnst_PoisonImmunity db 'Poison Immunity',0
dseg:412E                                                                                         ; DATA XREF: dseg:USW_Attributes_1o
dseg:413E 46 69 72 65 20 49 6D 6D 75 6E 69 74 79 00       cnst_FireImmunity db 'Fire Immunity',0  ; DATA XREF: dseg:USW_Attributes_1o
dseg:414C 53 74 6F 6E 69 6E 67 20 49 6D 6D 75 6E 69 74 79+cnst_StoningImmunity db 'Stoning Immunity',0
dseg:414C 00                                                                                      ; DATA XREF: dseg:USW_Attributes_1o
dseg:415D 57 65 61 70 6F 6E 20 49 6D 6D 75 6E 69 74 79 00 cnst_WeaponImmunity db 'Weapon Immunity',0
dseg:415D                                                                                         ; DATA XREF: dseg:USW_Attributes_1o
dseg:416D 4D 69 73 73 69 6C 65 73 20 49 6D 6D 75 6E 69 74+cnst_MissileImmunity db 'Missiles Immunity',0
dseg:416D 79 00                                                                                   ; DATA XREF: dseg:USW_Attributes_1o
dseg:417F 49 6C 6C 75 73 69 6F 6E 73 20 49 6D 6D 75 6E 69+cnst_IllusionsImm db 'Illusions Immunity',0
dseg:417F 74 79 00                                                                                ; DATA XREF: dseg:USW_Attributes_1o
dseg:4192 43 6F 6C 64 20 49 6D 6D 75 6E 69 74 79 00       cnst_ColdImmunity db 'Cold Immunity',0  ; DATA XREF: dseg:USW_Attributes_1o
dseg:41A0 4D 61 67 69 63 20 49 6D 6D 75 6E 69 74 79 00    cnst_MagicImmunity db 'Magic Immunity',0
dseg:41A0                                                                                         ; DATA XREF: dseg:USW_Attributes_1o ...
dseg:41AF 44 65 61 74 68 20 49 6D 6D 75 6E 69 74 79 00    cnst_DeathImmunity db 'Death Immunity',0
dseg:41AF                                                                                         ; DATA XREF: dseg:USW_Attributes_1o
dseg:41BE 48 65 61 6C 69 6E 67 20                         cnst_HealingSpell db 'Healing '         ; DATA XREF: dseg:USW_Attributes_2o
dseg:41C6 53 70 65 6C 6C 00                               cnst_Spell_2 db 'Spell',0               ; DATA XREF: USW_CreateFXList+B5Bo
dseg:41CC 46 69 72 65 20 42 61 6C 6C 20 53 70 65 6C 6C 00 cnst_FireBallSpell db 'Fire Ball Spell',0
dseg:41CC                                                                                         ; DATA XREF: dseg:USW_Attributes_2o
dseg:41DC 44 6F 6F 6D 62 6F 6C 74 20 53 70 65 6C 6C 00    cnst_DoomboltSpell db 'Doombolt Spell',0
dseg:41DC                                                                                         ; DATA XREF: dseg:USW_Attributes_2o
dseg:41EB 49 6D 6D 6F 6C 61 74 69 6F 6E 00                cnst_Immolation db 'Immolation',0       ; DATA XREF: dseg:USW_Attributes_2o ...
dseg:41F6 43 61 75 73 65 20 46 65 61 72 20 53 70 65 6C 6C+cnst_CauseFearSpell db 'Cause Fear Spell',0
dseg:41F6 00                                                                                      ; DATA XREF: dseg:USW_Attributes_2o
dseg:4207 57 65 62 20 53 70 65 6C 6C 00                   cnst_WebSpell db 'Web Spell',0          ; DATA XREF: dseg:USW_Attributes_2o
dseg:4211 52 65 73 69 73 74 61 6E 63 65 20 54 6F 20 41 6C+cnst_Resistance2All db 'Resistance To All',0
dseg:4211 6C 00                                                                                   ; DATA XREF: dseg:USW_Attributes_2o
dseg:4223 48 6F 6C 79 20 42 6F 6E 75                      cnst_HolyBonus db 'Holy Bonu'           ; DATA XREF: dseg:USW_Attributes_2o
dseg:422C 73 00                                           cnst_Ammo_Plural db 's',0               ; DATA XREF: USW_CreateFXList+B98o
dseg:422E 41 72 6D 6F 72 20 50 69 65 72 63 69 6E 67 00    cnst_ArmorPiercing db 'Armor Piercing',0
dseg:422E                                                                                         ; DATA XREF: dseg:USW_ATK_Flagso
dseg:423D 50 6F 69 73 6F 6E 00                            cnst_Poison db 'Poison',0               ; DATA XREF: dseg:USW_ATK_Flagso
dseg:4244 4C 69 66 65 20 53 74 65 61 6C 00                cnst_LifeSteal db 'Life Steal',0        ; DATA XREF: dseg:USW_ATK_Flagso
dseg:424F 41 75 74 6F 6D 61 74 69 63 20 44 61 6D 61 67 65+cnst_AutomaticDmg db 'Automatic Damage',0
dseg:424F 00                                                                                      ; DATA XREF: dseg:USW_ATK_Flagso
dseg:4260 44 65 73 74 72 75 63 74 69 6F 6E 00             cnst_Destruction db 'Destruction',0     ; DATA XREF: dseg:USW_ATK_Flagso ...
dseg:426C 49 6C 6C 75 73 69 6F 6E 00                      cnst_Illusion db 'Illusion',0           ; DATA XREF: dseg:USW_ATK_Flagso
dseg:4275 53 74 6F 6E 69 6E 67 20 54 6F 75 63 68 00       cnst_StoningTouch db 'Stoning Touch',0  ; DATA XREF: dseg:USW_ATK_Flagso
dseg:4283 44 65 61 74 68 20 54 6F 75 63 68 00             cnst_DeathTouch db 'Death Touch',0      ; DATA XREF: dseg:USW_ATK_Flagso
dseg:428F 50 6F 77 65 72 20 44 72 61 69 6E 00             cnst_PowerDrain db 'Power Drain',0      ; DATA XREF: dseg:USW_ATK_Flagso ...
dseg:429B 44 69 73 70 65 6C 20 45 76 69 6C 00             cnst_DispelEvil db 'Dispel Evil',0      ; DATA XREF: dseg:USW_ATK_Flagso
dseg:42A7 43 68 61 6F 73 20 43 68 61 6E 6E 65 6C 00       cnst_ChaosChannel db 'Chaos Channel',0  ; DATA XREF: dseg:USW_ChaosChannelso
dseg:42B5 54 68 72 6F 77 6E 00                            cnst_Thrown db 'Thrown',0               ; DATA XREF: dseg:USW_SR_Attackso
dseg:42BC 46 69 72 65 20 42 72 65 61 74 68 00             cnst_FireBreath db 'Fire Breath',0      ; DATA XREF: dseg:USW_SR_Attackso
dseg:42C8 4C 69 67 68 74 6E 69 6E 67 00                   cnst_Lightning db 'Lightning',0         ; DATA XREF: dseg:USW_SR_Attackso ...
dseg:42D2 53 74 6F 6E 69 6E 67 20 47 61 7A 65 00          cnst_StoningGaze db 'Stoning Gaze',0    ; DATA XREF: dseg:USW_SR_Attackso
dseg:42DF 44 6F 6F 6D 20 47 61 7A 65 00                   cnst_DoomGaze db 'Doom Gaze',0          ; DATA XREF: dseg:USW_SR_Attackso
dseg:42E9 44 65 61 74 68 20 47 61 7A 65 00                cnst_DeathGaze db 'Death Gaze',0        ; DATA XREF: dseg:USW_SR_Attackso
dseg:42F4 47 75 61 72 64 69 61 6E 20 57 69 6E 64 00       cnst_GuardianWind db 'Guardian Wind',0  ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:4302 42 65 72 73 65 72 6B 00                         cnst_Berserk db 'Berserk',0             ; DATA XREF: dseg:USW_UnitEnchantso
dseg:430A 43 6C 6F 61 6B 20 4F 66 20 46 65 61 72 00       cnst_CloakOfFear db 'Cloak Of Fear',0   ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:4318 42 6C 61 63 6B 20 43 68 61 6E 6E 65 6C 73 00    cnst_BlackChannels db 'Black Channels',0
dseg:4318                                                                                         ; DATA XREF: dseg:USW_UnitEnchantso
dseg:4327 57 72 61 69 74 68 20 46 6F 72 6D 00             cnst_WraithForm db 'Wraith Form',0      ; DATA XREF: dseg:USW_UnitEnchantso
dseg:4333 52 65 67 65 6E 65 72 61 74 65 00                cnst_Regenerate db 'Regenerate',0       ; DATA XREF: dseg:USW_UnitEnchantso
dseg:433E 50 61 74 68 20 46 69 6E 64 69 6E 67 00          cnst_PathFinding db 'Path Finding',0    ; DATA XREF: dseg:USW_UnitEnchantso
dseg:434B 57 61 74 65 72 20 57 61 6C 6B 69 6E 67 00       cnst_WaterWalking db 'Water Walking',0  ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:4359 45 6C 65 6D 65 6E 74 61 6C 20 41 72 6D 6F 72 00 cnst_ElementalArmor db 'Elemental Armor',0
dseg:4359                                                                                         ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:4369 52 65 73 69 73 74 20 45 6C 65 6D 65 6E 74 73 00 cnst_ResistElements db 'Resist Elements',0
dseg:4369                                                                                         ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:4379 53 74 6F 6E 65 20 53 6B 69 6E 00                cnst_StoneSkin db 'Stone Skin',0        ; DATA XREF: dseg:USW_UnitEnchantso
dseg:4384 49 72 6F 6E 20 53 6B 69 6E 00                   cnst_IronSkin db 'Iron Skin',0          ; DATA XREF: dseg:USW_UnitEnchantso
dseg:438E 45 6E 64 75 72 61 6E 63 65 00                   cnst_Endurance db 'Endurance',0         ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:4398 53 70 65 6C 6C 20 4C 6F 63 6B 00                cnst_SpellLock db 'Spell Lock',0        ; DATA XREF: dseg:USW_UnitEnchantso
dseg:43A3 46 6C 69 67 68 74 00                            cnst_Flight db 'Flight',0               ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:43AA 52 65 73 69 73 74 20 4D 61 67 69 63 00          cnst_ResistMagic db 'Resist Magic',0    ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:43B7 46 6C 61 6D 65 20 42 6C 61 64 65 00             cnst_FlameBlade db 'Flame Blade',0      ; DATA XREF: dseg:USW_UnitEnchantso
dseg:43C3 45 6C 64 72 69 74 63 68 20 57 65 61 70 6F 6E 00 cnst_EldritchWeapon db 'Eldritch Weapon',0
dseg:43C3                                                                                         ; DATA XREF: dseg:USW_UnitEnchantso
dseg:43D3 54 72 75 65 20 53 69 67 68 74 00                cnst_TrueSight db 'True Sight',0        ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:43DE 48 6F 6C 79 20 57 65 61 70 6F 6E 00             cnst_HolyWeapon db 'Holy Weapon',0      ; DATA XREF: dseg:USW_UnitEnchantso
dseg:43EA 48 65 72 6F 69 73 6D 00                         cnst_Heroism db 'Heroism',0             ; DATA XREF: dseg:USW_UnitEnchantso
dseg:43F2 42 6C 65 73 73 00                               cnst_Bless db 'Bless',0                 ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:43F8 4C 69 6F 6E 20 48 65 61 72 74 00                cnst_LionHeart db 'Lion Heart',0        ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:4403 47 69 61 6E 74 20 53 74 72 65 6E 67 74 68 00    cnst_GiantStrength db 'Giant Strength',0
dseg:4403                                                                                         ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:4412 50 6C 61 6E 61 72 20 54 72 61 76 65 6C 00       cnst_PlanarTravel db 'Planar Travel',0  ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:4420 48 6F 6C 79 20 41 72 6D 6F 72 00                cnst_HolyArmor db 'Holy Armor',0        ; DATA XREF: dseg:USW_UnitEnchantso
dseg:442B 52 69 67 68 74 65 6F 75 73 6E 65 73 73 00       cnst_Righteousness db 'Righteousness',0 ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:4439 49 6E 76 75 6C 6E 65 72 61 62 69 6C 69 74 79 00 cnst_Invulnerability db 'Invulnerability',0
dseg:4439                                                                                         ; DATA XREF: dseg:USW_UnitEnchantso ...
dseg:4449 56 65 72 74 69 67 6F 00                         cnst_Vertigo db 'Vertigo',0             ; DATA XREF: dseg:USW_Combat_FXo
dseg:4451 43 6F 6E 66 75 73 69 6F 6E 00                   cnst_Confusion db 'Confusion',0         ; DATA XREF: dseg:USW_Combat_FXo
dseg:445B 57 68 69 72 6C 77 69 6E 64 00                   cnst_Whirlwind db 'Whirlwind',0         ; DATA XREF: dseg:USW_Combat_FXo
dseg:4465 4D 69 6E 64 20 53 74 6F 72 6D 00                cnst_MindStorm db 'Mind Storm',0        ; DATA XREF: dseg:USW_Combat_FXo
dseg:4470 53 68 61 74 74 65 72 00                         cnst_Shatter db 'Shatter',0             ; DATA XREF: dseg:USW_Combat_FXo
dseg:4478 57 65 61 6B 6E 65 73 73 00                      cnst_Weakness db 'Weakness',0           ; DATA XREF: dseg:USW_Combat_FXo
dseg:4481 42 6C 61 63 6B 20 53 6C 65 65 70 00             cnst_BlackSleep db 'Black Sleep',0      ; DATA XREF: dseg:USW_Combat_FXo
dseg:448D 57 61 72 70 20 43 72 65 61 74 75 72 65 00       cnst_WarpCreature db 'Warp Creature',0  ; DATA XREF: dseg:USW_Combat_FXo
dseg:449B 4D 69 6E 64 20 54 77 69 73 74 00                cnst_MindTwist db 'Mind Twist',0        ; DATA XREF: dseg:USW_Combat_FXo
dseg:44A6 48 61 73 74 65 00                               cnst_Haste db 'Haste',0                 ; DATA XREF: dseg:USW_Combat_FXo ...
dseg:44AC 57 65 62 00                                     cnst_Web db 'Web',0                     ; DATA XREF: dseg:USW_Combat_FXo
dseg:44B0 43 72 65 61 74 75 72 65 20 42 69 6E 64 69 6E 67+cnst_CreatureBinding db 'Creature Binding',0
dseg:44B0 00                                                                                      ; DATA XREF: dseg:USW_Combat_FXo
dseg:44C1 50 6F 73 65 73 73 69 6F 6E 00                   cnst_Possession db 'Posession',0        ; DATA XREF: dseg:USW_Combat_FXo
dseg:44CB 4D 65 72 67 69 6E 67 00                         cnst_Merging db 'Merging',0             ; DATA XREF: dseg:USW_Item_UEso ...
dseg:44D3 52 65 63 72 75 69 74 00                         cnst_UnitLevel_1 db 'Recruit',0         ; DATA XREF: dseg:USW_UnitLevels@o
dseg:44DB 52 65 67 75 6C 61 72 00                         cnst_UnitLevel_2 db 'Regular',0         ; DATA XREF: dseg:USW_UnitLevels@o
dseg:44E3 56 65 74 65 72 61 6E 00                         cnst_UnitLevel_3 db 'Veteran',0         ; DATA XREF: dseg:USW_UnitLevels@o
dseg:44EB 45 6C 69 74 65 00                               cnst_UnitLevel_4 db 'Elite',0           ; DATA XREF: dseg:USW_UnitLevels@o
dseg:44F1 55 6C 74 72 61 20 45 6C 69 74 65 00             cnst_UnitLevel_5 db 'Ultra Elite',0     ; DATA XREF: dseg:USW_UnitLevels@o
dseg:44FD 43 68 61 6D 70 69 6F 6E 00                      cnst_UnitLevel_6 db 'Champion',0        ; DATA XREF: dseg:USW_UnitLevels@o ...
dseg:4506 48 65 72 6F 00                                  cnst_HeroLevel_1 db 'Hero',0            ; DATA XREF: dseg:USW_HeroLevels@o
dseg:450B 4D 79 72 6D 69 64 6F 6E 00                      cnst_HeroLevel_2 db 'Myrmidon',0        ; DATA XREF: dseg:USW_HeroLevels@o
dseg:4514 43 61 70 74 61 69 6E 00                         cnst_HeroLevel_3 db 'Captain',0         ; DATA XREF: dseg:USW_HeroLevels@o
dseg:451C 43 6F 6D 6D 61 6E 64 65 72 00                   cnst_HeroLevel_4 db 'Commander',0       ; DATA XREF: dseg:USW_HeroLevels@o
dseg:4526 4C 6F 72 64 00                                  cnst_HeroLevel_6 db 'Lord',0            ; DATA XREF: dseg:USW_HeroLevels@o
dseg:452B 47 72 61 6E 64 20 4C 6F 72 64 00                cnst_HeroLevel_7 db 'Grand Lord',0      ; DATA XREF: dseg:USW_HeroLevels@o
dseg:4536 53 75 70 65 72 20 48 65 72 6F 00                cnst_HeroLevel_8 db 'Super Hero',0      ; DATA XREF: dseg:USW_HeroLevels@o
dseg:4541 44 65 6D 69 2D 47 6F 64 00                      cnst_HeroLevel_9 db 'Demi-God',0        ; DATA XREF: dseg:USW_HeroLevels@o
dseg:454A 49 00                                           cnst_Rank_I db 'I',0                    ; DATA XREF: USW_CreateFXList:loc_679B7o ...
dseg:454C 49 49 00                                        cnst_Rank_II db 'II',0                  ; DATA XREF: USW_CreateFXList+26o ...
dseg:454F 49 49 49 00                                     cnst_Rank_III db 'III',0                ; DATA XREF: USW_CreateFXList+35o ...
dseg:4553 49 56 00                                        cnst_Rank_IV db 'IV',0                  ; DATA XREF: USW_CreateFXList+44o ...
dseg:4556 20 28 00                                        cnst_SpaceOpenBrace2 db ' (',0          ; DATA XREF: USW_CreateFXList+108o ...
dseg:4556                                                                                         ; should use dseg:2c40
dseg:4559 20 65 70                                        cnst_ExperiencePts db ' ep'             ; DATA XREF: USW_CreateFXList+159o
dseg:455C 29 00                                           cnst_ClosingBrace_3 db ')',0            ; DATA XREF: USW_CreateFXList+9F6o ...
dseg:455C                                                                                         ; should use dseg:3428
dseg:455E 4C 65 61 64 65 72 73 68 69 70 00                cnst_Leadership db 'Leadership',0       ; DATA XREF: USW_CreateFXList:loc_67E4Bo ...
dseg:4569 7D 00                                           cnst_SuperAbStar db '}',0               ; DATA XREF: USW_CreateFXList+4FAo ...
dseg:456B 4C 65 67 65 6E 64 61 72 79 00                   cnst_Legendary db 'Legendary',0         ; DATA XREF: USW_CreateFXList:loc_67EC8o ...
dseg:4575 42 6C 61 64 65 6D 61 73 74 65 72 00             cnst_Blademaster db 'Blademaster',0     ; DATA XREF: USW_CreateFXList:loc_67F2Ao ...
dseg:4581 41 72 6D 73 6D 61 73 74 65 72 00                cnst_Armsmaster db 'Armsmaster',0       ; DATA XREF: USW_CreateFXList:loc_67F91o ...
dseg:458C 43 6F 6E 73 74 69 74 75 74 69 6F 6E 00          cnst_Constitution db 'Constitution',0   ; DATA XREF: USW_CreateFXList:loc_6800Co ...
dseg:4599 4D 69 67 68 74 00                               cnst_Might db 'Might',0                 ; DATA XREF: USW_CreateFXList:loc_68018o ...
dseg:459F 41 72 63 61 6E 65 20 50 6F 77 65 72 00          cnst_ArcanePower db 'Arcane Power',0    ; DATA XREF: USW_CreateFXList:loc_68024o ...
dseg:45AC 53 61 67 65 00                                  cnst_Sage_2 db 'Sage',0                 ; DATA XREF: USW_CreateFXList:loc_68030o ...
dseg:45AC                                                                                         ; should use dseg:21e3
dseg:45B1 20 00                                           cnst_Space_3 db ' ',0                   ; DATA XREF: USW_CreateFXList+6A8o ...
dseg:45B1                                                                                         ; should use dseg:2ad9
dseg:45B3 20 72 70 00                                     cnst_ResearchPoints db ' rp',0          ; DATA XREF: USW_CreateFXList+706o
dseg:45B7 50 72 61 79 65 72 6D 61 73 74 65 72 00          cnst_Prayermaster db 'Prayermaster',0   ; DATA XREF: USW_CreateFXList:loc_68127o ...
dseg:45C4 41 67 69 6C 69 74 79 00                         cnst_Agility db 'Agility',0             ; DATA XREF: USW_CreateFXList:loc_68131o ...
dseg:45CC 43 68 61 72 6D 65 64 00                         cnst_Charmed db 'Charmed',0             ; DATA XREF: USW_CreateFXList:loc_681B4o
dseg:45D4 4E 6F 62 6C 65 00                               cnst_Noble db 'Noble',0                 ; DATA XREF: USW_CreateFXList:loc_681B9o
dseg:45DA 20                                              cnst_SpacePlus db ' '                   ; DATA XREF: USW_CreateFXList+865o
dseg:45DB 2B 00                                           cnst_Plus_1 db '+',0                    ; DATA XREF: USW_CreateFXList+C4Ao ...
dseg:45DD 43 61 73 74 65 72 00                            cnst_Caster db 'Caster',0               ; DATA XREF: USW_CreateFXList:loc_682B2o ...
dseg:45E4 20 6D 70 00                                     cnst_Space_mp db ' mp',0                ; DATA XREF: USW_CreateFXList+A33o ...
dseg:45E8 52 6F 63 6B 00                                  cnst_Rock db 'Rock',0                   ; DATA XREF: USW_CreateFXList+B0Fo
dseg:45ED 41 72 72 6F 77 00                               cnst_Arrow db 'Arrow',0                 ; DATA XREF: USW_CreateFXList+B38o
dseg:45F3 42 75 6C 6C 65 74 00                            cnst_Bullet db 'Bullet',0               ; DATA XREF: USW_CreateFXList:loc_684DDo
dseg:45FA 20 78 00                                        cnst_TimesSign db ' x',0                ; DATA XREF: USW_CreateFXList:loc_68550o ...
dseg:45FD 20 54 6F 20 48 69 74 00                         cnst_ToHit db ' To Hit',0               ; DATA XREF: USW_CreateFXList+CD2o ...
dseg:4605 43 6F 6E 73 74 72 75 63 74 69 6F 6E 00          cnst_Construction db 'Construction',0   ; DATA XREF: USW_CreateFXList+D0Do ...
dseg:4612 53 63 6F 75 74 69 6E 67 00                      cnst_Scouting db 'Scouting',0           ; DATA XREF: USW_CreateFXList:loc_6870Ao ...
dseg:461B 54 65 6C 65 70 6F 72 74 69 6E 67 00             cnst_Teleporting db 'Teleporting',0     ; DATA XREF: USW_CreateFXList+E73o ...
dseg:4627 50 61 74 68 66 69 6E 64 69 6E 67 00             cnst_Pathfinding db 'Pathfinding',0     ; DATA XREF: dseg:ITEM_ViewTextso ...
dseg:4633 4D 6F 75 6E 74 61 69 6E 65 65 72 00             cnst_Moutaineer db 'Mountaineer',0      ; DATA XREF: USW_CreateFXList+F35o ...
dseg:463F 46 6F 72 65 73 74 65 72 00                      cnst_Forester db 'Forester',0           ; DATA XREF: USW_CreateFXList+F8Fo ...
dseg:4648 53 75 6D 6D 6F 6E 20 44 65 6D 6F 6E 73 00       cnst_SummonDemons db 'Summon Demons',0  ; DATA XREF: USW_CreateFXList:loc_68A32o
dseg:4656 53 74 61 73 69 73 00                            cnst_Stasis db 'Stasis',0               ; DATA XREF: USW_CreateFXList+1A37o
dseg:465D 56 61 6D 70 69 72 69 63 00                      cnst_Vampiric db 'Vampiric',0           ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4666 2C 20 6C 69 66 65 20 64 72 61 69 6E 69 6E 67 00 cnst_VampiricText db ', life draining',0
dseg:4666                                                                                         ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4676 2C 20 61 72 6D 6F 72 20 70 69 65 72 63 69 6E 67+cnst_LightningText db ', armor piercing',0
dseg:4676 00                                                                                      ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4687 43 6C 6F 61 6B 20 6F 66 20 46 65 61 72 00       cnst_CloakofFear db 'Cloak of Fear',0   ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4695 2C 20 61 73 20 63 6C 6F 61 6B 20 6F 66 20 66 65+cnst_CoF_Text db ', as cloak of fear spell',0
dseg:4695 61 72 20 73 70 65 6C 6C 00                                                              ; DATA XREF: dseg:ITEM_ViewTextso
dseg:46AE 2C 20 72 65 73 69 73 74 20 6F 72 20 62 65 20 64+cnst_DestructionText db ', resist or be disintegrated',0
dseg:46AE 69 73 69 6E 74 65 67 72 61 74 65 64 00                                                  ; DATA XREF: dseg:ITEM_ViewTextso
dseg:46CB 57 72 61 69 74 68 66 6F 72 6D 00                cnst_Wraithform db 'Wraithform',0       ; DATA XREF: dseg:ITEM_ViewTextso
dseg:46D6 2C 20 61 73 20 77 72 61 69 74 68 66 6F 72 6D 20+cnst_WraithformText db ', as wraithform spell',0
dseg:46D6 73 70 65 6C 6C 00                                                                       ; DATA XREF: dseg:ITEM_ViewTextso
dseg:46EC 2C 20 61 73 20 72 65 67 65 6E 65 72 61 74 69 6F+cnst_RegenerateText db ', as regeneration spell',0
dseg:46EC 6E 20 73 70 65 6C 6C 00                                                                 ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4704 2C 20 61 73 20 70 61 74 68 66 69 6E 64 69 6E 67+cnst_PF_Text db ', as pathfinding spell',0
dseg:4704 20 73 70 65 6C 6C 00                                                                    ; DATA XREF: dseg:ITEM_ViewTextso
dseg:471B 2C 20 61 73 20 77 61 74 65 72 20 77 61 6C 6B 69+cnst_WaterWalkText db ', as water walking spell',0
dseg:471B 6E 67 20 73 70 65 6C 6C 00                                                              ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4734 2C 20 61 73 20 72 65 73 69 73 74 20 65 6C 65 6D+cnst_ResistElemText db ', as resist elements spell',0
dseg:4734 65 6E 74 73 20 73 70 65 6C 6C 00                                                        ; DATA XREF: dseg:ITEM_ViewTextso
dseg:474F 2C 20 61 73 20 65 6C 65 6D 65 6E 74 61 6C 20 61+cnst_ElemArmorText db ', as elemental armor spell',0
dseg:474F 72 6D 6F 72 20 73 70 65 6C 6C 00                                                        ; DATA XREF: dseg:ITEM_ViewTextso
dseg:476A 44 6F 6F 6D 00                                  cnst_Doom db 'Doom',0                   ; DATA XREF: dseg:ITEM_ViewTextso
dseg:476F 2C 20 69 67 6E 6F 72 65 73 20 61 72 6D 6F 72 00 cnst_DoomText db ', ignores armor',0    ; DATA XREF: dseg:ITEM_ViewTextso
dseg:477F 2C 20 61 73 20 73 77 69 66 74 6E 65 73 73 20 73+cnst_EnduranceText db ', as swiftness spell',0
dseg:477F 70 65 6C 6C 00                                                                          ; DATA XREF: dseg:ITEM_ViewTextso
dseg:477F                                                                                         ; BUG: there is no swiftness spell in the game
dseg:4794 2C 20 64 6F 75 62 6C 65 73 20 61 74 74 61 63 6B+cnst_HasteText db ', doubles attacks',0 ; DATA XREF: dseg:ITEM_ViewTextso
dseg:47A6 2C 20 61 73 20 74 68 65 20 69 6E 76 69 73 69 62+cnst_InvisText db ', as the invisibility spell',0
dseg:47A6 69 6C 69 74 79 20 73 70 65 6C 6C 00                                                     ; DATA XREF: dseg:ITEM_ViewTextso
dseg:47C2 44 65 61 74 68 00                               cnst_Death db 'Death',0                 ; DATA XREF: dseg:ITEM_ViewTextso
dseg:47C2                                                                                         ; should use dseg:26fa
dseg:47C8 2C 20 73 61 76 65 20 61 74 20 2D 33 20 6F 72 20+cnst_DeathText db ', save at -3 or die',0
dseg:47C8 64 69 65 00                                                                             ; DATA XREF: dseg:ITEM_ViewTextso
dseg:47DC 2C 20 61 73 20 66 6C 69 67 68 74 20 73 70 65 6C+cnst_FlightText db ', as flight spell',0
dseg:47DC 6C 00                                                                                   ; DATA XREF: dseg:ITEM_ViewTextso
dseg:47EE 2C 20 61 73 20 72 65 73 69 73 74 20 6D 61 67 69+cnst_ResMagicText db ', as resist magic spell',0
dseg:47EE 63 20 73 70 65 6C 6C 00                                                                 ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4806 2C 20 61 73 20 6D 61 67 69 63 20 69 6D 6D 75 6E+cnst_MagicImmText db ', as magic immunity spell',0
dseg:4806 69 74 79 20 73 70 65 6C 6C 00                                                           ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4820 2C 20 74 61 72 67 65 74 20 6C 6F 73 65 73 20 32+cnst_PDrainText db ', target loses 2-20 mana',0
dseg:4820 2D 32 30 20 6D 61 6E 61 00                                                              ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4839 48 6F 6C 79 20 41 76 65 6E 67 65 72 00          cnst_HolyAvenger db 'Holy Avenger',0    ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4846 2C 20 61 73 20 62 6C 65 73 73 20 26 20 64 69 73+cnst_HolyAvengerText db ', as bless & dispel evil spell',0
dseg:4846 70 65 6C 20 65 76 69 6C 20 73 70 65 6C 6C 00                                            ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4865 2C 20 61 73 20 74 72 75 65 20 73 69 67 68 74 20+cnst_TrueSightText db ', as true sight spell',0
dseg:4865 73 70 65 6C 6C 00                                                                       ; DATA XREF: dseg:ITEM_ViewTextso
dseg:487B 50 68 61 6E 74 61 73 6D 61 6C 00                cnst_Phantasmal db 'Phantasmal',0       ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4886 2C 20 69 67 6E 6F 72 65 73 20 61 72 6D 6F 72 20+cnst_PhantasmalText db ', ignores armor & illusion attacks',0
dseg:4886 26 20 69 6C 6C 75 73 69 6F 6E 20 61 74 74 61 63+                                        ; DATA XREF: dseg:ITEM_ViewTextso
dseg:48A9 46 6C 61 6D 69 6E 67 00                         cnst_Flaming db 'Flaming',0             ; DATA XREF: dseg:ITEM_ViewTextso
dseg:48B1 2C 20 2B 33 20 61 74 74 61 63 6B 20 73 74 72 65+cnst_FlamingText db ', +3 attack strength',0
dseg:48B1 6E 67 74 68 00                                                                          ; DATA XREF: dseg:ITEM_ViewTextso
dseg:48C6 2C 20 61 73 20 62 6C 65 73 73 20 73 70 65 6C 6C+cnst_BlessText db ', as bless spell',0  ; DATA XREF: dseg:ITEM_ViewTextso
dseg:48D7 2C 20 61 73 20 6C 69 6F 6E 20 68 65 61 72 74 20+cnst_LionheartText db ', as lion heart spell',0
dseg:48D7 73 70 65 6C 6C 00                                                                       ; DATA XREF: dseg:ITEM_ViewTextso
dseg:48ED 53 74 6F 6E 69 6E 67 00                         cnst_Stoning db 'Stoning',0             ; DATA XREF: dseg:ITEM_ViewTextso
dseg:48F5 2C 20 72 65 73 69 73 74 20 61 74 20 2D 31 20 6F+cnst_StoningText db ', resist at -1 or be stoned',0
dseg:48F5 72 20 62 65 20 73 74 6F 6E 65 64 00                                                     ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4911 2C 20 61 73 20 70 6C 61 6E 61 72 20 74 72 61 76+cnst_PlanarTrvlText db ', as planar travel spell',0
dseg:4911 65 6C 20 73 70 65 6C 6C 00                                                              ; DATA XREF: dseg:ITEM_ViewTextso
dseg:492A 2C 20 61 73 20 6D 65 72 67 69 6E 67 20 73 70 65+cnst_MergingText db ', as merging spell',0
dseg:492A 6C 6C 00                                                                                ; DATA XREF: dseg:ITEM_ViewTextso
dseg:492A                                                                                         ; BUG: there is no merging spell in the game
dseg:493D 2C 20 61 73 20 72 69 67 68 74 65 6F 75 73 6E 65+cnst_RightsnssText db ', as righteousness spell',0
dseg:493D 73 73 20 73 70 65 6C 6C 00                                                              ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4956 2C 20 61 73 20 69 6E 76 75 6C 6E 65 72 61 62 69+cnst_InvulnText db ', as invulnerability spell',0
dseg:4956 6C 69 74 79 20 73 70 65 6C 6C 00                                                        ; DATA XREF: dseg:ITEM_ViewTextso
dseg:4971 2C 20 61 73 20 67 75 61 72 64 69 61 6E 20 77 69+cnst_GWindText db ', as guardian wind spell',0
dseg:4971 6E 64 20 73 70 65 6C 6C 00                                                              ; DATA XREF: dseg:ITEM_ViewTextso
dseg:498A 2C 20 61 73 20 67 69 61 6E 74 20 73 74 72 65 6E+cnst_GStrText db ', as giant strength spell',0
dseg:498A 67 74 68 20 73 70 65 6C 6C 00                                                           ; DATA XREF: dseg:ITEM_ViewTextso
dseg:49A4 20 41 74 74 61 63 6B 00                         cnst_Space_ATK db ' Attack',0           ; DATA XREF: ITEM_GetPowerNames+A4o
dseg:49A4                                                                                         ; could use dseg:57b5
dseg:49AC 20 44 65 66 65 6E 73 65 00                      cnst_Space_DEF db ' Defense',0          ; DATA XREF: ITEM_GetPowerNames+1ACo
dseg:49B5 20 4D 6F 76 65 6D 65 6E 74 00                   cnst_Space_Moves db ' Movement',0       ; DATA XREF: ITEM_GetPowerNames+235o
dseg:49BF 20 52 65 73 69 73 74 61 6E 63 65 00             cnst_Space_RES db ' Resistance',0       ; DATA XREF: ITEM_GetPowerNames+2B9o
dseg:49BF                                                                                         ; could use dseg:57a7
dseg:49CB 20 53 70 65 6C 6C 20 53 6B 69 6C 6C 00          cnst_Space_Skill db ' Spell Skill',0    ; DATA XREF: ITEM_GetPowerNames+33Do
dseg:49D8 2D 00                                           cnst_MinusSign db '-',0                 ; DATA XREF: ITEM_GetPowerNames+36Fo
dseg:49DA 20 53 70 65 6C 6C 20 53 61 76 65 00             cnst_Space_SAVE db ' Spell Save',0      ; DATA XREF: ITEM_GetPowerNames+3C1o
dseg:49E6 58 00                                           cnst_CapTimesSign db 'X',0              ; DATA XREF: Unit_List_Window_Pup+252o ...
dseg:49E6                                                                                         ; could use dseg:2a8a

dseg:49E8 52 61 6D 70 61 67 69 6E 67 20 4D 6F 6E 73 74 65+aRampagingMonsters db 'Rampaging Monsters',0
dseg:49FB 52 61 69 64 65 72 73 00                         cnst_Raiders db 'Raiders',0             ; DATA XREF: Draw_Unit_List_Window_Pup:loc_6A745o
dseg:4A03 20 55 6E 69 74 73 00                            aUnits_0 db ' Units',0
dseg:4A0A                                                 ; char cnst_HOTKEY_EscC[]
dseg:4A0A 1B 00                                           cnst_HOTKEY_EscC db 1Bh,0               ; DATA XREF: IDK_ULW_Add_Fields+A7o
dseg:4A0A                                                                                         ; should use dseg:2c56

