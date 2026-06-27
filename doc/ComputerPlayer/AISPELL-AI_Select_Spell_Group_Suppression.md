AISPELL-AI_Select_Spell_Group_Suppression.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickRealmSupr.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickRealmSupr.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_Suppression()

---

# `AI_Select_Spell_Group_Suppression` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AI_Select_Spell_Group_Suppression` | [AISPELL.c:1650-1734](../../MoM/src/AISPELL.c#L1650-L1734) | When the category picker chose "realm suppression" (category 8), decide *which* realm-suppressing global the AI casts: weight each of the 4 by `casting_cost / 10`, scale by realm suppression, then weighted-random pick and return its `spl_*` id (0 = none). |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:1650](../../MoM/src/AISPELL.c#L1650)) is a Piethawn IDA-C paste, adapted to production. The four weights, the three suppression blocks, and the dispatch `switch` all match `AI_OVL_PickRealmSupr.asm` — the 4-entry jump table is contiguous (no gaps) and the `switch(choice)` mirrors it 1:1. `players_spells_list` keeps the 1-based offset. `cmake --build --preset MSVC-debug` compiles and links with no errors or warnings. The asm + the `_misc.asm` jump-table bytes are the authority.

## Purpose

Leaf picker for category 8 (realm suppression) of [`AI_Select_Spell_Group`](AISPELL-AI_OVL_SplCat_Picker.md). The four candidates are the overland enchantments that suppress an opposing magic realm: **Nature's Wrath**, **Life Force**, **Tranquility**, **Evil Omens**. Each known one is weighted by `casting_cost / 10`, scaled by any active realm suppression, then `Get_Weighted_Choice` selects a slot and the `switch` maps it to a spell id. This is the smallest weighted picker in the set — only 4 spells, all contiguous slots, no dead entries.

### The priority array

`AI_OVL_SplPriorities` is byte-offset addressed in the asm (**C index = byte/2**). Slots 1-4 are used; weight = `casting_cost / 10`.

| Slot | Spell | Weight |
|---|---|---|
| 1 | Nature's Wrath | `casting_cost / 10` |
| 2 | Life Force | `casting_cost / 10` |
| 3 | Tranquility | `casting_cost / 10` |
| 4 | Evil Omens | `casting_cost / 10` |

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Spell_Select` | per [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) | Dispatched when `AI_Select_Spell_Group` returns category 8. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AI_OVL_PickRealmSupr.asm`. `players_spells_list` is 1-based (asm 14, `offset spells_list-1`); production spells it `&_players[player_idx].spells_list[-1]`, equivalent to the siblings' `spells_list - 1`.

### Build cost-weighted weights ([1658-1681](../../MoM/src/AISPELL.c#L1658-L1681))

Clear slots 0..49, then for each known spell `slot = casting_cost / 10`: Nature's Wrath (1), Life Force (2), Tranquility (3), Evil Omens (4). All four are flat `/10`, contiguous — no skipped slots, no per-spell gates.

### Realm-suppression scaling ([1683-1716](../../MoM/src/AISPELL.c#L1683-L1716))

- `SPL_IsLifeSupressed()`: slots 2–3 (Life Force, Tranquility) → `*2/3` (asm 64-86).
- `CRP_SPL_IsNatSuppressed()`: slot 1 (Nature's Wrath) → `*2/3` (asm 88-110).
- `SPL_IsDthSuppressed()`: slot 4 (Evil Omens) → `<20 → 0`, `<50 → /3`, else `/2` (asm 112-159).

All index ranges, thresholds, and factors match the asm.

### Pick + dispatch ([1718-1732](../../MoM/src/AISPELL.c#L1718-L1732))

`choice = Get_Weighted_Choice(AI_OVL_SplPriorities, 50);` then `switch(choice)` returns the spell. Switching on `choice` directly, `case N` matches jump-table entry `bx = N-1` (`off_E5B8B`, in `_misc.asm`): 1→Nature's Wrath, 2→Life Force, 3→Tranquility, 4→Evil Omens, `default → 0`. The 4-entry table is contiguous — **no `return 0` gap**, unlike the larger pickers. **Verified entry-by-entry against the table bytes; faithful 1:1.**

## OG quirks

None. Four contiguous slots, uniform `/10` weighting, no dead entries, no off-by-ones. (Disassembler note: the asm pushes the raw address `0D386h` to `Get_Weighted_Choice` rather than the symbol `AI_OVL_SplPriorities` — that literal *is* the array's address; production uses the symbol. Same array, no divergence.)

## Sub-functions / external calls

- **`Get_Weighted_Choice(int16_t *, int16_t)`** ([random.h:31](../../MoX/src/random.h#L31)) — weighted pick over the 50-slot array.
- **`SPL_IsLifeSupressed` / `CRP_SPL_IsNatSuppressed` / `SPL_IsDthSuppressed`** — realm-suppression predicates (implemented). Note `SPL_IsChsSuppressed` is **not** used here (no Chaos-realm suppression spell among the four).
- **Globals:** `AI_OVL_SplPriorities[50]`, `_players[].spells_list`, `spell_data_table[]`.

## Related references

- `…\ovr156\AI_OVL_PickRealmSupr.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\_misc.asm` — contains the `off_E5B8B` jump-table bytes that pin the `bx → spell` map.
- `…\ovr156\AI_OVL_PickRealmSupr.c` — Piethawn IDA-C.
- [AISPELL-AI_Select_Spell_Group.md](AISPELL-AI_Select_Spell_Group.md) — the category picker that dispatches here.
- [AISPELL-AI_Select_Spell_Group_Attack.md](AISPELL-AI_Select_Spell_Group_Attack.md) — sibling leaf picker with the same weighted-array + suppression + jump-table shape.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
