


Active Unit Window
Spell Information Window
Defender Enchantment Window
Attacker Enchantment Window
Combat Unit Display
Combat Information Window



..."DEF"..."ATK"...
MoO2
Module: COLGCBT
    data (0 bytes) _N_DEF_UNIT_ROWS
    Address: 02:0017A8CE
    data (0 bytes) _N_ATK_UNIT_ROWS
    Address: 02:0017A8CF



move to move path notes
in Eval Move Path
¿ if no units in the army have a status of going-to, then the Combat_Path_Value flag is meaningless ?
¿ which means the Stop_Short flag is meaning less ?
But, then what happens when you are moving one space onto an enemy/lair?
That whole block just cancels the Go-To and then maybe stops the path short of the combat target square
...
computer player doesn't have go-to?
computer player handles go-to + combat differently, elsewhere?
stopping short is just for handling go-to
 - it uses the combat flag to decide to stop short, but unsets it because stopping short means it doesn't enter combat

OVL_Action_Structure is Lair Index or City Index
attackee_idx = Obstacle_Value is Player Index for Lair/City or Unit Index
OVL_Action_Type is 0 for Unit, 1 for City, 5 for Lair
So, ...
    no good reason for storing the attackee_idx and OVL_Action_Structure differently?
per the code structure they belong to different parts of the code?





Page 91  (PDF Page 96)
Combat and Spell Casting
Combat

The spell information window is devoted to statistical information on your wizard’s spell casting abilities for the current combat.
...
Range is a distance modifier for the cost of casting spells in battles that are far from the wizard’s enchanted fortress (see Spells in Combat).

Page 97  (PDF Page 102)
SPELLS IN COMBAT
...

Spells cast during combat generally cost more than they would if they were cast overland.
Because a wizard’s power base is situated in his or her enchanted fortress, battles often require a wizard to “channel” magic power for casting spells from this power base to a remote battle location.
This channeling process is very wasteful of mana
  (except for Channelers, see Special Abilities),
  and the farther away the battlefield is from the enchanted fortress, the more expensive casting combat spells becomes.
The basic rules are that
 1) when battles take place within five map squares of the wizard’s enchanted fortress, combat spells do not cost extra, and
 2) the maximum a combat spell can cost is three times its normal casting cost.
The actual casting cost multiplier for spells cast during the combat is shown in the spell information window (see Combat) next to the word “Range”.
Example: A range of 1x (times one) means that you spend no additional mana when casting a combat spell.
If the modifier shows a 2x (times two), you reduce the skill number for this battle by the mana cost of the spell,
  but extract double the mana from your mana reserve, due to the distance penalty representing the mana wasted by channeling.

A wizard’s spell casting skill limits how much total magic power the wizard can use to cast spells during combat.
However, the extra mana that it costs to cast a spell because of the distance of the battlefield from the wizard’s fortress does not count toward that limit
  (it is merely charged as a penalty against his or her mana reserves).






# Combat - ReMoM (Master of Magic v1.31 (1995))


main()
    |-> main2()
            |-> Screen_Control()
...
Main_Screen()
    |-> Move_Stack()
        |-> Move_Units()
...
EZ_Resolve()
    |-> Do_Lair_Confirm()  // "... Do you wish to enter?"
        |-> Lair_Confirm()
    |-> EZ_ResolveEntry()
        |-> OVL_ResolveConflict()

combat == {FALSE,TRUE}
    combat == FALSE
        ...
    combat == TRUE
        ...
        combat type == {Lair,Enemy City/Unit}
            combat type == Lair
                ...
            combat type == Enemy City/Unit
                ...

Code-Flow:
    if combat
    (else fallthrough)
    if combat
        if lair
            if 0
            if 1
            if 99
            (else fallthrough)



...tie back to movement code notes...
B&E of Code-Block
    ...
    ...
    ...
    @@EndOfMoving:
    (4) paths
    1) No Attack / Non Combat Move ... ~ Skip-It / Do-Nothing
    Combat_Handlers:
    2) Attack & Lair - {0,1,99}  ...  2) Yay-Attack; (Attack)/Move Type == Lair (~ Neutral Player)
        Yay-Attack; Yay-Lair; EZ_Resolve(); Yay-Lose;
        Yay-Attack; Yay-Lair; EZ_Resolve(); Nay-Lose; Nay-Win;
        Yay-Attack; Yay-Lair; EZ_Resolve(); Nay-Lose; Yay-Win;
2) Yay-Attack; Yay-Lair;
EZ_Resolve()
Nay-Lose
Nay-Win
Nay-Cancel

    3) Yay-Attack; Nay-Lair; Nay-Win;
    4) Yay-Attack; Nay-Lair; Yay-Win;

So, ...
    ¿ complete separation at {Nay-Attack, Yay-Attack} ?
    ¿ Lair vs. [City | Unit] ?
    ...something meaningful about the branches that update the troops[] 




## Who, What, When, Where, How

Player moves 'Owned Stack' onto 'Enemy Stack'
Player moves 'Owned Stack' onto 'Enemy City'
Player moves 'Owned Stack' onto 'Neutral City'
Player moves 'Owned Stack' onto 'Lair'
...
Human-Player ==> Computer-Player
Computer-Player ==> Human-Player
¿ Neutral-Player ?



Code starts in Main Screen Movement?
...and Next-Turn for CP?

Main_Screen()
    |-> Move_Stack()
        |-> Move_Units()

cmp     [bp+attack_flag], e_ST_TRUE     ; Combat_Move == ST_TRUE

mov     [CRP_OVL_Obstacle_Var1], e_ST_FALSE


mov     ax, [bp+First_Unit_Index]
mov     [CRP_OVL_Obstacle_Var2], ax


mov     ax, [bp+attackee_idx]
mov     [CRP_OVL_Obstacle_Var3], ax

cmp     [OVL_Action_Type], 5            ; sets in STK_EvaluatePath()
                                        ; set to 0 when pathing onto an enemy stack
                                        ; set to 1 when pathing onto an enemy city
                                        ; set to 5 when pathing onto a lair

; Moved at a Lair
push    [bp+player_idx]                 ; Player_Index
push    [OVL_Action_Structure]          ; Zone_Index
call    j_EZ_Resolve                    ; resolves movement onto the tile of an encounter zone,
                                        ; from the entry dialog to the treasure rewards
                                        ; returns 1 if the encounter was cleared, 0 if not,
                                        ; and 99 if entry was cancelled by the player



First_Unit_Index = unit_idx;                // unit_idx of passed in troops[0]
CRP_OVL_Obstacle_Var1 = ST_FALSE;           // cmp'd in Move_Stack(), if FALSE, calls empty function pointer; set to FALSE if OVL_MapVar3 is TRUE, right before call to Move_Units()
CRP_OVL_Obstacle_Var2 = First_Unit_Index;   // OON XREF;  CRP_OVL_Obstacle_Var2 = First_Unit_Index = _DI_unit_idx = troops[0]
CRP_OVL_Obstacle_Var3 = attackee_idx;       // OON XREF;  CRP_OVL_Obstacle_Var3 = attackee_idx = Obstacle_Value = dst_troops[0] || s_CITY.owner_idx || NEUTRAL_PLAYER;  set in Eval_Move_Path__WIP(); 

dseg:C042 00 00                                           OVL_MapVar3 dw 0                        ; DATA XREF: Main_Screen+6Cw ...
dseg:C044 00 00                                           CRP_OVL_Obstacle_Var1 dw 0              ; DATA XREF: Main_Screen+66w ...

dseg:C5DC                                                 ¿ BEGIN: Move_Stack() || UNITMOVE ?
dseg:C5DC                                                 BEGIN: ovr095

dseg:C5EC 00 00                                           CRP_OVL_Obstacle_Var3 dw 0              ; DATA XREF: Move_Units+50Dw
dseg:C5EE 00 00                                           CRP_OVL_Obstacle_Var2 dw 0              ; DATA XREF: Move_Units+507w

OVL_SWardTriggered

OVL_Action_OriginY
OVL_Action_OriginX



### CRP_OVL_Obstacle_Var1

in ¿ BEGIN: Main_Screen - Uninitialized Data ?

some relationship between CRP_OVL_Obstacle_Var1 and OVL_MapVar3
    cmp'd in Move_Stack(), if FALSE, calls empty function pointer; set to FALSE if OVL_MapVar3 is TRUE, right before call to Move_Units()
    do first time / always /override flags?
    move map related?

### CRP_OVL_Obstacle_Var2
¿ ~== combat attacker unit ?

### CRP_OVL_Obstacle_Var3
¿ ~== combat defender unit ?


### OVL_Action_Type
### OVL_Action_Structure

dseg:9284 00 00                                           OVL_Action_Type dw 0                    ; DATA XREF: CMB_TacticalCombat:loc_75EC4r ...
dseg:9284                                                                                         ; sets in Eval_Move_Path__WIP() (AKA STK_EvaluatePath)
dseg:9284                                                                                         ; set to 0 when pathing onto an enemy stack
dseg:9284                                                                                         ; set to 1 when pathing onto an enemy city
dseg:9284                                                                                         ; set to 5 when pathing onto a lair
dseg:9286                                                 ; int OVL_Action_Structure
dseg:9286 00 00                                           OVL_Action_Structure dw 0               ; DATA XREF: EZ_Resolve+92r ...
dseg:9286                                                                                         ; index of the City or Lair being pathed into



EZ_Resolve()
    |-> Do_Lair_Confirm()  // "... Do you wish to enter?"
    |-> EZ_ResolveEntry()
        |-> OVL_ResolveConflict()










Tactical_Combat__WIP()
    |-> CMB_Units_Init__WIP()
        |-> Deploy_Battle_Units()
            |-> Undeployable_Battle_Units_On_Water()


## CMB_Units_Init__WIP()
OON XREF:  Tactical_Combat__WIP()

    Load_Battle_Unit(); Combat_Figure_Load(); Deploy_Battle_Units();
    inits battle units, load figure picts, sets position & orientation


XREF:
    j_CMB_Units_Init__WIP()
        Tactical_Combat__WIP()





selected unit vs. scanned unit
selected/active unit is shown in the "active unit window"
scanned unit is shown in the "combat unit display"
    ..."if you have toggled on the additional unit information button in your game settings window"





## CMB_SelectedUnit

XREF:  (112)
    Tactical_Combat__WIP()
    CMB_PrepareTurn__WIP()
    G_BU_SelectUnit()
    Assign_Combat_Grids()
    CMB_SetActionCursor__WIP()
    Tactical_Combat_Draw()
    CMB_DrawActiveUnitW()
    Tactical_Combat_Draw_Buttons()
    CMB_DrawAUWStats()
    Next_Battle_Unit()
    WIZ_BU_SelectClosest()
    AI_BU_ProcessAction()
    End_Of_Combat__WIP()
    Combat_Results_Scroll_Text()
    STK_CaptureCity()
    CMB_VortexPlayerMove()







## CMB_CursorBattleUnit

    CMB_SetTargetCursor()
        CMB_CursorBattleUnit = ST_UNDEFINED

    CMB_SetActionCursor__WIP()
        CMB_CursorBattleUnit = ST_UNDEFINED
        CMB_CursorBattleUnit = scanned_battle_unit_idx



## CMB_ActiveUnitFrame




## CMB_SetActionCursor__WIP()

CMB_ActiveUnitFrame = 0;
CMB_TargetFrame = 0;


GUI_CombatWindow.image_num
    GUI_CombatWindow.center_offset = 0;
    GUI_CombatWindow.x1 = SCREEN_XMIN;
    GUI_CombatWindow.y1 = SCREEN_YMIN;
    GUI_CombatWindow.x2 = SCREEN_XMAX;
    GUI_CombatWindow.y2 = SCREEN_YMAX;


            CMB_TargetFrame = 1;

            CMB_TargetFrame_X = Tile_X;

            CMB_TargetFrame_Y = Tile_Y;

            CMB_TargetFrameStage = ((CMB_TargetFrameStage + 1) % 3);





##### Naming Things is Hard

Combat unit display
a combat unit display
upper right corner of the combat screen
active unit

standard city walls
magic walls
a magic wall
Wall Crusher ability

Settings Screen

Auto Unit Information



Page 90  (PDF Page 95)

Combat unit display


Page 92  (PDF Page 97)

Finally,  
  if you have toggled on the additional unit information button in your game settings window (see Settings),  
  you have a combat unit display open in the upper right corner of the combat screen.  
This window can be toggled on and off by pressing L on the keyboard.  
The combat unit display contains some useful information to have available during battle,  
  including the remaining ammunition (if relevant) of the active unit,   
  mana and health of the "front" figure in the active unit.

Page 57  (PDF Page 62)

CITY ENCHANTMENTS AND CITY WALLS

Some specific city enchantments create magic walls around the city  
  (wall of darkness and wall of fire; see Spellbook supplement).  
Only one magic wall can encircle a city at any one time;  
  however, a city can have standard city walls (an option in the production screen; see Buildings)  
  in addition to a magic wall.  
The Wall Crusher ability that some units have only applies to the standard city walls,  
  not to magic ones (see Special Unit Abilities).


Page 85  (PDF Page 90)

Wall Crusher . . . . . . .  
Ability to destroy a section of stone wall 50% of the time if unit is adjacent to the target wall section.  
This probability falls to 25% if the target wall section is attacked at range.  
Units with this ability melee attack adjacent enemy units across wall sections that they destroy that turn.  
When conducting ranged attacks, wall crushers also attack an enemy unit that is in the targeted wall square.  
If the wall is not crushed, the defenders receive the wall’s full benefit against the attack.  
If the wall crumbles, they don’t.  


Page 98  (PDF Page 103)

STRUCTURES INFLUENCING COMBAT

City Walls

City walls prevent all nonflying creatures from entering a city except through the city gates.  
Ranged attacks can be used on garrisoned units cowering behind the walls, but nonflying defenders gain a +3 bonus to their defense (shields) against such attacks.  
Flyers cannot really cower as well and are more exposed to ranged attacks, so they only receive a defense bonus of one shield when "behind" a wall.  
Units that have the special ability Wall Crusher (see Special Unit Abilities) can systematically destroy city walls, allowing attackers greater access to any garrisoned units.  
In addition, nonflying defenders behind destroyed city wall sections only get a +1 bonus to their defense against ranged attacks.  
Note that both magic and city walls can exist concurrently.  
They also have different effects (see Building Types and Spellbook supplement).  
Magic walls may not be crushed; they may only be dispelled.  



MoO2

Net_Next_Turn_()
    |-> Client_Next_Turn_()
        |-> Client_Combat_Loop_()
            _attack_confirmed

Next_Turn_Calc_()
    |-> Search_For_Battles_()
        |-> Do_1_Combat_()

Search_For_Battles_()
Client_Combat_Loop_()
Do_Nonhuman_Attack_Against_Human_()
Do_Net_Combat_Update_()
    |-> Do_1_Combat_()
        |-> Russ_Combat_()
            |-> Tactical_Combat_()
                |-> Combat_Screen_()

_combat_attacker_player
_combat_defender_player

_auto_combat_flag

_combat_init
_start_of_turn
_ship_turn_order



Deploy_Ships_()

Add_Tactical_Combat_Fields_()
Check_For_Winner_()
Draw_Main_Combat_Screen_()

    code (0 bytes) Player_Sighted_Monster_
    Address: 01:00079E4D

    code (0 bytes) Star_Guarded_By_Monster_
    Address: 01:0007A47A

Module: SYS

    code (0 bytes) Space_Monster_Here_
    Address: 01:000A4F09

Module: AIMOVE

    code (0 bytes) Star_Has_Monster_
    Address: 01:000D892B

Module: COLCALC

    code (0 bytes) Monster_At_Star_
    Address: 01:000E57F4

Module: COMBINIT

    code (0 bytes) Combat_Screen_
    Address: 01:000478A2

    code (0 bytes) End_Of_Combat_
    Address: 01:0004B184


Module: COMBAT1

    code (0 bytes) Ship_Controlled_By_Computer_
    Address: 01:00042ABF

    code (0 bytes) Init_Ship_For_Start_Of_Turn_
    Address: 01:00042B70

    code (0 bytes) Do_Combat_Turn_
    Address: 01:00042F7F

    code (0 bytes) Check_For_Winner_
    Address: 01:0004545B

    code (0 bytes) Ship_Done_Check_
    Address: 01:0004590D
