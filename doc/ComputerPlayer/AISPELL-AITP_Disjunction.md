AISPELL-AITP_Disjunction.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\IDK_AITP_Disjunction__STUB.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\IDK_AITP_Disjunction__STUB.c

Next_Turn_Proc()
|-> Next_Turn_Calc()
    |-> AI_Next_Turn()
         |-> Cast_Spell_Overland()

Cast_Spell_Overland()
    switch(spell_data_table[spell_idx].type)
        // OVERLAND:  Disjunction, Disjunction True
        case scc_Disjunctions:  // 20
            |-> AITP_Disjunction()
            |-> Overland_Enchantment_Spell_Index()
            ...in-line spell-cast code

Cast_Spell_Overland()
    switch(spell_data_table[spell_idx].type)
        // Animate Dead, Black Wind, Call The Void, Change Terrain, Chaos Channels, Corruption, Cracks Call, Death Wish, Disrupt, Earth Lore, Earth to Mud, Earthquake, Enchant Road, Great Unsummoning, Healing, Incarnation, Lycanthropy, Magic Vortex, Move Fortress, Natures Cures, Plane Shift, Raise Dead, Raise Volcano, Recall Hero, Resurrection, Spell Binding, Spell Of Mastery, Spell Of Return, Spell Ward, Stasis, Summon Champion, Summon Hero, Transmute, Wall of Stone, Warp Creature, Warp Node, Warp Wood, Word of Recall
        case scc_Special_Spell:  //  5
            switch(spell_idx)
                case spl_Spell_Binding:
                    |-> Cast_Spell_Binding()
                        |-> AITP_Disjunction()
                        |-> Overland_Enchantment_Spell_Index()
                        ...in-line spell-cast code

---

# `AITP_Disjunction` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Disjunction` | [AISPELL.c:3332-3797](../../MoM/src/AISPELL.c#L3332-L3797) | AI target picker for **Disjunction / Disjunction True** and **Spell Binding**: score every *enemy* overland enchantment, pick the single highest-scoring one, and return which wizard (`*targeted_player_idx`) and which enchantment slot (`*targeted_spell_idx`) to strip. Returns `ST_TRUE` if a target was found, `ST_FALSE` otherwise. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1), carrying a major preserved OG bug.** The body ([AISPELL.c:3332](../../MoM/src/AISPELL.c#L3332)) was reconstructed from `IDK_AITP_Disjunction__STUB.asm` and matches it value-for-value across both scoring branches, the realm conditionals, the max-find, and the return. The asm keeps two indices distinct — `itr_players` is the opponent being scored (asm `si`), `player_idx` is the caster parameter (asm `[bp+player_idx]`); the C mirrors that exactly. It compiles as part of AISPELL.c / momlib. The `__STUB` is gone from the production name but **remains in the asm/`.c` filename** (the OG symbol). The headline issue is an original-game bug faithfully preserved: the score matrix is a **signed `int8_t`**, so Disjunction's `200`/`250` scores read back **negative** and can never be selected ([B1](#b1-ogbug--signed-byte-score-overflow)). The asm is the authority.

## Purpose

The AI's target picker for the two "strip an enemy enchantment" spells. It is **not** one of the `AI_Select_Spell_Group_*` leaf pickers (those choose *which spell*); this chooses *what to target* once a Disjunction-type or Spell-Binding cast is underway. It has two callers (see header):

- `Cast_Spell_Overland` → `scc_Disjunctions` (Disjunction, Disjunction True)
- `Cast_Spell_Overland` → `scc_Special_Spell` → `Cast_Spell_Binding` (Spell Binding)

For each opponent (every player except the caster), it scores that opponent's *active* overland enchantments into a 6×25 matrix, then scans for the maximum and returns its coordinates.

### Signature & return

```c
int16_t AITP_Disjunction(int16_t * targeted_player_idx, int16_t * targeted_spell_idx, int16_t spell_idx, int16_t player_idx);
```

- `spell_idx` selects the scoring branch (Spell Binding vs Disjunction/True); `player_idx` is the **caster**.
- On success: `*targeted_player_idx = target wizard index`, `*targeted_spell_idx = target enchantment slot (0..23)`, returns `ST_TRUE`.
- On no target: returns `ST_FALSE` (outputs untouched).
- Unknown `spell_idx`: calls `Cast_Spell_Target_Error(spell_idx)` (a fatal diagnostic) and returns `ST_FALSE`.

### The score matrix

`int8_t target_spell_scores[6][25]` — one row per player (0..5), one column per overland enchantment. The enchantment constants (`ETERNAL_NIGHT=0`, `EVIL_OMENS=1`, …, `AWARENESS=0x17`) **are** the column indices `0..23` ([MOM_DEF.h:1361-1384](../../MoX/src/MOM_DEF.h#L1361-L1384)) — the same enum that indexes `_players[].Globals[]` — so `target_spell_scores[wiz][ENCHANT]` lines up 1:1 with the asm's `var_9E + wiz*25 + slot` (the asm stores to sequential slots `0..23` in check order, which equals the enum value). A score is written only if that opponent has the enchantment active.

## How it's reached

| Caller | Via | Notes |
|---|---|---|
| `Cast_Spell_Overland` | `scc_Disjunctions` | Disjunction / Disjunction True targeting. |
| `Cast_Spell_Binding` | `scc_Special_Spell` | Spell Binding targeting. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `IDK_AITP_Disjunction__STUB.asm`. Realm codes: `sbr_Nature=0, sbr_Sorcery=1, sbr_Chaos=2, sbr_Life=3, sbr_Death=4` ([MOM_DAT.h:732-736](../../MoX/src/MOM_DAT.h#L732-L736)).

### Clear the matrix ([3350-3356](../../MoM/src/AISPELL.c#L3350-L3356))

Zero all `6 × 25` cells. **Faithful** (asm 20-43).

### Score branches ([3358-3526](../../MoM/src/AISPELL.c#L3358-L3526) Spell Binding, [3556-3761](../../MoM/src/AISPELL.c#L3556-L3761) Disjunction)

For each `itr != player_idx`, set `itr_players = itr` (the opponent, asm `si`); if opponent `itr_players` has enchantment X active, write a desirability score into `target_spell_scores[itr_players][X]`. The `Globals[]` read and the score-row index use **`itr_players`** (opponent); every realm conditional reads **`player_idx`** (the **caster**'s `Prim_Realm`/`Sec_Realm`) — exactly the asm's `si`-vs-`[bp+player_idx]` split. Both branches cover the same 24 enchantments; the values differ. Conditions and values match the asm exactly:

| Slot | Enchantment | Spell Binding | Disjunction |
|---|---|---|---|
| 0 | ETERNAL_NIGHT | Death → 30, else 100 | Life → **200**, else 50 |
| 1 | EVIL_OMENS | 10 | Life\|Nature → **200**, else 0 |
| 2 | ZOMBIE_MASTERY | 50 | 50 |
| 3 | AURA_OF_MAJESTY | 20 | 20 |
| 4 | WIND_MASTERY | 15 | 15 |
| 5 | SUPPRESS_MAGIC | 100 | **250** |
| 6 | TIME_STOP | 10 | 10 |
| 7 | NATURES_AWARENESS | 10 | 10 |
| 8 | NATURES_WRATH | 20 | Death\|Chaos → **200**, else 0 |
| 9 | HERB_MASTERY | 20 | Death → 0, else 50 |
| 10 | CHAOS_SURGE | Chaos → 30, else 100 | Chaos → 0, else 100 |
| 11 | DOOM_MASTERY | 50 | 50 |
| 12 | GREAT_WASTING | 20 | 20 |
| 13 | METEOR_STORMS | 30 | 30 |
| 14 | ARMAGEDDON | 40 | 40 |
| 15 | TRANQUILITY | 30 | Chaos → **200**, else 0 |
| 16 | LIFE_FORCE | 30 | Chaos → **200**, else 0 |
| 17 | CRUSADE | 20 | 20 |
| 18 | JUST_CAUSE | 15 | 15 |
| 19 | HOLY_ARMS | 30 | 30 |
| 20 | PLANAR_SEAL | 10 | 10 |
| 21 | CHARM_OF_LIFE | 60 | 60 |
| 22 | DETECT_MAGIC | 1 | 1 |
| 23 | AWARENESS | 1 | 1 |

The **bold** Disjunction values (`200`, `250`) are the ones bitten by [B1](#b1-ogbug--signed-byte-score-overflow).

### Max-find + return ([3528-3553](../../MoM/src/AISPELL.c#L3528-L3553) / [3763-3788](../../MoM/src/AISPELL.c#L3763-L3788))

`max_score = 0`, `target_*_idx = ST_UNDEFINED`; scan the matrix, and on a strictly-greater score record the player and slot. If nothing beat 0, return `ST_FALSE`; else write `*targeted_player_idx`/`*targeted_spell_idx` and return `ST_TRUE`. The comparison reads the cell as `int8_t` and promotes to `int`, mirroring the asm's `mov al,[..] / cbw` (a **sign-extending** read — the crux of B1).

## B1 (OGBUG) — signed-byte score overflow

`target_spell_scores` is `int8_t`. The Disjunction branch assigns `0xC8` (200) and `0xFA` (250), which as a signed byte are `-56` and `-6`. The max-find reads cells with a sign-extending cast (`(int)`, matching the asm's `cbw`) and only accepts scores `> 0`. So **every Disjunction target the AI rates highest — enemy Eternal Night (vs a Life caster), Evil Omens, Suppress Magic, Nature's Wrath, Tranquility, Life Force — reads back negative and is never chosen.** The AI can only ever pick the `≤127` targets (Chaos Surge 100, Charm of Life 60, Zombie/Doom Mastery 50, …); if an opponent has *only* the high-value globals up, the AI finds no target and won't Disjunction at all.

Spell Binding is unaffected — its maximum score is `100`, which fits in a signed byte.

This is faithfully reproduced from the asm (byte storage + `cbw` read). Per the faithful-to-disassembly rule it is **preserved, not fixed** — keep `int8_t` and the sign-extending read. Reconstruction note: switching the matrix to `uint8_t` (or reading unsigned) would silently "fix" the OG behavior and change AI play — don't.

## Target priority (emergent)

There is **no explicit priority list** in the code (contra the fandom comment at the top of this file) — priority is an emergent property of the max-find: the highest score wins, and ties are broken by **lowest player index, then lowest enchantment slot** (the scan runs players `0→5`, slots `0→24`, and uses a *strictly-greater* test, so the first cell holding the maximum is kept). The tables below order the 24 enchantments by that effective preference. Where a realm-conditional changes the weight, the caster realm is the **caster's** (`player_idx`) `Prim_Realm`/`Sec_Realm`.

This reproduces the observed in-game behavior: Aura of Majesty (20) is stripped before Just Cause (15), which is stripped before Planar Seal (10).

### Spell Binding (`spl_Spell_Binding`)

Max weight is `100` — no `int8_t` overflow, so every row is reachable.

| Priority | Score | Enchantment(s) (slot) |
|---|---|---|
| 1 | 100 | ETERNAL_NIGHT (0)†, SUPPRESS_MAGIC (5), CHAOS_SURGE (10)‡ |
| 2 | 60 | CHARM_OF_LIFE (21) |
| 3 | 50 | ZOMBIE_MASTERY (2), DOOM_MASTERY (11) |
| 4 | 40 | ARMAGEDDON (14) |
| 5 | 30 | METEOR_STORMS (13), TRANQUILITY (15), LIFE_FORCE (16), HOLY_ARMS (19) |
| 6 | 20 | AURA_OF_MAJESTY (3), NATURES_WRATH (8), HERB_MASTERY (9), GREAT_WASTING (12), CRUSADE (17) |
| 7 | 15 | WIND_MASTERY (4), JUST_CAUSE (18) |
| 8 | 10 | EVIL_OMENS (1), TIME_STOP (6), NATURES_AWARENESS (7), PLANAR_SEAL (20) |
| 9 | 1 | DETECT_MAGIC (22), AWARENESS (23) |

† ETERNAL_NIGHT drops to **30** if the caster's realm includes **Death**.
‡ CHAOS_SURGE drops to **30** if the caster's realm includes **Chaos**.

### Disjunction / Disjunction True (`spl_Disjunction`, `spl_Disjunction_True`) — effective order after [B1](#b1-ogbug--signed-byte-score-overflow)

The nominal weights include several `≥128` values (`200`, `250`); as signed bytes those read back negative and are **never** selected, and the `0` outcomes are never selected either. The reachable order is therefore dominated by the `≤100` weights:

| Priority | Score | Enchantment(s) (slot) |
|---|---|---|
| 1 | 100 | CHAOS_SURGE (10)‡ |
| 2 | 60 | CHARM_OF_LIFE (21) |
| 3 | 50 | ETERNAL_NIGHT (0)†, ZOMBIE_MASTERY (2), HERB_MASTERY (9)§, DOOM_MASTERY (11) |
| 4 | 40 | ARMAGEDDON (14) |
| 5 | 30 | METEOR_STORMS (13), HOLY_ARMS (19) |
| 6 | 20 | AURA_OF_MAJESTY (3), GREAT_WASTING (12), CRUSADE (17) |
| 7 | 15 | WIND_MASTERY (4), JUST_CAUSE (18) |
| 8 | 10 | TIME_STOP (6), NATURES_AWARENESS (7), PLANAR_SEAL (20) |
| 9 | 1 | DETECT_MAGIC (22), AWARENESS (23) |
| — | never | EVIL_OMENS (1), SUPPRESS_MAGIC (5), NATURES_WRATH (8), TRANQUILITY (15), LIFE_FORCE (16) |

† ETERNAL_NIGHT scores **50** normally, but **200 → never** if the caster's realm includes **Life**.
‡ CHAOS_SURGE scores **100** normally, but **0 → never** if the caster's realm includes **Chaos**.
§ HERB_MASTERY scores **50** normally, but **0 → never** if the caster's realm includes **Death**.

## OG quirks (preserve when reconstructing)

- **B1 — signed-byte overflow** (above): the dominant one.
- **Asymmetric max-find bound**: the Spell Binding scan uses a hard-coded `itr_players < NUM_PLAYERS` (6) ([3532](../../MoM/src/AISPELL.c#L3532), asm 490 `cmp si, 6`), while the Disjunction scan uses `itr_players < _num_players` ([3767](../../MoM/src/AISPELL.c#L3767), asm 1084). Preserved as-is.
- **Realm conditions read the caster** (`player_idx`), not the target — correct per the asm (the AI weights stripping a global by how much *its own* realm dislikes it).

## Companion — `Overland_Enchantment_Spell_Index` (slot → spell ID)

`AITP_Disjunction` returns an enchantment **slot** (`*targeted_spell_idx`, 0..23); the caller must turn that back into a `spl_*` **spell ID** to look up the full `spell_data_table[]` record (for dispel difficulty and the fizzle notification). That inverse map is `Overland_Enchantment_Spell_Index` ([OverSpel.c:1988-2022](../../MoM/src/OverSpel.c#L1988-L2022), renamed from `IDK_Get_Global_Enchant_Index__STUB`; authority `…\ovr135\IDK_Get_Global_Enchant_Index__STUB.asm`). It is called by `Cast_Spell_Overland` immediately after the AI picker returns ([OverSpel.c:1862](../../MoM/src/OverSpel.c#L1862)) — not by `AITP_Disjunction` itself.

It is a flat 24-case `switch` (slot constant → `spl_*`), **1:1 with the asm jump table** `off_BCC31`, which lists the 24 targets in case order `0..23` with no gaps or reordering:

| Slot | → spell | Slot | → spell | Slot | → spell |
|---|---|---|---|---|---|
| 0 ETERNAL_NIGHT | `spl_Eternal_Night` (197) | 8 NATURES_WRATH | `spl_Natures_Wrath` (40) | 16 LIFE_FORCE | `spl_Life_Force` (156) |
| 1 EVIL_OMENS | `spl_Evil_Omens` (198) | 9 HERB_MASTERY | `spl_Herb_Mastery` (38) | 17 CRUSADE | `spl_Crusade` (158) |
| 2 ZOMBIE_MASTERY | `spl_Zombie_Mastery` (188) | 10 CHAOS_SURGE | `spl_Chaos_Surge` (116) | 18 JUST_CAUSE | `spl_Just_Cause` (127) |
| 3 AURA_OF_MAJESTY | `spl_Aura_Of_Majesty` (59) | 11 DOOM_MASTERY | `spl_Doom_Mastery` (117) | 19 HOLY_ARMS | `spl_Holy_Arms` (154) |
| 4 WIND_MASTERY | `spl_Wind_Mastery` (57) | 12 GREAT_WASTING | `spl_Great_Wasting` (114) | 20 PLANAR_SEAL | `spl_Planar_Seal` (135) |
| 5 SUPPRESS_MAGIC | `spl_Suppress_Magic` (79) | 13 METEOR_STORMS | `spl_Meteor_Storms` (113) | 21 CHARM_OF_LIFE | `spl_Charm_Of_Life` (160) |
| 6 TIME_STOP | `spl_Time_Stop` (80) | 14 ARMAGEDDON | `spl_Armageddon` (120) | 22 DETECT_MAGIC | `spl_Detect_Magic` (206) |
| 7 NATURES_AWARENESS | `spl_Natures_Awareness` (34) | 15 TRANQUILITY | `spl_Tranquility` (157) | 23 AWARENESS | `spl_Awareness` (209) |

**OGBUG (documented in-code):** for an out-of-range slot the asm falls through to `@@Done` with `DX` never set, returning whatever `DX` held on entry (undefined). The C port returns `0` from `default:` — a deterministic taming, not a faithful reproduction. It is **unreachable** in practice: the sole caller passes a slot that `AITP_Disjunction` produced, and that is always `0..23` (column 24 of the `[6][25]` matrix is never scored, so it can never win the max-find).

## Sub-functions / external calls

- **`Cast_Spell_Target_Error(spell_idx)`** — fatal "could not be found for CP" diagnostic on the unknown-`spell_idx` path.
- **`Overland_Enchantment_Spell_Index(slot)`** — *caller-side* companion (above); translates the returned slot to a `spl_*` ID. Not invoked by `AITP_Disjunction`.
- **Globals read:** `_players[].Globals[]` (all 24 overland-enchantment slots), `_players[].Prim_Realm`/`Sec_Realm`, `_num_players`.

## Related references

- `…\ovr156\IDK_AITP_Disjunction__STUB.asm` — IDA Pro 5.5 disassembly (the authority; the OG filename keeps the `__STUB` tag).
- `…\ovr156\IDK_AITP_Disjunction__STUB.c` — Piethawn IDA-C.
- [AISPELL-AI_Select_Spell_Group_Disjunction.md](AISPELL-AI_Select_Spell_Group_Disjunction.md) — the leaf picker that selects the Disjunction *spell* (this picks its *target*).
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
