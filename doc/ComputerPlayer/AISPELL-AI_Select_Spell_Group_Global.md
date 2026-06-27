AISPELL-AI_Select_Spell_Group_Global.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickGlobal.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickGlobal.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_Global()

---

# `AI_Select_Spell_Group_Global` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AI_Select_Spell_Group_Global` | [AISPELL.c:1740-1958](../../MoM/src/AISPELL.c#L1740-L1958) | When the category picker chose "overland enchantment" (category 9), decide *which* overland enchantment the AI casts: weight each of the 20 (mostly only if its global is not already active) by `casting_cost`, scale by realm suppression, then weighted-random pick and return its `spl_*` id (0 = none). |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:1740](../../MoM/src/AISPELL.c#L1740)) is a Piethawn IDA-C paste, adapted to production. The 20 weights, the per-spell "already-active" gates, the Planar Seal special case, the four suppression blocks, and the dispatch `switch` all match `AI_Select_Spell_Group_Global.asm` — the 20-entry jump table is contiguous (no gaps) and `switch(choice)` mirrors it 1:1. `players_spell_list` keeps the `- 1`; the asm's `AI_MyrrorAdvantage` is `AI_Has_More_Myrror_Cities` in production. `cmake --build --preset MSVC-debug` compiles and links with no errors or warnings. The asm + the `_misc.asm` jump-table bytes are the authority.

## Purpose

Leaf picker for category 9 (overland enchantment) of [`AI_Select_Spell_Group`](AISPELL-AI_OVL_SplCat_Picker.md) — the largest category. It scores each known global-enchantment spell by `casting_cost / 10` (Planar Seal `/20`), but **only if that global is not already active** for the wizard (no point re-casting one that's up), applies realm-suppression scaling, then `Get_Weighted_Choice` selects a slot and the `switch` maps it to a spell id.

### The priority array

`AI_OVL_SplPriorities` is byte-offset addressed in the asm (**C index = byte/2**). Slots 1-20 are used, contiguous (no dead slot). Weight = `casting_cost / 10` unless noted. "Gate" = the extra condition beyond `known`.

| Slot | Spell | ÷ | Gate (beyond known) |
|---|---|---|---|
| 1 | Herb_Mastery | 10 | `Globals[HERB_MASTERY] == 0` |
| 2 | Wind_Mastery | 10 | global inactive |
| 3 | Aura_Of_Majesty | 10 | global inactive |
| 4 | Great_Unsummoning | 10 | *(none — known only)* |
| 5 | Suppress_Magic | 10 | global inactive |
| 6 | Spell_Binding | 10 | *(none — known only)* |
| 7 | Great_Wasting | 10 | global inactive |
| 8 | Chaos_Surge | 10 | global inactive |
| 9 | Doom_Mastery | 10 | global inactive |
| 10 | Meteor_Storms | 10 | global inactive |
| 11 | Armageddon | 10 | global inactive |
| 12 | Just_Cause | 10 | global inactive |
| 13 | Planar_Seal | **20** | global inactive **and** `AI_Has_More_Myrror_Cities != 0` |
| 14 | Holy_Arms | 10 | global inactive |
| 15 | Charm_Of_Life | 10 | global inactive |
| 16 | Crusade | 10 | global inactive |
| 17 | Eternal_Night | 10 | global inactive |
| 18 | Death_Wish | 10 | *(none — known only)* |
| 19 | Zombie_Mastery | 10 | global inactive |
| 20 | Awareness | 10 | global inactive |

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Spell_Select` | per [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) | Dispatched when `AI_Select_Spell_Group` returns category 9. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AI_Select_Spell_Group_Global.asm`. `players_spell_list` is 1-based (`spells_list - 1`, asm 14) like the sibling pickers.

### Build cost-weighted weights ([1749-1854](../../MoM/src/AISPELL.c#L1749-L1854))

Clear slots 0..49, then per spell `slot = casting_cost / 10`. The gate is almost always `known && Globals[X] == 0` — don't bother re-casting an active global. Three are gated on `known` only (Great_Unsummoning, Spell_Binding, Death_Wish). **Planar_Seal** (slot 13) is the special case: `/20`, and additionally requires `AI_Has_More_Myrror_Cities(player_idx) != 0` (asm `AI_MyrrorAdvantage`, asm 203-209). All 20 slots are contiguous — no skipped slot.

### Realm-suppression scaling ([1856-1908](../../MoM/src/AISPELL.c#L1856-L1908))

- `SPL_IsLifeSupressed()`: slots 12–16 → `*2/3` (asm 316-338).
- `CRP_SPL_IsNatSuppressed()`: slot 1 → `*2/3` (asm 340-362).
- `SPL_IsDthSuppressed()`: slots 17–19 → `<20 → 0`, `<50 → /3`, else `/2` (asm 364-410).
- `SPL_IsChsSuppressed()`: slots 7–11 → same banding (asm 412-458).

All index ranges, thresholds, and factors match the asm.

### Pick + dispatch ([1910-1956](../../MoM/src/AISPELL.c#L1910-L1956))

`choice = Get_Weighted_Choice(AI_OVL_SplPriorities, 50);` then `switch(choice)` returns the spell. Switching on `choice` directly, `case N` matches jump-table entry `bx = N-1` (`off_E6074`, in `_misc.asm`): 1→Herb_Mastery … 20→Awareness, `default → spl_NONE`. The 20-entry table is contiguous — **no `return 0` gap**. **Verified entry-by-entry against the table bytes; faithful 1:1.**

## OG quirks

None. Twenty contiguous slots, uniform `/10` (Planar Seal `/20`), no dead entries, no off-by-ones. (Disassembler note: the asm pushes the raw address `0D386h` to `Get_Weighted_Choice` rather than the symbol `AI_OVL_SplPriorities` — that literal *is* the array's address; production uses the symbol. Same array, no divergence.)

## Sub-functions / external calls

- **`Get_Weighted_Choice(int16_t *, int16_t)`** ([random.h:31](../../MoX/src/random.h#L31)) — weighted pick over the 50-slot array.
- **`AI_Has_More_Myrror_Cities(player_idx)`** ([AISPELL.c](../../MoM/src/AISPELL.c)) — asm `AI_MyrrorAdvantage`; gates the Planar Seal weight.
- **`SPL_IsLifeSupressed` / `CRP_SPL_IsNatSuppressed` / `SPL_IsDthSuppressed` / `SPL_IsChsSuppressed`** — realm-suppression predicates (implemented).
- **Globals:** `AI_OVL_SplPriorities[50]`, `_players[]` (`spells_list`, `Globals[]`), `spell_data_table[]`.

## Related references

- `…\ovr156\AI_Select_Spell_Group_Global.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\_misc.asm` — contains the `off_E6074` jump-table bytes that pin the `bx → spell` map.
- `…\ovr156\AI_Select_Spell_Group_Global.c` — Piethawn IDA-C.
- [AISPELL-AI_Select_Spell_Group.md](AISPELL-AI_Select_Spell_Group.md) — the category picker that dispatches here.
- [AISPELL-AI_Select_Spell_Group_Attack.md](AISPELL-AI_Select_Spell_Group_Attack.md) — sibling leaf picker with the same weighted-array + suppression + jump-table shape.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
