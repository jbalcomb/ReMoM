
...unit’s rolls...
attack, defense, resistance
swords, shields, crosses
plus 10% modifier to hit, plus 10% chance to block, plus 10% modifier resistance checks
+1 ~== +10%





AType ==> attack_mode



Tactical_Combat__WIP()
        j_Battle_Unit_Action__WIP()
            Battle_Unit_Action__WIP()
                |-> j_Battle_Unit_Attack__WIP()
                    |-> Battle_Unit_Attack__WIP()
                        |-> BU_AttackTarget__WIP()
                            |-> BU_ProcessAttack__WIP()



MoO2
Fire_Ship_()
Final_Fire_()
Apply_Ship_Damage_()
Target_Is_Legal_For_Weapon_()
Weapon_In_Range_()
Is_Combat_Ship_Dead_()
Combat_Ship_Is_Functional_()
Destroy_Ship_()


## Where and how does a unit get killed?

BU_ApplyDamage__WIP()


    if(battle_units[battle_unit_idx].Cur_Figures <= 0)
        if(BU_IsCombatSummon__WIP(battle_unit_idx) == ST_TRUE)
            _UNITS[battle_units[battle_unit_idx].unit_idx].wp = 9;
BU_IsCombatSummon__WIP()
    // field_CA8 / 24h = 5Ah  90 Fire_Elemental
    // field_438 / 24h = 1Eh  30 Earth_Elemental  
    // field_870 / 24h = 3Ch  60 Phantom_Beast
    // field_654 / 24h = 2Dh  45 Phantom_Warriors
    // field_948 / 24h = 42h  66 Air_Elemental
    if(
        (spell_data_table[90].Param0 == unit_type)
        ||
        (spell_data_table[30].Param0 == unit_type)
        ||
        (spell_data_table[60].Param0 == unit_type)
        ||
        (spell_data_table[45].Param0 == unit_type)
        ||
        (spell_data_table[66].Param0 == unit_type)
        ||
        (unit_type == ut_Demon)
...depends on damage type
    if battle_units[battle_unit_idx].damage[2]
        battle_units[battle_unit_idx].status = bus_Gone
    if battle_units[battle_unit_idx].damage[1]
        if(_UNITS[battle_units[battle_unit_idx].unit_idx].wp != 9)
            battle_units[battle_unit_idx].status = bus_Drained
        else
            battle_units[battle_unit_idx].status = bus_Gone
    if battle_units[battle_unit_idx].damage[0]
        battle_units[battle_unit_idx].status = bus_Dead



## Damage Types

¿ damage_array[1] is 'Create Undead' damage ?
    if(((battle_units[attacker_battle_unit_idx].Abilities & UA_CREATEUNDEAD) != 0) && ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0))
        damage_array[1] += (battle_units[defender_battle_unit_idx].hits + Attack_Damage);
    else
        damage_array[0] += (battle_units[defender_battle_unit_idx].hits + Attack_Damage);

"Doom Damage"
"Armor Piercing Damage"  (//FanDom AKA "Lightning Damage)



***TABLE N: ATTACK TYPES***
Attack Type
Timing Relative to Melee Combat
In Addition to Melee?
Offensive Only?
Defense to Attack

Before Melee & No
Gaze

Before Melee & Yes
Breath
First Strike Weapons
Thrown

Simultaneous & No
Lifestealing Attacks
Poison Attacks
Touch

Simultaneous & Yes
N/A

Melee === Melee

Instead of Melee & Yes
Ranged

If a unit is listed as "Yes" in the Offensive Only category,  
  its ability only applies in melee situations where it is the attacking unit  
  (i.e., it is currently the active unit expending half its movement allowance to initiate the melee attack).  
A "No" means that the indicated attack type is used in melee combats in both offensive and defensive situations  
  (i.e., this attack can also occur when a unit is defending itself against an enemy's attack).





## battle_units[].Suppression
CMB_PrepareTurn__WIP() sets battle_units[].Suppression to 0, for all combat battle units
Load_Battle_Unit() sets battle_unit->Suppression to 0
BU_ProcessAttack__WIP()
    if(Counter == ST_TRUE)
        To_Hit -= (battle_units[attacker_battle_unit_idx].Suppression / 2);
Battle_Unit_Attack__WIP()
    battle_units[defender_battle_unit_idx].Suppression += 1;




## BU_AttackTarget__WIP()

void BU_AttackTarget__WIP(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t Target_Damage_Array[], int16_t Counter_Damage_Array[], int16_t ranged_attack_flag, int16_t SpFx);

SpFx
    if(SpFx != 0)
        BU_WallofFire__NOOP(attacker_battle_unit_idx);
    ...
    Battle_Unit_Attack__WIP()
        if(defender_battle_unit_idx != 99)
            ...ranged or non-ranged...
            BU_AttackTarget__WIP(attacker_battle_unit_idx, defender_battle_unit_idx, &defender_damage_array[0], &attacker_damage_array[0], ranged_attack_flag, 1);


Target_Damage_Sum
    accumulates damage after non-ranged attack phase, for bue_Black_Sleep and/or {srat_StoneGaze, srat_MultiGaze, srat_DeathGaze}, including second attack for Haste

Feared_Figures
    for melee/non-ranged defender counter-attack #2, count of figures not presumed dead and not 'Cause Fear'd
    for melee/non-ranged attacker melee non-FirstStrike, count of figures not presumed dead and not 'Cause Fear'd

Figs
    for melee/non-ranged defender counter-attack #2, count of figures not presumed dead and not 'Cause Fear'd

NOTE: both `Feared_Figures` and `Figs` get used as `figure_count` for BU_ProcessAttack__WIP()


    ranged_attack_check = Check_Attack_Ranged(attacker_battle_unit_idx, defender_battle_unit_idx);
    if(ranged_attack_flag != ST_TRUE)

    // ~Thrown
    // attack type 0
    // srat_Thrown, srat_FireBreath, srat_Lightning, srat_StoneGaze, srat_MultiGaze, srat_DeathGaze
    BU_ProcessAttack__WIP(attacker_battle_unit_idx, battle_units[attacker_battle_unit_idx].Cur_Figures, defender_battle_unit_idx, 1, &Damage_Array[0], 0, SpFx);

    // Haste AND {srat_Thrown, srat_FireBreath, srat_Lightning} NOT {srat_StoneGaze, srat_MultiGaze, srat_DeathGaze}
    // attack type 0
    BU_ProcessAttack__WIP(attacker_battle_unit_idx, battle_units[attacker_battle_unit_idx].Cur_Figures, defender_battle_unit_idx, 1, &Damage_Array[0], 0, SpFx);

    ...

    // Counter-Attack

    ...

    // Wall of Fire, First-Strike(Cause Fear, Melee)

    ...

    // Counter-Attack

    if defender not 'Black Sleep'

    if defender has 'Haste'

    ...

    // Melee  (non-First-Strike)



...
...
...




## BU_ProcessAttack__WIP()

void BU_ProcessAttack__WIP(int16_t attacker_battle_unit_idx, int16_t figure_count, int16_t defender_battle_unit_idx, int16_t attack_mode, int16_t damage_array[], int16_t Counter, int16_t SpFx);

attack_mode
    0: Melee
    1: Thrown/Breath
    2: Ranged/Magic


Attack_Type

if(attack_mode <= 0)
    Attack_Type = 0;
else
    Attack_Type = battle_units[attacker_battle_unit_idx].ranged_type;

cmp     [bp+Attack_Type], srat_Lightning



calls Battle_Unit_Defense_Special()

City Walls
    if(
        (Battle_Unit_Is_Within_City(defender_battle_unit_idx) == ST_TRUE)
        &&
        (Battle_Unit_Is_Within_City(attacker_battle_unit_idx) == ST_FALSE)
        &&
        (battlefield->walled != ST_FALSE)
    )
    {
        if(Combat_Grid_Cell_Has_City_Wall(battle_units[defender_battle_unit_idx].cgx, battle_units[defender_battle_unit_idx].cgy) != ST_FALSE)
        {
            defense_special += 3;
        }
        else
        {
            defense_special += 1;
        }
    }

Battle_Unit_Is_Within_City() just checks the coordinates of the battle unit, nothing for if there actually is a city

battlefield->walled != ST_FALSE
...
    CMB_Terrain_Init__WIP()
        City_Walls
        Generate_Combat_Map__WIP(..., City_Walls, ...)
            battlefield->walled = city_walls;
                if(city_walls == ST_TRUE)
                    battlefield->walls[((itr2 * 4) + itr1)] = 1;







srat_Thrown, srat_FireBreath, srat_Lightning, srat_StoneGaze, srat_MultiGaze, srat_DeathGaze


# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #
# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #
# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #

# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #
# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #
# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #

First Block:
    melee OR not melee
        attack_attributes
        Type_Specific_ATK_Flags
        attacker_tohit
        attack_strength
        attack_magic_realm
        attack_type

...
...
...

Defense Special, Eldritch Weapon, City Walls

...
...
...

¿ no more usages of attack_mode ?

...
...
...

¿ no more usages of attack_type ?

...
...
...

...
...
...


# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #
# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #
# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #

# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #
# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #
# ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # ### # #









...
...
...

Dispel Evil (as Touch Attack)
Stone Touch
Death Touch
Life Steal

...
...
...





Death Touch     new_damage_array[0]
Life Steal      new_damage_array[1]
Destruct        new_damage_array[2]

...

Poison Attack
    if(((battle_units[attacker_battle_unit_idx].Abilities & UA_CREATEUNDEAD) != 0) && ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0))
        new_damage_array[1] += 1;
    else
        new_damage_array[0] += 1;

...

    if((Combined_ATK_Flags & Att_Destruct) != 0)
        if(Combat_Resistance_Check(battle_units[defender_battle_unit_idx], 0, sbr_Death) > 0)
            new_damage_array[2] += battle_units[defender_battle_unit_idx].hits;

¿ Word of Death ... Death Realm ... deal 'Irreversible Damage' ... -5 to save ?



...special abilities such as Poison Attacks, Life-stealing, Gaze Attacks, etc.

00000001 ; enum e_ATK_FLAGS (bitfield)
00000001 Att_ArmorPrc  = 1
00000002 Att_1stStrike  = 2
00000004 Att_Poison  = 4
00000008 Att_LifeSteal  = 8
00000010 Att_DoomDmg  = 10h
00000020 Att_Destruct  = 20h
00000040 Att_Illusion  = 40h
00000080 Att_StnTouch  = 80h
00000100 _noeffect100  = 100h
00000200 Att_DthTouch  = 200h
00000400 Att_PwrDrain  = 400h
00000800 Att_DsplEvil  = 800h
00001000 Att_AREAFLAG  = 1000h
00002000 Att_DMGLIMIT  = 2000h
00004000 Att_EldrWeap  = 4000h
00008000 Att_WarpLghtn  = 8000h



## Battle_Unit_Attack__WIP()




Battle_Unit_Attack__WIP()
XREF:
    j_Battle_Unit_Attack__WIP jmp     Battle_Unit_Attack__WIP
j_Battle_Unit_Attack__WIP()
XREF:
    Battle_Unit_Action__WIP+111  call    j_Battle_Unit_Attack__WIP
    Battle_Unit_Action__WIP+150  call    j_Battle_Unit_Attack__WIP
    Battle_Unit_Action__WIP+18E  call    j_Battle_Unit_Attack__WIP
    Battle_Unit_Action__WIP+1CD  call    j_Battle_Unit_Attack__WIP
    Battle_Unit_Action__WIP+20C  call    j_Battle_Unit_Attack__WIP
    AI_BU_ProcessAction__WIP+47B call    j_Battle_Unit_Attack__WIP
    AI_BU_ProcessAction__WIP+64E call    j_Battle_Unit_Attack__WIP
    AI_BU_ProcessAction__WIP+991 call    j_Battle_Unit_Attack__WIP
    Auto_Move_Unit+5E8           call    j_Battle_Unit_Attack__WIP




##### Naming Things Is Hard

shield
one chance (30%)
basic attack (one damage)
...unit’s rolls ... attack, defense, resistance ... plus 10% modifier to hit, plus 10% chance to block, plus 10% modifier resistance checks


Page 75  (PDF Page 80)

Melee Strength

During battle, units can respond to all melee attacks directed against them as well as attack on their own,  
  potentially giving them several opportunities every turn to inflict damage with their melee weapons (sword icons).  
Both the attacker’s and defender’s melee attacks can be defended against; each attack-and-defense segment gets resolved simultaneously (see Combat).  



MELEE ATTACKS
Melee attacks are initiated by moving an active unit into a square
adjacent to an enemy unit. Next, click on the enemy unit (the mouse
cursor appears as a pair of crossed swords) to attack. The fundamentals
of melee attacks and defense are described in the sections Melee
Strength and Defense. Basically, when somebody hits somebody else,
the defender gets to poke back, and attack and response are
simultaneously resolved. The only exceptions are that breath attacks,
gaze attacks, thrown weapons, and units with First Strike (see Special
Unit Abilities) get to act before regular hand-to-hand combat is initiated.
Figures in units or even whole units may die as a result of such attacks—
before ever getting a chance to counterattack.
The only melee combat restriction is that the only way nonflying
units can melee attack flying units is through the use of breath attacks,
gaze attacks, thrown or ranged weapons—from an adjacent square.
Normal melee attacks cannot be initiated by nonfliers against fliers.
However, once a unit has conducted a breath attack, gaze attack or
thrown attack against a flying unit, the flying unit gets angry, descends
and forces the attacker to engage in normal melee combat. See Table
N in the Appendix.


Page 76  (PDF Page 81)

Defense
The armor row in the unit statistics window indicates, by the number of shields, the defense rating of the unit.  
Each shield represents one chance (30%) to stop one successful basic attack (one damage).  
Damage that is not stopped decreases the hit points of the defending figure.  
If the defending figure dies, then the next figure (if any remain in the unit) becomes the defender.  
Note that defense blocks both non-magic and magic melee or ranged attacks.  
The following attribute, resistance, describes a unit’s ability to resist spells only.  



Page 83  (PDF Page 88)

Luck . . . . . . . . . . . . . Adds one to all of unit’s rolls: attack, defense and resistance (i.e., a plus 10% modifier to hit with each sword, a plus 10% chance to block with each shield, and plus 10% modifier for all resistance checks).
