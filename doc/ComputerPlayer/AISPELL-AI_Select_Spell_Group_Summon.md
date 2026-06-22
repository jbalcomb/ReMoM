AISPELL-AI_Select_Spell_Group_Summon.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickSummon.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickSummon.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_Summon()
                    |-> SPL_IsLifeSupressed()
                    |-> CRP_SPL_IsNatSuppressed()
                    |-> SPL_IsDthSuppressed()
                    |-> SPL_IsChsSuppressed()

---

# `AI_Select_Spell_Group_Summon` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AI_Select_Spell_Group_Summon` | [AISPELL.c:1009-1352](../../MoM/src/AISPELL.c#L1009-L1352) | When the category picker chose "summon" (category 1), decide *which* summoning/utility spell the AI casts: build a weighted priority over ~41 candidate spells, then weighted-random pick one and return its `spl_*` id (0 = none). |

> **Status: BUILDS CLEAN — faithful to the asm, carries preserved OG bugs.** The body ([AISPELL.c:1009-1352](../../MoM/src/AISPELL.c#L1009-L1352)) started as a verbatim Piethawn IDA-C paste. The priority-building phase and the dispatch `switch` are faithful to `AI_OVL_PickSummon.asm` — the switch mirrors the 43-entry jump table 1:1, including the duplicate-Skeletons at `bx` 25/26 and the return-0 gap at `bx` 29 (verified against the `off_E53CC` bytes in `_misc.asm`), and `players_spell_list` is the 1-based `spells_list - 1` like the sibling pickers. `cmake --build --preset MSVC-debug` compiles and links with no errors or warnings. The Incarnation guard's two OG bugs are correctly preserved ([OG quirks](#og-quirks-preserve-when-reconstructing)). The asm + the `_misc.asm` jump-table bytes are the authority.

## Purpose

Leaf picker for category 1 (summon) of [`AI_Select_Spell_Group`](AISPELL-AI_Select_Spell_Group.md). It scores each summonable creature the wizard knows by `casting_cost`, squares the creature scores to bias toward expensive units, layers in special cases (transports, spirits for nodes, heroes, artifacts), scales by active realm-suppression globals, then calls `Get_Weighted_Choice` over a 50-slot priority array and maps the winning slot to a spell id.

### The priority array

`AI_OVL_SplPriorities` ([AISPELL.h](../../MoM/src/AISPELL.h)) is a 50-entry `int16_t` array. The asm addresses it by **byte offset** (`+2`, `+4`, …, `+56h`); since each entry is a word, **C index = byte-offset / 2**. Slot 0 is unused; real entries occupy slots 1..43. `Get_Weighted_Choice(arr, 50)` returns the chosen slot index, and `bx = Random_Result - 1` selects the return spell via the jump table.

| Slot | Spell | Slot | Spell | Slot | Spell |
|---|---|---|---|---|---|
| 1 | War_Bears | 16 | Doombat | 31 | Wraiths |
| 2 | Sprites | 17 | Chaos_Spawn | 32 | Shadow_Demons |
| 3 | Giant_Spiders | 18 | Fire_Giant | 33 | Death_Knights |
| 4 | Basilisk | 19 | Chimeras | 34 | Demon_Lord |
| 5 | Stone_Giant | 20 | Efreet | 35 | Resurrection |
| 6 | Gorgons | 21 | Hydra | 36 | Guardian_Spirit |
| 7 | Behemoth | 22 | Great_Drake | 37 | Magic_Spirit |
| 8 | Colossus | 23 | Unicorns | 38 | Enchant_Item |
| 9 | Great_Wyrm | 24 | Angel | 39 | Create_Artifact |
| 10 | Floating_Island | 25 | Arch_Angel | 40 | Incarnation |
| 11 | Storm_Giant | 26 | *(always 0)* | 41 | Summon_Champion |
| 12 | Djinn | 27 | Skeletons | 42 | Summon_Hero |
| 13 | Sky_Drake | 28 | Ghouls | 43 | Cockatrices |
| 14 | Hell_Hounds | 29 | Night_Stalker | | |
| 15 | Gargoyles | 30 | *(skipped)* | | |

Slots 26 and 30 are dead (weight never set non-zero), so `Get_Weighted_Choice` never returns them; the OG jump table still has entries for them (the duplicate-Skeletons and return-0 cases — see [Pick + dispatch](#pick--dispatch-1301-1351)).

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Spell_Select` | per [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) | Dispatched when `AI_Select_Spell_Group` returns category 1. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AI_OVL_PickSummon.asm`. `players_spell_list` is set up **1-based** (`spells_list - 1`, asm 16) like the other pickers, so each `players_spell_list[spl_X]` reads `spells_list[spl_X - 1]`. Every `casting_cost` read goes through the 1-based `spell_data_table[]`.

### Build base creature weights ([1020-1127](../../MoM/src/AISPELL.c#L1020-L1127))

Clear slots 0..49, then for each known creature set `slot = casting_cost / 10`. Two exceptions, both faithful:
- **Floating_Island** (slot 10): only if `ai_transport_count < 10` **and** `_turn/60 > ai_transport_count`, and uses `casting_cost / 5` (asm 111-127).
- Slot 26 is explicitly set to 0; slot 30 is skipped entirely (asm 268 / the gap before `+3Eh`).

### Square the creature weights ([1129-1163](../../MoM/src/AISPELL.c#L1129-L1163))

`slot = slot * slot` for slots 1..34 — **except slot 11 (Storm_Giant) and slot 30 are NOT squared** (asm jumps `+14h` → `+18h`, and `+3Ah` → `+3Eh`). Faithful; both omissions are preserved with comments.

### Special-case weights ([1165-1250](../../MoM/src/AISPELL.c#L1165-L1250))

- **Resurrection** (35): 100 if known, hero count `< NUM_HEROES`, and `Player_Dead_Hero_Count > 0`.
- **Guardian_Spirit** (36) / **Magic_Spirit** (37): 200 / 100 if known and `AI_NodeOpportunity == 1`, else 0 (`else if` — Magic_Spirit only checked when Guardian_Spirit unknown).
- **Enchant_Item** (38) / **Create_Artifact** (39): 30/20 and 40/30 by `artificer` retort.
- **Incarnation/Summon_Champion/Summon_Hero** (40/41/42): only if hero count `< NUM_HERO_SLOTS` (6); Incarnation 500 when known **and** `have_the_chosen == ST_TRUE`, else Champion 400, else Hero 200 (`else if` chain). The `have_the_chosen` guard carries two preserved OG bugs — see [OG quirks](#og-quirks-preserve-when-reconstructing).
- **Cockatrices** (43): `casting_cost / 5`.

### Realm-suppression scaling ([1252-1299](../../MoM/src/AISPELL.c#L1252-L1299))

- `SPL_IsLifeSupressed()`: scale slots 23,24,25,26,35,36,40 by `*2/3` (asm 553-601).
- `CRP_SPL_IsNatSuppressed()`: scale slots 1-10 and 43 by `*2/3` (asm 603-676).
- `SPL_IsDthSuppressed()`: slots 27-34 — `<20 → 0`, `<50 → /3`, else `/2` (asm 677-724).
- `SPL_IsChsSuppressed()`: slots 14-22 — same `<20/<50/else` banding (asm 725-772).

All four index ranges, thresholds, and scale factors match the asm.

### Pick + dispatch ([1301-1351](../../MoM/src/AISPELL.c#L1301-L1351))

`Random_Result = Get_Weighted_Choice(AI_OVL_SplPriorities, 50);` then `switch(Random_Result - 1)` returns the spell id. The `switch` mirrors the 43-entry jump table (`off_E53CC`, in `_misc.asm`) `bx → spell` map **exactly**, including the two structural oddities that a contiguous switch would lose: the **duplicate Skeletons** at `bx` 25 and 26 (both `loc_E5361`), and the **`return 0` gap** at `bx` 29 (`loc_E53C0`, the dead slot-30 entry). `case 42` → Cockatrices. Verified entry-by-entry against the table bytes.

## Build status

`cmake --build --preset MSVC-debug` builds clean (0 errors, 0 warnings in this function). The four realm-suppression predicates (`SPL_IsLifeSupressed`, `CRP_SPL_IsNatSuppressed`, `SPL_IsDthSuppressed`, `SPL_IsChsSuppressed`) are implemented ([AISPELL.c:1388+](../../MoM/src/AISPELL.c#L1388)), `Player_Dead_Hero_Count` is linkable, and the unit-type check uses the production constant `ut_BarbSpearmen` ([UNITTYPE.h:52](../../MoM/src/UNITTYPE.h#L52)).

## OG quirks (preserve when reconstructing)

- Slot 11 (Storm_Giant) and slot 30 are **not** squared, while their neighbors are.
- Slot 26 is a permanent-0 placeholder; slot 30 is never assigned. The jump table keeps a (dead) duplicate-Skeletons entry for slot 26's `bx` and a return-0 entry for slot 30's `bx` — both must survive in the faithful `switch`.
- Floating_Island and Cockatrices use `casting_cost / 5`; every other creature uses `/ 10`.
- The squared creature weights are biased toward expensive creatures; utility/spirit/hero slots (35-43) are flat constants and are **not** squared.
- **Incarnation guard — two OG bugs** ([1222-1246](../../MoM/src/AISPELL.c#L1222-L1246)), both preserved per the faithful-to-disassembly rule:
  - **Wrong unit type** ([1225](../../MoM/src/AISPELL.c#L1225)): the `have_the_chosen` scan tests `_UNITS[].type == ut_BarbSpearmen` (Barbarian Spearmen, 39); Incarnation summons the hero "The Chosen," so this looks like the wrong constant.
  - **Inverted polarity** ([1233](../../MoM/src/AISPELL.c#L1233)): the asm sets Incarnation's weight to 500 only when the wizard **already owns** that unit (`have_the_chosen == ST_TRUE`, asm `cmp Have_Chosen,1 / jnz`). Summoning a unique hero you already have is pointless — the sensible test is the inverse (`!= ST_TRUE`) — but the OG fires it backwards. The reconstruction matches the asm bytes exactly; both bugs are flagged in-code with `/* OGBUG */` and must not be "corrected."

## Sub-functions / external calls

- **`Get_Weighted_Choice(int16_t *, int16_t)`** ([random.h:31](../../MoX/src/random.h#L31)) — weighted pick over the 50-slot array.
- **`Player_Hero_Count`** ([NEXTTURN.h:84](../../MoM/src/NEXTTURN.h#L84)), **`Player_Dead_Hero_Count`** ([NEXTTURN.h:87](../../MoM/src/NEXTTURN.h#L87)) — hero gating.
- **`SPL_IsLifeSupressed` / `CRP_SPL_IsNatSuppressed` / `SPL_IsDthSuppressed` / `SPL_IsChsSuppressed`** — realm-suppression predicates ([AISPELL.c:1388+](../../MoM/src/AISPELL.c#L1388)).
- **Globals:** `AI_OVL_SplPriorities[50]`, `AI_NodeOpportunity`, `ai_transport_count`, `_players[]`, `_UNITS`/`_units`, `spell_data_table[]`, `_turn`.

## Related references

- `…\ovr156\AI_OVL_PickSummon.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\_misc.asm` — contains the `off_E53CC` jump-table bytes that pin the `bx → spell` map.
- `…\ovr156\AI_OVL_PickSummon.c` — Piethawn IDA-C (what was originally pasted; its contiguous switch did not reproduce the jump table's duplicate/gap entries).
- [AISPELL-AI_Select_Spell_Group.md](AISPELL-AI_Select_Spell_Group.md) — the category picker that dispatches here.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
