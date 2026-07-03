# Master of Magic — FeatureFreak: AI Usage of Wizard Special Abilities (Retorts)

> Companion to [#MOM-FeatureFreak.md](#MOM-FeatureFreak.md) and [MOM-FeatureFreak-AI-Usage.md](MOM-FeatureFreak-AI-Usage.md). This doc walks each of the **18 Wizard Special Abilities** ("retorts") and asks: **does the AI make an explicit decision keyed on this retort, or does it only benefit passively through shared game rules — and what bugs affect either path?**

## The central distinction: active AI logic vs. passive shared benefit

Retort effects are **not** implemented in the AI. They live in **shared game/engine code** that runs identically for the human and every AI wizard — city output ([CITYCALC.c](../../MoM/src/CITYCALC.c)), turn processing ([NEXTTURN.c](../../MoM/src/NEXTTURN.c)), combat ([Combat.c](../../MoM/src/Combat.c)), overland spell costs ([OverSpel.c](../../MoM/src/OverSpel.c)), map generation ([MAPGEN.c](../../MoM/src/MAPGEN.c)). So for **most** retorts the AI "uses" the ability only in the sense that the same rule-effect is applied to its empire — there is no AI-side reasoning about it.

Only **four** retorts are referenced in AI-specific decision code *(verified: grep of the `.<field>` accessor across `MoM/src/*.c`)*:

| Retort | AI-specific decision code | What the AI does with it |
| --- | --- | --- |
| **Alchemy** | [AIDUDES.c](../../MoM/src/AIDUDES.c) | Biases power-investment toward mana; rebalances gold/mana pool every turn (lossless) to a turn-dependent target ratio — see [§0](#0--alchemy-alchemy) |
| **Channeler** | [AISPELL.c](../../MoM/src/AISPELL.c) | Biases the Summoning spell group ×1.5 (shared site with Conjurer) — see [§8](#8--channeler-channeler) |
| **Conjurer** | [AISPELL.c](../../MoM/src/AISPELL.c) | Biases the Summoning spell group ×1.5 (shared site with Channeler) — see [§15](#15--conjurer-conjurer) |
| **Artificer** | [AISPELL.c](../../MoM/src/AISPELL.c) | Raises Enchant Item / Create Artifact cast priorities — see [§17](#17--artificer-artificer) |

Every other retort is **passive-only** from the AI's perspective: the AI never checks whether it has the retort; it simply receives the shared rule effect. That does **not** mean the AI uses them *well* — a passive economic bonus (e.g. Mana Focusing) helps the AI's numbers, but a retort whose value depends on *deliberate play* (e.g. Alchemy's free gold↔mana conversion, or Runemaster's dispel strength) is only as useful as the AI's generic logic that happens to touch it.

## Data model

Retorts are stored as **named `int8_t` fields** in the player struct, offsets 0x64–0x75 *(verified: [MOM_DAT.h:1467-1484](../../MoX/src/MOM_DAT.h#L1467-L1484))* — `alchemy`, `warlord`, `chaos_mastery`, `nature_mastery`, `sorcery_mastery`, `infernal_power`, `divine_power`, `sage_master`, `channeler`, `myrran`, `archmage`, `mana_focusing`, `node_mastery`, `famous`, `runemaster`, `conjurer`, `charismatic`, `artificer`. The `wsa_*` enum ([NewGame.h:69-91](../../MoM/src/NewGame.h#L69-L91)) is used only during **wizard creation/init** ([NewGame.c](../../MoM/src/NewGame.c), [INITGAME.c](../../MoM/src/INITGAME.c)); runtime effect code reads the named fields directly.

## Methodology (per retort)

Each section below records:
1. **In-game effect** — what the retort does.
2. **Where implemented** — the shared-code call sites (verified file:line).
3. **AI usage** — *active* (explicit AI decision) or *passive* (shared effect only); if active, the exact logic.
4. **Bugs** — any reconstruction-error (R) or original-game (OGBUG/B) issues at the call sites, and how they change the retort's real behavior for the AI.

Bug taxonomy follows the project convention: **OGBUG/B** = faithful-to-disassembly original bug (preserved), **R** = reconstruction error to fix. See [[project_aimove_walkthrough_workflow]].

---

## Master table — all 18 retorts

| # | Retort | Field | In-game effect (summary) | AI usage | Impl. files (verified by grep) |
| --- | --- | --- | --- | --- | --- |
| 0 | Alchemy | `alchemy` | Free gold↔mana conversion; +1 to-hit on built weapons | **active** (AIDUDES) | AIDUDES, MagicScr, Lair, NEXTTURN, MAPGEN, WIZVIEW |
| 1 | Warlord | `warlord` | +1 unit experience level | passive | CITYCALC, MAPGEN |
| 2 | Chaos Mastery | `chaos_mastery` | Cheaper/stronger Chaos spells | passive | CITYCALC, NEXTTURN, OverSpel |
| 3 | Nature Mastery | `nature_mastery` | Cheaper/stronger Nature spells | passive | CITYCALC, NEXTTURN, OverSpel |
| 4 | Sorcery Mastery | `sorcery_mastery` | Cheaper/stronger Sorcery spells | passive | CITYCALC, NEXTTURN, OverSpel |
| 5 | Infernal Power | `infernal_power` | +Power from Death religious buildings | passive | CITYCALC |
| 6 | Divine Power | `divine_power` | +Power from Life religious buildings | passive | CITYCALC |
| 7 | Sage Master | `sage_master` | +Research | passive | CITYCALC |
| 8 | Channeler | `channeler` | No spell upkeep; faster combat casting | **active** (AISPELL) | AISPELL, CITYCALC, Combat, NEXTTURN |
| 9 | Myrran | `myrran` | Start on Myrror | passive (start-time) | MAPGEN |
| 10 | Archmage | `archmage` | +10 casting skill; stronger dispel/resist | passive | NEXTTURN, OverSpel, Spells129 |
| 11 | Mana Focusing | `mana_focusing` | +25% Power to mana | passive | CITYCALC |
| 12 | Node Mastery | `node_mastery` | Control all node auras; +node power | passive | CITYCALC, Combat |
| 13 | Famous | `famous` | +Fame/turn; better offers | passive | Combat, NEXTTURN |
| 14 | Runemaster | `runemaster` | Cheaper Arcane; stronger dispel/resist | passive | CITYCALC, Combat, NEXTTURN, OverSpel |
| 15 | Conjurer | `conjurer` | Cheaper/stronger summoned creatures | **active** (AISPELL) | AISPELL, CITYCALC, NEXTTURN |
| 16 | Charismatic | `charismatic` | Better diplomacy; cheaper hero hire | passive | DIPLOMAC, HIRE, CITYCALC, NEXTTURN |
| 17 | Artificer | `artificer` | Half-price item creation; no level cap | **active** (AISPELL) | AISPELL, NEXTTURN |

*(Effect summaries are from MoM canon; each is verified against the implementation in the per-retort sections below. "AI usage" and impl. files are grep-verified.)*

---

## Per-retort walkthroughs

*Sections filled in as the walkthrough progresses (see the session todo list). Order follows the `wsa_*` enum index.*

### 0 — Alchemy (`alchemy`)  · **ACTIVE**

**In-game effect.** (1) Gold↔Mana conversion is **1:1 and lossless** instead of the normal 2:1; (2) every non-fantastic unit built in your cities is created with a **Magic weapon** (+1 to-hit).

**Where implemented.**
- Conversion (AI): [AIDUDES.c:1706-1804](../../MoM/src/AIDUDES.c#L1706-L1804) (`AI_Update_Gold_And_Mana_Reserves`) and the magic-investment nudge at [AIDUDES.c:658-665](../../MoM/src/AIDUDES.c#L658-L665) (`AI_Update_Magic_Power`).
- Conversion (human UI): [MagicScr.c:1648](../../MoM/src/MagicScr.c#L1648) (`_human_player_idx` — not AI).
- Magic weapon on built units (shared): [NEXTTURN.c:1069-1075](../../MoM/src/NEXTTURN.c#L1069-L1075) — `mutations = wq_Magic` for any owner with alchemy.

**AI usage — active, three sites:**
1. **Magic-investment bias** ([AIDUDES.c:658-665](../../MoM/src/AIDUDES.c#L658-L665)): if the AI has Alchemy, a `Random(3)==1` (⅓) chance per evaluation steers its power-investment type to **Mana**. Rationale encoded in the code: with lossless conversion, mana is the more flexible reserve, so the AI leans toward banking it.
2. **Gold/mana reserve rebalancing** ([AIDUDES.c:1723-1802](../../MoM/src/AIDUDES.c#L1723-L1802)): the **alchemist branch** (`alchemy == 1`, [line 1781](../../MoM/src/AIDUDES.c#L1781)) rebalances the *combined* gold+mana pool **every turn** to a target ratio — **90% gold / 10% mana before turn 150, flipping to 30% gold / 70% mana from turn 150** ([1784-1801](../../MoM/src/AIDUDES.c#L1784-L1801)). Non-alchemists ([1725-1780](../../MoM/src/AIDUDES.c#L1725-L1780)) only rebalance on a `Random(10)==1` (10%) tick and eat the 2:1 conversion loss. So Alchemy materially changes AI treasury behavior: continuous, lossless retargeting vs. occasional lossy nudges.
3. **Spell-of-Return path** ([AIDUDES.c:1711-1722](../../MoM/src/AIDUDES.c#L1711-L1722)): a banished, returning AI converts its whole gold reserve to mana at **1:1 with Alchemy vs 2:1 without** — funding the re-cast/rebuild.

**Passive (shared).** The +1 to-hit magic weapon on built units ([NEXTTURN.c:1069](../../MoM/src/NEXTTURN.c#L1069)) is applied to AI units automatically; the AI makes no decision about it and gets the same combat benefit the human does.

**Bugs.**
- **OGBUG — [AIDUDES.c:1715](../../MoM/src/AIDUDES.c#L1715) & [1719](../../MoM/src/AIDUDES.c#L1719):** the Spell-of-Return conversion uses `mana_reserve = …` where the intent is `+=` (both the alchemist 1:1 and non-alchemist 2:1 branches carry the `/* OGBUG should be +=, not = */` note). Effect: the returning wizard's existing mana is **overwritten**, not added to — any banked mana is discarded before `gold_reserve` is zeroed. Preserved as faithful-to-disassembly. **Impact on the AI:** small in practice — a wizard mid-`Spell_Of_Return` is freshly banished and typically holds ~0 mana — but if it had banked mana it is silently lost. Symmetric across the Alchemy and non-Alchemy branches, so it doesn't change Alchemy's *relative* advantage.

**Net.** Alchemy is one of the few retorts the AI reasons about explicitly, and it does so sensibly: bias power toward mana, and keep a strategically-shifting gold/mana split with the free conversion. The only defect at these sites is the preserved `=`/`+=` OGBUG, which is near-harmless.

### 1 — Warlord (`warlord`)  · passive

**Effect.** All your units are built one experience level higher (+1 `Level`).
**Where.** [CITYCALC.c:1293-1296](../../MoM/src/CITYCALC.c#L1293-L1296) (`Calc_Unit_Level` adds +1 for a warlord owner); starting units also get it at map gen ([MAPGEN.c:6983](../../MoM/src/MAPGEN.c#L6983)).
**AI usage.** Passive — the AI never checks `warlord`; its units simply come out a level higher, exactly as the human's do. Combines with the `Crusade` global.
**Bugs.** None at these sites.

### 2–4 — Chaos / Nature / Sorcery Mastery (`chaos_mastery`, `nature_mastery`, `sorcery_mastery`)  · passive

These three are structurally identical, each acting only on its own realm. **Four shared effects, all passive:**
1. **Research +15%** in-realm — [CITYCALC.c:1059-1089](../../MoM/src/CITYCALC.c#L1059-L1089) (Nature 1059, Sorcery 1070, Chaos 1081).
2. **Casting cost −15%** in-realm — [NEXTTURN.c:1238-1260](../../MoM/src/NEXTTURN.c#L1238-L1260) (Nature 1240, Sorcery 1248, Chaos 1256).
3. **Node power ×2** for that realm's nodes — [CITYCALC.c:190-192](../../MoM/src/CITYCALC.c#L190-L192).
4. **Dispel resistance ×2** — your spells of that realm are twice as hard to dispel, via `Calculate_Dispel_Difficulty` [OverSpel.c:186-211](../../MoM/src/OverSpel.c#L186-L211) (adds +1 to a multiplier that starts at 1).

**AI usage.** Passive across the board — no AI code branches on any mastery. The AI's spell-cost, research, node income, and dispel-resistance numbers are all silently improved. Because the masteries reduce *casting cost*, they indirectly let the AI's generic spell-selection logic afford more/bigger casts, but there is no mastery-aware decision.
**Bugs.** None at these sites. Note the dispel-resistance multiplier is **additive** with Archmage (mastery +1, archmage +1 → ×3), not multiplicative — faithful to the OG formula, not a bug.

### 5–6 — Infernal Power / Divine Power (`infernal_power`, `divine_power`)  · passive

**Effect.** Religious buildings (Shrine/Temple/Parthenon/Cathedral) produce **+50% Power** and give **+50% unrest pacification**. In the code the two retorts are checked together and grant the *same* bonus.
**Where.** Power: [CITYCALC.c:2318-2325](../../MoM/src/CITYCALC.c#L2318-L2325) (`building_magic_power_modifier` 100→150). Pacification: [CITYCALC.c:3614-3621](../../MoM/src/CITYCALC.c#L3614-L3621) (`Pacify_Religion += Pacify_Religion/2`).
**AI usage.** Passive — the AI's religious cities just yield more power and less unrest. (The gameplay difference between the two retorts — Divine Power ties to Life books, Infernal Power to Death books — is a wizard-creation prerequisite, not a runtime branch here.)
**Bugs.** None at these sites.

### 7 — Sage Master (`sage_master`)  · passive

**Effect.** +25% research on **all** spells.
**Where.** [CITYCALC.c:1045-1048](../../MoM/src/CITYCALC.c#L1045-L1048) (`research_bonus_percentage += 25`).
**AI usage.** Passive — faster research for the AI, no decision.
**Bugs.** None.

### 8 — Channeler (`channeler`)  · **ACTIVE**

**Effect.** (1) Spell (enchantment) upkeep halved; (2) no distance penalty when casting combat spells away from the fortress; (3) faster/cheaper combat casting.
**Where.**
- Enchantment upkeep −50%: [CITYCALC.c:557-562](../../MoM/src/CITYCALC.c#L557-L562).
- Combat-cast distance penalty removed (`distance = 2`): [Combat.c:13970-13973](../../MoM/src/Combat.c#L13970-L13973).
- Force-disband-summons pressure halved (`Channeler_Divisor = 2`): [NEXTTURN.c:2788-2795](../../MoM/src/NEXTTURN.c#L2788-L2795).
**AI usage — active.** In `AI_Select_Spell_Group`, Channeler (or Conjurer) multiplies the **Summoning group weight** `modifiers[1]` by 1.5 ([AISPELL.c:552-554](../../MoM/src/AISPELL.c#L552-L554); group dispatched at [351](../../MoM/src/AISPELL.c#L351)). So a Channeler AI is measurably biased toward summoning — sensible, since its summons are cheaper to keep and it casts combat spells without the distance tax.
**Bugs.** None at these sites.

### 9 — Myrran (`myrran`)  · passive (start-time)

**Effect.** You begin on **Myrror** instead of Arcanus (and can field the four Myrran races).
**Where.** Home-city placement forces `wp = MYRROR_PLANE` when `myrran` is set — [MAPGEN.c:1114-1117](../../MoM/src/MAPGEN.c#L1114-L1117); also consulted for opponent placement and lair/plane setup ([MAPGEN.c:1285](../../MoM/src/MAPGEN.c#L1285), [1518](../../MoM/src/MAPGEN.c#L1518), [1645](../../MoM/src/MAPGEN.c#L1645)).
**AI usage.** Passive and one-time — set entirely at map generation. A Myrran AI opponent thereafter simply *is* on Myrror; nothing in the turn loop reasons about the retort.
**Bugs.** None at these sites.

### 10 — Archmage (`archmage`)  · passive

**Effect.** +10 base casting skill; +50% skill growth from power invested in skill; your spells twice as hard to dispel.
**Where.**
- +10 base skill: [NEXTTURN.c:1151-1153](../../MoM/src/NEXTTURN.c#L1151-L1153) (`Player_Base_Casting_Skill`).
- +50% skill growth: [NEXTTURN.c:2432-2434](../../MoM/src/NEXTTURN.c#L2432-L2434) (`skill_income * 3 / 2`).
- Dispel resistance +1 multiplier: [OverSpel.c:213-218](../../MoM/src/OverSpel.c#L213-L218).
- Inverse skill computation backs out the +10: [Spells129.c:136-138](../../MoM/src/Spells129.c#L136-L138).
**AI usage.** Passive — the AI's higher skill and faster skill growth flow through the shared formulas; no archmage-aware branch. Indirectly potent for the AI because a higher casting-skill cap lets its spell-selection logic cast more per turn/combat.
**Bugs.** None at these sites (the additive stacking with masteries is OG-faithful, noted in §2–4).

### 11 — Mana Focusing (`mana_focusing`)  · passive

**Effect.** +25% to the mana portion of your power output.
**Where.** [CITYCALC.c:938-941](../../MoM/src/CITYCALC.c#L938-L941) (`mana_portion += mana_portion/4`).
**AI usage.** Passive — pure economic uplift; the AI banks more mana without deciding anything.
**Bugs.** None.

### 12 — Node Mastery (`node_mastery`)  · passive

**Effect.** (1) Power from **all** nodes ×2; (2) in node battles you are not restricted by the node's realm aura.
**Where.** Node power ×2 for any node type: [CITYCALC.c:193](../../MoM/src/CITYCALC.c#L193). Combat aura unrestricted (`Target_X = ST_UNDEFINED`): [Combat.c:12885-12894](../../MoM/src/Combat.c#L12885-L12894).
**AI usage.** Passive. Note the node bonus **stacks multiplicatively** with a matching realm mastery: a Chaos node owned by a wizard with both Chaos Mastery and Node Mastery is ×2 (mastery) × ×2 (node mastery) = **×4** ([CITYCALC.c:191](../../MoM/src/CITYCALC.c#L191) then [193](../../MoM/src/CITYCALC.c#L193)). The AI benefits automatically; its node-meld logic (`AI_Stacks_Do_Meld`) doesn't weigh the retort.
**Bugs.** None at these sites.

### 13 — Famous (`famous`)  · passive (partly human-only)

**Effect.** +Fame each turn; roughly double the chance of item / mercenary / hero offers.
**Where.** Offer-chance doubling in the offers loop: item [NEXTTURN.c:3394-3397](../../MoM/src/NEXTTURN.c#L3394-L3397), mercenary [3431-3434](../../MoM/src/NEXTTURN.c#L3431-L3434), hero [3493-3496](../../MoM/src/NEXTTURN.c#L3493-L3496). Hero battle-unit upkeep zeroed for a famous owner: [Combat.c:17525-17532](../../MoM/src/Combat.c#L17525-L17532) (shared with the Noble hero ability).
**AI usage.** Passive — but with an important asymmetry: the **item (magic-item merchant) offer block is guarded by `HUMAN_PLAYER_IDX`** ([NEXTTURN.c:3390](../../MoM/src/NEXTTURN.c#L3390)), so the AI *never* receives random item offers and Famous's item-offer doubling is **human-only**. The mercenary and hero offer doublings *do* apply to the AI, feeding its `AI_Accept_Mercenaries` / `AI_Accept_Hero` decisions.
**Bugs.** None at these sites, but the human-only item-offer path means Famous is **worth less to the AI than to the human** — a structural limitation, not a coding bug.

### 14 — Runemaster (`runemaster`)  · passive

**Effect.** Cheaper/faster Arcane spells; much stronger dispelling.
**Where.**
- Arcane research +25%: [CITYCALC.c:1106-1108](../../MoM/src/CITYCALC.c#L1106-L1108).
- Arcane casting cost −25%: [NEXTTURN.c:1278-1284](../../MoM/src/NEXTTURN.c#L1278-L1284).
- Overland dispel ×2 on Disenchant and Disjunction: [OverSpel.c:1811-1816](../../MoM/src/OverSpel.c#L1811-L1816), [1848-1853](../../MoM/src/OverSpel.c#L1848-L1853).
- Combat dispel ×3 (Dispel Magic True) / ×2 (disenchants): [Combat.c:12200](../../MoM/src/Combat.c#L12200), [12229](../../MoM/src/Combat.c#L12229).
**AI usage.** Passive — the AI's dispels (overland `AITP_Disjunction`/Disenchant path and combat) are automatically stronger. Runemaster is **not** part of `Calculate_Dispel_Difficulty`, so it strengthens *your* dispels but does not make *your* spells harder to dispel (that's the masteries + Archmage).
**Bugs.** **OGBUG/BUGBUG note — [Combat.c:12190](../../MoM/src/Combat.c#L12190):** the combat Runemaster ×3 is gated on `spl_Dispel_Magic_True` only; a source comment flags that plain combat **Dispel Magic gets no Runemaster bonus**. Preserved. Impact on the AI: when the AI casts ordinary combat Dispel Magic it dispels at normal strength despite Runemaster, mildly under-using the retort.

### 15 — Conjurer (`conjurer`)  · **ACTIVE**

**Effect.** Summoned creatures cost 25% less to cast, research 25% faster, and cost 25% less upkeep.
**Where.**
- Summon research +25%: [CITYCALC.c:1050-1052](../../MoM/src/CITYCALC.c#L1050-L1052).
- Summon casting −25%: [NEXTTURN.c:1270-1276](../../MoM/src/NEXTTURN.c#L1270-L1276).
- Fantastic-unit upkeep ×0.75: [CITYCALC.c:603-609](../../MoM/src/CITYCALC.c#L603-L609).
**AI usage — active.** Same site as Channeler — Conjurer multiplies the **Summoning group weight** ×1.5 in `AI_Select_Spell_Group` ([AISPELL.c:552-554](../../MoM/src/AISPELL.c#L552-L554)). So a Conjurer AI leans into summoning, which is exactly where the retort pays off (cheaper casts + cheaper upkeep).
**Bugs.** None at these sites.

### 16 — Charismatic (`charismatic`)  · passive (with an AI-affecting bug)

**Effect.** Positive diplomacy twice as effective, negative half as bad; heroes/mercenaries and item-merchant offers cost half.
**Where.**
- Diplomacy: positive value ×2, negative value ÷2 — [DIPLOMAC.c:3562-3578](../../MoM/src/DIPLOMAC.c#L3562-L3578).
- Hero-hire cost ÷2 in the AI's own accept path: [HIRE.c:924-926](../../MoM/src/HIRE.c#L924-L926) (`AI_Accept_Hero`).
- Hire cost ÷2 in the shared hire-cost routine: [CITYCALC.c:3024-3027](../../MoM/src/CITYCALC.c#L3024-L3027).
- Item-merchant discount (human-only UI): [HIRE.c:258](../../MoM/src/HIRE.c#L258), [NEXTTURN.c:3416](../../MoM/src/NEXTTURN.c#L3416).
**AI usage.** Mostly passive, but the AI's diplomacy engine and hero/merc affordability *do* consume the retort (its hire costs are halved, its diplomatic deltas scaled). No strategic "because I'm charismatic" branch.
**Bugs.** **OGBUG — [CITYCALC.c:3016-3027](../../MoM/src/CITYCALC.c#L3016-L3027) (drake178: "wrong order of operations"):** the affordability test `gold_reserve < Total_Cost` runs **before** the Charismatic ÷2 discount is applied, so a hire that would be affordable *at half price* can be rejected on the full price. This block is **not** human-gated (`if(player_idx != HUMAN_PLAYER_IDX)` follows at [3029](../../MoM/src/CITYCALC.c#L3029)), so it degrades the AI's Charismatic hero/merc hiring too — the AI under-hires relative to what Charismatic should allow. Preserved as faithful-to-disassembly.

### 17 — Artificer (`artificer`)  · **ACTIVE**

**Effect.** Magic-item creation spells cost half; created items have no level cap and can take other-realm powers.
**Where.** Crafting-spell casting cost −50%: [NEXTTURN.c:1262-1268](../../MoM/src/NEXTTURN.c#L1262-L1268) (`scc_Crafting_Spell`).
**AI usage — active.** In its overland spell-priority scoring the AI raises the priority of the item-creation spells when it has Artificer: **Enchant Item 20→30** ([AISPELL.c:1202-1211](../../MoM/src/AISPELL.c#L1202-L1211)) and **Create Artifact 30→40** ([AISPELL.c:1214-1223](../../MoM/src/AISPELL.c#L1214-L1223)). So an Artificer AI is more inclined to actually cast the (now half-price) crafting spells.
**Bugs.** None at these Artificer sites. (Adjacent unrelated OGBUG in the same function at [AISPELL.c:1229](../../MoM/src/AISPELL.c#L1229) — a "have the Chosen" check compares against Barbarian Spearmen — is not Artificer-related.)

---

## Bug-impact summary

| Retort | Bug | Site | Type | Impact on AI |
| --- | --- | --- | --- | --- |
| Alchemy | `=` should be `+=` in Spell-of-Return conversion; existing mana overwritten | [AIDUDES.c:1715/1719](../../MoM/src/AIDUDES.c#L1715-L1719) | OGBUG | Minor — returning AI loses any banked mana (usually ~0) |
| Charismatic | Discount applied *after* affordability check (wrong order of ops) | [CITYCALC.c:3016-3027](../../MoM/src/CITYCALC.c#L3016-L3027) | OGBUG | Real — AI under-hires heroes/mercs it could afford at half price; not human-gated |
| Runemaster | Plain combat Dispel Magic gets no Runemaster ×-bonus (only Dispel Magic True) | [Combat.c:12190-12202](../../MoM/src/Combat.c#L12190-L12202) | OGBUG (flagged) | Minor — AI's ordinary combat dispels are weaker than the retort implies |
| Famous | Random **item** offers are human-only | [NEXTTURN.c:3390](../../MoM/src/NEXTTURN.c#L3390) | Asymmetry (not a code bug) | Structural — Famous is worth less to the AI (no item offers); merc/hero offers still apply |

All are preserved OG behavior (no reconstruction-error "R" bugs found at retort sites). None should be "fixed" in the baseline per project policy; they are documented for behavior-accuracy.

## Takeaways

1. **Retorts are a rules layer, not an AI layer.** 14 of 18 are purely passive from the AI's side — it benefits from the same shared-code effect the human gets and never reasons about the retort.
2. **The four active retorts cluster on spellcasting.** Alchemy (treasury management), Channeler + Conjurer (summoning bias, shared site), and Artificer (item-spell priority) are the only ones with retort-aware AI branches — all in `AIDUDES`/`AISPELL`.
3. **The AI's weakest retort usage is where value depends on deliberate play.** Charismatic (hobbled by the order-of-ops bug), Famous (no AI item offers), and the passive economic multipliers help the AI's *numbers* but not its *decisions*.

## Cross-references

- Companions: [#MOM-FeatureFreak.md](#MOM-FeatureFreak.md) §6 (Wizards & Retorts), [MOM-FeatureFreak-AI-Usage.md](MOM-FeatureFreak-AI-Usage.md).
- Retort data & creation: [MoM-MirrorScreen.md](../MoM-MirrorScreen.md), [MoM-Wizard-SpellCastingSkill.md](../MoM-Wizard-SpellCastingSkill.md), [MoX-NewGame-Init-Players.md](../MoX-NewGame-Init-Players.md).
- AI spell selection: [MoM-NextTurn-AI.md](../MoM-NextTurn-AI.md), [ComputerPlayer/AISPELL-AI_Select_Spell_Group.md](../ComputerPlayer/AISPELL-AI_Select_Spell_Group.md).

---

*Status: all 18 retorts walked and verified against source (effect sites, AI-usage classification, and bugs). Data model and every cited file:line grep/Read-verified.*
