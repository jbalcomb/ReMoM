

battle_units[].defense
battle_units[].toblock
defense_specials

















## Battle_Unit_Defense_Special()

; calculates and returns the unit's effective Defense
; score based on immunities and other conditional
; modifiers (except City Walls and Doom Damage), attack_mode
; only matters if it is melee (value of 0)

// WZD o122p08
int16_t Battle_Unit_Defense_Special(int16_t battle_unit_idx, int16_t attack_type, int16_t attack_immunities, int16_t attack_attributes, int16_t magic_realm)

1. 'Immunity to Illusion' && 'Illusory Attack'
2. 'Large Sheild'
3.  Ranged Attack - 'Immunity...' Fire, Stoning, Missiles, Cold, Death, Poision
3.  Melee Attack - 'Immunity Weapon'
4.  Ranged Attack - 'Immunity Magic'
5.  Chaos, Death - 'Bless'
6.  Ranged Attack - 'Righteousness', 'Bless'
7.  Chaos, Nature - Ranged Attack - 'Elemental Armor', 'Resist Elements'
8.  'Armor-Piercing Attack'

if(Immunity_Type == 1) && (effective_defense < 10)
    effective_defense = 10;
if(Immunity_Type == 2)
    return 50;
...1 gets set for USA_IMMUNITY_WEAPON
...2 gets set for any 1:1 (Ranged Attack & USA_IMMUNITY), USA_IMMUNITY_MAGIC, UE_RIGHTEOUSNESS




XREF:
    BU_ProcessAttack__WIP()
    AI_BU_GetAttackValue__STUB()
    j_Battle_Unit_Defense_Special()
        CMB_ConvSpellAttack__WIP__SEGRAX()
        AI_BU_SelectAction__WIP()

battle_units[battle_unit_idx].defense
battleunit->defense

attack_type
    if not Melee, then Ranged
        ...missile, magic, rocks, thrown, breath 
    ¿ ~== enum e_COMBAT_ATTACK_MODE  { am_Melee = 0, am_ThrownOrBreath = 1, am_Ranged = 2 } ?

attack_immunities

attack_attributes

magic_realm



attack_type

attack_immunities

attack_attributes


### Attributes
Armor-Piercing Attack . . . . . . . . . . . . Halves defense (shields) of defender (rounded down).

Immunity to Illusion . . . . . . . . . . . Negates Illusory bonus of Illusory Attacks;

Illusory Attack . . . . . . Attacker ignores defender’s armor (shields).

Immunity to Spells . . . . . . . . . . . . Unit unaffected by spells of any type or by ranged magic attacks.

Large Shields. . . . . . . Adds two to defense (shields) against all ranged attacks (including missile, magic, rocks). It also adds +2 to defense against thrown and breath attacks.

Weapon Immunity . . . Unit nearly unaffected by attacks from normal units that do not have mithril, adamantium, blessed or flaming weapons; unit’s defense (shields) increases to 10 (if not already at or above 10); this ability has no effect on attacks from fantastic creatures.


### Item Enchantment
Phantasmal Sorcery 6 All - - Illusory Attack

'Immunity Fire', 'Immunity Stoning', 'Immunity Missiles', 'Immunity Illusion', 'Immunity Cold', 'Immunity Magic', 'Immunity Death', 'Immunity Poison', 'Immunity Weapon'
'Immunity Fire'
'Immunity Stoning'
'Immunity Missiles'
'Immunity Illusion'
'Immunity Cold'
'Immunity Magic'
'Immunity Death'
'Immunity Poison'
'Immunity Weapon'
#define USA_IMMUNITY_FIRE       0x0001  // 0b0000000000000001  0x0001      1  "Fire Immunity"
#define USA_IMMUNITY_STONING    0x0002  // 0b0000000000000010  0x0002      2  "Stoning Immunity"
#define USA_IMMUNITY_MISSILES   0x0004  // 0b0000000000000100  0x0004      4  "Immunity to Missiles"
#define USA_IMMUNITY_ILLUSION   0x0008  // 0b0000000000001000  0x0008      8  "Immunity to Illusion"
#define USA_IMMUNITY_COLD       0x0010  // 0b0000000000010000  0x0010     16  "Immunity to Cold"
#define USA_IMMUNITY_MAGIC      0x0020  // 0b0000000000100000  0x0020     32  "Immunity to Spells"
#define USA_IMMUNITY_DEATH      0x0040  // 0b0000000001000000  0x0040     64  "Immunity to Death Magic"
#define USA_IMMUNITY_POISON     0x0080  // 0b0000000010000000  0x0080    128  "Poison Immunity"
#define USA_IMMUNITY_WEAPON     0x0100  // 0b0000000100000000  0x0100    256  "Weapon Immunity"
