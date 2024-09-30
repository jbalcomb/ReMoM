

466 bytes






according to the code in Set_Default_Game_Settings()
magic_set.Using_Saves is an array of 8 elements of 16-bit values


according to the code in Set_Default_Game_Settings()
magic_set.HallofFame_Scores is an array of 10 elements of 16-bit values
magic_set.HallofFame_Races is an array of 10 elements of 16-bit values
magic_set.HallofFame_Names is an array of 10 elements of 20 characters c-strings





...as seen on Settings Screen...
    "Movement Animations" is out of order
    (New) Game Options, which are actually per SAVE_GAM
        difficulty = 0  "Intro"
        opponents = 4   "Four"
        magic = 1       "Weak"
        landsize = 1    "
    (Hardware) Configuration Settings, per CONFIG.MOM, from INSTALL.EXE
        input_type = 1
        sound_channels = 2



#### Set_Default_Game_Settings()
    magic_set.SoundFX = 1
    magic_set.BG_Music = 1
    magic_set.Event_Music = 1
    magic_set.Enemy_Moves = 1
    magic_set.Enemy_Spells = 1
    magic_set.Random_Events = 1
    magic_set.EoT_Wait = 1
    magic_set.end_of_turn_summary = 1
    magic_set.Raze_City = 1
    magic_set.Spell_Book_Order = 1
    magic_set.Strategic_Combat = 0
    magic_set.Spell_Anims = 1
    magic_set.City_Spell_Events = 1
    magic_set.Ovl_Spell_Events = 1
    magic_set.Summoning_Events = 1
    magic_set.Auto_Unit_Info = 1
    magic_set.Show_Node_Owners = 1
    magic_set.Expanding_Help = 1
    magic_set.Movement_Anims = 1
    magic_set.Difficulty = 0
    magic_set.Opponents = 4
    magic_set.MagicPower = 1
    magic_set.LandSize = 1
    magic_set.Input_Type = 1
    magic_set.Snd_Channels = 2



MGC seg049
o49p01  Settings Screen
o49p02  Settings Screen Draw
o49p03  Load_MAGIC_SET()        MoO2 Load_Game_Settings()
o49p04  Default_MAGIC_SET()     MoO2 Set_Default_Game_Settings()
o49p05  Load Settings Screen Help



## MoO2
Save_Game_Settings_
file_pointer = fopen("MOX.SET","wb");
fwrite(_settings, 1, 553, file_pointer);
fclose(file_pointer);
return ST_TRUE

Set_Default_Game_Settings_()
Write_Game_Settings_()




## Settings Screen



XREF
    WZD ovr125
        Settings_Screen()
        Settings_Screen_Draw()
        Set_Default_Game_Settings()
        Settings_Screen_Help()  HLP_Load_Settings()
    WZD ovr160
        Load_Screen()

only shares the mouse_list and the screen jump flag

WZD ovr160
    Load_Screen()
    Load_Screen_Draw()
    Load_Screen_Help()
    Loaded_Game_Update()

LOAD.LBX
MAGIC.SET
hlpentry.lbx



/*
    Initialized Data
*/

WZD dseg:60D4
 01 00 00 00 00 00 00 00+GUI_Settings_Wnd GUI_WINDOW <Crsr_Normal, 0, 0, 0, 319, 199>

dseg:60D4 3F 01 C7 00                                                     ; DATA XREF: Settings_Screen+112o
dseg:60D4                                                                 ; Load_Screen+1FDo


dseg:60E0 FF FF                   GUI_LoadSave_State dw -1                ; DATA XREF: Settings_Screen+39Fr
dseg:60E0                                                                 ; Settings_Screen:@@Donew
dseg:60E0                                                                 ; Load_Screen+13r
dseg:60E0                                                                 ; Load_Screen:loc_F29ACr
dseg:60E0                                                                 ; Load_Screen+73w
dseg:60E0                                                                 ; Load_Screen+3B0w
dseg:60E0                                                                 ; Load_Screen:loc_F2D33w
dseg:60E0                                                                 ; Load_Screen+40Ew
dseg:60E0                                                                 ; Load_Screen+43Dw
dseg:60E0                                                                 ; Load_Screen+45Bw
dseg:60E0                                                                 ; Load_Screen+48Dr
dseg:60E0                                                                 ; Load_Screen+4C0r
dseg:60E0                                                                 ; 3 indicates returning from the settings screen

dseg:60E2 35 00 40 00 4B 00 66 00+TBL_Settings_Tops dw    53,   64,   75,  102,  113,  124,  135,  153,  146,  157,  167,  163
dseg:60E2 71 00 7C 00 87 00 99 00+                                        ; DATA XREF: Settings_Screen+Do
dseg:60E2 92 00 9D 00 A7 00 A3 00+dw   174

dseg:60FC 62 62 62 62             COL_Settings db 4 dup( 62h)             ; DATA XREF: Settings_Screen_Draw+Br
dseg:60FC                                                                 ; Settings_Screen_Draw+8r

dseg:6100 4C 4F 41 44 2E 4C 42 58 cnst_LOAD_File db 'LOAD.LBX'            ; DATA XREF: Settings_Screen+22o
dseg:6100                                                                 ; Settings_Screen+39o
dseg:6100                                                                 ; Settings_Screen+56o
dseg:6100                                                                 ; Settings_Screen+7Eo
dseg:6100                                                                 ; Settings_Screen+A6o

dseg:6108 00                      cnst_ZeroString_8 db 0                  ; DATA XREF: Settings_Screen+168o
dseg:6108                                                                 ; Settings_Screen+1ABo
dseg:6108                                                                 ; Settings_Screen+1D0o
dseg:6108                                                                 ; Settings_Screen+207o
dseg:6108                                                                 ; Settings_Screen+24Ao
dseg:6108                                                                 ; Load_MAGIC_SET+D8o
dseg:6108                                                                 ; Set_Default_Game_Settings+C6o
dseg:6109                         ; char MAGIC_SET__4[]
dseg:6109 4D 41 47 49 43 2E 53 45+MAGIC_SET__4 db 'MAGIC.SET',0           ; DATA XREF: Settings_Screen+C4o
dseg:6109 54 00                                                           ; Settings_Screen+DEo
dseg:6109                                                                 ; Settings_Screen+2BCo
dseg:6109                                                                 ; Load_MAGIC_SET+Co
dseg:6109                                                                 ; Load_MAGIC_SET+1Bo
dseg:6109                                                                 ; Load_MAGIC_SET+37o
dseg:6109                                                                 ; Load_MAGIC_SET+5Co
dseg:6109                                                                 ; Load_MAGIC_SET+F8o
dseg:6109                                                                 ; should use dseg:28e5
dseg:6113 72 62 00                cnst_RB3 db 'rb',0                      ; DATA XREF: Settings_Screen:loc_A909Ao
dseg:6113                                                                 ; Load_MAGIC_SET:loc_A983Fo
dseg:6113                                                                 ; should use dseg:28b8
dseg:6116 1B 00                   cnst_HOTKEY_Esc1C db 1Bh,0              ; DATA XREF: Settings_Screen+126o
dseg:6116                                                                 ; should use dseg:2c56
dseg:6118 4F 00                   cnst_HOTKEY_O_9 db 'O',0                ; DATA XREF: Settings_Screen+137o
dseg:6118                                                                 ; should use dseg:2c54
dseg:611A 77 62 00                cnst_WB4 db 'wb',0                      ; DATA XREF: Settings_Screen+2B8o
dseg:611A                                                                 ; Load_MAGIC_SET+33o
dseg:611A                                                                 ; Load_MAGIC_SET+F4o

dseg:611D 53 6F 75 6E 64 20 45 66+cnst_Settings_0 db 'Sound Effects',0    ; DATA XREF: Settings_Screen_Draw+27Ao
dseg:612B 42 61 63 6B 67 72 6F 75+cnst_Settings_1 db 'Background Music',0 ; DATA XREF: Settings_Screen_Draw+28Eo
dseg:613C 45 76 65 6E 74 20 4D 75+cnst_Settings_2 db 'Event Music',0      ; DATA XREF: Settings_Screen_Draw+2A2o
dseg:6148 43 69 74 79 20 53 70 65+cnst_Settings_3 db 'City Spell Events',0
dseg:6148 6C 6C 20 45 76 65 6E 74+                                        ; DATA XREF: Settings_Screen_Draw+2B6o
dseg:615A 4F 76 65 72 6C 61 6E 64+cnst_Settings_4 db 'Overland Spell Events',0
dseg:615A 20 53 70 65 6C 6C 20 45+                                        ; DATA XREF: Settings_Screen_Draw+2CAo
dseg:6170 53 75 6D 6D 6F 6E 69 6E+cnst_Settings_5 db 'Summoning Events',0 ; DATA XREF: Settings_Screen_Draw+2DEo
dseg:6181 45 6E 64 20 6F 66 20 54+cnst_Settings_6 db 'End of Turn Summary',0
dseg:6181 75 72 6E 20 53 75 6D 6D+                                        ; DATA XREF: Settings_Screen_Draw+2F2o
dseg:6195 52 61 7A 65 20 43 69 74+cnst_Settings_7 db 'Raze City',0        ; DATA XREF: Settings_Screen_Draw+306o
dseg:619F 52 61 6E 64 6F 6D 20 45+cnst_Settings_8 db 'Random Events',0    ; DATA XREF: Settings_Screen_Draw+31Ao
dseg:61AD 45 6E 64 20 6F 66 20 54+cnst_Settings_9 db 'End of Turn Wait',0 ; DATA XREF: Settings_Screen_Draw+32Eo
dseg:61BE 53 74 72 61 74 65 67 69+cnst_Settings_A db 'Strategic Combat Only',0
dseg:61BE 63 20 43 6F 6D 62 61 74+                                        ; DATA XREF: Settings_Screen_Draw+342o
dseg:61D4 41 75 74 6F 20 55 6E 69+cnst_Settings_B db 'Auto Unit Information',0
dseg:61D4 74 20 49 6E 66 6F 72 6D+                                        ; DATA XREF: Settings_Screen_Draw+356o
dseg:61EA 4D 6F 76 65 6D 65 6E 74+cnst_Settings_C db 'Movement Animations',0
dseg:61EA 20 41 6E 69 6D 61 74 69+                                        ; DATA XREF: Settings_Screen_Draw+36Ao
dseg:61FE 45 6E 65 6D 79 20 4D 6F+cnst_Settings_D db 'Enemy Moves',0      ; DATA XREF: Settings_Screen_Draw+37Eo
dseg:620A 45 6E 65 6D 79 20 53 70+cnst_Settings_E db 'Enemy Spells',0     ; DATA XREF: Settings_Screen_Draw+392o
dseg:6217 53 70 65 6C 6C 20 42 6F+cnst_Settings_F db 'Spell Book Ordering',0
dseg:6217 6F 6B 20 4F 72 64 65 72+                                        ; DATA XREF: Settings_Screen_Draw+3A6o
dseg:622B 53 70 65 6C 6C 20 41 6E+cnst_Settings_10 db 'Spell Animations',0
dseg:622B 69 6D 61 74 69 6F 6E 73+                                        ; DATA XREF: Settings_Screen_Draw+3BAo
dseg:623C 53 68 6F 77 20 4E 6F 64+cnst_Settings_11 db 'Show Node Owners',0
dseg:623C 65 20 4F 77 6E 65 72 73+                                        ; DATA XREF: Settings_Screen_Draw+3CEo
dseg:624D 45 78 70 61 6E 64 69 6E+cnst_Settings_12 db 'Expanding Help',0  ; DATA XREF: Settings_Screen_Draw+3E2o
dseg:625C 53 6F 75 6E 64 20 61 6E+cnst_SettingsGrp_1 db 'Sound and Music',0
dseg:625C 64 20 4D 75 73 69 63 00                                         ; DATA XREF: Settings_Screen_Draw+419o
dseg:626C 4D 65 73 73 61 67 65 73+cnst_SettingsGrp_2 db 'Messages',0      ; DATA XREF: Settings_Screen_Draw+42Do
dseg:6275 43 6F 6D 62 61 74 00    cnst_SettingsGrp_3 db 'Combat',0        ; DATA XREF: Settings_Screen_Draw+441o
dseg:627C 44 69 73 70 6C 61 79 00 cnst_SettingsGrp_4 db 'Display',0       ; DATA XREF: Settings_Screen_Draw+455o

dseg:6284 68 6C 70 65 6E 74 72 79+cnst_HLPENTRY_File4 db 'hlpentry.lbx',0 ; DATA XREF: HLP_Load_Settings+16o

dseg:6291 00                      db    0




/*
    Uninitialized Data
*/


dseg:C980 00 00                   click_idx_SETTINGS dw 0                 ; DATA XREF: Load_Screen+2CDw
dseg:C980                                                                 ; Load_Screen+455r
dseg:C980                                                                 ; Load_Screen_Draw+D9r
dseg:C982 00                      db    0
dseg:C983 00                      db    0
dseg:C984 00                      db    0
dseg:C985 00                      db    0

dseg:C986 00 00 00 00             RP_IMG_GUI_ChkBoxDn@ dw 2 dup(     0)   ; DATA XREF: Settings_Screen+B6w
dseg:C986                                                                 ; array of 2 unused images appended into the sandbox
dseg:C98A 00                      db    0
dseg:C98B 00                      db    0

dseg:C98C 00 00 00 00 00 00       IMG_GUI_ChkBoxNo@ dw 3 dup(     0)      ; DATA XREF: Settings_Screen+8Ew
dseg:C98C                                                                 ; Settings_Screen_Draw+7Fr
dseg:C98C                                                                 ; Settings_Screen_Draw+D5r
dseg:C98C                                                                 ; Settings_Screen_Draw+11Fr
dseg:C98C                                                                 ; Settings_Screen_Draw+15Cr
dseg:C98C                                                                 ; Settings_Screen_Draw+1A5r
dseg:C98C                                                                 ; Settings_Screen_Draw+1FBr
dseg:C98C                                                                 ; Settings_Screen_Draw+236r
dseg:C98C                                                                 ; array of 3 images appended into the sandbox
dseg:C992 00 00 00 00 00 00       IMG_GUI_ChkBoxYes@ dw 3 dup(     0)     ; DATA XREF: Settings_Screen+66w
dseg:C992                                                                 ; Settings_Screen_Draw+91r
dseg:C992                                                                 ; Settings_Screen_Draw+E7r
dseg:C992                                                                 ; Settings_Screen_Draw+131r
dseg:C992                                                                 ; Settings_Screen_Draw+16Er
dseg:C992                                                                 ; Settings_Screen_Draw+1B7r
dseg:C992                                                                 ; Settings_Screen_Draw+20Dr
dseg:C992                                                                 ; Settings_Screen_Draw:loc_A95B4r
dseg:C992                                                                 ; array of 3 images appended into the sandbox
dseg:C998 00 00                   GUI_Settings_Offset dw 0                ; DATA XREF: Settings_Screen:loc_A90CCw
dseg:C998                                                                 ; Settings_Screen+307r
dseg:C998                                                                 ; Settings_Screen+316r
dseg:C998                                                                 ; Settings_Screen+326r
dseg:C998                                                                 ; Settings_Screen_Draw+68r
dseg:C998                                                                 ; Settings_Screen_Draw+BEr
dseg:C998                                                                 ; Settings_Screen_Draw+109r
dseg:C998                                                                 ; Settings_Screen_Draw+146r
dseg:C998                                                                 ; Settings_Screen_Draw+18Er
dseg:C998                                                                 ; Settings_Screen_Draw+1E4r
dseg:C99A                         ; unsigned int word_4343A
dseg:C99A 00 00                   word_4343A dw 0                         ; DATA XREF: Load_Screen+116w
dseg:C99A                                                                 ; Load_Screen_Draw:loc_F3028r
dseg:C99C 00 00                   word_4343C dw 0                         ; DATA XREF: Load_Screen+1C3w
dseg:C99E 00                      db    0
dseg:C99F 00                      db    0
dseg:C9A0 00                      db    0
dseg:C9A1 00                      db    0
dseg:C9A2 00                      db    0
dseg:C9A3 00                      db    0
dseg:C9A4 00                      db    0
dseg:C9A5 00                      db    0
dseg:C9A6 00                      db    0
dseg:C9A7 00                      db    0
dseg:C9A8 00                      db    0
dseg:C9A9 00                      db    0
dseg:C9AA 00                      db    0
dseg:C9AB 00                      db    0
dseg:C9AC 00 00                   word_4344C dw 0                         ; DATA XREF: Load_Screen+289w
dseg:C9AC                                                                 ; Load_Screen+430r
dseg:C9AC                                                                 ; Load_Screen_Draw+14Br
dseg:C9AE 00 00                   hotkey_idx_ZERO dw 0                    ; DATA XREF: Load_Screen+23Fw
dseg:C9AE                                                                 ; Load_Screen+3AAr
dseg:C9AE                                                                 ; Load_Screen_Draw+B2r
dseg:C9B0 00 00                   GUI_SetOKBtn_Index dw 0                 ; DATA XREF: Settings_Screen+153w
dseg:C9B0                                                                 ; Settings_Screen+2B2r
dseg:C9B0                                                                 ; Settings_Screen_Draw+253r
dseg:C9B0                                                                 ; Load_Screen+2ABw
dseg:C9B0                                                                 ; Load_Screen+3E5r
dseg:C9B0                                                                 ; Load_Screen_Draw:loc_F303Cr
dseg:C9B2                         ; int SaveGame_Index
dseg:C9B2 00 00                   SaveGame_Index dw 0                     ; DATA XREF: Load_Screen+1E4w
dseg:C9B2                                                                 ; Load_Screen+32Dw
dseg:C9B2                                                                 ; Load_Screen+379w
dseg:C9B2                                                                 ; Load_Screen+391w
dseg:C9B2                                                                 ; Load_Screen+407r
dseg:C9B2                                                                 ; Load_Screen+419r
dseg:C9B2                                                                 ; Load_Screen_Draw+100r
dseg:C9B4                         ; int word_43454
dseg:C9B4 00 00                   word_43454 dw 0                         ; DATA XREF: Load_Screen+1EAw
dseg:C9B4                                                                 ; Load_Screen+375w
dseg:C9B4                                                                 ; Load_Screen+436r
dseg:C9B4                                                                 ; Load_Screen+443r
dseg:C9B4                                                                 ; Load_Screen_Draw:loc_F2FF8r
dseg:C9B6 00 00                   word_43456 dw 0                         ; DATA XREF: Load_Screen+267w
dseg:C9B6                                                                 ; Load_Screen+401r
dseg:C9B6                                                                 ; Load_Screen_Draw+107r
dseg:C9B8 00 00                   IDK_Load_Screen_Fade_In dw 0            ; DATA XREF: Settings_Screen+273w
dseg:C9B8                                                                 ; Load_Screen:loc_F294Dw
dseg:C9B8                                                                 ; Load_Screen+47Br
dseg:C9B8                                                                 ; set to 0 on entering the settings screen
dseg:C9BA                         ; unsigned int word_4345A
dseg:C9BA 00 00                   word_4345A dw 0                         ; DATA XREF: Load_Screen+FFw
dseg:C9BA                                                                 ; Load_Screen_Draw:loc_F2FE4r
dseg:C9BC                         ; unsigned int S_Img
dseg:C9BC 00 00                   S_Img@ dw 0                             ; DATA XREF: Load_Screen+144w
dseg:C9BC                                                                 ; Load_Screen+2ECr
dseg:C9BE 00 00                   nSaveGam dw 0                           ; DATA XREF: Load_Screen+15Ew
dseg:C9BE                                                                 ; Load_Screen+1BAr
dseg:C9BE                                                                 ; Load_Screen+1C7w
dseg:C9BE                                                                 ; Load_Screen:loc_F2B0Dr
dseg:C9BE                                                                 ; Load_Screen+39Br
dseg:C9BE                                                                 ; Load_Screen_Draw:loc_F2F60r
dseg:C9C0 00 00                   word_43460 dw 0                         ; DATA XREF: Load_Screen+15Bw
dseg:C9C0                                                                 ; Load_Screen_Draw+DFr
dseg:C9C0                                                                 ; Load_Screen_Draw:loc_F2FA0r
dseg:C9C2 00 00                   word_43462 dw 0                         ; DATA XREF: Load_Screen+12Dw
dseg:C9C2                                                                 ; Load_Screen_Draw:loc_F2F3Fr
dseg:C9C4 00 00                   IMG_GUI_SetOKBtn@ dw 0                  ; DATA XREF: Settings_Screen+45w
dseg:C9C4                                                                 ; Settings_Screen_Draw+259r
dseg:C9C4                                                                 ; Settings_Screen_Draw:loc_A95DEr
dseg:C9C4                                                                 ; Load_Screen+E8w
dseg:C9C4                                                                 ; Load_Screen_Draw+18Er
dseg:C9C4                                                                 ; Load_Screen_Draw:loc_F304Fr
dseg:C9C6 00 00                   word_43466 dw 0                         ; DATA XREF: Load_Screen+BAw
dseg:C9C6                                                                 ; Load_Screen_Draw+10Dr
dseg:C9C6                                                                 ; Load_Screen_Draw:loc_F2FCEr
dseg:C9C8 00 00                   word_43468 dw 0                         ; DATA XREF: Load_Screen+A3w
dseg:C9C8                                                                 ; Load_Screen_Draw+B8r
dseg:C9C8                                                                 ; Load_Screen_Draw:loc_F2F79r
dseg:C9CA 00 00                   IMG_GUI_SettingsBG@ dw 0                ; DATA XREF: Settings_Screen+2Ew
dseg:C9CA                                                                 ; Settings_Screen_Draw+1Dr
dseg:C9CA                                                                 ; Load_Screen+8Cw
dseg:C9CA                                                                 ; Load_Screen_Draw+3Fr
