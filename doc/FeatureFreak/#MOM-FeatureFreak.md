# Master of Magic (1994) — FeatureFreak

> A comprehensive feature catalog for *Master of Magic* (SimTex / MicroProse, 1994; documented against the v1.31 release), framed as a **4X strategy game**.

This is the **overview and initial breakdown**. It frames the whole game under the four X's — **eXplore, eXpand, eXploit, eXterminate** — and then adds the pillars that make Master of Magic *Master of Magic* rather than a generic 4X: its **magic system**, its **wizards**, its **heroes and items**, its **two planes**, and its **tactical combat**.

Each feature area below gets a summary paragraph. Areas marked **→ (future child doc)** are candidates to be split out into their own file under [FeatureFreak/](.) as the catalog grows. Where an existing `doc/` file already covers the mechanics in depth, it is cross-referenced.

**Companion docs:**
- [MOM-FeatureFreak-AI-Usage.md](MOM-FeatureFreak-AI-Usage.md) maps every pillar below onto *how the computer player uses it*.
- [MOM-FeatureFreak-AI-Special-Abilities.md](MOM-FeatureFreak-AI-Special-Abilities.md) walks all 18 Wizard Special Abilities (retorts) — active AI logic vs. passive shared benefit, with the bugs at each site.
- [MOM-FeatureFreak-AI-Spells.md](MOM-FeatureFreak-AI-Spells.md) walks how the AI selects and casts spells — the overland pipeline & group handlers vs. the stubbed combat path — with the bugs (in progress).

---

## How to read this document

- **Scope:** the *game's features as experienced by a player* — not the code that implements them. Implementation lives in the rest of `doc/` (see [Cross-references](#cross-references-to-existing-docs)).
- **Version:** all counts and behaviors describe the final **v1.31** patch of the original 1994 game.
- **Conventions:** feature areas are numbered so future child docs can be named e.g. `FeatureFreak-03-eXploit.md`.
- **The 4X mapping is a lens, not a cage.** Several MoM systems (magic above all) cut across every X. Those get their own top-level pillars (§5–§10) rather than being force-fit under one X.

---

## Game at a glance

| Aspect | Summary |
| --- | --- |
| **Genre** | 4X turn-based strategy with tactical combat and a fantasy/magic theme |
| **Players** | 1 human vs. up to 4 AI wizards (5 wizards total per game) |
| **You play** | A **wizard** competing to dominate two parallel worlds through magic and conquest |
| **World** | **Two planes** — Arcanus and Myrror — each a wrap-around tile map |
| **Magic** | **5 realms** (Life, Death, Chaos, Nature, Sorcery) + shared Arcane spells; **214 spells** |
| **Customization** | 14 preset wizards or a **custom wizard** built from spellbooks + retorts |
| **Special abilities** | **18 retorts** (Alchemy, Warlord, Myrran, Archmage, Runemaster, …) |
| **Races** | 14 playable town races (10 Arcanian + 4 Myrran), each with unique unit rosters |
| **Heroes** | **35** heroes/champions in the roster (up to **6** employed per wizard) who level up and carry magic items |
| **Combat** | Separate **tactical battle** screen, stacks of up to 9 units, terrain + walls |
| **Victory** | Cast the **Spell of Mastery**, *or* eliminate/banish every rival wizard |

---

## The 4X framing

Master of Magic is a textbook 4X built on an unusual spine: **you are a spellcaster first and an empire-builder second.** Every X is mediated by magic — you explore with detection spells and flying scouts, expand with settlers *and* summoned outposts, exploit mana as a first-class resource alongside gold and food, and exterminate rivals on a tactical battlefield where a single well-cast spell can decide the war. The two-plane world doubles the map and injects an asymmetric "hard mode" continent (Myrror) into every pillar.

---

## 1. eXplore

**The unknown map, and the tools to reveal it.**

- **Fog of war & the two planes.** The world begins hidden. Each plane (Arcanus, Myrror) is separately explored; sight is a per-unit radius, extended by scouting and high ground. **→ (future child doc: exploration & vision)**
- **Movement & terrain.** Units cross terrain at differing costs (roads, grassland, forest, hills, mountains, water, swamp, desert, tundra, volcano). Flying, swimming, sailing, mountaineering, and pathfinding traits change what a unit can reach. Cross-referenced: [MoM-Terrain-Map_Square_Is.md](../MoM-Terrain-Map_Square_Is.md), [MoX-Terrain.md](../MoX-Terrain.md), [MoX-WorldMap.md](../MoX-WorldMap.md).
- **Lairs, nodes, and encounters.** The map is seeded with **Ruins, Dungeons, Ancient Temples, Abandoned Keeps, Monster Lairs, and magic Nodes**, each guarding treasure (gold, mana, spells, items, prisoners, spellbooks/retorts) behind a garrison of monsters. Clearing them is the primary early-game exploration reward. Cross-referenced: [MoM-Lair.md](../MoM-Lair.md), [MoX-NewGame-Lairs.md](../MoX-NewGame-Lairs.md).
- **Towers of Wizardry.** Special lairs that, once cleared, become the **only walkable bridges between Arcanus and Myrror** for non-flying/non-planar-travel units.
- **Map awareness tools.** Surveyor (terrain/special readout), Cartographer (overview map), and magic detection (Awareness, Nature's Eye, spell/enemy scrying) turn exploration into an ongoing information game. Cross-referenced: [MoM-SurveyorScreen.md](../MoM-SurveyorScreen.md), [MoM-CartographerScreen.md](../MoM-CartographerScreen.md).

---

## 2. eXpand

**Founding, growing, and multiplying your footholds.**

- **Cities & outposts.** New settlements begin as **outposts** (a growing hamlet) and mature into full cities as population accrues. Founding is done with Settlers or the *Create Outpost*-style effects. Cross-referenced: [MoM-OutpostScreen.md](../MoM-OutpostScreen.md), [MoX-Settle.md](../MoX-Settle.md), [MoM-Population.md](../MoM-Population.md).
- **Population & the city radius.** Each city works a fixed area of tiles; terrain, minerals, and adjacency determine its food cap and output. Population splits into **farmers, workers, and rebels**, and the race sets max size and growth rate (e.g. Halflings/Barbarians grow large; Dwarves/High Elves grow slowly). Cross-referenced: [MoM-CityArea.md](../MoM-CityArea.md), [MoM-Population.md](../MoM-Population.md).
- **Buildings.** A deep construction tree gates units, resources, and happiness (Granary → Farmers' Market; Barracks → Armory → Fighters' Guild → War College; Shrine → Temple → Parthenon → Cathedral; Sawmill, Miners' Guild, Marketplace, Bank, University, Wizards' Guild, etc.). Cross-referenced: [MoM-CityStuff.md](../MoM-CityStuff.md), [MoM-NextTurn-Buildings.md](../MoM-NextTurn-Buildings.md), [MoX-Data-BuildingDataTable.md](../MoX-Data-BuildingDataTable.md).
- **Expansion by magic.** Expansion is not only "build a settler and walk it" — summoning, *Move Fortress*, planar travel, and enchantments extend reach in ways a conventional 4X can't. **→ (future child doc: expansion & city founding)**
- **Roads & connectivity.** Roads (and enchanted roads) reduce movement cost and knit an empire together for defense and reinforcement. Cross-referenced: [MoM-Cityscape-Roads.md](../MoM-Cityscape-Roads.md), [MoX-Roads.md](../MoX-Roads.md).

---

## 3. eXploit

**Turning territory into the four resources that run the empire.**

- **The resource quartet.** Cities produce **Food** (feeds population & some units), **Production** (hammers → buildings & units), **Gold** (upkeep, buying, alchemy), and **Power/Mana**. Cross-referenced: [MoM-Calculations-Resources.md](../MoM-Calculations-Resources.md), [MoM-Calculations.md](../MoM-Calculations.md), [MoM-NextTurn-Production.md](../MoM-NextTurn-Production.md).
- **Power split — the magic economy.** Mana ("Power") is generated by cities, religious buildings, nodes, and enchantments, then **divided by the player among three sinks**: spell **Casting Skill** growth, spell **Research**, and a **Mana** reserve to actually cast with. This three-way slider is the strategic heart of the exploit layer. Cross-referenced: [MoM-MagicScreen.md](../MoM-MagicScreen.md), [MoM-Wizard-SpellCastingSkill.md](../MoM-Wizard-SpellCastingSkill.md), [MoM-MAGIC_SET.md](../MoM-MAGIC_SET.md).
- **Nodes as mana farms.** Captured/melded Nodes (Sorcery, Nature, Chaos) feed Power to their owner, boosted by the Mastery retorts and *Node Mastery*. Cross-referenced: [MoM-Lair.md](../MoM-Lair.md).
- **Alchemy.** Free (with the **Alchemy** retort) or 2:1 conversion between **Gold and Mana**, letting an empire liquidate one resource into the other on demand. Cross-referenced: [MoM-AlchemyPopup.md](../MoM-AlchemyPopup.md), [MoX-Data-MAGIC.md](../MoX-Data-MAGIC.md).
- **Minerals & terrain bonuses.** Special tiles (gold ore, silver, gems, iron, coal, mithril, adamantium, wild game, nightshade, quork/crysx crystals) boost a city's output or unlock better equipment. Cross-referenced: [MoM-Terrain-Map_Square_Is.md](../MoM-Terrain-Map_Square_Is.md).
- **Research.** Power routed to research learns new spells from your books; rarity and realm depth determine what's on offer. Cross-referenced: [MoM-Spells-ResearchLearn.md](../MoM-Spells-ResearchLearn.md).
- **Unrest & upkeep.** Rebels (from taxes, race relations, distance from the capital) cut output; Gold and Food upkeep drain the treasury; happiness buildings and spells counteract it. Cross-referenced: [MoX-RebelsUnrest.md](../MoX-RebelsUnrest.md).

---

## 4. eXterminate

**Fielding armies, fighting battles, and removing rivals.**

- **Units & stacks.** Armies move as **stacks of up to 9 units** on the overland map. Unit sources: **normal** units built in cities (race-specific rosters), **fantastic** units summoned by spells, **heroes** hired to your service, and monsters won from lairs. Cross-referenced: [MoM-Unit-vs-Stack.md](../MoM-Unit-vs-Stack.md), [MoM-MoveStack.md](../MoM-MoveStack.md), [MoM-GetUnits.md](../MoM-GetUnits.md).
- **Unit statistics.** Melee, ranged, defense, resistance, hit points, figures-per-unit, movement, and a rich set of **special abilities** (First Strike, Armor Piercing, Poison, Illusion, To Hit bonuses, immunities, flight, etc.). Cross-referenced: [MoM-BattleUnit.md](../MoM-BattleUnit.md), [MoM-UnitFigures.md](../MoM-UnitFigures.md), [MoM-UnitEffects.md](../MoM-UnitEffects.md).
- **Experience.** Units gain experience and rank up (Recruit → Regular → Veteran → Elite → …), improving stats; **Warlord** and *Crusade* push everyone higher. Cross-referenced: [MoM-NextTurn-ExperienceLevel.md](../MoM-NextTurn-ExperienceLevel.md).
- **Tactical combat.** Battles resolve on a **separate tactical battlefield** (see §9). This is where extermination actually happens — including **combat spells** cast live from your spellbook.
- **Conquest.** Taking a city lets you raze it, take it (with population loss and possible unrest), and capture its buildings/garrison. Cross-referenced: [MoX-Conquest.md](../MoX-Conquest.md).
- **Banishment vs. death.** Defeating a wizard's **Fortress** stack *banishes* them (they retreat and rebuild) rather than killing them outright; a banished wizard with no cities is eliminated. This gates the military victory path. **→ (future child doc: warfare & conquest)**

---

## 5. Magic System *(MoM-specific pillar)*

**The system every other pillar bends around.**

- **Five realms + Arcane.** All **214 spells** *(verified: `spl_Earth_To_Mud == 1` … `spl_Spell_Of_Return == 214` in `Spellbook.h`; `spl_MAX_SPELL_COUNT` follows)* belong to **Life, Death, Chaos, Nature, or Sorcery**, plus a shared **Arcane** category available to everyone (e.g. Spell of Return, Magic Spirit, Dispel Magic, Summoning Circle). A wizard's **spellbook counts** per realm determine which spells they can research and how fast. Cross-referenced: [MoM-Spellbooks.md](../MoM-Spellbooks.md), [MoM-SpellData.md](../MoM-SpellData.md), [999-Spells.md](../999-Spells.md).
- **Rarity tiers.** Within each realm, spells are **Common, Uncommon, Rare, and Very Rare** — deeper spellbook investment unlocks and speeds the rarer tiers. Cross-referenced: [MoM-Spellbooks-Pages.md](../MoM-Spellbooks-Pages.md).
- **Spell categories by use.** **Overland** vs **Combat** casting; **Summoning**, **City Enchantments**, **Unit Enchantments**, **Global Enchantments**, **Combat Instants**, **Battlefield Enchantments**, **Special/one-of-a-kind** spells. Cross-referenced: [MoM-Spells-Casting.md](../MoM-Spells-Casting.md), [MoM-Spells-Casting-Combat.md](../MoM-Spells-Casting-Combat.md), [MoM-Spells-Combat-Battlefield.md](../MoM-Spells-Combat-Battlefield.md), [Checklist-Spells.md](../Checklist-Spells.md).
- **Casting Skill, Research, Mana.** Casting Skill caps how much you can cast per turn/combat; Research unlocks spells; Mana is spent per cast. Overcasting risks fatigue. Cross-referenced: [MoM-Wizard-SpellCastingSkill.md](../MoM-Wizard-SpellCastingSkill.md).
- **Resistance & dispel.** Many spells are resisted by the target's Resistance stat; *Dispel Magic* and counters create a magic-vs-magic metagame. Cross-referenced: [MoM-Spells-Resistance.md](../MoM-Spells-Resistance.md), [MoM-Spell-Dispel.md](../MoM-Spell-Dispel.md).
- **Global enchantments & signature spells.** Realm-defining global spells (e.g. *Nature's Wrath*, *Eternal Night*, *Chaos Surge*, *Charm of Life*, *Time Stop*, *Armageddon*, *Great Wasting*) and the win-condition **Spell of Mastery**. Cross-referenced: [MoM-Spells-TargetGlobalEnchantmentScreen.md](../MoM-Spells-TargetGlobalEnchantmentScreen.md), [MoM-Spell-SpellOfMastery.md](../MoM-Spell-SpellOfMastery.md), [MoM-Spells-Special.md](../MoM-Spells-Special.md).
- **→ (future child doc: full magic system breakdown, realm-by-realm)**

---

## 6. Wizards, Realms & Retorts *(MoM-specific pillar)*

**Who you are, and how you're built.**

- **The wizard-avatar.** You *are* a wizard, not a faceless empire. Your identity is a bundle of **spellbook picks + retorts + starting spells + a home race**, embodied in a **Fortress** (your capital's magical heart) and a **Summoning Circle** (where summons appear). Cross-referenced: [MoM-Init-WZD.md](../MoM-Init-WZD.md), [MoX-NewGame-Init-Players.md](../MoX-NewGame-Init-Players.md).
- **14 preset wizards.** Merlin, Raven, Sharee, Lo Pan, Jafar, Oberic, Rjak, Sss'ra, Tauron, Freya, Horus, Ariel, Tlaloc, Kali — each a fixed personality with a themed book/retort loadout and AI temperament.
- **Custom wizard.** Spend **picks** on spellbooks (0–11 per realm within a total budget) and **retorts**; some retorts require minimum books in a realm (e.g. Divine/Infernal Power need Life/Death books; Mastery retorts need books in that realm).
- **18 retorts (special abilities).** *(verified: `retorts[18]` in the save/game data)* — Alchemy, Warlord, Chaos Mastery, Nature Mastery, Sorcery Mastery, Infernal Power, Divine Power, Sage Master, Channeler, Myrran, Archmage, Mana Focusing, Node Mastery, Famous, Runemaster, Conjurer, Charismatic, Artificer. **Myrran** notably starts you on the harder plane with its stronger races. Cross-referenced: [MoM-MirrorScreen.md](../MoM-MirrorScreen.md), [MoM-Wizard-SpellCastingSkill.md](../MoM-Wizard-SpellCastingSkill.md).
- **Personality & AI temperament.** Preset wizards carry diplomatic/strategic personalities (Maniacal, Ruthless, Aggressive, Militarist, Peaceful, Lawful, etc.) that shape their play and diplomacy. Cross-referenced: [MoM-NextTurn-AI.md](../MoM-NextTurn-AI.md).
- **→ (future child doc: wizard creation & retort reference)**

---

## 7. Races & Units *(MoM-specific pillar)*

**Fourteen town-races, each a different game.**

- **14 races**, split by home plane:
  - **Arcanus (10):** Barbarians, Gnolls, Halflings, High Elves, High Men, Klackons, Lizardmen, Nomads, Orcs, Dwarves.
  - **Myrror (4):** Beastmen, Dark Elves, Draconians, Trolls.
- **Race identity.** Each race has a **unique unit roster**, building restrictions (some can't build certain structures), population caps, growth rates, and quirks (Klackons' production bonus, Halflings' lucky slingers, Draconians fly and populate both planes, High Elves' innate magic resistance & happiness, Dark Elves' magic-ranged units, Trolls' regeneration, Dwarves' mining/gold bonus, Nomads' Rangers/Horsebows, etc.). Cross-referenced: [MoM-Population.md](../MoM-Population.md), [MoM-GameData.md](../MoM-GameData.md).
- **Normal unit tree.** Spearmen → Swordsmen → Bowmen → Cavalry → Shamans/Priests → Magicians → Engineers → Settlers → race-special elites. Gated by city buildings.
- **Racial relations.** Ruling a city of a race different from your capital's incurs unrest; some race pairs get along worse than others. Cross-referenced: [MoX-RebelsUnrest.md](../MoX-RebelsUnrest.md).
- **→ (future child doc: race-by-race unit rosters)**

---

## 8. Heroes & Items *(MoM-specific pillar)*

**RPG progression bolted onto the 4X.**

- **35 heroes and champions** *(verified: `heroes[0]`…`heroes[34]` — 35 records — in `gd_heroes_fields.h`)* who offer to join (for gold, via Fame, or via the *Summon Hero / Summon Champion* spells). A wizard may employ up to **6 at once** (`NUM_HEROES == 6`, the "Hero Slots"). Each is a named character with a class, starting level, and a set of **hero abilities** (Leadership, Might, Constitution, Agility, Arcane Power, Prayermaster, Blademaster, Lucky, Legendary, etc.; 23 ability types — `USW_HeroAbilities[23]`). Cross-referenced: [MoM-GameData-HeroAbilities.md](../MoM-GameData-HeroAbilities.md).
- **Leveling.** Heroes gain experience through combat, climbing rank titles and improving their abilities — the game's closest thing to unit-level RPG growth. Cross-referenced: [MoM-NextTurn-ExperienceLevel.md](../MoM-NextTurn-ExperienceLevel.md).
- **Magic items.** Heroes equip up to three **items** (swords, maces, bows, staffs, wands, armor, shields, rings, amulets) that grant attack/defense/movement/resistance bonuses and spell-like powers. Cross-referenced: [MoM-Item.md](../MoM-Item.md), [MoM-ItemPowers.md](../MoM-ItemPowers.md), [MoM-ItemView.md](../MoM-ItemView.md).
- **Crafting items.** With the **Artificer/Alchemy** retorts and *Create Artifact / Enchant Item* spells, you design custom items, spending mana per power. Cross-referenced: [MoM-Items-Make.md](../MoM-Items-Make.md), [MoM-ItemMakeScreen.md](../MoM-ItemMakeScreen.md), [MoM-ItemScreen.md](../MoM-ItemScreen.md).
- **→ (future child doc: hero & item reference)**

---

## 9. Tactical Combat *(MoM-specific pillar)*

**Where wars are actually won.**

- **The battlefield.** Battles move to a dedicated tactical screen: a gridded field with **terrain, obstacles, and (for cities) walls and a gate**. Attacker and defender each field their stack (up to 9 units). Cross-referenced: [Combat/](../Combat/), [MoM-BattleUnit.md](../MoM-BattleUnit.md).
- **Turn structure.** Alternating unit activation — move, melee, ranged/thrown attacks, and special-ability use. Multi-figure units lose figures as they take damage.
- **Combat spellcasting.** Both wizards can cast **combat spells** live during battle, spending combat casting skill and mana — buffs, direct-damage bolts, summons ("phantom"/conjured combat units), curses, and battlefield enchantments. This is the pillar where the magic system and the extermination pillar fuse. Cross-referenced: [MoM-Spells-Casting-Combat.md](../MoM-Spells-Casting-Combat.md), [MoM-SpellbookScreen-Combat.md](../MoM-SpellbookScreen-Combat.md), [MoM-Spells-Combat-UnitSpells.md](../MoM-Spells-Combat-UnitSpells.md).
- **City assault.** Walls, *Wall of Fire/Darkness*, and the central structure (Fortress/Lair/Node) shape city battles. Cross-referenced: the city-walls layout is documented in the combat notes.
- **Fantastic monsters.** Node/lair defenders and summoned creatures (from War Bears to Sky Drakes and Great Drakes) bring exotic abilities to the field.
- **→ (future child doc: tactical combat mechanics)**

---

## 10. The Two Planes *(MoM-specific pillar)*

**Arcanus and Myrror — asymmetry as a design pillar.**

- **Two full worlds.** Every game runs **two complete plane maps**. Arcanus is the "home" plane; **Myrror** is smaller-populated, richer in nodes/minerals, home to the four strongest races and tougher monsters. Cross-referenced: [MoX-EMM-CONTXXX.md](../MoX-EMM-CONTXXX.md), [MoX-WorldMap.md](../MoX-WorldMap.md).
- **Crossing over.** Movement between planes requires a cleared **Tower of Wizardry**, the *Planar Travel* spell, flying/planar units, or the **Myrran** retort (which starts you there). This makes the second plane a mid/late-game frontier.
- **Strategic doubling.** Two planes double the exploration, expansion, and node economy — and mean an enemy can be strong on one plane while you dominate the other.
- **→ (future child doc: planes & inter-plane travel)**

---

## 11. Diplomacy & the Wizard Metagame

**Talking, trading, and betraying the other wizards.**

- **Treaties.** Wizard Pact, Alliance, Peace, and declarations of War, plus trade of **gold and spells** between wizards. Relations drift on actions, borders, and personality. Cross-referenced: [MoM-Diplomacy.md](../MoM-Diplomacy.md), [MoX-Diplomacy.md](../MoX-Diplomacy.md), [MoM-Screen-Diplomacy.md](../MoM-Screen-Diplomacy.md).
- **Diplomatic actions.** Propose treaty, threaten/break treaty, demand tribute, ask to stop casting a spell, coordinate against a third wizard. Cross-referenced: [MoX-Diplomacy-Actions.md](../MoX-Diplomacy-Actions.md), [MoM-Screen-Diplomacy-ProposeTreaty.md](../MoM-Screen-Diplomacy-ProposeTreaty.md), [MoM-Screen-Diplomacy-ThreatenBreakTreaty.md](../MoM-Screen-Diplomacy-ThreatenBreakTreaty.md).
- **Fame & reputation.** Fame (from battles, heroes, buildings, spells) improves hiring, unrest, and diplomacy; a fearsome reputation cuts both ways.
- **→ (future child doc: diplomacy reference)**

---

## 12. Game Setup & Options

**The knobs set before turn one.**

- **Difficulty:** Intro, Easy, Normal, Hard, Extreme, Impossible — scales AI bonuses, starting resources, and lair strength.
- **World / land size** and **number of opponents (1–4)**.
- **Magic level:** Weak / Normal / Powerful — how much mana and how many nodes the world holds.
- **Wizard choice:** preset or custom (see §6), plus home **race** and **banner** color.

Cross-referenced: [MoX-NewGame.md](../MoX-NewGame.md), [MoX-NewGame-Init.md](../MoX-NewGame-Init.md), [MoX-NewGame-Warning.md](../MoX-NewGame-Warning.md).

---

## 13. Victory Conditions

**Two roads to winning.**

1. **Spell of Mastery.** Research and cast the ultimate Arcane spell; surviving the long casting window (during which every rival turns on you) wins the game outright. Cross-referenced: [MoM-Spell-SpellOfMastery.md](../MoM-Spell-SpellOfMastery.md).
2. **Last wizard standing.** Eliminate every rival — banish each from their Fortress and leave them with no cities to rebuild in (see §4). Cross-referenced: [MoX-Conquest.md](../MoX-Conquest.md).

---

## 14. Presentation & UX systems

*(Supporting features that aren't "4X mechanics" but are part of the game.)*

- **Advisor screens.** Advisors/reports (magic, cities, units, armies, diplomacy) surface empire state. Cross-referenced: [MoM-AdvisorScreen.md](../MoM-AdvisorScreen.md), [MoM-ChancellorScreen.md](../MoM-ChancellorScreen.md), [MoM-ArmyListScreen.md](../MoM-ArmyListScreen.md), [MoX-Reports.md](../MoX-Reports.md).
- **Automation aids.** Grand Vizier (auto-manage city production) and other quality-of-life toggles. Cross-referenced: [MoM-GrandVizier.md](../MoM-GrandVizier.md).
- **Audio/visual identity.** Cityscape art, spell animations, unit figures, the mirror/magic screens, palette-cycling effects. Cross-referenced: [MoM-Cityscape.md](../MoM-Cityscape.md), [MoM-Spells-Animations.md](../MoM-Spells-Animations.md).

---

## Feature index (quick map)

| # | Pillar | Core features | Future child doc |
| --- | --- | --- | --- |
| 1 | eXplore | Fog of war, two-plane vision, terrain/movement, lairs & nodes, Towers, map tools | exploration & vision |
| 2 | eXpand | Outposts→cities, population, city radius, buildings, roads, magical expansion | expansion & founding |
| 3 | eXploit | Food/Production/Gold/Power, power split, nodes, alchemy, minerals, research, unrest | economy & resources |
| 4 | eXterminate | Units & stacks, stats, experience, conquest, banishment | warfare & conquest |
| 5 | Magic System | 5 realms + Arcane, rarity tiers, spell categories, skill/research/mana, dispel, globals | magic (realm-by-realm) |
| 6 | Wizards & Retorts | 14 presets, custom builder, 18 retorts, personalities | wizard & retort ref |
| 7 | Races & Units | 14 races, rosters, racial relations | race rosters |
| 8 | Heroes & Items | ~35 heroes, abilities, leveling, items & crafting | hero & item ref |
| 9 | Tactical Combat | Battlefield, activation, combat spells, city assault, monsters | combat mechanics |
| 10 | Two Planes | Arcanus/Myrror, crossing, strategic doubling | planes & travel |
| 11 | Diplomacy | Treaties, actions, fame/reputation | diplomacy ref |
| 12 | Setup | Difficulty, size, magic level, wizard/race/banner | — |
| 13 | Victory | Spell of Mastery, last wizard standing | — |
| 14 | Presentation | Advisors, automation, audio/visual | — |

---

## Cross-references to existing `doc/`

The mechanics behind these features are documented across `doc/`. High-value entry points:

- **Overview / comparison:** [000-MoX_vs_MoM.md](../000-MoX_vs_MoM.md)
- **Spells:** [999-Spells.md](../999-Spells.md), [MoM-Spell-Overview.md](../MoM-Spell-Overview.md), [Checklist-Spells.md](../Checklist-Spells.md), [Spells/](../Spells/)
- **Cities & economy:** [MoM-CityStuff.md](../MoM-CityStuff.md), [MoM-Population.md](../MoM-Population.md), [MoM-Calculations-Resources.md](../MoM-Calculations-Resources.md)
- **Combat:** [Combat/](../Combat/), [MoM-BattleUnit.md](../MoM-BattleUnit.md)
- **AI / turn processing:** [MoM-NextTurn-AI.md](../MoM-NextTurn-AI.md), [ComputerPlayer/](../ComputerPlayer/), [NextTurn/](../NextTurn/)
- **World & terrain:** [MoX-WorldMap.md](../MoX-WorldMap.md), [MoX-Terrain.md](../MoX-Terrain.md), [MoX-EMM-CONTXXX.md](../MoX-EMM-CONTXXX.md)

---

*Status: initial overview & breakdown. Child docs under [FeatureFreak/](.) to be added per the "→ (future child doc)" markers above.*
