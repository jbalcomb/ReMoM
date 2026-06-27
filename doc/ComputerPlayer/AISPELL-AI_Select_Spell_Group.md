AISPELL-AI_Select_Spell_Group.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_Select_Spell_Group.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_Select_Spell_Group.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_Select_Spell_Group.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_Select_Spell_Group.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_Select_Spell_Group.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_Select_Spell_Group.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                    |-> AI_Has_More_Myrror_Cities()
                    |-> Player_Summon_City_Is_Fortress_City()

---

# `AI_Select_Spell_Group` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AI_Select_Spell_Group` | [AISPELL.c:396-823](../../MoM/src/AISPELL.c#L396-L823) | Decides *which kind* of overland spell the AI wants to cast this turn: builds 11 weighted "category" scores and returns a weighted-random pick `0..10`. |

> **Status: BUILDS CLEAN — faithful to the asm.** The body ([AISPELL.c:396-823](../../MoM/src/AISPELL.c#L396-L823), marked `/* GEMINI */`) has been adapted to the production types/symbols (city-enchantment arrays, `magic_power_strategy`, `cp_target_3`, `g_ai_spell_group_flags`, the AI-stack globals, prototypes) and `cmake --build --preset MSVC-debug` compiles and links with no errors or warnings. The body has been compared 1:1 with `AI_Select_Spell_Group.asm` (see [Fidelity](#fidelity)), carrying the documented OG bugs ([B1/B2](#og-quirks--bugs-preserve-when-reconstructing)) and structural-but-equivalent forms ([D1/D2](#ida-c-decompilation-divergences)). The algorithm walkthrough below is the authority-derived spec.

## Purpose

Called once per AI overland-cast decision, after [`AI_Compute_Spells_Info`](AISPELL-AI_Compute_Spells_Info.md) has populated `g_ai_spell_group_flags[]` (referred to in the asm by its pre-rename symbol `AI_OVL_Spell_Cats`). The picker computes a weight for each of 11 spell *categories*, then calls `Get_Weighted_Choice` to select one proportionally to its weight. The returned index drives the `switch` in `AI_Spell_Select` ([AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md)).

### The 11 category weights

These 11 `int16_t` values are a **contiguous stack array** (frame `-3Ah .. -26h`), which is exactly what is handed to `Get_Weighted_Choice(&No_Spell_Modifier, 11)` at the end. The return value is the chosen slot index.

| Slot | Frame | Name | Meaning |
|---|---|---|---|
| 0 | `-3Ah` | `No_Spell_Modifier` | cast nothing (always forced to `1` at the end — guaranteed fallback) |
| 1 | `-38h` | `Summon_Modifier` | summon a creature |
| 2 | `-36h` | `UnitEnhance_Modifier` | unit buff |
| 3 | `-34h` | `CityEnhance_Modifier` | city buff |
| 4 | `-32h` | `Disenchant_Modifier` | disenchant own cities/nodes/units |
| 5 | `-30h` | `Disjunction_Modifier` | disjoin enemy overland enchantments |
| 6 | `-2Eh` | `Summoning_Circle_Modifier` | move the summoning circle |
| 7 | `-2Ch` | `Curse_Modifier` | offensive spell at the chosen target wizard |
| 8 | `-2Ah` | `Suppression_Modifier` | suppression globals (Suppress Magic, etc.) |
| 9 | `-28h` | `GlobalE_Modifier` | overland enchantments |
| 10 | `-26h` | `Spell_of_Mastery_Modifier` | cast Spell of Mastery |

`Get_Weighted_Choice` ([random.h:31](../../MoX/src/random.h#L31)) returns `0..10`; that index is the picked category.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Spell_Select` | per [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) | Once per AI overland-cast decision; its return is the `switch(spell_category)` selector. |

## Code walk

Section line refs are the disassembly `AI_Select_Spell_Group.asm` (the authority). `players_spell_list` is set up **1-based** — `_players[player_idx].spells_list - 1` (asm 35, `offset _players.spells_list-1`) — matching the 1-based `spell_idx` convention used in [`AI_Compute_Spells_Info`](AISPELL-AI_Compute_Spells_Info.md). All `spl_*` and `SGRP_*` constants are from [Spellbook.h](../../MoM/src/Spellbook.h) / [AISPELL.h:19-99](../../MoM/src/AISPELL.h#L19-L99).

### Init + early No-Spell weight (asm 40-54)

Zero `modifiers[0..10]`. If `_turn < 50`, set `No_Spell_Modifier = 50 - _turn` (early game ⇒ more willing to do nothing). **This value is dead** — it is unconditionally overwritten by `No_Spell_Modifier = 1` just before the dispatch (asm 1506); see [quirk Q1](#og-quirks--bugs-preserve-when-reconstructing).

### Summon context (asm 55-145)

- `Summon_Plane = _players[player_idx].summon_wp`.
- `UU_Summon_Landmass = _landmasses[...]` at the summon point — **computed, never read** ([Q2](#og-quirks--bugs-preserve-when-reconstructing)).
- `Summon_City` loop over `_cities` matching the summon `wx/wy/wp` — **computed, never read** ([Q2](#og-quirks--bugs-preserve-when-reconstructing)).

### Summon weight (asm 146-241)

- Base `Summon_Modifier = 100`; `+= (6 - Player_Hero_Count(player_idx)) * 10` (fewer heroes ⇒ wants to summon more); `+= 30` if `Magic_Strategy == 3`.
- `Have_Spirit`: scan `_units` for an own unit on the summon plane whose type has `UA_MELD` (a magic/guardian spirit). If none, scan `_NODES[0..29]` for an enemy-owned node on the summon plane → sets `AI_NodeOpportunity = 1` and `Summon_Modifier += 30`. The node loop **terminates early** once `AI_NodeOpportunity` is set (asm 233-237 loop guard `&& AI_NodeOpportunity == 0`); the IDA-C loses this ([D1](#ida-c-decompilation-divergences)).

### Army totals (asm 243-279)

`Own_Army_Value` (sum of `AI_Own_Stacks[].value`) and `Hostile_Army_Value` (sum of `AI_Enemy_Stacks[].value`) are accumulated as `long`s — **both computed, never read** ([Q2](#og-quirks--bugs-preserve-when-reconstructing)).

### Summon weight, retort/fortress adjustments (asm 280-311)

- If `conjurer` **or** `channeler` retort: `Summon_Modifier = (Summon_Modifier * 3) / 2`.
- If `Player_Summon_City_Is_Fortress_City(player_idx) == 0` (asm `WIZ_SummonInFortress`; the misleading asm name notwithstanding, it returns true iff the summon city *is* the fortress city — so `== 0` means it is not): `Summon_Modifier *= 5`.

### Disenchant weight (asm 312-525)

Gate: only if the wizard knows `spl_Disenchant_Area` **or** `spl_Disenchant_True`. Then:

- Own cities (`_CITIES[i].owner_idx == player_idx`): `+40` for each of `CHAOS_RIFT`, `EVIL_PRESENCE`, `CURSED_LANDS`, `PESTILENCE`, `FAMINE` present.
- Neutral cities (`owner_idx == 0`): `+300` if `FLYING_FORTRESS` present. **In the asm this is a second, independent `if`, not an `else`** ([D2](#ida-c-decompilation-divergences)).
- Own warped nodes (`_NODES[i].flags & NF_WARPED`): `+100` each.
- Neutral (`owner_idx == 0`) enchanted units sharing the wizard's `Fortress_Landmass`: `+5` each.

### Disjunction weight (asm 527-861)

Gate: only if the wizard knows `spl_Disjunction` **or** `spl_Disjunction_True`. Loop over every player `i != player_idx`, scoring `i`'s active overland enchantments. Here `_players[i]` **is** properly indexed (unlike the suppression block below). Notable rules:

| Enemy global on player `i` | Weight added | Condition |
|---|---|---|
| `ETERNAL_NIGHT` | +100 | `_players[player_idx].Prim_Realm != sbr_Death` |
| `EVIL_OMENS` | +300 / +150 | Prim / Sec realm of `i` is Life or Nature |
| `Zombie_Mastery` | +100 | |
| `Aura_of_Majesty` | +25 **twice** (= +50) | checked at asm 614 and 638 ([Q4](#og-quirks--bugs-preserve-when-reconstructing)) |
| `Wind_Mastery` | +25 | |
| `Suppress_Magic` | +2000 | |
| `Natures_Wrath` | +400 / +200 | Prim / Sec realm of `i` is Death or Chaos |
| `Herb_Mastery` | +50 | |
| `CHAOS_SURGE` | +100 | neither realm of `i` is Chaos |
| `Doom_Mastery` | +200 | |
| `Great_Wasting` | +50 | |
| `Meteor_Storm` | +100 | |
| `Armageddon` | +200 | |
| `TRANQUILITY` | +500 / +250 | Prim / Sec realm of `i` is Chaos |
| `LIFE_FORCE` | +500 / +250 | Prim / Sec realm of `i` is Death |
| `Crusade` | +100 | |
| `Just_Cause` | +10 | |
| `HOLY_ARMS` | +100 | |
| `PLANAR_SEAL` | +200 | `i == 0` **or** `Random(5) == 1` — consumes RNG (asm 832-834) |
| `PLANAR_SEAL` | +25 | unconditional (second check) |
| `Charm_of_Life` | +200 | |

### Spell of Mastery weight (asm 863-867)

If `spl_Spell_Of_Mastery` known: `Spell_of_Mastery_Modifier = 500`.

### Target wizard + Curse weight (asm 868-1076)

- If `_cp_hostile_opponent_count > 0`: build `Opponent_Weights[]` over the hostile-opponent list — `(100 - Visible_Rel) - 2*Hidden_Rel`, floored at 0, `+50` if `Dipl_Status >= 3`, and `*(_difficulty + 1)` when the opponent is player 0 — then `OVL_TargetWiz = Get_Weighted_Choice(...)`. Otherwise `OVL_TargetWiz = ST_UNDEFINED`.
- If a target was chosen **and** any offensive category flag is set — `SGRP_Corruption`, `SGRP_Famine`, `SGRP_RaiseVolcano`, `SGRP_DestroyCity`, `SGRP_WarpNode`, `SGRP_Stasis`, group `9`, `SGRP_OVL_Damage`, `SGRP_Earthquake`, group `74` (`4Ah`), `SGRP_TimeStop`, a `Historian`-relative trait byte at `+53h`, `SGRP_DeathWish`, `SGRP_HarmWizard` — then `Curse_Modifier = (Opponent_Weights[OVL_TargetWiz] + TBL_AI_PRS_War_Mod[Personality]) / 4`. Groups `9` and `74` have no named `SGRP_*` ([D3](#ida-c-decompilation-divergences)).
- Floor: if `Curse_Modifier < 0`, set to 0.

### City-enhance weight (asm 1077-1107)

- If `SGRP_WallsRituals` or group `27` (`1Bh`) flagged: set `CityEnhance_Modifier = 50` or `5` based on a `Contacted` test — **but the asm indexes `_players[0].Dipl.Contacted[player_idx]`, a hardcoded player-0 base with `player_idx` as a byte offset** ([B1](#og-quirks--bugs-preserve-when-reconstructing)).
- If `SGRP_CityBuff_1`, `SGRP_CityBuff_3`, `SGRP_TerrainMod`, or `SGRP_CityBuff_2` flagged: `CityEnhance_Modifier = 150` (overrides the above).

### Unit-enhance weight (asm 1108-1177)

- If `SGRP_MoveBuff_1/2/3` or `SGRP_Gates` flagged: `UnitEnhance_Modifier = 0` (`_turn < 50`) / `100` (`< 200`) / `200`.
- If any of `SGRP_AttackBuff_1/2`, groups `50`(`32h`)/`72`(`48h`), `SGRP_DefenseBuff_1/2`, `SGRP_ImmolInvuln`, `SGRP_RegenMassInvis`, `SGRP_Heals` flagged: `+= (250 - _turn)/2` when `20 < _turn < 200`, else `+= 25`.

### Global-enchant & suppression weights (asm 1178-1444)

A long run of "knows spell *and* that global is not already active ⇒ add weight" tests feeding `GlobalE_Modifier` (slot 9) or `Suppression_Modifier` (slot 8). Most add `+100` to slot 9 (e.g. Herb Mastery, Awareness `+500`, Wind Mastery `+75`, Spell Binding, the Chaos/Death globals, etc.). The suppression entries — **Nature's Wrath, Life Force, Tranquility, Evil Omens** — gate on realm but read **`_players[0]` (the human) realms, not `_players[player_idx]`** ([B2](#og-quirks--bugs-preserve-when-reconstructing)). Finally `GlobalE_Modifier` is capped: **if `> 400` it is set to `300`** (not clamped to 400) ([Q3](#og-quirks--bugs-preserve-when-reconstructing)).

### Planar Seal, mana gate, summoning circle (asm 1446-1504)

- If `spl_Planar_Seal` known, its global inactive, and `AI_Has_More_Myrror_Cities(player_idx) == 1` (asm `AI_MyrrorAdvantage`): `GlobalE_Modifier += 50`.
- **Mana gate:** if `mana_reserve < 200`, force `GlobalE_Modifier = 0` and `Suppression_Modifier = 0`.
- If `spl_Summoning_Circle` known and `AI_SCircle_Reevals[player_idx] < 0` and the wizard owns ≥1 city: `Summoning_Circle_Modifier = 100`.

### Dispatch (asm 1506-1520)

`No_Spell_Modifier = 1` (forces a nonzero fallback weight so a choice always exists), then `return Get_Weighted_Choice(&No_Spell_Modifier, 11)`.

## Build status

`cmake --build --preset MSVC-debug` builds clean (0 errors, 0 warnings in this function). The body has been fully adapted to the production types/symbols.

Minor (non-blocking) note: `Hostile_Army_Value` / `Own_Army_Value` are `int16_t` ([401-402](../../MoM/src/AISPELL.c#L401-L402)); the OG accumulates `long`. They are NIU ([Q2](#og-quirks--bugs-preserve-when-reconstructing)), so the width is cosmetic — widening to `int32_t` would match the asm exactly.

## Fidelity

The full body has been compared 1:1 with the asm — init bounds, the summon / Have-Spirit / army sections, the entire Disenchant block, all Disjunction scoring rules, the target-wizard build, the 14-term Curse OR-chain, the CityEnhance `Contacted` 50/5 selection, the turn-tiered City/Unit-enhance weights, the GlobalE/Suppression run, the cap-to-300, the mana gate, and the Summoning-Circle block all match. The reconstruction is faithful, modulo the documented [D1/D2](#ida-c-decompilation-divergences) structural-but-equivalent forms and the preserved [B1/B2](#og-quirks--bugs-preserve-when-reconstructing) OG bugs.

## OG quirks & bugs (preserve when reconstructing)

Per the "faithful to the disassembly" rule, reconstruct these exactly as the asm has them — do **not** correct them.

- **Q1 — dead early No-Spell weight.** `No_Spell_Modifier = 50 - _turn` (asm 54) is overwritten by `= 1` (asm 1506). The early computation never affects the result.
- **Q2 — NIU computations.** `UU_Summon_Landmass`, `Summon_City`, `Own_Army_Value`, `Hostile_Army_Value` are all computed and never read. Preserve the dead work (it consumes no RNG, so it is behaviorally inert).
- **Q3 — GlobalE cap to 300.** `if (GlobalE_Modifier > 400) GlobalE_Modifier = 300;` (asm 1442-1444) — over-threshold collapses to 300, not a clamp to 400.
- **Q4 — Aura of Majesty double-counted.** In the Disjunction loop it is tested twice, `+25` each (asm 614, 638), netting +50.
- **B1 — Contacted index bug (OGBUG).** The city-enhance `Contacted` test reads `_players[0].Dipl.Contacted` at byte offset `player_idx` (asm 1085-1086, `mov bx, [bp+player_idx]` then `cmp [_players.Dipl.Contacted+bx], 1`) instead of `_players[player_idx]`. Preserve.
- **B2 — suppression realm reads player 0 (OGBUG).** The Nature's Wrath / Life Force / Tranquility / Evil Omens suppression gates read `_players[0]`'s `Prim_Realm`/`Sec_Realm` (asm 1199, 1377, 1393, 1420 — no `+bx`), not the acting CP's realms. Preserve.

## IDA-C decompilation divergences

The Piethawn `AI_Select_Spell_Group.c` is a decompiler reading, not OG truth. Where it disagrees with the asm, follow the asm:

- **D1 — node-opportunity loop.** IDA-C (lines 93-97) renders a full `for (i = 0; i < 30; i++)`. The asm loops `while (i < 30 && AI_NodeOpportunity == 0)` — it stops as soon as an opportunity is found. Same end flag value, different iteration count (and a future RNG-sensitive edit must respect the asm form).
- **D2 — Flying Fortress branch.** IDA-C (lines 123-131) uses `if (owner == player) {...} else if (owner == 0) {...}`. The asm is two independent `if`s sharing a fall-through (own-city enchant checks, then an unconditional `owner == 0` Flying-Fortress check). Equivalent only because a CP's `player_idx` is never 0; reconstruct as the asm's two `if`s.
- **D3 — unnamed spell groups.** Several category flags are raw byte offsets with no `SGRP_*` name: `9`, `27` (`1Bh`), `50` (`32h`), `72` (`48h`), `74` (`4Ah`). IDA-C mixes named constants and raw hex (e.g. `AI_OVL_Spell_Cats[9]`, `[0x4A]`). Keep them literal or add named constants to `enum e_AI_SPELL_GROUP`, but do not silently remap them.

## Sub-functions / external calls

- **`Get_Weighted_Choice(int16_t * weights, int16_t count)`** ([random.h:31](../../MoX/src/random.h#L31)) — weighted-random index over the 11 modifiers; also used for the target-wizard pick.
- **`Random(n)`** ([random.h](../../MoX/src/random.h)) — consumed once in the Disjunction `PLANAR_SEAL` branch.
- **`Player_Hero_Count(player_idx)`** — hero count for the summon weight.
- **`Player_Summon_City_Is_Fortress_City(player_idx)`** (asm `WIZ_SummonInFortress`, [AISPELL.c:1939](../../MoM/src/AISPELL.c#L1939)) — returns true iff the player's summon city is their fortress city. Verified 1:1 with `WIZ_SummonInFortress.asm`.
- **`AI_Has_More_Myrror_Cities(player_idx)`** (asm `AI_MyrrorAdvantage`, [AISPELL.c:1829](../../MoM/src/AISPELL.c#L1829)) — returns true iff the AI owns more Myrror cities than the human player. Verified 1:1 with `AI_MyrrorAdvantage.asm`.
- **Globals read:** `g_ai_spell_group_flags` (asm `AI_OVL_Spell_Cats`), `_players[]`, `_CITIES`/`_cities`, `_NODES`, `_UNITS`/`_units`, `_FORTRESSES`, `_landmasses`, `_unit_type_table`, `_turn`, `_difficulty`, `_num_players`, `AI_Own_Stacks`/`AI_Own_Stack_Count`, `AI_Enemy_Stacks`/`AI_Enemy_Stack_Count`, `AI_NodeOpportunity`, `AI_SCircle_Reevals` ([MOM_DAT.h:2916](../../MoX/src/MOM_DAT.h#L2916)), `TBL_AI_PRS_War_Mod` ([MOM_DAT.h:2529](../../MoX/src/MOM_DAT.h#L2529)), `_cp_hostile_opponents`/`_cp_hostile_opponent_count`.
- **Globals written:** `_players[player_idx].OVL_TargetWiz`, `AI_NodeOpportunity`.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_Select_Spell_Group.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\AI_Select_Spell_Group.c` — Piethawn IDA-C decompilation (cross-check only; see [divergences](#ida-c-decompilation-divergences)).
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the caller; maps this function's `0..10` return to the cast `switch`.
- [AISPELL-AI_Compute_Spells_Info.md](AISPELL-AI_Compute_Spells_Info.md) — produces the `g_ai_spell_group_flags[]` this picker reads.
- `AISPELL.h` — `enum e_AI_SPELL_GROUP` (`SGRP_*`).
- `random.h` — `Get_Weighted_Choice`, `Random`.
