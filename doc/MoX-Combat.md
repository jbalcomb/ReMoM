
Elsewhere, ...
    BattleUnit / strategic_unit / 'Combat Ship'
    MoM-BattleUnit.md






Settings - Help - 'Raze City'
RAZE CITY
Controls whether or not you are
prompted for confirmation on razing a
city after you conquer it. If this setting
is off you may still raze a captured city
by pressing the 'R' key while the combat
results scroll is on the screen.
ovr124




ovr123
¿ end of combat message ?

MoX_CMB
ovr098
ovr116
¿ ?

ovr090
ovr096
ovr110

/*
    Combat

    WIZARDS.EXE
        ovr090
        ovr096
        ovr110
        ovr122
        ovr123

*/
/*
    WIZARDS.EXE
        ovr098
        ovr116
*/



1oom
game_battle_finish()
    |-> game_planet_destroy()
    |-> game_diplo_battle_finish()

game_ai_classic_battle_ai_ai_resolve()
    |-> game_ai_classic_battle_ai_ai_resolve_do()
    |-> game_battle_finish()





Move_Units()
    |-> Combat()
    |-> Lair_Combat()

Lair_Combat()
    |-> Lair_Combat_Do()
        |-> Combat()

Combat()
    |-> Strategic_Combat()
    |-> Tactical_Combat()

Combat()
    |-> CTY_TakeOver()
        |-> WIZ_Conquer()

Combat() |-> STK_CaptureCity()
    |-> CTY_RampageVictory()
    ...
    |-> G_CMB_ShowSummary()

Combat() |-> Strategic_Combat()
    |-> IDK_Combat_Allocate()
    |-> Combat_Structure()


Combat() |-> Tactical_Combat()


WIZ_Conquer
    |-> WIZ_Banishment()
    |-> WIZ_Conquest()
    |-> WIZ_ConquestSpells()
    ...
    |-> GAME_LimboFallAnim()
    |-> GAME_OVER()
    |-> GAME_EXE_Swap()
    ...
    |-> GAME_IsWon()
    |-> GAME_PlayVictoryAnim()
    |-> GAME_OVER()
    |-> GAME_EXE_Swap()



ovr096
Combat()
j_Combat()
Lair_Combat_Do()
IDK_CMB_s7DE08()

IDK_CMB_s7DE08()
OON XREF: NX_j_IDK_CMB_s7DE08()

j_Combat()
OON XREF: Move_Units()

ovr090
OON function
CMB_TacticalCombat()
OON XREF: Combat()




### Strategic_Combat()

called from Combat() with OVL_Action_XPos, OVL_Action_YPos, OVL_Action_Plane
immediately uses those parameters to set OVL_Action_XPos, OVL_Action_YPos, OVL_Action_Plane




### Lair_Combat()
wraps confirmation box, combat, and treasure


Lair_Combat_Do()
takes lair_idx and player_idx
lair_idx is used to get the wx, wy, wp
builds a troops array using Player_Army_At_Square()
calls Combat()
with the troops array
combat_attacker_player is player_idx
combat_defender_player is hard-coded to NEUTRAL_PLAYER
does not used OVL_Action_Structure
...which means it's doing something Combat() must have been doing already?
...in Move_Units(), for stack/city combat, it passes the existing parameters directly


Lair_Combat_Do()
    |-> Lair_Make_Guardians()

Lair_Make_Guardians()
NX_Jj_Lair_Make_Guardians()
Combat();

Eh?
Move_Units()
    |-> Lair_Combat()
        |-> Lair_Combat_Do()
            |-> Combat()
                |-> Lair_Make_Guardians()
...*Lairs* and *Combat* are inseparable...








### OVL_Action_Type

WZD dseg:9284 00 00                                           OVL_Action_Type dw 0

values: {-1, 0, 1, 5}

-1 is only ever set in Combat()

...combat only happens as a result of movement...
Eval_Move_Path() sets {0,1,5}



### OVL_Action_Structure

WZD dseg:9286 00 00                                           OVL_Action_Structure dw 0

when OVL_Action_Type is 1, OVL_Action_Structure contains the City Index
when OVL_Action_Type is 5, OVL_Action_Structure contains the Lair Index

in the data segment, OVL_Action_Structure is followed by 14 *unused* bytes, so it may have been a ((common) paragraph sized) structure

in Lair_Combat(), the code uses OVL_Action_Structure instead of the passed in lair_idx
and this is only for setting OVL_Action_XPos, OVL_Action_YPos, and OVL_Action_Plane
Macro? API Boundary?
Why aren't they in the structure?





## drake178:
OVL_ResolveConflict() => Combat()
CMB_TacticalCombat() => Tactical_Combat()
EZ_Resolve() => Lair_Combat()
EZ_ResolveEntry() => Lair_Combat_Do()
GUI_EZ_Message => lair_message_box_text
EZ_Defenders => lair_confirm_buttons_flag
EZ_CreateGuardStack() => Lair_Make_Guardians()



OVL_Action_Type

CMB_DrawFullScreen:loc_80F24  cmp     [OVL_Action_Type], 0 
Combat+2B9                    cmp     [OVL_Action_Type], 0 
CMB_Victory:loc_A5E75         cmp     [OVL_Action_Type], 0 
G_CMB_ShowSummary:loc_A6A0A   cmp     [OVL_Action_Type], 1 
Combat:loc_7D7E5              cmp     [OVL_Action_Type], 1 
Combat:loc_7D702              cmp     [OVL_Action_Type], 1 
Combat+641                    cmp     [OVL_Action_Type], 1 
Combat+51E                    cmp     [OVL_Action_Type], 1 
CMB_Victory:loc_A5E7F         cmp     [OVL_Action_Type], 1 
CMB_Victory:loc_A5827         cmp     [OVL_Action_Type], 1 
CMB_Units_Init+435            cmp     [OVL_Action_Type], 1 
CMB_UnitCityDamage+4          cmp     [OVL_Action_Type], 1 
CMB_TacticalCombat:loc_75EC4  cmp     [OVL_Action_Type], 1 
AI_BU_ProcessAction+227       cmp     [OVL_Action_Type], 1 
CMB_TacticalCombat+6A         cmp     [OVL_Action_Type], 1 
CMB_ShowSpellbook+594         cmp     [OVL_Action_Type], 1 
CMB_ComposeBookText+1B4       cmp     [OVL_Action_Type], 1 
CMB_SetCommonXFX:loc_845A5    cmp     [OVL_Action_Type], 1 
Combat_Structure:loc_A7AAF    cmp     [OVL_Action_Type], 1 
CMB_CountExternalFX:loc_84778 cmp     [OVL_Action_Type], 1 
Move_Units+510                cmp     [OVL_Action_Type], 5 
CMB_Victory:loc_A6471         cmp     [OVL_Action_Type], 5 
CMB_Victory:loc_A5C58         cmp     [OVL_Action_Type], 5 
CMB_Victory+80                cmp     [OVL_Action_Type], 5 

Combat+749                    mov     [OVL_Action_Type], -1
Combat+C0                     mov     [OVL_Action_Type], -1
Eval_Move_Path__WIP+22B       mov     [OVL_Action_Type], 0 
IDK_CMB_s7DE08+56             mov     [OVL_Action_Type], 0 
Eval_Move_Path__WIP+185       mov     [OVL_Action_Type], 1 
Eval_Move_Path__WIP+C2        mov     [OVL_Action_Type], 5 

Combat:loc_7D4BC              mov     ax, [OVL_Action_Type]
Combat:loc_7D582              mov     ax, [OVL_Action_Type]
CMB_DrawFullScreen:loc_80FCD  mov     ax, [OVL_Action_Type]




##### Naming Things is Hard

creature lairs, uncontrolled magic nodes, and similar structures
sites
ancient ruins and towers of wizardry
Treasure
Artifacts
yield special treasures
gold reserve
loot
guardian creatures
yields treasures


Page 90  (PDF Page 95)

Combat
Combat begins when your wizard’s armies enter an overland
square occupied by creatures or enemy troops,
 or when enemy troops step onto map squares occupied by your units.
Before stepping onto a map square occupied by enemy troops,
you may wish to examine those troops.
To do so, right-click on the stack of enemy units in the main movement view.
A unit list window appears that describes all of the stacked units in a one-line format.
Enemy units use the same statistic icons as your troops.
You can see a unit’s melee and ranged attack strengths, defense, hit points, speed and movement type (see Basic Unit Information and Table of Basic Movement Types).
If you are planning to invade a neutral or enemy city,
  you can determine the number and type of enemy troops there by right-clicking on the city square in the main movement view (see Imperialism).

Finally, whenever you move a unit onto a map square that contains creature lairs, uncontrolled magic nodes, and similar structures,
you are always given an indication of what opponent to expect in combat if you enter the square.
You may then decide whether or not your current stack of units is prepared to meet the challenges ahead - before battle is joined.


Page 99  (PDF Page 104)

AFTER IT’S OVER
The following sections contain material on the aftermath of battle. In
addition to this information, you can find material on how undead creatures
come into being in Special Unit Abilities, as the forming of these creatures
is one potential consequence of battles involving creatures of death.
Treasure
Artifacts may be picked up from heroes killed in battle
  (unless the heroes died through some mechanism that completely destroyed or banished them including disintegration, banishment, unsummoning and holy word).
There are also sites,
  such as ancient ruins and towers of wizardry that can yield special treasures - even new spell picks.
Finally, when enemy cities are conquered, the conquering wizard gets a portion of the previous owner's gold reserve as loot.


Page   (PDF Page 123)
...
Page   (PDF Page 125)

TERRAIN SPECIALS (MINERALS, BIOTA, NODES AND STRUCTURES)

TERRAIN SPECIAL     MILITARY UNIT COST  GOLD  MAGIC POWER  SPECIAL EFFECTS AND RESTRICTIONS
Ancient Ruins                                              Monster lair**
Ancient Temples                                            Monster lair**
Chaos Nodes                                   * * *        Look like volcanoes**
Nature Nodes                                  * * *        Look like green and white glowing squares**
Sorcery Nodes                                 * * *        Look like radiant blue pools**
Towers of Wizardry                                         Allow planar travel for all units in the tower (use the plane button to shift planes) **
Underground Lairs                                          Monster lair**

** First time capture of these structures generally requires fighting guardian creatures and yields treasures! Treasures include
artifacts, gold, imprisoned heroes, magic power (found as mana crystals), maps, spell picks (as if you had chosen an extra rank
or special ability when creating your wizard) and spells. For towers of wizardry, one of the treasures is always a spell.


MoO2

nothing for "treasure", "loot", "award"

Module: MOX
    data (0 bytes) _monster_names
    Address: 02:00191266

Module: HOMEGEN
    code (0 bytes) Make_System_Monsters_
    Address: 01:0007CDC5

