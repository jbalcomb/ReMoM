
Do I still need this?
...spent a ridiculous amount of time organizing it...
...but, should just all be in the spells spreadsheet? - source of truth?


NOTE: no such thing as "spell of returning"
NOTE: Guises was deprecated



// enum enum_SPELL_DATA_TYPE
// sdt_Summoning         =  0,
// Unit_Enchantment        =  1,
// City_Enchantment        =  2,
// City_Curse              =  3,
// Fixed_Dmg_Spell         =  4,
// Special_Spell           =  5,
// Target_Wiz_Spell        =  6,
// 7 DNE
// 8 DNE
// sdt_Global_Enchantment  =  9,
// Battlefield_Spell       = 10,
// sdt_Crafting_Spell      = 11,
// Destruction_Spell       = 12,
// Resistable_Spell        = 13,
// No_Resist_Spell         = 14,
// Mundane_Enchantment     = 15,
// Mundane_Curse           = 16,
// sdt_Infusable_Spell     = 17,
// Dispel_Spell            = 18,
// Disenchant_Spell        = 19,
// Disjunction_Spell       = 20,
// Counter_Spell           = 21,
// Var_Dmg_Spell           = 22,
// Banish_Spell            = 23



UNDEFINED, spl_UNDEFINED,  -1,  /* Eh? */
NONE, spl_NONE,   0,
Earth_To_Mud, spl_Earth_To_Mud,   1,  /* sbr_Nature */
Resist_Elements, spl_Resist_Elements,   2,
Wall_Of_Stone, spl_Wall_Of_Stone,   3,
Giant_Strength, spl_Giant_Strength,   4,
Web, spl_Web,   5,
War_Bears, spl_War_Bears,   6,
Stone_Skin, spl_Stone_Skin,   7,
Water_Walking, spl_Water_Walking,   8,
Sprites, spl_Sprites,   9,
Earth_Lore, spl_Earth_Lore,  10,
Cracks_Call, spl_Cracks_Call,  11,
Natures_Eye, spl_Natures_Eye,  12,
Ice_Bolt, spl_Ice_Bolt,  13,
Giant_Spiders, spl_Giant_Spiders,  14,
Change_Terrain, spl_Change_Terrain,  15,
Path_Finding, spl_Path_Finding,  16,
Cockatrices, spl_Cockatrices,  17,
Transmute, spl_Transmute,  18,
Natures_Cures, spl_Natures_Cures,  19,
Basilisk, spl_Basilisk,  20,
Elemental_Armor, spl_Elemental_Armor,  21,
Petrify, spl_Petrify,  22,
Stone_Giant, spl_Stone_Giant,  23,
Iron_Skin, spl_Iron_Skin,  24,
Ice_Storm, spl_Ice_Storm,  25,
Earthquake, spl_Earthquake,  26,
Gorgons, spl_Gorgons,  27,
Move_Fortress, spl_Move_Fortress,  28,
Gaias_Blessing, spl_Gaias_Blessing,  29,
Earth_Elemental, spl_Earth_Elemental,  30,
Regeneration, spl_Regeneration,  31,
Behemoth, spl_Behemoth,  32,
Entangle, spl_Entangle,  33,
Natures_Awareness, spl_Natures_Awareness,  34,
Call_Lightning, spl_Call_Lightning,  35,
Colossus, spl_Colossus,  36,
Earth_Gate, spl_Earth_Gate,  37,
Herb_Mastery, spl_Herb_Mastery,  38,
Great_Wyrm, spl_Great_Wyrm,  39,
Natures_Wrath, spl_Natures_Wrath,  40,
Resist_Magic, spl_Resist_Magic,  41,  /* sbr_Sorcery */
Dispel_Magic_True, spl_Dispel_Magic_True,  42,
Floating_Island, spl_Floating_Island,  43,
Guardian_Wind, spl_Guardian_Wind,  44,
Phantom_Warriors, spl_Phantom_Warriors,  45,
Confusion, spl_Confusion,  46,
Word_Of_Recall, spl_Word_Of_Recall,  47,
Counter_Magic, spl_Counter_Magic,  48,
Nagas, spl_Nagas,  49,
Psionic_Blast, spl_Psionic_Blast,  50,
Blur, spl_Blur,  51,
Disenchant_True, spl_Disenchant_True,  52,
Vertigo, spl_Vertigo,  53,
Spell_Lock, spl_Spell_Lock,  54,
Enchant_Road, spl_Enchant_Road,  55,
Flight, spl_Flight,  56,
Wind_Mastery, spl_Wind_Mastery,  57,
Spell_Blast, spl_Spell_Blast,  58,
Aura_Of_Majesty, spl_Aura_Of_Majesty,  59,
Phantom_Beast, spl_Phantom_Beast,  60,
Disjunction_True, spl_Disjunction_True,  61,
Invisibility, spl_Invisibility,  62,
Wind_Walking, spl_Wind_Walking,  63,
Banish, spl_Banish,  64,
Storm_Giant, spl_Storm_Giant,  65,
Air_Elemental, spl_Air_Elemental,  66,
Mind_Storm, spl_Mind_Storm,  67,
Stasis, spl_Stasis,  68,
Magic_Immunity, spl_Magic_Immunity,  69,
Haste, spl_Haste,  70,
Djinn, spl_Djinn,  71,
Spell_Ward, spl_Spell_Ward,  72,
Creature_Binding, spl_Creature_Binding,  73,
Mass_Invisibility, spl_Mass_Invisibility,  74,
Great_Unsummoning, spl_Great_Unsummoning,  75,
Spell_Binding, spl_Spell_Binding,  76,
Flying_Fortress, spl_Flying_Fortress,  77,
Sky_Drake, spl_Sky_Drake,  78,
Suppress_Magic, spl_Suppress_Magic,  79,
Time_Stop, spl_Time_Stop,  80,
Warp_Wood, spl_Warp_Wood,  81,  /* sbr_Chaos */
Disrupt, spl_Disrupt,  82,
Fire_Bolt, spl_Fire_Bolt,  83,
Hell_Hounds, spl_Hell_Hounds,  84,
Corruption, spl_Corruption,  85,
Eldritch_Weapon, spl_Eldritch_Weapon,  86,
Wall_Of_Fire, spl_Wall_Of_Fire,  87,
Shatter, spl_Shatter,  88,
Warp_Creature, spl_Warp_Creature,  89,
Fire_Elemental, spl_Fire_Elemental,  90,
Lightning_Bolt, spl_Lightning_Bolt,  91,
Fire_Giant, spl_Fire_Giant,  92,
Chaos_Channels, spl_Chaos_Channels,  93,
Flame_Blade, spl_Flame_Blade,  94,
Gargoyles, spl_Gargoyles,  95,
Fireball, spl_Fireball,  96,
Doombat, spl_Doombat,  97,
Raise_Volcano, spl_Raise_Volcano,  98,
Immolation, spl_Immolation,  99,
Chimeras, spl_Chimeras, 100,
Warp_Lightning, spl_Warp_Lightning, 101,
Metal_Fires, spl_Metal_Fires, 102,
Chaos_Spawn, spl_Chaos_Spawn, 103,
Doom_Bolt, spl_Doom_Bolt, 104,
Magic_Vortex, spl_Magic_Vortex, 105,
Efreet, spl_Efreet, 106,
Fire_Storm, spl_Fire_Storm, 107,
Warp_Reality, spl_Warp_Reality, 108,
Flame_Strike, spl_Flame_Strike, 109,
Chaos_Rift, spl_Chaos_Rift, 110,
Hydra, spl_Hydra, 111,
Disintegrate, spl_Disintegrate, 112,
MeteorStorms, spl_MeteorStorms, 113,
Great_Wasting, spl_Great_Wasting, 114,
Call_Chaos, spl_Call_Chaos, 115,
Chaos_Surge, spl_Chaos_Surge, 116,
Doom_Mastery, spl_Doom_Mastery, 117,
Great_Drake, spl_Great_Drake, 118,
Call_The_Void, spl_Call_The_Void, 119,
Armageddon, spl_Armageddon, 120,  /* sbr_Life */
Bless, spl_Bless, 121,
Star_Fires, spl_Star_Fires, 122,
Endurance, spl_Endurance, 123,
Holy_Weapon, spl_Holy_Weapon, 124,
Healing, spl_Healing, 125,
Holy_Armor, spl_Holy_Armor, 126,
Just_Cause, spl_Just_Cause, 127,
True_Light, spl_True_Light, 128,
Guardian_Spirit, spl_Guardian_Spirit, 129,
Heroism, spl_Heroism, 130,
True_Sight, spl_True_Sight, 131,
Plane_Shift, spl_Plane_Shift, 132,
Resurrection, spl_Resurrection, 133,
Dispel_Evil, spl_Dispel_Evil, 134,
Planar_Seal, spl_Planar_Seal, 135,
Unicorns, spl_Unicorns, 136,
Raise_Dead, spl_Raise_Dead, 137,
Planar_Travel, spl_Planar_Travel, 138,
Heavenly_Light, spl_Heavenly_Light, 139,
Prayer, spl_Prayer, 140,
Lionheart, spl_Lionheart, 141,
Incarnation, spl_Incarnation, 142,
Invulnerability, spl_Invulnerability, 143,
Righteousness, spl_Righteousness, 144,
Prosperity, spl_Prosperity, 145,
Altar_Of_Battle, spl_Altar_Of_Battle, 146,
Angel, spl_Angel, 147,
Stream_Of_Life, spl_Stream_Of_Life, 148,
Mass_Healing, spl_Mass_Healing, 149,
Holy_Word, spl_Holy_Word, 150,
High_Prayer, spl_High_Prayer, 151,
Inspirations, spl_Inspirations, 152,
Astral_Gate, spl_Astral_Gate, 153,
Holy_Arms, spl_Holy_Arms, 154,
Consecration, spl_Consecration, 155,
Life_Force, spl_Life_Force, 156,
Tranquility, spl_Tranquility, 157,
Crusade, spl_Crusade, 158,
Arch_Angel, spl_Arch_Angel, 159,
Charm_Of_Life, spl_Charm_Of_Life, 160,
Skeletons, spl_Skeletons, 161,  /* sbr_Death */
Weakness, spl_Weakness, 162,
Dark_Rituals, spl_Dark_Rituals, 163,
Cloak_Of_Fear, spl_Cloak_Of_Fear, 164,
Black_Sleep, spl_Black_Sleep, 165,
Ghouls, spl_Ghouls, 166,
Life_Drain, spl_Life_Drain, 167,
Terror, spl_Terror, 168,
Darkness, spl_Darkness, 169,
Mana_Leak, spl_Mana_Leak, 170,
Drain_Power, spl_Drain_Power, 171,
Possession, spl_Possession, 172,
Lycantrophy, spl_Lycantrophy, 173,
Black_Prayer, spl_Black_Prayer, 174,
Black_Channels, spl_Black_Channels, 175,
Night_Stalker, spl_Night_Stalker, 176,
Subversion, spl_Subversion, 177,
Wall_Of_Darkness, spl_Wall_Of_Darkness, 178,
Berserk, spl_Berserk, 179,
Shadow_Demons, spl_Shadow_Demons, 180,
Wraith_Form, spl_Wraith_Form, 181,
Wrack, spl_Wrack, 182,
Evil_Presence, spl_Evil_Presence, 183,
Wraiths, spl_Wraiths, 184,
Cloud_Of_Shadow, spl_Cloud_Of_Shadow, 185,
Warp_Node, spl_Warp_Node, 186,
Black_Wind, spl_Black_Wind, 187,
Zombie_Mastery, spl_Zombie_Mastery, 188,
Famine, spl_Famine, 189,
Cursed_Lands, spl_Cursed_Lands, 190,
Cruel_Unminding, spl_Cruel_Unminding, 191,
Word_Of_Death, spl_Word_Of_Death, 192,
Death_Knights, spl_Death_Knights, 193,
Death_Spell, spl_Death_Spell, 194,
Animate_Dead, spl_Animate_Dead, 195,
Pestilence, spl_Pestilence, 196,
Eternal_Night, spl_Eternal_Night, 197,
Evil_Omens, spl_Evil_Omens, 198,
Death_Wish, spl_Death_Wish, 199,
Demon_Lord, spl_Demon_Lord, 200,
Magic_Spirit, spl_Magic_Spirit, 201,  /* sbr_Arcane */
Dispel_Magic, spl_Dispel_Magic, 202,
Summoning_Circle, spl_Summoning_Circle, 203,
Disenchant_Area, spl_Disenchant_Area, 204,
Recall_Hero, spl_Recall_Hero, 205,
Detect_Magic, spl_Detect_Magic, 206,
Enchant_Item, spl_Enchant_Item, 207,
Summon_Hero, spl_Summon_Hero, 208,
Awareness, spl_Awareness, 209,
Disjunction, spl_Disjunction, 210,
Create_Artifact, spl_Create_Artifact, 211,
Summon_Champion, spl_Summon_Champion, 212,
Spell_Of_Mastery, spl_Spell_Of_Mastery, 213,
Spell_Of_Return, spl_Spell_Of_Return, 214, 















"None", -1

"War Bears", 0
"Sprites", 0
"Giant Spiders", 0
"Cockatrices", 0
"Basilisk", 0
"Stone Giant", 0
"Gorgons", 0
"Earth Elemental", 0
"Behemoth", 0
"Colossus", 0
"Phantom Beast", 0
"Storm Giant", 0
"Air Elemental", 0
"Djinn", 0
"Sky Drake", 0
"Hell Hounds", 0
"Fire Elemental", 0
"Fire Giant", 0
"Gargoyles", 0
"Doom Bat", 0
"Great Wyrm", 0
"Floating Island", 0
"Phantom Warriors", 0
"Nagas", 0
"Chimeras", 0
"Chaos Spawn", 0
"Efreet", 0
"Hydra", 0
"Great Drake", 0
"Guardian Spirit", 0
"Unicorns", 0
"Angel", 0
"Arch Angel", 0
"Skeletons", 0
"Ghouls", 0
"Night Stalker", 0
"Shadow Demons", 0
"Wraiths", 0
"Death Knights", 0
"Demon Lord", 0
"Magic Spirit", 0


"Resist Elements", 1
"Giant Strength", 1
"Stone Skin", 1
"Water Walking", 1
"Path Finding", 1
"Elemental Armor", 1
"Iron Skin", 1
"Regeneration", 1
"Resist Magic", 1
"Guardian Wind", 1
"Spell Lock", 1
"Flight", 1
"Invisiblity", 1
"Wind Walking", 1
"Magic Immunity", 1
"Haste", 1
"Immolation", 1
"Bless", 1
"Endurance", 1
"True Sight", 1
"Planar Travel", 1
"Lionheart", 1
"Invulnerability", 1
"Righteousness", 1
"Cloak of Fear", 1
"Black Channels", 1
"Berserk", 1
"Wraith Form", 1


"Nature's Eye", 2
"Gaia's Blessing", 2
"Earth Gate", 2
"Flying Fortress", 2
"Wall of Fire", 2
"Heavenly Light", 2
"Prosperity", 2
"Altar of Battle", 2
"Stream of Life", 2
"Inspirations", 2
"Astral Gate", 2
"Consecration", 2
"Dark Rituals", 2
"Wall of Darkness", 2
"Cloud of Shadow", 2
"Summoning Circle", 2


"Chaos Rift", 3
"Evil Presence", 3
"Famine", 3
"Cursed Lands", 3
"Pestilence", 3


"Ice Storm", 4
"Warp Lightning", 4
"Doom Bolt", 4
"Fire Storm", 4
"Star Fires", 4


"Earth to Mud", 5
"Wall of Stone", 5
"Earth Lore", 5
"Cracks Call", 5
"Change Terrain", 5
"Transmute", 5
"Nature's Cures", 5
"Earthquake", 5
"Move Fortress", 5
"Word of Recall", 5
"Enchant Road", 5
"Stasis", 5
"Spell Ward", 5
"Great Unsummoning", 5
"Spell Binding", 5
"Warp Wood", 5
"Disrupt", 5
"Corruption", 5
"Warp Creature", 5
"Chaos Channels", 5
"Raise Volcano", 5
"Magic Vortex", 5
"Call the Void", 5
"Healing", 5
"Plane Shift", 5
"Resurrection", 5
"Raise Dead", 5
"Incarnation", 5
"Lycanthropy", 5
"Warp Node", 5
"Black Wind", 5
"Animate Dead", 5
"Death Wish", 5
"Recall Hero", 5
"Summon Hero", 5
"Summon Champion", 5
"Spell of Mastery", 5
"Spell of Return", 5


"Spell Blast", 6
"Drain Power", 6
"Subversion", 6
"Cruel Unminding", 6

7  DNE

8  DNE


"Nature Awareness", 9
"Herb Mastery", 9
"Nature's Wrath", 9
"Wind Mastery", 9
"Aura of Majesty", 9
"Suppress Magic", 9
"Time Stop", 9
"Meteor Storm", 9
"Great Wasting", 9
"Chaos Surge", 9
"Doom Mastery", 9
"Armageddon", 9
"Just Cause", 9
"Planar Seal", 9
"Holy Arms", 9
"Life Force", 9
"Tranquility", 9
"Crusade", 9
"Charm of Life", 9
"Zombie Mastery", 9
"Eternal Night", 9
"Evil Omens", 9
"Detect Magic", 9
"Awareness", 9


"Entangle", 10
"Call Lightning", 10
"Blur", 10
"Mass Invisibility", 10
"Metal Fires", 10
"Warp Reality", 10
"Flame Strike", 10
"Call Chaos", 10
"True Light", 10
"Prayer", 10
"Mass Healing", 10
"Holy Word", 10
"High Prayer", 10
"Terror", 10
"Darkness", 10
"Mana Leak", 10
"Black Prayer", 10
"Wrack", 10
"Death Spell", 10


"Enchant Item", 11
"Create Artifact", 11


"Petrify", 12
"Disintegrate", 12
"Dispel Evil", 12
"Word of Death", 12


"Confusion", 13
"Vertigo", 13
"Creature Binding", 13
"Weakness", 13
"Black Sleep", 13


"Web", 14
"Mind Storm", 14


"Eldritch Weapon", 15
"Flame Blade", 15
"Holy Weapon", 15
"Holy Armor", 15
"Heroism", 15


"Shatter", 16
"Possession", 16


17 DNE


"Dispel Magic True", 18
"Dispel Magic", 18


"Disenchant True", 19
"Disenchant Area", 19


"Disjunction True", 20
"Disjunction", 20


"Counter Magic", 21


"Ice Bolt", 22
"Psionic Blast", 22
"Fire Bolt", 22
"Lightning Bolt", 22
"Fireball", 22
"Life Drain", 22


"Banish", 23





BEGIN
sizeof(struct s_SPELL_DATA): 36
ret_code: 7740
"None", -1
"Earth to Mud", 5
"Resist Elements", 1
"Wall of Stone", 5
"Giant Strength", 1
"Web", 14
"War Bears", 0
"Stone Skin", 1
"Water Walking", 1
"Sprites", 0
"Earth Lore", 5
"Cracks Call", 5
"Nature's Eye", 2
"Ice Bolt", 22
"Giant Spiders", 0
"Change Terrain", 5
"Path Finding", 1
"Cockatrices", 0
"Transmute", 5
"Nature's Cures", 5
"Basilisk", 0
"Elemental Armor", 1
"Petrify", 12
"Stone Giant", 0
"Iron Skin", 1
"Ice Storm", 4
"Earthquake", 5
"Gorgons", 0
"Move Fortress", 5
"Gaia's Blessing", 2
"Earth Elemental", 0
"Regeneration", 1
"Behemoth", 0
"Entangle", 10
"Nature Awareness", 9
"Call Lightning", 10
"Colossus", 0
"Earth Gate", 2
"Herb Mastery", 9
"Great Wyrm", 0
"Nature's Wrath", 9
"Resist Magic", 1
"Dispel Magic True", 18
"Floating Island", 0
"Guardian Wind", 1
"Phantom Warriors", 0
"Confusion", 13
"Word of Recall", 5
"Counter Magic", 21
"Nagas", 0
"Psionic Blast", 22
"Blur", 10
"Disenchant True", 19
"Vertigo", 13
"Spell Lock", 1
"Enchant Road", 5
"Flight", 1
"Wind Mastery", 9
"Spell Blast", 6
"Aura of Majesty", 9
"Phantom Beast", 0
"Disjunction True", 20
"Invisiblity", 1
"Wind Walking", 1
"Banish", 23
"Storm Giant", 0
"Air Elemental", 0
"Mind Storm", 14
"Stasis", 5
"Magic Immunity", 1
"Haste", 1
"Djinn", 0
"Spell Ward", 5
"Creature Binding", 13
"Mass Invisibility", 10
"Great Unsummoning", 5
"Spell Binding", 5
"Flying Fortress", 2
"Sky Drake", 0
"Suppress Magic", 9
"Time Stop", 9
"Warp Wood", 5
"Disrupt", 5
"Fire Bolt", 22
"Hell Hounds", 0
"Corruption", 5
"Eldritch Weapon", 15
"Wall of Fire", 2
"Shatter", 16
"Warp Creature", 5
"Fire Elemental", 0
"Lightning Bolt", 22
"Fire Giant", 0
"Chaos Channels", 5
"Flame Blade", 15
"Gargoyles", 0
"Fireball", 22
"Doom Bat", 0
"Raise Volcano", 5
"Immolation", 1
"Chimeras", 0
"Warp Lightning", 4
"Metal Fires", 10
"Chaos Spawn", 0
"Doom Bolt", 4
"Magic Vortex", 5
"Efreet", 0
"Fire Storm", 4
"Warp Reality", 10
"Flame Strike", 10
"Chaos Rift", 3
"Hydra", 0
"Disintegrate", 12
"Meteor Storm", 9
"Great Wasting", 9
"Call Chaos", 10
"Chaos Surge", 9
"Doom Mastery", 9
"Great Drake", 0
"Call the Void", 5
"Armageddon", 9
"Bless", 1
"Star Fires", 4
"Endurance", 1
"Holy Weapon", 15
"Healing", 5
"Holy Armor", 15
"Just Cause", 9
"True Light", 10
"Guardian Spirit", 0
"Heroism", 15
"True Sight", 1
"Plane Shift", 5
"Resurrection", 5
"Dispel Evil", 12
"Planar Seal", 9
"Unicorns", 0
"Raise Dead", 5
"Planar Travel", 1
"Heavenly Light", 2
"Prayer", 10
"Lionheart", 1
"Incarnation", 5
"Invulnerability", 1
"Righteousness", 1
"Prosperity", 2
"Altar of Battle", 2
"Angel", 0
"Stream of Life", 2
"Mass Healing", 10
"Holy Word", 10
"High Prayer", 10
"Inspirations", 2
"Astral Gate", 2
"Holy Arms", 9
"Consecration", 2
"Life Force", 9
"Tranquility", 9
"Crusade", 9
"Arch Angel", 0
"Charm of Life", 9
"Skeletons", 0
"Weakness", 13
"Dark Rituals", 2
"Cloak of Fear", 1
"Black Sleep", 13
"Ghouls", 0
"Life Drain", 22
"Terror", 10
"Darkness", 10
"Mana Leak", 10
"Drain Power", 6
"Possession", 16
"Lycanthropy", 5
"Black Prayer", 10
"Black Channels", 1
"Night Stalker", 0
"Subversion", 6
"Wall of Darkness", 2
"Berserk", 1
"Shadow Demons", 0
"Wraith Form", 1
"Wrack", 10
"Evil Presence", 3
"Wraiths", 0
"Cloud of Shadow", 2
"Warp Node", 5
"Black Wind", 5
"Zombie Mastery", 9
"Famine", 3
"Cursed Lands", 3
"Cruel Unminding", 6
"Word of Death", 12
"Death Knights", 0
"Death Spell", 10
"Animate Dead", 5
"Pestilence", 3
"Eternal Night", 9
"Evil Omens", 9
"Death Wish", 5
"Demon Lord", 0
"Magic Spirit", 0
"Dispel Magic", 18
"Summoning Circle", 2
"Disenchant Area", 19
"Recall Hero", 5
"Detect Magic", 9
"Enchant Item", 11
"Summon Hero", 5
"Awareness", 9
"Disjunction", 20
"Create Artifact", 11
"Summon Champion", 5
"Spell of Mastery", 5
"Spell of Return", 5
END
