AISPELL-AI_Select_Spell_Group_Disjunction.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickDisj.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickDisj.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_Disjunction()

---

# `AI_Select_Spell_Group_Disjunction` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AI_Select_Spell_Group_Disjunction` | [AISPELL.c:2059-2077](../../MoM/src/AISPELL.c#L2059-L2077) | When the category picker chose "disjunction" (category 5), pick which disjunction spell to cast: prefer **Disjunction True**, else **Disjunction**, else nothing. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:2059](../../MoM/src/AISPELL.c#L2059)) is a direct, faithful reconstruction of `AI_OVL_PickDisj.asm` — a simple two-spell preference, no weighting (the same shape as the [Disenchant picker](AISPELL-AI_Select_Spell_Group_Disenchant.md)). `players_spell_list` is the 1-based `spells_list - 1`. Part of the AISPELL.c translation unit, which `cmake --build --preset MSVC-debug` compiles and links with no errors or warnings. The asm is the authority.

## Purpose

Leaf picker for category 5 (disjunction — strip enemy overland enchantments) of [`AI_Select_Spell_Group`](AISPELL-AI_Select_Spell_Group.md). Like the Disenchant picker, it has **no priority array, no `Get_Weighted_Choice`, and no jump table** — it is a deterministic two-way preference: Disjunction True is strictly better, so it is chosen first when known; otherwise plain Disjunction; otherwise `spl_NONE`.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Spell_Select` | per [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) | Dispatched when `AI_Select_Spell_Group` returns category 5. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AI_OVL_PickDisj.asm`. `players_spell_list` is 1-based (`spells_list - 1`, asm 11) like the sibling pickers.

```c
players_spell_list = (uint8_t *)&_players[player_idx].spells_list[0] - 1;

if (players_spell_list[spl_Disjunction_True] == sls_Known)   return spl_Disjunction_True;   // asm 13-17
if (players_spell_list[spl_Disjunction] == sls_Known)        return spl_Disjunction;        // asm 19-23
return spl_NONE;                                                                            // asm 25-27 (xor ax,ax)
```

Three checks, exactly mirroring the asm's `cmp / jnz` chain: True first, plain second, `spl_NONE` (== 0) as the fallthrough. **Faithful 1:1.**

## OG quirks

None. The function is a straight preference with no dead code, no off-by-ones, and no original-game bugs.

## Sub-functions / external calls

- None. No `Get_Weighted_Choice`, no `AITP_*`/`IDK_AITP_*` target-picker, no suppression predicates — it only reads `_players[player_idx].spells_list[]`.
- **Globals:** `_players[].spells_list` (read).

## Related references

- `…\ovr156\AI_OVL_PickDisj.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AI_OVL_PickDisj.c` — Piethawn IDA-C.
- [AISPELL-AI_Select_Spell_Group.md](AISPELL-AI_Select_Spell_Group.md) — the category picker that dispatches here.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
- [AISPELL-AI_Select_Spell_Group_Disenchant.md](AISPELL-AI_Select_Spell_Group_Disenchant.md) — sibling leaf picker with the identical two-spell-preference shape.
- `AITP_Disjunction.asm` (in ovr156) is a *separate* target-picker, not called by this function.
