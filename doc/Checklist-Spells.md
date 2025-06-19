
git commit -m "add Overland Specials 'Earthquake'"

git commit -m "add Overland Specials 'Spell Ward'"

git commit -m "add Overland Specials 'Chaos Channels'"
git commit -m "add Overland Specials 'Lycanthropy'"
git commit -m "add Overland Specials 'Natures Cures'"
git commit -m "add Overland Specials 'Plane Shift'"
git commit -m "add Overland Specials 'Word Of Recall'"

git commit -m "add Overland Specials 'Black Wind'"
git commit -m "add Overland Specials 'Stasis'"

git commit -m "add Overland Specials 'Healing'"

git commit -m "add Overland Specials 'Change Terrain'"
git commit -m "add Overland Specials 'Corruption'"
git commit -m "add Overland Specials 'Earth Lore'"
git commit -m "add Overland Specials 'Enchant Road'"
git commit -m "add Overland Specials 'Raise Volcano'"
git commit -m "add Overland Specials 'Transmute'"
git commit -m "add Overland Specials 'Warp Node'"

git commit -m "add Overland Specials 'Incarnation'"
git commit -m "add Overland Specials 'Resurrection'"
git commit -m "add Overland Specials 'Summon Champion'"
git commit -m "add Overland Specials 'Summon Hero'"

git commit -m "add Overland Specials 'Spell Of Mastery'"
git commit -m "add Overland Specials 'Spell Of Return'"


FIXME_RIGHTMEOW
CMB_PrepareTurn__WIP()
Combat_Spell_Target_Screen__WIP()
CMB_DrawMap__WIP()
OVL_ConvSpellAttack()
SoM_Started()



## 'Create Undead'
What is 'Create Undead'?
Where is 'Create Undead'?

"...(the same rules as those for the Create Undead special ability apply)."



scc_Destruction_Spell    ==>  scc_Combat_Destroy_Unit
scc_Fixed_Dmg_Spell      ==>  scc_Direct_Damage_Fixed
scc_Var_Dmg_Spell        ==>  scc_Direct_Damage_Variable
scc_Mundane_Enchantment  ==>  scc_Unit_Enchantment_Normal_Only
scc_Dispel_Spell         ==>  scc_Dispels
scc_Disenchant_Spell     ==>  scc_Disenchants
scc_Disjunction_Spell    ==>  scc_Disjunctions
scc_Target_Wiz_Spell     ==>  scc_Target_Wizard
scc_Counter_Spell        ==>  scc_Combat_Counter_Magic
scc_Banish_Spell         ==>  scc_Combat_Banish
scc_Special_Spell        ==>  scc_Specials

NOTE: the check on the check-list is only for the human player side, CP/AI is a different beast

NOTE: 'Blur' is not in the manual, but is in the OSG - ¿ added in v1.2 ?



no target / hard-coded to 99
Combat_Cast_Spell__WIP()
    (spell_data_table[spell_idx].type == scc_Battlefield_Spell)
    (spell_data_table[spell_idx].type == scc_Combat_Counter_Magic)
    (spell_data_table[spell_idx].type == scc_Disenchants)
    (spell_data_table[spell_idx].type == scc_City_Enchantment_Positive)
    (spell_idx == spl_Animate_Dead)
Target = 99;  Target_X = 0;  Target_Y = 0;



Checklist-Spells.ods
Green   Done-Done
Light-Lime-3    maybe, kinda?
...orangish     coded, but known issues
...purpleish    IDK/IDGI/weirdness

..."Combat Spells"..."Overland Spells"...



Weird Ones?
    Astral Gate
    Chaos Channels
    Change Terrain?
    Create Artifact!    handled special during casting
    Earth Gate
    Earthquake          lots of extra special code
    Enchant Item!       handled special during casting
    Enchant Road
    Flying Fortress?
    Move Fortress?
    Raise Volcano?
    Resurrection?
    Spell Ward          seem to recall the code tfor the effects being messy
    Summon Champion?
    Summon Hero?
    Summoning Circle?
    Warp Node



all overland summons are done
all overland/global enchantments are done
all city enchantments are done
all combat spell casting categories are done
all combat summons are done
...need to fix-up on-going/per-turn effects
...need to review battlefield effects/combat enchantments
...need to review battle unit effects/enchantments ...outlined, glassed/remapped?
...need to do color cycling
...need to do dissolve page-flip

    SCC  Description
[x]  -1  scc_None
[x]   0  scc_Summoning
[x]   1  scc_Unit_Enchantment
[x]   2  scc_City_Enchantment_Positive       ¿ ..., Stream of Life, ... ?
[x]   3  scc_City_Enchantment_Negative      ¿ Evil Presence, Famine, 
[x]   4  scc_Direct_Damage_Fixed
[-]   5  scc_Special_Spell
[x]   6  scc_Target_Wizard
[x]   7  N/A
[x]   8  N/A
[x]   9  scc_Global_Enchantment
[x]  10  scc_Battlefield_Spell
[ ]  11  scc_Crafting_Spell
[x]  12  scc_Combat_Destroy_Unit
[x]  13  scc_Resistable_Spell
[x]  14  scc_Unresistable_Spell
[x]  15  scc_Unit_Enchantment_Normal_Only
[x]  16  scc_Mundane_Curse
[x]  17  N/A
[x]  18  scc_Dispels
[x]  19  scc_Disenchants
[x]  20  scc_Disjunctions
[x]  21  scc_Combat_Counter_Magic  (w/ scc_Battlefield_Spell)
[x]  22  scc_Direct_Damage_Variable
[x]  23  scc_Combat_Banish  (w/ scc_Combat_Destroy_Unit)

scc_Special_Spell
    Overland
        Black Wind, Call The Void, Change Terrain, Chaos Channels, Corruption, Earth Lore, Earthquake, Enchant Road, Great Unsummoning, Incarnation, Lycanthropy, Move Fortress, Natures Cures, Plane Shift, Raise Volcano, Resurrection, Spell Binding, Spell Of Mastery, Spell Of Return, Spell Ward, Stasis, Summon Champion, Summon Hero, Transmute, Warp Node

scc_Crafting_Spell
    Enchant Item, Create Artifact



Spreadsheet - OSG Spells by Category
[x] Creature Summoning Spells
[x] City Enchantments
[x] Creature Enchantments
[x] Global Enchantments
[x] Combat Spells
[ ] Special Spells

Spreadsheet - OSG Spells by Realm
[ ] Arcane
[ ] Chaos
[ ] Death
[ ] Life
[ ] Nature
[ ] Sorcery



[x] Earth to Mud
[x] Resist Elements
[x] Wall of Stone
[x] Giant Strength
[x] Web
[x] War Bears
[x] Stone Skin
[x] Water Walking
[x] Sprites
[ ] Earth Lore
[x] Cracks Call
[ ] Natures eye
[x] Ice Bolt
[x] Giant Spiders
[ ] Change Terrain
[x] Path Finding
[x] Cockatrices
[ ] Transmute
[ ] Natures Cures
[x] Basilisk
[x] Elemental Armor
[x] Petrify
[x] Stone Giant
[x] Iron Skin
[x] Ice Storm
[x] Earthquake
[x] Gorgons
[ ] Move Fortress
[x] Gaias Blessing
[x] Earth Elemental
[-] Regeneration
[x] Behemoth
[x] Entangle
[x] Natures Awareness
[x] Call Lightning
[x] Colossus
[ ] Earth Gate
[x] Herb Mastery
[x] Great Wyrm
[x] Natures Wrath
[x] Resist Magic
[x] Dispel Magic True
[x] Floating Island
[x] Guardian Wind
[x] Phantom Warriors
[x] Confusion
[x] Word of Recall
[x] Counter Magic
[x] Nagas
[x] Psionic Blast
[x] Blur
[x] Disenchant True
[x] Vertigo
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
[x] Banish
[x] Storm Giant
[x] Air Elemental
[x] Mind Storm
[ ] Stasis
[x] Magic Immunity
[x] Haste
[x] Djinn
[x] Spell Ward
[x] Creature Binding
[x] Mass Invisibility
[x] Great Unsummoning
[x] Spell Binding
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
[x] Shatter
[x] Warp Creature
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
[x] Metal Fires
[x] Chaos Spawn
[x] Doom Bolt
[x] Magic Vortex
[X] Efreet
[x] Fire Storm
[x] Warp Reality
[x] Flame Strike
[x] Chaos Rift
[X] Hydra
[x] Disintegrate
[x] Meteor Storms
[x] Great Wasting
[x] Call Chaos
[x] Chaos Surge
[x] Doom Mastery
[x] Great Drake
[x] Call The Void
[x] Armageddon
[x] Bless
[x] Star Fires
[x] Endurance
[x] Holy Weapon
[x] Healing
[x] Holy Armor
[x] Just Cause
[x] True Light
[x] Guardian Spirit
[x] Heroism
[x] True Sight
[ ] Plane Shift
[ ] Resurrection
[x] Dispel Evil
[x] Planar Seal
[x] Unicorns
[x] Raise Dead
[x] Planar Travel
[x] Heavenly Light
[x] Prayer
[x] Lionheart
[ ] Incarnation
[x] Invulnerability
[x] Righteousness
[x] Prosperity
[x] Altar of Battle
[x] Angel
[x] Stream of Life
[x] Mass Healing
[x] Holy Word
[x] High Prayer
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
[x] Weakness
[x] Dark Rituals
[x] Cloak of Fear
[x] Black Sleep
[x] Ghouls
[ ] Life Drain
[x] Terror
[x] Darkness
[x] Mana Leak
[x] Drain Power
[x] Possession
[ ] Lycantrophy
[x] Black Prayer
[x] Black Channels
[x] Night Stalker
[x] Subversion
[x] Wall of Darkness
[x] Berserk
[x] Shadow Demons
[x] Wraith Form
[x] Wrack
[x] Evil Presence
[x] Wraiths
[x] Cloud of Shadow
[ ] Warp Node
[ ] Black Wind
[x] Zombie Mastery
[x] Famine
[x] Cursed Lands
[x] Cruel Unminding
[x] Word of Death
[x] Death Knights
[x] Death Spell
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
[-] Spell of Mastery
[ ] Spell of Returning
