# Warlord on ReMoM — gap analysis against Caster of Magic for Windows

What ReMoM still needs before Suppanut4625's **Warlord** mod (1.5.12.6.2) runs as a drop-in. Warlord is built for
**Caster of Magic for Windows** (CoM2, 1.05.11). The analysis compares Warlord's files with a full copy of CoM2 that
Seravy provided, and with ReMoM's code on branch `modding-phase1-lua-hooks`.

Decision already made: CoM2 support is a **rules profile**. The Community Patch, CoM1 (DOS) and other mods get
profiles of their own, and 1.31 stays the default profile with its behaviour unchanged. So this plan builds a
general profile layer, and CoM2 is the first big profile to use it.

## 1. What Warlord is

| Part | Count | Notes |
|---|---|---|
| CAS scripts | 38 | all parse in ReMoM ([MOD/src/CAS.c](../../MOD/src/CAS.c)), with 271 warnings for syntax CoM2 tolerates |
| INI tables | 30 | full replacements of CoM2's: units, spells, races, buildings, items, item powers, heroes, retorts, wizards, terrain, ore, difficulty, AI and more |
| MODDING.INI | 667 keys in 18 sections | the same 667 keys as CoM2's own file; Warlord changes 117 of them |
| HELP.TXT | 608 KB | help text |
| Images | about 1,700 | PNG and BMP files with CoM2's names (top level, `Units/`, `Wizard/`, `tiles/`) |
| Music | 10 WAV files | |

Warlord overrides almost every CoM2 text file. It inherits only `DISJUNCT.CAS`, `DISPEL.CAS`, `FortLig.CAS`,
`ELEVATION.INI`, `tax.ini` and `HELP.TXT` from CoM2, plus every image and sound it does not replace (CoM2's `Data/`
holds about 3,600). **A drop-in therefore needs the CoM2 install as well as Warlord**, the same way CoM2's own
"Load Mod" copies the mod over `data/`.

## 2. Gaps

### 2.1 Rules profile: engine behaviour (largest)

CoM2 is a reimplementation of the game with rebalanced rules, and many of the changes are in its code, not its data.
Examples from its manual (`CASTERWIN.odt`):
- **Map:** sizes vary by setting and player count, and can differ between planes (up to 200×200 per plane in its
  data model). New climate, mineral and continent options.
- **Terrain and economy:** new terrain yields; 3 new ores (Orihalcon among them); new growth and maximum-population
  formulas; a production queue; tax and unrest tables.
- **Game flow:** new random events (Aether Flux, Atlantis, Monster Invasion, ...); a new victory condition; wizard
  elimination; 9 difficulty levels; scoring options.
- **Combat:** a 29×29 combat map; range penalties; changed spell behaviour throughout.

**MODDING.INI is the tunable part of these rules:** 667 keys in `[EncounterBudget] [World] [Terrain] [CombatTerrain]
[EncounterAmounts] [Treasure] [Spells] [UI] [Elimination] [Gameplay] [Neutrals] [Diplomacy] [AI] [CombatMovement]
[OverlandMovement] [System]`, plus `[DataSize]` and `[Scripts]`. Each key is a rule value a profile must honour. The
rest of the rules are fixed in the engine and have to be implemented from the manual (and checked, see §3).

**Shared with other profiles:** the mechanism (named rule switches and values, 1.31 as the baseline) and most
individual rules. The Community Patch and CoM1 change many of the same formulas. Build the switch points once, in the
1.31 code paths, and let each profile set them.

### 2.2 Data tables and sizes

| Table | Warlord | CoM2 limit | ReMoM now ([MoX/src/MOM_DEF.h](../../MoX/src/MOM_DEF.h)) |
|---|---|---|---|
| Unit types | 364 (`Unittypes=363`) | 400 | 198 |
| Spells | 401 | 400 | 214 (`NUM_SPELLS` 240 slots) |
| Races | 26 (`MaxRaces=25`) | 100 | 14 |
| Buildings | 74 | 100 | 35 (`NUM_BUILDINGS` 36) |
| Hero types | 48 | 85 | 35 |
| Item powers | 87 | 150 | — |
| Players | — | 1 human + 13 AI | 6 including neutral |
| Units in play | — | 39,999 | 1,009 |
| Cities | — | 1,000 | 100 |
| Unit levels | — | 20 | 9 |

Work:
- **Loaders:** read CoM2's INI tables into ReMoM's structures. `UNITS.INI` uses 100 distinct keys, including about
  70 ability flags, many of them CoM2's own (`Supernatural`, `CreatePower`, `Egoism`, `HolyBonus`, ...).
  `spells.ini` uses 74.
- **Grow the counts and arrays** above, plus anything indexed by them. Examples: `bldg_status[]` in `s_CITY` becomes
  `[100]`; `spells_list[]` and `Globals[]` in `s_WIZARD` grow; city and global enchantments go up to 100 each.
  New save sections follow the existing pattern ([MoX/src/SAVEFILE.c](../../MoX/src/SAVEFILE.c)).
- **IDs:** CoM2 renumbers buildings, enchantments, retorts and so on. Today [MOD/src/CAS_Game.c](../../MOD/src/CAS_Game.c)
  translates them for scripts. Under a CoM2 profile the game would use CoM2's numbering itself, so the translation
  tables move from the CAS layer into the profile's loader.
- `Units.dat`, `Spells.dat` and `Items.dat` are caches CoM2 builds from the INIs ("delete UNITS.DAT to rebuild"), so
  they need no support.

**Shared:** the loaders are CoM2-specific (CoM1 and the Community Patch keep LBX tables). The larger arrays and
dynamic counts are shared: every profile with more units or spells needs them.

### 2.3 Assets

CoM2 has **no LBX files**. Every picture is its own file, for example:

| Pattern | Count | Content |
|---|---|---|
| `Units/u<type>x<facing>_<frame>.png` | 791 | unit figures |
| `Font<n>_<char>.bmp` | 663 | fonts |
| `Building<n>_<frame>.png` | 331 | city buildings |
| `ui<n>.png` | 243 | interface pieces |
| `ammo<n>_<dir>_<frame>.png` | 192 | missiles |
| `ux<n>.png` | 167 | ability icons (Warlord `MaxUniticons=409`) |
| `GE<n>.png` | 83 | global enchantments |
| `Music/<n>.wav` | 119 | music |

There are about 200 such name families. The art keeps 1.31's resolution and is 8-bit with a palette: for example
`Building10_0.png` is 40×33 indexed. So a CoM2 image can stand in for an LBX frame directly.

Work: an asset layer where a profile can supply a picture by name, with a table from each CoM2 name family to the LBX
entry (or the new slot) it replaces. New content (unit types 199+, buildings 36+, new icons) needs slots that 1.31
does not have, so it depends on 2.2.

**Shared:** the asset override layer is also the planned native mod feature ("asset overrides" in
[doc/@Devel/Modding.md](../@Devel/Modding.md)). Only the name tables are CoM2-specific.

### 2.4 Scripts

**Hooks.** CoM2 calls 40 scripts (`MODDING.INI [Scripts]`). ReMoM has 5:

| CoM2 hook | ReMoM |
|---|---|
| EconomyFood, EconomyProd, EconomyGold, EconomyResearch, EconomyPower | **done**, with CoM2's inputs ([MoM/src/City_Hook_Inputs.c](../../MoM/src/City_Hook_Inputs.c)) |
| System (MASTER.CAS) | **done** (`master=` / `System=`) |
| EconomySkill, EconomyUnrest, EconomyMaxPop, EconomyGrowth | not yet: same pattern as the five above |
| NewSettler, CreateUnit, UnitRecalculate, UnitRecalculateEarly, DisplayUnitAbilitiy, CombatInfo | not yet: need unit stats in scripts (`GETSTAT`/`SETSTAT`, enchantment flags) |
| EnterCombat, EndOfCombat, CombatEndTurn, CombatSpellEffect, FortLig | not yet: combat |
| OverlandSpellEffect, OverlandEndTurn, EnterGame, TreasureRareSpell | not yet |
| EffectiveCastingCost, DispelResistance, DispelPower, DispelRoll, DisjunctionRoll, ProductionCostReduction | not yet: small numeric hooks, like the economy ones |
| AIStartingSpell, AIResearch, AIHomeRace, AIPersonalityObjective, AICustomWizard, AIProducion, AIBuildPrio, AIPreHousing | not yet: AI decisions |

**Built-ins.** Warlord's scripts use 111 built-ins, and ReMoM implements 46 of them (all 40 the five economy scripts
need). CoM2's own scripts use 72, and 34 of those are implemented. Most of the rest read or set unit stats, spells and
combat state, so they come with the hooks above.

**Shared:** the CAS interpreter is CoM2/CoM1-specific. Hooks are not: they are the native modding points, and Lua uses
the same dispatcher ([MOD/src/MOD_Hook.h](../../MOD/src/MOD_Hook.h)).

### 2.5 Saves

New and grown structures become new save sections (already the pattern). Script slots are saved
(`CASCITYD`/`CASWIZD `). Still missing: recording the rules profile and loaded mod in the save, and clearing the
script slots when a new game starts.

## 3. Reference material from CoM2

- **`CasApi/`** — Seravy ships CoM2's gameplay engine as a separate DLL, "for tools such as save game editors".
  - `CasApi.pas` declares its **898 functions** (with comments).
  - `Typedec.pas` gives CoM2's record layouts (`CityT`, `WizardT`, `MapTileT`, `DiplomacyT`, ...).
  - `SharedConstants.pas` gives its limits and enumerations.

  Together these are the most precise description of CoM2's data model and rules surface available without the exe.
  On a Windows machine the DLL itself could serve as a **test oracle**: run the same situation through `Casapi.dll`
  and through ReMoM's CoM2 profile, and compare the results. This is a development-only use (blast-radius tier C,
  never shipped); vet it per [doc/@Devel/Dependency-Vetting.md](../@Devel/Dependency-Vetting.md) before relying on it.
- **`CASTERWIN.odt`** — the manual; its Game Mechanics chapters describe the rule changes.
- **`Scripts.TXT`** — the CAS reference (already used).

- **`Caster.exe` 1.05.11** (SHA-256 `3cc24227…af8d1e`), from a parsed dump: section layout, ASCII and UTF-16 strings,
  and the import, resource and section tables.
  - **Debug symbols.** The exe carries a 7 MB Borland TD32 debug section, so every procedure and global has its
    Delphi name: 26,142 `@Unit@Name` symbols. The gameplay units, with procedure counts: `Economy` 41, `City` 36,
    `Map` 47, `Newgame` 18, `Neutrals` 9, `Combat` 69, `Automaticcombat` 30, `Spells` 60, `Units` 48, `Wizard` 34,
    `Diplomacy` 58, `Ai` 98, `Aicombat` 26, `Game` 71, `Item` 16, `Heroes` 16, `Scripts` 75 (the CAS interpreter),
    plus `Castercore` with 716 and `Vars` with 839 globals. These names are the work breakdown for §2.1. For example,
    `Economy` has `citygoldproduction`, `cityroadbonus`, `cityterraingold`, `minimalfarmers`, `volcanopowerincome`, ...
  - **Rule globals.** 468 of MODDING.INI's 667 keys are `Vars` globals with the same name, for example
    `@Vars@NearShoreGoldBonus`. Most of the other 199 are script bindings (stored as `<Hook>Script`) or numbered keys
    loaded into arrays (`ExpForLevel1..9` → `ExpToLevel`). `Vars` also has 371 globals that are not MODDING keys:
    other INIs' tables (`RacialGrowth`, `OreTable`, `LevelBonus*`, `RetortPickCosts`, ...) and runtime state.
  - **CAS keywords.** The interpreter's keyword table in `.data` matches ReMoM's built-in table, except two names
    ReMoM had wrongly listed (`DEALDAMAGE`, `SETSPELLSTATE`, now removed) and `NODEDATA`, the exe's spelling of the
    documented `GETNODEDATA` (both accepted now).
  - **Asset names.** 259 image files are named literally in UTF-16 (`AlchemyBack.png`, `CityBackground.png`,
    `Gold10.png`, ...); the numbered families from §2.3 are built at run time.

  The TD32 data also maps each procedure to its code address and source line. With the `.text` bytes (the `regions/`
  hex dumps of the same parse), any single rule can be disassembled and checked, procedure by procedure.

Not obtained: `Casapi.dll` (8.9 MB) is under the connector's 10 MB file limit, but too large for its message size.
A zip, or the same parse the exe got, would give its export table.

## 4. Suggested order

1. **Profile mechanism.** Pick the profile at new game or load, store it in the save, and add the first switch points
   in the 1.31 code. Prove it with one small Community Patch fix and one CoM2 rule.
2. **Dynamic counts and larger arrays** (2.2), with save sections. Every later step depends on this.
3. **CoM2 INI loaders** for units, spells, races and buildings, then the rest.
4. **Asset override layer** and the CoM2 name tables.
5. **Remaining hooks**, in order: the economy ones, the small numeric ones, unit stats, overland, combat, AI. Built-ins
   come with each hook.
6. **CoM2 engine rules**, by subsystem, following the exe's units (`Map`/`Newgame`, `Economy`/`City`, `Combat`, `Ai`), checked against the CasApi oracle and the exe's code where
   possible.
7. **Converter** to the native ReMoM mod format, once 1–5 define that format.
