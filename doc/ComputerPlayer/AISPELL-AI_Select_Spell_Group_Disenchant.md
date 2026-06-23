AISPELL-AI_Select_Spell_Group_Disenchant.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickDise.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickDise.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_Disenchant()

---

# `AI_Select_Spell_Group_Disenchant` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AI_Select_Spell_Group_Disenchant` | [AISPELL.c:2038-2056](../../MoM/src/AISPELL.c#L2038-L2056) | When the category picker chose "disenchant" (category 4), pick which disenchant spell to cast: prefer **Disenchant True**, else **Disenchant Area**, else nothing. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:2038](../../MoM/src/AISPELL.c#L2038)) is a direct, faithful reconstruction of `AI_OVL_PickDise.asm` — a simple two-spell preference, no weighting. `players_spell_list` is the 1-based `spells_list - 1`. Part of the AISPELL.c translation unit, which `cmake --build --preset MSVC-debug` compiles and links with no errors or warnings. The asm is the authority.

## Purpose

Leaf picker for category 4 (disenchant own enchantments) of [`AI_Select_Spell_Group`](AISPELL-AI_Select_Spell_Group.md). Unlike the Summon / Unit-Enchant / City-Enchant pickers, this one has **no priority array, no `Get_Weighted_Choice`, and no jump table** — it is a deterministic two-way preference: Disenchant True is strictly better (ignores the target's spell-lock), so it is chosen first when known; otherwise Disenchant Area; otherwise `spl_NONE`.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Spell_Select` | per [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) | Dispatched when `AI_Select_Spell_Group` returns category 4. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AI_OVL_PickDise.asm`. `players_spell_list` is 1-based (`spells_list - 1`, asm 11) like the sibling pickers.

```c
players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

if (players_spell_list[spl_Disenchant_True] == sls_Known)   return spl_Disenchant_True;   // asm 13-17
if (players_spell_list[spl_Disenchant_Area] == sls_Known)   return spl_Disenchant_Area;   // asm 19-23
return spl_NONE;                                                                          // asm 25-27 (xor ax,ax)
```

Three checks, exactly mirroring the asm's `cmp / jnz` chain: True first, Area second, `spl_NONE` (== 0) as the fallthrough. **Faithful 1:1.**

## OG quirks

None. The function is a straight preference with no dead code, no off-by-ones, and no original-game bugs.

## Sub-functions / external calls

- None. No `Get_Weighted_Choice`, no `AITP_*` target-picker, no suppression predicates — it only reads `_players[player_idx].spells_list[]`.
- **Globals:** `_players[].spells_list` (read).

## Related references

- `…\ovr156\AI_OVL_PickDise.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AI_OVL_PickDise.c` — Piethawn IDA-C.
- [AISPELL-AI_Select_Spell_Group.md](AISPELL-AI_Select_Spell_Group.md) — the category picker that dispatches here.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
- `AITP_Disenchant.asm` (in ovr156) is a *separate* target-picker, not called by this function.
