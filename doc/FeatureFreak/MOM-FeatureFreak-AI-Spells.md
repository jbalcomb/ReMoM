# Master of Magic — FeatureFreak: AI Usage of Spells

> Companion to [#MOM-FeatureFreak.md](#MOM-FeatureFreak.md), [MOM-FeatureFreak-AI-Usage.md](MOM-FeatureFreak-AI-Usage.md), and [MOM-FeatureFreak-AI-Special-Abilities.md](MOM-FeatureFreak-AI-Special-Abilities.md). This doc walks how the AI decides which of the game's **214 spells** to cast — and, just as importantly, which it **never** casts and why. It asks per spell/spell-group: *does the AI cast it, how does it choose and target it, and what bugs affect that path?*

## Two headline findings (read these first)

1. **Overland spell casting is richly implemented.** The AI has a real selection pipeline (§A) with **8 group handlers** and **~33 `AITP_*` target pickers**, driven by each spell's `spell_data_table[].AI_Group` field (`SGRP_*`, values 0–90).

2. **Combat spell casting is a stub — the AI currently casts *no* combat spells.** The combined combat-spell target picker `AITP_CombatSpell__STUB` is reconstructed as `return ST_UNDEFINED;` ([Combat.c:11595-11600](../../MoM/src/Combat.c#L11595-L11600)). Since `ST_UNDEFINED` (−1) means "cannot cast," every combat-spell target request from the combat AI (`CMBTAI.c`) fails. The OG was a full picker that drake178 notes "contains numerous BUGs"; the ReMoM reconstruction has not yet ported it (§F). **This is a reconstruction gap (WIP/stub), not an original-game bug** — and the single biggest fact about AI spell usage today.

Consequently, "how the AI uses spells" is overwhelmingly a story about **overland** casting. Combat casting is documented as a known gap.

## The unit of analysis: `AI_Group` (`SGRP_*`), not 214 flat spells

The AI does not have per-spell code for all 214 spells. Every spell carries an **`AI_Group`** tag (`spell_data_table[spell_idx].AI_Group`), one of the `SGRP_*` values ([enum, MoM header](../../MoM/src)). The AI reasons at the granularity of these groups plus a set of per-spell **target pickers**. So this walkthrough is organized by:
- the **selection pipeline** (§A),
- the **group-availability filter** that decides which groups are even eligible overland (§B),
- the **8 overland group handlers** (§D),
- the **`AITP_*` target-picker catalog** (§E),
- **combat** casting (§F, the stub),
- **special/unique** spells (§G),
- and a consolidated **bug catalog** (§H).

Bug taxonomy: **OGBUG/B** = faithful original bug (preserved); **R** = reconstruction error to fix; **STUB/WIP** = not-yet-reconstructed. See [[project_aimove_walkthrough_workflow]].

---

## §A — The overland spell-selection pipeline

Driven from **`AI_Spell_Select`** ([AISPELL.c:327-396](../../MoM/src/AISPELL.c#L327-L396)), called from the per-wizard turn loop (`AI_Next_Turn`). Verified flow:

1. **`AI_Compute_Spells_Info(player_idx)`** ([AISPELL.c:910-974](../../MoM/src/AISPELL.c#L910-L974)) — scans the player's known spells and marks which **groups** are available overland, filtering out combat/utility groups (§B).
2. **`spell_category = AI_Select_Spell_Group(player_idx)`** ([AISPELL.c:457](../../MoM/src/AISPELL.c#L457)) — computes weighted scores for **11 categories** and returns one by weighted-random choice (§C).
3. **Dispatch by category** ([AISPELL.c:345-383](../../MoM/src/AISPELL.c#L345-L383)):

   | Cat | Meaning | Handler |
   | --- | --- | --- |
   | 0 | no spell | — (`spell_idx = 0`) |
   | 1 | Summon | `AI_Select_Spell_Group_Summon` |
   | 2 | Unit Enchantment | `AI_Select_Spell_Group_Unit_Enchantment` |
   | 3 | City Enchantment | `AI_Select_Spell_Group_City_Enchantment` |
   | 4 | Disenchant / True | `AI_Select_Spell_Group_Disenchant` |
   | 5 | Disjunction / True | `AI_Select_Spell_Group_Disjunction` |
   | 6 | Summoning Circle | direct `spl_Summoning_Circle` |
   | 7 | Overland curse/damage | `AI_Select_Spell_Group_Attack` |
   | 8 | Suppression global | `AI_Select_Spell_Group_Suppression` |
   | 9 | Overland enchantment | `AI_Select_Spell_Group_Global` |
   | 10 | Spell of Mastery | direct `spl_Spell_Of_Mastery` |

4. **Affordability gate** ([AISPELL.c:385-392](../../MoM/src/AISPELL.c#L385-L392)): `mana_per_turn = mana_reserve/10 + mana_income`; if `casting_cost/50 > mana_per_turn` the pick is cancelled (`spell_idx = 0`). I.e. the AI won't start a spell that would take more than ~50 turns of "perceived" mana to pay off.
5. **`Cast_Spell_Overland_Do(player_idx, spell_idx, …)`** ([AISPELL.c:394](../../MoM/src/AISPELL.c#L394)) — queues the chosen spell; per-spell **`AITP_*`** target pickers run at cast-resolution time (via `Cast_Spell_Overland`, see [MOM-FeatureFreak-AI-Usage.md](MOM-FeatureFreak-AI-Usage.md) §5).

Cross-referenced: [ComputerPlayer/AISPELL-AI_Spell_Select.md](../ComputerPlayer/AISPELL-AI_Spell_Select.md), [ComputerPlayer/AISPELL-AI_Select_Spell_Group.md](../ComputerPlayer/AISPELL-AI_Select_Spell_Group.md), [ComputerPlayer/AISPELL-AI_Compute_Spells_Info.md](../ComputerPlayer/AISPELL-AI_Compute_Spells_Info.md).

---

## §B — Group availability filter & overland blind spots

`AI_Compute_Spells_Info` walks the player's known spells and sets `g_ai_spell_group_flags[AI_Group] = ST_TRUE` — **except** for 21 groups it explicitly `continue`s past ([AISPELL.c:941-961](../../MoM/src/AISPELL.c#L941-L961)). These groups are therefore **never surfaced through the generic overland info list**:

| Filtered group(s) | Why (overland blind spot) |
| --- | --- |
| `SGRP_CombatHarm_1/2/3`, `SGRP_CombatCurse`, `SGRP_CombatMisc`, `SGRP_Disrupt` | Combat-only damage/curse spells — meaningless overland |
| `SGRP_CombatSummon_1/2/3`, `81` (`SGRP_IDK`) | Combat-only "phantom" summons |
| `SGRP_CE_1/2/3/4` | Combat (battlefield) enchantments |
| `SGRP_Haste`, `SGRP_AnimateDead` | Combat-only unit effects |
| `SGRP_Dispel`, `SGRP_DispelTrue`, `43` | Dispel handled via the dedicated Disenchant/Disjunction categories, not the generic list |
| `SGRP_SpellLock` | Combat-only |
| `SGRP_Scouting` | Not driven from this list |

**Implication:** the combat-tagged groups are correctly excluded from *overland* selection (they belong to the combat path — which is itself stubbed, §F). So a spell whose only `AI_Group` is a combat group is **cast by the AI in neither overland nor combat** today. This is the practical face of the combat stub: e.g. pure combat-damage spells (Fire Bolt, Ice Bolt in combat, etc.) currently see no AI casting at all.

*(Detailed enumeration of which specific spells fall into each filtered group is deferred to §E/§H as the group handlers are walked.)*

---

## §F — Combat spell casting (STUB — AI casts no combat spells)

The combat AI lives in **`CMBTAI.c`**; when the computer player takes its combat turn (`Auto_Cast_Spell_And_Do_Combat_Turn`), any spell it might cast is targeted through **`AITP_CombatSpell__STUB`**. Call sites: [CMBTAI.c:1211,1225,1238,1252,1272](../../MoM/src/CMBTAI.c#L1211) and [Combat.c:13054](../../MoM/src/Combat.c#L13054).

The current body ([Combat.c:11595-11600](../../MoM/src/Combat.c#L11595-L11600)):
```c
int16_t AITP_CombatSpell__STUB(int16_t spell_idx, int16_t player_idx, int16_t * Target_X, int16_t * Target_Y)
{
    return ST_UNDEFINED;
}
```
`ST_UNDEFINED` (−1) is the "spell cannot be cast" return, so **every combat-spell target request fails** and the combat AI casts nothing. The OG function (drake178: "combined AI target picker for combat spells … contains numerous BUGs") returned −1 / 99 (no target) / a battle-unit index, and set X/Y for square-targeted spells.

**Status:** reconstruction WIP/stub, not an OG bug. Until ported, the AI is a non-caster in tactical battles regardless of its spellbook — a significant combat-strength gap.

> **Scope note:** the *combat* spell-casting path (CMBTAI flow, the OG combined picker's per-spell logic, and its "numerous BUGs") is deferred to a **dedicated future doc**. This doc covers **overland** spell usage in full; §F records only the headline stub fact so the overland picture isn't misread as complete.

Cross-referenced: [Combat/](../Combat/), [ComputerPlayer/](../ComputerPlayer/).

---

## §C — Group weighting (`AI_Select_Spell_Group`, [AISPELL.c:457-880](../../MoM/src/AISPELL.c#L457-L880))

Computes a weight `modifiers[0..10]` for each of the 11 categories, then `Get_Weighted_Choice(&modifiers[0], 11)` ([AISPELL.c:879](../../MoM/src/AISPELL.c#L879)) returns one category by weighted random. Which categories are even *eligible* is gated on `g_ai_spell_group_flags[]` (set in §B). Verified weighting:

| Cat | Weight source (verified) |
| --- | --- |
| 0 no-spell | Set to **1** at the end ([877](../../MoM/src/AISPELL.c#L877)) — the always-present baseline. |
| 1 Summon | Base 100; +10×(hero deficit); ×1.5 with Conjurer/Channeler; ×5 if summon-city ≠ fortress-city ([503-560](../../MoM/src/AISPELL.c#L503-L560)). |
| 2 Unit ench. | MoveBuff groups → 0/100/200 by turn (<50 / <200 / else); AttackBuff/DefenseBuff/Heals add `(250 − turn)/2` mid-game ([767-797](../../MoM/src/AISPELL.c#L767-L797)). |
| 3 City ench. | WallsRituals → 50 (if human has contacted this AI) else 5; CityBuff/TerrainMod groups → 150 ([746-765](../../MoM/src/AISPELL.c#L746-L765)). |
| 4 Disenchant | +40 per hostile city-enchantment on your own cities (Chaos Rift, Evil Presence, Pestilence, …), +300 Flying Fortress, warped-node bonuses ([562-600](../../MoM/src/AISPELL.c#L562-L600)). |
| 5 Disjunction | Weighted by enemy global enchantments worth removing (Suppress Magic +2000, Zombie Mastery, Aura of Majesty, realm-supremacy globals, …) ([616-685](../../MoM/src/AISPELL.c#L616-L685)). |
| 6 Summon Circle | 100 if the relocation countdown has elapsed and the AI has ≥1 city ([860-875](../../MoM/src/AISPELL.c#L860-L875)). |
| 7 Attack/curse | `(Opponent_Weight + personality war-mod) / 4`, gated on owning an offensive group (Corruption/Famine/Raise Volcano/Destroy City/Earthquake/Time Stop/Death Wish/…) ([722-744](../../MoM/src/AISPELL.c#L722-L744)). |
| 8 Suppression | Anti-enemy-realm globals (Nature's Wrath, Suppress Magic, Life Force, Tranquility, Evil Omens) → +100 when the enemy has the countered realm ([801-843](../../MoM/src/AISPELL.c#L801-L843)). |
| 9 Global ench. | Per-known-global: Awareness +500, Wind Mastery +75, most others +100 ([799-845](../../MoM/src/AISPELL.c#L799-L845)); capped ([847](../../MoM/src/AISPELL.c#L847)). |
| 10 Spell of Mastery | 500 if known ([688-691](../../MoM/src/AISPELL.c#L688-L691)). |

**Opponent targeting.** Before scoring the offensive categories, the AI picks a target wizard `cp_target_3` by weighted choice over hostile opponents (weight from visible/hidden relations and diplomatic status), and **multiplies the human's weight by `(_difficulty + 1)`** ([712](../../MoM/src/AISPELL.c#L712)) — so at higher difficulty the AI targets the human player more.

**Mana gate.** If `mana_reserve < 200`, both the global (9) and suppression (8) weights are zeroed ([854-858](../../MoM/src/AISPELL.c#L854-L858)) — a poor AI won't attempt expensive global enchantments.

**Bugs / quirks.**
- **Hardcoded human-realm checks (OG quirk).** Several suppression/anti-realm globals evaluate **`_players[0]` (the human)** rather than the actual target wizard — Nature's Wrath ([804](../../MoM/src/AISPELL.c#L804)), Life Force ([829](../../MoM/src/AISPELL.c#L829)), Tranquility ([833](../../MoM/src/AISPELL.c#L833)), Evil Omens ([839](../../MoM/src/AISPELL.c#L839)); a source comment notes "Assembly checks against human player's realms (player 0)." In a multi-AI game the AI decides these anti-realm globals against the human specifically, not whichever rival is relevant. Faithful to disassembly.
- **`modifiers[9]` clamp is lopsided (OG quirk).** `if(modifiers[9] > 400) modifiers[9] = 300;` ([847](../../MoM/src/AISPELL.c#L847)) — values in (400,∞) snap to 300, but 301–400 pass through unclamped. Preserved.
- **`modifiers[0]` early-game weight overwritten.** An earlier `modifiers[0] = (50 − _turn)` ([482](../../MoM/src/AISPELL.c#L482)) is unconditionally overwritten by `modifiers[0] = 1` at the end ([877](../../MoM/src/AISPELL.c#L877)), so the intended early-game "prefer casting nothing" bias is discarded. Faithful to disassembly.

## §D — Overland group handlers

### §D.1 — Summon (category 1) · `AI_Select_Spell_Group_Summon` ([AISPELL.c:1013-1356](../../MoM/src/AISPELL.c#L1013-L1356))

The AI's most-used overland category. It builds a 50-slot weight array `AI_OVL_SplPriorities[]`, then `Get_Weighted_Choice` ([AISPELL.c:1305](../../MoM/src/AISPELL.c#L1305)) picks one and a switch maps the slot back to a `spl_*` id.

**Creature summons — weight = (casting_cost / 10)², then realm-suppression scaling.**
- Each known summon (War Bears … Demon Lord, slots 1–34) is seeded with `casting_cost/10` ([1030-1131](../../MoM/src/AISPELL.c#L1030-L1131)) then **squared** ([1134-1167](../../MoM/src/AISPELL.c#L1134-L1167)). Squaring is deliberate non-linear weighting: the AI strongly prefers the **most expensive (most powerful) summon it knows** — e.g. a 250-cost Sky Drake (25²=625) dwarfs a 30-cost War Bears (3²=9).
- If an enemy global suppresses a realm (`SPL_IsLifeSupressed` / `…Nat…` / `…Dth…` / `…Chs…`), the matching slots are down-weighted ([1257-1303](../../MoM/src/AISPELL.c#L1257-L1303)) — the AI backs off summoning in a suppressed realm.

**Utility / hero summons (fixed weights).**
- Resurrection (slot 35, weight 100) — only if the AI has a free hero slot *and* a dead hero ([1169-1175](../../MoM/src/AISPELL.c#L1169-L1175)).
- Guardian Spirit (36, 200) / Magic Spirit (37, 100) — only when `AI_NodeOpportunity` (a node worth melding exists) ([1179-1200](../../MoM/src/AISPELL.c#L1179-L1200)).
- Enchant Item (38, 20→30) / Create Artifact (39, 30→40) — Artificer raises these (cross-ref [special-abilities §17](MOM-FeatureFreak-AI-Special-Abilities.md#17--artificer-artificer)).
- Incarnation (40, 500) / Summon Champion (41, 400) / Summon Hero (42, 200) — gated on a free hero slot ([1235-1250](../../MoM/src/AISPELL.c#L1235-L1250)).
- Floating Island (slot 10) is additionally gated on transport count vs. turn ([1057-1061](../../MoM/src/AISPELL.c#L1057-L1061)); Cockatrices (43) uses cost/5 ([1252-1254](../../MoM/src/AISPELL.c#L1252-L1254)).

**AI usage.** Genuinely active and reasonably strategic: cost-squared weighting favors power, realm-suppression awareness, node-opportunity gating for the spirit summons, and hero-slot gating for hero summons. Conjurer/Channeler separately raise the *group's* selection odds (cross-ref [special-abilities §8/§15](MOM-FeatureFreak-AI-Special-Abilities.md#8--channeler-channeler)).

**Bugs.**
- **OGBUG (compound) — the AI never casts Incarnation.** Two defects combine: (1) `have_the_chosen` is set only if the AI owns `ut_BarbSpearmen` instead of *The Chosen* ([AISPELL.c:1229](../../MoM/src/AISPELL.c#L1229), flagged `/* OGBUG should be The Chosen, Barbarian Spearmen */`); (2) the Incarnation weight is set only when `have_the_chosen == ST_TRUE` but the comment notes the test is inverted — it should be `!= ST_TRUE` ([AISPELL.c:1237-1238](../../MoM/src/AISPELL.c#L1237-L1238)). Net: Incarnation's weight 500 is only ever set if the AI happens to own Barbarian Spearmen — and even then it's the opposite of the intent (you want Incarnation when you *don't* already have Torin). Effectively **Incarnation is never a sensible AI pick.** Preserved as OG.
- **OG quirk — Storm Giant (slot 11) and slot 30 are not squared** ([AISPELL.c:1144](../../MoM/src/AISPELL.c#L1144), [1163](../../MoM/src/AISPELL.c#L1163)), so Storm Giant is weighted linearly and thus under-represented relative to comparably-costed summons. Faithful to disassembly.
- **Redundant mapping** — switch cases 25 and 26 both return `spl_Skeletons`, and case 29 returns `0` (the skipped slot 30) ([AISPELL.c:1334-1338](../../MoM/src/AISPELL.c#L1334-L1338)); benign artifacts of the slot layout.

Cross-referenced: [ComputerPlayer/AISPELL-AI_Select_Spell_Group_Summon.md](../ComputerPlayer/AISPELL-AI_Select_Spell_Group_Summon.md).

### §D.2 — Attack / overland curse (category 7) · `AI_Select_Spell_Group_Attack` ([AISPELL.c:2026-2250](../../MoM/src/AISPELL.c#L2026-L2250))

Weights 20 known damage/curse spells and picks one by `Get_Weighted_Choice`. **Weight = `casting_cost / N`** (linear, *not* squared like summons), with a per-spell divisor that hand-tunes relative preference: cheap-to-favor spells use `/2` (Spell Blast, Call The Void, Pestilence), workhorses `/5` (Stasis, Corruption, Famine, Drain Power, Evil Presence, Cursed Lands, Cruel Unminding) or `/3` (Chaos Rift, Warp Node), and area spells `/10` (Ice Storm, Earthquake, Fire Storm, Black Wind, Subversion) ([2040-2158](../../MoM/src/AISPELL.c#L2040-L2158)).

**Gating (all verified):**
- **Time Stop** — only if `mana_reserve ≥ 3000` ([2065-2071](../../MoM/src/AISPELL.c#L2065-L2071)).
- **Raise Volcano** — only if the AI is not already running Armageddon ([2088-2091](../../MoM/src/AISPELL.c#L2088-L2091)).
- **No enemy city → no city-curses.** If `AI_Has_Targetable_City == 0`, the ten city-targeting curses (Evil Presence, Cursed Lands, Pestilence, Famine, Earthquake, Chaos Rift, Corruption, Raise Volcano, Call The Void, Warp Node) are zeroed ([2160-2173](../../MoM/src/AISPELL.c#L2160-L2173)) — the AI won't pick a city-curse it can't aim.
- **Realm suppression** — Nature/Death/Chaos-suppressed weights are scaled down ([2176-2220](../../MoM/src/AISPELL.c#L2176-L2220)).

**AI usage.** Active and target-aware. Note the actual *square* target is chosen later by the per-spell `AITP_*` pickers (§E) — this handler only decides *which* curse.
**Bugs.** None flagged at these sites; the missing switch `case 9` ([2226-2248](../../MoM/src/AISPELL.c#L2226-L2248)) is a benign slot-layout gap.
Cross-referenced: [ComputerPlayer/AISPELL-AI_Select_Spell_Group_Attack.md](../ComputerPlayer/AISPELL-AI_Select_Spell_Group_Attack.md).

### §D.3 — Unit Enchantment (category 2) · `AI_Select_Spell_Group_Unit_Enchantment` ([AISPELL.c:1360-1651](../../MoM/src/AISPELL.c#L1360-L1651))

Weights ~40 unit-buff spells with **fixed hand-tuned weights** (not cost-derived), picks by `Get_Weighted_Choice`, then maps back to a `spl_*`. The weighting reveals the AI's overland priorities: **strategic mobility ranks highest** — Wind Walking 50, Invisibility 40, Iron Skin / Flame Blade / Lionheart / Water Walking 30, Flight 25 — while minor combat buffs sit at 5–15 ([1373-1493](../../MoM/src/AISPELL.c#L1373-L1493)). A few are seeded to weight **0** (Blur slot 7, Entangle slot 26) so they're known but never picked.

**Gating.** Chaos Channels only if not already running Doom Mastery ([1477-1480](../../MoM/src/AISPELL.c#L1477-L1480)); Lycanthropy only if not Zombie Mastery ([1481-1484](../../MoM/src/AISPELL.c#L1481-L1484)); realm-suppression scaling as elsewhere ([1495-1561](../../MoM/src/AISPELL.c#L1495-L1561)).

**AI usage.** Active; the target unit is chosen later by `AITP_Unit_Enchantment` (§E).
**Bugs.**
- **OGBUG — Plane Shift planar-seal check is hardcoded to 4 players** ([AISPELL.c:1454-1461](../../MoM/src/AISPELL.c#L1454-L1461), flagged `/* OGBUG: fails to check all wizards, should iter over _num_players */`): it tests `_players[0..3].Globals[PLANAR_SEAL]` literally instead of looping `_num_players`, so a 5th wizard's Planar Seal is ignored when deciding whether Plane Shift is worth weighting. Preserved.

### §D.4 — City Enchantment (category 3) · `AI_Select_Spell_Group_City_Enchantment` ([AISPELL.c:2304-2527](../../MoM/src/AISPELL.c#L2304-L2527))

The most target-coupled handler: for each of 19 city-enchantment spells it **calls the spell's `AITP_*` picker inline** and only assigns a weight if the picker reports a valid target ([2322-2434](../../MoM/src/AISPELL.c#L2322-L2434)). So a city enchantment is only ever weighted when the AI actually has somewhere to put it (the picker internals are §E).

**Weighting shape:** economy/growth enchantments get fixed high weights (Gaias Blessing 300, Inspirations 300, Prosperity 300, Transmute 200); defensive/utility ones are **turn-scaled** so they matter more later (Wall of Stone `_turn/6`, Stream of Life `_turn/10`, Move Fortress / Earth Gate `_turn/15`, Wall of Fire / Cloud of Shadow / Wall of Darkness `_turn/20`, Heavenly Light `_turn/25`) ([2324-2432](../../MoM/src/AISPELL.c#L2324-L2432)). Realm-suppression scaling as elsewhere.

**AI usage.** Active and genuinely target-aware — the inline `AITP_*` calls make this the best-integrated of the handlers.
**Bugs / quirks.**
- **Astral Gate is dead-weighted.** Even when `AITP_Astral_Gate` finds a valid target, its weight is set to **0** ([AISPELL.c:2396](../../MoM/src/AISPELL.c#L2396)), so the AI **never casts Astral Gate through this path**. Faithful to disassembly; the effect is a permanent zero.
- Switch `case 4` is a slot gap ([2483-2524](../../MoM/src/AISPELL.c#L2483-L2524)), benign.

*(The realm-suppression predicates — `SPL_IsLifeSupressed`/`…Chs…`/`…Dth…`/`CRP_SPL_IsNatSuppressed`, [AISPELL.c:2531-2590](../../MoM/src/AISPELL.c#L2531-L2590) — each scan all players for the relevant global: Evil Omens ⇒ Life/Nature suppressed, Nature's Wrath ⇒ Chaos suppressed, Life Force/Nature's Wrath ⇒ Death suppressed. Used by every handler above.)*

### §D.5 — Global Enchantment (category 9) · `AI_Select_Spell_Group_Global` ([AISPELL.c:1744-1962](../../MoM/src/AISPELL.c#L1744-L1962))

Weights 20 global enchantments at **`casting_cost / 10`**, each gated on the AI **not already having that global active** (`Globals[X] == 0`) ([1758-1858](../../MoM/src/AISPELL.c#L1758-L1858)). Two special gates: **Planar Seal** is weighted only if the AI holds more Myrror cities (`AI_Has_More_Myrror_Cities`) and at the lower `cost/20` ([1818-1823](../../MoM/src/AISPELL.c#L1818-L1823)); one-shot globals (Great Unsummoning, Spell Binding, Death Wish) aren't gated on an active-flag since they don't persist. Realm-suppression scaling as elsewhere. Note category 9's weight is itself gated upstream by mana (§C: zeroed if `mana_reserve < 200`), so a poor AI never reaches this handler.

**AI usage.** Active; sensible "don't re-cast an active global" gating and cost-proportional weighting. **Bugs.** None flagged at these sites.

### §D.6 — Suppression, Disenchant, Disjunction (categories 8, 4, 5)

- **Suppression** (`AI_Select_Spell_Group_Suppression`, [AISPELL.c:1654-1741](../../MoM/src/AISPELL.c#L1654-L1741)) — weights the four anti-realm globals (Nature's Wrath, Life Force, Tranquility, Evil Omens) at `cost/10` with suppression scaling. Simple; no bugs. The *target realm/wizard* is chosen later (Evil Omens/Nature's Wrath are global; the §C weighting already decided they're worth casting based on the enemy's realms — see the hardcoded `_players[0]` quirk in §C).
- **Disenchant** (`AI_Select_Spell_Group_Disenchant`, [AISPELL.c:2623-2641](../../MoM/src/AISPELL.c#L2623-L2641)) — **no weighting**: returns `spl_Disenchant_True` if known, else `spl_Disenchant_Area`, else none. The target square/enchantment is chosen by `AITP_Disenchant` (§E).
- **Disjunction** (`AI_Select_Spell_Group_Disjunction`, [AISPELL.c:2673-2691](../../MoM/src/AISPELL.c#L2673-L2691)) — same shape: `spl_Disjunction_True` preferred, else `spl_Disjunction`. Target enemy global chosen by `AITP_Disjunction` (§E).

Both dispel handlers are deterministic "prefer the stronger variant" pickers; all the intelligence is in the §C decision to enter the category (which weighs how many enemy enchantments are worth removing) and in the §E target pickers. **Bugs.** None at these handler sites.

## §E — AITP target-picker catalog

Once a handler (§D) chooses *which* spell, an **`AITP_*`** picker chooses *where/whom*. The City-Enchantment handler calls its pickers **inline** to gate weighting (§D.4); the others run at cast-resolution (via `Cast_Spell_Overland`). All return `ST_TRUE` + write target coords/index, or `ST_FALSE` = no target. ~33 pickers exist ([defs across AISPELL.c:2732-6400](../../MoM/src/AISPELL.c#L2732-L6400)).

**Target-rule summary (grouped):**

| Picker(s) | Spell(s) | Target rule |
| --- | --- | --- |
| `AITP_Wall_Of_Stone`, `AITP_Wall_Of_Fire`, `AITP_Wall_Of_Darkness`, `AITP_Gaias_Blessing`, `AITP_Heavenly_Light`, `AITP_Stream_Of_Life`, `AITP_Inspirations`, `AITP_Prosperity`, `AITP_Consecration`, `AITP_Dark_Rituals`, `AITP_Cloud_Of_Shadow`, `AITP_Altar_Of_Battle`, `AITP_Flying_Fortress` | city enchantments | highest-value **own city** lacking the enchantment (from `_ai_all_own_city_values[]`) |
| `AITP_Move_Fortress`, `AITP_Earth_Gate`, `AITP_Astral_Gate`, `AITP_Summoning_Circle` | fortress/gate relocation | best own city per spell-specific criteria |
| `AITP_Transmute`, `AITP_Change_Terrain`, `AITP_Enchant_Road`, `AITP_Attack_Terrain`, `AITP_Corruption`, `AITP_Floating_Island`, `AITP_Natures_Cures` | terrain squares | scan own city catchments / target squares for the best square |
| `AITP_Unit_Enchantment`, `AITP_Attack_Stack` | unit buffs / stack attacks | best own unit (enchant) or best enemy stack (attack) |
| `AITP_Node` | Warp Node / node spells | a node meeting the spell's condition |
| `AITP_Spell_Ward`, `AITP_Spell_Ward_Wrapper`, `AITP_City_Enchantment` | ward / generic CE | city + realm selection |
| `AITP_Disenchant`, `AITP_Disjunction`, `AITP_Attack_Wizard`, `AITP_Plane_Shift` | dispel / anti-wizard / planar | enemy enchantment, target wizard, or destination square |

**AI usage.** Generally sensible heuristics (highest-value city, highest-cost unit, best node) — but the pickers are the **buggiest** part of the overland spell AI. Because a picker returning `ST_TRUE` with a bad target still lets the cast proceed, several of these bugs cause the AI to cast at the wrong place rather than to skip.

### §E.1 — AITP OGBUG cluster (all preserved OG behavior)

The overland pickers carry ~20 flagged original bugs — the overland counterpart to the combat picker's "numerous BUGs" (§F). Highlights, by severity:

| Severity | Picker | Bug | Site | Effect |
| --- | --- | --- | --- | --- |
| **High** | `AITP_Disenchant` | Reads `_UNITS[target_city_idx]` where `target_city_idx == ST_UNDEFINED` — **OOB read of `_UNITS[-1]`** — instead of `highest_cost_unit_idx` | [5735-5740](../../MoM/src/AISPELL.c#L5735-L5740) | Returns garbage target coords but `ST_TRUE`, so overland Disenchant aims at junk |
| **High** | `AITP_Attack_Wizard` | Targets `_cp_hostile_opponents[0]` instead of `HUMAN_PLAYER_IDX` | [6194](../../MoM/src/AISPELL.c#L6194) | Anti-wizard spell can hit the wrong wizard |
| **High** | `AITP_Node` | Warp-Node condition `(flags & NF_WARPED) == NF_WARPED` is inverted (should be `!=`); and target owner `Random(_cp_hostile_opponent_count)` can select self/non-hostile | [4730](../../MoM/src/AISPELL.c#L4730), [4718](../../MoM/src/AISPELL.c#L4718) | Warp Node targets already-warped nodes / wrong owner |
| **High** | `AITP_Plane_Shift` | Settler condition inverted (only considers Settlers instead of excluding them); missing MYRROR→ARCANUS return block; doesn't test square occupiable | [6083](../../MoM/src/AISPELL.c#L6083), [6120](../../MoM/src/AISPELL.c#L6120), [6101](../../MoM/src/AISPELL.c#L6101) | Plane Shift picks the wrong unit / can strand on Myrror |
| Med | `AITP_Attack_Terrain`, `AITP_Corruption` | Returns `city_wp` instead of `target_wp` (last-city plane) | [5888](../../MoM/src/AISPELL.c#L5888), [6057](../../MoM/src/AISPELL.c#L6057) | Wrong plane for the target square |
| Med | `AITP_Unit_Enchantment` | Possible OOB `_ai_all_own_stacks[-1]`; unit-type test `<= ut_BarbSwordsmen` (should be a `ut_Warship…ut_BarbSwordsmen` range) | [4590](../../MoM/src/AISPELL.c#L4590), [4633](../../MoM/src/AISPELL.c#L4633) | Mis-selects target unit |
| Med | `AITP_Enchant_Road` | Doesn't add catchment offsets — only checks the city-center square | [5592](../../MoM/src/AISPELL.c#L5592) | Under-scans for road targets |
| Med | `AITP_Disjunction` | Ignores invisibility and unit sight range (uses fixed 2) | [5338-5339](../../MoM/src/AISPELL.c#L5338-L5339) | Mis-scores enemy enchantments |
| Low | `AITP_Transmute` | `GET_TERRAIN_SPECIAL(...) & TS_COAL/IRON/SILVER` treats non-bitflag specials as bitflags; also treats catchment corners as in-area | [2863-2867](../../MoM/src/AISPELL.c#L2863-L2867) | May mis-detect transmutable terrain |
| Low | `AITP_City_Enchantment` | Chaos Rift also removable by Consecration not accounted for | [4249](../../MoM/src/AISPELL.c#L4249) | Minor mis-scoring |

All are **OGBUG** (faithful to disassembly) — do **not** fix in the baseline. They collectively mean the AI's overland spell *targeting* is materially less reliable than its spell *selection* (§C/§D), which is comparatively clean.

## §G — Special / unique spells

Three spells bypass the group handlers or have bespoke lifecycle handling.

### Spell of Mastery (category 10)
- **Selection:** weighted at **500** when known ([AISPELL.c:688-691](../../MoM/src/AISPELL.c#L688-L691)) and dispatched directly as `spl_Spell_Of_Mastery` ([378](../../MoM/src/AISPELL.c#L378)); research uses a dedicated `som_research_cost` ([279](../../MoM/src/AISPELL.c#L279)). No target picker (global effect).
- **AI reaction:** other AIs and the raider/monster system react to a wizard casting it ([AIDATA.c:952](../../MoM/src/AIDATA.c#L952), [1085](../../MoM/src/AIDATA.c#L1085), [1115](../../MoM/src/AIDATA.c#L1115)).
- **Bugs (both OGBUG, both `spl_Fire_Elemental` typos):**
  - [AIDUDES.c:742-743](../../MoM/src/AIDUDES.c#L742-L743): the AI's "am I researching the Spell of Mastery" check tests `researching_spell_idx == spl_Fire_Elemental` instead of `spl_Spell_Of_Mastery`.
  - [AIMOVE.c:7319-7321](../../MoM/src/AIMOVE.c#L7319-L7321): the loop that finds a hostile wizard casting the Spell of Mastery also checks `spl_Fire_Elemental` (plus a second flagged OGBUG: it fails to exclude self). **Impact:** the AI does not correctly detect an enemy's Spell-of-Mastery cast, so it won't rally to stop the impending winner — a real endgame weakness. Preserved as OG.

### Spell of Return (banishment recovery)
When a wizard is banished, `casting_spell_idx` is set to `spl_Spell_Of_Return` (by the shared banishment path). The AI turn loop **keeps processing a returning wizard** rather than skipping it ([AIDUDES.c:228](../../MoM/src/AIDUDES.c#L228), [340](../../MoM/src/AIDUDES.c#L340)), and `AI_Update_Gold_And_Mana_Reserves` dumps its gold into mana to fund the re-cast ([AIDUDES.c:1711-1722](../../MoM/src/AIDUDES.c#L1711-L1722) — see the Alchemy `=`/`+=` OGBUG in [special-abilities §0](MOM-FeatureFreak-AI-Special-Abilities.md#0--alchemy-alchemy)). It is not chosen through the §C category system — it's a forced state, not a discretionary cast.

### Summoning Circle (category 6)
- **Selection:** weight 100 when the relocation countdown has elapsed and the AI owns ≥1 city ([AISPELL.c:860-875](../../MoM/src/AISPELL.c#L860-L875)); dispatched directly as `spl_Summoning_Circle` ([366](../../MoM/src/AISPELL.c#L366)).
- **Cadence:** `_ai_reevaluate_summoning_circle_countdown` is initialized to **80 + Random(40) = [80,119]** turns ([INITGAME.c:775](../../MoM/src/INITGAME.c#L775)) and decremented once per AI turn ([AIDUDES.c:237](../../MoM/src/AIDUDES.c#L237)) — so the AI reconsiders where its summons appear only every ~80–119 turns.
- **Target:** `AITP_Summoning_Circle` ([AISPELL.c:6339](../../MoM/src/AISPELL.c#L6339)) picks the destination city. No bugs flagged at this picker.

## §H — Consolidated bug catalog

Every spell-AI defect found in this walkthrough. **Type:** `STUB/WIP` = not-yet-reconstructed (fix by porting); `OGBUG` = faithful original bug (do **not** fix in baseline). Ordered by impact.

| # | Type | Where | Effect | § |
| --- | --- | --- | --- | --- |
| 1 | **STUB/WIP** | `AITP_CombatSpell__STUB` [Combat.c:11595](../../MoM/src/Combat.c#L11595) | **AI casts no combat spells at all** — biggest single gap | §F |
| 2 | OGBUG | Spell-of-Mastery detection tests `spl_Fire_Elemental` [AIDUDES.c:742](../../MoM/src/AIDUDES.c#L742), [AIMOVE.c:7321](../../MoM/src/AIMOVE.c#L7321) | AI fails to detect an enemy casting Spell of Mastery → won't rush to stop the winner | §G |
| 3 | OGBUG | `AITP_Disenchant` OOB `_UNITS[-1]` [AISPELL.c:5735](../../MoM/src/AISPELL.c#L5735) | Overland Disenchant targets garbage coordinates | §E.1 |
| 4 | OGBUG | `AITP_Attack_Wizard` targets `_cp_hostile_opponents[0]` [AISPELL.c:6194](../../MoM/src/AISPELL.c#L6194) | Anti-wizard spell can hit the wrong wizard | §E.1 |
| 5 | OGBUG | `AITP_Node` inverted `NF_WARPED` + random owner [AISPELL.c:4730](../../MoM/src/AISPELL.c#L4730),[4718](../../MoM/src/AISPELL.c#L4718) | Warp Node aims at already-warped nodes / wrong owner | §E.1 |
| 6 | OGBUG | `AITP_Plane_Shift` inverted Settler test, missing Myrror→Arcanus, no occupiable check [AISPELL.c:6083](../../MoM/src/AISPELL.c#L6083),[6120](../../MoM/src/AISPELL.c#L6120) | Wrong unit / can strand on Myrror | §E.1 |
| 7 | OGBUG | Incarnation gate: wrong unit (`ut_BarbSpearmen`) + inverted test [AISPELL.c:1229](../../MoM/src/AISPELL.c#L1229),[1238](../../MoM/src/AISPELL.c#L1238) | AI never sensibly summons Incarnation (Torin) | §D.1 |
| 8 | OGBUG | `AITP_Attack_Terrain` / `AITP_Corruption` return `city_wp` [AISPELL.c:5888](../../MoM/src/AISPELL.c#L5888),[6057](../../MoM/src/AISPELL.c#L6057) | Wrong plane for target square | §E.1 |
| 9 | OGBUG | `AITP_Unit_Enchantment` possible OOB + unit-type test [AISPELL.c:4590](../../MoM/src/AISPELL.c#L4590),[4633](../../MoM/src/AISPELL.c#L4633) | Mis-selects buff target | §E.1 |
| 10 | OGBUG | `AITP_Enchant_Road` scans only city center [AISPELL.c:5592](../../MoM/src/AISPELL.c#L5592) | Under-finds road targets | §E.1 |
| 11 | OGBUG | `AITP_Disjunction` ignores invisibility / sight [AISPELL.c:5338](../../MoM/src/AISPELL.c#L5338) | Mis-scores enemy enchantments | §E.1 |
| 12 | OGBUG | Anti-realm globals hardcode `_players[0]` (human) [AISPELL.c:804](../../MoM/src/AISPELL.c#L804),[829](../../MoM/src/AISPELL.c#L829),[833](../../MoM/src/AISPELL.c#L833),[839](../../MoM/src/AISPELL.c#L839) | AI weighs Nature's Wrath / Life Force / etc. vs. the human specifically, not the actual rival | §C |
| 13 | OGBUG | Unit-ench Plane Shift planar-seal check hardcodes 4 players [AISPELL.c:1454](../../MoM/src/AISPELL.c#L1454) | Ignores a 5th wizard's Planar Seal | §D.3 |
| 14 | OGBUG | City-ench **Astral Gate weight forced to 0** [AISPELL.c:2396](../../MoM/src/AISPELL.c#L2396) | AI never casts Astral Gate via this path | §D.4 |
| 15 | OGBUG | `AITP_Transmute` bitflag misuse + corner squares [AISPELL.c:2863](../../MoM/src/AISPELL.c#L2863) | May mis-detect transmutable terrain | §E.1 |
| 16 | OGBUG | `AITP_City_Enchantment` Chaos Rift/Consecration overlap [AISPELL.c:4249](../../MoM/src/AISPELL.c#L4249) | Minor mis-scoring | §E.1 |
| 17 | OGBUG (quirk) | `modifiers[9]` lopsided clamp; `modifiers[0]` overwrite; Storm-Giant/slot-30 not squared [AISPELL.c:847](../../MoM/src/AISPELL.c#L847),[877](../../MoM/src/AISPELL.c#L877),[1144](../../MoM/src/AISPELL.c#L1144) | Minor weighting distortions | §C, §D.1 |

**Shape of the picture:** the AI's spell **selection** (§C/§D) is comparatively clean and strategic; the damage concentrates in (a) the **combat stub** (#1, a reconstruction gap) and (b) the **`AITP_*` targeting layer** (#3–#11, #15–#16, preserved OG bugs). So the AI usually picks a *reasonable spell* but frequently *aims it poorly* — and never casts in tactical combat at all.

## Cross-references

- Companions: [#MOM-FeatureFreak.md](#MOM-FeatureFreak.md) (overview), [MOM-FeatureFreak-AI-Usage.md](MOM-FeatureFreak-AI-Usage.md) §5 (spell selection in the turn loop), [MOM-FeatureFreak-AI-Special-Abilities.md](MOM-FeatureFreak-AI-Special-Abilities.md) (Conjurer/Channeler/Artificer/Alchemy interactions).
- AI spell walkthroughs: [ComputerPlayer/](../ComputerPlayer/) — the `AISPELL-AI_Select_Spell_Group_*.md` and `AISPELL-AITP_*.md` doc set.
- Source: `MoM/src/AISPELL.c` (selection + overland pickers), `MoM/src/Combat.c` + `MoM/src/CMBTAI.c` (combat, stubbed).

---

*Status: **overland** spell usage walked in full (pipeline §A, filter §B, weighting §C, all 8 handlers §D, AITP catalog + bug cluster §E, specials §G, consolidated bugs §H). **Combat** spell casting (§F) is recorded as a stub with the deep-dive deferred to a dedicated future doc. Every cited file:line grep/Read-verified.*

<!-- SECTIONS FILLED IN PER THE SESSION TODO LIST -->

---

*Status: scaffold + verified foundations (pipeline §A, group filter §B, combat stub §F). Remaining group-handler / AITP / specials / bug-catalog sections tracked in the session todo list. Every cited file:line grep/Read-verified.*
