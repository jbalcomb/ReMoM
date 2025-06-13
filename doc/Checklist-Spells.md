
scc_Destruction_Spell    ==>  scc_Combat_Destroy_Unit
scc_Fixed_Dmg_Spell      ==>  scc_Direct_Damage_Fixed
scc_Var_Dmg_Spell        ==>  scc_Direct_Damage_Variable
scc_Mundane_Enchantment  ==>  scc_Unit_Enchantment_Normal_Only
scc_Dispel_Spell         ==>  scc_Dispels
scc_Disenchant_Spell     ==>  scc_Disenchants
scc_Disjunction_Spell    ==>  scc_Disjunctions
scc_Target_Wiz_Spell     ==>  scc_Target_Wizard

NOTE: the check on the check-list is only for the human player side, CP/AI is a different beast

NOTE: 'Blur' is not in the manual, but is in the OSG - ¿ added in v1.2 ?



Checklist-Spells.ods
Green   Done-Done
Light-Lime-3    maybe, kinda?
...orangish     coded, but known issues
...purpleish    IDK/IDGI/weirdness

..."Combat Spells"..."Overland Spells"...


*infuseable*
search for "+ mana"


Target Wizard or Wizard's Spell:
(uses 'Magic Screen')
    Cruel Unminding
    Drain Power
    Spell Binding
    Spell Blast
    Subversion

Custom Animation? ...Custom Summon Animation?
    Animate Dead, Raise Dead
    Create Undead


Weird Ones?
    Astral Gate
    Banish
    Chaos Channels
    Change Terrain?
    Create Artifact!    handled special during casting
    Earth Gate
    Earth To Mud        don't have mud code in combat
    Earthquake          lots of extra special code
    Enchant Item!       handled special during casting
    Enchant Road
    Flying Fortress?
    Move Fortress?
    Raise Volcano?
    Recall Hero?
    Resurrection?
    Spell Ward          seem to recall the code tfor the effects being messy
    Summon Champion?
    Summon Hero?
    Summoning Circle?
    Warp Creature
    Warp Node
    Word of Recall?


all overland summons are done
all combat summons are done
all overland/global enchantments are done
all city enchantments are done

    SCC  Description
[x]  -1  scc_None
[x]   0  scc_Summoning
[x]   1  scc_Unit_Enchantment
[x]   2  scc_City_Enchantment_Positive       ¿ ..., Stream of Life, ... ?
[x]   3  scc_City_Enchantment_Negative      ¿ Evil Presence, Famine, 
[x]   4  scc_Direct_Damage_Fixed
[-]   5  scc_Special_Spell
[x]   6  scc_Target_Wizard
[ ]   7  N/A
[ ]   8  N/A
[x]   9  scc_Global_Enchantment
[ ]  10  scc_Battlefield_Spell
[ ]  11  scc_Crafting_Spell
[ ]  12  scc_Combat_Destroy_Unit
[ ]  13  scc_Resistable_Spell
[ ]  14  scc_Unresistable_Spell
[x]  15  scc_Unit_Enchantment_Normal_Only
[ ]  16  scc_Mundane_Curse
[ ]  17  scc_Infusable_Spell
[x]  18  scc_Dispels
[x]  19  scc_Disenchants
[x]  20  scc_Disjunctions
[ ]  21  scc_Counter_Spell
[ ]  22  scc_Var_Dmg_Spell
[ ]  23  scc_Banish_Spell

scc_Summoning
Air Elemental, Angel, Arch Angel, Basilisk, Behemoth, Chaos Spawn, Chimeras, Cockatrices, Colossus, Death Knights, Demon Lord, Djinn, Doom Bat, Earth Elemental, Efreet, Fire Elemental, Fire Giant, Floating Island, Gargoyles, Ghouls, Giant Spiders, Gorgons, Great Drake, Great Wyrm, Guardian Spirit, Hell Hounds, Hydra, Magic Spirit, Nagas, Night Stalker, Phantom Beast, Phantom Warriors, Shadow Demons, Skeletons, Sky Drake, Sprites, Stone Giant, Storm Giant, Unicorns, War Bears, Wraiths

scc_Unit_Enchantment
Berserk, Black Channels, Bless, Cloak of Fear, Elemental Armor, Endurance, Flight, Giant Strength, Guardian Wind, Haste, Immolation, Invisibility, Invulnerability, Iron Skin, Lionheart, Magic Immunity, Path Finding, Planar Travel, Regeneration, Resist Elements, Resist Magic, Righteousness, Spell Lock, Stone Skin, True Sight, Water Walking, Wind Walking, Wraith Form

scc_City_Enchantment_Positive
Altar of Battle, Astral Gate, Cloud of Shadow, Consecration, Dark Rituals, Earth Gate, Flying Fortress, Gaias Blessing, Heavenly Light, Inspirations, Natures Eye, Prosperity, Stream of Life, Summoning Circle, Wall of Darkness, Wall of Fire

scc_City_Enchantment_Negative
Chaos Rift, Cursed Lands, Evil Presence, Famine, Pestilence

scc_Direct_Damage_Fixed
Doom Bolt, Fire Storm, Ice Storm, Star Fires, Warp Lightning

scc_Special_Spell
Animate Dead, Black Wind, Call The Void, Change Terrain, Chaos Channels, Corruption, Cracks Call, Death Wish, Disrupt, Earth Lore, Earth to Mud, Earthquake, Enchant Road, Great Unsummoning, Healing, Incarnation, Lycanthropy, Magic Vortex, Move Fortress, Natures Cures, Plane Shift, Raise Dead, Raise Volcano, Recall Hero, Resurrection, Spell Binding, Spell Of Mastery, Spell Of Return, Spell Ward, Stasis, Summon Champion, Summon Hero, Transmute, Wall of Stone, Warp Creature, Warp Node, Warp Wood, Word of Recall

scc_Target_Wiz_Spell
Cruel Unminding, Drain Power, Spell Blast, Subversion

scc_Global_Enchantment
Armageddon, Aura of Majesty, Awareness, Chaos Surge, Charm of Life, Crusade, Detect Magic, Doom Mastery, Eternal Night, Evil Omens, Great Wasting, Herb Mastery, Holy Arms, Just Cause, Life Force, Meteor Storms, Natures Awareness, Natures Wrath, Planar Seal, Suppress Magic, Time Stop, Tranquility, Wind Mastery, Zombie Mastery */
scc_Battlefield_Spell   = 10,   /* COMBAT:  Black Prayer, Blur, Call Chaos, Call Lightning, Darkness, Death Spell, Entangle, Flame Strike, High Prayer, Holy Word, Mana Leak, Mass Healing, Mass Invisibility, Metal Fires, Prayer, Terror, True Light, Warp Reality, Wrack

scc_Crafting_Spell

scc_Combat_Destroy_Unit
Disintegrate, Dispel Evil, Petrify, Word of Death

scc_Resistable_Spell
Black Sleep, Confusion, Creature Binding, Vertigo, Weakness

scc_Unresistable_Spell
Mind Storm, Web

scc_Unit_Enchantment_Normal_Only
Eldritch Weapon, Flame Blade, Heroism, Holy Armor, Holy Weapon

scc_Mundane_Curse       = 16,   /* COMBAT:  Possession, Shatter */

scc_Dispels        = 18,   /* COMBAT:  Dispel Magic, Dispel Magic True */
scc_Disenchant_Spell    = 19,   /* ¿ BOTH ?  Disenchant Area, Disenchant True */
scc_Disjunctions   = 20,   /* OVERLAND:  Disjunction, Disjunction True */
scc_Counter_Spell       = 21,   /* COMBAT:  Counter Magic */
scc_Var_Dmg_Spell       = 22,   /* COMBAT:  Fire Bolt, Fireball, Ice Bolt, Life Drain, Lightning Bolt, Psionic Blast */
scc_Banish_Spell        = 23    /* COMBAT:  Banish */


Spreadsheet - OSG Spells by Category
[ ] Creature Summoning Spells
[ ] City Enchantments
[ ] Creature Enchantments
[x] Global Enchantments
[ ] Combat Spells
[ ] Special Spells

Spreadsheet - OSG Spells by Realm
[ ] Arcane
[ ] Chaos
[ ] Death
[ ] Life
[ ] Nature
[ ] Sorcery



[ ] Earth to Mud
[ ] Resist Elements
[x] Wall of Stone
[x] Giant Strength
[ ] Web
[x] War Bears
[x] Stone Skin
[x] Water Walking
[x] Sprites
[ ] Earth Lore
[ ] Cracks Call
[ ] Natures eye
[x] Ice Bolt
[x] Giant Spiders
[ ] Change Terrain
[ ] Path Finding
[x] Cockatrices
[ ] Transmute
[ ] Natures Cures
[x] Basilisk
[x] Elemental Armor
[ ] Petrify
[x] Stone Giant
[x] Iron Skin
[x] Ice Storm
[ ] Earthquake
[x] Gorgons
[ ] Move Fortress
[x] Gaias Blessing
[x] Earth Elemental
[ ] Regeneration
[x] Behemoth
[ ] Entangle
[x] Natures Awareness
[ ] Call Lightning
[x] Colossus
[ ] Earth Gate
[x] Herb Mastery
[x] Great Wyrm
[x] Natures Wrath
[x] Resist Magic
[x] Dispel Magic True
[x] Floating Island
[ ] Guardian Wind
[x] Phantom Warriors
[ ] Confusion
[x] Word of Recall
[ ] Counter Magic
[x] Nagas
[x] Psionic Blast
[ ] Blur
[x] Disenchant True
[ ] Vertigo
[x] Spell Lock
[ ] Enchant Road
[x] Flight
[x] Wind Mastery
[x] Spell Blast
[x] Aura of Majesty
[x] Phantom Beast
[x] Disjunction True
[x] Invisibility
[x] Wind Walking
[ ] Banish
[x] Storm Giant
[x] Air Elemental
[ ] Mind Storm
[ ] Stasis
[x] Magic Immunity
[x] Haste
[x] Djinn
[x] Spell Ward
[x] Creature Binding
[ ] Mass Invisibility
[x] Great Unsummoning
[ ] Spell Binding
[x] Flying Fortress
[x] Sky Drake
[x] Suppress Magic
[x] Time Stop
[x] Warp Wood
[x] Disrupt
[x] Fire Bolt
[x] Hell Hounds
[ ] Corruption
[x] Eldritch Weapon
[x] Wall of Fire
[ ] Shatter
[ ] Warp Creature
[x] Fire Elemental
[x] Lightning Bolt
[x] Fire Giant
[x] Chaos Channels
[x] Flame Blade
[X] Gargoyles
[x] Fireball
[X] Doombat
[ ] Raise Volcano
[x] Immolation
[X] Chimeras
[x] Warp Lightning
[ ] Metal Fires
[x] Chaos Spawn
[x] Doom Bolt
[ ] Magic Vortex
[X] Efreet
[x] Fire Storm
[ ] Warp Reality
[ ] Flame Strike
[ ] Chaos Rift
[X] Hydra
[ ] Disintegrate
[x] Meteor Storms
[x] Great Wasting
[ ] Call Chaos
[x] Chaos Surge
[x] Doom Mastery
[x] Great Drake
[ ] Call The Void
[x] Armageddon
[ ] Bless
[x] Star Fires
[x] Endurance
[x] Holy Weapon
[x] Healing
[x] Holy Armor
[x] Just Cause
[ ] True Light
[x] Guardian Spirit
[x] Heroism
[x] True Sight
[ ] Plane Shift
[ ] Resurrection
[ ] Dispel Evil
[x] Planar Seal
[x] Unicorns
[x] Raise Dead
[x] Planar Travel
[ ] Heavenly Light
[ ] Prayer
[ ] Lionheart
[ ] Incarnation
[ ] Invulnerability
[ ] Righteousness
[x] Prosperity
[x] Altar of Battle
[x] Angel
[x] Stream of Life
[ ] Mass Healing
[ ] Holy Word
[ ] High Prayer
[x] Inspirations
[x] Astral Gate
[x] Holy Arms
[x] Consecration
[x] Life Force
[x] Tranquility
[x] Crusade
[x] Arch Angel
[x] Charm of Life
[x] Skeletons
[ ] Weakness
[x] Dark Rituals
[x] Cloak of Fear
[ ] Black Sleep
[x] Ghouls
[ ] Life Drain
[ ] Terror
[ ] Darkness
[ ] Mana Leak
[x] Drain Power
[ ] Possession
[ ] Lycantrophy
[ ] Black Prayer
[x] Black Channels
[x] Night Stalker
[x] Subversion
[x] Wall of Darkness
[ ] Berserk
[x] Shadow Demons
[x] Wraith Form
[ ] Wrack
[x] Evil Presence
[x] Wraiths
[ ] Cloud of Shadow
[ ] Warp Node
[ ] Black Wind
[x] Zombie Mastery
[x] Famine
[x] Cursed Lands
[x] Cruel Unminding
[ ] Word of Death
[x] Death Knights
[ ] Death Spell
[x] Animate Dead
[x] Pestilence
[x] Eternal Night
[x] Evil Omens
[x] Death Wish
[x] Demon Lord
[x] Magic Spirit
[x] Dispel Magic
[x] Summoning Circle
[x] Disenchant Area
[x] Recall Hero
[x] Detect Magic
[ ] Enchant Item
[ ] Summon Hero
[x] Awareness
[x] Disjunction
[ ] Create Artifact
[ ] Summon Champion
[x] Spell of Mastery
[ ] Spell of Returning
