



¿ Main vs. Init ?

MGC seg001
WZD seg001

MGC main()
WZD main()

SEEALSO: gDrive > SimTexUni > MoM-MGC-vs-WZD.gsheet > Main

Almost as-is, just add Init_Platform()
remove Load_Game_Update_WZD(), just use Load_Game_Update()
move WZD Load_SAVE_GAM(8) and Load_Game_Update()
separate WZD: Load_WZD_Resources() and MGC: Load_MGC_Resources(), Load_TERRSTAT(), Load_SPELLDAT()
    Per-Screen
shift down Menu_Screen_Control(), under Screen_Control()

Now, ...
    fold-in the TST/PoC code...

~ MoM Main & MoX PFL & MsWin

ditch MoM_main.C, MoM_Init.C, Win32_Main.cpp, Win32_Init.cpp, Win32_Evnt.cpp

New Entry-Point:
    Per Platform
        dos_MoM  (dos_MGC.C, dos_WZD.C) || (MAGIC.C, WIZARDS.C)
        win_MoM  + win_PFL.cpp/.hpp
        sdl_MoM  + sdl_PFL.cpp/.hpp

MoX_PFL
    replace MoM_PFL & PoC_PFL

.cpp/.hpp just for MS-Windows code?
vs. .cpp/.hpp for Platform-Layer code?

¿ VIDEO_MODE ?

¿ MGC/WZD seg001 ?
    ...would have just been in ~MAGIC.C/WIZARDS.C
    ...so, now, would have just been in MoM.C
    ...in MoO2, in MoX.C and MoX2.C
    kinda like what I had meant for MoM_main.C
    








OG-MoM
IBM-PC, VGA, MS-DOS, 16-bit, EMS, Segmented Memory, Base/Far Pointers
dos_MGC.C/.H
dos_WZD.C/.H

MoM-Rasm
32-/64-bit, Linear Memory, Platform-Layer
win_MoM.C/.H

~ MGC & WZD vs. MoM

MoM_main.C/.H; MGC_main.C.H, WZD_main.C/.H




## OON
Cache LBX Files in EMM
Allocate Memory for Game Data
Load Pictures, Per Screen
¿ Defaults ?
¿ *Initialize* Game Data / Game State ?



## MoM Main
MGC & WZD
seg 001 proc 001
~ MGC_main + WZD_main
~ MoM_Init
*.* |-> Screen_Control()
~ WinMain() |-> InitializeGame() ... StartGame()
¿ Req'd: ?

MGC & WZD Main  ~==  MGC & WZD Init




## Initialization Functions
* MGC main()
* WZD main()
* MoM_Tables_Init()
* Load_MGC_Resources()
* Load_WZD_Resources()
* Loaded_Game_Update_WZD()  ~== WZD  ovr160  o150p04  Loaded_Game_Update()
GAME_Overland_Init()
G_WLD_StaticAssetRfrsh()
Terrain_Init()


¿ MGC Load_MGC_Resources() vs. WZD Load_WZD_Resources() ?
seems like a kind of equivalent - WZD may indeed be more 'Main Game' than 'Main Screen'

What would it seem like we would need to be initializing?
¿ Where does the caching of the LBX files fit in here ?

Hardware - Keyboard, Mouse, Sound Card, Video Card, Clock, Memory

Keyboard
Mouse
Sound - SFX,MIDI ~(VOC,XMIDI)
Video - set Mode 13, enable memory paging
Extended Memory
BIOS Timer/Clock
PRNG

Memory Allocations

Game Data

Game State

Load Resources - Per Screen
    MGC
        Menu Screen
        New Game Screen
        Load Game Screen
        Settings Screen
    WZD
        Main Screen
        Combat Screen
        Load Screen
        Settings Screen
        City Screen
        Outpost Screen
        Spell-Book Screen
        Armies Screen
        Cities Screen
        Magic Screen
        ...
        Overland Spell Casting
        Road Building
        Surveyor
        Cartographer Screen
        Apprentice Screen  (¿ vs. Spells Screen ?)
        Historian Screen  (full)  ("History of Wizards Power")
        Astrologer Screen (overlay)  ("Current Status of Wizards") 
        Chancellor Screen  (overlay) (Scroll, Events) ("NO EVENTS THIS MONTH")
        Tax Collector Screen (box/window (overlays Select An Advisor))  ("Tax Per Population")
        Grand Vizier Screen (Y/N message box (overlays Select An Advisor))
        Mirror Pop-Up
        ...
        ~ Overlays the Main Map Screen, replaces Summary/Unit Window and Unit Actions/Next Turn buttons
        "Casting"           `[CANCEL]`
        "Surveyor"          `[CANCEL]`
        "Road Building"     `[OK][CANCEL]`










lots of note scraps in STU-MoM_Rasm-NoGit, currently housed at \\HMS-IdeaPad330\C\devel\STU\STU-MoM_Rasm-NoGit\MoM_Rasm\docs
MGC-ProgramFlow.odg
MGC_and_WZD.md
MoM-ProgramPaths.md
MGC-Main.md
MGC-MainMenu


¿¿¿ 
    STGE
    MoX
    MoM
    MGC
    WZD
    *Game* vs. *Main Screen ~== Game*
???

MGC vs. WZD
MGC Main vs. WZD Main
MGC Main |-> Main_MenuScreen() vs. WZD Main |-> Main_Screen()
MGC Main &&|| WZD Main vs. Load_Screen() |-> Load_SAVE_GAM() |-> Loaded_Game_Update()

MGC main() |-> Screen_Control() |-> Load_Screen() |-> GAME_WizardsLaunch() |-> Load_SAVE_GAM() ... GAME_EXE_Swap() |-> execl("WIZARDS.EXE")



# Load / Init Game-Data / Game-State

MGC                         WZD
CONFIG.MOM                  CONFIG.MOM
MAGIC.SET                   MAGIC.SET
j_MoM_Tables_Init(6100)     j_MoM_Init_Tables(4600)

MGC
CONFIG.MOM
MAGIC.SET
j_MoM_Tables_Init(6100)
Load_MGC_Resources()
Load_TERRSTAT()
LOAD_SPELLDAT()

WZD
CONFIG.MOM
MAGIC.SET
j_MoM_Init_Tables(4600)
j_Load_SAVE_GAM(8)
j_Load_WZD_Resources()
WZD_Startup_MainGame()
_current_screen = scr_Main_Screen
_players.Banner+17E8h = BNR_Brown
GAME_SoM_Cast_By = ST_UNDEFINED

Load CONFIG.MOM
Load MAGIC.SET
Setup Sound (MIDI & SFX)
magic_set.Input_Type
Check Saved Games - update MAGIC.SET
Set Min EMM
Set Min RAM
Initialize Hardware
Init_Drivers()
MGC:    Release_Version(); j_MoM_Tables_Init(6100); Set_Global_Escape()
WZD:    Release_Version(); Set_Global_ESC(); j_MoM_Init_Tables(4600);



MGC vs. WZD
MGC_and_WZD.md says
    MGC seg001 vs. WZD seg001
    MAGC Main vs. WZD Main
    ...looks to be the same up until Table_Init()
MoM-ProgramPaths.md says
    Init_Tables - MGC vs. WZD
    These are the exact same.
    MGC
    ovr057
    s57p01  (1of1)
    MGC_Init_Tables
    WZD
    ovr152
    s152p01  (1of1)
    WZD_Init_Tables

Loads CONFIG.MOM    ¿ Req'd: input_type for Init_Drivers() ?
    config_mom.MIDI_IO, MIDI_IRQ, MIDI_HW
    config_mom.SFX_IO, SFX_IRQ, SFX_DMA, SFX_HW

Loads MAGIC.SET     ¿ Req'd: saves, save_name in ~Check_For_Saved_Games() ?
    magic_set.Snd_Channels
    magic_set.Input_type




### MoM_Tables_Init()

exact same in MGC & WZD

¿ everything from a SAVE_GAM ? ¿ ~== World_Data ?

 [x] p0_heroes
 [x] p1_heroes
 [x] p2_heroes
 [x] p3_heroes
 [x] p4_heroes
 [x] p5_heroes
 [-] _num_players
 [-] _landsize
 [-] _magic
 [-] _difficulty
 [-] _cities
 [-] _units
 [-] _turn
 [-] _unit
 [-] _players
 [x] _world_maps         World_Data
 [x] UU_TBL_1            World_Data
 [x] UU_TBL_2            World_Data
 [x] TBL_Landmasses      World_Data
 [x] TBL_Nodes
 [x] _FORTRESSES
 [x] _TOWERS
 [x] TBL_Lairs
 [x] TBL_Items
 [x] _CITIES             World_Data
 [x] _UNITS
 [x] TBL_Terr_Specials   World_Data
 [x] TBL_Scouting        World_Data
 [x] TBL_MoveMaps_EMS
 [x] _events_table
 [x] _map_square_flags   World_Data
 [-] grand_vizier
 [x] TBL_Premade_Items
 [x] TBL_Hero_Names
square_scouted_p0       World_Data
square_scouted_p1       World_Data



EmmHndl_FIGUREX
EmmHndl_TILEXXX
EmmHndl_CONTXXX

_screen_seg
GUI_SmallWork_IMG
GFX_Swap_Seg

World_Data
    _CITIES
    _world_maps
    UU_TBL_1
    UU_TBL_2
    TBL_Landmasses
    TBL_Terr_Specials
    _map_square_flags
    TBL_Scouting
    square_scouted_p0
    square_scouted_p1
    World_Data_Extra

TBL_TempMoveMap_EMS                         Nay-SAVE_GAM
TBL_MoveMaps_EMS                            Yay-SAVE_GAM
TBL_SharedTiles_EMS                         Nay-SAVE_GAM
TBL_Catchments_EMS                          Nay-SAVE_GAM
TBL_OvlMovePaths_EMS                        Nay-SAVE_GAM

p0_heroes                                   Yay-SAVE_GAM
p1_heroes                                   Yay-SAVE_GAM
p2_heroes                                   Yay-SAVE_GAM
p3_heroes                                   Yay-SAVE_GAM
p4_heroes                                   Yay-SAVE_GAM
p5_heroes                                   Yay-SAVE_GAM
_UNITS                                      Yay-SAVE_GAM

Active_Unit                                 Nay-SAVE_GAM

TBL_Nodes                                   Yay-SAVE_GAM
_FORTRESSES                                 Yay-SAVE_GAM
_TOWERS                                     Yay-SAVE_GAM
TBL_Lairs                                   Yay-SAVE_GAM
_events_table                               Yay-SAVE_GAM
TBL_Hero_Names                              Yay-SAVE_GAM
TBL_Items                                   Yay-SAVE_GAM
TBL_Premade_Items                           Yay-SAVE_GAM

TBL_Spell_Data                              Nay-SAVE_GAM
UnitDraw_WorkArea                           Nay-SAVE_GAM
SND_Music_Segment                           Nay-SAVE_GAM

// ?
// mov     [AI_Arc_MainWarConts@], (offset TBL_Wizards.Spells_Known+17E8h)
// mov     [AI_Myr_MainWarConts@], (offset TBL_Wizards.Spells_Known+17F4h)
// mov     [AI_CONTX_Reevals@], (offset TBL_Wizards.Spells_Known+1800h)
// mov     [Wiz5_Spell_28h@], (offset TBL_Wizards.Spells_Known+1810h)
// mov     [Wiz5_Spell_3Ch@], (offset TBL_Wizards.Spells_Known+1824h)
// mov     [Wiz5_Spell_50h@], (offset TBL_Wizards.Spells_Known+1838h)
// mov     [Wiz5_Spell_64h@], (offset TBL_Wizards.Spells_Known+184Ch)
// mov     [AI_Arc_NewColConts@], (offset TBL_Wizards.Spells_Known+1860h)
// mov     [AI_Myr_NewColConts@], (offset TBL_Wizards.Spells_Known+1874h)
// mov     [AI_Arc_NewColTgtXs@], (offset TBL_Wizards.Spells_Known+1888h)
// mov     [AI_Myr_NewColTgtXs@], (offset TBL_Wizards.Spells_Known+189Ch)
// mov     [AI_Arc_NewColTgtYs@], (offset TBL_Wizards.Spells_Known+18B0h)
// mov     [AI_Myr_NewColTgtYs@], (offset TBL_Wizards.Spells_Known+18BCh)
// mov     [AI_SCircle_Reevals@], (offset TBL_Wizards.Spells_Known+18C8h)
// ?





### Load_TERRSTAT()
~== MoO2 Module NEWGAME Reload_Newgame_Screen()
    terrain_stats_table = SA_MK_FP0( LBXR_LoadSingle("TERRSTAT", 0, 0, 770,  6) )
### Load_SPELLDAT()
~== MoO2 Module NEWGAME Reload_Newgame_Screen()
    spell_data_table    = SA_MK_FP0( LBXR_LoadSingle("SPELLDAT", 0, 0, 215, 36) )



### MGC Load_MGC_Resources() vs. WZD Load_WZD_Resources()


### Load_MGC_Resources

caches MAINSCR, WIZARDS, SPELLDAT
NOTE: back in main(), following this, it caches TERRSTAT and SPELLDAT
So, ...
    cache LBX for Menu Screen and New Game Screen
    ~ Menu_Screen_Load_Pictures()

    EMM_Load_LBX_File_1("MAINSCR")
    mainmenu_top = LBX_Load(mainscrn_lbx_file, 0);
    mainmenu_c   = LBX_Load(vortex_lbx_file, 1);
    mainmenu_h   = LBX_Load(vortex_lbx_file, 2);
    mainmenu_q   = LBX_Load(vortex_lbx_file, 3);
    mainmenu_n   = LBX_Load(vortex_lbx_file, 4);
    mainmenu_l   = LBX_Load(vortex_lbx_file, 5);
    mainmenu_bot = LBX_Load(mainscrn_lbx_file, 5);
    // HERE: ~== MoO2 Module NEWGAME Reload_Newgame_Screen()
    EMM_Load_LBX_File_1("WIZARDS")
    gsa_WIZARDS_0to13 = LBX_Load("WIZARDS", {0,13})
    EMM_Load_LBX_File_1("SPELLDAT")









### Load_WZD_Resources

¿ All Resources for Main_Screen() and Children/Siblings ?


LBX_Load_Click_SFX()
MoO2: button_sound

"Load" 

Cache
    BACKGRND, CHRIVER, CMBMAGIC, CMBTCITY, CITYWALL, CITYSCAP, ITEMISC, ITEMS, MAIN, MAPBACK, UNITS1, UNITS2, UNITVIEW, SPECIAL, SPECIAL2
    SPELLDAT, BUILDDAT
¿ ~ Cache(); ...; Cache_Load(); ?


j_Load_MAGIC_SET()
EMM_Load_LBX_File_1(rsc01_MAIN_LBX)
EMM_Load_LBX_File_1(rsc02_BACKGRND_LBX)
EMM_Load_LBX_File_1(rsc03_UNITS1_LBX)
EMM_Load_LBX_File_1(rsc04_UNITS2_LBX)
EMM_Load_LBX_File_1(rsc05_UNITVIEW_LBX)
EMM_Load_LBX_File_1(rsc06_SPECIAL_LBX)
EMM_Load_LBX_File_1(rsc07_SPECIAL2_LBX)
EMM_Load_LBX_File_1(rsc08_ITEMS_LBX)
EMM_Load_LBX_File_1(rsc09_CHRIVER_LBX)
EMM_Load_LBX_File_1(rsc0A_ITEMISC_LBX)
EMM_Load_LBX_File_1(rsc0B_CITYSCAP_LBX)
EMM_Load_LBX_File_1(rsc0C_MAPBACK_LBX)
EMM_Load_LBX_File_1(rsc0D_CMBMAGIC_LBX)
EMM_Load_LBX_File_1(rsc0E_CMBTCITY_LBX)
EMM_Load_LBX_File_1(rsc0F_CITYWALL_LBX)
Terrain_Init()
Main_Screen_Load_Pictures()
    loads main screen background, game buttons, unit action buttons, weapon icons, movement icons, 
        side window backgrounds and buttons for casting, road building, and surveyor
        mirror_popup_background, goto_booty_icon, event icons

Load_Combat_Background_Bottom()
    combat_background_bottom = LBX_Load("BACKGRND", 3)
// Since BACKGRND.LBX was just cached in EMM, this will only allocate and set the FLIC_HDR
; AKA LBX_Assign_Cmbt_BG
; creates a reserved EMM LBX header for the combat screen's bottom background

Load_SPELLDAT()          ; loads all records from SPELLDAT.LBX, overwriting
    spell_data_table = SA_MK_FP0( LBX_Load_Data("SPELLDAT", 0, 0, 215, 36) )
Load_BUILDDAT()          ; loads all records from BUILDDAT.LBX into a single
    build_data_table = SA_MK_FP0( LBX_Load_Data("BUILDDAT", 0, 0, 36, 52) )

Load_Button_Click_SFX()     ; loads the standard and left click sounds
// LBX_Load()  SOUNDFX.LBX
// Entry 0  SLX19.VOC  Standard Button click
// Enrty 2  OKBUTTN  left button click
// gsa_SOUNDFX_0_SFX_Standard_Click
// OON XREF  SND_PlayClickSound() |-> SND_PlayFile(gsa_SOUNDFX_0_SFX_Standard_Click)
// gsa_SOUNDFX_2_SFX_Left_Click
// SND_LeftClickSound() |-> SND_PlayFile(gsa_SOUNDFX_2_SFX_Left_Click)
// RP_SND_LeftClickSound2()  |-> SND_PlayFile(gsa_SOUNDFX_2_SFX_Left_Click)


UNIT_Upkeep_Reset()      ; resets the Upkeep Cost of Normal Units and Heroes
