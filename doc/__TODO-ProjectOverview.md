

gtest
gmock
ctest
github actions
tagged commits
releases
cpack
redo cmake
ninja compile_commands VSC



New Game

[ ] j_NEWG_CreateWorld__WIP()
[X] j_Initialize_Events()
[ ] j_NEWG_FinalizeTables()
[x] j_Save_SAVE_GAM()
[ ] GAME_WizardsLaunch()

[ ] j_NEWG_CreateWorld__WIP()

[ ] j_NEWG_FinalizeTables()
    [ ] Initialize_Items
    [ ] Init_Heroes
    [ ] Init_Players
    [ ] AI_WIZ_StrategyReset__WIP
    [ ] Init_Magic_Personalities_Objectives
    [ ] Init_Summoning_Circle_And_Spell_Of_Mastery
    [ ] Initialize_Messages
    [ ] _players.capital_race
    [ ] _players.gold_reserve
    [ ] Wiz5_Spell_E0h@[itr_players] = (80 + Random(40))  WTF is this?  ; 80 + rnd(40) is stored here for each AI wizard (word array pointer, human excluded) when setting initial gold

MoO2
Init_New_Game_()
|-> Init_Players_()


...something like create computer players...profile...spell library...

[x] Draw_Building_The_Worlds()
[x] Init_Computer_Players()
[x] |-> Init_Computer_Players_Wizard_Profile()
[x] |-> |-> Consolidate_Spell_Book_Realms
[x] |-> |-> Get_Weighted_Choice
[x] |-> Init_Computer_Players_Spell_Library()
[x] NEWG_ClearLandmasses()
[x] NEWG_CreateLands()
[x] |-> TILE_SetLandMass__WIP
[x] NEWG_SetBaseLands()
[x] NEWG_SetSpecLands()
[x] NEWG_CreateNodes()
[x] |-> NEWG_CreateNodeAura__WIP()
[x] |-> NODE_IsAuraUnique__WIP()
[x] |-> NEWG_SetNodeType__WIP()
[x] NEWG_EqualizeNodes()
[x] NEWG_CreateTowers()
[x] NEWG_TileIsleExtend()
[x] NEWG_CreateEZs()
[x] Generate_Home_City__WIP()
[x] |-> City_Maximum_Size_NewGame()
[x] |-> |-> Get_Useable_City_Area_NewGame()
[x] |-> |-> Square_Food2_NewGame()
[x] |-> Square_Is_Forest_NewGame()
[x] |-> Random_City_Name_By_Race_NewGame()
[x] |-> Create_Unit_NewGame()

[x] Generate_Neutral_Cities__WIP()

[ ] ...move special.c/h from MoM to MoX

[ ] NEWG_CreateSpecials()
[ ] NEWG_CreateRoads()
[ ] NEWG_CreateShores()
[ ] NEWG_CreateRiver()
[ ] NEWG_SetRiverTiles()
[ ] NEWG_SetDeserts()
[ ] NEWG_RandomizeTiles()
[ ] NEWG_SetMoveMaps()
[ ] CRP_NEWG_CreatePathGrids()
[ ] NEWG_SetScoutingMaps()
[ ] NEWG_AnimateOceans()
[ ] NEWG_EZ_MarkHadnLeft()



[x] New Game Screen 0 - Game Options
[x] New Game Screen 1 - Wizard Selection;   Title: "Select Wizard";         // HLPENTRY.LBX, 032  ""  "Wizard Selection Help"
[x] New Game Screen 2 - Wizard Portrait;    Title: "Select Portrait";       // HLPENTRY.LBX, 031  ""  "Wizard Picture Select"
[x] New Game Screen 3 - Wizards Name;       Title: "Wizard's Name";         N/A
[ ] New Game Screen 4 - ...custom wizard creation                           // HLPENTRY.LBX, 033  ""  "Wizard Creation Help"
[ ] New Game Screen 5 - ...custom ¿ ?                                       // HLPENTRY.LBX, 036  ""  "Select Spells Help"
[x] New Game Screen 6 - Wizards Race        Title: "Select Race";           // HLPENTRY.LBX, 035  ""  "Wizard Race Help"
[x] New Game Screen 7 - Wizards Banner      Title: "Select Banner";     // NEWGAME.LBX, 046  FLAGSNEW   // HLPENTRY.LBX, 030  ""  "Banner Selection Help"

// HLPENTRY.LBX, 029  ""  "Options Screen Help"
// HLPENTRY.LBX, 030  ""  "Banner Selection Help"
// HLPENTRY.LBX, 031  ""  "Wizard Picture Select"
// HLPENTRY.LBX, 032  ""  "Wizard Selection Help"
// HLPENTRY.LBX, 033  ""  "Wizard Creation Help"
// HLPENTRY.LBX, 034  ""  "settings Help"
// HLPENTRY.LBX, 035  ""  "Wizard Race Help"
// HLPENTRY.LBX, 036  ""  "Select Spells Help"

Module: COLONY
    data (0 bytes) _NEWGAME_LBX
        Address: 02:0017AA6E
Module: NEWGAME
    code (0 bytes) Draw_Newgame_Screen_
        Address: 01:000CCD3C
    code (0 bytes) Newgame_Screen_
        Address: 01:000CD435
Module: RACESEL
    code (0 bytes) Race_Selection_Screen_
        Address: 01:0005C510
    code (0 bytes) Flag_Screen_
        Address: 01:0005D03C
    code (0 bytes) Naming_Popup_
        Address: 01:0005D2BB

MoO2
Module: MAPGEN
    data (0 bytes) _fill_msg_bitmap
        Address: 02:001942E4
    data (0 bytes) _fill_msg_ctr
        Address: 02:001942F0
    data (0 bytes) _fill_msg
        Address: 02:001942F4

Module: HOMEGEN
    code (0 bytes) Set_Up_Home_Gen_Msg_
    Address: 01:0007C78E

Module: MISC
    code (0 bytes) Draw_Advancing_Fill_Message_
        Address: 01:0008EFE1

## _fill_msg_bitmap
XREF
    Build_Home_Star_List_()
    Generate_Home_Worlds_()
    Set_Up_Home_Gen_Msg_()
    Set_Star_XYs_()
    Universe_Generation_()

## _fill_msg

## _fill_msg_ctr

movsx   eax, _fill_msg_ctr
cmp     eax, _HOME_WORLD_GEN_MAX_CTR

if(_NUM_STARS > 54)
    _HOME_WORLD_GEN_MAX_CTR = 372





¿¿¿
ALPHA
    ''
BETA
    'City Screen' - 'Resource Breakdown'
???



https://www.google.com/search?q=C+code+test+argv+string+character+by+character+in+C%3F&oq=C+code+test+argv+string+character+by+character+in+C%3F&gs_lcrp=EgZjaHJvbWUyBggAEEUYQDIHCAEQIRigATIHCAIQIRigATIHCAMQIRigATIHCAQQIRigATIHCAUQIRigAdIBCTExNjg3ajBqN6gCCLACAfEF8LPJMOcKrCI&sourceid=chrome&ie=UTF-8

https://pages.cs.wisc.edu/~markhill/cs354/Fall2008/onyourown/C.argv.html




1>SDL2_mixer.lib(SDL2_mixer.dll) : warning LNK4006: __NULL_IMPORT_DESCRIPTOR already defined in SDL2.lib(SDL2.dll); second definition ignored



grep 
DEMOHACK
DEMO
HACK
STUB



if(strcmp(dst->name, src->name) != 0)
shows that "Banbury" got changes to "Creative Voice File"
¿ need to release city data before going into combat ?
...whatever gets dumped into COMBAT.TMP



When?
    if(remap_flag == ST_FALSE)
    {
        Clipped_Draw_Frame(start_x, start_y, actual_width, actual_height, skip_x, skip_y, frame_data);
    }
    else
    {
        // MoO2  Module: animate  Remap_Draw_Animated_Sprite(x_start, y_start, frame_data)
        // TODO  VGA_DrawPartImage_R(start_x, start_y, FLIC_GET_WIDTH(picture), p_FLIC_Frame);
        // TODO  Clipped_Remap_Draw_Frame__NOP(start_x, start_y, actual_width, actual_height, skip_x, skip_y, frame_data);
        Clipped_Draw_Frame(start_x, start_y, actual_width, actual_height, skip_x, skip_y, frame_data);
    }



// TODO  BU_Teleport(battle_unit_idx, target_cgx, target_cgy);
// TODO  BU_TunnelTo(battle_unit_idx, target_cgx, target_cgy);


Turn 0
Research Spells
all eight are same - Summon Champion

opponent made contact without any known connection

unexplored darkness still not right - see screen-shot

bad text on CityList
bad text on ArmyList



NEW TEST CASE - illusion ranged attack ... Aerie (Anskiy) ... Artifact (Suppanut)



int16_t WIZ_MatchManaUpkeep__WIP(int16_t player_idx, int16_t mana_upkeep)



            // SPELLY  CTY_StreamOfLife(itr_cities);
            // SPELLY  CTY_ChaosRift(itr_cities);
            // SPELLY  CTY_GaiasBlessing(itr_cities);
            // SPELLY  CTY_NightshadeDispel(itr_cities);



Combat.c
Strategic_Combat__WIP()
    /* DEMOHACK */ if(_combat_environ == 5) { IDK_health_attacker__2 = IDK_health_attacker = 0x7FFFFFFF /*0b01111111111111111111111111111111*/; }



MainScr.c
Move_Units()
        // TODO  OVL_SpellWardError();



        /* TODO */  itr_players = EVNT_FindNewMineral__STUB(player_idx, &wx, &wy, &wp, &terrain_special);
        /* TODO */  TILE_ReplaceMinerals__STUB(wx, wy, wp, terrain_special);



Rebellion Event
    player name and city name did not print



'Next Turn'
    clicked choose a new spell to research, screen didn't change, crashed in Next_Turn_Calc()
    something should happen to screen following/in between?



TODO 
How do we land in...
MainScr.c
    _UNITS[troops[itr_units]].Move_Failed = ST_TRUE;
What is Move_Failed even?

¿ MoO2 ~== NEXTTURN  Update_Units_MvsSts() ?



// TODO  DIPLO ROUND 2
e.g., no NPC demand/offer
¿ Next_Turn_Calc() stuff ?





DOME_RIGHTMEOW
    I'm an idiot, but I'm trying to move forward...
    So, move forward, but circle right back to this

FTLOGAATIH  [(f)or (t)he (l)ove (o)f (g)od (a)nd (a)ll (t)hat (i)s (h)oly]
    super silly buullshit that I should stop procrastinating about addressing properly
    or, stuff I don't really know how to do that I think a *Pro* would

OHDIO
    audio/sound code that I should have already fixed in the previous sprint

SPELLY
    Spell Casting code that I should have already finished probably in the current *sprint*

GRRRFUCKS
    VGA 'Mode Y', and other (low-level) graphics routines stuff
        Spellbook Page-Turn, Mosaic/Dissolve, Vanish/Jumble, 'Earthquake' shake, Expanding Box/Help, etc.

¿ ?
    Question?

TODO

DOMSDOS
    To-Do specific to the MS-DOS platform code

DEDU
    to be deduced - IDK, IDGI, could/would/should figure out

BUGBUG
    *real* bugs in OG-MoM



¿ // MSDOS ?  ...Allocate.C
¿ // PLATFORM  MSDOS ?  ..Fonts.C, Fonts.H
¿ // PLATFORM ?  ...Fonts.C, Fonts.H, Input.C, Input.H, Mouse.C, Mouse.H, Video.H


