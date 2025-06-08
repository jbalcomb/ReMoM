


Page 90  (PDF Page 95)
"...uncontrolled magic nodes..."
Page 120  (PDF Page 125)
"** First time capture of these structures..."



AI_CullTheWeak()    ==>  AI_Kill_Lame_Units()
AI_Overland_Turn()  ==>  AI_Next_Turn__WIP()
AI_AtWarWith_Players  ==>  _cp_hostile_opponents
AI_Active_War_Count  ==>  _cp_hostile_opponent_count
AI_Count_Active_Wars__WIP  ==>  Player_Hostile_Opponents()
AI_GetThreat_UNIT__WIP() ==>  Effective_Unit_Strength()
AI_OVL_Eval_Assets__WIP()  ==>  AI_Player_Calculate_Target_Values()
AI_Continent_Eval__WIP()
AI_OVL_GetSpellList__WIP()  ==>  AI_Compute_Spells_Info()
AI_OVL_SpellPicker() ==> AI_Spell_Select()
AI_Alchemy()  ==>  AI_Update_Gold_And_Mana_Reserves()
AI_Tax_And_Farmers()  ==> AI_Update_Gold_Income_And_Food_Income()
AI_Balance_Upkeep() == AI_Disband_To_Balance_Budget()



Main_Screen()
...current_screen = scr_NextTurn;
Screen_Control()
    |-> Next_Turn_Proc()
        |-> Next_Turn_Calc()
            |-> AI_Kill_Lame_Units()
                |-> AI_Calculate_Average_Unit_Cost()
            |-> AI_Next_Turn__WIP()
                |-> Allocate_AI_Data()

MoO2
Mean_Ship_Strength_()
Make_Scrap_Ships_Dead_()
Kill_Ships_()
Compute_Empire_Building_Needs_()
...
Compute_Needs_()
Player_War_Level_()

Get_AI_Target_()
    |-> Get_Best_Colony_Target_()
        |-> Enemy_Colony_Worth_To_Player_()



¿ AI *decisions* ?

New City?
New Building?
New Unit?
New Spell?

Attack Eenemy?

Personality
Objective


Diplomacy

    Magic Screen
        diplomatic_relations_idx = ((_players[gem_player_nums[itr_gems]].Dipl.Visible_Rel[_human_player_idx] + 100) / 20);
        Print_Centered(45 + (77 * itr_gems), 53, diplo_state[diplomatic_relations_idx]);
        diplomatic_treaties = _players[gem_player_nums[itr_gems]].Dipl.Dipl_Status[gem_player_nums[itr_treaties]];
        char diplo_state[11][9] =
        {
            "Hate",
            "Troubled",
            "Tense",
            "Restless",
            "Unease",
            "Neutral",
            "Relaxed",
            "Calm",
            "Peaceful",
            "Friendly",
            "Harmony"
        };
        char * _treaty_type_names[5] =
        {
            aNoTreaty,
            aWizardPact_1,
            aAlliance_1,
            aWar,
            aFinalWar
        };
        char aNoTreaty[] = "No Treaty";
        char aWizardPact_1[] = "Wizard Pact";
        char aAlliance_1[] = "Alliance";
        char aWar[] = "War";
        char aFinalWar[] = "Final War";
    Mirror Screen
        strcat(GUI_String_1, _treaty_type_names[_players[_human_player_idx].Dipl.Dipl_Status[mirror_screen_player_idx]]);





## AI_Calculate_Average_Unit_Cost()

does not include Settler; Magic Spirit, Guardian Spirit; Engineer, Trireme, Galley, Catapult, Warship;

Does the maths not look odd?
Yes'm. Cause 4-byte value (long) and Long Division (LDIV).

XREF:
    NX_j_AI_Calculate_Average_Unit_Cost()
    AI_Kill_Lame_Units()



## AI_Kill_Lame_Units()

...as of turn 100...

if((_turn % 25) != 0)
    ...non-human, non-neutral...flag all units in cities...
else
    ...recalculate average unit cost...
    ...non-human, non-neutral...flag all units in cities...
    if(_unit_type_table[_UNITS[troops[itr_troops]].type].cost < (_players[_CITIES[itr_cities].owner_idx].average_unit_cost / 2))
        Kill_Unit(troops[itr_troops], 0);
...every 25 turns...
...maybe, removes 1 unit from each city...
...oldest unit that has a cost less than half of the player's average military unit cost...

¿ BUGBUG should have the same exclusion for non-military units ?



## AI_Next_Turn__WIP()

...sanity check all unit coordinates...make insane units be dead... (owner == -1)

EMM_Map_DataH()
Allocate_AI_Data()

// process a planning phase for each AI in turn,
// evaluating the situation, casting a spell, setting
// production and strategy, and giving orders to all
// available units
For Each Computer Player

AI_Evaluate_War__WIP()



## AI_Evaluate_War__WIP()

¿ ~ MoO2 Player_War_Level_() ?  ... if _player[].contact != ST_FALSE ... _player[].treaty 

_players[].War_Reeval
_players[].Hostility
_players[].Dipl.Contacted
_players[].Dipl.Dipl_Status

reset
    _players[player_idx].Hostility[itr_players] = 0
then
maybe



## AI_Magic_Strategy__WIP

What in the holy-o-fuck is going on here?


## AI_Count_Active_Wars
not "Wars"
just attackable wizards?
that's why it checks peace_duration[] == 0?

AI_Active_War_Count
XREF:
    AI_Count_Active_Wars+7         mov     [AI_Active_War_Count], 0                      
    AI_Count_Active_Wars+3D        mov     bx, [AI_Active_War_Count]                     
    AI_Count_Active_Wars+47        inc     [AI_Active_War_Count]                         
    AI_OVL_SplCat_Picker:loc_E4359 cmp     [AI_Active_War_Count], 0                      
    AI_OVL_SplCat_Picker:loc_E4423 cmp     di, [AI_Active_War_Count]                     
    AI_OVL_SplCat_Picker:loc_E442C push    [AI_Active_War_Count]           ; weight_count
    sub_E882B+F                    cmp     [AI_Active_War_Count], 0                      
    sub_E882B:loc_E885C            push    [AI_Active_War_Count]           ; n           
    sub_EA43C+B                    cmp     [AI_Active_War_Count], 0                      
    sub_EA43C:loc_EA481            push    [AI_Active_War_Count]           ; n           

AI_AtWarWith_Players[]
XREF:
    AI_Count_Active_Wars+43  mov     [AI_AtWarWith_Players+bx], _CX_itr_players
    AI_OVL_SplCat_Picker+826 add     ax, [AI_AtWarWith_Players+bx]             
    AI_OVL_SplCat_Picker+843 add     ax, [AI_AtWarWith_Players+bx]             
    AI_OVL_SplCat_Picker+885 add     ax, [AI_AtWarWith_Players+bx]             
    AI_OVL_SplCat_Picker+8A2 cmp     [AI_AtWarWith_Players+bx], 0              
    sub_EA43C+40             mov     ax, [AI_AtWarWith_Players]                



## AI_Player_Calculate_Target_Values()

iters over all cities
    calculates a value for all cities

iters over all units
    sets a value for all stacks
    sets a value for all garrisons

AI_Enemy_City_Values
AI_Own_City_Values

AI_Enemy_Stacks
AI_Enemy_Stack_Count
AI_NME_Garrison_Strs
AI_Own_Stacks
AI_Own_Stack_Count
AI_Own_Garr_Strs

ai_transport_count

XREF:
    NX_j_AI_OVL_Eval_Assets__WIP()
    AI_Next_Turn__WIP()
    AI_Next_Turn__WIP()

AI_Enemy_City_Values
    ...looks to be used to pick the target for negative city (area) spells
AI_Own_City_Values
    ...assuming this is used to pick the target for negative city (area) spells



## AI_Continent_Eval__WIP()

sets AI_Cont_Own_Str[], AI_Cont_Own_Val[], AI_Cont_Nme_Val[], AI_ContBalances[]


AI_Cont_Own_Str[]
CRP_AI_Cont_Nme_Str[]
AI_Cont_Own_Val[]
AI_Cont_Nme_Val[]
AI_ContBalances[]


AI_Cont_Own_Str[0]
    AI_Cont_Own_Str[stack_wp][landmass_idx] += (AI_Own_Stacks[itr].value / 10);

CRP_AI_Cont_Nme_Str[]
    CRP_AI_Cont_Nme_Str[stack_wp][landmass_idx] += (AI_Enemy_Stacks[itr].value / 10);
    CRP_AI_Cont_Nme_Str[stack_wp][landmass_idx] += ((_LAIRS[itr].guard1_count & 0x0F) * (_unit_type_table[_LAIRS[itr].guard1_unit_type].cost / 10));
    CRP_AI_Cont_Nme_Str[stack_wp][landmass_idx] += ((_LAIRS[itr].guard2_count & 0x0F) * (_unit_type_table[_LAIRS[itr].guard2_unit_type].cost / 10));

AI_Cont_Own_Val[]
    AI_Cont_Own_Val[stack_wp][landmass_idx] = AI_Own_City_Values[itr];
    AI_Cont_Own_Val[stack_wp][landmass_idx] = (_NODES[itr].power * _magic);

AI_Cont_Nme_Val[]
    AI_Cont_Nme_Val[stack_wp][landmass_idx] = AI_Enemy_City_Values[itr];
    AI_Cont_Nme_Val[stack_wp][landmass_idx] = (_NODES[itr].power * _magic);


// ; own str - enemy str - own value
    Continent_Balance = (AI_Cont_Own_Str[stack_wp][landmass_idx] - (AI_Cont_Own_Val[stack_wp][landmass_idx] / 10) - CRP_AI_Cont_Nme_Str[stack_wp][landmass_idx]);
    if(Continent_Balance > 0)
        Continent_Balance -= 25;
        if(Continent_Balance < 0)
            Continent_Balance = 0;
    ...
    AI_ContBalances[0] = CRP_AI_Cont_Nme_Str[0];
    AI_ContBalances[1] = CRP_AI_Cont_Nme_Str[1];
    ...
    AI_ContBalances[stack_wp][landmass_idx] = Continent_Balance;
so, CRP_AI_Cont_Nme_Str[] gets overwritten...



## AI_Pick_Action_Conts__WIP()

needs
    CRP_UNIT_OverlandPath], -1     ; an index into OvlMovePaths_EMS@
    CONTX_CreateChains()            ; maps in the CONTXXX handle into the EMS page frame
                                        ; and (re)creates the continent tile chains
                                        ;
                                        ; BUG: only evaluates the first 60 continents
    CONTX_CreateLChains()           ; maps in the CONTXXX handle into the EMS page frame
                                        ; and (re)creates the transport loading tile chains
                                        ;
                                        ; BUG: only evaluates the first 60 continents
from
    Loaded_Game_Update_WZD()
for
    while(Value_Sum = CONTX_TileChain[wp][Value_Sum] != ST_UNDEFINED)



TBL_Arcanus
TBL_Myrror
_LANDMASSES[wp][player_idx][landmass_idx]
    ((wp * 900) + (player_idx * 180))
Current_Action_Continent ==> landmass_idx

Continent_Values[]
    adds 1 if neutral player city is on square
    adds 10 if other player city is on square

Continent_Values_2[]
    adds 1000, if feeling Jihad and landmass has other player's fortress



## AI_Spell_Select__STUB()

AI_OVL_SpellPicker__STUB()
    |-> AI_Compute_Spells_Info()
    |-> AI_OVL_SplCat_Picker()
    ...
    |-> AI_OVL_PickSummon()
    |-> AI_OVL_PickUnitBuff()
    |-> AI_OVL_PickCityBuff()
    |-> AI_OVL_PickDise()
    |-> AI_OVL_PickDisj()
    |-> AI_OVL_PickCurse()
    |-> AI_OVL_PickRealmSupr()
    |-> AI_OVL_PickGlobal()
    ...
    |-> Cast_Spell_Overland_Do()

## AI_Compute_Spells_Info()

sets CRP_AI_OVL_SpellList[], CRP_AI_OVL_SpellCount, and AI_OVL_Spell_Cats[]

CRP_AI_OVL_SpellList[] and CRP_AI_OVL_SpellCount are defunct

AI_OVL_Spell_Cats{} is used in AI_OVL_SplCat_Picker()



## AI_Update_Magic_Power()




## AI_ReleaseGlobals__WIP()




## AI_Update_Gold_And_Mana_Reserves()




## AI_Update_Gold_Income_And_Food_Income()




## Player_All_Colony_Autobuild()

For Each City...



## AI_SetEnemyStrMaps__WIP()

sets bit for non-hostile
sets bit for city/lair/node

clears CONTX_NmeStrMap[]

populates CONTX_NmeStrMap[]
    all opponent's unit strength
    sets bit for non-hostile
    all lair's guardian strength
    sets bit for cities
    adds one to strength for each opponent city  ¿ just so cities with empty garrisons appear ?
    sets bit for lairs
    sets bit for nodes

CONTX_NmeStrMap[]

Where/How/Why does this get used?

54 XREFs

gets used in AI_Pick_Conts__WIP()
    is it just reused memory?



## AI_Continent_Reeval__WIP()

Sum_City_Y_NonOwn_UnitCost
Sum_City_X_Own_UnitCost
local, multi-use variables

    _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_Invalid;  // actually just 0, cause its about to just be a city count
    Sum_City_X_Own_UnitCost[wp][landmass_idx] = 0;
    Sum_City_Y_NonOwn_UnitCost[wp][landmass_idx] = 0;

    Sum_City_X_Own_UnitCost[wp][landmass_idx] += _CITIES[itr].wx;
    Sum_City_Y_NonOwn_UnitCost[wp][landmass_idx] += _CITIES[itr].wy;

    _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = (Sum_City_X_Own_UnitCost[wp][landmass_idx] / city_count);
    _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = (Sum_City_Y_NonOwn_UnitCost[wp][landmass_idx] / city_count);

    Sum_City_X_Own_UnitCost[0][itr] = 0;
    Sum_City_X_Own_UnitCost[1][itr] = 0;
    Sum_City_Y_NonOwn_UnitCost[0][itr] = 0;
    Sum_City_Y_NonOwn_UnitCost[1][itr] = 0;

    if(_UNITS[itr].owner_idx == player_idx)
        Sum_City_X_Own_UnitCost[wp][landmass_idx] += _unit_type_table[unit_type].cost;
    else
        Sum_City_Y_NonOwn_UnitCost[wp][landmass_idx] += _unit_type_table[unit_type].cost;


sets



    _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = (Sum_City_X_Own_UnitCost[wp][landmass_idx] / city_count);
    _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = (Sum_City_Y_NonOwn_UnitCost[wp][landmass_idx] / city_count);

    _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;

    _ai_continents.plane[0].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;
    _ai_continents.plane[1].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;

    AI_NewColConts[wp][player_idx] = Target_Tile_Landmass;
    AI_NewColTgtXs[wp][player_idx] = Target_Tile_X;
    AI_NewColTgtYs[wp][player_idx] = Target_Tile_Y;
    else
    AI_NewColConts[wp][player_idx] = 0;

    _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = Target_Tile_X;
    _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = Target_Tile_Y;
    else
    _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = 0;
    _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = 0;
    _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;

    _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = Target_Tile_X;
    _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = Target_Tile_Y;
    else
    _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = 0;
    _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = 0;
    _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;

    _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = Target_Tile_X;
    _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = Target_Tile_Y;





## Allocate_AI_Data()

    AI_Own_City_Values       = Near_Allocate_First(200);
    AI_Enemy_City_Values     = Near_Allocate_Next(200);
    AI_Own_Garr_Strs         = Near_Allocate_Next(200);
    AI_NME_Garrison_Strs     = Near_Allocate_Next(200);
    AI_Own_Stacks            = Near_Allocate_Next(800);
    AI_Enemy_Stacks          = Near_Allocate_Next(1440);
    AI_OVL_Spell_Cats        = Near_Allocate_Next(92);
    CRP_AI_OVL_SpellList     = Near_Allocate_Next(50);
    AI_Arc_Cont_Own_Str      = Near_Allocate_Next(120);
    AI_Myr_Cont_Own_Str      = Near_Allocate_Next(120);
    CRP_AI_Arc_Cont_Nme_Str  = Near_Allocate_Next(120);
    CRP_AI_Myr_Cont_Nme_Str  = Near_Allocate_Next(120);
    AI_Arc_Cont_Own_Val      = Near_Allocate_Next(120);
    AI_Myr_Cont_Own_Val      = Near_Allocate_Next(120);
    AI_Arc_Cont_Nme_Val      = Near_Allocate_Next(120);
    AI_Myr_Cont_Nme_Val      = Near_Allocate_Next(120);
    AI_Arc_ContBalances      = CRP_AI_Arc_Cont_Nme_Str;
    AI_Myr_ContBalances      = CRP_AI_Myr_Cont_Nme_Str;

Allocate_Data_Space()
    AI_Arc_MainWarConts = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x17E8) );  // 12 bytes
    AI_Myr_MainWarConts = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x17F4) );  // 12 bytes
    AI_CONTX_Reevals    = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1800) );  // 16 bytes
    Wiz5_Spell_28h      = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1810) );  // 20 bytes
    Wiz5_Spell_3Ch      = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1824) );  // 20 bytes
    Wiz5_Spell_50h      = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1838) );  // 20 bytes
    Wiz5_Spell_64h      = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x184C) );  // 20 bytes
    AI_Arc_NewColConts  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1860) );  // 20 bytes
    AI_Myr_NewColConts  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1874) );  // 20 bytes
    AI_Arc_NewColTgtXs  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1888) );  // 20 bytes
    AI_Myr_NewColTgtXs  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x189C) );  // 20 bytes
    AI_Arc_NewColTgtYs  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x18B0) );  // 12 bytes
    AI_Myr_NewColTgtYs  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x18BC) );  // 12 bytes
    AI_SCircle_Reevals  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x18C8) );  // 16 bytes










Page 30  (PDF Page 35)

Magic

Determine the status of your relationship with a wizard. Below the
wizards’ faces, phrases like “neutral” or “hate” summarize the
current status of your relationship with a particular wizard.

Observe the status of wizards’ relationships with each other. To the
right of each wizard’s portrait you may see a series of icons
including scrolls, doves and crossed swords. These icons are colorcoded
to match the banner colors of the other wizards, including
yours. A scroll icon indicates that there is a wizard pact or alliance
in effect between the wizard in the portrait and the wizard whose
banner color matches the ribbon on the icon. Similarly, the crossed
swords icon indicates a state of war exists, while a peace symbol
icon indicates a peace treaty is currently in effect. If no icon
appears, those rivals are in a neutral state (i.e., not at war, nor
under the terms of a formal peace treaty, wizard pact or alliance).
See The Art of Diplomacy for more details.









https://masterofmagic.fandom.com/wiki/AI_to_AI_Diplomacy#End_of_turn_process

If an AI wizard is banished, it does no AI to AI diplomacy. Otherwise, at end of every turn, the following process is executed by each AI wizard. For the purpose of this section, the AI wizard currently making the decision will be called the "actor" or "acting wizard".  

First, if there is an AI ally who is at war with another AI wizard, the actor joins in by declaring War on that other wizard immediately.  
In the 1.50 unofficial patch, this feature is removed: Instead the actor will consider helping allies when checking for the Need for War.  
The primary differences this creates for 1.50 are:

 * The actor respects the Peace Counter, abstaining from going to war shortly after a Peace Treaty or being successfully threatened.
 * Joining the war isn't necessarily immediate, since the chance of war on each check is less than 100%.
 * The actor may help the human player if they are an ally, or declare War on them if they are an ally's enemy.

Afterwards, towards each other AI wizard in the game with whom it has Diplomatic Contact, the acting wizard does the following:  

 1. On most turns, it will randomly decide to skip all subsequent steps towards that wizard that turn and take no action at all. Specifically, if and only if a random number between 1 and 15-(2*Difficulty Level) is exactly 1, it will continue considering further actions.
Note that doing nothing at all is poor design, because it means the AI will not check for the Need for War even though that procedure was designed to be used every turn. Instead, it'll only be called once every ~10 turns, significantly reducing the chances of AI players Breaking Treaty or Declaring War on each other. Due to bugs that cause them to Declare War on uninvolved players for no reason at all, this bug isn't noticeable. In the 1.50 unofficial patch, this chance skips to the check for the Need for War instead.  
 2. Calculate an Action Score (AS) determining the chances of subsequent diplomatic interaction. All variables are for the acting wizard toward the other wizard.  
    * Until patch 1.50, AS = Hidden Relation + Visible Relation + Personality Modifier + random(1-100) + random(4-9)*Difficulty Level + Treaty Interest + 10*Wizard's Pact + 20*Alliance  
    * After patch 1.50, AS = Hidden Relation + Visible Relation + Personality Modifier + random(1-50) + random(3-9)*Difficulty Level + Treaty Interest/4 + 10*Wizard's Pact + 20*Alliance instead.  
 3. In patch 1.50 only, jump to the Diplomatic Reaction check below if there is a War between the two wizards.  
 4. If Action Score>150 and there is no Alliance present, form an Alliance. In patch 1.50, instead check if Action Score>170.  
 5. If Action Score>150 and there is an Alliance present, form a Wizard's Pact, replacing the Alliance. In patch 1.50, instead check if Action Score>112, and there must be no Alliance present (so the Wizard's Pact will not replace an Alliance).  
 6. If Action Score > Trade Interest+Treaty Interest+80, and there is a pair of spells available for trade, trade those spells. Note that the higher Trade Interest is, the less likely it is for this formula to result in a trade. In patch 1.50, instead check if Action Score+Trade Interest>=150. However, in all game versions, there is a bug in which the spells selected are assigned to the wrong wizards, so each party only receives the spell it already had, accomplishing nothing.  
 7. At a 5% chance, execute a +5 strength Diplomatic Reaction.  
 8. If there is a War, and Peace Interest+(a totally unrelated variable containing -1 to 3)>100, form a Peace Treaty. This is most certainly a buggy formula, as it uses a variable used to select text lines during Player to AI Diplomacy, and almost never meets the condition. In patch 1.50, this is fixed to check Action Score+Peace Interest>0 instead. However, it will use the other wizard's Peace Interest variable, not the proposing party's, to simulate the mutual agreement needed to sign the treaty.  
 9. Reduce Treaty Interest, Peace Interest and Trade Interest by 30. In patch 1.50, Peace Interest is not reduced.  
10. Check the Need for War, which may result in Moving towards War by breaking a treaty or declaring war.  
11. Check if there are any units standing in a location violating the Wizard's Pact or Alliance, the same way as in AI to Player Diplomacy. This check is removed in the 1.50 unofficial patch because the AI is unable to keep their troops away from cities to avoid penalties.  



## MoM OSG

Page 405  (PDF Page 420)

Appendix A

The *Next Turn* Sequence of Play

I. Computer Players Prepare  
    A.  Check to see if the player is under any *Peace Treaties* with any other players (see Chapter 25) and plan its turn accordingly if it is.  
    B. Assign a power value to each continent (contiguous land mass) for every player with units and cities on that continent.  
    C. Cast overland spells.  
    D. Consider troop values and make sure garrisons are either up to snuff or being improved.  
    F. Adjust mana ratios for wand, research, and skill level.  
    G. Eliminate any useless global spells.  
    H. Perform alchemy, if deemed necessary.  
    I. Adjust cities' production of resources, gold, and food.  
    J. Selects new buildings to build, if necessary.  
    K. Plan movement based on each player's need to expand or to press attacks against previously determined objectives, or react to defend from further attack by other players.  
II. Neutral Players' Turns  
    A. Select Neutral Player new building construction, if any.  
    B. Adjust Neutral Player food production, if necessary.  
    C. Determine targets and head toward them.  
    D. Generate raiders and rampaging monsters.  
    E. Eliminate surplus neutral units (i.e., ones that can no longer be supported by the city that generated them).  
    F. Attempt to disband their units in stasis.  
III. Computer Players Maneuver (in player turn order)  
    A. Consider hostility toward other players, states of war and peace, treaties; adjust all temporary Diplomacy Point modifiers by 10 points toward 0, etc. (see Chapter 25).  
    B. Disband unnecessary units.  
    C. Look for nearby Towers of Wizardry for inter-planar movement.  
    D. Interrupt previous orders if adjacent to an enemy force, city or node, and attack them if they are particularly weak.  
    E. Meld with nodes.  
    F. Attack cities.  
    G. Attack lairs.  
    H. Build new cities.  
    I. Garrison cities.  
    J. Node attacks.  
    K. Tower of Wizarcky attacks.  
    L. Centralize armies, assembling efficient forces in a strategic position to react to danger.  
    M. Build roads.  
    N. Garrison nodes.  
    O. Garrison Towers of Wizardry.  
    P. Purify land.  
    Q. Explore new continents.  
    R. Move ships.  
    S. Pick up ships.  

Page   (PDF Page )

Appendix B
...the make of a computer player...

Have you ever met a computer player wizard and wondered how he or she got that way?  
This appendix will tell you how.
Note that the information provided here makes a lot more sense if you're familiar with Chapter 25 (diplomacy).  

Wizard Personality Profiles

Each computer player has a distinct personality type.  
These personalities have a strong influence on most of a computer player's diplomatic decision-making (see Table 25.4), and some have additional quirks, as explained below:  

Peaceful  

Beyond their extremely high +20 Leader Personauty
Modifier, there is nothing particularly
special about a peacefol leader.

Lawful  

Lawful leaders have a mellow+ 10 Leader Personauty
Modifier - until you cross them. They
take great umbrage at an yone breaking treaties
with them, so aU Oath Breaker penalties
wiJJ be doubled when applied to relations ,vith
a Lawful leader. In other words, people who
break their treaties with a Lawful leader will
suffer an immediate -80 Diplomacy Point
penalty (wow!) and have a permanent -10
modifier to that player's Core Reactions toward
them.
Like Maniacal leaders, Lawful leaders also
double all DP losses made on their Diplomatic
Relations Scales. These are cumulative with
the effects described in Table 25.2. so if you're
ever above +50 on a Lawful leader's Diplomatic
Relations Scale, any DP loss with that
wizard will be quadrupled.
Lawful leaders will never attack someone
with whom they currently have a n·eaty. Instead,
they will take a turn to formally break
the treaty first.

Chaotic  

A Chaotic leader's Personality Modifier is
rolled at the beginning of each turn and will
be a random number between -40 and +40.
Consequently, you never can predict what her
mood might be on a given tw-n until you a ttempt
to parlay ,vith her.
A Chaotic leader has a traight two percent
chance each turn that she will just up
and declare war- completely out of the blue
- on a single, random, contacted player with
whom she is not currently at war.

Aggressive  

Besides their -10 Leader Personality Modifier,
aggressive leader arn twice as likely as other
leaders to create settlers a nd p eacefully
expand.

Ruthless  

Aside from their -30 Leader Personauty Modifier,
there is nothing special about a ruthless
leader.

Maniacal  

These wizards aTe mean. They hate everybody
and have a -50 Leader Personauty Modifier.
Furthermore, they halve any Diplomacy Point
gains and double all DP losses made with
tl1em. These are cumulative ,vi tl1 the effects
described in Table 25.2, so that if you're ever
above +50 on a Maniacal wizard's Diplomatic
Relations Scale, any DP loss will be quadrupled!
(Don't ever expect to be popular with
these guys.) Maniacal wizards also take almost
every opportunity to attack (see below).

A Computer Player's Hostile Mentality

A computer player will randomly reevaluate her relations with all of the other players every 3 to 25 turns.  
She does o to consider how she will deal wirb each player until her next reevaluation.  
Computer players make these reevaluations separately, not necessarily all to gether on the same turn.  
Note that you will never see their attitude toward another player spelled out anywhere.  
This is all behind-the-scenes- programmer-stuif that you'll have to infer from actual game-playing experiences.  
    There are four increasing levels of aggression that a computer player can have toward another player: None, Annoyed, Warlike, and Jihad (explained below).  
When a computer player makes this random reevaluation of her hostility toward each player, she begins by resetting her status toward everyone to the lowest level, None.  

Deciding Her Attitude

If that computer player is currently at war with
another player (i.e., the crossed swords icon
is in that player's color on the Magic Summai-
y screen ), the one status is immediately
changed to Wru·like. After this, a check is made
to see if her new, Warlike level is increased to
Jihad. Maniacal leaders have a 40 percent
chance, Ruthless leaders have a 20 percent
chance, and Chaotic leaders have a 10 percent
chance of taking their formal wars extremely
seriously at this time, thus upping
their hostility against that player from Warlike
to Jihad (at least until the next reevaluation
in 3 to 25 turns). While another wizard
is casting the Spell of Mastery, all computer
players declare wai·, become Warlike, and have
a 75 percent chance of going Jihad.
If that computer player cUITently has a
Wizai·d's Pact, Alliance, or Peace Treaty in
effect, her hostility rating stays at one with
that particulru· wizard. Furthermore, she will
conduct herself according to the rules of her
agreement with that player.
However, if that computer player is neither
in a formal war, nor bound by a formal
agreement (Wizard's Pact, Alliance, or Peace
Treaty), then things can get interesting. Under
these circumstances, the reevaluating wizard
receives a Hostility Chance value toward
the wizru·d. This value is determined by one
of the two formulae above:

Computer players who are neither at wai· with nor treaty boW1d to another wizard roll a d100 against their Hostilit-y Chance towai·d the wizard being evaluated.  
U the die roll is less than or equal to their Hostility Chance, her hostility is changed from None to Annoyed and she rolls again.  
If this second d100 roll also lands within the range of their Hostility Chance, forget Annoyed, the reevaluating wizard has increased her hostility to Warlike, and she rolls again!  
If this third d 100 roll also succeeds, it's Jihad time.  
Once any of these d 100 rolls fails to be less than or equal to her Hostility Chance, they stop rolling (until her next reevaluation period in 3 to 25 turns).  

Hostility Chance Formulae

If the present relations are less than zero between these two wizards:  

The Hostility Chance = ([Negative DPs] / 2) + 25 + Leader Personality.  

This means half of the absolute value of their negative DPs (rounded down) + 25.  
For example, if her relations were currently residing at -31 DPs, the absolute value of half of that would be 15.5, rounded down to 15.  
Adding 25 would give a total of 40.  
Thus, the Hostility Chance in this example would be 40, plus the evaluating Leader Personality modifier.  

If the present relations are one or greater between these two wizards:  

The Hostility Chance = ((100 - Positive DPs) / 4) + All Accumulated Oath Breaker Penalties (at +5 per) + Leader Personality  

For example, if the relations level of the wizard being reevaluated was currently + 73 DPs with the reevaluating wizard, the Hostility Chance would be 100 minus 73, which is 27.  
That 27 is then divided by 4, with a result of 6.75, which gets truncated to a 6.  
If the wizard being reevaluated had three outstanding Oath Breaker penalties against the reevaluating wizard, that would add another 15 to the 6, yielding a total Hostility Chance of 21 in this example, plus the evaluating Leader Personality modifier.  

Table B.1 Leader Personality Modifiers for Evaluating Hostility Chances  

...
...
...

*This becomes a +40 modifier instead if the player being considered has any Oath Breaker penalties in effect with a reevaluating Lawful wizard. Yikes!  
**Depending on their current turn's random Leader Personality rating.  


What Their Attitude Means

Of these four states of hostiJity, one means
that the computer player will completely leave
the other wizard alone. She won't cast any
spells against nor attack that player's units or
citi es. However, she st ill considers
ungarrisoned nodes and Towers of Wizardry
fair game ( so be sure to garrison these strategic
locations).
A computer player can become Annoyed
either as described above or automatically
when anything short of taking one of their
cities is done to them (i.e., casting a spell
against her units or cities, attacking her troops,
etc.). This is a sort of tit-for-tat state where
she will start throwing spells back against the
person she is Annoyed with. She will attack
that player's units freely when she thinks she
has the advantage and even attack that
player's cities when she figures that her attacking
force is at least twice as strong as the
city's garrison.
A computer player will adopt a Warlike
state if one of her cities is taken or the evaluated
wizard is casting the Spell of Mastery
( these are both considered acts of wru·). The
wizru·d committing these acts is considered ru1
Oath Breaker against the player whose city
fell ( or all the other wizards, in the case of
casting the Spell of Mastery) if there was a
Wizard's Pact, Alliance, or Peace Treaty existing
between them.  
A computer player's Warlike state is similar to Annoyed, except that she'll take more opportunities to throw hostile spells against her opponent's units and cities and will attack them wherever she can muster roughly even odds.  
Thus, computer players with a Warlike attitude no longer cautiously wait for an advantage before engaging in combat.  
The Jihad state can onJy be reached in the circumstances described in the preceding section or when considering a player who is casting the Spell of Mastery.  
In addition to adopting a Warlike attitude against the player she has decided to launch a Jihad against, that computer player also ignores all previous and future wars and Jihads against other players until the current Jihad status changes.  
A computer player in a state of Jihad will relentlessly attack her enemy ( although always looking for roughly equal odds before engaging in battle).  
She will attack even to the point of mobilizing all of her forces (including most of her garrison troops) to launch as many stacks against the targeted player as possible.  
Note that computer players can, technically, cop a Warlike or Jihad attitude without actually being at war against that player.  
Adopting these attitudes does not cause wizard to declare war im.medjately.  
However, you can bet that a formal war will soon occur once troops start to mobilize and battles are fought.  


Page 183  (PDF Page 192)

Neutral City Placement'

There are 15 (total, for both planes) random neutral cities placed each game.  
Every contiguous land mass has a continental race selected for it.  
Each city on that land mass has a 75 percent chance of being inhabited by that continental race.  
Thus, continents tend to have homogenous races upon them.  
...



Page 371  (PDF Page 372)

***Threaten To Attack***

Computer Player Considerations  

Although we're not providing an exact formula here, we will describe the three variables that go into formulating a computer player's opinion of your strength relative to hers.  
Primarily, she will be most intimidated by the strength of your forces on her Home Continent.  
Her Personality Modifier (as found under the first colunn of Table 25.4) and the value of all the Oath Breaker penalties you have with her also enter the equation.  

Appeasement

Cower

Ignore


Page 373  (PDF Page 374)

Chance of Declaration of War for Invading a Computer Player's Home Continent

Threat Value = Amount of invader's total unit upkeep costs (1 per gold+ 1/2 per mana upkeep) on aggrieved player's Home Continent.  

Paranoia = (Threat Value x 15) / threatened player's total unit upkeep costs (1 per gold + 2 per mana upkeep) on threatened player's Home Continent.  
If threatened player has no forces on her Home Continent, Paranoia = -50.  

War Chance = Paranoia + Current Diplomatic Relations Scale value + threatened player's Personality Modifier (see the first column of table 25.4) + Oath Breaker penalties the threatened player has with the invading player.  

If War Chance is less than -150, the threatened player declares war on the invader and the invader suffers an Oath Breaker penalty if a treaty was broken in the process.  
