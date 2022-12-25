


# MoM - Game Data
CONFIG.MOM
MAGIC.SET
SAVE.GAM

MAGIC.EXE
WIZARDS.EXE



## Game Options - New Game
    Difficulty
        {Intro, Easy, Normal, Hard, Impossible}
    Opponents
        {One, Two, Three, Four}
    Land Size
        {Small, Medium, Large}
    Magic
        {Weak, Normal, Powerful}


## Settings - Settings Screen

    Sound and Music
        Sound Effects
        Background Music
        Event Music

    Messages
        City Spell Events
        Overland Spell Events
        Summoning Events
        End of Turn Summary
        Raze City

        Random Events
        End of Turn Wait
    
    Combat
        Strategic Combat Only
        Auto Unit Information
        Movement Animations

    Display
        Enemy Moves
        Enemy Spells
        Spell Book Ordering
        Spell Animations
        Show Node Owners
        Expanding Help





CFG  Congiuration
SET  Settings
GAM  Saved Games



Load/Save


Configuration File
CONFIG.MOM
created by INSTALL.EXE
File Size: 20 bytes

Settings File
MAGIC.SET
created by MAGIC.EXE or WIZARDS.EXE
File Size: 466 bytes

Saved-Game File
SAVE.GAM
created by MAGIC.EXE or WIZARDS.EXE
File Size: 123,300 bytes


### Structures:
s_CFG
s_SET
s_GAM




## Settings
The settings structure is in the same order as the Settings Screen, except for Movement Animations.


## Saved Game

123,300 bytes






WZD

dseg:913A 00 00 00 00                 TBL_Premade_Items@ dd 0                 ; 11h LBX_Alloc_Space paragraphs

dseg:9D22 00 00                       grand_vizier dw 0


WZD

WZD o152p01 MoM_Init_Tables()

TBL_Premade_Items@ = Allocate_Space(17)  17 * 16 = 272 bytes









MGC
dseg:87A4 00 00 00 00 00 00 00 00+config_mom s_CONFIG_MOM <0>
WZD
dseg:BD6E 00 00 00 00 00 00 00 00+    config_mom CONFIG_DATA <0>

MGC
dseg:87D4 00 00 00 00 00 00 00 00+magic_set s_MAGIC_SET <0>
WZD
dseg:BD9E 00 00 00 00 00 00 00 00+    magic_set SETTINGS <0>



Set_Default_Game_Settings_()  ~==  GAME_ResetSettings()



MGC
dseg:87C4 00 00 00 00 00 00 00 00+game_data s_GAME_DATA <0>
WZD
dseg:BD8E 00 00 00 00 00 00 00 00+    Game GAME_DATA <0>

00000000 struc GAME_DATA ; (sizeof=0x10, standard type)
00000000 Current_Unit dw ?
00000002 Turn dw ?
00000004 Total_Units dw ?
00000006 Total_Cities dw ?
00000008 Difficulty dw ?
0000000A Magic dw ?
0000000C Land_Size dw ?
0000000E Total_Wizards dw ?
00000010 ends GAME_DATA





Load, Save; Read, Write; Defaults;
















## MoO2 XREF


Module: MOX

_save_game_description
data (0 bytes)
Address: 02:001916BE

Get_Saved_Game_Descriptions_()




#### MoO2  Module: FILEDEF

Load_Game_
Address: 01:00010E2F

Save_Game_
Address: 01:0001160B

Save_Game_Settings_
Address: 01:00011BE4

Load_Game_Settings_
Address: 01:00011C39

Read_Settings_From_Save_File_
Address: 01:00011EAE

Make_Autosave_File_Name_
Address: 01:00012175


#### MoO2  Module: INITGAME

Set_Defaults_
Address: 01:0001279F

Set_Default_Game_Settings_
Address: 01:000127E1

Write_Game_Settings_
Address: 01:00012937


#### MoO2  Module: LOADSAVE

Check_For_Saved_Games_
Address: 01:0007D954

Set_Current_Game_Option_Flags_
Address: 01:0007EFEF

Update_Game_Settings_
Address: 01:0007F14C


#### MoO2  Module: MAINSCR

Toggle_Game_Option_
Address: 01:0008A216



Module: MOX

_settings
data (0 bytes)
Address: 02:00191BDC


Module: FILEDEF
Load_Game_
code (0 bytes)
Address: 01:00010E2F
Locals:

Save_Game_
code (0 bytes)
Address: 01:0001160B
Locals:

Save_Game_Settings_
code (0 bytes)
Address: 01:00011BE4
Locals:

Load_Game_Settings_
code (0 bytes)
Address: 01:00011C39
Locals:

code (0 bytes) Read_Settings_From_Save_File_
Address: 01:00011EAE
Locals:

code (0 bytes) Save_Moo_Custom_Player_Data_
Address: 01:00011F10
Locals:

code (0 bytes) Save_Out_Settings_To_Temp_File_
Address: 01:00011F11
Locals:

code (0 bytes) Get_Game_Type_From_Saved_Game_
Address: 01:00011FCC
Locals:

code (0 bytes) Read_Moo_Custom_Player_Data_
Address: 01:0001202D
Locals:

code (0 bytes) Save_Session_Related_Settings_
Address: 01:00012030
Locals:

code (0 bytes) Copy_Over_Game_Universe_Settings_
Address: 01:000120FA
Locals:

code (0 bytes) Make_Autosave_File_Name_
Address: 01:00012175
Locals:

code (0 bytes) Update_Game_Settings_Version_Number_
Address: 01:00012227
Locals:


Module: INITGAME

	code (0 bytes) Set_Default_Game_Settings_
	Address: 01:000127E1
		Locals:

	code (0 bytes) Write_Game_Settings_
	Address: 01:00012937
		Locals:

	code (0 bytes) Set_Sound_Volume_To_Settings_
	Address: 01:0007A06C
		Locals:

	code (0 bytes) Write_Language_Setting_File_
	Address: 01:0007AA79
		Locals:


Module: LOADSAVE

	code (0 bytes) Get_File_Date_String_
	Address: 01:0007E609
		Locals:

	code (0 bytes) Get_Save_Game_Date_Strings_
	Address: 01:0007E752
		Locals:

	code (0 bytes) Set_Current_Game_Option_Flags_
	Address: 01:0007EFEF
		Locals:

	code (0 bytes) Update_Game_Settings_
	Address: 01:0007F14C
		Locals:

