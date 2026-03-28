Draw_Expanding_Bitmap is an animated screen transition effect — a "grow-out" or zoom-in reveal. It takes a source image and progressively scales it up from a starting point until it fills the full 320x200 screen.

Here's how it works step by step:

Setup — Copies the current screen contents into a buffer (the "background" behind the growing image).
Animation loop (runs counter frames) — Each iteration:
Calculates a growing rectangle anchored near (x_start, y_start) that expands proportionally each frame
Scales the source picture bitmap to the current Resize_Percent
Draws the scaled bitmap at the calculated position
Flips pages (double-buffered) and waits 1 tick
The effect is that the image starts as a small rectangle near the given coordinates and smoothly grows to fill the entire screen — like a "zoom in" or iris-out transition.

It's marked __WIP because the original EMM (Expanded Memory Manager) calls for saving/restoring the background screen buffer are still commented out (lines 792-795, 799-801) — those DOS-era memory operations haven't been fully replaced yet in the reconstruction.





[ ] break-down Combat.c
32K lines across many segments. Here are the natural split boundaries based on the disassembly segments:
Segment	Lines	Content	Suggested File
globals + s90	1-2795	Global vars, CMB_TacticalCombat (main loop)	Combat.c (keep)
s91	2796-4032	Turn flow, movement, unit selection, targeting	CombatTurn.c
s96	4033-4532	Combat entry, encounter resolution, hostility	CombatEntry.c
o98	4533-6588	Combat enchantments, AI turn, end-of-combat, unit loading, flee	CombatSetup.c
o99	6589-7564	Full screen draw, GUI buttons, entities, HP bars	CombatDraw.c
s103	7565-9204	Info window, unit selection, resource loading	CombatInfo.c
o105	9205-10352	Win/lose/flee, banner paint, image FX, animations	CombatFX.c
o110	10353-11672	Damage calc, healing, AI damage estimation	CombatDamage.c
o111-o112	11673-14118	AI spellcasting, spellbook UI, spell costs	CombatSpells.c
o113	14119-16230	Spell targeting, ranged/melee anim, summons	CombatAttack.c
o114	16231-18750	AI movement, action selection, pathfinding	CombatAI.c
o116	18751-20269	Unit stats, item enchants, specials, levels, node aura	CombatUnit.c
o122	20270-22700	Attack/defense rolls, resist, melee/ranged resolution	CombatRolls.c
o123-o124	22701-25642	End-of-combat, results scroll, raze, misc	CombatResults.c
ovr139	25643-25721	AI combat spell evaluation	fold into CombatAI.c
    ovr153	25722-27959	Draw map, entities, spawn walls/trees/figures	CombatEntities.c
    ovr154	27960-30045	Terrain init, map gen, tile gen, pathfinding coords	CombatTerrain.c
    ovr155	30046-31676	Pathfinding (GetPath, FillReachMap)	CombatPath.c
    ovr163	31677-32407	GFX loading, background compose, allocations	CombatGFX.c
I'd start with the cleanest cuts — the ones with no shared static variables and clear segment boundaries:
ovr154 + ovr155 (terrain gen + pathfinding, lines 27960-31676) — self-contained, rarely touches globals beyond the combat grid
ovr153 + ovr163 (entity spawning + GFX loading, lines 25722-27959 + 31677-32407) — drawing/resource code
o114 + ovr139 (AI, lines 16231-18750 + 25643-25721) — AI logic is naturally isolated
Want me to start splitting one of these out?
[ ] ovr154  CombatTerrain.c
[ ] ovr155  CombatPath.c
[ ] ovr153  CombatEntities.c
[ ] ovr163  CombatGraphics.c


MoO2 has:
    Module: CMBTAI
    Module: CMBTDRW1
    Module: CMBTFIR2
    Module: CMBTFIRE
    Module: CMBTMIS
    Module: CMBTMOV1
    Module: CMBTSPEC





Root Cause Analysis
The Error: Access violation reading location 0x0000019B9F96A565 when trying to access _UNITS[battle_units[target_battle_unit_idx].unit_idx].type where target_battle_unit_idx is -1 (ST_UNDEFINED).
Why This Is Happening:
The code is in a section that tries to coordinate targeting among friendly units:

// Comment in code: "if there is a lower index own unit with a target of the same type 
// and same base Defense, target that instead"

The loop iterates through battle units with indices less than battle_unit_idx (which is 7), checking if any friendly units at lower indices have targets of the same type. However, the code has a critical flaw:
1.	target_battle_unit_idx for battle_unit 7 is currently -1 (ST_UNDEFINED), meaning AI_BU_SelectAction__WIP didn't find a valid target
2.	The code tries to compare the type of the current iteration's target (battle_units[itr_battle_units].target_battle_unit_idx) with the type at target_battle_unit_idx (-1)
3.	When accessing battle_units[-1].unit_idx, it reads garbage memory (0x2122 = 8482)
4.	This garbage value is then used to index _UNITS[8482].type, which is far beyond the array bounds
5.	The resulting address 0x0000019B9F96A565 is in unallocated memory, causing the access violation
The Fix: The code must check if target_battle_unit_idx is valid (not ST_UNDEFINED) before attempting to use it as an array index. The entire block should be skipped if no target has been assigned yet.





gtest
gmock
ctest
github actions
tagged commits
releases
cpack
redo cmake
ninja compile_commands VSC





Back Around To The Beginning
[ ] Next_Turn_Proc()
[ ] Next_Turn_Calc()
[ ] NPC_To_Human_Diplomacy__WIP()




New Game

[ ] Init_New_Game()
[X] j_Initialize_Events()
[ ] j_NEWG_FinalizeTables()
[x] j_Save_SAVE_GAM()
[ ] GAME_WizardsLaunch()

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
[x] Init_New_Game()  
[x] Init_Computer_Players()
[x] |-> Init_Computer_Players_Wizard_Profile()
[x] |-> |-> Consolidate_Spell_Book_Realms
[x] |-> |-> Get_Weighted_Choice
[x] |-> Init_Computer_Players_Spell_Library()
[x] Init_Landmasses()
[x] Generate_Landmasses()
[x] |-> TILE_SetLandMass__WIP
[x] Translate_Heightmap_To_Base_Terrain_Types()
[x] Generate_Climate_Terrain_Types()
[x] Generate_Nodes()
[x] |-> NEWG_CreateNodeAura__WIP()
[x] |-> NODE_IsAuraUnique__WIP()
[x] |-> NEWG_SetNodeType__WIP()
[x] Rebalance_Node_Types()
[x] Generate_Towers()
[x] Extend_Islands()
[x] Generate_Lairs()
[x] Generate_Home_City__WIP()
[x] |-> City_Maximum_Size_NewGame()
[x] |-> |-> Get_Useable_City_Area_NewGame()
[x] |-> |-> Square_Food2_NewGame()
[x] |-> Square_Is_Forest_NewGame()
[x] |-> Random_City_Name_By_Race_NewGame()
[x] |-> Create_Unit_NewGame()
[x] Generate_Neutral_Cities__WIP()
[x] Generate_Terrain_Specials()
[x] |-> Square_Is_Grassland_NewGame()
[x] |-> Square_Is_Mountain_NewGame()
[x] |-> Mountain_Terrain_Special()
[x] |-> Square_Is_Hills_NewGame()
[x] |-> Hills_Terrain_Special()
[x] |-> Square_Is_Swamp_NewGame()
[x] |-> Square_Is_Desert_NewGame()
[x] |-> Desert_Terrain_Special()
[x] Generate_Roads()
[x] Simex_Autotiling()
[x] River_Path()
[x] River_Terrain()
[x] Desert_Autotile()
[x] Shuffle_Terrains()
[x] Movement_Mode_Cost_Maps()
[x] CRP_NEWG_CreatePathGrids__STUB()
[x] |-> NIU  MGC  ovr054  CRP_SPATH_Arbitrary()
[x] |-> NIU  MGC  ovr054  UU_SPATH_Segmented()
[x] |-> NIU  MGC  ovr054  UU_SPATH_15by15()
[x] |-> NIU  MGC  ovr054  UU_SPATH_Segments()
[x] |-> NIU  MGC  ovr054  UU_SPATH_GetBorderTiles()
[x] |-> NIU  MGC  ovr055  CRP_NEWG_CreatePathGrid()
[x] Init_Square_Explored()
[x] |-> Set_Square_Explored_Bits()
[x] Animate_Oceans()
[x] Set_Upper_Lair_Guardian_Count()
[x] BUG  starting city in middle of ocean, for human player; failed ocean check? failed max pop check?
[x] ...move special.c/h from MoM to MoX

[x] New Game Screen 0 - Game Options
[x] New Game Screen 1 - Wizard Selection;   Title: "Select Wizard";         // HLPENTRY.LBX, 032  ""  "Wizard Selection Help"
[x] New Game Screen 2 - Wizard Portrait;    Title: "Select Portrait";       // HLPENTRY.LBX, 031  ""  "Wizard Picture Select"
[x] New Game Screen 3 - Wizards Name;       Title: "Wizard's Name";         N/A
[x] New Game Screen 4 - ...custom wizard creation                           // HLPENTRY.LBX, 033  ""  "Wizard Creation Help"
[x] New Game Screen 5 - ...custom ¿ ?                                       // HLPENTRY.LBX, 036  ""  "Select Spells Help"
[x] New Game Screen 6 - Wizards Race        Title: "Select Race";           // HLPENTRY.LBX, 035  ""  "Wizard Race Help"
[x] New Game Screen 7 - Wizards Banner      Title: "Select Banner";     // NEWGAME.LBX, 046  FLAGSNEW   // HLPENTRY.LBX, 030  ""  "Banner Selection Help"

fixup Linux/Clang
remove all WIPs/dunders
remove all "retort"

[ ] WTF - C:\Users\jbalcomb\AppData\Roaming\Code\User\globalStorage\github.copilot-chat\memory-tool\memories\editing-preferences.md

[ ] Naming Things Is Hard^Hard - 'New Game'

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



[ ] add Set_Game_Random_Seed_() from MoO2, in Init_New_Game(), with _cmd_line_seed

[ ] MapGen location validation for neutral cities generation
[ ] MapGen - Towers too close together
[ ] MapGen - missing SE corner Shore?


[\] mouse movement still feels laggish/weird
[x] new game turn 1 all human contacted
[x][x] constant clicking sound on production screen - happens as soon as you click 'Change' button

[x] new game city max pop invalid

[x] research spells seem like the wrong ones
[ ] all research spells show (999 turns)

[ ] prod scr usa help is bldg help, melee is part maintenance but also melee - seems bldg is over unit

[ ] Unit_List_Window_Draw() - TODOs

[ ] MSVC-debug didn't copy SDL2_mixer.dll


not saving new game options

sometimes the build or purify buttons draw overtop the next turn button

[x] city combat not respecting city walls

some blue wave lightning looking attack?

townsfolk drawing below the bar, maybe just last one or rebels

[ ] SUCCESS:  Mix_OpenAudio():  Couldn't open timidity.cfg



DIPLOMAC.LBX [entry 52]  was not properly allocated or has been corrupted.

[ ] Clean the hot mess that is Play_Sound ... // DOMSDOS  Play_Sound__STUB(SND_Spell_Music);  Play_Sound(SND_Spell_Music, SND_Spell_Music_size);
sdl2_Play_Background_Music__WIP
        // DOMSDOS  Play_Background_Music__STUB();
[ ] Stop_All_Sounds__STUB();
// DOMSDOS  Stop_Music__STUB();



TODO  add test for WIZ_Conquest()

TODO  review CMB_TileGen__WIP() and friends
CMB_Units_Init__WIP()


[x] have Claude code a tool to generate the tan, sin, and cos tables for angle.c

[ ] feed Gemini the whole path from GAME_DrawLimboFall() through the rotated drawing functions

[ ] fix naming for attack_idx/defender_idx for player or battle_unit

[ ] check out: /home/jbalcomb/devel/ReMoM/build/_deps/googletest-src/googletest/include/gtest/internal/gtest-port.h



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


