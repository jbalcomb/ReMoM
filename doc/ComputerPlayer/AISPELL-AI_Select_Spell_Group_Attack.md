AISPELL-AI_Select_Spell_Group_Attack.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickCurse.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickCurse.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_Attack()
                    |-> AI_Has_Targetable_City()

---

# `AI_Select_Spell_Group_Attack` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AI_Select_Spell_Group_Attack` | [AISPELL.c:1723-1947](../../MoM/src/AISPELL.c#L1723-L1947) | When the category picker chose "overland curse / direct damage" (category 7), decide *which* offensive spell the AI casts: weight each known curse by `casting_cost`, zero the city-targeting ones if there is no targetable enemy city, scale by realm suppression, then weighted-random pick and return its `spl_*` id (0 = none). |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:1723](../../MoM/src/AISPELL.c#L1723)) is a Piethawn IDA-C paste, adapted to production. The priority table, both per-spell gates, the targetable-city zeroing, the three suppression blocks, and the dispatch `switch` all match `AI_OVL_PickCurse.asm` — the switch reproduces the 20-entry jump table 1:1, including the slot-9 `return 0` gap (handled by the absent `case 9` → `default`). `players_spell_list` keeps the `- 1`; the asm's `AI_CanTargetCities` is `AI_Has_Targetable_City` in production. `cmake --build --preset MSVC-debug` compiles and links with no errors or warnings. The asm + the `_misc.asm` jump-table bytes are the authority.

## Purpose

Leaf picker for category 7 (overland curse / direct damage) of [`AI_Select_Spell_Group`](AISPELL-AI_OVL_SplCat_Picker.md). It scores each offensive overland spell the wizard knows by `casting_cost / N` (the divisor encodes desirability — cheaper-to-favor spells use a smaller divisor), nulls out the city/terrain-targeting curses when the AI has no enemy city to hit, applies realm-suppression scaling, then `Get_Weighted_Choice` selects a slot and the `switch` maps it to a spell id.

### The priority array

`AI_OVL_SplPriorities` is byte-offset addressed in the asm (**C index = byte/2**). Slot 9 is never assigned (dead) — it maps to a `return 0` entry in the jump table. Weight = `casting_cost / divisor`.

| Slot | Spell | ÷ | Slot | Spell | ÷ |
|---|---|---|---|---|---|
| 1 | Ice_Storm | 10 | 11 | Call_The_Void | 2 |
| 2 | Earthquake | 10 | 12 | Famine | 5 |
| 3 | Spell_Blast | 2 | 13 | Warp_Node | 3 |
| 4 | Stasis | 5 | 14 | Black_Wind | 10 |
| 5 | Time_Stop | 10¹ | 15 | Drain_Power | 5 |
| 6 | Corruption | 5 | 16 | Evil_Presence | 5 |
| 7 | Fire_Storm | 10 | 17 | Subversion | 10 |
| 8 | Raise_Volcano | 10² | 18 | Pestilence | 2 |
| 9 | *(unused)* | — | 19 | Cursed_Lands | 5 |
| 10 | Chaos_Rift | 3 | 20 | Cruel_Unminding | 5 |

¹ Time_Stop only if `mana_reserve >= 3000`. ² Raise_Volcano only if the caster's Armageddon global is inactive.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Spell_Select` | per [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) | Dispatched when `AI_Select_Spell_Group` returns category 7. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AI_OVL_PickCurse.asm`. `players_spell_list` is 1-based (`spells_list - 1`, asm 14) like the sibling pickers.

### Build cost-weighted weights ([1732-1855](../../MoM/src/AISPELL.c#L1732-L1855))

Clear slots 0..49, then per known spell `slot = casting_cost / N`. Two gated entries match the asm:
- **Time_Stop** (slot 5): only if `_players[player_idx].mana_reserve >= 3000` (asm 69).
- **Raise_Volcano** (slot 8): only if `_players[player_idx].Globals[ARMAGEDDON] == 0` (asm 102).

Slot 9 is skipped (asm jumps `+10h` → `+14h`).

### Zero city-targeting curses ([1857-1870](../../MoM/src/AISPELL.c#L1857-L1870))

`if (AI_Has_Targetable_City(player_idx) == 0)` (asm `AI_CanTargetCities`, asm 210-216) zero the ten curses that need an enemy city/territory: Earthquake (2), Corruption (6), Raise_Volcano (8), Chaos_Rift (10), Call_The_Void (11), Famine (12), Warp_Node (13), Evil_Presence (16), Pestilence (18), Cursed_Lands (19). Same set and order as the asm.

### Realm-suppression scaling ([1872-1917](../../MoM/src/AISPELL.c#L1872-L1917))

- `CRP_SPL_IsNatSuppressed()`: slots 1–2 → `*2/3` (asm 227-250).
- `SPL_IsDthSuppressed()`: slots 12–20 → `<20 → 0`, `<50 → /3`, else `/2` (asm 251-298).
- `SPL_IsChsSuppressed()`: slots 7–11 → same banding (asm 299-346).

All index ranges, thresholds, and factors match the asm.

### Pick + dispatch ([1919-1945](../../MoM/src/AISPELL.c#L1919-L1945))

`choice = Get_Weighted_Choice(AI_OVL_SplPriorities, 50);` then `switch(choice)` returns the spell. Switching on `choice` directly, `case N` matches jump-table entry `bx = N-1` (`off_E6470`, in `_misc.asm`). The table's lone `return 0` entry — `bx` 8 (dead slot 9) — is reproduced by the **absence of `case 9`** falling to `default: return spl_NONE;`. **Verified entry-by-entry against the table bytes; faithful 1:1.**

## OG quirks (preserve when reconstructing)

- **Dead slot 9**: never assigned a weight; the jump table carries a `return 0` entry for it (reproduced by the missing `case 9`).
- The `casting_cost / N` divisors are hand-tuned per spell (2/3/5/10), not uniform — preserve each exactly.

## Sub-functions / external calls

- **`Get_Weighted_Choice(int16_t *, int16_t)`** ([random.h:31](../../MoX/src/random.h#L31)) — weighted pick over the 50-slot array.
- **`AI_Has_Targetable_City(player_idx)`** ([AISPELL.c:2946](../../MoM/src/AISPELL.c#L2946)) — asm `AI_CanTargetCities`; gates the city-targeting curses.
- **`CRP_SPL_IsNatSuppressed` / `SPL_IsDthSuppressed` / `SPL_IsChsSuppressed`** — realm-suppression predicates (implemented).
- **Globals:** `AI_OVL_SplPriorities[50]`, `_players[]` (`spells_list`, `mana_reserve`, `Globals[]`), `spell_data_table[]`.

## Related references

- `…\ovr156\AI_OVL_PickCurse.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\_misc.asm` — contains the `off_E6470` jump-table bytes that pin the `bx → spell` map.
- `…\ovr156\AI_OVL_PickCurse.c` — Piethawn IDA-C.
- [AISPELL-AI_Select_Spell_Group.md](AISPELL-AI_Select_Spell_Group.md) — the category picker that dispatches here.
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — sibling leaf picker with the same weighted-array + suppression + jump-table shape.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
