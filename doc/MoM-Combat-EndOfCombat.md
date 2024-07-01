
WIZARDS.EXE
ovr123
// WZD o123p01
End_Of_Combat__WIP()



Strategic_Combat()
    |-> End_Of_Combat()

Tactical_Combat()
    |-> End_Of_Combat()

Strategic_Combat()
    player_idx is for the winnner (comb attacker or defender)
    item_list and item_count are in-outs, passed empty
    type is {6,7}

Tactical_Combat()
    player_idx is for the winnner (comb attacker or defender)
    item_list and item_count are in-outs, passed empty
    type is {1,2,3,4,5}

handles each combat scenerio
    0 enemy stack
    1 enemy city
    5 monster lair

lots of special handling for neutral player

still not sure about the setting of plane to 9





...
...
...
And, then...
loop through all battle units
    IDA Color: black
    @@Enter_Loop_Battle_Unit__2
    @@Loop_Battle_Unit__2
    @@Next_Battle_Unit__2

1) fix up Lair guardians

¿ battle unit is alive or dead|destroyed ?

    battle units status is some kind of dead

    unit plane is 9

    is neutral player
    and not type 0
    and not type 1 or is type 1 and unit is not fantastic


adds to Experience_Gained and sets Rare_Foe_Defeated
So, ...
    it's ~dead and it's not our unit
    therefore, we killed it


Later, ...

GUI_Multipurpose_Int += Rare_Foe_Defeated
    "You have lost {} fame."
    "You have gained {} fame."


G_DIPL_Action(-Diplomatic_Value, _combat_attacker_player, _combat_defender_player, 8, 0, 0)
; multiple BUGs, unfinished subfunctions, and unknown variables; needs further study!
; registers a diplomatic action


Otherwise, ...
    battle unit is (still) alive
        Extra_Hits = 0
        enchantments = 0
        BU_CombatHits = BU_GetHitsPerFigure()
        TopFig_Dmg -= BU_CombatHits
        _UNITS[].Damage = ((Max_Figures - Cur_Figures) * (hits - BU_CombatHits)) + TopFig_Dmg
        HMoves = 0





unit damage
¿ BU_CombatHits ?





CMB_DrawSummaryText()
CMB_ScrollMsg_Type


### CMB_ScrollMsg_Type

{1,...,12}
Tactical_Combat()
1    - combat victory
2    - combat defeat
3    - combat fled
4    - combat draw
5    - combat enemy fled
Strategic_Combat()
6    - combat victory
7    - combat defeat
¿ {...,8,9,10} {attacker fled, exhausted, defender fled} ?

8-11 - ??? (no extra text)

12   - city lost


Battle Result 1,6
MESSAGE.LBX
Entry 5
"You are triumphant"

Battle Result 2,7
MESSAGE.LBX
Entry 6
"You have been defeated"

Battle Result 3
MESSAGE.LBX
Entry 7
"Your forces have retreated"

Battle Result 4
MESSAGE.LBX
Entry 8
"All units retreat exhausted"

Battle Result 5
MESSAGE.LBX
Entry 9
"Your opponent has fled"

Battle Result 5
WZD dseg:5F8E   " has been conquered"
...
WZD dseg:5FA2   " has been reduced to ruins"
WZD dseg:5FBD   " has fallen to raiders"


XREF:  (11)
    End_Of_Combat__WIP()          mov  [CMB_ScrollMsg_Type], ax
    Combat_Results_Scroll()       cmp  [CMB_ScrollMsg_Type], 4
    Combat_Results_Scroll()       cmp  [CMB_ScrollMsg_Type], 1
    Combat_Results_Scroll()       cmp  [CMB_ScrollMsg_Type], 5
    Combat_Results_Scroll()       cmp  [CMB_ScrollMsg_Type], 6
    Combat_Results_Scroll()       cmp  [CMB_ScrollMsg_Type], 1
    Combat_Results_Scroll()       cmp  [CMB_ScrollMsg_Type], 5
    Combat_Results_Scroll()       cmp  [CMB_ScrollMsg_Type], 6
    Combat_Results_Scroll_Text()  mov  bx, [CMB_ScrollMsg_Type]
    STK_CaptureCity()             mov  [CMB_ScrollMsg_Type], 1
    STK_CaptureCity()             mov  [CMB_ScrollMsg_Type], 12





### CMB_Population_Lost

XREF:  (7)
    End_Of_Combat__WIP()            mov   [CMB_Population_Lost], 0
    End_Of_Combat__WIP()            mov   [CMB_Population_Lost], 0
    End_Of_Combat__WIP()            mov   [CMB_Population_Lost], _DI_IDK_population_lost
    Combat_Results_Scroll_Text()    cmp   [CMB_Population_Lost], 0
    Combat_Results_Scroll_Text()    push  [CMB_Population_Lost]
    STK_CaptureCity()               mov   [CMB_Population_Lost], 0
    STK_CaptureCity()               mov   [CMB_Population_Lost], 0



### CMB_SelectedUnit

XREF:  (112)
    Tactical_Combat__STUB()
    CMB_PrepareTurn()
    G_BU_SelectUnit()
    CMB_FillTargetMaps()
    CMB_SetActionCursor()
    CMB_DrawFullScreen()
    CMB_DrawActiveUnitW()
    CMB_Draw_GUI_Btns()
    CMB_DrawAUWStats()
    WIZ_BU_SelectNext()
    WIZ_BU_SelectClosest()
    AI_BU_ProcessAction()
    End_Of_Combat__WIP()
    Combat_Results_Scroll_Text()
    STK_CaptureCity()
    CMB_VortexPlayerMove()

For 'Strategic Combat', only relevant references...
    End_Of_Combat__WIP()
    STK_CaptureCity()






/*
    WIZARDS.EXE  ovr123
*/

// WZD o123p01
CMB_Victory(int16_t player_idx, int16_t * item_count, int16_t item_list[], int16_t MsgType);

// WZD o123p02
// CMB_GetUndeadCreator()

// WZD o123p03
// CMB_DrawSummary()

// WZD o123p04
// G_CMB_ShowSummary()

// WZD o123p05
// CMB_DrawSummaryText()

// WZD o123p06
int16_t CTY_GetConquerGold(int16_t city_idx);

// WZD o123p07
int16_t CTY_RampageVictory(void);
